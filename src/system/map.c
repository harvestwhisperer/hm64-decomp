#include "common.h"

#include "system/map.h"

#include "system/globalSprites.h"
#include "system/graphic.h"
#include "system/math.h"
#include "system/mapController.h"
#include "system/sceneGraph.h"

#include "mainproc.h" 
 
// forward declarations
Vec3f getTerrainHeight(TerrainQuad *quad, f32 x, f32 z, u8 fallbackHeight);
void updateCameraViewBounds(MapCameraView*);
bool func_80035DA4(MainMap *map, u8 arg1, u8 arg2);       
bool func_800360BC(u16, u16, u8, u8, u16);       
Vec3f func_800366F4(u16, f32, f32);                  
inline u16 getTerrainIndexForTile(u16 mapIndex, u8 x, u8 z);
Gfx* func_8003797C(Gfx* dl, MainMap* map, u8 arg2);
u16 func_80036880(u16, f32, f32);  
void func_80036AB4(MainMap*); 
void func_80036C08(u16 mapIndex);
void setMapGrid(MapGrid*, MapGridData*);                 
u8* func_800374C0(UnknownMapStruct2*, u8*);
u8* func_80037614(MapVtx* arg0, VtxInfo* arg1);    
u16* func_80037650(u16 arg0, void *arg1);
u32 func_80037668(MainMap*, u16, f32, f32, f32);
Gfx* func_80037BC4(Gfx*, MainMap*, u16, u8);
s16 func_80037F08(Gfx*, MainMap*, UnknownMapStruct2**);  
Gfx* func_800383B0(Gfx* arg0, MainMap* arg1, u16 arg2, f32 arg3, f32 arg4, f32 arg5);
void func_8003851C(MainMap*);  
void func_80038630(DecompressedVec3f* arg0, CompressedVec3f* arg1);                        
void func_800386C0(UnknownMapStruct* arg0, u8* arg1); 
u8* func_800386D4(u16 arg0, u8* arg1);     
void setTerrainQuad(TerrainQuad *arg0, u8* arg1, u16 arg2);
u8* func_800388A4(u16 arg0, u8 *arg1);
void func_80038BC4(MainMap*); 
void func_800393E0(MainMap*); 
void func_80039990(MainMap*); 
void processMapSceneNode(u16, Gfx*);                 
void func_8003B1BC(MainMap*);         
void func_80039F58(u16);   


// bss
extern MainMap mainMap[1];

extern f32 D_8013D550;
extern f32 D_80170460;
extern DecompressedVec3f D_80181BA0[];
extern u8 D_801FB5CB;
extern f32 D_801FB5D4;
extern u8 D_801FB700;
extern f32 D_801FB5D4;
extern f32 D_802226EC;

extern u8 D_8013D5F0[];
extern Vtx D_80165500[2][320][4];
extern u8 D_8018A090[];
extern u8 D_80181BAC[0x10][0x10];
extern Gfx mapDisplayList[2][6912];
extern Vtx mapVertices[2][2560];
extern u16 D_80204B48[0x50];

// data
extern u8 D_80112D10[27][27];

// rodata
static const Gfx D_8011ED68;
static const Gfx D_8011ED70;
static const Gfx D_8011ED78;
static const Gfx D_8011ED88;
static const Gfx D_8011EDC8;
static const Gfx D_8011ED80;
static const Gfx D_8011ED90;
static const Gfx D_8011EDA8;
static const Gfx D_8011EDD0;
static const Gfx D_8011EDD8;

static const char D_8011EDB0[];
static const char D_8011EDB4[];

static inline u16 swap16Tile(Swap16 halfword) {

    Swap16 swap;
    
    swap.byte[1] = halfword.byte[0];
    swap.byte[0] = halfword.byte[1];

    swap.halfword -= 1;
    
    return swap.halfword;

}

//INCLUDE_ASM("asm/nonmatchings/system/map", initializeMap);

void initializeMap(void) {
 
    u16 i, j; 
     
    D_801FB700 = 0;
    D_801FB5CB = 0;
    
    for (i = 0; i < MAX_MAPS; i++) {
        
        mainMap[i].mapState.flags = 0;
        mainMap[i].mapState.unk_A = 0;
        mainMap[i].mapState.unk_C = 0;
        mainMap[i].mapState.height = 0;

        mainMap[i].mapFloats.translation.x = 0;
        mainMap[i].mapFloats.translation.y = 0;
        mainMap[i].mapFloats.translation.z = 0;
 
        mainMap[i].mapFloats.scale.x = 1.0f;
        mainMap[i].mapFloats.scale.y = 1.0f;
        mainMap[i].mapFloats.scale.z = 1.0f;
 
        mainMap[i].mapFloats.rotation.x = 0;
        mainMap[i].mapFloats.rotation.y = 0;
        mainMap[i].mapFloats.rotation.z = 0;

        mainMap[i].mapFloats.groundRgba.r = 255.0f;
        mainMap[i].mapFloats.groundRgba.g = 255.0f;
        mainMap[i].mapFloats.groundRgba.b = 255.0f;
        mainMap[i].mapFloats.groundRgba.a = 255.0f;

        mainMap[i].mapCameraView.calculatedTileX = 0;
        mainMap[i].mapCameraView.calculatedTileZ = 0;
        mainMap[i].mapCameraView.viewExtentX = 0;
        mainMap[i].mapCameraView.viewExtentZ = 0;

        mainMap[i].mapCameraView.viewOffset.x = 0;
        mainMap[i].mapCameraView.viewOffset.y = 0;
        mainMap[i].mapCameraView.viewOffset.z = 0;

        mainMap[i].mapCameraView.currentPosition.x = 0;
        mainMap[i].mapCameraView.currentPosition.y = 0;
        mainMap[i].mapCameraView.currentPosition.z = 0;

        mainMap[i].mapCameraView.unk_C.x = 0;
        mainMap[i].mapCameraView.unk_C.y = 0;
        mainMap[i].mapCameraView.unk_C.z = 0;
        
        mainMap[i].mapCameraView.unk_18.x = 0;
        mainMap[i].mapCameraView.unk_18.y = 0;
        mainMap[i].mapCameraView.unk_18.z = 0;

        mainMap[i].mapCameraView.unk_24.x = 0;
        mainMap[i].mapCameraView.unk_24.y = 0;
        mainMap[i].mapCameraView.unk_24.z = 0;

        mainMap[i].mapCameraView.rotation.x = 0;
        mainMap[i].mapCameraView.rotation.y = 0;
        mainMap[i].mapCameraView.rotation.z = 0;
  
        for (j = 0; j < 1024; j++) {
            mainMap[i].vtxs[j].currentVtxIndex = 0;
            mainMap[i].vtxs[j].vtxCount = 0;
            mainMap[i].vtxs[j].unk_8 = 0;
            mainMap[i].vtxs[j].unk_9 = 0;
            mainMap[i].vtxs[j].flags = 0;
            mainMap[i].vtxs[j].count = 0;
        }
    
    }

} 

//INCLUDE_ASM("asm/nonmatchings/system/map", func_80033A90);

// FIXME: iterators are messed up; loops are probably inline funcs 
bool func_80033A90(u16 mapIndex, MapGridData* grid, void* arg2, u8* terrainQuads, void* arg4, void* arg5, void* arg6, u16* paletteIndex, void* arg8, void* arg9, void *argA) {

    bool result;

    u16 i, k, l, m, n, o, p, q, j, r;

    u8 centerTileX;
    u8 centerTileZ;

    result = FALSE;

    if (mapIndex == MAIN_MAP_INDEX && !(mainMap[mapIndex].mapState.flags & MAP_ACTIVE)) {
        
        mainMap[mapIndex].mapState.flags = 1;
        mainMap[mapIndex].mapState.unk_A = 0;
        mainMap[mapIndex].mapState.mapObjectCount = 0;
        
        mainMap[mapIndex].unk_0 = arg2;
        mainMap[mapIndex].terrainQuads = (u8*)terrainQuads;
        mainMap[mapIndex].unk_8 = (u8**)arg4;
        mainMap[mapIndex].unk_C = (u8*)arg5;
        mainMap[mapIndex].unk_10 = arg6;
        mainMap[mapIndex].unk_14 = paletteIndex;
        mainMap[mapIndex].unk_18 = arg8;
        mainMap[mapIndex].unk_1C = arg9;
        mainMap[mapIndex].unk_20 = argA;

        for (j = 0; j < 16; j++) {

            mainMap[mapIndex].unknownMapStruct[j].unk_2 = 0; 
            mainMap[mapIndex].unknownMapStruct[j].unk_3 = 0; 
            mainMap[mapIndex].unknownMapStruct[j].unk_0 = 0; 

        }

        for (l = 0; l < 0x100; l++) {
            D_80181BA0[l].flags = 0;
        }

        for (i = 0; i < MAX_MAP_OBJECTS; i++) {
            mainMap[mapIndex].mapObjects[i].flags = 0;
        }

        for (i = 0; i < MAX_GROUND_OBJECTS; i++) {
            mainMap[mapIndex].groundObjects.arr[i] = 0xFFFF;
        }

        for (i = 0; i < MAX_WEATHER_SPRITES; i++) {
            mainMap[mapIndex].weatherSprites[i].flags = 0;
        }

        for (p = 0; p < 480; p++) {
            mainMap[mapIndex].groundObjects.arr2[0][p] = 0;
            mainMap[mapIndex].groundObjects.arr2[1][p] = 0;
            mainMap[mapIndex].groundObjects.arr2[2][p] = 0;
        }

        for (i = 0; i < MAX_MAP_ADDITIONS; i++) {

            for (j = 0; j < 16; j++) {
                mainMap[mapIndex].mapAdditions[i].arr1[j] = 0xFFFF;
                mainMap[mapIndex].mapAdditions[i].arr2[j] = 0;
            }
            
            mainMap[mapIndex].mapAdditions[i].unk_40 = 0;
            mainMap[mapIndex].mapAdditions[i].unk_42 = 0;
            mainMap[mapIndex].mapAdditions[i].flags = 0;
            
        }

        setMapGrid(&mainMap[mapIndex].mapGrid, grid);

        func_80036AB4(&mainMap[mapIndex]);
        func_80036C08(mapIndex);
        func_8003851C(&mainMap[mapIndex]);

        centerTileX = mainMap[mapIndex].mapGrid.mapWidth / 2;
        centerTileZ = mainMap[mapIndex].mapGrid.mapHeight / 2;

        func_800343FC(mapIndex, centerTileX, centerTileZ, centerTileX, centerTileZ, 0.0f, 0.0f, 0.0f, 1);
        updateCameraViewBounds(&mainMap[mapIndex].mapCameraView);

        mainMap[mapIndex].mapState.mapOriginX = ((mainMap[mapIndex].mapGrid.mapWidth * mainMap[mapIndex].mapGrid.tileSizeX) / 2) + (mainMap[mapIndex].mapGrid.tileSizeX / 2);
        mainMap[mapIndex].mapState.mapOriginZ = ((mainMap[mapIndex].mapGrid.mapHeight * mainMap[mapIndex].mapGrid.tileSizeZ) / 2) + (mainMap[mapIndex].mapGrid.tileSizeZ / 2);

        for (r = 0; r < 1596; r++) {
            D_8013D5F0[r] = r % mainMap[mapIndex].mapGrid.mapWidth;
            D_8018A090[r] = r / mainMap[mapIndex].mapGrid.mapWidth;
        }
        
        result = TRUE;
        
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_80034090);

bool func_80034090(u16 mapIndex) {

    bool result = FALSE;

    u8 i;

    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapState.flags & MAP_ACTIVE)) {
        
        D_801FB700 = 0;
        D_801FB5CB = 0;

        for (i = 0; i < MAX_MAP_OBJECTS; i++) {
            
            if (mainMap[mapIndex].mapObjects[i].flags & MAP_OBJECT_ACTIVE) {
                deactivateSprite(mainMap[mapIndex].mapObjects[i].spriteIndex);
            }
            
            mainMap[mapIndex].mapObjects[i].flags = 0;

        }
        
        for (i = 0; i < MAX_WEATHER_SPRITES; i++) { 
            
            if (mainMap[mapIndex].weatherSprites[i].flags & MAP_SPRITE_ACTIVE) {
                deactivateSprite(mainMap[mapIndex].weatherSprites[i].spriteIndex);
            }

            mainMap[mapIndex].weatherSprites[i].flags = 0;

        }
        
        mainMap[mapIndex].mapState.flags = 0;

        result = TRUE;
        
    }

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/map", setMapTranslation);

