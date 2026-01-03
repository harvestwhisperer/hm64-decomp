#include "common.h"

#include "game/namingScreen.h"

#include "ld_symbols.h"

#include "system/audio.h"
#include "system/controller.h"
#include "system/dialogue.h"
#include "system/message.h"
#include "system/globalSprites.h"

#include "game/animals.h"
#include "game/game.h"
#include "game/gameAudio.h"
#include "game/level.h"
#include "game/npc.h"
#include "game/player.h"
#include "game/time.h"
#include "game/transition.h"

#include "mainLoop.h"

#include "buffers/buffers.h"

#include "assetIndices/sequences.h"
#include "assetIndices/sfxs.h"
#include "assetIndices/sprites.h"


// FIXME:
// instead of including the header, defining here because loadCutscene(u32) doesn't match in namingScreenCallback
extern void loadCutscene(void);
extern u16 gCutsceneIndex;


// bss
NamingScreenContext namingScreenContext;

// data
// japanese strings
u8 D_8011C680[13][6] = {
    0x6E, 0x77, 0xE1, 0xFF, 0xFF, 0xFF,
    0x96, 0x94, 0x77, 0xFF, 0xFF, 0xFF,
    0x53, 0x77, 0x9D, 0xFF, 0xFF, 0xFF,
    0x76, 0x7D, 0xFF, 0xFF, 0xFF, 0xFF,
    0x55, 0x79, 0x7D, 0xFF, 0xFF, 0xFF,
    0x69, 0x77, 0x5C, 0xFF, 0xFF, 0xFF,
    0x80, 0x79, 0x51, 0xFF, 0xFF, 0xFF,
    0x84, 0x9F, 0x6B, 0xFF, 0xFF, 0xFF,
    0x57, 0x77, 0x6B, 0xFF, 0xFF, 0xFF,
    0x55, 0x51, 0xFF, 0xFF, 0xFF, 0xFF,
    0x58, 0x7D, 0x5F, 0xFF, 0xFF, 0xFF,
    0x74, 0x52, 0xFF, 0xFF, 0xFF, 0xFF,
    0x71, 0x51, 0xFF, 0xFF, 0xFF, 0xFF
};

// columns x rows
u8 gridToCharacterMapJP1[15][6] = {
    { 0, 0x5, 0xA, 0xF, 0x14, 0x19 },
    { 0x1, 0x6, 0xB, 0x10, 0x15, 0x1A },
    { 0x2, 0x7, 0xC, 0x11, 0x16, 0x1B },
    { 0x3, 0x8, 0xD, 0x12, 0x17, 0x1C },
    { 0x4, 0x9, 0xE, 0x13, 0x18, 0x1D },
    { 0x1E, 0x23, 0x26, 0x2B, 0x4B, 0x47 },
    { 0x1F, 0xEE, 0x27, 0x2C, 0x4C, 0x48 },
    { 0x20, 0x24, 0x28, 0x2D, 0x4D, 0x49 },
    { 0x21, 0xEE, 0x29, 0xEE, 0x4E, 0x4A },
    { 0x22, 0x25, 0x2A, 0xEE, 0x4F, 0xE1 },
    { 0x2E, 0x33, 0x38, 0x3D, 0x42, 0xEE },
    { 0x2F, 0x34, 0x39, 0x3E, 0x43, 0xEE },
    { 0x30, 0x35, 0x3A, 0x3F, 0x44, 0xEE },
    { 0x31, 0x36, 0x3B, 0x40, 0x45, 0xEE },
    { 0x32, 0x37, 0x3C, 0x41, 0x46, 0xEE },
};

u8 gridToCharacterMapJP2[15][6] = {
    { 0x50, 0x55, 0x5A, 0x5F, 0x64, 0x69 },
    { 0x51, 0x56, 0x5B, 0x60, 0x65, 0x6A },
    { 0x52, 0x57, 0x5C, 0x61, 0x66, 0x6B },
    { 0x53, 0x58, 0x5D, 0x62, 0x67, 0x6C },
    { 0x54, 0x59, 0x5E, 0x63, 0x68, 0x6D },
    { 0x6E, 0x73, 0x76, 0x7B, 0x9C, 0x98 },
    { 0x6F, 0xEE, 0x77, 0x7C, 0x9D, 0x99 },
    { 0x70, 0x74, 0x78, 0x7D, 0x9E, 0x9A },
    { 0x71, 0xEE, 0x79, 0xEE, 0x9F, 0x9B },
    { 0x72, 0x75, 0x7A, 0x97, 0xA0, 0xE1 },
    { 0x7E, 0x83, 0x88, 0x8D, 0x92, 0xEE },
    { 0x7F, 0x84, 0x89, 0x8E, 0x93, 0xEE },
    { 0x80, 0x85, 0x8A, 0x8F, 0x94, 0xEE },
    { 0x81, 0x86, 0x8B, 0x90, 0x95, 0xEE },
    { 0x82, 0x87, 0x8C, 0x91, 0x96, 0xEE },
};

u8 gridToCharacterMapEN[15][6] = {
    { char_A, char_F, char_K,                char_P,      char_U,           char_Z     },
    { char_B, char_G, char_L,                char_Q,      char_V,           char_SPACE },
    { char_C, char_H, char_M,                char_R,      char_W,           char_SPACE },
    { char_D, char_I, char_N,                char_S,      char_X,           char_SPACE },
    { char_E, char_J, char_O,                char_T,      char_Y,           char_SPACE },
    { char_a, char_f, char_k,                char_p,      char_u,           char_z     },
    { char_b, char_g, char_l,                char_q,      char_v,           char_SPACE },
    { char_c, char_h, char_m,                char_r,      char_w,           char_SPACE },
    { char_d, char_i, char_n,                char_s,      char_x,           char_SPACE },
    { char_e, char_j, char_o,                char_t,      char_y,           char_SPACE },
    { char_1, char_6, char_QUESTION_MARK,    char_COMMA,  char_AMPERSAND,   char_SPACE },
    { char_2, char_7, char_EXCLAMATION_MARK, char_DOT,    char_HEART_1,     char_SPACE },
    { char_3, char_8, char_DASH,             char_SLASH,  char_HEART_2,     char_SPACE },
    { char_4, char_9, char_TILDE,            char_STAR_1, char_DROP,        char_SPACE },
    { char_5, char_0, char_PERIOD,           char_STAR_2, char_PAW_PRINT,   char_SPACE },
};

// forward declarations
void loadNameSelectionSprites(void);
bool selectCharacterOrConfirm(void);
void moveCursorLeft(void);
void moveCursorRight(void);
void moveCursorUp(void);
void moveCursorDown(void);
void deactivateNamingScreenSprites(void);
void updateBottomRowUI(void);
void snapCursorToOKButton(void);
void loadSeasonSelectionSprites(void);
void handleSeasonSelectionInput(void);


static inline int getSpriteIndexFromFlags(u16 flags) {
    int v = (flags & NAMING_SCREEN_NAME_POSITION_MASK) >> NAMING_SCREEN_NAME_POSITION_SHIFT;
    v -= 1;
    return v;
}

static inline void setSpriteIndexOnFlags(s32 index) {
    namingScreenContext.flags &= ~NAMING_SCREEN_NAME_POSITION_MASK;
    namingScreenContext.flags |= (index << NAMING_SCREEN_NAME_POSITION_SHIFT);
}

//INCLUDE_ASM("asm/nonmatchings/game/namingScreen", initializeNamingScreen);

