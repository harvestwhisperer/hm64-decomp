#include "common.h"
#include "gcc/stdlib.h"

int sRandNext;

//INCLUDE_ASM(const s32, "lib/libkmc/rand", rand);

int rand() {
    
    sRandNext = sRandNext * 1103515245 + 12345;

    return ((u32)((sRandNext + 1) >> 16) % (RAND_MAX + 1));

}

// alternate
/*
int rand(void) {
    
    int result = sRandNext * 1103515245;
    
    sRandNext = result + 12345;

    return result + 0x303a >> 0x10 & 0x7fff; 
    
}
*/

// FIXME: has an extra nop that's not generated when compiled: likely an unaligned file split
// 80110444
INCLUDE_ASM(const s32, "lib/libkmc/rand", sRand);

// void sRand(int seed) {
//     sRandNext = seed; 
// } 
 
// hasm
INCLUDE_ASM(const s32, "lib/libkmc/rand", __muldi3);

INCLUDE_ASM(const s32, "lib/libkmc/rand", func_80110488);

INCLUDE_ASM(const s32, "lib/libkmc/rand", func_80110584);