#ifndef _OVERLAY_SCREENS_H
#define _OVERLAY_SCREENS_H

#include "common.h"

#define TOOL_SLOTS 5
#define BELONGINGS 6
#define KEY_ITEMS 7

typedef struct {
    u8 cellIndex;
    u8 previousCellIndex;
    u8 pageNumber;
    u8 unk_3;
    u8 unk_4;
    u8 unk_5;
    u8 subscreen;
    u8 unk_7;
    u8 unk_8;
    u8 unk_9;
    u8 unk_A;
    u8 unk_B;
} OverlayScreenTable;

// 0x801C3DA0
typedef struct {
    u16 unk_0;
    u16 unk_2;
    u16 unk_4;
    u16 unk_6;
    u16 unk_8;
    u16 unk_A;
    u8 dialogueVariableStrings[6][6]; // dialogue variable 0x2A-0x2F, 0x801C3DAC
    u8 strings2[3][6];
    u8 strings3[3][6];
    u16 unk_54[9];
    u8 dialogueVariableIndices[6]; // 0x801C3E06
    u8 unk_6A; // 0x801C3E0C, index into string10
    u8 unk_6B; // set from random number
    u8 unk_6C; // set from random number
    u8 unk_6D[3];
} OverlayScreenStrings;

extern void func_800B2CE0(void);
extern void func_800B3694(void);
extern void func_800B3A60(void);     
extern void func_800B3BD8(void);
extern void func_800B4160(void);
extern void func_800B4238(u8);
extern void func_800B42E0(void);    
extern void func_800B59AC(void);  
extern void func_800B5BD4(void);
extern void func_800B6674(void);
extern void func_800B7070(u8);
extern void func_800B7B34(void);
extern void func_800B7CB4(void);
extern void func_800B7E28(void);
extern void func_800B7F6C(u8 arg0);
extern void func_800CB0C4(void);
extern void func_800CB8F0(void);
extern void func_800CC17C(void); 
extern bool func_800CE714(bool);
extern bool func_800CE828(bool);  

extern u8 D_801890E8[31];
// calendar
// TODO: label
extern u8 D_80189108[5][7];
// refrigerator/freezer
// TODO: label
// FIXME: should be 2D array with [4][8]
extern u8 D_80237420[31];

extern OverlayScreenTable overlayScreenTable;
extern OverlayScreenStrings overlayScreenStrings;

#endif