#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "common.h"

#define MAX_BITMAPS 176

/* bitmap flags */
#define BITMAP_ACTIVE 1
// 2
#define BITMAP_USE_BILINEAR_FILTERING 4

/* rendering flags - bitfield layout:
 * Bits 0-1:   Flip flags (horizontal/vertical)
 * Bits 3-4:   Horizontal anchor alignment (0-3)
 * Bits 5-6:   Vertical anchor alignment (0-3)
 * Bits 7-8:   Billboard axis mapping mode (0-3)
 * Bit 9:      Triangle winding order (0x200)
 * Bits 10-12: Blend mode (0-7)
 */

#define BITMAP_RENDERING_FLIP_HORIZONTAL 1
#define BITMAP_RENDERING_FLIP_VERTICAL 2

// Field bit positions
#define BITMAP_ANCHOR_H_SHIFT  3   // bits 3-4
#define BITMAP_ANCHOR_V_SHIFT  5   // bits 5-6
#define BITMAP_AXIS_SHIFT      7   // bits 7-8
#define BITMAP_BLEND_SHIFT    10   // bits 10-12

// Construction macros
#define BITMAP_ANCHOR_H(val)  ((val) << BITMAP_ANCHOR_H_SHIFT)
#define BITMAP_ANCHOR_V(val)  ((val) << BITMAP_ANCHOR_V_SHIFT)
#define BITMAP_AXIS(val)      ((val) << BITMAP_AXIS_SHIFT)
#define BITMAP_BLEND(val)     ((val) << BITMAP_BLEND_SHIFT)

// Anchor alignment bitmasks (bits 3-6)
#define BITMAP_ANCHOR_HORIZONTAL_MASK (0x3 << BITMAP_ANCHOR_H_SHIFT)  // bits 3-4
#define BITMAP_ANCHOR_VERTICAL_MASK   (0x3 << BITMAP_ANCHOR_V_SHIFT)  // bits 5-6
#define BITMAP_ANCHOR_MASK (BITMAP_ANCHOR_HORIZONTAL_MASK | BITMAP_ANCHOR_VERTICAL_MASK)

// Anchor alignment modes (values 0-3)
#define SPRITE_ANCHOR_CENTER    2  // Default centered
#define SPRITE_ANCHOR_POSITIVE  1  // Offset positive (right/bottom edge)
#define SPRITE_ANCHOR_NEGATIVE  3  // Offset negative (left/top edge)

// Billboard axis mapping bitmask (bits 7-8)
#define BITMAP_AXIS_MAPPING_MASK (0x3 << BITMAP_AXIS_SHIFT)

// Billboard axis mapping modes (values 0-3)
#define SPRITE_BILLBOARD_XY  2  // Standard billboard facing camera (XY plane)
#define SPRITE_BILLBOARD_XZ  3  // Horizontal billboard (XZ plane, for ground effects)

// Triangle winding order flag (bit 9)
#define SPRITE_RENDERING_REVERSE_WINDING 0x200

// Blend mode bitmask (bits 10-12)
#define BITMAP_BLEND_MODE_MASK (0x7 << BITMAP_BLEND_SHIFT)

// Blend mode values (values 0-7)
#define SPRITE_BLEND_DEFAULT           0  // Default rendering
#define SPRITE_BLEND_OPAQUE            1  // Opaque surface
#define SPRITE_BLEND_ALPHA_MODULATED   2  // Alpha blending (runtime alpha control via setSpriteColor)
#define SPRITE_BLEND_ALPHA_DECAL       3  // Alpha from texture only (no modulation)
#define SPRITE_BLEND_ALPHA_DECAL_NO_Z  4  // Alpha from texture, no Z-write (UI overlays)



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
extern bool setBitmapAnchorAlignment(u16 index, u16, u16);  
extern bool setBitmapAxisMapping(u16 index, u16);
extern bool setBitmapTriangleWinding(u16 index, u16);
extern bool setBitmapFlip(u16 index, u8, u8);
extern bool setBitmapBlendMode(u16 index, u16);
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