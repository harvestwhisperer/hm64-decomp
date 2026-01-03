#ifndef _SET_CUTSCENES_H_
#define _SET_CUTSCENES_H_

#include "common.h"

#include "game/game.h"

#define MARRIED_TO_POPURI checkLifeEventBit(MARRIED) && gWife == POPURI
#define MARRIED_TO_ELLI checkLifeEventBit(MARRIED) && gWife == ELLI
#define MARRIED_TO_ANN checkLifeEventBit(MARRIED) && gWife == ANN
#define MARRIED_TO_KAREN checkLifeEventBit(MARRIED) && gWife == KAREN
// TODO: clean up usage/make consistent
#define ARE_MARRIED checkLifeEventBit(MARRIED)
#define NOT_MARRIED !checkLifeEventBit(MARRIED)

#define CUTSCENE_ACTIVE 1

extern u16 setLevelCutscene(u16 mapIndex);
extern u16 handleTimedDailyCutscenes(u8);  
extern u16 handleEndOfDayCutscenes();        
extern u16 setDreamCutscenes();   
extern void handleCutsceneCompletion();   
extern void loadCutscene(u8);  

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

extern u32 D_801C3F78;
extern u8 gRaceFinishOrder[6];

#endif