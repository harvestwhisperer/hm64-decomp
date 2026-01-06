#ifndef _NUMBER_DISPLAY_H_
#define _NUMBER_DISPLAY_H_

#include "common.h"

#include "system/globalSprites.h"

#define MAX_NUMBER_SPRITES 16

#define NUMBER_SPRITES_FLAG_ACTIVE         1
#define NUMBER_SPRITES_FLAG_RENDER_READY   2
#define NUMBER_SPRITES_FLAG_BLEND_DECAL    4

// ci4 asset layout
typedef struct {
    u32 romSpritesheetStart;
    u32 romSpritesheetEnd;
    u32 romAssetsIndexStart;
    u32 romAssetsIndexEnd;
    u8* vaddrTexture;
    u16* vaddrPalette;
    AnimationFrameMetadata* vaddrAnimationFrameMetadata;
    u32* spriteToPaletteVaddr;
    u32 *spritesheetIndexVaddr;
    u16 animationIndex;
    u8 animationBaseFrame;
    u8 digitSpacing;
    Vec3f coordinates;
    u32 value;
    u16 spriteIndex;
    u8 maxDigitIndex;
    u16 flags;
} NumberSprites;

extern bool dmaNumberSprites(u16 index, u32 arg1, u8 arg2, u16 flag);
extern void initializeNumberSprites();  
extern bool setNumberSprites(u16 index, u16 spriteIndex, u32 romSpritesheetStart, u32 romSpritesheetEnd, u32 romAssetsIndexStart, u32 romAssetsIndexEnd, u8 *vaddrTexture, u16 *vaddrPalette, AnimationFrameMetadata *vaddrAnimationFrameMetadata, u32 *spriteToPaletteVaddr, u32 *spritesheetIndexVaddr, u16 argB, u8 argC, f32 x, f32 y, f32 z, u8 arg10);
extern bool deactivateNumberSprites(u16 index);
extern bool setNumberSpritesRGBA(u16 arg0, u8 arg1, u8 arg2, u8 arg3, u8 arg4);
extern bool updateOverlayScreenSpriteRGBA(u16 spriteIndex, u8 arg1, u8 arg2, u8 arg3, u8 arg4, u16 rate);
extern void updateNumberSprites();

#endif