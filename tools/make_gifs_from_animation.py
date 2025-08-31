from pathlib import Path
import re
import json
from PIL import Image

BASE = Path("../assets/animations")
TICK_MS = 10   # multiply JSON frame_duration by this to get milliseconds (set 1 if already ms)
LOOP = 0       # 0 = loop forever
DISPOSAL = 2   # 2 = restore to background

PNG_PATTERN = re.compile(r".*frame-(\d+)-(\d+)\.png$", re.IGNORECASE)
# Captures: frame_num (group 1), layer_num (group 2)

def compose_frame_anchored(layer_imgs, anchors):
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

        layer_entries = sorted(frame_to_layers.get(frame_num, []))

        """
        layer_entries = [
            (1, Path("animation100-frame-1-1.png")),
            (2, Path("animation100-frame-1-2.png")),
            (3, Path("animation100-frame-1-3.png")),
        ]
        """
        
        if not layer_entries:
            # skip if no PNG for frame; shouldn't happen
            continue

        layer_paths = [p for _, p in layer_entries]
        bm_meta = frame_entry.get("bitmap_metadata", [])

        # shouldn't happen
        if not bm_meta:
            continue

        n = min(len(layer_paths), len(bm_meta))
        if n <= 0:
            # shouldn't happen
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
    
    print(f"Created {out_path}")
    return True
    

def main():

    print("Creating gifs...")

    # entity animations dirs under base dir
    for asset_dir in sorted(p for p in BASE.iterdir() if p.is_dir()):
        
        # animation subdirs inside each entity dir
        for anim_dir in sorted(p for p in asset_dir.iterdir() if p.is_dir()):
            build_gif_for_anim_dir(anim_dir)

if __name__ == "__main__":
    main()
