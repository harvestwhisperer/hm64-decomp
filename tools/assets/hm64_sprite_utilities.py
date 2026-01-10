import csv
import json
import numpy as np
from n64img.image import CI4, CI8, Image
from pathlib import Path
from typing import List, Tuple, Union, Optional, Dict
import sys
import argparse

# Make paths relative to this script's location
SCRIPT_DIR = Path(__file__).parent.resolve()
SPRITES_DIR = SCRIPT_DIR / "../../assets/sprites"
SPRITE_ADDRESSES_CSV_PATH = SCRIPT_DIR / "sprite_addresses.csv"
ROM_PATH = SCRIPT_DIR / "../../baserom.us.z64"

_rom: Optional[memoryview] = None
_csv_cache: Optional[List[List[str]]] = None
_label_to_row_cache: Optional[Dict[str, Tuple[int, str, List[str]]]] = None

# Legacy alias for backward compatibility
rom = None

KEEP_INTERMEDIATE_FILES = False


def _ensure_caches():
    """Ensure CSV and label caches are populated."""
    global _csv_cache, _label_to_row_cache

    if _csv_cache is None:
        _load_csv_cache()


def _load_csv_cache():
    """Load and cache CSV data."""
    global _csv_cache, _label_to_row_cache

    _csv_cache = []
    _label_to_row_cache = {}

    with open(SPRITE_ADDRESSES_CSV_PATH, newline="", encoding="utf-8") as f:
        reader = csv.reader(f)
        for idx, row in enumerate(reader):
            row = [cell.strip() for cell in row]
            _csv_cache.append(row)

            # Build label lookup
            if len(row) == 6:
                label = row[4]
                asset_type = "type-1"
            elif len(row) == 5:
                label = row[3]
                asset_type = "type-2"
            else:
                continue

            _label_to_row_cache[label] = (idx, asset_type, row)


def clear_caches():
    """Clear all caches (useful for testing or reloading)."""
    global _rom, _csv_cache, _label_to_row_cache, rom
    _rom = None
    _csv_cache = None
    _label_to_row_cache = None
    rom = None


def get_cached_row_by_index(idx: int) -> Optional[List[str]]:
    """Get cached CSV row by index."""
    _ensure_caches()
    if 0 <= idx < len(_csv_cache):
        return _csv_cache[idx]
    return None

def set_rom():
    """Load ROM into memory (cached)."""
    global rom, _rom
    if _rom is None:
        _rom = memoryview(ROM_PATH.read_bytes())
    rom = _rom


def get_rom() -> memoryview:
    """Get ROM memoryview, loading if necessary."""
    global _rom, rom
    if _rom is None:
        set_rom()
    return _rom


def check_asset_addresses_exist(label: str) -> Tuple[int, str]:
    """
    Check if asset exists and return (row_index, asset_type).
    Uses cached CSV data for performance.
    """
    _ensure_caches()

    if label in _label_to_row_cache:
        idx, asset_type, _ = _label_to_row_cache[label]
        return (idx, asset_type)

    return (-1, "none")

def get_sprite_subdir(row: list) -> str:
    """Get the subdirectory from the CSV row (last column)."""
    if len(row) >= 5:
        return row[-1]
    return ""

