#include "common.h"

#include "system/graphic.h"

#include "system/sprite.h"
#include "system/sceneGraph.h"

#include "mainproc.h"

// forward declarations
void setBitmapFormatAndSize(BitmapObject* sprite, u16* palette);
Gfx *setupCameraMatrices(Gfx*, Camera*, SceneMatrices*);
volatile u8 doViewportGfxTask(void);      
void setInitialWorldRotationAngles(f32, f32, f32);            

Gfx* clearFramebuffer(Gfx* dl);                  
Gfx* initRcp(Gfx*);                               
volatile u8 startGfxTask(void);

void setCameraLookAt(Camera*, f32, f32, f32, f32, f32, f32, f32, f32, f32); 
void setCameraOrthographicValues(Camera*, f32, f32, f32, f32, f32, f32); 
void setCameraPerspectiveValues(Camera*, f32, f32, f32, f32);    

// bss
static LookAt gSPLookAtBufferA;
static LookAt gSPLookAtBufferB;

// non-contigous bss
Camera gCamera;
Gfx initGfxList[2][0x20];
Gfx sceneGraphDisplayList[2][0x500];
Gfx D_80205000[2][0x20];
extern Gfx viewportDL[3];
                        
// data, possibly external
extern Gfx setup_rdpstate[];
extern Gfx setup_rspstate[];
extern Gfx rdpstateinit_dl[];

// data
extern NUUcode nugfx_ucode[];
extern u16*	FrameBuf[3];

// rodata
static const char gfxExceptionStr1[] = "EX";
static const char gfxExceptionStr2[] = "s:/system/graphic.c";

// shared globals
// also used by mapContext.c, map.c, and sceneGraph.c
extern Vec3f previousWorldRotationAngles;
extern Vec3f currentWorldRotationAngles;
extern f32 D_80170450;
extern f32 D_80170454;


//INCLUDE_RODATA("asm/nonmatchings/systemgraphic", D_8011EC40);

//INCLUDE_ASM("asm/nonmatchings/system/graphic", graphicsInit);

void graphicsInit(void) {

    u8 i;

    nuGfxInit();
 
    gCamera.perspectiveMode = FALSE;

    setCameraOrthographicValues(&gCamera, -160.0f, 160.0f, -120.0f, 120.0f, 0.0f, 900.0f);
    setCameraPerspectiveValues(&gCamera, 33.0f, 1.3333334f, 10.0f, 1000.0f);
    setCameraLookAt(&gCamera, 0.0f, 0.0f, 400.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.1f, 0.0f);

    for (i = 0; i < 2; i++) {
        sceneMatrices[i].translation.x = 0.0f;
        sceneMatrices[i].translation.y = 0.0f;
        sceneMatrices[i].translation.z = 0.0f;
        sceneMatrices[i].scaling.x = 1.0f;
        sceneMatrices[i].scaling.y = 1.0f;
        sceneMatrices[i].scaling.z = 1.0f;
        sceneMatrices[i].rotation.x = 0.0f;
        sceneMatrices[i].rotation.y = 0.0f;
        sceneMatrices[i].rotation.z = 0.0f;
    }

    setInitialWorldRotationAngles(45.0f, 315.0f, 0.0f);  
    
}

//INCLUDE_ASM("asm/nonmatchings/system/graphic", drawFrame);

void drawFrame(void) {

    gfxTaskNo = 0;

    startGfxTask();
    renderScene();
    doViewportGfxTask();

    gGraphicsBufferIndex ^= 1;

}

//INCLUDE_ASM("asm/nonmatchings/system/graphic", startGfxTask);

volatile u8 startGfxTask(void) {

    Gfx *dl;
 
    dl = initRcp(initGfxList[gGraphicsBufferIndex]);
    dl = clearFramebuffer(dl);

    gDPFullSync(dl++);
    gSPEndDisplayList(dl++);

    if (dl - initGfxList[gGraphicsBufferIndex] > GFX_GLIST_LEN) {
        // FIXME: get string literals working
        __assert(&gfxExceptionStr1, &gfxExceptionStr2, 288);
    }
    
    nuGfxTaskStart(initGfxList[gGraphicsBufferIndex], (s32)(dl - initGfxList[gGraphicsBufferIndex]) * sizeof(Gfx), NU_GFX_UCODE_F3DEX, NU_SC_NOSWAPBUFFER);
    
    gfxTaskNo += 1;
    
    return gfxTaskNo;
}

//INCLUDE_ASM("asm/nonmatchings/system/graphic", doViewportGfxTask);

