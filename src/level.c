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
#include "shop.h"            
#include "spriteIndices.h"                                                 
#include "weather.h"      

// offsets into combined textures
extern u8 D_801C3F35;
extern u8 D_801FD624;
                             
// likely bss
extern u8 previousEntranceIndex;
extern u8 gEntranceIndex;
extern u8 gBaseMapIndex;
extern u8 gMapWithSeasonIndex;
// global rotation
extern u8 D_8021E6D0;
 
extern u32 _homeItemsTextureSegmentRomStart;
extern u32 _homeItemsTextureSegmentRomEnd;
extern u32 _homeItemsAssetsIndexSegmentRomStart;
extern u32 _homeItemsAssetsIndexSegmentRomEnd;
extern u32 _homeItemsSpritesheetIndexSegmentRomStart;
extern u32 _homeItemsSpritesheetIndexSegmentRomEnd;

extern u32 _farmPondTextureSegmentRomStart;
extern u32 _farmPondTextureSegmentRomEnd;
extern u32 _farmPondAssetsIndexSegmentRomStart;
extern u32 _farmPondAssetsIndexSegmentRomEnd;
extern u32 _farmPondSpritesheetIndexSegmentRomStart;
extern u32 _farmPondSpritesheetIndexSegmentRomEnd;

extern u32 _waterTilesTextureSegmentRomStart;
extern u32 _waterTilesTextureSegmentRomEnd;
extern u32 _waterTilesAssetsIndexSegmentRomStart;
extern u32 _waterTilesAssetsIndexSegmentRomEnd;
extern u32 _waterTilesSpritesheetIndexSegmentRomStart;
extern u32 _waterTilesSpritesheetIndexSegmentRomEnd;

extern u32 _bridgeTextureSegmentRomStart;
extern u32 _bridgeTextureSegmentRomEnd;
extern u32 _bridgeAssetsIndexSegmentRomStart;
extern u32 _bridgeAssetsIndexSegmentRomEnd;
extern u32 _bridgeSpritesheetIndexSegmentRomStart;
extern u32 _bridgeSpritesheetIndexSegmentRomEnd;

extern u32 _boulderTextureSegmentRomStart;
extern u32 _boulderTextureSegmentRomEnd;
extern u32 _boulderAssetsIndexSegmentRomStart;
extern u32 _boulderAssetsIndexSegmentRomEnd;
extern u32 _boulderSpritesheetIndexSegmentRomStart;
extern u32 _boulderSpritesheetIndexSegmentRomEnd;

extern u32 _stonePillarTextureSegmentRomStart;
extern u32 _stonePillarTextureSegmentRomEnd;
extern u32 _stonePillarAssetsIndexSegmentRomStart;
extern u32 _stonePillarAssetsIndexSegmentRomEnd;
extern u32 _stonePillarSpritesheetIndexSegmentRomStart;
extern u32 _stonePillarSpritesheetIndexSegmentRomEnd;

extern u32 _steamTextureSegmentRomStart;
extern u32 _steamTextureSegmentRomEnd;
extern u32 _steamAssetsIndexSegmentRomStart;
extern u32 _steamAssetsIndexSegmentRomEnd;
extern u32 _steamSpritesheetIndexSegmentRomStart;
extern u32 _steamSpritesheetIndexSegmentRomEnd;

extern u32 _mountainPathTilesTextureSegmentRomStart;
extern u32 _mountainPathTilesTextureSegmentRomEnd;
extern u32 _mountainPathTilesAssetsIndexSegmentRomStart;
extern u32 _mountainPathTilesAssetsIndexSegmentRomEnd;
extern u32 _mountainPathTilesSpritesheetIndexSegmentRomStart;
extern u32 _mountainPathTilesSpritesheetIndexSegmentRomEnd;

extern u32 _squareFountainTextureSegmentRomStart;
extern u32 _squareFountainTextureSegmentRomEnd;
extern u32 _squareFountainAssetsIndexSegmentRomStart;
extern u32 _squareFountainAssetsIndexSegmentRomEnd;
extern u32 _squareFountainSpritesheetIndexSegmentRomStart;
extern u32 _squareFountainSpritesheetIndexSegmentRomEnd;

extern u32 _festivalFlowersTextureSegmentRomStart;
extern u32 _festivalFlowersTextureSegmentRomEnd;
extern u32 _festivalFlowersAssetsIndexSegmentRomStart;
extern u32 _festivalFlowersAssetsIndexSegmentRomEnd;
extern u32 _festivalFlowersSpritesheetIndexSegmentRomStart;
extern u32 _festivalFlowersSpritesheetIndexSegmentRomEnd;

extern u32 _village2WaterTextureSegmentRomStart;
extern u32 _village2WaterTextureSegmentRomEnd;
extern u32 _village2WaterAssetsIndexSegmentRomStart;
extern u32 _village2WaterAssetsIndexSegmentRomEnd;
extern u32 _village2WaterSpritesheetIndexSegmentRomStart;
extern u32 _village2WaterSpritesheetIndexSegmentRomEnd;

extern u32 _wavesTextureSegmentRomStart;
extern u32 _wavesTextureSegmentRomEnd;
extern u32 _wavesAssetsIndexSegmentRomStart;
extern u32 _wavesAssetsIndexSegmentRomEnd;
extern u32 _wavesSpritesheetIndexSegmentRomStart;
extern u32 _wavesSpritesheetIndexSegmentRomEnd;

extern u32 _pondWaterTextureSegmentRomStart;
extern u32 _pondWaterTextureSegmentRomEnd;
extern u32 _pondWaterAssetsIndexSegmentRomStart;
extern u32 _pondWaterAssetsIndexSegmentRomEnd;
extern u32 _pondWaterSpritesheetIndexSegmentRomStart;
extern u32 _pondWaterSpritesheetIndexSegmentRomEnd;

extern u32 _vineyardTreeTextureSegmentRomStart;
extern u32 _vineyardTreeTextureSegmentRomEnd;
extern u32 _vineyardTreeAssetsIndexSegmentRomStart;
extern u32 _vineyardTreeAssetsIndexSegmentRomEnd;
extern u32 _vineyardTreeSpritesheetIndexSegmentRomStart;
extern u32 _vineyardTreeSpritesheetIndexSegmentRomEnd;

extern u32 _stoneTextureSegmentRomStart;
extern u32 _stoneTextureSegmentRomEnd;
extern u32 _stoneAssetsIndexSegmentRomStart;
extern u32 _stoneAssetsIndexSegmentRomEnd;
extern u32 _stoneSpritesheetIndexSegmentRomStart;
extern u32 _stoneSpritesheetIndexSegmentRomEnd;

extern u32 _groundObjectsTextureSegmentRomStart;
extern u32 _groundObjectsTextureSegmentRomEnd;
extern u32 _groundObjectsIndexSegmentRomStart;
extern u32 _groundObjectsIndexSegmentRomEnd;

// data
extern u8 levelFlags[];

// rotations for map model context struct
// all 7
extern u8 D_80114280[95];

// global lighting defaults based on map index
extern u8 D_801142E0[95][4];

// song indices for level
// I feel this should be [0x60][2][4]
// [mapIndex][day or night][season-1]
extern u8 levelToMusicMappings[TOTAL_MAPS][8];

// exit to map indices
// FIXME: probably shouldn't be volatile, but need it for matching func_80074C50
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
        func_8003C504(MAIN_MAP_INDEX);
    }

    previousMapIndex = gBaseMapIndex;
    gBaseMapIndex = D_801147C0[entranceIndex];
    gMapWithSeasonIndex = gBaseMapIndex;

    if (getLevelFlags(gMapWithSeasonIndex) & HAS_SEASON_MAP) {
        gMapWithSeasonIndex = gMapWithSeasonIndex + (gSeason - 1);
    }

    // load map
    loadMap(MAIN_MAP_INDEX, gMapWithSeasonIndex);

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

    func_8003BD60(MAIN_MAP_INDEX);
    
    // set ground object sprites on map struct
    func_8006EC58(mapIndex);
    func_800735FC(mapIndex);

    // load and setup map object sprites
    func_8006F938(mapIndex); 

    setAdditionalMapAdditionsForLevel(mapIndex);

    func_800D9600(gBaseMapIndex);
    
    if (getLevelFlags(mapIndex) & OUTDOORS) {
        // load rain/snow sprites
        func_800DC360();
    }
    
    func_80036C08(MAIN_MAP_INDEX);

    func_80036FA0(MAIN_MAP_INDEX);

}
 
//INCLUDE_ASM(const s32, "level", setLevelAudio);

void setLevelAudio(u16 mapIndex, u8 season, u8 hour) {

    u32 nightOffset;
    
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

        if (hour >= 6 || gBaseMapIndex == BEACH) {
            
            nightOffset = 0;
            
            if (17 < hour &&  hour < 24) {

                nightOffset = 4;

                if (!(getLevelFlags(mapIndex) & 0x10)) {
                    gSongVolume = 128;
                } else {
                    gSongVolume = 64;
                }

             } else {
                gSongVolume = 128;
             }
            
            // probably should be:
            // if night --> nightOffset = 1
            // gCurrentSongIndex = levelToMusicMappings[mapIndex][nightOffset][season-1];
            gCurrentSongIndex = levelToMusicMappings[mapIndex][nightOffset+(season-1)];
            
        }   
        
    }

    if (gCurrentSongIndex != 0xFF) {
        setCurrentSong(gCurrentSongIndex);
    }
}

