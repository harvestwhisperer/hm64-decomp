# Harvest Moon 64 Decomp - Dev

## Overview

This is a development branch set up to support modding out-of-the-box. Simply run `./tools/setup.sh`, `make extract`, and `make -j4` to get started.

Currently supported modding workflows:
- All code changes
- Text editing and new texts (simply edit an existing text and rebuild the game)
- Cutscene editing and new cutscenes
- Dialogue logic editing and additional dialogues
- Audio sequence replacement

Replacing other assets (maps, sprites, animations, fonts) is supported in theory, but tooling to create/modify these assets is still in progress.

For this branch, `make extract` should only be run once. If needed, `make clean-all-dangerous` can be run, which will remove all texts (losing any edits). Note that the original dialogue bytecode relies on hardocded text indexes, so removing or changing existing texts may result in breaking the game. 

Basic changes to the development branch:
- Removing unued assets (sprites, maps, texts) and unused files
- Commenting out unused functions
- Fixing of fake matches to enchance readability

Note that you may run into performance issues when emulating non-matching builds. This is because many emulators hardcode game-specific graphics and overclocking settings that get applied when a game matches a known hash. Check your emulator's settings for vanilla Harvest Moon 64 and make sure those are applied when your mod is loaded.

### Music

See the README at `tools/modding/music` for more details for converting and inserting MIDI into existing song slots.
