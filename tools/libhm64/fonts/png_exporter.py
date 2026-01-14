"""
HM64 Font PNG Exporter

Generates viewable PNG images of font sheets and individual characters.
This is useful for inspecting font textures in a human-readable format.

The font format is CI2 (2-bit color indexed) with custom unpacking logic
based on unpackFontCI2Data from src/system/message.c.
"""

import argparse
import numpy as np
from pathlib import Path
from PIL import Image
from typing import List, Tuple

from ..common.rom import read_bytes
from ..common.colors import rgba5551_to_rgba_normalized

# ROM addresses for US version
FONT_TEXTURE_ROM_START = 0xE08870
FONT_TEXTURE_ROM_END = 0xE13770
FONT_PALETTE_1_ROM_START = 0xE13770
FONT_PALETTE_2_ROM_START = 0xE137A0
FONT_PALETTE_3_ROM_START = 0xE137D0

# Font parameters
BYTES_PER_CHARACTER = 0x40  # 64 bytes
CHARACTER_WIDTH = 14
CHARACTER_HEIGHT = 14
COLORS_PER_PALETTE = 4

# Character mappings from message.h
CHAR_NAMES = {
    0xA1: 'A', 0xA2: 'B', 0xA3: 'C', 0xA4: 'D', 0xA5: 'E',
    0xA6: 'F', 0xA7: 'G', 0xA8: 'H', 0xA9: 'I', 0xAA: 'J',
    0xAB: 'K', 0xAC: 'L', 0xAD: 'M', 0xAE: 'N', 0xAF: 'O',
    0xB0: 'P', 0xB1: 'Q', 0xB2: 'R', 0xB3: 'S', 0xB4: 'T',
    0xB5: 'U', 0xB6: 'V', 0xB7: 'W', 0xB8: 'X', 0xB9: 'Y',
    0xBA: 'Z',
    0xBB: 'a', 0xBC: 'b', 0xBD: 'c', 0xBE: 'd', 0xBF: 'e',
    0xC0: 'f', 0xC1: 'g', 0xC2: 'h', 0xC3: 'i', 0xC4: 'j',
    0xC5: 'k', 0xC6: 'l', 0xC7: 'm', 0xC8: 'n', 0xC9: 'o',
    0xCA: 'p', 0xCB: 'q', 0xCC: 'r', 0xCD: 's', 0xCE: 't',
    0xCF: 'u', 0xD0: 'v', 0xD1: 'w', 0xD2: 'x', 0xD3: 'y',
    0xD4: 'z',
    0xD5: '1', 0xD6: '2', 0xD7: '3', 0xD8: '4', 0xD9: '5',
    0xDA: '6', 0xDB: '7', 0xDC: '8', 0xDD: '9', 0xDE: '0',
    0xDF: 'question', 0xE0: 'exclamation', 0xE1: 'dash',
    0xE2: 'tilde', 0xE3: 'period', 0xE4: 'comma', 0xE5: 'dot',
    0xE6: 'slash', 0xE7: 'star1', 0xE8: 'star2', 0xE9: 'ampersand',
    0xEA: 'heart1', 0xEB: 'heart2', 0xEC: 'drop', 0xED: 'paw',
    0xEE: 'space',
}

# Default paths
_PACKAGE_DIR = Path(__file__).resolve().parent.parent.parent.parent
DEFAULT_OUTPUT_DIR = _PACKAGE_DIR / "assets" / "font" / "sprites"


def get_palette(palette_index: int) -> List[Tuple[int, int, int, int]]:
    """
    Get a font palette (1, 2, or 3) as RGBA8888 colors.

    Args:
        palette_index: Palette number (1, 2, or 3)

    Returns:
        List of 4 RGBA tuples
    """
    if palette_index == 1:
        palette_start = FONT_PALETTE_1_ROM_START
    elif palette_index == 2:
        palette_start = FONT_PALETTE_2_ROM_START
    else:
        palette_start = FONT_PALETTE_3_ROM_START

    # Read the first 4 colors (8 bytes) directly from ROM
    # Note: No header skip here - the CI2 palette colors start immediately
    data = read_bytes(palette_start, palette_start + COLORS_PER_PALETTE * 2)

    palette = []
    for i in range(COLORS_PER_PALETTE):
        offset = i * 2
        # Big-endian 16-bit color
        color_u16 = (data[offset] << 8) | data[offset + 1]
        palette.append(rgba5551_to_rgba_normalized(color_u16))

    return palette


def swap_top(source_byte: int) -> int:
    """
    Extract 2-bit value from top nibble.
    Mirrors swapTop from message.c:
    - Takes bits 7-6 (0xC0), shifts right by 2 to positions 5-4
    - Takes bits 5-4 (0x30), shifts right by 4 to positions 1-0
    - Combines them into one output byte
    """
    temp = (source_byte & 0xC0) >> 2  # bits 7-6 -> bits 5-4
    result = temp | ((source_byte & 0x30) >> 4)  # bits 5-4 -> bits 1-0
    return result


