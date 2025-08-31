from pathlib import Path
import json
import csv
import sys
import numpy as np

rom_path = Path("../baserom.us.z64")
sprite_addresses = Path("./sprite_addresses.csv")
output_dir = Path("../assets/animations" )

ANIM_DT  = np.dtype([       
    ("sprite_count", "<u2"),
    ("frame_duration","u1"),
    ("has_audio",     "u1"),
])
BMP_DT   = np.dtype([       
    ("spritesheet_index","<u2"),
    ("unused",           "<u2"),
    ("anchor_x",         "<i2"),
    ("anchor_y",         "<i2"),
])

def get_label(row):
    if len(row) == 5:
        return row[4]    
    
    elif len(row) == 4:
        return row[3]    

def get_animation_start_address(rom, row):

    asset_offsets_array = np.frombuffer(rom, dtype=np.dtype(">u4"), count=8, offset=int(row[1], 16))

    offset = int(asset_offsets_array[2])
    base = int(row[0], 16)

    return base + offset

def read_metadata(buf: memoryview, meta_addr: int) -> dict:
    
    num_frames = np.frombuffer(buf, dtype=np.dtype("<u2"), count=1, offset=(meta_addr + 4))[0].item()

    unused_header_value = np.frombuffer(buf, dtype=np.dtype("<u2"), count=1, offset=(meta_addr + 6))[0].item()
    p = meta_addr + 8
    frames = []
    
    for _ in range(num_frames):

        animation_metadata = np.frombuffer(buf, dtype=ANIM_DT, count=1, offset=p)[0]
        p += ANIM_DT.itemsize

        n_bmps = int(animation_metadata["sprite_count"])
        if n_bmps > 10:
            print(f"{n_bmps}")
        bmps_arr = np.frombuffer(buf, dtype=BMP_DT, count=n_bmps, offset=p)
        p += n_bmps * BMP_DT.itemsize

        frames.append({
            "animation_metadata": {
                "sprite_count": int(animation_metadata["sprite_count"]),
                "frame_duration": int(animation_metadata["frame_duration"]),
                "has_audio": bool(animation_metadata["has_audio"]),
            },
            "bitmap_metadata": [
                {
                    "spritesheet_index": int(b["spritesheet_index"]),
                    "unused":           int(b["unused"]),
                    "anchor_x":         int(b["anchor_x"]),
                    "anchor_y":         int(b["anchor_y"]),
                }
                for b in bmps_arr
            ]
        })

    return {
        "frame_count": int(num_frames),
        "unused": int(unused_header_value),
        "frames": frames
    }

def extract(buf: memoryview, label: str, table_base: int) -> None:
   
    first_offset = np.frombuffer(buf, dtype=np.dtype(">u4"), count=1, offset=table_base)[0].item()

    count = first_offset // 4
    count -= 1 # last address points to end of data (start of sprite to palette table)

    offsets = np.frombuffer(buf, dtype=np.dtype(">u4"), count=count, offset=table_base)

    outdir = output_dir / label
    outdir.mkdir(parents=True, exist_ok=True)
    
    for idx, off in enumerate(offsets, start=1):

        meta_addr = table_base + int(off)
        data = read_metadata(buf, meta_addr)

        subdir = outdir / f"animation{idx}"
        subdir.mkdir(parents=True, exist_ok=True)

        out_path = subdir / f"animation{idx}.json"
        with open(out_path, "w", encoding="utf-8") as f:
            json.dump(data, f, ensure_ascii=False, indent=2)

def main():

    print("Fetching animation metadata...")

    with open(sprite_addresses, newline="") as csvf:

        rom = memoryview(rom_path.read_bytes())
        reader = csv.reader(csvf)

        for line_no, row in enumerate(reader, start=1):

            row = [cell.strip() for cell in row]

            label = get_label(row)

            if label == -1:
                continue

            # FIXME: hardcoding problematic assets for now 
            if label == 'empty' or label == 'farm-pond' or label == 'naming-screen-2' or label == 'races' or label == 'shop-icons' or label == 'dialogue-button-icons' or label == 'controller-icons':
                continue

            start_address = get_animation_start_address(rom, row)

            try:
                extract(rom, label, start_address)

            except Exception as e:
                print(f"Error: {type(e).__name__}: {e}", file=sys.stderr)
                sys.exit(1)

if __name__ == "__main__":
    main()
