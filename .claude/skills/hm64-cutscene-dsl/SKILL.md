---
name: hm64-cutscene-dsl
description: Use when writing, editing, or generating HM64 cutscene DSL scripts (.cutscene files), or when adding a brand-new cutscene bank to the game (source-build modding on `dev` / `dev-qol`). Covers bytecode opcodes, DSL syntax, transpiler directives, built-in constants, common cutscene patterns, and the spec / C-array / index-header wiring needed to make a new cutscene reachable in-game. Trigger on mentions of cutscene DSL, .cutscene files, cutscene bytecode, cutscene scripting, adding a new cutscene, or wiring up a new cutscene bank.
---

# HM64 Cutscene DSL Reference

Full human-readable documentation: `docs/cutscene-dsl-guide.md` (DSL authoring) and `docs/cutscene-readability-guide.md` (cleanup patterns).

## Overview

The HM64 cutscene system uses a custom DSL that transpiles to GNU assembler, which is then assembled into bytecode. The bytecode is a sequence of variable-length commands (opcodes) executed by the cutscene executor engine (`src/system/cutscene.c`).

**Build pipeline:** `.cutscene` -> `cpp` (C preprocessor) -> `transpiler` (Python) -> `.s` (GNU as) -> `.bin.o`

## DSL Syntax

### Comments
```
; Semicolon comment
# Hash comment
// Double-slash comment
```

### Labels
Labels define branch/subroutine targets. They end with a colon:
```
.entry:              ; Local label (transpiled to .L_entry)
.setup_player:       ; Descriptive local label
my_global_label:     ; Global label (no dot prefix)
```

Labels starting with `.` are automatically prefixed with `.L_` to avoid conflicts with GNU assembler directives (e.g., `.end` -> `.L_end`).

### Commands
Commands are opcode names followed by comma-separated parameters:
```
COMMAND_NAME param1, param2, param3
```

The `CMD_` prefix is optional -- `SET_WAIT_FRAMES` and `CMD_SET_WAIT_FRAMES` are equivalent.

### Preprocessor
Since `cpp` runs first, standard C preprocessor directives work:
```c
#include "entities.h"    // Include header macros
#define MY_CONSTANT 42   // Define constants
```

Include paths available: `src/buffers`, `src/game`, `src/assetIndices`.

## Directives

Place directives at the top of the file, after `#include`/`#define` lines.

### `.buffer <num|addr>` -- Set local variable buffer base
```
.buffer 1    ; Buffer 1 at 0x802F4000 (used by farmBusiness)
.buffer 2    ; Buffer 2 at 0x802F5000 (used by most banks)
```

### `.local <name>, <type> [, <initial_value>]` -- Local buffer variable
Auto-allocated at sequential offsets within the buffer. Resolves to absolute address.
```
.local var0, u32, 0
.local var1, u32, 0
.local counter, u16, 0
```
Types: `u8`, `u16`, `u32`, `s8`, `s16`, `s32`

### `.array <name>, <element_type>` -- Declare external array
Enables `name[INDEX]` syntax by declaring element size:
```
.array npcAffection, u8       ; 1-byte elements
.array lifeEventBits, u32     ; 4-byte elements
.array dailyEventBits, u32
```

### `.var <name>, <type>, <value>` -- Linker-resolved variable
Emits a labeled data value in the output assembly:
```
.var myFlag, u8, 0
```

### `.struct <instance>, <TypeName>` -- Struct member access
```
.struct gPlayer, Player
.struct horseInfo, Horse
.struct gRacingContext, RacingContext
```
Available types: `Player`, `Dog`, `Horse`, `RacingContext`.
Access members with dot syntax: `gPlayer.currentStamina`, `horseInfo.name[2]`

### Data directives
```
.byte 0x00, 0x01       ; Raw bytes
.half 0x1234            ; 16-bit halfwords
.long 0x12345678        ; 32-bit words
.balign 4               ; Align to 4 bytes
```

### Animation directives
```
.anim_frame id, flag, (x, y, z), wait, flip
.anim_end
.anim_loop .target_label
```

## Built-in Constants

The transpiler resolves these automatically (no `#define` needed).

### Directions (0-7)
`SOUTHWEST`=0, `WEST`=1, `NORTHWEST`=2, `NORTH`=3, `NORTHEAST`=4, `EAST`=5, `SOUTHEAST`=6, `SOUTH`=7

