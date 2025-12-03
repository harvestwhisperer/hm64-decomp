import csv
import io
import json
import numpy as np
import struct
import sys

from pathlib import Path
from typing import List, BinaryIO
from dataclasses import dataclass
from enum import IntEnum

CSV_PATH = Path(__file__).parent / "dialogue_bytecode_addresses.csv"
DIALOGUES_DIR = Path(__file__).parent / "../assets/dialogue-bytecode"
ROM_PATH = Path(__file__).parent / "../baserom.us.z64"
rom = None

VERBOSE = "--verbose" in sys.argv

def set_rom():
    global rom
    if rom is None:
        rom = memoryview(ROM_PATH.read_bytes())

class DialogueOpcode(IntEnum):
    SHOW_TEXT = 0
    DIALOGUE_VARIABLE_BRANCH = 1
    UPDATE_DIALOGUE_VARIABLE = 2
    SET_DIALOGUE_VARIABLE = 3
    SPECIAL_DIALOGUE_BIT_BRANCH = 4
    SET_SPECIAL_DIALOGUE_BIT = 5
    TOGGLE_SPECIAL_DIALOGUE_BIT = 6
    RANDOM_BRANCH = 7
    JUMP_TO_DIALOGUE = 8
    UNUSED = 9
    SHOW_SUBDIALOGUE = 10
    HANDLE_MENU_SELECTION_BRANCH = 11
    END_DIALOGUE = 12

@dataclass
class DialogueInstruction:
    offset: int
    opcode: DialogueOpcode
    args: List[int]
    raw_bytes: bytes
    description: str

def serialize(obj):
    if isinstance(obj, np.integer):
        return int(obj)
    elif isinstance(obj, np.floating):
        return float(obj)
    elif isinstance(obj, np.ndarray):
        return obj.tolist()
    elif isinstance(obj, list):
        return [serialize(item) for item in obj]
    elif isinstance(obj, dict):
        return {key: serialize(value) for key, value in obj.items()}
    else:
        return obj

