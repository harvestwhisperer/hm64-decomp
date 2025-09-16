#include "common.h"

#include "system/mapController.h"

#include "system/entity.h"
#include "system/graphic.h"
#include "system/map.h"
#include "system/sceneGraph.h"

// bss
extern Vec4f D_8013D248;
extern Vec4f D_802373F8;

// RGBA update rate
extern s16 D_8017045A;

extern MapController mapControllers[1];
extern MapModelAddresses gMapModelAddresses[96];

// forward declarations
void func_8003C8D4(MapController*); 
 
// y rotations for maps
static const f32 D_8011EDE0[];
                    
//INCLUDE_ASM("asm/nonmatchings/system/mapController", initializeMapControllers);

void initializeMapControllers(void) {
    
    u16 i;
    
    D_802373F8.r = 0;
    D_802373F8.g = 0;
    D_802373F8.b = 0;
    D_802373F8.a = 0;
    
    D_8013D248.r = 0;
    D_8013D248.g = 0;
    D_8013D248.b = 0;
    D_8013D248.a = 0;
    
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
        
        mapControllers[i].unk_10.x = 0;
        mapControllers[i].unk_10.y = 0;
        mapControllers[i].unk_10.z = 0;

        mapControllers[i].unk_1C.x = 0;
        mapControllers[i].unk_1C.y = 0;
        mapControllers[i].unk_1C.z = 0;

        mapControllers[i].unk_28.x = 0;
        mapControllers[i].unk_28.y = 0;
        mapControllers[i].unk_28.z = 0;

        mapControllers[i].unk_34.x = 0;
        mapControllers[i].unk_34.y = 0;
        mapControllers[i].unk_34.z = 0;
    
    }
}

//INCLUDE_ASM("asm/nonmatchings/system/mapController", setMapModelAddresses);

bool setMapModelAddresses(u16 mapIndex, void *start, void *end) {
    
    bool result = FALSE;
    
    if (mapIndex < MAX_MODELS) {
        if (!(gMapModelAddresses[mapIndex].flags & 1)) {
            result = TRUE;
            gMapModelAddresses[mapIndex].romStart = start;
            gMapModelAddresses[mapIndex].romEnd = end;
            gMapModelAddresses[mapIndex].flags = 1;
        }
    }

    return result;
}

//INCLUDE_ASM("asm/nonmatchings/system/mapController", setupMapModel);

bool setupMapModel(u16 index, u16 mapIndex, u32 *modelDataIndex) {
    
    bool result;

    result = FALSE;
    
    if (index == MAIN_MAP_INDEX && !(mapControllers[index].flags & 1)) {
        
        result = TRUE;
        
        mapControllers[index].mainMapIndex = mapIndex;
        
        mapControllers[index].modelDataIndex = modelDataIndex;
        
        mapControllers[index].flags = 1;

        mapControllers[index].viewPosition.x = 0;
        mapControllers[index].viewPosition.y = 0;
        mapControllers[index].viewPosition.z = 0;

        mapControllers[index].currentTileX = 0;
        mapControllers[index].currentTileZ = 0;

        mapControllers[index].unk_10.x = 0;
        mapControllers[index].unk_10.y = 0;
        mapControllers[index].unk_10.z = 0;

        mapControllers[index].unk_1C.x = 0;
        mapControllers[index].unk_1C.y = 0;
        mapControllers[index].unk_1C.z = 0;

        mapControllers[index].unk_28.x = 0;
        mapControllers[index].unk_28.y = 0;
        mapControllers[index].unk_28.z = 0;

        mapControllers[index].unk_34.x = 0;
        mapControllers[index].unk_34.y = 0;
        mapControllers[index].unk_34.z = 0;

    }
    
    return result;
}

