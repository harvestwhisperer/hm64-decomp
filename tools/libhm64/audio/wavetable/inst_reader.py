"""Parse .inst text -> Bank.

Round-trips the format emitted by inst_writer.emit_inst_text. The Sound
payloads are NOT loaded here; instead each Instrument carries an aifc_path
attribute (str) which the caller (repacker) resolves via aifc_reader.

HM64-specific metadata (`# [hm64] ...` comment headers) carries the bank
name and sample rate. The opaque inter-segment padding bytes are not
round-tripped via .inst — pack() emits them from constants in container.py.
"""
import re
from dataclasses import dataclass
from typing import Optional

from libhm64.audio.wavetable.container import Bank, Codebook, Instrument, Sound


@dataclass
class _SoundDecl:
    name: str
    aifc_path: str


@dataclass
class _InstDecl:
    name: str
    sound_name: str
    basenote: int
    detune: int


_BANK_NAME_RE       = re.compile(r"^#\s*Bank:\s*(\S+)\s*$")
_HM64_KV_RE         = re.compile(r"^#\s*\[hm64\]\s*(\w+)\s*=\s*(.+?)\s*$")
_SOUND_OPEN_RE      = re.compile(r"^sound\s+(\w+)\s*\{\s*$")
_INST_OPEN_RE       = re.compile(r"^instrument\s+(\w+)\s*\{\s*$")
_BANK_OPEN_RE       = re.compile(r"^bank\s+(\w+)\s*\{\s*$")
_BLOCK_CLOSE_RE     = re.compile(r"^\}\s*$")
_USE_RE             = re.compile(r'^\s*use\s*\(\s*"([^"]+)"\s*\)\s*;\s*$')
_KV_RE              = re.compile(r"^\s*(\w+)\s*=\s*([^;]+?)\s*;\s*$")
_BANK_INSTR_RE      = re.compile(r"^\s*instrument\s*\[\s*(\d+)\s*\]\s*=\s*(\w+)\s*;\s*$")


def parse(text: str) -> Bank:
    """Parse .inst text into a Bank.

    Each Instrument's Sound carries the aifc path in `Sound.aifc_path` (a
    dynamically attached attribute). Caller fills in adpcm_bytes/codebook/loop
    by loading those paths.
    """
    bank_name: Optional[str] = None
    sample_rate = 22050

    sounds: dict[str, _SoundDecl] = {}
    insts: dict[str, _InstDecl] = {}
    bank_index_to_inst_name: dict[int, str] = {}
    declared_count: int = 0

    lines = text.splitlines()
    i = 0
    while i < len(lines):
        line = lines[i]
        stripped = line.strip()

        if not stripped:
            i += 1
            continue

        # Comment headers: bank name + [hm64] metadata.
        m = _BANK_NAME_RE.match(line)
        if m:
            bank_name = m.group(1)
            i += 1
            continue
        m = _HM64_KV_RE.match(line)
        if m:
            k, v = m.group(1), m.group(2)
            if k == "sample_rate":
                sample_rate = int(v)
            i += 1
            continue

        # Other comments (including `# instrument N: empty slot`): track count.
        if stripped.startswith("#"):
            empty_m = re.match(r"^#\s*instrument\s+(\d+)\s*:\s*empty slot", stripped)
            if empty_m:
                declared_count = max(declared_count, int(empty_m.group(1)) + 1)
            i += 1
            continue

        # envelope { ... }: skip until closing brace.
        if stripped.startswith("envelope "):
            i = _skip_block(lines, i)
            continue

        # sound NAME { ... }
        m = _SOUND_OPEN_RE.match(line)
        if m:
            name = m.group(1)
            decl, i = _parse_sound_block(lines, i, name)
            sounds[name] = decl
            continue

        # instrument NAME { ... }
        m = _INST_OPEN_RE.match(line)
        if m:
            name = m.group(1)
            decl, i = _parse_instrument_block(lines, i, name)
            insts[name] = decl
            continue

        # bank NAME { ... }
        m = _BANK_OPEN_RE.match(line)
        if m:
            i += 1
            while i < len(lines):
                bl = lines[i]
                if _BLOCK_CLOSE_RE.match(bl):
                    i += 1
                    break
                bm = _BANK_INSTR_RE.match(bl)
                if bm:
                    idx = int(bm.group(1))
                    bank_index_to_inst_name[idx] = bm.group(2)
                    declared_count = max(declared_count, idx + 1)
                i += 1
            continue

        raise ValueError(f"unrecognized line in .inst at {i+1}: {line!r}")

    if bank_name is None:
        raise ValueError("missing `# Bank: NAME` header")

    # Build Bank.instruments using bank-block ordering. Empty slots get
    # placeholder Instrument with empty Sound (matches container.parse output).
    instruments: list[Instrument] = []
    for idx in range(declared_count):
        inst_name = bank_index_to_inst_name.get(idx)
        if inst_name is None:
            instruments.append(_empty_instrument())
            continue
        if inst_name not in insts:
            raise ValueError(f"bank references undefined instrument {inst_name!r}")
        inst_decl = insts[inst_name]
        if inst_decl.sound_name not in sounds:
            raise ValueError(
                f"instrument {inst_name!r} references undefined sound "
                f"{inst_decl.sound_name!r}"
            )
        sound_decl = sounds[inst_decl.sound_name]
        sound = Sound(adpcm_bytes=b"", codebook=Codebook(0, 0, []))
        sound.aifc_path = sound_decl.aifc_path   # type: ignore[attr-defined]
        instruments.append(Instrument(basenote=inst_decl.basenote,
                                       detune=inst_decl.detune, sound=sound))

    return Bank(
        name=bank_name,
        sample_rate=sample_rate,
        instruments=instruments,
    )


