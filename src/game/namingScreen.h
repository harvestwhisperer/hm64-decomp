#ifndef _NAMING_SCREEN_H_
#define _NAMING_SCREEN_H_

#include "common.h"

#define FARM_ANIMAL_NAME_SCREEN 5
#define CHICKEN_NAME_SCREEN 7

#define NAME_LENGTH 6

#define NAMING_SCREEN_CHARSET_MASK            0x3
#define NAMING_SCREEN_CHARSET_HIRAGANA        0
#define NAMING_SCREEN_CHARSET_KATAKANA        1
#define NAMING_SCREEN_CHARSET_ENGLISH         2

#define NAMING_SCREEN_NAME_POSITION_MASK      0x1C
#define NAMING_SCREEN_NAME_POSITION_SHIFT     2

#define NAMING_SCREEN_SCREEN_STATE_MASK       0x60
#define NAMING_SCREEN_SCREEN_STATE_SHIFT      5
#define NAMING_SCREEN_STATE_NAMING_GRID       0
#define NAMING_SCREEN_STATE_SEASON_SELECT     1
#define NAMING_SCREEN_STATE_SEASON_CONFIRM    2
#define NAMING_SCREEN_STATE_ANIMAL_CONFIRM    3

#define NAMING_SCREEN_EMPTY_NAME_ERROR        0x80
#define NAMING_SCREEN_GOTO_SEASON_SELECT      0x100
#define NAMING_SCREEN_GOTO_SEASON_CONFIRM     0x200
#define NAMING_SCREEN_RETURN_TO_NAMING        0x400
#define NAMING_SCREEN_CONFIRM_AND_EXIT        0x800
#define NAMING_SCREEN_AUDIO_FADE_TRANSITION   0x1000

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
    s8 savedGridX;
    u16 unk_1C;
    u16 flags;
} NamingScreenContext;

extern u8 gNamingScreenIndex;
extern NamingScreenContext namingScreenContext;

inline void initializeNamingScreen(u8*, u8);

extern void loadNamingScreenCallback(void);
extern void namingScreenCallback(void);

#endif