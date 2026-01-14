"""
GIF builder for HM64 animations.

Composes multi-layer animation frames into GIFs using extracted PNG files
and animation metadata JSON.
"""

import argparse
import json
import re
import shutil
from pathlib import Path
from typing import List, Tuple, Optional

from PIL import Image

# Default paths
_REPO_DIR = Path(__file__).resolve().parent.parent.parent.parent
DEFAULT_ANIMATIONS_DIR = _REPO_DIR / "assets" / "animations"

# GIF parameters
TICK_MS = 10      # Multiply JSON frame_duration by this to get milliseconds
LOOP = 0          # 0 = loop forever
DISPOSAL = 2      # 2 = restore to background

# Pattern to match frame PNG filenames
PNG_PATTERN = re.compile(r".*frame-(\d+)-(\d+)\.png$", re.IGNORECASE)


def compute_sprite_rect(
    im: Image.Image,
    anchor: Tuple[int, int]
) -> Tuple[int, int, int, int]:
    """
    Compute the rectangle for a sprite in pivot space (anchor at 0,0).

    The game's setupBitmapVertices (graphic.c) positions sprites such that:
    - X: sprite center is at anchorX (left edge = anchorX - width/2)
    - Y: sprite center is at -anchorY (top edge = -anchorY - height/2)

    Returns: (left, top, right, bottom)
    """
    ax, ay = anchor
    w, h = im.size
    left = ax - w // 2
    top = -ay - h // 2
    return (left, top, left + w, top + h)


def compose_frame_anchored(
    layer_imgs: List[Image.Image],
    anchors: List[Tuple[int, int]],
    global_bounds: Optional[Tuple[int, int, int, int]] = None
) -> Image.Image:
    """
    Compose multiple sprite layers into a single frame.

    Args:
        layer_imgs: List of RGBA images (bottom to top order)
        anchors: List of (anchor_x, anchor_y) tuples, same order as layer_imgs
        global_bounds: Optional (min_left, min_top, max_right, max_bot) for
                       consistent canvas sizing across all frames

    Returns:
        Composed RGBA frame where each layer's anchor lands at the same pivot.
    """
    # Compute each layer's rectangle in pivot space (anchor at 0,0)
    rects = [compute_sprite_rect(im, anchor) for im, anchor in zip(layer_imgs, anchors)]

    # Use global bounds if provided, otherwise compute from this frame only
    if global_bounds:
        min_left, min_top, max_right, max_bot = global_bounds
    else:
        min_left = min(r[0] for r in rects)
        min_top = min(r[1] for r in rects)
        max_right = max(r[2] for r in rects)
        max_bot = max(r[3] for r in rects)

    W = int(max_right - min_left)
    H = int(max_bot - min_top)

    # Paste each layer shifted so everything is positive
    canvas = Image.new("RGBA", (W, H), (0, 0, 0, 0))
    for im, (left, top, _, _) in zip(layer_imgs, rects):
        x = int(left - min_left)
        y = int(top - min_top)
        canvas.paste(im, (x, y), im)  # Use image as mask to respect alpha

    return canvas


