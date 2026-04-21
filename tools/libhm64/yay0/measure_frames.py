"""Per-frame sprite compression measurement.

Splits each spritesheet into frames using its SpritesheetIndex (u32 BE offsets,
consecutive entries = frame bounds; duplicates are reused palettes and collapse
to zero-length chunks). Measures raw + yay0 size per unique frame, then reports
the distribution — specifically what fraction would fit in the 64 KB yay0Scratch
compressed.

For spritesheets with no SpritesheetIndex, treats the whole spritesheet sub-region
as a single 'chunk' (that's how globalSprites DMAs them).
"""

import struct
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[2]))
from libhm64.yay0.encoder import encode

ROOT = Path(__file__).resolve().parents[3]
BUILD = ROOT / "build" / "assets" / "sprites"
OBJCOPY = "mips-linux-gnu-objcopy"
SCRATCH_LIMIT = 0x10000  # 64 KB


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


def parse_u32s(b: bytes) -> list[int]:
    n = len(b) // 4
    return list(struct.unpack(f">{n}I", b[: n * 4]))


def spritesheet_frames(sheet: bytes, sheet_index: list[int]) -> list[tuple[int, int]]:
    """Return list of (offset, length) for unique consecutive entries.

    The SpritesheetIndex is a list of u32 offsets where consecutive distinct
    entries bound a frame. Trailing zeros are padding to the .bin.o alignment.
    The last real entry equals the spritesheet length (sentinel).
    """
    frames = []
    if not sheet_index:
        return [(0, len(sheet))]
    prev_off = None
    for off in sheet_index:
        # Stop at the first offset that regresses — that's padding after the sentinel.
        if prev_off is not None and off < prev_off:
            break
        if off != prev_off:
            if prev_off is not None:
                frames.append((prev_off, off - prev_off))
            prev_off = off
    return [(o, l) for o, l in frames if l > 0]


def main():
    total_frames = 0
    total_raw = 0
    total_yay0 = 0
    frames_over_scratch = 0
    raw_over_scratch = 0
    bucket_edges = [1024, 4096, 16384, 32768, 65536, 131072, 262144, 524288, 1 << 30]
    bucket_labels = ["<=1K", "<=4K", "<=16K", "<=32K", "<=64K", "<=128K", "<=256K", "<=512K", ">512K"]
    yay0_buckets = [0] * len(bucket_edges)

    sheets_with_index = 0
    sheets_without_index = 0
    with_index_raw = 0
    with_index_yay0 = 0
    without_index_raw = 0
    without_index_yay0 = 0
    big_frames = []  # (compressed_size, name, raw_size)

    for tex in sorted(BUILD.rglob("*Texture.bin.o")):
        base = tex.name[: -len("Texture.bin.o")]
        idx_path = tex.parent / f"{base}AssetsIndex.bin.o"
        sheet_idx_path = tex.parent / f"{base}SpritesheetIndex.bin.o"
        if not idx_path.exists():
            continue

        texture = extract_bin(tex)
        index = parse_u32s(extract_bin(idx_path))
        if len(index) < 5:
            continue
        spritesheet = texture[index[0] : index[1]]  # sub-region 0 = spritesheet

        has_index = sheet_idx_path.exists()
        if has_index:
            sheets_with_index += 1
            sheet_idx = parse_u32s(extract_bin(sheet_idx_path))
            frames = spritesheet_frames(spritesheet, sheet_idx)
        else:
            sheets_without_index += 1
            frames = [(0, len(spritesheet))] if len(spritesheet) > 0 else []

        for off, length in frames:
            chunk = spritesheet[off : off + length]
            enc = encode(chunk)
            total_frames += 1
            total_raw += length
            total_yay0 += len(enc)
            if has_index:
                with_index_raw += length
                with_index_yay0 += len(enc)
            else:
                without_index_raw += length
                without_index_yay0 += len(enc)
            if len(enc) > SCRATCH_LIMIT:
                frames_over_scratch += 1
                big_frames.append((len(enc), base, length))
            if length > SCRATCH_LIMIT:
                raw_over_scratch += 1
            for i, edge in enumerate(bucket_edges):
                if len(enc) <= edge:
                    yay0_buckets[i] += 1
                    break

    def pct(a, b):
        return 100.0 * a / b if b else 0.0

    print(f"sprite dirs with SpritesheetIndex   : {sheets_with_index}")
    print(f"sprite dirs without SpritesheetIndex: {sheets_without_index}")
    print()
    print(f"total chunks         : {total_frames:,}")
    print(f"total raw bytes      : {total_raw:,}")
    print(f"total yay0 bytes     : {total_yay0:,} ({pct(total_yay0, total_raw):.1f}% of raw, saves {total_raw - total_yay0:,})")
    print()
    print(f"  per-frame path (has SpritesheetIndex):")
    print(f"    raw : {with_index_raw:>10,}")
    print(f"    yay0: {with_index_yay0:>10,} ({pct(with_index_yay0, with_index_raw):.1f}%, saves {with_index_raw - with_index_yay0:,})")
    print(f"  whole-spritesheet path (no SpritesheetIndex):")
    print(f"    raw : {without_index_raw:>10,}")
    print(f"    yay0: {without_index_yay0:>10,} ({pct(without_index_yay0, without_index_raw):.1f}%, saves {without_index_raw - without_index_yay0:,})")
    print()
    print(f"frames > 64KB raw       : {raw_over_scratch}")
    print(f"frames > 64KB compressed: {frames_over_scratch}  (would not fit yay0Scratch)")
    print()
    print("Distribution by compressed (yay0) size:")
    print(f"  {'bucket':<10} {'count':>8}  {'pct':>6}")
    for label, cnt in zip(bucket_labels, yay0_buckets):
        print(f"  {label:<10} {cnt:>8}  {100.0 * cnt / max(1, total_frames):>5.1f}%")
    print()
    if big_frames:
        big_frames.sort(reverse=True)
        print(f"Largest compressed frames (top 15):")
        print(f"  {'compressed':>12} {'raw':>12}  name")
        for enc_sz, name, raw_sz in big_frames[:15]:
            print(f"  {enc_sz:>12,} {raw_sz:>12,}  {name}")


if __name__ == "__main__":
    main()
