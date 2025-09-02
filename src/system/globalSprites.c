#include "common.h"

#include "system/globalSprites.h"

#include "system/memory.h"
#include "system/sprite.h"
#include "system/mathUtils.h"

#include "mainproc.h"

// forward declarations
bool setupSpriteAnimation(u16 spriteIndex, u8 animationModeOrFrameIndex, u16* animationDataPtr);
void setTotalAnimationFrames(AnimationFrameMetadata*, u16*);             
u8* getAnimationFrameMetadataPtr(u16 arg0, void* arg1);
u16* getAnimationFrameMetadataPtrFromFrame(u16, u16*);   

// bss
extern SpriteObject globalSprites[MAX_ACTIVE_SPRITES];

typedef union {
    u8 byte[2];
    u16 halfword;
} Swap16;

typedef union {
    u16 halfword[2];
    u32 word;
} Swap32;

static inline u16 swap16(Swap16 halfword) {
    
    Swap16 swap;
    
    swap.byte[1] = halfword.byte[0];
    swap.byte[0] = halfword.byte[1];
    
    return swap.halfword;

}

// alternate
// static inline u16 swap16(u16 halfword) {
    
//     Swap16 swap;
    
//     swap.byte[1] = halfword >> 8;
//     swap.byte[0] = halfword;
    
//     return swap.halfword;

// }

// alternate
// static inline u16 swap16A(u16 halfword) {

//     u32 padding[4];
    
//     Swap16 swap;
    
//     u32 upper;
//     u32 lower;
    
//     lower = halfword;
    
//     upper = (halfword & 0xFF) << 8;
//     lower = halfword >> 8;
    
//     swap.halfword = lower | upper;
    
//     return swap.halfword;
    
// }

// unsure... not technically a 32 bit swap
// also used in graphic.c
static inline u32 swap16to32(u16 halfword) {
    
    Swap32 swap;
    u16 lower;
    u16 upper;

    upper = (halfword & 0xFF) << 8;
    lower = halfword >> 8;
    
    swap.word = lower | upper;

    return swap.halfword[0] | swap.halfword[1];
    
}

static inline void swapBytes(AnimationFrameMetadata *animationFrameMetadata, Swap16 halfword) {

    animationFrameMetadata->frameDuration = halfword.byte[0];
    animationFrameMetadata->audioTrigger = halfword.byte[1];
    
}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", initializeGlobalSprites);

void initializeGlobalSprites(void) {

    u16 i;

    for (i = 0; i < MAX_SPRITES; i++) {
        
        globalSprites[i].stateFlags = 0;
        globalSprites[i].renderingFlags = 0;
        
        globalSprites[i].paletteIndex = 0;
        globalSprites[i].currentAnimationFrame = 0;
        globalSprites[i].audioTrigger = FALSE;
        globalSprites[i].frameTickCounter = 0;
        
        globalSprites[i].shrink.x = 0;
        globalSprites[i].shrink.y = 0;
        globalSprites[i].shrink.z = 0;
        
        globalSprites[i].scale.x = 1.0f;
        globalSprites[i].scale.y = 1.0f;
        globalSprites[i].scale.z = 1.0f;

        globalSprites[i].rotation.x = 0;
        globalSprites[i].rotation.y = 0;
        globalSprites[i].rotation.z = 0;
        
        globalSprites[i].rgbaCurrent.r = 0;
        globalSprites[i].rgbaCurrent.g = 0;
        globalSprites[i].rgbaCurrent.b = 0;
        globalSprites[i].rgbaCurrent.a = 0;
        
        globalSprites[i].rgbaTarget.r = 0;
        globalSprites[i].rgbaTarget.g = 0;
        globalSprites[i].rgbaTarget.b = 0;
        globalSprites[i].rgbaTarget.a = 0;

    }
}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", dmaSprite);

