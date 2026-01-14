"""
Sprite Assembler for HM64 Build System.

Rebuilds sprite binary files from extracted/modified assets.
Used during the build process to convert edited sprites back to game format.

Input:  assets/sprites/<subdir>/<name>/manifest.json + textures/ + palettes/
Output: build/bin/sprites/<subdir>/<name>/*.bin

Binary format (type-1):
    Texture segment: textures + palettes + animations + S2P mapping
    AssetsIndex: 8 x u32 section offsets
    SpritesheetIndex: u32 offsets to each texture

Usage:
    python -m hm64.sprites.assembler assemble <input_dir> <output_dir>
    python -m hm64.sprites.assembler assemble_all <assets_dir> <build_dir>
"""

import argparse
import json
import struct
import sys
from pathlib import Path
from typing import Dict, List, Tuple

from PIL import Image

from ..common import colors


def pack_u32_be(val: int) -> bytes:
    """Pack unsigned 32-bit big-endian."""
    return struct.pack(">I", val)


def pack_u16_be(val: int) -> bytes:
    """Pack unsigned 16-bit big-endian."""
    return struct.pack(">H", val)


def pack_s16_le(val: int) -> bytes:
    """Pack signed 16-bit little-endian."""
    return struct.pack("<h", val)


def pack_u16_le(val: int) -> bytes:
    """Pack unsigned 16-bit little-endian."""
    return struct.pack("<H", val)


def align_to(data: bytearray, alignment: int, pad_byte: int = 0) -> None:
    """Pad data to alignment boundary in-place."""
    remainder = len(data) % alignment
    if remainder != 0:
        data.extend([pad_byte] * (alignment - remainder))


def load_texture_from_ci(ci_path: Path, tex_info: dict) -> Tuple[bytes, int, int, str]:
    """Load raw CI texture data with metadata from manifest."""
    with open(ci_path, 'rb') as f:
        pixel_data = f.read()
    return pixel_data, tex_info['width'], tex_info['height'], tex_info['format']


def load_texture_from_png(png_path: Path) -> Tuple[bytes, int, int, str]:
    """Load indexed PNG and extract CI data."""
    img = Image.open(png_path)

    if img.mode != 'P':
        raise ValueError(f"PNG must be indexed (mode P), got {img.mode}")

    width, height = img.size
    indices = list(img.getdata())

    max_idx = max(indices) if indices else 0
    tex_format = 'ci4' if max_idx < 16 else 'ci8'

    if tex_format == 'ci4':
        packed = bytearray()
        for i in range(0, len(indices), 2):
            hi = indices[i] & 0x0F
            lo = indices[i + 1] & 0x0F if i + 1 < len(indices) else 0
            packed.append((hi << 4) | lo)
        pixel_data = bytes(packed)
    else:
        pixel_data = bytes(indices)

    return pixel_data, width, height, tex_format


def load_palette_from_raw(pal_path: Path, pal_info: dict) -> Tuple[bytes, str]:
    """Load raw palette file directly."""
    with open(pal_path, 'rb') as f:
        pal_bytes = f.read()
    pal_format = pal_info.get('format', 'ci8')
    return pal_bytes, pal_format


def load_palette_from_json(json_path: Path) -> Tuple[bytes, str]:
    """Load palette from JSON and convert to RGBA5551 bytes."""
    with open(json_path) as f:
        data = json.load(f)

    result = bytearray()
    for color in data['colors']:
        rgba = color['rgba']
        val = colors.rgba_to_rgba5551(rgba[0], rgba[1], rgba[2], rgba[3])
        result.append((val >> 8) & 0xFF)
        result.append(val & 0xFF)

    pal_format = data.get('format', 'ci8')
    return bytes(result), pal_format


