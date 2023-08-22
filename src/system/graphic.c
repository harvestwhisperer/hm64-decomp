#include "common.h"
#include "graphic.h"
#include "sprite.h"

extern Camera gCamera;

extern NUUcode	nugfx_ucode[];
extern u16*	FrameBuf[3];

extern Gfx setup_rdpstate[];
extern Gfx setup_rspstate[];
extern Gfx rdpstateinit_dl[];
extern Gfx gfxList[0x1F];
extern Gfx D_80112A60[3];
extern Gfx D_80205000[0x20];

// data
/*
Gfx D_80112A60[] = { 
  // gsSPViewport(&viewport),
  // gsDPPipeSync(),
  // gsSPEndDisplayList(),
}
*/
  
extern Gfx D_801836A0[2][0x280];

extern f32 D_8011EC90[8];

extern volatile u8 gfxTaskNo;
extern volatile u32 gDisplayContext;

extern LookAt D_80126540;
//D_80112900
extern Vp viewport;

// should be Mtx?
extern f32 D_80237460[0x2D][4];

// doesn't seem right; need to fix this struct and base address
extern UnknownGraphicsStruct1 D_802373B4[2];

Gfx* clearFramebuffer(Gfx* dl);                  
Gfx* initRcp(Gfx*);                               
u32 startGfxTask(void);
                                                      
// assert strings                                                      
extern const char *D_8011EC60;
extern const char *D_8011EC64;

extern f64 D_8011EC78;
extern f64 D_8011EC80;
extern Vec3f D_8013D5D8;
extern Vec3f D_8017044C;
extern f32 D_80170450;
extern f32 D_80170454;

// forward declarations
void func_80026F30(Bitmap* arg0, u16* arg1);
Gfx *func_80028A64(Gfx*, Camera*, WorldGraphics*);
Gfx* func_800293C0(Gfx*, Mtx*);    
u8 func_80026BE0();      
void func_80028EB8(f32, f32, f32);                      
void setCameraLookAt(Camera*, f32, f32, f32, f32, f32, f32, f32, f32, f32); 
void setCameraOrthographicValues(Camera*, f32, f32, f32, f32, f32, f32); 
void setCameraPerspectiveValues(Camera*, f32, f32, f32, f32);       


//INCLUDE_ASM(const s32, "system/graphic", graphicsInit);

