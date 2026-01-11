import argparse
import sys
import re
from pathlib import Path
from typing import List, Tuple, Optional

from hm64_text_utilities import CHAR_MAP, CONTROL_CODES, GAMEVAR_MACRO_TO_INDEX, CHARACTER_AVATAR_MACRO_TO_INDEX

# Create reverse mappings for encoding
REVERSE_CHAR_MAP = {v: k for k, v in CHAR_MAP.items()}
REVERSE_CONTROL_CODES = {v: k for k, v in CONTROL_CODES.items()}

REVERSE_CHAR_MAP["'"] = 0x10E  # Straight apostrophe -> curly apostrophe '
REVERSE_CHAR_MAP['"'] = 0x107  # Straight double quote -> left curly "
REVERSE_CHAR_MAP['-'] = 0xEC   # Hyphen-minus (already mapped, but ensure it works)

CONTROL_CODE_PATTERN = re.compile(r'\[([A-Z_0-9]+)(?::([A-Za-z_0-9]+))?\]')
# Pattern for raw 1-byte char: [CHAR:0xXX] or [CHAR:0xXX→0xYY] (preserves original byte)
RAW_CHAR_PATTERN = re.compile(r'\[CHAR:0x([0-9A-Fa-f]{2})(?:→0x[0-9A-Fa-f]{2})?\]')
# Pattern for raw 2-byte word: [WORD:0xXXXX] (preserves as 2-byte code)
RAW_WORD_PATTERN = re.compile(r'\[WORD:0x([0-9A-Fa-f]{4})\]')
# Pattern for raw bytes directive: [RAWBYTES:hexstring] - bypasses compression entirely
RAWBYTES_PATTERN = re.compile(r'\[RAWBYTES:([0-9A-Fa-f]+)\]')
METADATA_PATTERN = re.compile(r'^#\s*([A-Z_]+):\s*(.+)$')

# Message box type to line width mapping
MESSAGE_BOX_TYPES = {
    'DEFAULT': 16,      # Main dialogue box
    'MENU': 10,         # Overlay menus, lists
    'NAMING_SCREEN': 6, # Naming screen input
    'SELECTION': 16,    # Pink dialogue selection boxes - preserves line layout
}

# Box types that should preserve line layout (auto_linebreak=False)
PRESERVE_LAYOUT_TYPES = {'SELECTION'}


