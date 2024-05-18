#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "common.h"

#define MAX_SPRITES 192
#define MAX_SPRITE_INFO 40
#define MAX_RENDERED_SPRITES 49
#define MAX_ACTIVE_SPRITES 192
#define MAX_SHADOW_SPRITES 3
#define MAX_CHARACTER_SPRITES 102
#define MAX_BITMAPS 176

#define NEED_DMA 1

#define OBJECT_SPRITE_BANK_1_START 0x802EB800

#define DOG_TITLE_SPRITE 0x44

typedef struct {
	u16 animationIndex;
	u16 nextAnimationIndex;
} Animation;

typedef struct {
	u16 animation; // or'd indices
	u8 unk_2; // lower bits
	u8 unk_3; // upper bits
} SpriteAnimation;

typedef struct {
    u16 unk_0;
    u8 unk_2;
    u8 unk_3;
    u32 unk_4;
    u32 unk_8;
} UnknownAnimation;

// 0x801FD630
typedef struct {	
	u32 *unknownAssetIndexPtr; // 0x0
	void *unknownAssetPtr; // 0x4 // byteswapped
	u32 *spritesheetIndexPtr; // 0x8
	u32 *paletteIndexPtr; // 0xC
	u8 *spriteToPaletteMappingPtr; // 0x10 // sprite to palette mapping
	u8 *texturePtr; // 0x14
	u8 *texture2Ptr; // 0x18
	void *romTexturePtr; // 0x1C
	void *unknownAsset3Ptr; // 0x20 // set from unknownAssetPtr
	void *unknownAsset4Ptr; // 0x24 // func_8002CDE8
	SpriteAnimation animation; // 0x28
	Vec3f shrink; // 0x2C
	Vec3f scale; // 0x38
	Vec3f unk_44;
    Vec4f rgba; // 0x50 
	Vec4f rgbaCurrent; // 0x60
	Vec4f rgbaDefault; // 0x70
	Vec4f normalized; // 0x80
	u8 unk_90; // counter for operation with unknownAssetPtr
	u8 unk_91;
	u8 unk_92; // audio
    u16 paletteIndex;
	u8 animationCounter1;
    u8 animationCounter2;
	u16 flags1;
    u16 flags2;
} Sprite;

typedef struct {
	void *romAddr;
	void *vaddr;
	u32 length;
	u16 flags;
} SpriteInfo;

// 0x801584F0
typedef struct {
	u16 *vaddr; /* 0x00 */ // offset indices; data from 80119750-8011BCA0, related to animation
	void *romTextureStart; /* 0x04 */
	void *romTextureEnd; /* 0x08 */
	void *romAssetIndexStart; /* 0x0C */
	void *romAssetIndexEnd; /* 0x10 */
	void *romSpritesheetIndexStart; /* 0x14 */
	void *romSpritesheetIndexEnd; /* 0x18 */
	u16 unk_1C; /* 0x1C */
	u16 unk_1E; /* 0x1E */
	u8 collisionBufferX; /* 0x20 */
	u8 collisionBufferY; /* 0x21 */
	u8 shadowSpriteIndex; /* 0x22 */
	u16 flags; /* 0x24 */ // bit 2 = asset type
} CharacterSprite;

// 0x801FC160
typedef struct {
	void *vaddrTexture1; // 00
	void *vaddrTexture2; // 04
	void *vaddrPalette;  // 08
	void *vaddrUnknownAsset; // 0C
	void *vaddrTextureToPaletteLookup; // 10
	void *vaddrSpritesheetIndex; // 14
	Vec3f unk_18; // 18-24
	u16 unk_24; // 24
	u8 unk_26; // 26
	Vec3f startingCoordinates; // 28-34
	Vec3f currentCoordinates; // 34-40
	Vec3f unk_3C; // 40-4C
	f32 unk_4C;
	u16 characterIndex; // 0x50
	u16 globalSpriteIndex;
	Animation anim; // 0x54-0x58
	u16 unk_58; 
	u16	unk_5A;
	u16 collision; // 0x5C
	u16 unk_5E;
	u16 spriteOffset; // 0x60;
	u16 unk_62;
	u16 unk_64;
	u8 unk_66;
	u8 direction;  // 0x67
	u8 flag; // 0x68
	u16 flags; // 0x6A
} RenderedSprite;

