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
    u8 string7[6];
    u8 string8[6];
    u8 string9[6];
    u8 string10[6];
    u8 string11[6];
    u8 string12[6];
    u8 string13[6];
    u8 string14[6];
    u8 string15[6];
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

extern u32 _checkerboardBackgroundTextureSegmentRomStart;
extern u32 _checkerboardBackgroundTextureSegmentRomEnd;
extern u32 _checkerboardBackgroundIndexSegmentRomStart;
extern u32 _checkerboardBackgroundIndexSegmentRomEnd;

extern u32 _holdableItemsTextureSegmentRomStart;
extern u32 _holdableItemsTextureSegmentRomEnd;
extern u32 _holdableItemsAssetsIndexSegmentRomStart;
extern u32 _holdableItemsAssetsIndexSegmentRomEnd;
extern u32 _holdableItemsSpritesheetIndexSegmentRomStart;
extern u32 _holdableItemsSpritesheetIndexSegmentRomEnd;

extern OverlayScreenTable overlayScreenTable;
extern OverlayScreenStrings overlayScreenStrings;

#endif