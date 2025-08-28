#include "common.h"

#include "game/overlayScreens.h"

#include "system/pauseScreen.h"
#include "system/globalSprites.h"

#include "game/animals.h"
#include "game/game.h"
#include "game/gameStatus.h"
#include "game/transition.h"
#include "game/itemHandlers.h"
#include "mainLoop.h"
#include "game/player.h"
#include "game/shop.h"
#include "game/tv.h"

// bss
extern OverlayScreenTable overlayScreenTable;
 
// shared bss
extern u8 D_801C3F70;
extern u8 D_80205636;

// data
extern f32 D_80116D80[];
extern f32 D_80116D90[];
extern f32 D_80116DA0[];
extern f32 D_80116DB0[];
extern f32 D_80116DC0[5];
extern f32 D_80116DD4[5];

// TODO: replace with linker symbols
// money sprites
// extern void *D_CFE610;
// extern void *D_CFE610_2;
// extern void *D_CFF6F0;
// extern void *D_CFF6F0_2;
// extern void *D_CFF6F0_3;
// extern void *D_CFF6F0_4;
// extern void *D_CFF710;

// extern void *D_D3BFE0;
// extern void *D_D3DCC0;
// extern void *D_D3DCC0_2;
// extern void *D_D3DCE0;

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


//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B2CE0);

void func_800B2CE0(void) {

    u8 i, j, k;

    func_800C81A8();
    
    i = 0;
    
    func_800C81C8();
    func_800C7DF8();
    func_800C7E18();
    func_800C7E58();
    func_800C7E38();

    deactivateSprite(0x82);
    deactivateSprite(0x83);
    deactivateSprite(0x84);
    deactivateSprite(0x85);
    deactivateSprite(0x86);
    deactivateSprite(0x87);
    deactivateSprite(0x88);
    deactivateSprite(0x81);
    resetAnimationState(0x79);

    deactivateSprite(0x89);
    deactivateSprite(0x8C);
    deactivateSprite(0x8D);
    deactivateSprite(0x8B);
    deactivateSprite(0x8A);
    deactivatePauseScreenSprites(0);

    deactivateSprite(0x8E);
    deactivatePauseScreenSprites(1);
    
    deactivateSprite(0x86);
    deactivateSprite(0x82);
    deactivateSprite(0x83);
    deactivateSprite(0x84);
    deactivateSprite(0x85);
    deactivateSprite(0xB9);
    deactivateSprite(0xBA);
    deactivateSprite(0xBB);
    deactivateSprite(0xBC);
    deactivateSprite(0xBD);
    deactivateSprite(0xBE);
    deactivateSprite(0x82);
    deactivateSprite(0x83);
    deactivateSprite(0x84);
    deactivateSprite(0x85);
    deactivateSprite(0x86);
    
    deactivatePauseScreenSprites(0);
    deactivatePauseScreenSprites(2);
    deactivatePauseScreenSprites(3);
    deactivatePauseScreenSprites(4);
    deactivatePauseScreenSprites(5);
    deactivatePauseScreenSprites(6);
    deactivatePauseScreenSprites(2);
    deactivatePauseScreenSprites(3);
    deactivatePauseScreenSprites(4);
    deactivatePauseScreenSprites(5);
    deactivatePauseScreenSprites(6);
    deactivatePauseScreenSprites(7);
    deactivatePauseScreenSprites(8);
    deactivatePauseScreenSprites(9);
    deactivatePauseScreenSprites(0xA);
    deactivatePauseScreenSprites(0xB);
    deactivatePauseScreenSprites(0xC);
    deactivatePauseScreenSprites(0xD);
    deactivatePauseScreenSprites(0xE);

    for (i = 0; i < 0x28; i++) {
        deactivateSprite(i + 0xA3);
    }

    for (j = 0; j < 0x1E; j++) {
        deactivateSprite(j + 0x8F);
    }

    for (k = 0; k < 0xA; k++) {
        deactivateSprite(k + 0xAD);
    }
}

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B2F34);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B3498);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B3694);

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B3A60);

void func_800B3A60(void) {

    resetAnimationState(0x82);
    resetAnimationState(0x83);
    resetAnimationState(0x84);
    resetAnimationState(0x85);
    
    switch (tvContext.channelIndex) {  

        case 0:
            startSpriteAnimation(0x82, 0, 2);
            startSpriteAnimation(0x83, 0, 3);
            startSpriteAnimation(0x84, 0, 5);
            startSpriteAnimation(0x85, 0, 7);
            break;
        case 1:
            startSpriteAnimation(0x82, 0, 1);
            startSpriteAnimation(0x83, 0, 4);
            startSpriteAnimation(0x84, 0, 5);
            startSpriteAnimation(0x85, 0, 7);
            break;
        case 2:
            startSpriteAnimation(0x82, 0, 1);
            startSpriteAnimation(0x83, 0, 3);
            startSpriteAnimation(0x84, 0, 6);
            startSpriteAnimation(0x85, 0, 7);
            break;
        case 3:
            startSpriteAnimation(0x82, 0, 1);
            startSpriteAnimation(0x83, 0, 3);
            startSpriteAnimation(0x84, 0, 5);
            startSpriteAnimation(0x85, 0, 8);
            break;
        }

}

// FIXME: indicates the grouped sprite functions are probably macro/static inline
static inline inline_func() {
    setSpriteShrinkFactor(0x8E, 0, 0, 256.0f);
}

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B3BD8);

