#ifndef _NAMING_SCREEN_H_
#define _NAMING_SCREEN_H_

#include "common.h"

#define FARM_ANIMAL_NAME_SCREEN 5
#define CHICKEN_NAME_SCREEN 7

#define NAME_LENGTH 6

// 0x8016FBD0
typedef struct {
    u8* name;
    u16 unk_4;
    u8 unk_6; // mode/name to enter
    u8 unk_7;
    Vec4f_Coordinates coordinates;
    s8 unk_18;
    s8 unk_19;
    s8 unk_1A;
    u16 unk_1C;
    u16 flags;
} NamingScreenContext;

extern u8 gNamingScreenIndex;
extern NamingScreenContext namingScreenContext;

inline void initializeNamingScreen(u8*, u8);

#endif