"""
HM64 Font Asset Assembler

Assembles font assets from extracted formats back to bin files:
- fontTexture.ci2 -> fontTexture.bin
- fontPaletteN.pal -> fontPaletteN.bin
"""

import argparse
from pathlib import Path

from ..common.binary import align_to

# Palette header: "46d" magic (0x34, 0x36, 0x64) + color count (0x10 = 16)
PALETTE_HEADER = bytes([0x34, 0x36, 0x64, 0x10])
ALIGNMENT = 16

# Default paths
_PACKAGE_DIR = Path(__file__).resolve().parent.parent.parent.parent
DEFAULT_ASSETS_DIR = _PACKAGE_DIR / "assets" / "font"
DEFAULT_OUTPUT_DIR = _PACKAGE_DIR / "build" / "assets" / "font"


def assemble_font_texture(ci2_path: Path, output_path: Path) -> int:
    """Assemble font texture from CI2 file."""
    # Font texture is raw CI2 data with no header
    data = ci2_path.read_bytes()
    output_path.write_bytes(data)
    return len(data)


def assemble_font_palette(pal_path: Path, output_path: Path) -> int:
    """Assemble font palette from .pal file."""
    color_data = pal_path.read_bytes()

    # Reconstruct the bin file:
    # - Header (4 bytes: "46d" + color count)
    # - Color data from .pal file (32 bytes)
    # - Pad to 16-byte alignment

    output_data = align_to(PALETTE_HEADER + color_data, ALIGNMENT)
    output_path.write_bytes(output_data)

    return len(output_data)


def assemble_all(assets_dir: Path = DEFAULT_ASSETS_DIR, output_dir: Path = DEFAULT_OUTPUT_DIR):
    """Assemble all font assets."""
    assets_dir = Path(assets_dir)
    output_dir = Path(output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)

    # Assemble texture
    texture_ci2 = assets_dir / "fontTexture.ci2"
    if texture_ci2.exists():
        texture_bin = output_dir / "fontTexture.bin"
        print(f"Assembling {texture_ci2.name} -> {texture_bin.name}")
        size = assemble_font_texture(texture_ci2, texture_bin)
        print(f"  Written {size} bytes")
    else:
        print(f"Warning: {texture_ci2} not found")

    # Assemble palettes
    for i in range(1, 4):
        palette_pal = assets_dir / f"fontPalette{i}.pal"
        if palette_pal.exists():
            palette_bin = output_dir / f"fontPalette{i}.bin"
            print(f"Assembling {palette_pal.name} -> {palette_bin.name}")
            size = assemble_font_palette(palette_pal, palette_bin)
            print(f"  Written {size} bytes")
        else:
            print(f"Warning: {palette_pal} not found")

    print("Assembly complete")
    return True


def main():
    parser = argparse.ArgumentParser(description="HM64 Font Asset Assembler")
    parser.add_argument("--assets-dir", type=str, default=str(DEFAULT_ASSETS_DIR),
                        help="Directory containing extracted font assets")
    parser.add_argument("--output-dir", type=str, default=str(DEFAULT_OUTPUT_DIR),
                        help="Output directory for assembled bin files")

    args = parser.parse_args()

    assemble_all(Path(args.assets_dir), Path(args.output_dir))


if __name__ == "__main__":
    main()
