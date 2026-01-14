"""
HM64 Cutscene Bytecode Extractor.

Extracts cutscene bytecode from ROM and generates GNU assembly (.s) files.

Usage:
    python -m hm64.cutscenes.extractor --all
    python -m hm64.cutscenes.extractor --bank <name>
    python -m hm64.cutscenes.extractor --offset <hex> --length <hex>
"""

import argparse
import csv
import struct
import sys
from collections import deque
from dataclasses import dataclass, field
from pathlib import Path
from typing import Dict, List, Optional, Set, Tuple

from ..common import rom
from ..common.rom import RomReader
from ..data import (
    CUTSCENE_ADDRESSES_CSV, CUTSCENE_ENTRY_POINTS_CSV,
    ROM_SEGMENT_START_SYMBOLS, ROM_SEGMENT_END_SYMBOLS
)
from .opcodes import ASM_COMMAND_SPECS, COMMAND_SPECS


# =============================================================================
# DATA CLASSES
# =============================================================================

@dataclass
class ParsedCommand:
    """Represents a parsed cutscene command"""
    offset: int
    opcode: int
    size: int
    name: str
    params: List[Tuple[str, any]]
    raw_bytes: bytes
    spawn_target: Optional[Tuple[int, int]] = None
    branch_target: Optional[int] = None
    anim_target: Optional[int] = None


@dataclass
class AnimationEntry:
    """Represents one frame or control code in an animation sequence"""
    offset: int
    raw_bytes: bytes
    is_control: bool = False
    is_terminator: bool = False
    anim_id: Optional[int] = None
    flag: Optional[int] = None
    vec: Optional[Tuple[int, int, int]] = None
    wait_frames: Optional[int] = None
    flip_flags: Optional[int] = None
    loop_target: Optional[int] = None


@dataclass
class AnimationBlock:
    """Represents a sequence of animation data"""
    start_offset: int
    entries: List[AnimationEntry] = field(default_factory=list)


# =============================================================================
# BYTECODE PARSER
# =============================================================================

