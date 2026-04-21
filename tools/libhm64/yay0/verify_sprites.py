"""Round-trip every sprite through compress_sprite.process(), then simulate
what the runtime would do: decode the compressed spritesheet sub-region (or
each frame, if SpritesheetIndex is present) and verify bytes match the
original.

Reads the raw .bin files from build/assets/sprites/ and runs the transform
in memory; nothing is written back.
"""

import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[2]))
from libhm64.yay0.encoder import decode
from libhm64.yay0.compress_sprite import process, parse_u32_be_all, unique_frames

ROOT = Path(__file__).resolve().parents[3]
BUILD = ROOT / "build" / "assets" / "sprites"

# Sprites that look type-1 on disk (SpritesheetIndex file present) but are
# consumed via the whole-blob DMA path at runtime. For these, the spritesheet
# sub-region must be compressed as a single yay0 blob.
WHOLE_BLOB_OVERRIDES = {"festivalFlowers"}


def verify_one(texture_path: Path, idx_path: Path, sht_path: Path | None) -> tuple[int, int]:
    texture = texture_path.read_bytes()
    idx_bytes = idx_path.read_bytes()
    sht_bytes = sht_path.read_bytes() if sht_path else None

    base = texture_path.name[: -len("Texture.bin")]
    mode = "whole-blob" if base in WHOLE_BLOB_OVERRIDES else "auto"
    new_tex, new_idx_bytes, new_sht_bytes = process(texture, idx_bytes, sht_bytes, mode=mode)

    effective_mode = ("per-frame" if sht_bytes is not None else "whole-blob") if mode == "auto" else mode

    orig_idx = parse_u32_be_all(idx_bytes)
    new_idx = parse_u32_be_all(new_idx_bytes)

    orig_spritesheet = texture[orig_idx[0]:orig_idx[1]]
    new_spritesheet = new_tex[new_idx[0]:new_idx[1]]

    if effective_mode == "whole-blob":
        decoded = decode(new_spritesheet)
        if decoded != orig_spritesheet:
            raise RuntimeError(f"{texture_path.name}: whole-spritesheet round-trip failed")
    else:
        orig_sht = parse_u32_be_all(sht_bytes)
        new_sht = parse_u32_be_all(new_sht_bytes)
        orig_uniques = unique_frames(orig_sht)
        new_uniques = unique_frames(new_sht)
        if len(orig_uniques) != len(new_uniques):
            raise RuntimeError(f"{texture_path.name}: unique frame count mismatch")
        # Well-formed: last unique is sentinel == len(spritesheet), real frames are uniques[:-1].
        # Malformed: last unique is a real frame whose end is spritesheet length.
        has_sentinel = orig_uniques and orig_uniques[-1] == len(orig_spritesheet)
        orig_starts = orig_uniques[:-1] if has_sentinel else orig_uniques
        orig_ends = orig_uniques[1:] if has_sentinel else (orig_uniques[1:] + [len(orig_spritesheet)])
        new_starts = new_uniques[:-1] if has_sentinel else new_uniques
        new_ends = new_uniques[1:] if has_sentinel else (new_uniques[1:] + [len(new_spritesheet)])
        for i, (o_s, o_e, n_s, n_e) in enumerate(zip(orig_starts, orig_ends, new_starts, new_ends)):
            orig_frame = orig_spritesheet[o_s:o_e]
            decoded = decode(new_spritesheet[n_s:n_e])
            if decoded != orig_frame:
                raise RuntimeError(
                    f"{texture_path.name}: frame {i} (orig_off={o_s}) round-trip failed"
                )

    # Sub-region checks: palette/animation/spriteToPalette unchanged
    for i in range(1, 4):
        orig = texture[orig_idx[i]:orig_idx[i + 1]]
        new = new_tex[new_idx[i]:new_idx[i + 1]]
        if orig != new:
            raise RuntimeError(f"{texture_path.name}: sub-region {i} bytes changed")

    return len(texture), len(new_tex)


def main():
    pairs = []
    for tex in sorted(BUILD.rglob("*Texture.bin")):
        base = tex.name[: -len("Texture.bin")]
        idx = tex.parent / f"{base}AssetsIndex.bin"
        sht = tex.parent / f"{base}SpritesheetIndex.bin"
        if not idx.exists():
            continue
        pairs.append((tex, idx, sht if sht.exists() else None))

    total_orig = 0
    total_new = 0
    failures = []
    for i, (tex, idx, sht) in enumerate(pairs, 1):
        try:
            o, n = verify_one(tex, idx, sht)
            total_orig += o
            total_new += n
        except Exception as e:
            failures.append((tex.name, str(e)))

    print(f"verified: {len(pairs) - len(failures)}/{len(pairs)}")
    print(f"total: {total_orig:,} -> {total_new:,} bytes "
          f"({100.0 * total_new / max(1, total_orig):.1f}%, saves {total_orig - total_new:,})")
    if failures:
        print(f"\nFAILURES:")
        for name, err in failures:
            print(f"  {name}: {err}")
        sys.exit(1)


if __name__ == "__main__":
    main()
