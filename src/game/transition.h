#ifndef _INITIALIZE_2_H_
#define _INITIALIZE_2_H_

#include "common.h"

extern inline void func_80055F08(u16 cutsceneIndex, u16 exitIndex, u8 arg2);
extern void func_80056030(u8);
extern void func_80059300(void);
extern void func_80059334();    
extern void func_800563D0(u8);
extern void func_80059368(void);
extern void func_800593EC();   

extern void startNewDay(void);
extern void mainGameLoopCallback(void);

#endif