bool setMapTranslation(u16 mapIndex, f32 x, f32 y, f32 z) {

    bool result = FALSE;
    
    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapState.flags & MAP_ACTIVE)) {

        mainMap[mapIndex].mapFloats.translation.x = x;
        mainMap[mapIndex].mapFloats.translation.y = y;
        mainMap[mapIndex].mapFloats.translation.z = z;

        result = TRUE;

    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_80034298);

bool setMapScale(u16 mapIndex, f32 arg1, f32 arg2, f32 arg3) {

    bool result = FALSE;
    
    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapState.flags & MAP_ACTIVE)) {
        
        mainMap[mapIndex].mapFloats.scale.x = arg1;
        mainMap[mapIndex].mapFloats.scale.y = arg2;
        mainMap[mapIndex].mapFloats.scale.z = arg3;
        
        result = TRUE;

    }
    
    return result;
}

//INCLUDE_ASM("asm/nonmatchings/system/map", setMapRotation);

bool setMapRotation(u16 mapIndex, f32 arg1, f32 arg2, f32 arg3) {

    bool result = FALSE;
    
    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapState.flags & MAP_ACTIVE)) {
        
        mainMap[mapIndex].mapFloats.rotation.x = arg1;
        mainMap[mapIndex].mapFloats.rotation.y = arg2;
        mainMap[mapIndex].mapFloats.rotation.z = arg3;
        
        result = TRUE;

    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/map", setMapRGBA);

bool setMapRGBA(u16 mapIndex, u8 arg1, u8 arg2, u8 arg3, u8 arg4) {
    
    bool result = FALSE;
    
    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapState.flags & MAP_ACTIVE)) {
        
        mainMap[mapIndex].mapFloats.groundRgba.r = arg1;
        mainMap[mapIndex].mapFloats.groundRgba.g = arg2;
        mainMap[mapIndex].mapFloats.groundRgba.b = arg3;
        mainMap[mapIndex].mapFloats.groundRgba.a = arg4;
        
        mainMap[mapIndex].mapFloats.defaultRgba.r = arg1;
        mainMap[mapIndex].mapFloats.defaultRgba.g = arg2;
        mainMap[mapIndex].mapFloats.defaultRgba.b = arg3;
        mainMap[mapIndex].mapFloats.defaultRgba.a = arg4;
    
        result = TRUE; 

    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_800343FC);

bool func_800343FC(u16 mapIndex, u8 arg1, u8 arg2, u8 arg3, u8 arg4, f32 x, f32 y, f32 z, u8 arg8) {

    bool result = FALSE;

    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapState.flags & MAP_ACTIVE)) {

        mainMap[mapIndex].mapCameraView.calculatedTileX = arg1;
        mainMap[mapIndex].mapCameraView.calculatedTileZ = arg2;
        mainMap[mapIndex].mapCameraView.viewExtentX = arg3;
        mainMap[mapIndex].mapCameraView.viewExtentZ = arg4;

        mainMap[mapIndex].mapCameraView.viewOffset.x = -x;
        mainMap[mapIndex].mapCameraView.viewOffset.y = -y;
        mainMap[mapIndex].mapCameraView.viewOffset.z = -z;
        
        if (arg8 == 1) {
            mainMap[mapIndex].mapState.flags |= 4;
        } else {
            mainMap[mapIndex].mapState.flags &= ~4;
        }

        updateCameraViewBounds(&mainMap[mapIndex].mapCameraView);
        
        result = TRUE;

    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", adjustMapTranslation);

bool adjustMapTranslation(u16 mapIndex, f32 x, f32 y, f32 z) {

    bool result = FALSE;
    
    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapState.flags & MAP_ACTIVE)) {
        
        mainMap[mapIndex].mapFloats.translation.x += x;
        mainMap[mapIndex].mapFloats.translation.y += y;
        mainMap[mapIndex].mapFloats.translation.z += z;
        
        result = TRUE;

    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/map", adjustMapScale);

// unused
bool adjustMapScale(u16 mapIndex, f32 x, f32 y, f32 z) {

    bool result = FALSE;
    
    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapState.flags & MAP_ACTIVE)) {
        
        mainMap[mapIndex].mapFloats.scale.x += x;
        mainMap[mapIndex].mapFloats.scale.y += y;
        mainMap[mapIndex].mapFloats.scale.z += z;
        
        result = TRUE;

    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/map", adjustMapRotation);

bool adjustMapRotation(u16 mapIndex, f32 x, f32 y, f32 z) {

    bool result = FALSE;
    
    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapState.flags & MAP_ACTIVE)) {
        
        mainMap[mapIndex].mapFloats.rotation.x += x;
        mainMap[mapIndex].mapFloats.rotation.y += y;
        mainMap[mapIndex].mapFloats.rotation.z += z;
        
        result = TRUE;

    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/map", adjustMapRGBA);

bool adjustMapRGBA(u16 mapIndex, s8 arg1, s8 arg2, s8 arg3, s8 arg4) {

    bool result = FALSE;
    
    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapState.flags & MAP_ACTIVE)) {
        
        mainMap[mapIndex].mapFloats.groundRgba.r += arg1;
        mainMap[mapIndex].mapFloats.groundRgba.g += arg2;
        mainMap[mapIndex].mapFloats.groundRgba.b += arg3;
        mainMap[mapIndex].mapFloats.groundRgba.a += arg4;
        
        result = TRUE;

    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_80034738);

bool func_80034738(u16 mapIndex, u8 r, u8 g, u8 b, u8 a, s16 arg5) {

    bool result;

    f32 tempFloat;

    s16 temp = getAbsoluteValue(arg5);

    result = FALSE;

    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapState.flags & MAP_ACTIVE)) {

        mainMap[mapIndex].mapFloats.defaultRgba.r = r;
        mainMap[mapIndex].mapFloats.defaultRgba.g = g;
        mainMap[mapIndex].mapFloats.defaultRgba.b = b;
        mainMap[mapIndex].mapFloats.defaultRgba.a = a;

        mainMap[mapIndex].mapState.unk_8 = temp;

        mainMap[mapIndex].mapState.flags &= ~8;

        if (mainMap[mapIndex].mapFloats.defaultRgba.r < mainMap[mapIndex].mapFloats.groundRgba.r) {
            tempFloat = mainMap[mapIndex].mapFloats.groundRgba.r - mainMap[mapIndex].mapFloats.defaultRgba.r;
        } else {
            tempFloat = mainMap[mapIndex].mapFloats.defaultRgba.r - mainMap[mapIndex].mapFloats.groundRgba.r;
        }

        mainMap[mapIndex].mapFloats.unk_60.r = (tempFloat * temp) / 255.0f;
        
        if (mainMap[mapIndex].mapFloats.defaultRgba.g < mainMap[mapIndex].mapFloats.groundRgba.g) {
            tempFloat = mainMap[mapIndex].mapFloats.groundRgba.g - mainMap[mapIndex].mapFloats.defaultRgba.g;
        } else {
            tempFloat = mainMap[mapIndex].mapFloats.defaultRgba.g - mainMap[mapIndex].mapFloats.groundRgba.g;
        }

        mainMap[mapIndex].mapFloats.unk_60.g = (tempFloat * temp) / 255.0f;

        if (mainMap[mapIndex].mapFloats.defaultRgba.b < mainMap[mapIndex].mapFloats.groundRgba.b) {
            tempFloat = mainMap[mapIndex].mapFloats.groundRgba.b - mainMap[mapIndex].mapFloats.defaultRgba.b;
        } else {
            tempFloat = mainMap[mapIndex].mapFloats.defaultRgba.b - mainMap[mapIndex].mapFloats.groundRgba.b;
        }

        mainMap[mapIndex].mapFloats.unk_60.b = (tempFloat * temp) / 255.0f;
        
        if (mainMap[mapIndex].mapFloats.defaultRgba.a < mainMap[mapIndex].mapFloats.groundRgba.a) {
            tempFloat = mainMap[mapIndex].mapFloats.groundRgba.a - mainMap[mapIndex].mapFloats.defaultRgba.a;
        } else {
            tempFloat = mainMap[mapIndex].mapFloats.defaultRgba.a - mainMap[mapIndex].mapFloats.groundRgba.a;
        }

        mainMap[mapIndex].mapFloats.unk_60.a = (tempFloat * temp) / 255.0f;

        result = TRUE;

    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_80034A6C);

bool func_80034A6C(u16 mapIndex, s8 arg1, s8 arg2, s8 arg3, u8 arg4, f32 arg5, f32 arg6, f32 arg7) {

    bool result = FALSE; 

    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapState.flags & MAP_ACTIVE)) { 

        mainMap[mapIndex].mapCameraView.calculatedTileX += arg1;

        mainMap[mapIndex].mapCameraView.currentPosition.y = 0;
        mainMap[mapIndex].mapCameraView.unk_C.y = 0;
        mainMap[mapIndex].mapCameraView.unk_18.y = 0;
        mainMap[mapIndex].mapCameraView.unk_24.y = 0;
        
        mainMap[mapIndex].mapCameraView.calculatedTileZ += arg2;
        mainMap[mapIndex].mapCameraView.viewExtentX += arg3;
        mainMap[mapIndex].mapCameraView.viewExtentZ += arg4;

        mainMap[mapIndex].mapCameraView.viewOffset.x += arg5;
        mainMap[mapIndex].mapCameraView.viewOffset.y += arg6;
        mainMap[mapIndex].mapCameraView.viewOffset.z += arg7;

        mainMap[mapIndex].mapCameraView.currentPosition.x += arg1;
        mainMap[mapIndex].mapCameraView.currentPosition.z += arg2;

        mainMap[mapIndex].mapCameraView.unk_C.x += arg1;
        mainMap[mapIndex].mapCameraView.unk_C.z += arg2;

        mainMap[mapIndex].mapCameraView.unk_18.x += arg1;
        mainMap[mapIndex].mapCameraView.unk_18.z += arg2;

        mainMap[mapIndex].mapCameraView.unk_24.x += arg1;
        mainMap[mapIndex].mapCameraView.unk_24.z += arg2;
        
        result = TRUE;
    
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", setMapObject);

// map object sprites, holdable items, overlay sprites
bool setMapObject(u16 mapIndex, u8 index, u16 spriteIndex, u16 arg3, f32 x, f32 y, f32 z, u8 arg7, u8 arg8, u8 arg9, u8 argA) {

    bool result = FALSE; 

    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapState.flags & MAP_ACTIVE)) {
         
        mainMap[mapIndex].mapObjects[index].spriteIndex = spriteIndex; 
        mainMap[mapIndex].mapObjects[index].animationIndex = arg3;

        mainMap[mapIndex].mapObjects[index].coordinates.x = x;
        mainMap[mapIndex].mapObjects[index].coordinates.y = y;
        mainMap[mapIndex].mapObjects[index].coordinates.z = z;

        mainMap[mapIndex].mapObjects[index].unk_10 = arg7;
        mainMap[mapIndex].mapObjects[index].animationMode = arg8;

        mainMap[mapIndex].mapObjects[index].flags |= 1;
        
        // unused flags
        if (arg9) {
            mainMap[mapIndex].mapObjects[index].flags |= 4;
        }

        if (argA) {
            mainMap[mapIndex].mapObjects[index].flags |= RGBA_FINISHED;
        }

        result = TRUE;
        
        mainMap[mapIndex].mapState.mapObjectCount++;

    }
    
    return result;

}
 
//INCLUDE_ASM("asm/nonmatchings/system/map", func_80034D64);

bool func_80034D64(u16 mapIndex, u8 index, u16 spriteIndex, u16 arg3) {

    bool result = FALSE;
 
    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapState.flags & MAP_ACTIVE)) {

        mainMap[mapIndex].weatherSprites[index].spriteIndex = spriteIndex;
        mainMap[mapIndex].weatherSprites[index].animationIndex = arg3;
        mainMap[mapIndex].weatherSprites[index].flags = 1;

        result = TRUE;

    }

    return result; 
        
}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_80034DC8);

