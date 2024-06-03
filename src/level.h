#ifndef _LEVEL_H_  
#define _LEVEL_H_

#include "common.h"

// TODO: finish adding map indices
/* map indices */
#define RANCH 0
#define ANN_ROOM 4
#define RANCH_BARN 5
#define RANCH_STORE 6
#define RANCH_HOUSE 7
#define BEACH 9
#define RACE_TRACK 0xD
#define MOUNTAIN_1 0x11
#define MOUNTAIN_1_SUMMER 0x12
#define MOUNTAIN_2 0x15
#define TOP_OF_MOUNTAIN_1 0x19
#define TOP_OF_MOUNTAIN_1_SUMMER 0x1A
#define MOON_MOUNTAIN 0x1D
#define CARPENTER_HUT 0x21
#define DUMPLING_HOUSE 0x22
#define POND 0x23
#define ELLI_ROOM 0x27
#define BAKERY 0x28
#define VILLAGE_1 0x29
#define VILLAGE_2 0x2D
#define POPURI_ROOM 0x31
#define FLOWER_SHOP 0x32
#define CHURCH 0x33
#define SOUVENIR_SHOP 0x34
#define SQUARE 0x35
#define RICK_STORE 0x39
#define MIDWIFE_HOUSE 0x3A
#define TAVERN 0x3B
#define LIBRARY 0x3C
#define MARIA_ROOM 0x3D
#define MAYOR_HOUSE 0x3E
#define POTION_SHOP_BEDROOM 0x3F
#define POTION_SHOP 0x40
#define UNUSED_MAP 0x41
#define HARVEST_SPRITE_CAVE 0x42
#define CAVE 0x43
#define MINE 0x44
#define MINE_2 0x45
#define KAREN_ROOM 0x46
#define VINEYARD 0x47
#define VINEYARD_HOUSE 0x4B
#define VINEYARD_CELLAR 0x4C
#define VINEYARD_CELLAR_BASEMENT 0x4D
#define ROAD 0x4E
#define FARM 0x52
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