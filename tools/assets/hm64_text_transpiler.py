#!/usr/bin/env python3
"""
HM64 Text Transpiler

Compiles text files to GNU assembler syntax (.s files) that can be
assembled and linked with the main game.

Generates two files for each text bank:
  - <bank_name>.s       (text data)
  - <bank_name>Index.s  (offset table)

"""

import argparse
import struct
import sys
import re
from pathlib import Path
from typing import List, Tuple, Optional

from hm64_text_utilities import CHAR_MAP, CONTROL_CODES, GAMEVAR_MACRO_TO_INDEX, CHARACTER_AVATAR_MACRO_TO_INDEX

# Create reverse mappings for encoding
REVERSE_CHAR_MAP = {v: k for k, v in CHAR_MAP.items()}
REVERSE_CONTROL_CODES = {v: k for k, v in CONTROL_CODES.items()}

CONTROL_CODE_PATTERN = re.compile(r'\[([A-Z_0-9]+)(?::([A-Za-z_0-9]+))?\]')

RAW_CHAR_PATTERN = re.compile(r'\[CHAR:0x([0-9A-Fa-f]{2})(?:→0x[0-9A-Fa-f]{2})?\]')
RAW_WORD_PATTERN = re.compile(r'\[WORD:0x([0-9A-Fa-f]{4})\]')
RAWBYTES_PATTERN = re.compile(r'\[RAWBYTES:([0-9A-Fa-f]+)\]')
BLOCK_END_PATTERN = re.compile(r'\[BLOCK_END:([0-9A-Fa-f]+)\]')
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
        self.auto_linebreak = False  # Default: exact reproduction; opt-in for auto-wrapping
        self.line_width = 16  # Default to main dialogue box width
        self.auto_textend = True  # Default enabled for user friendliness

    def encode_text(self, text: str, metadata: dict = None) -> bytes:
        """
        Encode a text string into the game's binary format.

        Args:
            text: Text string with control codes like [LINEBREAK], [TEXTEND], etc.
            metadata: Optional metadata dictionary with settings

        Returns:
            Encoded bytes ready to be written to ROM
        """

        # Apply metadata settings if provided
        if metadata:
            # MESSAGE_BOX_TYPE takes precedence over LINE_WIDTH
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
        # Used for mid-stream loaded segments that shouldn't have control bytes
        rawbytes_match = RAWBYTES_PATTERN.match(text.strip())
        if rawbytes_match:
            hex_string = rawbytes_match.group(1)
            raw_data = bytes.fromhex(hex_string)
            # Add padding to align to 4-byte boundary
            padding_needed = (4 - (len(raw_data) % 4)) % 4
            if padding_needed > 0:
                raw_data += b'\x00' * padding_needed
            if self.verbose:
                print(f"RAWBYTES: emitting {len(raw_data)} bytes directly (no compression)")
            return raw_data

        content_check = text
        content_check = re.sub(r'\[LINEBREAK\]', '', content_check)
        content_check = re.sub(r'\[SOFTBREAK\]', '', content_check)
        content_check = content_check.strip()

        if not content_check:
            return b'\x00\x00\x00\x00'

        # Handle linebreaks based on mode
        if self.auto_linebreak:
            # Auto-generate linebreaks based on message box width (for modding)
            text = self._add_automatic_linebreaks(text)
        else:
            # Convert newlines to [LINEBREAK] codes exactly (for matching)
            text = self._convert_newlines_to_linebreaks(text)

        # Extract [BLOCK_END:XX] if present (bytes to place after TEXTEND for byte-matching)
        block_end_bytes = b''
        block_end_match = BLOCK_END_PATTERN.search(text)
        if block_end_match:
            block_end_bytes = bytes.fromhex(block_end_match.group(1))
            text = text[:block_end_match.start()] + text[block_end_match.end():]
            if self.verbose:
                print(f"Extracted BLOCK_END bytes: {block_end_bytes.hex()}")

        if self.auto_textend and text.strip() and not text.endswith('[TEXTEND]'):
            # Don't use rstrip() - it would remove meaningful trailing spaces from the text
            text = text + '[TEXTEND]'

        # Parse the text into a list of character codes
        char_codes = self._parse_text_to_codes(text)

        # Add BLOCK_END bytes as additional codes after TEXTEND
        for byte_val in block_end_bytes:
            char_codes.append(byte_val)

        if self.verbose:
            print(f"Parsed {len(char_codes)} character codes")
            print(f"Codes: {char_codes}")

        # Encode the character codes into compressed binary format
        binary_data = self._encode_codes_to_binary(char_codes)

        # Add padding to align to 4-byte boundary
        padding_needed = (4 - (len(binary_data) % 4)) % 4

        if padding_needed > 0:
            binary_data += b'\x00' * padding_needed
            if self.verbose:
                print(f"Added {padding_needed} padding bytes for 4-byte alignment")

        return binary_data

    def _parse_text_to_codes(self, text: str) -> list:
        """
        Parse text string into a list of character codes.
        Handles control codes in square brackets.
        """

        codes = []
        i = 0

        while i < len(text):

            # Check for raw 1-byte codes first: [CHAR:0xXX] or [CHAR:0xXX→0xYY]
            # The first hex value (XX) is the original ROM byte to preserve
            char_match = RAW_CHAR_PATTERN.match(text, i)
            if char_match:
                raw_byte = int(char_match.group(1), 16)
                codes.append(raw_byte)  # Add as 1-byte code
                if self.verbose:
                    print(f"Found raw CHAR: 0x{raw_byte:02X}")
                i = char_match.end()
                continue

            # Check for raw 2-byte codes: [WORD:0xXXXX]
            # Preserves 2-byte value (encoder will set control bit for 2-byte)
            word_match = RAW_WORD_PATTERN.match(text, i)
            if word_match:
                raw_word = int(word_match.group(1), 16)
                codes.append(raw_word)  # Add as 2-byte code (>= 0x100)
                if self.verbose:
                    print(f"Found raw WORD: 0x{raw_word:04X}")
                i = word_match.end()
                continue

            # Check for control codes
            match = CONTROL_CODE_PATTERN.match(text, i)

            if match:
                control_name = match.group(1)
                param_str = match.group(2)

                # Check for gamevar macros first (e.g., [PLAYER_NAME] -> INSERT_GAMEVAR:00)
                if control_name in GAMEVAR_MACRO_TO_INDEX:
                    gamevar_index = GAMEVAR_MACRO_TO_INDEX[control_name]
                    codes.append(7)  # INSERT_GAMEVAR control code
                    codes.append(('param1', gamevar_index))

                    if self.verbose:
                        print(f"Found gamevar macro: {control_name} -> INSERT_GAMEVAR:{gamevar_index:02X}")

                    i = match.end()
                    continue

                elif control_name in REVERSE_CONTROL_CODES:
                    control_code = REVERSE_CONTROL_CODES[control_name]
                    codes.append(control_code)

                    # Add parameter if present
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

                        # Determine parameter size based on control code
                        if control_code == 5:  # LOAD_TEXT - 2-byte parameter
                            codes.append(('param2', param))
                        elif control_code in [3, 7, 9]:  # WAIT, INSERT_GAMEVAR, CHARACTER_AVATAR - 1-byte parameter
                            codes.append(('param1', param))

                    if self.verbose:
                        print(f"Found control code: {control_name} = {control_code}")

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
        """
        Convert newlines in text to [LINEBREAK] codes for exact reproduction.
        Used when AUTO_LINEBREAK is false (default for matching).
        """
        # Replace Windows line endings first
        text = text.replace('\r\n', '\n')

        # Remove everything after [TEXTEND] (decoder artifacts from reading padding)
        textend_pos = text.find('[TEXTEND]')
        if textend_pos >= 0:
            text = text[:textend_pos + len('[TEXTEND]')]

        # Remove newlines immediately after [LINEBREAK] or [SOFTBREAK] (redundant)
        text = re.sub(r'(\[(?:LINEBREAK|SOFTBREAK)\])\n', r'\1', text)

        # Convert all newlines to [LINEBREAK] - preserve trailing ones for matching
        text = text.replace('\n', '[LINEBREAK]')

        if self.verbose:
            print(f"Converted newlines to [LINEBREAK] codes")

        return text

    def _add_automatic_linebreaks(self, text: str) -> str:
        """
        Automatically add line breaks to text based on line width.
        Uses word wrapping to avoid breaking words unnecessarily.

        Preserves existing control codes.
        """

        if self.verbose:
            print(f"Auto line-breaking with width {self.line_width}")

        lines = []
        current_line = []
        current_length = 0

        # Split into tokens (words and control codes)
        tokens = []
        i = 0
        current_word = []

        while i < len(text):
            # Check for control code
            match = CONTROL_CODE_PATTERN.match(text, i)

            if match:
                # Save current word if any
                if current_word:
                    tokens.append(('word', ''.join(current_word)))
                    current_word = []

                # Save control code
                tokens.append(('control', match.group(0)))
                i = match.end()

            elif text[i] in ' \t':
                # Whitespace - save current word
                if current_word:
                    tokens.append(('word', ''.join(current_word)))
                    current_word = []
                tokens.append(('space', text[i]))
                i += 1

            elif text[i] in '\r\n':
                # Treat newlines as whitespace (for readability in source files)
                # Actual linebreaks are auto-generated based on line width
                if current_word:
                    tokens.append(('word', ''.join(current_word)))
                    current_word = []
                tokens.append(('space', ' '))
                i += 1

            else:
                # Regular character - add to current word
                current_word.append(text[i])
                i += 1

        # Save last word
        if current_word:
            tokens.append(('word', ''.join(current_word)))

        # Now process tokens into lines
        for token_type, token_value in tokens:

            if token_type == 'control':
                # Control codes don't count toward line length
                # But linebreak control codes should reset the line
                if token_value == '[LINEBREAK]' or token_value == '[SOFTBREAK]':
                    if current_line:
                        lines.append(''.join(current_line))
                    current_line = [token_value]
                    current_length = 0
                else:
                    current_line.append(token_value)

            elif token_type == 'space':
                # Add space if we have room
                if current_length + 1 <= self.line_width:
                    current_line.append(token_value)
                    current_length += 1
                # Otherwise skip it (will naturally happen at line break)

            elif token_type == 'word':
                word_length = len(token_value)

                # If adding this word would exceed line width
                if current_length + word_length > self.line_width:
                    # If the word itself is too long, we have to break it
                    if word_length > self.line_width:
                        # Add what we can
                        remaining = self.line_width - current_length
                        if remaining > 0:
                            current_line.append(token_value[:remaining])
                            lines.append(''.join(current_line))
                            token_value = token_value[remaining:]
                            current_line = []
                            current_length = 0

                        # Break the rest into chunks
                        while len(token_value) > self.line_width:
                            lines.append(token_value[:self.line_width] + '[LINEBREAK]')
                            token_value = token_value[self.line_width:]

                        if token_value:
                            current_line = [token_value]
                            current_length = len(token_value)
                    else:
                        # Start new line for this word
                        if current_line:
                            current_line.append('[LINEBREAK]')
                            lines.append(''.join(current_line))
                        current_line = [token_value]
                        current_length = word_length
                else:
                    # Word fits on current line
                    current_line.append(token_value)
                    current_length += word_length

        # Add last line
        if current_line:
            lines.append(''.join(current_line))

        result = ''.join(lines)

        if self.verbose:
            print(f"After line breaking:\n{result}\n")

        return result

    def _encode_codes_to_binary(self, codes: list) -> bytes:
        """
        Encode character codes into compressed binary format.

        The format uses control bytes every 8 characters:
        - Each bit in control byte determines if character is 1 or 2 bytes
        - Bit set (1) = 2-byte character (little-endian)
        - Bit clear (0) = 1-byte character
        """

        output = bytearray()
        char_count = 0
        control_byte = 0
        control_byte_bits = []
        pending_chars = []

        i = 0

        while i < len(codes):
            code = codes[i]

            # Handle parameter bytes (don't count towards 8-char block boundary)
            if isinstance(code, tuple):

                param_type, param_value = code

                if param_type == 'param2':
                    # 2-byte parameter (little-endian)
                    pending_chars.append(('raw', param_value & 0xFF))
                    pending_chars.append(('raw', (param_value >> 8) & 0xFF))
                elif param_type == 'param1':
                    # 1-byte parameter
                    pending_chars.append(('raw', param_value & 0xFF))

                i += 1
                continue

            # Every 8 bytes, write the control byte and data
            if char_count > 0 and char_count % 8 == 0:
                self._write_control_block(output, control_byte, pending_chars)
                control_byte = 0
                control_byte_bits = []
                pending_chars = []

            # Determine if this character needs 1 or 2 bytes
            bit_position = char_count % 8

            if code >= 0x100:
                # 2-byte character
                control_byte_bits.append(1)
                control_byte |= (0x80 >> bit_position)
                pending_chars.append(('char2', code))
            else:
                # 1-byte character
                control_byte_bits.append(0)
                pending_chars.append(('char1', code))

            char_count += 1
            i += 1

        # Write any remaining data
        if pending_chars:
            self._write_control_block(output, control_byte, pending_chars)

        return bytes(output)

    def _write_control_block(self, output: bytearray, control_byte: int, chars: list):
        """Write a control byte followed by its character data"""

        # Write control byte
        output.append(control_byte)

        if self.verbose:
            print(f"Control byte: 0x{control_byte:02X}")

        # Write character data
        for char_type, value in chars:
            if char_type == 'char1':
                output.append(value)
                if self.verbose:
                    print(f"  1-byte char: 0x{value:02X}")
            elif char_type == 'char2':
                # Little-endian 2-byte value
                output.append(value & 0xFF)
                output.append((value >> 8) & 0xFF)
                if self.verbose:
                    print(f"  2-byte char: 0x{value:04X} -> 0x{value & 0xFF:02X} 0x{(value >> 8) & 0xFF:02X}")
            elif char_type == 'raw':
                # Raw parameter byte
                output.append(value)
                if self.verbose:
                    print(f"  Raw param byte: 0x{value:02X}")

