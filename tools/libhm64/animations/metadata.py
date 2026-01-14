"""
Animation metadata extraction for HM64.

Extracts animation frame data, timing, and sprite references from ROM.
Handles animation aliases (multiple indices pointing to same animation data).
"""

import argparse
import csv
import json
from pathlib import Path
from typing import Dict, Any, List, Optional

import numpy as np

from ..common.rom import get_rom, set_rom_path
from ..data import SPRITE_ADDRESSES_CSV

# NumPy dtypes for animation structures
ANIM_DT = np.dtype([
    ("sprite_count", "<u2"),
    ("frame_duration", "u1"),
    ("has_audio", "u1"),
])

BMP_DT = np.dtype([
    ("spritesheet_index", "<u2"),
    ("unused", "<u2"),
    ("anchor_x", "<i2"),
    ("anchor_y", "<i2"),
])

# Default paths
_REPO_DIR = Path(__file__).resolve().parent.parent.parent.parent
DEFAULT_OUTPUT_DIR = _REPO_DIR / "assets" / "animations"


def get_label(row: List[str]) -> str:
    """Get the label from a sprite addresses CSV row."""
    if len(row) == 6:
        return row[4]
    elif len(row) == 5:
        return row[3]
    elif len(row) == 4:
        return row[3]
    else:
        return "Invalid row"


def get_animation_start_address(row: List[str]) -> int:
    """Get the animation table start address from a sprite addresses row."""
    rom = get_rom()
    asset_offsets_array = np.frombuffer(
        rom, dtype=np.dtype(">u4"), count=8, offset=int(row[1], 16)
    )
    offset = int(asset_offsets_array[2])
    base = int(row[0], 16)
    return base + offset


def read_metadata(meta_addr: int, simple_format: bool = False) -> Dict[str, Any]:
    """
    Read animation metadata from ROM address.

    Args:
        meta_addr: ROM address of the animation data
        simple_format: If True, use simplified output format for sprite manifests

    Returns:
        Animation metadata dict
    """
    rom = get_rom()

    num_frames = np.frombuffer(
        rom, dtype=np.dtype("<u2"), count=1, offset=(meta_addr + 4)
    )[0].item()
    unused_header_value = np.frombuffer(
        rom, dtype=np.dtype("<u2"), count=1, offset=(meta_addr + 6)
    )[0].item()

    p = meta_addr + 8
    frames = []

    for _ in range(num_frames):
        animation_metadata = np.frombuffer(rom, dtype=ANIM_DT, count=1, offset=p)[0]
        p += ANIM_DT.itemsize

        n_bmps = int(animation_metadata["sprite_count"])
        bmps_arr = np.frombuffer(rom, dtype=BMP_DT, count=n_bmps, offset=p)
        p += n_bmps * BMP_DT.itemsize

        if simple_format:
            # Simplified format for sprite manifests
            frames.append({
                "duration": int(animation_metadata["frame_duration"]),
                "has_audio": int(animation_metadata["has_audio"]),
                "sprites": [
                    {
                        "spritesheet_index": int(b["spritesheet_index"]),
                        "anchor_x": int(b["anchor_x"]),
                        "anchor_y": int(b["anchor_y"]),
                    }
                    for b in bmps_arr
                ]
            })
        else:
            # Full format for GIF builder
            frames.append({
                "animation_metadata": {
                    "sprite_count": int(animation_metadata["sprite_count"]),
                    "frame_duration": int(animation_metadata["frame_duration"]),
                    "has_audio": bool(animation_metadata["has_audio"]),
                },
                "bitmap_metadata": [
                    {
                        "spritesheet_index": int(b["spritesheet_index"]),
                        "unused": int(b["unused"]),
                        "anchor_x": int(b["anchor_x"]),
                        "anchor_y": int(b["anchor_y"]),
                    }
                    for b in bmps_arr
                ]
            })

    return {
        "frame_count": int(num_frames),
        "unused": int(unused_header_value),
        "frames": frames
    }


def read_animation_from_offsets(
    anim_base: int,
    anim_offsets: List[int],
    anim_idx: int,
    simple_format: bool = True
) -> Dict[str, Any]:
    """
    Read animation metadata using an offset table.

    This is the interface used by the sprite extractor.

    Args:
        anim_base: Base address of the animation section
        anim_offsets: List of offsets from animation index table
        anim_idx: Index of animation to read
        simple_format: If True, use simplified output format

    Returns:
        Animation metadata dict, or empty dict if invalid
    """
    if anim_idx >= len(anim_offsets) - 1:
        return {}

    offset = anim_offsets[anim_idx]
    next_offset = anim_offsets[anim_idx + 1]

    if offset == next_offset:
        return {}

    anim_addr = anim_base + offset
    return read_metadata(anim_addr, simple_format=simple_format)


