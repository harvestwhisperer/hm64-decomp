#include "common.h"

#include "system/pauseScreen.h"

#include "system/sprite.h"

// bss
// struct/array?
extern u16 D_80189A88;
extern u8 D_80189A8A;
extern u16 D_80189A8C;

extern PauseScreenSprites D_80189A50[0x10];

//INCLUDE_ASM(const s32, "system/pauseScreen", func_80045DE0);

void func_80045DE0(void) {

    u16 i;

    for (i = 0; i < 0x10; i++) {
        D_80189A50[i].unk_38 = 0;
        D_80189A50[i].flags = 0;
    }
}

//INCLUDE_ASM(const s32, "system/pauseScreen", func_80045E20);

bool func_80045E20(u16 index, u16 arg1, void *arg2, void *arg3, void *arg4, void *arg5, void *arg6, void *arg7, void *arg8, void *arg9, void *argA, u16 argB, u8 argC, f32 argD, f32 argE, f32 argF, u8 arg10) {
    
    bool result = 0;
    
    if (index < 0x10) {
        if (!(D_80189A50[index].flags & 1)) {
            result = 1;
            D_80189A50[index].romAddr1 = arg2;
            D_80189A50[index].romAddr2 = arg3;
            D_80189A50[index].romAddr3 = arg4;
            D_80189A50[index].romAddr4 = arg5;
            D_80189A50[index].vaddr1 = arg6;
            D_80189A50[index].vaddr2 = arg7;
            D_80189A50[index].vaddr3 = arg8;
            D_80189A50[index].vaddr4 = arg9;
            D_80189A50[index].unk_20 = argA;
            D_80189A50[index].specialItemPages = argB;
            D_80189A50[index].unk_26 = argC;
            D_80189A50[index].unk_28.x = argD;
            D_80189A50[index].unk_28.y = argE;
            D_80189A50[index].unk_28.z = argF;
            D_80189A50[index].unk_27 = arg10;
            D_80189A50[index].unk_38 = arg1;
            D_80189A50[index].flags = 1;
        }
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/pauseScreen", func_80045F5C);

bool func_80045F5C(u16 index, u32 arg1, u8 arg2, u16 flag) {
    
    bool result = 0;
    u16 tempIndex = index;
    u16 tempFlag = flag;
    u8 count;
    u8 check;
    
    if (tempIndex < 0x10) {
        
        if (D_80189A50[tempIndex].flags & 1) {
            
            D_80189A50[tempIndex].count = arg2;
            D_80189A50[tempIndex].value = arg1;
            count = D_80189A50[tempIndex].count;
            D_80189A50[tempIndex].flags |= 2;
            
            do {
                
                func_8002B138(D_80189A50[tempIndex].unk_38 + count, 
                    D_80189A50[tempIndex].romAddr1, 
                    D_80189A50[tempIndex].romAddr2, 
                    D_80189A50[tempIndex].romAddr3, 
                    D_80189A50[tempIndex].romAddr4, 
                    0, 
                    0, 
                    D_80189A50[tempIndex].vaddr1, 
                    0, 
                    D_80189A50[tempIndex].vaddr2, 
                    D_80189A50[tempIndex].vaddr3, 
                    D_80189A50[tempIndex].vaddr4, 
                    D_80189A50[tempIndex].unk_20, 
                    0, 
                    0);
                
                check = count--;
                
            } while (check);
            
            func_800461D8(tempIndex, 0xFF, 0xFF, 0xFF, 0xFF);
            
            result = 1;
            
            if (tempFlag == 3) {
                D_80189A50[tempIndex].flags |= 4;
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

    if (index < 0x10) {
        if (D_80189A50[index].flags & 1) {
            i = D_80189A50[index].count;
            do {
                func_8002B6B8(D_80189A50[index].unk_38+i);
                check = i--;
            } while (check);
            D_80189A50[index].flags = 0;
            result = 1;
        }
    }
    
    return result;
}

INCLUDE_ASM(const s32, "system/pauseScreen", func_800461D8);

INCLUDE_ASM(const s32, "system/pauseScreen", func_800462B4);

INCLUDE_ASM(const s32, "system/pauseScreen", func_8004635C);

INCLUDE_ASM(const s32, "system/pauseScreen", func_8004644C);

INCLUDE_ASM(const s32, "system/pauseScreen", func_80046504);

INCLUDE_ASM(const s32, "system/pauseScreen", func_80046650);

INCLUDE_ASM(const s32, "system/pauseScreen", func_800467F8);
