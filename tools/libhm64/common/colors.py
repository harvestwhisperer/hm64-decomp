"""
Color conversion utilities for N64 graphics formats.

The N64 uses RGBA5551 format: 5 bits each for R, G, B and 1 bit for alpha.
"""

from typing import Tuple


def rgba5551_to_rgba(val: int) -> Tuple[int, int, int, int]:
    """
    Convert N64 RGBA5551 color to RGBA8888.

    RGBA5551 layout: RRRRR GGGGG BBBBB A (16 bits)
    """
    r = ((val >> 11) & 0x1F) << 3
    g = ((val >> 6) & 0x1F) << 3
    b = ((val >> 1) & 0x1F) << 3
    a = 255 if (val & 1) else 0
    return (r, g, b, a)


def rgba_to_rgba5551(r: int, g: int, b: int, a: int) -> int:
    """
    Convert RGBA8888 to N64 RGBA5551 format.

    Args:
        r, g, b: Color components (0-255)
        a: Alpha component (0-255, threshold at 128)

    Returns:
        16-bit RGBA5551 value
    """
    r5 = (r >> 3) & 0x1F
    g5 = (g >> 3) & 0x1F
    b5 = (b >> 3) & 0x1F
    a1 = 1 if a >= 128 else 0
    return (r5 << 11) | (g5 << 6) | (b5 << 1) | a1


def rgba5551_to_rgba_normalized(val: int) -> Tuple[int, int, int, int]:
    """
    Convert N64 RGBA5551 color to RGBA8888 with proper value scaling.

    This version scales 5-bit values (0-31) to full 8-bit range (0-255)
    using multiplication rather than bit shifting, for more accurate colors.
    """
    r = ((val >> 11) & 0x1F) * 255 // 31
    g = ((val >> 6) & 0x1F) * 255 // 31
    b = ((val >> 1) & 0x1F) * 255 // 31
    a = (val & 0x1) * 255
    return (r, g, b, a)
