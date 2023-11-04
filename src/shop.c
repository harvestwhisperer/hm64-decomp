#include "common.h"

#include "shop.h"

#include "system/map.h"
#include "system/sprite.h"

#include "gameAudio.h"
#include "itemHandlers.h"
#include "mainLoop.h"
#include "overlayScreens.h"
#include "player.h"

// bss
extern u16 D_8023740A;

// rodata
extern u16 D_80118B70[];
extern u16 D_80118FD0[];
extern Vec3f D_80119040[];
extern void *D_801192E0[30][2];
extern u8 D_80119510[];
extern u8 D_80119548[];
extern u8 D_801195B8[];
extern u8 D_801194A0[];
extern u16 D_8011BA10[];

//INCLUDE_ASM(const s32, "shop", func_800DC750);

void func_800DC750(u8 arg0) {
    
    D_8023740C[1] = D_801195B8[arg0];
    D_8023740C[2] = arg0;
    D_8023740A = D_80118B70[arg0];
    D_8023740C[0] = 1;
    D_8023740C[3] = 0;
    
    setMainLoopCallbackFunctionIndex(SHOP_DIALOGUE);
}

//INCLUDE_ASM(const s32, "shop", func_800DC7BC);

void func_800DC7BC(u8 index) {
    
    func_8002B138(D_801194A0[index] + 0x62, &holdableItemsTexturesStart, &holdableItemsTexturesEnd, &holdableItemsPaletteStart, &holdableItemsPaletteEnd, &holdableItemsTableStart, &holdableItemsTableEnd, D_801192E0[index][0], D_801192E0[index][1], 0x8028DD50, 0x80290550, 0x80293A50, 0x80293C50, 1, 1);
    func_8002BD90(D_801194A0[index] + 0x62, 1.0f, 1.0f, 1.0f);
    func_8002C7EC(D_801194A0[index] + 0x62, 3);
    func_8002C85C(D_801194A0[index] + 0x62, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002C914(D_801194A0[index] + 0x62, 0xFF, 0xFF, 0xFF, 0xFF);

    func_80034C40(0, D_801194A0[index], D_801194A0[index] + 0x62, func_80030BA0(&D_8011BA10, func_800D5A88(D_80118FD0[index])), D_80119040[index].x, D_80119040[index].y, D_80119040[index].z, 0xFF, 0xFF, 0, 0);
}

//INCLUDE_ASM(const s32, "shop", func_800DC9C0);

void func_800DC9C0(u8 index) {

    if (D_801194A0[index] != 0xFF) {
        func_80034E64(0, D_801194A0[index]);
    }
}

//INCLUDE_ASM(const s32, "shop", func_800DC9FC);

void func_800DC9FC(u8 arg0) {
    
    gPlayer.unk_6B = arg0;

    if (func_800DCAA0(arg0)) {

        gPlayer.unk_2D = func_800D5308(D_80119548[arg0], 6, D_80118FD0[arg0], 0, 0);
        setAudio(0x24);

        if (D_801194A0[arg0] != 0xFF) {
            func_80034E64(0, D_801194A0[arg0]);
        }
    }

    setPlayerAction(0xB, 0xD);
}

//INCLUDE_ASM(const s32, "shop", func_800DCAA0);

u8 func_800DCAA0(u8 index) {
    return D_80119510[index];
}

// jtbl_80123480
INCLUDE_ASM(const s32, "shop", func_800DCAB8);

// jtbl_80123498
INCLUDE_ASM(const s32, "shop", func_800DD44C);

// animals/ranch store
// jtbl_80123578
INCLUDE_ASM(const s32, "shop", func_800DDDFC);