class TextEncoder:
    """Encodes text strings into the game's compressed binary format"""

    def __init__(self):
        self.verbose = False
        self.auto_linebreak = True  # Default ON for modding - auto-insert linebreaks based on MESSAGE_BOX_TYPE
        self.line_width = 16
        self.auto_textend = True

    def encode_text(self, text: str, metadata: dict = None) -> bytes:
        """
        Encode a text string into the game's binary format.

        Args:
            text: Text string with control codes like [LINEBREAK], [TEXTEND], etc.
            metadata: Optional metadata dictionary with settings

        Returns:
            Encoded bytes ready to be assembled
        """

        if metadata:
            if 'MESSAGE_BOX_TYPE' in metadata:
                box_type = metadata['MESSAGE_BOX_TYPE'].upper().strip()
                if box_type in MESSAGE_BOX_TYPES:
                    self.line_width = MESSAGE_BOX_TYPES[box_type]
                    # Disable auto_linebreak for box types that need to preserve layout
                    if box_type in PRESERVE_LAYOUT_TYPES:
                        self.auto_linebreak = False
                else:
                    raise ValueError(f"Unknown MESSAGE_BOX_TYPE: {box_type}. Valid types: {', '.join(MESSAGE_BOX_TYPES.keys())}")
            elif 'LINE_WIDTH' in metadata:
                self.line_width = int(metadata['LINE_WIDTH'])
            # Explicit AUTO_LINEBREAK setting overrides box type default
            if 'AUTO_LINEBREAK' in metadata:
                self.auto_linebreak = metadata['AUTO_LINEBREAK'].lower() in ['true', 'yes', '1']
            if 'AUTO_TEXTEND' in metadata:
                self.auto_textend = metadata['AUTO_TEXTEND'].lower() in ['true', 'yes', '1']

        if not text.strip():
            return b'\x00\x00\x00\x00'

        # Check for [RAWBYTES:hex] directive - bypasses compression entirely
        rawbytes_match = RAWBYTES_PATTERN.match(text.strip())
        if rawbytes_match:
            hex_string = rawbytes_match.group(1)
            raw_data = bytes.fromhex(hex_string)
            padding_needed = (4 - (len(raw_data) % 4)) % 4
            if padding_needed > 0:
                raw_data += b'\x00' * padding_needed
            return raw_data

        content_check = text
        content_check = re.sub(r'\[LINEBREAK\]', '', content_check)
        content_check = re.sub(r'\[SOFTBREAK\]', '', content_check)
        content_check = content_check.strip()

        if not content_check:
            return b'\x00\x00\x00\x00'

        # Handle linebreaks based on mode
        if self.auto_linebreak:
            text = self._add_automatic_linebreaks(text)
        else:
            text = self._convert_newlines_to_linebreaks(text)

        if self.auto_textend and text.strip() and not text.endswith('[TEXTEND]'):
            text = text + '[TEXTEND]'

        # Parse the text into a list of character codes
        char_codes = self._parse_text_to_codes(text)

        if self.verbose:
            print(f"Parsed {len(char_codes)} character codes")

        # Encode the character codes into compressed binary format
        binary_data = self._encode_codes_to_binary(char_codes)

        # Add padding to align to 4-byte boundary
        padding_needed = (4 - (len(binary_data) % 4)) % 4
        if padding_needed > 0:
            binary_data += b'\x00' * padding_needed

        return binary_data

    def _parse_text_to_codes(self, text: str) -> list:
        """Parse text string into a list of character codes."""

        codes = []
        i = 0

        while i < len(text):
            # Check for raw 1-byte codes first: [CHAR:0xXX]
            char_match = RAW_CHAR_PATTERN.match(text, i)
            if char_match:
                raw_byte = int(char_match.group(1), 16)
                codes.append(raw_byte)
                i = char_match.end()
                continue

            # Check for raw 2-byte codes: [WORD:0xXXXX]
            word_match = RAW_WORD_PATTERN.match(text, i)
            if word_match:
                raw_word = int(word_match.group(1), 16)
                codes.append(raw_word)
                i = word_match.end()
                continue

            match = CONTROL_CODE_PATTERN.match(text, i)

            if match:
                control_name = match.group(1)
                param_str = match.group(2)

                # Check for gamevar macros first
                if control_name in GAMEVAR_MACRO_TO_INDEX:
                    gamevar_index = GAMEVAR_MACRO_TO_INDEX[control_name]
                    codes.append(7)  # INSERT_GAMEVAR control code
                    codes.append(('param1', gamevar_index))
                    i = match.end()
                    continue

                elif control_name in REVERSE_CONTROL_CODES:
                    control_code = REVERSE_CONTROL_CODES[control_name]
                    codes.append(control_code)

                    if param_str is not None:
                        # CHARACTER_AVATAR supports macro names (e.g., MARIA_1) or decimal indices
                        if control_code == 9:  # CHARACTER_AVATAR
                            if param_str in CHARACTER_AVATAR_MACRO_TO_INDEX:
                                param = CHARACTER_AVATAR_MACRO_TO_INDEX[param_str]
                            else:
                                param = int(param_str, 10)
                        # WAIT, LOAD_TEXT use decimal for readability
                        elif control_code in (3, 5):  # WAIT, LOAD_TEXT
                            param = int(param_str, 10)
                        else:
                            param = int(param_str, 16)

                        if control_code == 5:  # LOAD_TEXT - 2-byte parameter
                            codes.append(('param2', param))
                        elif control_code in [3, 7, 9]:  # WAIT, INSERT_GAMEVAR, CHARACTER_AVATAR
                            codes.append(('param1', param))

                    i = match.end()
                    continue

                else:
                    raise ValueError(f"Unknown control code: {control_name}")

            # Regular character
            char = text[i]

            if char in REVERSE_CHAR_MAP:
                codes.append(REVERSE_CHAR_MAP[char])
            elif char == '\n':
                codes.append(0)
            elif char == '\r':
                pass
            else:
                print(f"Warning: Unknown character '{char}' (0x{ord(char):02X}) at position {i}, skipping")

            i += 1

        return codes

    def _convert_newlines_to_linebreaks(self, text: str) -> str:
        """Convert newlines in text to [LINEBREAK] codes."""
        text = text.replace('\r\n', '\n')

        textend_pos = text.find('[TEXTEND]')
        if textend_pos >= 0:
            text = text[:textend_pos + len('[TEXTEND]')]

        text = re.sub(r'(\[(?:LINEBREAK|SOFTBREAK)\])\n', r'\1', text)
        text = text.replace('\n', '[LINEBREAK]')

        return text

    def _add_automatic_linebreaks(self, text: str) -> str:
        """Automatically add line breaks to text based on line width."""

        lines = []
        current_line = []
        current_length = 0

        tokens = []
        i = 0
        current_word = []

        while i < len(text):
            match = CONTROL_CODE_PATTERN.match(text, i)

            if match:
                if current_word:
                    tokens.append(('word', ''.join(current_word)))
                    current_word = []
                tokens.append(('control', match.group(0)))
                i = match.end()

            elif text[i] in ' \t':
                if current_word:
                    tokens.append(('word', ''.join(current_word)))
                    current_word = []
                tokens.append(('space', text[i]))
                i += 1

            elif text[i] in '\r\n':
                if current_word:
                    tokens.append(('word', ''.join(current_word)))
                    current_word = []
                tokens.append(('space', ' '))
                i += 1

            else:
                current_word.append(text[i])
                i += 1

        if current_word:
            tokens.append(('word', ''.join(current_word)))

        for token_type, token_value in tokens:
            if token_type == 'control':
                if token_value == '[LINEBREAK]' or token_value == '[SOFTBREAK]':
                    if current_line:
                        lines.append(''.join(current_line))
                    current_line = [token_value]
                    current_length = 0
                else:
                    current_line.append(token_value)

            elif token_type == 'space':
                if current_length + 1 <= self.line_width:
                    current_line.append(token_value)
                    current_length += 1

            elif token_type == 'word':
                word_length = len(token_value)

                if current_length + word_length > self.line_width:
                    if word_length > self.line_width:
                        remaining = self.line_width - current_length
                        if remaining > 0:
                            current_line.append(token_value[:remaining])
                            lines.append(''.join(current_line))
                            token_value = token_value[remaining:]
                            current_line = []
                            current_length = 0

                        while len(token_value) > self.line_width:
                            lines.append(token_value[:self.line_width] + '[LINEBREAK]')
                            token_value = token_value[self.line_width:]

                        if token_value:
                            current_line = [token_value]
                            current_length = len(token_value)
                    else:
                        if current_line:
                            current_line.append('[LINEBREAK]')
                            lines.append(''.join(current_line))
                        current_line = [token_value]
                        current_length = word_length
                else:
                    current_line.append(token_value)
                    current_length += word_length

        if current_line:
            lines.append(''.join(current_line))

        return ''.join(lines)

    def _encode_codes_to_binary(self, codes: list) -> bytes:
        """Encode character codes into compressed binary format."""

        output = bytearray()
        char_count = 0
        control_byte = 0
        pending_chars = []

        i = 0

        while i < len(codes):
            code = codes[i]

            if isinstance(code, tuple):
                param_type, param_value = code

                if param_type == 'param2':
                    pending_chars.append(('raw', param_value & 0xFF))
                    pending_chars.append(('raw', (param_value >> 8) & 0xFF))
                elif param_type == 'param1':
                    pending_chars.append(('raw', param_value & 0xFF))

                i += 1
                continue

            if char_count > 0 and char_count % 8 == 0:
                self._write_control_block(output, control_byte, pending_chars)
                control_byte = 0
                pending_chars = []

            bit_position = char_count % 8

            if code >= 0x100:
                control_byte |= (0x80 >> bit_position)
                pending_chars.append(('char2', code))
            else:
                pending_chars.append(('char1', code))

            char_count += 1
            i += 1

        if pending_chars:
            self._write_control_block(output, control_byte, pending_chars)

        return bytes(output)

    def _write_control_block(self, output: bytearray, control_byte: int, chars: list):
        """Write a control byte followed by its character data"""
        output.append(control_byte)

        for char_type, value in chars:
            if char_type == 'char1':
                output.append(value)
            elif char_type == 'char2':
                output.append(value & 0xFF)
                output.append((value >> 8) & 0xFF)
            elif char_type == 'raw':
                output.append(value)


