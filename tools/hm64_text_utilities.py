import argparse
import csv
import json
import numpy as np
import os
import sys
    
from pathlib import Path

ROM_PATH = Path(__file__).parent / "../baserom.us.z64"
CSV_PATH = Path(__file__).parent / "text_addresses.csv"

VERBOSE = "--verbose" in sys.argv

rom = None

def set_rom():
    global rom
    rom = memoryview(ROM_PATH.read_bytes())

# (Game strings use these values -0xB, which are indices into the font texture array)
CHAR_MAP = {
    0xAC: 'A',
    0xAD: 'B', 
    0xAE: 'C',
    0xAF: 'D',
    0xB0: 'E',
    0xB1: 'F',
    0xB2: 'G',
    0xB3: 'H',
    0xB4: 'I',
    0xB5: 'J',
    0xB6: 'K',
    0xB7: 'L',
    0xB8: 'M',
    0xB9: 'N',
    0xBA: 'O',
    0xBB: 'P',
    0xBC: 'Q',
    0xBD: 'R',
    0xBE: 'S',
    0xBF: 'T',
    0xC0: 'U',
    0xC1: 'V',
    0xC2: 'W',
    0xC3: 'X',
    0xC4: 'Y',
    0xC5: 'Z',
    0xC6: 'a',
    0xC7: 'b',
    0xC8: 'c',
    0xC9: 'd',
    0xCA: 'e',
    0xCB: 'f',
    0xCC: 'g',
    0xCD: 'h',
    0xCE: 'i',
    0xCF: 'j',
    0xD0: 'k',
    0xD1: 'l',
    0xD2: 'm',
    0xD3: 'n',
    0xD4: 'o',
    0xD5: 'p',
    0xD6: 'q',
    0xD7: 'r',
    0xD8: 's',
    0xD9: 't',
    0xDA: 'u',
    0xDB: 'v',
    0xDC: 'w',
    0xDD: 'x',
    0xDE: 'y',
    0xDF: 'z',
    0xE0: '1',
    0xE1: '2',
    0xE2: '3',
    0xE3: '4',
    0xE4: '5',
    0xE5: '6',
    0xE6: '7',
    0xE7: '8',
    0xE8: '9',
    0xE9: '0',
    0xEA: '?',
    0xEB: '!', 
    0xEC: '-',
    0xEF: ',',
    0xF0: '.',
    0xF1: '/',
    0xF4: '&',
    0xF5: '♡', 
    0xF7: ' Ა',
    0xF9: ' ',
    0x101: '—',
    0x103: ':',
    0x108: '"',
    0x10A: ';', 
    0x10B: '(',
    0x10C: ")",
    0x10E: "'",
}

CONTROL_CODES = {
    0: 'LINEBREAK',
    1: 'SOFTBREAK', 
    2: 'TEXTEND',
    3: 'WAITINPUT',
    4: 'WAIT_WITH_ICON',
    5: 'LOAD_TEXT',  # Followed by 2-byte text index
    6: 'RESERVED',
    7: 'INSERT_GAMEVAR', # Followed by 1-byte string index
    8: 'WAIT_VARIANT',
    9: 'CHARACTER_AVATAR', # Followed by 1-byte animation index
}

BIT_MASKS = [0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01]

