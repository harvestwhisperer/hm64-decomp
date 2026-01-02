
#ifndef _MAP_CONTEXT_H_
#define _MAP_CONTEXT_H_

#include "common.h"

#define MAX_MAP_ADDRESSES 96

#define MAIN_MAP_INDEX 0

#define MAP_CONTROLLER_INITIALIZED 1
#define MAP_CONTROLLER_ASSETS_LOADED 2
#define MAP_CONTROLLER_ACTIVE 4
#define MAP_CONTROLLER_ROTATING_COUNTERCLOCKWISE 8
#define MAP_CONTROLLER_ROTATING_CLOCKWISE 0x10

// 0x802055D8
typedef struct {
	u32 *mapDataIndex; // virtual address for map data dma
	Vec3f viewPosition;
	Vec3f unk_10;
	Vec3f unk_1C;
	Vec3f unk_28;
	Vec3f unk_34; 
	u16 mainMapIndex;
	u16 mapIndex;
	u8 currentTileX;
	u8 currentTileZ;
	u8 rotationFrameCounter;
	u8 targetRotation;
	u8 rotation;
	u16 flags;
} MapController;

typedef struct {
    void *romStart;
    void *romEnd;
    u16 flags;
} MapDataAddress;

// possible struct
// 0x8017044C
/*
typedef struct {
	Vec3f currentWorldRotation;
	u8 currentMapIndex; // D_80170458/gBaseMapIndex
	u16 RGBARate; // D_8017045A
} CurrentMapContext;
*/

extern void initializeMapControllers();      
extern bool func_8003BA44(u16 index, u16 mapIndex, u32 *levelMap);
extern bool loadMap(u16 arg0, u16 mapIndex); 
extern bool dmaMapAssets(u16, u16);   
extern bool enableMapController(u16);                      
extern bool setMapControllerViewPosition(u16, f32, f32, f32);  
extern bool adjustMapControllerViewPosition(u16 arg0, f32 arg1, f32 arg2, f32 arg3);
extern bool setMapControllerRGBA(u16, u8, u8, u8, u8);
extern bool setMapControllerRGBAWithTransition(u16, u8, u8, u8, u8, s16);
extern bool setInitialMapRotation(u16, u8);    
extern void updateMapController(void);
extern u8 getCurrentMapRotation(u16);
extern bool setMapViewPositionAndCurrentTile(u16 arg0, f32 arg1, f32 arg2, f32 arg3, u8 arg4, u8 arg5);
extern bool unloadMapAssets(u16);     
extern void deactivateAllMapControllers(void);
extern bool startMapRotation(u16, u8, u8);

extern MapController mapControllers[1];

// 0x8017044C
//extern CurrentMapContext currentMapContext;

extern Vec4f targetMapLightingRGBA;
extern Vec4f currentMapLightingRGBA;

// RGBA update rate
extern s16 D_8017045A;

extern Vec3f previousWorldRotationAngles;
extern Vec3f currentWorldRotationAngles;

#endif