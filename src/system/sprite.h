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
	Vec3f vec3Unk;
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

#endif