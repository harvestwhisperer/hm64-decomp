"""
Text Extractor for HM64 Build System.

Extracts text data from ROM into editable text files that can be
reassembled during the build process.

Extracted format:
    assets/text/<bank_name>/
        _metadata.txt           # Bank metadata (addresses, terminators)
        text000.txt             # Decoded text segment 0
        text001.txt             # Decoded text segment 1
        ...

Usage:
    python -m hm64.text.extractor extract_all
    python -m hm64.text.extractor extract <bank_name>
    python -m hm64.text.extractor list_banks
"""

import argparse
import re
from pathlib import Path
from typing import Dict, List, Optional

import numpy as np

from ..common import rom
from . import addresses
from .charmap import (
    CHAR_MAP, REVERSE_CHAR_MAP, CONTROL_CODES,
    GAMEVAR_MACROS, CHARACTER_AVATAR_MACROS,
    BIT_MASKS
)


# Default output directory
_REPO_DIR = Path(__file__).resolve().parent.parent.parent.parent
DEFAULT_OUTPUT_DIR = _REPO_DIR / "assets" / "text"


# Message box types by line width
MESSAGE_BOX_TYPES = {
    'DEFAULT': 16,      # Main dialogue box
    'MENU': 10,         # Overlay menus, lists
    'NAMING_SCREEN': 6, # Naming screen input
    'SELECTION': 16,    # Pink dialogue selection boxes
}

LINE_WIDTH_TO_TYPE = {v: k for k, v in MESSAGE_BOX_TYPES.items()}


