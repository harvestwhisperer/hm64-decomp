#ifndef _MAP_H_
#define _MAP_H_

#include "common.h"

#include "system/mapController.h"

#define MAX_MAPS 1

/* flags */
#define MAP_ACTIVE 1
#define RGBA_FINISHED 8

#define MAP_OBJECT_ACTIVE 1
#define MAP_SPRITE_ACTIVE 1

typedef struct {
    void *timg;
    void *pal;
    s32 width;
    s32 height;
    u32 fmt;
} Tile;

// TODO: find better names for vec structs
typedef struct {
    f32 x, y, z; // Vec3f
    u8 flags;
} DecompressedVec3f;

typedef struct {
    u8 flags;
    u8 part1;
    u8 part2;
    u8 part3;
    u8 part4;
    u8 part5;
    u8 part6;
} CompressedVec3f;

typedef struct {
    u16 unk_0;
    u16 unk_2;
    u8 arr[4];
    u8 arr2[4];
    u8 arr3[4];
} UnknownVertexStruct;

typedef struct {
    u8 ob[3];
    u8 unk_3;
    u8 unk_4;
    u8 unk_5;
    u8 flags;
    u8 count;
} VtxInfo;

// 0x8013DC70
typedef struct MapVtx {
    Vtx *vtx;
    u16 currentVtxIndex;
    u16 vtxCount;
    u8 unk_8;
    u8 unk_9;
    u8 flags;
    u8 count;
} MapVtx;

// 0x8013DC64
// same layout as MapVtx but looks like members have a different function
typedef struct {
    u16 *tileIndices; // 0x64, might 2D array pointer // 0x8025503C
    u16 unk_4; // 0x68, used in looking up index of map object interacted with
    u16 vertexCount; // 0x6A, loop counter for vertex info objects
    u8 tileSizeX; // 0x6C
    u8 tileSizeZ; // 0x6D
    // counters for indexing into 8025503C
    u8 mapWidth; // 0x6E, mapWidth in tiles x
    u8 mapHeight; // 0x6F, mapHeight in tiles Z
} MapGrid;

// 0x8014198C
// offset 0x3D4C
typedef struct {
    Vec3f currentPosition;
    Vec3f unk_C; //0x998
    Vec3f unk_18; // 0x9A4
    Vec3f unk_24; // 0x9B0
    Vec3f rotation; // 0x9BC // angles
    Vec3f viewOffset; // 0x9C8
    u8 calculatedTileX; // 0x9D4
    u8 calculatedTileZ; // 0x9D5
    u8 viewExtentX; // 0x9D6
    u8 viewExtentZ; // 0x9D7
} MapCameraView;

// 0x801418D8
typedef struct {
    u16 unk_0; // counter
    u8 unk_2; // related to bitmap scale for traversing timg
    u8 unk_3; // counter for byteswapped Vec3fs
} UnknownMapStruct3;

// 0x80141A18
// map objects sprite info
typedef struct {
    Vec3f coordinates; // 0xA18, coordinates
    u16 spriteIndex; // 0xA24
    u16 unk_E; // 0xA26 // likely offset into shared spritesheet
    u8 unk_10; // 0xA28
    u8 unk_11; // 0xA29
    volatile u8 flags; // 0xA2A
} MapObject;

// 0x80141B64
typedef struct {
    u32 unk_0;
    u8 unk_4;
    u32 unk_8;
    u16 spriteIndex;
    u16 unk_E;
    u8 flags;
} WeatherSprite;

// 0x80141C98
// map spawnable sprites
// loads textures at 0xD8B1D0
typedef struct {
    u32 *textureIndex; // 0xC98
    u32 *paletteIndex; // 0xC9C
    u8 *spriteToPaletteIndex; // 0xCA0
    // sprite indices
    u8 unk_10; // 0xCA4 // x size of map, same as D_801FD624
    u8 unk_11; // 0xCA5 // z size of map, same as D_801C3F35
    u8 unk_12; // 0xCA6
    // map grid for sprite mapping
    u16 arr[0x40]; // 0xCA8
    u16 arr2[3][0x1E0]; // 0xD28
} GroundObjects;

