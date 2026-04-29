"""SFX byte-stream -> DSL disassembler.

Detects the canonical 5-opcode pattern and emits shorthand:
    .NAME:        play instr=... vol=... note=... length=...
Detects the sample-dispatch placeholder ([0x80, 0x00, 0x00, 0x00]) and emits:
    .NAME:        sample
The audible sound for `sample` entries comes from setSfx(index+1) in
gameAudio.c (sample-bank playback), not from executing the byte stream.
Falls back to opcode-block form for anything else.

"""
import re
import sys
from pathlib import Path

from libhm64.audio.sfx import opcodes, vlq

REPO_ROOT = Path(__file__).resolve().parents[3]
SFX_C = REPO_ROOT / "src" / "data" / "audio" / "sfx.c"
SFXS_H = REPO_ROOT / "src" / "assetIndices" / "sfxs.h"
SFX_DSL_OUT = REPO_ROOT / "src" / "bytecode" / "sfx" / "sfx.sfx"

SFX_ARRAY_RE = re.compile(
    r"char\s+sfx(\d+)\s*\[\s*\]\s*=\s*\{([^}]*)\}\s*;",
    re.MULTILINE,
)
PRIORITY_BLOCK_RE = re.compile(
    r"int\s+musPriorityList\s*\[\s*\]\s*=\s*\{([^}]*)\}\s*;",
    re.MULTILINE,
)
SFXS_H_DEFINE_RE = re.compile(r"#define\s+(\w+)\s+(\d+)")

LABEL_COL = 18


def _parse_byte_list(s: str) -> list[int]:
    out = []
    for tok in s.split(","):
        tok = tok.strip()
        if not tok:
            continue
        out.append(int(tok, 0))
    return out


def load_name_lookup() -> dict[int, str]:
    """Read sfxs.h, return {index: name} for the named SFX. First definition wins."""
    text = SFXS_H.read_text()
    lookup: dict[int, str] = {}
    for m in SFXS_H_DEFINE_RE.finditer(text):
        name, idx_str = m.group(1), m.group(2)
        idx = int(idx_str)
        if idx not in lookup:
            lookup[idx] = name
    return lookup


def label_for(idx: int, name_lookup: dict[int, str]) -> str:
    """Return the label form .NAME: or .SFX_<idx>: for a given index."""
    if idx in name_lookup:
        return f".{name_lookup[idx]}:"
    return f".SFX_{idx}:"


def _format_label(label: str) -> str:
    """Pad label (incl trailing colon) to LABEL_COL columns."""
    if len(label) >= LABEL_COL:
        return label + " "
    return label + " " * (LABEL_COL - len(label))


def _try_parse_shorthand(b: bytes) -> tuple[str, str, str, str] | None:
    """Try to match the canonical 5-opcode pattern.

    Returns (instr, vol, note, length) strings or None.
    Shorthand requires: SET_INSTRUMENT(VLQ), ADSR_SET(canonical 7 bytes),
    PAN(0x7F), NOTE_VOL(any), NOTE(pitch + 1-byte length), END, then zero padding.
    """
    pos = 0
    if pos >= len(b) or b[pos] != opcodes.SET_INSTRUMENT:
        return None
    pos += 1
    if pos >= len(b):
        return None
    try:
        consumed, prog = vlq.decode(b, pos)
    except ValueError:
        return None
    instr_str = f"0x{prog:02X}"
    pos += consumed

    if pos >= len(b) or b[pos] != opcodes.ADSR_SET:
        return None
    pos += 1
    if pos + len(opcodes.CANONICAL_ADSR) > len(b):
        return None
    if bytes(b[pos:pos + len(opcodes.CANONICAL_ADSR)]) != opcodes.CANONICAL_ADSR:
        return None
    pos += len(opcodes.CANONICAL_ADSR)

    if pos >= len(b) or b[pos] != opcodes.PAN:
        return None
    pos += 1
    if pos >= len(b):
        return None
    pan = b[pos]
    pos += 1

    if pos >= len(b) or b[pos] != opcodes.NOTE_VOL:
        return None
    pos += 1
    if pos >= len(b):
        return None
    vol = b[pos]
    pos += 1

    if pos >= len(b) or b[pos] >= 0x80:
        return None
    note = b[pos]
    pos += 1
    if pos >= len(b) or b[pos] >= 0x80:
        # 2-byte length -> not shorthand-expressible.
        return None
    length = b[pos]
    pos += 1

    if pos >= len(b) or b[pos] != opcodes.END_OF_TRACK:
        return None
    pos += 1
    if any(x != 0 for x in b[pos:]):
        return None

    # Pan is omitted from shorthand; non-default pan forces opcode form.
    if pan != 0x7F:
        return None

    return instr_str, f"0x{vol:02X}", f"0x{note:02X}", f"0x{length:02X}"


