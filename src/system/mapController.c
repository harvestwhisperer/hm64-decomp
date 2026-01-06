#include "common.h"

#include "system/mapController.h"

#include "system/entity.h"
#include "system/graphic.h"
#include "system/map.h"
#include "system/sceneGraph.h"

// bss
Vec4f targetMapLightingRGBA;
Vec4f currentMapLightingRGBA;

// RGBA update rate
s16 D_8017045A;

MapController mapControllers[1];
MapDataAddress mapDataAddresses[96];

Vec3f previousWorldRotationAngles;
Vec3f currentWorldRotationAngles;

// forward declarations
void updateMapViewport(MapController*); 
void handleMapRotation(u16 mapIndex);

// y rotations for maps
static const f32 yRotationAngles[];
                    
//INCLUDE_ASM("asm/nonmatchings/system/mapController", initializeMapControllers);

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
        
        mapControllers[i].flags = 0;
        
        mapControllers[i].mainMapIndex = 0;
        
        mapControllers[i].viewPosition.x = 0;
        mapControllers[i].viewPosition.y = 0;
        mapControllers[i].viewPosition.z = 0;

        mapControllers[i].rotation = 0;
        
        mapControllers[i].currentTileX = 0;
        mapControllers[i].currentTileZ = 0;
        
        mapControllers[i].viewBoundsCorner0.x = 0;
        mapControllers[i].viewBoundsCorner0.y = 0;
        mapControllers[i].viewBoundsCorner0.z = 0;

        mapControllers[i].viewBoundsCorner1.x = 0;
        mapControllers[i].viewBoundsCorner1.y = 0;
        mapControllers[i].viewBoundsCorner1.z = 0;

        mapControllers[i].viewBoundsCorner2.x = 0;
        mapControllers[i].viewBoundsCorner2.y = 0;
        mapControllers[i].viewBoundsCorner2.z = 0;

        mapControllers[i].viewBoundsCorner3.x = 0;
        mapControllers[i].viewBoundsCorner3.y = 0;
        mapControllers[i].viewBoundsCorner3.z = 0;
    
    }
}

//INCLUDE_ASM("asm/nonmatchings/system/mapController", setMapDataAddresses);

bool setMapDataAddresses(u16 mapIndex, void *start, void *end) {
    
    bool result = FALSE;
    
    if (mapIndex < MAX_MAP_ADDRESSES) {
        if (!(mapDataAddresses[mapIndex].flags & 1)) {
            result = TRUE;
            mapDataAddresses[mapIndex].romStart = start;
            mapDataAddresses[mapIndex].romEnd = end;
            mapDataAddresses[mapIndex].flags = 1;
        }
    }

    return result;
}

//INCLUDE_ASM("asm/nonmatchings/system/mapController", initializeMapController);

bool initializeMapController(u16 index, u16 mapIndex, u32 *mapDataIndex) {
    
    bool result;

    result = FALSE;
    
    if (index == MAIN_MAP_INDEX && !(mapControllers[index].flags & MAP_CONTROLLER_INITIALIZED)) {
        
        result = TRUE;
        
        mapControllers[index].mainMapIndex = mapIndex;
        
        mapControllers[index].mapDataIndex = mapDataIndex;
        
        mapControllers[index].flags = MAP_CONTROLLER_INITIALIZED;

        mapControllers[index].viewPosition.x = 0;
        mapControllers[index].viewPosition.y = 0;
        mapControllers[index].viewPosition.z = 0;

        mapControllers[index].currentTileX = 0;
        mapControllers[index].currentTileZ = 0;

        mapControllers[index].viewBoundsCorner0.x = 0;
        mapControllers[index].viewBoundsCorner0.y = 0;
        mapControllers[index].viewBoundsCorner0.z = 0;

        mapControllers[index].viewBoundsCorner1.x = 0;
        mapControllers[index].viewBoundsCorner1.y = 0;
        mapControllers[index].viewBoundsCorner1.z = 0;

        mapControllers[index].viewBoundsCorner2.x = 0;
        mapControllers[index].viewBoundsCorner2.y = 0;
        mapControllers[index].viewBoundsCorner2.z = 0;

        mapControllers[index].viewBoundsCorner3.x = 0;
        mapControllers[index].viewBoundsCorner3.y = 0;
        mapControllers[index].viewBoundsCorner3.z = 0;

    }
    
    return result;
}

