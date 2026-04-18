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

##### General MIDI mapping (on by default)
The converter maps General MIDI instrument patches to HM64's custom instrument order by default, since most MIDIs are authored against GM. If your MIDI file already uses HM64's instrument ordering (rare — only applies to MIDIs authored specifically for this game), pass `--no-gm` or `--use-hm64-order` to disable the remapping.

##### `--loop` or `--enable-loop`
Enables infinite looping of the sequence.

- The sequence will loop from the end back to the start
- Currently only supports full sequence looping (start to end)

##### `--quantize N`
Quantizes note timing to a specific musical grid.

- Useful for MIDIs where note events may be slightly off-grid (some DAWs like MuseScore do this for playback)
- `N` is the note division (e.g., `4` = 16th notes, `8` = 32nd notes)
- Grid size is calculated as: `ticks_per_quarter / N`
- Most MIDI files work fine with the default quantization

**Example:**
```bash
python main.py convert song.mid --quantize 4  # Quantize to 16th notes
```

> **Branch matters.** Custom sequences don't produce a matching ROM, so the `master` branch's default `make` will fail its checksum check. Pick one of two approaches:
>
> - **Rebuild from source** — switch to the `dev` or `dev-qol` branch, where the default `make` already uses a shiftable memory layout. This lets your custom sequence be any size and supports adding brand-new songs. Follow the *Replacing* or *Adding* flows below.
> - **Patch a pre-built ROM** — if you already have a built `hm64.z64` and just want to swap one song without rebuilding, use `insert_song.py` (see *Patching a Pre-Built ROM* below). Works from any branch, but the new song must fit in the original song's byte size.

### Replacing Sequences in the Game

After converting a MIDI to `.seq`, you can replace an existing game sequence by rebuilding from source:

1. Switch to a shiftable-build branch:
   ```bash
   git checkout dev        # or dev-qol
   ```

2. Make sure `assets/audio/sequences/` is populated. If it is empty, run `make extract-sequences` once — splat writes the extracted `.seq` files there.

3. Copy your `.seq` file over an existing sequence:
   ```bash
   cp my_song.seq ../../../assets/audio/sequences/spring.seq
   ```

4. Rebuild:
   ```bash
   make
   ```

The `dev`/`dev-qol` build supports a shiftable memory layout out of the box, so your custom sequence can be any size — it does not need to fit within the original sequence's space.

### Adding New Sequences

To add a completely new sequence (not replacing an existing one):

1. Switch to a shiftable-build branch: `git checkout dev` (or `dev-qol`).

2. Place your `.seq` file in `assets/audio/sequences/` (e.g. `assets/audio/sequences/newSong.seq`).

3. Add an entry to `assets/audio/sequences.spec`:

   ```
   beginseg
       name "newSong"
       flags RAW
       include "build/assets/audio/sequences/newSong.seq.o"
   endseg
   ```

4. Update the `sequenceRomAddresses` array in `src/game/gameAudio.c`. Append the new segment's linker-generated start/end symbols at the end of the array, matching the existing pattern:

   ```c
   Addresses sequenceRomAddresses[] = {
       // ...
       { &_beachAmbience1SegmentRomStart, &_beachAmbience1SegmentRomEnd },
       { &_beachAmbience2SegmentRomStart, &_beachAmbience2SegmentRomEnd },
       { &_newSongSegmentRomStart,        &_newSongSegmentRomEnd        },
       // ...
   };
   ```

   The symbol names come from the spec `name` value (step 3) — `newSong` becomes `_newSongSegmentRomStart` / `_newSongSegmentRomEnd`.

5. Add a `#define` in `src/assetIndices/sequences.h` for the new index, taking the next available integer (the last existing entry is `BEACH_AMBIENCE_2 40`, so a new song is `41`):

   ```c
   #define NEW_SONG 41
   ```

6. Trigger the song from game code or from a cutscene. Pick whichever applies:

   - **From C code** (title screen, naming screen, ad-hoc call sites):

     ```c
     setCurrentAudioSequence(NEW_SONG);
     ```

   - **From a cutscene DSL file**:

     ```text
     SET_AUDIO_SEQUENCE 0, _newSongSegmentRomStart, _newSongSegmentRomEnd
     ```

7. If the song is the background music for a map, update the `u8 levelToMusicMappings[TOTAL_MAPS][8]` array in `src/game/level.c`. The array is indexed by map order in `src/assetIndices/maps.h`. For example, to replace the farm's music (map index 82), edit row 82 — the 8 columns are spring / summer / autumn / winter / night-spring / night-summer / night-autumn / sentinel (`0xFF`):

   ```c
   { NEW_SONG, SUMMER_THEME, AUTUMN_THEME, WINTER_THEME, NIGHT_AMBIENCE_SPRING, NIGHT_AMBIENCE_SUMMER, NIGHT_AMBIENCE_AUTUMN, 0xFF },
   ```

8. Rebuild: `make`.

> You do **not** need to touch `config/us/splat.us.yaml` for new sequences. Splat only extracts content that already exists in the baserom; a brand-new song has nothing to extract. Only the spec entry, the on-disk `.seq` file, and the code wiring above matter to the build.
>
> Steps 4 and 5 are the minimum required to get the song *into* the build. Steps 6 and 7 are what make it actually audible in-game — skip them and the sequence will sit in the ROM unused.

### Patching a Pre-Built ROM

If you already have a built `hm64.z64` and just want to swap one song without rebuilding from source, use `insert_song.py`. It writes your `.seq` directly into the ROM at the target slot's original address.

```bash
python insert_song.py my_song.seq
```

The script looks for `hm64.z64` in the current directory first, then at the project root. It shows an interactive menu of all 41 sequence slots; pick one and confirm. **The new song must fit in the original slot's byte size** — the script checks and refuses to write if it does not fit. If you need a larger custom song, use the rebuild flow above instead.

This workflow works from any branch (including `master`) because it operates on the built ROM, not the source.

### Parsing N64 Sequence Files

Parse existing N64 sequence files to text logs for analysis:

```bash
python main.py parse <sequence.seq> [output_log.txt] [-v|--verbose]
```

If no output file is specified, the script generates one with `.txt` extension.

**Examples:**
```bash
# Auto-generate output filename (creates spring.txt)
python main.py parse ../../../assets/audio/sequences/spring.seq

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
