#include "common.h"

#include "game/loadGameScreen.h"

#include "system/controller.h"
#include "system/globalSprites.h"
#include "system/message.h"
#include "system/pauseScreen.h"

#include "game/animals.h"
#include "game/game.h"
#include "game/gameStatus.h"
#include "game/itemHandlers.h"
#include "game/npc.h"
#include "game/overlayScreens.h"
#include "game/player.h"
#include "game/updateGame.h"

#include "mainLoop.h"
                                                                         
// foward declarations
u8 func_800E4424(u8, u8);                 
void func_800E16A0(void);
void func_800E16D0(u8 arg0, u8 arg1);
void func_800E1998(void);
void func_800E1A94(void);
void func_800E3358(u8 arg0, u8 arg1, u8 arg2);
void func_800E6C08(u8);
void func_800E6FB4(u8);                 
void func_800E80AC(u8);                
void func_800E9550(u8);                  
void func_800E9B2C(u8);          
void func_800EA3AC(u8, u8);                              
void func_800EAA9C(u8);
void func_800EB74C(u8 arg0);               
void func_800EB788(u8);                                 
void func_800EBA90(u8 arg0);
void func_800EBAC8(u8 arg0);                       
void func_800EBEAC(u8, u8);

void func_800EBC00();                                  
u8 func_800EBCD8();      
 
// bss
extern LoadGameScreenContext loadGameScreenContext;

extern u32 D_8018A080;
extern u32 D_8018A728;
extern u32 D_801D6F2C;

// evaluation screen struct
extern u8 D_801FB963[];
// strings
extern u8 D_801FB708[5][6];
extern u8 D_801FB726[6];
extern u8 D_801FB750[6];
extern u8 D_801FB77A[6];
extern u8 D_801FB7A4[6];
extern u8 D_801FB7CE[6];
extern u8 D_801FB7F8[6];
extern u8 D_801FB809;
extern u8 D_801FB810;
extern u8 D_801FB817;
extern u8 D_801FB81E;
extern u8 D_801FB825;
extern u8 D_801FB82C;
extern u8 D_801FB833;
extern u8 D_801FB83A;
extern u8 D_801FB841;
extern u8 D_801FB848;
extern u8 D_801FB84F;
extern u8 D_801FB856;
extern u8 D_801FB85D;
extern u8 D_801FB864;
extern u8 D_801FB86B;
extern u8 D_801FB872;
extern u8 D_801FB89C;
extern u8 D_801FB89D;
extern u8 D_801FB89E;
extern u8 D_801FB89F;
extern u8 D_801FB8A0;
extern u8 D_801FB8A1;
extern u8 D_801FB8A2;
extern u8 D_801FB8A3;
extern u8 D_801FB8B1;
extern u32 D_801FB8C8;
extern u32 D_801FB8E4;
extern u32 D_801FB900;
extern u32 D_801FB91C;
extern u32 D_801FB938;
extern u8 D_801FB953;
extern u8 D_801FB96F;
extern u8 D_801FB976;
extern u16 D_801FB940[];
extern u8 D_801FB955[];
extern u8 D_801FB95C[];
extern f32 D_801FB978[5];
extern u8 D_801FB994[];
extern u8 D_801FB999[];
extern u8 D_801FB99B[];
extern u8 D_801FB9A0;

extern u32 D_80204DF0;
extern u32 D_802226E4;

// shared
// turnips shipped
extern u32 D_801654F4;
extern u8 gGlobalSeasonName[6];

// buffer bss
// sram memory
extern u8 D_8030D000[16];
extern u8 D_8030E000[16];

// data
// "PACKINSOFT FARM2" in ASCII
u8 D_80119740[16] = { 0x50, 0x41, 0x43, 0x4B, 0x49, 0x4E, 0x53, 0x4F, 0x46, 0x54, 0x20, 0x46, 0x41, 0x52, 0x4D, 0x32 };

//INCLUDE_ASM("asm/nonmatchings/game/loadGameScreen", func_800E1380);

