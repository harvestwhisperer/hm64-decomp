#include "common.h"

#include "title.h"

#include "system/controller.h"  
#include "system/dialogue.h"
#include "system/mathUtils.h"
#include "system/message.h"
#include "system/sprite.h"
                                                        
#include "gameAudio.h"
#include "initialize2.h"     
#include "loadGameScreen.h"      
#include "mainLoop.h" 
#include "setCutscenes.h"         
#include "spriteIndices.h"            
     
// forward declarations
void func_800E0ED4();                                  
void updateGrassAnimation();                                  
void func_800E10E8();                                  

// bss
TitleContext titleScreenContext;

extern u32 _dogTitleTextureSegmentRomStart;
extern u32 _dogTitleTextureSegmentRomEnd;
extern u32 _dogTitleAssetsIndexSegmentRomStart;
extern u32 _dogTitleAssetsIndexSegmentRomEnd;
extern u32 _dogTitleSpritesheetIndexSegmentRomStart;
extern u32 _dogTitleSpritesheetIndexSegmentRomEnd;

extern u32 _titleSpritesTextureSegmentRomStart;
extern u32 _titleSpritesTextureSegmentRomEnd;
extern u32 _titleSpritesIndexSegmentRomStart;
extern u32 _titleSpritesIndexSegmentRomEnd;

//INCLUDE_ASM(const s32, "title", initializeTitleScreen);

