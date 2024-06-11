#include "common.h"

#include "system/map.h"

#include "system/graphic.h"
#include "system/mathUtils.h"
#include "system/mapContext.h"
#include "system/worldGraphics.h"

#include "mainproc.h" 
 
// forward declarations
void func_80035A58(UnknownMapStruct2*);
bool func_800360BC(u16, u16, u8, u8, u32);       
Vec3f* func_800366F4(Vec3f*, u16, f32, f32);                  
u16 func_80036880(u16, f32, f32);  
void func_80036AB4(MainMap*); 
void func_800372F0(UnknownMapStruct1*, LevelMapContext*);                 
u8* func_800374C0(WeatherSprite*, u8*);
u8* func_80037614(MapVtx* arg0, VtxInfo* arg1);    
u16* func_80037650(u16 arg0, void *arg1);
u32 func_80037668(MainMap*, u16, f32, f32, f32);
Gfx* func_80037BC4(Gfx*, MainMap*, u16, u16);
s16 func_80037F08(Gfx*, MainMap*, WeatherSprite*);  
Gfx* func_800383B0(Gfx* arg0, MainMap* arg1, u16 arg2, f32 arg3, f32 arg4, f32 arg5);
void func_8003851C(MainMap*);  
void func_80038630(DecompressedVec3f* arg0, CompressedVec3f* arg1);                        
void func_800386C0(UnknownMapStruct3* arg0, u8* arg1); 
CompressedVec3f* func_800386D4(u16 arg0, u8* arg1);     
u8* func_800388A4(u16 arg0, u8 *arg1);
void func_80038BC4(MainMap*); 
void func_800393E0(MainMap*); 
void func_80039990(MainMap*); 
void func_80039E20(u16, Gfx*);                        
bool func_8003B1BC(MainMap*);         
void func_80039F58(u16);   


// bss
extern MainMap mainMap[1];

extern f32 D_8013D550;
extern f32 D_80170460;
extern DecompressedVec3f D_80181BA0[];
extern u8 D_801FB5CB;
extern f32 D_801FB5D4;
// counter for updating shrink factor
extern u8 D_801FB700;
extern f32 D_801FB5D4;
extern f32 D_802226EC;

extern u8 D_8013D5F0[];
extern Vtx D_80165500[2][320][4];
extern u8 D_8018A090[];
extern u8 D_80181BAC[0x10][0x10];
extern Gfx D_801A8B60[2][6912];
extern Vtx mapVertices[2][2560];

// rodata
extern Gfx D_8011ED68[4];
extern Gfx D_8011ED88[5];
extern Gfx D_8011EDC8[3];
extern Gfx D_8011ED80;
extern Gfx D_8011EDA8;
extern Gfx D_8011EDD8;

extern const char D_8011EDB0[];
extern const char D_8011EDB4[];

//INCLUDE_ASM(const s32, "system/map", func_800337D0);