// 0x801F7110
typedef struct {
	u8 *timg; // 00
	u16 *pal;  // 04
	s32 width;  // 08 
	s32 height; // 0C
	u32 fmt; // 10
	s32 pixelSize; // 14
	u16 spriteNumber; // 18
	u16 vtxIndex; // 1A
	Vec3f unk_1C; // set from decompressed level Vec3fs
	Vec3f scaling; // 28
	Vec3f unk_34; // 34 // angles
	Vec4f rgba; // 40
    s16 unk_50;
	u16 unk_52;
	u16 unk_54; // flags for microcodes, vertex type, pixel density
	u16 flags; //56
} Bitmap;

// 0x8016FFF8
// ci4 asset layout
typedef struct {
	void *romTextureStart;
	void *romTextureEnd;
	void *romAssetIndexStart;
	void *romAssetIndexEnd;
	void *vaddrSpritesheet;
	void *vaddrPalette;
	void *vaddrUnknownAssetSheet;
	void *vaddrUnknownAsset2;
	u16 unk_20;
	u16 unk_22;
} Shadow;

typedef struct {
	Vec3f unk_0;
	Vec3f unk_C;
	Vec3f angles;
    Vec4f unk_C8;
	Vec4f groundRgba;
	Vec4f defaultRgba;
	Vec4f unk_54;
} UnknownStruct5;


/* renderedSprites.c */
extern void initializeNpcSpriteStructs(void);
extern bool func_8002DEE8(u16, u16, u16, void*, void*, void*, void*, void*, void*);
extern bool func_8002E284(u16, u16, u8);      
extern bool func_8002EC18(u16 index, u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32* arg5, u16* arg6, u16* arg7, u16* arg8, u16 arg9, u8 argA);           
extern bool func_8002EDF0(u16 index, s16, s16, s16);
extern bool func_8002EEA4(u16 arg0);     
extern bool func_8002F014(u16, u8, u8, u8, u8);        
extern bool func_8002ECD4(u16, u16, u16);             
extern bool func_8002F114(u16, u8);
extern bool func_8002F1E0(u16 index, u8 r, u8 g, u8 b, u8 a, s16 arg5);
extern bool func_8002F2FC(u16, u16); 
extern bool setSpriteDirection(u16, u8);   
extern void func_8002F6F0();    
extern void func_8002F730();               
extern void func_8002F770(s16); 
extern void func_8002F7C8(u8, u8, u8, u8);               
extern void func_8002F8F0(u8 r, u8 g, u8 b, u8 a, s16);
extern bool func_8002FA2C(u16);      
extern bool func_8002FD80(u16, f32, f32, f32);   
extern void func_8002FB3C();
extern bool func_8002FCB4(u16, u8);  
extern bool func_8002FD24(u16 index);
extern bool func_8002FE10(u16, f32, f32, f32, f32);           
extern bool func_8002FECC(u16); 
extern bool func_8002ED80(u16 index, s16);
extern bool func_8002FF38(u16, u16);
extern bool func_80030054(u16, u8);                                                   
extern bool func_80030388(u16 index);        
extern void func_80033058(void);                           
extern bool func_8003019C(u16, u8);                            
extern bool func_80030240(u16, u8);                            
extern bool func_800302E4(u16, u8);         
extern u16 func_800305CC(u16 index, f32, f32, u16);
extern bool func_800309B4(u16, f32, f32);     
extern u16 func_80030BA0(u16* ptr, u16 offset);  
extern Vec3f* func_80030EAC(Vec3f*, u16, f32, f32);                  
extern Vec3f* func_800315A0(Vec3f*, u16 index);    
extern bool func_80031380(u16);   
extern bool func_80031830(u16, u32, u8);        
extern Vec3f* func_80031904(Vec3f* vec, u16 index, s16, u8);
extern bool setSpriteAnimation(u16 index, u16);

