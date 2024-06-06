#include "common.h"

#include "overlayScreens.h"

#include "system/pauseScreen.h"
#include "system/sprite.h"

#include "animals.h"
#include "game.h"
#include "gameStatus.h"
#include "initialize2.h"
#include "mainLoop.h"
#include "player.h"
#include "shop.h"
#include "tv.h"

// bss
// struct
// cell index
extern u8 D_801FB6F0;
// previous cell index
extern u8 D_801FB6F1;
// page number
extern u8 D_801FB6F2;
extern u8 D_801FB6F3;
extern u8 D_801FB6F6;
extern u8 D_801FB6F8;
extern u8 D_801FB6F9;
extern u8 D_801FB6FA;
extern u8 D_801FB6FB;
 
// shared
extern u8 D_801C3F70;
extern u8 D_80205636;

// data
extern f32 D_80116D80[];
extern f32 D_80116D90[];
extern f32 D_80116DA0[];
extern f32 D_80116DB0[];
extern f32 D_80116DC0[5];
extern f32 D_80116DD4[5];

// TODO: add linker symbols
// money
extern void *D_CFE610;
extern void *D_CFE610_2;
extern void *D_CFF6F0;
extern void *D_CFF6F0_2;
extern void *D_CFF6F0_3;
extern void *D_CFF6F0_4;
extern void *D_CFF710;

extern void *D_D3BFE0;
extern void *D_D3DCC0;
extern void *D_D3DCC0_2;
extern void *D_D3DCE0;

// forward declarations
void func_800B4160(void);
void func_800C81C8(void);
void func_800C7DF8(void);
void func_800C7E18(void);
void func_800C7E58(void);
void func_800C7E38(void);


// rodata

/*
// const double D_80121C90 = 0.5;
const u8 D_80121E34[16] = { 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x2D, 0x2E }
const u8 D_801220CC[][];
const u8 D_80122140[][];
const u16 D_801221CC[3] = { 0xBBB, 0x3E8, 0x1F4 }
*/


//INCLUDE_ASM(const s32, "overlayScreens", func_800B2CE0);

void func_800B2CE0(void) {

    u8 i, j, k;

    func_800C81A8();
    
    i = 0;
    
    func_800C81C8();
    func_800C7DF8();
    func_800C7E18();
    func_800C7E58();
    func_800C7E38();

    func_8002B6B8(0x82);
    func_8002B6B8(0x83);
    func_8002B6B8(0x84);
    func_8002B6B8(0x85);
    func_8002B6B8(0x86);
    func_8002B6B8(0x87);
    func_8002B6B8(0x88);
    func_8002B6B8(0x81);
    func_8002BAD8(0x79);
    func_8002B6B8(0x89);
    func_8002B6B8(0x8C);
    func_8002B6B8(0x8D);
    func_8002B6B8(0x8B);
    func_8002B6B8(0x8A);
    func_80046120(0);
    func_8002B6B8(0x8E);
    func_80046120(1);
    func_8002B6B8(0x86);
    func_8002B6B8(0x82);
    func_8002B6B8(0x83);
    func_8002B6B8(0x84);
    func_8002B6B8(0x85);
    func_8002B6B8(0xB9);
    func_8002B6B8(0xBA);
    func_8002B6B8(0xBB);
    func_8002B6B8(0xBC);
    func_8002B6B8(0xBD);
    func_8002B6B8(0xBE);
    func_8002B6B8(0x82);
    func_8002B6B8(0x83);
    func_8002B6B8(0x84);
    func_8002B6B8(0x85);
    func_8002B6B8(0x86);
    func_80046120(0);
    func_80046120(2);
    func_80046120(3);
    func_80046120(4);
    func_80046120(5);
    func_80046120(6);
    func_80046120(2);
    func_80046120(3);
    func_80046120(4);
    func_80046120(5);
    func_80046120(6);
    func_80046120(7);
    func_80046120(8);
    func_80046120(9);
    func_80046120(0xA);
    func_80046120(0xB);
    func_80046120(0xC);
    func_80046120(0xD);
    func_80046120(0xE);

    for (i = 0; i < 0x28; i++) {
        func_8002B6B8(i + 0xA3);
    }

    for (j = 0; j < 0x1E; j++) {
        func_8002B6B8(j + 0x8F);
    }

    for (k = 0; k < 0xA; k++) {
        func_8002B6B8(k + 0xAD);
    }
}

INCLUDE_ASM(const s32, "overlayScreens", func_800B2F34);

INCLUDE_ASM(const s32, "overlayScreens", func_800B3498);

INCLUDE_ASM(const s32, "overlayScreens", func_800B3694);

//INCLUDE_ASM(const s32, "overlayScreens", func_800B3A60);

void func_800B3A60(void) {

    func_8002BAD8(0x82);
    func_8002BAD8(0x83);
    func_8002BAD8(0x84);
    func_8002BAD8(0x85);
    
    switch (tvContext.channelIndex) {  

        case 0:
            func_8002B80C(0x82, 0, 2);
            func_8002B80C(0x83, 0, 3);
            func_8002B80C(0x84, 0, 5);
            func_8002B80C(0x85, 0, 7);
            break;
        case 1:
            func_8002B80C(0x82, 0, 1);
            func_8002B80C(0x83, 0, 4);
            func_8002B80C(0x84, 0, 5);
            func_8002B80C(0x85, 0, 7);
            break;
        case 2:
            func_8002B80C(0x82, 0, 1);
            func_8002B80C(0x83, 0, 3);
            func_8002B80C(0x84, 0, 6);
            func_8002B80C(0x85, 0, 7);
            break;
        case 3:
            func_8002B80C(0x82, 0, 1);
            func_8002B80C(0x83, 0, 3);
            func_8002B80C(0x84, 0, 5);
            func_8002B80C(0x85, 0, 8);
            break;
        }

}