### Entity Assets (0-101)
All `ENTITY_ASSET_*` names from `entities.h`:
`ENTITY_ASSET_PLAYER`=0, `ENTITY_ASSET_MARIA`=1, `ENTITY_ASSET_POPURI`=2, `ENTITY_ASSET_ELLI`=3, `ENTITY_ASSET_ANN`=4, `ENTITY_ASSET_KAREN`=5, `ENTITY_ASSET_BABY`=6, `ENTITY_ASSET_HARRIS`=7, `ENTITY_ASSET_GRAY`=8, `ENTITY_ASSET_JEFF`=9, `ENTITY_ASSET_CLIFF`=10, `ENTITY_ASSET_KAI`=11, `ENTITY_ASSET_MAYOR`=12, `ENTITY_ASSET_DOUG`=17, `ENTITY_ASSET_GOTZ`=18, `ENTITY_ASSET_POTION_SHOP_DEALER`=20, `ENTITY_ASSET_KENT`=21, `ENTITY_ASSET_STU`=22, `ENTITY_ASSET_RICK`=25, `ENTITY_ASSET_SHIPPER`=27, `ENTITY_ASSET_MASTER_CARPENTER`=32, `ENTITY_ASSET_HARVEST_SPRITE`=33, `ENTITY_ASSET_GOURMET_JUDGE`=37, `ENTITY_ASSET_HARVEST_GODDESS`=49, `ENTITY_ASSET_COW`=71, `ENTITY_ASSET_HOLDABLE_ITEMS_1`=91, `ENTITY_ASSET_HOLDABLE_ITEMS_2`=93, `ENTITY_ASSET_TOOLS`=95, ... (see `tools/libhm64/cutscenes/transpiler.py` BUILTIN_CONSTANTS for full list)

### NPC Indices (for array indexing)
Short aliases: `MARIA`=0, `POPURI`=1, `ELLI`=2, `ANN`=3, `KAREN`=4, `BABY`=5, `HARRIS`=6, `GRAY`=7, `JEFF`=8, `CLIFF`=9, `KAI`=10

Full `NPC_*` names: `NPC_MARIA`=0 through `NPC_KAREN_KAI_BABY`=0x2D (see transpiler for full list)

### Map Names
All map/location names: `RANCH_SPRING`=0, `RANCH_SUMMER`=1, ..., `BEACH_SPRING`=9, ..., `VILLAGE_1_SPRING`=0x29, ..., `SQUARE_SPRING`=0x35, ..., `TAVERN`=0x3B, `LIBRARY`=0x3C, ..., `VINEYARD_SPRING`=0x47, ..., `FARM_SPRING`=0x52, ..., `HOUSE`=0x57, `BARN`=0x58, `COOP`=0x59, `KITCHEN`=0x5A, `BATHROOM`=0x5B

### Text Bank Indices
`TEXT_CUTSCENES_1`=45, `TEXT_FUNERAL_INTRO`=46, `TEXT_DOG_RACE`=52, `TEXT_NEW_YEAR_FESTIVAL`=56, `TEXT_SPIRIT_FESTIVAL`=57, `TEXT_HORSE_RACE`=58, `TEXT_EGG_FESTIVAL`=65, `TEXT_VEGETABLE_FESTIVAL`=67, `TEXT_HOW_TO_PLAY`=72, ... (73 total)

### Buttons
`BUTTON_A`=0x8000, `BUTTON_B`=0x4000, `BUTTON_Z`=0x2000, `BUTTON_START`=0x1000, `BUTTON_L`=0x0020, `BUTTON_R`=0x0010, `BUTTON_C_UP`=0x0008, `BUTTON_C_DOWN`=0x0004, `BUTTON_C_LEFT`=0x0002, `BUTTON_C_RIGHT`=0x0001, `BUTTON_D_UP`=0x0800, `BUTTON_D_DOWN`=0x0400, `BUTTON_D_LEFT`=0x0200, `BUTTON_D_RIGHT`=0x0100, `BUTTON_STICK_UP`=0x10000, `BUTTON_STICK_DOWN`=0x20000, `BUTTON_STICK_LEFT`=0x40000, `BUTTON_STICK_RIGHT`=0x80000

### Other
`TRUE`=1, `FALSE`=0, `ENTITY_PLAYER`=0, `CUTSCENE_ASSET_BEHAVIOR_TRACK_ENTITY`=1

## Opcode Reference

All 98 opcodes organized by category. Parameter types: `u8`/`u16`/`u32`/`s8`/`s16`/`s32` (integers), `rel16` (label reference for branching), `addr32` (memory address/symbol), `rom_start32`/`rom_end32` (ROM segment boundaries).

### Flow Control

