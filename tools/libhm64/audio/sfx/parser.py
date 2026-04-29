"""SFX DSL parser.

Recognizes:
  - Comments (`;` lines), `#include` lines, blank lines: ignored.
  - Sample shorthand:
        .NAME:           sample
    Marks an entry whose audible sound is produced by the sample-playback
    path (`setSfx(index+1)` in gameAudio.c) rather than by executing the
    micro-sequence. The byte stream is a no-op placeholder
    `[0x80, 0x00, 0x00, 0x00]` (END_OF_TRACK + zero pad).
  - Play shorthand:
        .NAME:           play instr=<int> vol=<byte> note=<byte> length=<int>
    where `instr` and `length` are decoded values; the parser VLQ-encodes
    them on emit (1 byte for < 0x80, 2 bytes for 0x80..0x7FFF).
  - Opcode block:
        .NAME:
            SET_INSTRUMENT  <int>
            ADSR_SET        b0, b1, b2, b3, b4, b5, b6
            PAN             <byte>
            NOTE_VOL        <byte>
            NOTE            <pitch>, <length>
            END
  - priorities { ... } block:
        .LABEL:           <int>      (one per line)

Output bytes for non-sample forms are padded to a 4-byte boundary with
0x00, mirroring the original sfx.c (all 63 non-sample SFX are 20 bytes).
"""
import re
from dataclasses import dataclass
from typing import List, Tuple

from libhm64.audio.sfx import opcodes, vlq


@dataclass
class SfxEntry:
    label: str           # without leading '.' or trailing ':'
    sfx_bytes: List[int]


_SHORTHAND_RE = re.compile(
    r"^\.(?P<label>\w+):\s+play\s+"
    r"instr=(?P<instr>0x[0-9A-Fa-f]+)\s+"
    r"vol=(?P<vol>0x[0-9A-Fa-f]+)\s+"
    r"note=(?P<note>0x[0-9A-Fa-f]+)\s+"
    r"length=(?P<length>0x[0-9A-Fa-f]+)\s*$"
)
_SAMPLE_RE = re.compile(r"^\.(?P<label>\w+):\s+sample\s*$")
_LABEL_ONLY_RE = re.compile(r"^\.(?P<label>\w+):\s*$")
_OPCODE_LINE_RE = re.compile(r"^\s+(?P<mnem>[A-Z_]+)(?:\s+(?P<args>.+))?$")
_PRIORITIES_OPEN_RE = re.compile(r"^priorities\s*\{\s*$")
_PRIORITIES_LINE_RE = re.compile(r"^\s+\.(?P<label>\w+):\s+(?P<value>-?\d+)\s*$")
_PRIORITIES_CLOSE_RE = re.compile(r"^\}\s*$")


def _parse_args(s: str | None) -> List[int]:
    if not s:
        return []
    return [int(x.strip(), 0) for x in s.split(",")]


def _pad_to_4(out: List[int]) -> List[int]:
    while len(out) % 4 != 0:
        out.append(0x00)
    return out


def _emit_shorthand_bytes(instr: int, vol: int, note: int, length: int) -> List[int]:
    """Build the canonical 5-opcode byte sequence + 4-byte alignment padding."""
    out: List[int] = []
    out.append(opcodes.SET_INSTRUMENT)
    out += list(vlq.encode(instr))
    out.append(opcodes.ADSR_SET)
    out += list(opcodes.CANONICAL_ADSR)
    out.append(opcodes.PAN)
    out.append(0x7F)
    out.append(opcodes.NOTE_VOL)
    out.append(vol)
    out.append(note)
    out += list(vlq.encode(length))
    out.append(opcodes.END_OF_TRACK)
    return _pad_to_4(out)


def _emit_opcode_block_bytes(opcode_lines: List[Tuple[str, List[int]]]) -> List[int]:
    """Convert a list of (mnemonic, args) into raw bytes, then 4-byte pad."""
    out: List[int] = []
    for mnem, args in opcode_lines:
        if mnem == "END":
            out.append(opcodes.END_OF_TRACK)
            break
        if mnem == "SET_INSTRUMENT":
            if len(args) != 1:
                raise ValueError(f"SET_INSTRUMENT takes 1 arg, got {len(args)}")
            out.append(opcodes.SET_INSTRUMENT)
            out += list(vlq.encode(args[0]))
            continue
        if mnem == "NOTE":
            if len(args) != 2:
                raise ValueError(f"NOTE takes pitch + length, got {args}")
            pitch, length = args
            out.append(pitch)
            out += list(vlq.encode(length))
            continue
        if mnem in opcodes.MNEMONIC_TO_OPCODE:
            op = opcodes.MNEMONIC_TO_OPCODE[mnem]
            expected_argc = opcodes.OPCODE_ARG_COUNT.get(op, len(args))
            if len(args) != expected_argc:
                raise ValueError(f"{mnem} expects {expected_argc} args, got {len(args)}")
            out.append(op)
            out += args
            continue
        raise ValueError(f"unknown mnemonic: {mnem}")
    return _pad_to_4(out)


def parse(text: str) -> Tuple[List[SfxEntry], List[int]]:
    """Parse DSL text. Returns (entries in declaration order, priorities in declaration order)."""
    entries: List[SfxEntry] = []
    priorities: List[int] = []
    in_priorities = False
    in_opcode_block = False
    current_label: str | None = None
    current_opcodes: List[Tuple[str, List[int]]] = []

    def _flush_opcode_block():
        nonlocal in_opcode_block, current_label, current_opcodes
        if current_label is not None:
            entries.append(SfxEntry(label=current_label,
                                    sfx_bytes=_emit_opcode_block_bytes(current_opcodes)))
        in_opcode_block = False
        current_label = None
        current_opcodes = []

    for raw_line in text.splitlines():
        line = raw_line.rstrip()
        stripped = line.lstrip()
        if not stripped or stripped.startswith(";") or stripped.startswith("#include"):
            continue

        if in_priorities:
            if _PRIORITIES_CLOSE_RE.match(line):
                in_priorities = False
                continue
            m = _PRIORITIES_LINE_RE.match(line)
            if m:
                priorities.append(int(m.group("value")))
                continue
            raise ValueError(f"unexpected line in priorities block: {line!r}")

        if _PRIORITIES_OPEN_RE.match(line):
            _flush_opcode_block()
            in_priorities = True
            continue

        if in_opcode_block:
            m = _OPCODE_LINE_RE.match(line)
            if m:
                current_opcodes.append((m.group("mnem"), _parse_args(m.group("args"))))
                if m.group("mnem") == "END":
                    _flush_opcode_block()
                continue
            _flush_opcode_block()

        m = _SAMPLE_RE.match(line)
        if m:
            entries.append(SfxEntry(label=m.group("label"),
                                    sfx_bytes=[0x80, 0x00, 0x00, 0x00]))
            continue

        m = _SHORTHAND_RE.match(line)
        if m:
            instr = int(m.group("instr"), 16)
            vol = int(m.group("vol"), 16)
            note = int(m.group("note"), 16)
            length = int(m.group("length"), 16)
            entries.append(SfxEntry(label=m.group("label"),
                                    sfx_bytes=_emit_shorthand_bytes(instr, vol, note, length)))
            continue

        m = _LABEL_ONLY_RE.match(line)
        if m:
            current_label = m.group("label")
            current_opcodes = []
            in_opcode_block = True
            continue

        raise ValueError(f"unrecognized line: {line!r}")

    _flush_opcode_block()
    return entries, priorities
