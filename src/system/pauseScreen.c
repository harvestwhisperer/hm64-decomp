#include "common.h"

#include "system/pauseScreen.h"

#include "system/sprite.h"

// bss
extern PauseScreenSprite pauseScreenSprites[MAX_PAUSE_SCREEN_SPRITES];

// data
extern u32 D_801133B0[];

// forward declaration
bool func_80046504(u16 spriteIndex);
bool func_80046650(u16 spriteIndex, u8 arg1, u8 arg2);

//INCLUDE_ASM(const s32, "system/pauseScreen", func_80045DE0);

void func_80045DE0(void) {

    u16 i;

    for (i = 0; i < MAX_PAUSE_SCREEN_SPRITES; i++) {
        pauseScreenSprites[i].spriteIndex = 0;
        pauseScreenSprites[i].flags = 0;
    }
}

//INCLUDE_ASM(const s32, "system/pauseScreen", func_80045E20);

bool func_80045E20(u16 index, u16 spriteIndex, u32 romSpritesheetStart, u32 romSpritesheetEnd, u32 romAssetIndexStart, u32 romAssetIndexEnd, 
    void *vaddrSpritesheet, void *vaddrPalette, void *vaddrUnknownAssetSheet, void *vaddrUnknownAsset2, 
    u32 argA, u16 argB, u8 argC, f32 x, f32 y, f32 z, u8 arg10) {
    
    bool result = FALSE;
                    
    if (index < MAX_PAUSE_SCREEN_SPRITES) {

        if (!(pauseScreenSprites[index].flags & 1)) {
        
            result = TRUE;
        
            pauseScreenSprites[index].romSpritesheetStart = romSpritesheetStart;
            pauseScreenSprites[index].romSpritesheetEnd = romSpritesheetEnd;
            pauseScreenSprites[index].romAssetIndexStart = romAssetIndexStart;
            pauseScreenSprites[index].romAssetIndexEnd = romAssetIndexEnd;

            pauseScreenSprites[index].vaddrSpritesheet = vaddrSpritesheet;
            pauseScreenSprites[index].vaddrPalette = vaddrPalette;
            pauseScreenSprites[index].vaddrUnknownAssetSheet = vaddrUnknownAssetSheet;
            pauseScreenSprites[index].vaddrUnknownAsset2 = vaddrUnknownAsset2;

            pauseScreenSprites[index].unk_20 = argA;
            
            pauseScreenSprites[index].specialItemPages = argB;
            
            pauseScreenSprites[index].unk_26 = argC;
            
            pauseScreenSprites[index].coordinates.x = x;
            pauseScreenSprites[index].coordinates.y = y;
            pauseScreenSprites[index].coordinates.z = z;
            
            pauseScreenSprites[index].unk_27 = arg10;
            
            pauseScreenSprites[index].spriteIndex = spriteIndex;
            pauseScreenSprites[index].flags = 1;

        }
    }
    
    return result;

}

//INCLUDE_ASM(const s32, "system/pauseScreen", func_80045F5C);

bool func_80045F5C(u16 index, u32 arg1, u8 arg2, u16 flag) {
    
    bool result = FALSE;
    u8 count;
    u8 check;
    
    if (index < MAX_PAUSE_SCREEN_SPRITES) {
        
        if (pauseScreenSprites[index].flags & 1) {
            
            pauseScreenSprites[index].count = arg2;
            pauseScreenSprites[index].value = arg1;

            count = pauseScreenSprites[index].count;
            
            pauseScreenSprites[index].flags |= 2;
            
            do {
                
                func_8002B138(pauseScreenSprites[index].spriteIndex + count, 
                    pauseScreenSprites[index].romSpritesheetStart, 
                    pauseScreenSprites[index].romSpritesheetEnd, 
                    pauseScreenSprites[index].romAssetIndexStart, 
                    pauseScreenSprites[index].romAssetIndexEnd, 
                    0, 
                    0, 
                    pauseScreenSprites[index].vaddrSpritesheet, 
                    0, 
                    pauseScreenSprites[index].vaddrPalette, 
                    pauseScreenSprites[index].vaddrUnknownAssetSheet, 
                    pauseScreenSprites[index].vaddrUnknownAsset2, 
                    pauseScreenSprites[index].unk_20, 
                    0, 
                    0);
                
                check = count--;
                
            } while (check);
            
            func_800461D8(index, 0xFF, 0xFF, 0xFF, 0xFF);
            
            result = TRUE;
            
            if (flag == 3) {
                pauseScreenSprites[index].flags |= 4;
            }
        }
    }

    return result;
}


//INCLUDE_ASM(const s32, "system/pauseScreen", func_80046120);

