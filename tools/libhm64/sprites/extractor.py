"""
Sprite Extractor for HM64 Build System.

Extracts sprite assets from ROM into an editable format that can be
reassembled during the build process.

Extracted format:
    assets/sprites/<subdir>/<name>/
        manifest.json       # Master manifest with all metadata
        textures/
            000.ci          # Raw CI texture data
            000.png         # Viewable PNG (for development)
            ...
        palettes/
            00.json         # Editable palette with colors
            00.pal          # Raw RGBA5551 binary
            ...
        animations/
            00.json         # Animation metadata
            ...

Usage:
    python -m hm64.sprites.extractor extract_all
    python -m hm64.sprites.extractor extract player
"""

import argparse
import json
from pathlib import Path
from typing import Dict, List, Optional

from PIL import Image

from ..common import rom, colors
from ..animations.metadata import read_animation_from_offsets
from . import addresses


# Default output directory
_REPO_DIR = Path(__file__).resolve().parent.parent.parent.parent
DEFAULT_OUTPUT_DIR = _REPO_DIR / "assets" / "sprites"


def palette_to_json(pal_data: bytes) -> Dict:
    """Convert palette bytes to JSON format."""
    color_list = []
    for i in range(0, len(pal_data), 2):
        if i + 1 < len(pal_data):
            val = (pal_data[i] << 8) | pal_data[i + 1]
            r, g, b, a = colors.rgba5551_to_rgba(val)
            color_list.append({
                'index': i // 2,
                'hex': f'#{r:02X}{g:02X}{b:02X}',
                'alpha': a,
                'rgba': [r, g, b, a]
            })

    return {
        'color_count': len(color_list),
        'format': 'ci4' if len(color_list) <= 16 else 'ci8',
        'colors': color_list
    }


def create_viewable_png(tex_data: bytes, width: int, height: int,
                        pal_data: bytes, tex_format: str) -> Image.Image:
    """Create RGBA PNG from texture and palette data."""
    # Unpack pixel indices
    if tex_format == 'ci4':
        indices = []
        for byte in tex_data:
            indices.append((byte >> 4) & 0x0F)
            indices.append(byte & 0x0F)
        indices = indices[:width * height]
    else:
        indices = list(tex_data[:width * height])

    # Pad if needed
    while len(indices) < width * height:
        indices.append(0)

    # Parse palette with alpha
    palette = []
    for i in range(0, len(pal_data), 2):
        if i + 1 < len(pal_data):
            val = (pal_data[i] << 8) | pal_data[i + 1]
            palette.append(colors.rgba5551_to_rgba(val))

    # Pad palette if needed
    while len(palette) < 256:
        palette.append((0, 0, 0, 0))

    # Create RGBA image
    img = Image.new('RGBA', (width, height))
    pixels = [palette[idx] if idx < len(palette) else (0, 0, 0, 0) for idx in indices]
    img.putdata(pixels)

    return img


def extract_texture_data(addr_base: int, spritesheet_offsets: List[int],
                         sprite_idx: int) -> tuple:
    """Extract texture data, dimensions, and format for a sprite."""
    if sprite_idx >= len(spritesheet_offsets) - 1:
        return b'', 0, 0, 'ci8'

    offset = spritesheet_offsets[sprite_idx]
    next_offset = spritesheet_offsets[sprite_idx + 1]

    if offset == next_offset:
        return b'', 0, 0, 'ci8'

    sprite_addr = addr_base + offset

    flags = rom.read_u8(sprite_addr + 3)
    width = rom.read_s16_le(sprite_addr + 4)
    height = rom.read_s16_le(sprite_addr + 6)

    tex_format = 'ci4' if flags == 0x10 else 'ci8'

    data_start = sprite_addr + 8
    data_end = addr_base + next_offset

    if data_end <= data_start:
        return b'', 0, 0, tex_format

    tex_data = rom.read_bytes(data_start, data_end)

    return tex_data, width, height, tex_format


def extract_sprite_simple(info: addresses.SpriteInfo, out_dir: Path,
                          spritesheet_offsets: List[int], sprite_count: int) -> bool:
    """Extract sprite with minimal metadata (for type-2 or simple sprites)."""
    addr_base = info.addr_base

    texture_info = []
    for sprite_idx in range(sprite_count):
        tex_data, width, height, tex_format = extract_texture_data(
            addr_base, spritesheet_offsets, sprite_idx)

        if len(tex_data) == 0 or width <= 0 or height <= 0:
            continue

        # Save raw CI file
        with open(out_dir / 'textures' / f'{sprite_idx:03d}.ci', 'wb') as f:
            f.write(tex_data)

        texture_info.append({
            'index': sprite_idx,
            'width': width,
            'height': height,
            'format': tex_format
        })

    # Build minimal manifest
    manifest = {
        'label': info.label,
        'subdir': info.subdir,
        'type': info.sprite_type,
        'sprite_count': sprite_count,
        'palette_count': 0,
        'animation_count': 0,
        'textures': texture_info,
        'palettes': {},
        'sprite_to_palette': [],
        'simple_format': True
    }

    with open(out_dir / 'manifest.json', 'w') as f:
        json.dump(manifest, f, indent=2)

    print(f"    Extracted: {len(texture_info)} textures (raw CI format)")
    return True


