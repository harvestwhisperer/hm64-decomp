#include "common.h"

#include "system/numberSprites.h"

#include "system/globalSprites.h"
#include "system/sprite.h"

// bss
NumberSprites numberSprites[MAX_NUMBER_SPRITES];

static u32 powersOfTen[] = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000 };

// forward declaration
bool renderNumberSpriteDisplayDigits(u16 spriteIndex);
bool renderNumberSpriteDisplayDigit(u16 spriteIndex, u8 arg1, u8 arg2);

void initializeNumberSprites(void) {

    u16 i;

    for (i = 0; i < MAX_NUMBER_SPRITES; i++) {
        NumberSprites *ns = &numberSprites[i];
        ns->spriteIndex = 0;
        ns->flags = 0;
    }

}

bool setNumberSprites(u16 index, u16 spriteIndex, u32 romSpritesheetStart, u32 romSpritesheetEnd, u32 romAssetsIndexStart, u32 romAssetsIndexEnd, 
    u8 *vaddrTexture, u16 *vaddrPalette, AnimationFrameMetadata *vaddrAnimationFrameMetadata, u32 *spriteToPaletteVaddr, 
    u32 *spritesheetIndexVaddr, u16 animationIndex, u8 animationBaseFrame, f32 x, f32 y, f32 z, u8 digitSpacing) {

    NumberSprites *ns = &numberSprites[index];
    
    bool result = FALSE;
                    
    if (index < MAX_NUMBER_SPRITES) {

        if (!(ns->flags & NUMBER_SPRITES_FLAG_ACTIVE)) {
        
            result = TRUE;
        
            ns->romSpritesheetStart = romSpritesheetStart;
            ns->romSpritesheetEnd = romSpritesheetEnd;
            ns->romAssetsIndexStart = romAssetsIndexStart;
            ns->romAssetsIndexEnd = romAssetsIndexEnd;

            ns->vaddrTexture = vaddrTexture;
            ns->vaddrPalette = vaddrPalette;
            ns->vaddrAnimationFrameMetadata = vaddrAnimationFrameMetadata;
            ns->spriteToPaletteVaddr = spriteToPaletteVaddr;
            ns->spritesheetIndexVaddr = spritesheetIndexVaddr;
            
            ns->animationIndex = animationIndex;
            ns->animationBaseFrame = animationBaseFrame;
            
            ns->coordinates.x = x;
            ns->coordinates.y = y;
            ns->coordinates.z = z;
            
            ns->digitSpacing = digitSpacing;
            
            ns->spriteIndex = spriteIndex;
            ns->flags = NUMBER_SPRITES_FLAG_ACTIVE;

        }
    
    }
    
    return result;

}

bool dmaNumberSprites(u16 index, u32 value, u8 arg2, u16 flag) {

    NumberSprites *ns = &numberSprites[index];
    
    bool result = FALSE;

    u8 digitIndex;
    u8 remainingDigit;
    
    if (index < MAX_NUMBER_SPRITES) {
        
        if (ns->flags & NUMBER_SPRITES_FLAG_ACTIVE) {
            
            ns->maxDigitIndex = arg2;
            ns->value = value;

            digitIndex = ns->maxDigitIndex;
            
            ns->flags |= NUMBER_SPRITES_FLAG_RENDER_READY;
            
            do {
                
                dmaSprite(ns->spriteIndex + digitIndex, 
                    ns->romSpritesheetStart, 
                    ns->romSpritesheetEnd, 
                    ns->romAssetsIndexStart, 
                    ns->romAssetsIndexEnd, 
                    NULL, 
                    NULL, 
                    ns->vaddrTexture, 
                    NULL, 
                    ns->vaddrPalette, 
                    ns->vaddrAnimationFrameMetadata, 
                    ns->spriteToPaletteVaddr, 
                    ns->spritesheetIndexVaddr, 
                    0, 
                    FALSE);
                
                remainingDigit = digitIndex--;
                
            } while (remainingDigit);
            
            setNumberSpritesRGBA(index, 255, 255, 255, 255);
            
            result = TRUE;
            
            if (flag == 3) {
                ns->flags |= NUMBER_SPRITES_FLAG_BLEND_DECAL;
            }

        }

    }

    return result;
}

bool deactivateNumberSprites(u16 index) {

    NumberSprites *ns = &numberSprites[index];
    
    bool result = FALSE;
    
    u8 i;
    u16 check;

    if (index < MAX_NUMBER_SPRITES) {
        
        if (ns->flags & NUMBER_SPRITES_FLAG_ACTIVE) {
            
            i = ns->maxDigitIndex;

            do {
                deactivateSprite(ns->spriteIndex+i);
                check = i--;
            } while (check);
        
            ns->flags = 0;
        
            result = TRUE;
        }
        
    }
    
    return result;
}

bool setNumberSpritesRGBA(u16 spriteIndex, u8 r, u8 g, u8 b, u8 a) {

    NumberSprites *ns = &numberSprites[spriteIndex];

    u8 digitIndex = 0;
    bool result = FALSE;

    if (spriteIndex < MAX_NUMBER_SPRITES) {

        if (ns->flags & NUMBER_SPRITES_FLAG_ACTIVE) {

            digitIndex = ns->maxDigitIndex;

            do {
                setSpriteColor(ns->spriteIndex + digitIndex, r, g, b, a);
            } while (digitIndex--);

            result = TRUE;

        }

    }
    
    return result;
    
}