| Command | Parameters | Description |
|---------|-----------|-------------|
| `SET_WAIT_FRAMES` | frames(u16) | Wait N frames before next command |
| `DEACTIVATE_SELF` | | Stop this executor |
| `EXECUTE_SUBROUTINE` | target(label) | Call subroutine, returns with RETURN_FROM_SUBROUTINE |
| `RETURN_FROM_SUBROUTINE` | | Return from subroutine |
| `SPAWN_EXECUTOR` | executor(u16), target(label) | Start parallel executor at label |
| `SET_OTHER_EXECUTOR_PTR` | executor(u16), target(label) | Redirect another executor to label |
| `DEACTIVATE_EXECUTOR` | executor(u16) | Stop another executor |
| `PAUSE_EXECUTOR` | executor(u16) | Pause an executor |
| `TOGGLE_PAUSE_EXECUTOR` | executor(u16) | Toggle pause on executor |
| `PAUSE_ALL_CHILD_EXECUTORS` | | Pause all child executors |
| `TOGGLE_PAUSE_ALL_CHILDREN` | | Toggle pause on all children |
| `SET_CALLBACK_ROUTINE` | entity(u16), button(u16), target(label) | Set button callback |

### Branching

| Command | Parameters | Description |
|---------|-----------|-------------|
| `BRANCH_ON_CURRENT_BUTTON` | entity(u16), button(u32), target(label) | Branch if button currently held |
| `BRANCH_ON_BUTTON_PRESSED` | entity(u16), button(u32), target(label) | Branch if button just pressed |
| `BRANCH_ON_BUTTON_REPEAT` | entity(u16), button(u32), target(label) | Branch on button repeat |
| `BRANCH_U8_VAR_WITHIN_RANGE` | min(u8), max(u8), address(addr), target(label) | Branch if u8 value in [min,max] |
| `BRANCH_U16_VAR_WITHIN_RANGE` | min(u16), max(u16), address(addr), target(label) | Branch if u16 value in [min,max] |
| `BRANCH_U32_VAR_WITHIN_RANGE` | min(u32), max(u32), address(addr), target(label) | Branch if u32 value in [min,max] |
| `BRANCH_ON_SPECIAL_BIT` | bit(u16), address(addr), target(label) | Branch if bit is set |
| `BRANCH_ON_ENTITY_DIR` | entity(u16), direction(u8), target(label) | Branch if entity faces direction |
| `BRANCH_ON_DIALOGUE` | dialogue_index(u16), unk(u16), target(label) | Branch on dialogue result |
| `BRANCH_ON_RANDOM` | min(u16), max(u16), target(label) | Branch randomly within range |
| `BRANCH_IF_U16_PTR_IN_RANGE` | ptr(addr), min_ptr(addr), max_ptr(addr), target(label) | Branch if ptr value in range |
| `BRANCH_IF_U8_PTR_RANGE` | ptr(addr), min_ptr(addr), max_ptr(addr), target(label) | Branch if u8 ptr in range |

### Entity Setup & Properties

| Command | Parameters | Description |
|---------|-----------|-------------|
| `SETUP_ENTITY` | entity(u16), asset(u16), variant(u16) | Create entity with asset sprite |
| `SET_ENTITY_ANIMATIONS` | entity(u16), a0-a5(u16 x6) | Set 6 animation indices |
| `SET_ENTITY_ANIMATION` | anim(u16) | Set current animation |
| `SET_ENTITY_ANIMATION_WITH_DIRECTION_CHANGE` | anim(u16) | Set animation and update direction |
| `SET_BEHAVIOR_FLAGS` | flags(u8) | Set entity behavior (1=track entity) |
| `SET_ENTITY_NON_COLLIDABLE` | | Disable collision |
| `SET_ENTITY_MAP_SPACE_INDEPENDENT_FLAG` | flag(u16) | Set map-space independent flag |
| `SET_ENTITY_MAP_SPACE_INDEPENDENT` | flag(u16) | Set map-space independent |
| `SET_ENTITY_PHYSICS_FLAGS` | flags(u16) | Set physics flags |
| `SET_ENTITY_PALETTE` | palette(u16) | Set entity palette |
| `SET_ENTITY_DIMENSIONS` | width(u8), height(u8) | Set collision dimensions |
| `SET_ENTITY_TRACKING_TARGET` | target_sprite(u16), x(s16), y(s16), z(s16), tracking_mode(u8) | Set camera/tracking target |
| `SET_ENTITY_WANDER` | movement_distance(u8), collision_width(u16), collision_height(u16) | Set wander behavior |
| `SET_ENTITY_FLAG_1` / `PAUSE_ENTITIES` | | Pause all entities |
| `SET_ENTITY_FLAG_2` / `TOGGLE_PAUSE_ENTITIES` | | Toggle pause on all entities |
| `SET_ENTITY_FLAG_3` / `FLIP_ENTITY_ANIMATION` | | Flip entity animation |
| `FLIP_ENTITY_DIRECTION` | entity(u16) | Flip entity facing direction |
| `PAUSE_ENTITY` | entity(u16) | Pause specific entity |
| `TOGGLE_PAUSE_ENTITY` | entity(u16) | Toggle pause on entity |
| `SET_HOLDING_ANIMATION_FLAG` | useAltAnimations(u16) | Switch to holding animations |