class TextDecoder:
    """Decodes HM64 text data from ROM format."""

    def __init__(self, modding: bool = False):
        self.position = 0
        self.char_counter = 0
        self.control_byte = 0
        self._current_data = b''
        self.modding = modding

    def decode_stream(self, byte_data: bytes) -> List:
        """
        Decode a byte stream into a list of decoded items.

        Args:
            byte_data: Raw bytes from ROM

        Returns:
            List of decoded items (characters, control codes, etc.)
        """
        if isinstance(byte_data, str):
            byte_data = bytes.fromhex(byte_data.replace(' ', ''))
        elif isinstance(byte_data, list):
            byte_data = bytes(byte_data)

        self.position = 0
        self.char_counter = 0
        self.control_byte = 0
        self._current_data = byte_data
        result = []

        while self.position < len(byte_data):
            decoded_value = self._decode_next_character(byte_data)
            if decoded_value is not None:
                result.append(decoded_value)

        return result

    def _decode_next_character(self, data: bytes):
        """Decode the next character from the data stream."""
        if self.position >= len(data):
            return None

        # Read new control byte every 8 characters
        if self.char_counter % 8 == 0:
            if self.position >= len(data):
                return None
            self.control_byte = data[self.position]
            self.position += 1

        if self.position >= len(data):
            return None

        # Check if this character position needs 2-byte encoding
        bit_index = self.char_counter % 8
        needs_two_bytes = (self.control_byte & BIT_MASKS[bit_index]) != 0

        if needs_two_bytes:
            # Read 2 bytes for extended character
            if self.position + 1 >= len(data):
                if self.position < len(data):
                    remaining_byte = data[self.position]
                    self.position += 1
                    self.char_counter += 1
                    return f'[CHAR:0x{remaining_byte:02X}]'
                return None

            byte1 = data[self.position]
            byte2 = data[self.position + 1]
            self.position += 2

            # Byteswap 16-bit values
            result = byte1 | (byte2 << 8)

            if result in CHAR_MAP:
                decoded = CHAR_MAP[result]
            elif result == 0:
                return None
            else:
                decoded = f'[WORD:0x{result:04X}]'
        else:
            # Read 1 byte
            byte_val = data[self.position]
            self.position += 1
            result = byte_val
            decoded = self._decode_character(result)

        self.char_counter += 1
        return decoded

    def _extract_parameter(self, control_code: int) -> Optional[int]:
        """Extract parameter bytes for control codes that need them."""
        if control_code == 5:  # LOAD_TEXT - 2-byte little-endian
            if self.position + 1 < len(self._current_data):
                byte1 = self._current_data[self.position]
                byte2 = self._current_data[self.position + 1]
                self.position += 2
                return byte1 | (byte2 << 8)

        elif control_code in [3, 7, 9]:  # WAIT, INSERT_GAMEVAR, CHARACTER_AVATAR
            if self.position < len(self._current_data):
                param = self._current_data[self.position]
                self.position += 1
                return param

        return None

    def _decode_character(self, value: int):
        """Decode a single character value."""
        if value in CONTROL_CODES:
            control_name = CONTROL_CODES[value]
            param = self._extract_parameter(value)

            if param is not None:
                return {'type': 'control', 'code': value, 'name': control_name, 'parameter': param}
            else:
                return {'type': 'control', 'code': value, 'name': control_name}

        elif value in CHAR_MAP:
            return {'type': 'character', 'value': CHAR_MAP[value]}
        else:
            font_index = value - 0xB if value >= 0xB else value
            return {'type': 'unknown', 'raw': value, 'font_index': font_index}

    def decode_and_format(self, byte_data: bytes) -> str:
        """Decode and format as readable text."""
        decoded = self.decode_stream(byte_data)
        result = []
        seen_textend = False
        post_textend_bytes = [] if not self.modding else None

        for item in decoded:
            if isinstance(item, dict):
                if item['type'] == 'control':
                    if 'parameter' in item:
                        if seen_textend:
                            if not self.modding:
                                post_textend_bytes.append(item['code'])
                        else:
                            if item['name'] == 'INSERT_GAMEVAR' and item['parameter'] in GAMEVAR_MACROS:
                                result.append(f"[{GAMEVAR_MACROS[item['parameter']]}]")
                            elif item['name'] == 'CHARACTER_AVATAR' and item['parameter'] in CHARACTER_AVATAR_MACROS:
                                result.append(f"[CHARACTER_AVATAR:{CHARACTER_AVATAR_MACROS[item['parameter']]}]")
                            elif item['name'] in ('WAIT', 'CHARACTER_AVATAR', 'LOAD_TEXT'):
                                result.append(f"[{item['name']}:{item['parameter']}]")
                            else:
                                result.append(f"[{item['name']}:{item['parameter']:02X}]")
                    else:
                        if item['name'] == 'LINEBREAK':
                            if seen_textend:
                                if not self.modding:
                                    post_textend_bytes.append(0)
                                continue
                            result.append('\n')
                        elif item['name'] == 'TEXTEND':
                            seen_textend = True
                            if self.modding:
                                break
                        else:
                            if seen_textend:
                                if not self.modding:
                                    post_textend_bytes.append(item['code'])
                            else:
                                result.append(f"[{item['name']}]")

                elif item['type'] == 'character':
                    if seen_textend:
                        if not self.modding:
                            char = item['value']
                            if char in REVERSE_CHAR_MAP:
                                post_textend_bytes.append(REVERSE_CHAR_MAP[char])
                    else:
                        result.append(item['value'])

                elif item['type'] == 'unknown':
                    if seen_textend:
                        if not self.modding:
                            post_textend_bytes.append(item['raw'])
                    else:
                        result.append(f"[CHAR:0x{item['raw']:02X}→0x{item['font_index']:02X}]")

            else:
                result.append(str(item))

        if not self.modding and post_textend_bytes:
            hex_str = ''.join(f'{b:02X}' for b in post_textend_bytes)
            result.append(f'[BLOCK_END:{hex_str}]')

        return ''.join(result)


