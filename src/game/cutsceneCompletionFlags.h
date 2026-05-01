#ifndef _CUTSCENE_COMPLETION_FLAGS_H_
#define _CUTSCENE_COMPLETION_FLAGS_H_

// ---- bit indices for SET_SPECIAL_BIT in cutscene DSL ----

// lifecycle
#define CUTSCENE_COMPLETION_BIT_EXIT                  0
#define CUTSCENE_COMPLETION_BIT_CHAIN_NEXT            1

// post-race name publish
#define CUTSCENE_COMPLETION_BIT_PUBLISH_RACER_NAMES   2

// starry night festival sub-branch selectors
#define CUTSCENE_COMPLETION_BIT_STARRY_NIGHT_429      3
#define CUTSCENE_COMPLETION_BIT_STARRY_NIGHT_430      4
#define CUTSCENE_COMPLETION_BIT_STARRY_NIGHT_431      5

// one-shot side effects
#define CUTSCENE_COMPLETION_BIT_SPRITE_98_PALETTE_0   6
#define CUTSCENE_COMPLETION_BIT_SPRITE_98_PALETTE_1   7
#define CUTSCENE_COMPLETION_BIT_LOAD_FONT_PALETTE_3   8

// daily-event-bit mirrors
#define CUTSCENE_COMPLETION_BIT_DAY_CONSUMING         9
#define CUTSCENE_COMPLETION_BIT_BLOCK_DIARY          10
#define CUTSCENE_COMPLETION_BIT_RETURN_TO_TITLE      11
#define CUTSCENE_COMPLETION_BIT_SUSPEND_TIME         12
#define CUTSCENE_COMPLETION_BIT_BLOCK_PAUSE          13
#define CUTSCENE_COMPLETION_BIT_BLOCK_BUTTONS        14

// end-of-day held-item drop
#define CUTSCENE_COMPLETION_BIT_CLEAR_HELD_ITEM      15

// (bit 16 unused)

// map / sprite transitions; cleared by C cleanup after acting
#define CUTSCENE_COMPLETION_BIT_LOAD_FARM_MAP        17
#define CUTSCENE_COMPLETION_BIT_CHICKEN_FEED_0       18
#define CUTSCENE_COMPLETION_BIT_CHICKEN_FEED_1       19
#define CUTSCENE_COMPLETION_BIT_CHICKEN_FEED_2       20
#define CUTSCENE_COMPLETION_BIT_LOAD_COOP_MAP        21
#define CUTSCENE_COMPLETION_BIT_LOAD_BARN_MAP        22
#define CUTSCENE_COMPLETION_BIT_BARN_FODDER_0        23
#define CUTSCENE_COMPLETION_BIT_BARN_FODDER_2        24
#define CUTSCENE_COMPLETION_BIT_MAP_OBJECT_ANIM      25

// end-of-cutscene callbacks
#define CUTSCENE_COMPLETION_BIT_MINING_STRIKE        26
#define CUTSCENE_COMPLETION_BIT_REFRESH_HELD_ITEM    27
#define CUTSCENE_COMPLETION_BIT_REINIT_ANIMALS       28
#define CUTSCENE_COMPLETION_BIT_DEACTIVATE_ANIMALS   29

// per-cutscene conditional outcome -- different cutscenes use this to gate
// their own scripted post-effect (sprained-ankle / Karen yellow heart ->
// gHour += 2 + spawn-point change; festival farm visits -> END_OF_FESTIVAL_DAY_1).
// DSL clears it on the no-op branch and sets it on the firing branch.
#define CUTSCENE_COMPLETION_BIT_CONDITIONAL_OUTCOME  30

// cutscene owns audio + lighting (sign bit, tested via < 0 / >= 0).
// When set: setMapAudioAndLighting jumps to MAIN_GAME without setting
// audio/lighting, and time.c skips the per-tick setLevelLighting interpolation.
#define CUTSCENE_COMPLETION_BIT_OWN_AUDIO_LIGHTING   31

