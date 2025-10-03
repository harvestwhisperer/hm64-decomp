#ifndef _GAME_H_
#define _GAME_H_

#include "common.h"

#include "system/dialogue.h"

#define ENTITY_PLAYER 0
#define ENTITY_MARIA 1
#define ENTITY_POPURI 2
#define ENTITY_ELLI 3
#define ENTITY_ANN 4
#define ENTITY_KAREN 5
#define ENTITY_BABY 6
#define ENTITY_HARRIS 7
#define ENTITY_GRAY 8
#define ENTITY_JEFF 9
#define ENTITY_CLIFF 10
#define ENTITY_KAI 11
#define ENTITY_MAYOR 12
#define ENTITY_MAYOR_WIFE 13
#define ENTITY_LILIA 14
#define ENTITY_BASIL 15
#define ENTITY_ELLEN 16
#define ENTITY_DOUG 17
#define ENTITY_GOTZ 18
#define ENTITY_SASHA 19
#define ENTITY_POTION_SHOP_DEALER 20
#define ENTITY_KENT 21
#define ENTITY_STU 22
#define ENTITY_MIDWIFE 23
#define ENTITY_MAY 24
#define ENTITY_RICK 25
#define ENTITY_PASTOR 26
#define ENTITY_SHIPPER 27
#define ENTITY_SAIBARA 28
#define ENTITY_DUKE 29
#define ENTITY_GREG 30
#define ENTITY_ASSISTANT_CARPENTER 31
#define ENTITY_MASTER_CARPENTER 32
#define ENTITY_HARVEST_SPRITE 33
#define ENTITY_BARLEY 34
#define ENTITY_SYDNEY 35
#define ENTITY_WIFE 35
#define ENTITY_ENTOMOLOGIST 36
#define ENTITY_GOURMET_JUDGE 37
#define ENTITY_PHOTOGRAPHER 38
#define ENTITY_SHADY_SALESMAN 39
#define ENTITY_MRS_MANA_SON 40
#define ENTITY_MRS_MANA 41
#define ENTITY_OLD_MAN 42
#define ENTITY_OLD_WOMAN 43
#define ENTITY_TOURIST_COUPLE_MAN 44
#define ENTITY_TOURIST_COUPLE_WOMAN 45
#define ENTITY_FESTIVAL_GIRL1 46
#define ENTITY_FESTIVAL_GIRL2 47
#define ENTITY_FESTIVAL_GIRL3 48
#define ENTITY_HARVEST_GODDESS 49
#define ENTITY_SPIRIT_FESTIVAL_OCARINA_PERFORMER 50
#define ENTITY_KAPPA 51
#define ENTITY_PRINCESS 52
#define ENTITY_EMPTY_NPC_SPRITE 53
#define ENTITY_BUNNY 54
#define ENTITY_MOUSE 55
#define ENTITY_FOX 56
#define ENTITY_MONKEY 57
#define ENTITY_HAWK 58
#define ENTITY_MOLE 59
#define ENTITY_SQUIRREL 60
#define ENTITY_DOG_RACE 61
#define ENTITY_CAT 62
#define ENTITY_DOG 63
#define ENTITY_DOG_TITLE 64
#define ENTITY_HORSE_PONY 65
#define ENTITY_HORSE_GROWN 66
#define ENTITY_CHICK 67
#define ENTITY_CHICKEN 68
#define ENTITY_CALF 69
#define ENTITY_YOUNG_COW 70
#define ENTITY_COW 71
#define ENTITY_LAMB 72
#define ENTITY_SHEEP 73
#define ENTITY_FROG 74
#define ENTITY_CRAB 75
#define ENTITY_HORNED_BEETLE 76
#define ENTITY_STAG_BEETLE 77
#define ENTITY_BELL_RING_CRICKET 78
#define ENTITY_CICADA 79
#define ENTITY_BIRD 80
#define ENTITY_LADYBUG 81
#define ENTITY_DRAGONFLY 82
#define ENTITY_GRASSHOPPER 83
#define ENTITY_SNAKE 84
#define ENTITY_PURPLE_BUTTERFLY 85
#define ENTITY_DAD 86
#define ENTITY_COW_INDOORS 87
#define ENTITY_HORSE_UNBRIDLED 88
#define ENTITY_HORSE_RACER 89
#define ENTITY_EMPTY_ENTITY 90
#define ENTITY_MARIA_HARRIS_BABY 97
#define ENTITY_POPURI_GRAY_BABY 98
#define ENTITY_ELLI_JEFF_BABY 99
#define ENTITY_ANN_CLIFF_BABY 100
#define ENTITY_KAREN_KAI_BABY 101

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