bool func_80034DC8(u16 mapIndex, u8 index, u16 arg2) {

    bool result = FALSE; 

    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapState.flags & MAP_ACTIVE)) { 
        
        resetAnimationState(mainMap[mapIndex].mapObjects[index].spriteIndex);
        
        mainMap[mapIndex].mapObjects[index].animationIndex = arg2;
        mainMap[mapIndex].mapObjects[index].flags &= ~2;

        result = TRUE;

    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", deactivateMapObject);

bool deactivateMapObject(u16 mapIndex, u8 index) {

    bool result = FALSE; 

    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapState.flags & MAP_ACTIVE)) { 

        if (mainMap[mapIndex].mapObjects[index].flags & 2) {
        
            deactivateSprite(mainMap[mapIndex].mapObjects[index].spriteIndex);
        
            mainMap[mapIndex].mapObjects[index].flags = 0;
        
            result = TRUE;

        }
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_80034EF0);

// load and set texture for map spawnable sprite
// called by level.c
bool func_80034EF0(u16 mapIndex, u8 arg1, u8 arg2, u32* textureIndex, u32* paletteIndex, u8* spriteToPaletteIndex, u32 romTextureStart, u32 arg7, u32 romAssetsIndexStart, u32 romAssetsIndexEnd, u8 argA) {

    bool result = FALSE;
    
    u32 assetIndex[8];

    u32 offset1;
    u32 offset2;
    u32 offset3;
    u32 offset4;
    u32 offset5;

    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapState.flags & MAP_ACTIVE)) {
        
        mainMap[mapIndex].groundObjects.textureIndex = textureIndex;
        mainMap[mapIndex].groundObjects.paletteIndex = paletteIndex;
        mainMap[mapIndex].groundObjects.spriteToPaletteIndex = spriteToPaletteIndex;

        // D_801FD624 
        mainMap[mapIndex].groundObjects.unk_10 = arg1;
        // D_801C3F35
        mainMap[mapIndex].groundObjects.unk_11 = arg2;

        mainMap[mapIndex].groundObjects.unk_12 = argA;
        
        nuPiReadRom(romAssetsIndexStart, assetIndex, romAssetsIndexEnd - romAssetsIndexStart);
        
        offset1 = assetIndex[0];
        offset2 = assetIndex[1];
        offset3 = assetIndex[2];
        offset4 = assetIndex[3];
        offset5 = assetIndex[4];
        
        nuPiReadRom(romTextureStart + offset1, mainMap[mapIndex].groundObjects.textureIndex, offset2 - offset1);
        nuPiReadRom(romTextureStart + offset2, mainMap[mapIndex].groundObjects.paletteIndex, offset3 - offset2);
        nuPiReadRom(romTextureStart + offset4, mainMap[mapIndex].groundObjects.spriteToPaletteIndex, offset5 - offset4);
        
        result = TRUE;

    }

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_80035004);

bool func_80035004(u16 mapIndex, u16 arg1, u8 arg2, u8 arg3) {
    
    bool result = FALSE;
    
    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapState.flags & MAP_ACTIVE)) {

        // FIXME: likely a multidimensional array
        mainMap[mapIndex].groundObjects.arr2[0][arg3*0x14 + arg2] = arg1;
        result = TRUE;

    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_80035054);

// used for maps with foragable items
bool func_80035054(u16 mapIndex, u16 bitmapIndex, u16 spriteIndex, f32 x, f32 y, f32 z) {

    bool result = FALSE;

    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapState.flags & MAP_ACTIVE)) {
    
        if (bitmapIndex) {
            
            setBitmapFormat(&mainMap[mapIndex].mapBitmaps[bitmapIndex], getTexturePtr(spriteIndex, mainMap[mapIndex].groundObjects.textureIndex), getPalettePtrType2(spriteIndex, mainMap[mapIndex].groundObjects.paletteIndex, mainMap[mapIndex].groundObjects.spriteToPaletteIndex));
              
            mainMap[mapIndex].mapBitmaps[bitmapIndex].unk_1C.x = x; 
            mainMap[mapIndex].mapBitmaps[bitmapIndex].unk_1C.y = y;
            mainMap[mapIndex].mapBitmaps[bitmapIndex].unk_1C.z = z;
            
            result = TRUE;

        }    
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/map", getTerrainHeightAtPosition);

// FIXME: inline function used before it's defined means it doesn't properly inline; also can't use `swap16Tile` directly
static inline u16 getTerrainIndexForTile_static_inline(u16 mapIndex, u8 x, u8 z) {
    return swap16Tile((&mainMap[mapIndex].mapGrid.tileIndices[mainMap[mapIndex].mapGrid.mapWidth * z])[x]);
}

f32 getTerrainHeightAtPosition(u16 mapIndex, f32 x, f32 z) {
    
    Vec3f vec;
    TerrainQuad quad;

    f32 result;

    f32 worldX, worldZ, tileX, tileZ;
    f32 xRemainder, zRemainder;

    u16 terrainIndex;
    f32 tileOriginZ;

    u8 tileIndexX, tileIndexZ;

    result = 0.0f;

    if (mapIndex == MAIN_MAP_INDEX && mainMap[mapIndex].mapState.flags & 1) {

        worldX = x + mainMap[mapIndex].mapState.mapOriginX;
        tileX = worldX / mainMap[mapIndex].mapGrid.tileSizeX;
        
        tileIndexX = (u8)tileX;
        
        worldZ = z + mainMap[mapIndex].mapState.mapOriginZ;
        tileZ = worldZ / mainMap[mapIndex].mapGrid.tileSizeZ;
        
        xRemainder = worldX - tileIndexX * mainMap[mapIndex].mapGrid.tileSizeX;
        
        tileIndexZ = (u8)tileZ; 
        
        tileOriginZ = tileIndexZ * mainMap[mapIndex].mapGrid.tileSizeZ;        

        terrainIndex = getTerrainIndexForTile_static_inline(mapIndex, tileIndexX, tileIndexZ);
        
        zRemainder = worldZ - tileOriginZ;
        
        setTerrainQuad(&quad, mainMap[mapIndex].terrainQuads, terrainIndex);
        
        vec = getTerrainHeight(&quad, xRemainder, zRemainder, mainMap[mapIndex].vtxs[terrainIndex].unk_9);
        
        result = mainMap[mapIndex].vtxs[terrainIndex].unk_8 + vec.y;
        
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", getTerrainHeight);

Vec3f getTerrainHeight(TerrainQuad *quad, f32 x, f32 z, u8 fallbackHeight) {
  
    Plane planeEquation;
    
    u32 padding[6];
    u8 padding2[16];

    Vec3f heightResult;

    f32 calculatedHeight;
    f32 height;

    heightResult.x = 0.0f;
    heightResult.y = 0.0f;
    heightResult.z = 0.0f;

    planeEquation = calculatePlaneEquation( 
        quad->triangle1[0][0], 
        quad->triangle1[0][1], 
        quad->triangle1[0][2], 
        quad->triangle1[1][0], 
        quad->triangle1[1][1], 
        quad->triangle1[1][2], 
        quad->triangle1[2][0], 
        quad->triangle1[2][1], 
        quad->triangle1[2][2]);

    calculatedHeight = getHeightFromPlane(x, z, planeEquation);
    height = calculatedHeight;

    if (isPointInTriangle(x, 
            height, 
            z, 
            quad->triangle1[0][0], 
            quad->triangle1[0][1], 
            quad->triangle1[0][2], 
            quad->triangle1[1][0], 
            quad->triangle1[1][1], 
            quad->triangle1[1][2], 
            quad->triangle1[2][0], 
            quad->triangle1[2][1], 
            quad->triangle1[2][2])) {

        if (calculatedHeight != 0.0f) {
            heightResult.y = calculatedHeight;
        } else {
            heightResult.y = fallbackHeight;            
        }

    } else {

        planeEquation = calculatePlaneEquation( 
            quad->triangle2[0][0], 
            quad->triangle2[0][1], 
            quad->triangle2[0][2], 
            quad->triangle2[1][0], 
            quad->triangle2[1][1], 
            quad->triangle2[1][2], 
            quad->triangle2[2][0], 
            quad->triangle2[2][1], 
            quad->triangle2[2][2]);

        height = getHeightFromPlane(x, z, planeEquation);
        
        if (isPointInTriangle(x, 
                height, 
                z, 
                quad->triangle2[0][0], 
                quad->triangle2[0][1], 
                quad->triangle2[0][2], 
                quad->triangle2[1][0], 
                quad->triangle2[1][1], 
                quad->triangle2[1][2], 
                quad->triangle2[2][0], 
                quad->triangle2[2][1], 
                quad->triangle2[2][2])) {
            
            if (height != 0.0f) {
                heightResult.y = height;
            } else {
                heightResult.y = fallbackHeight;
            }

        } else if (evaluatePlaneEquation(x, height, z, planeEquation) < 0.0f) {
            heightResult.y = quad->baseHeight;
        } else {
            heightResult.y = fallbackHeight;
        }

    }

    return heightResult;

}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_80035914);

bool func_80035914(u16 mapIndex, f32 arg1, f32 arg2) {

    Vec3f vec;
    
    bool result = FALSE;

    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapState.flags & MAP_ACTIVE)) {

        vec = func_800366F4(0, arg1, arg2);

        if (vec.y != 65535.0f) {
            result = func_80036880(MAIN_MAP_INDEX, vec.x, vec.z) != 0;
        }

    }
    
    return result;

}
 
//INCLUDE_ASM("asm/nonmatchings/system/map", getTileCoordinates);

// alternate
/*
Vec3f* getTileCoordinates(Vec3f* arg0, MainMap* map, f32 x, f32 z) {
    
    Vec3f vec;

    vec.x = (x + map->mapState.mapOriginX) / map->mapGrid.tileSizeX;
    vec.y = 0;
    vec.z = (z + map->mapState.mapOriginZ) / map->mapGrid.tileSizeZ;

    *arg0 = vec;
    
    return arg0;
    
}
*/

