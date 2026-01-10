import csv
import json
import numpy as np
from n64img.image import CI4, CI8, Image
from pathlib import Path
from typing import List, Tuple, Union
import re
import sys

from hm64_sprite_utilities import *

MAPS_DIR = Path("../assets/maps")
MAP_ADDRESSES_CSV_PATH = Path("map_addresses.csv")
ROM_PATH = Path("../../baserom.us.z64")


def get_map_area(map_name: str) -> str:
    """
    Get the base area for a map (without variant/season).

    Examples:
        farmSpring -> farm
        mountain1Spring -> mountain/mountain1
        barn -> farm
        bakery -> bakery
    """
    # Explicit mappings for non-obvious cases
    explicit_mappings = {
        # Ranch area
        'annsRoom': 'ranch',
        'ranchBarn': 'ranch',
        'ranchStore': 'ranch',
        'ranchHouse': 'ranch',
        # Bakery area
        'ellisRoom': 'bakery',
        'bakery': 'bakery',
        # Flower shop area
        'popurisRoom': 'flowerShop',
        'flowerShop': 'flowerShop',
        # Church (standalone)
        'church': 'church',
        # Souvenir shop (standalone)
        'souvenirShop': 'souvenirShop',
        # Rick's shop
        'ricksShop': 'rickShop',
        # Midwife
        'midwifesHouse': 'midwife',
        # Tavern (standalone)
        'tavern': 'tavern',
        # Library (standalone)
        'library': 'library',
        # Mayor's house area
        'mariasRoom': 'mayorsHouse',
        'mayorsHouse': 'mayorsHouse',
        # Potion shop area
        'potionShopBackroom': 'potionShop',
        'potionShop': 'potionShop',
        # Cave area
        'spriteCave': 'cave',
        'harvestSpriteCave': 'cave',
        'cave': 'cave',
        # Mine area
        'emptyMine': 'mine',
        'mine': 'mine',
        # Vineyard area
        'karensRoom': 'vineyard',
        'vineyardHouse': 'vineyard',
        'vineyardCellar': 'vineyard',
        'vineyardCellar1': 'vineyard',
        'vineyardCellar2': 'vineyard',
        # Farm area (player's farm)
        'greenhouse': 'farm',
        'barn': 'farm',
        'coop': 'farm',
        # House area (player's house)
        'house': 'house',
        'kitchen': 'house',
        'bathroom': 'house',
        # Carpenter/dumpling (standalone)
        'carpentersHut': 'carpenterHut',
        'carpenterHut': 'carpenterHut',
        'dumplingHouse': 'dumplingHouse',
        # Empty/unknown
        'emptyMap1': 'unknown',
        'emptyMap2': 'unknown',
        'emptyMap3': 'unknown',
        'emptyMap4': 'unknown',
    }

    if map_name in explicit_mappings:
        return explicit_mappings[map_name]

    # Check for nested areas (mountain1, village2, etc.)
    nested_patterns = [
        (r'^mountain1', 'mountain/mountain1'),
        (r'^mountain2', 'mountain/mountain2'),
        (r'^topOfMountain1', 'mountain/topOfMountain1'),
        (r'^moonMountain', 'mountain/moonMountain'),
        (r'^village1', 'village/village1'),
        (r'^village2', 'village/village2'),
    ]

    for pattern, area in nested_patterns:
        if re.match(pattern, map_name):
            return area

    # Simple seasonal maps (farm, beach, pond, road, etc.)
    simple_areas = ['farm', 'beach', 'pond', 'road', 'square', 'vineyard',
                    'ranch', 'raceTrack']

    for area in simple_areas:
        if map_name.startswith(area):
            return area

    # Default: use the map name as the area
    return map_name


def get_map_variant(map_name: str) -> str:
    """
    Get the variant/season subdirectory for a map.

    For seasonal maps, returns the season (lowercase).
    For non-seasonal maps in shared areas, returns the map name.
    For standalone maps, returns the map name.

    Examples:
        farmSpring -> spring
        mountain1Winter -> winter
        barn -> barn
        bakery -> bakery
    """
    # Check for seasonal suffix
    seasonal_match = re.search(r'(Spring|Summer|Fall|Winter)$', map_name)
    if seasonal_match:
        return seasonal_match.group(1).lower()

    # Non-seasonal: use map name as variant
    return map_name