// money sprites
// TODO: add linker symbols
/*
void func_800B3BD8(void) {

    u32 temp = gGold;

    dmaSprite(0x8E, &D_CFE610, &D_CFF6F0, &D_CFF6F0_2, &D_CFF710, 0, 0, 0x80253B00, 0, 0x80254A00, 0x80254E00, 0x80254F00, 0, 0, 0);
    inline_func();
    setBilinearFiltering(0x8E, TRUE);
    setSpriteColor(0x8E, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x8E, 2);
    startSpriteAnimation(0x8E, 0, 0);
    
    func_80045E20(1, 0x91, &D_CFE610_2, &D_CFF6F0_3, &D_CFF6F0_4, &D_CFF710, 0x80253B00, 0x80254A00, 0x80254E00, 0x80254F00, 0, 1, 0, 0, 0, 320.0f, 0xA);
    dmaPauseScreenSprites(1, temp, 5, 3);
    setPauseScreenSpritesRGBA(1, 0xFF, 0xFF, 0xFF, 0xFF);
    setSpriteAlpha(0x8E, 0xFF);
    
    dmaSprite(0x82, &D_D3BFE0, &D_D3DCC0, &D_D3DCC0_2, &D_D3DCE0, 0, 0, 0x802E0EC0, 0, 0x802E27C0, 0x802E28C0, 0x802E2BC0, 0, 0, 0);
    setSpriteShrinkFactor(0x82, -96.0f, 64.0f, 256.0f);
    setBilinearFiltering(0x82, TRUE);
    setSpriteColor(0x82, 0xFF, 0xFF, 0xFF, 0xFF);
    startSpriteAnimation(0x82, 0, 0);
    
    dmaSprite(0x83, &D_D3BFE0, &D_D3DCC0, &D_D3DCC0_2, &D_D3DCE0, 0, 0, 0x802E0EC0, 0, 0x802E27C0, 0x802E28C0, 0x802E2BC0, 0, 0, 0);
    setSpriteShrinkFactor(0x83, -56.0f, 64.0f, 256.0f);
    setBilinearFiltering(0x83, TRUE);
    setSpriteColor(0x83, 0xFF, 0xFF, 0xFF, 0xFF);
    startSpriteAnimation(0x83, 1, 0);
    
    dmaSprite(0x84, &D_D3BFE0, &D_D3DCC0, &D_D3DCC0_2, &D_D3DCE0, 0, 0, 0x802E0EC0, 0, 0x802E27C0, 0x802E28C0, 0x802E2BC0, 0, 0, 0);
    setBilinearFiltering(0x84, TRUE);
    setSpriteColor(0x84, 0xFF, 0xFF, 0xFF, 0xFF);
    
    dmaSprite(0x85, &D_D3BFE0, &D_D3DCC0, &D_D3DCC0_2, &D_D3DCE0, 0, 0, 0x802E0EC0, 0, 0x802E27C0, 0x802E28C0, 0x802E2BC0, 0, 0, 0);
    setSpriteShrinkFactor(0x85, -12.0f, 13.0f, 256.0f);
    setBilinearFiltering(0x85, TRUE);
    setSpriteColor(0x85, 0xFF, 0xFF, 0xFF, 0xFF);
    
    dmaSprite(0x86, &D_D3BFE0, &D_D3DCC0, &D_D3DCC0_2, &D_D3DCE0, 0, 0, 0x802E0EC0, 0, 0x802E27C0, 0x802E28C0, 0x802E2BC0, 0, 0, 0);
    setSpriteShrinkFactor(0x86, -12.0f, 13.0f, 256.0f);
    setBilinearFiltering(0x86, TRUE);
    setSpriteColor(0x86, 0xFF, 0xFF, 0xFF, 0xFF);
    
    dmaSprite(0x87, &D_D3BFE0, &D_D3DCC0, &D_D3DCC0_2, &D_D3DCE0, 0, 0, 0x802E0EC0, 0, 0x802E27C0, 0x802E28C0, 0x802E2BC0, 0, 0, 0);
    setSpriteShrinkFactor(0x87, -16.0f, 11.0f, 224.0f);
    setBilinearFiltering(0x87, TRUE);
    setSpriteColor(0x87, 0xFF, 0xFF, 0xFF, 0xFF);

    func_800B4160();

}
*/

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B4160);

void func_800B4160(void) {

    // shop struct
    if (!shopContext.unk_5) {
        resetAnimationState(0x82);
        resetAnimationState(0x83);
        resetAnimationState(0x84);
        setSpriteShrinkFactor(0x84, -96.0f, 64.0f, 256.0f);
        startSpriteAnimation(0x82, 7, 0xFE);
        startSpriteAnimation(0x83, 1, 0);
        startSpriteAnimation(0x84, 5, 0);
    } else {
        resetAnimationState(0x82);
        resetAnimationState(0x83);
        resetAnimationState(0x84);
        setSpriteShrinkFactor(0x84, -56.0f, 64.0f, 256.0f);
        startSpriteAnimation(0x82, 0, 0);
        startSpriteAnimation(0x83, 8, 0xFE);
        startSpriteAnimation(0x84, 6, 0);
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B4238);

void func_800B4238(u8 arg0) {
    
    startSpriteAnimation(0x87, 9, 0xFE);
    
    if (shopContext.quantity != 0 && shopContext.quantity < arg0) {
        startSpriteAnimation(0x85, 3, 0);
    } else {
        resetAnimationState(0x85);
    }
    
    if (shopContext.quantity >= 2 && arg0 >= shopContext.quantity) {
        startSpriteAnimation(0x86, 4, 0);
        return;
    }
    
    resetAnimationState(0x86);
}

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B42E0);

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B59AC);

// adjust sprite colors
void func_800B59AC(void) {
    updateSpriteRGBA(0x80, 0xFF, 0xFF, 0xFF, 0xFF, 24);
    updateSpriteRGBA(0x82, 0xFF, 0xFF, 0xFF, 0xFF, 24);
    updateSpriteRGBA(0x83, 0xFF, 0xFF, 0xFF, 0xFF, 24);
    updateSpriteRGBA(0x84, 0xFF, 0xFF, 0xFF, 0xFF, 24);
    updateSpriteRGBA(0xA3, 0xFF, 0xFF, 0xFF, 0xFF, 24);
    updateSpriteRGBA(0xA4, 0xFF, 0xFF, 0xFF, 0xFF, 24);
    updateSpriteRGBA(0xA5, 0xFF, 0xFF, 0xFF, 0xFF, 24);
    updateSpriteRGBA(0xA6, 0xFF, 0xFF, 0xFF, 0xFF, 24);
    updateSpriteRGBA(0xA7, 0xFF, 0xFF, 0xFF, 0xFF, 24);
    updateSpriteRGBA(0xA8, 0xFF, 0xFF, 0xFF, 0xFF, 24);
    updateSpriteRGBA(0xA9, 0xFF, 0xFF, 0xFF, 0xFF, 24);
    updateSpriteRGBA(0xAA, 0xFF, 0xFF, 0xFF, 0xFF, 24);
    updateSpriteRGBA(0x85, 0xFF, 0xFF, 0xFF, 0xFF, 24);
    updateSpriteRGBA(0x86, 0xFF, 0xFF, 0xFF, 0xFF, 24);
    updateSpriteRGBA(0x87, 0xFF, 0xFF, 0xFF, 0xFF, 24);
    updateSpriteRGBA(0x88, 0xFF, 0xFF, 0xFF, 0xFF, 24);
    updateSpriteRGBA(0xAB, 0xFF, 0xFF, 0xFF, 0xFF, 24);
    updateSpriteRGBA(0xAD, 0xFF, 0xFF, 0xFF, 0xFF, 24);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B5BD4);

