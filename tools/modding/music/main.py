import sys
import os
from converter import SeqConverter
from parser import SequenceParser

def main():
    if len(sys.argv) < 2:
        print("N64 MIDI Tools")
        print("=" * 60)
        print("Usage:")
        print("  Parse N64 sequence:")
        print("    python main.py parse <sequence_file> [log_file] [-v|--verbose]")
        print("  Convert MIDI to N64:")
        print("    python main.py convert <midi_file> [output_file] [--gm] [--loop] [--quantize N]")
        print("        output_file  : optional, defaults to <midi_file>.seq")
        print("        --gm         : map General MIDI to in-game instruments")
        print("        --loop       : enable infinite looping")
        print("        --quantize N : quantize to 1/N note grid (4=16th, 8=32nd)")
        print()
        sys.exit(1)
    
    command = sys.argv[1].lower()
    
    if command == "parse":
        if len(sys.argv) < 3:
            print("ERROR: Missing sequence file")
            sys.exit(1)

        input_file = sys.argv[2]

        # Check for verbose flag and determine log file
        verbose = False
        log_file = None

        # Collect all args after the input file
        extra_args = sys.argv[3:]

        # Check for verbose flag
        if '-v' in extra_args or '--verbose' in extra_args:
            verbose = True
            # Remove verbose flags from extra_args
            extra_args = [arg for arg in extra_args if arg not in ('-v', '--verbose')]

        # Determine log file (first non-flag arg, or auto-generate)
        if extra_args:
            log_file = extra_args[0]
        else:
            base_name = os.path.splitext(input_file)[0]
            log_file = base_name + '.txt'

        print(f"Parsing N64 sequence: {input_file}")

        with open(input_file, 'rb') as f:
            data = f.read()

        parser = SequenceParser(data, log_filename=log_file, verbose=verbose)
        parser.parse()

        print(f"Parsing complete! Check {log_file} for output.")
    
    elif command == "convert":
        if len(sys.argv) < 3:
            print("ERROR: Missing input file")
            sys.exit(1)

        midi_file = sys.argv[2]

        # Check if output file is specified or if next arg is a flag
        if len(sys.argv) > 3 and not sys.argv[3].startswith('--'):
            output_file = sys.argv[3]
            extra_args = sys.argv[4:]
        else:
            # Auto-generate output filename from input
            base_name = os.path.splitext(midi_file)[0]
            output_file = base_name + '.seq'
            extra_args = sys.argv[3:]

        use_gm = True
        enable_loop = False
        quantize_grid = None

        if any(arg in ("--no-gm", "--use-hm64-order") for arg in extra_args):
            use_gm = False
        if any(arg in ("--loop", "--enable-loop") for arg in extra_args):
            enable_loop = True

        for i, arg in enumerate(extra_args):
            if arg == "--quantize" and i + 1 < len(extra_args):
                try:
                    quantize_grid = int(extra_args[i + 1])
                except ValueError:
                    print(f"ERROR: --quantize requires integer")
                    sys.exit(1)
                break

        converter = SeqConverter(use_gm_mapping=use_gm, quantize_grid=quantize_grid)
        converter.convert(midi_file, output_file, enable_loop=enable_loop)
    
    else:
        print(f"ERROR: Unknown command '{command}'")
        sys.exit(1)

if __name__ == "__main__":
    main()