// D_80142868
// extra geometry rendered on top of scene
typedef struct {
    u16 arr1[0x10]; // 0x80142868
    u16 arr2[0x10]; // 0x80142888
    u16 unk_40; // 0x801428A8
    u16 unk_42; // 0x801428AA, current index into arr1
    u8 unk_44; // 0x801428AC set from sprite vec.x, size
    u8 unk_45; // 0x801428AD set from sprite vec.z, size
    u16 flags; // 0x801428AE
} MapAdditions;

// 0x5528
// D_80143168
// size 0x24
typedef struct {
    void *timg; // 0x0
    void *pal; // 0x4
    u32 width; // 0x8
    u32 height; // 0xC
    u32 fmt; // 0x10
    u16 spriteNumber; // 0x14
    u16 size; // 0x16
    Vec3f unk_1C; // 0x18
} MapBitmap;

// D_801580A4
typedef struct {
    Vec3f translation; // 0xA4
    Vec3f scale; // 0xB0
    Vec3f rotation; // 0xBC
    Vec4f rgba; // 0xC8
    Vec4f groundRgba; // 0xD8
    Vec4f defaultRgba; // 0xE8
    Vec4f unk_60; // 0xF8
} MapFloats;

// 0x1A608
// D_80158248
typedef struct {
    f32 mapOriginX; // 0x48
    f32 mapOriginZ; // 0x4C
    u16 unk_8; // 0x50, timer for gradual RGBA; likely an s16
    u16 unk_A; // 0x52 // vertex count or part of map size
    u16 unk_C; // 0x54 // vertex count or part of map size
    u16 height; // 0x56
    u8 unk_10; // 0x58 // set from *mainMap.unk_c
    u8 mapObjectCount; // 0x59
    u16 flags; // 0x5A , 0x8 = RGBA
} MapState;

typedef struct {
      u8 baseHeight;           
      u8 triangle1[3][3];         
      u8 triangle2[3][3];
} TerrainQuad;

typedef struct {
    u16 header;
    u16 unk_2;
    u8 scalingX;
    u8 scalingY;;
    u8 mapWidth;
    u8 mapHeight;
    u8 unk_8;
    u8 unk_9;
    u8 unk_A;
    u8 unk_B;
} MapGridInfo;

// 0x8013DC40
typedef struct  {
    // ptrs set from offset array at 80255000
    // could be its own pointers substruct
    void *unk_0; // ptr to array of offsets + vertex info structs
    u8 *terrainQuads; // u8 array used for float calculations
    u8 **unk_8; // seems like grid to tile mapping, used in looking up index of map object interacted with
    u8 *unk_C; // ptr to compressed Vec3fs bank
    void *unk_10; // index + tile spritesheets
    void *unk_14; // index + tile palette
    void *unk_18; // index + ci sprites
    void *unk_1C; // index + palette
    void *unk_20; // param 2 of func_800388A4, 0x8013DC60
    MapGrid mapGrid; // 0x24, vertex/mesh info, 0x8013DC64
    MapVtx vtxs[1024]; // 0x30 // map model vertices
    u16 unkArr[80]; // 0x3030, corresponds to D_80204B48
    u16 unk_30D0;
    u16 unk_30D2[0x63C]; // more vertices
    MapCameraView mapCameraView; // 0x3D4C
    UnknownMapStruct3 mapStruct3[16]; // 0x3D98 // related to tile bitmaps/compressed vecs
    MapObject mapObjects[12]; // 0x3DD8
    u32 padding1[20];
    WeatherSprite weatherSprites[16];
    GroundObjects groundObjects; // 0x4058;
    // 0x44A8 / 0x801420E8: array
    MapAdditions mapAdditions[32]; // 0x4C28 // non-interactable map additions?
    MapBitmap mapBitmaps[56]; // 0x5528
    u32 padding2[0x48]; // 0x5D08
    Gfx displayLists[0x2800]; // 0x5E28, 0x80143A68
    u8 visibilityGrid[38][42]; // 0x19E28, might be larger/different dimensions
    MapFloats mapFloats; // 0x1A464
    u32 padding4[0x50]; // 0x1A4C8, general/base map reference values
    // 0x80157A68, 0x19e28 = related to byteswapped Vec3f scaling
    MapState mapState; // 0x1A608
} MainMap;