def extract_sprite(info: addresses.SpriteInfo, output_base: Path) -> bool:
    """Extract a single sprite to the output directory."""
    label = info.label
    subdir = info.subdir
    sprite_type = info.sprite_type
    addr_base = info.addr_base

    print(f"  Extracting: {label} ({sprite_type})")

    # Create output directory
    out_dir = output_base / subdir / label
    out_dir.mkdir(parents=True, exist_ok=True)
    (out_dir / 'textures').mkdir(exist_ok=True)
    (out_dir / 'palettes').mkdir(exist_ok=True)
    (out_dir / 'animations').mkdir(exist_ok=True)

    try:
        # Get offset tables
        spritesheet_offsets = addresses.get_spritesheet_offsets(info)
        asset_offsets = addresses.get_asset_offsets(info.addr_index)

        # Calculate sprite count
        spritesheet_index_count = len(spritesheet_offsets)
        sprite_count = spritesheet_index_count - 1
        while sprite_count > 0 and spritesheet_offsets[sprite_count] == 0:
            sprite_count -= 1

        # Capture raw bytes of last spritesheet index entry
        if info.is_type1:
            last_entry_addr = info.spritesheet_index_base + (spritesheet_index_count - 1) * 4
        else:
            last_entry_addr = addr_base + (spritesheet_index_count - 1) * 4
        rom_data = rom.get_rom()
        spritesheet_index_end_bytes = list(rom_data[last_entry_addr:last_entry_addr + 4])

        if sprite_count <= 0:
            print(f"    Warning: No sprites found")
            return False

        # Check for valid palette section
        if asset_offsets[1] == 0:
            print(f"    Note: No palette section found, extracting raw textures only")
            return extract_sprite_simple(info, out_dir, spritesheet_offsets, sprite_count)

        # Get palette info
        first_pal_offset = rom.read_u32_be(addr_base + asset_offsets[1])

        if first_pal_offset == 0 or first_pal_offset > 0x1000 or first_pal_offset % 4 != 0:
            print(f"    Note: Invalid palette section, extracting raw textures only")
            return extract_sprite_simple(info, out_dir, spritesheet_offsets, sprite_count)

        pal_count = first_pal_offset // 4
        palette_index_count = pal_count

        if pal_count > 256:
            print(f"    Note: Unreasonable palette count ({pal_count}), extracting raw textures only")
            return extract_sprite_simple(info, out_dir, spritesheet_offsets, sprite_count)

        # Read palette offsets
        palette_offsets_raw = []
        for i in range(pal_count):
            offset = rom.read_u32_be(addr_base + asset_offsets[1] + i * 4)
            if offset > 0x100000:
                break
            palette_offsets_raw.append(offset)

        # Capture raw bytes of last palette index entry
        palette_section = addr_base + asset_offsets[1]
        last_pal_entry_addr = palette_section + (pal_count - 1) * 4
        palette_index_end_bytes = list(rom_data[last_pal_entry_addr:last_pal_entry_addr + 4])

        # Derive actual sprite count from S2P section size
        s2p_section_size = asset_offsets[4] - asset_offsets[3]
        if s2p_section_size > 4:
            actual_sprite_count = s2p_section_size - 4
            if actual_sprite_count < sprite_count:
                sprite_count = actual_sprite_count

        # Get sprite-to-palette mapping
        sprite_to_palette = addresses.get_sprite_to_palette_map(addr_base, asset_offsets, sprite_count)

        # Get S2P section padding
        s2p_section_start = addr_base + asset_offsets[3]
        s2p_data_size = 4 + sprite_count
        s2p_padding_size = s2p_section_size - s2p_data_size
        if s2p_padding_size > 0:
            s2p_padding = list(rom_data[s2p_section_start + s2p_data_size:s2p_section_start + s2p_section_size])
        else:
            s2p_padding = []

        # Get animation offsets
        anim_offsets = addresses.get_animation_offsets(addr_base, asset_offsets)

        # Extract palettes
        palette_info = {}
        extracted_palettes = set()
        for pal_idx in range(len(palette_offsets_raw) - 1):
            offset = palette_offsets_raw[pal_idx]
            next_offset = palette_offsets_raw[pal_idx + 1]

            if next_offset == 0 or offset == next_offset:
                continue

            pal_base = addr_base + asset_offsets[1]
            start = pal_base + offset + 4
            end = pal_base + next_offset - 4

            if end <= start:
                continue

            pal_size = end - start
            if pal_size > 512:
                continue

            pal_data = rom.read_bytes(start, end)

            if len(pal_data) == 0:
                continue

            # Save raw palette
            with open(out_dir / 'palettes' / f'{pal_idx:02d}.pal', 'wb') as f:
                f.write(pal_data)

            # Save JSON palette
            pal_json = palette_to_json(pal_data)
            pal_json['palette_index'] = pal_idx
            with open(out_dir / 'palettes' / f'{pal_idx:02d}.json', 'w') as f:
                json.dump(pal_json, f, indent=2)

            palette_info[pal_idx] = {
                'color_count': pal_json['color_count'],
                'format': pal_json['format']
            }
            extracted_palettes.add(pal_idx)

        # Extract textures
        texture_info = []
        for sprite_idx in range(sprite_count):
            tex_data, width, height, tex_format = extract_texture_data(
                addr_base, spritesheet_offsets, sprite_idx)

            if len(tex_data) == 0 or width <= 0 or height <= 0:
                texture_info.append(None)
                continue

            pal_idx = sprite_to_palette[sprite_idx] if sprite_idx < len(sprite_to_palette) else 0

            # Save raw CI file
            ci_file = out_dir / 'textures' / f'{sprite_idx:03d}.ci'
            with open(ci_file, 'wb') as f:
                f.write(tex_data)

            # Save viewable PNG
            pal_file = out_dir / 'palettes' / f'{pal_idx:02d}.pal'
            if pal_file.exists():
                pal_data = pal_file.read_bytes()
                png = create_viewable_png(tex_data, width, height, pal_data, tex_format)
                png.save(out_dir / 'textures' / f'{sprite_idx:03d}.png')

            texture_info.append({
                'index': sprite_idx,
                'width': width,
                'height': height,
                'format': tex_format,
                'palette_index': pal_idx
            })

        # Extract animations
        animations = []
        for anim_idx in range(len(anim_offsets) - 1):
            anim = read_animation_from_offsets(addr_base + asset_offsets[2], anim_offsets, anim_idx)
            if anim:
                anim['index'] = anim_idx
                animations.append(anim)

                with open(out_dir / 'animations' / f'{anim_idx:02d}.json', 'w') as f:
                    json.dump(anim, f, indent=2)

        # Calculate texture end padding
        texture_segment_size = info.addr_index - addr_base
        texture_end_offset = asset_offsets[4]
        texture_end_padding_size = texture_segment_size - texture_end_offset
        if texture_end_padding_size > 0:
            texture_end_padding = list(rom_data[addr_base + texture_end_offset:addr_base + texture_segment_size])
        else:
            texture_end_padding = []

        # Build manifest
        manifest = {
            'label': label,
            'subdir': subdir,
            'type': sprite_type,
            'sprite_count': sprite_count,
            'spritesheet_index_count': spritesheet_index_count,
            'palette_count': len(extracted_palettes),
            'palette_index_count': palette_index_count,
            'animation_count': len(animations),
            'animation_index': anim_offsets,
            'palette_index': palette_offsets_raw,
            'palette_index_end_bytes': palette_index_end_bytes,
            's2p_padding': s2p_padding,
            'texture_end_padding': texture_end_padding,
            'spritesheet_index_end_bytes': spritesheet_index_end_bytes,
            'textures': [t for t in texture_info if t is not None],
            'palettes': palette_info,
            'sprite_to_palette': sprite_to_palette,
            'asset_offsets': asset_offsets
        }

        with open(out_dir / 'manifest.json', 'w') as f:
            json.dump(manifest, f, indent=2)

        valid_textures = len([t for t in texture_info if t is not None])
        print(f"    Extracted: {valid_textures} textures, {len(extracted_palettes)} palettes, {len(animations)} animations")
        return True

    except Exception as e:
        print(f"    Error: {e}")
        import traceback
        traceback.print_exc()
        return False