def parse_metadata(text: str) -> tuple:
    """
    Parse metadata from text file comments.

    Returns:
        (metadata_dict, clean_text)
    """

    metadata = {}
    lines = text.split('\n')
    clean_lines = []

    for line in lines:
        # Check for metadata comments
        if line.startswith('#'):
            match = METADATA_PATTERN.match(line)
            if match:
                key = match.group(1)
                value = match.group(2).strip()
                metadata[key] = value
            # Skip all comment lines (metadata and regular comments)
            continue
        clean_lines.append(line)

    # Preserve leading/trailing newlines - they represent LINEBREAK codes
    clean_text = '\n'.join(clean_lines)
    return metadata, clean_text

class TextTranspiler:
    """Transpiles text files to GNU assembler syntax"""

    def __init__(self, bank_name: str = "textBank"):

        self.bank_name = bank_name
        self.index_name = f"{bank_name}Index"

        # Output buffers
        self.data_lines: List[str] = []
        self.index_lines: List[str] = []

        # Tracking
        self.segment_offsets: List[int] = []
        self.current_offset = 0
        self.verbose = False
        self.errors: List[str] = []
        self.warnings: List[str] = []
        self.modding_mode = False  # When True, ignore metadata and add end-of-data index entry

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

        # Read metadata file (contains terminator and padding info for index building)
        # In modding mode, we skip metadata entirely and build index from text files only
        terminator_indices = []
        padding_segments = {}  # idx -> size
        total_segments = 0

        metadata_path = input_dir / "_metadata.txt"
        if not self.modding_mode and metadata_path.exists():
            with open(metadata_path, 'r', encoding='utf-8') as f:
                for line in f:
                    line = line.strip()
                    if line.startswith('# TOTAL_SEGMENTS:'):
                        try:
                            total_segments = int(line.split(':')[1].strip())
                        except (ValueError, IndexError):
                            pass
                    elif line.startswith('TERMINATOR:'):
                        try:
                            idx = int(line.split(':')[1].strip())
                            terminator_indices.append(idx)
                        except (ValueError, IndexError):
                            pass
                    elif line.startswith('PADDING:'):
                        # Format: PADDING: {index} SIZE: {size}
                        try:
                            parts = line.split()
                            idx = int(parts[1])
                            size = int(parts[3])
                            padding_segments[idx] = size
                        except (ValueError, IndexError):
                            pass
            if self.verbose and terminator_indices:
                print(f"  Found terminator segments: {terminator_indices}")
            if self.verbose and padding_segments:
                print(f"  Found padding segments: {padding_segments}")
        elif self.modding_mode and self.verbose:
            print(f"  Modding mode: ignoring metadata, building index from text files only")

        txt_files = sorted(input_dir.glob("text*.txt"))

        # Check if we have anything to process (text files OR padding/terminator segments)
        if not txt_files and not padding_segments and not terminator_indices:
            self.errors.append(f"No text files or metadata found in {input_dir}")
            return "", ""

        # Handle empty text bank (only padding/terminators, no actual text)
        if not txt_files:
            if self.verbose:
                print(f"  No text files - generating empty bank with {len(padding_segments)} padding and {len(terminator_indices)} terminator segments")
            segments: List[Tuple[int, bytes]] = []
            # Add padding segments
            for idx, size in padding_segments.items():
                segments.append((idx, b'\x00' * size))
            segments.sort(key=lambda x: x[0])
            # Generate assembly output
            self._generate_data_file(segments, padding_segments)
            self._generate_index_file(segments, terminator_indices, list(padding_segments.keys()))
            data_output = '\n'.join(self.data_lines) + '\n'
            index_output = '\n'.join(self.index_lines) + '\n'
            return data_output, index_output

        encoder = TextEncoder()
        encoder.verbose = self.verbose
        encoder.auto_textend = auto_textend

        # Compile all text segments
        segments: List[Tuple[int, bytes]] = []

        for txt_file in txt_files:

            # Extract index from filename (text000.txt -> 0)
            try:
                idx = int(txt_file.stem.replace('text', ''))
            except ValueError:
                self.warnings.append(f"Cannot parse index from filename: {txt_file.name}")
                continue

            # Read and compile text
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

        # Add padding segments (all-zero segments with no text files)
        for idx, size in padding_segments.items():
            segments.append((idx, b'\x00' * size))
            if self.verbose:
                print(f"  padding segment {idx}: {size} bytes (zeros)")

        # Sort segments by index
        segments.sort(key=lambda x: x[0])

        # Generate assembly output
        self._generate_data_file(segments, padding_segments)
        self._generate_index_file(segments, terminator_indices, list(padding_segments.keys()))

        data_output = '\n'.join(self.data_lines) + '\n'
        index_output = '\n'.join(self.index_lines) + '\n'

        return data_output, index_output

    def _emit_data(self, text: str = ""):
        """Emit a line to data output"""
        self.data_lines.append(text)

    def _emit_index(self, text: str = ""):
        """Emit a line to index output"""
        self.index_lines.append(text)

    def _generate_data_file(self, segments: List[Tuple[int, bytes]], padding_segments: dict = None):
        """Generate the text data assembly file"""

        if padding_segments is None:
            padding_segments = {}

        # Header
        self._emit_data(f"# Auto-generated text data: {self.bank_name}")
        self._emit_data("# Generated by hm64_text_transpiler.py")
        self._emit_data()
        self._emit_data(".section .data")
        self._emit_data()
        self._emit_data(f".global {self.bank_name}")
        self._emit_data(f"{self.bank_name}:")
        self._emit_data()

        self.current_offset = 0
        self.segment_offsets = []

        # Handle empty bank case - emit 16-byte aligned zeros
        if not segments:
            self._emit_data("# Empty text bank - 16-byte aligned padding")
            self._emit_data("    .balign 16")
            self._emit_data("    .space  16")
            self.current_offset = 16
            self._emit_data()
        else:
            for idx, binary_data in segments:

                self.segment_offsets.append((idx, self.current_offset))

                # Check if this is a padding segment (all zeros, no text file)
                if idx in padding_segments:
                    size = padding_segments[idx]
                    self._emit_data(f"# Padding segment {idx} (offset 0x{self.current_offset:04X})")
                    self._emit_data(f"{self.bank_name}_text_{idx}:")
                    self._emit_data(f"    .space  {size}")
                    self.current_offset += size
                else:
                    self._emit_data(f"# Text segment {idx} (offset 0x{self.current_offset:04X})")
                    self._emit_data(f"{self.bank_name}_text_{idx}:")

                    # Emit bytes in groups of 16 for readability
                    for i in range(0, len(binary_data), 16):
                        chunk = binary_data[i:i+16]
                        hex_bytes = ', '.join(f'0x{b:02X}' for b in chunk)
                        self._emit_data(f"    .byte   {hex_bytes}")

                    self.current_offset += len(binary_data)

                self._emit_data()

        # Footer
        self._emit_data(f"# End of {self.bank_name}")
        self._emit_data(f"# Total size: {self.current_offset} bytes")

    def _generate_index_file(self, segments: List[Tuple[int, bytes]], terminator_indices: List[int] = None, padding_indices: List[int] = None):
        """Generate the index/offset table assembly file"""

        if terminator_indices is None:
            terminator_indices = []
        if padding_indices is None:
            padding_indices = []

        # Header
        self._emit_index(f"# Auto-generated text index: {self.index_name}")
        if self.modding_mode:
            self._emit_index("# MODDING MODE: Index built from text files only (no metadata)")
        self._emit_index("# Offsets are pre-computed at transpile time (big-endian)")
        self._emit_index()
        self._emit_index(".section .data")
        self._emit_index()
        self._emit_index(f".global {self.index_name}")
        self._emit_index(f"{self.index_name}:")

        if not self.segment_offsets and not terminator_indices:
            # Empty bank - emit 16-byte aligned zeros for index
            self._emit_index("# Empty text bank - 16-byte aligned padding")
            self._emit_index("    .balign 16")
            self._emit_index("    .space  16")
            self._emit_index()
            return

        # Create a lookup dict for offsets
        offset_map = {idx: offset for idx, offset in self.segment_offsets}

        if self.modding_mode:
            # In modding mode, add a final entry pointing to end of text data
            # This serves as the terminator and allows the game to know where text data ends
            max_text_idx = max(offset_map.keys()) if offset_map else 0
            end_of_data_idx = max_text_idx + 1
            offset_map[end_of_data_idx] = self.current_offset
            if self.verbose:
                print(f"  Modding mode: Added end-of-data index entry {end_of_data_idx} -> 0x{self.current_offset:04X}")
        else:
            # Add terminator entries - they point to the end of all text data
            # (which is self.current_offset after all text is compiled)
            for term_idx in terminator_indices:
                offset_map[term_idx] = self.current_offset

        # Find max segment index to know table size
        max_idx = max(offset_map.keys()) if offset_map else 0

        # Emit entries for all indices 0 to max
        # Using .word for 4-byte big-endian values
        for i in range(max_idx + 1):
            if i in offset_map:
                offset = offset_map[i]
                note = ""
                if self.modding_mode and i == max_idx:
                    note = " (end of text data)"
                elif i in terminator_indices:
                    note = " (terminator)"
                elif i in padding_indices:
                    note = " (padding)"
                self._emit_index(f"    .word   {offset}  # text {i}{note}")
            else:
                # Missing segment - emit 0 placeholder
                self._emit_index(f"    .word   0  # text {i} (undefined)")

        self._emit_index()
        self._emit_index(f"# End of {self.index_name}")