inline void initializeNamingScreen(u8* arg0, u8 arg1) {

    s32 i = 0;
    
    namingScreenContext.screenType = arg1;
    namingScreenContext.selectedSeason = 0;

    namingScreenContext.name = arg0;

    for (i = 0; i < NAME_LENGTH; i++) {
        namingScreenContext.name[i] = 0xFF;
    } 

    loadNameSelectionSprites();

    setMessageBoxViewSpacePosition(0, 0, -64.0f, 352.0f);
    setMessageBoxInterpolationWithFlags(0, -4, 0);
    setMessageBoxSpriteIndices(0, 1, 0, 0);
    
    namingScreenContext.dialogueIndex = arg1;
    
    stopCurrentAudioSequence(NAMING_SCREEN_THEME);
    setCurrentAudioSequence(NAMING_SCREEN_THEME);
    setAudioSequenceVolume(NAMING_SCREEN_THEME, SEQUENCE_VOLUME);
    
    setMainLoopCallbackFunctionIndex(NAMING_SCREEN_LOAD);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/namingScreen", namingScreenCallback);

// main loop callback
void namingScreenCallback(void) {

    s32 i;
    s32 temp = (namingScreenContext.flags & NAMING_SCREEN_SCREEN_STATE_MASK) >> NAMING_SCREEN_SCREEN_STATE_SHIFT;
    s32 temp2;

    if (globalSprites[0x90].stateFlags & 0x400) {
        
        if (namingScreenContext.flags & NAMING_SCREEN_GOTO_SEASON_SELECT) {

            deactivateNamingScreenSprites();

            // message box
            deactivateMessageBox(3);

            loadSeasonSelectionSprites();

            namingScreenContext.flags &= ~NAMING_SCREEN_GOTO_SEASON_SELECT;

            return;

        }

        if (namingScreenContext.flags & NAMING_SCREEN_GOTO_SEASON_CONFIRM) {

            temp2 = namingScreenContext.selectedSeason;

            deactivateNamingScreenSprites();

            dmaSprite(0x80, &_namingScreen1TextureSegmentRomStart, &_namingScreen1TextureSegmentRomEnd, &_namingScreen1AssetsIndexSegmentRomStart, &_namingScreen1AssetsIndexSegmentRomEnd, NULL, NULL, (u8*)NAMING_SCREEN_CONFIRMATION_SCREEN_TEXTURE_BUFFER, NULL, (u16*)NAMING_SCREEN_CONFIRMATION_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)NAMING_SCREEN_CONFIRMATION_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)NAMING_SCREEN_CONFIRMATION_SCREEN_TEXTURE_TO_PALETTE_BUFFER, NULL, 0, FALSE);
            dmaSprite(0x84, &_namingScreen1TextureSegmentRomStart, &_namingScreen1TextureSegmentRomEnd, &_namingScreen1AssetsIndexSegmentRomStart, &_namingScreen1AssetsIndexSegmentRomEnd, NULL, NULL, (u8*)NAMING_SCREEN_CONFIRMATION_SCREEN_TEXTURE_BUFFER, NULL, (u16*)NAMING_SCREEN_CONFIRMATION_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)NAMING_SCREEN_CONFIRMATION_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)NAMING_SCREEN_CONFIRMATION_SCREEN_TEXTURE_TO_PALETTE_BUFFER, NULL, 0, FALSE);

            dmaSprite(LANDSCAPE_BACKGROUND, &_namingScreenBackgroundTextureSegmentRomStart, &_namingScreenBackgroundTextureSegmentRomEnd, &_namingScreenBackgroundAssetsIndexSegmentRomStart, &_namingScreenBackgroundAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)NAMING_SCREEN_BACKGROUND_TEXTURE_BUFFER, NULL, (u16*)NAMING_SCREEN_BACKGROUND_PALETTE_BUFFER, (AnimationFrameMetadata*)NAMING_SCREEN_BACKGROUND_ANIMATION_FRAME_METADATA_BUFFER, (u32*)NAMING_SCREEN_BACKGROUND_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
            setBilinearFiltering(LANDSCAPE_BACKGROUND, TRUE);
            setSpriteScale(LANDSCAPE_BACKGROUND, 2.0f, 2.0f, 1.0f);

            startSpriteAnimation(0x80, 1, 0);
            startSpriteAnimation(0x84, 2, 0);
            startSpriteAnimation(LANDSCAPE_BACKGROUND, 0, 0);

            setSpriteColor(0x80, 0, 0, 0, 0);
            setSpriteColor(0x84, 0, 0, 0, 0);
            setSpriteColor(LANDSCAPE_BACKGROUND, 0, 0, 0, 0);

            updateSpriteRGBA(0x80, 0xFF, 0xFF, 0xFF, 0xFF, 8);
            updateSpriteRGBA(0x84, 0xFF, 0xFF, 0xFF, 0xFF, 8);
            updateSpriteRGBA(LANDSCAPE_BACKGROUND, 0xFF, 0xFF, 0xFF, 0xFF, 8);

            namingScreenContext.dialogueIndex = 10;
            namingScreenContext.flags &= ~NAMING_SCREEN_SCREEN_STATE_MASK;
            namingScreenContext.flags |= (NAMING_SCREEN_STATE_SEASON_CONFIRM << NAMING_SCREEN_SCREEN_STATE_SHIFT);

            initializeEmptyMessageBox(3, (u8*)MESSAGE_BOX_3_TEXT_BUFFER);
            setMessageBoxViewSpacePosition(3, 4.0f, 56.0f, 30.0f);
            setMessageBoxLineAndRowSizes(3, 6, 1);
            setMessageBoxSpacing(3, 0, 2);
            setMessageBoxFont(3, 0xE, 0xE, (u8*)FONT_TEXTURE_BUFFER, FONT_PALETTE_1_BUFFER);
            setMessageBoxInterpolationWithFlags(3, 1, 1);
            setMessageBoxSpriteIndices(3, 0xFF, 0xFF, 0xFF);
            initializeMessageBox(3, 0xA, 0xD, MESSAGE_BOX_MODE_NO_INPUT);
            setMessageBoxRGBA(3, 0, 0, 0, 0);
            setMessageBoxRGBAWithTransition(3, 0xFF, 0xFF, 0xFF, 0xFF, 8);
            
            initializeEmptyMessageBox(4, (u8*)MESSAGE_BOX_4_TEXT_BUFFER);
            setMessageBoxViewSpacePosition(4, 4.0f, -10.0f, 30.0f);
            setMessageBoxLineAndRowSizes(4, 6, 1);
            setMessageBoxSpacing(4, 0, 2);
            setMessageBoxFont(4, 0xE, 0xE, (u8*)FONT_TEXTURE_BUFFER, FONT_PALETTE_1_BUFFER);
            setMessageBoxInterpolationWithFlags(4, 1, 1);
            setMessageBoxSpriteIndices(4, 0xFF, 0xFF, 0xFF);
            initializeMessageBox(4, 0xA, temp2 + 0xE, MESSAGE_BOX_MODE_NO_INPUT);
            setMessageBoxRGBA(4, 0, 0, 0, 0);
            setMessageBoxRGBAWithTransition(4, 0xFF, 0xFF, 0xFF, 0xFF, 8);
            
            namingScreenContext.flags &= ~NAMING_SCREEN_GOTO_SEASON_CONFIRM;

            return;

        }

        if (namingScreenContext.flags & NAMING_SCREEN_RETURN_TO_NAMING) {

            deactivateNamingScreenSprites();
            deactivateMessageBox(3);
            deactivateMessageBox(4);
            loadNameSelectionSprites();

            namingScreenContext.dialogueIndex = 12;
            namingScreenContext.flags &= ~NAMING_SCREEN_RETURN_TO_NAMING;

            return;

        }

         if (namingScreenContext.flags & NAMING_SCREEN_CONFIRM_AND_EXIT) {

            setGameVariableString(0, gPlayer.name, 6);
            
            deactivateNamingScreenSprites();
            deactivateMessageBox(3);
            deactivateMessageBox(4);
            setMessageBoxSpriteIndices(0, 0, 0, 0);
            setMessageBoxViewSpacePosition(0, 24.0f, -64.0f, 352.0f);

             switch (namingScreenContext.screenType) {
                 
                 case 0:
                    initializeNamingScreen(gFarmName, 1);
                    return;
                 
                 case 1:
                    initializeNamingScreen(dogInfo.name, 2);
                    return;
                 
                 case 2:
                    startGame();
                    return;
                 
                 case 3:

                    setLevelAudio(gBaseMapIndex, gSeason, gHour);
                     
                    gCutsceneIndex = 0x28B;
                     
                    loadCutscene();
                     
                    exitOverlayScreen();
                    setLevelLighting(8, 1);
                     
                    return;
                 
                 case 4:

                    setLevelAudio(gBaseMapIndex, gSeason, gHour);
                     
                    switch (gWife) {                   
                        case MARIA:                            
                            gCutsceneIndex = 5;
                            toggleSpecialDialogueBit(0x37);
                            break;
                        case POPURI:                            
                            gCutsceneIndex = 0xC;
                            toggleSpecialDialogueBit(0x38);
                            break;
                        case ELLI:                            
                            gCutsceneIndex = 0x13;
                            toggleSpecialDialogueBit(0x39);
                            break;
                        case ANN:                            
                            gCutsceneIndex = 0x1A;
                            toggleSpecialDialogueBit(0x3A);
                            break;
                        case KAREN:                            
                            gCutsceneIndex = 0x21;
                            toggleSpecialDialogueBit(0x3B);
                            break;
                        }
                     
                        loadCutscene();

                        exitOverlayScreen();
                        setLevelLighting(8, 1);
                     
                    return;
                 
                 case 5:
                 case 6:
                 case 7:
                    setLevelAudio(gBaseMapIndex, gSeason, gHour);
                    exitOverlayScreen();
                    setLevelLighting(8, 1);
                    return;
                 
             }

        } else {

            if (namingScreenContext.flags & NAMING_SCREEN_AUDIO_FADE_TRANSITION) {
                namingScreenContext.flags &= ~NAMING_SCREEN_AUDIO_FADE_TRANSITION;
                namingScreenContext.flags |= NAMING_SCREEN_CONFIRM_AND_EXIT;
                return;
            }
    
            if (namingScreenContext.dialogueIndex != 0xFFFF) {
                initializeDialogueSession(0, 0x3A, namingScreenContext.dialogueIndex, 0);
                namingScreenContext.dialogueIndex = 0xFFFF;
                return;
            }
                 
            if (dialogues[0].sessionManager.flags & 4) {
                
                    switch (temp) {   
    
                        case 0:
                            handleNamingGridInput();
                            return;

                        case 1:
                            handleSeasonSelectionInput();
                            return;
    
                        case NAMING_SCREEN_STATE_SEASON_CONFIRM:

                            if (dialogues[0].sessionManager.selectedMenuRow == 0) {

                                namingScreenContext.flags |= NAMING_SCREEN_CONFIRM_AND_EXIT;
                                
                                setMessageBoxRGBAWithTransition(3, 0, 0, 0, 0, 8);
                                setMessageBoxRGBAWithTransition(4, 0, 0, 0, 0, 8);
                                updateSpriteRGBA(0x80, 0, 0, 0, 0, 8);
                                updateSpriteRGBA(0x84, 0, 0, 0, 0, 8);
                                updateSpriteRGBA(LANDSCAPE_BACKGROUND, 0, 0, 0, 0, 8);
                                
                                gPlayerBirthdaySeason = namingScreenContext.selectedSeason + 1;

                                return;
                                
                            }
    
                            namingScreenContext.flags |= NAMING_SCREEN_RETURN_TO_NAMING;

                            setMessageBoxRGBAWithTransition(3, 0, 0, 0, 0, 8);
                            setMessageBoxRGBAWithTransition(4, 0, 0, 0, 0, 8);
                            updateSpriteRGBA(0x80, 0, 0, 0, 0, 8);
                            updateSpriteRGBA(0x84, 0, 0, 0, 0, 8);
                            updateSpriteRGBA(LANDSCAPE_BACKGROUND, 0, 0, 0, 0, 8);

                            return;

                        case NAMING_SCREEN_STATE_ANIMAL_CONFIRM:

                            if (dialogues[0].sessionManager.selectedMenuRow == 0) {

                                namingScreenContext.flags |= NAMING_SCREEN_CONFIRM_AND_EXIT;

                                setMessageBoxRGBAWithTransition(3, 0, 0, 0, 0, 8);
                                
                                updateSpriteRGBA(0x80, 0, 0, 0, 0, 8);
                                updateSpriteRGBA(0x81, 0, 0, 0, 0, 8);
                                updateSpriteRGBA(0x82, 0, 0, 0, 0, 8);
                                updateSpriteRGBA(0x8F, 0, 0, 0, 0, 8);
                                updateSpriteRGBA(LANDSCAPE_BACKGROUND, 0, 0, 0, 0, 8);
                                updateSpriteRGBA(0x83, 0, 0, 0, 0, 8);
                                updateSpriteRGBA(0x84, 0, 0, 0, 0, 8);
                                updateSpriteRGBA(0x85, 0, 0, 0, 0, 8);
                                updateSpriteRGBA(0x86, 0, 0, 0, 0, 8);
                                updateSpriteRGBA(0x87, 0, 0, 0, 0, 8);
                                updateSpriteRGBA(0x88, 0, 0, 0, 0, 8);
                                updateSpriteRGBA(0x89, 0, 0, 0, 0, 8);
                                updateSpriteRGBA(0x8A, 0, 0, 0, 0, 8);
                                updateSpriteRGBA(0x8B, 0, 0, 0, 0, 8);
                                updateSpriteRGBA(0x8C, 0, 0, 0, 0, 8);
                                updateSpriteRGBA(0x8D, 0, 0, 0, 0, 8);
                                updateSpriteRGBA(0x8E, 0, 0, 0, 0, 8);
                                updateSpriteRGBA(0x91, 0, 0, 0, 0, 8);
    
                                if (namingScreenContext.screenType != 1) {

                                    stopAudioSequenceWithDefaultFadeOut(NAMING_SCREEN_THEME);

                                    namingScreenContext.flags &= ~NAMING_SCREEN_CONFIRM_AND_EXIT;
                                    namingScreenContext.flags |= NAMING_SCREEN_AUDIO_FADE_TRANSITION;

                                }

                            } else {
                                namingScreenContext.flags &= ~NAMING_SCREEN_SCREEN_STATE_MASK;
                                setSpritePaletteIndex(0x91, 4);
                            }
                            
                        return;
    
                    default:
                        return;

                }

            }
        }


    }

}

