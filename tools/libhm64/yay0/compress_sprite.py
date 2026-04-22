"""Rewrite a sprite's Texture/AssetsIndex/[SpritesheetIndex] triplet so the
spritesheet sub-region is Yay0-compressed in place.

The AssetsIndex's 5 u32 offsets describe sub-regions of the texture blob:
    [spritesheet, palette, animation, spriteToPalette, end]

Two shapes:

- Per-frame (SpritesheetIndex present): each frame in the spritesheet is
  encoded as its own yay0 blob; frames are concatenated into the new
  spritesheet sub-region. SpritesheetIndex offsets get rewritten to point
  at the start of each frame's yay0 blob in the concatenated stream.

- Whole-spritesheet (no SpritesheetIndex): the spritesheet sub-region
  becomes a single yay0 blob.

In both cases, palette/animation/spriteToPalette stay raw. The AssetsIndex
offsets shift to reflect the new (smaller) spritesheet sub-region. Callers
that use `nextOffset - thisOffset` as a DMA length still work unchanged —
the difference is just the compressed length now.

I/O model: takes raw .bin inputs, writes Yay0-compressed .bin.yay0 outputs.
The Makefile wraps them into .bin.yay0.o via `ld -r -b binary`.
"""

import argparse
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[2]))
from libhm64.yay0.encoder import encode

ASSETS_INDEX_ENTRIES = 8   # matches u32 assetIndex[8] in globalSprites.c
ASSETS_INDEX_BYTES = ASSETS_INDEX_ENTRIES * 4


def parse_u32_be_all(data: bytes) -> list[int]:
    n = len(data) // 4
    return list(struct.unpack(f">{n}I", data[: n * 4]))


def pack_u32_be(values: list[int], total_entries: int) -> bytes:
    padded = list(values) + [0] * (total_entries - len(values))
    return struct.pack(f">{total_entries}I", *padded)


def unique_frames(sheet_index: list[int]) -> list[int]:
    """Return the list of distinct consecutive offsets, stopping at the first regression.

    Trailing zeros in SpritesheetIndex are padding to alignment.
    The last real entry equals the uncompressed spritesheet length (sentinel).
    """
    out = []
    for off in sheet_index:
        if out and off < out[-1]:
            break
        if not out or off != out[-1]:
            out.append(off)
    return out


def compress_per_frame(spritesheet: bytes, sheet_index: list[int]) -> tuple[bytes, list[int]]:
    """Returns (new_spritesheet_bytes, new_sheet_index).

    sheet_index is an array of u32 offsets (with duplicates for aliased
    sprites) nominally terminated by a sentinel equal to len(spritesheet).
    A handful of sprites (e.g. festivalFlowers) have malformed indices with
    no valid sentinel — the game's getTextureLength for the last sprite in
    those returns garbage, but those frames evidently aren't read at runtime.
    We preserve that behavior by encoding the bytes from the last real offset
    to end-of-spritesheet as a final frame, without adding a sentinel entry
    the index didn't have.
    """
    uniques = unique_frames(sheet_index)
    if not uniques:
        return b"", list(sheet_index)

    last = uniques[-1]
    if last == len(spritesheet):
        # Well-formed: last unique is an explicit sentinel, preceding entries are frames.
        frame_starts = uniques[:-1]
        frame_ends = uniques[1:]
        sentinel = last
    else:
        # Malformed: no sentinel. Treat the last unique as a real frame whose end
        # is end-of-spritesheet. Synthesize an internal-only sentinel for bookkeeping.
        frame_starts = uniques
        frame_ends = uniques[1:] + [len(spritesheet)]
        sentinel = None

    new_stream = bytearray()
    old_to_new = {}
    for start, end in zip(frame_starts, frame_ends):
        old_to_new[start] = len(new_stream)
        new_stream.extend(encode(spritesheet[start:end]))
    if sentinel is not None:
        old_to_new[sentinel] = len(new_stream)

    new_sheet_index = []
    for off in sheet_index:
        if off in old_to_new:
            new_sheet_index.append(old_to_new[off])
        else:
            # Trailing zero padding, or (on malformed indices) garbage entries
            # past the last frame — leave as zero. Not read by game code.
            new_sheet_index.append(0)

    return bytes(new_stream), new_sheet_index


def compress_whole(spritesheet: bytes) -> bytes:
    return encode(spritesheet)