//INCLUDE_ASM("asm/nonmatchings/system/mapController", loadMap);

bool loadMap(u16 index, u16 mapIndex) {

    bool result = FALSE;
    
    if (index == MAIN_MAP_INDEX && (mapControllers[index].flags & MAP_CONTROLLER_INITIALIZED)) {
        
        dmaMapAssets(MAIN_MAP_INDEX, mapIndex);
        
        setMapTranslation(mapControllers[index].mainMapIndex, 0.0f, 0.0f, 0.0f);
        setMapScale(mapControllers[index].mainMapIndex, 1.0f, 1.0f, 1.0f);
        setMapRotation(mapControllers[index].mainMapIndex, 45.0f, 0.0f, 0.0f);
        
        mapControllers[index].rotation = 0;
        
        setMapControllerViewPosition(MAIN_MAP_INDEX, 0.0f, 0.0f, 0.0f);
        setInitialMapRotation(MAIN_MAP_INDEX, SOUTHWEST);
        setMapViewPositionAndCurrentTile(MAIN_MAP_INDEX, 0.0f, 0.0f, 0.0f, 0, 0);
        setMapRGBA(mapControllers[index].mainMapIndex, 0, 0, 0, 0);
        
        result = TRUE; 

    } 

    return result;

}

static inline u8* getAddress(u32 offsets[], u32 i) {
    return (u8*)offsets + offsets[i];
}

//INCLUDE_ASM("asm/nonmatchings/system/mapController", dmaMapAssets);

