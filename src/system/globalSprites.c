#include "common.h"

#include "system/globalSprites.h"

#include "graphic.h"
#include "system/memory.h"
#include "system/sprite.h"
#include "system/math.h"
#include "system/sceneGraph.h"

#include "mainproc.h"

// forward declarations
bool setupSpriteAnimation(u16 spriteIndex, u8 animationModeOrFrameIndex, u16* animationDataPtr);
void setTotalAnimationFrames(AnimationFrameMetadata*, Swap16*);             
u8* getAnimationFrameMetadataPtr(u16 arg0, void* arg1);
inline AnimationFrameMetadata* getAnimationFrameMetadataPtrFromFrame(u16, u16*);   

// bss
SpriteObject globalSprites[MAX_SPRITES];


static inline u16 swap16(Swap16 halfword) {
    
    Swap16 swap;
    
    swap.byte[1] = halfword.byte[0];
    swap.byte[0] = halfword.byte[1];
    
    return swap.halfword;

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
        
        globalSprites[i].viewSpacePosition.x = 0;
        globalSprites[i].viewSpacePosition.y = 0;
        globalSprites[i].viewSpacePosition.z = 0;
        
        globalSprites[i].scale.x = 1.0f;
        globalSprites[i].scale.y = 1.0f;
        globalSprites[i].scale.z = 1.0f;

        globalSprites[i].rotation.x = 0;
        globalSprites[i].rotation.y = 0;
        globalSprites[i].rotation.z = 0;
        
        globalSprites[i].currentRGBA.r = 0;
        globalSprites[i].currentRGBA.g = 0;
        globalSprites[i].currentRGBA.b = 0;
        globalSprites[i].currentRGBA.a = 0;
        
        globalSprites[i].targetRGBA.r = 0;
        globalSprites[i].targetRGBA.g = 0;
        globalSprites[i].targetRGBA.b = 0;
        globalSprites[i].targetRGBA.a = 0;

    }
}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", dmaSprite);

bool dmaSprite(u16 index, 
    u32 romTextureStart, u32 romTextureEnd, 
    u32 romAssetsIndexStart, u32 romAssetsIndexEnd, 
    u32 romSpritesheetIndexStart, u32 romSpritesheetIndexEnd, 
    u8* texture1Vaddr, u8* texture2Vaddr, 
    u16* paletteVaddr, AnimationFrameMetadata* animationVaddr, 
    u8* spriteToPaletteVaddr, u32* spritesheetIndexVaddr, 
    u8 assetType, bool transformExempt) {

    u32 assetIndex[8];
    bool result = FALSE;
    
    u32 offset1;
    u32 offset2;
    u32 offset3;
    u32 offset4;
    u32 offset5;

    if (index < MAX_SPRITES) {
        
        if (!(globalSprites[index].stateFlags & SPRITE_ACTIVE)) { 
            
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

                setSpriteType1(index, animationVaddr, spritesheetIndexVaddr, paletteVaddr, spriteToPaletteVaddr, romTextureStart, texture1Vaddr, texture2Vaddr);

            // spritesheet index in spritesheet
            } else {

                nuPiReadRom(romTextureStart + offset1, texture1Vaddr, offset2 - offset1);
                nuPiReadRom(romTextureStart + offset2, paletteVaddr, offset3 - offset2);
                nuPiReadRom(romTextureStart + offset3, animationVaddr, offset4 - offset3);
                nuPiReadRom(romTextureStart + offset4, spriteToPaletteVaddr, offset5 - offset4);
                
                setSpriteType2(index, animationVaddr, texture1Vaddr, paletteVaddr, spriteToPaletteVaddr);

            }

            if (!transformExempt) {
                globalSprites[index].stateFlags |= SPRITE_NO_TRANSFORM;
            }

            globalSprites[index].stateFlags |= SPRITE_ANIMATION_STATE_CHANGED;

            result = TRUE;

        }
    
    }

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", setSpriteType2);

