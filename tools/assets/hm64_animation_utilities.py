import csv
import json
import numpy as np
import re
import shutil
import sys
from typing import Dict, Any, List, Tuple, Optional

from hm64_sprite_utilities import (
    check_asset_addresses_exist,
    write_texture_from_csv_row_and_index,
    set_rom as set_sprite_rom,
    get_rom,
    get_cached_row_by_index,
    _ensure_caches
)

from n64img.image import CI4, CI8, Image
from pathlib import Path
from PIL import Image

# Make paths relative to this script's location
SCRIPT_DIR = Path(__file__).parent.resolve()
METADATA_DIR = SCRIPT_DIR / "../../assets/animations/"
ROM_PATH = SCRIPT_DIR / "../../baserom.us.z64"
SPRITE_ADDRESSES_CSV_PATH = SCRIPT_DIR / "sprite_addresses.csv"

BASE_DIR = SCRIPT_DIR / "../../assets/animations"
OUTPUT_DIR = SCRIPT_DIR / "../../assets/animations"

TICK_MS = 10   # multiply JSON frame_duration by this to get milliseconds (set 1 if already ms)
LOOP = 0       # 0 = loop forever
DISPOSAL = 2   # 2 = restore to background

PNG_PATTERN = re.compile(r".*frame-(\d+)-(\d+)\.png$", re.IGNORECASE)
# Captures: frame_num (group 1), layer_num (group 2)

# ROM initialization flag
_rom_initialized = False

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
    """Initialize ROM (delegates to sprite utilities for caching)."""
    global _rom_initialized
    if not _rom_initialized:
        set_sprite_rom()
        _rom_initialized = True

def build_filename(base_filename: str, frame_count: int, bitmap_count: int) -> str:
    return base_filename + f"-frame-{frame_count}" + f"-{bitmap_count}"

def get_label(row: List[str]) -> str:
    if len(row) == 6:
        # 6 columns (type-1): addr1, addr2, addr3, addr4, label, subdir
        return row[4]

    elif len(row) == 5:
        # 5 columns (type-2): addr1, addr2, addr3, label, subdir
        return row[3]

    elif len(row) == 4:
        # Legacy 4 columns: addr1, addr2, addr3, label
        return row[3]

    else:
        return "Invalid row"

def get_animation_start_address(row: List[str]) -> int:
    rom = get_rom()
    asset_offsets_array = np.frombuffer(rom, dtype=np.dtype(">u4"), count=8, offset=int(row[1], 16))

    offset = int(asset_offsets_array[2])
    base = int(row[0], 16)

    return base + offset


def read_metadata(meta_addr: int) -> Dict[str, Any]:
    """Read animation metadata from ROM address."""
    rom = get_rom()

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


def analyze_animation_offsets(table_base: int) -> Dict[str, Any]:
    """
    Analyze animation offset table and return comprehensive mapping.

    Returns:
        Dict containing:
        - total_count: Total number of entries in the table
        - unique_animations: List of (index, offset) for unique animations
        - aliases: List of (index, alias_of_index, offset) for aliased animations
        - end_markers: List of indices that are end markers
        - index_mapping: Dict mapping each index to its status
    """
    rom = get_rom()

    first_offset = np.frombuffer(rom, dtype=np.dtype(">u4"), count=1, offset=table_base)[0].item()
    count = first_offset // 4

    offsets = np.frombuffer(rom, dtype=np.dtype(">u4"), count=count, offset=table_base)

    offset_to_first_idx = {}
    unique_animations = []
    aliases = []
    end_markers = []
    index_mapping = {}

    last_offset = int(offsets[-1])

    for idx in range(count):
        off_val = int(offsets[idx])

        if off_val == last_offset:
            end_markers.append(idx)
            index_mapping[idx] = {"status": "end_marker"}
            continue

        if off_val in offset_to_first_idx:
            original_idx = offset_to_first_idx[off_val]
            aliases.append((idx, original_idx, off_val))
            index_mapping[idx] = {
                "status": "alias",
                "alias_of": original_idx,
                "offset": off_val
            }
        else:
            offset_to_first_idx[off_val] = idx
            unique_animations.append((idx, off_val))
            index_mapping[idx] = {
                "status": "unique",
                "offset": off_val
            }

    return {
        "total_count": count,
        "unique_animations": unique_animations,
        "aliases": aliases,
        "end_markers": end_markers,
        "index_mapping": index_mapping
    }

