#ifndef _TITLE_H_
#define _TITLE_H_

#include "common.h"

#define TITLE_SCREEN_TEXTURES_VADDR_START 0x80255000
#define TITLE_SCREEN_TEXTURES_VADDR_END 0x8026E100
#define TITLE_SCREEN_PALETTE_VADDR_START 0x8026E500
#define TITLE_SCREEN_PALETTE_VADDR_END 0x8026E900

// 80126570
typedef struct {
    u8 mode;
    u8 rowHighlighted;
    u8 dogAnimationIndex;
    u8 padding;
    u16 unk_4; // counter
    u16 dogIdleCounter;
    f32 centerCoordinate;
    u32 frameCounter;
} TitleContext;

// demo cutscenes index
extern u8 D_801C3F50;

#endif