"""
ROM reading utilities for HM64 asset tools.

Provides cached ROM access to avoid repeatedly reading the ROM file.
"""

import struct
from pathlib import Path
from typing import Optional

# Default ROM path relative to repository root
_DEFAULT_ROM_PATH = Path(__file__).resolve().parent.parent.parent.parent / "baserom.us.z64"

_rom: Optional[memoryview] = None
_rom_path: Path = _DEFAULT_ROM_PATH


def set_rom_path(path: Path) -> None:
    """Set the ROM path and clear any cached ROM data."""
    global _rom_path, _rom
    _rom_path = Path(path)
    _rom = None


def get_rom() -> memoryview:
    """Get the ROM as a memoryview, loading from disk if necessary."""
    global _rom
    if _rom is None:
        _rom = memoryview(_rom_path.read_bytes())
    return _rom


def clear_cache() -> None:
    """Clear the cached ROM data."""
    global _rom
    _rom = None


def read_u32_be(offset: int) -> int:
    """Read unsigned 32-bit big-endian value from ROM."""
    rom = get_rom()
    return struct.unpack(">I", rom[offset:offset+4])[0]


def read_u16_be(offset: int) -> int:
    """Read unsigned 16-bit big-endian value from ROM."""
    rom = get_rom()
    return struct.unpack(">H", rom[offset:offset+2])[0]


def read_u16_le(offset: int) -> int:
    """Read unsigned 16-bit little-endian value from ROM."""
    rom = get_rom()
    return struct.unpack("<H", rom[offset:offset+2])[0]


def read_s16_le(offset: int) -> int:
    """Read signed 16-bit little-endian value from ROM."""
    rom = get_rom()
    return struct.unpack("<h", rom[offset:offset+2])[0]


def read_u8(offset: int) -> int:
    """Read unsigned 8-bit value from ROM."""
    rom = get_rom()
    return rom[offset]


def read_bytes(start: int, end: int) -> bytes:
    """Read a range of bytes from ROM."""
    rom = get_rom()
    return bytes(rom[start:end])


class RomReader:
    """Sequential reader with position tracking for parsing ROM data."""

    def __init__(self, data: bytes, start: int = 0):
        self.data = data
        self.pos = start

    @classmethod
    def from_rom(cls, start: int, end: int) -> 'RomReader':
        """Create a RomReader from a ROM slice."""
        return cls(read_bytes(start, end))

    def read_u8(self) -> int:
        """Read unsigned 8-bit value and advance position."""
        val = self.data[self.pos]
        self.pos += 1
        return val

    def read_s8(self) -> int:
        """Read signed 8-bit value and advance position."""
        val = self.data[self.pos]
        self.pos += 1
        return val - 256 if val >= 128 else val

    def read_u16(self) -> int:
        """Read unsigned 16-bit big-endian value and advance position."""
        val = struct.unpack_from('>H', self.data, self.pos)[0]
        self.pos += 2
        return val

    def read_s16(self) -> int:
        """Read signed 16-bit big-endian value and advance position."""
        val = struct.unpack_from('>h', self.data, self.pos)[0]
        self.pos += 2
        return val

    def read_u32(self) -> int:
        """Read unsigned 32-bit big-endian value and advance position."""
        val = struct.unpack_from('>I', self.data, self.pos)[0]
        self.pos += 4
        return val

    def read_s32(self) -> int:
        """Read signed 32-bit big-endian value and advance position."""
        val = struct.unpack_from('>i', self.data, self.pos)[0]
        self.pos += 4
        return val

    def skip(self, n: int) -> None:
        """Skip n bytes."""
        self.pos += n

    def peek_u8(self) -> int:
        """Peek at next byte without advancing."""
        return self.data[self.pos]

    def peek_u16(self) -> int:
        """Peek at next 2 bytes as u16 without advancing."""
        return struct.unpack_from('>H', self.data, self.pos)[0]

    def eof(self) -> bool:
        """Check if at end of data."""
        return self.pos >= len(self.data)
