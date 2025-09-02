import csv
import json
import numpy as np
from n64img.image import CI4, CI8, Image
from pathlib import Path
from typing import List, Tuple, Union
import sys

SPRITES_DIR = Path("../assets/sprites")
SPRITE_ADDRESSES_CSV_PATH = Path("sprite_addresses.csv")
ROM_PATH = Path("../baserom.us.z64")

rom = None

def set_rom():
    global rom
    rom = memoryview(ROM_PATH.read_bytes())

def check_asset_addresses_exist(label: str) -> Tuple[int, str]:

    with open("sprite_addresses.csv", newline="", encoding="utf-8") as f:
        reader = csv.reader(f)
        for idx, row in enumerate(reader, start=0):
            if len(row) == 5:
                if row[4] == label:
                    return (idx,"type-1")
            elif len(row) == 4:
                if row[3] == label:
                    return (idx,"type-2")
            else:
                continue

        return (-1, "none")

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

    if (sprite_index > len(spritesheet_offsets_array)):
        return -1

    if (len(spritesheet_offsets_array) == 0):
        return -1
    
    else:
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

def get_texture(asset_addresses: List[str], sprite_index: int, asset_type: str) -> np.ndarray:

    spritesheet_base = int(asset_addresses[0], 16)

    spritesheet_offsets_array = get_spritesheet_offsets_array(asset_addresses, asset_type)
    sprite_address = get_sprite_address(asset_addresses, spritesheet_offsets_array, sprite_index)

    width, height = get_texture_width_and_height(asset_addresses, sprite_index, asset_type)

    if width == -1 or height == -1:
        return []

    if spritesheet_offsets_array[sprite_index] == spritesheet_offsets_array[sprite_index + 1]:
        print(f"Duplicate sprite for: {asset_addresses}")
        print(f"Index: {sprite_index}")
        return []  

    texture_start = sprite_address + 8
    texture_end = spritesheet_base + spritesheet_offsets_array[sprite_index + 1]  

    if (texture_start > texture_end):
        return []

    texture_length = texture_end - texture_start

    return np.frombuffer(rom, dtype=np.dtype("u1"), count=texture_length, offset=texture_start)

def get_palette(asset_addresses: List[str], sprite_index: int, asset_type: str) -> np.ndarray:

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
        #skip header
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

    return np.frombuffer(rom, dtype=np.dtype("<u2"), count=palette_length, offset=palette_start_address)

def write_all_textures() -> None:

    if rom == None:
        set_rom()

    outdir = SPRITES_DIR
    outdir.mkdir(parents=True, exist_ok=True)

    with open(SPRITE_ADDRESSES_CSV_PATH) as f:
        reader = csv.reader(f)
        addresses_list = list(tuple(line) for line in reader)

    for idx, row in enumerate(addresses_list):
        if len(row) == 5:
            write_textures_for_sprite(row[4])
        elif len(row) == 4:
            write_textures_for_sprite(row[3])
        else:
            continue

    return

def write_textures_for_sprite(sprite_name: str) -> None:

    if rom == None:
        set_rom()

    result, asset_type = check_asset_addresses_exist(sprite_name)

    if result == -1:
        printf("Invalid sprite name: {sprite_name}")
        return

    basedir = SPRITES_DIR
    basedir.mkdir(parents=True, exist_ok=True)

    outdir = basedir / sprite_name
    outdir.mkdir(parents=True, exist_ok=True)

    with open(SPRITE_ADDRESSES_CSV_PATH) as f:
        reader = csv.reader(f)
        addresses_list = list(tuple(line) for line in reader)

    asset_addresses = addresses_list[result]

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
            pritn(f"Height: {height}")
            continue

        palette = get_palette(asset_addresses, sprite_number, asset_type)

        if len(palette) == 0:
            print(f"Unable to fetch palette: {asset_addresses}")
            continue

        # start count at 1    
        filename = sprite_name + '/texture-' + str(sprite_number + 1)

        write_texture(texture, palette, width, height, texture_type, filename, SPRITES_DIR, False)
    
    return

def write_texture_from_csv_row_and_index(row: int, sprite_index: int, filename: str, output_path: Path) -> None:

    if rom == None:
        set_rom()

    with open(SPRITE_ADDRESSES_CSV_PATH) as f:
        reader = csv.reader(f)
        addresses_list = list(tuple(line) for line in reader)

    asset_addresses = addresses_list[row]

    if (len(asset_addresses) == 5):
        asset_type = "type-1"
    elif (len(asset_addresses) == 4):
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

def write_texture(texture: np.ndarray, palette: np.ndarray, width: int, height: int, texture_type: str, filename: str, output_path: Path, keep_intermediate_files: bool = True) -> None:

    # FIXME: see if it's possible to convert the texture and palette arrays to a binary blob in memory before passing to n64img  

    tex_file = open(str(output_path) + '/%s.ci' % filename, 'wb')
    tex_file.write(texture)
    tex_file.close()

    pal_file = open(str(output_path) + '/%s.pal' % filename, 'wb')
    pal_file.write(palette)
    pal_file.close()

    tex_in = open(str(output_path) + '/%s.ci' % filename, 'rb')
    tex_data = tex_in.read()
    
    pal_in = open(str(output_path) + '/%s.pal' % filename, 'rb')
    pal_data = pal_in.read()

    # write texture

    if texture_type == 'ci8':
        img = CI8(tex_data, width, height)
    else:
        img = CI4(tex_data, width, height)

    img.set_palette(pal_data)

    img.write(str(output_path) + '/%s.png' % filename)

    tex_in.close()
    pal_in.close()

    if keep_intermediate_files == False:
        Path(str(output_path) + '/%s.ci' % filename).unlink()
        Path(str(output_path) + '/%s.pal' % filename).unlink()

    return


if __name__ == "__main__":
    
    if len(sys.argv) < 2:
        print("Usage: python hm64_animation_utitilies.py write_all_textures | write_textures_for_sprite (sprite_name)")
        sys.exit(1)

    cmd = sys.argv[1]

    if cmd == "write_textures_for_sprite":
        write_textures_for_sprite(sprite_name)
    elif cmd == "write_all_textures":
        write_all_textures()
    else:
        print(f"Unknown command: {cmd}")