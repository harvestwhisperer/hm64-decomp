#include "common.h"

#include "game/loadGameScreen.h"

#include "system/controller.h"
#include "system/globalSprites.h"
#include "system/message.h"
#include "system/overlayScreenSprites.h"

#include "game/animals.h"
#include "game/fieldObjects.h"
#include "game/game.h"
#include "game/gameAudio.h"
#include "game/gameStart.h"
#include "game/gameStatus.h"
#include "game/itemHandlers.h"
#include "game/level.h"
#include "game/npc.h"
#include "game/overlayScreens.h"
#include "game/player.h"
#include "game/shop.h"
#include "game/tv.h"
#include "game/time.h"
#include "game/weather.h"

#include "buffers/buffers.h"

#include "mainLoop.h"
 
#include "ld_symbols.h"

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
extern u8 gCurrentGameIndex;

// unused
extern u32 D_8018A080;
extern u32 D_8018A728;
extern u32 D_801D6F2C;

extern u32 D_801C3F3C;
extern u32 D_801C3F40;
extern u32 D_8016FFF0;
extern u32 D_8016FFF4;

// FIXME: convert to struct
// evaluation screen struct
// strings
// player abbreviated name
extern u8 D_801FB708[5][6];
extern u8 D_801FB726[6];
extern u8 D_801FB72C[];
extern u8 D_801FB730[];
// player name in individual farm completion screen
extern u8 D_801FB732[5][6];
// player name
extern u8 D_801FB750[6];
extern u8 D_801FB756[];
extern u8 D_801FB75C[5][6];
// wife name
extern u8 D_801FB77A[6];
extern u8 D_801FB780[6];
extern u8 D_801FB786[5][6];
// dog name
extern u8 D_801FB7A4[6];
extern u8 D_801FB7AA[6];
extern u8 D_801FB7B0[5][6];
// horse name
extern u8 D_801FB7CE[6];
extern u8 D_801FB7D4[6];
extern u8 D_801FB7DA[5][6];
// baby name
extern u8 D_801FB7F8[6];
extern u8 D_801FB7FE[6];
// pause screen sprite variable values
extern u8 D_801FB804[5];
// year
extern u8 D_801FB809;
// sprite animation mode (animationModeOrFrameIndex)
extern u8 D_801FB80B[5];
// season
extern u8 D_801FB810;
extern u8 D_801FB812[];
// maria affection
extern u8 D_801FB817;
extern u8 D_801FB819[];
// popuri affection
extern u8 D_801FB81E;
extern u8 D_801FB820[];
// elli affction
extern u8 D_801FB825;
extern u8 D_801FB827[];
// ann affection
extern u8 D_801FB82C;
extern u8 D_801FB82E[];
// karen affection
extern u8 D_801FB833;
extern u8 D_801FB835[];
// harris affection
extern u8 D_801FB83A;
extern u8 D_801FB83C[];
// gray affection
extern u8 D_801FB841;
extern u8 D_801FB843[];
// jeff affection
extern u8 D_801FB848;
extern u8 D_801FB84A[];
// cliff affection
extern u8 D_801FB84F;
extern u8 D_801FB851[];
// kai affection
extern u8 D_801FB856;
// sprite animation mode (animationModeOrFrameIndex)
extern u8 D_801FB858[];
// wife affection
extern u8 D_801FB85D;
// sprite animation mode (animationModeOrFrameIndex)
extern u8 D_801FB85F[];
// baby affection
extern u8 D_801FB864;
// sprite animation mode (animationModeOrFrameIndex)
extern u8 D_801FB866[];
// dog affection
extern u8 D_801FB86B;
// sprite animation mode (animationModeOrFrameIndex)
extern u8 D_801FB86D[];
extern u8 D_801FB971[];
// horse affection
extern u8 D_801FB872;
extern u8 D_801FB874[5][8];
// farm animal 1 affection
extern u8 D_801FB89C;
// farm animal 2 affection
extern u8 D_801FB89D;
// farm animal 3 affection
extern u8 D_801FB89E;
// farm animal 4 affection
extern u8 D_801FB89F;
// farm animal 5 affection
extern u8 D_801FB8A0;
// farm animal 6 affection
extern u8 D_801FB8A1;
// farm animal 7 affection
extern u8 D_801FB8A2;
// farm animal 8 affection
extern u8 D_801FB8A3;
extern u8 D_801FB8AC[];
// chickens count
extern u8 D_801FB8B1;
extern u32 D_801FB8B4[5];
// total crops shipped
extern u32 D_801FB8C8;
extern u32 D_801FB8D0[5][3];
// eggs shipped
extern u32 D_801FB8E4;
// milk shipped
extern u32 D_801FB900;
// fish caught
extern u32 D_801FB91C;
// gold
extern u32 D_801FB938;
// grass tiles
extern u16 D_801FB940[];
extern u8 D_801FB94E[];
// max stamina
extern u8 D_801FB953;
// album bits
extern u8 D_801FB955[];
// sprite animation mode (animationModeOrFrameIndex)
extern u8 D_801FB858[];
// recipe count
extern u8 D_801FB95C[];
extern u8 D_801FB85F[];
// house extension count
extern u8 D_801FB963[];
// sprite animation mode (animationModeOrFrameIndex)
extern u8 D_801FB866[];
extern u8 D_801FB96A[];
extern u8 D_801FB86D[];
// happiness
extern u8 D_801FB96F;
// sprite animation mode (animationModeOrFrameIndex)
extern u8 D_801FB971[];
// wife index
extern u8 D_801FB976;
extern f32 D_801FB978[5];
// flags
extern u8 D_801FB994[];
// flags
extern u8 D_801FB999[];
// UI element index
// index into D_801FB994
// param for func_800B7F6C; index into D_80116DC0
// param for func_800E80AC
// param for func_800B7070
extern u8 D_801FB99B[];
// evaluation screen handler mode
extern u8 D_801FB9A0;

// unused
extern u32 D_80204DF0;
extern u32 D_802226E4;

// shared
// turnips shipped
extern u32 D_801654F4;
extern u8 gGlobalSeasonName[6];

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
    
    setGameVariableString(12, loadGameScreenContext.playerNames[3], 6);
    setGameVariableString(13, loadGameScreenContext.dateNumbers[3], 2);
    setGameVariableString(14, loadGameScreenContext.seasonNames[3], 6);
    setGameVariableString(15, loadGameScreenContext.dateEndings[3], 2);
    
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
    
    if (loadGameScreenContext.showControllerPakScreen == FALSE) {
        
        if (arg0) {
            
            func_8003E77C(0, 0, 0, 0, 0);
            func_8003E77C(1, 0, 0, 0, 0);
            func_8003E77C(2, 0, 0, 0, 0);
            func_8003E77C(3, 0, 0, 0, 0);
            func_8003EA1C(0, 0xFF, 0xFF, 0xFF, 0xFF, 24);
            func_8003EA1C(1, 0xFF, 0xFF, 0xFF, 0xFF, 24);
            func_8003EA1C(2, 0xFF, 0xFF, 0xFF, 0xFF, 24);
            func_8003EA1C(3, 0xFF, 0xFF, 0xFF, 0xFF, 24);

        } else {

            func_8003E77C(0, 0, 0, 0, 0);
            func_8003E77C(1, 0, 0, 0, 0);
            func_8003E77C(2, 0, 0, 0, 0);
            func_8003E77C(3, 0, 0, 0, 0);
            func_8003E77C(0, 0xFF, 0xFF, 0xFF, 0xFF);
            func_8003E77C(1, 0xFF, 0xFF, 0xFF, 0xFF);
            func_8003E77C(2, 0xFF, 0xFF, 0xFF, 0xFF);
            func_8003E77C(3, 0xFF, 0xFF, 0xFF, 0xFF);

        }    
        
    } else {

        if (arg0) {
    
            func_8003E77C(0, 0, 0, 0, 0);
            func_8003E77C(1, 0, 0, 0, 0);
            func_8003EA1C(0, 0xFF, 0xFF, 0xFF, 0xFF, 24);
            func_8003EA1C(1, 0xFF, 0xFF, 0xFF, 0xFF, 24);
    
        } else {
    
            func_8003E77C(0, 0, 0, 0, 0);
            func_8003E77C(1, 0, 0, 0, 0);
            func_8003E77C(0, 0xFF, 0xFF, 0xFF, 0xFF);
            func_8003E77C(1, 0xFF, 0xFF, 0xFF, 0xFF);
    
        }

    }

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

        if (loadGameScreenContext.flags[0] & 1) { 
            func_8003DD14(0);
            initializeEmptyMessageBox(0, (u8*)MESSAGE_BOX_1_TEXT_BUFFER);
            setMessageBoxViewSpacePosition(0, 16.0f, 52.0f, 0);
            func_8003F5D0(0, 12, 2);
            func_8003F630(0, 0, 2);
            func_8003F464(0, 14, 14, (u8*)FONT_TEXTURE_BUFFER, (u16*)FONT_PALETTE_1_BUFFER);
            func_8003F360(0, -4, 2);
            setMessageBoxSpriteIndices(0, 0xFF, 0, 0);
            setMessageBoxButtonMask(0, BUTTON_A);
            func_8003FB4C(0, 1);
            initializeMessageBox(MAIN_DIALOGUE_BOX_INDEX, 0, 1, 0x80000);
        }

        if (loadGameScreenContext.flags[1] & 1) {
            func_8003DD14(1);
            initializeEmptyMessageBox(1, (u8*)MESSAGE_BOX_2_TEXT_BUFFER);
            setMessageBoxViewSpacePosition(1, 16.0f, 8.0f, 0);
            func_8003F5D0(1, 12, 2);
            func_8003F630(1, 0, 2);
            func_8003F464(1, 14, 14, (u8*)FONT_TEXTURE_BUFFER, (u16*)FONT_PALETTE_1_BUFFER);
            func_8003F360(1, -4, 2);
            setMessageBoxSpriteIndices(1, 0xFF, 0, 0);
            setMessageBoxButtonMask(1, BUTTON_A);
            func_8003FB4C(1, 1);
            initializeMessageBox(1, 0, 2, 0x80000);
        }

        if (loadGameScreenContext.flags[2] & 1) {
            func_8003DD14(2);
            initializeEmptyMessageBox(2, (u8*)MESSAGE_BOX_3_TEXT_BUFFER);
            setMessageBoxViewSpacePosition(2, 16.0f, -36.0f, 0);
            func_8003F5D0(2, 12, 2);
            func_8003F630(2, 0, 2);
            func_8003F464(2, 14, 14, (u8*)FONT_TEXTURE_BUFFER, (u16*)FONT_PALETTE_1_BUFFER);
            func_8003F360(2, -4, 2);
            setMessageBoxSpriteIndices(2, 0xFF, 0, 0);
            setMessageBoxButtonMask(2, BUTTON_A);
            func_8003FB4C(2, 1);
            initializeMessageBox(2, 0, 3, 0x80000);
        }

        if (loadGameScreenContext.flags[3] & 1) {
            func_8003DD14(3);
            initializeEmptyMessageBox(3, (u8*)MESSAGE_BOX_4_TEXT_BUFFER);
            setMessageBoxViewSpacePosition(3, 16.0f, -80.0f, 0);
            func_8003F5D0(3, 12, 2);
            func_8003F630(3, 0, 2);
            func_8003F464(3, 14, 14, (u8*)FONT_TEXTURE_BUFFER, (u16*)FONT_PALETTE_1_BUFFER);
            func_8003F360(3, -4, 2);
            setMessageBoxSpriteIndices(3, 0xFF, 0, 0);
            setMessageBoxButtonMask(3, BUTTON_A);
            func_8003FB4C(3, 1);
            initializeMessageBox(3, 0, 4, 0x80000);
        }

    } else {

        if (loadGameScreenContext.flags[0] & 1) {
            func_8003DD14(0);
            initializeEmptyMessageBox(0, (u8*)MESSAGE_BOX_1_TEXT_BUFFER);
            setMessageBoxViewSpacePosition(0, 16.0f, 52.0f, 0);
            func_8003F5D0(0, 12, 2);
            func_8003F630(0, 0, 2);
            func_8003F464(0, 14, 14, (u8*)FONT_TEXTURE_BUFFER, (u16*)FONT_PALETTE_1_BUFFER);
            func_8003F360(0, -4, 2);
            setMessageBoxSpriteIndices(0, 0xFF, 0, 0);
            setMessageBoxButtonMask(0, BUTTON_A);
            func_8003FB4C(0, 1);
            initializeMessageBox(MAIN_DIALOGUE_BOX_INDEX, 0, 1, 0x80000);
        }
        
        if (loadGameScreenContext.flags[1] & 1) {
            func_8003DD14(1);
            initializeEmptyMessageBox(1, (u8*)MESSAGE_BOX_2_TEXT_BUFFER);
            setMessageBoxViewSpacePosition(1, 16.0f, 8.0f, 0);
            func_8003F5D0(1, 12, 2);
            func_8003F630(1, 0, 2);
            func_8003F464(1, 14, 14, (u8*)FONT_TEXTURE_BUFFER, (u16*)FONT_PALETTE_1_BUFFER);
            func_8003F360(1, -4, 2);
            setMessageBoxSpriteIndices(1, 0xFF, 0, 0);
            setMessageBoxButtonMask(1, BUTTON_A);
            func_8003FB4C(1, 1);
            initializeMessageBox(1, 0, 2, 0x80000);
        }

    }

}

//INCLUDE_ASM("asm/nonmatchings/game/loadGameScreen", func_800E1FAC);

