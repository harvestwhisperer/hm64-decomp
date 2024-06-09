#ifndef _LEVEL_H_  
#define _LEVEL_H_

#include "common.h"

#define TOTAL_MAPS 0x60

// TODO: finish adding map indices
/* map indices */
#define RANCH 0
#define RANCH_SPRING 0
#define RANCH_SUMMER 1
#define RANCH_AUTUMN 2
#define RANCH_WINTER 3
#define ANN_ROOM 4
#define RANCH_BARN 5
#define RANCH_STORE 6
#define RANCH_HOUSE 7
#define EMPTY_MAP_1 8
#define BEACH 9
#define BEACH_SPRING 9
#define BEACH_SUMMER 0xA
#define BEACH_AUTUMN 0xB
#define BEACH_WINTER 0XC
#define RACE_TRACK 0xD
#define RACE_TRACK_SPRING 0xD
#define RACE_TRACK_SUMMER 0xE
#define RACE_TRACK_AUTUMN 0xF
#define RACE_TRACK_WINTER 0x10
#define MOUNTAIN_1 0x11
#define MOUNTAIN_1_SPRING 0x11
#define MOUNTAIN_1_SUMMER 0x12
#define MOUNTAIN_1_AUTUMN 0x13
#define MOUNTAIN_1_WINTER 0x14
#define MOUNTAIN_2 0x15
#define MOUNTAIN_2_SPRING 0x15
#define MOUNTAIN_2_SUMMER 0x16
#define MOUNTAIN_2_AUTUMN 0x17
#define MOUNTAIN_2_WINTER 0x18
#define TOP_OF_MOUNTAIN_1 0x19
#define TOP_OF_MOUNTAIN_1_SPRING 0x19
#define TOP_OF_MOUNTAIN_1_SUMMER 0x1A
#define TOP_OF_MOUNTAIN_1_AUTUMN 0x1B
#define TOP_OF_MOUNTAIN_1_WINTER 0x1C
#define MOON_MOUNTAIN 0x1D
#define MOON_MOUNTAIN_SPRING 0x1D
#define MOON_MOUNTAIN_SUMMER 0x1E
#define MOON_MOUNTAIN_AUTUMN 0x1F
#define MOON_MOUNTAIN_WINTER 0x20
#define CARPENTER_HUT 0x21
#define DUMPLING_HOUSE 0x22
#define POND 0x23
#define POND_SPRING 0x23
#define POND_SUMMER 0x24
#define POND_AUTUMN 0x25
#define POND_WINTER 0x26
#define ELLI_ROOM 0x27
#define BAKERY 0x28
#define VILLAGE_1 0x29
#define VILLAGE_1_SPRING 0x29
#define VILLAGE_1_SUMMER 0x2A
#define VILLAGE_1_AUTUMN 0x2B
#define VILLAGE_1_WINTER 0x2C
#define VILLAGE_2 0x2D
#define VILLAGE_2_SPRING 0x2D
#define VILLAGE_2_SUMMER 0x2E
#define VILLAGE_2_AUTUMN 0x2F
#define VILLAGE_2_WINTER 0x30
#define POPURI_ROOM 0x31
#define FLOWER_SHOP 0x32
#define CHURCH 0x33
#define SOUVENIR_SHOP 0x34
#define SQUARE 0x35
#define SQUARE_SPRING 0x35
#define SQUARE_SUMMER 0x36
#define SQUARE_AUTUMN 0x37
#define SQUARE_WINTER 0x38
#define RICK_STORE 0x39
#define MIDWIFE_HOUSE 0x3A
#define TAVERN 0x3B
#define LIBRARY 0x3C
#define MARIA_ROOM 0x3D
#define MAYOR_HOUSE 0x3E
#define POTION_SHOP_BEDROOM 0x3F
#define POTION_SHOP 0x40
#define EMPTY_MAP_2 0x41
#define HARVEST_SPRITE_CAVE 0x42
#define CAVE 0x43
#define MINE 0x44
#define MINE_2 0x45
#define KAREN_ROOM 0x46
#define VINEYARD 0x47
#define VINEYARD_SPRING 0x47
#define VINEYARD_SUMMER 0x48
#define VINEYARD_AUTUMN 0x49
#define VINEYARD_WINTER 0x4A
#define VINEYARD_HOUSE 0x4B
#define VINEYARD_CELLAR 0x4C
#define VINEYARD_CELLAR_BASEMENT 0x4D
#define ROAD 0x4E
#define ROAD_SPRING 0x4E
#define ROAD_SUMMER 0x4F
#define ROAD_AUTUMN 0x50
#define ROAD_WINTER 0x51
#define FARM 0x52
#define FARM_SPRING 0x52
#define FARM_SUMMER 0x53
#define FARM_AUTUMN 0x54
#define FARM_WINTER 0x55
#define GREENHOUSE 0x56
#define HOUSE 0x57
#define BARN 0x58
#define COOP 0x59
#define KITCHEN 0x5A
#define BATHROOM 0x5B

// TODO: finish adding these
/* entrances/exits */
#define HOUSE_EXIT 0
// 10 sick day
#define BATHROOM_TO_KITCHEN 0xB
#define MIDDLE_OF_HOUSE 0xC
#define ROAD_FROM_VILLAGE_1 0x17
#define CARPENTER_HUT_EXIT 0x20
#define MOON_MOUNTAIN_EXIT 0x21
#define DUMPLING_HOUSE_EXIT 0x26
#define RANCH_FROM_RANCH_STORE 0x2B
#define RANCH_FROM_RANCH_HOUSE 0x2C
// 0x32 square from church (wedding)
// 0x3E evaluation
#define MINE_EXIT 0x47
#define SPRING_ENTER 0x48
#define CARPERNTER_HUT_ENTER 0x49
#define ANN_ROOM_ENTER 0x4D
#define FLOWER_SHOP_ENTRANCE 0x57
// #0x6F farm entrance

// TODO: finish adding these
/* map flags */
#define INDOORS 1
#define OUTDOORS 2
#define HAS_SEASON_MAP 8

extern void func_8006E840(u16);  
extern void setLevelAudio(u16, u8, u8);     
extern Vec4f* func_8006EB94(Vec4f*, u16);      
extern void func_80073244(u8); 
extern u8 func_80074C38(u8);
extern u16 func_80074C50(u16);    

extern void setLevelGraphicsData(u16);        
extern u8 getLevelFlags(u16);    
extern void setEntrance(u16 entranceIndex);  

extern u8 gBaseMapIndex;
extern u8 gEntranceIndex;
extern u8 previousEntranceIndex;
extern u8 previousMapIndex;
extern u8 gMapWithSeasonIndex;

#endif