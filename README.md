# Harvest Moon 64 Decomp - Dev

## Overview

This is a development branch set up to support modding out-of-the-box with some additional QOL changes already implemented. On Linux (Debian/Ubuntu)/WSL/Posix, simply run `./tools/setup.sh`, `make extract`, and `make -j4` to get started.

Currently supported modding workflows:
- All code changes
- Text editing and new texts (simply edit an existing text and rebuild the game)
- Cutscene editing and new cutscenes
- Dialogue logic editing and additional dialogues
- Audio sequence replacement

QOL implementations:
- Time slowed to 1/5th of the original game speed
- Farm ranking screen accessible in game select menu
- Music continues playing unless the entered map contains a different theme
- Naming screen music doesn't restart across screens during new game naming 
- Adds compression of map and sprite assets, saving 6.56 MiB, or a total of ~41% of the space of the 16MB cartridge
- Use cached pointers wherever possible, reducing up to 50% of RAM lookups across translation units

Replacing other assets (maps, sprites, animations, fonts) is fully supported in theory, but tooling to create/modify these assets is still in progress.

For this branch, `make extract` should only be run once. If needed, `make clean-all-dangerous` can be run, which will remove all texts (losing any edits). Note that the original dialogue bytecode relies on hardocded text indexes, so removing or changing existing texts may result in breaking the game. This also applies to hardcoded references to texts in the game code itself as well as the cutscene DSL.

Basic changes to the development branch:
- Removing unued assets (sprites, maps, texts) and unused files
- Commenting out unused functions
- Fixing of fake matches to enchance readability

Note that you may run into performance issues when emulating non-matching builds. This is because many emulators hardcode game-specific graphics and overclocking settings that get applied when a game matches a known hash. Check your emulator's settings for vanilla Harvest Moon 64 and make sure those are applied when your mod is loaded.

This branch has been tested on real hardware and should be robust. If you run into any issues, please raise an issue.

### Music

See the README at `tools/modding/music` for more details for converting and inserting MIDI into existing song slots.

### Cutscenes

If you're using an AI agent, you can use the Claude skill at `.claude/skills/hm64-cutscene-dsl/SKILL.md` to aid with creating or modifying cutscene DSL and adding it into the game.