bool func_80046120(u16 index) {
    
    bool result = FALSE;
    
    u8 i;
    u16 check;

    if (index < MAX_PAUSE_SCREEN_SPRITES) {
        
        if (pauseScreenSprites[index].flags & 1) {
            
            i = pauseScreenSprites[index].count;

            do {
                deactivateSprite(pauseScreenSprites[index].spriteIndex+i);
                check = i--;
            } while (check);
        
            pauseScreenSprites[index].flags = 0;
        
            result = TRUE;
        }
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/pauseScreen", func_800461D8);

bool func_800461D8(u16 spriteIndex, u8 arg1, u8 arg2, u8 arg3, u8 arg4) {

    u8 count = 0;
    bool result = FALSE;

    if (spriteIndex < MAX_PAUSE_SCREEN_SPRITES) {

        if (pauseScreenSprites[spriteIndex].flags & 1) {

            count = pauseScreenSprites[spriteIndex].count;

            do {
                // set rgba
                func_8002C914(pauseScreenSprites[spriteIndex].spriteIndex + count, arg1, arg2, arg3, arg4);
            } while (count--);

            result = TRUE;
        }
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "system/pauseScreen", func_800462B4);

bool func_800462B4(u16 spriteIndex, u8 arg1) {

    u8 count = 0;
    bool result = FALSE;

    if (spriteIndex < MAX_PAUSE_SCREEN_SPRITES) {
        
        if (pauseScreenSprites[spriteIndex].flags & 1) {

            count = pauseScreenSprites[spriteIndex].count;

            do {
                func_8002CAA8(pauseScreenSprites[spriteIndex].spriteIndex + count, arg1);
            } while (count--);

            result = TRUE;
        }
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/pauseScreen", func_8004635C);

bool func_8004635C(u16 spriteIndex, u8 arg1, u8 arg2, u8 arg3, u8 arg4, u16 arg5) {

    u8 count = 0;
    bool result = FALSE;

    if (spriteIndex < MAX_PAUSE_SCREEN_SPRITES) {

        if (pauseScreenSprites[spriteIndex].flags & 1) {

            count = pauseScreenSprites[spriteIndex].count;

            do {
                func_8002C1C0(pauseScreenSprites[spriteIndex].spriteIndex + count, arg1, arg2, arg3, arg4, arg5);
            } while (count--);

            result = TRUE;
        }
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/pauseScreen", func_8004644C);

// update rgba alpha
bool func_8004644C(u16 spriteIndex, u8 arg1, s16 arg2) {

    u8 count = 0;
    bool result = FALSE;

    if (spriteIndex < MAX_PAUSE_SCREEN_SPRITES) {

        if (pauseScreenSprites[spriteIndex].flags & 1) {

            count = pauseScreenSprites[spriteIndex].count;

            do {
                func_8002C52C(pauseScreenSprites[spriteIndex].spriteIndex + count, arg1, arg2);
            } while (count--);

            result = TRUE;
        }
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/pauseScreen", func_80046504);

bool func_80046504(u16 spriteIndex) {

    u8 count = 0;
    u8 found = FALSE;

    bool result = FALSE;

    u32 value;
    u32 check;

    if (spriteIndex < MAX_PAUSE_SCREEN_SPRITES) {

        if (pauseScreenSprites[spriteIndex].flags & 1) {
            
            value = pauseScreenSprites[spriteIndex].value;
            count = pauseScreenSprites[spriteIndex].count;
            
            do {

                func_8002BAD8(pauseScreenSprites[spriteIndex].spriteIndex + count);

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

//INCLUDE_ASM(const s32, "system/pauseScreen", func_80046650);

bool func_80046650(u16 spriteIndex, u8 arg1, u8 arg2) {

    bool result = FALSE;
    u16 tempIndex;
    
    if (spriteIndex < MAX_PAUSE_SCREEN_SPRITES) {

        if (pauseScreenSprites[spriteIndex].flags & 1) {

            // global sprite index
            tempIndex = pauseScreenSprites[spriteIndex].spriteIndex + arg2; 
            
            // adjust Vec3fs
            setSpriteShrinkFactor(tempIndex, pauseScreenSprites[spriteIndex].coordinates.x - pauseScreenSprites[spriteIndex].unk_27 * arg2, pauseScreenSprites[spriteIndex].coordinates.y, pauseScreenSprites[spriteIndex].coordinates.z);
            setSpriteScale(tempIndex, 1.0f, 1.0f, 1.0f);
            func_8002BE14(tempIndex, 0, 0, 0);
            
            func_8002C680(tempIndex, 2, 2);

            if (pauseScreenSprites[spriteIndex].flags & 4) {
                func_8002C7EC(tempIndex, 3);
            } else {
                func_8002C7EC(tempIndex, 2);
            }

            // animation
            func_8002B80C(tempIndex, pauseScreenSprites[spriteIndex].specialItemPages, pauseScreenSprites[spriteIndex].unk_26 + arg1);
            
            result = TRUE;
        }
    }

    return result;

} 

//INCLUDE_ASM(const s32, "system/pauseScreen", func_800467F8);

// main loop callback
void func_800467F8(void) {

    u16 i;

    for (i = 0; i < MAX_PAUSE_SCREEN_SPRITES; i++) {
        if ((pauseScreenSprites[i].flags & 1) && (pauseScreenSprites[i].flags & 2)) {
            func_80046504(i);
        }
    }
}