void func_800B5BD4(void) {
    setSpriteColor(0x80, 0xFF, 0xFF, 0xFF, 0xFF);
    setSpriteColor(0x82, 0xFF, 0xFF, 0xFF, 0xFF);
    setSpriteColor(0x83, 0xFF, 0xFF, 0xFF, 0xFF);
    setSpriteColor(0x84, 0xFF, 0xFF, 0xFF, 0xFF);
    setSpriteColor(0xA3, 0xFF, 0xFF, 0xFF, 0xFF);
    setSpriteColor(0xA4, 0xFF, 0xFF, 0xFF, 0xFF);
    setSpriteColor(0xA5, 0xFF, 0xFF, 0xFF, 0xFF);
    setSpriteColor(0xA6, 0xFF, 0xFF, 0xFF, 0xFF);
    setSpriteColor(0xA7, 0xFF, 0xFF, 0xFF, 0xFF);
    setSpriteColor(0xA8, 0xFF, 0xFF, 0xFF, 0xFF);
    setSpriteColor(0xA9, 0xFF, 0xFF, 0xFF, 0xFF);
    setSpriteColor(0xAA, 0xFF, 0xFF, 0xFF, 0xFF);
    setSpriteColor(0x85, 0xFF, 0xFF, 0xFF, 0xFF);
    setSpriteColor(0x86, 0xFF, 0xFF, 0xFF, 0xFF);
    setSpriteColor(0x87, 0xFF, 0xFF, 0xFF, 0xFF);
    setSpriteColor(0x88, 0xFF, 0xFF, 0xFF, 0xFF);
    setSpriteColor(0xAB, 0xFF, 0xFF, 0xFF, 0xFF);
    setSpriteColor(0xAD, 0xFF, 0xFF, 0xFF, 0xFF);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B5DA8);

void func_800B5DA8(void) {
    updateSpriteRGBA(0x80, 0, 0, 0, 0, 24);
    updateSpriteRGBA(0x82, 0, 0, 0, 0, 24);
    updateSpriteRGBA(0x83, 0, 0, 0, 0, 24);
    updateSpriteRGBA(0x84, 0, 0, 0, 0, 24);
    updateSpriteRGBA(0xA3, 0, 0, 0, 0, 24);
    updateSpriteRGBA(0xA4, 0, 0, 0, 0, 24);
    updateSpriteRGBA(0xA5, 0, 0, 0, 0, 24);
    updateSpriteRGBA(0xA6, 0, 0, 0, 0, 24);
    updateSpriteRGBA(0xA7, 0, 0, 0, 0, 24);
    updateSpriteRGBA(0xA8, 0, 0, 0, 0, 24);
    updateSpriteRGBA(0xA9, 0, 0, 0, 0, 24);
    updateSpriteRGBA(0xAA, 0, 0, 0, 0, 24);
    updateSpriteRGBA(0x85, 0, 0, 0, 0, 24);
    updateSpriteRGBA(0x86, 0, 0, 0, 0, 24);
    updateSpriteRGBA(0x87, 0, 0, 0, 0, 24);
    updateSpriteRGBA(0x88, 0, 0, 0, 0, 24);
    updateSpriteRGBA(0xAB, 0, 0, 0, 0, 24);
    updateSpriteRGBA(0xAD, 0, 0, 0, 0, 24);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B5FC4);

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
            setSpriteShrinkFactor(0x79, D_80116D80[arg1], D_80116D90[arg1], 32.0f);
        }

        if (arg2 != 0xFF) {
            setSpriteShrinkFactor(0x79, D_80116DA0[arg2], D_80116DB0[arg2], 32.0f);
        }
        
        startSpriteAnimation(0x79, 3, 0);
        return;
    }
    
    if (arg1 != 0xFF) {
        setSpriteShrinkFactor(0xB2, D_80116D80[arg1], D_80116D90[arg1], 32.0f);
    }

    if (arg2 != 0xFF) {
        setSpriteShrinkFactor(0xB2, arg2, arg2, 32.0f);
    }
    
    // can't remove this without messing up control flow
    temp = 0xB2;

    startSpriteAnimation(0xB2, 3, 0);
}

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B60E4);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B64E4);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B6674);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B7070);

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B7B34);

void func_800B7B34(void) {
    updateSpriteRGBA(0x80, 0xFF, 0xFF, 0xFF, 0xFF, 24);
    updateSpriteRGBA(0x82, 0xFF, 0xFF, 0xFF, 0xFF, 24);
    updateSpriteRGBA(0x83, 0xFF, 0xFF, 0xFF, 0xFF, 24);
    updateSpriteRGBA(0x84, 0xFF, 0xFF, 0xFF, 0xFF, 24);
    updateSpriteRGBA(0x85, 0xFF, 0xFF, 0xFF, 0xFF, 24);
    updateSpriteRGBA(0x86, 0xFF, 0xFF, 0xFF, 0xFF, 24);
    updateSpriteRGBA(0x87, 0xFF, 0xFF, 0xFF, 0xFF, 24);
    updateSpriteRGBA(0xAE, 0xFF, 0xFF, 0xFF, 0xFF, 24);
    updateSpriteRGBA(0xAF, 0xFF, 0xFF, 0xFF, 0xFF, 24);
    updateSpriteRGBA(0xB0, 0xFF, 0xFF, 0xFF, 0xFF, 24);
    updateSpriteRGBA(0xB1, 0xFF, 0xFF, 0xFF, 0xFF, 24);
    updateSpriteRGBA(0xB2, 0xFF, 0xFF, 0xFF, 0xFF, 24);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B7CB4);

void func_800B7CB4(void) {
    updateSpriteRGBA(0x80, 0, 0, 0, 0, 24);
    updateSpriteRGBA(0x82, 0, 0, 0, 0, 24);
    updateSpriteRGBA(0x83, 0, 0, 0, 0, 24);
    updateSpriteRGBA(0x84, 0, 0, 0, 0, 24);
    updateSpriteRGBA(0x85, 0, 0, 0, 0, 24);
    updateSpriteRGBA(0x86, 0, 0, 0, 0, 24);
    updateSpriteRGBA(0x87, 0, 0, 0, 0, 24);
    updateSpriteRGBA(0xAE, 0, 0, 0, 0, 24);
    updateSpriteRGBA(0xAF, 0, 0, 0, 0, 24);
    updateSpriteRGBA(0xB0, 0, 0, 0, 0, 24);
    updateSpriteRGBA(0xB1, 0, 0, 0, 0, 24);
    updateSpriteRGBA(0xB2, 0, 0, 0, 0, 24);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B7E28);