bool dmaMapAssets(u16 mainMapIndex, u16 levelMapIndex) {

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
 
    if (mainMapIndex == MAIN_MAP_INDEX && mapControllers[mainMapIndex].flags & MAP_CONTROLLER_INITIALIZED) {
        
        mapControllers[mainMapIndex].mapIndex = levelMapIndex;
        
        // mapDataIndex = virtual address of map data 
        nuPiReadRom(mapDataAddresses[levelMapIndex].romStart, mapControllers[mainMapIndex].mapDataIndex, mapDataAddresses[levelMapIndex].romEnd - mapDataAddresses[levelMapIndex].romStart);
 
        mapGrid = getAddress(mapControllers[mainMapIndex].mapDataIndex, 0);
        mesh = getAddress(mapControllers[mainMapIndex].mapDataIndex, 1);
        terrainQuads = getAddress(mapControllers[mainMapIndex].mapDataIndex, 2);
        gridToLevelInteractionIndex = getAddress(mapControllers[mainMapIndex].mapDataIndex, 3);
        coreMapObjects = getAddress(mapControllers[mainMapIndex].mapDataIndex, 4);
        tileTextures = getAddress(mapControllers[mainMapIndex].mapDataIndex, 5);
        tilePalettes = getAddress(mapControllers[mainMapIndex].mapDataIndex, 6);
        coreMapObjectsTextures = getAddress(mapControllers[mainMapIndex].mapDataIndex, 7);
        coreMapObjectsPalettes = getAddress(mapControllers[mainMapIndex].mapDataIndex, 8);
        mapAdditionsMetadata = getAddress(mapControllers[mainMapIndex].mapDataIndex, 9);
         
        mapControllers[mainMapIndex].flags |= MAP_CONTROLLER_ASSETS_LOADED;
                
        setupMap(mapControllers[mainMapIndex].mainMapIndex, 
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

//INCLUDE_ASM("asm/nonmatchings/system/mapController", enableMapController);

bool enableMapController(u16 mapIndex) {
    
    bool result = FALSE;
    
    if (mapIndex == MAIN_MAP_INDEX && (mapControllers[mapIndex].flags & MAP_CONTROLLER_INITIALIZED) && (mapControllers[mapIndex].flags & MAP_CONTROLLER_ASSETS_LOADED)) {
        mapControllers[mapIndex].flags |= MAP_CONTROLLER_ACTIVE;
        result = TRUE;
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/mapContext", setMapControllerViewPosition);

bool setMapControllerViewPosition(u16 mapIndex, f32 x, f32 y, f32 z) {
    
    bool result = FALSE;
    
    if (mapIndex == MAIN_MAP_INDEX && (mapControllers[mapIndex].flags & MAP_CONTROLLER_INITIALIZED) && (mapControllers[mapIndex].flags & MAP_CONTROLLER_ASSETS_LOADED)) {
        
        mapControllers[mapIndex].viewPosition.x = x;
        mapControllers[mapIndex].viewPosition.y = y;
        mapControllers[mapIndex].viewPosition.z = z;
        
        result = TRUE;

    }
    
    return result;

}


//INCLUDE_ASM("asm/nonmatchings/system/mapContext", adjustMapControllerViewPosition);

bool adjustMapControllerViewPosition(u16 mapIndex, f32 x, f32 y, f32 z) {
    
    bool result = FALSE;
    
    if (mapIndex == MAIN_MAP_INDEX && (mapControllers[mapIndex].flags & MAP_CONTROLLER_INITIALIZED) && (mapControllers[mapIndex].flags & MAP_CONTROLLER_ASSETS_LOADED)) {
    
        mapControllers[mapIndex].viewPosition.x += x;
        mapControllers[mapIndex].viewPosition.y += y;
        mapControllers[mapIndex].viewPosition.z += z;

        result = TRUE;
    
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/mapContext", setMapControllerRGBA);

bool setMapControllerRGBA(u16 mapIndex, u8 r, u8 g, u8 b, u8 a) {
    
    bool result = FALSE;
    
    if (mapIndex == MAIN_MAP_INDEX && (mapControllers[mapIndex].flags & MAP_CONTROLLER_INITIALIZED) && (mapControllers[mapIndex].flags & MAP_CONTROLLER_ASSETS_LOADED)) {
    
        setMapRGBA(mapControllers[mapIndex].mainMapIndex, r, g, b, a);
    
        currentMapLightingRGBA.r = r;
        currentMapLightingRGBA.g = g;
        currentMapLightingRGBA.b = b;
        currentMapLightingRGBA.a = a;

        result = TRUE;
    
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/mapContext", setMapControllerRGBAWithTransition);

bool setMapControllerRGBAWithTransition(u16 mapIndex, u8 r, u8 g, u8 b, u8 a, s16 rate) {
    
    bool result = FALSE;
    
    if (mapIndex == MAIN_MAP_INDEX && (mapControllers[mapIndex].flags & MAP_CONTROLLER_INITIALIZED) && (mapControllers[mapIndex].flags & MAP_CONTROLLER_ASSETS_LOADED)) {

        setMapRGBAWithTransition(mapControllers[mapIndex].mainMapIndex, r, g, b, a, rate);

        targetMapLightingRGBA.r = r;
        targetMapLightingRGBA.g = g;
        targetMapLightingRGBA.b = b;
        targetMapLightingRGBA.a = a;
        
        D_8017045A = rate;
        
        result = TRUE;

    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/mapContext", setInitialMapRotation);

bool setInitialMapRotation(u16 mapIndex, u8 rotationIndex) {
    
    bool result;

    f32 buffer[8];
    f32 *ptr;
    f32 tempf;
    
    memcpy(buffer, yRotationAngles, 32);

    result = FALSE;

    if (mapIndex == MAIN_MAP_INDEX && mapControllers[mapIndex].flags & MAP_CONTROLLER_INITIALIZED && mapControllers[mapIndex].flags & MAP_CONTROLLER_ASSETS_LOADED) {

        if (!(mapControllers[mapIndex].flags & (MAP_CONTROLLER_ROTATING_COUNTERCLOCKWISE | MAP_CONTROLLER_ROTATING_CLOCKWISE))) {
        
            mapControllers[mapIndex].rotation = rotationIndex;
            tempf = buffer[rotationIndex];
        
            setMapRotation(mapControllers[mapIndex].mainMapIndex, 45.0f, tempf, 0);
            setInitialWorldRotationAngles(45.0f, tempf, 0);
         
            result = TRUE;

        }
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/mapContext", getCurrentMapRotation);

u8 getCurrentMapRotation(u16 mapIndex) {

    // FIXME: shouldn't be necessary
    u32 padding[3];
    
    u8 rotation = 0;

    if (mapIndex == MAIN_MAP_INDEX && (mapControllers[mapIndex].flags & MAP_CONTROLLER_INITIALIZED)) {
        rotation = mapControllers[mapIndex].rotation;
    }

    return rotation;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/mapContext", setMapViewPositionAndCurrentTile);

bool setMapViewPositionAndCurrentTile(u16 mapIndex, f32 x, f32 y, f32 z, u8 arg4, u8 arg5) {

    bool result = FALSE;

    if (mapIndex == MAIN_MAP_INDEX && (mapControllers[mapIndex].flags & MAP_CONTROLLER_INITIALIZED) && (mapControllers[mapIndex].flags & MAP_CONTROLLER_ASSETS_LOADED)) {
        
        mapControllers[mapIndex].currentTileX = arg4;
        mapControllers[mapIndex].currentTileZ = arg5;

        mapControllers[mapIndex].viewPosition.x = x;
        mapControllers[mapIndex].viewPosition.y = y;
        mapControllers[mapIndex].viewPosition.z = z;

        updateMapViewport(&mapControllers[mapIndex]);

        result = TRUE;

    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/mapContext", setMapBoundaries);

bool setMapBoundaries(u16 mapIndex, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8) {

    bool result = FALSE;
    
    if (mapIndex == MAIN_MAP_INDEX && (mapControllers[mapIndex].flags & MAP_CONTROLLER_INITIALIZED)) {

        mapControllers[mapIndex].viewBoundsCorner0.x = arg1;
        mapControllers[mapIndex].viewBoundsCorner0.z = arg2;

        mapControllers[mapIndex].viewBoundsCorner1.x = arg3;
        mapControllers[mapIndex].viewBoundsCorner1.z = arg4;

        mapControllers[mapIndex].viewBoundsCorner2.x = arg5;
        mapControllers[mapIndex].viewBoundsCorner2.z = arg6;

        mapControllers[mapIndex].viewBoundsCorner3.x = arg7;
        mapControllers[mapIndex].viewBoundsCorner3.z = arg8;
        
        result = TRUE;
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/mapContext", getClampedMapX);

// unused or inline
f32 getClampedMapX(u16 mapIndex, f32 x) {

    f32 result = 0.0f;

    if (mapIndex == MAIN_MAP_INDEX && (mapControllers[mapIndex].flags & MAP_CONTROLLER_INITIALIZED)) { 

        if (mapControllers[mapIndex].viewBoundsCorner0.x < x && mapControllers[mapIndex].viewBoundsCorner3.x < x) {
            
            if (!(x < mapControllers[mapIndex].viewBoundsCorner1.x) || !(x < mapControllers[mapIndex].viewBoundsCorner2.x)) {
                result = mapControllers[mapIndex].viewBoundsCorner1.x;
            } 
            
        } else {
            result = mapControllers[mapIndex].viewBoundsCorner0.x;
        }
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/mapContext", getClampedMapZ);

// unused or inline
f32 getClampedMapZ(u16 mapIndex, f32 z) {

    f32 result = 0.0f;
    
    if (mapIndex == MAIN_MAP_INDEX && mapControllers[mapIndex].flags & MAP_CONTROLLER_INITIALIZED) {

        if (z < mapControllers[mapIndex].viewBoundsCorner0.z && z < mapControllers[mapIndex].viewBoundsCorner1.z) {

            if (!(z > mapControllers[mapIndex].viewBoundsCorner2.z) || !(z > mapControllers[mapIndex].viewBoundsCorner3.z)) {
                result = mapControllers[mapIndex].viewBoundsCorner2.z;
            }
            
        } else {
            result = mapControllers[mapIndex].viewBoundsCorner0.z;
        }
    } 

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/mapContext", shutdownMapController);

// unused or inlne
bool shutdownMapController(u16 mapIndex) {

    bool result = FALSE;

    if (mapIndex == MAIN_MAP_INDEX && (mapControllers[mapIndex].flags & MAP_CONTROLLER_INITIALIZED)) {
        deactivateMapSprites(mapControllers[mapIndex].mainMapIndex);
        mapControllers[mapIndex].flags = 0;
        result = TRUE;
    }

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/mapContext", unloadMapAssets);

bool unloadMapAssets(u16 mapIndex) {

    bool result = FALSE;

    if (mapIndex == MAIN_MAP_INDEX && (mapControllers[mapIndex].flags & MAP_CONTROLLER_INITIALIZED) && (mapControllers[mapIndex].flags & MAP_CONTROLLER_ACTIVE)) {
        deactivateMapSprites(mapControllers[mapIndex].mainMapIndex);
        mapControllers[mapIndex].flags &= ~(2 | 4);
        result = TRUE;
    }

    return result;

}


//INCLUDE_ASM("asm/nonmatchings/system/mapContext", deactivateAllMapControllers);

void deactivateAllMapControllers(void) {

    u16 i;

    for (i = 0; i < MAX_MAPS; i++) {
        mapControllers[i].flags &= ~( 2 | 4);        
    }    

}

//INCLUDE_ASM("asm/nonmatchings/system/mapContext", startMapRotation);

bool startMapRotation(u16 mapIndex, u8 arg1, u8 targetRotation) {

    bool result = FALSE;
    u16 indexTemp;

    if (mapIndex == MAIN_MAP_INDEX && (mapControllers[mapIndex].flags & MAP_CONTROLLER_INITIALIZED) && (mapControllers[mapIndex].flags & MAP_CONTROLLER_ACTIVE)) { 

        // FIXME: this fixes regswap issue
        indexTemp = mapIndex;

        if (!(mapControllers[mapIndex].flags & (MAP_CONTROLLER_ROTATING_COUNTERCLOCKWISE | MAP_CONTROLLER_ROTATING_CLOCKWISE)) && targetRotation != mapControllers[mapIndex].rotation) {
            
            if (arg1 == 0xFF) {

                if (mapControllers[mapIndex].rotation < 4) {

                    if ((mapControllers[mapIndex].rotation + 4) >= targetRotation && targetRotation >= mapControllers[mapIndex].rotation) {
                        mapControllers[mapIndex].flags |= MAP_CONTROLLER_ROTATING_CLOCKWISE;
                    } else {
                        mapControllers[indexTemp].flags |= MAP_CONTROLLER_ROTATING_COUNTERCLOCKWISE;
                    }
                    
                } else {

                    if (targetRotation < mapControllers[mapIndex].rotation) {

                        if ((mapControllers[mapIndex].rotation - 4) >= targetRotation) { 
                            mapControllers[mapIndex].flags |= MAP_CONTROLLER_ROTATING_CLOCKWISE;
                        } else {
                            mapControllers[mapIndex].flags |= MAP_CONTROLLER_ROTATING_COUNTERCLOCKWISE;
                        }
                        
                    } else {
                        mapControllers[mapIndex].flags |= MAP_CONTROLLER_ROTATING_CLOCKWISE;
                    }
                    
                }
                
            } else {
                if (!arg1) {
                    mapControllers[mapIndex].flags |= MAP_CONTROLLER_ROTATING_COUNTERCLOCKWISE;
                } else {
                    mapControllers[mapIndex].flags |= MAP_CONTROLLER_ROTATING_CLOCKWISE;
                }
            }

            result = TRUE;
            
            mapControllers[indexTemp].targetRotation = targetRotation;
            mapControllers[indexTemp].rotationFrameCounter = 0;
            
        }

    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/mapContext", updateMapController);

void updateMapController(void) {

    u16 i;

    for (i = 0; i < MAX_MAPS; i++) {

        if ((mapControllers[i].flags & MAP_CONTROLLER_INITIALIZED) && (mapControllers[i].flags & MAP_CONTROLLER_ACTIVE)) {
            
            updateGroundObjects(mapControllers[i].mainMapIndex);
            updateMapViewport(&mapControllers[i]);
            
            if (mapControllers[i].flags & (MAP_CONTROLLER_ROTATING_COUNTERCLOCKWISE | MAP_CONTROLLER_ROTATING_CLOCKWISE)) {
                handleMapRotation(i);
            }

            currentMapLightingRGBA.r = mainMap[mapControllers[i].mainMapIndex].mapGlobals.currentRGBA.r;
            currentMapLightingRGBA.g = mainMap[mapControllers[i].mainMapIndex].mapGlobals.currentRGBA.g;
            currentMapLightingRGBA.b = mainMap[mapControllers[i].mainMapIndex].mapGlobals.currentRGBA.b;
            currentMapLightingRGBA.a = mainMap[mapControllers[i].mainMapIndex].mapGlobals.currentRGBA.a;
        
        }   

    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/mapContext", updateMapViewport);

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

//INCLUDE_ASM("asm/nonmatchings/system/mapController", handleMapRotation);

void handleMapRotation(u16 mapIndex) {

    if (mapControllers[mapIndex].flags & MAP_CONTROLLER_ROTATING_COUNTERCLOCKWISE) {
        adjustMapRotation(mapControllers[mapIndex].mainMapIndex, 0, -1.0f, 0);
        adjustCurrentWorldRotationAngles(0, -1.0f, 0);
    } else {
        adjustMapRotation(mapControllers[mapIndex].mainMapIndex, 0, 1.0f, 0);
        adjustCurrentWorldRotationAngles(0, 1.0f, 0);
    } 

    mapControllers[mapIndex].rotationFrameCounter++;

    if (mapControllers[mapIndex].rotationFrameCounter == 22) {
        if (mapControllers[mapIndex].flags & MAP_CONTROLLER_ROTATING_COUNTERCLOCKWISE) {
            rotateAllEntities(1);
        } else {
            rotateAllEntities(-1);
        }
    }

    if (mapControllers[mapIndex].rotationFrameCounter >= 45) { 

        if (mapControllers[mapIndex].flags & MAP_CONTROLLER_ROTATING_COUNTERCLOCKWISE) { 
            mapControllers[mapIndex].rotation = (mapControllers[mapIndex].rotation + 7) % 8; 
        } else {
            mapControllers[mapIndex].rotation = (mapControllers[mapIndex].rotation + 1) % 8;
        }

        mapControllers[mapIndex].flags &= ~( 8 | MAP_CONTROLLER_ROTATING_CLOCKWISE);

        if (mapControllers[mapIndex].targetRotation == 0xFF || mapControllers[mapIndex].rotation == mapControllers[mapIndex].targetRotation) {
            previousWorldRotationAngles.x = currentWorldRotationAngles.x;
            previousWorldRotationAngles.y = currentWorldRotationAngles.y;
            previousWorldRotationAngles.z = currentWorldRotationAngles.z;
        } else {
            startMapRotation(mapIndex, 0xFF, mapControllers[mapIndex].targetRotation);
        }
        
    }
    
}

//INCLUDE_RODATA("asm/nonmatchings/systemmapController", yRotationAngles);

// y rotation values
static const f32 yRotationAngles[] = { 0, 45.0f, 90.0f, 135.0f, 180.0f, 225.0f, 270.0f, 315.0f };