#!/usr/bin/env python3
"""
HM64 Cutscene ASM to DSL Converter

Converts cutscene assembly files (.s) generated from bytecode back into
DSL format compatible with hm64_cutscene_transpiler.py.

Usage:
    python hm64_asm_to_dsl.py input.s -o output.cutscene
    python hm64_asm_to_dsl.py input.s  # prints to stdout
"""

import re
import sys
import argparse
from dataclasses import dataclass, field
from typing import List, Dict, Optional, Tuple, Union, Set
from pathlib import Path
from enum import IntEnum


# =============================================================================
# COMMAND SPECIFICATIONS - imported from shared utilities
# =============================================================================

from hm64_cutscene_utilities import ASM_COMMAND_SPECS

# Local variable buffer ranges (two DMA buffers used by cutscene system)
# Buffer 1: 0x802F4000 (farmBusiness uses this)
# Buffer 2: 0x802F5000 (most other banks use this)
BUFFER_BASES = {
    1: (0x802F4000, 0x802F4100),  # Buffer 1 range
    2: (0x802F5000, 0x802F5100),  # Buffer 2 range
}

def is_local_var_addr(addr: int) -> tuple:
    """Check if address is in a local variable buffer.
    Returns (buffer_num, offset) if yes, (None, None) if no."""
    for buf_num, (base, end) in BUFFER_BASES.items():
        if base <= addr < end:
            return (buf_num, addr - base)
    return (None, None)


# =============================================================================
# DATA CLASSES
# =============================================================================

@dataclass
class ParsedValue:
    """A parsed value from ASM - either numeric or symbolic"""
    value: int  # Numeric value (0 if symbolic)
    symbol: Optional[str] = None  # Symbol name if present
    is_symbol: bool = False

    def __repr__(self):
        if self.is_symbol:
            return f"Symbol({self.symbol})"
        return f"Value({self.value})"


@dataclass
class ByteData:
    """Raw byte data with offset tracking"""
    offset: int
    size: int  # 1, 2, or 4 bytes
    value: ParsedValue
    label: Optional[str] = None


@dataclass
class DecodedCommand:
    """A decoded cutscene command"""
    offset: int
    opcode: int
    name: str
    params: List[str]
    label: Optional[str] = None
    size: int = 0


@dataclass
class AnimFrame:
    """Animation frame data"""
    offset: int
    anim_id: int
    flag: int
    vec: Tuple[int, int, int]
    wait: int
    flip: int
    label: Optional[str] = None


@dataclass
class AnimEnd:
    """Animation end marker"""
    offset: int
    label: Optional[str] = None


@dataclass
class AnimLoop:
    """Animation loop marker"""
    offset: int
    target_label: str
    label: Optional[str] = None


# Type alias for decoded items
DecodedItem = Union[DecodedCommand, AnimFrame, AnimEnd, AnimLoop]


# =============================================================================
# ASM PARSER
# =============================================================================

