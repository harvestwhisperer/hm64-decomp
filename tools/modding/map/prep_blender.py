#!/usr/bin/env python3

import csv
import sys
from pathlib import Path

# Add parent tools directory to path to import utilities
sys.path.insert(0, str(Path(__file__).parent.parent.parent))

import hm64_map_utilities
from hm64_map_utilities import set_rom, get_raw_bytes, write_tile_textures, write_core_map_object_textures

# Configuration - use absolute paths from script location
MAPS_DIR = Path(__file__).parent.parent.parent.parent / "assets" / "maps"
MAP_ADDRESSES_CSV_PATH = Path(__file__).parent.parent.parent / "map_addresses.csv"
ROM_PATH = Path(__file__).parent.parent.parent.parent / "baserom.us.z64"

# Override paths in hm64_map_utilities to use correct paths from this script's location
hm64_map_utilities.ROM_PATH = ROM_PATH
hm64_map_utilities.MAPS_DIR = MAPS_DIR
hm64_map_utilities.MAP_ADDRESSES_CSV_PATH = MAP_ADDRESSES_CSV_PATH


def main():
    """Extract all map assets for importing into Blender"""

    set_rom()

    # Extract binary map data
    print("\n" + "=" * 60)
    print("Extracting map files")
    print("=" * 60)

    with open(MAP_ADDRESSES_CSV_PATH) as f:
        reader = csv.reader(f)
        addresses_list = list(tuple(line) for line in reader)

    basedir = MAPS_DIR
    basedir.mkdir(parents=True, exist_ok=True)

    extracted_count = 0

    for idx, row in enumerate(addresses_list):
        map_name = row[1]

        # Skip empty maps
        if map_name.startswith("empty-map") or map_name == "end":
            print(f"Skipping {map_name}")
            continue

        # Get start address for this map
        start_addr = int(row[0], 16)

        # Get end address (start of next entry - could be another map, empty-map, or end marker)
        if idx + 1 < len(addresses_list):
            end_addr = int(addresses_list[idx + 1][0], 16)
        else:
            # Shouldn't happen if 'end' entry exists in CSV
            continue

        # Extract binary data
        map_data = get_raw_bytes(start_addr, end_addr)

        # Create output directory
        outdir = basedir / map_name
        outdir.mkdir(parents=True, exist_ok=True)

        # Write binary file using map name
        output_file = outdir / f"{map_name}.bin"
        output_file.write_bytes(map_data)

        print(f"Extracted {map_name}: 0x{start_addr:X} to 0x{end_addr:X} ({len(map_data)} bytes)")
        extracted_count += 1

    print(f"\nExtracted {extracted_count} map binaries")

    # Extract textures
    print("\n" + "=" * 60)
    print("Extracting tile textures")
    print("=" * 60)
    write_tile_textures()

    print("\n" + "=" * 60)
    print("Extracting core map object textures")
    print("=" * 60)
    write_core_map_object_textures()

    # Done
    print("\n" + "=" * 60)
    print("Asset prep complete!")
    print("=" * 60)
    print(f"\nAssets extracted to: {basedir}")

if __name__ == "__main__":
    main()
