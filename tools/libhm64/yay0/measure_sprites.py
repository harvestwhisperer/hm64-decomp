"""One-shot measurement: sprite compressibility + sub-region size breakdown.

For each sprite directory under build/assets/sprites that has both a
*Texture.bin and *AssetsIndex.bin, parses the AssetsIndex (u32 BE) and
yay0-compresses:
  - the whole texture blob
  - each of the 4 sub-regions (spritesheet, palette, animation, spriteToPalette)

Prints aggregate numbers and a per-sub-region breakdown.
"""

import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[2]))
from libhm64.yay0.encoder import encode

ROOT = Path(__file__).resolve().parents[3]
BUILD = ROOT / "build" / "assets" / "sprites"

SUB_NAMES = ["spritesheet", "palette", "animation", "spriteToPalette"]


def parse_offsets(index_bytes: bytes, count: int = 5) -> list[int]:
    n = len(index_bytes) // 4
    if n < count:
        count = n
    return list(struct.unpack(f">{count}I", index_bytes[: count * 4]))


def measure_one(texture_path: Path, index_path: Path):
    texture = texture_path.read_bytes()
    index = index_path.read_bytes()
    offsets = parse_offsets(index, 5)
    if len(offsets) < 5:
        return None

    subs = []
    for i in range(4):
        a, b = offsets[i], offsets[i + 1]
        if a > b or b > len(texture):
            return None
        subs.append(texture[a:b])

    whole_enc = encode(texture)
    sub_encs = [encode(s) if len(s) > 0 else b"" for s in subs]

    return {
        "name": texture_path.stem.replace(".bin", ""),
        "raw": len(texture),
        "whole_yay0": len(whole_enc),
        "subs_raw": [len(s) for s in subs],
        "subs_yay0": [len(e) for e in sub_encs],
    }


def find_pairs():
    pairs = []
    for tex in BUILD.rglob("*Texture.bin"):
        base = tex.name[: -len("Texture.bin")]
        idx = tex.parent / f"{base}AssetsIndex.bin"
        if idx.exists():
            pairs.append((tex, idx))
    return sorted(pairs)


def main():
    pairs = find_pairs()
    print(f"# sprite texture+index pairs: {len(pairs)}", file=sys.stderr)

    total_raw = 0
    total_whole_yay0 = 0
    sub_raw = [0, 0, 0, 0]
    sub_yay0 = [0, 0, 0, 0]
    rows = []

    for tex, idx in pairs:
        r = measure_one(tex, idx)
        if r is None:
            print(f"skip: {tex.name} (bad index)", file=sys.stderr)
            continue
        total_raw += r["raw"]
        total_whole_yay0 += r["whole_yay0"]
        for i in range(4):
            sub_raw[i] += r["subs_raw"][i]
            sub_yay0[i] += r["subs_yay0"][i]
        rows.append(r)

    def pct(a, b):
        return 100.0 * a / b if b else 0.0

    per_sub_total = sum(sub_yay0)

    print()
    print("=" * 72)
    print("Aggregate sprite texture compression")
    print("=" * 72)
    print(f"  raw total                 : {total_raw:>10,} bytes")
    print(f"  yay0 (whole blob)         : {total_whole_yay0:>10,} bytes   ({pct(total_whole_yay0, total_raw):5.1f}% of raw, saves {total_raw - total_whole_yay0:>8,})")
    print(f"  yay0 (per sub-region sum) : {per_sub_total:>10,} bytes   ({pct(per_sub_total, total_raw):5.1f}% of raw, saves {total_raw - per_sub_total:>8,})")
    print()
    print("-" * 72)
    print("Per sub-region breakdown")
    print("-" * 72)
    print(f"  {'sub-region':<18} {'raw':>12} {'yay0':>12} {'ratio':>8} {'saves':>12}")
    for i, name in enumerate(SUB_NAMES):
        print(f"  {name:<18} {sub_raw[i]:>12,} {sub_yay0[i]:>12,} {pct(sub_yay0[i], sub_raw[i]):>7.1f}% {sub_raw[i] - sub_yay0[i]:>12,}")
    print()
    print("-" * 72)
    print("Sub-region share of raw texture bytes (what's worth targeting)")
    print("-" * 72)
    for i, name in enumerate(SUB_NAMES):
        print(f"  {name:<18} {pct(sub_raw[i], total_raw):>6.1f}% of raw")
    print()
    print("-" * 72)
    print("Hybrid: compress only the spritesheet sub-region, leave others raw")
    print("-" * 72)
    hybrid = sub_yay0[0] + sub_raw[1] + sub_raw[2] + sub_raw[3]
    print(f"  hybrid total              : {hybrid:>10,} bytes   ({pct(hybrid, total_raw):5.1f}% of raw, saves {total_raw - hybrid:>8,})")
    print()

    # Top 10 biggest textures by raw size
    rows.sort(key=lambda r: -r["raw"])
    print("-" * 72)
    print("Top 10 largest texture blobs (raw)")
    print("-" * 72)
    print(f"  {'name':<40} {'raw':>10} {'whole':>10} {'sum(sub)':>10}")
    for r in rows[:10]:
        print(f"  {r['name']:<40} {r['raw']:>10,} {r['whole_yay0']:>10,} {sum(r['subs_yay0']):>10,}")


if __name__ == "__main__":
    main()
