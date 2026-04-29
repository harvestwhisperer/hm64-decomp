"""VLQ encoding for SFX byte streams.

7+8 bit variable-length quantity (matches tools/modding/music/converter.py):
  value < 0x80         -> one byte literal.
  0x80 <= v <= 0x7FFF  -> two bytes: [0x80 | ((v >> 8) & 0x7F), v & 0xFF].

The continuation bit is the top bit of the first byte. The first byte
carries the high 7 bits of the value; the second byte carries the low 8
bits verbatim (its top bit is data, not a continuation flag).

Used for SET_INSTRUMENT program numbers and NOTE lengths. Verified against
real bytes: sfx65 has `0x80, 0x84` for program 0x84; sfx55 has `0x80, 0x93`
for note length 0x93.
"""

MAX_VALUE = 0x7FFF  # (0x7F << 8) | 0xFF -- limit of two-byte form


def encode(value: int) -> bytes:
    """Encode a non-negative integer as 1 or 2 VLQ bytes."""
    if value < 0:
        raise ValueError(f"VLQ values must be non-negative, got {value}")
    if value < 0x80:
        return bytes([value])
    if value > MAX_VALUE:
        raise ValueError(f"VLQ value {value} exceeds 2-byte range (0x{MAX_VALUE:X})")
    return bytes([0x80 | ((value >> 8) & 0x7F), value & 0xFF])


def decode(data: bytes, offset: int = 0) -> tuple[int, int]:
    """Decode a VLQ value starting at offset. Returns (bytes_consumed, value)."""
    if offset >= len(data):
        raise ValueError(f"VLQ decode past end of buffer (offset={offset}, len={len(data)})")
    first = data[offset]
    if first < 0x80:
        return 1, first
    if offset + 1 >= len(data):
        raise ValueError(f"VLQ continuation byte missing at offset {offset}")
    return 2, ((first & 0x7F) << 8) | data[offset + 1]
