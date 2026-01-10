# Harvest Moon 64 Sequence Tools

Tools for converting MIDI files to Harvest Moon 64's sequence format. A short sample MIDI is provided at `mozart-sample.mid`.

## Installation

Make sure `mido` is installed. From the decomp project root:

```bash
cd tools
pip install -r requirements.txt
```

## Usage

### Converting MIDI to N64 Sequence Format

Use `main.py` to convert MIDI files to N64 sequence format:

```bash
python main.py convert <input.mid> [output.seq] [flags]
```

If no output file is specified, the script will automatically generate one by replacing the input's extension with `.seq` (e.g., `song.mid` -> `song.seq`).

**Examples:**
```bash
# Auto-generate output filename (mozart-sample.seq)
python main.py convert mozart-sample.mid --gm --loop

# Specify custom output filename
python main.py convert mozart-sample.mid custom_name.seq --gm --loop
```

#### Optional Flags

##### `--no-gm` or `--use-hm64-mapping`
By default, the converter maps general MIDI instrument numbering to HM64's custom instrument order. If you've created a MIDI with the game's instrument ordering, use this flag.

##### `--loop` or `--enable-loop`
Enables infinite looping of the sequence.

- The sequence will loop from the end back to the start
- Currently only supports full sequence looping (start to end)

##### `--quantize N`
Quantizes note timing to a specific musical grid.

- Useful for MIDIs where note events may be slightly off-grid (some notation software like MuseScore does this to enhance sample playback)
- `N` is the note division (e.g., `4` = 16th notes, `8` = 32nd notes)
- Grid size is calculated as: `ticks_per_quarter / N`
- Most MIDI files seem to work fine with the default quantization

**Example:**
```bash
python main.py convert song.mid --quantize 4  # Quantize to 16th notes
```

### Replacing Sequences in the Game

There are two ways to replace sequences: using the `insert_song.py` (easiest, doesn't require rebuilding the game, but inserted sequences must be less than or equal in size to the replaced sequences) or incorporating the new sequence into a build.

To do the latter, after converting a MIDI to `.seq` file, replace an existing sequence:

1.
   ```bash
   cp new_song.seq ../../../assets/audio/sequences/opening.seq
   ```

2. Rebuild the ROM:
   ```bash
   make
   ```

The build system supports shiftable builds, so your custom sequence can be any size so long as the 16 MB total ROM size isn't exceeded.

### Adding New Sequences

To add a completely new sequence:

1. Place your `.seq` file in `assets/audio/sequences/`

2. Add an entry to `assets/audio/audio.spec`:
   ```
   beginseg
       name "newSong"
       flags RAW
       include "build/assets/audio/sequences/newSong.seq.o"
   endseg
   ```

3. Update the `sequenceRomAddresses` array in `src/game/gameAudio.c` like this:
    ```
    Addresses sequenceRomAddresses[] = { 
        // ...
        { &_beachAmbience1SegmentRomStart, &_beachAmbience1SegmentRomEnd },
        { &_beachAmbience2SegmentRomStart, &_beachAmbience2SegmentRomEnd },
        { &_newSongSegmentRomStart, &_newSongSegmentRomEnd },
        // ...
    };
    ```

4. Add a new define in `src/assetIndices/sequences.h`:
    ```
    #DEFINE NEW_SONG 41
    ```

5. Use the new sequence either via game code or cutscenes:

    ```
    // manually; usage for title screen, naming screen, etc.
    setCurrentAudioSequence(NEW_SONG);
    ```

    ```
    SET_AUDIO_SEQUENCE 0, _newSongSegmentRomStart, _newSongSegmentRomEnd
    ```

6. If updating the music for a map, update the `u8 levelToMusicMappings[TOTAL_MAPS][8];` array in `src/game/level.c`, which is indexed by the map order in `assetIndices/maps.h`. I.e., to update the music for the farm, which is map index 82, update row 82 of the array:
    ```
    { NEW_SONG, SUMMER_THEME, AUTUMN_THEME, WINTER_THEME, NIGHT_AMBIENCE_SPRING, NIGHT_AMBIENCE_SUMMER, NIGHT_AMBIENCE_AUTUMN, 0xFF },
    ```

7. Rebuild the ROM with `make`

### Parsing and debugging sequences

Parse existing sequence files to text logs for analysis:

```bash
python main.py parse <sequence.seq> [output_log.txt] [-v|--verbose]
```

If no output file is specified, the script generates one with `.txt` extension.

**Examples:**
```bash
# Auto-generate output filename (creates newSong.txt)
python main.py parse ../../../assets/audio/sequences/newSong.seq

# Verbose mode - print all events to console
python main.py parse spring.seq -v
```

## File Format

The `.seq` format contains:
- Track data (notes, instruments, control commands)
- Volume envelopes
- Pitch bend envelopes
- ADSR envelope settings

The converter handles:
- MIDI timing conversion (variable TPQN -> fixed 48 TPQN)
- Polyphony splitting (MIDI polyphony -> multiple monophonic tracks)
- Track compression/multiplexing (merging non-overlapping tracks)
- Velocity and duration optimization