def transpile_text_bank(input_dir: Path, output_dir: Path = None,
                        bank_name: str = None, verbose: bool = False,
                        auto_textend: bool = True,
                        modding: bool = False) -> Tuple[Optional[Path], Optional[Path]]:
    """
    Transpile a text bank directory to assembly files.

    Args:
        input_dir: Directory containing text files
        output_dir: Output directory for .s files (default: same as input)
        bank_name: Name for the bank symbols (default: derived from directory)
        verbose: Enable verbose output
        auto_textend: Automatically add [TEXTEND] if missing
        modding: Enable modding mode (ignore metadata, add end-of-data index entry)

    Returns:
        Tuple of (data_path, index_path) for generated files
    """

    if bank_name is None:
        # Derive bank name from directory name
        bank_name = input_dir.name.replace('-', '_').replace(' ', '_')

    if output_dir is None:
        output_dir = input_dir

    output_dir = Path(output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)

    transpiler = TextTranspiler(bank_name=bank_name)
    transpiler.verbose = verbose
    transpiler.modding_mode = modding

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


def verify_roundtrip(input_dir: Path, rom_path: Path,
                     index_start: int, index_end: int, text_start: int,
                     verbose: bool = False) -> bool:
    """
    Verify that recompiled text matches original ROM data.

    Args:
        input_dir: Directory containing text files
        rom_path: Path to the base ROM
        index_start: ROM address of index table start
        index_end: ROM address of index table end
        text_start: ROM address of text data start
        verbose: Enable verbose output

    Returns:
        True if data matches, False otherwise
    """

    rom_data = rom_path.read_bytes()

    text_end = index_start

    original_text = rom_data[text_start:text_end]
    original_index = rom_data[index_start:index_end]

    txt_files = sorted(input_dir.glob("text*.txt"))

    if not txt_files:
        print(f"No text files found in {input_dir}")
        return False

    encoder = TextEncoder()
    encoder.auto_textend = True

    compiled_segments = []
    offsets = []
    current_offset = 0
    
    for txt_file in txt_files:
        with open(txt_file, 'r', encoding='utf-8') as f:
            text = f.read()
        
        metadata, clean_text = parse_metadata(text)
        
        try:
            binary_data = encoder.encode_text(clean_text, metadata)
            offsets.append(current_offset)
            compiled_segments.append(binary_data)
            current_offset += len(binary_data)
        except Exception as e:
            print(f"Error compiling {txt_file.name}: {e}")
            return False
    
    # Combine compiled text data
    compiled_text = b''.join(compiled_segments)
    
    # Build compiled index table (4-byte big-endian offsets)
    compiled_index = b''.join(struct.pack('>I', off) for off in offsets)
    
    # Compare
    text_match = compiled_text == original_text[:len(compiled_text)]
    index_match = compiled_index == original_index[:len(compiled_index)]
    
    if verbose:
        print(f"Original text size: {len(original_text)} bytes")
        print(f"Compiled text size: {len(compiled_text)} bytes")
        print(f"Original index size: {len(original_index)} bytes")
        print(f"Compiled index size: {len(compiled_index)} bytes")
    
    if not text_match:
        print("Text data MISMATCH!")
        # Find first difference
        for i in range(min(len(compiled_text), len(original_text))):
            if compiled_text[i] != original_text[i]:
                print(f"  First difference at offset 0x{i:04X}:")
                print(f"    Original: 0x{original_text[i]:02X}")
                print(f"    Compiled: 0x{compiled_text[i]:02X}")
                # Show context
                start = max(0, i - 8)
                end = min(len(original_text), i + 8)
                print(f"    Original context: {original_text[start:end].hex()}")
                print(f"    Compiled context: {compiled_text[start:end].hex()}")
                break
        return False
    
    if not index_match:
        print("Index data MISMATCH!")
        return False
    
    print("MATCH: Compiled data matches original ROM data")
    return True


