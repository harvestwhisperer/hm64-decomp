"""Convert indexed (palette + tRNS) PNGs under assets/maps/ to RGBA8.

One-shot migration. The extraction pipeline now emits RGBA8 directly
(see tools/libhm64/common/textures.py), but pre-existing files on disk
were written as indexed PNGs with tRNS transparency — which Godot's
built-in PNG loader silently strips alpha from, so the map editor
can't render alpha-keyed (invisible-wall, collision-only) texels
correctly. Re-saving as RGBA8 materializes the alpha channel.

Usage:
    python -m tools.libhm64.maps.convert_indexed_pngs
    python -m tools.libhm64.maps.convert_indexed_pngs --dry-run
    python -m tools.libhm64.maps.convert_indexed_pngs --root path/to/dir
"""

import argparse
import sys
from pathlib import Path

from PIL import Image

_PACKAGE_DIR = Path(__file__).resolve().parents[3]
DEFAULT_ROOT = _PACKAGE_DIR / "assets" / "maps"


def is_indexed(path: Path) -> bool:
    with Image.open(path) as im:
        return im.mode in ("P", "PA", "1") or "transparency" in im.info


def convert(path: Path) -> bool:
    with Image.open(path) as im:
        if im.mode == "RGBA":
            return False
        rgba = im.convert("RGBA")
    rgba.save(path, format="PNG", optimize=True)
    return True


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--root", type=Path, default=DEFAULT_ROOT,
                    help="Directory to scan recursively for *.png")
    ap.add_argument("--dry-run", action="store_true",
                    help="Report files that would be converted without writing")
    args = ap.parse_args()

    if not args.root.exists():
        print(f"error: {args.root} does not exist", file=sys.stderr)
        return 1

    pngs = sorted(args.root.rglob("*.png"))
    converted = 0
    skipped = 0
    for p in pngs:
        try:
            needs = is_indexed(p)
        except Exception as e:
            print(f"skip (open failed): {p} ({e})", file=sys.stderr)
            continue
        if not needs:
            skipped += 1
            continue
        if args.dry_run:
            print(f"would convert: {p.relative_to(args.root)}")
        else:
            try:
                if convert(p):
                    converted += 1
                else:
                    skipped += 1
            except Exception as e:
                print(f"skip (convert failed): {p} ({e})", file=sys.stderr)

    action = "would convert" if args.dry_run else "converted"
    print(f"{action} {converted} file(s); {skipped} already RGBA8")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
