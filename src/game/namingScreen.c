#include "common.h"

#include "game/namingScreen.h"

#include "system/audio.h"
#include "system/controller.h"
#include "system/dialogue.h"
#include "system/message.h"
#include "system/globalSprites.h"

#include "game/animals.h"
#include "game/game.h"
#include "game/gameAudio.h"
#include "game/transition.h"
#include "game/level.h"
#include "game/npc.h"
#include "game/player.h"
#include "game/spriteIndices.h"

// FIXME:
// instead of including the header, defining here because loadCutscene(u32) doesn't match in func_800ED974
extern void loadCutscene(void);
extern u16 gCutsceneIndex;

extern u32 _namingScreenTexture1SegmentRomStart;
extern u32 _namingScreenTexture1SegmentRomEnd;
extern u32 _namingScreenIndex1SegmentRomStart;
extern u32 _namingScreenIndex1SegmentRomEnd;

extern u32 _landscapeBackgroundTextureSegmentRomStart; 
extern u32 _landscapeBackgroundTextureSegmentRomEnd;
extern u32 _landscapeBackgroundIndexSegmentRomStart;
extern u32 _landscapeBackgroundIndexSegmentRomEnd;


// bss
extern NamingScreenContext namingScreenContext;
// FIXME: part of namingScreenContext, but needed for match 
// extern u16 D_8016FBEE;

// forward declarations
void loadNameSelectionSprites(void);
bool func_800EF578(void);
void func_800EFADC(void);
void func_800EFBEC(void);                                  
void func_800EFCF8(void);                                  
void func_800EFE84(void);   
void deactivateNamingScreenSprites(void);
void func_800F00D8(void);
void func_800F0320(void);
void loadSeasonSelectionSprites(void);
void func_800F0F84(void);


static inline int getSpriteIndexFromFlags(u16 flags) {
    int v = (flags & (0x4 | 0x8 | 0x10)) >> 2;
    v -= 1;
    return v;
}

static inline void setSpriteIndexOnFlags(s32 index) {
    namingScreenContext.flags &= ~(0x4 | 0x8 | 0x10);
    namingScreenContext.flags |= index << 2;
}

//INCLUDE_ASM("asm/nonmatchings/game/namingScreen", initializeNamingScreen);