void func_800B7E28(void) {
    setSpriteColor(0x80, 0xFF, 0xFF, 0xFF, 0xFF);
    setSpriteColor(0x82, 0xFF, 0xFF, 0xFF, 0xFF);
    setSpriteColor(0x83, 0xFF, 0xFF, 0xFF, 0xFF);
    setSpriteColor(0x84, 0xFF, 0xFF, 0xFF, 0xFF);
    setSpriteColor(0x85, 0xFF, 0xFF, 0xFF, 0xFF);
    setSpriteColor(0x86, 0xFF, 0xFF, 0xFF, 0xFF);
    setSpriteColor(0x87, 0xFF, 0xFF, 0xFF, 0xFF);
    setSpriteColor(0xAE, 0xFF, 0xFF, 0xFF, 0xFF);
    setSpriteColor(0xAF, 0xFF, 0xFF, 0xFF, 0xFF);
    setSpriteColor(0xB0, 0xFF, 0xFF, 0xFF, 0xFF);
    setSpriteColor(0xB1, 0xFF, 0xFF, 0xFF, 0xFF);
    setSpriteColor(0xB2, 0xFF, 0xFF, 0xFF, 0xFF);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B7F6C);

void func_800B7F6C(u8 arg0) {

    if (arg0 != 0xFF) {
        setSpriteShrinkFactor(0x79, D_80116DC0[arg0], D_80116DD4[arg0], 32.0f);
    }

    startSpriteAnimation(0x79, 3, 0);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B7FC8);

void func_800B7FC8(u8 arg0, u8 arg1) {

    if (arg0 != 0xFF) {
        if (arg1) {
            setSpritePaletteIndex(arg0 + 0x83, 1);
        } else {
            setSpritePaletteIndex(arg0+ 0x83, 0xF);
        }
    }
}

static inline void initializeOverlayScreenTable(void) {

    overlayScreenTable.cellIndex = 0;
    overlayScreenTable.previousCellIndex = 0;
    overlayScreenTable.pageNumber = 0;
    overlayScreenTable.unk_3 = 0xFF;
    overlayScreenTable.subscreen = 0;

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B8018);

// estimate/evaluation screen
void func_800B8018(void) {
    
    func_800593EC();

    dmaSprite(0x80, &_checkerboardBackgroundTextureSegmentRomStart, &_checkerboardBackgroundTextureSegmentRomEnd, &_checkerboardBackgroundIndexSegmentRomStart, &_checkerboardBackgroundIndexSegmentRomEnd, 0, 0, 0x802DE5C0, 0, 0x802E0BC0, 0x802E0CC0, 0x802E0DC0, 0, 0, 0);
    setSpriteShrinkFactor(0x80, 0, 0, 0);
    setSpriteScale(0x80, 2.0f, 2.0f, 1.0f);
    setBilinearFiltering(0x80, TRUE);
    setSpriteColor(0x80, 0xFF, 0xFF, 0xFF, 0xFF);
    startSpriteAnimation(0x80, 0, 0); 
     
    initializeOverlayScreenTable();
    
    setMainLoopCallbackFunctionIndex(ESTIMATE);

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B815C);

// kitchen picture load main loop callback
void func_800B815C(void) {

    func_800593EC();

    dmaSprite(0x80, &_checkerboardBackgroundTextureSegmentRomStart, &_checkerboardBackgroundTextureSegmentRomEnd, &_checkerboardBackgroundIndexSegmentRomStart, &_checkerboardBackgroundIndexSegmentRomEnd, NULL, NULL, (void*)0x802DE5C0, NULL, (void*)0x802E0BC0, (void*)0x802E0CC0, (void*)0x802E0DC0, NULL, NULL, 0);
    setSpriteShrinkFactor(0x80, 0.0f, 0.0f, 0.0f);
    setSpriteScale(0x80, 2.0f, 2.0f, 1.0f);
    setBilinearFiltering(0x80, TRUE);
    setSpriteColor(0x80, 0xFF, 0xFF, 0xFF, 0xFF);
    startSpriteAnimation(0x80, 0, 0);

    overlayScreenTable.cellIndex = func_800C2780(0, 0);
    overlayScreenTable.previousCellIndex = 0;
    overlayScreenTable.pageNumber = 0;
    overlayScreenTable.unk_3 = 0xFF;
    overlayScreenTable.subscreen = 0;
    
    setMainLoopCallbackFunctionIndex(KITCHEN_PICTURE);

}


//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B82AC);

// home extensions selection
void func_800B82AC(void) {

    func_800593EC();
    dmaSprite(0x80, &_checkerboardBackgroundTextureSegmentRomStart, &_checkerboardBackgroundTextureSegmentRomEnd, &_checkerboardBackgroundIndexSegmentRomStart, &_checkerboardBackgroundIndexSegmentRomEnd, NULL, NULL, (void*)0x802DE5C0, NULL, (void*)0x802E0BC0, (void*)0x802E0CC0, (void*)0x802E0DC0, NULL, NULL, 0);
    setSpriteShrinkFactor(0x80, 0.0f, 0.0f, 0.0f);
    setSpriteScale(0x80, 2.0f, 2.0f, 1.0f);
    setBilinearFiltering(0x80, TRUE);
    setSpriteColor(0x80, 0xFF, 0xFF, 0xFF, 0xFF);
    startSpriteAnimation(0x80, 0, 0);

    initializeOverlayScreenTable();
    
    setMainLoopCallbackFunctionIndex(HOME_EXTENSIONS_SELECT);

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B83F0);

// calendar load main loop callback
void func_800B83F0(void) {

    func_800593EC();
    dmaSprite(0x80, &_checkerboardBackgroundTextureSegmentRomStart, &_checkerboardBackgroundTextureSegmentRomEnd, &_checkerboardBackgroundIndexSegmentRomStart, &_checkerboardBackgroundIndexSegmentRomEnd, NULL, NULL, (void*)0x802DE5C0, NULL, (void*)0x802E0BC0, (void*)0x802E0CC0, (void*)0x802E0DC0, NULL, NULL, 0);
    setSpriteShrinkFactor(0x80, 0.0f, 0.0f, 0.0f);
    setSpriteScale(0x80, 2.0f, 2.0f, 1.0f);
    setBilinearFiltering(0x80, TRUE);
    setSpriteColor(0x80, 0xFF, 0xFF, 0xFF, 0xFF);
    startSpriteAnimation(0x80, 0, 0);

    initializeOverlayScreenTable();

    overlayScreenTable.unk_8 = 0;
    overlayScreenTable.unk_9 = 0;
    overlayScreenTable.unk_A = 0;
    overlayScreenTable.unk_B = 0;

    setMainLoopCallbackFunctionIndex(CALENDAR);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B8554);

