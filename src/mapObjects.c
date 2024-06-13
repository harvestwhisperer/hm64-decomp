#include "common.h"

#include "game.h"
#include "level.h"
#include "mapObjects.h"
#include "player.h"

#include "system/map.h"
#include "system/mathUtils.h"

// FIXME: this should be one symbol
// data
extern u8 D_80118700[218][5];
extern u8 D_80118701[218][5];
extern u8 D_80118702[218][5];
extern u8 D_80118703[218][5];
extern u8 D_80118704[218][5];
extern u8 D_8011870A;
extern u8 D_8011870B;

// shared
extern u8 D_801C3F35;
extern u8 D_801FD624;

// forward declarations
u8 func_800DA9A8(u8, u8, u8);


// jtbl_80122858
//INCLUDE_ASM(const s32, "mapObjects", func_800D9600);

void func_800D9600(u8 mapIndex) {

    u8 i, j;
    
    switch (mapIndex) {

        case FARM:
            for (i = 0; i < FIELD_WIDTH; i++) {
                for (j = 0; j < FIELD_HEIGHT; j++) {
                    if (farmFieldTiles[i][j]) {
                        func_800DAC70(mapIndex, farmFieldTiles[i][j], j, i);
                    }
                }
            }
            return;

        case GREENHOUSE:
            for (i = 0; i < FIELD_WIDTH; i++) {
                for (j = 0; j < FIELD_HEIGHT; j++) {
                    if (D_80182BA8[i][j]) {
                        func_800DAC70(mapIndex, D_80182BA8[i][j], j, i);
                    }
                }
            }
            return;

        case MOUNTAIN_1:
            for (i = 0; i < FIELD_WIDTH; i++) {
                for (j = 0; j < FIELD_HEIGHT; j++) {
                    if (D_80182FD8[i][j]) {
                        func_800DAC70(mapIndex, D_80182FD8[i][j], j, i);
                    }
                }
            }
            return;

        case MOUNTAIN_2:
            for (i = 0; i < FIELD_WIDTH; i++) {
                for (j = 0; j < FIELD_HEIGHT; j++) {
                    if (D_80189868[i][j]) {
                        func_800DAC70(mapIndex, D_80189868[i][j], j, i);
                    }
                }
            }
            return;    

        case TOP_OF_MOUNTAIN_1:
            for (i = 0; i < FIELD_WIDTH; i++) {
                for (j = 0; j < FIELD_HEIGHT; j++) {
                    if (D_8013D258[i][j]) {
                        func_800DAC70(mapIndex, D_8013D258[i][j], j, i);
                    }
                }
            }
            return;    

        case MOON_MOUNTAIN:
            for (i = 0; i < FIELD_WIDTH; i++) {
                for (j = 0; j < FIELD_HEIGHT; j++) {
                    if (D_8016FBF0[i][j]) {
                        func_800DAC70(mapIndex, D_8016FBF0[i][j], j, i);
                    }
                }
            }
            return;    

        case POND:
            for (i = 0; i < FIELD_WIDTH; i++) {
                for (j = 0; j < FIELD_HEIGHT; j++) {
                    if (D_8016FE08[i][j]) {
                        func_800DAC70(mapIndex, D_8016FE08[i][j], j, i);
                    }
                }
            }
            return;

        case CAVE:
            for (i = 0; i < FIELD_WIDTH; i++) {
                for (j = 0; j < FIELD_HEIGHT; j++) {
                    if (D_8016F8F8[i][j]) {
                        func_800DAC70(mapIndex, D_8016F8F8[i][j], j, i);
                    }
                }
            }
            return;    

        case MINE:
        case MINE_2:
            for (i = 0; i < FIELD_WIDTH; i++) {
                for (j = 0; j < FIELD_HEIGHT; j++) {
                    if (D_80170468[i][j]) {
                        func_800DAC70(mapIndex, D_80170468[i][j], j, i);
                    }
                }
            }
            return;    

        case RANCH:
            for (i = 0; i < FIELD_WIDTH; i++) {
                for (j = 0; j < FIELD_HEIGHT; j++) {
                    if (D_801C3FC0[i][j]) {
                        func_800DAC70(mapIndex, D_801C3FC0[i][j], j, i);
                    }
                }
            }
            return;   
        default:
            return;
    }
}

