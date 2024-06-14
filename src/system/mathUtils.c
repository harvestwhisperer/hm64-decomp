#include "common.h"
#include "gcc/stdlib.h"

// libmkc
extern int rand(void);

//INCLUDE_ASM(const s32, "system/utility", func_80026850);

s32 getAbsoluteValue(s32 number) {
    
    s16 result = number;
    
    if (result < 0) {
        result = number * -1;
        return result;
    }
    
    return result;

}

//INCLUDE_ASM(const s32, "system/mathUtils", getRandomNumberInRange);

u16 getRandomNumberInRange(u16 min, u16 max) {
    
    int temp;

    rand();

    temp = (u16)rand() * (max - min + 1);
    
    if (temp < 0) {
        temp += RAND_MAX;
    }
    
    return (min + (temp >> 0xF));

}