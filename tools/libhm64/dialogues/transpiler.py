"""
HM64 Dialogue DSL to GNU as Transpiler.

Converts dialogue DSL source to GNU assembler syntax (.s files) that can be
assembled and linked with the main game.

Usage:
    python -m hm64.dialogues.transpiler transpile <input.dialogue> -n <name>
"""

import argparse
import re
import sys
from dataclasses import dataclass, field
from pathlib import Path
from typing import Dict, List, Optional, Tuple

from .opcodes import BUILTIN_CONSTANTS, COMMAND_SPECS


# =============================================================================
# PARSER
# =============================================================================

@dataclass
class SourceLine:
    """Represents a parsed source line"""
    line_num: int
    raw_text: str
    label: Optional[str] = None
    command: Optional[str] = None
    params: List[str] = field(default_factory=list)
    is_directive: bool = False
    directive_name: Optional[str] = None
    directive_args: List[str] = field(default_factory=list)
    comment: Optional[str] = None


class DSLParser:
    """Parses the dialogue DSL source"""

    def __init__(self):
        self.lines: List[SourceLine] = []

    def parse_file(self, source: str) -> List[SourceLine]:
        """Parse source text into list of SourceLine objects"""
        self.lines = []

        for line_num, raw_line in enumerate(source.split('\n'), 1):
            parsed = self._parse_line(line_num, raw_line)
            if parsed:
                self.lines.append(parsed)

        return self.lines

    def _parse_line(self, line_num: int, raw_line: str) -> Optional[SourceLine]:
        """Parse a single source line"""
        result = SourceLine(line_num=line_num, raw_text=raw_line)

        # Extract comment
        line = raw_line

        for comment_char in [';', '#', '//']:
            if comment_char in line:
                idx = line.index(comment_char)
                result.comment = line[idx:].strip()
                line = line[:idx]
                break

        line = line.strip()

        if not line:
            return result if result.comment else None

        # Check for label (ends with colon, starts with . or alphanumeric)
        if ':' in line:
            parts = line.split(':', 1)
            label_part = parts[0].strip()

            if label_part and (label_part.startswith('.') or label_part[0].isalpha() or label_part[0] == '_'):
                result.label = label_part
                line = parts[1].strip() if len(parts) > 1 else ''

        if not line:
            return result if result.label or result.comment else None

        # Check for directives
        if line.startswith('.'):
            result.is_directive = True
            parts = line.split(None, 1)
            result.directive_name = parts[0]

            if len(parts) > 1:
                result.directive_args = self._parse_params(parts[1])

            return result

        # Parse command
        match = re.match(r'(\w+)\s*(.*)', line)

        if match:
            result.command = match.group(1)

            if match.group(2):
                result.params = self._parse_params(match.group(2))

        return result

    def _parse_params(self, param_str: str) -> List[str]:
        """Parse comma-separated parameters, handling parentheses and brackets"""
        params = []
        current = ""
        paren_depth = 0
        bracket_depth = 0

        for char in param_str:
            if char == '(':
                paren_depth += 1
                current += char
            elif char == ')':
                paren_depth -= 1
                current += char
            elif char == '[':
                bracket_depth += 1
                current += char
            elif char == ']':
                bracket_depth -= 1
                current += char
            elif char == ',' and paren_depth == 0 and bracket_depth == 0:
                params.append(current.strip())
                current = ""
            else:
                current += char

        if current.strip():
            params.append(current.strip())

        return params


# =============================================================================
# TRANSPILER
# =============================================================================

