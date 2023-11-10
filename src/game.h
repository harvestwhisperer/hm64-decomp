#ifndef _GAME_H_
#define _GAME_H_

#include "common.h"

#define MAX_GOLD 999999
#define MAX_ITEM_SHIPPING_VALUE 999
#define MAX_ANIMAL_ITEM_SHIPPED 9999
#define MAX_TOTAL_CROPS_SHIPPED 99999
#define MAX_TOTAL_SHIPPING 999999
#define MAX_AFFECTION 0xFF
#define TOTAL_GAME_VARIABLES 80

#define SPRING 1
#define SUMMER 2
#define AUTUMN 3
#define WINTER 4

#define SUNDAY 0
#define MONDAY 1
#define TUESDAY 2
#define WEDNESDAY 3
#define THURSDAY 4
#define FRIDAY 5
#define SATURDAY 6

#define DAYTIME (5 < gHour && gHour < 18)
#define NIGHTTIME (17 < gHour && gHour < 24)
#define RAINING (2 <= gWeather && gWeather < 4)

#define EGG_VALUE 50

typedef struct {
    u16 unk_0;
    u16 unk_2;
    u16 unk_4;
    u8 unk_6;
} MainLoopCallbackInfo;

// might be dialogue.h
typedef struct {
	void *address;
	u32 maxValue;
	u8 set;
} GameVariable;

typedef struct {
    u8 unk_0;
    u8 unk_1;
    u8 unk_2;
    u8 unk_3;
    u8 unk_4;
    u8 unk_5; 
} Days;

typedef struct {
    u16 unk_0;
    u16 unk_2;
    u16 unk_4;
    u16 unk_6;
    u16 unk_8;
    u16 unk_A;
} Bits;

typedef struct {
    s16 unk_0;
    s16 unk_2;
    s16 unk_4;
    s16 unk_6;
    s16 unk_8;
    s16 unk_A;
} UnknownStruct;

extern inline int adjustValue(int initial, int value, int max);

void resetDailyBits();
extern void setFestivalDailyBits(void);      
void setSpecialDialogues();             
void setLetters();  

extern void setDefaultBabyName(u8);
extern void setWifeNameString(u8 wife);   

extern u8 getBacholeretteWithHighestAffection(u8 affectionLevel);

extern void setFlowerFestivalGoddess();
extern u8 handleHouseConstruction(u8);   

extern void handleDailyShipment();   

extern void func_80059D90();    
extern void func_8005A60C();      
extern void func_8005AAE4();  
extern void func_8005B09C(u8);   
extern void showTextBox(u16 arg0, u16 arg1, u16 arg2, int arg3, u16 arg4);
extern void showDialogueBox(u16 arg0, u16 arg1, u16 arg2, u32 arg3, u16 arg4);
extern void func_8005C07C(s16 arg0, u16 arg1);
extern void func_8005C940(u16, u16 callbackIndex); 
extern void func_8005CA2C(u16, u16);   
extern void func_8005CDCC(); 
extern void func_80061690(void);
extern u8 func_800616CC(u8);  
extern u16 func_80063A2C(u8);

extern MainLoopCallbackInfo D_80205230;

extern Vec4f D_8016F8A0;
extern Vec4f D_80180718;
extern Vec4f globalLightingRgba;

extern u32 gGold;
extern u16 gLumber;

extern u32 dailyShippingBinValue;
extern u32 gTotalCropsShipped;
extern u32 gTotalEggsShipped;
extern u32 gTotalGoldenMilkShipped;

// fish caught total
extern u32 D_80189838;

extern u8 gItemBeingHeld;

extern u8 gSeconds;
extern u8 gHour;
extern u8 gMinutes;
extern u8 gDayOfWeek;
extern u8 gDayOfMonth;
extern u8 gSeason;
extern u8 gNextSeason;
extern u8 gYear;

extern u8 gWife;
extern u8 gWifeConceptionCounter;
extern u8 gWifePregnancyCounter;
extern u8 gBabyAge;

extern u8 gElliGrievingCounter;

extern u8 gFlowerFestivalGoddess;
extern u8 gHarvestCoinFinder;
extern u8 gHarvestKing;
extern u8 gVoteForFlowerFestivalGoddess;

extern u8 houseExtensionConstructionCounter;

// only used by load.c and initialize.c 
extern u16 gTotalGrassTiles;
extern u16 gTotalPinkCatMintFlowersGrowing;

extern u8 D_801FC150;
extern u8 D_801FD621;
extern u8 D_80237412;  

extern u8 gFarmName[6];
extern u8 gWifeName[6];
extern u8 gBabyName[6];
extern u8 harvestKingName[6];

// season married in
extern u8 D_80183690;
// day of month married on
extern u8 D_801C6220;

static inline void resetGlobalLighting() {
    globalLightingRgba.r = 0;
    globalLightingRgba.g = 0;
    globalLightingRgba.b = 0;
    globalLightingRgba.a = 0;

    // dialogue icon struct
    D_80180718.r = 0;
    D_80180718.g = 0;
    D_80180718.b = 0;
    D_80180718.a = 0;
}

#endif