//INCLUDE_ASM("asm/nonmatchings/game/namingScreen", loadNameSelectionSprites);

void loadNameSelectionSprites(void) {

    s32 i = 0;

    dmaSprite(0x80, &_dialogueIconsTextureSegmentRomStart, &_dialogueIconsTextureSegmentRomEnd, &_dialogueIconsAssetsIndexSegmentRomStart, &_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)DIALOGUE_ICON_TEXTURE_BUFFER, NULL, (u8*)DIALOGUE_ICON_PALETTE_BUFFER, (AnimationFrameMetadata*)DIALOGUE_ICON_ANIMATION_FRAME_METADATA_BUFFER, (u32*)DIALOGUE_ICON_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    
    dmaSprite(0x81, &_namingScreen2TextureSegmentRomStart, &_namingScreen2TextureSegmentRomEnd, &_namingScreen2AssetsIndexSegmentRomStart, &_namingScreen2AssetsIndexSegmentRomEnd, NULL, NULL, (u8*)NAMING_SCREEN_TEXTURE_BUFFER, NULL, (u16*)NAMING_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)NAMING_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)NAMING_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    dmaSprite(0x82, &_namingScreen2TextureSegmentRomStart, &_namingScreen2TextureSegmentRomEnd, &_namingScreen2AssetsIndexSegmentRomStart, &_namingScreen2AssetsIndexSegmentRomEnd, NULL, NULL, (u8*)NAMING_SCREEN_TEXTURE_BUFFER, NULL, (u16*)NAMING_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)NAMING_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)NAMING_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    dmaSprite(0x83, &_namingScreen2TextureSegmentRomStart, &_namingScreen2TextureSegmentRomEnd, &_namingScreen2AssetsIndexSegmentRomStart, &_namingScreen2AssetsIndexSegmentRomEnd, NULL, NULL, (u8*)NAMING_SCREEN_TEXTURE_BUFFER, NULL, (u16*)NAMING_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)NAMING_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)NAMING_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    dmaSprite(0x84, &_namingScreen2TextureSegmentRomStart, &_namingScreen2TextureSegmentRomEnd, &_namingScreen2AssetsIndexSegmentRomStart, &_namingScreen2AssetsIndexSegmentRomEnd, NULL, NULL, (u8*)NAMING_SCREEN_TEXTURE_BUFFER, NULL, (u16*)NAMING_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)NAMING_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)NAMING_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    dmaSprite(0x85, &_namingScreen2TextureSegmentRomStart, &_namingScreen2TextureSegmentRomEnd, &_namingScreen2AssetsIndexSegmentRomStart, &_namingScreen2AssetsIndexSegmentRomEnd, NULL, NULL, (u8*)NAMING_SCREEN_TEXTURE_BUFFER, NULL, (u16*)NAMING_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)NAMING_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)NAMING_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    dmaSprite(0x86, &_namingScreen2TextureSegmentRomStart, &_namingScreen2TextureSegmentRomEnd, &_namingScreen2AssetsIndexSegmentRomStart, &_namingScreen2AssetsIndexSegmentRomEnd, NULL, NULL, (u8*)NAMING_SCREEN_TEXTURE_BUFFER, NULL, (u16*)NAMING_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)NAMING_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)NAMING_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    dmaSprite(0x87, &_namingScreen2TextureSegmentRomStart, &_namingScreen2TextureSegmentRomEnd, &_namingScreen2AssetsIndexSegmentRomStart, &_namingScreen2AssetsIndexSegmentRomEnd, NULL, NULL, (u8*)NAMING_SCREEN_TEXTURE_BUFFER, NULL, (u16*)NAMING_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)NAMING_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)NAMING_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    dmaSprite(0x88, &_namingScreen2TextureSegmentRomStart, &_namingScreen2TextureSegmentRomEnd, &_namingScreen2AssetsIndexSegmentRomStart, &_namingScreen2AssetsIndexSegmentRomEnd, NULL, NULL, (u8*)NAMING_SCREEN_TEXTURE_BUFFER, NULL, (u16*)NAMING_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)NAMING_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)NAMING_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    dmaSprite(0x8A, &_namingScreen2TextureSegmentRomStart, &_namingScreen2TextureSegmentRomEnd, &_namingScreen2AssetsIndexSegmentRomStart, &_namingScreen2AssetsIndexSegmentRomEnd, NULL, NULL, (u8*)NAMING_SCREEN_TEXTURE_BUFFER, NULL, (u16*)NAMING_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)NAMING_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)NAMING_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    dmaSprite(0x8B, &_namingScreen2TextureSegmentRomStart, &_namingScreen2TextureSegmentRomEnd, &_namingScreen2AssetsIndexSegmentRomStart, &_namingScreen2AssetsIndexSegmentRomEnd, NULL, NULL, (u8*)NAMING_SCREEN_TEXTURE_BUFFER, NULL, (u16*)NAMING_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)NAMING_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)NAMING_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    dmaSprite(0x8C, &_namingScreen2TextureSegmentRomStart, &_namingScreen2TextureSegmentRomEnd, &_namingScreen2AssetsIndexSegmentRomStart, &_namingScreen2AssetsIndexSegmentRomEnd, NULL, NULL, (u8*)NAMING_SCREEN_TEXTURE_BUFFER, NULL, (u16*)NAMING_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)NAMING_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)NAMING_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    dmaSprite(0x8D, &_namingScreen2TextureSegmentRomStart, &_namingScreen2TextureSegmentRomEnd, &_namingScreen2AssetsIndexSegmentRomStart, &_namingScreen2AssetsIndexSegmentRomEnd, NULL, NULL, (u8*)NAMING_SCREEN_TEXTURE_BUFFER, NULL, (u16*)NAMING_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)NAMING_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)NAMING_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    dmaSprite(0x8E, &_namingScreen2TextureSegmentRomStart, &_namingScreen2TextureSegmentRomEnd, &_namingScreen2AssetsIndexSegmentRomStart, &_namingScreen2AssetsIndexSegmentRomEnd, NULL, NULL, (u8*)NAMING_SCREEN_TEXTURE_BUFFER, NULL, (u16*)NAMING_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)NAMING_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)NAMING_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    dmaSprite(0x8F, &_namingScreen2TextureSegmentRomStart, &_namingScreen2TextureSegmentRomEnd, &_namingScreen2AssetsIndexSegmentRomStart, &_namingScreen2AssetsIndexSegmentRomEnd, NULL, NULL, (u8*)NAMING_SCREEN_TEXTURE_BUFFER, NULL, (u16*)NAMING_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)NAMING_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)NAMING_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    
    dmaSprite(LANDSCAPE_BACKGROUND, &_namingScreenBackgroundTextureSegmentRomStart, &_namingScreenBackgroundTextureSegmentRomEnd, &_namingScreenBackgroundAssetsIndexSegmentRomStart, &_namingScreenBackgroundAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)NAMING_SCREEN_BACKGROUND_TEXTURE_BUFFER, NULL, (u16*)NAMING_SCREEN_BACKGROUND_PALETTE_BUFFER, (AnimationFrameMetadata*)NAMING_SCREEN_BACKGROUND_ANIMATION_FRAME_METADATA_BUFFER, (u32*)NAMING_SCREEN_BACKGROUND_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    
    dmaSprite(0x91, &_dialogueIconsTextureSegmentRomStart, &_dialogueIconsTextureSegmentRomEnd, &_dialogueIconsAssetsIndexSegmentRomStart, &_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)DIALOGUE_ICON_TEXTURE_BUFFER, NULL, (u8*)DIALOGUE_ICON_PALETTE_BUFFER, (AnimationFrameMetadata*)DIALOGUE_ICON_ANIMATION_FRAME_METADATA_BUFFER, (u32*)DIALOGUE_ICON_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    
    setBilinearFiltering(0x8F, TRUE);
    setBilinearFiltering(LANDSCAPE_BACKGROUND, TRUE);

    setSpriteScale(LANDSCAPE_BACKGROUND, 2.0f, 2.0f, 1.0f);
    
    setSpriteViewSpacePosition(0x80, 0.0f, 0.0f, -1.0f);
    setSpriteViewSpacePosition(0x81, 0.0f, -1.0f, 10.0f);
    setSpriteViewSpacePosition(0x82, 0.0f, 0.0f, -2.0f);
    setSpriteViewSpacePosition(0x8F, 0.0f, 0.0f, 1.0f);
    setSpriteViewSpacePosition(0x83, 8.0f, 72.0f, 20.0f);
    setSpriteViewSpacePosition(0x84, 24.0f, 72.0f, 20.0f);
    setSpriteViewSpacePosition(0x85, 40.0f, 72.0f, 20.0f);
    setSpriteViewSpacePosition(0x86, 56.0f, 72.0f, 20.0f);
    setSpriteViewSpacePosition(0x87, 72.0f, 72.0f, 20.0f);
    setSpriteViewSpacePosition(0x88, 88.0f, 72.0f, 20.0f);
    setSpriteViewSpacePosition(0x8A, 24.0f, 68.0f, 20.0f);
    setSpriteViewSpacePosition(0x8B, 40.0f, 68.0f, 20.0f);
    setSpriteViewSpacePosition(0x8C, 56.0f, 68.0f, 20.0f);
    setSpriteViewSpacePosition(0x8D, 72.0f, 68.0f, 20.0f);
    setSpriteViewSpacePosition(0x8E, 88.0f, 68.0f, 20.0f);
    setSpriteViewSpacePosition(0x91, 0.0f, 0.0f, 20.0f);
    
    startSpriteAnimation(0x80, 3, 0);
    startSpriteAnimation(0x81, 1, 2);
    startSpriteAnimation(0x82, 2, 0);
    startSpriteAnimation(0x8F, 0, 0);
    startSpriteAnimation(LANDSCAPE_BACKGROUND, 0, 0);
    startSpriteAnimation(0x83, 2, 0);
    startSpriteAnimation(0x84, 2, 0);
    startSpriteAnimation(0x85, 2, 0);
    startSpriteAnimation(0x86, 2, 0);
    startSpriteAnimation(0x87, 2, 0);
    startSpriteAnimation(0x88, 2, 0);
    startSpriteAnimation(0x8A, 2, 1);
    startSpriteAnimation(0x8B, 2, 1);
    startSpriteAnimation(0x8C, 2, 1);
    startSpriteAnimation(0x8D, 2, 1);
    startSpriteAnimation(0x8E, 2, 1);
    startSpriteAnimation(0x91, 2, 0);
    setSpritePaletteIndex(0x91, 3);
    
    setSpriteColor(0x80, 0, 0, 0, 0);
    setSpriteColor(0x81, 0, 0, 0, 0);
    setSpriteColor(0x82, 0, 0, 0, 0);
    setSpriteColor(0x8F, 0, 0, 0, 0);
    setSpriteColor(LANDSCAPE_BACKGROUND, 0, 0, 0, 0);
    setSpriteColor(0x83, 0, 0, 0, 0);
    setSpriteColor(0x84, 0, 0, 0, 0);
    setSpriteColor(0x85, 0, 0, 0, 0);
    setSpriteColor(0x86, 0, 0, 0, 0);
    setSpriteColor(0x87, 0, 0, 0, 0);
    setSpriteColor(0x88, 0, 0, 0, 0);
    setSpriteColor(0x89, 0, 0, 0, 0);
    setSpriteColor(0x8A, 0, 0, 0, 0);
    setSpriteColor(0x8B, 0, 0, 0, 0);
    setSpriteColor(0x8C, 0, 0, 0, 0);
    setSpriteColor(0x8D, 0, 0, 0, 0);
    setSpriteColor(0x8E, 0, 0, 0, 0);
    setSpriteColor(0x91, 0, 0, 0, 0);
    
    updateSpriteRGBA(0x80, 0xFF, 0xFF, 0xFF, 0xFF, 8);
    updateSpriteRGBA(0x81, 0xFF, 0xFF, 0xFF, 0xFF, 8);
    updateSpriteRGBA(0x82, 0xFF, 0xFF, 0xFF, 0xFF, 8);
    updateSpriteRGBA(0x8F, 0xFF, 0xFF, 0xFF, 0xFF, 8);
    updateSpriteRGBA(LANDSCAPE_BACKGROUND, 0xFF, 0xFF, 0xFF, 0xFF, 8);
    updateSpriteRGBA(0x83, 0xFF, 0xFF, 0xFF, 0xFF, 8);
    updateSpriteRGBA(0x84, 0xFF, 0xFF, 0xFF, 0xFF, 8);
    updateSpriteRGBA(0x85, 0xFF, 0xFF, 0xFF, 0xFF, 8);
    updateSpriteRGBA(0x86, 0xFF, 0xFF, 0xFF, 0xFF, 8);
    updateSpriteRGBA(0x87, 0xFF, 0xFF, 0xFF, 0xFF, 8);
    updateSpriteRGBA(0x88, 0xFF, 0xFF, 0xFF, 0xFF, 8);
    updateSpriteRGBA(0x89, 0xFF, 0xFF, 0xFF, 0xFF, 8);
    updateSpriteRGBA(0x8A, 0xFF, 0xFF, 0xFF, 0xFF, 8);
    updateSpriteRGBA(0x8B, 0xFF, 0xFF, 0xFF, 0xFF, 8);
    updateSpriteRGBA(0x8C, 0xFF, 0xFF, 0xFF, 0xFF, 8);
    updateSpriteRGBA(0x8D, 0xFF, 0xFF, 0xFF, 0xFF, 8);
    updateSpriteRGBA(0x8E, 0xFF, 0xFF, 0xFF, 0xFF, 8);
    updateSpriteRGBA(0x91, 0xFF, 0xFF, 0xFF, 0xFF, 8);

    namingScreenContext.gridY = 0;
    namingScreenContext.gridX = 0;
    namingScreenContext.unk_1C = 0;
    namingScreenContext.flags = NAMING_SCREEN_CHARSET_ENGLISH;

    namingScreenContext.cursor.x = -126.0f;
    namingScreenContext.cursor.y = 16.0f;
    namingScreenContext.shadow.x = -116.0f;
    namingScreenContext.shadow.y = 26.0f;

    for (i = 0; i < NAME_LENGTH; i++) {
        
        if (namingScreenContext.name[i] != 0xFF) {
            
            setSpriteColor(NAMING_SCREEN_SPRITES_BASE + i, 0xFF, 0xFF, 0xFF, 0xFF);
            resetAnimationState(NAMING_SCREEN_SPRITES_BASE + i);

            if (i != 5) {
               
                setSpriteColor(0x8A + i, 0xFF, 0xFF, 0xFF, 0xFF);
                resetAnimationState(0x8A + i);
                
            } 

            
        } else {
            break;
        }
    } 
      
    namingScreenContext.flags |= i << NAMING_SCREEN_NAME_POSITION_SHIFT;
    
    setGameVariableString(0, namingScreenContext.name, 6);
    
    initializeEmptyMessageBox(3, (void*)MESSAGE_BOX_3_TEXT_BUFFER);
    setMessageBoxViewSpacePosition(3, 48.0f, 72.0f, 30.0f);
    
    setMessageBoxLineAndRowSizes(3, 6, 1);
    setMessageBoxSpacing(3, 2, 2);
    setMessageBoxFont(3, 0xE, 0xE, (u8*)FONT_TEXTURE_BUFFER, FONT_PALETTE_1_BUFFER);
    setMessageBoxInterpolationWithFlags(3, 1, 1);
    setMessageBoxSpriteIndices(3, 0xFF, 0xFF, 0xFF);
    initializeMessageBox(3, 0xA, 0xD, MESSAGE_BOX_MODE_NO_INPUT);
    setMessageBoxRGBA(3, 0, 0, 0, 0);
    setMessageBoxRGBAWithTransition(3, 0xFF, 0xFF, 0xFF, 0xFF, 8);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/namingScreen", selectCharacterOrConfirm);

