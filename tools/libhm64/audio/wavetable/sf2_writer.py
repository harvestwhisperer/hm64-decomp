"""Bank -> SF2 (SoundFont 2.04)

  layout="game"  (pairs with converter's non-GM mode: game_inst = msg.program)
      One preset per wave-bank instrument. The preset's (bank, program) encodes
      the game instrument index P -- the operand of the seq SET_INSTRUMENT
      opcode -- as bank = P // 128, program = P % 128 (there are 147 game
      instruments, so they spill past the 128-program limit into bank 1).
      Names come from config.GAME_INSTRUMENTS where known, else "inst_NNN".

  layout="gm"    (pairs with converter's --gm mode: get_game_instrument_from_gm)
      One preset per General MIDI program (0-127), each a key-range split that
      mirrors config.GM_TO_GAME so the DAW preview tracks the converter's
      per-note instrument switching. Plus a percussion preset (bank 128) built
      from config.GM_DRUM_TO_GAME.

Pitch model (derived from lib/libmus/src/player.c):
    base_note = note + (s8(basenote) - BASEOFFSET) + s8(detune)/100      [semitones]
    pitch_ratio = 2^(base_note / 12)                                     (BASEOFFSET=48)

So a sample plays at its recorded rate when note = 48 - s8(basenote).
converter.py writes  note = playedKey - 12  (its documented octave transpose,
converter.py:582), so a DAW preview matches in-game pitch when the SF2 sample
root key is (48 + 12) - s8(basenote) = 60 - s8(basenote), with pitchCorrection
= s8(detune) cents.
"""

import importlib.util
import struct
from pathlib import Path
from typing import List, Optional

from libhm64.audio.wavetable import adpcm
from libhm64.audio.wavetable.container import Bank


# ---- SF2 generator operators (SoundFont 2.04 spec, section 8.1.3) ----------
GEN_KEY_RANGE = 43
GEN_COARSE_TUNE = 51
GEN_SAMPLE_MODES = 54
GEN_INSTRUMENT = 41
GEN_SAMPLE_ID = 53
GEN_OVERRIDING_ROOT_KEY = 58

GUARD_FRAMES = 46            # mandatory zero frames trailing every sample
BASEOFFSET = 48             # player.c BASEOFFSET (sample base-note default = C4)
DEFAULT_ROOT_TRANSPOSE = 12  # converter.py writes note = playedKey - 12
PROGRAMS_PER_BANK = 128

# In-game drums always trigger at this fixed note (converter.py:313).
DRUM_FIXED_NOTE = 0x24


def _s8(b: int) -> int:
    """Interpret a u8 the way libmus's U8_TO_FLOAT does (signed two's complement)."""
    return b - 256 if b >= 128 else b


def _clamp(v: int, lo: int, hi: int) -> int:
    return lo if v < lo else hi if v > hi else v


