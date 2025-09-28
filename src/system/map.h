#ifndef _MAP_H_
#define _MAP_H_

#include "common.h"

#define MAX_MAPS 1
#define MAX_GROUND_OBJECTS 64
#define MAX_CORE_MAP_OBJECTS 256
#define MAX_MAP_ADDITIONS 32
#define MAX_MAP_OBJECTS 16
#define MAX_MAP_TILES 1024
#define MAX_TILE_TEXTURES 80
#define MAX_WEATHER_SPRITES 16

/* flags */
#define MAP_ACTIVE 1
#define MAP_RGBA_FINISHED 8

#define MAP_ADDITION_ACTIVE 1

#define MAP_OBJECT_ACTIVE 1
#define MAP_OBJECT_STATIC 2

#define MAP_WEATHER_SPRITE_ACTIVE 1

// 0x8013DC64
typedef struct {
    u16 *gridToTileIndex; // 0x64
    u16 unused; // 0x68
    u16 tileCount; // 0x6A
    u8 tileSizeX; // 0x6C
    u8 tileSizeZ; // 0x6D
    u8 mapWidth; // 0x6E, in tiles
    u8 mapHeight; // 0x6F, in tiles
} MapGrid;

// 0x8013DC70
typedef struct {
    s8 *coordinates;
    u16 renderingCommandsOffset;
    u16 triCount;
    u8 yOffset;
    u8 fallbackHeight;
    u8 textureIndex; // bits 0-7 encode tile texture index, bit 8 = set max/invalid value
    u8 verticesPerTile; // processing count for gSPVertex
} Tile;

typedef struct {
    u16 triangle1Bitfield;
    u16 triangle2Bitfield;
    u8 data1[4]; // usually vtx number for modifying texture coordinates, sometimes r value for tiles that use a solid color
    u8 data2[4]; // usually tc[0] value, sometimes g
    u8 data3[4]; // usually tc[1] value, sometimes b
    u8 flags;
    u8 unused1;
    u8 unused2;
} TileRenderingInfo;

typedef struct {
    void *timg;
    void *pal;
    s32 width;
    s32 height;
    u32 fmt;
} TileBitmap;

typedef struct {
    Vec3f coordinates;
    u8 flags;
} CoreMapObject;

// 0x801418D8
typedef struct {
    u16 unk_0; // counter
    u8 spriteIndex;
    u8 repeatObjectCount;
} CoreMapObjectsMetadata;

// 0x80141A18
typedef struct {
    Vec3f coordinates; // 0xA18
    u16 spriteIndex; // 0xA24
    u16 animationIndex; // 0xA26
    u8 unk_10; // 0xA28
    u8 animationMode; // 0xA29
    volatile u8 flags; // 0xA2A
} MapObject;

// 0x80141C98
typedef struct {
    u32 *textureIndex; // 0xC98
    u32 *paletteIndex; // 0xC9C
    u8 *spriteToPaletteIndex; // 0xCA0
    u8 x; // 0xCA4
    u8 z; // 0xCA5
    u8 unk_12; // 0xCA6
    u16 spriteIndexToGrid[MAX_GROUND_OBJECTS]; // 0xCA8
    u16 gridToSpriteIndex[0x1E0];
    u16 previousGridToSpriteIndex[0x1E0];
    u16 nextGridToSpriteIndex[0x1E0]; // 0xD28
} GroundObjects;

// 0x5528
// 0x80143168
typedef struct {
    void *timg; // 0x0
    void *pal; // 0x4
    u32 width; // 0x8
    u32 height; // 0xC
    u32 fmt; // 0x10
    u16 spriteNumber; // 0x14
    u16 size; // 0x16
    Vec3f coordinates; // 0x18
} GroundObjectBitmap;

// 0x80141B64
typedef struct {
    Vec3f coordinates;
    u16 spriteIndex;
    u16 animationIndex;
    u8 flags;
} WeatherSprite;

// 0x80142868
typedef struct {
    u16 arr1[0x10]; // 0x80142868
    u16 arr2[0x10]; // 0x80142888
    u16 unk_40; // 0x801428A8
    u16 unk_42; // 0x801428AA, current index into arr1
    u8 x; // 0x801428AC
    u8 z; // 0x801428AD
    u16 flags; // 0x801428AE
} MapAdditions;

// 0x8014198C
// offset 0x3D4C
typedef struct {
    Vec3f frustumCorner0;
    Vec3f frustumCorner1; //0x998
    Vec3f frustumCorner2; // 0x9A4
    Vec3f frustumCorner3; // 0x9B0
    Vec3f rotation; // 0x9BC // angles
    Vec3f viewOffset; // 0x9C8
    u8 cameraTileX; // 0x9D4
    u8 cameraTileZ; // 0x9D5
    u8 viewExtentX; // 0x9D6
    u8 viewExtentZ; // 0x9D7
} MapCameraView;

// D_801580A4
typedef struct {
    Vec3f translation; // 0xA4
    Vec3f scale; // 0xB0
    Vec3f rotation; // 0xBC
    Vec4f rgba; // 0xC8
    Vec4f currentRGBA; // 0xD8
    Vec4f targetRGBA; // 0xE8
    Vec4f deltaRGBA; // 0xF8
} MapGlobals;

