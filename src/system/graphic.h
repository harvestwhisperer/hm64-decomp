#ifndef _GRAPHIC_H_
#define _GRAPHIC_H_

#include "common.h"
#include "system/map.h"
#include "system/sprite.h"

#define SCREEN_WD 320
#define SCREEN_HT 240
#define GFX_GLIST_LEN 31

#define DEGREES_TO_RADIANS_CONSTANT 0.0174532925199432955

#define NORTH 0
#define NORTHEAST 1
#define EAST 2
#define SOUTHEAST 3
#define SOUTH 4
#define SOUTHWEST 5
#define WEST 6
#define NORTHWEST 7
#define MAX_DIRECTIONS 7

#define convertSpriteToWorldDirection(direction, mapIndex) ((direction + 8) - getCurrentMapRotation(mapIndex)) % 8  
#define convertWorldToSpriteDirection(direction, mapIndex) (direction + getCurrentMapRotation(mapIndex)) % 8 

typedef struct {
	Mtx projection;
	Mtx viewing;
	f32 l;
	f32 r;
	f32 t;
	f32 b;
	f32 n;
	f32 f;
	f32 fov;
	f32 aspect;
	f32 near;
	f32 far;
	Vec3f eye;
	Vec3f at;
	Vec3f up;
	u8 perspectiveMode;
} Camera;

typedef struct {
	f32 x;
	f32 y;
	f32 z;
	f32 w;
} Plane;

typedef struct {
    u32 padding[4];
    u8 unk_10;
    u8 unk_11;
    u8 unk_12;
} UnknownGraphicsStruct;

extern void graphicsInit();
extern f32 sinfRadians(f32);
extern f32 cosfRadians(f32);
extern void setBitmapFormat(BitmapObject *sprite, Texture *timg, u16 *palette);
extern void setBitmapFormatAndSize(BitmapObject* sprite, u16* palette);
extern Gfx* loadBitmapTexture(Gfx* dl, BitmapObject* arg1, u32 arg2, u16 arg3);
extern void setupBitmapVertices(Vtx vtxs[], u16 width, u16 height, u16 textureSize, u16 textureOffset, u16 flipHorizontal, u16 flipVertical, s16 anchorX, s16 anchorY, u16 flags, u8 r, u8 g, u8 b, u8 a);
extern void rotateVector3D(Vec3f inputVec, Vec3f* outputVec, Vec3f rotationAngles);
extern inline Plane calculatePlaneEquation(f32 x1, f32 y1, f32 z1, f32 x2, f32 y2, f32 z2, f32 x3, f32 y3, f32 z3);
extern f32 getHeightFromPlane(f32 x, f32 z, Plane vec);
extern u8 isPointInTriangle(f32 pointX, f32 pointY, f32 pointZ, f32 v1x, f32 v1y, f32 v1z, f32 v2x, f32 v2y, f32 v2z, f32 v3x, f32 v3y, f32 v3z);
extern f32 evaluatePlaneEquation(f32 x, f32 y, f32 z, Plane plane);
extern Vec3f getMovementVectorFromDirection(f32 zDisplacement, u8 direction, f32 yOffset);    
extern u8* getTexturePtr(u16 index, u32* timgLookup);                   
extern u8 *getPalettePtrType1(u16 index, u32 *paletteLookup);
extern u8* getPalettePtrType2(u16 index, u32 *paletteLookup, u8 *arg2);
extern f32 getSpriteYValueFromDirection(u8 direction);    
extern void setInitialWorldRotationAngles(f32 x, f32 y, f32 z);
extern void func_80028EF8(f32 x, f32 y, f32 z);
extern void drawFrame(void);  

#endif