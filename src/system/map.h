#ifndef _MAP_H_
#define _MAP_H_

#include "common.h"

// alternate
// typedef struct {
// 	f32 unk_0;
// 	f32 unk_4;
// 	u16 unk_8;
// 	u16 unk_A;
// 	u16 unk_C;
// 	u8 unk_E;
// 	u16 spriteCounter;
// 	u16 flags;
// } UnknownMapStruct1;

// 80158248
typedef struct {
    f32 unk_0;
    f32 unk_4;
    u16 unk_8;
    u16 unk_A;
    u16 unk_C;
    u16 unk_E;
    u8 unk_10;
    u8 unk_11; // counter
    u16 flags;
} UnknownMapStruct1;

// 0x8013DC40
typedef struct  {
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
    u32 padding[3911];
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
} UnknownMapStruct2;

typedef struct {
    u32 unk_0[8];
    Vec3f unk_20;
    void *unk_2C;
    void *unk_30;
} UnknownMapStruct3;

//D_80141C98
typedef struct {
    void *vaddr1;
    void *vaddr2;
    void *vaddr3;
    u32 unk_C;
    u8 unk_10;
    u8 unk_11;
    u16 unk_12;
} UnknownMapStruct4;


extern void func_800337D0(void);    
extern void func_8003423C(u16, f32, f32, f32);
extern void func_80034298(u16, f32, f32, f32);   
extern void func_800342F4(u16, f32, f32, f32);     
extern bool func_80034350(u16 arg0, u8 arg1, u8 arg2, u8 arg3, u8 arg4);
extern bool func_80034C40(u16, u16, u16, u16, f32, f32, f32, s32, s32, s32, s32); 
extern void func_80034D64(u32, u8, u16, u32);
extern bool func_80034DC8(u16, u8, u16);
extern bool func_80034E64(u8, u8);     
extern bool func_80035004(u16 arg0, u16 arg1, u8 arg2, u8 arg3); 
extern f32 func_80035150(u16, f32, f32);      
extern Vec3f* func_80036610(Vec3f*, u16, f32, f32);   
extern bool func_80036A84(u16);      
extern void func_80036C08(u32);                                 
extern void func_80036FA0(u32); 
extern bool func_80038990(u16, u16, u8);       
extern void func_80038B58(u8, u8, u8, u8);   
extern void func_8003A1BC(void);


extern UnknownMapStruct1 D_80158248;

#endif