Vec3f getTileCoordinates(MainMap* map, f32 x, f32 z) {
    
    Vec3f tileCoordinates;

    tileCoordinates.x = (x + map->mapState.mapOriginX) / map->mapGrid.tileSizeX;
    tileCoordinates.y = 0;
    tileCoordinates.z = (z + map->mapState.mapOriginZ) / map->mapGrid.tileSizeZ;

    return tileCoordinates;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", updateCameraViewBounds);

void updateCameraViewBounds(MapCameraView* mapCameraView) {

    Vec3f padding[3];
    
    Vec3f cornerOffset;
    Vec3f rotatedCenterOffset;

    cornerOffset.x = -mapCameraView->viewExtentX;
    cornerOffset.y = 0;
    cornerOffset.z = -mapCameraView->viewExtentZ;

    rotateVector3D(cornerOffset, &mapCameraView->currentPosition, mapCameraView->rotation);

    cornerOffset.x = -mapCameraView->viewExtentX;
    cornerOffset.y = 0;
    cornerOffset.z = mapCameraView->viewExtentZ;

    rotateVector3D(cornerOffset, &mapCameraView->unk_C, mapCameraView->rotation);

    cornerOffset.x = mapCameraView->viewExtentX;
    cornerOffset.y = 0;
    cornerOffset.z = mapCameraView->viewExtentZ;

    rotateVector3D(cornerOffset, &mapCameraView->unk_18, mapCameraView->rotation);

    cornerOffset.x = mapCameraView->viewExtentX;
    cornerOffset.y = 0;
    cornerOffset.z = -mapCameraView->viewExtentZ;

    rotateVector3D(cornerOffset, &mapCameraView->unk_24, mapCameraView->rotation);

    rotatedCenterOffset.x = 0.0f;
    rotatedCenterOffset.y = 0;
    rotatedCenterOffset.z = 0.0f;
    
    rotateVector3D(rotatedCenterOffset, &rotatedCenterOffset, mapCameraView->rotation);
    
    mapCameraView->currentPosition.x += mapCameraView->calculatedTileX + rotatedCenterOffset.x;
    mapCameraView->currentPosition.y = 0;
    mapCameraView->currentPosition.z += mapCameraView->calculatedTileZ + rotatedCenterOffset.z;

    mapCameraView->unk_C.x += mapCameraView->calculatedTileX + rotatedCenterOffset.x;
    mapCameraView->unk_C.y = 0;
    mapCameraView->unk_C.z += mapCameraView->calculatedTileZ + rotatedCenterOffset.z;

    mapCameraView->unk_18.x += mapCameraView->calculatedTileX + rotatedCenterOffset.x;
    mapCameraView->unk_18.y = 0;
    mapCameraView->unk_18.z += mapCameraView->calculatedTileZ + rotatedCenterOffset.z;
    
    mapCameraView->unk_24.x += mapCameraView->calculatedTileX + rotatedCenterOffset.x;
    mapCameraView->unk_24.y = 0;
    mapCameraView->unk_24.z += mapCameraView->calculatedTileZ + rotatedCenterOffset.z;
    
    // edge widths
    D_802226EC = mapCameraView->currentPosition.x - mapCameraView->unk_C.x;
    D_80170460 = mapCameraView->unk_C.x - mapCameraView->unk_18.x;
    D_8013D550 = mapCameraView->unk_18.x - mapCameraView->unk_24.x;
    D_801FB5D4 = mapCameraView->unk_24.x - mapCameraView->currentPosition.x;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_80035DA4);

bool func_80035DA4(MainMap *map, u8 arg1, u8 arg2) {
    
  bool result = FALSE;

  // FIXME: likely macros
  if (-1.0f <= (((map->mapCameraView.currentPosition.z * (map->mapCameraView.unk_C.x - arg1)) + (map->mapCameraView.unk_C.z * (arg1 - map->mapCameraView.currentPosition.x))) + (arg2 * D_802226EC))) {
    if (-1.0f <= (((map->mapCameraView.unk_C.z * (map->mapCameraView.unk_18.x - arg1)) + (map->mapCameraView.unk_18.z * (arg1 - map->mapCameraView.unk_C.x))) + (arg2 * D_80170460))) {
      if (-1.0f <= (((map->mapCameraView.unk_18.z * (map->mapCameraView.unk_24.x - arg1)) + (map->mapCameraView.unk_24.z * (arg1 - map->mapCameraView.unk_18.x))) + (arg2 * D_8013D550))) {
        if (-1.0f <= (((map->mapCameraView.unk_24.z * (map->mapCameraView.currentPosition.x - arg1)) + (map->mapCameraView.currentPosition.z * (arg1 - map->mapCameraView.unk_24.x))) + (arg2 * D_801FB5D4))) {
          result = TRUE;
        }
      }
    }
  }
    
  return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_80035EE0);

// unused
void func_80035EE0(MainMap* mainMap) {

    u8 i, j;
    u8 temp1, temp2, temp3, temp4;
    
    mainMap->mapCameraView.viewExtentX = 13;
    mainMap->mapCameraView.viewExtentZ = 13;

    for (i = 0; i < 38; i++) {

        for (j = 0; j < 42; j++) {
            mainMap->visibilityGrid[i][j] = 0;
        }
        
    }

    if (mainMap->mapCameraView.calculatedTileX < mainMap->mapCameraView.viewExtentX) {
        temp1 = mainMap->mapCameraView.viewExtentX - mainMap->mapCameraView.calculatedTileX;    
    } else {
        temp1 = 0;
    }

    if (mainMap->mapCameraView.calculatedTileZ < mainMap->mapCameraView.viewExtentZ) {
        temp2 = mainMap->mapCameraView.viewExtentZ - mainMap->mapCameraView.calculatedTileZ;
    } else {    
        temp2 = 0;
    }

    if ((mainMap->mapCameraView.calculatedTileX + mainMap->mapCameraView.viewExtentX) >= mainMap->mapGrid.mapWidth) {
        temp3 = mainMap->mapCameraView.viewExtentX + (mainMap->mapGrid.mapWidth - mainMap->mapCameraView.calculatedTileX);
    } else {
        temp3 = (mainMap->mapCameraView.viewExtentX * 2) | 1;
    } 

    if ((mainMap->mapCameraView.calculatedTileZ + mainMap->mapCameraView.viewExtentZ) >= mainMap->mapGrid.mapHeight) {
        temp4 = mainMap->mapCameraView.viewExtentZ + (mainMap->mapGrid.mapHeight - mainMap->mapCameraView.calculatedTileZ);
    } else {
        temp4 = (mainMap->mapCameraView.viewExtentZ * 2) | 1;
    } 

    for (i = temp2; i < temp4; i++) {
        
        for (j = temp1; j < temp3; j++) {
            mainMap->visibilityGrid[(mainMap->mapCameraView.calculatedTileZ - mainMap->mapCameraView.viewExtentZ) + i][(mainMap->mapCameraView.calculatedTileX - mainMap->mapCameraView.viewExtentX) + j] = D_80112D10[i][j];
        } 
    }
        
}

// mapGrid, vertices for map additions (house extensions, hot springs, etc.)
//INCLUDE_ASM("asm/nonmatchings/system/map", func_800360BC);

u8 func_800360BC(u16 mapIndex, u16 arg1, u8 arg2, u8 arg3, u16 arg4) {

    Swap16 swap;
    
    bool result = FALSE;
    
    u8 i, j;
    u8 height, width;
    u8 x, z;
    
    s16 tempValue;
    u16 temp;
    u16 temp2;
    
    u8 *ptr;
    u8 *ptr2;

    if (mapIndex  == MAIN_MAP_INDEX) {

        if (mainMap[mapIndex].mapState.flags & MAP_ACTIVE) {

            ptr = func_800388A4(arg1, mainMap[mapIndex].unk_20);

            x = *ptr++;
            z = *ptr++;
            
            height = *ptr++;
            width = *ptr++;

            if (arg4) {
                x = arg2;
                z = arg3;
            }

            for (i = 0; i < width; i++) {

                for (j = 0; j < height; j++) {

                    swap.byte[0] = *ptr++;
                    swap.byte[1] = *ptr++;

                    temp = (mainMap[mapIndex].mapGrid.mapWidth * (z + i)) + x + j;
                    
                    mainMap[mapIndex].mapGrid.tileIndices[temp] = swap.halfword;

                }
                
            }

            for (i = 0; i < width; i++) {

                for (j = 0; j < height; j++) {

                    swap.byte[0] = *ptr++;
                    
                    ptr2 = mainMap[mapIndex].unk_8 + 1;

                    temp = (mainMap[mapIndex].mapGrid.mapWidth * (z + i)) + x + j;

                    ptr2[temp] = swap.byte[0];

                }
                
            }
            
            result = TRUE;
            
        }
        
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_80036318);

// returns index for interactable object/exit from rodata array per level
u8 func_80036318(u16 mapIndex, f32 x, f32 z) {

    Vec3f vec1;
    Vec3f vec2;
    u8 index = 0;
    u8* ptr;
    
    if (mapIndex == MAIN_MAP_INDEX) {
        
        if (mainMap[mapIndex].mapState.flags & MAP_ACTIVE) {
        
            vec2.x = (x + mainMap[mapIndex].mapState.mapOriginX) / mainMap[mapIndex].mapGrid.tileSizeX; 
            vec2.y = 0;
            vec2.z = (z + mainMap[mapIndex].mapState.mapOriginZ) / mainMap[mapIndex].mapGrid.tileSizeZ;

            vec1 = vec2;

            ptr = mainMap[mapIndex].unk_8 + 1; 

            index = (ptr + (mainMap[mapIndex].mapGrid.mapWidth * (u8)vec1.z))[(u8)vec1.x];
            
        }
        
    }

    return index;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_80036490);

bool func_80036490(u16 mapIndex, u8 arg1, f32 arg2, f32 arg3) {

    Vec3f vec1;
    Vec3f vec2;

    u8 temp1;
    u8 temp2;

    u8 *ptr;
    
    bool result = FALSE;

    if (mapIndex == MAIN_MAP_INDEX && mainMap[mapIndex].mapState.flags & MAP_ACTIVE) {

        vec2.x = ((arg2 + mainMap[mapIndex].mapState.mapOriginX) / mainMap[mapIndex].mapGrid.tileSizeX);
        vec2.y = 0;
        vec2.z = ((arg3 + mainMap[mapIndex].mapState.mapOriginZ) / mainMap[mapIndex].mapGrid.tileSizeZ);

        vec1 = vec2;

        ptr = mainMap[mapIndex].unk_8 + 1;
        
        (ptr + (mainMap[mapIndex].mapGrid.mapWidth * (u8)vec1.z))[(u8)vec1.x] = arg1;

        result = TRUE;
        
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", convertWorldToTileCoordinates);

Vec3f convertWorldToTileCoordinates(u16 mapIndex, f32 x, f32 z) {

    Vec3f tileCoordinates;
    Vec3f calculatedTiles;

    tileCoordinates.x = 0.0f;
    tileCoordinates.z = 0.0f;
    tileCoordinates.y = 65535.0f;

    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapState.flags & MAP_ACTIVE)) {
        
        calculatedTiles.x = (x + mainMap[mapIndex].mapState.mapOriginX) / mainMap[mapIndex].mapGrid.tileSizeX;
        calculatedTiles.y = 0;
        calculatedTiles.z = (z + mainMap[mapIndex].mapState.mapOriginZ) / mainMap[mapIndex].mapGrid.tileSizeZ;

        tileCoordinates = calculatedTiles; 

    }
 
    return tileCoordinates;

}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_800366F4);

Vec3f func_800366F4(u16 mapIndex, f32 arg2, f32 arg3) {

    Vec3f vec;
    Vec3f vec2;

    vec.x = 0.0f;
    vec.y = 0.0f;
    vec.z = 0.0f;

    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapState.flags & MAP_ACTIVE)) {
        
        vec2.x = (arg2 + mainMap[mapIndex].mapState.mapOriginX) / mainMap[mapIndex].mapGrid.tileSizeX;
        vec2.y = 0;
        vec2.z = (arg3 + mainMap[mapIndex].mapState.mapOriginZ) / mainMap[mapIndex].mapGrid.tileSizeZ;
        
        vec = vec2;
        
        if (mainMap[mapIndex].groundObjects.unk_10 < vec.x && mainMap[mapIndex].groundObjects.unk_11 < vec.z && vec.x < (mainMap[mapIndex].groundObjects.unk_10 + 0x14) && vec.z < (mainMap[mapIndex].groundObjects.unk_11 + 0x18)) {
            vec.x -= mainMap[mapIndex].groundObjects.unk_10;
            vec.z -= mainMap[mapIndex].groundObjects.unk_11;
        } else {
            vec.y = 65535.0f;
        }
    }

    return vec;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_80036880);

u16 func_80036880(u16 mapIndex, f32 x, f32 z) {

    u32 padding[4];
    
    u16 result = 0;
    
    if (mapIndex == MAIN_MAP_INDEX && mainMap[mapIndex].mapState.flags & MAP_ACTIVE) {

        result = mainMap[mapIndex].groundObjects.arr2[0][(u8)z*0x14+(u8)x];        
        
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_80036980);

bool func_80036980(u16 mapIndex, u16 arg1, f32 arg2, f32 arg3) {
    
    u32 padding[4];

    bool result = FALSE;
    
    if (mapIndex == MAIN_MAP_INDEX && mainMap[mapIndex].mapState.flags & MAP_ACTIVE) {

        // FIXME: array indexing
        mainMap[mapIndex].groundObjects.arr2[0][(u8)arg3*0x14+(u8)arg2] = arg1;    
        result = TRUE;
        
    }

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/map", checkMapRGBADone);

bool checkMapRGBADone(u16 mapIndex) {

    bool result = FALSE;
     
    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapState.flags & MAP_ACTIVE)) {
        result = (mainMap[mapIndex].mapState.flags >> 3) & 1;
    }

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_80036AB4);

void func_80036AB4(MainMap* map) {

    u16 i, j;
    u16 count;
    u8* ptr;
    u16 temp;

    UnknownMapStruct2 unknownMapStructs[32]; 

    count = 0;
    i = 0;
  
    if (map->mapGrid.vertexCount) {
        
        do {

            ptr = func_80037614(&map->vtxs[i], func_80037650(i, map->unk_0));

            j = 0;
            
            while (1) {

                ptr =  func_800374C0(&unknownMapStructs[j], ptr);
    
                if (unknownMapStructs[j].flags & 0x80) {
                    map->vtxs[i].flags |= 0x80;
                }

                if (unknownMapStructs[j++].flags & 0x10) {
                    break;
                } 

            } 

            map->vtxs[i].currentVtxIndex = count;
            temp = func_80037F08(&map->displayLists[count], map, unknownMapStructs);

            count += temp;
            map->vtxs[i].vtxCount = temp;
            
            i++;
            
        } while (i < map->mapGrid.vertexCount);
    }  

} 

INCLUDE_ASM("asm/nonmatchings/system/map", func_80036C08);

