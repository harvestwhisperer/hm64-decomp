#include "common.h"
#include "yay0.h"

#include "system/mapController.h"

#include "system/entity.h"
#include "system/graphic.h"
#include "system/map.h"
#include "system/sceneGraph.h"

MapController mapControllers[1];
MapDataAddress mapDataAddresses[96];

// bss
Vec4f targetMapLightingRGBA;
Vec4f currentMapLightingRGBA;

// RGBA update rate
s16 D_8017045A;

Vec3f previousWorldRotationAngles;
Vec3f currentWorldRotationAngles;

// forward declarations
void updateMapViewport(MapController*); 
void handleMapRotation(u16 mapIndex);

// y rotations for maps
static const f32 yRotationAngles[];

void initializeMapControllers(void) {
    
    u16 i;
    
    currentMapLightingRGBA.r = 0;
    currentMapLightingRGBA.g = 0;
    currentMapLightingRGBA.b = 0;
    currentMapLightingRGBA.a = 0;
    
    targetMapLightingRGBA.r = 0;
    targetMapLightingRGBA.g = 0;
    targetMapLightingRGBA.b = 0;
    targetMapLightingRGBA.a = 0;
    
    D_8017045A = 0;
    
    for (i = 0; i < MAX_MAPS; i++) {
        MapController *mc = &mapControllers[i];

        mc->flags = 0;

        mc->mainMapIndex = 0;

        mc->viewPosition.x = 0;
        mc->viewPosition.y = 0;
        mc->viewPosition.z = 0;

        mc->rotation = 0;

        mc->currentTileX = 0;
        mc->currentTileZ = 0;

        mc->viewBoundsCorner0.x = 0;
        mc->viewBoundsCorner0.y = 0;
        mc->viewBoundsCorner0.z = 0;

        mc->viewBoundsCorner1.x = 0;
        mc->viewBoundsCorner1.y = 0;
        mc->viewBoundsCorner1.z = 0;

        mc->viewBoundsCorner2.x = 0;
        mc->viewBoundsCorner2.y = 0;
        mc->viewBoundsCorner2.z = 0;

        mc->viewBoundsCorner3.x = 0;
        mc->viewBoundsCorner3.y = 0;
        mc->viewBoundsCorner3.z = 0;

    }
}

bool setMapDataAddresses(u16 mapIndex, void *start, void *end) {

    MapDataAddress *mda = &mapDataAddresses[mapIndex];
    
    bool result = FALSE;
    
    if (mapIndex < MAX_MAP_ADDRESSES) {
        if (!(mda->flags & MAP_DATA_ADDRESS_ACTIVE)) {
            result = TRUE;
            mda->romStart = start;
            mda->romEnd = end;
            mda->flags = MAP_DATA_ADDRESS_ACTIVE;
        }
    }

    return result;

}

bool initializeMapController(u16 index, u16 mapIndex, u32 *mapDataIndex) {

    MapController *mc = &mapControllers[index];
    
    bool result;

    result = FALSE;
    
    if (index == MAIN_MAP_INDEX && !(mc->flags & MAP_CONTROLLER_INITIALIZED)) {
        
        result = TRUE;
        
        mc->mainMapIndex = mapIndex;
        
        mc->mapDataIndex = mapDataIndex;
        
        mc->flags = MAP_CONTROLLER_INITIALIZED;

        mc->viewPosition.x = 0;
        mc->viewPosition.y = 0;
        mc->viewPosition.z = 0;

        mc->currentTileX = 0;
        mc->currentTileZ = 0;

        mc->viewBoundsCorner0.x = 0;
        mc->viewBoundsCorner0.y = 0;
        mc->viewBoundsCorner0.z = 0;

        mc->viewBoundsCorner1.x = 0;
        mc->viewBoundsCorner1.y = 0;
        mc->viewBoundsCorner1.z = 0;

        mc->viewBoundsCorner2.x = 0;
        mc->viewBoundsCorner2.y = 0;
        mc->viewBoundsCorner2.z = 0;

        mc->viewBoundsCorner3.x = 0;
        mc->viewBoundsCorner3.y = 0;
        mc->viewBoundsCorner3.z = 0;

    }
    
    return result;
}