### Movement & Position

| Command | Parameters | Description |
|---------|-----------|-------------|
| `SET_COORDINATES` | x(s16), y(s16), z(s16) | Set entity position |
| `SET_FRAME_DELTA` | x(s8), y(s8), z(s8), wait(u8) | Move by delta per frame |
| `SET_ASSET_ROTATION` | direction(u8) | Set facing direction (use direction constants) |
| `ENTITY_WALK` | distance(u16), direction(u8), speed(u8) | Walk in direction |
| `ENTITY_RUN` | distance(u16), direction(u8), speed(u8) | Run in direction |
| `CHECK_ENTITY_COLLISION` | entity(u16), target(label) | Branch on collision |
| `WAIT_ENTITY_ANIMATION` | | Wait for animation to finish |

### Memory Operations

| Command | Parameters | Description |
|---------|-----------|-------------|
| `SET_U8_VALUE` | value(u8), address(addr) | Write u8 to address |
| `SET_U16_VALUE` | value(u16), address(addr) | Write u16 to address |
| `SET_U32_VALUE` | address(addr), value(u32) | Write u32 to address |
| `UPDATE_U8_VALUE` | delta(s16), max(u8), address(addr) | Increment u8 clamped to max |
| `UPDATE_U16_VALUE` | delta(s16), max(u16), address(addr) | Increment u16 clamped to max |
| `UPDATE_U32_VALUE` | delta(s32), max(s32), address(addr) | Increment u32 clamped to max |
| `SET_SPECIAL_BIT` | bit(u16), address(addr) | Set a bit in a bitfield |
| `TOGGLE_SPECIAL_BIT` | bit(u16), address(addr) | Toggle a bit in a bitfield |

### Message & Dialogue

| Command | Parameters | Description |
|---------|-----------|-------------|
| `INIT_MESSAGE_BOX_TYPE1` | box(u16), bank(u16), index(u16) | Show message from text bank |
| `INIT_MESSAGE_BOX_TYPE2` | box(u16), bank(u16), index(u16) | Show message (type 2) |
| `WAIT_MESSAGE_BOX_CLOSED` | box(u16) | Wait for player to close message |
| `SET_MESSAGE_BOX_VIEW_POSITION` | box(u16), x(s16), y(s16), z(s16) | Set message box camera position |
| `RESET_MESSAGE_BOX_AVATAR` | box(u16) | Reset message box avatar |
| `SET_MESSAGE_BOX_ASSETS` | spriteIndex(u16), dialogueWindowIndex(u8), overlayIconIndex(u8), characterAvatarIndex(u8) | Configure message box appearance |
| `SET_MESSAGE_INTERPOLATION` | rate(s16), message_box_index(s16), flag(u8) | Set message scroll rate |
| `UPDATE_MESSAGE_BOX_RGBA` | box(u16), r(u8), g(u8), b(u8), a(u8), speed(u16) | Fade message box |
| `WAIT_MESSAGE_BOX_READY` | box(u16) | Wait for message box ready |
| `INIT_DIALOGUE_SESSION` | box(u16), bank(u16), index(u16) | Start dialogue bytecode |
| `WAIT_FOR_DIALOGUE_INPUT` | box(u16) | Wait for dialogue input |
| `BRANCH_ON_DIALOGUE` | dialogue_index(u16), unk(u16), target(label) | Branch on dialogue result |

### Map & Rendering

