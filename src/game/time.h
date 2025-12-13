#ifndef _UPDATE_GAME_H_  
#define _UPDATE_GAME_H_

#include "common.h"

extern void setClockNewDay(void);
extern void func_800D7C20(void);

extern void setSeasonName();  
void setGlobalSeasonName(u8);  

extern u8 gCurrentSeasonName[6];
extern u8 gGlobalSeasonName[6];

#endif