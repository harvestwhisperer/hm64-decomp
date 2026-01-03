#ifndef _INITIALIZE_2_H_
#define _INITIALIZE_2_H_

#include "common.h"

extern inline void launchIntroCutscene(u16 cutsceneIndex, u16 exitIndex, u8 arg2);
extern void loadLevel(u8);
extern void resumeGameplay(void);
extern void pauseGameplay();    
extern void initializeEntityInstances(u8);
extern void exitOverlayScreen(void);
extern void openOverlayScreen();   

extern void startNewDay(void);
extern void mainGameLoopCallback(void);

#endif