def build_gif_for_anim_dir(
    anim_dir: Path,
    *,
    tick_ms: int = TICK_MS,
    loop: int = LOOP,
    disposal: int = DISPOSAL
) -> bool:
    """
    Build a GIF from an animation directory.

    Args:
        anim_dir: Path to animation directory containing JSON and PNGs
        tick_ms: Multiply JSON frame_duration by this to get milliseconds
        loop: GIF loop count (0 = forever)
        disposal: GIF disposal method (2 = restore to background)

    Returns:
        True if GIF was created successfully, False otherwise
    """
    json_path = next(anim_dir.glob("*.json"), None)

    if json_path is None:
        return False

    data = json.loads(json_path.read_text(encoding="utf-8"))

    # Skip alias animations - they get their GIF copied from the original
    if data.get("is_alias"):
        return False  # Alias handled separately by process_alias_gifs

    frames_json = data.get("frames", [])

    # Index PNGs: frame_num -> [(layer_num, Path), ...]
    frame_to_layers = {}

    for p in anim_dir.glob("*.png"):
        m = PNG_PATTERN.match(p.name)
        if not m:
            continue

        fnum = int(m.group(1))   # 1-based frame number from filename
        lnum = int(m.group(2))   # Layer order: smaller first (bottom)
        frame_to_layers.setdefault(fnum, []).append((lnum, p))

    # FIRST PASS: Collect all frame data and compute global bounding box
    all_frame_data = []
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
            continue

        n = min(len(layer_paths), len(bm_meta))
        if n <= 0:
            continue

        # Load layer images in reverse order (layer n, n-1, ..., 1 -> bottom to top)
        # Handle corrupt/unreadable PNGs gracefully
        imgs = []
        valid_layer_indices = []
        for i, p in enumerate(layer_paths[:n]):
            try:
                imgs.append(Image.open(p).convert("RGBA"))
                valid_layer_indices.append(i)
            except Exception as e:
                print(f"Skipping unreadable image: {p} ({e})")
                continue

        if not imgs:
            continue

        # Anchors must match the image order (reversed)
        # Only use anchors for layers that were successfully loaded
        reversed_bm_meta = list(reversed(bm_meta[:n]))
        anchors = [(int(reversed_bm_meta[i]["anchor_x"]), int(reversed_bm_meta[i]["anchor_y"]))
                   for i in valid_layer_indices]

        # Update global bounds
        for im, anchor in zip(imgs, anchors):
            left, top, right, bot = compute_sprite_rect(im, anchor)
            global_min_left = min(global_min_left, left)
            global_min_top = min(global_min_top, top)
            global_max_right = max(global_max_right, right)
            global_max_bot = max(global_max_bot, bot)

        dur_ms = int(frame_entry["animation_metadata"]["frame_duration"]) * tick_ms
        all_frame_data.append((imgs, anchors, dur_ms))

    if not all_frame_data:
        return False

    global_bounds = (global_min_left, global_min_top, global_max_right, global_max_bot)

    # SECOND PASS: Compose frames using global bounds
    composed: List[Image.Image] = []
    durations: List[int] = []

    for imgs, anchors, dur_ms in all_frame_data:
        frame_img = compose_frame_anchored(imgs, anchors, global_bounds)
        composed.append(frame_img)
        durations.append(dur_ms)

    if not composed:
        return False

    # Quantize to a single global palette for GIF stability
    master = composed[0].convert("P", palette=Image.ADAPTIVE, colors=256)

    frames_p = [master]
    for im in composed[1:]:
        im_rgb = im.convert("RGB")
        frames_p.append(im_rgb.quantize(palette=master, dither=Image.Dither.FLOYDSTEINBERG))

    # Save GIF
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


def process_alias_gifs(asset_dir: Path) -> int:
    """
    Copy GIFs to alias animation directories.

    Returns count of aliases processed.
    """
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


def build_all_gifs(
    base_dir: Path = DEFAULT_ANIMATIONS_DIR,
    asset_name: Optional[str] = None
):
    """
    Build GIFs for all animations.

    Args:
        base_dir: Base directory containing animation assets
        asset_name: If provided, only process this asset
    """
    print("Creating GIFs...")

    if not base_dir.exists():
        print(f"Warning: Animation directory does not exist: {base_dir}")
        print("Run extract-animation-metadata first to extract animation data.")
        return

    if asset_name:
        asset_dirs = [base_dir / asset_name]
    else:
        asset_dirs = sorted(p for p in base_dir.iterdir() if p.is_dir())

    total_gifs = 0
    total_aliases = 0

    for asset_dir in asset_dirs:
        if not asset_dir.is_dir():
            continue

        gifs_built = 0
        for anim_dir in sorted(p for p in asset_dir.iterdir() if p.is_dir()):
            if build_gif_for_anim_dir(anim_dir):
                gifs_built += 1

        # Process alias GIFs
        aliases_copied = process_alias_gifs(asset_dir)

        if gifs_built > 0 or aliases_copied > 0:
            print(f"Built {gifs_built} GIFs for {asset_dir.name} ({aliases_copied} aliases)")

        total_gifs += gifs_built
        total_aliases += aliases_copied

    if total_gifs == 0 and total_aliases == 0:
        print("No GIFs were built. Make sure to run extract-animation-sprites first to extract PNG frames.")


def main():
    parser = argparse.ArgumentParser(description="HM64 Animation GIF Builder")
    parser.add_argument(
        "--base-dir", type=str, default=str(DEFAULT_ANIMATIONS_DIR),
        help="Base directory containing animation assets"
    )
    parser.add_argument(
        "--asset", type=str, default=None,
        help="Only process this specific asset"
    )

    args = parser.parse_args()

    build_all_gifs(Path(args.base_dir), args.asset)


if __name__ == "__main__":
    main()