bool selectCharacterOrConfirm(void) {

    bool result = FALSE; 

    s32 charsetFlags;
    s32 tempFlags;
    s32 tempFlags2;
    
    s32 characterPosition;
    u32 spriteIndex;
    u32 spriteIndex2;
    
    u8 character;
    s32 temp, temp2;
    u32 temp3;

    if (namingScreenContext.gridX >= 10 && namingScreenContext.gridY == 5) {

        charsetFlags = namingScreenContext.flags & NAMING_SCREEN_CHARSET_MASK;

        // navigate to other charset (japanese version)
        if (namingScreenContext.gridX == 10) {
            
            resetAnimationState(0x81);
            
            switch (charsetFlags) {                      
                case 0:                                 
                    temp = 2;
                    tempFlags = 2;
                    break;
                case 1:             
                    temp = 0;
                    tempFlags = 0;
                    break;
                case 2:                                 
                    temp = 1;
                    tempFlags = 1;
                    break;
            }    

            startSpriteAnimation(0x81, 1, temp);
            charsetFlags = tempFlags;
        
        // navigate to other charset (japanese version)
        } else if (namingScreenContext.gridX == 11) {
            
            resetAnimationState(0x81);
                
            switch (charsetFlags) {                      
                case 0:                                 
                    temp2 = 1;
                    tempFlags2 = 1;
                    break;
                case 1:                                 
                    temp2 = 2;
                    tempFlags2 = 2;
                    break;
                case 2:                                 
                    temp2 = 0;
                    tempFlags2 = 0;
                    break;
            }    

            startSpriteAnimation(0x81, 1, temp2);
            charsetFlags = tempFlags2;
                
        // ok button
        } else if (namingScreenContext.gridX == 12) {

            // get current character index
            temp3 = (namingScreenContext.flags & NAMING_SCREEN_NAME_POSITION_MASK) >> NAMING_SCREEN_NAME_POSITION_SHIFT;
                        
            resetAnimationState(0x91);
            startSpriteAnimation(0x91, 2, 2);
            setSpritePaletteIndex(0x91, 5);
            
            if (temp3 == 0) {

                initializeDialogueSession(0, 0x3A, 8, 0);
                namingScreenContext.flags |= NAMING_SCREEN_EMPTY_NAME_ERROR;
            
            } else if (namingScreenContext.screenType == 0) {
            
                setMessageBoxRGBAWithTransition(3, 0, 0, 0, 0, 8);
                updateSpriteRGBA(0x80, 0, 0, 0, 0, 8);
                updateSpriteRGBA(0x81, 0, 0, 0, 0, 8);
                updateSpriteRGBA(0x82, 0, 0, 0, 0, 8);
                updateSpriteRGBA(0x8F, 0, 0, 0, 0, 8);
                updateSpriteRGBA(0x90, 0, 0, 0, 0, 8);
                updateSpriteRGBA(0x83, 0, 0, 0, 0, 8);
                updateSpriteRGBA(0x84, 0, 0, 0, 0, 8);
                updateSpriteRGBA(0x85, 0, 0, 0, 0, 8);
                updateSpriteRGBA(0x86, 0, 0, 0, 0, 8);
                updateSpriteRGBA(0x87, 0, 0, 0, 0, 8);
                updateSpriteRGBA(0x88, 0, 0, 0, 0, 8);
                updateSpriteRGBA(0x89, 0, 0, 0, 0, 8);
                updateSpriteRGBA(0x8A, 0, 0, 0, 0, 8);
                updateSpriteRGBA(0x8B, 0, 0, 0, 0, 8);
                updateSpriteRGBA(0x8C, 0, 0, 0, 0, 8);
                updateSpriteRGBA(0x8D, 0, 0, 0, 0, 8);
                updateSpriteRGBA(0x8E, 0, 0, 0, 0, 8);
                updateSpriteRGBA(0x91, 0, 0, 0, 0, 8);
                
                namingScreenContext.flags |= NAMING_SCREEN_GOTO_SEASON_SELECT;
                result = TRUE;

            } else {

                namingScreenContext.flags |= (NAMING_SCREEN_STATE_ANIMAL_CONFIRM << NAMING_SCREEN_SCREEN_STATE_SHIFT);
                initializeDialogueSession(0, 0x3A, 0xB, 0);
            
            }

        }

        namingScreenContext.flags &= ~NAMING_SCREEN_CHARSET_MASK;
        namingScreenContext.flags |= (charsetFlags & NAMING_SCREEN_CHARSET_MASK);

    } else {

        characterPosition = ((namingScreenContext.flags & NAMING_SCREEN_NAME_POSITION_MASK) >> NAMING_SCREEN_NAME_POSITION_SHIFT);
        
        spriteIndex = NAMING_SCREEN_SPRITES_BASE + characterPosition;
        spriteIndex2 = 0x89 + characterPosition;
        
        switch (namingScreenContext.flags & NAMING_SCREEN_CHARSET_MASK) {

            // japanese 1
            case 0:
                character = gridToCharacterMapJP1[namingScreenContext.gridX][namingScreenContext.gridY];
                break;

            // japanese 2
            case 1:
                character = gridToCharacterMapJP2[namingScreenContext.gridX][namingScreenContext.gridY];
                break;

            // english
            case 2:
                character = gridToCharacterMapEN[namingScreenContext.gridX][namingScreenContext.gridY];
                break;
            
        }

        if (characterPosition == 6) {
            namingScreenContext.name[5] = character;
        } else {
            namingScreenContext.name[characterPosition] = character;
        }

        characterPosition++;
        
        if (characterPosition < 7) {

            namingScreenContext.flags &= ~NAMING_SCREEN_NAME_POSITION_MASK;
            namingScreenContext.flags |= (characterPosition << NAMING_SCREEN_NAME_POSITION_SHIFT);
            
            resetAnimationState(spriteIndex);
            
            if (characterPosition != 6) {
                resetAnimationState(spriteIndex2 + 1);
            }
            
        } else {
            return;
        }
        
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/namingScreen", moveCursorLeft);

void moveCursorLeft(void) {

    namingScreenContext.cursor.x -= 16.0f;
    namingScreenContext.shadow.x -= 16.0f;

    if (namingScreenContext.gridX != 0) {
        if (!(namingScreenContext.gridX % 5)) {
            namingScreenContext.cursor.x -= 3.0f;
            namingScreenContext.shadow.x -= 3.0f;
        }
    } 

    namingScreenContext.gridX -= 1;
    
    if ((namingScreenContext.gridX << 0x18) < 0) {

        namingScreenContext.gridX = 14;
        namingScreenContext.cursor.x = 104.0f;
        namingScreenContext.shadow.x = 114.0f;
        
    }
    
    updateBottomRowUI();

}

//INCLUDE_ASM("asm/nonmatchings/game/namingScreen", moveCursorRight);

void moveCursorRight(void) {
        
    namingScreenContext.cursor.x += 16.0f;
    namingScreenContext.shadow.x += 16.0f;

    namingScreenContext.gridX++;

    if (namingScreenContext.gridX >= 15) {
        namingScreenContext.gridX = 0;
    }

    if (namingScreenContext.gridX != 0) {
        
        if ((namingScreenContext.gridX % 5) == 0) {
            namingScreenContext.cursor.x += 3.0f;
            namingScreenContext.shadow.x += 3.0f;
        }

    } else {
        // FIXME: probably different control flow
        do {} while (0);
        namingScreenContext.cursor.x = -126.0f;
        namingScreenContext.shadow.x = -116.0f;

    }

    updateBottomRowUI();

}


//INCLUDE_ASM("asm/nonmatchings/game/namingScreen", moveCursorUp);

void moveCursorUp(void) {

    namingScreenContext.cursor.y += 16.0f;
    namingScreenContext.shadow.y += 16.0f;
    
    namingScreenContext.gridY -= 1;
    
    if ((namingScreenContext.gridY << 0x18) < 0) {
        
        namingScreenContext.gridY = 5;

        namingScreenContext.cursor.y = -64.0f;
        namingScreenContext.shadow.y = -54.0f;

        snapCursorToOKButton();

    }

    if (namingScreenContext.gridX >= 10 && namingScreenContext.gridY == 4) {

        namingScreenContext.gridX = namingScreenContext.savedGridX;

        namingScreenContext.cursor.y = -48.0f;
        namingScreenContext.shadow.y = -38.0f;

        namingScreenContext.cursor.x = (namingScreenContext.savedGridX * 16.0f) + -126.0f + 6.0f;
        namingScreenContext.shadow.x = (namingScreenContext.savedGridX * 16.0f) + -116.0f + 6.0f;
        
        resetAnimationState(0x82);
        startSpriteAnimation(0x82, 2, 0);

        resetAnimationState(0x91);
        startSpriteAnimation(0x91, 2, 0);
        
        setSpritePaletteIndex(0x91, 3);
        
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/namingScreen", moveCursorDown);

void moveCursorDown(void) {

    namingScreenContext.cursor.y -= 16.0f;
    namingScreenContext.shadow.y -= 16.0f;
    
    namingScreenContext.gridY += 1;
    
    if (namingScreenContext.gridY >= 6) {
        
        namingScreenContext.gridY = 0;

        namingScreenContext.cursor.y = 16.0f;
        namingScreenContext.shadow.y = 26.0f;

        if (namingScreenContext.gridX >= 10) {

            namingScreenContext.gridX = namingScreenContext.savedGridX;

            namingScreenContext.cursor.x = (namingScreenContext.gridX * 16.0f);
            namingScreenContext.cursor.x += -126.0f;
            namingScreenContext.cursor.x += 6.0f;
            
            namingScreenContext.shadow.x = (namingScreenContext.gridX * 16.0f);
            namingScreenContext.shadow.x += -116.0f;
            namingScreenContext.shadow.x += 6.0f;

            resetAnimationState(0x82);
            startSpriteAnimation(0x82, 2, 0);
            resetAnimationState(0x91);
            startSpriteAnimation(0x91, 2, 0);
            setSpritePaletteIndex(0x91, 3);
            
        }

    }

    snapCursorToOKButton();

}

//INCLUDE_ASM("asm/nonmatchings/game/namingScreen", checkNameProhibited);

// unused or inline
bool checkNameProhibited(void) {
    
    bool processingChar;
    bool doneProcessingWord;
    int processedWordCount;
    
    int spaceChar;
    int endChar;
    int endWord;
    
    u8 *currentNamePtr;
    u8 *D_8011C680_ptr;
    u8 *D_8011C680_stringPtr;
    u8 *namingScreenContextNamePtr;
    
    processedWordCount = 0;
    namingScreenContextNamePtr = namingScreenContext.name;
    
    spaceChar = 0xEE;
    endChar = 0xFF;
    
    D_8011C680_ptr = D_8011C680;
    
    while (processedWordCount < 13) {
        
        // FIXME: fake
        do { 
            processingChar = 0; 
            doneProcessingWord = 0; 
            D_8011C680_stringPtr = D_8011C680_ptr; 
        } while (0);
        
        currentNamePtr = namingScreenContextNamePtr;
        endWord = D_8011C680_stringPtr + 6;
        
        do {
        
            if (processingChar || (*currentNamePtr == 0xEE || *currentNamePtr == 0xFF) == 0) {
                
                processingChar = TRUE;
                
                if (*currentNamePtr != *D_8011C680_stringPtr) {
                    
                    if (*currentNamePtr != spaceChar) {
                        doneProcessingWord = TRUE;
                        break;
                    } else if (*D_8011C680_stringPtr != endChar) {
                        doneProcessingWord = TRUE;
                        break;
                    }
                    
                } else {
                
                    D_8011C680_stringPtr++;
                    currentNamePtr++;
                    continue;
                
                }
                
            }
                    
            D_8011C680_stringPtr++;
        
            // FIXME
            if (*D_8011C680_ptr) {
                currentNamePtr++;
            } else {
                currentNamePtr++;
            }
        
        } while ((s32)D_8011C680_stringPtr < (s32)endWord);
            
        if (doneProcessingWord) {
            
            processedWordCount++;
            // skip to next word
            D_8011C680_ptr += 6;
        
            if (processedWordCount >= 13) {
                return TRUE;
            }
        
        } else {
            return FALSE;
        }
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/namingScreen", deactivateNamingScreenSprites);

void deactivateNamingScreenSprites(void) {

    s32 i = 0x80;

    while (i < 0x92) {
        deactivateSprite(i);
        i++;
    } 
    
}

// alternate
/*
void deactivateNamingScreenSprites(void) {

    s32 i;

    for (i = 0x80; i < 0x92; i++) {
        deactivateSprite(i);
    } 
    
}
*/

//INCLUDE_ASM("asm/nonmatchings/game/namingScreen", updateBottomRowUI);

void updateBottomRowUI(void) {

    s8 temp;

    if (namingScreenContext.gridY == 5) {
        
        temp = namingScreenContext.gridX - 9;
        
        switch (temp) {

            case 0:
            case 2:
                namingScreenContext.savedGridX = 10;
                namingScreenContext.gridX = 9;
                
                resetAnimationState(0x91);
                startSpriteAnimation(0x91, 2, 0);
                setSpritePaletteIndex(0x91, 3);

                namingScreenContext.cursor.x = 21.0f;
                namingScreenContext.shadow.x = 31.0f;
                namingScreenContext.cursor.y = -64.0f;
                namingScreenContext.shadow.y = -54.0f;
                
                resetAnimationState(0x82);
                startSpriteAnimation(0x82, 2, 0);
                break;

            case 1:

                namingScreenContext.gridX = 12;

            case 3:

                namingScreenContext.savedGridX = 13;

                namingScreenContext.cursor.x = 100.0f;
                namingScreenContext.cursor.y = -95.0f;
                
                resetAnimationState(0x82);
                resetAnimationState(0x91);
                startSpriteAnimation(0x91, 2, 1);
                setSpritePaletteIndex(0x91, 4); 

                break;

            case 4:
                
                namingScreenContext.gridX = 0;

                namingScreenContext.cursor.x = -126.0f;
                namingScreenContext.shadow.x = -116.0f;
                namingScreenContext.cursor.y = -64.0f;
                namingScreenContext.shadow.y = -54.0f;

                resetAnimationState(0x82);
                startSpriteAnimation(0x82, 2, 0);
                resetAnimationState(0x91);
                startSpriteAnimation(0x91, 2, 0);
                setSpritePaletteIndex(0x91, 3);
                
                break;
            
            case 5:

                namingScreenContext.savedGridX = 13;
                namingScreenContext.gridX = 12;

                namingScreenContext.cursor.x = 100.0f;
                namingScreenContext.cursor.y = -95.0f;

                resetAnimationState(0x82);
                resetAnimationState(0x91);
                startSpriteAnimation(0x91, 2, 1);
                setSpritePaletteIndex(0x91, 4);
            
                break;
            
        }
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/namingScreen", snapCursorToOKButton);

void snapCursorToOKButton(void) {
    
    if ((namingScreenContext.gridY == 5) && (namingScreenContext.gridX >= 10)) {

        namingScreenContext.savedGridX = namingScreenContext.gridX;
        
        namingScreenContext.gridX = 12;
        namingScreenContext.cursor.x = 100.0f;
        namingScreenContext.cursor.y = -95.0f;
        
        resetAnimationState(0x82);
        resetAnimationState(0x91);
        startSpriteAnimation(0x91, 2, 1);
        setSpritePaletteIndex(0x91, 4);
        
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/namingScreen", handleNamingGridInput);

void handleNamingGridInput(void) {
        
    bool set = FALSE;
    s32 index;

    if (namingScreenContext.flags & NAMING_SCREEN_EMPTY_NAME_ERROR) {
        resetAnimationState(0x91);
        startSpriteAnimation(0x91, 2, 1);
        setSpritePaletteIndex(0x91, 4);
        namingScreenContext.flags &= ~NAMING_SCREEN_EMPTY_NAME_ERROR;
    }

    if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_NORTHWEST)) {
        set = TRUE;
        moveCursorLeft();
        setSfx(3);
        setSfxVolume(3, SFX_VOLUME);
    }

    if (!set) {
        if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTHEAST)) {
            set = TRUE;
            moveCursorRight();
            setSfx(3);
            setSfxVolume(3, SFX_VOLUME);
        }
    }

    if (!set) {
        if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTHWEST)) {
            set = TRUE;
            moveCursorDown();
            setSfx(3);
            setSfxVolume(3, SFX_VOLUME);
        }
    }

    if (!set) {

        if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_NORTHEAST)) {
            moveCursorUp();
            set = TRUE;
            setSfx(3);
            setSfxVolume(3, SFX_VOLUME);
        }
    }

    if (!set) {

        if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {

            set = TRUE;
            setSfx(1);
            setSfxVolume(CLOSE, SFX_VOLUME);

            if (selectCharacterOrConfirm()) {
                return;
            }
            
        }       
            
        
    }

    if (!set) {
            
        if (checkButtonPressed(CONTROLLER_1, BUTTON_B)) {

            set = TRUE;
            setSfx(1);
            setSfxVolume(CLOSE, SFX_VOLUME);
            
            index = getSpriteIndexFromFlags(namingScreenContext.flags);
            
            if (index >= 0) {
                
                namingScreenContext.name[index] = 0xFF;

                setSpriteIndexOnFlags(index);
                
                startSpriteAnimation(NAMING_SCREEN_SPRITES_BASE + index, 2, 0);
                
                if (index != 5) {
                    startSpriteAnimation(0x8A + index, 2, 1);
                }
                
            }
                
        }        
    } 
        
    if (!set) { 
        
        if (checkButtonPressed(CONTROLLER_1, BUTTON_START)) {

            namingScreenContext.savedGridX = 14;
            namingScreenContext.gridX = 12;
            namingScreenContext.gridY = 5;
                    
            namingScreenContext.cursor.x = 100.0f;
            namingScreenContext.cursor.y = -95.0f;
            
            resetAnimationState(0x82);
            resetAnimationState(0x91);
            startSpriteAnimation(0x91, 2, 1);
            setSpritePaletteIndex(0x91, 4);
            
            setSfx(3);
            setSfxVolume(3, SFX_VOLUME);
        
        }

    }

    setSpriteViewSpacePosition(0x80, namingScreenContext.cursor.x, namingScreenContext.cursor.y, 20.0f);
    setSpriteViewSpacePosition(0x82, namingScreenContext.shadow.x, namingScreenContext.shadow.y, 5.0f);

}

//INCLUDE_ASM("asm/nonmatchings/game/namingScreen", loadSeasonSelectionSprites);

void loadSeasonSelectionSprites(void) {
    
    dmaSprite(0x80, &_dialogueIconsTextureSegmentRomStart, &_dialogueIconsTextureSegmentRomEnd, &_dialogueIconsAssetsIndexSegmentRomStart, &_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, DIALOGUE_ICON_TEXTURE_BUFFER, 0, (u16*)DIALOGUE_ICON_PALETTE_BUFFER, (AnimationFrameMetadata*)DIALOGUE_ICON_ANIMATION_FRAME_METADATA_BUFFER, (u32*)DIALOGUE_ICON_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    
    // season names
    dmaSprite(0x83, &_calendarTextureSegmentRomStart, &_calendarTextureSegmentRomEnd, &_calendarAssetsIndexSegmentRomStart, &_calendarAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)NAMING_SCREEN_SEASONS_TEXTURE_BUFFER, NULL, (u16*)NAMING_SCREEN_SEASONS_PALETTE_BUFFER, (AnimationFrameMetadata*)NAMING_SCREEN_SEASONS_ANIMATION_FRAME_METADATA_BUFFER, (u32*)NAMING_SCREEN_SEASONS_TEXTURE_TO_PALETTE_BUFFER, NULL, 0, FALSE);
    dmaSprite(0x84, &_calendarTextureSegmentRomStart, &_calendarTextureSegmentRomEnd, &_calendarAssetsIndexSegmentRomStart, &_calendarAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)NAMING_SCREEN_SEASONS_TEXTURE_BUFFER, NULL, (u16*)NAMING_SCREEN_SEASONS_PALETTE_BUFFER, (AnimationFrameMetadata*)NAMING_SCREEN_SEASONS_ANIMATION_FRAME_METADATA_BUFFER, (u32*)NAMING_SCREEN_SEASONS_TEXTURE_TO_PALETTE_BUFFER, NULL, 0, FALSE);
    dmaSprite(0x85, &_calendarTextureSegmentRomStart, &_calendarTextureSegmentRomEnd, &_calendarAssetsIndexSegmentRomStart, &_calendarAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)NAMING_SCREEN_SEASONS_TEXTURE_BUFFER, NULL, (u16*)NAMING_SCREEN_SEASONS_PALETTE_BUFFER, (AnimationFrameMetadata*)NAMING_SCREEN_SEASONS_ANIMATION_FRAME_METADATA_BUFFER, (u32*)NAMING_SCREEN_SEASONS_TEXTURE_TO_PALETTE_BUFFER, NULL, 0, FALSE);
    dmaSprite(0x86, &_calendarTextureSegmentRomStart, &_calendarTextureSegmentRomEnd, &_calendarAssetsIndexSegmentRomStart, &_calendarAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)NAMING_SCREEN_SEASONS_TEXTURE_BUFFER, NULL, (u16*)NAMING_SCREEN_SEASONS_PALETTE_BUFFER, (AnimationFrameMetadata*)NAMING_SCREEN_SEASONS_ANIMATION_FRAME_METADATA_BUFFER, (u32*)NAMING_SCREEN_SEASONS_TEXTURE_TO_PALETTE_BUFFER, NULL, 0, FALSE);
    
    dmaSprite(0x89, &_namingScreen1TextureSegmentRomStart, &_namingScreen1TextureSegmentRomEnd, &_namingScreen1AssetsIndexSegmentRomStart, &_namingScreen1AssetsIndexSegmentRomEnd, NULL, NULL, (u8*)NAMING_SCREEN_CONFIRMATION_SCREEN_TEXTURE_BUFFER, NULL, (u16*)NAMING_SCREEN_CONFIRMATION_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)NAMING_SCREEN_CONFIRMATION_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)NAMING_SCREEN_CONFIRMATION_SCREEN_TEXTURE_TO_PALETTE_BUFFER, NULL, 0, FALSE);
    dmaSprite(0x8A, &_namingScreen1TextureSegmentRomStart, &_namingScreen1TextureSegmentRomEnd, &_namingScreen1AssetsIndexSegmentRomStart, &_namingScreen1AssetsIndexSegmentRomEnd, NULL, NULL, (u8*)NAMING_SCREEN_CONFIRMATION_SCREEN_TEXTURE_BUFFER, NULL, (u16*)NAMING_SCREEN_CONFIRMATION_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)NAMING_SCREEN_CONFIRMATION_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)NAMING_SCREEN_CONFIRMATION_SCREEN_TEXTURE_TO_PALETTE_BUFFER, NULL, 0, FALSE);
    dmaSprite(0x8B, &_namingScreen1TextureSegmentRomStart, &_namingScreen1TextureSegmentRomEnd, &_namingScreen1AssetsIndexSegmentRomStart, &_namingScreen1AssetsIndexSegmentRomEnd, NULL, NULL, (u8*)NAMING_SCREEN_CONFIRMATION_SCREEN_TEXTURE_BUFFER, NULL, (u16*)NAMING_SCREEN_CONFIRMATION_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)NAMING_SCREEN_CONFIRMATION_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)NAMING_SCREEN_CONFIRMATION_SCREEN_TEXTURE_TO_PALETTE_BUFFER, NULL, 0, FALSE);
    dmaSprite(0x8C, &_namingScreen1TextureSegmentRomStart, &_namingScreen1TextureSegmentRomEnd, &_namingScreen1AssetsIndexSegmentRomStart, &_namingScreen1AssetsIndexSegmentRomEnd, NULL, NULL, (u8*)NAMING_SCREEN_CONFIRMATION_SCREEN_TEXTURE_BUFFER, NULL, (u16*)NAMING_SCREEN_CONFIRMATION_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)NAMING_SCREEN_CONFIRMATION_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)NAMING_SCREEN_CONFIRMATION_SCREEN_TEXTURE_TO_PALETTE_BUFFER, NULL, 0, FALSE);

    dmaSprite(LANDSCAPE_BACKGROUND, &_namingScreenBackgroundTextureSegmentRomStart, &_namingScreenBackgroundTextureSegmentRomEnd, &_namingScreenBackgroundAssetsIndexSegmentRomStart, &_namingScreenBackgroundAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)NAMING_SCREEN_BACKGROUND_TEXTURE_BUFFER, 0, (u16*)NAMING_SCREEN_BACKGROUND_PALETTE_BUFFER, (AnimationFrameMetadata*)NAMING_SCREEN_BACKGROUND_ANIMATION_FRAME_METADATA_BUFFER, (u32*)NAMING_SCREEN_BACKGROUND_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);

    setSpriteViewSpacePosition(0x80, 0, 0, -1.0f);

    setSpriteViewSpacePosition(0x83, -48.0f, 0, 10.0f);
    setSpriteViewSpacePosition(0x84, 96.0f, 0, 10.0f);
    setSpriteViewSpacePosition(0x85, -48.0f, -48.0f, 10.0f);
    setSpriteViewSpacePosition(0x86, 96.0f, -48.0f, 10.0f);

    setSpriteViewSpacePosition(0x89, 0, 0, 5.0f);
    setSpriteViewSpacePosition(0x8A, 0, 0, 5.0f);
    setSpriteViewSpacePosition(0x8B, 0, 0, 5.0f);
    setSpriteViewSpacePosition(0x8C, 0, 0, 5.0f);

    setSpritePaletteIndex(0x84, 0);
    setSpritePaletteIndex(0x85, 0);
    setSpritePaletteIndex(0x86, 0);
    setSpritePaletteIndex(0x8A, 2);
    setSpritePaletteIndex(0x8B, 3);
    setSpritePaletteIndex(0x8C, 4);
    setBilinearFiltering(LANDSCAPE_BACKGROUND, TRUE);
    setSpriteScale(LANDSCAPE_BACKGROUND, 2.0f, 2.0f, 1.0f);
    startSpriteAnimation(0x80, 3, 0);
    startSpriteAnimation(0x83, 4, 0);
    startSpriteAnimation(0x84, 4, 1);
    startSpriteAnimation(0x85, 4, 2);
    startSpriteAnimation(0x86, 4, 3);
    startSpriteAnimation(0x89, 0, 0);
    startSpriteAnimation(0x8A, 0, 1);
    startSpriteAnimation(0x8B, 0, 2);
    startSpriteAnimation(0x8C, 0, 3);
    startSpriteAnimation(LANDSCAPE_BACKGROUND, 0, 0);
    setSpriteColor(0x80, 0, 0, 0, 0);
    setSpriteColor(0x83, 0, 0, 0, 0);
    setSpriteColor(0x84, 0, 0, 0, 0);
    setSpriteColor(0x85, 0, 0, 0, 0);
    setSpriteColor(0x86, 0, 0, 0, 0);
    setSpriteColor(0x89, 0, 0, 0, 0);
    setSpriteColor(0x8A, 0, 0, 0, 0);
    setSpriteColor(0x8B, 0, 0, 0, 0);
    setSpriteColor(0x8C, 0, 0, 0, 0);
    setSpriteColor(LANDSCAPE_BACKGROUND, 0, 0, 0, 0);
    
    updateSpriteRGBA(0x80, 0xFF, 0xFF, 0xFF, 0xFF, 8);
    updateSpriteRGBA(0x83, 0xFF, 0xFF, 0xFF, 0xFF, 8);
    updateSpriteRGBA(0x84, 0xFF, 0xFF, 0xFF, 0xFF, 8);
    updateSpriteRGBA(0x85, 0xFF, 0xFF, 0xFF, 0xFF, 8);
    updateSpriteRGBA(0x86, 0xFF, 0xFF, 0xFF, 0xFF, 8);
    updateSpriteRGBA(0x89, 0xFF, 0xFF, 0xFF, 0xFF, 8);
    updateSpriteRGBA(0x8A, 0xFF, 0xFF, 0xFF, 0xFF, 8);
    updateSpriteRGBA(0x8B, 0xFF, 0xFF, 0xFF, 0xFF, 8);
    updateSpriteRGBA(0x8C, 0xFF, 0xFF, 0xFF, 0xFF, 8);
    updateSpriteRGBA(LANDSCAPE_BACKGROUND, 0xFF, 0xFF, 0xFF, 0xFF, 8);
    
    namingScreenContext.selectedSeason = 0;
    namingScreenContext.dialogueIndex = 9;
    namingScreenContext.cursor.x = -128.0f;
    namingScreenContext.cursor.y = 52.0f;
    namingScreenContext.flags &= ~NAMING_SCREEN_SCREEN_STATE_MASK;
    namingScreenContext.flags |= (NAMING_SCREEN_STATE_SEASON_SELECT << NAMING_SCREEN_SCREEN_STATE_SHIFT);

}