void func_800E1FAC(void) {

    bool set = FALSE;
    u8 temp;

    switch (loadGameScreenContext.action) {

        // load
        case 0:
            
            if (func_8002CBF8(0x80)) {
                func_800B5FC4(0, loadGameScreenContext.diaryHighlighted, 0xFF);
                loadGameScreenContext.action = 1;
            }
            
            break;

        // default/select
        case 1:

            if (loadGameScreenContext.showControllerPakScreen == FALSE) {
                
                if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_NORTHEAST)) {
                    
                    if (!set) {
                        if (loadGameScreenContext.diaryHighlighted) {
                            loadGameScreenContext.diaryHighlighted--;
                            playSfx(2);
                        }
                        set = TRUE;
                    }
                    
                }
                 
                if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTHWEST)) {
                    
                    if (!set) {

                        if (loadGameScreenContext.diaryHighlighted == 3) {
                            
                            if (loadGameScreenContext.gamePakEnabled) {
                                
                                loadGameScreenContext.showControllerPakScreen = TRUE;
                                set = TRUE;
                                
                                func_800B2CE0();
                                deactivateSprite(0x80);
                                func_8003DD14(0);
                                func_8003DD14(1);
                                func_8003DD14(2);
                                func_8003DD14(3);
                                func_800E16D0(0, 0);
                                
                            } else {
                                set = TRUE;
                            }
                        } else {
                            loadGameScreenContext.diaryHighlighted++;
                            playSfx(2);
                        }
                    
                        set = TRUE;
                    
                    }
                }
                
            } else {
                
                if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_NORTHEAST)) {
                    
                    if (!set) {
                        
                        if (loadGameScreenContext.diaryHighlighted) {
                            loadGameScreenContext.diaryHighlighted--;
                            playSfx(2);
                            set = TRUE;
                        } else {
                            loadGameScreenContext.showControllerPakScreen = FALSE;
                            func_800B2CE0();
                            deactivateSprite(0x80);
                            func_8003DD14(0);
                            func_8003DD14(1);
                            func_8003DD14(2);
                            func_8003DD14(3);
                            func_800E16D0(0, 3);
                            set = TRUE;
                        }
                    }
                }
                
                if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTHWEST)) {
                    if (!set) {
                        if (loadGameScreenContext.diaryHighlighted != 1) {
                            loadGameScreenContext.diaryHighlighted++;
                            playSfx(2);
                        }
                        set = TRUE;
                    }
                }
            }
            
            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTHEAST)) {
                
                if (!set) {
                    if (loadGameScreenContext.action != 2) {
                        loadGameScreenContext.action = 2;
                        playSfx(2);
                    }
                    set = TRUE;
                }
            }
            
            func_800B60E4(0, 0xFF, 0);
            func_800B60E4(1, 0xFF, 0);
            func_800B60E4(2, 0xFF, 0);
            func_800B60E4(3, 0xFF, 0);
            func_800B60E4(0xFF, 0, 0);
            func_800B60E4(0xFF, 1, 0);
            func_800B60E4(0xFF, 2, 0);
            
            func_800B60E4(loadGameScreenContext.diaryHighlighted, 0xFF, 1);
            func_800B5FC4(0, loadGameScreenContext.diaryHighlighted, 0xFF);
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                
                if (!set) {
                    
                    // select diary
                    if (loadGameScreenContext.showControllerPakScreen == FALSE) {
                        func_800B64E4(loadGameScreenContext.diaryHighlighted, 0xFF);
                        deactivateSprite(0x79);
                        func_800C7DF8();
                        func_800C7E18();
                        func_800B5DA8();
                        func_8003EA1C(0, 0, 0, 0, 0, 0x18);
                        func_8003EA1C(1, 0, 0, 0, 0, 0x18);
                        func_8003EA1C(2, 0, 0, 0, 0, 0x18);
                        func_8003EA1C(3, 0, 0, 0, 0, 0x18);
                        loadGameScreenContext.action = 6;
                        playSfx(0);
                        stopAudioSequenceWithDefaultFadeOut(1);
                    } 
                    
                    set = TRUE;
                
                }
            }
            
            if ((checkButtonPressed(CONTROLLER_1, BUTTON_B)) && !set) {
                
                deactivateSprite(0x79);
                func_800C7DF8();
                func_800C7E18();
                func_800B5DA8();
                func_8003EA1C(0, 0, 0, 0, 0, 0x18);
                func_8003EA1C(1, 0, 0, 0, 0, 0x18);
                func_8003EA1C(2, 0, 0, 0, 0, 0x18);
                func_8003EA1C(3, 0, 0, 0, 0, 0x18);
                loadGameScreenContext.action = 7;
                stopAudioSequenceWithDefaultFadeOut(1);
                playSfx(1);
                
            }
    
            break;

        case 2:
            
            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_NORTHEAST)) {
                if (!set) {
                    if (loadGameScreenContext.actionColumnHighlighted) {
                        loadGameScreenContext.actionColumnHighlighted--;
                        playSfx(2);
                    }
                    set = TRUE;
                }
            }
            
            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTHWEST)) {
                if (!set) {
                    if (loadGameScreenContext.actionColumnHighlighted != 1) {
                        loadGameScreenContext.actionColumnHighlighted++;
                        playSfx(2);
                    }
                    set = TRUE;
                }
            }
            
            if (checkButtonRepeat(CONTROLLER_1, 0x40000)) {
                if (!set) {
                    if (loadGameScreenContext.action != 1) {
                        loadGameScreenContext.action = 1;
                        playSfx(2);
                    }
                    set = TRUE;
                }
            }
            
            func_800B60E4(0, 0xFF, 0);
            func_800B60E4(1, 0xFF, 0);
            func_800B60E4(2, 0xFF, 0);
            func_800B60E4(3, 0xFF, 0);
            func_800B60E4(0xFF, 0, 0);
            func_800B60E4(0xFF, 1, 0);
            func_800B60E4(0xFF, 2, 0);
            
            func_800B60E4(0xFF, loadGameScreenContext.actionColumnHighlighted, 1);
            func_800B5FC4(0, 0xFF, loadGameScreenContext.actionColumnHighlighted);
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                
                if (!set) {
                    
                    func_800B64E4(0xFF, loadGameScreenContext.actionColumnHighlighted);
                    
                    switch (loadGameScreenContext.actionColumnHighlighted) {                
                        case 0:                          
                            // delete   
                            loadGameScreenContext.action = 3;
                            break;
                        case 1:                            
                            // move 
                            loadGameScreenContext.action = 4;
                            break;
                        case 2:                             
                            deactivateSprite(0x79);
                            func_800C7DF8();
                            func_800C7E18();
                            func_800B5DA8();
                            func_8003EA1C(0, 0, 0, 0, 0, 0x18);
                            func_8003EA1C(1, 0, 0, 0, 0, 0x18);
                            func_8003EA1C(2, 0, 0, 0, 0, 0x18);
                            func_8003EA1C(3, 0, 0, 0, 0, 0x18);
                            // load completion screen
                            loadGameScreenContext.action = 10;
                            playSfx(0);
                            break;
                    }
                    
                    playSfx(0);
                    set = TRUE;
                
                }
            }
            
            if ((checkButtonPressed(CONTROLLER_1, BUTTON_B)) && !set) {
                deactivateSprite(0x79);
                func_800C7DF8();
                func_800C7E18();
                func_800B5DA8();
                func_8003EA1C(0, 0, 0, 0, 0, 0x18);
                func_8003EA1C(1, 0, 0, 0, 0, 0x18);
                func_8003EA1C(2, 0, 0, 0, 0, 0x18);
                func_8003EA1C(3, 0, 0, 0, 0, 0x18);
                loadGameScreenContext.action = 7;
                stopAudioSequenceWithDefaultFadeOut(1);
                playSfx(1);
            }
            
            break;

        // select diary to delete
        case 3:

            if (loadGameScreenContext.showControllerPakScreen == FALSE) {

                if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_NORTHEAST)) {
                    if (!set) {
                        if (loadGameScreenContext.diaryHighlighted) {
                            loadGameScreenContext.diaryHighlighted--;
                            playSfx(2);
                        }
                        set = TRUE;
                    }
                }

                if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTHWEST)) {
                    
                    if (!set) {
                        
                        if (loadGameScreenContext.diaryHighlighted == 3) {
                            
                            if (loadGameScreenContext.gamePakEnabled) {
                                loadGameScreenContext.showControllerPakScreen = TRUE;
                                set = TRUE;
                                func_800B2CE0();
                                deactivateSprite(0x80);
                                func_8003DD14(0);
                                func_8003DD14(1);
                                func_8003DD14(2);
                                func_8003DD14(3);
                                func_800E16D0(0, 0);
                            } else {
                                set = TRUE;
                            }
                        } else {
                            loadGameScreenContext.diaryHighlighted++;
                            playSfx(2);
                        }
                        
                        set = TRUE;
                        
                    }
                }
                
            } else {
        
                if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_NORTHEAST)) {
                    
                    if (!set) {
                        if (loadGameScreenContext.diaryHighlighted) {
                            loadGameScreenContext.diaryHighlighted--;
                            playSfx(2);
                            set = TRUE;
                        } else {
                            loadGameScreenContext.showControllerPakScreen = FALSE;
                            func_800B2CE0();
                            deactivateSprite(0x80);
                            func_8003DD14(0);
                            func_8003DD14(1);
                            func_8003DD14(2);
                            func_8003DD14(3);
                            func_800E16D0(0, 3);
                            set = TRUE;
                        }
                    }
                }
            
                if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTHWEST)) {
                    
                    if (!set) {
    
                        if (loadGameScreenContext.diaryHighlighted != 1) {
                            loadGameScreenContext.diaryHighlighted++;
                            playSfx(2);
                        }
                        set = TRUE;
                    }
                }
                
            }
    
            func_800B60E4(0, 0xFF, 0);
            func_800B60E4(1, 0xFF, 0);
            func_800B60E4(2, 0xFF, 0);
            func_800B60E4(3, 0xFF, 0);
            func_800B60E4(loadGameScreenContext.diaryHighlighted, 0xFF, 1);
            func_800B5FC4(0, loadGameScreenContext.diaryHighlighted, 0xFF);

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                if (!set) {
                    if (loadGameScreenContext.flags[loadGameScreenContext.diaryHighlighted] & 2) {
                        startSpriteAnimation(0xAC, 2, 0);
                        loadGameScreenContext.action = 8;
                        playSfx(0);
                    }
                    set = TRUE;
                }
            }
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_B)) {
                if (!set) {
                    loadGameScreenContext.action = 2;
                    playSfx(1);
                }
            }
            
            break;

        // select diary to move
        case 4:

            if (loadGameScreenContext.showControllerPakScreen == FALSE) {

                if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_NORTHEAST)) {
                    if (!set) {
                        if (loadGameScreenContext.diaryHighlighted) {
                            loadGameScreenContext.diaryHighlighted--;
                            playSfx(2);
                        }
                        set = TRUE;
                    }
                }

                if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTHWEST)) {
                    
                    if (!set) {
                    
                        if ((loadGameScreenContext.diaryHighlighted) == 3) {
                            if (loadGameScreenContext.gamePakEnabled) {
                                loadGameScreenContext.showControllerPakScreen = TRUE;
                                set = TRUE;
                                // show extra game pak diaries
                                func_800B2CE0();
                                deactivateSprite(0x80);
                                func_8003DD14(0);
                                func_8003DD14(1);
                                func_8003DD14(2);
                                func_8003DD14(3);
                                func_800E16D0(0, 0);
                            } else {
                                set = TRUE;
                            }
                        } else {
                            loadGameScreenContext.diaryHighlighted++;
                            playSfx(2);
                        }

                        set = TRUE;
                        
                    }
                }
                
            } else {

                if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_NORTHEAST)) {
                    
                    if (!set) {
                        
                        if (loadGameScreenContext.diaryHighlighted) {
                            loadGameScreenContext.diaryHighlighted--;
                            playSfx(2);
                            set = TRUE;
                        } else {
                            loadGameScreenContext.showControllerPakScreen = FALSE;
                            func_800B2CE0();
                            deactivateSprite(0x80);
                            func_8003DD14(0);
                            func_8003DD14(1);
                            func_8003DD14(2);
                            func_8003DD14(3);
                            func_800E16D0(0, 3);
                            set = TRUE;
                        }
                    }
                    
                }

                if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTHWEST)) {
                    
                    if (!set) {
                        if (loadGameScreenContext.diaryHighlighted != 1) {
                            loadGameScreenContext.diaryHighlighted++;
                            playSfx(2);
                        }
                        set = TRUE;
                    }
                    
                }
                
            }

            func_800B60E4(0, 0xFF, 0);
            func_800B60E4(1, 0xFF, 0);
            func_800B60E4(2, 0xFF, 0);
            func_800B60E4(3, 0xFF, 0);
            func_800B60E4(loadGameScreenContext.diaryHighlighted, 0xFF, 1);
            func_800B5FC4(0, loadGameScreenContext.diaryHighlighted, 0xFF);
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                
                if (!set) {
                    
                    if (loadGameScreenContext.flags[loadGameScreenContext.diaryHighlighted] & 2) {
                        func_800B5FC4(1, loadGameScreenContext.diaryHighlighted, 0xFF);
                        // move/copy to other diary
                        loadGameScreenContext.action = 5;
                        loadGameScreenContext.unk_86 = loadGameScreenContext.diaryHighlighted;
                        loadGameScreenContext.unk_88 = loadGameScreenContext.showControllerPakScreen;
                        playSfx(0);
                    
                    }
                    
                    set = TRUE;
                    
                }
                
            }
                
            if (checkButtonPressed(CONTROLLER_1, BUTTON_B)) {
                
                if (!set) {
                    loadGameScreenContext.action = 2;
                    playSfx(1);
                }
                
            }
                
            break;

        // select move to destination
        case 5:

            if (loadGameScreenContext.showControllerPakScreen == FALSE) {
                
                if (loadGameScreenContext.unk_88 == FALSE) {
                    func_800B5FC4(1, loadGameScreenContext.unk_86, 0xFF);
                }
                
                if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_NORTHEAST)) {
                    
                    if (!set) {
                        if (loadGameScreenContext.diaryHighlighted) {
                            loadGameScreenContext.diaryHighlighted--;
                            playSfx(2);
                        }
                        set = TRUE;
                    }

                }
                
                if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTHWEST)) {
                    
                    if (!set) {

                        if (loadGameScreenContext.diaryHighlighted == 3) {
                        
                            if (loadGameScreenContext.gamePakEnabled) {
                                loadGameScreenContext.showControllerPakScreen = TRUE;
                                set = TRUE;
                                func_800B2CE0();
                                deactivateSprite(0x80);
                                func_8003DD14(0);
                                func_8003DD14(1);
                                func_8003DD14(2);
                                func_8003DD14(3);
                                func_800E16D0(0, 0);
                            } else {
                                set = TRUE;
                            }

                        } else {
                            loadGameScreenContext.diaryHighlighted++;
                            playSfx(2);
                        }
                        
                        set = TRUE;
                        
                    }
                }

            } else {
                
                if (loadGameScreenContext.showControllerPakScreen == loadGameScreenContext.unk_88) {
                    func_800B5FC4(1, loadGameScreenContext.unk_86, 0xFF);
                }
                
                if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_NORTHEAST)) {
                    
                    if (!set) {
                    
                        if (loadGameScreenContext.diaryHighlighted) {
                            loadGameScreenContext.diaryHighlighted--;
                            playSfx(2);
                            set = TRUE;
                        } else {
                            loadGameScreenContext.showControllerPakScreen = FALSE;
                            func_800B2CE0();
                            deactivateSprite(0x80);
                            func_8003DD14(0);
                            func_8003DD14(1);
                            func_8003DD14(2);
                            func_8003DD14(3);
                            func_800E16D0(0, 3);
                            set = TRUE;
                        }
                    
                    }
                }
            
                if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTHWEST)) {
                    if (!set) {
                        if (loadGameScreenContext.diaryHighlighted != 1) {
                            loadGameScreenContext.diaryHighlighted++;
                            playSfx(2);
                        }
                        set = TRUE;
                    }
                }
            
            }
            
            func_800B60E4(0, 0xFF, 0);
            func_800B60E4(1, 0xFF, 0);
            func_800B60E4(2, 0xFF, 0);
            func_800B60E4(3, 0xFF, 0);
            func_800B60E4(loadGameScreenContext.diaryHighlighted, 0xFF, 1);
            func_800B5FC4(0, loadGameScreenContext.diaryHighlighted, 0xFF);
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                
                if (!set) {
                    
                    if (loadGameScreenContext.flags[loadGameScreenContext.diaryHighlighted] & 1) {
                        startSpriteAnimation(0xAC, 2, 0);
                        loadGameScreenContext.action = 9;
                        playSfx(0);
                    }
                    set = TRUE;
                }
            }
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_B) && !set) {
                resetAnimationState(0xB2);
                loadGameScreenContext.action = 2;
                playSfx(1);
            }

            break;

        // load selected diary
        case 6:
            
            if (func_8002CBF8(0x80) && checkDefaultSequenceChannelOpen(1)) {
                
                func_800B2CE0();
                deactivateSprite(0x80);
                func_8003DD14(0);
                func_8003DD14(1);
                func_8003DD14(2);
                func_8003DD14(3);
                
                temp = loadGameScreenContext.diaryHighlighted;
                gCurrentGameIndex = temp;
                
                func_80053088();
                
                // start existing game
                if (loadGameScreenContext.flags[gCurrentGameIndex] & 2) {
                    // set game state from sram
                    func_800E4424(gCurrentGameIndex, 0);
                    startGame();
                } else {
                    // start new game
                    initializeNewGameState();
                    initializeGameVariables();
                    initializeNamingScreen(gPlayer.name, 0);
                }
            }
            
            break;
        
        // go back to title
        case 7:
            
            if (func_8002CBF8(0x80) && checkDefaultSequenceChannelOpen(1)) {
                func_800B2CE0();
                deactivateSprite(0x80);
                func_8003DD14(0);
                func_8003DD14(1);
                func_8003DD14(2);
                func_8003DD14(3);
                initializeTitleScreen(1);
            }
            
            break;

        // perform delete
        case 8:
            
            func_800B5FC4(0, 0xFF, 3);
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A) && !set) {
                resetAnimationState(0xAC);
                set = TRUE;
                func_800E67E4(loadGameScreenContext.diaryHighlighted, loadGameScreenContext.showControllerPakScreen);
                func_800E1998();
                loadGameScreenContext.action = 2;
                playSfx(0);
            }
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_B) && !set) {
                resetAnimationState(0xAC);
                loadGameScreenContext.action = 3;
                playSfx(1);
            }
            
            break;

        // perform move
        case 9:
            
            func_800B5FC4(0, 0xFF, 3);
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A) && !set) {
                
                resetAnimationState(0xAC);
                resetAnimationState(0xB2);
                
                set = TRUE;
                func_800E66A0(loadGameScreenContext.unk_86, loadGameScreenContext.unk_88, loadGameScreenContext.diaryHighlighted, loadGameScreenContext.showControllerPakScreen);
                func_800E1998();
                loadGameScreenContext.action = 2;
                playSfx(0);

            }
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_B) && !set) {
                resetAnimationState(0xAC);
                loadGameScreenContext.action = 5;
                playSfx(1);
            }
            
            break;
            
        // completion screen
        case 10:
            
            if (func_8002CBF8(0x80)) {

                // deactivate overlay screen sprites
                func_800B2CE0();
                deactivateSprite(0x80);

                // message boxes
                func_8003DD14(0);
                func_8003DD14(1);
                func_8003DD14(2);
                func_8003DD14(3);

                loadCompletionScreen();

            }
            
            break;
            
        // controller pak load
        case 11:
            
            if (func_8002CBF8(0x80)) {
                func_800B5FC4(0, 0xFF, loadGameScreenContext.actionColumnHighlighted);
                loadGameScreenContext.action = 2; 
            }
            
            break;
            
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/loadGameScreen", func_800E3300);