bool loadMap(u16 index, u16 mapIndex) {

    MapController *mc = &mapControllers[index];

    bool result = FALSE;
    
    if (index == MAIN_MAP_INDEX && (mc->flags & MAP_CONTROLLER_INITIALIZED)) {
        
        dmaMapAssets(MAIN_MAP_INDEX, mapIndex);
        
        setMapTranslation(mc->mainMapIndex, 0.0f, 0.0f, 0.0f);
        setMapScale(mc->mainMapIndex, 1.0f, 1.0f, 1.0f);
        setMapRotation(mc->mainMapIndex, 45.0f, 0.0f, 0.0f);
        
        mc->rotation = 0;
        
        setMapControllerViewPosition(MAIN_MAP_INDEX, 0.0f, 0.0f, 0.0f);
        setInitialMapRotation(MAIN_MAP_INDEX, MAP_ROTATION_S);
        setMapViewPositionAndCurrentTile(MAIN_MAP_INDEX, 0.0f, 0.0f, 0.0f, 0, 0);
        setMapRGBA(mc->mainMapIndex, 0, 0, 0, 0);
        
        result = TRUE; 

    } 

    return result;

}

static inline u8* getAddress(u32 offsets[], u32 i) {
    return (u8*)offsets + offsets[i];
}

bool dmaMapAssets(u16 mainMapIndex, u16 levelMapIndex) {

    MapDataAddress *mda = &mapDataAddresses[levelMapIndex];

    MapController *mc = &mapControllers[mainMapIndex];

    bool result = FALSE;
    
    u8 *mapGrid;
    void *mesh;
    u8 *terrainQuads;
    u8 **gridToLevelInteractionIndex;
    void *coreMapObjects;
    void *tileTextures;
    void *tilePalettes;
    void *coreMapObjectsTextures;
    void *coreMapObjectsPalettes;
    u8 *mapAdditionsMetadata; 
 
    if (mainMapIndex == MAIN_MAP_INDEX && mc->flags & MAP_CONTROLLER_INITIALIZED) {
        
        mc->mapIndex = levelMapIndex;
        
        dmaReadRom(mda->romStart, mc->mapDataIndex, mda->romEnd - mda->romStart);
 
        mapGrid = getAddress(mc->mapDataIndex, 0);
        mesh = getAddress(mc->mapDataIndex, 1);
        terrainQuads = getAddress(mc->mapDataIndex, 2);
        gridToLevelInteractionIndex = getAddress(mc->mapDataIndex, 3);
        coreMapObjects = getAddress(mc->mapDataIndex, 4);
        tileTextures = getAddress(mc->mapDataIndex, 5);
        tilePalettes = getAddress(mc->mapDataIndex, 6);
        coreMapObjectsTextures = getAddress(mc->mapDataIndex, 7);
        coreMapObjectsPalettes = getAddress(mc->mapDataIndex, 8);
        mapAdditionsMetadata = getAddress(mc->mapDataIndex, 9);
         
        mc->flags |= MAP_CONTROLLER_ASSETS_LOADED;
                
        setupMap(mc->mainMapIndex, 
            mapGrid, 
            mesh, 
            terrainQuads, 
            gridToLevelInteractionIndex, 
            coreMapObjects, 
            tileTextures, 
            tilePalettes, 
            coreMapObjectsTextures, 
            coreMapObjectsPalettes, 
            mapAdditionsMetadata
            );
        
        result = TRUE;
    
    }

    return result;
    
}

bool enableMapController(u16 mapIndex) {

    MapController *mc = &mapControllers[mapIndex];
    
    bool result = FALSE;
    
    if (mapIndex == MAIN_MAP_INDEX && (mc->flags & MAP_CONTROLLER_INITIALIZED) && (mc->flags & MAP_CONTROLLER_ASSETS_LOADED)) {
        mc->flags |= MAP_CONTROLLER_ACTIVE;
        result = TRUE;
    }
    
    return result;

}

bool setMapControllerViewPosition(u16 mapIndex, f32 x, f32 y, f32 z) {

    MapController *mc = &mapControllers[mapIndex];
    
    bool result = FALSE;
    
    if (mapIndex == MAIN_MAP_INDEX && (mc->flags & MAP_CONTROLLER_INITIALIZED) && (mc->flags & MAP_CONTROLLER_ASSETS_LOADED)) {
        
        mc->viewPosition.x = x;
        mc->viewPosition.y = y;
        mc->viewPosition.z = z;
        
        result = TRUE;

    }
    
    return result;

}

bool adjustMapControllerViewPosition(u16 mapIndex, f32 x, f32 y, f32 z) {

    MapController *mc = &mapControllers[mapIndex];
    
    bool result = FALSE;
    
    if (mapIndex == MAIN_MAP_INDEX && (mc->flags & MAP_CONTROLLER_INITIALIZED) && (mc->flags & MAP_CONTROLLER_ASSETS_LOADED)) {
    
        mc->viewPosition.x += x;
        mc->viewPosition.y += y;
        mc->viewPosition.z += z;

        result = TRUE;
    
    }
    
    return result;

}

