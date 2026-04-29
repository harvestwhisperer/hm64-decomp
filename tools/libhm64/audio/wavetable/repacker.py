"""Wavetable repacker CLI (inverse of extractor).

    python -m libhm64.audio.wavetable.repacker \\
        --inst        assets/audio/wavetable/BOKUJO01.inst \\
        --sounds-dir  assets/audio/wavetable/sounds \\
        --out-pointer bin/audio/waveTablePointer.bin \\
        --out-data    bin/audio/waveTableData.bin
        [--modding]

Reads .inst + per-sound .aifc files, packs them back into the PtrTablesV2
binaries the build links into the ROM.

Pass --modding (dev / dev-qol builds) to omit the HM64-specific SDK padding
bytes from the bin output. The matching build (master) needs the trailers
to byte-match the original ROM and so does NOT pass --modding.
"""
import argparse
import sys
from pathlib import Path

from libhm64.audio.wavetable import aifc_reader, container, inst_reader


def repack(inst_path: Path, sounds_dir: Path,
           out_pointer: Path, out_data: Path,
           modding: bool = False) -> int:
    bank = inst_reader.parse(inst_path.read_text())
    print(f"parsed bank: {bank.name!r}, {len(bank.instruments)} instruments")

    n_loaded = 0
    for inst in bank.instruments:
        if not hasattr(inst.sound, "aifc_path"):
            continue
        # aifc_path in the .inst is sounds_subdir-relative (e.g. "sounds/000.aifc").
        # Resolve relative to the .inst directory, then fall back to sounds_dir
        # if the leading directory is the sounds-subdir name itself.
        rel = Path(inst.sound.aifc_path)
        candidate = inst_path.parent / rel
        if not candidate.exists():
            candidate = sounds_dir / rel.name
        inst.sound = aifc_reader.read_file(candidate)
        n_loaded += 1
    print(f"loaded {n_loaded} aifc files")

    pointer_bytes, data_bytes = container.pack(bank, include_trailers=not modding)
    out_pointer.parent.mkdir(parents=True, exist_ok=True)
    out_data.parent.mkdir(parents=True, exist_ok=True)
    out_pointer.write_bytes(pointer_bytes)
    out_data.write_bytes(data_bytes)
    print(f"wrote {out_pointer} ({len(pointer_bytes)} bytes"
          f"{' — modding, no SDK trailers' if modding else ''})")
    print(f"wrote {out_data} ({len(data_bytes)} bytes)")
    return 0


def main(argv: list[str]) -> int:
    ap = argparse.ArgumentParser(prog="libhm64.audio.wavetable.repacker")
    ap.add_argument("--inst", required=True, type=Path,
                    help="path to BOKUJO01.inst")
    ap.add_argument("--sounds-dir", required=True, type=Path,
                    help="directory holding the per-instrument .aifc files")
    ap.add_argument("--out-pointer", required=True, type=Path,
                    help="output path for waveTablePointer.bin")
    ap.add_argument("--out-data", required=True, type=Path,
                    help="output path for waveTableData.bin")
    ap.add_argument("--modding", action="store_true",
                    help="omit HM64-specific SDK padding (dev/dev-qol builds)")
    args = ap.parse_args(argv)
    return repack(args.inst, args.sounds_dir, args.out_pointer, args.out_data,
                  modding=args.modding)


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
