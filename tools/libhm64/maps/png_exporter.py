"""
Map texture PNG exporter for HM64.

Exports map tile textures and core object textures to PNG files for viewing.
"""

import argparse
from pathlib import Path

import numpy as np

from ..common.rom import get_rom, read_bytes
from ..common.textures import write_texture_png
from .addresses import (
    get_all_map_addresses,
    get_map_area_path,
    get_asset_offsets_array,
    get_texture_offsets_array,
    get_palette_offsets_array,
)

# Default paths
_PACKAGE_DIR = Path(__file__).resolve().parent.parent.parent.parent
DEFAULT_OUTPUT_DIR = _PACKAGE_DIR / "assets" / "maps"


def _get_texture_width_and_height(texture_start: int) -> tuple:
    """Get texture width and height from header."""
    rom = get_rom()
    width = np.frombuffer(rom, dtype=np.dtype("<i2"), count=1, offset=texture_start + 4).item()
    height = np.frombuffer(rom, dtype=np.dtype("<i2"), count=1, offset=texture_start + 6).item()
    return (width, height)


def _get_texture_type(texture_start: int) -> str:
    """Get texture type (ci4 or ci8) from header flags."""
    rom = get_rom()
    texture_flags = np.frombuffer(rom, dtype=np.dtype("u1"), count=1, offset=texture_start + 3).item()
    return 'ci4' if texture_flags == 16 else 'ci8'


def _get_texture_data(texture_start: int, texture_end: int) -> bytes:
    """Get raw texture data (skipping 8-byte header)."""
    return read_bytes(texture_start + 8, texture_end)


def _get_palette_data(palette_start: int, palette_end: int) -> bytes:
    """Get raw palette data (skipping 4-byte header, max 255 colors)."""
    palette_bytes = read_bytes(palette_start + 4, palette_end)
    # Slice to 255 u16 entries to handle palettes with bad data
    return palette_bytes[:510]


def write_tile_textures(output_dir: Path = DEFAULT_OUTPUT_DIR) -> None:
    """Write all tile textures for all maps to PNG files."""
    output_dir = Path(output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)

    for row in get_all_map_addresses():
        map_name = row[1]

        # Skip empty maps and end marker
        if map_name.startswith("empty") or map_name == "end":
            continue

        map_base = int(row[0], 16)
        asset_offsets = get_asset_offsets_array(row)

        tile_texture_start = asset_offsets[5]
        tile_palette_start = asset_offsets[6]

        if tile_texture_start == 0:
            continue

        texture_offsets = get_texture_offsets_array(map_base, tile_texture_start)
        palette_offsets = get_palette_offsets_array(map_base, tile_palette_start)

        # Output directory: maps/{area}/{variant}/tiles/
        area_path = get_map_area_path(map_name)
        outdir = output_dir / area_path / "tiles"
        outdir.mkdir(parents=True, exist_ok=True)

        # Process each texture (last element is end address)
        for i in range(len(texture_offsets) - 1):
            texture_start = map_base + tile_texture_start + texture_offsets[i]
            texture_end = map_base + tile_texture_start + texture_offsets[i + 1]

            palette_start = map_base + tile_palette_start + palette_offsets[i]
            palette_end = map_base + tile_palette_start + palette_offsets[i + 1]

            texture_data = _get_texture_data(texture_start, texture_end)
            palette_data = _get_palette_data(palette_start, palette_end)

            width, height = _get_texture_width_and_height(texture_start)
            texture_type = _get_texture_type(texture_start)

            output_path = outdir / f"tile-{i}.png"
            write_texture_png(texture_data, palette_data, width, height, texture_type, output_path)

        print(f"Wrote tile textures for {map_name}")


def write_core_object_textures(output_dir: Path = DEFAULT_OUTPUT_DIR) -> None:
    """Write all core object textures for all maps to PNG files."""
    output_dir = Path(output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)

    for row in get_all_map_addresses():
        map_name = row[1]

        # Skip empty maps and end marker
        if map_name.startswith("empty") or map_name == "end":
            continue

        map_base = int(row[0], 16)
        asset_offsets = get_asset_offsets_array(row)

        obj_texture_start = asset_offsets[7]
        obj_palette_start = asset_offsets[8]

        if obj_texture_start == 0:
            continue

        try:
            texture_offsets = get_texture_offsets_array(map_base, obj_texture_start)
            palette_offsets = get_palette_offsets_array(map_base, obj_palette_start)
        except Exception as e:
            print(f"Warning: Could not process core objects for {map_name}: {e}")
            continue

        # Output directory: maps/{area}/{variant}/core_map_objects/
        area_path = get_map_area_path(map_name)
        outdir = output_dir / area_path / "core_map_objects"
        outdir.mkdir(parents=True, exist_ok=True)

        # Process each texture (last element is end address)
        for i in range(len(texture_offsets) - 1):
            texture_start = map_base + obj_texture_start + texture_offsets[i]
            texture_end = map_base + obj_texture_start + texture_offsets[i + 1]

            palette_start = map_base + obj_palette_start + palette_offsets[i]
            palette_end = map_base + obj_palette_start + palette_offsets[i + 1]

            texture_data = _get_texture_data(texture_start, texture_end)
            palette_data = _get_palette_data(palette_start, palette_end)

            width, height = _get_texture_width_and_height(texture_start)
            texture_type = _get_texture_type(texture_start)

            output_path = outdir / f"map_object-{i}.png"
            write_texture_png(texture_data, palette_data, width, height, texture_type, output_path)

        print(f"Wrote core object textures for {map_name}")


def write_all_textures(output_dir: Path = DEFAULT_OUTPUT_DIR) -> None:
    """Write all tile and core object textures for all maps."""
    write_tile_textures(output_dir)
    write_core_object_textures(output_dir)


def main():
    parser = argparse.ArgumentParser(description="HM64 Map Texture PNG Exporter")
    parser.add_argument(
        "command",
        choices=["tiles", "core_objects", "all"],
        help="What to export: tiles, core_objects, or all"
    )
    parser.add_argument(
        "--output-dir", type=str, default=str(DEFAULT_OUTPUT_DIR),
        help="Output directory for PNG files"
    )

    args = parser.parse_args()
    output_dir = Path(args.output_dir)

    if args.command == "tiles":
        write_tile_textures(output_dir)
    elif args.command == "core_objects":
        write_core_object_textures(output_dir)
    elif args.command == "all":
        write_all_textures(output_dir)


if __name__ == "__main__":
    main()
