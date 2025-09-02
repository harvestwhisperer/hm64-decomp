import csv
import json
import numpy as np
import re
import sys
from typing import Dict, Any, List, Tuple

from hm64_sprite_utilities import *

from n64img.image import CI4, CI8, Image
from pathlib import Path
from PIL import Image

METADATA_DIR = Path("../assets/animations/")
ROM_PATH = Path("../baserom.us.z64")
SPRITE_ADDRESSES_CSV_PATH = Path("./sprite_addresses.csv")

BASE_DIR = Path("../assets/animations")
OUTPUT_DIR = Path("../assets/animations" )

TICK_MS = 10   # multiply JSON frame_duration by this to get milliseconds (set 1 if already ms)
LOOP = 0       # 0 = loop forever
DISPOSAL = 2   # 2 = restore to background

PNG_PATTERN = re.compile(r".*frame-(\d+)-(\d+)\.png$", re.IGNORECASE)
# Captures: frame_num (group 1), layer_num (group 2)

rom = None

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


def set_rom():
    global rom
    rom = memoryview(ROM_PATH.read_bytes())

def build_filename(base_filename: str, frame_count: int, bitmap_count: int) -> str:
    return base_filename + f"-frame-{frame_count}" + f"-{bitmap_count}"

def get_label(row: List[str]) -> str:
    if len(row) == 5:
        return row[4]    
    
    elif len(row) == 4:
        return row[3]

    else:
        return "Invalid row"

def get_animation_start_address(row: List[str]) -> int:

    asset_offsets_array = np.frombuffer(rom, dtype=np.dtype(">u4"), count=8, offset=int(row[1], 16))

    offset = int(asset_offsets_array[2])
    base = int(row[0], 16)

    return base + offset

def read_metadata(meta_addr: int) -> Dict[str, Any]:
    
    num_frames = np.frombuffer(rom, dtype=np.dtype("<u2"), count=1, offset=(meta_addr + 4))[0].item()

    unused_header_value = np.frombuffer(rom, dtype=np.dtype("<u2"), count=1, offset=(meta_addr + 6))[0].item()
    
    p = meta_addr + 8
    frames = []
    
    for _ in range(num_frames):

        animation_metadata = np.frombuffer(rom, dtype=ANIM_DT, count=1, offset=p)[0]
        p += ANIM_DT.itemsize

        n_bmps = int(animation_metadata["sprite_count"])

        bmps_arr = np.frombuffer(rom, dtype=BMP_DT, count=n_bmps, offset=p)
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

def extract(label: str, table_base: int) -> None:
   
    first_offset = np.frombuffer(rom, dtype=np.dtype(">u4"), count=1, offset=table_base)[0].item()

    count = first_offset // 4
    count -= 1 # last address points to end of data (start of sprite to palette table)

    offsets = np.frombuffer(rom, dtype=np.dtype(">u4"), count=count, offset=table_base)

    outdir = OUTPUT_DIR / label
    outdir.mkdir(parents=True, exist_ok=True)

    previous_off = -1

    for idx, off in enumerate(offsets, start=1):

        # skip repeats
        if (off == previous_off):
            continue

        if (off == offsets[-1]):
            break

        previous_off = off

        meta_addr = table_base + int(off)
        data = read_metadata(meta_addr)

        subdir = outdir / f"animation{idx}"
        subdir.mkdir(parents=True, exist_ok=True)

        out_path = subdir / f"animation{idx}.json"
        with open(out_path, "w", encoding="utf-8") as f:
            json.dump(data, f, ensure_ascii=False, indent=2)
    
    return

def write_frame_texture(sprite_index: int, frame_count: int, bitmap_count: int, output_path: Path, base_filename: str) -> None:

    result, asset_type = check_asset_addresses_exist(output_path.parent.name)

    filename = build_filename(base_filename, frame_count, bitmap_count)

    write_texture_from_csv_row_and_index(result, sprite_index, filename, output_path)

    return

def compose_frame_anchored(layer_imgs: List[Image.Image], anchors: List[Tuple[int, int]]) -> Image.Image:
    """
    layer_imgs: list[Image.Image]  (RGBA)
    anchors:    list[tuple[int,int]]   (anchor_x, anchor_y), same order as layer_imgs

    Returns a composed RGBA frame where each layer's anchor lands at the same pivot.
    """

    # compute each layer's rectangle in pivot space (anchor at 0,0)
    rects = []
    for im, (ax, ay) in zip(layer_imgs, anchors):
        w, h = im.size
        left = -ax
        top  = -ay
        rects.append((left, top, left + w, top + h))

    # set overall bounding box
    min_left  = min(r[0] for r in rects)
    min_top   = min(r[1] for r in rects)
    max_right = max(r[2] for r in rects)
    max_bot   = max(r[3] for r in rects)

    W = int(max_right - min_left)
    H = int(max_bot   - min_top)

    # paste each layer shifted so everything is positive
    canvas = Image.new("RGBA", (W, H), (0, 0, 0, 0))
    for im, (left, top, _, _) in zip(layer_imgs, rects):
        x = int(left - min_left)
        y = int(top  - min_top)
        canvas.paste(im, (x, y), im)  # use image as mask to respect alpha

    return canvas