# Standard General MIDI Level 1 program names (program 0..127).
GM_PROGRAM_NAMES = [
    "Acoustic Grand Piano", "Bright Acoustic Piano", "Electric Grand Piano",
    "Honky-tonk Piano", "Electric Piano 1", "Electric Piano 2", "Harpsichord",
    "Clavinet", "Celesta", "Glockenspiel", "Music Box", "Vibraphone", "Marimba",
    "Xylophone", "Tubular Bells", "Dulcimer", "Drawbar Organ", "Percussive Organ",
    "Rock Organ", "Church Organ", "Reed Organ", "Accordion", "Harmonica",
    "Tango Accordion", "Acoustic Guitar (nylon)", "Acoustic Guitar (steel)",
    "Electric Guitar (jazz)", "Electric Guitar (clean)", "Electric Guitar (muted)",
    "Overdriven Guitar", "Distortion Guitar", "Guitar Harmonics", "Acoustic Bass",
    "Electric Bass (finger)", "Electric Bass (pick)", "Fretless Bass", "Slap Bass 1",
    "Slap Bass 2", "Synth Bass 1", "Synth Bass 2", "Violin", "Viola", "Cello",
    "Contrabass", "Tremolo Strings", "Pizzicato Strings", "Orchestral Harp",
    "Timpani", "String Ensemble 1", "String Ensemble 2", "Synth Strings 1",
    "Synth Strings 2", "Choir Aahs", "Voice Oohs", "Synth Voice", "Orchestra Hit",
    "Trumpet", "Trombone", "Tuba", "Muted Trumpet", "French Horn", "Brass Section",
    "Synth Brass 1", "Synth Brass 2", "Soprano Sax", "Alto Sax", "Tenor Sax",
    "Baritone Sax", "Oboe", "English Horn", "Bassoon", "Clarinet", "Piccolo",
    "Flute", "Recorder", "Pan Flute", "Blown Bottle", "Shakuhachi", "Whistle",
    "Ocarina", "Lead 1 (square)", "Lead 2 (sawtooth)", "Lead 3 (calliope)",
    "Lead 4 (chiff)", "Lead 5 (charang)", "Lead 6 (voice)", "Lead 7 (fifths)",
    "Lead 8 (bass + lead)", "Pad 1 (new age)", "Pad 2 (warm)", "Pad 3 (polysynth)",
    "Pad 4 (choir)", "Pad 5 (bowed)", "Pad 6 (metallic)", "Pad 7 (halo)",
    "Pad 8 (sweep)", "FX 1 (rain)", "FX 2 (soundtrack)", "FX 3 (crystal)",
    "FX 4 (atmosphere)", "FX 5 (brightness)", "FX 6 (goblins)", "FX 7 (echoes)",
    "FX 8 (sci-fi)", "Sitar", "Banjo", "Shamisen", "Koto", "Kalimba", "Bag pipe",
    "Fiddle", "Shanai", "Tinkle Bell", "Agogo", "Steel Drums", "Woodblock",
    "Taiko Drum", "Melodic Tom", "Synth Drum", "Reverse Cymbal", "Guitar Fret Noise",
    "Breath Noise", "Seashore", "Bird Tweet", "Telephone Ring", "Helicopter",
    "Applause", "Gunshot",
]


def _load_music_config():
    """Load tools/modding/music/config.py (the GM<->game mapping tables).

    Loaded by path under a unique module name so the generic "config" name in
    that file doesn't collide with anything in sys.modules.
    """
    cfg_path = (Path(__file__).resolve().parents[3]
                / "modding" / "music" / "config.py")
    if not cfg_path.exists():
        raise FileNotFoundError(
            f"GM layout needs the music converter's config at {cfg_path}, "
            "but it was not found."
        )
    spec = importlib.util.spec_from_file_location("hm64_music_config", cfg_path)
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    return mod


