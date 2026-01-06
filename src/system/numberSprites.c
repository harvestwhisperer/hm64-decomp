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

//INCLUDE_ASM("asm/nonmatchings/system/numberSprites", initializeNumberSprites);

void initializeNumberSprites(void) {

    u16 i;

    for (i = 0; i < MAX_NUMBER_SPRITES; i++) {
        numberSprites[i].spriteIndex = 0;
        numberSprites[i].flags = 0;
    }

}

//INCLUDE_ASM("asm/nonmatchings/system/numberSprites", setNumberSprites);

bool setNumberSprites(u16 index, u16 spriteIndex, u32 romSpritesheetStart, u32 romSpritesheetEnd, u32 romAssetsIndexStart, u32 romAssetsIndexEnd, 
    u8 *vaddrTexture, u16 *vaddrPalette, AnimationFrameMetadata *vaddrAnimationFrameMetadata, u32 *spriteToPaletteVaddr, 
    u32 *spritesheetIndexVaddr, u16 animationIndex, u8 animationBaseFrame, f32 x, f32 y, f32 z, u8 digitSpacing) {
    
    bool result = FALSE;
                    
    if (index < MAX_NUMBER_SPRITES) {

        if (!(numberSprites[index].flags & NUMBER_SPRITES_FLAG_ACTIVE)) {
        
            result = TRUE;
        
            numberSprites[index].romSpritesheetStart = romSpritesheetStart;
            numberSprites[index].romSpritesheetEnd = romSpritesheetEnd;
            numberSprites[index].romAssetsIndexStart = romAssetsIndexStart;
            numberSprites[index].romAssetsIndexEnd = romAssetsIndexEnd;

            numberSprites[index].vaddrTexture = vaddrTexture;
            numberSprites[index].vaddrPalette = vaddrPalette;
            numberSprites[index].vaddrAnimationFrameMetadata = vaddrAnimationFrameMetadata;
            numberSprites[index].spriteToPaletteVaddr = spriteToPaletteVaddr;
            numberSprites[index].spritesheetIndexVaddr = spritesheetIndexVaddr;
            
            numberSprites[index].animationIndex = animationIndex;
            numberSprites[index].animationBaseFrame = animationBaseFrame;
            
            numberSprites[index].coordinates.x = x;
            numberSprites[index].coordinates.y = y;
            numberSprites[index].coordinates.z = z;
            
            numberSprites[index].digitSpacing = digitSpacing;
            
            numberSprites[index].spriteIndex = spriteIndex;
            numberSprites[index].flags = NUMBER_SPRITES_FLAG_ACTIVE;

        }
    
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/numberSprites", dmaNumberSprites);

bool dmaNumberSprites(u16 index, u32 value, u8 arg2, u16 flag) {
    
    bool result = FALSE;

    u8 digitIndex;
    u8 remainingDigit;
    
    if (index < MAX_NUMBER_SPRITES) {
        
        if (numberSprites[index].flags & NUMBER_SPRITES_FLAG_ACTIVE) {
            
            numberSprites[index].maxDigitIndex = arg2;
            numberSprites[index].value = value;

            digitIndex = numberSprites[index].maxDigitIndex;
            
            numberSprites[index].flags |= NUMBER_SPRITES_FLAG_RENDER_READY;
            
            do {
                
                dmaSprite(numberSprites[index].spriteIndex + digitIndex, 
                    numberSprites[index].romSpritesheetStart, 
                    numberSprites[index].romSpritesheetEnd, 
                    numberSprites[index].romAssetsIndexStart, 
                    numberSprites[index].romAssetsIndexEnd, 
                    NULL, 
                    NULL, 
                    numberSprites[index].vaddrTexture, 
                    NULL, 
                    numberSprites[index].vaddrPalette, 
                    numberSprites[index].vaddrAnimationFrameMetadata, 
                    numberSprites[index].spriteToPaletteVaddr, 
                    numberSprites[index].spritesheetIndexVaddr, 
                    0, 
                    FALSE);
                
                remainingDigit = digitIndex--;
                
            } while (remainingDigit);
            
            setNumberSpritesRGBA(index, 255, 255, 255, 255);
            
            result = TRUE;
            
            if (flag == 3) {
                numberSprites[index].flags |= NUMBER_SPRITES_FLAG_BLEND_DECAL;
            }

        }

    }

    return result;
}


//INCLUDE_ASM("asm/nonmatchings/system/numberSprites", deactivateNumberSprites);

bool deactivateNumberSprites(u16 index) {
    
    bool result = FALSE;
    
    u8 i;
    u16 check;

    if (index < MAX_NUMBER_SPRITES) {
        
        if (numberSprites[index].flags & NUMBER_SPRITES_FLAG_ACTIVE) {
            
            i = numberSprites[index].maxDigitIndex;

            do {
                deactivateSprite(numberSprites[index].spriteIndex+i);
                check = i--;
            } while (check);
        
            numberSprites[index].flags = 0;
        
            result = TRUE;
        }
        
    }
    
    return result;
}

//INCLUDE_ASM("asm/nonmatchings/system/numberSprites", setNumberSpritesRGBA);

bool setNumberSpritesRGBA(u16 spriteIndex, u8 r, u8 g, u8 b, u8 a) {

    u8 digitIndex = 0;
    bool result = FALSE;

    if (spriteIndex < MAX_NUMBER_SPRITES) {

        if (numberSprites[spriteIndex].flags & NUMBER_SPRITES_FLAG_ACTIVE) {

            digitIndex = numberSprites[spriteIndex].maxDigitIndex;

            do {
                setSpriteColor(numberSprites[spriteIndex].spriteIndex + digitIndex, r, g, b, a);
            } while (digitIndex--);

            result = TRUE;

        }

    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/numberSprites", setNumberSpritesAlpha);

bool setNumberSpritesAlpha(u16 spriteIndex, u8 a) {

    u8 digitIndex = 0;
    bool result = FALSE;

    if (spriteIndex < MAX_NUMBER_SPRITES) {
        
        if (numberSprites[spriteIndex].flags & NUMBER_SPRITES_FLAG_ACTIVE) {

            digitIndex = numberSprites[spriteIndex].maxDigitIndex;

            do {
                setSpriteAlpha(numberSprites[spriteIndex].spriteIndex + digitIndex, a);
            } while (digitIndex--);

            result = TRUE;

        }
    
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/numberSprites", updateOverlayScreenSpriteRGBA);

bool updateOverlayScreenSpriteRGBA(u16 spriteIndex, u8 r, u8 g, u8 b, u8 a, u16 rate) {

    u8 digitIndex = 0;
    bool result = FALSE;

    if (spriteIndex < MAX_NUMBER_SPRITES) {

        if (numberSprites[spriteIndex].flags & NUMBER_SPRITES_FLAG_ACTIVE) {

            digitIndex = numberSprites[spriteIndex].maxDigitIndex;

            do {
                updateSpriteRGBA(numberSprites[spriteIndex].spriteIndex + digitIndex, r, g, b, a, rate);
            } while (digitIndex--);

            result = TRUE;

        }
    
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/numberSprites", updateNumberSpriteAlphaWithTransition);

bool updateNumberSpriteAlphaWithTransition(u16 spriteIndex, u8 a, s16 rate) {

    u8 digitIndex = 0;
    bool result = FALSE;

    if (spriteIndex < MAX_NUMBER_SPRITES) {

        if (numberSprites[spriteIndex].flags & NUMBER_SPRITES_FLAG_ACTIVE) {

            digitIndex = numberSprites[spriteIndex].maxDigitIndex;

            do {
                updateSpriteAlpha(numberSprites[spriteIndex].spriteIndex + digitIndex, a, rate);
            } while (digitIndex--);

            result = TRUE;

        }

    }
    
    return result;
}

//INCLUDE_ASM("asm/nonmatchings/system/numberSprites", renderNumberSpriteDisplayDigits);

bool renderNumberSpriteDisplayDigits(u16 spriteIndex) {

    u8 digitIndex = 0;
    u8 foundNonZero = FALSE;

    bool result = FALSE;

    u32 remainingValue;
    u32 digitValue;

    if (spriteIndex < MAX_NUMBER_SPRITES) {

        if (numberSprites[spriteIndex].flags & NUMBER_SPRITES_FLAG_ACTIVE) {
            
            remainingValue = numberSprites[spriteIndex].value;
            digitIndex = numberSprites[spriteIndex].maxDigitIndex;
            
            do {

                resetAnimationState(numberSprites[spriteIndex].spriteIndex + digitIndex);

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

//INCLUDE_ASM("asm/nonmatchings/system/numberSprites", renderNumberSpriteDisplayDigit);

bool renderNumberSpriteDisplayDigit(u16 spriteIndex, u8 digitValue, u8 digitIndex) {

    bool result = FALSE;
    u16 tempIndex;
    
    if (spriteIndex < MAX_NUMBER_SPRITES) {

        if (numberSprites[spriteIndex].flags & NUMBER_SPRITES_FLAG_ACTIVE) {

            tempIndex = numberSprites[spriteIndex].spriteIndex + digitIndex; 
            
            setSpriteViewSpacePosition(tempIndex, numberSprites[spriteIndex].coordinates.x - numberSprites[spriteIndex].digitSpacing * digitIndex, numberSprites[spriteIndex].coordinates.y, numberSprites[spriteIndex].coordinates.z);
            setSpriteScale(tempIndex, 1.0f, 1.0f, 1.0f);
            setSpriteRotation(tempIndex, 0, 0, 0);
            
            setSpriteAnchorAlignment(tempIndex, SPRITE_ANCHOR_CENTER, SPRITE_ANCHOR_CENTER);;

            if (numberSprites[spriteIndex].flags & NUMBER_SPRITES_FLAG_BLEND_DECAL) {
                setSpriteBlendMode(tempIndex, SPRITE_BLEND_ALPHA_DECAL);
            } else {
                setSpriteBlendMode(tempIndex, SPRITE_BLEND_ALPHA_MODULATED);
            }

            startSpriteAnimation(tempIndex, numberSprites[spriteIndex].animationIndex, numberSprites[spriteIndex].animationBaseFrame + digitValue);
            
            result = TRUE;
        }

    }

    return result;

} 

//INCLUDE_ASM("asm/nonmatchings/system/numberSprites", updateNumberSprites);

void updateNumberSprites(void) {

    u16 i;

    for (i = 0; i < MAX_NUMBER_SPRITES; i++) {
        if ((numberSprites[i].flags & NUMBER_SPRITES_FLAG_ACTIVE) && (numberSprites[i].flags & NUMBER_SPRITES_FLAG_RENDER_READY)) {
            renderNumberSpriteDisplayDigits(i);
        }
    }
    
}