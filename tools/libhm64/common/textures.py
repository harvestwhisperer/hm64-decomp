"""
Texture utilities for N64 CI4/CI8 format conversion.

Uses the n64img library to convert color-indexed textures to PNG format,
then flattens to RGBA8 so downstream readers get a real alpha channel.
"""

from pathlib import Path
from n64img.image import CI4, CI8
from PIL import Image


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

    n64img produces an indexed (palette) PNG with a tRNS chunk for
    transparency. That's standards-valid, but some readers (notably Godot's
    built-in PNG loader used by the map editor) ignore tRNS on indexed
    PNGs and return alpha=1.0 for every pixel — which turns alpha-keyed
    pixels into opaque garbage. Re-saving as RGBA8 materializes the
    transparency into an explicit alpha channel that every reader handles.

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

    # Write PNG via n64img, then flatten indexed+tRNS → RGBA8.
    img.write(str(output_path))
    with Image.open(output_path) as loaded:
        rgba = loaded.convert("RGBA")
    rgba.save(output_path, format="PNG", optimize=True)

    # Optional: Write raw dumps if requested
    if keep_intermediate_files:
        stem = output_path.stem
        parent = output_path.parent
        with open(parent / f"{stem}.ci", 'wb') as f:
            f.write(texture)
        with open(parent / f"{stem}.pal", 'wb') as f:
            f.write(palette)