class AsmParser:
    """Parses GNU as assembly files into structured data"""

    def __init__(self):
        self.bytes_data: List[ByteData] = []
        self.labels: Dict[str, int] = {}  # label -> offset
        self.offset_to_label: Dict[int, str] = {}  # offset -> label
        self.current_offset = 0
        self.in_data_section = False
        self.global_symbol: Optional[str] = None

    def parse_file(self, path: Path) -> Tuple[List[ByteData], Dict[str, int], Dict[int, str]]:
        """Parse an assembly file and return byte data and label mappings"""

        with open(path, 'r') as f:
            lines = f.readlines()

        pending_label = None

        for line in lines:
            line = line.strip()

            # Skip empty lines and comments
            if not line or line.startswith('#') or line.startswith(';'):
                continue

            # Handle section directive
            if line.startswith('.section'):
                if '.data' in line:
                    self.in_data_section = True
                continue

            # Handle global directive
            if line.startswith('.global') or line.startswith('.globl'):
                match = re.match(r'\.globa?l\s+(\w+)', line)
                if match:
                    self.global_symbol = match.group(1)
                continue

            # Handle alignment
            if line.startswith('.balign') or line.startswith('.align'):
                # Align current offset
                match = re.match(r'\.b?align\s+(\d+)', line)
                if match:
                    alignment = int(match.group(1))
                    if self.current_offset % alignment != 0:
                        self.current_offset += alignment - (self.current_offset % alignment)
                continue

            # Handle labels
            if ':' in line and not line.startswith('.'):
                # Check if this is a label definition
                label_match = re.match(r'^(\w+):', line)
                if label_match:
                    label = label_match.group(1)
                    self.labels[label] = self.current_offset
                    self.offset_to_label[self.current_offset] = label
                    pending_label = label
                    # Check if there's more on the line after the label
                    rest = line[label_match.end():].strip()
                    if rest:
                        line = rest
                    else:
                        continue

            # Parse data directives
            if line.startswith('.half') or line.startswith('.hword') or line.startswith('.short'):
                self._parse_half(line, pending_label)
                pending_label = None
            elif line.startswith('.byte'):
                self._parse_byte(line, pending_label)
                pending_label = None
            elif line.startswith('.word') or line.startswith('.long'):
                self._parse_word(line, pending_label)
                pending_label = None

        return self.bytes_data, self.labels, self.offset_to_label

    def _parse_value(self, val_str: str) -> ParsedValue:
        """Parse a value string into ParsedValue"""
        val_str = val_str.strip()

        # Remove any trailing comment
        if '#' in val_str:
            val_str = val_str[:val_str.index('#')].strip()

        # Check if it's a symbol (starts with letter or underscore, not a number)
        if val_str and not val_str[0].isdigit() and val_str[0] != '-':
            # It's a symbol
            return ParsedValue(0, val_str, is_symbol=True)

        # Parse as number
        try:
            if val_str.startswith('0x') or val_str.startswith('0X'):
                return ParsedValue(int(val_str, 16))
            elif val_str.startswith('0b') or val_str.startswith('0B'):
                return ParsedValue(int(val_str, 2))
            else:
                return ParsedValue(int(val_str))
        except ValueError:
            # Might be a symbol we didn't recognize
            return ParsedValue(0, val_str, is_symbol=True)

    def _parse_half(self, line: str, label: Optional[str]):
        """Parse .half directive (16-bit values)"""
        # Extract values after directive
        match = re.match(r'\.(?:half|hword|short)\s+(.+?)(?:#.*)?$', line)
        if not match:
            return

        values_str = match.group(1)
        values = [v.strip() for v in values_str.split(',')]

        first = True
        for val in values:
            if not val:
                continue
            parsed = self._parse_value(val)
            self.bytes_data.append(ByteData(
                offset=self.current_offset,
                size=2,
                value=parsed,
                label=label if first else None
            ))
            self.current_offset += 2
            first = False

    def _parse_byte(self, line: str, label: Optional[str]):
        """Parse .byte directive (8-bit values)"""
        match = re.match(r'\.byte\s+(.+?)(?:#.*)?$', line)
        if not match:
            return

        values_str = match.group(1)
        values = [v.strip() for v in values_str.split(',')]

        first = True
        for val in values:
            if not val:
                continue
            parsed = self._parse_value(val)
            self.bytes_data.append(ByteData(
                offset=self.current_offset,
                size=1,
                value=parsed,
                label=label if first else None
            ))
            self.current_offset += 1
            first = False

    def _parse_word(self, line: str, label: Optional[str]):
        """Parse .word directive (32-bit values)"""
        match = re.match(r'\.(?:word|long)\s+(.+?)(?:#.*)?$', line)
        if not match:
            return

        values_str = match.group(1)
        values = [v.strip() for v in values_str.split(',')]

        first = True
        for val in values:
            if not val:
                continue
            parsed = self._parse_value(val)
            self.bytes_data.append(ByteData(
                offset=self.current_offset,
                size=4,
                value=parsed,
                label=label if first else None
            ))
            self.current_offset += 4
            first = False


# =============================================================================
# COMMAND DECODER
# =============================================================================