| Command | Parameters | Description |
|---------|-----------|-------------|
| `LOAD_MAP` | mapControllerIndex(u16), mapIndex(u16) | Load a map |
| `WAIT_MAP_LOAD` | map(u16) | Wait for map to finish loading |
| `SETUP_MAP_ASSET` | mapIndex(u16) | Setup map asset |
| `SET_MAP_ROTATION` | mapIndex(u16), arg1(u8), rotation(u8) | Set map rotation |
| `DEACTIVATE_MAP_OBJECTS` | mapIndex(u16) | Deactivate map objects |
| `DEACTIVATE_MAP_CONTROLLERS` | | Deactivate map controllers |
| `INIT_MAP_ADDITION` | mapAdditionIndex(u16), flag(u16) | Initialize map addition |
| `SET_MAP_ADDITION` | a(u16), b(u8), c(u8) | Set map addition properties |
| `SET_MAP_GROUND_OBJECT` | a(u16), b(u16), c(u16) | Set ground object |
| `SET_RGBA` | r(u8), g(u8), b(u8), a(u8) | Set screen color/fade |
| `UPDATE_RGBA` | r(u8), g(u8), b(u8), a(u8), speed(s16) | Animate screen color |
| `UPDATE_GLOBAL_RGBA` | r(u8), g(u8), b(u8), a(u8), speed(s16) | Animate global color |
| `WAIT_RGBA_FINISHED` | | Wait for color animation |
| `DEACTIVATE_SPRITES` | | Deactivate all sprites |

### Sprite Properties

| Command | Parameters | Description |
|---------|-----------|-------------|
| `DMA_SPRITE` | sprite(u16), asset_type(u16), rom_tex_start, rom_tex_end, rom_assets_start, rom_assets_end, rom_sheet_start, rom_sheet_end, tex1_vaddr, tex2_vaddr, palette_vaddr, anim_vaddr, s2p_vaddr, sheet_idx_vaddr | DMA sprite data (14 params) |
| `SET_ENTITY_ANIMATIONS` | entity(u16), a0-a5(u16 x6) | Set animation bank offsets |
| `DO_DMA` | start(rom_start), end(rom_end), dest(addr) | Raw DMA transfer |
| `SET_ANIM_DATA_PTR_WITH_FLAG` | target(label) | Set animation data pointer |
| `SET_ANIM_DATA_PTR` | target(label) | Set animation data pointer |
| `SET_SHADOW_FLAGS` | entity(u16), flags(u16) | Set shadow rendering |
| `SET_SPRITE_SCALE` | x(u16), y(u16), z(u16) | Set sprite scale |
| `SET_SPRITE_RENDERING_LAYER` | layer(u16) | Set rendering layer |
| `SET_SPRITE_PALETTE` | palette(u16) | Set sprite palette |
| `SET_SPRITE_BILINEAR` | flag(u16) | Enable/disable bilinear filtering |

### Audio

| Command | Parameters | Description |
|---------|-----------|-------------|
| `SET_AUDIO_SEQUENCE` | channel(u16), start(rom_start), end(rom_end) | Play audio sequence |
| `STOP_SEQUENCE_WITH_FADE` | channel(u16), speed(u16) | Fade out audio |
| `SET_AUDIO_SEQUENCE_VOLUME` | songIndex(u16), targetVolume(u16), volume(s16) | Set volume |
| `SET_SFX` | sfx(u16), volume(u16) | Play sound effect |
| `IDLE_WHILE_SEQUENCE_PLAYING` | channel(u16) | Wait for audio to finish |

## Common Patterns

### Standard File Header
```
; ============================================
; Bank N - Description
; ============================================

#include "entities.h"
#include "texts.h"

.buffer 2

.local var0, u32, 0

.array npcAffection, u8
.array lifeEventBits, u32
.array dailyEventBits, u32
```

### Entry Point with Cutscene Index Dispatch
```
.entry:
    TOGGLE_PAUSE_ENTITIES
    EXECUTE_SUBROUTINE .setup_message_box
    BRANCH_U16_VAR_WITHIN_RANGE 750, 750, gCutsceneIndex, .cutscene_750
    BRANCH_U16_VAR_WITHIN_RANGE 751, 751, gCutsceneIndex, .cutscene_751
```

### Cutscene Completion
```
.complete:
    SET_SPECIAL_BIT 0, gCutsceneCompletionFlags
    DEACTIVATE_SELF
```

### Infinite Wait Loop (subroutine)
```
.wait_loop:
    SET_WAIT_FRAMES 1
    EXECUTE_SUBROUTINE .wait_loop
```

### Message Box Setup Subroutine
```
.setup_message_box:
    SET_MESSAGE_INTERPOLATION -4, 0, 0
    SET_MESSAGE_BOX_ASSETS 0, 0, 0, 0
    SET_MESSAGE_BOX_VIEW_POSITION 0, 24, -64, 352
    RETURN_FROM_SUBROUTINE
```

