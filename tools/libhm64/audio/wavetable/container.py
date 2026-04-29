"""PtrTablesV2 binary parser.

Container layout matches `ptr_bank_t` in lib/libmus/include/PR/libmus_data.h:
    0x00  unsigned char header_name[16]   "N64 PtrTablesV2\\0"
    0x10  unsigned long flags             0 in the file (set to PTRFLAG_REMAPPED at runtime)
    0x14  unsigned long wbk_name[3]       "BOKUJO01.WBK\\0" — names the wave bank file
    0x20  int           count             number of instruments
    0x24  unsigned char *basenote         offset to basenote[count] (u8 each, MIDI note)
    0x28  float         *detune           offset to detune[count] (4-byte stride;
                                          file stores byte0=u8 cents, bytes1..3=0;
                                          libmus rewrites the slot to a real float
                                          via __MusIntRemapPtrBank in player.c)
    0x2C  ALWaveTable  **wave_list        offset to count u32s pointing at each
                                          ALWaveTable record
    0x30  variable data

Per-instrument record is `ALWaveTable` (lib/libultra/include/PR/libaudio.h:188).
sizeof = 20; HM64 places each record on an 8-byte boundary so the slot is 24
bytes (4-byte trailing pad before the codebook).

ALADPCMBook (codebook):
    0x00  u32 order
    0x04  u32 npredictors
    0x08  signed shorts, length = order * npredictors * 8

ALADPCMloop:
    0x00  u32 start
    0x04  u32 end
    0x08  u32 count
    0x0C  16 signed shorts (state)

All multi-byte values are big-endian (N64 native).
"""
import struct
from dataclasses import dataclass, field
from typing import List, Optional


MAGIC = b"N64 PtrTablesV2\x00"
DATA_MAGIC = b"N64 WaveTables \x00"   # 16-byte header at start of waveTableData.bin
SAMPLE_ALIGN = 16                      # samples in data_bin are 16-byte aligned

# HM64-specific inter-segment padding. These bytes sit between the bank's
# actual end (end of detune array / end of last sample) and the next ROM
# segment in the original ROM. They are NOT part of the V2 bank format
# (verified against N64SoundListTool's WriteAudioN64PtrWavetableV2 at
# N64AIFCAudio.cpp:9420 — the writer does not emit them). Source unknown,
# probably an SDK build-tool footer/checksum. Hardcoded so the lib stays
# free of opaque-bytes round-tripping; pack() always appends them and
# parse() validates them.
POINTER_TRAILER = bytes.fromhex("00ce07c706b0fed7")    # 8 bytes after detune array
DATA_TRAILER    = bytes.fromhex("00c2b02f02dcef26fe")  # 9 bytes after last sample


@dataclass
class Codebook:
    order: int
    npredictors: int
    predictors: List[int]   # length = order * npredictors * 8


@dataclass
class Loop:
    start: int
    end: int
    count: int
    state: List[int]        # 16 signed shorts


@dataclass
class Sound:
    adpcm_bytes: bytes
    codebook: Codebook
    loop: Optional[Loop] = None


@dataclass
class Instrument:
    basenote: int  # MIDI note where the sample plays at native pitch (0-127)
    detune: int    # u8 detune cents (interpreted via U8_TO_FLOAT, see player.c)
    sound: Sound


@dataclass
class Bank:
    name: str                                      # wbk_name in libmus, e.g. "BOKUJO01.WBK"
    sample_rate: int = 22050                       # N64 SDK default; HM64 doesn't override
    instruments: List[Instrument] = field(default_factory=list)


class WavetableParseError(Exception):
    """Raised when the PtrTablesV2 binary is malformed."""


def _read_u32(buf: bytes, offset: int) -> int:
    return struct.unpack(">I", buf[offset:offset+4])[0]


def _read_i16(buf: bytes, offset: int) -> int:
    return struct.unpack(">h", buf[offset:offset+2])[0]


def _read_cstring(buf: bytes, offset: int, max_len: int) -> str:
    chunk = buf[offset:offset+max_len]
    nul = chunk.find(b"\x00")
    if nul == -1:
        return chunk.decode("ascii", errors="replace")
    return chunk[:nul].decode("ascii", errors="replace")


