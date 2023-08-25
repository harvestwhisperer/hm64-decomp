# WIP Harvest Moon 64 Decomp

## Setting up

Note: The current workflow has only been tested on WSL. 

To build the project, first run `git submodule init` and `./tools/setup.sh`. This will fetch Splat, GCC 2.7.2, and the necessary GCC binutils. Rename your rom `baserom.us.z64` (must be in big endian/z64 format) and place it in the root directory before running `make setup && make VERBOSE=1`.

Note: currently, the most recent version of the Splat dependency spimdisasm is breaking linking. `setup.sh` manually uninstalls and reinstalls a working version for now.

## Progress

Currently, about 27% of the 1459 game functions have been decompiled, and most library functions have been identified.
