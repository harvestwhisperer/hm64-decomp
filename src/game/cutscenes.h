#ifndef _SET_CUTSCENES_H_
#define _SET_CUTSCENES_H_

#include "common.h"

#include "game/game.h"
#include "game/cutsceneCompletionFlags.h"
#include "game/raceStateFlags.h"

#define CUTSCENE_ACTIVE 1
#define CUTSCENE_SUPPRESS_NPC_SETUP                  0x02
#define CUTSCENE_SUPPRESS_FARM_ANIMALS               0x04
#define CUTSCENE_REINIT_ANIMALS_ON_COMPLETION        0x08
#define CUTSCENE_DEACTIVATE_ANIMALS_ON_COMPLETION    0x10

typedef union {
    u16 displayOddsBottom[3];
    u8 racerNameLookup[1][6];
} RacerOddsUnion;

// 0x801C3DA0
typedef struct {
    u16 displayOddsTop[3];
    RacerOddsUnion oddsAndNames;
    u8 racerNames[6][6]; // game variable strings 0x2A-0x2F, 0x801C3DAC
    u8 playerBets[3][6];
    u8 racerOdds[3][6];
    u8 racerNameIndices[6][3];
    u8 finishOrder[6]; // 0x801C3E06
    u8 currentRaceIndex;
    u8 playerRaceNumber;
    u8 rivalEntryRaceIndex; // race index 0..2 where the rival featured entry appears
    u8 betPlacedFlags[3];
} RacingContext;

extern u16 setLevelCutscene(u16 mapIndex);
extern u16 handleTimedDailyCutscenes(u8);  
extern u16 handleStartOfDayCutscenes();        
extern u16 setDreamCutscenes();   
extern void handleCutsceneCompletion();   
extern void loadCutscene(bool morningVisit);  

extern u16 gCutsceneIndex;
extern u32 gCutsceneFlags;
extern s32 gCutsceneCompletionFlags;
extern u16 gCutsceneBytecodeSegmentIndex;

// sum of girls' affection
extern u16 D_80215DF0;
// cow festival winner affection (gCowFestivalEnteredCowIndex)
extern u16 D_801886D2;

extern u8 cutsceneLevelInteractionIndex;
extern u8 interactedWithLevelInteractionIndex;

extern u32 raceStateFlags;
extern u8 gRaceFinishOrder[6];

extern RacingContext gRacingContext;

#endif