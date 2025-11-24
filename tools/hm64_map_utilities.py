import csv
import json
import numpy as np
from n64img.image import CI4, CI8, Image
from pathlib import Path
from typing import List, Tuple, Union
import sys

from hm64_sprite_utilities import *

MAPS_DIR = Path("../assets/maps")
MAP_ADDRESSES_CSV_PATH = Path("map_addresses.csv")
ROM_PATH = Path("../baserom.us.z64")

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
        if map_name.startswith("empty-map") or map_name == "end":
            continue

        map_base = int(row[0], 16)
        asset_offsets_array = get_asset_offsets_array(row)

        tile_texture_start = asset_offsets_array[5]
        tile_palette_start = asset_offsets_array[6]

        tile_textures_offsets_array = get_map_sprite_textures_offsets_array(map_base, tile_texture_start)
        tile_palette_offsets_array = get_map_sprite_palettes_offsets_array(map_base, tile_palette_start)

        dir_name = f"{map_name}"
        subdir_name = "tiles"

        outdir = basedir / dir_name / subdir_name
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
        if map_name.startswith("empty-map") or map_name == "end":
            continue

        map_base = int(row[0], 16)
        asset_offsets_array = get_asset_offsets_array(row)

        core_map_object_sprites_texture_start = asset_offsets_array[7]
        core_map_object_sprites_palette_start = asset_offsets_array[8]

        core_map_object_sprites_textures_offsets_array = get_map_sprite_textures_offsets_array(map_base, core_map_object_sprites_texture_start)
        core_map_object_sprites_palette_offsets_array = get_map_sprite_palettes_offsets_array(map_base, core_map_object_sprites_palette_start)

        dir_name = f"{map_name}"
        subdir_name = "core_map_objects"

        outdir = basedir / dir_name / subdir_name
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