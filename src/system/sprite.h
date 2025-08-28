#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "common.h"

#define MAX_BITMAPS 176

/* bitmap flags */
#define USE_BILINEAR_FILTERING 0x4

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
	Vec3f unk_1C; // set from byteswapped level Vec3fs from binary data; shrink factor
	Vec3f scaling; // 28
	Vec3f unk_34; // 34 // angles
	Vec4f rgba; // 40
	// texture indices
    s16 unk_50;
	u16 unk_52;
	u16 unk_54; // flags for microcodes, vertex type, pixel density, triangles
	u16 flags; //56
} BitmapObject;

typedef struct {
	u16 header;
	u16 flags;
	u16 width;
	u16 height;
	u8* texture;
} Texture;

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
extern bool func_8002A2E0(u16 index, s16, s16);
extern u8 *func_8002A340(u16 index, u32 *start, u8 *timg, u8 *romAddr);
extern u32 func_8002A3A0(u16 arg0, u32 arg1[]);
extern void updateBitmaps(void);

#endif