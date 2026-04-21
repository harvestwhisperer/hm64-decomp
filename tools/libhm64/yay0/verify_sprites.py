"""Round-trip every sprite through compress_sprite.process(), then simulate
what the runtime would do: decode the compressed spritesheet sub-region (or
each frame, if SpritesheetIndex is present) and verify bytes match the
original.

Does NOT touch the build dir. Reads .bin.o files via objcopy, runs the
transform in memory.
"""

import struct
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[2]))
from libhm64.yay0.encoder import decode
from libhm64.yay0.compress_sprite import process, parse_u32_be_all, unique_frames

ROOT = Path(__file__).resolve().parents[3]
BUILD = ROOT / "build" / "assets" / "sprites"
OBJCOPY = "mips-linux-gnu-objcopy"


def extract_bin(obj_path: Path) -> bytes:
    with tempfile.NamedTemporaryFile(suffix=".bin", delete=False) as tf:
        out = Path(tf.name)
    try:
        subprocess.run(
            [OBJCOPY, "-O", "binary", "-j", ".data", str(obj_path), str(out)],
            check=True,
            capture_output=True,
        )
        return out.read_bytes()
    finally:
        out.unlink(missing_ok=True)


def verify_one(texture_path: Path, idx_path: Path, sht_path: Path | None) -> tuple[int, int]:
    texture = extract_bin(texture_path)
    idx_bytes = extract_bin(idx_path)
    sht_bytes = extract_bin(sht_path) if sht_path else None

    new_tex, new_idx_bytes, new_sht_bytes = process(texture, idx_bytes, sht_bytes)

    orig_idx = parse_u32_be_all(idx_bytes)
    new_idx = parse_u32_be_all(new_idx_bytes)

    orig_spritesheet = texture[orig_idx[0]:orig_idx[1]]
    new_spritesheet = new_tex[new_idx[0]:new_idx[1]]

    if sht_bytes is None:
        # Whole-blob: decode new spritesheet, compare
        decoded = decode(new_spritesheet)
        if decoded != orig_spritesheet:
            raise RuntimeError(f"{texture_path.name}: whole-spritesheet round-trip failed")
    else:
        orig_sht = parse_u32_be_all(sht_bytes)
        new_sht = parse_u32_be_all(new_sht_bytes)
        # For each unique frame, check decode matches original frame bytes
        orig_uniques = unique_frames(orig_sht)
        new_uniques = unique_frames(new_sht)
        if len(orig_uniques) != len(new_uniques):
            raise RuntimeError(f"{texture_path.name}: unique frame count mismatch")
        for i, orig_off in enumerate(orig_uniques):
            orig_end = orig_uniques[i + 1] if i + 1 < len(orig_uniques) else len(orig_spritesheet)
            orig_frame = orig_spritesheet[orig_off:orig_end]
            new_off = new_uniques[i]
            new_end = new_uniques[i + 1] if i + 1 < len(new_uniques) else len(new_spritesheet)
            decoded = decode(new_spritesheet[new_off:new_end])
            if decoded != orig_frame:
                raise RuntimeError(
                    f"{texture_path.name}: frame {i} (orig_off={orig_off}) round-trip failed"
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
    for tex in sorted(BUILD.rglob("*Texture.bin.o")):
        base = tex.name[: -len("Texture.bin.o")]
        idx = tex.parent / f"{base}AssetsIndex.bin.o"
        sht = tex.parent / f"{base}SpritesheetIndex.bin.o"
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