void func_800337D0(void) {
 
    u16 i, j; 
     
    D_801FB700 = 0;
    D_801FB5CB = 0;
    
    for (i = 0; i < MAX_MAPS; i++) {
        
        mainMap[i].mapStruct9.flags = 0;
        mainMap[i].mapStruct9.unk_A = 0;
        mainMap[i].mapStruct9.unk_C = 0;
        mainMap[i].mapStruct9.height = 0;

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

        mainMap[i].mapStruct2.unk_48 = 0;
        mainMap[i].mapStruct2.unk_49 = 0;
        mainMap[i].mapStruct2.unk_4A = 0;
        mainMap[i].mapStruct2.unk_4B = 0;

        mainMap[i].mapStruct2.unk_3C.x = 0;
        mainMap[i].mapStruct2.unk_3C.y = 0;
        mainMap[i].mapStruct2.unk_3C.z = 0;

        mainMap[i].mapStruct2.unk_0.x = 0;
        mainMap[i].mapStruct2.unk_0.y = 0;
        mainMap[i].mapStruct2.unk_0.z = 0;

        mainMap[i].mapStruct2.unk_C.x = 0;
        mainMap[i].mapStruct2.unk_C.y = 0;
        mainMap[i].mapStruct2.unk_C.z = 0;
        
        mainMap[i].mapStruct2.unk_18.x = 0;
        mainMap[i].mapStruct2.unk_18.y = 0;
        mainMap[i].mapStruct2.unk_18.z = 0;

        mainMap[i].mapStruct2.unk_24.x = 0;
        mainMap[i].mapStruct2.unk_24.y = 0;
        mainMap[i].mapStruct2.unk_24.z = 0;

        mainMap[i].mapStruct2.unk_30.x = 0;
        mainMap[i].mapStruct2.unk_30.y = 0;
        mainMap[i].mapStruct2.unk_30.z = 0;
  
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

//INCLUDE_ASM(const s32, "system/map", func_80033A90);

// FIXME: iterators are messed up; loops are probably inline funcs 
bool func_80033A90(u16 mapIndex, LevelMapContext* arg1, void* arg2, void* arg3, void* arg4, void* arg5, void* arg6, void* arg7, void* arg8, void* arg9, void *argA) {

    bool result;

    u16 i, k, l, m, n, o, p, q, j, r;

    u8 temp1;
    u8 temp2;

    result = FALSE;

    if (mapIndex == 0 && !(mainMap[mapIndex].mapStruct9.flags & 1)) {
        
        mainMap[mapIndex].mapStruct9.flags = 1;
        mainMap[mapIndex].mapStruct9.unk_A = 0;
        mainMap[mapIndex].mapStruct9.unk_11 = 0;
        
        mainMap[mapIndex].unk_0 = arg2;
        mainMap[mapIndex].unk_4 = (u8*)arg3;
        mainMap[mapIndex].unk_8 = (u8**)arg4;
        mainMap[mapIndex].unk_C = (u8*)arg5;
        mainMap[mapIndex].unk_10 = arg6;
        mainMap[mapIndex].unk_14 = arg7;
        mainMap[mapIndex].unk_18 = arg8;
        mainMap[mapIndex].unk_1C = arg9;
        mainMap[mapIndex].unk_20 = argA;

        for (j = 0; j < 16; j++) {

            mainMap[mapIndex].mapStruct3[j].unk_2 = 0; 
            mainMap[mapIndex].mapStruct3[j].unk_3 = 0; 
            mainMap[mapIndex].mapStruct3[j].unk_0 = 0; 

        }

        for (l = 0; l < 0x100; l++) {
            D_80181BAC[l][0] = 0;
        }

        for (i = 0; i < 16; i++) {
            mainMap[mapIndex].mapObjects[i].flags = 0;
        }

        for (i = 0; i < 0x40; i++) {
            mainMap[mapIndex].groundObjects.arr[i] = 0xFFFF;
        }

        for (i = 0; i < 0x10; i++) {
            mainMap[mapIndex].weatherSprites[i].flags = 0;
        }

        for (p = 0; p < 0x1E0; p++) {
            mainMap[mapIndex].groundObjects.arr2[0][p] = 0;
            mainMap[mapIndex].groundObjects.arr2[1][p] = 0;
            mainMap[mapIndex].groundObjects.arr2[2][p] = 0;
        }

        for (i = 0; i < 0x20; i++) {

            for (j = 0; j < 0x10; j++) {
                mainMap[mapIndex].mapAdditions[i].arr1[j] = 0xFFFF;
                mainMap[mapIndex].mapAdditions[i].arr2[j] = 0;
            }
            
            mainMap[mapIndex].mapAdditions[i].unk_40 = 0;
            mainMap[mapIndex].mapAdditions[i].unk_42 = 0;
            mainMap[mapIndex].mapAdditions[i].flags = 0;
            
        }

        func_800372F0(&mainMap[mapIndex].mapStruct1, arg1);

        func_80036AB4(&mainMap[mapIndex]);
        func_80036C08(mapIndex);
        func_8003851C(&mainMap[mapIndex]);

        temp1 = mainMap[mapIndex].mapStruct1.unk_A >> 1;
        temp2 = mainMap[mapIndex].mapStruct1.unk_B >> 1;

        func_800343FC(mapIndex, temp1, temp2, temp1, temp2, 0.0f, 0.0f, 0.0f, 1);
        func_80035A58(&mainMap[mapIndex].mapStruct2);

        mainMap[mapIndex].mapStruct9.unk_0 = ((mainMap[mapIndex].mapStruct1.unk_A * mainMap[mapIndex].mapStruct1.unk_8) >> 1) + (mainMap[mapIndex].mapStruct1.unk_8 >>1);
        mainMap[mapIndex].mapStruct9.unk_4 = ((mainMap[mapIndex].mapStruct1.unk_B * mainMap[mapIndex].mapStruct1.unk_9) >> 1) + (mainMap[mapIndex].mapStruct1.unk_9 >>1);

        for (r = 0; r < 0x63C; r++) {
            D_8013D5F0[r] = r % mainMap[mapIndex].mapStruct1.unk_A;
            D_8018A090[r] = r / mainMap[mapIndex].mapStruct1.unk_A;
        }
        
        result = TRUE;
        
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/map", func_80034090);

// turn off active sprite flags for map objects
bool func_80034090(u16 mapIndex) {

    bool result = FALSE;

    u8 i;

    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapStruct9.flags & 1)) {
        
        D_801FB700 = 0;
        D_801FB5CB = 0;

        for (i = 0; i < 0x10; i++) {
            
            if (mainMap[mapIndex].mapObjects[i].flags & 1) {
                deactivateSprite(mainMap[mapIndex].mapObjects[i].spriteIndex);
            }
            
            mainMap[mapIndex].mapObjects[i].flags = 0;
        }
        
        for (i = 0; i < 0x10; i++) { 
            
            if (mainMap[mapIndex].weatherSprites[i].flags & 1) {
                deactivateSprite(mainMap[mapIndex].weatherSprites[i].spriteIndex);
            }

            mainMap[mapIndex].weatherSprites[i].flags = 0;
        }
        
        mainMap[mapIndex].mapStruct9.flags = 0;

        result = TRUE;
        
    }

    return result;
}

//INCLUDE_ASM(const s32, "system/map", setMapTranslation);

bool setMapTranslation(u16 mapIndex, f32 arg1, f32 arg2, f32 arg3) {

    bool result = FALSE;
    
    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapStruct9.flags & 1)) {

        mainMap[mapIndex].mapFloats.translation.x = arg1;
        mainMap[mapIndex].mapFloats.translation.y = arg2;
        mainMap[mapIndex].mapFloats.translation.z = arg3;

        result = TRUE;

    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/map", func_80034298);

bool setMapScale(u16 mapIndex, f32 arg1, f32 arg2, f32 arg3) {

    bool result = FALSE;
    
    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapStruct9.flags & 1)) {
        
        mainMap[mapIndex].mapFloats.scale.x = arg1;
        mainMap[mapIndex].mapFloats.scale.y = arg2;
        mainMap[mapIndex].mapFloats.scale.z = arg3;
        
        result = TRUE;

    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/map", setMapRotation);

bool setMapRotation(u16 mapIndex, f32 arg1, f32 arg2, f32 arg3) {

    bool result = FALSE;
    
    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapStruct9.flags & 1)) {
        
        mainMap[mapIndex].mapFloats.rotation.x = arg1;
        mainMap[mapIndex].mapFloats.rotation.y = arg2;
        mainMap[mapIndex].mapFloats.rotation.z = arg3;
        
        result = TRUE;

    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/map", setMapRGBA);

bool setMapRGBA(u16 mapIndex, u8 arg1, u8 arg2, u8 arg3, u8 arg4) {
    
    bool result = FALSE;
    
    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapStruct9.flags & 1)) {
        

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

//INCLUDE_ASM(const s32, "system/map", func_800343FC);

bool func_800343FC(u16 mapIndex, u8 arg1, u8 arg2, u8 arg3, u8 arg4, f32 arg5, f32 arg6, f32 arg7, u8 arg8) {

    bool result = FALSE;

    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapStruct9.flags & 1)) {

        mainMap[mapIndex].mapStruct2.unk_48 = arg1;
        mainMap[mapIndex].mapStruct2.unk_49 = arg2;
        mainMap[mapIndex].mapStruct2.unk_4A = arg3;
        mainMap[mapIndex].mapStruct2.unk_4B = arg4;

        mainMap[mapIndex].mapStruct2.unk_3C.x = -arg5;
        mainMap[mapIndex].mapStruct2.unk_3C.y = -arg6;
        mainMap[mapIndex].mapStruct2.unk_3C.z = -arg7;
        
        if (arg8 == 1) {
            mainMap[mapIndex].mapStruct9.flags |= 4;
        } else {
            mainMap[mapIndex].mapStruct9.flags &= ~4;
        }

        func_80035A58(&mainMap[mapIndex].mapStruct2);
        
        result = TRUE;

    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "system/map", adjustMapTranslation);

bool adjustMapTranslation(u16 mapIndex, f32 arg1, f32 arg2, f32 arg3) {

    bool result = FALSE;
    
    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapStruct9.flags & 1)) {
        
        mainMap[mapIndex].mapFloats.translation.x += arg1;
        mainMap[mapIndex].mapFloats.translation.y += arg2;
        mainMap[mapIndex].mapFloats.translation.z += arg3;
        
        result = TRUE;

    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/map", func_80034568);

bool func_80034568(u16 mapIndex, f32 arg1, f32 arg2, f32 arg3) {

    bool result = FALSE;
    
    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapStruct9.flags & 1)) {
        
        mainMap[mapIndex].mapFloats.scale.x += arg1;
        mainMap[mapIndex].mapFloats.scale.y += arg2;
        mainMap[mapIndex].mapFloats.scale.z += arg3;
        
        result = TRUE;

    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/map", adjustMapRotation);

bool adjustMapRotation(u16 mapIndex, f32 arg1, f32 arg2, f32 arg3) {

    bool result = FALSE;
    
    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapStruct9.flags & 1)) {
        
        mainMap[mapIndex].mapFloats.rotation.x += arg1;
        mainMap[mapIndex].mapFloats.rotation.y += arg2;
        mainMap[mapIndex].mapFloats.rotation.z += arg3;
        
        result = TRUE;

    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/map", adjustMapRGBA);

bool adjustMapRGBA(u16 mapIndex, s8 arg1, s8 arg2, s8 arg3, s8 arg4) {

    bool result = FALSE;
    
    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapStruct9.flags & 1)) {
        
        mainMap[mapIndex].mapFloats.groundRgba.r += arg1;
        mainMap[mapIndex].mapFloats.groundRgba.g += arg2;
        mainMap[mapIndex].mapFloats.groundRgba.b += arg3;
        mainMap[mapIndex].mapFloats.groundRgba.a += arg4;
        
        result = TRUE;

    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/map", func_80034738);

bool func_80034738(u16 mapIndex, u8 r, u8 g, u8 b, u8 a, s16 arg5) {

    bool result;

    f32 tempFloat;

    s16 temp = getAbsoluteValue(arg5);

    result = FALSE;

    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapStruct9.flags & 1)) {

        mainMap[mapIndex].mapFloats.defaultRgba.r = r;
        mainMap[mapIndex].mapFloats.defaultRgba.g = g;
        mainMap[mapIndex].mapFloats.defaultRgba.b = b;
        mainMap[mapIndex].mapFloats.defaultRgba.a = a;

        mainMap[mapIndex].mapStruct9.unk_8 = temp;

        mainMap[mapIndex].mapStruct9.flags &= ~8;

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

//INCLUDE_ASM(const s32, "system/map", func_80034A6C);

bool func_80034A6C(u16 mapIndex, s8 arg1, s8 arg2, s8 arg3, u8 arg4, f32 arg5, f32 arg6, f32 arg7) {

    bool result = FALSE; 

    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapStruct9.flags & 1)) { 

        mainMap[mapIndex].mapStruct2.unk_48 += arg1;

        mainMap[mapIndex].mapStruct2.unk_0.y = 0;
        mainMap[mapIndex].mapStruct2.unk_C.y = 0;
        mainMap[mapIndex].mapStruct2.unk_18.y = 0;
        mainMap[mapIndex].mapStruct2.unk_24.y = 0;
        
        mainMap[mapIndex].mapStruct2.unk_49 += arg2;
        mainMap[mapIndex].mapStruct2.unk_4A += arg3;
        mainMap[mapIndex].mapStruct2.unk_4B += arg4;

        mainMap[mapIndex].mapStruct2.unk_3C.x += arg5;
        mainMap[mapIndex].mapStruct2.unk_3C.y += arg6;
        mainMap[mapIndex].mapStruct2.unk_3C.z += arg7;

        mainMap[mapIndex].mapStruct2.unk_0.x += arg1;
        mainMap[mapIndex].mapStruct2.unk_0.z += arg2;

        mainMap[mapIndex].mapStruct2.unk_C.x += arg1;
        mainMap[mapIndex].mapStruct2.unk_C.z += arg2;

        mainMap[mapIndex].mapStruct2.unk_18.x += arg1;
        mainMap[mapIndex].mapStruct2.unk_18.z += arg2;

        mainMap[mapIndex].mapStruct2.unk_24.x += arg1;
        mainMap[mapIndex].mapStruct2.unk_24.z += arg2;
        
        result = TRUE;
    
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/map", func_80034C40);

// map object sprites, holdable items, overlay sprites
bool func_80034C40(u16 mapIndex, u8 index, u16 spriteIndex, u16 arg3, f32 arg4, f32 arg5, f32 arg6, u8 arg7, u8 arg8, u8 arg9, u8 argA) {

    bool result = FALSE; 

    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapStruct9.flags & 1)) {
         
        mainMap[mapIndex].mapObjects[index].spriteIndex = spriteIndex; 
        mainMap[mapIndex].mapObjects[index].unk_E = arg3;

        mainMap[mapIndex].mapObjects[index].coordinates.x = arg4;
        mainMap[mapIndex].mapObjects[index].coordinates.y = arg5;
        mainMap[mapIndex].mapObjects[index].coordinates.z = arg6;

        mainMap[mapIndex].mapObjects[index].unk_10 = arg7;
        mainMap[mapIndex].mapObjects[index].unk_11 = arg8;

        mainMap[mapIndex].mapObjects[index].flags |= 1;
        
        if (arg9) {
            mainMap[mapIndex].mapObjects[index].flags |= 4;
        }

        if (argA) {
            mainMap[mapIndex].mapObjects[index].flags |= 8;
        }

        result = TRUE;
        
        mainMap[mapIndex].mapStruct9.unk_11++;
    }
    
    return result;
}
 
