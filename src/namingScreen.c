#include "common.h"

#include "namingScreen.h"

#include "system/sprite.h"


// bss
NamingScreenContext namingScreenContext;

INCLUDE_ASM(const s32, "namingScreen", func_800ED8A0);

INCLUDE_ASM(const s32, "namingScreen", func_800ED974);

INCLUDE_ASM(const s32, "namingScreen", func_800EE678);

INCLUDE_ASM(const s32, "namingScreen", func_800EF578);

INCLUDE_ASM(const s32, "namingScreen", func_800EFADC);

INCLUDE_ASM(const s32, "namingScreen", func_800EFBEC);

INCLUDE_ASM(const s32, "namingScreen", func_800EFCF8);

//INCLUDE_ASM(const s32, "namingScreen", func_800EFE84);

void func_800EFE84(void) {

    namingScreenContext.coordinates.y -= 16.0f;
    namingScreenContext.coordinates.w -= 16.0f;
    
    namingScreenContext.unk_19 += 1;
    
    if (namingScreenContext.unk_19 >= 6) {
        
        namingScreenContext.unk_19 = 0;

        namingScreenContext.coordinates.y = 16.0f;
        namingScreenContext.coordinates.w = 26.0f;

        if (namingScreenContext.unk_18 >= 10) {

            namingScreenContext.unk_18 = namingScreenContext.unk_1A;

            namingScreenContext.coordinates.x = (namingScreenContext.unk_18 * 16.0f);
            namingScreenContext.coordinates.x += -126.0f;
            namingScreenContext.coordinates.x += 6.0f;
            
            namingScreenContext.coordinates.z = (namingScreenContext.unk_18 * 16.0f);
            namingScreenContext.coordinates.z += -116.0f;
            namingScreenContext.coordinates.z += 6.0f;

            func_8002BAD8(0x82);
            func_8002B80C(0x82, 2, 0);
            func_8002BAD8(0x91);
            func_8002B80C(0x91, 2, 0);
            func_8002CB88(0x91, 3);
            
        }
        
    }

    func_800F0320();
    
}

INCLUDE_ASM(const s32, "namingScreen", func_800EFFDC);

//INCLUDE_ASM(const s32, "namingScreen", func_800F009C);

void func_800F009C(void) {

    s32 i = 0x80;

    while (i < 0x92) {
        func_8002B6B8(i);
        i++;
    } 
    
}

// alternate
/*
void func_800F009C(void) {

    s32 i;

    for (i = 0x80; i < 0x92; i++) {
        func_8002B6B8(i);
    } 
    
}
*/

INCLUDE_ASM(const s32, "namingScreen", func_800F00D8);

//INCLUDE_ASM(const s32, "namingScreen", func_800F0320);

void func_800F0320(void) {
    
    if ((namingScreenContext.unk_19 == 5) && (namingScreenContext.unk_18 >= 10)) {
        
        namingScreenContext.unk_1A = namingScreenContext.unk_18;
        
        namingScreenContext.unk_18 = 12;
        namingScreenContext.coordinates.x = 100.0f;
        namingScreenContext.coordinates.y = -95.0f;
        
        func_8002BAD8(0x82);
        func_8002BAD8(0x91);
        func_8002B80C(0x91, 2, 1);
        func_8002CB88(0x91, 4);
        
    }
}


// check button pressed
INCLUDE_ASM(const s32, "namingScreen", func_800F03C4);

INCLUDE_ASM(const s32, "namingScreen", func_800F06D8);

INCLUDE_ASM(const s32, "namingScreen", func_800F0F84);

//INCLUDE_ASM(const s32, "namingScreen", func_800F121C);

// birthday season selection
void func_800F121C(void) {

    u16 temp1;
    u16 temp2;

    temp1 = namingScreenContext.unk_7;
    
    func_8002CB88(temp1 + 0x83, 0);
    
    if (namingScreenContext.coordinates.y == 52.0f) {
        namingScreenContext.coordinates.y -= 48.0f;
        temp2 = temp1 + 2;
    } else {
        temp2 = temp1 - 2;
        namingScreenContext.coordinates.y += 48.0f;
    }
    
    func_8002CB88(temp2 + 0x83, temp2 + 1);
    
    namingScreenContext.unk_7 = temp2;

}

//INCLUDE_ASM(const s32, "namingScreen", func_800F12C4);

// birthday season selection
void func_800F12C4(void) {

    u16 temp1;
    u16 temp2;

    temp1 = namingScreenContext.unk_7;
    
    func_8002CB88(temp1 + 0x83, 0);
    
    if (namingScreenContext.coordinates.x == -128.0f) {
        namingScreenContext.coordinates.x += 144.0f;
        temp2 = temp1 + 1;
    } else {
        temp2 = temp1 - 1;
        namingScreenContext.coordinates.x -= 144.0f;
    }
 
    func_8002CB88(temp2 + 0x83, temp2+1);
    namingScreenContext.unk_7 = temp2;

}