def _parse_codebook(pointer_bin: bytes, offset: int) -> Codebook:
    order = _read_u32(pointer_bin, offset + 0x0)
    npredictors = _read_u32(pointer_bin, offset + 0x4)
    n_shorts = order * npredictors * 8
    predictors = [
        _read_i16(pointer_bin, offset + 0x8 + i * 2) for i in range(n_shorts)
    ]
    return Codebook(order=order, npredictors=npredictors, predictors=predictors)


def _parse_loop(pointer_bin: bytes, offset: int) -> Loop:
    start = _read_u32(pointer_bin, offset + 0x0)
    end = _read_u32(pointer_bin, offset + 0x4)
    count = _read_u32(pointer_bin, offset + 0x8)
    state = [_read_i16(pointer_bin, offset + 0xC + i * 2) for i in range(16)]
    return Loop(start=start, end=end, count=count, state=state)


def parse(pointer_bin: bytes, data_bin: bytes) -> Bank:
    """Parse the PtrTablesV2 container and its sample table into a Bank object."""
    if len(pointer_bin) < 0x30:
        raise WavetableParseError(f"pointer file too short: {len(pointer_bin)} bytes")
    if pointer_bin[:len(MAGIC)] != MAGIC:
        raise WavetableParseError(
            f"bad pointer magic: expected {MAGIC!r}, got {pointer_bin[:len(MAGIC)]!r}"
        )
    if data_bin[:len(DATA_MAGIC)] != DATA_MAGIC:
        raise WavetableParseError(
            f"bad data magic: expected {DATA_MAGIC!r}, got {data_bin[:len(DATA_MAGIC)]!r}"
        )

    bank_name = _read_cstring(pointer_bin, 0x14, 12)
    count = _read_u32(pointer_bin, 0x20)
    basenote_offset = _read_u32(pointer_bin, 0x24)
    detune_offset   = _read_u32(pointer_bin, 0x28)
    wave_list_off   = _read_u32(pointer_bin, 0x2C)

    if count == 0:
        return Bank(name=bank_name, instruments=[])

    if wave_list_off + count * 4 > len(pointer_bin):
        raise WavetableParseError(
            f"wave_list out of bounds (wave_list_off={wave_list_off:#x}, count={count})"
        )

    instruments: List[Instrument] = []
    for i in range(count):
        inst_ptr = _read_u32(pointer_bin, wave_list_off + i * 4)
        if inst_ptr == 0:
            # Empty slot — synthesize a silent instrument with no sound.
            instruments.append(Instrument(basenote=0, detune=0,
                                          sound=Sound(adpcm_bytes=b"", codebook=Codebook(0, 0, []))))
            continue
        if inst_ptr + 16 > len(pointer_bin):
            raise WavetableParseError(
                f"instrument {i} record out of bounds (offset={inst_ptr:#x})"
            )

        wav_base = _read_u32(pointer_bin, inst_ptr + 0x0)
        wav_len  = _read_u32(pointer_bin, inst_ptr + 0x4)
        loop_off = _read_u32(pointer_bin, inst_ptr + 0xC)
        book_off = _read_u32(pointer_bin, inst_ptr + 0x10)

        if wav_base + wav_len > len(data_bin):
            raise WavetableParseError(
                f"instrument {i} sample out of bounds (base={wav_base:#x}, len={wav_len:#x}, "
                f"data_bin={len(data_bin):#x})"
            )
        if book_off == 0 or book_off >= len(pointer_bin):
            raise WavetableParseError(
                f"instrument {i} has invalid codebook offset {book_off:#x}"
            )

        codebook = _parse_codebook(pointer_bin, book_off)
        loop = _parse_loop(pointer_bin, loop_off) if loop_off != 0 else None

        sound = Sound(
            adpcm_bytes=bytes(data_bin[wav_base:wav_base + wav_len]),
            codebook=codebook,
            loop=loop,
        )

        # basenote: 1 byte per entry. detune: file stores byte0 as the u8 input
        # (libmus rewrites the slot to a real float at remap time); the slot
        # itself is 4 bytes wide because at runtime it's `float detune[count]`.
        if basenote_offset + i >= len(pointer_bin):
            raise WavetableParseError(f"basenote array out of bounds for instrument {i}")
        if detune_offset + i * 4 >= len(pointer_bin):
            raise WavetableParseError(f"detune array out of bounds for instrument {i}")
        basenote = pointer_bin[basenote_offset + i]
        detune = pointer_bin[detune_offset + i * 4]

        instruments.append(Instrument(basenote=basenote, detune=detune, sound=sound))

    # Validate inter-segment padding bytes (HM64-specific constants; see
    # POINTER_TRAILER / DATA_TRAILER comments for context).
    pointer_tail = pointer_bin[detune_offset + count * 4:]
    if pointer_tail and pointer_tail != POINTER_TRAILER:
        raise WavetableParseError(
            f"unexpected pointer-bin trailer: got {pointer_tail.hex()}, "
            f"expected {POINTER_TRAILER.hex()}"
        )
    sample_ends = []
    for i in range(count):
        ip = _read_u32(pointer_bin, wave_list_off + i * 4)
        if ip == 0:
            continue
        wav_base = _read_u32(pointer_bin, ip + 0x0)
        wav_len = _read_u32(pointer_bin, ip + 0x4)
        sample_ends.append(wav_base + wav_len)
    data_tail_start = max(sample_ends) if sample_ends else len(DATA_MAGIC)
    data_tail = data_bin[data_tail_start:]
    if data_tail and data_tail != DATA_TRAILER:
        raise WavetableParseError(
            f"unexpected data-bin trailer: got {data_tail.hex()}, "
            f"expected {DATA_TRAILER.hex()}"
        )

    return Bank(
        name=bank_name,
        instruments=instruments,
    )