class CommandDecoder:
    """Decodes byte data into commands and animation frames"""

    def __init__(self, bytes_data: List[ByteData], labels: Dict[str, int],
                 offset_to_label: Dict[int, str]):
        self.bytes_data = bytes_data
        self.labels = labels
        self.offset_to_label = offset_to_label
        self.idx = 0
        self.decoded: List[DecodedItem] = []
        self.local_vars: Dict[int, str] = {}  # address -> var name
        self.local_var_counter = 0
        self.animation_targets: Set[int] = set()  # Offsets that are animation data
        self.animation_byte_ranges: Set[int] = set()  # Byte offsets that are animation data
        self.detected_buffer: Optional[int] = None  # Which buffer (1 or 2) is used

    def decode(self) -> Tuple[List[DecodedItem], Dict[int, str], Optional[int]]:
        """Decode all bytes into commands. Returns (decoded, local_vars, detected_buffer)"""

        # First pass: decode commands to find animation targets (from opcodes 0 and 1)
        # We store results temporarily, then re-decode after processing animations
        self.idx = 0
        while self.idx < len(self.bytes_data):
            self._decode_command()

        # Second pass: decode animation blocks at identified targets
        for target in sorted(self.animation_targets):
            self._decode_animation_at(target)

        # Third pass: re-decode commands, skipping animation byte ranges
        self.decoded = [item for item in self.decoded
                       if not isinstance(item, DecodedCommand)]  # Keep only animation items
        self.idx = 0
        while self.idx < len(self.bytes_data):
            bd = self.bytes_data[self.idx]
            # Skip if this offset is part of animation data
            if bd.offset in self.animation_byte_ranges:
                self.idx += 1
                continue
            self._decode_command()

        # Sort decoded items by offset
        self.decoded.sort(key=lambda x: x.offset)

        return self.decoded, self.local_vars, self.detected_buffer

    def _decode_animation_at(self, target_offset: int):
        """Decode animation block at a specific offset"""
        # Find the index for this offset
        for i, bd in enumerate(self.bytes_data):
            if bd.offset == target_offset:
                self.idx = i
                self._decode_animation_block()
                break

    def _to_signed16(self, val: int) -> int:
        """Convert unsigned 16-bit to signed"""
        if val >= 0x8000:
            return val - 0x10000
        return val

    def _to_signed8(self, val: int) -> int:
        """Convert unsigned 8-bit to signed"""
        if val >= 0x80:
            return val - 0x100
        return val

    def _read_bytes(self, count: int) -> List[ByteData]:
        """Read next count byte entries"""
        result = []
        for _ in range(count):
            if self.idx < len(self.bytes_data):
                result.append(self.bytes_data[self.idx])
                self.idx += 1
        return result

    def _peek_value(self, offset: int = 0) -> Optional[ByteData]:
        """Peek at value at current index + offset"""
        idx = self.idx + offset
        if 0 <= idx < len(self.bytes_data):
            return self.bytes_data[idx]
        return None

    def _get_label_at(self, offset: int) -> str:
        """Get or create a label for an offset"""
        if offset in self.offset_to_label:
            return '.' + self.offset_to_label[offset]
        # Create synthetic label
        label = f".loc_{offset:X}"
        self.offset_to_label[offset] = label[1:]  # Store without dot
        return label

    def _get_local_var_name(self, addr: int) -> str:
        """Get or create a name for a local variable address"""
        if addr in self.local_vars:
            return self.local_vars[addr]
        # Create new variable name
        var_name = f"var{self.local_var_counter}"
        self.local_var_counter += 1
        self.local_vars[addr] = var_name
        return var_name

    def _format_addr(self, bd: ByteData) -> str:
        """Format an address value, handling symbols and local vars"""
        if bd.value.is_symbol:
            return bd.value.symbol

        addr = bd.value.value
        # Check if it's a local variable in either buffer
        buf_num, offset = is_local_var_addr(addr)
        if buf_num is not None:
            self.detected_buffer = buf_num  # Track which buffer is used
            return self._get_local_var_name(addr)
        # Return as hex
        return f"0x{addr:08X}"

    def _decode_command(self):
        """Decode a single command"""
        if self.idx >= len(self.bytes_data):
            return

        start_bd = self.bytes_data[self.idx]
        start_offset = start_bd.offset
        label = start_bd.label

        # Read opcode (first 2 bytes)
        if start_bd.size != 2:
            # Not a command, skip
            self.idx += 1
            return

        if start_bd.value.is_symbol:
            # Opcode shouldn't be a symbol
            self.idx += 1
            return

        opcode = start_bd.value.value
        self.idx += 1

        if opcode not in ASM_COMMAND_SPECS:
            # Unknown opcode, skip
            return

        cmd_name, param_types = ASM_COMMAND_SPECS[opcode]
        params = []

        bytes_consumed = 2  # opcode

        for ptype in param_types:
            if self.idx >= len(self.bytes_data):
                break

            bd = self.bytes_data[self.idx]

            if ptype == 'pad8':
                if bd.size == 1:
                    self.idx += 1
                    bytes_consumed += 1
                continue
            elif ptype == 'pad16':
                if bd.size == 2:
                    self.idx += 1
                    bytes_consumed += 2
                elif bd.size == 1:
                    # Skip two 1-byte entries
                    self.idx += 1
                    bytes_consumed += 1
                    if self.idx < len(self.bytes_data) and self.bytes_data[self.idx].size == 1:
                        self.idx += 1
                        bytes_consumed += 1
                continue
            elif ptype in ('u8', 's8'):
                if bd.size == 1:
                    val = bd.value.value
                    if ptype == 's8':
                        val = self._to_signed8(val)
                    params.append(str(val))
                    self.idx += 1
                    bytes_consumed += 1
            elif ptype in ('u16', 's16'):
                if bd.size == 2:
                    val = bd.value.value if not bd.value.is_symbol else 0
                    if ptype == 's16' and not bd.value.is_symbol:
                        val = self._to_signed16(val)
                    if bd.value.is_symbol:
                        params.append(bd.value.symbol)
                    else:
                        params.append(str(val))
                    self.idx += 1
                    bytes_consumed += 2
                elif bd.size == 1:
                    # Combine two 1-byte entries into a 16-bit value (little-endian)
                    low_byte = bd.value.value
                    self.idx += 1
                    bytes_consumed += 1
                    if self.idx < len(self.bytes_data):
                        high_bd = self.bytes_data[self.idx]
                        if high_bd.size == 1:
                            high_byte = high_bd.value.value
                            val = low_byte | (high_byte << 8)
                            if ptype == 's16':
                                val = self._to_signed16(val)
                            params.append(str(val))
                            self.idx += 1
                            bytes_consumed += 1
            elif ptype in ('u32', 's32'):
                if bd.size == 4:
                    if bd.value.is_symbol:
                        params.append(bd.value.symbol)
                    else:
                        params.append(str(bd.value.value))
                    self.idx += 1
                    bytes_consumed += 4
            elif ptype == 'rel16':
                if bd.size == 2 and not bd.value.is_symbol:
                    rel_val = self._to_signed16(bd.value.value)
                    # Target = position of this field + value
                    target_offset = bd.offset + rel_val
                    # Track animation targets for opcodes 0 and 1
                    if opcode in (0, 1) and target_offset > 0:
                        self.animation_targets.add(target_offset)
                    target_label = self._get_label_at(target_offset)
                    params.append(target_label)
                    self.idx += 1
                    bytes_consumed += 2
                elif bd.size == 2:
                    params.append(bd.value.symbol)
                    self.idx += 1
                    bytes_consumed += 2
            elif ptype in ('addr32', 'rom_start32', 'rom_end32'):
                if bd.size == 4:
                    params.append(self._format_addr(bd))
                    self.idx += 1
                    bytes_consumed += 4

        self.decoded.append(DecodedCommand(
            offset=start_offset,
            opcode=opcode,
            name=cmd_name,
            params=params,
            label=label,
            size=bytes_consumed
        ))

    def _decode_animation_block(self):
        """Decode an animation data block"""
        while self.idx < len(self.bytes_data):
            start_bd = self.bytes_data[self.idx]
            start_offset = start_bd.offset
            label = start_bd.label

            # Check if we have enough data for a frame header (2 bytes for anim_id)
            if start_bd.size != 2:
                break

            anim_id = start_bd.value.value if not start_bd.value.is_symbol else 0

            # Check for terminators
            if anim_id == 0xFFFF:
                # ANIM_END
                self.animation_byte_ranges.add(start_offset)
                self.animation_byte_ranges.add(start_offset + 1)
                self.idx += 1
                # Skip the following 0 (offset for end)
                if self.idx < len(self.bytes_data) and self.bytes_data[self.idx].size == 2:
                    self.animation_byte_ranges.add(self.bytes_data[self.idx].offset)
                    self.animation_byte_ranges.add(self.bytes_data[self.idx].offset + 1)
                    self.idx += 1
                self.decoded.append(AnimEnd(offset=start_offset, label=label))
                return  # End of animation block

            elif anim_id == 0xFFFE:
                # ANIM_LOOP
                self.animation_byte_ranges.add(start_offset)
                self.animation_byte_ranges.add(start_offset + 1)
                self.idx += 1
                # Read relative offset
                if self.idx < len(self.bytes_data) and self.bytes_data[self.idx].size == 2:
                    rel_bd = self.bytes_data[self.idx]
                    self.animation_byte_ranges.add(rel_bd.offset)
                    self.animation_byte_ranges.add(rel_bd.offset + 1)
                    rel_val = self._to_signed16(rel_bd.value.value)
                    target_offset = rel_bd.offset + rel_val
                    target_label = self._get_label_at(target_offset)
                    self.idx += 1
                    self.decoded.append(AnimLoop(
                        offset=start_offset,
                        target_label=target_label,
                        label=label
                    ))
                return  # End of animation block

            # Parse regular animation frame (8 bytes total)
            # anim_id (2) + flag (1) + vec (3) + wait (1) + flip (1)
            #
            # Validation: Real animation data has .byte entries after the anim_id.
            # If we see mostly .half entries, this is likely command data, not animation.
            # Check next few entries to validate structure.
            half_count = 0
            byte_count = 0
            for i in range(1, min(7, len(self.bytes_data) - self.idx)):
                check_bd = self.bytes_data[self.idx + i]
                if check_bd.size == 1:
                    byte_count += 1
                elif check_bd.size == 2:
                    half_count += 1

            # If we see more .half entries than .byte entries, this is probably not animation data
            if half_count > byte_count:
                break  # Exit without marking as animation

            # Mark all 8 bytes as animation data
            for i in range(8):
                self.animation_byte_ranges.add(start_offset + i)

            self.idx += 1  # consume anim_id

            # Read remaining 6 bytes (should be .byte entries)
            frame_bytes = []
            bytes_needed = 6
            while bytes_needed > 0 and self.idx < len(self.bytes_data):
                bd = self.bytes_data[self.idx]
                if bd.size == 1:
                    frame_bytes.append(bd.value.value if not bd.value.is_symbol else 0)
                    bytes_needed -= 1
                    self.idx += 1
                elif bd.size == 2:
                    # Two bytes packed
                    val = bd.value.value if not bd.value.is_symbol else 0
                    frame_bytes.append(val & 0xFF)
                    frame_bytes.append((val >> 8) & 0xFF)
                    bytes_needed -= 2
                    self.idx += 1
                else:
                    break

            if len(frame_bytes) >= 6:
                flag = frame_bytes[0]
                vec = (frame_bytes[1], frame_bytes[2], frame_bytes[3])
                wait = frame_bytes[4]
                flip = frame_bytes[5]

                self.decoded.append(AnimFrame(
                    offset=start_offset,
                    anim_id=anim_id,
                    flag=flag,
                    vec=vec,
                    wait=wait,
                    flip=flip,
                    label=label
                ))
            else:
                # Not enough bytes, probably not animation data
                break


