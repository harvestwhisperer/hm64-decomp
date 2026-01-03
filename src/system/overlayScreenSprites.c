#include "common.h"

#include "system/overlayScreenSprites.h"

#include "system/globalSprites.h"
#include "system/sprite.h"

// bss
OverlayScreenSprite overlayScreenSprites[MAX_PAUSE_SCREEN_SPRITES];

u32 D_801133B0[] = {
    1, 10, 100, 1000, 
    10000, 100000, 1000000, 10000000
};

// forward declaration
bool func_80046504(u16 spriteIndex);
bool func_80046650(u16 spriteIndex, u8 arg1, u8 arg2);

//INCLUDE_ASM("asm/nonmatchings/system/overlayScreenSprites", initializeOverlayScreenSprites);

void initializeOverlayScreenSprites(void) {

    u16 i;

    for (i = 0; i < MAX_PAUSE_SCREEN_SPRITES; i++) {
        overlayScreenSprites[i].spriteIndex = 0;
        overlayScreenSprites[i].flags = 0;
    }
}

//INCLUDE_ASM("asm/nonmatchings/system/overlayScreenSprites", setOverlayScreenSprites);

bool setOverlayScreenSprites(u16 index, u16 spriteIndex, u32 romSpritesheetStart, u32 romSpritesheetEnd, u32 romAssetsIndexStart, u32 romAssetsIndexEnd, 
    u8 *vaddrTexture, u16 *vaddrPalette, AnimationFrameMetadata *vaddrAnimationFrameMetadata, u32 *spriteToPaletteVaddr, 
    u32 *spritesheetIndexVaddr, u16 animationIndex, u8 animationBaseFrame, f32 x, f32 y, f32 z, u8 arg10) {
    
    bool result = FALSE;
                    
    if (index < MAX_PAUSE_SCREEN_SPRITES) {

        if (!(overlayScreenSprites[index].flags & 1)) {
        
            result = TRUE;
        
            overlayScreenSprites[index].romSpritesheetStart = romSpritesheetStart;
            overlayScreenSprites[index].romSpritesheetEnd = romSpritesheetEnd;
            overlayScreenSprites[index].romAssetsIndexStart = romAssetsIndexStart;
            overlayScreenSprites[index].romAssetsIndexEnd = romAssetsIndexEnd;

            overlayScreenSprites[index].vaddrTexture = vaddrTexture;
            overlayScreenSprites[index].vaddrPalette = vaddrPalette;
            overlayScreenSprites[index].vaddrAnimationFrameMetadata = vaddrAnimationFrameMetadata;
            overlayScreenSprites[index].spriteToPaletteVaddr = spriteToPaletteVaddr;
            overlayScreenSprites[index].spritesheetIndexVaddr = spritesheetIndexVaddr;
            
            overlayScreenSprites[index].animationIndex = animationIndex;
            overlayScreenSprites[index].animationBaseFrame = animationBaseFrame;
            
            overlayScreenSprites[index].coordinates.x = x;
            overlayScreenSprites[index].coordinates.y = y;
            overlayScreenSprites[index].coordinates.z = z;
            
            overlayScreenSprites[index].unk_27 = arg10;
            
            overlayScreenSprites[index].spriteIndex = spriteIndex;
            overlayScreenSprites[index].flags = 1;

        }
    
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/overlayScreenSprites", dmaOverlayScreenSprites);

bool dmaOverlayScreenSprites(u16 index, u32 value, u8 arg2, u16 flag) {
    
    bool result = FALSE;
    u8 count;
    u8 check;
    
    if (index < MAX_PAUSE_SCREEN_SPRITES) {
        
        if (overlayScreenSprites[index].flags & 1) {
            
            overlayScreenSprites[index].count = arg2;
            overlayScreenSprites[index].value = value;

            count = overlayScreenSprites[index].count;
            
            overlayScreenSprites[index].flags |= 2;
            
            do {
                
                dmaSprite(overlayScreenSprites[index].spriteIndex + count, 
                    overlayScreenSprites[index].romSpritesheetStart, 
                    overlayScreenSprites[index].romSpritesheetEnd, 
                    overlayScreenSprites[index].romAssetsIndexStart, 
                    overlayScreenSprites[index].romAssetsIndexEnd, 
                    NULL, 
                    NULL, 
                    overlayScreenSprites[index].vaddrTexture, 
                    NULL, 
                    overlayScreenSprites[index].vaddrPalette, 
                    overlayScreenSprites[index].vaddrAnimationFrameMetadata, 
                    overlayScreenSprites[index].spriteToPaletteVaddr, 
                    overlayScreenSprites[index].spritesheetIndexVaddr, 
                    0, 
                    FALSE);
                
                check = count--;
                
            } while (check);
            
            setOverlayScreenSpritesRGBA(index, 0xFF, 0xFF, 0xFF, 0xFF);
            
            result = TRUE;
            
            if (flag == 3) {
                overlayScreenSprites[index].flags |= 4;
            }

        }

    }

    return result;
}


//INCLUDE_ASM("asm/nonmatchings/system/overlayScreenSprites", deactivateOverlayScreenSprites);

bool deactivateOverlayScreenSprites(u16 index) {
    
    bool result = FALSE;
    
    u8 i;
    u16 check;

    if (index < MAX_PAUSE_SCREEN_SPRITES) {
        
        if (overlayScreenSprites[index].flags & 1) {
            
            i = overlayScreenSprites[index].count;

            do {
                deactivateSprite(overlayScreenSprites[index].spriteIndex+i);
                check = i--;
            } while (check);
        
            overlayScreenSprites[index].flags = 0;
        
            result = TRUE;
        }
    }
    
    return result;
}

//INCLUDE_ASM("asm/nonmatchings/system/overlayScreenSprites", setOverlayScreenSpritesRGBA);

bool setOverlayScreenSpritesRGBA(u16 spriteIndex, u8 r, u8 g, u8 b, u8 a) {

    u8 count = 0;
    bool result = FALSE;

    if (spriteIndex < MAX_PAUSE_SCREEN_SPRITES) {

        if (overlayScreenSprites[spriteIndex].flags & 1) {

            count = overlayScreenSprites[spriteIndex].count;

            do {
                setSpriteColor(overlayScreenSprites[spriteIndex].spriteIndex + count, r, g, b, a);
            } while (count--);

            result = TRUE;

        }
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/overlayScreenSprites", setOverlayScreenSpritesAlpha);

bool setOverlayScreenSpritesAlpha(u16 spriteIndex, u8 a) {

    u8 count = 0;
    bool result = FALSE;

    if (spriteIndex < MAX_PAUSE_SCREEN_SPRITES) {
        
        if (overlayScreenSprites[spriteIndex].flags & 1) {

            count = overlayScreenSprites[spriteIndex].count;

            do {
                setSpriteAlpha(overlayScreenSprites[spriteIndex].spriteIndex + count, a);
            } while (count--);

            result = TRUE;

        }
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/overlayScreenSprites", updateOverlayScreenSpriteRGBA);

bool updateOverlayScreenSpriteRGBA(u16 spriteIndex, u8 arg1, u8 arg2, u8 arg3, u8 arg4, u16 arg5) {

    u8 count = 0;
    bool result = FALSE;

    if (spriteIndex < MAX_PAUSE_SCREEN_SPRITES) {

        if (overlayScreenSprites[spriteIndex].flags & 1) {

            count = overlayScreenSprites[spriteIndex].count;

            do {
                updateSpriteRGBA(overlayScreenSprites[spriteIndex].spriteIndex + count, arg1, arg2, arg3, arg4, arg5);
            } while (count--);

            result = TRUE;

        }
    }
    
    return result;
}

//INCLUDE_ASM("asm/nonmatchings/system/overlayScreenSprites", func_8004644C);

// update rgba alpha
bool func_8004644C(u16 spriteIndex, u8 arg1, s16 arg2) {

    u8 count = 0;
    bool result = FALSE;

    if (spriteIndex < MAX_PAUSE_SCREEN_SPRITES) {

        if (overlayScreenSprites[spriteIndex].flags & 1) {

            count = overlayScreenSprites[spriteIndex].count;

            do {
                updateSpriteAlpha(overlayScreenSprites[spriteIndex].spriteIndex + count, arg1, arg2);
            } while (count--);

            result = TRUE;
        }
    }
    
    return result;
}

//INCLUDE_ASM("asm/nonmatchings/system/overlayScreenSprites", func_80046504);

bool func_80046504(u16 spriteIndex) {

    u8 count = 0;
    u8 found = FALSE;

    bool result = FALSE;

    u32 value;
    u32 check;

    if (spriteIndex < MAX_PAUSE_SCREEN_SPRITES) {

        if (overlayScreenSprites[spriteIndex].flags & 1) {
            
            value = overlayScreenSprites[spriteIndex].value;
            count = overlayScreenSprites[spriteIndex].count;
            
            do {

                resetAnimationState(overlayScreenSprites[spriteIndex].spriteIndex + count);

                check = value / D_801133B0[count];

                value %= D_801133B0[count];

                if (check) {
                    func_80046650(spriteIndex, check, count);
                    found = TRUE;
                } else if (!found) {
                    if (count == 0) {
                        func_80046650(spriteIndex, 0, count);
                    }
                } else {
                    func_80046650(spriteIndex, 0, count);
                }
                
            } while (count--);

            result = TRUE;

        }
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/overlayScreenSprites", func_80046650);

bool func_80046650(u16 spriteIndex, u8 arg1, u8 arg2) {

    bool result = FALSE;
    u16 tempIndex;
    
    if (spriteIndex < MAX_PAUSE_SCREEN_SPRITES) {

        if (overlayScreenSprites[spriteIndex].flags & 1) {

            tempIndex = overlayScreenSprites[spriteIndex].spriteIndex + arg2; 
            
            setSpriteViewSpacePosition(tempIndex, overlayScreenSprites[spriteIndex].coordinates.x - overlayScreenSprites[spriteIndex].unk_27 * arg2, overlayScreenSprites[spriteIndex].coordinates.y, overlayScreenSprites[spriteIndex].coordinates.z);
            setSpriteScale(tempIndex, 1.0f, 1.0f, 1.0f);
            setSpriteRotation(tempIndex, 0, 0, 0);
            
            setSpriteAnchorAlignment(tempIndex, SPRITE_ANCHOR_CENTER, SPRITE_ANCHOR_CENTER);;

            if (overlayScreenSprites[spriteIndex].flags & 4) {
                setSpriteBlendMode(tempIndex, SPRITE_BLEND_ALPHA_DECAL);
            } else {
                setSpriteBlendMode(tempIndex, SPRITE_BLEND_ALPHA_MODULATED);
            }

            startSpriteAnimation(tempIndex, overlayScreenSprites[spriteIndex].animationIndex, overlayScreenSprites[spriteIndex].animationBaseFrame + arg1);
            
            result = TRUE;
        }
    }

    return result;

} 

//INCLUDE_ASM("asm/nonmatchings/system/overlayScreenSprites", updateOverlayScreenSprites);

void updateOverlayScreenSprites(void) {

    u16 i;

    for (i = 0; i < MAX_PAUSE_SCREEN_SPRITES; i++) {
        if ((overlayScreenSprites[i].flags & 1) && (overlayScreenSprites[i].flags & 2)) {
            func_80046504(i);
        }
    }
    
}