#include "common.h"

#include "level.h"

#include "system/map.h"
#include "system/memory.h"
#include "system/sprite.h"
#include "system/mapContext.h"

#include "game.h"
#include "gameAudio.h"
#include "mapObjects.h"                                                                        
#include "weather.h"      

// offsets into combined textures
extern u8 D_801C3F35;
extern u8 D_801FD624;
                             
// likely bss
extern u8 previousEntranceIndex;
extern u8 gEntranceIndex;
extern u8 gBaseMapIndex;
extern u8 gMapWithSeasonIndex;
extern u8 D_8021E6D0;

extern void* homeItemsTextureStart;
extern void* homeItemsTextureEnd;
extern void* homeItemsPaletteStart;
extern void* homeItemsPaletteEnd;
extern void* homeItemsTableStart;
extern void* homeItemsTableEnd;

extern void* groundItemsTextureStart;
extern void* groundItemsTextureEnd;
extern void* groundItemsPaletteStart;
extern void* groundItemsPaletteEnd;


// rodata
extern u8 levelFlags[];
// map indices for level interactions
extern u8 D_80114280[];
// Vec4 u8s
extern u8 D_801142E0[4][4];
// song indices for level
extern u8 D_801144C0[4][8];
extern u8 D_801147C0[];
extern u8 D_8011FB28[];
extern Vec D_8011FB70[];

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
 
//INCLUDE_ASM(const s32, "level", setLevelAudio);

void setLevelAudio(u16 mapIndex, u8 season, u8 hour) {

    u32 offset;
    
    gCurrentSongIndex = 0xFF;

    if (gWeather == RAIN) {
        
        gCurrentSongIndex = RAIN_SFX;
        
        if (getLevelFlags(mapIndex) & 0x10) {
            gSongVolume = 64;
        } else {
            gSongVolume = 128;
        }
        
    } else if (gWeather == TYPHOON) {
        
        gCurrentSongIndex = TYPHOON_SFX;
        gSongVolume = 128;
        
    } else {

        if (hour >= 6 || gBaseMapIndex == 9) {
            
            offset = 0;
            
            if (17 < hour &&  hour < 24) {

                offset = 4;

                if (!(getLevelFlags(mapIndex) & 0x10)) {
                    gSongVolume = 128;
                } else {
                    gSongVolume = 64;
                }
             } else {
                gSongVolume = 128;
             }
            
            gCurrentSongIndex = D_801144C0[mapIndex][offset+(season-1)];
        }   
        
    }

    if (gCurrentSongIndex != 0xFF) {
        func_800ACB04(gCurrentSongIndex);
    }
}

//INCLUDE_ASM(const s32, "level", func_8006EB94);

Vec4f* func_8006EB94(Vec4f* arg0, u16 arg1) {

    Vec4f vec;

    vec.r = D_801142E0[arg1][0];
    vec.g = D_801142E0[arg1][1];
    vec.b = D_801142E0[arg1][2];
    vec.a = D_801142E0[arg1][3];

    *arg0 = vec;
    
    return arg0;
    
}

//INCLUDE_ASM(const s32, "level", getLevelFlags);

u8 getLevelFlags(u16 mapIndex) {
    return levelFlags[mapIndex];
}

//INCLUDE_ASM(const s32, "level", func_8006EC58);