class CutsceneBytecodeParser:
    """Parser for HM64 cutscene bytecode"""

    def __init__(self, data: bytes, base_offset: int = 0):
        self.data = data
        self.base_offset = base_offset
        self.reader = RomReader(data)

    @property
    def pos(self) -> int:
        return self.reader.pos

    @pos.setter
    def pos(self, value: int):
        self.reader.pos = value

    def seek(self, offset: int):
        """Seek to absolute offset"""
        self.reader.pos = offset - self.base_offset

    def tell(self) -> int:
        """Get current absolute offset"""
        return self.base_offset + self.reader.pos

    def parse_command(self) -> Optional[ParsedCommand]:
        """Parse a single command at current position"""
        if self.pos + 2 > len(self.data):
            return None

        start_pos = self.pos
        start_offset = self.tell()
        opcode = self.reader.read_u16()

        if opcode not in COMMAND_SPECS:
            self.pos = start_pos
            return None

        cmd_name, cmd_size, param_specs = COMMAND_SPECS[opcode]
        params = []
        spawn_target = None
        branch_target = None
        anim_target = None

        for pname, ptype in param_specs:
            if ptype == 'pad8':
                self.reader.read_u8()
            elif ptype == 'pad16':
                self.reader.read_u16()
            elif ptype == 'u8':
                params.append((pname, self.reader.read_u8()))
            elif ptype == 's8':
                params.append((pname, self.reader.read_s8()))
            elif ptype == 'u16':
                params.append((pname, self.reader.read_u16()))
            elif ptype == 's16':
                params.append((pname, self.reader.read_s16()))
            elif ptype == 'u32':
                params.append((pname, self.reader.read_u32()))
            elif ptype == 's32':
                params.append((pname, self.reader.read_s32()))
            elif ptype == 'rel16':
                rel_pos = self.tell()
                rel_val = self.reader.read_s16()
                target = rel_pos + rel_val
                params.append((pname, target))
                # Track targets
                if opcode in (0, 1):  # SET_ANIM_DATA_PTR
                    anim_target = target
                elif opcode == 6:  # EXECUTE_SUBROUTINE
                    branch_target = target
                elif opcode == 11:  # SPAWN_EXECUTOR
                    # Get executor index from previous param
                    exec_idx = params[-2][1] if len(params) >= 2 else 0
                    spawn_target = (exec_idx, target)
                elif opcode in (8, 9, 10, 20, 21, 22, 25, 61, 64, 70, 79):
                    branch_target = target
            elif ptype in ('addr32', 'rom_start32', 'rom_end32'):
                params.append((pname, self.reader.read_u32()))

        raw_bytes = self.data[start_pos:self.pos]

        return ParsedCommand(
            offset=start_offset,
            opcode=opcode,
            size=len(raw_bytes),
            name=cmd_name,
            params=params,
            raw_bytes=raw_bytes,
            spawn_target=spawn_target,
            branch_target=branch_target,
            anim_target=anim_target,
        )

    def parse_animation_sequence(self, offset: int) -> AnimationBlock:
        """Parse animation data at given offset"""
        self.seek(offset)
        block = AnimationBlock(start_offset=offset)

        while self.pos + 2 <= len(self.data):
            entry_offset = self.tell()
            entry_start = self.pos

            anim_id = self.reader.read_u16()

            if anim_id == 0xFFFF:
                # ANIM_END
                if self.pos + 2 <= len(self.data):
                    self.reader.read_u16()  # Skip padding
                raw = self.data[entry_start:self.pos]
                block.entries.append(AnimationEntry(
                    offset=entry_offset, raw_bytes=raw,
                    is_terminator=True
                ))
                break

            elif anim_id == 0xFFFE:
                # ANIM_LOOP
                if self.pos + 2 <= len(self.data):
                    rel_pos = self.tell()
                    rel_val = self.reader.read_s16()
                    loop_target = rel_pos + rel_val
                    raw = self.data[entry_start:self.pos]
                    block.entries.append(AnimationEntry(
                        offset=entry_offset, raw_bytes=raw,
                        is_control=True, loop_target=loop_target
                    ))
                break

            else:
                # Regular frame (8 bytes total)
                if self.pos + 6 > len(self.data):
                    break
                flag = self.reader.read_u8()
                vec = (self.reader.read_s8(), self.reader.read_s8(), self.reader.read_s8())
                wait = self.reader.read_u8()
                flip = self.reader.read_u8()
                raw = self.data[entry_start:self.pos]
                block.entries.append(AnimationEntry(
                    offset=entry_offset, raw_bytes=raw,
                    anim_id=anim_id, flag=flag, vec=vec,
                    wait_frames=wait, flip_flags=flip
                ))

        return block

    def _try_parse_padding_or_data(self) -> Optional[ParsedCommand]:
        """Try to parse padding or unrecognized data"""
        if self.pos + 2 > len(self.data):
            return None

        start_pos = self.pos
        start_offset = self.tell()
        peek = struct.unpack_from('>H', self.data, self.pos)[0]

        # Check for invalid opcode
        if peek >= 98:
            # Check for ANIM_END or padded terminators
            if self.pos + 4 <= len(self.data):
                next_word = struct.unpack_from('>H', self.data, self.pos + 2)[0]
                if (peek == 0xFFFF and next_word == 0) or (peek == 0xFE00 and next_word == 0):
                    self.pos += 4
                    raw = self.data[start_pos:self.pos]
                    return ParsedCommand(
                        offset=start_offset, opcode=-2, size=4,
                        name="PADDING_4", params=[], raw_bytes=raw
                    )

            # Single padding word
            self.pos += 2
            raw = self.data[start_pos:self.pos]
            return ParsedCommand(
                offset=start_offset, opcode=-2, size=2,
                name="PADDING_2", params=[], raw_bytes=raw
            )

        return None

    def parse_linear_scan(self, start_offset: int, end_offset: int = None,
                          known_anim_offsets: Set[int] = None) -> List[ParsedCommand]:
        """Parse all commands linearly"""
        self.seek(start_offset)
        commands = []
        anim_targets = set(known_anim_offsets) if known_anim_offsets else set()

        max_pos = (end_offset - self.base_offset) if end_offset else len(self.data)

        # Pre-pass: discover animation targets
        save_pos = self.pos
        while self.pos < max_pos - 1:
            if self.pos + 2 > len(self.data):
                break
            peek = struct.unpack_from('>H', self.data, self.pos)[0]
            if peek >= 98:
                self.pos += 2
                continue
            cmd = self.parse_command()
            if cmd and cmd.anim_target:
                if self.base_offset <= cmd.anim_target < self.base_offset + len(self.data):
                    anim_targets.add(cmd.anim_target)
            elif not cmd:
                self.pos += 2

        # Follow animation loops to find more targets
        discovery_queue = deque(anim_targets)
        processed = set()
        while discovery_queue:
            addr = discovery_queue.popleft()
            if addr in processed:
                continue
            processed.add(addr)
            if addr < self.base_offset or addr >= self.base_offset + len(self.data):
                continue
            try:
                block = self.parse_animation_sequence(addr)
                for entry in block.entries:
                    if entry.is_control and entry.loop_target:
                        if entry.loop_target not in anim_targets:
                            if self.base_offset <= entry.loop_target < self.base_offset + len(self.data):
                                anim_targets.add(entry.loop_target)
                                discovery_queue.append(entry.loop_target)
            except Exception:
                pass

        # Main pass
        self.pos = save_pos
        self.seek(start_offset)

        while self.pos < max_pos - 1:
            curr_offset = self.tell()

            # Animation data
            if curr_offset in anim_targets:
                anim_block = self.parse_animation_sequence(curr_offset)
                for entry in anim_block.entries:
                    if entry.is_terminator:
                        name = "ANIM_END"
                    elif entry.is_control:
                        name = "ANIM_LOOP"
                    else:
                        name = "ANIM_FRAME"
                    commands.append(ParsedCommand(
                        offset=entry.offset, opcode=-1, size=len(entry.raw_bytes),
                        name=name, params=[], raw_bytes=entry.raw_bytes
                    ))
                continue

            # Padding/data
            padding_cmd = self._try_parse_padding_or_data()
            if padding_cmd:
                commands.append(padding_cmd)
                continue

            # Regular command
            cmd = self.parse_command()
            if cmd is None:
                if self.pos + 2 <= max_pos:
                    self.pos += 2
                else:
                    break
                continue

            if cmd.anim_target:
                anim_targets.add(cmd.anim_target)
            commands.append(cmd)

        return commands