def parse_metadata(text: str) -> tuple:
    """Parse metadata from text file comments."""

    metadata = {}
    lines = text.split('\n')
    clean_lines = []

    for line in lines:
        if line.startswith('#'):
            match = METADATA_PATTERN.match(line)
            if match:
                key = match.group(1)
                value = match.group(2).strip()
                metadata[key] = value
            continue
        clean_lines.append(line)

    clean_text = '\n'.join(clean_lines)
    return metadata, clean_text


class TextTranspiler:
    """Transpiles text files to GNU assembler syntax for modding builds"""

    def __init__(self, bank_name: str = "textBank"):
        self.bank_name = bank_name
        self.index_name = f"{bank_name}Index"

        self.data_lines: List[str] = []
        self.index_lines: List[str] = []

        self.segment_offsets: List[int] = []
        self.current_offset = 0
        self.verbose = False
        self.errors: List[str] = []
        self.warnings: List[str] = []

    def transpile_directory(self, input_dir: Path,
                           auto_textend: bool = True) -> Tuple[str, str]:
        """
        Transpile a directory of text files to assembly.

        Args:
            input_dir: Directory containing text files (text000.txt, text001.txt, etc.)
            auto_textend: Automatically add [TEXTEND] if missing

        Returns:
            Tuple of (data_asm, index_asm) strings
        """
        if not input_dir.exists() or not input_dir.is_dir():
            self.errors.append(f"Input directory does not exist: {input_dir}")
            return "", ""

        txt_files = sorted(input_dir.glob("text*.txt"))

        if not txt_files:
            self.errors.append(f"No text files found in {input_dir}")
            return "", ""

        encoder = TextEncoder()
        encoder.verbose = self.verbose
        encoder.auto_textend = auto_textend

        segments: List[Tuple[int, bytes]] = []

        for txt_file in txt_files:
            try:
                idx = int(txt_file.stem.replace('text', ''))
            except ValueError:
                self.warnings.append(f"Cannot parse index from filename: {txt_file.name}")
                continue

            with open(txt_file, 'r', encoding='utf-8') as f:
                text = f.read()

            metadata, clean_text = parse_metadata(text)

            try:
                binary_data = encoder.encode_text(clean_text, metadata)
                segments.append((idx, binary_data))

                if self.verbose:
                    print(f"  {txt_file.name}: {len(binary_data)} bytes")

            except Exception as e:
                self.errors.append(f"Error compiling {txt_file.name}: {e}")
                segments.append((idx, b''))

        segments.sort(key=lambda x: x[0])

        self._generate_data_file(segments)
        self._generate_index_file(segments)

        data_output = '\n'.join(self.data_lines) + '\n'
        index_output = '\n'.join(self.index_lines) + '\n'

        return data_output, index_output

    def _emit_data(self, text: str = ""):
        self.data_lines.append(text)

    def _emit_index(self, text: str = ""):
        self.index_lines.append(text)

    def _generate_data_file(self, segments: List[Tuple[int, bytes]]):
        """Generate the text data assembly file"""

        self._emit_data(f"# Auto-generated text data: {self.bank_name}")
        self._emit_data("# Generated by hm64_text_transpiler_mod.py (modding build)")
        self._emit_data()
        self._emit_data(".section .data")
        self._emit_data()
        self._emit_data(f".global {self.bank_name}")
        self._emit_data(f"{self.bank_name}:")
        self._emit_data()

        self.current_offset = 0
        self.segment_offsets = []

        if not segments:
            self._emit_data("# Empty text bank - 16-byte aligned padding")
            self._emit_data("    .balign 16")
            self._emit_data("    .space  16")
            self.current_offset = 16
            self._emit_data()
        else:
            for idx, binary_data in segments:
                self.segment_offsets.append((idx, self.current_offset))

                self._emit_data(f"# Text segment {idx} (offset 0x{self.current_offset:04X})")
                self._emit_data(f"{self.bank_name}_text_{idx}:")

                for i in range(0, len(binary_data), 16):
                    chunk = binary_data[i:i+16]
                    hex_bytes = ', '.join(f'0x{b:02X}' for b in chunk)
                    self._emit_data(f"    .byte   {hex_bytes}")

                self.current_offset += len(binary_data)
                self._emit_data()

        self._emit_data(f"# End of {self.bank_name}")
        self._emit_data(f"# Total size: {self.current_offset} bytes")

    def _generate_index_file(self, segments: List[Tuple[int, bytes]]):
        """Generate the index/offset table assembly file"""

        self._emit_index(f"# Auto-generated text index: {self.index_name}")
        self._emit_index("# Index built from text files (modding build)")
        self._emit_index("# Last entry points to end of text data")
        self._emit_index()
        self._emit_index(".section .data")
        self._emit_index()
        self._emit_index(f".global {self.index_name}")
        self._emit_index(f"{self.index_name}:")

        if not self.segment_offsets:
            self._emit_index("# Empty text bank - 16-byte aligned padding")
            self._emit_index("    .balign 16")
            self._emit_index("    .space  16")
            self._emit_index()
            return

        # Create offset map from segments
        offset_map = {idx: offset for idx, offset in self.segment_offsets}

        # Add end-of-data entry (the key feature for modding builds)
        max_text_idx = max(offset_map.keys()) if offset_map else 0
        end_of_data_idx = max_text_idx + 1
        offset_map[end_of_data_idx] = self.current_offset

        if self.verbose:
            print(f"  Added end-of-data index entry {end_of_data_idx} -> 0x{self.current_offset:04X}")

        max_idx = max(offset_map.keys())

        # Emit entries for all indices 0 to max
        for i in range(max_idx + 1):
            if i in offset_map:
                offset = offset_map[i]
                note = " (end of text data)" if i == end_of_data_idx else ""
                self._emit_index(f"    .word   {offset}  # text {i}{note}")
            else:
                self._emit_index(f"    .word   0  # text {i} (undefined)")

        self._emit_index()
        self._emit_index(f"# End of {self.index_name}")


