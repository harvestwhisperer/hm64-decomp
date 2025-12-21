#ifndef _SET_CUTSCENES_H_
#define _SET_CUTSCENES_H_

#include "common.h"

#include "game/game.h"

#define MARRIED_TO_MARIA checkLifeEventBit(MARRIED) && gWife == MARIA
#define MARRIED_TO_POPURI checkLifeEventBit(MARRIED) && gWife == POPURI
#define MARRIED_TO_ELLI checkLifeEventBit(MARRIED) && gWife == ELLI
#define MARRIED_TO_ANN checkLifeEventBit(MARRIED) && gWife == ANN
#define MARRIED_TO_KAREN checkLifeEventBit(MARRIED) && gWife == KAREN
// TODO: clean up usage/make consistent
#define ARE_MARRIED checkLifeEventBit(MARRIED)
#define NOT_MARRIED !checkLifeEventBit(MARRIED)

u16 func_8009C054(u16 mapIndex);
extern u16 func_800A7AE8(u8);  
extern u16 func_800A7DFC();        
extern u16 func_800A87C4();   
extern void func_800A8F74();   
extern void loadCutscene(u8);  

// maybe game.c bss
extern u16 gCutsceneIndex;

// can't get a consistent match across files for this variable; some functions need array/struct loading, which breaks the match in other functions
extern u32 gCutsceneFlags;
extern u32 cutsceneFlagsHack[2];


// flags
extern s32 gCutsceneCompletionFlags;
extern u16 gCutsceneBytecodeSegmentIndex;

#endif