def get_map_area_path(map_name: str) -> str:
    """
    Get the full directory path for a map (area + variant).

    Examples:
        farmSpring -> farm/spring
        mountain1Spring -> mountain/mountain1/spring
        barn -> farm/barn
        cave -> cave
        bakery -> bakery
    """
    area = get_map_area(map_name)
    variant = get_map_variant(map_name)

    # Avoid redundant paths like cave/cave or bakery/bakery
    # If the area ends with the variant name, just use the area
    if area == variant or area.endswith(f"/{variant}"):
        return area

    return f"{area}/{variant}"

rom = None

def set_rom():
    global rom
    rom = memoryview(ROM_PATH.read_bytes())

def get_raw_bytes(start: int, end: int) -> bytes:
    return rom[start:end].tobytes()

def get_asset_offsets_array(asset_addresses: List[str]) -> np.ndarray:
    asset_index_base = int(asset_addresses[0], 16)
    # element 12 is always 0 padding
    return np.frombuffer(rom, dtype=np.dtype(">u4"), count=12, offset=asset_index_base)

def get_map_sprite_textures_offsets_array(map_start, tile_texture_offsets_start) -> np.ndarray:

    first_offset = np.frombuffer(rom, dtype=np.dtype(">u4"), count=1, offset=map_start + tile_texture_offsets_start).item()

    count = first_offset // 4

    offsets = np.frombuffer(rom, dtype=np.dtype(">u4"), count=count, offset=map_start + tile_texture_offsets_start)

    # handle offset arrays that have 0 padding at the end
    # get indices of all non-zero elements 
    nonzeros = np.nonzero(offsets)[0]
    # get last non-zero index
    last = nonzeros[-1] if len(nonzeros) > 0 else 0
    
    # trim original array
    return offsets[:last+1]

def get_map_sprite_palettes_offsets_array(map_start, tile_palette_offsets_start) -> np.ndarray:

    first_offset = np.frombuffer(rom, dtype=np.dtype(">u4"), count=1, offset=map_start + tile_palette_offsets_start).item()

    count = first_offset // 4

    offsets = np.frombuffer(rom, dtype=np.dtype(">u4"), count=count, offset=map_start + tile_palette_offsets_start)

    # handle offset arrays that have 0 padding at the end
    # get indices of all non-zero elements 
    nonzeros = np.nonzero(offsets)[0]
    # get last non-zero index
    last = nonzeros[-1] if len(nonzeros) > 0 else 0
    
    offsets = offsets[:last+1]

    # trim original array
    return offsets[:last+1]

def get_map_sprite_texture(texture_start, texture_end) -> bytes:
    # skip header + dimensions
    return get_raw_bytes(texture_start + 8, texture_end)

def get_map_sprite_palette(palette_start, palette_end) -> bytes:
    
    # skip header
    palette_bytes = get_raw_bytes(palette_start + 4, palette_end)

    # Slice to 255 u16 entries to handle palettes with messed up data
    # 255 * 2 = 510 bytes.
    return palette_bytes[:510]

def get_map_sprite_texture_width_and_height(texture_start):

    width = np.frombuffer(rom, dtype=np.dtype("<i2"), count=1, offset=texture_start + 4).item()
    height = np.frombuffer(rom, dtype=np.dtype("<i2"), count=1, offset=texture_start + 6).item()
    
    return (width, height)

def get_map_sprite_texture_type(texture_start):

    texture_flags = np.frombuffer(rom, dtype=np.dtype("u1"), count=1, offset=texture_start + 3).item()

    if texture_flags == 16:
        return 'ci4'
    else:
        return 'ci8'

    return texture_type

