#ifndef _STRUCTS_H_
#define _STRUCTS_H_

#include "common.h"

typedef struct {
	f32 x, y, z;
} Vec3f;

typedef struct {
    u32 x, y, z;
} Vec3i;

typedef struct {
	f32 r,g,b,a;
} Vec4f;

typedef struct {
    u8 r, g, b, a;
} Vec4c;

typedef struct {
    f32 x, y;
} Vec2f;

typedef union {
    Vec4f vec4;
    Vec3f vec3;
} Vec;

typedef struct {
    u32 romAddrStart;
    u32 romAddrEnd;
} Addresses;

typedef union {
    u8 byte[2];
    u16 halfword;
} Swap16;

#endif