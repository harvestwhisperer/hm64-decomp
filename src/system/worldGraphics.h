#ifndef _WORLD_GRAPHICS_H_
#define _WORLD_GRAPHICS_H_

#include "common.h"

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

extern void initializeWorldGraphics();   
// no op or shelved code
extern void func_800293B8();
Gfx* func_800293C0(Gfx* dl, WorldMatrices* arg1);

// previous rotation
extern Vec3f D_8013D5D8;
// current rotation
extern Vec3f D_8017044C;

#endif