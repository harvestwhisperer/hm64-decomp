#ifndef _GAME_H_
#define _GAME_H_

typedef struct {
    u16 unk_0;
    u16 unk_2;
    u16 unk_4;
    u8 unk_6;
} MainLoopCallbackInfo;


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
extern void func_80060E58();   
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


#endif