# =============================================================================
# ASSEMBLY OUTPUT
# =============================================================================

def format_rom_address(addr: int, use_symbols: bool, is_end: bool = False) -> str:
    """Format a ROM address, optionally using symbolic names.

    Args:
        addr: The ROM address to format
        use_symbols: Whether to use symbolic names when available
        is_end: If True, look up in ROM_SEGMENT_END_SYMBOLS

    Returns:
        Formatted address string (symbol name or hex)
    """
    if use_symbols:
        symbols = ROM_SEGMENT_END_SYMBOLS if is_end else ROM_SEGMENT_START_SYMBOLS
        if addr in symbols:
            return symbols[addr]
    return f"0x{addr:08X}"


class AsmLabelManager:
    """Manages unique labels for assembly output"""

    def __init__(self):
        self.labels: Dict[int, str] = {}

    def get_or_create(self, offset: int, type_hint: str = "branch", index_hint: int = None) -> str:
        if offset in self.labels:
            return self.labels[offset]

        if type_hint == "sub":
            name = f"sub_{offset:X}"
        elif type_hint == "spawn":
            name = f"executor_{index_hint}_{offset:X}"
        elif type_hint == "anim":
            name = f"anim_{offset:X}"
        else:
            name = f"branch_{offset:X}"

        self.labels[offset] = name
        return name