inline void initializeNamingScreen(u8* arg0, u8 arg1) {

    s32 i = 0;
    
    namingScreenContext.unk_6 = arg1;
    namingScreenContext.unk_7 = 0;

    namingScreenContext.name = arg0;

    for (i = 0; i < NAME_LENGTH; i++) {
        namingScreenContext.name[i] = 0xFF;
    } 

    loadNameSelectionSprites();

    // dialogue boxes
    func_8003F54C(0, 0, -64.0f, 352.0f);
    func_8003F360(0, -4, 0);
    setDialogueBoxSpriteIndices(0, 1, 0, 0);
    
    namingScreenContext.unk_4 = arg1;
    
    stopCurrentSong(NAMING_SCREEN_THEME);
    setCurrentSong(NAMING_SCREEN_THEME);
    setSongVolume(NAMING_SCREEN_THEME, SONG_VOLUME);
    
    setMainLoopCallbackFunctionIndex(0x34);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/namingScreen", func_800ED974);

// main loop callback
void func_800ED974(void) {

    s32 i;
    s32 temp = (namingScreenContext.flags & 0x60) >> 5;
    s32 temp2;

    // 0x90 = index 144
    if (globalSprites[0x90].stateFlags & 0x400) {
        
        if (namingScreenContext.flags & 0x100) {

            deactivateNamingScreenSprites();

            // dialogue box
            func_8003DD14(3);

            loadSeasonSelectionSprites();
            
            namingScreenContext.flags &= ~(0x100);

            return;

        }

        if (namingScreenContext.flags & 0x200) {

            temp2 = namingScreenContext.unk_7;

            deactivateNamingScreenSprites();
            
            dmaSprite(0x80, &_namingScreenTexture1SegmentRomStart, &_namingScreenTexture1SegmentRomEnd, &_namingScreenIndex1SegmentRomStart, &_namingScreenIndex1SegmentRomEnd, NULL, NULL, (void* )0x80263E00, NULL, (void* )0x80266E00, (void* )0x80267100, (void* )0x80267200, NULL, NULL, 0);
            dmaSprite(0x84, &_namingScreenTexture1SegmentRomStart, &_namingScreenTexture1SegmentRomEnd, &_namingScreenIndex1SegmentRomStart, &_namingScreenIndex1SegmentRomEnd, NULL, NULL, (void* )0x80263E00, NULL, (void* )0x80266E00, (void* )0x80267100, (void* )0x80267200, NULL, NULL, 0);
            dmaSprite(LANDSCAPE_BACKGROUND, &_landscapeBackgroundTextureSegmentRomStart, &_landscapeBackgroundTextureSegmentRomEnd, &_landscapeBackgroundIndexSegmentRomStart, &_landscapeBackgroundIndexSegmentRomEnd, NULL, NULL, (void* )0x8026AB00, NULL, (void* )0x8026DB00, (void* )0x8026DC00, (void* )0x8026DD00, NULL, NULL, 0);
            setBilinearFiltering(LANDSCAPE_BACKGROUND, TRUE);
            setSpriteScale(LANDSCAPE_BACKGROUND, 2.0f, 2.0f, 1.0f);

            // animation
            startSpriteAnimation(0x80, 1, 0);
            startSpriteAnimation(0x84, 2, 0);
            startSpriteAnimation(LANDSCAPE_BACKGROUND, 0, 0);
            
            // rgba
            setSpriteColor(0x80, 0, 0, 0, 0);
            setSpriteColor(0x84, 0, 0, 0, 0);
            setSpriteColor(LANDSCAPE_BACKGROUND, 0, 0, 0, 0);
            
            updateSpriteRGBA(0x80, 0xFF, 0xFF, 0xFF, 0xFF, 8);
            updateSpriteRGBA(0x84, 0xFF, 0xFF, 0xFF, 0xFF, 8);
            updateSpriteRGBA(LANDSCAPE_BACKGROUND, 0xFF, 0xFF, 0xFF, 0xFF, 8);

            namingScreenContext.unk_4 = 10;
            namingScreenContext.flags &= ~(0x20 | 0x40);
            namingScreenContext.flags |= 0x40;

            func_8003DBE8(3, (void*)0x8030B800);
            func_8003F54C(3, 4.0f, 56.0f, 30.0f);
            func_8003F5D0(3, 6, 1);
            func_8003F630(3, 0, 2);
            func_8003F464(3, 0xE, 0xE, 0x802FF000, 0x8030A000);
            func_8003F360(3, 1, 1);
            setDialogueBoxSpriteIndices(3, 0xFF, 0xFF, 0xFF);
            initializeDialogueBox(3, 0xA, 0xD, 0x80000);
            func_8003E77C(3, 0, 0, 0, 0);
            func_8003EA1C(3, 0xFF, 0xFF, 0xFF, 0xFF, 8);
            func_8003DBE8(4, (void*)0x8030BC00);
            func_8003F54C(4, 4.0f, -10.0f, 30.0f);
            func_8003F5D0(4, 6, 1);
            func_8003F630(4, 0, 2);
            func_8003F464(4, 0xE, 0xE, 0x802FF000, 0x8030A000);
            func_8003F360(4, 1, 1);
            setDialogueBoxSpriteIndices(4, 0xFF, 0xFF, 0xFF);
            initializeDialogueBox(4, 0xA, temp2 + 0xE, 0x80000);
            func_8003E77C(4, 0, 0, 0, 0);
            func_8003EA1C(4, 0xFF, 0xFF, 0xFF, 0xFF, 8);
            
            namingScreenContext.flags &= ~(0x200);
            
            return;
            
        }

        if (namingScreenContext.flags & 0x400) {
            
            deactivateNamingScreenSprites();
            func_8003DD14(3);
            func_8003DD14(4);
            loadNameSelectionSprites();
            
            namingScreenContext.unk_4 = 12;
            namingScreenContext.flags &= ~(0x400);
            
            return;
            
        }

         if (namingScreenContext.flags & 0x800) {

            setGameVariableString(0, gPlayer.name, 6);
            deactivateNamingScreenSprites();
            func_8003DD14(3);
            func_8003DD14(4);
            setDialogueBoxSpriteIndices(0, 0, 0, 0);
            func_8003F54C(0, 24.0f, -64.0f, 352.0f);

             switch (namingScreenContext.unk_6) {
                 
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
                     
                    func_80059368();
                    func_8005C07C(8, 1);
                     
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

                        func_80059368();
                        func_8005C07C(8, 1);
                     
                    return;
                 
                 case 5:
                 case 6:
                 case 7:
                    setLevelAudio(gBaseMapIndex, gSeason, gHour);
                    func_80059368();
                    func_8005C07C(8, 1);
                    return;
                 
             }

        } else {
             
            if (namingScreenContext.flags & 0x1000) {
                namingScreenContext.flags &= ~(0x1000);
                namingScreenContext.flags |= 0x800;
                return;
            }
    
            if (namingScreenContext.unk_4 != 0xFFFF) {
                func_80043430(0, 0x3A, namingScreenContext.unk_4, 0);
                namingScreenContext.unk_4 = 0xFFFF;
                return;
            }
                 
            if (dialogues[0].struct5.flags & 4) {
                
                    switch (temp) {   
    
                        case 0:
                            func_800F03C4();
                            return;
    
                        case 1:
                            func_800F0F84();
                            return;
    
                        case 2:
                            
                            if (dialogues[0].struct5.unk_17 == 0) {
                                
                                namingScreenContext.flags |= 0x800;
                                
                                func_8003EA1C(3, 0, 0, 0, 0, 8);
                                func_8003EA1C(4, 0, 0, 0, 0, 8);
                                updateSpriteRGBA(0x80, 0, 0, 0, 0, 8);
                                updateSpriteRGBA(0x84, 0, 0, 0, 0, 8);
                                updateSpriteRGBA(LANDSCAPE_BACKGROUND, 0, 0, 0, 0, 8);
                                
                                gPlayerBirthdaySeason = namingScreenContext.unk_7 + 1;

                                return;
                                
                            }
    
                            namingScreenContext.flags |= 0x400;
                            
                            func_8003EA1C(3, 0, 0, 0, 0, 8);
                            func_8003EA1C(4, 0, 0, 0, 0, 8);
                            updateSpriteRGBA(0x80, 0, 0, 0, 0, 8);
                            updateSpriteRGBA(0x84, 0, 0, 0, 0, 8);
                            updateSpriteRGBA(LANDSCAPE_BACKGROUND, 0, 0, 0, 0, 8);
                            
                            return;
    
                        case 3:
    
                            if (dialogues[0].struct5.unk_17 == 0) {
                                
                                namingScreenContext.flags |= 0x800;

                                func_8003EA1C(3, 0, 0, 0, 0, 8);
                                
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
    
                                if (namingScreenContext.unk_6 != 1) {
                                    
                                    setSongWithDefaultSpeed(NAMING_SCREEN_THEME);
                                    
                                    namingScreenContext.flags &= ~(0x800);
                                    namingScreenContext.flags |= 0x1000;
    
                                }
                                
                            } else {
                                namingScreenContext.flags &= ~(0x20 | 0x40);
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

INCLUDE_ASM("asm/nonmatchings/game/namingScreen", loadNameSelectionSprites);

// need to add linker symbols
/*
void loadNameSelectionSprites(void) {

    s32 i = 0;

    dmaSprite(0x80, &D_D47F00, &D_D49B80, &D_D49B80_2, &D_D49BA0, NULL, NULL, (void* )0x8023B400, NULL, (void* )0x8023CC00, (void* )0x8023CE00, (void* )0x8023D200, NULL, NULL, 0);
    dmaSprite(0x81, &D_D1DE00, &D_D2D600, &D_D2D600_2, &D_D2D620, NULL, NULL, (void* )0x80253B00, NULL, (void* )0x80263B00, (void* )0x80263C00, (void* )0x80263D00, NULL, NULL, 0);
    dmaSprite(0x82, &D_D1DE00, &D_D2D600, &D_D2D600_2, &D_D2D620, NULL, NULL, (void* )0x80253B00, NULL, (void* )0x80263B00, (void* )0x80263C00, (void* )0x80263D00, NULL, NULL, 0);
    dmaSprite(0x83, &D_D1DE00, &D_D2D600, &D_D2D600_2, &D_D2D620, NULL, NULL, (void* )0x80253B00, NULL, (void* )0x80263B00, (void* )0x80263C00, (void* )0x80263D00, NULL, NULL, 0);
    dmaSprite(0x84, &D_D1DE00, &D_D2D600, &D_D2D600_2, &D_D2D620, NULL, NULL, (void* )0x80253B00, NULL, (void* )0x80263B00, (void* )0x80263C00, (void* )0x80263D00, NULL, NULL, 0);
    dmaSprite(0x85, &D_D1DE00, &D_D2D600, &D_D2D600_2, &D_D2D620, NULL, NULL, (void* )0x80253B00, NULL, (void* )0x80263B00, (void* )0x80263C00, (void* )0x80263D00, NULL, NULL, 0);
    dmaSprite(0x86, &D_D1DE00, &D_D2D600, &D_D2D600_2, &D_D2D620, NULL, NULL, (void* )0x80253B00, NULL, (void* )0x80263B00, (void* )0x80263C00, (void* )0x80263D00, NULL, NULL, 0);
    dmaSprite(0x87, &D_D1DE00, &D_D2D600, &D_D2D600_2, &D_D2D620, NULL, NULL, (void* )0x80253B00, NULL, (void* )0x80263B00, (void* )0x80263C00, (void* )0x80263D00, NULL, NULL, 0);
    dmaSprite(0x88, &D_D1DE00, &D_D2D600, &D_D2D600_2, &D_D2D620, NULL, NULL, (void* )0x80253B00, NULL, (void* )0x80263B00, (void* )0x80263C00, (void* )0x80263D00, NULL, NULL, 0);
    dmaSprite(0x8A, &D_D1DE00, &D_D2D600, &D_D2D600_2, &D_D2D620, NULL, NULL, (void* )0x80253B00, NULL, (void* )0x80263B00, (void* )0x80263C00, (void* )0x80263D00, NULL, NULL, 0);
    dmaSprite(0x8B, &D_D1DE00, &D_D2D600, &D_D2D600_2, &D_D2D620, NULL, NULL, (void* )0x80253B00, NULL, (void* )0x80263B00, (void* )0x80263C00, (void* )0x80263D00, NULL, NULL, 0);
    dmaSprite(0x8C, &D_D1DE00, &D_D2D600, &D_D2D600_2, &D_D2D620, NULL, NULL, (void* )0x80253B00, NULL, (void* )0x80263B00, (void* )0x80263C00, (void* )0x80263D00, NULL, NULL, 0);
    dmaSprite(0x8D, &D_D1DE00, &D_D2D600, &D_D2D600_2, &D_D2D620, NULL, NULL, (void* )0x80253B00, NULL, (void* )0x80263B00, (void* )0x80263C00, (void* )0x80263D00, NULL, NULL, 0);
    dmaSprite(0x8E, &D_D1DE00, &D_D2D600, &D_D2D600_2, &D_D2D620, NULL, NULL, (void* )0x80253B00, NULL, (void* )0x80263B00, (void* )0x80263C00, (void* )0x80263D00, NULL, NULL, 0);
    dmaSprite(0x8F, &D_D1DE00, &D_D2D600, &D_D2D600_2, &D_D2D620, NULL, NULL, (void* )0x80253B00, NULL, (void* )0x80263B00, (void* )0x80263C00, (void* )0x80263D00, NULL, NULL, 0);
    dmaSprite(LANDSCAPE_BACKGROUND, &D_CC9390, &D_CCB980, &D_CCB980_2, &D_CCB9A0, NULL, NULL, (void* )0x8026AB00, NULL, (void* )0x8026DB00, (void* )0x8026DC00, (void* )0x8026DD00, NULL, NULL, 0);
    dmaSprite(0x91, &D_D47F00, &D_D49B80, &D_D49B80_2, &D_D49BA0, NULL, NULL, (void* )0x8023B400, NULL, (void* )0x8023CC00, (void* )0x8023CE00, (void* )0x8023D200, NULL, NULL, 0);
    
    setBilinearFiltering(0x8F, TRUE);
    setBilinearFiltering(LANDSCAPE_BACKGROUND, TRUE);

    setSpriteScale(LANDSCAPE_BACKGROUND, 2.0f, 2.0f, 1.0f);
    
    setSpriteShrinkFactor(0x80, 0.0f, 0.0f, -1.0f);
    setSpriteShrinkFactor(0x81, 0.0f, -1.0f, 10.0f);
    setSpriteShrinkFactor(0x82, 0.0f, 0.0f, -2.0f);
    setSpriteShrinkFactor(0x8F, 0.0f, 0.0f, 1.0f);
    setSpriteShrinkFactor(0x83, 8.0f, 72.0f, 20.0f);
    setSpriteShrinkFactor(0x84, 24.0f, 72.0f, 20.0f);
    setSpriteShrinkFactor(0x85, 40.0f, 72.0f, 20.0f);
    setSpriteShrinkFactor(0x86, 56.0f, 72.0f, 20.0f);
    setSpriteShrinkFactor(0x87, 72.0f, 72.0f, 20.0f);
    setSpriteShrinkFactor(0x88, 88.0f, 72.0f, 20.0f);
    setSpriteShrinkFactor(0x8A, 24.0f, 68.0f, 20.0f);
    setSpriteShrinkFactor(0x8B, 40.0f, 68.0f, 20.0f);
    setSpriteShrinkFactor(0x8C, 56.0f, 68.0f, 20.0f);
    setSpriteShrinkFactor(0x8D, 72.0f, 68.0f, 20.0f);
    setSpriteShrinkFactor(0x8E, 88.0f, 68.0f, 20.0f);
    setSpriteShrinkFactor(0x91, 0.0f, 0.0f, 20.0f);
    
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

    namingScreenContext.unk_19 = 0;
    namingScreenContext.unk_18 = 0;
    namingScreenContext.unk_1C = 0;
    namingScreenContext.flags = 2;

    namingScreenContext.coordinates.x = -126.0f;
    namingScreenContext.coordinates.y = 16.0f;
    namingScreenContext.coordinates.z = -116.0f;
    namingScreenContext.coordinates.w = 26.0f;

    for (i = 0; i < 6; i++) {
        
        if (namingScreenContext.name[i] != 0xFF) {
            
            setSpriteColor(i + 0x83, 0xFF, 0xFF, 0xFF, 0xFF);
            resetAnimationState(i + 0x83);

            if (i != 5) {
               
                setSpriteColor(i + 0x8A, 0xFF, 0xFF, 0xFF, 0xFF);
                resetAnimationState(i + 0x8A);
                
            } 

            
        } else {
            break;
        }
    } 
      
    namingScreenContext.flags |= i << 2;
    
    setGameVariableString(0, namingScreenContext.name, 6);
    
    func_8003DBE8(3, (void*)0x8030B800);
    func_8003F54C(3, 48.0f, 72.0f, 30.0f);
    
    func_8003F5D0(3, 6, 1);
    func_8003F630(3, 2, 2);
    func_8003F464(3, 0xE, 0xE, 0x802FF000, 0x8030A000);
    func_8003F360(3, 1, 1);
    setDialogueBoxSpriteIndices(3, 0xFF, 0xFF, 0xFF);
    initializeDialogueBox(3, 0xA, 0xD, 0x80000);
    func_8003E77C(3, 0, 0, 0, 0);
    func_8003EA1C(3, 0xFF, 0xFF, 0xFF, 0xFF, 8);
    
}
*/

INCLUDE_ASM("asm/nonmatchings/game/namingScreen", func_800EF578);

//INCLUDE_ASM("asm/nonmatchings/game/namingScreen", func_800EFADC);

void func_800EFADC(void) {

    namingScreenContext.coordinates.x -= 16.0f;
    namingScreenContext.coordinates.z -= 16.0f;

    if (namingScreenContext.unk_18 != 0) {
        if (!(namingScreenContext.unk_18 % 5)) {
            namingScreenContext.coordinates.x -= 3.0f;
            namingScreenContext.coordinates.z -= 3.0f;
        }
    } 

    namingScreenContext.unk_18 -= 1;
    
    if ((namingScreenContext.unk_18 << 0x18) < 0) {

        namingScreenContext.unk_18 = 14;
        namingScreenContext.coordinates.x = 104.0f;
        namingScreenContext.coordinates.z = 114.0f;
        
    }
    
    func_800F00D8();
}

//INCLUDE_ASM("asm/nonmatchings/game/namingScreen", func_800EFBEC);

void func_800EFBEC(void) {
        
    namingScreenContext.coordinates.x += 16.0f;
    namingScreenContext.coordinates.z += 16.0f;

    namingScreenContext.unk_18++;

    if (namingScreenContext.unk_18 >= 15) {
        namingScreenContext.unk_18 = 0;
    }

    if (namingScreenContext.unk_18 != 0) {
        
        if ((namingScreenContext.unk_18 % 5) == 0) {
            namingScreenContext.coordinates.x += 3.0f;
            namingScreenContext.coordinates.z += 3.0f;
        }

    } else {
        // FIXME: probably different control flow
        do {} while (0);
        namingScreenContext.coordinates.x = -126.0f;
        namingScreenContext.coordinates.z = -116.0f;

    }

    func_800F00D8();

}


//INCLUDE_ASM("asm/nonmatchings/game/namingScreen", func_800EFCF8);

void func_800EFCF8(void) {

    namingScreenContext.coordinates.y += 16.0f;
    namingScreenContext.coordinates.w += 16.0f;
    
    namingScreenContext.unk_19 -= 1;
    
    if ((namingScreenContext.unk_19 << 0x18) < 0) {
        
        namingScreenContext.unk_19 = 5;

        namingScreenContext.coordinates.y = -64.0f;
        namingScreenContext.coordinates.w = -54.0f;

        func_800F0320();
        
    }

    if (namingScreenContext.unk_18 >= 10 && namingScreenContext.unk_19 == 4) {

        namingScreenContext.unk_18 = namingScreenContext.unk_1A;

        namingScreenContext.coordinates.y = -48.0f;
        namingScreenContext.coordinates.w = -38.0f;

        namingScreenContext.coordinates.x = (namingScreenContext.unk_1A * 16.0f) + -126.0f + 6.0f;
        namingScreenContext.coordinates.z = (namingScreenContext.unk_1A * 16.0f) + -116.0f + 6.0f;
        
        resetAnimationState(0x82);
        startSpriteAnimation(0x82, 2, 0);

        resetAnimationState(0x91);
        startSpriteAnimation(0x91, 2, 0);
        
        setSpritePaletteIndex(0x91, 3);
        
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/namingScreen", func_800EFE84);

void func_800EFE84(void) {

    namingScreenContext.coordinates.y -= 16.0f;
    namingScreenContext.coordinates.w -= 16.0f;
    
    namingScreenContext.unk_19 += 1;
    
    if (namingScreenContext.unk_19 >= 6) {
        
        namingScreenContext.unk_19 = 0;

        namingScreenContext.coordinates.y = 16.0f;
        namingScreenContext.coordinates.w = 26.0f;

        if (namingScreenContext.unk_18 >= 10) {

            namingScreenContext.unk_18 = namingScreenContext.unk_1A;

            namingScreenContext.coordinates.x = (namingScreenContext.unk_18 * 16.0f);
            namingScreenContext.coordinates.x += -126.0f;
            namingScreenContext.coordinates.x += 6.0f;
            
            namingScreenContext.coordinates.z = (namingScreenContext.unk_18 * 16.0f);
            namingScreenContext.coordinates.z += -116.0f;
            namingScreenContext.coordinates.z += 6.0f;

            resetAnimationState(0x82);
            startSpriteAnimation(0x82, 2, 0);
            resetAnimationState(0x91);
            startSpriteAnimation(0x91, 2, 0);
            setSpritePaletteIndex(0x91, 3);
            
        }
        
    }

    func_800F0320();
    
}

// unused or inline
INCLUDE_ASM("asm/nonmatchings/game/namingScreen", func_800EFFDC);

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

//INCLUDE_ASM("asm/nonmatchings/game/namingScreen", func_800F00D8);

void func_800F00D8(void) {

    s8 temp;

    if (namingScreenContext.unk_19 == 5) {
        
        temp = namingScreenContext.unk_18 - 9;
        
        switch (temp) {

            case 0:
            case 2:
                namingScreenContext.unk_1A = 10;
                namingScreenContext.unk_18 = 9;
                
                resetAnimationState(0x91);
                startSpriteAnimation(0x91, 2, 0);
                setSpritePaletteIndex(0x91, 3);

                namingScreenContext.coordinates.x = 21.0f;
                namingScreenContext.coordinates.z = 31.0f;
                namingScreenContext.coordinates.y = -64.0f;
                namingScreenContext.coordinates.w = -54.0f;
                
                resetAnimationState(0x82);
                startSpriteAnimation(0x82, 2, 0);
                break;

            case 1:

                namingScreenContext.unk_18 = 12;

            case 3:

                namingScreenContext.unk_1A = 13;

                namingScreenContext.coordinates.x = 100.0f;
                namingScreenContext.coordinates.y = -95.0f;
                
                resetAnimationState(0x82);
                resetAnimationState(0x91);
                startSpriteAnimation(0x91, 2, 1);
                setSpritePaletteIndex(0x91, 4); 

                break;

            case 4:
                
                namingScreenContext.unk_18 = 0;

                namingScreenContext.coordinates.x = -126.0f;
                namingScreenContext.coordinates.z = -116.0f;
                namingScreenContext.coordinates.y = -64.0f;
                namingScreenContext.coordinates.w = -54.0f;

                resetAnimationState(0x82);
                startSpriteAnimation(0x82, 2, 0);
                resetAnimationState(0x91);
                startSpriteAnimation(0x91, 2, 0);
                setSpritePaletteIndex(0x91, 3);
                
                break;
            
            case 5:
                
                namingScreenContext.unk_1A = 13;
                namingScreenContext.unk_18 = 12;

                namingScreenContext.coordinates.x = 100.0f;
                namingScreenContext.coordinates.y = -95.0f;

                resetAnimationState(0x82);
                resetAnimationState(0x91);
                startSpriteAnimation(0x91, 2, 1);
                setSpritePaletteIndex(0x91, 4);
            
                break;
            
        }
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/namingScreen", func_800F0320);

void func_800F0320(void) {
    
    if ((namingScreenContext.unk_19 == 5) && (namingScreenContext.unk_18 >= 10)) {
        
        namingScreenContext.unk_1A = namingScreenContext.unk_18;
        
        namingScreenContext.unk_18 = 12;
        namingScreenContext.coordinates.x = 100.0f;
        namingScreenContext.coordinates.y = -95.0f;
        
        resetAnimationState(0x82);
        resetAnimationState(0x91);
        startSpriteAnimation(0x91, 2, 1);
        setSpritePaletteIndex(0x91, 4);
        
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/namingScreen", func_800F03C4);

// FIXME: have to fix D_8016FBEE usage

void func_800F03C4(void) {
        
    bool set = FALSE;
    s32 index;

    if (namingScreenContext.flags & 0x80) {
        resetAnimationState(0x91);
        startSpriteAnimation(0x91, 2, 1);
        setSpritePaletteIndex(0x91, 4);
        namingScreenContext.flags &= ~(0x80);
    }

    if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_LEFT)) {
        set = TRUE;
        func_800EFADC();
        setSfx(3);
        setSfxVolume(3, SFX_VOLUME);
    }

    if (!set) {
        if (checkButtonRepeat(CONTROLLER_1, 0x400000)) {
            set = TRUE;
            func_800EFBEC();
            setSfx(3);
            setSfxVolume(3, SFX_VOLUME);
        }
    }

    if (!set) {
        if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_UP)) {
            set = TRUE;
            func_800EFE84();
            setSfx(3);
            setSfxVolume(3, SFX_VOLUME);
        }
    }
        
    if (!set) {
        
        if (checkButtonRepeat(CONTROLLER_1, 0x100000)) {
            func_800EFCF8();
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
            
            if (func_800EF578()) { 
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
                
                startSpriteAnimation(index + 0x83, 2, 0);
                
                if (index != 5) {
                    startSpriteAnimation(index + 0x8A, 2, 1);
                }
                
            }
                
        }        
    } 
        
    if (!set) { 
        
        if (checkButtonPressed(CONTROLLER_1, BUTTON_START)) {
            
            namingScreenContext.unk_1A = 14;
            namingScreenContext.unk_18 = 12;
            namingScreenContext.unk_19 = 5;
                    
            namingScreenContext.coordinates.x = 100.0f;
            namingScreenContext.coordinates.y = -95.0f;
            
            resetAnimationState(0x82);
            resetAnimationState(0x91);
            startSpriteAnimation(0x91, 2, 1);
            setSpritePaletteIndex(0x91, 4);
            
            setSfx(3);
            setSfxVolume(3, SFX_VOLUME);
        
        }

    }

    setSpriteShrinkFactor(0x80, namingScreenContext.coordinates.x, namingScreenContext.coordinates.y, 20.0f);
    setSpriteShrinkFactor(0x82, namingScreenContext.coordinates.z, namingScreenContext.coordinates.w, 5.0f);

}

INCLUDE_ASM("asm/nonmatchings/game/namingScreen", loadSeasonSelectionSprites);

// need to add linker symbols
/*
void loadSeasonSelectionSprites(void) {
    
dmaSprite(0x80, &D_D47F00, &D_D49B80, &D_D49B80_2, &D_D49BA0, 0, 0, 0x8023B400, 0, 0x8023CC00, 0x8023CE00, 0x8023D200, 0, 0, 0);
dmaSprite(0x83, &D_CDE2E0, &D_CED610, &D_CED610_2, &D_CED630, 0, 0, 0x80267300, 0, 0x8026A300, 0x8026A700, 0x8026AA00, 0, 0, 0);
dmaSprite(0x84, &D_CDE2E0, &D_CED610, &D_CED610_2, &D_CED630, 0, 0, 0x80267300, 0, 0x8026A300, 0x8026A700, 0x8026AA00, 0, 0, 0);
    dmaSprite(0x85, &D_CDE2E0, &D_CED610, &D_CED610_2, &D_CED630, 0, 0, 0x80267300, 0, 0x8026A300, 0x8026A700, 0x8026AA00, 0, 0, 0);
    dmaSprite(0x86, &D_CDE2E0, &D_CED610, &D_CED610_2, &D_CED630, 0, 0, 0x80267300, 0, 0x8026A300, 0x8026A700, 0x8026AA00, 0, 0, 0);
    dmaSprite(0x89, &D_CDAEB0, &D_CDE2C0, &D_CDE2C0_2, &D_CDE2E0_2, 0, 0, 0x80263E00, 0, 0x80266E00, 0x80267100, 0x80267200, 0, 0, 0);
    dmaSprite(0x8A, &D_CDAEB0, &D_CDE2C0, &D_CDE2C0_2, &D_CDE2E0_2, 0, 0, 0x80263E00, 0, 0x80266E00, 0x80267100, 0x80267200, 0, 0, 0);
    dmaSprite(0x8B, &D_CDAEB0, &D_CDE2C0, &D_CDE2C0_2, &D_CDE2E0_2, 0, 0, 0x80263E00, 0, 0x80266E00, 0x80267100, 0x80267200, 0, 0, 0);
    dmaSprite(0x8C, &D_CDAEB0, &D_CDE2C0, &D_CDE2C0_2, &D_CDE2E0_2, 0, 0, 0x80263E00, 0, 0x80266E00, 0x80267100, 0x80267200, 0, 0, 0);
    dmaSprite(LANDSCAPE_BACKGROUND, &D_CC9390, &D_CCB980, &D_CCB980_2, &D_CCB9A0, 0, 0, 0x8026AB00, 0, 0x8026DB00, 0x8026DC00, 0x8026DD00, 0, 0, 0);
    
    setSpriteShrinkFactor(0x80, 0, 0, -1.0f);
    
    setSpriteShrinkFactor(0x83, -48.0f, 0, 10.0f);
    setSpriteShrinkFactor(0x84, 96.0f, 0, 10.0f);
    setSpriteShrinkFactor(0x85, -48.0f, -48.0f, 10.0f);
    setSpriteShrinkFactor(0x86, 96.0f, -48.0f, 10.0f);
    
    setSpriteShrinkFactor(0x89, 0, 0, 5.0f);
    setSpriteShrinkFactor(0x8A, 0, 0, 5.0f);
    setSpriteShrinkFactor(0x8B, 0, 0, 5.0f);
    setSpriteShrinkFactor(0x8C, 0, 0, 5.0f);
    
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
    
    namingScreenContext.unk_7 = 0;
    namingScreenContext.unk_4 = 9;
    namingScreenContext.coordinates.x = -128.0f;
    namingScreenContext.coordinates.y = 52.0f;
    namingScreenContext.flags &= ~(0x20 | -0x40);
    namingScreenContext.flags |= 0x20;
    
}
*/

//INCLUDE_ASM("asm/nonmatchings/game/namingScreen", func_800F0F84);

void func_800F0F84(void) {

    bool set = FALSE;

    if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_UP) || checkButtonRepeat(CONTROLLER_1, 0x100000)) {
        
        set = TRUE;
        
        func_800F121C();
        
        setSfx(3);
        setSfxVolume(3, SFX_VOLUME);

    }

    if (!set) {

        if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_LEFT) || checkButtonRepeat(CONTROLLER_1, 0x400000)) {
            
            set = TRUE;
            
            func_800F12C4();

            setSfx(3);
            setSfxVolume(3, SFX_VOLUME);
        }
        
    }

    if (!set) {

        if (checkButtonRepeat(CONTROLLER_1, 0x200000) || checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_RIGHT) || checkButtonRepeat(CONTROLLER_1, 0x800000) || checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_DOWN)) {
            
            set = TRUE;
            
            func_800F121C();
            func_800F12C4();

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

            namingScreenContext.flags |= 0x200;

        }
    }
    
    setSpriteShrinkFactor(0x80, namingScreenContext.coordinates.x, namingScreenContext.coordinates.y, 20.0f);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/namingScreen", func_800F121C);

// birthday season selection
void func_800F121C(void) {

    u16 temp1;
    u16 temp2;

    temp1 = namingScreenContext.unk_7;
    
    setSpritePaletteIndex(temp1 + 0x83, 0);
    
    if (namingScreenContext.coordinates.y == 52.0f) {
        namingScreenContext.coordinates.y -= 48.0f;
        temp2 = temp1 + 2;
    } else {
        temp2 = temp1 - 2;
        namingScreenContext.coordinates.y += 48.0f;
    }
    
    setSpritePaletteIndex(temp2 + 0x83, temp2 + 1);
    
    namingScreenContext.unk_7 = temp2;

}

//INCLUDE_ASM("asm/nonmatchings/game/namingScreen", func_800F12C4);

// birthday season selection
void func_800F12C4(void) {

    u16 temp1;
    u16 temp2;

    temp1 = namingScreenContext.unk_7;
    
    setSpritePaletteIndex(temp1 + 0x83, 0);
    
    if (namingScreenContext.coordinates.x == -128.0f) {
        namingScreenContext.coordinates.x += 144.0f;
        temp2 = temp1 + 1;
    } else {
        temp2 = temp1 - 1;
        namingScreenContext.coordinates.x -= 144.0f;
    }
 
    setSpritePaletteIndex(temp2 + 0x83, temp2+1);
    namingScreenContext.unk_7 = temp2;

}