def transpile_text_bank(input_dir: Path, output_dir: Path = None,
                        bank_name: str = None, verbose: bool = False,
                        auto_textend: bool = True) -> Tuple[Optional[Path], Optional[Path]]:
    """
    Transpile a text bank directory to assembly files.

    Args:
        input_dir: Directory containing text files
        output_dir: Output directory for .s files (default: same as input)
        bank_name: Name for the bank symbols (default: derived from directory)
        verbose: Enable verbose output
        auto_textend: Automatically add [TEXTEND] if missing

    Returns:
        Tuple of (data_path, index_path) for generated files
    """

    if bank_name is None:
        bank_name = input_dir.name.replace('-', '_').replace(' ', '_')

    if output_dir is None:
        output_dir = input_dir

    output_dir = Path(output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)

    transpiler = TextTranspiler(bank_name=bank_name)
    transpiler.verbose = verbose

    data_asm, index_asm = transpiler.transpile_directory(
        input_dir,
        auto_textend=auto_textend
    )

    for warning in transpiler.warnings:
        print(f"WARNING: {warning}", file=sys.stderr)
    for error in transpiler.errors:
        print(f"ERROR: {error}", file=sys.stderr)

    if transpiler.errors:
        return None, None

    data_path = output_dir / f"{bank_name}.s"
    index_path = output_dir / f"{bank_name}Index.s"

    with open(data_path, 'w') as f:
        f.write(data_asm)
    print(f"Wrote text data to {data_path}", file=sys.stderr)

    with open(index_path, 'w') as f:
        f.write(index_asm)
    print(f"Wrote text index to {index_path}", file=sys.stderr)

    return data_path, index_path