def get_offset_array(index_start: int, index_end: int) -> np.ndarray:
    """Get offset array from ROM, trimming trailing zeros."""
    rom_data = rom.get_rom()
    num_entries = (index_end - index_start) // 4

    offsets = np.frombuffer(rom_data, dtype=np.dtype(">u4"), count=num_entries, offset=index_start)

    # Trim trailing zeros
    nonzeros = np.nonzero(offsets)[0]
    last = nonzeros[-1] if len(nonzeros) > 0 else 0

    return offsets[:last + 1]


def set_text_segments(index_start: int, index_end: int, text_start: int) -> Dict:
    """Calculate text segment information from offset table."""
    text_end = index_start
    max_valid_offset = index_start - text_start

    index_size = index_end - index_start
    index_count = index_size // 4

    offsets = get_offset_array(index_start, index_end)

    segments = []
    for idx, offset in enumerate(offsets):
        start_address = text_start + offset

        if idx + 1 < len(offsets):
            next_offset = offsets[idx + 1]
            if next_offset > max_valid_offset:
                next_offset = max_valid_offset
            next_start = text_start + next_offset
            size = next_start - start_address
        else:
            size = text_end - start_address

        is_terminator = offset >= max_valid_offset

        if size < 0:
            size = 0

        segments.append({
            'index': idx,
            'start_address': start_address,
            'size': size,
            'offset': int(offset),
            'is_terminator': is_terminator
        })

    return {
        'index_count': index_count,
        'segments': segments,
        'index_start': index_start,
        'index_end': index_end,
        'text_start': text_start,
        'text_end': text_end
    }


def infer_line_width(decoded_text: str) -> int:
    """Infer line width from decoded text."""
    text = decoded_text.replace('[SOFTBREAK]', '\n')
    control_pattern = re.compile(r'\[[A-Z_0-9]+(?::[0-9]+)?\]')

    max_width = 0
    for line in text.split('\n'):
        clean_line = control_pattern.sub('', line)
        char_count = len(clean_line)
        if char_count > max_width:
            max_width = char_count

    return max_width if max_width > 0 else 16


def infer_message_box_type(decoded_text: str) -> str:
    """Infer MESSAGE_BOX_TYPE from line widths."""
    line_width = infer_line_width(decoded_text)

    if line_width in LINE_WIDTH_TO_TYPE:
        return LINE_WIDTH_TO_TYPE[line_width]

    if line_width <= 6:
        return 'NAMING_SCREEN'
    elif line_width <= 10:
        return 'MENU'
    else:
        return 'DEFAULT'


