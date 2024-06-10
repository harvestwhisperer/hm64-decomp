
#ifndef _MAP_CONTEXT_H_
#define _MAP_CONTEXT_H_

#include "common.h"

#define MAX_MODELS 96
#define MODEL_DATA_BANK 0x80255000
#define MAIN_MAP_INDEX 0

typedef struct {
	u16 header1;
	u16 header2;
	u32 unk_4;
	Vtx vertices[1]; // varying size
} Model;

// 0x802055D8
typedef struct {
	u32 *modelDataIndex; // vaddr = 0x80255000
	Vec3f unk_4;
	Vec3f unk_10;
	Vec3f unk_1C;
	Vec3f unk_28;
	Vec3f unk_34; 
	u16 mainMapIndex;
	u16 mapIndex;
	u8 unk_44;
	u8 unk_45;
	u8 unk_46;
	u8 unk_47;
	u8 rotation;
	u16 flags;
} LevelMapContext;

typedef struct {
    void *romStart;
    void *romEnd;
    u16 flags;
} MapContextAddresses;

// 0x8017044C
typedef struct {
	Vec3f currentWorldRotation;
	u8 currentMapIndex; // D_80170458/gBaseMapIndex
	u16 flags; // D_8017045A
} CurrentMapContext;

extern void initializeMapContext();      
extern bool func_8003BA44(u16 index, u16 mapIndex, u32 *levelMap);
extern bool func_8003BB14(u16 arg0, u16 mapIndex); 
extern bool func_8003BC50(u16, u16);   
extern bool func_8003BD60(u16);                      
extern bool func_8003BDA4(u16, f32, f32, f32);  
extern bool func_8003BE0C(u16 arg0, f32 arg1, f32 arg2, f32 arg3);
extern bool func_8003BE98(u16, u8, u8, u8, u8);
extern bool func_8003BF7C(u16, u8, u8, u8, u8, s16);
extern bool func_8003C084(u16, u8);    
extern void func_8003C6E4(void);
extern bool getCurrentMapRotation(u16);
extern bool func_8003C1E0(u16 arg0, f32 arg1, f32 arg2, f32 arg3, u8 arg4, u8 arg5);
extern bool func_8003C504(u16);     
extern bool func_8003C5C0(u16, u8, u8);

extern LevelMapContext gMapModelContext[1];

// 0x8017044C
//extern CurrentMapContext currentMapContext;

extern Vec4f D_802373F8;

#endif