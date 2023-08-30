
#ifndef _TILES_H_
#define _TILES_H_

#include "common.h"

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

extern void func_8003B870();      
extern void func_8003BC50(u8, u16);   
extern void func_8003BD60(s32);                      
extern void func_8003BE98(u16, u8, u8, u8, u8);
extern void func_8003BF7C(u16, u8, u8, u8, u8, s32);
extern void func_8003C084(u16, u8);    
extern void func_8003C6E4();
extern u8 func_8003C1A4(u16);
extern void func_8003C1E0(u16, f32, f32, f32, u8, u8);      
extern void func_8003C504(u32);     
extern u32 func_8004D380(u8, u32);   

extern u16 gTileContextFlags;
extern MapTileContext gTileContext[];

#endif