def decode_text_bank(index_start: int, index_end: int, text_start: int,
                     modding: bool = False) -> List[Dict]:
    """
    Decode a text bank from ROM.

    Args:
        index_start: ROM address of index table start
        index_end: ROM address of index table end
        text_start: ROM address of text data start
        modding: Enable modding mode (skip junk data, no metadata)

    Returns:
        List of decoded text segment dictionaries
    """
    rom_data = rom.get_rom()
    bank_info = set_text_segments(index_start, index_end, text_start)
    decoder = TextDecoder(modding=modding)
    decoded_texts = []

    for segment in bank_info['segments']:
        if segment.get('is_terminator', False):
            decoded_texts.append({
                'index': segment['index'],
                'start_address': f"0x{segment['start_address']:08X}",
                'size': 0,
                'actual_size': 0,
                'offset': segment['offset'],
                'is_terminator': True,
                'decoded_text': '',
                'raw_data': ''
            })
            continue

        segment_data = rom_data[segment['start_address']:segment['start_address'] + segment['size']]
        actual_size = len(segment_data)

        # Check for all-zero padding segments
        if actual_size > 0 and all(b == 0 for b in segment_data):
            decoded_texts.append({
                'index': segment['index'],
                'start_address': f"0x{segment['start_address']:08X}",
                'size': segment['size'],
                'actual_size': actual_size,
                'offset': segment['offset'],
                'is_padding': True,
                'decoded_text': '',
                'raw_data': segment_data[:50].hex()
            })
            continue

        try:
            decoded = decoder.decode_and_format(segment_data)

            # Check for undecodable data
            needs_rawbytes = False
            if re.search(r'\[CHAR:0x[0-9A-Fa-f]{2}\]', decoded) or re.search(r'\[WORD:0x[0-9A-Fa-f]{4}\]', decoded):
                needs_rawbytes = True

            # For small segments, verify round-trip to catch malformed control bytes
            # This is needed for junk data segments that decode but don't re-encode correctly
            # Skip in modding mode - we'll just skip junk segments entirely
            if not modding and not needs_rawbytes and len(segment_data) <= 16:
                try:
                    from .transpiler import TextEncoder
                    test_encoder = TextEncoder()
                    test_encoder.auto_textend = True
                    reencoded = test_encoder.encode_text(decoded)
                    original_trimmed = bytes(segment_data).rstrip(b'\x00')
                    reencoded_trimmed = reencoded.rstrip(b'\x00')
                    if original_trimmed != reencoded_trimmed:
                        needs_rawbytes = True
                except Exception:
                    pass  # If transpiler not available, skip round-trip check

            if needs_rawbytes:
                decoded = f'[RAWBYTES:{segment_data.hex()}]\n# Decoded interpretation: {decoded}'

            decoded_texts.append({
                'index': segment['index'],
                'start_address': f"0x{segment['start_address']:08X}",
                'size': segment['size'],
                'actual_size': actual_size,
                'offset': segment['offset'],
                'decoded_text': decoded,
                'raw_data': segment_data[:50].hex() + ('...' if segment['size'] > 50 else '')
            })

        except Exception as e:
            decoded_texts.append({
                'index': segment['index'],
                'start_address': f"0x{segment['start_address']:08X}",
                'size': segment['size'],
                'actual_size': actual_size,
                'offset': segment['offset'],
                'error': str(e),
                'raw_data': segment_data[:50].hex() + ('...' if segment['size'] > 50 else '')
            })

    return decoded_texts


def extract_text_bank(bank: addresses.TextBankInfo, output_base: Path,
                     modding: bool = False) -> bool:
    """
    Extract a text bank to output directory.

    Args:
        bank: TextBankInfo with addresses
        output_base: Base output directory
        modding: Enable modding mode (skip junk data, no metadata)

    Returns:
        True if successful
    """
    print(f"  Extracting: {bank.name}")

    output_path = output_base / bank.name
    output_path.mkdir(parents=True, exist_ok=True)

    try:
        decoded_texts = decode_text_bank(bank.index_start, bank.index_end, bank.text_start, modding=modding)

        # Get max segment index for junk detection
        max_segment_index = max(t['index'] for t in decoded_texts) if decoded_texts else 0

        files_written = 0
        for text_info in decoded_texts:
            # Skip terminator segments
            if text_info.get('is_terminator', False):
                continue

            # Handle padding segments differently in modding mode
            if text_info.get('is_padding', False):
                if modding:
                    # Write placeholder file
                    filename = f"text{text_info['index']:03d}.txt"
                    file_path = output_path / filename
                    with open(file_path, 'w', encoding='utf-8') as f:
                        f.write("# PLACEHOLDER: Empty segment for index alignment\n")
                    files_written += 1
                continue

            # Skip junk data in modding mode
            if modding and bank.name in addresses.JUNK_DATA_BANKS:
                if text_info['index'] == max_segment_index:
                    print(f"    Skipping junk data segment {text_info['index']}")
                    continue

            filename = f"text{text_info['index']:03d}.txt"
            file_path = output_path / filename

            with open(file_path, 'w', encoding='utf-8') as f:
                decoded_text = text_info.get('decoded_text', '')

                # Check for selection menu
                if addresses.is_selection_segment(bank.name, text_info['index']):
                    box_type = 'SELECTION'
                else:
                    box_type = infer_message_box_type(decoded_text)
                f.write(f"# MESSAGE_BOX_TYPE: {box_type}\n")

                f.write(f"# Text Segment {text_info['index']}\n")
                f.write(f"# Address: {text_info['start_address']}\n")
                f.write(f"# Size: {text_info['size']} bytes\n")

                if 'error' in text_info:
                    f.write(f"# ERROR: {text_info['error']}\n")

                f.write(decoded_text)

            files_written += 1

        # Write metadata file (matching mode only)
        if not modding:
            metadata_path = output_path / "_metadata.txt"
            with open(metadata_path, 'w', encoding='utf-8') as f:
                f.write("# Text Bank Metadata\n")
                f.write(f"# INDEX_START: 0x{bank.index_start:08X}\n")
                f.write(f"# INDEX_END: 0x{bank.index_end:08X}\n")
                f.write(f"# TEXT_START: 0x{bank.text_start:08X}\n")
                f.write(f"# TOTAL_SEGMENTS: {len(decoded_texts)}\n")
                f.write("\n")

                terminators = [t for t in decoded_texts if t.get('is_terminator', False)]
                if terminators:
                    f.write("# Terminator segments:\n")
                    for term in terminators:
                        f.write(f"TERMINATOR: {term['index']}\n")
                    f.write("\n")

                padding_segments = [t for t in decoded_texts if t.get('is_padding', False)]
                if padding_segments:
                    f.write("# Padding segments:\n")
                    for pad in padding_segments:
                        f.write(f"PADDING: {pad['index']} SIZE: {pad['size']}\n")

        print(f"    Extracted: {files_written} text files")
        return True

    except Exception as e:
        print(f"    Error: {e}")
        import traceback
        traceback.print_exc()
        return False