def format_asm_linear(commands: List[ParsedCommand], bank_name: str, use_symbols: bool = True) -> str:
    """Format commands as GNU assembly.

    Args:
        commands: List of parsed commands to format
        bank_name: Name for the bank (used in labels)
        use_symbols: Use symbolic names for ROM addresses when available

    Returns:
        GNU assembly source as a string
    """
    lines = []

    # Header
    lines.append(f"# {bank_name} - Cutscene Data")
    lines.append("# Auto-generated by libhm64.cutscenes.extractor")
    lines.append("")
    lines.append(".section .data")
    lines.append(".balign 4")
    lines.append(f".global {bank_name}_start")
    lines.append(f"{bank_name}_start:")
    lines.append("")

    label_mgr = AsmLabelManager()

    # Pass 1: Collect labels
    for cmd in commands:
        if cmd.spawn_target:
            label_mgr.get_or_create(cmd.spawn_target[1], "spawn", cmd.spawn_target[0])
        if cmd.branch_target is not None:
            hint = "sub" if cmd.opcode == 6 else "branch"
            label_mgr.get_or_create(cmd.branch_target, hint)
        if cmd.anim_target is not None:
            label_mgr.get_or_create(cmd.anim_target, "anim")
        # ANIM_LOOP targets
        if cmd.opcode == -1 and cmd.name == "ANIM_LOOP":
            if len(cmd.raw_bytes) >= 4:
                val = struct.unpack_from('>h', cmd.raw_bytes, 2)[0]
                target = cmd.offset + 2 + val
                label_mgr.get_or_create(target, "anim")

    # Pass 2: Emit
    for cmd in commands:
        # Label
        if cmd.offset in label_mgr.labels:
            lines.append(f"{label_mgr.labels[cmd.offset]}:")

        # Animation pseudo-commands
        if cmd.opcode == -1:
            if cmd.name == "ANIM_END":
                lines.append("    .half   0xFFFF")
                lines.append("    .half   0")
            elif cmd.name == "ANIM_LOOP":
                lines.append("    .half   0xFFFE")
                val = struct.unpack_from('>h', cmd.raw_bytes, 2)[0]
                lines.append(f"    .half   {val}")
            else:  # ANIM_FRAME
                if len(cmd.raw_bytes) == 8:
                    anim_id = struct.unpack_from('>H', cmd.raw_bytes, 0)[0]
                    flag = cmd.raw_bytes[2]
                    vec = struct.unpack_from('bbb', cmd.raw_bytes, 3)
                    wait = cmd.raw_bytes[6]
                    flip = cmd.raw_bytes[7]
                    lines.append(f"    .half   0x{anim_id:04X}")
                    lines.append(f"    .byte   0x{flag:02X}")
                    lines.append(f"    .byte   {vec[0]}, {vec[1]}, {vec[2]}")
                    lines.append(f"    .byte   {wait}")
                    lines.append(f"    .byte   0x{flip:02X}")
                else:
                    byte_str = ", ".join([f"0x{b:02X}" for b in cmd.raw_bytes])
                    lines.append(f"    .byte   {byte_str}")

        # Padding/data
        elif cmd.name.startswith("PADDING") or cmd.name.startswith("DATA"):
            byte_str = ", ".join([f"0x{b:02X}" for b in cmd.raw_bytes])
            lines.append(f"    .byte   {byte_str}")

        # Regular commands
        else:
            comment = f"  # {cmd.name}"
            lines.append(f"    .half   0x{cmd.opcode:04X}{comment}")

            specs = ASM_COMMAND_SPECS.get(cmd.opcode, (None, []))[1]
            curr_byte_pos = 2

            for spec_type in specs:
                if spec_type == 'pad8':
                    val = cmd.raw_bytes[curr_byte_pos]
                    lines.append(f"    .byte   0x{val:02X}")
                    curr_byte_pos += 1
                elif spec_type == 'pad16':
                    val = struct.unpack_from('>H', cmd.raw_bytes, curr_byte_pos)[0]
                    lines.append(f"    .half   0x{val:04X}")
                    curr_byte_pos += 2
                elif spec_type in ('u8', 's8'):
                    val = cmd.raw_bytes[curr_byte_pos]
                    lines.append(f"    .byte   0x{val:02X}")
                    curr_byte_pos += 1
                elif spec_type in ('u16', 's16'):
                    val = struct.unpack_from('>H', cmd.raw_bytes, curr_byte_pos)[0]
                    lines.append(f"    .half   0x{val:04X}")
                    curr_byte_pos += 2
                elif spec_type in ('u32', 's32'):
                    val = struct.unpack_from('>I', cmd.raw_bytes, curr_byte_pos)[0]
                    lines.append(f"    .word   0x{val:08X}")
                    curr_byte_pos += 4
                elif spec_type == 'rel16':
                    raw_offset = struct.unpack_from('>h', cmd.raw_bytes, curr_byte_pos)[0]
                    lines.append(f"    .half   {raw_offset}")
                    curr_byte_pos += 2
                elif spec_type in ('addr32', 'rom_start32', 'rom_end32'):
                    val = struct.unpack_from('>I', cmd.raw_bytes, curr_byte_pos)[0]
                    is_end = (spec_type == 'rom_end32')
                    formatted = format_rom_address(val, use_symbols, is_end)
                    lines.append(f"    .word   {formatted}")
                    curr_byte_pos += 4

    lines.append("")
    lines.append(f".global {bank_name}_end")
    lines.append(f"{bank_name}_end:")

    return "\n".join(lines)


