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

// move following structs to more specific headers in src directory

typedef struct {
	u16 animationIndex;
	u16 animationIndexPlusOne;
} Animation;

typedef struct {
	void *address;
	u32 maxValue;
	u8 set;
} GameVariable;

typedef struct {
    void *romStart;
    void *romEnd;
    u16 flags;
} TileInfo;

typedef struct {
    u8 unk_0;
    u8 unk_1;
    u8 unk_2;
    u8 unk_3;
    u8 unk_4;
    u8 unk_5; 
} Days;

typedef struct {
    u16 unk_0;
    u16 unk_2;
    u16 unk_4;
    u16 unk_6;
    u16 unk_8;
    u16 unk_A;
} Bits;

typedef struct {
    void* romAddr;
    void *vaddr;
} Addresses;

typedef struct {
    u32 padding[3923];
    f32 unk_3D4C;
    f32 unk_3D50;
    f32 unk_3D54;
    f32 unk_3D58;
    f32 unk_3D5C;
    f32 unk_3D60;
    f32 unk_3D64;
    f32 unk_3D68;
    f32 unk_3D6C;
    f32 unk_3D70;
    f32 unk_3D74;
    f32 unk_3D78;
    f32 unk_3D7C;
} UnknownStruct2;

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