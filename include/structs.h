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
	u8 currentStamina; // 0
	char name[6]; // 1-7
    u8 padding;
	u8 fatigueCounter; // 8
    u32 unk_C;
    u32 unk_10;
    u8 unk_14;
    u8 toolSlots[8]; // 0x15
    u8 currentTool; // 0x1D
    u8 bottleContents; // 0x1E
    u8 unk_1F[5];
    u8 belongingsSlots[8];
    u8 unk_2C;
    u8 unk_2D;
    u8 keyItemSlots[24];
    u8 unk_46;
    u8 unk_47;
	Vec3f startingCoordinates; // 0x48
	Vec3f currentCoordinates; // 0x54
	u32 unk_60; // 0x60
	u16 action1; // 0x64
	u16 action2; // 0x66
	u16 action3; // 0x68
	u8 action4; // 0x6A
	u8 unk_6B; // 0x6B
	u8 unk_6C; // 0x6C
	u8 direction; // 0x6D
	u8 unk_6E; // 0x6E
	u8 unk_6F; // 0x6F
	u8 unk_70;
	u8 fatigue[5]; // 71
    u16 flags; // 0x78
} Player;

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
	void *tileBank;
	Vec3f unk_4;
	Vec3f unk_10;
	Vec3f unk_1C;
	Vec3f unk_28;
	Vec3f unk_34; 
	u16 unk_40;
	u8 unk_42;
	u8 mapIndex;
	u8 unk_44;
	u8 unk_45;
	u16 unk_46;
	u8 rotation;
	u16 flags;
} MapTileContext;

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
    u16 unk_0;
    u16 unk_2;
    u16 unk_4;
    u8 unk_6;
} MainLoopCallbackInfo;

typedef struct {
    Vec3f unk_0;
    u32 unk_C;
    u32 unk_10;
    u32 unk_14;
    Vec3i unk_18;
    u16 unk_24;
    u16 unk_26;
    u16 unk_28;
    u8 unk_2A;
    u8 flags;
} UnknownStruct;

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

// likely related to tool use
typedef struct {
    u16 unk_0; // 28
    u8 unk_2; // 2A
    u8 unk_3; // 2B
    u16 unk_4; // 2C
    u16 unk_6; // 2E
    u8 unk_8; // 30
    u16 unk_A; // 32
    u16 unk_C; // 34
    u8 unk_E; // 36
} UnknownStruct3;

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