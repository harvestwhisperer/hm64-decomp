"""Wavetable extractor CLI.

Reads HM64's PtrTablesV2 wave bank from the ROM and emits canonical SDK
source artifacts (`.inst` manifest + per-sound `.aifc` + `.wav` previews):

    python -m libhm64.audio.wavetable.extractor --out assets/audio/wavetable

The ROM offsets mirror config/us/splat.us.yaml's `waveTablePointer` and
`waveTableData` segments; both have `extract: false` because this extractor
takes their place in the build.
"""
import argparse
import sys
from pathlib import Path

from libhm64.audio.wavetable import container, adpcm, aifc, inst_writer
from libhm64.common import rom


# Segment boundaries (must match config/us/splat.us.yaml). The bin files
# include trailing padding bytes that aren't part of the V2 bank format —
# see container.POINTER_TRAILER / DATA_TRAILER for the canonical values
# that pack() always emits at the end.
ROM_POINTER_START = 0xE93080
ROM_POINTER_END   = 0xE99910   # = ROM_DATA_START (next segment)
ROM_DATA_START    = 0xE99910
ROM_DATA_END      = 0xFD8510   # next segment (`opening`)


def extract(out_dir: Path) -> int:
    pointer_bytes = rom.read_bytes(ROM_POINTER_START, ROM_POINTER_END)
    data_bytes    = rom.read_bytes(ROM_DATA_START,    ROM_DATA_END)
    bank = container.parse(pointer_bytes, data_bytes)
    print(f"parsed bank: {bank.name!r}, {len(bank.instruments)} instruments")

    sounds_dir = out_dir / "sounds"
    sounds_dir.mkdir(parents=True, exist_ok=True)

    n_written = 0
    n_skipped = 0
    for idx, inst in enumerate(bank.instruments):
        if not inst.sound.adpcm_bytes:
            n_skipped += 1
            continue
        aifc_path = sounds_dir / f"{idx:03d}.aifc"
        aifc.write_aifc(aifc_path, inst.sound, bank.sample_rate)
        wav_path = sounds_dir / f"{idx:03d}.wav"
        try:
            samples = adpcm.decode(inst.sound.adpcm_bytes, inst.sound.codebook)
            adpcm.write_wav(wav_path, samples, bank.sample_rate)
        except Exception as e:
            print(f"WARN: instrument {idx}: WAV decode failed: {e}", file=sys.stderr)
        n_written += 1

    inst_path = out_dir / f"{bank.name.split('.')[0]}.inst"
    inst_path.write_text(inst_writer.emit_inst_text(bank))

    print(f"wrote {n_written} instruments ({n_skipped} empty slots skipped)")
    print(f"  AIFC + WAV pairs in: {sounds_dir}")
    print(f"  bank manifest:       {inst_path}")
    return 0


def main(argv: list[str]) -> int:
    ap = argparse.ArgumentParser(prog="libhm64.audio.wavetable.extractor")
    ap.add_argument("--out", required=True, type=Path,
                    help="output directory (will be created)")
    ap.add_argument("--rom", type=Path, default=None,
                    help="override ROM path (defaults to baserom.us.z64 in repo root)")
    args = ap.parse_args(argv)

    if args.rom is not None:
        rom.set_rom_path(args.rom)

    return extract(args.out)


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
