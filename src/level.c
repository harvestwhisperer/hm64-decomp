#include "common.h"

#include "level.h"

#include "system/cutscene.h"
#include "system/map.h"
#include "system/memory.h"
#include "system/sprite.h"
#include "system/mapContext.h"

#include "animals.h"
#include "game.h"
#include "gameAudio.h"
#include "gameStatus.h"
#include "mapObjects.h"   
#include "npc.h"
#include "setCutscenes.h"                                                                     
#include "weather.h"      

// offsets into combined textures
extern u8 D_801C3F35;
extern u8 D_801FD624;
                             
// likely bss
extern u8 previousEntranceIndex;
extern u8 gEntranceIndex;
extern u8 gBaseMapIndex;
extern u8 gMapWithSeasonIndex;
// rotation
extern u8 D_8021E6D0;
 
extern u32 homeItemsTexture_ROM_START;
extern u32 homeItemsTexture_ROM_END;
extern u32 homeItemsAssetsIndex_ROM_START;
extern u32 homeItemsAssetsIndex_ROM_END;
extern u32 homeItemsSpritesheetIndex_ROM_START;
extern u32 homeItemsSpritesheetIndex_ROM_END;

extern u32 groundObjectsTexture_ROM_START;
extern u32 groundObjectsTexture_ROM_END;
extern u32 groundObjectsIndex_ROM_START;
extern u32 groundObjectsIndex_ROM_END;

// rodata
extern u8 levelFlags[];
// map indices for level interactions
extern u8 D_80114280[];
// Vec4 u8s
extern u8 D_801142E0[4][4];
// song indices for level
extern u8 D_801144C0[4][8];

// exit to map indices
// probably shouldn't be volatile, but need it for matching func_80074C50
extern volatile u8 D_801147C0[];

/*
u8 D_801147C0[] = {
    FARM, FARM, FARM, FARM,
    FARM, FARM, FARM, FARM,
    FARM, FARM, HOUSE, HOUSE,
    HOUSE, BATHROOM, BATHROOM, KITCHEN,
    KITCHEN, BARN, COOP, GREENHOUSE,
    ROAD, ROAD, ROAD, ROAD,
    ROAD, ROAD, MOUNTAIN_1, MOUNTAIN_1,
    ...
}
*/

extern u8 D_8011FB28[];
extern Vec D_8011FB70[];

// forward declaration
void func_8006EC58(u16 mapIndex);
void func_8007341C(u8 itemIndex);

//INCLUDE_ASM(const s32, "level", setEntrance);

void setEntrance(u16 index) {
    u16 temp = gEntranceIndex;
    gEntranceIndex = index;
    previousEntranceIndex = temp;
}

//INCLUDE_ASM(const s32, "level", func_8006E840);

void func_8006E840(u16 entranceIndex) {

    if (gBaseMapIndex != 0xFF) {
        func_8003C504(0);
    }

    previousMapIndex = gBaseMapIndex;
    gBaseMapIndex = D_801147C0[entranceIndex];
    gMapWithSeasonIndex = gBaseMapIndex;

    if (getLevelFlags(gMapWithSeasonIndex) & HAS_SEASON_MAP) {
        gMapWithSeasonIndex = gMapWithSeasonIndex + (gSeason - 1);
    }

    // load map
    func_8003BB14(MAIN_MAP_INDEX, gMapWithSeasonIndex);

    // set rotation
    if (gBaseMapIndex == FARM) {
        func_8003C084(MAIN_MAP_INDEX, (D_80114280[gMapWithSeasonIndex] + D_8021E6D0) % 8);
    } else {
        func_8003C084(MAIN_MAP_INDEX, D_80114280[gMapWithSeasonIndex] % 8);
    }
     
    func_8003C1E0(MAIN_MAP_INDEX, 0, 0, 0, 8, 8);
    func_8009C054(gBaseMapIndex);
    setLevelGraphicsData(gBaseMapIndex);
    func_8002EEA4(0);
    
}

//INCLUDE_ASM(const s32, "level", setLevelGraphicsData);

void setLevelGraphicsData(u16 mapIndex) {

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
            func_80034EF0(MAIN_MAP_INDEX, 0xE, 0xB, (void*)0x802EB800, (void*)0x802F2800, (void*)0x802F3100, &groundObjectsTexture_ROM_START, &groundObjectsTexture_ROM_END, &groundObjectsIndex_ROM_START, &groundObjectsIndex_ROM_END, 0x60); 
            break;
        case GREENHOUSE:
            D_801FD624 = 1;
            D_801C3F35 = 1;
            func_80034EF0(MAIN_MAP_INDEX, 1, 1, (void*)0x802EB800, (void*)0x802F2800, (void*)0x802F3100, &groundObjectsTexture_ROM_START, &groundObjectsTexture_ROM_END, &groundObjectsIndex_ROM_START, &groundObjectsIndex_ROM_END, 2); 
            break;            
        case MOUNTAIN_1:
            D_801FD624 = 5;
            D_801C3F35 = 0;
            func_80034EF0(MAIN_MAP_INDEX, 5, 0, (void*)0x802EB800, (void*)0x802F2800, (void*)0x802F3100, &groundObjectsTexture_ROM_START, &groundObjectsTexture_ROM_END, &groundObjectsIndex_ROM_START, &groundObjectsIndex_ROM_END, 0); 
            break;    
        case MOUNTAIN_2:
            D_801FD624 = 3;
            D_801C3F35 = 5;
            func_80034EF0(MAIN_MAP_INDEX, 3, 5, (void*)0x802EB800, (void*)0x802F2800, (void*)0x802F3100, &groundObjectsTexture_ROM_START, &groundObjectsTexture_ROM_END, &groundObjectsIndex_ROM_START, &groundObjectsIndex_ROM_END, 0); 
            break;  
        case TOP_OF_MOUNTAIN_1:
            D_801FD624 = 5;
            D_801C3F35 = 0;
            func_80034EF0(MAIN_MAP_INDEX, 5, 0, (void*)0x802EB800, (void*)0x802F2800, (void*)0x802F3100, &groundObjectsTexture_ROM_START, &groundObjectsTexture_ROM_END, &groundObjectsIndex_ROM_START, &groundObjectsIndex_ROM_END, 0); 
            break;  
        case MOON_MOUNTAIN:
            D_801FD624 = 5;
            D_801C3F35 = 0;
            func_80034EF0(MAIN_MAP_INDEX, 5, 0, (void*)0x802EB800, (void*)0x802F2800, (void*)0x802F3100, &groundObjectsTexture_ROM_START, &groundObjectsTexture_ROM_END, &groundObjectsIndex_ROM_START, &groundObjectsIndex_ROM_END, 0); 
            break;                            
        case POND:
            D_801FD624 = 5;
            D_801C3F35 = 0;
            func_80034EF0(MAIN_MAP_INDEX, 5, 0, (void*)0x802EB800, (void*)0x802F2800, (void*)0x802F3100, &groundObjectsTexture_ROM_START, &groundObjectsTexture_ROM_END, &groundObjectsIndex_ROM_START, &groundObjectsIndex_ROM_END, 0); 
            break;          
        case CAVE:
            D_801FD624 = 0;
            D_801C3F35 = 0;
            func_80034EF0(MAIN_MAP_INDEX, 0, 0, (void*)0x802EB800, (void*)0x802F2800, (void*)0x802F3100, &groundObjectsTexture_ROM_START, &groundObjectsTexture_ROM_END, &groundObjectsIndex_ROM_START, &groundObjectsIndex_ROM_END, 0); 
            break;    
        case MINE:                                      
        case MINE_2:        
            D_801FD624 = 2;
            D_801C3F35 = 2;
            func_80034EF0(MAIN_MAP_INDEX, 2, 2, (void*)0x802EB800, (void*)0x802F2800, (void*)0x802F3100, &groundObjectsTexture_ROM_START, &groundObjectsTexture_ROM_END, &groundObjectsIndex_ROM_START, &groundObjectsIndex_ROM_END, 0); 
            break;    
        case RANCH:
            D_801FD624 = 0x10;
            D_801C3F35 = 0xA;
            func_80034EF0(MAIN_MAP_INDEX, 0x10, 0xA, (void*)0x802EB800, (void*)0x802F2800, (void*)0x802F3100, &groundObjectsTexture_ROM_START, &groundObjectsTexture_ROM_END, &groundObjectsIndex_ROM_START, &groundObjectsIndex_ROM_END, 0); 
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
                    func_80035054(MAIN_MAP_INDEX, 2, 3, 0, 8.0f, 0);
                    break;
                case 2:                      
                    func_80035054(MAIN_MAP_INDEX, 2, 0x26, 0, 8.0f, 0);
                    break;
                case 3:                                    
                    func_80035054(MAIN_MAP_INDEX, 2, 0x29, 0, 8.0f, 0);
                    break;
            }
            
            if (gSeason == WINTER) {
                func_80035054(MAIN_MAP_INDEX, 1, 0x1F, 0, 8.0f, 0);
                func_80035054(MAIN_MAP_INDEX, 3, 0x21, 0, 8.0f, 0);
                func_80035054(MAIN_MAP_INDEX, 4, 0x24, 0, 8.0f, 0);
                func_80035054(MAIN_MAP_INDEX, 0x2C, 0x20, 16.0f, 8.0f, 16.0f);
                func_80035054(MAIN_MAP_INDEX, 0x2D, 0x25, 16.0f, 8.0f, 16.0f);
            } else {
                func_80035054(MAIN_MAP_INDEX, 1, 0, 0, 8.0f, 0);
                func_80035054(MAIN_MAP_INDEX, 3, 0x1D, 0, 8.0f, 0);
                func_80035054(MAIN_MAP_INDEX, 4, 0x1E, 0, 8.0f, 0);
                func_80035054(MAIN_MAP_INDEX, 0x2C, 8, 16.0f, 8.0f, 16.0f);
                func_80035054(MAIN_MAP_INDEX, 0x2D, 0x19, 16.0f, 8.0f, 16.0f);
            }
            
            func_80035054(MAIN_MAP_INDEX, 5, 0x22, 0, 8.0f, 0);
            func_80035054(MAIN_MAP_INDEX, 6, 0x23, 0, 8.0f, 0);
            func_80035054(MAIN_MAP_INDEX, 7, 7, 0, 8.0f, 0);
            func_80035054(MAIN_MAP_INDEX, 8, 0x27, 0, 8.0f, 0);
            func_80035054(MAIN_MAP_INDEX, 9, 0x28, 0, 8.0f, 0);
            func_80035054(MAIN_MAP_INDEX, 0xA, 9, 0, 8.0f, 0);
            func_80035054(MAIN_MAP_INDEX, 0xB, 0x3F, 0, 8.0f, 0);
            func_80035054(MAIN_MAP_INDEX, 0xC, 0x40, 0, 8.0f, 0);
            func_80035054(MAIN_MAP_INDEX, 0xD, 0x41, 0, 8.0f, 0);
            func_80035054(MAIN_MAP_INDEX, 0xE, 0xD, 0, 8.0f, 0);
            func_80035054(MAIN_MAP_INDEX, 0xF, 1, 0, 8.0f, 0);
            func_80035054(MAIN_MAP_INDEX, 0x10, 0x2C, 0, 8.0f, 0);
            func_80035054(MAIN_MAP_INDEX, 0x11, 0x2D, 0, 8.0f, 0);
            func_80035054(MAIN_MAP_INDEX, 0x12, 0xA, 0, 8.0f, 0);
            func_80035054(MAIN_MAP_INDEX, 0x13, 2, 0, 8.0f, 0);
            func_80035054(MAIN_MAP_INDEX, 0x14, 4, 0, 8.0f, 0);
            func_80035054(MAIN_MAP_INDEX, 0x15, 5, 0, 8.0f, 0);
            func_80035054(MAIN_MAP_INDEX, 0x16, 6, 0, 8.0f, 0);
            func_80035054(MAIN_MAP_INDEX, 0x17, 0x31, 0, 8.0f, 0);
            func_80035054(MAIN_MAP_INDEX, 0x18, 0x32, 0, 8.0f, 0);
            func_80035054(MAIN_MAP_INDEX, 0x19, 0x33, 0, 8.0f, 0);
            func_80035054(MAIN_MAP_INDEX, 0x1A, 0x34, 0, 8.0f, 0);
            func_80035054(MAIN_MAP_INDEX, 0x1B, 0xB, 0, 8.0f, 0);
            func_80035054(MAIN_MAP_INDEX, 0x1C, 0x39, 0, 8.0f, 0);
            func_80035054(MAIN_MAP_INDEX, 0x1D, 0x3A, 0, 8.0f, 0);
            func_80035054(MAIN_MAP_INDEX, 0x1E, 0x3B, 0, 8.0f, 0);
            func_80035054(MAIN_MAP_INDEX, 0x1F, 0x3C, 0, 8.0f, 0);
            func_80035054(MAIN_MAP_INDEX, 0x20, 0xC, 0, 8.0f, 0);
            func_80035054(MAIN_MAP_INDEX, 0x21, 0xE, 0, 8.0f, 0);
            func_80035054(MAIN_MAP_INDEX, 0x22, 0xF, 0, 8.0f, 0);
            func_80035054(MAIN_MAP_INDEX, 0x23, 0x10, 0, 8.0f, 0);
            func_80035054(MAIN_MAP_INDEX, 0x24, 0x11, 0, 8.0f, 0);
            func_80035054(MAIN_MAP_INDEX, 0x25, 0x12, 0, 8.0f, 0);
            func_80035054(MAIN_MAP_INDEX, 0x26, 0x13, 0, 8.0f, 0);
            func_80035054(MAIN_MAP_INDEX, 0x27, 0x14, 0, 8.0f, 0);
            func_80035054(MAIN_MAP_INDEX, 0x28, 0x15, 0, 8.0f, 0);
            func_80035054(MAIN_MAP_INDEX, 0x29, 0x16, -8.0f, 8.0f, -8.0f);
            func_80035054(MAIN_MAP_INDEX, 0x2A, 0x17, -8.0f, 8.0f, -8.0f);
            func_80035054(MAIN_MAP_INDEX, 0x2B, 0x18, -8.0f, 8.0f, -8.0f);
            func_80035054(MAIN_MAP_INDEX, 0x30, 0x2A, 0, 8.0f, 0);
            func_80035054(MAIN_MAP_INDEX, 0x31, 0x2B, 0, 8.0f, 0);
            func_80035054(MAIN_MAP_INDEX, 0x32, 0x2E, 0, 8.0f, 0);
            func_80035054(MAIN_MAP_INDEX, 0x33, 0x2F, 0, 8.0f, 0);
            func_80035054(MAIN_MAP_INDEX, 0x34, 0x30, 0, 8.0f, 0);
            func_80035054(MAIN_MAP_INDEX, 0x35, 0x35, 0, 8.0f, 0);
            func_80035054(MAIN_MAP_INDEX, 0x36, 0x36, 0, 8.0f, 0);
            func_80035054(MAIN_MAP_INDEX, 0x37, 0x37, 0, 8.0f, 0);
            func_80035054(MAIN_MAP_INDEX, 0x38, 0x38, 0, 8.0f, 0);

            break;
        
        default:
            break;
    }    
}

