---
name: hm64-music-modding
description: Use when the user wants to mod Harvest Moon 64 music — converting a MIDI to the game's `.seq` format, replacing an existing in-game song, adding a brand-new song to the ROM, patching a built `hm64.z64` in place, or parsing/inspecting an existing `.seq` file. Trigger proactively whenever the user mentions MIDI→HM64 conversion, `.seq` files, `tools/modding/music/`, `insert_song.py`, `mozart-sample.mid`, "replace the spring theme", "swap out a song", "add custom music", "General MIDI mapping", "sequence looping", "patch the ROM with a new song", or any variant that would plausibly be handled by the modding music tooling. Do NOT use this skill for questions about the runtime audio C code in `src/system/audio.c` or for sound-effect/wave-table work — those are not handled by these tools.
---

# HM64 Music Modding

This skill drives the MIDI-to-HM64 music pipeline at [tools/modding/music/](tools/modding/music/). It handles four user intents:

1. **Convert** a `.mid` to HM64's `.seq` format. (Always step 1 for everything below.)
2. **Rebuild with a replacement song** — swap one of the existing songs in a source build.
3. **Rebuild with a new song** — add a brand-new song slot to the ROM.
4. **Patch a pre-built ROM in place** — overwrite one song's bytes in `hm64.z64` without rebuilding.

Plus one read-only intent:

5. **Parse** an existing `.seq` to a human-readable text log.

## Picking a workflow

Before running anything, figure out which path the user is on. Ask if it is not obvious from context:

| Situation | Workflow |
|---|---|
| User wants to change what plays in an existing game slot AND is fine rebuilding from source | Replace + rebuild (§2). Supports any song size. |
| User wants an *additional* song, not just a swap | Add + rebuild (§3). Needs spec edit and code changes so something actually plays it. |
| User has a built `hm64.z64` and just wants a quick song swap without rebuilding | In-place ROM patch (§4). Simpler and faster, but the new song must fit in the original slot's byte size. |
| User only wants to inspect an existing `.seq` | Parse (§5). |

The rebuild workflows (§2, §3) require being on the `dev` or `dev-qol` branch — their default `make` already uses a shiftable memory layout, so custom sequences of any size work without special flags. The in-place workflow (§4) works from any branch because it operates on the built ROM, not the source. If the user is on `master` and wants to rebuild, flag this and confirm they want to switch branches before you do anything — `git checkout` with uncommitted work in progress can be disruptive.

The default `master` `make` target will fail its checksum check the moment any sequence bytes change, so do not tell the user to run `make` from `master` to test a mod.

## Converting MIDI to `.seq`

This is the common prerequisite for §2–§4. The converter lives at [tools/modding/music/main.py](tools/modding/music/main.py) and uses relative imports, so always `cd tools/modding/music` before invoking.

```bash
cd tools/modding/music
python main.py convert <input.mid> [output.seq] [flags]
```

| Flag | When to use |
|---|---|
| `--no-gm` / `--use-hm64-order` | **GM mapping is ON by default.** Only pass this if the MIDI was specifically authored against HM64's in-game instrument order ([tools/modding/music/config.py](tools/modding/music/config.py) has the full instrument list). If you do not know how the MIDI was authored, leave the default on — GM is by far the common case. Wrong guess means right notes on wrong instruments. |
| `--loop` / `--enable-loop` | The song should loop forever from end back to start. Only full-sequence looping is supported. Most in-game music (seasonal themes, map ambience) loops, so replacements for those should usually pass `--loop`. |
| `--quantize N` | The MIDI has slightly-off-grid note events (some DAWs, e.g. MuseScore, nudge notes for playback). `N` is the grid division — `4` for 16th notes, `8` for 32nd notes. Only reach for this if the first conversion attempt sounds wrong or the parsed output shows jittery timing. Most MIDIs do not need it. |

Output path defaults to `<input>.seq` next to the input if omitted.

## Workflow 2 — Rebuild with a replacement song

Pick the target slot from the existing song list at [assets/audio/sequences.spec](assets/audio/sequences.spec) (e.g. `spring`, `title`, `dogRaceTheme`). If the user says "replace the spring theme", the target filename is `spring.seq`.

