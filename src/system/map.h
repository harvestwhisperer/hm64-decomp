#ifndef _MAP_H_
#define _MAP_H_

#include "common.h"

// 80158248
typedef struct {
	f32 unk_0;
	f32 unk_4;
	u16 unk_8;
	u16 unk_A;
	u16 unk_C;
	u8 unk_E;
	u16 spriteCounter;
	u16 flags;
} UnknownMapStruct1;

// 0x8013DC40
typedef struct {
	void *unk_0;
	void *unk_4;
	void *unk_8;
	void *unk_C;
	void *unk_10;
	void *unk_14;
	void *unk_18;
	void *unk_1C;
	void *unk_20;
	void *unk_24;
	void *unk_28;
	u8 unk_2C; // spacing between tiles
	u8 unk_2D; // spacing between tiles
	u8 unk_2E; // size (used in division)
	u8 unk_2F; // size (used in division)
} UnknownMapStruct2;

typedef struct {
    u32 unk_0[8];
    Vec3f unk_20;
    void *unk_2C;
    void *unk_30;
} UnknownMapStruct3;

#endif