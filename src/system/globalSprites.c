#include "common.h"
#include "yay0.h"

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

void initializeGlobalSprites(void) {

    u16 i;

    for (i = 0; i < MAX_SPRITES; i++) {
        SpriteObject *s = &globalSprites[i];

        s->stateFlags = 0;
        s->renderingFlags = 0;

        s->paletteIndex = 0;
        s->currentAnimationFrame = 0;
        s->audioTrigger = FALSE;
        s->frameTickCounter = 0;

        s->viewSpacePosition.x = 0;
        s->viewSpacePosition.y = 0;
        s->viewSpacePosition.z = 0;

        s->scale.x = 1.0f;
        s->scale.y = 1.0f;
        s->scale.z = 1.0f;

        s->rotation.x = 0;
        s->rotation.y = 0;
        s->rotation.z = 0;

        s->currentRGBA.r = 0;
        s->currentRGBA.g = 0;
        s->currentRGBA.b = 0;
        s->currentRGBA.a = 0;

        s->targetRGBA.r = 0;
        s->targetRGBA.g = 0;
        s->targetRGBA.b = 0;
        s->targetRGBA.a = 0;

    }
}

bool dmaSprite(u16 index, 
    u32 romTextureStart, u32 romTextureEnd, 
    u32 romAssetsIndexStart, u32 romAssetsIndexEnd, 
    u32 romSpritesheetIndexStart, u32 romSpritesheetIndexEnd, 
    u8* texture1Vaddr, u8* texture2Vaddr, 
    u16* paletteVaddr, AnimationFrameMetadata* animationVaddr, 
    u8* spriteToPaletteVaddr, u32* spritesheetIndexVaddr, 
    u8 assetType, bool transformExempt) {

    SpriteObject *s = &globalSprites[index];

    u32 assetIndex[8];
    bool result = FALSE;
    
    u32 offset1;
    u32 offset2;
    u32 offset3;
    u32 offset4;
    u32 offset5;

    if (index < MAX_SPRITES) {
        
        if (!(s->stateFlags & SPRITE_ACTIVE)) { 
            
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

                dmaReadRom(romTextureStart + offset1, texture1Vaddr, offset2 - offset1);
                nuPiReadRom(romTextureStart + offset2, paletteVaddr, offset3 - offset2);
                nuPiReadRom(romTextureStart + offset3, animationVaddr, offset4 - offset3);
                nuPiReadRom(romTextureStart + offset4, spriteToPaletteVaddr, offset5 - offset4);
                
                setSpriteType2(index, animationVaddr, texture1Vaddr, paletteVaddr, spriteToPaletteVaddr);

            }

            if (!transformExempt) {
                s->stateFlags |= SPRITE_NO_TRANSFORM;
            }

            s->stateFlags |= SPRITE_ANIMATION_CYCLE_ENDED;

            result = TRUE;

        }
    
    }

    return result;

}

bool setSpriteType2(u16 index, u32* animationIndexPtr, u32* spritesheetIndexPtr, u32* paletteIndexPtr, u8* spriteToPaletteMappingPtr) {

    SpriteObject *s = &globalSprites[index];

    bool result = FALSE;

    if (index < MAX_SPRITES) {

        if (!(s->stateFlags & SPRITE_ACTIVE)) {

            s->animationIndexPtr = animationIndexPtr;
            s->spritesheetIndexPtr = spritesheetIndexPtr;
            s->paletteIndexPtr = paletteIndexPtr;
            s->spriteToPaletteMappingPtr = spriteToPaletteMappingPtr;
            s->texturePtr[0] = NULL;
            s->texturePtr[1] = NULL;
            s->romTexturePtr = NULL;

            s->stateFlags = SPRITE_ACTIVE;
            s->renderingFlags = 0;

            s->paletteIndex = 0;
            s->audioTrigger = FALSE;

            setSpriteViewSpacePosition(index, 0.0f, 0.0f, 0.0f);
            setSpriteScale(index, 1.0f, 1.0f, 1.0f);
            setSpriteRotation(index, 0.0f, 0.0f, 0.0f);
            setSpriteBaseRGBA(index, 255, 255, 255, 255);
            setSpriteAnchorAlignment(index, SPRITE_ANCHOR_CENTER, SPRITE_ANCHOR_CENTER);;
            setSpriteAxisMapping(index, SPRITE_BILLBOARD_XY);
            setSpriteBlendMode(index, SPRITE_BLEND_ALPHA_DECAL);
            
            result = TRUE;
            
        }
    }
 
    return result;

}