def extract_all(output_dir: Path = DEFAULT_OUTPUT_DIR, modding: bool = False):
    """Extract all text banks."""
    banks = addresses.get_all_text_banks()
    print(f"Found {len(banks)} text banks to extract")

    success = 0
    for bank in banks:
        if extract_text_bank(bank, output_dir, modding=modding):
            success += 1

    print(f"\nExtracted {success}/{len(banks)} text banks")


def extract_one(name: str, output_dir: Path = DEFAULT_OUTPUT_DIR,
                modding: bool = False):
    """Extract a single text bank by name."""
    bank = addresses.get_text_bank_by_name(name)

    if bank is None:
        print(f"Text bank not found: {name}")
        print("Available banks:")
        for b in addresses.get_all_text_banks():
            print(f"  - {b.name}")
        return

    extract_text_bank(bank, output_dir, modding=modding)


def list_banks():
    """List all available text banks."""
    banks = addresses.get_all_text_banks()

    if not banks:
        print("No text banks found in CSV file.")
        return

    print(f"Text banks ({len(banks)} total):")
    for i, bank in enumerate(banks):
        print(f"  {i+1:2d}. {bank.name:25s} game:{bank.game_index:02X} "
              f"Index: 0x{bank.index_start:08X}-0x{bank.index_end:08X} "
              f"Text: 0x{bank.text_start:08X}")


def main():
    parser = argparse.ArgumentParser(description="Extract text assets for HM64")
    parser.add_argument('command', choices=['extract_all', 'extract', 'list_banks'],
                        help='Command to run')
    parser.add_argument('name', nargs='?', help='Bank name (for extract command)')
    parser.add_argument('--output', '-o', type=str, default=str(DEFAULT_OUTPUT_DIR),
                        help='Output directory')
    parser.add_argument('--modding', action='store_true',
                        help='Enable modding mode: skip junk data, no metadata, human-readable output')

    args = parser.parse_args()
    output_dir = Path(args.output)

    if args.command == 'extract_all':
        extract_all(output_dir, modding=args.modding)
    elif args.command == 'extract':
        if not args.name:
            print("Error: extract requires a bank name")
            list_banks()
        else:
            extract_one(args.name, output_dir, modding=args.modding)
    elif args.command == 'list_banks':
        list_banks()


if __name__ == '__main__':
    main()
