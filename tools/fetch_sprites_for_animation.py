import csv
import json
import numpy as np
from n64img.image import CI4, CI8, Image
from pathlib import Path
from typing import List

# hardcoded for now
JSON_DIR = Path("../assets/animations/")

SPRITE_ADDRESSES_CSV_PATH = Path("sprite_addresses.csv")

ROM_PATH = Path("../baserom.us.z64")

def check_asset_addresses_exit(label):
    with open("sprite_addresses.csv", newline="", encoding="utf-8") as f:
        reader = csv.reader(f)
        for idx, row in enumerate(reader, start=0):
            if len(row) == 5:
                if row[4] == label:
                    return idx
            # FIXME: skip type two sprite assets for now
            elif len(row) == 4:
                if row[3] == label:
                    return -1
            else:
                    continue

        return -1

# FIXME: break function up into subfunctions; put repeating code into a module
def write_textures(sprite_index: int, frame_count, bitmap_count, output_path, base_filename):

    result = check_asset_addresses_exit(output_path.parent.name)

    if result == -1:
        return

    with open(SPRITE_ADDRESSES_CSV_PATH) as f:
        reader = csv.reader(f)
        addresses_list = list(tuple(line) for line in reader)

    rom = memoryview(ROM_PATH.read_bytes())

    asset_addresses = addresses_list[result]

    spritesheet_base = int(asset_addresses[0], 16)
    asset_index_base = int(asset_addresses[1], 16)
    spritesheet_index_base = int(asset_addresses[2], 16)
    end_address = int(asset_addresses[3], 16)

    # get spritesheet lookup table array

    spritesheet_index_count = int((end_address - spritesheet_index_base) // 4)
    spritesheet_offsets_array = np.frombuffer(rom, dtype=np.dtype(">u4"), count=spritesheet_index_count, offset=spritesheet_index_base)

    # get general asset lookup table array

    asset_offsets_array = np.frombuffer(rom, dtype=np.dtype(">u4"), count=8, offset=asset_index_base)

    # get palette palette lookup array

    palette_index_base = int(spritesheet_base + asset_offsets_array[1])
    first_palette_offset = np.frombuffer(rom, dtype=np.dtype(">u4"), count=1, offset=palette_index_base).item()
    palette_count = int(first_palette_offset // 4)
    palette_offsets_array = np.frombuffer(rom, dtype=np.dtype(">u4"), count=palette_count, offset=palette_index_base)
    # get sprite to palette mapping array

    # skip header
    sprite_to_palette_index_base = int(spritesheet_base + asset_offsets_array[3]) + 4

    sprite_count = int((end_address - spritesheet_index_base) // 4)

    sprite_to_palette_array = np.frombuffer(rom, dtype=np.dtype("u1"), count=sprite_count, offset=sprite_to_palette_index_base)


    sprite_address = spritesheet_base + spritesheet_offsets_array[sprite_index]

    # skip header
    texture_flags = np.frombuffer(rom, dtype=np.dtype("u1"), count=1, offset=sprite_address + 3).item()

    if texture_flags == 16:
        type = 'ci4'
    else:
        type = 'ci8'

    width = np.frombuffer(rom, dtype=np.dtype("<i2"), count=1, offset=sprite_address + 4).item()
    height = np.frombuffer(rom, dtype=np.dtype("<i2"), count=1, offset=sprite_address + 6).item()

    texture_start = sprite_address + 8
    texture_end = spritesheet_base + spritesheet_offsets_array[sprite_index + 1]  

    if (texture_start > texture_end):
        print(f"Texture start address is greater than texture end address; likely a duplicate sprite")
        return

    texture_length = texture_end - texture_start

    texture = np.frombuffer(rom, dtype=np.dtype("u1"), count=texture_length, offset=texture_start)

    # get palette

    current_palette_index = sprite_to_palette_array[sprite_index]
    palette_start_address = palette_index_base + palette_offsets_array[current_palette_index]

    # start at header of next palette and subtract padding
    palette_end_address = palette_index_base + palette_offsets_array[current_palette_index+1]

    if palette_end_address != palette_start_address:
        palette_start_address += 4
        palette_end_address -= 4
    # some palette offsets are the same; occurs with dialogue icons
    else:
        j = 2
        while (palette_end_address == palette_start) and (current_palette_index+2 < len(sprite_to_palette_array)):
            palette_end_address = palette_index_base + palette_offsets_array[current_palette_index+j] - 4
            j+=1    
        if palette_end_address == palette_start:
            print('error getting palette length for sprite ' + sprite_name + ' ' + str(i))
            return
        else:
            palette_start += 4

    palette_length = int((palette_end_address - palette_start_address) // 2)

    palette = np.frombuffer(rom, dtype=np.dtype("<u2"), count=palette_length, offset=palette_start_address)


    filename = base_filename + f"-frame-{frame_count}" + f"-{bitmap_count}"

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

    if type == 'ci8':
        img = CI8(tex_data, width, height)
    else:
        img = CI4(tex_data, width, height)

    img.set_palette(pal_data)

    img.write(str(output_path) + '/%s.png' % filename)

    tex_in.close()
    pal_in.close()

    Path(str(output_path) + '/%s.ci' % filename).unlink()
    Path(str(output_path) + '/%s.pal' % filename).unlink()

def main():

    print("Fetching sprites for animations...")

    for path in JSON_DIR.rglob("*.json"):

        with path.open(encoding="utf-8") as f:
            data = json.load(f)

        frame_count = data["frame_count"]
        frames = data.get("frames", [])

        filename = path.stem

        for i, frame in enumerate(frames, start=1):

            for j, bitmap in enumerate(frame["bitmap_metadata"], start=1):
                write_textures(bitmap["spritesheet_index"], i, j, path.parent, filename)


if __name__ == "__main__":
    main()