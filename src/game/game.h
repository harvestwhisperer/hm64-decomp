#ifndef _GAME_H_
#define _GAME_H_

#include "common.h"

#define MAX_GOLD 999999
#define MAX_LUMBER 999
#define MAX_FODDER 999
#define MAX_ITEM_SHIPPING_VALUE 999
#define MAX_ANIMAL_ITEM_SHIPPED 9999
#define MAX_TOTAL_CROPS_SHIPPED 99999
#define MAX_TOTAL_SHIPPING 999999
#define MAX_FISH_CAUGHT 999
#define MAX_SEEDS 20
#define MAX_CHICKEN_FEED 999
#define MAX_BLUE_MIST_SEEDS 1
#define MAX_FLOWER_SHOP_POINTS 100
#define MAX_BAKERY_CARD_POINTS 100
#define MAX_AFFECTION 255
#define TOTAL_GAME_VARIABLES 80
#define MAX_ALCOHOL_TOLERANCE 255

#define MARRIAGE_READY_AFFECTION_THRESHOLD 220

#define EGG_VALUE 50

typedef struct {
    u16 callbackIndex;
    u16 unk_2;
    u16 unk_4;
    u8 unk_6;
} MainLoopCallbackInfo;

extern inline int adjustValue(int initial, int value, int max);

void resetDailyBits();
extern void setFestivalDailyBits(void);      
void setSpecialDialogues();             
void setLetters();  

extern void setDefaultBabyName(u8);
extern void setWifeNameString(u8 wife);   

extern u8 getBacholeretteWithHighestAffection(u8 affectionLevel);

extern void setHarvestKingName(u8 harvestKing);
extern void setFlowerFestivalGoddess();
extern u8 handleHouseConstruction(u8);   

extern void handleDailyShipment();   

extern void updateFamilyStates();    
extern void func_8005A60C();      
extern void func_8005AAE4();  
extern void func_8005B09C(u8);   
extern void showTextBox(u16 arg0, u16 arg1, u16 arg2, u32 arg3, u16 arg4);
extern void showMessageBox(u16 arg0, u16 arg1, u16 arg2, u32 arg3, u16 arg4);
extern void func_8005C07C(s16 arg0, u16 arg1);
extern void func_8005C940(u16, u16 callbackIndex); 
extern void func_8005CA2C(u16, u16);   
extern void func_8005CDCC(); 
extern u8 calculateAnimalDirectionToPlayer(f32 animalX, f32 animalZ, f32 playerX, f32 playerZ);
extern bool func_80061178(void);
extern void func_80061690(u8);
extern u8 func_800616CC(u8);  
extern u16 func_80063A2C(u8);

// main loop callback functions
extern void func_8005C00C();
extern void func_8005CA64();
extern void func_8005CAA8();
extern void func_8005CB50();
extern void func_8005CBA4();
extern void func_8005CBF0();
extern void func_8005CEFC();
extern void func_8005CF4C();
extern void func_8005CF94();
extern void func_8005D0BC();
extern void func_8005D2B0();
extern void func_80060454();
extern void func_80060490();
extern void func_800604B0();
extern void func_800605F0();
extern void func_80060624();
extern void func_80060838();

extern MainLoopCallbackInfo gameLoopContext;

extern Vec4f D_8016F8A0;
extern Vec4f globalLightingRGBA;

extern u32 gGold;
extern u16 gLumber;

extern u32 dailyShippingBinValue;
extern u32 gTotalCropsShipped;
extern u32 gTotalEggsShipped;
extern u32 gTotalMilkShipped;

extern u32 totalFishCaught;

extern u8 gWife;
extern u8 gWifeConceptionCounter;
extern u8 gWifePregnancyCounter;
extern u8 gBabyAge;

extern u8 gElliGrievingCounter;

extern u8 gHarvestKing;
extern u8 gHarvestCoinFinder;
extern u8 gFlowerFestivalGoddess;
extern u8 gVoteForFlowerFestivalGoddess;

extern u8 babyBirthdaySeason;
extern u8 babyBirthdayDate;

extern u8 houseExtensionConstructionCounter;

// only used by load.c and initialize.c 
extern u16 gTotalGrassTiles;
extern u16 gTotalPinkCatMintFlowersGrowing;

extern u8 spiritFestivalAssistant2;
extern u8 spiritFestivalAssistant1;
extern u8 spiritFestivalAssistant3;  

extern u16 D_801886D2;

extern u8 gFarmName[6];
extern u8 gWifeName[6];
extern u8 gBabyName[6];
extern u8 harvestKingName[6];

extern u8 gHappiness;

extern u8 mariaHarrisNewlywedCounter;
extern u8 mariaHarrisPregnancyCounter;
extern u8 mariaHarrisBabyAge;
extern u8 popuriGrayNewlywedCounter;
extern u8 popuriGrayPregnancyCounter;
extern u8 popuriGrayBabyAge;
extern u8 elliJeffNewlywedCounter;
extern u8 elliJeffPregnancyCounter;
extern u8 elliJeffBabyAge;
extern u8 karenKaiBabyAge;
extern u8 karenKaiNewlywedCounter;
extern u8 karenPregnancyCounter;
extern u8 annCliffBabyAge;
extern u8 annCliffNewlywedCounter;
extern u8 annPregnancyCounter;

extern u8 numberOfSpiritFestivalAssistantsRecruited;
extern u8 spiritFestivalAssistant1;
extern u8 spiritFestivalAssistant2;
extern u8 spiritFestivalAssistant3;  

extern u8 mountainConstructionWorkDays;

extern u8 anniversarySeason;
extern u8 anniversaryDate;

extern u8 gHouseExtensionSelection;

extern Vec4f unknownRGBA;

static inline void resetGlobalLighting() {

    globalLightingRGBA.r = 0;
    globalLightingRGBA.g = 0;
    globalLightingRGBA.b = 0;
    globalLightingRGBA.a = 0;

    // dialogue icon struct
    unknownRGBA.r = 0;
    unknownRGBA.g = 0;
    unknownRGBA.b = 0;
    unknownRGBA.a = 0;

}

#endif