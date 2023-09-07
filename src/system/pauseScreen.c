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
bool func_80046504(u16);
bool func_80046650(u16 arg0, u8 arg1, u8 arg2);

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
    
    bool result = 0;
    
    if (index < MAX_PAUSE_SCREEN_SPRITES) {

        if (!(pauseScreenSprites[index].flags & 1)) {
        
            result = 1;
        
            pauseScreenSprites[index].romAddr1 = arg2;
            pauseScreenSprites[index].romAddr2 = arg3;
            pauseScreenSprites[index].romAddr3 = arg4;
            pauseScreenSprites[index].romAddr4 = arg5;
            pauseScreenSprites[index].vaddr1 = arg6;
            pauseScreenSprites[index].vaddr2 = arg7;
            pauseScreenSprites[index].vaddr3 = arg8;
            pauseScreenSprites[index].vaddr4 = arg9;
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
    
    bool result = 0;
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
                    pauseScreenSprites[index].romAddr1, 
                    pauseScreenSprites[index].romAddr2, 
                    pauseScreenSprites[index].romAddr3, 
                    pauseScreenSprites[index].romAddr4, 
                    0, 
                    0, 
                    pauseScreenSprites[index].vaddr1, 
                    0, 
                    pauseScreenSprites[index].vaddr2, 
                    pauseScreenSprites[index].vaddr3, 
                    pauseScreenSprites[index].vaddr4, 
                    pauseScreenSprites[index].unk_20, 
                    0, 
                    0);
                
                check = count--;
                
            } while (check);
            
            func_800461D8(index, 0xFF, 0xFF, 0xFF, 0xFF);
            
            result = 1;
            
            if (flag == 3) {
                pauseScreenSprites[index].flags |= 4;
            }
        }
    }

    return result;
}


//INCLUDE_ASM(const s32, "system/pauseScreen", func_80046120);

bool func_80046120(u16 index) {
    
    bool result = 0;
    
    u8 i;
    u16 check;

    if (index < MAX_PAUSE_SCREEN_SPRITES) {
        
        if (pauseScreenSprites[index].flags & 1) {
            
            i = pauseScreenSprites[index].count;

            do {
                func_8002B6B8(pauseScreenSprites[index].unk_38+i);
                check = i--;
            } while (check);
        
            pauseScreenSprites[index].flags = 0;
        
            result = 1;
        }
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/pauseScreen", func_800461D8);

bool func_800461D8(u16 arg0, u8 arg1, u8 arg2, u8 arg3, u8 arg4) {

    u8 count = 0;
    u8 result = 0;

    if (arg0 < MAX_PAUSE_SCREEN_SPRITES) {
        if (pauseScreenSprites[arg0].flags & 1) {
            count = pauseScreenSprites[arg0].count;
            do {
                func_8002C914(pauseScreenSprites[arg0].unk_38 + count, arg1, arg2, arg3, arg4);
            } while (count--);
            result = 1;
        }
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "system/pauseScreen", func_800462B4);

bool func_800462B4(u16 arg0, u8 arg1) {

    u8 count = 0;
    u8 result = 0;

    if (arg0 < MAX_PAUSE_SCREEN_SPRITES) {
        if (pauseScreenSprites[arg0].flags & 1) {
            count = pauseScreenSprites[arg0].count;
            do {
                func_8002CAA8(pauseScreenSprites[arg0].unk_38 + count, arg1);
            } while (count--);
            result = 1;
        }
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/pauseScreen", func_8004635C);

bool func_8004635C(u16 arg0, u8 arg1, u8 arg2, u8 arg3, u8 arg4, u16 arg5) {

    u8 count = 0;
    u8 result = 0;

    if (arg0 < MAX_PAUSE_SCREEN_SPRITES) {
        if (pauseScreenSprites[arg0].flags & 1) {
            count = pauseScreenSprites[arg0].count;
            do {
                func_8002C1C0(pauseScreenSprites[arg0].unk_38 + count, arg1, arg2, arg3, arg4, arg5);
            } while (count--);
            result = 1;
        }
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/pauseScreen", func_8004644C);

bool func_8004644C(u16 arg0, u8 arg1, s16 arg2) {

    u8 count = 0;
    u8 result = 0;

    if (arg0 < MAX_PAUSE_SCREEN_SPRITES) {
        if (pauseScreenSprites[arg0].flags & 1) {
            count = pauseScreenSprites[arg0].count;
            do {
                func_8002C52C(pauseScreenSprites[arg0].unk_38 + count, arg1, arg2);
            } while (count--);
            result = 1;
        }
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/pauseScreen", func_80046504);

bool func_80046504(u16 arg0) {

    u8 count = 0;
    u8 found = 0;
    u8 result = 0;
    u32 value;
    u32 check;

    if (arg0 < MAX_PAUSE_SCREEN_SPRITES) {

        if (pauseScreenSprites[arg0].flags & 1) {
            
            value = pauseScreenSprites[arg0].value;
            count = pauseScreenSprites[arg0].count;
            
            do {

                func_8002BAD8(pauseScreenSprites[arg0].unk_38 + count);

                check = value / D_801133B0[count];

                value %= D_801133B0[count];

                if (check) {
                    func_80046650(arg0, check, count);
                    found = 1;
                } else if (!found) {
                    if (count == 0) {
                        func_80046650(arg0, 0, count);
                    }
                } else {
                    func_80046650(arg0, 0, count);
                }
                
            } while (count--);
            
            result = 1;
        }
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/pauseScreen", func_80046650);

bool func_80046650(u16 arg0, u8 arg1, u8 arg2) {

    u8 result = 0;
    u16 temp;
    
    if (arg0 < MAX_PAUSE_SCREEN_SPRITES) {
        if (pauseScreenSprites[arg0].flags & 1) {

            temp = pauseScreenSprites[arg0].unk_38 + arg2; 
            
            func_8002BD0C(temp, pauseScreenSprites[arg0].unk_28.x - pauseScreenSprites[arg0].unk_27 * arg2, pauseScreenSprites[arg0].unk_28.y, pauseScreenSprites[arg0].unk_28.z);
            func_8002BD90(temp, 1.0f, 1.0f, 1.0f);
            func_8002BE14(temp, 0, 0, 0);
            func_8002C680(temp, 2, 2);

            if (pauseScreenSprites[arg0].flags & 4) {
                func_8002C7EC(temp, 3);
            } else {
                func_8002C7EC(temp, 2);
            }

            func_8002B80C(temp, pauseScreenSprites[arg0].specialItemPages, pauseScreenSprites[arg0].unk_26 + arg1);
            
            result = 1;
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