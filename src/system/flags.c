#include "common.h"

#include "mainproc.h"

// bss
// never read from
// might be from mainproc.c
u16 D_80181B8C;

//INCLUDE_ASM("asm/nonmatchings/system/flags", initializeFlags);

// used by mainproc
void initializeFlags(void) {
    D_80222730 = 0;
    D_80181B8C = 0;
}

//INCLUDE_ASM("asm/nonmatchings/system/flags", func_8004DEC8);

// used at start of main lopo
void func_8004DEC8(void) {
    D_80222730 = 0;
    D_80181B8C = 1;
}

//INCLUDE_ASM("asm/nonmatchings/system/flags", func_8004DEE4);

// unused
void func_8004DEE4(void) {
    D_80181B8C &= ~1;
}

// empty function
void func_8004DF00(void) {}

//INCLUDE_ASM("asm/nonmatchings/system/flags", func_8004DF08);

void func_8004DF08(void) {
func_8004DF08:
    goto func_8004DF08;
}