def extract(label: str, table_base: int) -> None:
    """
    Extract animations (legacy function for backward compatibility).
    Extracts only unique animations, skips duplicates.
    """
    rom = get_rom()

    first_offset = np.frombuffer(rom, dtype=np.dtype(">u4"), count=1, offset=table_base)[0].item()
    count = first_offset // 4
    offsets = np.frombuffer(rom, dtype=np.dtype(">u4"), count=count, offset=table_base)

    outdir = OUTPUT_DIR / label
    outdir.mkdir(parents=True, exist_ok=True)

    previous_off = -1
    offsets = [offset for offset in offsets if offset != offsets[-1]]

    for idx, off in enumerate(offsets):
        if off == previous_off:
            continue

        previous_off = off
        meta_addr = table_base + int(off)
        data = read_metadata(meta_addr)

        subdir = outdir / f"animation{idx}"
        subdir.mkdir(parents=True, exist_ok=True)

        out_path = subdir / f"animation{idx}.json"
        with open(out_path, "w", encoding="utf-8") as f:
            json.dump(data, f, ensure_ascii=False, indent=2)

    return


def extract_with_aliases(label: str, table_base: int, verbose: bool = False) -> Dict[str, Any]:
    """
    Extract animations with full alias support.

    Creates:
    - Unique animation directories with full data
    - Alias animation directories with reference JSON
    - animation_index.json with complete mapping

    Returns:
        Dict with extraction statistics
    """
    rom = get_rom()

    # Analyze offsets
    analysis = analyze_animation_offsets(table_base)

    outdir = OUTPUT_DIR / label
    outdir.mkdir(parents=True, exist_ok=True)

    extracted_count = 0
    alias_count = 0

    # Extract unique animations
    for idx, off in analysis["unique_animations"]:
        meta_addr = table_base + off
        data = read_metadata(meta_addr)

        subdir = outdir / f"animation{idx}"
        subdir.mkdir(parents=True, exist_ok=True)

        out_path = subdir / f"animation{idx}.json"
        with open(out_path, "w", encoding="utf-8") as f:
            json.dump(data, f, ensure_ascii=False, indent=2)

        extracted_count += 1

        if verbose:
            print(f"  Extracted animation{idx} (offset 0x{off:X})")

    # Create alias references
    for idx, original_idx, off in analysis["aliases"]:
        subdir = outdir / f"animation{idx}"
        subdir.mkdir(parents=True, exist_ok=True)

        alias_data = {
            "is_alias": True,
            "alias_of": original_idx,
            "alias_of_name": f"animation{original_idx}",
            "offset": off,
            "note": f"This animation is identical to animation{original_idx}. The game reuses the same animation data."
        }

        out_path = subdir / f"animation{idx}.json"
        with open(out_path, "w", encoding="utf-8") as f:
            json.dump(alias_data, f, ensure_ascii=False, indent=2)

        alias_count += 1

        if verbose:
            print(f"  Created alias animation{idx} -> animation{original_idx}")

    # Write comprehensive index file
    index_data = {
        "asset": label,
        "total_indices": analysis["total_count"],
        "unique_count": len(analysis["unique_animations"]),
        "alias_count": len(analysis["aliases"]),
        "end_marker_count": len(analysis["end_markers"]),
        "animations": {}
    }

    for idx, info in analysis["index_mapping"].items():
        index_data["animations"][str(idx)] = info

    index_path = outdir / "animation_index.json"
    with open(index_path, "w", encoding="utf-8") as f:
        json.dump(index_data, f, ensure_ascii=False, indent=2)

    return {
        "label": label,
        "total_indices": analysis["total_count"],
        "extracted": extracted_count,
        "aliases_created": alias_count,
        "end_markers": len(analysis["end_markers"])
    }