// call map.c functions for foragable maps
void func_8006EC58(u16 mapIndex) {

    switch (mapIndex) {

        case FARM:
            D_801FD624 = 0xE;
            D_801C3F35 = 0xB;
            func_80034EF0(0, 0xE, 0xB, (void*)0x802EB800, (void*)0x802F2800, (void*)0x802F3100, &groundItemsTextureStart, &groundItemsTextureEnd, &groundItemsPaletteStart, &groundItemsPaletteEnd, 0x60); 
            break;
        case GREENHOUSE:
            D_801FD624 = 1;
            D_801C3F35 = 1;
            func_80034EF0(0, 1, 1, (void*)0x802EB800, (void*)0x802F2800, (void*)0x802F3100, &groundItemsTextureStart, &groundItemsTextureEnd, &groundItemsPaletteStart, &groundItemsPaletteEnd, 2); 
            break;            
        case MOUNTAIN_1:
            D_801FD624 = 5;
            D_801C3F35 = 0;
            func_80034EF0(0, 5, 0, (void*)0x802EB800, (void*)0x802F2800, (void*)0x802F3100, &groundItemsTextureStart, &groundItemsTextureEnd, &groundItemsPaletteStart, &groundItemsPaletteEnd, 0); 
            break;    
        case MOUNTAIN_2:
            D_801FD624 = 3;
            D_801C3F35 = 5;
            func_80034EF0(0, 3, 5, (void*)0x802EB800, (void*)0x802F2800, (void*)0x802F3100, &groundItemsTextureStart, &groundItemsTextureEnd, &groundItemsPaletteStart, &groundItemsPaletteEnd, 0); 
            break;  
        case TOP_OF_MOUNTAIN_1:
            D_801FD624 = 5;
            D_801C3F35 = 0;
            func_80034EF0(0, 5, 0, (void*)0x802EB800, (void*)0x802F2800, (void*)0x802F3100, &groundItemsTextureStart, &groundItemsTextureEnd, &groundItemsPaletteStart, &groundItemsPaletteEnd, 0); 
            break;  
        case MOON_MOUNTAIN:
            D_801FD624 = 5;
            D_801C3F35 = 0;
            func_80034EF0(0, 5, 0, (void*)0x802EB800, (void*)0x802F2800, (void*)0x802F3100, &groundItemsTextureStart, &groundItemsTextureEnd, &groundItemsPaletteStart, &groundItemsPaletteEnd, 0); 
            break;                            
        case POND:
            D_801FD624 = 5;
            D_801C3F35 = 0;
            func_80034EF0(0, 5, 0, (void*)0x802EB800, (void*)0x802F2800, (void*)0x802F3100, &groundItemsTextureStart, &groundItemsTextureEnd, &groundItemsPaletteStart, &groundItemsPaletteEnd, 0); 
            break;          
        case CAVE:
            D_801FD624 = 0;
            D_801C3F35 = 0;
            func_80034EF0(0, 0, 0, (void*)0x802EB800, (void*)0x802F2800, (void*)0x802F3100, &groundItemsTextureStart, &groundItemsTextureEnd, &groundItemsPaletteStart, &groundItemsPaletteEnd, 0); 
            break;    
        case MINE:                                      
        case MINE_2:        
            D_801FD624 = 2;
            D_801C3F35 = 2;
            func_80034EF0(0, 2, 2, (void*)0x802EB800, (void*)0x802F2800, (void*)0x802F3100, &groundItemsTextureStart, &groundItemsTextureEnd, &groundItemsPaletteStart, &groundItemsPaletteEnd, 0); 
            break;    
        case RANCH:
            D_801FD624 = 0x10;
            D_801C3F35 = 0xA;
            func_80034EF0(0, 0x10, 0xA, (void*)0x802EB800, (void*)0x802F2800, (void*)0x802F3100, &groundItemsTextureStart, &groundItemsTextureEnd, &groundItemsPaletteStart, &groundItemsPaletteEnd, 0); 
            break;   
    }

    switch (mapIndex) {
        
        case RANCH:                                       
        case MOUNTAIN_1:                                      
        case MOUNTAIN_2:                                      
        case TOP_OF_MOUNTAIN_1:                                      
        case MOON_MOUNTAIN:                                      
        case POND:                                      
        case CAVE:                                      
        case MINE:                                      
        case MINE_2:                                      
        case FARM:                                      
        case GREENHOUSE:        
            
            switch (gSeason) {                      
                case 1:                                    
                    func_80035054(0, 2, 3, 0, 8.0f, 0);
                    break;
                case 2:                      
                    func_80035054(0, 2, 0x26, 0, 8.0f, 0);
                    break;
                case 3:                                    
                    func_80035054(0, 2, 0x29, 0, 8.0f, 0);
                    break;
            }
            
            if (gSeason == WINTER) {
                func_80035054(0, 1, 0x1F, 0, 8.0f, 0);
                func_80035054(0, 3, 0x21, 0, 8.0f, 0);
                func_80035054(0, 4, 0x24, 0, 8.0f, 0);
                func_80035054(0, 0x2C, 0x20, 16.0f, 8.0f, 16.0f);
                func_80035054(0, 0x2D, 0x25, 16.0f, 8.0f, 16.0f);
            } else {
                func_80035054(0, 1, 0, 0, 8.0f, 0);
                func_80035054(0, 3, 0x1D, 0, 8.0f, 0);
                func_80035054(0, 4, 0x1E, 0, 8.0f, 0);
                func_80035054(0, 0x2C, 8, 16.0f, 8.0f, 16.0f);
                func_80035054(0, 0x2D, 0x19, 16.0f, 8.0f, 16.0f);
            }
            
            func_80035054(0, 5, 0x22, 0, 8.0f, 0);
            func_80035054(0, 6, 0x23, 0, 8.0f, 0);
            func_80035054(0, 7, 7, 0, 8.0f, 0);
            func_80035054(0, 8, 0x27, 0, 8.0f, 0);
            func_80035054(0, 9, 0x28, 0, 8.0f, 0);
            func_80035054(0, 0xA, 9, 0, 8.0f, 0);
            func_80035054(0, 0xB, 0x3F, 0, 8.0f, 0);
            func_80035054(0, 0xC, 0x40, 0, 8.0f, 0);
            func_80035054(0, 0xD, 0x41, 0, 8.0f, 0);
            func_80035054(0, 0xE, 0xD, 0, 8.0f, 0);
            func_80035054(0, 0xF, 1, 0, 8.0f, 0);
            func_80035054(0, 0x10, 0x2C, 0, 8.0f, 0);
            func_80035054(0, 0x11, 0x2D, 0, 8.0f, 0);
            func_80035054(0, 0x12, 0xA, 0, 8.0f, 0);
            func_80035054(0, 0x13, 2, 0, 8.0f, 0);
            func_80035054(0, 0x14, 4, 0, 8.0f, 0);
            func_80035054(0, 0x15, 5, 0, 8.0f, 0);
            func_80035054(0, 0x16, 6, 0, 8.0f, 0);
            func_80035054(0, 0x17, 0x31, 0, 8.0f, 0);
            func_80035054(0, 0x18, 0x32, 0, 8.0f, 0);
            func_80035054(0, 0x19, 0x33, 0, 8.0f, 0);
            func_80035054(0, 0x1A, 0x34, 0, 8.0f, 0);
            func_80035054(0, 0x1B, 0xB, 0, 8.0f, 0);
            func_80035054(0, 0x1C, 0x39, 0, 8.0f, 0);
            func_80035054(0, 0x1D, 0x3A, 0, 8.0f, 0);
            func_80035054(0, 0x1E, 0x3B, 0, 8.0f, 0);
            func_80035054(0, 0x1F, 0x3C, 0, 8.0f, 0);
            func_80035054(0, 0x20, 0xC, 0, 8.0f, 0);
            func_80035054(0, 0x21, 0xE, 0, 8.0f, 0);
            func_80035054(0, 0x22, 0xF, 0, 8.0f, 0);
            func_80035054(0, 0x23, 0x10, 0, 8.0f, 0);
            func_80035054(0, 0x24, 0x11, 0, 8.0f, 0);
            func_80035054(0, 0x25, 0x12, 0, 8.0f, 0);
            func_80035054(0, 0x26, 0x13, 0, 8.0f, 0);
            func_80035054(0, 0x27, 0x14, 0, 8.0f, 0);
            func_80035054(0, 0x28, 0x15, 0, 8.0f, 0);
            func_80035054(0, 0x29, 0x16, -8.0f, 8.0f, -8.0f);
            func_80035054(0, 0x2A, 0x17, -8.0f, 8.0f, -8.0f);
            func_80035054(0, 0x2B, 0x18, -8.0f, 8.0f, -8.0f);
            func_80035054(0, 0x30, 0x2A, 0, 8.0f, 0);
            func_80035054(0, 0x31, 0x2B, 0, 8.0f, 0);
            func_80035054(0, 0x32, 0x2E, 0, 8.0f, 0);
            func_80035054(0, 0x33, 0x2F, 0, 8.0f, 0);
            func_80035054(0, 0x34, 0x30, 0, 8.0f, 0);
            func_80035054(0, 0x35, 0x35, 0, 8.0f, 0);
            func_80035054(0, 0x36, 0x36, 0, 8.0f, 0);
            func_80035054(0, 0x37, 0x37, 0, 8.0f, 0);
            func_80035054(0, 0x38, 0x38, 0, 8.0f, 0);

            break;
        
        default:
            break;
    }    
}

