#include "common.h"

#include "game/fieldObjects.h"

#include "system/map.h"
#include "system/mapController.h"
#include "system/math.h"

#include "game/game.h"
#include "game/gameStatus.h"
#include "game/level.h"
#include "game/npc.h"
#include "game/player.h"
#include "game/weather.h"

// bss
extern u8 topOfMountain1FieldTiles[FIELD_HEIGHT][FIELD_WIDTH];
extern u8 moonMountainFieldTiles[FIELD_HEIGHT][FIELD_WIDTH];
extern u8 pondFieldTiles[FIELD_HEIGHT][FIELD_WIDTH];
extern u8 caveFieldTiles[FIELD_HEIGHT][FIELD_WIDTH];
extern u8 mineFieldTiles[FIELD_HEIGHT][FIELD_WIDTH];
extern u8 greenhouseFieldTiles[FIELD_HEIGHT][FIELD_WIDTH];
extern u8 mountain1FieldTiles[FIELD_HEIGHT][FIELD_WIDTH];
extern u8 mountain2FieldTiles[FIELD_HEIGHT][FIELD_WIDTH];
extern u8 ranchFieldTiles[FIELD_HEIGHT][FIELD_WIDTH];

// data
extern GroundObjectInfo groundObjectsInfo[MAX_FIELD_OBJECTS];

// forward declarations
u8 getGroundObjectIndexFromTilePosition(u8 mapIndex, u8 heightIndex, u8 widthIndex);

// TODO: put this in a header for field tiles data
extern u8 D_80113940[FIELD_HEIGHT][FIELD_WIDTH];

//INCLUDE_ASM("asm/nonmatchings/game/fieldObjects", setMapGroundObjects);

void setMapGroundObjects(u8 mapIndex) {

    u8 i, j;
    
    switch (mapIndex) {

        case FARM:
            for (i = 0; i < FIELD_HEIGHT; i++) {
                for (j = 0; j < FIELD_WIDTH; j++) {
                    if (farmFieldTiles[i][j]) {
                        addGroundObjectToMap(mapIndex, farmFieldTiles[i][j], j, i);
                    }
                }
            }
            break;

        case GREENHOUSE:
            for (i = 0; i < FIELD_HEIGHT; i++) {
                for (j = 0; j < FIELD_WIDTH; j++) {
                    if (greenhouseFieldTiles[i][j]) {
                        addGroundObjectToMap(mapIndex, greenhouseFieldTiles[i][j], j, i);
                    }
                }
            }
            break;

        case MOUNTAIN_1:
            for (i = 0; i < FIELD_HEIGHT; i++) {
                for (j = 0; j < FIELD_WIDTH; j++) {
                    if (mountain1FieldTiles[i][j]) {
                        addGroundObjectToMap(mapIndex, mountain1FieldTiles[i][j], j, i);
                    }
                }
            }
            break;

        case MOUNTAIN_2:
            for (i = 0; i < FIELD_HEIGHT; i++) {
                for (j = 0; j < FIELD_WIDTH; j++) {
                    if (mountain2FieldTiles[i][j]) {
                        addGroundObjectToMap(mapIndex, mountain2FieldTiles[i][j], j, i);
                    }
                }
            }
            break;    

        case TOP_OF_MOUNTAIN_1:
            for (i = 0; i < FIELD_HEIGHT; i++) {
                for (j = 0; j < FIELD_WIDTH; j++) {
                    if (topOfMountain1FieldTiles[i][j]) {
                        addGroundObjectToMap(mapIndex, topOfMountain1FieldTiles[i][j], j, i);
                    }
                }
            }
            break;    

        case MOON_MOUNTAIN:
            for (i = 0; i < FIELD_HEIGHT; i++) {
                for (j = 0; j < FIELD_WIDTH; j++) {
                    if (moonMountainFieldTiles[i][j]) {
                        addGroundObjectToMap(mapIndex, moonMountainFieldTiles[i][j], j, i);
                    }
                }
            }
            break;    

        case POND:
            for (i = 0; i < FIELD_HEIGHT; i++) {
                for (j = 0; j < FIELD_WIDTH; j++) {
                    if (pondFieldTiles[i][j]) {
                        addGroundObjectToMap(mapIndex, pondFieldTiles[i][j], j, i);
                    }
                }
            }
            break;

        case CAVE:
            for (i = 0; i < FIELD_HEIGHT; i++) {
                for (j = 0; j < FIELD_WIDTH; j++) {
                    if (caveFieldTiles[i][j]) {
                        addGroundObjectToMap(mapIndex, caveFieldTiles[i][j], j, i);
                    }
                }
            }
            break;    

        case MINE:
        case MINE_2:
            for (i = 0; i < FIELD_HEIGHT; i++) {
                for (j = 0; j < FIELD_WIDTH; j++) {
                    if (mineFieldTiles[i][j]) {
                        addGroundObjectToMap(mapIndex, mineFieldTiles[i][j], j, i);
                    }
                }
            }
            break;    

        case RANCH:
            for (i = 0; i < FIELD_HEIGHT; i++) {
                for (j = 0; j < FIELD_WIDTH; j++) {
                    if (ranchFieldTiles[i][j]) {
                        addGroundObjectToMap(mapIndex, ranchFieldTiles[i][j], j, i);
                    }
                }
            }
            break;   

    }
}

//INCLUDE_ASM("asm/nonmatchings/game/fieldObjects", clearForagableObjects);

void clearForagableObjects(u8 mapIndex) {

    u8 i, j;
    
    for (i = 0; i < FIELD_HEIGHT; i++) {

        for (j = 0; j < FIELD_WIDTH; j++) {

            switch (mapIndex) {

                case MOUNTAIN_1:
                    mountain1FieldTiles[i][j] = 0;
                    break;
                case MOUNTAIN_2:
                    mountain2FieldTiles[i][j] = 0;
                    break;
                case TOP_OF_MOUNTAIN_1:
                    topOfMountain1FieldTiles[i][j] = 0;
                    break;
                case MOON_MOUNTAIN:
                    moonMountainFieldTiles[i][j] = 0;
                    break;
                case POND:
                    pondFieldTiles[i][j] = 0;
                    break;
                case CAVE:
                    caveFieldTiles[i][j] = 0;
                    break;
                case MINE:
                case MINE_2:
                    mineFieldTiles[i][j] = 0;
                    break;
                case RANCH:
                    ranchFieldTiles[i][j] = 0;
                    break;

            }
        
        }

    }   

}

