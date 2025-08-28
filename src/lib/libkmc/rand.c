#include "common.h"
#include "gcc/stdlib.h"

static s32 sRandNext;

int rand() {
    sRandNext = sRandNext * 1103515245 + 12345;

    return ((u32)((sRandNext + 1) >> 16) % (RAND_MAX + 1));
}

void srand(unsigned seed) {
    sRandNext = seed;
}