class _Sf2Builder:
    """Accumulates SF2 samples / instruments / presets, then serializes."""

    def __init__(self, name: str, sample_rate: int):
        self.bank_name = name
        self.sample_rate = sample_rate
        self._pcm = bytearray()        # smpl chunk bytes (int16 LE + guard frames)
        self._frames = 0               # current length of _pcm in sample frames
        self._samples: List[dict] = []     # shdr records
        self._sample_of_bank: dict = {}    # bank index -> shdr index (dedup)
        self._instruments: List[tuple] = []  # (name, [zone, ...])
        self._presets: List[dict] = []        # {name, bank, program, inst}

    # -- model construction --------------------------------------------------
    def sample_for(self, bank: Bank, bank_idx: int) -> Optional[int]:
        """Decode and register the sample for game/bank instrument `bank_idx`.

        Returns its shdr index, or None for an empty slot. Cached per index.
        """
        if bank_idx in self._sample_of_bank:
            return self._sample_of_bank[bank_idx]
        inst = bank.instruments[bank_idx]
        if not inst.sound.adpcm_bytes:
            self._sample_of_bank[bank_idx] = None
            return None
        pcm = adpcm.decode(inst.sound.adpcm_bytes, inst.sound.codebook)
        if not pcm:
            self._sample_of_bank[bank_idx] = None
            return None

        start = self._frames
        self._pcm += struct.pack(f"<{len(pcm)}h", *pcm)
        end = start + len(pcm)
        self._pcm += b"\x00\x00" * GUARD_FRAMES
        self._frames = end + GUARD_FRAMES

        loop = inst.sound.loop
        if loop is not None:
            loop_start = start + loop.start
            loop_end = start + loop.end
        else:
            # No loop: keep the points in-range (ignored, since sampleModes is
            # unset) so strict loaders like Polyphone don't flag them.
            loop_start, loop_end = start, end

        sidx = len(self._samples)
        self._samples.append(dict(
            name=f"inst_{bank_idx:03d}",
            start=start, end=end,
            loop_start=loop_start, loop_end=loop_end,
            has_loop=loop is not None,
            corr=_clamp(_s8(inst.detune), -99, 99),
        ))
        self._sample_of_bank[bank_idx] = sidx
        return sidx

    def add_instrument(self, name: str, zones: List[dict]) -> int:
        self._instruments.append((name, zones))
        return len(self._instruments) - 1

    def add_preset(self, name: str, bank: int, program: int, inst_idx: int) -> None:
        self._presets.append(dict(name=name, bank=bank, program=program,
                                  inst=inst_idx))

    @staticmethod
    def zone(sample_idx: int, root_true: int,
             key_lo: Optional[int] = None, key_hi: Optional[int] = None,
             has_loop: bool = False) -> dict:
        return dict(sample=sample_idx, root_true=root_true,
                    key_lo=key_lo, key_hi=key_hi, has_loop=has_loop)

    # -- serialization -------------------------------------------------------
    def _igen_for_zone(self, z: dict) -> bytes:
        """Instrument generator list for one zone (sampleID MUST come last)."""
        out = bytearray()
        if z["key_lo"] is not None:
            # keyRange genAmount is two bytes: low, high.
            out += struct.pack("<HBB", GEN_KEY_RANGE,
                               _clamp(z["key_lo"], 0, 127), _clamp(z["key_hi"], 0, 127))
        root_clamped = _clamp(z["root_true"], 0, 127)
        out += struct.pack("<HH", GEN_OVERRIDING_ROOT_KEY, root_clamped)
        coarse = root_clamped - z["root_true"]   # absorb out-of-range root
        if coarse:
            out += struct.pack("<Hh", GEN_COARSE_TUNE, coarse)
        if z["has_loop"]:
            out += struct.pack("<HH", GEN_SAMPLE_MODES, 1)   # loop continuously
        out += struct.pack("<HH", GEN_SAMPLE_ID, z["sample"])
        return bytes(out)

    def _pdta(self) -> bytes:
        presets = self._presets
        instruments = self._instruments
        samples = self._samples

        # phdr / pbag / pgen ---------------------------------------------------
        phdr = bytearray()
        for i, p in enumerate(presets):
            phdr += struct.pack("<20sHHHIII", _name(p["name"]),
                                p["program"], p["bank"], i, 0, 0, 0)
        phdr += struct.pack("<20sHHHIII", _name("EOP"), 0, 0, len(presets), 0, 0, 0)

        pbag = bytearray()
        for i in range(len(presets)):
            pbag += struct.pack("<HH", i, 0)        # one zone per preset; no mods
        pbag += struct.pack("<HH", len(presets), 0)

        pgen = bytearray()
        for p in presets:
            pgen += struct.pack("<HH", GEN_INSTRUMENT, p["inst"])
        pgen += struct.pack("<HH", 0, 0)

        pmod = struct.pack("<HHhHH", 0, 0, 0, 0, 0)  # terminal record only (10 bytes)

        # inst / ibag / igen ---------------------------------------------------
        inst = bytearray()
        ibag = bytearray()
        igen = bytearray()
        zone_cursor = 0
        gen_cursor = 0
        for name, zones in instruments:
            inst += struct.pack("<20sH", _name(name), zone_cursor)
            for z in zones:
                ibag += struct.pack("<HH", gen_cursor, 0)
                gens = self._igen_for_zone(z)
                igen += gens
                gen_cursor += len(gens) // 4
                zone_cursor += 1
        inst += struct.pack("<20sH", _name("EOI"), zone_cursor)
        ibag += struct.pack("<HH", gen_cursor, 0)
        igen += struct.pack("<HH", 0, 0)

        imod = struct.pack("<HHhHH", 0, 0, 0, 0, 0)

        # shdr -----------------------------------------------------------------
        shdr = bytearray()
        for s in samples:
            shdr += struct.pack("<20sIIIIIBbHH", _name(s["name"]),
                                s["start"], s["end"], s["loop_start"], s["loop_end"],
                                self.sample_rate, 60, s["corr"], 0, 1)  # 1 = monoSample
        shdr += struct.pack("<20sIIIIIBbHH", _name("EOS"), 0, 0, 0, 0, 0, 0, 0, 0, 0)

        body = (_ck(b"phdr", phdr) + _ck(b"pbag", pbag) + _ck(b"pmod", pmod)
                + _ck(b"pgen", pgen) + _ck(b"inst", inst) + _ck(b"ibag", ibag)
                + _ck(b"imod", imod) + _ck(b"igen", igen) + _ck(b"shdr", shdr))
        return _list(b"pdta", body)

    def serialize(self) -> bytes:
        info = _list(b"INFO",
                     _ck(b"ifil", struct.pack("<HH", 2, 4))
                     + _ck(b"isng", _zstr("EMU8000"))
                     + _ck(b"INAM", _zstr(self.bank_name)))
        sdta = _list(b"sdta", _ck(b"smpl", bytes(self._pcm)))
        pdta = self._pdta()
        return _ck(b"RIFF", b"sfbk" + info + sdta + pdta)


