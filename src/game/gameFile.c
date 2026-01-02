#include "common.h"

#include "game/gameFile.h"

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
#include "game/items.h"
#include "game/level.h"
#include "game/npc.h"
#include "game/overlayScreens.h"
#include "game/player.h"
#include "game/shop.h"
#include "game/tv.h"
#include "game/time.h"
#include "game/title.h"
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
void func_800EBC00();                                  
u8 func_800EBCD8();      
void func_800ED160(u8 arg0, u8 arg1);
void func_800EBEAC(u8, u8);
 
// bss
LoadGameScreenContext loadGameScreenContext;
FarmRankingData gFarmRankingData;
u8 gCurrentGameIndex;
s32 D_8016FDFC;

// unused
u32 D_8018A080;
u32 D_8018A728;
u32 D_801D6F2C;

u32 D_801C3F3C;
u32 D_801C3F40;
u32 D_8016FFF0;
u32 D_8016FFF4;

// unused
u32 D_80204DF0;
u32 D_802226E4;

// data
// "PACKINSOFT FARM2" in ASCII
u8 D_80119740[16] = { 0x50, 0x41, 0x43, 0x4B, 0x49, 0x4E, 0x53, 0x4F, 0x46, 0x54, 0x20, 0x46, 0x41, 0x52, 0x4D, 0x32 };


/* helpers */

//INCLUDE_ASM("asm/nonmatchings/game/gameFile", func_800E1380);

// close farm ranking screen
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

//INCLUDE_ASM("asm/nonmatchings/game/gameFile", func_800E16A0);

void func_800E16A0(void) {

    u32 padding[4];
    
    loadGameScreenContext.gamePakEnabled = FALSE;
    loadGameScreenContext.unk_14 = 0;
    loadGameScreenContext.unk_29 = 0;
    loadGameScreenContext.unk_3C = 0;

}


/* select game functions */

//INCLUDE_ASM("asm/nonmatchings/game/gameFile", func_800E16D0);

void func_800E16D0(u8 arg0, u8 arg1) {

    loadGameScreenContext.diaryHighlighted = arg1;
    
    func_800E1998();
    loadDiarySelectScreen();
    
    if (arg0) {
        fadeInLoadGameScreen();
    } else {
        setLoadGameScreenFullAlpha();
    }
    
    if (loadGameScreenContext.showControllerPakScreen == FALSE) {
        
        if (arg0) {
            
            setMessageBoxRGBA(0, 0, 0, 0, 0);
            setMessageBoxRGBA(1, 0, 0, 0, 0);
            setMessageBoxRGBA(2, 0, 0, 0, 0);
            setMessageBoxRGBA(3, 0, 0, 0, 0);
            setMessageBoxRGBAWithTransition(0, 0xFF, 0xFF, 0xFF, 0xFF, 24);
            setMessageBoxRGBAWithTransition(1, 0xFF, 0xFF, 0xFF, 0xFF, 24);
            setMessageBoxRGBAWithTransition(2, 0xFF, 0xFF, 0xFF, 0xFF, 24);
            setMessageBoxRGBAWithTransition(3, 0xFF, 0xFF, 0xFF, 0xFF, 24);

        } else {

            setMessageBoxRGBA(0, 0, 0, 0, 0);
            setMessageBoxRGBA(1, 0, 0, 0, 0);
            setMessageBoxRGBA(2, 0, 0, 0, 0);
            setMessageBoxRGBA(3, 0, 0, 0, 0);
            setMessageBoxRGBA(0, 0xFF, 0xFF, 0xFF, 0xFF);
            setMessageBoxRGBA(1, 0xFF, 0xFF, 0xFF, 0xFF);
            setMessageBoxRGBA(2, 0xFF, 0xFF, 0xFF, 0xFF);
            setMessageBoxRGBA(3, 0xFF, 0xFF, 0xFF, 0xFF);

        }    
        
    } else {

        if (arg0) {
    
            setMessageBoxRGBA(0, 0, 0, 0, 0);
            setMessageBoxRGBA(1, 0, 0, 0, 0);
            setMessageBoxRGBAWithTransition(0, 0xFF, 0xFF, 0xFF, 0xFF, 24);
            setMessageBoxRGBAWithTransition(1, 0xFF, 0xFF, 0xFF, 0xFF, 24);
    
        } else {
    
            setMessageBoxRGBA(0, 0, 0, 0, 0);
            setMessageBoxRGBA(1, 0, 0, 0, 0);
            setMessageBoxRGBA(0, 0xFF, 0xFF, 0xFF, 0xFF);
            setMessageBoxRGBA(1, 0xFF, 0xFF, 0xFF, 0xFF);
    
        }

    }

}

//INCLUDE_ASM("asm/nonmatchings/game/gameFile", func_800E1998);

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

//INCLUDE_ASM("asm/nonmatchings/game/gameFile", func_800E1A94);

void func_800E1A94(void) {

    if (loadGameScreenContext.showControllerPakScreen == FALSE) {

        if (loadGameScreenContext.flags[0] & 1) { 
            deactivateMessageBox(0);
            initializeEmptyMessageBox(0, (u8*)MESSAGE_BOX_1_TEXT_BUFFER);
            setMessageBoxViewSpacePosition(0, 16.0f, 52.0f, 0);
            setMessageBoxLineAndRowSizes(0, 12, 2);
            setMessageBoxSpacing(0, 0, 2);
            setMessageBoxFont(0, 14, 14, (u8*)FONT_TEXTURE_BUFFER, (u16*)FONT_PALETTE_1_BUFFER);
            setMessageBoxInterpolationWithFlags(0, -4, 2);
            setMessageBoxSpriteIndices(0, 0xFF, 0, 0);
            setMessageBoxButtonMask(0, BUTTON_A);
            setMessageBoxScrollSpeed(0, 1);
            initializeMessageBox(MAIN_MESSAGE_BOX_INDEX, 0, 1, MESSAGE_BOX_MODE_NO_INPUT);
        }

        if (loadGameScreenContext.flags[1] & 1) {
            deactivateMessageBox(1);
            initializeEmptyMessageBox(1, (u8*)MESSAGE_BOX_2_TEXT_BUFFER);
            setMessageBoxViewSpacePosition(1, 16.0f, 8.0f, 0);
            setMessageBoxLineAndRowSizes(1, 12, 2);
            setMessageBoxSpacing(1, 0, 2);
            setMessageBoxFont(1, 14, 14, (u8*)FONT_TEXTURE_BUFFER, (u16*)FONT_PALETTE_1_BUFFER);
            setMessageBoxInterpolationWithFlags(1, -4, 2);
            setMessageBoxSpriteIndices(1, 0xFF, 0, 0);
            setMessageBoxButtonMask(1, BUTTON_A);
            setMessageBoxScrollSpeed(1, 1);
            initializeMessageBox(1, 0, 2, MESSAGE_BOX_MODE_NO_INPUT);
        }

        if (loadGameScreenContext.flags[2] & 1) {
            deactivateMessageBox(2);
            initializeEmptyMessageBox(2, (u8*)MESSAGE_BOX_3_TEXT_BUFFER);
            setMessageBoxViewSpacePosition(2, 16.0f, -36.0f, 0);
            setMessageBoxLineAndRowSizes(2, 12, 2);
            setMessageBoxSpacing(2, 0, 2);
            setMessageBoxFont(2, 14, 14, (u8*)FONT_TEXTURE_BUFFER, (u16*)FONT_PALETTE_1_BUFFER);
            setMessageBoxInterpolationWithFlags(2, -4, 2);
            setMessageBoxSpriteIndices(2, 0xFF, 0, 0);
            setMessageBoxButtonMask(2, BUTTON_A);
            setMessageBoxScrollSpeed(2, 1);
            initializeMessageBox(2, 0, 3, MESSAGE_BOX_MODE_NO_INPUT);
        }

        if (loadGameScreenContext.flags[3] & 1) {
            deactivateMessageBox(3);
            initializeEmptyMessageBox(3, (u8*)MESSAGE_BOX_4_TEXT_BUFFER);
            setMessageBoxViewSpacePosition(3, 16.0f, -80.0f, 0);
            setMessageBoxLineAndRowSizes(3, 12, 2);
            setMessageBoxSpacing(3, 0, 2);
            setMessageBoxFont(3, 14, 14, (u8*)FONT_TEXTURE_BUFFER, (u16*)FONT_PALETTE_1_BUFFER);
            setMessageBoxInterpolationWithFlags(3, -4, 2);
            setMessageBoxSpriteIndices(3, 0xFF, 0, 0);
            setMessageBoxButtonMask(3, BUTTON_A);
            setMessageBoxScrollSpeed(3, 1);
            initializeMessageBox(3, 0, 4, MESSAGE_BOX_MODE_NO_INPUT);
        }

    } else {

        if (loadGameScreenContext.flags[0] & 1) {
            deactivateMessageBox(0);
            initializeEmptyMessageBox(0, (u8*)MESSAGE_BOX_1_TEXT_BUFFER);
            setMessageBoxViewSpacePosition(0, 16.0f, 52.0f, 0);
            setMessageBoxLineAndRowSizes(0, 12, 2);
            setMessageBoxSpacing(0, 0, 2);
            setMessageBoxFont(0, 14, 14, (u8*)FONT_TEXTURE_BUFFER, (u16*)FONT_PALETTE_1_BUFFER);
            setMessageBoxInterpolationWithFlags(0, -4, 2);
            setMessageBoxSpriteIndices(0, 0xFF, 0, 0);
            setMessageBoxButtonMask(0, BUTTON_A);
            setMessageBoxScrollSpeed(0, 1);
            initializeMessageBox(MAIN_MESSAGE_BOX_INDEX, 0, 1, MESSAGE_BOX_MODE_NO_INPUT);
        }
        
        if (loadGameScreenContext.flags[1] & 1) {
            deactivateMessageBox(1);
            initializeEmptyMessageBox(1, (u8*)MESSAGE_BOX_2_TEXT_BUFFER);
            setMessageBoxViewSpacePosition(1, 16.0f, 8.0f, 0);
            setMessageBoxLineAndRowSizes(1, 12, 2);
            setMessageBoxSpacing(1, 0, 2);
            setMessageBoxFont(1, 14, 14, (u8*)FONT_TEXTURE_BUFFER, (u16*)FONT_PALETTE_1_BUFFER);
            setMessageBoxInterpolationWithFlags(1, -4, 2);
            setMessageBoxSpriteIndices(1, 0xFF, 0, 0);
            setMessageBoxButtonMask(1, BUTTON_A);
            setMessageBoxScrollSpeed(1, 1);
            initializeMessageBox(1, 0, 2, MESSAGE_BOX_MODE_NO_INPUT);
        }

    }

}

//INCLUDE_ASM("asm/nonmatchings/game/gameFile", gameSelectCallback);

