#include "common.h"

#include "game/title.h"

#include "ld_symbols.h"

#include "system/controller.h"  
#include "system/dialogue.h"
#include "system/globalSprites.h"
#include "system/math.h"
#include "system/message.h"
#include "system/sprite.h"
                                                        
#include "game/cutscenes.h"         
#include "game/gameAudio.h"
#include "game/gameFile.h"      
#include "game/transition.h"

#include "mainLoop.h" 

#include "assetIndices/cutscenes.h"
#include "assetIndices/maps.h"
#include "assetIndices/sequences.h"
#include "assetIndices/sfxs.h"
#include "assetIndices/sprites.h"

#include "buffers/buffers.h"

// bss
static TitleContext titleScreenContext;

u8 demoCutsceneIndex;

// forward declarations
void handleDogAnimation();                                  
void updateGrassAnimation();                                  
void resetWrappingSpritePositions();                                  
void loadDogTitleSprite(void);

void initializeTitleScreen(u8 arg0) {
    
    titleScreenContext.rowHighlighted = 0;
    titleScreenContext.dogAnimationIndex = 0;
    titleScreenContext.unk_4 = 0;
    titleScreenContext.dogIdleCounter = 0;
    titleScreenContext.centerCoordinate = -160.0f;

    dmaSprite(HARVEST_MOON_64_LOGO, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesAssetsIndexSegmentRomStart, &_titleSpritesAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)TITLE_SCREEN_SPRITES_TEXTURE_BUFFER, NULL, (u16*)TITLE_SCREEN_SPRITES_PALETTE_BUFFER, (AnimationFrameMetadata*)TITLE_SCREEN_SPRITE_ANIMATION_FRAME_METADATA_BUFFER, (u32*)TITLE_SCREEN_SPRITES_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(HARVEST_MOON_64_LOGO, 0.0f, 64.0f, 64.0f);
    setSpriteScale(HARVEST_MOON_64_LOGO, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(HARVEST_MOON_64_LOGO, 0.0f, 0.0f, 0.0f);
    setSpriteBlendMode(HARVEST_MOON_64_LOGO, SPRITE_BLEND_ALPHA_DECAL);
    setSpriteColor(HARVEST_MOON_64_LOGO, 255, 255, 255, 255);
    setBilinearFiltering(HARVEST_MOON_64_LOGO, TRUE);
    startSpriteAnimation(HARVEST_MOON_64_LOGO, 0, 0xFE);

    dmaSprite(PUSH_THE_START_SIGN_1, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesAssetsIndexSegmentRomStart, &_titleSpritesAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)TITLE_SCREEN_SPRITES_TEXTURE_BUFFER, NULL, (u16*)TITLE_SCREEN_SPRITES_PALETTE_BUFFER, (AnimationFrameMetadata*)TITLE_SCREEN_SPRITE_ANIMATION_FRAME_METADATA_BUFFER, (u32*)TITLE_SCREEN_SPRITES_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(PUSH_THE_START_SIGN_1, titleScreenContext.centerCoordinate + 160.0f, -16.0f, 48.0f);
    setSpriteScale(PUSH_THE_START_SIGN_1, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(PUSH_THE_START_SIGN_1, 0.0f, 0.0f, 0.0f);
    setSpriteBlendMode(PUSH_THE_START_SIGN_1, SPRITE_BLEND_ALPHA_DECAL);
    setSpriteColor(PUSH_THE_START_SIGN_1, 255, 255, 255, 255);
    setBilinearFiltering(PUSH_THE_START_SIGN_1, TRUE);
    startSpriteAnimation(PUSH_THE_START_SIGN_1, 3, 0xFE);

    dmaSprite(PLAY_SIGN, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesAssetsIndexSegmentRomStart, &_titleSpritesAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)TITLE_SCREEN_SPRITES_TEXTURE_BUFFER, NULL, (u16*)TITLE_SCREEN_SPRITES_PALETTE_BUFFER, (AnimationFrameMetadata*)TITLE_SCREEN_SPRITE_ANIMATION_FRAME_METADATA_BUFFER, (u32*)TITLE_SCREEN_SPRITES_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(PLAY_SIGN, titleScreenContext.centerCoordinate - 160.0f, -12.0f, 48.0f);
    setSpriteScale(PLAY_SIGN, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(PLAY_SIGN, 0.0f, 0.0f, 0.0f);
    setSpriteBlendMode(PLAY_SIGN, SPRITE_BLEND_ALPHA_DECAL);
    setSpriteColor(PLAY_SIGN, 255, 255, 255, 255);
    setBilinearFiltering(PLAY_SIGN, TRUE);
    startSpriteAnimation(PLAY_SIGN, 5, 0xFE);

    dmaSprite(HOW_TO_PLAY_SIGN, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesAssetsIndexSegmentRomStart, &_titleSpritesAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)TITLE_SCREEN_SPRITES_TEXTURE_BUFFER, NULL, (u16*)TITLE_SCREEN_SPRITES_PALETTE_BUFFER, (AnimationFrameMetadata*)TITLE_SCREEN_SPRITE_ANIMATION_FRAME_METADATA_BUFFER, (u32*)TITLE_SCREEN_SPRITES_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(HOW_TO_PLAY_SIGN, titleScreenContext.centerCoordinate - 160.0f, -44.0f, 48.0f);
    setSpriteScale(HOW_TO_PLAY_SIGN, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(HOW_TO_PLAY_SIGN, 0.0f, 0.0f, 0.0f);
    setSpriteBlendMode(HOW_TO_PLAY_SIGN, SPRITE_BLEND_ALPHA_DECAL);
    setSpriteColor(HOW_TO_PLAY_SIGN, 255, 255, 255, 255);
    setBilinearFiltering(HOW_TO_PLAY_SIGN, TRUE);
    startSpriteAnimation(HOW_TO_PLAY_SIGN, 7, 0xFE);

    dmaSprite(SIGN_POST_1, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesAssetsIndexSegmentRomStart, &_titleSpritesAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)TITLE_SCREEN_SPRITES_TEXTURE_BUFFER, NULL, (u16*)TITLE_SCREEN_SPRITES_PALETTE_BUFFER, (AnimationFrameMetadata*)TITLE_SCREEN_SPRITE_ANIMATION_FRAME_METADATA_BUFFER, (u32*)TITLE_SCREEN_SPRITES_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(SIGN_POST_1, titleScreenContext.centerCoordinate + 160.0f, -32.0f, 40.0f);
    setSpriteScale(SIGN_POST_1, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(SIGN_POST_1, 0.0f, 0.0f, 0.0f);
    setSpriteBlendMode(SIGN_POST_1, SPRITE_BLEND_ALPHA_DECAL);
    setSpriteColor(SIGN_POST_1, 255, 255, 255, 255);
    setBilinearFiltering(SIGN_POST_1, TRUE);
    startSpriteAnimation(SIGN_POST_1, 2, 0xFE);

    dmaSprite(SIGN_POST_2, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesAssetsIndexSegmentRomStart, &_titleSpritesAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)TITLE_SCREEN_SPRITES_TEXTURE_BUFFER, NULL, (u16*)TITLE_SCREEN_SPRITES_PALETTE_BUFFER, (AnimationFrameMetadata*)TITLE_SCREEN_SPRITE_ANIMATION_FRAME_METADATA_BUFFER, (u32*)TITLE_SCREEN_SPRITES_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(SIGN_POST_2, titleScreenContext.centerCoordinate - 192.0f, -32.0f, 40.0f);
    setSpriteScale(SIGN_POST_2, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(SIGN_POST_2, 0.0f, 0.0f, 0.0f);
    setSpriteBlendMode(SIGN_POST_2, SPRITE_BLEND_ALPHA_DECAL);
    setSpriteColor(SIGN_POST_2, 255, 255, 255, 255);
    setBilinearFiltering(SIGN_POST_2, TRUE);
    startSpriteAnimation(SIGN_POST_2, 2, 0xFE);

    dmaSprite(SIGN_POST_3, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesAssetsIndexSegmentRomStart, &_titleSpritesAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)TITLE_SCREEN_SPRITES_TEXTURE_BUFFER, NULL, (u16*)TITLE_SCREEN_SPRITES_PALETTE_BUFFER, (AnimationFrameMetadata*)TITLE_SCREEN_SPRITE_ANIMATION_FRAME_METADATA_BUFFER, (u32*)TITLE_SCREEN_SPRITES_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(SIGN_POST_3, titleScreenContext.centerCoordinate - 128.0f, -32.0f, 40.0f);
    setSpriteScale(SIGN_POST_3, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(SIGN_POST_3, 0.0f, 0.0f, 0.0f);
    setSpriteBlendMode(SIGN_POST_3, SPRITE_BLEND_ALPHA_DECAL);
    setSpriteColor(SIGN_POST_3, 255, 255, 255, 255);
    setBilinearFiltering(SIGN_POST_3, TRUE);
    startSpriteAnimation(SIGN_POST_3, 2, 0xFE);

    dmaSprite(SKY_1, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesAssetsIndexSegmentRomStart, &_titleSpritesAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)TITLE_SCREEN_SPRITES_TEXTURE_BUFFER, NULL, (u16*)TITLE_SCREEN_SPRITES_PALETTE_BUFFER, (AnimationFrameMetadata*)TITLE_SCREEN_SPRITE_ANIMATION_FRAME_METADATA_BUFFER, (u32*)TITLE_SCREEN_SPRITES_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(SKY_1, titleScreenContext.centerCoordinate + 160.0f, 0.0f, 56.0f);
    setSpriteScale(SKY_1, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(SKY_1, 0.0f, 0.0f, 0.0f);
    setSpriteBlendMode(SKY_1, SPRITE_BLEND_ALPHA_DECAL);
    setSpriteColor(SKY_1, 255, 255, 255, 255);
    setBilinearFiltering(SKY_1, TRUE);
    startSpriteAnimation(SKY_1, 9, 0xFE);

    dmaSprite(SKY_1_2, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesAssetsIndexSegmentRomStart, &_titleSpritesAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)TITLE_SCREEN_SPRITES_TEXTURE_BUFFER, NULL, (u16*)TITLE_SCREEN_SPRITES_PALETTE_BUFFER, (AnimationFrameMetadata*)TITLE_SCREEN_SPRITE_ANIMATION_FRAME_METADATA_BUFFER, (u32*)TITLE_SCREEN_SPRITES_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(SKY_1_2, titleScreenContext.centerCoordinate - 160.0f, 0.0f, 56.0f);
    setSpriteScale(SKY_1_2, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(SKY_1_2, 0.0f, 0.0f, 0.0f);
    setSpriteBlendMode(SKY_1_2, SPRITE_BLEND_ALPHA_DECAL);
    setSpriteColor(SKY_1_2, 255, 255, 255, 255);
    setBilinearFiltering(SKY_1_2, TRUE);
    startSpriteAnimation(SKY_1_2, 9, 0xFE);
    
    dmaSprite(CLOUD_1_1, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesAssetsIndexSegmentRomStart, &_titleSpritesAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)TITLE_SCREEN_SPRITES_TEXTURE_BUFFER, NULL, (u16*)TITLE_SCREEN_SPRITES_PALETTE_BUFFER, (AnimationFrameMetadata*)TITLE_SCREEN_SPRITE_ANIMATION_FRAME_METADATA_BUFFER, (u32*)TITLE_SCREEN_SPRITES_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(CLOUD_1_1, titleScreenContext.centerCoordinate + 160.0f, 64.0f, 64.0f);
    setSpriteScale(CLOUD_1_1, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(CLOUD_1_1, 0.0f, 0.0f, 0.0f);
    setSpriteBlendMode(CLOUD_1_1, SPRITE_BLEND_ALPHA_DECAL);
    setSpriteColor(CLOUD_1_1, 255, 255, 255, 255);
    setBilinearFiltering(CLOUD_1_1, TRUE);
    startSpriteAnimation(CLOUD_1_1, 16, 0xFE);
    
    dmaSprite(CLOUD_2_1, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesAssetsIndexSegmentRomStart, &_titleSpritesAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)TITLE_SCREEN_SPRITES_TEXTURE_BUFFER, NULL, (u16*)TITLE_SCREEN_SPRITES_PALETTE_BUFFER, (AnimationFrameMetadata*)TITLE_SCREEN_SPRITE_ANIMATION_FRAME_METADATA_BUFFER, (u32*)TITLE_SCREEN_SPRITES_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(CLOUD_2_1, titleScreenContext.centerCoordinate + 32.0f, 96.0f, 64.0f);
    setSpriteScale(CLOUD_2_1, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(CLOUD_2_1, 0.0f, 0.0f, 0.0f);
    setSpriteBlendMode(CLOUD_2_1, SPRITE_BLEND_ALPHA_DECAL);
    setSpriteColor(CLOUD_2_1, 255, 255, 255, 255);
    setBilinearFiltering(CLOUD_2_1, TRUE);
    startSpriteAnimation(CLOUD_2_1, 17, 0xFE);
    
    dmaSprite(CLOUD_3_1, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesAssetsIndexSegmentRomStart, &_titleSpritesAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)TITLE_SCREEN_SPRITES_TEXTURE_BUFFER, NULL, (u16*)TITLE_SCREEN_SPRITES_PALETTE_BUFFER, (AnimationFrameMetadata*)TITLE_SCREEN_SPRITE_ANIMATION_FRAME_METADATA_BUFFER, (u32*)TITLE_SCREEN_SPRITES_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(CLOUD_3_1, titleScreenContext.centerCoordinate + 224.0f, 80.0f, 64.0f);
    setSpriteScale(CLOUD_3_1, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(CLOUD_3_1, 0.0f, 0.0f, 0.0f);
    setSpriteBlendMode(CLOUD_3_1, SPRITE_BLEND_ALPHA_DECAL);
    setSpriteColor(CLOUD_3_1, 255, 255, 255, 255);
    setBilinearFiltering(CLOUD_3_1, TRUE);
    startSpriteAnimation(CLOUD_3_1, 18, 0xFE);
    
    dmaSprite(CLOUD_3_2, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesAssetsIndexSegmentRomStart, &_titleSpritesAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)TITLE_SCREEN_SPRITES_TEXTURE_BUFFER, NULL, (u16*)TITLE_SCREEN_SPRITES_PALETTE_BUFFER, (AnimationFrameMetadata*)TITLE_SCREEN_SPRITE_ANIMATION_FRAME_METADATA_BUFFER, (u32*)TITLE_SCREEN_SPRITES_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(CLOUD_3_2, titleScreenContext.centerCoordinate - 64.0f, 72.0f, 64.0f);
    setSpriteScale(CLOUD_3_2, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(CLOUD_3_2, 0.0f, 0.0f, 0.0f);
    setSpriteBlendMode(CLOUD_3_2, SPRITE_BLEND_ALPHA_DECAL);
    setSpriteColor(CLOUD_3_2, 255, 255, 255, 255);
    setBilinearFiltering(CLOUD_3_2, TRUE);
    startSpriteAnimation(CLOUD_3_2, 18, 0xFE);

    dmaSprite(CLOUD_2_2, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesAssetsIndexSegmentRomStart, &_titleSpritesAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)TITLE_SCREEN_SPRITES_TEXTURE_BUFFER, NULL, (u16*)TITLE_SCREEN_SPRITES_PALETTE_BUFFER, (AnimationFrameMetadata*)TITLE_SCREEN_SPRITE_ANIMATION_FRAME_METADATA_BUFFER, (u32*)TITLE_SCREEN_SPRITES_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(CLOUD_2_2, titleScreenContext.centerCoordinate - 192.0f, 108.0f, 64.0f);
    setSpriteScale(CLOUD_2_2, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(CLOUD_2_2, 0.0f, 0.0f, 0.0f);
    setSpriteBlendMode(CLOUD_2_2, SPRITE_BLEND_ALPHA_DECAL);
    setSpriteColor(CLOUD_2_2, 255, 255, 255, 255);
    setBilinearFiltering(CLOUD_2_2, TRUE);
    startSpriteAnimation(CLOUD_2_2, 17, 0xFE);

    dmaSprite(CLOUD_1_2, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesAssetsIndexSegmentRomStart, &_titleSpritesAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)TITLE_SCREEN_SPRITES_TEXTURE_BUFFER, NULL, (u16*)TITLE_SCREEN_SPRITES_PALETTE_BUFFER, (AnimationFrameMetadata*)TITLE_SCREEN_SPRITE_ANIMATION_FRAME_METADATA_BUFFER, (u32*)TITLE_SCREEN_SPRITES_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(CLOUD_1_2, titleScreenContext.centerCoordinate - 288.0f, 88.0f, 64.0f);
    setSpriteScale(CLOUD_1_2, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(CLOUD_1_2, 0.0f, 0.0f, 0.0f);
    setSpriteBlendMode(CLOUD_1_2, SPRITE_BLEND_ALPHA_DECAL);
    setSpriteColor(CLOUD_1_2, 255, 255, 255, 255);
    setBilinearFiltering(CLOUD_1_2, TRUE);
    startSpriteAnimation(CLOUD_1_2, 16, 0xFE);
    
    dmaSprite(DIRT_ROAD_1, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesAssetsIndexSegmentRomStart, &_titleSpritesAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)TITLE_SCREEN_SPRITES_TEXTURE_BUFFER, NULL, (u16*)TITLE_SCREEN_SPRITES_PALETTE_BUFFER, (AnimationFrameMetadata*)TITLE_SCREEN_SPRITE_ANIMATION_FRAME_METADATA_BUFFER, (u32*)TITLE_SCREEN_SPRITES_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(DIRT_ROAD_1, titleScreenContext.centerCoordinate + 160.0f, 0.0f, 8.0f);
    setSpriteScale(DIRT_ROAD_1, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(DIRT_ROAD_1, 0.0f, 0.0f, 0.0f);
    setSpriteBlendMode(DIRT_ROAD_1, SPRITE_BLEND_ALPHA_DECAL);
    setSpriteColor(DIRT_ROAD_1, 255, 255, 255, 255);
    setBilinearFiltering(DIRT_ROAD_1, TRUE);
    startSpriteAnimation(DIRT_ROAD_1, 10, 0xFE);

    dmaSprite(DIRT_ROAD_2, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesAssetsIndexSegmentRomStart, &_titleSpritesAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)TITLE_SCREEN_SPRITES_TEXTURE_BUFFER, NULL, (u16*)TITLE_SCREEN_SPRITES_PALETTE_BUFFER, (AnimationFrameMetadata*)TITLE_SCREEN_SPRITE_ANIMATION_FRAME_METADATA_BUFFER, (u32*)TITLE_SCREEN_SPRITES_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(DIRT_ROAD_2, titleScreenContext.centerCoordinate - 160.0f, 0.0f, 8.0f);
    setSpriteScale(DIRT_ROAD_2, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(DIRT_ROAD_2, 0.0f, 0.0f, 0.0f);
    setSpriteBlendMode(DIRT_ROAD_2, SPRITE_BLEND_ALPHA_DECAL);
    setSpriteColor(DIRT_ROAD_2, 255, 255, 255, 255);
    setBilinearFiltering(DIRT_ROAD_2, TRUE);
    startSpriteAnimation(DIRT_ROAD_2, 10, 0xFE);
    
    dmaSprite(FAR_GRASS_1_1, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesAssetsIndexSegmentRomStart, &_titleSpritesAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)TITLE_SCREEN_SPRITES_TEXTURE_BUFFER, NULL, (u16*)TITLE_SCREEN_SPRITES_PALETTE_BUFFER, (AnimationFrameMetadata*)TITLE_SCREEN_SPRITE_ANIMATION_FRAME_METADATA_BUFFER, (u32*)TITLE_SCREEN_SPRITES_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(FAR_GRASS_1_1, titleScreenContext.centerCoordinate + 160.0f, 0.0f, 16.0f);
    setSpriteScale(FAR_GRASS_1_1, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(FAR_GRASS_1_1, 0.0f, 0.0f, 0.0f);
    setSpriteBlendMode(FAR_GRASS_1_1, SPRITE_BLEND_ALPHA_DECAL);
    setSpriteColor(FAR_GRASS_1_1, 255, 255, 255, 255);
    setBilinearFiltering(FAR_GRASS_1_1, TRUE);
    startSpriteAnimation(FAR_GRASS_1_1, 11, 0xFE);

    dmaSprite(FAR_GRASS_2_1, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesAssetsIndexSegmentRomStart, &_titleSpritesAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)TITLE_SCREEN_SPRITES_TEXTURE_BUFFER, NULL, (u16*)TITLE_SCREEN_SPRITES_PALETTE_BUFFER, (AnimationFrameMetadata*)TITLE_SCREEN_SPRITE_ANIMATION_FRAME_METADATA_BUFFER, (u32*)TITLE_SCREEN_SPRITES_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(FAR_GRASS_2_1, titleScreenContext.centerCoordinate + 160.0f, 0.0f, 24.0f);
    setSpriteScale(FAR_GRASS_2_1, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(FAR_GRASS_2_1, 0.0f, 0.0f, 0.0f);
    setSpriteBlendMode(FAR_GRASS_2_1, SPRITE_BLEND_ALPHA_DECAL);
    setSpriteColor(FAR_GRASS_2_1, 255, 255, 255, 255);
    setBilinearFiltering(FAR_GRASS_2_1, TRUE);
    startSpriteAnimation(FAR_GRASS_2_1, 12, 0xFE);

    dmaSprite(NEAR_GRASS_1, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesAssetsIndexSegmentRomStart, &_titleSpritesAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)TITLE_SCREEN_SPRITES_TEXTURE_BUFFER, NULL, (u16*)TITLE_SCREEN_SPRITES_PALETTE_BUFFER, (AnimationFrameMetadata*)TITLE_SCREEN_SPRITE_ANIMATION_FRAME_METADATA_BUFFER, (u32*)TITLE_SCREEN_SPRITES_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(NEAR_GRASS_1, titleScreenContext.centerCoordinate + 160.0f, 0.0f, 32.0f);
    setSpriteScale(NEAR_GRASS_1, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(NEAR_GRASS_1, 0.0f, 0.0f, 0.0f);
    setSpriteBlendMode(NEAR_GRASS_1, SPRITE_BLEND_ALPHA_DECAL);
    setSpriteColor(NEAR_GRASS_1, 255, 255, 255, 255);
    setBilinearFiltering(NEAR_GRASS_1, TRUE);
    startSpriteAnimation(NEAR_GRASS_1, 14, 0xFE);
    
    dmaSprite(FAR_GRASS_1_2, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesAssetsIndexSegmentRomStart, &_titleSpritesAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)TITLE_SCREEN_SPRITES_TEXTURE_BUFFER, NULL, (u16*)TITLE_SCREEN_SPRITES_PALETTE_BUFFER, (AnimationFrameMetadata*)TITLE_SCREEN_SPRITE_ANIMATION_FRAME_METADATA_BUFFER, (u32*)TITLE_SCREEN_SPRITES_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(FAR_GRASS_1_2, titleScreenContext.centerCoordinate - 160.0f, 0.0f, 16.0f);
    setSpriteScale(FAR_GRASS_1_2, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(FAR_GRASS_1_2, 0.0f, 0.0f, 0.0f);
    setSpriteBlendMode(FAR_GRASS_1_2, SPRITE_BLEND_ALPHA_DECAL);
    setSpriteColor(FAR_GRASS_1_2, 255, 255, 255, 255);
    setBilinearFiltering(FAR_GRASS_1_2, TRUE);
    startSpriteAnimation(FAR_GRASS_1_2, 11, 0xFE);
    
    dmaSprite(FAR_GRASS_2_2, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesAssetsIndexSegmentRomStart, &_titleSpritesAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)TITLE_SCREEN_SPRITES_TEXTURE_BUFFER, NULL, (u16*)TITLE_SCREEN_SPRITES_PALETTE_BUFFER, (AnimationFrameMetadata*)TITLE_SCREEN_SPRITE_ANIMATION_FRAME_METADATA_BUFFER, (u32*)TITLE_SCREEN_SPRITES_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(FAR_GRASS_2_2, titleScreenContext.centerCoordinate - 160.0f, 0.0f, 24.0f);
    setSpriteScale(FAR_GRASS_2_2, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(FAR_GRASS_2_2, 0.0f, 0.0f, 0.0f);
    setSpriteBlendMode(FAR_GRASS_2_2, SPRITE_BLEND_ALPHA_DECAL);
    setSpriteColor(FAR_GRASS_2_2, 255, 255, 255, 255);
    setBilinearFiltering(FAR_GRASS_2_2, TRUE);
    startSpriteAnimation(FAR_GRASS_2_2, 12, 0xFE);
    
    dmaSprite(FAR_GRASS_3, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesAssetsIndexSegmentRomStart, &_titleSpritesAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)TITLE_SCREEN_SPRITES_TEXTURE_BUFFER, NULL, (u16*)TITLE_SCREEN_SPRITES_PALETTE_BUFFER, (AnimationFrameMetadata*)TITLE_SCREEN_SPRITE_ANIMATION_FRAME_METADATA_BUFFER, (u32*)TITLE_SCREEN_SPRITES_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(FAR_GRASS_3, titleScreenContext.centerCoordinate - 160.0f, 0.0f, 32.0f);
    setSpriteScale(FAR_GRASS_3, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(FAR_GRASS_3, 0.0f, 0.0f, 0.0f);
    setSpriteBlendMode(FAR_GRASS_3, SPRITE_BLEND_ALPHA_DECAL);
    setSpriteColor(FAR_GRASS_3, 255, 255, 255, 255);
    setBilinearFiltering(FAR_GRASS_3, TRUE);
    startSpriteAnimation(FAR_GRASS_3, 14, 0xFE);
    
    dmaSprite(PUSH_THE_START_SIGN_2, &_dialogueButtonIconsTextureSegmentRomStart, &_dialogueButtonIconsTextureSegmentRomEnd, &_dialogueButtonIconsAssetsIndexSegmentRomStart, &_dialogueButtonIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)DIALOGUE_ICON_TEXTURE_BUFFER, NULL, (u8*)DIALOGUE_ICON_PALETTE_BUFFER, (AnimationFrameMetadata*)DIALOGUE_ICON_ANIMATION_FRAME_METADATA_BUFFER, (u32*)DIALOGUE_ICON_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(PUSH_THE_START_SIGN_2, -54.0f, -24.0f, 56.0f);
    setSpriteScale(PUSH_THE_START_SIGN_2, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(PUSH_THE_START_SIGN_2, 0.0f, 0.0f, 0.0f);
    setSpriteBlendMode(PUSH_THE_START_SIGN_2, SPRITE_BLEND_ALPHA_DECAL);
    setSpriteColor(PUSH_THE_START_SIGN_2, 255, 255, 255, 255);
    setBilinearFiltering(PUSH_THE_START_SIGN_2, TRUE);
    startSpriteAnimation(PUSH_THE_START_SIGN_2, 3, 0xFE);
    
    setMainLoopCallbackFunctionIndex(TITLE_SCREEN);
    
    if (arg0 != 0) {

        titleScreenContext.mode = 2;

        adjustSpriteViewSpacePosition(PUSH_THE_START_SIGN_1, 320.0f, 0.0f, 0.0f);
        adjustSpriteViewSpacePosition(PLAY_SIGN, 320.0f, 0.0f, 0.0f);
        adjustSpriteViewSpacePosition(HOW_TO_PLAY_SIGN, 320.0f, 0.0f, 0.0f);
        adjustSpriteViewSpacePosition(SIGN_POST_1, 320.0f, 0.0f, 0.0f);
        adjustSpriteViewSpacePosition(SIGN_POST_2, 320.0f, 0.0f, 0.0f);
        adjustSpriteViewSpacePosition(SIGN_POST_3, 320.0f, 0.0f, 0.0f);
        adjustSpriteViewSpacePosition(DIRT_ROAD_1, 320.0f, 0.0f, 0.0f);
        adjustSpriteViewSpacePosition(DIRT_ROAD_2, 320.0f, 0.0f, 0.0f);
        adjustSpriteViewSpacePosition(FAR_GRASS_1_1, 128.0f, 0.0f, 0.0f);
        adjustSpriteViewSpacePosition(FAR_GRASS_2_1, 192.0f, 0.0f, 0.0f);
        adjustSpriteViewSpacePosition(NEAR_GRASS_1, 256.0f, 0.0f, 0.0f);
        adjustSpriteViewSpacePosition(FAR_GRASS_1_2, 128.0f, 0.0f, 0.0f);
        adjustSpriteViewSpacePosition(FAR_GRASS_2_2, 192.0f, 0.0f, 0.0f);
        adjustSpriteViewSpacePosition(FAR_GRASS_3, 256.0f, 0.0f, 0.0f);
        adjustSpriteViewSpacePosition(SKY_1, 64.0f, 0.0f, 0.0f);
        adjustSpriteViewSpacePosition(SKY_1_2, 64.0f, 0.0f, 0.0f);
        adjustSpriteViewSpacePosition(CLOUD_1_1, 64.0f, 0.0f, 0.0f);
        adjustSpriteViewSpacePosition(CLOUD_2_1, 64.0f, 0.0f, 0.0f);
        adjustSpriteViewSpacePosition(CLOUD_3_1, 64.0f, 0.0f, 0.0f);
        adjustSpriteViewSpacePosition(CLOUD_3_2, 64.0f, 0.0f, 0.0f);
        adjustSpriteViewSpacePosition(CLOUD_2_2, 64.0f, 0.0f, 0.0f);
        adjustSpriteViewSpacePosition(CLOUD_1_2, 64.0f, 0.0f, 0.0f);

    } else {

        titleScreenContext.mode = 0;
    
    }
    
    titleScreenContext.frameCounter = 0;
    
    loadDogTitleSprite();
    
    setCurrentAudioSequence(TITLE_THEME);
    setAudioSequenceVolume(TITLE_THEME, SEQUENCE_VOLUME);

}

void loadDogTitleSprite(void) {
    dmaSprite(DOG_TITLE_SPRITE, &_dogTextureSegmentRomStart, &_dogTextureSegmentRomEnd, &_dogAssetsIndexSegmentRomStart, &_dogAssetsIndexSegmentRomEnd, &_dogSpritesheetIndexSegmentRomStart, &_dogSpritesheetIndexSegmentRomEnd, (u8*)DOG_TITLE_TEXTURE_1_BUFFER, (u8*)DOG_TITLE_TEXTURE_2_BUFFER, (u16*)DOG_TITLE_PALETTE_BUFFER, (void*)DOG_TITLE_ANIMATION_FRAME_METADATA_BUFFER, (u32*)DOG_TITLE_SPRITESHEET_INDEX_BUFFER, (u32*)DOG_TITLE_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 1, 0);
    setSpriteViewSpacePosition(DOG_TITLE_SPRITE, 64.0f, -80.0f, 56.0f);
    setSpriteScale(DOG_TITLE_SPRITE, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(DOG_TITLE_SPRITE, 0, 0, 0);
    setSpriteBlendMode(DOG_TITLE_SPRITE, SPRITE_BLEND_ALPHA_DECAL);
    setSpriteColor(DOG_TITLE_SPRITE, 255, 255, 255, 255);
    // sitting
    startSpriteAnimation(DOG_TITLE_SPRITE, 29, 0xFE);
}

static inline void deactivateTitleScreenSprites() {

    deactivateSprite(HARVEST_MOON_64_LOGO);
    deactivateSprite(DOG_TITLE_SPRITE);
    deactivateSprite(PUSH_THE_START_SIGN_2);
    deactivateSprite(PUSH_THE_START_SIGN_1);
    deactivateSprite(PLAY_SIGN);
    deactivateSprite(HOW_TO_PLAY_SIGN);
    deactivateSprite(SIGN_POST_1);
    deactivateSprite(SIGN_POST_2);
    deactivateSprite(SIGN_POST_3);
    deactivateSprite(NEAR_GRASS_1);
    deactivateSprite(FAR_GRASS_2_1);
    deactivateSprite(FAR_GRASS_1_1);
    deactivateSprite(FAR_GRASS_3);
    deactivateSprite(FAR_GRASS_2_2);
    deactivateSprite(FAR_GRASS_1_2);
    deactivateSprite(DIRT_ROAD_1);
    deactivateSprite(DIRT_ROAD_2);
    deactivateSprite(SKY_1);
    deactivateSprite(SKY_1_2);
    deactivateSprite(CLOUD_1_1);
    deactivateSprite(CLOUD_2_1);
    deactivateSprite(CLOUD_3_1);
    deactivateSprite(CLOUD_3_2);
    deactivateSprite(CLOUD_2_2);
    deactivateSprite(CLOUD_1_2);

}

static inline bool handleFrameIncrement() {

    titleScreenContext.frameCounter++;
    
    // close title screen after timeout
    if (titleScreenContext.frameCounter == 500) {

        updateSpriteRGBA(HARVEST_MOON_64_LOGO, 0, 0, 0, 0, 8);
        updateSpriteRGBA(DOG_TITLE_SPRITE, 0, 0, 0, 0, 8);
        updateSpriteRGBA(PUSH_THE_START_SIGN_2, 0, 0, 0, 0, 8);
        updateSpriteRGBA(PUSH_THE_START_SIGN_1, 0, 0, 0, 0, 8);
        updateSpriteRGBA(PLAY_SIGN, 0, 0, 0, 0, 8);
        updateSpriteRGBA(HOW_TO_PLAY_SIGN, 0, 0, 0, 0, 8);
        updateSpriteRGBA(SIGN_POST_1, 0, 0, 0, 0, 8);
        updateSpriteRGBA(SIGN_POST_2, 0, 0, 0, 0, 8);
        updateSpriteRGBA(SIGN_POST_3, 0, 0, 0, 0, 8);
        updateSpriteRGBA(NEAR_GRASS_1, 0, 0, 0, 0, 8);
        updateSpriteRGBA(FAR_GRASS_2_1, 0, 0, 0, 0, 8);
        updateSpriteRGBA(FAR_GRASS_1_1, 0, 0, 0, 0, 8);
        updateSpriteRGBA(FAR_GRASS_3, 0, 0, 0, 0, 8);
        updateSpriteRGBA(FAR_GRASS_2_2, 0, 0, 0, 0, 8);
        updateSpriteRGBA(FAR_GRASS_1_2, 0, 0, 0, 0, 8);
        updateSpriteRGBA(DIRT_ROAD_1, 0, 0, 0, 0, 8);
        updateSpriteRGBA(DIRT_ROAD_2, 0, 0, 0, 0, 8);
        updateSpriteRGBA(SKY_1, 0, 0, 0, 0, 8);
        updateSpriteRGBA(SKY_1_2, 0, 0, 0, 0, 8);
        updateSpriteRGBA(CLOUD_1_1, 0, 0, 0, 0, 8);
        updateSpriteRGBA(CLOUD_2_1, 0, 0, 0, 0, 8);
        updateSpriteRGBA(CLOUD_3_1, 0, 0, 0, 0, 8);
        updateSpriteRGBA(CLOUD_3_2, 0, 0, 0, 0, 8);
        updateSpriteRGBA(CLOUD_2_2, 0, 0, 0, 0, 8);
        updateSpriteRGBA(CLOUD_1_2, 0, 0, 0, 0, 8);

        stopAudioSequenceWithDefaultFadeOut(TITLE_THEME);

        titleScreenContext.mode = 5;

        return 1;

    }
    
}

void titleScreenMainLoopCallback(void) {

    bool set = FALSE;
    
    switch (titleScreenContext.mode) {

        // initial title screen, waiting to push start
        case 0:

            // FIXME
            set = handleFrameIncrement();

            if (checkButtonPressed(CONTROLLER_1, BUTTON_START)) {

                if (!set) {
                    
                    resetAnimationState(PUSH_THE_START_SIGN_1);
                    // bounce sign
                    startSpriteAnimation(PUSH_THE_START_SIGN_1, 4, 0xFF);
                    
                    resetAnimationState(DOG_TITLE_SPRITE);
                    resetAnimationState(PUSH_THE_START_SIGN_2);
                    // running
                    startSpriteAnimation(DOG_TITLE_SPRITE, 8, 0xFE);
    
                    titleScreenContext.mode = 1;
                    playSfx(SELECT_1_SFX);

                }
            }

            handleDogAnimation();
            break;
        
        // move to other sign
        case 1:

            if (titleScreenContext.unk_4 < 64) {

                adjustSpriteViewSpacePosition(PUSH_THE_START_SIGN_1, 5.0f, 0.0f, 0.0f);
                adjustSpriteViewSpacePosition(PLAY_SIGN, 5.0f, 0, 0);
                adjustSpriteViewSpacePosition(HOW_TO_PLAY_SIGN, 5.0f, 0, 0);
                adjustSpriteViewSpacePosition(SIGN_POST_1, 5.0f, 0, 0);
                adjustSpriteViewSpacePosition(SIGN_POST_2, 5.0f, 0, 0);
                adjustSpriteViewSpacePosition(SIGN_POST_3, 5.0f, 0, 0);
                adjustSpriteViewSpacePosition(DIRT_ROAD_1, 5.0f, 0, 0);
                adjustSpriteViewSpacePosition(DIRT_ROAD_2, 5.0f, 0, 0);
                adjustSpriteViewSpacePosition(FAR_GRASS_1_1, 2.0f, 0, 0);
                adjustSpriteViewSpacePosition(FAR_GRASS_2_1, 3.0f, 0, 0);
                adjustSpriteViewSpacePosition(NEAR_GRASS_1, 5.0f, 0, 0);
                adjustSpriteViewSpacePosition(FAR_GRASS_1_2, 2.0f, 0, 0);
                adjustSpriteViewSpacePosition(FAR_GRASS_2_2, 3.0f, 0, 0);
                adjustSpriteViewSpacePosition(FAR_GRASS_3, 5.0f, 0, 0);
                adjustSpriteViewSpacePosition(SKY_1, 1.0f, 0, 0);
                adjustSpriteViewSpacePosition(SKY_1_2, 1.0f, 0, 0);
                adjustSpriteViewSpacePosition(CLOUD_1_1, 1.0f, 0, 0);
                adjustSpriteViewSpacePosition(CLOUD_2_1, 1.0f, 0, 0);
                adjustSpriteViewSpacePosition(CLOUD_3_1, 1.0f, 0, 0);
                adjustSpriteViewSpacePosition(CLOUD_3_2, 1.0f, 0, 0);
                adjustSpriteViewSpacePosition(CLOUD_2_2, 1.0f, 0, 0);
                adjustSpriteViewSpacePosition(CLOUD_1_2, 1.0f, 0, 0);

            } else {
                titleScreenContext.mode = 2;
                titleScreenContext.unk_4 = 0;
            }

            titleScreenContext.unk_4++;
            
            break;
        
        // load sign
        case 2:

            resetAnimationState(DOG_TITLE_SPRITE);
            // sitting
            startSpriteAnimation(DOG_TITLE_SPRITE, 29, 0xFE);

            setSpriteViewSpacePosition(PUSH_THE_START_SIGN_2, -56.0f, -20.0f, 56.0f);
            startSpriteAnimation(PUSH_THE_START_SIGN_2, 3, 0xFE);
            
            titleScreenContext.mode = 3;
            titleScreenContext.rowHighlighted = 0;

            break;
        
        // handle sign input; start game or how to play
        case 3:

            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTHWEST)) {        

                if (!set) {

                    if (titleScreenContext.rowHighlighted != 1) {
                        setSpriteViewSpacePosition(PUSH_THE_START_SIGN_2, -56.0f, -52.0f, 56.0f);
                        titleScreenContext.rowHighlighted = 1;
                        playSfx(MOVE_CURSOR_SFX);
                    }

                    set = TRUE;
    
                }
                
            }
            
            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_NORTHEAST)) {

                if (!set) {
                    
                    if (titleScreenContext.rowHighlighted != 0) {
                        setSpriteViewSpacePosition(PUSH_THE_START_SIGN_2, -56.0f, -20.0f, 56.0f);
                        titleScreenContext.rowHighlighted = 0;
                        playSfx(MOVE_CURSOR_SFX);
                    }
                    
                    set = TRUE;

                }

            }
            
            // go back to intro
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A | BUTTON_START)) {

                if (!set) {
                    
                    if (titleScreenContext.rowHighlighted == 0) {
                        resetAnimationState(PLAY_SIGN);
                        // bounce sign
                        startSpriteAnimation(PLAY_SIGN, 6, 0xFF);
                    } else {
                        resetAnimationState(HOW_TO_PLAY_SIGN);
                        // bounce sign
                        startSpriteAnimation(HOW_TO_PLAY_SIGN, 8, 0xFF);
                    }
                      
                    updateSpriteRGBA(HARVEST_MOON_64_LOGO, 0, 0, 0, 0, 8);
                    updateSpriteRGBA(DOG_TITLE_SPRITE, 0, 0, 0, 0, 8);
                    updateSpriteRGBA(PUSH_THE_START_SIGN_2, 0, 0, 0, 0, 8);
                    updateSpriteRGBA(PUSH_THE_START_SIGN_1, 0, 0, 0, 0, 8);
                    updateSpriteRGBA(PLAY_SIGN, 0, 0, 0, 0, 8);
                    updateSpriteRGBA(HOW_TO_PLAY_SIGN, 0, 0, 0, 0, 8);
                    updateSpriteRGBA(SIGN_POST_1, 0, 0, 0, 0, 8);
                    updateSpriteRGBA(SIGN_POST_2, 0, 0, 0, 0, 8);
                    updateSpriteRGBA(SIGN_POST_3, 0, 0, 0, 0, 8);
                    updateSpriteRGBA(NEAR_GRASS_1, 0, 0, 0, 0, 8);
                    updateSpriteRGBA(FAR_GRASS_2_1, 0, 0, 0, 0, 8);
                    updateSpriteRGBA(FAR_GRASS_1_1, 0, 0, 0, 0, 8);
                    updateSpriteRGBA(FAR_GRASS_3, 0, 0, 0, 0, 8);
                    updateSpriteRGBA(FAR_GRASS_2_2, 0, 0, 0, 0, 8);
                    updateSpriteRGBA(FAR_GRASS_1_2, 0, 0, 0, 0, 8);
                    updateSpriteRGBA(DIRT_ROAD_1, 0, 0, 0, 0, 8);
                    updateSpriteRGBA(DIRT_ROAD_2, 0, 0, 0, 0, 8);
                    updateSpriteRGBA(SKY_1, 0, 0, 0, 0, 8);
                    updateSpriteRGBA(SKY_1_2, 0, 0, 0, 0, 8);
                    updateSpriteRGBA(CLOUD_1_1, 0, 0, 0, 0, 8);
                    updateSpriteRGBA(CLOUD_2_1, 0, 0, 0, 0, 8);
                    updateSpriteRGBA(CLOUD_3_1, 0, 0, 0, 0, 8);
                    updateSpriteRGBA(CLOUD_3_2, 0, 0, 0, 0, 8);
                    updateSpriteRGBA(CLOUD_2_2, 0, 0, 0, 0, 8);
                    updateSpriteRGBA(CLOUD_1_2, 0, 0, 0, 0, 8);
    
                    titleScreenContext.mode = 4;
    
                    playSfx(SELECT_1_SFX);
                    
                }
                
            }

            handleDogAnimation();

            break;

        // leaving title screen
        case 4:
            
            if (checkSpriteRGBAUpdateFinished(HARVEST_MOON_64_LOGO)) {

                deactivateTitleScreenSprites();
                
                if (titleScreenContext.rowHighlighted == 0) {
                    initializeLoadGameScreen(FALSE);
                } else {
                    launchIntroCutscene(CUTSCENE_HOW_TO_PLAY_INTRO, FARM_SPAWN_POINT_1, 0);
                }
            }

            break;

        // fade out to intro cutscene after timeout 
        case 5:
            
            if (checkSpriteRGBAUpdateFinished(HARVEST_MOON_64_LOGO) && checkDefaultSequenceChannelOpen(TITLE_THEME)) {
                
                deactivateTitleScreenSprites();
 
                switch (demoCutsceneIndex) {
                    case 0:
                        launchIntroCutscene(CUTSCENE_DEMO_1, FARM_SPAWN_POINT_1, 1);
                        break;
                    case 1:
                        launchIntroCutscene(CUTSCENE_DEMO_2, MOUNTAIN_2_SPAWN_POINT_1, 1);
                        break;
                    case 2:
                        launchIntroCutscene(CUTSCENE_DEMO_3, VILLAGE_1_SPAWN_POINT_1, 1);
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
    resetWrappingSpritePositions();

}

static inline void updateDogTitleAnimation() {
    
    switch (titleScreenContext.dogAnimationIndex) {
        case 0:
            // barking
            startSpriteAnimation(DOG_TITLE_SPRITE, 11, 0xFF);
            titleScreenContext.dogAnimationIndex = 0xFF;
            break;
        case 1:
            // sniffing
            startSpriteAnimation(DOG_TITLE_SPRITE, 19, 0xFF);
            titleScreenContext.dogAnimationIndex = 0xFF;
            break;
        case 2:
            // sitting happy
            startSpriteAnimation(DOG_TITLE_SPRITE, 45, 0xFF);
            titleScreenContext.dogAnimationIndex = 0xFF;
            break;
        case 3:
            // sleeping
            startSpriteAnimation(DOG_TITLE_SPRITE, 50, 0xFF);
            titleScreenContext.dogAnimationIndex = 0xFF;
            break;            
        case 4:
            // wagging tail
            startSpriteAnimation(DOG_TITLE_SPRITE, 72, 0xFF);
            titleScreenContext.dogAnimationIndex = 0xFF;
            break;         
        case 5:
            // digging
            startSpriteAnimation(DOG_TITLE_SPRITE, 99, 0xFF);
            titleScreenContext.dogAnimationIndex = 0xFF;
            break;            
        case 6:
            // peeing
            startSpriteAnimation(DOG_TITLE_SPRITE, 105, 0xFF);
            titleScreenContext.dogAnimationIndex = 0xFF;
            break;            
        default:
            // sitting
            startSpriteAnimation(DOG_TITLE_SPRITE, 29, 0xFE);
            if (titleScreenContext.dogIdleCounter >= 31) {
                titleScreenContext.dogAnimationIndex = getRandomNumberInRange(0, 10);
                titleScreenContext.dogIdleCounter = 0;
            }
            break;
    }

    titleScreenContext.dogIdleCounter++;

}

void handleDogAnimation(void) {

    if (checkSpriteAnimationCycleEnded(DOG_TITLE_SPRITE)) {
        resetAnimationState(DOG_TITLE_SPRITE);
        updateDogTitleAnimation();
    }
    
}

void updateGrassAnimation(void) {

    if (checkSpriteAnimationCycleEnded(FAR_GRASS_2_1)) {
        
        if (getRandomNumberInRange(0, 1)) {
            
            resetAnimationState(FAR_GRASS_2_1);
            resetAnimationState(NEAR_GRASS_1);
            resetAnimationState(FAR_GRASS_2_2);
            resetAnimationState(FAR_GRASS_3);

            startSpriteAnimation(FAR_GRASS_2_1, 12, 0xFE);
            startSpriteAnimation(NEAR_GRASS_1, 14, 0xFE);
            startSpriteAnimation(FAR_GRASS_2_2, 12, 0xFE);
            startSpriteAnimation(FAR_GRASS_3, 14, 0xFE);

        } else {
            
            resetAnimationState(FAR_GRASS_2_1);
            resetAnimationState(NEAR_GRASS_1);
            resetAnimationState(FAR_GRASS_2_2);
            resetAnimationState(FAR_GRASS_3);

            startSpriteAnimation(FAR_GRASS_2_1, 13, 0xFE);
            startSpriteAnimation(NEAR_GRASS_1, 15, 0xFE);
            startSpriteAnimation(FAR_GRASS_2_2, 13, 0xFE);
            startSpriteAnimation(FAR_GRASS_3, 15, 0xFE);

        }
    }
    
}

void resetWrappingSpritePositions(void) {
    
    if (globalSprites[CLOUD_1_1].viewSpacePosition.x > 320.0f) {
        setSpriteViewSpacePosition(CLOUD_1_1, titleScreenContext.centerCoordinate - 320.0f, 64.0f, 64.0f);
    }
    if (globalSprites[CLOUD_2_1].viewSpacePosition.x > 320.0f) {
        setSpriteViewSpacePosition(CLOUD_2_1, titleScreenContext.centerCoordinate - 320.0f, 96.0f, 64.0f);
    }
    if (globalSprites[CLOUD_3_1].viewSpacePosition.x > 320.0f) {
        setSpriteViewSpacePosition(CLOUD_3_1, titleScreenContext.centerCoordinate - 320.0f, 80.0f, 64.0f);
    }
    if (globalSprites[CLOUD_3_2].viewSpacePosition.x > 320.0f) {
        setSpriteViewSpacePosition(CLOUD_3_2, titleScreenContext.centerCoordinate - 320.0f, 72.0f, 64.0f);
    }
    if (globalSprites[CLOUD_2_2].viewSpacePosition.x > 320.0f) {
        setSpriteViewSpacePosition(CLOUD_2_2, titleScreenContext.centerCoordinate - 320.0f, 108.0f, 64.0f);
    }
    if (globalSprites[CLOUD_1_2].viewSpacePosition.x > 320.0f) {
        setSpriteViewSpacePosition(CLOUD_1_2, titleScreenContext.centerCoordinate - 320.0f, 88.0f, 64.0f);
    }
    
    adjustSpriteViewSpacePosition(CLOUD_1_1, 0.2f, 0.0f, 0.0f);
    adjustSpriteViewSpacePosition(CLOUD_2_1, 0.1f, 0.0f, 0.0f);
    adjustSpriteViewSpacePosition(CLOUD_3_1, 0.3f, 0.0f, 0.0f);
    adjustSpriteViewSpacePosition(CLOUD_3_2, 0.4f, 0.0f, 0.0f);
    adjustSpriteViewSpacePosition(CLOUD_2_2, 0.1f, 0.0f, 0.0f);
    adjustSpriteViewSpacePosition(CLOUD_1_2, 0.2f, 0.0f, 0.0f);
    
}