#define DIALOGUE_1_TEXT_INDEX 0
#define LIBRARY_TEXT_INDEX 1
#define DIARY_TEXT_INDEX 2
#define DIALOGUE_4_TEXT_INDEX 3
#define FESTIVAL_OVERLAY_SELECTIONS_TEXT_INDEX 4
#define LETTERS_TEXT_INDEX 5
#define SHOP_TEXT_INDEX 6
#define ANIMAL_INTERACTIONS_TEXT_INDEX 7
#define TV_TEXT_INDEX 8
#define DIALOGUE_10_TEXT_INDEX 9
#define NAMING_SCREEN_TEXT_INDEX 0xA
#define ELLI_TEXT_INDEX 0xB
#define KAI_TEXT_INDEX 0xC
#define KAREN_TEXT_INDEX 0xD
#define GOTZ_TEXT_INDEX 0xE
#define SASHA_TEXT_INDEX 0xF
#define CLIFF_TEXT_INDEX 0x10
#define JEFF_TEXT_INDEX 0x11
#define KENT_TEXT_INDEX 0x12
#define HARRIS_TEXT_INDEX 0x13
#define POPURI_TEXT_INDEX 0x14
#define MARIA_TEXT_INDEX 0x15
#define MAY_TEXT_INDEX 0x16
#define ANN_TEXT_INDEX 0x17
#define DOUG_TEXT_INDEX 0x18
#define GRAY_TEXT_INDEX 0x19
#define BASIL_TEXT_INDEX 0x1A
#define LILIA_TEXT_INDEX 0x1B
#define DUKE_TEXT_INDEX 0x1C
#define SHIPPER_TEXT_INDEX 0x1D
#define HARVEST_SPRITES_TEXT_INDEX 0x1E
#define ASSISTANT_CARPENTERS_TEXT_INDEX 0x1F
#define MASTER_CARPENTER_TEXT_INDEX 0x20
#define MAYOR_TEXT_INDEX 0x21
#define GREG_TEXT_INDEX 0x22
#define RICK_TEXT_INDEX 0x23
#define BARLEY_TEXT_INDEX 0x24
#define SYDNEY_TEXT_INDEX 0x25
#define POTION_SHOP_DEALER_TEXT_INDEX 0x26
#define MAYOR_WIFE_TEXT_INDEX 0x27
#define ELLEN_TEXT_INDEX 0x28
#define STU_TEXT_INDEX 0x29
#define MIDWIFE_TEXT_INDEX 0x2A
#define PASTOR_TEXT_INDEX 0x2B
#define SAIBARA_TEXT_INDEX 0x2C
#define CUTSCENES_1_TEXT_INDEX 0x2D
#define FUNERAL_INTRO_TEXT_INDEX 0x2E
#define DIALOGUE_48_TEXT_INDEX 0x2F
#define DIALOGUE_49_TEXT_INDEX 0x30
#define DIALOGUE_50_TEXT_INDEX 0x31
#define DIALOGUE_51_TEXT_INDEX 0x32
#define DIALOGUE_52_TEXT_INDEX 0x33
#define DOG_RACE_TEXT_INDEX 0x34
#define DIALOGUE_54_TEXT_INDEX 0x35
#define DIALOGUE_55_TEXT_INDEX 0x36
#define DIALOGUE_56_TEXT_INDEX 0x37
#define NEW_YEAR_FESTIVAL_TEXT_INDEX 0x38
#define SPIRIT_FESTIVAL_TEXT_INDEX 0x39
#define HORSE_RACE_TEXT_INDEX 0x3A
#define MARRIED_DIALOGUES_TEXT_INDEX 0x3B
#define DIALOGUE_61_TEXT_INDEX 0x3C
#define KAREN_CUTSCENES_1_TEXT_INDEX 0x3D
#define DIALOGUE_63_TEXT_INDEX 0x3E
#define DIALOGUE_64_TEXT_INDEX 0x3F
#define DIALOGUE_65_TEXT_INDEX 0x40
#define EGG_FESTIVAL_TEXT_INDEX 0x41
#define KAREN_CUTSCENES_2_TEXT_INDEX 0x42
#define VEGETABLE_FESTIVAL_TEXT_INDEX 0x43
#define BABY_TEXT_INDEX 0x44
#define MRS_MANA_AND_JOHN_TEXT_INDEX 0x45
#define ADDITIONAL_NPCS_TEXT_INDEX 0x46
#define NPC_BABY_TEXT_INDEX 0x47
#define HOW_TO_PLAY_TEXT_INDEX 0x48

#define DIALOGUE_TEXT_VADDR 0x8030A800
#define DIALOGUE_BYTECODE_INDEX_VADDR 0x8030C800
#define DIALOGUE_BYTECODE_VADDR 0x8030CC00

#define MARRIAGE_READY_AFFECTION_THRESHOLD 220

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

extern MainLoopCallbackInfo gameLoopContext;

extern Vec4f D_8016F8A0;
extern Vec4f globalLightingRGBA;

extern u32 gGold;
extern u16 gLumber;

extern u32 dailyShippingBinValue;
extern u32 gTotalCropsShipped;
extern u32 gTotalEggsShipped;
extern u32 gTotalGoldenMilkShipped;

extern u32 totalFishCaught;

extern u8 gItemBeingHeld;

extern u8 gSeconds;
extern u8 gHour;
extern u8 gMinutes;
extern u8 gDayOfWeek;
extern u8 gDayOfMonth;
extern u8 gSeason;
extern u8 gSeasonTomorrow;
extern u8 gYear;

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