def swap_bottom(source_byte: int) -> int:
    """
    Extract 2-bit value from bottom nibble.
    Mirrors swapBottom from message.c:
    - Takes bits 3-2 (0x0C), shifts left by 2 to positions 5-4
    - Takes bits 1-0 (0x03), keeps them at positions 1-0
    - Combines them into one output byte
    """
    temp = (source_byte & 0x0C) << 2  # bits 3-2 -> bits 5-4
    result = temp | (source_byte & 0x03)  # bits 1-0 stay at 1-0
    return result


def unpack_font_ci2_data(character_index: int, font_data: bytes) -> List[int]:
    """
    Unpack CI2 font data for a single character.
    Mirrors unpackFontCI2Data from message.c.

    The packed format stores 4 pixels per byte (2 bits each).
    The output is one byte per pixel with values 0-3.

    Args:
        character_index: Character index in the font
        font_data: Raw font texture data from ROM

    Returns:
        List of unpacked bytes (one per pixel)
    """
    # Position to character data (each character is 64 bytes)
    source_base = character_index * BYTES_PER_CHARACTER

    output = []
    source_offset = 3  # Start at byte 3 of character data

    # Process CHARACTER_HEIGHT rows
    for row in range(CHARACTER_HEIGHT):
        # Process 4 bytes backward, output up to 8 bytes per row
        # Each source byte contains 4 pixels (2 bits each)

        # Byte 1 (offset 3)
        source_byte = font_data[source_base + source_offset]
        output.append(swap_top(source_byte))
        output.append(swap_bottom(source_byte))
        source_offset -= 1

        # Byte 2 (offset 2)
        source_byte = font_data[source_base + source_offset]
        output.append(swap_top(source_byte))
        output.append(swap_bottom(source_byte))
        source_offset -= 1

        # Byte 3 (offset 1)
        source_byte = font_data[source_base + source_offset]
        output.append(swap_top(source_byte))
        output.append(swap_bottom(source_byte))
        source_offset -= 1

        # Byte 4 (offset 0) - only top nibble
        source_byte = font_data[source_base + source_offset]
        output.append(swap_top(source_byte))

        # Skip to next row (source += 7 effectively moves forward by 4 after the 3 decrements)
        source_offset += 7

        # Add padding byte for 8-byte alignment
        output.append(0)

    return output


def extract_character_pixels(character_index: int, font_data: bytes) -> np.ndarray:
    """
    Extract pixel data for a character and return as a 2D numpy array.
    Values are 0-3 (CI2 palette indices).

    Uses the same unpacking logic as the game's unpackFontCI2Data function.

    Args:
        character_index: Character index in the font
        font_data: Raw font texture data from ROM

    Returns:
        2D numpy array of palette indices (14x14)
    """
    unpacked = unpack_font_ci2_data(character_index, font_data)

    # Create pixel array
    pixels = np.zeros((CHARACTER_HEIGHT, CHARACTER_WIDTH), dtype=np.uint8)

    # The unpacked data has 8 bytes per row (7 with pixel data + 1 padding).
    # Each byte contains 2 pixel values in bits 5-4 and 1-0.
    for row in range(CHARACTER_HEIGHT):
        row_start = row * 8
        x = 0

        for col in range(7):  # 7 output bytes per row (8th is padding)
            if row_start + col < len(unpacked):
                byte_val = unpacked[row_start + col]
                # Extract the 2-bit pixel values
                pixel1 = (byte_val >> 4) & 0x03
                pixel2 = byte_val & 0x03

                if x < CHARACTER_WIDTH:
                    pixels[row, x] = pixel1
                    x += 1
                if x < CHARACTER_WIDTH:
                    pixels[row, x] = pixel2
                    x += 1

    return pixels


def create_character_image(character_index: int, palette_index: int, font_data: bytes) -> Image.Image:
    """
    Create a PIL Image for a character using the specified palette.

    Args:
        character_index: Character index in the font
        palette_index: Palette number (1, 2, or 3)
        font_data: Raw font texture data from ROM

    Returns:
        PIL Image (RGBA, 14x14)
    """
    pixels = extract_character_pixels(character_index, font_data)
    palette = get_palette(palette_index)

    # Create RGBA image
    img = Image.new('RGBA', (CHARACTER_WIDTH, CHARACTER_HEIGHT))
    img_data = img.load()

    for y in range(CHARACTER_HEIGHT):
        for x in range(CHARACTER_WIDTH):
            color_index = pixels[y, x]
            img_data[x, y] = palette[color_index]

    return img