def copy_gif_for_alias(alias_dir: Path, original_dir: Path) -> bool:
    """Copy GIF from original animation to alias directory."""
    original_gif = original_dir / f"{original_dir.name}.gif"
    alias_gif = alias_dir / f"{alias_dir.name}.gif"

    if not original_gif.exists():
        return False

    try:
        shutil.copy2(original_gif, alias_gif)
        return True
    except Exception as e:
        print(f"Failed to copy GIF for alias: {e}")
        return False


def process_alias_gifs(asset_name: str) -> int:
    """
    Copy GIFs to alias animation directories.
    Returns count of aliases processed.
    """
    asset_dir = OUTPUT_DIR / asset_name
    index_path = asset_dir / "animation_index.json"

    if not index_path.exists():
        return 0

    with open(index_path, encoding="utf-8") as f:
        index_data = json.load(f)

    processed = 0

    for idx_str, info in index_data.get("animations", {}).items():
        if info.get("status") == "alias":
            idx = int(idx_str)
            original_idx = info["alias_of"]

            alias_dir = asset_dir / f"animation{idx}"
            original_dir = asset_dir / f"animation{original_idx}"

            if copy_gif_for_alias(alias_dir, original_dir):
                processed += 1

    return processed

def write_frame_texture(sprite_index: int, frame_count: int, bitmap_count: int, output_path: Path, base_filename: str) -> None:

    result, asset_type = check_asset_addresses_exist(output_path.parent.name)

    filename = build_filename(base_filename, frame_count, bitmap_count)

    write_texture_from_csv_row_and_index(result, sprite_index, filename, output_path)

    return

def compute_sprite_rect(im: Image.Image, anchor: Tuple[int, int]) -> Tuple[int, int, int, int]:
    """
    Compute the rectangle for a sprite in pivot space (anchor at 0,0).

    The game's setupBitmapVertices (graphic.c) positions sprites such that:
    - X: sprite center is at anchorX (left edge = anchorX - width/2)
    - Y: sprite center is at -anchorY (top edge = -anchorY - height/2)

    Returns: (left, top, right, bottom)
    """
    ax, ay = anchor
    w, h = im.size
    # Game formula: center_x = ax, center_y = -ay
    # So: left = center_x - w/2 = ax - w/2
    #     top  = center_y - h/2 = -ay - h/2
    left = ax - w // 2
    top  = -ay - h // 2
    return (left, top, left + w, top + h)