bool setSpriteType2(u16 index, u32* animationIndexPtr, u32* spritesheetIndexPtr, u32* paletteIndexPtr, u8* spriteToPaletteMappingPtr) {

    bool result = FALSE;

    if (index < MAX_SPRITES) {

        if (!(globalSprites[index].stateFlags & SPRITE_ACTIVE)) {

            globalSprites[index].animationIndexPtr = animationIndexPtr;
            globalSprites[index].spritesheetIndexPtr = spritesheetIndexPtr;
            globalSprites[index].paletteIndexPtr = paletteIndexPtr;
            globalSprites[index].spriteToPaletteMappingPtr = spriteToPaletteMappingPtr;
            globalSprites[index].texturePtr[0] = NULL;
            globalSprites[index].texturePtr[1] = NULL;
            globalSprites[index].romTexturePtr = NULL;

            globalSprites[index].stateFlags = SPRITE_ACTIVE;
            globalSprites[index].renderingFlags = 0;

            globalSprites[index].paletteIndex = 0;
            globalSprites[index].audioTrigger = FALSE;

            setSpriteViewSpacePosition(index, 0.0f, 0.0f, 0.0f);
            setSpriteScale(index, 1.0f, 1.0f, 1.0f);
            setSpriteRotation(index, 0.0f, 0.0f, 0.0f);
            setSpriteBaseRGBA(index, 0xFF, 0xFF, 0xFF, 0xFF);
            setSpriteAnchorAlignment(index, SPRITE_ANCHOR_CENTER, SPRITE_ANCHOR_CENTER);;
            setSpriteAxisMapping(index, SPRITE_BILLBOARD_XY);
            setSpriteBlendMode(index, SPRITE_BLEND_ALPHA_DECAL);
            
            result = TRUE;
            
        }
    }
 
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", setSpriteType1);

bool setSpriteType1(u16 spriteIndex, u32* animationIndexPtr, u32* spritesheetIndexPtr, u32* paletteIndexPtr, u8* spriteToPaletteMappingPtr, u32 romTexturePtr, u8* texturePtr, u8* texture2Ptr) {
    
    bool result = FALSE;

    if (spriteIndex < MAX_SPRITES) {

        if (!(globalSprites[spriteIndex].stateFlags & SPRITE_ACTIVE)) {

            globalSprites[spriteIndex].animationIndexPtr = animationIndexPtr;
            globalSprites[spriteIndex].spritesheetIndexPtr = spritesheetIndexPtr;
            globalSprites[spriteIndex].paletteIndexPtr = paletteIndexPtr;
            globalSprites[spriteIndex].spriteToPaletteMappingPtr = spriteToPaletteMappingPtr;
            globalSprites[spriteIndex].texturePtr[0] = texturePtr;
            globalSprites[spriteIndex].texturePtr[1] = texture2Ptr;
            globalSprites[spriteIndex].romTexturePtr = romTexturePtr;

            globalSprites[spriteIndex].stateFlags = (SPRITE_ACTIVE | 4);
            globalSprites[spriteIndex].renderingFlags = 0;

            globalSprites[spriteIndex].paletteIndex = 0;
            globalSprites[spriteIndex].audioTrigger = FALSE;

            setSpriteViewSpacePosition(spriteIndex, 0, 0, 0);
            setSpriteScale(spriteIndex, 1.0f, 1.0f, 1.0f);
            setSpriteRotation(spriteIndex, 0, 0, 0);
            setSpriteBaseRGBA(spriteIndex, 0xFF, 0xFF, 0xFF, 0xFF);
            setSpriteAnchorAlignment(spriteIndex, SPRITE_ANCHOR_CENTER, SPRITE_ANCHOR_CENTER);;
            setSpriteAxisMapping(spriteIndex, SPRITE_BILLBOARD_XY);
            setSpriteBlendMode(spriteIndex, SPRITE_BLEND_ALPHA_DECAL);
            
            result = TRUE;
            
        }
    }
 
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", deactivateSprite);

inline bool deactivateSprite(u16 spriteIndex) {

    bool result = FALSE;

    if (spriteIndex < MAX_SPRITES) {
        if (globalSprites[spriteIndex].stateFlags & SPRITE_ACTIVE) {
            globalSprites[spriteIndex].stateFlags &= ~(SPRITE_ACTIVE | SPRITE_ANIMATION_HEADER_PROCESSED);
            result = TRUE;
        }        
    }

    return result; 

}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", deactivateGlobalSprites);

void deactivateGlobalSprites(void) {

    u16 i;

    for (i = 0; i < MAX_SPRITES; i++) {

        if ((globalSprites[i].stateFlags & SPRITE_ACTIVE) && (globalSprites[i].stateFlags & SPRITE_ANIMATION_HEADER_PROCESSED)) {
            globalSprites[i].stateFlags &= ~(SPRITE_ACTIVE | 2);
        }

        globalSprites[i].stateFlags = 0;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", func_8002B7A0);

// unused or inline
// bool func_8002B7A0(u16 spriteIndex, u8 animationModeOrFrameIndex) {

//     bool result;

//     result = setupSpriteAnimation(spriteIndex, animationModeOrFrameIndex, globalSprites[spriteIndex].animationIndexPtr);

//     globalSprites[spriteIndex].animationCounter[0] = 0xFF;
//     globalSprites[spriteIndex].animationCounter[1] = 0xFF;

//     return result;
    
// }

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", startSpriteAnimation);

bool startSpriteAnimation(u16 spriteIndex, u16 animationIndex, u8 animationModeOrFrameIndex) {

    bool result = FALSE;
    
    if (spriteIndex < MAX_SPRITES) {

        if (globalSprites[spriteIndex].stateFlags & SPRITE_ACTIVE) {
            
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
    
    if ((spriteIndex < MAX_SPRITES) && (globalSprites[spriteIndex].stateFlags & SPRITE_ACTIVE) && !(globalSprites[spriteIndex].stateFlags & SPRITE_ANIMATION_HEADER_PROCESSED)) {

        globalSprites[spriteIndex].animationDataPtr = animationDataPtr;

        globalSprites[spriteIndex].frameTickCounter = 0;
        globalSprites[spriteIndex].audioTrigger = FALSE;

        globalSprites[spriteIndex].stateFlags &= ~SPRITE_ANIMATION_STATE_CHANGED;
        globalSprites[spriteIndex].stateFlags |= SPRITE_ANIMATION_HEADER_PROCESSED;

        switch (animationModeOrFrameIndex) {
            case 0xFF:
                globalSprites[spriteIndex].currentAnimationFrame = 0;
                globalSprites[spriteIndex].stateFlags &= ~SPRITE_ANIMATION_LOOPS;
                globalSprites[spriteIndex].stateFlags |= SPRITE_ANIMATION_PLAYING;
                break;
            case 0xFE:
                globalSprites[spriteIndex].currentAnimationFrame = 0;
                globalSprites[spriteIndex].stateFlags |= SPRITE_ANIMATION_PLAYING | SPRITE_ANIMATION_LOOPS;
                break;
            case 0xFD:
                globalSprites[spriteIndex].currentAnimationFrame = 0;
                globalSprites[spriteIndex].stateFlags |= SPRITE_ANIMATION_PLAYING | SPRITE_DESTROY_ON_ANIMATION_END;
                break;
            default:
                // for static sprites, show a single frame
                globalSprites[spriteIndex].currentAnimationFrame = animationModeOrFrameIndex;
                globalSprites[spriteIndex].stateFlags &= ~(SPRITE_ANIMATION_PLAYING | SPRITE_ANIMATION_LOOPS);
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

    if (index < MAX_SPRITES) {

        if (globalSprites[index].stateFlags & SPRITE_ACTIVE) {
            globalSprites[index].stateFlags &= ~SPRITE_ANIMATION_HEADER_PROCESSED;
            result = TRUE;
        }
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", resumeSpriteAnimation);

bool resumeSpriteAnimation(u16 index) {

    bool result = FALSE;

    if (index < MAX_SPRITES) {

        if (globalSprites[index].stateFlags & SPRITE_ACTIVE) {
            globalSprites[index].stateFlags &= ~SPRITE_ANIMATION_PAUSED;
            result = TRUE;
        }
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", pauseSpriteAnimation);

bool pauseSpriteAnimation(u16 index) {

    bool result = FALSE;

    if (index < MAX_SPRITES) {

        if (globalSprites[index].stateFlags & SPRITE_ACTIVE) {
            globalSprites[index].stateFlags |= SPRITE_ANIMATION_PAUSED;
            result = TRUE;
        }
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", setSpriteFlip);

bool setSpriteFlip(u16 index, bool flipHorizontal, bool flipVertical) {

    bool result = FALSE;
    
    if (index < MAX_SPRITES) {

        if (globalSprites[index].stateFlags & SPRITE_ACTIVE) {

            if (flipHorizontal) {
                globalSprites[index].renderingFlags |= SPRITE_RENDERING_FLIP_HORIZONTAL;
            } else {
                globalSprites[index].renderingFlags &= ~SPRITE_RENDERING_FLIP_HORIZONTAL;
            }

            if (flipVertical) {
                globalSprites[index].renderingFlags |= SPRITE_RENDERING_FLIP_VERTICAL;
            } else {
                globalSprites[index].renderingFlags &= ~SPRITE_RENDERING_FLIP_VERTICAL;
            }
            
            result = TRUE;

        }
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", getSpriteAnimationStateChangedFlag);

u16 getSpriteAnimationStateChangedFlag(u16 index) {

    u16 result = FALSE;

    if (index < MAX_SPRITES) {

        if (globalSprites[index].stateFlags & SPRITE_ACTIVE) {
            result = globalSprites[index].stateFlags & SPRITE_ANIMATION_STATE_CHANGED;
        }

    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", setSpriteViewSpacePosition);

bool setSpriteViewSpacePosition(u16 index, f32 x, f32 y, f32 z) {

    bool result = FALSE;

    if (index < MAX_SPRITES) {

        if (globalSprites[index].stateFlags & SPRITE_ACTIVE) {

            globalSprites[index].viewSpacePosition.x = x;
            globalSprites[index].viewSpacePosition.y = y;
            globalSprites[index].viewSpacePosition.z = z;

            result = TRUE;

        }
    }

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", setSpriteScale);

bool setSpriteScale(u16 index, f32 x, f32 y, f32 z) {

    bool result = FALSE;

    if (index < MAX_SPRITES) {

        if (globalSprites[index].stateFlags & SPRITE_ACTIVE) {

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

    if (index < MAX_SPRITES) {

        if (globalSprites[index].stateFlags & SPRITE_ACTIVE) {

            globalSprites[index].rotation.x = x;
            globalSprites[index].rotation.y = y;
            globalSprites[index].rotation.z = z;

            result = TRUE;

        }
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", adjustSpriteViewSpacePosition);

bool adjustSpriteViewSpacePosition(u16 index, f32 x, f32 y, f32 z) {

    bool result = FALSE;
    
    if (index < MAX_SPRITES) {
        
        if (globalSprites[index].stateFlags & SPRITE_ACTIVE) {

            globalSprites[index].viewSpacePosition.x += x;
            globalSprites[index].viewSpacePosition.y += y;
            globalSprites[index].viewSpacePosition.z += z;
                
            result = TRUE;
            
        }   
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", adjustSpriteScale);

// unused
// bool adjustSpriteScale(u16 index, f32 x, f32 y, f32 z) {

//     bool result = FALSE;
    
//     if (index < MAX_SPRITES) {

//         if (globalSprites[index].stateFlags & SPRITE_ACTIVE) {

//             globalSprites[index].scale.x += x;
//             globalSprites[index].scale.y += y;
//             globalSprites[index].scale.z += z;
            
//             result = TRUE;

//         }
//     }

//     return result;
    
// }

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", adjustSpriteRotation);

// unused or inline
// bool adjustSpriteRotation(u16 index, f32 x, f32 y, f32 z) {

//     bool result = FALSE;
    
//     if (index < MAX_SPRITES) {
        
//         if (globalSprites[index].stateFlags & SPRITE_ACTIVE) {

//             globalSprites[index].rotation.x += x;
//             globalSprites[index].rotation.y += y;
//             globalSprites[index].rotation.z += z;
                
//             result = TRUE;
            
//         }   
//     }
    
//     return result;

// }

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", adjustSpriteRGBA);

// unused or inline
// bool adjustSpriteRGBA(u16 index, s8 r, s8 g, s8 b, s8 a) {

//     bool result = FALSE;
    
//     if (index < MAX_SPRITES) {

//         if (globalSprites[index].stateFlags & SPRITE_ACTIVE) {

//             globalSprites[index].currentRGBA.r += r;
//             globalSprites[index].currentRGBA.g += g;
//             globalSprites[index].currentRGBA.b += b;
//             globalSprites[index].currentRGBA.a += a;
            
//             result = TRUE;
            
//         }   
//     }
    
//     return result;

// }

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", updateSpriteRGBA);

// update sprite rgba
// fade out on screen transitions
bool updateSpriteRGBA(u16 index, u8 r, u8 g, u8 b, u8 a, s16 rate) {

    bool result;
    
    s16 absValueRate = getAbsoluteValue(rate);
    f32 tempFloat;
    
    result = 0;
    
    if (index < MAX_SPRITES) {
        
        if (globalSprites[index].stateFlags & SPRITE_ACTIVE) {
 
            globalSprites[index].targetRGBA.r = (globalSprites[index].baseRGBA.r * r) / 255.0f;
            globalSprites[index].targetRGBA.g = (globalSprites[index].baseRGBA.g * g) / 255.0f;
            globalSprites[index].targetRGBA.b = (globalSprites[index].baseRGBA.b * b) / 255.0f;
            globalSprites[index].targetRGBA.a = (globalSprites[index].baseRGBA.a * a) / 255.0f;
            
            globalSprites[index].stateFlags &= ~SPRITE_RGBA_IN_PROGRESS;

            if (globalSprites[index].targetRGBA.r < globalSprites[index].currentRGBA.r) {
                tempFloat = globalSprites[index].currentRGBA.r - globalSprites[index].targetRGBA.r;
            } else {
                tempFloat = globalSprites[index].targetRGBA.r - globalSprites[index].currentRGBA.r;
            }

            globalSprites[index].deltaRGBA.r = (tempFloat * absValueRate) / globalSprites[index].baseRGBA.r;

            if (globalSprites[index].targetRGBA.g < globalSprites[index].currentRGBA.g) {
                tempFloat = globalSprites[index].currentRGBA.g - globalSprites[index].targetRGBA.g;
            } else {
                tempFloat = globalSprites[index].targetRGBA.g - globalSprites[index].currentRGBA.g;
            }

            globalSprites[index].deltaRGBA.g = (tempFloat * absValueRate) / globalSprites[index].baseRGBA.g;

            if (globalSprites[index].targetRGBA.b < globalSprites[index].currentRGBA.b) {
                tempFloat = globalSprites[index].currentRGBA.b - globalSprites[index].targetRGBA.b;
            } else {
                tempFloat = globalSprites[index].targetRGBA.b - globalSprites[index].currentRGBA.b;
            }

            globalSprites[index].deltaRGBA.b = (tempFloat * absValueRate) / globalSprites[index].baseRGBA.b;

            if (globalSprites[index].targetRGBA.a < globalSprites[index].currentRGBA.a) {
                tempFloat = globalSprites[index].currentRGBA.a - globalSprites[index].targetRGBA.a;
            } else {
                tempFloat = globalSprites[index].targetRGBA.a - globalSprites[index].currentRGBA.a;
            }

            globalSprites[index].deltaRGBA.a = (tempFloat * absValueRate) / globalSprites[index].baseRGBA.a;
            
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

    if (index < MAX_SPRITES) {

        if (globalSprites[index].stateFlags & SPRITE_ACTIVE) {

            globalSprites[index].targetRGBA.a = (globalSprites[index].baseRGBA.a * arg1) / 255.0f;

            globalSprites[index].stateFlags &= ~SPRITE_RGBA_IN_PROGRESS;

            if (globalSprites[index].targetRGBA.a < globalSprites[index].currentRGBA.a) {
                tempF = globalSprites[index].currentRGBA.a - globalSprites[index].targetRGBA.a;
            } else {
                tempF = globalSprites[index].targetRGBA.a - globalSprites[index].currentRGBA.a;
            }

            globalSprites[index].deltaRGBA.a = (tempF * absValueRate) / globalSprites[index].baseRGBA.a;
            
            result = TRUE;

        }
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", setSpriteAnchorAlignment);

bool setSpriteAnchorAlignment(u16 index, u16 arg1, u16 flags) {

    bool result = FALSE;
    int temp;

    if (index < MAX_SPRITES) {

        if (globalSprites[index].stateFlags & SPRITE_ACTIVE) { 
            
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

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", setSpriteAxisMapping);

bool setSpriteAxisMapping(u16 index, u16 mode) {
    
    int temp;

    bool result = FALSE;
    
    if (index < MAX_SPRITES) {
        
        if (globalSprites[index].stateFlags & SPRITE_ACTIVE) {
            
            globalSprites[index].renderingFlags &= ~(0x80 | 0x100);
            temp = mode << 7;
            globalSprites[index].renderingFlags |= temp;
            
            result = TRUE;

        }
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", setSpriteTriangleWinding);

bool setSpriteTriangleWinding(u16 index, u16 arg1) {

    bool result = FALSE;
    
    if (index < MAX_SPRITES) {
        
        if (globalSprites[index].stateFlags & SPRITE_ACTIVE) {

            if (arg1) {
                globalSprites[index].renderingFlags |= SPRITE_RENDERING_REVERSE_WINDING;
            } else {
                globalSprites[index].renderingFlags &= ~SPRITE_RENDERING_REVERSE_WINDING;
            }


            result = TRUE;

        }
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", setSpriteBlendMode);

bool setSpriteBlendMode(u16 index, u16 flags) {
    
    int temp;

    bool result = FALSE;
    
    if (index < MAX_SPRITES) {
        
        if (globalSprites[index].stateFlags & SPRITE_ACTIVE) {
            
            globalSprites[index].renderingFlags &= ~BITMAP_BLEND_MODE_MASK;
            temp = flags << 10;
            globalSprites[index].renderingFlags |= temp;
            
            result = TRUE;

        }
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", setSpriteBaseRGBA);

bool setSpriteBaseRGBA(u16 index, u8 r, u8 g, u8 b, u8 a) {

    bool result = FALSE;

    if (index < MAX_SPRITES) {
        
        if (globalSprites[index].stateFlags & SPRITE_ACTIVE) {
            
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
    
    if (index < MAX_SPRITES) {
        
        if (globalSprites[index].stateFlags & SPRITE_ACTIVE) {        

            result = TRUE;
            
            globalSprites[index].currentRGBA.r = (globalSprites[index].baseRGBA.r * r) / 255.0f;
            globalSprites[index].currentRGBA.g = (globalSprites[index].baseRGBA.g * g) / 255.0f;
            globalSprites[index].currentRGBA.b = (globalSprites[index].baseRGBA.b * b) / 255.0f;
            globalSprites[index].currentRGBA.a = (globalSprites[index].baseRGBA.a * a) / 255.0f;

            globalSprites[index].targetRGBA.r = (globalSprites[index].baseRGBA.r * r) / 255.0f;
            globalSprites[index].targetRGBA.g = (globalSprites[index].baseRGBA.g * g) / 255.0f;
            globalSprites[index].targetRGBA.b = (globalSprites[index].baseRGBA.b * b) / 255.0f;
            globalSprites[index].targetRGBA.a = (globalSprites[index].baseRGBA.a * a) / 255.0f;

        }
    }
    
    return result;
}


//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", setSpriteAlpha);

bool setSpriteAlpha(u16 index, u8 a) {
    
    bool result = FALSE;

    if (index < MAX_SPRITES) {

        if (globalSprites[index].stateFlags & SPRITE_ACTIVE) {

            globalSprites[index].baseRGBA.a = a;
            globalSprites[index].currentRGBA.a = a;
            globalSprites[index].targetRGBA.a = a;
            
            result = TRUE;

        }
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", setBilinearFiltering);

bool setBilinearFiltering(u16 index, bool useBilinearFiltering) {

    bool result = FALSE;

    if (index < MAX_SPRITES) {
        
        if (globalSprites[index].stateFlags & SPRITE_ACTIVE) {
            if (useBilinearFiltering == TRUE) {
                globalSprites[index].stateFlags |= SPRITE_ENABLE_BILINEAR_FILTERING;
            } else {
                globalSprites[index].stateFlags &= ~SPRITE_ENABLE_BILINEAR_FILTERING;
            }

            result = TRUE;

        }
    }
    
    return result;
}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", setSpritePaletteIndex);

bool setSpritePaletteIndex(u16 index, u16 paletteIndex) {

    bool result = FALSE;

    if (index < MAX_SPRITES) {

        if (globalSprites[index].stateFlags & SPRITE_ACTIVE) {

            globalSprites[index].paletteIndex = paletteIndex;
            globalSprites[index].stateFlags |= SPRITE_DIRECT_PALETTE_MODE;

            result = TRUE;

        }
    }

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", checkSpriteRGBAUpdateFinished);

// seems like check fade out completed
bool checkSpriteRGBAUpdateFinished(u16 index) {

    bool result;

    result = FALSE;
    
    if (index < MAX_SPRITES) {
        if (globalSprites[index].stateFlags & SPRITE_ACTIVE) {
            result = (globalSprites[index].stateFlags >> 10) & 1;
        }
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", checkSpriteAnimationStateChanged);

bool checkSpriteAnimationStateChanged(u16 index) {

    bool result = FALSE;

    if (index < MAX_SPRITES) {
        // flag 0x40, SPRITE_ANIMATION_STATE_CHANGED
        result = (globalSprites[index].stateFlags >> 6) & 1;
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", setTotalAnimationFrames);

void setTotalAnimationFrames(AnimationFrameMetadata *animationFrameMetadata, Swap16 *animationAsset) {
    
    // skip 4 byte header
    animationFrameMetadata->objectCount = swap16(animationAsset[2]);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", setAnimationFrameMetadata);

inline void setAnimationFrameMetadata(AnimationFrameMetadata* animationFrameMetadata, Swap16* animationFrameMetadataPtr) {

    Swap16 swap;

    swap.byte[1] = animationFrameMetadataPtr[0].halfword >> 8;
    swap.byte[0] = animationFrameMetadataPtr[0].halfword;
    
    animationFrameMetadata->objectCount = swap.halfword;

    swap.halfword = animationFrameMetadataPtr[1].halfword;

    animationFrameMetadata->frameDuration = swap.byte[0];
    animationFrameMetadata->audioTrigger = swap.byte[1];
    
}

// alternate: 
/*
static inline void swapBytes(AnimationFrameMetadata *animationFrameMetadata, Swap16 halfword) {
    
    animationFrameMetadata->frameDuration = halfword.byte[0];
    animationFrameMetadata->audioTrigger = halfword.byte[1];

}

inline void setAnimationFrameMetadata(AnimationFrameMetadata* animationFrameMetadata, u16* animationFrameMetadataPtr) {
    
    animationFrameMetadata->objectCount = swap16(animationFrameMetadataPtr[0]);
    swapBytes(animationFrameMetadata, animationFrameMetadataPtr[1]);
    animationFrameMetadata->frameDuration = swap.byte[0];
    animationFrameMetadata->audioTrigger = swap.byte[1];

}
*/

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", setBitmapMetadata);

inline void setBitmapMetadata(BitmapMetadata* ptr, Swap16* data) {
    
    Swap16 swap;
    
    swap.byte[1] = data[0].halfword >> 8;
    swap.byte[0] = data[0].halfword;
    
    ptr->spritesheetIndex = swap.halfword;    
    
    ptr->unk_2 = 0;
    
    swap.byte[1] = data[2].halfword >> 8;
    swap.byte[0] = data[2].halfword;
    
    ptr->anchorX = swap.halfword;

    swap.byte[1] = data[3].halfword >> 8;
    swap.byte[0] = data[3].halfword;
    
    ptr->anchorY = swap.halfword;
    
}

// alternate
/*
inline void setBitmapMetadata(BitmapMetadata* ptr, u16* data) {
    
    ptr->spritesheetIndex = swap16Alt(data[0]);    
    ptr->unk_2 = 0;
    ptr->anchorX = swap16Alt(data[2]);
    ptr->anchorY = swap16(data[3]);

}
*/

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
inline AnimationFrameMetadata* getAnimationFrameMetadataPtrFromFrame(u16 currentFrame, u16* animationFrameMetadataPtr) {
    
    u16 i;
    AnimationFrameMetadata animationFrameMetadata;
    
    i = 0;
        
    while (i < currentFrame) {

        setAnimationFrameMetadata(&animationFrameMetadata, animationFrameMetadataPtr);

        // skip header for next iteration
        animationFrameMetadataPtr += 2;
        // * sizeof(animationFrameMetadata); frame data consists of animationFrameMetadata + varying lengths of bitmapMetadata objects
        animationFrameMetadataPtr += animationFrameMetadata.objectCount * 4;
        
        i++;
        
    } 
    
    return (AnimationFrameMetadata*)animationFrameMetadataPtr;

}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", advanceBitmapMetadataPtr);

inline u16* advanceBitmapMetadataPtr(u16 numFrames, u16* bitmapMetadataPtr) {
    
    u16 i;
    
    for (i = 0; i < numFrames; i++) {
        bitmapMetadataPtr += 4;
    }

    return bitmapMetadataPtr;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", setBitmapFromSpriteObject);

void setBitmapFromSpriteObject(u16 spriteIndex, AnimationFrameMetadata* animationFrameMetadataPtr, u8 animationType) {
    
    BitmapMetadata bitmapMetadata; 
    
    u32 length;
    u16 bitmapIndex;
    
    u32 texturePtr;
    u16 *palettePtr;
    
    f32 viewSpacePositionX;
    f32 viewSpacePositionY;
    f32 viewSpacePositionZ;
    
    u16 objectCount;
    
    u16 i = 0;
    
    objectCount = animationFrameMetadataPtr->objectCount;
    
    length = 0;
    
    // virtual address; destination of DMA
    texturePtr = globalSprites[spriteIndex].texturePtr[gGraphicsBufferIndex];

    if (objectCount) {
        
        do {
            
            if (animationType) {

                setBitmapMetadata(&bitmapMetadata, advanceBitmapMetadataPtr((objectCount - i) - 1, globalSprites[spriteIndex].bitmapMetadataPtr));
                
                texturePtr += length;
            
                if (animationType == 2) {
                    setSpriteDMAInfo(bitmapMetadata.spritesheetIndex, globalSprites[spriteIndex].spritesheetIndexPtr, texturePtr, globalSprites[spriteIndex].romTexturePtr);
                }
            
                length = getTextureLength(bitmapMetadata.spritesheetIndex, globalSprites[spriteIndex].spritesheetIndexPtr);
        
                
            } else {
                
                setBitmapMetadata(&bitmapMetadata, advanceBitmapMetadataPtr(((animationFrameMetadataPtr->objectCount - i) - 1), globalSprites[spriteIndex].bitmapMetadataPtr));
                texturePtr = getTexturePtr(bitmapMetadata.spritesheetIndex, globalSprites[spriteIndex].spritesheetIndexPtr);
                
            }

            if (globalSprites[spriteIndex].stateFlags & SPRITE_DIRECT_PALETTE_MODE) {
                palettePtr = getPalettePtrType1(globalSprites[spriteIndex].paletteIndex, globalSprites[spriteIndex].paletteIndexPtr);
            } else {
                palettePtr = getPalettePtrType2(bitmapMetadata.spritesheetIndex, globalSprites[spriteIndex].paletteIndexPtr, globalSprites[spriteIndex].spriteToPaletteMappingPtr);
            }

            viewSpacePositionX = globalSprites[spriteIndex].viewSpacePosition.x;
            viewSpacePositionY = globalSprites[spriteIndex].viewSpacePosition.y;
            viewSpacePositionZ = globalSprites[spriteIndex].viewSpacePosition.z;

            if (globalSprites[spriteIndex].stateFlags & SPRITE_NO_TRANSFORM) {
                bitmapIndex = setBitmap(texturePtr, palettePtr, (0x8 | SCENE_NODE_UPDATE_SCALE | SCENE_NODE_UPDATE_ROTATION | SCENE_NODE_TRANSFORM_EXEMPT));
            } else {
                bitmapIndex = setBitmap(texturePtr, palettePtr, (0x8 | SCENE_NODE_UPDATE_SCALE | SCENE_NODE_UPDATE_ROTATION));
            }

            setBitmapViewSpacePosition(bitmapIndex, viewSpacePositionX, viewSpacePositionY, viewSpacePositionZ);
            setBitmapScale(bitmapIndex, globalSprites[spriteIndex].scale.x, globalSprites[spriteIndex].scale.y, globalSprites[spriteIndex].scale.z);
            setBitmapRotation(bitmapIndex, globalSprites[spriteIndex].rotation.x, globalSprites[spriteIndex].rotation.y, globalSprites[spriteIndex].rotation.z);
            setBitmapRGBA(bitmapIndex, globalSprites[spriteIndex].currentRGBA.r, globalSprites[spriteIndex].currentRGBA.g, globalSprites[spriteIndex].currentRGBA.b, globalSprites[spriteIndex].currentRGBA.a);
            setBitmapAnchor(bitmapIndex, bitmapMetadata.anchorX, bitmapMetadata.anchorY);
            
            setBitmapFlip(bitmapIndex, globalSprites[spriteIndex].renderingFlags & SPRITE_RENDERING_FLIP_HORIZONTAL, globalSprites[spriteIndex].renderingFlags & SPRITE_RENDERING_FLIP_VERTICAL);
            setBitmapAnchorAlignment(bitmapIndex, (globalSprites[spriteIndex].renderingFlags >> 3) & 3, (globalSprites[spriteIndex].renderingFlags >> 5) & 3);
            setBitmapAxisMapping(bitmapIndex, (globalSprites[spriteIndex].renderingFlags >> 7) & 3);
            setBitmapTriangleWinding(bitmapIndex, globalSprites[spriteIndex].renderingFlags & SPRITE_RENDERING_REVERSE_WINDING);
            setBitmapBlendMode(bitmapIndex, (globalSprites[spriteIndex].renderingFlags >> 10) & 7);

            if (globalSprites[spriteIndex].stateFlags & SPRITE_ENABLE_BILINEAR_FILTERING) {
                bitmaps[bitmapIndex].flags |= BITMAP_USE_BILINEAR_FILTERING;
            } else {
                bitmaps[bitmapIndex].flags &= ~BITMAP_USE_BILINEAR_FILTERING;
            }

            objectCount = animationFrameMetadataPtr->objectCount;
            i++;
            
        } while (i < objectCount);
        
    }
    
}

static inline u8 updateSpriteCurrentRGBA(u16 i) {

    u8 count = 0;
    f32 tempf;

    if (globalSprites[i].currentRGBA.r < globalSprites[i].targetRGBA.r) {
        
        globalSprites[i].currentRGBA.r += globalSprites[i].deltaRGBA.r;
        
        if (globalSprites[i].targetRGBA.r <= globalSprites[i].currentRGBA.r) {
            globalSprites[i].currentRGBA.r = globalSprites[i].targetRGBA.r;
        } else {
            count = 1;
        }
    }

    if (globalSprites[i].currentRGBA.r > globalSprites[i].targetRGBA.r) {
        
        globalSprites[i].currentRGBA.r -= globalSprites[i].deltaRGBA.r;
        
        if (globalSprites[i].targetRGBA.r >= globalSprites[i].currentRGBA.r) {
            globalSprites[i].currentRGBA.r = globalSprites[i].targetRGBA.r;
        } else {
            count++;
        }
    }

    if (globalSprites[i].currentRGBA.g <  globalSprites[i].targetRGBA.g) {
        
        globalSprites[i].currentRGBA.g +=  globalSprites[i].deltaRGBA.g;
        
        if ((globalSprites[i].targetRGBA.g <= globalSprites[i].currentRGBA.g)) {
            globalSprites[i].currentRGBA.g = globalSprites[i].targetRGBA.g;
        } else {
            count++;
        }
    }

    if (globalSprites[i].currentRGBA.g >  globalSprites[i].targetRGBA.g) {
        
        globalSprites[i].currentRGBA.g -= globalSprites[i].deltaRGBA.g;
        
        if (globalSprites[i].targetRGBA.g >= globalSprites[i].currentRGBA.g) {
            globalSprites[i].currentRGBA.g = globalSprites[i].targetRGBA.g;
        } else {
            count++;
        }
    }

    if (globalSprites[i].currentRGBA.b <  globalSprites[i].targetRGBA.b) {
        
        globalSprites[i].currentRGBA.b += globalSprites[i].deltaRGBA.b;
        
        if ((globalSprites[i].targetRGBA.b <= globalSprites[i].currentRGBA.b)) {
            globalSprites[i].currentRGBA.b = globalSprites[i].targetRGBA.b;
        } else {
            count++;
        }
    }

    if (globalSprites[i].currentRGBA.b >  globalSprites[i].targetRGBA.b) {
        
        globalSprites[i].currentRGBA.b -= globalSprites[i].deltaRGBA.b;
        
        if (globalSprites[i].targetRGBA.b >= globalSprites[i].currentRGBA.b) {
            globalSprites[i].currentRGBA.b = globalSprites[i].targetRGBA.b;
        } else {
            count++;
        }
    }

    if (globalSprites[i].currentRGBA.a <  globalSprites[i].targetRGBA.a) {
        
        globalSprites[i].currentRGBA.a += globalSprites[i].deltaRGBA.a;
        
        if ((globalSprites[i].targetRGBA.a <= globalSprites[i].currentRGBA.a)) {
            globalSprites[i].currentRGBA.a = globalSprites[i].targetRGBA.a;
        } else {
            count++;
        }
    }

    if (globalSprites[i].currentRGBA.a >  globalSprites[i].targetRGBA.a) {
        
        globalSprites[i].currentRGBA.a -= globalSprites[i].deltaRGBA.a;
        
        if (globalSprites[i].targetRGBA.a >= globalSprites[i].currentRGBA.a) {
            globalSprites[i].currentRGBA.a = globalSprites[i].targetRGBA.a;
        } else {
            count++;
        }
    }

    return count;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/globalSprites", updateSprites);

void updateSprites(void) {

    AnimationFrameMetadata animationFrameMetadata;
    u32 pad[4]; 
    
    u16 i = 0;
    u16* ptr;
    u32 temp;

    u16 j;
        
    while (i < MAX_SPRITES) {

        if (globalSprites[i].stateFlags & SPRITE_ACTIVE) {
        
            if (updateSpriteCurrentRGBA(i) == 0) {
                globalSprites[i].stateFlags |= SPRITE_RGBA_IN_PROGRESS;
            }

            globalSprites[i].audioTrigger = FALSE;

            if ((globalSprites[i].stateFlags & SPRITE_ANIMATION_HEADER_PROCESSED)) {
            
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
    
                if (!(globalSprites[i].stateFlags & SPRITE_ANIMATION_PAUSED)) {
                    globalSprites[i].frameTickCounter += 2; 
                }
    
                if (globalSprites[i].frameTickCounter >= animationFrameMetadata.frameDuration) {
    
                    globalSprites[i].audioTrigger = animationFrameMetadata.audioTrigger;
                    globalSprites[i].frameTickCounter = 0;
    
                    if (globalSprites[i].stateFlags & SPRITE_ANIMATION_PLAYING) {
                     
                        globalSprites[i].currentAnimationFrame++;
             
                        if (!(globalSprites[i].currentAnimationFrame < globalSprites[i].animationFrameMetadata.objectCount)) {
    
                            if (globalSprites[i].stateFlags & SPRITE_ANIMATION_LOOPS) {
                                
                                globalSprites[i].currentAnimationFrame = 0;
                                
                            } else if (globalSprites[i].stateFlags & SPRITE_DESTROY_ON_ANIMATION_END) {
                            
                                deactivateSprite(i);
                                
                            } else {
                                globalSprites[i].currentAnimationFrame = globalSprites[i].animationFrameMetadata.objectCount - 1;
                            }
    
                            globalSprites[i].stateFlags |= SPRITE_ANIMATION_STATE_CHANGED;
                            
                        } 

                        // globalSprites[i].animationDataPtr + 4 = skip header + 4 byte total metadata object count; start at first metadata object
                        ptr = getAnimationFrameMetadataPtrFromFrame(globalSprites[i].currentAnimationFrame, globalSprites[i].animationDataPtr + 4);
                        
                        globalSprites[i].animationFrameMetadataPtr = ptr;
                        globalSprites[i].bitmapMetadataPtr = ptr + 2;
                        
                    } else {
                        globalSprites[i].stateFlags |= SPRITE_ANIMATION_STATE_CHANGED;
                    }
                    
                }
    
            }   
        } 
    
        i++;
        
    } 
    
}