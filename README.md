# WIP Harvest Moon 64 Decomp

## Setting up

Note: The current workflow has only been tested on WSL. 

To build the project, first run `git submodule init` and `./tools/setup.sh`. This will fetch Splat, GCC 2.7.2, and the necessary GCC binutils. Rename your rom `baserom.us.z64` (must be in big endian/z64 format) and place it in the root directory before running `make setup && make VERBOSE=1`.

## Progress

Currently, about 51% of the 1451 game functions have been decompiled, and most library functions have been identified.

## Asset extraction

Sprites are now able to be extracted and converted to png files using the `extract.py` script if the relevant rom addresses are specified in the `sprite_addresses.csv` file (spritesheet start, sprite asset index table start, spritesheet index start, asset end). 

There are two sprite asset formats used. Type 1 includes a separate spritesheet index at the end of the asset, while type 2 (for smaller sprites, such as overlay icons) has the spritesheet index at the beginning of the actual spritesheet.