def main():
    parser = argparse.ArgumentParser(
        description='HM64 Text Transpiler - Compile text files to GNU assembler',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Commands:
  transpile  - Convert text files to GNU assembler syntax (two files)
  verify     - Verify compiled output matches original ROM data
  
Examples:
  # Transpile text files to assembly
  %(prog)s transpile assets/text/libraryText -n libraryText -o src/text/
  
  This generates two files:
    - libraryText.s       (text data)
    - libraryTextIndex.s  (offset table)

"""
    )

    subparsers = parser.add_subparsers(dest='command', help='Command to execute')

    transpile_parser = subparsers.add_parser('transpile',
                                              help='Compile text to assembly')
    transpile_parser.add_argument('input', type=Path,
                                   help='Input directory containing text files')
    transpile_parser.add_argument('-o', '--output', type=Path,
                                   help='Output directory (default: same as input)')
    transpile_parser.add_argument('-n', '--name',
                                   help='Bank name for symbols (default: derived from directory)')
    transpile_parser.add_argument('-v', '--verbose', action='store_true',
                                   help='Enable verbose output')
    transpile_parser.add_argument('--no-auto-textend', action='store_true',
                                   help='Disable automatic [TEXTEND] insertion')
    transpile_parser.add_argument('--modding', action='store_true',
                                   help='Enable modding mode: ignore _metadata.txt, build index from text files only, add end-of-data entry')

    verify_parser = subparsers.add_parser('verify',
                                           help='Verify compiled output matches ROM')
    verify_parser.add_argument('input', type=Path,
                                help='Input directory containing text files')
    verify_parser.add_argument('--rom', type=Path, required=True,
                                help='Path to base ROM')
    verify_parser.add_argument('--index-start', type=str, required=True,
                                help='Index table start address (hex)')
    verify_parser.add_argument('--index-end', type=str, required=True,
                                help='Index table end address (hex)')
    verify_parser.add_argument('--text-start', type=str, required=True,
                                help='Text data start address (hex)')
    verify_parser.add_argument('-v', '--verbose', action='store_true',
                                help='Enable verbose output')

    args = parser.parse_args()

    if args.command == 'transpile':
        auto_textend = not args.no_auto_textend

        data_path, index_path = transpile_text_bank(
            args.input,
            output_dir=args.output,
            bank_name=args.name,
            verbose=args.verbose,
            auto_textend=auto_textend,
            modding=args.modding
        )
        sys.exit(0 if data_path else 1)

    elif args.command == 'verify':
        index_start = int(args.index_start, 16)
        index_end = int(args.index_end, 16)
        text_start = int(args.text_start, 16)

        success = verify_roundtrip(
            args.input,
            args.rom,
            index_start,
            index_end,
            text_start,
            verbose=args.verbose
        )

        sys.exit(0 if success else 1)

    else:
        parser.print_help()
        sys.exit(1)


if __name__ == '__main__':
    main()