def rewrite_assets_index(assets_index: list[int], new_spritesheet_len: int) -> list[int]:
    """Shift offsets2..5 to reflect the new (shorter) spritesheet sub-region."""
    if len(assets_index) < 5:
        raise ValueError("AssetsIndex must have at least 5 entries")
    old_spritesheet_len = assets_index[1] - assets_index[0]
    delta = old_spritesheet_len - new_spritesheet_len
    new = list(assets_index)
    new[1] -= delta
    new[2] -= delta
    new[3] -= delta
    new[4] -= delta
    return new


def process(texture_bytes: bytes, assets_index_bytes: bytes, sheet_index_bytes: bytes | None, mode: str = "auto"):
    """mode: 'per-frame', 'whole-blob', or 'auto' (per-frame iff sheet_index_bytes given)."""
    assets_index = parse_u32_be_all(assets_index_bytes)
    if len(assets_index) < 5:
        raise ValueError("AssetsIndex too short")

    off0, off1 = assets_index[0], assets_index[1]
    if off0 != 0:
        raise ValueError(f"expected AssetsIndex[0] == 0, got {off0}")
    if off1 > len(texture_bytes):
        raise ValueError("AssetsIndex[1] exceeds texture size")

    spritesheet = texture_bytes[off0:off1]
    rest = texture_bytes[off1:]  # palette + animation + spriteToPalette, unchanged

    if mode == "auto":
        mode = "per-frame" if sheet_index_bytes is not None else "whole-blob"

    if mode == "per-frame":
        if sheet_index_bytes is None:
            raise ValueError("per-frame mode requires a SpritesheetIndex")
        sheet_index = parse_u32_be_all(sheet_index_bytes)
        new_spritesheet, new_sheet_index = compress_per_frame(spritesheet, sheet_index)
        new_sheet_index_bytes = pack_u32_be(new_sheet_index, len(sheet_index))
    elif mode == "whole-blob":
        new_spritesheet = compress_whole(spritesheet)
        # In whole-blob mode, SpritesheetIndex (if any) passes through unchanged —
        # it's either unused at runtime (type-2 sprite) or references offsets in
        # the decompressed spritesheet in RAM, which matches the original layout.
        new_sheet_index_bytes = sheet_index_bytes
    else:
        raise ValueError(f"unknown mode {mode!r}")

    new_texture = new_spritesheet + rest
    new_assets_index = rewrite_assets_index(assets_index, len(new_spritesheet))
    new_assets_index_bytes = pack_u32_be(new_assets_index, len(assets_index))

    return new_texture, new_assets_index_bytes, new_sheet_index_bytes


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--texture-in", required=True, type=Path)
    ap.add_argument("--assets-index-in", required=True, type=Path)
    ap.add_argument("--sheet-index-in", type=Path, default=None)
    ap.add_argument("--texture-out", required=True, type=Path)
    ap.add_argument("--assets-index-out", required=True, type=Path)
    ap.add_argument("--sheet-index-out", type=Path, default=None)
    ap.add_argument("--mode", choices=("auto", "per-frame", "whole-blob"), default="auto")
    args = ap.parse_args()

    texture_bytes = args.texture_in.read_bytes()
    assets_index_bytes = args.assets_index_in.read_bytes()
    sheet_index_bytes = args.sheet_index_in.read_bytes() if args.sheet_index_in else None

    if (sheet_index_bytes is None) != (args.sheet_index_out is None):
        ap.error("--sheet-index-in and --sheet-index-out must be provided together")

    new_texture, new_assets_index, new_sheet_index = process(
        texture_bytes, assets_index_bytes, sheet_index_bytes, mode=args.mode
    )

    args.texture_out.parent.mkdir(parents=True, exist_ok=True)
    args.texture_out.write_bytes(new_texture)
    args.assets_index_out.parent.mkdir(parents=True, exist_ok=True)
    args.assets_index_out.write_bytes(new_assets_index)
    if new_sheet_index is not None:
        args.sheet_index_out.parent.mkdir(parents=True, exist_ok=True)
        args.sheet_index_out.write_bytes(new_sheet_index)

    orig_total = len(texture_bytes)
    new_total = len(new_texture)
    print(
        f"sprite {args.texture_in.name}: texture {orig_total} -> {new_total} bytes "
        f"({100.0 * new_total / max(1, orig_total):.1f}%)",
        file=sys.stderr,
    )


if __name__ == "__main__":
    main()
