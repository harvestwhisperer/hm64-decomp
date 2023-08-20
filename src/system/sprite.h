#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "common.h"

#define MAX_SPRITES 192
#define MAX_SPRITE_INFO 40
#define MAX_RENDERED_SPRITES 49
#define MAX_ACTIVE_SPRITES 192

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
	void *ptr_00; // 00
	void *ptr_04; // 04
	void *ptr_08;  // 08
	void *ptr_0C; // 0C
	void *ptr_10; // 10
	void *ptr_14; // 14
	Vec3f unk_18; // 18-24
	u16 unk_24; // 24
	u16 unk_26; // 26
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

#endif