1. Confirm the user is on `dev` or `dev-qol`; if not, confirm they want to switch before running `git checkout`.
2. If [assets/audio/sequences/](assets/audio/sequences/) is empty, run `make extract-sequences` once from the project root to populate it.
3. `cd tools/modding/music`
4. Convert the MIDI (see above): `python main.py convert /path/to/their.mid their.seq --loop` (add `--no-gm` only if they asked for it).
5. Copy the output over the target: `cp their.seq ../../../assets/audio/sequences/spring.seq` (or whichever slot).
6. From the project root, build: `make`.

Do not edit `assets/audio/sequences.spec` or `config/us/splat.us.yaml` when replacing — the slot already exists.

Before overwriting the target `.seq`, check `git status` on `assets/audio/sequences/`. If the target is dirty, the user may already have a prior mod there; ask before clobbering.

## Workflow 3 — Rebuild with a brand-new song

This workflow requires one spec edit plus several game-code edits. Crucially, **just adding the spec entry does not make the song audible** — the runtime music system identifies songs by an integer index, and that index has to be registered in two places and then actually triggered somewhere.

Here's what the skill should do vs. what it should ask:

- **Do autonomously** (steps 1–5 below, i.e. spec + rom-address array + index define). These are mechanical and always required for any new song, regardless of how it'll be used. Do not ask permission to make them — just do them and report.
- **Ask before doing** (steps 6–7, i.e. call sites and map mapping). These depend on *what the user wants the song to do in-game*. A song for the title screen needs a `setCurrentAudioSequence()` call; a song for a specific map needs a `levelToMusicMappings` edit; a song for a cutscene needs a DSL opcode in the right `.cutscene` file. Ask the user which of these apply before touching any of them.

### Steps

1. Confirm `dev` or `dev-qol` branch as in Workflow 2. If the user is on `master`, confirm before switching.

2. `cd tools/modding/music` and convert the MIDI to a sensibly-named file (camelCase, e.g. `newSong.seq`). The base name becomes the linker symbol name in later steps, so pick something `_<baseName>SegmentRomStart`-friendly.

3. Place the file at `assets/audio/sequences/newSong.seq`.

4. Append a new segment to [assets/audio/sequences.spec](assets/audio/sequences.spec):

   ```text
   beginseg
       name "newSong"
       flags RAW
       include "build/assets/audio/sequences/newSong.seq.o"
   endseg
   ```

5. Append a new entry to the `sequenceRomAddresses` array in [src/game/gameAudio.c](src/game/gameAudio.c). The existing pattern is one entry per line with linker-generated start/end symbol pointers. The symbol names are derived from the spec `name` field — `newSong` becomes `&_newSongSegmentRomStart` / `&_newSongSegmentRomEnd`. Add it at the end of the array, after `_beachAmbience2SegmentRomEnd`:

   ```c
   { &_newSongSegmentRomStart, &_newSongSegmentRomEnd },
   ```

6. Add a `#define` at the end of [src/assetIndices/sequences.h](src/assetIndices/sequences.h). The last existing entry as of this writing is `BEACH_AMBIENCE_2 40`, so a new song is `41`. **Verify the last existing define number before hardcoding** — count the entries in the current file, since other mods may have already added songs:

   ```c
   #define NEW_SONG 41
   ```

   The name should be SCREAMING_SNAKE_CASE and reasonably descriptive. Use the user's preferred name if they gave one; otherwise match the `newSong` base name (`NEW_SONG`).

### Ask the user before steps 7 and 8

After finishing steps 1–6, stop and ask the user **where the song should play** before making any further edits. Offer the three common patterns explicitly so they don't have to know the internals:

- **A specific call site in C** (e.g. the title screen, naming screen, credits). They'll need to name the function or describe the moment; then edit that spot to call `setCurrentAudioSequence(NEW_SONG)`.
- **A cutscene**. Ask which `.cutscene` file and where in it. The DSL opcode is:

  ```text
  SET_AUDIO_SEQUENCE 0, _newSongSegmentRomStart, _newSongSegmentRomEnd
  ```

  (Note: the cutscene DSL form takes the symbol names directly, no `&`, unlike the C array.)
