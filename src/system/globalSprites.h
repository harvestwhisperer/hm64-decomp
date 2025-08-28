#ifndef _GLOBAL_SPRITES_H_
#define _GLOBAL_SPRITES_H_

#include "common.h"

#define MAX_SPRITES 192
#define MAX_ACTIVE_SPRITES 192
#define MAX_SHADOW_SPRITES 3

/* global sprite rendering flags */

/* global sprite state flags */
#define ACTIVE 0x1
#define ANIMATION_HEADER_PROCESSED 0x2 // read from unknownAsset3Ptr and do 16 bit swap
#define UNKNOWN_SPRITE_FLAG 0x4
#define ANIMATION_PLAYING 0x8
#define ANIMATION_LOOPS 0x10
#define ANIMATION_PAUSED 0x20
#define ANIMATION_STATE_CHANGED 0x40
#define ENABLE_BILINEAR_FILTERING 0x80
#define DIRECT_PALETTE_MODE 0x100
#define SPRITE_FLIP_WINDING 0x200
#define RGBA_IN_PROGRESS 0x400
#define DESTROY_ON_ANIMATION_END 0x800 

#define SHADOW_ALPHA 96

typedef struct {
	u16 totalFrames; // or'd indices
	u8 unk_2; // lower bits, might be duration in game frames
	u8 audioTrigger; // upper bits
} SpriteAnimation;

typedef struct {
    u16 spriteIndex;
    u16 unk_2;
    u16 textureIndex;
    u16 unk_6;
    u16 ptr;
    u32 pad[2]; 
    u32 ptr2;
    u32 unk_18; 
    u32 length;
} SpriteData;

// 0x801FD630
typedef struct {	
	u32 *animationIndexPtr; // 0x0
	void *animationDataPtr; // 0x4 // byteswapped
	u32 *spritesheetIndexPtr; // 0x8
	u32 *paletteIndexPtr; // 0xC
	u8 *spriteToPaletteMappingPtr; // 0x10 // sprite to palette mapping
	u8 *texturePtr; // 0x14
	u8 *texture2Ptr; // 0x18 // probably ptr to end of texture (per animation)
	void *romTexturePtr; // 0x1C
	void *unknownAsset3Ptr; // 0x20 // set from animationDataPtr
	void *unknownAsset4Ptr; // 0x24 // func_8002CDE8, SpriteData objects
	SpriteAnimation animation; // 0x28
	Vec3f shrink; // 0x2C
	Vec3f scale; // 0x38
	Vec3f angles;
    Vec4f baseRGBA; // 0x50 
	Vec4f rgbaCurrent; // 0x60
	Vec4f rgbaTarget; // 0x70
	Vec4f rgbaDelta; // 0x80
	u8 currentAnimationFrame; // 0x801FD6C0 counter for operation with animationDataPtr;
	u8 frameTimer; // set from SpriteAnimation unk_2
	bool audioTrigger; // 801FD6C2, set from SpriteAnimation unk_3; play audio when frame starts
    u16 paletteIndex; // 0x801FD6C4
	u8 animationCounter[2]; // 0x801FD6C6-0x801FD6C7
	u16 renderingFlags; // 0xC8
    u16 stateFlags; // 0xCA
} SpriteObject;

// typedef struct {
// 	Vec3f unk_0;
// 	Vec3f unk_C;
// 	Vec3f angles;
//     Vec4f unk_C8;
// 	Vec4f groundRgba;
// 	Vec4f defaultRgba;
// 	Vec4f unk_54;
// } UnknownStruct5;

extern void initializeGlobalSprites(void); 
extern bool dmaSprite(u16 index, u32 romTextureStart, u32 romTextureEnd, u32 romAssetIndexStart, u32 romAssetIndexEnd, u32 romSpritesheetIndexStart, u32 romSpritesheetIndexEnd, u8* texture1Vaddr, u8* texture2Vaddr, u16* paletteVaddr, u16* animationVaddr, u8* spriteToPaletteVaddr, u32* spritesheetIndexVaddr, u8 assetType, u8 argE);
extern bool func_8002B36C(u16 index, u32* animationIndexPtr, u32* spritesheetIndexPtr, u32* paletteIndexPtr, u8* spriteToPaletteMappingPtr);
extern bool func_8002B50C(u16 index, u32* animationIndexPtr, u32* spritesheetIndexPtr, u32* paletteIndexPtr, u8* spriteToPaletteMappingPtr, u32 romTexturePtr, u8* texturePtr, u8* texture2Ptr);
extern bool deactivateSprite(u16 index);
extern void deactivateGlobalSprites(void);
extern bool startSpriteAnimation(u16 index, u16 offset, u8);
extern bool resetAnimationState(u16);   
extern bool func_8002BB30(u16);
extern bool func_8002BB88(u16); 
extern bool func_8002BBE0(u16 index, u8 arg1, u8 arg2);
extern bool setSpriteShrinkFactor(u16 index, f32 x, f32 y, f32 z);  
extern bool setSpriteScale(u16, f32, f32, f32);                                                                                                        
extern bool adjustSpriteShrinkFactor(u16, f32, f32, f32);    
extern bool setSpriteAngles(u16, f32, f32, f32);                        
extern bool updateSpriteRGBA(u16 index, u8 r, u8 g, u8 b, u8 a, s16 arg5);
extern bool updateSpriteAlpha(u16, u8, s16);
extern bool func_8002C680(u16 index, u16, u16);
extern bool func_8002C6F8(u16, u16);
extern bool func_8002C768(u16, u16);
extern bool setSpriteRenderingLayer(u16, u16);                              
extern bool setSpriteDefaultRGBA(u16 index, u8 r, u8 g, u8 b, u8 a);
extern bool setSpriteColor(u16, u8, u8, u8, u8);
extern bool setSpriteAlpha(u16, u8);  
extern bool setBilinearFiltering(u16 spriteIndex, bool useBilinearFiltering);     
extern bool setSpritePaletteIndex(u16, u16);     
extern bool func_8002CBF8(u16);   
extern bool checkSpriteAnimationStateChanged(u16);
extern void updateSprites(void);
extern void deactivateSprites(void);

extern SpriteObject globalSprites[MAX_ACTIVE_SPRITES];

#endif