void func_800E1380(u8 controllerPakEnabled) {
    
    if (!controllerPakEnabled) {
        loadGameScreenContext.showControllerPakScreen = 0;
        loadGameScreenContext.actionColumnHighlighted = 0;
        loadGameScreenContext.action = 0;
        loadGameScreenContext.diaryHighlighted = 0;
    } else {
        loadGameScreenContext.action = 11;
    }
    
    loadGameScreenContext.unk_2A[0] = 0x45;
    loadGameScreenContext.unk_2A[1] = 0x39;
    loadGameScreenContext.unk_2A[2] = 0x4E;
    loadGameScreenContext.unk_2A[3] = 0x59;
    loadGameScreenContext.unk_2A[4] = 0x57;
    loadGameScreenContext.unk_2A[5] = 0x45;
    
    loadGameScreenContext.unk_0[0] = 0x48;
    loadGameScreenContext.unk_0[1] = 0x41;
    loadGameScreenContext.unk_0[2] = 0x52;
    loadGameScreenContext.unk_0[3] = 0x56;
    loadGameScreenContext.unk_0[4] = 0x45;
    loadGameScreenContext.unk_0[5] = 0x53;
    loadGameScreenContext.unk_0[6] = 0x54;
    loadGameScreenContext.unk_0[7] = 0x4D;
    loadGameScreenContext.unk_0[8] = 0x4F;
    loadGameScreenContext.unk_0[9] = 0x4F;
    loadGameScreenContext.unk_0[10] = 0x4E;
    loadGameScreenContext.unk_0[11] = 0;
    loadGameScreenContext.unk_0[12] = 0;
    loadGameScreenContext.unk_0[13] = 0;
    loadGameScreenContext.unk_0[14] = 0;
    loadGameScreenContext.unk_0[15] = 0;
    loadGameScreenContext.unk_0[16] = 0x41;
    loadGameScreenContext.unk_0[17] = 0;
    loadGameScreenContext.unk_0[18] = 0;
    loadGameScreenContext.unk_0[19] = 0;
    
    loadGameScreenContext.unk_15[0] = 0x48;
    loadGameScreenContext.unk_15[1] = 0x41;
    loadGameScreenContext.unk_15[2] = 0x52;
    loadGameScreenContext.unk_15[3] = 0x56;
    loadGameScreenContext.unk_15[4] = 0x45;
    loadGameScreenContext.unk_15[5] = 0x53;
    loadGameScreenContext.unk_15[6] = 0x54;
    loadGameScreenContext.unk_15[7] = 0x4D;
    loadGameScreenContext.unk_15[8] = 0x4F;
    loadGameScreenContext.unk_15[9] = 0x4F;
    loadGameScreenContext.unk_15[10] = 0x4E;
    loadGameScreenContext.unk_15[11] = 0;
    loadGameScreenContext.unk_15[12] = 0;
    loadGameScreenContext.unk_15[13] = 0;
    loadGameScreenContext.unk_15[14] = 0;
    loadGameScreenContext.unk_15[15] = 0;
    loadGameScreenContext.unk_15[16] = 0x42;
    loadGameScreenContext.unk_15[17] = 0;
    loadGameScreenContext.unk_15[18] = 0;
    loadGameScreenContext.unk_15[19] = 0;
    
    setGameVariableString(0, loadGameScreenContext.playerNames[0], 6);
    setGameVariableString(1, loadGameScreenContext.dateNumbers[0], 2);
    setGameVariableString(2, loadGameScreenContext.seasonNames[0], 6);
    setGameVariableString(3, loadGameScreenContext.dateEndings[0], 2);
    
    setGameVariableString(4, loadGameScreenContext.playerNames[1], 6);
    setGameVariableString(5, loadGameScreenContext.dateNumbers[1], 2);
    setGameVariableString(6, loadGameScreenContext.seasonNames[1], 6);
    setGameVariableString(7, loadGameScreenContext.dateEndings[1], 2);
    
    setGameVariableString(8, loadGameScreenContext.playerNames[2], 6);
    setGameVariableString(9, loadGameScreenContext.dateNumbers[2], 2);
    setGameVariableString(10, loadGameScreenContext.seasonNames[2], 6);
    setGameVariableString(11, loadGameScreenContext.dateEndings[2], 2);
    
    setGameVariableString(0xC, loadGameScreenContext.playerNames[3], 6);
    setGameVariableString(0xD, loadGameScreenContext.dateNumbers[3], 2);
    setGameVariableString(0xE, loadGameScreenContext.seasonNames[3], 6);
    setGameVariableString(0xF, loadGameScreenContext.dateEndings[3], 2);
    
    func_800E16D0(1, loadGameScreenContext.diaryHighlighted);
    
    setMainLoopCallbackFunctionIndex(SELECT_GAME);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/loadGameScreen", func_800E16A0);

void func_800E16A0(void) {

    u32 padding[4];
    
    loadGameScreenContext.gamePakEnabled = FALSE;
    loadGameScreenContext.unk_14 = 0;
    loadGameScreenContext.unk_29 = 0;
    loadGameScreenContext.unk_3C = 0;

}

//INCLUDE_ASM("asm/nonmatchings/game/loadGameScreen", func_800E16D0);

void func_800E16D0(u8 arg0, u8 arg1) {

    loadGameScreenContext.diaryHighlighted = arg1;
    
    func_800E1998();
    func_800B42E0();
    
    if (arg0) {
        func_800B59AC();
    } else {
        func_800B5BD4();
    }
    
    if (loadGameScreenContext.showControllerPakScreen == 0) {
        
        if (arg0) {
            func_8003E77C(0, 0, 0, 0, 0);
            func_8003E77C(1, 0, 0, 0, 0);
            func_8003E77C(2, 0, 0, 0, 0);
            func_8003E77C(3, 0, 0, 0, 0);
            func_8003EA1C(0, 0xFF, 0xFF, 0xFF, 0xFF, 24);
            func_8003EA1C(1, 0xFF, 0xFF, 0xFF, 0xFF, 24);
            func_8003EA1C(2, 0xFF, 0xFF, 0xFF, 0xFF, 24);
            func_8003EA1C(3, 0xFF, 0xFF, 0xFF, 0xFF, 24);
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
        func_8003EA1C(0, 0xFF, 0xFF, 0xFF, 0xFF, 24);
        func_8003EA1C(1, 0xFF, 0xFF, 0xFF, 0xFF, 24);
        return;
    }
    
    func_8003E77C(0, 0, 0, 0, 0);
    func_8003E77C(1, 0, 0, 0, 0);
    func_8003E77C(0, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8003E77C(1, 0xFF, 0xFF, 0xFF, 0xFF);

}


//INCLUDE_ASM("asm/nonmatchings/game/loadGameScreen", func_800E1998);

void func_800E1998(void) {

    func_800E16A0();

    if (loadGameScreenContext.showControllerPakScreen == FALSE) {
        func_800E3358(0, 0, 0);
        func_800E3358(1, 0, 0);
        func_800E3358(2, 0, 0);
        func_800E3358(3, 0, 0);
    } else {

        if (loadGameScreenContext.unk_14 & 2) {
            func_800E3358(0, 1, 0);
        } else if (loadGameScreenContext.unk_14 & 1) {
            func_800E3358(0, 1, 1);
        } else {
            func_800E3358(0, 1, 2);
        }

        if (loadGameScreenContext.unk_29 & 2) {
            func_800E3358(1, 1, 0);
        } else {
            if (loadGameScreenContext.unk_29 & 1) {
                func_800E3358(1, 1, 1);
            } else {
                func_800E3358(1, 1, 2);   
            }
        }
    }
    
    func_800E1A94();

}

//INCLUDE_ASM("asm/nonmatchings/game/loadGameScreen", func_800E1A94);

void func_800E1A94(void) {

    if (loadGameScreenContext.showControllerPakScreen == 0) {

        if (loadGameScreenContext.fontLoadingFlags[0] & 1) { 
            func_8003DD14(0);
            initializeEmptyMessageBox(0, (void*)0x8030B000);
            setMessageBoxViewSpacePosition(0, 16.0f, 52.0f, 0);
            func_8003F5D0(0, 0xC, 2);
            func_8003F630(0, 0, 2);
            func_8003F464(0, 0xE, 0xE, (u8*)COMPRESSED_FONT_VADDR, (u16*)FONT_PALETTE_1_VADDR);
            func_8003F360(0, -4, 2);
            setMessageBoxSpriteIndices(0, 0xFF, 0, 0);
            setMessageBoxButtonMask(0, BUTTON_A);
            func_8003FB4C(0, 1);
            initializeMessageBox(MAIN_DIALOGUE_BOX_INDEX, 0, 1, 0x80000);
        }

        if (loadGameScreenContext.fontLoadingFlags[1] & 1) {
            func_8003DD14(1);
            initializeEmptyMessageBox(1, (void*)0x8030B400);
            setMessageBoxViewSpacePosition(1, 16.0f, 8.0f, 0);
            func_8003F5D0(1, 0xC, 2);
            func_8003F630(1, 0, 2);
            func_8003F464(1, 0xE, 0xE, (u8*)COMPRESSED_FONT_VADDR, (u16*)FONT_PALETTE_1_VADDR);
            func_8003F360(1, -4, 2);
            setMessageBoxSpriteIndices(1, 0xFF, 0, 0);
            setMessageBoxButtonMask(1, BUTTON_A);
            func_8003FB4C(1, 1);
            initializeMessageBox(1, 0, 2, 0x80000);
        }

        if (loadGameScreenContext.fontLoadingFlags[2] & 1) {
            func_8003DD14(2);
            initializeEmptyMessageBox(2, (void*)0x8030B800);
            setMessageBoxViewSpacePosition(2, 16.0f, -36.0f, 0);
            func_8003F5D0(2, 0xC, 2);
            func_8003F630(2, 0, 2);
            func_8003F464(2, 0xE, 0xE, (u8*)COMPRESSED_FONT_VADDR, (u16*)FONT_PALETTE_1_VADDR);
            func_8003F360(2, -4, 2);
            setMessageBoxSpriteIndices(2, 0xFF, 0, 0);
            setMessageBoxButtonMask(2, BUTTON_A);
            func_8003FB4C(2, 1);
            initializeMessageBox(2, 0, 3, 0x80000);
        }

        if (loadGameScreenContext.fontLoadingFlags[3] & 1) {
            func_8003DD14(3);
            initializeEmptyMessageBox(3, (void*)0x8030BC00);
            setMessageBoxViewSpacePosition(3, 16.0f, -80.0f, 0);
            func_8003F5D0(3, 0xC, 2);
            func_8003F630(3, 0, 2);
            func_8003F464(3, 0xE, 0xE, (u8*)COMPRESSED_FONT_VADDR, (u16*)FONT_PALETTE_1_VADDR);
            func_8003F360(3, -4, 2);
            setMessageBoxSpriteIndices(3, 0xFF, 0, 0);
            setMessageBoxButtonMask(3, BUTTON_A);
            func_8003FB4C(3, 1);
            initializeMessageBox(3, 0, 4, 0x80000);
        }

    } else {

        if (loadGameScreenContext.fontLoadingFlags[0] & 1) {
            func_8003DD14(0);
            initializeEmptyMessageBox(0, (void*)0x8030B000);
            setMessageBoxViewSpacePosition(0, 16.0f, 52.0f, 0);
            func_8003F5D0(0, 0xC, 2);
            func_8003F630(0, 0, 2);
            func_8003F464(0, 0xE, 0xE, (u8*)COMPRESSED_FONT_VADDR, (u16*)FONT_PALETTE_1_VADDR);
            func_8003F360(0, -4, 2);
            setMessageBoxSpriteIndices(0, 0xFF, 0, 0);
            setMessageBoxButtonMask(0, BUTTON_A);
            func_8003FB4C(0, 1);
            initializeMessageBox(MAIN_DIALOGUE_BOX_INDEX, 0, 1, 0x80000);
        }
        
        if (loadGameScreenContext.fontLoadingFlags[1] & 1) {
            func_8003DD14(1);
            initializeEmptyMessageBox(1, (void*)0x8030B400);
            setMessageBoxViewSpacePosition(1, 16.0f, 8.0f, 0);
            func_8003F5D0(1, 0xC, 2);
            func_8003F630(1, 0, 2);
            func_8003F464(1, 0xE, 0xE, (u8*)COMPRESSED_FONT_VADDR, (u16*)FONT_PALETTE_1_VADDR);
            func_8003F360(1, -4, 2);
            setMessageBoxSpriteIndices(1, 0xFF, 0, 0);
            setMessageBoxButtonMask(1, BUTTON_A);
            func_8003FB4C(1, 1);
            initializeMessageBox(1, 0, 2, 0x80000);
        }
    }

}

INCLUDE_ASM("asm/nonmatchings/game/loadGameScreen", func_800E1FAC);

//INCLUDE_ASM("asm/nonmatchings/game/loadGameScreen", func_800E3300);

bool func_800E3300(u8 arg0[]) {

    u8 i = 0;
    bool result = 0;

    if (*arg0 == D_80119740[0]) {

        do {
            i++;
        } while (arg0[i] == D_80119740[i]);
        
    }

    if (i == 16) {
        result = TRUE;
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/loadGameScreen", func_800E3358);

void func_800E3358(u8 arg0, u8 arg1, u8 arg2) {

    switch (arg2) {

        case 0:
        
            if (func_800E4424(arg0, arg1)) {
            
                setGlobalSeasonName(gSeason);
                
                loadGameScreenContext.seasonNames[arg0][0] = gGlobalSeasonName[0];
                loadGameScreenContext.seasonNames[arg0][1] = gGlobalSeasonName[1];
                loadGameScreenContext.seasonNames[arg0][2] = gGlobalSeasonName[2];
                loadGameScreenContext.seasonNames[arg0][3] = gGlobalSeasonName[3];
                loadGameScreenContext.seasonNames[arg0][4] = gGlobalSeasonName[4];
                loadGameScreenContext.seasonNames[arg0][5] = gGlobalSeasonName[5];
        
                switch (gYear) {
                    case 1:
                        loadGameScreenContext.dateEndings[arg0][0] = 0xCD;
                        loadGameScreenContext.dateEndings[arg0][1] = 0xCE;
                        break;
                    case 2:
                        loadGameScreenContext.dateEndings[arg0][0] = 0xC8;
                        loadGameScreenContext.dateEndings[arg0][1] = 0xBE;
                        break;
                    case 3:
                        loadGameScreenContext.dateEndings[arg0][0] = 0xCC;
                        loadGameScreenContext.dateEndings[arg0][1] = 0xBE;
                        break;
                    default:
                        loadGameScreenContext.dateEndings[arg0][0] = 0xCE;
                        loadGameScreenContext.dateEndings[arg0][1] = 0xC2;
                        break;
                }

                switch (arg0) {
                    case 0:
                        convertNumberToGameVariableString(1, gYear, 1);
                        break;                  
                    case 1:
                        convertNumberToGameVariableString(5, gYear, 1);
                        break;
                    case 2:
                        convertNumberToGameVariableString(9, gYear, 1);
                        break;
                    case 3:
                        convertNumberToGameVariableString(13, gYear, 1);
                        break;                  
                    default:
                        break;
                }
                
                loadGameScreenContext.playerNames[arg0][0] = gPlayer.name[0];
                loadGameScreenContext.playerNames[arg0][1] = gPlayer.name[1];
                loadGameScreenContext.playerNames[arg0][2] = gPlayer.name[2];
                loadGameScreenContext.playerNames[arg0][3] = gPlayer.name[3];
                loadGameScreenContext.playerNames[arg0][4] = gPlayer.name[4];
                loadGameScreenContext.playerNames[arg0][5] = gPlayer.name[5];
    
                loadGameScreenContext.fontLoadingFlags[arg0] = 3;

            } else {
                
                loadGameScreenContext.playerNames[arg0][0] = 0xF6;
                loadGameScreenContext.playerNames[arg0][1] = 0xF6;
                loadGameScreenContext.playerNames[arg0][2] = 0xF6;
                loadGameScreenContext.playerNames[arg0][3] = 0xF6;
                loadGameScreenContext.playerNames[arg0][4] = 0xF6;
                loadGameScreenContext.playerNames[arg0][5] = 0xF6;
                
                loadGameScreenContext.dateNumbers[arg0][0] = 0xF6;
                loadGameScreenContext.dateNumbers[arg0][1] = 0xF6;

                loadGameScreenContext.seasonNames[arg0][0] = 0xF6;
                loadGameScreenContext.seasonNames[arg0][1] = 0xF6;
                loadGameScreenContext.seasonNames[arg0][2] = 0xF6;
                loadGameScreenContext.seasonNames[arg0][3] = 0xF6;
                loadGameScreenContext.seasonNames[arg0][4] = 0xF6;
                loadGameScreenContext.seasonNames[arg0][5] = 0xF6;
                    
                loadGameScreenContext.dateEndings[arg0][0] = 0xF6;
                loadGameScreenContext.dateEndings[arg0][1] = 0xF6;     
                
                loadGameScreenContext.fontLoadingFlags[arg0] = 1;
            }

            break;
        
        case 1:
                   
            loadGameScreenContext.playerNames[arg0][0] = 0xF6;
            loadGameScreenContext.playerNames[arg0][1] = 0xF6;
            loadGameScreenContext.playerNames[arg0][2] = 0xF6;
            loadGameScreenContext.playerNames[arg0][3] = 0xF6;
            loadGameScreenContext.playerNames[arg0][4] = 0xF6;
            loadGameScreenContext.playerNames[arg0][5] = 0xF6;
            
            loadGameScreenContext.dateNumbers[arg0][0] = 0xF6;
            loadGameScreenContext.dateNumbers[arg0][1] = 0xF6;
            
            loadGameScreenContext.seasonNames[arg0][0] = 0xF6;
            loadGameScreenContext.seasonNames[arg0][1] = 0xF6;
            loadGameScreenContext.seasonNames[arg0][2] = 0xF6;
            loadGameScreenContext.seasonNames[arg0][3] = 0xF6;
            loadGameScreenContext.seasonNames[arg0][4] = 0xF6;
            loadGameScreenContext.seasonNames[arg0][5] = 0xF6;
            
            loadGameScreenContext.dateEndings[arg0][0] = 0xF6;
            loadGameScreenContext.dateEndings[arg0][1] = 0xF6;
            
            loadGameScreenContext.fontLoadingFlags[arg0] = 1;
            
            break;
        
        case 2:
            
            loadGameScreenContext.playerNames[arg0][0] = 0xEE;
            loadGameScreenContext.playerNames[arg0][1] = 0xEE;
            loadGameScreenContext.playerNames[arg0][2] = 0xEE;
            loadGameScreenContext.playerNames[arg0][3] = 0xEE;
            loadGameScreenContext.playerNames[arg0][4] = 0xEE;
            loadGameScreenContext.playerNames[arg0][5] = 0xEE;
            
            loadGameScreenContext.dateNumbers[arg0][0] = 0xEE;
            loadGameScreenContext.dateNumbers[arg0][1] = 0xEE;

            loadGameScreenContext.seasonNames[arg0][0] = 0xEE;
            loadGameScreenContext.seasonNames[arg0][1] = 0xEE;
            loadGameScreenContext.seasonNames[arg0][2] = 0xEE;
            loadGameScreenContext.seasonNames[arg0][3] = 0xEE;
            loadGameScreenContext.seasonNames[arg0][4] = 0xEE;
            loadGameScreenContext.seasonNames[arg0][5] = 0xEE;
            
            loadGameScreenContext.dateEndings[arg0][0] = 0xEE;
            loadGameScreenContext.dateEndings[arg0][1] = 0xEE;
            
            loadGameScreenContext.fontLoadingFlags[arg0] = 0;

            break;       

    }

}

//INCLUDE_ASM("asm/nonmatchings/game/loadGameScreen", func_800E38E8);

u16 func_800E38E8(u8 arg0[]) {

    u16 result = 0;
    u16 i = 0;

    do {

        result += arg0[i];
        i++;
        
    } while (i < 4094);

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/loadGameScreen", func_800E391C);

u16 func_800E391C(u8* arg0) {

    u16 result = 0;
    u16 i = 0;

    do {

        result += arg0[i];
        i++;
        
    } while (i < 0xFFE);

    return result == *(u16*)(arg0 + 4094);

}

//INCLUDE_ASM("asm/nonmatchings/game/loadGameScreen", func_800E395C);

u16 func_800E395C(u8 arg0[]) {

    u16 result = 0;
    u16 i = 0;

    do {

        result += arg0[i];
        i++;
        
    } while (i < 254);

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/loadGameScreen", func_800E3990);

u16 func_800E3990(u8 arg0[]) {

    u16 result = 0;
    u16 i = 0;

    do {

        result += arg0[i];
        i++;
        
    } while (i < 254);

    return result == *(u16*)(arg0 + 254);
    
}

INCLUDE_ASM("asm/nonmatchings/game/loadGameScreen", func_800E39D0);

INCLUDE_ASM("asm/nonmatchings/game/loadGameScreen", func_800E4424);

INCLUDE_ASM("asm/nonmatchings/game/loadGameScreen", func_800E53E8);

//INCLUDE_ASM("asm/nonmatchings/game/loadGameScreen", func_800E66A0);

void func_800E66A0(u8 arg0, u8 arg1, u8 arg2, u8 arg3) {

    u32 devAddr;
    u32 vaddr;
    u32 temp;

    temp =  arg0 << 12;
    
    devAddr = temp + 0x08000000;
    vaddr = 0x8030D000;
     
    if (arg1 == 0) {
        func_8004DC48(devAddr, vaddr, 0x1000);
    } else {

        func_800E16A0();

        if (loadGameScreenContext.gamePakEnabled) {
            func_8004D9AC(CONTROLLER_1, &loadGameScreenContext.unk_0[(arg0 * 4 + arg0) * 4 + arg0], &loadGameScreenContext.unk_15[(arg0 * 4 + arg0) * 4 + arg0 - 5], 0, 0x1000, vaddr);
        }
        
    }

    temp = arg2 << 12;
    devAddr = temp + 0x08000000;
    
    if (arg3 == 0) {

        func_8004DD7C(devAddr, vaddr, 0x1000);
        
    } else {

        func_800E16A0();
        
        if (loadGameScreenContext.gamePakEnabled) {
            func_8004DA48(CONTROLLER_1, &loadGameScreenContext.unk_0[(arg2 * 4 + arg2) * 4 + arg2], &loadGameScreenContext.unk_15[(arg2 * 4 + arg2) * 4 + arg2 - 5], 0, 0x1000, vaddr);
        }
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/loadGameScreen", func_800E67E4);

void func_800E67E4(u8 arg0, u8 arg1) {

    u32 devAddr;
    u32 vaddr;
    u32 temp;

    D_8030D000[0] = 0;
    D_8030D000[1] = 0;
    D_8030D000[2] = 0;
    D_8030D000[3] = 0;
    D_8030D000[4] = 0;
    D_8030D000[5] = 0;
    D_8030D000[6] = 0;
    D_8030D000[7] = 0;
    D_8030D000[8] = 0;
    D_8030D000[9] = 0;
    D_8030D000[10] = 0;
    D_8030D000[11] = 0;
    D_8030D000[12] = 0;
    D_8030D000[13] = 0;
    D_8030D000[14] = 0;
    D_8030D000[15] = 0;

    temp =  arg0 << 12;
    devAddr = temp + 0x08000000;
    vaddr = 0x8030D000;

    if (arg1 == 0) {
        func_8004DD7C(devAddr, vaddr, 0x10);
    } else {

        func_800E16A0();

        if (loadGameScreenContext.gamePakEnabled) {
            func_8004DAF4(CONTROLLER_1, &loadGameScreenContext.unk_0[(arg0 * 4 + arg0) * 4 + arg0], &loadGameScreenContext.unk_15[(arg0 * 4 + arg0) * 4 + arg0 - 5]);
        }
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/loadGameScreen", func_800E68F4);

void func_800E68F4(void) {

    u8 i = 0;

    for (i = 0; i < 7; i++) {
        func_800E6C08(i);
    }
    
    setGameVariableString(0, &D_801FB708[0], 6);
    setGameVariableString(1, &D_801FB708[1], 6);
    setGameVariableString(2, &D_801FB708[2], 6);
    setGameVariableString(3, &D_801FB708[3], 6);
    setGameVariableString(4, &D_801FB708[4], 6);
    
    func_800E9550(0);
    func_800E9550(1);
    func_800E9550(2);
    func_800E9550(3);
    func_800E9550(4);
    func_800EA3AC(0, 0);
    func_800EA3AC(1, 0);
    func_800EA3AC(2, 0);
    func_800EA3AC(3, 0);
    func_800EA3AC(0, 1);
    func_800EA3AC(1, 1);
    func_800E9B2C(0);
    func_800E9B2C(1);
    func_800E9B2C(2);
    func_800E9B2C(3);
    func_800E9B2C(4);
    func_800E6FB4(0);
    func_800B6674();
    func_800B7B34();

    setMainLoopCallbackFunctionIndex(0x36);
    
    D_801FB9A0 = 0;
    D_801FB99B[0] = 0;

    D_8018A728 = D_801FB978[0];
    D_801D6F2C = D_801FB978[1];
    D_8018A080 = D_801FB978[2];
    D_802226E4 = D_801FB978[3];
    D_80204DF0 = D_801FB978[4];
    
}

INCLUDE_ASM("asm/nonmatchings/game/loadGameScreen", func_800E6C08);

INCLUDE_ASM("asm/nonmatchings/game/loadGameScreen", func_800E6FB4);

INCLUDE_ASM("asm/nonmatchings/game/loadGameScreen", func_800E80AC);

//INCLUDE_ASM("asm/nonmatchings/game/loadGameScreen", func_800E8F08);

// evaluation screen main loop callback
void func_800E8F08(void) {

    bool set = FALSE;

    switch (D_801FB9A0) {

        case 0:
            
            if (func_8002CBF8(0x80)) {
                func_800B7F6C(D_801FB99B[0]);
                D_801FB9A0 = 1;
            }
            
            break;

        case 1:
            
            if (checkButtonRepeat(CONTROLLER_1, 0x100000)) {
                
                if (!set) {
                    
                    if (D_801FB99B[0]) {
                        D_801FB99B[0]--;
                        setAudio(2);
                    }
                    
                    set = TRUE;
                    
                }
                
            }
            
            if (checkButtonRepeat(CONTROLLER_1, 0x10000) && !set) {
                
                if (D_801FB99B[0] != 4) {
                    D_801FB99B[0]++;
                    setAudio(2);
                }
                
                set = TRUE;
                
            }

            func_800B7F6C(D_801FB99B[0]);
            
            if (checkButtonPressed(CONTROLLER_1, 0x8000) && !set) {
                
                if (D_801FB994[D_801FB99B[0]] & 1) {
                    
                    deactivateSprite(0x79);
                    deactivateSprite(0x82);
                    deactivateSprite(0x83);
                    deactivateSprite(0x84);
                    deactivateSprite(0x85);
                    deactivateSprite(0x86);
                    deactivateSprite(0x87);
                    deactivateSprite(0xAE);
                    deactivateSprite(0xAF);
                    deactivateSprite(0xB0);
                    deactivateSprite(0xB1);
                    deactivateSprite(0xB2);
                    
                    func_8003DD14(0);
                    func_8003DD14(1);
                    func_8003DD14(2);
                    func_8003DD14(3);
                    func_8003DD14(4);
                    
                    deactivatePauseScreenSprites(0);
                    deactivatePauseScreenSprites(1);
                    deactivatePauseScreenSprites(2);
                    deactivatePauseScreenSprites(3);
                    deactivatePauseScreenSprites(4);
                    deactivatePauseScreenSprites(5);
                    deactivatePauseScreenSprites(6);
                    deactivatePauseScreenSprites(7);
                    deactivatePauseScreenSprites(8);
                    deactivatePauseScreenSprites(9);
                    
                    D_801FB9A0 = 2;
                    setAudio(0);
                    
                }
            
                set = TRUE;
                
            }

            if (checkButtonPressed(CONTROLLER_1, 0x4000) && !set) {
                
                deactivateSprite(0x79);
                
                func_800B7CB4();
                func_8003EA1C(0, 0, 0, 0, 0, 0x18);
                func_8003EA1C(1, 0, 0, 0, 0, 0x18);
                func_8003EA1C(2, 0, 0, 0, 0, 0x18);
                func_8003EA1C(3, 0, 0, 0, 0, 0x18);
                func_8003EA1C(4, 0, 0, 0, 0, 0x18);
                func_8004635C(0, 0, 0, 0, 0, 0x18);
                func_8004635C(1, 0, 0, 0, 0, 0x18);
                func_8004635C(2, 0, 0, 0, 0, 0x18);
                func_8004635C(3, 0, 0, 0, 0, 0x18);
                func_8004635C(4, 0, 0, 0, 0, 0x18);
                func_8004635C(5, 0, 0, 0, 0, 0x18);
                func_8004635C(6, 0, 0, 0, 0, 0x18);
                func_8004635C(7, 0, 0, 0, 0, 0x18);
                func_8004635C(8, 0, 0, 0, 0, 0x18);
                func_8004635C(9, 0, 0, 0, 0, 0x18);
                
                D_801FB9A0 = 5;
                setAudio(0);
                
            }

            break;

        case 2:
            func_800E80AC(D_801FB99B[0]);
            func_800B7070(D_801FB99B[0]);
            D_801FB9A0 = 3;
            break;

        case 3:
            if (checkButtonPressed(CONTROLLER_1, 0x4000) && !set) {
                
                deactivateSprite(0x81);
                deactivateSprite(0x82);
                deactivateSprite(0x83);
                deactivateSprite(0x84);
                deactivateSprite(0x85);
                deactivateSprite(0x86);
                deactivateSprite(0xAE);
                deactivateSprite(0xAF);
                deactivateSprite(0xB0);
                deactivateSprite(0xB1);
                deactivateSprite(0xB2);
                
                func_8003DD14(0);
                func_8003DD14(1);
                func_8003DD14(2);
                func_8003DD14(3);
                func_8003DD14(4);
                func_8003DD14(5);
                
                deactivatePauseScreenSprites(0);
                deactivatePauseScreenSprites(1);
                deactivatePauseScreenSprites(2);
                deactivatePauseScreenSprites(3);
                deactivatePauseScreenSprites(4);
                deactivatePauseScreenSprites(5);
                deactivatePauseScreenSprites(6);
                deactivatePauseScreenSprites(7);
                deactivatePauseScreenSprites(8);
                deactivatePauseScreenSprites(9);
                deactivatePauseScreenSprites(0xA);
                
                D_801FB9A0 = 4;
                setAudio(0);
                
            }
            
            break;
        
        case 4:
            func_800E6FB4(1);
            func_800B6674();
            func_800B7E28();

            D_801FB9A0 = 1;
            break;

        case 5:
            
            if (func_8002CBF8(0x80)) {
                
                func_800B2CE0();
                
                deactivateSprite(0x80);
                
                func_8003DD14(0);
                func_8003DD14(1);
                func_8003DD14(2);
                func_8003DD14(3);
                func_8003DD14(4);
                
                deactivatePauseScreenSprites(0);
                deactivatePauseScreenSprites(1);
                deactivatePauseScreenSprites(2);
                deactivatePauseScreenSprites(3);
                deactivatePauseScreenSprites(4);
                deactivatePauseScreenSprites(5);
                deactivatePauseScreenSprites(6);
                deactivatePauseScreenSprites(7);
                deactivatePauseScreenSprites(8);
                deactivatePauseScreenSprites(9);
                
                func_800E1380(1);
                
            }
            
            break;
                
        }
    
}

INCLUDE_ASM("asm/nonmatchings/game/loadGameScreen", func_800E9550);

INCLUDE_ASM("asm/nonmatchings/game/loadGameScreen", func_800E9B2C);

//INCLUDE_ASM("asm/nonmatchings/game/loadGameScreen", func_800EA2A4);

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

    func_8004DD7C(((arg0 * 0x100) + 0x4000) | 0x08000000, 0x8030E000, 0x10);

} 

//INCLUDE_ASM("asm/nonmatchings/game/loadGameScreen", func_800EA360);

u8 func_800EA360(void) {

    u8 count = 0;

    if (D_801FB994[0] & 1) {

loop:
        if (count < 5) {
            
            count++;
            
            if (D_801FB994[count] & 1) {
                goto loop;
            }
            
        } 
    }

    return count;
    
}

static inline void setBabyAffection() {
    if (checkLifeEventBit(HAVE_BABY)) {
        D_801FB864 = npcAffection[BABY];
        D_801FB999[0] |= 4;
    } else {
        D_801FB864 = 0;
    }
}

static inline void setHorseAffection() {
    if (checkLifeEventBit(HAVE_HORSE)) {
        D_801FB872 = horseInfo.affection;
        D_801FB999[0] |= 8;
    } else {
        D_801FB872 = 0;
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/loadGameScreen", func_800EA3AC);

void func_800EA3AC(u8 arg0, u8 arg1) {

    if (func_800E4424(arg0, arg1)) {

        D_801FB999[0] = 1;
        
        D_801FB726[0] = gFarmName[0];
        D_801FB726[1] = gFarmName[1];
        D_801FB726[2] = gFarmName[2];
        D_801FB726[3] = gFarmName[3];
        D_801FB726[4] = gFarmName[4];
        D_801FB726[5] = gFarmName[5];
        
        D_801FB750[0] = gPlayer.name[0];
        D_801FB750[1] = gPlayer.name[1];
        D_801FB750[2] = gPlayer.name[2];
        D_801FB750[3] = gPlayer.name[3];
        D_801FB750[4] = gPlayer.name[4];
        D_801FB750[5] = gPlayer.name[5];
        
        D_801FB976 = gWife;

        if (checkLifeEventBit(MARRIED)) {
            
            setWifeNameString(D_801FB976);
            
            D_801FB77A[0] = gWifeName[0];
            D_801FB77A[1] = gWifeName[1];
            D_801FB77A[2] = gWifeName[2];
            D_801FB77A[3] = gWifeName[3];
            D_801FB77A[4] = gWifeName[4];
            D_801FB77A[5] = gWifeName[5];
            
        }

        if (checkLifeEventBit(1)) {
                
            D_801FB7A4[0] = gBabyName[0];
            D_801FB7A4[1] = gBabyName[1];
            D_801FB7A4[2] = gBabyName[2];
            D_801FB7A4[3] = gBabyName[3];
            D_801FB7A4[4] = gBabyName[4];
            D_801FB7A4[5] = gBabyName[5];
            
        }

        D_801FB7CE[0] = dogInfo.name[0];
        D_801FB7CE[1] = dogInfo.name[1];
        D_801FB7CE[2] = dogInfo.name[2];
        D_801FB7CE[3] = dogInfo.name[3];
        D_801FB7CE[4] = dogInfo.name[4];
        D_801FB7CE[5] = dogInfo.name[5];

        if (checkLifeEventBit(HAVE_HORSE)) {
            
            D_801FB7F8[0] = horseInfo.name[0];
            D_801FB7F8[1] = horseInfo.name[1];
            D_801FB7F8[2] = horseInfo.name[2];
            D_801FB7F8[3] = horseInfo.name[3];
            D_801FB7F8[4] = horseInfo.name[4];
            D_801FB7F8[5] = horseInfo.name[5];
            
        }

        D_801FB809 = gYear;
        D_801FB810 = gSeason;
        
        D_801FB817 = npcAffection[MARIA];
        D_801FB81E = npcAffection[POPURI];
        D_801FB825 = npcAffection[ELLI];
        D_801FB82C = npcAffection[ANN];
        D_801FB833 = npcAffection[KAREN];
        D_801FB83A = npcAffection[HARRIS];
        D_801FB841 = npcAffection[GRAY];
        D_801FB848 = npcAffection[JEFF];
        D_801FB84F = npcAffection[CLIFF];
        D_801FB856 = npcAffection[KAI];

        if (checkLifeEventBit(MARRIED)) {
            D_801FB85D = npcAffection[D_801FB976];
            D_801FB999[0] |= 2;
        } else {
            D_801FB85D = 0;
        }

        setBabyAffection();
        
        D_801FB86B = dogInfo.affection;

        setHorseAffection();
        
        if (gFarmAnimals[0].flags & 1) {
            D_801FB89C = gFarmAnimals[0].affection;
        } else {
            D_801FB89C = 0;
        }
                
        if (gFarmAnimals[1].flags & 1) {
            D_801FB89D = gFarmAnimals[1].affection;
        } else {
            D_801FB89D = 0;
        }
                
        if (gFarmAnimals[2].flags & 1) {
            D_801FB89E = gFarmAnimals[2].affection;
        } else {
            D_801FB89E = 0;
        }
                
        if (gFarmAnimals[3].flags & 1) {
            D_801FB89F = gFarmAnimals[3].affection;
        } else {
            D_801FB89F = 0;
        }
                
        if (gFarmAnimals[4].flags & 1) {
            D_801FB8A0 = gFarmAnimals[4].affection;
        } else {
            D_801FB8A0 = 0;
        }
                
        if (gFarmAnimals[5].flags & 1) {
            D_801FB8A1 = gFarmAnimals[5].affection;
        } else {
            D_801FB8A1 = 0;
        }
                        
        if (gFarmAnimals[6].flags & 1) {
            D_801FB8A2 = gFarmAnimals[6].affection;
        } else {
            D_801FB8A2 = 0;
        }
                
        if (gFarmAnimals[7].flags & 1) {
            D_801FB8A3 = gFarmAnimals[7].affection;
        } else {
            D_801FB8A3 = 0;
        }

        D_801FB8B1 = func_8009B564();
        D_801FB8C8 = gTotalCropsShipped;
        D_801FB8E4 = gTotalEggsShipped;
        D_801FB900 = gTotalGoldenMilkShipped;
        D_801FB91C = totalFishCaught;
        D_801FB938 = gGold;
        D_801FB953 = gMaximumStamina;
        D_801FB96F = gHappiness;
        
        func_800EB74C(5);
        func_800EB788(5);
        func_800EBA90(5);
        func_800EBAC8(5);
        func_800EAA9C(5);
        
        if (!(func_800EBCD8())) {
            func_800EBC00();
        }
        
    } else {
        D_801FB999[0] = 0;
    }
    
}

INCLUDE_ASM("asm/nonmatchings/game/loadGameScreen", func_800EAA9C);

//INCLUDE_ASM("asm/nonmatchings/game/loadGameScreen", func_800EB74C);

void func_800EB74C(u8 arg0) {
    D_801FB940[arg0] = getFarmGrassTilesSum();
}

//INCLUDE_ASM("asm/nonmatchings/game/loadGameScreen", func_800EB788);

void func_800EB788(u8 arg0) {

    D_801FB955[arg0] = 0;

    if (albumBits & 1) {
        D_801FB955[arg0]++;
    }

    if (albumBits & 2) {
        D_801FB955[arg0]++;
    }

    if (albumBits & 4) {
        D_801FB955[arg0]++;
    }

    if (albumBits & 8) {
        D_801FB955[arg0]++;
    }

    if (albumBits & 0x10) {
        D_801FB955[arg0]++;
    }

    if (albumBits & 0x20) {
        D_801FB955[arg0]++;
    }

    if (albumBits & 0x40) {
        D_801FB955[arg0]++;
    }
    
    if (albumBits & 0x80) {
        D_801FB955[arg0]++;
    }

    if (albumBits & 0x100) {
        D_801FB955[arg0]++;
    }
    
    if (albumBits & 0x200) {
        D_801FB955[arg0]++;
    }
    
    if (albumBits & 0x400) {
        D_801FB955[arg0]++;
    }

    if (albumBits & 0x800) {
        D_801FB955[arg0]++;
    }

    if (albumBits & 0x1000) {
        D_801FB955[arg0]++;
    }
    
    if (albumBits & 0x2000) {
        D_801FB955[arg0]++;
    }

    if (albumBits & 0x4000) {
        D_801FB955[arg0]++;
    }

    if (albumBits & 0x8000) {
        D_801FB955[arg0]++;
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/loadGameScreen", func_800EBA90);

void func_800EBA90(u8 arg0) {
    D_801FB95C[arg0] = func_80065518();
}

//INCLUDE_ASM("asm/nonmatchings/game/loadGameScreen", func_800EBAC8);

void func_800EBAC8(u8 arg0) {

    D_801FB963[arg0] = 0;
    
    if (checkLifeEventBit(HAVE_KITCHEN)) {
        D_801FB963[arg0]++;
    }
    if (checkLifeEventBit(HAVE_BATHROOM)) {
        D_801FB963[arg0]++;
    }
    if (checkLifeEventBit(HAVE_STAIRS)) {
        D_801FB963[arg0]++;
    }
    if (checkLifeEventBit(HAVE_GREENHOUSE)) {
        D_801FB963[arg0]++;
    }
    if (checkLifeEventBit(HAVE_LOG_TERRACE)) {
        D_801FB963[arg0]++;
    }
    if (checkLifeEventBit(HAVE_BABY_BED)) {
        D_801FB963[arg0]++;
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/loadGameScreen", func_800EBC00);

void func_800EBC00(void) {

    f32 f1, f2;
    u8 i = 0;
    u8 j;
    u8 temp = 0;

    while (i < 5) {
        
        temp = i;

        f1 = D_801FB978[i];

        j = i + 1;
        
        while (j < 6) {

            f2 = D_801FB978[j];

            if ((f1 < f2 || !(D_801FB994[temp] & 1)) && D_801FB994[j] & 1) {
                temp = j;
                f1 = f2;
            }

            j++;

        } 
        
        func_800EBEAC(i, temp);

        i++;
        
    } 
    
}

INCLUDE_ASM("asm/nonmatchings/game/loadGameScreen", func_800EBCD8);

INCLUDE_ASM("asm/nonmatchings/game/loadGameScreen", func_800EBEAC);

INCLUDE_ASM("asm/nonmatchings/game/loadGameScreen", func_800ED160);