bool setMapControllerRGBA(u16 mapIndex, u8 r, u8 g, u8 b, u8 a) {

    MapController *mc = &mapControllers[mapIndex];
    
    bool result = FALSE;
    
    if (mapIndex == MAIN_MAP_INDEX && (mc->flags & MAP_CONTROLLER_INITIALIZED) && (mc->flags & MAP_CONTROLLER_ASSETS_LOADED)) {
    
        setMapRGBA(mc->mainMapIndex, r, g, b, a);
    
        currentMapLightingRGBA.r = r;
        currentMapLightingRGBA.g = g;
        currentMapLightingRGBA.b = b;
        currentMapLightingRGBA.a = a;

        result = TRUE;
    
    }
    
    return result;

}

bool setMapControllerRGBAWithTransition(u16 mapIndex, u8 r, u8 g, u8 b, u8 a, s16 rate) {

    MapController *mc = &mapControllers[mapIndex];
    
    bool result = FALSE;
    
    if (mapIndex == MAIN_MAP_INDEX && (mc->flags & MAP_CONTROLLER_INITIALIZED) && (mc->flags & MAP_CONTROLLER_ASSETS_LOADED)) {

        setMapRGBAWithTransition(mc->mainMapIndex, r, g, b, a, rate);

        targetMapLightingRGBA.r = r;
        targetMapLightingRGBA.g = g;
        targetMapLightingRGBA.b = b;
        targetMapLightingRGBA.a = a;
        
        D_8017045A = rate;
        
        result = TRUE;

    }
    
    return result;

}

bool setInitialMapRotation(u16 mapIndex, u8 rotationIndex) {

    MapController *mc = &mapControllers[mapIndex];
    
    bool result;

    f32 tempf;

    result = FALSE;

    if (mapIndex == MAIN_MAP_INDEX && mc->flags & MAP_CONTROLLER_INITIALIZED && mc->flags & MAP_CONTROLLER_ASSETS_LOADED) {

        if (!(mc->flags & (MAP_CONTROLLER_ROTATING_COUNTERCLOCKWISE | MAP_CONTROLLER_ROTATING_CLOCKWISE))) {
        
            mc->rotation = rotationIndex;
            tempf = yRotationAngles[rotationIndex];
        
            setMapRotation(mc->mainMapIndex, 45.0f, tempf, 0);
            setInitialWorldRotationAngles(45.0f, tempf, 0);
         
            result = TRUE;

        }
    }

    return result;
    
}

u8 getCurrentMapRotation(u16 mapIndex) {

    MapController *mc = &mapControllers[mapIndex];

    u8 rotation = 0;

    if (mapIndex == MAIN_MAP_INDEX && (mc->flags & MAP_CONTROLLER_INITIALIZED)) {
        rotation = mc->rotation;
    }

    return rotation;
    
}

bool setMapViewPositionAndCurrentTile(u16 mapIndex, f32 x, f32 y, f32 z, u8 arg4, u8 arg5) {

    MapController *mc = &mapControllers[mapIndex];

    bool result = FALSE;

    if (mapIndex == MAIN_MAP_INDEX && (mc->flags & MAP_CONTROLLER_INITIALIZED) && (mc->flags & MAP_CONTROLLER_ASSETS_LOADED)) {
        
        mc->currentTileX = arg4;
        mc->currentTileZ = arg5;

        mc->viewPosition.x = x;
        mc->viewPosition.y = y;
        mc->viewPosition.z = z;

        updateMapViewport(mc);

        result = TRUE;

    }
    
    return result;
    
}

bool setMapBoundaries(u16 mapIndex, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8) {

    MapController *mc = &mapControllers[mapIndex];

    bool result = FALSE;
    
    if (mapIndex == MAIN_MAP_INDEX && (mc->flags & MAP_CONTROLLER_INITIALIZED)) {

        mc->viewBoundsCorner0.x = arg1;
        mc->viewBoundsCorner0.z = arg2;

        mc->viewBoundsCorner1.x = arg3;
        mc->viewBoundsCorner1.z = arg4;

        mc->viewBoundsCorner2.x = arg5;
        mc->viewBoundsCorner2.z = arg6;

        mc->viewBoundsCorner3.x = arg7;
        mc->viewBoundsCorner3.z = arg8;
        
        result = TRUE;
    }

    return result;
    
}

// unused or inline
// f32 getClampedMapX(u16 mapIndex, f32 x) {

//     f32 result = 0.0f;

