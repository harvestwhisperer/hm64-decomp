#include "common.h"

#include "system/globalSprites.h"

// 0x80238800
u8 messageBoxTextureBuffer[0x2900];
// 0x8023B100
u16 messageBoxPaletteBuffer[0x80];
// 0x8023B200
AnimationFrameMetadata messageBoxAnimationFrameMetadataBuffer[0x40];
// 0x8023B300
u32 messageBoxAnimationTextureToPaletteLookupBuffer[0x40];