def _is_sample(b: bytes) -> bool:
    """Sample-dispatch placeholder: a no-op micro-sequence whose audible
    sound is produced by setSfx(index+1) (see gameAudio.c:playSfx)."""
    return bytes(b) == bytes([0x80, 0x00, 0x00, 0x00])


def _disassemble_opcode_form(b: bytes) -> list[str]:
    """Walk byte-by-byte and emit one mnemonic line per opcode. Used for outliers."""
    lines: list[str] = []
    pos = 0
    while pos < len(b):
        op = b[pos]
        if op == opcodes.END_OF_TRACK:
            lines.append("    END")
            pos += 1
            tail = b[pos:]
            if tail and any(x != 0 for x in tail):
                lines.append(f"    ; trailing non-zero bytes: {' '.join(f'0x{x:02X}' for x in tail)}")
            return lines
        if op == opcodes.SET_INSTRUMENT:
            consumed, prog = vlq.decode(b, pos + 1)
            lines.append(f"    SET_INSTRUMENT  0x{prog:02X}")
            pos += 1 + consumed
            continue
        if op in opcodes.OPCODE_TO_MNEMONIC:
            mnem = opcodes.OPCODE_TO_MNEMONIC[op]
            argc = opcodes.OPCODE_ARG_COUNT[op]
            args = b[pos+1:pos+1+argc]
            args_str = ", ".join(f"0x{x:02X}" for x in args)
            lines.append(f"    {mnem:<15} {args_str}")
            pos += 1 + argc
            continue
        if op < 0x80:
            pitch = op
            consumed, length = vlq.decode(b, pos + 1)
            lines.append(f"    NOTE            0x{pitch:02X}, 0x{length:02X}")
            pos += 1 + consumed
            continue
        raise ValueError(f"unknown opcode 0x{op:02X} at offset {pos} in {b.hex()}")
    return lines


def disassemble_one(b: bytes, label: str) -> str:
    """Disassemble a single SFX byte sequence into one or more DSL lines."""
    if _is_sample(b):
        return f"{_format_label(label)}sample"
    short = _try_parse_shorthand(b)
    if short is not None:
        instr, vol, note, length = short
        return f"{_format_label(label)}play instr={instr} vol={vol} note={note} length={length}"
    body = _disassemble_opcode_form(b)
    return "\n".join([label] + body)


def disassemble_full(
    sfx_arrays: dict[int, list[int]],
    priorities: list[int],
    name_lookup: dict[int, str],
    total_count: int,
) -> str:
    """Build the complete sfx.sfx file text."""
    out: list[str] = []
    out.append("; SFX bank -- corresponds to src/data/audio/sfx.c")
    out.append("; Indices and names match src/assetIndices/sfxs.h where defined.")
    out.append("; Generated by tools/libhm64/audio/sfx/disassembler.py.")
    out.append("")
    out.append('#include "sfxs.h"')
    out.append("")
    for idx in range(total_count):
        if idx not in sfx_arrays:
            raise ValueError(f"missing sfx index {idx}")
        b = bytes(sfx_arrays[idx])
        label = label_for(idx, name_lookup)
        out.append(disassemble_one(b, label))
    out.append("")
    out.append("priorities {")
    for idx in range(total_count):
        bare = label_for(idx, name_lookup).strip(".:")
        pad = max(1, LABEL_COL - len(bare) - 2)
        out.append(f"    .{bare}:{' ' * pad}{priorities[idx]}")
    out.append("}")
    out.append("")
    return "\n".join(out)


def main(argv: list[str]) -> int:
    """CLI: regenerate src/bytecode/sfxs/sfx.sfx from src/data/audio/sfx.c."""
    text = SFX_C.read_text()
    sfx_arrays: dict[int, list[int]] = {}
    for m in SFX_ARRAY_RE.finditer(text):
        sfx_arrays[int(m.group(1))] = _parse_byte_list(m.group(2))
    pm = PRIORITY_BLOCK_RE.search(text)
    if not pm:
        print("ERROR: no musPriorityList[] found", file=sys.stderr)
        return 1
    priorities = _parse_byte_list(pm.group(1))
    name_lookup = load_name_lookup()
    total = max(sfx_arrays) + 1
    out_text = disassemble_full(sfx_arrays, priorities, name_lookup, total)
    SFX_DSL_OUT.parent.mkdir(parents=True, exist_ok=True)
    SFX_DSL_OUT.write_text(out_text)
    print(f"wrote {SFX_DSL_OUT} ({len(out_text)} bytes, {total} sfx)")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