// recipe book load main loop callback
void func_800B8554(void) {
    
    func_800593EC();
    dmaSprite(0x80, &_checkerboardBackgroundTextureSegmentRomStart, &_checkerboardBackgroundTextureSegmentRomEnd, &_checkerboardBackgroundIndexSegmentRomStart, &_checkerboardBackgroundIndexSegmentRomEnd, NULL, NULL, (void*)0x802DE5C0, NULL, (void*)0x802E0BC0, (void*)0x802E0CC0, (void*)0x802E0DC0, NULL, NULL, 0);
    setSpriteShrinkFactor(0x80, 0.0f, 0.0f, 0.0f);
    setSpriteScale(0x80, 2.0f, 2.0f, 1.0f);
    setBilinearFiltering(0x80, TRUE);
    setSpriteColor(0x80, 0xFF, 0xFF, 0xFF, 0xFF);
    startSpriteAnimation(0x80, 0, 0);

    initializeOverlayScreenTable();

    overlayScreenTable.unk_8 = 0;
    overlayScreenTable.unk_9 = 0;
    overlayScreenTable.unk_A = 0;
    overlayScreenTable.unk_B = 0;

    setMainLoopCallbackFunctionIndex(RECIPE_BOOK);
}

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B86B8);

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B881C);

// prepare pause screen
void func_800B881C(void) {
    func_800593EC();
    func_800B8844();
}

// pause screen
INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B8844);

// pause screen assets
INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B8980);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B90E0);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B9914);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B99EC);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B9B8C);

// jtbl_80121C98
// pause screen main loop callback
INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B9D3C);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BA928);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BA9E8);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BAAA0);

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BAC7C);

void func_800BAC7C(void) {
    
    u8 temp;

    switch (overlayScreenTable.subscreen) {          
        
        case TOOL_SLOTS:
            temp = gPlayer.toolSlots[overlayScreenTable.unk_3];
            gPlayer.toolSlots[overlayScreenTable.unk_3] = gPlayer.toolSlots[overlayScreenTable.cellIndex];
            gPlayer.toolSlots[overlayScreenTable.cellIndex] = temp;
            break;
        
        case BELONGINGS:
            temp = gPlayer.belongingsSlots[overlayScreenTable.unk_3];
            gPlayer.belongingsSlots[overlayScreenTable.unk_3] = gPlayer.belongingsSlots[overlayScreenTable.cellIndex];
            gPlayer.belongingsSlots[overlayScreenTable.cellIndex] = temp;
            break;
        
        case KEY_ITEMS:
            temp = gPlayer.keyItemSlots[overlayScreenTable.unk_4* 8 + overlayScreenTable.unk_3];
            gPlayer.keyItemSlots[overlayScreenTable.unk_4*8 + overlayScreenTable.unk_3] = gPlayer.keyItemSlots[overlayScreenTable.pageNumber*8 + overlayScreenTable.cellIndex];
            gPlayer.keyItemSlots[overlayScreenTable.pageNumber*8 + overlayScreenTable.cellIndex] = temp;
            break;

        }
    
}

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BADD0);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BAF1C);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BB590);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BBAF0);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BBBC8);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BBD34);

// jtbl_80121CC0
// toolbox callback
INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BBEC0);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BC700);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BC770);

// rucksack
INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BC7D8);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BC918);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BCA9C);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BCBE8);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BD25C);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BD754);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BD82C);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BD998);

// jtbl_80121CE0
INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BDB24);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BE3B8);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BE428);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BE490);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BE5D0);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BE684);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BE808);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BE954);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BF0C8);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BF5C0);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BF698);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BF804);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BF990);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C0238);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C02A8);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C0310);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C0450);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C0504);

// jtbl_80121D00
// player struct unk_1F
INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C0688);

// house extension selection
INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C0714);

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C0F24);

void func_800C0F24(void) {
    updateSpriteAlpha(0x89, 0xFF, 24);
    updateSpriteAlpha(0x8C, 0xFF, 24);
    updateSpriteAlpha(0x8D, 0xFF, 24);
    updateSpriteAlpha(0x8B, 0xFF, 24);
    updateSpriteAlpha(0x8A, 0xFF, 24);
    updateSpriteAlpha(0x8E, 0xFF, 24);
    updateSpriteAlpha(0x81, 0xFF, 24);
    updateSpriteAlpha(0x82, 0xFF, 24);
    updateSpriteAlpha(0x83, 0xFF, 24);
    updateSpriteAlpha(0x84, 0xFF, 24);
    updateSpriteAlpha(0x85, 0xFF, 24);
    updateSpriteAlpha(0x86, 0xFF, 24);
    updateSpriteAlpha(0x87, 0xFF, 24);
    updateSpriteAlpha(0x88, 0xFF, 24);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C101C);

void func_800C101C(void) {
    updateSpriteAlpha(0x89, 0, 24);
    updateSpriteAlpha(0x8C, 0, 24);
    updateSpriteAlpha(0x8D, 0, 24);
    updateSpriteAlpha(0x8B, 0, 24);
    updateSpriteAlpha(0x8A, 0, 24);
    updateSpriteAlpha(0x8E, 0, 24);
    updateSpriteAlpha(0x81, 0, 24);
    updateSpriteAlpha(0x82, 0, 24);
    updateSpriteAlpha(0x83, 0, 24);
    updateSpriteAlpha(0x84, 0, 24);
    updateSpriteAlpha(0x85, 0, 24);
    updateSpriteAlpha(0x86, 0, 24);
    updateSpriteAlpha(0x87, 0, 24);
    updateSpriteAlpha(0x88, 0, 24);
    deactivatePauseScreenSprites(0);
    deactivatePauseScreenSprites(1);
}

//INCLUDE_RODATA("asm/nonmatchings/game/overlayScreens", D_80121D74);

static const u16 D_80121D74[6] = { 0x10, 0x11, 0x12, 0x13, 0x14, 0x15};

// rodata: D_80121D74
// jtbl_80121D80
// house extension selection callback
INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C1124);

// estimate screen
INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C1710);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C2060);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C2128);

// jtbl_80121DA0
INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C224C);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C2364);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C2504);

// rucksack
INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C2660);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C2780);

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C292C);

inline void func_800C292C(void) {
    updateSpriteAlpha(0x89, 0xFF, 24);
    updateSpriteAlpha(0x8C, 0xFF, 24);
    updateSpriteAlpha(0x8D, 0xFF, 24);
    updateSpriteAlpha(0x8B, 0xFF, 24);
    updateSpriteAlpha(0x8A, 0xFF, 24);
    updateSpriteAlpha(0x8E, 0xFF, 24);
    updateSpriteAlpha(0x82, 0xFF, 24);
    updateSpriteAlpha(0xA3, 0xFF, 24);
    updateSpriteAlpha(0xA4, 0xFF, 24);
    updateSpriteAlpha(0xA5, 0xFF, 24);
    updateSpriteAlpha(0xA6, 0xFF, 24);
    updateSpriteAlpha(0xA7, 0xFF, 24);
    updateSpriteAlpha(0xA8, 0xFF, 24);
    updateSpriteAlpha(0xA9, 0xFF, 24);
    updateSpriteAlpha(0xAA, 0xFF, 24);
    updateSpriteAlpha(0xAB, 0xFF, 24);
    updateSpriteAlpha(0xAC, 0xFF, 24);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C2A54);

