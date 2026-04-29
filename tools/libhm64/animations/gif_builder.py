"""
GIF builder for HM64 animations.

Composes multi-layer animation frames into GIFs using sprite textures
and animation metadata from assets/sprites/.

Directory structure (from sprite extractor):
    assets/sprites/{subdir}/{asset}/
        textures/{index:03d}.png  - Sprite textures
        animations/{index}.json   - Animation metadata
        manifest.json             - Asset manifest with metadata

Output:
    assets/sprites/{subdir}/{asset}/animations/{index}.gif
"""

import argparse
import json
from pathlib import Path
from typing import List, Tuple, Optional, Dict, Any

from PIL import Image

# Default paths
_REPO_DIR = Path(__file__).resolve().parent.parent.parent.parent
DEFAULT_SPRITES_DIR = _REPO_DIR / "assets" / "sprites"

# GIF parameters
TICK_MS = 20      # Multiply JSON frame_duration by this to get milliseconds
LOOP = 0          # 0 = loop forever
DISPOSAL = 2      # 2 = restore to background


def compute_sprite_rect(
    im: Image.Image,
    anchor: Tuple[int, int]
) -> Tuple[int, int, int, int]:
    """
    Compute the rectangle for a sprite in pivot space (anchor at 0,0).

    The game's setupBitmapVertices (graphic.c) positions sprite centers at:
    - X: anchorX (so left edge = anchorX - width/2)
    - Y: -anchorY in game coords (Y-up), which becomes anchorY in PIL coords (Y-down)

    So in PIL coordinates:
    - left = anchorX - width/2
    - top = anchorY - height/2

    Returns: (left, top, right, bottom)
    """
    ax, ay = anchor
    w, h = im.size
    left = ax - w // 2
    top = ay - h // 2
    return (left, top, left + w, top + h)