// 0x1A608
// D_80158248
typedef struct {
    f32 mapOriginX; // 0x48
    f32 mapOriginZ; // 0x4C
    s16 RGBARate; // 0x50
    u16 renderedVertexCount; // 0x52
    u16 startingVertex; // 0x54
    u16 totalVertexCount; // 0x56
    u8 coreMapObjectsCount;
    u8 mapObjectCount; // 0x59
    u16 flags; // 0x5A
} MapState;

typedef struct {
      u8 baseHeight;           
      u8 triangle1[3][3];         
      u8 triangle2[3][3];
} TerrainQuad;

// 0x8013DC40
typedef struct  {
    void *mesh;
    u8 *terrainQuads;
    u8 **gridToLevelInteractionIndex;
    u8 *coreMapObjects;
    void *tileTextures;
    void *tilePalettes;
    void *coreMapObjectsTextures;
    void *coreMapObjectsPalettes;
    u8 *mapAdditionsMetadataPtr;
    MapGrid mapGrid; // 0x24
    Tile tiles[MAX_MAP_TILES]; // 0x30
    u16 textureToFirstGrid[MAX_TILE_TEXTURES + 1]; // 0x3030
    u16 nextGridPositionWithSharedTile[MAX_TILE_TEXTURES]; // 0x30D2
    u32 unused1[758];
    MapCameraView mapCameraView; // 0x3D4C
    CoreMapObjectsMetadata coreMapObjectsMetadata[16]; // 0x3D98
    MapObject mapObjects[12]; // 0x3DD8
    u32 unused2[20];
    WeatherSprite weatherSprites[16];
    GroundObjects groundObjects; // 0x4058, foragable items
    MapAdditions mapAdditions[32]; // 0x4C28
    GroundObjectBitmap groundObjectBitmaps[56]; // 0x5528
    u32 unused3[72]; // 0x5D08
    Gfx tileRenderingCommands[0x2800]; // 0x5E28, 0x80143A68
    u8 visibilityGrid[38][42]; // 0x19E28
    MapGlobals mapGlobals; // 0x1A464
    u32 unused4[80]; // 0x1A4C8
    MapState mapState; // 0x1A608
} MainMap;

extern void func_800337D0(void);    
extern bool setupMap(u16 mapIndex, u8* grid, void* mesh, u8* terrainQuads, u8** gridToLevelInteractionIndex, u8* coreMapObjectsData, void* tileTextures, void* tilePalettes, void* coreMapObjectsTextures, void* coreMapObjectsPalettes, u8 *mapAdditionsMetadataPtr);
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
extern bool setMapWeatherSprite(u16 arg0, u8 arg1, u16 arg2, u16 arg3);
extern bool func_80034DC8(u16, u8, u16);
extern bool deactivateMapObject(u16, u8);     
extern bool loadGroundObjects(u16 mapIndex, u8 arg1, u8 arg2, u32* arg3, u32* arg4, u8* arg5, u32 arg6, u32 arg7, u32 arg8, u32 arg9, u8 argA);
extern bool setMapGroundObjectSpriteIndex(u16 arg0, u16 arg1, u8 arg2, u8 arg3); 
extern bool setGroundObjectBitmap(u16 mapIndex, u16 bitmapIndex, u16 arg2, f32 arg3, f32 arg4, f32 arg5);
extern f32 getTerrainHeightAtPosition(u16 mapIndex, f32, f32);      
extern bool checkTileHasGroundObject(u16 mapIndex, f32 arg1, f32 arg2);
extern Vec3f getTileCoordinates(MainMap* arg1, f32 arg2, f32 arg3);
extern u8 getLevelInteractionIndexFromPosition(u16, f32, f32);
extern Vec3f convertWorldToTileCoordinates(u16, f32, f32);   
extern Vec3f getMapGroundObjectCoordinates(u16 mapIndex, f32 x, f32 z);
extern u16 getMapGroundObjectSpriteIndex(u16, f32, f32);  
extern bool setMapGroundObjectSpriteIndexFromFloat(u16, u16, f32, f32);
extern bool checkMapRGBADone(u16);      
extern void setGridToTileTextureMappings(u16 mapIndex);                                 
extern void setGroundObjects(u16 mapIndex); 
extern u16 getTileIndexFromGrid(u16, u8, u8);
extern bool updateGroundObjects(u16 mapIndex);
extern bool func_80038900(u16 mapIndex, u16 arg1, u16 arg2, u16 arg3, u16 arg4); 
extern bool func_80038990(u16, u16, u8);       
extern bool func_80038A2C(u16 mapIndex, u16 arg1, u8 arg2, u8 arg3);
extern bool setMapAddition(u16 mapIndex, u16 arg1);
extern bool setMapAdditionIndexFromCoordinates(u16, u16, u8, u8);   
extern void updateMapGraphics(void);
extern Vec3f func_8003AF58(u16, u8, u8); 

extern MainMap mainMap[1];

#endif