def load_animation_from_json(json_path: Path) -> bytes:
    """Load animation from JSON and convert to binary format."""
    with open(json_path) as f:
        data = json.load(f)

    frames = data.get('frames', [])
    if not frames:
        return b''

    result = bytearray()

    # Header: "46d" signature + 0x00, frame_count (u16 LE), 2 unused bytes
    result.extend(b'\x34\x36\x64\x00')
    result.extend(pack_u16_le(len(frames)))
    result.extend(b'\x00\x00')

    for frame in frames:
        sprites = frame.get('sprites', [])
        result.extend(pack_u16_le(len(sprites)))
        result.append(frame.get('duration', 1))
        result.append(frame.get('has_audio', 0))

        for sprite in sprites:
            result.extend(pack_u16_le(sprite['spritesheet_index']))
            result.extend(pack_u16_le(0))  # Unused
            result.extend(pack_s16_le(sprite['anchor_x']))
            result.extend(pack_s16_le(sprite['anchor_y']))

    return bytes(result)


def build_palette_section(manifest: Dict, sprite_dir: Path) -> Tuple[bytearray, bytearray]:
    """Build palette index and data from manifest and palette files."""
    palettes_info = manifest.get('palettes', {})
    sprite_to_palette = manifest.get('sprite_to_palette', [])
    original_pal_index = manifest.get('palette_index', None)

    if original_pal_index:
        palette_index_count = manifest.get('palette_index_count', len(original_pal_index))
        palette_index_size = palette_index_count * 4
        palette_data = bytearray()
        written_offsets = {}

        for pal_idx in range(len(original_pal_index) - 1):
            orig_offset = original_pal_index[pal_idx]
            next_offset = original_pal_index[pal_idx + 1]

            if next_offset == 0 or orig_offset == next_offset:
                written_offsets[orig_offset] = len(palette_data) + palette_index_size
                continue

            pal_path = sprite_dir / 'palettes' / f'{pal_idx:02d}.pal'
            json_path = sprite_dir / 'palettes' / f'{pal_idx:02d}.json'
            pal_info = palettes_info.get(str(pal_idx), palettes_info.get(pal_idx, {}))

            if pal_path.exists():
                new_offset = len(palette_data) + palette_index_size
                written_offsets[orig_offset] = new_offset
                pal_bytes, pal_format = load_palette_from_raw(pal_path, pal_info)
            elif json_path.exists():
                new_offset = len(palette_data) + palette_index_size
                written_offsets[orig_offset] = new_offset
                pal_bytes, pal_format = load_palette_from_json(json_path)
            else:
                written_offsets[orig_offset] = len(palette_data) + palette_index_size
                continue

            flag = 0x10 if pal_format == 'ci4' else 0x00
            palette_data.extend(b'\x34\x36\x64')
            palette_data.append(flag)
            palette_data.extend(pal_bytes)
            palette_data.extend(b'\xff\xff\xff\xff')

        end_offset = len(palette_data) + palette_index_size
        last_orig_offset = original_pal_index[-1]
        if last_orig_offset != 0:
            written_offsets[last_orig_offset] = end_offset

        palette_index = bytearray()
        palette_index_end_bytes = manifest.get('palette_index_end_bytes', [])

        for i in range(palette_index_count):
            if i < len(original_pal_index):
                orig_offset = original_pal_index[i]
                if orig_offset == 0:
                    palette_index.extend(pack_u32_be(0))
                else:
                    new_offset = written_offsets.get(orig_offset, end_offset)
                    palette_index.extend(pack_u32_be(new_offset))
            elif i == palette_index_count - 1 and palette_index_end_bytes:
                palette_index.extend(bytes(palette_index_end_bytes))
            else:
                palette_index.extend(pack_u32_be(0))
    else:
        palette_data = bytearray()
        palette_offsets = []

        unique_palettes = sorted(set(sprite_to_palette)) if sprite_to_palette else []
        palette_count = max(unique_palettes) + 1 if unique_palettes else 0

        for pal_idx in range(palette_count):
            palette_offsets.append(len(palette_data))

            pal_path = sprite_dir / 'palettes' / f'{pal_idx:02d}.pal'
            json_path = sprite_dir / 'palettes' / f'{pal_idx:02d}.json'
            pal_info = palettes_info.get(str(pal_idx), palettes_info.get(pal_idx, {}))

            if pal_path.exists():
                pal_bytes, pal_format = load_palette_from_raw(pal_path, pal_info)
            elif json_path.exists():
                pal_bytes, pal_format = load_palette_from_json(json_path)
            else:
                continue

            flag = 0x10 if pal_format == 'ci4' else 0x00
            palette_data.extend(b'\x34\x36\x64')
            palette_data.append(flag)
            palette_data.extend(pal_bytes)
            palette_data.extend(b'\xff\xff\xff\xff')

        palette_offsets.append(len(palette_data))

        palette_index_size = len(palette_offsets) * 4
        palette_index = bytearray()
        for offset in palette_offsets:
            palette_index.extend(pack_u32_be(offset + palette_index_size))

    return palette_index, palette_data