def analyze_animation_offsets(table_base: int) -> Dict[str, Any]:
    """
    Analyze animation offset table and return comprehensive mapping.

    Returns:
        Dict containing:
        - total_count: Total number of entries in the table
        - unique_animations: List of (index, offset) for unique animations
        - aliases: List of (index, alias_of_index, offset) for aliased animations
        - end_markers: List of indices that are end markers
        - index_mapping: Dict mapping each index to its status
    """
    rom = get_rom()

    first_offset = np.frombuffer(
        rom, dtype=np.dtype(">u4"), count=1, offset=table_base
    )[0].item()
    count = first_offset // 4

    offsets = np.frombuffer(rom, dtype=np.dtype(">u4"), count=count, offset=table_base)

    offset_to_first_idx = {}
    unique_animations = []
    aliases = []
    end_markers = []
    index_mapping = {}

    last_offset = int(offsets[-1])

    for idx in range(count):
        off_val = int(offsets[idx])

        if off_val == last_offset:
            end_markers.append(idx)
            index_mapping[idx] = {"status": "end_marker"}
            continue

        if off_val in offset_to_first_idx:
            original_idx = offset_to_first_idx[off_val]
            aliases.append((idx, original_idx, off_val))
            index_mapping[idx] = {
                "status": "alias",
                "alias_of": original_idx,
                "offset": off_val
            }
        else:
            offset_to_first_idx[off_val] = idx
            unique_animations.append((idx, off_val))
            index_mapping[idx] = {
                "status": "unique",
                "offset": off_val
            }

    return {
        "total_count": count,
        "unique_animations": unique_animations,
        "aliases": aliases,
        "end_markers": end_markers,
        "index_mapping": index_mapping
    }


def extract_with_aliases(
    label: str,
    table_base: int,
    output_dir: Path = DEFAULT_OUTPUT_DIR,
    verbose: bool = False
) -> Dict[str, Any]:
    """
    Extract animations with full alias support.

    Creates:
    - Unique animation directories with full data
    - Alias animation directories with reference JSON
    - animation_index.json with complete mapping

    Returns:
        Dict with extraction statistics
    """
    analysis = analyze_animation_offsets(table_base)

    outdir = output_dir / label
    outdir.mkdir(parents=True, exist_ok=True)

    extracted_count = 0
    alias_count = 0

    # Extract unique animations
    for idx, off in analysis["unique_animations"]:
        meta_addr = table_base + off
        data = read_metadata(meta_addr)

        subdir = outdir / f"animation{idx}"
        subdir.mkdir(parents=True, exist_ok=True)

        out_path = subdir / f"animation{idx}.json"
        with open(out_path, "w", encoding="utf-8") as f:
            json.dump(data, f, ensure_ascii=False, indent=2)

        extracted_count += 1

        if verbose:
            print(f"  Extracted animation{idx} (offset 0x{off:X})")

    # Create alias references
    for idx, original_idx, off in analysis["aliases"]:
        subdir = outdir / f"animation{idx}"
        subdir.mkdir(parents=True, exist_ok=True)

        alias_data = {
            "is_alias": True,
            "alias_of": original_idx,
            "alias_of_name": f"animation{original_idx}",
            "offset": off,
            "note": f"This animation is identical to animation{original_idx}. "
                    "The game reuses the same animation data."
        }

        out_path = subdir / f"animation{idx}.json"
        with open(out_path, "w", encoding="utf-8") as f:
            json.dump(alias_data, f, ensure_ascii=False, indent=2)

        alias_count += 1

        if verbose:
            print(f"  Created alias animation{idx} -> animation{original_idx}")

    # Write comprehensive index file
    index_data = {
        "asset": label,
        "total_indices": analysis["total_count"],
        "unique_count": len(analysis["unique_animations"]),
        "alias_count": len(analysis["aliases"]),
        "end_marker_count": len(analysis["end_markers"]),
        "animations": {}
    }

    for idx, info in analysis["index_mapping"].items():
        index_data["animations"][str(idx)] = info

    index_path = outdir / "animation_index.json"
    with open(index_path, "w", encoding="utf-8") as f:
        json.dump(index_data, f, ensure_ascii=False, indent=2)

    return {
        "label": label,
        "total_indices": analysis["total_count"],
        "extracted": extracted_count,
        "aliases_created": alias_count,
        "end_markers": len(analysis["end_markers"])
    }


def extract_all_metadata(
    csv_path: Path = SPRITE_ADDRESSES_CSV,
    output_dir: Path = DEFAULT_OUTPUT_DIR,
    verbose: bool = False,
    with_aliases: bool = True
):
    """Extract animation metadata for all assets."""
    print("Extracting animation metadata...")

    with open(csv_path, newline="", encoding="utf-8") as csvf:
        reader = csv.reader(csvf)

        for line_no, row in enumerate(reader, start=1):
            row = [cell.strip() for cell in row]
            label = get_label(row)

            if label == "Invalid row":
                continue

            start_address = get_animation_start_address(row)

            try:
                if with_aliases:
                    result = extract_with_aliases(
                        label, start_address, output_dir, verbose=verbose
                    )
                    if verbose:
                        print(f"{label}: {result['extracted']} unique, "
                              f"{result['aliases_created']} aliases")
                else:
                    # Simple extraction without alias tracking
                    extract_with_aliases(label, start_address, output_dir, verbose=False)
            except Exception as e:
                print(f"Error processing {label}: {type(e).__name__}: {e}")


def main():
    parser = argparse.ArgumentParser(description="HM64 Animation Metadata Extractor")
    parser.add_argument(
        "--output-dir", type=str, default=str(DEFAULT_OUTPUT_DIR),
        help="Output directory for animation metadata"
    )
    parser.add_argument(
        "--csv", type=str, default=str(SPRITE_ADDRESSES_CSV),
        help="Path to sprite addresses CSV"
    )
    parser.add_argument(
        "--rom", type=str, default=None,
        help="Path to ROM file"
    )
    parser.add_argument(
        "-v", "--verbose", action="store_true",
        help="Print detailed progress"
    )

    args = parser.parse_args()

    if args.rom:
        set_rom_path(Path(args.rom))

    extract_all_metadata(Path(args.csv), Path(args.output_dir), args.verbose)


if __name__ == "__main__":
    main()