def analyze_map_textures(output_json: bool = True) -> dict:
    """
    Analyze all map textures and gather statistics about formats, dimensions, etc.

    Returns:
        Dictionary with analysis results
    """
    if rom == None:
        set_rom()

    with open(MAP_ADDRESSES_CSV_PATH) as f:
        reader = csv.reader(f)
        addresses_list = list(tuple(line) for line in reader)

    # Statistics collectors
    stats = {
        "tile_textures": {
            "total": 0,
            "ci4_count": 0,
            "ci8_count": 0,
            "dimensions": {},  # "WxH": count
            "by_map": {},      # map_name: {ci4, ci8, dimensions}
        },
        "core_object_textures": {
            "total": 0,
            "ci4_count": 0,
            "ci8_count": 0,
            "dimensions": {},
            "by_map": {},
        },
        "unique_dimensions": set(),
        "format_details": []  # List of all textures with details
    }

    print("=" * 70)
    print("MAP TEXTURE ANALYSIS")
    print("=" * 70)

    for idx, row in enumerate(addresses_list):
        map_name = row[1]

        if map_name.startswith("empty") or map_name == "end":
            continue

        map_base = int(row[0], 16)
        asset_offsets_array = get_asset_offsets_array(row)

        # Initialize per-map stats
        map_tile_stats = {"ci4": 0, "ci8": 0, "dimensions": {}, "textures": []}
        map_obj_stats = {"ci4": 0, "ci8": 0, "dimensions": {}, "textures": []}

        # ===== TILE TEXTURES =====
        tile_texture_start = asset_offsets_array[5]
        tile_palette_start = asset_offsets_array[6]

        if tile_texture_start > 0:
            tile_textures_offsets_array = get_map_sprite_textures_offsets_array(map_base, tile_texture_start)
            tile_palette_offsets_array = get_map_sprite_palettes_offsets_array(map_base, tile_palette_start)

            for i in range(len(tile_textures_offsets_array) - 1):
                texture_start = map_base + tile_texture_start + tile_textures_offsets_array[i]

                width, height = get_map_sprite_texture_width_and_height(texture_start)
                texture_type = get_map_sprite_texture_type(texture_start)

                # Get raw flag byte for detailed analysis
                flag_byte = np.frombuffer(rom, dtype=np.dtype("u1"), count=1, offset=texture_start + 3).item()

                # Calculate texture data size
                texture_end = map_base + tile_texture_start + tile_textures_offsets_array[i + 1]
                data_size = texture_end - texture_start - 8  # minus header

                # Calculate palette size
                palette_start = map_base + tile_palette_start + tile_palette_offsets_array[i]
                palette_end = map_base + tile_palette_start + tile_palette_offsets_array[i + 1]
                palette_size = (palette_end - palette_start - 4) // 2  # colors (2 bytes each)

                dim_key = f"{width}x{height}"

                # Update stats
                stats["tile_textures"]["total"] += 1
                if texture_type == "ci4":
                    stats["tile_textures"]["ci4_count"] += 1
                    map_tile_stats["ci4"] += 1
                else:
                    stats["tile_textures"]["ci8_count"] += 1
                    map_tile_stats["ci8"] += 1

                stats["tile_textures"]["dimensions"][dim_key] = \
                    stats["tile_textures"]["dimensions"].get(dim_key, 0) + 1
                map_tile_stats["dimensions"][dim_key] = \
                    map_tile_stats["dimensions"].get(dim_key, 0) + 1
                stats["unique_dimensions"].add(dim_key)

                # Detailed record
                tex_detail = {
                    "map": map_name,
                    "type": "tile",
                    "index": i,
                    "format": texture_type,
                    "flag_byte": f"0x{flag_byte:02X}",
                    "width": width,
                    "height": height,
                    "data_size": data_size,
                    "palette_colors": palette_size
                }
                map_tile_stats["textures"].append(tex_detail)
                stats["format_details"].append(tex_detail)

        # ===== CORE OBJECT TEXTURES =====
        core_obj_texture_start = asset_offsets_array[7]
        core_obj_palette_start = asset_offsets_array[8]

        if core_obj_texture_start > 0:
            try:
                obj_textures_offsets_array = get_map_sprite_textures_offsets_array(map_base, core_obj_texture_start)
                obj_palette_offsets_array = get_map_sprite_palettes_offsets_array(map_base, core_obj_palette_start)

                for i in range(len(obj_textures_offsets_array) - 1):
                    texture_start = map_base + core_obj_texture_start + obj_textures_offsets_array[i]

                    width, height = get_map_sprite_texture_width_and_height(texture_start)
                    texture_type = get_map_sprite_texture_type(texture_start)
                    flag_byte = np.frombuffer(rom, dtype=np.dtype("u1"), count=1, offset=texture_start + 3).item()

                    texture_end = map_base + core_obj_texture_start + obj_textures_offsets_array[i + 1]
                    data_size = texture_end - texture_start - 8

                    palette_start = map_base + core_obj_palette_start + obj_palette_offsets_array[i]
                    palette_end = map_base + core_obj_palette_start + obj_palette_offsets_array[i + 1]
                    palette_size = (palette_end - palette_start - 4) // 2

                    dim_key = f"{width}x{height}"

                    stats["core_object_textures"]["total"] += 1
                    if texture_type == "ci4":
                        stats["core_object_textures"]["ci4_count"] += 1
                        map_obj_stats["ci4"] += 1
                    else:
                        stats["core_object_textures"]["ci8_count"] += 1
                        map_obj_stats["ci8"] += 1

                    stats["core_object_textures"]["dimensions"][dim_key] = \
                        stats["core_object_textures"]["dimensions"].get(dim_key, 0) + 1
                    map_obj_stats["dimensions"][dim_key] = \
                        map_obj_stats["dimensions"].get(dim_key, 0) + 1
                    stats["unique_dimensions"].add(dim_key)

                    tex_detail = {
                        "map": map_name,
                        "type": "core_object",
                        "index": i,
                        "format": texture_type,
                        "flag_byte": f"0x{flag_byte:02X}",
                        "width": width,
                        "height": height,
                        "data_size": data_size,
                        "palette_colors": palette_size
                    }
                    map_obj_stats["textures"].append(tex_detail)
                    stats["format_details"].append(tex_detail)
            except Exception as e:
                print(f"  Warning: Could not analyze core objects for {map_name}: {e}")

        # Store per-map stats
        stats["tile_textures"]["by_map"][map_name] = map_tile_stats
        stats["core_object_textures"]["by_map"][map_name] = map_obj_stats

    # Convert set to list for JSON serialization
    stats["unique_dimensions"] = sorted(list(stats["unique_dimensions"]))

    # Print summary
    print("\n" + "=" * 70)
    print("TILE TEXTURES SUMMARY")
    print("=" * 70)
    print(f"Total textures: {stats['tile_textures']['total']}")
    print(f"CI4 (16 colors):  {stats['tile_textures']['ci4_count']} ({100*stats['tile_textures']['ci4_count']/max(1,stats['tile_textures']['total']):.1f}%)")
    print(f"CI8 (256 colors): {stats['tile_textures']['ci8_count']} ({100*stats['tile_textures']['ci8_count']/max(1,stats['tile_textures']['total']):.1f}%)")
    print("\nDimensions distribution:")
    for dim, count in sorted(stats["tile_textures"]["dimensions"].items(), key=lambda x: -x[1]):
        print(f"  {dim}: {count}")

    print("\n" + "=" * 70)
    print("CORE OBJECT TEXTURES SUMMARY")
    print("=" * 70)
    print(f"Total textures: {stats['core_object_textures']['total']}")
    print(f"CI4 (16 colors):  {stats['core_object_textures']['ci4_count']} ({100*stats['core_object_textures']['ci4_count']/max(1,stats['core_object_textures']['total']):.1f}%)")
    print(f"CI8 (256 colors): {stats['core_object_textures']['ci8_count']} ({100*stats['core_object_textures']['ci8_count']/max(1,stats['core_object_textures']['total']):.1f}%)")
    print("\nDimensions distribution:")
    for dim, count in sorted(stats["core_object_textures"]["dimensions"].items(), key=lambda x: -x[1]):
        print(f"  {dim}: {count}")

    print("\n" + "=" * 70)
    print("UNIQUE DIMENSIONS (all textures)")
    print("=" * 70)
    for dim in stats["unique_dimensions"]:
        print(f"  {dim}")

    # Find any unusual flag bytes (not 0x10 for CI4 or 0x00 for CI8)
    unusual_flags = set()
    for detail in stats["format_details"]:
        flag = int(detail["flag_byte"], 16)
        if flag not in (0x00, 0x10):
            unusual_flags.add(detail["flag_byte"])

    if unusual_flags:
        print("\n" + "=" * 70)
        print("UNUSUAL FORMAT FLAGS DETECTED")
        print("=" * 70)
        for flag in unusual_flags:
            examples = [d for d in stats["format_details"] if d["flag_byte"] == flag][:3]
            print(f"\nFlag {flag}:")
            for ex in examples:
                print(f"  {ex['map']} {ex['type']} #{ex['index']}: {ex['width']}x{ex['height']}")

    # Optionally write to JSON
    if output_json:
        json_path = MAPS_DIR / "texture_analysis.json"
        # Remove format_details for cleaner JSON (too verbose)
        json_stats = {k: v for k, v in stats.items() if k != "format_details"}
        with open(json_path, 'w') as f:
            json.dump(json_stats, f, indent=2)
        print(f"\nDetailed analysis written to: {json_path}")

    return stats