bool loadMap(u16 index, u16 mapIndex) {

    bool result = FALSE;
    
    if (index == MAIN_MAP_INDEX && (mapControllers[index].flags & 1)) {
        
        // dma map data
        dmaMapAssets(MAIN_MAP_INDEX, mapIndex);
        
        setMapTranslation(mapControllers[index].mainMapIndex, 0.0f, 0.0f, 0.0f);
        setMapScale(mapControllers[index].mainMapIndex, 1.0f, 1.0f, 1.0f);
        setMapRotation(mapControllers[index].mainMapIndex, 45.0f, 0.0f, 0.0f);
        
        mapControllers[index].rotation = 0;
        
        setMapControllerViewPosition(MAIN_MAP_INDEX, 0.0f, 0.0f, 0.0f);
        func_8003C084(MAIN_MAP_INDEX, 0);
        func_8003C1E0(MAIN_MAP_INDEX, 0.0f, 0.0f, 0.0f, 0, 0);
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
    
    u32 *offset1;
    u32 *offset2;
    u32 *terrainQuads;
    u32 *offset4;
    u32 *offset5;
    u32 *offset6;
    u32 *offset7;
    u32 *offset8;
    u32 *offset9;
    u32 *offset10; 
 
    if (mainMapIndex == MAIN_MAP_INDEX && mapControllers[mainMapIndex].flags & 1) {
        
        mapControllers[mainMapIndex].mapIndex = levelMapIndex;
        
        nuPiReadRom(gMapModelAddresses[levelMapIndex].romStart, mapControllers[mainMapIndex].modelDataIndex, gMapModelAddresses[levelMapIndex].romEnd - gMapModelAddresses[levelMapIndex].romStart);
 
        offset1 = getAddress(mapControllers[mainMapIndex].modelDataIndex, 0);
        offset2 = getAddress(mapControllers[mainMapIndex].modelDataIndex, 1);
        terrainQuads = getAddress(mapControllers[mainMapIndex].modelDataIndex, 2);
        offset4 = getAddress(mapControllers[mainMapIndex].modelDataIndex, 3);
        offset5 = getAddress(mapControllers[mainMapIndex].modelDataIndex, 4);
        offset6 = getAddress(mapControllers[mainMapIndex].modelDataIndex, 5);
        offset7 = getAddress(mapControllers[mainMapIndex].modelDataIndex, 6);
        offset8 = getAddress(mapControllers[mainMapIndex].modelDataIndex, 7);
        offset9 = getAddress(mapControllers[mainMapIndex].modelDataIndex, 8);
        offset10 = getAddress(mapControllers[mainMapIndex].modelDataIndex, 9);
        
        mapControllers[mainMapIndex].flags |= 2;
                
        func_80033A90(mapControllers[mainMapIndex].mainMapIndex, 
            offset1, 
            offset2, 
            terrainQuads, 
            offset4, 
            offset5, 
            offset6, 
            offset7, 
            offset8, 
            offset9, 
            offset10
            );
        
        result = TRUE;
    
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/mapController", func_8003BD60);

bool func_8003BD60(u16 mapIndex) {
    
    bool result = FALSE;
    
    if (mapIndex == MAIN_MAP_INDEX && (mapControllers[mapIndex].flags & 1) && (mapControllers[mapIndex].flags & 2)) {
        mapControllers[mapIndex].flags |= 4;
        result = TRUE;
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/mapContext", setMapControllerViewPosition);

bool setMapControllerViewPosition(u16 mapIndex, f32 x, f32 y, f32 z) {
    
    bool result = FALSE;
    
    if (mapIndex == MAIN_MAP_INDEX && (mapControllers[mapIndex].flags & 1) && (mapControllers[mapIndex].flags & 2)) {
        
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
    
    if (mapIndex == MAIN_MAP_INDEX && (mapControllers[mapIndex].flags & 1) && (mapControllers[mapIndex].flags & 2)) {
    
        mapControllers[mapIndex].viewPosition.x += x;
        mapControllers[mapIndex].viewPosition.y += y;
        mapControllers[mapIndex].viewPosition.z += z;

        result = TRUE;
    
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/mapContext", func_8003BE98);

bool func_8003BE98(u16 mapIndex, u8 r, u8 g, u8 b, u8 a) {
    
    bool result = FALSE;
    
    if (mapIndex == MAIN_MAP_INDEX && (mapControllers[mapIndex].flags & 1) && (mapControllers[mapIndex].flags & 2)) {
    
        setMapRGBA(mapControllers[mapIndex].mainMapIndex, r, g, b, a);
    
        D_802373F8.r = r;
        D_802373F8.g = g;
        D_802373F8.b = b;
        D_802373F8.a = a;

        result = TRUE;
    
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/mapContext", func_8003BF7C);

bool func_8003BF7C(u16 mapIndex, u8 r, u8 g, u8 b, u8 a, s16 rate) {
    
    bool result = FALSE;
    
    if (mapIndex == MAIN_MAP_INDEX && (mapControllers[mapIndex].flags & 1) && (mapControllers[mapIndex].flags & 2)) {

        func_80034738(mapControllers[mapIndex].mainMapIndex, r, g, b, a, rate);

        D_8013D248.r = r;
        D_8013D248.g = g;
        D_8013D248.b = b;
        D_8013D248.a = a;
        
        D_8017045A = rate;
        
        result = TRUE;

    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/mapContext", func_8003C084);

bool func_8003C084(u16 mapIndex, u8 rotationIndex) {
    
    bool result;

    f32 buffer[8];
    f32 *ptr;
    f32 tempf;
    
    memcpy(buffer, D_8011EDE0, 32);

    result = FALSE;

    if (mapIndex == MAIN_MAP_INDEX && mapControllers[mapIndex].flags & 1 && mapControllers[mapIndex].flags & 2) {

        if (!(mapControllers[mapIndex].flags & (0x8 | 0x10))) {
        
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

    if (mapIndex == MAIN_MAP_INDEX && (mapControllers[mapIndex].flags & 1)) {
        rotation = mapControllers[mapIndex].rotation;
    }

    return rotation;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/mapContext", func_8003C1E0);

bool func_8003C1E0(u16 mapIndex, f32 x, f32 y, f32 z, u8 arg4, u8 arg5) {

    bool result = FALSE;

    if (mapIndex == MAIN_MAP_INDEX && (mapControllers[mapIndex].flags & 1) && (mapControllers[mapIndex].flags & 2)) {
        
        mapControllers[mapIndex].currentTileX = arg4;
        mapControllers[mapIndex].currentTileZ = arg5;

        mapControllers[mapIndex].viewPosition.x = x;
        mapControllers[mapIndex].viewPosition.y = y;
        mapControllers[mapIndex].viewPosition.z = z;

        func_8003C8D4(&mapControllers[mapIndex]);

        result = TRUE;

    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/mapContext", func_8003C280);

bool func_8003C280(u16 mapIndex, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8) {

    bool result = FALSE;
    
    if (mapIndex == MAIN_MAP_INDEX && (mapControllers[mapIndex].flags & 1)) {

        mapControllers[mapIndex].unk_10.x = arg1;
        mapControllers[mapIndex].unk_10.z = arg2;

        mapControllers[mapIndex].unk_1C.x = arg3;
        mapControllers[mapIndex].unk_1C.z = arg4;

        mapControllers[mapIndex].unk_28.x = arg5;
        mapControllers[mapIndex].unk_28.z = arg6;

        mapControllers[mapIndex].unk_34.x = arg7;
        mapControllers[mapIndex].unk_34.z = arg8;
        
        result = TRUE;
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/mapContext", func_8003C318);

// unused or inline
f32 func_8003C318(u16 mapIndex, f32 x) {

    f32 result = 0.0f;

    if (mapIndex == MAIN_MAP_INDEX && (mapControllers[mapIndex].flags & 1)) { 

        if (mapControllers[mapIndex].unk_10.x < x && mapControllers[mapIndex].unk_34.x < x) {
            
            if (!(x < mapControllers[mapIndex].unk_1C.x) || !(x < mapControllers[mapIndex].unk_28.x)) {
                result = mapControllers[mapIndex].unk_1C.x;
            } 
            
        } else {
            result = mapControllers[mapIndex].unk_10.x;
        }
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/mapContext", func_8003C3E4);


// unused or inline
f32 func_8003C3E4(u16 mapIndex, f32 z) {

    f32 result = 0.0f;
    
    if (mapIndex == MAIN_MAP_INDEX && mapControllers[mapIndex].flags & 1) {

        if (z < mapControllers[mapIndex].unk_10.z && z < mapControllers[mapIndex].unk_1C.z) {

            if (!(z > mapControllers[mapIndex].unk_28.z) || !(z > mapControllers[mapIndex].unk_34.z)) {
                result = mapControllers[mapIndex].unk_28.z;
            }
            
        } else {
            result = mapControllers[mapIndex].unk_10.z;
        }
    } 

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/mapContext", func_8003C4B0);

// unused or inlne
bool func_8003C4B0(u16 mapIndex) {

    bool result = FALSE;

    if (mapIndex == MAIN_MAP_INDEX && (mapControllers[mapIndex].flags & 1)) {
        func_80034090(mapControllers[mapIndex].mainMapIndex);
        mapControllers[mapIndex].flags = 0;
        result = TRUE;
    }

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/mapContext", func_8003C504);

bool func_8003C504(u16 mapIndex) {

    bool result = FALSE;

    if (mapIndex == MAIN_MAP_INDEX && (mapControllers[mapIndex].flags & 1) && (mapControllers[mapIndex].flags & 4)) {
        func_80034090(mapControllers[mapIndex].mainMapIndex);
        mapControllers[mapIndex].flags &= ~(2 | 4);
        result = TRUE;
    }

    return result;

}


//INCLUDE_ASM("asm/nonmatchings/system/mapContext", func_8003C570);

void func_8003C570(void) {

    u16 i;

    for (i = 0; i < MAX_MAPS; i++) {
        mapControllers[i].flags &= ~( 2 | 4);        
    }    

}

//INCLUDE_ASM("asm/nonmatchings/system/mapContext", func_8003C5C0);

bool func_8003C5C0(u16 mapIndex, u8 arg1, u8 targetRotation) {

    bool result = FALSE;
    u16 indexTemp;

    if (mapIndex == MAIN_MAP_INDEX && (mapControllers[mapIndex].flags & 1) && (mapControllers[mapIndex].flags & 4)) { 

        // FIXME: this fixes regswap issue
        indexTemp = mapIndex;

        if (!(mapControllers[mapIndex].flags & (0x8 | 0x10)) && targetRotation != mapControllers[mapIndex].rotation) {
            
            if (arg1 == 0xFF) {

                if (mapControllers[mapIndex].rotation < 4) {

                    if ((mapControllers[mapIndex].rotation + 4) >= targetRotation && targetRotation >= mapControllers[mapIndex].rotation) {
                        mapControllers[mapIndex].flags |= 0x10;
                    } else {
                        mapControllers[indexTemp].flags |= 8;
                    }
                    
                } else {

                    if (targetRotation < mapControllers[mapIndex].rotation) {

                        if ((mapControllers[mapIndex].rotation - 4) >= targetRotation) { 
                            mapControllers[mapIndex].flags |= 0x10;
                        } else {
                            mapControllers[mapIndex].flags |= 8;
                        }
                        
                    } else {
                        mapControllers[mapIndex].flags |= 0x10;
                    }
                    
                }
                
            } else {
                if (!arg1) {
                    mapControllers[mapIndex].flags |= 8;
                } else {
                    mapControllers[mapIndex].flags |= 0x10;
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

        if ((mapControllers[i].flags & 1) && (mapControllers[i].flags & 4)) {
            
            func_80038810(mapControllers[i].mainMapIndex);
            func_8003C8D4(&mapControllers[i]);
            
            if (mapControllers[i].flags & (0x8 | 0x10)) {
                func_8003CB3C(i);
            }

            D_802373F8.r = mainMap[mapControllers[i].mainMapIndex].mapFloats.groundRgba.r;
            D_802373F8.g = mainMap[mapControllers[i].mainMapIndex].mapFloats.groundRgba.g;
            D_802373F8.b = mainMap[mapControllers[i].mainMapIndex].mapFloats.groundRgba.b;
            D_802373F8.a = mainMap[mapControllers[i].mainMapIndex].mapFloats.groundRgba.a;
        
        }   
    }
}

//INCLUDE_ASM("asm/nonmatchings/system/mapContext", func_8003C8D4);

void func_8003C8D4(MapController* mapController) {
    
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
    
    func_800343FC(mapController->mainMapIndex, param1, param2, mapController->currentTileX, mapController->currentTileZ, param3, y, param4, 1);

}

//INCLUDE_ASM("asm/nonmatchings/system/mapController", func_8003CB3C);

void func_8003CB3C(u16 mapIndex) {

    if (mapControllers[mapIndex].flags & 8) {
        adjustMapRotation(mapControllers[mapIndex].mainMapIndex, 0, -1.0f, 0);
        func_80028EF8(0, -1.0f, 0);
    } else {
        adjustMapRotation(mapControllers[mapIndex].mainMapIndex, 0, 1.0f, 0);
        func_80028EF8(0, 1.0f, 0);
    }

    mapControllers[mapIndex].rotationFrameCounter++;

    if (mapControllers[mapIndex].rotationFrameCounter == 22) {
        if (mapControllers[mapIndex].flags & 8) {
            func_8002F770(1);
        } else {
            func_8002F770(-1);
        }
    }

    if (mapControllers[mapIndex].rotationFrameCounter >= 45) { 

        if (mapControllers[mapIndex].flags & 8) { 
            mapControllers[mapIndex].rotation = (mapControllers[mapIndex].rotation + 7) % 8; 
        } else {
            mapControllers[mapIndex].rotation = (mapControllers[mapIndex].rotation + 1) % 8;
        }

        mapControllers[mapIndex].flags &= ~( 8 | 0x10);

        if (mapControllers[mapIndex].targetRotation == 0xFF || mapControllers[mapIndex].rotation == mapControllers[mapIndex].targetRotation) {
            previousWorldRotationAngles.x = currentWorldRotationAngles.x;
            previousWorldRotationAngles.y = currentWorldRotationAngles.y;
            previousWorldRotationAngles.z = currentWorldRotationAngles.z;
        } else {
            func_8003C5C0(mapIndex, 0xFF, mapControllers[mapIndex].targetRotation);
        }
        
    }
}

//INCLUDE_RODATA("asm/nonmatchings/systemmapController", D_8011EDE0);

// y rotation values
static const f32 D_8011EDE0[] = { 0, 45.0f, 90.0f, 135.0f, 180.0f, 225.0f, 270.0f, 315.0f };