#include "common.h"

#include "game/overlayScreens.h"

#include "system/audio.h"
#include "system/controller.h"
#include "system/globalSprites.h"
#include "system/math.h"
#include "system/message.h"
#include "system/overlayScreenSprites.h"

#include "game/animals.h"
#include "game/game.h"
#include "game/gameStatus.h"
#include "game/initialize.h"
#include "game/level.h"
#include "game/transition.h"
#include "game/itemHandlers.h"
#include "game/loadGameScreen.h"
#include "game/npc.h"
#include "game/player.h"
#include "game/shop.h"
#include "game/tv.h"

#include "mainLoop.h"

#include "ld_symbols.h"

// bss
extern OverlayScreenTable overlayScreenTable;
extern u32 D_80189858;
extern u32 D_80205204;
extern u8 D_801890E8[31];
extern u8 D_80189108[5][7];

// shared bss
extern u8 D_801C3F70;
extern u8 D_80205636;

extern u8 D_8018A72C[6];
extern u8 D_80205640[10];
extern u8 D_80237420[31];

// data
extern f32 D_80116C80[];
extern f32 D_80116C90;
extern f32 D_80116C94[];
extern f32 D_80116C9C;
extern f32 D_80116CA0[];
extern f32 D_80116CB0;
extern f32 D_80116CB4[];
extern f32 D_80116CBC;
extern f32 D_80116CC0[];
extern f32 D_80116CC8[];
extern f32 D_80116CD8[];
extern f32 D_80116CE8[];
extern f32 D_80116CF0[];
extern f32 D_80116D00;
extern f32 D_80116D04[];
extern f32 D_80116D0C;
extern f32 D_80116D10[];
extern f32 D_80116D20[];
extern f32 D_80116D28[];
extern f32 D_80116D3C[];
extern f32 D_80116D38;
extern f32 D_80116D44;
extern f32 D_80116D80[];
extern f32 D_80116D90[];
extern f32 D_80116DA0[];
extern f32 D_80116DB0[];
extern f32 D_80116DC0[5];
extern f32 D_80116DD4[5];
extern f32 D_80116E18[];
extern f32 D_80116E40[];
extern f32 D_80116E68[];
extern f32 D_80116E90[];
extern u32 D_80116F24[];
extern u16 D_80116F4C[];
extern u16 D_80116F88[];
extern u16 D_80117148[];

// forward declarations
void func_800B2F34(void); 
void func_800B4160(void);
void func_800B8844(void);
void func_800B90E0(void);
void func_800B99EC(void);
f32 func_800BA928(u8, u32);                          
f32 func_800BA9E8(u8, u32);                
void func_800BB590(void);
void func_800BBBC8(void);             
f32 func_800BE3B8(u8, u32);                          
f32 func_800BE428(u8, u32);
f32 func_800C0238(u8, u8);             
f32 func_800C02A8(u8, u8);    
u8 func_800C0688(u8 tool);
void func_800C0F24(void);                   
void func_800C1710(void);               
void func_800C2060(void);        
void func_800C2128(void);           
void func_800C2504(void);
inline void func_800C292C(void);
void func_800C3108(void);
void func_800C34C0(u8, u8, u8); 
void func_800C39A0(void);
void func_800C4E90(void);
void func_800C5080(void);
void func_800C6E98(void);
void func_800C5DA4(void);
f32 func_800BC700(u8, u32);
f32 func_800BC770(u8, u32);
void func_800C7850(void);
void func_800C7C88(f32 x, f32 y);
void func_800C7CE4(f32 x, f32 y);
void func_800C7DF8(void);
void func_800C7E18(void);
void func_800C7E38(void);
void func_800C7E58(void);
void func_800C7E78(void);
void func_800C81C8(void);
void func_800CC6F0(void);
void func_800CCFC8(void);
void func_800CD628(void);
void func_800CEA74(void);
bool func_800CF34C(u8 arg0);
void func_800CF3C4(u8 arg0);
bool func_800CF498(u8 arg0);
void func_800CF544(u8 arg0);
u32 checkRecipe(u16 bitIndex);

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
    deactivateOverlayScreenSprites(0);

    deactivateSprite(0x8E);
    deactivateOverlayScreenSprites(1);
    
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
    
    deactivateOverlayScreenSprites(0);
    deactivateOverlayScreenSprites(2);
    deactivateOverlayScreenSprites(3);
    deactivateOverlayScreenSprites(4);
    deactivateOverlayScreenSprites(5);
    deactivateOverlayScreenSprites(6);
    deactivateOverlayScreenSprites(2);
    deactivateOverlayScreenSprites(3);
    deactivateOverlayScreenSprites(4);
    deactivateOverlayScreenSprites(5);
    deactivateOverlayScreenSprites(6);
    deactivateOverlayScreenSprites(7);
    deactivateOverlayScreenSprites(8);
    deactivateOverlayScreenSprites(9);
    deactivateOverlayScreenSprites(0xA);
    deactivateOverlayScreenSprites(0xB);
    deactivateOverlayScreenSprites(0xC);
    deactivateOverlayScreenSprites(0xD);
    deactivateOverlayScreenSprites(0xE);

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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B2F34);

void func_800B2F34(void) {

    dmaSprite(0x89, (u32)&_timeUiTextureSegmentRomStart, (u32)&_timeUiTextureSegmentRomEnd, (u32)&_timeUiAssetsIndexSegmentRomStart, (u32)&_timeUiAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8027EC00, NULL, (u16*)0x80281E00, (u16*)0x80282200, (u8*)0x80282400, NULL, 0, 0);
    setSpriteViewSpacePosition(0x89, 0.0f, 0.0f, 256.0f);
    setBilinearFiltering(0x89, 1);
    setSpriteColor(0x89, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x89, 2);

    if (checkLifeEventBit(0x59)) {
        startSpriteAnimation(0x89, 6, 0);
    } else {
        startSpriteAnimation(0x89, 0, 0);
    }

    dmaSprite(0x8C, (u32)&_timeUiTextureSegmentRomStart, (u32)&_timeUiTextureSegmentRomEnd, (u32)&_timeUiAssetsIndexSegmentRomStart, (u32)&_timeUiAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8027EC00, NULL, (u16*)0x80281E00, (u16*)0x80282200, (u8*)0x80282400, NULL, 0, 0);
    setSpriteViewSpacePosition(0x8C, -79.0f, 81.0f, 256.0f);
    setSpriteRotation(0x8C, 0.0f, 0.0f, (360 - (gMinutes * 6)));
    setBilinearFiltering(0x8C, 1);
    setSpriteColor(0x8C, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x8C, 2);
    startSpriteAnimation(0x8C, 4, 0);
    
    dmaSprite(0x8D, (u32)&_timeUiTextureSegmentRomStart, (u32)&_timeUiTextureSegmentRomEnd, (u32)&_timeUiAssetsIndexSegmentRomStart, (u32)&_timeUiAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8027EC00, NULL, (u16*)0x80281E00, (u16*)0x80282200, (u8*)0x80282400, NULL, 0, 0);
    setSpriteViewSpacePosition(0x8D, -80.0f, 80.0f, 256.0f);
    setSpriteRotation(0x8D, 0.0f, 0.0f, ((360 - (gHour * 30)) - (gMinutes * 0.5)));
    setBilinearFiltering(0x8D, 1);
    setSpriteColor(0x8D, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x8D, 2);
    startSpriteAnimation(0x8D, 5, 0);
    
    dmaSprite(0x8B, (u32)&_timeUiTextureSegmentRomStart, (u32)&_timeUiTextureSegmentRomEnd, (u32)&_timeUiAssetsIndexSegmentRomStart, (u32)&_timeUiAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8027EC00, NULL, (u16*)0x80281E00, (u16*)0x80282200, (u8*)0x80282400, NULL, 0, 0);
    setSpriteViewSpacePosition(0x8B, 0.0f, 0.0f, 256.0f);
    setBilinearFiltering(0x8B, 1);
    setSpriteColor(0x8B, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x8B, 2);
    startSpriteAnimation(0x8B, 1, gSeason - 1);
    
    dmaSprite(0x8A, (u32)&_timeUiTextureSegmentRomStart, (u32)&_timeUiTextureSegmentRomEnd, (u32)&_timeUiAssetsIndexSegmentRomStart, (u32)&_timeUiAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8027EC00, NULL, (u16*)0x80281E00, (u16*)0x80282200, (u8*)0x80282400, NULL, 0, 0);
    setSpriteViewSpacePosition(0x8A, 0.0f, 0.0f, 256.0f);
    setBilinearFiltering(0x8A, 1);
    setSpriteColor(0x8A, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x8A, 2);
    startSpriteAnimation(0x8A, 3, gDayOfWeek);
    func_80045E20(0, 0x8F, (u32)&_timeUiTextureSegmentRomStart, (u32)&_timeUiTextureSegmentRomEnd, (u32)&_timeUiAssetsIndexSegmentRomStart, (u32)&_timeUiAssetsIndexSegmentRomEnd, (void*)0x8027EC00, (void*)0x80281E00, (void*)0x80282200, (void*)0x80282400, 0, 2, 0, 0.0f, 0.0f, 256.0f, 8);
    
    dmaOverlayScreenSprites(0, (u32)gDayOfMonth, 1, 3);
    setOverlayScreenSpritesRGBA(0, 0xFF, 0xFF, 0xFF, 0xFF);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B3498);

// unused or inline
void func_800B3498(f32 arg0, f32 arg1, f32 arg2, u16 arg3, u32 arg4) {

    dmaSprite(0x8E, (u32)&_moneyTextureSegmentRomStart, (u32)&_moneyTextureSegmentRomEnd, (u32)&_moneyAssetsIndexSegmentRomStart, (u32)&_moneyAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x80253B00, NULL, (u16*)0x80254A00, (u16*)0x80254E00, (u8*)0x80254F00, NULL, 0, 0);
    setSpriteViewSpacePosition(0x8E, arg0, arg1, arg2);
    setBilinearFiltering(0x8E, 1);
    setSpriteColor(0x8E, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x8E, arg3 & 0xFFFF);
    startSpriteAnimation(0x8E, 0, 0);

    func_80045E20(1, 0x91, (u32)&_moneyTextureSegmentRomStart, (u32)&_moneyTextureSegmentRomEnd, (u32)&_moneyAssetsIndexSegmentRomStart, (u32)&_moneyAssetsIndexSegmentRomEnd, (void*)0x80253B00, (void*)0x80254A00, (void*)0x80254E00, (void*)0x80254F00, 0, 1, 0, arg0, arg1, arg2 + 64.0f, 0xA);

    dmaOverlayScreenSprites(1, arg4, 5, 3);
    setOverlayScreenSpritesRGBA(1, 0xFF, 0xFF, 0xFF, 0xFF);

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B3694);

void func_800B3694(void) {

    dmaSprite(0x86, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8023B400, NULL, (u16*)0x8023CC00, (u16*)0x8023CE00, (u8*)0x8023D200, NULL, 0, 0);
    setSpriteViewSpacePosition(0x86, 80.0f, 32.0f, 256.0f);
    setBilinearFiltering(0x86, 1);
    setSpriteColor(0x86, 0xFF, 0xFF, 0xFF, 0xFF);
    startSpriteAnimation(0x86, 0, 0);
    
    dmaSprite(0x82, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8023B400, NULL, (u16*)0x8023CC00, (u16*)0x8023CE00, (u8*)0x8023D200, NULL, 0, 0);
    setSpriteViewSpacePosition(0x82, 80.0f, 32.0f, 256.0f);
    setBilinearFiltering(0x82, 1);
    setSpriteColor(0x82, 0xFF, 0xFF, 0xFF, 0xFF);
    startSpriteAnimation(0x82, 0, 1);
    
    dmaSprite(0x83, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8023B400, NULL, (u16*)0x8023CC00, (u16*)0x8023CE00, (u8*)0x8023D200, NULL, 0, 0);
    setSpriteViewSpacePosition(0x83, 80.0f, 32.0f, 256.0f);
    setBilinearFiltering(0x83, 1);
    setSpriteColor(0x83, 0xFF, 0xFF, 0xFF, 0xFF);
    startSpriteAnimation(0x83, 0, 3);
    
    dmaSprite(0x84, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8023B400, NULL, (u16*)0x8023CC00, (u16*)0x8023CE00, (u8*)0x8023D200, NULL, 0, 0);
    setSpriteViewSpacePosition(0x84, 80.0f, 32.0f, 256.0f);
    setBilinearFiltering(0x84, 1);
    setSpriteColor(0x84, 0xFF, 0xFF, 0xFF, 0xFF);
    startSpriteAnimation(0x84, 0, 5);
    
    dmaSprite(0x85, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8023B400, NULL, (u16*)0x8023CC00, (u16*)0x8023CE00, (u8*)0x8023D200, NULL, 0, 0);
    setSpriteViewSpacePosition(0x85, 80.0f, 32.0f, 256.0f);
    setBilinearFiltering(0x85, 1);
    setSpriteColor(0x85, 0xFF, 0xFF, 0xFF, 0xFF);
    startSpriteAnimation(0x85, 0, 7);
    
    func_800B3A60();

}

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

static inline void setMoneySprites() {
    dmaSprite(0x8E, (u32)&_moneyTextureSegmentRomStart, (u32)&_moneyTextureSegmentRomEnd, (u32)&_moneyAssetsIndexSegmentRomStart, (u32)&_moneyAssetsIndexSegmentRomEnd, 0, 0, 0x80253B00, 0, 0x80254A00, 0x80254E00, 0x80254F00, 0, 0, 0);
    setSpriteViewSpacePosition(0x8E, 0, 0, 256.0f);
    setBilinearFiltering(0x8E, TRUE);
    setSpriteColor(0x8E, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x8E, 2);
    startSpriteAnimation(0x8E, 0, 0);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B3BD8);

void func_800B3BD8(void) {
    
    u32 temp = gGold;

    setMoneySprites();
    
    func_80045E20(1, 0x91, (u32)&_moneyTextureSegmentRomStart, (u32)&_moneyTextureSegmentRomEnd, (u32)&_moneyAssetsIndexSegmentRomStart, (u32)&_moneyAssetsIndexSegmentRomEnd, 0x80253B00, 0x80254A00, 0x80254E00, 0x80254F00, 0, 1, 0, 0, 0, 320.0f, 0xA);
    dmaOverlayScreenSprites(1, temp, 5, 3);
    setOverlayScreenSpritesRGBA(1, 0xFF, 0xFF, 0xFF, 0xFF);
    setSpriteAlpha(0x8E, 0xFF);
    
    dmaSprite(0x82, (u32)&_shopIconsTextureSegmentRomStart, (u32)&_shopIconsTextureSegmentRomEnd, (u32)&_shopIconsAssetsIndexSegmentRomStart, (u32)&_shopIconsAssetsIndexSegmentRomEnd, 0, 0, 0x802E0EC0, 0, 0x802E27C0, 0x802E28C0, 0x802E2BC0, 0, 0, 0);
    setSpriteViewSpacePosition(0x82, -96.0f, 64.0f, 256.0f);
    setBilinearFiltering(0x82, TRUE);
    setSpriteColor(0x82, 0xFF, 0xFF, 0xFF, 0xFF);
    startSpriteAnimation(0x82, 0, 0);
    
    dmaSprite(0x83, (u32)&_shopIconsTextureSegmentRomStart, (u32)&_shopIconsTextureSegmentRomEnd, (u32)&_shopIconsAssetsIndexSegmentRomStart, (u32)&_shopIconsAssetsIndexSegmentRomEnd, 0, 0, 0x802E0EC0, 0, 0x802E27C0, 0x802E28C0, 0x802E2BC0, 0, 0, 0);
    setSpriteViewSpacePosition(0x83, -56.0f, 64.0f, 256.0f);
    setBilinearFiltering(0x83, TRUE);
    setSpriteColor(0x83, 0xFF, 0xFF, 0xFF, 0xFF);
    startSpriteAnimation(0x83, 1, 0);
    
    dmaSprite(0x84, (u32)&_shopIconsTextureSegmentRomStart, (u32)&_shopIconsTextureSegmentRomEnd, (u32)&_shopIconsAssetsIndexSegmentRomStart, (u32)&_shopIconsAssetsIndexSegmentRomEnd, 0, 0, 0x802E0EC0, 0, 0x802E27C0, 0x802E28C0, 0x802E2BC0, 0, 0, 0);
    setBilinearFiltering(0x84, TRUE);
    setSpriteColor(0x84, 0xFF, 0xFF, 0xFF, 0xFF);
    
    dmaSprite(0x85, (u32)&_shopIconsTextureSegmentRomStart, (u32)&_shopIconsTextureSegmentRomEnd, (u32)&_shopIconsAssetsIndexSegmentRomStart, (u32)&_shopIconsAssetsIndexSegmentRomEnd, 0, 0, 0x802E0EC0, 0, 0x802E27C0, 0x802E28C0, 0x802E2BC0, 0, 0, 0);
    setSpriteViewSpacePosition(0x85, -12.0f, 13.0f, 256.0f);
    setBilinearFiltering(0x85, TRUE);
    setSpriteColor(0x85, 0xFF, 0xFF, 0xFF, 0xFF);
    
    dmaSprite(0x86, (u32)&_shopIconsTextureSegmentRomStart, (u32)&_shopIconsTextureSegmentRomEnd, (u32)&_shopIconsAssetsIndexSegmentRomStart, (u32)&_shopIconsAssetsIndexSegmentRomEnd, 0, 0, 0x802E0EC0, 0, 0x802E27C0, 0x802E28C0, 0x802E2BC0, 0, 0, 0);
    setSpriteViewSpacePosition(0x86, -12.0f, 13.0f, 256.0f);
    setBilinearFiltering(0x86, TRUE);
    setSpriteColor(0x86, 0xFF, 0xFF, 0xFF, 0xFF);
    
    dmaSprite(0x87, (u32)&_shopIconsTextureSegmentRomStart, (u32)&_shopIconsTextureSegmentRomEnd, (u32)&_shopIconsAssetsIndexSegmentRomStart, (u32)&_shopIconsAssetsIndexSegmentRomEnd, 0, 0, 0x802E0EC0, 0, 0x802E27C0, 0x802E28C0, 0x802E2BC0, 0, 0, 0);
    setSpriteViewSpacePosition(0x87, -16.0f, 11.0f, 224.0f);
    setBilinearFiltering(0x87, TRUE);
    setSpriteColor(0x87, 0xFF, 0xFF, 0xFF, 0xFF);

    func_800B4160();
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B4160);

