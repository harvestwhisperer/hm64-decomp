# Harvest Moon 64 Sequence Tools

A collection of tools for converting MIDI to Harvest Moon 64 sequence format, parsing sequences, and inserting sequences into the ROM. A short sample MIDI is provided at `mozart-sample.mid` 

## Installation

Make sure `mido` is installed. From the decomp project root:

```bash
cd tools
pip install -r requirements.txt
```

## Usage

### Converting MIDI to N64 Sequence Format

Use `main.py` to convert MIDI files to N64 sequence binary format:

```bash
python main.py convert <input.mid> [output.bin] [flags]
```

**Note:** If no output file is specified, the script will automatically generate one by replacing the input's extension with `.bin` (e.g., `song.mid` → `song.bin`).

**Examples:**
```bash
# Auto-generate output filename (mozart-sample.bin)
python main.py convert mozart-sample.mid --gm --loop

# Specify custom output filename
python main.py convert mozart-sample.mid custom_name.bin --gm --loop
```

#### Optional Flags

##### `--gm` or `--use-gm-mapping`
Maps General MIDI instrument patches to HM64's custom instrument order.

- **Use this flag** when your MIDI file uses standard General MIDI instrument ordering
- **Omit this flag** if your MIDI file already uses HM64's instrument patch ordering


##### `--loop` or `--enable-loop`
Enables infinite looping of the sequence.

- The sequence will loop from the end back to the start
- Currently only supports full sequence looping (start to end)

##### `--quantize N`
**Experimental feature** - Quantizes note timing to a specific musical grid

- This is needed for any midis where note-on and note-off events may come slightly before or after the next quantized grid event (some composition software, like MuseScore, may do this to enhance patch playback)
- `N` is the note division (e.g., `4` = 16th notes, `8` = 32nd notes)
- Grid size is calculated as: `ticks_per_quarter / N`
- Most MIDI files seem to work fine with the default quantization

**Example:**
```bash
python main.py convert song.mid --quantize 4  # Quantize to 16th notes
```

### Parsing N64 Sequence Files

Parse existing N64 sequence binary files to text logs:

```bash
python main.py parse <sequence.bin> [output_log.txt] [-v|--verbose]
```

**Note:** If no output file is specified, the script will automatically generate one by replacing the input's extension with `.txt` (e.g., `song.bin` → `song.txt`).

**Verbose Mode:** By default, the parser only displays progress messages. Use `-v` or `--verbose` to print all parsed events to the console in addition to the log file.

**Examples:**
```bash
# Auto-generate output filename (creates original_song.txt)
python main.py parse original_song.bin

# Specify custom output filename
python main.py parse original_song.bin custom_log.txt

# Print all events to console while parsing
python main.py parse original_song.bin -v

### Inserting Sequences into ROM

Use `insert_song.py` to insert converted sequence files into a ROM:

```bash
python insert_song.py <song.bin>
```

The script will:
1. Search for the ROM file (`hm64.z64`) in the current directory and project root
2. Display a menu of all 41 available music sequences in the game
3. Validate that the input sequence fits into the chosen game sequence (shiftability is not yet supported)
4. Ask for confirmation before writing
5. Clear the sequence segment in the rom and insert the new sequence

**Example:**
```bash
python insert_song.py mozart-sample.bin
```

**Interactive Menu:**
```
Available sequences:
--------------------------------------------------------------------------------
#    Name                           Start Addr   End Addr     Size (bytes)
--------------------------------------------------------------------------------
1    opening                        0xFD8510     0xFDACFF     10224        (0x27F0)
2    title                          0xFDAD00     0xFDD6BF     10688        (0x29C0)
3    sports-festival-1              0xFDD6C0     0xFDFB4F     9360         (0x2490)
...

Select sequence number (1-41) or 'q' to quit:
```