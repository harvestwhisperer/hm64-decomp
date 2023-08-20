#include "common.h"

void func_8002BD0C(u8, f32, f32, f32);                      
void func_8002BE98(u8, f32, f32, f32);                        

extern f32 D_80126578;
extern f32 D_80200AC4;
extern f32 D_80200B60;
extern f32 D_80200BFC;
extern f32 D_80200C98;
extern f32 D_80200D34;
extern f32 D_80200DD0;

extern u8 D_80119510[];


//INCLUDE_ASM(const s32, "title", func_800DCAA0);

u8 func_800DCAA0(u8 index) {
    return D_80119510[index];
}

// jtbl_80123480
INCLUDE_ASM(const s32, "title", func_800DCAB8);

// jtbl_80123498
INCLUDE_ASM(const s32, "title", func_800DD44C);

// jtbl_80123578
INCLUDE_ASM(const s32, "title", func_800DDDFC);

INCLUDE_ASM(const s32, "title", func_800DE3C0);

INCLUDE_ASM(const s32, "title", func_800DFF64);

// jtbl_80123658
INCLUDE_ASM(const s32, "title", func_800E00A4);

// jtbl_80123670
INCLUDE_ASM(const s32, "title", func_800E0ED4);

INCLUDE_ASM(const s32, "title", func_800E0FEC);

//INCLUDE_ASM(const s32, "title", func_800E10E8);

void func_800E10E8(void) {

    if (D_80200AC4 > 320.0f) {
        func_8002BD0C(0x56, D_80126578 - 320.0f, 64.0f, 64.0f);
    }

    if (D_80200B60 > 320.0f) {
        func_8002BD0C(0x57, D_80126578 - 320.0f, 96.0f, 64.0f);
    }

    if (D_80200BFC > 320.0f) {
        func_8002BD0C(0x58, D_80126578 - 320.0f, 80.0f, 64.0f);
    }

    if (D_80200C98 > 320.0f) {
        func_8002BD0C(0x59, D_80126578 - 320.0f, 72.0f, 64.0f);
    }

    if (D_80200D34 > 320.0f) {
        func_8002BD0C(0x5A, D_80126578 - 320.0f, 108.f, 64.0f);
    }

    if (D_80200DD0 > 320.0f) {
        func_8002BD0C(0x5B, D_80126578 - 320.0f, 88.0f, 64.0f);
    }

    func_8002BE98(0x56, 0.2f, 0.0f, 0.0f);
    func_8002BE98(0x57, 0.1f, 0, 0);
    func_8002BE98(0x58, 0.3f, 0, 0);
    func_8002BE98(0x59, 0.4f, 0, 0);
    func_8002BE98(0x5A, 0.1f, 0, 0);
    func_8002BE98(0x5B, 0.2f, 0, 0);
    
}
