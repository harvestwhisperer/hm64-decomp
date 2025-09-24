#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "common.h"

#define MAX_BITMAPS 176

/* bitmap flags */
#define BITMAP_ACTIVE 1
#define BITMAP_USE_BILINEAR_FILTERING 0x4

/* rendering flags */
#define BITMAP_RENDERING_FLIP_HORIZONTAL 1
#define BITMAP_RENDERING_FLIP_VERTICAL 2
// 0x200 related to vertex order on triangles

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
	Vec3f viewSpacePosition; // set from byteswapped level Vec3fs from binary data for map bitmaps
	Vec3f scaling; // 28
	Vec3f rotation; // 34
	Vec4f rgba; // 40
    s16 anchorX;
	u16 anchorY;
	u16 renderingFlags; // flags for microcodes, vertex type, pixel density, triangles
	u16 flags; //56
} BitmapObject;

typedef struct {
	u16 header;
	u16 flags;
	u16 width;
	u16 height;
	u8* texture;
} Texture;

typedef struct {
    u16 unk_0;
    u16 unk_2;
    u16 unk_4;
    u16 width;
} TextureInfo;

extern void initializeBitmaps(void);
extern u16 setBitmap(u8 *timg, u16 *pal, u16 flags);
extern bool func_80029E2C(u16 index, u16, u16);  
extern bool func_80029EA4(u16 index, u16);
extern bool func_80029F14(u16 index, u16);
extern bool setBitmapFlip(u16 index, u8, u8);
extern bool func_8002A02C(u16 index, u16);
extern bool setBitmapViewSpacePosition(u16 index, f32, f32, f32);
extern bool setBitmapScale(u16 index, f32, f32, f32);
extern bool setBitmapRotation(u16 index, f32, f32, f32);
extern bool setBitmapRGBA(u16 index, u8 r, u8 g, u8 b, u8 a);
extern bool setBitmapAnchor(u16 index, s16, s16);
extern u8 *setSpriteDMAInfo(u16 index, u32 *start, u8 *timg, u8 *romAddr);
extern u32 getTextureLength(u16 arg0, u32 arg1[]);
extern void updateBitmaps(void);

extern BitmapObject bitmaps[MAX_BITMAPS];

#endif