def get_offset_array(row):

    if rom is None:
        set_rom()

    index_start = int(row[0], 16)
    index_end = int(row[1], 16)

    num_entries = int((index_end - index_start) // 4)

    offsets = np.frombuffer(rom, dtype=np.dtype(">u4"), count=num_entries, offset=index_start)

    # handle offset arrays that have 0 padding at the end

    # get indices of all non-zero elements 
    nonzeros = np.nonzero(offsets)[0]
    # get last non-zero index
    last = nonzeros[-1] if len(nonzeros) > 0 else 0
    
    # trim original array
    return offsets[:last+1]

def parse_all():

    if rom is None:
        set_rom()

    with open(CSV_PATH) as f:
        reader = csv.reader(f)
        addresses_list = list(tuple(line) for line in reader)

    basedir = DIALOGUES_DIR
    basedir.mkdir(parents=True, exist_ok=True)

    for idx, row in enumerate(addresses_list):

        game_index = row[3]
        dir_name = f"dialogue-{game_index}"

        outdir = basedir / dir_name
        outdir.mkdir(parents=True, exist_ok=True)

        block = parse_dialogue_block(row)
        write_block_to_file(game_index, block, outdir)

    return

def write_block_to_file(index, data, outdir):

    json_file = f"dialogue_segment_{index}.json"
    json_path = outdir / json_file
    
    serialized_data = serialize(data)
    
    with open(json_path, 'w') as f:
        json.dump(serialized_data, f, indent=2)

    disasm_file = f"dialogue_segment_{index}_disassembly.txt"
    disasm_path = outdir / disasm_file
    
    disassembly_text = generate_disassembly(serialized_data)
    
    with open(disasm_path, 'w') as f:
        f.write(disassembly_text)

    return   

def parse_dialogue_block(row):

    index_start = int(row[0], 16)
    index_end = int(row[1], 16)
    dialogue_bank_start = int(row[2], 16)
    offsets = get_offset_array(row)
    
    segments = []
    for idx, offset in enumerate(offsets):
        
        segment_size = get_segment_size(idx, offsets, row)
        start_address = dialogue_bank_start + offset

        data_array = get_bytecode_data(start_address, segment_size)
        data = io.BytesIO(data_array)
        
        instructions = parse_bytecode_stream(data)

        segments.append({
                "index": idx,
                "rom_address": f"0x{start_address:X}",
                "offset": f"0x{offset:X}",
                "instructions": [{
                    "offset": instr.offset,
                    "opcode": instr.opcode.value,
                    "opcode_name": instr.opcode.name,
                    "args": instr.args,
                    "raw_bytes": list(instr.raw_bytes),
                    "description": instr.description
                } for instr in instructions]
            })

    return {
        "metadata": {
            "index_start": f"0x{index_start:X}",
            "index_end": f"0x{offset:X}",
            "bytecode_start": f"0x{dialogue_bank_start:X}",
            "num_dialogue_segments": len(segments)
        },
        "segments": segments
    }

def get_bytecode_data(address, segment_size):
    
    if rom is None:
        set_rom()

    return np.frombuffer(rom, dtype=np.dtype("u1"), count=segment_size, offset=address)

def get_segment_size(idx, offsets, row):

    index_start = int(row[0], 16)
    dialogue_start = int(row[2], 16)

    # last index
    if idx == len(offsets) - 1:
        segment_size = index_start - (dialogue_start + offsets[idx])
    else:
        segment_size = (dialogue_start + offsets[idx + 1]) - (dialogue_start + offsets[idx])

    return segment_size

def parse_bytecode_stream(data: BinaryIO) -> List[DialogueInstruction]:

    instructions = []
    offset = 0
    
    while True:

        start_offset = offset
        
        opcode_byte = data.read(1)
        if not opcode_byte:
            break
            
        opcode = opcode_byte[0]
        offset += 1
        
        if opcode >= len(DialogueOpcode):
            if VERBOSE == True:
                print(f"      Warning: Unknown opcode {opcode} at offset {start_offset:04X}")
            continue
        
        opcode_enum = DialogueOpcode(opcode)
        args = []
        raw_bytes = opcode_byte
        
        if opcode_enum == DialogueOpcode.SHOW_TEXT:

            dialogue_bytes = data.read(2)

            if len(dialogue_bytes) < 2:
                break

            text_index = struct.unpack('<H', dialogue_bytes)[0]
            args = [text_index]
            raw_bytes += dialogue_bytes
            offset += 2
            
        elif opcode_enum == DialogueOpcode.DIALOGUE_VARIABLE_BRANCH:

            var_data = data.read(9)
            
            if len(var_data) < 9:
                break

            var_index = var_data[0]
            min_val = struct.unpack('<H', var_data[1:3])[0]
            max_val = struct.unpack('<H', var_data[3:5])[0]
            true_dialogue = struct.unpack('<H', var_data[5:7])[0]
            false_dialogue = struct.unpack('<H', var_data[7:9])[0]
            args = [var_index, min_val, max_val, true_dialogue, false_dialogue]
            raw_bytes += var_data
            offset += 9
            
        elif opcode_enum == DialogueOpcode.UPDATE_DIALOGUE_VARIABLE:

            var_data = data.read(3)
            
            if len(var_data) < 3:
                break

            var_index = var_data[0]
            adjustment = struct.unpack('<h', var_data[1:3])[0]  # signed 16-bit
            args = [var_index, adjustment]
            raw_bytes += var_data
            offset += 3
            
        elif opcode_enum == DialogueOpcode.SET_DIALOGUE_VARIABLE:

            var_data = data.read(3)
            
            if len(var_data) < 3:
                break

            var_index = var_data[0]
            value = struct.unpack('<H', var_data[1:3])[0]
            args = [var_index, value]
            raw_bytes += var_data
            offset += 3

        elif opcode_enum == DialogueOpcode.SPECIAL_DIALOGUE_BIT_BRANCH:

            bit_data = data.read(6)

            if len(bit_data) < 6:
                break

            bit_index = struct.unpack('<H', bit_data[0:2])[0]
            true_dialogue_index = struct.unpack('<H', bit_data[2:4])[0]
            false_dialogue_index = struct.unpack('<H', bit_data[4:6])[0]
            args.extend([bit_index, true_dialogue_index, false_dialogue_index])
            raw_bytes += bit_data
            offset += 6
        
        elif opcode_enum in [DialogueOpcode.SET_SPECIAL_DIALOGUE_BIT,
                            DialogueOpcode.TOGGLE_SPECIAL_DIALOGUE_BIT]:

            bit_data = data.read(2)

            if len(bit_data) < 2:
                break
            
            bit_index = struct.unpack('<H', bit_data)[0]
            args = [bit_index]
            raw_bytes += bit_data
            offset += 2
                
        elif opcode_enum == DialogueOpcode.RANDOM_BRANCH:

            random_data = data.read(6)

            if len(random_data) < 6:
                break
            
            min_val = random_data[0]
            max_val = random_data[1]
            true_dialogue = struct.unpack('<H', random_data[2:4])[0]
            false_dialogue = struct.unpack('<H', random_data[4:6])[0]
            args = [min_val, max_val, true_dialogue, false_dialogue]
            raw_bytes += random_data
            offset += 6
            
        elif opcode_enum == DialogueOpcode.JUMP_TO_DIALOGUE:

            branching_data = data.read(2)
            
            if len(branching_data) < 2:
                break
            
            branching_dialogue_index = struct.unpack('<H', branching_data)[0]
            args = [branching_dialogue_index]
            raw_bytes += branching_data
            offset += 2
            
        elif opcode_enum == DialogueOpcode.SHOW_SUBDIALOGUE:

            text_data = data.read(3)
            
            if len(text_data) < 3:
                break
            
            text_offset = struct.unpack('<H', text_data[0:2])[0]
            unused_field = text_data[2]
            args = [text_offset, unused_field]
            raw_bytes += text_data
            offset += 3
            
        elif opcode_enum == DialogueOpcode.HANDLE_MENU_SELECTION_BRANCH:

            opcode11_data = data.read(3)
            
            if len(opcode11_data) < 3:
                break
            
            unk_18 = opcode11_data[0]
            branching_dialogue_index = struct.unpack('<H', opcode11_data[1:3])[0]
            args = [unk_18, branching_dialogue_index]
            raw_bytes += opcode11_data
            offset += 3
            
        elif opcode_enum in [DialogueOpcode.END_DIALOGUE, DialogueOpcode.UNUSED]:
            pass
        
        description = generate_description(opcode_enum, args)
        
        instruction = DialogueInstruction(
            offset=start_offset,
            opcode=opcode_enum,
            args=args,
            raw_bytes=raw_bytes,
            description=description
        )
        
        instructions.append(instruction)
        
        if opcode_enum == DialogueOpcode.END_DIALOGUE:
            break
    
    return instructions


def generate_description(opcode: DialogueOpcode, args: List[int]) -> str:

    if opcode == DialogueOpcode.SHOW_TEXT:
        return f"Show text {args[0]}"
    elif opcode == DialogueOpcode.DIALOGUE_VARIABLE_BRANCH:
        
        # special value set on the text index field when only branching to another dialogue segment 
        # (most branching goes to another bytecode segment rather than a specific text)
        if args[3] == 65535:
            return f"Branch on dialogue variable {args[0]}: if {args[1]} <= value <= {args[2]} go to bytecode segment {args[4]}"
        else:
            return f"Branch on dialogue variable {args[0]}: if {args[1]} <= value <= {args[2]} go direct to text {args[3]}, else go to bytecode segment {args[4]}"
    
    elif opcode == DialogueOpcode.UPDATE_DIALOGUE_VARIABLE:
        op = "+" if args[1] >= 0 else ""
        return f"Update variable[{args[0]}] {op}{args[1]}"
    elif opcode == DialogueOpcode.SET_DIALOGUE_VARIABLE:
        return f"Set variable[{args[0]}] = {args[1]}"

    elif opcode == DialogueOpcode.SPECIAL_DIALOGUE_BIT_BRANCH:

        if args[1] == 65535:
            return f"Branch on bit {args[0] // 32}: if set go to bytecode segment {args[2]}"
        else:
            return f"Branch on bit {args[0] // 32}: if set go direct to text {args[1]}, else go to bytecode segment {args[2]}"

    elif opcode == DialogueOpcode.SET_SPECIAL_DIALOGUE_BIT:
        # the game uses these bits / 32 as array indices into the bitfield
        return f"Set bit {args[0] // 32}"

    elif opcode == DialogueOpcode.TOGGLE_SPECIAL_DIALOGUE_BIT:
        # the game uses these bits / 32 as array indices into the bitfield
        return f"Toggle bit {args[0] // 32}"

    elif opcode == DialogueOpcode.RANDOM_BRANCH:

        if args[2] == 65535:
            return f"Random branch ({args[0]}-{args[1]}): if in range go to bytecode segment {args[3]}"
        else:
            return f"Random branch ({args[0]}-{args[1]}): if in range go direct to text {args[2]}, else go to bytecode segment {args[3]}"
    
    elif opcode == DialogueOpcode.JUMP_TO_DIALOGUE:
        return f"Branch to dialogue {args[0]}"
    elif opcode == DialogueOpcode.SHOW_SUBDIALOGUE:
        return f"Show sub-dialogue box with text {args[0]}"
    elif opcode == DialogueOpcode.HANDLE_MENU_SELECTION_BRANCH:
        return f"Set unk_18={args[0]}, branching_dialogue={args[1]}"
    elif opcode == DialogueOpcode.END_DIALOGUE:
        return "End dialogue"
    elif opcode == DialogueOpcode.UNUSED:
        return "Unused opcode"
    else:
        return f"Unknown opcode {opcode}"

def generate_disassembly(segments_data):
    lines = []
    lines.append("=" * 80)
    lines.append("")

    for segment in segments_data["segments"]:
        lines.append(f"=== Segment {segment['index']} (ROM: {segment['rom_address']}, Offset: {segment['offset']}) ===")
        lines.append("")

        # Calculate column widths for this segment (different opcodes have shorter or longer parameter counts)
        max_hex_width = 0
        max_opcode_width = 0

        for instr_data in segment["instructions"]:
            raw_bytes = instr_data["raw_bytes"]
            hex_bytes = " ".join(f"{b:02X}" for b in raw_bytes)
            opcode_name = instr_data["opcode_name"]

            max_hex_width = max(max_hex_width, len(hex_bytes))
            max_opcode_width = max(max_opcode_width, len(opcode_name))

        # Add some padding to the calculated widths
        hex_width = max_hex_width + 2
        opcode_width = max_opcode_width + 2

        for instr_data in segment["instructions"]:

            raw_bytes = instr_data["raw_bytes"]
            hex_bytes = " ".join(f"{b:02X}" for b in raw_bytes)

            offset = instr_data["offset"]
            opcode_name = instr_data["opcode_name"]
            description = instr_data["description"]

            lines.append(f"{offset:04X}: {hex_bytes:<{hex_width}} {opcode_name:<{opcode_width}} ; {description}")

            # add spacing for readability
            if opcode_name in ["END_DIALOGUE", "JUMP_TO_DIALOGUE", "START_DIALOGUE"]:
                lines.append("")

        lines.append("")

    return "\n".join(lines)

def json_to_dsl(json_data: dict) -> str:
    """Convert parsed JSON dialogue data to DSL format for editing"""
    lines = []
    
    # Header comment
    if 'metadata' in json_data:
        meta = json_data['metadata']
        lines.append(f"; Dialogue Bank")
        lines.append(f"; Index Start: {meta.get('index_start', 'unknown')}")
        lines.append(f"; Bytecode Start: {meta.get('bytecode_start', 'unknown')}")
        lines.append(f"; Segments: {meta.get('num_dialogue_segments', 'unknown')}")
        lines.append("")
    
    for segment in json_data.get('segments', []):
        seg_idx = segment['index']
        rom_addr = segment.get('rom_address', '')
        instructions = segment.get('instructions', [])
        
        # Detect padding segments: all SHOW_TEXT 0 with no END_DIALOGUE
        # These are just null bytes being parsed as opcodes
        is_padding = (
            len(instructions) > 0 and
            all(
                instr['opcode_name'] == 'SHOW_TEXT' and 
                instr.get('args', [0])[0] == 0 
                for instr in instructions
            )
        )
        
        lines.append(f"; {rom_addr}")
        
        if is_padding:
            lines.append(f".segment_{seg_idx}:  ; alignment padding")
        else:
            lines.append(f".segment_{seg_idx}:")
            
            for instr in instructions:

                opcode_name = instr['opcode_name']
                args = instr.get('args', [])
                
                # Format arguments based on opcode
                if opcode_name == 'SHOW_TEXT':
                    args_str = str(args[0]) if args else ''
                    
                elif opcode_name == 'DIALOGUE_VARIABLE_BRANCH':
                    # var_index, min, max, text_index (true), segment_index (false)
                    true_text = str(args[3]) if args[3] != 65535 else "NO_TEXT"
                    false_seg = f".segment_{args[4]}" if args[4] != 65535 else "NO_BRANCH"
                    args_str = f"{args[0]}, {args[1]}, {args[2]}, {true_text}, {false_seg}"
                    
                elif opcode_name == 'UPDATE_DIALOGUE_VARIABLE':
                    args_str = f"{args[0]}, {args[1]}"
                    
                elif opcode_name == 'SET_DIALOGUE_VARIABLE':
                    args_str = f"{args[0]}, {args[1]}"
                    
                elif opcode_name == 'SPECIAL_DIALOGUE_BIT_BRANCH':
                    # bit_index, text_index (true), segment_index (false)
                    true_text = str(args[1]) if args[1] != 65535 else "NO_TEXT"
                    false_seg = f".segment_{args[2]}" if args[2] != 65535 else "NO_BRANCH"
                    args_str = f"{args[0]}, {true_text}, {false_seg}"
                    
                elif opcode_name in ['SET_SPECIAL_DIALOGUE_BIT', 'TOGGLE_SPECIAL_DIALOGUE_BIT']:
                    args_str = str(args[0]) if args else ''
                    
                elif opcode_name == 'RANDOM_BRANCH':
                    # min, max, text_index (true), segment_index (false)
                    true_text = str(args[2]) if args[2] != 65535 else "NO_TEXT"
                    false_seg = f".segment_{args[3]}" if args[3] != 65535 else "NO_BRANCH"
                    args_str = f"{args[0]}, {args[1]}, {true_text}, {false_seg}"
                    
                elif opcode_name == 'JUMP_TO_DIALOGUE':
                    args_str = f".segment_{args[0]}" if args else ''
                    
                elif opcode_name == 'SHOW_SUBDIALOGUE':
                    args_str = f"{args[0]}, {args[1]}" if len(args) >= 2 else str(args[0]) if args else ''
                    
                elif opcode_name == 'HANDLE_MENU_SELECTION_BRANCH':
                    target_seg = f".segment_{args[1]}" if len(args) > 1 and args[1] != 65535 else "NO_BRANCH"
                    args_str = f"{args[0]}, {target_seg}"
                    
                elif opcode_name in ['END_DIALOGUE', 'UNUSED']:
                    args_str = ''
                    
                else:
                    args_str = ', '.join(str(a) for a in args)
                
                # Emit instruction with proper indentation
                if args_str:
                    lines.append(f"    {opcode_name} {args_str}")
                else:
                    lines.append(f"    {opcode_name}")
        
        lines.append("")
    
    return '\n'.join(lines)

def main():
        
    if len(sys.argv) < 2:
        print("Usage:")
        print("  python hm64_dialogue_utilities.py decode_all")
        print("  python hm64_dialogue_utilities.py to_dsl <json_file> [output_file]")
        sys.exit(1)

    cmd = sys.argv[1]

    if cmd == "decode_all":
        parse_all()
    
    elif cmd == "to_dsl":
        if len(sys.argv) < 3:
            print("Usage: python hm64_dialogue_utilities.py to_dsl <json_file> [output_file]")
            sys.exit(1)
        
        json_file = Path(sys.argv[2])
        output_file = Path(sys.argv[3]) if len(sys.argv) > 3 else None
        
        with open(json_file) as f:
            data = json.load(f)
        
        dsl_output = json_to_dsl(data)
        
        if output_file:
            with open(output_file, 'w') as f:
                f.write(dsl_output)
            print(f"Wrote DSL to {output_file}")
        else:
            print(dsl_output)
        
    else:
        print(f"Unknown command: {cmd}")
        print("Available commands: decode_all, to_dsl")

if __name__ == '__main__':
    main()