//INCLUDE_ASM(const s32, "system/map", func_80034D64);

bool func_80034D64(u16 mapIndex, u8 index, u16 spriteIndex, u16 arg3) {

    bool result = FALSE;
 
    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapStruct9.flags & 1)) {

        mainMap[mapIndex].weatherSprites[index].spriteIndex = spriteIndex;
        mainMap[mapIndex].weatherSprites[index].unk_E = arg3;
        mainMap[mapIndex].weatherSprites[index].flags = 1;

        result = TRUE;

    }

    return result; 
        
}

//INCLUDE_ASM(const s32, "system/map", func_80034DC8);

bool func_80034DC8(u16 mapIndex, u8 index, u16 arg2) {

    bool result = FALSE; 

    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapStruct9.flags & 1)) { 
        
        func_8002BAD8(mainMap[mapIndex].mapObjects[index].spriteIndex);
        
        mainMap[mapIndex].mapObjects[index].unk_E = arg2;
        mainMap[mapIndex].mapObjects[index].flags &= ~2;

        result = TRUE;

    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/map", deactivateMapObject);

bool deactivateMapObject(u16 mapIndex, u8 index) {

    bool result = FALSE; 

    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapStruct9.flags & 1)) { 

        if (mainMap[mapIndex].mapObjects[index].flags & 2) {
        
            deactivateSprite(mainMap[mapIndex].mapObjects[index].spriteIndex);
        
            mainMap[mapIndex].mapObjects[index].flags = 0;
        
            result = TRUE;

        }
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/map", func_80034EF0);

// load and set texture for map spawnable sprite
// called by level.c
bool func_80034EF0(u16 mapIndex, u8 arg1, u8 arg2, u32* textureIndex, u32* paletteIndex, u8* spriteToPaletteIndex, u32 romTextureStart, u32 arg7, u32 romAssetIndexStart, u32 romAssetIndexEnd, u8 argA) {

    bool result = FALSE;
    
    u32 assetIndex[8];

    u32 offset1;
    u32 offset2;
    u32 offset3;
    u32 offset4;
    u32 offset5;

    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapStruct9.flags & 1)) {
        
        mainMap[mapIndex].groundObjects.textureIndex = textureIndex;
        mainMap[mapIndex].groundObjects.paletteIndex = paletteIndex;
        mainMap[mapIndex].groundObjects.spriteToPaletteIndex = spriteToPaletteIndex;

        // D_801FD624 
        mainMap[mapIndex].groundObjects.unk_10 = arg1;
        // D_801C3F35
        mainMap[mapIndex].groundObjects.unk_11 = arg2;

        mainMap[mapIndex].groundObjects.unk_12 = argA;
        
        nuPiReadRom(romAssetIndexStart, assetIndex, romAssetIndexEnd - romAssetIndexStart);
        
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

//INCLUDE_ASM(const s32, "system/map", func_80035004);

bool func_80035004(u16 mapIndex, u16 arg1, u8 arg2, u8 arg3) {
    
    bool result = FALSE;
    
    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapStruct9.flags & 1)) {

        mainMap[mapIndex].groundObjects.arr2[0][arg3*0x14 + arg2] = arg1;
        result = TRUE;

    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/map", func_80035054);

// used for maps with foragable items
bool func_80035054(u16 mapIndex, u16 bitmapIndex, u16 spriteIndex, f32 x, f32 y, f32 z) {

    bool result = FALSE;

    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapStruct9.flags & 1)) {
    
        if (bitmapIndex) {
            
            setBitmapFormat(&mainMap[mapIndex].mapBitmaps[bitmapIndex], func_80028888(spriteIndex, mainMap[mapIndex].groundObjects.textureIndex), func_800288B8(spriteIndex, mainMap[mapIndex].groundObjects.paletteIndex, mainMap[mapIndex].groundObjects.spriteToPaletteIndex));
              
            mainMap[mapIndex].mapBitmaps[bitmapIndex].unk_1C.x = x; 
            mainMap[mapIndex].mapBitmaps[bitmapIndex].unk_1C.y = y;
            mainMap[mapIndex].mapBitmaps[bitmapIndex].unk_1C.z = z;
            
            result = TRUE;

        }    
    }
    
    return result;
}

INCLUDE_ASM(const s32, "system/map", func_80035150);

INCLUDE_ASM(const s32, "system/map", func_8003544C);

//INCLUDE_ASM(const s32, "system/map", func_80035914);

