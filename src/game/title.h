#ifndef _TITLE_H_
#define _TITLE_H_

#include "common.h"

// 80126570
typedef struct {
    u8 mode;
    u8 rowHighlighted;
    u8 dogAnimationIndex;
    u8 padding;
    u16 unk_4; // counter for grass animations
    u16 dogIdleCounter;
    f32 centerCoordinate;
    u32 frameCounter;
} TitleContext;

// demo cutscenes index
extern u8 demoCutsceneIndex;

extern void titleScreenMainLoopCallback();
extern void initializeTitleScreen(u8 arg0);

#endif