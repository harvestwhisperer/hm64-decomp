#ifndef _WORLD_GRAPHICS_H_
#define _WORLD_GRAPHICS_H_

#include "common.h"

typedef struct {
	Vec3f position;
	Vec3f scaling;
	Vec3f rotation;
	Mtx translation;
	Mtx scale;
	Mtx rotationX;
	Mtx rotationY;
	Mtx rotationZ;
	Gfx *dl;
	u16 flags;
} WorldGraphics;

extern void func_80029170();   
// no op or shelved code
extern void func_800293B8();

#endif