### Player Entity Setup
```
.setup_player:
    SETUP_ENTITY 0, ENTITY_ASSET_PLAYER, 1
    SET_ENTITY_ANIMATIONS 0, 0, 8, 16, 24, 32, 40
    SET_BEHAVIOR_FLAGS CUTSCENE_ASSET_BEHAVIOR_TRACK_ENTITY
    RETURN_FROM_SUBROUTINE
```

### NPC Entity Setup
```
.setup_karen:
    SETUP_ENTITY 36, ENTITY_ASSET_KAREN, 1
    SET_ENTITY_ANIMATIONS 36, 0, 8, 16, 24, 32, 40
    SET_BEHAVIOR_FLAGS 0
    SET_ENTITY_NON_COLLIDABLE
    RETURN_FROM_SUBROUTINE
```

### Show Message and Wait
```
    INIT_MESSAGE_BOX_TYPE1 0, CUTSCENES_1_TEXT_INDEX, 3
    WAIT_MESSAGE_BOX_CLOSED 0
    RESET_MESSAGE_BOX_AVATAR 0
```

### Screen Fade In/Out
```
    ; Fade to black
    UPDATE_RGBA 0, 0, 0, 255, 4
    WAIT_RGBA_FINISHED
    ; ... scene change ...
    ; Fade in
    UPDATE_RGBA 0, 0, 0, 0, -4
    WAIT_RGBA_FINISHED
```

### Music Change
```
    STOP_SEQUENCE_WITH_FADE 0, 8
    IDLE_WHILE_SEQUENCE_PLAYING 0
    SET_AUDIO_SEQUENCE 0, _festivalThemeSegmentRomStart, _festivalThemeSegmentRomEnd
    SET_AUDIO_SEQUENCE_VOLUME 0, 128, 8
```

### Character Movement Sequence
```
    SET_ASSET_ROTATION SOUTHEAST
    SET_COORDINATES -590, 0, 152
    ENTITY_WALK 34, SOUTHEAST, 2       ; Walk 34 units SE at speed 2
    ENTITY_WALK 90, SOUTHEAST, 2       ; Continue walking
    SET_ASSET_ROTATION NORTHEAST        ; Turn to face NE
```

### Parallel Executor (secondary character actions)
```
    SPAWN_EXECUTOR 2, .executor_2_entry
    ; ... main executor continues ...
    SET_OTHER_EXECUTOR_PTR 2, .executor_2_next_action  ; redirect executor 2

.executor_2_entry:
    SET_WAIT_FRAMES 8
    SETUP_ENTITY 31, ENTITY_ASSET_STU, 1
    SET_ENTITY_ANIMATIONS 31, 0, 8, 0, 0, 0, 0
    SET_BEHAVIOR_FLAGS 0
    SET_ASSET_ROTATION SOUTHEAST
    SET_COORDINATES -590, 0, 160
    ENTITY_WALK 34, SOUTHEAST, 2
    EXECUTE_SUBROUTINE .wait_loop
```

### Branching on Game State
```
    ; Check affection level
    BRANCH_U8_VAR_WITHIN_RANGE 200, 255, npcAffection[KAREN], .high_affection

    ; Check event bit
    BRANCH_ON_SPECIAL_BIT 5, lifeEventBits[1], .event_happened

    ; Check cutscene index
    BRANCH_U16_VAR_WITHIN_RANGE 750, 750, gCutsceneIndex, .cutscene_750
```

### Holdable Item Display
```
    SETUP_ENTITY 47, ENTITY_ASSET_HOLDABLE_ITEMS_2, 1
    SET_ENTITY_ANIMATIONS 47, 0, 0, 0, 0, 0, 0
    SET_BEHAVIOR_FLAGS 0
    SET_ENTITY_TRACKING_TARGET 0, 0, 14, 20, 255
    SET_ENTITY_ANIMATION 21          ; Select specific item frame
```

## Entity Index Conventions

- Entity 0: Always the player
- Entities 21-35: NPCs (ENTITY_NPC_BASE = 21)
- Entity 36+: Additional entities (items, cutscene-specific)
- Entity 47-48: Commonly used for holdable items in cutscenes

## Key Global Symbols

These are linker-resolved symbols used in cutscene DSL:
- `gCutsceneIndex` -- current cutscene ID being executed
- `gCutsceneCompletionFlags` -- bitfield marking cutscene completion
- `npcAffection` -- u8 array of NPC affection values
- `lifeEventBits` -- u32 array of persistent event flags
- `dailyEventBits` -- u32 array of daily-reset event flags
- `gPlayer` -- player struct (use with `.struct gPlayer, Player`)
- Audio segment symbols: `_segmentNameRomStart` / `_segmentNameRomEnd`

