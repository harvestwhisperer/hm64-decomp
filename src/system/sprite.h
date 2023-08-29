#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "common.h"

#define MAX_SPRITES 192
#define MAX_SPRITE_INFO 40
#define MAX_RENDERED_SPRITES 49
#define MAX_ACTIVE_SPRITES 192

#define NEED_DMA 1
#define FINISHED_DMA 0xFFFE

typedef struct {	
	void *vaddrStart;
	void *vaddrCurrent;
	void *vaddrIndex;
	void *vaddrPalette;
	void *vaddrUnk1;
	void *vaddrUnk2;
	void *vaddrUnk3;
	void *rom;
	void *vaddrAnimationStart;
	void *vaddrAnimationEnd;
	u16 animation;
	Vec3f shrink;
	Vec3f scale;
	Vec3f unk_44;
    Vec4f rgba;
	Vec4f rgbaCurrent;
	Vec4f rgbaDefault;
	Vec4f normalized;
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
	void *vaddr; /* 0x00 */
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
	void *pal;  // 04
	u32 width;  // 08 
	u32 height; // 0C
	u32 fmt; // image format // 10
	u32 flag; // 14
	u16 spriteNumber; // 18
	u16 size; // image size // 1A
	Vec3f unk_1C;
	Vec3f scaling; // 28
	Vec3f unk_34; // 34
	Vec4f rgba; // 40
    u32 unk_50;
	u16 unk_54;
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
void initializeNpcSpriteStructs(void);
bool func_8002E284(u16, u16, u32);      
bool func_8002EDF0(u16 index, s16 arg1, s16 arg2, s16 arg3);
bool func_8002F014(u16, u8, u8, u8, u8);             
bool func_8002ECD4(u16, u16, u16);              
bool func_8002F114(u16, u8);                              
bool func_8002F684(u16, u8);                    
void func_8002F6F0();    
bool func_8002F2FC(u16, u16); 
void func_8002F730();               
bool func_8002F7C8(u8, u8, u8, u8);               
bool func_8002F8F0(u8, u8, u8, u8, s32);      
bool func_8002FA2C(u16);      
bool func_8002FD80(u16, f32, f32, f32);   
void func_8002FB3C();
void func_8002FCB4(u16, u8);  
bool func_8002FD24(u16 index);
bool func_8002FE10(u16, f32, f32, f32, f32);           
bool func_8002FECC(u16); 
bool func_8002FF38(u16, u8);
bool func_80030054(u16, u8);                                                   
bool func_80030388(u16 index);                   
void func_80033058(void);                           
bool func_8003019C(u16, u8);                            
bool func_80030240(u16, u8);                            
bool func_800302E4(u16, u8);         
bool func_800309B4(u16, f32, f32);     
u16 func_80030BA0(u16* ptr, u16 offset);  
Vec3f* func_800315A0(Vec3f*, u16 index);    
bool func_80031380(s32);   
bool func_80031830(u16, u32, u8);        
Vec3f* func_80031904(Vec3f* vec, u16 index, s16 arg2, u8 arg3);
bool setSpriteAnimation(u16 index, u16 arg1);

/* globalSprites.c */
void func_8002AFE0(void); 
void func_8002B138(u16, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, u8); 
bool func_8002B6B8(u16 index);
bool func_8002B80C(u16 index, u16 offset, u8 arg2);
bool func_8002BAD8(u16);   
void func_8002BB88(u16); 
bool func_8002BD0C(u16 index, f32 x, f32 y, f32 z);  
bool func_8002BD90(u16, f32, f32, f32);                                                                                                        
bool func_8002BE98(u16, f32, f32, f32);    
bool func_8002BE14(u16, f32, f32, f32);                        
bool func_8002C1C0(u16 index, u8 r, u8 g, u8 b, u8 a, s16 arg5);
bool func_8002C7EC(u16, u16);                              
bool func_8002C85C(u16 index, u8 r, u8 g, u8 b, u8 a);
bool func_8002C914(u16, u8, u8, u8, u8);
bool func_8002CAA8(u16, u8);  
bool func_8002CB24(u16, u8);     
void func_8002CB88(u16, u16);     
bool func_8002CBF8(u16);   
void func_8002D3D4(void);

/* spriteAddresses.c */
void resetSpriteAddressesFlags(void);
u16 setSpriteAddresses(u8 *romAddr, u8 *vaddr, u32 length);
void dmaSprites();

/* sprite.c */
void func_80029B30(void);
bool func_8002AE58(void);


extern RenderedSprite renderedSprites[MAX_RENDERED_SPRITES];
extern Sprite globalSprites[MAX_ACTIVE_SPRITES];

#endif