bool func_80035914(u16 mapIndex, f32 arg1, f32 arg2) {

    Vec3f vec;
    
    bool result = FALSE;

    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapStruct9.flags & 1)) {

        func_800366F4(&vec, 0, arg1, arg2);

        if (vec.y != 65535.0f) {
            result = func_80036880(MAIN_MAP_INDEX, vec.x, vec.z) != 0;
        }

    }
    
    return result;
}
 
//INCLUDE_ASM(const s32, "system/map", func_800359C8);

Vec3f* func_800359C8(Vec3f* arg0, MainMap* map, f32 arg2, f32 arg3) {
    
    Vec3f vec;

    vec.x = (arg2 + map->mapStruct9.unk_0) / map->mapStruct1.unk_8;
    vec.y = 0;
    vec.z = (arg3 + map->mapStruct9.unk_4) / map->mapStruct1.unk_9;

    *arg0 = vec;
    
    return arg0;
}

//INCLUDE_ASM(const s32, "system/map", func_80035A58);

// 3D rotations
void func_80035A58(UnknownMapStruct2* arg0) {

    Vec3f padding[3];
    
    Vec3f vec;
    Vec3f vec2;

    vec.x = -arg0->unk_4A;
    vec.y = 0;
    vec.z = -arg0->unk_4B;

    func_80027950(vec, &arg0->unk_0, arg0->unk_30);

    vec.x = -arg0->unk_4A;
    vec.y = 0;
    vec.z = arg0->unk_4B;

    func_80027950(vec, &arg0->unk_C, arg0->unk_30);

    vec.x = arg0->unk_4A;
    vec.y = 0;
    vec.z = arg0->unk_4B;

    func_80027950(vec, &arg0->unk_18, arg0->unk_30);

    vec.x = arg0->unk_4A;
    vec.y = 0;
    vec.z = -arg0->unk_4B;

    func_80027950(vec, &arg0->unk_24, arg0->unk_30);

    vec2.x = 0.0f;
    vec2.y = 0;
    vec2.z = 0.0f;
    
    func_80027950(vec2, &vec2, arg0->unk_30);
    
    arg0->unk_0.x += arg0->unk_48 + vec2.x;
    arg0->unk_0.y = 0;
    arg0->unk_0.z += arg0->unk_49 + vec2.z;

    arg0->unk_C.x += arg0->unk_48 + vec2.x;
    arg0->unk_C.y = 0;
    arg0->unk_C.z += arg0->unk_49 + vec2.z;

    arg0->unk_18.x += arg0->unk_48 + vec2.x;
    arg0->unk_18.y = 0;
    arg0->unk_18.z += arg0->unk_49 + vec2.z;
    
    arg0->unk_24.x += arg0->unk_48 + vec2.x;
    arg0->unk_24.y = 0;
    arg0->unk_24.z += arg0->unk_49 + vec2.z;
    
    D_802226EC = arg0->unk_0.x - arg0->unk_C.x;
    D_80170460 = arg0->unk_C.x - arg0->unk_18.x;
    D_8013D550 = arg0->unk_18.x - arg0->unk_24.x;
    D_801FB5D4 = arg0->unk_24.x - arg0->unk_0.x;
    
}

//INCLUDE_ASM(const s32, "system/map", func_80035DA4);

bool func_80035DA4(MainMap *map, u8 arg1, u8 arg2) {
    
  bool result = FALSE;

  if (-1.0f <= (((map->mapStruct2.unk_0.z * (map->mapStruct2.unk_C.x - arg1)) + (map->mapStruct2.unk_C.z * (arg1 - map->mapStruct2.unk_0.x))) + (arg2 * D_802226EC))) {
    if (-1.0f <= (((map->mapStruct2.unk_C.z * (map->mapStruct2.unk_18.x - arg1)) + (map->mapStruct2.unk_18.z * (arg1 - map->mapStruct2.unk_C.x))) + (arg2 * D_80170460))) {
      if (-1.0f <= (((map->mapStruct2.unk_18.z * (map->mapStruct2.unk_24.x - arg1)) + (map->mapStruct2.unk_24.z * (arg1 - map->mapStruct2.unk_18.x))) + (arg2 * D_8013D550))) {
        if (-1.0f <= (((map->mapStruct2.unk_24.z * (map->mapStruct2.unk_0.x - arg1)) + (map->mapStruct2.unk_0.z * (arg1 - map->mapStruct2.unk_24.x))) + (arg2 * D_801FB5D4))) {
          result = TRUE;
        }
      }
    }
  }
    
  return result;
}

// unused or inline
INCLUDE_ASM(const s32, "system/map", func_80035EE0);

// mapStruct1, vertices for map additions (house extensions, hot springs, etc.)
INCLUDE_ASM(const s32, "system/map", func_800360BC);

// returns index for interactable object/exit from rodata array per level
INCLUDE_ASM(const s32, "system/map", func_80036318);

#ifdef PERMUTER
bool func_80036490(u16 mapIndex, u8 arg1, f32 arg2, f32 arg3) {

    Vec3f vec1;
    Vec3f vec2;

    u8 temp1;
    u8 temp2;

    u8 *ptr;
    
    bool result = FALSE;

    if (mapIndex == MAIN_MAP_INDEX && mainMap[mapIndex].mapStruct9.flags & 1) {

        vec2.x = ((arg2 + mainMap[mapIndex].mapStruct9.unk_0) / mainMap[mapIndex].mapStruct1.unk_8);
        vec2.y = 0;
        vec2.z = ((arg3 + mainMap[mapIndex].mapStruct9.unk_4) / mainMap[mapIndex].mapStruct1.unk_9);

        vec1 = vec2;

        ptr = (u8*)&mainMap[mapIndex].unk_8[0]+4;
        
        ptr[(mainMap[mapIndex].mapStruct1.unk_A * (u8)vec1.z) + (u8)vec1.x] = arg1;

        result = TRUE;
        
    }

    return result;
    
}
#else
INCLUDE_ASM(const s32, "system/map", func_80036490);
#endif

//INCLUDE_ASM(const s32, "system/map", func_80036610);

// get map coordinates for sprites
Vec3f *func_80036610(Vec3f *arg0, u16 mapIndex, f32 arg2, f32 arg3) {

    Vec3f vec;
    Vec3f vec2;

    vec.x = 0.0f;
    vec.z = 0.0f;
    vec.y = 65535.0f;

    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapStruct9.flags & 1)) {
        
        vec2.y = 0;
        vec2.x = (arg2 + mainMap[mapIndex].mapStruct9.unk_0) / mainMap[mapIndex].mapStruct1.unk_8;
        vec2.z = (arg3 + mainMap[mapIndex].mapStruct9.unk_4) / mainMap[mapIndex].mapStruct1.unk_9;

        vec = vec2; 
    }
 
    *arg0 = vec;
    
    return arg0;
}

//INCLUDE_ASM(const s32, "system/map", func_800366F4);

Vec3f* func_800366F4(Vec3f* arg0, u16 mapIndex, f32 arg2, f32 arg3) {

    Vec3f vec;
    Vec3f vec2;

    vec.x = 0.0f;
    vec.y = 0.0f;
    vec.z = 0.0f;

    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapStruct9.flags & 1)) {
        
        vec2.x = (arg2 + mainMap[mapIndex].mapStruct9.unk_0) / mainMap[mapIndex].mapStruct1.unk_8;
        vec2.y = 0;
        vec2.z = (arg3 + mainMap[mapIndex].mapStruct9.unk_4) / mainMap[mapIndex].mapStruct1.unk_9;
        
        vec = vec2;
        
        if (mainMap[mapIndex].groundObjects.unk_10 < vec.x && mainMap[mapIndex].groundObjects.unk_11 < vec.z && vec.x < (mainMap[mapIndex].groundObjects.unk_10 + 0x14) && vec.z < (mainMap[mapIndex].groundObjects.unk_11 + 0x18)) {
            vec.x -= mainMap[mapIndex].groundObjects.unk_10;
            vec.z -= mainMap[mapIndex].groundObjects.unk_11;
        } else {
            vec.y = 65535.0f;
        }
    }

    *arg0 = vec;

    return arg0;
    
}

