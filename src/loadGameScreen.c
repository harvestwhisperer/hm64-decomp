#include "common.h"

#include "loadGameScreen.h"

#include "system/message.h"

#include "game.h"
#include "mainLoop.h"
#include "overlayScreens.h"
#include "player.h"
#include "updateGame.h"
                                                                         
// forward declaration
u8 func_800E4424(u8, u8);                 
 
// bss
u32 D_801654F4;
extern LoadGameScreenInfo D_801D6238;
extern u8 D_8030E000[16];

// struct
extern u8 D_801D624C;
extern u8 D_801D6261;
extern u8 D_801D62B8;
extern u8 D_801D62B9;
extern u8 D_801D62BA;
extern u8 D_801D62BB;
extern u8 D_801D62BF;
extern u8 D_801D62BC;
extern u8 D_801D62BF;

// shared
extern u8 gGlobalSeasonName[6];

//INCLUDE_ASM(const s32, "loadGameScreen", func_800E1380);

void func_800E1380(u8 controllerPakEnabled) {
    
    if (!controllerPakEnabled) {
        D_801D6238.showControllerPakScreen = 0;
        D_801D6238.actionColumnHighlighted = 0;
        D_801D6238.action = 0;
        D_801D6238.diaryHighlighted = 0;
    } else {
        D_801D6238.action = 11;
    }
    
    D_801D6238.unk_2A[0] = 0x45;
    D_801D6238.unk_2A[1] = 0x39;
    D_801D6238.unk_2A[2] = 0x4E;
    D_801D6238.unk_2A[3] = 0x59;
    D_801D6238.unk_2A[4] = 0x57;
    D_801D6238.unk_2A[5] = 0x45;
    
    D_801D6238.unk_0[0] = 0x48;
    D_801D6238.unk_0[1] = 0x41;
    D_801D6238.unk_0[2] = 0x52;
    D_801D6238.unk_0[3] = 0x56;
    D_801D6238.unk_0[4] = 0x45;
    D_801D6238.unk_0[5] = 0x53;
    D_801D6238.unk_0[6] = 0x54;
    D_801D6238.unk_0[7] = 0x4D;
    D_801D6238.unk_0[8] = 0x4F;
    D_801D6238.unk_0[9] = 0x4F;
    D_801D6238.unk_0[10] = 0x4E;
    D_801D6238.unk_0[11] = 0;
    D_801D6238.unk_0[12] = 0;
    D_801D6238.unk_0[13] = 0;
    D_801D6238.unk_0[14] = 0;
    D_801D6238.unk_0[15] = 0;
    D_801D6238.unk_0[16] = 0x41;
    D_801D6238.unk_0[17] = 0;
    D_801D6238.unk_0[18] = 0;
    D_801D6238.unk_0[19] = 0;
    
    D_801D6238.unk_15[0] = 0x48;
    D_801D6238.unk_15[1] = 0x41;
    D_801D6238.unk_15[2] = 0x52;
    D_801D6238.unk_15[3] = 0x56;
    D_801D6238.unk_15[4] = 0x45;
    D_801D6238.unk_15[5] = 0x53;
    D_801D6238.unk_15[6] = 0x54;
    D_801D6238.unk_15[7] = 0x4D;
    D_801D6238.unk_15[8] = 0x4F;
    D_801D6238.unk_15[9] = 0x4F;
    D_801D6238.unk_15[10] = 0x4E;
    D_801D6238.unk_15[11] = 0;
    D_801D6238.unk_15[12] = 0;
    D_801D6238.unk_15[13] = 0;
    D_801D6238.unk_15[14] = 0;
    D_801D6238.unk_15[15] = 0;
    D_801D6238.unk_15[16] = 0x42;
    D_801D6238.unk_15[17] = 0;
    D_801D6238.unk_15[18] = 0;
    D_801D6238.unk_15[19] = 0;
    
    func_8003FBA0(0, D_801D6238.playerNames[0], 6);
    func_8003FBA0(1, D_801D6238.dateNumbers[0], 2);
    func_8003FBA0(2, D_801D6238.seasonNames[0], 6);
    func_8003FBA0(3, D_801D6238.dateEndings[0], 2);
    
    func_8003FBA0(4, D_801D6238.playerNames[1], 6);
    func_8003FBA0(5, D_801D6238.dateNumbers[1], 2);
    func_8003FBA0(6, D_801D6238.seasonNames[1], 6);
    func_8003FBA0(7, D_801D6238.dateEndings[1], 2);
    
    func_8003FBA0(8, D_801D6238.playerNames[2], 6);
    func_8003FBA0(9, D_801D6238.dateNumbers[2], 2);
    func_8003FBA0(10, D_801D6238.seasonNames[2], 6);
    func_8003FBA0(11, D_801D6238.dateEndings[2], 2);
    
    func_8003FBA0(0xC, D_801D6238.playerNames[3], 6);
    func_8003FBA0(0xD, D_801D6238.dateNumbers[3], 2);
    func_8003FBA0(0xE, D_801D6238.seasonNames[3], 6);
    func_8003FBA0(0xF, D_801D6238.dateEndings[3], 2);
    
    func_800E16D0(1, D_801D6238.diaryHighlighted);
    
    setMainLoopCallbackFunctionIndex(SELECT_GAME);
}