def write_tile_textures() -> None:

    if rom == None:
        set_rom()

    with open(MAP_ADDRESSES_CSV_PATH) as f:
        reader = csv.reader(f)
        addresses_list = list(tuple(line) for line in reader)

    basedir = MAPS_DIR
    basedir.mkdir(parents=True, exist_ok=True)

    for idx, row in enumerate(addresses_list):

        map_name = row[1]

        # Skip empty maps and end marker
        if map_name.startswith("empty") or map_name == "end":
            continue

        map_base = int(row[0], 16)
        asset_offsets_array = get_asset_offsets_array(row)

        tile_texture_start = asset_offsets_array[5]
        tile_palette_start = asset_offsets_array[6]

        tile_textures_offsets_array = get_map_sprite_textures_offsets_array(map_base, tile_texture_start)
        tile_palette_offsets_array = get_map_sprite_palettes_offsets_array(map_base, tile_palette_start)

        # Use area-based directory structure: maps/{area}/{variant}/tiles/
        # get_map_area_path returns the full path including variant (e.g., farm/spring)
        area_path = get_map_area_path(map_name)
        outdir = basedir / area_path / "tiles"
        outdir.mkdir(parents=True, exist_ok=True)

        # last element is end address/start of palette
        for i in range(len(tile_textures_offsets_array) - 1):

            texture_start = map_base + tile_texture_start + tile_textures_offsets_array[i]
            texture_end = map_base + tile_texture_start + tile_textures_offsets_array[i + 1]

            palette_start = map_base + tile_palette_start + tile_palette_offsets_array[i]
            palette_end = map_base + tile_palette_start + tile_palette_offsets_array[i + 1]

            tile_texture = get_map_sprite_texture(texture_start, texture_end)
            tile_palette = get_map_sprite_palette(palette_start, palette_end)

            width, height = get_map_sprite_texture_width_and_height(texture_start)
            texture_type = get_map_sprite_texture_type(texture_start)

            filename = f"tile-{i}"

            write_texture(tile_texture, tile_palette, width, height, texture_type, filename, outdir, False)

    return