//INCLUDE_ASM(const s32, "system/map", func_80036880);

u16 func_80036880(u16 mapIndex, f32 arg1, f32 arg2) {

    u32 padding[4];
    
    u16 result = 0;
    
    if (mapIndex == MAIN_MAP_INDEX && mainMap[mapIndex].mapStruct9.flags & 1) {

        result = mainMap[mapIndex].groundObjects.arr2[0][(u8)arg2*0x14+(u8)arg1];        
        
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/map", func_80036980);

bool func_80036980(u16 mapIndex, u16 arg1, f32 arg2, f32 arg3) {
    
    u32 padding[4];

    bool result = FALSE;
    
    if (mapIndex == MAIN_MAP_INDEX && mainMap[mapIndex].mapStruct9.flags & 1) {

        mainMap[mapIndex].groundObjects.arr2[0][(u8)arg3*0x14+(u8)arg2] = arg1;    
        result = TRUE;
        
    }

    return result;

}

//INCLUDE_ASM(const s32, "system/map", func_80036A84);

// check bit 3 on mapStruct9 flags
bool func_80036A84(u16 mapIndex) {

    bool result = FALSE;
     
    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapStruct9.flags & 1)) {
        result = (mainMap[mapIndex].mapStruct9.flags >> 3) & 1;
    }

    return result;
}

//INCLUDE_ASM(const s32, "system/map", func_80036AB4);

void func_80036AB4(MainMap* map) {

    u16 i;
    u16 j;
    u16 count;
    u8* ptr;
    u16 temp;
    WeatherSprite weatherSprites[32]; 

    count = 0;
    i = 0;
  
    if (map->mapStruct1.unk_6) {
        
        do {

            // func_80037650 = get vertex info object from vertex info object bank + (offset)
            ptr = func_80037614(&map->vtxs[i], func_80037650(i, map->unk_0));

            j = 0;
            
            while (1) {

                ptr =  func_800374C0(&weatherSprites[j], ptr);
    
                if (weatherSprites[j].flags & 0x80) {
                    map->vtxs[i].flags |= 0x80;
                }

                if (weatherSprites[j++].flags & 0x10) {
                    break;
                } 

            } 

            map->vtxs[i].currentVtxIndex = count;
            temp = func_80037F08(&map->displayLists[count], map, weatherSprites);

            count += temp;
            map->vtxs[i].vtxCount = temp;
            
            i++;
            
        } while (i < map->mapStruct1.unk_6);
    }  
} 

INCLUDE_ASM(const s32, "system/map", func_80036C08);

INCLUDE_ASM(const s32, "system/map", func_80036FA0);

INCLUDE_ASM(const s32, "system/map", func_80037254);

#ifdef PERMUTER
void func_800372F0(UnknownMapStruct1* arg0, Unknown1* arg1) {

    Unknown1 struc;
    
    arg0->unk_8 = arg1->unk_4;
    arg0->unk_9 = arg1->unk_5;
    arg0->unk_A = arg1->unk_6;
    arg0->unk_B = arg1->unk_7;
    
    arg0->unk_4 = arg1->unk_9 << 8 | arg1->unk_8;
    arg0->unk_6 = arg1->unk_B << 8 | arg1->unk_A;
    
    arg0->ptr = arg1+1;

}
#else
INCLUDE_ASM(const s32, "system/map", func_800372F0);
#endif

//INCLUDE_ASM(const s32, "system/map", func_80037350);

// unused or inline
// arg0 = struct
u8* func_80037350(UnknownVertexStruct* arg0, u8* arg1) {

    // arg1 = &*(arg1+4);
    // skip header
    arg1 = &arg1[4];
    
    arg0->arr2[0] = *arg1;
    
    arg1++;

    arg0->arr2[1] = *arg1;
    
    arg1++;
    
    arg0->arr2[2] = *arg1;
    
    arg1++;
    
    arg0->arr2[3] = *arg1;
    
    arg1++;

    return arg1;
    
}

// inline (called by func_80037400 and func_800374C0)
#ifdef PERMUTER
u8* func_80037388(UnknownVertexStruct* arg0, u8* arg1, u8 arg2) {

    UnknownVertexStruct struc;

    u8 temp;

    arg0->arr[0] = *arg1;
    arg1++;
    
    arg0->arr2[0] = *arg1;
    arg1++;
    
    arg0->arr3[0] = *arg1;
    arg1++;

    temp = *arg1;

    arg1++;

    arg0->unk_0 = temp | (*arg1 << 8);
    
    arg1++;

    if (arg2) {

        temp = *arg1;

        arg1++;

        arg0->unk_2 = temp | (*arg1 << 8);

        arg1++;
    }
    
    return arg1;
}
#else
INCLUDE_ASM(const s32, "system/map", func_80037388);
#endif

INCLUDE_ASM(const s32, "system/map", func_80037400);

INCLUDE_ASM(const s32, "system/map", func_800374C0);

//INCLUDE_ASM(const s32, "system/map", func_80037614);

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

//INCLUDE_ASM(const s32, "system/map", func_80037650);

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

//INCLUDE_ASM(const s32, "system/map", func_80037668);

u32 func_80037668(MainMap* map, u16 vtxIndex, f32 arg2, f32 arg3, f32 arg4) {

    u8 count = 0;
    u16 count2;
    s8* vtx;

    vtx = map->vtxs[vtxIndex].vtx;

    count2 = map->mapStruct9.unk_C + map->mapStruct9.unk_A; 
    
    if (map->vtxs[vtxIndex].count) {
        
        do {
            
            mapVertices[gDisplayContextIndex][count2].v.ob[0] = *vtx + arg2;
            vtx++;
            mapVertices[gDisplayContextIndex][count2].v.ob[1] = *(u8*)vtx + arg3;
            vtx++;
            mapVertices[gDisplayContextIndex][count2].v.ob[2] = *vtx + arg4;
            vtx++;

            mapVertices[gDisplayContextIndex][count2].v.cn[0] = map->mapFloats.groundRgba.r;
            mapVertices[gDisplayContextIndex][count2].v.cn[1] = map->mapFloats.groundRgba.g;
            mapVertices[gDisplayContextIndex][count2].v.cn[2] = map->mapFloats.groundRgba.b;
            mapVertices[gDisplayContextIndex][count2].v.cn[3] = map->mapFloats.groundRgba.a;
            
            count2++;
            count++;
            
        } while (count < map->vtxs[vtxIndex].count);
    }

    return count;
    
}

//INCLUDE_ASM(const s32, "system/map", func_8003797C);

