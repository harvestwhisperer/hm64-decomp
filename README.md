# WIP Harvest Moon 64 Decomp

## Setting up

Note: The current workflow has only been tested on WSL. 

To build the project, first run `git submodule init` and `./tools/setup.sh`. This will fetch Splat, GCC 2.7.2, and the necessary GCC binutils. Rename your rom `baserom.us.z64` (must be in big endian/z64 format) and place it in the root directory before running `make setup && make VERBOSE=1`.

## Progress

Currently, about 33% of the 1459 game functions have been decompiled, and most library functions have been identified.