bool setSpriteType1(u16 spriteIndex, u32* animationIndexPtr, u32* spritesheetIndexPtr, u32* paletteIndexPtr, u8* spriteToPaletteMappingPtr, u32 romTexturePtr, u8* texturePtr, u8* texture2Ptr) {

    SpriteObject *s = &globalSprites[spriteIndex];
    
    bool result = FALSE;

    if (spriteIndex < MAX_SPRITES) {

        if (!(s->stateFlags & SPRITE_ACTIVE)) {

            s->animationIndexPtr = animationIndexPtr;
            s->spritesheetIndexPtr = spritesheetIndexPtr;
            s->paletteIndexPtr = paletteIndexPtr;
            s->spriteToPaletteMappingPtr = spriteToPaletteMappingPtr;
            s->texturePtr[0] = texturePtr;
            s->texturePtr[1] = texture2Ptr;
            s->romTexturePtr = romTexturePtr;

            s->stateFlags = (SPRITE_ACTIVE | 4);
            s->renderingFlags = 0;

            s->paletteIndex = 0;
            s->audioTrigger = FALSE;

            setSpriteViewSpacePosition(spriteIndex, 0, 0, 0);
            setSpriteScale(spriteIndex, 1.0f, 1.0f, 1.0f);
            setSpriteRotation(spriteIndex, 0, 0, 0);
            setSpriteBaseRGBA(spriteIndex, 255, 255, 255, 255);
            setSpriteAnchorAlignment(spriteIndex, SPRITE_ANCHOR_CENTER, SPRITE_ANCHOR_CENTER);;
            setSpriteAxisMapping(spriteIndex, SPRITE_BILLBOARD_XY);
            setSpriteBlendMode(spriteIndex, SPRITE_BLEND_ALPHA_DECAL);
            
            result = TRUE;
            
        }
    }
 
    return result;
    
}

inline bool deactivateSprite(u16 spriteIndex) {

    SpriteObject *s = &globalSprites[spriteIndex];

    bool result = FALSE;

    if (spriteIndex < MAX_SPRITES) {
        if (s->stateFlags & SPRITE_ACTIVE) {
            s->stateFlags &= ~(SPRITE_ACTIVE | SPRITE_ANIMATION_HEADER_PROCESSED);
            result = TRUE;
        }        
    }

    return result; 

}

void deactivateGlobalSprites(void) {

    u16 i;

    for (i = 0; i < MAX_SPRITES; i++) {
        SpriteObject *s = &globalSprites[i];

        if ((s->stateFlags & SPRITE_ACTIVE) && (s->stateFlags & SPRITE_ANIMATION_HEADER_PROCESSED)) {
            s->stateFlags &= ~(SPRITE_ACTIVE | 2);
        }

        s->stateFlags = 0;

    }
    
}

// unused or inline
// bool func_8002B7A0(u16 spriteIndex, u8 animationModeOrFrameIndex) {

//     bool result;

//     result = setupSpriteAnimation(spriteIndex, animationModeOrFrameIndex, globalSprites[spriteIndex].animationIndexPtr);

//     globalSprites[spriteIndex].animationCounter[0] = 0xFF;
//     globalSprites[spriteIndex].animationCounter[1] = 0xFF;

//     return result;
    
// }

bool startSpriteAnimation(u16 spriteIndex, u16 animationIndex, u8 animationModeOrFrameIndex) {

    SpriteObject *s = &globalSprites[spriteIndex];

    bool result = FALSE;
    
    if (spriteIndex < MAX_SPRITES) {

        if (s->stateFlags & SPRITE_ACTIVE) {
            
            if (s->animationIndexPtr[animationIndex] != s->animationIndexPtr[animationIndex+1]) {

                result = setupSpriteAnimation(spriteIndex, animationModeOrFrameIndex, getAnimationFrameMetadataPtr(animationIndex, s->animationIndexPtr));
                
                s->animationCounter[0] = 0xFF;
                s->animationCounter[1] = 0xFF;
           
            }
        }
    }

    return result;
    
}