def extract_all(output_dir: Path = DEFAULT_OUTPUT_DIR):
    """Extract all sprites."""
    sprites = addresses.get_all_sprites()
    print(f"Found {len(sprites)} sprites to extract")

    success = 0
    for info in sprites:
        if extract_sprite(info, output_dir):
            success += 1

    print(f"\nExtracted {success}/{len(sprites)} sprites")


def extract_one(label: str, output_dir: Path = DEFAULT_OUTPUT_DIR):
    """Extract a single sprite by label."""
    info = addresses.get_sprite_by_label(label)

    if info is None:
        print(f"Sprite not found: {label}")
        return

    extract_sprite(info, output_dir)


def main():
    parser = argparse.ArgumentParser(description="Extract sprite assets for HM64")
    parser.add_argument('command', choices=['extract_all', 'extract'],
                        help='Command to run')
    parser.add_argument('label', nargs='?', help='Sprite label (for extract command)')
    parser.add_argument('--output', '-o', type=str, default=str(DEFAULT_OUTPUT_DIR),
                        help='Output directory')

    args = parser.parse_args()
    output_dir = Path(args.output)

    if args.command == 'extract_all':
        extract_all(output_dir)
    elif args.command == 'extract':
        if not args.label:
            print("Error: extract requires a sprite label")
        else:
            extract_one(args.label, output_dir)


if __name__ == '__main__':
    main()