void func_800C2A54(void) {

    updateSpriteAlpha(0x89, 0, 24);
    updateSpriteAlpha(0x8C, 0, 24);
    updateSpriteAlpha(0x8D, 0, 24);
    updateSpriteAlpha(0x8B, 0, 24);
    updateSpriteAlpha(0x8A, 0, 24);
    updateSpriteAlpha(0x8E, 0, 24);
    updateSpriteAlpha(0x82, 0, 24);
    updateSpriteAlpha(0xA3, 0, 24);
    updateSpriteAlpha(0xA4, 0, 24);
    updateSpriteAlpha(0xA5, 0, 24);
    updateSpriteAlpha(0xA6, 0, 24);
    updateSpriteAlpha(0xA7, 0, 24);
    updateSpriteAlpha(0xA8, 0, 24);
    updateSpriteAlpha(0xA9, 0, 24);
    updateSpriteAlpha(0xAA, 0, 24);
    updateSpriteAlpha(0xAB, 0, 24);
    updateSpriteAlpha(0xAC, 0, 24);

    deactivatePauseScreenSprites(0);
    deactivatePauseScreenSprites(1);

}

// jtbl_80121DB8
INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C2B8C);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C2F50);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C3108);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C34C0);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C39A0);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C3B1C);

// jtbl_80121DD0
INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C3D20);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C4A74);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C4E90);

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C5080);

void func_800C5080(void) {
    updateSpriteAlpha(0x89, 0xFF, 24);
    updateSpriteAlpha(0x8C, 0xFF, 24);
    updateSpriteAlpha(0x8D, 0xFF, 24);
    updateSpriteAlpha(0x8B, 0xFF, 24);
    updateSpriteAlpha(0x8A, 0xFF, 24);
    updateSpriteAlpha(0x8E, 0xFF, 24);
    updateSpriteAlpha(0x81, 0xFF, 24);
    updateSpriteAlpha(0x82, 0xFF, 24);
    updateSpriteAlpha(0xA3, 0xFF, 24);
    updateSpriteAlpha(0xA4, 0xFF, 24);
    updateSpriteAlpha(0xA5, 0xFF, 24);
    updateSpriteAlpha(0xA6, 0xFF, 24);
    updateSpriteAlpha(0xA7, 0xFF, 24);
    updateSpriteAlpha(0xA8, 0xFF, 24);
    updateSpriteAlpha(0xA9, 0xFF, 24);
    updateSpriteAlpha(0xAA, 0xFF, 24);
    updateSpriteAlpha(0xAB, 0xFF, 24);
    updateSpriteAlpha(0xAC, 0xFF, 24);
    updateSpriteAlpha(0xAD, 0xFF, 24);
    updateSpriteAlpha(0xAE, 0xFF, 24);
    updateSpriteAlpha(0xAF, 0xFF, 24);
    updateSpriteAlpha(0xB0, 0xFF, 24);
    updateSpriteAlpha(0xB1, 0xFF, 24);
    updateSpriteAlpha(0xB2, 0xFF, 24);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C5218);

void func_800C5218(void) {

    updateSpriteAlpha(0x89, 0, 24);
    updateSpriteAlpha(0x8C, 0, 24);
    updateSpriteAlpha(0x8D, 0, 24);
    updateSpriteAlpha(0x8B, 0, 24);
    updateSpriteAlpha(0x8A, 0, 24);
    updateSpriteAlpha(0x8E, 0, 24);
    updateSpriteAlpha(0x81, 0, 24);
    updateSpriteAlpha(0x82, 0, 24);
    updateSpriteAlpha(0xA3, 0, 24);
    updateSpriteAlpha(0xA4, 0, 24);
    updateSpriteAlpha(0xA5, 0, 24);
    updateSpriteAlpha(0xA6, 0, 24);
    updateSpriteAlpha(0xA7, 0, 24);
    updateSpriteAlpha(0xA8, 0, 24);
    updateSpriteAlpha(0xA9, 0, 24);
    updateSpriteAlpha(0xAA, 0, 24);
    updateSpriteAlpha(0xAB, 0, 24);
    updateSpriteAlpha(0xAC, 0, 24);
    updateSpriteAlpha(0xAD, 0, 24);
    updateSpriteAlpha(0xAE, 0, 24);
    updateSpriteAlpha(0xAF, 0, 24);
    updateSpriteAlpha(0xB0, 0, 24);
    updateSpriteAlpha(0xB1, 0, 24);
    updateSpriteAlpha(0xB2, 0, 24);

    deactivatePauseScreenSprites(0);
    deactivatePauseScreenSprites(1);

}

// jtbl_80121DF0
INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C53C0);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C5AA4);

// jtbl_80121E08
// jtbl_80121E20
INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C5DA4);

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C6E98);

void func_800C6E98(void) {
    updateSpriteAlpha(0x89, 0xFF, 24);
    updateSpriteAlpha(0x8C, 0xFF, 24);
    updateSpriteAlpha(0x8D, 0xFF, 24);
    updateSpriteAlpha(0x8B, 0xFF, 24);
    updateSpriteAlpha(0x8A, 0xFF, 24);
    updateSpriteAlpha(0x8E, 0xFF, 24);
    updateSpriteAlpha(0x81, 0xFF, 24);
    updateSpriteAlpha(0x82, 0xFF, 24);
    updateSpriteAlpha(0x83, 0xFF, 24);
    updateSpriteAlpha(0x84, 0xFF, 24);
    updateSpriteAlpha(0x85, 0xFF, 24);
    updateSpriteAlpha(0x86, 0xFF, 24);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C6F70);

void func_800C6F70(void) {

    updateSpriteAlpha(0x89, 0, 24);
    updateSpriteAlpha(0x8C, 0, 24);
    updateSpriteAlpha(0x8D, 0, 24);
    updateSpriteAlpha(0x8B, 0, 24);
    updateSpriteAlpha(0x8A, 0, 24);
    updateSpriteAlpha(0x8E, 0, 24);
    updateSpriteAlpha(0x81, 0, 24);
    updateSpriteAlpha(0x82, 0, 24);
    updateSpriteAlpha(0x83, 0, 24);
    updateSpriteAlpha(0x84, 0, 24);
    updateSpriteAlpha(0x85, 0, 24);
    updateSpriteAlpha(0x86, 0, 24);

    deactivatePauseScreenSprites(0);
    deactivatePauseScreenSprites(1);

}

