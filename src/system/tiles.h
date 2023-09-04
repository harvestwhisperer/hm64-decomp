
#ifndef _TILES_H_
#define _TILES_H_

#include "common.h"

#define MAX_TILES 96

typedef struct {
	u8 *tileBank;
	Vec3f unk_4;
	Vec3f unk_10;
	Vec3f unk_1C;
	Vec3f unk_28;
	Vec3f unk_34; 
	u16 mapIndex;
	u8 unk_42;
	u8 unk_43;
	u8 unk_44;
	u8 unk_45;
	u16 unk_46;
	u8 rotation;
	u16 flags;
} MapTileContext;

typedef struct {
    void *romStart;
    void *romEnd;
    u16 flags;
} TileInfo;

extern void intializeTileContext();      
bool func_8003BB14(u16 arg0, u16 mapIndex); 
extern void func_8003BC50(u8, u16);   
extern void func_8003BD60(s32);                      
extern void func_8003BE98(u16, u8, u8, u8, u8);
extern void func_8003BF7C(u16, u8, u8, u8, u8, s32);
extern bool func_8003C084(u16, u8);    
extern bool func_8003C6E4();
extern bool func_8003C1A4(u16);
bool func_8003C1E0(u16 arg0, f32 arg1, f32 arg2, f32 arg3, u8 arg4, u8 arg5);
extern void func_8003C504(u32);     
extern bool func_8003C5C0(u16, u8, u8);

extern u16 gTileContextFlags;
extern MapTileContext gTileContext[];

#endif