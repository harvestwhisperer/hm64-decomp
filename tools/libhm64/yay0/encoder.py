"""Yay0 encoder/decoder for HM64 asset compression.

Yay0 container layout:
  [0..3]   magic "Yay0"
  [4..7]   decompressed size (u32 BE)
  [8..11]  link-table offset  (u32 BE)
  [12..15] chunk-data offset  (u32 BE)
  [16..]   flag bit stream (1 bit per output op, MSB first; 1 = literal, 0 = back-ref)
  [..]     link table (big-endian u16 entries: high 4 bits = length code, low 12 bits = dist-1)
  [..]     chunk data (literal bytes + extended-length bytes)

For length codes: code 0 means "read an extra byte from chunk data, length = byte + 0x12";
otherwise length = code + 2 (so 3..17 inline, 18..273 via extended byte).
"""

import struct
import sys
from pathlib import Path

_MIN_MATCH = 3
_MAX_MATCH_INLINE = 0xF + 2   # 17
_MAX_MATCH_EXT = 0xFF + 0x12  # 273
_MAX_DIST = 0x1000            # 4096


def encode(src: bytes) -> bytes:
    n = len(src)
    flags = bytearray()
    links = bytearray()
    chunks = bytearray()

    # Hash index: hash of 3-byte prefix -> list of positions (most recent first).
    # Cheap acceleration so we don't do O(n * window) on every position.
    index = {}

    def hash3(p):
        return (src[p] << 16) | (src[p + 1] << 8) | src[p + 2]

    def find_match(pos):
        if pos + _MIN_MATCH > n:
            return 0, 0
        max_possible = min(_MAX_MATCH_EXT, n - pos)
        key = hash3(pos)
        bucket = index.get(key)
        if not bucket:
            return 0, 0
        best_len = 0
        best_dist = 0
        window_lo = pos - _MAX_DIST
        # Bucket is newest-first; truncate older entries that fall outside the window.
        while bucket and bucket[-1] < window_lo:
            bucket.pop()
        for src_pos in bucket:
            if src_pos < window_lo:
                break
            mlen = 0
            while mlen < max_possible and src[src_pos + mlen] == src[pos + mlen]:
                mlen += 1
            if mlen > best_len:
                best_len = mlen
                best_dist = pos - src_pos
                if best_len == max_possible:
                    break
        if best_len < _MIN_MATCH:
            return 0, 0
        return best_len, best_dist

    def register(pos):
        if pos + _MIN_MATCH > n:
            return
        key = hash3(pos)
        bucket = index.get(key)
        if bucket is None:
            bucket = []
            index[key] = bucket
        bucket.insert(0, pos)
        # Keep buckets bounded so the match search stays O(1) amortized.
        if len(bucket) > 64:
            bucket.pop()

    flag_byte = 0
    flag_bit_count = 0

    def push_flag(bit):
        nonlocal flag_byte, flag_bit_count
        flag_byte = (flag_byte << 1) | (bit & 1)
        flag_bit_count += 1
        if flag_bit_count == 8:
            flags.append(flag_byte & 0xFF)
            flag_byte = 0
            flag_bit_count = 0

    pos = 0
    while pos < n:
        mlen, mdist = find_match(pos)
        if mlen >= _MIN_MATCH:
            if mlen <= _MAX_MATCH_INLINE:
                links.extend(struct.pack(">H", ((mlen - 2) << 12) | (mdist - 1)))
            else:
                if mlen > _MAX_MATCH_EXT:
                    mlen = _MAX_MATCH_EXT
                links.extend(struct.pack(">H", (mdist - 1) & 0x0FFF))
                chunks.append(mlen - 0x12)
            push_flag(0)
            for i in range(mlen):
                register(pos + i)
            pos += mlen
        else:
            chunks.append(src[pos])
            push_flag(1)
            register(pos)
            pos += 1

    if flag_bit_count > 0:
        flag_byte <<= (8 - flag_bit_count)
        flags.append(flag_byte & 0xFF)

    def pad4(size):
        return (4 - (size % 4)) % 4

    link_offset = 16 + len(flags)
    link_pad = pad4(link_offset)
    link_offset += link_pad
    chunk_offset = link_offset + len(links)
    chunk_pad = pad4(chunk_offset)
    chunk_offset += chunk_pad

    out = bytearray(b"Yay0")
    out.extend(struct.pack(">I", n))
    out.extend(struct.pack(">I", link_offset))
    out.extend(struct.pack(">I", chunk_offset))
    out.extend(flags)
    out.extend(b"\x00" * link_pad)
    out.extend(links)
    out.extend(b"\x00" * chunk_pad)
    out.extend(chunks)

    # DMA alignment: size must be a multiple of 2.
    if len(out) % 2:
        out.append(0)

    return bytes(out)


def decode(data: bytes) -> bytes:
    assert data[:4] == b"Yay0", "bad magic"
    decompressed_size = struct.unpack(">I", data[4:8])[0]
    link_offset = struct.unpack(">I", data[8:12])[0]
    chunk_offset = struct.unpack(">I", data[12:16])[0]

    flag_ptr = 16
    link_ptr = link_offset
    chunk_ptr = chunk_offset

    out = bytearray()
    flag_byte = 0
    flag_bits = 0

    while len(out) < decompressed_size:
        if flag_bits == 0:
            flag_byte = data[flag_ptr]
            flag_ptr += 1
            flag_bits = 8

        if flag_byte & 0x80:
            out.append(data[chunk_ptr])
            chunk_ptr += 1
        else:
            link = struct.unpack(">H", data[link_ptr:link_ptr + 2])[0]
            link_ptr += 2
            dist = (link & 0x0FFF) + 1
            length = link >> 12
            if length == 0:
                length = data[chunk_ptr] + 0x12
                chunk_ptr += 1
            else:
                length += 2
            src_pos = len(out) - dist
            for _ in range(length):
                out.append(out[src_pos])
                src_pos += 1

        flag_byte = (flag_byte << 1) & 0xFF
        flag_bits -= 1

    return bytes(out)


def main():
    if len(sys.argv) != 3:
        print("Usage: encoder.py <input> <output>", file=sys.stderr)
        sys.exit(1)
    src = Path(sys.argv[1]).read_bytes()
    enc = encode(src)
    dec = decode(enc)
    if dec != src:
        print(
            f"Round-trip mismatch: decoded {len(dec)} bytes, expected {len(src)}",
            file=sys.stderr,
        )
        sys.exit(1)
    Path(sys.argv[2]).write_bytes(enc)
    print(
        f"Yay0 {len(src)} -> {len(enc)} bytes "
        f"({100.0 * len(enc) / max(1, len(src)):.1f}%)",
        file=sys.stderr,
    )


if __name__ == "__main__":
    main()