## Matching Build Considerations

For the matching decompilation build, the transpiled output must produce byte-identical bytecode. When editing existing cutscene files:
- Do NOT reorder commands or change control flow
- Only replace literals with equivalent macros/constants
- Verify with `make` after changes (runs checksum comparison)
- The `.buffer`, `.local`, `.array` directives affect output layout -- match existing patterns

## Adding a Brand-New Cutscene Bank (modding)

This workflow adds a new cutscene bank to the ROM from a fresh `.cutscene` source file. It is a **source-build modding flow**, not a matching change — the ROM will shift, and the default `master` build will fail its checksum the moment a new bank is linked in.

**Branch requirement.** Use `dev` or `dev-qol` and build with [Makefile.modding](Makefile.modding). Those branches already have shiftability wired up — `common_bss.ld` is cleared, `bssPadding.o` is removed, and `MODERN_GCC=1` is available. If the user is currently on `master`, check `git status` for uncommitted work and confirm before running `git checkout dev` / `dev-qol`.

**Do not use `make` / the default Makefile** for this flow — it will fail the checksum check as soon as any cutscene bank changes size. Always build with:

```bash
make -f Makefile.modding
```

Here's what the skill should do vs. what it should ask:

- **Do autonomously** (steps 1–6 below — the mechanical spec + dispatch-table + index-header + loader wiring). These are always required for any new bank. Do not ask permission to make them — just do them and report.
- **Ask before doing** (step 7 — the launch site). That depends on *when the new cutscene should fire*. Different triggers touch different files (`title.c`, `cutscenes.c`, `gameStart.c`, `game.c`, a map transition table, etc.). Ask the user first.

### Steps

Assume the new bank is called `myCutscene` (camelCase) and its cutscene index constant will be `MY_CUTSCENE`. The bank will take the next free bank index, which is **36** as of this writing — but **verify by counting entries in `cutsceneBytecodeAddresses[]`** in [src/game/cutscenes.c](src/game/cutscenes.c) since other mods may already have consumed it. Call that final index `<N>` in the steps below.

1. **Create the DSL source** at `src/bytecode/cutscenes/myCutscene.cutscene`. Use the Standard File Header + Entry Point patterns earlier in this doc. The `BRANCH_U16_VAR_WITHIN_RANGE ..., gCutsceneIndex, ...` dispatch inside `.main` / `.entry` is what picks which cutscene within the bank to run.

2. **Add a segment** to [assets/cutscenes/cutscenes.spec](assets/cutscenes/cutscenes.spec). The segment name must match the DSL filename base (camelCase):

   ```text
   beginseg
       name "myCutscene"
       flags OBJECT
       include "build/assets/cutscenes/myCutscene.bin.o"
   endseg
   ```

   The linker will emit `_myCutsceneSegmentRomStart` / `_myCutsceneSegmentRomEnd` from this entry. Do NOT add anything to [config/us/splat.us.yaml](config/us/splat.us.yaml) — splat only extracts what's already in the baserom, and a brand-new cutscene has nothing to extract.

3. **Add the build-object entry** to `CUTSCENE_OBJECTS` in [Makefile.modding](Makefile.modding) (appended to the list that currently ends with `$(CUTSCENE_BUILD_DIR)/howToPlay.bin.o`):

   ```make
   $(CUTSCENE_BUILD_DIR)/myCutscene.bin.o
   ```

   The generic `%.cutscene → %.s → %.bin.o` rules already in `Makefile.modding` will pick it up — no new rule needed.

4. **Add three parallel array entries** in [src/game/cutscenes.c](src/game/cutscenes.c), all indexed by bank number `<N>`. Every new bank must appear in all three arrays at the same index, and in `cutsceneExecutorIndices[]` as well. Mirror the pattern used by the last real entry (`howToPlay`, bank 35):

   - `cutsceneBytecodeAddresses[<N>]` → `{ &_myCutsceneSegmentRomStart, &_myCutsceneSegmentRomEnd }` (replacing the next `{ 0, 0 }` padding slot, or appending if no padding remains).
   - `cutsceneEntryPointAddresses[<N>]` → `CUTSCENE_BYTECODE_BUFFER_1 + CUTSCENE_BANK_<N>_ENTRY_OFFSET` **or** `CUTSCENE_BYTECODE_BUFFER_2 + ...`. The existing entries split across BUFFER_1 and BUFFER_2; pick whichever the new bank should load into (most non-farm banks use BUFFER_2 — mirror `howToPlay`).
   - `cutsceneBankLoadAddresses[<N>]` → the same `CUTSCENE_BYTECODE_BUFFER_1` or `CUTSCENE_BYTECODE_BUFFER_2` chosen in the previous line.
   - `cutsceneExecutorIndices[<N>]` → the starting executor slot (usually `0`; match the pattern used by neighboring banks).

