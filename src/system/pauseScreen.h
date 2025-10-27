#ifndef _PAUSE_SCREEN_H_
#define _PAUSE_SCREEN_H_

#include "common.h"

#define MAX_PAUSE_SCREEN_SPRITES 16

// ci4 asset layout
typedef struct {
    u32 romSpritesheetStart;
    u32 romSpritesheetEnd;
    u32 romAssetsIndexStart;
    u32 romAssetsIndexEnd;
    void* vaddrSpritesheet;
    void* vaddrPalette;
    void* vaddrUnknownAssetSheet;
    void* vaddrUnknownAsset2;
    u32 unk_20;
    u16 specialItemPages;
    u8 unk_26;
    u8 unk_27;
    Vec3f coordinates;
    u32 value;
    u16 spriteIndex;
    u8 count;
    u8 unk_3B;
    u16 flags;
} PauseScreenSprite;

extern bool dmaPauseScreenSprites(u16 index, u32 arg1, u8 arg2, u16 flag);
extern void func_80045DE0();  
extern bool func_80045E20(u16 index, u16 spriteIndex, u32 romSpritesheetStart, u32 romSpritesheetEnd, u32 romAssetsIndexStart, u32 romAssetsIndexEnd, void *vaddrSpritesheet, void *vaddrPalette, void *vaddrUnknownAssetSheet, void *vaddrUnknownAsset2, u32 argA, u16 argB, u8 argC, f32 x, f32 y, f32 z, u8 arg10);
extern bool func_80045F5C(u16 index, u32 arg1, u8 arg2, u16 flag);
extern bool func_8004635C(u16 spriteIndex, u8 arg1, u8 arg2, u8 arg3, u8 arg4, u16 arg5);
extern bool deactivatePauseScreenSprites(u16 index);
extern bool setPauseScreenSpritesRGBA(u16 arg0, u8 arg1, u8 arg2, u8 arg3, u8 arg4);
extern void updatePauseScreenSprites();

#endif