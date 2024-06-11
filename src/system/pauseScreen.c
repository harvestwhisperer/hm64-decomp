#include "common.h"

#include "system/pauseScreen.h"

#include "system/sprite.h"

// bss
// struct/array?
extern u16 D_80189A88;
extern u8 D_80189A8A;
extern u16 D_80189A8C;

// data
extern u32 D_801133B0[];

extern PauseScreenSprite pauseScreenSprites[MAX_PAUSE_SCREEN_SPRITES];

// forward declaration
bool func_80046504(u16 spriteIndex);
bool func_80046650(u16 spriteIndex, u8 arg1, u8 arg2);

//INCLUDE_ASM(const s32, "system/pauseScreen", func_80045DE0);

void func_80045DE0(void) {

    u16 i;

    for (i = 0; i < MAX_PAUSE_SCREEN_SPRITES; i++) {
        pauseScreenSprites[i].unk_38 = 0;
        pauseScreenSprites[i].flags = 0;
    }
}

//INCLUDE_ASM(const s32, "system/pauseScreen", func_80045E20);

bool func_80045E20(u16 index, u16 arg1, void *arg2, void *arg3, void *arg4, void *arg5, void *arg6, void *arg7, void *arg8, void *arg9, void *argA, u16 argB, u8 argC, f32 argD, f32 argE, f32 argF, u8 arg10) {
    
    bool result = FALSE;
    
    if (index < MAX_PAUSE_SCREEN_SPRITES) {

        if (!(pauseScreenSprites[index].flags & 1)) {
        
            result = TRUE;
        
            pauseScreenSprites[index].romSpritesheetStart = arg2;
            pauseScreenSprites[index].romSpritesheetEnd = arg3;
            pauseScreenSprites[index].romAssetIndexStart = arg4;
            pauseScreenSprites[index].romAssetIndexEnd = arg5;
            pauseScreenSprites[index].vaddrSpritesheet = arg6;
            pauseScreenSprites[index].vaddrPalette = arg7;
            pauseScreenSprites[index].vaddrUnknownAssetSheet = arg8;
            pauseScreenSprites[index].vaddrUnknownAsset2 = arg9;
            pauseScreenSprites[index].unk_20 = argA;
            pauseScreenSprites[index].specialItemPages = argB;
            pauseScreenSprites[index].unk_26 = argC;
            pauseScreenSprites[index].unk_28.x = argD;
            pauseScreenSprites[index].unk_28.y = argE;
            pauseScreenSprites[index].unk_28.z = argF;
            pauseScreenSprites[index].unk_27 = arg10;
            pauseScreenSprites[index].unk_38 = arg1;
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
                
                func_8002B138(pauseScreenSprites[index].unk_38 + count, 
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
                deactivateSprite(pauseScreenSprites[index].unk_38+i);
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
                func_8002C914(pauseScreenSprites[spriteIndex].unk_38 + count, arg1, arg2, arg3, arg4);
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
                func_8002CAA8(pauseScreenSprites[spriteIndex].unk_38 + count, arg1);
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
                func_8002C1C0(pauseScreenSprites[spriteIndex].unk_38 + count, arg1, arg2, arg3, arg4, arg5);
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
                func_8002C52C(pauseScreenSprites[spriteIndex].unk_38 + count, arg1, arg2);
            } while (count--);

            result = TRUE;
        }
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/pauseScreen", func_80046504);

bool func_80046504(u16 spriteIndex) {

    u8 count = 0;
    u8 found = 0;

    bool result = FALSE;

    u32 value;
    u32 check;

    if (spriteIndex < MAX_PAUSE_SCREEN_SPRITES) {

        if (pauseScreenSprites[spriteIndex].flags & 1) {
            
            value = pauseScreenSprites[spriteIndex].value;
            count = pauseScreenSprites[spriteIndex].count;
            
            do {

                func_8002BAD8(pauseScreenSprites[spriteIndex].unk_38 + count);

                check = value / D_801133B0[count];

                value %= D_801133B0[count];

                if (check) {
                    func_80046650(spriteIndex, check, count);
                    found = 1;
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
            tempIndex = pauseScreenSprites[spriteIndex].unk_38 + arg2; 
            
            // adjust Vec3fs
            setSpriteShrinkFactor(tempIndex, pauseScreenSprites[spriteIndex].unk_28.x - pauseScreenSprites[spriteIndex].unk_27 * arg2, pauseScreenSprites[spriteIndex].unk_28.y, pauseScreenSprites[spriteIndex].unk_28.z);
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