// jtbl_801229B8
//INCLUDE_ASM(const s32, "mapObjects", func_800D9AC8);

void func_800D9AC8(u8 mapIndex) {

    u8 i, j;
    
    for (i = 0; i < FIELD_WIDTH; i++) {
        for (j = 0; j < FIELD_HEIGHT; j++) {
            switch (mapIndex) {
                case MOUNTAIN_1:
                    D_80182FD8[i][j] = 0;
                    break;
                case MOUNTAIN_2:
                    D_80189868[i][j] = 0;
                    break;
                case TOP_OF_MOUNTAIN_1:
                    D_8013D258[i][j] = 0;
                    break;
                case MOON_MOUNTAIN:
                    D_8016FBF0[i][j] = 0;
                    break;
                case POND:
                    D_8016FE08[i][j] = 0;
                    break;
                case CAVE:
                    D_8016F8F8[i][j] = 0;
                    break;
                case MINE:
                case MINE_2:
                    D_80170468[i][j] = 0;
                    break;
                case RANCH:
                    D_801C3FC0[i][j] = 0;
                    break;
            }
        }
    }   
}

INCLUDE_ASM(const s32, "mapObjects", func_800D9BFC);

//INCLUDE_ASM(const s32, "mapObjects", func_800DA8B8);

u8 func_800DA8B8(u8 index) {

    u8 result;

    if (index >= 218) {
        result = 0;
    } else {
        result = D_80118700[index][0]; 
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "mapObjects", func_800DA8E8);

u8 func_800DA8E8(u8 index) {

    u8 result;

    if (index >= 218) {
        result = 0;
    } else {
        result = D_80118701[index][0]; 
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "mapObjects", func_800DA918);

u8 func_800DA918(u8 index) {

    u8 result;

    if (index >= 218) {
        result = 0;
    } else {
        result = D_80118702[index][0]; 
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "mapObjects", func_800DA948);

u8 func_800DA948(u8 index) {

    u8 result;

    if (index >= 218) {
        result = 0;
    } else {
        result = D_80118703[index][0]; 
    }
    
    return result;
    
}


//INCLUDE_ASM(const s32, "mapObjects", func_800DA978);

u8 func_800DA978(u8 index) {

    u8 result;

    if (index >= 218) {
        result = 0;
    } else {
        result = D_80118704[index][0]; 
    }
    
    return result;
    
}

// alternate
/*
u8 func_800DA978(u8 index) {

    if (index < 218) {
		return D_80118704[index][0];
    }
    
    return 0;
}
*/

// jtbl_80122AD0
//INCLUDE_ASM(const s32, "mapObjects", func_800DA9A8);

u8 func_800DA9A8(u8 mapIndex, u8 heightIndex, u8 widthIndex) {
    
    u8 result;
    
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
            result = farmFieldTiles[widthIndex][heightIndex];
            break;
        case GREENHOUSE:
            result = D_80182BA8[widthIndex][heightIndex];
            break;
        case MOUNTAIN_1:
            result = D_80182FD8[widthIndex][heightIndex];
            break;
        case MOUNTAIN_2:
            result = D_80189868[widthIndex][heightIndex];
            break;
        case TOP_OF_MOUNTAIN_1:
            result = D_8013D258[widthIndex][heightIndex];
            break;
        case MOON_MOUNTAIN:
            result = D_8016FBF0[widthIndex][heightIndex];
            break;
        case POND:
            result = D_8016FE08[widthIndex][heightIndex];
            break;
        case CAVE:
            result = D_8016F8F8[widthIndex][heightIndex];
            break;
        case MINE:
        case MINE_2:
            result = D_80170468[widthIndex][heightIndex];
            break;
        case RANCH:
            result = D_801C3FC0[widthIndex][heightIndex];
            break;
        }
    
    return result;
}

// jtbl_80122C30
//INCLUDE_ASM(const s32, "mapObjects", func_800DAA90);

bool func_800DAA90(u8 mapIndex, u8 arg1, u8 heightIndex, u8 widthIndex) {

    bool set = FALSE;

    switch (mapIndex) {

        case FARM:
            farmFieldTiles[widthIndex][heightIndex] = arg1;
            set = TRUE;
            break;

        case GREENHOUSE:
            D_80182BA8[widthIndex][heightIndex] = arg1;
            set = TRUE;
            break;

        case MOUNTAIN_1:
            if (arg1 != 1) {
                D_80182FD8[widthIndex][heightIndex] = arg1;
            } else {
                D_80182FD8[widthIndex][heightIndex] = 0;
            }
            set = TRUE;
            break;

        case MOUNTAIN_2:
            if (arg1 != 1) {
                D_80189868[widthIndex][heightIndex] = arg1;
            } else {
                D_80189868[widthIndex][heightIndex] = 0;
            }
            set = TRUE;
            break;

        case TOP_OF_MOUNTAIN_1:
            if (arg1 != 1) {
                D_8013D258[widthIndex][heightIndex] = arg1;
            } else {
               D_8013D258[widthIndex][heightIndex] = 0;
            }
            set = TRUE;
            break;

        case MOON_MOUNTAIN:
            if (arg1 != 1) {
                D_8016FBF0[widthIndex][heightIndex] = arg1;
            } else {
                D_8016FBF0[widthIndex][heightIndex] = 0;
            }
            set = TRUE;
            break;

        case POND:
            if (arg1 != 1) {
                D_8016FE08[widthIndex][heightIndex] = arg1;
            } else {
                D_8016FE08[widthIndex][heightIndex] = 0;
            }
            set = TRUE;
            break;
        
        case CAVE:
            if (arg1 != 1) {
                D_8016F8F8[widthIndex][heightIndex] = arg1;
            } else {
                D_8016F8F8[widthIndex][heightIndex] = 0;
            }
            set = TRUE;
            break;

        case MINE:
        case MINE_2:
            D_80170468[widthIndex][heightIndex] = arg1;
            set = TRUE;
            break;

        case RANCH:
            if (arg1 != 1) {
                D_801C3FC0[widthIndex][heightIndex] = arg1;
            } else {
                D_801C3FC0[widthIndex][heightIndex] = 0;
            }
            set = TRUE;
            break;
        
        
    }

    return set;
    
}

//INCLUDE_ASM(const s32, "mapObjects", func_800DAC70);

void func_800DAC70(u8 arg0, u8 arg1, u8 arg2, u8 arg3) {
    
    u8 temp;
    u8 temp2;
    
    if (func_800DAA90(arg0, arg1, arg2, arg3)) {
        
        temp2 = D_80118700[arg1][1];
        
        if (D_80118700[arg1][0] != 0xFF) {
            func_80038B58(0, D_80118700[arg1][0], D_801FD624 + arg2, D_801C3F35 + arg3);
        }
        
        temp = 0;
        
        switch (temp2) {
            default:
                func_80035004(0, temp2, arg2, arg3);
                break;

            case 0xFF:
                func_80035004(0, 0xFFFF, arg2, arg3);
                break;

            case 0:
                func_80035004(0, 0, arg2, arg3);
                break;
        }

        func_80036FA0(0);
        func_80036C08(temp);

    }
}

//INCLUDE_ASM(const s32, "mapObjects", func_800DAD74);

void func_800DAD74(u8 arg0, f32 arg1, u8 arg2) {

    Vec3f vec;

    u8 temp1;
    u8 temp2;
    u8 temp3;
    u8 temp4;
    u8 temp5;

    func_80065F94(&vec, arg1, arg2);

    if (vec.y != 65535.0f) {

        temp1 = vec.x;
        temp4 = temp1 - D_801FD624;
        
        temp2 = vec.z;
        temp5 = temp2 - D_801C3F35;
        
        if (func_800DAA90(gBaseMapIndex, arg0, temp4, temp5)) {

            temp3 = D_80118701[arg0][0];

            if (D_80118700[arg0][0] != 0xFF) {
                func_80038B58(0, D_80118700[arg0][0], D_801FD624 + temp4, D_801C3F35 + temp5);
            }

            switch (temp3) {
                default: 
                    func_80035004(0, temp3, temp4, temp5);
                    break;
                case 0xFF:
                    func_80035004(0, 0xFFFF, temp4, temp5);
                    break;
                case 0:
                    func_80035004(0, 0, temp4, temp5);
                    break;
            }
            
            func_80036FA0(0);
            func_80036C08(0);
            
        }
    }
}

#ifdef PERMUTER
u8 func_800DAF58(f32 arg0, u8 arg1) {

    Vec3f vec;
    Vec3f vec2;

    u8 temp1;
    u8 temp2;

    u8 temp3;
    
    u8 result = 0xFF;

    if (gBaseMapIndex == FARM || gBaseMapIndex == GREENHOUSE || gBaseMapIndex == MOUNTAIN_1 | gBaseMapIndex == MOUNTAIN_2 || gBaseMapIndex == TOP_OF_MOUNTAIN_1 || gBaseMapIndex == MOON_MOUNTAIN || gBaseMapIndex == POND || gBaseMapIndex == CAVE || ((u32) (gBaseMapIndex - MINE) < 2U) || gBaseMapIndex == RANCH) {

        func_80065F94(&vec, arg0, arg1);

        func_8003AF58(&vec2, 0, vec.x, vec.z);

        if (vec2.y != 65535.0f) {

            temp3 = gBaseMapIndex;
            
            temp1 = vec.x;
            temp1 -= D_801FD624;
            
            temp2 = vec.z;
            temp2 -= D_801C3F35;
            
            result = func_800DA9A8(temp3, temp1, temp2);
            
        }
        
    }

    return result;
  
}
#else
INCLUDE_ASM(const s32, "mapObjects", func_800DAF58);
#endif

INCLUDE_ASM(const s32, "mapObjects", func_800DB1BC);

INCLUDE_ASM(const s32, "mapObjects", func_800DB424);

INCLUDE_ASM(const s32, "mapObjects", func_800DB858);

INCLUDE_ASM(const s32, "mapObjects", func_800DB99C);

//INCLUDE_ASM(const s32, "mapObjects", func_800DBAC4);

// randomly add weed to farm tiles
void func_800DBAC4(void) {

    u8 i, j;

    if (gSeason != WINTER) {
        for (i = 0; i < FIELD_WIDTH; i++) {
            for (j = 0; j < FIELD_HEIGHT; j++) {
                if (farmFieldTiles[i][j] != 0 && farmFieldTiles[i][j] < 4 && !getRandomNumberInRange(0, 128)) {
                    func_800DAA90(FARM, WEED, j, i);
                }
            }
        }
    }
    
}

//INCLUDE_ASM(const s32, "mapObjects", func_800DBBB0);

// randomly break log fence pieces
void func_800DBBB0(u8 arg0) {

    u8 i, j;

    for (i = 0; i < FIELD_WIDTH; i++) {
        for (j = 0; j < FIELD_HEIGHT; j++) {
            if (farmFieldTiles[i][j] == LOG && !getRandomNumberInRange(0, arg0)) {
                func_800DAA90(FARM, BROKEN_LOG, j, i);
            }
        }
    }
    
}

//INCLUDE_ASM(const s32, "mapObjects", func_800DBC9C);

// randomly cut grass
void func_800DBC9C(u8 arg0) {

    u8 i;
    u8 j;
    
    int temp;
    int temp2;
    
    for (i = 0; i < FIELD_WIDTH; i++) {
    
        for (j = 0; j < FIELD_HEIGHT; j++) {
            
            temp = GRASS_CUT;
            temp2 = GRASS_PLANTED;
            
            if (farmFieldTiles[i][j] < temp && farmFieldTiles[i][j] >= temp2 && !getRandomNumberInRange(0, arg0)) {
                func_800DAA90(FARM, temp, j, i);    
            }
            
        }
    }
}

#ifdef PERMUTER
void func_800DBD88(u8 arg0) {

    u8 i;
    u8 j;
    
    int temp;
    int temp2;
    int temp3;
    int temp4;
    
    for (i = 0; i < FIELD_WIDTH; i++) {
    
        for (j = 0; j < FIELD_HEIGHT; j++) {
            
            temp = 0x85;
            temp2 = 0x90;
            temp3 = 8;
            temp4 = 0xC4;
            
            if (farmFieldTiles[i][j] >= temp3) {
                if (farmFieldTiles[i][j] >= temp) {
                     if (farmFieldTiles[i][j] < temp4 && farmFieldTiles[i][j] >= temp2) {
                        if (!getRandomNumberInRange(0, arg0)) {
                            func_800DAA90(FARM, 2, j, i);    
                        }   
                     }
                } else {
                    if (!getRandomNumberInRange(0, arg0)) {
                        func_800DAA90(FARM, 2, j, i);    
                    }
                }
            }
            
        }
    }
    
}
#else
INCLUDE_ASM(const s32, "mapObjects", func_800DBD88);
#endif

INCLUDE_ASM(const s32, "mapObjects", func_800DBE8C);

INCLUDE_ASM(const s32, "mapObjects", func_800DBF90);

INCLUDE_ASM(const s32, "mapObjects", func_800DC008);

INCLUDE_ASM(const s32, "mapObjects", func_800DC080);

//INCLUDE_ASM(const s32, "mapObjects", countPinkCatMintFlowersFarm);

u16 countPinkCatMintFlowersFarm(void) {

    u8 i, j;
    u16 count = 0;

    for (i = 0; i < FIELD_WIDTH; i++) {
        for (j = 0; j < FIELD_HEIGHT; j++) {
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

//INCLUDE_ASM(const s32, "mapObjects", countPinkCatMintFlowersGreenhouse);

u16 countPinkCatMintFlowersGreenhouse(void) {

    u8 i, j;
    u16 count = 0;

    for (i = 0; i < FIELD_WIDTH; i++) {
        for (j = 0; j < FIELD_HEIGHT; j++) {
            switch (D_80182BA8[i][j]) {
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

//INCLUDE_ASM(const s32, "mapObjects", func_800DC1E8);

void func_800DC1E8(void) {

    bool found = FALSE;
    u8 i, j;

    u8 temp;

    int temp2;
    int temp3;
    u8 *temp4;

    for (i = 0; i < FIELD_WIDTH; i++) {
    
        for (j = 0; j < FIELD_HEIGHT; j++) {

            temp2 = 0xB1;
            temp3 = 0xAF;
            
            // FIXME: something wrong
            if (farmFieldTiles[i][j] < temp2 && (temp4 = farmFieldTiles[i])[j] >= temp3 && !found) {
                
                if (func_800DAA90(FARM, 2, j, i)) {

                    temp = D_8011870B;

                    if (D_8011870A != 0xFF) {
                        func_80038B58(0, D_8011870A, D_801FD624 + j, D_801C3F35 + i);
                    }

                    switch (temp) {
                        default:
                            func_80035004(0, temp, j, i);
                            break;
                        case 0xFF:
                            func_80035004(0, 0xFFFF, j, i);
                            break;
                        case 0:
                            func_80035004(0, 0, j, i);
                            break;
                    }

                    func_80036FA0(0);
                    func_80036C08(0);
                    
                }
                
                found = TRUE;
                
            }
        }
    }
}