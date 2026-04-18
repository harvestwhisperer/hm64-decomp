This is a repository for a decompilation project of the Nintendo 64 game Harvest Moon 64. The US version is 100% decompiled and builds a byte-identical ROM. The Japanese version has initial scaffolding and a few matched functions.

## Directory Structure

```
hm64-decomp-master/
├── src/                  # Game source code
│   ├── game/             # Game logic (~49 files): animals, cutscenes, items, player, menus, etc.
│   ├── system/           # Engine subsystems (~48 files): audio, controller, entity, map, sprite, graphics
│   ├── buffers/          # Pre-allocated fixed-size memory buffers (~32 files)
│   ├── data/             # Static data (animation scripts, audio data, tile maps)
│   ├── bytecode/         # Custom DSL source files
│   │   ├── cutscenes/    # Cutscene DSL scripts (~26 files) -> compiled to bytecode
│   │   └── dialogues/    # Dialogue DSL scripts (~48 files) -> compiled to bytecode
│   ├── assetIndices/     # Asset lookup tables
│   ├── mainproc.c/h      # Entry point
│   └── mainLoop.c/h      # Main game loop
├── include/              # Global headers (~70 files)
│   ├── ld_symbols.h      # Linker-generated symbols (325KB, US)
│   └── common.h, structs.h, macros.h, etc.
├── lib/                  # SDK and runtime libraries
│   ├── libultra/         # Official N64 SDK (OS, graphics, I/O, libc)
│   ├── nusys-1/          # NuSystem game framework
│   ├── libmus/           # Music/audio playback
│   ├── libkmc/           # KMC compiler runtime
│   ├── gcc/              # GCC runtime
│   └── ucode/            # RSP/RDP GPU microcode
├── tools/                # Build and modding tools (mostly Python)
│   ├── libhm64/          # Core Python library: transpilers, extractors, and utilities per asset type
│   │   ├── animations/   # Animation metadata parsing and GIF generation
│   │   ├── cutscenes/    # Cutscene DSL transpiler, extractor, opcodes
│   │   ├── dialogues/    # Dialogue DSL transpiler, opcodes
│   │   ├── fonts/        # Font extraction and assembly
│   │   ├── maps/         # Map mesh/texture extraction
│   │   ├── sprites/      # Sprite extraction, assembly, textures
│   │   ├── text/         # Text encoding/decoding, transpiler, extractor
│   │   ├── common/       # Shared utilities (image ops, binary parsing)
│   │   └── data/         # Data structure definitions
│   ├── build/            # Build infrastructure (mkldscript.c, spec parser)
│   ├── splat_ext/        # Custom Splat ROM extraction extensions
│   ├── sprite_utilities/ # Tkinter GUI: sprite browser, viewer, animation editor
│   └── modding/          # Modding utilities
│       ├── map/          # Blender map importer, CLI tools, analysis, tests
│       └── music/        # MIDI-to-N64 music converter
├── assets/               # Extracted game assets (~40K files)
│   ├── animations/       # Animation metadata (JSON) organized by character/object
│   ├── sprites/          # Sprite assets (entitySprites, mapObjects, effects, overlayScreens)
│   ├── cutscenes/        # Cutscene binary data
│   ├── dialogues/        # Dialogue binary data
│   ├── text/             # Encoded text strings
│   ├── font/             # Font textures and palettes
│   ├── maps/             # Map mesh, texture, and object data
│   └── audio/            # Audio data
├── config/               # Build configuration per region
│   ├── us/               # splat.us.yaml, symbol_addrs.txt, undefined_funcs/syms.txt, common_bss.ld
│   └── jp/               # splat.jp.yaml, symbol_addrs.txt, undefined_funcs/syms.txt
├── spec                  # ROM layout specification (segment definitions)
├── Makefile              # Main build (matching, US)
├── Makefile.jp           # Japanese version build
├── Makefile.modding      # Modding build (shiftable, non-matching OK)
└── docs/                 # Documentation (cutscene readability guide, etc.)
```

## Code vs. Tools

**Game code** (`src/`, `include/`, `lib/`) is C and MIPS assembly targeting the N64 via KMC GCC 2.7.2. The decompiled code must produce a byte-identical ROM when compiled with the original compiler toolchain (`make` with default settings).