bool setupSpriteAnimation(u16 spriteIndex, u8 animationModeOrFrameIndex, u16* animationDataPtr) {

    SpriteObject *s = &globalSprites[spriteIndex];

    bool result = FALSE;
    u16 *metadataPtr;
    
    if ((spriteIndex < MAX_SPRITES) && (s->stateFlags & SPRITE_ACTIVE) && !(s->stateFlags & SPRITE_ANIMATION_HEADER_PROCESSED)) {

        s->animationDataPtr = animationDataPtr;

        s->frameTickCounter = 0;
        s->audioTrigger = FALSE;

        s->stateFlags &= ~SPRITE_ANIMATION_CYCLE_ENDED;
        s->stateFlags |= SPRITE_ANIMATION_HEADER_PROCESSED;

        switch (animationModeOrFrameIndex) {
            case 0xFF:
                s->currentAnimationFrame = 0;
                s->stateFlags &= ~SPRITE_ANIMATION_LOOPS;
                s->stateFlags |= SPRITE_ANIMATION_PLAYING;
                break;
            case 0xFE:
                s->currentAnimationFrame = 0;
                s->stateFlags |= SPRITE_ANIMATION_PLAYING | SPRITE_ANIMATION_LOOPS;
                break;
            case 0xFD:
                s->currentAnimationFrame = 0;
                s->stateFlags |= SPRITE_ANIMATION_PLAYING | SPRITE_DESTROY_ON_ANIMATION_END;
                break;
            default:
                // for static sprites, show a single frame
                s->currentAnimationFrame = animationModeOrFrameIndex;
                s->stateFlags &= ~(SPRITE_ANIMATION_PLAYING | SPRITE_ANIMATION_LOOPS);
                break;
        }

        // skips header and gets little endian u16 value
        setTotalAnimationFrames(&s->animationFrameMetadata, s->animationDataPtr);

        // animationDataPtr + 4 = skip header and frame count
        metadataPtr = getAnimationFrameMetadataPtrFromFrame(s->currentAnimationFrame, s->animationDataPtr + 4);
        
        s->animationFrameMetadataPtr = metadataPtr;
        s->bitmapMetadataPtr = metadataPtr + 2;

        result = TRUE;
        
    }

    return result;
    
}

bool resetAnimationState(u16 index) {

    SpriteObject *s = &globalSprites[index];

    bool result = FALSE;

    if (index < MAX_SPRITES) {

        if (s->stateFlags & SPRITE_ACTIVE) {
            s->stateFlags &= ~SPRITE_ANIMATION_HEADER_PROCESSED;
            result = TRUE;
        }
    }
    
    return result;

}

bool resumeSpriteAnimation(u16 index) {

    SpriteObject *s = &globalSprites[index];

    bool result = FALSE;

    if (index < MAX_SPRITES) {

        if (s->stateFlags & SPRITE_ACTIVE) {
            s->stateFlags &= ~SPRITE_ANIMATION_PAUSED;
            result = TRUE;
        }
    }
    
    return result;

}

bool pauseSpriteAnimation(u16 index) {

    SpriteObject *s = &globalSprites[index];

    bool result = FALSE;

    if (index < MAX_SPRITES) {

        if (s->stateFlags & SPRITE_ACTIVE) {
            s->stateFlags |= SPRITE_ANIMATION_PAUSED;
            result = TRUE;
        }
    }
    
    return result;

}

bool setSpriteFlip(u16 index, bool flipHorizontal, bool flipVertical) {

    SpriteObject *s = &globalSprites[index];

    bool result = FALSE;
    
    if (index < MAX_SPRITES) {

        if (s->stateFlags & SPRITE_ACTIVE) {

            if (flipHorizontal) {
                s->renderingFlags |= SPRITE_RENDERING_FLIP_HORIZONTAL;
            } else {
                s->renderingFlags &= ~SPRITE_RENDERING_FLIP_HORIZONTAL;
            }

            if (flipVertical) {
                s->renderingFlags |= SPRITE_RENDERING_FLIP_VERTICAL;
            } else {
                s->renderingFlags &= ~SPRITE_RENDERING_FLIP_VERTICAL;
            }
            
            result = TRUE;

        }
    }

    return result;
    
}