# =============================================================================
# CSV LOADING
# =============================================================================

def load_cutscene_addresses(csv_path: Path) -> List[Dict]:
    """Load cutscene bank addresses from CSV"""
    banks = []
    if not csv_path.exists():
        return banks

    with open(csv_path, 'r', newline='', encoding='utf-8') as f:
        reader = csv.reader(f)
        for row in reader:
            if len(row) < 3:
                continue
            try:
                banks.append({
                    'start': int(row[0], 16),
                    'end': int(row[1], 16),
                    'name': row[2].strip() if len(row) > 2 else f"bank_{len(banks)}",
                })
            except ValueError:
                continue

    return banks


def load_entry_points(csv_path: Path) -> Dict[str, List[Dict]]:
    """Load entry points from CSV"""
    entry_points = {}
    if not csv_path.exists():
        return entry_points

    with open(csv_path, 'r', newline='', encoding='utf-8') as f:
        reader = csv.reader(f)
        for row in reader:
            if len(row) < 4:
                continue
            try:
                bank_name = row[0].strip()
                entry = {
                    'offset': int(row[1], 16),
                    'array_index': int(row[2]),
                    'executor_index': int(row[3], 16) if row[3].startswith('0x') else int(row[3]),
                }
                if bank_name not in entry_points:
                    entry_points[bank_name] = []
                entry_points[bank_name].append(entry)
            except ValueError:
                continue

    return entry_points


# =============================================================================
# MAIN
# =============================================================================

def extract_bank(bank: Dict, output_dir: Path, known_anims: Set[int] = None,
                 use_symbols: bool = True) -> bool:
    """Extract a single cutscene bank to assembly.

    Args:
        bank: Bank info dict with 'start', 'end', 'name' keys
        output_dir: Directory to write output files
        known_anims: Optional set of known animation offsets
        use_symbols: Use symbolic names for ROM addresses

    Returns:
        True if extraction succeeded
    """
    print(f"  Extracting: {bank['name']}")

    try:
        rom_data = rom.get_rom()
        data = rom_data[bank['start']:bank['end']]

        parser = CutsceneBytecodeParser(data, base_offset=bank['start'])
        commands = parser.parse_linear_scan(bank['start'], bank['end'], known_anims)

        output = format_asm_linear(commands, bank['name'], use_symbols)

        output_path = output_dir / f"{bank['name']}.s"
        with open(output_path, 'w', encoding='utf-8') as f:
            f.write(output)
            f.write('\n')

        print(f"    Wrote {output_path.name} ({len(commands)} commands)")
        return True

    except Exception as e:
        print(f"    Error: {e}")
        import traceback
        traceback.print_exc()
        return False


