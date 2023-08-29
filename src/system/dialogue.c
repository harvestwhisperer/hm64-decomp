#include "common.h"

// bss
// struct/array
extern u32 D_8013CE0C;
extern u8 D_8013CE10;

extern GameVariable gameVariables[];


INCLUDE_ASM(const s32, "system/dialogue", func_80042F60);

INCLUDE_ASM(const s32, "system/dialogue", func_80042FEC);

INCLUDE_ASM(const s32, "system/dialogue", func_80043050);

//INCLUDE_ASM(const s32, "system/dialogue", setGameVariable);

u8 setGameVariable(u16 index, void *address, u8 numSet, s32 max) {

    u8 result = 0;

    if ((index < TOTAL_GAME_VARIABLES)) {

        if (numSet < 5) {
            gameVariables[index].address = address;
            gameVariables[index].set = numSet;
            gameVariables[index].maxValue = max;
            result = 1;
        }

    }

    return result;
}

INCLUDE_ASM(const s32, "system/dialogue", func_80043138);

INCLUDE_ASM(const s32, "system/dialogue", func_80043148);

INCLUDE_ASM(const s32, "system/dialogue", func_8004318C);

INCLUDE_ASM(const s32, "system/dialogue", func_80043260);

INCLUDE_ASM(const s32, "system/dialogue", func_80043334);

INCLUDE_ASM(const s32, "system/dialogue", func_80043408);

INCLUDE_ASM(const s32, "system/dialogue", func_80043430);

INCLUDE_ASM(const s32, "system/dialogue", func_80043A88);

INCLUDE_ASM(const s32, "system/dialogue", func_80043AD8);

INCLUDE_ASM(const s32, "system/dialogue", func_80043B84);

INCLUDE_ASM(const s32, "system/dialogue", func_80043C6C);

INCLUDE_ASM(const s32, "system/dialogue", func_80043C98);

INCLUDE_ASM(const s32, "system/dialogue", func_80043CF8);

INCLUDE_ASM(const s32, "system/dialogue", func_80043D8C);

INCLUDE_ASM(const s32, "system/dialogue", func_80043E28);

INCLUDE_ASM(const s32, "system/dialogue", func_80043E5C);

INCLUDE_ASM(const s32, "system/dialogue", func_80043E94);

INCLUDE_ASM(const s32, "system/dialogue", func_80043EC8);

INCLUDE_ASM(const s32, "system/dialogue", func_80044684);

INCLUDE_ASM(const s32, "system/dialogue", func_800449C4);

INCLUDE_ASM(const s32, "system/dialogue", func_80044BF4);

INCLUDE_ASM(const s32, "system/dialogue", func_80044D78);

INCLUDE_ASM(const s32, "system/dialogue", func_80045260);

INCLUDE_ASM(const s32, "system/dialogue", func_80045CB0);