def _empty_instrument() -> Instrument:
    return Instrument(basenote=0, detune=0,
                      sound=Sound(adpcm_bytes=b"", codebook=Codebook(0, 0, [])))


def _skip_block(lines: list[str], start: int) -> int:
    """Return index of the line just past the matching `}`."""
    i = start + 1
    while i < len(lines):
        if _BLOCK_CLOSE_RE.match(lines[i]):
            return i + 1
        i += 1
    raise ValueError(f"unterminated block starting at line {start+1}")


def _parse_sound_block(lines: list[str], start: int, name: str) -> tuple[_SoundDecl, int]:
    aifc_path: Optional[str] = None
    i = start + 1
    while i < len(lines):
        line = lines[i]
        if _BLOCK_CLOSE_RE.match(line):
            i += 1
            break
        m = _USE_RE.match(line)
        if m:
            aifc_path = m.group(1)
            i += 1
            continue
        # Other key-value lines (envelope = ...) are ignored — basenote and
        # detune live on the instrument block, not the sound block.
        i += 1
    if aifc_path is None:
        raise ValueError(f"sound {name!r} missing use(...) at line {start+1}")
    return _SoundDecl(name=name, aifc_path=aifc_path), i


def _parse_instrument_block(lines: list[str], start: int, name: str) -> tuple[_InstDecl, int]:
    sound_name: Optional[str] = None
    basenote: Optional[int] = None
    detune: Optional[int] = None
    i = start + 1
    while i < len(lines):
        line = lines[i]
        if _BLOCK_CLOSE_RE.match(line):
            i += 1
            break
        m = _KV_RE.match(line)
        if m:
            k, v = m.group(1), m.group(2)
            if k == "sound":
                sound_name = v
            elif k == "basenote":
                basenote = int(v)
            elif k == "detune":
                detune = int(v)
            i += 1
            continue
        i += 1
    if sound_name is None or basenote is None or detune is None:
        raise ValueError(f"instrument {name!r} missing sound/basenote/detune at line {start+1}")
    return _InstDecl(name=name, sound_name=sound_name, basenote=basenote, detune=detune), i
