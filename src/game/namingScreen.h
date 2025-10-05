#ifndef _NAMING_SCREEN_H_
#define _NAMING_SCREEN_H_

#include "common.h"

#define FARM_ANIMAL_NAME_SCREEN 5
#define CHICKEN_NAME_SCREEN 7

#define NAME_LENGTH 6

// 0x8016FBD0
typedef struct {
    u8* name;
    u16 dialogueIndex;
    u8 screenType;
    u8 selectedSeason;
    Vec2f cursor;
    Vec2f shadow;
    s8 gridX;
    s8 gridY; 
    s8 unk_1A;
    u16 unk_1C;
    u16 flags;
} NamingScreenContext;

extern u8 gNamingScreenIndex;
extern NamingScreenContext namingScreenContext;

inline void initializeNamingScreen(u8*, u8);

#endif