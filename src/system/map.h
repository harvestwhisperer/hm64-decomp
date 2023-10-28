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

typedef struct {
    short tc[2];
    unsigned char cn[4];
} VtxTexture;

typedef struct {
    VtxTexture *vtx;
    VtxTexture vtxTex;
} VtxTextureInfo;

// 8013DC70
typedef struct MapVtx {
    struct MapVtx *vtx;
    u16 currentVtxIndex;
    u16 vtxCount;
    u8 ob[3];
    u8 flags;
} MapVtx;

typedef u16 VtxArray[0x30][0x30];

// 8013DC64
typedef struct {
    // 2D array pointer: u16 arr[0x13][?] at 0x8025503C
    VtxArray *ptr; // 0x64
    u16 unk_4; // 0x68
    u16 unk_6; // 0x6A
    u8 unk_8; // 0x6C
    u8 unk_9; // 0x6D
    // counters for indexing into 8025503C
    u8 unk_A; // 0x6E
    u8 unk_B; // 0x6F
} UnknownMapStruct1;

// D_8014198C
// offset 0x3D4C
typedef struct {
    Vec3f unk_0;
    Vec3f unk_C; //0x998
    Vec3f unk_18; // 0x9A4
    Vec3f unk_24; // 0x9B0
    Vec3f unk_30; // 0x9BC
    Vec3f unk_3C; // 0x9C8
    u8 unk_48; // 0x9D4
    u8 unk_49; // 0x9D5
    u8 unk_4A; // 0x9D6
    u8 unk_4B; // 0x9D7
} UnknownMapStruct2;

// D_801418D8
typedef struct {
    u16 unk_0;
    u8 unk_2;
    u8 unk_3;
} UnknownMapStruct3;

// D_80141A18
typedef struct {
    Vec3f unk_0; // 0xA18
    u16 spriteIndex; // 0xA24
    u16 unk_E; // 0xA26
    u8 unk_10; // 0xA28
    u8 unk_11; // 0xA29
    volatile u8 flags; // 0xA2A
} UnknownMapStruct4;

// 80141B64
typedef struct {
    u32 unk_0;
    u8 unk_4;
    u32 unk_8;
    u16 spriteIndex;
    u16 unk_E;
    u8 flags;
} UnknownMapStruct5;

// D_80141C98
// map object sprites
typedef struct {
    void *vaddr1; // 0x98
    void *vaddr2; // 0x9C
    void *vaddr3; // 0xA0
    u8 unk_10; // 0xA4
    u8 unk_11; // 0xA5
    u8 unk_12; // 0xA6
    u16 arr[0x40]; // 0xA8
    u16 arr2[0x40]; // 0x40E8
} UnknownMapStruct6;

// 0x5528
// D_80143168
// size 0x24
typedef struct {
    void *timg; // 0x0
    void *pal; // 0x4
    u32 height; // 0xC
    u32 width; // 0x8
    u32 fmt; // 0x10
    u16 spriteNumber; // 0x14
    u16 size; // 0x16
    Vec3f unk_1C; // 0x18
} MapBitmap;

// D_801580A4
typedef struct {
    Vec3f unk_0; // 0xA4
    Vec3f unk_C; // 0xB0
    Vec3f angles; // 0xBC
    Vec4f rgba; // 0xC8
    Vec4f groundRgba; // 0xD8
    Vec4f defaultRgba; // 0xE8
    Vec4f unk_60; // 0xF8
} UnknownMapStruct7;

// 0x1A608
// D_80158248
typedef struct {
    f32 unk_0; // 0x48
    f32 unk_4; // 0x4C
    u16 unk_8; // 0x50
    u16 unk_A; // 0x52
    u16 unk_C; // 0x54
    u16 height; // 0x56
    u8 unk_10; // 0x58
    u8 unk_11; // counter // 0x59
    u16 flags; // 0x5A
} UnknownMapStruct8;

// more likely just binary data
// typedef struct {
//     u32 unk_0;
//     u8 unk_4;
//     u8 unk_5;
//     u8 unk_6;
//     u8 unk_7;
//     u32 padding[8];
// } UnknownMapStruct9;

// 0x8013DC40
typedef struct  {
    // active vertex?
    void *unk_0;
    void *unk_4;
    void *unk_8;
    void *unk_C;
    void *unk_10;
    void *unk_14;
    void *unk_18;
    void *unk_1C;
    void *unk_20;
    UnknownMapStruct1 mapStruct1; // 0x24
    MapVtx vtxs[0x400]; // 0x30
    u16 unk_arr[0x50]; // 0x3030
    u32 padding2[0x31F];
    UnknownMapStruct2 mapStruct2; // 0x3D4C
    UnknownMapStruct3 mapStruct3[0x10]; // 0x3D98
    UnknownMapStruct4 mapStruct4[12]; // 0x3DD8
    u32 padding4[0x14];
    UnknownMapStruct5 mapStruct5[0x10];
    UnknownMapStruct6 mapStruct6; // 0x4058;
    u32 padding6[0x4F0]; // 0x4168
    // 0x44A8 / 0x801420E8: array
    MapBitmap mapBitmaps[0x38]; // 0x5528
    u32 padding7[0x48]; // 0x5D08
    // might be bigger
    Gfx displayLists[0x1000]; // 0x5E28
    u32 padding8[0x318F];
    UnknownMapStruct7 mapStruct7; // 0x1A464
    u32 padding9[0x50]; // 0x1A4C8
    UnknownMapStruct8 mapStruct8; // 0x1A608
    u32 padding10;
} MainMap;


extern void func_800337D0(void);    
extern bool func_80033A90(u16, u16*, void*, void*, void *, void*, void*, void*, void*, void*, void*);
extern bool func_8003423C(u16, f32, f32, f32);
extern bool func_80034298(u16, f32, f32, f32);   
extern bool func_800342F4(u16, f32, f32, f32);     
extern bool func_80034350(u16 arg0, u8 arg1, u8 arg2, u8 arg3, u8 arg4);
extern bool func_80034C40(u16 arg0, u8 arg1, u16 arg2, u16 arg3, f32 arg4, f32 arg5, f32 arg6, u8 arg7, u8 arg8, u8 arg9, u8 argA);
extern bool func_80034D64(u16 arg0, u8 arg1, u16 arg2, u16 arg3);
extern bool func_80034DC8(u16, u8, u16);
extern bool func_80034E64(u16, u8);     
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
Vec3f* func_800359C8(Vec3f* arg0, MainMap* arg1, f32 arg2, f32 arg3);

extern MainMap mainMap[1];

#endif