"""
HM64 Font Asset Extractor

Extracts font texture and palette data directly from ROM to editable formats:
- fontTexture.ci2: Raw CI2 packed character data (44800 bytes, 700 characters)
- fontPaletteN.pal: Raw RGBA5551 color data (32 bytes, 16 colors)
"""

import argparse
from pathlib import Path

from ..common.rom import get_rom, set_rom_path, read_bytes

# ROM addresses for US version
FONT_TEXTURE_ROM_START = 0xE08870
FONT_TEXTURE_ROM_END = 0xE13770
FONT_PALETTE_1_ROM_START = 0xE13770
FONT_PALETTE_1_ROM_END = 0xE137A0
FONT_PALETTE_2_ROM_START = 0xE137A0
FONT_PALETTE_2_ROM_END = 0xE137D0
FONT_PALETTE_3_ROM_START = 0xE137D0
FONT_PALETTE_3_ROM_END = 0xE13800

# Font parameters
BYTES_PER_CHARACTER = 0x40  # 64 bytes
CHARACTER_WIDTH = 14
CHARACTER_HEIGHT = 14
PALETTE_HEADER_SIZE = 4  # "46d" magic + color count byte
PALETTE_COLOR_COUNT = 16
PALETTE_COLOR_DATA_SIZE = PALETTE_COLOR_COUNT * 2  # 32 bytes

# Default paths
_PACKAGE_DIR = Path(__file__).resolve().parent.parent.parent.parent
DEFAULT_OUTPUT_DIR = _PACKAGE_DIR / "assets" / "font"


def extract_font_texture(output_path: Path):
    """Extract font texture from ROM to raw CI2 file."""
    data = read_bytes(FONT_TEXTURE_ROM_START, FONT_TEXTURE_ROM_END)
    output_path.write_bytes(data)


def extract_font_palette(output_path: Path, palette_index: int):
    """Extract font palette from ROM to raw .pal file."""
    # Get ROM addresses for this palette
    if palette_index == 1:
        rom_start = FONT_PALETTE_1_ROM_START
        rom_end = FONT_PALETTE_1_ROM_END
    elif palette_index == 2:
        rom_start = FONT_PALETTE_2_ROM_START
        rom_end = FONT_PALETTE_2_ROM_END
    else:
        rom_start = FONT_PALETTE_3_ROM_START
        rom_end = FONT_PALETTE_3_ROM_END

    data = read_bytes(rom_start, rom_end)

    # Palette format in ROM:
    # - 4 bytes: header ("46d" magic + color count 0x10)
    # - 32 bytes: 16 colors x 2 bytes RGBA5551
    # - 12 bytes: alignment padding to 16-byte boundary

    color_data = data[PALETTE_HEADER_SIZE:PALETTE_HEADER_SIZE + PALETTE_COLOR_DATA_SIZE]
    output_path.write_bytes(color_data)


def extract_all(output_dir: Path = DEFAULT_OUTPUT_DIR):
    """Extract all font assets from ROM."""
    output_dir = Path(output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)

    # Extract texture
    texture_ci2 = output_dir / "fontTexture.ci2"
    print(f"Extracting fontTexture from ROM -> {texture_ci2.name}")
    extract_font_texture(texture_ci2)

    # Extract palettes
    for i in range(1, 4):
        palette_pal = output_dir / f"fontPalette{i}.pal"
        print(f"Extracting fontPalette{i} from ROM -> {palette_pal.name}")
        extract_font_palette(palette_pal, i)


def main():
    parser = argparse.ArgumentParser(description="HM64 Font Asset Extractor")
    parser.add_argument("--output-dir", type=str, default=str(DEFAULT_OUTPUT_DIR),
                        help="Output directory for extracted assets")
    parser.add_argument("--rom", type=str, default=None,
                        help="Path to ROM file")

    args = parser.parse_args()

    if args.rom:
        set_rom_path(Path(args.rom))

    extract_all(Path(args.output_dir))


if __name__ == "__main__":
    main()