//     if (mapIndex == MAIN_MAP_INDEX && (mapControllers[mapIndex].flags & MAP_CONTROLLER_INITIALIZED)) { 

//         if (mapControllers[mapIndex].viewBoundsCorner0.x < x && mapControllers[mapIndex].viewBoundsCorner3.x < x) {
            
//             if (!(x < mapControllers[mapIndex].viewBoundsCorner1.x) || !(x < mapControllers[mapIndex].viewBoundsCorner2.x)) {
//                 result = mapControllers[mapIndex].viewBoundsCorner1.x;
//             } 
            
//         } else {
//             result = mapControllers[mapIndex].viewBoundsCorner0.x;
//         }
//     }
    
//     return result;

// }

// unused or inline
// f32 getClampedMapZ(u16 mapIndex, f32 z) {

//     f32 result = 0.0f;
    
//     if (mapIndex == MAIN_MAP_INDEX && mapControllers[mapIndex].flags & MAP_CONTROLLER_INITIALIZED) {

//         if (z < mapControllers[mapIndex].viewBoundsCorner0.z && z < mapControllers[mapIndex].viewBoundsCorner1.z) {

//             if (!(z > mapControllers[mapIndex].viewBoundsCorner2.z) || !(z > mapControllers[mapIndex].viewBoundsCorner3.z)) {
//                 result = mapControllers[mapIndex].viewBoundsCorner2.z;
//             }
            
//         } else {
//             result = mapControllers[mapIndex].viewBoundsCorner0.z;
//         }
//     } 

//     return result;
    
// }

// unused or inlne
// bool shutdownMapController(u16 mapIndex) {

//     bool result = FALSE;

//     if (mapIndex == MAIN_MAP_INDEX && (mapControllers[mapIndex].flags & MAP_CONTROLLER_INITIALIZED)) {
//         deactivateMapSprites(mapControllers[mapIndex].mainMapIndex);
//         mapControllers[mapIndex].flags = 0;
//         result = TRUE;
//     }

//     return result;

// }

bool unloadMapAssets(u16 mapIndex) {

    MapController *mc = &mapControllers[mapIndex];

    bool result = FALSE;

    if (mapIndex == MAIN_MAP_INDEX && (mc->flags & MAP_CONTROLLER_INITIALIZED) && (mc->flags & MAP_CONTROLLER_ACTIVE)) {
        deactivateMapSprites(mc->mainMapIndex);
        mc->flags &= ~(MAP_CONTROLLER_ASSETS_LOADED | MAP_CONTROLLER_ACTIVE);
        result = TRUE;
    }

    return result;

}

void deactivateAllMapControllers(void) {

    u16 i;

    for (i = 0; i < MAX_MAPS; i++) {
        mapControllers[i].flags &= ~(MAP_CONTROLLER_ASSETS_LOADED | MAP_CONTROLLER_ACTIVE);        
    }    

}

bool startMapRotation(u16 mapIndex, u8 arg1, u8 targetRotation) {

    MapController *mc = &mapControllers[mapIndex];

    bool result = FALSE;

    if (mapIndex == MAIN_MAP_INDEX && (mc->flags & MAP_CONTROLLER_INITIALIZED) && (mc->flags & MAP_CONTROLLER_ACTIVE)) { 

        if (!(mc->flags & (MAP_CONTROLLER_ROTATING_COUNTERCLOCKWISE | MAP_CONTROLLER_ROTATING_CLOCKWISE)) && targetRotation != mc->rotation) {
            
            if (arg1 == 0xFF) {

                if (mc->rotation < 4) {

                    if ((mc->rotation + 4) >= targetRotation && targetRotation >= mc->rotation) {
                        mc->flags |= MAP_CONTROLLER_ROTATING_CLOCKWISE;
                    } else {
                        mc->flags |= MAP_CONTROLLER_ROTATING_COUNTERCLOCKWISE;
                    }
                    
                } else {

                    if (targetRotation < mc->rotation) {

                        if ((mc->rotation - 4) >= targetRotation) { 
                            mc->flags |= MAP_CONTROLLER_ROTATING_CLOCKWISE;
                        } else {
                            mc->flags |= MAP_CONTROLLER_ROTATING_COUNTERCLOCKWISE;
                        }
                        
                    } else {
                        mc->flags |= MAP_CONTROLLER_ROTATING_CLOCKWISE;
                    }
                    
                }
                
            } else {
                if (!arg1) {
                    mc->flags |= MAP_CONTROLLER_ROTATING_COUNTERCLOCKWISE;
                } else {
                    mc->flags |= MAP_CONTROLLER_ROTATING_CLOCKWISE;
                }
            }

            result = TRUE;
            
            mc->targetRotation = targetRotation;
            mc->rotationFrameCounter = 0;
            
        }

    }

    return result;
    
}

