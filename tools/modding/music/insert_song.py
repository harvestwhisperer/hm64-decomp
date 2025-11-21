#!/usr/bin/env python3
"""
Enhanced script to insert song data into N64 ROM with sequence selection.
Usage: python insert_song_updated.py <song.bin>
"""

import sys
import os

ROM_FILENAME = "hm64.z64"

SEQUENCES = [
    {'name': 'opening', 'start_addr': 0xFD8510, 'end_addr': 0xFDACFF},
    {'name': 'title', 'start_addr': 0xFDAD00, 'end_addr': 0xFDD6BF},
    {'name': 'sports-festival-1', 'start_addr': 0xFDD6C0, 'end_addr': 0xFDFB4F},
    {'name': 'goodbye', 'start_addr': 0xFDFB50, 'end_addr': 0xFE12EF},
    {'name': 'spring', 'start_addr': 0xFE12F0, 'end_addr': 0xFE27AF},
    {'name': 'summer', 'start_addr': 0xFE27B0, 'end_addr': 0xFE4D1F},
    {'name': 'autumn', 'start_addr': 0xFE4D20, 'end_addr': 0xFE5E4F},
    {'name': 'winter', 'start_addr': 0xFE5E50, 'end_addr': 0xFE6E5F},
    {'name': 'naming-screen', 'start_addr': 0xFE6E60, 'end_addr': 0xFE7DFF},
    {'name': 'festival', 'start_addr': 0xFE7E00, 'end_addr': 0xFE903F},
    {'name': 'sports-festival-2', 'start_addr': 0xFE9040, 'end_addr': 0xFEAB6F},
    {'name': 'spirit-festival', 'start_addr': 0xFEAB70, 'end_addr': 0xFEB8EF},
    {'name': 'dog-race', 'start_addr': 0xFEB8F0, 'end_addr': 0xFED5EF},
    {'name': 'racing-festival', 'start_addr': 0xFED5F0, 'end_addr': 0xFEEA3F},
    {'name': 'festival-dance', 'start_addr': 0xFEEA40, 'end_addr': 0xFEF9EF},
    {'name': 'mountain', 'start_addr': 0xFEF9F0, 'end_addr': 0xFF1CCF},
    {'name': 'village', 'start_addr': 0xFF1CD0, 'end_addr': 0xFF375F},
    {'name': 'pond', 'start_addr': 0xFF3760, 'end_addr': 0xFF415F},
    {'name': 'church', 'start_addr': 0xFF4160, 'end_addr': 0xFF474F},
    {'name': 'tavern', 'start_addr': 0xFF4750, 'end_addr': 0xFF62FF},
    {'name': 'wedding', 'start_addr': 0xFF6300, 'end_addr': 0xFF6ECF},
    {'name': 'new-year-sunrise', 'start_addr': 0xFF6ED0, 'end_addr': 0xFF7D7F},
    {'name': 'credits', 'start_addr': 0xFF7D80, 'end_addr': 0xFF936F},
    {'name': 'broken-music-box', 'start_addr': 0xFF9370, 'end_addr': 0xFF959F},
    {'name': 'music-box', 'start_addr': 0xFF95A0, 'end_addr': 0xFF983F},
    {'name': 'flute-performance', 'start_addr': 0xFF9840, 'end_addr': 0xFF99EF},
    {'name': 'organ-performance', 'start_addr': 0xFF99F0, 'end_addr': 0xFF9BBF},
    {'name': 'ocarina-performance', 'start_addr': 0xFF9BC0, 'end_addr': 0xFF9DBF},
    {'name': 'sleigh-bells', 'start_addr': 0xFF9DC0, 'end_addr': 0xFF9F3F},
    {'name': 'bad-ending', 'start_addr': 0xFF9F40, 'end_addr': 0xFFA7AF},
    {'name': 'vocal-performance', 'start_addr': 0xFFA7B0, 'end_addr': 0xFFA94F},
    {'name': 'ensemble-performance', 'start_addr': 0xFFA950, 'end_addr': 0xFFAF7F},
    {'name': 'drum-performance', 'start_addr': 0xFFAF80, 'end_addr': 0xFFB08F},
    {'name': 'typhoon-ambience-1', 'start_addr': 0xFFB090, 'end_addr': 0xFFB6FF},
    {'name': 'typhoon-ambience-2', 'start_addr': 0xFFB700, 'end_addr': 0xFFC52F},
    {'name': 'night-ambience-spring', 'start_addr': 0xFFC530, 'end_addr': 0xFFC9EF},
    {'name': 'night-ambience-summer', 'start_addr': 0xFFC9F0, 'end_addr': 0xFFCC3F},
    {'name': 'night-ambience-autumn', 'start_addr': 0xFFCC40, 'end_addr': 0xFFCE0F},
    {'name': 'starry-night-festival', 'start_addr': 0xFFCE10, 'end_addr': 0xFFD48F},
    {'name': 'beach-ambience-1', 'start_addr': 0xFFD490, 'end_addr': 0xFFD67F},
    {'name': 'beach-ambience-2', 'start_addr': 0xFFD680, 'end_addr': 0xFFD81F},
]