def extract_all(output_dir: Path, use_symbols: bool = True):
    """Extract all cutscene banks."""
    output_dir.mkdir(parents=True, exist_ok=True)

    banks = load_cutscene_addresses(CUTSCENE_ADDRESSES_CSV)
    if not banks:
        print(f"No banks found in {CUTSCENE_ADDRESSES_CSV}")
        return

    print(f"Found {len(banks)} cutscene banks")

    success = 0
    for bank in banks:
        if extract_bank(bank, output_dir, use_symbols=use_symbols):
            success += 1

    print(f"\nExtracted {success}/{len(banks)} banks")


def extract_one(name: str, output_dir: Path, use_symbols: bool = True):
    """Extract a single bank by name."""
    output_dir.mkdir(parents=True, exist_ok=True)

    banks = load_cutscene_addresses(CUTSCENE_ADDRESSES_CSV)
    bank = next((b for b in banks if b['name'] == name), None)

    if bank is None:
        print(f"Bank not found: {name}")
        print("Available banks:")
        for b in banks:
            print(f"  - {b['name']}")
        return

    extract_bank(bank, output_dir, use_symbols=use_symbols)


def list_banks():
    """List all available cutscene banks"""
    banks = load_cutscene_addresses(CUTSCENE_ADDRESSES_CSV)
    if not banks:
        print(f"No banks found in {CUTSCENE_ADDRESSES_CSV}")
        return

    print(f"Cutscene banks ({len(banks)} total):")
    for i, bank in enumerate(banks):
        size = bank['end'] - bank['start']
        print(f"  {i+1:2d}. {bank['name']:30s} 0x{bank['start']:06X}-0x{bank['end']:06X} ({size} bytes)")


def main():
    parser = argparse.ArgumentParser(
        description='Extract HM64 cutscene bytecode to GNU assembly'
    )

    source_group = parser.add_mutually_exclusive_group(required=True)
    source_group.add_argument('--all', '-a', action='store_true',
                              help='Extract all banks from CSV')
    source_group.add_argument('--bank', '-b', type=str,
                              help='Extract a single bank by name')
    source_group.add_argument('--list', '-l', action='store_true',
                              help='List available banks')
    source_group.add_argument('--offset', '-o', type=lambda x: int(x, 0),
                              help='Extract from offset (requires --length)')

    parser.add_argument('--length', type=lambda x: int(x, 0),
                        help='Length in bytes (required with --offset)')
    parser.add_argument('--output-dir', '-d', type=str, default='assets/cutscenes',
                        help='Output directory')
    parser.add_argument('--name', '-n', type=str, default='cutscene_bank',
                        help='Bank name (for --offset mode)')
    parser.add_argument('--symbols', dest='use_symbols', action='store_true', default=True,
                        help='Use symbolic names for ROM addresses (default)')
    parser.add_argument('--no-symbols', dest='use_symbols', action='store_false',
                        help='Use raw hex for ROM addresses')

    args = parser.parse_args()

    if args.list:
        list_banks()
        return

    output_dir = Path(args.output_dir)

    if args.all:
        extract_all(output_dir, use_symbols=args.use_symbols)
    elif args.bank:
        extract_one(args.bank, output_dir, use_symbols=args.use_symbols)
    elif args.offset is not None:
        if args.length is None:
            print("Error: --length required with --offset")
            sys.exit(1)
        output_dir.mkdir(parents=True, exist_ok=True)
        bank = {
            'start': args.offset,
            'end': args.offset + args.length,
            'name': args.name,
        }
        extract_bank(bank, output_dir, use_symbols=args.use_symbols)


if __name__ == '__main__':
    main()