def compose_frame_anchored(
    layer_imgs: List[Image.Image],
    anchors: List[Tuple[int, int]],
    global_bounds: Optional[Tuple[int, int, int, int]] = None
) -> Image.Image:
    """
    Compose multiple sprite layers into a single frame.

    Args:
        layer_imgs: List of RGBA images in JSON `sprites` order. layer_imgs[0]
                    corresponds to bitmap[0] in the game and ends up on top;
                    layer_imgs[-1] is the bottom layer.
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

    # Paste each layer shifted so everything is positive. Iterate in reverse
    # so layer_imgs[-1] paints first (background) and layer_imgs[0] paints
    # last (on top), matching the game's bitmap[0]-on-top ordering.
    canvas = Image.new("RGBA", (W, H), (0, 0, 0, 0))
    for im, (left, top, _, _) in zip(reversed(layer_imgs), reversed(rects)):
        x = int(left - min_left)
        y = int(top - min_top)
        canvas.paste(im, (x, y), im)  # Use image as mask to respect alpha

    return canvas


def normalize_animation_data(data: Dict[str, Any]) -> List[Dict[str, Any]]:
    """
    Normalize animation data to a common format.

    Handles both formats:
    - Simplified format (from sprite extractor): frame.duration, frame.sprites
    - Full format (from animation metadata): frame.animation_metadata.frame_duration, frame.bitmap_metadata

    Returns:
        List of normalized frames with keys: duration, sprites (list of {spritesheet_index, anchor_x, anchor_y})
    """
    frames_json = data.get("frames", [])
    normalized = []

    for frame in frames_json:
        # Check which format we have
        if "sprites" in frame:
            # Simplified format from sprite extractor
            normalized.append({
                "duration": int(frame.get("duration", 1)),
                "sprites": frame["sprites"]
            })
        elif "bitmap_metadata" in frame:
            # Full format from animation metadata extractor
            anim_meta = frame.get("animation_metadata", {})
            normalized.append({
                "duration": int(anim_meta.get("frame_duration", 1)),
                "sprites": [
                    {
                        "spritesheet_index": bm["spritesheet_index"],
                        "anchor_x": bm["anchor_x"],
                        "anchor_y": bm["anchor_y"]
                    }
                    for bm in frame["bitmap_metadata"]
                ]
            })

    return normalized


def build_gif_for_animation(
    anim_json_path: Path,
    textures_dir: Path,
    output_path: Path,
    *,
    tick_ms: int = TICK_MS,
    loop: int = LOOP,
    disposal: int = DISPOSAL
) -> bool:
    """
    Build a GIF from an animation JSON file.

    Args:
        anim_json_path: Path to animation JSON file
        textures_dir: Directory containing sprite texture PNGs ({index:03d}.png)
        output_path: Path for output GIF file
        tick_ms: Multiply JSON frame_duration by this to get milliseconds
        loop: GIF loop count (0 = forever)
        disposal: GIF disposal method (2 = restore to background)

    Returns:
        True if GIF was created successfully, False otherwise
    """
    if not anim_json_path.exists():
        return False

    data = json.loads(anim_json_path.read_text(encoding="utf-8"))

    # Normalize to common format
    frames = normalize_animation_data(data)

    if not frames:
        return False

    # FIRST PASS: Collect all frame data and compute global bounding box
    all_frame_data = []
    global_min_left = float('inf')
    global_min_top = float('inf')
    global_max_right = float('-inf')
    global_max_bot = float('-inf')

    for frame in frames:
        sprites = frame.get("sprites", [])

        if not sprites:
            continue

        # Load sprite images for each layer
        # In the game, bitmap[0] ends up on top, so we paste in order
        imgs = []
        anchors = []
        for sprite in sprites:
            sprite_idx = sprite["spritesheet_index"]
            sprite_path = textures_dir / f"{sprite_idx:03d}.png"

            if not sprite_path.exists():
                continue

            try:
                img = Image.open(sprite_path).convert("RGBA")
                imgs.append(img)
                anchors.append((int(sprite["anchor_x"]), int(sprite["anchor_y"])))
            except Exception as e:
                print(f"Skipping unreadable sprite: {sprite_path} ({e})")
                continue

        if not imgs:
            continue

        # Update global bounds
        for im, anchor in zip(imgs, anchors):
            left, top, right, bot = compute_sprite_rect(im, anchor)
            global_min_left = min(global_min_left, left)
            global_min_top = min(global_min_top, top)
            global_max_right = max(global_max_right, right)
            global_max_bot = max(global_max_bot, bot)

        dur_ms = int(frame["duration"]) * tick_ms
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

    # Quantize to a single global palette with transparency support
    TRANS_COLOR = (255, 0, 255)

    def rgba_to_p_with_transparency(rgba_img: Image.Image) -> Image.Image:
        """Convert RGBA image to palette mode preserving transparency."""
        rgb_img = Image.new("RGB", rgba_img.size, TRANS_COLOR)
        rgb_img.paste(rgba_img, mask=rgba_img.split()[3])
        return rgb_img

    # Convert all frames to RGB with transparency color
    rgb_frames = [rgba_to_p_with_transparency(im) for im in composed]

    # Create master palette from first frame
    master = rgb_frames[0].quantize(colors=255, method=Image.Quantize.MEDIANCUT)

    # Find or add the transparency color index in the palette
    palette_data = master.getpalette()
    if palette_data is None:
        palette_data = [0] * 768

    # Pad palette to 256 colors if needed
    while len(palette_data) < 768:
        palette_data.append(0)

    trans_index = None
    for i in range(256):
        r, g, b = palette_data[i*3:i*3+3]
        if (r, g, b) == TRANS_COLOR:
            trans_index = i
            break

    if trans_index is None:
        trans_index = 255
        palette_data[trans_index*3:trans_index*3+3] = list(TRANS_COLOR)

    master.putpalette(palette_data)

    # Quantize all frames using the master palette
    frames_p = [master]
    for rgb_img in rgb_frames[1:]:
        frames_p.append(rgb_img.quantize(palette=master, dither=Image.Dither.FLOYDSTEINBERG))

    # Save GIF with transparency
    output_path.parent.mkdir(parents=True, exist_ok=True)
    frames_p[0].save(
        output_path,
        save_all=True,
        append_images=frames_p[1:],
        duration=durations,
        loop=loop,
        disposal=disposal,
        optimize=False,
        transparency=trans_index,
    )

    return True


def build_gifs_for_asset(asset_dir: Path) -> int:
    """
    Build GIFs for all animations in an asset directory.

    Expects directory structure:
        asset_dir/
            textures/{index:03d}.png
            animations/{index}.json

    Output:
        asset_dir/animations/{index}.gif

    Args:
        asset_dir: Path to asset directory (e.g., assets/sprites/entitySprites/player)

    Returns:
        Number of GIFs created
    """
    textures_dir = asset_dir / "textures"
    animations_dir = asset_dir / "animations"

    if not textures_dir.exists() or not animations_dir.exists():
        return 0

    gifs_built = 0

    # Find all animation JSON files
    for anim_json in sorted(animations_dir.glob("*.json")):
        # Skip non-numeric files (like index files)
        stem = anim_json.stem
        if not stem.isdigit():
            continue

        output_path = animations_dir / f"{stem}.gif"

        if build_gif_for_animation(anim_json, textures_dir, output_path):
            gifs_built += 1

    return gifs_built


def build_all_gifs(
    sprites_dir: Path = DEFAULT_SPRITES_DIR,
    asset_name: Optional[str] = None
):
    """
    Build GIFs for all animations in the sprites directory.

    Args:
        sprites_dir: Base directory containing sprite assets (assets/sprites/)
        asset_name: If provided, only process this asset
    """
    print("Creating GIFs from extracted sprites...")

    if not sprites_dir.exists():
        print(f"Warning: Sprites directory does not exist: {sprites_dir}")
        print("Run 'python -m libhm64.sprites.extractor extract_all' first.")
        return

    total_gifs = 0
    total_assets = 0

    # Iterate through subdirs (entitySprites, etc.)
    for subdir in sorted(sprites_dir.iterdir()):
        if not subdir.is_dir():
            continue

        # Iterate through asset directories
        for asset_dir in sorted(subdir.iterdir()):
            if not asset_dir.is_dir():
                continue

            # Skip if looking for specific asset
            if asset_name and asset_dir.name != asset_name:
                continue

            gifs_built = build_gifs_for_asset(asset_dir)

            if gifs_built > 0:
                print(f"  Built {gifs_built} GIFs for {asset_dir.name}")
                print(f"    Output: {(asset_dir / 'animations').resolve()}")
                total_gifs += gifs_built
                total_assets += 1

    if total_gifs == 0:
        print("No GIFs were built. Make sure sprites have been extracted with PNGs.")
    else:
        print(f"Built {total_gifs} GIFs across {total_assets} assets.")


def main():
    parser = argparse.ArgumentParser(
        description="HM64 Animation GIF Builder",
        epilog="Reads from assets/sprites/{subdir}/{asset}/ and creates GIFs in animations/ subdirectory."
    )
    parser.add_argument(
        "--sprites-dir", type=str, default=str(DEFAULT_SPRITES_DIR),
        help="Base directory containing sprite assets (default: assets/sprites/)"
    )
    parser.add_argument(
        "--asset", type=str, default=None,
        help="Only process this specific asset (e.g., 'player')"
    )

    args = parser.parse_args()

    build_all_gifs(Path(args.sprites_dir), args.asset)


if __name__ == "__main__":
    main()