def build_animation_section(manifest: Dict, sprite_dir: Path) -> Tuple[bytearray, bytearray]:
    """Build animation index and data from manifest and animation files."""
    original_anim_index = manifest.get('animation_index', None)
    anim_dir = sprite_dir / 'animations'

    if original_anim_index:
        animation_index_size = len(original_anim_index) * 4
        animation_data = bytearray()
        written_offsets = {}

        for idx, orig_offset in enumerate(original_anim_index[:-1]):
            if orig_offset in written_offsets:
                continue

            anim_file = anim_dir / f'{idx:02d}.json'
            if anim_file.exists():
                new_offset = len(animation_data) + animation_index_size
                written_offsets[orig_offset] = new_offset
                anim_bytes = load_animation_from_json(anim_file)
                animation_data.extend(anim_bytes)

        end_offset = len(animation_data) + animation_index_size
        written_offsets[original_anim_index[-1]] = end_offset

        animation_index = bytearray()
        for orig_offset in original_anim_index:
            new_offset = written_offsets.get(orig_offset, end_offset)
            animation_index.extend(pack_u32_be(new_offset))
    else:
        animation_data = bytearray()
        animation_offsets = []

        anim_files = sorted(anim_dir.glob('*.json')) if anim_dir.exists() else []

        for anim_file in anim_files:
            animation_offsets.append(len(animation_data))
            anim_bytes = load_animation_from_json(anim_file)
            animation_data.extend(anim_bytes)

        animation_offsets.append(len(animation_data))

        animation_index_size = len(animation_offsets) * 4
        animation_index = bytearray()
        for offset in animation_offsets:
            animation_index.extend(pack_u32_be(offset + animation_index_size))

    return animation_index, animation_data


def build_s2p_section(manifest: Dict) -> bytearray:
    """Build sprite-to-palette mapping section."""
    sprite_to_palette = manifest.get('sprite_to_palette', [])

    s2p_data = bytearray()
    s2p_data.extend(b'\x34\x36\x64\x00')  # "46d" signature + padding
    s2p_data.extend(bytes(sprite_to_palette))

    s2p_padding = manifest.get('s2p_padding', None)
    if s2p_padding:
        s2p_data.extend(bytes(s2p_padding))
    else:
        asset_offsets_orig = manifest.get('asset_offsets', [])
        if len(asset_offsets_orig) >= 5 and asset_offsets_orig[3] > 0:
            original_s2p_size = asset_offsets_orig[4] - asset_offsets_orig[3]
            while len(s2p_data) < original_s2p_size:
                s2p_data.append(0xFF)
        else:
            align_to(s2p_data, 4, pad_byte=0xFF)

    return s2p_data


