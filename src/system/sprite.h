#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "common.h"

#define MAX_SPRITES 192
#define MAX_SPRITE_INFO 40
#define MAX_RENDERED_SPRITES 49
#define MAX_ACTIVE_SPRITES 192
#define MAX_SHADOW_SPRITES 3
#define MAX_NPC_SPRITES 102
#define MAX_BITMAPS 176

#define NEED_DMA 1

#define OBJECT_SPRITE_BANK_1_START 0x802EB800

#define DOG_TITLE_SPRITE 0x44

typedef struct {	
	void *vaddrStart; // 0x0
	void *vaddrCurrent; // 0x4
	void *vaddrIndex; // 0x8
	void *vaddrPalette; // 0xC
	void *vaddrUnk1; // 0x10
	void *vaddrUnk2; // 0x14
	void *vaddrUnk3; // 0x18
	void *rom; // 0x1C
	void *vaddrAnimationStart; // 0x20
	void *vaddrAnimationEnd; // 0x24
	u16 animation; // 0x28
	Vec3f shrink; // 0x2C
	Vec3f scale; // 0x38
	Vec3f unk_44;
    Vec4f rgba; // 0x50 
	Vec4f rgbaCurrent; // 0x60
	Vec4f rgbaDefault; // 0x70
	Vec4f normalized; // 0x80
	u8 unk_90;
	u8 unk_91;
	u8 unk_92;
    u16 unk_94;
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

typedef struct {
	void *vaddr; /* 0x00 */ // offset indices; data from initalize.c: 80119750-8011BCA0
	void *romTextureStart; /* 0x04 */
	void *romTextureEnd; /* 0x08 */
	void *romPaletteStart; /* 0x0C */
	void *romPaletteEnd; /* 0x10 */
	void *romIndexStart; /* 0x14 */
	void *romIndexEnd; /* 0x18 */
	u16 unk_1C; /* 0x1C */
	u16 unk_1E; /* 0x1E */
	u8 collisionBufferX; /* 0x20 */
	u8 collisionBufferY; /* 0x21 */
	u8 flag; /* 0x22 */
	u16 flags; /* 0x24 */
} CharacterSprite;

typedef struct {
	void *ptr_00; // 00
	void *ptr_04; // 04
	void *ptr_08;  // 08
	void *ptr_0C; // 0C
	void *ptr_10; // 10
	void *ptr_14; // 14
	Vec3f unk_18; // 18-24
	u16 unk_24; // 24
	u8 unk_26; // 26
	Vec3f startingCoordinates; // 28-34
	Vec3f currentCoordinates; // 34-40
	Vec3f unk_3C; // 40-4C
	u32 unk_4C;
	u16 characterIndex; // 0x50
	u16 unk_52;
	Animation anim; // 0x54-0x58
	u16 unk_58; 
	u16	unk_5A;
	u16 collision; // 0x5C
	u16 unk_5E;
	u16 spriteOffset; // 0x60;
	u16 unk_62; //
	u16 unk_64;
	u8 unk_66;
	u8 direction;  // 0x67
	u16 flag; // 0x68
	u16 flags; // 0x6A
} RenderedSprite;

typedef struct {
	void *timg; // 00
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
	u16 unk_54; // flags for microcodes
	u16 flags; //56
} Bitmap;

typedef struct {
	void *romTextureStart;
	void *romTextureEnd;
	void *romPaletteStart;
	void *romPaletteEnd;
	void *vaddr1;
	void *vaddr2;
	void *vaddr3;
	void *vaddr4;
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
extern bool func_8002E284(u16, u16, u32);      
extern bool func_8002EDF0(u16 index, s16 arg1, s16 arg2, s16 arg3);
extern bool func_8002EEA4(u16 arg0);     
extern bool func_8002F014(u16, u8, u8, u8, u8);        
extern bool func_8002ECD4(u16, u16, u16);              
extern bool func_8002F114(u16, u8);
extern bool func_8002F2FC(u16, u16); 
extern bool func_8002F684(u16, u8);                    
extern void func_8002F6F0();    
extern void func_8002F730();               
extern void func_8002F7C8(u8, u8, u8, u8);               
extern bool func_8002F8F0(u8, u8, u8, u8, s32);      
extern bool func_8002FA2C(u16);      
extern bool func_8002FD80(u16, f32, f32, f32);   
extern void func_8002FB3C();
extern void func_8002FCB4(u16, u8);  
extern bool func_8002FD24(u16 index);
extern bool func_8002FE10(u16, f32, f32, f32, f32);           
extern bool func_8002FECC(u16); 
extern bool func_8002ED80(u16 index, s16 arg1);
extern bool func_8002FF38(u16, u8);
extern bool func_80030054(u16, u8);                                                   
extern bool func_80030388(u16 index);                   
extern void func_80033058(void);                           
extern bool func_8003019C(u16, u8);                            
extern bool func_80030240(u16, u8);                            
extern bool func_800302E4(u16, u8);         
extern bool func_800305CC(u16, f32, f32, f32);
extern bool func_800309B4(u16, f32, f32);     
extern u16 func_80030BA0(u16* ptr, u16 offset);  
extern Vec3f* func_800315A0(Vec3f*, u16 index);    
extern bool func_80031380(s32);   
extern bool func_80031830(u16, u32, u8);        
extern Vec3f* func_80031904(Vec3f* vec, u16 index, s16 arg2, u8 arg3);
extern bool setSpriteAnimation(u16 index, u16 arg1);

/* globalSprites.c */
extern void initializeGlobalSprites(void); 
extern void func_8002B138(u16, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, u8); 
extern bool func_8002B6B8(u16 index);
extern void func_8002B710(void);
extern bool func_8002B80C(u16 index, u16 offset, u8 arg2);
extern bool func_8002BAD8(u16);   
extern void func_8002BB88(u16); 
extern bool func_8002BD0C(u16 index, f32 x, f32 y, f32 z);  
extern bool func_8002BD90(u16, f32, f32, f32);                                                                                                        
extern bool func_8002BE98(u16, f32, f32, f32);    
extern bool func_8002BE14(u16, f32, f32, f32);                        
extern bool func_8002C1C0(u16 index, u8 r, u8 g, u8 b, u8 a, s16 arg5);
extern bool func_8002C52C(u16, u8, s16);
extern bool func_8002C7EC(u16, u16);                              
extern bool func_8002C85C(u16 index, u8 r, u8 g, u8 b, u8 a);
extern bool func_8002C914(u16, u8, u8, u8, u8);
extern bool func_8002CAA8(u16, u8);  
extern bool func_8002CB24(u16, u8);     
extern bool func_8002CB88(u16, u16);     
extern bool func_8002CBF8(u16);   
extern bool func_8002CC44(u16);
extern void func_8002D3D4(void);

/* spriteAddresses.c */
extern void resetSpriteAddressesFlags(void);
u16 setSpriteAddresses(u8 *romAddr, u8 *vaddr, u32 length);
extern void dmaSprites();

/* sprite.c */
extern void initializeBitmaps(void);
extern void func_8002AE58(void);


extern RenderedSprite renderedSprites[MAX_RENDERED_SPRITES];
extern Sprite globalSprites[MAX_ACTIVE_SPRITES];

#endif