def write_core_map_object_textures() -> None:

    if rom == None:
        set_rom()

    with open(MAP_ADDRESSES_CSV_PATH) as f:
        reader = csv.reader(f)
        addresses_list = list(tuple(line) for line in reader)
    
    basedir = MAPS_DIR
    basedir.mkdir(parents=True, exist_ok=True)

    for idx, row in enumerate(addresses_list):

        map_name = row[1]

        # Skip empty maps and end marker
        if map_name.startswith("empty") or map_name == "end":
            continue

        map_base = int(row[0], 16)
        asset_offsets_array = get_asset_offsets_array(row)

        core_map_object_sprites_texture_start = asset_offsets_array[7]
        core_map_object_sprites_palette_start = asset_offsets_array[8]

        core_map_object_sprites_textures_offsets_array = get_map_sprite_textures_offsets_array(map_base, core_map_object_sprites_texture_start)
        core_map_object_sprites_palette_offsets_array = get_map_sprite_palettes_offsets_array(map_base, core_map_object_sprites_palette_start)

        # Use area-based directory structure: maps/{area}/{variant}/core_map_objects/
        # get_map_area_path returns the full path including variant (e.g., farm/spring)
        area_path = get_map_area_path(map_name)
        outdir = basedir / area_path / "core_map_objects"
        outdir.mkdir(parents=True, exist_ok=True)

        # last element is end address/start of palette
        for i in range(len(core_map_object_sprites_textures_offsets_array) - 1):

            texture_start = map_base + core_map_object_sprites_texture_start + core_map_object_sprites_textures_offsets_array[i]
            texture_end = map_base + core_map_object_sprites_texture_start + core_map_object_sprites_textures_offsets_array[i + 1]

            palette_start = map_base + core_map_object_sprites_palette_start + core_map_object_sprites_palette_offsets_array[i]
            palette_end = map_base + core_map_object_sprites_palette_start + core_map_object_sprites_palette_offsets_array[i + 1]

            core_map_object_sprites_texture = get_map_sprite_texture(texture_start, texture_end)
            core_map_object_sprites_palette = get_map_sprite_palette(palette_start, palette_end)

            width, height = get_map_sprite_texture_width_and_height(texture_start)
            texture_type = get_map_sprite_texture_type(texture_start)

            filename = f"map_object-{i}"

            write_texture(core_map_object_sprites_texture, core_map_object_sprites_palette, width, height, texture_type, filename, outdir, False) 

    return

if __name__ == "__main__":
    
    if len(sys.argv) < 2:
        print("Usage: python hm64_map_utitilies.py write_tile_textures | write_core_map_object_textures | write_all_textures")
        sys.exit(1)

    cmd = sys.argv[1]

    if cmd == "write_tile_textures":
        write_tile_textures()
    elif cmd == "write_core_map_object_textures":
        write_core_map_object_textures()
    elif cmd == "write_all_textures":
        write_tile_textures()
        write_core_map_object_textures()
    else:
        print(f"Unknown command: {cmd}")