# Calculate sizes for each sequence
for seq in SEQUENCES:
    seq['size'] = seq['end_addr'] - seq['start_addr'] + 1


def find_rom():
    """
    Search for the ROM file in two locations:
    1. Script's directory (e.g., ./tools/modding/music/)
    2. Project root (3 levels up from script directory)
    Returns the full path to the ROM if found, None otherwise.
    """

    script_dir = os.path.dirname(os.path.abspath(__file__))

    # First, check in the script's directory
    rom_path = os.path.join(script_dir, ROM_FILENAME)

    if os.path.exists(rom_path):
        return rom_path

    # If not found, check in the project root
    project_root = os.path.abspath(os.path.join(script_dir, '..', '..', '..'))
    rom_path = os.path.join(project_root, ROM_FILENAME)

    if os.path.exists(rom_path):
        return rom_path

    # ROM not found
    return None


def display_sequence_menu(sequences):
    """Display numbered menu of available sequences."""

    print("\nAvailable sequences:")
    print("-" * 80)
    print(f"{'#':<4} {'Name':<30} {'Start Addr':<12} {'End Addr':<12} {'Size (bytes)':<12}")
    print("-" * 80)

    for i, seq in enumerate(sequences, 1):
        print(f"{i:<4} {seq['name']:<30} 0x{seq['start_addr']:<10X} "
              f"0x{seq['end_addr']:<10X} {seq['size']:<12} (0x{seq['size']:X})")

    print("-" * 80)


def get_user_choice(sequences):
    """Get user's sequence selection."""

    while True:

        try:
            choice = input(f"\nSelect sequence number (1-{len(sequences)}) or 'q' to quit: ").strip()

            if choice.lower() == 'q':
                return None

            choice_num = int(choice)
            
            if 1 <= choice_num <= len(sequences):
                return choice_num - 1  # Return 0-indexed
            else:
                print(f"Invalid choice. Please enter a number between 1 and {len(sequences)}.")

        except ValueError:
            print("Invalid input. Please enter a number or 'q' to quit.")


def insert_song(bin_file, rom_path, sequence):
    """Insert song data into ROM at the specified sequence location."""

    # Check if bin file exists
    if not os.path.exists(bin_file):
        print(f"Error: Song file '{bin_file}' not found!")
        return 1

    # Read the song data
    with open(bin_file, 'rb') as f:
        song_data = f.read()

    song_size = len(song_data)
    available_size = sequence['size']

    print(f"\nSequence: {sequence['name']}")
    print(f"Song size: {song_size} bytes (0x{song_size:X})")
    print(f"Available space: {available_size} bytes (0x{available_size:X})")

    # Check if song fits in the allocated space
    if song_size > available_size:
        print(f"\nError: Song is too large! ({song_size} > {available_size})")
        print(f"The song exceeds the available space by {song_size - available_size} bytes.")
        return 1

    # Confirm with user before writing
    confirm = input(f"\nProceed with insertion into '{sequence['name']}'? (y/n): ").strip().lower()
    if confirm != 'y':
        print("Insertion cancelled.")
        return 1

    # Open ROM in read-write binary mode
    with open(rom_path, 'r+b') as rom:
        # Seek to start position
        rom.seek(sequence['start_addr'])

        # Clear the region by writing zeros
        print(f"\nClearing region 0x{sequence['start_addr']:X} - 0x{sequence['end_addr']:X}...")
        rom.write(b'\x00' * available_size)

        # Seek back to start and write song data
        rom.seek(sequence['start_addr'])
        rom.write(song_data)

        print(f"Successfully inserted song data at 0x{sequence['start_addr']:X}")
        print(f"Bytes written: {song_size}")
        print(f"Remaining space: {available_size - song_size} bytes")

    return 0


def main():
    """Main function."""
    if len(sys.argv) != 2:
        print("Usage: python insert_song_updated.py <song.bin>")
        sys.exit(1)

    bin_file = sys.argv[1]

    # Find the ROM file
    print(f"Searching for '{ROM_FILENAME}'...")
    rom_path = find_rom()

    if rom_path is None:
        print(f"Error: ROM file '{ROM_FILENAME}' not found!")
        print("Searched in:")
        script_dir = os.path.dirname(os.path.abspath(__file__))
        print(f"  1. {script_dir}")
        project_root = os.path.abspath(os.path.join(script_dir, '..', '..', '..'))
        print(f"  2. {project_root}")
        sys.exit(1)

    print(f"Found ROM at: {rom_path}")
    print(f"Found {len(SEQUENCES)} sequences.")

    # Display menu and get user choice
    display_sequence_menu(SEQUENCES)
    choice_idx = get_user_choice(SEQUENCES)

    if choice_idx is None:
        print("Operation cancelled.")
        sys.exit(0)

    selected_sequence = SEQUENCES[choice_idx]

    # Insert the song
    result = insert_song(bin_file, rom_path, selected_sequence)
    sys.exit(result)


if __name__ == "__main__":
    main()