INCLUDE_ASM(const s32, "level", func_8006F938);

// need to add linker symbols
/*
void func_8006F938(u16 levelIndex) {

    u8 i;
    u8 j;
    u16 k;

    switch (levelIndex) {

        case 0x52:
            
            func_8002B138(0x62, &D_D86D90, &D_D8B140, &D_D8B140_2, &D_D8B160, &D_D8B160_2, &D_D8B1D0, (void*)0x802E4000, (void*)0x802E4D00, (void*)0x802E7400, (void*)0x802E7700, (void*)0x802E7A00, (void*)0x802E7B00, 1, 1);
            func_8002BD90(0x62, 1.0f, 1.0f, 1.0f);
            func_8002C7EC(0x62, 3);
            func_8002C85C(0x62, 0xFF, 0xFF, 0xFF, 0xFF);
            func_8002C914(0x62, 0xFF, 0xFF, 0xFF, 0xFF);
            
            if (checkDailyEventBit(0x43)) {
                func_80034C40(0, 0, 0x62, 0x13, -464.0f, 96.0f, 112.0f, 0xFF, 0xFF, 0, 0);
            } else {
                func_80034C40(0, 0, 0x62, 0xF, -464.0f, 96.0f, 112.0f, 0xFF, 0xFF, 0, 0);
            }
            
            func_8002B138(0x63, &D_CAD500, &D_CAD5E0, &D_CAD5E0_2, &D_CAD600, &D_CAD600_2, &D_CAD610, (void*)0x802E7C00, (void*)0x802E8900, (void*)0x802E9600, (void*)0x802E9700, (void*)0x802E9800, (void*)0x802E9900, 1,1);
            func_8002BD90(0x63, 6.0f, 1.0f, 6.0f);
            func_8002C6F8(0x63, 3);
            func_8002C768(0x63, 0x200);
            func_8002C7EC(0x63, 2);
            func_8002C85C(0x63, 0xFF, 0xFF, 0xFF, 0x80);
            func_8002C914(0x63, 0xFF, 0xFF, 0xFF, 0xFF);
            
            func_80034C40(0, 1, 0x63, 0, 304.f, 64.0f, -384.0f, 0, 0xFE, 0, 0);

            break;

        case 0x57:
            
            func_8002B138(0x62, &D_D86D90, &D_D8B140, &D_D8B140_2, &D_D8B160, &D_D8B160_2, &D_D8B1D0, (void*)0x802E4000, (void*)0x802E4D00, (void*)0x802E7400, (void*)0x802E7700, (void*)0x802E7A00, (void*)0x802E7B00, 1, 1);
            func_8002BD90(0x62, 2.0f, 2.0f, 1.0f);
            func_8002C7EC(0x62, 3);
            func_8002C85C(0x62, 0xFF, 0xFF, 0xFF, 0xFF);
            func_8002C914(0x62, 0xFF, 0xFF, 0xFF, 0xFF);
            func_80034C40(0, 0, 0x62, 6, -160.0f, 32.0f, -112.0f, 0xFF, 0xFF, 0, 0);
            
            if (checkLifeEventBit(0x46)) {
                
                func_8002B138(0x63, &D_D86D90, &D_D8B140, &D_D8B140_2, &D_D8B160, &D_D8B160_2, &D_D8B1D0, (void*)0x802E7C00, (void*)0x802E7E80, (void*)0x802E7400, (void*)0x802E7700, (void*)0x802E7A00, (void*)0x802E7B00, 1, 1);
                func_8002BD90(0x63, 1.0f, 1.0f, 1.0f);
                func_8002C7EC(0x63, 3);
                func_8002C85C(0x63, 0xFF, 0xFF, 0xFF, 0xFF);
                func_8002C914(0x63, 0xFF, 0xFF, 0xFF, 0xFF);
                func_80034C40(0, 1, 0x63, 0x10, 8.0f, 72.0f, -168.0f, 0xFF, 0xFF, 0, 0);
            }
            
            if (checkLifeEventBit(0x47)) {
                
                func_8002B138(0x64, &D_D86D90, &D_D8B140, &D_D8B140_2, &D_D8B160, &D_D8B160_2, &D_D8B1D0, (void*)0x802E8100, (void*)0x802E8380, (void*)0x802E7400, (void*)0x802E7700, (void*)0x802E7A00, (void*)0x802E7B00, 1, 1);
                func_8002BD90(0x64, 1.0f, 1.0f, 1.0f);
                func_8002C7EC(0x64, 3);
                func_8002C85C(0x64, 0xFF, 0xFF, 0xFF, 0xFF);
                func_8002C914(0x64, 0xFF, 0xFF, 0xFF, 0xFF);
                func_80034C40(0, 2, 0x64, 9, 104.0f, 0, 24.0f, 0xFF, 0xFF, 0, 0);
                
            }
            
            if (checkLifeEventBit(0x58)) {
                
                func_8002B138(0x65, &D_D86D90, &D_D8B140, &D_D8B140_2, &D_D8B160, &D_D8B160_2, &D_D8B1D0, (void*)0x802E8600, (void*)0x802E8880, (void*)0x802E7400, (void*)0x802E7700, (void*)0x802E7A00, (void*)0x802E7B00, 1, 1);
                func_8002BD90(0x65, 1.0f, 1.0f, 1.0f);
                func_8002C7EC(0x65, 3);
                func_8002C85C(0x65, 0xFF, 0xFF, 0xFF, 0xFF);
                func_8002C914(0x65, 0xFF, 0xFF, 0xFF, 0xFF);
                
                if (checkLifeEventBit(0x16)) {
                    func_80034C40(0, 3, 0x65, 0x12, -192.0f, 64.0f, 104.0f, 0xFF, 0xFF, 0, 0);
                } else {
                    func_80034C40(0, 3, 0x65, 0x12, -176.0f, 0, 88.0f, 0xFF, 0xFF, 0, 0);
                }

            }
            
            if (checkLifeEventBit(0x5A)) {
                
                func_8002B138(0x66, &D_D86D90, &D_D8B140, &D_D8B140_2, &D_D8B160, &D_D8B160_2, &D_D8B1D0, (void*)0x802E8B00, (void*)0x802E8D80, (void*)0x802E7400, (void*)0x802E7700, (void*)0x802E7A00, (void*)0x802E7B00, 1, 1);
                func_8002BD90(0x66, 1.0f, 1.0f, 1.0f);
                func_8002C7EC(0x66, 3);
                func_8002C85C(0x66, 0xFF, 0xFF, 0xFF, 0xFF);
                func_8002C914(0x66, 0xFF, 0xFF, 0xFF, 0xFF);
                func_80034C40(0, 4, 0x66, 0x11, -176.0f, 24.0f, -160.0f, 0xFF, 0xFF, 0, 0);
                
            }
            
            if (checkLifeEventBit(8)) {
                
                func_8002B138(0x67, &D_D86D90, &D_D8B140, &D_D8B140_2, &D_D8B160, &D_D8B160_2, &D_D8B1D0, (void*)0x802E9000, (void*)0x802E9280, (void*)0x802E7400, (void*)0x802E7700, (void*)0x802E7A00, (void*)0x802E7B00, 1, 1);
                func_8002BD90(0x67, 1.0f, 1.0f, 1.0f);
                func_8002C7EC(0x67, 3);
                func_8002C85C(0x67, 0xFF, 0xFF, 0xFF, 0xFF);
                func_8002C914(0x67, 0xFF, 0xFF, 0xFF, 0xFF);
                func_80034C40(0, 5, 0x67, 4, -40.0f, 8.0f, 8.0f, 0xFF, 0xFF, 0, 0);
            
            }
            
            break;

        case 0x5A:
            
            if (checkLifeEventBit(0x5C)) {
                
                func_8002B138(0x62, &D_D86D90, &D_D8B140, &D_D8B140_2, &D_D8B160, &D_D8B160_2, &D_D8B1D0, (void*)0x802E7C00, (void*)0x802E7E80, (void*)0x802E7400, (void*)0x802E7700, (void*)0x802E7A00, (void*)0x802E7B00, 1, 1);
                func_8002BD90(0x62, 1.0f, 1.0f, 1.0f);
                func_8002C7EC(0x62, 3);
                func_8002C85C(0x62, 0xFF, 0xFF, 0xFF, 0xFF);
                func_8002C914(0x62, 0xFF, 0xFF, 0xFF, 0xFF);
                func_80034C40(0, 0, 0x62, 10, -48.f, 16.0f, -80.0f, 0xFF, 0xFF, 0, 0);
        		
            }
            
            break;
        
         case 0x11:

            func_8002B138(0x62, &D_C9D680, &D_CA0DB0, &D_CA0DB0_2, &D_CA0DD0, &D_CA0DD0_2, &D_CA0DF0, (void*)0x802E4000, (void*)0x802E4D00, (void*)0x802E7400, (void*)0x802E7700, (void*)0x802E7A00, (void*)0x802E7B00, 1, 1);
            func_8002BD90(0x62, 3.8f, 1.0f, 6.0f);
            func_8002C6F8(0x62, 3);
            func_8002C768(0x62, 0x200);
            func_8002C7EC(0x62, 2);
            func_8002C85C(0x62, 0xFF, 0xFF, 0xFF, 0x80);
            func_8002C914(0x62, 0xFF, 0xFF, 0xFF, 0xFF);
            func_80034C40(0, 0, 0x62, 0, -288.0f, 0.0f, -140.0f, 0x5A, 0xFE, 0, 0);
             
            func_8002B138(0x63, &D_C9D680, &D_CA0DB0, &D_CA0DB0_2, &D_CA0DD0, &D_CA0DD0_2, &D_CA0DF0, (void*)0x802E5A00, (void*)0x802E6700, (void*)0x802E7400, (void*)0x802E7700, (void*)0x802E7A00, (void*)0x802E7B00, 1, 1);
            func_8002BD90(0x63, 6.0f, 1.0f, 6.0f);
            func_8002C6F8(0x63, 3);
            func_8002C768(0x63, 0x200);
            func_8002C7EC(0x63, 2);
            func_8002C85C(0x63, 0xFF, 0xFF, 0xFF, 0x80);
            func_8002C914(0x63, 0xFF, 0xFF, 0xFF, 0xFF);
            func_80034C40(0, 1, 0x63, 0, -224.0f, 0.0f, 320.0f, 0, 0xFE, 0, 0);
             
            func_8002B138(0x64, &D_C9D680, &D_CA0DB0, &D_CA0DB0_2, &D_CA0DD0, &D_CA0DD0_2, &D_CA0DF0, (void*)0x802E7C00, (void*)0x802E8900, (void*)0x802E9600, (void*)0x802E9700, (void*)0x802E9800, (void*)0x802E9900, 1, 1);
            func_8002BD90(0x64, 6.0f, 1.0f, 6.0f);
            func_8002C6F8(0x64, 3);
            func_8002C768(0x64, 0x200);
            func_8002C7EC(0x64, 2);
            func_8002C85C(0x64, 0xFF, 0xFF, 0xFF, 0x80);
            func_8002C914(0x64, 0xFF, 0xFF, 0xFF, 0xFF);
            func_80034C40(0, 2, 0x64, 1, 160.0f, 32.0f, -160.0f, 0, 0xFE, 0, 0);
             
            if (!checkLifeEventBit(0x45)) {
                
                func_8002B138(0x65, &D_CA0DF0_2, &D_CA3F60, &D_CA3F60_2, &D_CA3F80, &D_CA3F80_2, &D_CA3FA0_2, (void*)0x802E9A00, (void*)0x802EA700, (void*)0x802EB400, (void*)0x802EB500, (void*)0x802EB600, (void*)0x802EB700, 1, 1);
                func_8002BD90(0x65, 1.0f, 1.0f, 1.0f);
                func_8002C7EC(0x65, 3);
                func_8002C85C(0x65, 0xFF, 0xFF, 0xFF, 0xFF);
                func_8002C914(0x65, 0xFF, 0xFF, 0xFF, 0xFF);

                if (gSeason == 4) {
                    func_80034C40(0, 3, 0x65, 0, 192.0f, 88.0f, -208.0f, 0xFF, 0, 0, 0);
                } else {
                    func_80034C40(0, 3, 0x65, 1, 192.0f, 88.0f, -208.0f, 0xFF, 0, 0, 0);
                }
            }
             
            break;

        case 0x15:

            func_8002B138(0x62, &D_C9D680, &D_CA0DB0, &D_CA0DB0_2, &D_CA0DD0, &D_CA0DD0_2, &D_CA0DF0, (void*)0x802E4000, (void*)0x802E4D00, (void*)0x802E7400, (void*)0x802E7700, (void*)0x802E7A00, (void*)0x802E7B00, 1, 1);
            func_8002BD90(0x62, 6.0f, 1.0f, 6.0f);
            func_8002C6F8(0x62, 3);
            func_8002C768(0x62, 0x200);
            func_8002C7EC(0x62, 2);
            func_8002C85C(0x62, 0xFF, 0xFF, 0xFF, 0x80);
            func_8002C914(0x62, 0xFF, 0xFF, 0xFF, 0xFF);
            func_80034C40(0, 0, 0x62, 0, 224.0f, 0, -254.0f, 0x5A, 0xFE, 0, 0);
            
            func_8002B138(0x63, &D_C9D680, &D_CA0DB0, &D_CA0DB0_2, &D_CA0DD0, &D_CA0DD0_2, &D_CA0DF0, (void*)0x802E5A00, (void*)0x802E6700, (void*)0x802E7400, (void*)0x802E7700, (void*)0x802E7A00, (void*)0x802E7B00, 1, 1);
            func_8002BD90(0x63, 6.0f, 1.0f, 6.0f);
            func_8002C6F8(0x63, 3);
            func_8002C768(0x63, 0x200);
            func_8002C7EC(0x63, 2);
            func_8002C85C(0x63, 0xFF, 0xFF, 0xFF, 0x80);
            func_8002C914(0x63, 0xFF, 0xFF, 0xFF, 0xFF);
            func_80034C40(0, 1, 0x63, 0, 192.0f, 0, 416.0f, 0, 0xFE, 0, 0);
        
            if (checkLifeEventBit(0x48)) {
                
                if (gSeason != WINTER) {
                    func_8002B138(0x64, &D_CA3FA0, &D_CA4AF0, &D_CA4AF0_2, &D_CA4B10, &D_CA4B10_2, &D_CA4B30_2, (void*)0x802E7C00, (void*)0x802E8280, (void*)0x802EB400, (void*)0x802EB500, (void*)0x802EB600, (void*)0x802EB700, 1, 1);
                } else {
                    func_8002B138(0x64, &D_CA4B30, &D_CA4FE0, &D_CA4FE0_2, &D_CA5000, &D_CA5000_2, &D_CA5010, (void*)0x802E7C00, (void*)0x802E8280, (void*)0x802EB400, (void*)0x802EB500, (void*)0x802EB600, (void*)0x802EB700, 1, 1);
                }

                func_8002BD90(0x64, 1.0f, 1.0f, 1.0f);
                func_8002C7EC(0x64, 3);
                func_8002C85C(0x64, 0xFF, 0xFF, 0xFF, 0xFF);
                func_8002C914(0x64, 0xFF, 0xFF, 0xFF, 0xFF);
                func_80034C40(0, 2, 0x64, 0, 96.0f, 224.0f, -448.0f, 0xFF, 0, 0, 0);

                if (gSeason != WINTER) {
                    func_8002B138(0x65, &D_CA3FA0, &D_CA4AF0, &D_CA4AF0_2, &D_CA4B10, &D_CA4B10_2, &D_CA4B30_2, (void*)0x802E8900, (void*)0x802E8F80, (void*)0x802EB400, (void*)0x802EB500, (void*)0x802EB600, (void*)0x802EB700, 1, 1);
                } else {
                    func_8002B138(0x65, &D_CA4B30, &D_CA4FE0, &D_CA4FE0_2, &D_CA5000, &D_CA5000_2, &D_CA5010, (void*)0x802E8900, (void*)0x802E8F80, (void*)0x802EB400, (void*)0x802EB500, (void*)0x802EB600, (void*)0x802EB700, 1, 1);
                }

                func_8002BD90(0x65, 1.0f, 1.0f, 1.0f);
                func_8002C7EC(0x65, 3);
                func_8002C85C(0x65, 0xFF, 0xFF, 0xFF, 0xFF);
                func_8002C914(0x65, 0xFF, 0xFF, 0xFF, 0xFF);
                func_80034C40(0, 3, 0x65, 0, 96.0f, 224.0f, -376, 0xFF, 0, 0, 0);

                if (gSeason != WINTER) {
                    func_8002B138(0x66, &D_CA3FA0, &D_CA4AF0, &D_CA4AF0_2, &D_CA4B10, &D_CA4B10_2, &D_CA4B30_2, (void*)0x802E9A00, (void*)0x802EA080, (void*)0x802EB400, (void*)0x802EB500, (void*)0x802EB600, (void*)0x802EB700, 1, 1);
                } else {
                    func_8002B138(0x66, &D_CA4B30, &D_CA4FE0, &D_CA4FE0_2, &D_CA5000, &D_CA5000_2, &D_CA5010, (void*)0x802E9A00, (void*)0x802EA080, (void*)0x802EB400, (void*)0x802EB500, (void*)0x802EB600, (void*)0x802EB700, 1, 1);
                }

                func_8002BD90(0x66, 1.0f, 1.0f, 1.0f);
                func_8002C7EC(0x66, 3);
                func_8002C85C(0x66, 0xFF, 0xFF, 0xFF, 0xFF);
                func_8002C914(0x66, 0xFF, 0xFF, 0xFF, 0xFF);
                func_80034C40(0, 4, 0x66, 0, 304.0f, 224.0f, -432.0f, 0xFF, 0, 0, 0);

                if (gSeason != WINTER) {
                    func_8002B138(0x67, &D_CA3FA0, &D_CA4AF0, &D_CA4AF0_2, &D_CA4B10, &D_CA4B10_2, &D_CA4B30_2, (void*)0x802EA700, (void*)0x802EAD80, (void*)0x802EB400, (void*)0x802EB500, (void*)0x802EB600, (void*)0x802EB700, 1, 1);
                } else {
                    func_8002B138(0x67, &D_CA4B30, &D_CA4FE0, &D_CA4FE0_2, &D_CA5000, &D_CA5000_2, &D_CA5010, (void*)0x802EA700, (void*)0x802EAD80, (void*)0x802EB400, (void*)0x802EB500, (void*)0x802EB600, (void*)0x802EB700, 1, 1);
                }

                func_8002BD90(0x67, 1.0f, 1.0f, 1.0f);
                func_8002C7EC(0x67, 3);
                func_8002C85C(0x67, 0xFF, 0xFF, 0xFF, 0xFF);
                func_8002C914(0x67, 0xFF, 0xFF, 0xFF, 0xFF);
                func_80034C40(0, 5, 0x67, 0, 304.0f, 224.0f, -368.0f, 0xFF, 0, 0, 0);
                
            }
            
            break;

        case 0x19:
            
            if (checkLifeEventBit(0x48)) {
                
                func_8002B138(0x62, &D_CA5040, &D_CA5430, &D_CA5430_2, &D_CA5450, &D_CA5450_2, &D_CA5460_2, (void*)0x802E4000, (void*)0x802E4400, (void*)0x802E4800, (void*)0x802E4900, (void*)0x802E4A00, (void*)0x802E4B00, 1, 1);
                func_8002BD90(0x62, 6.0f, 1.0f, 6.0f);
                func_8002C6F8(0x62, 3);
                func_8002C768(0x62, 0x200);
                func_8002C7EC(0x62, 2);
                func_8002C85C(0x62, 0xFF, 0xFF, 0xFF, 0x80);
                func_8002C914(0x62, 0xFF, 0xFF, 0xFF, 0xFF);
                func_80034C40(0, 0, 0x62, 0, 0, 216.0f, -128.0f, 0, 0xFE, 0, 0);

                if (gSeason != WINTER) {
                    func_8002B138(0x64, &D_CA3FA0, &D_CA4AF0, &D_CA4AF0_2, &D_CA4B10, &D_CA4B10_2, &D_CA4B30_2, (void*)0x802E7C00, (void*)0x802E8280, (void*)0x802EB400, (void*)0x802EB500, (void*)0x802EB600, (void*)0x802EB700, 1, 1);
                } else {
                    func_8002B138(0x64, &D_CA4B30, &D_CA4FE0, &D_CA4FE0_2, &D_CA5000, &D_CA5000_2, &D_CA5010, (void*)0x802E7C00, (void*)0x802E8280, (void*)0x802EB400, (void*)0x802EB500, (void*)0x802EB600, (void*)0x802EB700, 1, 1);
                }

                func_8002BD90(0x64, 1.0f, 1.0f, 1.0f);
                func_8002C7EC(0x64, 3);
                func_8002C85C(0x64, 0xFF, 0xFF, 0xFF, 0xFF);
                func_8002C914(0x64, 0xFF, 0xFF, 0xFF, 0xFF);
                func_80034C40(0, 2, 0x64, 0, -192.0f, 228.0f, 4.0f, 0xFF, 0, 0, 0);

                if (gSeason != WINTER) {
                    func_8002B138(0x65, &D_CA3FA0, &D_CA4AF0, &D_CA4AF0_2, &D_CA4B10, &D_CA4B10_2, &D_CA4B30_2, (void*)0x802E8900, (void*)0x802E8F80, (void*)0x802EB400, (void*)0x802EB500, (void*)0x802EB600, (void*)0x802EB700, 1, 1);
                } else {
                    func_8002B138(0x65, &D_CA4B30, &D_CA4FE0, &D_CA4FE0_2, &D_CA5000, &D_CA5000_2, &D_CA5010, (void*)0x802E8900, (void*)0x802E8F80, (void*)0x802EB400, (void*)0x802EB500, (void*)0x802EB600, (void*)0x802EB700, 1, 1);
                }
                
                func_8002BD90(0x65, 1.0f, 1.0f, 1.0f);
                func_8002C7EC(0x65, 3);
                func_8002C85C(0x65, 0xFF, 0xFF, 0xFF, 0xFF);
                func_8002C914(0x65, 0xFF, 0xFF, 0xFF, 0xFF);
                func_80034C40(0, 3, 0x65, 0, -192.0f, 224.0f, 64.0f, 0xFF, 0, 0, 0);

                if (gSeason != WINTER) {
                    func_8002B138(0x66, &D_CA3FA0, &D_CA4AF0, &D_CA4AF0_2, &D_CA4B10, &D_CA4B10_2, &D_CA4B30_2, (void*)0x802E9A00, (void*)0x802EA080, (void*)0x802EB400, (void*)0x802EB500, (void*)0x802EB600, (void*)0x802EB700, 1, 1);
                } else {
                    func_8002B138(0x66, &D_CA4B30, &D_CA4FE0, &D_CA4FE0_2, &D_CA5000, &D_CA5000_2, &D_CA5010, (void*)0x802E9A00, (void*)0x802EA080, (void*)0x802EB400, (void*)0x802EB500, (void*)0x802EB600, (void*)0x802EB700, 1, 1);
                }
                
                func_8002BD90(0x66, 1.0f, 1.0f, 1.0f);
                func_8002C7EC(0x66, 3);
                func_8002C85C(0x66, 0xFF, 0xFF, 0xFF, 0xFF);
                func_8002C914(0x66, 0xFF, 0xFF, 0xFF, 0xFF);
                func_80034C40(0, 4, 0x66, 0, -408.0f, 224.0f, -16.0f, 0xFF, 0, 0, 0);

                if (gSeason != WINTER) {
                    func_8002B138(0x67, &D_CA3FA0, &D_CA4AF0, &D_CA4AF0_2, &D_CA4B10, &D_CA4B10_2, &D_CA4B30_2, (void*)0x802EA700, (void*)0x802EAD80, (void*)0x802EB400, (void*)0x802EB500, (void*)0x802EB600, (void*)0x802EB700, 1, 1);
                } else {
                    func_8002B138(0x67, &D_CA4B30, &D_CA4FE0, &D_CA4FE0_2, &D_CA5000, &D_CA5000_2, &D_CA5010, (void*)0x802EA700, (void*)0x802EAD80, (void*)0x802EB400, (void*)0x802EB500, (void*)0x802EB600, (void*)0x802EB700, 1, 1);
                }
                
                func_8002BD90(0x67, 1.0f, 1.0f, 1.0f);
                func_8002C7EC(0x67, 3);
                func_8002C85C(0x67, 0xFF, 0xFF, 0xFF, 0xFF);
                func_8002C914(0x67, 0xFF, 0xFF, 0xFF, 0xFF);
                func_80034C40(0, 5, 0x67, 0, -408.0f, 224.0f, 48.0f, 0xFF, 0, 0, 0);

                
            } else {
                
                func_8002B138(0x62, &D_CA3FA0, &D_CA4AF0, &D_CA4AF0_2, &D_CA4B10, &D_CA4B10_2, &D_CA4B30_2, (void*)0x802E4000, (void*)0x802E4D00, (void*)0x802E7400, (void*)0x802E7700, (void*)0x802E7A00, (void*)0x802E7B00, 1, 1);
                func_8002BD90(0x62, 1.0f, 1.0f, 1.0f);
                func_8002C7EC(0x62, 3);
                func_8002C85C(0x62, 0xFF, 0xFF, 0xFF, 0xFF);
                func_8002C914(0x62, 0xFF, 0xFF, 0xFF, 0xFF);
                func_80034C40(0, 0, 0x62, 3, -160.0f, 224.0f, 8.0f, 0xFF, 0, 0, 0);
                
            }
            
            if (checkLifeEventBit(0x4C)) {
            
                func_8002B138(0x63, &D_CA5460, &D_CA7500, &D_CA7500_2, &D_CA7520, &D_CA7520_2, &D_CA7540, (void*)0x802E4C00, (void*)0x802E5800, (void*)0x802E6400, (void*)0x802E6500, (void*)0x802E6600, (void*)0x802E6700, 1, 1);
                func_8002BD90(0x63, 2.0f, 2.0f, 1.0f);
                func_8002C7EC(0x63, 2);
                func_8002C85C(0x63, 0xFF, 0xFF, 0xFF, 0x60);
                func_8002C914(0x63, 0xFF, 0xFF, 0xFF, 0xFF);
                func_80034C40(0, 1, 0x63, 0, 0.0f, 256.0f, -136.0f, 0xFF, 0xFE, 0, 0);

            }
            
            break;

        case 0x23:
            
            func_8002B138(0x62, &D_CA7540, &D_CA87D0, &D_CA87D0_2, &D_CA87F0, &D_CA87F0_2, &D_CA8800, (void*)0x802E4000, (void*)0x802E4D00, (void*)0x802E7400, (void*)0x802E7700, (void*)0x802E7A00, (void*)0x802E7B00, 1, 1);
            func_8002BD90(0x62, 6.0f, 1.0f, 6.0f);
            func_8002C6F8(0x62, 3);
            func_8002C768(0x62, 0x200);
            func_8002C7EC(0x62, 2);
            func_8002C85C(0x62, 0xFF, 0xFF, 0xFF, 0x80);
            func_8002C914(0x62, 0xFF, 0xFF, 0xFF, 0xFF);
            func_80034C40(0, 0, 0x62, 0, 0.0f, 64.0f, -96.0f, 0x5A, 0xFE, 0, 0);

            break;

        case 9:
            
            func_8002B138(0x62, &D_C951A0, &D_C9D640, &D_C9D640_2, &D_C9D660, &D_C9D660_2, &D_C9D680, (void*)0x802E4000, (void*)0x802E6400, (void*)0x802E8800, (void*)0x802E8900, (void*)0x802E8A00, (void*)0x802E8B00, 1, 1);
            func_8002BD90(0x62, 6.0f, 1.0f, 5.4f);
            func_8002C6F8(0x62, 3);
            func_8002C768(0x62, 0x200);
            func_8002C7EC(0x62, 2);
            func_8002C85C(0x62, 0xFF, 0xFF, 0xFF, 0x80);
            func_8002C914(0x62, 0xFF, 0xFF, 0xFF, 0xFF);
            func_80034C40(0, 0, 0x62, 0, 304.f, 112.0f, -10.0f, 0, 0xFE, 0, 1);
            
            break;

        case 0x2D:
            
            func_8002B138(0x62, &D_CAA080, &D_CAB010, &D_CAB010_2, &D_CAB030, &D_CAB030_2, &D_CAB040, (void*)0x802E4000, (void*)0x802E4000, (void*)0x802E7400, (void*)0x802E7700, (void*)0x802E7A00, (void*)0x802E7B00, 1, 1);
            func_8002BD90(0x62, 6.0f, 1.0f, 6.0f);
            func_8002C6F8(0x62, 3);
            func_8002C768(0x62, 0x200);
            func_8002C7EC(0x62, 2);
            func_8002C85C(0x62, 0xFF, 0xFF, 0xFF, 0x80);
            func_8002C914(0x62, 0xFF, 0xFF, 0xFF, 0xFF);
            func_80034C40(0, 0, 0x62, 0, -40.0f, 64.0f, -208.0f, 0, 0xFE, 0, 0);    

            break;
        
        case 0x35:
            
            func_8002B138(0x62, &D_CA90D0, &D_CAA040, &D_CAA040_2, &D_CAA060, &D_CAA060_2, &D_CAA080_2, (void*)0x802E7C00, (void*)0x802E7C00, (void*)0x802E9600, (void*)0x802E9700, (void*)0x802E9800, (void*)0x802E9900, 1, 1);
            func_8002BD90(0x62, 5.0f, 1.0f, 5.0f);
            func_8002C6F8(0x62, 3);
            func_8002C768(0x62, 0x200);
            func_8002C7EC(0x62, 2);
            func_8002C85C(0x62, 0xFF, 0xFF, 0xFF, 0x80);
            func_8002C914(0x62, 0xFF, 0xFF, 0xFF, 0xFF);
            func_80034C40(0, 0, 0x62, 0, -256.0f, 104.f, -24.0f, 0, 0xFE, 0, 0);
            
            if (gSeason == 1) {
                
                if (18 < gDayOfMonth && gDayOfMonth < 23) {
                    func_8002B138(0x63, &D_D86D90, &D_D8B140, &D_D8B140_2, &D_D8B160, &D_D8B160_2, &D_D8B1D0, (void*)0x802E4000, (void*)0x802E4D00, (void*)0x802E7400, (void*)0x802E7700, (void*)0x802E7A00, (void*)0x802E7B00, 1, 1);
                    func_8002BD90(0x63, 1.0f, 1.0f, 1.0f);
                    func_8002C7EC(0x63, 3);
                    func_8002C85C(0x63, 0xFF, 0xFF, 0xFF, 0xFF);
                    func_8002C914(0x63, 0xFF, 0xFF, 0xFF, 0xFF);
                    func_80034C40(0, 1, 0x63, 0xD, 80.0f, 128.0f, 208.0f, 0xFF, 0xFF, 0, 0);
                }

                if ((0x351 < gCutsceneIndex && gCutsceneIndex < 0x355) || gCutsceneIndex == 0x3B6) {
                    
                   for (j = 4; j < 12; j++) {

                        func_8002B138(j + 0x63, &D_CA8800, &D_CA90A0, &D_CA90A0_2, &D_CA90C0, &D_CA90C0_2, &D_CA90D0_2, (void*)0x802E4000, 0, (void*)0x802E7400, (void*)0x802E7700, (void*)0x802E7A00, 0, 0, 1);
                        func_8002BD90(j + 0x63, 1.0f, 1.0f, 1.0f);
                        func_8002C7EC(j + 0x63, 3);
                        func_8002C85C(j + 0x63, 0xFF, 0xFF, 0xFF, 0xFF);
                        func_8002C914(j + 0x63, 0xFF, 0xFF, 0xFF, 0xFF);
                        
                    } 

                    func_80034C40(0, 5, 0x67, 0, 176.0f, 96.0f, -96.0f, 0xFF, 0xFE, 0, 0);
                    func_80034C40(0, 6, 0x68, 1, 176.0f, 96.0f, -128.0f, 0xFF, 0xFE, 0, 0);
                    func_80034C40(0, 7, 0x69, 0, 176.0f, 96.0f, -160.0f, 0xFF, 0xFE, 0, 0);
                    func_80034C40(0, 8, 0x6A, 1, 176.0f, 96.0f, -192.0f, 0xFF, 0xFE, 0, 0);
                    func_80034C40(0, 9, 0x6B, 0, 176.0f, 96.0f, 64.0f, 0xFF, 0xFE, 0, 0);
                    func_80034C40(0, 0xA, 0x6C, 1, 176.0f, 96.0f, 96.0f, 0xFF, 0xFE, 0, 0);
                    func_80034C40(0, 0xB, 0x6D, 0, 176.0f, 96.0f, 128.0f, 0xFF, 0xFE, 0, 0);
                    func_80034C40(0, 0xC, 0x6E, 1, 176.0f, 96.0f, 160.0f, 0xFF, 0xFE, 0, 0);
                    
                }

            }
            
            if (func_800DDDFC(0x35)) {
                func_800DC7BC(0x35);
            }
            if (func_800DDDFC(0x36)) {
                func_800DC7BC(0x36);
            }
            if (func_800DDDFC(0x37)) {
                func_800DC7BC(0x37);
                return;
            }
            
            break;

       case 0x5B:
           
            func_8002B138(0x62, &D_CAB0D0, &D_CAB310, &D_CAB310_2, &D_CAB330, &D_CAB330_2, &D_CAB340, (void*)0x802E4000, (void*)0x802E4D00, (void*)0x802E7400, (void*)0x802E7700, (void*)0x802E7A00, (void*)0x802E7B00, 1, 1);
            func_8002BD90(0x62, 6.0f, 1.0f, 6.0f);
            func_8002C6F8(0x62, 3);
            func_8002C768(0x62, 0x200);
            func_8002C7EC(0x62, 2);
            func_8002C85C(0x62, 0xFF, 0xFF, 0xFF, 0x80);
            func_8002C914(0x62, 0xFF, 0xFF, 0xFF, 0xFF);
            func_80034C40(0, 0, 0x62, 0, -128.0f, 8.0f, -128.0f, 0, 0xFE, 0, 0);
            func_8002B138(0x63, &D_CA5460, &D_CA7500, &D_CA7500_2, &D_CA7520, &D_CA7520_2, &D_CA7540_2, (void*)0x802E7C00, (void*)0x802E8900, (void*)0x802E9600, (void*)0x802E9700, (void*)0x802E9800, (void*)0x802E9900, 1, 1);
            func_8002BD90(0x63, 1.6f, 1.6f, 1.0f);
            func_8002C7EC(0x63, 2);
            func_8002C85C(0x63, 0xFF, 0xFF, 0xFF, 0x60);
            func_8002C914(0x63, 0xFF, 0xFF, 0xFF, 0xFF);
            func_80034C40(0, 1, 0x63, 0, -64.0f, 64.0f, -80.0f, 0xFF, 0xFE, 0, 0);
           
            break;
        
        case 0x32:
            
            if (func_800DDDFC(0)) {
                func_800DC7BC(0);
            }
            if (func_800DDDFC(1)) {
                func_800DC7BC(1);
            }
            if (func_800DDDFC(2)) {
                func_800DC7BC(2);
            }
            if (func_800DDDFC(4)) {
                func_800DC7BC(4);
            }
            if (func_800DDDFC(5)) {
                func_800DC7BC(5);
            }
            if (func_800DDDFC(6)) {
                func_800DC7BC(6);
            }
            if (func_800DDDFC(7)) {
                func_800DC7BC(7);
            }
            if (func_800DDDFC(3)) {
                func_800DC7BC(3);
            }
            if (func_800DDDFC(8)) {
                func_800DC7BC(8);
            }
            if (func_800DDDFC(0xA)) {
                func_800DC7BC(0xA);
                return;
            }
            break;

       case 0x6:
           
            if (func_800DDDFC(0x1A)) {
                func_800DC7BC(0x1A);
            }
            if (func_800DDDFC(0x1D)) {
                func_800DC7BC(0x1D);
            }
            if (func_800DDDFC(0x1B)) {
                func_800DC7BC(0x1B);
            }
            if (func_800DDDFC(0x1C)) {
                func_800DC7BC(0x1C);
                return;
            }
            break;
        
        case 0x39:
            
            if (func_800DDDFC(0x15)) {
                func_800DC7BC(0x15);
            }
            if (func_800DDDFC(0x14)) {
                func_800DC7BC(0x14);
            }
            if (func_800DDDFC(0x13)) {
                func_800DC7BC(0x13);
            }
            if (func_800DDDFC(0x16)) {
                func_800DC7BC(0x16);
            }
            if (func_800DDDFC(0x18)) {
                func_800DC7BC(0x18);
            }
            if (func_800DDDFC(0x19)) {
                func_800DC7BC(0x19);
            }
            if (func_800DDDFC(0x17)) {
                func_800DC7BC(0x17);
                func_80038AE0(0, 0);
            }
            
            break;

        case 0x34:
            
            if (func_800DDDFC(0x25)) {
                func_800DC7BC(0x25);
            }
            if (func_800DDDFC(0x26)) {
                func_800DC7BC(0x26);
            }
            if (func_800DDDFC(0x27)) {
                func_800DC7BC(0x27);
            }
            
            break;

        case 0x59:
            
            for (i = 0; i < 6; i++) { 
                if (gChickens[i].flags & 0x10) {
                    func_80073244(i);
                }
            }
            
            break;

        case 0x58:

            for (i = 0; i < 8; i++) {
                
                if (gFarmAnimals[i].flags & 8) {
                    func_8007341C(i);
                }
                if (gFarmAnimals[i].flags & 0x10) {
                    func_8007341C(8);
                }
                
            }
            
            break;

        case 0x47:
            
            func_8002B138(0x62, &D_CAB340, &D_CAD490, &D_CAD490_2, &D_CAD4B0, &D_CAD4B0_2, &D_CAD4D0, (void*)0x802E4000, (void*)0x802E4D00, (void*)0x802E7400, (void*)0x802E7700, (void*)0x802E7A00, (void*)0x802E7B00, 1, 1);
            func_8002BD90(0x62, 2.0f, 2.0f, 1.0f);
            func_8002C7EC(0x62, 3);
            func_8002C85C(0x62, 0xFF, 0xFF, 0xFF, 0xFF);
            func_8002C914(0x62, 0xFF, 0xFF, 0xFF, 0xFF);
            
            if (checkSpecialDialogueBit(0x14B)) {
                func_80034C40(0, 0, 0x62, 1, -416.0f, 144.0f, -352.0f, 0xFF, 0, 0, 0);
            } else {
                func_80034C40(0, 0, 0x62, 0, -416.0f, 144.0f, -352.0f, 0xFF, 0, 0, 0);
            }

        break;
    }
    
}
*/