bool dmaSprite(u16 index, u32 romTextureStart, u32 romTextureEnd, u32 romAssetsIndexStart, u32 romAssetsIndexEnd, 
    u32 romSpritesheetIndexStart, u32 romSpritesheetIndexEnd, u8* texture1Vaddr, u8* texture2Vaddr, u16* paletteVaddr, u16* animationVaddr, 
    u8* spriteToPaletteVaddr, u32* spritesheetIndexVaddr, u8 assetType, u8 flag) {

    u32 assetIndex[8];
    bool result = FALSE;
    
    u32 offset1;
    u32 offset2;
    u32 offset3;
    u32 offset4;
    u32 offset5;

    if (index < MAX_ACTIVE_SPRITES) {
        
        if (!(globalSprites[index].stateFlags & ACTIVE)) { 
            
            nuPiReadRom(romAssetsIndexStart, assetIndex, romAssetsIndexEnd - romAssetsIndexStart);

            // spritesheet
            offset1 = assetIndex[0];
            // palette
            offset2 = assetIndex[1];
            // animation
            offset3 = assetIndex[2];
            // sprite to palette
            offset4 = assetIndex[3];
            // end
            offset5 = assetIndex[4];
        
            // has separate spritesheet index
            if (assetType & 1) {

                nuPiReadRom(romTextureStart + offset2, paletteVaddr, offset3 - offset2);
                nuPiReadRom(romTextureStart + offset3, animationVaddr, offset4 - offset3);
                nuPiReadRom(romTextureStart + offset4, spriteToPaletteVaddr, offset5 - offset4);
                nuPiReadRom(romSpritesheetIndexStart, spritesheetIndexVaddr, romSpritesheetIndexEnd - romSpritesheetIndexStart);

                func_8002B50C(index, animationVaddr, spritesheetIndexVaddr, paletteVaddr, spriteToPaletteVaddr, romTextureStart, texture1Vaddr, texture2Vaddr);

            // spritesheet index in spritesheet
            } else {

                nuPiReadRom(romTextureStart + offset1, texture1Vaddr, offset2 - offset1);
                nuPiReadRom(romTextureStart + offset2, paletteVaddr, offset3 - offset2);
                nuPiReadRom(romTextureStart + offset3, animationVaddr, offset4 - offset3);
                nuPiReadRom(romTextureStart + offset4, spriteToPaletteVaddr, offset5 - offset4);
                
                func_8002B36C(index, animationVaddr, texture1Vaddr, paletteVaddr, spriteToPaletteVaddr);

            }

            if (!flag) {
                globalSprites[index].stateFlags |= SPRITE_FLIP_WINDING;
            }

            globalSprites[index].stateFlags |= ANIMATION_STATE_CHANGED;

            result = TRUE;

        }
    
    }

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", func_8002B36C);

bool func_8002B36C(u16 index, u32* animationIndexPtr, u32* spritesheetIndexPtr, u32* paletteIndexPtr, u8* spriteToPaletteMappingPtr) {

    bool result = FALSE;

    if (index < MAX_ACTIVE_SPRITES) {

        if (!(globalSprites[index].stateFlags & ACTIVE)) {

            globalSprites[index].animationIndexPtr = animationIndexPtr;
            globalSprites[index].spritesheetIndexPtr = spritesheetIndexPtr;
            globalSprites[index].paletteIndexPtr = paletteIndexPtr;
            globalSprites[index].spriteToPaletteMappingPtr = spriteToPaletteMappingPtr;
            globalSprites[index].texturePtr[0] = (void*)0;
            globalSprites[index].texturePtr[1] = (void*)0;
            globalSprites[index].romTexturePtr = (void*)0;

            globalSprites[index].stateFlags = ACTIVE;
            globalSprites[index].renderingFlags = 0;

            globalSprites[index].paletteIndex = 0;
            globalSprites[index].audioTrigger = FALSE;

            setSpriteShrinkFactor(index, 0.0f, 0.0f, 0.0f);
            setSpriteScale(index, 1.0f, 1.0f, 1.0f);
            setSpriteRotation(index, 0.0f, 0.0f, 0.0f);
            setSpriteDefaultRGBA(index, 0xFF, 0xFF, 0xFF, 0xFF);
            func_8002C680(index, 2, 2);
            func_8002C6F8(index, 2);
            setSpriteRenderingLayer(index, (1 | 2));
            
            result = TRUE;
            
        }
    }
 
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", func_8002B50C);

bool func_8002B50C(u16 spriteIndex, u32* animationIndexPtr, u32* spritesheetIndexPtr, u32* paletteIndexPtr, u8* spriteToPaletteMappingPtr, u32 romTexturePtr, u8* texturePtr, u8* texture2Ptr) {
    
    bool result = FALSE;

    if (spriteIndex < MAX_ACTIVE_SPRITES) {

        if (!(globalSprites[spriteIndex].stateFlags & ACTIVE)) {

            globalSprites[spriteIndex].animationIndexPtr = animationIndexPtr;
            globalSprites[spriteIndex].spritesheetIndexPtr = spritesheetIndexPtr;
            globalSprites[spriteIndex].paletteIndexPtr = paletteIndexPtr;
            globalSprites[spriteIndex].spriteToPaletteMappingPtr = spriteToPaletteMappingPtr;
            globalSprites[spriteIndex].texturePtr[0] = texturePtr;
            globalSprites[spriteIndex].texturePtr[1] = texture2Ptr;
            globalSprites[spriteIndex].romTexturePtr = romTexturePtr;

            globalSprites[spriteIndex].stateFlags = (ACTIVE | 4);
            globalSprites[spriteIndex].renderingFlags = 0;

            globalSprites[spriteIndex].paletteIndex = 0;
            globalSprites[spriteIndex].audioTrigger = FALSE;

            setSpriteShrinkFactor(spriteIndex, 0, 0, 0);
            setSpriteScale(spriteIndex, 1.0f, 1.0f, 1.0f);
            setSpriteRotation(spriteIndex, 0, 0, 0);
            setSpriteDefaultRGBA(spriteIndex, 0xFF, 0xFF, 0xFF, 0xFF);
            func_8002C680(spriteIndex, 2, 2);
            func_8002C6F8(spriteIndex, 2);
            setSpriteRenderingLayer(spriteIndex, (1 | 2));
            
            result = TRUE;
            
        }
    }
 
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", deactivateSprite);

inline bool deactivateSprite(u16 spriteIndex) {

    bool result = FALSE;

    if (spriteIndex < MAX_ACTIVE_SPRITES) {
        if (globalSprites[spriteIndex].stateFlags & ACTIVE) {
            globalSprites[spriteIndex].stateFlags &= ~(ACTIVE | ANIMATION_HEADER_PROCESSED);
            result = TRUE;
        }        
    }

    return result; 

}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", deactivateGlobalSprites);

void deactivateGlobalSprites(void) {

    u16 i;

    for (i = 0; i < MAX_ACTIVE_SPRITES; i++) {

            if ((globalSprites[i].stateFlags & ACTIVE) && (globalSprites[i].stateFlags & ANIMATION_HEADER_PROCESSED)) {

                // FIXME: probably inline
                if (i < MAX_ACTIVE_SPRITES) {
                    
                    globalSprites[i].stateFlags &= ~(ACTIVE | 2);

                }

            }

            globalSprites[i].stateFlags = 0;
        
    }
}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", func_8002B7A0);

// unused or inline
bool func_8002B7A0(u16 spriteIndex, u8 animationModeOrFrameIndex) {

    bool result;

    result = setupSpriteAnimation(spriteIndex, animationModeOrFrameIndex, globalSprites[spriteIndex].animationIndexPtr);

    globalSprites[spriteIndex].animationCounter[0] = 0xFF;
    globalSprites[spriteIndex].animationCounter[1] = 0xFF;

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", startSpriteAnimation);

bool startSpriteAnimation(u16 spriteIndex, u16 animationIndex, u8 animationModeOrFrameIndex) {

    bool result = FALSE;
    
    if (spriteIndex < MAX_ACTIVE_SPRITES) {

        if (globalSprites[spriteIndex].stateFlags & ACTIVE) {
            
            if (globalSprites[spriteIndex].animationIndexPtr[animationIndex] != globalSprites[spriteIndex].animationIndexPtr[animationIndex+1]) {

                result = setupSpriteAnimation(spriteIndex, animationModeOrFrameIndex, getAnimationFrameMetadataPtr(animationIndex, globalSprites[spriteIndex].animationIndexPtr));
                
                globalSprites[spriteIndex].animationCounter[0] = 0xFF;
                globalSprites[spriteIndex].animationCounter[1] = 0xFF;
           
            }
        }
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", setupSpriteAnimation);

bool setupSpriteAnimation(u16 spriteIndex, u8 animationModeOrFrameIndex, u16* animationDataPtr) {

    bool result = FALSE;
    u16 *metadataPtr;
    
    if ((spriteIndex < MAX_ACTIVE_SPRITES) && (globalSprites[spriteIndex].stateFlags & ACTIVE) && !(globalSprites[spriteIndex].stateFlags & ANIMATION_HEADER_PROCESSED)) {

        globalSprites[spriteIndex].animationDataPtr = animationDataPtr;

        globalSprites[spriteIndex].frameTickCounter = 0;
        globalSprites[spriteIndex].audioTrigger = FALSE;

        globalSprites[spriteIndex].stateFlags &= ~ANIMATION_STATE_CHANGED;
        globalSprites[spriteIndex].stateFlags |= ANIMATION_HEADER_PROCESSED;

        switch (animationModeOrFrameIndex) {
            case 0xFF:
                globalSprites[spriteIndex].currentAnimationFrame = 0;
                globalSprites[spriteIndex].stateFlags &= ~ANIMATION_LOOPS;
                globalSprites[spriteIndex].stateFlags |= ANIMATION_PLAYING;
                break;
            case 0xFE:
                globalSprites[spriteIndex].currentAnimationFrame = 0;
                globalSprites[spriteIndex].stateFlags |= ANIMATION_PLAYING | ANIMATION_LOOPS;
                break;
            case 0xFD:
                globalSprites[spriteIndex].currentAnimationFrame = 0;
                globalSprites[spriteIndex].stateFlags |= ANIMATION_PLAYING | DESTROY_ON_ANIMATION_END;
                break;
            default:
                // for static sprites, show a single frame
                globalSprites[spriteIndex].currentAnimationFrame = animationModeOrFrameIndex;
                globalSprites[spriteIndex].stateFlags &= ~(ANIMATION_PLAYING | ANIMATION_LOOPS);
                break;
        }

        // skips header and gets little endian u16 value
        setTotalAnimationFrames(&globalSprites[spriteIndex].animationFrameMetadata, globalSprites[spriteIndex].animationDataPtr);

        // animationDataPtr + 4 = skip header and frame count
        metadataPtr = getAnimationFrameMetadataPtrFromFrame(globalSprites[spriteIndex].currentAnimationFrame, globalSprites[spriteIndex].animationDataPtr + 4);
        
        globalSprites[spriteIndex].animationFrameMetadataPtr = metadataPtr;
        globalSprites[spriteIndex].bitmapMetadataPtr = metadataPtr + 2;

        result = TRUE;
        
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", resetAnimationState);

bool resetAnimationState(u16 index) {

    bool result = FALSE;

    if (index < MAX_ACTIVE_SPRITES) {

        if (globalSprites[index].stateFlags & ACTIVE) {
            globalSprites[index].stateFlags &= ~ANIMATION_HEADER_PROCESSED;
            result = TRUE;
        }
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", func_8002BB30);

bool func_8002BB30(u16 index) {

    bool result = FALSE;

    if (index < MAX_ACTIVE_SPRITES) {

        if (globalSprites[index].stateFlags & ACTIVE) {
            globalSprites[index].stateFlags &= ~ANIMATION_PAUSED;
            result = TRUE;
        }
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", func_8002BB88);

bool func_8002BB88(u16 index) {

    bool result = FALSE;

    if (index < MAX_ACTIVE_SPRITES) {

        if (globalSprites[index].stateFlags & ACTIVE) {
            globalSprites[index].stateFlags |= ANIMATION_PAUSED;
            result = TRUE;
        }
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", setSpriteFlip);

bool setSpriteFlip(u16 index, bool flipHorizontal, bool flipVertical) {

    bool result = FALSE;
    
    if (index < MAX_ACTIVE_SPRITES) {

        if (globalSprites[index].stateFlags & ACTIVE) {

            if (flipHorizontal) {
                globalSprites[index].renderingFlags |= FLIP_HORIZONTAL;
            } else {
                globalSprites[index].renderingFlags &= ~FLIP_HORIZONTAL;
            }

            if (flipVertical) {
                globalSprites[index].renderingFlags |= FLIP_VERTICAL;
            } else {
                globalSprites[index].renderingFlags &= ~FLIP_VERTICAL;
            }
            
            result = TRUE;

        }
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", func_8002BCC8);

bool func_8002BCC8(u16 index) {

    bool result = FALSE;

    if (index < MAX_ACTIVE_SPRITES) {

        if (globalSprites[index].stateFlags & ACTIVE) {
            result = globalSprites[index].stateFlags & ANIMATION_STATE_CHANGED;
        }
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", setSpriteShrinkFactor);

bool setSpriteShrinkFactor(u16 index, f32 x, f32 y, f32 z) {

    bool result = FALSE;

    if (index < MAX_ACTIVE_SPRITES) {

        if (globalSprites[index].stateFlags & ACTIVE) {

            globalSprites[index].shrink.x = x;
            globalSprites[index].shrink.y = y;
            globalSprites[index].shrink.z = z;

            result = TRUE;

        }
    }

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", setSpriteScale);

bool setSpriteScale(u16 index, f32 x, f32 y, f32 z) {

    bool result = FALSE;

    if (index < MAX_ACTIVE_SPRITES) {

        if (globalSprites[index].stateFlags & ACTIVE) {

            globalSprites[index].scale.x = x;
            globalSprites[index].scale.y = y;
            globalSprites[index].scale.z = z;
            
            result = TRUE;

        }
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", setSpriteRotation);

bool setSpriteRotation(u16 index, f32 x, f32 y, f32 z) {

    bool result = FALSE;

    if (index < MAX_ACTIVE_SPRITES) {

        if (globalSprites[index].stateFlags & ACTIVE) {

            globalSprites[index].rotation.x = x;
            globalSprites[index].rotation.y = y;
            globalSprites[index].rotation.z = z;

            result = TRUE;

        }
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", adjustSpriteShrinkFactor);

bool adjustSpriteShrinkFactor(u16 index, f32 x, f32 y, f32 z) {

    bool result = FALSE;
    
    if (index < MAX_ACTIVE_SPRITES) {
        
        if (globalSprites[index].stateFlags & ACTIVE) {

            globalSprites[index].shrink.x += x;
            globalSprites[index].shrink.y += y;
            globalSprites[index].shrink.z += z;
                
            result = TRUE;
            
        }   
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", adjustSpriteScale);

// unused
bool adjustSpriteScale(u16 index, f32 x, f32 y, f32 z) {

    bool result = FALSE;
    
    if (index < MAX_ACTIVE_SPRITES) {

        if (globalSprites[index].stateFlags & ACTIVE) {

            globalSprites[index].scale.x += x;
            globalSprites[index].scale.y += y;
            globalSprites[index].scale.z += z;
            
            result = TRUE;

        }
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", func_8002C000);

// unused or inline
bool adjustSpriteRotation(u16 index, f32 x, f32 y, f32 z) {

    bool result = FALSE;
    
    if (index < MAX_ACTIVE_SPRITES) {
        
        if (globalSprites[index].stateFlags & ACTIVE) {

            globalSprites[index].rotation.x += x;
            globalSprites[index].rotation.y += y;
            globalSprites[index].rotation.z += z;
                
            result = TRUE;
            
        }   
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", func_8002C0B4);

// unused or inline
bool adjustSpriteRGBA(u16 index, s8 r, s8 g, s8 b, s8 a) {

    bool result = FALSE;
    
    if (index < MAX_ACTIVE_SPRITES) {

        if (globalSprites[index].stateFlags & ACTIVE) {

            globalSprites[index].rgbaCurrent.r += r;
            globalSprites[index].rgbaCurrent.g += g;
            globalSprites[index].rgbaCurrent.b += b;
            globalSprites[index].rgbaCurrent.a += a;
            
            result = TRUE;
            
        }   
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", updateSpriteRGBA);

// update sprite rgba
// fade out on screen transitions
bool updateSpriteRGBA(u16 index, u8 r, u8 g, u8 b, u8 a, s16 rate) {

    bool result;
    
    s16 absValueRate = getAbsoluteValue(rate);
    f32 tempFloat;
    
    result = 0;
    
    if (index < MAX_ACTIVE_SPRITES) {
        
        if (globalSprites[index].stateFlags & ACTIVE) {
 
            globalSprites[index].rgbaTarget.r = (globalSprites[index].baseRGBA.r * r) / 255.0f;
            globalSprites[index].rgbaTarget.g = (globalSprites[index].baseRGBA.g * g) / 255.0f;
            globalSprites[index].rgbaTarget.b = (globalSprites[index].baseRGBA.b * b) / 255.0f;
            globalSprites[index].rgbaTarget.a = (globalSprites[index].baseRGBA.a * a) / 255.0f;
            
            globalSprites[index].stateFlags &= ~RGBA_IN_PROGRESS;

            if (globalSprites[index].rgbaTarget.r < globalSprites[index].rgbaCurrent.r) {
                tempFloat = globalSprites[index].rgbaCurrent.r - globalSprites[index].rgbaTarget.r;
            } else {
                tempFloat = globalSprites[index].rgbaTarget.r - globalSprites[index].rgbaCurrent.r;
            }

            globalSprites[index].rgbaDelta.r = (tempFloat * absValueRate) / globalSprites[index].baseRGBA.r;

            if (globalSprites[index].rgbaTarget.g < globalSprites[index].rgbaCurrent.g) {
                tempFloat = globalSprites[index].rgbaCurrent.g - globalSprites[index].rgbaTarget.g;
            } else {
                tempFloat = globalSprites[index].rgbaTarget.g - globalSprites[index].rgbaCurrent.g;
            }

            globalSprites[index].rgbaDelta.g = (tempFloat * absValueRate) / globalSprites[index].baseRGBA.g;

            if (globalSprites[index].rgbaTarget.b < globalSprites[index].rgbaCurrent.b) {
                tempFloat = globalSprites[index].rgbaCurrent.b - globalSprites[index].rgbaTarget.b;
            } else {
                tempFloat = globalSprites[index].rgbaTarget.b - globalSprites[index].rgbaCurrent.b;
            }

            globalSprites[index].rgbaDelta.b = (tempFloat * absValueRate) / globalSprites[index].baseRGBA.b;

            if (globalSprites[index].rgbaTarget.a < globalSprites[index].rgbaCurrent.a) {
                tempFloat = globalSprites[index].rgbaCurrent.a - globalSprites[index].rgbaTarget.a;
            } else {
                tempFloat = globalSprites[index].rgbaTarget.a - globalSprites[index].rgbaCurrent.a;
            }

            globalSprites[index].rgbaDelta.a = (tempFloat * absValueRate) / globalSprites[index].baseRGBA.a;
            
            result = TRUE;
            
        }
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", updateSpriteAlpha);

bool updateSpriteAlpha(u16 index, u8 arg1, s16 rate) {

    bool result;

    f32 tempF;
    s16 absValueRate = getAbsoluteValue(rate);

    result = FALSE;

    if (index < MAX_ACTIVE_SPRITES) {

        if (globalSprites[index].stateFlags & ACTIVE) {

            globalSprites[index].rgbaTarget.a = (globalSprites[index].baseRGBA.a * arg1) / 255.0f;

            globalSprites[index].stateFlags &= ~RGBA_IN_PROGRESS;

            if (globalSprites[index].rgbaTarget.a < globalSprites[index].rgbaCurrent.a) {
                tempF = globalSprites[index].rgbaCurrent.a - globalSprites[index].rgbaTarget.a;
            } else {
                tempF = globalSprites[index].rgbaTarget.a - globalSprites[index].rgbaCurrent.a;
            }

            globalSprites[index].rgbaDelta.a = (tempF * absValueRate) / globalSprites[index].baseRGBA.a;
            
            result = TRUE;

        }
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", func_8002C680);

bool func_8002C680(u16 index, u16 arg1, u16 flags) {

    bool result = FALSE;
    int temp;

    if (index < MAX_ACTIVE_SPRITES) {

        if (globalSprites[index].stateFlags & ACTIVE) { 
            
            globalSprites[index].renderingFlags &= ~( 8 | 0x10 | 0x20 | 0x40);
            
            // probably a shift
            globalSprites[index].renderingFlags |= arg1 * 8;
            
            temp = flags << 5;
            globalSprites[index].renderingFlags |= temp;
            
            result = TRUE;
            
        } 
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", func_8002C6F8);

bool func_8002C6F8(u16 index, u16 arg1) {
    
    int temp;

    bool result = FALSE;
    
    if (index < MAX_ACTIVE_SPRITES) {
        
        if (globalSprites[index].stateFlags & ACTIVE) {
            
            globalSprites[index].renderingFlags &= ~(0x80 | 0x100);
            temp = arg1 << 7;
            globalSprites[index].renderingFlags |= temp;
            
            result = TRUE;

        }
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", func_8002C768);

bool func_8002C768(u16 index, u16 arg1) {

    bool result = FALSE;
    
    if (index < MAX_ACTIVE_SPRITES) {
        
        if (globalSprites[index].stateFlags & ACTIVE) {

            if (arg1) {
                globalSprites[index].renderingFlags |= 0x200;
            } else {
                globalSprites[index].renderingFlags &= ~0x200;
            }


            result = TRUE;

        }
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", setSpriteRenderingLayer);

bool setSpriteRenderingLayer(u16 index, u16 flags) {
    
    int temp;

    bool result = FALSE;
    
    if (index < MAX_ACTIVE_SPRITES) {
        
        if (globalSprites[index].stateFlags & ACTIVE) {
            
            globalSprites[index].renderingFlags &= ~(0x400 | 0x800 | 0x1000);
            temp = flags << 10;
            globalSprites[index].renderingFlags |= temp;
            
            result = TRUE;

        }
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", setSpriteDefaultRGBA);

bool setSpriteDefaultRGBA(u16 index, u8 r, u8 g, u8 b, u8 a) {

    bool result = FALSE;

    if (index < MAX_ACTIVE_SPRITES) {
        
        if (globalSprites[index].stateFlags & ACTIVE) {
            
            globalSprites[index].baseRGBA.r = r;
            globalSprites[index].baseRGBA.g = g;
            globalSprites[index].baseRGBA.b = b;
            globalSprites[index].baseRGBA.a = a;
            
            result = TRUE;

        }
    }

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", setSpriteColor);

bool setSpriteColor(u16 index, u8 r, u8 g, u8 b, u8 a) {

    bool result = FALSE;
    
    if (index < MAX_ACTIVE_SPRITES) {
        
        if (globalSprites[index].stateFlags & ACTIVE) {        

            result = TRUE;
            
            globalSprites[index].rgbaCurrent.r = (globalSprites[index].baseRGBA.r * r) / 255.0f;
            globalSprites[index].rgbaCurrent.g = (globalSprites[index].baseRGBA.g * g) / 255.0f;
            globalSprites[index].rgbaCurrent.b = (globalSprites[index].baseRGBA.b * b) / 255.0f;
            globalSprites[index].rgbaCurrent.a = (globalSprites[index].baseRGBA.a * a) / 255.0f;

            globalSprites[index].rgbaTarget.r = (globalSprites[index].baseRGBA.r * r) / 255.0f;
            globalSprites[index].rgbaTarget.g = (globalSprites[index].baseRGBA.g * g) / 255.0f;
            globalSprites[index].rgbaTarget.b = (globalSprites[index].baseRGBA.b * b) / 255.0f;
            globalSprites[index].rgbaTarget.a = (globalSprites[index].baseRGBA.a * a) / 255.0f;

        }
    }
    
    return result;
}


//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", setSpriteAlpha);

bool setSpriteAlpha(u16 index, u8 a) {
    
    bool result = FALSE;

    if (index < MAX_ACTIVE_SPRITES) {

        if (globalSprites[index].stateFlags & ACTIVE) {

            globalSprites[index].baseRGBA.a = a;
            globalSprites[index].rgbaCurrent.a = a;
            globalSprites[index].rgbaTarget.a = a;
            
            result = TRUE;

        }
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", setBilinearFiltering);

bool setBilinearFiltering(u16 index, bool useBilinearFiltering) {

    bool result = FALSE;

    if (index < MAX_ACTIVE_SPRITES) {
        
        if (globalSprites[index].stateFlags & ACTIVE) {
            if (useBilinearFiltering == TRUE) {
                globalSprites[index].stateFlags |= ENABLE_BILINEAR_FILTERING;
            } else {
                globalSprites[index].stateFlags &= ~ENABLE_BILINEAR_FILTERING;
            }

            result = TRUE;

        }
    }
    
    return result;
}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", setSpritePaletteIndex);

bool setSpritePaletteIndex(u16 index, u16 paletteIndex) {

    bool result = FALSE;

    if (index < MAX_ACTIVE_SPRITES) {

        if (globalSprites[index].stateFlags & ACTIVE) {

            globalSprites[index].paletteIndex = paletteIndex;
            globalSprites[index].stateFlags |= DIRECT_PALETTE_MODE;

            result = TRUE;

        }
    }

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", func_8002CBF8);

bool func_8002CBF8(u16 index) {

    bool result;

    result = FALSE;
    
    if (index < MAX_ACTIVE_SPRITES) {
        if (globalSprites[index].stateFlags & ACTIVE) {
            result = (globalSprites[index].stateFlags >> 10) & 1;
        }
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", checkSpriteAnimationStateChanged);

bool checkSpriteAnimationStateChanged(u16 index) {

    bool result = FALSE;

    if (index < MAX_ACTIVE_SPRITES) {
        // flag 0x40, ANIMATION_STATE_CHANGED
        result = (globalSprites[index].stateFlags >> 6) & 1;
    }

    return result;
    
}

void setTotalAnimationFrames(AnimationFrameMetadata *animationFrameMetadata, u16 *animationAsset) {
    
    // skip 4 byte header
    animationFrameMetadata->objectCount = swap16(animationAsset[2]);
    
}

inline void setAnimationFrameMetadata(AnimationFrameMetadata* animationFrameMetadata, u16* animationFrameMetadataPtr) {

    animationFrameMetadata->objectCount = swap16(animationFrameMetadataPtr[0]);
    swapBytes(animationFrameMetadata, animationFrameMetadataPtr[1]);
    
}

inline void setBitmapMetadata(BitmapMetadata* ptr, u16* data) {
    
    ptr->spritesheetIndex = swap16to32(data[0]);    
    ptr->unk_2 = 0;
    ptr->anchorX = swap16to32(data[2]);
    ptr->anchorY = swap16(data[3]);

}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", getAnimationFrameMetadataPtr);

// TODO: see if void* type is necessary
u8* getAnimationFrameMetadataPtr(u16 index, void* table) {
 
    u32 *arr = (u32*)table;
    
    return (u8*)(table + arr[index]);
    
}

// alternate
/*
u8* getAnimationFrameMetadataPtr(u16 arg0, void* arg1) {
    return (u8*)(arg1 + *(u32*)(arg1 + arg0*4));
}
*/

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", getAnimationFrameMetadataPtrFromFrame);

// update pointer by iterateing through animation data (contains header, animation metadata, and sprite metadata)
inline u16* getAnimationFrameMetadataPtrFromFrame(u16 currentFrame, u16* animationFrameMetadataPtr) {
    
    u16 i;
    AnimationFrameMetadata animationFrameMetadata;

    u32 pad[2];

    u32 temp;
    u32 temp2;
    u32 temp3;
    u8 temp4;
    
    i = 0;
    
    if (currentFrame != 0) {
        
        do {

            // FIXME: should be inline setAnimationFrameMetadata call
            
            temp2 = animationFrameMetadataPtr[0] << 8;
            temp3 = animationFrameMetadataPtr[0] >> 8;
            
            temp = temp2 | temp3;
            
            animationFrameMetadata.objectCount = temp;

            temp2 = animationFrameMetadataPtr[1];
            temp = temp2;
            temp2 = temp;
            
            temp2 = temp >> 8;
            temp4 = temp >> 8;
            
            animationFrameMetadata.frameDuration = temp4;
            animationFrameMetadata.audioTrigger = temp;
            
            //

            // skip header for next iteration
            animationFrameMetadataPtr += 2;
            // * sizeof(animationFrameMetadata); frame data consists of animationFrameMetadata + varying lengths of bitmapMetadata objects
            animationFrameMetadataPtr += animationFrameMetadata.objectCount * 4;
            
            i++;
            
        } while (i < currentFrame);

    }
    
    return animationFrameMetadataPtr;

}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", advanceBitmapMetadataPtr);

inline u16* advanceBitmapMetadataPtr(u16 numFrames, u16* bitmapMetadataPtr) {
    
    u32 i;
    
    // FIXME: weird typecast
    for (i = 0; (u16)i < numFrames; i++) {
        bitmapMetadataPtr += 4;
    }

    return bitmapMetadataPtr;
    
}

// alternate
/*
u16* advanceBitmapMetadataPtr(u16 numFrames, u16* bitmapMetadataPtr) {
    
    u16* result;
    u32 padding[2];
    
    u16 i = 0;
    
    if (numFrames) {
        do {
            result += 4;
            i++;
        } while (i < numFrames);
    }
    
    return result;
    
}
*/


/*
typedef struct {
    volatile u16 spritesheetIndex;
    u16 unk_2;
    s16 anchorX;
    s16 anchorY;
    u16 unk_8; 
} BitmapMetadataAlt;

typedef struct {
    u32 unk_0;
    AnimationFrameMetadata *animationFrameMetadataPtr;
    u32 unk_8; 
    u32 length;
} SpriteMetadata;
*/

#ifdef PERMUTER
void setBitmapFromSpriteObject(u16 spriteIndex, AnimationFrameMetadata* animationFrameMetadataPtr, u8 animationType) {

    u16 bitmapIndex;
    
    u16 *texturePtr;
    u16 *palettePtr;
    
    f32 shrinkX;
    f32 shrinkY;
    f32 shrinkZ;
    
    BitmapMetadataAlt bitmapMetadata;
    SpriteMetadata metadata;
    
    u16 objectCount;
    u32 temp2;
    u16 temp3;
    u16 i;
    
    metadata.animationFrameMetadataPtr = animationFrameMetadataPtr;
    objectCount = animationFrameMetadataPtr->objectCount;
    
    metadata.length = 0;
    
    texturePtr = globalSprites[spriteIndex].texturePtr[gGraphicsBufferIndex];

    if (objectCount) {
        
        i = 0;
    
        temp2 = animationType;
    
        do {
            
            if (temp2) {

                do {
                    
                    setBitmapMetadata(&bitmapMetadata, advanceBitmapMetadataPtr((objectCount - i) - 1, globalSprites[spriteIndex].bitmapMetadataPtr));
                    
                    texturePtr += metadata.length;
                
                    if (temp2 == 2) {
                        setSpriteDMAInfo(bitmapMetadata.spritesheetIndex, globalSprites[spriteIndex].spritesheetIndexPtr, texturePtr, globalSprites[spriteIndex].romTexturePtr);
                    }
                
                    metadata.length = getTextureLength(bitmapMetadata.spritesheetIndex, globalSprites[spriteIndex].spritesheetIndexPtr);
            
                } while (0);
                
            } else {

                do {
                    
                    temp3 = ((metadata.animationFrameMetadataPtr->objectCount - i) - 1);
    
                    if (temp2 < temp3) {
                        
                    }
    
                    setBitmapMetadata(&bitmapMetadata, advanceBitmapMetadataPtr(temp3, globalSprites[spriteIndex].bitmapMetadataPtr));
    
                    texturePtr = getTexturePtr(bitmapMetadata.spritesheetIndex, globalSprites[spriteIndex].spritesheetIndexPtr);

                } while (0);
               
            }

            if (globalSprites[spriteIndex].stateFlags & DIRECT_PALETTE_MODE) {
                palettePtr = getPalettePtrType1(globalSprites[spriteIndex].paletteIndex, globalSprites[spriteIndex].paletteIndexPtr);
            } else {
                palettePtr = getPalettePtrType2(bitmapMetadata.spritesheetIndex, globalSprites[spriteIndex].paletteIndexPtr, globalSprites[spriteIndex].spriteToPaletteMappingPtr);
            }

            shrinkX = globalSprites[spriteIndex].shrink.x;
            shrinkY = globalSprites[spriteIndex].shrink.y;
            shrinkZ = globalSprites[spriteIndex].shrink.z;

            if (globalSprites[spriteIndex].stateFlags & SPRITE_FLIP_WINDING) {
                bitmapIndex = setBitmap(texturePtr, palettePtr, (0x8 | 0x10 | 0x20 | 0x40));
            } else {
                bitmapIndex = setBitmap(texturePtr, palettePtr, (0x8 | 0x10 | 0x30));
            }
            
            setBitmapShrink(bitmapIndex, shrinkX, shrinkY, shrinkZ);
            setBitmapScale(bitmapIndex, globalSprites[spriteIndex].scale.x, globalSprites[spriteIndex].scale.y, globalSprites[spriteIndex].scale.z);
            setBitmapRotation(bitmapIndex, globalSprites[spriteIndex].rotation.x, globalSprites[spriteIndex].rotation.y, globalSprites[spriteIndex].rotation.z);
            setBitmapRGBA(bitmapIndex, globalSprites[spriteIndex].rgbaCurrent.r, globalSprites[spriteIndex].rgbaCurrent.g, globalSprites[spriteIndex].rgbaCurrent.b, globalSprites[spriteIndex].rgbaCurrent.a);
            setBitmapAnchor(bitmapIndex, bitmapMetadata.anchorX, bitmapMetadata.anchorY);
            
            setBitmapFlip(bitmapIndex, globalSprites[spriteIndex].renderingFlags & FLIP_HORIZONTAL, globalSprites[spriteIndex].renderingFlags & FLIP_VERTICAL);
            func_80029E2C(bitmapIndex, (globalSprites[spriteIndex].renderingFlags >> 3) & 3, (globalSprites[spriteIndex].renderingFlags >> 5) & 3);
            func_80029EA4(bitmapIndex, (globalSprites[spriteIndex].renderingFlags >> 7) & 3);
            func_80029F14(bitmapIndex, globalSprites[spriteIndex].renderingFlags & 0x200);
            func_8002A02C(bitmapIndex, (globalSprites[spriteIndex].renderingFlags >> 0xA) & 7);

            if (globalSprites[spriteIndex].stateFlags & 0x80) {
                bitmaps[bitmapIndex].flags |= 4;
            } else {
                bitmaps[bitmapIndex].flags &= ~4;
            }

            objectCount = metadata.animationFrameMetadataPtr->objectCount;
            i++;
            
        } while (i < metadata.animationFrameMetadataPtr->objectCount);
        
    }
    
}
#else
INCLUDE_ASM("asm/nonmatchings/system/globalSprites", setBitmapFromSpriteObject);
#endif

static inline u8 updateSpriteCurrentRGBA(u16 i) {

    u8 count = 0;
    f32 tempf;

    if (globalSprites[i].rgbaCurrent.r <  globalSprites[i].rgbaTarget.r) {
        
        globalSprites[i].rgbaCurrent.r += globalSprites[i].rgbaDelta.r;
        
        if (globalSprites[i].rgbaTarget.r <= globalSprites[i].rgbaCurrent.r) {
            globalSprites[i].rgbaCurrent.r = globalSprites[i].rgbaTarget.r;
        } else {
            count = 1;
        }
    }

    if (globalSprites[i].rgbaCurrent.r > globalSprites[i].rgbaTarget.r) {
        
        globalSprites[i].rgbaCurrent.r -= globalSprites[i].rgbaDelta.r;
        
        if (globalSprites[i].rgbaTarget.r >= globalSprites[i].rgbaCurrent.r) {
            globalSprites[i].rgbaCurrent.r = globalSprites[i].rgbaTarget.r;
        } else {
            count++;
        }
    }

    if (globalSprites[i].rgbaCurrent.g <  globalSprites[i].rgbaTarget.g) {
        
        globalSprites[i].rgbaCurrent.g +=  globalSprites[i].rgbaDelta.g;
        
        if ((globalSprites[i].rgbaTarget.g <= globalSprites[i].rgbaCurrent.g)) {
            globalSprites[i].rgbaCurrent.g = globalSprites[i].rgbaTarget.g;
        } else {
            count++;
        }
    }

    if (globalSprites[i].rgbaCurrent.g >  globalSprites[i].rgbaTarget.g) {
        
        globalSprites[i].rgbaCurrent.g -= globalSprites[i].rgbaDelta.g;
        
        if (globalSprites[i].rgbaTarget.g >= globalSprites[i].rgbaCurrent.g) {
            globalSprites[i].rgbaCurrent.g = globalSprites[i].rgbaTarget.g;
        } else {
            count++;
        }
    }

    if (globalSprites[i].rgbaCurrent.b <  globalSprites[i].rgbaTarget.b) {
        
        globalSprites[i].rgbaCurrent.b += globalSprites[i].rgbaDelta.b;
        
        if ((globalSprites[i].rgbaTarget.b <= globalSprites[i].rgbaCurrent.b)) {
            globalSprites[i].rgbaCurrent.b = globalSprites[i].rgbaTarget.b;
        } else {
            count++;
        }
    }

    if (globalSprites[i].rgbaCurrent.b >  globalSprites[i].rgbaTarget.b) {
        
        globalSprites[i].rgbaCurrent.b -= globalSprites[i].rgbaDelta.b;
        
        if (globalSprites[i].rgbaTarget.b >= globalSprites[i].rgbaCurrent.b) {
            globalSprites[i].rgbaCurrent.b = globalSprites[i].rgbaTarget.b;
        } else {
            count++;
        }
    }

    if (globalSprites[i].rgbaCurrent.a <  globalSprites[i].rgbaTarget.a) {
        
        globalSprites[i].rgbaCurrent.a += globalSprites[i].rgbaDelta.a;
        
        if ((globalSprites[i].rgbaTarget.a <= globalSprites[i].rgbaCurrent.a)) {
            globalSprites[i].rgbaCurrent.a = globalSprites[i].rgbaTarget.a;
        } else {
            count++;
        }
    }

    if (globalSprites[i].rgbaCurrent.a >  globalSprites[i].rgbaTarget.a) {
        
        globalSprites[i].rgbaCurrent.a -= globalSprites[i].rgbaDelta.a;
        
        if (globalSprites[i].rgbaTarget.a >= globalSprites[i].rgbaCurrent.a) {
            globalSprites[i].rgbaCurrent.a = globalSprites[i].rgbaTarget.a;
        } else {
            count++;
        }
    }

    return count;
    
}

#ifdef PERMUTER
void updateSprites(void) {

    AnimationFrameMetadata animationFrameMetadata;
    u32 pad[3];
    
    u16 i;
    u16* ptr;

    i = 0;
    
    while (i < MAX_ACTIVE_SPRITES) {

        if (globalSprites[i].stateFlags & ACTIVE) {
        
            if (updateSpriteCurrentRGBA(i) == 0) {
                globalSprites[i].stateFlags |= RGBA_IN_PROGRESS;
            }

            globalSprites[i].audioTrigger = FALSE;

            if (!(globalSprites[i].stateFlags & ANIMATION_HEADER_PROCESSED)) {
                continue;
            } 
            
            setAnimationFrameMetadata(&animationFrameMetadata, globalSprites[i].animationFrameMetadataPtr);

            if (globalSprites[i].stateFlags & 4) {
                
                if (globalSprites[i].animationCounter[gGraphicsBufferIndex] != globalSprites[i].currentAnimationFrame) {
                    setBitmapFromSpriteObject(i, &animationFrameMetadata, 2);
                } else {
                    setBitmapFromSpriteObject(i, &animationFrameMetadata, 1);
                }

                globalSprites[i].animationCounter[gGraphicsBufferIndex] = globalSprites[i].currentAnimationFrame;
        
            } else {        
                setBitmapFromSpriteObject(i, &animationFrameMetadata, 0);
            }

            if (!(globalSprites[i].stateFlags & ANIMATION_PAUSED)) {
                globalSprites[i].frameTickCounter += 2; 
            }

            if (globalSprites[i].frameTickCounter >= animationFrameMetadata.frameDuration) {

                globalSprites[i].audioTrigger = animationFrameMetadata.audioTrigger;
                globalSprites[i].frameTickCounter = 0;

                if (globalSprites[i].stateFlags & ANIMATION_PLAYING) {
                 
                    globalSprites[i].currentAnimationFrame++;
         
                    if (!(globalSprites[i].currentAnimationFrame < globalSprites[i].animationFrameMetadata.objectCount)) {

                        if (globalSprites[i].stateFlags & ANIMATION_LOOPS) {
                            
                            globalSprites[i].currentAnimationFrame = 0;
                            
                        } else if (globalSprites[i].stateFlags & DESTROY_ON_ANIMATION_END) {
                        
                            deactivateSprite(i);
                            
                        } else {
                            globalSprites[i].currentAnimationFrame = globalSprites[i].animationFrameMetadata.objectCount - 1;
                        }

                        globalSprites[i].stateFlags |= ANIMATION_STATE_CHANGED;
                        
                    } 

                    // globalSprites[i].animationDataPtr + 4 = skip header + 4 byte total metadata object count; start at first metadata object
                    ptr = getAnimationFrameMetadataPtrFromFrame(globalSprites[i].currentAnimationFrame, globalSprites[i].animationDataPtr + 4);

                    globalSprites[i].animationFrameMetadataPtr = ptr;
                    globalSprites[i].bitmapMetadataPtr = ptr + 2;
                    
                } else {
                    globalSprites[i].stateFlags |= ANIMATION_STATE_CHANGED;
                }
                
            }

        }   
    
        i++;
        
    } 
    
}
#else
INCLUDE_ASM("asm/nonmatchings/system/globalSprites", updateSprites);
#endif