//INCLUDE_ASM("asm/nonmatchings/system/map", func_80036FA0);

void func_80036FA0(u16 mapIndex) {

    u16 arr[0x40];
    
    u16 i, j;
    u8 k, m;
    u16 l;
    
    u16 temp;

    for (i = 0; i < 0x40; i++) {
        mainMap[mapIndex].groundObjects.arr[i] = 0xFFFF;
    }

    for (j = 0; j < 0x1E0; j++) {
        mainMap[mapIndex].groundObjects.arr2[1][j] = 0;
        mainMap[mapIndex].groundObjects.arr2[2][j] = 0;
    }

    for (k = 0; k < 0x40; k++) {
        mainMap[mapIndex].groundObjects.arr[k] = 0xFFFF;
        arr[k] = 0xFFFF;
    }
    
    for (l = 0; l < 0x1E0; l++) {

        if (mainMap[mapIndex].groundObjects.arr2[0][l] && mainMap[mapIndex].groundObjects.arr2[0][l] != 0xFFFF) {

            if (mainMap[mapIndex].groundObjects.arr[mainMap[mapIndex].groundObjects.arr2[0][l]] == 0xFFFF) {

                mainMap[mapIndex].groundObjects.arr[mainMap[mapIndex].groundObjects.arr2[0][l]] = l;
                arr[mainMap[mapIndex].groundObjects.arr2[0][l]] = l;
                mainMap[mapIndex].groundObjects.arr2[1][l] = 0xFFFF;
                
            } else {

                temp = arr[mainMap[mapIndex].groundObjects.arr2[0][l]];
                
                arr[mainMap[mapIndex].groundObjects.arr2[0][l]] = l;
                
                mainMap[mapIndex].groundObjects.arr2[2][temp] = l;
                mainMap[mapIndex].groundObjects.arr2[1][l] = temp;

            }
            
        }
        
    }

    for (m = 0; m < 0x40; m++) {

        if (arr[m] != 0xFFFF) {
            mainMap[mapIndex].groundObjects.arr2[2][arr[m]] = 0xFFFF;
        }
        
    }

}

//INCLUDE_ASM("asm/nonmatchings/system/map", getTerrainIndexForTile);

// used in getTerrainHeightAtPosition
inline u16 getTerrainIndexForTile(u16 mapIndex, u8 x, u8 z) {
    return swap16Tile((&mainMap[mapIndex].mapGrid.tileIndices[mainMap[mapIndex].mapGrid.mapWidth * z])[x]);
}

//INCLUDE_ASM("asm/nonmatchings/system/map", setMapGrid);

void setMapGrid(MapGrid* mapGrid, MapGridData* data) {

    Swap16 swap;
    
    mapGrid->tileSizeX = data->tileSizeX;
    mapGrid->tileSizeZ = data->tileSizeY;
    mapGrid->mapWidth = data->mapWidth;
    mapGrid->mapHeight = data->mapHeight;

    swap.byte[1] = data->unk_8;
    swap.byte[0] = data->unk_9;
    
    mapGrid->unk_4 = swap.halfword;

    swap.byte[1] = data->unk_A;
    swap.byte[0] = data->unk_B;
    
    mapGrid->vertexCount = swap.halfword;

    mapGrid->tileIndices = data + 1;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_80037350);

// unused or inline
u8* func_80037350(UnknownMapStruct2* arg0, u8* arg1) {

    // arg1 = &*(arg1+4);
    // skip header
    arg1 = &arg1[4];
    
    arg0->unk_8[0] = *arg1++;
    arg0->unk_8[1] = *arg1++;
    arg0->unk_8[2] = *arg1++;
    arg0->unk_B = *arg1++;

    return arg1;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_80037388);

// unused or inline
u8* func_80037388(UnknownMapStruct2* arg0, u8* arg1, u8 arg2) {
    
    Swap16 swap;

    arg0->unk_4[0] = *arg1++;
    arg0->unk_8[0] = *arg1++;
    arg0->unk_C[0] = *arg1++;

    swap.byte[1] = *arg1++;
    swap.byte[0] = *arg1++;
    
    arg0->unk_0 = swap.halfword;

    if (arg2) {

        swap.byte[1] = *arg1++;
        swap.byte[0] = *arg1++;
        
        arg0->unk_2 = swap.halfword;
        
    }
    
    return arg1;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_80037400);

inline u8* func_80037400(UnknownMapStruct2* unknownMapStruct, u8* ptr, bool flag) {
    
    Swap16 swap;
    u8 i;
        
    for (i = 0; i < 3; i++) {
        unknownMapStruct->unk_4[i] = *ptr++;
        unknownMapStruct->unk_8[i] = *ptr++;
        unknownMapStruct->unk_C[i] = *ptr++;
    }

    if (flag) {
        unknownMapStruct->unk_7 = *ptr++;
        unknownMapStruct->unk_B = *ptr++;
        unknownMapStruct->unk_F = *ptr++;
    }
    
    swap.byte[1] = *ptr++;
    swap.byte[0] = *ptr++;

    unknownMapStruct->unk_0 = swap.halfword;

    if (flag) {

        swap.byte[1] = *ptr++;
        swap.byte[0] = *ptr++;

        unknownMapStruct->unk_2 = swap.halfword;
        
    }
    
    return ptr;
    
}

static inline u8* unknownMapStructSwap(UnknownMapStruct2* unknownMapStruct, u8* ptr, u8 flag) {

    u16 temp;
    u8 temp1, temp2;
    
    if (flag) {

        temp1 = ptr[1];
        temp2 = ptr[0];

        temp = temp1 << 8 | temp2;

        unknownMapStruct->unk_2 = temp;
        
        ptr += 2;
        
    }
    
    return ptr;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_800374C0);

u8* func_800374C0(UnknownMapStruct2* unknownMapStruct, u8* ptr) {

    Swap16 swap;
    u32 padding[2];
    
    u8 i;
    u8 flags, flags2;
    u8* ptr2;
    
    unknownMapStruct->flags = *ptr++;
    
    if (unknownMapStruct->flags & 0x20) {
        unknownMapStruct->unk_11 = *ptr++;
        unknownMapStruct->unk_12 = *ptr++;
    }

    flags = unknownMapStruct->flags;

    if (unknownMapStruct->flags & 0x80) {

        ptr = func_80037400(unknownMapStruct, ptr, flags & 0x40);
        
    } else {

        unknownMapStruct->unk_4[0] = ptr[0];
        unknownMapStruct->unk_8[0] = ptr[1];
        unknownMapStruct->unk_C[0] = ptr[2];

        swap.byte[1] = ptr[3];
        swap.byte[0] = ptr[4];

        unknownMapStruct->unk_0 = swap.halfword;
        
        ptr = unknownMapStructSwap(unknownMapStruct, ptr + 5, flags & 0x40);
                
    }

    return ptr;
    
}


//INCLUDE_ASM("asm/nonmatchings/system/map", func_80037614);

// param 2 = 80255A10 + (n * 0x28)
u8* func_80037614(MapVtx* arg0, VtxInfo* arg1) {

    u8 *ptr;
    
    arg0->unk_8 = arg1->unk_4;
    arg0->unk_9 = arg1->unk_5;
    arg0->flags = arg1->flags;
    arg0->count = arg1->count;

    arg1++;
    ptr = arg1;
    
    arg0->vtx = ptr;
    
    return (ptr + arg0->count*3); 
    
} 

//INCLUDE_ASM("asm/nonmatchings/system/map", func_80037650);

// arg0 = i
// arg1 = map.unk_0, u16*
// get ptr to vertex value from offset table 
// arg2 = 802551A8, u32 array of offsets
// vtx = &802551A8 + 802551A8[offset]
u16* func_80037650(u16 arg0, void* arg1) {

    u32* ptr = (u32*)arg1;
    u32 offset = ptr[arg0];
    
    return (u16*)(arg1+offset);

}

// alternate
/*
s32 func_80037650(u16 arg0, s32 arg1) {
    return arg1 + *(s32*)(arg0*4 + arg1);
}
*/

// alternate
/*
u16* func_80037650(u16 arg0, void *arg1) {
    // should be: return (u16*)&(u32*)arg1[arg0];
    return (u16*)(arg1 + *(u32*)(arg1 + arg0*4));
}
*/

//INCLUDE_ASM("asm/nonmatchings/system/map", func_80037668);

u32 func_80037668(MainMap* map, u16 vtxIndex, f32 arg2, f32 arg3, f32 arg4) {

    u8 count = 0;
    u16 count2;
    s8* vtx;

    vtx = map->vtxs[vtxIndex].vtx;

    count2 = map->mapState.unk_C + map->mapState.unk_A; 
    
    if (map->vtxs[vtxIndex].count) {
        
        do {
            
            mapVertices[gGraphicsBufferIndex][count2].v.ob[0] = *vtx + arg2;
            vtx++;
            // FIXME
            mapVertices[gGraphicsBufferIndex][count2].v.ob[1] = *(u8*)vtx + arg3;
            vtx++;
            mapVertices[gGraphicsBufferIndex][count2].v.ob[2] = *vtx + arg4;
            vtx++;

            mapVertices[gGraphicsBufferIndex][count2].v.cn[0] = map->mapFloats.groundRgba.r;
            mapVertices[gGraphicsBufferIndex][count2].v.cn[1] = map->mapFloats.groundRgba.g;
            mapVertices[gGraphicsBufferIndex][count2].v.cn[2] = map->mapFloats.groundRgba.b;
            mapVertices[gGraphicsBufferIndex][count2].v.cn[3] = map->mapFloats.groundRgba.a;
            
            count2++;
            count++;
            
        } while (count < map->vtxs[vtxIndex].count);
    }

    return count;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_8003797C);

Gfx* func_8003797C(Gfx* dl, MainMap* map, u8 arg2) {

    Tile bitmap;

    setBitmapFormat((BitmapObject*)&bitmap, getTexturePtr(arg2, map->unk_10), getPalettePtrType1(arg2, map->unk_14));

    gDPLoadTextureTile_4b(dl++, bitmap.timg, bitmap.fmt, bitmap.width, bitmap.height, 0, 0, bitmap.width - 1, bitmap.height - 1, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
    gDPLoadTLUT_pal16(dl++, 0, bitmap.pal);

    return dl++;

}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_80037BC4);

Gfx* func_80037BC4(Gfx* dl, MainMap* mainMap, u16 arg2, u8 arg3) {

    Swap16 swap;
    
    u16 temp;
    u16 temp2;
    
    do {

        if (func_80035DA4(mainMap, D_8013D5F0[arg2], D_8018A090[arg2])) {

            mainMap->visibilityGrid[D_8018A090[arg2]][D_8013D5F0[arg2]] = 1;
            
            if (arg3 != 0x50) {
                dl = func_8003797C(dl, mainMap, arg3);
                arg3 = 0x50;
            }

            // FIXME: should be inline swap16Tile?
            swap.byte[1] = mainMap->mapGrid.tileIndices[arg2] >> 8;
            swap.byte[0] = mainMap->mapGrid.tileIndices[arg2];
        
            temp = swap.halfword;

            temp2 = temp - 1;
            
            dl = func_800383B0(dl, 
                mainMap,
                temp2, 
                (D_8013D5F0[arg2] - mainMap->mapCameraView.calculatedTileX) * mainMap->mapGrid.tileSizeX,
                mainMap->vtxs[temp2].unk_8,
                (D_8018A090[arg2] - mainMap->mapCameraView.calculatedTileZ) * mainMap->mapGrid.tileSizeZ);
        
        } else {
            mainMap->visibilityGrid[D_8018A090[arg2]][D_8013D5F0[arg2]] = 0;
        }

        arg2 = mainMap->unk_30D2[arg2];
        
    } while (arg2 != 0xFFFF);

    return dl;
    
}


//INCLUDE_ASM("asm/nonmatchings/system/map", func_80037DF0);

inline Gfx* func_80037DF0(Gfx* dl, MainMap* map, u16 arg2) {

    u8 i;
    u16 temp;
    
    map->mapState.unk_A = 0;
    map->mapState.unk_C = arg2;

    *dl++ = D_8011ED68; 
    *dl++ = D_8011ED70; 
    *dl++ = D_8011ED78; 

    for (i = 0; i < 0x51; i++) {
  
        temp = map->unkArr[i];
        
        if (temp != 0xFFFF) {
            dl = func_80037BC4(dl, map, temp, i);
        }

    } 

    *dl++ = D_8011ED80;

    map->mapState.height = map->mapState.unk_A;
    
    return dl;
    
} 