//INCLUDE_RODATA("asm/nonmatchings/game/overlayScreens", D_80121E34);

static const u8 D_80121E34[20] = { 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x2D, 0x2E, 0, 0, 0, 0 };

// D_80121E34
// jtbl_80121E48
INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C7058);

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", addRecipe);

void addRecipe(u16 bitIndex) {
    u32 temp = bitIndex;
    recipesBits[temp >> 5] |= 1 << (temp & 0x1F);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", toggleRecipe);

void toggleRecipe(u16 bitIndex) {
    u32 temp = bitIndex;
    recipesBits[temp >> 5] &= ~(1 << (temp & 0x1F));
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", checkRecipe);

u32 checkRecipe(u16 bitIndex) {
    u32 temp = bitIndex;
    return recipesBits[temp >> 5] & (1 << (temp & 0x1F));
}


// possible split: 0xA2C50
INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C7850);

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C7C88);

void func_800C7C88(f32 x, f32 y) {
    startSpriteAnimation(0x7C, 0xB, 0xFE);
    setSpriteShrinkFactor(0x7C, x, y, 32.0f);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C7CE4);

void func_800C7CE4(f32 x, f32 y) {
    startSpriteAnimation(0x7D, 0xA, 0xFE);
    setSpriteShrinkFactor(0x7D, x, y, 32.0f);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C7D40);

void func_800C7D40(f32 x, f32 y) {
    startSpriteAnimation(0x7F, 1, 0xFE);
    setSpriteShrinkFactor(0x7F, x, y, 32.0f);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C7D9C);

void func_800C7D9C(f32 x, f32 y) {
    startSpriteAnimation(0x7E, 0xC, 0xFE);
    setSpriteShrinkFactor(0x7E, x, y, 32.0f);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C7DF8);

void func_800C7DF8(void) {
    resetAnimationState(0x7C);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C7E18);

void func_800C7E18(void) {
    resetAnimationState(0x7D);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C7E38);

void func_800C7E38(void) {
    resetAnimationState(0x7F);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C7E58);

void func_800C7E58(void) {
    resetAnimationState(0x7E);
}

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C7E78);

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C80F0);

void func_800C80F0(f32 x, f32 y) {
    startSpriteAnimation(0x7B, 0xB, 0xFE);
    setSpriteShrinkFactor(0x7B, x, y, 32.0f);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C814C);

void func_800C814C(f32 x, f32 y) {
    startSpriteAnimation(0x7A, 0xA, 0xFE);
    setSpriteShrinkFactor(0x7A, x, y, 32.0f);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C81A8);

void func_800C81A8(void) {
    resetAnimationState(0x7B);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C81C8);

void func_800C81C8(void) {
    resetAnimationState(0x7A);
}

// jtbl_80121E60
INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C81E8);

// jtbl_80121F28
INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C8424);

// jtbl_80122098
INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C8784);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C88F4);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C8B04);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C9FF0);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CA590);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CA678);

// jtbl_801220B0
INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CA808);

// horse race
// D_801220CC
INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CB0C4);

// D_80122140
INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CB8F0);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CC11C);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CC17C);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CC518);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CC6F0);

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CCFC8);

void func_800CCFC8(void) {

    setGameVariableString(0x2A, overlayScreenStrings.dialogueVariableStrings[overlayScreenStrings.dialogueVariableIndices[0]], 6);
    setGameVariableString(0x2B, overlayScreenStrings.dialogueVariableStrings[overlayScreenStrings.dialogueVariableIndices[1]], 6);
    setGameVariableString(0x2C, overlayScreenStrings.dialogueVariableStrings[overlayScreenStrings.dialogueVariableIndices[2]], 6);
    setGameVariableString(0x2D, overlayScreenStrings.dialogueVariableStrings[overlayScreenStrings.dialogueVariableIndices[3]], 6);
    setGameVariableString(0x2E, overlayScreenStrings.dialogueVariableStrings[overlayScreenStrings.dialogueVariableIndices[4]], 6);
    setGameVariableString(0x2F, overlayScreenStrings.dialogueVariableStrings[overlayScreenStrings.dialogueVariableIndices[5]], 6);
    
    func_8003DBE8(0, (u8*)0x8030B000);
    func_8003F54C(0, -32.0f, 32.0f, 0.0f);
    func_8003F5D0(0, 0xA, 2);
    func_8003F630(0, 0, 2);
    func_8003F464(0, 0xE, 0xE, (u8*)0x802FF000, (u16*)0x8030A000);
    func_8003F360(0, -4, 2);
    setDialogueBoxSpriteIndices(0, 0xFF, 0, 0);
    func_8003FAF8(0, 0x8000);
    func_8003FB4C(0, 1);
    initializeDialogueBox(0, 0, 0x12, 0x80000);
    func_8003E77C(0, 0xFF, 0xFF, 0xFF, 0xFF);
    
    func_8003DBE8(1, (u8*)0x8030B400);
    func_8003F54C(1, -32.0f, 16.0f, 0.0f);
    func_8003F5D0(1, 0xA, 2);
    func_8003F630(1, 0, 2);
    func_8003F464(1, 0xE, 0xE, (u8*)0x802FF000, (u16*)0x8030A000);
    func_8003F360(1, -4, 2);
    setDialogueBoxSpriteIndices(1, 0xFF, 0, 0);
    func_8003FAF8(1, 0x8000);
    func_8003FB4C(1, 1);
    initializeDialogueBox(1, 0, 0x13, 0x80000);
    func_8003E77C(1, 0xFF, 0xFF, 0xFF, 0xFF);
    
    func_8003DBE8(2, (u8*)0x8030B800);
    func_8003F54C(2, -32.0f, 0.0f, 0.0f);
    func_8003F5D0(2, 0xA, 2);
    func_8003F630(2, 0, 2);
    func_8003F464(2, 0xE, 0xE, (u8*)0x802FF000, (u16*)0x8030A000);
    func_8003F360(2, -4, 2);
    setDialogueBoxSpriteIndices(2, 0xFF, 0, 0);
    func_8003FAF8(2, 0x8000);
    func_8003FB4C(2, 1);
    initializeDialogueBox(2, 0, 0x14, 0x80000);
    func_8003E77C(2, 0xFF, 0xFF, 0xFF, 0xFF);
    
    func_8003DBE8(3, (u8*)0x8030BC00);
    func_8003F54C(3, 96.0f, 32.0f, 0.0f);
    func_8003F5D0(3, 0xA, 2);
    func_8003F630(3, 0, 2);
    func_8003F464(3, 0xE, 0xE, (u8*)0x802FF000, (u16*)0x8030A000);
    func_8003F360(3, -4, 2);
    setDialogueBoxSpriteIndices(3, 0xFF, 0, 0);
    func_8003FAF8(3, 0x8000);
    func_8003FB4C(3, 1);
    initializeDialogueBox(3, 0, 0x15, 0x80000);
    func_8003E77C(3, 0xFF, 0xFF, 0xFF, 0xFF);
    
    func_8003DBE8(4, (u8*)0x8030C000);
    func_8003F54C(4, 96.0f, 16.0f, 0.0f);
    func_8003F5D0(4, 0xA, 2);
    func_8003F630(4, 0, 2);
    func_8003F464(4, 0xE, 0xE, (u8*)0x802FF000, (u16*)0x8030A000);
    func_8003F360(4, -4, 2);
    setDialogueBoxSpriteIndices(4, 0xFF, 0, 0);
    func_8003FAF8(4, 0x8000);
    func_8003FB4C(4, 1);
    initializeDialogueBox(4, 0, 0x16, 0x80000);
    func_8003E77C(4, 0xFF, 0xFF, 0xFF, 0xFF);
    
    func_8003DBE8(5, (u8*)0x8030C400);
    func_8003F54C(5, 96.0f, 0.0f, 0.0f);
    func_8003F5D0(5, 0xA, 2);
    func_8003F630(5, 0, 2);
    func_8003F464(5, 0xE, 0xE, (u8*)0x802FF000, (u16*)0x8030A000);
    func_8003F360(5, -4, 2);
    setDialogueBoxSpriteIndices(5, 0xFF, 0, 0);
    func_8003FAF8(5, 0x8000);
    func_8003FB4C(5, 1);
    initializeDialogueBox(5, 0, 0x17, 0x80000);
    func_8003E77C(5, 0xFF, 0xFF, 0xFF, 0xFF);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CD628);