/* globalSprites.c */
extern void initializeGlobalSprites(void); 
extern bool func_8002B138(u16 index, u32 romTextureStart, u32 romTextureEnd, u32 romAssetIndexStart, u32 romAssetIndexEnd, u32 romSpritesheetIndexStart, u32 romSpritesheetIndexEnd, u8* texture1Vaddr, u8* texture2Vaddr, u16* paletteVaddr, u16* animationVaddr, u8* spriteToPaletteVaddr, u32* spritesheetIndexVaddr, u8 assetType, u8 argE);
extern bool func_8002B36C(u16 index, u32* unknownAssetIndexPtr, u32* spritesheetIndexPtr, u32* paletteIndexPtr, u8* spriteToPaletteMappingPtr);
extern bool func_8002B50C(u16 index, u32* unknownAssetIndexPtr, u32* spritesheetIndexPtr, u32* paletteIndexPtr, u8* spriteToPaletteMappingPtr, u32 romTexturePtr, u8* texturePtr, u8* texture2Ptr);
extern bool func_8002B6B8(u16 index);
extern void func_8002B710(void);
extern bool func_8002B80C(u16 index, u16 offset, u8);
extern bool func_8002BAD8(u16);   
extern bool func_8002BB30(u16);
extern bool func_8002BB88(u16); 
extern bool func_8002BD0C(u16 index, f32 x, f32 y, f32 z);  
extern bool func_8002BD90(u16, f32, f32, f32);                                                                                                        
extern bool func_8002BE98(u16, f32, f32, f32);    
extern bool func_8002BE14(u16, f32, f32, f32);                        
extern bool func_8002C1C0(u16 index, u8 r, u8 g, u8 b, u8 a, s16 arg5);
extern bool func_8002C52C(u16, u8, s16);
extern bool func_8002C680(u16 index, u16, u16);
extern bool func_8002C6F8(u16, u16);
extern bool func_8002C768(u16, u16);
extern bool func_8002C7EC(u16, u16);                              
extern bool func_8002C85C(u16 index, u8 r, u8 g, u8 b, u8 a);
extern bool func_8002C914(u16, u8, u8, u8, u8);
extern bool func_8002CAA8(u16, u8);  
extern bool func_8002CB24(u16, u8);     
extern bool func_8002CB88(u16, u16);     
extern bool func_8002CBF8(u16);   
extern bool func_8002CC44(u16);
extern void func_8002D3D4(void);
extern void func_8002E1B8(void);

/* spriteAddresses.c */
extern void resetSpriteAddressesFlags(void);
u16 setSpriteAddresses(u8 *romAddr, u8 *vaddr, u32 length);
extern void dmaSprites();

/* sprite.c */
extern void initializeBitmaps(void);
extern u16 func_80029DAC(u8 *timg, u16 *pal, u16 flags);
extern bool func_80029E2C(u16 index, u16, u16);  
extern bool func_80029EA4(u16 index, u16);
extern bool func_80029F14(u16 index, u16);
extern bool func_80029F98(u16 index, u8, u8);
extern bool func_8002A02C(u16 index, u16);
extern bool func_8002A09C(u16 index, f32, f32, f32);
extern bool func_8002A120(u16 index, f32, f32, f32);
extern bool func_8002A1A4(u16 index, f32, f32, f32);
extern bool func_8002A228(u16 index, u8 r, u8 g, u8 b, u8 a);
extern bool func_8002A2E0(u16 index, u16, u16);
extern u8 *func_8002A340(u16 index, u32 *start, u8 *timg, u8 *romAddr);
extern u32 func_8002A3A0(u16 arg0, u32 arg1[]);
extern void func_8002AE58(void);

extern CharacterSprite characterSprites[MAX_CHARACTER_SPRITES];
extern RenderedSprite renderedSprites[MAX_RENDERED_SPRITES];
extern Sprite globalSprites[MAX_ACTIVE_SPRITES];

#endif