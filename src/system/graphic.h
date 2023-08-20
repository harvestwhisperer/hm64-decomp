#ifndef _GRAPHIC_H_
#define _GRAPHIC_H_

#define SCREEN_WD 320
#define SCREEN_HT 240
#define GFX_GLIST_LEN 31

typedef struct {
	Mtx matrix1;
	Mtx matrix2;
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
	Vec3f position;
	Vec3f scaling;
	Vec3f rotation;
	Mtx translation;
	Mtx scale;
	Mtx rotationX;
	Mtx rotationY;
	Mtx rotationZ;
	Gfx *dl;
	u16 flags;
} WorldGraphics;

typedef struct {
	Mtx projection;
	Mtx viewing;
	Mtx translation;
	Mtx scale;
	Mtx rotationX;
	Mtx rotationY;
	Mtx rotationZ;
	Vec3f translationCoords;
	Vec3f scaleCoords;
	Vec3f rotationCoords;
	Mtx unk_B90;
} WorldMatrices;

// need to fix this struct
typedef struct {
    u32 unk[171];
	Vec3f translationCoords;
	Vec3f scaleCoords;
	Vec3f rotationCoords;
} UnknownGraphicsStruct1;

typedef struct {
    u32 padding[4];
    u8 unk_10;
    u8 unk_11;
    u8 unk_12;
} UnknownGraphicsStruct2;

typedef struct {
    f32 angle1;
    f32 angle2;
    f32 angle3;
    f32 angle4;
    f32 angle5;
    f32 angle6;
    f32 angle7;
    f32 angle8;
} Angles;

#endif