def main():
    parser = argparse.ArgumentParser(
        description='HM64 Text Transpiler (Modding Version) - Compile text files to GNU assembler',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Compiles text files to assembly for modding builds.

The index table includes an end-of-data entry pointing to the end of all
text data, allowing the game to know where text content ends.

Examples:
  # Transpile text files to assembly
  %(prog)s assets/text/library -n libraryText -o assets/text/

  This generates two files:
    - libraryText.s       (text data)
    - libraryTextIndex.s  (offset table with end-of-data entry)

"""
    )

    parser.add_argument('input', type=Path,
                        help='Input directory containing text files')
    parser.add_argument('-o', '--output', type=Path,
                        help='Output directory (default: same as input)')
    parser.add_argument('-n', '--name',
                        help='Bank name for symbols (default: derived from directory)')
    parser.add_argument('-v', '--verbose', action='store_true',
                        help='Enable verbose output')
    parser.add_argument('--no-auto-textend', action='store_true',
                        help='Disable automatic [TEXTEND] insertion')

    args = parser.parse_args()

    auto_textend = not args.no_auto_textend

    data_path, index_path = transpile_text_bank(
        args.input,
        output_dir=args.output,
        bank_name=args.name,
        verbose=args.verbose,
        auto_textend=auto_textend
    )
    sys.exit(0 if data_path else 1)


if __name__ == '__main__':
    main()