volatile u8 doViewportGfxTask(void) {

    Gfx *dl = D_80205000[gGraphicsBufferIndex];
    
    gSPDisplayList(dl++, OS_K0_TO_PHYSICAL(&viewportDL));
    gDPFullSync(dl++);
    gSPEndDisplayList(dl++);

    if (dl - D_80205000[gGraphicsBufferIndex] >= 32) {
        // FIXME: get string literals working
        __assert(&gfxExceptionStr1, &gfxExceptionStr2, 319);
    }

    nuGfxTaskStart(D_80205000[gGraphicsBufferIndex], (s32)(dl - D_80205000[gGraphicsBufferIndex]) * sizeof(Gfx), NU_GFX_UCODE_F3DEX, NU_SC_SWAPBUFFER);
    
    gfxTaskNo += 1;

    return gfxTaskNo;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/graphic", renderScene);

volatile u8 renderScene(s32 arg0, s32 arg1) {

    Gfx *dl = sceneGraphDisplayList[gGraphicsBufferIndex];
    
    gSPDisplayList(dl++, OS_K0_TO_PHYSICAL(&viewportDL));
    
    dl = setupCameraMatrices(dl++, &gCamera, &sceneMatrices[gGraphicsBufferIndex]);
    dl = renderSceneGraph(dl++, &sceneMatrices[gGraphicsBufferIndex]);
    
    gDPFullSync(dl++);
    gSPEndDisplayList(dl++);
    
    if (dl - sceneGraphDisplayList[gGraphicsBufferIndex] >= 0x500) {
        // FIXME: get string literals working
        __assert(&gfxExceptionStr1, &gfxExceptionStr2, 0x166);
    }

    nuGfxTaskStart(sceneGraphDisplayList[gGraphicsBufferIndex], (s32)(dl - sceneGraphDisplayList[gGraphicsBufferIndex]) * sizeof(Gfx), NU_GFX_UCODE_F3DEX, NU_SC_NOSWAPBUFFER);
    
    gfxTaskNo += 1;
    
    return gfxTaskNo;

}

//INCLUDE_ASM("asm/nonmatchings/system/graphic", setBitmapFormat);

// FIXME: something wrong, but matches
static inline u16 swap16(u16 halfword) {

    u32 padding[9];

    u16 swapped;
    
    u32 upper;
    u32 lower;

    lower = halfword;

    upper = (halfword & 0xFF) << 8;
    lower = halfword >> 8;
    
    swapped = lower | upper;
    
    return swapped;
    
}

void setBitmapFormat(BitmapObject *sprite, Texture *timg, u16 *palette) {

    setBitmapFormatAndSize(sprite, palette);
    
    // skip header and size bytes
    sprite->timg = &timg->texture;
    
    // bytes 4-8 = width and height (16 bit) (byte swapped)
    sprite->width = swap16(timg->width);  
    sprite->height = swap16(timg->height);
    
    // get pixel size from bit 5 in header (bit one when swapped)
    switch ((timg->flags >> 4) & 0xF) {
        case 0:
          sprite->fmt = G_IM_FMT_CI;
          sprite->pixelSize = G_IM_SIZ_8b;
          break;
        
        case 1:
          sprite->fmt = G_IM_FMT_CI;
          sprite->pixelSize = G_IM_SIZ_4b; 
          break;
    }

}

/*
static inline u16 getWidth(u16* timg) {
    
    return swap(timg[2]);

}

static inline u16 getHeight(u16* timg) {

    return swap(timg[3]);
    
}
*/

// alternate with timg as array
/*
void setBitmapFormat(Bitmap *sprite, u16 *timg, u16 *palette) {
    
    u32 padding[10];

    setBitmapFormatAndSize(sprite, palette);
    
    // skip header and size bytes
    sprite->timg = timg + 4;
    
    // bytes 4-8 = width and height (16 bit) (byte swapped)
    sprite->width = getWidth(timg);  
    sprite->height = getHeight(timg);
    
    // get pixel size from bit 5 in header (bit one when swapped)
    switch (*(timg + 1) >> 4) & (1 | 2 | 4 | 8)) {
        case 0:
          sprite->fmt = G_IM_FMT_CI;
          sprite->pixelSize = G_IM_SIZ_8b;
          break;
        
        case 1:
          sprite->fmt = G_IM_FMT_CI;
          sprite->pixelSize = G_IM_SIZ_4b; 
          break;
    }
}

*/

//INCLUDE_ASM("asm/nonmatchings/system/graphic", setBitmapFormatAndSize);

void setBitmapFormatAndSize(BitmapObject* sprite, u16* palette) {

    // FIXME: shouldn't be necessary
    u32 padding[5];
    
    // skip header
    sprite->pal = palette + 2;

    // get pixel size from bit 5 in header (bit one when swapped)
    switch ((*(palette + 1) >> 4) & (1 | 2 | 4 | 8)) {                           
        case 0:
            sprite->fmt = G_IM_FMT_CI;
            sprite->pixelSize = G_IM_SIZ_8b;
            return;
        case 1:
            sprite->fmt = G_IM_FMT_CI;
            sprite->pixelSize = G_IM_SIZ_4b;
            return;
        }
}

//INCLUDE_ASM("asm/nonmatchings/system/graphic", loadBitmapTexture);

Gfx* loadBitmapTexture(Gfx* dl, BitmapObject* sprite, u32 offset, u16 height) {
    
    switch (sprite->pixelSize) {
        case G_IM_SIZ_4b:
            gDPLoadTextureTile_4b(dl++, sprite->timg + offset, sprite->fmt, sprite->width, sprite->height, 0, 0, sprite->width - 1, height - 1, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);        
            gDPLoadTLUT_pal16(dl++, 0, sprite->pal);
            break; 
        case G_IM_SIZ_8b:
            gDPLoadTextureTile(dl++, sprite->timg + offset, sprite->fmt, G_IM_SIZ_8b, sprite->width, sprite->height, 0, 0, sprite->width - 1, height - 1, sprite->pal, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
            gDPLoadTLUT_pal256(dl++, sprite->pal);
            break;
        case G_IM_SIZ_16b:
            gDPLoadTextureBlock(dl++, sprite->timg + offset, sprite->fmt, G_IM_SIZ_16b, sprite->width, height, sprite->pal, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
            break;
        case G_IM_SIZ_32b:
            gDPLoadTextureBlock(dl++, sprite->timg + offset, sprite->fmt, G_IM_SIZ_32b, sprite->width, height, sprite->pal, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
            break;
        default:
            break;
    }

    return dl++;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/graphic", setupBitmapVertices);

void setupBitmapVertices(Vtx vtxs[], 
    u16 width, u16 height, 
    u16 textureSize, u16 textureOffset, 
    u16 flipHorizontal, u16 flipVertical, 
    s16 anchorX, s16 anchorY, 
    u16 flags, 
    u8 r, u8 g, u8 b, u8 a) {
    
    u8 coordinate1;
    u8 coordinate2;
    u8 coordinate3;
    
    u16 centerOffsetY;
    u16 temp;
    u16 centerOffsetX;
    u16 temp2;

    // coordinate mapping
    switch ((flags >> 7) & 3) {

        case 2:
            coordinate1 = 0;
            coordinate2 = 1;
            coordinate3 = 2;
            break;

        case 3:
            coordinate1 = 0;
            coordinate2 = 2;
            coordinate3 = 1;
            break;

        case 0:
        case 1:
        default:
            break;

    }

    centerOffsetY = height / 2;
    centerOffsetX = width / 2;
    
    // horizontal flip
    if (flipHorizontal) {

        vtxs[0].v.tc[0] = width << 6;
        vtxs[3].v.tc[0] = width << 6;
        
        vtxs[1].v.tc[0] = 0;
        vtxs[2].v.tc[0] = 0;
        
        vtxs[0].v.ob[coordinate1] = -centerOffsetX - anchorX;
        vtxs[3].v.ob[coordinate1] = -centerOffsetX - anchorX;
        
        vtxs[2].v.ob[coordinate1] = vtxs[1].v.ob[coordinate1] = vtxs[0].v.ob[coordinate1] + width;
        
    } else {
        
        vtxs[1].v.tc[0] = width << 6;
        vtxs[2].v.tc[0] = width << 6;
        
        vtxs[0].v.tc[0] = 0;
        vtxs[3].v.tc[0] = 0;
        
        vtxs[0].v.ob[coordinate1] = anchorX - centerOffsetX;
        vtxs[3].v.ob[coordinate1] = anchorX - centerOffsetX;
        
        vtxs[2].v.ob[coordinate1] = vtxs[1].v.ob[coordinate1] = vtxs[0].v.ob[coordinate1] + width;  
        
    }
    
    vtxs[2].v.tc[1] = textureSize << 6;
    vtxs[3].v.tc[1] = textureSize << 6;
    
    vtxs[0].v.tc[1] = 0;
    vtxs[1].v.tc[1] = 0;
    
    // vertical alignment
    switch ((flags >> 5) & 3) {

        case 2:
            vtxs[0].v.ob[coordinate2] = (centerOffsetY - textureOffset) - anchorY;
            vtxs[1].v.ob[coordinate2] = (centerOffsetY - textureOffset) - anchorY;
            vtxs[3].v.ob[coordinate2] = vtxs[2].v.ob[coordinate2] = temp = vtxs[0].v.ob[coordinate2] - textureSize;
            break;

        case 1:
            vtxs[0].v.ob[coordinate2] = textureOffset - anchorY;
            vtxs[1].v.ob[coordinate2] = textureOffset - anchorY;
            vtxs[3].v.ob[coordinate2] = vtxs[2].v.ob[coordinate2] = temp2 = vtxs[0].v.ob[coordinate2] - textureSize;
            break;

        case 3:
            vtxs[0].v.ob[coordinate2] = (height - textureOffset) - anchorY;
            vtxs[1].v.ob[coordinate2] = (height - textureOffset) - anchorY;
            vtxs[3].v.ob[coordinate2] = vtxs[2].v.ob[coordinate2] = temp = vtxs[0].v.ob[coordinate2] - textureSize;
            break;

        default:
            break;

    }

    vtxs[0].v.ob[coordinate3] = 0;
    vtxs[1].v.ob[coordinate3] = 0;
    vtxs[2].v.ob[coordinate3] = 0;
    vtxs[3].v.ob[coordinate3] = 0;

    vtxs[0].v.cn[0] = r;
    vtxs[0].v.cn[1] = g;
    vtxs[0].v.cn[2] = b;
    vtxs[0].v.cn[3] = a;
    
    vtxs[1].v.cn[0] = r;
    vtxs[1].v.cn[1] = g;
    vtxs[1].v.cn[2] = b;
    vtxs[1].v.cn[3] = a;

    vtxs[2].v.cn[0] = r;
    vtxs[2].v.cn[1] = g;
    vtxs[2].v.cn[2] = b;
    vtxs[2].v.cn[3] = a;
    
    vtxs[3].v.cn[0] = r;
    vtxs[3].v.cn[1] = g;
    vtxs[3].v.cn[2] = b;
    vtxs[3].v.cn[3] = a;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/graphic", sinfRadians);

f32 sinfRadians(f32 angle) {
    return sinf(angle * DEGREES_TO_RADIANS_CONSTANT);
}

//INCLUDE_ASM("asm/nonmatchings/system/graphic", cosfRadians);

f32 cosfRadians(f32 angle) {
    return cosf(angle * DEGREES_TO_RADIANS_CONSTANT);
}

//INCLUDE_ASM("asm/nonmatchings/system/graphic", rotateVector3D);

void rotateVector3D(Vec3f inputVec, Vec3f* outputVec, Vec3f rotationAngles) {
    
    f32 sinX;
    f32 cosX;
    f32 sinY;
    f32 cosY;
    f32 sinZ;
    f32 cosZ;

    f32 tempX;
    f32 tempY;
    f32 tempZ;
    
    f32 tempRadiansX = rotationAngles.x * DEGREES_TO_RADIANS_CONSTANT;
    f32 tempRadiansY = rotationAngles.y * DEGREES_TO_RADIANS_CONSTANT;
    f32 tempRadiansZ = rotationAngles.z * DEGREES_TO_RADIANS_CONSTANT;
    
    sinX = sinf(tempRadiansX);
    cosX = cosf(tempRadiansX);
    sinY = sinf(tempRadiansY);
    cosY = cosf(tempRadiansY);
    sinZ = sinf(tempRadiansZ);
    cosZ = cosf(tempRadiansZ);
    
    outputVec->x = inputVec.x;
    outputVec->y = inputVec.y;
    outputVec->z = inputVec.z;
    
    tempX = outputVec->x;
    tempY = outputVec->y;
    tempZ = outputVec->z;
    
    if (tempRadiansZ != 0.0f) {
        
        outputVec->x = (-tempY * sinZ) + (tempX * cosZ);
        outputVec->y = (tempY * sinZ) + (tempX * sinZ);
        outputVec->z = tempZ;

    }
    
    tempX = outputVec->x;
    tempY = outputVec->y;
    tempZ = outputVec->z;
    
    if (tempRadiansY != 0.0f) {
        
        outputVec->x = (tempZ * sinY) + (tempX * cosY);
        outputVec->y = tempY;
        outputVec->z = (tempZ * cosY) - (tempX * sinY);
   
    }
    
    tempX = outputVec->x;
    tempY = outputVec->y;
    tempZ = outputVec->z;
    
    if (tempRadiansX != 0.0f) {
    
        outputVec->x = tempX;
        outputVec->y = (-tempZ * sinX) + (tempY * cosX);
        outputVec->z = (tempZ * cosX) + (tempY * sinX);
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/graphic", func_80027B74);

// unused or inline
void func_80027B74(Vec3f arg0, Vec3f* arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6) {
    
    f32 temp;
    
    f32 tempX;
    f32 tempY;
    f32 tempZ;
    
    tempX = (arg0.z * arg3) + (arg0.x * arg6);
    
    temp = (arg0.z * arg6) - (arg0.x * arg3);
    
    tempY = (-temp * arg2) + (arg0.y * arg5);
    tempZ = (temp * arg5) + (arg0.y * arg2);
    
    arg1->x = tempX;
    arg1->y = tempY;
    arg1->z = tempZ;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/graphic", calculatePlaneEquation);

inline Plane calculatePlaneEquation( 
    f32 x1, f32 y1, f32 z1, 
    f32 x2, f32 y2, f32 z2, 
    f32 x3, f32 y3, f32 z3) {
    
    Plane plane;

    f32 normalX;
    f32 normalY;
    f32 normalZ;
    
    f32 magnitude;
    
    // calculate cross product
    normalX = ((y1 * (z2 - z3)) + (y2 * (z3 - z1))) + (y3 * (z1 - z2));
    normalY = ((z1 * (x2 - x3)) + (z2 * (x3 - x1))) + (z3 * (x1 - x2));
    normalZ = ((x1 * (y2 - y3)) + (x2 * (y3 - y1))) + (x3 * (y1 - y2));
    
    // normalize
    magnitude = sqrtf((normalX * normalX) + (normalY * normalY) + (normalZ * normalZ));
    
    if (magnitude == 0) {
        
        plane.x = 0.0f;
        plane.y = 0.0f;
        plane.z = 0.0f;
        plane.w = 0.0f;
        
    } else {
        
        normalX /= magnitude;
        normalY /= magnitude;
        normalZ /= magnitude;
        
        // plane distance: w = -(normal.x * point.x + normal.y * point.y + normal.z * point.z)
        // -(normal · point1)
        plane.x = normalX;
        plane.y = normalY;
        plane.z = normalZ;
        plane.w = -(((normalX * x1) + (normalY * y1)) + (normalZ * z1));

    }
    
    return plane;

}

//INCLUDE_ASM("asm/nonmatchings/system/graphic", getHeightFromPlane);

f32 getHeightFromPlane(f32 x, f32 z, Plane planeEquation) {

    f32 result;
    
    result = 0.0f;
    
    if (planeEquation.y != 0.0f) {
        result = ( (-(planeEquation.x*x) - (planeEquation.z * z)) - planeEquation.w) / planeEquation.y;
    }
    

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/graphic", isPointInTriangle)

u8 isPointInTriangle(f32 pointX, f32 pointY, f32 pointZ, f32 v1x, f32 v1y, f32 v1z, f32 v2x, f32 v2y, f32 v2z, f32 v3x, f32 v3y, f32 v3z) {

    Plane plane;
    
    u8 count = 0;
    
    plane = calculatePlaneEquation(v1x, v1y, v1z, v2x, v2y, v2z, pointX, pointY, pointZ);
    
    if (plane.y >= 0.0f) {
        count++;
    }
    
    plane = calculatePlaneEquation(v2x, v2y, v2z, v3x, v3y, v3z, pointX, pointY, pointZ);

    if (plane.y >= 0.0f) {
        count++;
    }
    
    plane = calculatePlaneEquation(v3x, v3y, v3z, v1x, v1y, v1z, pointX, pointY, pointZ);
    
    if (plane.y >= 0.0f) {
        count++;
    }
    
    count /= 3;
    
    return count;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/graphic", evaluatePlaneEquation);

f32 evaluatePlaneEquation(f32 x, f32 y, f32 z, Plane plane) {
    
    f32 ax = plane.x * x;
    f32 by = plane.y * y;
    f32 cz = plane.z * z;
    f32 d = plane.w * 1;
  
    return ax + by + cz + d;
    
}

// alternate
/*
f32 evaluatePlaneEquation(f32 arg0, f32 arg1, f32 arg2, Plane coordinates) {
    f32 temp = coordinates.x * arg0;
  
    return (temp) + (coordinates.y * arg1) + (coordinates.z * arg2) + coordinates.w;    
}
*/

//INCLUDE_RODATA("asm/nonmatchings/systemgraphic", directionsToYValues);

static const f32 directionsToYValues[8] = {
    0.0f,   // north
    315.0f, // northeast
    270.0f, // east
    225.0f, // southeast 
    180.0f, // south
    135.0f, // southwest
    90.0f,  // west
    45.0f   // northwest
};

//INCLUDE_ASM("asm/nonmatchings/system/graphic", getMovementVectorFromDirection);

// Alternate: Vec3f* getMovementVectorFromDirection(Vec3f *outputVec, f32 zDisplacement, u8 direction, f32 yOffset)

Vec3f getMovementVectorFromDirection(f32 zDisplacement, u8 direction, f32 yOffset) {

    f32 buffer[8];

    f32 sinX;
    f32 cosX;
    f32 sinY;
    f32 cosY;
    f32 sinZ;
    f32 cosZ;

    f32 temp1;
    f32 temp2;
    f32 temp3;
    
    Vec3f vec1;
    Vec3f vec2;
    Vec3f vec3;
    Vec3f vec4;
    Vec3f vec5;
    
    f32 x;
    f32 y;
    f32 z;

    memcpy(buffer, directionsToYValues, 32);
    
    if (direction != 0xFF) {

        vec1.x = 0.0f;
        vec1.y = yOffset;
        vec1.z = zDisplacement;

        vec3.x = 0.0f;
        vec3.y = buffer[direction];
        vec3.z = 0.0f;

        vec4 = vec1;
        vec5 = vec3;

        x = vec5.x * DEGREES_TO_RADIANS_CONSTANT;
        y = vec5.y * DEGREES_TO_RADIANS_CONSTANT;
        z = vec5.z * DEGREES_TO_RADIANS_CONSTANT;

        sinX = sinf(x);
        cosX = cosf(x);

        sinY = sinf(y);
        cosY = cosf(y);

        sinZ = sinf(z);
        cosZ = cosf(z);

        vec2.x = vec4.x;
        vec2.y = vec4.y;        
        vec2.z = vec4.z;
        
        temp1 = vec4.x;
        temp2 = vec4.y;
        temp3 = vec4.z;

        if (z != 0.0f) {
            vec2.x = (-temp2 * sinZ) + (temp1 * cosZ);
            vec2.y = (temp2 * sinZ) + (temp1 * sinZ);
            vec2.z = temp3;
        }

        temp1 = vec2.x;
        temp2 = vec2.y;
        temp3 = vec2.z;
        
        if (y != 0.0f) {
            vec2.x = (temp3 * sinY) + (temp1 * cosY);
            vec2.y = temp2;
            vec2.z = (temp3 * cosY) - (temp1 * sinY);
        }

        temp1 = vec2.x;
        temp2 = vec2.y;
        temp3 = vec2.z;
        
        if (x != 0.0f) {
            vec2.x = temp1;
            vec2.y = (-temp3 * sinX) + (temp2 * cosX);
            vec2.z = (temp3 * cosX) + (temp2 * sinX);
        }
        
    } else {
        vec2.x = 0.0f;
        vec2.y = yOffset;
        vec2.z = 0.0f;
    }

    return vec2;

    // alternate return type
    /*
    *outputVec = vec2;
    
    return outputVec;
    */

}

//INCLUDE_ASM("asm/nonmatchings/system/graphic", getSpriteYValueFromDirection);

// get y value for sprite based on direction
f32 getSpriteYValueFromDirection(u8 direction) {

    f32 buffer[8];

    memcpy(buffer, directionsToYValues, 32);
    
    return buffer[direction];

}
 
//INCLUDE_ASM("asm/nonmatchings/system/graphic", getTexturePtr);

// get ptr to ci texture from index
u8* getTexturePtr(u16 spriteIndex, u32* textureIndex) {
    return (u8*)textureIndex + textureIndex[spriteIndex];
}

// alternate
/*
void *getTexturePtr(u16 arg0, u32 *arg1) {
  void *res = arg1;
  return res + *(arg1 + arg0);
}
*/

//INCLUDE_ASM("asm/nonmatchings/system/graphic", getPalettePtrType1);

// get ptr to palette
// FIXME: should return u16*?
u8 *getPalettePtrType1(u16 index, u32 *paletteIndex) {
  return (u8*)paletteIndex + paletteIndex[index];
}

//INCLUDE_ASM("asm/nonmatchings/system/graphic", getPalettePtrType2);

// returns palette pointer from sprite-to-palette mapping table
// used if global sprite flag 0x100 is set
u8* getPalettePtrType2(u16 spriteIndex, u32* paletteIndex, u8* spriteToPaletteIndex) {
    
    u8* arr = spriteToPaletteIndex + 4;
    u16 i = arr[spriteIndex];
    
    return (u8*)paletteIndex + paletteIndex[i]; 

}

// alternate
/*
u8* getPalettePtrType2(u16 arg0, u32 *arg1, u8 *arg2) {
    return (u8*)arg1 + *(arg1 + *(arg0 + arg2 + 4));
}
*/

//INCLUDE_ASM("asm/nonmatchings/system/graphic", initRcp);

Gfx* initRcp(Gfx* dl) {
    
    gSPSegment(dl++, 0, 0x0);
    gSPDisplayList(dl++, OS_K0_TO_PHYSICAL(setup_rspstate));
    gSPDisplayList(dl++, OS_K0_TO_PHYSICAL(setup_rdpstate));

    return dl++;

}

//INCLUDE_ASM("asm/nonmatchings/system/graphic", clearFramebuffer);

Gfx* clearFramebuffer(Gfx* dl) {

    gDPSetDepthImage(dl++, OS_K0_TO_PHYSICAL(nuGfxZBuffer));
    gDPPipeSync(dl++);
    gDPSetCycleType(dl++, G_CYC_FILL);
    gDPSetColorImage(dl++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD, OS_K0_TO_PHYSICAL(nuGfxZBuffer));
    gDPSetFillColor(dl++,(GPACK_ZDZ(G_MAXFBZ,0) << 16 | GPACK_ZDZ(G_MAXFBZ,0)));
    gDPFillRectangle(dl++, 0, 0, SCREEN_WD-1, SCREEN_HT-1);
    gDPPipeSync(dl++);
    
    gDPSetColorImage(dl++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD, osVirtualToPhysical(nuGfxCfb_ptr));   
    gDPSetFillColor(dl++, (GPACK_RGBA5551(0, 0, 0, 1) << 16 | GPACK_RGBA5551(0, 0, 0, 1)));
    gDPFillRectangle(dl++, 0, 0, SCREEN_WD-1, SCREEN_HT-1);
    gDPPipeSync(dl++);
    
    return dl++;

}

//INCLUDE_ASM("asm/nonmatchings/system/graphic", setupCameraMatrices);

Gfx* setupCameraMatrices(Gfx* dl, Camera* camera, SceneMatrices* matrices) {

    u16 perspNorm;

    switch (camera->perspectiveMode) {
        case 0:
            guOrtho(&matrices->projection, camera->l, camera->r, camera->t, camera->b, camera->n, camera->f, 0.9999f);
            break;
        case 1:
            guPerspective(&matrices->projection, &perspNorm, camera->fov, camera->aspect, camera->near, camera->far, 1.0f);
            gSPPerspNormalize(dl++, perspNorm);
            break;
        default:
            break;
    }

    guLookAt(&matrices->viewing, camera->eye.x, camera->eye.y, camera->eye.z, camera->at.x, camera->at.y, camera->at.z, camera->up.x, camera->up.y, camera->up.z);
    gSPLookAt(dl++, &gSPLookAtBufferA);

    gSPMatrix(dl++, &matrices->projection, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPMatrix(dl++, &matrices->viewing, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);

    return dl++;

}

//INCLUDE_ASM("asm/nonmatchings/system/graphic", func_80028C00);

// unused
Gfx* func_80028C00(Gfx* dl, Camera* camera) {
 
    u16 perspNorm;
    
    switch (camera->perspectiveMode) {
        case 0:
            guOrtho(&camera->projection, camera->l, camera->r, camera->t, camera->b, camera->n, camera->f, 1.0f);
            break;
        case 1:
            guPerspective(&camera->projection, &perspNorm, camera->fov, camera->aspect, camera->near, camera->far, 1.0f);
            gSPPerspNormalize(dl++, perspNorm);
            break;
        default:
            break;
    }

    guLookAt(&camera->viewing, camera->eye.x, camera->eye.y, camera->eye.z, camera->at.x, camera->at.y, camera->at.z, camera->up.x, camera->up.y, camera->up.z);
    gSPLookAt(dl++, &gSPLookAtBufferB);

    gSPMatrix(dl++, &camera->projection, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPMatrix(dl++, &camera->viewing, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);

    return dl++;

}

//INCLUDE_ASM("asm/nonmatchings/system/graphic", setCameraOrthographicValues);

void setCameraOrthographicValues(Camera* camera, f32 l, f32 r, f32 t, f32 b, f32 n, f32 f) {
    
    camera->l = l;
    camera->r = r;
    camera->t = t;
    camera->b = b;
    camera->n = n;
    camera->f = f;

}

//INCLUDE_ASM("asm/nonmatchings/system/graphic", setCameraPerspectiveValues);

void setCameraPerspectiveValues(Camera* camera, f32 fov, f32 aspect, f32 near, f32 far) {

    camera->fov = fov;
    camera->aspect = aspect;
    camera->near = near;
    camera->far = far;

}

//INCLUDE_ASM("asm/nonmatchings/system/graphic", setCameraLookAt);

void setCameraLookAt(Camera* camera, f32 xEye, f32 yEye, f32 zEye, f32 atX, f32 atY, f32 atZ, f32 upX, f32 upY, f32 upZ) {

    camera->eye.x = xEye;
    camera->eye.y = yEye;
    camera->eye.z = zEye;

    camera->at.x = atX;
    camera->at.y = atY;
    camera->at.z = atZ;
    
    camera->up.x = upX;
    camera->up.y = upY;
    camera->up.z = upZ;

}

//INCLUDE_ASM("asm/nonmatchings/system/graphic", func_80028E14);

// unused
inline Gfx *func_80028E14(Gfx* dl, u8 a, u8 r, u8 g, u8 b) {

    gSPLightColor(dl++, LIGHT_2, (r << 0x18) + (g << 0x10) + (b << 8));

    return dl++;

}

//INCLUDE_ASM("asm/nonmatchings/system/graphic", func_80028E60);

// unused
inline Gfx *func_80028E60(Gfx* dl, u8 a, u8 r, u8 g, u8 b) {

    gSPLightColor(dl++, LIGHT_1, (r << 0x18) + (g << 0x10) + (b << 8));

    return dl++;

}

//INCLUDE_ASM("asm/nonmatchings/system/graphic", func_80028EA8);

// unused
void func_80028EA8(UnknownGraphicsStruct* arg0, u8 arg1, u8 arg2, u8 arg3) {
    arg0->unk_10 = arg1;
    arg0->unk_11 = arg2;
    arg0->unk_12 = arg3;
}

//INCLUDE_ASM("asm/nonmatchings/system/graphic", setInitialWorldRotationAngles);

void setInitialWorldRotationAngles(f32 x, f32 y, f32 z) {
    
    currentWorldRotationAngles.x = x;
    currentWorldRotationAngles.y = y;

    previousWorldRotationAngles.x = x;
    previousWorldRotationAngles.y = y;

    currentWorldRotationAngles.z = z;
    previousWorldRotationAngles.z = z;

}

//INCLUDE_ASM("asm/nonmatchings/system/graphic", func_80028EF8);

// mapContext.c --> increments/decrements 6 by 1
void func_80028EF8(f32 x, f32 y, f32 z) {
    currentWorldRotationAngles.x += x;
    currentWorldRotationAngles.y += y;
    currentWorldRotationAngles.z += z;
}

//INCLUDE_ASM("asm/nonmatchings/system/graphic", nuGfxInit);

void nuGfxInit(void) {

    Gfx	gfxList[0x100];
    Gfx* gfxList_ptr;
    
    nuGfxThreadStart();
    nuGfxSetCfb(FrameBuf, 3);

    nuGfxSetZBuffer((u16*) NU_GFX_ZBUFFER_ADDR);
    
    nuGfxSwapCfbFuncSet(nuGfxSwapCfb);
    
    // macro isn't working yet
    // nuGfxSetUcode(nugfx_ucode);
    nuGfxUcode = &nugfx_ucode;

    nuGfxTaskMgrInit();
    
    gfxList_ptr = gfxList;
    
    gSPDisplayList(gfxList_ptr++, OS_K0_TO_PHYSICAL(rdpstateinit_dl));
    gDPFullSync(gfxList_ptr++);
    gSPEndDisplayList(gfxList_ptr++);
    
    nuGfxTaskStart(gfxList, (s32)(gfxList_ptr - gfxList) * sizeof(Gfx), NU_GFX_UCODE_F3DEX, NU_SC_NOSWAPBUFFER);

    nuGfxTaskAllEndWait();
    
}
