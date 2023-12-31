#include "common.h"

#include "system/graphic.h"

#include "system/sprite.h"
#include "system/worldGraphics.h"

#include "mainproc.h"

// forward declarations

void func_80026F30(Bitmap* arg0, u16* arg1);
Gfx *func_80028A64(Gfx*, Camera*, WorldMatrices*);
volatile u8 func_80026BE0();      
void func_80028EB8(f32, f32, f32);            

Gfx* clearFramebuffer(Gfx* dl);                  
Gfx* initRcp(Gfx*);                               
volatile u8 startGfxTask(void);

void setCameraLookAt(Camera*, f32, f32, f32, f32, f32, f32, f32, f32, f32); 
void setCameraOrthographicValues(Camera*, f32, f32, f32, f32, f32, f32); 
void setCameraPerspectiveValues(Camera*, f32, f32, f32, f32);    

// bss
Camera gCamera;

LookAt D_80126540;

Gfx initGfxList[2][0x20];
Gfx D_801836A0[2][0x500];
Gfx D_80205000[2][0x20];
// viewport
extern Gfx D_80112A60[3];

WorldMatrices worldMatrices[2];
                        
// data, possibly external
extern Gfx setup_rdpstate[];
extern Gfx setup_rspstate[];
extern Gfx rdpstateinit_dl[];

// data
extern NUUcode nugfx_ucode[];
// nugfxtaskinit data?
extern u16*	FrameBuf[3];

extern Vp viewport;

// rodata
extern f64 D_8011EC78;
extern f64 D_8011EC80;
extern f64 D_8011EC88;

// assert strings                                                      
extern const char D_8011EC60[];
extern const char D_8011EC64[];

// shared globals
// also used by mapContext.c, map.c, and worldGraphics.c
extern Vec3f previousWorldRotationAngles;
extern Vec3f currentWorldRotationAngles;
extern f32 D_80170450;
extern f32 D_80170454;
// nu idle statck
extern u64 *D_80126520;

extern f32 cosf(f32);
extern f32 sinf(f32);


//INCLUDE_RODATA(const s32, "system/graphic", D_8011EC40);

// unused 
static const u32 D_8011EC40[8] = { 0x10000000, 0x20000000, 0x00004000, 0x00000040, 0x00001188, 0x020712D0, 0x00000250, 0x021F12D0 };

INCLUDE_RODATA(const s32, "system/graphic", D_8011EC60);

//static const char D_8011EC60 = "EX";

INCLUDE_RODATA(const s32, "system/graphic", D_8011EC64);

//static const char *D_8011EC64 = "s:/system/graphic.c";


//INCLUDE_ASM(const s32, "system/graphic", graphicsInit);

void graphicsInit(void) {

    u8 i;

    nuGfxInit();
 
    gCamera.perspectiveMode = FALSE;

    setCameraOrthographicValues(&gCamera, -160.0f, 160.0f, -120.0f, 120.0f, 0.0f, 900.0f);
    setCameraPerspectiveValues(&gCamera, 33.0f, 1.3333334f, 10.0f, 1000.0f);
    setCameraLookAt(&gCamera, 0.0f, 0.0f, 400.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.1f, 0.0f);

    for (i = 0; i < 2; i++) {
        worldMatrices[i].translation.x = 0.0f;
        worldMatrices[i].translation.y = 0.0f;
        worldMatrices[i].translation.z = 0.0f;
        worldMatrices[i].scaling.x = 1.0f;
        worldMatrices[i].scaling.y = 1.0f;
        worldMatrices[i].scaling.z = 1.0f;
        worldMatrices[i].rotation.x = 0.0f;
        worldMatrices[i].rotation.y = 0.0f;
        worldMatrices[i].rotation.z = 0.0f;
    }

    func_80028EB8(45.0f, 315.0f, 0.0f);  
    
}

//INCLUDE_ASM(const s32, "system/graphic", drawFrame);

