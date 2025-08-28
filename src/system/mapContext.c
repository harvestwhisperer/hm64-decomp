#include "common.h"

#include "system/mapContext.h"

#include "system/entity.h"
#include "system/graphic.h"
#include "system/map.h"
#include "system/sceneGraph.h"

// bss
extern Vec4f D_8013D248;
extern Vec4f D_802373F8;

// struct
// flags
extern s16 D_8017045A;

extern LevelMapContext gMapModelContext[1];
extern MapContextAddresses gMapModelAddresses[96];

// forward declarations
void func_8003C8D4(LevelMapContext*); 
 
// y rotations for maps
static const f32 D_8011EDE0[];
                    
//INCLUDE_ASM("asm/nonmatchings/system/mapContext", initializeMapContext);

void initializeMapContext(void) {
    
    u16 i;
    
    D_802373F8.r = 0;
    D_802373F8.g = 0;
    D_802373F8.b = 0;
    D_802373F8.a = 0;
    
    D_8013D248.r = 0;
    D_8013D248.g = 0;
    D_8013D248.b = 0;
    D_8013D248.a = 0;
    
    // flags
    D_8017045A = 0;
    
    for (i = 0; i < MAX_MAPS; i++) {
        
        gMapModelContext[i].flags = 0;
        
        gMapModelContext[i].mainMapIndex = 0;
        
        gMapModelContext[i].unk_4.x = 0;
        gMapModelContext[i].unk_4.y = 0;
        gMapModelContext[i].unk_4.z = 0;

        gMapModelContext[i].rotation = 0;
        
        gMapModelContext[i].unk_44 = 0;
        gMapModelContext[i].unk_45 = 0;
        
        gMapModelContext[i].unk_10.x = 0;
        gMapModelContext[i].unk_10.y = 0;
        gMapModelContext[i].unk_10.z = 0;

        gMapModelContext[i].unk_1C.x = 0;
        gMapModelContext[i].unk_1C.y = 0;
        gMapModelContext[i].unk_1C.z = 0;

        gMapModelContext[i].unk_28.x = 0;
        gMapModelContext[i].unk_28.y = 0;
        gMapModelContext[i].unk_28.z = 0;

        gMapModelContext[i].unk_34.x = 0;
        gMapModelContext[i].unk_34.y = 0;
        gMapModelContext[i].unk_34.z = 0;
    
    }
}

//INCLUDE_ASM("asm/nonmatchings/system/mapContext", setMapModelAddresses);

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

//INCLUDE_ASM("asm/nonmatchings/system/mapContext", setupMapModel);

bool setupMapModel(u16 index, u16 mapIndex, u32 *modelDataIndex) {
    
    bool result;

    result = FALSE;
    
    if (index == MAIN_MAP_INDEX && !(gMapModelContext[index].flags & 1)) {
        
        result = TRUE;
        
        gMapModelContext[index].mainMapIndex = mapIndex;
        
        gMapModelContext[index].modelDataIndex = modelDataIndex;
        
        gMapModelContext[index].flags = 1;

        gMapModelContext[index].unk_4.x = 0;
        gMapModelContext[index].unk_4.y = 0;
        gMapModelContext[index].unk_4.z = 0;

        gMapModelContext[index].unk_44 = 0;
        gMapModelContext[index].unk_45 = 0;

        gMapModelContext[index].unk_10.x = 0;
        gMapModelContext[index].unk_10.y = 0;
        gMapModelContext[index].unk_10.z = 0;

        gMapModelContext[index].unk_1C.x = 0;
        gMapModelContext[index].unk_1C.y = 0;
        gMapModelContext[index].unk_1C.z = 0;

        gMapModelContext[index].unk_28.x = 0;
        gMapModelContext[index].unk_28.y = 0;
        gMapModelContext[index].unk_28.z = 0;

        gMapModelContext[index].unk_34.x = 0;
        gMapModelContext[index].unk_34.y = 0;
        gMapModelContext[index].unk_34.z = 0;

    }
    
    return result;
}