def get_offset_array(index_start, index_end):

    if rom is None:
        set_rom()

    num_entries = int((index_end - index_start) // 4)

    offsets = np.frombuffer(rom, dtype=np.dtype(">u4"), count=num_entries, offset=index_start)

    # handle offset arrays that have 0 padding at the end

    # get indices of all non-zero elements 
    nonzeros = np.nonzero(offsets)[0]
    # get last non-zero index
    last = nonzeros[-1] if len(nonzeros) > 0 else 0
    
    # trim original array
    return offsets[:last+1]

def set_text_segments(index_start: int, index_end: int, text_start: int) -> dict:

    # Text data ends at the start of its own index table
    text_end = index_start
    
    if VERBOSE == True:
        print(f"Text bank layout:")
        print(f"  Index: 0x{index_start:08X} - 0x{index_end:08X}")
        print(f"  Text:  0x{text_start:08X} - 0x{text_end:08X}")
    
    index_size = index_end - index_start
    index_count = index_size // 4
    
    if VERBOSE == True:
        print(f"  Index size: {index_size} bytes, {index_count} entries")
    
    offsets = get_offset_array(index_start, index_end)
    
    # Calculate start addresses and sizes for each valid text segment
    segments = []
    for idx, offset in enumerate(offsets):

        offset = offset
        
        start_address = text_start + offset
        
        # Calculate size by getting difference between segments or between text_end and last segment
        if idx + 1 < len(offsets):
            next_offset = offsets[idx + 1]
            next_start = text_start + next_offset
            size = next_start - start_address
        else:
            # Last segment goes to the start of the index table
            size = text_end - start_address
            
        if size <= 0:
            print(f"  WARNING: Segment {idx} has invalid size {size} bytes")
            
        if VERBOSE == True:    
            print(f"  Segment {idx} (index {idx}): 0x{start_address:08X}, size {size} bytes")
            
        segments.append({
            'index': idx,
            'start_address': start_address,
            'size': size,
            'offset': offset
        })
    
    return {
        'index_count': index_count,
        'segments': segments,
        'index_start': index_start,
        'index_end': index_end,
        'text_start': text_start,
        'text_end': text_end
    }

class TextDecoder:

    def __init__(self):
        self.position = 0
        self.char_counter = 0
        self.control_byte = 0
        
    def decode_stream(self, byte_data):

        if isinstance(byte_data, str):
            # Convert hex string to bytes
            byte_data = bytes.fromhex(byte_data.replace(' ', ''))

        elif isinstance(byte_data, list):
            byte_data = bytes(byte_data)
            
        self.position = 0
        self.char_counter = 0
        self.control_byte = 0
        self._current_data = byte_data  # Store reference for parameter extraction
        result = []
        
        while self.position < len(byte_data):

            decoded_value = self._decode_next_character(byte_data)
            
            if decoded_value is not None:
                result.append(decoded_value)

                # Check if this is a segment-ending control code
                if isinstance(decoded_value, dict) and decoded_value.get('segment_end'):
                    if VERBOSE == True:
                        print(f"    Decode stopped at WAITINPUT at position {self.position}/{len(byte_data)}")
                    break
            
            else:
                if VERBOSE == True:
                    # stop if next character can't be decoded
                    print(f"    Decode stopped at position {self.position}/{len(byte_data)}")
                    
                    # Show the last few bytes for debugging
                    start_pos = max(0, self.position - 10)
                    end_pos = min(len(byte_data), self.position + 10)
                    context_bytes = byte_data[start_pos:end_pos]
                    
                    print(f"    Context bytes around position {self.position}: {context_bytes.hex()}")

                break
                
        return result
    
    def _decode_next_character(self, data):

        if self.position >= len(data):
            if VERBOSE == True:
                print(f"    EOF reached at position {self.position}")
            return None
            
        # Read new control byte every 8 characters
        if self.char_counter % 8 == 0:

            if self.position >= len(data):
                if VERBOSE == True:
                    print(f"    EOF while reading control byte at position {self.position}")
                return None

            self.control_byte = data[self.position]
            
            if VERBOSE == True:
                print(f"    Char {self.char_counter}: New control byte 0x{self.control_byte:02X} at position {self.position}")
            
            self.position += 1
            
        if self.position >= len(data):
            if VERBOSE == True:
                print(f"    EOF after control byte at position {self.position}")
            return None
            
        # Check if this character position needs 2-byte encoding
        bit_index = self.char_counter % 8
        needs_two_bytes = (self.control_byte & BIT_MASKS[bit_index]) != 0
        
        if VERBOSE == True:
            print(f"    Char {self.char_counter}: bit_index={bit_index}, needs_two_bytes={needs_two_bytes}")
        
        if needs_two_bytes:
            
            # Read 2 bytes for control command
            if self.position + 1 >= len(data):
                print(f"    Not enough data for 2-byte read at position {self.position}, ending decode")
                return None

            byte1 = data[self.position]
            byte2 = data[self.position + 1]

            self.position += 2

            # byteswap 16-bit values
            result = byte1 | (byte2 << 8) 
            
            if result in CHAR_MAP:
                decoded = CHAR_MAP[result]
                if VERBOSE == True:
                    print(f"    Read 2-byte character: 0x{result:04X} -> '{decoded}'")

            elif result == 0:
                if VERBOSE == True:
                    print(f"    Found null terminator (0x0000), ending decode")
                return None
                
            else:
                decoded = f'[CMD:0x{result:04X}]'
                if VERBOSE == True:
                    print(f"    Read 2-byte command: 0x{result:04X}")
                

        else:
            # Read 1 byte
            byte_val = data[self.position]
            self.position += 1
    
            result = byte_val
            decoded = self._decode_character(result)
    
            if VERBOSE == True:
                print(f"    Read 1-byte character: 0x{result:02X} -> {decoded}")
            
        self.char_counter += 1
        
        # sanity check if addresses are messed up
        if self.char_counter > 5000:
            print(f"    ERROR: Processed {self.char_counter} characters - likely incorrect segment size")
            print(f"    Current position: {self.position}/{len(data)} bytes")
            return None
            
        return decoded
    
    def _extract_parameter(self, control_code):
        """Extract parameter bytes that follow certain control codes"""

        if VERBOSE == True:
            print(f"      Extracting parameter for control code {control_code}")
        
        if control_code == 5:  # LOAD_TEXT - 2-byte little-endian parameter
        
            if hasattr(self, '_current_data') and self.position + 1 < len(self._current_data):
                byte1 = self._current_data[self.position]
                byte2 = self._current_data[self.position + 1]
                self.position += 2
                # DON'T increment char_counter here - it messes up control byte timing
                param = byte1 | (byte2 << 8)  # Little endian
    
                if VERBOSE == True:
                    print(f"      Extracted 2-byte parameter: 0x{param:04X}")
    
                return param    
        
        elif control_code in [7, 9]:  # INSERT_GAMEVAR, CHAR_ANIMATION - 1-byte parameter
        
            if hasattr(self, '_current_data') and self.position < len(self._current_data):
    
                param = self._current_data[self.position]
                self.position += 1
    
                # don't increment char_counter here
    
                if VERBOSE == True:
                    print(f"      Extracted 1-byte parameter: 0x{param:02X}")
    
                return param
        
        return None
    
    def _decode_character(self, value):

        if value in CONTROL_CODES:

            control_name = CONTROL_CODES[value]

            # Extract parameter if this control code needs one
            param = self._extract_parameter(value)

            if param is not None:
                return {'type': 'control', 'code': value, 'name': control_name, 'parameter': param}
            else:
                # WAITINPUT marks the end of current segment
                if value == 3:  # WAITINPUT
                    return {'type': 'control', 'code': value, 'name': control_name, 'segment_end': True}
                else:
                    return {'type': 'control', 'code': value, 'name': control_name}
            
        elif value in CHAR_MAP:
            return {'type': 'character', 'value': CHAR_MAP[value]}
        else:
            # Show both the raw value and after -0xB
            font_index = value - 0xB if value >= 0xB else value
            return {'type': 'unknown', 'raw': value, 'font_index': font_index}
    
    def decode_and_format(self, byte_data):
        """Decode and format as readable text with enhanced control code handling"""

        decoded = self.decode_stream(byte_data)
        result = []
        
        for item in decoded:
        
            if isinstance(item, dict):
        
                if item['type'] == 'control':
        
                    if 'parameter' in item:
                        result.append(f"[{item['name']}:{item['parameter']:02X}]")
                    else:
                        if item['name'] == 'LINEBREAK':
                            result.append('\n')
                        elif item['name'] == 'SOFTBREAK':
                            result.append('\n')
                        elif item['name'] == 'TEXTEND':
                            result.append('\n[END]')
                        else:
                            result.append(f"[{item['name']}]")
        
                elif item['type'] == 'character':
                    result.append(item['value'])
        
                elif item['type'] == 'unknown':
                    result.append(f"[CHAR:0x{item['raw']:02X}→0x{item['font_index']:02X}]")
        
            else:
                # Fallback for old format
                result.append(str(item))
        
        return ''.join(result)

def decode_text_bank(index_start: int, index_end: int, text_start: int) -> list:

    if rom is None:
        set_rom()
    
    bank_info = set_text_segments(index_start, index_end, text_start)
    decoder = TextDecoder()
    decoded_texts = []
    
    for segment in bank_info['segments']:

        if VERBOSE == True:
            print(f"Processing segment {segment['index']}: 0x{segment['start_address']:08X}, calculated size: {segment['size']}")
            
        segment_data = rom[segment['start_address']:segment['start_address'] + segment['size']]
        actual_size = len(segment_data)

        if VERBOSE == True:
            print(f"  Actual segment_data size: {actual_size} bytes")
            print(f"  Last 16 bytes: {segment_data[-16:].hex()}")
            print(f"  First 16 bytes: {segment_data[:16].hex()}")
        
        try:
    
            if VERBOSE == True:
                print(f"  Starting decode for segment {segment['index']}...")
    
            decoded = decoder.decode_and_format(segment_data)
    
            if VERBOSE == True:
                print(f"  Successfully decoded segment {segment['index']}")
            
            decoded_texts.append({
                'index': segment['index'],
                'start_address': f"0x{segment['start_address']:08X}",
                'size': segment['size'],
                'actual_size': actual_size,
                'decoded_text': decoded,
                'raw_data': segment_data[:50].hex() + ('...' if segment['size'] > 50 else '')
            })

        except Exception as e:
            print(f"  Error decoding segment {segment['index']}: {e}")
            decoded_texts.append({
                'index': segment['index'],
                'start_address': f"0x{segment['start_address']:08X}",
                'size': segment['size'],
                'actual_size': actual_size,
                'error': str(e),
                'raw_data': segment_data[:50].hex() + ('...' if segment['size'] > 50 else '')
            })
    
    return decoded_texts

def write_text_bank_to_files(index_start: int, index_end: int, text_start: int, output_dir: str = None) -> str:

    if rom is None:
        set_rom()
    
    if output_dir is None:
        output_dir = f"text-block-0x{text_start:08X}"
    
    assets_path = Path(__file__).parent.parent / "assets" / "text"
    output_path = assets_path / output_dir
    
    output_path.mkdir(parents=True, exist_ok=True)
    
    decoded_texts = decode_text_bank(index_start, index_end, text_start)
    
    for text_info in decoded_texts:

        filename = f"text{text_info['index']:03d}.txt"
        file_path = output_path / filename
        
        with open(file_path, 'w', encoding='utf-8') as f:

            f.write(f"# Text Segment {text_info['index']}\n")
            f.write(f"# Address: {text_info['start_address']}\n")
            f.write(f"# Size: {text_info['size']} bytes\n")
            f.write(f"# Raw data: {text_info['raw_data']}\n")
            f.write("# " + "="*60 + "\n\n")
            
            if 'error' in text_info:
                f.write(f"ERROR: {text_info['error']}\n")
            else:
                f.write(text_info['decoded_text'])

    print(f"Wrote {len(decoded_texts)} text files to directory: {output_path.absolute()}")
    
    return str(output_path.absolute())

def load_text_addresses() -> list:

    if not CSV_PATH.exists():
        print(f"CSV file not found: {CSV_PATH}")
        return []
    
    text_banks = []

    with open(CSV_PATH, 'r', newline='', encoding='utf-8') as csvfile:

        reader = csv.DictReader(csvfile)
        
        for row in reader:

            try:
                index_start = int(row['index_start'], 16)
                index_end = int(row['index_end'], 16)
                text_start = int(row['text_start'], 16)
                game_index = int(row['game_index']) if 'game_index' in row else 0
                
                text_banks.append({
                    'name': row['name'].strip(),
                    'index_start': index_start,
                    'index_end': index_end,
                    'text_start': text_start,
                    'game_index': game_index
                })
                
            except (ValueError, KeyError) as e:
                print(f"Error parsing CSV row {row}: {e}")
                continue
    
    return text_banks

def process_all_text_banks(command: str = 'write_files'):

    text_banks = load_text_addresses()

    if not text_banks:
        print("No text banks found in CSV file.")
        return
    
    if VERBOSE == True:
        print(f"Found {len(text_banks)} text banks in {CSV_PATH}")
    
    for bank in text_banks:

        if VERBOSE == True:
            print(f"\n--- Processing '{bank['name']}' ---")
            print(f"Index: 0x{bank['index_start']:08X} - 0x{bank['index_end']:08X}")
            print(f"Text:  0x{bank['text_start']:08X} - 0x{bank['index_start']:08X}")
        
        try:

            if command == 'write_files':
                output_path = write_text_bank_to_files(bank['index_start'], bank['index_end'], bank['text_start'], bank['name'])
                print(f"Files written to: {output_path}")
                
            elif command == 'decode_bank':
                decoded_texts = decode_text_bank(bank['index_start'], bank['index_end'], bank['text_start'])
                
            elif command == 'analyze_bank':
                set_rom()
                bank_info = set_text_segments(bank['index_start'], bank['index_end'], bank['text_start'])
                
                if VERBOSE == True:
                    print(f"  Total segments: {bank_info['index_count']}")
                
        except Exception as e:
            print(f"Error processing '{bank['name']}': {e}")

def main():

    if len(sys.argv) < 2:
        print("Usage:")
        print("  python hm64_text_utilities.py decode_bank <index_start> <index_end> <text_start>")
        print("  python hm64_text_utilities.py analyze_bank <index_start> <index_end> <text_start>")
        print("  python hm64_text_utilities.py write_files <index_start> <index_end> <text_start> [output_dir]")
        print("  python hm64_text_utilities.py process_all [write_files|decode_bank|analyze_bank]")
        print("  python hm64_text_utilities.py list_banks")
        sys.exit(1)

    cmd = sys.argv[1]

    if cmd == "decode_bank":

        if len(sys.argv) >= 5:

            index_start = int(sys.argv[2], 16)
            index_end = int(sys.argv[3], 16)
            text_start = int(sys.argv[4], 16)
            
            print(f"Decoding text bank:")
            print(f"Index: 0x{index_start:08X} - 0x{index_end:08X}")
            print(f"Text:  0x{text_start:08X}")
            
            decoded_texts = decode_text_bank(index_start, index_end, text_start)
            
            for text_info in decoded_texts:
                
                print(f"\n--- Text {text_info['index']} ---")
                print(f"Address: {text_info['start_address']}")
                print(f"Size: {text_info['size']} bytes")
                
                if 'error' in text_info:
                    print(f"Error: {text_info['error']}")
                else:
                    print(f"Text: {text_info['decoded_text']}")

                print(f"Raw data: {text_info['raw_data']}")
                
        else:
            print("Please provide index_start, index_end, and text_start addresses in hex")
            sys.exit(1)
            
    elif cmd == "analyze_bank":

        if len(sys.argv) >= 5:

            index_start = int(sys.argv[2], 16)
            index_end = int(sys.argv[3], 16)
            text_start = int(sys.argv[4], 16)
            
            set_rom()
            bank_info = set_text_segments(index_start, index_end, text_start)
            
            print(f"Text Bank Analysis:")
            print(f"Index: 0x{index_start:08X} - 0x{index_end:08X}")
            print(f"Text:  0x{text_start:08X}")
            print(f"Total segments: {bank_info['index_count']}")
            print(f"\nSegment breakdown:")
            
            for segment in bank_info['segments']:
                print(f"  {segment['index']:3d}: 0x{segment['start_address']:08X} (offset +0x{segment['offset']:04X}) - {segment['size']:4d} bytes")
                
        else:
            print("Please provide index_start, index_end, and text_start addresses in hex")
            sys.exit(1)
            
    elif cmd == "write_files":

        if len(sys.argv) >= 5:
        
            index_start = int(sys.argv[2], 16)
            index_end = int(sys.argv[3], 16)
            text_start = int(sys.argv[4], 16)
            
            # Optional custom directory name
            output_dir = sys.argv[5] if len(sys.argv) >= 6 else None
            
            print(f"Writing text bank files:")
            print(f"Index: 0x{index_start:08X} - 0x{index_end:08X}")
            print(f"Text:  0x{text_start:08X}")
            
            output_path = write_text_bank_to_files(index_start, index_end, text_start, output_dir)
            print(f"Files written to: {output_path}")
            
        else:
            print("Please provide index_start, index_end, and text_start addresses in hex")
            sys.exit(1)
            
    elif cmd == "process_all":

        command = sys.argv[2] if len(sys.argv) >= 3 else 'write_files'
        
        if command not in ['write_files', 'decode_bank', 'analyze_bank']:
            print(f"Unknown command for process_all: {command}")
            print("Available commands: write_files, decode_bank, analyze_bank")
            sys.exit(1)

        process_all_text_banks(command)
        
    elif cmd == "list_banks":

        text_banks = load_text_addresses()

        if not text_banks:
            print("No text banks found in CSV file.")
        else:
            print(f"Text banks found in {CSV_PATH}:")
            for i, bank in enumerate(text_banks):
                game_idx = f"(game:{bank['game_index']:02X})" if 'game_index' in bank else ""
                print(f"  {i+1:2d}. {bank['name']:15s} {game_idx:10s} Index: 0x{bank['index_start']:08X}-0x{bank['index_end']:08X} Text: 0x{bank['text_start']:08X}-0x{bank['index_start']:08X}")
    
    else:
        print(f"Unknown command: {cmd}")
        print("Available commands: decode_bank, analyze_bank, write_files, process_all, list_banks")

if __name__ == "__main__":
    main()