void drawFrame(void) {
    gfxTaskNo = 0;
    startGfxTask();
    // set world and view matrices
    func_80026CEC();
    // draw
    func_80026BE0();
    gDisplayContextIndex ^= 1;
}

//INCLUDE_ASM(const s32, "system/graphic", startGfxTask);

volatile u8 startGfxTask(void) {

    Gfx *dl;
 
    dl = initRcp(initGfxList[gDisplayContextIndex]);
    dl = clearFramebuffer(dl);

    gDPFullSync(dl++);
    gSPEndDisplayList(dl++);

    if (dl - initGfxList[gDisplayContextIndex] > GFX_GLIST_LEN) {
        __assert(&D_8011EC60, &D_8011EC64, 288);
    }
    
    nuGfxTaskStart(initGfxList[gDisplayContextIndex], (s32)(dl - initGfxList[gDisplayContextIndex]) * sizeof(Gfx), NU_GFX_UCODE_F3DEX2, NU_SC_NOSWAPBUFFER);
    
    gfxTaskNo += 1;
    
    return gfxTaskNo;
}

//INCLUDE_ASM(const s32, "system/graphic", func_80026BE0);

volatile u8 func_80026BE0(void) {

    Gfx *dl = D_80205000[gDisplayContextIndex];
    
    // set viewport
    gSPDisplayList(dl++, OS_K0_TO_PHYSICAL(&D_80112A60));
    gDPFullSync(dl++);
    gSPEndDisplayList(dl++);

    if (dl - D_80205000[gDisplayContextIndex] >= 32) {
        __assert(&D_8011EC60, &D_8011EC64, 319);
    }

    nuGfxTaskStart(D_80205000[gDisplayContextIndex], (s32)(dl - D_80205000[gDisplayContextIndex]) * sizeof(Gfx), NU_GFX_UCODE_F3DEX2, NU_SC_SWAPBUFFER);
    
    gfxTaskNo += 1;
    return gfxTaskNo;
}

//INCLUDE_ASM(const s32, "system/graphic", func_80026CEC);

volatile u8 func_80026CEC(s32 arg0, s32 arg1) {

    Gfx *dl = D_801836A0[gDisplayContextIndex];
    
    // set viewport
    gSPDisplayList(dl++, OS_K0_TO_PHYSICAL(&D_80112A60));
    
    dl = func_80028A64(dl++, &gCamera, &worldMatrices[gDisplayContextIndex]);
    dl = func_800293C0(dl++, &worldMatrices[gDisplayContextIndex]);
    
    gDPFullSync(dl++);
    gSPEndDisplayList(dl++);
    
    if (dl - D_801836A0[gDisplayContextIndex] >= 0x500) {
        __assert(&D_8011EC60, &D_8011EC64, 0x166);
    }

    nuGfxTaskStart(D_801836A0[gDisplayContextIndex], (s32)(dl - D_801836A0[gDisplayContextIndex]) * sizeof(Gfx), NU_GFX_UCODE_F3DEX2, NU_SC_NOSWAPBUFFER);
    
    gfxTaskNo += 1;
    return gfxTaskNo;
}

//INCLUDE_ASM(const s32, "system/graphic", setBitmapFormat);

// 16-bit endian swap
static inline u16 getWidth(u16* timg) {
    
    u16 temp1;
    
    u32 temp2;
    u32 temp3;

    // skip header
    temp2 = temp1 = timg[2];
    
    temp3 = (temp1 & 0xFF) << 8;
    temp2 >>= 8;
    
    temp1 = temp2 | temp3;

    return temp1;

    // return (temp2 | temp3) also matches

    // should just be
    /*
    u32 temp1;
    u32 temp2;
    
    temp1 = (*(timg+2) & 0xFF) << 8;
    
    temp2 = *(timg+2) >> 8;

    return (temp2 | temp1);
    */
    
}