void gameSelectCallback(void) {

    bool set = FALSE;
    u8 temp;

    switch (loadGameScreenContext.action) {

        // load
        case 0:
            
            if (checkSpriteRGBAUpdateFinished(0x80)) {
                setDiaryCursorPosition(0, loadGameScreenContext.diaryHighlighted, 0xFF);
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
                                
                                closeOverlayScreen();
                                deactivateSprite(0x80);
                                deactivateMessageBox(0);
                                deactivateMessageBox(1);
                                deactivateMessageBox(2);
                                deactivateMessageBox(3);
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
                            closeOverlayScreen();
                            deactivateSprite(0x80);
                            deactivateMessageBox(0);
                            deactivateMessageBox(1);
                            deactivateMessageBox(2);
                            deactivateMessageBox(3);
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
            
            updateDiaryHighlighted(0, 0xFF, FALSE);
            updateDiaryHighlighted(1, 0xFF, FALSE);
            updateDiaryHighlighted(2, 0xFF, FALSE);
            updateDiaryHighlighted(3, 0xFF, FALSE);
            updateDiaryHighlighted(0xFF, 0, FALSE);
            updateDiaryHighlighted(0xFF, 1, FALSE);
            updateDiaryHighlighted(0xFF, 2, FALSE);
            
            updateDiaryHighlighted(loadGameScreenContext.diaryHighlighted, 0xFF, TRUE);
            setDiaryCursorPosition(0, loadGameScreenContext.diaryHighlighted, 0xFF);
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                
                if (!set) {
                    
                    // select diary
                    if (loadGameScreenContext.showControllerPakScreen == FALSE) {
                        animateDiarySelected(loadGameScreenContext.diaryHighlighted, 0xFF);
                        deactivateSprite(0x79);
                        hideDownArrow();
                        hideUpArrow();
                        fadeOutLoadGameScreen();
                        setMessageBoxRGBAWithTransition(0, 0, 0, 0, 0, 0x18);
                        setMessageBoxRGBAWithTransition(1, 0, 0, 0, 0, 0x18);
                        setMessageBoxRGBAWithTransition(2, 0, 0, 0, 0, 0x18);
                        setMessageBoxRGBAWithTransition(3, 0, 0, 0, 0, 0x18);
                        loadGameScreenContext.action = 6;
                        playSfx(0);
                        stopAudioSequenceWithDefaultFadeOut(1);
                    } 
                    
                    set = TRUE;
                
                }
            }
            
            if ((checkButtonPressed(CONTROLLER_1, BUTTON_B)) && !set) {
                
                deactivateSprite(0x79);
                hideDownArrow();
                hideUpArrow();
                fadeOutLoadGameScreen();
                setMessageBoxRGBAWithTransition(0, 0, 0, 0, 0, 0x18);
                setMessageBoxRGBAWithTransition(1, 0, 0, 0, 0, 0x18);
                setMessageBoxRGBAWithTransition(2, 0, 0, 0, 0, 0x18);
                setMessageBoxRGBAWithTransition(3, 0, 0, 0, 0, 0x18);
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
            
            updateDiaryHighlighted(0, 0xFF, FALSE);
            updateDiaryHighlighted(1, 0xFF, FALSE);
            updateDiaryHighlighted(2, 0xFF, FALSE);
            updateDiaryHighlighted(3, 0xFF, FALSE);
            updateDiaryHighlighted(0xFF, 0, FALSE);
            updateDiaryHighlighted(0xFF, 1, FALSE);
            updateDiaryHighlighted(0xFF, 2, FALSE);
            
            updateDiaryHighlighted(0xFF, loadGameScreenContext.actionColumnHighlighted, TRUE);
            setDiaryCursorPosition(0, 0xFF, loadGameScreenContext.actionColumnHighlighted);
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                
                if (!set) {
                    
                    animateDiarySelected(0xFF, loadGameScreenContext.actionColumnHighlighted);
                    
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
                            hideDownArrow();
                            hideUpArrow();
                            fadeOutLoadGameScreen();
                            setMessageBoxRGBAWithTransition(0, 0, 0, 0, 0, 0x18);
                            setMessageBoxRGBAWithTransition(1, 0, 0, 0, 0, 0x18);
                            setMessageBoxRGBAWithTransition(2, 0, 0, 0, 0, 0x18);
                            setMessageBoxRGBAWithTransition(3, 0, 0, 0, 0, 0x18);
                            // load farm ranking screen
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
                hideDownArrow();
                hideUpArrow();
                fadeOutLoadGameScreen();
                setMessageBoxRGBAWithTransition(0, 0, 0, 0, 0, 0x18);
                setMessageBoxRGBAWithTransition(1, 0, 0, 0, 0, 0x18);
                setMessageBoxRGBAWithTransition(2, 0, 0, 0, 0, 0x18);
                setMessageBoxRGBAWithTransition(3, 0, 0, 0, 0, 0x18);
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
                                closeOverlayScreen();
                                deactivateSprite(0x80);
                                deactivateMessageBox(0);
                                deactivateMessageBox(1);
                                deactivateMessageBox(2);
                                deactivateMessageBox(3);
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
                            closeOverlayScreen();
                            deactivateSprite(0x80);
                            deactivateMessageBox(0);
                            deactivateMessageBox(1);
                            deactivateMessageBox(2);
                            deactivateMessageBox(3);
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
    
            updateDiaryHighlighted(0, 0xFF, FALSE);
            updateDiaryHighlighted(1, 0xFF, FALSE);
            updateDiaryHighlighted(2, 0xFF, FALSE);
            updateDiaryHighlighted(3, 0xFF, FALSE);
            updateDiaryHighlighted(loadGameScreenContext.diaryHighlighted, 0xFF, TRUE);
            setDiaryCursorPosition(0, loadGameScreenContext.diaryHighlighted, 0xFF);

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
                                closeOverlayScreen();
                                deactivateSprite(0x80);
                                deactivateMessageBox(0);
                                deactivateMessageBox(1);
                                deactivateMessageBox(2);
                                deactivateMessageBox(3);
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
                            closeOverlayScreen();
                            deactivateSprite(0x80);
                            deactivateMessageBox(0);
                            deactivateMessageBox(1);
                            deactivateMessageBox(2);
                            deactivateMessageBox(3);
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

            updateDiaryHighlighted(0, 0xFF, FALSE);
            updateDiaryHighlighted(1, 0xFF, FALSE);
            updateDiaryHighlighted(2, 0xFF, FALSE);
            updateDiaryHighlighted(3, 0xFF, FALSE);
            updateDiaryHighlighted(loadGameScreenContext.diaryHighlighted, 0xFF, TRUE);
            setDiaryCursorPosition(0, loadGameScreenContext.diaryHighlighted, 0xFF);
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                
                if (!set) {
                    
                    if (loadGameScreenContext.flags[loadGameScreenContext.diaryHighlighted] & 2) {
                        setDiaryCursorPosition(1, loadGameScreenContext.diaryHighlighted, 0xFF);
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
                    setDiaryCursorPosition(1, loadGameScreenContext.unk_86, 0xFF);
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
                                closeOverlayScreen();
                                deactivateSprite(0x80);
                                deactivateMessageBox(0);
                                deactivateMessageBox(1);
                                deactivateMessageBox(2);
                                deactivateMessageBox(3);
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
                    setDiaryCursorPosition(1, loadGameScreenContext.unk_86, 0xFF);
                }
                
                if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_NORTHEAST)) {
                    
                    if (!set) {
                    
                        if (loadGameScreenContext.diaryHighlighted) {
                            loadGameScreenContext.diaryHighlighted--;
                            playSfx(2);
                            set = TRUE;
                        } else {
                            loadGameScreenContext.showControllerPakScreen = FALSE;
                            closeOverlayScreen();
                            deactivateSprite(0x80);
                            deactivateMessageBox(0);
                            deactivateMessageBox(1);
                            deactivateMessageBox(2);
                            deactivateMessageBox(3);
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
            
            updateDiaryHighlighted(0, 0xFF, FALSE);
            updateDiaryHighlighted(1, 0xFF, FALSE);
            updateDiaryHighlighted(2, 0xFF, FALSE);
            updateDiaryHighlighted(3, 0xFF, FALSE);
            updateDiaryHighlighted(loadGameScreenContext.diaryHighlighted, 0xFF, TRUE);
            setDiaryCursorPosition(0, loadGameScreenContext.diaryHighlighted, 0xFF);
            
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
            
            if (checkSpriteRGBAUpdateFinished(0x80) && checkDefaultSequenceChannelOpen(1)) {
                
                closeOverlayScreen();
                deactivateSprite(0x80);
                deactivateMessageBox(0);
                deactivateMessageBox(1);
                deactivateMessageBox(2);
                deactivateMessageBox(3);
                
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
            
            if (checkSpriteRGBAUpdateFinished(0x80) && checkDefaultSequenceChannelOpen(1)) {
                closeOverlayScreen();
                deactivateSprite(0x80);
                deactivateMessageBox(0);
                deactivateMessageBox(1);
                deactivateMessageBox(2);
                deactivateMessageBox(3);
                initializeTitleScreen(1);
            }
            
            break;

        // perform delete
        case 8:
            
            setDiaryCursorPosition(0, 0xFF, 3);
            
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
            
            setDiaryCursorPosition(0, 0xFF, 3);
            
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
            
        // farm ranking screen
        case 10:
            
            if (checkSpriteRGBAUpdateFinished(0x80)) {

                // deactivate overlay screen sprites
                closeOverlayScreen();
                deactivateSprite(0x80);

                // message boxes
                deactivateMessageBox(0);
                deactivateMessageBox(1);
                deactivateMessageBox(2);
                deactivateMessageBox(3);

                loadFarmRankingScreen();

            }
            
            break;
            
        // controller pak load
        case 11:
            
            if (checkSpriteRGBAUpdateFinished(0x80)) {
                setDiaryCursorPosition(0, 0xFF, loadGameScreenContext.actionColumnHighlighted);
                loadGameScreenContext.action = 2; 
            }
            
            break;
            
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/gameFile", func_800E3300);

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

//INCLUDE_ASM("asm/nonmatchings/game/gameFile", func_800E3358);

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

//INCLUDE_ASM("asm/nonmatchings/game/gameFile", func_800E38E8);

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

//INCLUDE_ASM("asm/nonmatchings/game/gameFile", func_800E391C);

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

//INCLUDE_ASM("asm/nonmatchings/game/gameFile", func_800E395C);

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

//INCLUDE_ASM("asm/nonmatchings/game/gameFile", func_800E3990);

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

//INCLUDE_ASM("asm/nonmatchings/game/gameFile", initializeNewGameState);

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
    gCameraRotationOffset = 0;
    
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
        gToolboxSlots[j] = 0;
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
        gFarmAnimals[j].milkType = 0;
        
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

//INCLUDE_ASM("asm/nonmatchings/game/gameFile", func_800E4424);

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
    gCameraRotationOffset = buff->unk_42;

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
    D_801C3F80 = buff->unk_6E;
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
        gToolboxSlots[i] = *((u8*)buff + 0x380 + i);
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
        gFarmAnimals[i].milkType = *((u8*)buff + 0x81E + (i << 5));
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

//INCLUDE_ASM("asm/nonmatchings/game/gameFile", func_800E53E8);

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
    sramBuffer.unk_42 = gCameraRotationOffset;

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
    sramBuffer.unk_6E = D_801C3F80;
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
        *((u8*)buff + 0x380 + i) = gToolboxSlots[i];
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
        *((u8*)buff + 0x81E + (i << 5)) = gFarmAnimals[i].milkType;
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

//INCLUDE_ASM("asm/nonmatchings/game/gameFile", func_800E66A0);

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

//INCLUDE_ASM("asm/nonmatchings/game/gameFile", func_800E67E4);

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


/* farm ranking screen functions */

// INCLUDE_ASM("asm/nonmatchings/game/gameFile", loadFarmRankingScreen);

void loadFarmRankingScreen(void) {

    u8 i = 0;

    // 0 initialize strings
    for (i = 0; i < 7; i++) {
        func_800E6C08(i);
    } 
    
    // abbreviated player names on first screen
    setGameVariableString(0, (u8*)&gFarmRankingData.farmNames[0], 6);
    setGameVariableString(1, (u8*)&gFarmRankingData.farmNames[1], 6);
    setGameVariableString(2, (u8*)&gFarmRankingData.farmNames[2], 6);
    setGameVariableString(3, (u8*)&gFarmRankingData.farmNames[3], 6);
    setGameVariableString(4, (u8*)&gFarmRankingData.farmNames[4], 6);
    
    // load game state from sram and write farm ranking screen strings
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
    loadFarmRankingsListScreen();
    // update sprite rgba
    fadeInFarmRankingScreen();

    setMainLoopCallbackFunctionIndex(FARM_RANKING_SCREEN);
    
    gFarmRankingData.screenState = 0;
    gFarmRankingData.cursorIndex = 0;

    // unused  copies of scores
    D_8018A728 = gFarmRankingData.scores[0];
    D_801D6F2C = gFarmRankingData.scores[1];
    D_8018A080 = gFarmRankingData.scores[2];
    D_802226E4 = gFarmRankingData.scores[3];
    D_80204DF0 = gFarmRankingData.scores[4];
    
}

// INCLUDE_ASM("asm/nonmatchings/game/gameFile", func_800E6C08);

// setup/reset farm ranking screen strings
void func_800E6C08(u8 arg0) {

    u32 *ptr;
    u16* ptr2;
    f32* ptr3;
    u32 *ptr4;
    u32 *ptr5;

    gFarmRankingData.flags[arg0] = 0;

    gFarmRankingData.farmNames[arg0][0] = 0xF6;
    gFarmRankingData.farmNames[arg0][1] = 0xF6;
    gFarmRankingData.farmNames[arg0][2] = 0xF6;
    gFarmRankingData.farmNames[arg0][3] = 0xF6;
    gFarmRankingData.farmNames[arg0][4] = 0xF6;
    gFarmRankingData.farmNames[arg0][5] = 0xF6;

    gFarmRankingData.playerNames[arg0][0] = 0xF6;
    gFarmRankingData.playerNames[arg0][1] = 0xF6;
    gFarmRankingData.playerNames[arg0][2] = 0xF6;
    gFarmRankingData.playerNames[arg0][3] = 0xF6;
    gFarmRankingData.playerNames[arg0][4] = 0xF6;
    gFarmRankingData.playerNames[arg0][5] = 0xF6;
    
    gFarmRankingData.wifeNames[arg0][0] = 0xF6;
    gFarmRankingData.wifeNames[arg0][1] = 0xF6;
    gFarmRankingData.wifeNames[arg0][2] = 0xF6;
    gFarmRankingData.wifeNames[arg0][3] = 0xF6;
    gFarmRankingData.wifeNames[arg0][4] = 0xF6;
    gFarmRankingData.wifeNames[arg0][5] = 0xF6;
        
    gFarmRankingData.babyNames[arg0][0] = 0xF6;
    gFarmRankingData.babyNames[arg0][1] = 0xF6;
    gFarmRankingData.babyNames[arg0][2] = 0xF6;
    gFarmRankingData.babyNames[arg0][3] = 0xF6;
    gFarmRankingData.babyNames[arg0][4] = 0xF6;
    gFarmRankingData.babyNames[arg0][5] = 0xF6;
    
    gFarmRankingData.dogNames[arg0][0] = 0xF6;
    gFarmRankingData.dogNames[arg0][1] = 0xF6;
    gFarmRankingData.dogNames[arg0][2] = 0xF6;
    gFarmRankingData.dogNames[arg0][3] = 0xF6;
    gFarmRankingData.dogNames[arg0][4] = 0xF6;
    gFarmRankingData.dogNames[arg0][5] = 0xF6;

    gFarmRankingData.horseNames[arg0][0] = 0xF6;
    gFarmRankingData.horseNames[arg0][1] = 0xF6;
    gFarmRankingData.horseNames[arg0][2] = 0xF6;
    gFarmRankingData.horseNames[arg0][3] = 0xF6;
    gFarmRankingData.horseNames[arg0][4] = 0xF6;
    gFarmRankingData.horseNames[arg0][5] = 0xF6;

    gFarmRankingData.years[arg0] = 0;
    gFarmRankingData.seasons[arg0] = 0;    
    gFarmRankingData.mariaAffection[arg0] = 0;
    gFarmRankingData.popuriAffection[arg0] = 0;
    gFarmRankingData.elliAffection[arg0] = 0;    
    gFarmRankingData.annAffection[arg0] = 0;
    gFarmRankingData.karenAffection[arg0] = 0;
    gFarmRankingData.harrisAffection[arg0] = 0;    
    gFarmRankingData.grayAffection[arg0] = 0;
    gFarmRankingData.jeffAffection[arg0] = 0;
    gFarmRankingData.cliffAffection[arg0] = 0;    
    gFarmRankingData.kaiAffection[arg0] = 0;
    gFarmRankingData.wifeAffection[arg0] = 0;
    gFarmRankingData.babyAffection[arg0] = 0;
    gFarmRankingData.dogAffection[arg0] = 0;    
    gFarmRankingData.horseAffection[arg0] = 0;
    
    gFarmRankingData.farmAnimalAffection[arg0][0] = 0;
    gFarmRankingData.farmAnimalAffection[arg0][1] = 0;
    gFarmRankingData.farmAnimalAffection[arg0][2] = 0;
    gFarmRankingData.farmAnimalAffection[arg0][3] = 0;
    gFarmRankingData.farmAnimalAffection[arg0][4] = 0;
    gFarmRankingData.farmAnimalAffection[arg0][5] = 0;
    gFarmRankingData.farmAnimalAffection[arg0][6] = 0;
    gFarmRankingData.farmAnimalAffection[arg0][7] = 0;
    
    gFarmRankingData.chickenCounts[arg0] = 0;
    
    gFarmRankingData.cropsShipped[arg0] = 0;
    gFarmRankingData.eggsShipped[arg0] = 0;
    gFarmRankingData.milkShipped[arg0] = 0;
    gFarmRankingData.fishCaught[arg0] = 0;
    
    gFarmRankingData.gold[arg0] = 0;

    gFarmRankingData.grassTiles[arg0] = 0;

    gFarmRankingData.maxStamina[arg0] = 0;
    gFarmRankingData.photoCount[arg0] = 0;
    gFarmRankingData.recipeCount[arg0] = 0;
    gFarmRankingData.houseExtensions[arg0] = 0;
    gFarmRankingData.happiness[arg0] = 0;

    gFarmRankingData.scores[arg0] = 0;

}

// INCLUDE_ASM("asm/nonmatchings/game/gameFile", func_800E6FB4);

// set up farm ranking screen sprites and message boxes
void func_800E6FB4(u8 arg0) {

    deactivateMessageBox(0);
    initializeEmptyMessageBox(0, (u8*)MESSAGE_BOX_1_TEXT_BUFFER);
    setMessageBoxViewSpacePosition(0, -28.0f, 32.0f, 0.0f);
    setMessageBoxLineAndRowSizes(0, 10, 2);
    setMessageBoxSpacing(0, 0, 2);
    setMessageBoxFont(0, 14, 14, (u8*)FONT_TEXTURE_BUFFER, (u16*)FONT_PALETTE_1_BUFFER);
    setMessageBoxInterpolationWithFlags(0, -4, 2);
    setMessageBoxSpriteIndices(0, 0xFF, 0, 0);
    setMessageBoxButtonMask(0, BUTTON_A);
    setMessageBoxScrollSpeed(0, 1);
    initializeMessageBox(0, 0, 7, MESSAGE_BOX_MODE_NO_INPUT);
    
    deactivateMessageBox(1);
    initializeEmptyMessageBox(1, (u8*)MESSAGE_BOX_2_TEXT_BUFFER);
    setMessageBoxViewSpacePosition(1, -28.0f, 0.0f, 0.0f);
    setMessageBoxLineAndRowSizes(1, 10, 2);
    setMessageBoxSpacing(1, 0, 2);
    setMessageBoxFont(1, 14, 14, (u8*)FONT_TEXTURE_BUFFER, (u16*)FONT_PALETTE_1_BUFFER);
    setMessageBoxInterpolationWithFlags(1, -4, 2);
    setMessageBoxSpriteIndices(1, 0xFF, 0, 0);
    setMessageBoxButtonMask(1, BUTTON_A);
    setMessageBoxScrollSpeed(1, 1);
    initializeMessageBox(1, 0, 8, MESSAGE_BOX_MODE_NO_INPUT);
    
    deactivateMessageBox(2);
    initializeEmptyMessageBox(2, (u8*)MESSAGE_BOX_3_TEXT_BUFFER);
    setMessageBoxViewSpacePosition(2, -28.0f, -32.0f, 0.0f);
    setMessageBoxLineAndRowSizes(2, 10, 2);
    setMessageBoxSpacing(2, 0, 2);
    setMessageBoxFont(2, 14, 14, (u8*)FONT_TEXTURE_BUFFER, (u16*)FONT_PALETTE_1_BUFFER);
    setMessageBoxInterpolationWithFlags(2, -4, 2);
    setMessageBoxSpriteIndices(2, 0xFF, 0, 0);
    setMessageBoxButtonMask(2, BUTTON_A);
    setMessageBoxScrollSpeed(2, 1);
    initializeMessageBox(2, 0, 9, MESSAGE_BOX_MODE_NO_INPUT);
    
    deactivateMessageBox(3);
    initializeEmptyMessageBox(3, (u8*)MESSAGE_BOX_4_TEXT_BUFFER);
    setMessageBoxViewSpacePosition(3, -28.0f, -64.0f, 0.0f);
    setMessageBoxLineAndRowSizes(3, 10, 2);
    setMessageBoxSpacing(3, 0, 2);
    setMessageBoxFont(3, 14, 14, (u8*)FONT_TEXTURE_BUFFER, (u16*)FONT_PALETTE_1_BUFFER);
    setMessageBoxInterpolationWithFlags(3, -4, 2);
    setMessageBoxSpriteIndices(3, 0xFF, 0, 0);
    setMessageBoxButtonMask(3, BUTTON_A);
    setMessageBoxScrollSpeed(3, 1);
    initializeMessageBox(3, 0, 10, MESSAGE_BOX_MODE_NO_INPUT);
    
    deactivateMessageBox(4);
    initializeEmptyMessageBox(4, (u8*)MESSAGE_BOX_5_TEXT_BUFFER);
    setMessageBoxViewSpacePosition(4, -28.0f, -96.0f, 0.0f);
    setMessageBoxLineAndRowSizes(4, 10, 2);
    setMessageBoxSpacing(4, 0, 2);
    setMessageBoxFont(4, 14, 14, (u8*)FONT_TEXTURE_BUFFER, (u16*)FONT_PALETTE_1_BUFFER);
    setMessageBoxInterpolationWithFlags(4, -4, 2);
    setMessageBoxSpriteIndices(4, 0xFF, 0, 0);
    setMessageBoxButtonMask(4, BUTTON_A);
    setMessageBoxScrollSpeed(4, 1);
    initializeMessageBox(4, 0, 11, MESSAGE_BOX_MODE_NO_INPUT);

    // render percentages
    if (gFarmRankingData.flags[0] & RANKING_FLAG_ACTIVE) {
        setOverlayScreenSprites(0, 0x89, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, (u8*)FARM_RANKINGS_TEXTURE_BUFFER, (u16*)FARM_RANKINGS_PALETTE_BUFFER, (AnimationFrameMetadata*)FARM_RANKINGS_ANIMATION_FRAME_METADATA_BUFFER, (u32*)FARM_RANKINGS_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, 4, 0, 48.0f, 40.0f, 16.0f, 0xA);
        dmaOverlayScreenSprites(0, gFarmRankingData.years[0], 1, 3);
        setOverlayScreenSprites(5, 0x8B, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, (u8*)FARM_RANKINGS_TEXTURE_BUFFER, (u16*)FARM_RANKINGS_PALETTE_BUFFER, (AnimationFrameMetadata*)FARM_RANKINGS_ANIMATION_FRAME_METADATA_BUFFER, (u32*)FARM_RANKINGS_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, 4, 0, 116.0f, 40.0f, 16.0f, 0xA);
        dmaOverlayScreenSprites(5, gFarmRankingData.scores[0], 2, 3);
    }

    if (gFarmRankingData.flags[1] & RANKING_FLAG_ACTIVE) {
        setOverlayScreenSprites(1, 0x8E, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, (u8*)FARM_RANKINGS_TEXTURE_BUFFER, (u16*)FARM_RANKINGS_PALETTE_BUFFER, (AnimationFrameMetadata*)FARM_RANKINGS_ANIMATION_FRAME_METADATA_BUFFER, (u32*)FARM_RANKINGS_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, 4, 0, 48.0f, 8.0f, 16.0f, 0xA);
        dmaOverlayScreenSprites(1, gFarmRankingData.years[1], 1, 3);
        setOverlayScreenSprites(6, 0x90, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, (u8*)FARM_RANKINGS_TEXTURE_BUFFER, (u16*)FARM_RANKINGS_PALETTE_BUFFER, (AnimationFrameMetadata*)FARM_RANKINGS_ANIMATION_FRAME_METADATA_BUFFER, (u32*)FARM_RANKINGS_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, 4, 0, 116.0f, 8.0f, 16.0f, 0xA);
        dmaOverlayScreenSprites(6, gFarmRankingData.scores[1], 2, 3);
    }
    
    if (gFarmRankingData.flags[2] & RANKING_FLAG_ACTIVE) {
        setOverlayScreenSprites(2, 0x93, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, (u8*)FARM_RANKINGS_TEXTURE_BUFFER, (u16*)FARM_RANKINGS_PALETTE_BUFFER, (AnimationFrameMetadata*)FARM_RANKINGS_ANIMATION_FRAME_METADATA_BUFFER, (u32*)FARM_RANKINGS_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, 4, 0, 48.0f, -24.0f, 16.0f, 0xA);
        dmaOverlayScreenSprites(2, gFarmRankingData.years[2], 1, 3);
        setOverlayScreenSprites(7, 0x95, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, (u8*)FARM_RANKINGS_TEXTURE_BUFFER, (u16*)FARM_RANKINGS_PALETTE_BUFFER, (AnimationFrameMetadata*)FARM_RANKINGS_ANIMATION_FRAME_METADATA_BUFFER, (u32*)FARM_RANKINGS_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, 4, 0, 116.0f, -24.0f, 16.0f, 0xA);
        dmaOverlayScreenSprites(7, gFarmRankingData.scores[2], 2, 3);
    }
    if (gFarmRankingData.flags[3] & RANKING_FLAG_ACTIVE) {
        setOverlayScreenSprites(3, 0x98, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, (u8*)FARM_RANKINGS_TEXTURE_BUFFER, (u16*)FARM_RANKINGS_PALETTE_BUFFER, (AnimationFrameMetadata*)FARM_RANKINGS_ANIMATION_FRAME_METADATA_BUFFER, (u32*)FARM_RANKINGS_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, 4, 0, 48.0f, -56.0f, 16.0f, 0xA);
        dmaOverlayScreenSprites(3, gFarmRankingData.years[3], 1, 3);
        setOverlayScreenSprites(8, 0x9A, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, (u8*)FARM_RANKINGS_TEXTURE_BUFFER, (u16*)FARM_RANKINGS_PALETTE_BUFFER, (AnimationFrameMetadata*)FARM_RANKINGS_ANIMATION_FRAME_METADATA_BUFFER, (u32*)FARM_RANKINGS_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, 4, 0, 116.0f, -56.0f, 16.0f, 0xA);
        dmaOverlayScreenSprites(8, gFarmRankingData.scores[3], 2, 3);
    }
    
    if (gFarmRankingData.flags[4] & RANKING_FLAG_ACTIVE) {
        setOverlayScreenSprites(4, 0x9D, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, (u8*)FARM_RANKINGS_TEXTURE_BUFFER, (u16*)FARM_RANKINGS_PALETTE_BUFFER, (AnimationFrameMetadata*)FARM_RANKINGS_ANIMATION_FRAME_METADATA_BUFFER, (u32*)FARM_RANKINGS_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, 4, 0, 48.0f, -88.0f, 16.0f, 0xA);
        dmaOverlayScreenSprites(4, gFarmRankingData.years[4], 1, 3);
        setOverlayScreenSprites(9, 0x9F, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, (u8*)FARM_RANKINGS_TEXTURE_BUFFER, (u16*)FARM_RANKINGS_PALETTE_BUFFER, (AnimationFrameMetadata*)FARM_RANKINGS_ANIMATION_FRAME_METADATA_BUFFER, (u32*)FARM_RANKINGS_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, 4, 0, 116.0f, -88.0f, 16.0f, 0xA);
        dmaOverlayScreenSprites(9, gFarmRankingData.scores[4], 2, 3);
    }
    
    setMessageBoxRGBA(0, 0, 0, 0, 0);
    setMessageBoxRGBA(1, 0, 0, 0, 0);
    setMessageBoxRGBA(2, 0, 0, 0, 0);
    setMessageBoxRGBA(3, 0, 0, 0, 0);
    setMessageBoxRGBA(4, 0, 0, 0, 0);

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
        
        setMessageBoxRGBAWithTransition(0, 0xFF, 0xFF, 0xFF, 0xFF, 24);
        setMessageBoxRGBAWithTransition(1, 0xFF, 0xFF, 0xFF, 0xFF, 24);
        setMessageBoxRGBAWithTransition(2, 0xFF, 0xFF, 0xFF, 0xFF, 24);
        setMessageBoxRGBAWithTransition(3, 0xFF, 0xFF, 0xFF, 0xFF, 24);
        setMessageBoxRGBAWithTransition(4, 0xFF, 0xFF, 0xFF, 0xFF, 24);
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
        
        setMessageBoxRGBA(0, 0xFF, 0xFF, 0xFF, 0xFF);
        setMessageBoxRGBA(1, 0xFF, 0xFF, 0xFF, 0xFF);
        setMessageBoxRGBA(2, 0xFF, 0xFF, 0xFF, 0xFF);
        setMessageBoxRGBA(3, 0xFF, 0xFF, 0xFF, 0xFF);
        setMessageBoxRGBA(4, 0xFF, 0xFF, 0xFF, 0xFF);

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

//INCLUDE_ASM("asm/nonmatchings/game/gameFile", func_800E80AC);

void func_800E80AC(u8 arg0) {

    setGameVariableString(5, (u8*)&gFarmRankingData.farmNames[arg0], 6);
    setGameVariableString(6, (u8*)&gFarmRankingData.farmNames[arg0 + 7], 6);
    setGameVariableString(7, (u8*)&gFarmRankingData.farmNames[arg0 + 14], 6);
    setGameVariableString(8, (u8*)&gFarmRankingData.farmNames[arg0 + 21], 6);
    setGameVariableString(9, (u8*)&gFarmRankingData.farmNames[arg0 + 28], 6);
    setGameVariableString(10, (u8*)&gFarmRankingData.farmNames[arg0 + 35], 6);

    deactivateMessageBox(0);
    initializeEmptyMessageBox(0, (u8*)MESSAGE_BOX_1_TEXT_BUFFER);
    setMessageBoxViewSpacePosition(0, -28.0f, 72.0f, 0.0f);
    setMessageBoxLineAndRowSizes(0, 0xA, 2);
    setMessageBoxSpacing(0, 0, 2);
    setMessageBoxFont(0, 0xE, 0xE, (u8*)FONT_TEXTURE_BUFFER, (u16*)FONT_PALETTE_1_BUFFER);
    setMessageBoxInterpolationWithFlags(0, -4, 2);

    setMessageBoxSpriteIndices(0, 0xFF, 0, 0);
    setMessageBoxButtonMask(0, 0x8000);
    setMessageBoxScrollSpeed(0, 1);
    initializeMessageBox(0, 0, 0xC, MESSAGE_BOX_MODE_NO_INPUT);
    deactivateMessageBox(1);
    initializeEmptyMessageBox(1, (u8*)MESSAGE_BOX_2_TEXT_BUFFER);
    setMessageBoxViewSpacePosition(1, -40.0f, 40.0f, 0.0f);
    setMessageBoxLineAndRowSizes(1, 0xA, 2);
    setMessageBoxSpacing(1, 0, 2);
    setMessageBoxFont(1, 0xE, 0xE, (u8*)FONT_TEXTURE_BUFFER, (u16*)FONT_PALETTE_1_BUFFER);
    setMessageBoxInterpolationWithFlags(1, -4, 2);
    setMessageBoxSpriteIndices(1, 0xFF, 0, 0);
    setMessageBoxButtonMask(1, 0x8000);
    setMessageBoxScrollSpeed(1, 1);
    initializeMessageBox(1, 0, 0xD, MESSAGE_BOX_MODE_NO_INPUT);

    if (gFarmRankingData.flags[arg0] & 2) {
        
        deactivateMessageBox(2);
        initializeEmptyMessageBox(2, (u8*)MESSAGE_BOX_3_TEXT_BUFFER);
        setMessageBoxViewSpacePosition(2, -40.0f, 24.0f, 0.0f);
        setMessageBoxLineAndRowSizes(2, 0xA, 2);
        setMessageBoxSpacing(2, 0, 2);
        setMessageBoxFont(2, 0xE, 0xE, (u8*)FONT_TEXTURE_BUFFER, (u16*)FONT_PALETTE_1_BUFFER);
        setMessageBoxInterpolationWithFlags(2, -4, 2);
        setMessageBoxSpriteIndices(2, 0xFF, 0, 0);
        setMessageBoxButtonMask(2, 0x8000);
        setMessageBoxScrollSpeed(2, 1);
        initializeMessageBox(2, 0, 0xE, MESSAGE_BOX_MODE_NO_INPUT);
    
    }

    if (gFarmRankingData.flags[arg0] & 4) {
        deactivateMessageBox(3);
        initializeEmptyMessageBox(3, (u8*)MESSAGE_BOX_4_TEXT_BUFFER);
        setMessageBoxViewSpacePosition(3, -40.0f, 8.0f, 0.0f);
        setMessageBoxLineAndRowSizes(3, 0xA, 2);
        setMessageBoxSpacing(3, 0, 2);
        setMessageBoxFont(3, 0xE, 0xE, (u8*)FONT_TEXTURE_BUFFER, (u16*)FONT_PALETTE_1_BUFFER);
        setMessageBoxInterpolationWithFlags(3, -4, 2);
        setMessageBoxSpriteIndices(3, 0xFF, 0, 0);
        setMessageBoxButtonMask(3, 0x8000);
        setMessageBoxScrollSpeed(3, 1);
        initializeMessageBox(3, 0, 0xF, MESSAGE_BOX_MODE_NO_INPUT);
    }
    
    deactivateMessageBox(4);
    initializeEmptyMessageBox(4, (u8*)MESSAGE_BOX_5_TEXT_BUFFER);
    setMessageBoxViewSpacePosition(4, -40.0f, -8.0f, 0.0f);
    setMessageBoxLineAndRowSizes(4, 0xA, 2);
    setMessageBoxSpacing(4, 0, 2);
    setMessageBoxFont(4, 0xE, 0xE, (u8*)FONT_TEXTURE_BUFFER, (u16*)FONT_PALETTE_1_BUFFER);
    setMessageBoxInterpolationWithFlags(4, -4, 2);
    setMessageBoxSpriteIndices(4, 0xFF, 0, 0);
    setMessageBoxButtonMask(4, 0x8000);
    setMessageBoxScrollSpeed(4, 1);
    initializeMessageBox(4, 0, 0x10, MESSAGE_BOX_MODE_NO_INPUT);

    if (gFarmRankingData.flags[arg0] & 8) {
        deactivateMessageBox(5);
        initializeEmptyMessageBox(5, (u8*)MESSAGE_BOX_6_TEXT_BUFFER);
        setMessageBoxViewSpacePosition(5, -40.0f, -24.0f, 0.0f);
        setMessageBoxLineAndRowSizes(5, 0xA, 2);
        setMessageBoxSpacing(5, 0, 2);
        setMessageBoxFont(5, 0xE, 0xE, (u8*)FONT_TEXTURE_BUFFER, (u16*)FONT_PALETTE_1_BUFFER);
        setMessageBoxInterpolationWithFlags(5, -4, 2);
        setMessageBoxSpriteIndices(5, 0xFF, 0, 0);
        setMessageBoxButtonMask(5, 0x8000);
        setMessageBoxScrollSpeed(5, 1);
        initializeMessageBox(5, 0, 0x11, MESSAGE_BOX_MODE_NO_INPUT);
    }
    
    setOverlayScreenSprites(0, 0x89, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, (u8*)FARM_RANKINGS_TEXTURE_BUFFER, (u16*)FARM_RANKINGS_PALETTE_BUFFER, (AnimationFrameMetadata*)FARM_RANKINGS_ANIMATION_FRAME_METADATA_BUFFER, (u32*)FARM_RANKINGS_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, 4, 0, 48.0f, 80.0f, 16.0f, 0xA);
    dmaOverlayScreenSprites(0, gFarmRankingData.years[arg0], 1, 3);
    
    setOverlayScreenSprites(1, 0x8B, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, (u8*)FARM_RANKINGS_TEXTURE_BUFFER, (u16*)FARM_RANKINGS_PALETTE_BUFFER, (AnimationFrameMetadata*)FARM_RANKINGS_ANIMATION_FRAME_METADATA_BUFFER, (u32*)FARM_RANKINGS_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, 4, 0, 116.0f, 80.0f, 16.0f, 0xA);
    dmaOverlayScreenSprites(1, gFarmRankingData.scores[arg0], 2, 3);

    setOverlayScreenSprites(2, 0xA4, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, (u8*)FARM_RANKINGS_TEXTURE_BUFFER, (u16*)FARM_RANKINGS_PALETTE_BUFFER, (AnimationFrameMetadata*)FARM_RANKINGS_ANIMATION_FRAME_METADATA_BUFFER, (u32*)FARM_RANKINGS_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, 4, 0, -26.0f, -32.0f, 16.0f, 0xA);
    dmaOverlayScreenSprites(2, gFarmRankingData.photoCount[arg0], 1, 3);
    
    setOverlayScreenSprites(3, 0xA6, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, (u8*)FARM_RANKINGS_TEXTURE_BUFFER, (u16*)FARM_RANKINGS_PALETTE_BUFFER, (AnimationFrameMetadata*)FARM_RANKINGS_ANIMATION_FRAME_METADATA_BUFFER, (u32*)FARM_RANKINGS_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, 4, 0, -26.0f, -48.0f, 16.0f, 0xA);
    dmaOverlayScreenSprites(3, gFarmRankingData.recipeCount[arg0], 1, 3);
    
    setOverlayScreenSprites(4, 0xA8, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, (u8*)FARM_RANKINGS_TEXTURE_BUFFER, (u16*)FARM_RANKINGS_PALETTE_BUFFER, (AnimationFrameMetadata*)FARM_RANKINGS_ANIMATION_FRAME_METADATA_BUFFER, (u32*)FARM_RANKINGS_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, 4, 0, -26.0f, -64.0f, 16.0f, 0xA);
    dmaOverlayScreenSprites(4, (gFarmRankingData.houseExtensions[arg0] * 100) / 6, 2, 3);
    
    setOverlayScreenSprites(5, 0xAB, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, (u8*)FARM_RANKINGS_TEXTURE_BUFFER, (u16*)FARM_RANKINGS_PALETTE_BUFFER, (AnimationFrameMetadata*)FARM_RANKINGS_ANIMATION_FRAME_METADATA_BUFFER, (u32*)FARM_RANKINGS_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, 4, 0, -26.0f, -80.0f, 16.0f, 0xA);
    dmaOverlayScreenSprites(5, gFarmRankingData.grassTiles[arg0], 2, 3);
    
    setOverlayScreenSprites(6, 0x8E, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, (u8*)FARM_RANKINGS_TEXTURE_BUFFER, (u16*)FARM_RANKINGS_PALETTE_BUFFER, (AnimationFrameMetadata*)FARM_RANKINGS_ANIMATION_FRAME_METADATA_BUFFER, (u32*)FARM_RANKINGS_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, 4, 0, 98.0f, 34.0f, 16.0f, 0xA);
    dmaOverlayScreenSprites(6, gFarmRankingData.cropsShipped[arg0], 4, 3);
    
    setOverlayScreenSprites(7, 0x93, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, (u8*)FARM_RANKINGS_TEXTURE_BUFFER, (u16*)FARM_RANKINGS_PALETTE_BUFFER, (AnimationFrameMetadata*)FARM_RANKINGS_ANIMATION_FRAME_METADATA_BUFFER, (u32*)FARM_RANKINGS_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, 4, 0, 98.0f, 6.0f, 16.0f, 0xA);
    dmaOverlayScreenSprites(7, gFarmRankingData.milkShipped[arg0], 3, 3);
    
    setOverlayScreenSprites(8, 0x97, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, (u8*)FARM_RANKINGS_TEXTURE_BUFFER, (u16*)FARM_RANKINGS_PALETTE_BUFFER, (AnimationFrameMetadata*)FARM_RANKINGS_ANIMATION_FRAME_METADATA_BUFFER, (u32*)FARM_RANKINGS_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, 4, 0, 98.0f, -22.0f, 16.0f, 0xA);
    dmaOverlayScreenSprites(8, gFarmRankingData.eggsShipped[arg0], 3, 3);
    
    setOverlayScreenSprites(9, 0x9B, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, (u8*)FARM_RANKINGS_TEXTURE_BUFFER, (u16*)FARM_RANKINGS_PALETTE_BUFFER, (AnimationFrameMetadata*)FARM_RANKINGS_ANIMATION_FRAME_METADATA_BUFFER, (u32*)FARM_RANKINGS_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, 4, 0, 98.0f, -50.0f, 16.0f, 0xA);
    dmaOverlayScreenSprites(9, gFarmRankingData.fishCaught[arg0], 2, 3);
    
    setOverlayScreenSprites(0xA, 0x9E, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, (u8*)FARM_RANKINGS_TEXTURE_BUFFER, (u16*)FARM_RANKINGS_PALETTE_BUFFER, (AnimationFrameMetadata*)FARM_RANKINGS_ANIMATION_FRAME_METADATA_BUFFER, (u32*)FARM_RANKINGS_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, 4, 0, 98.0f, -78.0f, 16.0f, 0xA);
    dmaOverlayScreenSprites(0xA, gFarmRankingData.gold[arg0], 5, 3);

    setMessageBoxRGBA(0, 0xFF, 0xFF, 0xFF, 0xFF);
    setMessageBoxRGBA(1, 0xFF, 0xFF, 0xFF, 0xFF);
    setMessageBoxRGBA(2, 0xFF, 0xFF, 0xFF, 0xFF);
    setMessageBoxRGBA(3, 0xFF, 0xFF, 0xFF, 0xFF);
    setMessageBoxRGBA(4, 0xFF, 0xFF, 0xFF, 0xFF);
    setMessageBoxRGBA(5, 0xFF, 0xFF, 0xFF, 0xFF);
    
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
    setOverlayScreenSpritesRGBA(10, 0xFF, 0xFF, 0xFF, 0xFF);
    
}

// INCLUDE_ASM("asm/nonmatchings/game/gameFile", farmRankingScreenCallback);

// farm ranking screen main loop callback
void farmRankingScreenCallback(void) {

    bool set = FALSE;

    switch (gFarmRankingData.screenState) {

        case 0:
            
            if (checkSpriteRGBAUpdateFinished(0x80)) {
                setFarmRankingCursorPosition(gFarmRankingData.cursorIndex);
                gFarmRankingData.screenState = 1;
            }
            
            break;

        case 1:
            
            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_NORTHEAST)) {
                
                if (!set) {
                    
                    if (gFarmRankingData.cursorIndex) {
                        gFarmRankingData.cursorIndex--;
                        playSfx(2);
                    }
                    
                    set = TRUE;
                    
                }
                
            }
            
            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTHWEST) && !set) {
                
                if (gFarmRankingData.cursorIndex != 4) {
                    gFarmRankingData.cursorIndex++;
                    playSfx(2);
                }
                
                set = TRUE;
                
            }

            setFarmRankingCursorPosition(gFarmRankingData.cursorIndex);
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A) && !set) {
                
                if (gFarmRankingData.flags[gFarmRankingData.cursorIndex] & RANKING_FLAG_ACTIVE) {
                    
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
                    
                    deactivateMessageBox(0);
                    deactivateMessageBox(1);
                    deactivateMessageBox(2);
                    deactivateMessageBox(3);
                    deactivateMessageBox(4);
                    
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
                    
                    gFarmRankingData.screenState = 2;
                    playSfx(0);
                    
                }
            
                set = TRUE;
                
            }

            if (checkButtonPressed(CONTROLLER_1, BUTTON_B) && !set) {
                
                deactivateSprite(0x79);
                
                fadeOutFarmRankingScreen();

                setMessageBoxRGBAWithTransition(0, 0, 0, 0, 0, 24);
                setMessageBoxRGBAWithTransition(1, 0, 0, 0, 0, 24);
                setMessageBoxRGBAWithTransition(2, 0, 0, 0, 0, 24);
                setMessageBoxRGBAWithTransition(3, 0, 0, 0, 0, 24);
                setMessageBoxRGBAWithTransition(4, 0, 0, 0, 0, 24);
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
                
                gFarmRankingData.screenState = 5;
                playSfx(0);
                
            }

            break;

        case 2:
            func_800E80AC(gFarmRankingData.cursorIndex);
            loadIndividualRankingScreen(gFarmRankingData.cursorIndex);
            gFarmRankingData.screenState = 3;
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
                
                deactivateMessageBox(0);
                deactivateMessageBox(1);
                deactivateMessageBox(2);
                deactivateMessageBox(3);
                deactivateMessageBox(4);
                deactivateMessageBox(5);
                
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
                
                gFarmRankingData.screenState = 4;
                playSfx(0);
                
            }
            
            break;
        
        case 4:
            func_800E6FB4(1);
            loadFarmRankingsListScreen();
            setFarmRankingScreenFullAlpha();

            gFarmRankingData.screenState = 1;
            break;

        case 5:
            
            if (checkSpriteRGBAUpdateFinished(0x80)) {
                
                closeOverlayScreen();
                
                deactivateSprite(0x80);
                
                deactivateMessageBox(0);
                deactivateMessageBox(1);
                deactivateMessageBox(2);
                deactivateMessageBox(3);
                deactivateMessageBox(4);
                
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
                
                func_800E1380(TRUE);
                
            }
            
            break;
                
        }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/gameFile", func_800E9550);

void func_800E9550(u8 slot) {

    FarmRankingsBuffer* buff = (FarmRankingsBuffer*)FARM_RANKINGS_BUFFER;
    u32 devAddr = ((slot << 8) + 0x4000) | 0x08000000;

    func_8004DC48(devAddr, FARM_RANKINGS_BUFFER, 0x100);

    if (func_800E3300(buff->signature)) {
        
        if (func_800E3990((u8*)buff)) {

            gFarmRankingData.flags[slot] = 1;
            gFarmRankingData.flags[slot] = buff->flags;

            gFarmRankingData.farmNames[slot][0] = buff->farmName[0];
            gFarmRankingData.farmNames[slot][1] = buff->farmName[1];
            gFarmRankingData.farmNames[slot][2] = buff->farmName[2];
            gFarmRankingData.farmNames[slot][3] = buff->farmName[3];
            gFarmRankingData.farmNames[slot][4] = buff->farmName[4];
            gFarmRankingData.farmNames[slot][5] = buff->farmName[5];

            gFarmRankingData.playerNames[slot][0] = buff->playerName[0];
            gFarmRankingData.playerNames[slot][1] = buff->playerName[1];
            gFarmRankingData.playerNames[slot][2] = buff->playerName[2];
            gFarmRankingData.playerNames[slot][3] = buff->playerName[3];
            gFarmRankingData.playerNames[slot][4] = buff->playerName[4];
            gFarmRankingData.playerNames[slot][5] = buff->playerName[5];

            gFarmRankingData.wifeNames[slot][0] = buff->wifeName[0];
            gFarmRankingData.wifeNames[slot][1] = buff->wifeName[1];
            gFarmRankingData.wifeNames[slot][2] = buff->wifeName[2];
            gFarmRankingData.wifeNames[slot][3] = buff->wifeName[3];
            gFarmRankingData.wifeNames[slot][4] = buff->wifeName[4];
            gFarmRankingData.wifeNames[slot][5] = buff->wifeName[5];

            gFarmRankingData.babyNames[slot][0] = buff->babyName[0];
            gFarmRankingData.babyNames[slot][1] = buff->babyName[1];
            gFarmRankingData.babyNames[slot][2] = buff->babyName[2];
            gFarmRankingData.babyNames[slot][3] = buff->babyName[3];
            gFarmRankingData.babyNames[slot][4] = buff->babyName[4];
            gFarmRankingData.babyNames[slot][5] = buff->babyName[5];

            gFarmRankingData.dogNames[slot][0] = buff->dogName[0];
            gFarmRankingData.dogNames[slot][1] = buff->dogName[1];
            gFarmRankingData.dogNames[slot][2] = buff->dogName[2];
            gFarmRankingData.dogNames[slot][3] = buff->dogName[3];
            gFarmRankingData.dogNames[slot][4] = buff->dogName[4];
            gFarmRankingData.dogNames[slot][5] = buff->dogName[5];

            gFarmRankingData.horseNames[slot][0] = buff->horseName[0];
            gFarmRankingData.horseNames[slot][1] = buff->horseName[1];
            gFarmRankingData.horseNames[slot][2] = buff->horseName[2];
            gFarmRankingData.horseNames[slot][3] = buff->horseName[3];
            gFarmRankingData.horseNames[slot][4] = buff->horseName[4];
            gFarmRankingData.horseNames[slot][5] = buff->horseName[5];

            gFarmRankingData.years[slot] = buff->year;
            gFarmRankingData.seasons[slot] = buff->season;

            gFarmRankingData.mariaAffection[slot] = buff->mariaAffection;
            gFarmRankingData.popuriAffection[slot] = buff->popuriAffection;
            gFarmRankingData.elliAffection[slot] = buff->elliAffection;
            gFarmRankingData.annAffection[slot] = buff->annAffection;
            gFarmRankingData.karenAffection[slot] = buff->karenAffection;
            gFarmRankingData.harrisAffection[slot] = buff->harrisAffection;
            gFarmRankingData.grayAffection[slot] = buff->grayAffection;
            gFarmRankingData.jeffAffection[slot] = buff->jeffAffection;
            gFarmRankingData.cliffAffection[slot] = buff->cliffAffection;
            gFarmRankingData.kaiAffection[slot] = buff->kaiAffection;
            gFarmRankingData.wifeAffection[slot] = buff->wifeAffection;
            gFarmRankingData.babyAffection[slot] = buff->babyAffection;
            gFarmRankingData.dogAffection[slot] = buff->dogAffection;
            gFarmRankingData.horseAffection[slot] = buff->horseAffection;

            gFarmRankingData.farmAnimalAffection[slot][0] = buff->farmAnimalAffection[0];
            gFarmRankingData.farmAnimalAffection[slot][1] = buff->farmAnimalAffection[1];
            gFarmRankingData.farmAnimalAffection[slot][2] = buff->farmAnimalAffection[2];
            gFarmRankingData.farmAnimalAffection[slot][3] = buff->farmAnimalAffection[3];
            gFarmRankingData.farmAnimalAffection[slot][4] = buff->farmAnimalAffection[4];
            gFarmRankingData.farmAnimalAffection[slot][5] = buff->farmAnimalAffection[5];
            gFarmRankingData.farmAnimalAffection[slot][6] = buff->farmAnimalAffection[6];
            gFarmRankingData.farmAnimalAffection[slot][7] = buff->farmAnimalAffection[7];

            gFarmRankingData.chickenCounts[slot] = buff->chickenCount;
            gFarmRankingData.wife[slot] = buff->wife;

            gFarmRankingData.cropsShipped[slot] = buff->cropsShipped;
            gFarmRankingData.eggsShipped[slot] = buff->eggsShipped;
            gFarmRankingData.milkShipped[slot] = buff->milkShipped;
            gFarmRankingData.fishCaught[slot] = buff->fishCaught;
            gFarmRankingData.gold[slot] = buff->gold;

            gFarmRankingData.grassTiles[slot] = buff->grassTiles;
            gFarmRankingData.maxStamina[slot] = buff->maxStamina;
            gFarmRankingData.photoCount[slot] = buff->photoCount;
            gFarmRankingData.recipeCount[slot] = buff->recipeCount;
            gFarmRankingData.houseExtensions[slot] = buff->houseExtensions;
            gFarmRankingData.happiness[slot] = buff->happiness;
            gFarmRankingData.scores[slot] = buff->score;

            return;
            
        }
        
    }

    gFarmRankingData.flags[slot] = 0;

}

// save/cache top completion state into sram
//INCLUDE_ASM("asm/nonmatchings/game/gameFile", func_800E9B2C);

void func_800E9B2C(u8 slot) {

    u32 temp = (slot << 8) + 0x4000;
    u32 devAddr = temp + 0x08000000;

    u32 temp1;
    u32 temp2;
    u32 temp3;
    u32 temp4;
    u32 temp5;
    u16 temp6;
    
    FarmRankingsBuffer* buff = (FarmRankingsBuffer*)FARM_RANKINGS_BUFFER;

    // Write signature
    buff->signature[0] = D_80119740[0];
    farmRankingsBuffer.signature[1] = D_80119740[1];
    farmRankingsBuffer.signature[2] = D_80119740[2];
    farmRankingsBuffer.signature[3] = D_80119740[3];
    farmRankingsBuffer.signature[4] = D_80119740[4];
    farmRankingsBuffer.signature[5] = D_80119740[5];
    farmRankingsBuffer.signature[6] = D_80119740[6];
    farmRankingsBuffer.signature[7] = D_80119740[7];
    farmRankingsBuffer.signature[8] = D_80119740[8];
    farmRankingsBuffer.signature[9] = D_80119740[9];
    farmRankingsBuffer.signature[10] = D_80119740[10];
    farmRankingsBuffer.signature[11] = D_80119740[11];
    farmRankingsBuffer.signature[12] = D_80119740[12];
    farmRankingsBuffer.signature[13] = D_80119740[13];
    farmRankingsBuffer.signature[14] = D_80119740[14];
    farmRankingsBuffer.signature[15] = D_80119740[15];

    farmRankingsBuffer.flags = gFarmRankingData.flags[slot];

    farmRankingsBuffer.farmName[0] = gFarmRankingData.farmNames[slot][0];
    farmRankingsBuffer.farmName[1] = gFarmRankingData.farmNames[slot][1];
    farmRankingsBuffer.farmName[2] = gFarmRankingData.farmNames[slot][2];
    farmRankingsBuffer.farmName[3] = gFarmRankingData.farmNames[slot][3];
    farmRankingsBuffer.farmName[4] = gFarmRankingData.farmNames[slot][4];
    farmRankingsBuffer.farmName[5] = gFarmRankingData.farmNames[slot][5];

    farmRankingsBuffer.playerName[0] = gFarmRankingData.playerNames[slot][0];
    farmRankingsBuffer.playerName[1] = gFarmRankingData.playerNames[slot][1];
    farmRankingsBuffer.playerName[2] = gFarmRankingData.playerNames[slot][2];
    farmRankingsBuffer.playerName[3] = gFarmRankingData.playerNames[slot][3];
    farmRankingsBuffer.playerName[4] = gFarmRankingData.playerNames[slot][4];
    farmRankingsBuffer.playerName[5] = gFarmRankingData.playerNames[slot][5];

    farmRankingsBuffer.wifeName[0] = gFarmRankingData.wifeNames[slot][0];
    farmRankingsBuffer.wifeName[1] = gFarmRankingData.wifeNames[slot][1];
    farmRankingsBuffer.wifeName[2] = gFarmRankingData.wifeNames[slot][2];
    farmRankingsBuffer.wifeName[3] = gFarmRankingData.wifeNames[slot][3];
    farmRankingsBuffer.wifeName[4] = gFarmRankingData.wifeNames[slot][4];
    farmRankingsBuffer.wifeName[5] = gFarmRankingData.wifeNames[slot][5];

    farmRankingsBuffer.babyName[0] = gFarmRankingData.babyNames[slot][0];
    farmRankingsBuffer.babyName[1] = gFarmRankingData.babyNames[slot][1];
    farmRankingsBuffer.babyName[2] = gFarmRankingData.babyNames[slot][2];
    farmRankingsBuffer.babyName[3] = gFarmRankingData.babyNames[slot][3];
    farmRankingsBuffer.babyName[4] = gFarmRankingData.babyNames[slot][4];
    farmRankingsBuffer.babyName[5] = gFarmRankingData.babyNames[slot][5];

    farmRankingsBuffer.dogName[0] = gFarmRankingData.dogNames[slot][0];
    farmRankingsBuffer.dogName[1] = gFarmRankingData.dogNames[slot][1];
    farmRankingsBuffer.dogName[2] = gFarmRankingData.dogNames[slot][2];
    farmRankingsBuffer.dogName[3] = gFarmRankingData.dogNames[slot][3];
    farmRankingsBuffer.dogName[4] = gFarmRankingData.dogNames[slot][4];
    farmRankingsBuffer.dogName[5] = gFarmRankingData.dogNames[slot][5];

    farmRankingsBuffer.horseName[0] = gFarmRankingData.horseNames[slot][0];
    farmRankingsBuffer.horseName[1] = gFarmRankingData.horseNames[slot][1];
    farmRankingsBuffer.horseName[2] = gFarmRankingData.horseNames[slot][2];
    farmRankingsBuffer.horseName[3] = gFarmRankingData.horseNames[slot][3];
    farmRankingsBuffer.horseName[4] = gFarmRankingData.horseNames[slot][4];
    farmRankingsBuffer.horseName[5] = gFarmRankingData.horseNames[slot][5];

    farmRankingsBuffer.year = gFarmRankingData.years[slot];
    farmRankingsBuffer.season = gFarmRankingData.seasons[slot];

    farmRankingsBuffer.mariaAffection = gFarmRankingData.mariaAffection[slot];
    farmRankingsBuffer.popuriAffection = gFarmRankingData.popuriAffection[slot];
    farmRankingsBuffer.elliAffection = gFarmRankingData.elliAffection[slot];
    farmRankingsBuffer.annAffection = gFarmRankingData.annAffection[slot];
    farmRankingsBuffer.karenAffection = gFarmRankingData.karenAffection[slot];
    farmRankingsBuffer.harrisAffection = gFarmRankingData.harrisAffection[slot];
    farmRankingsBuffer.grayAffection = gFarmRankingData.grayAffection[slot];
    farmRankingsBuffer.jeffAffection = gFarmRankingData.jeffAffection[slot];
    farmRankingsBuffer.cliffAffection = gFarmRankingData.cliffAffection[slot];
    farmRankingsBuffer.kaiAffection = gFarmRankingData.kaiAffection[slot];
    farmRankingsBuffer.wifeAffection = gFarmRankingData.wifeAffection[slot];
    farmRankingsBuffer.babyAffection = gFarmRankingData.babyAffection[slot];
    farmRankingsBuffer.dogAffection = gFarmRankingData.dogAffection[slot];
    farmRankingsBuffer.horseAffection = gFarmRankingData.horseAffection[slot];

    farmRankingsBuffer.farmAnimalAffection[0] = gFarmRankingData.farmAnimalAffection[slot][0];
    farmRankingsBuffer.farmAnimalAffection[1] = gFarmRankingData.farmAnimalAffection[slot][1];
    farmRankingsBuffer.farmAnimalAffection[2] = gFarmRankingData.farmAnimalAffection[slot][2];
    farmRankingsBuffer.farmAnimalAffection[3] = gFarmRankingData.farmAnimalAffection[slot][3];
    farmRankingsBuffer.farmAnimalAffection[4] = gFarmRankingData.farmAnimalAffection[slot][4];
    farmRankingsBuffer.farmAnimalAffection[5] = gFarmRankingData.farmAnimalAffection[slot][5];
    farmRankingsBuffer.farmAnimalAffection[6] = gFarmRankingData.farmAnimalAffection[slot][6];
    farmRankingsBuffer.farmAnimalAffection[7] = gFarmRankingData.farmAnimalAffection[slot][7];

    farmRankingsBuffer.chickenCount = gFarmRankingData.chickenCounts[slot];
    farmRankingsBuffer.wife = gFarmRankingData.wife[slot];

    temp1 = gFarmRankingData.cropsShipped[slot];
    temp2 = gFarmRankingData.eggsShipped[slot];
    temp3 = gFarmRankingData.milkShipped[slot];
    temp4 = gFarmRankingData.fishCaught[slot];
    temp5 = gFarmRankingData.gold[slot];
    temp6 = gFarmRankingData.grassTiles[slot];

    farmRankingsBuffer.cropsShipped = temp1;
    farmRankingsBuffer.eggsShipped = temp2;
    farmRankingsBuffer.milkShipped  = temp3;
    farmRankingsBuffer.fishCaught = temp4;
    farmRankingsBuffer.gold = temp5;

    farmRankingsBuffer.grassTiles = temp6;
    
    farmRankingsBuffer.maxStamina = gFarmRankingData.maxStamina[slot];
    farmRankingsBuffer.photoCount = gFarmRankingData.photoCount[slot];
    farmRankingsBuffer.recipeCount = gFarmRankingData.recipeCount[slot];
    farmRankingsBuffer.houseExtensions = gFarmRankingData.houseExtensions[slot];
    farmRankingsBuffer.happiness = gFarmRankingData.happiness[slot];
    
    farmRankingsBuffer.score = gFarmRankingData.scores[slot];
    
    // Calculate and store checksum
    buff->checksum = func_800E395C(buff->signature);

    // Write to SRAM
    func_8004DD7C(devAddr, buff, 0x100);

}

// INCLUDE_ASM("asm/nonmatchings/game/gameFile", func_800EA2A4);

void func_800EA2A4(u8 arg0) { 
    
    farmRankingsBuffer.signature[0] = 0;
    farmRankingsBuffer.signature[1] = 0;
    farmRankingsBuffer.signature[2] = 0;
    farmRankingsBuffer.signature[3] = 0;
    farmRankingsBuffer.signature[4] = 0;
    farmRankingsBuffer.signature[5] = 0;
    farmRankingsBuffer.signature[6] = 0;
    farmRankingsBuffer.signature[7] = 0;
    farmRankingsBuffer.signature[8] = 0;
    farmRankingsBuffer.signature[9] = 0;
    farmRankingsBuffer.signature[10] = 0;
    farmRankingsBuffer.signature[11] = 0;
    farmRankingsBuffer.signature[12] = 0;
    farmRankingsBuffer.signature[13] = 0;
    farmRankingsBuffer.signature[14] = 0;
    farmRankingsBuffer.signature[15] = 0;

    func_8004DD7C(((arg0 * 0x100) + 0x4000) | 0x08000000, (FarmRankingsBuffer*)FARM_RANKINGS_BUFFER, 0x10);

} 

// INCLUDE_ASM("asm/nonmatchings/game/gameFile", func_800EA360);

u8 func_800EA360(void) {

    u8 count = 0;

    if (gFarmRankingData.flags[0] & RANKING_FLAG_ACTIVE) {

loop:
        if (count < 5) {
            
            count++;
            
            if (gFarmRankingData.flags[count] & RANKING_FLAG_ACTIVE) {
                goto loop;
            }
            
        } 
    }

    return count;
    
}

static inline void setBabyAffection() {
    if (checkLifeEventBit(HAVE_BABY)) {
        gFarmRankingData.babyAffection[RANKING_TEMP_SLOT] = npcAffection[BABY];
        gFarmRankingData.tempFlags |= 4;
    } else {
        gFarmRankingData.babyAffection[RANKING_TEMP_SLOT] = 0;
    }
}

static inline void setHorseAffection() {
    if (checkLifeEventBit(HAVE_HORSE)) {
        gFarmRankingData.horseAffection[RANKING_TEMP_SLOT] = horseInfo.affection;
        gFarmRankingData.tempFlags |= 8;
    } else {
        gFarmRankingData.horseAffection[RANKING_TEMP_SLOT] = 0;
    }
}

// INCLUDE_ASM("asm/nonmatchings/game/gameFile", func_800EA3AC);

// set strings from game state
void func_800EA3AC(u8 arg0, u8 arg1) {

    // set game state from sram
    if (func_800E4424(arg0, arg1)) {

        gFarmRankingData.tempFlags = 1;
        
        gFarmRankingData.tempFarmName[0] = gFarmName[0];
        gFarmRankingData.tempFarmName[1] = gFarmName[1];
        gFarmRankingData.tempFarmName[2] = gFarmName[2];
        gFarmRankingData.tempFarmName[3] = gFarmName[3];
        gFarmRankingData.tempFarmName[4] = gFarmName[4];
        gFarmRankingData.tempFarmName[5] = gFarmName[5];
        
        gFarmRankingData.tempPlayerName[0] = gPlayer.name[0];
        gFarmRankingData.tempPlayerName[1] = gPlayer.name[1];
        gFarmRankingData.tempPlayerName[2] = gPlayer.name[2];
        gFarmRankingData.tempPlayerName[3] = gPlayer.name[3];
        gFarmRankingData.tempPlayerName[4] = gPlayer.name[4];
        gFarmRankingData.tempPlayerName[5] = gPlayer.name[5];
        
        gFarmRankingData.wife[RANKING_TEMP_SLOT] = gWife;

        if (checkLifeEventBit(MARRIED)) {
            
            setWifeNameString(gFarmRankingData.wife[RANKING_TEMP_SLOT]);
            
            gFarmRankingData.tempWifeName[0] = gWifeName[0];
            gFarmRankingData.tempWifeName[1] = gWifeName[1];
            gFarmRankingData.tempWifeName[2] = gWifeName[2];
            gFarmRankingData.tempWifeName[3] = gWifeName[3];
            gFarmRankingData.tempWifeName[4] = gWifeName[4];
            gFarmRankingData.tempWifeName[5] = gWifeName[5];
            
        }

        if (checkLifeEventBit(HAVE_BABY)) {
                
            gFarmRankingData.tempBabyName[0] = gBabyName[0];
            gFarmRankingData.tempBabyName[1] = gBabyName[1];
            gFarmRankingData.tempBabyName[2] = gBabyName[2];
            gFarmRankingData.tempBabyName[3] = gBabyName[3];
            gFarmRankingData.tempBabyName[4] = gBabyName[4];
            gFarmRankingData.tempBabyName[5] = gBabyName[5];
            
        }

        gFarmRankingData.tempDogName[0] = dogInfo.name[0];
        gFarmRankingData.tempDogName[1] = dogInfo.name[1];
        gFarmRankingData.tempDogName[2] = dogInfo.name[2];
        gFarmRankingData.tempDogName[3] = dogInfo.name[3];
        gFarmRankingData.tempDogName[4] = dogInfo.name[4];
        gFarmRankingData.tempDogName[5] = dogInfo.name[5];

        if (checkLifeEventBit(HAVE_HORSE)) {
            
            gFarmRankingData.tempHorseName[0] = horseInfo.name[0];
            gFarmRankingData.tempHorseName[1] = horseInfo.name[1];
            gFarmRankingData.tempHorseName[2] = horseInfo.name[2];
            gFarmRankingData.tempHorseName[3] = horseInfo.name[3];
            gFarmRankingData.tempHorseName[4] = horseInfo.name[4];
            gFarmRankingData.tempHorseName[5] = horseInfo.name[5];
            
        }

        gFarmRankingData.years[RANKING_TEMP_SLOT] = gYear;
        gFarmRankingData.seasons[RANKING_TEMP_SLOT] = gSeason;
        
        gFarmRankingData.mariaAffection[RANKING_TEMP_SLOT] = npcAffection[MARIA];
        gFarmRankingData.popuriAffection[RANKING_TEMP_SLOT] = npcAffection[POPURI];
        gFarmRankingData.elliAffection[RANKING_TEMP_SLOT] = npcAffection[ELLI];
        gFarmRankingData.annAffection[RANKING_TEMP_SLOT] = npcAffection[ANN];
        gFarmRankingData.karenAffection[RANKING_TEMP_SLOT] = npcAffection[KAREN];
        gFarmRankingData.harrisAffection[RANKING_TEMP_SLOT] = npcAffection[HARRIS];
        gFarmRankingData.grayAffection[RANKING_TEMP_SLOT] = npcAffection[GRAY];
        gFarmRankingData.jeffAffection[RANKING_TEMP_SLOT] = npcAffection[JEFF];
        gFarmRankingData.cliffAffection[RANKING_TEMP_SLOT] = npcAffection[CLIFF];
        gFarmRankingData.kaiAffection[RANKING_TEMP_SLOT] = npcAffection[KAI];

        if (checkLifeEventBit(MARRIED)) {
            gFarmRankingData.wifeAffection[RANKING_TEMP_SLOT] = npcAffection[gFarmRankingData.wife[RANKING_TEMP_SLOT]];
            gFarmRankingData.tempFlags |= 2;
        } else {
            gFarmRankingData.wifeAffection[RANKING_TEMP_SLOT] = 0;
        }

        setBabyAffection();
        
        gFarmRankingData.dogAffection[RANKING_TEMP_SLOT] = dogInfo.affection;

        setHorseAffection();
        
        if (gFarmAnimals[0].flags & 1) {
            gFarmRankingData.tempFarmAnimalAffection[0] = gFarmAnimals[0].affection;
        } else {
            gFarmRankingData.tempFarmAnimalAffection[0] = 0;
        }
                
        if (gFarmAnimals[1].flags & 1) {
            gFarmRankingData.tempFarmAnimalAffection[1] = gFarmAnimals[1].affection;
        } else {
            gFarmRankingData.tempFarmAnimalAffection[1] = 0;
        }
                
        if (gFarmAnimals[2].flags & 1) {
            gFarmRankingData.tempFarmAnimalAffection[2] = gFarmAnimals[2].affection;
        } else {
            gFarmRankingData.tempFarmAnimalAffection[2] = 0;
        }
                
        if (gFarmAnimals[3].flags & 1) {
            gFarmRankingData.tempFarmAnimalAffection[3] = gFarmAnimals[3].affection;
        } else {
            gFarmRankingData.tempFarmAnimalAffection[3] = 0;
        }
                
        if (gFarmAnimals[4].flags & 1) {
            gFarmRankingData.tempFarmAnimalAffection[4] = gFarmAnimals[4].affection;
        } else {
            gFarmRankingData.tempFarmAnimalAffection[4] = 0;
        }
                
        if (gFarmAnimals[5].flags & 1) {
            gFarmRankingData.tempFarmAnimalAffection[5] = gFarmAnimals[5].affection;
        } else {
            gFarmRankingData.tempFarmAnimalAffection[5] = 0;
        }
                        
        if (gFarmAnimals[6].flags & 1) {
            gFarmRankingData.tempFarmAnimalAffection[6] = gFarmAnimals[6].affection;
        } else {
            gFarmRankingData.tempFarmAnimalAffection[6] = 0;
        }
                
        if (gFarmAnimals[7].flags & 1) {
            gFarmRankingData.tempFarmAnimalAffection[7] = gFarmAnimals[7].affection;
        } else {
            gFarmRankingData.tempFarmAnimalAffection[7] = 0;
        }

        gFarmRankingData.chickenCounts[RANKING_TEMP_SLOT] = func_8009B564();

        gFarmRankingData.cropsShipped[RANKING_TEMP_SLOT] = gTotalCropsShipped;
        gFarmRankingData.eggsShipped[RANKING_TEMP_SLOT] = gTotalEggsShipped;
        gFarmRankingData.milkShipped[RANKING_TEMP_SLOT] = gTotalMilkShipped;
        gFarmRankingData.fishCaught[RANKING_TEMP_SLOT] = totalFishCaught;
        gFarmRankingData.gold[RANKING_TEMP_SLOT] = gGold;

        gFarmRankingData.maxStamina[RANKING_TEMP_SLOT] = gMaximumStamina;
        gFarmRankingData.happiness[RANKING_TEMP_SLOT] = gHappiness;
        
        func_800EB74C(5);
        func_800EB788(5);
        func_800EBA90(5);
        func_800EBAC8(5);
        func_800EAA9C(5);
        
        if (!(func_800EBCD8())) {
            func_800EBC00();
        }
        
    } else {
        gFarmRankingData.tempFlags = 0;
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/gameFile", func_800EAA9C);

void func_800EAA9C(u8 slot) {
    
    f32 totalScore;
    s32 finalIntScore;
    
    gFarmRankingData.scores[slot] = 0.0f;

    totalScore = (f32)(f64)(((u32)gFarmRankingData.mariaAffection[slot] * 100) / 255);

    totalScore += (f32)(f64)(((u32)gFarmRankingData.popuriAffection[slot] * 100) / 255);

    totalScore += (f32)(f64)(((u32)gFarmRankingData.elliAffection[slot] * 100) / 255);

    totalScore += (f32)(f64)(((u32)gFarmRankingData.annAffection[slot] * 100) / 255);

    totalScore += (f32)(f64)(((u32)gFarmRankingData.karenAffection[slot] * 100) / 255);

    totalScore += (f32)(f64)(((u32)gFarmRankingData.harrisAffection[slot] * 100) / 255);

    totalScore += (f32)(f64)(((u32)gFarmRankingData.grayAffection[slot] * 100) / 255);

    totalScore += (f32)(f64)(((u32)gFarmRankingData.jeffAffection[slot] * 100) / 255);

    totalScore += (f32)(f64)(((u32)gFarmRankingData.cliffAffection[slot] * 100) / 255);

    totalScore += (f32)(f64)(((u32)gFarmRankingData.kaiAffection[slot] * 100) / 255);

    totalScore += (f32)(f64)(((u32)gFarmRankingData.kaiAffection[slot] * 100) / 255);

    totalScore += (f32)(f64)(((u32)gFarmRankingData.babyAffection[slot] * 100) / 255);

    totalScore += (f32)(f64)(((u32)gFarmRankingData.dogAffection[slot] * 100) / 255);

    totalScore += (f32)(f64)(((u32)gFarmRankingData.horseAffection[slot] * 100) / 255);

    totalScore += (f32)(f64)(((u32)gFarmRankingData.farmAnimalAffection[slot][0] * 100) / 255);
    totalScore += (f32)(f64)(((u32)gFarmRankingData.farmAnimalAffection[slot][1] * 100) / 255);
    totalScore += (f32)(f64)(((u32)gFarmRankingData.farmAnimalAffection[slot][2] * 100) / 255);
    totalScore += (f32)(f64)(((u32)gFarmRankingData.farmAnimalAffection[slot][3] * 100) / 255);
    totalScore += (f32)(f64)(((u32)gFarmRankingData.farmAnimalAffection[slot][4] * 100) / 255);
    totalScore += (f32)(f64)(((u32)gFarmRankingData.farmAnimalAffection[slot][5] * 100) / 255);
    totalScore += (f32)(f64)(((u32)gFarmRankingData.farmAnimalAffection[slot][6] * 100) / 255);
    totalScore += (f32)(f64)(((u32)gFarmRankingData.farmAnimalAffection[slot][7] * 100) / 255);

    totalScore += (f32)(f64)(((u32)gFarmRankingData.chickenCounts[slot] * 100) / 6);

    totalScore += (f32)(f64)(((u32)gFarmRankingData.cropsShipped[slot] * 1000) / 99999);

    totalScore += (f32)(f64)(((u32)gFarmRankingData.eggsShipped[slot] * 500) / 9999);

    totalScore += (f32)(f64)(((u32)gFarmRankingData.milkShipped[slot] * 500) / 9999);

    totalScore += (f32)(f64)(((u32)gFarmRankingData.fishCaught[slot] * 500) / 999);

    totalScore += (f32)(f64)(((u32)gFarmRankingData.gold[slot] * 1000) / 999999);

    totalScore += (f32)(f64)(((u32)gFarmRankingData.maxStamina[slot] * 100) / 250);

    totalScore += (f32)(f64)(((u32)gFarmRankingData.photoCount[slot] * 25) / 4);

    totalScore += (f32)(f64)(((u32)gFarmRankingData.recipeCount[slot] * 100) / 35);

    totalScore += (f32)(f64)(((u32)gFarmRankingData.houseExtensions[slot] * 100) / 6);

    totalScore += (f32)(f64)(((u32)gFarmRankingData.grassTiles[slot] * 400) / 480);

    totalScore += (f32)(f64)(((u32)gFarmRankingData.happiness[slot] * 400) / 255);

    D_8016FDFC = (s32)(u32)totalScore;
    
    gFarmRankingData.scores[slot] = totalScore / 70.0f;
    
}

// INCLUDE_ASM("asm/nonmatchings/game/gameFile", func_800EB74C);

void func_800EB74C(u8 arg0) {
    gFarmRankingData.grassTiles[arg0] = getFarmGrassTilesSum();
}

// INCLUDE_ASM("asm/nonmatchings/game/gameFile", func_800EB788);

void func_800EB788(u8 arg0) {

    gFarmRankingData.photoCount[arg0] = 0;

    if (albumBits & 1) {
        gFarmRankingData.photoCount[arg0]++;
    }

    if (albumBits & 2) {
        gFarmRankingData.photoCount[arg0]++;
    }

    if (albumBits & 4) {
        gFarmRankingData.photoCount[arg0]++;
    }

    if (albumBits & 8) {
        gFarmRankingData.photoCount[arg0]++;
    }

    if (albumBits & 0x10) {
        gFarmRankingData.photoCount[arg0]++;
    }

    if (albumBits & 0x20) {
        gFarmRankingData.photoCount[arg0]++;
    }

    if (albumBits & 0x40) {
        gFarmRankingData.photoCount[arg0]++;
    }
    
    if (albumBits & 0x80) {
        gFarmRankingData.photoCount[arg0]++;
    }

    if (albumBits & 0x100) {
        gFarmRankingData.photoCount[arg0]++;
    }
    
    if (albumBits & 0x200) {
        gFarmRankingData.photoCount[arg0]++;
    }
    
    if (albumBits & 0x400) {
        gFarmRankingData.photoCount[arg0]++;
    }

    if (albumBits & 0x800) {
        gFarmRankingData.photoCount[arg0]++;
    }

    if (albumBits & 0x1000) {
        gFarmRankingData.photoCount[arg0]++;
    }
    
    if (albumBits & 0x2000) {
        gFarmRankingData.photoCount[arg0]++;
    }

    if (albumBits & 0x4000) {
        gFarmRankingData.photoCount[arg0]++;
    }

    if (albumBits & 0x8000) {
        gFarmRankingData.photoCount[arg0]++;
    }
    
}

// INCLUDE_ASM("asm/nonmatchings/game/gameFile", func_800EBA90);

void func_800EBA90(u8 arg0) {
    gFarmRankingData.recipeCount[arg0] = getAcquiredRecipesTotal();
}

// INCLUDE_ASM("asm/nonmatchings/game/gameFile", func_800EBAC8);

void func_800EBAC8(u8 arg0) {

    gFarmRankingData.houseExtensions[arg0] = 0;
    
    if (checkLifeEventBit(HAVE_KITCHEN)) {
        gFarmRankingData.houseExtensions[arg0]++;
    }
    if (checkLifeEventBit(HAVE_BATHROOM)) {
        gFarmRankingData.houseExtensions[arg0]++;
    }
    if (checkLifeEventBit(HAVE_STAIRS)) {
        gFarmRankingData.houseExtensions[arg0]++;
    }
    if (checkLifeEventBit(HAVE_GREENHOUSE)) {
        gFarmRankingData.houseExtensions[arg0]++;
    }
    if (checkLifeEventBit(HAVE_LOG_TERRACE)) {
        gFarmRankingData.houseExtensions[arg0]++;
    }
    if (checkLifeEventBit(HAVE_BABY_BED)) {
        gFarmRankingData.houseExtensions[arg0]++;
    }
    
}

// INCLUDE_ASM("asm/nonmatchings/game/gameFile", func_800EBC00);

// get highest percentage
void func_800EBC00(void) {

    f32 f1, f2;
    u8 i = 0;
    u8 j;
    u8 temp = 0;

    while (i < 5) {
        
        temp = i;

        f1 = gFarmRankingData.scores[i];

        j = i + 1;
        
        while (j < 6) {

            f2 = gFarmRankingData.scores[j];

            if ((f1 < f2 || !(gFarmRankingData.flags[temp] & RANKING_FLAG_ACTIVE)) && gFarmRankingData.flags[j] & RANKING_FLAG_ACTIVE) {
                temp = j;
                f1 = f2;
            }

            j++;

        } 
        
        // swap highest
        func_800EBEAC(i, temp);

        i++;
        
    } 
    
}

// INCLUDE_ASM("asm/nonmatchings/game/gameFile", func_800EBCD8);

u8 func_800EBCD8(void) {
    
    bool found = FALSE;
    u8 i = 0;

    while (i < FARM_RANKING_SLOTS && !found) {

        if (gFarmRankingData.tempFarmName[0] == gFarmRankingData.farmNames[i][0] &&
            gFarmRankingData.tempFarmName[1] == gFarmRankingData.farmNames[i][1] &&
            gFarmRankingData.tempFarmName[2] == gFarmRankingData.farmNames[i][2] &&
            gFarmRankingData.tempFarmName[3] == gFarmRankingData.farmNames[i][3] &&
            gFarmRankingData.tempFarmName[4] == gFarmRankingData.farmNames[i][4] &&
            gFarmRankingData.tempFarmName[5] == gFarmRankingData.farmNames[i][5] &&
            gFarmRankingData.tempPlayerName[0] == gFarmRankingData.playerNames[i][0] &&
            gFarmRankingData.tempPlayerName[1] == gFarmRankingData.playerNames[i][1] &&
            gFarmRankingData.tempPlayerName[2] == gFarmRankingData.playerNames[i][2] &&
            gFarmRankingData.tempPlayerName[3] == gFarmRankingData.playerNames[i][3] &&
            gFarmRankingData.tempPlayerName[4] == gFarmRankingData.playerNames[i][4] &&
            gFarmRankingData.tempPlayerName[5] == gFarmRankingData.playerNames[i][5]) {

            if (gFarmRankingData.scores[RANKING_TEMP_SLOT] >= gFarmRankingData.scores[i]) {
                func_800ED160(i, RANKING_TEMP_SLOT);
            } else {
                found = TRUE;
            }
            
        }

        i++;

    } 

    return found;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/gameFile", func_800EBEAC);

void func_800EBEAC(u8 slotA, u8 slotB) {

    gFarmRankingData.swapFlags = gFarmRankingData.flags[slotA];

    gFarmRankingData.swapFarmName[0] = gFarmRankingData.farmNames[slotA][0];
    gFarmRankingData.swapFarmName[1] = gFarmRankingData.farmNames[slotA][1];
    gFarmRankingData.swapFarmName[2] = gFarmRankingData.farmNames[slotA][2];
    gFarmRankingData.swapFarmName[3] = gFarmRankingData.farmNames[slotA][3];
    gFarmRankingData.swapFarmName[4] = gFarmRankingData.farmNames[slotA][4];
    gFarmRankingData.swapFarmName[5] = gFarmRankingData.farmNames[slotA][5];

    gFarmRankingData.swapPlayerName[0] = gFarmRankingData.playerNames[slotA][0];
    gFarmRankingData.swapPlayerName[1] = gFarmRankingData.playerNames[slotA][1];
    gFarmRankingData.swapPlayerName[2] = gFarmRankingData.playerNames[slotA][2];
    gFarmRankingData.swapPlayerName[3] = gFarmRankingData.playerNames[slotA][3];
    gFarmRankingData.swapPlayerName[4] = gFarmRankingData.playerNames[slotA][4];
    gFarmRankingData.swapPlayerName[5] = gFarmRankingData.playerNames[slotA][5];

    gFarmRankingData.swapWifeName[0] = gFarmRankingData.wifeNames[slotA][0];
    gFarmRankingData.swapWifeName[1] = gFarmRankingData.wifeNames[slotA][1];
    gFarmRankingData.swapWifeName[2] = gFarmRankingData.wifeNames[slotA][2];
    gFarmRankingData.swapWifeName[3] = gFarmRankingData.wifeNames[slotA][3];
    gFarmRankingData.swapWifeName[4] = gFarmRankingData.wifeNames[slotA][4];
    gFarmRankingData.swapWifeName[5] = gFarmRankingData.wifeNames[slotA][5];

    gFarmRankingData.swapBabyName[0] = gFarmRankingData.babyNames[slotA][0];
    gFarmRankingData.swapBabyName[1] = gFarmRankingData.babyNames[slotA][1];
    gFarmRankingData.swapBabyName[2] = gFarmRankingData.babyNames[slotA][2];
    gFarmRankingData.swapBabyName[3] = gFarmRankingData.babyNames[slotA][3];
    gFarmRankingData.swapBabyName[4] = gFarmRankingData.babyNames[slotA][4];
    gFarmRankingData.swapBabyName[5] = gFarmRankingData.babyNames[slotA][5];

    gFarmRankingData.swapDogName[0] = gFarmRankingData.dogNames[slotA][0];
    gFarmRankingData.swapDogName[1] = gFarmRankingData.dogNames[slotA][1];
    gFarmRankingData.swapDogName[2] = gFarmRankingData.dogNames[slotA][2];
    gFarmRankingData.swapDogName[3] = gFarmRankingData.dogNames[slotA][3];
    gFarmRankingData.swapDogName[4] = gFarmRankingData.dogNames[slotA][4];
    gFarmRankingData.swapDogName[5] = gFarmRankingData.dogNames[slotA][5];

    gFarmRankingData.swapHorseName[0] = gFarmRankingData.horseNames[slotA][0];
    gFarmRankingData.swapHorseName[1] = gFarmRankingData.horseNames[slotA][1];
    gFarmRankingData.swapHorseName[2] = gFarmRankingData.horseNames[slotA][2];
    gFarmRankingData.swapHorseName[3] = gFarmRankingData.horseNames[slotA][3];
    gFarmRankingData.swapHorseName[4] = gFarmRankingData.horseNames[slotA][4];
    gFarmRankingData.swapHorseName[5] = gFarmRankingData.horseNames[slotA][5];

    gFarmRankingData.years[RANKING_SWAP_SLOT] = gFarmRankingData.years[slotA];
    gFarmRankingData.seasons[RANKING_SWAP_SLOT] = gFarmRankingData.seasons[slotA];

    gFarmRankingData.mariaAffection[RANKING_SWAP_SLOT] = gFarmRankingData.mariaAffection[slotA];
    gFarmRankingData.popuriAffection[RANKING_SWAP_SLOT] = gFarmRankingData.popuriAffection[slotA];
    gFarmRankingData.elliAffection[RANKING_SWAP_SLOT] = gFarmRankingData.elliAffection[slotA];
    gFarmRankingData.annAffection[RANKING_SWAP_SLOT] = gFarmRankingData.annAffection[slotA];
    gFarmRankingData.karenAffection[RANKING_SWAP_SLOT] = gFarmRankingData.karenAffection[slotA];
    gFarmRankingData.harrisAffection[RANKING_SWAP_SLOT] = gFarmRankingData.harrisAffection[slotA];
    gFarmRankingData.grayAffection[RANKING_SWAP_SLOT] = gFarmRankingData.grayAffection[slotA];
    gFarmRankingData.jeffAffection[RANKING_SWAP_SLOT] = gFarmRankingData.jeffAffection[slotA];
    gFarmRankingData.cliffAffection[RANKING_SWAP_SLOT] = gFarmRankingData.cliffAffection[slotA];
    gFarmRankingData.kaiAffection[RANKING_SWAP_SLOT] = gFarmRankingData.kaiAffection[slotA];
    gFarmRankingData.wifeAffection[RANKING_SWAP_SLOT] = gFarmRankingData.wifeAffection[slotA];
    gFarmRankingData.babyAffection[RANKING_SWAP_SLOT] = gFarmRankingData.babyAffection[slotA];
    gFarmRankingData.dogAffection[RANKING_SWAP_SLOT] = gFarmRankingData.dogAffection[slotA];
    gFarmRankingData.horseAffection[RANKING_SWAP_SLOT] = gFarmRankingData.horseAffection[slotA];

    gFarmRankingData.swapFarmAnimalAffection1[0] = gFarmRankingData.farmAnimalAffection[slotA][0];
    gFarmRankingData.swapFarmAnimalAffection1[1] = gFarmRankingData.farmAnimalAffection[slotA][1];
    gFarmRankingData.swapFarmAnimalAffection1[2] = gFarmRankingData.farmAnimalAffection[slotA][2];
    gFarmRankingData.swapFarmAnimalAffection1[3] = gFarmRankingData.farmAnimalAffection[slotA][3];
    gFarmRankingData.swapFarmAnimalAffection2[0] = gFarmRankingData.farmAnimalAffection[slotA][4];
    gFarmRankingData.swapFarmAnimalAffection2[1] = gFarmRankingData.farmAnimalAffection[slotA][5];
    gFarmRankingData.swapFarmAnimalAffection2[2] = gFarmRankingData.farmAnimalAffection[slotA][6];
    gFarmRankingData.swapFarmAnimalAffection2[3] = gFarmRankingData.farmAnimalAffection[slotA][7];

    gFarmRankingData.chickenCounts[RANKING_SWAP_SLOT] = gFarmRankingData.chickenCounts[slotA];
    gFarmRankingData.wife[RANKING_SWAP_SLOT] = gFarmRankingData.wife[slotA];

    gFarmRankingData.cropsShipped[RANKING_SWAP_SLOT] = gFarmRankingData.cropsShipped[slotA];
    gFarmRankingData.eggsShipped[RANKING_SWAP_SLOT] = gFarmRankingData.eggsShipped[slotA];
    gFarmRankingData.milkShipped[RANKING_SWAP_SLOT] = gFarmRankingData.milkShipped[slotA];
    gFarmRankingData.fishCaught[RANKING_SWAP_SLOT] = gFarmRankingData.fishCaught[slotA];
    gFarmRankingData.gold[RANKING_SWAP_SLOT] = gFarmRankingData.gold[slotA];

    gFarmRankingData.grassTiles[RANKING_SWAP_SLOT] = gFarmRankingData.grassTiles[slotA];
    gFarmRankingData.maxStamina[RANKING_SWAP_SLOT] = gFarmRankingData.maxStamina[slotA];
    gFarmRankingData.photoCount[RANKING_SWAP_SLOT] = gFarmRankingData.photoCount[slotA];
    gFarmRankingData.recipeCount[RANKING_SWAP_SLOT] = gFarmRankingData.recipeCount[slotA];
    gFarmRankingData.houseExtensions[RANKING_SWAP_SLOT] = gFarmRankingData.houseExtensions[slotA];
    gFarmRankingData.happiness[RANKING_SWAP_SLOT] = gFarmRankingData.happiness[slotA];
    gFarmRankingData.scores[RANKING_SWAP_SLOT] = gFarmRankingData.scores[slotA];

    // Step 2: Copy slotB to slotA
    gFarmRankingData.flags[slotA] = gFarmRankingData.flags[slotB];

    gFarmRankingData.farmNames[slotA][0] = gFarmRankingData.farmNames[slotB][0];
    gFarmRankingData.farmNames[slotA][1] = gFarmRankingData.farmNames[slotB][1];
    gFarmRankingData.farmNames[slotA][2] = gFarmRankingData.farmNames[slotB][2];
    gFarmRankingData.farmNames[slotA][3] = gFarmRankingData.farmNames[slotB][3];
    gFarmRankingData.farmNames[slotA][4] = gFarmRankingData.farmNames[slotB][4];
    gFarmRankingData.farmNames[slotA][5] = gFarmRankingData.farmNames[slotB][5];

    gFarmRankingData.playerNames[slotA][0] = gFarmRankingData.playerNames[slotB][0];
    gFarmRankingData.playerNames[slotA][1] = gFarmRankingData.playerNames[slotB][1];
    gFarmRankingData.playerNames[slotA][2] = gFarmRankingData.playerNames[slotB][2];
    gFarmRankingData.playerNames[slotA][3] = gFarmRankingData.playerNames[slotB][3];
    gFarmRankingData.playerNames[slotA][4] = gFarmRankingData.playerNames[slotB][4];
    gFarmRankingData.playerNames[slotA][5] = gFarmRankingData.playerNames[slotB][5];

    gFarmRankingData.wifeNames[slotA][0] = gFarmRankingData.wifeNames[slotB][0];
    gFarmRankingData.wifeNames[slotA][1] = gFarmRankingData.wifeNames[slotB][1];
    gFarmRankingData.wifeNames[slotA][2] = gFarmRankingData.wifeNames[slotB][2];
    gFarmRankingData.wifeNames[slotA][3] = gFarmRankingData.wifeNames[slotB][3];
    gFarmRankingData.wifeNames[slotA][4] = gFarmRankingData.wifeNames[slotB][4];
    gFarmRankingData.wifeNames[slotA][5] = gFarmRankingData.wifeNames[slotB][5];

    gFarmRankingData.babyNames[slotA][0] = gFarmRankingData.babyNames[slotB][0];
    gFarmRankingData.babyNames[slotA][1] = gFarmRankingData.babyNames[slotB][1];
    gFarmRankingData.babyNames[slotA][2] = gFarmRankingData.babyNames[slotB][2];
    gFarmRankingData.babyNames[slotA][3] = gFarmRankingData.babyNames[slotB][3];
    gFarmRankingData.babyNames[slotA][4] = gFarmRankingData.babyNames[slotB][4];
    gFarmRankingData.babyNames[slotA][5] = gFarmRankingData.babyNames[slotB][5];

    gFarmRankingData.dogNames[slotA][0] = gFarmRankingData.dogNames[slotB][0];
    gFarmRankingData.dogNames[slotA][1] = gFarmRankingData.dogNames[slotB][1];
    gFarmRankingData.dogNames[slotA][2] = gFarmRankingData.dogNames[slotB][2];
    gFarmRankingData.dogNames[slotA][3] = gFarmRankingData.dogNames[slotB][3];
    gFarmRankingData.dogNames[slotA][4] = gFarmRankingData.dogNames[slotB][4];
    gFarmRankingData.dogNames[slotA][5] = gFarmRankingData.dogNames[slotB][5];

    gFarmRankingData.horseNames[slotA][0] = gFarmRankingData.horseNames[slotB][0];
    gFarmRankingData.horseNames[slotA][1] = gFarmRankingData.horseNames[slotB][1];
    gFarmRankingData.horseNames[slotA][2] = gFarmRankingData.horseNames[slotB][2];
    gFarmRankingData.horseNames[slotA][3] = gFarmRankingData.horseNames[slotB][3];
    gFarmRankingData.horseNames[slotA][4] = gFarmRankingData.horseNames[slotB][4];
    gFarmRankingData.horseNames[slotA][5] = gFarmRankingData.horseNames[slotB][5];

    gFarmRankingData.years[slotA] = gFarmRankingData.years[slotB];
    gFarmRankingData.seasons[slotA] = gFarmRankingData.seasons[slotB];

    gFarmRankingData.mariaAffection[slotA] = gFarmRankingData.mariaAffection[slotB];
    gFarmRankingData.popuriAffection[slotA] = gFarmRankingData.popuriAffection[slotB];
    gFarmRankingData.elliAffection[slotA] = gFarmRankingData.elliAffection[slotB];
    gFarmRankingData.annAffection[slotA] = gFarmRankingData.annAffection[slotB];
    gFarmRankingData.karenAffection[slotA] = gFarmRankingData.karenAffection[slotB];
    gFarmRankingData.harrisAffection[slotA] = gFarmRankingData.harrisAffection[slotB];
    gFarmRankingData.grayAffection[slotA] = gFarmRankingData.grayAffection[slotB];
    gFarmRankingData.jeffAffection[slotA] = gFarmRankingData.jeffAffection[slotB];
    gFarmRankingData.cliffAffection[slotA] = gFarmRankingData.cliffAffection[slotB];
    gFarmRankingData.kaiAffection[slotA] = gFarmRankingData.kaiAffection[slotB];
    gFarmRankingData.wifeAffection[slotA] = gFarmRankingData.wifeAffection[slotB];
    gFarmRankingData.babyAffection[slotA] = gFarmRankingData.babyAffection[slotB];
    gFarmRankingData.dogAffection[slotA] = gFarmRankingData.dogAffection[slotB];
    gFarmRankingData.horseAffection[slotA] = gFarmRankingData.horseAffection[slotB];

    gFarmRankingData.farmAnimalAffection[slotA][0] = gFarmRankingData.farmAnimalAffection[slotB][0];
    gFarmRankingData.farmAnimalAffection[slotA][1] = gFarmRankingData.farmAnimalAffection[slotB][1];
    gFarmRankingData.farmAnimalAffection[slotA][2] = gFarmRankingData.farmAnimalAffection[slotB][2];
    gFarmRankingData.farmAnimalAffection[slotA][3] = gFarmRankingData.farmAnimalAffection[slotB][3];
    gFarmRankingData.farmAnimalAffection[slotA][4] = gFarmRankingData.farmAnimalAffection[slotB][4];
    gFarmRankingData.farmAnimalAffection[slotA][5] = gFarmRankingData.farmAnimalAffection[slotB][5];
    gFarmRankingData.farmAnimalAffection[slotA][6] = gFarmRankingData.farmAnimalAffection[slotB][6];
    gFarmRankingData.farmAnimalAffection[slotA][7] = gFarmRankingData.farmAnimalAffection[slotB][7];

    gFarmRankingData.chickenCounts[slotA] = gFarmRankingData.chickenCounts[slotB];
    gFarmRankingData.wife[slotA] = gFarmRankingData.wife[slotB];

    gFarmRankingData.cropsShipped[slotA] = gFarmRankingData.cropsShipped[slotB];
    gFarmRankingData.eggsShipped[slotA] = gFarmRankingData.eggsShipped[slotB];
    gFarmRankingData.milkShipped[slotA] = gFarmRankingData.milkShipped[slotB];
    gFarmRankingData.fishCaught[slotA] = gFarmRankingData.fishCaught[slotB];
    gFarmRankingData.gold[slotA] = gFarmRankingData.gold[slotB];

    gFarmRankingData.grassTiles[slotA] = gFarmRankingData.grassTiles[slotB];
    gFarmRankingData.maxStamina[slotA] = gFarmRankingData.maxStamina[slotB];
    gFarmRankingData.photoCount[slotA] = gFarmRankingData.photoCount[slotB];
    gFarmRankingData.recipeCount[slotA] = gFarmRankingData.recipeCount[slotB];
    gFarmRankingData.houseExtensions[slotA] = gFarmRankingData.houseExtensions[slotB];
    gFarmRankingData.happiness[slotA] = gFarmRankingData.happiness[slotB];
    gFarmRankingData.scores[slotA] = gFarmRankingData.scores[slotB];

    // Step 3: Copy swap slot to slotB
    gFarmRankingData.flags[slotB] = gFarmRankingData.swapFlags;

    gFarmRankingData.farmNames[slotB][0] = gFarmRankingData.swapFarmName[0];
    gFarmRankingData.farmNames[slotB][1] = gFarmRankingData.swapFarmName[1];
    gFarmRankingData.farmNames[slotB][2] = gFarmRankingData.swapFarmName[2];
    gFarmRankingData.farmNames[slotB][3] = gFarmRankingData.swapFarmName[3];
    gFarmRankingData.farmNames[slotB][4] = gFarmRankingData.swapFarmName[4];
    gFarmRankingData.farmNames[slotB][5] = gFarmRankingData.swapFarmName[5];

    gFarmRankingData.playerNames[slotB][0] = gFarmRankingData.swapPlayerName[0];
    gFarmRankingData.playerNames[slotB][1] = gFarmRankingData.swapPlayerName[1];
    gFarmRankingData.playerNames[slotB][2] = gFarmRankingData.swapPlayerName[2];
    gFarmRankingData.playerNames[slotB][3] = gFarmRankingData.swapPlayerName[3];
    gFarmRankingData.playerNames[slotB][4] = gFarmRankingData.swapPlayerName[4];
    gFarmRankingData.playerNames[slotB][5] = gFarmRankingData.swapPlayerName[5];

    gFarmRankingData.wifeNames[slotB][0] = gFarmRankingData.swapWifeName[0];
    gFarmRankingData.wifeNames[slotB][1] = gFarmRankingData.swapWifeName[1];
    gFarmRankingData.wifeNames[slotB][2] = gFarmRankingData.swapWifeName[2];
    gFarmRankingData.wifeNames[slotB][3] = gFarmRankingData.swapWifeName[3];
    gFarmRankingData.wifeNames[slotB][4] = gFarmRankingData.swapWifeName[4];
    gFarmRankingData.wifeNames[slotB][5] = gFarmRankingData.swapWifeName[5];

    gFarmRankingData.babyNames[slotB][0] = gFarmRankingData.swapBabyName[0];
    gFarmRankingData.babyNames[slotB][1] = gFarmRankingData.swapBabyName[1];
    gFarmRankingData.babyNames[slotB][2] = gFarmRankingData.swapBabyName[2];
    gFarmRankingData.babyNames[slotB][3] = gFarmRankingData.swapBabyName[3];
    gFarmRankingData.babyNames[slotB][4] = gFarmRankingData.swapBabyName[4];
    gFarmRankingData.babyNames[slotB][5] = gFarmRankingData.swapBabyName[5];

    gFarmRankingData.dogNames[slotB][0] = gFarmRankingData.swapDogName[0];
    gFarmRankingData.dogNames[slotB][1] = gFarmRankingData.swapDogName[1];
    gFarmRankingData.dogNames[slotB][2] = gFarmRankingData.swapDogName[2];
    gFarmRankingData.dogNames[slotB][3] = gFarmRankingData.swapDogName[3];
    gFarmRankingData.dogNames[slotB][4] = gFarmRankingData.swapDogName[4];
    gFarmRankingData.dogNames[slotB][5] = gFarmRankingData.swapDogName[5];

    gFarmRankingData.horseNames[slotB][0] = gFarmRankingData.swapHorseName[0];
    gFarmRankingData.horseNames[slotB][1] = gFarmRankingData.swapHorseName[1];
    gFarmRankingData.horseNames[slotB][2] = gFarmRankingData.swapHorseName[2];
    gFarmRankingData.horseNames[slotB][3] = gFarmRankingData.swapHorseName[3];
    gFarmRankingData.horseNames[slotB][4] = gFarmRankingData.swapHorseName[4];
    gFarmRankingData.horseNames[slotB][5] = gFarmRankingData.swapHorseName[5];

    gFarmRankingData.years[slotB] = gFarmRankingData.years[RANKING_SWAP_SLOT];
    gFarmRankingData.seasons[slotB] = gFarmRankingData.seasons[RANKING_SWAP_SLOT];

    gFarmRankingData.mariaAffection[slotB] = gFarmRankingData.mariaAffection[RANKING_SWAP_SLOT];
    gFarmRankingData.popuriAffection[slotB] = gFarmRankingData.popuriAffection[RANKING_SWAP_SLOT];
    gFarmRankingData.elliAffection[slotB] = gFarmRankingData.elliAffection[RANKING_SWAP_SLOT];
    gFarmRankingData.annAffection[slotB] = gFarmRankingData.annAffection[RANKING_SWAP_SLOT];
    gFarmRankingData.karenAffection[slotB] = gFarmRankingData.karenAffection[RANKING_SWAP_SLOT];
    gFarmRankingData.harrisAffection[slotB] = gFarmRankingData.harrisAffection[RANKING_SWAP_SLOT];
    gFarmRankingData.grayAffection[slotB] = gFarmRankingData.grayAffection[RANKING_SWAP_SLOT];
    gFarmRankingData.jeffAffection[slotB] = gFarmRankingData.jeffAffection[RANKING_SWAP_SLOT];
    gFarmRankingData.cliffAffection[slotB] = gFarmRankingData.cliffAffection[RANKING_SWAP_SLOT];
    gFarmRankingData.kaiAffection[slotB] = gFarmRankingData.kaiAffection[RANKING_SWAP_SLOT];
    gFarmRankingData.wifeAffection[slotB] = gFarmRankingData.wifeAffection[RANKING_SWAP_SLOT];
    gFarmRankingData.babyAffection[slotB] = gFarmRankingData.babyAffection[RANKING_SWAP_SLOT];
    gFarmRankingData.dogAffection[slotB] = gFarmRankingData.dogAffection[RANKING_SWAP_SLOT];
    gFarmRankingData.horseAffection[slotB] = gFarmRankingData.horseAffection[RANKING_SWAP_SLOT];

    gFarmRankingData.farmAnimalAffection[slotB][0] = gFarmRankingData.swapFarmAnimalAffection1[0];
    gFarmRankingData.farmAnimalAffection[slotB][1] = gFarmRankingData.swapFarmAnimalAffection1[1];
    gFarmRankingData.farmAnimalAffection[slotB][2] = gFarmRankingData.swapFarmAnimalAffection1[2];
    gFarmRankingData.farmAnimalAffection[slotB][3] = gFarmRankingData.swapFarmAnimalAffection1[3];
    gFarmRankingData.farmAnimalAffection[slotB][4] = gFarmRankingData.swapFarmAnimalAffection2[0];
    gFarmRankingData.farmAnimalAffection[slotB][5] = gFarmRankingData.swapFarmAnimalAffection2[1];
    gFarmRankingData.farmAnimalAffection[slotB][6] = gFarmRankingData.swapFarmAnimalAffection2[2];
    gFarmRankingData.farmAnimalAffection[slotB][7] = gFarmRankingData.swapFarmAnimalAffection2[3];

    gFarmRankingData.chickenCounts[slotB] = gFarmRankingData.chickenCounts[RANKING_SWAP_SLOT];
    gFarmRankingData.wife[slotB] = gFarmRankingData.wife[RANKING_SWAP_SLOT];

    gFarmRankingData.cropsShipped[slotB] = gFarmRankingData.cropsShipped[RANKING_SWAP_SLOT];
    gFarmRankingData.eggsShipped[slotB] = gFarmRankingData.eggsShipped[RANKING_SWAP_SLOT];
    gFarmRankingData.milkShipped[slotB] = gFarmRankingData.milkShipped[RANKING_SWAP_SLOT];
    gFarmRankingData.fishCaught[slotB] = gFarmRankingData.fishCaught[RANKING_SWAP_SLOT];
    gFarmRankingData.gold[slotB] = gFarmRankingData.gold[RANKING_SWAP_SLOT];

    gFarmRankingData.grassTiles[slotB] = gFarmRankingData.grassTiles[RANKING_SWAP_SLOT];
    gFarmRankingData.maxStamina[slotB] = gFarmRankingData.maxStamina[RANKING_SWAP_SLOT];
    gFarmRankingData.photoCount[slotB] = gFarmRankingData.photoCount[RANKING_SWAP_SLOT];
    gFarmRankingData.recipeCount[slotB] = gFarmRankingData.recipeCount[RANKING_SWAP_SLOT];
    gFarmRankingData.houseExtensions[slotB] = gFarmRankingData.houseExtensions[RANKING_SWAP_SLOT];
    gFarmRankingData.happiness[slotB] = gFarmRankingData.happiness[RANKING_SWAP_SLOT];
    gFarmRankingData.scores[slotB] = gFarmRankingData.scores[RANKING_SWAP_SLOT];
   
}

//INCLUDE_ASM("asm/nonmatchings/game/gameFile", func_800ED160);

void func_800ED160(u8 arg0, u8 arg1) {

    gFarmRankingData.flags[arg0] = gFarmRankingData.flags[arg1];

    gFarmRankingData.farmNames[arg0][0] = gFarmRankingData.farmNames[arg1][0];
    gFarmRankingData.farmNames[arg0][1] = gFarmRankingData.farmNames[arg1][1];
    gFarmRankingData.farmNames[arg0][2] = gFarmRankingData.farmNames[arg1][2];
    gFarmRankingData.farmNames[arg0][3] = gFarmRankingData.farmNames[arg1][3];
    gFarmRankingData.farmNames[arg0][4] = gFarmRankingData.farmNames[arg1][4];
    gFarmRankingData.farmNames[arg0][5] = gFarmRankingData.farmNames[arg1][5];

    gFarmRankingData.playerNames[arg0][0] = gFarmRankingData.playerNames[arg1][0];
    gFarmRankingData.playerNames[arg0][1] = gFarmRankingData.playerNames[arg1][1];
    gFarmRankingData.playerNames[arg0][2] = gFarmRankingData.playerNames[arg1][2];
    gFarmRankingData.playerNames[arg0][3] = gFarmRankingData.playerNames[arg1][3];
    gFarmRankingData.playerNames[arg0][4] = gFarmRankingData.playerNames[arg1][4];
    gFarmRankingData.playerNames[arg0][5] = gFarmRankingData.playerNames[arg1][5];

    gFarmRankingData.wifeNames[arg0][0] = gFarmRankingData.wifeNames[arg1][0];
    gFarmRankingData.wifeNames[arg0][1] = gFarmRankingData.wifeNames[arg1][1];
    gFarmRankingData.wifeNames[arg0][2] = gFarmRankingData.wifeNames[arg1][2];
    gFarmRankingData.wifeNames[arg0][3] = gFarmRankingData.wifeNames[arg1][3];
    gFarmRankingData.wifeNames[arg0][4] = gFarmRankingData.wifeNames[arg1][4];
    gFarmRankingData.wifeNames[arg0][5] = gFarmRankingData.wifeNames[arg1][5];

    gFarmRankingData.babyNames[arg0][0] = gFarmRankingData.babyNames[arg1][0];
    gFarmRankingData.babyNames[arg0][1] = gFarmRankingData.babyNames[arg1][1];
    gFarmRankingData.babyNames[arg0][2] = gFarmRankingData.babyNames[arg1][2];
    gFarmRankingData.babyNames[arg0][3] = gFarmRankingData.babyNames[arg1][3];
    gFarmRankingData.babyNames[arg0][4] = gFarmRankingData.babyNames[arg1][4];
    gFarmRankingData.babyNames[arg0][5] = gFarmRankingData.babyNames[arg1][5];

    gFarmRankingData.dogNames[arg0][0] = gFarmRankingData.dogNames[arg1][0];
    gFarmRankingData.dogNames[arg0][1] = gFarmRankingData.dogNames[arg1][1];
    gFarmRankingData.dogNames[arg0][2] = gFarmRankingData.dogNames[arg1][2];
    gFarmRankingData.dogNames[arg0][3] = gFarmRankingData.dogNames[arg1][3];
    gFarmRankingData.dogNames[arg0][4] = gFarmRankingData.dogNames[arg1][4];
    gFarmRankingData.dogNames[arg0][5] = gFarmRankingData.dogNames[arg1][5];

    gFarmRankingData.horseNames[arg0][0] = gFarmRankingData.horseNames[arg1][0];
    gFarmRankingData.horseNames[arg0][1] = gFarmRankingData.horseNames[arg1][1];
    gFarmRankingData.horseNames[arg0][2] = gFarmRankingData.horseNames[arg1][2];
    gFarmRankingData.horseNames[arg0][3] = gFarmRankingData.horseNames[arg1][3];
    gFarmRankingData.horseNames[arg0][4] = gFarmRankingData.horseNames[arg1][4];
    gFarmRankingData.horseNames[arg0][5] = gFarmRankingData.horseNames[arg1][5];

    gFarmRankingData.years[arg0] = gFarmRankingData.years[arg1];
    gFarmRankingData.seasons[arg0] = gFarmRankingData.seasons[arg1];
    gFarmRankingData.mariaAffection[arg0] = gFarmRankingData.mariaAffection[arg1];
    gFarmRankingData.popuriAffection[arg0] = gFarmRankingData.popuriAffection[arg1];
    gFarmRankingData.elliAffection[arg0] = gFarmRankingData.elliAffection[arg1];
    gFarmRankingData.annAffection[arg0] = gFarmRankingData.annAffection[arg1];
    gFarmRankingData.karenAffection[arg0] = gFarmRankingData.karenAffection[arg1];
    gFarmRankingData.harrisAffection[arg0] = gFarmRankingData.harrisAffection[arg1];
    gFarmRankingData.grayAffection[arg0] = gFarmRankingData.grayAffection[arg1];
    gFarmRankingData.jeffAffection[arg0] = gFarmRankingData.jeffAffection[arg1];
    gFarmRankingData.cliffAffection[arg0] = gFarmRankingData.cliffAffection[arg1];
    gFarmRankingData.kaiAffection[arg0] = gFarmRankingData.kaiAffection[arg1];
    gFarmRankingData.wifeAffection[arg0] = gFarmRankingData.wifeAffection[arg1];
    gFarmRankingData.babyAffection[arg0] = gFarmRankingData.babyAffection[arg1];
    gFarmRankingData.dogAffection[arg0] = gFarmRankingData.dogAffection[arg1];

    gFarmRankingData.horseAffection[arg0] = gFarmRankingData.horseAffection[arg1];

    gFarmRankingData.farmAnimalAffection[arg0][0] = gFarmRankingData.farmAnimalAffection[arg1][0];
    gFarmRankingData.farmAnimalAffection[arg0][1] = gFarmRankingData.farmAnimalAffection[arg1][1];
    gFarmRankingData.farmAnimalAffection[arg0][2] = gFarmRankingData.farmAnimalAffection[arg1][2];
    gFarmRankingData.farmAnimalAffection[arg0][3] = gFarmRankingData.farmAnimalAffection[arg1][3];
    gFarmRankingData.farmAnimalAffection[arg0][4] = gFarmRankingData.farmAnimalAffection[arg1][4];
    gFarmRankingData.farmAnimalAffection[arg0][5] = gFarmRankingData.farmAnimalAffection[arg1][5];
    gFarmRankingData.farmAnimalAffection[arg0][6] = gFarmRankingData.farmAnimalAffection[arg1][6];
    gFarmRankingData.farmAnimalAffection[arg0][7] = gFarmRankingData.farmAnimalAffection[arg1][7];

    gFarmRankingData.chickenCounts[arg0] = gFarmRankingData.chickenCounts[arg1];

    gFarmRankingData.wife[arg0] = gFarmRankingData.wife[arg1];
    
    gFarmRankingData.cropsShipped[arg0] = gFarmRankingData.cropsShipped[arg1];
    gFarmRankingData.eggsShipped[arg0] = gFarmRankingData.eggsShipped[arg1];
    gFarmRankingData.milkShipped[arg0] = gFarmRankingData.milkShipped[arg1];
    gFarmRankingData.fishCaught[arg0] = gFarmRankingData.fishCaught[arg1];

    gFarmRankingData.gold[arg0] = gFarmRankingData.gold[arg1];
    
    gFarmRankingData.grassTiles[arg0] = gFarmRankingData.grassTiles[arg1];
    
    gFarmRankingData.maxStamina[arg0] = gFarmRankingData.maxStamina[arg1];
    gFarmRankingData.photoCount[arg0] = gFarmRankingData.photoCount[arg1];
    gFarmRankingData.recipeCount[arg0] = gFarmRankingData.recipeCount[arg1];
    gFarmRankingData.houseExtensions[arg0] = gFarmRankingData.houseExtensions[arg1];
    gFarmRankingData.happiness[arg0] = gFarmRankingData.happiness[arg1];
    
    
    gFarmRankingData.scores[arg0] = gFarmRankingData.scores[arg1];
    
    func_800E6C08(arg1);
        
}