# =============================================================================
# DSL EMITTER
# =============================================================================

class DslEmitter:
    """Emits DSL from decoded commands and animation data"""

    def __init__(self, decoded: List[DecodedItem], local_vars: Dict[int, str],
                 offset_to_label: Dict[int, str], detected_buffer: Optional[int] = None):
        self.decoded = decoded
        self.local_vars = local_vars
        self.offset_to_label = offset_to_label
        self.detected_buffer = detected_buffer
        self.output: List[str] = []

    def emit(self) -> str:
        """Generate DSL output"""

        # Emit header
        self._emit("; Auto-generated from ASM by hm64_asm_to_dsl.py")
        self._emit("")

        # Emit local variable declarations
        if self.local_vars:
            self._emit("; Local variables")
            # Determine which buffer base to use
            if self.detected_buffer and self.detected_buffer in BUFFER_BASES:
                buffer_base = BUFFER_BASES[self.detected_buffer][0]
            else:
                buffer_base = BUFFER_BASES[1][0]  # Default to buffer 1
            # Sort by address
            sorted_vars = sorted(self.local_vars.items(), key=lambda x: x[0])
            for addr, name in sorted_vars:
                # Determine type based on address alignment
                offset = addr - buffer_base
                if offset % 4 == 0:
                    self._emit(f".var {name}, u32")
                elif offset % 2 == 0:
                    self._emit(f".var {name}, u16")
                else:
                    self._emit(f".var {name}, u8")
            self._emit("")

        # Emit commands and animation data
        for item in self.decoded:
            if isinstance(item, DecodedCommand):
                self._emit_command(item)
            elif isinstance(item, AnimFrame):
                self._emit_anim_frame(item)
            elif isinstance(item, AnimEnd):
                self._emit_anim_end(item)
            elif isinstance(item, AnimLoop):
                self._emit_anim_loop(item)

        return '\n'.join(self.output)

    def _emit(self, text: str = ""):
        """Add a line to output"""
        self.output.append(text)

    def _emit_label(self, label: Optional[str]):
        """Emit a label if present"""
        if label:
            self._emit("")
            # Add dot prefix for local labels
            if not label.startswith('.'):
                label = '.' + label
            self._emit(f"{label}:")

    def _emit_command(self, cmd: DecodedCommand):
        """Emit a command"""
        self._emit_label(cmd.label)

        name = cmd.name
        if name.startswith('CMD_'):
            name = name[4:]

        # Format parameters
        if cmd.params:
            params_str = ', '.join(cmd.params)
            self._emit(f"    {name} {params_str}")
        else:
            self._emit(f"    {name}")

    def _emit_anim_frame(self, frame: AnimFrame):
        """Emit an animation frame"""
        self._emit_label(frame.label)
        vec_str = f"({frame.vec[0]}, {frame.vec[1]}, {frame.vec[2]})"
        self._emit(f"    .anim_frame {frame.anim_id}, 0x{frame.flag:02X}, {vec_str}, {frame.wait}, {frame.flip}")

    def _emit_anim_end(self, end: AnimEnd):
        """Emit animation end marker"""
        self._emit_label(end.label)
        self._emit("    .anim_end")

    def _emit_anim_loop(self, loop: AnimLoop):
        """Emit animation loop marker"""
        self._emit_label(loop.label)
        self._emit(f"    .anim_loop {loop.target_label}")


# =============================================================================
# MAIN
# =============================================================================

def main():
    parser = argparse.ArgumentParser(
        description='Convert HM64 cutscene ASM files to DSL format'
    )
    parser.add_argument('input', help='Input .s assembly file')
    parser.add_argument('-o', '--output', help='Output .cutscene file (default: stdout)')

    args = parser.parse_args()

    input_path = Path(args.input)
    if not input_path.exists():
        print(f"Error: Input file '{args.input}' not found", file=sys.stderr)
        sys.exit(1)

    # Parse ASM
    asm_parser = AsmParser()
    bytes_data, labels, offset_to_label = asm_parser.parse_file(input_path)

    # Decode commands
    decoder = CommandDecoder(bytes_data, labels, offset_to_label)
    decoded, local_vars, detected_buffer = decoder.decode()

    # Emit DSL
    emitter = DslEmitter(decoded, local_vars, offset_to_label, detected_buffer)
    output = emitter.emit()

    # Write output
    if args.output:
        output_path = Path(args.output)
        with open(output_path, 'w') as f:
            f.write(output)
            f.write('\n')
        print(f"Wrote {output_path}")
    else:
        print(output)


if __name__ == '__main__':
    main()
