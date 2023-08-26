#include "common.h"

#include "message.h"

extern DialogueBox D_80188B70[6];

extern u16 D_80204BF0[];


INCLUDE_ASM(const s32, "system/message", func_8003D970);

INCLUDE_ASM(const s32, "system/message", func_8003DBE8);

INCLUDE_ASM(const s32, "system/message", func_8003DD14);

INCLUDE_ASM(const s32, "system/message", func_8003DDF8);

INCLUDE_ASM(const s32, "system/message", func_8003E77C);

INCLUDE_ASM(const s32, "system/message", func_8003E910);

INCLUDE_ASM(const s32, "system/message", func_8003EA1C);

INCLUDE_ASM(const s32, "system/message", func_8003EE7C);
 
INCLUDE_ASM(const s32, "system/message", func_8003EFD8);

INCLUDE_ASM(const s32, "system/message", func_8003F024);

INCLUDE_ASM(const s32, "system/message", func_8003F0DC);

INCLUDE_ASM(const s32, "system/message", func_8003F130);

INCLUDE_ASM(const s32, "system/message", func_8003F28C);

INCLUDE_ASM(const s32, "system/message", func_8003F30C);

INCLUDE_ASM(const s32, "system/message", func_8003F360);

INCLUDE_ASM(const s32, "system/message", func_8003F464);

INCLUDE_ASM(const s32, "system/message", func_8003F4E0);

INCLUDE_ASM(const s32, "system/message", func_8003F54C);

INCLUDE_ASM(const s32, "system/message", func_8003F5D0);

INCLUDE_ASM(const s32, "system/message", func_8003F630);

INCLUDE_ASM(const s32, "system/message", func_8003F690);

INCLUDE_ASM(const s32, "system/message", func_8003F80C);

INCLUDE_ASM(const s32, "system/message", func_8003F910);

INCLUDE_ASM(const s32, "system/message", func_8003FA1C);

INCLUDE_ASM(const s32, "system/message", func_8003FAE8);

INCLUDE_ASM(const s32, "system/message", func_8003FAF8);

INCLUDE_ASM(const s32, "system/message", func_8003FB4C);

INCLUDE_ASM(const s32, "system/message", func_8003FBA0);

INCLUDE_ASM(const s32, "system/message", func_8003FBD8);

//INCLUDE_ASM(const s32, "system/message", func_8003FD74);

void func_8003FD74(void) {

    u16 i;

    for (i = 0; i < 256; i++) {
        D_80204BF0[i] = 0xEE;
    }
    
}

INCLUDE_ASM(const s32, "system/message", func_8003FDB0);

INCLUDE_ASM(const s32, "system/message", func_8003FE9C);

INCLUDE_ASM(const s32, "system/message", func_8003FFF4);

INCLUDE_ASM(const s32, "system/message", func_80040140);

//INCLUDE_ASM(const s32, "system/message", func_800401C8);

u8 func_800401C8(u16 index) {

    u8 result = 0;

    if (index < 6) {
        if (D_80188B70[index].flags & 1 && D_80188B70[index].flags & 2) {
            result = D_80188B70[index].buttonSfxCounter != 0;
        }
    }

    return result;
}

INCLUDE_ASM(const s32, "system/message", func_8004022C);

INCLUDE_ASM(const s32, "system/message", func_80040318);

INCLUDE_RODATA(const s32, "system/message", D_8011EE10);

INCLUDE_RODATA(const s32, "system/message", D_8011EE30);

INCLUDE_ASM(const s32, "system/message", func_800403F0);

INCLUDE_ASM(const s32, "system/message", func_80040628);

INCLUDE_ASM(const s32, "system/message", func_80040858);

INCLUDE_ASM(const s32, "system/message", func_80040C38);

INCLUDE_ASM(const s32, "system/message", func_80041850);

INCLUDE_ASM(const s32, "system/message", func_800419C4);

INCLUDE_ASM(const s32, "system/message", func_80041B28);

INCLUDE_ASM(const s32, "system/message", func_80041B80);

INCLUDE_ASM(const s32, "system/message", func_80041CD8);

INCLUDE_ASM(const s32, "system/message", func_80042014);

INCLUDE_ASM(const s32, "system/message", func_800423F0);

INCLUDE_RODATA(const s32, "system/message", D_8011EE90);

INCLUDE_RODATA(const s32, "system/message", D_8011EE98);

INCLUDE_RODATA(const s32, "system/message", D_8011EE9C);

INCLUDE_RODATA(const s32, "system/message", D_8011EEA0);

INCLUDE_RODATA(const s32, "system/message", D_8011EEA4);

INCLUDE_RODATA(const s32, "system/message", D_8011EEA8);

INCLUDE_RODATA(const s32, "system/message", D_8011EEAC);

INCLUDE_RODATA(const s32, "system/message", D_8011EEB0);

INCLUDE_RODATA(const s32, "system/message", D_8011EEB4);

INCLUDE_RODATA(const s32, "system/message", D_8011EEB8);

INCLUDE_RODATA(const s32, "system/message", D_8011EEBC);

INCLUDE_RODATA(const s32, "system/message", D_8011EEC0);

INCLUDE_RODATA(const s32, "system/message", D_8011EEC4);

INCLUDE_ASM(const s32, "system/message", func_80042634);
