#include "common.h"

#include "system/controller.h"
#include "system/dialogue.h"
#include "system/entity.h"
#include "system/graphic.h"
#include "system/map.h"
#include "system/mapController.h"
#include "system/math.h"
#include "system/memory.h"
#include "system/message.h"

#include "game/animals.h"
#include "game/evaluation.h"
#include "game/game.h"
#include "game/gameAudio.h"
#include "game/gameStatus.h"
#include "game/transition.h"
#include "game/itemHandlers.h"
#include "game/level.h"
#include "mainLoop.h"
#include "game/fieldObjects.h"
#include "game/namingScreen.h"
#include "game/npc.h"
#include "game/player.h"
#include "game/setCutscenes.h"
#include "game/shop.h"
#include "game/spriteIndices.h"
#include "game/time.h"
#include "game/weather.h"

#include "buffers/buffers.h"

#include "ld_symbols.h"

// forward declaration
u8 func_80060DC0();

// shared bss
extern u8 gSeconds;
extern u8 gHour;
extern u8 gMinutes;
extern u8 gDayOfWeek;
extern u8 gDayOfMonth;
extern u8 gSeason;
extern u8 gSeasonTomorrow;
extern u8 gYear;

extern Vec4f globalLightingRGBA;

extern u8 gHarvestKing;
extern u8 gFlowerFestivalGoddess;
extern u8 gVoteForFlowerFestivalGoddess;

extern u8 gHappiness;

extern u8 gWife;
extern u8 gWifePregnancyCounter;
extern u8 gWifeConceptionCounter;

extern u8 gBabyAge;

extern u32 gGold;
extern u16 gLumber;
extern u32 dailyShippingBinValue;

extern u8 houseExtensionConstructionCounter;

extern u8 gItemBeingHeld;

extern u16 gCurrentAudioSequenceIndex;
extern u8 gNamingScreenIndex;

extern u8 gFarmName[6];
extern u8 gWifeName[6];
extern u8 gBabyName[6];
extern u8 harvestKingName[6];

extern u16 gTotalGrassTiles;
extern u16 gTotalPinkCatMintFlowersGrowing;

extern u32 gTotalCropsShipped;
extern u32 gTotalEggsShipped;
extern u32 gTotalMilkShipped;

extern u8 popuriGrayBabyAge;

extern u8 numberOfSpiritFestivalAssistantsRecruited;
extern u8 spiritFestivalAssistant1;
extern u8 spiritFestivalAssistant2;
extern u8 spiritFestivalAssistant3;  
extern u8 elliJeffBabyAge;

extern u8 mariaHarrisPregnancyCounter;
extern u8 mariaHarrisBabyAge;
extern u8 popuriGrayNewlywedCounter;
extern u8 popuriGrayPregnancyCounter;
extern u8 elliJeffNewlywedCounter;
extern u8 elliJeffPregnancyCounter;
extern u8 karenKaiBabyAge;
extern u8 karenKaiNewlywedCounter;
extern u8 karenPregnancyCounter;
extern u8 annCliffBabyAge;
extern u8 annCliffNewlywedCounter;
extern u8 annPregnancyCounter;

extern MainLoopCallbackInfo gameLoopContext;

// unused
extern u16 D_801FB686;   

extern u8 mariaHarrisNewlywedCounter;

extern u8 D_801C4216;
// cow festival winner affection (D_80189054)
extern u16 D_801886D2;
extern u8 D_80189054;
// animal price?
extern u32 D_801890E0;

// data

// lighting values based on hour

// FIXME: these could be D_80113B20[4][24] or struct { r[24]; g[24]; b[24]; a[24]; }

// sunny, weather type 4, typhoon
// r values
u8 D_80113B20[24] = {
    80,
    80,
    80,
    80,
    80,
    128,
    192,
    192,
    224,
    224,
    224,
    224,
    255,
    255,
    255,
    255,
    255,
    255,
    128,
    128,
    80,
    80,
    80,
    80,
};

// g
u8 D_80113B38[24] = {
    80,
    80,
    80,
    80,
    80,
    128,
    192,
    192,
    224,
    224,
    224,
    224,
    255,
    255,
    255,
    255,
    224,
    192,
    128,
    128,
    80,
    80,
    80,
    80,
};

// b
u8 D_80113B50[24] = {
    80,
    80,
    80,
    80,
    80,
    128,
    192,
    192,
    224,
    224,
    224,
    224,
    255,
    255,
    255,
    255,
    224,
    192,
    128,
    128,
    80,
    80,
    80,
    80,
};

// a
u8 D_80113B68[24] = {
    80,
    80,
    80,
    80,
    80,
    128,
    192,
    192,
    224,
    224,
    224,
    224,
    255,
    255,
    255,
    255,
    224,
    192,
    128,
    128,
    80,
    80,
    80,
    80,
};

// rain

// r
u8 D_80113B80[24] = {
    80,
    80,
    80,
    80,
    80,
    112,
    112,
    112,
    128,
    128,
    128,
    128,
    144,
    144,
    144,
    144,
    128,
    128,
    112,
    112,
    80,
    80,
    80,
    80,
};

// g
u8 D_80113B98[24] = {
    80,
    80,
    80,
    80,
    80,
    112,
    112,
    112,
    128,
    128,
    128,
    128,
    144,
    144,
    144,
    144,
    128,
    128,
    112,
    112,
    80,
    80,
    80,
    80,
};

// b
u8 D_80113BB0[24] = {
    128,
    128,
    128,
    128,
    128,
    192,
    192,
    192,
    208,
    208,
    208,
    208,
    255,
    255,
    255,
    255,
    208,
    208,
    192,
    192,
    128,
    128,
    128,
    128,
};

// a
u8 D_80113BC8[24] = {
    80,
    80,
    80,
    80,
    80,
    112,
    112,
    112,
    128,
    128,
    128,
    128,
    144,
    144,
    144,
    144,
    128,
    128,
    112,
    112,
    80,
    80,
    80,
    80,
};

// snow

// r
u8 D_80113BE0[24] = {
    80,
    80,
    80,
    80,
    80,
    128,
    192,
    192,
    224,
    224,
    224,
    224,
    255,
    255,
    255,
    255,
    224,
    192,
    128,
    128,
    80,
    80,
    80,
    80,
};

// g
u8 D_80113BF8[24] = {
    80,
    80,
    80,
    80,
    80,
    128,
    192,
    192,
    224,
    224,
    224,
    224,
    255,
    255,
    255,
    255,
    224,
    192,
    128,
    128,
    80,
    80,
    80,
    80,
};

// b
u8 D_80113C10[24] = {
    80,
    80,
    80,
    80,
    80,
    128,
    192,
    192,
    224,
    224,
    224,
    224,
    255,
    255,
    255,
    255,
    224,
    192,
    128,
    128,
    80,
    80,
    80,
    80,
};

// a
u8 D_80113C28[24] = {
    80,
    80,
    80,
    80,
    80,
    128,
    192,
    192,
    224,
    224,
    224,
    224,
    255,
    255,
    255,
    255,
    224,
    192,
    128,
    128,
    80,
    80,
    80,
    80,
};

// unused or padding
u8 D_80113C40[] = {
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0
};

// rodata
static const u8 houseConstructionDays[6];
static const u16 lifeEventHouseConstructionBits[6];
static const u8 animalLocationsHouseConstruction[6];
static const u16 D_8011F25C[79];

static const s16 houseExtensionPrices[6];     
static const s16 houseExtensionLumberCosts[6];

// forward declarations
extern inline void showTextBox(u16 arg0, u16 arg1, u16 arg2, u32 flag, u16 arg4);
extern void func_8005B09C(u8);

//INCLUDE_ASM("asm/nonmatchings/game/game", updateFamilyStates);

void updateFamilyStates(void) {

    if (!checkLifeEventBit(MARRIED)) goto not_married;

    gWifeConceptionCounter += adjustValue(gWifeConceptionCounter, 1, 120);
    
    if (checkLifeEventBit(HAVE_BABY) && gBabyAge < 0xFF) {
        gBabyAge++;
    }

    if (!checkLifeEventBit(HAVE_BABY) && checkLifeEventBit(WIFE_PREGNANT) && gWifePregnancyCounter >= 60) {
         
        toggleLifeEventBit(WIFE_PREGNANT);
        setLifeEventBit(HAVE_BABY);
        setSpecialDialogueBit(4);
        
         switch (gWife) {        

            case MARIA:                              
                setSpecialDialogueBit(0x151);
                break;
            case POPURI:                             
                setSpecialDialogueBit(0x152);
                break;
            case ELLI:                                
                setSpecialDialogueBit(0x153);
                break;
            case ANN:                      
                setSpecialDialogueBit(0x154);
                break;
            case KAREN:                    
                setSpecialDialogueBit(0x155);
                break;
            default:
                break;

        }

     }

    if (!checkLifeEventBit(HAVE_BABY) && checkLifeEventBit(WIFE_PREGNANT)) {
        gWifePregnancyCounter++;
    }

    if (!checkLifeEventBit(HAVE_BABY) && checkLifeEventBit(HAVE_BABY_BED) && !checkLifeEventBit(WIFE_PREGNANT) && npcAffection[gWife] >= 250 && gWifeConceptionCounter >= 30) {
        
        setLifeEventBit(WIFE_PREGNANT);
        
        gWifePregnancyCounter = 0;
        
         switch (gWife) {                        
            case MARIA:                               
                setSpecialDialogueBit(WIFE_PREGNANT_MARIA);
                break;
            case POPURI:                               
                setSpecialDialogueBit(WIFE_PREGNANT_POPURI);
                break;
            case ELLI:                                
                setSpecialDialogueBit(WIFE_PREGNANT_ELLI);
                break;
            case ANN:                             
                setSpecialDialogueBit(WIFE_PREGNANT_ANN);
                break;
            case KAREN:                               
                setSpecialDialogueBit(WIFE_PREGNANT_KAREN);
                break;
            default:
                break;
        }

    }

not_married:

    if (checkLifeEventBit(MARIA_HARRIS_MARRIED)) {

        mariaHarrisNewlywedCounter += adjustValue(mariaHarrisNewlywedCounter, 1, 120);

        if (checkLifeEventBit(MARIA_HARRIS_HAVE_BABY) && mariaHarrisBabyAge < 120) {
            mariaHarrisBabyAge++;
        }

        if (!checkLifeEventBit(MARIA_HARRIS_HAVE_BABY) && checkLifeEventBit(MARIA_PREGNANT) && mariaHarrisPregnancyCounter >= 60) {
            toggleLifeEventBit(MARIA_PREGNANT);
            setLifeEventBit(MARIA_HARRIS_HAVE_BABY);
            setSpecialDialogueBit(MARIA_HARRIS_HAVE_BABY_DIALOGUE);
            toggleSpecialDialogueBit(MARIA_PREGNANT_DIALOGUE);
        }

        if (!checkLifeEventBit(MARIA_HARRIS_HAVE_BABY) && checkLifeEventBit(MARIA_PREGNANT)) {
            mariaHarrisPregnancyCounter++;
        }

        if (!checkLifeEventBit(MARIA_HARRIS_HAVE_BABY) && !checkLifeEventBit(MARIA_PREGNANT) && npcAffection[HARRIS] >= 250 && mariaHarrisNewlywedCounter >= 30) {
            setLifeEventBit(MARIA_PREGNANT);
            mariaHarrisPregnancyCounter = 0;
            setSpecialDialogueBit(MARIA_PREGNANT_DIALOGUE);
        }

    }

     if (checkLifeEventBit(POPURI_GRAY_MARRIED)) {

        popuriGrayNewlywedCounter += adjustValue(popuriGrayNewlywedCounter, 1, 120);

        if (checkLifeEventBit(POPURI_GRAY_HAVE_BABY) && popuriGrayBabyAge < 120) {
            popuriGrayBabyAge++;
        }

        if (!checkLifeEventBit(POPURI_GRAY_HAVE_BABY) && checkLifeEventBit(POPURI_PREGNANT) && popuriGrayPregnancyCounter >= 60) {
            toggleLifeEventBit(POPURI_PREGNANT);
            setLifeEventBit(POPURI_GRAY_HAVE_BABY);
            setSpecialDialogueBit(POPURI_GRAY_HAVE_BABY_DIALOGUE);
            toggleSpecialDialogueBit(0x38);
        }

        if (!checkLifeEventBit(POPURI_GRAY_HAVE_BABY) && checkLifeEventBit(POPURI_PREGNANT)) {
            popuriGrayPregnancyCounter++;
        }

        if (!checkLifeEventBit(POPURI_GRAY_HAVE_BABY) && !checkLifeEventBit(POPURI_PREGNANT) && npcAffection[GRAY] >= 250 && popuriGrayNewlywedCounter >= 30) {
            setLifeEventBit(POPURI_PREGNANT);
            popuriGrayPregnancyCounter = 0;
            setSpecialDialogueBit(0x38);
        }

    }

    if (checkLifeEventBit(ELLI_JEFF_MARRIED)) {

        elliJeffNewlywedCounter += adjustValue(elliJeffNewlywedCounter, 1, 120);

        if (checkLifeEventBit(ELLI_JEFF_HAVE_BABY) && elliJeffBabyAge < 120) {
            elliJeffBabyAge++;
        }

        if (!checkLifeEventBit(ELLI_JEFF_HAVE_BABY) && checkLifeEventBit(ELLI_PREGNANT) && elliJeffPregnancyCounter >= 60) {
            toggleLifeEventBit(ELLI_PREGNANT);
            setLifeEventBit(ELLI_JEFF_HAVE_BABY);
            setSpecialDialogueBit(ELLI_JEFF_HAVE_BABY_DIALOGUE);
            toggleSpecialDialogueBit(ELLI_PREGNANT_DIALOGUE);
        }

        if (!checkLifeEventBit(ELLI_JEFF_HAVE_BABY) && checkLifeEventBit(ELLI_PREGNANT)) {
            elliJeffPregnancyCounter++;
        }

        if (!checkLifeEventBit(ELLI_JEFF_HAVE_BABY) && !checkLifeEventBit(ELLI_PREGNANT) && npcAffection[JEFF] >= 250 && elliJeffNewlywedCounter >= 30) {
            setLifeEventBit(ELLI_PREGNANT);
            elliJeffPregnancyCounter = 0;
            setSpecialDialogueBit(ELLI_PREGNANT_DIALOGUE);
        }

    }

    if (checkLifeEventBit(ANN_CLIFF_MARRIED)) {

        annCliffNewlywedCounter += adjustValue(annCliffNewlywedCounter, 1, 120);

        if (checkLifeEventBit(ANN_CLIFF_HAVE_BABY) && annCliffBabyAge < 120) {
            annCliffBabyAge++;
        }

        if (!checkLifeEventBit(ANN_CLIFF_HAVE_BABY) && checkLifeEventBit(ANN_PREGNANT) && annPregnancyCounter >= 60) {
            toggleLifeEventBit(ANN_PREGNANT);
            setLifeEventBit(ANN_CLIFF_HAVE_BABY);
            setSpecialDialogueBit(ANN_CLIFF_HAVE_BABY_DIALOGUE);
            toggleSpecialDialogueBit(ANN_PREGNANT_DIALOGUE);
        }

        if (!checkLifeEventBit(ANN_CLIFF_HAVE_BABY) && checkLifeEventBit(ANN_PREGNANT)) {
            annPregnancyCounter++;
        }

        if (!checkLifeEventBit(ANN_CLIFF_HAVE_BABY) && !checkLifeEventBit(ANN_PREGNANT) && npcAffection[CLIFF] >= 250 && annCliffNewlywedCounter >= 30) {
            setLifeEventBit(ANN_PREGNANT);
            annPregnancyCounter = 0;
            setSpecialDialogueBit(ANN_PREGNANT_DIALOGUE);
        }

    }

    if (checkLifeEventBit(KAREN_KAI_MARRIED)) {
        
        karenKaiNewlywedCounter += adjustValue(karenKaiNewlywedCounter, 1, 120);

        if (checkLifeEventBit(KAREN_KAI_HAVE_BABY) && karenKaiBabyAge < 120) {
            karenKaiBabyAge++;
        }

        if (!checkLifeEventBit(KAREN_KAI_HAVE_BABY) && checkLifeEventBit(KAREN_PREGNANT) && karenPregnancyCounter >= 60) {
            toggleLifeEventBit(KAREN_PREGNANT);
            setLifeEventBit(KAREN_KAI_HAVE_BABY);
            setSpecialDialogueBit(KAREN_KAI_HAVE_BABY_DIALOGUE);
            toggleSpecialDialogueBit(KAREN_PREGNANT_DIALOGUE);
        }

        if (!checkLifeEventBit(KAREN_KAI_HAVE_BABY) && checkLifeEventBit(KAREN_PREGNANT)) {
            karenPregnancyCounter++;
        }

        if (!checkLifeEventBit(KAREN_KAI_HAVE_BABY) && !checkLifeEventBit(KAREN_PREGNANT) && npcAffection[KAI] >= 250 && karenKaiNewlywedCounter >= 30) {
            setLifeEventBit(0x25);
            karenPregnancyCounter = 0;
            setSpecialDialogueBit(KAREN_PREGNANT_DIALOGUE);
        }

    }

}