// vertex coloring
INCLUDE_ASM("asm/nonmatchings/system/map", func_80037F08);

//INCLUDE_ASM("asm/nonmatchings/system/map", func_800383B0);

Gfx* func_800383B0(Gfx* dl, MainMap* map, u16 vtxIndex, f32 arg3, f32 arg4, f32 arg5) {

    u16 i;
    
    u32 count;
    
    Gfx* tempDl;
    Gfx tempDl2;

    // FIXME: shouldn't be necessary
    f32 temp;
    u32 padding[2];

    temp = arg4;

    // FIXME:
    // ??
    count = func_80037668(map, vtxIndex, arg3, *(f32*)&arg4, arg5);

    // FIXME: might be a wrapper around gSPVertex
    gSPVertex(&tempDl2 + 1, &mapVertices[gGraphicsBufferIndex][map->mapState.unk_C + map->mapState.unk_A], map->vtxs[vtxIndex].count, 0);

    tempDl2 = *(&tempDl2 + 1);
    *dl++ = tempDl2;
    
    map->mapState.unk_A += count;

    tempDl = &map->displayLists[map->vtxs[vtxIndex].currentVtxIndex];
    
    for (i = 0; i < map->vtxs[vtxIndex].vtxCount; i++) {
        *dl++ = *tempDl++;
    }

    return dl++;
    
}

void func_80038514(void) {}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_8003851C);

void func_8003851C(MainMap* map) {

    u16 i, j;
    u8 temp;
    u16 count = 0;
    u8 *ptr;
    
    count = 0;
    i = 0;
    
    temp = *(map->unk_C+4);
    map->mapState.unk_10 = temp;
    
    if (temp) {
        
        do {

            ptr = map->unk_C+4;

            ptr = func_800386D4(i, ptr);

            func_800386C0(&map->unknownMapStruct[i].unk_0, ptr);

            map->unknownMapStruct[i].unk_0 = count;
            j = 0;
            
            ptr += 2;
            
            if (map->unknownMapStruct[i].unk_3) {
                do {
                    func_80038630(&D_80181BA0[count], ptr);
                    ptr += 7;
                    count++;
                    j++;
                }  while (j < map->unknownMapStruct[i].unk_3);
            }
            
            i++;
            
        } while (i < map->mapState.unk_10);
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_80038630);

// byteswapped s16s converted to floats
void func_80038630(DecompressedVec3f* arg0, CompressedVec3f* arg1) {

    // FIXME: shouldn't be necessary/likely a union for byte-swapping
    u32 padding[8];
    
    arg0->flags = arg1->flags;
    
    arg0->x = (s16)(arg1->part1 | (arg1->part2 << 8));
    arg0->y = (s16)(arg1->part3 | (arg1->part4 << 8));
    arg0->z = (s16)(arg1->part5 | (arg1->part6 << 8));

}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_800386C0);

void func_800386C0(UnknownMapStruct* arg0, u8* arg1) {
    arg0->unk_2 = arg1[0];
    arg0->unk_3 = arg1[1];
}

// arg1 = ptr from mainMap.unk_C = ptr to byteswapped shorts for vec3f conversion
//INCLUDE_ASM("asm/nonmatchings/system/map", func_800386D4);

u8* func_800386D4(u16 arg0, u8* arg1) {

    u16 i;
    u8 arr[4];

    u8 temp;

    arg1++;

    for (i = 0; i < arg0; i++) {
        
        arr[2] = arg1[0];
        temp = arg1[1];
        
        arg1 += 2;
        arg1 += (temp *  7);
        
        arr[3] = temp;
   
    }

    return arg1;

}

//INCLUDE_ASM("asm/nonmatchings/system/map", setTerrainQuad);
 
// arg2 = offset, return value of getTerrainIndexForTile
void setTerrainQuad(TerrainQuad* quad, u8* quadPtr, u16 offset) {

    u8* arr;

    // skip header
    arr = quadPtr + 4;

    arr += offset * sizeof(TerrainQuad);

    quad->baseHeight = arr[0];

    quad->triangle1[0][0] = arr[1];
    quad->triangle1[0][1] = arr[2];
    quad->triangle1[0][2] = arr[3];
    
    quad->triangle1[1][0] = arr[4];
    quad->triangle1[1][1] = arr[5];
    quad->triangle1[1][2] = arr[6];
    
    quad->triangle1[2][0] = arr[7];
    quad->triangle1[2][1] = arr[8];
    quad->triangle1[2][2] = arr[9];

    quad->triangle2[0][0] = arr[10];
    quad->triangle2[0][1] = arr[11];
    quad->triangle2[0][2] = arr[12];
    
    quad->triangle2[1][0] = arr[13];
    quad->triangle2[1][1] = arr[14];
    quad->triangle2[1][2] = arr[15];
    
    quad->triangle2[2][0] = arr[16];
    quad->triangle2[2][1] = arr[17];
    quad->triangle2[2][2] = arr[18];
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_800387E0);

u32* func_800387E0(u16 arg0, u32* arg1) {
    return (u8*)arg1 + arg1[arg0];
}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_800387F8);

u32* func_800387F8(u16 arg0, u32* arg1) {
    return (u8*)arg1 + arg1[arg0];
}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_80038810);

bool func_80038810(u16 mapIndex) {

    bool result;

    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapState.flags & MAP_ACTIVE)) {
        
        if (!(mainMap[mapIndex].mapState.flags & 2)) {
            mainMap[mapIndex].mapState.flags |= 2;
            func_80036FA0(MAIN_MAP_INDEX);
        }

    }

    // never initialized; whoops
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_8003886C);

bool func_8003886C(u16 mapIndex) {
    
    bool result  = 0;

    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapState.flags & MAP_ACTIVE)) {
        mainMap[mapIndex].mapState.flags &= ~2;
        result = TRUE;
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_800388A4);

// map additions
// param1 = mapAdditions.unk_44, set from sprite vec.x
// param2 = mainMap.unk_20
u8* func_800388A4(u16 count, u8 *ptr) {

    u16 i;
    u32 size;
    u32 temp;
    
    // skip header
    ptr += 6;

    i = 0;

    while (i < count) {
            
        ptr += 2;
    
        size = *ptr++;

        temp = *ptr++;
        temp *= size;

        ptr += (temp * 2);
        
        i++;

        ptr += temp;
            
    } 

    return ptr;
    
}

// alternate 
/*
u8* func_800388A4(u16 arg0, u8 *arg1) {
    
    u16 i;

    u32 temp1;
    u32 temp2;
    
    arg1 += 6;
    
    i = 0;
    
    if (arg0) {

        do {
            
            arg1 += 2;
            
            temp1 = *arg1;
            
            arg1++;
            
            temp2 = *arg1;
            temp2 = temp2 * temp1;

            arg1++;
            
            arg1 += temp2 * 2;
            arg1 += temp2;

            i++;
            
        } while (i < arg0);
    }
    
    return arg1;

}
*/

//INCLUDE_ASM("asm/nonmatchings/system/map", func_80038900);

