#include "common.h"

extern u8 D_8016FBD7;
extern f32 D_8016FBD8;
extern f32 D_8016FBDC;

void func_8002CB88(u16, u16);                          


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
    u16 temp_s0;
    u16 var_s0;

    temp_s0 = D_8016FBD7;
    
    func_8002CB88(temp_s0 + 0x83, 0);
    
    if (D_8016FBDC == 52.0f) {
        D_8016FBDC -= 48.0f;
        var_s0 = temp_s0 + 2;
    } else {
        var_s0 = temp_s0 - 2;
        D_8016FBDC += 48.0f;
    }
    
    func_8002CB88(var_s0 + 0x83, var_s0 + 1);
    
    D_8016FBD7 = var_s0;
}

//INCLUDE_ASM(const s32, "namingScreen", func_800F12C4);

// birthday season selection
void func_800F12C4(void) {

    u16 temp_s0;
    u16 var_s0;

    temp_s0 = D_8016FBD7;
    
    func_8002CB88(temp_s0 + 0x83, 0);
    
    if (D_8016FBD8 == -128.0f) {
        D_8016FBD8 += 144.0f;
        var_s0 = temp_s0 + 1;
    } else {
        var_s0 = temp_s0 - 1;
        D_8016FBD8 -= 144.0f;
    }
 
    func_8002CB88(var_s0 + 0x83, var_s0+1);
    D_8016FBD7 = var_s0;

}