extern void func_800337D0(void);    
extern bool func_80033A90(u16 mapIndex, MapController* arg1, void* arg2, void* arg3, void* arg4, void* arg5, void* arg6, void* arg7, void* arg8, void* arg9, void *argA);
extern bool func_80034090(u16 mapIndex);  
extern bool setMapTranslation(u16, f32, f32, f32);
extern bool func_80034298(u16, f32, f32, f32);   
extern bool setMapScale(u16 mapIndex, f32 arg1, f32 arg2, f32 arg3);
extern bool setMapRotation(u16, f32, f32, f32);     
extern bool setMapRGBA(u16 arg0, u8 arg1, u8 arg2, u8 arg3, u8 arg4);
extern bool func_800343FC(u16 mapIndex, u8 arg1, u8 arg2, u8 arg3, u8 arg4, f32 arg5, f32 arg6, f32 arg7, u8 arg8);
extern bool adjustMapRotation(u16 mapIndex, f32 arg1, f32 arg2, f32 arg3);
extern bool func_80034738(u16 mapIndex, u8 r, u8 g, u8 b, u8 a, s16 arg5);
extern bool setMapObject(u16 mapIndex, u8 index, u16 spriteIndex, u16 arg3, f32 arg4, f32 arg5, f32 arg6, u8 arg7, u8 arg8, u8 arg9, u8 argA);
extern bool func_80034D64(u16 arg0, u8 arg1, u16 arg2, u16 arg3);
extern bool func_80034DC8(u16, u8, u16);
extern bool deactivateMapObject(u16, u8);     
extern bool func_80034EF0(u16 mapIndex, u8 arg1, u8 arg2, u32* arg3, u32* arg4, u8* arg5, u32 arg6, u32 arg7, u32 arg8, u32 arg9, u8 argA);
extern bool func_80035004(u16 arg0, u16 arg1, u8 arg2, u8 arg3); 
extern bool func_80035054(u16 mapIndex, u16 bitmapIndex, u16 arg2, f32 arg3, f32 arg4, f32 arg5);
extern f32 getTerrainHeightAtPosition(u16 mapIndex, f32, f32);      
extern bool func_80035914(u16 mapIndex, f32 arg1, f32 arg2);
extern Vec3f getTileCoordinates(MainMap* arg1, f32 arg2, f32 arg3);
extern u8 func_80036318(u16, f32, f32);
extern Vec3f convertWorldToTileCoordinates(u16, f32, f32);   
extern Vec3f func_800366F4(u16 mapIndex, f32 x, f32 z);
extern u16 func_80036880(u16, f32, f32);  
extern bool func_80036980(u16, u16, f32, f32);
extern bool checkMapRGBADone(u16);      
extern void func_80036C08(u16 mapIndex);                                 
extern void func_80036FA0(u16 mapIndex); 
extern u16 getTerrainIndexForTile(u16, u8, u8);
extern bool func_80038810(u16 mapIndex);
extern bool func_80038900(u16 mapIndex, u16 arg1, u16 arg2, u16 arg3, u16 arg4); 
extern bool func_80038990(u16, u16, u8);       
extern bool func_80038A2C(u16 mapIndex, u16 arg1, u8 arg2, u8 arg3);
extern bool func_80038AE0(u16 mapIndex, u16 arg1);
extern bool func_80038B58(u16, u16, u8, u8);   
extern void updateMapGraphics(void);
extern Vec3f func_8003AF58(u16, u8, u8); 

extern MainMap mainMap[1];

#endif