// verify signature
inline bool func_800E3300(u8 arg0[]) {

    u8 i = 0;
    bool result = 0;

    if (arg0[0] == D_80119740[0]) {

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
    
                loadGameScreenContext.flags[arg0] = 3;

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
                
                loadGameScreenContext.flags[arg0] = 1;
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
            
            loadGameScreenContext.flags[arg0] = 1;
            
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
            
            loadGameScreenContext.flags[arg0] = 0;

            break;       

    }

}

//INCLUDE_ASM("asm/nonmatchings/game/loadGameScreen", func_800E38E8);

// calculate
inline u16 func_800E38E8(u8 arg0[]) {

    u16 result = 0;
    u16 i = 0;

    do {

        result += arg0[i];
        i++;
        
    } while (i < 4094);

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/loadGameScreen", func_800E391C);

// verify checksum
inline u16 func_800E391C(u8* arg0) {

    u16 result = 0;
    u16 i = 0;

    do {

        result += arg0[i];
        i++;
        
    } while (i < 4094);

    return result == *(u16*)(arg0 + 4094);

}

//INCLUDE_ASM("asm/nonmatchings/game/loadGameScreen", func_800E395C);

// calculate checksum
inline u16 func_800E395C(u8 arg0[]) {

    u16 result = 0;
    u16 i = 0;

    do {

        result += arg0[i];
        i++;
        
    } while (i < 254);

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/loadGameScreen", func_800E3990);

// verify checksum
inline u16 func_800E3990(u8 arg0[]) {

    u16 result = 0;
    u16 i = 0;

    do {

        result += arg0[i];
        i++;
        
    } while (i < 254);

    return result == *(u16*)(arg0 + 254);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/loadGameScreen", initializeNewGameState);

void initializeNewGameState(void) {

    u8 i, j;
    
    gYear = 0;
    gSeason = 0;
    gDayOfWeek = 0;
    gDayOfMonth = 0;
    
    gPlayer.name[0] = 0;
    gPlayer.name[1] = 0;
    gPlayer.name[2] = 0;
    gPlayer.name[3] = 0;
    gPlayer.name[4] = 0;
    gPlayer.name[5] = 0;
    
    gFarmName[0] = 0;
    gFarmName[1] = 0;
    gFarmName[2] = 0;
    gFarmName[3] = 0;
    gFarmName[4] = 0;
    gFarmName[5] = 0;
    
    gGold = 0;
    gWeather = 0;
    gForecast = 0;
    gPlayerBirthdaySeason = 0;
    wateringCanUses = 0;
    gLumber = 0;
    fodderQuantity = 0;
    gSeasonTomorrow = 0;
    
    D_801FC154 = 0;
    D_80204DF4 = 0;
    D_80237458 = 0;
    D_8018A725 = 0;
    D_802373E8 = 0;
    D_8013DC2C = 0;
    D_801FAD91 = 0;
    D_801C3E28 = 0;
    D_801C3F70 = 0;
    D_80205636 = 0;
    
    gHarvestKing = 6;

    D_802373A8 = 0;
    
    gMaximumStamina = 0;
    gPlayer.currentStamina = 0;
    gPlayer.fatigueCounter = 0;
    gPlayer.staminaExhaustionLevel = 0;
    
    gAlcoholTolerance = 0;
    
    gHouseExtensionSelection = 0;
    houseExtensionConstructionCounter = 0;
    
    chickenFeedQuantity = 0;
    D_8021E6D0 = 0;
    
    gHarvestCoinFinder = 0;
    gFlowerFestivalGoddess = 0;
    D_80189054 = 0;
    gVoteForFlowerFestivalGoddess = 0;
    
    deadAnimalName[0] = 0;
    deadAnimalName[1] = 0;
    deadAnimalName[2] = 0;
    deadAnimalName[3] = 0;
    deadAnimalName[4] = 0;
    deadAnimalName[5] = 0;

    gHappiness = 0;
    blueMistFlowerPlot = 0;
    anniversarySeason = 0;
    anniversaryDate = 0;
    babyBirthdaySeason = 0;
    babyBirthdayDate = 0;
    
    powerNutBits = 0;
    D_80205204 = 0;
    flowerShopPoints = 0;
    bakeryCardPoints = 0;
    
    gElliGrievingCounter = 0;
    
    D_801654F4 = 0;
    D_80237414 = 0;
    D_801FB6FC = 0;
    D_801C3F80 = 0;
    D_801806C0 = 0;
    D_80188F60 = 0;
    D_801FB5D0 = 0;
    
    gTotalMilkShipped = 0;
    gTotalEggsShipped = 0;
    gTotalCropsShipped = 0;
    totalFishCaught = 0;
    
    gBabyName[0] = 0;
    gBabyName[1] = 0;
    gBabyName[2] = 0;
    gBabyName[3] = 0;
    gBabyName[4] = 0;
    gBabyName[5] = 0;
    
    gSickDays = 0;
    D_8013DC2E = 0;
    
    mrsManaCow1Index = 0;
    mrsManaCow2Index = 0;
    mrsManaCow3Index = 0;

    gPlayer.toolUseCounters[0] = 0;
    gPlayer.toolUseCounters[1] = 0;
    gPlayer.toolUseCounters[2] = 0;
    gPlayer.toolUseCounters[3] = 0;
    gPlayer.toolUseCounters[4] = 0;

    gPlayer.toolLevels[0] = 0;
    gPlayer.toolLevels[1] = 0;
    gPlayer.toolLevels[2] = 0;
    gPlayer.toolLevels[3] = 0;
    gPlayer.toolLevels[4] = 0;
    
    gPlayer.bottleContents = 0;

    tvContext.channelIndex = 0;
    tvContext.varietyShowEpisodeCounters[0] = 0;
    tvContext.varietyShowEpisodeCounters[1] = 0;
    tvContext.varietyShowEpisodeCounters[2] = 0;
    tvContext.varietyShowEpisodeCounters[3] = 0;
    tvContext.varietyShowEpisodeCounters[4] = 0;
    tvContext.varietyShowEpisodeCounters[5] = 0;
    tvContext.varietyShowEpisodeCounters[6] = 0;

    recipesBits[0] = 0;
    D_8016FB00 = 0;
    albumBits = 0;
    
    bornChickenIndex = 0;
    bornAnimalIndex = 0;
    
    gWife = 0xFF;
    gBabyAge = 0;
    
    gWifeConceptionCounter = 0;
    gWifePregnancyCounter = 0;
    mariaHarrisBabyAge = 0;
    mariaHarrisNewlywedCounter = 0;
    mariaHarrisPregnancyCounter = 0;
    popuriGrayBabyAge = 0;
    popuriGrayNewlywedCounter = 0;
    popuriGrayPregnancyCounter = 0;
    elliJeffBabyAge = 0;
    elliJeffNewlywedCounter = 0;
    elliJeffPregnancyCounter = 0;
    annCliffBabyAge = 0;
    annCliffNewlywedCounter = 0;
    annPregnancyCounter = 0;
    karenKaiBabyAge = 0;
    karenKaiNewlywedCounter = 0;
    karenPregnancyCounter = 0;
    
    dailyShippingBinValue = 0;
    
    readMailBits[0] = 0;
    
    D_801C3F3C = 0;
    D_801C3F40 = 0;
    
    mailboxBits[0] = 0;
    
    D_8016FFF0 = 0;
    D_8016FFF4 = 0;

    for (j = 0; j < 32; j++) {
        gToolchestSlots[j] = 0;
    }

    for (j = 0; j < 32; j++) {
        D_80237420[j] = 0;
    }

    for (j = 0; j < 32; j++) {
        D_801890E8[j] = 0;
    }
    
    for (j = 0; j < 9; j++) {
        gPlayer.belongingsSlots[j] = 0;
    }

    for (j = 0; j < 9; j++) {
        gPlayer.toolSlots[j] = 0;
    }

    for (j = 0; j < 24; j++) {
        gPlayer.keyItemSlots[j] = 0;
    }

    for (j = 0; j < MAX_NPCS; j++) {
        npcAffection[j] = 0;
    }

    for (i = 0; i < 5; i++) {
        for (j = 0; j < 7; j++) {
            D_80189108[i][j] = 0;
        }
    }

    dogInfo.name[0] = 0;
    dogInfo.name[1] = 0;
    dogInfo.name[2] = 0;
    dogInfo.name[3] = 0;
    dogInfo.name[4] = 0;
    dogInfo.name[5] = 0;
    dogInfo.affection = 0;
    dogInfo.location = 0;
    dogInfo.coordinates.x = 0;
    dogInfo.coordinates.z = 0;
    dogInfo.unk_1C = 0;

    horseInfo.name[0] = 0;
    horseInfo.name[1] = 0;
    horseInfo.name[2] = 0;
    horseInfo.name[3] = 0;
    horseInfo.name[4] = 0;
    horseInfo.name[5] = 0;
    horseInfo.affection = 0;
    horseInfo.location = 0;
    horseInfo.coordinates.x = 0;
    horseInfo.coordinates.z = 0;
    horseInfo.grown = 0;
    horseInfo.age = 0;
    horseInfo.unk_1E = 0;
    horseInfo.flags = 0;

    for (j = 0; j < MAX_CHICKENS; j++) {
        
        gChickens[j].name[0] = 0;
        gChickens[j].name[1] = 0;
        gChickens[j].name[2] = 0;
        gChickens[j].name[3] = 0;
        gChickens[j].name[4] = 0;
        gChickens[j].name[5] = 0;
        
        gChickens[j].location = 0;

        gChickens[j].coordinates.x = 0;
        gChickens[j].coordinates.z = 0;
        
        gChickens[j].type = 0;
        gChickens[j].condition = 0;
        gChickens[j].typeCounter = 0;
        gChickens[j].conditionCounter = 0;
        gChickens[j].flags = 0;

    }

    for (j = 0; j < MAX_FARM_ANIMALS; j++) {

        gFarmAnimals[j].name[0] = 0;
        gFarmAnimals[j].name[1] = 0;
        gFarmAnimals[j].name[2] = 0;
        gFarmAnimals[j].name[3] = 0;
        gFarmAnimals[j].name[4] = 0;
        gFarmAnimals[j].name[5] = 0;

        gFarmAnimals[j].affection = 0;
        gFarmAnimals[j].location = 0;

        gFarmAnimals[j].coordinates.x = 0;
        gFarmAnimals[j].coordinates.z = 0;
                
        gFarmAnimals[j].type = 0;
        gFarmAnimals[j].condition = 0;
        gFarmAnimals[j].typeCounter = 0;
        gFarmAnimals[j].conditionCounter = 0;
        gFarmAnimals[j].flags = 0;

        gFarmAnimals[j].unk_23[0] = 0;
        gFarmAnimals[j].unk_23[1] = 0;
        gFarmAnimals[j].unk_23[2] = 0;
        gFarmAnimals[j].unk_23[3] = 0;
        gFarmAnimals[j].unk_23[4] = 0;
        gFarmAnimals[j].unk_23[5] = 0;
        gFarmAnimals[j].birthdaySeason = 0;
        gFarmAnimals[j].birthdayDayOfMonth = 0;
        gFarmAnimals[j].normalMilk = 0;
        
    }
    
    for (i = 0; i < FIELD_HEIGHT; i++) {
        for (j = 0; j < FIELD_WIDTH; j++) {
            farmFieldTiles[i][j] = 0;
        }
    }

    for (i = 0; i < FIELD_HEIGHT; i++) {
        for (j = 0; j < FIELD_WIDTH; j++) {
            greenhouseFieldTiles[i][j] = 0;
        }
    }

    lifeEventBits[0] = 0;
    lifeEventBits[1] = 0;
    lifeEventBits[2] = 0;
    lifeEventBits[3] = 0;
    lifeEventBits[4] = 0;
    lifeEventBits[5] = 0;
    lifeEventBits[6] = 0;
    lifeEventBits[7] = 0;
    lifeEventBits[8] = 0;
    lifeEventBits[9] = 0;
    lifeEventBits[10] = 0;
    lifeEventBits[11] = 0;
    lifeEventBits[12] = 0;
    lifeEventBits[13] = 0;
    lifeEventBits[14] = 0;
    lifeEventBits[15] = 0;

    specialDialogueBits[0] = 0;
    specialDialogueBits[1] = 0;
    specialDialogueBits[2] = 0;
    specialDialogueBits[3] = 0;
    specialDialogueBits[4] = 0;
    specialDialogueBits[5] = 0;
    specialDialogueBits[6] = 0;
    specialDialogueBits[7] = 0;
    specialDialogueBits[8] = 0;
    specialDialogueBits[9] = 0;
    specialDialogueBits[10] = 0;
    specialDialogueBits[11] = 0;
    specialDialogueBits[12] = 0;
    specialDialogueBits[13] = 0;
    specialDialogueBits[14] = 0;
    specialDialogueBits[15] = 0;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/loadGameScreen", func_800E4424);

// set game state globals from sram
bool func_800E4424(u8 saveSlot, u8 arg1) {

    u32 devAddr;
    u32 vaddr;
    u8 signatureValid;
    u8 checksumValid;
    u16 checksum;
    SramBuffer* buff;
    
    u8 i, j;
    u8 row, col;
    u16 idx;
    
    bool result;

    // Get SRAM address (each save is 0x1000 bytes)
    devAddr = (saveSlot << 12) + 0x08000000;
    vaddr = SRAM_BUFFER;
    buff = (SramBuffer*)vaddr;

    if (arg1 == 0) {
        func_8004DC48(devAddr, vaddr, 0x1000);
    } else {
        func_800E16A0();
        if (loadGameScreenContext.gamePakEnabled) {
            func_8004D9AC(CONTROLLER_1,
                &loadGameScreenContext.unk_0[(saveSlot * 4 + saveSlot) * 4 + saveSlot],
                &loadGameScreenContext.unk_15[(saveSlot * 4 + saveSlot) * 4 + saveSlot - 5],
                0, 0x1000, vaddr);
        }
    }

    gYear = buff->year;
    gSeason = buff->season;
    gDayOfWeek = buff->dayOfWeek;
    gDayOfMonth = buff->dayOfMonth;

    gPlayer.name[0] = buff->playerName[0];
    gPlayer.name[1] = buff->playerName[1];
    gPlayer.name[2] = buff->playerName[2];
    gPlayer.name[3] = buff->playerName[3];
    gPlayer.name[4] = buff->playerName[4];
    gPlayer.name[5] = buff->playerName[5];

    gFarmName[0] = buff->farmName[0];
    gFarmName[1] = buff->farmName[1];
    gFarmName[2] = buff->farmName[2];
    gFarmName[3] = buff->farmName[3];
    gFarmName[4] = buff->farmName[4];
    gFarmName[5] = buff->farmName[5];

    gGold = buff->gold;
    gWeather = buff->weather;
    gForecast = buff->forecast;
    gPlayerBirthdaySeason = buff->playerBirthdaySeason;
    wateringCanUses = buff->wateringCanUses;
    gLumber = buff->lumber;
    fodderQuantity = buff->fodderQuantity;
    gSeasonTomorrow = buff->seasonTomorrow;

    D_801FC154 = buff->unk_2D;
    D_80204DF4 = buff->unk_2E;
    D_80237458 = buff->unk_2F;
    D_8018A725 = buff->unk_30;
    D_802373E8 = buff->unk_31;
    D_8013DC2C = buff->unk_32;
    D_801FAD91 = buff->unk_33;
    D_801C3E28 = buff->unk_34;
    D_801C3F70 = buff->unk_35;
    D_80205636 = buff->unk_36;
    D_802373A8 = buff->unk_37;

    gMaximumStamina = buff->maximumStamina;
    gPlayer.currentStamina = buff->currentStamina;
    gPlayer.fatigueCounter = buff->fatigueCounter;
    gPlayer.staminaExhaustionLevel = buff->staminaExhaustionLevel;

    gAlcoholTolerance = buff->alcoholTolerance;
    gHouseExtensionSelection = buff->houseExtensionSelection;
    houseExtensionConstructionCounter = buff->houseExtensionConstructionCounter;
    chickenFeedQuantity = buff->chickenFeedQuantity;
    D_8021E6D0 = buff->unk_42;

    gHarvestKing = buff->harvestKing;
    gHarvestCoinFinder = buff->harvestCoinFinder;
    gFlowerFestivalGoddess = buff->flowerFestivalGoddess;
    D_80189054 = buff->cowFestivalStall;
    gVoteForFlowerFestivalGoddess = buff->voteForFlowerFestivalGoddess;

    deadAnimalName[0] = buff->deadAnimalName[0];
    deadAnimalName[1] = buff->deadAnimalName[1];
    deadAnimalName[2] = buff->deadAnimalName[2];
    deadAnimalName[3] = buff->deadAnimalName[3];
    deadAnimalName[4] = buff->deadAnimalName[4];
    deadAnimalName[5] = buff->deadAnimalName[5];

    gHappiness = buff->happiness;
    blueMistFlowerPlot = buff->blueMistFlowerPlot;
    anniversarySeason = buff->anniversarySeason;
    anniversaryDate = buff->anniversaryDate;
    babyBirthdaySeason = buff->babyBirthdaySeason;
    babyBirthdayDate = buff->babyBirthdayDate;

    powerNutBits = buff->powerNutBits;
    D_80205204 = buff->unk_58;
    flowerShopPoints = buff->flowerShopPoints;
    bakeryCardPoints = buff->bakeryCardPoints;
    gElliGrievingCounter = buff->elliGrievingCounter;

    D_801654F4 = buff->unk_60;
    D_80237414 = buff->unk_64;
    D_801FB6FC = buff->unk_68;
    D_801C3F80 = buff->unk_6C;
    D_801806C0 = buff->unk_70;
    D_80188F60 = buff->unk_74;
    D_801FB5D0 = buff->unk_78;

    gTotalMilkShipped = buff->totalMilkShipped;
    gTotalEggsShipped = buff->totalEggsShipped;
    gTotalCropsShipped = buff->totalCropsShipped;
    totalFishCaught = buff->totalFishCaught;

    gBabyName[0] = buff->babyName[0];
    gBabyName[1] = buff->babyName[1];
    gBabyName[2] = buff->babyName[2];
    gBabyName[3] = buff->babyName[3];
    gBabyName[4] = buff->babyName[4];
    gBabyName[5] = buff->babyName[5];

    gSickDays = buff->sickDays;
    D_8013DC2E = buff->deadAnimalCount;

    mrsManaCow1Index = buff->mrsManaCow1Index;
    mrsManaCow2Index = buff->mrsManaCow2Index;
    mrsManaCow3Index = buff->mrsManaCow3Index;

    gPlayer.toolUseCounters[0] = buff->toolUseCounters[0];
    gPlayer.toolUseCounters[1] = buff->toolUseCounters[1];
    gPlayer.toolUseCounters[2] = buff->toolUseCounters[2];
    gPlayer.toolUseCounters[3] = buff->toolUseCounters[3];
    gPlayer.toolUseCounters[4] = buff->toolUseCounters[4];

    gPlayer.toolLevels[0] = buff->toolLevels[0];
    gPlayer.toolLevels[1] = buff->toolLevels[1];
    gPlayer.toolLevels[2] = buff->toolLevels[2];
    gPlayer.toolLevels[3] = buff->toolLevels[3];
    gPlayer.toolLevels[4] = buff->toolLevels[4];

    gPlayer.bottleContents = buff->bottleContents;

    tvContext.channelIndex = buff->tvChannelIndex;
    tvContext.varietyShowEpisodeCounters[0] = buff->varietyShowEpisodeCounters[0];
    tvContext.varietyShowEpisodeCounters[1] = buff->varietyShowEpisodeCounters[1];
    tvContext.varietyShowEpisodeCounters[2] = buff->varietyShowEpisodeCounters[2];
    tvContext.varietyShowEpisodeCounters[3] = buff->varietyShowEpisodeCounters[3];
    tvContext.varietyShowEpisodeCounters[4] = buff->varietyShowEpisodeCounters[4];
    tvContext.varietyShowEpisodeCounters[5] = buff->varietyShowEpisodeCounters[5];
    tvContext.varietyShowEpisodeCounters[6] = buff->varietyShowEpisodeCounters[6];

    recipesBits[0] = buff->recipesBits;
    D_8016FB00 = buff->unk_DC;
    albumBits = buff->albumBits;

    bornChickenIndex = buff->bornChickenIndex;
    bornAnimalIndex = buff->bornAnimalIndex;

    gWife = buff->wife;
    gBabyAge = buff->babyAge;
    gWifeConceptionCounter = buff->wifeConceptionCounter;
    gWifePregnancyCounter = buff->wifePregnancyCounter;

    mariaHarrisBabyAge = buff->mariaHarrisBabyAge;
    mariaHarrisNewlywedCounter = buff->mariaHarrisNewlywedCounter;
    mariaHarrisPregnancyCounter = buff->mariaHarrisPregnancyCounter;
    popuriGrayBabyAge = buff->popuriGrayBabyAge;
    popuriGrayNewlywedCounter = buff->popuriGrayNewlywedCounter;
    popuriGrayPregnancyCounter = buff->popuriGrayPregnancyCounter;
    elliJeffBabyAge = buff->elliJeffBabyAge;
    elliJeffNewlywedCounter = buff->elliJeffNewlywedCounter;
    elliJeffPregnancyCounter = buff->elliJeffPregnancyCounter;
    annCliffBabyAge = buff->annCliffBabyAge;
    annCliffNewlywedCounter = buff->annCliffNewlywedCounter;
    annPregnancyCounter = buff->annPregnancyCounter;
    karenKaiBabyAge = buff->karenKaiBabyAge;
    karenKaiNewlywedCounter = buff->karenKaiNewlywedCounter;
    karenPregnancyCounter = buff->karenPregnancyCounter;

    dailyShippingBinValue = buff->dailyShippingBinValue;
    readMailBits[0] = buff->readMailBits;
    D_801C3F3C = buff->unk_104;
    D_801C3F40 = buff->unk_108;
    mailboxBits[0] = buff->mailboxBits;
    D_8016FFF0 = buff->unk_110;
    D_8016FFF4 = buff->unk_114;

    for (i = 0; i < 32; i++) {
        gToolchestSlots[i] = *((u8*)buff + 0x380 + i);
    }

    for (i = 0; i < 32; i++) {
        D_80237420[i] = *((u8*)buff + 0x3A0 + i);
    }

    for (i = 0; i < 32; i++) {
        D_801890E8[i] = *((u8*)buff + 0x3C0 + i);
    }
 
    for (i = 0; i < 9; i++) {
        gPlayer.belongingsSlots[i] = *((u8*)buff + 0x400 + i);
    }

    for (i = 0; i < 9; i++) {
        gPlayer.toolSlots[i] = *((u8*)buff + 0x410 + i);
    }

    for (i = 0; i < 24; i++) {
        gPlayer.keyItemSlots[i] = *((u8*)buff + 0x420 + i);
    } 

    for (i = 0; i < MAX_NPCS; i++) {
        npcAffection[i] =  *((u8*)buff + 0x480 + i);
    } 
    
    row = 0;
    i = 0;

    for (row = 0; row < 5; row++) {
        for (col = 0; col < 7; col++) {
            D_80189108[row][col] = ((u8*)&buff->D_80189108)[i];
            i++;
        }
    }

    dogInfo.name[0] = buff->dogName[0];
    dogInfo.name[1] = buff->dogName[1];
    dogInfo.name[2] = buff->dogName[2];
    dogInfo.name[3] = buff->dogName[3];
    dogInfo.name[4] = buff->dogName[4];
    dogInfo.name[5] = buff->dogName[5];
    dogInfo.affection = buff->dogAffection;
    dogInfo.location = buff->dogLocation;
    dogInfo.coordinates.x = buff->dogCoordX;
    dogInfo.coordinates.z = buff->dogCoordZ;
    dogInfo.unk_1C = buff->dogUnk_1C;

    horseInfo.name[0] = buff->horseName[0];
    horseInfo.name[1] = buff->horseName[1];
    horseInfo.name[2] = buff->horseName[2];
    horseInfo.name[3] = buff->horseName[3];
    horseInfo.name[4] = buff->horseName[4];
    horseInfo.name[5] = buff->horseName[5];
    horseInfo.affection = buff->horseAffection;
    horseInfo.location = buff->horseLocation;
    horseInfo.coordinates.x = buff->horseCoordX;
    horseInfo.coordinates.z = buff->horseCoordZ;
    horseInfo.grown = buff->horseGrown;
    horseInfo.age = buff->horseAge;
    horseInfo.unk_1E = buff->horseUnk_1E;
    horseInfo.flags = buff->horseFlags;

    for (i = 0; i < MAX_CHICKENS; i++) {
        gChickens[i].name[0] = *((u8*)buff + 0x600 + (i << 5));
        gChickens[i].name[1] = *((u8*)buff + 0x601 + (i << 5));
        gChickens[i].name[2] = *((u8*)buff + 0x602 + (i << 5));
        gChickens[i].name[3] = *((u8*)buff + 0x603 + (i << 5));
        gChickens[i].name[4] = *((u8*)buff + 0x604 + (i << 5));
        gChickens[i].name[5] = *((u8*)buff + 0x605 + (i << 5));
        gChickens[i].location = *((u8*)buff + 0x607 + (i << 5));
        gChickens[i].coordinates.x = *(f32*)((u8*)buff + (i << 5) + 0x608);
        gChickens[i].coordinates.z = *(f32*)((u8*)buff + (i << 5) + 0x60C);
        gChickens[i].type = *((u8*)buff + 0x610 + (i << 5));
        gChickens[i].condition = *((u8*)buff + 0x611 + (i << 5));
        gChickens[i].typeCounter = *((u8*)buff + 0x612 + (i << 5));
        gChickens[i].conditionCounter = *((u8*)buff + 0x613 + (i << 5));
        gChickens[i].flags = *(u16*)((u8*)buff + (i << 5) + 0x614);
    } 

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {
        gFarmAnimals[i].name[0] = *((u8*)buff + 0x800 + (i << 5));
        gFarmAnimals[i].name[1] = *((u8*)buff + 0x801 + (i << 5));
        gFarmAnimals[i].name[2] = *((u8*)buff + 0x802 + (i << 5));
        gFarmAnimals[i].name[3] = *((u8*)buff + 0x803 + (i << 5));
        gFarmAnimals[i].name[4] = *((u8*)buff + 0x804 + (i << 5));
        gFarmAnimals[i].name[5] = *((u8*)buff + 0x805 + (i << 5));
        gFarmAnimals[i].affection = *((u8*)buff + 0x806 + (i << 5));
        gFarmAnimals[i].location = *((u8*)buff + 0x807 + (i << 5));
        gFarmAnimals[i].coordinates.x = *(f32*)((u8*)buff + (i << 5) + 0x808);
        gFarmAnimals[i].coordinates.z = *(f32*)((u8*)buff + (i << 5) + 0x80C);
        gFarmAnimals[i].type = *((u8*)buff + 0x810 + (i << 5));
        gFarmAnimals[i].condition = *((u8*)buff + 0x811 + (i << 5));
        gFarmAnimals[i].typeCounter = *((u8*)buff + 0x812 + (i << 5));
        gFarmAnimals[i].conditionCounter = *((u8*)buff + 0x813 + (i << 5));
        gFarmAnimals[i].flags = *(u16*)((u8*)buff + 0x814 + (i << 5));
        gFarmAnimals[i].unk_23[0] = *((u8*)buff + 0x816 + (i << 5));
        gFarmAnimals[i].unk_23[1] = *((u8*)buff + 0x817 + (i << 5));
        gFarmAnimals[i].unk_23[2] = *((u8*)buff + 0x818 + (i << 5));
        gFarmAnimals[i].unk_23[3] = *((u8*)buff + 0x819 + (i << 5));
        gFarmAnimals[i].unk_23[4] = *((u8*)buff + 0x81A + (i << 5));
        gFarmAnimals[i].unk_23[5] = *((u8*)buff + 0x81B + (i << 5));
        gFarmAnimals[i].birthdaySeason = *((u8*)buff + 0x81C + (i << 5));
        gFarmAnimals[i].birthdayDayOfMonth = *((u8*)buff + 0x81D + (i << 5));
        gFarmAnimals[i].normalMilk = *((u8*)buff + 0x81E + (i << 5));
    }

    row = 0;
    idx = 0;
    
    for (row = 0; row < FIELD_HEIGHT; row++) {
        for (col = 0; col < FIELD_WIDTH; col++) {
            farmFieldTiles[row][col] = ((u8*)&buff->farmFieldTiles)[idx];
            idx++;
        }
    }

    row = 0;
    idx = 0;
    
    for (row = 0; row < FIELD_HEIGHT; row++) {
        for (col = 0; col < FIELD_WIDTH; col++) {
            greenhouseFieldTiles[row][col] = ((u8*)&buff->greenhouseFieldTiles)[idx];
            idx++;
        }
    }
        
    result = FALSE;
    
    lifeEventBits[0] = buff->lifeEventBits[0];
    lifeEventBits[1] = buff->lifeEventBits[1];
    lifeEventBits[2] = buff->lifeEventBits[2];
    lifeEventBits[3] = buff->lifeEventBits[3];
    lifeEventBits[4] = buff->lifeEventBits[4];
    lifeEventBits[5] = buff->lifeEventBits[5];
    lifeEventBits[6] = buff->lifeEventBits[6];
    lifeEventBits[7] = buff->lifeEventBits[7];
    lifeEventBits[8] = buff->lifeEventBits[8];
    lifeEventBits[9] = buff->lifeEventBits[9];
    lifeEventBits[10] = buff->lifeEventBits[10];
    lifeEventBits[11] = buff->lifeEventBits[11];
    lifeEventBits[12] = buff->lifeEventBits[12];
    lifeEventBits[13] = buff->lifeEventBits[13];
    lifeEventBits[14] = buff->lifeEventBits[14];
    lifeEventBits[15] = buff->lifeEventBits[15];

    specialDialogueBits[0] = buff->specialDialogueBits[0];
    specialDialogueBits[1] = buff->specialDialogueBits[1];
    specialDialogueBits[2] = buff->specialDialogueBits[2];
    specialDialogueBits[3] = buff->specialDialogueBits[3];
    specialDialogueBits[4] = buff->specialDialogueBits[4];
    specialDialogueBits[5] = buff->specialDialogueBits[5];
    specialDialogueBits[6] = buff->specialDialogueBits[6];
    specialDialogueBits[7] = buff->specialDialogueBits[7];
    specialDialogueBits[8] = buff->specialDialogueBits[8];
    specialDialogueBits[9] = buff->specialDialogueBits[9];
    specialDialogueBits[10] = buff->specialDialogueBits[10];
    specialDialogueBits[11] = buff->specialDialogueBits[11];
    specialDialogueBits[12] = buff->specialDialogueBits[12];
    specialDialogueBits[13] = buff->specialDialogueBits[13];
    specialDialogueBits[14] = buff->specialDialogueBits[14];
    specialDialogueBits[15] = buff->specialDialogueBits[15];

    if (func_800E3300(buff->signature) ) {
        if (func_800E391C(buff)) {
            result = TRUE;
        }
    } 

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/game/loadGameScreen", func_800E53E8);

// save game state to sram
bool func_800E53E8(u8 saveSlot) {

    SramBuffer* buff = (SramBuffer*)SRAM_BUFFER;
    u32 temp = saveSlot << 12;
    u32 devAddr = temp + 0x08000000;
    
    bool result = FALSE;

    u8 i, j;
    u8 row, col;
    u16 idx;
    u8 idx2;
    
    // ?
    buff->signature[0] = D_80119740[0];
    sramBuffer.signature[1] = D_80119740[1];
    sramBuffer.signature[2] = D_80119740[2];
    sramBuffer.signature[3] = D_80119740[3];
    sramBuffer.signature[4] = D_80119740[4];
    sramBuffer.signature[5] = D_80119740[5];
    sramBuffer.signature[6] = D_80119740[6];
    sramBuffer.signature[7] = D_80119740[7];
    sramBuffer.signature[8] = D_80119740[8];
    sramBuffer.signature[9] = D_80119740[9];
    sramBuffer.signature[10] = D_80119740[10];
    sramBuffer.signature[11] = D_80119740[11];
    sramBuffer.signature[12] = D_80119740[12];
    sramBuffer.signature[13] = D_80119740[13];
    sramBuffer.signature[14] = D_80119740[14];
    sramBuffer.signature[15] = D_80119740[15];

    sramBuffer.year = gYear;
    sramBuffer.season = gSeason;
    sramBuffer.dayOfWeek = gDayOfWeek;
    sramBuffer.dayOfMonth = gDayOfMonth;

    sramBuffer.playerName[0] = gPlayer.name[0];
    sramBuffer.playerName[1] = gPlayer.name[1];
    sramBuffer.playerName[2] = gPlayer.name[2];
    sramBuffer.playerName[3] = gPlayer.name[3];
    sramBuffer.playerName[4] = gPlayer.name[4];
    sramBuffer.playerName[5] = gPlayer.name[5];

    sramBuffer.farmName[0] = gFarmName[0];
    sramBuffer.farmName[1] = gFarmName[1];
    sramBuffer.farmName[2] = gFarmName[2];
    sramBuffer.farmName[3] = gFarmName[3];
    sramBuffer.farmName[4] = gFarmName[4];
    sramBuffer.farmName[5] = gFarmName[5];

    sramBuffer.gold = gGold;
    sramBuffer.weather = gWeather;
    sramBuffer.forecast = gForecast;
    sramBuffer.playerBirthdaySeason = gPlayerBirthdaySeason;
    sramBuffer.wateringCanUses = wateringCanUses;
    sramBuffer.lumber = gLumber;
    sramBuffer.fodderQuantity = fodderQuantity;
    sramBuffer.seasonTomorrow = gSeasonTomorrow;

    sramBuffer.unk_2D = D_801FC154;
    sramBuffer.unk_2E = D_80204DF4;
    sramBuffer.unk_2F = D_80237458;
    sramBuffer.unk_30 = D_8018A725;
    sramBuffer.unk_31 = D_802373E8;
    sramBuffer.unk_32 = D_8013DC2C;
    sramBuffer.unk_33 = D_801FAD91;
    sramBuffer.unk_34 = D_801C3E28;
    sramBuffer.unk_35 = D_801C3F70;
    sramBuffer.unk_36 = D_80205636;
    sramBuffer.unk_37 = D_802373A8;

    sramBuffer.maximumStamina = gMaximumStamina;
    sramBuffer.currentStamina = gPlayer.currentStamina;
    sramBuffer.fatigueCounter = gPlayer.fatigueCounter;
    sramBuffer.staminaExhaustionLevel = gPlayer.staminaExhaustionLevel;

    sramBuffer.alcoholTolerance = gAlcoholTolerance;
    sramBuffer.houseExtensionSelection = gHouseExtensionSelection;
    sramBuffer.houseExtensionConstructionCounter = houseExtensionConstructionCounter;
    sramBuffer.chickenFeedQuantity = chickenFeedQuantity;
    sramBuffer.unk_42 = D_8021E6D0;

    sramBuffer.harvestKing = gHarvestKing;
    sramBuffer.harvestCoinFinder = gHarvestCoinFinder;
    sramBuffer.flowerFestivalGoddess = gFlowerFestivalGoddess;
    sramBuffer.cowFestivalStall = D_80189054;
    sramBuffer.voteForFlowerFestivalGoddess = gVoteForFlowerFestivalGoddess;

    sramBuffer.deadAnimalName[0] = deadAnimalName[0];
    sramBuffer.deadAnimalName[1] = deadAnimalName[1];
    sramBuffer.deadAnimalName[2] = deadAnimalName[2];
    sramBuffer.deadAnimalName[3] = deadAnimalName[3];
    sramBuffer.deadAnimalName[4] = deadAnimalName[4];
    sramBuffer.deadAnimalName[5] = deadAnimalName[5];

    sramBuffer.happiness = gHappiness;
    sramBuffer.blueMistFlowerPlot = blueMistFlowerPlot;
    sramBuffer.anniversarySeason = anniversarySeason;
    sramBuffer.anniversaryDate = anniversaryDate;
    sramBuffer.babyBirthdaySeason = babyBirthdaySeason;
    sramBuffer.babyBirthdayDate = babyBirthdayDate;

    sramBuffer.powerNutBits = powerNutBits;
    sramBuffer.unk_58 = D_80205204;
    sramBuffer.flowerShopPoints = flowerShopPoints;
    sramBuffer.bakeryCardPoints = bakeryCardPoints;
    sramBuffer.elliGrievingCounter = gElliGrievingCounter;

    sramBuffer.unk_60 = D_801654F4;
    sramBuffer.unk_64 = D_80237414;
    sramBuffer.unk_68 = D_801FB6FC;
    sramBuffer.unk_6C = D_801C3F80;
    sramBuffer.unk_70 = D_801806C0;
    sramBuffer.unk_74 = D_80188F60;
    sramBuffer.unk_78 = D_801FB5D0;

    sramBuffer.totalMilkShipped = gTotalMilkShipped;
    sramBuffer.totalEggsShipped = gTotalEggsShipped;
    sramBuffer.totalCropsShipped = gTotalCropsShipped;
    sramBuffer.totalFishCaught = totalFishCaught;

    sramBuffer.babyName[0] = gBabyName[0];
    sramBuffer.babyName[1] = gBabyName[1];
    sramBuffer.babyName[2] = gBabyName[2];
    sramBuffer.babyName[3] = gBabyName[3];
    sramBuffer.babyName[4] = gBabyName[4];
    sramBuffer.babyName[5] = gBabyName[5];

    sramBuffer.sickDays = gSickDays;
    sramBuffer.deadAnimalCount = D_8013DC2E;

    sramBuffer.mrsManaCow1Index = mrsManaCow1Index;
    sramBuffer.mrsManaCow2Index = mrsManaCow2Index;
    sramBuffer.mrsManaCow3Index = mrsManaCow3Index;

    sramBuffer.toolUseCounters[0] = gPlayer.toolUseCounters[0];
    sramBuffer.toolUseCounters[1] = gPlayer.toolUseCounters[1];
    sramBuffer.toolUseCounters[2] = gPlayer.toolUseCounters[2];
    sramBuffer.toolUseCounters[3] = gPlayer.toolUseCounters[3];
    sramBuffer.toolUseCounters[4] = gPlayer.toolUseCounters[4];

    sramBuffer.toolLevels[0] = gPlayer.toolLevels[0];
    sramBuffer.toolLevels[1] = gPlayer.toolLevels[1];
    sramBuffer.toolLevels[2] = gPlayer.toolLevels[2];
    sramBuffer.toolLevels[3] = gPlayer.toolLevels[3];
    sramBuffer.toolLevels[4] = gPlayer.toolLevels[4];

    sramBuffer.bottleContents = gPlayer.bottleContents;

    sramBuffer.tvChannelIndex = tvContext.channelIndex;
    sramBuffer.varietyShowEpisodeCounters[0] = tvContext.varietyShowEpisodeCounters[0];
    sramBuffer.varietyShowEpisodeCounters[1] = tvContext.varietyShowEpisodeCounters[1];
    sramBuffer.varietyShowEpisodeCounters[2] = tvContext.varietyShowEpisodeCounters[2];
    sramBuffer.varietyShowEpisodeCounters[3] = tvContext.varietyShowEpisodeCounters[3];
    sramBuffer.varietyShowEpisodeCounters[4] = tvContext.varietyShowEpisodeCounters[4];
    sramBuffer.varietyShowEpisodeCounters[5] = tvContext.varietyShowEpisodeCounters[5];
    sramBuffer.varietyShowEpisodeCounters[6] = tvContext.varietyShowEpisodeCounters[6];

    sramBuffer.recipesBits = recipesBits[0];
    sramBuffer.unk_DC = D_8016FB00;
    sramBuffer.albumBits = albumBits;

    sramBuffer.bornChickenIndex = bornChickenIndex;
    sramBuffer.bornAnimalIndex = bornAnimalIndex;

    sramBuffer.wife = gWife; 
    sramBuffer.babyAge = gBabyAge;
    sramBuffer.wifeConceptionCounter = gWifeConceptionCounter;
    sramBuffer.wifePregnancyCounter = gWifePregnancyCounter;

    sramBuffer.mariaHarrisBabyAge = mariaHarrisBabyAge;
    sramBuffer.mariaHarrisNewlywedCounter = mariaHarrisNewlywedCounter;
    sramBuffer.mariaHarrisPregnancyCounter = mariaHarrisPregnancyCounter;
    sramBuffer.popuriGrayBabyAge = popuriGrayBabyAge;
    sramBuffer.popuriGrayNewlywedCounter = popuriGrayNewlywedCounter;
    sramBuffer.popuriGrayPregnancyCounter = popuriGrayPregnancyCounter;
    sramBuffer.elliJeffBabyAge = elliJeffBabyAge;
    sramBuffer.elliJeffNewlywedCounter = elliJeffNewlywedCounter;
    sramBuffer.elliJeffPregnancyCounter = elliJeffPregnancyCounter;
    sramBuffer.annCliffBabyAge = annCliffBabyAge;
    sramBuffer.annCliffNewlywedCounter = annCliffNewlywedCounter;
    sramBuffer.annPregnancyCounter = annPregnancyCounter;
    sramBuffer.karenKaiBabyAge = karenKaiBabyAge;
    sramBuffer.karenKaiNewlywedCounter = karenKaiNewlywedCounter;
    sramBuffer.karenPregnancyCounter = karenPregnancyCounter;
    
    sramBuffer.dailyShippingBinValue = dailyShippingBinValue;
    sramBuffer.readMailBits = readMailBits[0];
    sramBuffer.unk_104 = D_801C3F3C;
    sramBuffer.unk_108 = D_801C3F40;
    sramBuffer.mailboxBits = mailboxBits[0];
    sramBuffer.unk_110 = D_8016FFF0;
    sramBuffer.unk_114 = D_8016FFF4;

    for (i = 0; i < 32; i++) {
        *((u8*)buff + 0x380 + i) = gToolchestSlots[i];
    }

    for (i = 0; i < 32; i++) {
        *((u8*)buff + 0x3A0 + i) = D_80237420[i];
    }

    for (i = 0; i < 32; i++) {
        *((u8*)buff + 0x3C0 + i) = D_801890E8[i];
    }

    for (i = 0; i < 9; i++) {
        *((u8*)buff + 0x400 + i) = gPlayer.belongingsSlots[i];
    }

    for (i = 0; i < 9; i++) {
        *((u8*)buff + 0x410 + i) = gPlayer.toolSlots[i];
    }

    for (i = 0; i < 24; i++) {
        *((u8*)buff + 0x420 + i) = gPlayer.keyItemSlots[i];
    }

    for (i = 0; i < MAX_NPCS; i++) {
        *((u8*)buff + 0x480 + i) = npcAffection[i];
    }

    row = 0;
    i = 0;

    for (row = 0; row < 5; row++) {
        for (col = 0; col < 7; col++) {
            ((u8*)buff->D_80189108)[i] = D_80189108[row][col];
            i++;
        }
    } 

    buff->dogName[0] = dogInfo.name[0];
    buff->dogName[1] = dogInfo.name[1];
    buff->dogName[2] = dogInfo.name[2];
    buff->dogName[3] = dogInfo.name[3];
    buff->dogName[4] = dogInfo.name[4];
    buff->dogName[5] = dogInfo.name[5];
    buff->dogAffection = dogInfo.affection;
    buff->dogLocation = dogInfo.location;
    buff->dogCoordX = dogInfo.coordinates.x;
    buff->dogCoordZ = dogInfo.coordinates.z;
    buff->dogUnk_1C = dogInfo.unk_1C;

    buff->horseName[0] = horseInfo.name[0];
    buff->horseName[1] = horseInfo.name[1];
    buff->horseName[2] = horseInfo.name[2];
    buff->horseName[3] = horseInfo.name[3];
    buff->horseName[4] = horseInfo.name[4];
    buff->horseName[5] = horseInfo.name[5];
    buff->horseAffection = horseInfo.affection;
    buff->horseLocation = horseInfo.location;
    buff->horseCoordX = horseInfo.coordinates.x;
    buff->horseCoordZ = horseInfo.coordinates.z;
    buff->horseGrown = horseInfo.grown;
    buff->horseAge = horseInfo.age;
    buff->horseUnk_1E = horseInfo.unk_1E;
    buff->horseFlags = horseInfo.flags;
    
    for (i = 0; i < MAX_CHICKENS; i++) {
        *((u8*)buff + 0x600 + (i << 5)) = gChickens[i].name[0];
        *((u8*)buff + 0x601 + (i << 5)) = gChickens[i].name[1];
        *((u8*)buff + 0x602 + (i << 5)) = gChickens[i].name[2];
        *((u8*)buff + 0x603 + (i << 5)) = gChickens[i].name[3];
        *((u8*)buff + 0x604 + (i << 5)) = gChickens[i].name[4];
        *((u8*)buff + 0x605 + (i << 5)) = gChickens[i].name[5];
        *((u8*)buff + 0x607 + (i << 5)) = gChickens[i].location;
        *(f32*)((u8*)buff + (i << 5) + 0x608) = gChickens[i].coordinates.x;
        *(f32*)((u8*)buff + (i << 5) + 0x60C) = gChickens[i].coordinates.z;
        *((u8*)buff + 0x610 + (i << 5)) = gChickens[i].type;
        *((u8*)buff + 0x611 + (i << 5)) = gChickens[i].condition;
        *((u8*)buff + 0x612 + (i << 5)) = gChickens[i].typeCounter;
        *((u8*)buff + 0x613 + (i << 5)) = gChickens[i].conditionCounter;
        *(u16*)((u8*)buff + (i << 5) + 0x614) = gChickens[i].flags;
    }

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {
        *((u8*)buff + 0x800 + (i << 5)) = gFarmAnimals[i].name[0];
        *((u8*)buff + 0x801 + (i << 5)) = gFarmAnimals[i].name[1];
        *((u8*)buff + 0x802 + (i << 5)) = gFarmAnimals[i].name[2];
        *((u8*)buff + 0x803 + (i << 5)) = gFarmAnimals[i].name[3];
        *((u8*)buff + 0x804 + (i << 5)) = gFarmAnimals[i].name[4];
        *((u8*)buff + 0x805 + (i << 5)) = gFarmAnimals[i].name[5];
        *((u8*)buff + 0x806 + (i << 5)) = gFarmAnimals[i].affection;
        *((u8*)buff + 0x807 + (i << 5)) = gFarmAnimals[i].location;
        *(f32*)((u8*)buff + (i << 5) + 0x808) = gFarmAnimals[i].coordinates.x;
        *(f32*)((u8*)buff + (i << 5) + 0x80C) = gFarmAnimals[i].coordinates.z;
        *((u8*)buff + 0x810 + (i << 5)) = gFarmAnimals[i].type;
        *((u8*)buff + 0x811 + (i << 5)) = gFarmAnimals[i].condition;
        *((u8*)buff + 0x812 + (i << 5)) = gFarmAnimals[i].typeCounter;
        *((u8*)buff + 0x813 + (i << 5)) = gFarmAnimals[i].conditionCounter;
        *(u16*)((u8*)buff + 0x814 + (i << 5)) = gFarmAnimals[i].flags;
        *((u8*)buff + 0x816 + (i << 5)) = gFarmAnimals[i].unk_23[0];
        *((u8*)buff + 0x817 + (i << 5)) = gFarmAnimals[i].unk_23[1];
        *((u8*)buff + 0x818 + (i << 5)) = gFarmAnimals[i].unk_23[2];
        *((u8*)buff + 0x819 + (i << 5)) = gFarmAnimals[i].unk_23[3];
        *((u8*)buff + 0x81A + (i << 5)) = gFarmAnimals[i].unk_23[4];
        *((u8*)buff + 0x81B + (i << 5)) = gFarmAnimals[i].unk_23[5];
        *((u8*)buff + 0x81C + (i << 5)) = gFarmAnimals[i].birthdaySeason;
        *((u8*)buff + 0x81D + (i << 5)) = gFarmAnimals[i].birthdayDayOfMonth;
        *((u8*)buff + 0x81E + (i << 5)) = gFarmAnimals[i].normalMilk;
    }

    row = 0;
    idx = 0;

    for (row = 0; row < FIELD_HEIGHT; row++) {
        for (col = 0; col < FIELD_WIDTH; col++) {
            ((u8*)buff->farmFieldTiles)[idx] = farmFieldTiles[row][col];
            idx++;
        }
    }

    row = 0;
    idx = 0;

    for (row = 0; row < FIELD_HEIGHT; row++) {
        for (col = 0; col < FIELD_WIDTH; col++) {
            ((u8*)buff->greenhouseFieldTiles)[idx] = greenhouseFieldTiles[row][col];
            idx++;
        }
    }

    buff->lifeEventBits[0] = lifeEventBits[0];
    buff->lifeEventBits[1] = lifeEventBits[1];
    buff->lifeEventBits[2] = lifeEventBits[2];
    buff->lifeEventBits[3] = lifeEventBits[3];
    buff->lifeEventBits[4] = lifeEventBits[4];
    buff->lifeEventBits[5] = lifeEventBits[5];
    buff->lifeEventBits[6] = lifeEventBits[6];
    buff->lifeEventBits[7] = lifeEventBits[7];
    buff->lifeEventBits[8] = lifeEventBits[8];
    buff->lifeEventBits[9] = lifeEventBits[9];
    buff->lifeEventBits[10] = lifeEventBits[10];
    buff->lifeEventBits[11] = lifeEventBits[11];
    buff->lifeEventBits[12] = lifeEventBits[12];
    buff->lifeEventBits[13] = lifeEventBits[13];
    buff->lifeEventBits[14] = lifeEventBits[14];
    buff->lifeEventBits[15] = lifeEventBits[15];

    buff->specialDialogueBits[0] = specialDialogueBits[0];
    buff->specialDialogueBits[1] = specialDialogueBits[1];
    buff->specialDialogueBits[2] = specialDialogueBits[2];
    buff->specialDialogueBits[3] = specialDialogueBits[3];
    buff->specialDialogueBits[4] = specialDialogueBits[4];
    buff->specialDialogueBits[5] = specialDialogueBits[5];
    buff->specialDialogueBits[6] = specialDialogueBits[6];
    buff->specialDialogueBits[7] = specialDialogueBits[7];
    buff->specialDialogueBits[8] = specialDialogueBits[8];
    buff->specialDialogueBits[9] = specialDialogueBits[9];
    buff->specialDialogueBits[10] = specialDialogueBits[10];
    buff->specialDialogueBits[11] = specialDialogueBits[11];
    buff->specialDialogueBits[12] = specialDialogueBits[12];
    buff->specialDialogueBits[13] = specialDialogueBits[13];
    buff->specialDialogueBits[14] = specialDialogueBits[14];
    buff->specialDialogueBits[15] = specialDialogueBits[15];

    // calculate and write checksum
    buff->checksum = func_800E38E8(buff);

    // write to memory
    func_8004DD7C(devAddr, buff, 0x1000);

    result = FALSE;

    // verify save (signature and checksum)
    if (func_800E3300(buff->signature)) {
        if (func_800E391C(buff)) {
            result = TRUE;
        }
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/loadGameScreen", func_800E66A0);

void func_800E66A0(u8 saveSlot, u8 arg1, u8 arg2, u8 arg3) {

    u32 devAddr;
    u32 vaddr;
    u32 temp;

    temp =  saveSlot << 12;
    
    devAddr = temp + 0x08000000;
    vaddr = SRAM_BUFFER;
     
    if (arg1 == 0) {
        func_8004DC48(devAddr, vaddr, 0x1000);
    } else {

        func_800E16A0();

        if (loadGameScreenContext.gamePakEnabled) {
            func_8004D9AC(CONTROLLER_1, &loadGameScreenContext.unk_0[(saveSlot * 4 + saveSlot) * 4 + saveSlot], &loadGameScreenContext.unk_15[(saveSlot * 4 + saveSlot) * 4 + saveSlot - 5], 0, 0x1000, vaddr);
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

void func_800E67E4(u8 saveSlot, u8 arg1) {

    u32 devAddr;
    u32 vaddr;
    u32 temp;

    sramBuffer.signature[0] = 0;
    sramBuffer.signature[1] = 0;
    sramBuffer.signature[2] = 0;
    sramBuffer.signature[3] = 0;
    sramBuffer.signature[4] = 0;
    sramBuffer.signature[5] = 0;
    sramBuffer.signature[6] = 0;
    sramBuffer.signature[7] = 0;
    sramBuffer.signature[8] = 0;
    sramBuffer.signature[9] = 0;
    sramBuffer.signature[10] = 0;
    sramBuffer.signature[11] = 0;
    sramBuffer.signature[12] = 0;
    sramBuffer.signature[13] = 0;
    sramBuffer.signature[14] = 0;
    sramBuffer.signature[15] = 0;

    temp =  saveSlot << 12;
    devAddr = temp + 0x08000000;
    vaddr = SRAM_BUFFER;

    if (arg1 == 0) {
        func_8004DD7C(devAddr, vaddr, 0x10);
    } else {

        func_800E16A0();

        if (loadGameScreenContext.gamePakEnabled) {
            func_8004DAF4(CONTROLLER_1, &loadGameScreenContext.unk_0[(saveSlot * 4 + saveSlot) * 4 + saveSlot], &loadGameScreenContext.unk_15[(saveSlot * 4 + saveSlot) * 4 + saveSlot - 5]);
        }
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/loadGameScreen", loadCompletionScreen);

void loadCompletionScreen(void) {

    u8 i = 0;

    // 0 initialize strings
    for (i = 0; i < 7; i++) {
        func_800E6C08(i);
    } 
    
    // abbreviated player names on first screen
    setGameVariableString(0, (u8*)&D_801FB708[0], 6);
    setGameVariableString(1, (u8*)&D_801FB708[1], 6);
    setGameVariableString(2, (u8*)&D_801FB708[2], 6);
    setGameVariableString(3, (u8*)&D_801FB708[3], 6);
    setGameVariableString(4, (u8*)&D_801FB708[4], 6);
    
    // load game state from sram and write completion screen strings
    func_800E9550(0);
    func_800E9550(1);
    func_800E9550(2);
    func_800E9550(3);
    func_800E9550(4);

    // set strings from game state
    func_800EA3AC(0, 0);
    func_800EA3AC(1, 0);
    func_800EA3AC(2, 0);
    func_800EA3AC(3, 0);
    // game pak
    func_800EA3AC(0, 1);
    func_800EA3AC(1, 1);

    // save/cache completion state into sram?
    func_800E9B2C(0);
    func_800E9B2C(1);
    func_800E9B2C(2);
    func_800E9B2C(3);
    func_800E9B2C(4);

    // set up sprites and message boxes
    func_800E6FB4(0);

    // overlay screen rankings sprites
    func_800B6674();
    // update sprite rgba
    func_800B7B34();

    setMainLoopCallbackFunctionIndex(COMPLETION_SCREEN);
    
    // completion screen action index
    D_801FB9A0 = 0;
    // row index
    D_801FB99B[0] = 0;

    // unused
    D_8018A728 = D_801FB978[0];
    D_801D6F2C = D_801FB978[1];
    D_8018A080 = D_801FB978[2];
    D_802226E4 = D_801FB978[3];
    D_80204DF0 = D_801FB978[4];
    
}

//INCLUDE_ASM("asm/nonmatchings/game/loadGameScreen", func_800E6C08);

// setup/reset completion screen strings
void func_800E6C08(u8 arg0) {

    u32 *ptr;
    u16* ptr2;
    f32* ptr3;
    u32 *ptr4;
    u32 *ptr5;

    D_801FB994[arg0] = 0;

    D_801FB708[arg0][0] = 0xF6;
    D_801FB708[arg0][1] = 0xF6;
    D_801FB708[arg0][2] = 0xF6;
    D_801FB708[arg0][3] = 0xF6;
    D_801FB708[arg0][4] = 0xF6;
    D_801FB708[arg0][5] = 0xF6;

    D_801FB732[arg0][0] = 0xF6;
    D_801FB732[arg0][1] = 0xF6;
    D_801FB732[arg0][2] = 0xF6;
    D_801FB732[arg0][3] = 0xF6;
    D_801FB732[arg0][4] = 0xF6;
    D_801FB732[arg0][5] = 0xF6;
    
    D_801FB75C[arg0][0] = 0xF6;
    D_801FB75C[arg0][1] = 0xF6;
    D_801FB75C[arg0][2] = 0xF6;
    D_801FB75C[arg0][3] = 0xF6;
    D_801FB75C[arg0][4] = 0xF6;
    D_801FB75C[arg0][5] = 0xF6;
        
    D_801FB786[arg0][0] = 0xF6;
    D_801FB786[arg0][1] = 0xF6;
    D_801FB786[arg0][2] = 0xF6;
    D_801FB786[arg0][3] = 0xF6;
    D_801FB786[arg0][4] = 0xF6;
    D_801FB786[arg0][5] = 0xF6;

    D_801FB7B0[arg0][0] = 0xF6;
    D_801FB7B0[arg0][1] = 0xF6;
    D_801FB7B0[arg0][2] = 0xF6;
    D_801FB7B0[arg0][3] = 0xF6;
    D_801FB7B0[arg0][4] = 0xF6;
    D_801FB7B0[arg0][5] = 0xF6;

    D_801FB7DA[arg0][0] = 0xF6;
    D_801FB7DA[arg0][1] = 0xF6;
    D_801FB7DA[arg0][2] = 0xF6;
    D_801FB7DA[arg0][3] = 0xF6;
    D_801FB7DA[arg0][4] = 0xF6;
    D_801FB7DA[arg0][5] = 0xF6;
    
    D_801FB804[arg0] = 0;
    D_801FB80B[arg0] = 0;    
    D_801FB812[arg0] = 0;
    D_801FB819[arg0] = 0;
    D_801FB820[arg0] = 0;    
    D_801FB827[arg0] = 0;
    D_801FB82E[arg0] = 0;
    D_801FB835[arg0] = 0;    
    D_801FB83C[arg0] = 0;
    D_801FB843[arg0] = 0;
    D_801FB84A[arg0] = 0;    
    D_801FB851[arg0] = 0;
    D_801FB858[arg0] = 0;
    D_801FB85F[arg0] = 0;
    D_801FB866[arg0] = 0;    
    D_801FB86D[arg0] = 0;
    
    D_801FB874[arg0][0] = 0;
    D_801FB874[arg0][1] = 0;
    D_801FB874[arg0][2] = 0;
    D_801FB874[arg0][3] = 0;
    D_801FB874[arg0][4] = 0;
    D_801FB874[arg0][5] = 0;
    D_801FB874[arg0][6] = 0;
    D_801FB874[arg0][7] = 0;
    
    D_801FB8AC[arg0] = 0;
    
    // FIXME: forced match
    ptr5 = D_801FB8B4;
    ptr = D_801FB8B4;
    
    ptr5[arg0] = 0;
    
    ptr4 = ptr + 7;
    
    ptr4[arg0] = 0;

    asm("");

    ptr += 7 * 2;
    ptr[arg0] = 0;
    asm("");
    
    ptr += 7;
    ptr[arg0] = 0;
    asm("");
    
    ptr += 7;
    ptr[arg0] = 0;
    asm("");
    
    ptr += 7;
    ptr2 = ptr;
    ptr2[arg0] = 0;

    D_801FB94E[arg0] = 0;
    D_801FB955[arg0] = 0;
    D_801FB95C[arg0] = 0;
    D_801FB963[arg0] = 0;
    D_801FB96A[arg0] = 0;

    ptr2 += 7 * 4;
    ptr3 = ptr2;
    
    ptr3[arg0] = 0;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/loadGameScreen", func_800E6FB4);

// set up completion screen sprites and message boxes
void func_800E6FB4(u8 arg0) {

    func_8003DD14(0);
    initializeEmptyMessageBox(0, (u8*)MESSAGE_BOX_1_TEXT_BUFFER);
    setMessageBoxViewSpacePosition(0, -28.0f, 32.0f, 0.0f);
    func_8003F5D0(0, 10, 2);
    func_8003F630(0, 0, 2);
    func_8003F464(0, 14, 14, (u8*)FONT_TEXTURE_BUFFER, (u16*)FONT_PALETTE_1_BUFFER);
    func_8003F360(0, -4, 2);
    setMessageBoxSpriteIndices(0, 0xFF, 0, 0);
    setMessageBoxButtonMask(0, BUTTON_A);
    func_8003FB4C(0, 1);
    initializeMessageBox(0, 0, 7, 0x80000);
    
    func_8003DD14(1);
    initializeEmptyMessageBox(1, (u8*)MESSAGE_BOX_2_TEXT_BUFFER);
    setMessageBoxViewSpacePosition(1, -28.0f, 0.0f, 0.0f);
    func_8003F5D0(1, 10, 2);
    func_8003F630(1, 0, 2);
    func_8003F464(1, 14, 14, (u8*)FONT_TEXTURE_BUFFER, (u16*)FONT_PALETTE_1_BUFFER);
    func_8003F360(1, -4, 2);
    setMessageBoxSpriteIndices(1, 0xFF, 0, 0);
    setMessageBoxButtonMask(1, BUTTON_A);
    func_8003FB4C(1, 1);
    initializeMessageBox(1, 0, 8, 0x80000);
    
    func_8003DD14(2);
    initializeEmptyMessageBox(2, (u8*)MESSAGE_BOX_3_TEXT_BUFFER);
    setMessageBoxViewSpacePosition(2, -28.0f, -32.0f, 0.0f);
    func_8003F5D0(2, 10, 2);
    func_8003F630(2, 0, 2);
    func_8003F464(2, 14, 14, (u8*)FONT_TEXTURE_BUFFER, (u16*)FONT_PALETTE_1_BUFFER);
    func_8003F360(2, -4, 2);
    setMessageBoxSpriteIndices(2, 0xFF, 0, 0);
    setMessageBoxButtonMask(2, BUTTON_A);
    func_8003FB4C(2, 1);
    initializeMessageBox(2, 0, 9, 0x80000);
    
    func_8003DD14(3);
    initializeEmptyMessageBox(3, (u8*)MESSAGE_BOX_4_TEXT_BUFFER);
    setMessageBoxViewSpacePosition(3, -28.0f, -64.0f, 0.0f);
    func_8003F5D0(3, 10, 2);
    func_8003F630(3, 0, 2);
    func_8003F464(3, 14, 14, (u8*)FONT_TEXTURE_BUFFER, (u16*)FONT_PALETTE_1_BUFFER);
    func_8003F360(3, -4, 2);
    setMessageBoxSpriteIndices(3, 0xFF, 0, 0);
    setMessageBoxButtonMask(3, BUTTON_A);
    func_8003FB4C(3, 1);
    initializeMessageBox(3, 0, 10, 0x80000);
    
    func_8003DD14(4);
    initializeEmptyMessageBox(4, (u8*)MESSAGE_BOX_5_TEXT_BUFFER);
    setMessageBoxViewSpacePosition(4, -28.0f, -96.0f, 0.0f);
    func_8003F5D0(4, 10, 2);
    func_8003F630(4, 0, 2);
    func_8003F464(4, 14, 14, (u8*)FONT_TEXTURE_BUFFER, (u16*)FONT_PALETTE_1_BUFFER);
    func_8003F360(4, -4, 2);
    setMessageBoxSpriteIndices(4, 0xFF, 0, 0);
    setMessageBoxButtonMask(4, BUTTON_A);
    func_8003FB4C(4, 1);
    initializeMessageBox(4, 0, 11, 0x80000);

     if (D_801FB994[0] & 1) {
        func_80045E20(0, 0x89, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, (void*)0x8026F000, (void*)0x8027E200, (void*)0x8027E700, (void*)0x8027EB00, 0, 4, 0, 48.0f, 40.0f, 16.0f, 0xA);
        dmaOverlayScreenSprites(0, D_801FB804[0], 1, 3);
        func_80045E20(5, 0x8B, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, (void*)0x8026F000, (void*)0x8027E200, (void*)0x8027E700, (void*)0x8027EB00, 0, 4, 0, 116.0f, 40.0f, 16.0f, 0xA);
        dmaOverlayScreenSprites(5, D_801FB978[0], 2, 3);
    }

    if (D_801FB994[1] & 1) {
        func_80045E20(1, 0x8E, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, (void*)0x8026F000, (void*)0x8027E200, (void*)0x8027E700, (void*)0x8027EB00, 0, 4, 0, 48.0f, 8.0f, 16.0f, 0xA);
        dmaOverlayScreenSprites(1, D_801FB804[1], 1, 3);
        func_80045E20(6, 0x90, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, (void*)0x8026F000, (void*)0x8027E200, (void*)0x8027E700, (void*)0x8027EB00, 0, 4, 0, 116.0f, 8.0f, 16.0f, 0xA);
        dmaOverlayScreenSprites(6, D_801FB978[1], 2, 3);
    }
    
    if (D_801FB994[2] & 1) {
        func_80045E20(2, 0x93, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, (void*)0x8026F000, (void*)0x8027E200, (void*)0x8027E700, (void*)0x8027EB00, 0, 4, 0, 48.0f, -24.0f, 16.0f, 0xA);
        dmaOverlayScreenSprites(2, D_801FB804[2], 1, 3);
        func_80045E20(7, 0x95, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, (void*)0x8026F000, (void*)0x8027E200, (void*)0x8027E700, (void*)0x8027EB00, 0, 4, 0, 116.0f, -24.0f, 16.0f, 0xA);
        dmaOverlayScreenSprites(7, D_801FB978[2], 2, 3);
    }
    if (D_801FB994[3] & 1) {
        func_80045E20(3, 0x98, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, (void*)0x8026F000, (void*)0x8027E200, (void*)0x8027E700, (void*)0x8027EB00, 0, 4, 0, 48.0f, -56.0f, 16.0f, 0xA);
        dmaOverlayScreenSprites(3, D_801FB804[3], 1, 3);
        func_80045E20(8, 0x9A, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, (void*)0x8026F000, (void*)0x8027E200, (void*)0x8027E700, (void*)0x8027EB00, 0, 4, 0, 116.0f, -56.0f, 16.0f, 0xA);
        dmaOverlayScreenSprites(8, D_801FB978[3], 2, 3);
    }
    
    if (D_801FB994[4] & 1) {
        func_80045E20(4, 0x9D, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, (void*)0x8026F000, (void*)0x8027E200, (void*)0x8027E700, (void*)0x8027EB00, 0, 4, 0, 48.0f, -88.0f, 16.0f, 0xA);
        dmaOverlayScreenSprites(4, D_801FB804[4], 1, 3);
        func_80045E20(9, 0x9F, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, (void*)0x8026F000, (void*)0x8027E200, (void*)0x8027E700, (void*)0x8027EB00, 0, 4, 0, 116.0f, -88.0f, 16.0f, 0xA);
        dmaOverlayScreenSprites(9, D_801FB978[4], 2, 3);
    }
    
    func_8003E77C(0, 0, 0, 0, 0);
    func_8003E77C(1, 0, 0, 0, 0);
    func_8003E77C(2, 0, 0, 0, 0);
    func_8003E77C(3, 0, 0, 0, 0);
    func_8003E77C(4, 0, 0, 0, 0);

    setOverlayScreenSpritesRGBA(0, 0, 0, 0, 0);
    setOverlayScreenSpritesRGBA(1, 0, 0, 0, 0);
    setOverlayScreenSpritesRGBA(2, 0, 0, 0, 0);
    setOverlayScreenSpritesRGBA(3, 0, 0, 0, 0);
    setOverlayScreenSpritesRGBA(4, 0, 0, 0, 0);
    setOverlayScreenSpritesRGBA(5, 0, 0, 0, 0);
    setOverlayScreenSpritesRGBA(6, 0, 0, 0, 0);
    setOverlayScreenSpritesRGBA(7, 0, 0, 0, 0);
    setOverlayScreenSpritesRGBA(8, 0, 0, 0, 0);
    setOverlayScreenSpritesRGBA(9, 0, 0, 0, 0);
    
    if (arg0 == 0) {
        
        func_8003EA1C(0, 0xFF, 0xFF, 0xFF, 0xFF, 24);
        func_8003EA1C(1, 0xFF, 0xFF, 0xFF, 0xFF, 24);
        func_8003EA1C(2, 0xFF, 0xFF, 0xFF, 0xFF, 24);
        func_8003EA1C(3, 0xFF, 0xFF, 0xFF, 0xFF, 24);
        func_8003EA1C(4, 0xFF, 0xFF, 0xFF, 0xFF, 24);
        func_8004635C(0, 0xFF, 0xFF, 0xFF, 0xFF, 24);

        func_8004635C(1, 0xFF, 0xFF, 0xFF, 0xFF, 24);
        func_8004635C(2, 0xFF, 0xFF, 0xFF, 0xFF, 24);
        func_8004635C(3, 0xFF, 0xFF, 0xFF, 0xFF, 24);
        func_8004635C(4, 0xFF, 0xFF, 0xFF, 0xFF, 24);
        func_8004635C(5, 0xFF, 0xFF, 0xFF, 0xFF, 24);
        func_8004635C(6, 0xFF, 0xFF, 0xFF, 0xFF, 24);
        func_8004635C(7, 0xFF, 0xFF, 0xFF, 0xFF, 24);
        func_8004635C(8, 0xFF, 0xFF, 0xFF, 0xFF, 24);
        func_8004635C(9, 0xFF, 0xFF, 0xFF, 0xFF, 24);
        
    } else {
        
        func_8003E77C(0, 0xFF, 0xFF, 0xFF, 0xFF);
        func_8003E77C(1, 0xFF, 0xFF, 0xFF, 0xFF);
        func_8003E77C(2, 0xFF, 0xFF, 0xFF, 0xFF);
        func_8003E77C(3, 0xFF, 0xFF, 0xFF, 0xFF);
        func_8003E77C(4, 0xFF, 0xFF, 0xFF, 0xFF);

        setOverlayScreenSpritesRGBA(0, 0xFF, 0xFF, 0xFF, 0xFF);
        setOverlayScreenSpritesRGBA(1, 0xFF, 0xFF, 0xFF, 0xFF);
        setOverlayScreenSpritesRGBA(2, 0xFF, 0xFF, 0xFF, 0xFF);
        setOverlayScreenSpritesRGBA(3, 0xFF, 0xFF, 0xFF, 0xFF);
        setOverlayScreenSpritesRGBA(4, 0xFF, 0xFF, 0xFF, 0xFF);
        setOverlayScreenSpritesRGBA(5, 0xFF, 0xFF, 0xFF, 0xFF);
        setOverlayScreenSpritesRGBA(6, 0xFF, 0xFF, 0xFF, 0xFF);
        setOverlayScreenSpritesRGBA(7, 0xFF, 0xFF, 0xFF, 0xFF);
        setOverlayScreenSpritesRGBA(8, 0xFF, 0xFF, 0xFF, 0xFF);
        setOverlayScreenSpritesRGBA(9, 0xFF, 0xFF, 0xFF, 0xFF);
        
    }
    
}

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
            
            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_NORTHEAST)) {
                
                if (!set) {
                    
                    if (D_801FB99B[0]) {
                        D_801FB99B[0]--;
                        playSfx(2);
                    }
                    
                    set = TRUE;
                    
                }
                
            }
            
            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTHWEST) && !set) {
                
                if (D_801FB99B[0] != 4) {
                    D_801FB99B[0]++;
                    playSfx(2);
                }
                
                set = TRUE;
                
            }

            func_800B7F6C(D_801FB99B[0]);
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A) && !set) {
                
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
                    
                    deactivateOverlayScreenSprites(0);
                    deactivateOverlayScreenSprites(1);
                    deactivateOverlayScreenSprites(2);
                    deactivateOverlayScreenSprites(3);
                    deactivateOverlayScreenSprites(4);
                    deactivateOverlayScreenSprites(5);
                    deactivateOverlayScreenSprites(6);
                    deactivateOverlayScreenSprites(7);
                    deactivateOverlayScreenSprites(8);
                    deactivateOverlayScreenSprites(9);
                    
                    D_801FB9A0 = 2;
                    playSfx(0);
                    
                }
            
                set = TRUE;
                
            }

            if (checkButtonPressed(CONTROLLER_1, BUTTON_B) && !set) {
                
                deactivateSprite(0x79);
                
                func_800B7CB4();

                func_8003EA1C(0, 0, 0, 0, 0, 24);
                func_8003EA1C(1, 0, 0, 0, 0, 24);
                func_8003EA1C(2, 0, 0, 0, 0, 24);
                func_8003EA1C(3, 0, 0, 0, 0, 24);
                func_8003EA1C(4, 0, 0, 0, 0, 24);
                func_8004635C(0, 0, 0, 0, 0, 24);
                func_8004635C(1, 0, 0, 0, 0, 24);
                func_8004635C(2, 0, 0, 0, 0, 24);
                func_8004635C(3, 0, 0, 0, 0, 24);
                func_8004635C(4, 0, 0, 0, 0, 24);
                func_8004635C(5, 0, 0, 0, 0, 24);
                func_8004635C(6, 0, 0, 0, 0, 24);
                func_8004635C(7, 0, 0, 0, 0, 24);
                func_8004635C(8, 0, 0, 0, 0, 24);
                func_8004635C(9, 0, 0, 0, 0, 24);
                
                D_801FB9A0 = 5;
                playSfx(0);
                
            }

            break;

        case 2:
            func_800E80AC(D_801FB99B[0]);
            func_800B7070(D_801FB99B[0]);
            D_801FB9A0 = 3;
            break;

        case 3:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_B) && !set) {
                
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
                
                deactivateOverlayScreenSprites(0);
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
                
                D_801FB9A0 = 4;
                playSfx(0);
                
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
                
                deactivateOverlayScreenSprites(0);
                deactivateOverlayScreenSprites(1);
                deactivateOverlayScreenSprites(2);
                deactivateOverlayScreenSprites(3);
                deactivateOverlayScreenSprites(4);
                deactivateOverlayScreenSprites(5);
                deactivateOverlayScreenSprites(6);
                deactivateOverlayScreenSprites(7);
                deactivateOverlayScreenSprites(8);
                deactivateOverlayScreenSprites(9);
                
                func_800E1380(1);
                
            }
            
            break;
                
        }
    
}

// load game state from sram and write completion screen strings
INCLUDE_ASM("asm/nonmatchings/game/loadGameScreen", func_800E9550);

// save/cache completion state into sram?
INCLUDE_ASM("asm/nonmatchings/game/loadGameScreen", func_800E9B2C);

//INCLUDE_ASM("asm/nonmatchings/game/loadGameScreen", func_800EA2A4);

void func_800EA2A4(u8 arg0) {
    
    gamePakBuffer[0] = 0;
    gamePakBuffer[1] = 0;
    gamePakBuffer[2] = 0;
    gamePakBuffer[3] = 0;
    gamePakBuffer[4] = 0;
    gamePakBuffer[5] = 0;
    gamePakBuffer[6] = 0;
    gamePakBuffer[7] = 0;
    gamePakBuffer[8] = 0;
    gamePakBuffer[9] = 0;
    gamePakBuffer[10] = 0;
    gamePakBuffer[11] = 0;
    gamePakBuffer[12] = 0;
    gamePakBuffer[13] = 0;
    gamePakBuffer[14] = 0;
    gamePakBuffer[15] = 0;

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

// set strings from game state
void func_800EA3AC(u8 arg0, u8 arg1) {

    // set game state from sram
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
        D_801FB900 = gTotalMilkShipped;
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

// completion score calculation
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