u16 getSpriteAnimationStateChangedFlag(u16 index) {

    SpriteObject *s = &globalSprites[index];

    u16 result = FALSE;

    if (index < MAX_SPRITES) {

        if (s->stateFlags & SPRITE_ACTIVE) {
            result = s->stateFlags & SPRITE_ANIMATION_CYCLE_ENDED;
        }

    }
    
    return result;

}

bool setSpriteViewSpacePosition(u16 index, f32 x, f32 y, f32 z) {

    SpriteObject *s = &globalSprites[index];

    bool result = FALSE;

    if (index < MAX_SPRITES) {

        if (s->stateFlags & SPRITE_ACTIVE) {

            s->viewSpacePosition.x = x;
            s->viewSpacePosition.y = y;
            s->viewSpacePosition.z = z;

            result = TRUE;

        }
    }

    return result;

}

bool setSpriteScale(u16 index, f32 x, f32 y, f32 z) {

    SpriteObject *s = &globalSprites[index];

    bool result = FALSE;

    if (index < MAX_SPRITES) {

        if (s->stateFlags & SPRITE_ACTIVE) {

            s->scale.x = x;
            s->scale.y = y;
            s->scale.z = z;
            
            result = TRUE;

        }
    }
    
    return result;
    
}

bool setSpriteRotation(u16 index, f32 x, f32 y, f32 z) {

    SpriteObject *s = &globalSprites[index];

    bool result = FALSE;

    if (index < MAX_SPRITES) {

        if (s->stateFlags & SPRITE_ACTIVE) {

            s->rotation.x = x;
            s->rotation.y = y;
            s->rotation.z = z;

            result = TRUE;

        }
    }
    
    return result;
    
}

bool adjustSpriteViewSpacePosition(u16 index, f32 x, f32 y, f32 z) {

    SpriteObject *s = &globalSprites[index];

    bool result = FALSE;
    
    if (index < MAX_SPRITES) {
        
        if (s->stateFlags & SPRITE_ACTIVE) {

            s->viewSpacePosition.x += x;
            s->viewSpacePosition.y += y;
            s->viewSpacePosition.z += z;
                
            result = TRUE;
            
        }   
    }
    
    return result;

}

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

// update sprite rgba
// fade out on screen transitions
bool updateSpriteRGBA(u16 index, u8 r, u8 g, u8 b, u8 a, s16 rate) {

    SpriteObject *s = &globalSprites[index];

    bool result;
    
    s16 absValueRate = getAbsoluteValue(rate);
    f32 tempFloat;
    
    result = 0;
    
    if (index < MAX_SPRITES) {
        
        if (s->stateFlags & SPRITE_ACTIVE) {
 
            s->targetRGBA.r = (s->baseRGBA.r * r) / 255.0f;
            s->targetRGBA.g = (s->baseRGBA.g * g) / 255.0f;
            s->targetRGBA.b = (s->baseRGBA.b * b) / 255.0f;
            s->targetRGBA.a = (s->baseRGBA.a * a) / 255.0f;
            
            s->stateFlags &= ~SPRITE_RGBA_IN_PROGRESS;

            if (s->targetRGBA.r < s->currentRGBA.r) {
                tempFloat = s->currentRGBA.r - s->targetRGBA.r;
            } else {
                tempFloat = s->targetRGBA.r - s->currentRGBA.r;
            }

            s->deltaRGBA.r = (tempFloat * absValueRate) / s->baseRGBA.r;

            if (s->targetRGBA.g < s->currentRGBA.g) {
                tempFloat = s->currentRGBA.g - s->targetRGBA.g;
            } else {
                tempFloat = s->targetRGBA.g - s->currentRGBA.g;
            }

            s->deltaRGBA.g = (tempFloat * absValueRate) / s->baseRGBA.g;

            if (s->targetRGBA.b < s->currentRGBA.b) {
                tempFloat = s->currentRGBA.b - s->targetRGBA.b;
            } else {
                tempFloat = s->targetRGBA.b - s->currentRGBA.b;
            }

            s->deltaRGBA.b = (tempFloat * absValueRate) / s->baseRGBA.b;

            if (s->targetRGBA.a < s->currentRGBA.a) {
                tempFloat = s->currentRGBA.a - s->targetRGBA.a;
            } else {
                tempFloat = s->targetRGBA.a - s->currentRGBA.a;
            }

            s->deltaRGBA.a = (tempFloat * absValueRate) / s->baseRGBA.a;
            
            result = TRUE;
            
        }
    }
    
    return result;

}