def build_gif_for_anim_dir(
    anim_dir: Path,
    *,
    tick_ms: int = 20,       # multiply JSON frame_duration by this (set 1 if already ms)
    loop: int = 0,           # 0 = loop forever
    disposal: int = 2,       # 2 = restore to background (safer when frames don't cover canvas)
    transparency_index: int = 0
) -> bool:    
    json_path = next(anim_dir.glob("*.json"), None)

    if json_path is None:
        return False

    data = json.loads(json_path.read_text(encoding="utf-8"))
    frames_json = data.get("frames", [])

    # index PNGs: frame_num -> [(layer_num, Path), ...]
    frame_to_layers = {}

    for p in anim_dir.glob("*.png"):
        
        m = PNG_PATTERN.match(p.name)
        if not m:
            print(f"no PNGs found in {anim_dir}")
            continue

        fnum = int(m.group(1))   # 1-based frame number from filename
        lnum = int(m.group(2))   # layer order: smaller first (bottom)
        frame_to_layers.setdefault(fnum, []).append((lnum, p))

        # creates this structure:
        """
        {
            1: [
                (1, Path("animation-100-frame-1-1.png")),
                (2, Path("animation-100-frame-1-2.png"))
                ],
            2: [
                (1, Path("animation-100-frame-2-1.png"))
                ]
        }
        """


    composed: list[Image.Image] = []
    durations: list[int] = []

    # iterate through frame objects in JSON
    for idx, frame_entry in enumerate(frames_json):

        # start counting at 1
        frame_num = idx + 1

        # bitmaps are layered in reverse order
        layer_entries = sorted(frame_to_layers.get(frame_num, []), reverse=True)

        """
        layer_entries = [
            (1, Path("animation100-frame-1-1.png")),
            (2, Path("animation100-frame-1-2.png")),
            (3, Path("animation100-frame-1-3.png")),
        ]
        """
        
        if not layer_entries:
            # skip if no PNG for frame; some animations have a frame count of 0
            continue

        layer_paths = [p for _, p in layer_entries]
        bm_meta = frame_entry.get("bitmap_metadata", [])

        # shouldn't happen
        if not bm_meta:
            print(f"No bitmap metadata found for animation: {frame_entry}")
            continue

        n = min(len(layer_paths), len(bm_meta))
        if n <= 0:
            # shouldn't happen
            print(f"continuing, n <= 0")
            continue

        # load layer images and anchors (in filename layer order)
        imgs = [Image.open(p).convert("RGBA") for p in layer_paths[:n]]
        anchors = [(int(m["anchor_x"]), int(m["anchor_y"])) for m in bm_meta[:n]]

        # compose one RGBA frame
        frame_img = compose_frame_anchored(imgs, anchors)
        composed.append(frame_img)

        # per-frame duration from JSON
        dur_ms = int(frame_entry["animation_metadata"]["frame_duration"]) * tick_ms
        durations.append(dur_ms)

    if not composed:
        print(f"Gif failed to compose: {anim_dir}")
        return False

    # quantize to a single global palette for GIF stability
    master = composed[0].convert("P", palette=Image.ADAPTIVE, colors=256)

    # quantize all frames against the same palette
    frames_p = [master]
    for im in composed[1:]:
        im_rgb = im.convert("RGB")   # strip alpha channel
        frames_p.append(im_rgb.quantize(palette=master, dither=Image.Dither.FLOYDSTEINBERG))

    # save using palette-stable frames
    out_path = anim_dir / (anim_dir.name + ".gif")
    frames_p[0].save(
        out_path,
        save_all=True,
        append_images=frames_p[1:],
        duration=durations,
        loop=loop,
        disposal=disposal,
        optimize=False,
    )

    return True

def extract_animation_metadata() -> None:

    print("Fetching animation metadata...")

    with open(SPRITE_ADDRESSES_CSV_PATH, newline="") as csvf:

        set_rom()
        reader = csv.reader(csvf)

        for line_no, row in enumerate(reader, start=1):

            row = [cell.strip() for cell in row]

            label = get_label(row)

            if label == -1:
                print(f"Invalid CSV row: {row}")
                continue

            start_address = get_animation_start_address(row)

            try:
                extract(label, start_address)

            except Exception as e:
                print(f"Error: {type(e).__name__}: {e}", file=sys.stderr)
                sys.exit(1)
    
    return

def fetch_sprites_for_animations() -> None:

    print("Fetching sprites for animations...")

    for path in METADATA_DIR.rglob("*.json"):

        with path.open(encoding="utf-8") as f:
            data = json.load(f)

        frame_count = data["frame_count"]
        frames = data.get("frames", [])

        filename = path.stem
        
        for i, frame in enumerate(frames, start=1):

            for j, bitmap in enumerate(frame["bitmap_metadata"], start=1):
                write_frame_texture(bitmap["spritesheet_index"], i, j, path.parent, filename)

    return

def make_gifs_from_animations() -> None:

    print("Creating gifs...")

    # entity animations dirs under base dir
    for asset_dir in sorted(p for p in BASE_DIR.iterdir() if p.is_dir()):
        
        # animation subdirs inside each entity dir
        for anim_dir in sorted(p for p in asset_dir.iterdir() if p.is_dir()):
            build_gif_for_anim_dir(anim_dir)
    
if __name__ == "__main__":

    if len(sys.argv) < 2:
        print("Usage: python hm64_animation_utitilies.py extract_animation_metadata | fetch_sprites_for_animations | make_gifs_from_animations | extract_all")
        sys.exit(1)

    cmd = sys.argv[1]

    if cmd == "extract_animation_metadata":
        extract_animation_metadata()
    elif cmd == "fetch_sprites_for_animations":
        fetch_sprites_for_animations()
    elif cmd == "make_gifs_from_animations":
        make_gifs_from_animations()
    elif cmd == "extract_all":
        extract_animation_metadata()
        fetch_sprites_for_animations()
        make_gifs_from_animations()
    else:
        print(f"Unknown command: {cmd}")