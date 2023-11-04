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

// sets viewport at beginning of display lists
extern Gfx D_80112A60[3];
/*
Gfx D_80112A60[] = { 
  // gsSPViewport(&viewport),
  // gsDPPipeSync(),
  // gsSPEndDisplayList(),
}
*/

// rodata
extern f64 D_8011EC78;
extern f64 D_8011EC80;

// assert strings                                                      
extern const char D_8011EC60[];
extern const char D_8011EC64[];

// shared globals
// also used by mapContext.c, map.c, and worldGraphics.c
extern Vec3f previousWorldRotationAngles;
extern Vec3f currentWorldRotationAngles;
extern f32 D_80170450;
extern f32 D_80170454;
// sprite vertices
extern Vtx D_8021E6E0[2][0x80][4];
// nu idle statck
extern u64 *D_80126520;

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

void setBitmapFormat(Bitmap *sprite, u16 *timg, u16 *palette) {
    
    u16 temp1;
    u16 temp2;
    int temp3;
    u32 padding[10];

    func_80026F30(sprite, palette);
    
    // skip header and size
    sprite->timg = timg + 4;
    
    // bytes 4-8 = width and height (16 bit)
    temp1 = *(timg+2);
    temp3 = temp1;
    sprite->width = (u8)temp1 << 8;
    
    temp3 >>= 8;
    temp1 = temp3 | sprite->width;
    sprite->width = temp1;  
     
    temp1 = *(timg+3);
    sprite->height = (u8)temp1 << 8;
    temp2 = (temp1 >> 8) | sprite->height;
    sprite->height = temp2;
    
    // get pixel size from bit
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

    // probably a stack struct
    u32 padding[5];
    
    // skip header
    sprite->pal = palette + 2;

    switch ((*(palette + 1) >> 4) & 0xF) {                           
        case 0:
            sprite->fmt = G_IM_FMT_CI;
            // SDK docs say to use this pixel size only with G_IM_FMT_I
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

// param1 = Mtx*
// param1: 8021E6E0 + sizeof(Mtx) * offset = + 0x40 * n
INCLUDE_ASM(const s32, "system/graphic", func_800276AC);

// unused
//static const u32 D_8011EC40[8] = { 0x10000000, 0x20000000, 0x00004000, 0x00000040, 0x00001188, 0x020712D0, 0x00000250, 0x021F12D0 };

INCLUDE_RODATA(const s32, "system/graphic", D_8011EC40);

INCLUDE_RODATA(const s32, "system/graphic", D_8011EC60);

//static const char D_8011EC60 = "EX";

INCLUDE_RODATA(const s32, "system/graphic", D_8011EC64);

//static const char *D_8011EC64 = "s:/system/graphic.c";

// matches but include_rodata macro creates a problem for the assembler

INCLUDE_ASM(const s32, "system/graphic", sinfRadians);

/*
f32 sinfRadians(f32 angle) {
    return sinf(angle * D_8011EC78);
}
*/

// matches but include_rodata macro creates a problem for the assembler

INCLUDE_ASM(const s32, "system/graphic", cosfRadians);

/*
f32 cosfRadians(f32 angle) {
    return cosf(angle * D_8011EC80);
}
*/

#ifdef PERMUTER
void func_80027950(f32 arg0, f32 arg1, f32 arg2, Vec3f* arg3, f32 arg4, f32 arg5, f32 arg6) {

    f32 temp1;
    f32 temp2;
    f32 temp3;
    f32 temp4;
    f32 temp5;
    f32 temp6;

    f32 temp7;
    f32 temp8;
    f32 temp9;

    f32 tempRadiansX = arg4*D_8011EC88;
    f32 tempRadiansY = arg5*D_8011EC88;
    f32 tempRadiansZ = arg6*D_8011EC88;

    temp1 = sinf(tempRadiansX);
    temp2 = cosf(tempRadiansX);
    
    temp3 = sinf(tempRadiansY);
    temp4 = cosf(tempRadiansY);

    temp5 = sinf(tempRadiansZ);
    temp6 = cosf(tempRadiansZ);
    
    arg3->x = arg0;
    arg3->y = arg1;
    arg3->z = arg2;

    if (tempRadiansZ != 0.0f) {
        arg3->y = (arg3->y*temp5) + (arg3->x * temp5);
        arg3->x = (-arg3->y * temp5) + (arg3->x * temp6);
    }

    if (tempRadiansY != 0.0f) {
        arg3->x = (arg3->z * temp3) + (arg3->x * temp4);
        arg3->z = (arg3->z * temp4) + (arg3->x * temp3);
    }

    if (tempRadiansX != 0.0f) {
        arg3->z = (arg3->z * temp2) + (arg3->y * temp1);
        arg3->y = (-arg3->z * temp1) + (arg3->y * temp2);
    }
    
}
#else
INCLUDE_ASM(const s32, "system/graphic", func_80027950);
#endif

INCLUDE_ASM(const s32, "system/graphic", func_80027B74);

INCLUDE_ASM(const s32, "system/graphic", func_80027BFC);

INCLUDE_ASM(const s32, "system/graphic", func_80027DC0);

INCLUDE_ASM(const s32, "system/graphic", func_80027E10);

#ifdef PERMUTER
f32 func_800284E8(f32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6) {
    return (arg3 * arg0) + (arg4 * arg1) + (arg5 * arg2) + arg6;
}
#else
INCLUDE_ASM(const s32, "system/graphic", func_800284E8);
#endif

//INCLUDE_RODATA(const s32, "system/graphic", D_8011EC90);

static const f32 D_8011EC90[8] = { 0.0f, 315.0f, 270.0f, 225.0f, 180.0f, 135.0f, 90.0f, 45.0f };

// radians conversion
//static const double D_8011ECB0 = 0.017453292519943295;

#ifdef PERMUTER
Vec3f* func_80028520(Vec3f* arg0, u8 arg1, f32 arg2, f32 arg3) {

    Angles angles;
    f32 *ptr;
    Vec3f vec1;
    Vec3f vec2;
    Vec3f vec3;
    Vec3f vec4;
    Vec3f vec5;
    f32 x;
    f32 y;
    f32 z;
    f32 temp1;
    f32 temp2;
    f32 temp3;
    f32 temp4;
    f32 temp5;
    f32 temp6;
    f32 temp7;
    f32 temp8;

    angles = D_8011EC90;
    ptr = (f32*)&angles;

    if (arg1 != 0xFF) {
        
        vec1.x = 0.0f;
        vec1.y = arg1;
        vec1.z = arg3;

        vec2.x = 0.0f;
        vec2.y = 0.0f;
        vec2.z = ptr[arg1];

        vec3 = vec1;
        vec4 = vec2;

        // radians conversion
        // temp_f26 = (f32) ((f64) sp70 * 0.017453292519943295);
        x = vec3.x * D_8011ECB0;
        y = vec3.y * D_8011ECB0;
        z = vec3.z * D_8011ECB0;

        temp1 = sinf(x);
        temp2 = cosf(x);

        temp3 = sinf(y);
        temp4 = cosf(y);

        temp5 = sinf(z);
        temp6 = cosf(z);

        vec5 = vec3;
        
        if (z != 0.0f) {
            vec5.z = vec3.z;
            vec5.y = (vec3.y * temp5) + (vec3.x * temp5);
            vec5.x = (-vec3.y * temp5) + (vec3.x * temp6);
        }
        if (y != 0.0f) {
            temp7 = (vec5.z * temp1) - (vec5.x * temp3);
            vec5.x = (vec5.z * temp3) + (vec5.x * temp4);
            vec5.z = temp7;
        }
        if (z != 0.0f) {
            temp8 = (-vec5.z * temp1) + (vec5.y * temp2);
            vec5.z = (vec5.z * temp2) + (vec5.y * temp1);
            vec5.y = temp8;
        }
    } else {
        vec5.x = 0.0f;
        vec5.y = arg2;
        vec5.z = 0.0f;
    }

    *arg0 = vec5;
    
    return arg0;
}
#else
INCLUDE_ASM(const s32, "system/graphic", func_80028520);
#endif

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
void *func_80028888(u16 arg0, u32 *arg1) {
  void *res = arg1;
  return res + *(arg1 + arg0);
}

//INCLUDE_ASM(const s32, "system/graphic", func_800288A0);

void *func_800288A0(u16 arg0, u32 *arg1) {
  void *res = arg1;
  return res + *(arg1 + arg0);
}

//INCLUDE_ASM(const s32, "system/graphic", func_800288B8);

u8* func_800288B8(u16 arg0, u32 *arg1, u8 *arg2) {
    return (u8*)arg1 + *(arg1 + *(arg0 + arg2 + 4));
}

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

Gfx *func_80028E14(Gfx* dl, u8 a, u8 r, u8 g, u8 b) {

    gSPLightColor(dl++, LIGHT_2, (r << 0x18) + (g << 0x10) + (b << 8));

    return dl++;
}

//INCLUDE_ASM(const s32, "system/graphic", func_80028E60);

Gfx *func_80028E60(Gfx* dl, u8 a, u8 r, u8 g, u8 b) {

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
