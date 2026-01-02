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

from hm64_text_utilities import CHAR_MAP, CONTROL_CODES

# Create reverse mappings for encoding
REVERSE_CHAR_MAP = {v: k for k, v in CHAR_MAP.items()}
REVERSE_CONTROL_CODES = {v: k for k, v in CONTROL_CODES.items()}

CONTROL_CODE_PATTERN = re.compile(r'\[([A-Z_]+)(?::([0-9A-Fa-f]+))?\]')

RAW_CHAR_PATTERN = re.compile(r'\[CHAR:0x([0-9A-Fa-f]{2})(?:→0x[0-9A-Fa-f]{2})?\]')
RAW_WORD_PATTERN = re.compile(r'\[WORD:0x([0-9A-Fa-f]{4})\]')
RAWBYTES_PATTERN = re.compile(r'\[RAWBYTES:([0-9A-Fa-f]+)\]')
METADATA_PATTERN = re.compile(r'^#\s*([A-Z_]+):\s*(.+)$')

class TextEncoder:
    """Encodes text strings into the game's compressed binary format"""

    def __init__(self):
        self.verbose = False
        self.auto_linebreak = False
        self.line_width = 16  # Default to main dialogue box width
        self.auto_textend = True  # Default enabled for user friendliness
        self.convert_newlines = True  # Default: convert \n to [LINEBREAK]

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
            if 'LINE_WIDTH' in metadata:
                self.line_width = int(metadata['LINE_WIDTH'])
            if 'AUTO_LINEBREAK' in metadata:
                self.auto_linebreak = metadata['AUTO_LINEBREAK'].lower() in ['true', 'yes', '1']
            if 'AUTO_TEXTEND' in metadata:
                self.auto_textend = metadata['AUTO_TEXTEND'].lower() in ['true', 'yes', '1']
            if 'CONVERT_NEWLINES' in metadata:
                self.convert_newlines = metadata['CONVERT_NEWLINES'].lower() in ['true', 'yes', '1']

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

        # Preprocess text if auto features are enabled
        if self.convert_newlines and not self.auto_linebreak:
            # Simple newline conversion (auto_linebreak handles this itself)
            text = self._convert_newlines_to_linebreaks(text)

        if self.auto_linebreak:
            text = self._add_automatic_linebreaks(text)

        if self.auto_textend and text.strip() and not text.rstrip().endswith('[TEXTEND]'):
            text = text.rstrip() + '[TEXTEND]'

        # Parse the text into a list of character codes
        char_codes = self._parse_text_to_codes(text)

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

            # Check for control codes
            match = CONTROL_CODE_PATTERN.match(text, i)

            if match:
                control_name = match.group(1)
                param_str = match.group(2)

                if control_name in REVERSE_CONTROL_CODES:
                    control_code = REVERSE_CONTROL_CODES[control_name]
                    codes.append(control_code)

                    # Add parameter if present
                    if param_str is not None:

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

            # Check for raw 1-byte codes: [CHAR:0xXX] or [CHAR:0xXX→0xYY]
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

        # Replace Windows line endings first
        text = text.replace('\r\n', '\n')
        
        # Remove everything after [TEXTEND] (decoder artifacts from reading padding)
        textend_pos = text.find('[TEXTEND]')
        if textend_pos >= 0:
            text = text[:textend_pos + len('[TEXTEND]')]
        
        # Remove newlines immediately after [LINEBREAK] or [SOFTBREAK] (redundant)
        text = re.sub(r'(\[(?:LINEBREAK|SOFTBREAK)\])\n', r'\1', text)
        
        # Remove newlines immediately before [TEXTEND] (formatting artifact)
        text = re.sub(r'\n+(?=\[TEXTEND\])', '', text)
        
        # Remove trailing newlines
        text = text.rstrip('\n')
        
        # Convert remaining newlines to [LINEBREAK]
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
                # Manual line break - save current word and add linebreak
                if current_word:
                    tokens.append(('word', ''.join(current_word)))
                    current_word = []
                tokens.append(('newline', text[i]))
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

            if token_type == 'newline':
                # Manual newline - respect it (but only if not already a [LINEBREAK])
                if not current_line or current_line[-1] != '[LINEBREAK]':
                    current_line.append('[LINEBREAK]')
                # Start new line
                if current_line:
                    lines.append(''.join(current_line))
                current_line = []
                current_length = 0

            elif token_type == 'control':
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

            # Handle parameter bytes (these don't count as characters)
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

            # Every 8 characters, write the control byte and character data
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

        # Write any remaining characters
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

    clean_text = '\n'.join(clean_lines).strip('\n')
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

    def transpile_directory(self, input_dir: Path,
                           auto_textend: bool = True,
                           convert_newlines: bool = True) -> Tuple[str, str]:
        """
        Transpile a directory of text files to assembly.

        Args:
            input_dir: Directory containing text files (text000.txt, text001.txt, etc.)
            auto_textend: Automatically add [TEXTEND] if missing
            convert_newlines: Convert newlines to [LINEBREAK] codes

        Returns:
            Tuple of (data_asm, index_asm) strings
        """
        if not input_dir.exists() or not input_dir.is_dir():
            self.errors.append(f"Input directory does not exist: {input_dir}")
            return "", ""

        # Read metadata file first (contains terminator and padding info)
        terminator_indices = []
        padding_segments = {}  # idx -> size
        total_segments = 0
        metadata_path = input_dir / "_metadata.txt"
        if metadata_path.exists():
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
        encoder.convert_newlines = convert_newlines
        encoder.auto_linebreak = False

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
                if i in terminator_indices:
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
                        convert_newlines: bool = True) -> Tuple[Optional[Path], Optional[Path]]:
    """
    Transpile a text bank directory to assembly files.
    
    Args:
        input_dir: Directory containing text files
        output_dir: Output directory for .s files (default: same as input)
        bank_name: Name for the bank symbols (default: derived from directory)
        verbose: Enable verbose output
        auto_textend: Automatically add [TEXTEND] if missing
        convert_newlines: Convert newlines to [LINEBREAK] codes
        
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

    data_asm, index_asm = transpiler.transpile_directory(
        input_dir, 
        auto_textend=auto_textend,
        convert_newlines=convert_newlines
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
                     verbose: bool = False, literal_input: bool = False) -> bool:
    """
    Verify that recompiled text matches original ROM data.
    
    Args:
        input_dir: Directory containing text files
        rom_path: Path to the base ROM
        index_start: ROM address of index table start
        index_end: ROM address of index table end
        text_start: ROM address of text data start
        verbose: Enable verbose output
        literal_input: If True, text files are in literal mode format
                      (no auto-TEXTEND, no newline conversion)
        
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
    
    if literal_input:
        # Literal mode: text files contain exact control codes including trailing content
        encoder.auto_textend = False
        encoder.convert_newlines = False
    else:
        # Readable mode: auto-add TEXTEND, convert newlines
        encoder.auto_textend = True
        encoder.convert_newlines = True
    
    encoder.auto_linebreak = False
    
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
    transpile_parser.add_argument('--no-convert-newlines', action='store_true',
                                   help='Disable newline to [LINEBREAK] conversion')
    transpile_parser.add_argument('--literal', action='store_true',
                                   help='Literal mode input (equivalent to --no-auto-textend --no-convert-newlines)')

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
    verify_parser.add_argument('--literal', action='store_true',
                                help='Text files are in literal mode (exact control codes, no auto-processing)')

    args = parser.parse_args()

    if args.command == 'transpile':
        # --literal is an alias for `--no-auto-textend --no-convert-newlines`
        if args.literal:
            auto_textend = False
            convert_newlines = False
        else:
            auto_textend = not args.no_auto_textend
            convert_newlines = not args.no_convert_newlines
        
        data_path, index_path = transpile_text_bank(
            args.input,
            output_dir=args.output,
            bank_name=args.name,
            verbose=args.verbose,
            auto_textend=auto_textend,
            convert_newlines=convert_newlines
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
            verbose=args.verbose,
            literal_input=args.literal
        )
        
        sys.exit(0 if success else 1)

    else:
        parser.print_help()
        sys.exit(1)


if __name__ == '__main__':
    main()