def create_font_sheet(palette_index: int, chars_per_row: int = 16) -> Image.Image:
    """
    Create a sprite sheet of all font characters.

    Args:
        palette_index: Palette number (1, 2, or 3)
        chars_per_row: Number of characters per row in the sheet

    Returns:
        PIL Image containing all characters
    """
    # Load font data from ROM
    font_data = read_bytes(FONT_TEXTURE_ROM_START, FONT_TEXTURE_ROM_END)

    # Calculate number of characters
    num_characters = len(font_data) // BYTES_PER_CHARACTER

    # Calculate sheet dimensions
    num_rows = (num_characters + chars_per_row - 1) // chars_per_row
    sheet_width = chars_per_row * CHARACTER_WIDTH
    sheet_height = num_rows * CHARACTER_HEIGHT

    # Create the sheet
    sheet = Image.new('RGBA', (sheet_width, sheet_height), (0, 0, 0, 0))

    for char_idx in range(num_characters):
        row = char_idx // chars_per_row
        col = char_idx % chars_per_row

        try:
            char_img = create_character_image(char_idx, palette_index, font_data)
            x = col * CHARACTER_WIDTH
            y = row * CHARACTER_HEIGHT
            sheet.paste(char_img, (x, y))
        except Exception as e:
            print(f"Error extracting character {char_idx}: {e}")

    return sheet


def write_all_characters(output_dir: Path, palette_index: int = 1):
    """
    Extract all font characters to individual PNG files.

    Args:
        output_dir: Directory to save character PNGs
        palette_index: Palette number (1, 2, or 3)
    """
    output_dir = Path(output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)

    # Load font data from ROM
    font_data = read_bytes(FONT_TEXTURE_ROM_START, FONT_TEXTURE_ROM_END)
    num_characters = len(font_data) // BYTES_PER_CHARACTER

    print(f"Extracting {num_characters} characters to {output_dir}...")

    for char_idx in range(num_characters):
        try:
            char_img = create_character_image(char_idx, palette_index, font_data)

            # Use character name if available, otherwise use index
            if char_idx in CHAR_NAMES:
                filename = f"char_{char_idx:03d}_{CHAR_NAMES[char_idx]}.png"
            else:
                filename = f"char_{char_idx:03d}.png"

            char_img.save(output_dir / filename)
        except Exception as e:
            print(f"Error extracting character {char_idx}: {e}")

    print(f"Done! Characters saved to {output_dir}")


def write_font_sheet(output_path: Path, palette_index: int = 1, chars_per_row: int = 16):
    """
    Create and save a font sprite sheet.

    Args:
        output_path: Path to save the PNG file
        palette_index: Palette number (1, 2, or 3)
        chars_per_row: Number of characters per row in the sheet
    """
    output_path = Path(output_path)
    output_path.parent.mkdir(parents=True, exist_ok=True)

    sheet = create_font_sheet(palette_index, chars_per_row)
    sheet.save(output_path)
    print(f"Font sheet saved to {output_path}")


def write_all_palettes(output_dir: Path):
    """
    Write font sheets for all three palettes.

    Args:
        output_dir: Directory to save the font sheets
    """
    output_dir = Path(output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)

    print(f"Generating font sheets for all palettes...")
    for palette_idx in range(1, 4):
        output_path = output_dir / f"font_sheet_palette{palette_idx}.png"
        write_font_sheet(output_path, palette_idx)

    print(f"Done! Font sheets saved to {output_dir}")


def main():
    """Command-line interface for font PNG export."""
    parser = argparse.ArgumentParser(
        description="Generate viewable PNG images of HM64 font sheets and characters"
    )

    parser.add_argument(
        "command",
        choices=["extract_all", "extract_sheet", "extract_all_palettes"],
        help="Command to execute"
    )

    parser.add_argument(
        "--palette",
        type=int,
        default=1,
        choices=[1, 2, 3],
        help="Palette to use (1, 2, or 3)"
    )

    parser.add_argument(
        "--output",
        type=str,
        default=None,
        help="Output path or directory"
    )

    parser.add_argument(
        "--chars-per-row",
        type=int,
        default=16,
        help="Characters per row in sprite sheet"
    )

    parser.add_argument(
        "--rom",
        type=str,
        default=None,
        help="Path to ROM file"
    )

    args = parser.parse_args()

    # Set ROM path if provided
    if args.rom:
        from ..common.rom import set_rom_path
        set_rom_path(Path(args.rom))

    # Execute command
    if args.command == "extract_all":
        output_dir = Path(args.output) if args.output else DEFAULT_OUTPUT_DIR
        write_all_characters(output_dir, args.palette)

    elif args.command == "extract_sheet":
        if args.output:
            output_path = Path(args.output)
        else:
            output_path = DEFAULT_OUTPUT_DIR / f"font_sheet_palette{args.palette}.png"
        write_font_sheet(output_path, args.palette, args.chars_per_row)

    elif args.command == "extract_all_palettes":
        output_dir = Path(args.output) if args.output else DEFAULT_OUTPUT_DIR
        write_all_palettes(output_dir)


if __name__ == "__main__":
    main()
