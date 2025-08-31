import os
import csv
from n64img.image import CI4, CI8, Image
import numpy as np

path = '../assets/sprites/'
info_filename = 'sprite_info.txt'
sprite_addresses_csv = 'sprite_addresses.csv'
rom = '../baserom.us.z64'

os.system('mkdir -p ' + path)
os.system('rm -rf ' + path + '/*')

info = open(path + info_filename, 'w')
info.write('sprite information: \n')
info.close()

with open(sprite_addresses_csv) as f:
    reader = csv.reader(f)
    addresses_list = list(tuple(line) for line in reader)

f = open(rom, 'rb')

for asset_addresses in addresses_list:

    # set rom addresses from file
    # convert from hexidecimal to decimal

    # asset type 1: has separate spritesheet index
    if len(asset_addresses) == 5:

        sprite_base = int(asset_addresses[0], 16)
        spritesheet_index_base = int(asset_addresses[2], 16)
        
        end = int(asset_addresses[3], 16)
        
        sprite_name = asset_addresses[4]
        sprite_count = int((end - spritesheet_index_base) / 4)

    # asset type 2: spritesheet index is at start of spritesheet
    elif len(asset_addresses) == 4:

        spritesheet_index_base = int(asset_addresses[0], 16)
        
        f.seek(spritesheet_index_base)
        
        spritesheet_first_index = int(np.fromfile(f, dtype='>u4', count=1))
        sprite_base = spritesheet_index_base + spritesheet_first_index
        
        end = int(asset_addresses[2], 16)

        sprite_name = asset_addresses[3]
        sprite_count = int((sprite_base - spritesheet_index_base) / 4)

    else:
        print("invalid list in csv file")
        break

    asset_index_base = int(asset_addresses[1], 16)

    info = open(path + info_filename, 'a')
    info.write(sprite_name + ':\n')
    info.close()

    os.system('mkdir ' + path + '/' + sprite_name)

    # get spritesheet index

    f.seek(spritesheet_index_base)

    # copy the array of offsets to an array
    spritesheet_index = np.fromfile(f, dtype='>u4', count=sprite_count)

    # reduce sprite count if spritesheet_index has padding
    if len(spritesheet_index) > 2:
        while spritesheet_index[-1] == 0:
            spritesheet_index = spritesheet_index[:-1]
            sprite_count -= 1

    # last offset is end/palette index
    sprite_count -= 1

    # there are duplicate entries for some reason; maybe overlapping animations?
    # spritesheet_index = np.unique(spritesheet_index)

    # get asset_index array

    f.seek(asset_index_base)

    asset_index = np.fromfile(f, dtype='>u4', count=8)

    # asset_index[0] = spritesheet
    # asset_index[1] = palettes
    # asset_index[2] = animations
    # asset_index[3] = sprite to palette mapping 
    # asset_index[4] = spritesheet end

    # get address of palette index
    if len(asset_addresses) == 5:
        palette_index_base = sprite_base + asset_index[1]
        # could also be set this way
        # palette_index_base = sprite_base + spritesheet_index[-1]

    elif len(asset_addresses) == 4:
        palette_index_base = spritesheet_index_base + asset_index[1]
        # palette_index_base = sprite_base + spritesheet_index[-1]
    else:
        break

    f.seek(palette_index_base)

    first_palette_offset = int(np.fromfile(f, dtype='>u4', count=1))

    # just first_palette_offset / 4
    palette_count = int(((palette_index_base + first_palette_offset) - palette_index_base) / 4)

    f.seek(palette_index_base)

    palette_index = np.fromfile(f, dtype='>u4', count=palette_count)


    # get address of sprite-to-palette mappings

    if len(asset_addresses) == 5:
        sprite_to_palette_index_base = sprite_base + asset_index[3]
    elif len(asset_addresses) == 4:
        sprite_to_palette_index_base = spritesheet_index_base + asset_index[3]
    else:
        # shouldn't be reachable
        break

    # skip header
    f.seek(sprite_to_palette_index_base + 4)

    sprite_to_palette_index = np.fromfile(f, dtype=np.uint8, count=sprite_count)


    # set file pointer to start of spritesheet

    f.seek(sprite_base)

    # last index is offset to palette_index
    for i in range(len(spritesheet_index) - 1):

        # extra check; sometimes this is duplicated
        if ((sprite_base + spritesheet_index[i]) == palette_index_base) or (spritesheet_index_base + spritesheet_index[i] == palette_index_base):
            break

        # handle texture parsing
    
        # probably don't need to do anything with this
        # header = np.fromfile(f, dtype='<i2', count=1)

        # skip header
        f.seek(3, 1)

        flags = np.fromfile(f, dtype=np.uint8, count=1)

        # flags == 0x10 indicates ci4 format
        if flags[0] == 16:
            type = 'ci4'
        else:
            type = 'ci8'

        # width and height are little endian
        width = int(np.fromfile(f, dtype='<i2', count=1))
        height = int(np.fromfile(f, dtype='<i2', count=1))

        # sprite start + header + size info
        sprite_texture_start = f.tell()

        if len(asset_addresses) == 5:
            sprite_end = sprite_base + spritesheet_index[i+1]
        elif len(asset_addresses) == 4:
            sprite_end = spritesheet_index_base + spritesheet_index[i+1]
        else:
            # shouldn't be reachable
            break

        if (sprite_texture_start > sprite_end):
            info = open(path + info_filename, 'a')
            info.write(sprite_name + ': duplicate sprite found at index: ' + str(i) + '; address: ' + str(hex(sprite_texture_start)) + '\n')
            info.close()
            f.seek(sprite_end)
            continue

        sprite_length = sprite_end - sprite_texture_start

        texture = np.fromfile(f, dtype=np.uint8, count=(sprite_length))

        filename = sprite_name + '/texture-' + str(i+1)

        tex_file = open(path + '/%s.ci' % filename, 'wb')
        tex_file.write(texture)
        tex_file.close()


        # handle palette parsing

        current_palette_index = sprite_to_palette_index[i]

        # skip header
        palette_address = palette_index_base + palette_index[current_palette_index]

        # start at header of next palette and subtract padding
        palette_end_address = palette_index_base + palette_index[current_palette_index + 1]

        if palette_end_address != palette_address:
            palette_address += 4
            palette_end_address -= 4
        # some palette offsets are the same; occurs with dialogue icons
        else:
            j = 2
            while (palette_end_address == palette_address) and (current_palette_index+2 < len(sprite_to_palette_index)):
                palette_end_address = palette_index_base + palette_index[current_palette_index+j] - 4
                j+=1    
            if palette_end_address == palette_address:
                print('error getting palette length for sprite ' + sprite_name + ' ' + str(i))
                f.seek(sprite_end)
                continue
            else:
                palette_address += 4
                
        palette_length = int((palette_end_address - palette_address) / 2)

        f.seek(palette_address)

        palette = np.fromfile(f, dtype='<u2', count=palette_length)

        pal_file = open(path + '/%s.pal' % filename, 'wb')
        pal_file.write(palette)
        pal_file.close()


        # do conversion

        # read files as a binary blob
        tex_in = open(path + '/%s.ci' % filename, 'rb')
        tex_data = tex_in.read()
        
        pal_in = open(path + '/%s.pal' % filename, 'rb')
        pal_data = pal_in.read()

        if type == 'ci8':
            img = CI8(tex_data, width, height)
        else:
            img = CI4(tex_data, width, height)
            
        img.set_palette(pal_data)

        img.write(path + '/%s.png' % filename)

        tex_in.close()
        pal_in.close()

        # log info

        info = open(path + info_filename, 'a')
        info.write(sprite_name + ': sprite ' + str(i) + ' at ' + str(hex(sprite_texture_start)) + ' : format ' + type + ' : width : ' + str(int(width)) + ' height: ' + str(int(height)) + ': palette ' + str(current_palette_index) + ' at ' + str(hex(palette_address)) + '\n')
        info.close()

        # update file pointer for next iteration

        f.seek(sprite_end)
    
    info = open(path + info_filename, 'a')
    info.write('\n\n')
    info.close()

f.close()

