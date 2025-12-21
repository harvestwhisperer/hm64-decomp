#ifndef _GLOBAL_SPRITES_H_
#define _GLOBAL_SPRITES_H_

#include "common.h"

#define MAX_SPRITES 192
#define MAX_SHADOW_SPRITES 3

/* global sprite rendering flags */
#define SPRITE_RENDERING_FLIP_HORIZONTAL 1
#define SPRITE_RENDERING_FLIP_VERTICAL 2

/* global sprite state flags */
#define SPRITE_ACTIVE 0x1
#define SPRITE_ANIMATION_HEADER_PROCESSED 0x2
#define SPRITE_UNKNOWN_FLAG 0x4
#define SPRITE_ANIMATION_PLAYING 0x8
#define SPRITE_ANIMATION_LOOPS 0x10
#define SPRITE_ANIMATION_PAUSED 0x20
#define SPRITE_ANIMATION_STATE_CHANGED 0x40
#define SPRITE_ENABLE_BILINEAR_FILTERING 0x80
#define SPRITE_DIRECT_PALETTE_MODE 0x100
#define SPRITE_NO_TRANSFORM 0x200
#define SPRITE_RGBA_IN_PROGRESS 0x400
#define SPRITE_DESTROY_ON_ANIMATION_END 0x800 

#define SHADOW_ALPHA 96

typedef struct {
	u16 objectCount; // overloaded field set from both header value (total frame count) and from AnimationFrameMetadata value (number of bitmap metadata objects)
	u8 frameDuration; // in game ticks
	u8 audioTrigger;
} AnimationFrameMetadata;

typedef struct {
    u16 spritesheetIndex;
    u16 unk_2; // unused
    s16 anchorX;
    s16 anchorY;
} BitmapMetadata;

// 0x801FD630
typedef struct {	
	u32 *animationIndexPtr; // 0x0
	u16 *animationDataPtr; // 0x4
	u32 *spritesheetIndexPtr; // 0x8
	u32 *paletteIndexPtr; // 0xC
	u8 *spriteToPaletteMappingPtr; // 0x10
	u8 *texturePtr[2]; // 0x14
	void *romTexturePtr; // 0x1C
	void *animationFrameMetadataPtr; // 0x20
	void *bitmapMetadataPtr; // 0x24
	AnimationFrameMetadata animationFrameMetadata; // 0x28
	Vec3f viewSpacePosition; // 0x2C
	Vec3f scale; // 0x38
	Vec3f rotation;
    Vec4f baseRGBA; // 0x50 
	Vec4f currentRGBA; // 0x60
	Vec4f targetRGBA; // 0x70
	Vec4f deltaRGBA; // 0x80
	u8 currentAnimationFrame;
	u8 frameTickCounter;
	bool audioTrigger;
    u16 paletteIndex; 
	u8 animationCounter[2];
	u16 renderingFlags;
    u16 stateFlags; // 0xCA
} SpriteObject;

extern void initializeGlobalSprites(void); 
extern bool dmaSprite(u16 index, u32 romTextureStart, u32 romTextureEnd, u32 romAssetsIndexStart, u32 romAssetsIndexEnd, u32 romSpritesheetIndexStart, u32 romSpritesheetIndexEnd, u8* texture1Vaddr, u8* texture2Vaddr, u16* paletteVaddr, AnimationFrameMetadata* animationVaddr, u8* spriteToPaletteVaddr, u32* spritesheetIndexVaddr, u8 assetType, u8 argE);
extern bool func_8002B36C(u16 index, u32* animationIndexPtr, u32* spritesheetIndexPtr, u32* paletteIndexPtr, u8* spriteToPaletteMappingPtr);
extern bool func_8002B50C(u16 index, u32* animationIndexPtr, u32* spritesheetIndexPtr, u32* paletteIndexPtr, u8* spriteToPaletteMappingPtr, u32 romTexturePtr, u8* texturePtr, u8* texture2Ptr);
extern bool deactivateSprite(u16 index);
extern void deactivateGlobalSprites(void);
extern bool startSpriteAnimation(u16 index, u16 offset, u8);
extern bool resetAnimationState(u16);   
extern bool func_8002BB30(u16);
extern bool func_8002BB88(u16); 
extern u16 func_8002BCC8(u16 index);
extern bool setSpriteFlip(u16 index, bool flipHorizontal, bool flipVertical);
extern bool setSpriteViewSpacePosition(u16 index, f32 x, f32 y, f32 z);  
extern bool setSpriteScale(u16, f32, f32, f32);                                                                                                        
extern bool adjustSpriteViewSpacePosition(u16, f32, f32, f32);    
extern bool setSpriteRotation(u16, f32, f32, f32);                        
extern bool updateSpriteRGBA(u16 index, u8 r, u8 g, u8 b, u8 a, s16 arg5);
extern bool updateSpriteAlpha(u16, u8, s16);
extern bool func_8002C680(u16 index, u16, u16);
extern bool func_8002C6F8(u16, u16);
extern bool func_8002C768(u16, u16);
extern bool setSpriteRenderingLayer(u16, u16);                              
extern bool setSpriteBaseRGBA(u16 index, u8 r, u8 g, u8 b, u8 a);
extern bool setSpriteColor(u16, u8, u8, u8, u8);
extern bool setSpriteAlpha(u16, u8);  
extern bool setBilinearFiltering(u16 spriteIndex, bool useBilinearFiltering);     
extern bool setSpritePaletteIndex(u16, u16);     
extern bool func_8002CBF8(u16);   
extern bool checkSpriteAnimationStateChanged(u16);
extern void updateSprites(void);
extern void deactivateSprites(void);

extern SpriteObject globalSprites[MAX_SPRITES];

#endif