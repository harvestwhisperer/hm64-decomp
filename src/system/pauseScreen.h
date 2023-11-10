#ifndef _PAUSE_SCREEN_H_
#define _PAUSE_SCREEN_H_

#include "common.h"

#define MAX_PAUSE_SCREEN_SPRITES 16

// ci4 asset layout
typedef struct {
    void *romSpritesheetStart;
    void *romSpritesheetEnd;
    void *romAssetIndexStart;
    void *romAssetIndexEnd;
    void *vaddrSpritesheet;
    void *vaddrPalette;
    void *vaddrUnknownAssetSheet;
    void *vaddrUnknownAsset2;
    u32 unk_20;
    u16 specialItemPages;
    u8 unk_26;
    u8 unk_27;
    Vec3f unk_28;
    u32 value;
    u16 unk_38;
    u8 count;
    u8 unk_3B;
    u16 flags;
} PauseScreenSprite;

extern void func_80045DE0();  
extern bool func_80045E20(u16 index, u16 arg1, void *arg2, void *arg3, void *arg4, void *arg5, void *arg6, void *arg7, void *arg8, void *arg9, void *argA, u16 argB, u8 argC, f32 argD, f32 argE, f32 argF, u8 arg10);
extern bool func_80045F5C(u16 index, u32 arg1, u8 arg2, u16 flag);
extern bool func_80046120(u16 index);
extern bool func_800461D8(u16 arg0, u8 arg1, u8 arg2, u8 arg3, u8 arg4);
extern void func_800467F8();

#endif