Gfx* func_8003797C(Gfx* dl, MainMap* map, u8 arg2) {

    Tile bitmap;

    setBitmapFormat((Bitmap*)&bitmap, func_80028888(arg2, map->unk_10), func_800288A0(arg2, map->unk_14));

    gDPLoadTextureTile_4b(dl++, bitmap.timg, bitmap.fmt, bitmap.width, bitmap.height, 0, 0, bitmap.width - 1, bitmap.height - 1, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
    gDPLoadTLUT_pal16(dl++, 0, bitmap.pal);

    return dl++;
}

// uses gSPVertex
// calls func_800383B0
INCLUDE_ASM(const s32, "system/map", func_80037BC4);

//INCLUDE_ASM(const s32, "system/map", func_80037DF0);

inline Gfx* func_80037DF0(Gfx* dl, MainMap* map, u16 arg2) {

    u8 i;
    u16 temp;
    
    // width
    map->mapStruct9.unk_A = 0;
    // height
    map->mapStruct9.unk_C = arg2;

    *dl = D_8011ED68[0]; 
    dl++;
    *dl = D_8011ED68[1]; 
    dl++;
    *dl = D_8011ED68[2]; 
    dl++;

    for (i = 0; i < 0x51; i++) {
  
        temp = map->unkArr[i];
        
        if (temp != 0xFFFF) {
            dl = func_80037BC4(dl, map, temp, i);
        }

    } 

    *dl = D_8011ED68[3];

    map->mapStruct9.height = map->mapStruct9.unk_A;

    dl++;
    
    return dl;
    
} 

// vertex coloring
INCLUDE_ASM(const s32, "system/map", func_80037F08);

//INCLUDE_ASM(const s32, "system/map", func_800383B0);

Gfx* func_800383B0(Gfx* arg0, MainMap* map, u16 vtxIndex, f32 arg3, f32 arg4, f32 arg5) {

    u16 i;
    
    u32 count;
    
    Gfx* dl;
    Gfx dl2;

    // FIXME: shouldn't be necessary
    // ??
    f32 temp;
    u32 padding[2];

    temp = arg4;

    // FIXME:
    // ??
    count = func_80037668(map, vtxIndex, arg3, *(f32*)&arg4, arg5);

    gSPVertex(&dl2 + 1, &mapVertices[gDisplayContextIndex][map->mapStruct9.unk_C + map->mapStruct9.unk_A], map->vtxs[vtxIndex].count, 0);

    // FIXME:
    dl2 = *(&dl2 + 1);
    *arg0++ = dl2;
    
    map->mapStruct9.unk_A += count;

    dl = &map->displayLists[map->vtxs[vtxIndex].currentVtxIndex];
    
    for (i = 0; i < map->vtxs[vtxIndex].vtxCount; i++) {
        *arg0++ = *dl++;
    }

    return arg0++;
}

void func_80038514(void) {}

//INCLUDE_ASM(const s32, "system/map", func_8003851C);

void func_8003851C(MainMap* map) {

    u16 i, j;
    u8 temp;
    u16 count = 0;
    u8 *ptr;
    
    count = 0;
    i = 0;
    
    temp = *(map->unk_C+4);
    map->mapStruct9.unk_10 = temp;
    
    if (temp) {
        
        do {

            ptr = map->unk_C+4;

            ptr = func_800386D4(i, ptr);

            func_800386C0(&map->mapStruct3[i].unk_0, ptr);

            map->mapStruct3[i].unk_0 = count;
            j = 0;
            
            ptr += 2;
            
            if (map->mapStruct3[i].unk_3) {
                do {
                    func_80038630(&D_80181BA0[count], ptr);
                    ptr += 7;
                    count++;
                    j++;
                }  while (j < map->mapStruct3[i].unk_3);
            }
            
            i++;
            
        } while (i < map->mapStruct9.unk_10);
    }
    
}

//INCLUDE_ASM(const s32, "system/map", func_80038630);

// byteswapped s16s converted to floats
void func_80038630(DecompressedVec3f* arg0, CompressedVec3f* arg1) {

    // FIXME: shouldn't be necessary/likely a union for byte-swapping
    u32 padding[8];
    
    arg0->flags = arg1->flags;
    arg0->x = (s16)(arg1->part1 | (arg1->part2 << 8));
    arg0->y = (s16)(arg1->part3 | (arg1->part4 << 8));
    arg0->z = (s16)(arg1->part5 | (arg1->part6 << 8));

}

//INCLUDE_ASM(const s32, "system/map", func_800386C0);

void func_800386C0(UnknownMapStruct3* arg0, u8* arg1) {
    arg0->unk_2 = arg1[0];
    arg0->unk_3 = arg1[1];
}

// arg1 = ptr from mainMap.unk_C = ptr to byteswapped shorts for vec3f conversion
#ifdef PERMUTER
u8* func_800386D4(u16 arg0, u8* arg1) {

    u16 i;
    u8 arr[4];

    arg1++;

    for (i = 0; i < arg0; i++) {
        arr[2] = arg1[0];
        arg1 += arg1[1] * 7;
        arr[3] = 2;
   }

    return arg1;

}
#else
INCLUDE_ASM(const s32, "system/map", func_800386D4);
#endif

//INCLUDE_ASM(const s32, "system/map", func_80038728);

// arg0 = stack array
// arg1 = map.unk_4
// arg2 = offset
void func_80038728(u8 arg0[], u8* arg1, u16 arg2) {

    u8* arr;

    // skip header
    arr = (arg1 + 4);

    // get offset
    // 0x13 = sizeof arrays
    arr += (arg2*0x13);

    arg0[0] = arr[0];
    arg0[1] = arr[1];
    arg0[2] = arr[2];
    arg0[3] = arr[3];
    arg0[4] = arr[4];
    arg0[5] = arr[5];
    arg0[6] = arr[6];
    arg0[7] = arr[7];
    arg0[8] = arr[8];
    arg0[9] = arr[9];
    arg0[10] = arr[10];
    arg0[11] = arr[11];
    arg0[12] = arr[12];
    arg0[13] = arr[13];
    arg0[14] = arr[14];
    arg0[15] = arr[15];
    arg0[16] = arr[16];
    arg0[17] = arr[17];
    arg0[18] = arr[18];
    
}
 
//INCLUDE_ASM(const s32, "system/map", func_800387E0);

u32* func_800387E0(u16 arg0, u32* arg1) {
    return (u8*)arg1 + arg1[arg0];
}

//INCLUDE_ASM(const s32, "system/map", func_800387F8);

u32* func_800387F8(u16 arg0, u32* arg1) {
    return (u8*)arg1 + arg1[arg0];
}

//INCLUDE_ASM(const s32, "system/map", func_80038810);

bool func_80038810(u16 mapIndex) {

    bool result;

    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapStruct9.flags & 1)) {
        if (!(mainMap[mapIndex].mapStruct9.flags & 2)) {
            mainMap[mapIndex].mapStruct9.flags |= 2;
            func_80036FA0(MAIN_MAP_INDEX);
        }
    }

    // never initialized; whoops
    return result;

}

//INCLUDE_ASM(const s32, "system/map", func_8003886C);

