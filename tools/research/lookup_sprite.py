#!/usr/bin/env python3
"""
Look up the extracted PNG(s) for an animation script entry.

Given a sprite asset segment label (e.g. "holdableItems") and an animation
index N (the first arg of PACK_ANIM_DATA), prints the animation metadata
JSON path and each frame's spritesheet_index + PNG path.

Example:
    python tools/research/lookup_sprite.py holdableItems 103
    python tools/research/lookup_sprite.py holdableItems 103 --paths-only
"""

import argparse
import json
import sys
from pathlib import Path

REPO_DIR = Path(__file__).resolve().parent.parent.parent
sys.path.insert(0, str(REPO_DIR / "tools"))

from libhm64.sprites.addresses import get_sprite_by_label

ASSETS_DIR = REPO_DIR / "assets"


def lookup(segment: str, anim_index: int):
    info = get_sprite_by_label(segment)
    if info is None:
        sys.exit(f"error: no sprite segment named {segment!r} in sprite_addresses.csv")

    asset_dir = ASSETS_DIR / "sprites" / info.subdir / segment
    texture_dir = asset_dir / "textures"
    anim_json = asset_dir / "animations" / f"{anim_index:02d}.json"
    gif_path = asset_dir / "animations" / f"{anim_index:02d}.gif"

    if not anim_json.exists():
        sys.exit(f"error: animation JSON not found: {anim_json}")

    with open(anim_json) as f:
        data = json.load(f)

    frames = []
    for i, frame in enumerate(data["frames"]):
        duration = frame["duration"]
        sprites = frame.get("sprites", [])
        if not sprites:
            frames.append((i, duration, None, None))
            continue
        for sp in sprites:
            idx = sp["spritesheet_index"]
            png = texture_dir / f"{idx:03d}.png"
            frames.append((i, duration, idx, png))

    return info, anim_json, gif_path, frames


def main():
    p = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    p.add_argument("segment", help="Sprite segment label (e.g. holdableItems)")
    p.add_argument("anim_index", type=int, help="Animation index (the N in PACK_ANIM_DATA(N, ...))")
    p.add_argument("--paths-only", action="store_true", help="Print unique PNG paths, one per line")
    p.add_argument("--frame", type=int, help="Show only this frame index")
    args = p.parse_args()

    info, anim_json, gif_path, frames = lookup(args.segment, args.anim_index)

    if args.frame is not None:
        frames = [f for f in frames if f[0] == args.frame]
        if not frames:
            sys.exit(f"error: frame {args.frame} not in animation")

    if args.paths_only:
        seen = set()
        for _, _, _, png in frames:
            if png is None or png in seen:
                continue
            seen.add(png)
            print(png)
        return

    print(f"segment:  {info.label} (subdir: {info.subdir})")
    print(f"anim:     {anim_json.relative_to(REPO_DIR)}")
    gif_marker = "" if gif_path.exists() else "  [MISSING]"
    print(f"gif:      {gif_path.relative_to(REPO_DIR)}{gif_marker}")
    print(f"frames:   {len(frames)}")
    print()
    print(f"{'frame':>5}  {'dur':>4}  {'sprite':>6}  png")
    print(f"{'-----':>5}  {'----':>4}  {'------':>6}  ---")
    for i, duration, idx, png in frames:
        if idx is None:
            print(f"{i:>5}  {duration:>4}  {'--':>6}  (no bitmap)")
            continue
        rel = png.relative_to(REPO_DIR)
        marker = "" if png.exists() else "  [MISSING]"
        print(f"{i:>5}  {duration:>4}  {idx:>6}  {rel}{marker}")


if __name__ == "__main__":
    main()
