#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "common.h"

#include "system/globalSprites.h"

#define MAX_ENTITIES 49
#define MAX_ENTITY_ASSETS 102

/* flags */
#define ENTITY_ACTIVE 1
#define ENTITY_MOVEMENT_DISABLED 2
#define ENTITY_INITIALIZED 0x4
#define ENTITY_UNKNOWN_FLAG_1 0x8 // deactivating sprite
#define ENTITY_ANIMATION_DIRECTION_CHANGE 0x10
#define ENTITY_PAUSED 0x40 
#define ENTITY_Y_MOVEMENT 0x80
#define ENTITY_MAP_SPACE_INDEPENDENT 0x100
#define ENTITY_TRACKING_ACTIVE 0x200
#define ENTITY_NON_COLLIDABLE 0x400
#define ENTITY_TOUCHING_GROUND 0x800 // used by item entities
#define ENTITY_LOAD_PENDING 0x1000
#define ENTITY_PALETTE_SET 0x2000
#define ENTITY_COLLISION_EXEMPT 0x4000
#define ENTITY_SINGLE_COLLISION_CHECK 0x8000

typedef struct {
	u16 animationIndex;
	u16 nextAnimationIndex;
} AnimationIndices;

// 0x801FC160
typedef struct {
	void *vaddrTexture1; // 00
	void *vaddrTexture2; // 04
	void *vaddrPalette;  // 08
	void *vaddrAnimationMetadata; // 0C
	void *vaddrTextureToPaletteLookup; // 10
	void *vaddrSpritesheetIndex; // 14
	Vec3f attachmentOffset; // 18-20
	u16 targetEntityIndex; // 24, for tracking one sprite to another (holdable items, expression bubbles, etc.)
	u8 trackingMode; // 26
	Vec3f coordinates; // 28-30
	Vec3f movementVector; // 34-3C
	Vec3f viewSpacePosition;
	f32 speed;
	u16 entityAssetIndex; // 0x50
	u16 globalSpriteIndex;
	AnimationIndices animationIndices; // 0x54-0x58
	u16 entityCollidedWithIndex; 
	u16	buttonPressed;
	u16 collision; // 0x5C
	u16 unk_5E; // also tracks entity collided with, but never checked
	u16 shadowSpriteIndex; // 0x60;
	u16 paletteIndex;
	s16 yOffset;
	u8 unk_66;
	u8 direction;  // 0x67
	bool transformExempt; // 0x68
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
	u16 spriteWidth; /* 0x1C */
	u16 spriteHeight; /* 0x1E */
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
	void *vaddrAnimationMetadata;
	u16 unk_20; // related to animation, offset
	u16 unk_22; // related to animation, arg2 of startSpriteAnimation
} ShadowSpriteDescriptor;

extern void initializeEntities(void);
extern bool initializeEntity(u16 entityIndex, u16 globalSpriteIndex, u16 shadowSpriteIndex, u8* vaddrTexture1, u8* vaddrTexture2, u16* vaddrPalette, AnimationFrameMetadata* vaddrAnimationMetadata, u32* vaddrTextureToPaletteLookup, u32* vaddrSpritesheetIndex);
extern bool initializeAnimalEntity(u16 index, u16* arg1, AnimationFrameMetadata* arg2, u32* arg3, u32* arg4);
extern bool loadEntity(u16, u16, u8);      
extern bool initializeShadowSprite(u16 index, u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32* arg5, u16* arg6, u16* arg7, u16* arg8, u16 arg9, u8 argA);           
extern bool setEntityAttachmentOffset(u16 entityIndex, s16, s16, s16);
extern bool setMainMapIndex(u16 arg0);     
extern bool setEntityColor(u16, u8, u8, u8, u8);        
extern bool setEntityTrackingTarget(u16, u16, u16);             
extern bool setEntityShadow(u16, u8);
extern bool updateEntityRGBA(u16 entityIndex, u8 r, u8 g, u8 b, u8 a, s16 rate);
extern bool setEntityAnimation(u16, u16); 
extern bool setEntityDirection(u16, u8);   
extern void pauseEntities();    
extern void togglePauseEntities();               
extern void rotateAllEntities(s16); 
extern void setEntitiesColor(u8, u8, u8, u8);               
extern void updateEntitiesColor(u8 r, u8 g, u8 b, u8 a, s16);
extern bool deactivateEntity(u16);      
extern bool setEntityCoordinates(u16, f32, f32, f32);   
extern void pauseAllEntityLoads();
extern bool unpauseEntity(u16 index); 
extern bool pauseEntity(u16 index);
extern bool setEntityMapSpaceIndependent(u16, bool);  
extern bool checkEntityMapSpaceDependent(u16 entityIndex);
extern bool setEntityMovementVector(u16, f32, f32, f32, f32);           
extern bool checkEntityAnimationStateChanged(u16); 
extern bool setEntityPaletteIndex(u16 entityIndex, u16);
extern bool setCameraTrackingEntity(u16, u16);
extern bool setEntityCollisionBuffers(u16 entityIndex, u8 xValue, u8 yValue);
extern bool setEntitySpriteDimensions(u16 index, u8 arg1, u8 arg2);
extern bool setEntityYMovement(u16 entityIndex, bool flag);                                                   
extern bool checkEntityPaused(u16 entityIndex);        
extern void updateEntities(void);                          
extern bool setEntityCollidable(u16, u8);                            
extern bool setEntityTracksCollisions(u16, u8);                            
extern bool enableEntityMovement(u16, u8);         
extern u16 func_800305CC(u16 entityIndex, f32, f32, u16);
extern u8 getLevelInteractionIndexFromEntityPosition(u16, f32, f32);     
extern u16 getAnimationOffsetFromScript(u16* ptr, u16 offset);  
extern Vec3f getEntityRelativeGroundObjectCoords(u16, f32, f32);                  
extern bool func_800300F8(u16 index, u8 flag);
extern Vec3f getEntityTileCoordinates(u16 index);    
extern u8 detectEntityOverlap(Entity* entity, u16 entityIndex, f32 deltaX, f32 deltaZ, u16 collisionWidth, u16 collisionHeight);
extern bool checkEntityShouldPlaySoundEffect(u16 entityIndex);   
extern bool checkTerrainCollisionInDirection(u16 entityIndex, s16 z, u8 direction);        
extern Vec3f projectEntityPosition(u16 entityIndex, s16 z, u8 direction);
extern bool setEntityAnimationWithDirectionChange(u16 entityIndex, u16);

extern Entity entities[MAX_ENTITIES];
extern EntityAssetDescriptor entityAssetDescriptors[MAX_ENTITY_ASSETS];

#endif