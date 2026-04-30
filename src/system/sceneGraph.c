#include "common.h"

#include "system/mapController.h"
#include "system/sceneGraph.h"

#include "system/graphic.h"
#include "system/map.h"

// bss
u16 sceneNodeCounter;
SceneNode sceneNodes[MAX_SCENE_NODES];
SceneMatrices sceneMatrices[2];

void initializeSceneNodes(void) {

    u16 i;

    for (i = 0; i < MAX_SCENE_NODES; i++) {
        SceneNode *sn = &sceneNodes[i];
        sn->flags = 0;
        sn->positions.x = 0;
        sn->positions.y = 0;
        sn->positions.z = 0;
        sn->scaling.x = 0;
        sn->scaling.y = 0;
        sn->scaling.z = 0;
        sn->rotation.x = 0;
        sn->rotation.y = 0;
        sn->rotation.z = 0;
    }
    
    resetSceneNodeCounter();
    
}

void resetSceneNodeCounter(void) {
    sceneNodeCounter = 0;
}

// empty function
// void func_80029294(void) {}

u16 addSceneNode(Gfx *dl, u16 flags) {

    u16 index = sceneNodeCounter;
    SceneNode *sn = &sceneNodes[index];

    sn->dl = dl;
    sn->flags = flags | SCENE_NODE_ACTIVE;

    sceneNodeCounter++;

    return index;

}

bool addSceneNodePosition(u16 index, f32 x, f32 y, f32 z) {
    SceneNode *sn = &sceneNodes[index];
    sn->positions.x = x;
    sn->positions.y = y;
    sn->positions.z = z;
    return TRUE;
}

bool addSceneNodeScaling(u16 index, f32 x, f32 y, f32 z) {
    SceneNode *sn = &sceneNodes[index];
    sn->scaling.x = x;
    sn->scaling.y = y;
    sn->scaling.z = z;
    return TRUE;
}

bool addSceneNodeRotation(u16 index, f32 x, f32 y, f32 z) {
    SceneNode *sn = &sceneNodes[index];
    sn->rotation.x = x;
    sn->rotation.y = y;
    sn->rotation.z = z;
    return TRUE;
}

// empty function
// void func_800293B8(void) {}

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
        SceneNode *sn = &sceneNodes[i];

        if (sn->flags & SCENE_NODE_ACTIVE) {

            if (sn->flags & SCENE_NODE_TRANSFORM_EXEMPT) {

                vec = sn->positions;

            } else {

                x = sn->positions.x;
                y = sn->positions.y;
                z = sn->positions.z;

                vec.x = (z * sinYCurrent) + (x * cosYCurrent);
                vec.z = (z * cosYCurrent) - (x * sinYCurrent);
                vec.y = ((-vec.z) * sinXCurrent) + (y * cosXCurrent);
                vec.z = (vec.z * cosXCurrent) + (y * sinXCurrent);

            }

            if (sn->flags & 0x80) {
                guTranslate(&sn->translation, vec.x, vec.y, vec.z + 24.0f);
            } else {
                guTranslate(&sn->translation, vec.x, vec.y, vec.z);
            }

            gSPMatrix(dl++, OS_K0_TO_PHYSICAL(&sn->translation), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

            if (sn->flags & SCENE_NODE_UPDATE_ROTATION) {

                guRotateRPY(&sn->rotationX, sn->rotation.x, 0.0f, 0.0f);
                guRotateRPY(&sn->rotationY, 0.0f, sn->rotation.y, 0.0f);
                guRotateRPY(&sn->rotationZ, 0.0f, 0.0f, sn->rotation.z);

                gSPMatrix(dl++, OS_K0_TO_PHYSICAL(&sn->rotationX), G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
                gSPMatrix(dl++, OS_K0_TO_PHYSICAL(&sn->rotationY), G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
                gSPMatrix(dl++, OS_K0_TO_PHYSICAL(&sn->rotationZ), G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);

            }

            if (sn->flags & SCENE_NODE_UPDATE_SCALE) {

                guScale(&sn->scale, sn->scaling.x, sn->scaling.y, sn->scaling.z);
                gSPMatrix(dl++, OS_K0_TO_PHYSICAL(&sn->scale), G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);

            }

            gSPDisplayList(dl++, OS_K0_TO_PHYSICAL(sn->dl));

            sn->flags = 0;

        }

    }
    
    return dl++;
    
}