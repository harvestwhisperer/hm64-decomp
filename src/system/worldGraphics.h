#ifndef _WORLD_GRAPHICS_H_
#define _WORLD_GRAPHICS_H_

#include "common.h"

#define ACTIVE 1
#define UPDATE_SCALE 0x10
#define UPDATE_ROTATION 0x20
// 0x40 don't update translation based on rotation
// 0x80 = bump up z value

// D_8018A850
typedef struct {
	Vec3f positions;
	Vec3f scaling;
	Vec3f rotation;
	u16 spriteCount;
	Mtx translation;
	Mtx scale;
	Mtx rotationX;
	Mtx rotationY;
	Mtx rotationZ;
	Gfx *dl;
	u16 flags;
} WorldGraphics;

typedef struct {
    Mtx projection;
	Mtx orthographic;
	Mtx translationM;
	Mtx scale;
	Mtx rotationX;
	Mtx rotationY;
	Mtx rotationZ;
    Mtx viewing;
	Vec3f translation;
	Vec3f scaling;
	Vec3f rotation;
	u32 unknown[0x2B];
} WorldMatrices;

extern void initializeWorldGraphics(void);   
extern u16 func_8002929C(Gfx *dl, u16 flags);
extern bool func_800292EC(u16 index, f32 x, f32 y, f32 z);
extern bool func_80029330(u16 index, f32 arg1, f32 arg2, f32 arg3);
extern bool func_80029374(u16 index, f32 arg1, f32 arg2, f32 arg3);
extern void func_800293B8(void);
extern Gfx* func_800293C0(Gfx* dl, WorldMatrices* arg1);

extern Vec3f previousWorldRotationAngles;
extern Vec3f currentWorldRotationAngles;

#endif