bool loadMap(u16 index, u16 mapIndex) {

    bool result = FALSE;
    
    if (index == MAIN_MAP_INDEX && (gMapModelContext[index].flags & 1)) {
        
        // dma map data
        func_8003BC50(MAIN_MAP_INDEX, mapIndex);
        
        setMapTranslation(gMapModelContext[index].mainMapIndex, 0.0f, 0.0f, 0.0f);
        setMapScale(gMapModelContext[index].mainMapIndex, 1.0f, 1.0f, 1.0f);
        setMapRotation(gMapModelContext[index].mainMapIndex, 45.0f, 0.0f, 0.0f);
        
        gMapModelContext[index].rotation = 0;
        
        func_8003BDA4(MAIN_MAP_INDEX, 0.0f, 0.0f, 0.0f);
        func_8003C084(MAIN_MAP_INDEX, 0);
        func_8003C1E0(MAIN_MAP_INDEX, 0.0f, 0.0f, 0.0f, 0, 0);
        setMapRGBA(gMapModelContext[index].mainMapIndex, 0, 0, 0, 0);
        
        result = TRUE; 

    } 

    return result;

}

static inline u8* getAddress(u32 offsets[], u32 i) {
    return (u8*)offsets + offsets[i];
}

//INCLUDE_ASM("asm/nonmatchings/system/mapContext", func_8003BC50);