# ---- Writer ---------------------------------------------------------------

# Per-instrument record is 24 bytes in the original layout. Parser only reads
# 16 meaningful bytes; the other two u32 fields (offsets 0x8 and 0x14) are
# always zero across every entry in HM64's bank.
INST_RECORD_SIZE = 24

# Loop record is 4+4+4 + 16*2 = 44 bytes; followed by a 4-byte alignment pad
# in the original layout (so loop blocks consume 48 bytes total).
LOOP_RECORD_SIZE = 44
LOOP_PAD_SIZE = 4

HEADER_SIZE = 0x30


def _codebook_size(cb: Codebook) -> int:
    return 8 + cb.order * cb.npredictors * 8 * 2


def _emit_codebook(cb: Codebook) -> bytes:
    out = struct.pack(">II", cb.order, cb.npredictors)
    out += struct.pack(f">{len(cb.predictors)}h", *cb.predictors)
    return out


def _emit_loop(lp: Loop) -> bytes:
    out = struct.pack(">III", lp.start, lp.end, lp.count)
    out += struct.pack(">16h", *lp.state)
    return out


def pack(bank: Bank, include_trailers: bool = True) -> tuple[bytes, bytes]:
    """Pack a Bank into (pointer_bin, data_bin) byte tuples.

    Pointer-bin layout:
      - 0x00..0x30        header (magic + flags=0 + wbk_name + count + 3 ptr fields)
      - 0x30..            per-instrument blocks in declaration order:
                            24-byte ALWaveTable slot (20-byte struct + 4-byte align pad)
                            codebook (8 + 16*order*npred bytes)
                            loop (44 bytes) + 4-byte pad   [if present and not last inst]
                            loop (44 bytes) only           [if present and last inst]
      - wave_list_off     instrument pointer table (count * u32 BE)
      - basenote_offset   basenote array (count u8 bytes)
      - 1-byte zero pad   to 4-align detune array
      - detune_offset     detune array (count * 4 bytes; byte0 = u8 input, bytes 1..3 = 0)
      - POINTER_TRAILER (HM64-specific 8-byte SDK footer; only if include_trailers)

    Data-bin layout:
      - 0x00..0x10   DATA_MAGIC ("N64 WaveTables \\0")
      - per sample (declaration order): 16-byte alignment pad + sample bytes
      - DATA_TRAILER (HM64-specific 9-byte SDK footer; only if include_trailers)

    `include_trailers=True` (default) is required for matching builds (master);
    pass False for modding builds (dev/dev-qol) where the SDK-emitted padding
    bytes are unwanted noise.
    """
    count = len(bank.instruments)

    # ---- Data-bin layout pass: assign sample offsets with 16-byte alignment.
    data_parts: List[bytes] = [DATA_MAGIC]
    data_cursor = len(DATA_MAGIC)
    sample_offsets: List[int] = []
    for inst in bank.instruments:
        pad = (-data_cursor) % SAMPLE_ALIGN
        if pad:
            data_parts.append(b"\x00" * pad)
            data_cursor += pad
        sample_offsets.append(data_cursor)
        data_parts.append(inst.sound.adpcm_bytes)
        data_cursor += len(inst.sound.adpcm_bytes)
    if include_trailers:
        data_parts.append(DATA_TRAILER)
    data_bin = b"".join(data_parts)

    # ---- Pointer-bin layout pass: per-instrument blocks.
    inst_block_bytes: List[bytes] = []
    inst_record_offsets: List[int] = []
    pointer_cursor = HEADER_SIZE
    for i, inst in enumerate(bank.instruments):
        sound = inst.sound
        codebook_off = pointer_cursor + INST_RECORD_SIZE
        if sound.loop is not None:
            loop_off = codebook_off + _codebook_size(sound.codebook)
            # 4-byte pad after the loop record for the next inst's 8-alignment;
            # skipped on the LAST instrument (wave_list_off is only 4-aligned).
            tail_pad = LOOP_PAD_SIZE if i < count - 1 else 0
            block_len = (INST_RECORD_SIZE + _codebook_size(sound.codebook)
                         + LOOP_RECORD_SIZE + tail_pad)
        else:
            loop_off = 0
            tail_pad = 0
            block_len = INST_RECORD_SIZE + _codebook_size(sound.codebook)

        record = struct.pack(
            ">IIIIII",
            sample_offsets[i],           # 0x00 wav_base
            len(sound.adpcm_bytes),      # 0x04 wav_len
            0,                           # 0x08 type+flags+pad (zero in file; runtime sets flags)
            loop_off,                    # 0x0C loop_off (or 0)
            codebook_off,                # 0x10 book_off
            0,                           # 0x14 8-byte alignment pad (between struct and codebook)
        )
        block = record + _emit_codebook(sound.codebook)
        if sound.loop is not None:
            block += _emit_loop(sound.loop) + b"\x00" * tail_pad

        inst_block_bytes.append(block)
        inst_record_offsets.append(pointer_cursor)
        pointer_cursor += block_len

    # ---- Pointer-bin tail layout: wave_list, basenote, detune, trailer.
    wave_list_off = pointer_cursor
    pointer_cursor += count * 4
    basenote_offset = pointer_cursor
    pointer_cursor += count
    # 1-byte zero pad to 4-align detune array.
    while pointer_cursor % 4 != 0:
        pointer_cursor += 1
    detune_offset = pointer_cursor
    pointer_cursor += count * 4

    # ---- Assemble pointer_bin.
    out = bytearray()
    out += MAGIC                                              # 0x00..0x10
    out += b"\x00" * 4                                        # 0x10..0x14 flags = 0
    name_bytes = bank.name.encode("ascii")
    if len(name_bytes) > 12:
        raise ValueError(f"bank name {bank.name!r} > 12 bytes")
    out += name_bytes + b"\x00" * (12 - len(name_bytes))      # 0x14..0x20 wbk_name
    out += struct.pack(">IIII", count, basenote_offset, detune_offset, wave_list_off)
    for block in inst_block_bytes:
        out += block
    for off in inst_record_offsets:
        out += struct.pack(">I", off)
    for inst in bank.instruments:
        out += bytes([inst.basenote])
    while len(out) < detune_offset:
        out += b"\x00"
    for inst in bank.instruments:
        # 4-byte detune slot: byte0 = u8 detune input, bytes 1..3 = 0.
        out += bytes([inst.detune, 0, 0, 0])
    if include_trailers:
        out += POINTER_TRAILER

    return bytes(out), data_bin
