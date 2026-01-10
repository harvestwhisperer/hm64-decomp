# Harvest Moon 64 Decomp

## Progress

### North America

The US version is now 100% decompiled! This includes all game functions, data and rodata within the game's main code section, and all library functions, as well as DSL for the cutscenes and dialogue that compiles to the game's custom bytecode systems. All text segments are also automatically extracted and transpiled back to the custom text format during the build process.

The current build system also supports shiftability. To preserve matching, `config/us/common_bss.ld` is used to match up game variables with their original addresses. For modding, this file should be empty or removed to allow the linker to reallocate memory addresses without conflict.

### Japan

The repository now has initial support for Japanese-version builds.

## Setting up
1. Install [WSL2](https://learn.microsoft.com/en-us/windows/wsl/install)
1. WSL: `sudo apt-get update && sudo apt install -y python3 pip binutils-mips-linux-gnu gcc-mips-linux-gnu build-essential wget`

### WSL Workflow
1. `git clone --recursive https://github.com/harvestwhisperer/hm64-decomp.git`
1. `cd hm64-decomp`
1. `sudo chmod +x tools/setup.sh`
1. `sudo tools/setup.sh`. This will fetch needed system packages not already installed, Splat, GCC 2.7.2, and the necessary GCC binutils for compiling and linking.
1. Copy `baserom.us.z64` to the project's root directory (must provide your own and must be in big endian/z64 format).
1. Run `make setup && make` (with `VERBOSE=1` optional)

## Asset extraction

### Sprites

Sprites can be extracted and converted to png files using the `extract.py` script in the `tools` directory (`make extract-sprites` or `cd tools && python3 extract_sprites.py`). Extracted assets will be put in `./assets/sprites` folder. The locations for all used sprite assets are listed in the `sprite_addresses.csv` file in the tools directory.

There are two sprite asset formats used. Both types have a spritesheet at the beginning and an asset lookup table that follows it. Type 1 (used with character and map object sprites) includes a separate spritesheet index at the end of the asset, while type 2 (for smaller sprites, such as overlay icons) has the spritesheet index at the beginning of the actual spritesheet. Thus, the format for `sprite_addresses.csv` is: 
- type 1: spritesheet start, asset lookup table start, spritesheet index start, asset end, and asset name
- type 2: spritesheet start, asset lookup table start, asset end, and asset name.

### Animations

The game contains two layers of animation data: animation scripts compiled in the data section and another set of metadata stored on the sprite asset in the binary section of the rom. The binary data contains metadata for the animation sequence (total frame count, duration in game ticks, contains audio trigger) and metadata for the final rendered bitmap. The scripts are bitpacked arrays that contain an index into the animation metadata table, an animation type, and a flag for whether to flip horizontally.

#### Animation Scripts
 
These animation scripts are automatically generated as `.inc.c` files by Splat (via an extension), but they can be manually generated as well via `make extract-animation-scripts`. Running this script will generate a c file in `src/data/animations` for each specified asset in the `animation_addresses.csv` file. 

### Gifs

There's now support for converting animations to gifs. These seem to be working with all sprites, but there are issues with sprite anchoring for frames with layered sprites.

To create gifs, simply run `make extract-animations`, which will fetch the animation metadata, necessary sprites, and then run the gif conversion script, with all the generated assets in the `/assets/animations` directory. 

### Text

Text data (including decoding of control characters that encode character avatar information, new lines, etc.) can now be dumped via `make extract-texts`. Currently, only English characters are supported (there appears to be at least one text bank in Japanese included in the US version of the game).

The text system has two key components: special opcode characters (0-10), which `system/message.c` functions parse and use to modify message boxes and printing, and a 1-byte bitfield number that appears every 8 characters. The bitfield encodes which characters in an 8-character stream are 1 or 2 bytes. I.e., 0x03 (b0000 0011) has bits 1 and 2 set, which means the first and second characters are 2-bytes long, while the rest are 1 byte.

The character values themselves are used by the game code as offsets into the font texture array, adjusted for the opcode characters by subtracting 11. 

### Dialogue Bytecode

Dialogue bytecode can be both compiled (from DSL) and disassembled into text and json via `make extract-dialogues` (only a handful of dialogue bytecode segments are listed out currently). The dialogue bytecode is a somewhat sophisticated system that allows for branching to different texts within a text bank based on various conditions (game variable values, like NPC affection; random values; special dialogue status bits) and also setting game variable values and special dialogue bits. The bytecode also supports spawning sub-dialogue boxes (such as with choice menus).

Decompiled dialogue bytecode can be found in `src/bytecode/dialogues`.

### Maps

Map tiles and "core map objects" can now be extracted via `make extract-map-sprites`, and maps can now be rendered in Blender via the `./tools/modding/map/blender_import.py` script (follow the README in that directory for more information).

Map assets consist of 10 sub-assets:
- 1.) The map grid information, which includes basic map metadata and a mapping between tile number and grid position.
- 2.) Mesh data, which is essentially tile vertex information. This consists of two parts:
    - a.) Individual vertex information, which contains a pointer to its coordinate data, how many vertices to use per tile, y offest information, etc.
    - b.) Drawing information, which includes flags, coloring data, and bitfields that encapsulate vertex order for triangle drawing commands
