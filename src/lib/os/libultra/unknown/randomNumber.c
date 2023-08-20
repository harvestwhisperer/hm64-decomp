#include "common.h"

extern int D_8013CDF0;

// -O1
// 80110410
//INCLUDE_ASM(const s32, "lib/os/libultra/unknown/randomNumber", getRandomNumber);

int getRandomNumber(void) {
    
    int result = D_8013CDF0 * 1103515245;
    
    D_8013CDF0 = result + 12345;

    return result + 0x303a >> 0x10 & 0x7fff;
    
}

// -O2/-O3
// 80110444
INCLUDE_ASM(const s32, "lib/os/libultra/unknown/randomNumber", func_80110444);

// void func_80110444(u32 value) {
//     D_8013CDF0 = value;
//     return;
// }

INCLUDE_ASM(const s32, "lib/os/libultra/unknown/randomNumber", func_80110454);

INCLUDE_ASM(const s32, "lib/os/libultra/unknown/randomNumber", func_80110488);

INCLUDE_ASM(const s32, "lib/os/libultra/unknown/randomNumber", func_80110584);
