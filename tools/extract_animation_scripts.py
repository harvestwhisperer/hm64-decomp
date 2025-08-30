import csv
import os
import sys
from typing import List

import numpy as np

rom = '../baserom.us.z64'
animation_addresses_csv = './animation_addresses.csv'
output_dir  = "../src/data/animation" 

ANIMATION_TYPE_MAP = {
    0x0000: "ANIM_TYPE_ONE_SHOT",
    0x2000: "ANIM_TYPE_LOOP",
    0x4000: "ANIM_TYPE_DESTROY_ON_END",
}

MASK_METADATA_OFFSET = 0x1FFF
MASK_ANIMATION_TYPE  = 0x6000
MASK_HORIZONTAL_FLIP = 0x8000

FILE_HEADER = '''#include "common.h"

#include "animationData.h"

'''

ARRAY_TEMPLATE = '''u16 {label}AnimationData[] = {{
{body}
}};

'''

def read_slice_as_u16(romf, start: int, end: int) -> np.ndarray:
    romf.seek(start)
    print(f"seeking... ")
    buf = romf.read(end - start)
    dtype = np.dtype(">u2")
    return np.frombuffer(buf, dtype=dtype)

def write_c_file(label: str, entries: List[str], out_dir: str):
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

    out_path = os.path.join(out_dir, f"{label}.c")

    with open(out_path, "w", newline="\n") as f:
        f.write(content)
    print(f"Wrote {out_path}")

def decode_values(u16s: np.ndarray) -> List[str]:
    total = len(u16s)

    metadata_offsets = (u16s & MASK_METADATA_OFFSET).astype(np.uint16)
    animation_types = (u16s & MASK_ANIMATION_TYPE).astype(np.uint16)
    horizontal_flips = (u16s & MASK_HORIZONTAL_FLIP) != 0

    animation_type_names = []
    for tb in animation_types.tolist():
        animation_type_names.append(ANIMATION_TYPE_MAP.get(int(tb), f"(/*UNKNOWN_TYPE*/0x{tb:04X})"))

#     horizontal_flip_strs = ["FLIP_HORIZONTAL" if s else "0" for s in horizontal_flips.tolist()]

    out = []
    for i, (val, meta, tname, flipstr) in enumerate(zip(u16s.tolist(), metadata_offsets.tolist(), animation_type_names, horizontal_flips.tolist())):

        if i % 8 == 0:
            out.append("\t") 

        if val == 0:
            comma = "," if i < total - 1 else ""  # no comma on last element
            out.append(f"0{comma}")
        else:
            comma = "," if i < total - 1 else ""  # no comma on last element
            sstr = "FLIP_HORIZONTAL" if flipstr else "0" 
            out.append(f"PACK_ANIM_DATA({meta}, {tname}, {sstr}){comma}")

        print(f"PACK_ANIM_DATA({meta}, {tname}, {sstr}){comma}")
    return out

def main():
    with open(rom, "rb") as f, open(animation_addresses_csv, newline="") as csvf:

        reader = csv.reader(csvf)

        for line_no, row in enumerate(reader, start=1):

            start_address, end_address, label = [cell.strip() for cell in row]

            print(f"first and start: {start_address}, {end_address}, {label}")

            try:
                start = int(start_address, 16)
                end   = int(end_address, 16)
                vals  = read_slice_as_u16(f, start, end)
                entries = decode_values(vals)
                write_c_file(label, entries, output_dir)
            except Exception as e:
                print(f"Error on CSV line {line_no}: {e}", file=sys.stderr)
                sys.exit(1)

if __name__ == "__main__":
    main()