INCLUDE_ASM(const s32, "loadGameScreen", func_800E16A0);

//INCLUDE_ASM(const s32, "loadGameScreen", func_800E16D0);

void func_800E16D0(u8 arg0, u8 arg1) {

    D_801D62BC = arg1;
    
    func_800E1998();
    func_800B42E0();
    
    if (arg0) {
        func_800B59AC();
    } else {
        func_800B5BD4();
    }
    
    if (D_801D62BF == 0) {
        
        if (arg0) {
            func_8003E77C(0, 0, 0, 0, 0);
            func_8003E77C(1, 0, 0, 0, 0);
            func_8003E77C(2, 0, 0, 0, 0);
            func_8003E77C(3, 0, 0, 0, 0);
            func_8003EA1C(0, 0xFF, 0xFF, 0xFF, 0xFF, 0x18);
            func_8003EA1C(1, 0xFF, 0xFF, 0xFF, 0xFF, 0x18);
            func_8003EA1C(2, 0xFF, 0xFF, 0xFF, 0xFF, 0x18);
            func_8003EA1C(3, 0xFF, 0xFF, 0xFF, 0xFF, 0x18);
            return;
        }
        
        func_8003E77C(0, 0, 0, 0, 0);
        func_8003E77C(1, 0, 0, 0, 0);
        func_8003E77C(2, 0, 0, 0, 0);
        func_8003E77C(3, 0, 0, 0, 0);
        func_8003E77C(0, 0xFF, 0xFF, 0xFF, 0xFF);
        func_8003E77C(1, 0xFF, 0xFF, 0xFF, 0xFF);
        func_8003E77C(2, 0xFF, 0xFF, 0xFF, 0xFF);
        func_8003E77C(3, 0xFF, 0xFF, 0xFF, 0xFF);
        return;
    }
    
    if (arg0) {
        func_8003E77C(0, 0, 0, 0, 0);
        func_8003E77C(1, 0, 0, 0, 0);
        func_8003EA1C(0, 0xFF, 0xFF, 0xFF, 0xFF, 0x18);
        func_8003EA1C(1, 0xFF, 0xFF, 0xFF, 0xFF, 0x18);
        return;
    }
    
    func_8003E77C(0, 0, 0, 0, 0);
    func_8003E77C(1, 0, 0, 0, 0);
    func_8003E77C(0, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8003E77C(1, 0xFF, 0xFF, 0xFF, 0xFF);
}

//INCLUDE_ASM(const s32, "loadGameScreen", func_800E1998);

void func_800E1998(void) {

    func_800E16A0();
    
    if (D_801D62BF == 0) {

        func_800E3358(0, 0, 0);
        func_800E3358(1, 0, 0);
        func_800E3358(2, 0, 0);
        func_800E3358(3, 0, 0);

    } else {

        if (D_801D624C & 2) {
            func_800E3358(0, 1, 0);
        } else if (D_801D624C & 1) {
            func_800E3358(0, 1, 1);
        } else {
            func_800E3358(0, 1, 2);
        }
        
        if (D_801D6261 & 2) {
            func_800E3358(1, 1, 0);
        } else {
            if (D_801D6261 & 1) {
                func_800E3358(1, 1, 1);
            } else {
                func_800E3358(1, 1, 2);
            }
        }
    }
    
    func_800E1A94();
}

//INCLUDE_ASM(const s32, "loadGameScreen", func_800E1A94);

void func_800E1A94(void) {

    if (D_801D62BF == 0) {
        if (D_801D62B8 & 1) {
            func_8003DD14(0);
            func_8003DBE8(0, (void*)0x8030B000);
            func_8003F54C(0, 16.0f, 52.0f, 0);
            func_8003F5D0(0, 0xC, 2);
            func_8003F630(0, 0, 2);
            // 0x802FF000 = 
            // 0x8030A000 = font palette vaddr
            func_8003F464(0, 0xE, 0xE, (void*)0x802FF000, (void*)0x8030A000);
            func_8003F360(0, ~(1 | 2), 2);
            func_8003F690(0, 0xFF, 0, 0);
            func_8003FAF8(0, 0x8000);
            func_8003FB4C(0, 1);
            func_8003DDF8(0, 0, 1, 0x80000);
        }
        if (D_801D62B9 & 1) {
            func_8003DD14(1);
            func_8003DBE8(1, (void*)0x8030B400);
            func_8003F54C(1, 16.0f, 8.0f, 0);
            func_8003F5D0(1, 0xC, 2);
            func_8003F630(1, 0, 2);
            func_8003F464(1, 0xE, 0xE, (void*)0x802FF000, (void*)0x8030A000);
            func_8003F360(1, -4, 2);
            func_8003F690(1, 0xFF, 0, 0);
            func_8003FAF8(1, 0x8000);
            func_8003FB4C(1, 1);
            func_8003DDF8(1, 0, 2, 0x80000);
        }
        if (D_801D62BA & 1) {
            func_8003DD14(2);
            func_8003DBE8(2, (void*)0x8030B800);
            func_8003F54C(2, 16.0f, -36.0f, 0);
            func_8003F5D0(2, 0xC, 2);
            func_8003F630(2, 0, 2);
            func_8003F464(2, 0xE, 0xE, (void*)0x802FF000, (void*)0x8030A000);
            func_8003F360(2, -4, 2);
            func_8003F690(2, 0xFF, 0, 0);
            func_8003FAF8(2, 0x8000);
            func_8003FB4C(2, 1);
            func_8003DDF8(2, 0, 3, 0x80000);
        }
        if (D_801D62BB & 1) {
            func_8003DD14(3);
            func_8003DBE8(3, (void*)0x8030BC00);
            func_8003F54C(3, 16.0f, -80.0f, 0);
            func_8003F5D0(3, 0xC, 2);
            func_8003F630(3, 0, 2);
            func_8003F464(3, 0xE, 0xE, (void*)0x802FF000, (void*)0x8030A000);
            func_8003F360(3, -4, 2);
            func_8003F690(3, 0xFF, 0, 0);
            func_8003FAF8(3, 0x8000);
            func_8003FB4C(3, 1);
            func_8003DDF8(3, 0, 4, 0x80000);
        }
    } else {
        if (D_801D62B8 & 1) {
            func_8003DD14(0);
            func_8003DBE8(0, (void*)0x8030B000);
            func_8003F54C(0, 16.0f, 52.0f, 0);
            func_8003F5D0(0, 0xC, 2);
            func_8003F630(0, 0, 2);
            func_8003F464(0, 0xE, 0xE, (void*)0x802FF000, (void*)0x8030A000);
            func_8003F360(0, ~(1 | 2), 2);
            func_8003F690(0, 0xFF, 0, 0);
            func_8003FAF8(0, 0x8000);
            func_8003FB4C(0, 1);
            func_8003DDF8(0, 0, 1, 0x80000);
        }
        if (D_801D62B9 & 1) {
            func_8003DD14(1);
            func_8003DBE8(1, (void*)0x8030B400);
            func_8003F54C(1, 16.0f, 8.0f, 0);
            func_8003F5D0(1, 0xC, 2);
            func_8003F630(1, 0, 2);
            func_8003F464(1, 0xE, 0xE, (void*)0x802FF000, (void*)0x8030A000);
            func_8003F360(1, -4, 2);
            func_8003F690(1, 0xFF, 0, 0);
            func_8003FAF8(1, 0x8000);
            func_8003FB4C(1, 1);
            func_8003DDF8(1, 0, 2, 0x80000);
        }
    }
}

INCLUDE_ASM(const s32, "loadGameScreen", func_800E1FAC);

INCLUDE_ASM(const s32, "loadGameScreen", func_800E3300);

//INCLUDE_ASM(const s32, "loadGameScreen", func_800E3358);

void func_800E3358(u8 arg0, u8 arg1, u8 arg2) {

    switch (arg2) {
        case 0:
            if (func_800E4424(arg0, arg1)) {
            
                setGlobalSeasonName(gSeason);
                
                D_801D6238.seasonNames[arg0][0] = gGlobalSeasonName[0];
                D_801D6238.seasonNames[arg0][1] = gGlobalSeasonName[1];
                D_801D6238.seasonNames[arg0][2] = gGlobalSeasonName[2];
                D_801D6238.seasonNames[arg0][3] = gGlobalSeasonName[3];
                D_801D6238.seasonNames[arg0][4] = gGlobalSeasonName[4];
                D_801D6238.seasonNames[arg0][5] = gGlobalSeasonName[5];
        
                switch (gYear) {
                    case 1:
                        D_801D6238.dateEndings[arg0][0] = 0xCD;
                        D_801D6238.dateEndings[arg0][1] = 0xCE;
                        break;
                    case 2:
                        D_801D6238.dateEndings[arg0][0] = 0xC8;
                        D_801D6238.dateEndings[arg0][1] = 0xBE;
                        break;
                    case 3:
                        D_801D6238.dateEndings[arg0][0] = 0xCC;
                        D_801D6238.dateEndings[arg0][1] = 0xBE;
                        break;
                    default:
                        D_801D6238.dateEndings[arg0][0] = 0xCE;
                        D_801D6238.dateEndings[arg0][1] = 0xC2;
                        break;
                }

                switch (arg0) {
                    case 0:
                        func_8003FBD8(1, gYear, 1);
                        break;                  
                    case 1:
                        func_8003FBD8(5, gYear, 1);
                        break;
                    case 2:
                        func_8003FBD8(9, gYear, 1);
                        break;
                    case 3:
                        func_8003FBD8(13, gYear, 1);
                        break;                  
                    default:
                        break;
                }
                
                D_801D6238.playerNames[arg0][0] = gPlayer.name[0];
                D_801D6238.playerNames[arg0][1] = gPlayer.name[1];
                D_801D6238.playerNames[arg0][2] = gPlayer.name[2];
                D_801D6238.playerNames[arg0][3] = gPlayer.name[3];
                D_801D6238.playerNames[arg0][4] = gPlayer.name[4];
                D_801D6238.playerNames[arg0][5] = gPlayer.name[5];
    
                D_801D6238.fontLoadingFlags[arg0] = 3;

            } else {
                
                D_801D6238.playerNames[arg0][0] = 0xF6;
                D_801D6238.playerNames[arg0][1] = 0xF6;
                D_801D6238.playerNames[arg0][2] = 0xF6;
                D_801D6238.playerNames[arg0][3] = 0xF6;
                D_801D6238.playerNames[arg0][4] = 0xF6;
                D_801D6238.playerNames[arg0][5] = 0xF6;
                
                D_801D6238.dateNumbers[arg0][0] = 0xF6;
                D_801D6238.dateNumbers[arg0][1] = 0xF6;

                D_801D6238.seasonNames[arg0][0] = 0xF6;
                D_801D6238.seasonNames[arg0][1] = 0xF6;
                D_801D6238.seasonNames[arg0][2] = 0xF6;
                D_801D6238.seasonNames[arg0][3] = 0xF6;
                D_801D6238.seasonNames[arg0][4] = 0xF6;
                D_801D6238.seasonNames[arg0][5] = 0xF6;
                    
                D_801D6238.dateEndings[arg0][0] = 0xF6;
                D_801D6238.dateEndings[arg0][1] = 0xF6;     
                
                D_801D6238.fontLoadingFlags[arg0] = 1;
            }

            break;
        
        case 1:
                   
            D_801D6238.playerNames[arg0][0] = 0xF6;
            D_801D6238.playerNames[arg0][1] = 0xF6;
            D_801D6238.playerNames[arg0][2] = 0xF6;
            D_801D6238.playerNames[arg0][3] = 0xF6;
            D_801D6238.playerNames[arg0][4] = 0xF6;
            D_801D6238.playerNames[arg0][5] = 0xF6;
            
            D_801D6238.dateNumbers[arg0][0] = 0xF6;
            D_801D6238.dateNumbers[arg0][1] = 0xF6;
            
            D_801D6238.seasonNames[arg0][0] = 0xF6;
            D_801D6238.seasonNames[arg0][1] = 0xF6;
            D_801D6238.seasonNames[arg0][2] = 0xF6;
            D_801D6238.seasonNames[arg0][3] = 0xF6;
            D_801D6238.seasonNames[arg0][4] = 0xF6;
            D_801D6238.seasonNames[arg0][5] = 0xF6;
            
            D_801D6238.dateEndings[arg0][0] = 0xF6;
            D_801D6238.dateEndings[arg0][1] = 0xF6;
            
            D_801D6238.fontLoadingFlags[arg0] = 1;
            
            break;
        
        case 2:
            
            D_801D6238.playerNames[arg0][0] = 0xEE;
            D_801D6238.playerNames[arg0][1] = 0xEE;
            D_801D6238.playerNames[arg0][2] = 0xEE;
            D_801D6238.playerNames[arg0][3] = 0xEE;
            D_801D6238.playerNames[arg0][4] = 0xEE;
            D_801D6238.playerNames[arg0][5] = 0xEE;
            
            D_801D6238.dateNumbers[arg0][0] = 0xEE;
            D_801D6238.dateNumbers[arg0][1] = 0xEE;

            D_801D6238.seasonNames[arg0][0] = 0xEE;
            D_801D6238.seasonNames[arg0][1] = 0xEE;
            D_801D6238.seasonNames[arg0][2] = 0xEE;
            D_801D6238.seasonNames[arg0][3] = 0xEE;
            D_801D6238.seasonNames[arg0][4] = 0xEE;
            D_801D6238.seasonNames[arg0][5] = 0xEE;
            
            D_801D6238.dateEndings[arg0][0] = 0xEE;
            D_801D6238.dateEndings[arg0][1] = 0xEE;
            
            D_801D6238.fontLoadingFlags[arg0] = 0;

            break;       
    }
}

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