// FIXME: probably not right
static inline inline_func() {
    func_8002BD0C(0x8E, 0, 0, 256.0f);
}

//INCLUDE_ASM(const s32, "overlayScreens", func_800B3BD8);

// money sprites
// TODO: add linker symbols
void func_800B3BD8(void) {

    u32 temp = gGold;

    func_8002B138(0x8E, &D_CFE610, &D_CFF6F0, &D_CFF6F0_2, &D_CFF710, 0, 0, 0x80253B00, 0, 0x80254A00, 0x80254E00, 0x80254F00, 0, 0, 0);
    inline_func();
    func_8002CB24(0x8E, 1);
    func_8002C914(0x8E, 0xFF, 0xFF, 0xFF, 0);
    func_8002C7EC(0x8E, 2);
    func_8002B80C(0x8E, 0, 0);
    
    func_80045E20(1, 0x91, &D_CFE610_2, &D_CFF6F0_3, &D_CFF6F0_4, &D_CFF710, 0x80253B00, 0x80254A00, 0x80254E00, 0x80254F00, 0, 1, 0, 0, 0, 320.0f, 0xA);
    func_80045F5C(1, temp, 5, 3);
    func_800461D8(1, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002CAA8(0x8E, 0xFF);
    
    func_8002B138(0x82, &D_D3BFE0, &D_D3DCC0, &D_D3DCC0_2, &D_D3DCE0, 0, 0, 0x802E0EC0, 0, 0x802E27C0, 0x802E28C0, 0x802E2BC0, 0, 0, 0);
    func_8002BD0C(0x82, -96.0f, 64.0f, 256.0f);
    func_8002CB24(0x82, 1);
    func_8002C914(0x82, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002B80C(0x82, 0, 0);
    
    func_8002B138(0x83, &D_D3BFE0, &D_D3DCC0, &D_D3DCC0_2, &D_D3DCE0, 0, 0, 0x802E0EC0, 0, 0x802E27C0, 0x802E28C0, 0x802E2BC0, 0, 0, 0);
    func_8002BD0C(0x83, -56.0f, 64.0f, 256.0f);
    func_8002CB24(0x83, 1);
    func_8002C914(0x83, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002B80C(0x83, 1, 0);
    
    func_8002B138(0x84, &D_D3BFE0, &D_D3DCC0, &D_D3DCC0_2, &D_D3DCE0, 0, 0, 0x802E0EC0, 0, 0x802E27C0, 0x802E28C0, 0x802E2BC0, 0, 0, 0);
    func_8002CB24(0x84, 1);
    func_8002C914(0x84, 0xFF, 0xFF, 0xFF, 0xFF);
    
    func_8002B138(0x85, &D_D3BFE0, &D_D3DCC0, &D_D3DCC0_2, &D_D3DCE0, 0, 0, 0x802E0EC0, 0, 0x802E27C0, 0x802E28C0, 0x802E2BC0, 0, 0, 0);
    func_8002BD0C(0x85, -12.0f, 13.0f, 256.0f);
    func_8002CB24(0x85, 1);
    func_8002C914(0x85, 0xFF, 0xFF, 0xFF, 0xFF);
    
    func_8002B138(0x86, &D_D3BFE0, &D_D3DCC0, &D_D3DCC0_2, &D_D3DCE0, 0, 0, 0x802E0EC0, 0, 0x802E27C0, 0x802E28C0, 0x802E2BC0, 0, 0, 0);
    func_8002BD0C(0x86, -12.0f, 13.0f, 256.0f);
    func_8002CB24(0x86, 1);
    func_8002C914(0x86, 0xFF, 0xFF, 0xFF, 0xFF);
    
    func_8002B138(0x87, &D_D3BFE0, &D_D3DCC0, &D_D3DCC0_2, &D_D3DCE0, 0, 0, 0x802E0EC0, 0, 0x802E27C0, 0x802E28C0, 0x802E2BC0, 0, 0, 0);
    func_8002BD0C(0x87, -16.0f, 11.0f, 224.0f);
    func_8002CB24(0x87, 1);
    func_8002C914(0x87, 0xFF, 0xFF, 0xFF, 0xFF);
    func_800B4160();

}

//INCLUDE_ASM(const s32, "overlayScreens", func_800B4160);

void func_800B4160(void) {

    // shop struct
    if (!D_8023740F) {
        func_8002BAD8(0x82);
        func_8002BAD8(0x83);
        func_8002BAD8(0x84);
        func_8002BD0C(0x84, -96.0f, 64.0f, 256.0f);
        func_8002B80C(0x82, 7, 0xFE);
        func_8002B80C(0x83, 1, 0);
        func_8002B80C(0x84, 5, 0);
    } else {
        func_8002BAD8(0x82);
        func_8002BAD8(0x83);
        func_8002BAD8(0x84);
        func_8002BD0C(0x84, -56.0f, 64.0f, 256.0f);
        func_8002B80C(0x82, 0, 0);
        func_8002B80C(0x83, 8, 0xFE);
        func_8002B80C(0x84, 6, 0);
    }
}

//INCLUDE_ASM(const s32, "overlayScreens", func_800B4238);

void func_800B4238(u8 arg0) {
    
    func_8002B80C(0x87, 9, 0xFE);
    
    if (D_8023740C[0] && D_8023740C[0] < arg0) {
        func_8002B80C(0x85, 3, 0);
    } else {
        func_8002BAD8(0x85);
    }
    
    if (D_8023740C[0] >= 2 && arg0 >= D_8023740C[0]) {
        func_8002B80C(0x86, 4, 0);
        return;
    }
    
    func_8002BAD8(0x86);
}

INCLUDE_ASM(const s32, "overlayScreens", func_800B42E0);

//INCLUDE_ASM(const s32, "overlayScreens", func_800B59AC);

// adjust sprite colors
void func_800B59AC(void) {
    func_8002C1C0(0x80, 0xFF, 0xFF, 0xFF, 0xFF, 0x18);
    func_8002C1C0(0x82, 0xFF, 0xFF, 0xFF, 0xFF, 0x18);
    func_8002C1C0(0x83, 0xFF, 0xFF, 0xFF, 0xFF, 0x18);
    func_8002C1C0(0x84, 0xFF, 0xFF, 0xFF, 0xFF, 0x18);
    func_8002C1C0(0xA3, 0xFF, 0xFF, 0xFF, 0xFF, 0x18);
    func_8002C1C0(0xA4, 0xFF, 0xFF, 0xFF, 0xFF, 0x18);
    func_8002C1C0(0xA5, 0xFF, 0xFF, 0xFF, 0xFF, 0x18);
    func_8002C1C0(0xA6, 0xFF, 0xFF, 0xFF, 0xFF, 0x18);
    func_8002C1C0(0xA7, 0xFF, 0xFF, 0xFF, 0xFF, 0x18);
    func_8002C1C0(0xA8, 0xFF, 0xFF, 0xFF, 0xFF, 0x18);
    func_8002C1C0(0xA9, 0xFF, 0xFF, 0xFF, 0xFF, 0x18);
    func_8002C1C0(0xAA, 0xFF, 0xFF, 0xFF, 0xFF, 0x18);
    func_8002C1C0(0x85, 0xFF, 0xFF, 0xFF, 0xFF, 0x18);
    func_8002C1C0(0x86, 0xFF, 0xFF, 0xFF, 0xFF, 0x18);
    func_8002C1C0(0x87, 0xFF, 0xFF, 0xFF, 0xFF, 0x18);
    func_8002C1C0(0x88, 0xFF, 0xFF, 0xFF, 0xFF, 0x18);
    func_8002C1C0(0xAB, 0xFF, 0xFF, 0xFF, 0xFF, 0x18);
    func_8002C1C0(0xAD, 0xFF, 0xFF, 0xFF, 0xFF, 0x18);
}

//INCLUDE_ASM(const s32, "overlayScreens", func_800B5BD4);

void func_800B5BD4(void) {
    func_8002C914(0x80, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002C914(0x82, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002C914(0x83, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002C914(0x84, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002C914(0xA3, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002C914(0xA4, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002C914(0xA5, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002C914(0xA6, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002C914(0xA7, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002C914(0xA8, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002C914(0xA9, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002C914(0xAA, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002C914(0x85, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002C914(0x86, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002C914(0x87, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002C914(0x88, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002C914(0xAB, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002C914(0xAD, 0xFF, 0xFF, 0xFF, 0xFF);
}

//INCLUDE_ASM(const s32, "overlayScreens", func_800B5DA8);

void func_800B5DA8(void) {
    func_8002C1C0(0x80, 0, 0, 0, 0, 0x18);
    func_8002C1C0(0x82, 0, 0, 0, 0, 0x18);
    func_8002C1C0(0x83, 0, 0, 0, 0, 0x18);
    func_8002C1C0(0x84, 0, 0, 0, 0, 0x18);
    func_8002C1C0(0xA3, 0, 0, 0, 0, 0x18);
    func_8002C1C0(0xA4, 0, 0, 0, 0, 0x18);
    func_8002C1C0(0xA5, 0, 0, 0, 0, 0x18);
    func_8002C1C0(0xA6, 0, 0, 0, 0, 0x18);
    func_8002C1C0(0xA7, 0, 0, 0, 0, 0x18);
    func_8002C1C0(0xA8, 0, 0, 0, 0, 0x18);
    func_8002C1C0(0xA9, 0, 0, 0, 0, 0x18);
    func_8002C1C0(0xAA, 0, 0, 0, 0, 0x18);
    func_8002C1C0(0x85, 0, 0, 0, 0, 0x18);
    func_8002C1C0(0x86, 0, 0, 0, 0, 0x18);
    func_8002C1C0(0x87, 0, 0, 0, 0, 0x18);
    func_8002C1C0(0x88, 0, 0, 0, 0, 0x18);
    func_8002C1C0(0xAB, 0, 0, 0, 0, 0x18);
    func_8002C1C0(0xAD, 0, 0, 0, 0, 0x18);
}

//INCLUDE_ASM(const s32, "overlayScreens", func_800B5FC4);

void func_800B5FC4(u8 arg0, u8 arg1, u8 arg2) {

    // FIXME: shouldn't be necessary
    u8 temp;

    if (arg0 != 0) {
        
        if (arg0 != 1) {
            // FIXME: something wrong here
            temp = 0xB2;
            return;
        }

    } else {
        
        if (arg1 != 0xFF) {
            func_8002BD0C(0x79, D_80116D80[arg1], D_80116D90[arg1], 32.0f);
        }

        if (arg2 != 0xFF) {
            func_8002BD0C(0x79, D_80116DA0[arg2], D_80116DB0[arg2], 32.0f);
        }
        
        func_8002B80C(0x79, 3, 0);
        return;
    }
    
    if (arg1 != 0xFF) {
        func_8002BD0C(0xB2, D_80116D80[arg1], D_80116D90[arg1], 32.0f);
    }

    if (arg2 != 0xFF) {
        func_8002BD0C(0xB2, arg2, arg2, 32.0f);
    }
    
    // can't remove this without messing up control flow
    temp = 0xB2;

    func_8002B80C(0xB2, 3, 0);
}

INCLUDE_ASM(const s32, "overlayScreens", func_800B60E4);

INCLUDE_ASM(const s32, "overlayScreens", func_800B64E4);

INCLUDE_ASM(const s32, "overlayScreens", func_800B6674);

INCLUDE_ASM(const s32, "overlayScreens", func_800B7070);

INCLUDE_ASM(const s32, "overlayScreens", func_800B7B34);

INCLUDE_ASM(const s32, "overlayScreens", func_800B7CB4);

INCLUDE_ASM(const s32, "overlayScreens", func_800B7E28);

//INCLUDE_ASM(const s32, "overlayScreens", func_800B7F6C);

void func_800B7F6C(u8 arg0) {

    if (arg0 != 0xFF) {
        func_8002BD0C(0x79, D_80116DC0[arg0], D_80116DD4[arg0], 32.0f);
    }

    func_8002B80C(0x79, 3, 0);
}

//INCLUDE_ASM(const s32, "overlayScreens", func_800B7FC8);

void func_800B7FC8(u8 arg0, u8 arg1) {

    if (arg0 != 0xFF) {
        if (arg1) {
            func_8002CB88(arg0 + 0x83, 1);
        } else {
            func_8002CB88(arg0+ 0x83, 0xF);
        }
    }
}

//INCLUDE_ASM(const s32, "overlayScreens", func_800B8018);

// estimate/evaluation screen
void func_800B8018(void) {
    
    func_800593EC();
    func_8002B138(0x80, &_checkerboardBackgroundTextureSegmentRomStart, &_checkerboardBackgroundTextureSegmentRomEnd, &_checkerboardBackgroundIndexSegmentRomStart, &_checkerboardBackgroundIndexSegmentRomEnd, 0, 0, 0x802DE5C0, 0, 0x802E0BC0, 0x802E0CC0, 0x802E0DC0, 0, 0, 0);
    func_8002BD0C(0x80, 0, 0, 0);
    func_8002BD90(0x80, 2.0f, 2.0f, 1.0f);
    func_8002CB24(0x80, 1);
    func_8002C914(0x80, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002B80C(0x80, 0, 0); 
     
    D_801FB6F0 = 0;
    D_801FB6F1 = 0;
    D_801FB6F2 = 0;
    D_801FB6F3 = 0xFF;
    D_801FB6F6 = 0;
    
    setMainLoopCallbackFunctionIndex(ESTIMATE);
}

//INCLUDE_ASM(const s32, "overlayScreens", func_800B815C);

// kitchen picture
void func_800B815C(void) {

    func_800593EC();
    func_8002B138(0x80, &_checkerboardBackgroundTextureSegmentRomStart, &_checkerboardBackgroundTextureSegmentRomEnd, &_checkerboardBackgroundIndexSegmentRomStart, &_checkerboardBackgroundIndexSegmentRomEnd, NULL, NULL, (void*)0x802DE5C0, NULL, (void*)0x802E0BC0, (void*)0x802E0CC0, (void*)0x802E0DC0, NULL, NULL, 0);
    func_8002BD0C(0x80, 0.0f, 0.0f, 0.0f);
    func_8002BD90(0x80, 2.0f, 2.0f, 1.0f);
    func_8002CB24(0x80, 1);
    func_8002C914(0x80, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002B80C(0x80, 0, 0);
    
    D_801FB6F0 = func_800C2780(0, 0);
    D_801FB6F1 = 0;
    D_801FB6F2 = 0;
    D_801FB6F3 = 0xFF;
    D_801FB6F6 = 0;
    
    setMainLoopCallbackFunctionIndex(KITCHEN_PICTURE);
}


//INCLUDE_ASM(const s32, "overlayScreens", func_800B82AC);

// home extensions selection
void func_800B82AC(void) {

    func_800593EC();
    func_8002B138(0x80, &_checkerboardBackgroundTextureSegmentRomStart, &_checkerboardBackgroundTextureSegmentRomEnd, &_checkerboardBackgroundIndexSegmentRomStart, &_checkerboardBackgroundIndexSegmentRomEnd, NULL, NULL, (void*)0x802DE5C0, NULL, (void*)0x802E0BC0, (void*)0x802E0CC0, (void*)0x802E0DC0, NULL, NULL, 0);
    func_8002BD0C(0x80, 0.0f, 0.0f, 0.0f);
    func_8002BD90(0x80, 2.0f, 2.0f, 1.0f);
    func_8002CB24(0x80, 1);
    func_8002C914(0x80, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002B80C(0x80, 0, 0);
    
    D_801FB6F0 = 0;
    D_801FB6F1 = 0;
    D_801FB6F2 = 0;
    D_801FB6F3 = 0xFF;
    D_801FB6F6 = 0;
    
    setMainLoopCallbackFunctionIndex(HOME_EXTENSIONS_SELECT);

}

//INCLUDE_ASM(const s32, "overlayScreens", func_800B83F0);

// calendar
void func_800B83F0(void) {

    func_800593EC();
    func_8002B138(0x80, &_checkerboardBackgroundTextureSegmentRomStart, &_checkerboardBackgroundTextureSegmentRomEnd, &_checkerboardBackgroundIndexSegmentRomStart, &_checkerboardBackgroundIndexSegmentRomEnd, NULL, NULL, (void*)0x802DE5C0, NULL, (void*)0x802E0BC0, (void*)0x802E0CC0, (void*)0x802E0DC0, NULL, NULL, 0);
    func_8002BD0C(0x80, 0.0f, 0.0f, 0.0f);
    func_8002BD90(0x80, 2.0f, 2.0f, 1.0f);
    func_8002CB24(0x80, 1);
    func_8002C914(0x80, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002B80C(0x80, 0, 0);

    D_801FB6F0 = 0;
    D_801FB6F1 = 0;
    D_801FB6F2 = 0;
    D_801FB6F3 = 0xFF;
    D_801FB6F6 = 0;
    D_801FB6F8 = 0;
    D_801FB6F9 = 0;
    D_801FB6FA = 0;
    D_801FB6FB = 0;

    setMainLoopCallbackFunctionIndex(CALENDAR);
}

//INCLUDE_ASM(const s32, "overlayScreens", func_800B8554);

// recipe book
void func_800B8554(void) {
    
    func_800593EC();
    func_8002B138(0x80, &_checkerboardBackgroundTextureSegmentRomStart, &_checkerboardBackgroundTextureSegmentRomEnd, &_checkerboardBackgroundIndexSegmentRomStart, &_checkerboardBackgroundIndexSegmentRomEnd, NULL, NULL, (void*)0x802DE5C0, NULL, (void*)0x802E0BC0, (void*)0x802E0CC0, (void*)0x802E0DC0, NULL, NULL, 0);
    func_8002BD0C(0x80, 0.0f, 0.0f, 0.0f);
    func_8002BD90(0x80, 2.0f, 2.0f, 1.0f);
    func_8002CB24(0x80, 1);
    func_8002C914(0x80, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002B80C(0x80, 0, 0);

    D_801FB6F0 = 0;
    D_801FB6F1 = 0;
    D_801FB6F2 = 0;
    D_801FB6F3 = 0xFF;
    D_801FB6F6 = 0;
    D_801FB6F8 = 0;
    D_801FB6F9 = 0;
    D_801FB6FA = 0;
    D_801FB6FB = 0;

    setMainLoopCallbackFunctionIndex(RECIPE_BOOK);
}

INCLUDE_ASM(const s32, "overlayScreens", func_800B86B8);

//INCLUDE_ASM(const s32, "overlayScreens", func_800B881C);

// prepare pause screen
void func_800B881C(void) {
    func_800593EC();
    func_800B8844();
}

// pause screen
INCLUDE_ASM(const s32, "overlayScreens", func_800B8844);

// pause screen assets
INCLUDE_ASM(const s32, "overlayScreens", func_800B8980);

INCLUDE_ASM(const s32, "overlayScreens", func_800B90E0);

INCLUDE_ASM(const s32, "overlayScreens", func_800B9914);

INCLUDE_ASM(const s32, "overlayScreens", func_800B99EC);

INCLUDE_ASM(const s32, "overlayScreens", func_800B9B8C);

// jtbl_80121C98
// pause screen main loop callback
INCLUDE_ASM(const s32, "overlayScreens", func_800B9D3C);

INCLUDE_ASM(const s32, "overlayScreens", func_800BA928);

INCLUDE_ASM(const s32, "overlayScreens", func_800BA9E8);

INCLUDE_ASM(const s32, "overlayScreens", func_800BAAA0);

INCLUDE_ASM(const s32, "overlayScreens", func_800BAC7C);

INCLUDE_ASM(const s32, "overlayScreens", func_800BADD0);

// pause screen
INCLUDE_ASM(const s32, "overlayScreens", func_800BAF1C);

INCLUDE_ASM(const s32, "overlayScreens", func_800BB590);

INCLUDE_ASM(const s32, "overlayScreens", func_800BBAF0);

INCLUDE_ASM(const s32, "overlayScreens", func_800BBBC8);

INCLUDE_ASM(const s32, "overlayScreens", func_800BBD34);

// jtbl_80121CC0
// toolbox callback
INCLUDE_ASM(const s32, "overlayScreens", func_800BBEC0);

INCLUDE_ASM(const s32, "overlayScreens", func_800BC700);

INCLUDE_ASM(const s32, "overlayScreens", func_800BC770);

// rucksack
INCLUDE_ASM(const s32, "overlayScreens", func_800BC7D8);

INCLUDE_ASM(const s32, "overlayScreens", func_800BC918);

INCLUDE_ASM(const s32, "overlayScreens", func_800BCA9C);

INCLUDE_ASM(const s32, "overlayScreens", func_800BCBE8);

INCLUDE_ASM(const s32, "overlayScreens", func_800BD25C);

INCLUDE_ASM(const s32, "overlayScreens", func_800BD754);

INCLUDE_ASM(const s32, "overlayScreens", func_800BD82C);

INCLUDE_ASM(const s32, "overlayScreens", func_800BD998);

// jtbl_80121CE0
INCLUDE_ASM(const s32, "overlayScreens", func_800BDB24);

INCLUDE_ASM(const s32, "overlayScreens", func_800BE3B8);

INCLUDE_ASM(const s32, "overlayScreens", func_800BE428);

INCLUDE_ASM(const s32, "overlayScreens", func_800BE490);

INCLUDE_ASM(const s32, "overlayScreens", func_800BE5D0);

INCLUDE_ASM(const s32, "overlayScreens", func_800BE684);

INCLUDE_ASM(const s32, "overlayScreens", func_800BE808);

INCLUDE_ASM(const s32, "overlayScreens", func_800BE954);

INCLUDE_ASM(const s32, "overlayScreens", func_800BF0C8);

INCLUDE_ASM(const s32, "overlayScreens", func_800BF5C0);

INCLUDE_ASM(const s32, "overlayScreens", func_800BF698);

INCLUDE_ASM(const s32, "overlayScreens", func_800BF804);

INCLUDE_ASM(const s32, "overlayScreens", func_800BF990);

INCLUDE_ASM(const s32, "overlayScreens", func_800C0238);

INCLUDE_ASM(const s32, "overlayScreens", func_800C02A8);

INCLUDE_ASM(const s32, "overlayScreens", func_800C0310);

INCLUDE_ASM(const s32, "overlayScreens", func_800C0450);

INCLUDE_ASM(const s32, "overlayScreens", func_800C0504);

// jtbl_80121D00
// player struct unk_1F
INCLUDE_ASM(const s32, "overlayScreens", func_800C0688);

// house extension selection
INCLUDE_ASM(const s32, "overlayScreens", func_800C0714);

//INCLUDE_ASM(const s32, "overlayScreens", func_800C0F24);

void func_800C0F24(void) {
    func_8002C52C(0x89, 0xFF, 0x18);
    func_8002C52C(0x8C, 0xFF, 0x18);
    func_8002C52C(0x8D, 0xFF, 0x18);
    func_8002C52C(0x8B, 0xFF, 0x18);
    func_8002C52C(0x8A, 0xFF, 0x18);
    func_8002C52C(0x8E, 0xFF, 0x18);
    func_8002C52C(0x81, 0xFF, 0x18);
    func_8002C52C(0x82, 0xFF, 0x18);
    func_8002C52C(0x83, 0xFF, 0x18);
    func_8002C52C(0x84, 0xFF, 0x18);
    func_8002C52C(0x85, 0xFF, 0x18);
    func_8002C52C(0x86, 0xFF, 0x18);
    func_8002C52C(0x87, 0xFF, 0x18);
    func_8002C52C(0x88, 0xFF, 0x18);
}

//INCLUDE_ASM(const s32, "overlayScreens", func_800C101C);

void func_800C101C(void) {
    func_8002C52C(0x89, 0, 0x18);
    func_8002C52C(0x8C, 0, 0x18);
    func_8002C52C(0x8D, 0, 0x18);
    func_8002C52C(0x8B, 0, 0x18);
    func_8002C52C(0x8A, 0, 0x18);
    func_8002C52C(0x8E, 0, 0x18);
    func_8002C52C(0x81, 0, 0x18);
    func_8002C52C(0x82, 0, 0x18);
    func_8002C52C(0x83, 0, 0x18);
    func_8002C52C(0x84, 0, 0x18);
    func_8002C52C(0x85, 0, 0x18);
    func_8002C52C(0x86, 0, 0x18);
    func_8002C52C(0x87, 0, 0x18);
    func_8002C52C(0x88, 0, 0x18);
    func_80046120(0);
    func_80046120(1);
}

//INCLUDE_RODATA(const s32, "overlayScreens", D_80121D74);

static const u16 D_80121D74[6] = { 0x10, 0x11, 0x12, 0x13, 0x14, 0x15};

// rodata: D_80121D74
// jtbl_80121D80
// house extension selection callback
INCLUDE_ASM(const s32, "overlayScreens", func_800C1124);

// estimate screen
INCLUDE_ASM(const s32, "overlayScreens", func_800C1710);

INCLUDE_ASM(const s32, "overlayScreens", func_800C2060);

INCLUDE_ASM(const s32, "overlayScreens", func_800C2128);

// jtbl_80121DA0
INCLUDE_ASM(const s32, "overlayScreens", func_800C224C);

INCLUDE_ASM(const s32, "overlayScreens", func_800C2364);

INCLUDE_ASM(const s32, "overlayScreens", func_800C2504);

// rucksack
INCLUDE_ASM(const s32, "overlayScreens", func_800C2660);

INCLUDE_ASM(const s32, "overlayScreens", func_800C2780);

INCLUDE_ASM(const s32, "overlayScreens", func_800C292C);

INCLUDE_ASM(const s32, "overlayScreens", func_800C2A54);

// jtbl_80121DB8
INCLUDE_ASM(const s32, "overlayScreens", func_800C2B8C);

INCLUDE_ASM(const s32, "overlayScreens", func_800C2F50);

INCLUDE_ASM(const s32, "overlayScreens", func_800C3108);

INCLUDE_ASM(const s32, "overlayScreens", func_800C34C0);

INCLUDE_ASM(const s32, "overlayScreens", func_800C39A0);

INCLUDE_ASM(const s32, "overlayScreens", func_800C3B1C);

// jtbl_80121DD0
INCLUDE_ASM(const s32, "overlayScreens", func_800C3D20);

INCLUDE_ASM(const s32, "overlayScreens", func_800C4A74);

INCLUDE_ASM(const s32, "overlayScreens", func_800C4E90);

INCLUDE_ASM(const s32, "overlayScreens", func_800C5080);

INCLUDE_ASM(const s32, "overlayScreens", func_800C5218);

// jtbl_80121DF0
INCLUDE_ASM(const s32, "overlayScreens", func_800C53C0);

INCLUDE_ASM(const s32, "overlayScreens", func_800C5AA4);

// jtbl_80121E08
// jtbl_80121E20
INCLUDE_ASM(const s32, "overlayScreens", func_800C5DA4);

INCLUDE_ASM(const s32, "overlayScreens", func_800C6E98);

INCLUDE_ASM(const s32, "overlayScreens", func_800C6F70);

INCLUDE_RODATA(const s32, "overlayScreens", D_80121E34);

//static const u8 D_80121E34[20] = { 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x2D, 0x2E, 0, 0, 0, 0 };

// static const u32 D_80121E34[4] = { 0x15161718, 0x191A1B1C, 0x1D1E1F20, 0x21222D2E, 0 };

// D_80121E34
// jtbl_80121E48
INCLUDE_ASM(const s32, "overlayScreens", func_800C7058);

INCLUDE_ASM(const s32, "overlayScreens", addRecipe);

INCLUDE_ASM(const s32, "overlayScreens", func_800C77EC);

//INCLUDE_ASM(const s32, "overlayScreens", checkRecipe);

u32 checkRecipe(u16 bitIndex) {
    u32 temp = bitIndex;
    return recipesBits[temp >> 5] & (1 << (temp & 0x1F));
}

// possible split: 0xA2C50
INCLUDE_ASM(const s32, "overlayScreens", func_800C7850);

INCLUDE_ASM(const s32, "overlayScreens", func_800C7C88);

INCLUDE_ASM(const s32, "overlayScreens", func_800C7CE4);

INCLUDE_ASM(const s32, "overlayScreens", func_800C7D40);

INCLUDE_ASM(const s32, "overlayScreens", func_800C7D9C);

//INCLUDE_ASM(const s32, "overlayScreens", func_800C7DF8);

void func_800C7DF8(void) {
    func_8002BAD8(0x7C);
}

//INCLUDE_ASM(const s32, "overlayScreens", func_800C7E18);

void func_800C7E18(void) {
    func_8002BAD8(0x7D);
}

//INCLUDE_ASM(const s32, "overlayScreens", func_800C7E38);

void func_800C7E38(void) {
    func_8002BAD8(0x7F);
}

//INCLUDE_ASM(const s32, "overlayScreens", func_800C7E58);

void func_800C7E58(void) {
    func_8002BAD8(0x7E);
}

INCLUDE_ASM(const s32, "overlayScreens", func_800C7E78);

INCLUDE_ASM(const s32, "overlayScreens", func_800C80F0);

INCLUDE_ASM(const s32, "overlayScreens", func_800C814C);

INCLUDE_ASM(const s32, "overlayScreens", func_800C81A8);

//INCLUDE_ASM(const s32, "overlayScreens", func_800C81C8);

void func_800C81C8(void) {
    func_8002BAD8(0x7A);
}

// jtbl_80121E60
INCLUDE_ASM(const s32, "overlayScreens", func_800C81E8);

// jtbl_80121F28
INCLUDE_ASM(const s32, "overlayScreens", func_800C8424);

// jtbl_80122098
INCLUDE_ASM(const s32, "overlayScreens", func_800C8784);

INCLUDE_ASM(const s32, "overlayScreens", func_800C88F4);

INCLUDE_ASM(const s32, "overlayScreens", func_800C8B04);

INCLUDE_ASM(const s32, "overlayScreens", func_800C9FF0);

INCLUDE_ASM(const s32, "overlayScreens", func_800CA590);

INCLUDE_ASM(const s32, "overlayScreens", func_800CA678);

// jtbl_801220B0
INCLUDE_ASM(const s32, "overlayScreens", func_800CA808);

// horse race
// D_801220CC
INCLUDE_ASM(const s32, "overlayScreens", func_800CB0C4);

// D_80122140
INCLUDE_ASM(const s32, "overlayScreens", func_800CB8F0);

INCLUDE_ASM(const s32, "overlayScreens", func_800CC11C);

INCLUDE_ASM(const s32, "overlayScreens", func_800CC17C);

INCLUDE_ASM(const s32, "overlayScreens", func_800CC518);

INCLUDE_ASM(const s32, "overlayScreens", func_800CC6F0);

INCLUDE_ASM(const s32, "overlayScreens", func_800CCFC8);

INCLUDE_ASM(const s32, "overlayScreens", func_800CD628);

INCLUDE_ASM(const s32, "overlayScreens", func_800CD690);

INCLUDE_RODATA(const s32, "overlayScreens", D_801220CC);

INCLUDE_RODATA(const s32, "overlayScreens", D_80122140);

// jtbl_801221B8
INCLUDE_ASM(const s32, "overlayScreens", func_800CD750);

INCLUDE_ASM(const s32, "overlayScreens", func_800CD928);

INCLUDE_ASM(const s32, "overlayScreens", func_800CDA6C);

INCLUDE_ASM(const s32, "overlayScreens", func_800CDFB0);

//INCLUDE_ASM(const s32, "overlayScreens", func_800CE008);

void func_800CE008(void) {
    func_8002C52C(0x81, 0, 0x18);
    func_8002C52C(0x82, 0, 0x18);
    func_8002C52C(0x83, 0, 0x18);
    func_8002C52C(0x84, 0, 0x18);
    func_80046120(2);
}

INCLUDE_RODATA(const s32, "overlayScreens", D_801221CC);

// D_801221CC
// jtbl_801221D8
INCLUDE_ASM(const s32, "overlayScreens", func_800CE068);

INCLUDE_ASM(const s32, "overlayScreens", func_800CE714);

//INCLUDE_ASM(const s32, "overlayScreens", func_800CE7A8);

void func_800CE7A8(u8 arg0) {

    switch (arg0) {                              
        case 0:
            setLifeEventBit(0x56);
            adjustHorseAffection(10);
            break;
        case 1:
            setLifeEventBit(0x58);
            break;
        case 2:
            acquireKeyItem(STAMINA_CARROT);
            break;
        }
}

//INCLUDE_ASM(const s32, "overlayScreens", func_800CE828);

bool func_800CE828(u8 arg0) {

    bool result = 0;

    switch (arg0) {
        case 0:
            if (checkLifeEventBit(0x57)) {
                result = 1;
            }
            break;
        case 1:
            if (checkLifeEventBit(0x59)) {
                result = 1;
            }
            break;
        case 2:
            break;
    }

    return result;
    
}

INCLUDE_ASM(const s32, "overlayScreens", func_800CE894);

INCLUDE_ASM(const s32, "overlayScreens", func_800CE930);

INCLUDE_ASM(const s32, "overlayScreens", func_800CEA74);

//INCLUDE_ASM(const s32, "overlayScreens", func_800CED60);

void func_800CED60(void) {
    func_8002C52C(0x81, 0xFF, 0x18);
    func_8002C52C(0x82, 0xFF, 0x18);
    func_8002C52C(0x83, 0xFF, 0x18);
}

//INCLUDE_ASM(const s32, "overlayScreens", func_800CEDA8);

void func_800CEDA8(void) {
    func_8002C52C(0x81, 0, 0x18);
    func_8002C52C(0x82, 0, 0x18);
    func_8002C52C(0x83, 0, 0x18);
}

// jtbl_80122200
// jtbl_80122220
// jtbl_80122238
INCLUDE_ASM(const s32, "overlayScreens", func_800CEDF0);

// jtbl_80122250
//INCLUDE_ASM(const s32, "overlayScreens", func_800CF34C);

bool func_800CF34C(u8 arg0) {

    bool result = 0;
    
    switch (arg0) {
        case 3:
        case 4:
            break;
        case 0:
            if (checkLifeEventBit(0x5A)) {
                result = 1;
            }
            break;
        case 1:
            if (checkHaveKeyItem(FLOWER_BATH_CRYSTALS)) {
                result = 1;
            }
            break;
        case 2:
            if (checkLifeEventBit(0x5B)) {
                result = 1;
            }
            break;
    }

    return result;
    
}

// jtbl_80122268
//INCLUDE_ASM(const s32, "overlayScreens", func_800CF3C4);

void func_800CF3C4(u8 arg0) {

    switch (arg0) {
        case 0:
            setLifeEventBit(0x5A);
            break;
        case 1:
            acquireKeyItem(0x14);
            break;
        case 2:
            setLifeEventBit(0x5B);
            break;
        case 3:
            func_80065BCC(0x12);
            D_80205636 += adjustValue(D_80205636, 1, 20);
            break;
        case 4:
            func_80065BCC(0x11);
            D_801C3F70 += adjustValue(D_801C3F70, 1, 20);
        default:
            break;
    }
} 

// jtbl_80122280
//INCLUDE_ASM(const s32, "overlayScreens", func_800CF498);

bool func_800CF498(u8 arg0) {

    bool result = 0;
    
    switch (arg0) {

        case 0:
            if (checkLifeEventBit(HAVE_TABLECLOTH)) {
                result = 1;
            }
            break;
 
        case 1:
             if (checkLifeEventBit(0x5C)) {
                 result = 1;
             }
            break;
        
        case 2:
             if (checkLifeEventBit(0x5D)) {
                 result = 1;
             }
            break;
        case 3:
             if (!checkHaveTool(EMPTY_BOTTLE)) {
                 result = 1;
             }
            break;
        case 4:
            break;
    }

    return result;
    
}

// jtbl_80122298
INCLUDE_ASM(const s32, "overlayScreens", func_800CF544);
