#include "common.h"

#include "system/worldGraphics.h"

#include "system/graphic.h"
#include "system/map.h"

// bss
extern u16 spriteCounter;

// unused
extern u8 D_80189135[9];

extern WorldGraphics D_8018A850[0x150];


//INCLUDE_ASM(const s32, "system/worldGraphics", initializeWorldGraphics);

void initializeWorldGraphics(void) {

    u16 i;
     
    D_80189135[0] = 0xFF;
    D_80189135[1] = 0xFF;
    D_80189135[2] = 0xFF;
    D_80189135[3] = 0xFF;
    D_80189135[4] = 0xFF;
    D_80189135[5] = 0xFF;
    D_80189135[6] = 0xFF;
    D_80189135[7] = 0xFF;
    D_80189135[8] = 0xFF;
 
    for (i = 0; i < 0x150; i++) {
        D_8018A850[i].flags = 0;
        D_8018A850[i].positions.x = 0;
        D_8018A850[i].positions.y = 0;
        D_8018A850[i].positions.z = 0;
        D_8018A850[i].scaling.x = 0;
        D_8018A850[i].scaling.y = 0;
        D_8018A850[i].scaling.z = 0;
        D_8018A850[i].rotation.x = 0;
        D_8018A850[i].rotation.y = 0;
        D_8018A850[i].rotation.z = 0;
    }
    
    resetSpriteCounter();
    
}

//INCLUDE_ASM(const s32, "system/worldGraphics", resetSpriteCounter);

void resetSpriteCounter(void) {
    spriteCounter = 0;
}

void func_80029294(void) {}

//INCLUDE_ASM(const s32, "system/worldGraphics", func_8002929C);

u16 func_8002929C(Gfx *dl, u16 flags) {

    u16 index = spriteCounter;

    D_8018A850[index].dl = dl;
    D_8018A850[index].flags = flags | 1;
    
    spriteCounter++;
    
    return index;
}

//INCLUDE_ASM(const s32, "system/worldGraphics", func_800292EC);

bool func_800292EC(u16 index, f32 x, f32 y, f32 z) {
    D_8018A850[index].positions.x = x;
    D_8018A850[index].positions.y = y;
    D_8018A850[index].positions.z = z;
    return 1;
}

//INCLUDE_ASM(const s32, "system/worldGraphics", func_80029330);

bool func_80029330(u16 index, f32 arg1, f32 arg2, f32 arg3) {
    D_8018A850[index].scaling.x = arg1;
    D_8018A850[index].scaling.y = arg2;
    D_8018A850[index].scaling.z = arg3;
    return 1;
}

//INCLUDE_ASM(const s32, "system/worldGraphics", func_80029374);

bool func_80029374(u16 index, f32 arg1, f32 arg2, f32 arg3) {
    D_8018A850[index].rotation.x = arg1;
    D_8018A850[index].rotation.y = arg2;
    D_8018A850[index].rotation.z = arg3;
    return 1;
}

void func_800293B8(void) {}

//INCLUDE_ASM(const s32, "system/worldGraphics", func_800293C0);

