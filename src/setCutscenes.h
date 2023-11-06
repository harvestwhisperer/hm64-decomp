#ifndef _SET_CUTSCENES_H_
#define _SET_CUTSCENES_H_

#include "common.h"
#include "game.h"

#define MARRIED_TO_POPURI checkLifeEventBit(MARRIED) && gWife == POPURI
#define MARRIED_TO_ANN checkLifeEventBit(MARRIED) && gWife == ANN
#define MARRIED_TO_KAREN checkLifeEventBit(MARRIED) && gWife == KAREN
#define ARE_MARRIED checkLifeEventBit(MARRIED)
#define NOT_MARRIED !checkLifeEventBit(MARRIED)

/* cutscene indices */
/* farm cutscenes */
#define KENT_FARM_SOWING_FESITVAL 0x64
#define KENT_FARM_THANKSGIVING_FESTIVAL 0x67
#define KENT_AND_STU_FARM_PUPPIES 0x71
#define MAYOR_FARM_SOWING_FESTIVAL_YEAR_1 0x75
#define MAYOR_FARM_SOWING_FESTIVAL 0x76
#define MAYOR_FARM_HORSE_RACE 0x77
#define MAYOR_FARM_FIREFLY_FESTIVAL 0x78
#define MAYOR_FARM_SEA_FESTIVAL 0x79
#define MAYOR_FARM_HARVEST_FESTIVAL 0x7A
#define MAYOR_FARM_EGG_FESTIVAL 0x7B
#define MAYOR_FARM_SPIRIT_FESTIVAL_PLAYER_HARVEST_KING 0x7C 
#define MAYOR_FARM_SPIRIT_FESTIVAL 0x7D
#define DOUG_FARM_HORSE_RACE 0x7E
#define DOUG_FARM_DOG_RACE 0x80
#define DOUG_FARM_FAVOR 0x83
#define BASIL_FARM_FLOWER_FESTIVAL 0x89
#define BASIL_FARM_VEGETABLE_FESTIVAL 0x8A
#define BASIL_INTRO 0x8C
#define BASIL_POWER_NUT_GIFT 0x8D
#define BASIL_RETURN 0x8E
#define BASIL_LEAVING 0x8F
#define MARIA_FARM_GIFT_CHOCOLATE 0x90
#define MARIA_FARM_GIFT_RICE_BALLS 0x91
#define MARIA_FIREWORKS_INVITATION 0x92
#define MARIA_THANKSGIVING_GIFT 0x94
#define POPURI_FARM_GIFT_TEA 0x9A
#define POPURI_FARM_GIFT_COOKIES 0x9B
#define POPURI_FIREWORKS_INVITATION 0x9D
#define POPURI_THANKSGIVING_GIFT 0x9F
#define ELLI_FIREWORKS_INVITATION 0xA7
#define ELLI_THANKSGIVING_GIFT 0xAB
#define ANN_FIREWORKS_INVITATION 0xB4
#define ANN_THANKSGIVING_GIFT 0xB6
#define KAREN_FIREWORKS_INVITATION 0xC0
#define KAREN_THANKSGIVING_GIFT 0xC3
#define CLIFF_INTRO 0xD1
#define CLIFF_LEAVING_DEFAULT 0xD4
#define DOUG_DROPPING_OFF_COWS 0xED
#define MISS_MANA_PICKUP_COWS 0xEE

#define MOUNTAIN_FIREWORKS 0x130
#define ELLI_SPRAINED_ANKLE 0x157
#define ANN_AND_RICK 0x1A6
#define STARRY_NIGHT_FESTIVAL 0x1AC 
#define CHURCH_NEW_YEARS 0x1B3
#define POPURI_SICK 0x1B9
#define KAREN_YELLOW_HEART_EVENT_1 0x2C1
#define KAREN_YELLOW_HEART_EVENT_4 0x2C4
#define KAREN_SICK 0x2C7
#define BEACH_FIREWORKS 0x2EE
#define KAREN_AND_ELLI 0x2F2
#define KAREN_PINK_HEART_EVENT 0x2F3
#define SHADY_SALESMAN_AND_MARIA_1 0x322
#define SHADY_SALESMAN_AND_MARIA_2 0x323
#define ANN_SPRAINED_ANKLE 0x324
#define FIREFLY_FESTIVAL 0x41A
#define SEA_FESTIVAL 0x44C
#define MAYOR_VILLAGE_TOUR 0x5AD
#define INTRO 0x5B0
#define EVALUATION 0x60E

#define DEMO_CUTSCENE_1 0x5DC
#define DEMO_CUTSCENE_2 0x5DD
// 0x5DE
#define DEMO_CUTSCENE_3 0x5DF

u16 func_8009C054(u16 mapIndex);
extern u16 func_800A7AE8(u8);  
extern u16 func_800A7DFC();        
extern u16 func_800A87C4();   
extern void func_800A8F74();   
extern void loadCutscene(u32);  

// maybe game.c
extern u16 gCutsceneIndex;

extern u8 D_80113580[0x18][0x14];

// flags
extern s32 D_801891D4;
extern u16 D_8013DC2E;
extern u16 D_8018981C;

#endif