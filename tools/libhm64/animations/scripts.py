"""
Animation script extraction for HM64.

Extracts animation script data from ROM and generates C source files.
Animation scripts contain packed data with animation type flags and metadata offsets.
"""

import argparse
import csv
from pathlib import Path
from typing import List

import numpy as np

from ..common.rom import get_rom, set_rom_path
from ..data import ANIMATION_SCRIPTS_ADDRESSES_CSV

# Animation type flags
ANIMATION_TYPE_MAP = {
    0x0000: "ANIM_TYPE_ONE_SHOT",
    0x2000: "ANIM_TYPE_LOOP",
    0x4000: "ANIM_TYPE_DESTROY_ON_END",
}

# Bit masks for unpacking animation data
MASK_METADATA_OFFSET = 0x1FFF
MASK_ANIMATION_TYPE = 0x6000
MASK_HORIZONTAL_FLIP = 0x8000

# C file templates
FILE_HEADER = '''#include "common.h"

#include "animationScripts.h"

'''

ARRAY_TEMPLATE = '''u16 {label}AnimationScripts[] = {{
{body}

}};

'''

# Default paths
_REPO_DIR = Path(__file__).resolve().parent.parent.parent.parent
ANIMATION_ADDRESSES_CSV = ANIMATION_SCRIPTS_ADDRESSES_CSV
DEFAULT_OUTPUT_DIR = _REPO_DIR / "src" / "data" / "animationScripts"


def read_slice_as_u16(start: int, end: int) -> np.ndarray:
    """Read a slice of ROM as big-endian u16 array."""
    rom = get_rom()
    return np.frombuffer(rom[start:end], dtype=np.dtype(">u2"))


def decode_values(u16s: np.ndarray) -> List[str]:
    """Decode animation script values into C macro calls."""
    total = len(u16s)

    metadata_offsets = (u16s & MASK_METADATA_OFFSET).astype(np.uint16)
    animation_types = (u16s & MASK_ANIMATION_TYPE).astype(np.uint16)
    horizontal_flips = (u16s & MASK_HORIZONTAL_FLIP) != 0

    animation_type_names = []
    for tb in animation_types.tolist():
        animation_type_names.append(
            ANIMATION_TYPE_MAP.get(int(tb), f"(/*UNKNOWN_TYPE*/0x{tb:04X})")
        )

    out = []
    for i, (val, meta, tname, flipstr) in enumerate(
        zip(u16s.tolist(), metadata_offsets.tolist(), animation_type_names, horizontal_flips.tolist())
    ):
        # Group by 8 (per 8 sprite directions)
        if i % 8 == 0:
            out.append("\t")

        comma = "," if i < total - 1 else ""

        if val == 0:
            out.append(f"0{comma}")
        else:
            sstr = "FLIP_HORIZONTAL" if flipstr else "0"
            out.append(f"PACK_ANIM_DATA({meta}, {tname}, {sstr}){comma}")

    return out


def write_c_file(label: str, entries: List[str], output_dir: Path):
    """Write animation script data to a C source file."""
    body_lines = []
    line_buf = []

    for e in entries:
        line_buf.append("\t" + e)
        if len(line_buf) >= 1:
            body_lines.append(" ".join(line_buf))
            line_buf = []

    if line_buf:
        body_lines.append(" ".join(line_buf))

    body = "\n".join(body_lines) if body_lines else "\t/* empty */"

    content = FILE_HEADER + ARRAY_TEMPLATE.format(label=label, body=body)

    output_dir.mkdir(parents=True, exist_ok=True)
    out_path = output_dir / f"{label}.c"

    with open(out_path, "w", newline="\n") as f:
        f.write(content)

    print(f"Wrote {out_path}")


def extract_all(
    csv_path: Path = ANIMATION_ADDRESSES_CSV,
    output_dir: Path = DEFAULT_OUTPUT_DIR
):
    """Extract all animation scripts from ROM."""
    with open(csv_path, newline="", encoding="utf-8") as csvf:
        reader = csv.reader(csvf)

        for line_no, row in enumerate(reader, start=1):
            start_address, end_address, label = [cell.strip() for cell in row]

            try:
                start = int(start_address, 16)
                end = int(end_address, 16)
                vals = read_slice_as_u16(start, end)
                entries = decode_values(vals)
                write_c_file(label, entries, output_dir)
            except Exception as e:
                print(f"Error on CSV line {line_no}: {e}")
                raise


def main():
    parser = argparse.ArgumentParser(description="HM64 Animation Script Extractor")
    parser.add_argument(
        "--output-dir", type=str, default=str(DEFAULT_OUTPUT_DIR),
        help="Output directory for C source files"
    )
    parser.add_argument(
        "--csv", type=str, default=str(ANIMATION_ADDRESSES_CSV),
        help="Path to animation addresses CSV"
    )
    parser.add_argument(
        "--rom", type=str, default=None,
        help="Path to ROM file"
    )

    args = parser.parse_args()

    if args.rom:
        set_rom_path(Path(args.rom))

    extract_all(Path(args.csv), Path(args.output_dir))


if __name__ == "__main__":
    main()
