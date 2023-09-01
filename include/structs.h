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
	u16 animationIndex;
	u16 animationIndexPlusOne;
} Animation;

typedef struct {
	void *address;
	u32 maxValue;
	u8 set;
} GameVariable;

// change to generic struct for reading 6 bits
typedef struct {
    u8 unk_0;
    u8 unk_1;
    u8 unk_2;
    u8 unk_3;
    u8 unk_4;
    u8 unk_5; 
} Days;

// change to generic struct for reading 6 shorts
typedef struct {
    u16 unk_0;
    u16 unk_2;
    u16 unk_4;
    u16 unk_6;
    u16 unk_8;
    u16 unk_A;
} Bits;

typedef struct {
    u16 unk_0;
    u16 unk_2;
    u16 unk_4;
    u16 unk_6;
    u16 unk_8;
    u16 unk_A;
    u16 unk_C;
    u16 unk_E;
} Unaligned32;

typedef struct {
    u32 unk_0;
    u32 unk_4;
    u32 unk_8;
    u32 unk_C;
} Aligned32;

typedef union {
    Unaligned32 unaligned_32;
    Aligned32 aligned_32;
} MemoryRead_32;

typedef union {
    Vec4f vec4;
    Vec3f vec3;
} Vec;

typedef struct {
    void *romAddr;
    void *vaddr;
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