void graphicsInit(void) {

    u8 i;

    nuGfxInit();

    gCamera.perspectiveMode = FALSE;

    setCameraOrthographicValues(&gCamera, -160.0f, 160.0f, -120.0f, 120.0f, 0.0f, 900.0f);
    setCameraPerspectiveValues(&gCamera, 33.0f, 1.3333334f, 10.0f, 1000.0f);
    setCameraLookAt(&gCamera, 0.0f, 0.0f, 400.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.1f, 0.0f);

    for (i = 0; i < 2; i++) {
        D_802373B4[i].translationCoords.x = 0.0f;
        D_802373B4[i].translationCoords.y = 0.0f;
        D_802373B4[i].translationCoords.z = 0.0f;
        D_802373B4[i].scaleCoords.x = 1.0f;
        D_802373B4[i].scaleCoords.y = 1.0f;
        D_802373B4[i].scaleCoords.z = 1.0f;
        D_802373B4[i].rotationCoords.x = 0.0f;
        D_802373B4[i].rotationCoords.y = 0.0f;
        D_802373B4[i].rotationCoords.z = 0.0f;
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
    gDisplayContext ^= 1;
}

//INCLUDE_ASM(const s32, "system/graphic", startGfxTask);

u32 startGfxTask(void) {

    Gfx *dl;
 
    dl = initRcp(&gfxList[gDisplayContext*32]);
    dl = clearFramebuffer(dl);

    gDPFullSync(dl++);
    gSPEndDisplayList(dl++);

    if ((dl - &gfxList[gDisplayContext*32]) > GFX_GLIST_LEN) {
        __assert(&D_8011EC60, &D_8011EC64, 288);
    }
    
    nuGfxTaskStart(&gfxList[gDisplayContext*32], (s32)(dl - &gfxList[gDisplayContext*32]) << 3, NU_GFX_UCODE_F3DEX2, NU_SC_NOSWAPBUFFER);
    
    gfxTaskNo += 1;
    
    return gfxTaskNo;
}

//INCLUDE_ASM(const s32, "system/graphic", func_80026BE0);

u8 func_80026BE0(void) {

    Gfx *dl;

    dl = &D_80205000[gDisplayContext*32];
    
    gSPDisplayList(dl++, OS_K0_TO_PHYSICAL(&D_80112A60));
    gDPFullSync(dl++);
    gSPEndDisplayList(dl++);

    if (32 <= (dl - &D_80205000[gDisplayContext*32])) {
        __assert(&D_8011EC60, &D_8011EC64, 319);
    }

    nuGfxTaskStart(&D_80205000[gDisplayContext*32], (s32)(dl - &D_80205000[gDisplayContext*32]) << 3, NU_GFX_UCODE_F3DEX2, 1);
    
    gfxTaskNo += 1;
    return gfxTaskNo;
}

//INCLUDE_ASM(const s32, "system/graphic", func_80026CEC);

u8 func_80026CEC(s32 arg0, s32 arg1) {

    Gfx *dl;

    dl = &D_801836A0[gDisplayContext*2][0];
    
    gSPDisplayList(dl++, OS_K0_TO_PHYSICAL(&D_80112A60));
    
    dl = func_80028A64(dl++, &gCamera, &D_80237460[gDisplayContext*0x2D][0]);
    dl = func_800293C0(dl++, &D_80237460[gDisplayContext*0x2D][0]);
    
    gDPFullSync(dl++);
    gSPEndDisplayList(dl++);
    
    if (1279 < (dl - &D_801836A0[gDisplayContext*2][0])) {
        __assert(&D_8011EC60, &D_8011EC64, 358);
    }

    nuGfxTaskStart(&D_801836A0[gDisplayContext*2], (s32)(dl - &D_801836A0[gDisplayContext*2][0]) << 3, NU_GFX_UCODE_F3DEX2, 0);
    
    gfxTaskNo += 1;
    return gfxTaskNo;
}

//INCLUDE_ASM(const s32, "system/graphic", func_80026E78);

void func_80026E78(Bitmap *sprite, u16 *arg1, u16 *arg2) {
    
    u16 temp1;
    u16 temp2;
    int temp3;
    u32 padding[10];

    func_80026F30(sprite, arg2);
    
    sprite->timg = arg1 + 4;
    
    temp1 = arg1[2];
    temp3 = temp1;
    sprite->width = (u8) temp1 << 8;
    
    temp3 >>= 8;
    temp1 = temp3 | sprite->width;
    sprite->width = temp1;
    
    temp1 = arg1[3];
    sprite->height = (u8) temp1 << 8;
    temp2 = (temp1 >> 8) | sprite->height;
    sprite->height = temp2;
    
    switch (*(arg1 + 1) >> 4 & 0xF) {
        case 0:
          sprite->fmt = 2;
          sprite->flag = 1;
          return;
        
        case 1:
          sprite->fmt = 2;
          sprite->flag = 0; 
    }
}

//INCLUDE_ASM(const s32, "system/graphic", func_80026F30);

void func_80026F30(Bitmap* sprite, u16* arg1) {

    u32 padding[5];
    
    sprite->pal = arg1 + 2;

    switch ((*(arg1 + 1) >> 4) & 0xF) {                           
        case 0:
            sprite->fmt = 2;
            sprite->flag = 1;
            return;
        case 1:
            sprite->fmt = 2;
            sprite->flag = 0;
            return;
        }
}

INCLUDE_ASM(const s32, "system/graphic", func_80026F88);

INCLUDE_ASM(const s32, "system/graphic", func_800276AC);

INCLUDE_RODATA(const s32, "system/graphic", D_8011EC40);

INCLUDE_RODATA(const s32, "system/graphic", D_8011EC60);

//static const char *D_8011EC60 = "EX";

INCLUDE_RODATA(const s32, "system/graphic", D_8011EC64);

//static const char *D_8011EC64 = "s:/system/graphic.c";

// matches but include_rodata macro creates a problem for the assembler

INCLUDE_ASM(const s32, "system/graphic", func_800278F0);

// f32 func_800278F0(f32 angle) {
//     return sinf(angle * D_8011EC78);
// }

// matches but include_rodata macro creates a problem for the assembler

INCLUDE_ASM(const s32, "system/graphic", func_80027920);

// f32 func_80027920(f32 angle) {
//     return cosf(angle * D_8011EC80);
// }

INCLUDE_ASM(const s32, "system/graphic", func_80027950);

INCLUDE_ASM(const s32, "system/graphic", func_80027B74);

INCLUDE_ASM(const s32, "system/graphic", func_80027BFC);

INCLUDE_ASM(const s32, "system/graphic", func_80027DC0);

INCLUDE_ASM(const s32, "system/graphic", func_80027E10);

INCLUDE_ASM(const s32, "system/graphic", func_800284E8);

//INCLUDE_RODATA(const s32, "system/graphic", D_8011EC90);

static const f32 D_8011EC90[8] = { 0.0f, 315.0f, 270.0f, 225.0f, 180.0f, 135.0f, 90.0f, 45.0f };

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

        //temp_f26 = (f32) ((f64) sp70 * 0.017453292519943295);
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

void *func_80028888(u16 arg0, u32 *arg1) {
  void *res = arg1;
  return res + *(arg1 + arg0);
}

//INCLUDE_ASM(const s32, "system/graphic", func_800288A0);

void *func_800288A0(u16 arg0, u32 *arg1) {
  void *res = arg1;
  return res + *(arg1 + arg0);
}

INCLUDE_ASM(const s32, "system/graphic", func_800288B8);

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

INCLUDE_ASM(const s32, "system/graphic", func_80028A64);

//INCLUDE_ASM(const s32, "system/graphic", func_80028C00);

Gfx* func_80028C00(Gfx* dl, Camera* camera) {
 
    u16 perspNorm;
    
    switch (camera->perspectiveMode) {
        case 0:
            guOrtho(&camera->matrix1, camera->l, camera->r, camera->t, camera->b, camera->n, camera->f, 1.0f);
            break;
        case 1:
            guPerspective(&camera->matrix1, &perspNorm, camera->fov, camera->aspect, camera->near, camera->far, 1.0f);
            gSPPerspNormalize(dl++, perspNorm);
            break;
        default:
            break;
    }

    guLookAt(&camera->matrix2, camera->eye.x, camera->eye.y, camera->eye.z, camera->at.x, camera->at.y, camera->at.z, camera->up.x, camera->up.y, camera->up.z);
    gSPLookAt(dl++, &D_80126540);

    gSPMatrix(dl++, &camera->matrix1, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPMatrix(dl++, &camera->matrix2, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);

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

void func_80028EA8(UnknownGraphicsStruct2* arg0, u8 arg1, u8 arg2, u8 arg3) {
    arg0->unk_10 = arg1;
    arg0->unk_11 = arg2;
    arg0->unk_12 = arg3;
}

//INCLUDE_ASM(const s32, "system/graphic", func_80028EB8);

void func_80028EB8(f32 x, f32 y, f32 z) {
    
    D_8017044C.x = x;
    D_8017044C.y = y;

    D_8013D5D8.x = x;
    D_8013D5D8.y = y;

    D_8017044C.z = z;
    D_8013D5D8.z = z;

}

//INCLUDE_ASM(const s32, "system/graphic", func_80028EF8);

void func_80028EF8(f32 x, f32 y, f32 z) {
    D_8017044C.x += x;
    D_8017044C.y += y;
    D_8017044C.z += z;
}

//INCLUDE_ASM(const s32, "system/graphic", nuGfxInit);

void nuGfxInit(void) {

    Gfx	 gfxList[0x100];
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
    
    nuGfxTaskStart(gfxList, (s32)(gfxList_ptr - gfxList) * sizeof (Gfx), NU_GFX_UCODE_F3DEX2, 0);

    nuGfxTaskAllEndWait();
    
}
