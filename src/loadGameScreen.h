#ifndef _LOAD_GAME_SCREEN_H_
#define _LOAD_GAME_SCREEN_H_

#include "common.h"

// D_801D6238
typedef struct {
    u8 unk_0[15];
    u8 numGamePaks; // 0xF, D_801D624C
    u32 unk_10;
    u8 unk_14;
    u8 unk_15[20]; //  D_801D624D
    u8 unk_29; // D_801D6261
    u8 unk_2A[6]; // D_801D6262
    u8 unk_2B;
    u32 unk_2C;
    u32 unk_34;
    u16 unk_3A;
    u8 unk_3C;
    u8 gamePakEnabled; // 0x3D, D_801D6275
    u8 playerNames[4][6]; // D_801D6278
    u8 dateNumbers[4][2]; // D_801D6290
    u8 seasonNames[4][6]; // D_801D6298
    u8 dateEndings[4][2];  // "st" or "rd" strings /* 0x78 */ D_801D62B0
    u8 fontLoadingFlags[4]; // D_801D62B8
    u8 diaryHighlighted; /* 0x84 */  // D_801D62BC
    u8 actionColumnHighlighted; /* 0x85 */
    u8 unk_86;
    u8 showControllerPakScreen; /* 0x87 */
    u8 unk_88;
    u8 action; // move/delete /* 0x89 */
} LoadGameScreenInfo;

void func_800E1380(u8);            
extern void func_800E53E8(u8);    

extern u8 gCurrentGameIndex;
extern u32 D_801654F4;

#endif