5. **Add `#define`s** to [src/assetIndices/cutscenes.h](src/assetIndices/cutscenes.h):

   ```c
   #define MY_CUTSCENE <someIndex>          // the u16 value loadCutscene() will dispatch on
   #define CUTSCENE_BANK_<N>_ENTRY_OFFSET 8
   #define CUTSCENE_BANK_<N>_STARTING_EXECUTOR_INDEX 0
   ```

   **Picking the cutscene index value.** Existing banks own disjoint ranges (farmVisits ~50s, roadCutscenes ~125s, ..., EGG_FESTIVAL=1250, ..., HOW_TO_PLAY_CUTSCENE=1600). Pick a value in a range that isn't already claimed by another bank, and make sure the `loadCutscene()` if-ladder (step 6) recognizes it. If unsure, pick something comfortably above the last existing value (e.g. 1700+).

   **Entry offset.** `8` matches most existing banks. Only change this if the user wants a specific entry point inside their bank — most authors don't.

6. **Add an if-ladder branch** in `loadCutscene()` in [src/game/cutscenes.c](src/game/cutscenes.c) (starts around line 5699). The function maps `gCutsceneIndex` ranges to `bytecodeSegmentIndex`. Insert a new `else if (gCutsceneIndex >= MY_CUTSCENE_RANGE_MIN)` branch that sets `bytecodeSegmentIndex = <N>;`. Place it in the ladder so the range comparisons still work correctly (the ladder is written top-down from highest range to lowest).

   ```c
   } else if (gCutsceneIndex >= MY_CUTSCENE) {
       // my cutscene bank
       bytecodeSegmentIndex = <N>;
   }
   ```

   Match the setup other branches do (e.g. `setInitialMapRotation`, `gCameraRotationOffset = 0`, `memcpy(farmFieldTiles, ...)`) only if the new cutscene actually needs it.

### Ask the user before step 7

After finishing steps 1–6, stop and ask the user **how the cutscene should be triggered** before making any further edits. Offer the common patterns explicitly:

- **From a specific call site in C.** Most cutscenes are launched via:

  ```c
  launchIntroCutscene(MY_CUTSCENE, spawnPoint, entityType);
  ```

  The caller has to name the function or describe the moment (title screen, naming screen, on entering a map, a specific NPC interaction, etc.). Common existing call sites: `src/game/title.c`, `src/game/gameStart.c`, `src/game/game.c`, `src/game/cutscenes.c`. The `spawnPoint` arg is a spawn-point index — see [src/assetIndices/maps.h](src/assetIndices/maps.h) for the `*_SPAWN_POINT_*` macros.

- **From another cutscene** (chained). Inside the DSL, set `gCutsceneIndex` via a memory-write opcode and then let the current cutscene exit — the next-cutscene machinery will pick it up. Ask which existing `.cutscene` file should chain into the new one.

- **From a map transition or festival trigger.** Some cutscenes are keyed to season/day triggers; others fire on entering a specific map. Ask where the user wants it.

- **None yet — just want it linked into the ROM for testing.** That's fine. Steps 1–6 are sufficient for the bank to load and be reachable via a debug `gCutsceneIndex = MY_CUTSCENE; loadCutscene(FALSE);` call, but the user needs to confirm they'll wire a trigger later.

If the user answers more than one, do them all.

### Step 8 — Build

From the project root:

```bash
make -f Makefile.modding
```

The build will produce a shifted ROM that is NOT byte-identical to the original — this is expected. If the build fails at the link step with unresolved `_myCutsceneSegmentRomStart` / `_myCutsceneSegmentRomEnd` symbols, the spec entry (step 2) was missed or the segment `name` doesn't match the DSL filename base.

### What you do NOT need to do

- Do NOT touch [config/us/splat.us.yaml](config/us/splat.us.yaml). Splat extracts from the baserom; a new cutscene has no baserom bytes to extract.
- Do NOT run `make extract-cutscenes`. That re-populates `assets/cutscenes/` from the baserom and would not generate anything for a new bank.
- Do NOT edit `config/us/symbol_addrs.txt` — that's for matching/decomp naming, not for new content.
- Do NOT run the default `make` to test this — it will fail the checksum check. Always `make -f Makefile.modding`.