# ---- RIFF helpers ----------------------------------------------------------
def _ck(tag: bytes, data: bytes) -> bytes:
    pad = b"\x00" if (len(data) & 1) else b""
    return tag + struct.pack("<I", len(data)) + data + pad


def _list(list_type: bytes, body: bytes) -> bytes:
    return _ck(b"LIST", list_type + body)


def _name(s: str) -> bytes:
    return s.encode("ascii", "replace")[:20]


def _zstr(s: str) -> bytes:
    b = s.encode("ascii", "replace") + b"\x00"
    if len(b) & 1:
        b += b"\x00"
    return b


# ---- layout builders -------------------------------------------------------
def _build_game(b: _Sf2Builder, bank: Bank, root_transpose: int) -> None:
    base = BASEOFFSET + root_transpose
    for idx, inst in enumerate(bank.instruments):
        sidx = b.sample_for(bank, idx)
        if sidx is None:
            continue
        root = base - _s8(inst.basenote)
        zone = b.zone(sidx, root, has_loop=inst.sound.loop is not None)
        from_cfg = _GAME_NAMES.get(idx)
        name = from_cfg if from_cfg else f"inst_{idx:03d}"
        inst_idx = b.add_instrument(name, [zone])
        b.add_preset(name, idx // PROGRAMS_PER_BANK, idx % PROGRAMS_PER_BANK, inst_idx)


def _build_gm(b: _Sf2Builder, bank: Bank, cfg, root_transpose: int) -> None:
    base = BASEOFFSET + root_transpose

    def melodic_root(gi: int) -> int:
        return base - _s8(bank.instruments[gi].basenote)

    # Melodic presets, one per GM program, mirroring config.GM_TO_GAME.
    for prog in range(128):
        mappings = cfg.GM_TO_GAME.get(prog)
        if not mappings:
            continue
        zones = []
        for gi, lo, hi in mappings:
            sidx = b.sample_for(bank, gi)
            if sidx is None:
                continue
            # GM_TO_GAME ranges are in post-transpose notes; the composer plays
            # playedKey = note + root_transpose, so shift the key range up.
            zones.append(b.zone(sidx, melodic_root(gi),
                                key_lo=lo + root_transpose, key_hi=hi + root_transpose,
                                has_loop=bank.instruments[gi].sound.loop is not None))
        if not zones:
            continue
        # Close gaps at the keyboard extremes (the transpose shift opens them).
        zones.sort(key=lambda z: z["key_lo"])
        zones[0]["key_lo"] = 0
        zones[-1]["key_hi"] = 127
        name = GM_PROGRAM_NAMES[prog] if prog < len(GM_PROGRAM_NAMES) else f"GM {prog}"
        inst_idx = b.add_instrument(name, zones)
        b.add_preset(name, 0, prog, inst_idx)

    # Percussion preset (bank 128) from config.GM_DRUM_TO_GAME.
    drum_zones = []
    for drum_note, gi in sorted(cfg.GM_DRUM_TO_GAME.items()):
        sidx = b.sample_for(bank, gi)
        if sidx is None:
            continue
        # In-game the drum always sounds at DRUM_FIXED_NOTE on gi; pick a root so
        # that playing the GM drum key in the DAW previews that same pitch.
        root = drum_note - (DRUM_FIXED_NOTE + _s8(bank.instruments[gi].basenote) - BASEOFFSET)
        drum_zones.append(b.zone(sidx, root, key_lo=drum_note, key_hi=drum_note,
                                 has_loop=bank.instruments[gi].sound.loop is not None))
    if drum_zones:
        inst_idx = b.add_instrument("GM Drum Kit", drum_zones)
        b.add_preset("Drum Kit", PROGRAMS_PER_BANK, 0, inst_idx)


# config.GAME_INSTRUMENTS is loaded lazily; cached here once resolved.
_GAME_NAMES: dict = {}


def build_sf2(bank: Bank, layout: str = "gm",
              root_transpose: int = DEFAULT_ROOT_TRANSPOSE) -> bytes:
    """Render `bank` as SF2 bytes. layout is "gm" or "game"."""
    global _GAME_NAMES
    if layout not in ("gm", "game"):
        raise ValueError(f"layout must be 'gm' or 'game', got {layout!r}")

    builder = _Sf2Builder(bank.name.split(".")[0], bank.sample_rate)
    if layout == "game":
        if not _GAME_NAMES:
            try:
                _GAME_NAMES = dict(_load_music_config().GAME_INSTRUMENTS)
            except FileNotFoundError:
                _GAME_NAMES = {}   # names are cosmetic; fall back to inst_NNN
        _build_game(builder, bank, root_transpose)
    else:
        _build_gm(builder, bank, _load_music_config(), root_transpose)
    return builder.serialize()


# ---- standalone CLI --------------------------------------------------------
# Repo root, so default source paths don't depend on the caller's CWD.
_REPO_ROOT = Path(__file__).resolve().parents[4]
_DEFAULT_INST = _REPO_ROOT / "assets" / "audio" / "wavetable" / "BOKUJO01.inst"
_DEFAULT_POINTER = _REPO_ROOT / "bin" / "audio" / "waveTablePointer.bin"
_DEFAULT_DATA = _REPO_ROOT / "bin" / "audio" / "waveTableData.bin"


def _bank_from_bins(pointer_bin: Path, data_bin: Path) -> Bank:
    from libhm64.audio.wavetable import container
    return container.parse(pointer_bin.read_bytes(), data_bin.read_bytes())


def _bank_from_inst(inst_path: Path) -> Bank:
    from libhm64.audio.wavetable import inst_reader, aifc_reader
    bank = inst_reader.parse(inst_path.read_text())
    for inst in bank.instruments:
        if hasattr(inst.sound, "aifc_path"):
            inst.sound = aifc_reader.read_file(inst_path.parent / inst.sound.aifc_path)
    return bank


def main(argv: List[str]) -> int:
    import argparse
    ap = argparse.ArgumentParser(
        prog="libhm64.audio.wavetable.sf2_writer",
        description="Build an SF2 soundfont from the HM64 wave bank. "
                    "By default reads the extracted assets/audio/wavetable/ "
                    "(run `make extract-wavetable` first).")
    ap.add_argument("--out", required=True, type=Path, help="output .sf2 path")
    ap.add_argument("--layout", choices=("gm", "game"), default="gm",
                    help="preset layout (default: gm)")
    ap.add_argument("--root-transpose", type=int, default=DEFAULT_ROOT_TRANSPOSE,
                    help="semitone transpose baked into sample roots (default 12)")
    src = ap.add_mutually_exclusive_group()
    src.add_argument("--inst", type=Path, default=None,
                     help=f"build from an extracted .inst manifest (+ its sounds/); "
                          f"default {_DEFAULT_INST}")
    src.add_argument("--pointer", type=Path, default=None,
                     help="build from the split bins instead; path to "
                          "waveTablePointer.bin (implies --data)")
    ap.add_argument("--data", type=Path, default=None,
                    help=f"waveTableData.bin to pair with --pointer (default {_DEFAULT_DATA})")
    args = ap.parse_args(argv)

    if args.pointer is not None:
        data = args.data if args.data is not None else _DEFAULT_DATA
        bank = _bank_from_bins(args.pointer, data)
    else:
        inst = args.inst if args.inst is not None else _DEFAULT_INST
        if not inst.exists():
            ap.error(f"{inst} not found -- run `make extract-wavetable` first, "
                     f"or pass --inst / --pointer explicitly.")
        bank = _bank_from_inst(inst)

    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_bytes(build_sf2(bank, layout=args.layout,
                                   root_transpose=args.root_transpose))
    print(f"wrote {args.out} (layout={args.layout}, "
          f"{len(bank.instruments)} bank instruments)")
    return 0


if __name__ == "__main__":
    import sys
    sys.exit(main(sys.argv[1:]))