def compose_frame_anchored(
    layer_imgs: List[Image.Image],
    anchors: List[Tuple[int, int]],
    global_bounds: Optional[Tuple[int, int, int, int]] = None
) -> Image.Image:
    """
    layer_imgs: list[Image.Image]  (RGBA)
    anchors:    list[tuple[int,int]]   (anchor_x, anchor_y), same order as layer_imgs
    global_bounds: Optional (min_left, min_top, max_right, max_bot) for consistent canvas sizing

    Returns a composed RGBA frame where each layer's anchor lands at the same pivot.
    """

    # compute each layer's rectangle in pivot space (anchor at 0,0)
    rects = [compute_sprite_rect(im, anchor) for im, anchor in zip(layer_imgs, anchors)]

    # Use global bounds if provided, otherwise compute from this frame only
    if global_bounds:
        min_left, min_top, max_right, max_bot = global_bounds
    else:
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

    # FIRST PASS: Collect all frame data and compute global bounding box
    # This ensures all frames have consistent canvas size for proper animation
    all_frame_data = []  # List of (imgs, anchors, duration)
    global_min_left = float('inf')
    global_min_top = float('inf')
    global_max_right = float('-inf')
    global_max_bot = float('-inf')

    for idx, frame_entry in enumerate(frames_json):
        frame_num = idx + 1

        # In the game (globalSprites.c:setBitmapFromSpriteObject), bitmaps are drawn
        # in reverse order: bitmap[n-1] first (bottom), bitmap[0] last (top).
        # For PIL, we paste bottom layers first, so we need reverse order too.
        layer_entries = sorted(frame_to_layers.get(frame_num, []), reverse=True)

        if not layer_entries:
            continue

        layer_paths = [p for _, p in layer_entries]
        bm_meta = frame_entry.get("bitmap_metadata", [])

        if not bm_meta:
            print(f"No bitmap metadata found for animation: {frame_entry}")
            continue

        n = min(len(layer_paths), len(bm_meta))
        if n <= 0:
            print(f"continuing, n <= 0")
            continue

        # Load layer images in reverse order (layer 3, 2, 1 -> bottom to top)
        imgs = [Image.open(p).convert("RGBA") for p in layer_paths[:n]]

        # CRITICAL: Anchors must match the image order!
        # layer_paths are in reverse order (layer n, n-1, ..., 1)
        # So we need anchors in reverse order too: bm_meta[n-1], bm_meta[n-2], ..., bm_meta[0]
        reversed_bm_meta = list(reversed(bm_meta[:n]))
        anchors = [(int(m["anchor_x"]), int(m["anchor_y"])) for m in reversed_bm_meta]

        # Compute rectangles for all sprites in this frame to update global bounds
        for im, anchor in zip(imgs, anchors):
            left, top, right, bot = compute_sprite_rect(im, anchor)
            global_min_left = min(global_min_left, left)
            global_min_top = min(global_min_top, top)
            global_max_right = max(global_max_right, right)
            global_max_bot = max(global_max_bot, bot)

        # Store frame data for second pass
        dur_ms = int(frame_entry["animation_metadata"]["frame_duration"]) * tick_ms
        all_frame_data.append((imgs, anchors, dur_ms))

    # Check if we have any frames
    if not all_frame_data:
        print(f"No frame data found for {anim_dir}")
        return False

    # Create global bounds tuple
    global_bounds = (global_min_left, global_min_top, global_max_right, global_max_bot)

    # SECOND PASS: Compose frames using global bounds for consistent canvas size
    composed: list[Image.Image] = []
    durations: list[int] = []

    for imgs, anchors, dur_ms in all_frame_data:
        # compose one RGBA frame with global bounds
        frame_img = compose_frame_anchored(imgs, anchors, global_bounds)
        composed.append(frame_img)
        durations.append(dur_ms)

    # happens when an animation uses an invalid sprite index
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

def extract_animation_metadata(verbose: bool = False, with_aliases: bool = True) -> None:
    """Extract animation metadata for all assets."""
    print("Fetching animation metadata...")

    _ensure_caches()
    set_rom()

    with open(SPRITE_ADDRESSES_CSV_PATH, newline="") as csvf:
        reader = csv.reader(csvf)

        for line_no, row in enumerate(reader, start=1):
            row = [cell.strip() for cell in row]
            label = get_label(row)

            if label == -1:
                continue

            start_address = get_animation_start_address(row)

            try:
                if with_aliases:
                    result = extract_with_aliases(label, start_address, verbose=verbose)
                    if verbose:
                        print(f"{label}: {result['extracted']} unique, {result['aliases_created']} aliases")
                else:
                    extract(label, start_address)
            except Exception as e:
                print(f"Error processing {label}: {type(e).__name__}: {e}", file=sys.stderr)

    return


def fetch_sprites_for_animations(asset_name: Optional[str] = None, verbose: bool = False) -> None:
    """
    Fetch sprites for animations.
    Args:
        asset_name: If provided, only process this asset. Otherwise process all.
        verbose: Print progress information
    """
    print("Fetching sprites for animations...")

    _ensure_caches()
    set_rom()

    if asset_name:
        search_path = METADATA_DIR / asset_name
        json_paths = list(search_path.rglob("*.json"))
    else:
        json_paths = list(METADATA_DIR.rglob("*.json"))

    for path in json_paths:
        # Skip index files
        if path.name == "animation_index.json":
            continue

        with path.open(encoding="utf-8") as f:
            data = json.load(f)

        # Skip aliases - they don't have their own frames
        if data.get("is_alias"):
            continue

        frames = data.get("frames", [])
        filename = path.stem

        if verbose:
            print(f"  Processing {path.parent.name}...")

        for i, frame in enumerate(frames, start=1):
            for j, bitmap in enumerate(frame["bitmap_metadata"], start=1):
                write_frame_texture(bitmap["spritesheet_index"], i, j, path.parent, filename)

    return