def build_assets_index(spritesheet_start: int, palette_start: int,
                       animation_start: int, s2p_start: int,
                       assets_index_start: int) -> bytearray:
    """Build the 8 u32 assets index table."""
    assets_index = bytearray()
    assets_index.extend(pack_u32_be(spritesheet_start))
    assets_index.extend(pack_u32_be(palette_start))
    assets_index.extend(pack_u32_be(animation_start))
    assets_index.extend(pack_u32_be(s2p_start))
    assets_index.extend(pack_u32_be(assets_index_start))
    assets_index.extend(pack_u32_be(0))
    assets_index.extend(pack_u32_be(0))
    assets_index.extend(pack_u32_be(0))
    return assets_index


def build_sprite_binary_type1(manifest: Dict, sprite_dir: Path) -> Dict[str, bytes]:
    """Build type-1 sprite binary from manifest and assets."""
    textures_info = manifest['textures']
    sprite_count = manifest['sprite_count']
    label = manifest.get('label', '')
    spritesheet_index_count = manifest.get('spritesheet_index_count', sprite_count + 1)

    # Build spritesheet section
    spritesheet_data = bytearray()
    texture_offsets = []
    texture_map = {t['index']: t for t in textures_info}

    for sprite_idx in range(sprite_count):
        texture_offsets.append(len(spritesheet_data))

        if sprite_idx not in texture_map:
            continue

        tex_info = texture_map[sprite_idx]
        ci_path = sprite_dir / 'textures' / f'{sprite_idx:03d}.ci'
        png_path = sprite_dir / 'textures' / f'{sprite_idx:03d}.png'

        if ci_path.exists():
            pixel_data, width, height, tex_format = load_texture_from_ci(ci_path, tex_info)
        elif png_path.exists():
            pixel_data, width, height, tex_format = load_texture_from_png(png_path)
        else:
            continue

        header = bytearray(8)
        header[0:3] = b'\x34\x36\x64'
        header[3] = 0x10 if tex_format == 'ci4' else 0x00
        header[4:6] = pack_s16_le(width)
        header[6:8] = pack_s16_le(height)

        spritesheet_data.extend(header)
        spritesheet_data.extend(pixel_data)

    texture_offsets.append(len(spritesheet_data))

    # Build sections
    palette_index, palette_data = build_palette_section(manifest, sprite_dir)
    animation_index, animation_data = build_animation_section(manifest, sprite_dir)
    s2p_data = build_s2p_section(manifest)

    # Calculate offsets
    embedded_index_size = spritesheet_index_count * 4 if label == 'festivalFlowers' else 0
    current_offset = embedded_index_size

    spritesheet_start = 0 if label == 'festivalFlowers' else current_offset
    current_offset += len(spritesheet_data)

    palette_section_start = current_offset
    palette_section = palette_index + palette_data
    current_offset += len(palette_section)

    animation_section_start = current_offset
    animation_section = bytearray(animation_index + animation_data)

    animation_section_end = animation_section_start + len(animation_section)
    if animation_section_end % 8 == 4:
        animation_section.extend(b'\xff\xff\xff\xff')

    current_offset += len(animation_section)

    s2p_start = current_offset
    current_offset += len(s2p_data)

    assets_index_start = current_offset
    assets_index = build_assets_index(spritesheet_start, palette_section_start,
                                      animation_section_start, s2p_start,
                                      assets_index_start)

    # Build spritesheet index
    spritesheet_index = bytearray()
    spritesheet_index_end_bytes = manifest.get('spritesheet_index_end_bytes', [])

    for i in range(spritesheet_index_count):
        if i == spritesheet_index_count - 1 and spritesheet_index_end_bytes:
            spritesheet_index.extend(bytes(spritesheet_index_end_bytes))
        elif i < len(texture_offsets):
            spritesheet_index.extend(pack_u32_be(texture_offsets[i]))
        else:
            spritesheet_index.extend(pack_u32_be(0))

    while len(spritesheet_index) % 16 != 0:
        spritesheet_index.extend(pack_u32_be(0))

    # Assemble texture binary
    texture_binary = bytearray()

    if label == 'festivalFlowers':
        embedded_index = bytearray()
        for i in range(spritesheet_index_count):
            if i < len(texture_offsets):
                embedded_index.extend(pack_u32_be(texture_offsets[i] + embedded_index_size))
            else:
                embedded_index.extend(pack_u32_be(0))
        texture_binary.extend(embedded_index)

    texture_binary.extend(spritesheet_data)
    texture_binary.extend(palette_section)
    texture_binary.extend(animation_section)
    texture_binary.extend(s2p_data)

    texture_end_padding = manifest.get('texture_end_padding', [])
    if texture_end_padding:
        texture_binary.extend(bytes(texture_end_padding))

    return {
        'texture': bytes(texture_binary),
        'assets_index': bytes(assets_index),
        'spritesheet_index': bytes(spritesheet_index),
    }