bool updateSpriteAlpha(u16 index, u8 arg1, s16 rate) {

    SpriteObject *s = &globalSprites[index];

    bool result;

    f32 tempF;
    s16 absValueRate = getAbsoluteValue(rate);

    result = FALSE;

    if (index < MAX_SPRITES) {

        if (s->stateFlags & SPRITE_ACTIVE) {

            s->targetRGBA.a = (s->baseRGBA.a * arg1) / 255.0f;

            s->stateFlags &= ~SPRITE_RGBA_IN_PROGRESS;

            if (s->targetRGBA.a < s->currentRGBA.a) {
                tempF = s->currentRGBA.a - s->targetRGBA.a;
            } else {
                tempF = s->targetRGBA.a - s->currentRGBA.a;
            }

            s->deltaRGBA.a = (tempF * absValueRate) / s->baseRGBA.a;
            
            result = TRUE;

        }
    }
    
    return result;
    
}

bool setSpriteAnchorAlignment(u16 index, u16 arg1, u16 flags) {

    SpriteObject *s = &globalSprites[index];

    bool result = FALSE;
    int temp;

    if (index < MAX_SPRITES) {

        if (s->stateFlags & SPRITE_ACTIVE) { 
            
            s->renderingFlags &= ~( 8 | 0x10 | 0x20 | 0x40);
            
            // probably a shift
            s->renderingFlags |= arg1 * 8;
            
            temp = flags << 5;
            s->renderingFlags |= temp;
            
            result = TRUE;
            
        } 
    }
    
    return result;

}

bool setSpriteAxisMapping(u16 index, u16 mode) {

    SpriteObject *s = &globalSprites[index];
    
    int temp;

    bool result = FALSE;
    
    if (index < MAX_SPRITES) {
        
        if (s->stateFlags & SPRITE_ACTIVE) {
            
            s->renderingFlags &= ~(0x80 | 0x100);
            temp = mode << 7;
            s->renderingFlags |= temp;
            
            result = TRUE;

        }
    }
    
    return result;

}

bool setSpriteTriangleWinding(u16 index, u16 arg1) {

    SpriteObject *s = &globalSprites[index];

    bool result = FALSE;
    
    if (index < MAX_SPRITES) {
        
        if (s->stateFlags & SPRITE_ACTIVE) {

            if (arg1) {
                s->renderingFlags |= SPRITE_RENDERING_REVERSE_WINDING;
            } else {
                s->renderingFlags &= ~SPRITE_RENDERING_REVERSE_WINDING;
            }

result = TRUE;

        }
    }
    
    return result;
    
}

bool setSpriteBlendMode(u16 index, u16 flags) {

    SpriteObject *s = &globalSprites[index];
    
    int temp;

    bool result = FALSE;
    
    if (index < MAX_SPRITES) {
        
        if (s->stateFlags & SPRITE_ACTIVE) {
            
            s->renderingFlags &= ~BITMAP_BLEND_MODE_MASK;
            temp = flags << 10;
            s->renderingFlags |= temp;
            
            result = TRUE;

        }
    }
    
    return result;

}

bool setSpriteBaseRGBA(u16 index, u8 r, u8 g, u8 b, u8 a) {

    SpriteObject *s = &globalSprites[index];

    bool result = FALSE;

    if (index < MAX_SPRITES) {
        
        if (s->stateFlags & SPRITE_ACTIVE) {
            
            s->baseRGBA.r = r;
            s->baseRGBA.g = g;
            s->baseRGBA.b = b;
            s->baseRGBA.a = a;
            
            result = TRUE;

        }
    }

    return result;

}

