"""
Binary packing utilities for building game assets.

These functions pack Python values into bytes for writing to binary files.
"""

import struct


def pack_u32_be(val: int) -> bytes:
    """Pack unsigned 32-bit big-endian."""
    return struct.pack(">I", val)


def pack_u32_le(val: int) -> bytes:
    """Pack unsigned 32-bit little-endian."""
    return struct.pack("<I", val)


def pack_u16_be(val: int) -> bytes:
    """Pack unsigned 16-bit big-endian."""
    return struct.pack(">H", val)


def pack_u16_le(val: int) -> bytes:
    """Pack unsigned 16-bit little-endian."""
    return struct.pack("<H", val)


def pack_s16_be(val: int) -> bytes:
    """Pack signed 16-bit big-endian."""
    return struct.pack(">h", val)


def pack_s16_le(val: int) -> bytes:
    """Pack signed 16-bit little-endian."""
    return struct.pack("<h", val)


def pack_u8(val: int) -> bytes:
    """Pack unsigned 8-bit."""
    return struct.pack("B", val)


def pack_s8(val: int) -> bytes:
    """Pack signed 8-bit."""
    return struct.pack("b", val)


def align_to(data: bytes, alignment: int, pad_byte: int = 0) -> bytes:
    """
    Pad data with bytes to reach the next alignment boundary.

    Args:
        data: The data to align
        alignment: The alignment boundary (e.g., 16 for 16-byte alignment)
        pad_byte: The byte value to use for padding (default 0)

    Returns:
        The data padded to the alignment boundary
    """
    if len(data) % alignment == 0:
        return data
    padding_needed = alignment - (len(data) % alignment)
    return data + bytes([pad_byte] * padding_needed)
