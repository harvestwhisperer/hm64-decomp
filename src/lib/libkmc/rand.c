#include "common.h"
#include "gcc/stdlib.h"

int sRandNext;

//INCLUDE_ASM(const s32, "lib/libkmc/rand", rand);

int rand() {
    
    sRandNext = sRandNext * 1103515245 + 12345;

    return ((u32)((sRandNext + 1) >> 16) % (RAND_MAX + 1));

}

// alternate for reference
/*
int rand(void) {
    
    int result = sRandNext * 1103515245;
    
    sRandNext = result + 12345;

    return result + 0x303a >> 0x10 & 0x7fff; 
    
}
*/

// 80110444
INCLUDE_ASM(const s32, "lib/libkmc/rand", sRand);

// void sRand(int seed) {
//     sRandNext = seed; 
// } 
 

// FIXME: should be a file split here, but unaligned

// hasm
// mmuldi3.s
INCLUDE_ASM(const s32, "lib/libkmc/rand", __muldi3);

// part of mmuldi3.s: div64_64
INCLUDE_ASM(const s32, "lib/libkmc/rand", func_80110488);

// part of mmuldi3.s: div64_32
INCLUDE_ASM(const s32, "lib/libkmc/rand", func_80110584);