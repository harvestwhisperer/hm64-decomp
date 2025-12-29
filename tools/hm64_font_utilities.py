"""
HM64 Font Texture Extraction Utility

Extracts font textures from Harvest Moon 64 ROM using the CI2 (2-bit color indexed) format.
Based on the unpackFontCI2Data function in src/system/message.c.

Font Format:
- Each character is stored as 64 (0x40) bytes of packed CI2 data
- Characters are 14x14 pixels
- Unpacking starts at byte 3 within each character block, reading backwards
- Palettes are 4 colors in RGBA5551 format (8 bytes per palette)
"""

import argparse
import numpy as np
from PIL import Image
from pathlib import Path
from typing import List, Tuple, Optional

# ROM addresses for US version
FONT_TEXTURE_ROM_START = 0xE08870
FONT_TEXTURE_ROM_END = 0xE13770
FONT_PALETTE_1_ROM_START = 0xE13770
FONT_PALETTE_2_ROM_START = 0xE137A0
FONT_PALETTE_3_ROM_START = 0xE137D0
FONT_PALETTE_ROM_END = 0xE13800

# Font parameters
BYTES_PER_CHARACTER = 0x40  # 64 bytes
CHARACTER_WIDTH = 14
CHARACTER_HEIGHT = 14
COLORS_PER_PALETTE = 4
BYTES_PER_PALETTE = 8  # 4 colors * 2 bytes (RGBA5551)

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

# Paths relative to script location
SCRIPT_DIR = Path(__file__).resolve().parent
ROM_PATH = SCRIPT_DIR / "../baserom.us.z64"
OUTPUT_DIR = SCRIPT_DIR / "../assets/font/sprites"

rom = None


def set_rom(rom_path: Path = ROM_PATH):
    """Load ROM into memory."""
    global rom
    rom = memoryview(rom_path.read_bytes())


def rgba5551_to_rgba8888(color_u16: int) -> Tuple[int, int, int, int]:
    """Convert N64 RGBA5551 color to RGBA8888."""
    r = ((color_u16 >> 11) & 0x1F) * 255 // 31
    g = ((color_u16 >> 6) & 0x1F) * 255 // 31
    b = ((color_u16 >> 1) & 0x1F) * 255 // 31
    a = (color_u16 & 0x1) * 255
    return (r, g, b, a)


def get_palette(palette_index: int = 0) -> List[Tuple[int, int, int, int]]:
    """Get a font palette (0, 1, or 2) as RGBA8888 colors."""
    if palette_index == 0:
        palette_start = FONT_PALETTE_1_ROM_START
    elif palette_index == 1:
        palette_start = FONT_PALETTE_2_ROM_START
    else:
        palette_start = FONT_PALETTE_3_ROM_START

    palette = []
    for i in range(COLORS_PER_PALETTE):
        offset = palette_start + i * 2
        # Big-endian 16-bit color
        color_u16 = (rom[offset] << 8) | rom[offset + 1]
        palette.append(rgba5551_to_rgba8888(color_u16))

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


def unpack_font_ci2_data(character_index: int, font_data_start: int) -> List[int]:
    """
    Unpack CI2 font data for a single character.
    Mirrors unpackFontCI2Data from message.c.

    The packed format stores 4 pixels per byte (2 bits each).
    The output is one byte per pixel with values 0-3.
    """
    # Position to character data (each character is 64 bytes)
    source_base = font_data_start + (character_index * BYTES_PER_CHARACTER)

    output = []
    source_offset = 3  # Start at byte 3 of character data

    # Process CHARACTER_WIDTH rows
    for row in range(CHARACTER_WIDTH):
        # Process 4 bytes backward, output up to 8 bytes per row
        # Each source byte contains 4 pixels (2 bits each)

        # Byte 1 (offset 3)
        source_byte = rom[source_base + source_offset]
        output.append(swap_top(source_byte))
        output.append(swap_bottom(source_byte))
        source_offset -= 1

        # Byte 2 (offset 2)
        source_byte = rom[source_base + source_offset]
        output.append(swap_top(source_byte))
        output.append(swap_bottom(source_byte))
        source_offset -= 1

        # Byte 3 (offset 1)
        source_byte = rom[source_base + source_offset]
        output.append(swap_top(source_byte))
        output.append(swap_bottom(source_byte))
        source_offset -= 1

        # Byte 4 (offset 0) - only top nibble
        source_byte = rom[source_base + source_offset]
        output.append(swap_top(source_byte))

        # Skip to next row (source += 7 effectively moves forward by 4 after the 3 decrements)
        source_offset += 7

        # Add padding byte for 8-byte alignment
        output.append(0)

    return output


def extract_character_pixels(character_index: int) -> np.ndarray:
    """
    Extract pixel data for a character and return as a 2D numpy array.
    Values are 0-3 (CI2 palette indices).

    Uses the same unpacking logic as the game's unpackFontCI2Data function.
    """
    unpacked = unpack_font_ci2_data(character_index, FONT_TEXTURE_ROM_START)

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