bool setSpriteColor(u16 index, u8 r, u8 g, u8 b, u8 a) {

    SpriteObject *s = &globalSprites[index];

    bool result = FALSE;
    
    if (index < MAX_SPRITES) {
        
        if (s->stateFlags & SPRITE_ACTIVE) {        

            result = TRUE;
            
            s->currentRGBA.r = (s->baseRGBA.r * r) / 255.0f;
            s->currentRGBA.g = (s->baseRGBA.g * g) / 255.0f;
            s->currentRGBA.b = (s->baseRGBA.b * b) / 255.0f;
            s->currentRGBA.a = (s->baseRGBA.a * a) / 255.0f;

            s->targetRGBA.r = (s->baseRGBA.r * r) / 255.0f;
            s->targetRGBA.g = (s->baseRGBA.g * g) / 255.0f;
            s->targetRGBA.b = (s->baseRGBA.b * b) / 255.0f;
            s->targetRGBA.a = (s->baseRGBA.a * a) / 255.0f;

        }
    }
    
    return result;
}

bool setSpriteAlpha(u16 index, u8 a) {

    SpriteObject *s = &globalSprites[index];
    
    bool result = FALSE;

    if (index < MAX_SPRITES) {

        if (s->stateFlags & SPRITE_ACTIVE) {

            s->baseRGBA.a = a;
            s->currentRGBA.a = a;
            s->targetRGBA.a = a;
            
            result = TRUE;

        }
    }
    
    return result;
    
}

bool setBilinearFiltering(u16 index, bool useBilinearFiltering) {

    SpriteObject *s = &globalSprites[index];

    bool result = FALSE;

    if (index < MAX_SPRITES) {
        
        if (s->stateFlags & SPRITE_ACTIVE) {
            if (useBilinearFiltering == TRUE) {
                s->stateFlags |= SPRITE_ENABLE_BILINEAR_FILTERING;
            } else {
                s->stateFlags &= ~SPRITE_ENABLE_BILINEAR_FILTERING;
            }

            result = TRUE;

        }
    }
    
    return result;
}

bool setSpritePaletteIndex(u16 index, u16 paletteIndex) {

    SpriteObject *s = &globalSprites[index];

    bool result = FALSE;

    if (index < MAX_SPRITES) {

        if (s->stateFlags & SPRITE_ACTIVE) {

            s->paletteIndex = paletteIndex;
            s->stateFlags |= SPRITE_DIRECT_PALETTE_MODE;

            result = TRUE;

        }
    }

    return result;

}

// seems like check fade out completed
bool checkSpriteRGBAUpdateFinished(u16 index) {

    SpriteObject *s = &globalSprites[index];

    bool result;

    result = FALSE;
    
    if (index < MAX_SPRITES) {
        if (s->stateFlags & SPRITE_ACTIVE) {
            result = (s->stateFlags >> 10) & 1;
        }
    }

    return result;
    
}

bool checkSpriteAnimationCycleEnded(u16 index) {

    SpriteObject *s = &globalSprites[index];

    bool result = FALSE;

    if (index < MAX_SPRITES) {
        // flag 0x40, SPRITE_ANIMATION_CYCLE_ENDED
        result = (s->stateFlags >> 6) & 1;
    }

    return result;
    
}

void setTotalAnimationFrames(AnimationFrameMetadata *animationFrameMetadata, Swap16 *animationAsset) {
    
    // skip 4 byte header
    animationFrameMetadata->objectCount = swap16(animationAsset[2]);
    
}

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

inline u16* advanceBitmapMetadataPtr(u16 numFrames, u16* bitmapMetadataPtr) {
    
    u16 i;
    
    for (i = 0; i < numFrames; i++) {
        bitmapMetadataPtr += 4;
    }

    return bitmapMetadataPtr;
    
}