class DialogueTranspiler:
    """Transpiles dialogue DSL to GNU as syntax (two separate files)"""

    # Reserved GNU as directives
    RESERVED_DIRECTIVES = {
        '.end', '.data', '.text', '.section', '.global', '.globl',
        '.byte', '.half', '.word', '.long', '.short', '.hword',
        '.align', '.balign', '.p2align',
    }

    def __init__(self, bank_name: str = "text1Dialogue"):
        self.bank_name = bank_name
        self.index_name = f"{bank_name}Index"
        self.parser = DSLParser()
        self.bytecode_lines: List[str] = []
        self.index_lines: List[str] = []
        self.errors: List[str] = []
        self.warnings: List[str] = []
        self.current_line: int = 0
        self.labels: Dict[str, int] = {}  # label -> segment index for offset table
        self.segment_labels: List[Tuple[int, str]] = []  # [(index, label), ...]
        self.bytecode_size: Optional[int] = None  # Expected bytecode size for padding
        self.index_size: Optional[int] = None  # Expected index size for padding

    def transpile(self, source: str) -> Tuple[str, str]:
        """
        Transpile DSL source to two GNU as files.

        Returns:
            Tuple of (bytecode_asm, index_asm)
        """
        self.bytecode_lines = []
        self.index_lines = []
        self.errors = []
        self.warnings = []
        self.labels = {}
        self.segment_labels = []
        self.segment_offsets = {}  # segment_index -> byte offset from start
        self.segment_aliases = {}  # custom_name -> segment_index (e.g., '.winter' -> 19)
        self.current_byte_offset = 0  # Track current position in bytecode

        lines = self.parser.parse_file(source)

        # First pass: collect directives (.define, .bytecode_size, .index_size)
        for line in lines:
            if line.is_directive:
                if line.directive_name == '.define':
                    # Parse .define arguments - split on whitespace since they're space-separated
                    args = []
                    if line.directive_args:
                        # The parser may have combined args into one string, so split them
                        for arg in line.directive_args:
                            args.extend(arg.split())

                    if len(args) >= 2:
                        alias_name = args[0]
                        try:
                            segment_idx = int(args[1])
                            self.segment_aliases[alias_name] = segment_idx
                        except ValueError:
                            self.errors.append(f"Line {line.line_num}: .define requires numeric segment index, got '{args[1]}'")

                elif line.directive_name == '.bytecode_size':
                    # Parse expected bytecode size for padding
                    if line.directive_args:
                        try:
                            self.bytecode_size = int(line.directive_args[0])
                        except ValueError:
                            self.errors.append(f"Line {line.line_num}: .bytecode_size requires numeric value")

                elif line.directive_name == '.index_size':
                    # Parse expected index size for padding
                    if line.directive_args:
                        try:
                            self.index_size = int(line.directive_args[0])
                        except ValueError:
                            self.errors.append(f"Line {line.line_num}: .index_size requires numeric value")

        # Second pass: collect segment labels for offset table
        # Handle both .segment_X and custom names defined via .define
        for line in lines:
            if line.label:
                segment_idx = None

                # Check if it's a standard .segment_X label
                if line.label.startswith('.segment_'):
                    try:
                        segment_idx = int(line.label.split('_')[1])
                    except (ValueError, IndexError):
                        self.warnings.append(f"Line {line.line_num}: Could not parse segment index from '{line.label}'")

                # Check if it's a custom name that was defined
                elif line.label in self.segment_aliases:
                    segment_idx = self.segment_aliases[line.label]

                # If we found a segment index, record it
                if segment_idx is not None:
                    self.labels[line.label] = segment_idx
                    self.segment_labels.append((segment_idx, line.label))

        # Sort segment labels by index for proper offset table ordering
        self.segment_labels.sort(key=lambda x: x[0])

        # Generate bytecode file
        self._generate_bytecode_file(lines)

        # Generate index file
        self._generate_index_file()

        bytecode_output = '\n'.join(self.bytecode_lines) + '\n'
        index_output = '\n'.join(self.index_lines) + '\n'

        return bytecode_output, index_output

    def _emit_bytecode(self, text: str = ""):
        """Emit a line to bytecode output"""
        self.bytecode_lines.append(text)

    def _emit_bytecode_data(self, text: str, byte_count: int):
        """Emit a line of data to bytecode output and track byte offset"""
        self.bytecode_lines.append(text)
        self.current_byte_offset += byte_count

    def _emit_index(self, text: str = ""):
        """Emit a line to index output"""
        self.index_lines.append(text)

    def _generate_bytecode_file(self, lines: List[SourceLine]):
        """Generate the bytecode assembly file"""
        # Header
        self._emit_bytecode(f"# Auto-generated dialogue bytecode: {self.bank_name}")
        self._emit_bytecode()
        self._emit_bytecode(".section .data")
        self._emit_bytecode()

        self._emit_bytecode(f".global {self.bank_name}")
        self._emit_bytecode(f"{self.bank_name}:")
        self._emit_bytecode()

        # Process code lines
        for line in lines:
            self.current_line = line.line_num
            self._process_line(line)

        # Emit padding to reach expected bytecode size
        if self.bytecode_size is not None and self.current_byte_offset < self.bytecode_size:
            padding_needed = self.bytecode_size - self.current_byte_offset
            self._emit_bytecode()
            self._emit_bytecode(f"    # Padding to reach {self.bytecode_size} bytes (0x{self.bytecode_size:X})")
            # Emit padding in chunks of 16 bytes for readability
            while padding_needed > 0:
                chunk = min(padding_needed, 16)
                padding_bytes = ', '.join(['0x00'] * chunk)
                self._emit_bytecode_data(f"    .byte   {padding_bytes}", chunk)
                padding_needed -= chunk

        # Footer
        self._emit_bytecode()
        self._emit_bytecode(f"# End of {self.bank_name}")

    def _generate_index_file(self):
        """Generate the index/offset table assembly file"""
        # Header
        self._emit_index(f"# Auto-generated dialogue index: {self.index_name}")
        self._emit_index("# Generated by hm64.dialogues.transpiler")
        self._emit_index("# Offsets are pre-computed at transpile time")
        self._emit_index()
        self._emit_index(".section .data")
        self._emit_index()

        self._emit_index(f".global {self.index_name}")
        self._emit_index(f"{self.index_name}:")

        # Track how many bytes we've emitted
        index_bytes_emitted = 0

        if self.segment_labels:
            # Find max segment index to know table size
            max_idx = max(idx for idx, _ in self.segment_labels)

            # Emit entries for all indices 0 to max using pre-computed offsets
            for i in range(max_idx + 1):
                if i in self.segment_offsets:
                    offset = self.segment_offsets[i]
                    self._emit_index(f"    .word   {offset}  # segment {i}")
                else:
                    # Missing segment - emit 0 placeholder
                    self._emit_index(f"    .word   0  # segment {i} (undefined)")
                index_bytes_emitted += 4

        # Emit padding to reach expected index size
        if self.index_size is not None and index_bytes_emitted < self.index_size:
            padding_needed = self.index_size - index_bytes_emitted
            if padding_needed > 0:
                self._emit_index()
                self._emit_index(f"    # Padding to reach {self.index_size} bytes (0x{self.index_size:X})")
                # Emit padding in chunks of 16 bytes for readability
                while padding_needed > 0:
                    chunk = min(padding_needed, 16)
                    padding_bytes = ', '.join(['0x00'] * chunk)
                    self._emit_index(f"    .byte   {padding_bytes}")
                    padding_needed -= chunk
        elif not self.segment_labels and self.index_size is None:
            self._emit_index("    # No segments defined")

        self._emit_index()
        self._emit_index(f"# End of {self.index_name}")

    def _transform_label(self, label: str) -> str:
        """
        Transform label to a global-safe name.
        Uses bank_name prefix to avoid collisions between dialogue banks.
        """
        if label.startswith('.segment_'):
            # Extract segment number and create a global label
            seg_num = label.split('_')[1]
            return f"{self.bank_name}_seg_{seg_num}"
        elif label.startswith('.'):
            # Other local labels - prefix with bank name
            return f"{self.bank_name}_{label[1:]}"
        return label

    def _process_line(self, line: SourceLine):
        """Process a single parsed line"""
        # Skip directives that were already processed in first pass
        if line.is_directive and line.directive_name in ['.define', '.bytecode_size', '.index_size']:
            return

        if line.label:
            asm_label = self._transform_label(line.label)

            # Determine if this is a segment label (either .segment_X or a defined alias)
            seg_idx = None
            if line.label.startswith('.segment_'):
                try:
                    seg_idx = int(line.label.split('_')[1])
                except (ValueError, IndexError):
                    pass
            elif line.label in self.segment_aliases:
                seg_idx = self.segment_aliases[line.label]

            # For segment labels, add padding to align to 4 bytes
            if seg_idx is not None:
                padding_needed = (4 - (self.current_byte_offset % 4)) % 4
                if padding_needed > 0:
                    # Emit padding bytes
                    padding_bytes = ', '.join(['0x00'] * padding_needed)
                    self._emit_bytecode_data(f"    .byte   {padding_bytes}  # padding for 4-byte alignment", padding_needed)

                # Record byte offset for this segment
                self.segment_offsets[seg_idx] = self.current_byte_offset

            self._emit_bytecode(f"{asm_label}:")

        # Handle command
        if line.command:
            self._emit_command(line)

        # Handle standalone comment - convert ; to # for GNU assembler
        elif line.comment and not line.label and not line.command:
            # Convert ; or // style comments to # for assembly
            comment_text = line.comment
            if comment_text.startswith(';'):
                comment_text = '#' + comment_text[1:]
            elif comment_text.startswith('//'):
                comment_text = '#' + comment_text[2:]
            elif not comment_text.startswith('#'):
                comment_text = '# ' + comment_text
            self._emit_bytecode(f"    {comment_text}")

    def _emit_command(self, line: SourceLine):
        """Emit bytecode for a command"""
        cmd_name = line.command.upper()
        params = line.params

        # Look up command specification
        if cmd_name not in COMMAND_SPECS:
            self.errors.append(f"Line {line.line_num}: Unknown command '{cmd_name}'")
            return

        opcode, size, param_specs = COMMAND_SPECS[cmd_name]

        # Verify parameter count
        expected_params = len(param_specs)
        provided_params = len(params)

        if provided_params != expected_params:
            param_names = [name for name, _ in param_specs]
            self.errors.append(
                f"Line {line.line_num}: {cmd_name} expects {expected_params} parameter(s) "
                f"({', '.join(param_names)}), but got {provided_params}"
            )
            return

        # Add comment with original command (use # for GNU assembler)
        comment = f"# {line.command}"
        if params:
            comment += f" {', '.join(params)}"

        # Emit opcode (1 byte for dialogue bytecode)
        self._emit_bytecode_data(f"    .byte   0x{opcode:02X}          {comment}", 1)

        # Emit parameters
        for i, (spec_name, spec_type) in enumerate(param_specs):
            param_value = params[i] if i < len(params) else '0'
            self._emit_param(param_value, spec_type, line.line_num)

    def _emit_param(self, value: str, param_type: str, line_num: int):
        """Emit a parameter value based on its type"""
        resolved = self._resolve_value(value)

        if param_type in ['u8', 's8']:
            self._emit_bytecode_data(f"    .byte   {resolved}", 1)

        elif param_type in ['u16', 's16']:
            # Little-endian 16-bit value
            # MIPS is big-endian, so .2byte/.half would emit wrong byte order
            # Must emit as two bytes manually: low byte first, high byte second
            self._emit_u16_le(resolved)

        elif param_type == 'rel16':
            # Segment index reference - resolve label to index or pass through
            resolved = self._resolve_segment_ref(value)
            self._emit_u16_le(resolved)

        else:
            self.errors.append(f"Line {line_num}: Unknown param type '{param_type}'")

    def _emit_u16_le(self, value: str):
        """Emit a 16-bit value in little-endian byte order"""
        # Try to evaluate the value to emit as literal bytes
        try:
            if value.startswith('0x') or value.startswith('0X'):
                int_val = int(value, 16)
            elif value.startswith('-'):
                # Signed value - convert to unsigned 16-bit
                int_val = int(value) & 0xFFFF
            else:
                int_val = int(value)

            low_byte = int_val & 0xFF
            high_byte = (int_val >> 8) & 0xFF
            self._emit_bytecode_data(f"    .byte   0x{low_byte:02X}, 0x{high_byte:02X}", 2)
        except ValueError:
            # Not a simple numeric value - fall back to .2byte (may have wrong endianness)
            self.warnings.append(f"Cannot emit '{value}' as little-endian bytes, using .2byte")
            self._emit_bytecode_data(f"    .2byte  {value}  # WARNING: may have wrong endianness", 2)

    def _resolve_value(self, s: str) -> str:
        """Resolve a value - returns string suitable for GNU as"""
        s = s.strip()

        # Check built-in constants
        if s in BUILTIN_CONSTANTS:
            return str(BUILTIN_CONSTANTS[s])

        # Check for hex
        if s.startswith('0x') or s.startswith('0X'):
            return s

        # Check for binary
        if s.startswith('0b') or s.startswith('0B'):
            return str(int(s, 2))

        # Check for negative number
        if s.startswith('-') and s[1:].isdigit():
            return s

        # Check for decimal
        if s.isdigit():
            return s

        # Otherwise, it's a symbol
        if s.startswith('.'):
            return self._transform_label(s)

        return s

    def _resolve_segment_ref(self, s: str) -> str:
        """Resolve a segment reference to its index"""
        s = s.strip()

        # Check if numeric literal
        if s.isdigit():
            return s
        if s.startswith('0x') or s.startswith('0X'):
            return s

        # Check built-in constants (like NO_BRANCH)
        if s in BUILTIN_CONSTANTS:
            return str(BUILTIN_CONSTANTS[s])

        # Check if known segment label
        if s in self.labels:
            return str(self.labels[s])

        # Check if defined alias (for forward references or external segments)
        if s in self.segment_aliases:
            return str(self.segment_aliases[s])

        # Unknown label - try to extract segment number from .segment_N format
        if s.startswith('.segment_'):
            try:
                seg_num = int(s.split('_')[1])
                self.warnings.append(f"Line {self.current_line}: Unknown segment label '{s}' - using index {seg_num}")
                return str(seg_num)
            except (ValueError, IndexError):
                pass

        # Completely unknown - emit warning and pass through as-is
        self.warnings.append(f"Line {self.current_line}: Cannot resolve segment reference '{s}'")
        return self._resolve_value(s)