**Tools** (`tools/`) are Python with a small amount of C (3 files for build infrastructure). Key tool categories:
- **libhm64**: Core Python library containing transpilers, extractors, and utilities organized by asset type (cutscenes, dialogues, text, sprites, fonts, maps, animations). Each submodule typically has a `transpiler.py` (DSL -> assembly), `extractor.py` (ROM -> assets), and `opcodes.py` or format-specific modules.
- **sprite_utilities**: Tkinter desktop GUI for browsing sprites, viewing animations, editing animation scripts, and importing sprites. Run via `python -m sprite_utilities`.
- **modding/map**: Blender map importer with CLI tools, analysis utilities, and tests. See `tools/modding/map/README.md`.
- **modding/music**: MIDI-to-N64 music format converter.
- **Splat extensions** (`splat_ext/`): Custom handlers for HM64-specific ROM segment types (animation scripts, maps, sequences).

## Build System

The project uses GNU Make with the MIPS Linux GNU cross-compiler toolchain. Two compiler modes:
- **Default (matching)**: KMC GCC 2.7.2 (`tools/gcc-2.7.2/`) -- produces byte-identical ROM
- **Modern GCC**: `MODERN_GCC=1` -- uses system `mips-linux-gnu-gcc`, non-matching but enables warnings

Key build targets:
- `make` / `make all` -- build and verify ROM matches original
- `make setup` -- clean, split ROM via Splat, extract sprites/fonts
- `make clean` -- remove build artifacts
- `make extract-sprites`, `extract-fonts`, `extract-animations`, `extract-texts`, `extract-cutscenes`, `extract-dialogues`, `extract-map-sprites` -- asset extraction

The ROM is assembled from a `spec` file that defines segments (`header`, `boot`, `entry`, `code`, buffer segments, `rom_data`) linked at specific addresses.

## Custom Bytecode / DSL Systems

The game uses two custom bytecode systems with their own DSLs:
- **Cutscene bytecode** (`src/bytecode/cutscenes/*.dsl`): Variable-length opcodes indexing into a handler function table in `system/cutscene.c`. Supports spawning child executors, branching, and subroutine jumps.
- **Dialogue bytecode** (`src/bytecode/dialogues/*.dsl`): Condition-based branching (affection values, random, status bits), game variable manipulation, and sub-dialogue spawning.
- **Text format** (`assets/text/*.txt`): Custom encoding with opcode characters (0-10) and 1-byte bitfields every 8 characters indicating 1-byte vs 2-byte character widths.

## Common Workflows

### Modding Tool Development
Tools live in `tools/` and are primarily Python. The core library is `tools/libhm64/` with submodules for each asset type -- transpilers, extractors, and format-specific utilities are all within their respective submodules (e.g., `libhm64/cutscenes/transpiler.py`). The tkinter sprite/animation GUI is `tools/sprite_utilities/`. Map modding tools integrate with Blender via `tools/modding/map/` (which includes its own CLI, analysis tools, and tests). When developing tools, test against `make setup` and asset extraction targets.

### Decompiled Code Research and Naming
The codebase contains many generic or auto-generated names (e.g., `func_XXXXXXXX`, unnamed struct fields, numeric flag values). Research work involves:
- Identifying what functions, variables, and struct members do and giving them accurate names
- Adding macro values for magic numbers (see `player.h` for examples of player action macros)
- Improving readability of bytecode DSL files with better labels and macros
- Cleaning up fake/forced matches (search for `FIXME` comments)
- The `config/us/symbol_addrs.txt` file maps symbol names to ROM addresses

### Matching Refactors
When refactoring decompiled code, the ROM must still match byte-for-byte. Always verify with `make` (which runs a checksum comparison). Use the default compiler (KMC GCC 2.7.2), not `MODERN_GCC=1`. The `config/us/common_bss.ld` file hardcodes BSS addresses for matching -- do not modify it for matching builds.

## Key Files for Orientation
- `src/game/game.c` -- main game loop and state machine
- `src/system/cutscene.c` -- cutscene bytecode executor and handler function table
- `src/system/dialogue.c` -- dialogue bytecode executor
- `src/system/message.c` -- message box rendering and text parsing
- `src/system/entity.c` -- entity/sprite lifecycle
- `src/system/map.c` -- map rendering and display list generation
- `src/game/player.c` -- player character control
- `spec` -- ROM segment layout
- `config/us/splat.us.yaml` -- Splat ROM extraction configuration

## Environment
- Builds run in WSL2 (Ubuntu) on Windows
- Requires: `mips-linux-gnu-` cross toolchain, Python 3, KMC GCC 2.7.2 (fetched by `tools/setup.sh`)
- Python dependencies: `tools/requirements.txt` (splat64, n64img, Pillow, numpy, mapfile_parser, ipl3checksum, mido)

## Additional Rules
- When naming functions, variables, flags, macros, cutscene bytecode, data, etc, do NOT rename things based on unfounded inferences. Prefer the unknown to a possibly inaccurate label.