bool setNumberSpritesAlpha(u16 spriteIndex, u8 a) {

    NumberSprites *ns = &numberSprites[spriteIndex];

    u8 digitIndex = 0;
    bool result = FALSE;

    if (spriteIndex < MAX_NUMBER_SPRITES) {
        
        if (ns->flags & NUMBER_SPRITES_FLAG_ACTIVE) {

            digitIndex = ns->maxDigitIndex;

            do {
                setSpriteAlpha(ns->spriteIndex + digitIndex, a);
            } while (digitIndex--);

            result = TRUE;

        }
    
    }
    
    return result;

}

bool updateNumberSpriteRGBA(u16 spriteIndex, u8 r, u8 g, u8 b, u8 a, u16 rate) {

    NumberSprites *ns = &numberSprites[spriteIndex];

    u8 digitIndex = 0;
    bool result = FALSE;

    if (spriteIndex < MAX_NUMBER_SPRITES) {

        if (ns->flags & NUMBER_SPRITES_FLAG_ACTIVE) {

            digitIndex = ns->maxDigitIndex;

            do {
                updateSpriteRGBA(ns->spriteIndex + digitIndex, r, g, b, a, rate);
            } while (digitIndex--);

            result = TRUE;

        }
    
    }
    
    return result;

}

bool updateNumberSpriteAlphaWithTransition(u16 spriteIndex, u8 a, s16 rate) {

    NumberSprites *ns = &numberSprites[spriteIndex];

    u8 digitIndex = 0;
    bool result = FALSE;

    if (spriteIndex < MAX_NUMBER_SPRITES) {

        if (ns->flags & NUMBER_SPRITES_FLAG_ACTIVE) {

            digitIndex = ns->maxDigitIndex;

            do {
                updateSpriteAlpha(ns->spriteIndex + digitIndex, a, rate);
            } while (digitIndex--);

            result = TRUE;

        }

    }
    
    return result;
}

bool renderNumberSpriteDisplayDigits(u16 spriteIndex) {

    NumberSprites *ns = &numberSprites[spriteIndex];

    u8 digitIndex = 0;
    u8 foundNonZero = FALSE;

    bool result = FALSE;

    u32 remainingValue;
    u32 digitValue;

    if (spriteIndex < MAX_NUMBER_SPRITES) {

        if (ns->flags & NUMBER_SPRITES_FLAG_ACTIVE) {
            
            remainingValue = ns->value;
            digitIndex = ns->maxDigitIndex;
            
            do {

                resetAnimationState(ns->spriteIndex + digitIndex);

                digitValue = remainingValue / powersOfTen[digitIndex];

                remainingValue %= powersOfTen[digitIndex];

                if (digitValue) {
                    renderNumberSpriteDisplayDigit(spriteIndex, digitValue, digitIndex);
                    foundNonZero = TRUE;
                } else if (!foundNonZero) {
                    if (digitIndex == 0) {
                        renderNumberSpriteDisplayDigit(spriteIndex, 0, digitIndex);
                    }
                } else {
                    renderNumberSpriteDisplayDigit(spriteIndex, 0, digitIndex);
                }
                
            } while (digitIndex--);

            result = TRUE;

        }

    }

    return result;
    
}

bool renderNumberSpriteDisplayDigit(u16 spriteIndex, u8 digitValue, u8 digitIndex) {

    NumberSprites *ns = &numberSprites[spriteIndex];

    bool result = FALSE;
    u16 tempIndex;
    
    if (spriteIndex < MAX_NUMBER_SPRITES) {

        if (ns->flags & NUMBER_SPRITES_FLAG_ACTIVE) {

            tempIndex = ns->spriteIndex + digitIndex; 
            
            setSpriteViewSpacePosition(tempIndex, ns->coordinates.x - ns->digitSpacing * digitIndex, ns->coordinates.y, ns->coordinates.z);
            setSpriteScale(tempIndex, 1.0f, 1.0f, 1.0f);
            setSpriteRotation(tempIndex, 0, 0, 0);
            
            setSpriteAnchorAlignment(tempIndex, SPRITE_ANCHOR_CENTER, SPRITE_ANCHOR_CENTER);;

            if (ns->flags & NUMBER_SPRITES_FLAG_BLEND_DECAL) {
                setSpriteBlendMode(tempIndex, SPRITE_BLEND_ALPHA_DECAL);
            } else {
                setSpriteBlendMode(tempIndex, SPRITE_BLEND_ALPHA_MODULATED);
            }

            startSpriteAnimation(tempIndex, ns->animationIndex, ns->animationBaseFrame + digitValue);
            
            result = TRUE;
        }

    }

    return result;

} 

void updateNumberSprites(void) {

    u16 i;

    for (i = 0; i < MAX_NUMBER_SPRITES; i++) {
        if ((numberSprites[i].flags & NUMBER_SPRITES_FLAG_ACTIVE) && (numberSprites[i].flags & NUMBER_SPRITES_FLAG_RENDER_READY)) {
            renderNumberSpriteDisplayDigits(i);
        }
    }
    
}