def create_character_image(character_index: int, palette_index: int = 0) -> Image.Image:
    """Create a PIL Image for a character using the specified palette."""
    pixels = extract_character_pixels(character_index)
    palette = get_palette(palette_index)

    # Create RGBA image
    img = Image.new('RGBA', (CHARACTER_WIDTH, CHARACTER_HEIGHT))
    img_data = img.load()

    for y in range(CHARACTER_HEIGHT):
        for x in range(CHARACTER_WIDTH):
            color_index = pixels[y, x]
            img_data[x, y] = palette[color_index]

    return img


def create_font_sheet(palette_index: int = 0, chars_per_row: int = 16) -> Image.Image:
    """Create a sprite sheet of all font characters."""
    # Calculate number of characters
    font_data_size = FONT_TEXTURE_ROM_END - FONT_TEXTURE_ROM_START
    num_characters = font_data_size // BYTES_PER_CHARACTER

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
            char_img = create_character_image(char_idx, palette_index)
            x = col * CHARACTER_WIDTH
            y = row * CHARACTER_HEIGHT
            sheet.paste(char_img, (x, y))
        except Exception as e:
            print(f"Error extracting character {char_idx}: {e}")

    return sheet


def write_all_characters(output_dir: Path = OUTPUT_DIR, palette_index: int = 0):
    """Extract all font characters to individual PNG files."""
    if rom is None:
        set_rom()

    output_dir.mkdir(parents=True, exist_ok=True)

    font_data_size = FONT_TEXTURE_ROM_END - FONT_TEXTURE_ROM_START
    num_characters = font_data_size // BYTES_PER_CHARACTER

    print(f"Extracting {num_characters} characters...")

    for char_idx in range(num_characters):
        try:
            char_img = create_character_image(char_idx, palette_index)

            # Use character name if available, otherwise use index
            if char_idx in CHAR_NAMES:
                filename = f"char_{char_idx:03d}_{CHAR_NAMES[char_idx]}.png"
            else:
                filename = f"char_{char_idx:03d}.png"

            char_img.save(output_dir / filename)
        except Exception as e:
            print(f"Error extracting character {char_idx}: {e}")

    print(f"Characters saved to {output_dir}")


def write_font_sheet(output_path: Path = None, palette_index: int = 0, chars_per_row: int = 16):
    """Create and save a font sprite sheet."""
    if rom is None:
        set_rom()

    if output_path is None:
        output_path = OUTPUT_DIR / f"font_sheet_palette{palette_index}.png"

    output_path.parent.mkdir(parents=True, exist_ok=True)

    sheet = create_font_sheet(palette_index, chars_per_row)
    sheet.save(output_path)
    print(f"Font sheet saved to {output_path}")


def write_all_palettes(output_dir: Path = OUTPUT_DIR):
    """Write font sheets for all three palettes."""
    if rom is None:
        set_rom()

    output_dir.mkdir(parents=True, exist_ok=True)

    for palette_idx in range(3):
        output_path = output_dir / f"font_sheet_palette{palette_idx + 1}.png"
        write_font_sheet(output_path, palette_idx)


def print_palette_info():
    """Print information about the font palettes."""
    if rom is None:
        set_rom()

    for palette_idx in range(3):
        palette = get_palette(palette_idx)
        print(f"\nPalette {palette_idx + 1}:")
        for i, color in enumerate(palette):
            print(f"  Color {i}: RGBA({color[0]}, {color[1]}, {color[2]}, {color[3]})")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="HM64 Font Texture Extraction Utility")

    parser.add_argument("command",
                        choices=["extract_all", "extract_sheet", "extract_all_palettes", "info"],
                        help="Command to execute")

    parser.add_argument("--palette", type=int, default=0, choices=[0, 1, 2],
                        help="Palette index to use (0, 1, or 2)")

    parser.add_argument("--output", type=str, default=None,
                        help="Output path for sheet or directory for characters")

    parser.add_argument("--chars-per-row", type=int, default=16,
                        help="Characters per row in sprite sheet")

    parser.add_argument("--rom", type=str, default=None,
                        help="Path to ROM file")

    args = parser.parse_args()

    if args.rom:
        ROM_PATH = Path(args.rom)

    set_rom(ROM_PATH)

    if args.command == "extract_all":
        output_dir = Path(args.output) if args.output else OUTPUT_DIR
        write_all_characters(output_dir, args.palette)

    elif args.command == "extract_sheet":
        output_path = Path(args.output) if args.output else None
        write_font_sheet(output_path, args.palette, args.chars_per_row)

    elif args.command == "extract_all_palettes":
        output_dir = Path(args.output) if args.output else OUTPUT_DIR
        write_all_palettes(output_dir)

    elif args.command == "info":
        print_palette_info()
        font_data_size = FONT_TEXTURE_ROM_END - FONT_TEXTURE_ROM_START
        num_characters = font_data_size // BYTES_PER_CHARACTER
        print(f"\nFont Info:")
        print(f"  Texture ROM: 0x{FONT_TEXTURE_ROM_START:X} - 0x{FONT_TEXTURE_ROM_END:X}")
        print(f"  Data size: {font_data_size} bytes")
        print(f"  Characters: {num_characters}")
        print(f"  Character size: {CHARACTER_WIDTH}x{CHARACTER_HEIGHT}")