//INCLUDE_ASM("asm/nonmatchings/game/game", func_8005A60C);

void func_8005A60C(void) {

    if (!checkLifeEventBit(MARRIED)) goto handle_animals;

    switch (gWife) {     
        case MARIA:
            npcAffection[MARIA] += adjustValue(npcAffection[MARIA], -1, 0xFF);
            break;
        case POPURI:
            npcAffection[POPURI] += adjustValue(npcAffection[POPURI], -1, 0xFF);
            break;
        case ELLI:
            npcAffection[ELLI] += adjustValue(npcAffection[ELLI], -1, 0xFF);
            break;
        case ANN:
            npcAffection[ANN] += adjustValue(npcAffection[ANN], -1, 0xFF);
            break;
        case KAREN:
            npcAffection[KAREN] += adjustValue(npcAffection[KAREN], -1, 0xFF);
            break;
        default:
            break;
        }
    
    if (checkLifeEventBit(HAVE_BABY)) {
        npcAffection[BABY] += adjustValue(npcAffection[BABY], -1, 0xFF);
    }

handle_animals:
    adjustDogAffection(-1);
    adjustHorseAffection(-1);

}

// func_8005A708
//INCLUDE_ASM("asm/nonmatchings/game/game", setSpecialDialogues);

void setSpecialDialogues(void) {

    // have at least 1 house extensions
    if (func_8009B3DC()) {
        setSpecialDialogueBit(HAVE_HOUSE_EXTENSION);
    } else {
        toggleSpecialDialogueBit(HAVE_HOUSE_EXTENSION);
    }

    // cow and sheep status
    if (func_8009B4EC()) {
        setSpecialDialogueBit(0x28);
    } else {
        toggleSpecialDialogueBit(0x28);
    }

    // chicken status
    if (func_8009B564()) {
        setSpecialDialogueBit(0x2C);
    } else {
        toggleSpecialDialogueBit(0x2C);
    }

    if (checkLifeEventBit(HAVE_HORSE)) {
        setSpecialDialogueBit(HAVE_HORSE_DIALOGUE);
    }

    if (gForecast == RAIN) {
        setSpecialDialogueBit(RAIN_FORECAST_DIALOGUE);
    } else {
        toggleSpecialDialogueBit(RAIN_FORECAST_DIALOGUE);
    }

    if (checkLifeEventBit(HAVE_KITCHEN)) {
        setSpecialDialogueBit(0x18);
    }

    if (checkLifeEventBit(HAVE_BATHROOM)) {
        setSpecialDialogueBit(0x19);
    }

    if (checkLifeEventBit(HAVE_STAIRS)) {
        setSpecialDialogueBit(0x1A);
    }

    if (checkLifeEventBit(HAVE_GREENHOUSE)) {
        setSpecialDialogueBit(0x1B);
    }

    // spirit festival
    if (gSeason == WINTER && gDayOfMonth == 27) {

        if (gHarvestKing == PLAYER) {
            setSpecialDialogueBit(PLAYER_HARVEST_KING_SPIRIT_FESTIVAL_ASSISTANTS);
        }

        toggleSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_MARIA);
        toggleSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_POPURI);
        toggleSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_ELLI);
        toggleSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_ANN);
        toggleSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_KAREN);
        toggleSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_HARRIS);
        toggleSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_GRAY);
        toggleSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_JEFF);
        toggleSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_CLIFF);
        toggleSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_KAI);

        numberOfSpiritFestivalAssistantsRecruited = 0;
        spiritFestivalAssistant1 = 0;
        spiritFestivalAssistant2 = 0;
        spiritFestivalAssistant3 = 0;

    } else {
        toggleSpecialDialogueBit(PLAYER_HARVEST_KING_SPIRIT_FESTIVAL_ASSISTANTS);
    }

    if (gSeason == WINTER && gDayOfMonth == 10) {
        setSpecialDialogueBit(THANKSGIVING);
    } else {
        toggleSpecialDialogueBit(THANKSGIVING);
    }

    if (gSeason == WINTER && gDayOfMonth == 30) {
        setSpecialDialogueBit(NEW_YEARS_EVE);
    } else {
        toggleSpecialDialogueBit(NEW_YEARS_EVE);
    }

    if (gSeason == WINTER && gDayOfMonth == 24) {
        setSpecialDialogueBit(STARRY_NIGHT_FESTIVAL_DIALOGUE_BIT);
    } else {
        toggleSpecialDialogueBit(STARRY_NIGHT_FESTIVAL_DIALOGUE_BIT);
    }

    if (gSeason == WINTER && gDayOfMonth == 11) {
        setSpecialDialogueBit(MARIA_BIRTHDAY);
    } else {
        toggleSpecialDialogueBit(MARIA_BIRTHDAY);
    }

    if (gSeason == SPRING && gDayOfMonth == 22) {
        setSpecialDialogueBit(POPURI_BIRTHDAY);
    } else {
        toggleSpecialDialogueBit(POPURI_BIRTHDAY);
    }

    if (gSeason == AUTUMN && gDayOfMonth == 1) {
        setSpecialDialogueBit(ELLI_BIRTHDAY);
    } else {
        toggleSpecialDialogueBit(ELLI_BIRTHDAY);
    }

    if (gSeason == SUMMER && gDayOfMonth == 14) {
        setSpecialDialogueBit(ANN_BIRTHDAY);
    } else {
        toggleSpecialDialogueBit(ANN_BIRTHDAY);
    }

    if (gSeason == WINTER && gDayOfMonth == 29) {
        setSpecialDialogueBit(KAREN_BIRTHDAY);
    }
    else {
        toggleSpecialDialogueBit(KAREN_BIRTHDAY);
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/game", func_8005AAE4);

void func_8005AAE4(void) {

    toggleSpecialDialogueBit(0x40);
    toggleSpecialDialogueBit(0x46);
    toggleSpecialDialogueBit(0x91);
    toggleSpecialDialogueBit(0x92);
    toggleSpecialDialogueBit(0x93);
    toggleSpecialDialogueBit(180);
    toggleSpecialDialogueBit(0x96);
    toggleSpecialDialogueBit(0x97);
    toggleSpecialDialogueBit(0x98);
    toggleSpecialDialogueBit(0x99);
    toggleSpecialDialogueBit(0x9E);
    toggleSpecialDialogueBit(0x9F);
    toggleSpecialDialogueBit(160);
    toggleSpecialDialogueBit(0xA1);
    toggleSpecialDialogueBit(0xA9);
    toggleSpecialDialogueBit(0xAA);
    toggleSpecialDialogueBit(0xAB);
    toggleSpecialDialogueBit(0xB1);
    toggleSpecialDialogueBit(0x131);
    toggleSpecialDialogueBit(0x132);
    toggleSpecialDialogueBit(0x133);
    toggleSpecialDialogueBit(0x134);
    toggleSpecialDialogueBit(0x136);
    toggleSpecialDialogueBit(0x137);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/game", resetDailyBits);

void resetDailyBits(void) {
    
    u16 i;

    for (i = 0; i < 32; i++) {
        dailyEventBits[i] = 0;
    }
    
    toggleSpecialDialogueBit(1);
    toggleSpecialDialogueBit(0xE1);
    toggleSpecialDialogueBit(0xE2);
    toggleSpecialDialogueBit(0xE3);
    toggleSpecialDialogueBit(0xE4);
    toggleSpecialDialogueBit(0xE5);
    toggleSpecialDialogueBit(0xE6);
    toggleSpecialDialogueBit(0xE7);
    toggleSpecialDialogueBit(0xE8);
    toggleSpecialDialogueBit(0xE9);
    toggleSpecialDialogueBit(0xEA);
    toggleSpecialDialogueBit(0xEB);
    toggleSpecialDialogueBit(0xEC);
    toggleSpecialDialogueBit(0xED);
    toggleSpecialDialogueBit(0xEE);
    toggleSpecialDialogueBit(0xEF);
    toggleSpecialDialogueBit(0xF0);
    toggleSpecialDialogueBit(0xF1);
    toggleSpecialDialogueBit(0xF2);
    toggleSpecialDialogueBit(0xF3);
    toggleSpecialDialogueBit(0xF4);
    toggleSpecialDialogueBit(0xF5);
    toggleSpecialDialogueBit(0xF6);
    toggleSpecialDialogueBit(0xF7);
    toggleSpecialDialogueBit(0xF8);
    toggleSpecialDialogueBit(0xF9);
    toggleSpecialDialogueBit(0xFA);
    toggleSpecialDialogueBit(0xFB);
    toggleSpecialDialogueBit(0xFC);
    toggleSpecialDialogueBit(0xFD);
    toggleSpecialDialogueBit(0xFE);
    toggleSpecialDialogueBit(0xFF);
    toggleSpecialDialogueBit(0x100);
    toggleSpecialDialogueBit(0x101);
    toggleSpecialDialogueBit(0x102);
    toggleSpecialDialogueBit(0x103);
    toggleSpecialDialogueBit(0x104);
    toggleSpecialDialogueBit(0x105);
    toggleSpecialDialogueBit(2);
    toggleSpecialDialogueBit(0x106);
    toggleSpecialDialogueBit(0x107);
    toggleSpecialDialogueBit(0x108);
    toggleSpecialDialogueBit(0x109);
    toggleSpecialDialogueBit(0x10A);
    toggleSpecialDialogueBit(0x10B);
    toggleSpecialDialogueBit(0x10C);
    toggleSpecialDialogueBit(0x10D);
    toggleSpecialDialogueBit(0x10E);
    toggleSpecialDialogueBit(0x10F);
    toggleSpecialDialogueBit(0x110);
    toggleSpecialDialogueBit(0x111);
    toggleSpecialDialogueBit(0x112);
    toggleSpecialDialogueBit(0x113);
    toggleSpecialDialogueBit(0x114);
    toggleSpecialDialogueBit(0x115);
    toggleSpecialDialogueBit(0x116);
    toggleSpecialDialogueBit(0x117);
    toggleSpecialDialogueBit(0x118);
    toggleSpecialDialogueBit(0x119);
    toggleSpecialDialogueBit(0x11A);
    toggleSpecialDialogueBit(0x11B);
    toggleSpecialDialogueBit(0x11C);
    toggleSpecialDialogueBit(0x11D);
    toggleSpecialDialogueBit(0x11E);
    toggleSpecialDialogueBit(0x11F);
    toggleSpecialDialogueBit(0x120);
    toggleSpecialDialogueBit(0x121);
    toggleSpecialDialogueBit(0x122);
    toggleSpecialDialogueBit(0x123);
    toggleSpecialDialogueBit(0x124);
    toggleSpecialDialogueBit(0x125);
    toggleSpecialDialogueBit(0x126);
    toggleSpecialDialogueBit(0x127);
    toggleSpecialDialogueBit(0x128);
    toggleSpecialDialogueBit(0x129);
    toggleSpecialDialogueBit(0x12A);

}

//INCLUDE_ASM("asm/nonmatchings/game/game", adjustValue);

// same as func_80046D50
inline int adjustValue(int initial, int value, int max) {

    int temp;
    int adjusted;

    adjusted = value;
    temp = initial + adjusted;
    
    if (max < temp) {
        adjusted -= temp - max;
        temp = max;
    }
    
    if (temp < 0) {
        adjusted -= temp;
    } 
      
    return adjusted;
    
}


// from transition.c

static inline func_80059334_2(void) {
    pauseEntities();
    pauseAllCutsceneExecutors();
    setEntityMapSpaceIndependent(ENTITY_PLAYER, FALSE);
}

static inline func_800593EC_2(void) {
    func_8002FB3C();
    pauseAllCutsceneExecutors();
    setEntityMapSpaceIndependent(ENTITY_PLAYER, FALSE);
    func_8003C504(MAIN_MAP_INDEX);
}


//INCLUDE_ASM("asm/nonmatchings/game/game", showTextBox);

inline void showTextBox(u16 messageBoxType, u16 dialogueInfoIndex, u16 dialogueIndex, u32 flag, u16 flags) {
  
    func_80059334_2();
    
    switch (messageBoxType) {
        
        case 0:
          setMessageBoxViewSpacePosition(0, 24.0f, -64.0f, 352.0f);
          setMessageBoxSpriteIndices(0, 0, 0, 0);
          break;
        
        case 1:
          setMessageBoxViewSpacePosition(0, 0, -64.0f, 352.0f);
          setMessageBoxSpriteIndices(0, 1, 0, 0);
          break;
        
        default:
          break;
    }

    func_8003F360(0, -4, flags);
  
    initializeMessageBox(MAIN_DIALOGUE_BOX_INDEX, dialogueInfoIndex, dialogueIndex, flag);
  
    setMainLoopCallbackFunctionIndex(TEXT);
    
    controllers[CONTROLLER_1].buttonPressed = 0;

    setPlayerAction(0, 0);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/game", showMessageBox);

// show dialogue box
inline void showMessageBox(u16 arg0, u16 dialogueBytecodeAddressesIndex, u16 dialogueIndex, u32 flag, u16 messageBoxFlags) {
    
    func_80059334_2();
    
    switch (arg0) {
        case 0:
            setMessageBoxViewSpacePosition(0, 24.0f, -64.0f, 352.0f);
            setMessageBoxSpriteIndices(0, 0, 0, 0);
            break;
        case 1:
            setMessageBoxViewSpacePosition(0, 0, -64.0f, 352.0f);
            setMessageBoxSpriteIndices(0, 1, 0, 0);
            break;
        default:
            break;
    }

    func_8003F360(0, -4, messageBoxFlags);
    initializeDialogueSession(0, dialogueBytecodeAddressesIndex, dialogueIndex, flag);

    setMainLoopCallbackFunctionIndex(DIALOGUE);

    controllers[CONTROLLER_1].buttonPressed = 0;
    
    setPlayerAction(0, 0);

}

//INCLUDE_ASM("asm/nonmatchings/game/game", func_8005B09C);

// show pink overlay texts basd on level interaction/animals/cutscene
void func_8005B09C(u8 arg0) {

    gameLoopContext.unk_6 = arg0;

    switch (arg0) {
        default:
            break;
        case 0:
            showMessageBox(1, 1, 0, 0, 2);
            break;
        case 1:
            showMessageBox(1, 1, 1, 0, 2);
            break;    
        case 2:
            showMessageBox(0, 0x3D, 0xC, 0x40, 0);
            break;         
        case 3:
            showMessageBox(0, 0x3D, 0xB, 0x40, 0);
            break;          
        case 4:
            showMessageBox(0, 0x3D, 0x12, 0x40, 0);
            break;       
        case 5:
            showMessageBox(0, 0x3D, 0xD, 0x40, 0);
            break;        
        case 6:
            showMessageBox(0, 0x3D, 0xE, 0x40, 0);
            break;      
        case 7:
            showMessageBox(0, 0x3D, 0xF, 0x40, 0);
            break;      
        case 8:
            showMessageBox(0, 0x3E, 7, 0x40, 0);
            break;      
        case 9:
            showMessageBox(0, 0x3E, 6, 0x40, 0);
            break;    
        case 10:
            showMessageBox(0, 0x3E, 2, 0x40, 0);
            break;  
        case 11:
            showMessageBox(0, 0x3E, 0, 0x40, 0);
            break;        
        case 12:
            showMessageBox(0, 0x3E, 1, 0x40, 0);
            break;          
        case 13:
            if (gSeason == SPRING || gSeason == WINTER) {
                showMessageBox(1, 0x44, 0, 0, 2);
            } else {
                showMessageBox(1, 0x44, 10, 0, 2);
            }            
            break;
        case 14:
            if (gSeason == SPRING || gSeason == WINTER) {
                showMessageBox(1, 0x44, 1, 0x40, 2);
            } else {
                showMessageBox(1, 0x44, 0xB, 0, 2);
            }
            break;
        case 15:
            if (gSeason == SPRING || gSeason == WINTER) {
                showMessageBox(1, 0x44, 2, 0x40, 2);
            } else {
                showMessageBox(1, 0x44, 0xC, 0, 2);
            }
            break; 
        case 16:
            if (gSeason == SPRING || gSeason == WINTER) {
                showMessageBox(1, 0x44, 3, 0x40, 2);
            } else {
                showMessageBox(1, 0x44, 0xD, 0, 2);
            }
            break;   
        case 17:
            if (gSeason == SPRING || gSeason == WINTER) {
                showMessageBox(1, 0x44, 4, 0x40, 2);
            } else {
                showMessageBox(1, 0x44, 0xE, 0, 2);
            }
            break;     
        case 18:
            if (gSeason == SPRING || gSeason == WINTER) {
                showMessageBox(1, 0x44, 5, 0x40, 2);
            } else {
                showMessageBox(1, 0x44, 0xF, 0, 2);
            }
            break;        
        case 19:
            if (gSeason == SPRING || gSeason == WINTER) {
                showMessageBox(1, 0x44, 6, 0x40, 2);
            } else {
                showMessageBox(1, 0x44, 0x10, 0, 2);
            }
            break;        
        case 20:
            if (gSeason == SPRING || gSeason == WINTER) {
                showMessageBox(1, 0x44, 7, 0x40, 2);
            } else {
                showMessageBox(1, 0x44, 0x11, 0, 2);
            }
            break;       
        case 21:
            if (gSeason == SPRING || gSeason == WINTER) {
                showMessageBox(1, 0x44, 8, 0x40, 2);
            } else {
                showMessageBox(1, 0x44, 0x12, 0, 2);
            }
            break;       
        case 22:
            if (gSeason == SPRING || gSeason == WINTER) {
                showMessageBox(1, 0x44, 9, 0x40, 2);
            } else {
                showMessageBox(1, 0x44, 0x13, 0, 2);
            }
            break;     
        case 23:
            showMessageBox(1, 1, 0x15, 0, 2);
            break;    
    }

    setMainLoopCallbackFunctionIndex(PINK_OVERLAY_TEXT);

}

//INCLUDE_ASM("asm/nonmatchings/game/game", func_8005C00C);

void func_8005C00C(void) {
    
    if (gCutsceneCompletionFlags < 0) {
        setMainLoopCallbackFunctionIndex(MAIN_GAME);
        return;
    }
    
    if (!checkDailyEventBit(0x4B)) {
        setLevelAudio(gBaseMapIndex, gSeason, gHour);
        //setLevelAudio(currentMapContext.currentMapIndex, gSeason, gHour);
    }
     
    // set lighting for level based on weather
    func_8005C07C(8, 1);

}

//INCLUDE_ASM("asm/nonmatchings/game/game", func_8005C07C);

void func_8005C07C(s16 arg0, u16 arg1) {

    Vec4f vec;
    
    // 4 = bit 3 = outdoors flag
    if (getLevelFlags(gBaseMapIndex) & 4) {
        
        vec = setWeatherLighting(gWeather);

        switch (gWeather) {
            case SUNNY:
            case 4:
            case TYPHOON:
                globalLightingRGBA.r = D_80113B20[gHour];
                globalLightingRGBA.g = D_80113B38[gHour];
                globalLightingRGBA.b = D_80113B50[gHour];
                globalLightingRGBA.a = D_80113B68[gHour];
                break;
            case RAIN:
                globalLightingRGBA.r = D_80113B80[gHour];
                globalLightingRGBA.g = D_80113B98[gHour];
                globalLightingRGBA.b = D_80113BB0[gHour];
                globalLightingRGBA.a = D_80113BC8[gHour];  
                break;
            case SNOW:
                globalLightingRGBA.r = D_80113BE0[gHour];
                globalLightingRGBA.g = D_80113BF8[gHour];
                globalLightingRGBA.b = D_80113C10[gHour];
                globalLightingRGBA.a = D_80113C28[gHour];  
                break;
            default:
                break;
        }
        
    } else {
        func_8006EB94(&globalLightingRGBA, gBaseMapIndex);
    }
    
    if (arg0 == 0) {
        
        setEntitiesColor(globalLightingRGBA.r, globalLightingRGBA.g, globalLightingRGBA.b, globalLightingRGBA.a);
        func_8003BE98(MAIN_MAP_INDEX, globalLightingRGBA.r, globalLightingRGBA.g, globalLightingRGBA.b, globalLightingRGBA.a);
        
    } else if (globalLightingRGBA.r != unknownRGBA.r || globalLightingRGBA.g != unknownRGBA.g || globalLightingRGBA.b != unknownRGBA.b || globalLightingRGBA.a != unknownRGBA.a) {
        
        updateEntitiesColor(globalLightingRGBA.r, globalLightingRGBA.g, globalLightingRGBA.b, globalLightingRGBA.a, arg0);
        func_8003BF7C(0, globalLightingRGBA.r, globalLightingRGBA.g, globalLightingRGBA.b, globalLightingRGBA.a, arg0);
        
        if (!checkDailyEventBit(0x4B)) {
            setAudioSequenceVolume(gCurrentAudioSequenceIndex, gAudioSequenceVolume);
        }

    }

    unknownRGBA.r = globalLightingRGBA.r;
    unknownRGBA.g = globalLightingRGBA.g;
    unknownRGBA.b = globalLightingRGBA.b;
    unknownRGBA.a = globalLightingRGBA.a;

    gameLoopContext.callbackIndex = arg1;
    
    if (arg1) {
        setMainLoopCallbackFunctionIndex(LEVEL_LOAD_1);
        togglePauseEntities();
        func_80046CF4();
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/game", func_8005C940);

// arg0 = unused
inline void func_8005C940(u16 arg0, u16 callbackIndex) {
    
    func_8003BF7C(MAIN_MAP_INDEX, 0, 0, 0, 0, 8);
    updateEntitiesColor(0, 0, 0, 0, 8);
       
    stopAudioSequenceWithDefaultFadeOut(gCurrentAudioSequenceIndex);
    
    gameLoopContext.callbackIndex = callbackIndex;

    resetGlobalLighting();
    
    if (gameLoopContext.callbackIndex) {
        setMainLoopCallbackFunctionIndex(LEVEL_LOAD_2);
        func_80059334_2();
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/game", func_8005CA2C);

void func_8005CA2C(u16 arg0, u16 arg1) {
    gameLoopContext.unk_4 = arg0;
    gameLoopContext.unk_2 = 0;
    gameLoopContext.callbackIndex = arg1;
    setMainLoopCallbackFunctionIndex(OVERLAY_SCREEN_LOAD);
}

//INCLUDE_ASM("asm/nonmatchings/game/game", func_8005CA64);

void func_8005CA64(void) {

    func_800A8F74();

    if (checkMapRGBADone(MAIN_MAP_INDEX)) {
        setMainLoopCallbackFunctionIndex(gameLoopContext.callbackIndex);
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/game", func_8005CAA8);

void func_8005CAA8(void) {

    func_800A8F74();

    if (checkMapRGBADone(MAIN_MAP_INDEX) && checkDefaultSequenceChannelOpen(gCurrentAudioSequenceIndex)) {

        setEntitiesColor(0, 0, 0, 0);
        // map rgba
        func_8003BE98(MAIN_MAP_INDEX, 0, 0, 0, 0);
        // reset cutscene structs
        deactivateCutsceneExecutors();
        initializeCutsceneExecutors();

        gCutsceneCompletionFlags = 0;

        gCutsceneFlags = 0;

        setMainLoopCallbackFunctionIndex(gameLoopContext.callbackIndex);

    }

}

//INCLUDE_ASM("asm/nonmatchings/game/game", func_8005CB50);

void func_8005CB50(void) {

    if (gameLoopContext.unk_2 >= gameLoopContext.unk_4) {
        setMainLoopCallbackFunctionIndex(gameLoopContext.callbackIndex);
    } else {
        gameLoopContext.unk_2++;
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/game", func_8005CBA4);

void func_8005CBA4(void) {

    if (!(mapControllers[MAIN_MAP_INDEX].flags & (0x8 | 0x10))) {
        togglePauseEntities();
        func_80046CF4();
        setEntityMapSpaceIndependent(ENTITY_PLAYER, TRUE);
        setMainLoopCallbackFunctionIndex(MAIN_GAME);
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/game", func_8005CBF0);

// main loop callback for finishing dialogue
void func_8005CBF0(void) {
    
    // check flag on dialogue box struct
    if (func_80043A88()) {
        
        if (!(getItemFlags(gPlayer.heldItem) & (0x80 | 0x100 | 0x400 | 0x800))) {
            func_800D55E4(gPlayer.itemInfoIndex, 1);
            gPlayer.heldItem = 0;
            gItemBeingHeld = 0xFF;
        }
         
        func_8003F910(0, 0x78, &_dialogueIconsTextureSegmentRomStart, &_dialogueIconsTextureSegmentRomEnd, &_dialogueIconsAssetsIndexSegmentRomStart, &_dialogueIconsAssetsIndexSegmentRomEnd, (void*)DIALOGUE_ICONS_TEXTURE_VADDR_START, (void*)DIALOGUE_ICONS_TEXTURE_VADDR_END, (void*)DIALOGUE_ICONS_ASSETS_INDEX_VADDR_START, (void*)DIALOGUE_ICONS_ASSETS_INDEX_VADDR_END, 0, 4, 0xFE, 106.0f, -15.0f, 0.0f);
        func_8003F910(1, 0x78, &_dialogueIconsTextureSegmentRomStart, &_dialogueIconsTextureSegmentRomEnd, &_dialogueIconsAssetsIndexSegmentRomStart, &_dialogueIconsAssetsIndexSegmentRomEnd, (void*)DIALOGUE_ICONS_TEXTURE_VADDR_START, (void*)DIALOGUE_ICONS_TEXTURE_VADDR_END, (void*)DIALOGUE_ICONS_ASSETS_INDEX_VADDR_START, (void*)DIALOGUE_ICONS_ASSETS_INDEX_VADDR_END, 0, 0xD, 0xFE, 106.0f, -15.0f, 0.0f);
       
        // update stuff after closing dialogue 
        func_8005CDCC();
        
        setMainLoopCallbackFunctionIndex(MAIN_GAME);
        
        togglePauseEntities();
        func_80046CF4();
        setEntityMapSpaceIndependent(ENTITY_PLAYER, TRUE);

    }

}

//INCLUDE_ASM("asm/nonmatchings/game/game", func_8005CDCC);

// update stuff after closing dialogue
void func_8005CDCC(void) {
    
    npcAffection[HARVEST_SPRITE_2] = npcAffection[HARVEST_SPRITE_1];
    npcAffection[HARVEST_SPRITE_3] = npcAffection[HARVEST_SPRITE_1];
    
    toggleSpecialDialogueBit(0x135);

    recruitSpiritFestivalAssistants();

    setRecipes();
    
    if (checkSpecialDialogueBit(0x22) && !checkHaveKeyItem(DOOR_TO_HEAVEN)) {
        acquireKeyItem(DOOR_TO_HEAVEN);
    }
    
    if (checkSpecialDialogueBit(0xB0)) {
        if (!checkHaveKeyItem(FIXED_MUSIC_BOX)) {
            removeKeyItem(BROKEN_MUSIC_BOX);
            acquireKeyItem(FIXED_MUSIC_BOX);
        }
    } else {
        removeKeyItem(FIXED_MUSIC_BOX);
    }
    
    if (!checkSpecialDialogueBit(0x15)) {
        removeKeyItem(WEATHER_VANE);
    }
    
    if (!checkSpecialDialogueBit(0x17)) {
        removeKeyItem(GOLD_PENDANT);
    }
    
    if (checkSpecialDialogueBit(0x14)) {
        setLifeEventBit(9);
    }
    
    if (checkSpecialDialogueBit(0x20)) {
        setLifeEventBit(GAVE_BLUE_ROCK_TO_SAIBARA);
    }

    if (checkSpecialDialogueBit(0x84)) {
        setLifeEventBit(GIVE_RICK_RARE_METAL);
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/game", func_8005CEFC);

void func_8005CEFC(void) {
    
    // check if a message box has flag 4 set
    if (func_8003F0DC()) {

        setMainLoopCallbackFunctionIndex(MAIN_GAME);

        togglePauseEntities();
        func_80046CF4();
        setEntityMapSpaceIndependent(ENTITY_PLAYER, TRUE);

    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/game", func_8005CF4C);

// unused main loop callback functions
// ends cutscene
void func_8005CF4C(void) {

    // unused variable
    if (D_801FB686 == 0) {

        setMainLoopCallbackFunctionIndex(MAIN_GAME);
        togglePauseEntities();
        // cutscene executor flags
        func_80046CF4();
        setEntityMapSpaceIndependent(ENTITY_PLAYER, TRUE);

    }

}

//INCLUDE_ASM("asm/nonmatchings/game/game", func_8005CF94);

void func_8005CF94(void) {
    
    u8* namePtr;

    func_800593EC_2();
    
    // naming screen index
    switch (gNamingScreenIndex) {
        case 0:
            namePtr = gPlayer.name;
            break;
        case 1:
            namePtr = &gFarmName;
            break;
        case 2:
            namePtr = dogInfo.name;
            break;
        case 3:
            namePtr = horseInfo.name; 
            break;
        case 4:
            namePtr = &gBabyName;
            break;
        case 5:
            namePtr = gFarmAnimals[D_801FC155].name;
            break;
        case 6:
            namePtr = gFarmAnimals[D_801FC155].name;
            break;
        case 7:
            namePtr = gChickens[D_801FC155].name;
            break;
        }
    
    initializeNamingScreen(namePtr, gNamingScreenIndex);

}

//INCLUDE_ASM("asm/nonmatchings/game/game", func_8005D0BC);

// map load callback
void func_8005D0BC(void) {
    
    bool set;
    int checkOverflow;
    s16 maxHappiness;
    u8 happinessIncrease;
    
    toggleDailyEventBit(6);
    toggleDailyEventBit(0x12);
    toggleDailyEventBit(0x13);

    setPlayerAction(0, 0);

    func_80056030(1);
    
    set = FALSE;
    
    // chicken born
    if (gBaseMapIndex == COOP && checkLifeEventBit(3)) {
        
        gNamingScreenIndex = 7;
        D_801FC155 = bornChickenIndex;
        gChickens[bornChickenIndex].flags &= ~0x100;
        
        setMainLoopCallbackFunctionIndex(NAMING_SCREEN);
        toggleLifeEventBit(3);
        setLifeEventBit(0x38);
        
        set = TRUE;
        
        // FIXME: should be inline adjustValue call

        checkOverflow = gHappiness;
        checkOverflow += 2;
        maxHappiness = 0xFF;
        happinessIncrease = 2;
        
        if (checkOverflow > maxHappiness) {
          happinessIncrease -= (checkOverflow - maxHappiness);
          checkOverflow = maxHappiness;
        }
        
        if (checkOverflow < 0) {
          happinessIncrease -= checkOverflow;
        }
        
        gHappiness += happinessIncrease;

        //

    }
    
    // farm animal
    if (gBaseMapIndex == BARN && checkLifeEventBit(4)) {
        
        gNamingScreenIndex = 5;
        D_801FC155 = bornAnimalIndex;
        gFarmAnimals[bornAnimalIndex].flags &= ~0x800;
            
        setMainLoopCallbackFunctionIndex(NAMING_SCREEN);
        toggleLifeEventBit(4);
            
        set = TRUE;
        
        // FIXME: should be inline adjustValue call

        checkOverflow = gHappiness;
        checkOverflow += 20;
        maxHappiness = 0xFF;

        happinessIncrease = 20;
            
        if (checkOverflow > maxHappiness) {
          happinessIncrease -= (checkOverflow - maxHappiness);
          checkOverflow = maxHappiness;
        }
            
        if (checkOverflow < 0) {
          happinessIncrease -= checkOverflow;
        }
            
        gHappiness += happinessIncrease;

        //
    
    }
    
    if (gHour >= 18) {
        setDailyEventBit(0xF);
        setDailyEventBit(0x10);
    }
    
    if (!set) {
        setMainLoopCallbackFunctionIndex(4);
    }

}

// FIXME: clean up/rename inlines

static inline void inline1(u8 arg0, u8 arg1, u8 arg2, u8 arg3, u8 arg4) {

    switch (arg0) {                   
        case 0:                                 
            func_800DC9FC(arg1);
            setMainLoopCallbackFunctionIndex(MAIN_GAME);
            break;
        case 1:                                 
            func_800DC9FC(arg2);
            setMainLoopCallbackFunctionIndex(MAIN_GAME);
            break;
        case 2:                                 
            func_800DC9FC(arg3);
            setMainLoopCallbackFunctionIndex(MAIN_GAME);
            break;
        case 3:                                 
            func_800DC9FC(arg4);
            setMainLoopCallbackFunctionIndex(MAIN_GAME);
            break;
    }

}

static inline void inline2(u8 arg0, u8 arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5) {
    switch (arg0) {                    
        case 0:                                 
            func_800DC9FC(arg1);
            setMainLoopCallbackFunctionIndex(MAIN_GAME);
            break;
        case 1:                                 
            func_800DC9FC(arg2);
            setMainLoopCallbackFunctionIndex(MAIN_GAME);
            break;
        case 2:                                 
            func_800DC9FC(arg3);
            setMainLoopCallbackFunctionIndex(MAIN_GAME);
            break;
        case 3:                                 
            func_800DC9FC(arg4);
            setMainLoopCallbackFunctionIndex(MAIN_GAME);
            break;
        case 4:                                 
            func_800DC9FC(arg5);
            setMainLoopCallbackFunctionIndex(MAIN_GAME);
            break;
    }
}

static inline void inline3() {           

    dialogues[0].sessionManager.flags &= ~0x40;
    
    func_80043AD8(0);

    setPlayerAction(0, 0);
    setMainLoopCallbackFunctionIndex(MAIN_GAME);

}

// from transition.c
static inline void func_80055F08_2(u16 cutsceneIndex, u16 entranceIndex, u8 arg2) {
    
    deactivateSprites();
    
    deactivateGlobalSprites();
    
    initializeMessageBoxes();
    initializeCutsceneExecutors();
    func_80053088();
    
    resetGlobalLighting();

    setEntitiesColor(0, 0, 0, 0);
    func_8003BE98(MAIN_MAP_INDEX, 0, 0, 0, 0);

    gHour = 12;

    // set ptrs to rom addresses for sprites
    initializeEntityInstances(arg2);

    setEntrance(entranceIndex);

    gCutsceneIndex = cutsceneIndex;

    // trigger cutscene and load cutscene assets
    loadCutscene(0);
    
    func_8006E840(gEntranceIndex);
    setupPlayerEntity(gEntranceIndex, 0);

    handlePlayerAnimation();

    setMainLoopCallbackFunctionIndex(4);

}

// possible split

//INCLUDE_ASM("asm/nonmatchings/game/game", func_8005D2B0);

// pink overlay main loop callback
void func_8005D2B0() {

    u8 temp;

    if (dialogues[0].sessionManager.flags & 4) {
        
        togglePauseEntities();
        func_80046CF4();
        setEntityMapSpaceIndependent(ENTITY_PLAYER, TRUE);

        temp = func_80043C6C(0);

        switch (gameLoopContext.unk_6) {

            case 0:                                     
                switch (temp) {                      
                    case 0:
                        // show another message box                 
                        func_8005B09C(1);
                        break;
                    case 1:                                 
                        showMessageBox(1, 1, 2, 0x80, 2);
                        break;
                    case 2:                                 
                        setMainLoopCallbackFunctionIndex(0x19);
                        break;
                    case 3:                                 
                        setMainLoopCallbackFunctionIndex(0x1D);
                        break;
                    case 4:                                 
                        setMainLoopCallbackFunctionIndex(MAIN_GAME);
                        break;
                    }
                
                break;
            
            // diary
            case 1:
                switch (temp) {
                     
                    // write in diary/save game
                    case 0:                                 
                        setMainLoopCallbackFunctionIndex(MAIN_GAME);
                        setPlayerAction(8, 10);
                        setDailyEventBit(7);
                        break;
                        
                    // don't write
                    case 1:                                 
                        setMainLoopCallbackFunctionIndex(MAIN_GAME);
                        setPlayerAction(8, 10);
                        break;

                    case 2:
                        setMainLoopCallbackFunctionIndex(MAIN_GAME);
                        break;
                }
                
                break;

            case 2:                       
                inline1(temp, 11, 12, 13, 18);
                break;

            case 3:
                inline1(temp, 14, 15, 16, 17);
                break;

            case 4:       
                inline2(temp, 40, 41, 42, 43, 44);
                break;

            case 5:                                     
                switch (temp) {                    
                    case 0:
                        // show another message box               
                        func_8005B09C(6);
                        break;
                    case 1:                                
                        showTextBox(0, 6, 0x55, 0, 0);
                        setDailyEventBit(2);
                        setDailyEventBit(5);
                        break;
                    case 2:                                
                        func_800DC9FC(0x21);
                        setMainLoopCallbackFunctionIndex(MAIN_GAME);
                        break;
                    }
                
                break;

            case 6:                                
                inline1(temp, 30, 31, 32, 33);
                break;

            case 7: 

                switch (temp) {                   

                    case 0:                         

                        gGold += adjustValue(gGold, D_801890E0, MAX_GOLD);
                        
                        D_801890E0 = 0;                        
                        dialogues[0].sessionManager.flags &= ~0x40;
                        
                        func_80043AD8(0);
                        setEntrance(9);

                        func_8005C940(0, 2);

                        toggleDailyEventBit(2);
                        setDailyEventBit(4);

                        switch (D_801C4216) {
                            case 4:
                                initializeChicken(D_801FC155);
                                break;
                            case 2 ... 3:
                                initializeFarmAnimal(D_801FC155);
                                break;
                        }
                        
                        break;
                    
                    case 1:                                 
                        dialogues[0].sessionManager.flags &= ~0x40;
                        func_80043AD8(0);
                        setMainLoopCallbackFunctionIndex(MAIN_GAME);
                        
                        break;

                    }
                
                break;

            case 8:                         
                switch (temp) {                    
                    case 0:                                 
                        
                        dialogues[0].sessionManager.flags &= ~0x40;
                        func_80043AD8(0);
                        setEntrance(9);

                        func_8005C940(0, 2);

                        toggleDailyEventBit(0x1D);
                        setDailyEventBit(0x1E);
                        setDailyEventBit(0x20);
                        // cow festival
                        D_80189054 = D_801FC155;
                        break;
                        
                    case 1:                                 
                        dialogues[0].sessionManager.flags &= ~0x40;
                        func_80043AD8(0);
                        setMainLoopCallbackFunctionIndex(MAIN_GAME);
                        break;
                    }
                
                break;

            case 9:                                     
                if (temp >= 5) {
                    gVoteForFlowerFestivalGoddess = 0xFF;
                } else {
                    gVoteForFlowerFestivalGoddess = temp;
                }
                
                dialogues[0].sessionManager.flags &= ~0x40;
                func_80043AD8(0);
                setMainLoopCallbackFunctionIndex(MAIN_GAME);
                break;
            
            case 10:                                    
                switch (temp) {                   
                    case 0:                                 
                        func_800DC9FC(50);
                        setMainLoopCallbackFunctionIndex(MAIN_GAME);
                        break;
                    case 1:                                 
                        func_800DC9FC(51);
                        setMainLoopCallbackFunctionIndex(MAIN_GAME);
                        break;    
                    case 2:                                 
                        func_800DC9FC(52);
                        setMainLoopCallbackFunctionIndex(MAIN_GAME);
                        break;    
                    case 3:                                 
                        showTextBox(0, 4, 31, 0, 0);
                        break; 
                    }
                
                    break;

            case 11:                     
                switch (temp) {                     
                    case 0:                                 
                        func_800DC9FC(45);
                        setMainLoopCallbackFunctionIndex(MAIN_GAME);
                        break;    
                    case 1:                                 
                        func_800DC9FC(46);
                        setMainLoopCallbackFunctionIndex(MAIN_GAME);
                        break;    
                    case 2:                                 
                        func_800DC9FC(49);
                        setMainLoopCallbackFunctionIndex(MAIN_GAME);
                        break;    
                    case 3:
                        showTextBox(0, 4, 20, 0, 0);
                        break;
                    }
                
                break;

            case 12:                                    
                switch (temp) {                     
                    case 0:                                 
                        func_800DC9FC(47);
                        setMainLoopCallbackFunctionIndex(MAIN_GAME);
                        break;    
                    case 1:                                 
                        func_800DC9FC(48);
                        setMainLoopCallbackFunctionIndex(MAIN_GAME);
                        break;    
                    case 2:                                 
                        func_800DC9FC(49);
                        setMainLoopCallbackFunctionIndex(MAIN_GAME);
                        break;    
                    case 3:
                        showTextBox(0, 4, 20, 0, 0);
                        break;
                    }
                
                break;

            case 13:                                    
                if (gSeason == SPRING || gSeason == WINTER) {
                    if (gPlayer.actionHandler == 0x1E || temp == 3) {
                        switch (temp) {             
                            case 0:                         
                                showTextBox(1, 1, 0, 0, 0);
                                break;
                            case 1:                         
                                showTextBox(1, 1, 1, 0, 0);
                                break;
                            case 2:                         
                                showTextBox(1, 1, 2, 0, 0);
                                break;
                            case 3:
                                dialogues[0].sessionManager.flags &= ~0x40;
                                func_80043AD8(0);
                                setPlayerAction(0, 0);
                                setMainLoopCallbackFunctionIndex(MAIN_GAME);
                                break;
                            }
                        
                        func_800D55E4(gPlayer.itemInfoIndex, 1);
                        gPlayer.heldItem = 0;
                        break;
                    }
                    
                    setPlayerAction(0x1E, 0x1F);
                    setMainLoopCallbackFunctionIndex(MAIN_GAME);
                    break;
                }
                
                if (gPlayer.actionHandler == 0x1E || temp == 3) {
                    
                    switch (temp) {                 
                        case 0:                             
                            showTextBox(1, 1, 3, 0, 0);
                            break;
                        case 1:                             
                            showTextBox(1, 1, 4, 0, 0);
                            break;     
                        case 2:                             
                            showTextBox(1, 1, 5, 0, 0);
                            break;
                        case 3:
                            dialogues[0].sessionManager.flags &= ~0x40;
                            func_80043AD8(0);
                            setPlayerAction(0, 0);
                            setMainLoopCallbackFunctionIndex(MAIN_GAME);
                            break;
                        }
                    
                    func_800D55E4(gPlayer.itemInfoIndex, 1);
                    gPlayer.heldItem = 0;
                    break;
                    
                }
                
                setPlayerAction(0x1E, 0x1F);
                setMainLoopCallbackFunctionIndex(MAIN_GAME);
                break;

            case 14:                                    
                if (gSeason == SPRING || gSeason == WINTER) {
                    if (gPlayer.actionHandler == 0x1E || temp == 3) {
                        switch (temp) {            
                            case 0:                        
                                showTextBox(1, 1, 9, 0, 0);
                                break;
                            case 1:                        
                                showTextBox(1, 1, 10, 0, 0);
                                break;
                            case 2:                        
                                showTextBox(1, 1, 11, 0, 0);
                                break;
                            case 3:
                                dialogues[0].sessionManager.flags &= ~0x40;
                                func_80043AD8(0);
                                setPlayerAction(0, 0);
                                setMainLoopCallbackFunctionIndex(MAIN_GAME);
                                break;
                            }
                        
                        func_800D55E4(gPlayer.itemInfoIndex, 1);
                        gPlayer.heldItem = 0;
                        break;
                        
                    }
                    
                    setPlayerAction(0x1E, 0x1F);
                    setMainLoopCallbackFunctionIndex(MAIN_GAME);
                    break;
                }
                
                if (gPlayer.actionHandler == 0x1E || temp == 3) {
                    switch (temp) {                
                        case 0:                             
                            showTextBox(1, 1, 6, 0, 0);
                            break;
                        case 1:                             
                            showTextBox(1, 1, 7, 0, 0);
                            break;
                        case 2:                             
                            showTextBox(1, 1, 8, 0, 0);
                            break;
                        case 3:
                            dialogues[0].sessionManager.flags &= ~0x40;
                            func_80043AD8(0);
                            setPlayerAction(0, 0);
                            setMainLoopCallbackFunctionIndex(MAIN_GAME);
                            break;
                    }
                
                    func_800D55E4(gPlayer.itemInfoIndex, 1);
                    gPlayer.heldItem = 0;
                    break;
                    
                }
                
                setPlayerAction(0x1E, 0x1F);
                setMainLoopCallbackFunctionIndex(MAIN_GAME);
                break;


        case 15:                                    
            if (gSeason == SPRING || gSeason == WINTER) {
                if (gPlayer.actionHandler == 0x1E || temp == 3) {
                    switch (temp) {             
                        case 0:                         
                            showTextBox(1, 1, 12, 0, 0);
                            break;
                        case 1:                         
                            showTextBox(1, 1, 13, 0, 0);
                            break;
                        case 2:                         
                            showTextBox(1, 1, 14, 0, 0);
                            break;
                        case 3:
                            dialogues[0].sessionManager.flags &= ~0x40;
                            func_80043AD8(0);
                            setPlayerAction(0, 0);
                            setMainLoopCallbackFunctionIndex(MAIN_GAME);
                            break;
                        }
                    
                    func_800D55E4(gPlayer.itemInfoIndex, 1);
                    gPlayer.heldItem = 0;
                    break;
                }
                
                setPlayerAction(0x1E, 0x1F);
                setMainLoopCallbackFunctionIndex(MAIN_GAME);
                break;
            }
            
            if (gPlayer.actionHandler == 0x1E || temp == 3) {
                switch (temp) {                         
                    case 0:                             
                        showTextBox(1, 1, 15, 0, 0);
                        break;
                    case 1:                             
                        showTextBox(1, 1, 16, 0, 0);
                        break;
                    case 2:                             
                        showTextBox(1, 1, 17, 0, 0);
                        break;
                    case 3:
                        dialogues[0].sessionManager.flags &= ~0x40;
                        func_80043AD8(0);
                        setPlayerAction(0, 0);
                        setMainLoopCallbackFunctionIndex(MAIN_GAME);
                        break;
                    }
                
                func_800D55E4(gPlayer.itemInfoIndex, 1);
                gPlayer.heldItem = 0;
                break;
                
            }
            
                setPlayerAction(0x1E, 0x1F);
                setMainLoopCallbackFunctionIndex(MAIN_GAME);
                break;

        case 16:                                    
            if (gSeason == SPRING || gSeason == WINTER) {
                if (gPlayer.actionHandler == 0x1E || temp == 3) {
                    switch (temp) {             
                        case 0:                         
                            showTextBox(1, 1, 18, 0, 0);
                            break;
                        case 1:                         
                            showTextBox(1, 1, 19, 0, 0);
                            break;
                        case 2:                         
                            showTextBox(1, 1, 20, 0, 0);
                            break;                        
                        case 3:
                            dialogues[0].sessionManager.flags &= ~0x40;
                            func_80043AD8(0);
                            setPlayerAction(0, 0);
                            setMainLoopCallbackFunctionIndex(MAIN_GAME);
                            break;
                        }
                    
                        func_800D55E4(gPlayer.itemInfoIndex, 1);
                        gPlayer.heldItem = 0;
                        break;
                    }
                
                    setPlayerAction(0x1E, 0x1F);
                    setMainLoopCallbackFunctionIndex(MAIN_GAME);
                    break;
                }
            
            if (gPlayer.actionHandler == 0x1E || temp == 3) {
                switch (temp) {                 
                    case 0:                             
                        showTextBox(1, 1, 21, 0, 0);
                        break;
                    case 1:                             
                        showTextBox(1, 1, 22, 0, 0);
                        break;
                    case 2:                             
                        showTextBox(1, 1, 23, 0, 0);
                        break;
                    case 3:
                        dialogues[0].sessionManager.flags &= ~0x40;
                        func_80043AD8(0);
                        setPlayerAction(0, 0);
                        setMainLoopCallbackFunctionIndex(MAIN_GAME);
                        break;
                    }
                
                    func_800D55E4(gPlayer.itemInfoIndex, 1);
                    gPlayer.heldItem = 0;
                    break;
                }
            
            setPlayerAction(0x1E, 0x1F);
            setMainLoopCallbackFunctionIndex(MAIN_GAME);
            break;

        case 17:                                    
            if (gSeason == SPRING || gSeason == WINTER) {
                if (gPlayer.actionHandler == 0x1E || temp == 4) {
                    switch (temp) {            
                        case 0:                         
                            showTextBox(1, 1, 25, 0, 0);
                            break;
                        case 1:                         
                            showTextBox(1, 1, 26, 0, 0);
                            break;
                        case 2:                         
                            showTextBox(1, 1, 27, 0, 0);
                            break;
                        case 3:                         
                            showTextBox(1, 1, 24, 0, 0);
                            break;
                        case 4:
                            dialogues[0].sessionManager.flags &= ~0x40;
                            func_80043AD8(0);
                            setPlayerAction(0, 0);
                            setMainLoopCallbackFunctionIndex(MAIN_GAME);
                            break;
                        }
                    
                    func_800D55E4(gPlayer.itemInfoIndex, 1);
                    gPlayer.heldItem = 0;
                    break;
                }
                
                setPlayerAction(0x1E, 0x1F);
                setMainLoopCallbackFunctionIndex(MAIN_GAME);
                break;
            }
            
            if (gPlayer.actionHandler == 0x1E || temp == 4) {
                switch (temp) {                
                    case 0:                             
                        showTextBox(1, 1, 28, 0, 0);
                        break;
                    case 1:                             
                        showTextBox(1, 1, 29, 0, 0);
                        break;
                    case 2:                             
                        showTextBox(1, 1, 30, 0, 0);
                        break;
                    case 3:                             
                        showTextBox(1, 1, 31, 0, 0);
                        break;                       
                    case 4:        
                        dialogues[0].sessionManager.flags &= ~0x40;
                        func_80043AD8(0);
                        setPlayerAction(0, 0);
                        setMainLoopCallbackFunctionIndex(MAIN_GAME);
                        break;
                    }
            
                    func_800D55E4(gPlayer.itemInfoIndex, 1);
                    gPlayer.heldItem = 0;
                    break;
                }
        
            setPlayerAction(0x1E, 0x1F);
            setMainLoopCallbackFunctionIndex(MAIN_GAME);
            break;

        case 18:                                    
            if (gSeason == SPRING || gSeason == WINTER) {
                if (gPlayer.actionHandler == 0x1E || temp == 4) {
                    switch (temp) {             
                        case 0:                         
                            showTextBox(1, 1, 32, 0, 0);
                            break;
                        case 1:                         
                            showTextBox(1, 1, 33, 0, 0);
                            break;
                        case 2:                         
                            showTextBox(1, 1, 34, 0, 0);
                            break;
                        case 3:
                            showTextBox(1, 1, 35, 0, 0);
                            break;
                        case 4:
                            dialogues[0].sessionManager.flags &= ~0x40;
                            func_80043AD8(0);
                            setPlayerAction(0, 0);
                            setMainLoopCallbackFunctionIndex(MAIN_GAME);
                            break;
                            
                        }
                    
                        func_800D55E4(gPlayer.itemInfoIndex, 1);
                        gPlayer.heldItem = 0;
                        break;
                    }
                
                    setPlayerAction(0x1E, 0x1F);
                    setMainLoopCallbackFunctionIndex(MAIN_GAME);
                    break;
                }
            
            if (gPlayer.actionHandler == 0x1E || temp == 4) {
                switch (temp) {                 
                    case 0:                             
                        showTextBox(1, 1, 36, 0, 0);
                        break;
                    case 1:                             
                        showTextBox(1, 1, 37, 0, 0);
                        break;
                    case 2:                             
                        showTextBox(1, 1, 38, 0, 0);
                        break;
                    case 3:
                        showTextBox(1, 1, 39, 0, 0);
                        break;
                    case 4:
                        dialogues[0].sessionManager.flags &= ~0x40;
                        func_80043AD8(0);
                        setPlayerAction(0, 0);
                        setMainLoopCallbackFunctionIndex(MAIN_GAME);
                        break;
                    }
                
                    func_800D55E4(gPlayer.itemInfoIndex, 1);
                    gPlayer.heldItem = 0;
                    break;
                }
            
            setPlayerAction(0x1E, 0x1F);
            setMainLoopCallbackFunctionIndex(MAIN_GAME);
            break;
        
        case 19:                                    
            if (gSeason == SPRING || gSeason == WINTER) {
                if (gPlayer.actionHandler == 0x1E || temp == 4) {
                    switch (temp) {             
                        case 0:                         
                            showTextBox(1, 1, 40, 0, 0);
                            break;
                        case 1:                         
                            showTextBox(1, 1, 41, 0, 0);
                            break;
                        case 2:                         
                            showTextBox(1, 1, 42, 0, 0);
                            break;
                        case 3:
                            showTextBox(1, 1, 43, 0, 0);
                            break;
                        case 4:
                            dialogues[0].sessionManager.flags &= ~0x40;
                            func_80043AD8(0);
                            setPlayerAction(0, 0);
                            setMainLoopCallbackFunctionIndex(MAIN_GAME);
                            break;
                        }
                    
                        func_800D55E4(gPlayer.itemInfoIndex, 1);
                        gPlayer.heldItem = 0;
                        break;
                    }
                
                    setPlayerAction(0x1E, 0x1F);
                    setMainLoopCallbackFunctionIndex(MAIN_GAME);
                    break;
                }
            
            if (gPlayer.actionHandler == 0x1E || temp == 4) {
                switch (temp) {                 
                    case 0:                             
                        showTextBox(1, 1, 44, 0, 0);
                        break;
                    case 1:                             
                        showTextBox(1, 1, 45, 0, 0);
                        break;
                    case 2:                             
                        showTextBox(1, 1, 46, 0, 0);
                        break;
                    case 3:
                        showTextBox(1, 1, 47, 0, 0);
                        break;
                    case 4:
                        dialogues[0].sessionManager.flags &= ~0x40;
                        func_80043AD8(0);
                        setPlayerAction(0, 0);
                        setMainLoopCallbackFunctionIndex(MAIN_GAME);
                        break;
                    }
                
                    func_800D55E4(gPlayer.itemInfoIndex, 1);
                    gPlayer.heldItem = 0;
                    break;
                }
            
            setPlayerAction(0x1E, 0x1F);
            setMainLoopCallbackFunctionIndex(MAIN_GAME);
            break;
        
        case 20:                                    
            if (gSeason == SPRING || gSeason == WINTER) {
                if (gPlayer.actionHandler == 0x1E || temp == 4) {
                    switch (temp) {             
                        case 0:                         
                            showTextBox(1, 1, 48, 0, 0);
                            break;
                        case 1:                         
                            showTextBox(1, 1, 49, 0, 0);
                            break;
                        case 2:                         
                            showTextBox(1, 1, 50, 0, 0);
                            break;
                        case 3:
                            showTextBox(1, 1, 51, 0, 0);
                            break;
                        case 4:
                            dialogues[0].sessionManager.flags &= ~0x40;
                            func_80043AD8(0);
                            setPlayerAction(0, 0);
                            setMainLoopCallbackFunctionIndex(MAIN_GAME);
                            break;
                        }
                    
                        func_800D55E4(gPlayer.itemInfoIndex, 1);
                        gPlayer.heldItem = 0;
                        break;
                    }
                
                setPlayerAction(0x1E, 0x1F);
                setMainLoopCallbackFunctionIndex(MAIN_GAME);
                break;
            }
            
            if (gPlayer.actionHandler == 0x1E || temp == 4) {
                switch (temp) {                 
                    case 0:                             
                        showTextBox(1, 1, 52, 0, 0);
                        break;
                    case 1:                             
                        showTextBox(1, 1, 53, 0, 0);
                        break;
                    case 2:                             
                        showTextBox(1, 1, 54, 0, 0);
                        break;
                    case 3:   
                        showTextBox(1, 1, 55, 0, 0);
                        break;
                    case 4:
                        dialogues[0].sessionManager.flags &= ~0x40;
                        func_80043AD8(0);
                        setPlayerAction(0, 0);
                        setMainLoopCallbackFunctionIndex(MAIN_GAME);
                        break;
                    }
                
                func_800D55E4(gPlayer.itemInfoIndex, 1);
                gPlayer.heldItem = 0;
                break;
            }
            
            setPlayerAction(0x1E, 0x1F);
            setMainLoopCallbackFunctionIndex(MAIN_GAME);
            break;

        case 21:                                    
            if (gSeason == SPRING || gSeason == WINTER) {
                if (gPlayer.actionHandler == 0x1E || temp == 4) {
                    switch (temp) {             
                        case 0:                         
                            showTextBox(1, 1, 56, 0, 0);
                            break;
                        case 1:                         
                            showTextBox(1, 1, 57, 0, 0);
                            break;
                        case 2:                         
                            showTextBox(1, 1, 58, 0, 0);
                            break;
                        case 3:
                            showTextBox(1, 1, 59, 0, 0);
                            break;
                        case 4:
                            dialogues[0].sessionManager.flags &= ~0x40;
                            func_80043AD8(0);
                            setPlayerAction(0, 0);
                            setMainLoopCallbackFunctionIndex(MAIN_GAME);
                            break;
                        }
                
                    func_800D55E4(gPlayer.itemInfoIndex, 1);
                    gPlayer.heldItem = 0;
                    break;
                
                }
                
                setPlayerAction(0x1E, 0x1F);
                setMainLoopCallbackFunctionIndex(MAIN_GAME);
                break;
            }
            
            if (gPlayer.actionHandler == 0x1E || temp == 4) {

                switch (temp) {       

                    case 0:                             
                        showTextBox(1, 1, 60, 0, 0);
                        break;
                    case 1:                             
                        showTextBox(1, 1, 61, 0, 0);
                        break;
                    case 2:                             
                        showTextBox(1, 1, 62, 0, 0);
                        break;
                    case 3:
                        showTextBox(1, 1, 63, 0, 0);
                        break;
                    case 4:
                        dialogues[0].sessionManager.flags &= ~0x40;
                        func_80043AD8(0);
                        setPlayerAction(0, 0);
                        setMainLoopCallbackFunctionIndex(MAIN_GAME);
                        break;

                    }
                
                    func_800D55E4(gPlayer.itemInfoIndex, 1);
                    gPlayer.heldItem = 0;

                    break;

                }
            
            setPlayerAction(0x1E, 0x1F);
            setMainLoopCallbackFunctionIndex(MAIN_GAME);

            break;

        case 22:             

            if (gSeason == SPRING || gSeason == WINTER) {

                if (gPlayer.actionHandler == 0x1E || temp == 4) {

                    switch (temp) {             
                        case 0:                         
                            showTextBox(1, 1, 64, 0, 0);
                            break;
                        case 1:                         
                            showTextBox(1, 1, 65, 0, 0);
                            break;
                        case 2:                         
                            showTextBox(1, 1, 66, 0, 0);
                            break;
                        case 3:
                            showTextBox(1, 1, 67, 0, 0);
                            break;
                        case 4:
                            dialogues[0].sessionManager.flags &= ~0x40;
                            func_80043AD8(0);
                            setPlayerAction(0, 0);
                            setMainLoopCallbackFunctionIndex(MAIN_GAME);
                            break;
                        }
                    
                        func_800D55E4(gPlayer.itemInfoIndex, 1);
                        gPlayer.heldItem = 0;
                        break;
                    }
                
                    setPlayerAction(0x1E, 0x1F);
                    setMainLoopCallbackFunctionIndex(MAIN_GAME);
                    break;
                }
            
            if (gPlayer.actionHandler == 0x1E || temp == 4) {

                switch (temp) {                 
                    case 0:                             
                        showTextBox(1, 1, 68, 0, 0);
                        break;
                    case 1:                             
                        showTextBox(1, 1, 69, 0, 0);
                        break;
                    case 2:                             
                        showTextBox(1, 1, 70, 0, 0);
                        break;
                    case 3:
                        showTextBox(1, 1, 71, 0, 0);
                        break;
                    case 4:
                        dialogues[0].sessionManager.flags &= ~0x40;
                        func_80043AD8(0);
                        setPlayerAction(0, 0);
                        setMainLoopCallbackFunctionIndex(MAIN_GAME);
                        break;
                    }
                
                func_800D55E4(gPlayer.itemInfoIndex, 1);
                gPlayer.heldItem = 0;
                break;
                
            }
            
            setPlayerAction(0x1E, 0x1F);
            setMainLoopCallbackFunctionIndex(MAIN_GAME);
            break;

        case 23:           

            switch (temp) {                

                case 0:                 
                
                    if (calculateHouseExtensionScore() == 6 && checkLifeEventBit(MARRIED) && npcAffection[gWife] >= 250 && checkLifeEventBit(HAVE_BABY) && dogInfo.affection >= 200 && getSumNpcAffection() >= 2500 && getFarmGrassTilesSum() >= 384 && gMaximumStamina >= 190 && gHappiness >= 250 && func_8009B5E0() && D_801886D2 >= 10) {          
                        albumBits |= 0x8000;  
                    }
                    
                    setMainLoopCallbackFunctionIndex(END_OF_DAY_1);
                    break;
                
                case 1:                      
                    func_80055F08_2(0x5AA, 0x61, 1);
                    break;
                }
            
            break;

        }
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/game", func_80060454);

// main loop callback
void func_80060454(void) {
    if (checkAllSfxInactive()) {
        setMainLoopCallbackFunctionIndex(gameLoopContext.callbackIndex);
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/game", func_80060490);

// main loop callback; set cutscene prep
void func_80060490(void) {
    func_800A8F74();
} 

//INCLUDE_ASM("asm/nonmatchings/game/game", func_800604B0);

// show end of festival day dialogue box 
void func_800604B0(void) {

    if (checkMapRGBADone(MAIN_MAP_INDEX) || !(mainMap[MAIN_MAP_INDEX].mapState.flags & MAP_ACTIVE)) {
        
        setMessageBoxViewSpacePosition(MAIN_DIALOGUE_BOX_INDEX, 0, -64.0f, 352.0f);
        setMessageBoxSpriteIndices(MAIN_DIALOGUE_BOX_INDEX, 1, 0, 0);
        func_8003F360(MAIN_DIALOGUE_BOX_INDEX, -4, 0);
        
        switch (gCutsceneIndex) {
            case 0x81 ... 0x82:
            case 0x8B:              
            case 0x195 ... 0x199:
                initializeMessageBox(MAIN_DIALOGUE_BOX_INDEX, 4, 0x4D, 0);
                break;
            case SEA_FESTIVAL:
            case EGG_FESTIVAL:
            case NEW_YEAR_FESTIVAL:
                initializeMessageBox(MAIN_DIALOGUE_BOX_INDEX, 4, 0x4A, 0);
                break;
            case 0x1A0:
                initializeMessageBox(MAIN_DIALOGUE_BOX_INDEX, 4, 0x4C, 0);
                break;
            default:
                initializeMessageBox(MAIN_DIALOGUE_BOX_INDEX, 4, 0x4B, 0);
                break;
        }
        
        setMainLoopCallbackFunctionIndex(END_OF_FESTIVAL_DAY_2);

    }
}

//INCLUDE_ASM("asm/nonmatchings/game/game", func_800605F0);

// second end of festival day callback
void func_800605F0(void) {
    // check if a message box has flag 4 set
    if (func_8003F0DC()) {
        setMainLoopCallbackFunctionIndex(END_OF_DAY_1);
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/game", func_80060624);

// first end of day callback
void func_80060624(void) {
    
    u8 tempTime;

    if (checkMapRGBADone(MAIN_MAP_INDEX) || !(mainMap[MAIN_MAP_INDEX].mapState.flags & MAP_ACTIVE)) {
        
        func_800610DC();
        
        // earthquake
        if (func_80060DC0()) {
            playSfx(RUMBLE);
            setMainLoopCallbackFunctionIndex(WAIT_AUDIO_FINISH);
            gameLoopContext.callbackIndex = END_OF_DAY_1;
            return;
        }
        
        setEntrance(MIDDLE_OF_HOUSE);
        func_80056030(2);
        
        // dream cutscenes
        if (func_800A87C4()) {
            setMainLoopCallbackFunctionIndex(3);
            return;
        }
        
        if (!checkDailyEventBit(0)) {
            handleDailyShipment();
        }
        
        if (checkFatigueLevel() != 3) {
            
            tempTime = gHour;
            
            if (tempTime < 6) {
                tempTime += 24;
            }
            
            if (checkLifeEventBit(0x5A)) {
                gPlayer.fatigueCounter += adjustValue(gPlayer.fatigueCounter, -((30 - tempTime) * 3), 100);
            } else {
                gPlayer.fatigueCounter += adjustValue(gPlayer.fatigueCounter, -((30 - tempTime) * 2), 100);
            }
        }
        
        if (17 < gHour && gHour < 23) {
            setLifeEventBit(0x60);
        } else {
            toggleLifeEventBit(0x60);
        }
         
        setClockNewDay(); 
        
        gPlayer.currentStamina = gMaximumStamina;
        
        startNewDay();
        setEntrance(MIDDLE_OF_HOUSE);
        
        setMainLoopCallbackFunctionIndex(MAP_LOAD);
        
        setDailyEventBit(0x53);
        setDailyEventBit(0x54);

    }
}

//INCLUDE_ASM("asm/nonmatchings/game/game", func_80060838);

// second end of day callback
void func_80060838(void) {
    
    if (checkMapRGBADone(MAIN_MAP_INDEX) || !(mainMap[MAIN_MAP_INDEX].mapState.flags & MAP_ACTIVE)) {

        func_800610DC();

        if (gPlayer.flags & 1) {

            gPlayer.flags &= ~1;
            
            toggleDailyEventBit(0x5C);
             
            setHorseLocation(0xFF);
            horseInfo.flags &= ~0x8;
            
        }

        // earthquake
        if (func_80060DC0()) {
            
            playSfx(RUMBLE);
            
            setMainLoopCallbackFunctionIndex(WAIT_AUDIO_FINISH);
            gameLoopContext.callbackIndex = END_OF_DAY_1;

            return;

        }

        setEntrance(MIDDLE_OF_HOUSE);
        
        // setup/handle loading
        func_80056030(2);

        // dream cutscenes
        if (func_800A87C4()) {
            setMainLoopCallbackFunctionIndex(3);
            return;
        }

        if (!checkDailyEventBit(0)) {
            // handle daily shipment
            handleDailyShipment();
        }

        startNewDay();

        setEntrance(MIDDLE_OF_HOUSE);
        
        setMainLoopCallbackFunctionIndex(MAP_LOAD);

        setDailyEventBit(0x53);
        setDailyEventBit(0x54);

    }
}

//INCLUDE_ASM("asm/nonmatchings/game/game", setFestivalDailyBits);

void setFestivalDailyBits(void) {

    if (gSeason == SPRING && gDayOfMonth == 1) {
        setDailyEventBit(0x30);
        setDailyEventBit(0x28);
        setDailyEventBit(FESTIVAL);
        setDailyEventBit(0x4E);
    }
    
    if (gSeason == SPRING && gDayOfMonth == 8) {
        setDailyEventBit(0x30);
        setDailyEventBit(0x28);
        setDailyEventBit(FESTIVAL);
        setDailyEventBit(0x4E);
    }
    
    if (gSeason == SPRING && gDayOfMonth == 17) {
        setDailyEventBit(0x30);
        setDailyEventBit(0x28);
        setDailyEventBit(FESTIVAL);
        setDailyEventBit(0x4E);
    }

    if (gSeason == AUTUMN && gDayOfMonth == 28) {
        setDailyEventBit(0x30);
        setDailyEventBit(0x28);
        setDailyEventBit(FESTIVAL);
        setDailyEventBit(0x4E);
    }
    
    if (gSeason == SPRING && gDayOfMonth == 23) {
        setDailyEventBit(0x30);
        setDailyEventBit(0x28);
        setDailyEventBit(FESTIVAL);
        setDailyEventBit(0x4E);
    }

    if (gSeason == SUMMER && gDayOfMonth == 1) {
        setDailyEventBit(0x30);
        setDailyEventBit(0x4E);
    }
    
    if (gSeason == SUMMER && gDayOfMonth == 9) {
        setDailyEventBit(0x30);
        setDailyEventBit(0x28);
        setDailyEventBit(FESTIVAL);
        setDailyEventBit(0x4E);
    }

    if (gSeason == SUMMER && gDayOfMonth == 17) {
        setDailyEventBit(0x30);
        setDailyEventBit(0x4E);
    }

    if (gSeason == SUMMER && gDayOfMonth == 24) {
        setDailyEventBit(0x30);
        setDailyEventBit(0x28);
        setDailyEventBit(FESTIVAL);
        setDailyEventBit(0x4E);
    }

    if (gSeason == AUTUMN && gDayOfMonth == 4) {
        setDailyEventBit(0x30);
        setDailyEventBit(0x28);
        setDailyEventBit(FESTIVAL);
        setDailyEventBit(0x4E);
    }

    if (gSeason == AUTUMN && gDayOfMonth == 12) {
        setDailyEventBit(0x30);
        setDailyEventBit(0x28);
        setDailyEventBit(FESTIVAL);
        setDailyEventBit(0x4E);
    }
    
    if (gSeason == AUTUMN && gDayOfMonth == 20) {
        setDailyEventBit(0x30);
        setDailyEventBit(0x28);
        setDailyEventBit(FESTIVAL);
        setDailyEventBit(0x4E);
    }

    if (gSeason == WINTER && gDayOfMonth == 10) {
        setDailyEventBit(0x30);
        setDailyEventBit(0x4E);
    }
    
    if (gSeason == WINTER && gDayOfMonth == 19) {
        setDailyEventBit(0x30);
        setDailyEventBit(0x28);
        setDailyEventBit(FESTIVAL);
        setDailyEventBit(0x4E);
    }

    if (gSeason == WINTER && gDayOfMonth == 24) {
        setDailyEventBit(0x30);
        setDailyEventBit(0x4E);
    }
    
    if (gSeason == WINTER && gDayOfMonth == 27) {
        setDailyEventBit(0x30);
        setDailyEventBit(0x4E);
    }
    if (gSeason == WINTER && gDayOfMonth == 30) {
        setDailyEventBit(0x30);
        setDailyEventBit(0x4E);
    }

    if (gDayOfWeek == SUNDAY && checkLifeEventBit(0xF)) {
        setDailyEventBit(0x4E);
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/game", func_80060DC0);

// earthquake
bool func_80060DC0(void) {

    bool result = FALSE;

    if (!checkLifeEventBit(0x4E) && gSeason > AUTUMN && (5 < gDayOfMonth && gDayOfMonth < 10)) {

        if (!getRandomNumberInRange(0, 2) || gDayOfMonth == 9) {
            setLifeEventBit(0x4E);
            func_80064CF0();
            result = TRUE;
        }
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/game/game", handleDailyShipment);

void handleDailyShipment(void) {

    gGold += adjustValue(gGold, dailyShippingBinValue, MAX_GOLD);
    dailyShippingBinValue = 0;

    setDailyEventBit(DAILY_SHIPMENT);

}
 
//INCLUDE_ASM("asm/nonmatchings/game/game", calculateAnimalDirectionToPlayer);

u8 calculateAnimalDirectionToPlayer(f32 animalX, f32 animalZ, f32 playerX, f32 playerZ) {
   
    u8 direction;
    
    s16 deltaX;
    s16 deltaZ;

    deltaX = getAbsoluteValue((s32)(s16)(playerX - animalX));
    deltaZ = getAbsoluteValue((s32)(s16)(playerZ - animalZ));
    
    if (animalX <= playerX) {
        
        if (animalZ <= playerZ) {
            
            if (deltaX <= deltaZ) {
                direction = (deltaZ / 2) < deltaX ? SOUTH : SOUTHWEST;
            } else {
                direction = (deltaX / 2) < deltaZ ? SOUTH : SOUTHEAST;
            }
            
        } else if (deltaX <= deltaZ) {

            direction = deltaX > (deltaZ / 2) ? EAST : NORTHEAST;

        } else {

            if (deltaX / 2 >= deltaZ) {
                direction = SOUTHEAST;
            } else {
                direction = EAST;
            }

        }
        
    } else if (animalZ <= playerZ) {
        
        if (deltaX <= deltaZ) {

            direction = (deltaZ / 2) < deltaX;
            
        } else {

            if ((deltaX / 2) >= deltaZ) {
                direction = NORTHWEST;
            } else {
                direction = WEST;
            }
        }
        
    } else if (deltaX <= deltaZ) {
        
        if ((deltaZ / 2) >= deltaX) {
            direction = NORTHEAST;
        } else {
            direction = NORTH;
        }
        
    } else {
        direction = (deltaZ > (deltaX / 2)) ? NORTH : NORTHWEST;
    }
    
    return direction;

}

//INCLUDE_ASM("asm/nonmatchings/game/game", func_800610DC);

void func_800610DC(void) {
    
    if (gPlayer.heldItem) {
        
        func_800D5548(gPlayer.itemInfoIndex);

        switch (gPlayer.heldItem) {
            case 0x60 ... 0x6F:
                func_80088C1C(0xFF, gPlayer.heldAnimalIndex);
                break;
            case 0x58 ... 0x5F:
                setDogLocation(0xFF);
                break;
            default:
                break;
        }
        
        gPlayer.heldItem = 0;
        gItemBeingHeld = 0xFF;

    }

}

//INCLUDE_ASM("asm/nonmatchings/game/game", func_80061178);

// check if girl is ready to get married for blue feather to appear
bool func_80061178(void) {
    
    bool result;

    result = npcAffection[MARIA] >= MARRIAGE_READY_AFFECTION_THRESHOLD;
    
    if (npcAffection[POPURI] >= MARRIAGE_READY_AFFECTION_THRESHOLD) {
        result = TRUE;
    }
    
    if (npcAffection[ELLI] >= MARRIAGE_READY_AFFECTION_THRESHOLD) {
        result = TRUE;
    }

    if (npcAffection[ANN] >= MARRIAGE_READY_AFFECTION_THRESHOLD) {
        result = TRUE;
    }

    if (npcAffection[KAREN] >= MARRIAGE_READY_AFFECTION_THRESHOLD) {
        result = TRUE;
    }
    
    return result;

}

// func_800611E0

//INCLUDE_ASM("asm/nonmatchings/game/game", setWifeNameString);

void setWifeNameString(u8 wife) {

    switch (wife) {
        case MARIA:
            gWifeName[0] = char_M;
            gWifeName[1] = char_a;
            gWifeName[2] = char_r;
            gWifeName[3] = char_i;
            gWifeName[4] = char_a;
            gWifeName[5] = 0xFF;
            break;
        case POPURI:
            gWifeName[0] = char_P;
            gWifeName[1] = char_o;
            gWifeName[2] = char_p;
            gWifeName[3] = char_u;
            gWifeName[4] = char_r;
            gWifeName[5] = char_i;
            break;
        case ELLI:
            gWifeName[0] = char_E;
            gWifeName[1] = char_l;
            gWifeName[2] = char_l;
            gWifeName[3] = char_i;
            gWifeName[4] = 0xFF;
            gWifeName[5] = 0xFF;
            break;
        case ANN:
            gWifeName[0] = char_A;
            gWifeName[1] = char_n;
            gWifeName[2] = char_n;
            gWifeName[3] = 0xFF;
            gWifeName[4] = 0xFF;
            gWifeName[5] = 0xFF;
            break;
        case KAREN:
            gWifeName[0] = char_K;
            gWifeName[1] = char_a;
            gWifeName[2] = char_r;
            gWifeName[3] = char_e;
            gWifeName[4] = char_n;
            gWifeName[5] = 0xFF;
            break;
        default:
            return;
            
    }
}

// func_80061354
//INCLUDE_ASM("asm/nonmatchings/game/game", setDefaultBabyName);

void setDefaultBabyName(u8 wife) {
    
    switch (wife) {
        case MARIA:
            gBabyName[0] = char_L;
            gBabyName[1] = char_i;
            gBabyName[2] = char_y;
            gBabyName[3] = char_a;
            gBabyName[4] = 0xFF;
            gBabyName[5] = 0xFF;
            break;
        case POPURI:
            gBabyName[0] = char_P;
            gBabyName[1] = char_a;
            gBabyName[2] = char_r;
            gBabyName[3] = char_s;
            gBabyName[4] = char_l;
            gBabyName[5] = char_y;
            break;
        case ELLI:
            gBabyName[0] = char_C;
            gBabyName[1] = char_o;
            gBabyName[2] = char_c;
            gBabyName[3] = char_o;
            gBabyName[4] = char_t;
            gBabyName[5] = char_e;
            break;
        case ANN:
            gBabyName[0] = char_M;
            gBabyName[1] = char_i;
            gBabyName[2] = char_n;
            gBabyName[3] = char_t;
            gBabyName[4] = 0xFF;
            gBabyName[5] = 0xFF; 
            break;
        case KAREN:
            gBabyName[0] = char_K;
            gBabyName[1] = char_e;
            gBabyName[2] = char_l;
            gBabyName[3] = char_l;
            gBabyName[4] = char_y;
            gBabyName[5] = 0xFF;
            break;
        default:
            break;
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/game", setHarvestKingName);
 
void setHarvestKingName(u8 harvestKing) {

    switch (harvestKing) {
        case PLAYER:
            harvestKingName[0] = gPlayer.name[0];
            harvestKingName[1] = gPlayer.name[1];
            harvestKingName[2] = gPlayer.name[2];
            harvestKingName[3] = gPlayer.name[3];
            harvestKingName[4] = gPlayer.name[4];
            harvestKingName[5] = gPlayer.name[5];
            break;
        case 1:
            harvestKingName[0] = char_H;
            harvestKingName[1] = char_a;
            harvestKingName[2] = char_r;
            harvestKingName[3] = char_r;
            harvestKingName[4] = char_i;
            harvestKingName[5] = char_s;
            break;
        default:
            break;
        case 2:
            harvestKingName[0] = char_G;
            harvestKingName[1] = char_r;
            harvestKingName[2] = char_a;
            harvestKingName[3] = char_y;
            harvestKingName[4] = 0xFF;
            harvestKingName[5] = 0xFF;
            break;
        case 3:
            harvestKingName[0] = char_J;
            harvestKingName[1] = char_e;
            harvestKingName[2] = char_f;
            harvestKingName[3] = char_f;
            harvestKingName[4] = 0xFF;
            harvestKingName[5] = 0xFF;
            break;
        case 4:
            harvestKingName[0] = char_C;
            harvestKingName[1] = char_l;
            harvestKingName[2] = char_i;
            harvestKingName[3] = char_f;
            harvestKingName[4] = char_f;
            harvestKingName[5] = 0xFF;
            break;
        case 5:
            harvestKingName[0] = char_K;
            harvestKingName[1] = char_a;
            harvestKingName[2] = char_i;
            harvestKingName[3] = 0xFF;
            harvestKingName[4] = 0xFF;
            harvestKingName[5] = 0xFF;
            break;
        }
}

//INCLUDE_ASM("asm/nonmatchings/game/game", func_80061690);

void func_80061690(u8 arg0) {
    D_801886D4[0] = 0xF6;
    D_801886D4[1] = 0xF6;
    D_801886D4[2] = 0xF6;
    D_801886D4[3] = 0xF6;
    D_801886D4[4] = 0xF6;
    D_801886D4[5] = 0xF6;
}

//INCLUDE_ASM("asm/nonmatchings/game/game", func_800616CC);

u8 func_800616CC(u8 houseExtensionIndex) {

    s16 prices[6];
    s16 lumberCosts[6];

    memcpy(prices, houseExtensionPrices, 12);
    memcpy(lumberCosts, houseExtensionLumberCosts, 12);

    if (adjustValue(gLumber, lumberCosts[houseExtensionIndex], MAX_LUMBER) == lumberCosts[houseExtensionIndex]) {
        
        if (adjustValue(gGold, prices[houseExtensionIndex], MAX_GOLD) == prices[houseExtensionIndex]) {

            gLumber += adjustValue(gLumber, lumberCosts[houseExtensionIndex], MAX_LUMBER);
            gGold += adjustValue(gGold, prices[houseExtensionIndex], MAX_GOLD);

            houseExtensionConstructionCounter = 0;
            
            setDailyEventBit(9);
            
            return 0;

        } else {
            return 1;
        }
    }

    return 2;
    
}

// func_80061860
//INCLUDE_ASM("asm/nonmatchings/game/game", setFlowerFestivalGoddess);

void setFlowerFestivalGoddess(void) {
    
    u8 temp;

    if (gVoteForFlowerFestivalGoddess < 5 && !getRandomNumberInRange(0, 2)) {
        temp = gVoteForFlowerFestivalGoddess;
    } else {
        temp = getRandomNumberInRange(0, 4);
    }
    
    gFlowerFestivalGoddess = temp;
    
    if (checkLifeEventBit(0x2F) && gFlowerFestivalGoddess == KAREN) {
        gFlowerFestivalGoddess = MARIA;
    }

}

// func_800618EC
//INCLUDE_ASM("asm/nonmatchings/game/game", getBacholeretteWithHighestAffection);

u8 getBacholeretteWithHighestAffection(u8 affectionLevel) {
    
    u8 girlIndex = 0;

    if (npcAffection[MARIA] >= affectionLevel && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
        affectionLevel = npcAffection[MARIA];
        girlIndex = 1;
    }

    if (npcAffection[POPURI] >= affectionLevel && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
        affectionLevel = npcAffection[POPURI];
        girlIndex = 2;
    }
    
    if (npcAffection[ELLI] >= affectionLevel && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {
        affectionLevel = npcAffection[ELLI];
        girlIndex = 3;
    }

    if (npcAffection[ANN] >= affectionLevel && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
        affectionLevel = npcAffection[ANN];
        girlIndex = 4;
    }

    if (npcAffection[KAREN] >= affectionLevel && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
        affectionLevel = npcAffection[KAREN];
        girlIndex = 5;
    }
    
    return girlIndex;

}

//INCLUDE_ASM("asm/nonmatchings/game/game", func_80061A1C);

inline void func_80061A1C(u8 numberOfSpiritFestivalAssistants, u8 npcIndex) {
    switch (numberOfSpiritFestivalAssistants) {                   
        case 1:
            spiritFestivalAssistant1 = npcIndex;
            break;
        case 2:
            spiritFestivalAssistant2 = npcIndex;
            break;
        case 3:
            spiritFestivalAssistant3 = npcIndex;
            break;
    }
}

static inline void setSpiritFestivalAssistant(u8 npcIndex) {
    
    numberOfSpiritFestivalAssistantsRecruited++;

    func_80061A1C(numberOfSpiritFestivalAssistantsRecruited, npcIndex);

}

//INCLUDE_ASM("asm/nonmatchings/game/game", recruitSpiritFestivalAssistants);

void recruitSpiritFestivalAssistants(void) {
    
    if (checkSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_MARIA)) {
        
        toggleSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_MARIA);
        
        setSpiritFestivalAssistant(MARIA+1);
        
    }
    
    if (checkSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_POPURI)) {
        
        toggleSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_POPURI);
        
        setSpiritFestivalAssistant(ELLI+1);
    }

    if (checkSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_ELLI)) {
        
        toggleSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_ELLI);
        
        setSpiritFestivalAssistant(POPURI+1);
    }
    
    if (checkSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_ANN)) {
        
        toggleSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_ANN);
        
        setSpiritFestivalAssistant(ANN+1);
    }
    
   if (checkSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_KAREN)) {
        
        toggleSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_KAREN);
       
        setSpiritFestivalAssistant(KAREN+1);
    } 

   if (checkSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_HARRIS)) {
        
        toggleSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_HARRIS);
       
        setSpiritFestivalAssistant(HARRIS);
    }

   if (checkSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_GRAY)) {
        
        toggleSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_GRAY);
       
        setSpiritFestivalAssistant(GRAY);
    } 

   if (checkSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_JEFF)) {
        
        toggleSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_JEFF);
       
        setSpiritFestivalAssistant(JEFF);
    } 

   if (checkSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_CLIFF)) {
        
        toggleSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_CLIFF);
       
        setSpiritFestivalAssistant(CLIFF);
    } 

   if (checkSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_KAI)) {
        
        toggleSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_KAI);
       
        setSpiritFestivalAssistant(KAI);

    } 
    
    if (numberOfSpiritFestivalAssistantsRecruited >= 3) {
        toggleSpecialDialogueBit(PLAYER_HARVEST_KING_SPIRIT_FESTIVAL_ASSISTANTS);
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/game", setRecipes);

void setRecipes(void) {

    if (checkSpecialDialogueBit(7)) {
        addRecipe(0);
    }
    if (checkSpecialDialogueBit(0x50)) {
        addRecipe(1);
    }
    if (checkSpecialDialogueBit(0x51)) {
        addRecipe(2);
    }
    if (checkSpecialDialogueBit(0x52)) {
        addRecipe(3);
    }
    if (checkSpecialDialogueBit(0x53)) {
        addRecipe(4);
    }
    if (checkSpecialDialogueBit(0x54)) {
        addRecipe(5);
    }
    if (checkSpecialDialogueBit(0x55)) {
        addRecipe(6);
    }
    if (checkSpecialDialogueBit(0x56)) {
        addRecipe(7);
    }
    if (checkSpecialDialogueBit(0x57)) {
        addRecipe(8);
    }
    if (checkSpecialDialogueBit(0x58)) {
        addRecipe(9);
    }
    if (checkSpecialDialogueBit(0x59)) {
        addRecipe(0xA);
    }
    if (checkSpecialDialogueBit(0x5A)) {
        addRecipe(0xB);
    }
    if (checkSpecialDialogueBit(0x5B)) {
        addRecipe(0xC);
    }
    if (checkSpecialDialogueBit(0x5C)) {
        addRecipe(0xD);
    }
    if (checkSpecialDialogueBit(0x5D)) {
        addRecipe(0xE);
    }
    if (checkSpecialDialogueBit(0x5E)) {
        addRecipe(0xF);
    } 
    if (checkSpecialDialogueBit(0x5F)) {
        addRecipe(0x10);
    }
    if (checkSpecialDialogueBit(0x60)) {
        addRecipe(0x11);
    }
    if (checkSpecialDialogueBit(0x61)) {
        addRecipe(0x12);
    }
    if (checkSpecialDialogueBit(0x62)) {
        addRecipe(0x13);
    }
    if (checkSpecialDialogueBit(0x63)) {
        addRecipe(0x14);
    }
    if (checkSpecialDialogueBit(0x64)) {
        addRecipe(0x15);
    }
    if (checkSpecialDialogueBit(0x65)) {
        addRecipe(0x16);
    }
    if (checkSpecialDialogueBit(0x66)) {
        addRecipe(0x17);
    }
    if (checkSpecialDialogueBit(0x67)) {
        addRecipe(0x18);
    }
    if (checkSpecialDialogueBit(0x68)) {
        addRecipe(0x19);
    }
    if (checkSpecialDialogueBit(0x69)) {
        addRecipe(0x1A);
    }
    if (checkSpecialDialogueBit(0x6A)) {
        addRecipe(0x1B);
    }
    if (checkSpecialDialogueBit(0x6B)) {
        addRecipe(0x1C);
    }
    if (checkSpecialDialogueBit(0x6C)) {
        addRecipe(0x1D);
    }
    if (checkSpecialDialogueBit(0x6D)) {
        addRecipe(0x1E);
    }
    if (checkSpecialDialogueBit(0x6E)) {
        addRecipe(0x1F);
    }
    if (checkSpecialDialogueBit(0x6F)) {
        addRecipe(0x20);
    }
    if (checkSpecialDialogueBit(0x70)) {
        addRecipe(0x21);
    }
    if (checkSpecialDialogueBit(0x71)) {
        addRecipe(0x22);
    }

}

// func_800623E4
//INCLUDE_ASM("asm/nonmatchings/game/game", handleHouseConstruction);

u8 handleHouseConstruction(u8 day) {

    u8 result;
    
    u8 buffer1[6];
    u16 buffer2[8];
    u8 buffer3[6];

    memcpy(buffer1, houseConstructionDays, 6);
    memcpy(buffer2, lifeEventHouseConstructionBits, 12);
    memcpy(buffer3, animalLocationsHouseConstruction, 6);

    result = FALSE;

    if (checkLifeEventBit(HOUSE_EXTENSION_CONSTRUCTION)) {

        if (houseExtensionConstructionCounter >= buffer1[day]) {

            toggleLifeEventBit(HOUSE_EXTENSION_CONSTRUCTION);
            setDailyEventBit(CARPENTER_FINISHED);
            setLifeEventBit(buffer2[day]);

            if (day == 3) {
                toggleLifeEventBit(0xD7);
            }

            setAnimalLocations(buffer3[day]);

            result = TRUE;
            
        }
        
    }

    return result;

}

// alternate without memcpy
/*
u8 handleHouseConstruction(u8 day) {

    u8 *dayArrayPtr;
    u8 *animalLocationsArrayPtr;
    
    u8 result;
    
    Days dayArray;
    Bits lifeEventBitArray;
    AnimalLocations animalLocationsArray;
    
    dayArray = *(Days*)houseConstructionDays;
    lifeEventBitArray = *(Bits*)lifeEventHouseConstructionBits;
    animalLocationsArray = *(AnimalLocations*)animalLocationsHouseConstruction;
    
    dayArrayPtr = (u8*)&dayArray;
    animalLocationsArrayPtr = (u8*)&animalLocationsArray;
    
    result = 0;
    
    if (checkLifeEventBit(HOUSE_EXTENSION_CONSTRUCTION)) {

        if (houseExtensionConstructionCounter >= dayArrayPtr[day]) {
            
            toggleLifeEventBit(HOUSE_EXTENSION_CONSTRUCTION);
            setDailyEventBit(CARPENTER_FINISHED);
            setLifeEventBit(((u16*)(&lifeEventBitArray))[day]);
            
            if (day == 3) {
                toggleLifeEventBit(0xD7);
            }
            
            setAnimalLocations(animalLocationsArrayPtr[day]);
            result = TRUE;
            
        }
    }
    
    return result;
}
*/

// func_8006252C
//INCLUDE_ASM("asm/nonmatchings/game/game", setLetters);

// TODO: label bits/use flags
void setLetters(void) {

    if (!checkMailRead(0) && (!checkLifeEventBit(MARRIED) || gWife != MARIA) && npcAffection[MARIA] >= 120 && gSeason == SPRING && !getRandomNumberInRange(0, 10)) {
        setMail(0);
    }

    if (!checkMailRead(1) && (!checkLifeEventBit(MARRIED) || gWife != MARIA) && npcAffection[MARIA] >= 140 && gSeason == SUMMER && !getRandomNumberInRange(0, 10)) {
        setMail(1);
    }

    if (!checkMailRead(2) && (!checkLifeEventBit(MARRIED) || gWife != MARIA) && npcAffection[MARIA] >= 160 && gSeason == AUTUMN && !getRandomNumberInRange(0, 10)) {
        setMail(2);
    }

    if (!checkMailRead(3) && (!checkLifeEventBit(MARRIED) || gWife != MARIA) && npcAffection[MARIA] >= 180 && gSeason == WINTER && !getRandomNumberInRange(0, 10)) {
        setMail(3);
    }

    if (!checkMailRead(4) && (!checkLifeEventBit(MARRIED) || gWife != POPURI) && npcAffection[POPURI] >= 120 && gSeason == SPRING && !getRandomNumberInRange(0, 10)) {
        setMail(4);
    }

    if (!checkMailRead(5) && (!checkLifeEventBit(MARRIED) || gWife != POPURI) && npcAffection[POPURI] >= 140 && gSeason == SUMMER && !getRandomNumberInRange(0, 10)) {
        setMail(5);
    }
    if (!checkMailRead(6) && (!checkLifeEventBit(MARRIED) || gWife != POPURI) && npcAffection[POPURI] >= 160 && gSeason == AUTUMN && !getRandomNumberInRange(0, 10)) {
        setMail(6);
    }

    if (!checkMailRead(7) && (!checkLifeEventBit(MARRIED) || gWife != POPURI) && npcAffection[POPURI] >= 180 && gSeason == WINTER && !getRandomNumberInRange(0, 10)) {
        setMail(7);
    }

    if (!checkMailRead(8) && (!checkLifeEventBit(MARRIED) || gWife != ELLI) && npcAffection[ELLI] >= 120 && gSeason == SPRING && !getRandomNumberInRange(0, 10)) {
        setMail(8);
    }

    if (!checkMailRead(9) && (!checkLifeEventBit(MARRIED) || gWife != ELLI) && npcAffection[ELLI] >= 140 && gSeason == SUMMER && !getRandomNumberInRange(0, 10)) {
        setMail(9);
    }

    if (!checkMailRead(0xA) && (!checkLifeEventBit(MARRIED) || (gWife != ELLI)) && npcAffection[ELLI] >= 160 && gSeason == AUTUMN && !getRandomNumberInRange(0, 10)) {
        setMail(0xA);
    }

    if (!checkMailRead(0xB) && (!checkLifeEventBit(MARRIED) || gWife != ELLI) && npcAffection[ELLI] >= 180 && gSeason == WINTER && !getRandomNumberInRange(0, 10)) {
        setMail(0xB);
    }

    if (!checkMailRead(0xC) && (!checkLifeEventBit(MARRIED) || gWife != ANN) && npcAffection[ANN] >= 120 && gSeason == SPRING && !getRandomNumberInRange(0, 10)) {
        setMail(0xC);
    }

    if (!checkMailRead(0xD) && (!checkLifeEventBit(MARRIED) || gWife != ANN) && npcAffection[ANN] >= 140 && gSeason == SUMMER && !getRandomNumberInRange(0, 10)) {
        setMail(0xD);
    }

    if (!checkMailRead(0xE) && (!checkLifeEventBit(MARRIED) || gWife != ANN) && npcAffection[ANN] >= 160 && (gSeason == AUTUMN) && !getRandomNumberInRange(0, 10)) {
        setMail(0xE);
    }

    if (!checkMailRead(0xF) && (!checkLifeEventBit(MARRIED) || gWife != ANN) && npcAffection[ANN] >= 180 && gSeason == WINTER && !getRandomNumberInRange(0, 10)) {
        setMail(0xF);
    }

    if (!checkMailRead(0x10) && (!checkLifeEventBit(MARRIED) || gWife != KAREN) && npcAffection[KAREN] >= 120 && gSeason == SPRING && !getRandomNumberInRange(0, 10)) {
        setMail(0x10);
    }
    
    if (!checkMailRead(0x11) && (!checkLifeEventBit(MARRIED) || gWife != KAREN) && npcAffection[KAREN] >= 140 && gSeason == SUMMER && !getRandomNumberInRange(0, 10)) {
        setMail(0x11);
    }

    if (!checkMailRead(0x12) && (!checkLifeEventBit(MARRIED) || gWife != KAREN) && npcAffection[KAREN] >= 160 && gSeason == AUTUMN && !getRandomNumberInRange(0, 10)) {
        setMail(0x12);
    }

    if (!checkMailRead(0x13) && (!checkLifeEventBit(MARRIED) || gWife != KAREN) && npcAffection[KAREN] >= 180 && gSeason == WINTER && !getRandomNumberInRange(0, 10)) {
        setMail(0x13);
    }

    if (!checkMailRead(0x14) && npcAffection[MAYOR] >= 120 && gSeason == SPRING && !getRandomNumberInRange(0, 10)) {
        setMail(0x14);
    }

    if (!checkMailRead(0x15) && npcAffection[HARRIS] >= 120 && gSeason == SPRING && !getRandomNumberInRange(0, 10)) {
        setMail(0x15);
    }

    if (!checkMailRead(0x16) && checkLifeEventBit(MARIA_HARRIS_HAVE_BABY)) {
        setMail(0x16);
    }

    if (!checkMailRead(0x17) && npcAffection[GRAY] >= 120 && gSeason == SPRING && !getRandomNumberInRange(0, 10)) {
        setMail(0x17);
    }

    if (!checkMailRead(0x18) && checkLifeEventBit(POPURI_GRAY_HAVE_BABY)) {
        setMail(0x18);
    }

    if (!checkMailRead(0x19) && npcAffection[JEFF] >= 120 && gSeason == SPRING && !getRandomNumberInRange(0, 10)) {
        setMail(0x19);
    }

    if (!checkMailRead(0x1A) && checkLifeEventBit(ELLI_JEFF_HAVE_BABY)) {
        setMail(0x1A);
    }

    if (!checkMailRead(0x1B) && checkLifeEventBit(CLIFF_GONE) && !getRandomNumberInRange(0, 10)) {
        setMail(0x1B);
    }
 
    if (!checkMailRead(0x1C) && checkLifeEventBit(ANN_CLIFF_HAVE_BABY)) {
        setMail(0x1C);
    }

    if (!checkMailRead(0x1D) && npcAffection[KAI] >= 120 && gSeason == SPRING && !getRandomNumberInRange(0, 10)) {
        setMail(0x1D);
    }

    if (!checkMailRead(0x1E) && checkLifeEventBit(KAREN_KAI_HAVE_BABY)) {
        setMail(0x1E);
    }

    if (!checkMailRead(0x1F) && checkLifeEventBit(KAI_GONE) && !getRandomNumberInRange(0, 10)) {
        setMail(0x1F);
    }

    if (!checkMailRead(0x20) && (npcAffection[KENT] + npcAffection[STU]) >= 160 && gSeason == SPRING && !getRandomNumberInRange(0, 10)) {
        setMail(0x20);
    }

    if (!checkMailRead(0x21) && npcAffection[RICK] >= 120 && gSeason == SPRING && !getRandomNumberInRange(0, 10)) {
        setMail(0x21);
    }
    
    if (!checkMailRead(0x22) && npcAffection[BASIL] >= 100 && gSeason == WINTER && !getRandomNumberInRange(0, 10)) {
        setMail(0x22);
    }

    if (!checkMailRead(0x23) && checkLifeEventBit(SHADY_SALESMAN_MARIA_CUTSCENE)) {
        setMail(0x23);
    }

    if (!checkMailRead(0x24) && checkLifeEventBit(MARRIED) && gWife == MARIA) {
        setMail(0x24);
    }

    if (!checkMailRead(0x25) && checkLifeEventBit(MARRIED) && (gWife == POPURI)) {
        setMail(0x25);
    }

    if (!checkMailRead(0x26) && checkLifeEventBit(MARRIED) && (gWife == ELLI)) {
        setMail(0x26);
    }

    if (!checkMailRead(0x27) && checkLifeEventBit(MARRIED) && (gWife == ANN)) {
        setMail(0x27);
    }

    if (!checkMailRead(0x28) && checkLifeEventBit(MARRIED) && (gWife == KAREN)) {
        setMail(0x28);
    }

    if (!checkMailRead(0x29) && checkLifeEventBit(HAVE_BABY) && (gWife == MARIA)) {
        setMail(0x29);
    }

    if (!checkMailRead(0x2A) && checkLifeEventBit(HAVE_BABY) && (gWife == POPURI)) {
        setMail(0x2A);
    }

    if (!checkMailRead(0x2B) && checkLifeEventBit(HAVE_BABY) && (gWife == ELLI)) {
        setMail(0x2B);
    }

    if (!checkMailRead(0x2C) && checkLifeEventBit(HAVE_BABY) && (gWife == ANN)) {
        setMail(0x2C);
    }

    if (!checkMailRead(0x2D) && checkLifeEventBit(HAVE_BABY) && (gWife == KAREN)) {
        setMail(0x2D);
    }

    if (!checkMailRead(0x2E) && checkDailyEventBit(0x34)) {
        setMail(0x2E);
    }

    if (!checkMailRead(0x2F) && checkDailyEventBit(0x35)) {
        setMail(0x2F);
    }

    if (!checkMailRead(0x30) && checkDailyEventBit(0x37)) {
        setMail(0x30);
    }

    if (!checkMailRead(0x31) && checkDailyEventBit(0x39)) {
        setMail(0x31);
    }

    if (!checkMailRead(0x32) && checkDailyEventBit(WON_CALENDAR_STICKERS_FROM_RAFFLE)) {
        setMail(0x32);
    }

    if (!checkMailRead(0x33) && checkLifeEventBit(HAVE_KITCHEN) && checkLifeEventBit(HAVE_BATHROOM) && checkLifeEventBit(HAVE_STAIRS) && checkLifeEventBit(HAVE_GREENHOUSE) && checkLifeEventBit(HAVE_LOG_TERRACE) && checkLifeEventBit(HAVE_BABY_BED)) {
        setMail(0x33);
    }
    
    if (!checkMailRead(0x34) && checkDailyEventBit(0x36)) {
        setMail(0x34);
    }

    if (!checkMailRead(0x35) && checkDailyEventBit(0x3A)) {
        setMail(0x35);
    }

    if (!checkMailRead(0x36) && checkDailyEventBit(0x3B)) {
        setMail(0x36);
    }

    if (!checkMailRead(0x37) && checkDailyEventBit(0x3C)) {
        setMail(0x37);
    }

    if (!checkMailRead(0x38) && checkDailyEventBit(0x3D)) {
        setMail(0x38);
    }

    if (!checkMailRead(0x39) && checkDailyEventBit(0x3E)) {
        setMail(0x39);
    }

    if (!checkMailRead(0x3A) && ((gYear == 1 && 1 < gSeason) || 1 < gYear)) {
        setMail(0x3A);
    }

    if (!checkMailRead(0x3B) && gSeason == SPRING && gDayOfMonth == 20) {
        setMail(0x3B);
    }

    if (!checkMailRead(0x3D) && gSeason == SPRING && gDayOfMonth == 15) {
        setMail(0x3D);
    }

    if (!checkMailRead(0x3E) && gSeason == SPRING && gDayOfMonth == 19) {
        setMail(0x3E);
    }

    if (!checkMailRead(0x3F) && gSeason == SUMMER && gDayOfMonth == 1) {
        setMail(0x3F);
    }

    if (!checkMailRead(0x40) && !getRandomNumberInRange(0, 20)) {
        setMail(0x40);
    }
    
    if (!checkMailRead(0x41) && gSeason == SPRING && gDayOfMonth == 30) {
        setMail(0x41);
    }

    if (!checkMailRead(0x42) && gSeason == AUTUMN && gDayOfMonth == 30) {
        setMail(0x42);
    }

    if (!checkMailRead(0x43) && gSeason == AUTUMN && gDayOfMonth == 10) {
        setMail(0x43);
    }

    if (!checkMailRead(0x44) && gSeason == AUTUMN && gDayOfMonth == 1) {
        setMail(0x44);
    }

    if (!checkMailRead(0x45) && gSeason == WINTER && gDayOfMonth == 7) {
        setMail(0x45);
    }

    if (!checkMailRead(0x46) && gSeason == WINTER && gDayOfMonth == 6) {
        setMail(0x46);
    }

    if (!checkMailRead(0x47) && gSeason == WINTER && gDayOfMonth == 17) {
        setMail(0x47);
    }

    if (!checkMailRead(0x49) && gYear >= 2 && gSeason == SPRING && gDayOfMonth == 24) {
        setMail(0x49);
    }
    
    if (!checkMailRead(0x4A) && gYear >= 2 && gSeason == SPRING) {
        setMail(0x4A);
    }

    if (!checkMailRead(0x4B) && checkLifeEventBit(MARRIED)) {
        setMail(0x4B);
    }

    if (!checkMailRead(0x4C) && gYear >= 3 && gSeason == SPRING && gDayOfMonth == 3) {
        setMail(0x4C);
    }

}

// D_8011F25C
//INCLUDE_ASM("asm/nonmatchings/game/game", func_80063A2C);

// get text index for letter
u16 func_80063A2C(u8 index) {

    u16 arr[80];
    
    memcpy(arr,  D_8011F25C, 0xA0);

    return arr[index];

}

/* rodata */

// D_8011F228-D_8011FA700

//INCLUDE_RODATA("asm/nonmatchings/game/game", houseExtensionPrices);

static const s16 houseExtensionPrices[6] = { -5000, -3000, -2000, -30000, -7000, -1000 };

//INCLUDE_RODATA("asm/nonmatchings/game/game", houseExtensionLumberCosts);

static const s16 houseExtensionLumberCosts[6] = { -450, -300, -250, -580, -350, -150 };

//INCLUDE_RODATA("asm/nonmatchings/game/game", houseConstructionDays);

static const u8 houseConstructionDays[6] = { 3, 3, 3, 3, 3, 1 };

//INCLUDE_RODATA("asm/nonmatchings/game/game", lifeEventHouseConstructionBits);

static const u16 lifeEventHouseConstructionBits[6] = { 0x10, 0x11, 0x12, 0x13, 0x14, 0x15 };

//INCLUDE_RODATA("asm/nonmatchings/game/game", animalLocationsHouseConstruction);

static const u8 animalLocationsHouseConstruction[6] = { FARM, FARM, HOUSE, FARM, FARM, HOUSE };

// INCLUDE_RODATA("asm/nonmatchings/game/game", D_8011F25C);

// text indices for letters
static const u16 D_8011F25C[79] = { 
     0, 1, 2, 3, 4, 5, 
     6, 7, 8, 9, 10, 11, 
     12, 13, 14, 15, 16, 
     17, 18, 19, 20, 21, 
     22, 23, 24, 25, 26, 
     27, 28, 29, 30, 31, 
     32, 33, 34, 35, 36, 
     36, 36, 36, 36, 36, 
     36, 36, 36, 36, 36, 
     36, 36, 36, 36, 36, 
     38, 39, 39, 39, 39, 
     39, 40, 41, 42, 43, 
     44, 45, 46, 47, 47, 
     48, 49, 50, 51, 52, 
     53, 54, 55, 56, 57,
     0, 0, 0, 0
};

