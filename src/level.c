#include "common.h"

#include "level.h"

#include "system/map.h"
#include "system/memory.h"
#include "system/sprite.h"
#include "system/tiles.h"

#include "game.h"
#include "mapObjects.h"                                                                        
#include "weather.h"      
                             
// likely bss
extern u8 previousEntranceIndex;
extern u8 gEntranceIndex;
extern u8 gBaseMapIndex;
extern u8 gMapWithSeasonIndex;
extern u8 D_8021E6D0;

extern void* D_D86D90;
extern void* D_D8B140;
extern void* D_D8B140_2;
extern void* D_D8B160;
extern void* D_D8B160_2;
extern void* D_D8B1D0;

// rodata
extern u8 levelFlags[];
// map indices for level interactions
extern u8 D_80114280[];
extern u8 D_801147C0[];
extern u8 D_8011FB28[];
extern Vec D_8011FB70[6];

// shared with player.c and setCutscenes.c
extern u8 D_8021E6D0;

//INCLUDE_ASM(const s32, "level", setEntrance);

void setEntrance(u16 index) {
    u16 temp = gEntranceIndex;
    gEntranceIndex = index;
    previousEntranceIndex = temp;
}

//INCLUDE_ASM(const s32, "level", func_8006E840);

void func_8006E840(u16 arg0) {

    if (gBaseMapIndex != 0xFF) {
        func_8003C504(0);
    }

    previousMapIndex = gBaseMapIndex;
    gBaseMapIndex = D_801147C0[arg0];
    gMapWithSeasonIndex = gBaseMapIndex;

    if (getLevelFlags(gMapWithSeasonIndex) & 8) {
        gMapWithSeasonIndex = (gSeason + 0xFF) + gMapWithSeasonIndex;
    }

    func_8003BB14(0, gMapWithSeasonIndex);

    if (gBaseMapIndex == FARM) {
        func_8003C084(0, (D_80114280[gMapWithSeasonIndex] + D_8021E6D0) % 8);
    } else {
        func_8003C084(0, D_80114280[gMapWithSeasonIndex] & 7);
    }
     
    func_8003C1E0(0, 0, 0, 0, 8, 8);
    func_8009C054(gBaseMapIndex);
    setLevelGraphicsData(gBaseMapIndex);
    func_8002EEA4(0);
    
}

//INCLUDE_ASM(const s32, "level", setLevelGraphicsData);

void setLevelGraphicsData(u16 mapIndex) {

    // set tile flag
    func_8003BD60(0);
    
    func_8006EC58(mapIndex);
    func_800735FC(mapIndex);

    // load map object sprites
    func_8006F938(mapIndex);

    func_800746B4(mapIndex);

    func_800D9600(gBaseMapIndex);
    
    if (getLevelFlags(mapIndex) & OUTDOORS) {
        // load rain/snow sprites
        func_800DC360();
    }
    
    // tiling
    func_80036C08(0);

    func_80036FA0(0);
}
 
INCLUDE_ASM(const s32, "level", func_8006EA44);

INCLUDE_ASM(const s32, "level", func_8006EB94);

//INCLUDE_ASM(const s32, "level", getLevelFlags);

u8 getLevelFlags(u16 mapIndex) {
    return levelFlags[mapIndex];
}

INCLUDE_ASM(const s32, "level", func_8006EC58);

INCLUDE_ASM(const s32, "level", func_8006F938);

//INCLUDE_ASM(const s32, "level", func_80073244);