void updateMapController(void) {

    u16 i;

    for (i = 0; i < MAX_MAPS; i++) {
        MapController *mc = &mapControllers[i];

        if ((mc->flags & MAP_CONTROLLER_INITIALIZED) && (mc->flags & MAP_CONTROLLER_ACTIVE)) {

            updateGroundObjects(mc->mainMapIndex);
            updateMapViewport(mc);

            if (mc->flags & (MAP_CONTROLLER_ROTATING_COUNTERCLOCKWISE | MAP_CONTROLLER_ROTATING_CLOCKWISE)) {
                handleMapRotation(i);
            }

            currentMapLightingRGBA.r = mainMap[mc->mainMapIndex].mapGlobals.currentRGBA.r;
            currentMapLightingRGBA.g = mainMap[mc->mainMapIndex].mapGlobals.currentRGBA.g;
            currentMapLightingRGBA.b = mainMap[mc->mainMapIndex].mapGlobals.currentRGBA.b;
            currentMapLightingRGBA.a = mainMap[mc->mainMapIndex].mapGlobals.currentRGBA.a;

        }

    }
    
}

void updateMapViewport(MapController* mapController) {
    
    f32 x;
    f32 y;
    f32 z;
    
    u8 param1;
    u8 param2;
    
    f32 param3;
    f32 param4;
    
    f32 temp1;
    f32 temp2;
    
    x = mapController->viewPosition.x + ((mainMap[mapController->mainMapIndex].mapGrid.mapWidth * mainMap[mapController->mainMapIndex].mapGrid.tileSizeX) / 2);
    param1 = x / mainMap[mapController->mainMapIndex].mapGrid.tileSizeX;
    
    z = mapController->viewPosition.z + ((mainMap[mapController->mainMapIndex].mapGrid.mapHeight * mainMap[mapController->mainMapIndex].mapGrid.tileSizeZ) / 2);
    
    temp1 = param1 * mainMap[mapController->mainMapIndex].mapGrid.tileSizeX;
    
    param3 = x - temp1;
    
    y = mapController->viewPosition.y;
    
    param2 = z / mainMap[mapController->mainMapIndex].mapGrid.tileSizeZ;

    temp2 = param2 * mainMap[mapController->mainMapIndex].mapGrid.tileSizeZ;

    param4 = z - temp2;
    
    setMapCameraView(mapController->mainMapIndex, param1, param2, mapController->currentTileX, mapController->currentTileZ, param3, y, param4, 1);

}

void handleMapRotation(u16 mapIndex) {

    MapController *mc = &mapControllers[mapIndex];

    if (mc->flags & MAP_CONTROLLER_ROTATING_COUNTERCLOCKWISE) {
        adjustMapRotation(mc->mainMapIndex, 0, -1.0f, 0);
        adjustCurrentWorldRotationAngles(0, -1.0f, 0);
    } else {
        adjustMapRotation(mc->mainMapIndex, 0, 1.0f, 0);
        adjustCurrentWorldRotationAngles(0, 1.0f, 0);
    } 

    mc->rotationFrameCounter++;

    if (mc->rotationFrameCounter == 22) {
        if (mc->flags & MAP_CONTROLLER_ROTATING_COUNTERCLOCKWISE) {
            rotateAllEntities(1);
        } else {
            rotateAllEntities(-1);
        }
    }

    if (mc->rotationFrameCounter >= 45) { 

        if (mc->flags & MAP_CONTROLLER_ROTATING_COUNTERCLOCKWISE) { 
            mc->rotation = (mc->rotation + 7) % 8; 
        } else {
            mc->rotation = (mc->rotation + 1) % 8;
        }

        mc->flags &= ~( 8 | MAP_CONTROLLER_ROTATING_CLOCKWISE);

        if (mc->targetRotation == 0xFF || mc->rotation == mc->targetRotation) {
            previousWorldRotationAngles.x = currentWorldRotationAngles.x;
            previousWorldRotationAngles.y = currentWorldRotationAngles.y;
            previousWorldRotationAngles.z = currentWorldRotationAngles.z;
        } else {
            startMapRotation(mapIndex, 0xFF, mc->targetRotation);
        }
        
    }
    
}

// y rotation values
static const f32 yRotationAngles[] = { 0, 45.0f, 90.0f, 135.0f, 180.0f, 225.0f, 270.0f, 315.0f };