void func_800B4160(void) {

    if (!shopContext.unk_5) {
        resetAnimationState(0x82);
        resetAnimationState(0x83);
        resetAnimationState(0x84);
        setSpriteViewSpacePosition(0x84, -96.0f, 64.0f, 256.0f);
        startSpriteAnimation(0x82, 7, 0xFE);
        startSpriteAnimation(0x83, 1, 0);
        startSpriteAnimation(0x84, 5, 0);
    } else {
        resetAnimationState(0x82);
        resetAnimationState(0x83);
        resetAnimationState(0x84);
        setSpriteViewSpacePosition(0x84, -56.0f, 64.0f, 256.0f);
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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B42E0);

void func_800B42E0(void) {

    dmaSprite(0x80, (u32)&_checkerboardBackgroundTextureSegmentRomStart, (u32)&_checkerboardBackgroundTextureSegmentRomEnd, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomStart, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x802DE5C0, NULL, (u16*)0x802E0BC0, (u16*)0x802E0CC0, (u8*)0x802E0DC0, NULL, 0, 0);
    setSpriteViewSpacePosition(0x80, 0.0f, 0.0f, 0.0f);
    setSpriteScale(0x80, 2.0f, 2.0f, 1.0f);
    setBilinearFiltering(0x80, 1);
    setSpriteColor(0x80, 0, 0, 0, 0);
    startSpriteAnimation(0x80, 0, 0);
    
    if (loadGameScreenContext.showControllerPakScreen == FALSE) {
        
        dmaSprite(0x82, (u32)&_diaryTextureSegmentRomStart, (u32)&_diaryTextureSegmentRomEnd, (u32)&_diaryAssetsIndexSegmentRomStart, (u32)&_diaryAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
        setSpriteViewSpacePosition(0x82, 0.0f, 0.0f, 8.0f);
        setSpriteColor(0x82, 0, 0, 0, 0);
        setSpriteRenderingLayer(0x82, 3);
        setBilinearFiltering(0x82, 1);
        startSpriteAnimation(0x82, 0, 0);
        
        dmaSprite(0x83, (u32)&_diaryTextureSegmentRomStart, (u32)&_diaryTextureSegmentRomEnd, (u32)&_diaryAssetsIndexSegmentRomStart, (u32)&_diaryAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
        setSpriteViewSpacePosition(0x83, 0.0f, 0.0f, 8.0f);
        setSpriteColor(0x83, 0, 0, 0, 0);
        setSpriteRenderingLayer(0x83, 3);
        setBilinearFiltering(0x83, 1);
        startSpriteAnimation(0x83, 3, 0);
        
        dmaSprite(0x84, (u32)&_diaryTextureSegmentRomStart, (u32)&_diaryTextureSegmentRomEnd, (u32)&_diaryAssetsIndexSegmentRomStart, (u32)&_diaryAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
        setSpriteViewSpacePosition(0x84, 0.0f, 0.0f, 8.0f);
        setSpriteColor(0x84, 0, 0, 0, 0);
        setSpriteRenderingLayer(0x84, 3);
        setBilinearFiltering(0x84, 1);
        startSpriteAnimation(0x84, 1, 0);
        
        dmaSprite(0xA3, (u32)&_diaryTextureSegmentRomStart, (u32)&_diaryTextureSegmentRomEnd, (u32)&_diaryAssetsIndexSegmentRomStart, (u32)&_diaryAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
        setSpriteViewSpacePosition(0xA3, 0.0f, 0.0f, 8.0f);
        setSpriteColor(0xA3, 0, 0, 0, 0);
        setSpriteRenderingLayer(0xA3, 3);
        setBilinearFiltering(0xA3, 1);
        setSpritePaletteIndex(0xA3, 2);
        startSpriteAnimation(0xA3, 4, 0);
        
        dmaSprite(0xA4, (u32)&_diaryTextureSegmentRomStart, (u32)&_diaryTextureSegmentRomEnd, (u32)&_diaryAssetsIndexSegmentRomStart, (u32)&_diaryAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
        setSpriteViewSpacePosition(0xA4, 0.0f, -44.0f, 8.0f);
        setSpriteColor(0xA4, 0, 0, 0, 0);
        setSpriteRenderingLayer(0xA4, 3);
        setBilinearFiltering(0xA4, 1);
        setSpritePaletteIndex(0xA4, 3);
        startSpriteAnimation(0xA4, 4, 0);
        
        dmaSprite(0xA5, (u32)&_diaryTextureSegmentRomStart, (u32)&_diaryTextureSegmentRomEnd, (u32)&_diaryAssetsIndexSegmentRomStart, (u32)&_diaryAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
        setSpriteViewSpacePosition(0xA5, 0.0f, -88.0f, 8.0f);
        setSpriteColor(0xA5, 0, 0, 0, 0);
        setSpriteRenderingLayer(0xA5, 3);
        setBilinearFiltering(0xA5, 1);
        setSpritePaletteIndex(0xA5, 4);
        startSpriteAnimation(0xA5, 4, 0);
        
        dmaSprite(0xA6, (u32)&_diaryTextureSegmentRomStart, (u32)&_diaryTextureSegmentRomEnd, (u32)&_diaryAssetsIndexSegmentRomStart, (u32)&_diaryAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
        setSpriteViewSpacePosition(0xA6, 0.0f, -132.0f, 8.0f);
        setSpriteColor(0xA6, 0, 0, 0, 0);
        setSpriteRenderingLayer(0xA6, 3);
        setBilinearFiltering(0xA6, 1);
        setSpritePaletteIndex(0xA6, 5);
        startSpriteAnimation(0xA6, 4, 0);
        
        dmaSprite(0xA7, (u32)&_diaryTextureSegmentRomStart, (u32)&_diaryTextureSegmentRomEnd, (u32)&_diaryAssetsIndexSegmentRomStart, (u32)&_diaryAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
        setSpriteViewSpacePosition(0xA7, 0.0f, 0.0f, 16.0f);
        setSpriteColor(0xA7, 0, 0, 0, 0);
        setSpriteRenderingLayer(0xA7, 3);
        setBilinearFiltering(0xA7, 1);
        startSpriteAnimation(0xA7, 5, 0);
        
        dmaSprite(0xA8, (u32)&_diaryTextureSegmentRomStart, (u32)&_diaryTextureSegmentRomEnd, (u32)&_diaryAssetsIndexSegmentRomStart, (u32)&_diaryAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
        setSpriteViewSpacePosition(0xA8, 0.0f, 0.0f, 16.0f);
        setSpriteColor(0xA8, 0, 0, 0, 0);
        setSpriteRenderingLayer(0xA8, 3);
        setBilinearFiltering(0xA8, 1);
        startSpriteAnimation(0xA8, 5, 2);
        
        dmaSprite(0xA9, (u32)&_diaryTextureSegmentRomStart, (u32)&_diaryTextureSegmentRomEnd, (u32)&_diaryAssetsIndexSegmentRomStart, (u32)&_diaryAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
        setSpriteViewSpacePosition(0xA9, 0.0f, 0.0f, 16.0f);
        setSpriteColor(0xA9, 0, 0, 0, 0);
        setSpriteRenderingLayer(0xA9, 3);
        setBilinearFiltering(0xA9, 1);
        startSpriteAnimation(0xA9, 5, 4);
        
        
        dmaSprite(0xAA, (u32)&_diaryTextureSegmentRomStart, (u32)&_diaryTextureSegmentRomEnd, (u32)&_diaryAssetsIndexSegmentRomStart, (u32)&_diaryAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
        setSpriteViewSpacePosition(0xAA, 0.0f, 0.0f, 16.0f);
        setSpriteColor(0xAA, 0, 0, 0, 0);
        setSpriteRenderingLayer(0xAA, 3);
        setBilinearFiltering(0xAA, 1);
        startSpriteAnimation(0xAA, 5, 6);
        
        dmaSprite(0x85, (u32)&_diaryTextureSegmentRomStart, (u32)&_diaryTextureSegmentRomEnd, (u32)&_diaryAssetsIndexSegmentRomStart, (u32)&_diaryAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
        setSpriteViewSpacePosition(0x85, 0.0f, 0.0f, 4.0f);
        setSpriteColor(0x85, 0, 0, 0, 0);
        setSpriteRenderingLayer(0x85, 3);
        setBilinearFiltering(0x85, 1);
        setSpritePaletteIndex(0x85, 1);
        startSpriteAnimation(0x85, 2, 0);
        
        dmaSprite(0x86, (u32)&_diaryTextureSegmentRomStart, (u32)&_diaryTextureSegmentRomEnd, (u32)&_diaryAssetsIndexSegmentRomStart, (u32)&_diaryAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
        setSpriteViewSpacePosition(0x86, 0.0f, -44.0f, 4.0f);
        setSpriteColor(0x86, 0, 0, 0, 0);
        setSpriteRenderingLayer(0x86, 3);
        setBilinearFiltering(0x86, 1);
        setSpritePaletteIndex(0x86, 1);
        startSpriteAnimation(0x86, 2, 0);
        
        dmaSprite(0x87, (u32)&_diaryTextureSegmentRomStart, (u32)&_diaryTextureSegmentRomEnd, (u32)&_diaryAssetsIndexSegmentRomStart, (u32)&_diaryAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
        setSpriteViewSpacePosition(0x87, 0.0f, -88.0f, 4.0f);
        setSpriteColor(0x87, 0, 0, 0, 0);
        setSpriteRenderingLayer(0x87, 3);
        setBilinearFiltering(0x87, 1);
        setSpritePaletteIndex(0x87, 1);
        startSpriteAnimation(0x87, 2, 0);
        
        dmaSprite(0x88, (u32)&_diaryTextureSegmentRomStart, (u32)&_diaryTextureSegmentRomEnd, (u32)&_diaryAssetsIndexSegmentRomStart, (u32)&_diaryAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
        setSpriteViewSpacePosition(0x88, 0.0f, -132.0f, 4.0f);
        setSpriteColor(0x88, 0, 0, 0, 0);
        setSpriteRenderingLayer(0x88, 3);
        setBilinearFiltering(0x88, 1);
        setSpritePaletteIndex(0x88, 1);
        startSpriteAnimation(0x88, 2, 0);
        
        dmaSprite(0x79, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8023B400, NULL, (u16*)0x8023CC00, (u16*)0x8023CE00, (u8*)0x8023D200, NULL, 0, 0);
        setSpriteViewSpacePosition(0x79, 0.0f, 0.0f, 32.0f);
        setBilinearFiltering(0x79, 1);
        setSpriteColor(0x79, 0xFF, 0xFF, 0xFF, 0xFF);
        setSpriteRenderingLayer(0x79, 3);
        
        dmaSprite(0xB2, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8023B400, NULL, (u16*)0x8023CC00, (u16*)0x8023CE00, (u8*)0x8023D200, NULL, 0, 0);
        setSpriteViewSpacePosition(0xB2, 0.0f, 0.0f, 32.0f);
        setBilinearFiltering(0xB2, 1);
        setSpriteColor(0xB2, 0xFF, 0xFF, 0xFF, 0x80);
        setSpriteRenderingLayer(0xB2, 2);

        dmaSprite(0xAC, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8023B400, NULL, (u16*)0x8023CC00, (u16*)0x8023CE00, (u8*)0x8023D200, NULL, 0, 0);
        setSpriteViewSpacePosition(0xAC, 0.0f, 0.0f, 32.0f);
        setBilinearFiltering(0xAC, 1);
        setSpriteColor(0xAC, 0xFF, 0xFF, 0xFF, 0xFF);
        setSpriteRenderingLayer(0xAC, 3);
        
        func_800C7850();
        
    } else {
        
        dmaSprite(0x82, (u32)&_diaryTextureSegmentRomStart, (u32)&_diaryTextureSegmentRomEnd, (u32)&_diaryAssetsIndexSegmentRomStart, (u32)&_diaryAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
        setSpriteViewSpacePosition(0x82, 0.0f, 0.0f, 8.0f);
        setSpriteColor(0x82, 0, 0, 0, 0);
        setSpriteRenderingLayer(0x82, 3);
        setBilinearFiltering(0x82, 1);
        startSpriteAnimation(0x82, 7, 0);
        
        dmaSprite(0x83, (u32)&_diaryTextureSegmentRomStart, (u32)&_diaryTextureSegmentRomEnd, (u32)&_diaryAssetsIndexSegmentRomStart, (u32)&_diaryAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
        setSpriteViewSpacePosition(0x83, 0.0f, 0.0f, 8.0f);
        setSpriteColor(0x83, 0, 0, 0, 0);
        setSpriteRenderingLayer(0x83, 3);
        setBilinearFiltering(0x83, 1);
        startSpriteAnimation(0x83, 3, 0);
        
        dmaSprite(0x84, (u32)&_diaryTextureSegmentRomStart, (u32)&_diaryTextureSegmentRomEnd, (u32)&_diaryAssetsIndexSegmentRomStart, (u32)&_diaryAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
        setSpriteViewSpacePosition(0x84, 0.0f, 0.0f, 8.0f);
        setSpriteColor(0x84, 0, 0, 0, 0);
        setSpriteRenderingLayer(0x84, 3);
        setBilinearFiltering(0x84, 1);
        startSpriteAnimation(0x84, 1, 0);
        
        dmaSprite(0xA3, (u32)&_diaryTextureSegmentRomStart, (u32)&_diaryTextureSegmentRomEnd, (u32)&_diaryAssetsIndexSegmentRomStart, (u32)&_diaryAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
        setSpriteViewSpacePosition(0xA3, 0.0f, 0.0f, 8.0f);
        setSpriteColor(0xA3, 0, 0, 0, 0);
        setSpriteRenderingLayer(0xA3, 3);
        setBilinearFiltering(0xA3, 1);
        setSpritePaletteIndex(0xA3, 2);
        startSpriteAnimation(0xA3, 8, 0);
        
        dmaSprite(0xA4, (u32)&_diaryTextureSegmentRomStart, (u32)&_diaryTextureSegmentRomEnd, (u32)&_diaryAssetsIndexSegmentRomStart, (u32)&_diaryAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
        setSpriteViewSpacePosition(0xA4, 0.0f, -44.0f, 8.0f);
        setSpriteColor(0xA4, 0, 0, 0, 0);
        setSpriteRenderingLayer(0xA4, 3);
        setBilinearFiltering(0xA4, 1);
        setSpritePaletteIndex(0xA4, 3);
        startSpriteAnimation(0xA4, 8, 0);
        
        dmaSprite(0xA7, (u32)&_diaryTextureSegmentRomStart, (u32)&_diaryTextureSegmentRomEnd, (u32)&_diaryAssetsIndexSegmentRomStart, (u32)&_diaryAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
        setSpriteViewSpacePosition(0xA7, 0.0f, 0.0f, 16.0f);
        setSpriteColor(0xA7, 0, 0, 0, 0);
        setSpriteRenderingLayer(0xA7, 3);
        setBilinearFiltering(0xA7, 1);
        startSpriteAnimation(0xA7, 5, 0);
        
        dmaSprite(0xA8, (u32)&_diaryTextureSegmentRomStart, (u32)&_diaryTextureSegmentRomEnd, (u32)&_diaryAssetsIndexSegmentRomStart, (u32)&_diaryAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
        setSpriteViewSpacePosition(0xA8, 0.0f, 0.0f, 16.0f);
        setSpriteColor(0xA8, 0, 0, 0, 0);
        setSpriteRenderingLayer(0xA8, 3);
        setBilinearFiltering(0xA8, 1);
        startSpriteAnimation(0xA8, 5, 2);
        
        dmaSprite(0x85, (u32)&_diaryTextureSegmentRomStart, (u32)&_diaryTextureSegmentRomEnd, (u32)&_diaryAssetsIndexSegmentRomStart, (u32)&_diaryAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
        setSpriteViewSpacePosition(0x85, 0.0f, 0.0f, 4.0f);
        setSpriteColor(0x85, 0, 0, 0, 0);
        setSpriteRenderingLayer(0x85, 3);
        setBilinearFiltering(0x85, 1);
        setSpritePaletteIndex(0x85, 1);
        startSpriteAnimation(0x85, 2, 0);
        
        dmaSprite(0x86, (u32)&_diaryTextureSegmentRomStart, (u32)&_diaryTextureSegmentRomEnd, (u32)&_diaryAssetsIndexSegmentRomStart, (u32)&_diaryAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
        setSpriteViewSpacePosition(0x86, 0.0f, -44.0f, 4.0f);
        setSpriteColor(0x86, 0, 0, 0, 0);
        setSpriteRenderingLayer(0x86, 3);
        setBilinearFiltering(0x86, 1);
        setSpritePaletteIndex(0x86, 1);
        startSpriteAnimation(0x86, 2, 0);
        
        dmaSprite(0xAB, (u32)&_diaryTextureSegmentRomStart, (u32)&_diaryTextureSegmentRomEnd, (u32)&_diaryAssetsIndexSegmentRomStart, (u32)&_diaryAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
        setSpriteViewSpacePosition(0xAB, 0.0f, 0.0f, 8.0f);
        setSpriteColor(0xAB, 0, 0, 0, 0);
        setSpriteRenderingLayer(0xAB, 3);
        setBilinearFiltering(0xAB, 1);
        startSpriteAnimation(0xAB, 9, 0);
        
        dmaSprite(0x79, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8023B400, NULL, (u16*)0x8023CC00, (u16*)0x8023CE00, (u8*)0x8023D200, NULL, 0, 0);
        setSpriteViewSpacePosition(0x79, 0.0f, 0.0f, 32.0f);
        setBilinearFiltering(0x79, 1);
        setSpriteColor(0x79, 0xFF, 0xFF, 0xFF, 0xFF);
        setSpriteRenderingLayer(0x79, 3);
        
        dmaSprite(0xB2, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8023B400, NULL, (u16*)0x8023CC00, (u16*)0x8023CE00, (u8*)0x8023D200, NULL, 0, 0);
        setSpriteViewSpacePosition(0xB2, 0.0f, 0.0f, 32.0f);
        setBilinearFiltering(0xB2, 1);
        setSpriteColor(0xB2, 0xFF, 0xFF, 0xFF, 0x80);
        setSpriteRenderingLayer(0xB2, 2);

        dmaSprite(0xAC, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8023B400, NULL, (u16*)0x8023CC00, (u16*)0x8023CE00, (u8*)0x8023D200, NULL, 0, 0);
        setSpriteViewSpacePosition(0xAC, 0.0f, 0.0f, 32.0f);
        setBilinearFiltering(0xAC, 1);
        setSpriteColor(0xAC, 0xFF, 0xFF, 0xFF, 0xFF);
        setSpriteRenderingLayer(0xAC, 3);
        
        func_800C7850();
        
    }
    
}

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
            // FIXME: fake
            temp = 0xB2;
            return;
        }

    } else {
        
        if (arg1 != 0xFF) {
            setSpriteViewSpacePosition(0x79, D_80116D80[arg1], D_80116D90[arg1], 32.0f);
        }

        if (arg2 != 0xFF) {
            setSpriteViewSpacePosition(0x79, D_80116DA0[arg2], D_80116DB0[arg2], 32.0f);
        }
        
        startSpriteAnimation(0x79, 3, 0);
        return;

    }
    
    if (arg1 != 0xFF) {
        setSpriteViewSpacePosition(0xB2, D_80116D80[arg1], D_80116D90[arg1], 32.0f);
    }

    if (arg2 != 0xFF) {
        setSpriteViewSpacePosition(0xB2, arg2, arg2, 32.0f);
    }
    
    // FIXME: fake
    temp = 0xB2;

    startSpriteAnimation(0xB2, 3, 0);

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B60E4);

void func_800B60E4(u8 arg0, u8 arg1, u8 arg2) {

    if (loadGameScreenContext.showControllerPakScreen == FALSE) {

        if (arg0 != 0xFF) {

            resetAnimationState(0xA3 + arg0);
            resetAnimationState(0xA7 + arg0);

            if (arg2) {

                startSpriteAnimation(0xA3 + arg0, 4, 1);
                setSpritePaletteIndex(0x85 + arg0, 0);

                switch (arg0) {

                    case 0:
                        startSpriteAnimation(0xA7, 5, 1);
                        break;
                    case 1:
                        startSpriteAnimation(0xA8, 5, 3);
                        break;
                    case 2:
                        startSpriteAnimation(0xA9, 5, 5);
                        break;
                    case 3:
                        startSpriteAnimation(0xAA, 5, 7);
                        break;
                    
                }
                
            } else {

                startSpriteAnimation(0xA3 + arg0, 4, 0);
                setSpritePaletteIndex(0x85 + arg0, 1);

                switch (arg0) {

                    case 0:
                        startSpriteAnimation(0xA7, 5, 0);
                        break;
                    case 1:
                        startSpriteAnimation(0xA8, 5, 2);
                        break;
                    case 2:
                        startSpriteAnimation(0xA9, 5, 4);
                        break;
                    case 3:
                        startSpriteAnimation(0xAA, 5, 6);
                        break;
                    
                }
                
            }
            
        }

        if (loadGameScreenContext.gamePakEnabled) {
            func_800C7C88(64.0f, -108.0f);
        } else {
            func_800C7DF8();
        }
        
    } else {

        if (arg0 != 0xFF)  {

            resetAnimationState(0xA3 + arg0);
            resetAnimationState(0xA7 + arg0);

            if (arg2) {
                
                startSpriteAnimation(0xA3 + arg0, 8, 1);
                setSpritePaletteIndex(0x85 + arg0, 0);

                switch (arg0) {

                    case 0:
                        startSpriteAnimation(0xA7, 5, 1);
                        break;
                    case 1:
                        startSpriteAnimation(0xA8, 5, 3);
                        break;
                    
                }
                
            } else {

                startSpriteAnimation(0xA3 + arg0, 8, 0);
                setSpritePaletteIndex(0x85 + arg0, 1);

                switch (arg0) {

                    case 0:
                        startSpriteAnimation(0xA7, 5, 0);
                        break;
                    case 1:
                        startSpriteAnimation(0xA8, 5, 2);
                        break;
                    
                }
                
            }

        }
        
        if (loadGameScreenContext.gamePakEnabled) {
            func_800C7CE4(64.0f, 80.0f);
        } else {
            func_800C7E18();
        }

    }
    
    if (arg1 != 0xFF) {

        if (arg2) {

            switch (arg1) {
                
                case 0:
                    resetAnimationState(0x83);
                    startSpriteAnimation(0x83, 3, 1);
                    break;

                case 1:
                    resetAnimationState(0x84);
                    startSpriteAnimation(0x84, 1, 1);
                    break;

                case 2:
                    resetAnimationState(0xAD);
                    startSpriteAnimation(0xAD, 6, 1);
                    break;

            }
            
        } else {

            switch (arg1) {
                
                case 0:
                    resetAnimationState(0x83);
                    startSpriteAnimation(0x83, 3, 0);
                    break;

                case 1:
                    resetAnimationState(0x84);
                    startSpriteAnimation(0x84, 1, 0);
                    break;

                case 2:
                    resetAnimationState(0xAD);
                    startSpriteAnimation(0xAD, 6, 0);
                    break;

            }
            
        }
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B64E4);

void func_800B64E4(u8 arg0, u8 arg1) {

    if (!loadGameScreenContext.showControllerPakScreen) {

        if (arg0 != 0xFF) {

            resetAnimationState(arg0 + 0xA3);
            startSpriteAnimation(arg0 + 0xA3, 4, 0xFF);
            
            switch (arg0) {

                case 0:
                    resetAnimationState(0xA7);
                    break; 
                case 1:
                    resetAnimationState(0xA8);
                    break;
                case 2:
                    resetAnimationState(0xA9);
                    break;
                case 3:
                    resetAnimationState(0xAA);
                    break;
                
            }
            
        }
        
    } else {

        if (arg0 != 0xFF) {

            resetAnimationState(arg0 + 0xA3);
            startSpriteAnimation(arg0 + 0xA3, 8, 0xFF);
            
            switch (arg0) {

                case 0:
                    resetAnimationState(0xA7);
                    break; 
                case 1:
                    resetAnimationState(0xA8);
                    break;

            }
            
        }
        
    }

    if (arg1 != 0xFF) {
        
        switch (arg1) {
            case 0:
                resetAnimationState(0x83);
                startSpriteAnimation(0x83, 3, 2);
                break;
            case 1:
                resetAnimationState(0x84);
                startSpriteAnimation(0x84, 1, 2);
                break;    
            case 2:
                resetAnimationState(0xAD);
                startSpriteAnimation(0xAD, 6, 2);
                break; 
        }
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B6674);

void func_800B6674(void) {
    
    dmaSprite(0x80, (u32)&_namingScreenBackgroundTextureSegmentRomStart, (u32)&_namingScreenBackgroundTextureSegmentRomEnd, (u32)&_namingScreenBackgroundAssetsIndexSegmentRomStart, (u32)&_namingScreenBackgroundAssetsIndexSegmentRomEnd, 0, 0, 0x802DE5C0, NULL, 0x802E0BC0, 0x802E0CC0, 0x802E0DC0, NULL, 0, 0);
    setSpriteViewSpacePosition(0x80, 0.0f, 0.0f, 0.0f);
    setSpriteScale(0x80, 2.0f, 2.0f, 1.0f);
    setBilinearFiltering(0x80, 1);
    setSpriteColor(0x80, 0, 0, 0, 0);
    startSpriteAnimation(0x80, 0, 0);
    
    dmaSprite(0x82, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, 0, 0, 0x8026F000, NULL, 0x8027E200, 0x8027E700, 0x8027EB00, NULL, 0, 0);
    setSpriteViewSpacePosition(0x82, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x82, 0, 0, 0, 0);
    setSpriteRenderingLayer(0x82, 2);
    setBilinearFiltering(0x82, 1);
    startSpriteAnimation(0x82, 0, 0);
    
    dmaSprite(0x83, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, 0, 0, 0x8026F000, NULL, 0x8027E200, 0x8027E700, 0x8027EB00, NULL, 0, 0);
    setSpriteViewSpacePosition(0x83, 0.0f, 40.0f, 8.0f);
    setSpriteColor(0x83, 0, 0, 0, 0);
    setSpriteRenderingLayer(0x83, 3);
    setBilinearFiltering(0x83, 1);
    startSpriteAnimation(0x83, 1, 0);
    
    dmaSprite(0x84, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, 0, 0, 0x8026F000, NULL, 0x8027E200, 0x8027E700, 0x8027EB00, NULL, 0, 0);
    setSpriteViewSpacePosition(0x84, 0.0f, 8.0f, 8.0f);
    setSpriteColor(0x84, 0, 0, 0, 0);
    setSpriteRenderingLayer(0x84, 3);
    setBilinearFiltering(0x84, 1);
    startSpriteAnimation(0x84, 1, 1);
    
    dmaSprite(0x85, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, 0, 0, 0x8026F000, NULL, 0x8027E200, 0x8027E700, 0x8027EB00, NULL, 0, 0);
    setSpriteViewSpacePosition(0x85, 0.0f, -24.0f, 8.0f);
    setSpriteColor(0x85, 0, 0, 0, 0);
    setSpriteRenderingLayer(0x85, 3);
    setBilinearFiltering(0x85, 1);
    startSpriteAnimation(0x85, 1, 2);
    
    dmaSprite(0x86, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, 0, 0, 0x8026F000, NULL, 0x8027E200, 0x8027E700, 0x8027EB00, NULL, 0, 0);
    setSpriteViewSpacePosition(0x86, 0.0f, -56.0f, 8.0f);
    setSpriteColor(0x86, 0, 0, 0, 0);
    setSpriteRenderingLayer(0x86, 3);
    setBilinearFiltering(0x86, 1);
    startSpriteAnimation(0x86, 1, 3);
    
    dmaSprite(0x87, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, 0, 0, 0x8026F000, NULL, 0x8027E200, 0x8027E700, 0x8027EB00, NULL, 0, 0);
    setSpriteViewSpacePosition(0x87, 0.0f, -88.0f, 8.0f);
    setSpriteColor(0x87, 0, 0, 0, 0);
    setSpriteRenderingLayer(0x87, 3);
    setBilinearFiltering(0x87, 1);
    startSpriteAnimation(0x87, 1, 4);
    
    if (D_801FB994[0] & 1) {
        dmaSprite(0xAE, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, 0, 0, 0x8026F000, NULL, 0x8027E200, 0x8027E700, 0x8027EB00, NULL, 0, 0);
        setSpriteViewSpacePosition(0xAE, -4.0f, 40.0f, 16.0f);
        setSpriteColor(0xAE, 0, 0, 0, 0);
        setSpriteRenderingLayer(0xAE, 3);
        setBilinearFiltering(0xAE, 1);
        startSpriteAnimation(0xAE, 7, D_801FB80B[0] - 1);
    }
    
    if (D_801FB994[1] & 1) {
        dmaSprite(0xAF, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, 0, 0, 0x8026F000, NULL, 0x8027E200, 0x8027E700, 0x8027EB00, NULL, 0, 0);
        setSpriteViewSpacePosition(0xAF, -4.0f, 8.0f, 16.0f);
        setSpriteColor(0xAF, 0, 0, 0, 0);
        setSpriteRenderingLayer(0xAF, 3);
        setBilinearFiltering(0xAF, 1);
        startSpriteAnimation(0xAF, 7, D_801FB80B[1] - 1);
    }
    
    if (D_801FB994[2] & 1) {
        dmaSprite(0xB0, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, 0, 0, 0x8026F000, NULL, 0x8027E200, 0x8027E700, 0x8027EB00, NULL, 0, 0);
        setSpriteViewSpacePosition(0xB0, -4.0f, -24.0f, 16.0f);
        setSpriteColor(0xB0, 0, 0, 0, 0);
        setSpriteRenderingLayer(0xB0, 3);
        setBilinearFiltering(0xB0, 1);
        startSpriteAnimation(0xB0, 7, D_801FB80B[2]- 1);
    }
    
    if (D_801FB994[3] & 1) {
        dmaSprite(0xB1, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, 0, 0, 0x8026F000, NULL, 0x8027E200, 0x8027E700, 0x8027EB00, NULL, 0, 0);
        setSpriteViewSpacePosition(0xB1, -4.0f, -56.0f, 16.0f);
        setSpriteColor(0xB1, 0, 0, 0, 0);
        setSpriteRenderingLayer(0xB1, 3);
        setBilinearFiltering(0xB1, 1);
        startSpriteAnimation(0xB1, 7, D_801FB80B[3] - 1);
    }
    
    if (D_801FB994[4] & 1) {
        dmaSprite(0xB2, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, 0, 0, 0x8026F000, NULL, 0x8027E200, 0x8027E700, 0x8027EB00, NULL, 0, 0);
        setSpriteViewSpacePosition(0xB2, -4.0f, -88.0f, 16.0f);
        setSpriteColor(0xB2, 0, 0, 0, 0);
        setSpriteRenderingLayer(0xB2, 3);
        setBilinearFiltering(0xB2, 1);
        startSpriteAnimation(0xB2, 7, D_801FB80B[4] - 1);
    }
    
    dmaSprite(0x79, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, 0, 0, 0x8023B400, NULL, 0x8023CC00, 0x8023CE00, 0x8023D200, NULL, 0, 0);
    setSpriteViewSpacePosition(0x79, 0.0f, 0.0f, 32.0f);
    setBilinearFiltering(0x79, 1);
    setSpriteColor(0x79, 0xFF, 0xFF, 0xFF, 0xFF);
    setSpriteRenderingLayer(0x79, 3);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B7070);

void func_800B7070(u8 arg0) {

    dmaSprite(0x82, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
    setSpriteViewSpacePosition(0x82, 0.0f, 80.0f, 8.0f);
    setSpriteColor(0x82, 0xFF, 0xFF, 0xFF, 0xFF);
    setSpriteRenderingLayer(0x82, 3);
    setBilinearFiltering(0x82, 1);
    startSpriteAnimation(0x82, 1, arg0);
    
    dmaSprite(0x81, (u32)&_snapshotLargeTextureSegmentRomStart, (u32)&_snapshotLargeTextureSegmentRomEnd, (u32)&_snapshotLargeAssetsIndexSegmentRomStart, (u32)&_snapshotLargeAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x80289900, NULL, (u16*)0x8028EF00, (u16*)0x8028F000, (u8*)0x8028F100, NULL, 0, 0);
    setSpriteViewSpacePosition(0x81, 0.0f, -16.0f, 8.0f);
    setSpriteColor(0x81, 0xFF, 0xFF, 0xFF, 0xFF);
    setSpriteRenderingLayer(0x81, 2);
    setBilinearFiltering(0x81, 1);
    startSpriteAnimation(0x81, 0, 0);
    
    dmaSprite(0x83, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
    setSpriteViewSpacePosition(0x83, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x83, 0xFF, 0xFF, 0xFF, 0xFF);
    setSpriteRenderingLayer(0x83, 2);
    setBilinearFiltering(0x83, 1);
    startSpriteAnimation(0x83, 2, 0);

    if (D_801FB994[arg0] & 2) {
        
        dmaSprite(0x84, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
        setSpriteViewSpacePosition(0x84, 0.0f, 0.0f, 8.0f);
        setSpriteColor(0x84, 0xFF, 0xFF, 0xFF, 0xFF);
        setSpriteRenderingLayer(0x84, 3);
        setBilinearFiltering(0x84, 1);
        startSpriteAnimation(0x84, 3, D_801FB971[arg0]);
        
        dmaSprite(0xAE, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
        setSpriteViewSpacePosition(0xAE, 0.0f, 32.0f, 8.0f);
        setSpriteColor(0xAE, 0xFF, 0xFF, 0xFF, 0xFF);
        setSpriteRenderingLayer(0xAE, 3);
        setBilinearFiltering(0xAE, 1);
        
        if (D_801FB858[arg0] == 0xFF) {
            startSpriteAnimation(0xAE, 6, 0xFE);
        } else {
            startSpriteAnimation(0xAE, 5, (D_801FB858[arg0] * 9) / 255);
        }
        
    }

    if (D_801FB994[arg0] & 4) {
        
        dmaSprite(0x85, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
        setSpriteViewSpacePosition(0x85, 0.0f, 0.0f, 8.0f);
        setSpriteColor(0x85, 0xFF, 0xFF, 0xFF, 0xFF);
        setSpriteRenderingLayer(0x85, 3);
        setBilinearFiltering(0x85, 1);
        startSpriteAnimation(0x85, 3, 5);
        
        dmaSprite(0xAF, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
        setSpriteViewSpacePosition(0xAF, 0.0f, 16.0f, 8.0f);
        setSpriteColor(0xAF, 0xFF, 0xFF, 0xFF, 0xFF);
        setSpriteRenderingLayer(0xAF, 3);
        setBilinearFiltering(0xAF, 1);

        if (D_801FB85F[arg0] == 0xFF) {
            startSpriteAnimation(0xAF, 6, 0xFE);
        } else {
            startSpriteAnimation(0xAF, 5, (D_801FB85F[arg0] * 9) / 255);
        }
    
    }

    if (D_801FB994[arg0] & 8) {
        
        dmaSprite(0x86, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
        setSpriteViewSpacePosition(0x86, 0.0f, 0.0f, 8.0f);
        setSpriteColor(0x86, 0xFF, 0xFF, 0xFF, 0xFF);
        setSpriteRenderingLayer(0x86, 3);
        setBilinearFiltering(0x86, 1);
        startSpriteAnimation(0x86, 3, 6);
        
        dmaSprite(0xB1, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
        setSpriteViewSpacePosition(0xB1, 0.0f, -16.0f, 8.0f);
        setSpriteColor(0xB1, 0xFF, 0xFF, 0xFF, 0xFF);
        setSpriteRenderingLayer(0xB1, 3);
        setBilinearFiltering(0xB1, 1);
        
        if (D_801FB86D[arg0] == 0xFF) {
            startSpriteAnimation(0xB1, 6, 0xFE);
        } else {
            startSpriteAnimation(0xB1, 5, (D_801FB86D[arg0] * 9) / 255);
        }
        
    }
    
    dmaSprite(0xB0, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
    setSpriteViewSpacePosition(0xB0, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0xB0, 0xFF, 0xFF, 0xFF, 0xFF);
    setSpriteRenderingLayer(0xB0, 3);
    setBilinearFiltering(0xB0, 1);
    
    if (D_801FB866[arg0] == 0xFF) {
        startSpriteAnimation(0xB0, 6, 0xFE);
    } else {
        startSpriteAnimation(0xB0, 5, (D_801FB866[arg0] * 9) / 255);
    }

    if (D_801FB994[arg0] & 1) {
        dmaSprite(0xB2, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
        setSpriteViewSpacePosition(0xB2, -4.0f, 80.0f, 16.0f);
        setSpriteColor(0xB2, 0xFF, 0xFF, 0xFF, 0xFF);
        setSpriteRenderingLayer(0xB2, 3);
        setBilinearFiltering(0xB2, 1);
        startSpriteAnimation(0xB2, 7, D_801FB80B[arg0] - 1);
    }
    
}

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
        setSpriteViewSpacePosition(0x79, D_80116DC0[arg0], D_80116DD4[arg0], 32.0f);
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

    dmaSprite(0x80, (u32)&_checkerboardBackgroundTextureSegmentRomStart, (u32)&_checkerboardBackgroundTextureSegmentRomEnd, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomStart, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomEnd, 0, 0, 0x802DE5C0, 0, 0x802E0BC0, 0x802E0CC0, 0x802E0DC0, 0, 0, 0);
    setSpriteViewSpacePosition(0x80, 0, 0, 0);
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

    dmaSprite(0x80, (u32)&_checkerboardBackgroundTextureSegmentRomStart, (u32)&_checkerboardBackgroundTextureSegmentRomEnd, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomStart, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomEnd, NULL, NULL, (void*)0x802DE5C0, NULL, (void*)0x802E0BC0, (void*)0x802E0CC0, (void*)0x802E0DC0, NULL, NULL, 0);
    setSpriteViewSpacePosition(0x80, 0.0f, 0.0f, 0.0f);
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
    dmaSprite(0x80, (u32)&_checkerboardBackgroundTextureSegmentRomStart, (u32)&_checkerboardBackgroundTextureSegmentRomEnd, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomStart, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomEnd, NULL, NULL, (void*)0x802DE5C0, NULL, (void*)0x802E0BC0, (void*)0x802E0CC0, (void*)0x802E0DC0, NULL, NULL, 0);
    setSpriteViewSpacePosition(0x80, 0.0f, 0.0f, 0.0f);
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
    dmaSprite(0x80, (u32)&_checkerboardBackgroundTextureSegmentRomStart, (u32)&_checkerboardBackgroundTextureSegmentRomEnd, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomStart, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomEnd, NULL, NULL, (void*)0x802DE5C0, NULL, (void*)0x802E0BC0, (void*)0x802E0CC0, (void*)0x802E0DC0, NULL, NULL, 0);
    setSpriteViewSpacePosition(0x80, 0.0f, 0.0f, 0.0f);
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
    dmaSprite(0x80, (u32)&_checkerboardBackgroundTextureSegmentRomStart, (u32)&_checkerboardBackgroundTextureSegmentRomEnd, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomStart, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomEnd, NULL, NULL, (void*)0x802DE5C0, NULL, (void*)0x802E0BC0, (void*)0x802E0CC0, (void*)0x802E0DC0, NULL, NULL, 0);
    setSpriteViewSpacePosition(0x80, 0.0f, 0.0f, 0.0f);
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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B86B8);

void func_800B86B8(void) {
    
    func_800593EC();
    
    dmaSprite(0x80, (u32)&_checkerboardBackgroundTextureSegmentRomStart, (u32)&_checkerboardBackgroundTextureSegmentRomEnd, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomStart, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x802DE5C0, NULL, (u16*)0x802E0BC0, (u16*)0x802E0CC0, (u8*)0x802E0DC0, NULL, 0, 0);
    
    setSpriteViewSpacePosition(0x80, 0.0f, 0.0f, 0.0f);
    setSpriteScale(0x80, 2.0f, 2.0f, 1.0f);
    setBilinearFiltering(0x80, 1);
    setSpriteColor(0x80, 0xFF, 0xFF, 0xFF, 0xFF);
    startSpriteAnimation(0x80, 0, 0);
    
    overlayScreenTable.cellIndex = 0;
    overlayScreenTable.previousCellIndex = 0;
    overlayScreenTable.pageNumber = 0;
    overlayScreenTable.unk_3 = 0xFF;
    overlayScreenTable.subscreen = 0;
    overlayScreenTable.unk_8 = 0;
    overlayScreenTable.unk_9 = 0;
    overlayScreenTable.unk_A = 0;
    overlayScreenTable.unk_B = 0;
    
    setMainLoopCallbackFunctionIndex(0x2C);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B881C);

void func_800B881C(void) {
    func_800593EC();
    func_800B8844();
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B8844);

void func_800B8844(void) {
    
    dmaSprite(0x80, (u32)&_checkerboardBackgroundTextureSegmentRomStart, (u32)&_checkerboardBackgroundTextureSegmentRomEnd, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomStart, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x802DE5C0, NULL, (u16*)0x802E0BC0, (u16*)0x802E0CC0, (u8*)0x802E0DC0, NULL, 0, 0);
    setSpriteViewSpacePosition(0x80, 0.0f, 0.0f, 0.0f);
    setSpriteScale(0x80, 2.0f, 2.0f, 1.0f);
    setBilinearFiltering(0x80, 1);
    setSpriteColor(0x80, 0xFF, 0xFF, 0xFF, 0xFF);
    startSpriteAnimation(0x80, 0, 0);
    
    overlayScreenTable.cellIndex = 0;
    overlayScreenTable.previousCellIndex = 0;
    overlayScreenTable.pageNumber = 0;
    overlayScreenTable.unk_3 = 0xFF;
    overlayScreenTable.subscreen = 0;
    
    setMainLoopCallbackFunctionIndex(0x23);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B8980);

void func_800B8980(void) {

    u32 temp;

    func_8005CDCC();
    func_800B2F34();

    temp = gGold;

    setMoneySprites();
    
    func_80045E20(1, 0x91, (u32)&_moneyTextureSegmentRomStart, (u32)&_moneyTextureSegmentRomEnd, (u32)&_moneyAssetsIndexSegmentRomStart, (u32)&_moneyAssetsIndexSegmentRomEnd, (void*)0x80253B00, (void*)0x80254A00, (void*)0x80254E00, (void*)0x80254F00, 0, 1, 0, 0.0f, 0.0f, 320.0f, 0xA);
    dmaOverlayScreenSprites(1, temp, 5, 3);
    setOverlayScreenSpritesRGBA(1, 0xFF, 0xFF, 0xFF, 0xFF);

    dmaSprite(0x83, (u32)&_inventoryScreenTextureSegmentRomStart, (u32)&_inventoryScreenTextureSegmentRomEnd, (u32)&_inventoryScreenAssetsIndexSegmentRomStart, (u32)&_inventoryScreenAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
    setSpriteViewSpacePosition(0x83, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x83, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x83, 2);
    setBilinearFiltering(0x83, 1);
    startSpriteAnimation(0x83, 0, 0);

    dmaSprite(0x84, (u32)&_inventoryScreenTextureSegmentRomStart, (u32)&_inventoryScreenTextureSegmentRomEnd, (u32)&_inventoryScreenAssetsIndexSegmentRomStart, (u32)&_inventoryScreenAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
    setSpriteViewSpacePosition(0x84, 0.0f, -80.0f, 8.0f);
    setSpriteColor(0x84, 0x80, 0x80, 0x80, 0);
    setSpriteRenderingLayer(0x84, 2);
    setBilinearFiltering(0x84, 1);
    startSpriteAnimation(0x84, 0, 0);
    
    dmaSprite(0x85, (u32)&_inventoryScreenTextureSegmentRomStart, (u32)&_inventoryScreenTextureSegmentRomEnd, (u32)&_inventoryScreenAssetsIndexSegmentRomStart, (u32)&_inventoryScreenAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
    setSpriteViewSpacePosition(0x85, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x85, 0x80, 0x80, 0x80, 0);
    setSpriteRenderingLayer(0x85, 2);
    setBilinearFiltering(0x85, 1);
    startSpriteAnimation(0x85, 1, 0);
    
    dmaSprite(0x86, (u32)&_inventoryScreenTextureSegmentRomStart, (u32)&_inventoryScreenTextureSegmentRomEnd, (u32)&_inventoryScreenAssetsIndexSegmentRomStart, (u32)&_inventoryScreenAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
    setSpriteViewSpacePosition(0x86, 0.0f, 0.0f, 16.0f);
    setSpriteColor(0x86, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x86, 2);
    setBilinearFiltering(0x86, 1);
    startSpriteAnimation(0x86, 5, 0);
    
    dmaSprite(0x87, (u32)&_inventoryScreenTextureSegmentRomStart, (u32)&_inventoryScreenTextureSegmentRomEnd, (u32)&_inventoryScreenAssetsIndexSegmentRomStart, (u32)&_inventoryScreenAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
    setSpriteViewSpacePosition(0x87, -28.0f, 80.0f, 8.0f);
    setSpriteColor(0x87, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x87, 2);
    setBilinearFiltering(0x87, 1);
    startSpriteAnimation(0x87, 2, 0);
    setSpritePaletteIndex(0x87, 3);
    
    dmaSprite(0x88, (u32)&_inventoryScreenTextureSegmentRomStart, (u32)&_inventoryScreenTextureSegmentRomEnd, (u32)&_inventoryScreenAssetsIndexSegmentRomStart, (u32)&_inventoryScreenAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
    setSpriteViewSpacePosition(0x88, 20.0f, 80.0f, 8.0f);
    setSpriteColor(0x88, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x88, 2);
    setBilinearFiltering(0x88, 1);
    startSpriteAnimation(0x88, 2, 0);
    setSpritePaletteIndex(0x88, 2);
    
    dmaSprite(0x79, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8023B400, NULL, (u16*)0x8023CC00, (u16*)0x8023CE00, (u8*)0x8023D200, NULL, 0, 0);
    setBilinearFiltering(0x79, 1);
    setSpriteColor(0x79, 0xFF, 0xFF, 0xFF, 0xFF);
    
    dmaSprite(0x82, (u32)&_inventoryScreenTextureSegmentRomStart, (u32)&_inventoryScreenTextureSegmentRomEnd, (u32)&_inventoryScreenAssetsIndexSegmentRomStart, (u32)&_inventoryScreenAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
    setSpriteViewSpacePosition(0x82, 0.0f, 0.0f, 24.0f);
    setBilinearFiltering(0x82, 1);
    setSpriteColor(0x82, 0xFF, 0xFF, 0xFF, 0xFF);
    
    func_800C7850();
    func_800B90E0();
    func_800B99EC();
    
}

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B90E0);

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B9914);

void func_800B9914(void) {
    
    u32 temp;
    u32 temp2;
    u8 *ptr = &overlayScreenTable.unk_5;

    temp2 = *ptr;
    if (*ptr == 7) {

        temp = overlayScreenTable.unk_3;
        if (overlayScreenTable.unk_3 != 0xFF) {
            
            if (overlayScreenTable.unk_4 == overlayScreenTable.pageNumber) {
                setSpriteViewSpacePosition(0x82, func_800BA928(temp2, temp), func_800BA9E8(*ptr, overlayScreenTable.unk_3), 16.0f);
                startSpriteAnimation(0x82, 6, 0xFE);
                return;
            } else {
                goto reset;
            }
            
        } 
        
        goto reset;
        
    }

    temp = overlayScreenTable.unk_3;
    
    if (overlayScreenTable.unk_3 != 0xFF) {
startAnim:
        setSpriteViewSpacePosition(0x82, func_800BA928(temp2, temp), func_800BA9E8(*ptr, overlayScreenTable.unk_3), 16.0f);
        startSpriteAnimation(0x82, 6, 0xFE);
    } else {
reset:
        resetAnimationState(0x82);
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B99EC);

void func_800B99EC(void) {

    u8 i;
    
    updateSpriteAlpha(0x89, 0xFF, 0x18);
    updateSpriteAlpha(0x8C, 0xFF, 0x18);
    updateSpriteAlpha(0x8D, 0xFF, 0x18);
    updateSpriteAlpha(0x8B, 0xFF, 0x18);
    updateSpriteAlpha(0x8A, 0xFF, 0x18);
    updateSpriteAlpha(0x8E, 0xFF, 0x18);
    updateSpriteAlpha(0x83, 0xFF, 0x18);
    updateSpriteAlpha(0x84, 0xFF, 0x18);
    updateSpriteAlpha(0x85, 0xFF, 0x18);
    updateSpriteAlpha(0x86, 0xFF, 0x18);
    updateSpriteAlpha(0x87, 0xFF, 0x18);
    updateSpriteAlpha(0x88, 0xFF, 0x18);

    for (i = 0; i < 9; i++) {
        if (gPlayer.toolSlots[i]) {
            updateSpriteAlpha(i + 0xA3, 0xFF, 0x18);
        }
    }

    for (i = 0; i < 9; i++) {
        if (gPlayer.belongingsSlots[i]) {
            updateSpriteAlpha(i + 0xAC, 0xFF, 0x18);
        }
    }

    for (i = 0; i < 9; i++) {
        if (gPlayer.keyItemSlots[i]) {
            updateSpriteAlpha(i + 0xB5, 0xFF, 0x18);
        }
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B9B8C);

void func_800B9B8C(void) {

    u8 i;
    
    updateSpriteAlpha(0x89, 0, 0x18);
    updateSpriteAlpha(0x8C, 0, 0x18);
    updateSpriteAlpha(0x8D, 0, 0x18);
    updateSpriteAlpha(0x8B, 0, 0x18);
    updateSpriteAlpha(0x8A, 0, 0x18);
    updateSpriteAlpha(0x8E, 0, 0x18);
    updateSpriteAlpha(0x83, 0, 0x18);
    updateSpriteAlpha(0x84, 0, 0x18);
    updateSpriteAlpha(0x85, 0, 0x18);
    updateSpriteAlpha(0x86, 0, 0x18);
    updateSpriteAlpha(0x87, 0, 0x18);
    updateSpriteAlpha(0x88, 0, 0x18);
    
    for (i = 0; i < 9; i++) {
        if (gPlayer.toolSlots[i]) {
            updateSpriteAlpha(i + 0xA3, 0, 0x18);
        }
    }

    for (i = 0; i < 9; i++) {
        if (gPlayer.belongingsSlots[i]) {
            updateSpriteAlpha(i + 0xAC, 0, 0x18);
        }
    }

    for (i = 0; i < 9; i++) {
        if (gPlayer.keyItemSlots[i]) {
            updateSpriteAlpha(i + 0xB5, 0, 0x18);
        }
    }
    
    deactivateOverlayScreenSprites(0);
    deactivateOverlayScreenSprites(1);
    
}

// pause screen main loop callback
INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B9D3C);

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BA928);

f32 func_800BA928(u8 arg0, u32 arg1) {

    f32 res;
    
    switch (arg0) {

        case 5:

            arg1 &= 0xFF;

            if (arg1 < 8) {
                res = D_80116C80[arg1 & 0x3];
            } else {
                res = D_80116C90;
            }
            break;
        
        case 6:

            arg1 &= 0xFF;

            if (arg1 < 8) {
                res = D_80116CA0[arg1 & 0x3];
            } else {
                res = D_80116CB0;
            }
            
            break;

        case 7:
            res = D_80116CC0[arg1 & 1];
            break;
        
    }

    return res;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BA9E8);

f32 func_800BA9E8(u8 arg0, u32 arg1) {

    f32 res;
    
    switch (arg0) {

        case 5:
            
            arg1 &= 0xFF;
            
            if (arg1 < 8) {
                res = D_80116C94[(arg1 & 0xFC) / 4];
            } else {
                res = D_80116C9C;
            }
            break;
        
        case 6:

            arg1 &= 0xFF;
            
            if (arg1 < 8) {
                res = D_80116CB4[(arg1 & 0xFC) / 4];
            } else {
                res = D_80116CBC;
            }
            
            break;

        case 7:
            res = D_80116CC8[(arg1 /2) & 0x7F];
            break;
        
    }

    return res;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BAAA0);

void func_800BAAA0(void) {

    u8 i;
    
    setSpriteColor(0x83, 0x80, 0x80, 0x80, 0xFF);
    setSpriteColor(0x84, 0x80, 0x80, 0x80, 0xFF);
    setSpriteColor(0x85, 0x80, 0x80, 0x80, 0xFF);

    switch (overlayScreenTable.subscreen) {

        case 5:
            setSpriteColor(0x83, 0xFF, 0xFF, 0xFF, 0xFF);
            break;
        case 6:
            setSpriteColor(0x84, 0xFF, 0xFF, 0xFF, 0xFF);
            break;
        case 7:
            setSpriteColor(0x85, 0xFF, 0xFF, 0xFF, 0xFF);
            break;
    }

    for (i = 0; i < 9; i++) {
         if (gPlayer.toolSlots[i]) {
             setSpriteColor(i + 0xA3, 0xFF, 0xFF, 0xFF, 0xFF);
         }
    }

    for (i = 0; i < 9; i++) {
         if (gPlayer.belongingsSlots[i]) {
             setSpriteColor(i + 0xAC, 0xFF, 0xFF, 0xFF, 0xFF);
         }
    }

    for (i = 0; i < 9; i++) {
         if (gPlayer.keyItemSlots[(overlayScreenTable.pageNumber * 8) + i]) {
             setSpriteColor(i + 0xB5, 0xFF, 0xFF, 0xFF, 0xFF);
         }
    }
    
}

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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BADD0);

void func_800BADD0(void) {
    
    func_800593EC();
    
    dmaSprite(0x80, 
        (u32)&_checkerboardBackgroundTextureSegmentRomStart,
        (u32)&_checkerboardBackgroundTextureSegmentRomEnd, 
        (u32)&_checkerboardBackgroundAssetsIndexSegmentRomStart, 
        (u32)&_checkerboardBackgroundAssetsIndexSegmentRomEnd, 
        NULL, 
        NULL, 
        (u8*)0x802DE5C0, 
        NULL, 
        (u16*)0x802E0BC0,
        (u16*)0x802E0CC0, 
        (u8*)0x802E0DC0, 
        NULL, 
        0, 
        0
    );
    
    setSpriteViewSpacePosition(0x80, 0.0f, 0.0f, 0.0f);
    setSpriteScale(0x80, 2.0f, 2.0f, 1.0f);
    setBilinearFiltering(0x80, 1);
    setSpriteColor(0x80, 0xFF, 0xFF, 0xFF, 0xFF);
    startSpriteAnimation(0x80, 0, 0);
    
    overlayScreenTable.cellIndex = 0;
    overlayScreenTable.previousCellIndex = 0;
    overlayScreenTable.pageNumber = 0;
    overlayScreenTable.unk_3 = 0xFF;
    overlayScreenTable.unk_4 = 0;
    overlayScreenTable.subscreen = 0;
        
    setMainLoopCallbackFunctionIndex(0x24);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BAF1C);

void func_800BAF1C(void) {

    u32 temp;
    
    func_800B2F34();
    
    temp = gGold;

    setMoneySprites();
    
    func_80045E20(1, 0x91, (u32)&_moneyTextureSegmentRomStart, (u32)&_moneyTextureSegmentRomEnd, (u32)&_moneyAssetsIndexSegmentRomStart, (u32)&_moneyAssetsIndexSegmentRomEnd, (void*)0x80253B00, (void*)0x80254A00, (void*)0x80254E00, (void*)0x80254F00, 0, 1, 0, 0.0f, 0.0f, 320.0f, 0xA);
    dmaOverlayScreenSprites(1, temp, 5, 3);
    setOverlayScreenSpritesRGBA(1, 0xFF, 0xFF, 0xFF, 0xFF);
    
    dmaSprite(0x83, (u32)&_inventoryScreenTextureSegmentRomStart, (u32)&_inventoryScreenTextureSegmentRomEnd, (u32)&_inventoryScreenAssetsIndexSegmentRomStart, (u32)&_inventoryScreenAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
    setSpriteViewSpacePosition(0x83, 42.0f, 0.0f, 8.0f);
    setSpriteColor(0x83, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x83, 2);
    setBilinearFiltering(0x83, 1);
    startSpriteAnimation(0x83, 0, 0);
    
    dmaSprite(0x84, (u32)&_inventoryScreenTextureSegmentRomStart, (u32)&_inventoryScreenTextureSegmentRomEnd, (u32)&_inventoryScreenAssetsIndexSegmentRomStart, (u32)&_inventoryScreenAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
    setSpriteViewSpacePosition(0x84, 42.0f, -80.0f, 8.0f);
    setSpriteColor(0x84, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x84, 2);
    setBilinearFiltering(0x84, 1);
    startSpriteAnimation(0x84, 0, 0);
    
    dmaSprite(0x86, (u32)&_inventoryScreenTextureSegmentRomStart, (u32)&_inventoryScreenTextureSegmentRomEnd, (u32)&_inventoryScreenAssetsIndexSegmentRomStart, (u32)&_inventoryScreenAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
    setSpriteViewSpacePosition(0x86, 0.0f, 0.0f, 16.0f);
    setSpriteColor(0x86, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x86, 2);
    setBilinearFiltering(0x86, 1);
    startSpriteAnimation(0x86, 5, 1);
    
    dmaSprite(0x87, (u32)&_inventoryScreenTextureSegmentRomStart, (u32)&_inventoryScreenTextureSegmentRomEnd, (u32)&_inventoryScreenAssetsIndexSegmentRomStart, (u32)&_inventoryScreenAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
    setSpriteViewSpacePosition(0x87, 0.0f, 80.0f, 8.0f);
    setSpriteColor(0x87, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x87, 2);
    setBilinearFiltering(0x87, 1);
    startSpriteAnimation(0x87, 2, 0);
    setSpritePaletteIndex(0x87, 3);
    
    dmaSprite(0x79, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8023B400, NULL, (u16*)0x8023CC00, (u16*)0x8023CE00, (u8*)0x8023D200, NULL, 0, 0);
    setSpriteViewSpacePosition(0x79, func_800BC700(overlayScreenTable.subscreen, overlayScreenTable.cellIndex) - 16.0f, func_800BC770(overlayScreenTable.subscreen, overlayScreenTable.cellIndex) - 8.0f, 32.0f);
    setBilinearFiltering(0x79, 1);
    setSpriteColor(0x79, 0xFF, 0xFF, 0xFF, 0xFF);
    
    dmaSprite(0x82, (u32)&_inventoryScreenTextureSegmentRomStart, (u32)&_inventoryScreenTextureSegmentRomEnd, (u32)&_inventoryScreenAssetsIndexSegmentRomStart, (u32)&_inventoryScreenAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
    setSpriteViewSpacePosition(0x82, 0.0f, 0.0f, 24.0f);
    setBilinearFiltering(0x82, 1);
    setSpriteColor(0x82, 0xFF, 0xFF, 0xFF, 0xFF);
    
    func_800C7850();
    func_800BB590();
    func_800BBBC8();
    
}

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BB590);

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BBAF0);

void func_800BBAF0(void) {

    f32 x;
    
    if (overlayScreenTable.unk_5 == 5) {

        if (overlayScreenTable.unk_3 != 0xFF) {
            
            if (overlayScreenTable.unk_4 == overlayScreenTable.pageNumber) {
                
                x = func_800BC700(overlayScreenTable.unk_5, overlayScreenTable.unk_3);
                setSpriteViewSpacePosition(0x82, x, func_800BC770(overlayScreenTable.unk_5, overlayScreenTable.unk_3), 16.0f);
                startSpriteAnimation(0x82, 6, 0xFE);
            
            } else {
                resetAnimationState(0x82);
            }
            
        } else {
            resetAnimationState(0x82);
        }
        
    } else {

        if (overlayScreenTable.unk_3 != 0xFF) {
            
            x = func_800BC700(overlayScreenTable.unk_5, overlayScreenTable.unk_3);
            setSpriteViewSpacePosition(0x82, x, func_800BC770(overlayScreenTable.unk_5, overlayScreenTable.unk_3), 16.0f);
            startSpriteAnimation(0x82, 6, 0xFE);
            
        } else {
            resetAnimationState(0x82);
        }
        
    } 
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BBBC8);

void func_800BBBC8(void) {
    
    u8 i;

    updateSpriteAlpha(0x89, 0xFF, 0x18);
    updateSpriteAlpha(0x8C, 0xFF, 0x18);
    updateSpriteAlpha(0x8D, 0xFF, 0x18);
    updateSpriteAlpha(0x8B, 0xFF, 0x18);
    updateSpriteAlpha(0x8A, 0xFF, 0x18);
    updateSpriteAlpha(0x8E, 0xFF, 0x18);
    updateSpriteAlpha(0x83, 0xFF, 0x18);
    updateSpriteAlpha(0x84, 0xFF, 0x18);
    updateSpriteAlpha(0x86, 0xFF, 0x18);
    updateSpriteAlpha(0x87, 0xFF, 0x18);
    updateSpriteAlpha(0x88, 0xFF, 0x18);

    for (i = 0; i < 9; i++) {
        if (gToolchestSlots[(overlayScreenTable.pageNumber * 8) + i]) {
            updateSpriteAlpha(0xA3 + i, 0xFF, 0x18);
        }
    }

    for (i = 0; i < 9; i++) {
        if (gPlayer.toolSlots[i]) {
            updateSpriteAlpha(0xAC + i, 0xFF, 0x18);
        }
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BBD34);

void func_800BBD34(void) {

    u8 i;

    updateSpriteAlpha(0x89, 0, 0x18);
    updateSpriteAlpha(0x8C, 0, 0x18);
    updateSpriteAlpha(0x8D, 0, 0x18);
    updateSpriteAlpha(0x8B, 0, 0x18);
    updateSpriteAlpha(0x8A, 0, 0x18);
    updateSpriteAlpha(0x8E, 0, 0x18);
    updateSpriteAlpha(0x83, 0, 0x18);
    updateSpriteAlpha(0x84, 0, 0x18);
    updateSpriteAlpha(0x85, 0, 0x18);
    updateSpriteAlpha(0x86, 0, 0x18);
    updateSpriteAlpha(0x87, 0, 0x18);
    updateSpriteAlpha(0x88, 0, 0x18);

    for (i = 0; i < 9; i++) {
        if (gToolchestSlots[(overlayScreenTable.pageNumber * 8) + i]) {
            updateSpriteAlpha(0xA3 + i, 0, 0x18);
        }
    }

    for (i = 0; i < 9; i++) {
        if (gPlayer.toolSlots[i]) {
            updateSpriteAlpha(0xAC + i, 0, 0x18);
        }
    }

    deactivateOverlayScreenSprites(0);
    deactivateOverlayScreenSprites(1);
    
}

// toolbox callback
INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BBEC0);

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BC700);

f32 func_800BC700(u8 arg0, u32 arg1) {

    f32 res;
    
    switch (arg0) {

        case 5:
            res = D_80116CD8[arg1 & 0x3];
            break;
        
        case 6:

            arg1 &= 0xFF;
            if (arg1 < 8) {
                res = D_80116CF0[arg1 & 0x3];
            } else {
                res = D_80116D00;
            }
            
            break;
        
    }

    return res;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BC770);

f32 func_800BC770(u8 arg0, u32 arg1) {

    f32 res;
    
    switch (arg0) {

        case 5:
            res = D_80116CE8[(arg1 & 0xFC) / 4];
            break;
        
        case 6:

            arg1 &= 0xFF;
            if (arg1 < 8) {
                res = D_80116D04[(arg1 & 0xFC) / 4];
            } else {
                res = D_80116D0C;
            }
            
            break;
        
    }

    return res;
    
}

// rucksack
INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BC7D8);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BC918);

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BCA9C);

void func_800BCA9C(void) {
    
    func_800593EC();
    
    dmaSprite(0x80, (u32)&_checkerboardBackgroundTextureSegmentRomStart, (u32)&_checkerboardBackgroundTextureSegmentRomEnd, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomStart, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x802DE5C0, NULL, (u16*)0x802E0BC0, (u16*)0x802E0CC0, (u8*)0x802E0DC0, NULL, 0, 0);
    setSpriteViewSpacePosition(0x80, 0.0f, 0.0f, 0.0f);
    setSpriteScale(0x80, 2.0f, 2.0f, 1.0f);
    setBilinearFiltering(0x80, TRUE);
    setSpriteColor(0x80, 0xFF, 0xFF, 0xFF, 0xFF);
    startSpriteAnimation(0x80, 0, 0);
    
    overlayScreenTable.cellIndex = 0;
    overlayScreenTable.previousCellIndex = 0;
    overlayScreenTable.pageNumber = 0;
    overlayScreenTable.unk_3 = 0xFF;
    overlayScreenTable.unk_4 = 0;
    overlayScreenTable.subscreen = 0;
    
    setMainLoopCallbackFunctionIndex(0x25);
    
}


INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BCBE8);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BD25C);

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BD754);

void func_800BD754(void) {

    u32 temp;
    u32 temp2;
    u8 *ptr = &overlayScreenTable.unk_5;

    temp2 = *ptr;
    if (*ptr == 5) {

        temp = overlayScreenTable.unk_3;
        if (overlayScreenTable.unk_3 != 0xFF) {
            
            if (overlayScreenTable.unk_4 == overlayScreenTable.pageNumber) {
                setSpriteViewSpacePosition(0x82, func_800BE3B8(temp2, temp), func_800BE428(*ptr, overlayScreenTable.unk_3), 16.0f);
                startSpriteAnimation(0x82, 6, 0xFE);
                return;
            } else {
                goto reset;
            }
            
        } 
        
        goto reset;
        
    }

    temp = overlayScreenTable.unk_3;
    
    if (overlayScreenTable.unk_3 != 0xFF) {
startAnim:
        setSpriteViewSpacePosition(0x82, func_800BE3B8(temp2, temp), func_800BE428(*ptr, overlayScreenTable.unk_3), 16.0f);
        startSpriteAnimation(0x82, 6, 0xFE);
    } else {
reset:
        resetAnimationState(0x82);
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BD82C);

void func_800BD82C(void) {
    
    u8 i;
    
    updateSpriteAlpha(0x89U, 0xFFU, 0x18);
    updateSpriteAlpha(0x8CU, 0xFFU, 0x18);
    updateSpriteAlpha(0x8DU, 0xFFU, 0x18);
    updateSpriteAlpha(0x8BU, 0xFFU, 0x18);
    updateSpriteAlpha(0x8AU, 0xFFU, 0x18);
    updateSpriteAlpha(0x8EU, 0xFFU, 0x18);
    updateSpriteAlpha(0x83U, 0xFFU, 0x18);
    updateSpriteAlpha(0x84U, 0xFFU, 0x18);
    updateSpriteAlpha(0x86U, 0xFFU, 0x18);
    updateSpriteAlpha(0x87U, 0xFFU, 0x18);
    updateSpriteAlpha(0x88U, 0xFFU, 0x18);
    
    for (i = 0; i < 9; i++) {
        if (D_80237420[(overlayScreenTable.pageNumber * 8) + i]) {
            updateSpriteAlpha(i + 0xA3, 0xFF, 0x18);
        }
    }

    for (i = 0; i < 9; i++) {
        if (gPlayer.belongingsSlots[i]) {
            updateSpriteAlpha(i + 0xAC, 0xFF, 0x18);
        }
    }
    
}


//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BD998);

void func_800BD998(void) {

    u8 i;

    updateSpriteAlpha(0x89, 0, 0x18);
    updateSpriteAlpha(0x8C, 0, 0x18);
    updateSpriteAlpha(0x8D, 0, 0x18);
    updateSpriteAlpha(0x8B, 0, 0x18);
    updateSpriteAlpha(0x8A, 0, 0x18);
    updateSpriteAlpha(0x8E, 0, 0x18);
    updateSpriteAlpha(0x83, 0, 0x18);
    updateSpriteAlpha(0x84, 0, 0x18);
    updateSpriteAlpha(0x85, 0, 0x18);
    updateSpriteAlpha(0x86, 0, 0x18);
    updateSpriteAlpha(0x87, 0, 0x18);
    updateSpriteAlpha(0x88, 0, 0x18);

    for (i = 0; i < 9; i++) {
        if (D_80237420[(overlayScreenTable.pageNumber * 8) + i]) {
            updateSpriteAlpha(i + 0xA3, 0, 0x18);
        }
    }

    for (i = 0; i < 9; i++) {
        if (gPlayer.belongingsSlots[i]) {
            updateSpriteAlpha(i + 0xAC, 0, 0x18);
        }
    }

    deactivateOverlayScreenSprites(0);
    deactivateOverlayScreenSprites(1);
    
}

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BDB24);

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BE3B8);

inline f32 func_800BE3B8(u8 arg0, u32 arg1) {

    f32 res;
    
    switch (arg0) {
        
        case 5:
            arg1 &= 0xFF;
            res = D_80116D10[arg1 & 3];
            break;
        
        case 6:
            arg1 &= 0xFF;
            if (arg1 < 8) {
                res = D_80116D28[arg1 & 3];
            } else {
                res = D_80116D38;
            }
            
            break;
        
    }

    return res;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BE428);

f32 func_800BE428(u8 arg0, u32 arg1) {

        f32 res;
    
    switch (arg0) {

        case 5:
            res = D_80116D20[(arg1 & 0xFC) / 4];
            break;
        
        case 6:

            arg1 &= 0xFF;
            if (arg1 < 8) {
                res = D_80116D3C[(arg1 & 0xFC) / 4];
            } else {
                res = D_80116D44;
            }
            
            break;
        
    }

    return res;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BE490);

void func_800BE490(void) {

    u8 i;

    setSpriteColor(0x83, 0x80, 0x80, 0x80, 0xFF);
    setSpriteColor(0x84, 0x80, 0x80, 0x80, 0xFF);
    setSpriteColor(0x83, 0xFF, 0xFF, 0xFF, 0xFF);
    setSpriteColor(0x84, 0xFF, 0xFF, 0xFF, 0xFF);

    for (i = 0; i < 9; i++) {

        if (D_80237420[(overlayScreenTable.pageNumber * 8) + i]) {
            setSpriteColor(i + 0xA3, 0xFF, 0xFF, 0xFF, 0xFF);
        }
        
    }

    for (i = 0; i < 9; i++) {
        
        if (gPlayer.belongingsSlots[i] != 0) {
            setSpriteColor(i + 0xAC, 0xFF, 0xFF, 0xFF, 0xFF);
        }
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BE5D0);

bool func_800BE5D0(u8 arg0) {
    
    bool result = FALSE;
    u8 temp;
    
    switch (overlayScreenTable.subscreen) {

        case 5:
            temp = D_80237420[(overlayScreenTable.pageNumber * 8) + overlayScreenTable.cellIndex];
            break;
        case 6:
            temp = gPlayer.belongingsSlots[overlayScreenTable.cellIndex];
            break;
        
    }

    if (getItemFlags(temp) & 0x200) {
        result = TRUE;
    }

    if (temp == 0) {
        result = TRUE;
    }

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BE684);

void func_800BE684(void) {

    u8 temp;

    switch (overlayScreenTable.subscreen) {

        case 5:

            temp = D_80237420[(overlayScreenTable.pageNumber * 8) + overlayScreenTable.cellIndex];
            
            if (overlayScreenTable.unk_5 == overlayScreenTable.subscreen) {
                D_80237420[(overlayScreenTable.pageNumber * 8) + overlayScreenTable.cellIndex] = D_80237420[(overlayScreenTable.unk_4 * 8) + overlayScreenTable.unk_3];
                D_80237420[(overlayScreenTable.unk_4 * 8) + overlayScreenTable.unk_3] = temp;
            } else {
                D_80237420[(overlayScreenTable.pageNumber * 8) + overlayScreenTable.cellIndex] = gPlayer.belongingsSlots[overlayScreenTable.unk_3];
                gPlayer.belongingsSlots[overlayScreenTable.unk_3] = temp;
            }

            break;

        case 6:

            temp = gPlayer.belongingsSlots[overlayScreenTable.cellIndex];
            
            if (overlayScreenTable.unk_5 == overlayScreenTable.subscreen) {
                gPlayer.belongingsSlots[overlayScreenTable.cellIndex] = gPlayer.belongingsSlots[overlayScreenTable.unk_3];
                gPlayer.belongingsSlots[overlayScreenTable.unk_3] = temp;
            } else {
                gPlayer.belongingsSlots[overlayScreenTable.cellIndex] = D_80237420[(overlayScreenTable.unk_4 * 8) + overlayScreenTable.unk_3];
                D_80237420[(overlayScreenTable.unk_4 * 8) + overlayScreenTable.unk_3] = temp;
            }

        break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BE808);

void func_800BE808(void) {
    
    func_800593EC();
    
    dmaSprite(0x80, (u32)&_checkerboardBackgroundTextureSegmentRomStart, (u32)&_checkerboardBackgroundTextureSegmentRomEnd, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomStart, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x802DE5C0, NULL, (u16*)0x802E0BC0, (u16*)0x802E0CC0, (u8*)0x802E0DC0, NULL, 0, 0);
    setSpriteViewSpacePosition(0x80, 0.0f, 0.0f, 0.0f);
    setSpriteScale(0x80, 2.0f, 2.0f, 1.0f);
    setBilinearFiltering(0x80, 1);
    setSpriteColor(0x80, 0xFF, 0xFF, 0xFF, 0xFF);
    startSpriteAnimation(0x80, 0, 0);
    
    overlayScreenTable.cellIndex = 0;
    overlayScreenTable.previousCellIndex = 0;
    overlayScreenTable.pageNumber = 0;
    overlayScreenTable.unk_3 = 0xFF;
    overlayScreenTable.unk_4 = 0;
    overlayScreenTable.subscreen = 0;
    
    setMainLoopCallbackFunctionIndex(0x26);
    
}


INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BE954);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BF0C8);

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BF5C0);

void func_800BF5C0(void) {

    u32 temp;
    u32 temp2;
    u8 *ptr = &overlayScreenTable.unk_5;

    temp2 = *ptr;
    if (*ptr == 5) {

        temp = overlayScreenTable.unk_3;
        if (overlayScreenTable.unk_3 != 0xFF) {
            
            if (overlayScreenTable.unk_4 == overlayScreenTable.pageNumber) {
                setSpriteViewSpacePosition(0x82, func_800C0238(temp2, temp), func_800C02A8(*ptr, overlayScreenTable.unk_3), 16.0f);
                startSpriteAnimation(0x82, 6, 0xFE);
                return;
            } else {
                goto reset;
            }
            
        } 
        
        goto reset;
        
    }

    temp = overlayScreenTable.unk_3;
    
    if (overlayScreenTable.unk_3 != 0xFF) {
startAnim:
        setSpriteViewSpacePosition(0x82, func_800C0238(temp2, temp), func_800C02A8(*ptr, overlayScreenTable.unk_3), 16.0f);
        startSpriteAnimation(0x82, 6, 0xFE);
    } else {
reset:
        resetAnimationState(0x82);
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BF698);

void func_800BF698(void) {
    
    u8 i;

    updateSpriteAlpha(0x89, 0xFF, 0x18);
    updateSpriteAlpha(0x8C, 0xFF, 0x18);
    updateSpriteAlpha(0x8D, 0xFF, 0x18);
    updateSpriteAlpha(0x8B, 0xFF, 0x18);
    updateSpriteAlpha(0x8A, 0xFF, 0x18);
    updateSpriteAlpha(0x8E, 0xFF, 0x18);
    updateSpriteAlpha(0x83, 0xFF, 0x18);
    updateSpriteAlpha(0x84, 0xFF, 0x18);
    updateSpriteAlpha(0x86, 0xFF, 0x18);
    updateSpriteAlpha(0x87, 0xFF, 0x18);
    updateSpriteAlpha(0x88, 0xFF, 0x18);

    for (i = 0; i < 9; i++) {
        if (D_801890E8[(overlayScreenTable.pageNumber * 8) + i]) {
            updateSpriteAlpha(i + 0xA3, 0xFF, 0x18);
        }
    }

    for (i = 0; i < 9; i++) {
        if (gPlayer.belongingsSlots[i]) {
            updateSpriteAlpha(i + 0xAC, 0xFF, 0x18);
        }
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BF804);

void func_800BF804(void) {

    u8 i;

    updateSpriteAlpha(0x89, 0, 0x18);
    updateSpriteAlpha(0x8C, 0, 0x18);
    updateSpriteAlpha(0x8D, 0, 0x18);
    updateSpriteAlpha(0x8B, 0, 0x18);
    updateSpriteAlpha(0x8A, 0, 0x18);
    updateSpriteAlpha(0x8E, 0, 0x18);
    updateSpriteAlpha(0x83, 0, 0x18);
    updateSpriteAlpha(0x84, 0, 0x18);
    updateSpriteAlpha(0x85, 0, 0x18);
    updateSpriteAlpha(0x86, 0, 0x18);
    updateSpriteAlpha(0x87, 0, 0x18);
    updateSpriteAlpha(0x88, 0, 0x18);

    for (i = 0; i < 9; i++) {

        if (D_801890E8[(overlayScreenTable.pageNumber * 8) + i]) {
            updateSpriteAlpha(i + 0xA3, 0, 0x18);
        }
        
    }
    
    for (i = 0; i < 9; i++) {
        
        if (gPlayer.belongingsSlots[i] != 0) {
            updateSpriteAlpha(i + 0xAC, 0, 0x18);
        }
        
    }

    deactivateOverlayScreenSprites(0);
    deactivateOverlayScreenSprites(1);
    
}

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BF990);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C0238);

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C02A8);

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C0310);

void func_800C0310(void) {

    u8 i;

    setSpriteColor(0x83, 0x80, 0x80, 0x80, 0xFF);
    setSpriteColor(0x84, 0x80, 0x80, 0x80, 0xFF);
    setSpriteColor(0x83, 0xFF, 0xFF, 0xFF, 0xFF);
    setSpriteColor(0x84, 0xFF, 0xFF, 0xFF, 0xFF);

    for (i = 0; i < 9; i++) {

        if (D_801890E8[(overlayScreenTable.pageNumber * 8) + i]) {
            setSpriteColor(i + 0xA3, 0xFF, 0xFF, 0xFF, 0xFF);
        }
        
    }

    for (i = 0; i < 9; i++) {
        
        if (gPlayer.belongingsSlots[i] != 0) {
            setSpriteColor(i + 0xAC, 0xFF, 0xFF, 0xFF, 0xFF);
        }
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C0450);

bool func_800C0450(u8 arg0) {

    bool result = FALSE;
    u8 temp;
    
    switch (overlayScreenTable.subscreen) {

        case 5:
            temp = D_801890E8[(overlayScreenTable.pageNumber * 8) + overlayScreenTable.cellIndex];
            break;
        case 0x65:
            temp = gPlayer.belongingsSlots[overlayScreenTable.cellIndex];
            break;
        
    }

    if (getItemFlags(temp) & 0x1000) {
        result = TRUE;
    }

    if (temp == 0) {
        result = TRUE;
    }

    return result;
    
}

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C0504);

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C0688);

u8 func_800C0688(u8 tool) {
    
    u8 level;

    level = 0;
    
    switch (tool) {
        case 29:
            level = gPlayer.bottleContents;
            break;
        case 1:
            level = gPlayer.toolLevels[0];
            break;
        case 2:
            level = gPlayer.toolLevels[1];
            break;
        case 3:
            level = gPlayer.toolLevels[2];
            break;
        case 4:
            level = gPlayer.toolLevels[3];
            break;
        case 5:
            level = gPlayer.toolLevels[4];
            break;
        }
    
    return level;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C0714);

void func_800C0714(void) {
    
    u32 temp;

    func_800B2F34();

    temp = gGold;
    
    setMoneySprites();
    
    func_80045E20(1, 0x91, (u32)&_moneyTextureSegmentRomStart, (u32)&_moneyTextureSegmentRomEnd, (u32)&_moneyAssetsIndexSegmentRomStart, (u32)&_moneyAssetsIndexSegmentRomEnd, (void*)0x80253B00, (void*)0x80254A00, (void*)0x80254E00, (void*)0x80254F00, 0, 1, 0, 0.0f, 0.0f, 320.0f, 0xA);
    dmaOverlayScreenSprites(1, temp, 5, 3);
    setOverlayScreenSpritesRGBA(1, 0xFF, 0xFF, 0xFF, 0xFF);

    dmaSprite(0x82, (u32)&_annexExtensionSelectionTextureSegmentRomStart, (u32)&_annexExtensionSelectionTextureSegmentRomEnd, (u32)&_annexExtensionSelectionAssetsIndexSegmentRomStart, (u32)&_annexExtensionSelectionAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
    setSpriteViewSpacePosition(0x82, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x82, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x82, 2);
    setBilinearFiltering(0x82, 1);
    startSpriteAnimation(0x82, 0, 0);

    dmaSprite(0x83, (u32)&_annexExtensionSelectionTextureSegmentRomStart, (u32)&_annexExtensionSelectionTextureSegmentRomEnd, (u32)&_annexExtensionSelectionAssetsIndexSegmentRomStart, (u32)&_annexExtensionSelectionAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
    setSpriteViewSpacePosition(0x83, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x83, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x83, 2);
    setBilinearFiltering(0x83, 1);
    startSpriteAnimation(0x83, 1, 0);
    
    dmaSprite(0x84, (u32)&_annexExtensionSelectionTextureSegmentRomStart, (u32)&_annexExtensionSelectionTextureSegmentRomEnd, (u32)&_annexExtensionSelectionAssetsIndexSegmentRomStart, (u32)&_annexExtensionSelectionAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
    setSpriteViewSpacePosition(0x84, 0.0f, -16.0f, 8.0f);
    setSpriteColor(0x84, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x84, 2);
    setBilinearFiltering(0x84, 1);
    startSpriteAnimation(0x84, 1, 0);
    
    dmaSprite(0x85, (u32)&_annexExtensionSelectionTextureSegmentRomStart, (u32)&_annexExtensionSelectionTextureSegmentRomEnd, (u32)&_annexExtensionSelectionAssetsIndexSegmentRomStart, (u32)&_annexExtensionSelectionAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
    setSpriteViewSpacePosition(0x85, 0.0f, -32.0f, 8.0f);
    setSpriteColor(0x85, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x85, 2);
    setBilinearFiltering(0x85, 1);
    startSpriteAnimation(0x85, 1, 0);
    
    dmaSprite(0x86, (u32)&_annexExtensionSelectionTextureSegmentRomStart, (u32)&_annexExtensionSelectionTextureSegmentRomEnd, (u32)&_annexExtensionSelectionAssetsIndexSegmentRomStart, (u32)&_annexExtensionSelectionAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
    setSpriteViewSpacePosition(0x86, 0.0f, -48.0f, 8.0f);
    setSpriteColor(0x86, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x86, 2);
    setBilinearFiltering(0x86, 1);
    startSpriteAnimation(0x86, 1, 0);
    
    dmaSprite(0x87, (u32)&_annexExtensionSelectionTextureSegmentRomStart, (u32)&_annexExtensionSelectionTextureSegmentRomEnd, (u32)&_annexExtensionSelectionAssetsIndexSegmentRomStart, (u32)&_annexExtensionSelectionAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
    setSpriteViewSpacePosition(0x87, 0.0f, -64.0f, 8.0f);
    setSpriteColor(0x87, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x87, 2);
    setBilinearFiltering(0x87, 1);
    startSpriteAnimation(0x87, 1, 0);
    
    dmaSprite(0x88, (u32)&_annexExtensionSelectionTextureSegmentRomStart, (u32)&_annexExtensionSelectionTextureSegmentRomEnd, (u32)&_annexExtensionSelectionAssetsIndexSegmentRomStart, (u32)&_annexExtensionSelectionAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
    setSpriteViewSpacePosition(0x88, 0.0f, -80.0f, 8.0f);
    setSpriteColor(0x88, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x88, 2);
    setBilinearFiltering(0x88, 1);
    startSpriteAnimation(0x88, 1, 0);
        
    dmaSprite(0x81, (u32)&_photoSmallTextureSegmentRomStart, (u32)&_photoSmallTextureSegmentRomEnd, (u32)&_photoSmallAssetsIndexSegmentRomStart, (u32)&_photoSmallAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x80289900, NULL, (u16*)0x8028EF00, (u16*)0x8028F000, (u8*)0x8028F100, NULL, 0, 0);
    setSpriteViewSpacePosition(0x81, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x81, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x81, 2);
    setBilinearFiltering(0x81, 1);
    startSpriteAnimation(0x81, 0, 0);

    dmaSprite(0x79, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8023B400, NULL, (u16*)0x8023CC00, (u16*)0x8023CE00, (u8*)0x8023D200, NULL, 0, 0);
    setBilinearFiltering(0x79, 1);
    setSpriteColor(0x79, 0xFF, 0xFF, 0xFF, 0xFF);
    func_800C0F24();
    
}

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
    deactivateOverlayScreenSprites(0);
    deactivateOverlayScreenSprites(1);
}

//INCLUDE_RODATA("asm/nonmatchings/game/overlayScreens", D_80121D74);

static const u16 D_80121D74[6] = { 0x10, 0x11, 0x12, 0x13, 0x14, 0x15};

// rodata: D_80121D74
// house extension selection callback
INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C1124);

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C1710);

void func_800C1710(void) {

    u32 temp;
    u8 temp2;
    
    func_800B2F34();

    temp = gGold;
    
    setMoneySprites();
    
    func_80045E20(1, 0x91, (u32)&_moneyTextureSegmentRomStart, (u32)&_moneyTextureSegmentRomEnd, (u32)&_moneyAssetsIndexSegmentRomStart, (u32)&_moneyAssetsIndexSegmentRomEnd, (void*)0x80253B00, (void*)0x80254A00, (void*)0x80254E00, (void*)0x80254F00, 0, 1, 0, 0.0f, 0.0f, 320.0f, 0xA);
    dmaOverlayScreenSprites(1, temp, 5, 3);
    setOverlayScreenSpritesRGBA(1, 0xFF, 0xFF, 0xFF, 0xFF);

    dmaSprite(0x82, (u32)&_evaluationScreenTextureSegmentRomStart, (u32)&_evaluationScreenTextureSegmentRomEnd, (u32)&_evaluationScreenAssetsIndexSegmentRomStart, (u32)&_evaluationScreenAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
    setSpriteViewSpacePosition(0x82, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x82, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x82, 2);
    setBilinearFiltering(0x82, 1);
    startSpriteAnimation(0x82, 0, 0);

    dmaSprite(0x81, (u32)&_snapshotLargeTextureSegmentRomStart, (u32)&_snapshotLargeTextureSegmentRomEnd, (u32)&_snapshotLargeAssetsIndexSegmentRomStart, (u32)&_snapshotLargeAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x80289900, NULL, (u16*)0x8028EF00, (u16*)0x8028F000, (u8*)0x8028F100, NULL, 0, 0);
    setSpriteViewSpacePosition(0x81, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x81, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x81, 2);
    setBilinearFiltering(0x81, 1);
    startSpriteAnimation(0x81, 0, 0);

    temp2 = func_8009BC44();
    
    if (temp2 != 0xFF) {
        dmaSprite(0x83, (u32)&_evaluationScreenTextureSegmentRomStart, (u32)&_evaluationScreenTextureSegmentRomEnd, (u32)&_evaluationScreenAssetsIndexSegmentRomStart, (u32)&_evaluationScreenAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, 0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
        setSpriteViewSpacePosition(0x83, 32.0f, 36.0f, 16.0f);
        setSpriteColor(0x83, 0xFF, 0xFF, 0xFF, 0);
        setSpriteRenderingLayer(0x83, 2);
        setBilinearFiltering(0x83, 1);
        startSpriteAnimation(0x83, temp2 + 1, 0);
    }

    temp2 = func_8009BC54();
    
    if (temp2 != 0xFF) {
        dmaSprite(0x84, (u32)&_evaluationScreenTextureSegmentRomStart, (u32)&_evaluationScreenTextureSegmentRomEnd, (u32)&_evaluationScreenAssetsIndexSegmentRomStart, (u32)&_evaluationScreenAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, 0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
        setSpriteViewSpacePosition(0x84, 32.0f, 20.0f, 8.0f);
        setSpriteColor(0x84, 0xFF, 0xFF, 0xFF, 0);
        setSpriteRenderingLayer(0x84, 2);
        setBilinearFiltering(0x84, 1);
        startSpriteAnimation(0x84, temp2 + 1, 0);
    }

    temp2 = func_8009BBAC();
    
    if (temp2 != 0xFF) {
        dmaSprite(0x85, (u32)&_evaluationScreenTextureSegmentRomStart, (u32)&_evaluationScreenTextureSegmentRomEnd, (u32)&_evaluationScreenAssetsIndexSegmentRomStart, (u32)&_evaluationScreenAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, 0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
        setSpriteViewSpacePosition(0x85, -24.0f, 4.0f, 8.0f);
        setSpriteColor(0x85, 0xFF, 0xFF, 0xFF, 0);
        setSpriteRenderingLayer(0x85, 2);
        setBilinearFiltering(0x85, 1);
        startSpriteAnimation(0x85, temp2 + 1, 0);
    }
    
    func_80045E20(2, 0x9A, (u32)&_bulletNumeralTextureSegmentRomStart, (u32)&_bulletNumeralTextureSegmentRomEnd, (u32)&_bulletNumeralAssetsIndexSegmentRomStart, (u32)&_bulletNumeralAssetsIndexSegmentRomEnd, (void*)0x8028F200, (void*)0x8028F880, (void*)0x8028F900, (void*)0x8028F9C0, 0, 1, 0, -60.0f, 4.0f, 256.0f, 8);
    dmaOverlayScreenSprites(2, func_8009B3DC() & 0xFF, 1, 3);
    setOverlayScreenSpritesRGBA(2, 0xFF, 0xFF, 0xFF, 0xFF);
    
    func_80045E20(3, 0x9B, (u32)&_bulletNumeralTextureSegmentRomStart, (u32)&_bulletNumeralTextureSegmentRomEnd, (u32)&_bulletNumeralAssetsIndexSegmentRomStart, (u32)&_bulletNumeralAssetsIndexSegmentRomEnd, (void*)0x8028F200, (void*)0x8028F880, (void*)0x8028F900, (void*)0x8028F9C0, 0, 1, 0, -60.0f, -12.0f, 256.0f, 8);
    dmaOverlayScreenSprites(3, func_8009B4EC() & 0xFF, 1, 3);
    setOverlayScreenSpritesRGBA(3, 0xFF, 0xFF, 0xFF, 0xFF);
    
    func_80045E20(4, 0x9C, (u32)&_bulletNumeralTextureSegmentRomStart, (u32)&_bulletNumeralTextureSegmentRomEnd, (u32)&_bulletNumeralAssetsIndexSegmentRomStart, (u32)&_bulletNumeralAssetsIndexSegmentRomEnd, (void*)0x8028F200, (void*)0x8028F880, (void*)0x8028F900, (void*)0x8028F9C0, 0, 1, 0, -60.0f, -28.0f, 256.0f, 8);
    dmaOverlayScreenSprites(4, func_8009B564() & 0xFF, 1, 3);
    setOverlayScreenSpritesRGBA(4, 0xFF, 0xFF, 0xFF, 0xFF);
    
    func_80045E20(5, 0x9D, (u32)&_bulletNumeralTextureSegmentRomStart, (u32)&_bulletNumeralTextureSegmentRomEnd, (u32)&_bulletNumeralAssetsIndexSegmentRomStart, (u32)&_bulletNumeralAssetsIndexSegmentRomEnd, (void*)0x8028F200, (void*)0x8028F880, (void*)0x8028F900, (void*)0x8028F9C0, 0, 1, 0, -16.0f, -44.0f, 256.0f, 8);
    dmaOverlayScreenSprites(5, gLumber, 3, 3);
    setOverlayScreenSpritesRGBA(5, 0xFF, 0xFF, 0xFF, 0xFF);
    
    func_80045E20(6, 0xA0, (u32)&_bulletNumeralTextureSegmentRomStart, (u32)&_bulletNumeralTextureSegmentRomEnd, (u32)&_bulletNumeralAssetsIndexSegmentRomStart, (u32)&_bulletNumeralAssetsIndexSegmentRomEnd, (void*)0x8028F200, (void*)0x8028F880, (void*)0x8028F900, (void*)0x8028F9C0, 0, 1, 0, -16.0f, -60.0f, 256.0f, 8);
    dmaOverlayScreenSprites(6, fodderQuantity, 3, 3);
    setOverlayScreenSpritesRGBA(6, 0xFF, 0xFF, 0xFF, 0xFF);
    
    setMessageBoxViewSpacePosition(0, 44.0f, 20.0f, 352.0f);
    setMessageBoxSpriteIndices(0, 0xFF, 0xFF, 0xFF);
    func_8003F360(0, -4, 2);
    initializeMessageBox(0, 0, 5, 0x8000);
    
    func_800C2060();
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C2060);

void func_800C2060(void) {
    updateSpriteAlpha(0x89, 0xFF, 0x18);
    updateSpriteAlpha(0x8C, 0xFF, 0x18);
    updateSpriteAlpha(0x8D, 0xFF, 0x18);
    updateSpriteAlpha(0x8B, 0xFF, 0x18);
    updateSpriteAlpha(0x8A, 0xFF, 0x18);
    updateSpriteAlpha(0x8E, 0xFF, 0x18);
    updateSpriteAlpha(0x81, 0xFF, 0x18);
    updateSpriteAlpha(0x82, 0xFF, 0x18);
    updateSpriteAlpha(0x83, 0xFF, 0x18);
    updateSpriteAlpha(0x84, 0xFF, 0x18);
    updateSpriteAlpha(0x85, 0xFF, 0x18);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C2128);

void func_800C2128(void) {

    updateSpriteAlpha(0x89, 0, 0x18);
    updateSpriteAlpha(0x8C, 0, 0x18);
    updateSpriteAlpha(0x8D, 0, 0x18);
    updateSpriteAlpha(0x8B, 0, 0x18);
    updateSpriteAlpha(0x8A, 0, 0x18);
    updateSpriteAlpha(0x8E, 0, 0x18);
    updateSpriteAlpha(0x81, 0, 0x18);
    updateSpriteAlpha(0x82, 0, 0x18);
    updateSpriteAlpha(0x83, 0, 0x18);
    updateSpriteAlpha(0x84, 0, 0x18);
    
    updateSpriteAlpha(0x85, 0, 0x18);
    
    messageBoxes[0].flags &= ~0x8000;
    
    func_8003F130(0);
    deactivateOverlayScreenSprites(0);
    deactivateOverlayScreenSprites(1);
    deactivateOverlayScreenSprites(2);
    deactivateOverlayScreenSprites(3);
    deactivateOverlayScreenSprites(4);
    deactivateOverlayScreenSprites(5);
    deactivateOverlayScreenSprites(6);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C224C);

void func_800C224C(void) {

    u8 temp = 0;

    switch (overlayScreenTable.subscreen) {
        case 4:
            break;
        case 0:
            func_800C1710();
            overlayScreenTable.subscreen = 1;
            break;
        case 1:
            if (func_8002CBF8(0x89)) {
                overlayScreenTable.subscreen = 4;
            }
            break;
        case 2:
            if (func_8002CBF8(0x89)) {
                func_800B2CE0();
                overlayScreenTable.subscreen = 3;
            }
            break;
        case 3:
            deactivateSprite(0x80);
            func_80059368();
            func_80059300();
            break;
    }
    
    if ((overlayScreenTable.subscreen >= 4) && (checkButtonPressed(CONTROLLER_1, BUTTON_B)) && (temp == 0)) {
        func_800C2128();
        overlayScreenTable.subscreen = 2;
        playSfx(1);
    }
    
}


//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C2364);

void func_800C2364(void) {
    
    func_800B2F34();
    
    dmaSprite(0x82, (u32)&_powerNutTreeDrawingTextureSegmentRomStart, (u32)&_powerNutTreeDrawingTextureSegmentRomEnd, (u32)&_powerNutTreeDrawingAssetsIndexSegmentRomStart, (u32)&_powerNutTreeDrawingAssetsIndexSegmentRomEnd,  0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
    setSpriteViewSpacePosition(0x82, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x82, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x82, 2);
    setBilinearFiltering(0x82, 1);
    startSpriteAnimation(0x82, 0, 0);
    
    dmaSprite(0x79, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8023B400, NULL, (u16*)0x8023CC00, (u16*)0x8023CE00, (u8*)0x8023D200, NULL, 0, 0);
    setBilinearFiltering(0x79, 1);
    setSpriteColor(0x79, 0xFF, 0xFF, 0xFF, 0xFF);
    
    func_800C2504();
    func_800C292C();

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C2504);

void func_800C2504(void) {

    u8 i;

    for (i = 0; i < 10; i++) {

        if (powerNutBits & D_80116F24[i]) {

            dmaSprite(i + 0xA3, (u32)&_powerNutTreeDrawingTextureSegmentRomStart, (u32)&_powerNutTreeDrawingTextureSegmentRomEnd, (u32)&_powerNutTreeDrawingAssetsIndexSegmentRomStart, (u32)&_powerNutTreeDrawingAssetsIndexSegmentRomEnd, 0, 0, (u8* )0x8026F000, NULL, (u16* )0x8027E200, (u16* )0x8027E700, (u8* )0x8027EB00, NULL, 0, 0);
            
            setSpriteViewSpacePosition(i + 0xA3, D_80116E18[i], D_80116E40[i], 8.0f);
            setSpriteColor(i + 0xA3, 0xFF, 0xFF, 0xFF, 0);
            setSpriteRenderingLayer(i + 0xA3, 2);
            setBilinearFiltering(i + 0xA3, 1);
            startSpriteAnimation(i + 0xA3, 1, 0);
            
        }
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C2660);

void func_800C2660(void) {
    
    resetAnimationState(0xA3);
    resetAnimationState(0xA4);
    resetAnimationState(0xA5);
    resetAnimationState(0xA6);
    resetAnimationState(0xA7);
    resetAnimationState(0xA8);
    resetAnimationState(0xA9);
    resetAnimationState(0xAA);
    resetAnimationState(0xAB);
    resetAnimationState(0xAC);
    
    startSpriteAnimation(overlayScreenTable.cellIndex + 0xA3, 2, 0xFE);
    startSpriteAnimation(0xA3, 1, 0);
    startSpriteAnimation(0xA4, 1, 0);
    startSpriteAnimation(0xA5, 1, 0);
    startSpriteAnimation(0xA6, 1, 0);
    startSpriteAnimation(0xA7, 1, 0);
    startSpriteAnimation(0xA8, 1, 0);
    startSpriteAnimation(0xA9, 1, 0);
    startSpriteAnimation(0xAA, 1, 0);
    startSpriteAnimation(0xAB, 1, 0);
    startSpriteAnimation(0xAC, 1, 0);
    
}

// power nuts
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

    deactivateOverlayScreenSprites(0);
    deactivateOverlayScreenSprites(1);

}

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C2B8C);

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C2F50);

void func_800C2F50(void) {
    
    dmaSprite(0x82, (u32)&_calendarTextureSegmentRomStart, (u32)&_calendarTextureSegmentRomEnd, (u32)&_calendarAssetsIndexSegmentRomStart, (u32)&_calendarAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
    setSpriteViewSpacePosition(0x82, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x82, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x82, 2);
    setBilinearFiltering(0x82, 1);
    startSpriteAnimation(0x82, 0, 0);
    
    dmaSprite(0x79, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8023B400, NULL, (u16*)0x8023CC00, (u16*)0x8023CE00, (u8*)0x8023D200, NULL, 0, 0);
    setBilinearFiltering(0x79, 1);
    setSpriteColor(0x79, 0xFF, 0xFF, 0xFF, 0xFF);
    
    func_800C3108();
    func_800C34C0(gYear, gSeason, gDayOfMonth);
    func_800C39A0();

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C3108);

void func_800C3108(void) {

    u8 i, j, k;
    u8 temp;
    
    D_80205640[0] = 0;
    D_80205640[1] = 1;
    D_80205640[2] = 2;
    D_80205640[3] = 3;
    D_80205640[4] = 4;

    if (checkLifeEventBit(0x5B)) {
        D_80205640[5] = 5;
        D_80205640[6] = 6;
        D_80205640[7] = 7;
        D_80205640[8] = 8;
        D_80205640[9] = 9;
    } else {
        D_80205640[5] = 0xFF;
        D_80205640[6] = 0xFF;
        D_80205640[7] = 0xFF;
        D_80205640[8] = 0xFF;
        D_80205640[9] = 0xFF;
    }

    for (i = 0; i < 5; i++) {
        
        for (j = 0; j < 7; j++) {

            temp = D_80189108[i][j];

            if (temp != 0xFF) {

                D_80205640[temp] = 0xFF;
                
                dmaSprite(temp + 0xAD, (u32)&_calendarTextureSegmentRomStart, (u32)&_calendarTextureSegmentRomEnd, (u32)&_calendarAssetsIndexSegmentRomStart, (u32)&_calendarAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
                setSpriteViewSpacePosition(temp + 0xAD, D_80116E68[0], D_80116E90[0], 8.0f);
                setSpriteViewSpacePosition(temp + 0xAD, (j << 5) - 0x7A, -(i * 0x18) + 0x1A, 4.0f);
                setSpriteColor(temp + 0xAD, 0xFF, 0xFF, 0xFF, 0);
                setSpriteRenderingLayer(temp + 0xAD, 2);
                setBilinearFiltering(temp + 0xAD, 1);
                startSpriteAnimation(temp + 0xAD, 1, temp);
                
            }
            
        } 
    
    }

    
    for (k = 0; k < 10; k++) {

        if (D_80205640[k] != 0xFF) {

            dmaSprite(k + 0xAD, (u32)&_calendarTextureSegmentRomStart, (u32)&_calendarTextureSegmentRomEnd, (u32)&_calendarAssetsIndexSegmentRomStart, (u32)&_calendarAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
            setSpriteViewSpacePosition(k + 0xAD, D_80116E68[k], D_80116E90[k], 8.0f);
            setSpriteColor(k + 0xAD, 0xFF, 0xFF, 0xFF, 0);
            setSpriteRenderingLayer(k + 0xAD, 2);
            setBilinearFiltering(k + 0xAD, 1);
            startSpriteAnimation(k + 0xAD, 1, k);
            
        }
        
    }

}

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C34C0);

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C39A0);

void func_800C39A0(void) {
    
    u8 i;

    updateSpriteAlpha(0x89, 0xFF, 0x18);
    updateSpriteAlpha(0x8C, 0xFF, 0x18);
    updateSpriteAlpha(0x8D, 0xFF, 0x18);
    updateSpriteAlpha(0x8B, 0xFF, 0x18);
    updateSpriteAlpha(0x8A, 0xFF, 0x18);
    updateSpriteAlpha(0x8E, 0xFF, 0x18);
    updateSpriteAlpha(0x82, 0xFF, 0x18);
    updateSpriteAlpha(0x83, 0xFF, 0x18);
    updateSpriteAlpha(0xB7, 0xFF, 0x18);
    updateSpriteAlpha(0xAD, 0xFF, 0x18);
    updateSpriteAlpha(0xAE, 0xFF, 0x18);
    updateSpriteAlpha(0xAF, 0xFF, 0x18);
    updateSpriteAlpha(0xB0, 0xFF, 0x18);
    updateSpriteAlpha(0xB1, 0xFF, 0x18);
    updateSpriteAlpha(0xB2, 0xFF, 0x18);
    updateSpriteAlpha(0xB3, 0xFF, 0x18);
    updateSpriteAlpha(0xB4, 0xFF, 0x18);
    updateSpriteAlpha(0xB5, 0xFF, 0x18);
    updateSpriteAlpha(0xB6, 0xFF, 0x18);
    
    for (i = 0; i < 30; i++) {
        updateSpriteAlpha(i + 0x8F, 0xFF, 0x18);
    }
    
}


//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C3B1C);

void func_800C3B1C(void) {

    u8 i;
    
    updateSpriteAlpha(0x89, 0, 0x18);
    updateSpriteAlpha(0x8C, 0, 0x18);
    updateSpriteAlpha(0x8D, 0, 0x18);
    updateSpriteAlpha(0x8B, 0, 0x18);
    updateSpriteAlpha(0x8A, 0, 0x18);
    updateSpriteAlpha(0x8E, 0, 0x18);
    updateSpriteAlpha(0x82, 0, 0x18);
    updateSpriteAlpha(0x83, 0, 0x18);
    updateSpriteAlpha(0xB7, 0, 0x18);

    setSpriteRenderingLayer(0xAD, 2);
    setSpriteRenderingLayer(0xAE, 2);
    setSpriteRenderingLayer(0xAF, 2);
    setSpriteRenderingLayer(0xB0, 2);
    setSpriteRenderingLayer(0xB1, 2);
    setSpriteRenderingLayer(0xB2, 2);
    setSpriteRenderingLayer(0xB3, 2);
    setSpriteRenderingLayer(0xB4, 2);
    setSpriteRenderingLayer(0xB5, 2);
    setSpriteRenderingLayer(0xB6, 2);

    updateSpriteAlpha(0xAD, 0, 0x18);
    updateSpriteAlpha(0xAE, 0, 0x18);
    updateSpriteAlpha(0xAF, 0, 0x18);
    updateSpriteAlpha(0xB0, 0, 0x18);
    updateSpriteAlpha(0xB1, 0, 0x18);
    updateSpriteAlpha(0xB2, 0, 0x18);
    updateSpriteAlpha(0xB3, 0, 0x18);
    updateSpriteAlpha(0xB4, 0, 0x18);
    updateSpriteAlpha(0xB5, 0, 0x18);
    updateSpriteAlpha(0xB6, 0, 0x18);
    
    for (i = 0; i < 30; i++) {
        updateSpriteAlpha(i + 0x8F, 0, 0x18);
    }
    
    deactivateOverlayScreenSprites(0);
    deactivateOverlayScreenSprites(1);
    
}

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C3D20);

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C4A74);

void func_800C4A74(void) {
    
    dmaSprite(0x81, (u32)&_bookTextureSegmentRomStart, (u32)&_bookTextureSegmentRomEnd, (u32)&_bookAssetsIndexSegmentRomStart, (u32)&_bookAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x80289900, NULL, (u16*)0x8028EF00, (u16*)0x8028F000, (u8*)0x8028F100, NULL, 0, 0);
    setSpriteViewSpacePosition(0x81, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x81, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x81, 2);
    setBilinearFiltering(0x81, 1);
    startSpriteAnimation(0x81, 0, 0);
    
    dmaSprite(0x82, (u32)&_diaryOptionsTextureSegmentRomStart, (u32)&_diaryOptionsTextureSegmentRomEnd, (u32)&_diaryOptionsAssetsIndexSegmentRomStart, (u32)&_diaryOptionsAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
    setSpriteViewSpacePosition(0x82, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x82, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x82, 2);
    setBilinearFiltering(0x82, 1);
    startSpriteAnimation(0x82, 1, 0);
    
    dmaSprite(0x79, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8023B400, NULL, (u16*)0x8023CC00, (u16*)0x8023CE00, (u8*)0x8023D200, NULL, 0, 0);
    setBilinearFiltering(0x79, 1);
    setSpriteColor(0x79, 0xFF, 0xFF, 0xFF, 0xFF);
    
    func_800C7850();
    func_800C7E78();
    func_800C4E90();
    
    setSpriteColor(0xA3, 0xFF, 0xFF, 0xFF, 0);
    setSpriteColor(0xA4, 0xFF, 0xFF, 0xFF, 0);
    setSpriteColor(0xA5, 0xFF, 0xFF, 0xFF, 0);
    setSpriteColor(0xA6, 0xFF, 0xFF, 0xFF, 0);
    setSpriteColor(0xA7, 0xFF, 0xFF, 0xFF, 0);
    setSpriteColor(0xA8, 0xFF, 0xFF, 0xFF, 0);
    setSpriteColor(0xA9, 0xFF, 0xFF, 0xFF, 0);
    setSpriteColor(0xAA, 0xFF, 0xFF, 0xFF, 0);
    setSpriteColor(0xAB, 0xFF, 0xFF, 0xFF, 0);
    setSpriteColor(0xAC, 0xFF, 0xFF, 0xFF, 0);
    setSpriteColor(0xAD, 0xFF, 0xFF, 0xFF, 0);
    setSpriteColor(0xAE, 0xFF, 0xFF, 0xFF, 0);
    setSpriteColor(0xAF, 0xFF, 0xFF, 0xFF, 0);
    setSpriteColor(0xB0, 0xFF, 0xFF, 0xFF, 0);
    setSpriteColor(0xB1, 0xFF, 0xFF, 0xFF, 0);
    setSpriteColor(0xB2, 0xFF, 0xFF, 0xFF, 0);
    
    func_800C5080();

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C4E90);

void func_800C4E90(void) {

    u8 i, j, k;

    for (i = 0; i < 0x10; i++) {
        resetAnimationState(i + 0xA3);
    }

    i = 0;

    for (k = 0; k < 2; k++) {
        
        for (j = 0; j < 8; j++) {

            if (checkRecipe(overlayScreenTable.pageNumber * 0x10 + i)) {

                dmaSprite(i + 0xA3, (u32)&_overlayScreenNamesTextureSegmentRomStart, (u32)&_overlayScreenNamesTextureSegmentRomEnd, (u32)&_overlayScreenNamesAssetsIndexSegmentRomStart, (u32)&_overlayScreenNamesAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8027EC00, NULL, (u16*)0x80289400, (u16*)0x80289500, (u8*)0x80289800, NULL, 0, 0);
                setSpriteViewSpacePosition(i + 0xA3, (k  << 7) + -120.0f, 64.0f - (j * 0x10), 8.0f);
                setSpriteColor(i + 0xA3, 0xFF, 0xFF, 0xFF, 0xFF);
                setSpriteRenderingLayer(i + 0xA3, 2);
                setBilinearFiltering(i + 0xA3, 1);
                startSpriteAnimation(i + 0xA3, 1, (i + (overlayScreenTable.pageNumber * 0x10)));
                
            }

            i++;
            
        }
        
    }
    
}

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

    deactivateOverlayScreenSprites(0);
    deactivateOverlayScreenSprites(1);

}

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C53C0);

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C5AA4);

void func_800C5AA4(void) {
    
    dmaSprite(0x81, (u32)&_bookTextureSegmentRomStart, (u32)&_bookTextureSegmentRomEnd, (u32)&_bookAssetsIndexSegmentRomStart, (u32)&_bookAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x80289900, NULL, (u16*)0x8028EF00, (u16*)0x8028F000, (u8*)0x8028F100, NULL, 0, 0);
    setSpriteViewSpacePosition(0x81, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x81, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x81, 2);
    setBilinearFiltering(0x81, 1);
    setSpritePaletteIndex(0x81, 1);
    startSpriteAnimation(0x81, 0, 0);
    
    dmaSprite(0x82, (u32)&_diaryOptionsTextureSegmentRomStart, (u32)&_diaryOptionsTextureSegmentRomEnd, (u32)&_diaryOptionsAssetsIndexSegmentRomStart, (u32)&_diaryOptionsAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
    setSpriteViewSpacePosition(0x82, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x82, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x82, 2);
    setBilinearFiltering(0x82, 1);
    startSpriteAnimation(0x82, 3, 0);
    
    dmaSprite(0x79, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8023B400, NULL, (u16*)0x8023CC00, (u16*)0x8023CE00, (u8*)0x8023D200, NULL, 0, 0);
    setBilinearFiltering(0x79, 1);
    setSpriteColor(0x79, 0xFF, 0xFF, 0xFF, 0xFF);
    
    func_800C7850();
    func_800C5DA4();
    
    setSpriteColor(0x83, 0xFF, 0xFF, 0xFF, 0);
    setSpriteColor(0x84, 0xFF, 0xFF, 0xFF, 0);
    setSpriteColor(0x85, 0xFF, 0xFF, 0xFF, 0);
    setSpriteColor(0x86, 0xFF, 0xFF, 0xFF, 0);
    
    func_800C6E98();
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C5DA4);

void func_800C5DA4(void) {

    resetAnimationState(0x83);
    resetAnimationState(0x84);
    resetAnimationState(0x85);
    resetAnimationState(0x86);

    switch (overlayScreenTable.pageNumber) {

        case 0:

            if (albumBits & 1) {
                dmaSprite(0x83, (u32)&_albumPhotosTextureSegmentRomStart, (u32)&_albumPhotosTextureSegmentRomEnd, (u32)&_albumPhotosAssetsIndexSegmentRomStart, (u32)&_albumPhotosAssetsIndexSegmentRomEnd, (u32)&_albumPhotosSpritesheetIndexSegmentRomStart, (u32)&_albumPhotosSpritesheetIndexSegmentRomEnd, (u8*)0x8028FA00, (u8*)0x80292A00, (u16*)0x802A7A00, (u16*)0x802AA200, (u8*)0x802AA600, (u32*)0x802AA800, 1, 0);
                setSpriteViewSpacePosition(0x83, -64.0f, 40.0f, 8.0f);
                setSpriteColor(0x83, 0xFF, 0xFF, 0xFF, 0xFF);
                setSpriteRenderingLayer(0x83, 2);
                setBilinearFiltering(0x83, 1);
                startSpriteAnimation(0x83, 0, 0);
            }
            
            if (albumBits & 2) {
                dmaSprite(0x84, (u32)&_albumPhotosTextureSegmentRomStart, (u32)&_albumPhotosTextureSegmentRomEnd, (u32)&_albumPhotosAssetsIndexSegmentRomStart, (u32)&_albumPhotosAssetsIndexSegmentRomEnd, (u32)&_albumPhotosSpritesheetIndexSegmentRomStart, (u32)&_albumPhotosSpritesheetIndexSegmentRomEnd, (u8*)0x80295A00, (u8*)0x80298A00, (u16*)0x802A7A00, (u16*)0x802AA200, (u8*)0x802AA600, (u32*)0x802AA800, 1, 0);
                setSpriteViewSpacePosition(0x84, -64.0f, -30.0f, 8.0f);
                setSpriteColor(0x84, 0xFF, 0xFF, 0xFF, 0xFF);
                setSpriteRenderingLayer(0x84, 2);
                setBilinearFiltering(0x84, 1);
                startSpriteAnimation(0x84, 1, 0);
            }
            
            if (albumBits & 4) {
                dmaSprite(0x85, (u32)&_albumPhotosTextureSegmentRomStart, (u32)&_albumPhotosTextureSegmentRomEnd, (u32)&_albumPhotosAssetsIndexSegmentRomStart, (u32)&_albumPhotosAssetsIndexSegmentRomEnd, (u32)&_albumPhotosSpritesheetIndexSegmentRomStart, (u32)&_albumPhotosSpritesheetIndexSegmentRomEnd, (u8*)0x8029BA00, (u8*)0x8029EA00, (u16*)0x802A7A00, (u16*)0x802AA200, (u8*)0x802AA600, (u32*)0x802AA800, 1, 0);
                setSpriteViewSpacePosition(0x85, 64.0f, 40.0f, 8.0f);
                setSpriteColor(0x85, 0xFF, 0xFF, 0xFF, 0xFF);
                setSpriteRenderingLayer(0x85, 2);
                setBilinearFiltering(0x85, 1);
                startSpriteAnimation(0x85, 2, 0);
            }
            
            if (albumBits & 8) {
                dmaSprite(0x86, (u32)&_albumPhotosTextureSegmentRomStart, (u32)&_albumPhotosTextureSegmentRomEnd, (u32)&_albumPhotosAssetsIndexSegmentRomStart, (u32)&_albumPhotosAssetsIndexSegmentRomEnd, (u32)&_albumPhotosSpritesheetIndexSegmentRomStart, (u32)&_albumPhotosSpritesheetIndexSegmentRomEnd, (u8*)0x802A1A00, (u8*)0x802A4A00, (u16*)0x802A7A00, (u16*)0x802AA200, (u8*)0x802AA600, (u32*)0x802AA800, 1, 0);
                setSpriteViewSpacePosition(0x86, 64.0f, -30.0f, 8.0f);
                setSpriteColor(0x86, 0xFF, 0xFF, 0xFF, 0xFF);
                setSpriteRenderingLayer(0x86, 2);
                setBilinearFiltering(0x86, 1);
                startSpriteAnimation(0x86, 3, 0);
            }
            
            break;

        case 1:

            if (albumBits & 0x10) {
                dmaSprite(0x83, (u32)&_albumPhotosTextureSegmentRomStart, (u32)&_albumPhotosTextureSegmentRomEnd, (u32)&_albumPhotosAssetsIndexSegmentRomStart, (u32)&_albumPhotosAssetsIndexSegmentRomEnd, (u32)&_albumPhotosSpritesheetIndexSegmentRomStart, (u32)&_albumPhotosSpritesheetIndexSegmentRomEnd, (u8*)0x8028FA00, (u8*)0x80292A00, (u16*)0x802A7A00, (u16*)0x802AA200, (u8*)0x802AA600, (u32*)0x802AA800, 1, 0);
                setSpriteViewSpacePosition(0x83, -64.0f, 40.0f, 8.0f);
                setSpriteColor(0x83, 0xFF, 0xFF, 0xFF, 0xFF);
                setSpriteRenderingLayer(0x83, 2);
                setBilinearFiltering(0x83, 1);
                startSpriteAnimation(0x83, 4, 0);
            }
            
            if (albumBits & 0x20) {
                dmaSprite(0x84, (u32)&_albumPhotosTextureSegmentRomStart, (u32)&_albumPhotosTextureSegmentRomEnd, (u32)&_albumPhotosAssetsIndexSegmentRomStart, (u32)&_albumPhotosAssetsIndexSegmentRomEnd, (u32)&_albumPhotosSpritesheetIndexSegmentRomStart, (u32)&_albumPhotosSpritesheetIndexSegmentRomEnd, (u8*)0x80295A00, (u8*)0x80298A00, (u16*)0x802A7A00, (u16*)0x802AA200, (u8*)0x802AA600, (u32*)0x802AA800, 1, 0);
                setSpriteViewSpacePosition(0x84, -64.0f, -30.0f, 8.0f);
                setSpriteColor(0x84, 0xFF, 0xFF, 0xFF, 0xFF);
                setSpriteRenderingLayer(0x84, 2);
                setBilinearFiltering(0x84, 1);
                startSpriteAnimation(0x84, 5, 0);
            }
            
            if (albumBits & 0x40) {
                dmaSprite(0x85, (u32)&_albumPhotosTextureSegmentRomStart, (u32)&_albumPhotosTextureSegmentRomEnd, (u32)&_albumPhotosAssetsIndexSegmentRomStart, (u32)&_albumPhotosAssetsIndexSegmentRomEnd, (u32)&_albumPhotosSpritesheetIndexSegmentRomStart, (u32)&_albumPhotosSpritesheetIndexSegmentRomEnd, (u8*)0x8029BA00, (u8*)0x8029EA00, (u16*)0x802A7A00, (u16*)0x802AA200, (u8*)0x802AA600, (u32*)0x802AA800, 1, 0);
                setSpriteViewSpacePosition(0x85, 64.0f, 40.0f, 8.0f);
                setSpriteColor(0x85, 0xFF, 0xFF, 0xFF, 0xFF);
                setSpriteRenderingLayer(0x85, 2);
                setBilinearFiltering(0x85, 1);
                startSpriteAnimation(0x85, 6, 0);
            }
            
            if (albumBits & 0x80) {
                
                dmaSprite(0x86, (u32)&_albumPhotosTextureSegmentRomStart, (u32)&_albumPhotosTextureSegmentRomEnd, (u32)&_albumPhotosAssetsIndexSegmentRomStart, (u32)&_albumPhotosAssetsIndexSegmentRomEnd, (u32)&_albumPhotosSpritesheetIndexSegmentRomStart, (u32)&_albumPhotosSpritesheetIndexSegmentRomEnd, (u8*)0x802A1A00, (u8*)0x802A4A00, (u16*)0x802A7A00, (u16*)0x802AA200, (u8*)0x802AA600, (u32*)0x802AA800, 1, 0);
                setSpriteViewSpacePosition(0x86, 64.0f, -30.0f, 8.0f);
                setSpriteColor(0x86, 0xFF, 0xFF, 0xFF, 0xFF);
                setSpriteRenderingLayer(0x86, 2);
                setBilinearFiltering(0x86, 1);
                
                switch (gWife) {                        
                    case 0:                                 
                        startSpriteAnimation(0x86, 8, 0);
                        break;
                    case 1:                                 
                        startSpriteAnimation(0x86, 9, 0);
                        break;
                    case 2:                                 
                        startSpriteAnimation(0x86, 7, 0);
                        break;
                    case 3:                                 
                        startSpriteAnimation(0x86, 11, 0);
                        break;
                    case 4:                                 
                        startSpriteAnimation(0x86, 10, 0);
                        break;
                    }
                
            }
            
            break;

        case 2:

            if (albumBits & 0x100) {
                
                dmaSprite(0x83, (u32)&_albumPhotosTextureSegmentRomStart, (u32)&_albumPhotosTextureSegmentRomEnd, (u32)&_albumPhotosAssetsIndexSegmentRomStart, (u32)&_albumPhotosAssetsIndexSegmentRomEnd, (u32)&_albumPhotosSpritesheetIndexSegmentRomStart, (u32)&_albumPhotosSpritesheetIndexSegmentRomEnd, (u8*)0x8028FA00, (u8*)0x80292A00, (u16*)0x802A7A00, (u16*)0x802AA200, (u8*)0x802AA600, (u32*)0x802AA800, 1, 0);
                setSpriteViewSpacePosition(0x83, -64.0f, 40.0f, 8.0f);
                setSpriteColor(0x83, 0xFF, 0xFF, 0xFF, 0xFF);
                setSpriteRenderingLayer(0x83, 2);
                setBilinearFiltering(0x83, 1);

                switch (gWife) {           
                    
                    case 0:                                 
                        startSpriteAnimation(0x83, 0xD, 0);
                        break;
                    case 1:                                 
                        startSpriteAnimation(0x83, 0xE, 0);
                        break;
                    case 2:                                 
                        startSpriteAnimation(0x83, 0xC, 0);
                        break;
                    case 3:                                 
                        startSpriteAnimation(0x83, 0x10, 0);
                        break;
                    case 4:                                 
                        startSpriteAnimation(0x83, 0xF, 0);
                        break;
                    }
                
            }
            
            if (albumBits & 0x200) {
                dmaSprite(0x84, (u32)&_albumPhotosTextureSegmentRomStart, (u32)&_albumPhotosTextureSegmentRomEnd, (u32)&_albumPhotosAssetsIndexSegmentRomStart, (u32)&_albumPhotosAssetsIndexSegmentRomEnd, (u32)&_albumPhotosSpritesheetIndexSegmentRomStart, (u32)&_albumPhotosSpritesheetIndexSegmentRomEnd, (u8*)0x80295A00, (u8*)0x80298A00, (u16*)0x802A7A00, (u16*)0x802AA200, (u8*)0x802AA600, (u32*)0x802AA800, 1, 0);
                setSpriteViewSpacePosition(0x84, -64.0f, -30.0f, 8.0f);
                setSpriteColor(0x84, 0xFF, 0xFF, 0xFF, 0xFF);
                setSpriteRenderingLayer(0x84, 2);
                setBilinearFiltering(0x84, 1);
                startSpriteAnimation(0x84, 0x11, 0);
            }
            
            if (albumBits & 0x400) {
                dmaSprite(0x85, (u32)&_albumPhotosTextureSegmentRomStart, (u32)&_albumPhotosTextureSegmentRomEnd, (u32)&_albumPhotosAssetsIndexSegmentRomStart, (u32)&_albumPhotosAssetsIndexSegmentRomEnd, (u32)&_albumPhotosSpritesheetIndexSegmentRomStart, (u32)&_albumPhotosSpritesheetIndexSegmentRomEnd, (u8*)0x8029BA00, (u8*)0x8029EA00, (u16*)0x802A7A00, (u16*)0x802AA200, (u8*)0x802AA600, (u32*)0x802AA800, 1, 0);
                setSpriteViewSpacePosition(0x85, 64.0f, 40.0f, 8.0f);
                setSpriteColor(0x85, 0xFF, 0xFF, 0xFF, 0xFF);
                setSpriteRenderingLayer(0x85, 2);
                setBilinearFiltering(0x85, 1);
                startSpriteAnimation(0x85, 0x12, 0);
            }
            
            if (albumBits & 0x800) {
                dmaSprite(0x86, (u32)&_albumPhotosTextureSegmentRomStart, (u32)&_albumPhotosTextureSegmentRomEnd, (u32)&_albumPhotosAssetsIndexSegmentRomStart, (u32)&_albumPhotosAssetsIndexSegmentRomEnd, (u32)&_albumPhotosSpritesheetIndexSegmentRomStart, (u32)&_albumPhotosSpritesheetIndexSegmentRomEnd, (u8*)0x802A1A00, (u8*)0x802A4A00, (u16*)0x802A7A00, (u16*)0x802AA200, (u8*)0x802AA600, (u32*)0x802AA800, 1, 0);
                setSpriteViewSpacePosition(0x86, 64.0f, -30.0f, 8.0f);
                setSpriteColor(0x86, 0xFF, 0xFF, 0xFF, 0xFF);
                setSpriteRenderingLayer(0x86, 2);
                setBilinearFiltering(0x86, 1);
                startSpriteAnimation(0x86, 0x13, 0);
            }
            
            break;

        case 3:

            if (albumBits & 0x1000) {
                dmaSprite(0x83, (u32)&_albumPhotosTextureSegmentRomStart, (u32)&_albumPhotosTextureSegmentRomEnd, (u32)&_albumPhotosAssetsIndexSegmentRomStart, (u32)&_albumPhotosAssetsIndexSegmentRomEnd, (u32)&_albumPhotosSpritesheetIndexSegmentRomStart, (u32)&_albumPhotosSpritesheetIndexSegmentRomEnd, (u8*)0x8028FA00, (u8*)0x80292A00, (u16*)0x802A7A00, (u16*)0x802AA200, (u8*)0x802AA600, (u32*)0x802AA800, 1, 0);
                setSpriteViewSpacePosition(0x83, -64.0f, 40.0f, 8.0f);
                setSpriteColor(0x83, 0xFF, 0xFF, 0xFF, 0xFF);
                setSpriteRenderingLayer(0x83, 2);
                setBilinearFiltering(0x83, 1);
                startSpriteAnimation(0x83, 0x14, 0);
            }
            
            if (albumBits & 0x2000) {
                dmaSprite(0x84, (u32)&_albumPhotosTextureSegmentRomStart, (u32)&_albumPhotosTextureSegmentRomEnd, (u32)&_albumPhotosAssetsIndexSegmentRomStart, (u32)&_albumPhotosAssetsIndexSegmentRomEnd, (u32)&_albumPhotosSpritesheetIndexSegmentRomStart, (u32)&_albumPhotosSpritesheetIndexSegmentRomEnd, (u8*)0x80295A00, (u8*)0x80298A00, (u16*)0x802A7A00, (u16*)0x802AA200, (u8*)0x802AA600, (u32*)0x802AA800, 1, 0);
                setSpriteViewSpacePosition(0x84, -64.0f, -30.0f, 8.0f);
                setSpriteColor(0x84, 0xFF, 0xFF, 0xFF, 0xFF);
                setSpriteRenderingLayer(0x84, 2);
                setBilinearFiltering(0x84, 1);
                startSpriteAnimation(0x84, 0x15, 0);
            }
            
            if (albumBits & 0x4000) {
                dmaSprite(0x85, (u32)&_albumPhotosTextureSegmentRomStart, (u32)&_albumPhotosTextureSegmentRomEnd, (u32)&_albumPhotosAssetsIndexSegmentRomStart, (u32)&_albumPhotosAssetsIndexSegmentRomEnd, (u32)&_albumPhotosSpritesheetIndexSegmentRomStart, (u32)&_albumPhotosSpritesheetIndexSegmentRomEnd, (u8*)0x8029BA00, (u8*)0x8029EA00, (u16*)0x802A7A00, (u16*)0x802AA200, (u8*)0x802AA600, (u32*)0x802AA800, 1, 0);
                setSpriteViewSpacePosition(0x85, 64.0f, 40.0f, 8.0f);
                setSpriteColor(0x85, 0xFF, 0xFF, 0xFF, 0xFF);
                setSpriteRenderingLayer(0x85, 2);
                setBilinearFiltering(0x85, 1);
                startSpriteAnimation(0x85, 0x16, 0);
            }
            
            if (albumBits & 0x8000) {
                dmaSprite(0x86, (u32)&_albumPhotosTextureSegmentRomStart, (u32)&_albumPhotosTextureSegmentRomEnd, (u32)&_albumPhotosAssetsIndexSegmentRomStart, (u32)&_albumPhotosAssetsIndexSegmentRomEnd, (u32)&_albumPhotosSpritesheetIndexSegmentRomStart, (u32)&_albumPhotosSpritesheetIndexSegmentRomEnd, (u8*)0x802A1A00, (u8*)0x802A4A00, (u16*)0x802A7A00, (u16*)0x802AA200, (u8*)0x802AA600, (u32*)0x802AA800, 1, 0);
                setSpriteViewSpacePosition(0x86, 64.0f, -30.0f, 8.0f);
                setSpriteColor(0x86, 0xFF, 0xFF, 0xFF, 0xFF);
                setSpriteRenderingLayer(0x86, 2);
                setBilinearFiltering(0x86, 1);
                startSpriteAnimation(0x86, 0x17, 0);
            }
            
            break;
        
    }
    
}

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

    deactivateOverlayScreenSprites(0);
    deactivateOverlayScreenSprites(1);

}

//INCLUDE_RODATA("asm/nonmatchings/game/overlayScreens", D_80121E34);

static const u8 D_80121E34[20] = { 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x2D, 0x2E, 0, 0, 0, 0 };

// D_80121E34
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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C7850);

void func_800C7850(void) {

    dmaSprite(0x7C, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8023B400, NULL, (u16*)0x8023CC00, (u16*)0x8023CE00, (u8*)0x8023D200, NULL, 0, 0);
    setSpriteViewSpacePosition(0x7C, 0.0f, 0.0f, 0.0f);
    setSpriteScale(0x7C, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(0x7C, 0.0f, 0.0f, 0.0f);
    setBilinearFiltering(0x7C, 1);
    setSpriteColor(0x7C, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002C680(0x7C, 2, 2);
    setSpriteRenderingLayer(0x7C, 3);
    
    dmaSprite(0x7D, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8023B400, NULL, (u16*)0x8023CC00, (u16*)0x8023CE00, (u8*)0x8023D200, NULL, 0, 0);
    setSpriteViewSpacePosition(0x7D, 0.0f, 0.0f, 0.0f);
    setSpriteScale(0x7D, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(0x7D, 0.0f, 0.0f, 0.0f);
    setBilinearFiltering(0x7D, 1);
    setSpriteColor(0x7D, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002C680(0x7D, 2, 2);
    setSpriteRenderingLayer(0x7D, 3);
    
    dmaSprite(0x7E, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8023B400, NULL, (u16*)0x8023CC00, (u16*)0x8023CE00, (u8*)0x8023D200, NULL, 0, 0);
    setSpriteViewSpacePosition(0x7E, 0.0f, 0.0f, 0.0f);
    setSpriteScale(0x7E, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(0x7E, 0.0f, 0.0f, 0.0f);
    setBilinearFiltering(0x7E, 1);
    setSpriteColor(0x7E, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002C680(0x7E, 2, 2);
    setSpriteRenderingLayer(0x7E, 3);
    
    dmaSprite(0x7F, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8023B400, NULL, (u16*)0x8023CC00, (u16*)0x8023CE00, (u8*)0x8023D200, NULL, 0, 0);
    setSpriteViewSpacePosition(0x7F, 0.0f, 0.0f, 0.0f);
    setSpriteScale(0x7F, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(0x7F, 0.0f, 0.0f, 0.0f);
    setBilinearFiltering(0x7F, 1);
    setSpriteColor(0x7F, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002C680(0x7F, 2, 2);
    setSpriteRenderingLayer(0x7F, 3);

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C7C88);

void func_800C7C88(f32 x, f32 y) {
    startSpriteAnimation(0x7C, 0xB, 0xFE);
    setSpriteViewSpacePosition(0x7C, x, y, 32.0f);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C7CE4);

void func_800C7CE4(f32 x, f32 y) {
    startSpriteAnimation(0x7D, 0xA, 0xFE);
    setSpriteViewSpacePosition(0x7D, x, y, 32.0f);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C7D40);

void func_800C7D40(f32 x, f32 y) {
    startSpriteAnimation(0x7F, 1, 0xFE);
    setSpriteViewSpacePosition(0x7F, x, y, 32.0f);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C7D9C);

void func_800C7D9C(f32 x, f32 y) {
    startSpriteAnimation(0x7E, 0xC, 0xFE);
    setSpriteViewSpacePosition(0x7E, x, y, 32.0f);
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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C7E78);

void func_800C7E78(void) {
    
    dmaSprite(0x7A, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8023B400, NULL, (u16*)0x8023CC00, (u16*)0x8023CE00, (u8*)0x8023D200, NULL, 0, 0);
    setSpriteViewSpacePosition(0x7A, 0.0f, 0.0f, 0.0f);
    setSpriteScale(0x7A, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(0x7A, 0.0f, 0.0f, 0.0f);
    setBilinearFiltering(0x7A, 1);
    setSpriteColor(0x7A, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002C680(0x7A, 2, 2);
    setSpriteRenderingLayer(0x7A, 3);

    dmaSprite(0x7B, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8023B400, NULL, (u16*)0x8023CC00, (u16*)0x8023CE00, (u8*)0x8023D200, NULL, 0, 0);
    setSpriteViewSpacePosition(0x7B, 0.0f, 0.0f, 0.0f);
    setSpriteScale(0x7B, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(0x7B, 0.0f, 0.0f, 0.0f);
    setBilinearFiltering(0x7B, 1);
    setSpriteColor(0x7B, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002C680(0x7B, 2, 2);
    setSpriteRenderingLayer(0x7B, 3);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C80F0);

void func_800C80F0(f32 x, f32 y) {
    startSpriteAnimation(0x7B, 0xB, 0xFE);
    setSpriteViewSpacePosition(0x7B, x, y, 32.0f);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C814C);

void func_800C814C(f32 x, f32 y) {
    startSpriteAnimation(0x7A, 0xA, 0xFE);
    setSpriteViewSpacePosition(0x7A, x, y, 32.0f);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C81A8);

void func_800C81A8(void) {
    resetAnimationState(0x7B);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C81C8);

void func_800C81C8(void) {
    resetAnimationState(0x7A);
}

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C81E8);

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C8424);

void func_800C8424(void) {
    
    dmaSprite(0x82, (u32)&_emptyTexture2SegmentRomStart, (u32)&_emptyTexture2SegmentRomEnd, (u32)&_emptyTexture2AssetsIndexSegmentRomStart, (u32)&_emptyTexture2AssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x80282000, (u16*)0x80282300, (u8*)0x80282400, NULL, 0, 0);
    setSpriteViewSpacePosition(0x82, 0.0f, 0.0f, 0.0f);
    setSpriteScale(0x82, 1.0f, 1.0f, 1.0f);
    setBilinearFiltering(0x82, 1);
    setSpriteColor(0x82, 0, 0, 0, 0);
    startSpriteAnimation(0x82, 0, 0);
    
    dmaSprite(0xA3, (u32)&_emptyTexture2SegmentRomStart, (u32)&_emptyTexture2SegmentRomEnd, (u32)&_emptyTexture2AssetsIndexSegmentRomStart, (u32)&_emptyTexture2AssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x80282000, (u16*)0x80282300, (u8*)0x80282400, NULL, 0, 0);
    setSpriteScale(0xA3, 1.0f, 1.0f, 1.0f);
    setBilinearFiltering(0xA3, 1);
    setSpriteColor(0xA3, 0, 0, 0, 0);
    startSpriteAnimation(0xA3, 1, 0xFE);
    
    switch (gBaseMapIndex) {
        
        case RANCH:
        case ANN_ROOM:
        case RANCH_BARN:
        case RANCH_STORE:
        case RANCH_HOUSE:
            setSpriteViewSpacePosition(0xA3, 0.0f, -72.0f, 8.0f);
            break;

        case BEACH:
            setSpriteViewSpacePosition(0xA3, 56.0f, -96.0f, 8.0f);
            break;

        case MOUNTAIN_1:
            setSpriteViewSpacePosition(0xA3, 88.0f, 12.0f, 8.0f);
            break;

        case MOUNTAIN_2:
        case CARPENTER_HUT:
            setSpriteViewSpacePosition(0xA3, 32.0f, 20.0f, 8.0f);
            break;
        
        case TOP_OF_MOUNTAIN_1:
            setSpriteViewSpacePosition(0xA3, 104.0f, 48.0f, 8.0f);
            break;

        case MOON_MOUNTAIN:
        case DUMPLING_HOUSE:
            setSpriteViewSpacePosition(0xA3, 104.0f, 84.0f, 8.0f);
            break;

        case POND:
            setSpriteViewSpacePosition(0xA3, -16.0f, 36.0f, 8.0f);
            break;

        case ELLI_ROOM:
        case BAKERY:
        case VILLAGE_1:
        case POPURI_ROOM:
        case FLOWER_SHOP:
        case CHURCH:
        case SOUVENIR_SHOP:
        case RICK_STORE:
        case TAVERN:
        case EMPTY_MAP_2:
            setSpriteViewSpacePosition(0xA3, -20.0f, -24.0f, 8.0f);
            break;

        case VILLAGE_2:
        case MIDWIFE_HOUSE:
        case LIBRARY:
        case MARIA_ROOM:
        case MAYOR_HOUSE:
        case POTION_SHOP_BEDROOM:
        case POTION_SHOP:
            setSpriteViewSpacePosition(0xA3, -44.0f, -56.0f, 8.0f);
            break;

        case SQUARE:
            setSpriteViewSpacePosition(0xA3, -72.0f, -16.0f, 8.0f);
            break;

        case RACE_TRACK:
            setSpriteViewSpacePosition(0xA3, -124.0f, -24.0f, 8.0f);
            break;
        
        case HARVEST_SPRITE_CAVE:
        case CAVE:
        case MINE:
        case MINE_2:
            setSpriteViewSpacePosition(0xA3, 56.0f, 32.0f, 8.0f);
            break;
                
        case KAREN_ROOM:
        case VINEYARD:
        case VINEYARD_HOUSE:
        case VINEYARD_CELLAR:
        case VINEYARD_CELLAR_BASEMENT:
            setSpriteViewSpacePosition(0xA3, 16.0f, -4.0f, 8.0f);
            break;

        case ROAD:
            setSpriteViewSpacePosition(0xA3, 24.0f, -44.0f, 8.0f);
            break;
        
        case FARM:
        case GREENHOUSE:
        case HOUSE:
        case BARN:
        case COOP:
        case KITCHEN:
        case BATHROOM:
            setSpriteViewSpacePosition(0xA3, 64.0f, -56.0f, 8.0f);
            break;
        
    }
    
    overlayScreenTable.cellIndex = 0;
    overlayScreenTable.previousCellIndex = 0;
    overlayScreenTable.pageNumber = 0;
    overlayScreenTable.unk_3 = 0xFF;
    overlayScreenTable.subscreen = 0;
    overlayScreenTable.unk_8 = 0;
    overlayScreenTable.unk_9 = 0;
    overlayScreenTable.unk_A = 0;
    overlayScreenTable.unk_B = 0;
    
    setMainLoopCallbackFunctionIndex(0x2D);

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C8784);

void func_800C8784(void) {    

    u8 temp = 0;

    switch (overlayScreenTable.subscreen) {
        
        case 0:
            updateSpriteRGBA(0x82, 0xFF, 0xFF, 0xFF, 0xFF, 0x18);
            updateSpriteRGBA(0xA3, 0xFF, 0xFF, 0xFF, 0xFF, 0x18);
            overlayScreenTable.subscreen = 1;
            break;
            
        case 1:
            if (func_8002CBF8(0x82)) {
                overlayScreenTable.subscreen = 2;
            }
            break;
            
        case 2:
            if ((checkButtonPressed(CONTROLLER_1, BUTTON_B)) && (temp == 0)) {
                updateSpriteRGBA(0x82, 0, 0, 0, 0, 0x18);
                updateSpriteRGBA(0xA3, 0, 0, 0, 0, 0x18);
                overlayScreenTable.subscreen = 3;
                playSfx(1);
            }
            break;
            
        case 3:
            if (func_8002CBF8(0x82)) {
                func_800B2CE0();
                overlayScreenTable.subscreen = 4;
            }
            break;
            
        case 4:
            func_800B8844();
            break;
        }
    
}


//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C88F4);

void func_800C88F4(void) {

    func_800593EC();
    
    if (checkDailyEventBit(0x41)) {
        removeKeyItem(0xE);
    } else {
        removeKeyItem(0xF);
    }
    dmaSprite(0x80, (u32)&_checkerboardBackgroundTextureSegmentRomStart, (u32)&_checkerboardBackgroundTextureSegmentRomEnd, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomStart, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x802DE5C0, NULL, (u16*)0x802E0BC0, (u16*)0x802E0CC0, (u8*)0x802E0DC0, NULL, 0, 0);
    setSpriteViewSpacePosition(0x80, 0.0f, 0.0f, 0.0f);
    setSpriteScale(0x80, 2.0f, 2.0f, 1.0f);
    setBilinearFiltering(0x80, 1);
    setSpriteColor(0x80, 0xFF, 0xFF, 0xFF, 0xFF);
    startSpriteAnimation(0x80, 0, 0);
    
    overlayScreenTable.cellIndex = 0;
    overlayScreenTable.previousCellIndex = 0;
    overlayScreenTable.pageNumber = 0;
    overlayScreenTable.unk_3 = 0xFF;
    overlayScreenTable.subscreen = 0;
    
    overlayScreenStrings.strings2[(overlayScreenStrings.unk_6A)][0] = 0;
    overlayScreenStrings.strings2[(overlayScreenStrings.unk_6A)][1] = 0;
    overlayScreenStrings.strings2[(overlayScreenStrings.unk_6A)][2] = 0;
    overlayScreenStrings.strings2[(overlayScreenStrings.unk_6A)][3] = 0;
    overlayScreenStrings.strings2[(overlayScreenStrings.unk_6A)][4] = 0;
    overlayScreenStrings.strings2[(overlayScreenStrings.unk_6A)][5] = 0;

    setMainLoopCallbackFunctionIndex(0x2E);
    
}

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C8B04);

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C9FF0);

void func_800C9FF0(void) {
    
    setGameVariableString(0x2A, overlayScreenStrings.dialogueVariableStrings[0], 6);
    setGameVariableString(0x2B, overlayScreenStrings.dialogueVariableStrings[1], 6);
    setGameVariableString(0x2C, overlayScreenStrings.dialogueVariableStrings[2], 6);
    setGameVariableString(0x2D, overlayScreenStrings.dialogueVariableStrings[3], 6);
    setGameVariableString(0x2E, overlayScreenStrings.dialogueVariableStrings[4], 6);
    setGameVariableString(0x2F, overlayScreenStrings.dialogueVariableStrings[5], 6);
    
    initializeEmptyMessageBox(0, 0x8030B000);
    setMessageBoxViewSpacePosition(0, -22.0f, 24.0f, 0);
    func_8003F5D0(0, 0xA, 2);
    func_8003F630(0, 0, 2);
    func_8003F464(0, 0xE, 0xE, (u8*)0x802FF000, (u16*)0x8030A000);
    func_8003F360(0, -4, 2);
    setMessageBoxSpriteIndices(0, 0xFF, 0, 0);
    setMessageBoxButtonMask(0, 0x8000);
    func_8003FB4C(0, 1);
    initializeMessageBox(0, 0, 0x12, 0x80000);
    func_8003E77C(0, 0xFF, 0xFF, 0xFF, 0xFF);
    
    initializeEmptyMessageBox(1, 0x8030B400);
    setMessageBoxViewSpacePosition(1, -22.0f, 8.0f, 0);
    func_8003F5D0(1, 0xA, 2);
    func_8003F630(1, 0, 2);
    func_8003F464(1, 0xE, 0xE, (u8*)0x802FF000, (u16*)0x8030A000);
    func_8003F360(1, -4, 2);
    setMessageBoxSpriteIndices(1, 0xFF, 0, 0);
    setMessageBoxButtonMask(1, 0x8000);
    func_8003FB4C(1, 1);
    initializeMessageBox(1, 0, 0x13, 0x80000);
    func_8003E77C(1, 0xFF, 0xFF, 0xFF, 0xFF);
    
    initializeEmptyMessageBox(2, 0x8030B800);
    setMessageBoxViewSpacePosition(2, -22.0f, -8.0f, 0);
    func_8003F5D0(2, 0xA, 2);
    func_8003F630(2, 0, 2);
    func_8003F464(2, 0xE, 0xE, (u8*)0x802FF000, (u16*)0x8030A000);
    func_8003F360(2, -4, 2);
    setMessageBoxSpriteIndices(2, 0xFF, 0, 0);
    setMessageBoxButtonMask(2, 0x8000);
    func_8003FB4C(2, 1);
    initializeMessageBox(2, 0, 0x14, 0x80000);
    func_8003E77C(2, 0xFF, 0xFF, 0xFF, 0xFF);
    
    initializeEmptyMessageBox(3, 0x8030BC00);
    setMessageBoxViewSpacePosition(3, -22.0f, -24.0f, 0);
    func_8003F5D0(3, 0xA, 2);
    func_8003F630(3, 0, 2);
    func_8003F464(3, 0xE, 0xE, (u8*)0x802FF000, (u16*)0x8030A000);
    func_8003F360(3, -4, 2);
    setMessageBoxSpriteIndices(3, 0xFF, 0, 0);
    setMessageBoxButtonMask(3, 0x8000);
    func_8003FB4C(3, 1);
    initializeMessageBox(3, 0, 0x15, 0x80000);
    func_8003E77C(3, 0xFF, 0xFF, 0xFF, 0xFF);
    
    initializeEmptyMessageBox(4, 0x8030C000);
    setMessageBoxViewSpacePosition(4, -22.0f, -40.0f, 0);
    func_8003F5D0(4, 0xA, 2);
    func_8003F630(4, 0, 2);
    func_8003F464(4, 0xE, 0xE, (u8*)0x802FF000, (u16*)0x8030A000);
    func_8003F360(4, -4, 2);
    setMessageBoxSpriteIndices(4, 0xFF, 0, 0);
    setMessageBoxButtonMask(4, 0x8000);
    func_8003FB4C(4, 1);
    initializeMessageBox(4, 0, 0x16, 0x80000);
    func_8003E77C(4, 0xFF, 0xFF, 0xFF, 0xFF);
    
    initializeEmptyMessageBox(5, 0x8030C400);
    setMessageBoxViewSpacePosition(5, -22.0f, -56.0f, 0);
    func_8003F5D0(5, 0xA, 2);
    func_8003F630(5, 0, 2);
    func_8003F464(5, 0xE, 0xE, (u8*)0x802FF000, (u16*)0x8030A000);
    func_8003F360(5, -4, 2);
    setMessageBoxSpriteIndices(5, 0xFF, 0, 0);
    setMessageBoxButtonMask(5, 0x8000);
    func_8003FB4C(5, 1);
    initializeMessageBox(5, 0, 0x17, 0x80000);
    func_8003E77C(5, 0xFF, 0xFF, 0xFF, 0xFF);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CA590);

void func_800CA590(void) {
    updateSpriteAlpha(0x8E, 0xFF, 0x18);
    updateSpriteAlpha(0x81, 0xFF, 0x18);
    updateSpriteAlpha(0x82, 0xFF, 0x18);
    updateSpriteAlpha(0x83, 0xFF, 0x18);
    updateSpriteAlpha(0x84, 0xFF, 0x18);
    updateSpriteAlpha(0x85, 0xFF, 0x18);
    updateSpriteAlpha(0x86, 0xFF, 0x18);
    updateSpriteAlpha(0xB9, 0xFF, 0x18);
    updateSpriteAlpha(0xBA, 0xFF, 0x18);
    updateSpriteAlpha(0xBB, 0xFF, 0x18);
    updateSpriteAlpha(0xBC, 0xFF, 0x18);
    updateSpriteAlpha(0xBD, 0xFF, 0x18);
    updateSpriteAlpha(0xBE, 0xFF, 0x18);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CA678);

void func_800CA678(void) {
    updateSpriteAlpha(0x8E, 0, 0x18);
    updateSpriteAlpha(0x81, 0, 0x18);
    updateSpriteAlpha(0x82, 0, 0x18);
    updateSpriteAlpha(0x83, 0, 0x18);
    updateSpriteAlpha(0x84, 0, 0x18);
    updateSpriteAlpha(0x85, 0, 0x18);
    updateSpriteAlpha(0x86, 0, 0x18);
    updateSpriteAlpha(0xB9, 0, 0x18);
    updateSpriteAlpha(0xBA, 0, 0x18);
    updateSpriteAlpha(0xBB, 0, 0x18);
    updateSpriteAlpha(0xBC, 0, 0x18);
    updateSpriteAlpha(0xBD, 0, 0x18);
    updateSpriteAlpha(0xBE, 0, 0x18);
    func_8003DD14(0);
    func_8003DD14(1);
    func_8003DD14(2);
    func_8003DD14(3);
    func_8003DD14(4);
    func_8003DD14(5);
    deactivateOverlayScreenSprites(1);
    deactivateOverlayScreenSprites(2);
    deactivateOverlayScreenSprites(3);
    deactivateOverlayScreenSprites(4);
    deactivateOverlayScreenSprites(5);
    deactivateOverlayScreenSprites(6);
    deactivateOverlayScreenSprites(7);
    deactivateOverlayScreenSprites(8);
    deactivateOverlayScreenSprites(9);
    deactivateOverlayScreenSprites(0xA);
    deactivateOverlayScreenSprites(0xB);
    deactivateOverlayScreenSprites(0xC);
    deactivateOverlayScreenSprites(0xD);
    deactivateOverlayScreenSprites(0xE);
    func_80053088();
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CA808);

void func_800CA808(void) {

    u8 temp = 0;
    bool set = FALSE;
    
    switch (overlayScreenTable.subscreen) {
        
        case 0:
            func_800C8B04();
            overlayScreenTable.subscreen = 1;
            break;
        
        case 1:
            if (func_8002CBF8(0x82)) {
                setSpriteViewSpacePosition(0x79, -112.0f, (-(overlayScreenTable.cellIndex * 0x10) + 0x18), 32.0f);
                startSpriteAnimation(0x79, 3, 0);
                overlayScreenTable.subscreen = 4;
            }
            
            break;
        
        case 2:
            if (func_8002CBF8(0x82)) {
                func_800B2CE0();
                overlayScreenTable.subscreen = 3;
            }
            break;
        
        case 3:
            deactivateSprite(0x80);
            func_80059368();
            func_80059300();
            break;
        
        case 4:

            if (checkButtonRepeat(0, 0x100000) && !set) {
                
                if (overlayScreenTable.cellIndex) {
                    overlayScreenTable.cellIndex--;
                    playSfx(2);
                    setSpriteViewSpacePosition(0x79, -112.0f, (f32) (-(overlayScreenTable.cellIndex * 0x10) + 0x18), 32.0f);
                } else {
                    overlayScreenTable.cellIndex = 0xFF;
                    overlayScreenTable.subscreen = 6;
                    playSfx(2);
                    setSpriteViewSpacePosition(0x79, 96.0f, -96.0f, 32.0f);
                }
                
                set = TRUE;
                    
                
            }
            
            if (checkButtonRepeat(0, BUTTON_STICK_UP) && !set) {
                
                    if ((overlayScreenTable.cellIndex) != 5) {
                        overlayScreenTable.cellIndex++;
                        playSfx(2);
                        setSpriteViewSpacePosition(0x79, -112.0f, (f32) (-(overlayScreenTable.cellIndex * 16) + 24), 32.0f);
                    } else {
                        overlayScreenTable.cellIndex = 0xFF;
                        overlayScreenTable.subscreen = 6;
                        playSfx(2);
                        setSpriteViewSpacePosition(0x79, 96.0f, -96.0f, 32.0f);
                    }
                
                    set = TRUE;
                
            }
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A) && !set) {
                overlayScreenTable.subscreen = 5;
                setSpriteViewSpacePosition(0x79, 32.0f, (f32) (-(overlayScreenTable.cellIndex * 0x10) + 0x18), 32.0f);
                set = TRUE;
                playSfx(0);
            }
            
            if ((checkButtonPressed(CONTROLLER_1, BUTTON_START)) && !set) {
                overlayScreenTable.cellIndex = 0xFF;
                overlayScreenTable.subscreen = 6;
                playSfx(2);
                setSpriteViewSpacePosition(0x79, 96.0f, -96.0f, 32.0f);
            }
            
            break;

        case 5:

            if (checkButtonRepeat(CONTROLLER_1, 0x10000)) {
                
                if (!set) {
                    
                    if (adjustValue(D_80189858, 50, gGold) == 50) {
                        
                        if (overlayScreenStrings.strings2[(overlayScreenStrings.unk_6A)][overlayScreenTable.cellIndex]) {
                            D_80189858 += adjustValue(D_80189858, 50, gGold);
                            overlayScreenStrings.strings2[(overlayScreenStrings.unk_6A)][overlayScreenTable.cellIndex]--;
                            playSfx(2);
                        }
                        
                    }
                    
                    
                    setSpriteViewSpacePosition(0x79, 32.0f, (f32) (-(overlayScreenTable.cellIndex * 16) + 24), 32.0f);
                    
                    set = TRUE;
                    
                }
                
            }

            if (checkButtonRepeat(0, 0x100000)) {

                if (!set) {

                    if (adjustValue(D_80189858, -50, gGold) == -50) {

                        if (overlayScreenStrings.strings2[(overlayScreenStrings.unk_6A)][overlayScreenTable.cellIndex] != 99) {
                            D_80189858 += adjustValue(D_80189858, -50, gGold);
                            overlayScreenStrings.strings2[(overlayScreenStrings.unk_6A)][overlayScreenTable.cellIndex]++;
                            playSfx(2);
                        }
                    }
                    
                    
                    setSpriteViewSpacePosition(0x79, 32.0f, (f32) (-(overlayScreenTable.cellIndex * 16) + 24), 32.0f);
                    set = TRUE;
                        
                }
                    
            }

            if ((checkButtonPressed(CONTROLLER_1, 0x4000 | 0x8000))) {

                if (!set) {
                    overlayScreenTable.subscreen = 4;
                    setSpriteViewSpacePosition(0x79, -112.0f, (f32) (-(overlayScreenTable.cellIndex * 16) + 24), 32.0f);
                    set = TRUE;
                    playSfx(1);
                }
                
            }

            dmaOverlayScreenSprites(1, (u32) D_80189858, 5, 3);
            dmaOverlayScreenSprites(overlayScreenTable.cellIndex + 3, overlayScreenStrings.strings2[(overlayScreenStrings.unk_6A)][overlayScreenTable.cellIndex], 1, 3);
                
            break;

        case 6:

            if (checkButtonRepeat(CONTROLLER_1, 0x100000)) {
                
                if (!set) {
                    overlayScreenTable.cellIndex = 5;
                    playSfx(2);
                    setSpriteViewSpacePosition(0x79, -112.0f, (f32) (-(overlayScreenTable.cellIndex * 16) + 24), 32.0f);
                    set = TRUE;
                    overlayScreenTable.subscreen = 4;
                }
                
            }

            if (checkButtonRepeat(CONTROLLER_1, 0x10000)) {
                
                if (!set) {
                    overlayScreenTable.cellIndex = 0;
                    playSfx(2);
                    setSpriteViewSpacePosition(0x79, -112.0f, (f32) (-(overlayScreenTable.cellIndex * 0x10) + 0x18), 32.0f);
                    set = TRUE;
                    overlayScreenTable.subscreen = 4;
                }
                
            }

            if ((checkButtonPressed(CONTROLLER_1, 0x8000))) {

                if (!set) {
                    
                    if (overlayScreenStrings.strings2[(overlayScreenStrings.unk_6A)][0] 
                        || overlayScreenStrings.strings2[(overlayScreenStrings.unk_6A)][1] 
                        || overlayScreenStrings.strings2[(overlayScreenStrings.unk_6A)][2] 
                        || overlayScreenStrings.strings2[(overlayScreenStrings.unk_6A)][3] 
                        || overlayScreenStrings.strings2[(overlayScreenStrings.unk_6A)][4] 
                        || overlayScreenStrings.strings2[(overlayScreenStrings.unk_6A)][5]) {
                        
                        overlayScreenStrings.unk_6D[overlayScreenStrings.unk_6A] = 0xFF;
                        gGold = D_80189858;
                        
                        if (checkDailyEventBit(0x41)) {
                            acquireKeyItem(0xE);
                        } else {
                            acquireKeyItem(0xF);
                        }
                        
                    }
                    
                    resetAnimationState(0x79);
                    set = TRUE;
                    
                    func_800CA678();
                    overlayScreenTable.subscreen = 2;
                    
                    playSfx(0);
                    
                }
            }

            break;
        
        }
    
    if (overlayScreenTable.subscreen >= 4 && checkButtonPressed(CONTROLLER_1, BUTTON_B) && !set) {
        resetAnimationState(0x79);
        func_800CA678();
        overlayScreenTable.subscreen = 2;
        playSfx(1);
    }
    
}

// horse race
// D_801220CC
INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CB0C4);

// D_80122140
INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CB8F0);

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CC11C);

bool func_800CC11C(u8 arg0, u8 arg1, u8 arg2) {

    bool result = FALSE;
    u8 i;
    
    for (i = 0; i < arg1; i++) {

        if (overlayScreenStrings.strings3[arg0][i] == arg2) {
            result = TRUE;
        }
        
    }
    
    return result;
    
}

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CC17C);

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CC518);

void func_800CC518(void) {

    func_800593EC();
    
    if (checkDailyEventBit(0x41) != 0) {
        removeKeyItem(0xE);
    } else {
        removeKeyItem(0xF);
    }
    
    dmaSprite(0x80, (u32)&_checkerboardBackgroundTextureSegmentRomStart, (u32)&_checkerboardBackgroundTextureSegmentRomEnd, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomStart, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x802DE5C0, NULL, (u16*)0x802E0BC0, (u16*)0x802E0CC0, (u8*)0x802E0DC0, NULL, 0, 0);
    setSpriteViewSpacePosition(0x80, 0.0f, 0.0f, 0.0f);
    setSpriteScale(0x80, 2.0f, 2.0f, 1.0f);
    setBilinearFiltering(0x80, 1);
    setSpriteColor(0x80, 0xFF, 0xFF, 0xFF, 0xFF);
    startSpriteAnimation(0x80, 0, 0);
    
    overlayScreenTable.cellIndex = 0;
    overlayScreenTable.previousCellIndex = 0;
    overlayScreenTable.pageNumber = 0;
    overlayScreenTable.unk_3 = 0xFF;
    overlayScreenTable.subscreen = 0;
    
    setMainLoopCallbackFunctionIndex(0x2F);
    
    overlayScreenStrings.dialogueVariableIndices[0] = D_8018A72C[0] - 1;
    overlayScreenStrings.dialogueVariableIndices[1] = D_8018A72C[1] - 1;
    overlayScreenStrings.dialogueVariableIndices[2] = D_8018A72C[2] - 1;
    overlayScreenStrings.dialogueVariableIndices[3] = D_8018A72C[3] - 1;
    overlayScreenStrings.dialogueVariableIndices[4] = D_8018A72C[4] - 1;
    overlayScreenStrings.dialogueVariableIndices[5] = D_8018A72C[5] - 1;
    
}

INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CC6F0);

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CCFC8);

void func_800CCFC8(void) {

    setGameVariableString(0x2A, overlayScreenStrings.dialogueVariableStrings[overlayScreenStrings.dialogueVariableIndices[0]], 6);
    setGameVariableString(0x2B, overlayScreenStrings.dialogueVariableStrings[overlayScreenStrings.dialogueVariableIndices[1]], 6);
    setGameVariableString(0x2C, overlayScreenStrings.dialogueVariableStrings[overlayScreenStrings.dialogueVariableIndices[2]], 6);
    setGameVariableString(0x2D, overlayScreenStrings.dialogueVariableStrings[overlayScreenStrings.dialogueVariableIndices[3]], 6);
    setGameVariableString(0x2E, overlayScreenStrings.dialogueVariableStrings[overlayScreenStrings.dialogueVariableIndices[4]], 6);
    setGameVariableString(0x2F, overlayScreenStrings.dialogueVariableStrings[overlayScreenStrings.dialogueVariableIndices[5]], 6);
    
    initializeEmptyMessageBox(0, (u8*)0x8030B000);
    setMessageBoxViewSpacePosition(0, -32.0f, 32.0f, 0.0f);
    func_8003F5D0(0, 0xA, 2);
    func_8003F630(0, 0, 2);
    func_8003F464(0, 0xE, 0xE, (u8*)COMPRESSED_FONT_VADDR, (u16*)FONT_PALETTE_1_VADDR);
    func_8003F360(0, -4, 2);
    setMessageBoxSpriteIndices(0, 0xFF, 0, 0);
    setMessageBoxButtonMask(0, BUTTON_A);
    func_8003FB4C(0, 1);
    initializeMessageBox(0, 0, 0x12, 0x80000);
    func_8003E77C(0, 0xFF, 0xFF, 0xFF, 0xFF);
    
    initializeEmptyMessageBox(1, (u8*)0x8030B400);
    setMessageBoxViewSpacePosition(1, -32.0f, 16.0f, 0.0f);
    func_8003F5D0(1, 0xA, 2);
    func_8003F630(1, 0, 2);
    func_8003F464(1, 0xE, 0xE, (u8*)COMPRESSED_FONT_VADDR, (u16*)FONT_PALETTE_1_VADDR);
    func_8003F360(1, -4, 2);
    setMessageBoxSpriteIndices(1, 0xFF, 0, 0);
    setMessageBoxButtonMask(1, BUTTON_A);
    func_8003FB4C(1, 1);
    initializeMessageBox(1, 0, 0x13, 0x80000);
    func_8003E77C(1, 0xFF, 0xFF, 0xFF, 0xFF);
    
    initializeEmptyMessageBox(2, (u8*)0x8030B800);
    setMessageBoxViewSpacePosition(2, -32.0f, 0.0f, 0.0f);
    func_8003F5D0(2, 0xA, 2);
    func_8003F630(2, 0, 2);
    func_8003F464(2, 0xE, 0xE, (u8*)COMPRESSED_FONT_VADDR, (u16*)FONT_PALETTE_1_VADDR);
    func_8003F360(2, -4, 2);
    setMessageBoxSpriteIndices(2, 0xFF, 0, 0);
    setMessageBoxButtonMask(2, BUTTON_A);
    func_8003FB4C(2, 1);
    initializeMessageBox(2, 0, 0x14, 0x80000);
    func_8003E77C(2, 0xFF, 0xFF, 0xFF, 0xFF);
    
    initializeEmptyMessageBox(3, (u8*)0x8030BC00);
    setMessageBoxViewSpacePosition(3, 96.0f, 32.0f, 0.0f);
    func_8003F5D0(3, 0xA, 2);
    func_8003F630(3, 0, 2);
    func_8003F464(3, 0xE, 0xE, (u8*)COMPRESSED_FONT_VADDR, (u16*)FONT_PALETTE_1_VADDR);
    func_8003F360(3, -4, 2);
    setMessageBoxSpriteIndices(3, 0xFF, 0, 0);
    setMessageBoxButtonMask(3, BUTTON_A);
    func_8003FB4C(3, 1);
    initializeMessageBox(3, 0, 0x15, 0x80000);
    func_8003E77C(3, 0xFF, 0xFF, 0xFF, 0xFF);
    
    initializeEmptyMessageBox(4, (u8*)0x8030C000);
    setMessageBoxViewSpacePosition(4, 96.0f, 16.0f, 0.0f);
    func_8003F5D0(4, 0xA, 2);
    func_8003F630(4, 0, 2);
    func_8003F464(4, 0xE, 0xE, (u8*)COMPRESSED_FONT_VADDR, (u16*)FONT_PALETTE_1_VADDR);
    func_8003F360(4, -4, 2);
    setMessageBoxSpriteIndices(4, 0xFF, 0, 0);
    setMessageBoxButtonMask(4, BUTTON_A);
    func_8003FB4C(4, 1);
    initializeMessageBox(4, 0, 0x16, 0x80000);
    func_8003E77C(4, 0xFF, 0xFF, 0xFF, 0xFF);
    
    initializeEmptyMessageBox(5, (u8*)0x8030C400);
    setMessageBoxViewSpacePosition(5, 96.0f, 0.0f, 0.0f);
    func_8003F5D0(5, 0xA, 2);
    func_8003F630(5, 0, 2);
    func_8003F464(5, 0xE, 0xE, (u8*)COMPRESSED_FONT_VADDR, (u16*)FONT_PALETTE_1_VADDR);
    func_8003F360(5, -4, 2);
    setMessageBoxSpriteIndices(5, 0xFF, 0, 0);
    setMessageBoxButtonMask(5, BUTTON_A);
    func_8003FB4C(5, 1);
    initializeMessageBox(5, 0, 0x17, 0x80000);
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

    deactivateOverlayScreenSprites(2);
    deactivateOverlayScreenSprites(3);
    deactivateOverlayScreenSprites(8);
    deactivateOverlayScreenSprites(9);

    func_80053088();

}

INCLUDE_RODATA("asm/nonmatchings/game/overlayScreens", D_801220CC);

INCLUDE_RODATA("asm/nonmatchings/game/overlayScreens", D_80122140);

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CD750);

void func_800CD750(void) {
    
    u16 temp = 0;

    switch (overlayScreenTable.subscreen) {
        
        case 4:
            break;
        
        case 0:
            func_800CC6F0();
            overlayScreenTable.subscreen = 1;
            break;
        
        case 1:
            if (func_8002CBF8(0x82) & 0xFF) {
                overlayScreenTable.subscreen = 4;
            }
            break;
        
        case 2:
            if (func_8002CBF8(0x82)) {
                func_800B2CE0();
                overlayScreenTable.subscreen = 3;
            }
            break;
        
        case 3:
            deactivateSprite(0x80);
            func_80059368();
            func_80059300();
            setDailyEventBit(0x40);
            setEntrance(0x6C);
            setPlayerAction(0, 0);
            setMainLoopCallbackFunctionIndex(2);
            break;
        }
    
        if (overlayScreenTable.subscreen >= 4 && checkButtonPressed(CONTROLLER_1, BUTTON_B) && temp == 0) {
            
            updateSpriteAlpha(0x81, 0, 0x18);
            updateSpriteAlpha(0x82, 0, 0x18);
            updateSpriteAlpha(0x83, 0, 0x18);
            updateSpriteAlpha(0x84, 0, 0x18);
            updateSpriteAlpha(0x85, 0, 0x18);
            func_8003DD14(0);
            func_8003DD14(1);
            func_8003DD14(2);
            func_8003DD14(3);
            func_8003DD14(4);
            func_8003DD14(5);
            deactivateOverlayScreenSprites(2);
            deactivateOverlayScreenSprites(3);
            deactivateOverlayScreenSprites(8);
            deactivateOverlayScreenSprites(9);
            func_80053088();
            
            overlayScreenTable.subscreen = 2;
            
            playSfx(1);
            
        }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CD928);

void func_800CD928(void) {
    
    func_800593EC();
    
    dmaSprite(0x80, (u32)&_checkerboardBackgroundTextureSegmentRomStart, (u32)&_checkerboardBackgroundTextureSegmentRomEnd, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomStart, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x802DE5C0, NULL, (u16*)0x802E0BC0, (u16*)0x802E0CC0, (u8*)0x802E0DC0, NULL, 0, 0);
    setSpriteViewSpacePosition(0x80, 0.0f, 0.0f, 0.0f);
    setSpriteScale(0x80, 2.0f, 2.0f, 1.0f);
    setBilinearFiltering(0x80, 1);
    setSpriteColor(0x80, 0xFF, 0xFF, 0xFF, 0xFF);
    startSpriteAnimation(0x80, 0, 0);
    
    overlayScreenTable.cellIndex = 0;
    overlayScreenTable.previousCellIndex = 0;
    overlayScreenTable.pageNumber = 0;
    overlayScreenTable.unk_3 = 0xFF;
    overlayScreenTable.subscreen = 0;
    
    setMainLoopCallbackFunctionIndex(0x30);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CDA6C);

void func_800CDA6C(void) {

    dmaSprite(0x81, (u32)&_snapshotLargeTextureSegmentRomStart, (u32)&_snapshotLargeTextureSegmentRomEnd, (u32)&_snapshotLargeAssetsIndexSegmentRomStart, (u32)&_snapshotLargeAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x80289900, NULL, (u16*)0x8028EF00, (u16*)0x8028F000, (u8*)0x8028F100, NULL, 0, 0);
    setSpriteViewSpacePosition(0x81, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x81, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x81, 2);
    setBilinearFiltering(0x81, 1);
    startSpriteAnimation(0x81, 0, 0);
    
    dmaSprite(0x82, (u32)&_prizeScreenTextureSegmentRomStart, (u32)&_prizeScreenTextureSegmentRomEnd, (u32)&_prizeScreenAssetsIndexSegmentRomStart, (u32)&_prizeScreenAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
    setSpriteViewSpacePosition(0x82, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x82, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x82, 2);
    setBilinearFiltering(0x82, 1);
    
    if (checkDailyEventBit(0x41) != 0) {
        startSpriteAnimation(0x82, 2, 0);
    } else {
        startSpriteAnimation(0x82, 5, 0);
    }
    
    dmaSprite(0x83, (u32)&_prizeScreenTextureSegmentRomStart, (u32)&_prizeScreenTextureSegmentRomEnd, (u32)&_prizeScreenAssetsIndexSegmentRomStart, (u32)&_prizeScreenAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
    setSpriteViewSpacePosition(0x83, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x83, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x83, 2);
    setBilinearFiltering(0x83, 1);
    startSpriteAnimation(0x83, 4, 0);
    
    dmaSprite(0x84, (u32)&_prizeScreenTextureSegmentRomStart, (u32)&_prizeScreenTextureSegmentRomEnd, (u32)&_prizeScreenAssetsIndexSegmentRomStart, (u32)&_prizeScreenAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
    setSpriteViewSpacePosition(0x84, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x84, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x84, 2);
    setBilinearFiltering(0x84, 1);
    startSpriteAnimation(0x84, 4, 1);
    
    func_80045E20(2, 0x97, (u32)&_moneyTextureSegmentRomStart, (u32)&_moneyTextureSegmentRomEnd, (u32)&_moneyAssetsIndexSegmentRomStart, (u32)&_moneyAssetsIndexSegmentRomEnd, 0x80253B00, 0x80254A00, 0x80254E00, 0x80254F00, 0, 1, 0, -24.0f, 6.0f, 256.0f, 0xA);
    
    dmaOverlayScreenSprites(2, D_80205204, 3, 3);
    setOverlayScreenSpritesRGBA(2, 0xFF, 0xFF, 0xFF, 0xFF);
    
    dmaSprite(0x79, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8023B400, NULL, (u16*)0x8023CC00, (u16*)0x8023CE00, (u8*)0x8023D200, NULL, 0, 0);
    setBilinearFiltering(0x79, 1);
    setSpriteColor(0x79, 0xFF, 0xFF, 0xFF, 0xFF);
    
    func_800CDFB0();

}

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
    deactivateOverlayScreenSprites(2);
}

//INCLUDE_RODATA("asm/nonmatchings/game/overlayScreens", D_801221CC);

static const s16 D_801221CC[6] = { 3000, 1000, 500, 0, 0, 0 };

// D_801221CC
INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CE068);

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CE714);

bool func_800CE714(u8 arg0) {

    bool result = FALSE;
    
    switch (arg0) {
        
        case 0:
            if (checkLifeEventBit(0x56)) {
                result = TRUE;
            }
            break;
        case 1:
            if (checkLifeEventBit(0x58)) {
                result = TRUE;
            }
            break;
        case 2:
            if (checkHaveKeyItem(0x15)) {
                result = TRUE;
            }
            break;
        
    }

    return result;
    
}

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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CE930);

void func_800CE930(void) {
    
    func_800593EC();
    
    dmaSprite(0x80, (u32)&_checkerboardBackgroundTextureSegmentRomStart, (u32)&_checkerboardBackgroundTextureSegmentRomEnd, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomStart, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x802DE5C0, NULL, (u16*)0x802E0BC0, (u16*)0x802E0CC0, (u8*)0x802E0DC0, NULL, 0, 0);
    setSpriteViewSpacePosition(0x80, 0.0f, 0.0f, 0.0f);
    setSpriteScale(0x80, 2.0f, 2.0f, 1.0f);
    setBilinearFiltering(0x80, 1);
    setSpriteColor(0x80, 0xFF, 0xFF, 0xFF, 0xFF);
    startSpriteAnimation(0x80, 0, 0);
    
    overlayScreenTable.cellIndex = 0;
    overlayScreenTable.previousCellIndex = 0;
    overlayScreenTable.pageNumber = 0;
    overlayScreenTable.unk_3 = 0xFF;
    overlayScreenTable.subscreen = 0;
    
    setMainLoopCallbackFunctionIndex(0x31);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CEA74);

void func_800CEA74(void) {
    
    dmaSprite(0x81, (u32)&_snapshotLargeTextureSegmentRomStart, (u32)&_snapshotLargeTextureSegmentRomEnd, (u32)&_snapshotLargeAssetsIndexSegmentRomStart, (u32)&_snapshotLargeAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x80289900, NULL, (u16*)0x8028EF00, (u16*)0x8028F000, (u8*)0x8028F100, NULL, 0, 0);
    setSpriteViewSpacePosition(0x81, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x81, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x81, 2);
    setBilinearFiltering(0x81, 1);
    startSpriteAnimation(0x81, 0, 0);
    
    dmaSprite(0x82, (u32)&_prizeScreenTextureSegmentRomStart, (u32)&_prizeScreenTextureSegmentRomEnd, (u32)&_prizeScreenAssetsIndexSegmentRomStart, (u32)&_prizeScreenAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
    setSpriteViewSpacePosition(0x82, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x82, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x82, 2);
    setBilinearFiltering(0x82, 1);
    startSpriteAnimation(0x82, 3, 0);
    
    dmaSprite(0x83, (u32)&_prizeScreenTextureSegmentRomStart, (u32)&_prizeScreenTextureSegmentRomEnd, (u32)&_prizeScreenAssetsIndexSegmentRomStart, (u32)&_prizeScreenAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
    setSpriteViewSpacePosition(0x83, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x83, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x83, 2);
    setBilinearFiltering(0x83, 1);
    
    if (gBaseMapIndex == FLOWER_SHOP) {
        startSpriteAnimation(0x83, 0, 0);
    } else {
        startSpriteAnimation(0x83, 1, 0);
    }
    
    func_800CED60();
    
}

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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CEDF0);

void func_800CEDF0(void) {

    switch (overlayScreenTable.subscreen) {

        case 0:
            func_800CEA74();
            overlayScreenTable.subscreen = 1;
            break;
        
        case 1:
            
            if (func_8002CBF8(0x82)) {
                
                setMessageBoxViewSpacePosition(0, 24.0f, -64.0f, 352.0f);
                setMessageBoxSpriteIndices(0, 0, 0, 0);
                func_8003F360(0, -4, 0);
                
                if (gBaseMapIndex == 0x32) {
                    initializeDialogueSession(0, 0x3E, 4, 0x40);
                } else {
                    initializeDialogueSession(0, 0x3E, 0xA, 0x40);
                }
                
                overlayScreenTable.subscreen = 4;

            }
            
            break;
        
        case 4:

            if (func_80043A88()) {
                
                if (!(func_80043C6C(0))) {
                    
                    dialogues[0].sessionManager.flags &= ~0x40;
                    
                    func_80043AD8(0);
                    overlayScreenTable.subscreen = 6;
                    
                    playSfx(0x20);
                    
                    if (gBaseMapIndex == 0x32) {
                        flowerShopPoints += adjustValue(flowerShopPoints, -10, 100);
                    } else {
                        bakeryCardPoints += adjustValue(bakeryCardPoints, -10, 100);
                    }
                    

                } else {
                    
                    if (gBaseMapIndex == 0x32) {
                        initializeMessageBox(0, 4, 0x3D, 0);
                    } else {
                        initializeMessageBox(0, 4, 0x45, 0);
                    }
                
                    overlayScreenTable.subscreen = 3;

                }
                
            
            }
            
            break;
        
        case 3:

            // check if a message box has flag 4 set
            if (func_8003F0DC()) {
                
                func_8003F130(0);
                
                resetAnimationState(0x79);
                
                updateSpriteAlpha(0x81, 0, 0x18);
                updateSpriteAlpha(0x82, 0, 0x18);
                updateSpriteAlpha(0x83, 0, 0x18);
                
                overlayScreenTable.subscreen = 2;

            }
            
            break;
        
        case 2:
        
            if (func_8002CBF8(0x82)) {
                func_800B2CE0();
                overlayScreenTable.subscreen = 5;
            }

            break;
        
        case 5:
            deactivateSprite(0x80);
            func_80059368();
            func_80059300();

            break;
        
        case 6:
            
            if (gAudioSequences[1].flags == 0) {

                if (gBaseMapIndex == FLOWER_SHOP) {

                    switch (getRandomNumberInRange(0, 4)) {

                        case 0:     
                            
                            if (!(func_800CF34C(0))) {
                                initializeMessageBox(0, 4, 0x38, 0);
                                overlayScreenTable.subscreen = 3;
                                func_800CF3C4(0);
                            } else {
                                initializeMessageBox(0, 4, 0x3C, 0);
                                overlayScreenTable.subscreen = 3;
                                func_800CF3C4(4);
                            }
                            
                            break;
                        
                    case 1:               
                        
                        if (!(func_800CF34C(1))) {
                            initializeMessageBox(0, 4, 0x39, 0);
                            overlayScreenTable.subscreen = 3;
                            func_800CF3C4(1);
                        } else {
                            initializeMessageBox(0, 4, 0x3C, 0);
                            overlayScreenTable.subscreen = 3;
                            func_800CF3C4(4);
                        }
                        
                        break;
                        
                    case 2:                             
                        
                        if (!(func_800CF34C(2))) {
                            initializeMessageBox(0, 4, 0x3A, 0);
                            overlayScreenTable.subscreen = 3;
                            func_800CF3C4(2);
                        } else {
                            initializeMessageBox(0, 4, 0x3C, 0);
                            overlayScreenTable.subscreen = 3;
                            func_800CF3C4(4);
                        }
                        
                        break;
                        
                    case 3: 
                        
                        if (!(func_800CF34C(3))) {
                            initializeMessageBox(0, 4, 0x3B, 0);
                            overlayScreenTable.subscreen = 3;
                            func_800CF3C4(3);
                        } else {
                            initializeMessageBox(0, 4, 0x3C, 0);
                            overlayScreenTable.subscreen = 3;
                            func_800CF3C4(4);
                        }
                        
                        break;

                    case 4:   
                        initializeMessageBox(0, 4, 0x3C, 0);
                        overlayScreenTable.subscreen = 3;
                        func_800CF3C4(4);
                        
                        break;                        
                    
                    }
                    
                } else {
                    
                    switch (getRandomNumberInRange(0, 4)) {      
                        
                        case 0:                             
                            
                            if (!(func_800CF498(0))) {
                                initializeMessageBox(0, 4, 0x40, 0);
                                overlayScreenTable.subscreen = 3;
                                func_800CF544(0);
                            } else {
                                initializeMessageBox(0, 4, 0x44, 0);
                                overlayScreenTable.subscreen = 3;
                                func_800CF544(4);
                            }
                            
                            break;
                        
                        case 1:                             
                            
                            if (!(func_800CF498(1))) {
                                initializeMessageBox(0, 4, 0x41, 0);
                                overlayScreenTable.subscreen = 3;
                                func_800CF544(1);
                            } else {
                                initializeMessageBox(0, 4, 0x44, 0);
                                overlayScreenTable.subscreen = 3;
                                func_800CF544(4);
                            }
                        
                            break;
                        
                        case 2:                             
                            
                            if (!(func_800CF498(2))) {
                                initializeMessageBox(0, 4, 0x42, 0);
                                overlayScreenTable.subscreen = 3;
                                func_800CF544(2);
                            } else {
                                initializeMessageBox(0, 4, 0x44, 0);
                                overlayScreenTable.subscreen = 3;
                                func_800CF544(4);
                            }
                            
                            break;
                        
                        case 3:            
                            
                            if (!(func_800CF498(3))) {
                                initializeMessageBox(0, 4, 0x43, 0);
                                overlayScreenTable.subscreen = 3;
                                func_800CF544(3);
                            } else {
                                initializeMessageBox(0, 4, 0x44, 0);
                                overlayScreenTable.subscreen = 3;
                                func_800CF544(4);
                            }
                        
                            break;
                        
                        case 4:                             
                            initializeMessageBox(0, 4, 0x44, 0);
                            overlayScreenTable.subscreen = 3;
                            func_800CF544(4);

                            break;
                            
                        }
                }
                
            }
            
            break;
        
        default:
            break;
   
    }
    
}

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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CF544);

void func_800CF544(u8 arg0) {

    switch (arg0) {
        
        case 0:
            
            setLifeEventBit(0x17);
            
            if (checkLifeEventBit(MARRIED) && gWife == MARIA) {
                npcAffection[MARIA] += adjustValue(npcAffection[MARIA], 10, MAX_AFFECTION);
            }
            if (checkLifeEventBit(MARRIED) && gWife == POPURI) {
                npcAffection[POPURI] += adjustValue(npcAffection[POPURI], 10, MAX_AFFECTION);
            }
            if (checkLifeEventBit(MARRIED) && gWife == ELLI) {
                npcAffection[ELLI] += adjustValue(npcAffection[ELLI], 10, MAX_AFFECTION);
            }
            if (checkLifeEventBit(MARRIED) && gWife == ANN) {
                npcAffection[ANN] += adjustValue(npcAffection[ANN], 10, MAX_AFFECTION);
            }
            if (checkLifeEventBit(MARRIED) && gWife == KAREN) {
                npcAffection[4] += adjustValue(npcAffection[4], 10, MAX_AFFECTION);
            }
            
            break;

        case 1:
            
            setLifeEventBit(0x5C);
            
            if (checkLifeEventBit(MARRIED) && gWife == MARIA) {
                npcAffection[MARIA] += adjustValue(npcAffection[MARIA], 8, MAX_AFFECTION);
            }
            if (checkLifeEventBit(MARRIED) && gWife == POPURI) {
                npcAffection[POPURI] += adjustValue(npcAffection[POPURI], 8, MAX_AFFECTION);
            }
            if (checkLifeEventBit(MARRIED) && gWife == ELLI) {
                npcAffection[ELLI] += adjustValue(npcAffection[ELLI], 8, MAX_AFFECTION);
            }
            if (checkLifeEventBit(MARRIED) && gWife == ANN) {
                npcAffection[ANN] += adjustValue(npcAffection[ANN], 8, MAX_AFFECTION);
            }
            if (checkLifeEventBit(MARRIED) && gWife == KAREN) {
                npcAffection[4] += adjustValue(npcAffection[4], 8, MAX_AFFECTION);
            }
            
            break;

        case 2:
            setLifeEventBit(0x5D);
            break;
        
        case 3:
            
            if (checkHaveTool(EMPTY_BOTTLE)) {
                gPlayer.bottleContents = 7;
            } else {
                gPlayer.heldItem = COOKIES;
            }
            break;
        
        case 4:
            gPlayer.heldItem = COOKIES;
            break;
    
    }
    
}