// called by func_800735FC
bool func_80038900(u16 mapIndex, u16 arg1, u16 arg2, u16 arg3, u16 arg4) {

    bool result = FALSE;

    if (mapIndex == MAIN_MAP_INDEX && mainMap[mapIndex].mapState.flags & MAP_ACTIVE) {
        if (arg1 < 0x20 && !(mainMap[mapIndex].mapAdditions[arg1].flags & 1)) {
            if (arg2 < 0x10) {
                mainMap[mapIndex].mapAdditions[arg1].arr1[arg2] = arg3;
                mainMap[mapIndex].mapAdditions[arg1].arr2[arg2] = arg4;
                result = TRUE;
            }
        }
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_80038990);

bool func_80038990(u16 mapIndex, u16 mapAdditionIndex, u8 arg2) {

    bool result = FALSE;

    if (mapIndex == MAIN_MAP_INDEX && mainMap[mapIndex].mapState.flags & MAP_ACTIVE && mapAdditionIndex < 0x20) {

        if (!(mainMap[mapIndex].mapAdditions[mapAdditionIndex].flags & 1)) {

            mainMap[mapIndex].mapAdditions[mapAdditionIndex].flags = 1;
            mainMap[mapIndex].mapAdditions[mapAdditionIndex].unk_40 = 0;
            mainMap[mapIndex].mapAdditions[mapAdditionIndex].unk_42 = 0;

            result = TRUE;

            if (arg2) {
                mainMap[mapIndex].mapAdditions[mapAdditionIndex].flags = 1 | 4;
            }
        }
    }

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_80038A2C);

bool func_80038A2C(u16 mapIndex, u16 mapAdditionIndex, u8 x, u8 z) {

    bool result = FALSE;

    if (mapIndex == MAIN_MAP_INDEX && mainMap[mapIndex].mapState.flags & MAP_ACTIVE && mapAdditionIndex < 0x20) {

        if (!(mainMap[mapIndex].mapAdditions[mapAdditionIndex].flags & 1)) {
            
            mainMap[mapIndex].mapAdditions[mapAdditionIndex].flags = 0;
            mainMap[mapIndex].mapAdditions[mapAdditionIndex].unk_40 = 0;
            mainMap[mapIndex].mapAdditions[mapAdditionIndex].unk_42 = 0;
            mainMap[mapIndex].mapAdditions[mapAdditionIndex].unk_44 = x;
            mainMap[mapIndex].mapAdditions[mapAdditionIndex].unk_45 = z;

            mainMap[mapIndex].mapAdditions[mapAdditionIndex].flags |= 3;
            
            result = TRUE;

        }
        
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_80038AE0);

bool func_80038AE0(u16 mapIndex, u16 mapAdditionIndex) {

    bool result = FALSE;

    if (mapIndex == MAIN_MAP_INDEX && mainMap[mapIndex].mapState.flags & MAP_ACTIVE) {

        func_800360BC(MAIN_MAP_INDEX, mainMap[mapIndex].mapAdditions[mapAdditionIndex].arr1[0], mainMap[mapIndex].mapAdditions[mapAdditionIndex].unk_44, mainMap[mapIndex].mapAdditions[mapAdditionIndex].unk_45, 0);
        
        result = TRUE;

    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_80038B58);

bool func_80038B58(u16 mapIndex, u16 mapAdditionIndex, u8 arg2, u8 arg3) {
    
    bool result  = 0;

    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapState.flags & MAP_ACTIVE)) {
        func_800360BC(MAIN_MAP_INDEX, mainMap[mapIndex].mapAdditions[mapAdditionIndex].arr1[0], arg2 , arg3, 2);
        result = TRUE;
    }
    
    return result;
    
}

INCLUDE_ASM("asm/nonmatchings/system/map", func_80038BC4);

//INCLUDE_ASM("asm/nonmatchings/system/map", func_800393E0);

void func_800393E0(MainMap *mainMap) {
    
    u16 i;

    u8 flags;
    
    f32 xPosition, yPosition, zPosition;
    f32 tileOffsetX, tileOffsetZ;
    f32 cameraWorldX, cameraWorldZ;
    f32 mapWorldCenterX, mapWorldCenterZ;
    
    s32 temp1, temp2;
    f32 tempF;
    
    for (i = 0; i < MAX_MAP_OBJECTS; i++) {

        flags = mainMap->mapObjects[i].flags; 
        
        if (flags & 1) {
            
            if (!(flags & 2)) {
                startSpriteAnimation(mainMap->mapObjects[i].spriteIndex, mainMap->mapObjects[i].animationIndex, mainMap->mapObjects[i].animationMode);
                mainMap->mapObjects[i].flags |= 2;
            }
            
            xPosition = mainMap->mapObjects[i].coordinates.x;
            zPosition = mainMap->mapObjects[i].coordinates.z;
            
            mapWorldCenterX = (mainMap->mapGrid.mapWidth * mainMap->mapGrid.tileSizeX) / 2;
            mapWorldCenterZ = (mainMap->mapGrid.mapHeight * mainMap->mapGrid.tileSizeZ) / 2;
            
            tileOffsetX = mainMap->mapGrid.tileSizeX / 2;
            tileOffsetZ = mainMap->mapGrid.tileSizeZ / 2;
            
            xPosition = (mainMap->mapObjects[i].coordinates.x + mapWorldCenterX) - tileOffsetX;
            yPosition = mainMap->mapObjects[i].coordinates.y;
            zPosition = (mainMap->mapObjects[i].coordinates.z + mapWorldCenterZ) - tileOffsetZ;
            
            cameraWorldX = mainMap->mapCameraView.calculatedTileX * mainMap->mapGrid.tileSizeX;
            cameraWorldZ = mainMap->mapCameraView.calculatedTileZ * mainMap->mapGrid.tileSizeZ;
            
            xPosition -= cameraWorldX;
            zPosition -= cameraWorldZ;
            
            // FIXME: fake
            tempF = mainMap->mapCameraView.viewOffset.z;
            
            xPosition += mainMap->mapCameraView.viewOffset.x;
            zPosition += mainMap->mapCameraView.viewOffset.z;
            
            if (mainMap->mapObjects[i].flags & 4) {
    
                temp1 = (mainMap->mapCameraView.calculatedTileX - (mainMap->mapGrid.mapWidth) / 2) * mainMap->mapGrid.tileSizeX;
                temp2 = (mainMap->mapCameraView.calculatedTileZ - (mainMap->mapGrid.mapHeight) / 2) * mainMap->mapGrid.tileSizeZ;
    
                if (temp1 >= 0) {
                    temp1 += ((mainMap->mapCameraView.viewExtentX * mainMap->mapGrid.tileSizeX) / 2); 
                } else {
                    temp1 -= ((mainMap->mapCameraView.viewExtentX * mainMap->mapGrid.tileSizeX) / 2); 
                }
    
                if (temp2 >= 0) {
                    temp2 += ((mainMap->mapCameraView.viewExtentZ * mainMap->mapGrid.tileSizeZ) / 2); 
                } else {
                    temp2 -= ((mainMap->mapCameraView.viewExtentZ * mainMap->mapGrid.tileSizeZ) / 2); 
                }

                temp1 = (s32)((temp1 - xPosition) / 320) * 320;
                temp2 = (s32)((temp2 - zPosition) / 240) * 240;

                xPosition = (s32)xPosition + temp1;
                yPosition = (s32)yPosition;
                zPosition = (s32)zPosition + temp2;

            }
        
            tempF = mainMap->mapCameraView.viewOffset.y;
            flags = mainMap->mapObjects[i].flags & 8;

            // FIXME: likely fake
            do {
                
                yPosition += tempF;
                
                if (flags) {
                    
                    if (D_801FB700 < 0x18) {
                        D_801FB5CB++;
                    } else {
                        D_801FB5CB--;
                    }
                    
                    D_801FB700++;
                    
                    if (D_801FB700 == 0x30) {
                        D_801FB700 = 0;
                    }
                    
                    yPosition += D_801FB5CB / 2;
                
                }
                
            } while (0);
        
            setSpriteViewSpacePosition(mainMap->mapObjects[i].spriteIndex, xPosition, yPosition, zPosition);
            
            if (mainMap->mapObjects[i].unk_10 != 0xFF) {
                setSpriteRotation(mainMap->mapObjects[i].spriteIndex, mainMap->mapFloats.rotation.x, mainMap->mapFloats.rotation.y + mainMap->mapObjects[i].unk_10, mainMap->mapFloats.rotation.z);
            }
            
            setSpriteColor(mainMap->mapObjects[i].spriteIndex, mainMap->mapFloats.groundRgba.r, mainMap->mapFloats.groundRgba.g, mainMap->mapFloats.groundRgba.b, mainMap->mapFloats.groundRgba.a);
            setBilinearFiltering(mainMap->mapObjects[i].spriteIndex, 1);
        
        }
    
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_80039990);

void func_80039990(MainMap* mainMap) {

    u16 i;

    s16 temp1;
    s32 temp2;
    s16 temp3;
    s32 temp4;
    
    f32 xPosition, yPosition, zPosition;
    f32 cameraWorldX, cameraWorldZ;
    f32 tileCenterOffsetX, tileCenterOffsetZ;
    f32 spriteWorldX, spriteWorldZ;
    f32 mapWorldCenterX, mapWorldCenterZ;
    f32 tileOffsetX, tileOffsetZ; 
    
    for (i = 0; i < MAX_WEATHER_SPRITES; i++) {

        if (mainMap->weatherSprites[i].flags & 1) {

            if (func_8002BCC8(mainMap->weatherSprites[i].spriteIndex) || !(mainMap->weatherSprites[i].flags & 2)) {
                
                resetAnimationState(mainMap->weatherSprites[i].spriteIndex);

                if (!(getRandomNumberInRange(0, 4))) {
                
                    globalSprites[mainMap->weatherSprites[i].spriteIndex].stateFlags &= ~0x40;
                    startSpriteAnimation(mainMap->weatherSprites[i].spriteIndex, mainMap->weatherSprites[i].animationIndex, 0xFE);
                    
                    mainMap->weatherSprites[i].flags |= 2;
                    
                    temp1 = getRandomNumberInRange(0, 0x140);
                    temp2 = (((mainMap->mapCameraView.calculatedTileX - (mainMap->mapGrid.mapWidth / 2)) * mainMap->mapGrid.tileSizeX)) - 0xA0;

                    mainMap->weatherSprites[i].coordinates.x = temp1 + temp2;
                        
                    mainMap->weatherSprites[i].coordinates.y = 0;
                    
                    temp3 = getRandomNumberInRange(0, 0xF0);
                    temp4 = (((mainMap->mapCameraView.calculatedTileZ - (mainMap->mapGrid.mapHeight / 2)) * mainMap->mapGrid.tileSizeZ)) - 0x78;
                    
                    mainMap->weatherSprites[i].coordinates.z = temp3 + temp4;
                    
                }
            
            }

            mapWorldCenterX = (mainMap->mapGrid.mapWidth * mainMap->mapGrid.tileSizeX) / 2;
            mapWorldCenterZ = (mainMap->mapGrid.mapHeight * mainMap->mapGrid.tileSizeZ) / 2;

            tileOffsetX = mainMap->mapGrid.tileSizeX / 2;
            tileOffsetZ = mainMap->mapGrid.tileSizeZ / 2;

            cameraWorldX = mainMap->mapCameraView.calculatedTileX * mainMap->mapGrid.tileSizeX;
            cameraWorldZ = mainMap->mapCameraView.calculatedTileZ * mainMap->mapGrid.tileSizeZ;

            spriteWorldX = mainMap->weatherSprites[i].coordinates.x;
            spriteWorldZ = mainMap->weatherSprites[i].coordinates.z;

            xPosition = spriteWorldX + mapWorldCenterX - tileOffsetX;
            zPosition = spriteWorldZ + mapWorldCenterZ - tileOffsetZ;
            
            xPosition -= cameraWorldX;
            zPosition -= cameraWorldZ;
            
            xPosition += mainMap->mapCameraView.viewOffset.x;
            zPosition += mainMap->mapCameraView.viewOffset.z;

            yPosition = mainMap->weatherSprites[i].coordinates.y;
            
            if (globalSprites[mainMap->weatherSprites[i].spriteIndex].renderingFlags & 0x200) {
                yPosition -= mainMap->mapCameraView.viewOffset.y;
            } else {
                yPosition += mainMap->mapCameraView.viewOffset.y;
            }

            setSpriteViewSpacePosition(mainMap->weatherSprites[i].spriteIndex,
                xPosition, yPosition, zPosition);

            setSpriteColor(mainMap->weatherSprites[i].spriteIndex, 
                   mainMap->mapFloats.groundRgba.r, 
                   mainMap->mapFloats.groundRgba.g, 
                   mainMap->mapFloats.groundRgba.b, 
                   mainMap->mapFloats.groundRgba.a);
            
            setBilinearFiltering(mainMap->weatherSprites[i].spriteIndex, TRUE);
                
        }
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", processMapSceneNode);

void processMapSceneNode(u16 mapIndex, Gfx* dl) {

    u16 temp = addSceneNode(dl, (8 | 0x20));
    
    addSceneNodePosition(temp, mainMap[mapIndex].mapFloats.translation.x + mainMap[mapIndex].mapCameraView.viewOffset.x, 
        mainMap[mapIndex].mapFloats.translation.y + mainMap[mapIndex].mapCameraView.viewOffset.y, 
        mainMap[mapIndex].mapFloats.translation.z + mainMap[mapIndex].mapCameraView.viewOffset.z);

    addSceneNodeScaling(temp, mainMap[mapIndex].mapFloats.scale.x, mainMap[mapIndex].mapFloats.scale.y, mainMap[mapIndex].mapFloats.scale.z);
    
    addSceneNodeRotation(temp, mainMap[mapIndex].mapFloats.rotation.x, mainMap[mapIndex].mapFloats.rotation.y, mainMap[mapIndex].mapFloats.rotation.z);

}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_80039F58);

void func_80039F58(u16 mapIndex) {

    bool found = FALSE;
    bool set = FALSE;
    u16 i = 0;

    do {

        if (mainMap[mapIndex].mapAdditions[i].flags & 1 && !set) {
        
            do {

                if (mainMap[mapIndex].mapAdditions[i].unk_40 == 0) { 

                    if (mainMap[mapIndex].mapAdditions[i].arr1[mainMap[mapIndex].mapAdditions[i].unk_42] != 0xFFFF) {
 
                        func_800360BC(mapIndex, mainMap[mapIndex].mapAdditions[i].arr1[mainMap[mapIndex].mapAdditions[i].unk_42], mainMap[mapIndex].mapAdditions[i].unk_44, mainMap[mapIndex].mapAdditions[i].unk_45, mainMap[mapIndex].mapAdditions[i].flags & 2);
                        found = TRUE;
                        
                    } else {
                        
                        if (mainMap[mapIndex].mapAdditions[i].flags & 4) {
                            
                            mainMap[mapIndex].mapAdditions[i].unk_40 = 0;
                            mainMap[mapIndex].mapAdditions[i].unk_42 = 0;
                            
                            func_800360BC(mapIndex, mainMap[mapIndex].mapAdditions[i].arr1[mainMap[mapIndex].mapAdditions[i].unk_42], mainMap[mapIndex].mapAdditions[i].unk_44, mainMap[mapIndex].mapAdditions[i].unk_45, mainMap[mapIndex].mapAdditions[i].flags & 2);
                            found = TRUE;
                            
                        } else {
                            mainMap[mapIndex].mapAdditions[i].flags &= ~1;
                        }

                    }
                }
                
                if (mainMap[mapIndex].mapAdditions[i].unk_40 >= mainMap[mapIndex].mapAdditions[i].arr2[mainMap[mapIndex].mapAdditions[i].unk_42]) {

                    mainMap[mapIndex].mapAdditions[i].unk_42++;
                    
                    if (mainMap[mapIndex].mapAdditions[i].unk_42 == 16) {
                        mainMap[mapIndex].mapAdditions[i].flags &= ~1;
                        set = TRUE;
                    } 

                    mainMap[mapIndex].mapAdditions[i].unk_40 = 0;
                     
                } else {
                    mainMap[mapIndex].mapAdditions[i].unk_40++;
                    set = TRUE;
                    
                }
                
            } while (!set);
            
        }

        i++;
            
    } while (i < 32);

    if (found) {
        func_80036C08(MAIN_MAP_INDEX);
    }
    
}

static inline u8 updateMapRGBA(u16 i) {

    u8 count = 0;
    
    if (mainMap[i].mapFloats.groundRgba.r < mainMap[i].mapFloats.defaultRgba.r) {
                
        mainMap[i].mapFloats.groundRgba.r += mainMap[i].mapFloats.unk_60.r; 
        
        if (mainMap[i].mapFloats.defaultRgba.r <= mainMap[i].mapFloats.groundRgba.r) {
            mainMap[i].mapFloats.groundRgba.r = mainMap[i].mapFloats.defaultRgba.r;
        } else {
            count = 1;
        } 
    }

    if (mainMap[i].mapFloats.groundRgba.r > mainMap[i].mapFloats.defaultRgba.r) {
        
        mainMap[i].mapFloats.groundRgba.r -= mainMap[i].mapFloats.unk_60.r;

        if (mainMap[i].mapFloats.groundRgba.r <= mainMap[i].mapFloats.defaultRgba.r) {
            mainMap[i].mapFloats.groundRgba.r = mainMap[i].mapFloats.defaultRgba.r;
        } else {
            count += 1;
        } 
        
    }

    if (mainMap[i].mapFloats.groundRgba.g < mainMap[i].mapFloats.defaultRgba.g) {
        
        mainMap[i].mapFloats.groundRgba.g += mainMap[i].mapFloats.unk_60.g; 
        
        if (mainMap[i].mapFloats.defaultRgba.g <= mainMap[i].mapFloats.groundRgba.g) {
            mainMap[i].mapFloats.groundRgba.g = mainMap[i].mapFloats.defaultRgba.g;
        } else {
            count += 1;
        } 
    }

    if (mainMap[i].mapFloats.groundRgba.g > mainMap[i].mapFloats.defaultRgba.g) {
        
        mainMap[i].mapFloats.groundRgba.g -= mainMap[i].mapFloats.unk_60.g;

        if (mainMap[i].mapFloats.groundRgba.g <= mainMap[i].mapFloats.defaultRgba.g) {
            mainMap[i].mapFloats.groundRgba.g = mainMap[i].mapFloats.defaultRgba.g;
        } else {
            count += 1;
        } 
        
    }

    if (mainMap[i].mapFloats.groundRgba.b < mainMap[i].mapFloats.defaultRgba.b) {
        
        mainMap[i].mapFloats.groundRgba.b += mainMap[i].mapFloats.unk_60.b; 
        
        if (mainMap[i].mapFloats.defaultRgba.b <= mainMap[i].mapFloats.groundRgba.b) {
            mainMap[i].mapFloats.groundRgba.b = mainMap[i].mapFloats.defaultRgba.b;
        } else {
            count += 1;
        } 
    }

    if (mainMap[i].mapFloats.groundRgba.b > mainMap[i].mapFloats.defaultRgba.b) {
        
        mainMap[i].mapFloats.groundRgba.b -= mainMap[i].mapFloats.unk_60.b;

        if (mainMap[i].mapFloats.groundRgba.b <= mainMap[i].mapFloats.defaultRgba.b) {
            mainMap[i].mapFloats.groundRgba.b = mainMap[i].mapFloats.defaultRgba.b;
        } else {
            count += 1;
        } 
        
    }

    if (mainMap[i].mapFloats.groundRgba.a < mainMap[i].mapFloats.defaultRgba.a) {
        
        mainMap[i].mapFloats.groundRgba.a += mainMap[i].mapFloats.unk_60.a; 
        
        if (mainMap[i].mapFloats.defaultRgba.a <= mainMap[i].mapFloats.groundRgba.a) {
            mainMap[i].mapFloats.groundRgba.a = mainMap[i].mapFloats.defaultRgba.a;
        } else {
            count += 1;
        } 
    }

    if (mainMap[i].mapFloats.groundRgba.a > mainMap[i].mapFloats.defaultRgba.a) {
        
        mainMap[i].mapFloats.groundRgba.a -= mainMap[i].mapFloats.unk_60.a;

        if (mainMap[i].mapFloats.groundRgba.a <= mainMap[i].mapFloats.defaultRgba.a) {
            mainMap[i].mapFloats.groundRgba.a = mainMap[i].mapFloats.defaultRgba.a;
        } else {
            count += 1;
        } 
        
    }

    return count;
    
}

static inline void handleRotation(u16 i) {
    
    mainMap[i].mapCameraView.rotation.x = 0.0f;
    mainMap[i].mapCameraView.rotation.y = 360.0f - currentWorldRotationAngles.y;
    mainMap[i].mapCameraView.rotation.z = 0.0f;

    if (mainMap[i].mapCameraView.rotation.y < 0.0f) {
        mainMap[i].mapCameraView.rotation.y += 360.0f;
    }

    if (mainMap[i].mapCameraView.rotation.y >= 360.0f) {
        mainMap[i].mapCameraView.rotation.y -= 360.0f;
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", updateMapGraphics);

void updateMapGraphics(void) {

    u16 height = 0;
    u16 i;
    
    Gfx *dl = mapDisplayList[gGraphicsBufferIndex];
    Gfx *dlStartingPosition;
    
    // FIXME: shoudn't be necessary
    u32 padding[4];
    
    for (i = 0; i < MAX_MAPS; i++) {

        if ((mainMap[i].mapState.flags & MAP_ACTIVE) && (mainMap[i].mapState.flags & 2)) {
            
            func_80039F58(i);

            if (updateMapRGBA(i) == 0) {
                mainMap[i].mapState.flags |= RGBA_FINISHED;
            } else {
                mainMap[i].mapState.flags &= ~RGBA_FINISHED;
            }

            if (mainMap[i].mapFloats.rotation.x < 0) {
                mainMap[i].mapFloats.rotation.x += 360.0f;
            }

            if (mainMap[i].mapFloats.rotation.x >= 360.0f) {
                mainMap[i].mapFloats.rotation.x -= 360.0f;
            }
            
            if (mainMap[i].mapFloats.rotation.y < 0) {
                mainMap[i].mapFloats.rotation.y += 360.0f;
            }

            if (mainMap[i].mapFloats.rotation.y >= 360.0f) {
                mainMap[i].mapFloats.rotation.y -= 360.0f;
            }

            if (mainMap[i].mapFloats.rotation.z < 0) {
                mainMap[i].mapFloats.rotation.z += 360.0f;
            }

            if (mainMap[i].mapFloats.rotation.z >= 360.0f) {
                mainMap[i].mapFloats.rotation.z -= 360.0f;
            }

            handleRotation(i);
            
            dlStartingPosition = dl;
            
            dl = func_80037DF0(dlStartingPosition, &mainMap[i], height);
            
            func_80038BC4(&mainMap[i]);
            func_800393E0(&mainMap[i]);
            func_80039990(&mainMap[i]);
            
            processMapSceneNode(i, dlStartingPosition);

            // append spawnable sprites to display list
            func_8003B1BC(&mainMap[i]);

            height += mainMap[i].mapState.height;

        }
        
    }

    if (dl - mapDisplayList[gGraphicsBufferIndex] >= 0x1B00) {
        // FIXME: replace with string literals; same strings used by updateMapGraphics
        __assert(D_8011EDB0, D_8011EDB4, 3912);
    }

}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_8003AC14);

inline void func_8003AC14(Gfx* dl, MapBitmap* arg1) {
    
    // gsDPSetCombineMode(G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA)
    *dl++ = D_8011ED90;
    // gsDPSetRenderMode(G_RM_TEX_EDGE, G_RM_TEX_EDGE2)
    *dl++ = D_8011EDC8;
    // gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON)
    *dl++ = D_8011EDD0;
    // gsDPSetCombineMode(G_CC_MODULATEIA, G_CC_MODULATEIA)
    *dl++ = D_8011ED78;

    loadBitmapTexture(dl++, (BitmapObject*)arg1, 0, arg1->height);
    
}
 
//INCLUDE_ASM("asm/nonmatchings/system/map", func_8003ACA8);

Gfx* func_8003ACA8(Gfx* dl, MainMap* map, MapBitmap* bitmap, u16 vtxIndex) {

    Gfx tempDl[2];
      
    setupBitmapVertices((Vtx*)&D_80165500[gGraphicsBufferIndex][vtxIndex], 
        bitmap->width, 
        bitmap->height,
        bitmap->height, 
        0, 
        0,
        0, 
        0, 
        0, 
        (0x10 | 0x40 | 0x100), 
        map->mapFloats.groundRgba.r, 
        map->mapFloats.groundRgba.g, 
        map->mapFloats.groundRgba.b,
        map->mapFloats.groundRgba.a);
 
    // FIXME: might be a wrapper around gSPVertex
    gSPVertex(&tempDl[1], &D_80165500[gGraphicsBufferIndex][vtxIndex][0], 4, 0);

    *tempDl = *(tempDl+1);
    *dl++ = *tempDl;
    
    *dl++ = D_8011EDD8;
    *dl++ = D_8011EDA8;
    *dl++ = D_8011ED80;
    
    return dl++;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_8003AF58);

Vec3f func_8003AF58(u16 arg1, u8 arg2, u8 arg3) {

    Vec3f vec;

    f32 temp1;
    f32 temp2;

    // these two statements have to be on the same line to match lmao (otherwise compiler inserts a nop)
    temp1 = mainMap[arg1].mapGrid.tileSizeX; vec.x = ((arg2 *  temp1) - mainMap[arg1].mapState.mapOriginX) + (mainMap[arg1].mapGrid.tileSizeX / 2);

    temp2 = mainMap[arg1].mapGrid.tileSizeZ; vec.z = ((arg3 * temp2) - mainMap[arg1].mapState.mapOriginZ) + (mainMap[arg1].mapGrid.tileSizeZ / 2);
    
    vec.y = getTerrainHeightAtPosition(MAIN_MAP_INDEX, vec.x, vec.z);
    
    if (arg2 < mainMap[arg1].groundObjects.unk_10 || arg3 < mainMap[arg1].groundObjects.unk_11 || arg2 >= mainMap[arg1].groundObjects.unk_10 + 0x14 || arg3 >= mainMap[arg1].groundObjects.unk_11 + 0x18) {
        vec.y = 65535.0f;
    }
    
    return vec;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_8003B100);

// unused or inline
void func_8003B100(MainMap* map, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, Gfx* dl) {

    Vec3f vec;
    
    f32 x = (arg1 + map->mapState.mapOriginX + map->mapCameraView.viewOffset.x) - arg4;
    f32 y = arg2 + map->mapCameraView.viewOffset.y;
    f32 z = (arg3 + map->mapState.mapOriginZ + map->mapCameraView.viewOffset.z) - arg5;

    addSceneNodePosition(addSceneNode(dl, 8), x, y, z);

}

// check if map with spawnable items and append to display lists
INCLUDE_ASM("asm/nonmatchings/system/map", func_8003B1BC);

static const Gfx D_8011ED68 = gsDPSetCombineMode(G_CC_MODULATEIA, G_CC_MODULATEIA);

//INCLUDE_RODATA("asm/nonmatchings/systemmap", D_8011ED68);

static const Gfx D_8011ED70 = gsDPSetRenderMode(G_RM_RA_ZB_OPA_SURF, G_RM_RA_ZB_OPA_SURF2);

//INCLUDE_RODATA("asm/nonmatchings/systemmap", D_8011ED70);

static const Gfx D_8011ED78 = gsDPSetTextureFilter(G_TF_BILERP);

//INCLUDE_RODATA("asm/nonmatchings/systemmap", D_8011ED78);

static const Gfx D_8011ED80 = gsSPEndDisplayList();

//INCLUDE_RODATA("asm/nonmatchings/systemmap", D_8011ED80);

static const Gfx D_8011ED88 = gsSPTexture(0, 0, 0, G_TX_RENDERTILE, G_ON);

//INCLUDE_RODATA("asm/nonmatchings/systemmap", D_8011ED88);

static const Gfx D_8011ED90 = gsDPSetCombineMode(G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);

//INCLUDE_RODATA("asm/nonmatchings/systemmap", D_8011ED90);

static const Gfx D_8011ED98 = gsSPTexture(0, 0, 0, G_TX_RENDERTILE, G_OFF);

//INCLUDE_RODATA("asm/nonmatchings/systemmap", D_8011ED98);

static const Gfx D_8011EDA0 = gsDPSetCombineLERP(PRIMITIVE, 0, SHADE, 0, PRIMITIVE, 0, SHADE, 0, PRIMITIVE, 0, SHADE, 0, PRIMITIVE, 0, SHADE, 0);

//INCLUDE_RODATA("asm/nonmatchings/systemmap", D_8011EDA0);

static const Gfx D_8011EDA8 = gsDPPipeSync();

//INCLUDE_RODATA("asm/nonmatchings/systemmap", D_8011EDA8);

//INCLUDE_RODATA("asm/nonmatchings/systemmap", D_8011EDB0);

//INCLUDE_RODATA("asm/nonmatchings/systemmap", D_8011EDB4);

static const char D_8011EDB0[] = "EX";
static const char D_8011EDB4[] = "s:/system/map.c";

static const Gfx D_8011EDC8 = gsDPSetRenderMode(G_RM_TEX_EDGE, G_RM_TEX_EDGE2);

//INCLUDE_RODATA("asm/nonmatchings/systemmap", D_8011EDC8);

static const Gfx D_8011EDD0 = gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON);

//INCLUDE_RODATA("asm/nonmatchings/systemmap", D_8011EDD0);

static const Gfx D_8011EDD8 = gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0);

//INCLUDE_RODATA("asm/nonmatchings/systemmap", D_8011EDD8);