bool func_8003886C(u16 mapIndex) {
    
    bool result  = 0;

    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapStruct9.flags & 1)) {
        mainMap[mapIndex].mapStruct9.flags &= ~2;
        result = TRUE;
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/map", func_800388A4);

// FIXME: likely uses inlines
// param_2 = mainMap.unk_20 (set from gMapModelContext in func_80033A90, called by func_8003BC50)
// map additions
// param1 = mapAdditions.unk_44, set from sprite vec.x
// param2 = mainMap.unk_20
u8* func_800388A4(u16 arg0, u8 *arg1) {
    
    u32 temp1;
    u32 temp2;

    u16 i;
    
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

//INCLUDE_ASM(const s32, "system/map", func_80038900);

// called by func_800735FC
bool func_80038900(u16 mapIndex, u16 arg1, u16 arg2, u16 arg3, u16 arg4) {

    bool result = FALSE;

    if (mapIndex == MAIN_MAP_INDEX && mainMap[mapIndex].mapStruct9.flags & 1) {
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

//INCLUDE_ASM(const s32, "system/map", func_80038990);

bool func_80038990(u16 mapIndex, u16 mapAdditionIndex, u8 arg2) {

    bool result = FALSE;

    if (mapIndex == MAIN_MAP_INDEX && mainMap[mapIndex].mapStruct9.flags & 1 && mapAdditionIndex < 0x20) {

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

//INCLUDE_ASM(const s32, "system/map", func_80038A2C);

bool func_80038A2C(u16 mapIndex, u16 mapAdditionIndex, u8 arg2, u8 arg3) {

    bool result = FALSE;

    if (mapIndex == MAIN_MAP_INDEX && mainMap[mapIndex].mapStruct9.flags & 1 && mapAdditionIndex < 0x20) {

        if (!(mainMap[mapIndex].mapAdditions[mapAdditionIndex].flags & 1)) {
            
            mainMap[mapIndex].mapAdditions[mapAdditionIndex].flags = 0;
            mainMap[mapIndex].mapAdditions[mapAdditionIndex].unk_40 = 0;
            mainMap[mapIndex].mapAdditions[mapAdditionIndex].unk_42 = 0;
            mainMap[mapIndex].mapAdditions[mapAdditionIndex].unk_44 = arg2;
            mainMap[mapIndex].mapAdditions[mapAdditionIndex].unk_45 = arg3;

            mainMap[mapIndex].mapAdditions[mapAdditionIndex].flags |= 3;
            
            result = TRUE;

        }
        
    }
    
    return result;

}

//INCLUDE_ASM(const s32, "system/map", func_80038AE0);

// load map additions (house extensions, hot springs, etc.)
bool func_80038AE0(u16 mapIndex, u16 mapAdditionIndex) {

    bool result = FALSE;

    if (mapIndex == MAIN_MAP_INDEX && mainMap[mapIndex].mapStruct9.flags & 1) {

        func_800360BC(MAIN_MAP_INDEX, mainMap[mapIndex].mapAdditions[mapAdditionIndex].arr1[0], mainMap[mapIndex].mapAdditions[mapAdditionIndex].unk_44, mainMap[mapIndex].mapAdditions[mapAdditionIndex].unk_45, 0);
        
        result = TRUE;

    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "system/map", func_80038B58);

bool func_80038B58(u16 mapIndex, u16 mapAdditionIndex, u8 arg2, u8 arg3) {
    
    bool result  = 0;

    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapStruct9.flags & 1)) {
        func_800360BC(MAIN_MAP_INDEX, mainMap[mapIndex].mapAdditions[mapAdditionIndex].arr1[0], arg2 , arg3, 2);
        result = TRUE;
    }
    
    return result;
}

INCLUDE_ASM(const s32, "system/map", func_80038BC4);

INCLUDE_ASM(const s32, "system/map", func_800393E0);

INCLUDE_ASM(const s32, "system/map", func_80039990);

//INCLUDE_ASM(const s32, "system/map", func_80039E00);

//INCLUDE_ASM(const s32, "system/map", func_80039E20);

void func_80039E20(u16 arg0, Gfx* arg1) {

    u16 temp = func_8002929C(arg1, 0x28);
    
    func_800292EC(temp, mainMap[arg0].mapFloats.translation.x + mainMap[arg0].mapStruct2.unk_3C.x, 
        mainMap[arg0].mapFloats.translation.y + mainMap[arg0].mapStruct2.unk_3C.y, 
        mainMap[arg0].mapFloats.translation.z + mainMap[arg0].mapStruct2.unk_3C.z);

    func_80029330(temp, mainMap[arg0].mapFloats.scale.x, mainMap[arg0].mapFloats.scale.y, mainMap[arg0].mapFloats.scale.z);
    
    func_80029374(temp, mainMap[arg0].mapFloats.rotation.x, mainMap[arg0].mapFloats.rotation.y, mainMap[arg0].mapFloats.rotation.z);
}

//INCLUDE_ASM(const s32, "system/map", func_80039F58);

void func_80039F58(u16 mapIndex) {

    bool found = 0;
    bool set = FALSE;
    u16 i = 0;

    do {

        if (mainMap[mapIndex].mapAdditions[i].flags & 1 && !set) {
        
            do {

                if (mainMap[mapIndex].mapAdditions[i].unk_40 == 0) { 

                    if (mainMap[mapIndex].mapAdditions[i].arr1[mainMap[mapIndex].mapAdditions[i].unk_42] != 0xFFFF) {
 
                        func_800360BC(mapIndex, mainMap[mapIndex].mapAdditions[i].arr1[mainMap[mapIndex].mapAdditions[i].unk_42], mainMap[mapIndex].mapAdditions[i].unk_44, mainMap[mapIndex].mapAdditions[i].unk_45, mainMap[mapIndex].mapAdditions[i].flags & 2);
                        found = 1;
                        
                    } else {
                        
                        if (mainMap[mapIndex].mapAdditions[i].flags & 4) {
                            
                            mainMap[mapIndex].mapAdditions[i].unk_40 = 0;
                            mainMap[mapIndex].mapAdditions[i].unk_42 = 0;
                            
                            func_800360BC(mapIndex, mainMap[mapIndex].mapAdditions[i].arr1[mainMap[mapIndex].mapAdditions[i].unk_42], mainMap[mapIndex].mapAdditions[i].unk_44, mainMap[mapIndex].mapAdditions[i].unk_45, mainMap[mapIndex].mapAdditions[i].flags & 2);
                            found = 1;
                            
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

static inline u8 handleRgba(u16 i) {

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
    
    mainMap[i].mapStruct2.unk_30.x = 0.0f;
    mainMap[i].mapStruct2.unk_30.y = 360.0f - currentWorldRotationAngles.y;
    mainMap[i].mapStruct2.unk_30.z = 0.0f;

    if (mainMap[i].mapStruct2.unk_30.y < 0.0f) {
        mainMap[i].mapStruct2.unk_30.y += 360.0f;
    }

    if (mainMap[i].mapStruct2.unk_30.y >= 360.0f) {
        mainMap[i].mapStruct2.unk_30.y -= 360.0f;
    }
    
}


//INCLUDE_ASM(const s32, "system/map", func_8003A1BC);

// main loop function
void func_8003A1BC(void) {

    u16 height = 0;
    u16 i;
    
    Gfx *dl = D_801A8B60[gDisplayContextIndex];
    Gfx *tempDl;
    
    // FIXME: shoudn't be necessary
    u32 padding[4];
    
    for (i = 0; i < MAX_MAPS; i++) {

        if ((mainMap[i].mapStruct9.flags & 1) && (mainMap[i].mapStruct9.flags & 2)) {
            
            func_80039F58(i);

            if (!handleRgba(i)) {
                mainMap[i].mapStruct9.flags |= 8;
            } else {
                mainMap[i].mapStruct9.flags &= ~8;
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
            
            tempDl = dl;
            
            dl = func_80037DF0(tempDl, &mainMap[i], height);
            
            func_80038BC4(&mainMap[i]);
            func_800393E0(&mainMap[i]);
            func_80039990(&mainMap[i]);
            
            func_80039E20(i, tempDl);

            // append spawnable sprites to display list
            func_8003B1BC(&mainMap[i]);

            height += mainMap[i].mapStruct9.height;

        }
        
    }

    if (dl - D_801A8B60[gDisplayContextIndex] >= 0x1B00) {
        // FIXME: replace with string literals
        __assert(D_8011EDB0, D_8011EDB4, 3912);
    }

}

//INCLUDE_ASM(const s32, "system/map", func_8003AC14);

inline void func_8003AC14(Gfx* dl, MapBitmap* arg1) {
    
    // gsDPSetCombineMode(G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA)
    // gsDPSetRenderMode(G_RM_TEX_EDGE, G_RM_TEX_EDGE2)
    // gsSPTexture(qu016(0.5), qu016(0.5), 0, G_TX_RENDERTILE, G_ON)
    // gsDPSetCombineMode(G_CC_MODULATEIA, G_CC_MODULATEIA)
     
    *dl = D_8011ED88[1];
    dl++;
    *dl = D_8011EDC8[0];
    dl++;
    *dl = D_8011EDC8[1];
    dl++;
    *dl = D_8011ED68[2];
    dl++;

    func_80026F88(dl++, (Bitmap*)arg1, 0, arg1->height);
    
}
 
// param1: Gfx* 8020DE78
// param2: MainMap*
// param3: mapBitmap*, 8014318C
// param4 = offset into map vertex array
//INCLUDE_ASM(const s32, "system/map", func_8003ACA8);

Gfx* func_8003ACA8(Gfx* arg0, MainMap* map, MapBitmap* arg2, u16 vtxIndex) {

    Gfx dl[2];
      
    func_800276AC((Vtx*)&D_80165500[gDisplayContextIndex][vtxIndex], arg2->width, arg2->height, arg2->height, 0, 0,
        0, 0, 0, (0x10 | 0x40 | 0x100), map->mapFloats.groundRgba.r, map->mapFloats.groundRgba.g, map->mapFloats.groundRgba.b,
        map->mapFloats.groundRgba.a);
 
    gSPVertex(&dl[1], &D_80165500[gDisplayContextIndex][vtxIndex][0], 4, 0);

    *dl = *(dl+1);
    *arg0 = *dl;
    
    arg0++;
    *arg0 = D_8011EDD8;
    arg0++;
    *arg0 = D_8011EDA8;
    arg0++;
    *arg0 = D_8011ED80;
    arg0++;
    
    return arg0++;
    
}

//INCLUDE_ASM(const s32, "system/map", func_8003AF58);

Vec3f* func_8003AF58(Vec3f* arg0, u16 arg1, u8 arg2, u8 arg3) {

    Vec3f vec;

    f32 temp1;
    f32 temp2;

    // these two statements have to be on the same line to match lmao (otherwise compiler inserts a nop)
    temp1 = mainMap[arg1].mapStruct1.unk_8; vec.x = ((arg2 *  temp1) - mainMap[arg1].mapStruct9.unk_0) + (mainMap[arg1].mapStruct1.unk_8 / 2);

    temp2 = mainMap[arg1].mapStruct1.unk_9; vec.z = ((arg3 * temp2) - mainMap[arg1].mapStruct9.unk_4) + (mainMap[arg1].mapStruct1.unk_9 / 2);
    
    vec.y = func_80035150(MAIN_MAP_INDEX, vec.x, vec.z);
    
    if (arg2 < mainMap[arg1].groundObjects.unk_10 || arg3 < mainMap[arg1].groundObjects.unk_11 || arg2 >= mainMap[arg1].groundObjects.unk_10 + 0x14 || arg3 >= mainMap[arg1].groundObjects.unk_11 + 0x18) {
        vec.y = 65535.0f;
    }
    
    *arg0 = vec;

    return arg0;
    
}

//INCLUDE_ASM(const s32, "system/map", func_8003B100);

void func_8003B100(MainMap* map, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, Gfx* dl) {

    Vec3f vec;
    
    f32 temp_f20;
    f32 temp_f22;
    f32 temp_f24;

    temp_f22 = (arg1 + map->mapStruct9.unk_0 + map->mapStruct2.unk_3C.x) - arg4;
    temp_f24 = arg2 + map->mapStruct2.unk_3C.y;
    temp_f20 = (arg3 + map->mapStruct9.unk_4 + map->mapStruct2.unk_3C.z) - arg5;
    
    func_800292EC(func_8002929C(dl, 8), temp_f22, temp_f24, temp_f20);

}

// check if map with spawnable items and append to display lists
INCLUDE_ASM(const s32, "system/map", func_8003B1BC);


// have to remove all references to D_8011ED6C, D_8011ED74, etc. before using

// static const Gfx D_8011ED68[4] = {

//     gsDPSetCombineMode(G_CC_MODULATEIA, G_CC_MODULATEIA),
//     gsDPSetRenderMode(G_RM_RA_ZB_OPA_SURF, G_RM_RA_ZB_OPA_SURF2),
//     gsDPSetTextureFilter(G_TF_BILERP),
//     gsSPEndDisplayList()

// };

// gsDPSetCombineMode(G_CC_MODULATEIA, G_CC_MODULATEIA)

INCLUDE_RODATA(const s32, "system/map", D_8011ED68);

INCLUDE_RODATA(const s32, "system/map", D_8011ED6C);

// gsDPSetRenderMode(G_RM_RA_ZB_OPA_SURF, G_RM_RA_ZB_OPA_SURF2)

INCLUDE_RODATA(const s32, "system/map", D_8011ED70);

INCLUDE_RODATA(const s32, "system/map", D_8011ED74);

// gsDPSetTextureFilter(G_TF_BILERP)

INCLUDE_RODATA(const s32, "system/map", D_8011ED78);

INCLUDE_RODATA(const s32, "system/map", D_8011ED7C);

// gsSPEndDisplayList()

INCLUDE_RODATA(const s32, "system/map", D_8011ED80);

INCLUDE_RODATA(const s32, "system/map", D_8011ED84);


// static const Gfx D_8011ED88[5] = {

//     gsSPTexture(0, 0, 0, G_TX_RENDERTILE, G_ON),
//     gsDPSetCombineMode(G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA),
//     gsSPTexture(0, 0, 0, G_TX_RENDERTILE, G_OFF),
//     gsDPSetCombineLERP(PRIMITIVE, 0, SHADE, 0, PRIMITIVE, 0, SHADE, 0, PRIMITIVE, 0, SHADE, 0, PRIMITIVE, 0, SHADE, 0),
//     gsDPPipeSync()

// };

// gsSPTexture(0, 0, 0, G_TX_RENDERTILE, G_ON),

INCLUDE_RODATA(const s32, "system/map", D_8011ED88);

INCLUDE_RODATA(const s32, "system/map", D_8011ED8C);

// gsDPSetCombineMode(G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA)

INCLUDE_RODATA(const s32, "system/map", D_8011ED90);

INCLUDE_RODATA(const s32, "system/map", D_8011ED94);

// gsSPTexture(0, 0, 0, G_TX_RENDERTILE, G_OFF)

INCLUDE_RODATA(const s32, "system/map", D_8011ED98);

INCLUDE_RODATA(const s32, "system/map", D_8011ED9C);

// gsDPSetCombineMode
// gsDPSetCombineLERP(PRIMITIVE, 0, SHADE, 0, PRIMITIVE, 0, SHADE, 0, PRIMITIVE, 0, SHADE, 0, PRIMITIVE, 0, SHADE, 0)

INCLUDE_RODATA(const s32, "system/map", D_8011EDA0);

INCLUDE_RODATA(const s32, "system/map", D_8011EDA4);

// gsDPPipeSync()

INCLUDE_RODATA(const s32, "system/map", D_8011EDA8);

INCLUDE_RODATA(const s32, "system/map", D_8011EDAC);


INCLUDE_RODATA(const s32, "system/map", D_8011EDB0);

INCLUDE_RODATA(const s32, "system/map", D_8011EDB4);


// static const Gfx D_8011EDC8[3] = {

//     gsDPSetRenderMode(G_RM_TEX_EDGE, G_RM_TEX_EDGE2),
//     gsSPTexture(qu016(0.5), qu016(0.5), 0, G_TX_RENDERTILE, G_ON),
//     gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0)

// };

// gsDPSetRenderMode(G_RM_TEX_EDGE, G_RM_TEX_EDGE2)

INCLUDE_RODATA(const s32, "system/map", D_8011EDC8);

INCLUDE_RODATA(const s32, "system/map", D_8011EDCC);

// gsSPTexture(qu016(0.5), qu016(0.5), 0, G_TX_RENDERTILE, G_ON)

INCLUDE_RODATA(const s32, "system/map", D_8011EDD0);

INCLUDE_RODATA(const s32, "system/map", D_8011EDD4);

// gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0)

INCLUDE_RODATA(const s32, "system/map", D_8011EDD8);

INCLUDE_RODATA(const s32, "system/map", D_8011EDDC);
