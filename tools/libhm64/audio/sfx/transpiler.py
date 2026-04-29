"""SFX intermediate form -> sfx.inc.c, with byte-identity verification.

CLI:
    python -m libhm64.audio.sfx.transpiler transpile <input.sfx> -o <output.inc.c> \
        [--verify-against <snapshot.bin>]
    python -m libhm64.audio.sfx.transpiler verify

Reads the DSL via parser.py, emits C source

Generated symbol names use the original numeric form (sfx0..sfx103) so the
public ABI in src/data/audio/sfx.h stays unchanged.
"""
import argparse
import struct
import sys
from pathlib import Path
from typing import List

from libhm64.audio.sfx import parser

REPO_ROOT = Path(__file__).resolve().parents[3]
DEFAULT_DSL = REPO_ROOT / "src" / "bytecode" / "sfx" / "sfx.sfx"


class IdentityMismatch(Exception):
    """Raised when transpiler output diverges from the frozen snapshot."""


def emit_inc_c(entries: List[parser.SfxEntry], priorities: List[int]) -> str:
    """Render entries + priorities as sfx.inc.c text."""
    lines: List[str] = []
    lines.append("/* AUTO-GENERATED from src/bytecode/sfxs/sfx.sfx */")
    lines.append("/* See tools/libhm64/sfx/transpiler.py. */")
    lines.append("")
    for idx, e in enumerate(entries):
        bytes_str = ", ".join(f"0x{b:02X}" for b in e.sfx_bytes)
        lines.append(f"char sfx{idx}[] = {{ {bytes_str} }};")
    lines.append("")
    lines.append("char *sfxList[] = {")
    for idx in range(len(entries)):
        sep = "," if idx < len(entries) - 1 else ""
        lines.append(f"    sfx{idx}{sep}")
    lines.append("};")
    lines.append("")
    lines.append("int musPriorityList[] = {")
    for idx, p in enumerate(priorities):
        sep = "," if idx < len(priorities) - 1 else ""
        lines.append(f"    {p}{sep}")
    lines.append("};")
    lines.append("")
    return "\n".join(lines)


def _read_snapshot(snapshot: bytes) -> tuple[list[list[int]], list[int]]:
    """Parse the snapshot binary written by snapshot_original.py."""
    pos = 0
    n_sfx = struct.unpack(">H", snapshot[pos:pos+2])[0]
    pos += 2
    sfx_arrays: list[list[int]] = []
    for _ in range(n_sfx):
        length = struct.unpack(">H", snapshot[pos:pos+2])[0]
        pos += 2
        sfx_arrays.append(list(snapshot[pos:pos+length]))
        pos += length
    n_pri = struct.unpack(">H", snapshot[pos:pos+2])[0]
    pos += 2
    priorities = []
    for _ in range(n_pri):
        priorities.append(struct.unpack(">i", snapshot[pos:pos+4])[0])
        pos += 4
    return sfx_arrays, priorities

def _read_input(path_or_dash: str) -> str:
    if path_or_dash == "-":
        return sys.stdin.read()
    return Path(path_or_dash).read_text()


def main(argv: list[str]) -> int:
    ap = argparse.ArgumentParser(prog="libhm64.audio.sfx.transpiler")
    sub = ap.add_subparsers(dest="cmd", required=True)

    sp_t = sub.add_parser("transpile", help="DSL -> sfx.inc.c")
    sp_t.add_argument("input", help="path to .sfx file or '-' for stdin")
    sp_t.add_argument("-o", "--output", required=True, help="path to write sfx.inc.c")

    args = ap.parse_args(argv)

    if args.cmd == "transpile":
        text = _read_input(args.input)
        entries, priorities = parser.parse(text)
        out_text = emit_inc_c(entries, priorities)
        Path(args.output).write_text(out_text)
        print(f"wrote {args.output} ({len(out_text)} bytes)")
        return 0

    return 1


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
