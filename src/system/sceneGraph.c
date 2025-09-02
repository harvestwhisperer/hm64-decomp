#include "common.h"

#include "system/sceneGraph.h"

#include "system/graphic.h"
#include "system/map.h"

// bss
u16 sceneNodeCounter;
// unused
u8 D_80189135[9];
SceneNode sceneNodes[MAX_SCENE_NODES];


//INCLUDE_ASM("asm/nonmatchings/system/sceneGraph", initializeSceneNodes);

void initializeSceneNodes(void) {

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
 
    for (i = 0; i < MAX_SCENE_NODES; i++) {
        sceneNodes[i].flags = 0;
        sceneNodes[i].positions.x = 0;
        sceneNodes[i].positions.y = 0;
        sceneNodes[i].positions.z = 0;
        sceneNodes[i].scaling.x = 0;
        sceneNodes[i].scaling.y = 0;
        sceneNodes[i].scaling.z = 0;
        sceneNodes[i].rotation.x = 0;
        sceneNodes[i].rotation.y = 0;
        sceneNodes[i].rotation.z = 0;
    }
    
    resetSceneNodeCounter();
    
}

//INCLUDE_ASM("asm/nonmatchings/system/sceneGraph", resetSceneNodeCounter);

void resetSceneNodeCounter(void) {
    sceneNodeCounter = 0;
}

void func_80029294(void) {}

//INCLUDE_ASM("asm/nonmatchings/system/sceneGraph", addSceneNode);

u16 addSceneNode(Gfx *dl, u16 flags) {

    u16 index = sceneNodeCounter;

    sceneNodes[index].dl = dl;
    sceneNodes[index].flags = flags | 1;
    
    sceneNodeCounter++;
    
    return index;

}

//INCLUDE_ASM("asm/nonmatchings/system/sceneGraph", addSceneNodePosition);

bool addSceneNodePosition(u16 index, f32 x, f32 y, f32 z) {
    sceneNodes[index].positions.x = x;
    sceneNodes[index].positions.y = y;
    sceneNodes[index].positions.z = z;
    return TRUE;
}

//INCLUDE_ASM("asm/nonmatchings/system/sceneGraph", addSceneNodeScaling);

bool addSceneNodeScaling(u16 index, f32 x, f32 y, f32 z) {
    sceneNodes[index].scaling.x = x;
    sceneNodes[index].scaling.y = y;
    sceneNodes[index].scaling.z = z;
    return TRUE;
}

//INCLUDE_ASM("asm/nonmatchings/system/sceneGraph", addSceneNodeRotation);

bool addSceneNodeRotation(u16 index, f32 x, f32 y, f32 z) {
    sceneNodes[index].rotation.x = x;
    sceneNodes[index].rotation.y = y;
    sceneNodes[index].rotation.z = z;
    return TRUE;
}

void func_800293B8(void) {}

//INCLUDE_ASM("asm/nonmatchings/system/sceneGraph", renderSceneGraph);

Gfx* renderSceneGraph(Gfx* dl, SceneMatrices* matrices) {

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

    guTranslate(&matrices->translationM, matrices->translation.x, matrices->translation.y, matrices->translation.z);
    guScale(&matrices->scale, matrices->scaling.x, matrices->scaling.y, matrices->scaling.z);

    guRotateRPY(&matrices->rotationX, matrices->rotation.x, 0.0f, 0.0f);
    guRotateRPY(&matrices->rotationY, 0.0f, matrices->rotation.y, 0.0f);
    guRotateRPY(&matrices->rotationZ, 0.0f, 0.0f, matrices->rotation.z);

    gSPMatrix(dl++, OS_K0_TO_PHYSICAL(&matrices->translationM), G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
    gSPMatrix(dl++, OS_K0_TO_PHYSICAL(&matrices->scale), G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
    gSPMatrix(dl++, OS_K0_TO_PHYSICAL(&matrices->rotationX), G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
    gSPMatrix(dl++, OS_K0_TO_PHYSICAL(&matrices->rotationY), G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
    gSPMatrix(dl++, OS_K0_TO_PHYSICAL(&matrices->rotationZ), G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);

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

    for (i = sceneNodeCounter; i < MAX_SCENE_NODES; i++) {
        sceneNodes[i].flags = 0;
    }

    for (i = 0; i < MAX_SCENE_NODES; i++) {
        
        if (sceneNodes[i].flags & ACTIVE) {
        
            if (sceneNodes[i].flags & 0x40) {

                vec = sceneNodes[i].positions;
                
            } else {
        
                x = sceneNodes[i].positions.x;
                y = sceneNodes[i].positions.y;
                z = sceneNodes[i].positions.z;
        
                // FIXME: don't use temp
                rotatedX = (sceneNodes[i].positions.z * sinYCurrent) + (x * cosYCurrent);
                tempZ = (sceneNodes[i].positions.z * cosYCurrent) - (x * sinYCurrent);
                rotatedY = ((-tempZ) * sinXCurrent) + (y * cosXCurrent);
        
                vec.x = rotatedX;
                vec.y = rotatedY;
                vec.z = (tempZ * cosXCurrent) + (y * sinXCurrent);
                       
            }

            if (sceneNodes[i].flags & 0x80) {
                guTranslate(&sceneNodes[i].translation, vec.x, vec.y, vec.z + 24.0f);
            } else {
                guTranslate(&sceneNodes[i].translation, vec.x, vec.y, vec.z);
            }
            
            gSPMatrix(dl++, OS_K0_TO_PHYSICAL(&sceneNodes[i].translation), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

            if (sceneNodes[i].flags & UPDATE_ROTATION) {

                guRotateRPY(&sceneNodes[i].rotationX, sceneNodes[i].rotation.x, 0.0f, 0.0f);
                guRotateRPY(&sceneNodes[i].rotationY, 0.0f, sceneNodes[i].rotation.y, 0.0f);
                guRotateRPY(&sceneNodes[i].rotationZ, 0.0f, 0.0f, sceneNodes[i].rotation.z);
                
                gSPMatrix(dl++, OS_K0_TO_PHYSICAL(&sceneNodes[i].rotationX), G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
                gSPMatrix(dl++, OS_K0_TO_PHYSICAL(&sceneNodes[i].rotationY), G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
                gSPMatrix(dl++, OS_K0_TO_PHYSICAL(&sceneNodes[i].rotationZ), G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
                
            }

            if (sceneNodes[i].flags & UPDATE_SCALE) {

                guScale(&sceneNodes[i].scale, sceneNodes[i].scaling.x, sceneNodes[i].scaling.y, sceneNodes[i].scaling.z);                
                gSPMatrix(dl++, OS_K0_TO_PHYSICAL(&sceneNodes[i].scale), G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);

            }
 
            gSPDisplayList(dl++, OS_K0_TO_PHYSICAL(sceneNodes[i].dl));
            
            sceneNodes[i].flags = 0;
            
        }
 
    }
    
    return dl++;
    
}