void func_80073244(u8 arg0) {

    Vec3f arr[6];

    MemoryRead_32 *ptr = arr;
    MemoryRead_32 *ptr2 = D_8011FB28;

    do {
        *(Aligned32*)ptr = *(Aligned32*)ptr2;
        ptr2++;
        ptr++;
    } while (ptr2 != (D_8011FB28 + 0x40));    

    *(Vec2f*)ptr = *(Vec2f*)ptr2;

    func_8002B138(arg0 +  0x62, &D_D86D90, &D_D8B140, &D_D8B140_2, &D_D8B160, &D_D8B160_2, &D_D8B1D0, (void*)0x802E4000, (void*)0x802E4D00, (void*)0x802E7400, (void*)0x802E7700, (void*)0x802E7A00, (void*)0x802E7B00, 1, 1);
    func_8002BD90(arg0 +  0x62, 1.0f, 1.0f, 1.0f);
    func_8002C7EC(arg0 +  0x62, 3);
    func_8002C85C(arg0 +  0x62, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002C914(arg0 +  0x62, 0xFF, 0xFF, 0xFF, 0xFF);

    func_80034C40(0, arg0, arg0 + 0x62, 0xC, arr[arg0].x, arr[arg0].y, arr[arg0].z, 0xFF, 0xFF, 0, 0);
    
}

//INCLUDE_ASM(const s32, "level", func_8007341C);

void func_8007341C(u8 arg0) {

    Vec3f arr[9];
    
    Vec* ptr = arr;
    Vec* ptr2 = D_8011FB70;

    do {    
        *(Vec4f*)ptr = *(Vec4f*)ptr2;
        ptr2++;
        ptr++;
    } while (ptr2 != (D_8011FB70 + 0x6));

    *(Vec3f*)ptr = *(Vec3f*)ptr2;

    func_8002B138(arg0 + 0x62, &D_D86D90, &D_D8B140, &D_D8B140_2, &D_D8B160, &D_D8B160_2, &D_D8B1D0, (void* )0x802E4000, (void* )0x802E4D00, (void* )0x802E7400, (void* )0x802E7700, (void* )0x802E7A00, (void* )0x802E7B00, (void* )1, (u8) (void* )1);
    func_8002BD90(arg0 + 0x62, 1.0f, 1.0f, 1.0f);
    func_8002C7EC(arg0 + 0x62, 3);
    func_8002C85C(arg0 + 0x62, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002C914(arg0 + 0x62, 0xFF, 0xFF, 0xFF, 0xFF);

    func_80034C40(0, arg0, arg0 + 0x62, 0xB, arr[arg0].x, arr[arg0].y, arr[arg0].z, 0xFF, 0xFF, 0, 0);
}

// alternate
// Vec4f D_8011FB70[];
/* 
void func_8007341C(u8 arg0) {

    Vec3f arr[9];
    
    Vec4f* ptr = arr;
    Vec4f* ptr2 = D_8011FB70;

    do {    
        *ptr = *ptr2;
        ptr2++;
        ptr++;
    } while (ptr2 != (D_8011FB70 + 0x6));

    *(Vec3f*)ptr = *(Vec3f*)ptr2;

    func_8002B138(arg0 + 0x62, &D_D86D90, &D_D8B140, &D_D8B140_2, &D_D8B160, &D_D8B160_2, &D_D8B1D0, (void* )0x802E4000, (void* )0x802E4D00, (void* )0x802E7400, (void* )0x802E7700, (void* )0x802E7A00, (void* )0x802E7B00, (void* )1, (u8) (void* )1);
    func_8002BD90(arg0 + 0x62, 1.0f, 1.0f, 1.0f);
    func_8002C7EC(arg0 + 0x62, 3);
    func_8002C85C(arg0 + 0x62, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002C914(arg0 + 0x62, 0xFF, 0xFF, 0xFF, 0xFF);

    func_80034C40(0, arg0, arg0 + 0x62, 0xB, arr[arg0].x, arr[arg0].y, arr[arg0].z, 0xFF, 0xFF, 0, 0);
}
*/

INCLUDE_RODATA(const s32, "level", D_8011FB28);

INCLUDE_RODATA(const s32, "level", D_8011FB70);

INCLUDE_ASM(const s32, "level", func_800735FC);

INCLUDE_ASM(const s32, "level", func_800746B4);

//INCLUDE_ASM(const s32, "level", func_80074C38);

u8 func_80074C38(u8 mapIndex) {
    return D_801147C0[mapIndex];
}

// jtbl_8011FEA0
INCLUDE_ASM(const s32, "level", func_80074C50);

INCLUDE_RODATA(const s32, "level", D_80120008);

INCLUDE_RODATA(const s32, "level", D_8012000C);