void func_800CD628(void) {
    updateSpriteAlpha(0x81, 0xFF, 24);
    updateSpriteAlpha(0x82, 0xFF, 24);
    updateSpriteAlpha(0x83, 0xFF, 24);
    updateSpriteAlpha(0x84, 0xFF, 24);
    updateSpriteAlpha(0x85, 0xFF, 24);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CD690);

void func_800CD690(void) {

    updateSpriteAlpha(0x81, 0, 24);
    updateSpriteAlpha(0x82, 0, 24);
    updateSpriteAlpha(0x83, 0, 24);
    updateSpriteAlpha(0x84, 0, 24);
    updateSpriteAlpha(0x85, 0, 24);

    func_8003DD14(0);
    func_8003DD14(1);
    func_8003DD14(2);
    func_8003DD14(3);
    func_8003DD14(4);
    func_8003DD14(5);

    deactivatePauseScreenSprites(2);
    deactivatePauseScreenSprites(3);
    deactivatePauseScreenSprites(8);
    deactivatePauseScreenSprites(9);

    func_80053088();

}

INCLUDE_RODATA("asm/nonmatchings/game/overlayScreens", D_801220CC);

INCLUDE_RODATA("asm/nonmatchings/game/overlayScreens", D_80122140);

// jtbl_801221B8
INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CD750);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CD928);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CDA6C);

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CDFB0);

void func_800CDFB0(void) {
    updateSpriteAlpha(0x81, 0xFF, 24);
    updateSpriteAlpha(0x82, 0xFF, 24);
    updateSpriteAlpha(0x83, 0xFF, 24);
    updateSpriteAlpha(0x84, 0xFF, 24);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CE008);

void func_800CE008(void) {
    updateSpriteAlpha(0x81, 0, 24);
    updateSpriteAlpha(0x82, 0, 24);
    updateSpriteAlpha(0x83, 0, 24);
    updateSpriteAlpha(0x84, 0, 24);
    deactivatePauseScreenSprites(2);
}

//INCLUDE_RODATA("asm/nonmatchings/game/overlayScreens", D_801221CC);

static const s16 D_801221CC[6] = { 3000, 1000, 500, 0, 0, 0 };

// D_801221CC
// jtbl_801221D8
INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CE068);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CE714);

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CE7A8);

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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CE828);

bool func_800CE828(u8 arg0) {

    bool result = FALSE;

    switch (arg0) {
        case 0:
            if (checkLifeEventBit(0x57)) {
                result = TRUE;
            }
            break;
        case 1:
            if (checkLifeEventBit(0x59)) {
                result = TRUE;
            }
            break;
        case 2:
            break;
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CE894);

void func_800CE894(u8 arg0) {

    switch (arg0) {                              
        case 0:
            setLifeEventBit(0x57);
            adjustDogAffection(10);
            return;
        case 1:
            setLifeEventBit(0x59);
            return;
        case 2:
            gLumber += adjustValue(gLumber, 500, MAX_LUMBER);
            return;
        }
    
}


INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CE930);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CEA74);

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CED60);

void func_800CED60(void) {
    updateSpriteAlpha(0x81, 0xFF, 24);
    updateSpriteAlpha(0x82, 0xFF, 24);
    updateSpriteAlpha(0x83, 0xFF, 24);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CEDA8);

void func_800CEDA8(void) {
    updateSpriteAlpha(0x81, 0, 24);
    updateSpriteAlpha(0x82, 0, 24);
    updateSpriteAlpha(0x83, 0, 24);
}

// jtbl_80122200
// jtbl_80122220
// jtbl_80122238
INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CEDF0);

// jtbl_80122250
//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CF34C);

bool func_800CF34C(u8 arg0) {

    bool result = FALSE;
    
    switch (arg0) {
        case 3:
        case 4:
            break;
        case 0:
            if (checkLifeEventBit(0x5A)) {
                result = TRUE;
            }
            break;
        case 1:
            if (checkHaveKeyItem(FLOWER_BATH_CRYSTALS)) {
                result = TRUE;
            }
            break;
        case 2:
            if (checkLifeEventBit(0x5B)) {
                result = TRUE;
            }
            break;
    }

    return result;
    
}

// jtbl_80122268
//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CF3C4);

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
//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CF498);

bool func_800CF498(u8 arg0) {

    bool result = FALSE;
    
    switch (arg0) {

        case 0:
            if (checkLifeEventBit(HAVE_TABLECLOTH)) {
                result = TRUE;
            }
            break;
 
        case 1:
             if (checkLifeEventBit(0x5C)) {
                 result = TRUE;
             }
            break;
        
        case 2:
             if (checkLifeEventBit(0x5D)) {
                 result = TRUE;
             }
            break;
        case 3:
             if (!checkHaveTool(EMPTY_BOTTLE)) {
                 result = TRUE;
             }
            break;
        case 4:
            break;
    }

    return result;
    
}

// jtbl_80122298
INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CF544);
