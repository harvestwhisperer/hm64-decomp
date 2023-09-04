#ifndef _GAME_H_
#define _GAME_H_

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

#define EGG_VALUE 50

typedef struct {
    u16 unk_0;
    u16 unk_2;
    u16 unk_4;
    u8 unk_6;
} MainLoopCallbackInfo;

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

extern u32 adjustValue(s32 current, s32 amount, s32 max);

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
extern void func_8005AE8C(u16 arg0, u16 arg1, u16 arg2, int arg3, u16 arg4);
extern void func_8005AF94(u16 arg0, u16 arg1, u16 arg2, u32 arg3, u16 arg4);
extern void func_8005C07C(s16 arg0, u16 arg1);
extern void func_8005C940(u16, u16 callbackIndex); 
extern void func_8005CA2C(u16, u16);   
extern void func_8005CDCC(); 
extern u8 func_800616CC(u8);  
u16 func_80063A2C(u8 arg0);


extern Vec3f D_8016F8A4;
extern Vec4f D_80180718;
extern Vec4f globalLightingRgba;

extern u32 gGold;
extern u16 gLumber;

extern u32 dailyShippingBinValue;
extern u32 gTotalCropsShipped;
extern u32 gTotalEggsShipped;
extern u32 gTotalGoldenMilkShipped;

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

extern s32 gSongVolume;
extern u16 gCurrentSongIndex;

extern u8 gFlowerFestivalGoddess;
extern u8 gHarvestCoinFinder;
extern u8 gHarvestKing;
extern u8 gVoteForFlowerFestivalGoddess;

extern u8 houseExtensionConstructionCounter;

// only used by load.c and initialize.c 
extern u16 gTotalGrassTiles;
extern u16 gTotalPinkCatMintFlowersGrowing;

// maybe evaluation.c
extern u16 D_80215DF0;


static inline void handleCompleteShipment() {

    int moneyEarned;
    int checkOverflow;
    int temp;
    
    moneyEarned = dailyShippingBinValue;
    checkOverflow = gGold + moneyEarned;
    
    if (checkOverflow > MAX_GOLD) {
        temp = checkOverflow - MAX_GOLD;
        moneyEarned -= temp;
        checkOverflow = MAX_GOLD;
    }
    
    if (checkOverflow < 0) {
        moneyEarned -= checkOverflow;
    }
    
    gGold += moneyEarned;
    dailyShippingBinValue = 0;
}

static inline void handleAddShipment(s32 amount) {

    int temp;
    int checkShippingOverflow;
    int shippingValue;    
    int maxShipping;

    checkShippingOverflow = dailyShippingBinValue + amount;
    shippingValue = amount;
    maxShipping = MAX_TOTAL_SHIPPING;
    
    if (checkShippingOverflow > maxShipping) {
        temp = checkShippingOverflow - MAX_TOTAL_SHIPPING;
        shippingValue -= temp;
        checkShippingOverflow = maxShipping;
    }
    if (checkShippingOverflow < 0) {
        shippingValue -= checkShippingOverflow;
    }
    
    dailyShippingBinValue += shippingValue;
    
}

static inline void handleShipEgg() {

    int temp;
    
    int checkEggsOverflow;
    int eggValue;
    
    int maxEggsShipped = MAX_ANIMAL_ITEM_SHIPPED;

    checkEggsOverflow = gTotalEggsShipped + 1;
    eggValue = 1;
    
    if (maxEggsShipped < checkEggsOverflow) {
        temp = gTotalEggsShipped - (MAX_ANIMAL_ITEM_SHIPPED - 1);
        eggValue -= temp;
        checkEggsOverflow = MAX_ANIMAL_ITEM_SHIPPED;
    }
    if (checkEggsOverflow < 0) {
        eggValue -= checkEggsOverflow;
    }
    
    gTotalEggsShipped += eggValue;
    
}

#endif