def get_spritesheet_offsets_array(asset_addresses: List[str], asset_type: str) -> np.ndarray:

    if asset_type == "type-1":
        
        spritesheet_index_base = int(asset_addresses[2], 16)
        end_address = int(asset_addresses[3], 16)

        spritesheet_index_count = int((end_address - spritesheet_index_base) // 4)

        return np.frombuffer(rom, dtype=np.dtype(">u4"), count=spritesheet_index_count, offset=spritesheet_index_base)

    elif asset_type == "type-2":

        spritesheet_index_base = int(asset_addresses[0], 16)
        first_sprite_offset = np.frombuffer(rom, dtype=np.dtype(">u4"), count=1, offset=spritesheet_index_base).item()

        # distance from spritsheet base / size of data
        spritesheet_index_count = first_sprite_offset // 4

        return np.frombuffer(rom, dtype=np.dtype(">u4"), count=spritesheet_index_count, offset=spritesheet_index_base)

    else:
        return []

def get_asset_offsets_array(asset_addresses: List[str]) -> np.ndarray:

    asset_index_base = int(asset_addresses[1], 16)
    
    return np.frombuffer(rom, dtype=np.dtype(">u4"), count=8, offset=asset_index_base)

def get_palette_offsets_array(asset_addresses: List[str], asset_offsets_array: np.ndarray) -> np.ndarray:

    sprite_base = int(asset_addresses[0], 16)
    palette_index_base = int(sprite_base + asset_offsets_array[1])

    first_palette_offset = np.frombuffer(rom, dtype=np.dtype(">u4"), count=1, offset=palette_index_base).item()
    palette_count = int(first_palette_offset // 4)
    
    return np.frombuffer(rom, dtype=np.dtype(">u4"), count=palette_count, offset=palette_index_base)

def get_sprite_count(asset_addresses: List[str], asset_type: str) -> int:

    if asset_type == "type-1":

        spritesheet_offsets_array = get_spritesheet_offsets_array(asset_addresses, asset_type)

        spritesheet_index_base = int(asset_addresses[2], 16)
        end_address = int(asset_addresses[3], 16)

        sprite_count = int((end_address - spritesheet_index_base) // 4)

        if len(spritesheet_offsets_array) > 2:
            while spritesheet_offsets_array[-1] == 0:
                spritesheet_offsets_array = spritesheet_offsets_array[:-1]
                sprite_count -= 1
    
        # last offset is offset for palette index
        return sprite_count - 1

    elif asset_type == "type-2":

        spritesheet_offsets_array = get_spritesheet_offsets_array(asset_addresses, asset_type)

        spritesheet_index_base = int(asset_addresses[0], 16)
        spritesheet_first_index = np.frombuffer(rom, dtype=np.dtype(">u4"), count=1, offset=spritesheet_index_base).item()

        sprite_base = spritesheet_index_base + spritesheet_first_index

        sprite_count = int((sprite_base - spritesheet_index_base) // 4)

        if len(spritesheet_offsets_array) > 2:
            while spritesheet_offsets_array[-1] == 0:
                spritesheet_offsets_array = spritesheet_offsets_array[:-1]
                sprite_count -= 1
    
        # last offset is offset for palette index
        return sprite_count - 1

    else:
        return -1

def get_sprite_to_palette_array(asset_addresses: List[str], asset_offsets_array: np.ndarray, asset_type: str) -> np.ndarray:
    
    sprite_base = int(asset_addresses[0], 16)

    # skip header
    sprite_to_palette_index_base = int(sprite_base + asset_offsets_array[3]) + 4

    sprite_count = get_sprite_count(asset_addresses, asset_type)

    if sprite_count is None or sprite_count == -1:
        print(f"Invalid sprite count: {asset_addresses}")
        return []
    
    return np.frombuffer(rom, dtype=np.dtype("u1"), count=sprite_count, offset=sprite_to_palette_index_base)

def get_sprite_address(asset_addresses: List[str], spritesheet_offsets_array: np.ndarray, sprite_index: int) -> int:

    sprite_asset_base = int(asset_addresses[0], 16)

    if len(spritesheet_offsets_array) == 0:
        return -1

    if sprite_index >= len(spritesheet_offsets_array):
        return -1

    return sprite_asset_base + spritesheet_offsets_array[sprite_index]


def get_texture_type(asset_addresses: List[str], sprite_index: int, asset_type: str) -> str:

    spritesheet_offsets_array = get_spritesheet_offsets_array(asset_addresses, asset_type)
    sprite_address = get_sprite_address(asset_addresses, spritesheet_offsets_array, sprite_index)

    texture_flags = np.frombuffer(rom, dtype=np.dtype("u1"), count=1, offset=sprite_address + 3).item()

    if texture_flags == 16:
        return 'ci4'
    else:
        return 'ci8'

def get_texture_width_and_height(asset_addresses: List[str], sprite_index: int, asset_type: str) -> Tuple[int, int]:

    spritesheet_offsets_array = get_spritesheet_offsets_array(asset_addresses, asset_type)
    sprite_address = get_sprite_address(asset_addresses, spritesheet_offsets_array, sprite_index)

    if sprite_address == -1:
        return (-1, -1)

    width = np.frombuffer(rom, dtype=np.dtype("<i2"), count=1, offset=sprite_address + 4).item()
    height = np.frombuffer(rom, dtype=np.dtype("<i2"), count=1, offset=sprite_address + 6).item()
    
    return (width, height)

def get_texture(asset_addresses: List[str], sprite_index: int, asset_type: str) -> bytes:

    spritesheet_base = int(asset_addresses[0], 16)

    spritesheet_offsets_array = get_spritesheet_offsets_array(asset_addresses, asset_type)
    sprite_address = get_sprite_address(asset_addresses, spritesheet_offsets_array, sprite_index)

    width, height = get_texture_width_and_height(asset_addresses, sprite_index, asset_type)

    if width == -1 or height == -1:
        return []

    # Check bounds before accessing sprite_index + 1
    if sprite_index + 1 >= len(spritesheet_offsets_array):
        # Last sprite - can't determine texture end from next offset
        return []

    if spritesheet_offsets_array[sprite_index] == spritesheet_offsets_array[sprite_index + 1]:
        print(f"Duplicate sprite for: {asset_addresses}")
        print(f"Index: {sprite_index}")
        return []

    texture_start = sprite_address + 8
    texture_end = spritesheet_base + spritesheet_offsets_array[sprite_index + 1]

    if (texture_start > texture_end):
        return bytes()

    return rom[texture_start:texture_end].tobytes()

def get_palette(asset_addresses: List[str], sprite_index: int, asset_type: str) -> bytes:

    sprite_base = int(asset_addresses[0], 16)

    asset_offsets_array = get_asset_offsets_array(asset_addresses)
    palette_offsets_array = get_palette_offsets_array(asset_addresses, asset_offsets_array)
    sprite_to_palette_array = get_sprite_to_palette_array(asset_addresses, asset_offsets_array, asset_type)

    if len(sprite_to_palette_array) == 0:
        return []

    palette_index_base = int(sprite_base + asset_offsets_array[1])

    current_palette_index = sprite_to_palette_array[sprite_index]
    palette_start_address = palette_index_base + palette_offsets_array[current_palette_index]

    # start at header of next palette
    palette_end_address = palette_index_base + palette_offsets_array[current_palette_index + 1]

    if palette_end_address != palette_start_address:
        # skip header
        palette_start_address += 4
        # subtract padding
        palette_end_address -= 4
    # some palette offsets are the same; occurs with dialogue icons
    else:
        j = 2
        # keep checking subsequent palettes, stopping before padding at end of sprite to palette array
        while (palette_end_address == palette_start_address) and (current_palette_index + 2 < len(sprite_to_palette_array)):
            # subtract header
            palette_end_address = palette_index_base + palette_offsets_array[current_palette_index + j] - 4
            j+=1    
        if palette_end_address == palette_start_address:
            print(f"error getting palette length for sprite: {asset_addresses}")
            return []
        else:
            palette_start_address += 4

    palette_length = int((palette_end_address - palette_start_address) // 2)

    return rom[palette_start_address:palette_end_address].tobytes()

def write_all_textures() -> None:
    """Write all textures for all sprites. Uses cached CSV data."""

    _ensure_caches()
    set_rom()

    outdir = SPRITES_DIR
    outdir.mkdir(parents=True, exist_ok=True)

    for idx, row in enumerate(_csv_cache):
        subdir = get_sprite_subdir(row)
        if len(row) == 6:
            write_textures_for_sprite(row[4], subdir)
        elif len(row) == 5:
            write_textures_for_sprite(row[3], subdir)
        else:
            continue

    return

def write_textures_for_sprite(sprite_name: str, subdir: str = "") -> None:
    """Write all textures for a sprite. Uses cached CSV data."""

    _ensure_caches()
    set_rom()

    result, asset_type = check_asset_addresses_exist(sprite_name)

    if result == -1:
        print(f"Invalid sprite name: {sprite_name}")
        return

    # Build output path: SPRITES_DIR / subdir / sprite_name
    if subdir:
        basedir = SPRITES_DIR / subdir
    else:
        basedir = SPRITES_DIR
    basedir.mkdir(parents=True, exist_ok=True)

    outdir = basedir / sprite_name
    outdir.mkdir(parents=True, exist_ok=True)

    asset_addresses = get_cached_row_by_index(result)

    sprite_count = get_sprite_count(asset_addresses, asset_type)

    for sprite_number in range(sprite_count):

        if (sprite_number == sprite_count - 1):
            break

        texture = get_texture(asset_addresses, sprite_number, asset_type)

        if len(texture) == 0:
            continue

        texture_type = get_texture_type(asset_addresses, sprite_number, asset_type)
        width, height = get_texture_width_and_height(asset_addresses, sprite_number, asset_type)

        if width < 0 or height < 0:
            print(f"Invalid width or height: {asset_addresses}")
            print(f"Width: {width}")
            print(f"Height: {height}")
            continue

        palette = get_palette(asset_addresses, sprite_number, asset_type)

        if len(palette) == 0:
            print(f"Unable to fetch palette: {asset_addresses}")
            continue

        # start count at 1
        filename = sprite_name + '/texture-' + str(sprite_number + 1)

        write_texture(texture, palette, width, height, texture_type, filename, basedir, keep_intermediate_files=KEEP_INTERMEDIATE_FILES)

    return

def write_texture_from_csv_row_and_index(row: int, sprite_index: int, filename: str, output_path: Path) -> None:
    """Write a texture from CSV row index and sprite index. Uses cached CSV data."""

    _ensure_caches()
    set_rom()

    asset_addresses = get_cached_row_by_index(row)
    if asset_addresses is None:
        print(f"Invalid row index: {row}")
        return

    if (len(asset_addresses) == 6):
        asset_type = "type-1"
    elif (len(asset_addresses) == 5):
        asset_type = "type-2"
    else:
        print(f"Invalid CSV row: {asset_addresses}")
        return

    texture = get_texture(asset_addresses, sprite_index, asset_type)

    if len(texture) == 0:
        print(f"Unable to fetch texture: {asset_addresses}")
        return

    texture_type = get_texture_type(asset_addresses, sprite_index, asset_type)
    width, height = get_texture_width_and_height(asset_addresses, sprite_index, asset_type)

    palette = get_palette(asset_addresses, sprite_index, asset_type)

    if len(palette) == 0:
        print(f"Unable to fetch palette: {asset_addresses}")
        return

    write_texture(texture, palette, width, height, texture_type, filename, output_path, False)

    return

def write_texture(texture: bytes, palette: bytes, width: int, height: int, texture_type: str, filename: str, output_path: Path, keep_intermediate_files: bool = False) -> None:

    # Initialize n64img object with raw bytes
    if texture_type == 'ci8':
        img = CI8(texture, width, height)
    else:
        img = CI4(texture, width, height)

    # Set palette with raw Big Endian bytes (fixes transparency)
    img.set_palette(palette)

    # Ensure directory exists
    full_output_dir = output_path / filename.rsplit('/', 1)[0] if '/' in filename else output_path
    full_output_dir.mkdir(parents=True, exist_ok=True)

    # Write PNG
    out_file = output_path / f"{filename}.png"
    img.write(str(out_file))

    # Optional: Write raw dumps if requested
    if keep_intermediate_files:
        with open(output_path / f"{filename}.ci", 'wb') as f:
            f.write(texture)
        with open(output_path / f"{filename}.pal", 'wb') as f:
            f.write(palette)

    return


if __name__ == "__main__":

    parser = argparse.ArgumentParser(description="HM64 Sprite Utilities")
    
    parser.add_argument("command", help="The operation to perform")

    parser.add_argument("target", nargs="?", help="The sprite name (required for write_textures_for_sprite)")

    parser.add_argument(
        "--keep-intermediate-files", 
        action="store_true", 
        help="Keep intermediate .ci and .pal files"
    )

    args = parser.parse_args()

    KEEP_INTERMEDIATE_FILES = args.keep_intermediate_files

    if args.command == "write_textures_for_sprite":

        if not args.target:
            parser.error("The command 'write_textures_for_sprite' requires a sprite name.")

        # Look up subdir from CSV for the given sprite name
        with open(SPRITE_ADDRESSES_CSV_PATH) as f:

            reader = csv.reader(f)
            
            for row in reader:
                sprite_name_idx = 4 if len(row) == 6 else 3 if len(row) == 5 else -1
                if sprite_name_idx >= 0 and row[sprite_name_idx] == args.target:
                    subdir = get_sprite_subdir(row)
                    write_textures_for_sprite(args.target, subdir)
                    break
            else:
                print(f"Sprite not found in CSV: {args.target}")

    elif args.command == "write_all_textures":
        write_all_textures()

    else:
        print(f"Unknown command: {args.command}")