void setBitmapFromSpriteObject(u16 spriteIndex, AnimationFrameMetadata* animationFrameMetadataPtr, u8 animationType) {

    SpriteObject *s = &globalSprites[spriteIndex];

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
    texturePtr = s->texturePtr[gGraphicsBufferIndex];

    if (objectCount) {
        
        do {
            
            if (animationType) {

                setBitmapMetadata(&bitmapMetadata, advanceBitmapMetadataPtr((objectCount - i) - 1, s->bitmapMetadataPtr));
                
                texturePtr += length;

                // Always re-DMA, even when the animation frame hasn't
                // advanced (animationType would be 1). The "skip DMA"
                // optimization required getTextureLength to return the
                // RAM stride — but for per-frame-compressed sprites,
                // getTextureLength returns the *compressed* length, which
                // differs from the decompressed stride used when frames
                // were packed. Mismatched strides give bitmap 2+ in a
                // layered frame a bogus timg → RDP reads past the buffer
                // → crash deep in display-list generation.
                length = setSpriteDMAInfo(bitmapMetadata.spritesheetIndex, s->spritesheetIndexPtr, texturePtr, s->romTexturePtr);

            } else {
                
                setBitmapMetadata(&bitmapMetadata, advanceBitmapMetadataPtr(((animationFrameMetadataPtr->objectCount - i) - 1), s->bitmapMetadataPtr));
                texturePtr = getTexturePtr(bitmapMetadata.spritesheetIndex, s->spritesheetIndexPtr);
                
            }

            if (s->stateFlags & SPRITE_DIRECT_PALETTE_MODE) {
                palettePtr = getPalettePtrType1(s->paletteIndex, s->paletteIndexPtr);
            } else {
                palettePtr = getPalettePtrType2(bitmapMetadata.spritesheetIndex, s->paletteIndexPtr, s->spriteToPaletteMappingPtr);
            }

            viewSpacePositionX = s->viewSpacePosition.x;
            viewSpacePositionY = s->viewSpacePosition.y;
            viewSpacePositionZ = s->viewSpacePosition.z;

            if (s->stateFlags & SPRITE_NO_TRANSFORM) {
                bitmapIndex = setBitmap(texturePtr, palettePtr, (0x8 | SCENE_NODE_UPDATE_SCALE | SCENE_NODE_UPDATE_ROTATION | SCENE_NODE_TRANSFORM_EXEMPT));
            } else {
                bitmapIndex = setBitmap(texturePtr, palettePtr, (0x8 | SCENE_NODE_UPDATE_SCALE | SCENE_NODE_UPDATE_ROTATION));
            }

            setBitmapViewSpacePosition(bitmapIndex, viewSpacePositionX, viewSpacePositionY, viewSpacePositionZ);
            setBitmapScale(bitmapIndex, s->scale.x, s->scale.y, s->scale.z);
            setBitmapRotation(bitmapIndex, s->rotation.x, s->rotation.y, s->rotation.z);
            setBitmapRGBA(bitmapIndex, s->currentRGBA.r, s->currentRGBA.g, s->currentRGBA.b, s->currentRGBA.a);
            setBitmapAnchor(bitmapIndex, bitmapMetadata.anchorX, bitmapMetadata.anchorY);
            
            setBitmapFlip(bitmapIndex, s->renderingFlags & SPRITE_RENDERING_FLIP_HORIZONTAL, s->renderingFlags & SPRITE_RENDERING_FLIP_VERTICAL);
            setBitmapAnchorAlignment(bitmapIndex, (s->renderingFlags >> 3) & 3, (s->renderingFlags >> 5) & 3);
            setBitmapAxisMapping(bitmapIndex, (s->renderingFlags >> 7) & 3);
            setBitmapTriangleWinding(bitmapIndex, s->renderingFlags & SPRITE_RENDERING_REVERSE_WINDING);
            setBitmapBlendMode(bitmapIndex, (s->renderingFlags >> 10) & 7);

            if (s->stateFlags & SPRITE_ENABLE_BILINEAR_FILTERING) {
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

    SpriteObject *s = &globalSprites[i];

    u8 count = 0;
    f32 tempf;

    if (s->currentRGBA.r < s->targetRGBA.r) {
        
        s->currentRGBA.r += s->deltaRGBA.r;
        
        if (s->targetRGBA.r <= s->currentRGBA.r) {
            s->currentRGBA.r = s->targetRGBA.r;
        } else {
            count = 1;
        }
    }

    if (s->currentRGBA.r > s->targetRGBA.r) {
        
        s->currentRGBA.r -= s->deltaRGBA.r;
        
        if (s->targetRGBA.r >= s->currentRGBA.r) {
            s->currentRGBA.r = s->targetRGBA.r;
        } else {
            count++;
        }
    }

    if (s->currentRGBA.g <  s->targetRGBA.g) {
        
        s->currentRGBA.g +=  s->deltaRGBA.g;
        
        if ((s->targetRGBA.g <= s->currentRGBA.g)) {
            s->currentRGBA.g = s->targetRGBA.g;
        } else {
            count++;
        }
    }

    if (s->currentRGBA.g >  s->targetRGBA.g) {
        
        s->currentRGBA.g -= s->deltaRGBA.g;
        
        if (s->targetRGBA.g >= s->currentRGBA.g) {
            s->currentRGBA.g = s->targetRGBA.g;
        } else {
            count++;
        }
    }

    if (s->currentRGBA.b <  s->targetRGBA.b) {
        
        s->currentRGBA.b += s->deltaRGBA.b;
        
        if ((s->targetRGBA.b <= s->currentRGBA.b)) {
            s->currentRGBA.b = s->targetRGBA.b;
        } else {
            count++;
        }
    }

    if (s->currentRGBA.b >  s->targetRGBA.b) {
        
        s->currentRGBA.b -= s->deltaRGBA.b;
        
        if (s->targetRGBA.b >= s->currentRGBA.b) {
            s->currentRGBA.b = s->targetRGBA.b;
        } else {
            count++;
        }
    }

    if (s->currentRGBA.a <  s->targetRGBA.a) {
        
        s->currentRGBA.a += s->deltaRGBA.a;
        
        if ((s->targetRGBA.a <= s->currentRGBA.a)) {
            s->currentRGBA.a = s->targetRGBA.a;
        } else {
            count++;
        }
    }

    if (s->currentRGBA.a >  s->targetRGBA.a) {
        
        s->currentRGBA.a -= s->deltaRGBA.a;
        
        if (s->targetRGBA.a >= s->currentRGBA.a) {
            s->currentRGBA.a = s->targetRGBA.a;
        } else {
            count++;
        }
    }

    return count;
    
}

void updateSprites(void) {

    AnimationFrameMetadata animationFrameMetadata;
    u32 pad[4]; 
    
    u16 i = 0;
    u16* ptr;
    u32 temp;

    u16 j;
        
    while (i < MAX_SPRITES) {
        SpriteObject *s = &globalSprites[i];

        if (s->stateFlags & SPRITE_ACTIVE) {

            if (updateSpriteCurrentRGBA(i) == 0) {
                s->stateFlags |= SPRITE_RGBA_IN_PROGRESS;
            }

            s->audioTrigger = FALSE;

            if ((s->stateFlags & SPRITE_ANIMATION_HEADER_PROCESSED)) {

                setAnimationFrameMetadata(&animationFrameMetadata, s->animationFrameMetadataPtr);

                if (s->stateFlags & 4) {

                    if (s->animationCounter[gGraphicsBufferIndex] != s->currentAnimationFrame) {
                        setBitmapFromSpriteObject(i, &animationFrameMetadata, 2);
                    } else {
                        setBitmapFromSpriteObject(i, &animationFrameMetadata, 1);
                    }

                    s->animationCounter[gGraphicsBufferIndex] = s->currentAnimationFrame;

                } else {
                    setBitmapFromSpriteObject(i, &animationFrameMetadata, 0);
                }

                if (!(s->stateFlags & SPRITE_ANIMATION_PAUSED)) {
                    s->frameTickCounter += 2;
                }

                if (s->frameTickCounter >= animationFrameMetadata.frameDuration) {

                    s->audioTrigger = animationFrameMetadata.audioTrigger;
                    s->frameTickCounter = 0;

                    if (s->stateFlags & SPRITE_ANIMATION_PLAYING) {

                        s->currentAnimationFrame++;

                        if (!(s->currentAnimationFrame < s->animationFrameMetadata.objectCount)) {

                            if (s->stateFlags & SPRITE_ANIMATION_LOOPS) {

                                s->currentAnimationFrame = 0;

                            } else if (s->stateFlags & SPRITE_DESTROY_ON_ANIMATION_END) {

                                deactivateSprite(i);

                            } else {
                                s->currentAnimationFrame = s->animationFrameMetadata.objectCount - 1;
                            }

                            s->stateFlags |= SPRITE_ANIMATION_CYCLE_ENDED;

                        }

                        // s->animationDataPtr + 4 = skip header + 4 byte total metadata object count; start at first metadata object
                        ptr = getAnimationFrameMetadataPtrFromFrame(s->currentAnimationFrame, s->animationDataPtr + 4);

                        s->animationFrameMetadataPtr = ptr;
                        s->bitmapMetadataPtr = ptr + 2;

                    } else {
                        s->stateFlags |= SPRITE_ANIMATION_CYCLE_ENDED;
                    }

                }

            }
        }

        i++;

    }
    
}