# =============================================================================
# MAIN
# =============================================================================

def main():
    parser = argparse.ArgumentParser(
        description='HM64 Dialogue DSL Transpiler',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Example:
  %(prog)s transpile dialogue.dialogue -n text1Dialogue

  This generates two files:
    - text1Dialogue.s       (bytecode)
    - text1DialogueIndex.s  (offset table)
"""
    )

    parser.add_argument('command', choices=['transpile'],
                        help='Command to execute')
    parser.add_argument('input', help='Input file')
    parser.add_argument('-o', '--output', help='Output directory or file (default: same as input)')
    parser.add_argument('-n', '--name', default='text1Dialogue',
                        help='Bank name for symbols (default: text1Dialogue)')
    parser.add_argument('-q', '--quiet', action='store_true',
                        help='Suppress warnings about undefined segment labels')

    args = parser.parse_args()

    with open(args.input, 'r', encoding='utf-8') as f:
        source = f.read()

    transpiler = DialogueTranspiler(bank_name=args.name)
    bytecode_asm, index_asm = transpiler.transpile(source)

    for warning in transpiler.warnings:
        if not args.quiet:
            print(f"WARNING: {warning}", file=sys.stderr)

    for error in transpiler.errors:
        print(f"ERROR: {error}", file=sys.stderr)

    if transpiler.errors:
        sys.exit(1)

    # Determine output paths
    if args.output:
        output_path = Path(args.output)

        if output_path.is_dir() or args.output.endswith('/'):
            # Output is a directory - generate filenames from bank name
            output_path.mkdir(parents=True, exist_ok=True)
            bytecode_path = output_path / f"{args.name}.s"
            index_path = output_path / f"{args.name}Index.s"
        elif args.output.endswith('.s'):
            # Output is a .s file - use it for bytecode, derive index name
            bytecode_path = output_path
            index_path = output_path.with_name(output_path.stem + "Index.s")
        else:
            # Treat as base path - append .s and Index.s
            bytecode_path = Path(f"{args.output}.s")
            index_path = Path(f"{args.output}Index.s")
    else:
        # Default to same directory as input
        input_dir = Path(args.input).parent
        bytecode_path = input_dir / f"{args.name}.s"
        index_path = input_dir / f"{args.name}Index.s"

    with open(bytecode_path, 'w', encoding='utf-8') as f:
        f.write(bytecode_asm)
    print(f"Wrote bytecode to {bytecode_path}", file=sys.stderr)

    with open(index_path, 'w', encoding='utf-8') as f:
        f.write(index_asm)
    print(f"Wrote index to {index_path}", file=sys.stderr)


if __name__ == '__main__':
    main()