def build_sprite_binary_type2(manifest: Dict, sprite_dir: Path) -> Dict[str, bytes]:
    """Build type-2 sprite binary from manifest and assets."""
    textures_info = manifest['textures']
    sprite_count = manifest['sprite_count']

    texture_data_list = []
    texture_map = {t['index']: t for t in textures_info}

    for sprite_idx in range(sprite_count):
        if sprite_idx not in texture_map:
            texture_data_list.append(b'')
            continue

        tex_info = texture_map[sprite_idx]
        ci_path = sprite_dir / 'textures' / f'{sprite_idx:03d}.ci'
        png_path = sprite_dir / 'textures' / f'{sprite_idx:03d}.png'

        if ci_path.exists():
            pixel_data, width, height, tex_format = load_texture_from_ci(ci_path, tex_info)
        elif png_path.exists():
            pixel_data, width, height, tex_format = load_texture_from_png(png_path)
        else:
            texture_data_list.append(b'')
            continue

        header = bytearray(8)
        header[0:3] = b'\x34\x36\x64'
        header[3] = 0x10 if tex_format == 'ci4' else 0x00
        header[4:6] = pack_s16_le(width)
        header[6:8] = pack_s16_le(height)

        texture_data_list.append(bytes(header) + pixel_data)

    spritesheet_index_count = manifest.get('spritesheet_index_count', sprite_count + 1)
    index_table_size = spritesheet_index_count * 4
    current_offset = index_table_size

    texture_offsets = []
    for tex_data in texture_data_list:
        texture_offsets.append(current_offset)
        current_offset += len(tex_data)
    texture_offsets.append(current_offset)

    spritesheet_data = bytearray()
    spritesheet_index_end_bytes = manifest.get('spritesheet_index_end_bytes', [])

    for i in range(spritesheet_index_count):
        if i == spritesheet_index_count - 1 and spritesheet_index_end_bytes:
            spritesheet_data.extend(bytes(spritesheet_index_end_bytes))
        elif i < len(texture_offsets):
            spritesheet_data.extend(pack_u32_be(texture_offsets[i]))
        else:
            spritesheet_data.extend(pack_u32_be(0))

    for tex_data in texture_data_list:
        spritesheet_data.extend(tex_data)

    palette_index, palette_data = build_palette_section(manifest, sprite_dir)
    animation_index, animation_data = build_animation_section(manifest, sprite_dir)
    s2p_data = build_s2p_section(manifest)

    current_offset = 0
    spritesheet_start = current_offset
    current_offset += len(spritesheet_data)

    palette_section_start = current_offset
    palette_section = palette_index + palette_data
    current_offset += len(palette_section)

    animation_section_start = current_offset
    animation_section = bytearray(animation_index + animation_data)

    animation_section_end = animation_section_start + len(animation_section)
    if animation_section_end % 8 == 4:
        animation_section.extend(b'\xff\xff\xff\xff')

    current_offset += len(animation_section)

    s2p_start = current_offset
    current_offset += len(s2p_data)

    assets_index_start = current_offset
    assets_index = build_assets_index(spritesheet_start, palette_section_start,
                                      animation_section_start, s2p_start,
                                      assets_index_start)

    texture_binary = bytearray()
    texture_binary.extend(spritesheet_data)
    texture_binary.extend(palette_section)
    texture_binary.extend(animation_section)
    texture_binary.extend(s2p_data)

    texture_end_padding = manifest.get('texture_end_padding', [])
    if texture_end_padding:
        texture_binary.extend(bytes(texture_end_padding))

    return {
        'texture': bytes(texture_binary),
        'assets_index': bytes(assets_index),
    }


