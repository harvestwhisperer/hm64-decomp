"""Parse a Nintendo VADPCM AIFC file -> Sound.

Inverse of aifc.write_aifc. The chunks we read:
  - FORM/AIFC envelope
  - COMM (validated; sample rate is decoded but the caller already knows it)
  - APPL `stoc` VADPCMCODES (codebook: order, npredictors, predictor table)
  - APPL `stoc` VADPCMLOOPS (optional loop)
  - SSND (the VADPCM byte stream)

INST is read but ignored (HM64 stores no per-instrument values there).
"""
import math
import struct
from pathlib import Path

from libhm64.audio.wavetable.container import Codebook, Loop, Sound


class AifcParseError(Exception):
    pass


def _read_chunk_header(buf: bytes, pos: int) -> tuple[bytes, int, int]:
    if pos + 8 > len(buf):
        raise AifcParseError(f"chunk header truncated at {pos}")
    tag = buf[pos:pos+4]
    size = struct.unpack(">I", buf[pos+4:pos+8])[0]
    return tag, size, pos + 8


def _decode_ieee_extended_80(b: bytes) -> float:
    """Inverse of aifc._ieee_extended_80."""
    if len(b) != 10:
        raise AifcParseError(f"expected 10-byte extended float, got {len(b)}")
    sign_exp = struct.unpack(">H", b[:2])[0]
    mantissa = struct.unpack(">Q", b[2:10])[0]
    sign = -1.0 if (sign_exp & 0x8000) else 1.0
    biased_exp = sign_exp & 0x7FFF
    if biased_exp == 0 and mantissa == 0:
        return 0.0
    exponent = biased_exp - 0x3FFE
    return sign * math.ldexp(mantissa / float(1 << 64), exponent)


def _read_pascal_string(buf: bytes, pos: int) -> tuple[str, int]:
    length = buf[pos]
    end = pos + 1 + length
    s = buf[pos+1:end].decode("ascii")
    # Pad to even.
    if (1 + length) % 2 != 0:
        end += 1
    return s, end


def _parse_comm(payload: bytes) -> tuple[int, int]:
    """Returns (num_samples, sample_rate). Validates VAPC compression type."""
    if len(payload) < 18:
        raise AifcParseError(f"COMM chunk too short: {len(payload)}")
    num_channels = struct.unpack(">h", payload[0:2])[0]
    if num_channels != 1:
        raise AifcParseError(f"expected mono, got {num_channels} channels")
    num_sample_frames = struct.unpack(">I", payload[2:6])[0]
    # sample_size = struct.unpack(">h", payload[6:8])[0]   # 16
    sample_rate = int(_decode_ieee_extended_80(payload[8:18]))
    if len(payload) < 22:
        raise AifcParseError("COMM missing AIFC compression fields")
    compression_type = payload[18:22]
    if compression_type != b"VAPC":
        raise AifcParseError(f"expected VAPC compression, got {compression_type!r}")
    return num_sample_frames, sample_rate


def _parse_appl_codes(inner: bytes) -> Codebook:
    if len(inner) < 6:
        raise AifcParseError(f"VADPCMCODES inner too short: {len(inner)}")
    version, order, npredictors = struct.unpack(">HHH", inner[:6])
    if version != 1:
        raise AifcParseError(f"unexpected VADPCMCODES version {version}")
    n_shorts = order * npredictors * 8
    expected_len = 6 + n_shorts * 2
    if len(inner) < expected_len:
        raise AifcParseError(
            f"VADPCMCODES truncated: got {len(inner)} bytes, need {expected_len}"
        )
    predictors = list(struct.unpack(f">{n_shorts}h", inner[6:6 + n_shorts * 2]))
    return Codebook(order=order, npredictors=npredictors, predictors=predictors)


def _parse_appl_loops(inner: bytes) -> Loop | None:
    if len(inner) < 4:
        raise AifcParseError(f"VADPCMLOOPS inner too short: {len(inner)}")
    version, nloops = struct.unpack(">HH", inner[:4])
    if version != 1:
        raise AifcParseError(f"unexpected VADPCMLOOPS version {version}")
    if nloops == 0:
        return None
    if nloops != 1:
        raise AifcParseError(f"only 1 loop supported, got {nloops}")
    expected_len = 4 + 12 + 32
    if len(inner) < expected_len:
        raise AifcParseError(f"VADPCMLOOPS truncated: {len(inner)} < {expected_len}")
    start, end, count = struct.unpack(">III", inner[4:16])
    state = list(struct.unpack(">16h", inner[16:48]))
    return Loop(start=start, end=end, count=count, state=state)


def _parse_appl(payload: bytes) -> tuple[str, bytes]:
    """Returns (subtype name, inner payload). Skips `stoc` prefix + pascal string."""
    if len(payload) < 4 or payload[:4] != b"stoc":
        raise AifcParseError(f"expected APPL stoc, got {payload[:4]!r}")
    name, inner_start = _read_pascal_string(payload, 4)
    return name, payload[inner_start:]


def parse(buf: bytes) -> Sound:
    """Parse AIFC bytes into a Sound (codebook + optional loop + adpcm bytes)."""
    if len(buf) < 12:
        raise AifcParseError("file too short for FORM header")
    if buf[:4] != b"FORM":
        raise AifcParseError(f"expected FORM, got {buf[:4]!r}")
    form_size = struct.unpack(">I", buf[4:8])[0]
    if buf[8:12] != b"AIFC":
        raise AifcParseError(f"expected AIFC, got {buf[8:12]!r}")

    pos = 12
    end = 8 + form_size
    codebook: Codebook | None = None
    loop: Loop | None = None
    adpcm_bytes: bytes | None = None

    while pos < end:
        tag, size, payload_start = _read_chunk_header(buf, pos)
        payload = buf[payload_start:payload_start + size]
        # Advance past payload + odd-size pad byte.
        pos = payload_start + size + (size % 2)

        if tag == b"COMM":
            _parse_comm(payload)
        elif tag == b"INST":
            pass  # ignored; HM64 has no per-instrument values
        elif tag == b"APPL":
            name, inner = _parse_appl(payload)
            if name == "VADPCMCODES":
                codebook = _parse_appl_codes(inner)
            elif name == "VADPCMLOOPS":
                loop = _parse_appl_loops(inner)
            else:
                raise AifcParseError(f"unknown APPL subtype {name!r}")
        elif tag == b"SSND":
            if len(payload) < 8:
                raise AifcParseError("SSND payload too short")
            offset = struct.unpack(">I", payload[0:4])[0]
            # block_size at [4:8] ignored
            adpcm_bytes = bytes(payload[8 + offset:])
        else:
            raise AifcParseError(f"unknown chunk tag {tag!r}")

    if codebook is None:
        raise AifcParseError("missing VADPCMCODES chunk")
    if adpcm_bytes is None:
        raise AifcParseError("missing SSND chunk")

    return Sound(adpcm_bytes=adpcm_bytes, codebook=codebook, loop=loop)


def read_file(path: Path) -> Sound:
    return parse(Path(path).read_bytes())