void initializeTitleScreen(u8 arg0) {
    
    titleScreenContext.rowHighlighted = 0;
    titleScreenContext.dogAnimationIndex = 0;
    titleScreenContext.unk_4 = 0;
    titleScreenContext.dogIdleCounter = 0;
    titleScreenContext.centerCoordinate = -160.0f;
    
    func_8002B138(HARVEST_MOON_64_LOGO, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesIndexSegmentRomStart, &_titleSpritesIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_END, (void*)TITLE_SCREEN_PALETTE_VADDR_START, (void*)TITLE_SCREEN_PALETTE_VADDR_END, NULL, NULL, 0);
    setSpriteShrinkFactor(HARVEST_MOON_64_LOGO, 0.0f, 64.0f, 64.0f);
    setSpriteScale(HARVEST_MOON_64_LOGO, 1.0f, 1.0f, 1.0f);
    func_8002BE14(HARVEST_MOON_64_LOGO, 0.0f, 0.0f, 0.0f);
    func_8002C7EC(HARVEST_MOON_64_LOGO, 3);
    func_8002C914(HARVEST_MOON_64_LOGO, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002CB24(HARVEST_MOON_64_LOGO, 1);
    func_8002B80C(HARVEST_MOON_64_LOGO, 0, 0xFE);
     
    func_8002B138(MARVCOME_VICTOR_COPYRIGHT_INFO, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesIndexSegmentRomStart, &_titleSpritesIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_END, (void*)TITLE_SCREEN_PALETTE_VADDR_START, (void*)TITLE_SCREEN_PALETTE_VADDR_END, NULL, NULL, 0);
    setSpriteShrinkFactor(MARVCOME_VICTOR_COPYRIGHT_INFO, 0.0f, -96.0f, 64.0f);
    setSpriteScale(MARVCOME_VICTOR_COPYRIGHT_INFO, 1.0f, 1.0f, 1.0f);
    func_8002BE14(MARVCOME_VICTOR_COPYRIGHT_INFO, 0.0f, 0.0f, 0.0f);
    func_8002C7EC(MARVCOME_VICTOR_COPYRIGHT_INFO, 3);
    func_8002C914(MARVCOME_VICTOR_COPYRIGHT_INFO, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002B80C(MARVCOME_VICTOR_COPYRIGHT_INFO, 1, 0xFE);
    
    func_8002B138(PUSH_THE_START_SIGN_1, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesIndexSegmentRomStart, &_titleSpritesIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_END, (void*)TITLE_SCREEN_PALETTE_VADDR_START, (void*)TITLE_SCREEN_PALETTE_VADDR_END, NULL, NULL, 0);
    setSpriteShrinkFactor(PUSH_THE_START_SIGN_1, titleScreenContext.centerCoordinate + 160.0f, -16.0f, 48.0f);
    setSpriteScale(PUSH_THE_START_SIGN_1, 1.0f, 1.0f, 1.0f);
    func_8002BE14(PUSH_THE_START_SIGN_1, 0.0f, 0.0f, 0.0f);
    func_8002C7EC(PUSH_THE_START_SIGN_1, 3);
    func_8002C914(PUSH_THE_START_SIGN_1, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002CB24(PUSH_THE_START_SIGN_1, 1);
    func_8002B80C(PUSH_THE_START_SIGN_1, 3, 0xFE);
    
    func_8002B138(HOW_TO_PLAY_SIGN, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesIndexSegmentRomStart, &_titleSpritesIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_END, (void*)TITLE_SCREEN_PALETTE_VADDR_START, (void*)TITLE_SCREEN_PALETTE_VADDR_END, NULL, NULL, 0);
    setSpriteShrinkFactor(HOW_TO_PLAY_SIGN, titleScreenContext.centerCoordinate - 160.0f, -12.0f, 48.0f);
    setSpriteScale(HOW_TO_PLAY_SIGN, 1.0f, 1.0f, 1.0f);
    func_8002BE14(HOW_TO_PLAY_SIGN, 0.0f, 0.0f, 0.0f);
    func_8002C7EC(HOW_TO_PLAY_SIGN, 3);
    func_8002C914(HOW_TO_PLAY_SIGN, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002CB24(HOW_TO_PLAY_SIGN, 1);
    func_8002B80C(HOW_TO_PLAY_SIGN, 5, 0xFE);

    func_8002B138(DIRT_ROAD, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesIndexSegmentRomStart, &_titleSpritesIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_END, (void*)TITLE_SCREEN_PALETTE_VADDR_START, (void*)TITLE_SCREEN_PALETTE_VADDR_END, NULL, NULL, 0);
    setSpriteShrinkFactor(DIRT_ROAD, titleScreenContext.centerCoordinate - 160.0f, -44.0f, 48.0f);
    setSpriteScale(DIRT_ROAD, 1.0f, 1.0f, 1.0f);
    func_8002BE14(DIRT_ROAD, 0.0f, 0.0f, 0.0f);
    func_8002C7EC(DIRT_ROAD, 3);
    func_8002C914(DIRT_ROAD, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002CB24(DIRT_ROAD, 1);
    func_8002B80C(DIRT_ROAD, 7, 0xFE);

    func_8002B138(SIGN_POST_1, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesIndexSegmentRomStart, &_titleSpritesIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_END, (void*)TITLE_SCREEN_PALETTE_VADDR_START, (void*)TITLE_SCREEN_PALETTE_VADDR_END, NULL, NULL, 0);
    setSpriteShrinkFactor(SIGN_POST_1, titleScreenContext.centerCoordinate + 160.0f, -32.0f, 40.0f);
    setSpriteScale(SIGN_POST_1, 1.0f, 1.0f, 1.0f);
    func_8002BE14(SIGN_POST_1, 0.0f, 0.0f, 0.0f);
    func_8002C7EC(SIGN_POST_1, 3);
    func_8002C914(SIGN_POST_1, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002CB24(SIGN_POST_1, 1);
    func_8002B80C(SIGN_POST_1, 2, 0xFE);

    func_8002B138(SIGN_POST_2, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesIndexSegmentRomStart, &_titleSpritesIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_END, (void*)TITLE_SCREEN_PALETTE_VADDR_START, (void*)TITLE_SCREEN_PALETTE_VADDR_END, NULL, NULL, 0);
    setSpriteShrinkFactor(SIGN_POST_2, titleScreenContext.centerCoordinate - 192.0f, -32.0f, 40.0f);
    setSpriteScale(SIGN_POST_2, 1.0f, 1.0f, 1.0f);
    func_8002BE14(SIGN_POST_2, 0.0f, 0.0f, 0.0f);
    func_8002C7EC(SIGN_POST_2, 3);
    func_8002C914(SIGN_POST_2, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002CB24(SIGN_POST_2, 1);
    func_8002B80C(SIGN_POST_2, 2, 0xFE);

    func_8002B138(SIGN_POST_3, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesIndexSegmentRomStart, &_titleSpritesIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_END, (void*)TITLE_SCREEN_PALETTE_VADDR_START, (void*)TITLE_SCREEN_PALETTE_VADDR_END, NULL, NULL, 0);
    setSpriteShrinkFactor(SIGN_POST_3, titleScreenContext.centerCoordinate - 128.0f, -32.0f, 40.0f);
    setSpriteScale(SIGN_POST_3, 1.0f, 1.0f, 1.0f);
    func_8002BE14(SIGN_POST_3, 0.0f, 0.0f, 0.0f);
    func_8002C7EC(SIGN_POST_3, 3);
    func_8002C914(SIGN_POST_3, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002CB24(SIGN_POST_3, 1);
    func_8002B80C(SIGN_POST_3, 2, 0xFE);

    func_8002B138(CLOUD_1_1, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesIndexSegmentRomStart, &_titleSpritesIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_END, (void*)TITLE_SCREEN_PALETTE_VADDR_START, (void*)TITLE_SCREEN_PALETTE_VADDR_END, NULL, NULL, 0);
    setSpriteShrinkFactor(CLOUD_1_1, titleScreenContext.centerCoordinate + 160.0f, 0.0f, 56.0f);
    setSpriteScale(CLOUD_1_1, 1.0f, 1.0f, 1.0f);
    func_8002BE14(CLOUD_1_1, 0.0f, 0.0f, 0.0f);
    func_8002C7EC(CLOUD_1_1, 3);
    func_8002C914(CLOUD_1_1, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002CB24(CLOUD_1_1, 1);
    func_8002B80C(CLOUD_1_1, 9, 0xFE);

    func_8002B138(CLOUD_1_2, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesIndexSegmentRomStart, &_titleSpritesIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_END, (void*)TITLE_SCREEN_PALETTE_VADDR_START, (void*)TITLE_SCREEN_PALETTE_VADDR_END, NULL, NULL, 0);
    setSpriteShrinkFactor(CLOUD_1_2, titleScreenContext.centerCoordinate - 160.0f, 0.0f, 56.0f);
    setSpriteScale(CLOUD_1_2, 1.0f, 1.0f, 1.0f);
    func_8002BE14(CLOUD_1_2, 0.0f, 0.0f, 0.0f);
    func_8002C7EC(CLOUD_1_2, 3);
    func_8002C914(CLOUD_1_2, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002CB24(CLOUD_1_2, 1);
    func_8002B80C(CLOUD_1_2, 9, 0xFE);
    
    func_8002B138(CLOUD_2_1, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesIndexSegmentRomStart, &_titleSpritesIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_END, (void*)TITLE_SCREEN_PALETTE_VADDR_START, (void*)TITLE_SCREEN_PALETTE_VADDR_END, NULL, NULL, 0);
    setSpriteShrinkFactor(CLOUD_2_1, titleScreenContext.centerCoordinate + 160.0f, 64.0f, 64.0f);
    setSpriteScale(CLOUD_2_1, 1.0f, 1.0f, 1.0f);
    func_8002BE14(CLOUD_2_1, 0.0f, 0.0f, 0.0f);
    func_8002C7EC(CLOUD_2_1, 3);
    func_8002C914(CLOUD_2_1, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002CB24(CLOUD_2_1, 1);
    func_8002B80C(CLOUD_2_1, 0x10, 0xFE);
    
    func_8002B138(CLOUD_3_1, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesIndexSegmentRomStart, &_titleSpritesIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_END, (void*)TITLE_SCREEN_PALETTE_VADDR_START, (void*)TITLE_SCREEN_PALETTE_VADDR_END, NULL, NULL, 0);
    setSpriteShrinkFactor(CLOUD_3_1, titleScreenContext.centerCoordinate + 32.0f, 96.0f, 64.0f);
    setSpriteScale(CLOUD_3_1, 1.0f, 1.0f, 1.0f);
    func_8002BE14(CLOUD_3_1, 0.0f, 0.0f, 0.0f);
    func_8002C7EC(CLOUD_3_1, 3);
    func_8002C914(CLOUD_3_1, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002CB24(CLOUD_3_1, 1);
    func_8002B80C(CLOUD_3_1, 0x11, 0xFE);
    
    func_8002B138(LICENSED_BY_NINTENDO_1, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesIndexSegmentRomStart, &_titleSpritesIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_END, (void*)TITLE_SCREEN_PALETTE_VADDR_START, (void*)TITLE_SCREEN_PALETTE_VADDR_END, NULL, NULL, 0);
    setSpriteShrinkFactor(LICENSED_BY_NINTENDO_1, titleScreenContext.centerCoordinate + 224.0f, 80.0f, 64.0f);
    setSpriteScale(LICENSED_BY_NINTENDO_1, 1.0f, 1.0f, 1.0f);
    func_8002BE14(LICENSED_BY_NINTENDO_1, 0.0f, 0.0f, 0.0f);
    func_8002C7EC(LICENSED_BY_NINTENDO_1, 3);
    func_8002C914(LICENSED_BY_NINTENDO_1, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002CB24(LICENSED_BY_NINTENDO_1, 1);
    func_8002B80C(LICENSED_BY_NINTENDO_1, 0x12, 0xFE);
    
    func_8002B138(LICENSED_BY_NINTENDO_2, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesIndexSegmentRomStart, &_titleSpritesIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_END, (void*)TITLE_SCREEN_PALETTE_VADDR_START, (void*)TITLE_SCREEN_PALETTE_VADDR_END, NULL, NULL, 0);
    setSpriteShrinkFactor(LICENSED_BY_NINTENDO_2, titleScreenContext.centerCoordinate - 64.0f, 72.0f, 64.0f);
    setSpriteScale(LICENSED_BY_NINTENDO_2, 1.0f, 1.0f, 1.0f);
    func_8002BE14(LICENSED_BY_NINTENDO_2, 0.0f, 0.0f, 0.0f);
    func_8002C7EC(LICENSED_BY_NINTENDO_2, 3);
    func_8002C914(LICENSED_BY_NINTENDO_2, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002CB24(LICENSED_BY_NINTENDO_2, 1);
    func_8002B80C(LICENSED_BY_NINTENDO_2, 0x12, 0xFE);

    func_8002B138(CLOUD_3_2, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesIndexSegmentRomStart, &_titleSpritesIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_END, (void*)TITLE_SCREEN_PALETTE_VADDR_START, (void*)TITLE_SCREEN_PALETTE_VADDR_END, NULL, NULL, 0);
    setSpriteShrinkFactor(CLOUD_3_2, titleScreenContext.centerCoordinate - 192.0f, 108.0f, 64.0f);
    setSpriteScale(CLOUD_3_2, 1.0f, 1.0f, 1.0f);
    func_8002BE14(CLOUD_3_2, 0.0f, 0.0f, 0.0f);
    func_8002C7EC(CLOUD_3_2, 3);
    func_8002C914(CLOUD_3_2, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002CB24(CLOUD_3_2, 1);
    func_8002B80C(CLOUD_3_2, 0x11, 0xFE);

    func_8002B138(CLOUD_2_2, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesIndexSegmentRomStart, &_titleSpritesIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_END, (void*)TITLE_SCREEN_PALETTE_VADDR_START, (void*)TITLE_SCREEN_PALETTE_VADDR_END, NULL, NULL, 0);
    setSpriteShrinkFactor(CLOUD_2_2, titleScreenContext.centerCoordinate - 288.0f, 88.0f, 64.0f);
    setSpriteScale(CLOUD_2_2, 1.0f, 1.0f, 1.0f);
    func_8002BE14(CLOUD_2_2, 0.0f, 0.0f, 0.0f);
    func_8002C7EC(CLOUD_2_2, 3);
    func_8002C914(CLOUD_2_2, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002CB24(CLOUD_2_2, 1);
    func_8002B80C(CLOUD_2_2, 0x10, 0xFE);
    
    func_8002B138(FAR_GRASS_1_1, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesIndexSegmentRomStart, &_titleSpritesIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_END, (void*)TITLE_SCREEN_PALETTE_VADDR_START, (void*)TITLE_SCREEN_PALETTE_VADDR_END, NULL, NULL, 0);
    setSpriteShrinkFactor(FAR_GRASS_1_1, titleScreenContext.centerCoordinate + 160.0f, 0.0f, 8.0f);
    setSpriteScale(FAR_GRASS_1_1, 1.0f, 1.0f, 1.0f);
    func_8002BE14(FAR_GRASS_1_1, 0.0f, 0.0f, 0.0f);
    func_8002C7EC(FAR_GRASS_1_1, 3);
    func_8002C914(FAR_GRASS_1_1, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002CB24(FAR_GRASS_1_1, 1);
    func_8002B80C(FAR_GRASS_1_1, 0xA, 0xFE);

    func_8002B138(FAR_GRASS_1_2, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesIndexSegmentRomStart, &_titleSpritesIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_END, (void*)TITLE_SCREEN_PALETTE_VADDR_START, (void*)TITLE_SCREEN_PALETTE_VADDR_END, NULL, NULL, 0);
    setSpriteShrinkFactor(FAR_GRASS_1_2, titleScreenContext.centerCoordinate - 160.0f, 0.0f, 8.0f);
    setSpriteScale(FAR_GRASS_1_2, 1.0f, 1.0f, 1.0f);
    func_8002BE14(FAR_GRASS_1_2, 0.0f, 0.0f, 0.0f);
    func_8002C7EC(FAR_GRASS_1_2, 3);
    func_8002C914(FAR_GRASS_1_2, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002CB24(FAR_GRASS_1_2, 1);
    func_8002B80C(FAR_GRASS_1_2, 0xA, 0xFE);
    
    func_8002B138(FAR_GRASS_2_1, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesIndexSegmentRomStart, &_titleSpritesIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_END, (void*)TITLE_SCREEN_PALETTE_VADDR_START, (void*)TITLE_SCREEN_PALETTE_VADDR_END, NULL, NULL, 0);
    setSpriteShrinkFactor(FAR_GRASS_2_1, titleScreenContext.centerCoordinate + 160.0f, 0.0f, 16.0f);
    setSpriteScale(FAR_GRASS_2_1, 1.0f, 1.0f, 1.0f);
    func_8002BE14(FAR_GRASS_2_1, 0.0f, 0.0f, 0.0f);
    func_8002C7EC(FAR_GRASS_2_1, 3);
    func_8002C914(FAR_GRASS_2_1, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002CB24(FAR_GRASS_2_1, 1);
    func_8002B80C(FAR_GRASS_2_1, 0xB, 0xFE);

    func_8002B138(FAR_GRASS_3_1, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesIndexSegmentRomStart, &_titleSpritesIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_END, (void*)TITLE_SCREEN_PALETTE_VADDR_START, (void*)TITLE_SCREEN_PALETTE_VADDR_END, NULL, NULL, 0);
    setSpriteShrinkFactor(FAR_GRASS_3_1, titleScreenContext.centerCoordinate + 160.0f, 0.0f, 24.0f);
    setSpriteScale(FAR_GRASS_3_1, 1.0f, 1.0f, 1.0f);
    func_8002BE14(FAR_GRASS_3_1, 0.0f, 0.0f, 0.0f);
    func_8002C7EC(FAR_GRASS_3_1, 3);
    func_8002C914(FAR_GRASS_3_1, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002CB24(FAR_GRASS_3_1, 1);
    func_8002B80C(FAR_GRASS_3_1, 0xC, 0xFE);

    func_8002B138(FRONT_GRASS_2_1, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesIndexSegmentRomStart, &_titleSpritesIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_END, (void*)TITLE_SCREEN_PALETTE_VADDR_START, (void*)TITLE_SCREEN_PALETTE_VADDR_END, NULL, NULL, 0);
    setSpriteShrinkFactor(FRONT_GRASS_2_1, titleScreenContext.centerCoordinate + 160.0f, 0.0f, 32.0f);
    setSpriteScale(FRONT_GRASS_2_1, 1.0f, 1.0f, 1.0f);
    func_8002BE14(FRONT_GRASS_2_1, 0.0f, 0.0f, 0.0f);
    func_8002C7EC(FRONT_GRASS_2_1, 3);
    func_8002C914(FRONT_GRASS_2_1, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002CB24(FRONT_GRASS_2_1, 1);
    func_8002B80C(FRONT_GRASS_2_1, 0xE, 0xFE);
    
    func_8002B138(BACK_GRASS_2, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesIndexSegmentRomStart, &_titleSpritesIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_END, (void*)TITLE_SCREEN_PALETTE_VADDR_START, (void*)TITLE_SCREEN_PALETTE_VADDR_END, NULL, NULL, 0);
    setSpriteShrinkFactor(BACK_GRASS_2, titleScreenContext.centerCoordinate - 160.0f, 0.0f, 16.0f);
    setSpriteScale(BACK_GRASS_2, 1.0f, 1.0f, 1.0f);
    func_8002BE14(BACK_GRASS_2, 0.0f, 0.0f, 0.0f);
    func_8002C7EC(BACK_GRASS_2, 3);
    func_8002C914(BACK_GRASS_2, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002CB24(BACK_GRASS_2, 1);
    func_8002B80C(BACK_GRASS_2, 0xB, 0xFE);
    
    func_8002B138(BACK_GRASS_3_2, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesIndexSegmentRomStart, &_titleSpritesIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_END, (void*)TITLE_SCREEN_PALETTE_VADDR_START, (void*)TITLE_SCREEN_PALETTE_VADDR_END, NULL, NULL, 0);
    setSpriteShrinkFactor(BACK_GRASS_3_2, titleScreenContext.centerCoordinate - 160.0f, 0.0f, 24.0f);
    setSpriteScale(BACK_GRASS_3_2, 1.0f, 1.0f, 1.0f);
    func_8002BE14(BACK_GRASS_3_2, 0.0f, 0.0f, 0.0f);
    func_8002C7EC(BACK_GRASS_3_2, 3);
    func_8002C914(BACK_GRASS_3_2, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002CB24(BACK_GRASS_3_2, 1);
    func_8002B80C(BACK_GRASS_3_2, 0xC, 0xFE);
    
    func_8002B138(FRONT_GRASS_2_2, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesIndexSegmentRomStart, &_titleSpritesIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_END, (void*)TITLE_SCREEN_PALETTE_VADDR_START, (void*)TITLE_SCREEN_PALETTE_VADDR_END, NULL, NULL, 0);
    setSpriteShrinkFactor(FRONT_GRASS_2_2, titleScreenContext.centerCoordinate - 160.0f, 0.0f, 32.0f);
    setSpriteScale(FRONT_GRASS_2_2, 1.0f, 1.0f, 1.0f);
    func_8002BE14(FRONT_GRASS_2_2, 0.0f, 0.0f, 0.0f);
    func_8002C7EC(FRONT_GRASS_2_2, 3);
    func_8002C914(FRONT_GRASS_2_2, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002CB24(FRONT_GRASS_2_2, 1);
    func_8002B80C(FRONT_GRASS_2_2, 0xE, 0xFE);
    
    func_8002B138(PUSH_THE_START_SIGN_2, &_dialogueIconsTextureSegmentRomStart, &_dialogueIconsTextureSegmentRomEnd, &_dialogueIconsIndexSegmentRomStart, &_dialogueIconsIndexSegmentRomEnd, NULL, NULL, (void*)0x8023B400, NULL, (void*)0x8023CC00, (void*)0x8023CE00, (void*)0x8023D200, NULL, NULL, 0);
    setSpriteShrinkFactor(PUSH_THE_START_SIGN_2, -54.0f, -24.0f, 56.0f);
    setSpriteScale(PUSH_THE_START_SIGN_2, 1.0f, 1.0f, 1.0f);
    func_8002BE14(PUSH_THE_START_SIGN_2, 0.0f, 0.0f, 0.0f);
    func_8002C7EC(PUSH_THE_START_SIGN_2, 3);
    func_8002C914(PUSH_THE_START_SIGN_2, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002CB24(PUSH_THE_START_SIGN_2, 1);
    func_8002B80C(PUSH_THE_START_SIGN_2, 3, 0xFE);
    
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
    
    func_800ACB04(TITLE_THEME);
    setSongWithVolume(TITLE_THEME, SONG_VOLUME);

}

//INCLUDE_ASM(const s32, "title", loadDogTitleSprite);

void loadDogTitleSprite(void) {
    func_8002B138(DOG_TITLE_SPRITE, &_dogTitleTextureSegmentRomStart, &_dogTitleTextureSegmentRomEnd, &_dogTitleAssetsIndexSegmentRomStart, &_dogTitleAssetsIndexSegmentRomEnd, &_dogTitleSpritesheetIndexSegmentRomStart, &_dogTitleSpritesheetIndexSegmentRomEnd, (void*)0x80240B00, (void*)0x80243B00, (void*)0x80246B00, (void*)0x8024AB00, (void*)0x80252300, (void*)0x80252B00, 1, 0);
    setSpriteShrinkFactor(DOG_TITLE_SPRITE, 64.0f, -80.0f, 56.0f);
    setSpriteScale(DOG_TITLE_SPRITE, 1.0f, 1.0f, 1.0f);
    func_8002BE14(DOG_TITLE_SPRITE, 0, 0, 0);
    func_8002C7EC(DOG_TITLE_SPRITE, 3);
    func_8002C914(DOG_TITLE_SPRITE, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002B80C(DOG_TITLE_SPRITE, 0x1D, 0xFE);
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

        func_8002C1C0(HARVEST_MOON_64_LOGO, 0, 0, 0, 0, 8);
        func_8002C1C0(MARVCOME_VICTOR_COPYRIGHT_INFO, 0, 0, 0, 0, 8);
        func_8002C1C0(DOG_TITLE_SPRITE, 0, 0, 0, 0, 8);
        func_8002C1C0(PUSH_THE_START_SIGN_2, 0, 0, 0, 0, 8);
        func_8002C1C0(PUSH_THE_START_SIGN_1, 0, 0, 0, 0, 8);
        func_8002C1C0(HOW_TO_PLAY_SIGN, 0, 0, 0, 0, 8);
        func_8002C1C0(DIRT_ROAD, 0, 0, 0, 0, 8);
        func_8002C1C0(SIGN_POST_1, 0, 0, 0, 0, 8);
        func_8002C1C0(SIGN_POST_2, 0, 0, 0, 0, 8);
        func_8002C1C0(SIGN_POST_3, 0, 0, 0, 0, 8);
        func_8002C1C0(FRONT_GRASS_2_1, 0, 0, 0, 0, 8);
        func_8002C1C0(FAR_GRASS_3_1, 0, 0, 0, 0, 8);
        func_8002C1C0(FAR_GRASS_2_1, 0, 0, 0, 0, 8);
        func_8002C1C0(FRONT_GRASS_2_2, 0, 0, 0, 0, 8);
        func_8002C1C0(BACK_GRASS_3_2, 0, 0, 0, 0, 8);
        func_8002C1C0(BACK_GRASS_2, 0, 0, 0, 0, 8);
        func_8002C1C0(FAR_GRASS_1_1, 0, 0, 0, 0, 8);
        func_8002C1C0(FAR_GRASS_1_2, 0, 0, 0, 0, 8);
        func_8002C1C0(CLOUD_1_1, 0, 0, 0, 0, 8);
        func_8002C1C0(CLOUD_1_2, 0, 0, 0, 0, 8);
        func_8002C1C0(CLOUD_2_1, 0, 0, 0, 0, 8);
        func_8002C1C0(CLOUD_3_1, 0, 0, 0, 0, 8);
        func_8002C1C0(LICENSED_BY_NINTENDO_1, 0, 0, 0, 0, 8);
        func_8002C1C0(LICENSED_BY_NINTENDO_2, 0, 0, 0, 0, 8);
        func_8002C1C0(CLOUD_3_2, 0, 0, 0, 0, 8);
        func_8002C1C0(CLOUD_2_2, 0, 0, 0, 0, 8);

        setSongWithDefaultSpeed(TITLE_THEME);

        titleScreenContext.mode = 5;

        return 1;

    }
    
}

// jtbl_80123658
//INCLUDE_ASM(const s32, "title", titleScreenMainLoopCallback);

// main loop callback
void titleScreenMainLoopCallback(void) {

    bool set = FALSE;
    
    switch (titleScreenContext.mode) {

        case 0:

            set = handleFrameIncrement();

            if (func_8004D380(CONTROLLER_1, BUTTON_START)) {

                if (!set) {
                    
                    func_8002BAD8(PUSH_THE_START_SIGN_1);
                    func_8002B80C(PUSH_THE_START_SIGN_1, 4, 0xFF);
                    func_8002BAD8(DOG_TITLE_SPRITE);
                    func_8002BAD8(PUSH_THE_START_SIGN_2);
                    func_8002B80C(DOG_TITLE_SPRITE, 8, 0xFE);
    
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

            func_8002BAD8(DOG_TITLE_SPRITE);
            func_8002B80C(DOG_TITLE_SPRITE, 0x1D, 0xFE);
            setSpriteShrinkFactor(PUSH_THE_START_SIGN_2, -56.0f, -20.0f, 56.0f);
            func_8002B80C(PUSH_THE_START_SIGN_2, 3, 0xFE);
            
            titleScreenContext.mode = 3;
            titleScreenContext.rowHighlighted = 0;

            break;
        
        case 3:

            if (func_8004D3C8(CONTROLLER_1, BUTTON_STICK_UP)) {        

                if (!set) {
                    set = inline1();
                }
            }
            
            if (func_8004D3C8(CONTROLLER_1, 0x100000)) {

                if (!set) {
                    set = inline2();
                }
            }
            
            if (func_8004D380(CONTROLLER_1, BUTTON_A | BUTTON_START)) {

                if (!set) {
                    
                    if (titleScreenContext.rowHighlighted == 0) {
                        func_8002BAD8(HOW_TO_PLAY_SIGN);
                        func_8002B80C(HOW_TO_PLAY_SIGN, 6, 0xFF);
                    } else {
                        func_8002BAD8(DIRT_ROAD);
                        func_8002B80C(DIRT_ROAD, 8, 0xFF);
                    }
                      
                    func_8002C1C0(HARVEST_MOON_64_LOGO, 0, 0, 0, 0, 8);
                    func_8002C1C0(MARVCOME_VICTOR_COPYRIGHT_INFO, 0, 0, 0, 0, 8);
                    func_8002C1C0(DOG_TITLE_SPRITE, 0, 0, 0, 0, 8);
                    func_8002C1C0(PUSH_THE_START_SIGN_2, 0, 0, 0, 0, 8);
                    func_8002C1C0(PUSH_THE_START_SIGN_1, 0, 0, 0, 0, 8);
                    func_8002C1C0(HOW_TO_PLAY_SIGN, 0, 0, 0, 0, 8);
                    func_8002C1C0(DIRT_ROAD, 0, 0, 0, 0, 8);
                    func_8002C1C0(SIGN_POST_1, 0, 0, 0, 0, 8);
                    func_8002C1C0(SIGN_POST_2, 0, 0, 0, 0, 8);
                    func_8002C1C0(SIGN_POST_3, 0, 0, 0, 0, 8);
                    func_8002C1C0(FRONT_GRASS_2_1, 0, 0, 0, 0, 8);
                    func_8002C1C0(FAR_GRASS_3_1, 0, 0, 0, 0, 8);
                    func_8002C1C0(FAR_GRASS_2_1, 0, 0, 0, 0, 8);
                    func_8002C1C0(FRONT_GRASS_2_2, 0, 0, 0, 0, 8);
                    func_8002C1C0(BACK_GRASS_3_2, 0, 0, 0, 0, 8);
                    func_8002C1C0(BACK_GRASS_2, 0, 0, 0, 0, 8);
                    func_8002C1C0(FAR_GRASS_1_1, 0, 0, 0, 0, 8);
                    func_8002C1C0(FAR_GRASS_1_2, 0, 0, 0, 0, 8);
                    func_8002C1C0(CLOUD_1_1, 0, 0, 0, 0, 8);
                    func_8002C1C0(CLOUD_1_2, 0, 0, 0, 0, 8);
                    func_8002C1C0(CLOUD_2_1, 0, 0, 0, 0, 8);
                    func_8002C1C0(CLOUD_3_1, 0, 0, 0, 0, 8);
                    func_8002C1C0(LICENSED_BY_NINTENDO_1, 0, 0, 0, 0, 8);
                    func_8002C1C0(LICENSED_BY_NINTENDO_2, 0, 0, 0, 0, 8);
                    func_8002C1C0(CLOUD_3_2, 0, 0, 0, 0, 8);
                    func_8002C1C0(CLOUD_2_2, 0, 0, 0, 0, 8);
    
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
                func_8002B80C(DOG_TITLE_SPRITE, 0xB, 0xFF);
                titleScreenContext.dogAnimationIndex = 0xFF;
                break;
            case 1:
                func_8002B80C(DOG_TITLE_SPRITE, 0x13, 0xFF);
                titleScreenContext.dogAnimationIndex = 0xFF;
                break;
            case 2:
                func_8002B80C(DOG_TITLE_SPRITE, 0x2D, 0xFF);
                titleScreenContext.dogAnimationIndex = 0xFF;
                break;
            case 3:
                func_8002B80C(DOG_TITLE_SPRITE, 0x32, 0xFF);
                titleScreenContext.dogAnimationIndex = 0xFF;
                break;            
            case 4:
                func_8002B80C(DOG_TITLE_SPRITE, 0x48, 0xFF);
                titleScreenContext.dogAnimationIndex = 0xFF;
                break;         
            case 5:
                func_8002B80C(DOG_TITLE_SPRITE, 0x63, 0xFF);
                titleScreenContext.dogAnimationIndex = 0xFF;
                break;            
            case 6:
                func_8002B80C(DOG_TITLE_SPRITE, 0x69, 0xFF);
                titleScreenContext.dogAnimationIndex = 0xFF;
                break;            
            default:
               func_8002B80C(DOG_TITLE_SPRITE, 0x1D, 0xFE);
                if (titleScreenContext.dogIdleCounter >= 0x1F) {
                    titleScreenContext.dogAnimationIndex = getRandomNumberInRange(0, 10);
                    titleScreenContext.dogIdleCounter = 0;
                }
                break;
        }
    
        titleScreenContext.dogIdleCounter++;
}

// jtbl_80123670
//INCLUDE_ASM(const s32, "title", func_800E0ED4);

void func_800E0ED4(void) {

    if (func_8002CC44(DOG_TITLE_SPRITE)) {
        func_8002BAD8(DOG_TITLE_SPRITE);
        handleUpdateDogAnimation();
    }
}

// alternate
/*
void func_800E0ED4(void) {

    if (func_8002CC44(0x44)) {
        
        func_8002BAD8(0x44);

        switch (D_80126572) {
            case 0:
                func_8002B80C(0x44, 0xB, 0xFF);
                D_80126572 = 0xFF;
                break;
            case 1:
                func_8002B80C(0x44, 0x13, 0xFF);
                D_80126572 = 0xFF;
                break;
            case 2:
                func_8002B80C(0x44, 0x2D, 0xFF);
                D_80126572 = 0xFF;
                break;
            case 3:
                func_8002B80C(0x44, 0x32, 0xFF);
                D_80126572 = 0xFF;
                break;            
            case 4:
                func_8002B80C(0x44, 0x48, 0xFF);
                D_80126572 = 0xFF;
                break;         
            case 5:
                func_8002B80C(0x44, 0x63, 0xFF);
                D_80126572 = 0xFF;
                break;            
            case 6:
                func_8002B80C(0x44, 0x69, 0xFF);
                D_80126572 = 0xFF;
                break;            
            default:
               func_8002B80C(0x44, 0x1D, 0xFE);
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
//INCLUDE_ASM(const s32, "title", updateGrassAnimation);

// update grass
void updateGrassAnimation(void) {

    // check flag 0x40
    if (func_8002CC44(FAR_GRASS_3_1)) {
        
        if (getRandomNumberInRange(0, 1)) {
            
            // toggle bit 2
            func_8002BAD8(FAR_GRASS_3_1);
            func_8002BAD8(FRONT_GRASS_2_1);
            func_8002BAD8(BACK_GRASS_3_2);
            func_8002BAD8(FRONT_GRASS_2_2);

            // update animation
            func_8002B80C(FAR_GRASS_3_1, 0xC, 0xFE);
            func_8002B80C(FRONT_GRASS_2_1, 0xE, 0xFE);
            func_8002B80C(BACK_GRASS_3_2, 0xC, 0xFE);
            func_8002B80C(FRONT_GRASS_2_2, 0xE, 0xFE);

        } else {
            
            func_8002BAD8(FAR_GRASS_3_1);
            func_8002BAD8(FRONT_GRASS_2_1);
            func_8002BAD8(BACK_GRASS_3_2);
            func_8002BAD8(FRONT_GRASS_2_2);

            func_8002B80C(FAR_GRASS_3_1, 0xD, 0xFE);
            func_8002B80C(FRONT_GRASS_2_1, 0xF, 0xFE);
            func_8002B80C(BACK_GRASS_3_2, 0xD, 0xFE);
            func_8002B80C(FRONT_GRASS_2_2, 0xF, 0xFE);
        }
    }
}

//INCLUDE_ASM(const s32, "title", func_800E10E8);

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