// 16-bit endian swap
static inline u16 getHeight(u16* timg) {

    u16 temp1;
    
    u32 temp2;
    u32 temp3;

    // skip header + width
    temp2 = temp1 = timg[3];
    
    temp3 = (temp1 & 0xFF) << 8;
    temp2 >>= 8;
    
    temp1 = temp2 | temp3;

    return temp1;

    // return (temp2 | temp3) also matches
    
    // should just be
    /*
    u32 temp1;
    u32 temp2;
    
    temp1 = (*(timg+3) & 0xFF) << 8;
    
    temp2 = *(timg+3) >> 8;

    return (temp2 | temp1);
    */
    
}

void setBitmapFormat(Bitmap *sprite, u16 *timg, u16 *palette) {
    
    u32 padding[10];

    func_80026F30(sprite, palette);
    
    // skip header and size bytes
    sprite->timg = timg + 4;
    
    // bytes 4-8 = width and height (16 bit) (byte swapped)
    sprite->width = getWidth(timg);  
    sprite->height = getHeight(timg);
    
    // get pixel size from bit 5 in header (bit one when swapped)
    switch (*(timg + 1) >> 4 & 0xF) {
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

//INCLUDE_ASM(const s32, "system/graphic", func_80026F30);

void func_80026F30(Bitmap* sprite, u16* palette) {

    u32 padding[5];
    
    // skip header
    sprite->pal = palette + 2;

    // get pixel size from bit 5 in header (bit one when swapped)
    switch ((*(palette + 1) >> 4) & 0xF) {                           
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

//INCLUDE_ASM(const s32, "system/graphic", func_80026F88);

Gfx* func_80026F88(Gfx* dl, Bitmap* sprite, u32 offset, u16 height) {
    
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

// param1 = Vtx, 8021E6E0
// vtx adjustments/shading
INCLUDE_ASM(const s32, "system/graphic", func_800276AC);

//INCLUDE_ASM(const s32, "system/graphic", sinfRadians);

f32 sinfRadians(f32 angle) {
    return sinf(angle * DEGREES_TO_RADIANS_CONSTANT);
}

//INCLUDE_ASM(const s32, "system/graphic", cosfRadians);

f32 cosfRadians(f32 angle) {
    return cosf(angle * DEGREES_TO_RADIANS_CONSTANT);
}

//INCLUDE_ASM(const s32, "system/graphic", func_80027950);

// 3D rotation
void func_80027950(Vec3f arg0, Vec3f* arg1, Vec3f arg2) {
    
    f32 sinX;
    f32 cosX;
    f32 sinY;
    f32 cosY;
    f32 sinZ;
    f32 cosZ;

    f32 tempX;
    f32 tempY;
    f32 tempZ;
    
    f32 tempRadiansX = arg2.x * DEGREES_TO_RADIANS_CONSTANT;
    f32 tempRadiansY = arg2.y * DEGREES_TO_RADIANS_CONSTANT;
    f32 tempRadiansZ = arg2.z * DEGREES_TO_RADIANS_CONSTANT;
    
    sinX = sinf(tempRadiansX);
    cosX = cosf(tempRadiansX);
    sinY = sinf(tempRadiansY);
    cosY = cosf(tempRadiansY);
    sinZ = sinf(tempRadiansZ);
    cosZ = cosf(tempRadiansZ);
    
    arg1->x = arg0.x;
    arg1->y = arg0.y;
    arg1->z = arg0.z;
    
    tempX = arg1->x;
    tempY = arg1->y;
    tempZ = arg1->z;
    
    if (tempRadiansZ != 0.0f) {
        
        arg1->x = (-tempY * sinZ) + (tempX * cosZ);
        arg1->y = (tempY * sinZ) + (tempX * sinZ);
        arg1->z = tempZ;

    }
    
    tempX = arg1->x;
    tempY = arg1->y;
    tempZ = arg1->z;
    
    if (tempRadiansY != 0.0f) {
        
        arg1->x = (tempZ * sinY) + (tempX * cosY);
        arg1->y = tempY;
        arg1->z = (tempZ * cosY) - (tempX * sinY);
   
    }
    
    tempX = arg1->x;
    tempY = arg1->y;
    tempZ = arg1->z;
    
    if (tempRadiansX != 0.0f) {
    
        arg1->x = tempX;
        arg1->y = (-tempZ * sinX) + (tempY * cosX);
        arg1->z = (tempZ * cosX) + (tempY * sinX);
        
    }
    
}

//INCLUDE_ASM(const s32, "system/graphic", func_80027B74);

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

//INCLUDE_ASM(const s32, "system/graphic", func_80027BFC);

inline Coordinates* func_80027BFC(Coordinates* arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9) {
    
    Coordinates vec;
    f32 temp_f12;
    f32 temp_f14;
    f32 temp_f20;
    f32 temp_f20_2;
    f32 temp_f22;
    f32 temp_f22_2;
    f32 temp_f24;
    f32 temp_f24_2;
    f32 var_f0;
    
    temp_f14 = (arg2 * (arg6 - arg9)) + (arg5 * (arg9 - arg3));
    temp_f24 = temp_f14 + (arg8 * (arg3 - arg6));
    temp_f22_2 = temp_f24;
    
    temp_f22 = ((arg3 * (arg4 - arg7)) + (arg6 * (arg7 - arg1))) + (arg9 * (arg1 - arg4));
    temp_f20 = ((arg1 * (arg5 - arg8)) + (arg4 * (arg8 - arg2))) + (arg7 * (arg2 - arg5));
    temp_f12 = ((temp_f24 * temp_f24) + (temp_f22 * temp_f22)) + (temp_f20 * temp_f20);
    
    var_f0 = sqrtf(temp_f12);
    
    if (var_f0 == 0) {
        
        vec.x = 0.0f;
        vec.y = 0.0f;
        vec.z = 0.0f;
        vec.w = 0.0f;
        
    } else {
        
        temp_f22_2 /= var_f0;
        temp_f22 /= var_f0;
        temp_f20 /= var_f0;
        
        vec.x = temp_f22_2;
        vec.y = temp_f22;
        vec.z = temp_f20;
        vec.w = -(((temp_f22_2 * arg1) + (temp_f22 * arg2)) + (temp_f20 * arg3));
    }
    
    *arg0 = vec;
    return arg0;
}

//INCLUDE_ASM(const s32, "system/graphic", func_80027DC0);

f32 func_80027DC0(f32 arg0, f32 arg1, Coordinates vec) {

    f32 result;
    
    result = 0.0f;
    
    if (vec.y != 0.0f) {
        result = ( (-(vec.x*arg0) - (vec.z * arg1)) - vec.w) / vec.y;
    }
    

    return result;
}

#ifdef PERMUTER
u8 func_80027E10(Coordinates arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9) {

    u8 count = 0;

    func_80027BFC(&arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);

    if (arg0.x >= 0.0f) {
        count = 1;
    }

    func_80027BFC(&arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);

    if (arg0.x >= 0.0f) {
        count++;
    }

    func_80027BFC(&arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);

    if (arg0.x >= 0.0f) {
        count++;
    }

    return count / 3;
    
}
#else
INCLUDE_ASM(const s32, "system/graphic", func_80027E10);
#endif

#ifdef PERMUTER
f32 func_800284E8(f32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6) {
    return (arg3 * arg0) + (arg4 * arg1) + (arg5 * arg2) + arg6;
}
#else
INCLUDE_ASM(const s32, "system/graphic", func_800284E8);
#endif

//INCLUDE_RODATA(const s32, "system/graphic", D_8011EC90);

static const f32 D_8011EC90[8] = { 0.0f, 315.0f, 270.0f, 225.0f, 180.0f, 135.0f, 90.0f, 45.0f };

//INCLUDE_ASM(const s32, "system/graphic", func_80028520);

Vec3f* func_80028520(Vec3f *arg0, f32 arg1, u8 arg2, f32 arg3) {

    Angles angles;
    f32 *ptr;

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
    
    angles = *(Angles*)D_8011EC90;
    ptr = (f32*)&angles;
    
    if (arg2 != 0xFF) {

        vec1.x = 0.0f;
        vec1.y = arg3;
        vec1.z = arg1;

        vec3.x = 0.0f;
        vec3.y = ptr[arg2];
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
        vec2.y = arg3;
        vec2.z = 0.0f;
    }

    *arg0 = vec2;
    
    return arg0;
    
}

//INCLUDE_ASM(const s32, "system/graphic", func_80028820);

f32 func_80028820(u8 arg0) {
    
    Angles angles;
    f32 *ptr;
    
    angles = *(Angles*)D_8011EC90;
    ptr = (f32*)&angles;

    return ptr[arg0];
}
 
//INCLUDE_ASM(const s32, "system/graphic", func_80028888);

// get ptr to ci texture from index
// used by map
u8* func_80028888(u16 spriteIndex, u32* textureIndex) {
    return (u8*)textureIndex + textureIndex[spriteIndex];
}

// alternate
/*
void *func_80028888(u16 arg0, u32 *arg1) {
  void *res = arg1;
  return res + *(arg1 + arg0);
}
*/

//INCLUDE_ASM(const s32, "system/graphic", func_800288A0);

// get ptr to palette
// used by map
// should return u16*?
u8 *func_800288A0(u16 index, u32 *paletteIndex) {
  return (u8*)paletteIndex + paletteIndex[index];
}

//INCLUDE_ASM(const s32, "system/graphic", func_800288B8);

// returns palette pointer from sprite-to-palette mapping table
u8* func_800288B8(u16 spriteIndex, u32* paletteIndex, u8* spriteToPaletteIndex) {
    
    u8* arr = spriteToPaletteIndex + 4;
    u16 i = arr[spriteIndex];
    
    return (u8*)paletteIndex + paletteIndex[i]; 

}

// alternate
/*
u8* func_800288B8(u16 arg0, u32 *arg1, u8 *arg2) {
    return (u8*)arg1 + *(arg1 + *(arg0 + arg2 + 4));
}
*/

//INCLUDE_ASM(const s32, "system/graphic", initRcp);

Gfx* initRcp(Gfx* dl) {
    
    gSPSegment(dl++, 0, 0x0);
    gSPDisplayList(dl++, OS_K0_TO_PHYSICAL(setup_rspstate));
    gSPDisplayList(dl++, OS_K0_TO_PHYSICAL(setup_rdpstate));

    return dl++;
}

//INCLUDE_ASM(const s32, "system/graphic", clearFramebuffer);

Gfx* clearFramebuffer(Gfx* dl) {

    gDPSetDepthImage(dl++, OS_K0_TO_PHYSICAL(nuGfxZBuffer));
    gDPPipeSync(dl++);
    gDPSetCycleType(dl++, G_CYC_FILL);
    gDPSetColorImage(dl++, G_IM_FMT_RGBA, G_IM_SIZ_16b,SCREEN_WD, OS_K0_TO_PHYSICAL(nuGfxZBuffer));
    gDPSetFillColor(dl++,(GPACK_ZDZ(G_MAXFBZ,0) << 16 | GPACK_ZDZ(G_MAXFBZ,0)));
    gDPFillRectangle(dl++, 0, 0, SCREEN_WD-1, SCREEN_HT-1);
    gDPPipeSync(dl++);
    
    gDPSetColorImage(dl++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD, osVirtualToPhysical(nuGfxCfb_ptr));   
    gDPSetFillColor(dl++, (GPACK_RGBA5551(0, 0, 0, 1) << 16 | GPACK_RGBA5551(0, 0, 0, 1)));
    gDPFillRectangle(dl++, 0, 0, SCREEN_WD-1, SCREEN_HT-1);
    gDPPipeSync(dl++);
    
    return dl++;
}

//INCLUDE_ASM(const s32, "system/graphic", func_80028A64);

Gfx* func_80028A64(Gfx* dl, Camera* camera, WorldMatrices* matrices) {

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
    gSPLookAt(dl++, &D_80126520);

    gSPMatrix(dl++, &matrices->projection, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPMatrix(dl++, &matrices->viewing, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);

    return dl++;
}

//INCLUDE_ASM(const s32, "system/graphic", func_80028C00);

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
    gSPLookAt(dl++, &D_80126540);

    gSPMatrix(dl++, &camera->projection, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPMatrix(dl++, &camera->viewing, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);

    return dl++;

}

//INCLUDE_ASM(const s32, "system/graphic", setCameraOrthographicValues);

void setCameraOrthographicValues(Camera* camera, f32 l, f32 r, f32 t, f32 b, f32 n, f32 f) {
    camera->l = l;
    camera->r = r;
    camera->t = t;
    camera->b = b;
    camera->n = n;
    camera->f = f;
}

//INCLUDE_ASM(const s32, "system/graphic", setCameraPerspectiveValues);

void setCameraPerspectiveValues(Camera* camera, f32 fov, f32 aspect, f32 near, f32 far) {
    camera->fov = fov;
    camera->aspect = aspect;
    camera->near = near;
    camera->far = far;
}

//INCLUDE_ASM(const s32, "system/graphic", setCameraLookAt);

void setCameraLookAt (Camera* camera, f32 xEye, f32 yEye, f32 zEye, f32 atX, f32 atY, f32 atZ, f32 upX, f32 upY, f32 upZ) {
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

//INCLUDE_ASM(const s32, "system/graphic", func_80028E14);

inline Gfx *func_80028E14(Gfx* dl, u8 a, u8 r, u8 g, u8 b) {

    gSPLightColor(dl++, LIGHT_2, (r << 0x18) + (g << 0x10) + (b << 8));

    return dl++;
}

//INCLUDE_ASM(const s32, "system/graphic", func_80028E60);

inline Gfx *func_80028E60(Gfx* dl, u8 a, u8 r, u8 g, u8 b) {

    gSPLightColor(dl++, LIGHT_1, (r << 0x18) + (g << 0x10) + (b << 8));

    return dl++;
}

//INCLUDE_ASM(const s32, "system/graphic", func_80028EA8);

void func_80028EA8(UnknownGraphicsStruct* arg0, u8 arg1, u8 arg2, u8 arg3) {
    arg0->unk_10 = arg1;
    arg0->unk_11 = arg2;
    arg0->unk_12 = arg3;
}

//INCLUDE_ASM(const s32, "system/graphic", func_80028EB8);

void func_80028EB8(f32 x, f32 y, f32 z) {
    
    currentWorldRotationAngles.x = x;
    currentWorldRotationAngles.y = y;

    previousWorldRotationAngles.x = x;
    previousWorldRotationAngles.y = y;

    currentWorldRotationAngles.z = z;
    previousWorldRotationAngles.z = z;

}

//INCLUDE_ASM(const s32, "system/graphic", func_80028EF8);

void func_80028EF8(f32 x, f32 y, f32 z) {
    currentWorldRotationAngles.x += x;
    currentWorldRotationAngles.y += y;
    currentWorldRotationAngles.z += z;
}

//INCLUDE_ASM(const s32, "system/graphic", nuGfxInit);

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
    
    nuGfxTaskStart(gfxList, (s32)(gfxList_ptr - gfxList) * sizeof(Gfx), NU_GFX_UCODE_F3DEX2, NU_SC_NOSWAPBUFFER);

    nuGfxTaskAllEndWait();
    
}