INCLUDE_ASM(const s32, "level", func_8006F938);

//INCLUDE_ASM(const s32, "level", func_80073244);

void func_80073244(u8 itemIndex) {

    Vec3f arr[6];

    MemoryRead_32 *ptr = arr;
    MemoryRead_32 *ptr2 = D_8011FB28;

    do {
        *(Aligned32*)ptr++ = *(Aligned32*)ptr2++;
    } while (ptr2 != (D_8011FB28 + 0x40));    

    *(Vec2f*)ptr = *(Vec2f*)ptr2;

    func_8002B138(itemIndex +  0x62, &homeItemsTextureStart, &homeItemsTextureEnd, &homeItemsPaletteStart, &homeItemsPaletteEnd, &homeItemsTableStart, &homeItemsTableEnd, (void*)0x802E4000, (void*)0x802E4D00, (void*)0x802E7400, (void*)0x802E7700, (void*)0x802E7A00, (void*)0x802E7B00, 1, 1);
    func_8002BD90(itemIndex +  0x62, 1.0f, 1.0f, 1.0f);
    func_8002C7EC(itemIndex +  0x62, 3);
    func_8002C85C(itemIndex +  0x62, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002C914(itemIndex +  0x62, 0xFF, 0xFF, 0xFF, 0xFF);

    func_80034C40(0, itemIndex, itemIndex + 0x62, 0xC, arr[itemIndex].x, arr[itemIndex].y, arr[itemIndex].z, 0xFF, 0xFF, 0, 0);
    
}

//INCLUDE_ASM(const s32, "level", func_8007341C);

void func_8007341C(u8 itemIndex) {

    Vec3f arr[9];
    
    Vec* ptr = arr;
    Vec* ptr2 = D_8011FB70;

    do {    
        *(Vec4f*)ptr++ = *(Vec4f*)ptr2++;
    } while (ptr2 != (D_8011FB70 + 0x6));

    *(Vec3f*)ptr = *(Vec3f*)ptr2;

    func_8002B138(itemIndex + 0x62, &homeItemsTextureStart, &homeItemsTextureEnd, &homeItemsPaletteStart, &homeItemsPaletteEnd, &homeItemsTableStart, &homeItemsTableEnd, (void* )0x802E4000, (void* )0x802E4D00, (void* )0x802E7400, (void* )0x802E7700, (void* )0x802E7A00, (void* )0x802E7B00, (void* )1, (u8) (void* )1);
    func_8002BD90(itemIndex + 0x62, 1.0f, 1.0f, 1.0f);
    func_8002C7EC(itemIndex + 0x62, 3);
    func_8002C85C(itemIndex + 0x62, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002C914(itemIndex + 0x62, 0xFF, 0xFF, 0xFF, 0xFF);

    func_80034C40(0, itemIndex, itemIndex + 0x62, 0xB, arr[itemIndex].x, arr[itemIndex].y, arr[itemIndex].z, 0xFF, 0xFF, 0, 0);
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

    func_8002B138(arg0 + 0x62, &homeItemsTextureStart, &homeItemsTextureEnd, &homeItemsPaletteStart, &homeItemsPaletteEnd, &homeItemsTableStart, &homeItemsTableEnd, (void* )0x802E4000, (void* )0x802E4D00, (void* )0x802E7400, (void* )0x802E7700, (void* )0x802E7A00, (void* )0x802E7B00, (void* )1, (u8) (void* )1);
    func_8002BD90(arg0 + 0x62, 1.0f, 1.0f, 1.0f);
    func_8002C7EC(arg0 + 0x62, 3);
    func_8002C85C(arg0 + 0x62, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002C914(arg0 + 0x62, 0xFF, 0xFF, 0xFF, 0xFF);

    func_80034C40(0, arg0, arg0 + 0x62, 0xB, arr[arg0].x, arr[arg0].y, arr[arg0].z, 0xFF, 0xFF, 0, 0);
}
*/

INCLUDE_RODATA(const s32, "level", D_8011FB28);

INCLUDE_RODATA(const s32, "level", D_8011FB70);

// static const f32 D_8011FB70[28] = {
//     -136.0f, 0.0f, 88.0f,
//     -136.0f, 0.0f, 24.0f,
//     -136.0f, 0.0f, -40.0f,
//     -136.0f, 0.0f, -104.0f,
//     120.0f, 0.0f, 88.0f,
//     120.0f, 0.0f, 24.0f,
//     120.0f, 0.0f, -40.0f,
//     120.0f, 0.0f, -104.0f,
//     220.0f, 0.0f, -264.0f,
//     0.0f
// };

INCLUDE_ASM(const s32, "level", func_800735FC);

// matches but have to match preceding rodata
/*
void func_800735FC(u16 levelIndex) {

    switch (levelIndex) {
        case FARM:
            func_80038900(0, 9, 0, 0, 1);
            func_80038900(0, 0xA, 0, 2, 3);
            func_80038900(0, 0xA, 1, 3, 2);
            func_80038900(0, 0xB, 0, 4, 1);
            func_80038900(0, 0xC, 0, 1, 1);
            func_80038900(0, 0xD, 0, 0xC, 1);
            func_80038900(0, 0xE, 0, 0xA, 1);
            func_80038900(0, 0xF, 0, 0xB, 1);
            func_80038900(0, 0x10, 0, 0xD, 1);
            func_80038900(0, 0x11, 0, 9, 1);
            func_80038900(0, 0x13, 0, 0xE, 1);
            func_80038900(0, 0x14, 0, 0xF, 1);
            func_80038900(0, 0x12, 0, 0x11, 2);
            func_80038900(0, 0x12, 1, 0x12, 2);
            func_80038900(0, 0x12, 2, 0x11, 2);
            func_80038900(0, 0x12, 3, 0x13, 2);
            func_80038900(0, 0x12, 4, 0x14, 2);
            func_80038900(0, 0x1A, 0, 0x1A, 1);
            func_80038900(0, 0x1B, 0, 2, 3);
            func_80038900(0, 0x1B, 1, 3, 8);
            func_80038900(0, 0x1B, 2, 2, 2);
            func_80038900(0, 0x1B, 3, 0x1B, 2);
            func_80038900(0, 0x1C, 0, 0x1C, 1);
            func_80038900(0, 0x1D, 0, 0x1D, 1);
            func_80038900(0, 0x1E, 0, 0x1E, 1);
            func_80038900(0, 0, 0, 0x20, 0);
            func_80038900(0, 1, 0, 0x21, 0);
            func_80038900(0, 2, 0, 0x22, 0);
            func_80038900(0, 3, 0, 0x1F, 0);
            func_80038900(0, 4, 0, 5, 0);
            func_80038900(0, 5, 0, 6, 0);
            func_80038900(0, 6, 0, 7, 0);
            func_80038900(0, 7, 0, 8, 0);
            func_80038900(0, 8, 0, 0x10, 0);
            break;
        default:
            break;
        case GREENHOUSE:
            func_80038900(0, 0, 0, 0, 2);
            func_80038900(0, 0, 1, 1, 2);
            func_80038900(0, 0, 2, 0, 2);
            func_80038900(0, 0, 3, 2, 2);
            func_80038900(0, 0, 4, 3, 2);
            func_80038900(0, 4, 0, 4, 0);
            func_80038900(0, 5, 0, 5, 0);
            func_80038900(0, 6, 0, 6, 0);
            func_80038900(0, 7, 0, 7, 0);
            func_80038900(0, 8, 0, 8, 0);
            break;
        case HOUSE:
            func_80038900(0, 0, 0, 2, 1);
            func_80038900(0, 1, 0, 9, 1);
            func_80038900(0, 2, 0, 3, 1);
            func_80038900(0, 3, 0, 0, 1);
            func_80038900(0, 4, 0, 7, 1);
            func_80038900(0, 5, 0, 6, 1);
            func_80038900(0, 6, 0, 5, 1);
            func_80038900(0, 7, 0, 4, 1);
            func_80038900(0, 8, 0, 1, 1);
            func_80038900(0, 9, 0, 8, 1);
            func_80038900(0, 0xA, 0, 0xA, 1);
            func_80038900(0, 0xB, 0, 0xB, 1);
            func_80038900(0, 0xC, 0, 0xC, 1);
            break;
        case KITCHEN:
            func_80038900(0, 0, 0, 0, 1);
            func_80038900(0, 1, 0, 1, 1);
            func_80038900(0, 2, 0, 2, 1);
            func_80038900(0, 3, 0, 3, 1);
            break;
        case BATHROOM:
            func_80038900(0, 0, 0, 0, 1);
            func_80038900(0, 1, 1, 1, 1);
            func_80038900(0, 2, 2, 3, 1);
            func_80038900(0, 3, 3, 2, 1);
            break;
        case COOP:
            func_80038900(0, 0, 0, 0, 2);
            func_80038900(0, 0, 1, 1, 2);
            func_80038900(0, 0, 2, 0, 2);
            func_80038900(0, 0, 3, 2, 2);
            func_80038900(0, 0, 4, 3, 2);
            func_80038900(0, 1, 0, 4, 1);
            break;
        case BARN:
            func_80038900(0, 0, 0, 0, 2);
            func_80038900(0, 0, 1, 1, 2);
            func_80038900(0, 0, 2, 0, 2);
            func_80038900(0, 0, 3, 2, 2);
            func_80038900(0, 0, 4, 3, 2);
            break;
        case VILLAGE_1:
            func_80038900(0, 0, 0, 0, 1);
            func_80038900(0, 1, 0, 1, 1);
            func_80038900(0, 2, 0, 2, 1);
            func_80038900(0, 3, 0, 4, 1);
            func_80038900(0, 4, 0, 3, 1);
            func_80038900(0, 5, 0, 5, 1);
            func_80038900(0, 6, 0, 6, 1);
            func_80038900(0, 7, 0, 7, 1);
            func_80038900(0, 8, 0, 8, 1);
            func_80038900(0, 9, 0, 9, 1);
            func_80038900(0, 0xA, 0, 0xA, 1);
            func_80038900(0, 0xB, 0, 0xB, 1);
            func_80038900(0, 0xC, 0, 0xC, 1);
            break;
        case VILLAGE_2:
            func_80038900(0, 0, 0, 0, 1);
            func_80038900(0, 1, 0, 1, 1);
            func_80038900(0, 2, 0, 2, 1);
            func_80038900(0, 3, 0, 3, 1);
            break;

        case SQUARE:
            switch (gSeason) {                          
                case SPRING:                                     
                    func_80038900(0, 0, 0, 0, 1);
                    func_80038900(0, 1, 0, 1, 1);
                    func_80038900(0, 2, 0, 2, 1);
                    func_80038900(0, 3, 0, 3, 1);
                    func_80038900(0, 4, 0, 4, 1);
                    func_80038900(0, 5, 0, 5, 1);
                    func_80038900(0, 6, 0, 6, 1);
                    func_80038900(0, 7, 0, 7, 1);
                    func_80038900(0, 8, 0, 8, 1);
                    func_80038900(0, 9, 0, 9, 1);
                    func_80038900(0, 0xA, 0, 0xA, 1);
                    break;
                case SUMMER:                                     
                    func_80038900(0, 0, 0, 0, 1);
                    func_80038900(0, 1, 0, 1, 1);
                    func_80038900(0, 2, 0, 2, 1);
                    func_80038900(0, 3, 0, 3, 1);
                    func_80038900(0, 4, 0, 4, 1);
                    func_80038900(0, 5, 0, 5, 1);
                    func_80038900(0, 6, 0, 6, 1);
                    break;
                case AUTUMN:                                     
                    func_80038900(0, 0, 0, 0, 1);
                    func_80038900(0, 1, 0, 1, 1);
                    func_80038900(0, 2, 0, 2, 1);
                    func_80038900(0, 3, 0, 3, 1);
                    func_80038900(0, 4, 0, 4, 1);
                    func_80038900(0, 5, 0, 5, 1);
                    func_80038900(0, 6, 0, 6, 1);
                    break;
                case WINTER:                                     
                    func_80038900(0, 0, 0, 0, 1);
                    func_80038900(0, 1, 0, 1, 1);
                    break;
                }
            break;
        case FLOWER_SHOP:
            func_80038900(0, 0, 0, 0, 1);
            break;
        case BAKERY:
            func_80038900(0, 0, 0, 0, 1);
            func_80038900(0, 1, 0, 1, 1);
            break;
        case MAYOR_HOUSE:
            func_80038900(0, 0, 0, 0, 1);
            break;
        case POTION_SHOP:
            func_80038900(0, 0, 0, 0, 1);
            break;
        case MOUNTAIN_1:
            func_80038900(0, 0, 0, 0, 1);
            break;
        case MOUNTAIN_2:
            func_80038900(0, 0, 0, 0, 1);
            func_80038900(0, 1, 0, 1, 1);
            func_80038900(0, 2, 0, 2, 1);
            func_80038900(0, 3, 0, 3, 1);
            break;
        case TOP_OF_MOUNTAIN_1:
            func_80038900(0, 0, 0, 0, 1);
            func_80038900(0, 1, 0, 1, 1);
            func_80038900(0, 2, 0, 2, 1);
            func_80038900(0, 3, 0, 3, 1);
            func_80038900(0, 4, 0, 4, 1);
            func_80038900(0, 5, 0, 5, 1);
            if (gSeason == WINTER) {
                func_80038900(0, 6, 0, 6, 1);
                break;
            }
            break;
        case MOON_MOUNTAIN:
            func_80038900(0, 0, 0, 0, 1);
            break;
        case POND:
            func_80038900(0, 4, 0, 0, 0);
            func_80038900(0, 5, 0, 1, 0);
            func_80038900(0, 6, 0, 2, 0);
            func_80038900(0, 7, 0, 3, 0);
            break;
        case VINEYARD:
            func_80038900(0, 0, 0, 1, 1);
            func_80038900(0, 1, 0, 0, 1);
            break;
        case RANCH:
            func_80038900(0, 0, 0, 0, 1);
            func_80038900(0, 1, 0, 1, 1);
            func_80038900(0, 2, 0, 2, 1);
            if (gSeason == AUTUMN) {
                func_80038900(0, 3, 0, 3, 1);
            }
            break;
        case RANCH_STORE:
            func_80038900(0, 0, 0, 0, 1);
            break;
        case RICK_STORE:
            func_80038900(0, 0, 0, 0, 1);
            break;
        case CAVE:
            func_80038900(0, 0, 0, 0, 1);
            break;
        case BEACH:
            func_80038900(0, 0, 0, 0, 4);
            func_80038900(0, 0, 1, 1, 4);
            func_80038900(0, 0, 2, 2, 4);
            func_80038900(0, 0, 3, 3, 4);
            func_80038900(0, 0, 4, 4, 4);
            func_80038900(0, 0, 5, 5, 4);
            func_80038900(0, 0, 6, 6, 4);
            func_80038900(0, 0, 7, 5, 4);
            func_80038900(0, 0, 8, 4, 4);
            func_80038900(0, 0, 9, 3, 4);
            func_80038900(0, 0, 0xA, 2, 4);
            func_80038900(0, 0, 0xB, 1, 4);
            break;
        case VINEYARD_CELLAR:
            func_80038900(0, 0, 0, 0, 1);
            break;
        case MINE:
        case MINE_2:
            func_80038900(0, 4, 0, 0, 0);
            func_80038900(0, 0, 0, 1, 0);
            break;
        
    }   
}
*/

INCLUDE_ASM(const s32, "level", func_800746B4);

//INCLUDE_ASM(const s32, "level", func_80074C38);

u8 func_80074C38(u8 mapIndex) {
    return D_801147C0[mapIndex];
}

// jtbl_8011FEA0
INCLUDE_ASM(const s32, "level", func_80074C50);

INCLUDE_RODATA(const s32, "level", D_80120008);

INCLUDE_RODATA(const s32, "level", D_8012000C);