//INCLUDE_ASM("asm/nonmatchings/game/fieldObjects", func_800D9BFC);

void func_800D9BFC(void) {

    u8 i, j;

    for (i = 0; i < FIELD_HEIGHT; i++) {

        for (j = 0; j < FIELD_WIDTH; j++) {
            mountain1FieldTiles[i][j] = 0;
            mountain2FieldTiles[i][j] = 0;
            topOfMountain1FieldTiles[i][j] = 0;
            moonMountainFieldTiles[i][j] = 0;
            pondFieldTiles[i][j] = 0;
            caveFieldTiles[i][j] = 0;
            mineFieldTiles[i][j] = 0;
            ranchFieldTiles[i][j] = 0;
        }
        
    }

    switch (gSeason) {

        case SPRING:

            if (gBaseMapIndex != MOUNTAIN_1) {
                mountain1FieldTiles[22][12] = WEED;
                mountain1FieldTiles[20][13] = WEED;
                mountain1FieldTiles[22][4] = WEED;
                mountain1FieldTiles[17][13] = 0xCD;
                mountain1FieldTiles[14][15] = 0xCE;
                mountain1FieldTiles[12][10] = 0xD5;
                mountain1FieldTiles[2][16] = STUMP;
                mountain1FieldTiles[2][17] = 0xC9;
                mountain1FieldTiles[3][16] = 0xCA;
                mountain1FieldTiles[3][17] = 0xCB;
            }

            if (gBaseMapIndex != MOUNTAIN_2) {
                mountain2FieldTiles[23][5] = WEED;
                mountain2FieldTiles[16][1] = 0xCE;
                mountain2FieldTiles[23][6] = 0xD5;
                mountain2FieldTiles[20][8] = STUMP;
                mountain2FieldTiles[20][9] = 0xC9;
                mountain2FieldTiles[21][8] = 0xCA;
                mountain2FieldTiles[21][9] = 0xCB;
                mountain2FieldTiles[9][6] = STUMP;
                mountain2FieldTiles[9][7] = 0xC9;
                mountain2FieldTiles[10][6] = 0xCA;
                mountain2FieldTiles[10][7] = 0xCB;
            }

            if (gBaseMapIndex != TOP_OF_MOUNTAIN_1) {
                
                if (!(checkLifeEventBit(HOT_SPRINGS_COMPLETED)) && gYear == 1 && ((gSeason < WINTER) || ((gSeason == WINTER) && (gDayOfMonth < 12)))) {
                    topOfMountain1FieldTiles[3][7] = WEED;
                    topOfMountain1FieldTiles[9][16] = 0xCE;
                    topOfMountain1FieldTiles[3][14] = 0xD5;
                    topOfMountain1FieldTiles[4][6] = STUMP;
                    topOfMountain1FieldTiles[4][7] = 0xC9;
                    topOfMountain1FieldTiles[5][6] = 0xCA;
                    topOfMountain1FieldTiles[5][7] = 0xCB;
                    topOfMountain1FieldTiles[7][7] = STUMP;
                    topOfMountain1FieldTiles[7][8] = 0xC9;
                    topOfMountain1FieldTiles[8][7] = 0xCA;
                    topOfMountain1FieldTiles[8][8] = 0xCB;
                    topOfMountain1FieldTiles[5][10] = STUMP;
                    topOfMountain1FieldTiles[5][11] = 0xC9;
                    topOfMountain1FieldTiles[6][10] = 0xCA;
                    topOfMountain1FieldTiles[6][11] = 0xCB;
                }
                
            }
    
            if (gBaseMapIndex != CAVE) {
                
                caveFieldTiles[3][9] = 0xCD;
                caveFieldTiles[2][3] = 0xCC;
                caveFieldTiles[3][4] = 0xCC;
    
            }
    
            if (gBaseMapIndex != POND) {
                
                pondFieldTiles[3][5] = 0xCC;
                pondFieldTiles[4][14] = 0xCE;
                pondFieldTiles[3][10] = 0xD5;
                pondFieldTiles[8][9] = 0xD5;
                pondFieldTiles[8][6] = 0xD5;
                pondFieldTiles[3][4] = 0xD5;
                pondFieldTiles[2][1] = 0xD5;
                pondFieldTiles[4][12] = STUMP;
                pondFieldTiles[4][13] = 0xC9;
                pondFieldTiles[5][12] = 0xCA;
                pondFieldTiles[5][13] = 0xCB;
                
                if (blueMistFlowerPlot) {
                    pondFieldTiles[5][3] = blueMistFlowerPlot;
                }
                
            }
    
            if (gBaseMapIndex != RANCH && npcAffection[GRAY] >= 150) {
                memcpy((u32)&ranchFieldTiles, (u32)D_80113940, 0x1E0);
            }

        break;

        case SUMMER:

            if (gBaseMapIndex != MOUNTAIN_1) {
                mountain1FieldTiles[22][12] = WEED;
                mountain1FieldTiles[20][13] = WEED;
                mountain1FieldTiles[22][4] = WEED;
                mountain1FieldTiles[17][13] = 0xCD;
                mountain1FieldTiles[12][10] = 0xD5;
                mountain1FieldTiles[16][0] = 0xD0;
                mountain1FieldTiles[2][16] = STUMP;
                mountain1FieldTiles[2][17] = 0xC9;
                mountain1FieldTiles[3][16] = 0xCA;
                mountain1FieldTiles[3][17] = 0xCB;
            }

            if (gBaseMapIndex != MOUNTAIN_2) {
                mountain2FieldTiles[23][5] = WEED;
                mountain2FieldTiles[23][6] = 0xD5;
                mountain2FieldTiles[12][13] = 0xD0;
                mountain2FieldTiles[20][8] = STUMP;
                mountain2FieldTiles[20][9] = 0xC9;
                mountain2FieldTiles[21][8] = 0xCA;
                mountain2FieldTiles[21][9] = 0xCB;
                mountain2FieldTiles[11][7] = 0xCB;
                mountain2FieldTiles[10][6] = STUMP;
                mountain2FieldTiles[10][7] = 0xC9;
                mountain2FieldTiles[11][6] = 0xCA;
                mountain2FieldTiles[7][2] = 0xCF;
            }

            if (gBaseMapIndex != TOP_OF_MOUNTAIN_1) {
                if (!(checkLifeEventBit(HOT_SPRINGS_COMPLETED)) && gYear == 1 && ((gSeason < WINTER) || (gSeason == WINTER && gDayOfMonth < 12))) {
                    topOfMountain1FieldTiles[3][7] = WEED;
                    topOfMountain1FieldTiles[3][14] = 0xD5;
                    topOfMountain1FieldTiles[4][6] = STUMP;
                    topOfMountain1FieldTiles[4][7] = 0xC9;
                    topOfMountain1FieldTiles[5][6] = 0xCA;
                    topOfMountain1FieldTiles[5][7] = 0xCB;
                    topOfMountain1FieldTiles[7][7] = STUMP;
                    topOfMountain1FieldTiles[7][8] = 0xC9;
                    topOfMountain1FieldTiles[8][7] = 0xCA;
                    topOfMountain1FieldTiles[8][8] = 0xCB;
                    topOfMountain1FieldTiles[5][10] = STUMP;
                    topOfMountain1FieldTiles[5][11] = 0xC9;
                    topOfMountain1FieldTiles[6][10] = 0xCA;
                    topOfMountain1FieldTiles[6][11] = 0xCB;
                    topOfMountain1FieldTiles[3][15] = 0xCF;
                }
            }

            if (gBaseMapIndex != CAVE) {
                
                caveFieldTiles[3][9] = 0xCD;
                caveFieldTiles[2][3] = 0xCC;
                caveFieldTiles[3][4] = 0xCC;
    
            }

            if (gBaseMapIndex != POND) {
                
                pondFieldTiles[3][5] = 0xCC;
                pondFieldTiles[3][10] = 0xD5;
                pondFieldTiles[8][9] = 0xD5;
                pondFieldTiles[8][6] = 0xD5;
                pondFieldTiles[3][4] = 0xD5;
                pondFieldTiles[2][1] = 0xD5;
                pondFieldTiles[4][12] = STUMP;
                pondFieldTiles[4][13] = 0xC9;
                pondFieldTiles[5][12] = 0xCA;
                pondFieldTiles[5][13] = 0xCB;
                
                if (blueMistFlowerPlot) {
                    pondFieldTiles[5][3] = blueMistFlowerPlot;
                }
                
            }
    
            if (gBaseMapIndex != RANCH && npcAffection[GRAY] >= 150) {
                memcpy((u32)&ranchFieldTiles, (u32)D_80113940, 0x1E0);
            }

            break;
        
        case AUTUMN:

            if (gBaseMapIndex != MOUNTAIN_1) {
                mountain1FieldTiles[22][12] = WEED;
                mountain1FieldTiles[20][13] = WEED;
                mountain1FieldTiles[22][4] = WEED;
                mountain1FieldTiles[17][13] = 0xCD;
                mountain1FieldTiles[21][14] = 0xD2;
                mountain1FieldTiles[2][16] = STUMP;
                mountain1FieldTiles[2][17] = 0xC9;
                mountain1FieldTiles[3][16] = 0xCA;
                mountain1FieldTiles[3][17] = 0xCB;
            }

            if (gBaseMapIndex != MOUNTAIN_2) {
                mountain2FieldTiles[23][5] = WEED;
                mountain2FieldTiles[23][12] = 0xD2;
                mountain2FieldTiles[20][8] = STUMP;
                mountain2FieldTiles[20][9] = 0xC9;
                mountain2FieldTiles[21][8] = 0xCA;
                mountain2FieldTiles[21][9] = 0xCB;
                mountain2FieldTiles[9][6] = STUMP;
                mountain2FieldTiles[9][7] = 0xC9;
                mountain2FieldTiles[10][6] = 0xCA;
                mountain2FieldTiles[10][7] = 0xCB;
                mountain2FieldTiles[15][2] = 0xD3;
                mountain2FieldTiles[10][2] = 0xD1;
            }

            if (gBaseMapIndex != TOP_OF_MOUNTAIN_1) {
                if (!(checkLifeEventBit(HOT_SPRINGS_COMPLETED)) && gYear == 1 && ((gSeason < 4) || (gSeason == 4 && gDayOfMonth < 12))) {
                    topOfMountain1FieldTiles[3][7] = WEED;
                    topOfMountain1FieldTiles[3][3] = 0xD2;
                    topOfMountain1FieldTiles[4][6] = STUMP;
                    topOfMountain1FieldTiles[4][7] = 0xC9;
                    topOfMountain1FieldTiles[5][6] = 0xCA;
                    topOfMountain1FieldTiles[5][7] = 0xCB;
                    topOfMountain1FieldTiles[7][7] = STUMP;
                    topOfMountain1FieldTiles[7][8] = 0xC9;
                    topOfMountain1FieldTiles[8][7] = 0xCA;
                    topOfMountain1FieldTiles[8][8] = 0xCB;
                    topOfMountain1FieldTiles[5][10] = STUMP;
                    topOfMountain1FieldTiles[5][11] = 0xC9;
                    topOfMountain1FieldTiles[6][10] = 0xCA;
                    topOfMountain1FieldTiles[6][11] = 0xCB;
                    topOfMountain1FieldTiles[6][15] = 0xD1;
                }
            }
            
            if (gBaseMapIndex != MOON_MOUNTAIN && (9 < gDayOfMonth && gDayOfMonth < 13)) {
                moonMountainFieldTiles[17][14] = 0xD4;
            }

            if (gBaseMapIndex != CAVE) {
                
                caveFieldTiles[3][9] = 0xCD;
                caveFieldTiles[2][3] = 0xCC;
                caveFieldTiles[3][4] = 0xCC;
                caveFieldTiles[3][8] = 0xD3;
    
            }

            if (gBaseMapIndex != POND) {
                
                pondFieldTiles[3][5] = 0xCC;
                pondFieldTiles[5][1] = 0xD2;
                pondFieldTiles[4][12] = STUMP;
                pondFieldTiles[4][13] = 0xC9;
                pondFieldTiles[5][12] = 0xCA;
                pondFieldTiles[5][13] = 0xCB;

            }
    
            break;
        
        case WINTER:

            if (gBaseMapIndex != MOUNTAIN_1) {
                mountain1FieldTiles[2][16] = STUMP;
                mountain1FieldTiles[2][17] = 0xC9;
                mountain1FieldTiles[3][16] = 0xCA;
                mountain1FieldTiles[3][17] = 0xCB;
            }

            if (gBaseMapIndex != MOUNTAIN_2) {
                mountain2FieldTiles[20][8] = STUMP;
                mountain2FieldTiles[20][9] = 0xC9;
                mountain2FieldTiles[21][8] = 0xCA;
                mountain2FieldTiles[21][9] = 0xCB;
                mountain2FieldTiles[9][6] = STUMP;
                mountain2FieldTiles[9][7] = 0xC9;
                mountain2FieldTiles[10][6] = 0xCA;
                mountain2FieldTiles[10][7] = 0xCB;
            }

            if (gBaseMapIndex != TOP_OF_MOUNTAIN_1) {
                if (!(checkLifeEventBit(HOT_SPRINGS_COMPLETED)) && gYear == 1 && ((gSeason < 4U) || (gSeason == 4 && gDayOfMonth < 12))) {
                    topOfMountain1FieldTiles[4][6] = STUMP;
                    topOfMountain1FieldTiles[4][7] = 0xC9;
                    topOfMountain1FieldTiles[5][6] = 0xCA;
                    topOfMountain1FieldTiles[5][7] = 0xCB;
                    topOfMountain1FieldTiles[7][7] = STUMP;
                    topOfMountain1FieldTiles[7][8] = 0xC9;
                    topOfMountain1FieldTiles[8][7] = 0xCA;
                    topOfMountain1FieldTiles[8][8] = 0xCB;
                    topOfMountain1FieldTiles[5][10] = STUMP;
                    topOfMountain1FieldTiles[5][11] = 0xC9;
                    topOfMountain1FieldTiles[6][10] = 0xCA;
                    topOfMountain1FieldTiles[6][11] = 0xCB;
                }
            }

           if (gBaseMapIndex != POND) {
                
                pondFieldTiles[4][12] = STUMP;
                pondFieldTiles[4][13] = 0xC9;
                pondFieldTiles[5][12] = 0xCA;
                pondFieldTiles[5][13] = 0xCB;
                
            }
            
            break;
 
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/fieldObjects", getGroundObjectMapAdditionIndex);

u8 getGroundObjectMapAdditionIndex(u8 index) {

    u8 result;

    if (index >= MAX_FIELD_OBJECTS) {
        result = 0;
    } else {
        result = groundObjectsInfo[index].mapAdditionIndex; 
    }
    
    return result;
    
} 

//INCLUDE_ASM("asm/nonmatchings/game/fieldObjects", func_800DA8E8);

// unused
u8 func_800DA8E8(u8 index) {

    u8 result;

    if (index >= MAX_FIELD_OBJECTS) {
        result = 0;
    } else {
        result = groundObjectsInfo[index].spriteIndex; 
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/fieldObjects", getHeldItemIndexFromGroundObject);

u8 getHeldItemIndexFromGroundObject(u8 index) {

    u8 result;

    if (index >= MAX_FIELD_OBJECTS) {
        result = 0;
    } else {
        result = groundObjectsInfo[index].heldItemIndex; 
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/fieldObjects", func_800DA948);

u8 func_800DA948(u8 groundObjectIndex) {

    u8 result;

    if (groundObjectIndex >= MAX_FIELD_OBJECTS) {
        result = 0;
    } else {
        result = groundObjectsInfo[groundObjectIndex].flag2; 
    }
    
    return result;
    
}


//INCLUDE_ASM("asm/nonmatchings/game/fieldObjects", getGroundObjectFlags);

u8 getGroundObjectFlags(u8 index) {

    u8 result;

    if (index >= MAX_FIELD_OBJECTS) {
        result = 0;
    } else {
        result = groundObjectsInfo[index].flag3; 
    }
    
    return result;
    
}

// alternate
/*
u8 getGroundObjectFlags(u8 index) {

    if (index < MAX_FIELD_OBJECTS) {
		return D_80118704[index][0];
    }
    
    return 0;
}
*/

//INCLUDE_ASM("asm/nonmatchings/game/fieldObjects", getGroundObjectIndexFromTilePosition);

u8 getGroundObjectIndexFromTilePosition(u8 mapIndex, u8 heightIndex, u8 widthIndex) {
    
    u8 groundObjectIndex;
    
    switch (mapIndex) {

        case RANCH_SUMMER:
        case RANCH_AUTUMN:
        case RANCH_WINTER:
        case ANN_ROOM:
        case RANCH_BARN:
        case RANCH_STORE:
        case RANCH_HOUSE:
        case EMPTY_MAP_1:
        case BEACH_SPRING:
        case BEACH_SUMMER:
        case BEACH_AUTUMN:
        case BEACH_WINTER:
        case RACE_TRACK_SPRING:
        case RACE_TRACK_SUMMER:
        case RACE_TRACK_AUTUMN:
        case RACE_TRACK_WINTER:
        case MOUNTAIN_1_SUMMER:
        case MOUNTAIN_1_AUTUMN:
        case MOUNTAIN_1_WINTER:
        case MOUNTAIN_2_SUMMER:
        case MOUNTAIN_2_AUTUMN:
        case MOUNTAIN_2_WINTER:
        case TOP_OF_MOUNTAIN_1_SUMMER:
        case TOP_OF_MOUNTAIN_1_AUTUMN:
        case TOP_OF_MOUNTAIN_1_WINTER:
        case MOON_MOUNTAIN_SUMMER:
        case MOON_MOUNTAIN_AUTUMN:
        case MOON_MOUNTAIN_WINTER:
        case CARPENTER_HUT:
        case DUMPLING_HOUSE:
        case POND_SUMMER:
        case POND_AUTUMN:
        case POND_WINTER:
        case ELLI_ROOM:
        case BAKERY:
        case VILLAGE_1_SPRING:
        case VILLAGE_1_SUMMER:
        case VILLAGE_1_AUTUMN:
        case VILLAGE_1_WINTER:
        case VILLAGE_2_SPRING:
        case VILLAGE_2_SUMMER:
        case VILLAGE_2_AUTUMN:
        case VILLAGE_2_WINTER:
        case POPURI_ROOM:
        case FLOWER_SHOP:
        case CHURCH:
        case SOUVENIR_SHOP:
        case SQUARE_SPRING:
        case SQUARE_SUMMER:
        case SQUARE_AUTUMN:
        case SQUARE_WINTER:
        case RICK_STORE:
        case MIDWIFE_HOUSE:
        case TAVERN:
        case LIBRARY:
        case MARIA_ROOM:
        case MAYOR_HOUSE:
        case POTION_SHOP_BEDROOM:
        case POTION_SHOP:
        case EMPTY_MAP_2:
        case HARVEST_SPRITE_CAVE:
        case KAREN_ROOM:
        case VINEYARD_SPRING:
        case VINEYARD_SUMMER:
        case VINEYARD_AUTUMN:
        case VINEYARD_WINTER:
        case VINEYARD_HOUSE:
        case VINEYARD_CELLAR:
        case VINEYARD_CELLAR_BASEMENT:
        case ROAD_SPRING:
        case ROAD_SUMMER:
        case ROAD_AUTUMN:
        case ROAD_WINTER:
        case FARM_SUMMER:
        case FARM_AUTUMN:
        case FARM_WINTER:
            break;
            
        case FARM:
            groundObjectIndex = farmFieldTiles[widthIndex][heightIndex];
            break;
        case GREENHOUSE:
            groundObjectIndex = greenhouseFieldTiles[widthIndex][heightIndex];
            break;
        case MOUNTAIN_1:
            groundObjectIndex = mountain1FieldTiles[widthIndex][heightIndex];
            break;
        case MOUNTAIN_2:
            groundObjectIndex = mountain2FieldTiles[widthIndex][heightIndex];
            break;
        case TOP_OF_MOUNTAIN_1:
            groundObjectIndex = topOfMountain1FieldTiles[widthIndex][heightIndex];
            break;
        case MOON_MOUNTAIN:
            groundObjectIndex = moonMountainFieldTiles[widthIndex][heightIndex];
            break;
        case POND:
            groundObjectIndex = pondFieldTiles[widthIndex][heightIndex];
            break;
        case CAVE:
            groundObjectIndex = caveFieldTiles[widthIndex][heightIndex];
            break;
        case MINE:
        case MINE_2:
            groundObjectIndex = mineFieldTiles[widthIndex][heightIndex];
            break;
        case RANCH:
            groundObjectIndex = ranchFieldTiles[widthIndex][heightIndex];
            break;
        }
    
    return groundObjectIndex;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/fieldObjects", setFieldTile);

bool setFieldTile(u8 mapIndex, u8 groundObjectIndex, u8 heightIndex, u8 widthIndex) {

    bool set = FALSE;

    switch (mapIndex) {

        case FARM:
            farmFieldTiles[widthIndex][heightIndex] = groundObjectIndex;
            set = TRUE;
            break;

        case GREENHOUSE:
            greenhouseFieldTiles[widthIndex][heightIndex] = groundObjectIndex;
            set = TRUE;
            break;

        case MOUNTAIN_1:
            if (groundObjectIndex != 1) {
                mountain1FieldTiles[widthIndex][heightIndex] = groundObjectIndex;
            } else {
                mountain1FieldTiles[widthIndex][heightIndex] = 0;
            }
            set = TRUE;
            break;

        case MOUNTAIN_2:
            if (groundObjectIndex != 1) {
                mountain2FieldTiles[widthIndex][heightIndex] = groundObjectIndex;
            } else {
                mountain2FieldTiles[widthIndex][heightIndex] = 0;
            }
            set = TRUE;
            break;

        case TOP_OF_MOUNTAIN_1:
            if (groundObjectIndex != 1) {
                topOfMountain1FieldTiles[widthIndex][heightIndex] = groundObjectIndex;
            } else {
                topOfMountain1FieldTiles[widthIndex][heightIndex] = 0;
            }
            set = TRUE;
            break;

        case MOON_MOUNTAIN:
            if (groundObjectIndex != 1) {
                moonMountainFieldTiles[widthIndex][heightIndex] = groundObjectIndex;
            } else {
                moonMountainFieldTiles[widthIndex][heightIndex] = 0;
            }
            set = TRUE;
            break;

        case POND:
            if (groundObjectIndex != 1) {
                pondFieldTiles[widthIndex][heightIndex] = groundObjectIndex;
            } else {
                pondFieldTiles[widthIndex][heightIndex] = 0;
            }
            set = TRUE;
            break;
        
        case CAVE:
            if (groundObjectIndex != 1) {
                caveFieldTiles[widthIndex][heightIndex] = groundObjectIndex;
            } else {
                caveFieldTiles[widthIndex][heightIndex] = 0;
            }
            set = TRUE;
            break;

        case MINE:
        case MINE_2:
            mineFieldTiles[widthIndex][heightIndex] = groundObjectIndex;
            set = TRUE;
            break;

        case RANCH:
            if (groundObjectIndex != 1) {
                ranchFieldTiles[widthIndex][heightIndex] = groundObjectIndex;
            } else {
                ranchFieldTiles[widthIndex][heightIndex] = 0;
            }
            set = TRUE;
            break;
        
        
    }

    return set;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/fieldObjects", addGroundObjectToMap);

inline void addGroundObjectToMap(u8 mapIndex, u8 groundObjectIndex, u8 x, u8 z) {
    
    u8 spriteIndex;
    
    if (setFieldTile(mapIndex, groundObjectIndex, x, z)) {
        
        spriteIndex = groundObjectsInfo[groundObjectIndex].spriteIndex;
        
        if (groundObjectsInfo[groundObjectIndex].mapAdditionIndex != 0xFF) {
            setMapAdditionIndexFromCoordinates(MAIN_MAP_INDEX, groundObjectsInfo[groundObjectIndex].mapAdditionIndex, groundObjectsGridX + x, groundObjectsGridZ + z);
        }
        
        switch (spriteIndex) {

            default:
                setMapGroundObjectSpriteIndex(MAIN_MAP_INDEX, spriteIndex, x, z);
                break;

            case 0xFF:
                setMapGroundObjectSpriteIndex(MAIN_MAP_INDEX, 0xFFFF, x, z);
                break;

            case 0:
                setMapGroundObjectSpriteIndex(MAIN_MAP_INDEX, 0, x, z);
                break;

        }

        setGroundObjects(MAIN_MAP_INDEX);
        setGridToTileTextureMappings(MAIN_MAP_INDEX);

    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/fieldObjects", addGroundObjectToMapFromPlayerPosition);

void addGroundObjectToMapFromPlayerPosition(u8 groundObjectIndex, f32 arg1, u8 arg2) {

    Vec3f vec;

    u8 temp1;
    u8 temp2;
    u8 spriteIndex;
    u8 x;
    u8 z;

    vec = func_80065F94(arg1, arg2);

    if (vec.y != 65535.0f) {

        temp1 = vec.x;
        x = temp1 - groundObjectsGridX;
        
        temp2 = vec.z;
        z = temp2 - groundObjectsGridZ;

        addGroundObjectToMap(gBaseMapIndex, groundObjectIndex, x, z);
        
    }

}

static inline u8 subtractX(u8 x) {

    u8 result;
    
    result = x - groundObjectsGridX; 
    
    return result;
    
}

static inline u8 subtractZ(u8 z) {

    u8 result;
    
    result = z - groundObjectsGridZ; 
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/fieldObjects", getGroundObjectIndexFromPlayerPosition);

u8 getGroundObjectIndexFromPlayerPosition(f32 arg0, u8 arg1) {
    
    Vec3f vec;
    Vec3f vec2;

    u8 groundObjectIndex = 0xFF;

    if (gBaseMapIndex == FARM || gBaseMapIndex == GREENHOUSE || gBaseMapIndex == MOUNTAIN_1 | gBaseMapIndex == MOUNTAIN_2 || gBaseMapIndex == TOP_OF_MOUNTAIN_1 || gBaseMapIndex == MOON_MOUNTAIN || gBaseMapIndex == POND || gBaseMapIndex == CAVE || (CAVE < gBaseMapIndex && gBaseMapIndex < MINE_2 + 1) || gBaseMapIndex == RANCH) {

        vec = func_80065F94(arg0, arg1);
        vec2 = func_8003AF58(0, vec.x, vec.z);

        if (vec2.y != 65535.0f) {
            groundObjectIndex = getGroundObjectIndexFromTilePosition(gBaseMapIndex, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ);
        }
        
    }

    return groundObjectIndex;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/fieldObjects", getGroundObjectIndexFromCoordinates);

u8 getGroundObjectIndexFromCoordinates(f32 x, f32 z) {

    Vec3f vec;
    Vec3f vec2;
    
    u8 mapIndex;
    u8 groundObjectIndex = 0xFF;
    
    if (gBaseMapIndex == FARM || gBaseMapIndex == GREENHOUSE || gBaseMapIndex == MOUNTAIN_1 | gBaseMapIndex == MOUNTAIN_2 || gBaseMapIndex == TOP_OF_MOUNTAIN_1 || gBaseMapIndex == MOON_MOUNTAIN || gBaseMapIndex == POND || gBaseMapIndex == CAVE || (CAVE < gBaseMapIndex && gBaseMapIndex < MINE_2 + 1) || gBaseMapIndex == RANCH) {

        vec = convertWorldToTileCoordinates(0, x, z);
        vec2 = func_8003AF58(0, vec.x, vec.z);

        if (vec2.y != 65535.0f) {
            mapIndex = gBaseMapIndex;
            groundObjectIndex = getGroundObjectIndexFromTilePosition(mapIndex, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ);
        }
        
    }

    return groundObjectIndex;
    
}

INCLUDE_ASM("asm/nonmatchings/game/fieldObjects", func_800DB424);

//INCLUDE_ASM("asm/nonmatchings/game/fieldObjects", updateCropsIfRain);

void updateCropsIfRain(void) {

    u8 i, j;
    u8 flags, groundObjectIndex;
    
    if (gWeather == RAIN) {

        for (i = 0; i < FIELD_HEIGHT; i++) {
            
            for (j = 0; j < FIELD_WIDTH; j++) {

                groundObjectIndex = farmFieldTiles[i][j];

                if (groundObjectIndex) {
                    
                    if (groundObjectIndex < 0xDA) {
                        flags = groundObjectsInfo[groundObjectIndex].flag2;
                    } else {
                        flags = 0;
                    }

                    if (flags & 0x10) {
                        setFieldTile(FARM, groundObjectIndex + 1, j, i);
                    }
                        
                }
        
            }
        }
        
        if (blueMistFlowerPlot) {

            if (blueMistFlowerPlot < 0xDA) {
                flags = groundObjectsInfo[blueMistFlowerPlot].flag2;
            } else {
                flags = 0;
            }

            if (flags & 0x10) {
                blueMistFlowerPlot++;
            }
            
        }
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/fieldObjects", func_800DB99C);

bool func_800DB99C(void) {

    u8 i, j;
    
    bool found = FALSE;
    
    u8 temp1 = 0xFF;
    u8 temp2 = 0xFF;

    for (i = 0; i < FIELD_HEIGHT; i++) {
        
        for (j = 0; j < FIELD_WIDTH; j++) {

            if (farmFieldTiles[i][j] == GRASS_GROWN) {

                if (found) continue;
    
                temp1 = j;
                temp2 = i;
                
                if (!getRandomNumberInRange(0, 9)) {
                    found = TRUE;
                }

            }
    
        }

    }

    if (temp1 == 0xFF && temp2 == 0xFF) {
        return FALSE;
    } else {
        setFieldTile(FARM, GRASS_CUT, temp1, temp2);
        return TRUE;
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/fieldObjects", randomlyAddWeedsToFarmField);

void randomlyAddWeedsToFarmField(void) {

    u8 i, j;

    if (gSeason != WINTER) {
        for (i = 0; i < FIELD_HEIGHT; i++) {
            for (j = 0; j < FIELD_WIDTH; j++) {
                if (farmFieldTiles[i][j] != 0 && farmFieldTiles[i][j] < 4 && !getRandomNumberInRange(0, 128)) {
                    setFieldTile(FARM, WEED, j, i);
                }
            }
        }
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/fieldObjects", randomlyBreakLogPieces);

void randomlyBreakLogPieces(u8 randomRange) {

    u8 i, j;

    for (i = 0; i < FIELD_HEIGHT; i++) {
        for (j = 0; j < FIELD_WIDTH; j++) {
            if (farmFieldTiles[i][j] == LOG && !getRandomNumberInRange(0, randomRange)) {
                setFieldTile(FARM, BROKEN_LOG, j, i);
            }
        }
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/fieldObjects", func_800DBC9C);

// randomly cut grass
void func_800DBC9C(u8 randomRange) {

    u8 i;
    u8 j;
    
    for (i = 0; i < FIELD_HEIGHT; i++) {
    
        for (j = 0; j < FIELD_WIDTH; j++) {
            
            switch (farmFieldTiles[i][j]) {
                case GRASS_PLANTED ... GRASS_GROWN:
                    if (!getRandomNumberInRange(0, randomRange)) {
                        setFieldTile(FARM, GRASS_CUT, j, i);    
                    }
                    break;
                default:
                    break;
                
            }
            
            
        }
    }
    
}

// alternate
/*
void func_800DBC9C(u8 arg0) {

    u8 i;
    u8 j;
    
    int temp;
    int temp2;
    
    for (i = 0; i < FIELD_HEIGHT; i++) {
    
        for (j = 0; j < FIELD_WIDTH; j++) {
            
            temp = GRASS_CUT;
            temp2 = GRASS_PLANTED;
            
            if (farmFieldTiles[i][j] < temp && farmFieldTiles[i][j] >= temp2 && !getRandomNumberInRange(0, arg0)) {
                setFieldTile(FARM, temp, j, i);    
            }
            
        }
    }
}
*/

//INCLUDE_ASM("asm/nonmatchings/game/fieldObjects", func_800DBD88);

void func_800DBD88(u8 randomRange) {

    u8 i;
    u8 j;
    
    int temp;
    int temp2;
    int temp3;
    int temp4;
    
    for (i = 0; i < FIELD_HEIGHT; i++) {
    
        for (j = 0; j < FIELD_WIDTH; j++) {
            
            switch (farmFieldTiles[i][j]) {
                case 0x8 ... CORN_DEAD:
                case MOONDROP_PLANTED ... 0xC3:
                    if (!getRandomNumberInRange(0, randomRange)) {
                        setFieldTile(FARM, TILLED, j, i);    
                    }
                    break;
                default:
                    break;
                
            }
            
        }
    }
    
}

// alternate
/*
void func_800DBD88(u8 arg0) {

    u8 i;
    u8 j;
    
    int temp;
    int temp2;
    int temp3;
    int temp4;
    
    for (i = 0; i < FIELD_HEIGHT; i++) {
    
        for (j = 0; j < FIELD_WIDTH; j++) {
            
            temp = CORN_PLANT_WITHERED;
            temp2 = MOONDROP_PLANTED;
            temp3 = 8;
            temp4 = BOULDER;
            
            // FIXME: probably ranged switch
            if (farmFieldTiles[i][j] >= temp3) {
                if (farmFieldTiles[i][j] >= temp) {
                     if (farmFieldTiles[i][j] < temp4 && farmFieldTiles[i][j] >= temp2) {
                        goto label;
                     }
                } else {
label:
                    if (!getRandomNumberInRange(0, arg0)) {
                        setFieldTile(FARM, 2, j, i);    
                    }
                }
            }
            
        }
    }
    
}
*/

//INCLUDE_ASM("asm/nonmatchings/game/fieldObjects", func_800DBE8C);

// update greenhouse field after typhoon
void func_800DBE8C(u8 randomRange) {
    
    u8 i;
    u8 j;
    
    for (i = 0; i < FIELD_HEIGHT; i++) {
    
        for (j = 0; j < FIELD_WIDTH; j++) {
            
            switch (greenhouseFieldTiles[i][j]) {
                case 0x8 ... CORN_DEAD:
                case MOONDROP_PLANTED ... 0xC3:
                    if (!getRandomNumberInRange(0, randomRange)) {
                        setFieldTile(GREENHOUSE, TILLED, j, i);    
                    }
                    break;
                default:
                    break;
                
            }
            
        }

    }
    
}

// alternate
/*
void func_800DBE8C(u8 arg0) {
    
    u8 i;
    u8 j;
    
    int temp;
    int temp2;
    int temp3;
    int temp4;
    
    for (i = 0; i < FIELD_HEIGHT; i++) {
    
        for (j = 0; j < FIELD_WIDTH; j++) {
            
            temp = CORN_PLANT_WITHERED;
            temp2 = MOONDROP_PLANTED;
            temp3 = 8;
            temp4 = BOULDER;
            
            // FIXME: probably ranged switch
            if (greenhouseFieldTiles[i][j] >= temp3) {
                if (greenhouseFieldTiles[i][j] >= temp) {
                     if (greenhouseFieldTiles[i][j] < temp4 && greenhouseFieldTiles[i][j] >= temp2) {
                        goto label;
                     }
                } else {
label:
                    if (!getRandomNumberInRange(0, arg0)) {
                        setFieldTile(GREENHOUSE, 2, j, i);    
                    }
                }
            }
            
        }
    }
    
}
*/

//INCLUDE_ASM("asm/nonmatchings/game/fieldObjects", getFarmGrassTilesSum);

u16 getFarmGrassTilesSum(void) {
    
    u8 i;
    u8 j;
    u16 count = 0; 
    
    for (i = 0; i < FIELD_HEIGHT; i++) {
    
        for (j = 0; j < FIELD_WIDTH; j++) {

            switch (farmFieldTiles[i][j]) {
                case CORN_PLANT_WITHERED ... GRASS_CUT:
                    count++;
                    break;
                default:
                    break;
                
            }
            
        }

    }

    return count;
    
}

// alternate
/*
u16 getFarmGrassTilesSum(void) {
    
    u8 i;
    u8 j;
    u16 count = 0; 
    
    int temp;
    int temp2;
    
    for (i = 0; i < FIELD_HEIGHT; i++) {
    
        for (j = 0; j < FIELD_WIDTH; j++) {
            
            temp = MOONDROP_PLANTED;
            temp2 = CORN_PLANT_WITHERED;
            
            if (farmFieldTiles[i][j] < temp && farmFieldTiles[i][j] >= temp2) {
                count++;
            }
            
        }
    }

    return count;
    
}
*/

//INCLUDE_ASM("asm/nonmatchings/game/fieldObjects", func_800DC008);

u16 func_800DC008(void) {
    
    u8 i;
    u8 j;
    u16 count = 0; 
    
    
    for (i = 0; i < FIELD_HEIGHT; i++) {
    
        for (j = 0; j < FIELD_WIDTH; j++) {
            
            switch (farmFieldTiles[i][j]) {
                case 0x9C ... 0x9D:
                    count++;
                    break;
                default:
                    break;
                
            }
            
            
        }
    }

    return count;

}

// alternate
/*
u16 func_800DC008(void) {
    
    u8 i;
    u8 j;
    u16 count = 0; 

    int temp;
    int temp2;

    for (i = 0; i < FIELD_HEIGHT; i++) {
        
    for (j = 0; j < FIELD_WIDTH; j++) {
        
    temp = 0x9E;
    temp2 = 0x9C;

    if (farmFieldTiles[i][j] < temp && farmFieldTiles[i][j] >= temp2) {
        count++;
    }

            }
        }
        
    return count;
    
}
*/

//INCLUDE_ASM("asm/nonmatchings/game/fieldObjects", func_800DC080);

u16 func_800DC080(void) {

    u8 i;
    u8 j;
    u16 count = 0; 
    
    for (i = 0; i < FIELD_HEIGHT; i++) {
    
        for (j = 0; j < FIELD_WIDTH; j++) {
            
            switch (greenhouseFieldTiles[i][j]) {
                case 0x9C ... 0x9D:
                    count++;
                    break;
                default:
                    break;
                
            }
            
            
        }
    }

    return count;
    
}

// alternate
/*
u16 func_800DC080(void) {

    u8 i;
    u8 j;
    u16 count = 0; 
    
    int temp;
    int temp2;
    
    for (i = 0; i < FIELD_HEIGHT; i++) {
    
        for (j = 0; j < FIELD_WIDTH; j++) {
            
            temp = 0x9E;
            temp2 = 0x9C;
            
             if (greenhouseFieldTiles[i][j] < temp && greenhouseFieldTiles[i][j] >= temp2) {
                count++;
            }
            
        }
    }

    return count;
    
}
*/

//INCLUDE_ASM("asm/nonmatchings/game/fieldObjects", getFarmPinkCatMintFlowersTilesSum);

u16 getFarmPinkCatMintFlowersTilesSum(void) {

    u8 i, j;
    u16 count = 0;

    for (i = 0; i < FIELD_HEIGHT; i++) {
        for (j = 0; j < FIELD_WIDTH; j++) {
            switch (farmFieldTiles[i][j]) {
                case 0xAF ... 0xB0:
                    count++;
                    break;
                default:
                    break;
            }
        }
    }
    
    return count;
}

//INCLUDE_ASM("asm/nonmatchings/game/fieldObjects", getGreenhousePinkCatMintFlowersTilesSum);

u16 getGreenhousePinkCatMintFlowersTilesSum(void) {

    u8 i, j;
    u16 count = 0;

    for (i = 0; i < FIELD_HEIGHT; i++) {
        for (j = 0; j < FIELD_WIDTH; j++) {
            switch (greenhouseFieldTiles[i][j]) {
                // pink cat mint flowers
                case 0xAF ... 0xB0:
                    count++;
                    break;
                default:
                    break;
            }
        }
    }
    
    return count;
}

//INCLUDE_ASM("asm/nonmatchings/game/fieldObjects", func_800DC1E8);

void func_800DC1E8(void) {

    u8 i;
    u8 j;
    bool found = FALSE;

    u8 spriteIndex;

    for (i = 0; i < FIELD_HEIGHT; i++) {
    
        for (j = 0; j < FIELD_WIDTH; j++) {

            switch (farmFieldTiles[i][j]) {

                case 0xAF ... 0xB0:
                    
                    if (!found) {
                        
                        addGroundObjectToMap(FARM, 2, j, i);
                        found = TRUE;
                        
                    }
                    
                    break;

                default:
                    break;
                
            }
            
        }
        
    }

}