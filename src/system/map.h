#ifndef _MAP_H_
#define _MAP_H_

#include "common.h"

typedef struct {
    void *timg;
    void *pal;
    s32 width;
    s32 height;
    u32 fmt;
} Tile;

typedef struct {
    f32 x, y, z;
    u8 size;
} Decompressed;

typedef struct {
    u8 size;
    u8 part1;
    u8 part2;
    u8 part3;
    u8 part4;
    u8 part5;
    u8 part6;
} Compressed;

// D_8013DC70
typedef struct {
    void *ptr;
    u16 unk_4;
    u16 unk_6;
    u8 unk_8;
    u8 unk_9;
    u8 unk_A;
    u8 unk_B;
} UnknownMapStruct1;

// D_8014198C
// offset 0x3D4C
typedef struct {
    Vec3f unk_0;
    Vec3f unk_C;
    Vec3f unk_18;
    Vec3f unk_24;
    Vec3f unk_30;
    Vec3f unk_3C;
    u8 unk_48; 
    u8 unk_49;
    u8 unk_4A;   
    u8 unk_4B;
} UnknownMapStruct2;

// D_801418D8
typedef struct {
    u16 unk_0;
    u8 unk_2;
    u8 unk_3;
} UnknownMapStruct3;

// D_80141A18
typedef struct {
    Vec3f unk_0;
    u16 unk_C;
    u16 unk_E;
    u8 unk_10;
    u8 unk_11;
    volatile u8 flags;
} UnknownMapStruct4;

// 80141B64
typedef struct {
    u32 pad[3];
    u16 unk_0;
    u16 unk_2;
    u8 flags;
} UnknownMapStruct5;

// D_80141C98
// map object sprites
typedef struct {
    void *vaddr1;
    void *vaddr2;
    void *vaddr3;
    u8 unk_10;
    u8 unk_11;
    u8 unk_12;
    u16 arr[0x40];
    u16 arr2[0x40];
} UnknownMapStruct6;

// 0x5528
// D_80143168
typedef struct {
    void *timg;
    void *pal;
    u32 width;
    u32 height;
    u32 fmt;
    u16 spriteNumber;
    u16 size;
    Vec3f unk_1C;
} MapBitmap;

// D_801580A4
typedef struct {
    Vec3f unk_0;
    Vec3f unk_C;
    Vec3f angles;
    Vec4f rgba;
    Vec4f groundRgba;
    Vec4f defaultRgba;
    Vec4f unk_60;
} UnknownMapStruct7;

// D_80158248
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
} UnknownMapStruct8;

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
    UnknownMapStruct1 mapStruct1[0x400];
    u32 padding2[0x347];
    UnknownMapStruct2 mapStruct2; // 0x3D4C
    UnknownMapStruct3 mapStruct3[0x10]; // 0x3D98
    UnknownMapStruct4 mapStruct4[12]; // 0x3DD8
    u32 padding4[0x14];
    UnknownMapStruct5 mapStruct5[0x10];
    UnknownMapStruct6 mapStruct6; // 0x4058;
    // 0x44A8 / 0x801420E8: array
    u32 padding6[0x4F0];
    MapBitmap mapBitmaps[0x38]; // 0x5528
    u32 padding7[0x51D7]; // 0x6088
    UnknownMapStruct7 mapStruct7; // 0x1A464
    u32 padding8[0x50]; // 0x1A4C8
    UnknownMapStruct8 mapStruct8; // 0x1A608
    u32 padding9;
} MainMap;


extern void func_800337D0(void);    
extern bool func_80033A90(u16, u16*, void*, void*, void *, void*, void*, void*, void*, void*, void*);
extern void func_8003423C(u16, f32, f32, f32);
extern bool func_80034298(u16, f32, f32, f32);   
extern void func_800342F4(u16, f32, f32, f32);     
extern bool func_80034350(u16 arg0, u8 arg1, u8 arg2, u8 arg3, u8 arg4);
extern bool func_80034C40(u16 arg0, u8 arg1, u16 arg2, u16 arg3, f32 arg4, f32 arg5, f32 arg6, u8 arg7, u8 arg8, u8 arg9, u8 argA);
extern bool func_80034D64(u16 arg0, u8 arg1, u16 arg2, u16 arg3);
extern bool func_80034DC8(u16, u8, u16);
extern bool func_80034E64(u8, u8);     
extern bool func_80034EF0(u16, u8, u8, void*, void*, void*, void*, void*, void*, void*, u8); 
extern bool func_80035004(u16 arg0, u16 arg1, u8 arg2, u8 arg3); 
extern bool func_80035054(u16 mapIndex, u16 bitmapIndex, u16 arg2, f32 arg3, f32 arg4, f32 arg5);
extern f32 func_80035150(u16, f32, f32);      
extern Vec3f* func_80036610(Vec3f*, u16, f32, f32);   
extern bool func_80036A84(u16);      
extern void func_80036C08(u32);                                 
extern void func_80036FA0(u16); 
extern bool func_80038990(u16, u16, u8);       
extern bool func_80038B58(u16, u16, u8, u8);   
extern void func_8003A1BC(void);


extern MainMap mainMap[1];

#endif