//INCLUDE_ASM("asm/nonmatchings/game/namingScreen", handleSeasonSelectionInput);

void handleSeasonSelectionInput(void) {
    
    bool set = FALSE;

    if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTHWEST) || checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_NORTHEAST)) {

        set = TRUE;

        moveSeasonCursorVertically();

        setSfx(3);
        setSfxVolume(3, SFX_VOLUME);

    }

    if (!set) {

        if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_NORTHWEST) || checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTHEAST)) {

            set = TRUE;

            moveSeasonCursorHorizontally();

            setSfx(3);
            setSfxVolume(3, SFX_VOLUME);
        }

    }

    if (!set) {

        if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_EAST) || checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_NORTH) || checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTH) || checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_WEST)) {

            set = TRUE;

            moveSeasonCursorVertically();
            moveSeasonCursorHorizontally();

            setSfx(3);
            setSfxVolume(3, SFX_VOLUME);

        }
        
    }

    if (!set) {

        if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
            
            setSfx(1);
            setSfxVolume(CLOSE, SFX_VOLUME);

            updateSpriteRGBA(0x80, 0, 0, 0, 0, 8);
            updateSpriteRGBA(0x83, 0, 0, 0, 0, 8);
            updateSpriteRGBA(0x84, 0, 0, 0, 0, 8);
            updateSpriteRGBA(0x85, 0, 0, 0, 0, 8);
            updateSpriteRGBA(0x86, 0, 0, 0, 0, 8);
            updateSpriteRGBA(0x89, 0, 0, 0, 0, 8);
            updateSpriteRGBA(0x8A, 0, 0, 0, 0, 8);
            updateSpriteRGBA(0x8B, 0, 0, 0, 0, 8);
            updateSpriteRGBA(0x8C, 0, 0, 0, 0, 8);
            updateSpriteRGBA(LANDSCAPE_BACKGROUND, 0, 0, 0, 0, 8);

            namingScreenContext.flags |= NAMING_SCREEN_GOTO_SEASON_CONFIRM;

        }
    }
    
    setSpriteViewSpacePosition(0x80, namingScreenContext.cursor.x, namingScreenContext.cursor.y, 20.0f);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/namingScreen", moveSeasonCursorVertically);

