# WIP Harvest Moon 64 Decomp

## Setting up

Note: The current workflow has only been tested on WSL. 

To build the project, first run `git submodule init` and `./tools/setup.sh`. This will fetch Splat, GCC 2.7.2, and the necessary GCC binutils. Rename your rom `baserom.us.z64` (must be in big endian/z64 format) and place it in the root directory before running `make setup && make VERBOSE=1`.

## Progress

Currently, about 55% of the 1451 game functions have been decompiled, and most library functions have been identified.

## Asset extraction

Sprites are now able to be extracted and converted to png files using the `extract.py` script in the `tools` directory (`make extract` or `cd tools && python3 extract.py`). Extracted assets will be put in `./assets/sprites` folder. A handful of asset locations are already listed in the `sprite_addresses.csv` file in the tools directory (see next paragraph for formatting).

There are two sprite asset formats used. Both types have a spritesheet at the beginning and an asset lookup table that follows it. Type 1 (used with character and map object sprites) includes a separate spritesheet index at the end of the asset, while type 2 (for smaller sprites, such as overlay icons) has the spritesheet index at the beginning of the actual spritesheet. Thus, the format for `sprite_addresses.csv` is spritesheet start, asset lookup table start, spritesheet index start, asset end, and asset name for type 1, and spritesheet start, asset lookup table start, asset end, and asset name for type 2.
