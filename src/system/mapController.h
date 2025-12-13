
#ifndef _MAP_CONTEXT_H_
#define _MAP_CONTEXT_H_

#include "common.h"

#define MAX_MAP_ADDRESSES 96


#define MAIN_MAP_INDEX 0

// 0x802055D8
typedef struct {
	u32 *mapDataIndex; // vaddr = 0x80255000
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
extern bool func_8003BD60(u16);                      
extern bool setMapControllerViewPosition(u16, f32, f32, f32);  
extern bool adjustMapControllerViewPosition(u16 arg0, f32 arg1, f32 arg2, f32 arg3);
extern bool func_8003BE98(u16, u8, u8, u8, u8);
extern bool func_8003BF7C(u16, u8, u8, u8, u8, s16);
extern bool func_8003C084(u16, u8);    
extern void updateMapController(void);
extern u8 getCurrentMapRotation(u16);
extern bool func_8003C1E0(u16 arg0, f32 arg1, f32 arg2, f32 arg3, u8 arg4, u8 arg5);
extern bool func_8003C504(u16);     
extern void func_8003C570(void);
extern bool func_8003C5C0(u16, u8, u8);

extern MapController mapControllers[1];

// 0x8017044C
//extern CurrentMapContext currentMapContext;

extern Vec4f D_8013D248;
// used by sprites
extern Vec4f D_802373F8;

// RGBA update rate
extern s16 D_8017045A;

#endif