//INCLUDE_ASM(const s32, "level", func_8006EB94);

// param1 = globalLighting
Vec4f* func_8006EB94(Vec4f* rgba, u16 mapIndex) {

    Vec4f vec;

    vec.r = D_801142E0[mapIndex][0];
    vec.g = D_801142E0[mapIndex][1];
    vec.b = D_801142E0[mapIndex][2];
    vec.a = D_801142E0[mapIndex][3];

    *rgba = vec;
    
    return rgba;
    
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
            func_80034EF0(MAIN_MAP_INDEX, 0xE, 0xB, (void*)0x802EB800, (void*)0x802F2800, (void*)0x802F3100, &_groundObjectsTextureSegmentRomStart, &_groundObjectsTextureSegmentRomEnd, &_groundObjectsIndexSegmentRomStart, &_groundObjectsIndexSegmentRomEnd, 0x60); 
            break;
        case GREENHOUSE:
            D_801FD624 = 1;
            D_801C3F35 = 1;
            func_80034EF0(MAIN_MAP_INDEX, 1, 1, (void*)0x802EB800, (void*)0x802F2800, (void*)0x802F3100, &_groundObjectsTextureSegmentRomStart, &_groundObjectsTextureSegmentRomEnd, &_groundObjectsIndexSegmentRomStart, &_groundObjectsIndexSegmentRomEnd, 2); 
            break;            
        case MOUNTAIN_1:
            D_801FD624 = 5;
            D_801C3F35 = 0;
            func_80034EF0(MAIN_MAP_INDEX, 5, 0, (void*)0x802EB800, (void*)0x802F2800, (void*)0x802F3100, &_groundObjectsTextureSegmentRomStart, &_groundObjectsTextureSegmentRomEnd, &_groundObjectsIndexSegmentRomStart, &_groundObjectsIndexSegmentRomEnd, 0); 
            break;    
        case MOUNTAIN_2:
            D_801FD624 = 3;
            D_801C3F35 = 5;
            func_80034EF0(MAIN_MAP_INDEX, 3, 5, (void*)0x802EB800, (void*)0x802F2800, (void*)0x802F3100, &_groundObjectsTextureSegmentRomStart, &_groundObjectsTextureSegmentRomEnd, &_groundObjectsIndexSegmentRomStart, &_groundObjectsIndexSegmentRomEnd, 0); 
            break;  
        case TOP_OF_MOUNTAIN_1:
            D_801FD624 = 5;
            D_801C3F35 = 0;
            func_80034EF0(MAIN_MAP_INDEX, 5, 0, (void*)0x802EB800, (void*)0x802F2800, (void*)0x802F3100, &_groundObjectsTextureSegmentRomStart, &_groundObjectsTextureSegmentRomEnd, &_groundObjectsIndexSegmentRomStart, &_groundObjectsIndexSegmentRomEnd, 0); 
            break;  
        case MOON_MOUNTAIN:
            D_801FD624 = 5;
            D_801C3F35 = 0;
            func_80034EF0(MAIN_MAP_INDEX, 5, 0, (void*)0x802EB800, (void*)0x802F2800, (void*)0x802F3100, &_groundObjectsTextureSegmentRomStart, &_groundObjectsTextureSegmentRomEnd, &_groundObjectsIndexSegmentRomStart, &_groundObjectsIndexSegmentRomEnd, 0); 
            break;                            
        case POND:
            D_801FD624 = 5;
            D_801C3F35 = 0;
            func_80034EF0(MAIN_MAP_INDEX, 5, 0, (void*)0x802EB800, (void*)0x802F2800, (void*)0x802F3100, &_groundObjectsTextureSegmentRomStart, &_groundObjectsTextureSegmentRomEnd, &_groundObjectsIndexSegmentRomStart, &_groundObjectsIndexSegmentRomEnd, 0); 
            break;          
        case CAVE:
            D_801FD624 = 0;
            D_801C3F35 = 0;
            func_80034EF0(MAIN_MAP_INDEX, 0, 0, (void*)0x802EB800, (void*)0x802F2800, (void*)0x802F3100, &_groundObjectsTextureSegmentRomStart, &_groundObjectsTextureSegmentRomEnd, &_groundObjectsIndexSegmentRomStart, &_groundObjectsIndexSegmentRomEnd, 0); 
            break;    
        case MINE:                                      
        case MINE_2:        
            D_801FD624 = 2;
            D_801C3F35 = 2;
            func_80034EF0(MAIN_MAP_INDEX, 2, 2, (void*)0x802EB800, (void*)0x802F2800, (void*)0x802F3100, &_groundObjectsTextureSegmentRomStart, &_groundObjectsTextureSegmentRomEnd, &_groundObjectsIndexSegmentRomStart, &_groundObjectsIndexSegmentRomEnd, 0); 
            break;    
        case RANCH:
            D_801FD624 = 0x10;
            D_801C3F35 = 0xA;
            func_80034EF0(MAIN_MAP_INDEX, 0x10, 0xA, (void*)0x802EB800, (void*)0x802F2800, (void*)0x802F3100, &_groundObjectsTextureSegmentRomStart, &_groundObjectsTextureSegmentRomEnd, &_groundObjectsIndexSegmentRomStart, &_groundObjectsIndexSegmentRomEnd, 0); 
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

//INCLUDE_ASM(const s32, "level", func_8006F938);

// do additional loading for assets per level
void func_8006F938(u16 levelIndex) {

    u8 i;
    u8 j;
    u16 k;

    switch (levelIndex) {

        case FARM:
            
            func_8002B138(MAP_OBJECT_1, &_homeItemsTextureSegmentRomStart, &_homeItemsTextureSegmentRomEnd, &_homeItemsAssetsIndexSegmentRomStart, &_homeItemsAssetsIndexSegmentRomEnd, &_homeItemsSpritesheetIndexSegmentRomStart, &_homeItemsSpritesheetIndexSegmentRomEnd, (void*)0x802E4000, (void*)0x802E4D00, (void*)0x802E7400, (void*)0x802E7700, (void*)0x802E7A00, (void*)0x802E7B00, 1, 1);
            setSpriteScale(MAP_OBJECT_1, 1.0f, 1.0f, 1.0f);
            func_8002C7EC(MAP_OBJECT_1, 3);
            setSpriteDefaultRGBA(MAP_OBJECT_1, 0xFF, 0xFF, 0xFF, 0xFF);
            func_8002C914(MAP_OBJECT_1, 0xFF, 0xFF, 0xFF, 0xFF);
            
            if (checkDailyEventBit(0x43)) {
                setMapObject(MAIN_MAP_INDEX, 0, MAP_OBJECT_1, 0x13, -464.0f, 96.0f, 112.0f, 0xFF, 0xFF, 0, 0);
            } else {
                setMapObject(MAIN_MAP_INDEX, 0, MAP_OBJECT_1, 0xF, -464.0f, 96.0f, 112.0f, 0xFF, 0xFF, 0, 0);
            }
            
            func_8002B138(MAP_OBJECT_2, &_farmPondTextureSegmentRomStart, &_farmPondTextureSegmentRomEnd, &_farmPondAssetsIndexSegmentRomStart, &_farmPondAssetsIndexSegmentRomEnd, &_farmPondSpritesheetIndexSegmentRomStart, &_farmPondSpritesheetIndexSegmentRomEnd, (void*)0x802E7C00, (void*)0x802E8900, (void*)0x802E9600, (void*)0x802E9700, (void*)0x802E9800, (void*)0x802E9900, 1,1);
            setSpriteScale(MAP_OBJECT_2, 6.0f, 1.0f, 6.0f);
            func_8002C6F8(MAP_OBJECT_2, 3);
            func_8002C768(MAP_OBJECT_2, 0x200);
            func_8002C7EC(MAP_OBJECT_2, 2);
            setSpriteDefaultRGBA(MAP_OBJECT_2, 0xFF, 0xFF, 0xFF, 0x80);
            func_8002C914(MAP_OBJECT_2, 0xFF, 0xFF, 0xFF, 0xFF);
            
            setMapObject(MAIN_MAP_INDEX, 1, MAP_OBJECT_2, 0, 304.f, 64.0f, -384.0f, 0, 0xFE, 0, 0);

            break;

        case HOUSE:
            
            func_8002B138(MAP_OBJECT_1, &_homeItemsTextureSegmentRomStart, &_homeItemsTextureSegmentRomEnd, &_homeItemsAssetsIndexSegmentRomStart, &_homeItemsAssetsIndexSegmentRomEnd, &_homeItemsSpritesheetIndexSegmentRomStart, &_homeItemsSpritesheetIndexSegmentRomEnd, (void*)0x802E4000, (void*)0x802E4D00, (void*)0x802E7400, (void*)0x802E7700, (void*)0x802E7A00, (void*)0x802E7B00, 1, 1);
            setSpriteScale(MAP_OBJECT_1, 2.0f, 2.0f, 1.0f);
            func_8002C7EC(MAP_OBJECT_1, 3);
            setSpriteDefaultRGBA(MAP_OBJECT_1, 0xFF, 0xFF, 0xFF, 0xFF);
            func_8002C914(MAP_OBJECT_1, 0xFF, 0xFF, 0xFF, 0xFF);
            setMapObject(MAIN_MAP_INDEX, 0, MAP_OBJECT_1, 6, -160.0f, 32.0f, -112.0f, 0xFF, 0xFF, 0, 0);
            
            if (checkLifeEventBit(0x46)) {
                
                func_8002B138(MAP_OBJECT_2, &_homeItemsTextureSegmentRomStart, &_homeItemsTextureSegmentRomEnd, &_homeItemsAssetsIndexSegmentRomStart, &_homeItemsAssetsIndexSegmentRomEnd, &_homeItemsSpritesheetIndexSegmentRomStart, &_homeItemsSpritesheetIndexSegmentRomEnd, (void*)0x802E7C00, (void*)0x802E7E80, (void*)0x802E7400, (void*)0x802E7700, (void*)0x802E7A00, (void*)0x802E7B00, 1, 1);
                setSpriteScale(MAP_OBJECT_2, 1.0f, 1.0f, 1.0f);
                func_8002C7EC(MAP_OBJECT_2, 3);
                setSpriteDefaultRGBA(MAP_OBJECT_2, 0xFF, 0xFF, 0xFF, 0xFF);
                func_8002C914(MAP_OBJECT_2, 0xFF, 0xFF, 0xFF, 0xFF);
                setMapObject(MAIN_MAP_INDEX, 1, MAP_OBJECT_2, 0x10, 8.0f, 72.0f, -168.0f, 0xFF, 0xFF, 0, 0);
            }
            
            if (checkLifeEventBit(0x47)) {
                
                func_8002B138(MAP_OBJECT_3, &_homeItemsTextureSegmentRomStart, &_homeItemsTextureSegmentRomEnd, &_homeItemsAssetsIndexSegmentRomStart, &_homeItemsAssetsIndexSegmentRomEnd, &_homeItemsSpritesheetIndexSegmentRomStart, &_homeItemsSpritesheetIndexSegmentRomEnd, (void*)0x802E8100, (void*)0x802E8380, (void*)0x802E7400, (void*)0x802E7700, (void*)0x802E7A00, (void*)0x802E7B00, 1, 1);
                setSpriteScale(MAP_OBJECT_3, 1.0f, 1.0f, 1.0f);
                func_8002C7EC(MAP_OBJECT_3, 3);
                setSpriteDefaultRGBA(MAP_OBJECT_3, 0xFF, 0xFF, 0xFF, 0xFF);
                func_8002C914(MAP_OBJECT_3, 0xFF, 0xFF, 0xFF, 0xFF);
                setMapObject(MAIN_MAP_INDEX, 2, MAP_OBJECT_3, 9, 104.0f, 0, 24.0f, 0xFF, 0xFF, 0, 0);
                
            }
            
            // clock
            if (checkLifeEventBit(0x58)) {
                
                func_8002B138(MAP_OBJECT_4, &_homeItemsTextureSegmentRomStart, &_homeItemsTextureSegmentRomEnd, &_homeItemsAssetsIndexSegmentRomStart, &_homeItemsAssetsIndexSegmentRomEnd, &_homeItemsSpritesheetIndexSegmentRomStart, &_homeItemsSpritesheetIndexSegmentRomEnd, (void*)0x802E8600, (void*)0x802E8880, (void*)0x802E7400, (void*)0x802E7700, (void*)0x802E7A00, (void*)0x802E7B00, 1, 1);
                setSpriteScale(MAP_OBJECT_4, 1.0f, 1.0f, 1.0f);
                func_8002C7EC(MAP_OBJECT_4, 3);
                setSpriteDefaultRGBA(MAP_OBJECT_4, 0xFF, 0xFF, 0xFF, 0xFF);
                func_8002C914(MAP_OBJECT_4, 0xFF, 0xFF, 0xFF, 0xFF);

                // cabinet
                if (checkLifeEventBit(HAVE_CABINET)) {
                    setMapObject(MAIN_MAP_INDEX, 3, MAP_OBJECT_4, 0x12, -192.0f, 64.0f, 104.0f, 0xFF, 0xFF, 0, 0);
                } else {
                    setMapObject(MAIN_MAP_INDEX, 3, MAP_OBJECT_4, 0x12, -176.0f, 0, 88.0f, 0xFF, 0xFF, 0, 0);
                }

            }
            
            if (checkLifeEventBit(0x5A)) {
                
                func_8002B138(MAP_OBJECT_5, &_homeItemsTextureSegmentRomStart, &_homeItemsTextureSegmentRomEnd, &_homeItemsAssetsIndexSegmentRomStart, &_homeItemsAssetsIndexSegmentRomEnd, &_homeItemsSpritesheetIndexSegmentRomStart, &_homeItemsSpritesheetIndexSegmentRomEnd, (void*)0x802E8B00, (void*)0x802E8D80, (void*)0x802E7400, (void*)0x802E7700, (void*)0x802E7A00, (void*)0x802E7B00, 1, 1);
                setSpriteScale(MAP_OBJECT_5, 1.0f, 1.0f, 1.0f);
                func_8002C7EC(MAP_OBJECT_5, 3);
                setSpriteDefaultRGBA(MAP_OBJECT_5, 0xFF, 0xFF, 0xFF, 0xFF);
                func_8002C914(MAP_OBJECT_5, 0xFF, 0xFF, 0xFF, 0xFF);
                setMapObject(MAIN_MAP_INDEX, 4, MAP_OBJECT_5, 0x11, -176.0f, 24.0f, -160.0f, 0xFF, 0xFF, 0, 0);
                
            }
            
            // vase
            if (checkLifeEventBit(8)) {
                
                func_8002B138(MAP_OBJECT_6, &_homeItemsTextureSegmentRomStart, &_homeItemsTextureSegmentRomEnd, &_homeItemsAssetsIndexSegmentRomStart, &_homeItemsAssetsIndexSegmentRomEnd, &_homeItemsSpritesheetIndexSegmentRomStart, &_homeItemsSpritesheetIndexSegmentRomEnd, (void*)0x802E9000, (void*)0x802E9280, (void*)0x802E7400, (void*)0x802E7700, (void*)0x802E7A00, (void*)0x802E7B00, 1, 1);
                setSpriteScale(MAP_OBJECT_6, 1.0f, 1.0f, 1.0f);
                func_8002C7EC(MAP_OBJECT_6, 3);
                setSpriteDefaultRGBA(MAP_OBJECT_6, 0xFF, 0xFF, 0xFF, 0xFF);
                func_8002C914(MAP_OBJECT_6, 0xFF, 0xFF, 0xFF, 0xFF);
                setMapObject(MAIN_MAP_INDEX, 5, MAP_OBJECT_6, 4, -40.0f, 8.0f, 8.0f, 0xFF, 0xFF, 0, 0);
            
            }
            
            break;

        case KITCHEN:
            
            // table cloth
            if (checkLifeEventBit(0x5C)) {
                
                func_8002B138(MAP_OBJECT_1, &_homeItemsTextureSegmentRomStart, &_homeItemsTextureSegmentRomEnd, &_homeItemsAssetsIndexSegmentRomStart, &_homeItemsAssetsIndexSegmentRomEnd, &_homeItemsSpritesheetIndexSegmentRomStart, &_homeItemsSpritesheetIndexSegmentRomEnd, (void*)0x802E7C00, (void*)0x802E7E80, (void*)0x802E7400, (void*)0x802E7700, (void*)0x802E7A00, (void*)0x802E7B00, 1, 1);
                setSpriteScale(MAP_OBJECT_1, 1.0f, 1.0f, 1.0f);
                func_8002C7EC(MAP_OBJECT_1, 3);
                setSpriteDefaultRGBA(MAP_OBJECT_1, 0xFF, 0xFF, 0xFF, 0xFF);
                func_8002C914(MAP_OBJECT_1, 0xFF, 0xFF, 0xFF, 0xFF);
                setMapObject(MAIN_MAP_INDEX, 0, MAP_OBJECT_1, 10, -48.f, 16.0f, -80.0f, 0xFF, 0xFF, 0, 0);
        		
            }
            
            break;
        
         case MOUNTAIN_1:

            func_8002B138(MAP_OBJECT_1, &_waterTilesTextureSegmentRomStart, &_waterTilesTextureSegmentRomEnd, &_waterTilesAssetsIndexSegmentRomStart, &_waterTilesAssetsIndexSegmentRomEnd, &_waterTilesSpritesheetIndexSegmentRomStart, &_waterTilesSpritesheetIndexSegmentRomEnd, (void*)0x802E4000, (void*)0x802E4D00, (void*)0x802E7400, (void*)0x802E7700, (void*)0x802E7A00, (void*)0x802E7B00, 1, 1);
            setSpriteScale(MAP_OBJECT_1, 3.8f, 1.0f, 6.0f);
            func_8002C6F8(MAP_OBJECT_1, 3);
            func_8002C768(MAP_OBJECT_1, 0x200);
            func_8002C7EC(MAP_OBJECT_1, 2);
            setSpriteDefaultRGBA(MAP_OBJECT_1, 0xFF, 0xFF, 0xFF, 0x80);
            func_8002C914(MAP_OBJECT_1, 0xFF, 0xFF, 0xFF, 0xFF);
            setMapObject(MAIN_MAP_INDEX, 0, MAP_OBJECT_1, 0, -288.0f, 0.0f, -140.0f, 0x5A, 0xFE, 0, 0);
             
            func_8002B138(MAP_OBJECT_2, &_waterTilesTextureSegmentRomStart, &_waterTilesTextureSegmentRomEnd, &_waterTilesAssetsIndexSegmentRomStart, &_waterTilesAssetsIndexSegmentRomEnd, &_waterTilesSpritesheetIndexSegmentRomStart, &_waterTilesSpritesheetIndexSegmentRomEnd, (void*)0x802E5A00, (void*)0x802E6700, (void*)0x802E7400, (void*)0x802E7700, (void*)0x802E7A00, (void*)0x802E7B00, 1, 1);
            setSpriteScale(MAP_OBJECT_2, 6.0f, 1.0f, 6.0f);
            func_8002C6F8(MAP_OBJECT_2, 3);
            func_8002C768(MAP_OBJECT_2, 0x200);
            func_8002C7EC(MAP_OBJECT_2, 2);
            setSpriteDefaultRGBA(MAP_OBJECT_2, 0xFF, 0xFF, 0xFF, 0x80);
            func_8002C914(MAP_OBJECT_2, 0xFF, 0xFF, 0xFF, 0xFF);
            setMapObject(MAIN_MAP_INDEX, 1, MAP_OBJECT_2, 0, -224.0f, 0.0f, 320.0f, 0, 0xFE, 0, 0);
             
            func_8002B138(MAP_OBJECT_3, &_waterTilesTextureSegmentRomStart, &_waterTilesTextureSegmentRomEnd, &_waterTilesAssetsIndexSegmentRomStart, &_waterTilesAssetsIndexSegmentRomEnd, &_waterTilesSpritesheetIndexSegmentRomStart, &_waterTilesSpritesheetIndexSegmentRomEnd, (void*)0x802E7C00, (void*)0x802E8900, (void*)0x802E9600, (void*)0x802E9700, (void*)0x802E9800, (void*)0x802E9900, 1, 1);
            setSpriteScale(MAP_OBJECT_3, 6.0f, 1.0f, 6.0f);
            func_8002C6F8(MAP_OBJECT_3, 3);
            func_8002C768(MAP_OBJECT_3, 0x200);
            func_8002C7EC(MAP_OBJECT_3, 2);
            setSpriteDefaultRGBA(MAP_OBJECT_3, 0xFF, 0xFF, 0xFF, 0x80);
            func_8002C914(MAP_OBJECT_3, 0xFF, 0xFF, 0xFF, 0xFF);
            setMapObject(MAIN_MAP_INDEX, 2, MAP_OBJECT_3, 1, 160.0f, 32.0f, -160.0f, 0, 0xFE, 0, 0);
             
            if (!checkLifeEventBit(0x45)) {
                
                func_8002B138(MAP_OBJECT_4, &_boulderTextureSegmentRomStart, &_boulderTextureSegmentRomEnd, &_boulderAssetsIndexSegmentRomStart, &_boulderAssetsIndexSegmentRomEnd, &_boulderSpritesheetIndexSegmentRomStart, &_boulderSpritesheetIndexSegmentRomEnd, (void*)0x802E9A00, (void*)0x802EA700, (void*)0x802EB400, (void*)0x802EB500, (void*)0x802EB600, (void*)0x802EB700, 1, 1);
                setSpriteScale(MAP_OBJECT_4, 1.0f, 1.0f, 1.0f);
                func_8002C7EC(MAP_OBJECT_4, 3);
                setSpriteDefaultRGBA(MAP_OBJECT_4, 0xFF, 0xFF, 0xFF, 0xFF);
                func_8002C914(MAP_OBJECT_4, 0xFF, 0xFF, 0xFF, 0xFF);

                if (gSeason == WINTER) {
                    setMapObject(MAIN_MAP_INDEX, 3, MAP_OBJECT_4, 0, 192.0f, 88.0f, -208.0f, 0xFF, 0, 0, 0);
                } else {
                    setMapObject(MAIN_MAP_INDEX, 3, MAP_OBJECT_4, 1, 192.0f, 88.0f, -208.0f, 0xFF, 0, 0, 0);
                }
            }
             
            break;

        case MOUNTAIN_2:

            func_8002B138(MAP_OBJECT_1, &_waterTilesTextureSegmentRomStart, &_waterTilesTextureSegmentRomEnd, &_waterTilesAssetsIndexSegmentRomStart, &_waterTilesAssetsIndexSegmentRomEnd, &_waterTilesSpritesheetIndexSegmentRomStart, &_waterTilesSpritesheetIndexSegmentRomEnd, (void*)0x802E4000, (void*)0x802E4D00, (void*)0x802E7400, (void*)0x802E7700, (void*)0x802E7A00, (void*)0x802E7B00, 1, 1);
            setSpriteScale(MAP_OBJECT_1, 6.0f, 1.0f, 6.0f);
            func_8002C6F8(MAP_OBJECT_1, 3);
            func_8002C768(MAP_OBJECT_1, 0x200);
            func_8002C7EC(MAP_OBJECT_1, 2);
            setSpriteDefaultRGBA(MAP_OBJECT_1, 0xFF, 0xFF, 0xFF, 0x80);
            func_8002C914(MAP_OBJECT_1, 0xFF, 0xFF, 0xFF, 0xFF);

            setMapObject(MAIN_MAP_INDEX, 0, MAP_OBJECT_1, 0, 224.0f, 0, -254.0f, 0x5A, 0xFE, 0, 0);
            
            func_8002B138(MAP_OBJECT_2, &_waterTilesTextureSegmentRomStart, &_waterTilesTextureSegmentRomEnd, &_waterTilesAssetsIndexSegmentRomStart, &_waterTilesAssetsIndexSegmentRomEnd, &_waterTilesSpritesheetIndexSegmentRomStart, &_waterTilesSpritesheetIndexSegmentRomEnd, (void*)0x802E5A00, (void*)0x802E6700, (void*)0x802E7400, (void*)0x802E7700, (void*)0x802E7A00, (void*)0x802E7B00, 1, 1);
            setSpriteScale(MAP_OBJECT_2, 6.0f, 1.0f, 6.0f);
            func_8002C6F8(MAP_OBJECT_2, 3);
            func_8002C768(MAP_OBJECT_2, 0x200);
            func_8002C7EC(MAP_OBJECT_2, 2);
            setSpriteDefaultRGBA(MAP_OBJECT_2, 0xFF, 0xFF, 0xFF, 0x80);
            func_8002C914(MAP_OBJECT_2, 0xFF, 0xFF, 0xFF, 0xFF);
            
            setMapObject(MAIN_MAP_INDEX, 1, MAP_OBJECT_2, 0, 192.0f, 0, 416.0f, 0, 0xFE, 0, 0);
        
            if (checkLifeEventBit(0x48)) {


                if (gSeason != WINTER) {
                    func_8002B138(MAP_OBJECT_3, &_bridgeTextureSegmentRomStart, &_bridgeTextureSegmentRomEnd, &_bridgeAssetsIndexSegmentRomStart, &_bridgeAssetsIndexSegmentRomEnd, &_bridgeSpritesheetIndexSegmentRomStart, &_bridgeSpritesheetIndexSegmentRomEnd, (void*)0x802E7C00, (void*)0x802E8280, (void*)0x802EB400, (void*)0x802EB500, (void*)0x802EB600, (void*)0x802EB700, 1, 1);
                } else {
                    func_8002B138(MAP_OBJECT_3, &_stonePillarTextureSegmentRomStart, &_stonePillarTextureSegmentRomEnd, &_stonePillarAssetsIndexSegmentRomStart, &_stonePillarAssetsIndexSegmentRomEnd, &_stonePillarSpritesheetIndexSegmentRomStart, &_stonePillarSpritesheetIndexSegmentRomEnd, (void*)0x802E7C00, (void*)0x802E8280, (void*)0x802EB400, (void*)0x802EB500, (void*)0x802EB600, (void*)0x802EB700, 1, 1);
                }

                setSpriteScale(MAP_OBJECT_3, 1.0f, 1.0f, 1.0f);
                func_8002C7EC(MAP_OBJECT_3, 3);
                setSpriteDefaultRGBA(MAP_OBJECT_3, 0xFF, 0xFF, 0xFF, 0xFF);
                func_8002C914(MAP_OBJECT_3, 0xFF, 0xFF, 0xFF, 0xFF);

                setMapObject(MAIN_MAP_INDEX, 2, MAP_OBJECT_3, 0, 96.0f, 224.0f, -448.0f, 0xFF, 0, 0, 0);

                if (gSeason != WINTER) {
                    func_8002B138(MAP_OBJECT_4, &_bridgeTextureSegmentRomStart, &_bridgeTextureSegmentRomEnd, &_bridgeAssetsIndexSegmentRomStart, &_bridgeAssetsIndexSegmentRomEnd, &_bridgeSpritesheetIndexSegmentRomStart, &_bridgeSpritesheetIndexSegmentRomEnd, (void*)0x802E8900, (void*)0x802E8F80, (void*)0x802EB400, (void*)0x802EB500, (void*)0x802EB600, (void*)0x802EB700, 1, 1);
                } else {
                    func_8002B138(MAP_OBJECT_4, &_stonePillarTextureSegmentRomStart, &_stonePillarTextureSegmentRomEnd, &_stonePillarAssetsIndexSegmentRomStart, &_stonePillarAssetsIndexSegmentRomEnd, &_stonePillarSpritesheetIndexSegmentRomStart, &_stonePillarSpritesheetIndexSegmentRomEnd, (void*)0x802E8900, (void*)0x802E8F80, (void*)0x802EB400, (void*)0x802EB500, (void*)0x802EB600, (void*)0x802EB700, 1, 1);
                }

                setSpriteScale(MAP_OBJECT_4, 1.0f, 1.0f, 1.0f);
                func_8002C7EC(MAP_OBJECT_4, 3);
                setSpriteDefaultRGBA(MAP_OBJECT_4, 0xFF, 0xFF, 0xFF, 0xFF);
                func_8002C914(MAP_OBJECT_4, 0xFF, 0xFF, 0xFF, 0xFF);
                setMapObject(MAIN_MAP_INDEX, 3, MAP_OBJECT_4, 0, 96.0f, 224.0f, -376, 0xFF, 0, 0, 0);

                if (gSeason != WINTER) {
                    func_8002B138(MAP_OBJECT_5, &_bridgeTextureSegmentRomStart, &_bridgeTextureSegmentRomEnd, &_bridgeAssetsIndexSegmentRomStart, &_bridgeAssetsIndexSegmentRomEnd, &_bridgeSpritesheetIndexSegmentRomStart, &_bridgeSpritesheetIndexSegmentRomEnd, (void*)0x802E9A00, (void*)0x802EA080, (void*)0x802EB400, (void*)0x802EB500, (void*)0x802EB600, (void*)0x802EB700, 1, 1);
                } else {
                    func_8002B138(MAP_OBJECT_5, &_stonePillarTextureSegmentRomStart, &_stonePillarTextureSegmentRomEnd, &_stonePillarAssetsIndexSegmentRomStart, &_stonePillarAssetsIndexSegmentRomEnd, &_stonePillarSpritesheetIndexSegmentRomStart, &_stonePillarSpritesheetIndexSegmentRomEnd, (void*)0x802E9A00, (void*)0x802EA080, (void*)0x802EB400, (void*)0x802EB500, (void*)0x802EB600, (void*)0x802EB700, 1, 1);
                }

                setSpriteScale(MAP_OBJECT_5, 1.0f, 1.0f, 1.0f);
                func_8002C7EC(MAP_OBJECT_5, 3);
                setSpriteDefaultRGBA(MAP_OBJECT_5, 0xFF, 0xFF, 0xFF, 0xFF);
                func_8002C914(MAP_OBJECT_5, 0xFF, 0xFF, 0xFF, 0xFF);
                setMapObject(MAIN_MAP_INDEX, 4, MAP_OBJECT_5, 0, 304.0f, 224.0f, -432.0f, 0xFF, 0, 0, 0);

                if (gSeason != WINTER) {
                    func_8002B138(MAP_OBJECT_6, &_bridgeTextureSegmentRomStart, &_bridgeTextureSegmentRomEnd, &_bridgeAssetsIndexSegmentRomStart, &_bridgeAssetsIndexSegmentRomEnd, &_bridgeSpritesheetIndexSegmentRomStart, &_bridgeSpritesheetIndexSegmentRomEnd, (void*)0x802EA700, (void*)0x802EAD80, (void*)0x802EB400, (void*)0x802EB500, (void*)0x802EB600, (void*)0x802EB700, 1, 1);
                } else {
                    func_8002B138(MAP_OBJECT_6, &_stonePillarTextureSegmentRomStart, &_stonePillarTextureSegmentRomEnd, &_stonePillarAssetsIndexSegmentRomStart, &_stonePillarAssetsIndexSegmentRomEnd, &_stonePillarSpritesheetIndexSegmentRomStart, &_stonePillarSpritesheetIndexSegmentRomEnd, (void*)0x802EA700, (void*)0x802EAD80, (void*)0x802EB400, (void*)0x802EB500, (void*)0x802EB600, (void*)0x802EB700, 1, 1);
                }

                setSpriteScale(MAP_OBJECT_6, 1.0f, 1.0f, 1.0f);
                func_8002C7EC(MAP_OBJECT_6, 3);
                setSpriteDefaultRGBA(MAP_OBJECT_6, 0xFF, 0xFF, 0xFF, 0xFF);
                func_8002C914(MAP_OBJECT_6, 0xFF, 0xFF, 0xFF, 0xFF);
                setMapObject(MAIN_MAP_INDEX, 5, MAP_OBJECT_6, 0, 304.0f, 224.0f, -368.0f, 0xFF, 0, 0, 0);
                
            }
            
            break;

        case TOP_OF_MOUNTAIN_1:
            
            if (checkLifeEventBit(0x48)) {

                func_8002B138(MAP_OBJECT_1, &_mountainPathTilesTextureSegmentRomStart, &_mountainPathTilesTextureSegmentRomEnd, &_mountainPathTilesAssetsIndexSegmentRomStart, &_mountainPathTilesAssetsIndexSegmentRomEnd, &_mountainPathTilesSpritesheetIndexSegmentRomStart, &_mountainPathTilesSpritesheetIndexSegmentRomEnd, (void*)0x802E4000, (void*)0x802E4400, (void*)0x802E4800, (void*)0x802E4900, (void*)0x802E4A00, (void*)0x802E4B00, 1, 1);
                setSpriteScale(MAP_OBJECT_1, 6.0f, 1.0f, 6.0f);
                func_8002C6F8(MAP_OBJECT_1, 3);
                func_8002C768(MAP_OBJECT_1, 0x200);
                func_8002C7EC(MAP_OBJECT_1, 2);
                setSpriteDefaultRGBA(MAP_OBJECT_1, 0xFF, 0xFF, 0xFF, 0x80);
                func_8002C914(MAP_OBJECT_1, 0xFF, 0xFF, 0xFF, 0xFF);
                setMapObject(MAIN_MAP_INDEX, 0, MAP_OBJECT_1, 0, 0, 216.0f, -128.0f, 0, 0xFE, 0, 0);

                if (gSeason != WINTER) {
                    func_8002B138(MAP_OBJECT_3, &_bridgeTextureSegmentRomStart, &_bridgeTextureSegmentRomEnd, &_bridgeAssetsIndexSegmentRomStart, &_bridgeAssetsIndexSegmentRomEnd, &_bridgeSpritesheetIndexSegmentRomStart, &_bridgeSpritesheetIndexSegmentRomEnd, (void*)0x802E7C00, (void*)0x802E8280, (void*)0x802EB400, (void*)0x802EB500, (void*)0x802EB600, (void*)0x802EB700, 1, 1);
                } else {
                    func_8002B138(MAP_OBJECT_3, &_stonePillarTextureSegmentRomStart, &_stonePillarTextureSegmentRomEnd, &_stonePillarAssetsIndexSegmentRomStart, &_stonePillarAssetsIndexSegmentRomEnd, &_stonePillarSpritesheetIndexSegmentRomStart, &_stonePillarSpritesheetIndexSegmentRomEnd, (void*)0x802E7C00, (void*)0x802E8280, (void*)0x802EB400, (void*)0x802EB500, (void*)0x802EB600, (void*)0x802EB700, 1, 1);
                }

                setSpriteScale(MAP_OBJECT_3, 1.0f, 1.0f, 1.0f);
                func_8002C7EC(MAP_OBJECT_3, 3);
                setSpriteDefaultRGBA(MAP_OBJECT_3, 0xFF, 0xFF, 0xFF, 0xFF);
                func_8002C914(MAP_OBJECT_3, 0xFF, 0xFF, 0xFF, 0xFF);
                setMapObject(MAIN_MAP_INDEX, 2, MAP_OBJECT_3, 0, -192.0f, 228.0f, 4.0f, 0xFF, 0, 0, 0);

                if (gSeason != WINTER) {
                    func_8002B138(MAP_OBJECT_4, &_bridgeTextureSegmentRomStart, &_bridgeTextureSegmentRomEnd, &_bridgeAssetsIndexSegmentRomStart, &_bridgeAssetsIndexSegmentRomEnd, &_bridgeSpritesheetIndexSegmentRomStart, &_bridgeSpritesheetIndexSegmentRomEnd, (void*)0x802E8900, (void*)0x802E8F80, (void*)0x802EB400, (void*)0x802EB500, (void*)0x802EB600, (void*)0x802EB700, 1, 1);
                } else {
                    func_8002B138(MAP_OBJECT_4, &_stonePillarTextureSegmentRomStart, &_stonePillarTextureSegmentRomEnd, &_stonePillarAssetsIndexSegmentRomStart, &_stonePillarAssetsIndexSegmentRomEnd, &_stonePillarSpritesheetIndexSegmentRomStart, &_stonePillarSpritesheetIndexSegmentRomEnd, (void*)0x802E8900, (void*)0x802E8F80, (void*)0x802EB400, (void*)0x802EB500, (void*)0x802EB600, (void*)0x802EB700, 1, 1);
                }
                
                setSpriteScale(MAP_OBJECT_4, 1.0f, 1.0f, 1.0f);
                func_8002C7EC(MAP_OBJECT_4, 3);
                setSpriteDefaultRGBA(MAP_OBJECT_4, 0xFF, 0xFF, 0xFF, 0xFF);
                func_8002C914(MAP_OBJECT_4, 0xFF, 0xFF, 0xFF, 0xFF);
                setMapObject(MAIN_MAP_INDEX, 3, MAP_OBJECT_4, 0, -192.0f, 224.0f, 64.0f, 0xFF, 0, 0, 0);

                if (gSeason != WINTER) {
                    func_8002B138(MAP_OBJECT_5, &_bridgeTextureSegmentRomStart, &_bridgeTextureSegmentRomEnd, &_bridgeAssetsIndexSegmentRomStart, &_bridgeAssetsIndexSegmentRomEnd, &_bridgeSpritesheetIndexSegmentRomStart, &_bridgeSpritesheetIndexSegmentRomEnd, (void*)0x802E9A00, (void*)0x802EA080, (void*)0x802EB400, (void*)0x802EB500, (void*)0x802EB600, (void*)0x802EB700, 1, 1);
                } else {
                    func_8002B138(MAP_OBJECT_5, &_stonePillarTextureSegmentRomStart, &_stonePillarTextureSegmentRomEnd, &_stonePillarAssetsIndexSegmentRomStart, &_stonePillarAssetsIndexSegmentRomEnd, &_stonePillarSpritesheetIndexSegmentRomStart, &_stonePillarSpritesheetIndexSegmentRomEnd, (void*)0x802E9A00, (void*)0x802EA080, (void*)0x802EB400, (void*)0x802EB500, (void*)0x802EB600, (void*)0x802EB700, 1, 1);
                }
                
                setSpriteScale(MAP_OBJECT_5, 1.0f, 1.0f, 1.0f);
                func_8002C7EC(MAP_OBJECT_5, 3);
                setSpriteDefaultRGBA(MAP_OBJECT_5, 0xFF, 0xFF, 0xFF, 0xFF);
                func_8002C914(MAP_OBJECT_5, 0xFF, 0xFF, 0xFF, 0xFF);
                setMapObject(MAIN_MAP_INDEX, 4, MAP_OBJECT_5, 0, -408.0f, 224.0f, -16.0f, 0xFF, 0, 0, 0);

                if (gSeason != WINTER) {
                    func_8002B138(MAP_OBJECT_6, &_bridgeTextureSegmentRomStart, &_bridgeTextureSegmentRomEnd, &_bridgeAssetsIndexSegmentRomStart, &_bridgeAssetsIndexSegmentRomEnd, &_bridgeSpritesheetIndexSegmentRomStart, &_bridgeSpritesheetIndexSegmentRomEnd, (void*)0x802EA700, (void*)0x802EAD80, (void*)0x802EB400, (void*)0x802EB500, (void*)0x802EB600, (void*)0x802EB700, 1, 1);
                } else {
                    func_8002B138(MAP_OBJECT_6, &_stonePillarTextureSegmentRomStart, &_stonePillarTextureSegmentRomEnd, &_stonePillarAssetsIndexSegmentRomStart, &_stonePillarAssetsIndexSegmentRomEnd, &_stonePillarSpritesheetIndexSegmentRomStart, &_stonePillarSpritesheetIndexSegmentRomEnd, (void*)0x802EA700, (void*)0x802EAD80, (void*)0x802EB400, (void*)0x802EB500, (void*)0x802EB600, (void*)0x802EB700, 1, 1);
                }
                
                setSpriteScale(MAP_OBJECT_6, 1.0f, 1.0f, 1.0f);
                func_8002C7EC(MAP_OBJECT_6, 3);
                setSpriteDefaultRGBA(MAP_OBJECT_6, 0xFF, 0xFF, 0xFF, 0xFF);
                func_8002C914(MAP_OBJECT_6, 0xFF, 0xFF, 0xFF, 0xFF);
                setMapObject(MAIN_MAP_INDEX, 5, MAP_OBJECT_6, 0, -408.0f, 224.0f, 48.0f, 0xFF, 0, 0, 0);
                
            } else {
                
                func_8002B138(MAP_OBJECT_1, &_bridgeTextureSegmentRomStart, &_bridgeTextureSegmentRomEnd, &_bridgeAssetsIndexSegmentRomStart, &_bridgeAssetsIndexSegmentRomEnd, &_bridgeSpritesheetIndexSegmentRomStart, &_bridgeSpritesheetIndexSegmentRomEnd, (void*)0x802E4000, (void*)0x802E4D00, (void*)0x802E7400, (void*)0x802E7700, (void*)0x802E7A00, (void*)0x802E7B00, 1, 1);
                setSpriteScale(MAP_OBJECT_1, 1.0f, 1.0f, 1.0f);
                func_8002C7EC(MAP_OBJECT_1, 3);
                setSpriteDefaultRGBA(MAP_OBJECT_1, 0xFF, 0xFF, 0xFF, 0xFF);
                func_8002C914(MAP_OBJECT_1, 0xFF, 0xFF, 0xFF, 0xFF);
                setMapObject(MAIN_MAP_INDEX, 0, MAP_OBJECT_1, 3, -160.0f, 224.0f, 8.0f, 0xFF, 0, 0, 0);
                
            }
            
            if (checkLifeEventBit(HOT_SPRINGS_COMPLETED)) {

                func_8002B138(MAP_OBJECT_2, &_steamTextureSegmentRomStart, &_steamTextureSegmentRomEnd, &_steamAssetsIndexSegmentRomStart, &_steamAssetsIndexSegmentRomEnd, &_steamSpritesheetIndexSegmentRomStart, &_steamSpritesheetIndexSegmentRomEnd, (void*)0x802E4C00, (void*)0x802E5800, (void*)0x802E6400, (void*)0x802E6500, (void*)0x802E6600, (void*)0x802E6700, 1, 1);
                setSpriteScale(MAP_OBJECT_2, 2.0f, 2.0f, 1.0f);
                func_8002C7EC(MAP_OBJECT_2, 2);
                setSpriteDefaultRGBA(MAP_OBJECT_2, 0xFF, 0xFF, 0xFF, 0x60);
                func_8002C914(MAP_OBJECT_2, 0xFF, 0xFF, 0xFF, 0xFF);
                setMapObject(MAIN_MAP_INDEX, 1, MAP_OBJECT_2, 0, 0.0f, 256.0f, -136.0f, 0xFF, 0xFE, 0, 0);

            }
            
            break;

        case POND:
            
            func_8002B138(MAP_OBJECT_1, &_pondWaterTextureSegmentRomStart, &_pondWaterTextureSegmentRomEnd, &_pondWaterAssetsIndexSegmentRomStart, &_pondWaterAssetsIndexSegmentRomEnd, &_pondWaterSpritesheetIndexSegmentRomStart, &_pondWaterSpritesheetIndexSegmentRomEnd, (void*)0x802E4000, (void*)0x802E4D00, (void*)0x802E7400, (void*)0x802E7700, (void*)0x802E7A00, (void*)0x802E7B00, 1, 1);
            setSpriteScale(MAP_OBJECT_1, 6.0f, 1.0f, 6.0f);
            func_8002C6F8(MAP_OBJECT_1, 3);
            func_8002C768(MAP_OBJECT_1, 0x200);
            func_8002C7EC(MAP_OBJECT_1, 2);
            setSpriteDefaultRGBA(MAP_OBJECT_1, 0xFF, 0xFF, 0xFF, 0x80);
            func_8002C914(MAP_OBJECT_1, 0xFF, 0xFF, 0xFF, 0xFF);
            setMapObject(MAIN_MAP_INDEX, 0, MAP_OBJECT_1, 0, 0.0f, 64.0f, -96.0f, 0x5A, 0xFE, 0, 0);

            break;

        case BEACH:

            func_8002B138(MAP_OBJECT_1, &_wavesTextureSegmentRomStart, &_wavesTextureSegmentRomEnd, &_wavesAssetsIndexSegmentRomStart, &_wavesAssetsIndexSegmentRomEnd, &_wavesSpritesheetIndexSegmentRomStart, &_wavesSpritesheetIndexSegmentRomEnd, (void*)0x802E4000, (void*)0x802E6400, (void*)0x802E8800, (void*)0x802E8900, (void*)0x802E8A00, (void*)0x802E8B00, 1, 1);
            setSpriteScale(MAP_OBJECT_1, 6.0f, 1.0f, 5.4f);
            func_8002C6F8(MAP_OBJECT_1, 3);
            func_8002C768(MAP_OBJECT_1, 0x200);
            func_8002C7EC(MAP_OBJECT_1, 2);
            setSpriteDefaultRGBA(MAP_OBJECT_1, 0xFF, 0xFF, 0xFF, 0x80);
            func_8002C914(MAP_OBJECT_1, 0xFF, 0xFF, 0xFF, 0xFF);
            setMapObject(MAIN_MAP_INDEX, 0, MAP_OBJECT_1, 0, 304.f, 112.0f, -10.0f, 0, 0xFE, 0, 1);
            
            break;

        case VILLAGE_2:

            func_8002B138(MAP_OBJECT_1, &_village2WaterTextureSegmentRomStart, &_village2WaterTextureSegmentRomEnd, &_village2WaterAssetsIndexSegmentRomStart, &_village2WaterAssetsIndexSegmentRomEnd, &_village2WaterSpritesheetIndexSegmentRomStart, &_village2WaterSpritesheetIndexSegmentRomEnd, (void*)0x802E4000, (void*)0x802E4000, (void*)0x802E7400, (void*)0x802E7700, (void*)0x802E7A00, (void*)0x802E7B00, 1, 1);
            setSpriteScale(MAP_OBJECT_1, 6.0f, 1.0f, 6.0f);
            func_8002C6F8(MAP_OBJECT_1, 3);
            func_8002C768(MAP_OBJECT_1, 0x200);
            func_8002C7EC(MAP_OBJECT_1, 2);
            setSpriteDefaultRGBA(MAP_OBJECT_1, 0xFF, 0xFF, 0xFF, 0x80);
            func_8002C914(MAP_OBJECT_1, 0xFF, 0xFF, 0xFF, 0xFF);
            setMapObject(MAIN_MAP_INDEX, 0, MAP_OBJECT_1, 0, -40.0f, 64.0f, -208.0f, 0, 0xFE, 0, 0);    

            break;
        
        case SQUARE:

            func_8002B138(MAP_OBJECT_1, &_squareFountainTextureSegmentRomStart, &_squareFountainTextureSegmentRomEnd, &_squareFountainAssetsIndexSegmentRomStart, &_squareFountainAssetsIndexSegmentRomEnd, &_squareFountainSpritesheetIndexSegmentRomStart, &_squareFountainSpritesheetIndexSegmentRomEnd, (void*)0x802E7C00, (void*)0x802E7C00, (void*)0x802E9600, (void*)0x802E9700, (void*)0x802E9800, (void*)0x802E9900, 1, 1);
            setSpriteScale(MAP_OBJECT_1, 5.0f, 1.0f, 5.0f);
            func_8002C6F8(MAP_OBJECT_1, 3);
            func_8002C768(MAP_OBJECT_1, 0x200);
            func_8002C7EC(MAP_OBJECT_1, 2);
            setSpriteDefaultRGBA(MAP_OBJECT_1, 0xFF, 0xFF, 0xFF, 0x80);
            func_8002C914(MAP_OBJECT_1, 0xFF, 0xFF, 0xFF, 0xFF);
            setMapObject(MAIN_MAP_INDEX, 0, MAP_OBJECT_1, 0, -256.0f, 104.f, -24.0f, 0, 0xFE, 0, 0);
            
            if (gSeason == SPRING) {
                
                if (18 < gDayOfMonth && gDayOfMonth < 23) {
                    func_8002B138(MAP_OBJECT_2, &_homeItemsTextureSegmentRomStart, &_homeItemsTextureSegmentRomEnd, &_homeItemsAssetsIndexSegmentRomStart, &_homeItemsAssetsIndexSegmentRomEnd, &_homeItemsSpritesheetIndexSegmentRomStart, &_homeItemsSpritesheetIndexSegmentRomEnd, (void*)0x802E4000, (void*)0x802E4D00, (void*)0x802E7400, (void*)0x802E7700, (void*)0x802E7A00, (void*)0x802E7B00, 1, 1);
                    setSpriteScale(MAP_OBJECT_2, 1.0f, 1.0f, 1.0f);
                    func_8002C7EC(MAP_OBJECT_2, 3);
                    setSpriteDefaultRGBA(MAP_OBJECT_2, 0xFF, 0xFF, 0xFF, 0xFF);
                    func_8002C914(MAP_OBJECT_2, 0xFF, 0xFF, 0xFF, 0xFF);
                    setMapObject(MAIN_MAP_INDEX, 1, MAP_OBJECT_2, 0xD, 80.0f, 128.0f, 208.0f, 0xFF, 0xFF, 0, 0);
                }

                // sowing festival
                if ((0x351 < gCutsceneIndex && gCutsceneIndex < 0x355) || gCutsceneIndex == FLOWER_FESTIVAL) {
                    
                   for (j = 4; j < 12; j++) {
                    
                        func_8002B138(j + MAP_OBJECT_2, &_festivalFlowersTextureSegmentRomStart, &_festivalFlowersTextureSegmentRomEnd, &_festivalFlowersAssetsIndexSegmentRomStart, &_festivalFlowersAssetsIndexSegmentRomEnd, &_festivalFlowersSpritesheetIndexSegmentRomStart, &_festivalFlowersSpritesheetIndexSegmentRomEnd, (void*)0x802E4000, 0, (void*)0x802E7400, (void*)0x802E7700, (void*)0x802E7A00, 0, 0, 1);
                        setSpriteScale(j + MAP_OBJECT_2, 1.0f, 1.0f, 1.0f);
                        func_8002C7EC(j + MAP_OBJECT_2, 3);
                        setSpriteDefaultRGBA(j + MAP_OBJECT_2, 0xFF, 0xFF, 0xFF, 0xFF);
                        func_8002C914(j + MAP_OBJECT_2, 0xFF, 0xFF, 0xFF, 0xFF);
                        
                    } 

                    setMapObject(MAIN_MAP_INDEX, 5, 0x67, 0, 176.0f, 96.0f, -96.0f, 0xFF, 0xFE, 0, 0);
                    setMapObject(MAIN_MAP_INDEX, 6, 0x68, 1, 176.0f, 96.0f, -128.0f, 0xFF, 0xFE, 0, 0);
                    setMapObject(MAIN_MAP_INDEX, 7, 0x69, 0, 176.0f, 96.0f, -160.0f, 0xFF, 0xFE, 0, 0);
                    setMapObject(MAIN_MAP_INDEX, 8, 0x6A, 1, 176.0f, 96.0f, -192.0f, 0xFF, 0xFE, 0, 0);
                    setMapObject(MAIN_MAP_INDEX, 9, 0x6B, 0, 176.0f, 96.0f, 64.0f, 0xFF, 0xFE, 0, 0);
                    setMapObject(MAIN_MAP_INDEX, 0xA, 0x6C, 1, 176.0f, 96.0f, 96.0f, 0xFF, 0xFE, 0, 0);
                    setMapObject(MAIN_MAP_INDEX, 0xB, 0x6D, 0, 176.0f, 96.0f, 128.0f, 0xFF, 0xFE, 0, 0);
                    setMapObject(MAIN_MAP_INDEX, 0xC, 0x6E, 1, 176.0f, 96.0f, 160.0f, 0xFF, 0xFE, 0, 0);
                    
                }

            }
            
            if (checkShopItemShouldBeDisplayed(0x35)) {
                loadShopItemSprite(0x35);
            }
            if (checkShopItemShouldBeDisplayed(0x36)) {
                loadShopItemSprite(0x36);
            }
            if (checkShopItemShouldBeDisplayed(0x37)) {
                loadShopItemSprite(0x37);
            }
            
            break;

       case BATHROOM:

       
            func_8002B138(MAP_OBJECT_1, &_stoneTextureSegmentRomStart, &_stoneTextureSegmentRomEnd, &_stoneAssetsIndexSegmentRomStart, &_stoneAssetsIndexSegmentRomEnd, &_stoneSpritesheetIndexSegmentRomStart, &_stoneSpritesheetIndexSegmentRomEnd, (void*)0x802E4000, (void*)0x802E4D00, (void*)0x802E7400, (void*)0x802E7700, (void*)0x802E7A00, (void*)0x802E7B00, 1, 1);
            setSpriteScale(MAP_OBJECT_1, 6.0f, 1.0f, 6.0f);
            func_8002C6F8(MAP_OBJECT_1, 3);
            func_8002C768(MAP_OBJECT_1, 0x200);
            func_8002C7EC(MAP_OBJECT_1, 2);
            setSpriteDefaultRGBA(MAP_OBJECT_1, 0xFF, 0xFF, 0xFF, 0x80);
            func_8002C914(MAP_OBJECT_1, 0xFF, 0xFF, 0xFF, 0xFF);

            setMapObject(MAIN_MAP_INDEX, 0, MAP_OBJECT_1, 0, -128.0f, 8.0f, -128.0f, 0, 0xFE, 0, 0);

            func_8002B138(MAP_OBJECT_2, &_steamTextureSegmentRomStart, &_steamTextureSegmentRomEnd, &_steamAssetsIndexSegmentRomStart, &_steamAssetsIndexSegmentRomEnd, &_steamSpritesheetIndexSegmentRomStart, &_steamSpritesheetIndexSegmentRomEnd, (void*)0x802E7C00, (void*)0x802E8900, (void*)0x802E9600, (void*)0x802E9700, (void*)0x802E9800, (void*)0x802E9900, 1, 1);
            setSpriteScale(MAP_OBJECT_2, 1.6f, 1.6f, 1.0f);
            func_8002C7EC(MAP_OBJECT_2, 2);
            setSpriteDefaultRGBA(MAP_OBJECT_2, 0xFF, 0xFF, 0xFF, 0x60);
            func_8002C914(MAP_OBJECT_2, 0xFF, 0xFF, 0xFF, 0xFF);

            setMapObject(MAIN_MAP_INDEX, 1, MAP_OBJECT_2, 0, -64.0f, 64.0f, -80.0f, 0xFF, 0xFE, 0, 0);
           
            break;
        
        case FLOWER_SHOP:
            
            if (checkShopItemShouldBeDisplayed(0)) {
                loadShopItemSprite(0);
            }
            if (checkShopItemShouldBeDisplayed(1)) {
                loadShopItemSprite(1);
            }
            if (checkShopItemShouldBeDisplayed(2)) {
                loadShopItemSprite(2);
            }
            if (checkShopItemShouldBeDisplayed(4)) {
                loadShopItemSprite(4);
            }
            if (checkShopItemShouldBeDisplayed(5)) {
                loadShopItemSprite(5);
            }
            if (checkShopItemShouldBeDisplayed(6)) {
                loadShopItemSprite(6);
            }
            if (checkShopItemShouldBeDisplayed(7)) {
                loadShopItemSprite(7);
            }
            if (checkShopItemShouldBeDisplayed(3)) {
                loadShopItemSprite(3);
            }
            if (checkShopItemShouldBeDisplayed(8)) {
                loadShopItemSprite(8);
            }
            if (checkShopItemShouldBeDisplayed(0xA)) {
                loadShopItemSprite(0xA);
                return;
            }
            break;

       case RANCH_STORE:
           
            if (checkShopItemShouldBeDisplayed(0x1A)) {
                loadShopItemSprite(0x1A);
            }
            if (checkShopItemShouldBeDisplayed(0x1D)) {
                loadShopItemSprite(0x1D);
            }
            if (checkShopItemShouldBeDisplayed(0x1B)) {
                loadShopItemSprite(0x1B);
            }
            if (checkShopItemShouldBeDisplayed(0x1C)) {
                loadShopItemSprite(0x1C);
                return;
            }
            break;
        
        case RICK_STORE:
            
            if (checkShopItemShouldBeDisplayed(0x15)) {
                loadShopItemSprite(0x15);
            }
            if (checkShopItemShouldBeDisplayed(0x14)) {
                loadShopItemSprite(0x14);
            }
            if (checkShopItemShouldBeDisplayed(0x13)) {
                loadShopItemSprite(0x13);
            }
            if (checkShopItemShouldBeDisplayed(0x16)) {
                loadShopItemSprite(0x16);
            }
            if (checkShopItemShouldBeDisplayed(0x18)) {
                loadShopItemSprite(0x18);
            }
            if (checkShopItemShouldBeDisplayed(0x19)) {
                loadShopItemSprite(0x19);
            }
            if (checkShopItemShouldBeDisplayed(0x17)) {
                loadShopItemSprite(0x17);
                func_80038AE0(0, 0);
            }
            
            break;

        case SOUVENIR_SHOP:
            
            if (checkShopItemShouldBeDisplayed(0x25)) {
                loadShopItemSprite(0x25);
            }
            if (checkShopItemShouldBeDisplayed(0x26)) {
                loadShopItemSprite(0x26);
            }
            if (checkShopItemShouldBeDisplayed(0x27)) {
                loadShopItemSprite(0x27);
            }
            
            break;

        case COOP:
            
            for (i = 0; i < 6; i++) { 
                if (gChickens[i].flags & 0x10) {
                    func_80073244(i);
                }
            }
            
            break;

        case BARN:

            for (i = 0; i < MAX_FARM_ANIMALS; i++) {
                
                if (gFarmAnimals[i].flags & 8) {
                    func_8007341C(i);
                }
                if (gFarmAnimals[i].flags & 0x10) {
                    func_8007341C(8);
                }
                
            }
            
            break;

        case VINEYARD:
            
            func_8002B138(MAP_OBJECT_1, &_vineyardTreeTextureSegmentRomStart, &_vineyardTreeTextureSegmentRomEnd, &_vineyardTreeAssetsIndexSegmentRomStart, &_vineyardTreeAssetsIndexSegmentRomEnd, &_vineyardTreeSpritesheetIndexSegmentRomStart, &_vineyardTreeSpritesheetIndexSegmentRomEnd, (void*)0x802E4000, (void*)0x802E4D00, (void*)0x802E7400, (void*)0x802E7700, (void*)0x802E7A00, (void*)0x802E7B00, 1, 1);
            setSpriteScale(MAP_OBJECT_1, 2.0f, 2.0f, 1.0f);
            func_8002C7EC(MAP_OBJECT_1, 3);
            setSpriteDefaultRGBA(MAP_OBJECT_1, 0xFF, 0xFF, 0xFF, 0xFF);
            func_8002C914(MAP_OBJECT_1, 0xFF, 0xFF, 0xFF, 0xFF);
            
            if (checkSpecialDialogueBit(0x14B)) {
                setMapObject(MAIN_MAP_INDEX, 0, MAP_OBJECT_1, 1, -416.0f, 144.0f, -352.0f, 0xFF, 0, 0, 0);
            } else {
                setMapObject(MAIN_MAP_INDEX, 0, MAP_OBJECT_1, 0, -416.0f, 144.0f, -352.0f, 0xFF, 0, 0, 0);
            }

        break;
    }
    
}

//INCLUDE_ASM(const s32, "level", func_80073244);

void func_80073244(u8 itemIndex) {

    Vec3f arr[6];

    MemoryRead_32 *ptr = (MemoryRead_32*)arr;
    MemoryRead_32 *ptr2 = (MemoryRead_32*)D_8011FB28;

    do {
        *(Aligned32*)ptr++ = *(Aligned32*)ptr2++;
    } while (ptr2 != (D_8011FB28 + 0x40));    

    *(Vec2f*)ptr = *(Vec2f*)ptr2;

    func_8002B138(itemIndex + MAP_OBJECT_1, &_homeItemsTextureSegmentRomStart, &_homeItemsTextureSegmentRomEnd, &_homeItemsAssetsIndexSegmentRomStart, &_homeItemsAssetsIndexSegmentRomEnd, &_homeItemsSpritesheetIndexSegmentRomStart, &_homeItemsSpritesheetIndexSegmentRomEnd, (void*)0x802E4000, (void*)0x802E4D00, (void*)0x802E7400, (void*)0x802E7700, (void*)0x802E7A00, (void*)0x802E7B00, 1, 1);
    setSpriteScale(itemIndex + MAP_OBJECT_1, 1.0f, 1.0f, 1.0f);
    func_8002C7EC(itemIndex + MAP_OBJECT_1, 3);
    setSpriteDefaultRGBA(itemIndex + MAP_OBJECT_1, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002C914(itemIndex + MAP_OBJECT_1, 0xFF, 0xFF, 0xFF, 0xFF);

    setMapObject(MAIN_MAP_INDEX, itemIndex, itemIndex + MAP_OBJECT_1, 0xC, arr[itemIndex].x, arr[itemIndex].y, arr[itemIndex].z, 0xFF, 0xFF, 0, 0);
    
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

    func_8002B138(itemIndex + MAP_OBJECT_1, &_homeItemsTextureSegmentRomStart, &_homeItemsTextureSegmentRomEnd, &_homeItemsAssetsIndexSegmentRomStart, &_homeItemsAssetsIndexSegmentRomEnd, &_homeItemsSpritesheetIndexSegmentRomStart, &_homeItemsSpritesheetIndexSegmentRomEnd, (void* )0x802E4000, (void* )0x802E4D00, (void* )0x802E7400, (void* )0x802E7700, (void* )0x802E7A00, (void* )0x802E7B00, (void* )1, (u8) (void* )1);
    setSpriteScale(itemIndex + MAP_OBJECT_1, 1.0f, 1.0f, 1.0f);
    func_8002C7EC(itemIndex + MAP_OBJECT_1, 3);
    setSpriteDefaultRGBA(itemIndex + MAP_OBJECT_1, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002C914(itemIndex + MAP_OBJECT_1, 0xFF, 0xFF, 0xFF, 0xFF);

    setMapObject(MAIN_MAP_INDEX, itemIndex, itemIndex + MAP_OBJECT_1, 0xB, arr[itemIndex].x, arr[itemIndex].y, arr[itemIndex].z, 0xFF, 0xFF, 0, 0);

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

    func_8002B138(arg0 + MAP_OBJECT_1, &_homeItemsTextureSegmentRomStart, &_homeItemsTextureSegmentRomEnd, &_homeItemsAssetsIndexSegmentRomStart, &_homeItemsAssetsIndexSegmentRomEnd, &_homeItemsSpritesheetIndexSegmentRomStart, &_homeItemsSpritesheetIndexSegmentRomEnd, (void* )0x802E4000, (void* )0x802E4D00, (void* )0x802E7400, (void* )0x802E7700, (void* )0x802E7A00, (void* )0x802E7B00, (void* )1, (u8) (void* )1);
    setSpriteScale(arg0 + MAP_OBJECT_1, 1.0f, 1.0f, 1.0f);
    func_8002C7EC(arg0 + MAP_OBJECT_1, 3);
    setSpriteDefaultRGBA(arg0 + MAP_OBJECT_1, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002C914(arg0 + MAP_OBJECT_1, 0xFF, 0xFF, 0xFF, 0xFF);

    setMapObject(MAIN_MAP_INDEX, arg0, arg0 + MAP_OBJECT_1, 0xB, arr[arg0].x, arr[arg0].y, arr[arg0].z, 0xFF, 0xFF, 0, 0);
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
// TODO: matches but have to match preceding rodata
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

//INCLUDE_ASM(const s32, "level", setAdditionalMapAdditionsForLevel);

void setAdditionalMapAdditionsForLevel(u16 mapIndex) {
    
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

//INCLUDE_ASM(const s32, "level", func_80074C50);

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