//INCLUDE_ASM(const s32, "level", func_80073244);

void func_80073244(u8 itemIndex) {

    Vec3f arr[6];

    MemoryRead_32 *ptr = (MemoryRead_32*)arr;
    MemoryRead_32 *ptr2 = (MemoryRead_32*)D_8011FB28;

    do {
        *(Aligned32*)ptr++ = *(Aligned32*)ptr2++;
    } while (ptr2 != (D_8011FB28 + 0x40));    

    *(Vec2f*)ptr = *(Vec2f*)ptr2;

    func_8002B138(itemIndex +  0x62, &homeItemsTexture_ROM_START, &homeItemsTexture_ROM_END, &homeItemsAssetsIndex_ROM_START, &homeItemsAssetsIndex_ROM_END, &homeItemsSpritesheetIndex_ROM_START, &homeItemsSpritesheetIndex_ROM_END, (void*)0x802E4000, (void*)0x802E4D00, (void*)0x802E7400, (void*)0x802E7700, (void*)0x802E7A00, (void*)0x802E7B00, 1, 1);
    func_8002BD90(itemIndex +  0x62, 1.0f, 1.0f, 1.0f);
    func_8002C7EC(itemIndex +  0x62, 3);
    func_8002C85C(itemIndex +  0x62, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002C914(itemIndex +  0x62, 0xFF, 0xFF, 0xFF, 0xFF);

    func_80034C40(MAIN_MAP_INDEX, itemIndex, itemIndex + 0x62, 0xC, arr[itemIndex].x, arr[itemIndex].y, arr[itemIndex].z, 0xFF, 0xFF, 0, 0);
    
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

    func_8002B138(itemIndex + 0x62, &homeItemsTexture_ROM_START, &homeItemsTexture_ROM_END, &homeItemsAssetsIndex_ROM_START, &homeItemsAssetsIndex_ROM_END, &homeItemsSpritesheetIndex_ROM_START, &homeItemsSpritesheetIndex_ROM_END, (void* )0x802E4000, (void* )0x802E4D00, (void* )0x802E7400, (void* )0x802E7700, (void* )0x802E7A00, (void* )0x802E7B00, (void* )1, (u8) (void* )1);
    func_8002BD90(itemIndex + 0x62, 1.0f, 1.0f, 1.0f);
    func_8002C7EC(itemIndex + 0x62, 3);
    func_8002C85C(itemIndex + 0x62, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002C914(itemIndex + 0x62, 0xFF, 0xFF, 0xFF, 0xFF);

    func_80034C40(MAIN_MAP_INDEX, itemIndex, itemIndex + 0x62, 0xB, arr[itemIndex].x, arr[itemIndex].y, arr[itemIndex].z, 0xFF, 0xFF, 0, 0);
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

    func_8002B138(arg0 + 0x62, &homeItemsTexture_ROM_START, &homeItemsTexture_ROM_END, &homeItemsAssetsIndex_ROM_START, &homeItemsAssetsIndex_ROM_END, &homeItemsSpritesheetIndex_ROM_START, &homeItemsSpritesheetIndex_ROM_END, (void* )0x802E4000, (void* )0x802E4D00, (void* )0x802E7400, (void* )0x802E7700, (void* )0x802E7A00, (void* )0x802E7B00, (void* )1, (u8) (void* )1);
    func_8002BD90(arg0 + 0x62, 1.0f, 1.0f, 1.0f);
    func_8002C7EC(arg0 + 0x62, 3);
    func_8002C85C(arg0 + 0x62, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002C914(arg0 + 0x62, 0xFF, 0xFF, 0xFF, 0xFF);

    func_80034C40(MAIN_MAP_INDEX, arg0, arg0 + 0x62, 0xB, arr[arg0].x, arr[arg0].y, arr[arg0].z, 0xFF, 0xFF, 0, 0);
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

// map struct 7
// matches but have to match preceding rodata
/*
void func_800735FC(u16 levelIndex) {

    switch (levelIndex) {
        case FARM:
            func_80038900(MAIN_MAP_INDEX, 9, 0, 0, 1);
            func_80038900(MAIN_MAP_INDEX, 0xA, 0, 2, 3);
            func_80038900(MAIN_MAP_INDEX, 0xA, 1, 3, 2);
            func_80038900(MAIN_MAP_INDEX, 0xB, 0, 4, 1);
            func_80038900(MAIN_MAP_INDEX, 0xC, 0, 1, 1);
            func_80038900(MAIN_MAP_INDEX, 0xD, 0, 0xC, 1);
            func_80038900(MAIN_MAP_INDEX, 0xE, 0, 0xA, 1);
            func_80038900(MAIN_MAP_INDEX, 0xF, 0, 0xB, 1);
            func_80038900(MAIN_MAP_INDEX, 0x10, 0, 0xD, 1);
            func_80038900(MAIN_MAP_INDEX, 0x11, 0, 9, 1);
            func_80038900(MAIN_MAP_INDEX, 0x13, 0, 0xE, 1);
            func_80038900(MAIN_MAP_INDEX, 0x14, 0, 0xF, 1);
            func_80038900(MAIN_MAP_INDEX, 0x12, 0, 0x11, 2);
            func_80038900(MAIN_MAP_INDEX, 0x12, 1, 0x12, 2);
            func_80038900(MAIN_MAP_INDEX, 0x12, 2, 0x11, 2);
            func_80038900(MAIN_MAP_INDEX, 0x12, 3, 0x13, 2);
            func_80038900(MAIN_MAP_INDEX, 0x12, 4, 0x14, 2);
            func_80038900(MAIN_MAP_INDEX, 0x1A, 0, 0x1A, 1);
            func_80038900(MAIN_MAP_INDEX, 0x1B, 0, 2, 3);
            func_80038900(MAIN_MAP_INDEX, 0x1B, 1, 3, 8);
            func_80038900(MAIN_MAP_INDEX, 0x1B, 2, 2, 2);
            func_80038900(MAIN_MAP_INDEX, 0x1B, 3, 0x1B, 2);
            func_80038900(MAIN_MAP_INDEX, 0x1C, 0, 0x1C, 1);
            func_80038900(MAIN_MAP_INDEX, 0x1D, 0, 0x1D, 1);
            func_80038900(MAIN_MAP_INDEX, 0x1E, 0, 0x1E, 1);
            func_80038900(MAIN_MAP_INDEX, 0, 0, 0x20, 0);
            func_80038900(MAIN_MAP_INDEX, 1, 0, 0x21, 0);
            func_80038900(MAIN_MAP_INDEX, 2, 0, 0x22, 0);
            func_80038900(MAIN_MAP_INDEX, 3, 0, 0x1F, 0);
            func_80038900(MAIN_MAP_INDEX, 4, 0, 5, 0);
            func_80038900(MAIN_MAP_INDEX, 5, 0, 6, 0);
            func_80038900(MAIN_MAP_INDEX, 6, 0, 7, 0);
            func_80038900(MAIN_MAP_INDEX, 7, 0, 8, 0);
            func_80038900(MAIN_MAP_INDEX, 8, 0, 0x10, 0);
            break;
        default:
            break;
        case GREENHOUSE:
            func_80038900(MAIN_MAP_INDEX, 0, 0, 0, 2);
            func_80038900(MAIN_MAP_INDEX, 0, 1, 1, 2);
            func_80038900(MAIN_MAP_INDEX, 0, 2, 0, 2);
            func_80038900(MAIN_MAP_INDEX, 0, 3, 2, 2);
            func_80038900(MAIN_MAP_INDEX, 0, 4, 3, 2);
            func_80038900(MAIN_MAP_INDEX, 4, 0, 4, 0);
            func_80038900(MAIN_MAP_INDEX, 5, 0, 5, 0);
            func_80038900(MAIN_MAP_INDEX, 6, 0, 6, 0);
            func_80038900(MAIN_MAP_INDEX, 7, 0, 7, 0);
            func_80038900(MAIN_MAP_INDEX, 8, 0, 8, 0);
            break;
        case HOUSE:
            func_80038900(MAIN_MAP_INDEX, 0, 0, 2, 1);
            func_80038900(MAIN_MAP_INDEX, 1, 0, 9, 1);
            func_80038900(MAIN_MAP_INDEX, 2, 0, 3, 1);
            func_80038900(MAIN_MAP_INDEX, 3, 0, 0, 1);
            func_80038900(MAIN_MAP_INDEX, 4, 0, 7, 1);
            func_80038900(MAIN_MAP_INDEX, 5, 0, 6, 1);
            func_80038900(MAIN_MAP_INDEX, 6, 0, 5, 1);
            func_80038900(MAIN_MAP_INDEX, 7, 0, 4, 1);
            func_80038900(MAIN_MAP_INDEX, 8, 0, 1, 1);
            func_80038900(MAIN_MAP_INDEX, 9, 0, 8, 1);
            func_80038900(MAIN_MAP_INDEX, 0xA, 0, 0xA, 1);
            func_80038900(MAIN_MAP_INDEX, 0xB, 0, 0xB, 1);
            func_80038900(MAIN_MAP_INDEX, 0xC, 0, 0xC, 1);
            break;
        case KITCHEN:
            func_80038900(MAIN_MAP_INDEX, 0, 0, 0, 1);
            func_80038900(MAIN_MAP_INDEX, 1, 0, 1, 1);
            func_80038900(MAIN_MAP_INDEX, 2, 0, 2, 1);
            func_80038900(MAIN_MAP_INDEX, 3, 0, 3, 1);
            break;
        case BATHROOM:
            func_80038900(MAIN_MAP_INDEX, 0, 0, 0, 1);
            func_80038900(MAIN_MAP_INDEX, 1, 1, 1, 1);
            func_80038900(MAIN_MAP_INDEX, 2, 2, 3, 1);
            func_80038900(MAIN_MAP_INDEX, 3, 3, 2, 1);
            break;
        case COOP:
            func_80038900(MAIN_MAP_INDEX, 0, 0, 0, 2);
            func_80038900(MAIN_MAP_INDEX, 0, 1, 1, 2);
            func_80038900(MAIN_MAP_INDEX, 0, 2, 0, 2);
            func_80038900(MAIN_MAP_INDEX, 0, 3, 2, 2);
            func_80038900(MAIN_MAP_INDEX, 0, 4, 3, 2);
            func_80038900(MAIN_MAP_INDEX, 1, 0, 4, 1);
            break;
        case BARN:
            func_80038900(MAIN_MAP_INDEX, 0, 0, 0, 2);
            func_80038900(MAIN_MAP_INDEX, 0, 1, 1, 2);
            func_80038900(MAIN_MAP_INDEX, 0, 2, 0, 2);
            func_80038900(MAIN_MAP_INDEX, 0, 3, 2, 2);
            func_80038900(MAIN_MAP_INDEX, 0, 4, 3, 2);
            break;
        case VILLAGE_1:
            func_80038900(MAIN_MAP_INDEX, 0, 0, 0, 1);
            func_80038900(MAIN_MAP_INDEX, 1, 0, 1, 1);
            func_80038900(MAIN_MAP_INDEX, 2, 0, 2, 1);
            func_80038900(MAIN_MAP_INDEX, 3, 0, 4, 1);
            func_80038900(MAIN_MAP_INDEX, 4, 0, 3, 1);
            func_80038900(MAIN_MAP_INDEX, 5, 0, 5, 1);
            func_80038900(MAIN_MAP_INDEX, 6, 0, 6, 1);
            func_80038900(MAIN_MAP_INDEX, 7, 0, 7, 1);
            func_80038900(MAIN_MAP_INDEX, 8, 0, 8, 1);
            func_80038900(MAIN_MAP_INDEX, 9, 0, 9, 1);
            func_80038900(MAIN_MAP_INDEX, 0xA, 0, 0xA, 1);
            func_80038900(MAIN_MAP_INDEX, 0xB, 0, 0xB, 1);
            func_80038900(MAIN_MAP_INDEX, 0xC, 0, 0xC, 1);
            break;
        case VILLAGE_2:
            func_80038900(MAIN_MAP_INDEX, 0, 0, 0, 1);
            func_80038900(MAIN_MAP_INDEX, 1, 0, 1, 1);
            func_80038900(MAIN_MAP_INDEX, 2, 0, 2, 1);
            func_80038900(MAIN_MAP_INDEX, 3, 0, 3, 1);
            break;

        case SQUARE:
            switch (gSeason) {                          
                case SPRING:                                     
                    func_80038900(MAIN_MAP_INDEX, 0, 0, 0, 1);
                    func_80038900(MAIN_MAP_INDEX, 1, 0, 1, 1);
                    func_80038900(MAIN_MAP_INDEX, 2, 0, 2, 1);
                    func_80038900(MAIN_MAP_INDEX, 3, 0, 3, 1);
                    func_80038900(MAIN_MAP_INDEX, 4, 0, 4, 1);
                    func_80038900(MAIN_MAP_INDEX, 5, 0, 5, 1);
                    func_80038900(MAIN_MAP_INDEX, 6, 0, 6, 1);
                    func_80038900(MAIN_MAP_INDEX, 7, 0, 7, 1);
                    func_80038900(MAIN_MAP_INDEX, 8, 0, 8, 1);
                    func_80038900(MAIN_MAP_INDEX, 9, 0, 9, 1);
                    func_80038900(MAIN_MAP_INDEX, 0xA, 0, 0xA, 1);
                    break;
                case SUMMER:                                     
                    func_80038900(MAIN_MAP_INDEX, 0, 0, 0, 1);
                    func_80038900(MAIN_MAP_INDEX, 1, 0, 1, 1);
                    func_80038900(MAIN_MAP_INDEX, 2, 0, 2, 1);
                    func_80038900(MAIN_MAP_INDEX, 3, 0, 3, 1);
                    func_80038900(MAIN_MAP_INDEX, 4, 0, 4, 1);
                    func_80038900(MAIN_MAP_INDEX, 5, 0, 5, 1);
                    func_80038900(MAIN_MAP_INDEX, 6, 0, 6, 1);
                    break;
                case AUTUMN:                                     
                    func_80038900(MAIN_MAP_INDEX, 0, 0, 0, 1);
                    func_80038900(MAIN_MAP_INDEX, 1, 0, 1, 1);
                    func_80038900(MAIN_MAP_INDEX, 2, 0, 2, 1);
                    func_80038900(MAIN_MAP_INDEX, 3, 0, 3, 1);
                    func_80038900(MAIN_MAP_INDEX, 4, 0, 4, 1);
                    func_80038900(MAIN_MAP_INDEX, 5, 0, 5, 1);
                    func_80038900(MAIN_MAP_INDEX, 6, 0, 6, 1);
                    break;
                case WINTER:                                     
                    func_80038900(MAIN_MAP_INDEX, 0, 0, 0, 1);
                    func_80038900(MAIN_MAP_INDEX, 1, 0, 1, 1);
                    break;
                }
            break;
        case FLOWER_SHOP:
            func_80038900(MAIN_MAP_INDEX, 0, 0, 0, 1);
            break;
        case BAKERY:
            func_80038900(MAIN_MAP_INDEX, 0, 0, 0, 1);
            func_80038900(MAIN_MAP_INDEX, 1, 0, 1, 1);
            break;
        case MAYOR_HOUSE:
            func_80038900(MAIN_MAP_INDEX, 0, 0, 0, 1);
            break;
        case POTION_SHOP:
            func_80038900(MAIN_MAP_INDEX, 0, 0, 0, 1);
            break;
        case MOUNTAIN_1:
            func_80038900(MAIN_MAP_INDEX, 0, 0, 0, 1);
            break;
        case MOUNTAIN_2:
            func_80038900(MAIN_MAP_INDEX, 0, 0, 0, 1);
            func_80038900(MAIN_MAP_INDEX, 1, 0, 1, 1);
            func_80038900(MAIN_MAP_INDEX, 2, 0, 2, 1);
            func_80038900(MAIN_MAP_INDEX, 3, 0, 3, 1);
            break;
        case TOP_OF_MOUNTAIN_1:
            func_80038900(MAIN_MAP_INDEX, 0, 0, 0, 1);
            func_80038900(MAIN_MAP_INDEX, 1, 0, 1, 1);
            func_80038900(MAIN_MAP_INDEX, 2, 0, 2, 1);
            func_80038900(MAIN_MAP_INDEX, 3, 0, 3, 1);
            func_80038900(MAIN_MAP_INDEX, 4, 0, 4, 1);
            func_80038900(MAIN_MAP_INDEX, 5, 0, 5, 1);
            if (gSeason == WINTER) {
                func_80038900(MAIN_MAP_INDEX, 6, 0, 6, 1);
            }
            break;
        case MOON_MOUNTAIN:
            func_80038900(MAIN_MAP_INDEX, 0, 0, 0, 1);
            break;
        case POND:
            func_80038900(MAIN_MAP_INDEX, 4, 0, 0, 0);
            func_80038900(MAIN_MAP_INDEX, 5, 0, 1, 0);
            func_80038900(MAIN_MAP_INDEX, 6, 0, 2, 0);
            func_80038900(MAIN_MAP_INDEX, 7, 0, 3, 0);
            break;
        case VINEYARD:
            func_80038900(MAIN_MAP_INDEX, 0, 0, 1, 1);
            func_80038900(MAIN_MAP_INDEX, 1, 0, 0, 1);
            break;
        case RANCH:
            func_80038900(MAIN_MAP_INDEX, 0, 0, 0, 1);
            func_80038900(MAIN_MAP_INDEX, 1, 0, 1, 1);
            func_80038900(MAIN_MAP_INDEX, 2, 0, 2, 1);
            if (gSeason == AUTUMN) {
                func_80038900(MAIN_MAP_INDEX, 3, 0, 3, 1);
            }
            break;
        case RANCH_STORE:
            func_80038900(MAIN_MAP_INDEX, 0, 0, 0, 1);
            break;
        case RICK_STORE:
            func_80038900(MAIN_MAP_INDEX, 0, 0, 0, 1);
            break;
        case CAVE:
            func_80038900(MAIN_MAP_INDEX, 0, 0, 0, 1);
            break;
        case BEACH:
            func_80038900(MAIN_MAP_INDEX, 0, 0, 0, 4);
            func_80038900(MAIN_MAP_INDEX, 0, 1, 1, 4);
            func_80038900(MAIN_MAP_INDEX, 0, 2, 2, 4);
            func_80038900(MAIN_MAP_INDEX, 0, 3, 3, 4);
            func_80038900(MAIN_MAP_INDEX, 0, 4, 4, 4);
            func_80038900(MAIN_MAP_INDEX, 0, 5, 5, 4);
            func_80038900(MAIN_MAP_INDEX, 0, 6, 6, 4);
            func_80038900(MAIN_MAP_INDEX, 0, 7, 5, 4);
            func_80038900(MAIN_MAP_INDEX, 0, 8, 4, 4);
            func_80038900(MAIN_MAP_INDEX, 0, 9, 3, 4);
            func_80038900(MAIN_MAP_INDEX, 0, 0xA, 2, 4);
            func_80038900(MAIN_MAP_INDEX, 0, 0xB, 1, 4);
            break;
        case VINEYARD_CELLAR:
            func_80038900(MAIN_MAP_INDEX, 0, 0, 0, 1);
            break;
        case MINE:
        case MINE_2:
            func_80038900(MAIN_MAP_INDEX, 4, 0, 0, 0);
            func_80038900(MAIN_MAP_INDEX, 0, 0, 1, 0);
            break;
        
    }   
}
*/

//INCLUDE_ASM(const s32, "level", func_800746B4);

// load map additions based on level index
void func_800746B4(u16 mapIndex) {
    
    switch (mapIndex) {
        
        case FARM:
            if (checkLifeEventBit(HAVE_KITCHEN)) {
                if (checkLifeEventBit(HAVE_BATHROOM)) {
                    func_80038AE0(MAIN_MAP_INDEX, 0x10);
                } else {
                    func_80038AE0(MAIN_MAP_INDEX, 0xE);
                }
            } else if (checkLifeEventBit(HAVE_BATHROOM)) {
                func_80038AE0(MAIN_MAP_INDEX, 0xF);
            }
            if (checkLifeEventBit(HAVE_GREENHOUSE)) {
                func_80038AE0(MAIN_MAP_INDEX, 0x13);
            }
            if (checkLifeEventBit(HAVE_LOG_TERRACE)) {
                func_80038AE0(MAIN_MAP_INDEX, 0x11);
            }
            if (checkLifeEventBit(HAVE_STAIRS)) {
                func_80038AE0(MAIN_MAP_INDEX, 0x1A);
            }
            if (checkLifeEventBit(0x57)) {
                func_80038AE0(MAIN_MAP_INDEX, 0x1C);
            }
            if (checkLifeEventBit(0x56)) {
                func_80038AE0(MAIN_MAP_INDEX, 0x1D);
            }
            if (checkLifeEventBit(0xD7)) {
                func_80038AE0(MAIN_MAP_INDEX, 0x1E);
            }
            break;
        case HOUSE:
            if ((checkLifeEventBit(HAVE_KITCHEN)) || (checkLifeEventBit(HAVE_BATHROOM))) {
                func_80038AE0(MAIN_MAP_INDEX, 0);
            }
            if (checkLifeEventBit(HAVE_STAIRS)) {
                func_80038AE0(MAIN_MAP_INDEX, 7);
            }
            if (checkLifeEventBit(HAVE_BABY_BED)) {
                func_80038AE0(MAIN_MAP_INDEX, 8);
            }
            if (checkLifeEventBit(HAVE_CABINET)) {
                 func_80038AE0(MAIN_MAP_INDEX, 1);
            } else if (checkLifeEventBit(0x58)) {
                func_80038AE0(MAIN_MAP_INDEX, 0xC);
            }
            if (checkLifeEventBit(0x3F)) {
                func_80038AE0(MAIN_MAP_INDEX, 9);
            }
            if (checkLifeEventBit(HAVE_TABLECLOTH)) {
                if (checkLifeEventBit(HAVE_RUG)) {
                    func_80038AE0(MAIN_MAP_INDEX, 6);
                } else {
                    func_80038AE0(MAIN_MAP_INDEX, 4);
                }
            } else if (checkLifeEventBit(HAVE_RUG)) {
                func_80038AE0(MAIN_MAP_INDEX, 5);
            }
            if (checkLifeEventBit(0x47)) {
                func_80038AE0(MAIN_MAP_INDEX, 0xB);
            }
            break;
        case KITCHEN:
            if (checkLifeEventBit(HAVE_BATHROOM)) {
                func_80038AE0(MAIN_MAP_INDEX, 1);
            }
            if (checkLifeEventBit(0x5D)) {
                func_80038AE0(MAIN_MAP_INDEX, 3);
            }
            break;
        case COOP:
            if (func_8009B564() == 6 && !func_8009B7BC()) {
                func_80038AE0(MAIN_MAP_INDEX, 1);
            }
            break;
        case MOUNTAIN_1:
            if (checkLifeEventBit(0x45) && gSeason == WINTER) {
                func_80038AE0(MAIN_MAP_INDEX, 0);
            }
            break;
        case MOUNTAIN_2:
            if (checkLifeEventBit(BRIDGE_COMPLETED)) {
                func_80038AE0(MAIN_MAP_INDEX, 0);
                func_80038AE0(MAIN_MAP_INDEX, 1);
                func_80038AE0(MAIN_MAP_INDEX, 2);
            }
            break;
        case TOP_OF_MOUNTAIN_1:
            if (checkLifeEventBit(BRIDGE_COMPLETED)) {
                func_80038AE0(MAIN_MAP_INDEX, 2);
                func_80038AE0(MAIN_MAP_INDEX, 3);
                func_80038AE0(MAIN_MAP_INDEX, 4);
                func_80038AE0(MAIN_MAP_INDEX, 5);
            }
            if (checkLifeEventBit(HOT_SPRINGS_COMPLETED)) {
                func_80038AE0(MAIN_MAP_INDEX, 0);
                break;
            }
            if (gYear == 1 && gSeason == WINTER && (11 < gDayOfMonth && gDayOfMonth < 17)) {
                func_80038AE0(MAIN_MAP_INDEX, 6);
            }
            break;
        case BEACH:
            func_80038990(MAIN_MAP_INDEX, MAIN_MAP_INDEX, 1);
            break;
        case VINEYARD_CELLAR:
            // karen yellow heart event
            if (0x2C2 < gCutsceneIndex && gCutsceneIndex < 0x2C5) {
                func_80038AE0(MAIN_MAP_INDEX, 0);
            }
            break;
        case SQUARE:
            if (gSeason == SPRING) {
                if (18 < gDayOfMonth && gDayOfMonth < 23) {
                    func_80038AE0(MAIN_MAP_INDEX, 4);
                }
            }
            if (gSeason == SUMMER && gDayOfMonth == 1 && (17 < gHour && gHour < 24) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
                func_80038AE0(MAIN_MAP_INDEX, 1);
                func_80038AE0(MAIN_MAP_INDEX, 2);
            }
            if (gSeason == SPRING) {
                if (gDayOfMonth == 17 && (7 < gHour && gHour < 18) && gCutsceneFlags & 1) {
                    func_80038AE0(MAIN_MAP_INDEX, 3);
                    func_80038AE0(MAIN_MAP_INDEX, 5);
                }
            }
            if (gSeason == AUTUMN && gDayOfMonth == 28 && (7 < gHour && gHour < 18) && gCutsceneFlags & 1) {
                func_80038AE0(MAIN_MAP_INDEX, 1);
                func_80038AE0(MAIN_MAP_INDEX, 6);
            }
            if (gSeason == WINTER && gDayOfMonth == 19 && (7 < gHour && gHour < 18) && gCutsceneFlags & 1) {
                func_80038AE0(MAIN_MAP_INDEX, 1);
            }
            break;
        case BATHROOM:
        default:
            break;
    }
}

//INCLUDE_ASM(const s32, "level", func_80074C38);

// get map index from exit
u8 func_80074C38(u8 exitIndex) {
    return D_801147C0[exitIndex];
}

// jtbl_8011FEA0
u16 func_80074C50(u16 exitIndex) {

    u16 index = 0xFFFF;

    // FIXME: no idea
    u32 temp = D_801147C0[exitIndex];
    u8 temp2 = temp;

    switch (temp2) {

        case ANN_ROOM:

            if (!checkLifeEventBit(MARRIED) || gWife != ANN) {
                if (!checkLifeEventBit(ANN_CLIFF_MARRIED)) {
                    index = 0xE2;
                    break;
                }
            }
            
            index = 0xD8;
            break;

        case RANCH_BARN:
            
            if (gDayOfWeek != THURSDAY) {
                index = (7 < gHour && gHour < 17) == 0 ? 0xBC : 0xFFFF;
            } else {
                index = 0xBC;
            }

            break;

        case RANCH_STORE:

            if (gDayOfWeek != THURSDAY) {
                index = (7 < gHour && gHour < 17) == 0 ? 0xC7 : 0xFFFF;
            } else {
                index = 0xC7;
            }
            
            break;

        case RANCH_HOUSE:
            index = (7 < gHour && gHour < 17) == 0 ? 0xBC : 0xFFFF;
            break;

        case RACE_TRACK:

            index = 0xBB;
            
            if (9 < gHour && gHour < 18) {

                switch (gSeason) {
                    case SPRING:
                        if (gDayOfMonth == 17) {
                            index = 0xFFFF;
                        }
                        break;
                    case AUTUMN:
                        if (gDayOfMonth == 28) {
                            index = 0xFFFF;
                        }
                        break;
                    case WINTER:
                        if (gDayOfMonth == 19) {
                            index = 0xFFFF;
                        }
                        break;
                }
            }

            break;

        case CARPENTER_HUT:

            index = 0xB0;
            
            if (gDayOfWeek != TUESDAY) {
                
                if (!checkLifeEventBit(HOUSE_EXTENSION_CONSTRUCTION) && !checkDailyEventBit(9)) {

                    if (gYear != 1 || ((gSeason != AUTUMN || !(20 <= gDayOfMonth && gDayOfMonth < 28)) && (gSeason != WINTER || !(8 < gDayOfMonth && gDayOfMonth < 17)))) {
                        if (7 < gHour && gHour < 18) {
                            index = 0xFFFF;
                        }
                    }
                }
            }
            
            break;

        case DUMPLING_HOUSE:
            index = (7 < gHour && gHour < 17) == 0 ? 0xBD : 0xFFFF;
            break;

        case ELLI_ROOM:

            if (!checkLifeEventBit(MARRIED) || gWife != ELLI) {
                if (!checkLifeEventBit(0x1C)) {
                    index = 0xE1;
                    break;
                }
            }
            
            index = 0xD8;
            break;

        case BAKERY:

            if (gDayOfWeek != MONDAY) {
                index = (8 < gHour && gHour < 17) == 0 ? 0xC2 : 0xFFFF;
            } else {
                index = 0xC2;
            }
            
            break;

        case POPURI_ROOM:
            
            if (!checkLifeEventBit(MARRIED) || gWife != POPURI) {
                if (!checkLifeEventBit(POPURI_GRAY_MARRIED)) {
                    index = 0xE0;
                    break;
                }
            }
            
            index = 0xD8;
            break;

        case FLOWER_SHOP:

            if (gDayOfWeek != SUNDAY) {
                index = (8 < gHour && gHour < 17) == 0 ? 0xC1 : 0xFFFF;
            } else {
                index = 0xC1;
            }

            break;

        case CHURCH:
            index = (7 < gHour && gHour < 17) == 0 ? 0xBC : 0xFFFF;
            break;
        
        case SOUVENIR_SHOP:
            
            if (gDayOfWeek != 1) {
                index = (8 < gHour && gHour < 17) == 0 ? 0xC4 : 0xFFFF;
            } else {
                index = 0xC4;
            }
            
            break;
        
        case RICK_STORE:
            
            if (gWeather == SUNNY && gDayOfWeek != WEDNESDAY && gDayOfWeek != SATURDAY && gDayOfWeek != SUNDAY) {
                index = (9 < gHour && gHour < 18) == 0 ? 0xC3 : 0xFFFF;
            } else {
                index = 0xC3;
            }
        
            break;
        
        case MIDWIFE_HOUSE:
            index = (8 < gHour && gHour < 17) == 0 ? 0xBD : 0xFFFF;
            break;
        
        case TAVERN:
            
            if (gDayOfWeek != SUNDAY) {
                index = (17 < gHour && gHour < 24) == 0 ? 0xC5 : 0xFFFF;
            } else {
                index = 0xC5;
            }
            
            break;
        
        case LIBRARY:

            index = 0xC6;
            
            if (gDayOfWeek != MONDAY) {
                
                switch (gSeason) {
                    case SUMMER:
                        if ((1 <= gDayOfMonth && gDayOfMonth < 11) == 0) {
                            if (8 < gHour && gHour < 17) {
                                index = 0xFFFF;
                            }
                        }
                        break;
                    case WINTER:
                        if ((1 <= gDayOfMonth && gDayOfMonth < 11) == 0) {
                            if (gDayOfMonth != 30) {
                    default:
                                if (8 < gHour && gHour < 17) {
                                    index = 0xFFFF;
                                }
                            }
                        }
                        break;
                    }
            } 

            break;

        case MARIA_ROOM:
            
            if (!checkLifeEventBit(MARRIED) || gWife != 0) {
              
                if (!checkLifeEventBit(0x1A)) {
                    index = 0xDF;
                    break;
                }
            }
            
            index = 0xD8;
            
            break;

        case MAYOR_HOUSE:

            if (gDayOfWeek != SUNDAY && gDayOfWeek != FRIDAY) {
                index = (7 < gHour && gHour < 17) == 0 ? 0xBC : 0xFFFF;
            } else {
                index = 0xBC;
            }
            
            break;

        case POTION_SHOP:
            
            if (gDayOfWeek != SATURDAY && gDayOfWeek != SUNDAY) {
                index = (8 < gHour && gHour < 17) == 0 ? 0xE4 : 0xFFFF;
            } else {
                index = 0xE4;
            }
            
            break;

        case HARVEST_SPRITE_CAVE:
            index = (6 < gHour && gHour < 19) == 0 ? 0xBD : 0xFFFF;
            break;

        case MINE:

            if (gSeason == WINTER && (7 < gDayOfMonth && gDayOfMonth < 31)) {
                index = (8 < gHour && gHour < 17) == 0 ? 0xB2 : 0xFFFF;
            } else {
                index = 0xB2;
            }

            break;

        case KAREN_ROOM:

            if (!checkLifeEventBit(MARRIED) || gWife != KAREN) {
                if (!checkLifeEventBit(KAREN_KAI_MARRIED)) {                
                    index = 0xE3;
                    break;
                }
            } 
            
            index = 0xD8;
            
            break;

        case VINEYARD_HOUSE:

            index = 0xBC;   
            
            if (gSeason != AUTUMN || !(7 < gDayOfMonth && gDayOfMonth < 11)) {
                if (7 < gHour && gHour < 17) {   
                    index = 0xFFFF;
                }   
            }

            break;

        case VINEYARD_CELLAR:
            index = (7 < gHour && gHour < 18) == 0 ? 0xBC : 0xFFFF;
            break;
        
        case RANCH:
        case BEACH:
        case MOUNTAIN_1:
        case MOUNTAIN_2:
        case TOP_OF_MOUNTAIN_1:
        case MOON_MOUNTAIN:
        case POND:
        case VILLAGE_1:
        case VILLAGE_2:
        case SQUARE:
        case POTION_SHOP_BEDROOM:
        // unused/cut map
        case 0x41:
        case CAVE:
        case MINE_2:
        case VINEYARD:
        case VINEYARD_CELLAR_BASEMENT:
        case ROAD:
        case FARM:
        case GREENHOUSE:
        case HOUSE:
        case BARN:
        case COOP:
        case KITCHEN:
        case BATHROOM:
            index = 0xFFFF;
            break;
        
        default:
            break;
    }

    return index;
}

//INCLUDE_ASM(const s32, "level", func_80074C50);