Gfx* func_800293C0(Gfx* dl, WorldMatrices* arg1) {

    u16 i;

    Vec3f vec;

    f32 sinXCurrent;
    f32 sinYCurrent;
    f32 sinXPrevious;
    f32 sinYPrevious;
    f32 cosXCurrent;
    f32 cosYCurrent;
    f32 cosXPrevious;
    f32 cosYPrevious;
    
    f32 x, y, z;
    f32 rotatedX;
    f32 rotatedY;
    f32 tempZ;
    
    // FIXME: shouldn't be necessary
    u32 padding[11];

    gDPPipeSync(dl++);
    gDPSetCycleType(dl++, G_CYC_1CYCLE);
    gDPSetTextureLUT(dl++, G_TT_RGBA16);
    gDPSetAlphaCompare(dl++, G_AC_NONE); 
    gDPSetDepthSource(dl++, G_ZS_PIXEL);

    // not sure what the 0xC0F9FA is
    gSPClearGeometryMode(dl++, G_ZBUFFER | G_SHADE | G_CULL_FRONT | G_CULL_BACK | G_CULL_BOTH | G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR | G_LOD | G_SHADING_SMOOTH | G_CLIPPING | 0xC0F9FA);
    gSPSetGeometryMode(dl++, G_ZBUFFER | G_SHADE | G_CULL_FRONT | G_SHADING_SMOOTH);
    gSPClipRatio(dl++, FRUSTRATIO_6);
    gSPTexture(dl++, 0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON);

    guTranslate(&arg1->translationM, arg1->translation.x, arg1->translation.y, arg1->translation.z);
    guScale(&arg1->scale, arg1->scaling.x, arg1->scaling.y, arg1->scaling.z);

    guRotateRPY(&arg1->rotationX, arg1->rotation.x, 0.0f, 0.0f);
    guRotateRPY(&arg1->rotationY, 0.0f, arg1->rotation.y, 0.0f);
    guRotateRPY(&arg1->rotationZ, 0.0f, 0.0f, arg1->rotation.z);

    gSPMatrix(dl++, OS_K0_TO_PHYSICAL(&arg1->translationM), G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
    gSPMatrix(dl++, OS_K0_TO_PHYSICAL(&arg1->scale), G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
    gSPMatrix(dl++, OS_K0_TO_PHYSICAL(&arg1->rotationX), G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
    gSPMatrix(dl++, OS_K0_TO_PHYSICAL(&arg1->rotationY), G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
    gSPMatrix(dl++, OS_K0_TO_PHYSICAL(&arg1->rotationZ), G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);

    if (currentWorldRotationAngles.x < 0.0f) {
        currentWorldRotationAngles.x += 360.0f;
    }
    
    if (currentWorldRotationAngles.x >= 360.0f) {
        currentWorldRotationAngles.x -= 360.0f;
    }
    
    if (currentWorldRotationAngles.y < 0.0f) {
        currentWorldRotationAngles.y += 360.0f;
    }
    
    if (currentWorldRotationAngles.y >= 360.0f) {
        currentWorldRotationAngles.y -= 360.0f;
    }

    sinXCurrent = sinfRadians(currentWorldRotationAngles.x);
    sinYCurrent = sinfRadians(currentWorldRotationAngles.y);
    
    cosXCurrent = cosfRadians(currentWorldRotationAngles.x);
    cosYCurrent = cosfRadians(currentWorldRotationAngles.y);
    
    sinXPrevious = sinfRadians(previousWorldRotationAngles.x);
    sinYPrevious = sinfRadians(previousWorldRotationAngles.y);
    
    cosXPrevious = cosfRadians(previousWorldRotationAngles.x);
    cosYPrevious = cosfRadians(previousWorldRotationAngles.y);

    for (i = spriteCounter; i < 0x150; i++) {
        D_8018A850[i].flags = 0;
    }

    for (i = 0; i < 0x150; i++) {
        
        if (D_8018A850[i].flags & ACTIVE) {
        
            if (D_8018A850[i].flags & 0x40) {

                vec = D_8018A850[i].positions;
                
            } else {
        
                x = D_8018A850[i].positions.x;
                y = D_8018A850[i].positions.y;
                z = D_8018A850[i].positions.z;
        
                rotatedX = (D_8018A850[i].positions.z * sinYCurrent) + (x * cosYCurrent);
                tempZ = (D_8018A850[i].positions.z * cosYCurrent) - (x * sinYCurrent);
                rotatedY = ((-tempZ) * sinXCurrent) + (y * cosXCurrent);
        
                vec.x = rotatedX;
                vec.y = rotatedY;
                vec.z = (tempZ * cosXCurrent) + (y * sinXCurrent);
                       
            }

            if (D_8018A850[i].flags & 0x80) {
                guTranslate(&D_8018A850[i].translation, vec.x, vec.y, vec.z + 24.0f);
            } else {
                guTranslate(&D_8018A850[i].translation, vec.x, vec.y, vec.z);
            }
            
            gSPMatrix(dl++, OS_K0_TO_PHYSICAL(&D_8018A850[i].translation), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

            if (D_8018A850[i].flags & UPDATE_ROTATION) {

                guRotateRPY(&D_8018A850[i].rotationX, D_8018A850[i].rotation.x, 0.0f, 0.0f);
                guRotateRPY(&D_8018A850[i].rotationY, 0.0f, D_8018A850[i].rotation.y, 0.0f);
                guRotateRPY(&D_8018A850[i].rotationZ, 0.0f, 0.0f, D_8018A850[i].rotation.z);
                
                gSPMatrix(dl++, OS_K0_TO_PHYSICAL(&D_8018A850[i].rotationX), G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
                gSPMatrix(dl++, OS_K0_TO_PHYSICAL(&D_8018A850[i].rotationY), G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
                gSPMatrix(dl++, OS_K0_TO_PHYSICAL(&D_8018A850[i].rotationZ), G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
                
            }

            if (D_8018A850[i].flags & UPDATE_SCALE) {

                guScale(&D_8018A850[i].scale, D_8018A850[i].scaling.x, D_8018A850[i].scaling.y, D_8018A850[i].scaling.z);                
                gSPMatrix(dl++, OS_K0_TO_PHYSICAL(&D_8018A850[i].scale), G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);

            }
 
            gSPDisplayList(dl++, OS_K0_TO_PHYSICAL(D_8018A850[i].dl));
            
            D_8018A850[i].flags = 0;
            
        }
 
    }
    
    return dl++;
    
}