- **The background music for a map**. Ask which map(s). Map indices are defined in [src/assetIndices/maps.h](src/assetIndices/maps.h), and row index N of `levelToMusicMappings` in [src/game/level.c](src/game/level.c) corresponds to the map at index N. The 8 columns are: spring, summer, autumn, winter, night-spring, night-summer, night-autumn, sentinel (`0xFF`). Example — to make map 82 (the farm) play the new song in spring:

  ```c
  { NEW_SONG, SUMMER_THEME, AUTUMN_THEME, WINTER_THEME, NIGHT_AMBIENCE_SPRING, NIGHT_AMBIENCE_SUMMER, NIGHT_AMBIENCE_AUTUMN, 0xFF },
  ```

- **None of the above — they just want the song in the ROM for now.** That's fine; steps 1–6 are complete. Flag that the song won't be audible until a trigger is added later.

If the user answers more than one, do them all.

### Step 8 — Rebuild

From the project root: `make`.

**Do not add a `config/us/splat.us.yaml` entry for a new song.** Splat only extracts content that already exists in the baserom; a song the user just authored has nothing to extract. The spec entry, the rom-address array entry, the `#define`, and (optionally) the trigger call site are all the build needs.

## Workflow 4 — Patch a pre-built ROM in place

Use [tools/modding/music/insert_song.py](tools/modding/music/insert_song.py) when the user already has a built `hm64.z64` and wants a fast swap without rebuilding. This works from any branch, including `master`.

```bash
cd tools/modding/music
python insert_song.py my_song.seq
```

The script:

- Looks for `hm64.z64` in its own directory first, then at the project root. If the user's ROM is elsewhere, either copy it into one of those locations first or tell them to.
- Prints an interactive menu listing all 41 sequence slots with their byte sizes. The user picks a number; the script asks for `y/n` confirmation before writing.
- **Enforces the original byte size**: if `my_song.seq` is larger than the slot's original allocation, the script prints an error and exits without modifying the ROM. When this happens, either re-convert with fewer tracks / shorter content, or use Workflow 2 (rebuild path) which has no size limit.
- Overwrites the slot with zeros first, then writes the new `.seq` bytes. This is destructive to the ROM on disk — remind the user to keep a clean backup of `hm64.z64` before running it.

Because the script is interactive (menu selection + y/n prompt), run it in the foreground so the user can see the menu and respond. Do not try to pipe automated input in.

## Workflow 5 — Parse for inspection

```bash
cd tools/modding/music
python main.py parse ../../../assets/audio/sequences/spring.seq
```

Writes `spring.txt` next to the input. Add `-v` to also echo events to the console. No build step; read-only.

## Executing the flow

Run commands yourself when the user provides a MIDI path. Confirm before:

- **`git checkout dev` / `dev-qol`** when the user is currently on another branch. Check `git status` first — if there are uncommitted changes, flag them before switching.
- **Overwriting any file the user may have modified** — the target `.seq`, `hm64.z64`, or an existing spec entry. Always check `git status` on the target first.
- **Kicking off `make`.** Full builds take minutes. If the user only asked for a conversion, do the conversion and stop — do not silently start a build.
- **Running `insert_song.py`** — it destructively modifies `hm64.z64`. Confirm the user has a backup copy before running.

Don't confirm before:

- Running `python main.py convert` or `parse` — those are local, reversible file operations.
- Creating `assets/audio/sequences/` if it's missing.

## What the converter actually does

Useful context when diagnosing conversion problems:

- MIDI timing is resampled from whatever TPQN the source uses to HM64's fixed 48 TPQN.
- Polyphonic MIDI tracks are split into multiple monophonic tracks (HM64 tracks are monophonic).
- Non-overlapping monophonic tracks are then merged back to save track slots.
- Velocities and note durations are compressed/optimized.

If a converted song sounds "wrong", the most common causes — in rough order — are: the MIDI wasn't actually GM and needed `--no-gm`, the MIDI uses more simultaneous voices than HM64 has tracks, or slight off-grid timing needs `--quantize`. Parse the output `.seq` to see what the converter actually produced before blaming the tool.

## Prefer unknown over inference

If the user asks which HM64 instrument corresponds to a GM patch, or what an in-game song's structure is, look it up in [tools/modding/music/config.py](tools/modding/music/config.py) (for the instrument table) or parse the relevant `.seq` first. Do not guess — HM64's instrument list has entries still labelled `Instr_12`, `Instr_13`, etc., because their exact sound hasn't been confirmed, and guessing a name for them would be exactly the kind of unfounded inference the project avoids.