- 3.) Terrain quads used for certain entity height calculations
- 4.) Grid to level interaction indicies (i.e., which squares in the grid have a specific flag the game can use for game interaction logic, such as the home TV, signs, the vineyard wine barrel, etc.)
- 5.) Core map objects data (coordinates and flags)
- 6.) Tile textures
- 7.) Tile palettes
- 8.) Core map object textures
- 9.) Core map object palettes
- 10.) "Map addtions" metadata (new geometry added to the map once they're unlocked in the game, such as the house extensions)

The "core map objects" are sprites that are always rendered on top of the geometry, independent of game status, which includes things like trees and fences. 

The map code manages two separate display lists for the tiles and ground objects (foragable items) and appends other sprites to the scene graph indirectly via calling `sprite.c` functions (i.e., weather sprites, map objects set up by the game, map additions, and the core map objects).

### Cutscenes

The repo now supports parsing cutscene bytecode and building the rom with matching DSL (see src/bytecode/cutscenes for matched segments). 

The bytecode system works by having variable length messages where the first two bits correspond to an index in a cutscene handler function table defined in `cutscene.c`. 

As an example, opcode 88 corresponds to `cutsceneHandlerSetSong`, which is the 88th function in the cutscene function table (starting count from 0). Each opcode is variable length, where the rest of the data after the first two bits are used by the handlng function (these data are usually parameters for other function calls). In the case of `cutsceneHandlerSetSong`, the parameters are song index (16 bits), a pointer to the rom address for the song start (32 bits), and a pointer to the rom address for the song end (32 bits). The handler function then calls `setSong(songIndex, songStart, songEnd);` from `audio.c` and increments the bytecode pointer for the next message.

Compared to the dialogue bytecode, the cutscene bytecode is much more sophisticated and involves multiple bytecode executors. I.e., the initial executor can spawn child executors that execute different segments of the bytecode and can spawn their own child executors as well. To handle this, the default `make extract-cutscenes` command generates 3 parses of each bytecode segment: a linear scan, a graph-based scan that tracks spawns, branches, and subroutine jumps, and a disassembled assembly file.

## Modding

### Music

See the README at `tools/modding/music` for more details for converting and inserting MIDI into existing song slots.

## Contributing

Contributions are much welcome. There are a few areas of work still do in the project:
- Cleaning up fake/forced matches (searchable under `FIXME`)
- Research into function, struct member, flag, and variable purposes and making accurate labels. This also includes adding macro values, such as player actions (see `player.h`)
- Cleaning up and improving asset tools (most are heavily vibecoded)
- Improving labeling and macro usage in bytecode files for readability
- JP version matching (only basic project scaffolding is done so far)

For function matching work, Decomp.me has a `Harvest Moon 64` compiler preset that's selectable when creating new scratches.

## Legal Disclaimer

### Intellectual Property Notice
This project is a non-commercial, fan-made modification (mod) for **Harvest Moon 64**. 

* **Harvest Moon** is a registered trademark of **Natsume Inc.**
* All original game assets, including but not limited to code, graphics, characters, and music, are the property of **Marvelous Inc.** (formerly Victor Interactive Software).
* This project is not affiliated with, endorsed by, or sponsored by Natsume Inc., Marvelous Inc., or any of their subsidiaries.

### Non-Commercial Use & Fair Use
This modification is provided for **educational and portfolio purposes only**. 
* **No Commercial Gain:** This mod is distributed free of charge. I do not accept donations or payments for this project.
* **Original Game Required:** This repository does not contain any original game ROMs or copyrighted proprietary binaries. Users must provide their own legally obtained copy of the original game to use this mod.
* **Fair Use:** I believe this work constitutes a "fair use" of copyrighted material as provided for in section 107 of the US Copyright Law (for purposes such as criticism, comment, and scholarship).

### No Liability
This software is provided "as is," without warranty of any kind. I am not responsible for any damage to your hardware, software, or data that may result from the use of this mod.

### Removal Request
If you are a copyright holder and believe this project infringes on your rights, please contact me via [GitHub Issue](https://github.com/elnanni/hm64-decomp-mod/issues) or email (elnanni@gmail.com), and I will address your concerns or remove the content immediately.