#define CUTSCENE_COMPLETION_EXIT                  0x1         // run per-cutscene exit handler
#define CUTSCENE_COMPLETION_CHAIN_NEXT            0x2         // dispatch follow-up cutscene via transitionCutscenes()
#define CUTSCENE_COMPLETION_PUBLISH_RACER_NAMES   0x4         // copy finishOrder racer names into gameVariableString 40/41
#define CUTSCENE_COMPLETION_STARRY_NIGHT_429      0x8         // STARRY_NIGHT_FESTIVAL -> 429
#define CUTSCENE_COMPLETION_STARRY_NIGHT_430      0x10        // STARRY_NIGHT_FESTIVAL -> 430
#define CUTSCENE_COMPLETION_STARRY_NIGHT_431      0x20        // STARRY_NIGHT_FESTIVAL -> 431
#define CUTSCENE_COMPLETION_SPRITE_98_PALETTE_0   0x40        // setSpritePaletteIndex(98, 0)
#define CUTSCENE_COMPLETION_SPRITE_98_PALETTE_1   0x80        // setSpritePaletteIndex(98, 1)
#define CUTSCENE_COMPLETION_LOAD_FONT_PALETTE_3   0x100       // reload fontPalette3 + reconfigure msgbox font/sfx
#define CUTSCENE_COMPLETION_DAY_CONSUMING         0x200       // sets DAY_CONSUMING_CUTSCENE (set-only)
#define CUTSCENE_COMPLETION_BLOCK_DIARY           0x400       // mirrors DIARY_BLOCKED
#define CUTSCENE_COMPLETION_RETURN_TO_TITLE       0x800       // funeral intro / demos: initializeTitleScreen(0)
#define CUTSCENE_COMPLETION_SUSPEND_TIME          0x1000      // mirrors SUSPEND_TIME_DURING_ANIMATION
#define CUTSCENE_COMPLETION_BLOCK_PAUSE           0x2000      // mirrors BLOCK_PAUSE_SCREEN
#define CUTSCENE_COMPLETION_BLOCK_BUTTONS         0x4000      // mirrors BLOCK_BUTTON_USAGE
#define CUTSCENE_COMPLETION_CLEAR_HELD_ITEM       0x8000      // clearHeldItemsAtEndOfDay() (sleep/dream cutscenes)
#define CUTSCENE_COMPLETION_LOAD_FARM_MAP         0x20000     // loadMapAtSpawnPoint(FARM_SPAWN_POINT_1)
#define CUTSCENE_COMPLETION_CHICKEN_FEED_0        0x40000     // setChickenFeedSprite(0)
#define CUTSCENE_COMPLETION_CHICKEN_FEED_1        0x80000     // setChickenFeedSprite(1)
#define CUTSCENE_COMPLETION_CHICKEN_FEED_2        0x100000    // setChickenFeedSprite(2)
#define CUTSCENE_COMPLETION_LOAD_COOP_MAP         0x200000    // loadMapAtSpawnPoint(COOP_SPAWN_POINT_1)
#define CUTSCENE_COMPLETION_LOAD_BARN_MAP         0x400000    // loadMapAtSpawnPoint(BARN_SPAWN_POINT_1)
#define CUTSCENE_COMPLETION_BARN_FODDER_0         0x800000    // setBarnFodderSprite(0)
#define CUTSCENE_COMPLETION_BARN_FODDER_2         0x1000000   // setBarnFodderSprite(2)
#define CUTSCENE_COMPLETION_MAP_OBJECT_ANIM       0x2000000   // setMapObjectAnimation(0, 0, 0x13)
#define CUTSCENE_COMPLETION_MINING_STRIKE         0x4000000   // mine target tile (gated on player anim 152..175)
#define CUTSCENE_COMPLETION_REFRESH_HELD_ITEM     0x8000000   // initializePlayerHeldItem() if gItemBeingHeld < 252
#define CUTSCENE_COMPLETION_REINIT_ANIMALS        0x10000000  // initializeAnimalEntities()
#define CUTSCENE_COMPLETION_DEACTIVATE_ANIMALS    0x20000000  // deactivateAnimalsAfterCutscene()
#define CUTSCENE_COMPLETION_CONDITIONAL_OUTCOME   0x40000000  // see BIT_CONDITIONAL_OUTCOME comment above
#define CUTSCENE_COMPLETION_OWN_AUDIO_LIGHTING    0x80000000

#endif
