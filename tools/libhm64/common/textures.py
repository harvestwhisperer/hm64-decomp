"""
Texture utilities for N64 CI4/CI8 format conversion.

Uses the n64img library to convert color-indexed textures to PNG format.
"""

from pathlib import Path
from n64img.image import CI4, CI8


def write_texture_png(
    texture: bytes,
    palette: bytes,
    width: int,
    height: int,
    texture_type: str,
    output_path: Path,
    keep_intermediate_files: bool = False
) -> None:
    """
    Write a CI4/CI8 texture to PNG file.

    Args:
        texture: Raw texture data bytes
        palette: Raw palette data bytes (RGBA5551 format, big-endian)
        width: Texture width in pixels
        height: Texture height in pixels
        texture_type: Either 'ci4' or 'ci8'
        output_path: Full path for output PNG file (including .png extension)
        keep_intermediate_files: If True, also write raw .ci and .pal files
    """
    # Initialize n64img object with raw bytes
    if texture_type == 'ci8':
        img = CI8(texture, width, height)
    else:
        img = CI4(texture, width, height)

    # Set palette with raw Big Endian bytes (fixes transparency)
    img.set_palette(palette)

    # Ensure directory exists
    output_path.parent.mkdir(parents=True, exist_ok=True)

    # Write PNG
    img.write(str(output_path))

    # Optional: Write raw dumps if requested
    if keep_intermediate_files:
        stem = output_path.stem
        parent = output_path.parent
        with open(parent / f"{stem}.ci", 'wb') as f:
            f.write(texture)
        with open(parent / f"{stem}.pal", 'wb') as f:
            f.write(palette)
