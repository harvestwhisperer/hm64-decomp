#include "common.h"

#include "system/globalSprites.h"

#include "system/memory.h"
#include "system/sprite.h"
#include "system/mathUtils.h"

// forward declarations
bool func_8002B8E0(u16, u8, void*);
void setTotalAnimationFrames(SpriteAnimation*, u16*);             
u8* func_8002CD34(u16 arg0, void* arg1);
u16* func_8002CD4C(u16, u16*);   

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

// unsure... not technically a 32 bit swap
// also used in graphic.c
static inline u32 swap32(u16 halfword) {
    
    Swap32 swap;
    u16 lower;
    u16 upper;

    upper = (halfword & 0xFF) << 8;
    lower = halfword >> 8;
    
    swap.word = lower | upper;

    return swap.halfword[0] | swap.halfword[1];
    
}

static inline void swapBytes(SpriteAnimation *animation, Swap16 halfword) {

    animation->unk_2 = halfword.byte[0];
    animation->audioTrigger = halfword.byte[1];
    
}

// static inline u16 swap16Alt(u16 halfword) {

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

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", initializeGlobalSprites);

void initializeGlobalSprites(void) {

    u16 i;

    for (i = 0; i < MAX_SPRITES; i++) {
        
        globalSprites[i].stateFlags = 0;
        globalSprites[i].renderingFlags = 0;
        
        globalSprites[i].paletteIndex = 0;
        globalSprites[i].currentAnimationFrame = 0;
        globalSprites[i].audioTrigger = FALSE;
        globalSprites[i].frameTimer = 0;
        
        globalSprites[i].shrink.x = 0;
        globalSprites[i].shrink.y = 0;
        globalSprites[i].shrink.z = 0;
        
        globalSprites[i].scale.x = 1.0f;
        globalSprites[i].scale.y = 1.0f;
        globalSprites[i].scale.z = 1.0f;

        globalSprites[i].angles.x = 0;
        globalSprites[i].angles.y = 0;
        globalSprites[i].angles.z = 0;
        
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

bool dmaSprite(u16 index, u32 romTextureStart, u32 romTextureEnd, u32 romAssetIndexStart, u32 romAssetIndexEnd, 
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
            
            nuPiReadRom(romAssetIndexStart, assetIndex, romAssetIndexEnd - romAssetIndexStart);

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
            globalSprites[index].texturePtr = (void*)0;
            globalSprites[index].texture2Ptr = (void*)0;
            globalSprites[index].romTexturePtr = (void*)0;

            globalSprites[index].stateFlags = ACTIVE;
            globalSprites[index].renderingFlags = 0;

            globalSprites[index].paletteIndex = 0;
            globalSprites[index].audioTrigger = FALSE;

            setSpriteShrinkFactor(index, 0.0f, 0.0f, 0.0f);
            setSpriteScale(index, 1.0f, 1.0f, 1.0f);
            setSpriteAngles(index, 0.0f, 0.0f, 0.0f);
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
            globalSprites[spriteIndex].texturePtr = texturePtr;
            globalSprites[spriteIndex].texture2Ptr = texture2Ptr;
            globalSprites[spriteIndex].romTexturePtr = romTexturePtr;

            globalSprites[spriteIndex].stateFlags = (ACTIVE | 4);
            globalSprites[spriteIndex].renderingFlags = 0;

            globalSprites[spriteIndex].paletteIndex = 0;
            globalSprites[spriteIndex].audioTrigger = FALSE;

            setSpriteShrinkFactor(spriteIndex, 0, 0, 0);
            setSpriteScale(spriteIndex, 1.0f, 1.0f, 1.0f);
            setSpriteAngles(spriteIndex, 0, 0, 0);
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

bool deactivateSprite(u16 spriteIndex) {

    bool result = FALSE;

    if (spriteIndex < MAX_ACTIVE_SPRITES) {
        if (globalSprites[spriteIndex].stateFlags & ACTIVE) {
            globalSprites[spriteIndex].stateFlags &= ~(ACTIVE | 2);
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

// possibly inline, otherwise unused
bool func_8002B7A0(u16 spriteIndex, u8 arg1) {

    bool result;

    result = func_8002B8E0(spriteIndex, arg1, globalSprites[spriteIndex].animationIndexPtr);

    globalSprites[spriteIndex].animationCounter[0] = 0xFF;
    globalSprites[spriteIndex].animationCounter[1] = 0xFF;

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", startSpriteAnimation);

bool startSpriteAnimation(u16 spriteIndex, u16 offset, u8 arg2) {

    bool result = FALSE;
    
    if (spriteIndex < MAX_ACTIVE_SPRITES) {

        if (globalSprites[spriteIndex].stateFlags & ACTIVE) {
            
            if (globalSprites[spriteIndex].animationIndexPtr[offset] != globalSprites[spriteIndex].animationIndexPtr[offset+1]) {

                // func_8002CD34(offset, globalSprites[spriteIndex].animationIndexPtr) = get address + offset for unknown asset ptr based on sprite index
                result = func_8002B8E0(spriteIndex, arg2, func_8002CD34(offset, globalSprites[spriteIndex].animationIndexPtr));
                
                globalSprites[spriteIndex].animationCounter[0] = 0xFF;
                globalSprites[spriteIndex].animationCounter[1] = 0xFF;
           
            }
        }
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", func_8002B8E0);

bool func_8002B8E0(u16 spriteIndex, u8 arg1, void* animationDataPtr) {

    bool result = FALSE;
    u16 *temp;
    
    if ((spriteIndex < MAX_ACTIVE_SPRITES) && (globalSprites[spriteIndex].stateFlags & ACTIVE) && !(globalSprites[spriteIndex].stateFlags & ANIMATION_HEADER_PROCESSED)) {

        globalSprites[spriteIndex].animationDataPtr = animationDataPtr;

        globalSprites[spriteIndex].frameTimer = 0;
        globalSprites[spriteIndex].audioTrigger = FALSE;

        globalSprites[spriteIndex].stateFlags &= ~ANIMATION_STATE_CHANGED;
        globalSprites[spriteIndex].stateFlags |= ANIMATION_HEADER_PROCESSED;

        switch (arg1) {
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
                globalSprites[spriteIndex].currentAnimationFrame = arg1;
                globalSprites[spriteIndex].stateFlags &= ~(ANIMATION_PLAYING | ANIMATION_LOOPS);
                break;
        }

        // update animation from byteswapped table
        setTotalAnimationFrames(&globalSprites[spriteIndex].animation, globalSprites[spriteIndex].animationDataPtr);

        temp = func_8002CD4C(globalSprites[spriteIndex].currentAnimationFrame, globalSprites[spriteIndex].animationDataPtr+8);
        
        globalSprites[spriteIndex].unknownAsset3Ptr = temp;
        globalSprites[spriteIndex].unknownAsset4Ptr = temp + 2;

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

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", func_8002BBE0);

bool func_8002BBE0(u16 index, u8 arg1, u8 arg2) {

    bool result = FALSE;
    
    if (index < MAX_ACTIVE_SPRITES) {

        if (globalSprites[index].stateFlags & ACTIVE) {

            if (arg1) {
                globalSprites[index].renderingFlags |= 1;
            } else {
                globalSprites[index].renderingFlags &= ~1;
            }

            if (arg2) {
                globalSprites[index].renderingFlags |= 2;
            } else {
                globalSprites[index].renderingFlags &= ~2;
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

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", setSpriteAngles);

bool setSpriteAngles(u16 index, f32 x, f32 y, f32 z) {

    bool result = FALSE;

    if (index < MAX_ACTIVE_SPRITES) {

        if (globalSprites[index].stateFlags & ACTIVE) {

            globalSprites[index].angles.x = x;
            globalSprites[index].angles.y = y;
            globalSprites[index].angles.z = z;

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

bool func_8002C000(u16 index, f32 x, f32 y, f32 z) {

    bool result = FALSE;
    
    if (index < MAX_ACTIVE_SPRITES) {
        
        if (globalSprites[index].stateFlags & ACTIVE) {

            globalSprites[index].angles.x += x;
            globalSprites[index].angles.y += y;
            globalSprites[index].angles.z += z;
                
            result = TRUE;
            
        }   
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", func_8002C0B4);

// unused or inline
bool func_8002C0B4(u16 index, s8 r, s8 g, s8 b, s8 a) {

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

void setTotalAnimationFrames(SpriteAnimation *animation, u16 *animationData) {
    
    animation->totalFrames = swap16(animationData[2]);
    
}

inline void func_8002CCA8(SpriteAnimation* animation, u16* animationData) {

    animation->totalFrames = swap16(animationData[0]);
    swapBytes(animation, animationData[1]);
    
}

// inline used by func_8002CDE8
inline void func_8002CCDC(SpriteData* ptr, u16* data) {
    
    ptr->spriteIndex = swap32(data[0]);    
    ptr->unk_2 = 0;
    ptr->textureIndex = swap32(data[2]);
    ptr->unk_6 = swap16(data[3]);

}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", func_8002CD34);

// TODO: see if void* type is necessary
// unknown sprite asset index lookup
u8* func_8002CD34(u16 arg0, void* arg1) {
 
    u32 *arr = (u32*)arg1;
    
    return (u8*)(arg1 + arr[arg0]);
    
}

// alternate
/*
u8* func_8002CD34(u16 arg0, void* arg1) {
    return (u8*)(arg1 + *(u32*)(arg1 + arg0*4));
}
*/

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", func_8002CD4C);

// FIXME: probably uses inlines; this could be an inline function used by graphic.c too
// iterates through array of byteswapped shorts and swaps and increments ptr
// arg0 = unk_90, arg1 = animationDataPtr
inline u16* func_8002CD4C(u16 arg0, u16* arg1) {
    
    u16 i;
    SpriteAnimation unk;
    u32 pad[2];

    u32 temp;
    u32 temp2;
    u32 temp3;
    u8 temp4;
    
    i = 0;
    
    if (arg0 != 0) {
        
        do {
            
            // FIXME: use swap inline
            temp2 = arg1[0] << 8;
            temp3 = arg1[0] >> 8;
            
            temp = temp2 | temp3;
            
            unk.totalFrames = temp;

            temp2 = arg1[1];
            temp = temp2;
            temp2 = temp;
            
            temp2 = temp >> 8;
            temp4 = temp >> 8;
            
            unk.unk_2 = temp4;
            unk.audioTrigger = temp;
              
            arg1 += 2;
            arg1 += unk.totalFrames * 4;
            
            i++;
            
        } while (i < arg0);
    }
    
    return arg1;

}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", func_8002CDB4);

inline u16* func_8002CDB4(u16 arg0, u16* arg1) {
    
    u32 i;
    
    for (i = 0; (u16)i < arg0; i++) {
        arg1+=4;
    }

    return arg1;
    
}

// alternate
/*
u32 func_8002CDB4(u16 arg0, u32 arg1) {
    
    u16 i = 0;

    u32 padding[2];
    
    if (arg0) {
        do {
            arg1 += 8;
            i++;
        } while (i < arg0);
    }
    
    return arg1;
}
*/

// alternate
/*
u32 func_8002CDB4(u16 arg0, u32 arg1) {
    
    u32 result;
    u32 padding[2];
    
    u16 i = 0;
    
    if (arg0) {
        do {
            result += 8;
            i++;
        } while (i < arg0);
    }
    
    return result;
}
*/

// sprites to bitmaps
// set flags on bitmaps
// arg1 = stack
#ifdef PERMUTER
void func_8002CDE8(u16 spriteIndex, SpriteAnimation* arg1, u8 arg2) {

    u16 bitmapIndex;
    
    u16 *texturePtr;
    u16 *palettePtr;
    
    f32 shrinkX;
    f32 shrinkY;
    f32 shrinkZ;
    
    SpriteData data;
    
    u16 temp;
    u32 temp2;
    u16 temp3;
    u16 i;

    u32 pad2[4];
    
    data.ptr2 = arg1;
    temp = arg1->animation;
    
    data.length = 0;
    
    texturePtr = globalSprites[spriteIndex].texturePtrs[gGraphicsBufferIndex];

    if (temp) {
        
        i = 0;
    
        temp2 = arg2;
    
        do {
            
            if (temp2) {

                do {
                    
                    func_8002CCDC(&data, func_8002CDB4((temp - i) - 1, globalSprites[spriteIndex].unknownAsset4Ptr));
                    
                    texturePtr += data.length;
                    
                    if (temp2 == 2) {
                        func_8002A340(data.spriteIndex, globalSprites[spriteIndex].spritesheetIndexPtr, texturePtr, globalSprites[spriteIndex].romTexturePtr);
                    }
                    
                    data.length = func_8002A3A0(data.spriteIndex, globalSprites[spriteIndex].spritesheetIndexPtr);
                        
                } while (0);
                
            } else {

                do {
                    
                    temp3 = ((data.ptr2->animation - i) - 1);
    
                    if (temp2 < temp3) {
                        
                    }
    
                    func_8002CCDC(&data, func_8002CDB4(temp3, globalSprites[spriteIndex].unknownAsset4Ptr));
    
                    texturePtr = func_80028888(data.spriteIndex, globalSprites[spriteIndex].spritesheetIndexPtr);

                } while (0);
               
            }

            if (globalSprites[spriteIndex].stateFlags & DIRECT_PALETTE_MODE) {
                palettePtr = func_800288A0(globalSprites[spriteIndex].paletteIndex, globalSprites[spriteIndex].paletteIndexPtr);
            } else {
                palettePtr = func_800288B8(data.spriteIndex, globalSprites[spriteIndex].paletteIndexPtr, globalSprites[spriteIndex].spriteToPaletteMappingPtr);
            }

            shrinkX = globalSprites[spriteIndex].shrink.x;
            shrinkY = globalSprites[spriteIndex].shrink.y;
            shrinkZ = globalSprites[spriteIndex].shrink.z;

            if (globalSprites[spriteIndex].stateFlags & SPRITE_FLIP_WINDING) {
                bitmapIndex = func_80029DAC(texturePtr, palettePtr, (0x8 | 0x10 | 0x20 | 0x40));
            } else {
                bitmapIndex = func_80029DAC(texturePtr, palettePtr, (0x8 | 0x10 | 0x30));
            }
            
            func_8002A09C(bitmapIndex, shrinkX, shrinkY, shrinkZ);
            func_8002A120(bitmapIndex, globalSprites[spriteIndex].scale.x, globalSprites[spriteIndex].scale.y, globalSprites[spriteIndex].scale.z);
            func_8002A1A4(bitmapIndex, globalSprites[spriteIndex].angles.x, globalSprites[spriteIndex].angles.y, globalSprites[spriteIndex].angles.z);

            func_8002A228(bitmapIndex, globalSprites[spriteIndex].rgbaCurrent.r, globalSprites[spriteIndex].rgbaCurrent.g, globalSprites[spriteIndex].rgbaCurrent.b, globalSprites[spriteIndex].rgbaCurrent.a);

            func_8002A2E0(bitmapIndex, data.textureIndex, data.unk_6);
            
            func_80029F98(bitmapIndex, globalSprites[spriteIndex].renderingFlags & 1, globalSprites[spriteIndex].renderingFlags & 2);

            func_80029E2C(bitmapIndex, (globalSprites[spriteIndex].renderingFlags >> 3) & (1 | 3), (globalSprites[spriteIndex].renderingFlags >> 5) & (1 | 3));
            func_80029EA4(bitmapIndex, (globalSprites[spriteIndex].renderingFlags >> 7) & (1 | 3));
            func_80029F14(bitmapIndex, globalSprites[spriteIndex].renderingFlags & 0x200);
            func_8002A02C(bitmapIndex, (globalSprites[spriteIndex].renderingFlags >> 10) & (1 | 2 | 4));

            if (globalSprites[spriteIndex].stateFlags & ENABLE_BILINEAR_FILTERING) {
                bitmaps[bitmapIndex].flags |= USE_BILINEAR_FILTERING;
            } else {
                bitmaps[bitmapIndex].flags &= ~USE_BILINEAR_FILTERING;
            }

            temp = data.ptr2->animation;
            i++;
            
        } while (i < data.ptr2->animation);
        
    }
    
}
#else
INCLUDE_ASM("asm/nonmatchings/system/globalSprites", func_8002CDE8);
#endif

#ifdef PERMUTER
void updateSprites(void) {

    u16 i = 0;
    u8 check;
    f32 tempf;
    u16* ptr;
    SpriteAnimation animation;
    
    u32 pad[4];

    do {

        if (globalSprites[i].stateFlags & 1) {
        
            check = 0;

            if (globalSprites[i].rgbaCurrent.r <  globalSprites[i].rgbaTarget.r) {
                
                tempf = globalSprites[i].rgbaCurrent.r + globalSprites[i].rgbaDelta.r;
                globalSprites[i].rgbaCurrent.r = tempf;
                
                if (globalSprites[i].rgbaTarget.r <= tempf) {
                    globalSprites[i].rgbaCurrent.r = globalSprites[i].rgbaTarget.r;
                } else {
                    check = 1;
                }
            }
    
            if (globalSprites[i].rgbaCurrent.r > globalSprites[i].rgbaTarget.r) {
                
                tempf = globalSprites[i].rgbaCurrent.r - globalSprites[i].rgbaDelta.r;
                globalSprites[i].rgbaCurrent.r = tempf;
                
                if (globalSprites[i].rgbaTarget.r >= tempf) {
                    globalSprites[i].rgbaCurrent.r = globalSprites[i].rgbaTarget.r;
                } else {
                    check++;
                }
            }
    
            if (globalSprites[i].rgbaCurrent.g <  globalSprites[i].rgbaTarget.g) {
                
                tempf = globalSprites[i].rgbaCurrent.g + globalSprites[i].rgbaDelta.g;
                globalSprites[i].rgbaCurrent.g = tempf;
                
                if ((globalSprites[i].rgbaTarget.g <= tempf)) {
                    globalSprites[i].rgbaCurrent.g = globalSprites[i].rgbaTarget.g;
                } else {
                    check++;
                }
            }
    
            if (globalSprites[i].rgbaCurrent.g >  globalSprites[i].rgbaTarget.g) {
                
                tempf = globalSprites[i].rgbaCurrent.g - globalSprites[i].rgbaDelta.g;
                globalSprites[i].rgbaCurrent.g = tempf;
                
                if (globalSprites[i].rgbaTarget.g >= tempf) {
                    globalSprites[i].rgbaCurrent.g = globalSprites[i].rgbaTarget.g;
                } else {
                    check++;
                }
            }
    
            if (globalSprites[i].rgbaCurrent.b <  globalSprites[i].rgbaTarget.b) {
                
                tempf = globalSprites[i].rgbaCurrent.b + globalSprites[i].rgbaDelta.b;
                globalSprites[i].rgbaCurrent.b = tempf;
                
                if ((globalSprites[i].rgbaTarget.b <= tempf)) {
                    globalSprites[i].rgbaCurrent.b = globalSprites[i].rgbaTarget.b;
                } else {
                    check++;
                }
            }
    
            if (globalSprites[i].rgbaCurrent.b >  globalSprites[i].rgbaTarget.b) {
                
                tempf = globalSprites[i].rgbaCurrent.b - globalSprites[i].rgbaDelta.b;
                globalSprites[i].rgbaCurrent.b = tempf;
                
                if (globalSprites[i].rgbaTarget.b >= tempf) {
                    globalSprites[i].rgbaCurrent.b = globalSprites[i].rgbaTarget.b;
                } else {
                    check++;
                }
            }
    
            if (globalSprites[i].rgbaCurrent.a <  globalSprites[i].rgbaTarget.a) {
                
                tempf = globalSprites[i].rgbaCurrent.a + globalSprites[i].rgbaDelta.a;
                globalSprites[i].rgbaCurrent.a = tempf;
                
                if ((globalSprites[i].rgbaTarget.a <= tempf)) {
                    globalSprites[i].rgbaCurrent.a = globalSprites[i].rgbaTarget.a;
                } else {
                    check++;
                }
            }
    
            if (globalSprites[i].rgbaCurrent.a >  globalSprites[i].rgbaTarget.a) {
                
                tempf = globalSprites[i].rgbaCurrent.a - globalSprites[i].rgbaDelta.a;
                globalSprites[i].rgbaCurrent.a = tempf;
                
                if (globalSprites[i].rgbaTarget.a >= tempf) {
                    globalSprites[i].rgbaCurrent.a = globalSprites[i].rgbaTarget.a;
                } else {
                    check++;
                }
            }

            if (check == 0) {
                globalSprites[i].stateFlags |= RGBA_IN_PROGRESS;
            }

            globalSprites[i].audioTrigger = FALSE;

            if (globalSprites[i].stateFlags & ANIMATION_HEADER_PROCESSED) { 

                func_8002CCA8(&animation, globalSprites[i].unknownAsset3Ptr);

                if (globalSprites[i].stateFlags & 4) {
                    
                    if (globalSprites[i].animationCounter[gGraphicsBufferIndex] != globalSprites[i].currentAnimationFrame) {
                        func_8002CDE8(i, &animation, 2);
                    } else {
                        func_8002CDE8(i, &animation, 1);
                    }

                    globalSprites[i].animationCounter[gGraphicsBufferIndex] = globalSprites[i].currentAnimationFrame;
                    
                } else {
                    func_8002CDE8(i, &animation, 0);
                }

                if (!(globalSprites[i].stateFlags & ANIMATION_PAUSED)) {
                    globalSprites[i].frameTimer += 2; 
                }

                if (globalSprites[i].frameTimer >= animation.unk_2) {

                    globalSprites[i].audioTrigger = animation.audioTrigger;
                    globalSprites[i].frameTimer = 0;

                    if (globalSprites[i].stateFlags & ANIMATION_PLAYING) {

                        globalSprites[i].currentAnimationFrame += 1;
                        
                        if (globalSprites[i].animation.totalFrames <= globalSprites[i].currentAnimationFrame) {

                            if (globalSprites[i].stateFlags & ANIMATION_LOOPS) {
                                
                                globalSprites[i].currentAnimationFrame = 0;
                                
                            } else if (globalSprites[i].stateFlags & DESTROY_ON_ANIMATION_END) {
                                
                                if (i < MAX_ACTIVE_SPRITES && globalSprites[i].stateFlags & ACTIVE) {
                                    globalSprites[i].stateFlags &= ~(ACTIVE | ANIMATION_HEADER_PROCESSED); 
                                }
                                
                            } else {
                                globalSprites[i].currentAnimationFrame -= 2;
                            }

                            globalSprites[i].stateFlags |= ANIMATION_STATE_CHANGED;
                            
                        }

                        ptr = globalSprites[i].animationDataPtr + 8;

                        func_8002CD4C(globalSprites[i].currentAnimationFrame, globalSprites[i].animationDataPtr);

                        globalSprites[i].unknownAsset3Ptr = ptr;
                        globalSprites[i].unknownAsset4Ptr = ptr + 2;
                        
                    } else {
                        globalSprites[i].stateFlags |= ANIMATION_STATE_CHANGED;
                    }
                    
                }

            }
        }   
    
        i++;
        
    } while (i < MAX_ACTIVE_SPRITES);
        
} 
#else
INCLUDE_ASM("asm/nonmatchings/system/globalSprites", updateSprites);
#endif