"""VADPCM decoder.

Each frame is 9 bytes:
    [0]    header: high nibble = scale (shift), low nibble = predictor index
    [1:9]  16 4-bit nibbles, sign-extended to 4-bit signed values (residuals)

A frame is two 8-sample vectors. For each vector:
    1. Initialize an 8-element accumulator from the previous vector's last
       `order` samples (state) dotted with each of the `order` predictor rows:
            for k in 0..order-1:
                for i in 0..7:
                    acc[i] += state[k] * predictor[k][i]
    2. For each residual in 0..7 (within this vector):
            residual_scaled = nibble << scale
            acc[k] += residual_scaled << 11
            for i in 0..6-k:
                acc[k+1+i] += residual_scaled * predictor[order-1][i]
       The second loop is the "triangular extension": each newly-introduced
       residual contributes forward to subsequent samples in the same vector
       via the LAST predictor row.
    3. Output sample[i] = clip_i16(acc[i] >> 11). The 8 outputs become the
       new state for the next vector / frame.

Reference: skelly64 lib/vadpcm/decode.c, N64 SDK convention.
"""
import struct
import wave
from pathlib import Path
from typing import List

from libhm64.audio.wavetable.container import Codebook


def _sign_extend_4bit(n: int) -> int:
    """Sign-extend a 4-bit value (0..15) to a Python int (-8..7)."""
    return n - 16 if n >= 8 else n


def _clip_i16(v: int) -> int:
    if v > 32767:
        return 32767
    if v < -32768:
        return -32768
    return v


def decode(adpcm_bytes: bytes, codebook: Codebook) -> List[int]:
    """Decode VADPCM bytes into a list of int16 PCM samples.

    Returns one full pass through the data (no loop unrolling).
    """
    if codebook.order == 0 or codebook.npredictors == 0:
        return []

    order = codebook.order
    n_pred = codebook.npredictors
    preds = codebook.predictors

    # Last 8 decoded samples; only the last `order` get read back as state.
    state: List[int] = [0] * 8
    out: List[int] = []

    n_frames = len(adpcm_bytes) // 9
    for f in range(n_frames):
        frame = adpcm_bytes[f * 9 : f * 9 + 9]
        header = frame[0]
        scale = header >> 4
        pred_idx = header & 0xF

        if pred_idx >= n_pred:
            raise ValueError(
                f"predictor index {pred_idx} out of range "
                f"(npredictors={n_pred}) at frame {f}"
            )

        # Predictor base offset into the codebook (order rows of 8 shorts).
        pred_base = pred_idx * order * 8
        last_row_base = pred_base + (order - 1) * 8

        # Two vectors of 8 samples each, 4 data bytes per vector.
        for vec in range(2):
            byte_off = 1 + vec * 4
            residuals: List[int] = []
            for b in frame[byte_off:byte_off + 4]:
                residuals.append(_sign_extend_4bit(b >> 4))
                residuals.append(_sign_extend_4bit(b & 0xF))

            # 1. State contribution: previous vector's last `order` samples
            #    dot-producted with each predictor row.
            acc: List[int] = [0] * 8
            for k in range(order):
                s = state[8 - order + k]
                row_base = pred_base + k * 8
                for i in range(8):
                    acc[i] += s * preds[row_base + i]

            # 2. Residual + within-vector triangular extension.
            for k in range(8):
                residual_scaled = residuals[k] << scale
                acc[k] += residual_scaled << 11
                for i in range(7 - k):
                    acc[k + 1 + i] += residual_scaled * preds[last_row_base + i]

            # 3. Final samples + state update.
            samples = [_clip_i16(acc[i] >> 11) for i in range(8)]
            out.extend(samples)
            state = samples

    return out


def write_wav(path: Path, samples: List[int], sample_rate: int) -> None:
    """Write int16 mono PCM samples to a WAV file."""
    with wave.open(str(path), "wb") as w:
        w.setnchannels(1)
        w.setsampwidth(2)        # 16-bit
        w.setframerate(sample_rate)
        frames = struct.pack(f"<{len(samples)}h", *samples)
        w.writeframes(frames)
