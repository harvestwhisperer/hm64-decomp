#ifndef _GRAPHIC_H_
#define _GRAPHIC_H_

#include "common.h"
#include "system/map.h"
#include "system/sprite.h"

#define SCREEN_WD 320
#define SCREEN_HT 240
#define GFX_GLIST_LEN 31

#define DEGREES_TO_RADIANS_CONSTANT 0.0174532925199432955

typedef struct {
	Mtx projection;
	Mtx viewing;
	f32 l;
	f32 r;
	f32 t;
	f32 b;
	f32 n;
	f32 f;
	f32 fov;
	f32 aspect;
	f32 near;
	f32 far;
	Vec3f eye;
	Vec3f at;
	Vec3f up;
	u8 perspectiveMode;
} Camera;

typedef struct {
	f32 x;
	f32 y;
	f32 z;
	f32 w;
} Coordinates;

typedef struct {
    u32 padding[4];
    u8 unk_10;
    u8 unk_11;
    u8 unk_12;
} UnknownGraphicsStruct;

extern void graphicsInit();
extern f32 sinfRadians(f32);
extern f32 cosfRadians(f32);
void setBitmapFormat(Bitmap *sprite, Texture *timg, u16 *palette);
extern Gfx* func_80026F88(Gfx* dl, Bitmap* arg1, u32 arg2, u16 arg3);
extern void func_800276AC(Vtx*, u16 width, u16 height, u16 textureSize, u16, u16, u16, s16, s16, u16, u8 r, u8 g, u8 b, u8 a);
extern void func_80027950(Vec3f, Vec3f*, Vec3f);
extern Vec3f* func_80028520(Vec3f *arg0, f32 arg1, u8 arg2, f32 arg3);        
extern u8* func_80028888(u16 arg0, u32* arg1);                   
extern u8 *func_800288A0(u16 arg0, u32 *arg1);
extern u8* func_800288B8(u16 arg0, u32 *arg1, u8 *arg2);
extern f32 getSpriteYValueFromDirection(u8 arg0);    
extern void setInitialWorldRotationAngles(f32 x, f32 y, f32 z);
extern void func_80028EF8(f32 x, f32 y, f32 z);
extern void drawFrame(void);  

extern Vtx bitmapVertices[2][0x80][4];

#endif