void moveSeasonCursorVertically(void) {

    u16 temp1;
    u16 temp2;

    temp1 = namingScreenContext.selectedSeason;
    
    setSpritePaletteIndex(0x83 + temp1, 0);
    
    if (namingScreenContext.cursor.y == 52.0f) {
        namingScreenContext.cursor.y -= 48.0f;
        temp2 = temp1 + 2;
    } else {
        temp2 = temp1 - 2;
        namingScreenContext.cursor.y += 48.0f;
    }
    
    setSpritePaletteIndex(NAMING_SCREEN_SPRITES_BASE + temp2, temp2 + 1);
    
    namingScreenContext.selectedSeason = temp2;

}

//INCLUDE_ASM("asm/nonmatchings/game/namingScreen", moveSeasonCursorHorizontally);

void moveSeasonCursorHorizontally(void) {

    u16 temp1;
    u16 temp2;

    temp1 = namingScreenContext.selectedSeason;
    
    setSpritePaletteIndex(NAMING_SCREEN_SPRITES_BASE + temp1, 0);
    
    if (namingScreenContext.cursor.x == -128.0f) {
        namingScreenContext.cursor.x += 144.0f;
        temp2 = temp1 + 1;
    } else {
        temp2 = temp1 - 1;
        namingScreenContext.cursor.x -= 144.0f;
    }
 
    setSpritePaletteIndex(NAMING_SCREEN_SPRITES_BASE + temp2, temp2 + 1);
    namingScreenContext.selectedSeason = temp2;

}