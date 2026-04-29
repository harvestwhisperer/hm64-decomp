"""Nintendo VADPCM AIFC writer.

Output is a valid IFF AIFC file with two non-standard APPL chunks holding
the VADPCM codebook and (optional) loop record.

References:
  - https://muffinlabs.com/aiff-1.3.pdf (AIFF/AIFC base format)
  - sm64 decomp tools/audio/vadpcm_enc.c (Nintendo's APPL chunks)
"""
import math
import struct
from pathlib import Path

from libhm64.audio.wavetable.container import Codebook, Loop, Sound


def _ieee_extended_80(value: float) -> bytes:
    """Encode a float as an 80-bit IEEE 754 extended-precision number (big-endian).

    Used by AIFC's COMM chunk for sample rate.
    """
    if value < 0:
        sign = 0x8000
        value = -value
    else:
        sign = 0
    if value == 0:
        return struct.pack(">HQ", sign, 0)
    mantissa, exponent = math.frexp(value)
    # IEEE 80-bit: 1-bit sign, 15-bit biased exponent (bias 0x3FFF),
    # 64-bit mantissa with explicit leading 1.
    biased_exp = exponent + 0x3FFE   # mantissa is in [0.5, 1) -> exp shifted by -1
    int_mantissa = int(mantissa * (1 << 64))
    return struct.pack(">HQ", sign | biased_exp, int_mantissa)


def _pascal_string(s: str) -> bytes:
    """Pascal string: 1 length byte + content + optional pad byte to even."""
    encoded = s.encode("ascii")
    out = bytes([len(encoded)]) + encoded
    if len(out) % 2 != 0:
        out += b"\x00"
    return out


def _chunk(tag: bytes, payload: bytes) -> bytes:
    """Wrap a payload as an IFF chunk: 4-byte tag + u32 BE size + payload + pad to even."""
    if len(tag) != 4:
        raise ValueError(f"chunk tag must be 4 bytes, got {tag!r}")
    out = tag + struct.pack(">I", len(payload)) + payload
    if len(payload) % 2 != 0:
        out += b"\x00"
    return out


def _build_comm_chunk(num_samples: int, sample_rate: int) -> bytes:
    """COMM chunk for VADPCM AIFC (compressed)."""
    payload = (
        struct.pack(">h", 1)                      # numChannels
        + struct.pack(">I", num_samples)          # numSampleFrames
        + struct.pack(">h", 16)                   # sampleSize (bits)
        + _ieee_extended_80(float(sample_rate))   # sampleRate (80-bit IEEE 754)
        + b"VAPC"                                  # compressionType
        + _pascal_string("VADPCM ~4-to-1")        # compressionName
    )
    return _chunk(b"COMM", payload)


def _build_inst_chunk() -> bytes:
    """Default INST chunk (HM64 doesn't store these per-instrument values)."""
    payload = (
        struct.pack(">b", 60)         # baseNote (middle C)
        + struct.pack(">b", 0)        # detune
        + struct.pack(">b", 0)        # lowNote
        + struct.pack(">b", 127)      # highNote
        + struct.pack(">b", 1)        # lowVelocity
        + struct.pack(">b", 127)      # highVelocity
        + struct.pack(">h", 0)        # gain
        # Sustain loop: playMode, beginLoop, endLoop (markers, 0 = none)
        + struct.pack(">h", 0) + struct.pack(">h", 0) + struct.pack(">h", 0)
        # Release loop
        + struct.pack(">h", 0) + struct.pack(">h", 0) + struct.pack(">h", 0)
    )
    return _chunk(b"INST", payload)


def _build_ssnd_chunk(adpcm_bytes: bytes) -> bytes:
    payload = struct.pack(">II", 0, 0) + adpcm_bytes
    return _chunk(b"SSND", payload)


def _build_vadpcmcodes_chunk(codebook: Codebook) -> bytes:
    """APPL stoc VADPCMCODES."""
    inner = (
        struct.pack(">H", 1)                       # version
        + struct.pack(">H", codebook.order)
        + struct.pack(">H", codebook.npredictors)
    )
    for s in codebook.predictors:
        inner += struct.pack(">h", s)
    payload = b"stoc" + _pascal_string("VADPCMCODES") + inner
    return _chunk(b"APPL", payload)


def _build_vadpcmloops_chunk(loop: Loop | None) -> bytes | None:
    if loop is None:
        return None
    inner = struct.pack(">HH", 1, 1)               # version, nloops
    inner += struct.pack(">III", loop.start, loop.end, loop.count)
    for s in loop.state:
        inner += struct.pack(">h", s)
    payload = b"stoc" + _pascal_string("VADPCMLOOPS") + inner
    return _chunk(b"APPL", payload)


def write_aifc(path: Path, sound: Sound, sample_rate: int) -> None:
    """Write a sound (ADPCM bytes + codebook + optional loop) as a Nintendo VADPCM AIFC."""
    # numSamples in AIFC is the post-decode sample count: 16 samples per 9-byte frame.
    n_frames = len(sound.adpcm_bytes) // 9
    num_samples = n_frames * 16

    chunks = [b"AIFC"]
    chunks.append(_build_comm_chunk(num_samples, sample_rate))
    chunks.append(_build_inst_chunk())
    chunks.append(_build_vadpcmcodes_chunk(sound.codebook))
    loops_chunk = _build_vadpcmloops_chunk(sound.loop)
    if loops_chunk is not None:
        chunks.append(loops_chunk)
    chunks.append(_build_ssnd_chunk(sound.adpcm_bytes))

    body = b"".join(chunks)
    form = _chunk(b"FORM", body)
    path.write_bytes(form)