def assemble_sprite(input_dir: Path, output_dir: Path) -> bool:
    """Assemble sprite binaries from extracted assets."""
    manifest_path = input_dir / 'manifest.json'

    if not manifest_path.exists():
        print(f"Error: manifest.json not found in {input_dir}")
        return False

    with open(manifest_path) as f:
        manifest = json.load(f)

    sprite_type = manifest.get('type', 'type-1')
    label = manifest.get('label', input_dir.name)

    print(f"Assembling: {label} ({sprite_type})")

    try:
        if sprite_type == 'type-1':
            segments = build_sprite_binary_type1(manifest, input_dir)
        else:
            segments = build_sprite_binary_type2(manifest, input_dir)

        output_dir.mkdir(parents=True, exist_ok=True)

        total_bytes = 0
        for segment_name, data in segments.items():
            parts = segment_name.split('_')
            pascal_name = ''.join(p.capitalize() for p in parts)

            output_file = output_dir / f"{label}{pascal_name}.bin"
            with open(output_file, 'wb') as f:
                f.write(data)

            print(f"  {output_file.name}: {len(data)} bytes")
            total_bytes += len(data)

        print(f"  Total: {total_bytes} bytes in {len(segments)} files")
        return True

    except Exception as e:
        print(f"  Error: {e}")
        import traceback
        traceback.print_exc()
        return False


def assemble_all_sprites(assets_dir: Path, build_dir: Path) -> bool:
    """Assemble all sprites found in assets directory."""
    manifests = list(assets_dir.rglob('manifest.json'))

    if not manifests:
        print(f"No sprites found in {assets_dir}")
        return True

    print(f"Found {len(manifests)} sprites to assemble")

    success_count = 0
    fail_count = 0

    for manifest_path in sorted(manifests):
        sprite_dir = manifest_path.parent
        rel_path = sprite_dir.relative_to(assets_dir)
        output_dir = build_dir / rel_path

        if assemble_sprite(sprite_dir, output_dir):
            success_count += 1
        else:
            fail_count += 1

    print(f"\nAssembled {success_count} sprites, {fail_count} failures")
    return fail_count == 0


def main():
    parser = argparse.ArgumentParser(description="Assemble sprite binaries from assets")
    subparsers = parser.add_subparsers(dest='command')

    single_parser = subparsers.add_parser('assemble', help='Assemble a single sprite')
    single_parser.add_argument('input_dir', type=Path, help='Input sprite directory')
    single_parser.add_argument('output_dir', type=Path, help='Output directory')

    all_parser = subparsers.add_parser('assemble_all', help='Assemble all sprites')
    all_parser.add_argument('assets_dir', type=Path, help='Root assets/sprites directory')
    all_parser.add_argument('build_dir', type=Path, help='Root build directory')

    args = parser.parse_args()

    if args.command == 'assemble_all':
        success = assemble_all_sprites(args.assets_dir, args.build_dir)
    elif args.command == 'assemble':
        success = assemble_sprite(args.input_dir, args.output_dir)
    else:
        if len(sys.argv) == 3:
            success = assemble_sprite(Path(sys.argv[1]), Path(sys.argv[2]))
        else:
            parser.print_help()
            sys.exit(1)

    sys.exit(0 if success else 1)


if __name__ == '__main__':
    main()
