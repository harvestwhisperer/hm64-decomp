#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "common.h"

#define MAX_ENTITIES 49
#define MAX_ENTITY_ASSETS 102

/* flags */
#define ENTITY_ACTIVE 0x1
#define ENTITY_INITIALIZED 0x4
#define ENTITY_UNKNOWN_FLAG_1 0x8 // deactivating sprite
#define ENTITY_ANIMATION_DIRECTION_CHANGE 0x10
#define ENTITY_PAUSED 0x40 
#define ENTITY_UNKNOWN_FLAG_2 0x80
#define ENTITY_UNKNOWN_FLAG_3 0x100
#define ENTITY_TRACKING_ACTIVE 0x200
#define ENTITY_UNKNOWN_FLAG_4 0x1000 // load
#define ENTITY_PALETTE_SET 0x2000

typedef struct {
	u16 animationIndex;
	u16 nextAnimationIndex;
} AnimationIndices;

// 0x801FC160
typedef struct {
	void *vaddrTexture1; // 00
	void *vaddrTexture2; // 04
	void *vaddrPalette;  // 08
	void *vaddrUnknownAsset; // 0C
	void *vaddrTextureToPaletteLookup; // 10
	void *vaddrSpritesheetIndex; // 14
	Vec3f unk_18; // 18-20
	u16 targetSpriteIndex; // 24, for tracking one sprite to another (holdable items, expression bubbles, etc.)
	u8 trackingMode; // 26
	Vec3f coordinates; // 28-30
	Vec3f unk_34; // 34-3C
	Vec3f unk_3C;
	f32 unk_4C;
	u16 entityAssetIndex; // 0x50
	u16 globalSpriteIndex;
	AnimationIndices animationIndices; // 0x54-0x58
	u16 unk_58; 
	u16	unk_5A;
	u16 collision; // 0x5C
	u16 unk_5E;
	u16 shadowSpriteIndex; // 0x60;
	u16 paletteIndex;
	u16 unk_64;
	u8 unk_66;
	u8 direction;  // 0x67
	u8 flag; // 0x68
	u16 flags; // 0x6A
} Entity;

// 0x801584F0
typedef struct {
	u16 *animationScripts; /* 0x00 */ // data from 80119750-8011BCA0, related to animation; updateEntities
	void *romTextureStart; /* 0x04 */
	void *romTextureEnd; /* 0x08 */
	void *romAssetsIndexStart; /* 0x0C */
	void *romAssetsIndexEnd; /* 0x10 */
	void *romSpritesheetIndexStart; /* 0x14 */
	void *romSpritesheetIndexEnd; /* 0x18 */
	u16 unk_1C; /* 0x1C */
	u16 unk_1E; /* 0x1E */
	u8 collisionBufferX; /* 0x20 */
	u8 collisionBufferY; /* 0x21 */
	u8 shadowSpriteIndex; /* 0x22 */
	u16 flags; /* 0x24 */ // bit 2 = asset type
} EntityAssetDescriptor;

// 0x8016FFF8
// ci4 asset layout
typedef struct {
	void *romTextureStart;
	void *romTextureEnd;
	void *romAssetsIndexStart;
	void *romAssetsIndexEnd;
	void *vaddrSpritesheet;
	void *vaddrPalette;
	void *vaddrUnknownAssetSheet;
	void *vaddrUnknownAsset2;
	u16 unk_20; // related to animation, offset
	u16 unk_22; // related to animation, arg2 of startSpriteAnimation
} ShadowSpriteDescriptor;

extern void initializeEntities(void);
extern bool initializeEntityAsset(u16 index, void* arg1, void* arg2, void* arg3, void* arg4, void* arg5, void* arg6, u8 arg7, u8 arg8, u16* arg9);
extern bool initializeEntity(u16, u16, u16, void*, void*, void*, void*, void*, void*);
extern bool initializeAnimalEntity(u16 index, void* arg1, void* arg2, void* arg3, void* arg4);
extern bool loadEntity(u16, u16, u8);      
extern bool initializeShadowSprite(u16 index, u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32* arg5, u16* arg6, u16* arg7, u16* arg8, u16 arg9, u8 argA);           
extern bool func_8002EDF0(u16 entityIndex, s16, s16, s16);
extern bool setMainMapIndex(u16 arg0);     
extern bool setEntityColor(u16, u8, u8, u8, u8);        
extern bool setEntityTrackingTarget(u16, u16, u16);             
extern bool setEntityShadow(u16, u8);
extern bool updateEntityRGBA(u16 entityIndex, u8 r, u8 g, u8 b, u8 a, s16 rate);
extern bool setEntityAnimation(u16, u16); 
extern bool setEntityDirection(u16, u8);   
extern void pauseEntities();    
extern void togglePauseEntities();               
extern void func_8002F770(s16); 
extern void setEntitiesColor(u8, u8, u8, u8);               
extern void updateEntitiesColor(u8 r, u8 g, u8 b, u8 a, s16);
extern bool deactivateEntity(u16);      
extern bool setEntityCoordinates(u16, f32, f32, f32);   
extern void func_8002FB3C();
extern bool func_8002FCB4(u16, u8);  
extern bool func_8002FD24(u16 entityIndex);
extern bool func_8002FE10(u16, f32, f32, f32, f32);           
extern bool checkEntityAnimationStateChanged(u16); 
extern bool setEntityPaletteIndex(u16 entityIndex, s16);
extern bool func_8002FF38(u16, u16);
extern bool setEntityCollisionBuffers(u16 entityIndex, u8 xValue, u8 yValue);
extern bool func_8002FFF4(u16 index, u8 arg1, u8 arg2);
extern bool func_80030054(u16, u8);                                                   
extern bool func_80030388(u16 entityIndex);        
extern void updateEntities(void);                          
extern bool func_8003019C(u16, u8);                            
extern bool func_80030240(u16, u8);                            
extern bool func_800302E4(u16, u8);         
extern u16 func_800305CC(u16 entityIndex, f32, f32, u16);
extern u8 func_800309B4(u16, f32, f32);     
extern u16 func_80030BA0(u16* ptr, u16 offset);  
extern Vec3f* func_80030EAC(Vec3f*, u16, f32, f32);                  
extern Vec3f* func_800315A0(Vec3f*, u16 index);    
extern bool checkEntityShouldPlaySoundEffect(u16 entityIndex);   
extern bool func_80031830(u16 entityIndex, s16 z, u8 direction);        
extern Vec3f func_80031904(u16 entityIndex, s16 z, u8 direction);
extern bool setEntityAnimationWithDirectionChange(u16 entityIndex, u16);

extern Entity entities[MAX_ENTITIES];
extern EntityAssetDescriptor entityAssetDescriptors[MAX_ENTITY_ASSETS];

#endif