def make_gifs_from_animations(asset_name: Optional[str] = None) -> None:
    """Create GIFs from animations (optionally for single asset)."""
    print("Creating gifs...")

    if asset_name:
        asset_dirs = [BASE_DIR / asset_name]
    else:
        asset_dirs = sorted(p for p in BASE_DIR.iterdir() if p.is_dir())

    for asset_dir in asset_dirs:
        if not asset_dir.is_dir():
            continue

        for anim_dir in sorted(p for p in asset_dir.iterdir() if p.is_dir()):
            build_gif_for_anim_dir(anim_dir)

        # Process alias GIFs
        process_alias_gifs(asset_dir.name)


def extract_single_asset(asset_name: str, force: bool = False, verbose: bool = False) -> Dict[str, Any]:
    """
    Extract animations for a single asset with alias support.

    Args:
        asset_name: Name of the asset to extract
        force: If True, re-extract even if data exists
        verbose: Print progress information

    Returns:
        Dict with extraction statistics
    """
    _ensure_caches()
    set_rom()

    # Find the asset row
    result, asset_type = check_asset_addresses_exist(asset_name)

    if result == -1:
        return {"status": "error", "message": f"Asset '{asset_name}' not found"}

    row = get_cached_row_by_index(result)

    outdir = OUTPUT_DIR / asset_name

    if outdir.exists() and not force:
        # Return cached info
        index_path = outdir / "animation_index.json"
        if index_path.exists():
            with open(index_path, encoding="utf-8") as f:
                index_data = json.load(f)
            return {
                "status": "cached",
                "asset": asset_name,
                "total_indices": index_data.get("total_indices", 0),
                "unique_count": index_data.get("unique_count", 0),
                "alias_count": index_data.get("alias_count", 0)
            }

    start_address = get_animation_start_address(row)

    # Extract with aliases
    stats = extract_with_aliases(asset_name, start_address, verbose=verbose)

    # Fetch sprites
    if verbose:
        print(f"Fetching sprites for {asset_name}...")
    fetch_sprites_for_animations(asset_name, verbose=verbose)

    # Create GIFs
    if verbose:
        print(f"Creating GIFs for {asset_name}...")
    make_gifs_from_animations(asset_name)

    return {
        "status": "success",
        **stats
    }


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python hm64_animation_utilities.py <command> [options]")
        print()
        print("Commands:")
        print("  extract_animation_metadata    Extract metadata for all assets")
        print("  fetch_sprites_for_animations  Fetch sprite images for all animations")
        print("  make_gifs_from_animations     Create GIFs from extracted animations")
        print("  extract_all                   Run all extraction steps")
        print("  extract_single <name>         Extract single asset with aliases")
        print()
        print("Options:")
        print("  --verbose, -v                 Print detailed progress")
        sys.exit(1)

    cmd = sys.argv[1]
    verbose = "--verbose" in sys.argv or "-v" in sys.argv

    if cmd == "extract_animation_metadata":
        extract_animation_metadata(verbose=verbose)
    elif cmd == "fetch_sprites_for_animations":
        fetch_sprites_for_animations(verbose=verbose)
    elif cmd == "make_gifs_from_animations":
        make_gifs_from_animations()
    elif cmd == "extract_all":
        extract_animation_metadata(verbose=verbose)
        fetch_sprites_for_animations(verbose=verbose)
        make_gifs_from_animations()
    elif cmd == "extract_single":
        if len(sys.argv) < 3:
            print("Usage: python hm64_animation_utilities.py extract_single <asset_name>")
            sys.exit(1)
        asset_name = sys.argv[2]
        result = extract_single_asset(asset_name, force=True, verbose=verbose)
        print(json.dumps(result, indent=2))
    else:
        print(f"Unknown command: {cmd}")