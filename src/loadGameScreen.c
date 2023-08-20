#include "common.h"

void func_8004DD7C(void*, void*, u8);
extern u8 D_8030E000[16];


INCLUDE_ASM(const s32, "loadGameScreen", func_800E1380);

INCLUDE_ASM(const s32, "loadGameScreen", func_800E16A0);

INCLUDE_ASM(const s32, "loadGameScreen", func_800E16D0);

INCLUDE_ASM(const s32, "loadGameScreen", func_800E1998);

INCLUDE_ASM(const s32, "loadGameScreen", func_800E1A94);

// func_800E1FAC
INCLUDE_ASM(const s32, "loadGameScreen", func_800E1FAC);

INCLUDE_ASM(const s32, "loadGameScreen", func_800E3300);

INCLUDE_ASM(const s32, "loadGameScreen", func_800E3358);

INCLUDE_ASM(const s32, "loadGameScreen", func_800E38E8);

INCLUDE_ASM(const s32, "loadGameScreen", func_800E391C);

INCLUDE_ASM(const s32, "loadGameScreen", func_800E395C);

INCLUDE_ASM(const s32, "loadGameScreen", func_800E3990);

INCLUDE_ASM(const s32, "loadGameScreen", func_800E39D0);

INCLUDE_ASM(const s32, "loadGameScreen", func_800E4424);

INCLUDE_ASM(const s32, "loadGameScreen", func_800E53E8);

INCLUDE_ASM(const s32, "loadGameScreen", func_800E66A0);

INCLUDE_ASM(const s32, "loadGameScreen", func_800E67E4);

INCLUDE_ASM(const s32, "loadGameScreen", func_800E68F4);

INCLUDE_ASM(const s32, "loadGameScreen", func_800E6C08);

INCLUDE_ASM(const s32, "loadGameScreen", func_800E6FB4);

INCLUDE_ASM(const s32, "loadGameScreen", func_800E80AC);

// jtbl_801236C0
INCLUDE_ASM(const s32, "loadGameScreen", func_800E8F08);

INCLUDE_ASM(const s32, "loadGameScreen", func_800E9550);

INCLUDE_ASM(const s32, "loadGameScreen", func_800E9B2C);

//INCLUDE_ASM(const s32, "loadGameScreen", func_800EA2A4);

void func_800EA2A4(u8 arg0) {
    D_8030E000[0] = 0;
    D_8030E000[1] = 0;
    D_8030E000[2] = 0;
    D_8030E000[3] = 0;
    D_8030E000[4] = 0;
    D_8030E000[5] = 0;
    D_8030E000[6] = 0;
    D_8030E000[7] = 0;
    D_8030E000[8] = 0;
    D_8030E000[9] = 0;
    D_8030E000[10] = 0;
    D_8030E000[11] = 0;
    D_8030E000[12] = 0;
    D_8030E000[13] = 0;
    D_8030E000[14] = 0;
    D_8030E000[15] = 0;
    func_8004DD7C(((arg0  << 8) + 0x4000) | 0x08000000, 0x8030E000, 0x10);
}

INCLUDE_ASM(const s32, "loadGameScreen", func_800EA360);

INCLUDE_ASM(const s32, "loadGameScreen", func_800EA3AC);

INCLUDE_ASM(const s32, "loadGameScreen", func_800EAA9C);

INCLUDE_ASM(const s32, "loadGameScreen", func_800EB74C);

INCLUDE_ASM(const s32, "loadGameScreen", func_800EB788);

INCLUDE_ASM(const s32, "loadGameScreen", func_800EBA90);

INCLUDE_ASM(const s32, "loadGameScreen", func_800EBAC8);

INCLUDE_ASM(const s32, "loadGameScreen", func_800EBC00);

INCLUDE_ASM(const s32, "loadGameScreen", func_800EBCD8);

INCLUDE_ASM(const s32, "loadGameScreen", func_800EBEAC);

INCLUDE_ASM(const s32, "loadGameScreen", func_800ED160);
