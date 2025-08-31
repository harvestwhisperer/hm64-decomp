# WIP Harvest Moon 64 Decomp

## Progress

Currently, 70% of the 1467 game functions and 61% of the game source files have been decompiled. All library functions have also been identified with much progress on the matching (many thanks to cblck for the great work). More robust progress tracking is in the works.

## Asset extraction

### Sprites

Sprites can be extracted and converted to png files using the `extract.py` script in the `tools` directory (`make extract-sprites` or `cd tools && python3 extract_sprites.py`). Extracted assets will be put in `./assets/sprites` folder. A number of sprite asset locations are already listed in the `sprite_addresses.csv` file in the tools directory (see next paragraph for formatting).

There are two sprite asset formats used. Both types have a spritesheet at the beginning and an asset lookup table that follows it. Type 1 (used with character and map object sprites) includes a separate spritesheet index at the end of the asset, while type 2 (for smaller sprites, such as overlay icons) has the spritesheet index at the beginning of the actual spritesheet. Thus, the format for `sprite_addresses.csv` is: 
- type 1: spritesheet start, asset lookup table start, spritesheet index start, asset end, and asset name
- type 2: spritesheet start, asset lookup table start, asset end, and asset name.

### Animations

The game contains two layers of animation data: animation scripts compiled in the data section and another set of metadata stored in the sprite asset in the binary section of the rom. The binary data contains metadata for the animation sequence (total frame count, duration in game ticks, contains audio trigger) and metadata for the final rendered bitmap. The scripts are bitpacked arrays that contain an index into the animation metadata table, an animation type, and whether to flip horizontally.

#### Animation Scripts
 
These animation scripts can now be generated via `make extract-animation-scripts` or `cd tools && python3 extract_animation_scripts.py`. Running this script will generate a c file in `src/data/animations` for each specified asset in the `animation_addresses.csv` file. 

### Gifs (experimental)

There's now experimental support for converting animations to gifs (via some messy, vibe-coded scripts). These seem to mostly be working for basic animations (one sprite per frame), but there are issues with sprite anchoring for frames with layered sprites.

To create gifs, simply run `make extract-animations`, which will fetch the animation metadata, necessary sprites, and then run the gif conversion script, with all the generated assets in the `/assets/animations` directory. 

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

## Known bugs

With the current version of Splat (0.35.1) and spimdisasm (1.36.0), the `asm_nonmatching_label_macro` setting must be set to `""` in order to preserve matching, otherwise spimdisasm will generate duplicate symbols for unmatched but already labeled functions, which affects the final build.

## Contributing

Contributions are much welcome. There are a few areas of work left in the project:
- Easy, repetitive (but numerous) functions, namely in `animals.c` and `npc.c`. These are great for getting started, as the structs and many examples functions are already available.
- Getting "almost" matches over the finish line: there are several functions that are 95-99% matching in the repo (searchable under `#ifdef PERMUTER`)
- Research into function, struct member, flag, and variable purposes and making accurate labels. This also includes adding macro values, such as player actions (see `player.h`)
- Research into binary assets. Currently, only the sprite and audio formats are understood. Some work remains for maps and dialogue formats.

For function matching work, Decomp.me has a `Harvest Moon 64` compiler preset that's selectable when creating new scratches. 