bool func_8003BC50(u16 mainMapIndex, u16 levelMapIndex) {

    bool result = FALSE;
    
    u32 *offset1;
    u32 *offset2;
    u32 *offset3;
    u32 *offset4;
    u32 *offset5;
    u32 *offset6;
    u32 *offset7;
    u32 *offset8;
    u32 *offset9;
    u32 *offset10; 
 
    if (mainMapIndex == MAIN_MAP_INDEX && gMapModelContext[mainMapIndex].flags & 1) {
        
        gMapModelContext[mainMapIndex].mapIndex = levelMapIndex;
        
        nuPiReadRom(gMapModelAddresses[levelMapIndex].romStart, gMapModelContext[mainMapIndex].modelDataIndex, gMapModelAddresses[levelMapIndex].romEnd - gMapModelAddresses[levelMapIndex].romStart);
 
        offset1 = getAddress(gMapModelContext[mainMapIndex].modelDataIndex, 0);
        offset2 = getAddress(gMapModelContext[mainMapIndex].modelDataIndex, 1);
        offset3 = getAddress(gMapModelContext[mainMapIndex].modelDataIndex, 2);
        offset4 = getAddress(gMapModelContext[mainMapIndex].modelDataIndex, 3);
        offset5 = getAddress(gMapModelContext[mainMapIndex].modelDataIndex, 4);
        offset6 = getAddress(gMapModelContext[mainMapIndex].modelDataIndex, 5);
        offset7 = getAddress(gMapModelContext[mainMapIndex].modelDataIndex, 6);
        offset8 = getAddress(gMapModelContext[mainMapIndex].modelDataIndex, 7);
        offset9 = getAddress(gMapModelContext[mainMapIndex].modelDataIndex, 8);
        offset10 = getAddress(gMapModelContext[mainMapIndex].modelDataIndex, 9);
        
        gMapModelContext[mainMapIndex].flags |= 2;
                
        func_80033A90(gMapModelContext[mainMapIndex].mainMapIndex, 
            offset1, 
            offset2, 
            offset3, 
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

//INCLUDE_ASM("asm/nonmatchings/system/mapContext", func_8003BD60);

bool func_8003BD60(u16 mapIndex) {
    
    bool result = FALSE;
    
    if (mapIndex == MAIN_MAP_INDEX && (gMapModelContext[mapIndex].flags & 1) && (gMapModelContext[mapIndex].flags & 2)) {
        gMapModelContext[mapIndex].flags |= 4;
        result = TRUE;
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/mapContext", func_8003BDA4);

bool func_8003BDA4(u16 mapIndex, f32 x, f32 y, f32 z) {
    
    bool result = FALSE;
    
    if (mapIndex == MAIN_MAP_INDEX && (gMapModelContext[mapIndex].flags & 1) && (gMapModelContext[mapIndex].flags & 2)) {
        
        gMapModelContext[mapIndex].unk_4.x = x;
        gMapModelContext[mapIndex].unk_4.y = y;
        gMapModelContext[mapIndex].unk_4.z = z;
        
        result = TRUE;

    }
    
    return result;

}


//INCLUDE_ASM("asm/nonmatchings/system/mapContext", func_8003BE0C);

bool func_8003BE0C(u16 mapIndex, f32 x, f32 y, f32 z) {
    
    bool result = FALSE;
    
    if (mapIndex == MAIN_MAP_INDEX && (gMapModelContext[mapIndex].flags & 1) && (gMapModelContext[mapIndex].flags & 2)) {
    
        gMapModelContext[mapIndex].unk_4.x += x;
        gMapModelContext[mapIndex].unk_4.y += y;
        gMapModelContext[mapIndex].unk_4.z += z;

        result = TRUE;
    
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/mapContext", func_8003BE98);

bool func_8003BE98(u16 mapIndex, u8 r, u8 g, u8 b, u8 a) {
    
    bool result = FALSE;
    
    if (mapIndex == MAIN_MAP_INDEX && (gMapModelContext[mapIndex].flags & 1) && (gMapModelContext[mapIndex].flags & 2)) {
    
        setMapRGBA(gMapModelContext[mapIndex].mainMapIndex, r, g, b, a);
    
        D_802373F8.r = r;
        D_802373F8.g = g;
        D_802373F8.b = b;
        D_802373F8.a = a;

        result = TRUE;
    
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/mapContext", func_8003BF7C);

bool func_8003BF7C(u16 mapIndex, u8 r, u8 g, u8 b, u8 a, s16 arg5) {
    
    bool result = FALSE;
    
    if (mapIndex == MAIN_MAP_INDEX && (gMapModelContext[mapIndex].flags & 1) && (gMapModelContext[mapIndex].flags & 2)) {

        func_80034738(gMapModelContext[mapIndex].mainMapIndex, r, g, b, a, arg5);

        D_8013D248.r = r;
        D_8013D248.g = g;
        D_8013D248.b = b;
        D_8013D248.a = a;
        
        D_8017045A = arg5;
        
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

    if (mapIndex == MAIN_MAP_INDEX && gMapModelContext[mapIndex].flags & 1 && gMapModelContext[mapIndex].flags & 2) {

        if (!(gMapModelContext[mapIndex].flags & (0x8 | 0x10))) {
        
            gMapModelContext[mapIndex].rotation = rotationIndex;
            tempf = buffer[rotationIndex];
        
            setMapRotation(gMapModelContext[mapIndex].mainMapIndex, 45.0f, tempf, 0);
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

    if (mapIndex == MAIN_MAP_INDEX && (gMapModelContext[mapIndex].flags & 1)) {
        rotation = gMapModelContext[mapIndex].rotation;
    }

    return rotation;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/mapContext", func_8003C1E0);

bool func_8003C1E0(u16 mapIndex, f32 x, f32 y, f32 z, u8 arg4, u8 arg5) {

    bool result = FALSE;

    if (mapIndex == MAIN_MAP_INDEX && (gMapModelContext[mapIndex].flags & 1) && (gMapModelContext[mapIndex].flags & 2)) {
        
        gMapModelContext[mapIndex].unk_44 = arg4;
        gMapModelContext[mapIndex].unk_45 = arg5;

        gMapModelContext[mapIndex].unk_4.x = x;
        gMapModelContext[mapIndex].unk_4.y = y;
        gMapModelContext[mapIndex].unk_4.z = z;

        func_8003C8D4(&gMapModelContext[mapIndex]);

        result = TRUE;

    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/mapContext", func_8003C280);

bool func_8003C280(u16 mapIndex, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8) {

    bool result = FALSE;
    
    if (mapIndex == MAIN_MAP_INDEX && (gMapModelContext[mapIndex].flags & 1)) {

        gMapModelContext[mapIndex].unk_10.x = arg1;
        gMapModelContext[mapIndex].unk_10.z = arg2;

        gMapModelContext[mapIndex].unk_1C.x = arg3;
        gMapModelContext[mapIndex].unk_1C.z = arg4;

        gMapModelContext[mapIndex].unk_28.x = arg5;
        gMapModelContext[mapIndex].unk_28.z = arg6;

        gMapModelContext[mapIndex].unk_34.x = arg7;
        gMapModelContext[mapIndex].unk_34.z = arg8;
        
        result = TRUE;
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/mapContext", func_8003C318);

f32 func_8003C318(u16 mapIndex, f32 x) {

    f32 result = 0.0f;

    if (mapIndex == MAIN_MAP_INDEX && (gMapModelContext[mapIndex].flags & 1)) { 

        if (gMapModelContext[mapIndex].unk_10.x < x && gMapModelContext[mapIndex].unk_34.x < x) {
            
            if (!(x < gMapModelContext[mapIndex].unk_1C.x) || !(x < gMapModelContext[mapIndex].unk_28.x)) {
                result = gMapModelContext[mapIndex].unk_1C.x;
            } 
            
        } else {
            result = gMapModelContext[mapIndex].unk_10.x;
        }
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/mapContext", func_8003C3E4);

// get z value
f32 func_8003C3E4(u16 mapIndex, f32 z) {

    f32 result = 0.0f;
    
    if (mapIndex == MAIN_MAP_INDEX && gMapModelContext[mapIndex].flags & 1) {

        if (z < gMapModelContext[mapIndex].unk_10.z && z < gMapModelContext[mapIndex].unk_1C.z) {

            if (!(z > gMapModelContext[mapIndex].unk_28.z) || !(z > gMapModelContext[mapIndex].unk_34.z)) {
                result = gMapModelContext[mapIndex].unk_28.z;
            }
            
        } else {
            result = gMapModelContext[mapIndex].unk_10.z;
        }
    } 

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/mapContext", func_8003C4B0);

bool func_8003C4B0(u16 mapIndex) {

    bool result = FALSE;

    if (mapIndex == MAIN_MAP_INDEX && (gMapModelContext[mapIndex].flags & 1)) {
        func_80034090(gMapModelContext[mapIndex].mainMapIndex);
        gMapModelContext[mapIndex].flags = 0;
        result = TRUE;
    }

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/mapContext", func_8003C504);

bool func_8003C504(u16 mapIndex) {

    bool result = FALSE;

    if (mapIndex == MAIN_MAP_INDEX && (gMapModelContext[mapIndex].flags & 1) && (gMapModelContext[mapIndex].flags & 4)) {
        func_80034090(gMapModelContext[mapIndex].mainMapIndex);
        gMapModelContext[mapIndex].flags &= ~(2 | 4);
        result = TRUE;
    }

    return result;

}


//INCLUDE_ASM("asm/nonmatchings/system/mapContext", func_8003C570);

void func_8003C570(void) {

    u16 i = 0;

    do {

        gMapModelContext[i].flags &= ~( 2 | 4);
        i++;
        
    } while (i == 0);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/mapContext", func_8003C5C0);

bool func_8003C5C0(u16 mapIndex, u8 arg1, u8 rotation) {

    bool result = FALSE;
    u16 indexTemp;

    if (mapIndex == MAIN_MAP_INDEX && (gMapModelContext[mapIndex].flags & 1) && (gMapModelContext[mapIndex].flags & 4)) { 

        // FIXME: this fixes regswap issue
        indexTemp = mapIndex;

        if (!(gMapModelContext[mapIndex].flags & (0x8 | 0x10)) && rotation != gMapModelContext[mapIndex].rotation) {
            
            if (arg1 == 0xFF) {

                if (gMapModelContext[mapIndex].rotation < 4) {

                    if ((gMapModelContext[mapIndex].rotation + 4) >= rotation && rotation >= gMapModelContext[mapIndex].rotation) {
                        gMapModelContext[mapIndex].flags |= 0x10;
                    } else {
                        gMapModelContext[indexTemp].flags |= 8;
                    }
                    
                } else {

                    if (rotation < gMapModelContext[mapIndex].rotation) {

                        if ((gMapModelContext[mapIndex].rotation - 4) >= rotation) { 
                            gMapModelContext[mapIndex].flags |= 0x10;
                        } else {
                            gMapModelContext[mapIndex].flags |= 8;
                        }
                        
                    } else {
                        gMapModelContext[mapIndex].flags |= 0x10;
                    }
                    
                }
                
            } else {
                if (!arg1) {
                    gMapModelContext[mapIndex].flags |= 8;
                } else {
                    gMapModelContext[mapIndex].flags |= 0x10;
                }
            }

            result = TRUE;
            
            gMapModelContext[indexTemp].unk_47 = rotation;
            gMapModelContext[indexTemp].unk_46 = 0;
            
        }

    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/mapContext", updateMapContext);

void updateMapContext(void) {

    u16 i;

    for (i = 0; i < MAX_MAPS; i++) {

        if ((gMapModelContext[i].flags & 1) && (gMapModelContext[i].flags & 4)) {
            
            func_80038810(gMapModelContext[i].mainMapIndex);
            func_8003C8D4(&gMapModelContext[i]);
            
            if (gMapModelContext[i].flags & (0x8 | 0x10)) {
                func_8003CB3C(i);
            }

            D_802373F8.r = mainMap[gMapModelContext[i].mainMapIndex].mapFloats.groundRgba.r;
            D_802373F8.g = mainMap[gMapModelContext[i].mainMapIndex].mapFloats.groundRgba.g;
            D_802373F8.b = mainMap[gMapModelContext[i].mainMapIndex].mapFloats.groundRgba.b;
            D_802373F8.a = mainMap[gMapModelContext[i].mainMapIndex].mapFloats.groundRgba.a;
        
        }   
    }
}

//INCLUDE_ASM("asm/nonmatchings/system/mapContext", func_8003C8D4);

void func_8003C8D4(LevelMapContext* mapContext) {
    
    f32 x;
    f32 y;
    f32 z;
    
    u8 param1;
    u8 param2;
    
    f32 param3;
    f32 param4;
    
    f32 temp1;
    f32 temp2;
    
    x = mapContext->unk_4.x + ((mainMap[mapContext->mainMapIndex].mapStruct1.mapWidth * mainMap[mapContext->mainMapIndex].mapStruct1.scalingFactorX) / 2);
    param1 = x / mainMap[mapContext->mainMapIndex].mapStruct1.scalingFactorX;
    
    z = mapContext->unk_4.z + ((mainMap[mapContext->mainMapIndex].mapStruct1.mapHeight * mainMap[mapContext->mainMapIndex].mapStruct1.scalingFactorZ) / 2);
    
    temp1 = param1 * mainMap[mapContext->mainMapIndex].mapStruct1.scalingFactorX;
    
    param3 = x - temp1;
    
    y = mapContext->unk_4.y;
    
    param2 = z / mainMap[mapContext->mainMapIndex].mapStruct1.scalingFactorZ;

    temp2 = param2 * mainMap[mapContext->mainMapIndex].mapStruct1.scalingFactorZ;

    param4 = z - temp2;
    
    func_800343FC(mapContext->mainMapIndex, param1, param2, mapContext->unk_44, mapContext->unk_45, param3, y, param4, 1);

}

//INCLUDE_ASM("asm/nonmatchings/system/mapContext", func_8003CB3C);

void func_8003CB3C(u16 mapIndex) {

    if (gMapModelContext[mapIndex].flags & 8) {
        adjustMapRotation(gMapModelContext[mapIndex].mainMapIndex, 0, -1.0f, 0);
        func_80028EF8(0, -1.0f, 0);
    } else {
        adjustMapRotation(gMapModelContext[mapIndex].mainMapIndex, 0, 1.0f, 0);
        func_80028EF8(0, 1.0f, 0);
    }

    gMapModelContext[mapIndex].unk_46++;

    if (gMapModelContext[mapIndex].unk_46 == 22) {
        if (gMapModelContext[mapIndex].flags & 8) {
            func_8002F770(1);
        } else {
            func_8002F770(-1);
        }
    }

    if (gMapModelContext[mapIndex].unk_46 >= 45) { 

        if (gMapModelContext[mapIndex].flags & 8) { 
            gMapModelContext[mapIndex].rotation = (gMapModelContext[mapIndex].rotation + 7) % 8; 
        } else {
            gMapModelContext[mapIndex].rotation = (gMapModelContext[mapIndex].rotation + 1) % 8;
        }

        gMapModelContext[mapIndex].flags &= ~( 8 | 0x10);

        if (gMapModelContext[mapIndex].unk_47 == 0xFF || gMapModelContext[mapIndex].rotation == gMapModelContext[mapIndex].unk_47) {
            previousWorldRotationAngles.x = currentWorldRotationAngles.x;
            previousWorldRotationAngles.y = currentWorldRotationAngles.y;
            previousWorldRotationAngles.z = currentWorldRotationAngles.z;
        } else {
            func_8003C5C0(mapIndex, 0xFF, gMapModelContext[mapIndex].unk_47);
        }
        
    }
}

//INCLUDE_RODATA("asm/nonmatchings/systemmapContext", D_8011EDE0);

// y rotation values
static const f32 D_8011EDE0[] = { 0, 45.0f, 90.0f, 135.0f, 180.0f, 225.0f, 270.0f, 315.0f };