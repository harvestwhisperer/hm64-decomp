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
    f32 x, y;
} Vec2f;

typedef union {
    Vec4f vec4;
    Vec3f vec3;
} Vec;

typedef struct {
    void *romAddrStart;
    void *romAddrEnd;
} Addresses;

typedef struct {
    s8 unk_0;
    s8 unk_2;
    s8 unk_4;
    s8 unk_6;
    s8 unk_8;
    s8 unk_A;
    s8 unk_C;
    s8 unk_E;
    s8 unk_F;
} UnknownStruct4;

#endif