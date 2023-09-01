#include "common.h"

#include "system/sprite.h"

// bss
// struct
extern u8 D_8016FBD7;
extern f32 D_8016FBD8;
extern f32 D_8016FBDC;

INCLUDE_ASM(const s32, "namingScreen", func_800ED8A0);

INCLUDE_ASM(const s32, "namingScreen", func_800ED974);

INCLUDE_ASM(const s32, "namingScreen", func_800EE678);

INCLUDE_ASM(const s32, "namingScreen", func_800EF578);

INCLUDE_ASM(const s32, "namingScreen", func_800EFADC);

INCLUDE_ASM(const s32, "namingScreen", func_800EFBEC);

INCLUDE_ASM(const s32, "namingScreen", func_800EFCF8);

INCLUDE_ASM(const s32, "namingScreen", func_800EFE84);

INCLUDE_ASM(const s32, "namingScreen", func_800EFFDC);

INCLUDE_ASM(const s32, "namingScreen", func_800F009C);

INCLUDE_ASM(const s32, "namingScreen", func_800F00D8);

INCLUDE_ASM(const s32, "namingScreen", func_800F0320);

// check button pressed
INCLUDE_ASM(const s32, "namingScreen", func_800F03C4);

INCLUDE_ASM(const s32, "namingScreen", func_800F06D8);

INCLUDE_ASM(const s32, "namingScreen", func_800F0F84);

//INCLUDE_ASM(const s32, "namingScreen", func_800F121C);

// birthday season selection
void func_800F121C(void) {

    u16 temp1;
    u16 temp2;

    temp1 = D_8016FBD7;
    
    func_8002CB88(temp1 + 0x83, 0);
    
    if (D_8016FBDC == 52.0f) {
        D_8016FBDC -= 48.0f;
        temp2 = temp1 + 2;
    } else {
        temp2 = temp1 - 2;
        D_8016FBDC += 48.0f;
    }
    
    func_8002CB88(temp2 + 0x83, temp2 + 1);
    
    D_8016FBD7 = temp2;

}

//INCLUDE_ASM(const s32, "namingScreen", func_800F12C4);

// birthday season selection
void func_800F12C4(void) {

    u16 temp1;
    u16 temp2;

    temp1 = D_8016FBD7;
    
    func_8002CB88(temp1 + 0x83, 0);
    
    if (D_8016FBD8 == -128.0f) {
        D_8016FBD8 += 144.0f;
        temp2 = temp1 + 1;
    } else {
        temp2 = temp1 - 1;
        D_8016FBD8 -= 144.0f;
    }
 
    func_8002CB88(temp2 + 0x83, temp2+1);
    D_8016FBD7 = temp2;

}