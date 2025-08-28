#ifndef _WORLD_GRAPHICS_H_
#define _WORLD_GRAPHICS_H_

#include "common.h"

#define MAX_SCENE_NODES 0x150

#define ACTIVE 1
#define UPDATE_SCALE 0x10
#define UPDATE_ROTATION 0x20
// 0x40 don't update translation based on rotation
// 0x80 = bump up z value

typedef struct {
	Vec3f positions;
	Vec3f scaling;
	Vec3f rotation;
	u16 spriteCount;
	Mtx translation;
	Mtx scale;
	Mtx rotationX;
	Mtx rotationY;
	Mtx rotationZ;
	Gfx *dl;
	u16 flags;
} SceneNode;

typedef struct {
    Mtx projection;
	Mtx orthographic;
	Mtx translationM;
	Mtx scale;
	Mtx rotationX;
	Mtx rotationY;
	Mtx rotationZ;
    Mtx viewing;
	Vec3f translation;
	Vec3f scaling;
	Vec3f rotation;
	u32 unknown[0x2B];
} SceneMatrices;

extern void initializeSceneNodes(void);   
extern u16 addSceneNode(Gfx *dl, u16 flags);
extern bool addSceneNodePosition(u16 index, f32 x, f32 y, f32 z);
extern bool addSceneNodeScaling(u16 index, f32 arg1, f32 arg2, f32 arg3);
extern bool addSceneNodeRotation(u16 index, f32 arg1, f32 arg2, f32 arg3);
extern void func_800293B8(void);
extern Gfx* renderSceneGraph(Gfx* dl, SceneMatrices* arg1);

extern Vec3f previousWorldRotationAngles;
extern Vec3f currentWorldRotationAngles;

extern SceneMatrices sceneMatrices[2];

#endif