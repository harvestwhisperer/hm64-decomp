# WIP Harvest Moon 64 Decomp

## Progress

Currently, 80% of the 1467 game functions and 77% of the game source files have been decompiled, with the entire game engine (the "system" according to leftover debug logs) now fully decompiled. All library functions have also been identified with much progress on the matching (many thanks to cblck for the great work). More robust progress tracking is in the works.

## Asset extraction

### Sprites

Sprites can be extracted and converted to png files using the `extract.py` script in the `tools` directory (`make extract-sprites` or `cd tools && python3 extract_sprites.py`). Extracted assets will be put in `./assets/sprites` folder. The locations for all used sprite assets are listed in the `sprite_addresses.csv` file in the tools directory.

There are two sprite asset formats used. Both types have a spritesheet at the beginning and an asset lookup table that follows it. Type 1 (used with character and map object sprites) includes a separate spritesheet index at the end of the asset, while type 2 (for smaller sprites, such as overlay icons) has the spritesheet index at the beginning of the actual spritesheet. Thus, the format for `sprite_addresses.csv` is: 
- type 1: spritesheet start, asset lookup table start, spritesheet index start, asset end, and asset name
- type 2: spritesheet start, asset lookup table start, asset end, and asset name.

All sprites with the exception of map tile data are listed in the `sprite_addresses.csv` file.

### Animations

The game contains two layers of animation data: animation scripts compiled in the data section and another set of metadata stored on the sprite asset in the binary section of the rom. The binary data contains metadata for the animation sequence (total frame count, duration in game ticks, contains audio trigger) and metadata for the final rendered bitmap. The scripts are bitpacked arrays that contain an index into the animation metadata table, an animation type, and a flag for whether to flip horizontally.

#### Animation Scripts
 
These animation scripts can now be generated via `make extract-animation-scripts` or `cd tools && python3 extract_animation_scripts.py`. Running this script will generate a c file in `src/data/animations` for each specified asset in the `animation_addresses.csv` file. 

### Gifs

There's now support for converting animations to gifs. These seem to be working with all sprites, but there are issues with sprite anchoring for frames with layered sprites.

To create gifs, simply run `make extract-animations`, which will fetch the animation metadata, necessary sprites, and then run the gif conversion script, with all the generated assets in the `/assets/animations` directory. 

### Text

Text data (including decoding of control characters that encode character avatar information, new lines, etc.) can now be dumped via `make extract-texts`. Currently, only English characters are supported (there appears to be at least one text bank in Japanese included in the US version of the gam).

The text system has two key components: special opcode characters (0-10), which `system/message.c` functions parse and use to modify message boxes and printing, and a 1-byte bitfield number that appears every 8 characters. The bitfield encodes which characters in an 8-character stream are 1 or 2 bytes. I.e., 0x03 (b0000 0011) has bits 1 and 2 set, which means the first and second characters are 2-bytes long, while the rest are 1 byte.

The character values themselves are used by the game code as offsets into the font texture array, adjusted for the opcode characters by subtracting 11. 

### Dialogue Bytecode

Dialogue bytecode can now be disassembled into text and json via `make extract-dialogues` (only a handful of dialogue bytecode segments are listed out currently). The dialogue bytecode is a somewhat sophisticated system that allows for branching to different texts within a text bank based on various conditions (game variable values, like NPC affection; random values; special dialogue status bits) and also setting game variable values and special dialogue bits. The bytecode also supports spawning sub-dialogue boxes (such as with choice menus).

Currently, the disassembly references game variables, special dialogue bits, and text indices by number, but I hope to eventually add in support for using their in-game identifiers.

### Maps

Map tiles and "core map objects" can now be extracted via `make extract-map-sprites`. 

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

Work is being done on cutscene bytecode interpretation. The bytecode system works by having variable length messages where the first two bits correspond to an index in a cutscene handler function table defined in `cutscene.c`. 

As an example, opcode 88 corresponds to `cutsceneHandlerSetSong`, which is the 88th function in the cutscene function table (starting count from 0). Each opcode is variable length, where the rest of the data after the first two bits are used by the handlng function (these data are usually parameters for other function calls). In the case of `cutsceneHandlerSetSong`, the parameters are song index (16 bits), a pointer to the rom address for the song start (32 bits), and a pointer to the rom address for the song end (32 bits). The handler function then calls `setSong(songIndex, songStart, songEnd);` from `audio.c` and increments the bytecode pointer for the next message.

Compared to the dialogue bytecode, the cutscene bytecode is much more sophisticated and involves multiple bytecode executors. I.e., the initial executor can spawn child executors that execute different segments of the bytecode and can spawn their own child executors as well.

## Setting up
1. Install [WSL2](https://learn.microsoft.com/en-us/windows/wsl/install)
1. WSL: `sudo apt-get update && sudo apt install -y python3 pip binutils-mips-linux-gnu gcc-mips-linux-gnu build-essential wget`

### WSL Workflow
1. `git clone --recursive https://github.com/harvestwhisperer/hm64-decomp.git`
1. `cd hm64-decomp`
1. `sudo chmod +x tools/setup.sh`
1. `sudo tools/setup.sh`. This will fetch needed system packages not already installd, Splat, GCC 2.7.2, and the necessary GCC binutils for compiling and linking.
1. Copy `baserom.us.z64` to the project's root directory (must provide your own and must be in big endian/z64 format).
1. Run `make setup && make` (with `VERBOSE=1` optional)

## Contributing

Contributions are much welcome. There are a few areas of work left in the project:
- Easy, repetitive (but numerous) functions (`npc.c` is a great example). These are great for getting started, as the structs and many examples functions are already available.
- Getting "almost" matches over the finish line: there are several functions that are 95-99% matching in the repo (searchable under `#ifdef PERMUTER`)
- Cleaning up fake/forced matches (searchable under `FIXME`)
- Research into function, struct member, flag, and variable purposes and making accurate labels. This also includes adding macro values, such as player actions (see `player.h`)

For function matching work, Decomp.me has a `Harvest Moon 64` compiler preset that's selectable when creating new scratches.
