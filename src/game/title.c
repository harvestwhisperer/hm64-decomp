#include "common.h"

#include "game/title.h"

#include "ld_symbols.h"

#include "system/controller.h"  
#include "system/dialogue.h"
#include "system/mathUtils.h"
#include "system/message.h"
#include "system/globalSprites.h"
                                                        
#include "game/gameAudio.h"
#include "game/transition.h"     
#include "game/loadGameScreen.h"      
#include "mainLoop.h" 
#include "game/setCutscenes.h"         
#include "game/spriteInfo.h"            
     
// forward declarations
void func_800E0ED4();                                  
void updateGrassAnimation();                                  
void func_800E10E8();                                  
void loadDogTitleSprite(void);

// bss
static TitleContext titleScreenContext;

//INCLUDE_ASM("asm/nonmatchings/game/title", initializeTitleScreen);

void initializeTitleScreen(u8 arg0) {
    
    titleScreenContext.rowHighlighted = 0;
    titleScreenContext.dogAnimationIndex = 0;
    titleScreenContext.unk_4 = 0;
    titleScreenContext.dogIdleCounter = 0;
    titleScreenContext.centerCoordinate = -160.0f;

    dmaSprite(HARVEST_MOON_64_LOGO, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesAssetsIndexSegmentRomStart, &_titleSpritesAssetsIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURE_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURE_VADDR_END, (void*)TITLE_SCREEN_ASSETS_INDEX_VADDR_START, (void*)TITLE_SCREEN_ASSETS_INDEX_VADDR_END, NULL, NULL, 0);
    setSpriteShrinkFactor(HARVEST_MOON_64_LOGO, 0.0f, 64.0f, 64.0f);
    setSpriteScale(HARVEST_MOON_64_LOGO, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(HARVEST_MOON_64_LOGO, 0.0f, 0.0f, 0.0f);
    setSpriteRenderingLayer(HARVEST_MOON_64_LOGO, (1 | 2));
    setSpriteColor(HARVEST_MOON_64_LOGO, 0xFF, 0xFF, 0xFF, 0xFF);
    setBilinearFiltering(HARVEST_MOON_64_LOGO, TRUE);
    startSpriteAnimation(HARVEST_MOON_64_LOGO, 0, 0xFE);
     
    dmaSprite(MARVCOME_VICTOR_COPYRIGHT_INFO, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesAssetsIndexSegmentRomStart, &_titleSpritesAssetsIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURE_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURE_VADDR_END, (void*)TITLE_SCREEN_ASSETS_INDEX_VADDR_START, (void*)TITLE_SCREEN_ASSETS_INDEX_VADDR_END, NULL, NULL, 0);
    setSpriteShrinkFactor(MARVCOME_VICTOR_COPYRIGHT_INFO, 0.0f, -96.0f, 64.0f);
    setSpriteScale(MARVCOME_VICTOR_COPYRIGHT_INFO, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(MARVCOME_VICTOR_COPYRIGHT_INFO, 0.0f, 0.0f, 0.0f);
    setSpriteRenderingLayer(MARVCOME_VICTOR_COPYRIGHT_INFO, (1 | 2));
    setSpriteColor(MARVCOME_VICTOR_COPYRIGHT_INFO, 0xFF, 0xFF, 0xFF, 0xFF);
    startSpriteAnimation(MARVCOME_VICTOR_COPYRIGHT_INFO, 1, 0xFE);
    
    dmaSprite(PUSH_THE_START_SIGN_1, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesAssetsIndexSegmentRomStart, &_titleSpritesAssetsIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURE_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURE_VADDR_END, (void*)TITLE_SCREEN_ASSETS_INDEX_VADDR_START, (void*)TITLE_SCREEN_ASSETS_INDEX_VADDR_END, NULL, NULL, 0);
    setSpriteShrinkFactor(PUSH_THE_START_SIGN_1, titleScreenContext.centerCoordinate + 160.0f, -16.0f, 48.0f);
    setSpriteScale(PUSH_THE_START_SIGN_1, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(PUSH_THE_START_SIGN_1, 0.0f, 0.0f, 0.0f);
    setSpriteRenderingLayer(PUSH_THE_START_SIGN_1, (1 | 2));
    setSpriteColor(PUSH_THE_START_SIGN_1, 0xFF, 0xFF, 0xFF, 0xFF);
    setBilinearFiltering(PUSH_THE_START_SIGN_1, TRUE);
    startSpriteAnimation(PUSH_THE_START_SIGN_1, 3, 0xFE);
    
    dmaSprite(HOW_TO_PLAY_SIGN, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesAssetsIndexSegmentRomStart, &_titleSpritesAssetsIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURE_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURE_VADDR_END, (void*)TITLE_SCREEN_ASSETS_INDEX_VADDR_START, (void*)TITLE_SCREEN_ASSETS_INDEX_VADDR_END, NULL, NULL, 0);
    setSpriteShrinkFactor(HOW_TO_PLAY_SIGN, titleScreenContext.centerCoordinate - 160.0f, -12.0f, 48.0f);
    setSpriteScale(HOW_TO_PLAY_SIGN, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(HOW_TO_PLAY_SIGN, 0.0f, 0.0f, 0.0f);
    setSpriteRenderingLayer(HOW_TO_PLAY_SIGN, (1 | 2));
    setSpriteColor(HOW_TO_PLAY_SIGN, 0xFF, 0xFF, 0xFF, 0xFF);
    setBilinearFiltering(HOW_TO_PLAY_SIGN, TRUE);
    startSpriteAnimation(HOW_TO_PLAY_SIGN, 5, 0xFE);

    dmaSprite(DIRT_ROAD, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesAssetsIndexSegmentRomStart, &_titleSpritesAssetsIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURE_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURE_VADDR_END, (void*)TITLE_SCREEN_ASSETS_INDEX_VADDR_START, (void*)TITLE_SCREEN_ASSETS_INDEX_VADDR_END, NULL, NULL, 0);
    setSpriteShrinkFactor(DIRT_ROAD, titleScreenContext.centerCoordinate - 160.0f, -44.0f, 48.0f);
    setSpriteScale(DIRT_ROAD, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(DIRT_ROAD, 0.0f, 0.0f, 0.0f);
    setSpriteRenderingLayer(DIRT_ROAD, (1 | 2));
    setSpriteColor(DIRT_ROAD, 0xFF, 0xFF, 0xFF, 0xFF);
    setBilinearFiltering(DIRT_ROAD, TRUE);
    startSpriteAnimation(DIRT_ROAD, 7, 0xFE);

    dmaSprite(SIGN_POST_1, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesAssetsIndexSegmentRomStart, &_titleSpritesAssetsIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURE_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURE_VADDR_END, (void*)TITLE_SCREEN_ASSETS_INDEX_VADDR_START, (void*)TITLE_SCREEN_ASSETS_INDEX_VADDR_END, NULL, NULL, 0);
    setSpriteShrinkFactor(SIGN_POST_1, titleScreenContext.centerCoordinate + 160.0f, -32.0f, 40.0f);
    setSpriteScale(SIGN_POST_1, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(SIGN_POST_1, 0.0f, 0.0f, 0.0f);
    setSpriteRenderingLayer(SIGN_POST_1, (1 | 2));
    setSpriteColor(SIGN_POST_1, 0xFF, 0xFF, 0xFF, 0xFF);
    setBilinearFiltering(SIGN_POST_1, TRUE);
    startSpriteAnimation(SIGN_POST_1, 2, 0xFE);

    dmaSprite(SIGN_POST_2, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesAssetsIndexSegmentRomStart, &_titleSpritesAssetsIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURE_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURE_VADDR_END, (void*)TITLE_SCREEN_ASSETS_INDEX_VADDR_START, (void*)TITLE_SCREEN_ASSETS_INDEX_VADDR_END, NULL, NULL, 0);
    setSpriteShrinkFactor(SIGN_POST_2, titleScreenContext.centerCoordinate - 192.0f, -32.0f, 40.0f);
    setSpriteScale(SIGN_POST_2, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(SIGN_POST_2, 0.0f, 0.0f, 0.0f);
    setSpriteRenderingLayer(SIGN_POST_2, (1 | 2));
    setSpriteColor(SIGN_POST_2, 0xFF, 0xFF, 0xFF, 0xFF);
    setBilinearFiltering(SIGN_POST_2, TRUE);
    startSpriteAnimation(SIGN_POST_2, 2, 0xFE);

    dmaSprite(SIGN_POST_3, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesAssetsIndexSegmentRomStart, &_titleSpritesAssetsIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURE_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURE_VADDR_END, (void*)TITLE_SCREEN_ASSETS_INDEX_VADDR_START, (void*)TITLE_SCREEN_ASSETS_INDEX_VADDR_END, NULL, NULL, 0);
    setSpriteShrinkFactor(SIGN_POST_3, titleScreenContext.centerCoordinate - 128.0f, -32.0f, 40.0f);
    setSpriteScale(SIGN_POST_3, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(SIGN_POST_3, 0.0f, 0.0f, 0.0f);
    setSpriteRenderingLayer(SIGN_POST_3, (1 | 2));
    setSpriteColor(SIGN_POST_3, 0xFF, 0xFF, 0xFF, 0xFF);
    setBilinearFiltering(SIGN_POST_3, TRUE);
    startSpriteAnimation(SIGN_POST_3, 2, 0xFE);

    dmaSprite(CLOUD_1_1, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesAssetsIndexSegmentRomStart, &_titleSpritesAssetsIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURE_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURE_VADDR_END, (void*)TITLE_SCREEN_ASSETS_INDEX_VADDR_START, (void*)TITLE_SCREEN_ASSETS_INDEX_VADDR_END, NULL, NULL, 0);
    setSpriteShrinkFactor(CLOUD_1_1, titleScreenContext.centerCoordinate + 160.0f, 0.0f, 56.0f);
    setSpriteScale(CLOUD_1_1, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(CLOUD_1_1, 0.0f, 0.0f, 0.0f);
    setSpriteRenderingLayer(CLOUD_1_1, (1 | 2));
    setSpriteColor(CLOUD_1_1, 0xFF, 0xFF, 0xFF, 0xFF);
    setBilinearFiltering(CLOUD_1_1, TRUE);
    startSpriteAnimation(CLOUD_1_1, 9, 0xFE);

    dmaSprite(CLOUD_1_2, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesAssetsIndexSegmentRomStart, &_titleSpritesAssetsIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURE_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURE_VADDR_END, (void*)TITLE_SCREEN_ASSETS_INDEX_VADDR_START, (void*)TITLE_SCREEN_ASSETS_INDEX_VADDR_END, NULL, NULL, 0);
    setSpriteShrinkFactor(CLOUD_1_2, titleScreenContext.centerCoordinate - 160.0f, 0.0f, 56.0f);
    setSpriteScale(CLOUD_1_2, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(CLOUD_1_2, 0.0f, 0.0f, 0.0f);
    setSpriteRenderingLayer(CLOUD_1_2, (1 | 2));
    setSpriteColor(CLOUD_1_2, 0xFF, 0xFF, 0xFF, 0xFF);
    setBilinearFiltering(CLOUD_1_2, TRUE);
    startSpriteAnimation(CLOUD_1_2, 9, 0xFE);
    
    dmaSprite(CLOUD_2_1, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesAssetsIndexSegmentRomStart, &_titleSpritesAssetsIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURE_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURE_VADDR_END, (void*)TITLE_SCREEN_ASSETS_INDEX_VADDR_START, (void*)TITLE_SCREEN_ASSETS_INDEX_VADDR_END, NULL, NULL, 0);
    setSpriteShrinkFactor(CLOUD_2_1, titleScreenContext.centerCoordinate + 160.0f, 64.0f, 64.0f);
    setSpriteScale(CLOUD_2_1, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(CLOUD_2_1, 0.0f, 0.0f, 0.0f);
    setSpriteRenderingLayer(CLOUD_2_1, (1 | 2));
    setSpriteColor(CLOUD_2_1, 0xFF, 0xFF, 0xFF, 0xFF);
    setBilinearFiltering(CLOUD_2_1, TRUE);
    startSpriteAnimation(CLOUD_2_1, 0x10, 0xFE);
    
    dmaSprite(CLOUD_3_1, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesAssetsIndexSegmentRomStart, &_titleSpritesAssetsIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURE_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURE_VADDR_END, (void*)TITLE_SCREEN_ASSETS_INDEX_VADDR_START, (void*)TITLE_SCREEN_ASSETS_INDEX_VADDR_END, NULL, NULL, 0);
    setSpriteShrinkFactor(CLOUD_3_1, titleScreenContext.centerCoordinate + 32.0f, 96.0f, 64.0f);
    setSpriteScale(CLOUD_3_1, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(CLOUD_3_1, 0.0f, 0.0f, 0.0f);
    setSpriteRenderingLayer(CLOUD_3_1, (1 | 2));
    setSpriteColor(CLOUD_3_1, 0xFF, 0xFF, 0xFF, 0xFF);
    setBilinearFiltering(CLOUD_3_1, TRUE);
    startSpriteAnimation(CLOUD_3_1, 0x11, 0xFE);
    
    dmaSprite(LICENSED_BY_NINTENDO_1, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesAssetsIndexSegmentRomStart, &_titleSpritesAssetsIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURE_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURE_VADDR_END, (void*)TITLE_SCREEN_ASSETS_INDEX_VADDR_START, (void*)TITLE_SCREEN_ASSETS_INDEX_VADDR_END, NULL, NULL, 0);
    setSpriteShrinkFactor(LICENSED_BY_NINTENDO_1, titleScreenContext.centerCoordinate + 224.0f, 80.0f, 64.0f);
    setSpriteScale(LICENSED_BY_NINTENDO_1, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(LICENSED_BY_NINTENDO_1, 0.0f, 0.0f, 0.0f);
    setSpriteRenderingLayer(LICENSED_BY_NINTENDO_1, (1 | 2));
    setSpriteColor(LICENSED_BY_NINTENDO_1, 0xFF, 0xFF, 0xFF, 0xFF);
    setBilinearFiltering(LICENSED_BY_NINTENDO_1, TRUE);
    startSpriteAnimation(LICENSED_BY_NINTENDO_1, 0x12, 0xFE);
    
    dmaSprite(LICENSED_BY_NINTENDO_2, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesAssetsIndexSegmentRomStart, &_titleSpritesAssetsIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURE_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURE_VADDR_END, (void*)TITLE_SCREEN_ASSETS_INDEX_VADDR_START, (void*)TITLE_SCREEN_ASSETS_INDEX_VADDR_END, NULL, NULL, 0);
    setSpriteShrinkFactor(LICENSED_BY_NINTENDO_2, titleScreenContext.centerCoordinate - 64.0f, 72.0f, 64.0f);
    setSpriteScale(LICENSED_BY_NINTENDO_2, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(LICENSED_BY_NINTENDO_2, 0.0f, 0.0f, 0.0f);
    setSpriteRenderingLayer(LICENSED_BY_NINTENDO_2, (1 | 2));
    setSpriteColor(LICENSED_BY_NINTENDO_2, 0xFF, 0xFF, 0xFF, 0xFF);
    setBilinearFiltering(LICENSED_BY_NINTENDO_2, TRUE);
    startSpriteAnimation(LICENSED_BY_NINTENDO_2, 0x12, 0xFE);

    dmaSprite(CLOUD_3_2, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesAssetsIndexSegmentRomStart, &_titleSpritesAssetsIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURE_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURE_VADDR_END, (void*)TITLE_SCREEN_ASSETS_INDEX_VADDR_START, (void*)TITLE_SCREEN_ASSETS_INDEX_VADDR_END, NULL, NULL, 0);
    setSpriteShrinkFactor(CLOUD_3_2, titleScreenContext.centerCoordinate - 192.0f, 108.0f, 64.0f);
    setSpriteScale(CLOUD_3_2, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(CLOUD_3_2, 0.0f, 0.0f, 0.0f);
    setSpriteRenderingLayer(CLOUD_3_2, (1 | 2));
    setSpriteColor(CLOUD_3_2, 0xFF, 0xFF, 0xFF, 0xFF);
    setBilinearFiltering(CLOUD_3_2, TRUE);
    startSpriteAnimation(CLOUD_3_2, 0x11, 0xFE);

    dmaSprite(CLOUD_2_2, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesAssetsIndexSegmentRomStart, &_titleSpritesAssetsIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURE_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURE_VADDR_END, (void*)TITLE_SCREEN_ASSETS_INDEX_VADDR_START, (void*)TITLE_SCREEN_ASSETS_INDEX_VADDR_END, NULL, NULL, 0);
    setSpriteShrinkFactor(CLOUD_2_2, titleScreenContext.centerCoordinate - 288.0f, 88.0f, 64.0f);
    setSpriteScale(CLOUD_2_2, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(CLOUD_2_2, 0.0f, 0.0f, 0.0f);
    setSpriteRenderingLayer(CLOUD_2_2, (1 | 2));
    setSpriteColor(CLOUD_2_2, 0xFF, 0xFF, 0xFF, 0xFF);
    setBilinearFiltering(CLOUD_2_2, TRUE);
    startSpriteAnimation(CLOUD_2_2, 0x10, 0xFE);
    
    dmaSprite(FAR_GRASS_1_1, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesAssetsIndexSegmentRomStart, &_titleSpritesAssetsIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURE_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURE_VADDR_END, (void*)TITLE_SCREEN_ASSETS_INDEX_VADDR_START, (void*)TITLE_SCREEN_ASSETS_INDEX_VADDR_END, NULL, NULL, 0);
    setSpriteShrinkFactor(FAR_GRASS_1_1, titleScreenContext.centerCoordinate + 160.0f, 0.0f, 8.0f);
    setSpriteScale(FAR_GRASS_1_1, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(FAR_GRASS_1_1, 0.0f, 0.0f, 0.0f);
    setSpriteRenderingLayer(FAR_GRASS_1_1, (1 | 2));
    setSpriteColor(FAR_GRASS_1_1, 0xFF, 0xFF, 0xFF, 0xFF);
    setBilinearFiltering(FAR_GRASS_1_1, TRUE);
    startSpriteAnimation(FAR_GRASS_1_1, 0xA, 0xFE);

    dmaSprite(FAR_GRASS_1_2, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesAssetsIndexSegmentRomStart, &_titleSpritesAssetsIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURE_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURE_VADDR_END, (void*)TITLE_SCREEN_ASSETS_INDEX_VADDR_START, (void*)TITLE_SCREEN_ASSETS_INDEX_VADDR_END, NULL, NULL, 0);
    setSpriteShrinkFactor(FAR_GRASS_1_2, titleScreenContext.centerCoordinate - 160.0f, 0.0f, 8.0f);
    setSpriteScale(FAR_GRASS_1_2, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(FAR_GRASS_1_2, 0.0f, 0.0f, 0.0f);
    setSpriteRenderingLayer(FAR_GRASS_1_2, (1 | 2));
    setSpriteColor(FAR_GRASS_1_2, 0xFF, 0xFF, 0xFF, 0xFF);
    setBilinearFiltering(FAR_GRASS_1_2, TRUE);
    startSpriteAnimation(FAR_GRASS_1_2, 0xA, 0xFE);
    
    dmaSprite(FAR_GRASS_2_1, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesAssetsIndexSegmentRomStart, &_titleSpritesAssetsIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURE_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURE_VADDR_END, (void*)TITLE_SCREEN_ASSETS_INDEX_VADDR_START, (void*)TITLE_SCREEN_ASSETS_INDEX_VADDR_END, NULL, NULL, 0);
    setSpriteShrinkFactor(FAR_GRASS_2_1, titleScreenContext.centerCoordinate + 160.0f, 0.0f, 16.0f);
    setSpriteScale(FAR_GRASS_2_1, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(FAR_GRASS_2_1, 0.0f, 0.0f, 0.0f);
    setSpriteRenderingLayer(FAR_GRASS_2_1, (1 | 2));
    setSpriteColor(FAR_GRASS_2_1, 0xFF, 0xFF, 0xFF, 0xFF);
    setBilinearFiltering(FAR_GRASS_2_1, TRUE);
    startSpriteAnimation(FAR_GRASS_2_1, 0xB, 0xFE);

    dmaSprite(FAR_GRASS_3_1, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesAssetsIndexSegmentRomStart, &_titleSpritesAssetsIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURE_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURE_VADDR_END, (void*)TITLE_SCREEN_ASSETS_INDEX_VADDR_START, (void*)TITLE_SCREEN_ASSETS_INDEX_VADDR_END, NULL, NULL, 0);
    setSpriteShrinkFactor(FAR_GRASS_3_1, titleScreenContext.centerCoordinate + 160.0f, 0.0f, 24.0f);
    setSpriteScale(FAR_GRASS_3_1, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(FAR_GRASS_3_1, 0.0f, 0.0f, 0.0f);
    setSpriteRenderingLayer(FAR_GRASS_3_1, (1 | 2));
    setSpriteColor(FAR_GRASS_3_1, 0xFF, 0xFF, 0xFF, 0xFF);
    setBilinearFiltering(FAR_GRASS_3_1, TRUE);
    startSpriteAnimation(FAR_GRASS_3_1, 0xC, 0xFE);

    dmaSprite(FRONT_GRASS_2_1, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesAssetsIndexSegmentRomStart, &_titleSpritesAssetsIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURE_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURE_VADDR_END, (void*)TITLE_SCREEN_ASSETS_INDEX_VADDR_START, (void*)TITLE_SCREEN_ASSETS_INDEX_VADDR_END, NULL, NULL, 0);
    setSpriteShrinkFactor(FRONT_GRASS_2_1, titleScreenContext.centerCoordinate + 160.0f, 0.0f, 32.0f);
    setSpriteScale(FRONT_GRASS_2_1, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(FRONT_GRASS_2_1, 0.0f, 0.0f, 0.0f);
    setSpriteRenderingLayer(FRONT_GRASS_2_1, (1 | 2));
    setSpriteColor(FRONT_GRASS_2_1, 0xFF, 0xFF, 0xFF, 0xFF);
    setBilinearFiltering(FRONT_GRASS_2_1, TRUE);
    startSpriteAnimation(FRONT_GRASS_2_1, 0xE, 0xFE);
    
    dmaSprite(BACK_GRASS_2, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesAssetsIndexSegmentRomStart, &_titleSpritesAssetsIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURE_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURE_VADDR_END, (void*)TITLE_SCREEN_ASSETS_INDEX_VADDR_START, (void*)TITLE_SCREEN_ASSETS_INDEX_VADDR_END, NULL, NULL, 0);
    setSpriteShrinkFactor(BACK_GRASS_2, titleScreenContext.centerCoordinate - 160.0f, 0.0f, 16.0f);
    setSpriteScale(BACK_GRASS_2, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(BACK_GRASS_2, 0.0f, 0.0f, 0.0f);
    setSpriteRenderingLayer(BACK_GRASS_2, (1 | 2));
    setSpriteColor(BACK_GRASS_2, 0xFF, 0xFF, 0xFF, 0xFF);
    setBilinearFiltering(BACK_GRASS_2, TRUE);
    startSpriteAnimation(BACK_GRASS_2, 0xB, 0xFE);
    
    dmaSprite(BACK_GRASS_3_2, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesAssetsIndexSegmentRomStart, &_titleSpritesAssetsIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURE_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURE_VADDR_END, (void*)TITLE_SCREEN_ASSETS_INDEX_VADDR_START, (void*)TITLE_SCREEN_ASSETS_INDEX_VADDR_END, NULL, NULL, 0);
    setSpriteShrinkFactor(BACK_GRASS_3_2, titleScreenContext.centerCoordinate - 160.0f, 0.0f, 24.0f);
    setSpriteScale(BACK_GRASS_3_2, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(BACK_GRASS_3_2, 0.0f, 0.0f, 0.0f);
    setSpriteRenderingLayer(BACK_GRASS_3_2, (1 | 2));
    setSpriteColor(BACK_GRASS_3_2, 0xFF, 0xFF, 0xFF, 0xFF);
    setBilinearFiltering(BACK_GRASS_3_2, TRUE);
    startSpriteAnimation(BACK_GRASS_3_2, 0xC, 0xFE);
    
    dmaSprite(FRONT_GRASS_2_2, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesAssetsIndexSegmentRomStart, &_titleSpritesAssetsIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURE_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURE_VADDR_END, (void*)TITLE_SCREEN_ASSETS_INDEX_VADDR_START, (void*)TITLE_SCREEN_ASSETS_INDEX_VADDR_END, NULL, NULL, 0);
    setSpriteShrinkFactor(FRONT_GRASS_2_2, titleScreenContext.centerCoordinate - 160.0f, 0.0f, 32.0f);
    setSpriteScale(FRONT_GRASS_2_2, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(FRONT_GRASS_2_2, 0.0f, 0.0f, 0.0f);
    setSpriteRenderingLayer(FRONT_GRASS_2_2, (1 | 2));
    setSpriteColor(FRONT_GRASS_2_2, 0xFF, 0xFF, 0xFF, 0xFF);
    setBilinearFiltering(FRONT_GRASS_2_2, TRUE);
    startSpriteAnimation(FRONT_GRASS_2_2, 0xE, 0xFE);
    
    dmaSprite(PUSH_THE_START_SIGN_2, &_dialogueIconsTextureSegmentRomStart, &_dialogueIconsTextureSegmentRomEnd, &_dialogueIconsAssetsIndexSegmentRomStart, &_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (void*)0x8023B400, NULL, (void*)0x8023CC00, (void*)0x8023CE00, (void*)0x8023D200, NULL, NULL, 0);
    setSpriteShrinkFactor(PUSH_THE_START_SIGN_2, -54.0f, -24.0f, 56.0f);
    setSpriteScale(PUSH_THE_START_SIGN_2, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(PUSH_THE_START_SIGN_2, 0.0f, 0.0f, 0.0f);
    setSpriteRenderingLayer(PUSH_THE_START_SIGN_2, (1 | 2));
    setSpriteColor(PUSH_THE_START_SIGN_2, 0xFF, 0xFF, 0xFF, 0xFF);
    setBilinearFiltering(PUSH_THE_START_SIGN_2, TRUE);
    startSpriteAnimation(PUSH_THE_START_SIGN_2, 3, 0xFE);
    
    setMainLoopCallbackFunctionIndex(TITLE_SCREEN);
    
    if (arg0 != 0) {

        titleScreenContext.mode = 2;

        adjustSpriteShrinkFactor(PUSH_THE_START_SIGN_1, 320.0f, 0.0f, 0.0f);
        adjustSpriteShrinkFactor(HOW_TO_PLAY_SIGN, 320.0f, 0.0f, 0.0f);
        adjustSpriteShrinkFactor(DIRT_ROAD, 320.0f, 0.0f, 0.0f);
        adjustSpriteShrinkFactor(SIGN_POST_1, 320.0f, 0.0f, 0.0f);
        adjustSpriteShrinkFactor(SIGN_POST_2, 320.0f, 0.0f, 0.0f);
        adjustSpriteShrinkFactor(SIGN_POST_3, 320.0f, 0.0f, 0.0f);
        adjustSpriteShrinkFactor(FAR_GRASS_1_1, 320.0f, 0.0f, 0.0f);
        adjustSpriteShrinkFactor(FAR_GRASS_1_2, 320.0f, 0.0f, 0.0f);
        adjustSpriteShrinkFactor(FAR_GRASS_2_1, 128.0f, 0.0f, 0.0f);
        adjustSpriteShrinkFactor(FAR_GRASS_3_1, 192.0f, 0.0f, 0.0f);
        adjustSpriteShrinkFactor(FRONT_GRASS_2_1, 256.0f, 0.0f, 0.0f);
        adjustSpriteShrinkFactor(BACK_GRASS_2, 128.0f, 0.0f, 0.0f);
        adjustSpriteShrinkFactor(BACK_GRASS_3_2, 192.0f, 0.0f, 0.0f);
        adjustSpriteShrinkFactor(FRONT_GRASS_2_2, 256.0f, 0.0f, 0.0f);
        adjustSpriteShrinkFactor(CLOUD_1_1, 64.0f, 0.0f, 0.0f);
        adjustSpriteShrinkFactor(CLOUD_1_2, 64.0f, 0.0f, 0.0f);
        adjustSpriteShrinkFactor(CLOUD_2_1, 64.0f, 0.0f, 0.0f);
        adjustSpriteShrinkFactor(CLOUD_3_1, 64.0f, 0.0f, 0.0f);
        adjustSpriteShrinkFactor(LICENSED_BY_NINTENDO_1, 64.0f, 0.0f, 0.0f);
        adjustSpriteShrinkFactor(LICENSED_BY_NINTENDO_2, 64.0f, 0.0f, 0.0f);
        adjustSpriteShrinkFactor(CLOUD_3_2, 64.0f, 0.0f, 0.0f);
        adjustSpriteShrinkFactor(CLOUD_2_2, 64.0f, 0.0f, 0.0f);

    } else {

        titleScreenContext.mode = 0;
    
    }
    
    titleScreenContext.frameCounter = 0;
    
    loadDogTitleSprite();
    
    setCurrentSong(TITLE_THEME);
    setSongVolume(TITLE_THEME, SONG_VOLUME);

}

//INCLUDE_ASM("asm/nonmatchings/game/title", loadDogTitleSprite);

void loadDogTitleSprite(void) {
    dmaSprite(DOG_TITLE_SPRITE, &_dogTitleTextureSegmentRomStart, &_dogTitleTextureSegmentRomEnd, &_dogTitleAssetsIndexSegmentRomStart, &_dogTitleAssetsIndexSegmentRomEnd, &_dogTitleSpritesheetIndexSegmentRomStart, &_dogTitleSpritesheetIndexSegmentRomEnd, (void*)0x80240B00, (void*)0x80243B00, (void*)0x80246B00, (void*)0x8024AB00, (void*)0x80252300, (void*)0x80252B00, 1, 0);
    setSpriteShrinkFactor(DOG_TITLE_SPRITE, 64.0f, -80.0f, 56.0f);
    setSpriteScale(DOG_TITLE_SPRITE, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(DOG_TITLE_SPRITE, 0, 0, 0);
    setSpriteRenderingLayer(DOG_TITLE_SPRITE, (1 | 2));
    setSpriteColor(DOG_TITLE_SPRITE, 0xFF, 0xFF, 0xFF, 0xFF);
    startSpriteAnimation(DOG_TITLE_SPRITE, 0x1D, 0xFE);
}

static inline u32 increment(u32 x) {
    return x+1;
}

static inline u32 incrementUnk4() {
    titleScreenContext.unk_4 = increment(titleScreenContext.unk_4);
}

static inline void setNewMode(u8 mode) {
    titleScreenContext.mode = mode;
    titleScreenContext.unk_4 = 0;
}

static inline void resetSpriteFlags() {

    deactivateSprite(HARVEST_MOON_64_LOGO);
    deactivateSprite(MARVCOME_VICTOR_COPYRIGHT_INFO);
    deactivateSprite(DOG_TITLE_SPRITE);
    deactivateSprite(PUSH_THE_START_SIGN_2);
    deactivateSprite(PUSH_THE_START_SIGN_1);
    deactivateSprite(HOW_TO_PLAY_SIGN);
    deactivateSprite(DIRT_ROAD);
    deactivateSprite(SIGN_POST_1);
    deactivateSprite(SIGN_POST_2);
    deactivateSprite(SIGN_POST_3);
    deactivateSprite(FRONT_GRASS_2_1);
    deactivateSprite(FAR_GRASS_3_1);
    deactivateSprite(FAR_GRASS_2_1);
    deactivateSprite(FRONT_GRASS_2_2);
    deactivateSprite(BACK_GRASS_3_2);
    deactivateSprite(BACK_GRASS_2);
    deactivateSprite(FAR_GRASS_1_1);
    deactivateSprite(FAR_GRASS_1_2);
    deactivateSprite(CLOUD_1_1);
    deactivateSprite(CLOUD_1_2);
    deactivateSprite(CLOUD_2_1);
    deactivateSprite(CLOUD_3_1);
    deactivateSprite(LICENSED_BY_NINTENDO_1);
    deactivateSprite(LICENSED_BY_NINTENDO_2);
    deactivateSprite(CLOUD_3_2);
    deactivateSprite(CLOUD_2_2);

}

// TODO: label properly
static inline bool inline1() {

    if (titleScreenContext.rowHighlighted != 1) {
        setSpriteShrinkFactor(PUSH_THE_START_SIGN_2, -56.0f, -52.0f, 56.0f);
        titleScreenContext.rowHighlighted = 1;
        setAudio(MOVE_CURSOR);
    }

   return 1;

}

// TODO: label properly
static inline bool inline2() {

    if (titleScreenContext.rowHighlighted != 0) {
        setSpriteShrinkFactor(PUSH_THE_START_SIGN_2, -56.0f, -20.0f, 56.0f);
        titleScreenContext.rowHighlighted = 0;
        setAudio(MOVE_CURSOR);
    }

    return 1;

}

static inline bool handleFrameIncrement() {

    titleScreenContext.frameCounter++;
    
    if (titleScreenContext.frameCounter == 500) {

        updateSpriteRGBA(HARVEST_MOON_64_LOGO, 0, 0, 0, 0, 8);
        updateSpriteRGBA(MARVCOME_VICTOR_COPYRIGHT_INFO, 0, 0, 0, 0, 8);
        updateSpriteRGBA(DOG_TITLE_SPRITE, 0, 0, 0, 0, 8);
        updateSpriteRGBA(PUSH_THE_START_SIGN_2, 0, 0, 0, 0, 8);
        updateSpriteRGBA(PUSH_THE_START_SIGN_1, 0, 0, 0, 0, 8);
        updateSpriteRGBA(HOW_TO_PLAY_SIGN, 0, 0, 0, 0, 8);
        updateSpriteRGBA(DIRT_ROAD, 0, 0, 0, 0, 8);
        updateSpriteRGBA(SIGN_POST_1, 0, 0, 0, 0, 8);
        updateSpriteRGBA(SIGN_POST_2, 0, 0, 0, 0, 8);
        updateSpriteRGBA(SIGN_POST_3, 0, 0, 0, 0, 8);
        updateSpriteRGBA(FRONT_GRASS_2_1, 0, 0, 0, 0, 8);
        updateSpriteRGBA(FAR_GRASS_3_1, 0, 0, 0, 0, 8);
        updateSpriteRGBA(FAR_GRASS_2_1, 0, 0, 0, 0, 8);
        updateSpriteRGBA(FRONT_GRASS_2_2, 0, 0, 0, 0, 8);
        updateSpriteRGBA(BACK_GRASS_3_2, 0, 0, 0, 0, 8);
        updateSpriteRGBA(BACK_GRASS_2, 0, 0, 0, 0, 8);
        updateSpriteRGBA(FAR_GRASS_1_1, 0, 0, 0, 0, 8);
        updateSpriteRGBA(FAR_GRASS_1_2, 0, 0, 0, 0, 8);
        updateSpriteRGBA(CLOUD_1_1, 0, 0, 0, 0, 8);
        updateSpriteRGBA(CLOUD_1_2, 0, 0, 0, 0, 8);
        updateSpriteRGBA(CLOUD_2_1, 0, 0, 0, 0, 8);
        updateSpriteRGBA(CLOUD_3_1, 0, 0, 0, 0, 8);
        updateSpriteRGBA(LICENSED_BY_NINTENDO_1, 0, 0, 0, 0, 8);
        updateSpriteRGBA(LICENSED_BY_NINTENDO_2, 0, 0, 0, 0, 8);
        updateSpriteRGBA(CLOUD_3_2, 0, 0, 0, 0, 8);
        updateSpriteRGBA(CLOUD_2_2, 0, 0, 0, 0, 8);

        setSongWithDefaultSpeed(TITLE_THEME);

        titleScreenContext.mode = 5;

        return 1;

    }
    
}

// jtbl_80123658
//INCLUDE_ASM("asm/nonmatchings/game/title", titleScreenMainLoopCallback);

// main loop callback
void titleScreenMainLoopCallback(void) {

    bool set = FALSE;
    
    switch (titleScreenContext.mode) {

        case 0:

            set = handleFrameIncrement();

            if (checkButtonPressed(CONTROLLER_1, BUTTON_START)) {

                if (!set) {
                    
                    resetAnimationState(PUSH_THE_START_SIGN_1);
                    startSpriteAnimation(PUSH_THE_START_SIGN_1, 4, 0xFF);
                    resetAnimationState(DOG_TITLE_SPRITE);
                    resetAnimationState(PUSH_THE_START_SIGN_2);
                    startSpriteAnimation(DOG_TITLE_SPRITE, 8, 0xFE);
    
                    titleScreenContext.mode = 1;
                    setAudio(0);

                }
            }

            func_800E0ED4();
            break;
        
        case 1:

            // static inline?
            if (titleScreenContext.unk_4 < 64) {

                adjustSpriteShrinkFactor(PUSH_THE_START_SIGN_1, 5.0f, 0.0f, 0.0f);
                adjustSpriteShrinkFactor(HOW_TO_PLAY_SIGN, 5.0f, 0, 0);
                adjustSpriteShrinkFactor(DIRT_ROAD, 5.0f, 0, 0);
                adjustSpriteShrinkFactor(SIGN_POST_1, 5.0f, 0, 0);
                adjustSpriteShrinkFactor(SIGN_POST_2, 5.0f, 0, 0);
                adjustSpriteShrinkFactor(SIGN_POST_3, 5.0f, 0, 0);
                adjustSpriteShrinkFactor(FAR_GRASS_1_1, 5.0f, 0, 0);
                adjustSpriteShrinkFactor(FAR_GRASS_1_2, 5.0f, 0, 0);
                adjustSpriteShrinkFactor(FAR_GRASS_2_1, 2.0f, 0, 0);
                adjustSpriteShrinkFactor(FAR_GRASS_3_1, 3.0f, 0, 0);
                adjustSpriteShrinkFactor(FRONT_GRASS_2_1, 5.0f, 0, 0);
                adjustSpriteShrinkFactor(BACK_GRASS_2, 2.0f, 0, 0);
                adjustSpriteShrinkFactor(BACK_GRASS_3_2, 3.0f, 0, 0);
                adjustSpriteShrinkFactor(FRONT_GRASS_2_2, 5.0f, 0, 0);
                adjustSpriteShrinkFactor(CLOUD_1_1, 1.0f, 0, 0);
                adjustSpriteShrinkFactor(CLOUD_1_2, 1.0f, 0, 0);
                adjustSpriteShrinkFactor(CLOUD_2_1, 1.0f, 0, 0);
                adjustSpriteShrinkFactor(CLOUD_3_1, 1.0f, 0, 0);
                adjustSpriteShrinkFactor(LICENSED_BY_NINTENDO_1, 1.0f, 0, 0);
                adjustSpriteShrinkFactor(LICENSED_BY_NINTENDO_2, 1.0f, 0, 0);
                adjustSpriteShrinkFactor(CLOUD_3_2, 1.0f, 0, 0);
                adjustSpriteShrinkFactor(CLOUD_2_2, 1.0f, 0, 0);

            } else {
                setNewMode(2);
            }

            // FIXME: shouldn't be right
            incrementUnk4();
            
            break;
        
        case 2:

            resetAnimationState(DOG_TITLE_SPRITE);
            startSpriteAnimation(DOG_TITLE_SPRITE, 0x1D, 0xFE);
            setSpriteShrinkFactor(PUSH_THE_START_SIGN_2, -56.0f, -20.0f, 56.0f);
            startSpriteAnimation(PUSH_THE_START_SIGN_2, 3, 0xFE);
            
            titleScreenContext.mode = 3;
            titleScreenContext.rowHighlighted = 0;

            break;
        
        case 3:

            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_UP)) {        

                if (!set) {
                    set = inline1();
                }
            }
            
            if (checkButtonRepeat(CONTROLLER_1, 0x100000)) {

                if (!set) {
                    set = inline2();
                }
            }
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A | BUTTON_START)) {

                if (!set) {
                    
                    if (titleScreenContext.rowHighlighted == 0) {
                        resetAnimationState(HOW_TO_PLAY_SIGN);
                        startSpriteAnimation(HOW_TO_PLAY_SIGN, 6, 0xFF);
                    } else {
                        resetAnimationState(DIRT_ROAD);
                        startSpriteAnimation(DIRT_ROAD, 8, 0xFF);
                    }
                      
                    updateSpriteRGBA(HARVEST_MOON_64_LOGO, 0, 0, 0, 0, 8);
                    updateSpriteRGBA(MARVCOME_VICTOR_COPYRIGHT_INFO, 0, 0, 0, 0, 8);
                    updateSpriteRGBA(DOG_TITLE_SPRITE, 0, 0, 0, 0, 8);
                    updateSpriteRGBA(PUSH_THE_START_SIGN_2, 0, 0, 0, 0, 8);
                    updateSpriteRGBA(PUSH_THE_START_SIGN_1, 0, 0, 0, 0, 8);
                    updateSpriteRGBA(HOW_TO_PLAY_SIGN, 0, 0, 0, 0, 8);
                    updateSpriteRGBA(DIRT_ROAD, 0, 0, 0, 0, 8);
                    updateSpriteRGBA(SIGN_POST_1, 0, 0, 0, 0, 8);
                    updateSpriteRGBA(SIGN_POST_2, 0, 0, 0, 0, 8);
                    updateSpriteRGBA(SIGN_POST_3, 0, 0, 0, 0, 8);
                    updateSpriteRGBA(FRONT_GRASS_2_1, 0, 0, 0, 0, 8);
                    updateSpriteRGBA(FAR_GRASS_3_1, 0, 0, 0, 0, 8);
                    updateSpriteRGBA(FAR_GRASS_2_1, 0, 0, 0, 0, 8);
                    updateSpriteRGBA(FRONT_GRASS_2_2, 0, 0, 0, 0, 8);
                    updateSpriteRGBA(BACK_GRASS_3_2, 0, 0, 0, 0, 8);
                    updateSpriteRGBA(BACK_GRASS_2, 0, 0, 0, 0, 8);
                    updateSpriteRGBA(FAR_GRASS_1_1, 0, 0, 0, 0, 8);
                    updateSpriteRGBA(FAR_GRASS_1_2, 0, 0, 0, 0, 8);
                    updateSpriteRGBA(CLOUD_1_1, 0, 0, 0, 0, 8);
                    updateSpriteRGBA(CLOUD_1_2, 0, 0, 0, 0, 8);
                    updateSpriteRGBA(CLOUD_2_1, 0, 0, 0, 0, 8);
                    updateSpriteRGBA(CLOUD_3_1, 0, 0, 0, 0, 8);
                    updateSpriteRGBA(LICENSED_BY_NINTENDO_1, 0, 0, 0, 0, 8);
                    updateSpriteRGBA(LICENSED_BY_NINTENDO_2, 0, 0, 0, 0, 8);
                    updateSpriteRGBA(CLOUD_3_2, 0, 0, 0, 0, 8);
                    updateSpriteRGBA(CLOUD_2_2, 0, 0, 0, 0, 8);
    
                    titleScreenContext.mode = 4;
    
                    setAudio(0);
                    
                }
                
            }

            func_800E0ED4();

            break;

        // leaving title screen
        case 4:
            
            // check sprite flag
            if (func_8002CBF8(HARVEST_MOON_64_LOGO)) {

                resetSpriteFlags();
                
                if (titleScreenContext.rowHighlighted == 0) {
                    // load select game screen
                    func_800E1380(FALSE);
                } else {
                    func_80055F08(HOW_TO_PLAY_CUTSCENE, 0, 0);
                }
            }

            break;

        case 5:
            
            if (func_8002CBF8(HARVEST_MOON_64_LOGO) && checkDefaultSongChannelOpen(TITLE_THEME)) {
                
                resetSpriteFlags();
 
                switch (demoCutsceneIndex) {
                    case 0:
                        func_80055F08(DEMO_CUTSCENE_1, 0, 1);
                        break;
                    case 1:
                        func_80055F08(DEMO_CUTSCENE_2, 0x1D, 1);
                        break;
                    case 2:
                        func_80055F08(DEMO_CUTSCENE_3, 0x32, 1);
                        break;
                    default:
                        break;
                }

                demoCutsceneIndex++;

                if (demoCutsceneIndex > 2) {
                    demoCutsceneIndex = 0;
                }   
            }
        }  
            
    updateGrassAnimation();
    func_800E10E8();

}

static inline void handleUpdateDogAnimation() {
    
    switch (titleScreenContext.dogAnimationIndex) {
            case 0:
                startSpriteAnimation(DOG_TITLE_SPRITE, 0xB, 0xFF);
                titleScreenContext.dogAnimationIndex = 0xFF;
                break;
            case 1:
                startSpriteAnimation(DOG_TITLE_SPRITE, 0x13, 0xFF);
                titleScreenContext.dogAnimationIndex = 0xFF;
                break;
            case 2:
                startSpriteAnimation(DOG_TITLE_SPRITE, 0x2D, 0xFF);
                titleScreenContext.dogAnimationIndex = 0xFF;
                break;
            case 3:
                startSpriteAnimation(DOG_TITLE_SPRITE, 0x32, 0xFF);
                titleScreenContext.dogAnimationIndex = 0xFF;
                break;            
            case 4:
                startSpriteAnimation(DOG_TITLE_SPRITE, 0x48, 0xFF);
                titleScreenContext.dogAnimationIndex = 0xFF;
                break;         
            case 5:
                startSpriteAnimation(DOG_TITLE_SPRITE, 0x63, 0xFF);
                titleScreenContext.dogAnimationIndex = 0xFF;
                break;            
            case 6:
                startSpriteAnimation(DOG_TITLE_SPRITE, 0x69, 0xFF);
                titleScreenContext.dogAnimationIndex = 0xFF;
                break;            
            default:
               startSpriteAnimation(DOG_TITLE_SPRITE, 0x1D, 0xFE);
                if (titleScreenContext.dogIdleCounter >= 0x1F) {
                    titleScreenContext.dogAnimationIndex = getRandomNumberInRange(0, 10);
                    titleScreenContext.dogIdleCounter = 0;
                }
                break;
        }
    
        titleScreenContext.dogIdleCounter++;
}

// jtbl_80123670
//INCLUDE_ASM("asm/nonmatchings/game/title", func_800E0ED4);

void func_800E0ED4(void) {

    if (checkSpriteAnimationStateChanged(DOG_TITLE_SPRITE)) {
        resetAnimationState(DOG_TITLE_SPRITE);
        handleUpdateDogAnimation();
    }
}

// alternate
/*
void func_800E0ED4(void) {

    if (checkSpriteAnimationStateChanged(0x44)) {
        
        resetAnimationState(0x44);

        switch (D_80126572) {
            case 0:
                startSpriteAnimation(0x44, 0xB, 0xFF);
                D_80126572 = 0xFF;
                break;
            case 1:
                startSpriteAnimation(0x44, 0x13, 0xFF);
                D_80126572 = 0xFF;
                break;
            case 2:
                startSpriteAnimation(0x44, 0x2D, 0xFF);
                D_80126572 = 0xFF;
                break;
            case 3:
                startSpriteAnimation(0x44, 0x32, 0xFF);
                D_80126572 = 0xFF;
                break;            
            case 4:
                startSpriteAnimation(0x44, 0x48, 0xFF);
                D_80126572 = 0xFF;
                break;         
            case 5:
                startSpriteAnimation(0x44, 0x63, 0xFF);
                D_80126572 = 0xFF;
                break;            
            case 6:
                startSpriteAnimation(0x44, 0x69, 0xFF);
                D_80126572 = 0xFF;
                break;            
            default:
               startSpriteAnimation(0x44, 0x1D, 0xFE);
                if (D_80126576 >= 0x1F) {
                    D_80126572 = getRandomNumberInRange(0, 10);
                    D_80126576 = 0;
                }
                break;
        }

        D_80126576++;
    }
}
*/
//INCLUDE_ASM("asm/nonmatchings/game/title", updateGrassAnimation);

// update grass
void updateGrassAnimation(void) {

    // check flag 0x40
    if (checkSpriteAnimationStateChanged(FAR_GRASS_3_1)) {
        
        if (getRandomNumberInRange(0, 1)) {
            
            // toggle bit 2
            resetAnimationState(FAR_GRASS_3_1);
            resetAnimationState(FRONT_GRASS_2_1);
            resetAnimationState(BACK_GRASS_3_2);
            resetAnimationState(FRONT_GRASS_2_2);

            startSpriteAnimation(FAR_GRASS_3_1, 0xC, 0xFE);
            startSpriteAnimation(FRONT_GRASS_2_1, 0xE, 0xFE);
            startSpriteAnimation(BACK_GRASS_3_2, 0xC, 0xFE);
            startSpriteAnimation(FRONT_GRASS_2_2, 0xE, 0xFE);

        } else {
            
            resetAnimationState(FAR_GRASS_3_1);
            resetAnimationState(FRONT_GRASS_2_1);
            resetAnimationState(BACK_GRASS_3_2);
            resetAnimationState(FRONT_GRASS_2_2);

            startSpriteAnimation(FAR_GRASS_3_1, 0xD, 0xFE);
            startSpriteAnimation(FRONT_GRASS_2_1, 0xF, 0xFE);
            startSpriteAnimation(BACK_GRASS_3_2, 0xD, 0xFE);
            startSpriteAnimation(FRONT_GRASS_2_2, 0xF, 0xFE);

        }
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/title", func_800E10E8);

// adjust shrink of overlaying sprites
void func_800E10E8(void) {
    
    if (globalSprites[CLOUD_2_1].shrink.x > 320.0f) {
        setSpriteShrinkFactor(CLOUD_2_1, titleScreenContext.centerCoordinate - 320.0f, 64.0f, 64.0f);
    }
    if (globalSprites[CLOUD_3_1].shrink.x > 320.0f) {
        setSpriteShrinkFactor(CLOUD_3_1, titleScreenContext.centerCoordinate - 320.0f, 96.0f, 64.0f);
    }
    if (globalSprites[LICENSED_BY_NINTENDO_1].shrink.x > 320.0f) {
        setSpriteShrinkFactor(LICENSED_BY_NINTENDO_1, titleScreenContext.centerCoordinate - 320.0f, 80.0f, 64.0f);
    }
    if (globalSprites[LICENSED_BY_NINTENDO_2].shrink.x > 320.0f) {
        setSpriteShrinkFactor(LICENSED_BY_NINTENDO_2, titleScreenContext.centerCoordinate - 320.0f, 72.0f, 64.0f);
    }
    if (globalSprites[CLOUD_3_2].shrink.x > 320.0f) {
        setSpriteShrinkFactor(CLOUD_3_2, titleScreenContext.centerCoordinate - 320.0f, 108.0f, 64.0f);
    }
    if (globalSprites[CLOUD_2_2].shrink.x > 320.0f) {
        setSpriteShrinkFactor(CLOUD_2_2, titleScreenContext.centerCoordinate - 320.0f, 88.0f, 64.0f);
    }
    
    adjustSpriteShrinkFactor(CLOUD_2_1, 0.2f, 0.0f, 0.0f);
    adjustSpriteShrinkFactor(CLOUD_3_1, 0.1f, 0.0f, 0.0f);
    adjustSpriteShrinkFactor(LICENSED_BY_NINTENDO_1, 0.3f, 0.0f, 0.0f);
    adjustSpriteShrinkFactor(LICENSED_BY_NINTENDO_2, 0.4f, 0.0f, 0.0f);
    adjustSpriteShrinkFactor(CLOUD_3_2, 0.1f, 0.0f, 0.0f);
    adjustSpriteShrinkFactor(CLOUD_2_2, 0.2f, 0.0f, 0.0f);
    
}