"""
Texture reading and writing utilities for sprite assets.

Handles extraction and assembly of CI4/CI8 textures.
"""

from pathlib import Path
from typing import List, Tuple

from PIL import Image

from ..common import rom, colors
from . import addresses


def extract_texture(addr_base: int, spritesheet_offsets: List[int],
                    sprite_idx: int) -> Tuple[bytes, int, int, str]:
    """
    Extract texture data, dimensions, and format for a sprite.

    Args:
        addr_base: Base address of the sprite asset
        spritesheet_offsets: List of offsets to each sprite
        sprite_idx: Index of the sprite to extract

    Returns:
        Tuple of (texture_data, width, height, format)
        Format is 'ci4' or 'ci8'
    """
    if sprite_idx >= len(spritesheet_offsets) - 1:
        return b'', 0, 0, 'ci8'

    offset = spritesheet_offsets[sprite_idx]
    next_offset = spritesheet_offsets[sprite_idx + 1]

    # Skip if this slot points to same location as next (empty/duplicate slot)
    if offset == next_offset:
        return b'', 0, 0, 'ci8'

    sprite_addr = addr_base + offset

    # Read header
    flags = rom.read_u8(sprite_addr + 3)
    width = rom.read_s16_le(sprite_addr + 4)
    height = rom.read_s16_le(sprite_addr + 6)

    tex_format = 'ci4' if flags == 0x10 else 'ci8'

    # Read texture data
    data_start = sprite_addr + 8
    data_end = addr_base + next_offset

    if data_end <= data_start:
        return b'', 0, 0, tex_format

    tex_data = rom.read_bytes(data_start, data_end)

    return tex_data, width, height, tex_format


def extract_palette(addr_base: int, palette_offsets: List[int],
                    pal_idx: int) -> bytes:
    """
    Extract raw palette bytes.

    Args:
        addr_base: Base address of the palette section
        palette_offsets: List of offsets to each palette
        pal_idx: Index of the palette to extract

    Returns:
        Raw palette bytes (RGBA5551 format)
    """
    if pal_idx >= len(palette_offsets) - 1:
        return b''

    offset = palette_offsets[pal_idx]
    next_offset = palette_offsets[pal_idx + 1]

    start = addr_base + offset + 4  # Skip header
    end = addr_base + next_offset - 4  # Skip padding

    if end <= start:
        return b''

    return rom.read_bytes(start, end)


def palette_to_rgba_list(palette_bytes: bytes) -> List[Tuple[int, int, int, int]]:
    """Convert raw palette bytes to list of RGBA tuples."""
    palette = []
    for i in range(0, len(palette_bytes), 2):
        if i + 1 < len(palette_bytes):
            val = (palette_bytes[i] << 8) | palette_bytes[i + 1]
            palette.append(colors.rgba5551_to_rgba(val))
    return palette


def save_texture_png(tex_data: bytes, width: int, height: int,
                     tex_format: str, palette: List[Tuple[int, int, int, int]],
                     output_path: Path) -> bool:
    """
    Save texture data as indexed PNG.

    Args:
        tex_data: Raw texture bytes
        width: Texture width
        height: Texture height
        tex_format: 'ci4' or 'ci8'
        palette: List of RGBA tuples
        output_path: Path to save PNG

    Returns:
        True if successful
    """
    if width <= 0 or height <= 0:
        return False

    # Create image
    img = Image.new('P', (width, height))

    # Unpack indices
    indices = []
    if tex_format == 'ci4':
        for byte in tex_data:
            indices.append((byte >> 4) & 0x0F)
            indices.append(byte & 0x0F)
    else:
        indices = list(tex_data)

    # Truncate or pad to exact pixel count
    pixel_count = width * height
    if len(indices) < pixel_count:
        indices.extend([0] * (pixel_count - len(indices)))
    else:
        indices = indices[:pixel_count]

    img.putdata(indices)

    # Build PIL palette (flat RGB list, 768 bytes for 256 colors)
    pil_palette = []
    for r, g, b, a in palette:
        pil_palette.extend([r, g, b])
    # Pad to 256 colors
    while len(pil_palette) < 768:
        pil_palette.extend([0, 0, 0])

    img.putpalette(pil_palette)

    # Handle transparency - find transparent color index
    transparent_idx = None
    for idx, (r, g, b, a) in enumerate(palette):
        if a == 0:
            transparent_idx = idx
            break

    output_path.parent.mkdir(parents=True, exist_ok=True)
    img.save(output_path, transparency=transparent_idx)

    return True


def load_texture_png(png_path: Path) -> Tuple[bytes, int, int, str]:
    """
    Load indexed PNG and extract CI data.

    Args:
        png_path: Path to indexed PNG file

    Returns:
        Tuple of (pixel_data, width, height, format)
    """
    img = Image.open(png_path)

    if img.mode != 'P':
        raise ValueError(f"PNG must be indexed (mode P), got {img.mode}")

    width, height = img.size
    indices = list(img.getdata())

    # Determine format based on max index
    max_idx = max(indices) if indices else 0
    tex_format = 'ci4' if max_idx < 16 else 'ci8'

    # Pack pixel data
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


def load_palette_from_png(png_path: Path) -> List[Tuple[int, int, int, int]]:
    """
    Extract palette from an indexed PNG file.

    Args:
        png_path: Path to indexed PNG file

    Returns:
        List of RGBA tuples
    """
    img = Image.open(png_path)

    if img.mode != 'P':
        raise ValueError(f"PNG must be indexed (mode P), got {img.mode}")

    # Get palette
    pal = img.getpalette()
    if pal is None:
        return []

    # Convert to RGBA tuples
    rgba_palette = []
    transparency = img.info.get('transparency')

    for i in range(0, len(pal), 3):
        r, g, b = pal[i], pal[i + 1], pal[i + 2]
        idx = i // 3
        # Check if this index is marked as transparent
        a = 0 if (transparency is not None and idx == transparency) else 255
        rgba_palette.append((r, g, b, a))

    return rgba_palette
