#include "common.h"

#include "system/controller.h"
#include "system/cutscene.h"
#include "system/dialogue.h"
#include "system/entity.h"
#include "system/globalSprites.h"
#include "system/graphic.h"
#include "system/map.h"
#include "system/mapController.h"
#include "system/math.h"
#include "system/memory.h"
#include "system/message.h"

#include "game/animals.h"
#include "game/cutscenes.h"
#include "game/evaluation.h"
#include "game/fieldObjects.h"
#include "game/game.h"
#include "game/gameAudio.h"
#include "game/gameStatus.h"
#include "game/initialize.h"
#include "game/items.h"
#include "game/level.h"
#include "game/namingScreen.h"
#include "game/npc.h"
#include "game/overlayScreens.h"
#include "game/player.h"
#include "game/shop.h"
#include "game/time.h"
#include "game/transition.h"
#include "game/weather.h"

#include "mainLoop.h"

#include "buffers/buffers.h"

#include "assetIndices/cutscenes.h"
#include "assetIndices/dialogueMenus.h"
#include "assetIndices/dialogues.h"
#include "assetIndices/entities.h"
#include "assetIndices/maps.h"
#include "assetIndices/sfxs.h"
#include "assetIndices/sprites.h"

#include "ld_symbols.h"

// forward declaration
u8 checkEarthquakeShouldHappen();
void recruitSpiritFestivalAssistants(void);

Vec4f globalLightingRGBA;

u8 gHarvestKing;
u8 gFlowerFestivalGoddess;
u8 gVoteForFlowerFestivalGoddess;

u8 gHappiness;

u8 gWife;
u8 gWifePregnancyCounter;
u8 gWifeConceptionCounter;

u8 gBabyAge;

u32 gGold;
u16 gLumber;
u32 dailyShippingBinValue;

u16 gSickDays;
u32 totalFishCaught;

u8 houseExtensionConstructionCounter;

u8 gItemBeingHeld;

u8 gNamingScreenIndex;

u8 gFarmName[6];
u8 gWifeName[6];
u8 gBabyName[6];
u8 gHarvestKingName[6];

u16 gTotalGrassTiles;
u16 gTotalPinkCatMintFlowersGrowing;

u32 gTotalCropsShipped;
u32 gTotalEggsShipped;
u32 gTotalMilkShipped;

u8 popuriGrayBabyAge;

u8 numberOfSpiritFestivalAssistantsRecruited;
u8 spiritFestivalAssistant1;
u8 spiritFestivalAssistant2;
u8 spiritFestivalAssistant3;  
u8 elliJeffBabyAge;

u8 mariaHarrisNewlywedCounter;
u8 mariaHarrisPregnancyCounter;
u8 mariaHarrisBabyAge;
u8 popuriGrayNewlywedCounter;
u8 popuriGrayPregnancyCounter;
u8 elliJeffNewlywedCounter;
u8 elliJeffPregnancyCounter;
u8 karenKaiBabyAge;
u8 karenKaiNewlywedCounter;
u8 karenPregnancyCounter;
u8 annCliffBabyAge;
u8 annCliffNewlywedCounter;
u8 annPregnancyCounter;

u8 gElliGrievingCounter;

u8 anniversarySeason;
u8 anniversaryDate;

u8 babyBirthdaySeason;
u8 babyBirthdayDate;

u8 gHarvestCoinFinder;

u8 gHouseExtensionSelection;
u8 mountainConstructionWorkDays;

u8 gSumGirlsWithHighAffection;
u8 gAverageFarmAnimalAffection;

MainLoopCallbackInfo gameLoopContext;

// unused
u16 D_801FB686;

// unused game dialogue variables
u8 D_8017044B;
u8 D_80180712;
u8 D_801890D8;
u8 D_80189A48;
u8 D_801C3B64;
u8 D_801C4214;

// unused strings
u8 D_801594E0[3];
u8 D_801594E6[3];
u8 D_8016F6E0[3];
u8 D_80170268[6];
u8 D_801806C8[2];
u8 D_80182D90[11][3];
u8 D_801A8B50[8];
u8 D_801FB5C4[6];
u8 D_801FC152[2];
u8 D_801FC156[6];
u8 D_80204B3C[6];
u8 D_80205210[3];
u8 D_8020563B[4];
u8 D_80237380[6];
u8 D_80237418[3];

Vec4f previousLightingRGBA;


// data

// lighting values based on hour

// FIXME: these could be D_80113B20[4][24] or struct { r[24]; g[24]; b[24]; a[24]; 
// TODO: label

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
static const u16 mailTextIndices[79];

static const s16 houseExtensionPrices[6];     
static const s16 houseExtensionLumberCosts[6];

// forward declarations
extern inline void showTextBox(u16 arg0, u16 arg1, u16 arg2, u32 flag, u16 arg4);
extern void showDialogueTextBox(u8);

//INCLUDE_ASM("asm/nonmatchings/game/game", updateFamilyStates);

void updateFamilyStates(void) {

    if (!checkLifeEventBit(MARRIED)) goto not_married;

    gWifeConceptionCounter += adjustValue(gWifeConceptionCounter, 1, 120);
    
    if (checkLifeEventBit(HAVE_BABY) && gBabyAge < 0xFF) {
        gBabyAge++;
    }

    if (!checkLifeEventBit(HAVE_BABY) && checkLifeEventBit(WIFE_PREGNANT) && gWifePregnancyCounter >= 60) {
         
        clearLifeEventBit(WIFE_PREGNANT);
        setLifeEventBit(HAVE_BABY);
        setSpecialDialogueBit(HAVE_BABY_DIALOGUE);
        
         switch (gWife) {        

            case MARIA:                              
                setSpecialDialogueBit(HAVE_BABY_WIFE_MARIA_DIALOGUE);
                break;
            case POPURI:                             
                setSpecialDialogueBit(HAVE_BABY_WIFE_POPURI_DIALOGUE);
                break;
            case ELLI:                                
                setSpecialDialogueBit(HAVE_BABY_WIFE_ELLI_DIALOGUE);
                break;
            case ANN:                      
                setSpecialDialogueBit(HAVE_BABY_WIFE_ANN_DIALOGUE);
                break;
            case KAREN:                    
                setSpecialDialogueBit(HAVE_BABY_WIFE_KAREN_DIALOGUE);
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
                setSpecialDialogueBit(WIFE_PREGNANT_MARIA_DIALOGUE);
                break;
            case POPURI:                               
                setSpecialDialogueBit(WIFE_PREGNANT_POPURI_DIALOGUE);
                break;
            case ELLI:                                
                setSpecialDialogueBit(WIFE_PREGNANT_ELLI_DIALOGUE);
                break;
            case ANN:                             
                setSpecialDialogueBit(WIFE_PREGNANT_ANN_DIALOGUE);
                break;
            case KAREN:                               
                setSpecialDialogueBit(WIFE_PREGNANT_KAREN_DIALOGUE);
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
            clearLifeEventBit(MARIA_PREGNANT);
            setLifeEventBit(MARIA_HARRIS_HAVE_BABY);
            setSpecialDialogueBit(MARIA_HARRIS_HAVE_BABY_DIALOGUE);
            clearSpecialDialogueBit(MARIA_PREGNANT_DIALOGUE);
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
            clearLifeEventBit(POPURI_PREGNANT);
            setLifeEventBit(POPURI_GRAY_HAVE_BABY);
            setSpecialDialogueBit(POPURI_GRAY_HAVE_BABY_DIALOGUE);
            clearSpecialDialogueBit(POPURI_PREGNANT_DIALOGUE);
        }

        if (!checkLifeEventBit(POPURI_GRAY_HAVE_BABY) && checkLifeEventBit(POPURI_PREGNANT)) {
            popuriGrayPregnancyCounter++;
        }

        if (!checkLifeEventBit(POPURI_GRAY_HAVE_BABY) && !checkLifeEventBit(POPURI_PREGNANT) && npcAffection[GRAY] >= 250 && popuriGrayNewlywedCounter >= 30) {
            setLifeEventBit(POPURI_PREGNANT);
            popuriGrayPregnancyCounter = 0;
            setSpecialDialogueBit(POPURI_PREGNANT_DIALOGUE);
        }

    }

    if (checkLifeEventBit(ELLI_JEFF_MARRIED)) {

        elliJeffNewlywedCounter += adjustValue(elliJeffNewlywedCounter, 1, 120);

        if (checkLifeEventBit(ELLI_JEFF_HAVE_BABY) && elliJeffBabyAge < 120) {
            elliJeffBabyAge++;
        }

        if (!checkLifeEventBit(ELLI_JEFF_HAVE_BABY) && checkLifeEventBit(ELLI_PREGNANT) && elliJeffPregnancyCounter >= 60) {
            clearLifeEventBit(ELLI_PREGNANT);
            setLifeEventBit(ELLI_JEFF_HAVE_BABY);
            setSpecialDialogueBit(ELLI_JEFF_HAVE_BABY_DIALOGUE);
            clearSpecialDialogueBit(ELLI_PREGNANT_DIALOGUE);
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
            clearLifeEventBit(ANN_PREGNANT);
            setLifeEventBit(ANN_CLIFF_HAVE_BABY);
            setSpecialDialogueBit(ANN_CLIFF_HAVE_BABY_DIALOGUE);
            clearSpecialDialogueBit(ANN_PREGNANT_DIALOGUE);
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
            clearLifeEventBit(KAREN_PREGNANT);
            setLifeEventBit(KAREN_KAI_HAVE_BABY);
            setSpecialDialogueBit(KAREN_KAI_HAVE_BABY_DIALOGUE);
            clearSpecialDialogueBit(KAREN_PREGNANT_DIALOGUE);
        }

        if (!checkLifeEventBit(KAREN_KAI_HAVE_BABY) && checkLifeEventBit(KAREN_PREGNANT)) {
            karenPregnancyCounter++;
        }

        if (!checkLifeEventBit(KAREN_KAI_HAVE_BABY) && !checkLifeEventBit(KAREN_PREGNANT) && npcAffection[KAI] >= 250 && karenKaiNewlywedCounter >= 30) {
            setLifeEventBit(KAREN_PREGNANT);
            karenPregnancyCounter = 0;
            setSpecialDialogueBit(KAREN_PREGNANT_DIALOGUE);
        }

    }

}

//INCLUDE_ASM("asm/nonmatchings/game/game", decrementFamilyAndPetAffection);

void decrementFamilyAndPetAffection(void) {

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

//INCLUDE_ASM("asm/nonmatchings/game/game", setSpecialDialogues);

void setSpecialDialogues(void) {

    if (getTotalCowsCount()) {
        setSpecialDialogueBit(HAVE_COW_DIALOGUE);
    } else {
        clearSpecialDialogueBit(HAVE_COW_DIALOGUE);
    }

    if (getTotalSheepCount()) {
        setSpecialDialogueBit(HAVE_SHEEP_DIALOGUE);
    } else {
        clearSpecialDialogueBit(HAVE_SHEEP_DIALOGUE);
    }

    if (getTotalChickenCount()) {
        setSpecialDialogueBit(HAVE_CHICKEN_DIALOGUE);
    } else {
        clearSpecialDialogueBit(HAVE_CHICKEN_DIALOGUE);
    }

    if (checkLifeEventBit(HAVE_HORSE)) {
        setSpecialDialogueBit(HAVE_HORSE_DIALOGUE);
    }

    if (gForecast == RAIN) {
        setSpecialDialogueBit(RAIN_FORECAST_DIALOGUE);
    } else {
        clearSpecialDialogueBit(RAIN_FORECAST_DIALOGUE);
    }

    if (checkLifeEventBit(HAVE_KITCHEN)) {
        setSpecialDialogueBit(HAVE_KITCHEN_DIALOGUE);
    }

    if (checkLifeEventBit(HAVE_BATHROOM)) {
        setSpecialDialogueBit(HAVE_BATHROOM_DIALOGUE);
    }

    if (checkLifeEventBit(HAVE_STAIRS)) {
        setSpecialDialogueBit(HAVE_STAIRS_DIALOGUE);
    }

    if (checkLifeEventBit(HAVE_GREENHOUSE)) {
        setSpecialDialogueBit(HAVE_GREENHOUSE_DIALOGUE);
    }

    // spirit festival
    if (gSeason == WINTER && gDayOfMonth == 27) {

        if (gHarvestKing == PLAYER) {
            setSpecialDialogueBit(PLAYER_HARVEST_KING_SPIRIT_FESTIVAL_ASSISTANTS_DIALOGUE);
        }

        clearSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_MARIA_DIALOGUE);
        clearSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_POPURI_DIALOGUE);
        clearSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_ELLI_DIALOGUE);
        clearSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_ANN_DIALOGUE);
        clearSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_KAREN_DIALOGUE);
        clearSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_HARRIS_DIALOGUE);
        clearSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_GRAY_DIALOGUE);
        clearSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_JEFF_DIALOGUE);
        clearSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_CLIFF_DIALOGUE);
        clearSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_KAI_DIALOGUE);

        numberOfSpiritFestivalAssistantsRecruited = 0;
        spiritFestivalAssistant1 = 0;
        spiritFestivalAssistant2 = 0;
        spiritFestivalAssistant3 = 0;

    } else {
        clearSpecialDialogueBit(PLAYER_HARVEST_KING_SPIRIT_FESTIVAL_ASSISTANTS_DIALOGUE);
    }

    if (gSeason == WINTER && gDayOfMonth == 10) {
        setSpecialDialogueBit(THANKSGIVING_DIALOGUE);
    } else {
        clearSpecialDialogueBit(THANKSGIVING_DIALOGUE);
    }

    if (gSeason == WINTER && gDayOfMonth == 30) {
        setSpecialDialogueBit(NEW_YEARS_EVE_DIALOGUE);
    } else {
        clearSpecialDialogueBit(NEW_YEARS_EVE_DIALOGUE);
    }

    if (gSeason == WINTER && gDayOfMonth == 24) {
        setSpecialDialogueBit(STARRY_NIGHT_FESTIVAL_DIALOGUE);
    } else {
        clearSpecialDialogueBit(STARRY_NIGHT_FESTIVAL_DIALOGUE);
    }

    if (gSeason == WINTER && gDayOfMonth == 11) {
        setSpecialDialogueBit(MARIA_BIRTHDAY_DIALOGUE);
    } else {
        clearSpecialDialogueBit(MARIA_BIRTHDAY_DIALOGUE);
    }

    if (gSeason == SPRING && gDayOfMonth == 22) {
        setSpecialDialogueBit(POPURI_BIRTHDAY_DIALOGUE);
    } else {
        clearSpecialDialogueBit(POPURI_BIRTHDAY_DIALOGUE);
    }

    if (gSeason == AUTUMN && gDayOfMonth == 1) {
        setSpecialDialogueBit(ELLI_BIRTHDAY_DIALOGUE);
    } else {
        clearSpecialDialogueBit(ELLI_BIRTHDAY_DIALOGUE);
    }

    if (gSeason == SUMMER && gDayOfMonth == 14) {
        setSpecialDialogueBit(ANN_BIRTHDAY_DIALOGUE);
    } else {
        clearSpecialDialogueBit(ANN_BIRTHDAY_DIALOGUE);
    }

    if (gSeason == WINTER && gDayOfMonth == 29) {
        setSpecialDialogueBit(KAREN_BIRTHDAY_DIALOGUE);
    }
    else {
        clearSpecialDialogueBit(KAREN_BIRTHDAY_DIALOGUE);
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/game", clearNPCAlternateLocationDialogueBits);

void clearNPCAlternateLocationDialogueBits(void) {

    clearSpecialDialogueBit(HARRIS_AT_TAVERN_DIALOGUE);
    clearSpecialDialogueBit(POPURI_AT_RANCH_DIALOGUE);
    clearSpecialDialogueBit(POPURI_AT_MOUNTAIN_2_DIALOGUE);
    clearSpecialDialogueBit(POPURI_AT_MOON_MOUNTAIN_DIALOGUE);
    clearSpecialDialogueBit(POPURI_AT_BAKERY_DIALOGUE);
    clearSpecialDialogueBit(POPURI_AT_BEACH_DIALOGUE);
    clearSpecialDialogueBit(ELLI_AT_MOUNTAIN_1_DIALOGUE);
    clearSpecialDialogueBit(ELLI_AT_MOUNTAIN_2_DIALOGUE);
    clearSpecialDialogueBit(ELLI_AT_FLOWER_SHOP_DIALOGUE);
    clearSpecialDialogueBit(ELLI_AT_BEACH_DIALOGUE);
    clearSpecialDialogueBit(ANN_AT_MOUNTAIN_DIALOGUE);
    clearSpecialDialogueBit(ANN_AT_BEACH_DIALOGUE);
    clearSpecialDialogueBit(ANN_AT_VINEYARD_DIALOGUE);
    clearSpecialDialogueBit(ANN_AT_RICK_STORE_DIALOGUE);
    clearSpecialDialogueBit(KAREN_AT_BEACH_DIALOGUE);
    clearSpecialDialogueBit(KAREN_AT_MOUNTAIN_DIALOGUE);
    clearSpecialDialogueBit(KAREN_AT_TAVERN_DIALOGUE);
    clearSpecialDialogueBit(MARIA_AT_BEACH_DIALOGUE);
    clearSpecialDialogueBit(GRAY_AT_TAVERN_DIALOGUE);
    clearSpecialDialogueBit(JEFF_AT_TAVERN_DIALOGUE);
    clearSpecialDialogueBit(CLIFF_AT_TAVERN_DIALOGUE);
    clearSpecialDialogueBit(KAI_AT_TAVERN_DIALOGUE);
    clearSpecialDialogueBit(MARIA_AT_MOUNTAIN_DIALOGUE);
    clearSpecialDialogueBit(KENT_AT_MOUNTAIN_DIALOGUE);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/game", resetDailyBits);

void resetDailyBits(void) {
    
    u16 i;

    for (i = 0; i < 32; i++) {
        dailyEventBits[i] = 0;
    }
    
    // clear greeted NPC dialogues
    clearSpecialDialogueBit(GREETED_MARIA_DIALOGUE);
    clearSpecialDialogueBit(GREETED_POPURI_DIALOGUE);
    clearSpecialDialogueBit(GREETED_ELLI_DIALOGUE);
    clearSpecialDialogueBit(GREETED_ANN_DIALOGUE);
    clearSpecialDialogueBit(GREETED_KAREN_DIALOGUE);
    clearSpecialDialogueBit(GREETED_HARRIS_DIALOGUE);
    clearSpecialDialogueBit(GREETED_GRAY_DIALOGUE);
    clearSpecialDialogueBit(GREETED_JEFF_DIALOGUE);
    clearSpecialDialogueBit(GREETED_CLIFF_DIALOGUE);
    clearSpecialDialogueBit(GREETED_KAI_DIALOGUE);
    clearSpecialDialogueBit(GREETED_MAYOR_DIALOGUE);
    clearSpecialDialogueBit(GREETED_MAYOR_WIFE_DIALOGUE);
    clearSpecialDialogueBit(GREETED_LILLIA_DIALOGUE);
    clearSpecialDialogueBit(GREETED_BASIL_DIALOGUE);
    clearSpecialDialogueBit(GREETED_ELLEN_DIALOGUE);
    clearSpecialDialogueBit(GREETED_PASTOR_DIALOGUE);
    clearSpecialDialogueBit(GREETED_RICK_DIALOGUE);
    clearSpecialDialogueBit(GREETED_SAIBARA_DIALOGUE);
    clearSpecialDialogueBit(GREETED_POTION_SHOP_DEALER_DIALOGUE);
    clearSpecialDialogueBit(GREETED_KENT_DIALOGUE);
    clearSpecialDialogueBit(GREETED_STU_DIALOGUE);
    clearSpecialDialogueBit(GREETED_MIDWIFE_DIALOGUE);
    clearSpecialDialogueBit(GREETED_MAY_DIALOGUE);
    clearSpecialDialogueBit(GREETED_ASSITANT_CARPENTER_1_DIALOGUE);
    clearSpecialDialogueBit(GREETED_ASSITANT_CARPENTER_2_DIALOGUE);
    clearSpecialDialogueBit(GREETED_MASTER_CARPENTER_DIALOGUE);
    clearSpecialDialogueBit(GREETED_HARVEST_SPRITE_DIALOGUE);
    clearSpecialDialogueBit(GREETED_SYDNEY_DIALOGUE);
    clearSpecialDialogueBit(GREETED_BARLEY_DIALOGUE);
    clearSpecialDialogueBit(GREETED_GREG_DIALOGUE);
    clearSpecialDialogueBit(GREETED_BABY_DIALOGUE);

    // cut/unused NPC
    clearSpecialDialogueBit(255);

    clearSpecialDialogueBit(GREETED_DOUG_DIALOGUE);
    clearSpecialDialogueBit(GREETED_GOTZ_DIALOGUE);
    clearSpecialDialogueBit(GREETED_SASHA_DIALOGUE);
    clearSpecialDialogueBit(GREETED_SHIPPER_DIALOGUE);
    clearSpecialDialogueBit(GREETED_DUKE_DIALOGUE);

    // clear gave gift
    clearSpecialDialogueBit(GREETED_GOURMET_JUDGE_DIALOGUE);
    clearSpecialDialogueBit(GAVE_GIFT_MARIA_DIALOGUE);
    clearSpecialDialogueBit(GAVE_GIFT_POPURI_DIALOGUE);
    clearSpecialDialogueBit(GAVE_GIFT_ELLI_DIALOGUE);
    clearSpecialDialogueBit(GAVE_GIFT_ANN_DIALOGUE);
    clearSpecialDialogueBit(GAVE_GIFT_KAREN_DIALOGUE);
    clearSpecialDialogueBit(GAVE_GIFT_HARRIS_DIALOGUE);
    clearSpecialDialogueBit(GAVE_GIFT_GRAY_DIALOGUE);
    clearSpecialDialogueBit(GAVE_GIFT_JEFF_DIALOGUE);
    clearSpecialDialogueBit(GAVE_GIFT_CLIFF_DIALOGUE);
    clearSpecialDialogueBit(GAVE_GIFT_KAI_DIALOGUE);
    clearSpecialDialogueBit(GAVE_GIFT_MAYOR_DIALOGUE);
    clearSpecialDialogueBit(GAVE_GIFT_MAYOR_WIFE_DIALOGUE);
    clearSpecialDialogueBit(GAVE_GIFT_LILLIA_DIALOGUE);
    clearSpecialDialogueBit(GAVE_GIFT_BASIL_DIALOGUE);
    clearSpecialDialogueBit(GAVE_GIFT_ELLEN_DIALOGUE);
    clearSpecialDialogueBit(GAVE_GIFT_PASTOR_DIALOGUE);
    clearSpecialDialogueBit(GAVE_GIFT_RICK_DIALOGUE);
    clearSpecialDialogueBit(GAVE_GIFT_SAIBARA_DIALOGUE);
    clearSpecialDialogueBit(GAVE_GIFT_POTION_SHOP_DEALER_DIALOGUE);
    clearSpecialDialogueBit(GAVE_GIFT_KENT_DIALOGUE);
    clearSpecialDialogueBit(GAVE_GIFT_STU_DIALOGUE);
    clearSpecialDialogueBit(GAVE_GIFT_MIDWIFE_DIALOGUE);
    clearSpecialDialogueBit(GAVE_GIFT_MAY_DIALOGUE);
    clearSpecialDialogueBit(GAVE_GIFT_ASSITANT_CARPENTER_1_DIALOGUE);
    clearSpecialDialogueBit(GAVE_GIFT_ASSITANT_CARPENTER_2_DIALOGUE);
    clearSpecialDialogueBit(GAVE_GIFT_MASTER_CARPENTER_DIALOGUE);
    clearSpecialDialogueBit(GAVE_GIFT_HARVEST_SPRITE_DIALOGUE);
    clearSpecialDialogueBit(GAVE_GIFT_SYDNEY_DIALOGUE);
    clearSpecialDialogueBit(GAVE_GIFT_BARLEY_DIALOGUE);
    clearSpecialDialogueBit(GAVE_GIFT_GREG_DIALOGUE);
    clearSpecialDialogueBit(GAVE_GIFT_BABY_DIALOGUE);

    // cut/unused NPC
    clearSpecialDialogueBit(292);

    clearSpecialDialogueBit(GAVE_GIFT_DOUG_DIALOGUE);
    clearSpecialDialogueBit(GAVE_GIFT_GOTZ_DIALOGUE);
    clearSpecialDialogueBit(GAVE_GIFT_SASHA_DIALOGUE);
    clearSpecialDialogueBit(GAVE_GIFT_SHIPPER_DIALOGUE);
    clearSpecialDialogueBit(GAVE_GIFT_DUKE_DIALOGUE);
    clearSpecialDialogueBit(GAVE_GIFT_GOURMET_JUDGE_DIALOGUE);

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

static inline void pauseGameplay_2(void) {
    pauseEntities();
    pauseAllCutsceneExecutors();
    setEntityMapSpaceIndependent(ENTITY_PLAYER, FALSE);
}

static inline void openOverlayScreen_2(void) {
    pauseAllEntityLoads();
    pauseAllCutsceneExecutors();
    setEntityMapSpaceIndependent(ENTITY_PLAYER, FALSE);
    unloadMapAssets(MAIN_MAP_INDEX);
}


//INCLUDE_ASM("asm/nonmatchings/game/game", showTextBox);

inline void showTextBox(u16 messageBoxType, u16 textBankIndex, u16 textIndex, u32 flag, u16 flags) {
  
    pauseGameplay_2();
    
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

    setMessageBoxInterpolationWithFlags(0, -4, flags);
  
    initializeMessageBox(MAIN_MESSAGE_BOX_INDEX, textBankIndex, textIndex, flag);
  
    setMainLoopCallbackFunctionIndex(MESSAGE_BOX);
    
    controllers[CONTROLLER_1].buttonPressed = 0;

    setPlayerAction(CONTROLLER_INPUT, ANIM_DEFAULT);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/game", showMessageBox);

inline void showMessageBox(u16 arg0, u16 dialogueBytecodeAddressesIndex, u16 dialogueIndex, u32 flag, u16 messageBoxFlags) {
    
    pauseGameplay_2();
    
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

    setMessageBoxInterpolationWithFlags(0, -4, messageBoxFlags);
    initializeDialogueSession(0, dialogueBytecodeAddressesIndex, dialogueIndex, flag);

    setMainLoopCallbackFunctionIndex(DIALOGUE);

    controllers[CONTROLLER_1].buttonPressed = 0;
    
    setPlayerAction(CONTROLLER_INPUT, ANIM_DEFAULT);

}

//INCLUDE_ASM("asm/nonmatchings/game/game", showDialogueTextBox);

void showDialogueTextBox(u8 dialogueMenuIndex) {

    gameLoopContext.dialogueMenuIndex = dialogueMenuIndex;

    switch (dialogueMenuIndex) {

        // diary menus
        case DIALOGUE_MENU_DIARY_ROOT:
            showMessageBox(1, DIALOGUE_DIARY, 0, 0, 2);
            break;
        case DIALOGUE_MENU_DIARY_SAVE_PROMPT:
            showMessageBox(1, DIALOGUE_DIARY, 1, 0, 2);
            break;
            
        // shop selection
        case DIALOGUE_MENU_BAKERY_FOOD:
            showMessageBox(0, DIALOGUE_SHOP, DIALOGUE_SHOP_BAKERY_DRINK_MENU, 0x40, 0);
            break;         
        case DIALOGUE_MENU_BAKERY_DRINK_MENU:
            showMessageBox(0, DIALOGUE_SHOP, DIALOGUE_SHOP_BAKERY_FOOD_MENU, 0x40, 0);
            break;          
        case DIALOGUE_MENU_TAVERN_MENU:
            showMessageBox(0, DIALOGUE_SHOP, DIALOGUE_SHOP_TAVERN_MENU, 0x40, 0);
            break;       
        case DIALOGUE_MENU_RANCH_STORE_ANIMALS_ROOT:
            showMessageBox(0, DIALOGUE_SHOP, DIALOGUE_SHOP_RANCH_STORE_MAIN_MENU, 0x40, 0);
            break;        
        case DIALOGUE_MENU_RANCH_STORE_ANIMAL_BUY:
            showMessageBox(0, DIALOGUE_SHOP, DIALOGUE_SHOP_RANCH_STORE_BUY_MENU, 0x40, 0);
            break;      
        case DIALOGUE_MENU_RANCH_STORE_ANIMAL_SELL_CONFIRM:
            showMessageBox(0, DIALOGUE_SHOP, DIALOGUE_SHOP_RANCH_STORE_SELL_CONFIRM, 0x40, 0);
            break;      

        // festivals
        case DIALOGUE_MENU_COW_FESTIVAL_ENTRY_CONFIRM:
            showMessageBox(0, DIALOGUE_FESTIVAL_OVERLAY_SELECTIONS, DIALOGUE_FESTIVAL_OVERLAY_COW_ENTRY, 0x40, 0);
            break;      
        case DIALOGUE_MENU_FLOWER_FESTIVAL_VOTE:
            showMessageBox(0, DIALOGUE_FESTIVAL_OVERLAY_SELECTIONS, DIALOGUE_FESTIVAL_OVERLAY_FLOWER_VOTE, 0x40, 0);
            break;    
        case DIALOGUE_MENU_SQUARE_STALL:
            showMessageBox(0, DIALOGUE_FESTIVAL_OVERLAY_SELECTIONS, DIALOGUE_FESTIVAL_OVERLAY_SQUARE_STALL, 0x40, 0);
            break;
        case DIALOGUE_MENU_HORSE_RACE_STALL:
            showMessageBox(0, DIALOGUE_FESTIVAL_OVERLAY_SELECTIONS, DIALOGUE_FESTIVAL_OVERLAY_HORSE_RACE_STALL, 0x40, 0);
            break;   
        case DIALOGUE_MENU_DOG_RACE_STALL:
            showMessageBox(0, DIALOGUE_FESTIVAL_OVERLAY_SELECTIONS, DIALOGUE_FESTIVAL_OVERLAY_DOG_RACE_STALL, 0x40, 0);
            break;
            
        // library
        case DIALOGUE_MENU_LIBRARY_BOOKSHELF_1:
            if (gSeason == SPRING || gSeason == WINTER) {
                showMessageBox(1, DIALOGUE_LIBRARY, DIALOGUE_LIBRARY_BOOKSHELF_1_SPRING_WINTER, 0, 2);
            } else {
                showMessageBox(1, DIALOGUE_LIBRARY, DIALOGUE_LIBRARY_BOOKSHELF_1_SUMMER_AUTUMN, 0, 2);
            }            
            break;
        case DIALOGUE_MENU_LIBRARY_BOOKSHELF_2:
            if (gSeason == SPRING || gSeason == WINTER) {
                showMessageBox(1, DIALOGUE_LIBRARY, DIALOGUE_LIBRARY_BOOKSHELF_2_SPRING_WINTER, 0x40, 2);
            } else {
                showMessageBox(1, DIALOGUE_LIBRARY, DIALOGUE_LIBRARY_BOOKSHELF_2_SUMMER_AUTUMN, 0, 2);
            }
            break;
        case DIALOGUE_MENU_LIBRARY_BOOKSHELF_3:
            if (gSeason == SPRING || gSeason == WINTER) {
                showMessageBox(1, DIALOGUE_LIBRARY, DIALOGUE_LIBRARY_BOOKSHELF_3_SPRING_WINTER, 0x40, 2);
            } else {
                showMessageBox(1, DIALOGUE_LIBRARY, DIALOGUE_LIBRARY_BOOKSHELF_3_SUMMER_AUTUMN, 0, 2);
            }
            break; 
        case DIALOGUE_MENU_LIBRARY_BOOKSHELF_4:
            if (gSeason == SPRING || gSeason == WINTER) {
                showMessageBox(1, DIALOGUE_LIBRARY, DIALOGUE_LIBRARY_BOOKSHELF_4_SPRING_WINTER, 0x40, 2);
            } else {
                showMessageBox(1, DIALOGUE_LIBRARY, DIALOGUE_LIBRARY_BOOKSHELF_4_SUMMER_AUTUMN, 0, 2);
            }
            break;
        case DIALOGUE_MENU_LIBRARY_BOOKSHELF_5:
            if (gSeason == SPRING || gSeason == WINTER) {
                showMessageBox(1, DIALOGUE_LIBRARY, DIALOGUE_LIBRARY_BOOKSHELF_5_SPRING_WINTER, 0x40, 2);
            } else {
                showMessageBox(1, DIALOGUE_LIBRARY, DIALOGUE_LIBRARY_BOOKSHELF_5_SUMMER_AUTUMN, 0, 2);
            }
            break;
        case DIALOGUE_MENU_LIBRARY_BOOKSHELF_6:
            if (gSeason == SPRING || gSeason == WINTER) {
                showMessageBox(1, DIALOGUE_LIBRARY, DIALOGUE_LIBRARY_BOOKSHELF_6_SPRING_WINTER, 0x40, 2);
            } else {
                showMessageBox(1, DIALOGUE_LIBRARY, DIALOGUE_LIBRARY_BOOKSHELF_6_SUMMER_AUTUMN, 0, 2);
            }
            break;
        case DIALOGUE_MENU_LIBRARY_BOOKSHELF_7:
            if (gSeason == SPRING || gSeason == WINTER) {
                showMessageBox(1, DIALOGUE_LIBRARY, DIALOGUE_LIBRARY_BOOKSHELF_7_SPRING_WINTER, 0x40, 2);
            } else {
                showMessageBox(1, DIALOGUE_LIBRARY, DIALOGUE_LIBRARY_BOOKSHELF_7_SUMMER_AUTUMN, 0, 2);
            }
            break;
        case DIALOGUE_MENU_LIBRARY_BOOKSHELF_8:
            if (gSeason == SPRING || gSeason == WINTER) {
                showMessageBox(1, DIALOGUE_LIBRARY, DIALOGUE_LIBRARY_BOOKSHELF_8_SPRING_WINTER, 0x40, 2);
            } else {
                showMessageBox(1, DIALOGUE_LIBRARY, DIALOGUE_LIBRARY_BOOKSHELF_8_SUMMER_AUTUMN, 0, 2);
            }
            break;
        case DIALOGUE_MENU_LIBRARY_BOOKSHELF_9:
            if (gSeason == SPRING || gSeason == WINTER) {
                showMessageBox(1, DIALOGUE_LIBRARY, DIALOGUE_LIBRARY_BOOKSHELF_9_SPRING_WINTER, 0x40, 2);
            } else {
                showMessageBox(1, DIALOGUE_LIBRARY, DIALOGUE_LIBRARY_BOOKSHELF_9_SUMMER_AUTUMN, 0, 2);
            }
            break;
        case DIALOGUE_MENU_LIBRARY_BOOKSHELF_10:
            if (gSeason == SPRING || gSeason == WINTER) {
                showMessageBox(1, DIALOGUE_LIBRARY, DIALOGUE_LIBRARY_BOOKSHELF_10_SPRING_WINTER, 0x40, 2);
            } else {
                showMessageBox(1, DIALOGUE_LIBRARY, DIALOGUE_LIBRARY_BOOKSHELF_10_SUMMER_AUTUMN, 0, 2);
            }
            break;
        
        // continue after ending
        case DIALOGUE_MENU_EVALUATION_CONTINUE_PROMPT:
            showMessageBox(1, DIALOGUE_DIARY, 21, 0, 2);
            break;

    }

    setMainLoopCallbackFunctionIndex(DIALOGUE_SELECTION);

}

//INCLUDE_ASM("asm/nonmatchings/game/game", setMapAudioAndLighting);

void setMapAudioAndLighting(void) {
    
    // ?
    if (gCutsceneCompletionFlags < 0) {
        setMainLoopCallbackFunctionIndex(MAIN_GAME);
    } else {
        
        if (!checkDailyEventBit(CUTSCENE_AUDIO_OVERRIDE)) {
            setLevelAudio(gBaseMapIndex, gSeason, gHour);
            //setLevelAudio(currentMapContext.currentMapIndex, gSeason, gHour);
        }
         
        // set lighting for level based on weather
        setLevelLighting(8, 1);
    
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/game", setLevelLighting);

void setLevelLighting(s16 rate, u16 callbackFunctionIndex) {

    Vec4f vec;
    
    if (getLevelFlags(gBaseMapIndex) & LEVEL_WEATHER_LIGHTING) {
        
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
        getDefaultLevelLighting(&globalLightingRGBA, gBaseMapIndex);
    }
    
    if (rate == 0) {
        
        setEntitiesRGBA(globalLightingRGBA.r, globalLightingRGBA.g, globalLightingRGBA.b, globalLightingRGBA.a);
        setMapControllerRGBA(MAIN_MAP_INDEX, globalLightingRGBA.r, globalLightingRGBA.g, globalLightingRGBA.b, globalLightingRGBA.a);
        
    } else if (globalLightingRGBA.r != previousLightingRGBA.r || globalLightingRGBA.g != previousLightingRGBA.g || globalLightingRGBA.b != previousLightingRGBA.b || globalLightingRGBA.a != previousLightingRGBA.a) {
        
        setEntitiesRGBAWithTransition(globalLightingRGBA.r, globalLightingRGBA.g, globalLightingRGBA.b, globalLightingRGBA.a, rate);
        setMapControllerRGBAWithTransition(0, globalLightingRGBA.r, globalLightingRGBA.g, globalLightingRGBA.b, globalLightingRGBA.a, rate);
        
        if (!checkDailyEventBit(CUTSCENE_AUDIO_OVERRIDE)) {
            setAudioSequenceVolume(gCurrentAudioSequenceIndex, gAudioSequenceVolume);
        }

    }

    previousLightingRGBA.r = globalLightingRGBA.r;
    previousLightingRGBA.g = globalLightingRGBA.g;
    previousLightingRGBA.b = globalLightingRGBA.b;
    previousLightingRGBA.a = globalLightingRGBA.a;

    gameLoopContext.callbackIndex = callbackFunctionIndex;
    
    if (callbackFunctionIndex) {
        setMainLoopCallbackFunctionIndex(LEVEL_LOAD);
        togglePauseEntities();
        resumeCutsceneExecutors();
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/game", handleExitLevel);

// arg0 = unused
inline void handleExitLevel(u16 arg0, u16 callbackIndex) {
    
    // fade out map and entities
    setMapControllerRGBAWithTransition(MAIN_MAP_INDEX, 0, 0, 0, 0, 8);
    setEntitiesRGBAWithTransition(0, 0, 0, 0, 8);
       
    stopAudioSequenceWithDefaultFadeOut(gCurrentAudioSequenceIndex);
    
    gameLoopContext.callbackIndex = callbackIndex;

    resetGlobalLighting();
    
    if (gameLoopContext.callbackIndex) {
        setMainLoopCallbackFunctionIndex(EXIT_LEVEL);
        pauseGameplay_2();
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/game", loadOverlayScreen);

void loadOverlayScreen(u16 delayedFramesCount, u16 callbackFunctionIndex) {
    gameLoopContext.delayedFramesCount = delayedFramesCount;
    gameLoopContext.frameCount = 0;
    gameLoopContext.callbackIndex = callbackFunctionIndex;
    setMainLoopCallbackFunctionIndex(OVERLAY_SCREEN_LOAD);
}

//INCLUDE_ASM("asm/nonmatchings/game/game", levelLoadCallback);

void levelLoadCallback(void) {

    handleCutsceneCompletion();

    if (checkMapRGBADone(MAIN_MAP_INDEX)) {
        setMainLoopCallbackFunctionIndex(gameLoopContext.callbackIndex);
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/game", exitLevelCallback);

void exitLevelCallback(void) {

    handleCutsceneCompletion();

    if (checkMapRGBADone(MAIN_MAP_INDEX) && checkDefaultSequenceChannelOpen(gCurrentAudioSequenceIndex)) {

        setEntitiesRGBA(0, 0, 0, 0);
        setMapControllerRGBA(MAIN_MAP_INDEX, 0, 0, 0, 0);
        deactivateCutsceneExecutors();
        initializeCutsceneExecutors();

        gCutsceneCompletionFlags = 0;
        gCutsceneFlags = 0;

        setMainLoopCallbackFunctionIndex(gameLoopContext.callbackIndex);

    }

}

//INCLUDE_ASM("asm/nonmatchings/game/game", setOverlayScreenCallbackWithDelay);

void setOverlayScreenCallbackWithDelay(void) {

    if (gameLoopContext.frameCount >= gameLoopContext.delayedFramesCount) {
        setMainLoopCallbackFunctionIndex(gameLoopContext.callbackIndex);
    } else {
        gameLoopContext.frameCount++;
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/game", handleRotationCallback);

void handleRotationCallback(void) {

    if (!(mapControllers[MAIN_MAP_INDEX].flags & (MAP_CONTROLLER_ROTATING_COUNTERCLOCKWISE | MAP_CONTROLLER_ROTATING_CLOCKWISE))) {
        togglePauseEntities();
        resumeCutsceneExecutors();
        setEntityMapSpaceIndependent(ENTITY_PLAYER, TRUE);
        setMainLoopCallbackFunctionIndex(MAIN_GAME);
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/game", handleDialogueCallback);

// main loop callback for finishing dialogue
void handleDialogueCallback(void) {
    
    if (checkAllDialoguesCompleted()) {
        
        if (!(getItemFlags(gPlayer.heldItem) & (ITEM_SMALL_ANIMAL | ITEM_BABY | ITEM_CRIB_PLACEABLE | ITEM_TOOL))) {
            setItemState(gPlayer.itemInfoIndex, 1);
            gPlayer.heldItem = 0;
            gItemBeingHeld = 0xFF;
        }
         
        setOverlayIconSprite(0, 0x78, &_dialogueButtonIconsTextureSegmentRomStart, &_dialogueButtonIconsTextureSegmentRomEnd, &_dialogueButtonIconsAssetsIndexSegmentRomStart, &_dialogueButtonIconsAssetsIndexSegmentRomEnd, (u8*)DIALOGUE_ICON_TEXTURE_BUFFER, (u16*)DIALOGUE_ICON_PALETTE_BUFFER, (AnimationFrameMetadata*)DIALOGUE_ICON_ANIMATION_FRAME_METADATA_BUFFER, (u32*)DIALOGUE_ICON_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, 4, 0xFE, 106.0f, -15.0f, 0.0f);
        setOverlayIconSprite(1, 0x78, &_dialogueButtonIconsTextureSegmentRomStart, &_dialogueButtonIconsTextureSegmentRomEnd, &_dialogueButtonIconsAssetsIndexSegmentRomStart, &_dialogueButtonIconsAssetsIndexSegmentRomEnd, (u8*)DIALOGUE_ICON_TEXTURE_BUFFER, (u16*)DIALOGUE_ICON_PALETTE_BUFFER, (AnimationFrameMetadata*)DIALOGUE_ICON_ANIMATION_FRAME_METADATA_BUFFER, (u32*)DIALOGUE_ICON_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, 0xD, 0xFE, 106.0f, -15.0f, 0.0f);
       
        // update stuff after closing dialogue 
        func_8005CDCC();
        
        setMainLoopCallbackFunctionIndex(MAIN_GAME);
        
        togglePauseEntities();
        resumeCutsceneExecutors();
        setEntityMapSpaceIndependent(ENTITY_PLAYER, TRUE);

    }

}

//INCLUDE_ASM("asm/nonmatchings/game/game", func_8005CDCC);

// update stuff after closing dialogue/overlay screen
void func_8005CDCC(void) {
    
    npcAffection[HARVEST_SPRITE_2] = npcAffection[HARVEST_SPRITE_1];
    npcAffection[HARVEST_SPRITE_3] = npcAffection[HARVEST_SPRITE_1];
    
    clearSpecialDialogueBit(USED_BLUE_FEATHER_DIALOGUE);

    recruitSpiritFestivalAssistants();

    setRecipes();
    
    if (checkSpecialDialogueBit(HAVE_DOOR_TO_HEAVEN_WINE_DIALOGUE) && !checkHaveKeyItem(DOOR_TO_HEAVEN)) {
        acquireKeyItem(DOOR_TO_HEAVEN);
    }
    
    if (checkSpecialDialogueBit(FIXED_MUSIC_BOX_DIALOGUE)) {
        if (!checkHaveKeyItem(FIXED_MUSIC_BOX)) {
            removeKeyItem(BROKEN_MUSIC_BOX);
            acquireKeyItem(FIXED_MUSIC_BOX);
        }
    } else {
        removeKeyItem(FIXED_MUSIC_BOX);
    }
    
    if (!checkSpecialDialogueBit(HAVE_WEATHER_VANE_DIALOGUE)) {
        removeKeyItem(WEATHER_VANE);
    }
    
    if (!checkSpecialDialogueBit(HAVE_GOLD_PENDANT_DIALOGUE)) {
        removeKeyItem(GOLD_PENDANT);
    }
    
    if (checkSpecialDialogueBit(GAVE_PONTATA_ROOT_TO_POTION_SHOP_DEALER_DIALOGUE)) {
        setLifeEventBit(GAVE_PONTATA_ROOT_TO_POTION_SHOP_DEALER);
    }
    
    if (checkSpecialDialogueBit(GAVE_BLUE_ROCK_TO_SAIBARA_DIALOGUE)) {
        setLifeEventBit(GAVE_BLUE_ROCK_TO_SAIBARA);
    }

    if (checkSpecialDialogueBit(GAVE_RICK_RARE_METAL_DIALOGUE)) {
        setLifeEventBit(GIVE_RICK_RARE_METAL);
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/game", messageBoxCallback);

void messageBoxCallback(void) {
    
    // check if a message box has flag 4 set
    if (checkAnyMessageBoxTextFinished()) {

        setMainLoopCallbackFunctionIndex(MAIN_GAME);

        togglePauseEntities();
        resumeCutsceneExecutors();
        setEntityMapSpaceIndependent(ENTITY_PLAYER, TRUE);

    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/game", endCutsceneCallback);

// ends cutscene
void endCutsceneCallback(void) {

    // unused variable
    if (D_801FB686 == 0) {

        setMainLoopCallbackFunctionIndex(MAIN_GAME);
        togglePauseEntities();
        // cutscene executor flags
        resumeCutsceneExecutors();
        setEntityMapSpaceIndependent(ENTITY_PLAYER, TRUE);

    }

}

//INCLUDE_ASM("asm/nonmatchings/game/game", loadNamingScreenCallback);

void loadNamingScreenCallback(void) {
    
    u8* namePtr;

    openOverlayScreen_2();
    
    switch (gNamingScreenIndex) {

        case NAMING_SCREEN_TYPE_PLAYER:
            namePtr = gPlayer.name;
            break;
        case NAMING_SCREEN_TYPE_FARM:
            namePtr = gFarmName;
            break;
        case NAMING_SCREEN_TYPE_DOG:
            namePtr = dogInfo.name;
            break;
        case NAMING_SCREEN_TYPE_HORSE:
            namePtr = horseInfo.name;
            break;
        case NAMING_SCREEN_TYPE_BABY:
            namePtr = &gBabyName;
            break;
        case NAMING_SCREEN_TYPE_COW:
            namePtr = gFarmAnimals[gSelectedAnimalIndex].name;
            break;
        case NAMING_SCREEN_TYPE_SHEEP:
            namePtr = gFarmAnimals[gSelectedAnimalIndex].name;
            break;
        case NAMING_SCREEN_TYPE_CHICKEN:
            namePtr = gChickens[gSelectedAnimalIndex].name;
            break;
            
    }
    
    initializeNamingScreen(namePtr, gNamingScreenIndex);

}

//INCLUDE_ASM("asm/nonmatchings/game/game", mapLoadCallback);

void mapLoadCallback(void) {
    
    bool set;
    int checkOverflow;
    s16 maxHappiness;
    u8 happinessIncrease;
    
    clearDailyEventBit(SUSPEND_TIME_DURING_ANIMATION);
    clearDailyEventBit(BLOCK_BUTTON_USAGE);
    clearDailyEventBit(BLOCK_PAUSE_SCREEN);

    setPlayerAction(CONTROLLER_INPUT, ANIM_DEFAULT);

    loadLevel(1);
    
    set = FALSE;
    
    // chicken born
    if (gBaseMapIndex == COOP && checkLifeEventBit(CHICKEN_BORN)) {
        
        gNamingScreenIndex = NAMING_SCREEN_TYPE_CHICKEN;
        gSelectedAnimalIndex = bornChickenIndex;
        gChickens[bornChickenIndex].flags &= ~CHICKEN_NEWBORN;
        
        setMainLoopCallbackFunctionIndex(NAMING_SCREEN_LOAD);
        clearLifeEventBit(CHICKEN_BORN);
        setLifeEventBit(CHICK_HATCHED);
        
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
    
    // farm animal born
    if (gBaseMapIndex == BARN && checkLifeEventBit(ANIMAL_SOLD)) {
        
        // TODO: this might be an inline function

        gNamingScreenIndex = NAMING_SCREEN_TYPE_COW;
        gSelectedAnimalIndex = bornAnimalIndex;
        gFarmAnimals[bornAnimalIndex].flags &= ~FARM_ANIMAL_NEWBORN;
            
        setMainLoopCallbackFunctionIndex(NAMING_SCREEN_LOAD);
        clearLifeEventBit(ANIMAL_SOLD);
            
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
        setDailyEventBit(NIGHTTIME_AUDIO_STOPPED);
        setDailyEventBit(NIGHTTIME_AUDIO_STARTED);
    }
    
    if (!set) {
        setMainLoopCallbackFunctionIndex(SET_AUDIO_AND_LIGHTING);
    }

}

// FIXME: clean up/rename inlines

static inline void pickUpShopItem(u8 arg0, u8 arg1, u8 arg2, u8 arg3, u8 arg4) {

    switch (arg0) {                   
        case 0:                                 
            handlePickUpShopItem(arg1);
            setMainLoopCallbackFunctionIndex(MAIN_GAME);
            break;
        case 1:                                 
            handlePickUpShopItem(arg2);
            setMainLoopCallbackFunctionIndex(MAIN_GAME);
            break;
        case 2:                                 
            handlePickUpShopItem(arg3);
            setMainLoopCallbackFunctionIndex(MAIN_GAME);
            break;
        case 3:                                 
            handlePickUpShopItem(arg4);
            setMainLoopCallbackFunctionIndex(MAIN_GAME);
            break;
    }

}

static inline void inline2(u8 arg0, u8 arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5) {
    switch (arg0) {                    
        case 0:                                 
            handlePickUpShopItem(arg1);
            setMainLoopCallbackFunctionIndex(MAIN_GAME);
            break;
        case 1:                                 
            handlePickUpShopItem(arg2);
            setMainLoopCallbackFunctionIndex(MAIN_GAME);
            break;
        case 2:                                 
            handlePickUpShopItem(arg3);
            setMainLoopCallbackFunctionIndex(MAIN_GAME);
            break;
        case 3:                                 
            handlePickUpShopItem(arg4);
            setMainLoopCallbackFunctionIndex(MAIN_GAME);
            break;
        case 4:                                 
            handlePickUpShopItem(arg5);
            setMainLoopCallbackFunctionIndex(MAIN_GAME);
            break;
    }
}

static inline void inline3() {           

    dialogues[0].sessionManager.flags &= ~0x40;
    
    closeDialogueSession(0);

    setPlayerAction(CONTROLLER_INPUT, ANIM_DEFAULT);
    setMainLoopCallbackFunctionIndex(MAIN_GAME);

}

// from transition.c
static inline void launchIntroCutscene_2(u16 cutsceneIndex, u16 spawnPoint, u8 arg2) {
    
    deactivateSprites();
    
    deactivateGlobalSprites();
    
    initializeMessageBoxes();
    initializeCutsceneExecutors();
    initializeMainMessageBoxes();
    
    resetGlobalLighting();

    setEntitiesRGBA(0, 0, 0, 0);
    setMapControllerRGBA(MAIN_MAP_INDEX, 0, 0, 0, 0);

    gHour = 12;

    // set ptrs to rom addresses for sprites
    initializeEntityInstances(arg2);

    setSpawnPoint(spawnPoint);

    gCutsceneIndex = cutsceneIndex;

    loadCutscene(FALSE);
    
    loadMapAtSpawnPoint(gSpawnPointIndex);
    setupPlayerEntity(gSpawnPointIndex, 0);

    handlePlayerAnimation();

    setMainLoopCallbackFunctionIndex(SET_AUDIO_AND_LIGHTING);

}

// possible split

//INCLUDE_ASM("asm/nonmatchings/game/game", dialogueMenuCallback);

void dialogueMenuCallback() {

    u8 selectedMenuRow;

    if (dialogues[0].sessionManager.flags & 4) {
        
        togglePauseEntities();
        resumeCutsceneExecutors();
        setEntityMapSpaceIndependent(ENTITY_PLAYER, TRUE);

        selectedMenuRow = getSelectedMenuRow(0);

        switch (gameLoopContext.dialogueMenuIndex) {

            case DIALOGUE_MENU_DIARY_ROOT:          

                switch (selectedMenuRow) {                      
                    case 0:
                        showDialogueTextBox(DIALOGUE_MENU_DIARY_SAVE_PROMPT);
                        break;
                    case 1:                                 
                        showMessageBox(1, DIALOGUE_DIARY, 2, 0x80, 2);
                        break;
                    case 2:                                 
                        setMainLoopCallbackFunctionIndex(ESTIMATE_LOAD);
                        break;
                    case 3:                                 
                        setMainLoopCallbackFunctionIndex(ALBUM_LOAD);
                        break;
                    case 4:                                 
                        setMainLoopCallbackFunctionIndex(MAIN_GAME);
                        break;
                    }
                
                break;
            
            case DIALOGUE_MENU_DIARY_SAVE_PROMPT:

                switch (selectedMenuRow) {
                     
                    // write in diary/save game
                    case 0:                                 
                        setMainLoopCallbackFunctionIndex(MAIN_GAME);
                        setPlayerAction(GETTING_INTO_BED, ANIM_GET_INTO_BED);
                        setDailyEventBit(WROTE_IN_DIARY);
                        break;
                        
                    // don't write
                    case 1:                                 
                        setMainLoopCallbackFunctionIndex(MAIN_GAME);
                        setPlayerAction(GETTING_INTO_BED, ANIM_GET_INTO_BED);
                        break;

                    case 2:
                        setMainLoopCallbackFunctionIndex(MAIN_GAME);
                        break;

                }
                
                break;

            case DIALOGUE_MENU_BAKERY_FOOD:                       
                pickUpShopItem(selectedMenuRow, 11, 12, 13, 18);
                break;

            case DIALOGUE_MENU_BAKERY_DRINK_MENU:
                pickUpShopItem(selectedMenuRow, 14, 15, 16, 17);
                break;

            case DIALOGUE_MENU_TAVERN_MENU:       
                inline2(selectedMenuRow, 40, 41, 42, 43, 44);
                break;

            case DIALOGUE_MENU_RANCH_STORE_ANIMALS_ROOT:                                     
                switch (selectedMenuRow) {                    
                    case 0:
                        showDialogueTextBox(DIALOGUE_MENU_RANCH_STORE_ANIMAL_BUY);
                        break;
                    // sell animal
                    case 1:                                
                        showTextBox(0, LEVEL_INTERACTIONS_TEXT_INDEX, 85, 0, 0);
                        setDailyEventBit(ANIMAL_SALE_IN_PROGRESS);
                        setDailyEventBit(DAILY_ANIMAL_TRANSACTION);
                        break;
                    // "Not now"
                    case 2:                                
                        handlePickUpShopItem(RANCH_STORE_SHOP_CANCEL);
                        setMainLoopCallbackFunctionIndex(MAIN_GAME);
                        break;
                    }
                
                break;

            case DIALOGUE_MENU_RANCH_STORE_ANIMAL_BUY:                                
                pickUpShopItem(selectedMenuRow, 30, 31, 32, 33);
                break;

            case DIALOGUE_MENU_RANCH_STORE_ANIMAL_SELL_CONFIRM: 

                switch (selectedMenuRow) {                   

                    case 0:                         

                        gGold += adjustValue(gGold, gAnimalSalePrice, MAX_GOLD);
                        
                        gAnimalSalePrice = 0;                        
                        dialogues[0].sessionManager.flags &= ~0x40;
                        
                        closeDialogueSession(0);
                        setSpawnPoint(FARM_SPAWN_POINT_10);

                        handleExitLevel(0, MAP_LOAD);

                        clearDailyEventBit(ANIMAL_SALE_IN_PROGRESS);
                        setDailyEventBit(ANIMAL_SOLD);

                        switch (selectedAnimalType) {
                            case CHICKEN_TYPE:
                                initializeChicken(gSelectedAnimalIndex);
                                break;
                            case COW_TYPE ... SHEEP_TYPE:
                                initializeFarmAnimal(gSelectedAnimalIndex);
                                break;
                        }
                        
                        break;
                    
                    case 1:                                 
                        dialogues[0].sessionManager.flags &= ~0x40;
                        closeDialogueSession(0);
                        setMainLoopCallbackFunctionIndex(MAIN_GAME);
                        
                        break;

                    }
                
                break;

            case DIALOGUE_MENU_COW_FESTIVAL_ENTRY_CONFIRM:                

                switch (selectedMenuRow) {         

                    case 0:                                 
                        
                        dialogues[0].sessionManager.flags &= ~0x40;
                        closeDialogueSession(0);
                        setSpawnPoint(FARM_SPAWN_POINT_10);

                        handleExitLevel(0, MAP_LOAD);

                        clearDailyEventBit(COW_FESTIVAL_ELIGIBLE);
                        setDailyEventBit(COW_FESTIVAL_ENTRY_RESOLVED);
                        setDailyEventBit(COW_FESTIVAL_COW_TAKEN);
                        gCowFestivalEnteredCowIndex = gSelectedAnimalIndex;
                        break;
                        
                    case 1:                                 
                        dialogues[0].sessionManager.flags &= ~0x40;
                        closeDialogueSession(0);
                        setMainLoopCallbackFunctionIndex(MAIN_GAME);
                        break;
                    }
                
                break;

            case DIALOGUE_MENU_FLOWER_FESTIVAL_VOTE:                

                if (selectedMenuRow >= 5) {
                    gVoteForFlowerFestivalGoddess = 0xFF;
                } else {
                    gVoteForFlowerFestivalGoddess = selectedMenuRow;
                }
                
                dialogues[0].sessionManager.flags &= ~0x40;
                closeDialogueSession(0);
                setMainLoopCallbackFunctionIndex(MAIN_GAME);
                break;
            
            case DIALOGUE_MENU_SQUARE_STALL:                

                switch (selectedMenuRow) {                   
                    case 0:                                 
                        handlePickUpShopItem(50);
                        setMainLoopCallbackFunctionIndex(MAIN_GAME);
                        break;
                    case 1:                                 
                        handlePickUpShopItem(51);
                        setMainLoopCallbackFunctionIndex(MAIN_GAME);
                        break;    
                    case 2:                                 
                        handlePickUpShopItem(52);
                        setMainLoopCallbackFunctionIndex(MAIN_GAME);
                        break;    
                    case 3:                                 
                        showTextBox(0, FESTIVALS_TEXT_INDEX, 31, 0, 0);
                        break; 
                    }
                
                    break;

            case DIALOGUE_MENU_HORSE_RACE_STALL:                 

                switch (selectedMenuRow) {                     
                    case 0:                                 
                        handlePickUpShopItem(45);
                        setMainLoopCallbackFunctionIndex(MAIN_GAME);
                        break;    
                    case 1:                                 
                        handlePickUpShopItem(46);
                        setMainLoopCallbackFunctionIndex(MAIN_GAME);
                        break;    
                    case 2:                                 
                        handlePickUpShopItem(49);
                        setMainLoopCallbackFunctionIndex(MAIN_GAME);
                        break;    
                    case 3:
                        showTextBox(0, FESTIVALS_TEXT_INDEX, 20, 0, 0);
                        break;
                    }
                
                break;

            case DIALOGUE_MENU_DOG_RACE_STALL:                   

                switch (selectedMenuRow) {                     
                    case 0:                                 
                        handlePickUpShopItem(47);
                        setMainLoopCallbackFunctionIndex(MAIN_GAME);
                        break;    
                    case 1:                                 
                        handlePickUpShopItem(48);
                        setMainLoopCallbackFunctionIndex(MAIN_GAME);
                        break;    
                    case 2:                                 
                        handlePickUpShopItem(49);
                        setMainLoopCallbackFunctionIndex(MAIN_GAME);
                        break;    
                    case 3:
                        showTextBox(0, FESTIVALS_TEXT_INDEX, 20, 0, 0);
                        break;
                    }
                
                break;

            case DIALOGUE_MENU_LIBRARY_BOOKSHELF_1:                                    

                if (gSeason == SPRING || gSeason == WINTER) {

                    if (gPlayer.actionHandler == DIALOGUE_SELECTING || selectedMenuRow == 3) {

                        switch (selectedMenuRow) {             
                            case 0:                         
                                showTextBox(1, LIBRARY_TEXT_INDEX, 0, 0, 0);
                                break;
                            case 1:                         
                                showTextBox(1, LIBRARY_TEXT_INDEX, 1, 0, 0);
                                break;
                            case 2:                         
                                showTextBox(1, LIBRARY_TEXT_INDEX, 2, 0, 0);
                                break;
                            case 3:
                                dialogues[0].sessionManager.flags &= ~0x40;
                                closeDialogueSession(0);
                                setPlayerAction(CONTROLLER_INPUT, ANIM_DEFAULT);
                                setMainLoopCallbackFunctionIndex(MAIN_GAME);
                                break;
                            }
                        
                        setItemState(gPlayer.itemInfoIndex, 1);
                        gPlayer.heldItem = 0;

                    } else {
                        setPlayerAction(DIALOGUE_SELECTING, ANIM_DIALOGUE_SELECTING);
                        setMainLoopCallbackFunctionIndex(MAIN_GAME);
                    }

                } else if (gPlayer.actionHandler == DIALOGUE_SELECTING || selectedMenuRow == 3) {
                    
                    switch (selectedMenuRow) {                 
                        case 0:                             
                            showTextBox(1, LIBRARY_TEXT_INDEX, 3, 0, 0);
                            break;
                        case 1:                             
                            showTextBox(1, LIBRARY_TEXT_INDEX, 4, 0, 0);
                            break;     
                        case 2:                             
                            showTextBox(1, LIBRARY_TEXT_INDEX, 5, 0, 0);
                            break;
                        case 3:
                            dialogues[0].sessionManager.flags &= ~0x40;
                            closeDialogueSession(0);
                            setPlayerAction(CONTROLLER_INPUT, ANIM_DEFAULT);
                            setMainLoopCallbackFunctionIndex(MAIN_GAME);
                            break;
                        }
                    
                    setItemState(gPlayer.itemInfoIndex, 1);
                    gPlayer.heldItem = 0;

                } else {
                    setPlayerAction(DIALOGUE_SELECTING, ANIM_DIALOGUE_SELECTING);
                    setMainLoopCallbackFunctionIndex(MAIN_GAME);
                }

                break;

            case DIALOGUE_MENU_LIBRARY_BOOKSHELF_2:                                   

                if (gSeason == SPRING || gSeason == WINTER) {

                    if (gPlayer.actionHandler == DIALOGUE_SELECTING || selectedMenuRow == 3) {

                        switch (selectedMenuRow) {            
                            case 0:                        
                                showTextBox(1, LIBRARY_TEXT_INDEX, 9, 0, 0);
                                break;
                            case 1:                        
                                showTextBox(1, LIBRARY_TEXT_INDEX, 10, 0, 0);
                                break;
                            case 2:                        
                                showTextBox(1, LIBRARY_TEXT_INDEX, 11, 0, 0);
                                break;
                            case 3:
                                dialogues[0].sessionManager.flags &= ~0x40;
                                closeDialogueSession(0);
                                setPlayerAction(CONTROLLER_INPUT, ANIM_DEFAULT);
                                setMainLoopCallbackFunctionIndex(MAIN_GAME);
                                break;
                            }
                        
                        setItemState(gPlayer.itemInfoIndex, 1);
                        gPlayer.heldItem = 0;
                        
                    } else {
                        setPlayerAction(DIALOGUE_SELECTING, ANIM_DIALOGUE_SELECTING);
                        setMainLoopCallbackFunctionIndex(MAIN_GAME);
                    }
                    
                } else if (gPlayer.actionHandler == DIALOGUE_SELECTING || selectedMenuRow == 3) {

                    switch (selectedMenuRow) {                
                        case 0:                             
                            showTextBox(1, LIBRARY_TEXT_INDEX, 6, 0, 0);
                            break;
                        case 1:                             
                            showTextBox(1, LIBRARY_TEXT_INDEX, 7, 0, 0);
                            break;
                        case 2:                             
                            showTextBox(1, LIBRARY_TEXT_INDEX, 8, 0, 0);
                            break;
                        case 3:
                            dialogues[0].sessionManager.flags &= ~0x40;
                            closeDialogueSession(0);
                            setPlayerAction(CONTROLLER_INPUT, ANIM_DEFAULT);
                            setMainLoopCallbackFunctionIndex(MAIN_GAME);
                            break;
                    }
                
                    setItemState(gPlayer.itemInfoIndex, 1);
                    gPlayer.heldItem = 0;
                    
                } else {
                    setPlayerAction(DIALOGUE_SELECTING, ANIM_DIALOGUE_SELECTING);
                    setMainLoopCallbackFunctionIndex(MAIN_GAME);
                }

                break;


            case DIALOGUE_MENU_LIBRARY_BOOKSHELF_3:                 

                if (gSeason == SPRING || gSeason == WINTER) {

                    if (gPlayer.actionHandler == DIALOGUE_SELECTING || selectedMenuRow == 3) {

                        switch (selectedMenuRow) {             
                            case 0:                         
                                showTextBox(1, LIBRARY_TEXT_INDEX, 12, 0, 0);
                                break;
                            case 1:                         
                                showTextBox(1, LIBRARY_TEXT_INDEX, 13, 0, 0);
                                break;
                            case 2:                         
                                showTextBox(1, LIBRARY_TEXT_INDEX, 14, 0, 0);
                                break;
                            case 3:
                                dialogues[0].sessionManager.flags &= ~0x40;
                                closeDialogueSession(0);
                                setPlayerAction(CONTROLLER_INPUT, ANIM_DEFAULT);
                                setMainLoopCallbackFunctionIndex(MAIN_GAME);
                                break;
                            }
                        
                        setItemState(gPlayer.itemInfoIndex, 1);
                        gPlayer.heldItem = 0;

                    } else {
                        setPlayerAction(DIALOGUE_SELECTING, ANIM_DIALOGUE_SELECTING);
                        setMainLoopCallbackFunctionIndex(MAIN_GAME);
                    }
                    
                } else if (gPlayer.actionHandler == DIALOGUE_SELECTING || selectedMenuRow == 3) {

                    switch (selectedMenuRow) {                         
                        case 0:                             
                            showTextBox(1, LIBRARY_TEXT_INDEX, 15, 0, 0);
                            break;
                        case 1:                             
                            showTextBox(1, LIBRARY_TEXT_INDEX, 16, 0, 0);
                            break;
                        case 2:                             
                            showTextBox(1, LIBRARY_TEXT_INDEX, 17, 0, 0);
                            break;
                        case 3:
                            dialogues[0].sessionManager.flags &= ~0x40;
                            closeDialogueSession(0);
                            setPlayerAction(CONTROLLER_INPUT, ANIM_DEFAULT);
                            setMainLoopCallbackFunctionIndex(MAIN_GAME);
                            break;
                        }
                    
                    setItemState(gPlayer.itemInfoIndex, 1);
                    gPlayer.heldItem = 0;

                } else {
                    setPlayerAction(DIALOGUE_SELECTING, ANIM_DIALOGUE_SELECTING);
                    setMainLoopCallbackFunctionIndex(MAIN_GAME);
                }

                break;

            case DIALOGUE_MENU_LIBRARY_BOOKSHELF_4:   

                if (gSeason == SPRING || gSeason == WINTER) {

                    if (gPlayer.actionHandler == DIALOGUE_SELECTING || selectedMenuRow == 3) {

                        switch (selectedMenuRow) {             
                            case 0:                         
                                showTextBox(1, LIBRARY_TEXT_INDEX, 18, 0, 0);
                                break;
                            case 1:                         
                                showTextBox(1, LIBRARY_TEXT_INDEX, 19, 0, 0);
                                break;
                            case 2:                         
                                showTextBox(1, LIBRARY_TEXT_INDEX, 20, 0, 0);
                                break;                        
                            case 3:
                                dialogues[0].sessionManager.flags &= ~0x40;
                                closeDialogueSession(0);
                                setPlayerAction(CONTROLLER_INPUT, ANIM_DEFAULT);
                                setMainLoopCallbackFunctionIndex(MAIN_GAME);
                                break;
                            }
                        
                            setItemState(gPlayer.itemInfoIndex, 1);
                            gPlayer.heldItem = 0;

                    } else {
                        setPlayerAction(DIALOGUE_SELECTING, ANIM_DIALOGUE_SELECTING);
                        setMainLoopCallbackFunctionIndex(MAIN_GAME);
                    }

                } else if (gPlayer.actionHandler == DIALOGUE_SELECTING || selectedMenuRow == 3) {

                    switch (selectedMenuRow) {                 
                        case 0:                             
                            showTextBox(1, LIBRARY_TEXT_INDEX, 21, 0, 0);
                            break;
                        case 1:                             
                            showTextBox(1, LIBRARY_TEXT_INDEX, 22, 0, 0);
                            break;
                        case 2:                             
                            showTextBox(1, LIBRARY_TEXT_INDEX, 23, 0, 0);
                            break;
                        case 3:
                            dialogues[0].sessionManager.flags &= ~0x40;
                            closeDialogueSession(0);
                            setPlayerAction(CONTROLLER_INPUT, ANIM_DEFAULT);
                            setMainLoopCallbackFunctionIndex(MAIN_GAME);
                            break;
                        }
                    
                        setItemState(gPlayer.itemInfoIndex, 1);
                        gPlayer.heldItem = 0;

                    } else {
                        setPlayerAction(DIALOGUE_SELECTING, ANIM_DIALOGUE_SELECTING);
                        setMainLoopCallbackFunctionIndex(MAIN_GAME);
                    }
                
                break;

            case DIALOGUE_MENU_LIBRARY_BOOKSHELF_5:   

                if (gSeason == SPRING || gSeason == WINTER) {

                    if (gPlayer.actionHandler == DIALOGUE_SELECTING || selectedMenuRow == 4) {

                        switch (selectedMenuRow) {            
                            case 0:                         
                                showTextBox(1, LIBRARY_TEXT_INDEX, 25, 0, 0);
                                break;
                            case 1:                         
                                showTextBox(1, LIBRARY_TEXT_INDEX, 26, 0, 0);
                                break;
                            case 2:                         
                                showTextBox(1, LIBRARY_TEXT_INDEX, 27, 0, 0);
                                break;
                            case 3:                         
                                showTextBox(1, LIBRARY_TEXT_INDEX, 24, 0, 0);
                                break;
                            case 4:
                                dialogues[0].sessionManager.flags &= ~0x40;
                                closeDialogueSession(0);
                                setPlayerAction(CONTROLLER_INPUT, ANIM_DEFAULT);
                                setMainLoopCallbackFunctionIndex(MAIN_GAME);
                                break;
                            }
                        
                        setItemState(gPlayer.itemInfoIndex, 1);
                        gPlayer.heldItem = 0;

                    } else {
                        setPlayerAction(DIALOGUE_SELECTING, ANIM_DIALOGUE_SELECTING);
                        setMainLoopCallbackFunctionIndex(MAIN_GAME);
                    }
                
                } else if (gPlayer.actionHandler == DIALOGUE_SELECTING || selectedMenuRow == 4) {

                    switch (selectedMenuRow) {                
                        case 0:                             
                            showTextBox(1, LIBRARY_TEXT_INDEX, 28, 0, 0);
                            break;
                        case 1:                             
                            showTextBox(1, LIBRARY_TEXT_INDEX, 29, 0, 0);
                            break;
                        case 2:                             
                            showTextBox(1, LIBRARY_TEXT_INDEX, 30, 0, 0);
                            break;
                        case 3:                             
                            showTextBox(1, LIBRARY_TEXT_INDEX, 31, 0, 0);
                            break;                       
                        case 4:        
                            dialogues[0].sessionManager.flags &= ~0x40;
                            closeDialogueSession(0);
                            setPlayerAction(CONTROLLER_INPUT, ANIM_DEFAULT);
                            setMainLoopCallbackFunctionIndex(MAIN_GAME);
                            break;
                        }
                
                        setItemState(gPlayer.itemInfoIndex, 1);
                        gPlayer.heldItem = 0;

                    } else {
                        setPlayerAction(DIALOGUE_SELECTING, ANIM_DIALOGUE_SELECTING);
                        setMainLoopCallbackFunctionIndex(MAIN_GAME);
                    }
        
                break;

            case DIALOGUE_MENU_LIBRARY_BOOKSHELF_6:                             

                if (gSeason == SPRING || gSeason == WINTER) {

                    if (gPlayer.actionHandler == DIALOGUE_SELECTING || selectedMenuRow == 4) {

                        switch (selectedMenuRow) {             
                            case 0:                         
                                showTextBox(1, LIBRARY_TEXT_INDEX, 32, 0, 0);
                                break;
                            case 1:                         
                                showTextBox(1, LIBRARY_TEXT_INDEX, 33, 0, 0);
                                break;
                            case 2:                         
                                showTextBox(1, LIBRARY_TEXT_INDEX, 34, 0, 0);
                                break;
                            case 3:
                                showTextBox(1, LIBRARY_TEXT_INDEX, 35, 0, 0);
                                break;
                            case 4:
                                dialogues[0].sessionManager.flags &= ~0x40;
                                closeDialogueSession(0);
                                setPlayerAction(CONTROLLER_INPUT, ANIM_DEFAULT);
                                setMainLoopCallbackFunctionIndex(MAIN_GAME);
                                break;
                            }
                        
                        setItemState(gPlayer.itemInfoIndex, 1);
                        gPlayer.heldItem = 0;

                    } else {
                        setPlayerAction(DIALOGUE_SELECTING, ANIM_DIALOGUE_SELECTING);
                        setMainLoopCallbackFunctionIndex(MAIN_GAME);
                    }
                    
                } else if (gPlayer.actionHandler == DIALOGUE_SELECTING || selectedMenuRow == 4) {

                    switch (selectedMenuRow) {                 
                        case 0:                             
                            showTextBox(1, LIBRARY_TEXT_INDEX, 36, 0, 0);
                            break;
                        case 1:                             
                            showTextBox(1, LIBRARY_TEXT_INDEX, 37, 0, 0);
                            break;
                        case 2:                             
                            showTextBox(1, LIBRARY_TEXT_INDEX, 38, 0, 0);
                            break;
                        case 3:
                            showTextBox(1, LIBRARY_TEXT_INDEX, 39, 0, 0);
                            break;
                        case 4:
                            dialogues[0].sessionManager.flags &= ~0x40;
                            closeDialogueSession(0);
                            setPlayerAction(CONTROLLER_INPUT, ANIM_DEFAULT);
                            setMainLoopCallbackFunctionIndex(MAIN_GAME);
                            break;
                        }
                    
                    setItemState(gPlayer.itemInfoIndex, 1);
                    gPlayer.heldItem = 0;
                
                } else {
                    setPlayerAction(DIALOGUE_SELECTING, ANIM_DIALOGUE_SELECTING);
                    setMainLoopCallbackFunctionIndex(MAIN_GAME);
                }

                break;
            
            case DIALOGUE_MENU_LIBRARY_BOOKSHELF_7:            

                if (gSeason == SPRING || gSeason == WINTER) {

                    if (gPlayer.actionHandler == DIALOGUE_SELECTING || selectedMenuRow == 4) {

                        switch (selectedMenuRow) {             
                            case 0:                         
                                showTextBox(1, LIBRARY_TEXT_INDEX, 40, 0, 0);
                                break;
                            case 1:                         
                                showTextBox(1, LIBRARY_TEXT_INDEX, 41, 0, 0);
                                break;
                            case 2:                         
                                showTextBox(1, LIBRARY_TEXT_INDEX, 42, 0, 0);
                                break;
                            case 3:
                                showTextBox(1, LIBRARY_TEXT_INDEX, 43, 0, 0);
                                break;
                            case 4:
                                dialogues[0].sessionManager.flags &= ~0x40;
                                closeDialogueSession(0);
                                setPlayerAction(CONTROLLER_INPUT, ANIM_DEFAULT);
                                setMainLoopCallbackFunctionIndex(MAIN_GAME);
                                break;
                            }
                        
                        setItemState(gPlayer.itemInfoIndex, 1);
                        gPlayer.heldItem = 0;

                    } else {
                        setPlayerAction(DIALOGUE_SELECTING, ANIM_DIALOGUE_SELECTING);
                        setMainLoopCallbackFunctionIndex(MAIN_GAME);
                    }

                } else if (gPlayer.actionHandler == DIALOGUE_SELECTING || selectedMenuRow == 4) {

                    switch (selectedMenuRow) {                 
                        case 0:                             
                            showTextBox(1, LIBRARY_TEXT_INDEX, 44, 0, 0);
                            break;
                        case 1:                             
                            showTextBox(1, LIBRARY_TEXT_INDEX, 45, 0, 0);
                            break;
                        case 2:                             
                            showTextBox(1, LIBRARY_TEXT_INDEX, 46, 0, 0);
                            break;
                        case 3:
                            showTextBox(1, LIBRARY_TEXT_INDEX, 47, 0, 0);
                            break;
                        case 4:
                            dialogues[0].sessionManager.flags &= ~0x40;
                            closeDialogueSession(0);
                            setPlayerAction(CONTROLLER_INPUT, ANIM_DEFAULT);
                            setMainLoopCallbackFunctionIndex(MAIN_GAME);
                            break;
                        }
                
                    setItemState(gPlayer.itemInfoIndex, 1);
                    gPlayer.heldItem = 0;

                } else {
                    setPlayerAction(DIALOGUE_SELECTING, ANIM_DIALOGUE_SELECTING);
                    setMainLoopCallbackFunctionIndex(MAIN_GAME);
                }

                break;
            
            case DIALOGUE_MENU_LIBRARY_BOOKSHELF_8:                                    

                if (gSeason == SPRING || gSeason == WINTER) {

                    if (gPlayer.actionHandler == DIALOGUE_SELECTING || selectedMenuRow == 4) {

                        switch (selectedMenuRow) {             
                            case 0:                         
                                showTextBox(1, LIBRARY_TEXT_INDEX, 48, 0, 0);
                                break;
                            case 1:                         
                                showTextBox(1, LIBRARY_TEXT_INDEX, 49, 0, 0);
                                break;
                            case 2:                         
                                showTextBox(1, LIBRARY_TEXT_INDEX, 50, 0, 0);
                                break;
                            case 3:
                                showTextBox(1, LIBRARY_TEXT_INDEX, 51, 0, 0);
                                break;
                            case 4:
                                dialogues[0].sessionManager.flags &= ~0x40;
                                closeDialogueSession(0);
                                setPlayerAction(CONTROLLER_INPUT, ANIM_DEFAULT);
                                setMainLoopCallbackFunctionIndex(MAIN_GAME);
                                break;
                            }
                        
                        setItemState(gPlayer.itemInfoIndex, 1);
                        gPlayer.heldItem = 0;

                    } else {
                        setPlayerAction(DIALOGUE_SELECTING, ANIM_DIALOGUE_SELECTING);
                        setMainLoopCallbackFunctionIndex(MAIN_GAME);
                    }

                } else if (gPlayer.actionHandler == DIALOGUE_SELECTING || selectedMenuRow == 4) {
                    
                    switch (selectedMenuRow) {                 
                        case 0:                             
                            showTextBox(1, LIBRARY_TEXT_INDEX, 52, 0, 0);
                            break;
                        case 1:                             
                            showTextBox(1, LIBRARY_TEXT_INDEX, 53, 0, 0);
                            break;
                        case 2:                             
                            showTextBox(1, LIBRARY_TEXT_INDEX, 54, 0, 0);
                            break;
                        case 3:   
                            showTextBox(1, LIBRARY_TEXT_INDEX, 55, 0, 0);
                            break;
                        case 4:
                            dialogues[0].sessionManager.flags &= ~0x40;
                            closeDialogueSession(0);
                            setPlayerAction(CONTROLLER_INPUT, ANIM_DEFAULT);
                            setMainLoopCallbackFunctionIndex(MAIN_GAME);
                            break;
                        }
                    
                    setItemState(gPlayer.itemInfoIndex, 1);
                    gPlayer.heldItem = 0;

                } else {
                    setPlayerAction(DIALOGUE_SELECTING, ANIM_DIALOGUE_SELECTING);
                    setMainLoopCallbackFunctionIndex(MAIN_GAME);
                }

                break;

            case DIALOGUE_MENU_LIBRARY_BOOKSHELF_9:                                   

                if (gSeason == SPRING || gSeason == WINTER) {

                    if (gPlayer.actionHandler == DIALOGUE_SELECTING || selectedMenuRow == 4) {

                        switch (selectedMenuRow) {             
                            case 0:                         
                                showTextBox(1, LIBRARY_TEXT_INDEX, 56, 0, 0);
                                break;
                            case 1:                         
                                showTextBox(1, LIBRARY_TEXT_INDEX, 57, 0, 0);
                                break;
                            case 2:                         
                                showTextBox(1, LIBRARY_TEXT_INDEX, 58, 0, 0);
                                break;
                            case 3:
                                showTextBox(1, LIBRARY_TEXT_INDEX, 59, 0, 0);
                                break;
                            case 4:
                                dialogues[0].sessionManager.flags &= ~0x40;
                                closeDialogueSession(0);
                                setPlayerAction(CONTROLLER_INPUT, ANIM_DEFAULT);
                                setMainLoopCallbackFunctionIndex(MAIN_GAME);
                                break;
                            }
                    
                        setItemState(gPlayer.itemInfoIndex, 1);
                        gPlayer.heldItem = 0;
                    
                    } else {
                        setPlayerAction(DIALOGUE_SELECTING, ANIM_DIALOGUE_SELECTING);
                        setMainLoopCallbackFunctionIndex(MAIN_GAME);
                    }

                } else if (gPlayer.actionHandler == DIALOGUE_SELECTING || selectedMenuRow == 4) {

                    switch (selectedMenuRow) {       

                        case 0:                             
                            showTextBox(1, LIBRARY_TEXT_INDEX, 60, 0, 0);
                            break;
                        case 1:                             
                            showTextBox(1, LIBRARY_TEXT_INDEX, 61, 0, 0);
                            break;
                        case 2:                             
                            showTextBox(1, LIBRARY_TEXT_INDEX, 62, 0, 0);
                            break;
                        case 3:
                            showTextBox(1, LIBRARY_TEXT_INDEX, 63, 0, 0);
                            break;
                        case 4:
                            dialogues[0].sessionManager.flags &= ~0x40;
                            closeDialogueSession(0);
                            setPlayerAction(CONTROLLER_INPUT, ANIM_DEFAULT);
                            setMainLoopCallbackFunctionIndex(MAIN_GAME);
                            break;

                        }
                    
                        setItemState(gPlayer.itemInfoIndex, 1);
                        gPlayer.heldItem = 0;

                } else {
                    setPlayerAction(DIALOGUE_SELECTING, ANIM_DIALOGUE_SELECTING);
                    setMainLoopCallbackFunctionIndex(MAIN_GAME);
                }

                break;

            case DIALOGUE_MENU_LIBRARY_BOOKSHELF_10:             

                if (gSeason == SPRING || gSeason == WINTER) {

                    if (gPlayer.actionHandler == DIALOGUE_SELECTING || selectedMenuRow == 4) {

                        switch (selectedMenuRow) {             
                            case 0:                         
                                showTextBox(1, LIBRARY_TEXT_INDEX, 64, 0, 0);
                                break;
                            case 1:                         
                                showTextBox(1, LIBRARY_TEXT_INDEX, 65, 0, 0);
                                break;
                            case 2:                         
                                showTextBox(1, LIBRARY_TEXT_INDEX, 66, 0, 0);
                                break;
                            case 3:
                                showTextBox(1, LIBRARY_TEXT_INDEX, 67, 0, 0);
                                break;
                            case 4:
                                dialogues[0].sessionManager.flags &= ~0x40;
                                closeDialogueSession(0);
                                setPlayerAction(CONTROLLER_INPUT, ANIM_DEFAULT);
                                setMainLoopCallbackFunctionIndex(MAIN_GAME);
                                break;
                            }
                        
                            setItemState(gPlayer.itemInfoIndex, 1);
                            gPlayer.heldItem = 0;

                    } else {
                        setPlayerAction(DIALOGUE_SELECTING, ANIM_DIALOGUE_SELECTING);
                        setMainLoopCallbackFunctionIndex(MAIN_GAME);
                    }

                } else if (gPlayer.actionHandler == DIALOGUE_SELECTING || selectedMenuRow == 4) {

                    switch (selectedMenuRow) {                 
                        case 0:                             
                            showTextBox(1, LIBRARY_TEXT_INDEX, 68, 0, 0);
                            break;
                        case 1:                             
                            showTextBox(1, LIBRARY_TEXT_INDEX, 69, 0, 0);
                            break;
                        case 2:                             
                            showTextBox(1, LIBRARY_TEXT_INDEX, 70, 0, 0);
                            break;
                        case 3:
                            showTextBox(1, LIBRARY_TEXT_INDEX, 71, 0, 0);
                            break;
                        case 4:
                            dialogues[0].sessionManager.flags &= ~0x40;
                            closeDialogueSession(0);
                            setPlayerAction(CONTROLLER_INPUT, ANIM_DEFAULT);
                            setMainLoopCallbackFunctionIndex(MAIN_GAME);
                            break;
                        }
                    
                    setItemState(gPlayer.itemInfoIndex, 1);
                    gPlayer.heldItem = 0;
                    
                } else {
                    setPlayerAction(DIALOGUE_SELECTING, ANIM_DIALOGUE_SELECTING);
                    setMainLoopCallbackFunctionIndex(MAIN_GAME);
                }
                
                break;

            case DIALOGUE_MENU_EVALUATION_CONTINUE_PROMPT:           

                switch (selectedMenuRow) {                

                    case 0:                 
                    
                        // TODO: could move this condition to a macro
                        if (calculateHouseExtensionScore() == 6 && checkLifeEventBit(MARRIED) && npcAffection[gWife] >= 250 && checkLifeEventBit(HAVE_BABY) && dogInfo.affection >= 200 && getSumNpcAffection() >= 2500 && getFarmGrassTilesSum() >= 384 && gMaximumStamina >= 190 && gHappiness >= 250 && getHealthyChickenCount() && D_801886D2 >= 10) {          
                            albumBits |= PHOTO_PARTY;  
                        }
                        
                        setMainLoopCallbackFunctionIndex(END_OF_DAY_1);

                        break;
                    
                    case 1:                      
                        launchIntroCutscene_2(FUNERAL, SQUARE_SPAWN_POINT_1, 1);
                        break;

                }
                
                break;

        }

    }

}

//INCLUDE_ASM("asm/nonmatchings/game/game", waitForAudioFinishCallback);

void waitForAudioFinishCallback(void) {
    if (checkAllSfxInactive()) {
        setMainLoopCallbackFunctionIndex(gameLoopContext.callbackIndex);
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/game", dreamCutscenesCallback);

void dreamCutscenesCallback(void) {
    handleCutsceneCompletion();
} 

//INCLUDE_ASM("asm/nonmatchings/game/game", endOfFestivalDayCallback1);

// show end of festival day dialogue box 
void endOfFestivalDayCallback1(void) {

    if (checkMapRGBADone(MAIN_MAP_INDEX) || !(mainMap[MAIN_MAP_INDEX].mapState.flags & MAP_ACTIVE)) {
        
        setMessageBoxViewSpacePosition(MAIN_MESSAGE_BOX_INDEX, 0, -64.0f, 352.0f);
        setMessageBoxSpriteIndices(MAIN_MESSAGE_BOX_INDEX, 1, 0, 0);
        setMessageBoxInterpolationWithFlags(MAIN_MESSAGE_BOX_INDEX, -4, 0);
        
        switch (gCutsceneIndex) {
            case 129 ... 130:
            case 139:              
            case 405 ... 409:
                initializeMessageBox(MAIN_MESSAGE_BOX_INDEX, FESTIVALS_TEXT_INDEX, 77, 0);
                break;
            case SEA_FESTIVAL:
            case EGG_FESTIVAL:
            case NEW_YEAR_FESTIVAL:
                initializeMessageBox(MAIN_MESSAGE_BOX_INDEX, FESTIVALS_TEXT_INDEX, 74, 0);
                break;
            case 416:
                initializeMessageBox(MAIN_MESSAGE_BOX_INDEX, FESTIVALS_TEXT_INDEX, 76, 0);
                break;
            default:
                initializeMessageBox(MAIN_MESSAGE_BOX_INDEX, FESTIVALS_TEXT_INDEX, 75, 0);
                break;
        }
        
        setMainLoopCallbackFunctionIndex(END_OF_FESTIVAL_DAY_2);

    }
}

//INCLUDE_ASM("asm/nonmatchings/game/game", endOfFestivalDayCallback2);

// second end of festival day callback
void endOfFestivalDayCallback2(void) {
    // check if a message box has flag 4 set
    if (checkAnyMessageBoxTextFinished()) {
        setMainLoopCallbackFunctionIndex(END_OF_DAY_1);
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/game", endOfDayCallback1);

// first end of day callback
void endOfDayCallback1(void) {
    
    u8 tempTime;

    if (checkMapRGBADone(MAIN_MAP_INDEX) || !(mainMap[MAIN_MAP_INDEX].mapState.flags & MAP_ACTIVE)) {
        
        clearHeldItemsAtEndOfDay();
        
        // earthquake
        if (checkEarthquakeShouldHappen()) {
            playSfx(RUMBLE);
            setMainLoopCallbackFunctionIndex(WAIT_AUDIO_FINISH);
            gameLoopContext.callbackIndex = END_OF_DAY_1;
            return;
        }
        
        setSpawnPoint(HOUSE_SPAWN_POINT_3);
        loadLevel(2);
        
        // dream cutscenes
        if (setDreamCutscenes()) {
            setMainLoopCallbackFunctionIndex(DREAM_CUTSCENES);
            return;
        }
        
        if (!checkDailyEventBit(DAILY_SHIPMENT)) {
            handleDailyShipment();
        }
        
        if (checkFatigueLevel() != 3) {
            
            tempTime = gHour;
            
            if (tempTime < 6) {
                tempTime += 24;
            }
            
            if (checkLifeEventBit(HAVE_CUSHION)) {
                gPlayer.fatigueCounter += adjustValue(gPlayer.fatigueCounter, -((30 - tempTime) * 3), 100);
            } else {
                gPlayer.fatigueCounter += adjustValue(gPlayer.fatigueCounter, -((30 - tempTime) * 2), 100);
            }
        }
        
        if (17 < gHour && gHour < 23) {
            setLifeEventBit(WENT_TO_BED_EARLY);
        } else {
            clearLifeEventBit(WENT_TO_BED_EARLY);
        }
         
        setClockNewDay(); 
        
        gPlayer.currentStamina = gMaximumStamina;
        
        startNewDay();
        setSpawnPoint(HOUSE_SPAWN_POINT_3);
        
        setMainLoopCallbackFunctionIndex(MAP_LOAD);
        
        setDailyEventBit(EAT_BREAKFAST);
        setDailyEventBit(DEFAULT_MORNING);

    }
}

//INCLUDE_ASM("asm/nonmatchings/game/game", endOfDayCallback2);

// second end of day callback
void endOfDayCallback2(void) {
    
    if (checkMapRGBADone(MAIN_MAP_INDEX) || !(mainMap[MAIN_MAP_INDEX].mapState.flags & MAP_ACTIVE)) {

        clearHeldItemsAtEndOfDay();

        if (gPlayer.flags & PLAYER_RIDING_HORSE) {
            gPlayer.flags &= ~PLAYER_RIDING_HORSE;
            clearDailyEventBit(RIDING_HORSE);
            setHorseLocation(0xFF);
            horseInfo.flags &= ~HORSE_BEING_RODE;
        }

        // earthquake
        if (checkEarthquakeShouldHappen()) {
            
            playSfx(RUMBLE);
            
            setMainLoopCallbackFunctionIndex(WAIT_AUDIO_FINISH);
            gameLoopContext.callbackIndex = END_OF_DAY_1;

            return;

        }

        setSpawnPoint(HOUSE_SPAWN_POINT_3);
        
        // setup/handle loading
        loadLevel(2);

        // dream cutscenes
        if (setDreamCutscenes()) {
            setMainLoopCallbackFunctionIndex(DREAM_CUTSCENES);
            return;
        }

        if (!checkDailyEventBit(DAILY_SHIPMENT)) {
            // handle daily shipment
            handleDailyShipment();
        }

        startNewDay();

        setSpawnPoint(HOUSE_SPAWN_POINT_3);
        
        setMainLoopCallbackFunctionIndex(MAP_LOAD);

        setDailyEventBit(EAT_BREAKFAST);
        setDailyEventBit(DEFAULT_MORNING);

    }
}

//INCLUDE_ASM("asm/nonmatchings/game/game", setFestivalDailyBits);

void setFestivalDailyBits(void) {

    // New Year's Festival
    if (gSeason == SPRING && gDayOfMonth == 1) {
        setDailyEventBit(FESTIVAL_DAY_CUTSCENE_GUARD);
        setDailyEventBit(DAY_CONSUMING_CUTSCENE);
        setDailyEventBit(FESTIVAL);
        setDailyEventBit(FORCE_SUNNY_WEATHER_DAILY);
    }
    
    // Planting Festival
    if (gSeason == SPRING && gDayOfMonth == 8) {
        setDailyEventBit(FESTIVAL_DAY_CUTSCENE_GUARD);
        setDailyEventBit(DAY_CONSUMING_CUTSCENE);
        setDailyEventBit(FESTIVAL);
        setDailyEventBit(FORCE_SUNNY_WEATHER_DAILY);
    }
    
    // Horse race spring
    if (gSeason == SPRING && gDayOfMonth == 17) {
        setDailyEventBit(FESTIVAL_DAY_CUTSCENE_GUARD);
        setDailyEventBit(DAY_CONSUMING_CUTSCENE);
        setDailyEventBit(FESTIVAL);
        setDailyEventBit(FORCE_SUNNY_WEATHER_DAILY);
    }

    // Horse race autumn
    if (gSeason == AUTUMN && gDayOfMonth == 28) {
        setDailyEventBit(FESTIVAL_DAY_CUTSCENE_GUARD);
        setDailyEventBit(DAY_CONSUMING_CUTSCENE);
        setDailyEventBit(FESTIVAL);
        setDailyEventBit(FORCE_SUNNY_WEATHER_DAILY);
    }
    
    // Flower Festival
    if (gSeason == SPRING && gDayOfMonth == 23) {
        setDailyEventBit(FESTIVAL_DAY_CUTSCENE_GUARD);
        setDailyEventBit(DAY_CONSUMING_CUTSCENE);
        setDailyEventBit(FESTIVAL);
        setDailyEventBit(FORCE_SUNNY_WEATHER_DAILY);
    }

    // Fireworks
    if (gSeason == SUMMER && gDayOfMonth == 1) {
        setDailyEventBit(FESTIVAL_DAY_CUTSCENE_GUARD);
        setDailyEventBit(FORCE_SUNNY_WEATHER_DAILY);
    }
    
    // Vegetable Festival
    if (gSeason == SUMMER && gDayOfMonth == 9) {
        setDailyEventBit(FESTIVAL_DAY_CUTSCENE_GUARD);
        setDailyEventBit(DAY_CONSUMING_CUTSCENE);
        setDailyEventBit(FESTIVAL);
        setDailyEventBit(FORCE_SUNNY_WEATHER_DAILY);
    }

    // Firefly Festival
    if (gSeason == SUMMER && gDayOfMonth == 17) {
        setDailyEventBit(FESTIVAL_DAY_CUTSCENE_GUARD);
        setDailyEventBit(FORCE_SUNNY_WEATHER_DAILY);
    }

    // Swimming Festival
    if (gSeason == SUMMER && gDayOfMonth == 24) {
        setDailyEventBit(FESTIVAL_DAY_CUTSCENE_GUARD);
        setDailyEventBit(DAY_CONSUMING_CUTSCENE);
        setDailyEventBit(FESTIVAL);
        setDailyEventBit(FORCE_SUNNY_WEATHER_DAILY);
    }

    // Cow Festival
    if (gSeason == AUTUMN && gDayOfMonth == 4) {
        setDailyEventBit(FESTIVAL_DAY_CUTSCENE_GUARD);
        setDailyEventBit(DAY_CONSUMING_CUTSCENE);
        setDailyEventBit(FESTIVAL);
        setDailyEventBit(FORCE_SUNNY_WEATHER_DAILY);
    }

    // Harvest Festival
    if (gSeason == AUTUMN && gDayOfMonth == 12) {
        setDailyEventBit(FESTIVAL_DAY_CUTSCENE_GUARD);
        setDailyEventBit(DAY_CONSUMING_CUTSCENE);
        setDailyEventBit(FESTIVAL);
        setDailyEventBit(FORCE_SUNNY_WEATHER_DAILY);
    }
    
    // Egg Festival
    if (gSeason == AUTUMN && gDayOfMonth == 20) {
        setDailyEventBit(FESTIVAL_DAY_CUTSCENE_GUARD);
        setDailyEventBit(DAY_CONSUMING_CUTSCENE);
        setDailyEventBit(FESTIVAL);
        setDailyEventBit(FORCE_SUNNY_WEATHER_DAILY);
    }

    // Thanksgiving
    if (gSeason == WINTER && gDayOfMonth == 10) {
        setDailyEventBit(FESTIVAL_DAY_CUTSCENE_GUARD);
        setDailyEventBit(FORCE_SUNNY_WEATHER_DAILY);
    }
    
    // Dog Race
    if (gSeason == WINTER && gDayOfMonth == 19) {
        setDailyEventBit(FESTIVAL_DAY_CUTSCENE_GUARD);
        setDailyEventBit(DAY_CONSUMING_CUTSCENE);
        setDailyEventBit(FESTIVAL);
        setDailyEventBit(FORCE_SUNNY_WEATHER_DAILY);
    }

    // Starry Night Festival
    if (gSeason == WINTER && gDayOfMonth == 24) {
        setDailyEventBit(FESTIVAL_DAY_CUTSCENE_GUARD);
        setDailyEventBit(FORCE_SUNNY_WEATHER_DAILY);
    }
    
    // Spirit Festival
    if (gSeason == WINTER && gDayOfMonth == 27) {
        setDailyEventBit(FESTIVAL_DAY_CUTSCENE_GUARD);
        setDailyEventBit(FORCE_SUNNY_WEATHER_DAILY);
    }

    // New Year's Eve
    if (gSeason == WINTER && gDayOfMonth == 30) {
        setDailyEventBit(FESTIVAL_DAY_CUTSCENE_GUARD);
        setDailyEventBit(FORCE_SUNNY_WEATHER_DAILY);
    }

    // Wedding
    if (gDayOfWeek == SUNDAY && checkLifeEventBit(ENGAGED)) {
        setDailyEventBit(FORCE_SUNNY_WEATHER_DAILY);
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/game", checkEarthquakeShouldHappen);

bool checkEarthquakeShouldHappen(void) {

    bool result = FALSE;

    if (!checkLifeEventBit(EARTHQUAKE_HAPPENED) && gSeason > AUTUMN && (5 < gDayOfMonth && gDayOfMonth < 10)) {

        if (!getRandomNumberInRange(0, 2) || gDayOfMonth == 9) {
            setLifeEventBit(EARTHQUAKE_HAPPENED);
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

//INCLUDE_ASM("asm/nonmatchings/game/game", clearHeldItemsAtEndOfDay);

void clearHeldItemsAtEndOfDay(void) {
    
    if (gPlayer.heldItem) {
        
        clearHeldItemSlot(gPlayer.itemInfoIndex);

        switch (gPlayer.heldItem) {
            case 0x60 ... 0x6F:
                resetChickenLocation(0xFF, gPlayer.heldAnimalIndex);
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

//INCLUDE_ASM("asm/nonmatchings/game/game", checkBacheloretteReadyForMarriage);

bool checkBacheloretteReadyForMarriage(void) {
    
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
            gHarvestKingName[0] = gPlayer.name[0];
            gHarvestKingName[1] = gPlayer.name[1];
            gHarvestKingName[2] = gPlayer.name[2];
            gHarvestKingName[3] = gPlayer.name[3];
            gHarvestKingName[4] = gPlayer.name[4];
            gHarvestKingName[5] = gPlayer.name[5];
            break;
        case 1:
            gHarvestKingName[0] = char_H;
            gHarvestKingName[1] = char_a;
            gHarvestKingName[2] = char_r;
            gHarvestKingName[3] = char_r;
            gHarvestKingName[4] = char_i;
            gHarvestKingName[5] = char_s;
            break;
        default:
            break;
        case 2:
            gHarvestKingName[0] = char_G;
            gHarvestKingName[1] = char_r;
            gHarvestKingName[2] = char_a;
            gHarvestKingName[3] = char_y;
            gHarvestKingName[4] = 0xFF;
            gHarvestKingName[5] = 0xFF;
            break;
        case 3:
            gHarvestKingName[0] = char_J;
            gHarvestKingName[1] = char_e;
            gHarvestKingName[2] = char_f;
            gHarvestKingName[3] = char_f;
            gHarvestKingName[4] = 0xFF;
            gHarvestKingName[5] = 0xFF;
            break;
        case 4:
            gHarvestKingName[0] = char_C;
            gHarvestKingName[1] = char_l;
            gHarvestKingName[2] = char_i;
            gHarvestKingName[3] = char_f;
            gHarvestKingName[4] = char_f;
            gHarvestKingName[5] = 0xFF;
            break;
        case 5:
            gHarvestKingName[0] = char_K;
            gHarvestKingName[1] = char_a;
            gHarvestKingName[2] = char_i;
            gHarvestKingName[3] = 0xFF;
            gHarvestKingName[4] = 0xFF;
            gHarvestKingName[5] = 0xFF;
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

//INCLUDE_ASM("asm/nonmatchings/game/game", handlePurchaseHouseExtension);

// returns cutscene index
u8 handlePurchaseHouseExtension(u8 houseExtensionIndex) {

    s16 prices[6];
    s16 lumberCosts[6];

    memcpy(prices, houseExtensionPrices, 12);
    memcpy(lumberCosts, houseExtensionLumberCosts, 12);

    if (adjustValue(gLumber, lumberCosts[houseExtensionIndex], MAX_LUMBER) == lumberCosts[houseExtensionIndex]) {
        
        if (adjustValue(gGold, prices[houseExtensionIndex], MAX_GOLD) == prices[houseExtensionIndex]) {

            gLumber += adjustValue(gLumber, lumberCosts[houseExtensionIndex], MAX_LUMBER);
            gGold += adjustValue(gGold, prices[houseExtensionIndex], MAX_GOLD);

            houseExtensionConstructionCounter = 0;
            
            setDailyEventBit(CARPENTER_ESTIMATE);
            
            return 0;

        } else {
            return 1;
        }
        
    }

    return 2;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/game", setFlowerFestivalGoddess);

void setFlowerFestivalGoddess(void) {
    
    u8 temp;

    if (gVoteForFlowerFestivalGoddess < 5 && !getRandomNumberInRange(0, 2)) {
        temp = gVoteForFlowerFestivalGoddess;
    } else {
        temp = getRandomNumberInRange(0, 4);
    }
    
    gFlowerFestivalGoddess = temp;
    
    if (checkLifeEventBit(KAREN_GONE) && gFlowerFestivalGoddess == KAREN) {
        gFlowerFestivalGoddess = MARIA;
    }

}

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

//INCLUDE_ASM("asm/nonmatchings/game/game", setSpritiFestivalRecruits);

inline void setSpritiFestivalRecruits(u8 numberOfSpiritFestivalAssistants, u8 npcIndex) {
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
    setSpritiFestivalRecruits(numberOfSpiritFestivalAssistantsRecruited, npcIndex);
}

//INCLUDE_ASM("asm/nonmatchings/game/game", recruitSpiritFestivalAssistants);

void recruitSpiritFestivalAssistants(void) {
    
    if (checkSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_MARIA_DIALOGUE)) {
        clearSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_MARIA_DIALOGUE);
        setSpiritFestivalAssistant(MARIA+1);
    }
    
    if (checkSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_POPURI_DIALOGUE)) {
        clearSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_POPURI_DIALOGUE);
        setSpiritFestivalAssistant(ELLI+1);
    }

    if (checkSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_ELLI_DIALOGUE)) {
        clearSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_ELLI_DIALOGUE);
        setSpiritFestivalAssistant(POPURI+1);
    }
    
    if (checkSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_ANN_DIALOGUE)) {
        clearSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_ANN_DIALOGUE);
        setSpiritFestivalAssistant(ANN+1);
    }
    
   if (checkSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_KAREN_DIALOGUE)) {
        clearSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_KAREN_DIALOGUE);
        setSpiritFestivalAssistant(KAREN+1);
    } 

   if (checkSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_HARRIS_DIALOGUE)) {
        clearSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_HARRIS_DIALOGUE);
        setSpiritFestivalAssistant(HARRIS);
    }

   if (checkSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_GRAY_DIALOGUE)) {
        clearSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_GRAY_DIALOGUE);
        setSpiritFestivalAssistant(GRAY);
    } 

   if (checkSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_JEFF_DIALOGUE)) {
        clearSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_JEFF_DIALOGUE);
        setSpiritFestivalAssistant(JEFF);
    } 

   if (checkSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_CLIFF_DIALOGUE)) {
        clearSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_CLIFF_DIALOGUE);
        setSpiritFestivalAssistant(CLIFF);
    } 

   if (checkSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_KAI_DIALOGUE)) {
        clearSpecialDialogueBit(SPIRIT_FESTIVAL_RECRUITING_KAI_DIALOGUE);
        setSpiritFestivalAssistant(KAI);

    } 
    
    if (numberOfSpiritFestivalAssistantsRecruited >= 3) {
        clearSpecialDialogueBit(PLAYER_HARVEST_KING_SPIRIT_FESTIVAL_ASSISTANTS_DIALOGUE);
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/game", setRecipes);

void setRecipes(void) {

    if (checkSpecialDialogueBit(CREAM_OF_TURNIP_STEW_RECIPE_DIALOGUE)) {
        addRecipe(RECIPE_CREAM_OF_TURNIP_STEW);
    }
    if (checkSpecialDialogueBit(EASY_TOMATO_SOUP_RECIPE_DIALOGUE)) {
        addRecipe(RECIPE_EASY_TOMATO_SOUP);
    }
    if (checkSpecialDialogueBit(TOMATO_RICE_RECIPE_DIALOGUE)) {
        addRecipe(RECIPE_TOMATO_RICE);
    }
    if (checkSpecialDialogueBit(TOMATO_SOUP_RECIPE_DIALOGUE)) {
        addRecipe(RECIPE_TOMATO_SOUP);
    }
    if (checkSpecialDialogueBit(CORN_FRITTER_RECIPE_DIALOGUE)) {
        addRecipe(RECIPE_CORN_FRITTER);
    }
    if (checkSpecialDialogueBit(CORN_PASTA_RECIPE_DIALOGUE)) {
        addRecipe(RECIPE_CORN_PASTA);
    }
    if (checkSpecialDialogueBit(MASHED_POTATO_RECIPE_DIALOGUE)) {
        addRecipe(RECIPE_MASHED_POTATO);
    }
    if (checkSpecialDialogueBit(FRIED_POTATOES_AND_BACON_RECIPE_DIALOGUE)) {
        addRecipe(RECIPE_FRIED_POTATOES_AND_BACON);
    }
    if (checkSpecialDialogueBit(VEGETABLE_TOMATO_STEW_RECIPE_DIALOGUE)) {
        addRecipe(RECIPE_VEGETABLE_TOMATO_STEW);
    }
    if (checkSpecialDialogueBit(GARLIC_POTATO_BEEF_RECIPE_DIALOGUE)) {
        addRecipe(RECIPE_GARLIC_POTATO_BEEF);
    }
    if (checkSpecialDialogueBit(EGGPLANT_WITH_MISO_PASTE_RECIPE_DIALOGUE)) {
        addRecipe(RECIPE_EGGPLANT_WITH_MISO_PASTE);
    }
    if (checkSpecialDialogueBit(ROLLED_CABBAGE_RECIPE_DIALOGUE)) {
        addRecipe(RECIPE_ROLLED_CABBAGE);
    }
    if (checkSpecialDialogueBit(STUFFED_OMELET_RECIPE_DIALOGUE)) {
        addRecipe(RECIPE_STUFFED_OMELET);
    }
    if (checkSpecialDialogueBit(SPA_POACHED_EGG_RECIPE_DIALOGUE)) {
        addRecipe(RECIPE_SPA_POACHED_EGG);
    }
    if (checkSpecialDialogueBit(HANDMADE_BUTTER_RECIPE_DIALOGUE)) {
        addRecipe(RECIPE_HANDMADE_BUTTER);
    }
    if (checkSpecialDialogueBit(MUSHROOM_RICE_RECIPE_DIALOGUE)) {
        addRecipe(RECIPE_MUSHROOM_RICE);
    }
    if (checkSpecialDialogueBit(FRIED_CHAR_RECIPE_DIALOGUE)) {
        addRecipe(RECIPE_FRIED_CHAR);
    }
    if (checkSpecialDialogueBit(GRILLED_TROUT_CHEESE_RECIPE_DIALOGUE)) {
        addRecipe(RECIPE_GRILLED_TROUT_CHEESE);
    }
    if (checkSpecialDialogueBit(MUSHROOM_STUFFED_CHAR_RECIPE_DIALOGUE)) {
        addRecipe(RECIPE_MUSHROOM_STUFFED_CHAR);
    }
    if (checkSpecialDialogueBit(STEAMED_CLAM_WITH_WINE_RECIPE_DIALOGUE)) {
        addRecipe(RECIPE_STEAMED_CLAM_WITH_WINE);
    }
    if (checkSpecialDialogueBit(MISO_SOUP_WITH_SPROUTS_RECIPE_DIALOGUE)) {
        addRecipe(RECIPE_MISO_SOUP_WITH_SPROUTS);
    }
    if (checkSpecialDialogueBit(SESAME_DANDELION_GREENS_RECIPE_DIALOGUE)) {
        addRecipe(RECIPE_SESAME_DANDELION_GREENS);
    }
    if (checkSpecialDialogueBit(MUSHROOM_SALSA_RECIPE_DIALOGUE)) {
        addRecipe(RECIPE_MUSHROOM_SALSA);
    }
    if (checkSpecialDialogueBit(STRAWBERRY_DOG_RECIPE_DIALOGUE)) {
        addRecipe(RECIPE_STRAWBERRY_DOG);
    }
    if (checkSpecialDialogueBit(WALNUT_CAKE_RECIPE_DIALOGUE)) {
        addRecipe(RECIPE_WALNUT_CAKE);
    }
    if (checkSpecialDialogueBit(BREAD_PUDDING_RECIPE_DIALOGUE)) {
        addRecipe(RECIPE_BREAD_PUDDING);
    }
    if (checkSpecialDialogueBit(HERB_RICE_CAKE_RECIPE_DIALOGUE)) {
        addRecipe(RECIPE_HERB_RICE_CAKE);
    }
    if (checkSpecialDialogueBit(POTATO_PANCAKE_RECIPE_DIALOGUE)) {
        addRecipe(RECIPE_POTATO_PANCAKE);
    }
    if (checkSpecialDialogueBit(STRAWBERRY_JAM_RECIPE_DIALOGUE)) {
        addRecipe(RECIPE_STRAWBERRY_JAM);
    }
    if (checkSpecialDialogueBit(STRAWBERRY_CHAMPAGNE_RECIPE_DIALOGUE)) {
        addRecipe(RECIPE_STRAWBERRY_CHAMPAGNE);
    }
    if (checkSpecialDialogueBit(VERYBERRY_WINE_RECIPE_DIALOGUE)) {
        addRecipe(RECIPE_VERYBERRY_WINE);
    }
    if (checkSpecialDialogueBit(SPICE_TEA_RECIPE_DIALOGUE)) {
        addRecipe(RECIPE_SPICE_TEA);
    }
    if (checkSpecialDialogueBit(HOT_SPICY_WINE_RECIPE_DIALOGUE)) {
        addRecipe(RECIPE_HOT_SPICY_WINE);
    }
    if (checkSpecialDialogueBit(CINNAMON_MILK_TEA_RECIPE_DIALOGUE)) {
        addRecipe(RECIPE_CINNAMON_MILK_TEA);
    }
    if (checkSpecialDialogueBit(PICKLED_TURNIPS_AND_CABBAGE_RECIPE_DIALOGUE)) {
        addRecipe(RECIPE_PICKLED_TURNIPS_AND_CABBAGE);
    }

}

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

            clearLifeEventBit(HOUSE_EXTENSION_CONSTRUCTION);
            setDailyEventBit(CARPENTER_FINISHED);
            setLifeEventBit(buffer2[day]);

            if (day == 3) {
                clearLifeEventBit(GREENHOUSE_DESTROYED);
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
            
            clearLifeEventBit(HOUSE_EXTENSION_CONSTRUCTION);
            setDailyEventBit(CARPENTER_FINISHED);
            setLifeEventBit(((u16*)(&lifeEventBitArray))[day]);
            
            if (day == 3) {
                clearLifeEventBit(GREENHOUSE_DESTROYED);
            }
            
            setAnimalLocations(animalLocationsArrayPtr[day]);
            result = TRUE;
            
        }
    }
    
    return result;
}
*/

//INCLUDE_ASM("asm/nonmatchings/game/game", setLetters);

void setLetters(void) {

    if (!checkMailRead(MAIL_MARIA_SPRING) && (!checkLifeEventBit(MARRIED) || gWife != MARIA) && npcAffection[MARIA] >= 120 && gSeason == SPRING && !getRandomNumberInRange(0, 10)) {
        setMail(MAIL_MARIA_SPRING);
    }

    if (!checkMailRead(MAIL_MARIA_SUMMER) && (!checkLifeEventBit(MARRIED) || gWife != MARIA) && npcAffection[MARIA] >= 140 && gSeason == SUMMER && !getRandomNumberInRange(0, 10)) {
        setMail(MAIL_MARIA_SUMMER);
    }

    if (!checkMailRead(MAIL_MARIA_AUTUMN) && (!checkLifeEventBit(MARRIED) || gWife != MARIA) && npcAffection[MARIA] >= 160 && gSeason == AUTUMN && !getRandomNumberInRange(0, 10)) {
        setMail(MAIL_MARIA_AUTUMN);
    }

    if (!checkMailRead(MAIL_MARIA_WINTER) && (!checkLifeEventBit(MARRIED) || gWife != MARIA) && npcAffection[MARIA] >= 180 && gSeason == WINTER && !getRandomNumberInRange(0, 10)) {
        setMail(MAIL_MARIA_WINTER);
    }

    if (!checkMailRead(MAIL_POPURI_SPRING) && (!checkLifeEventBit(MARRIED) || gWife != POPURI) && npcAffection[POPURI] >= 120 && gSeason == SPRING && !getRandomNumberInRange(0, 10)) {
        setMail(MAIL_POPURI_SPRING);
    }

    if (!checkMailRead(MAIL_POPURI_SUMMER) && (!checkLifeEventBit(MARRIED) || gWife != POPURI) && npcAffection[POPURI] >= 140 && gSeason == SUMMER && !getRandomNumberInRange(0, 10)) {
        setMail(MAIL_POPURI_SUMMER);
    }
    if (!checkMailRead(MAIL_POPURI_AUTUMN) && (!checkLifeEventBit(MARRIED) || gWife != POPURI) && npcAffection[POPURI] >= 160 && gSeason == AUTUMN && !getRandomNumberInRange(0, 10)) {
        setMail(MAIL_POPURI_AUTUMN);
    }

    if (!checkMailRead(MAIL_POPURI_WINTER) && (!checkLifeEventBit(MARRIED) || gWife != POPURI) && npcAffection[POPURI] >= 180 && gSeason == WINTER && !getRandomNumberInRange(0, 10)) {
        setMail(MAIL_POPURI_WINTER);
    }

    if (!checkMailRead(MAIL_ELLI_SPRING) && (!checkLifeEventBit(MARRIED) || gWife != ELLI) && npcAffection[ELLI] >= 120 && gSeason == SPRING && !getRandomNumberInRange(0, 10)) {
        setMail(MAIL_ELLI_SPRING);
    }

    if (!checkMailRead(MAIL_ELLI_SUMMER) && (!checkLifeEventBit(MARRIED) || gWife != ELLI) && npcAffection[ELLI] >= 140 && gSeason == SUMMER && !getRandomNumberInRange(0, 10)) {
        setMail(MAIL_ELLI_SUMMER);
    }

    if (!checkMailRead(MAIL_ELLI_AUTUMN) && (!checkLifeEventBit(MARRIED) || (gWife != ELLI)) && npcAffection[ELLI] >= 160 && gSeason == AUTUMN && !getRandomNumberInRange(0, 10)) {
        setMail(MAIL_ELLI_AUTUMN);
    }

    if (!checkMailRead(MAIL_ELLI_WINTER) && (!checkLifeEventBit(MARRIED) || gWife != ELLI) && npcAffection[ELLI] >= 180 && gSeason == WINTER && !getRandomNumberInRange(0, 10)) {
        setMail(MAIL_ELLI_WINTER);
    }

    if (!checkMailRead(MAIL_ANN_SPRING) && (!checkLifeEventBit(MARRIED) || gWife != ANN) && npcAffection[ANN] >= 120 && gSeason == SPRING && !getRandomNumberInRange(0, 10)) {
        setMail(MAIL_ANN_SPRING);
    }

    if (!checkMailRead(MAIL_ANN_SUMMER) && (!checkLifeEventBit(MARRIED) || gWife != ANN) && npcAffection[ANN] >= 140 && gSeason == SUMMER && !getRandomNumberInRange(0, 10)) {
        setMail(MAIL_ANN_SUMMER);
    }

    if (!checkMailRead(MAIL_ANN_AUTUMN) && (!checkLifeEventBit(MARRIED) || gWife != ANN) && npcAffection[ANN] >= 160 && (gSeason == AUTUMN) && !getRandomNumberInRange(0, 10)) {
        setMail(MAIL_ANN_AUTUMN);
    }

    if (!checkMailRead(MAIL_ANN_WINTER) && (!checkLifeEventBit(MARRIED) || gWife != ANN) && npcAffection[ANN] >= 180 && gSeason == WINTER && !getRandomNumberInRange(0, 10)) {
        setMail(MAIL_ANN_WINTER);
    }

    if (!checkMailRead(MAIL_KAREN_SPRING) && (!checkLifeEventBit(MARRIED) || gWife != KAREN) && npcAffection[KAREN] >= 120 && gSeason == SPRING && !getRandomNumberInRange(0, 10)) {
        setMail(MAIL_KAREN_SPRING);
    }

    if (!checkMailRead(MAIL_KAREN_SUMMER) && (!checkLifeEventBit(MARRIED) || gWife != KAREN) && npcAffection[KAREN] >= 140 && gSeason == SUMMER && !getRandomNumberInRange(0, 10)) {
        setMail(MAIL_KAREN_SUMMER);
    }

    if (!checkMailRead(MAIL_KAREN_AUTUMN) && (!checkLifeEventBit(MARRIED) || gWife != KAREN) && npcAffection[KAREN] >= 160 && gSeason == AUTUMN && !getRandomNumberInRange(0, 10)) {
        setMail(MAIL_KAREN_AUTUMN);
    }

    if (!checkMailRead(MAIL_KAREN_WINTER) && (!checkLifeEventBit(MARRIED) || gWife != KAREN) && npcAffection[KAREN] >= 180 && gSeason == WINTER && !getRandomNumberInRange(0, 10)) {
        setMail(MAIL_KAREN_WINTER);
    }

    if (!checkMailRead(MAIL_MAYOR_SPRING) && npcAffection[MAYOR] >= 120 && gSeason == SPRING && !getRandomNumberInRange(0, 10)) {
        setMail(MAIL_MAYOR_SPRING);
    }

    if (!checkMailRead(MAIL_HARRIS_SPRING) && npcAffection[HARRIS] >= 120 && gSeason == SPRING && !getRandomNumberInRange(0, 10)) {
        setMail(MAIL_HARRIS_SPRING);
    }

    if (!checkMailRead(MAIL_MARIA_HARRIS_BABY) && checkLifeEventBit(MARIA_HARRIS_HAVE_BABY)) {
        setMail(MAIL_MARIA_HARRIS_BABY);
    }

    if (!checkMailRead(MAIL_GRAY_SPRING) && npcAffection[GRAY] >= 120 && gSeason == SPRING && !getRandomNumberInRange(0, 10)) {
        setMail(MAIL_GRAY_SPRING);
    }

    if (!checkMailRead(MAIL_POPURI_GRAY_BABY) && checkLifeEventBit(POPURI_GRAY_HAVE_BABY)) {
        setMail(MAIL_POPURI_GRAY_BABY);
    }

    if (!checkMailRead(MAIL_JEFF_SPRING) && npcAffection[JEFF] >= 120 && gSeason == SPRING && !getRandomNumberInRange(0, 10)) {
        setMail(MAIL_JEFF_SPRING);
    }

    if (!checkMailRead(MAIL_ELLI_JEFF_BABY) && checkLifeEventBit(ELLI_JEFF_HAVE_BABY)) {
        setMail(MAIL_ELLI_JEFF_BABY);
    }

    if (!checkMailRead(MAIL_CLIFF_GONE) && checkLifeEventBit(CLIFF_GONE) && !getRandomNumberInRange(0, 10)) {
        setMail(MAIL_CLIFF_GONE);
    }

    if (!checkMailRead(MAIL_ANN_CLIFF_BABY) && checkLifeEventBit(ANN_CLIFF_HAVE_BABY)) {
        setMail(MAIL_ANN_CLIFF_BABY);
    }

    if (!checkMailRead(MAIL_KAI_SPRING) && npcAffection[KAI] >= 120 && gSeason == SPRING && !getRandomNumberInRange(0, 10)) {
        setMail(MAIL_KAI_SPRING);
    }

    if (!checkMailRead(MAIL_KAREN_KAI_BABY) && checkLifeEventBit(KAREN_KAI_HAVE_BABY)) {
        setMail(MAIL_KAREN_KAI_BABY);
    }

    if (!checkMailRead(MAIL_KAI_GONE) && checkLifeEventBit(KAI_GONE) && !getRandomNumberInRange(0, 10)) {
        setMail(MAIL_KAI_GONE);
    }

    if (!checkMailRead(MAIL_KENT_STU_SPRING) && (npcAffection[KENT] + npcAffection[STU]) >= 160 && gSeason == SPRING && !getRandomNumberInRange(0, 10)) {
        setMail(MAIL_KENT_STU_SPRING);
    }

    if (!checkMailRead(MAIL_RICK_SPRING) && npcAffection[RICK] >= 120 && gSeason == SPRING && !getRandomNumberInRange(0, 10)) {
        setMail(MAIL_RICK_SPRING);
    }

    if (!checkMailRead(MAIL_BASIL_WINTER) && npcAffection[BASIL] >= 100 && gSeason == WINTER && !getRandomNumberInRange(0, 10)) {
        setMail(MAIL_BASIL_WINTER);
    }

    if (!checkMailRead(MAIL_SHADY_SALESMAN_GOLD_PENDANT) && checkLifeEventBit(SHADY_SALESMAN_MARIA_CUTSCENE)) {
        setMail(MAIL_SHADY_SALESMAN_GOLD_PENDANT);
    }

    if (!checkMailRead(MAIL_WEDDING_MARIA) && checkLifeEventBit(MARRIED) && gWife == MARIA) {
        setMail(MAIL_WEDDING_MARIA);
    }

    if (!checkMailRead(MAIL_WEDDING_POPURI) && checkLifeEventBit(MARRIED) && (gWife == POPURI)) {
        setMail(MAIL_WEDDING_POPURI);
    }

    if (!checkMailRead(MAIL_WEDDING_ELLI) && checkLifeEventBit(MARRIED) && (gWife == ELLI)) {
        setMail(MAIL_WEDDING_ELLI);
    }

    if (!checkMailRead(MAIL_WEDDING_ANN) && checkLifeEventBit(MARRIED) && (gWife == ANN)) {
        setMail(MAIL_WEDDING_ANN);
    }

    if (!checkMailRead(MAIL_WEDDING_KAREN) && checkLifeEventBit(MARRIED) && (gWife == KAREN)) {
        setMail(MAIL_WEDDING_KAREN);
    }

    if (!checkMailRead(MAIL_BIRTH_MARIA) && checkLifeEventBit(HAVE_BABY) && (gWife == MARIA)) {
        setMail(MAIL_BIRTH_MARIA);
    }

    if (!checkMailRead(MAIL_BIRTH_POPURI) && checkLifeEventBit(HAVE_BABY) && (gWife == POPURI)) {
        setMail(MAIL_BIRTH_POPURI);
    }

    if (!checkMailRead(MAIL_BIRTH_ELLI) && checkLifeEventBit(HAVE_BABY) && (gWife == ELLI)) {
        setMail(MAIL_BIRTH_ELLI);
    }

    if (!checkMailRead(MAIL_BIRTH_ANN) && checkLifeEventBit(HAVE_BABY) && (gWife == ANN)) {
        setMail(MAIL_BIRTH_ANN);
    }

    if (!checkMailRead(MAIL_BIRTH_KAREN) && checkLifeEventBit(HAVE_BABY) && (gWife == KAREN)) {
        setMail(MAIL_BIRTH_KAREN);
    }

    if (!checkMailRead(MAIL_PHOTO_SOWING_FESTIVAL) && checkDailyEventBit(SOWING_FESTIVAL_BALLOON_RIDE_PHOTO_MAIL)) {
        setMail(MAIL_PHOTO_SOWING_FESTIVAL);
    }

    if (!checkMailRead(MAIL_PHOTO_HORSE_RACE) && checkDailyEventBit(WON_HORSE_RACE_PHOTO_MAIL)) {
        setMail(MAIL_PHOTO_HORSE_RACE);
    }

    if (!checkMailRead(MAIL_PHOTO_SEA_FESTIVAL) && checkDailyEventBit(WON_SEA_FESTIVAL_PHOTO_MAIL)) {
        setMail(MAIL_PHOTO_SEA_FESTIVAL);
    }

    if (!checkMailRead(MAIL_PHOTO_DOG_RACE) && checkDailyEventBit(WON_DOG_RACE_PHOTO_MAIL)) {
        setMail(MAIL_PHOTO_DOG_RACE);
    }

    if (!checkMailRead(MAIL_PHOTO_HOT_SPRINGS) && checkDailyEventBit(HOT_SPRINGS_COMPLETED_PHOTO_MAIL)) {
        setMail(MAIL_PHOTO_HOT_SPRINGS);
    }

    if (!checkMailRead(MAIL_PHOTO_HOME_EXTENSIONS) && checkLifeEventBit(HAVE_KITCHEN) && checkLifeEventBit(HAVE_BATHROOM) && checkLifeEventBit(HAVE_STAIRS) && checkLifeEventBit(HAVE_GREENHOUSE) && checkLifeEventBit(HAVE_LOG_TERRACE) && checkLifeEventBit(HAVE_BABY_BED)) {
        setMail(MAIL_PHOTO_HOME_EXTENSIONS);
    }

    if (!checkMailRead(MAIL_PHOTO_COW_FESTIVAL) && checkDailyEventBit(WON_COW_FESTIVAL_PHOTO_MAIL)) {
        setMail(MAIL_PHOTO_COW_FESTIVAL);
    }

    if (!checkMailRead(MAIL_PHOTO_MARIA_FIREFLIES) && checkDailyEventBit(MARIA_FIREFLIES_PHOTO_MAIL)) {
        setMail(MAIL_PHOTO_MARIA_FIREFLIES);
    }

    if (!checkMailRead(MAIL_PHOTO_POPURI_BLUE_MIST) && checkDailyEventBit(POPURI_BLUE_MIST_PHOTO_MAIL)) {
        setMail(MAIL_PHOTO_POPURI_BLUE_MIST);
    }

    if (!checkMailRead(MAIL_PHOTO_ELLI_ESSENCE_OF_MOONDROP) && checkDailyEventBit(ELLI_ESSENCE_OF_MOONDROP_PHOTO_MAIL)) {
        setMail(MAIL_PHOTO_ELLI_ESSENCE_OF_MOONDROP);
    }

    if (!checkMailRead(MAIL_PHOTO_ANN_PIKA_BUNNY) && checkDailyEventBit(ANN_PIKA_BUNNY_PHOTO_MAIL)) {
        setMail(MAIL_PHOTO_ANN_PIKA_BUNNY);
    }

    if (!checkMailRead(MAIL_PHOTO_KAREN_KEIFU_FAIRIES) && checkDailyEventBit(KAREN_KEIFU_FAIRIES_PHOTO_MAIL)) {
        setMail(MAIL_PHOTO_KAREN_KEIFU_FAIRIES);
    }

    if (!checkMailRead(MAIL_RICK_CHEST_OF_DRAWERS) && ((gYear == 1 && 1 < gSeason) || 1 < gYear)) {
        setMail(MAIL_RICK_CHEST_OF_DRAWERS);
    }

    if (!checkMailRead(MAIL_PINK_CAT_MINT_SEEDS) && gSeason == SPRING && gDayOfMonth == 20) {
        setMail(MAIL_PINK_CAT_MINT_SEEDS);
    }

    if (!checkMailRead(MAIL_HORSE_RACE_NOTICE) && gSeason == SPRING && gDayOfMonth == 15) {
        setMail(MAIL_HORSE_RACE_NOTICE);
    }

    if (!checkMailRead(MAIL_GODDESS_VOTING) && gSeason == SPRING && gDayOfMonth == 19) {
        setMail(MAIL_GODDESS_VOTING);
    }

    if (!checkMailRead(MAIL_VEGETABLE_FESTIVAL_NOTICE) && gSeason == SUMMER && gDayOfMonth == 1) {
        setMail(MAIL_VEGETABLE_FESTIVAL_NOTICE);
    }

    if (!checkMailRead(MAIL_MOUNTAIN_CARPENTERS_AD) && !getRandomNumberInRange(0, 20)) {
        setMail(MAIL_MOUNTAIN_CARPENTERS_AD);
    }

    if (!checkMailRead(MAIL_LIBRARY_CLOSURE_SPRING) && gSeason == SPRING && gDayOfMonth == 30) {
        setMail(MAIL_LIBRARY_CLOSURE_SPRING);
    }

    if (!checkMailRead(MAIL_LIBRARY_CLOSURE_AUTUMN) && gSeason == AUTUMN && gDayOfMonth == 30) {
        setMail(MAIL_LIBRARY_CLOSURE_AUTUMN);
    }

    if (!checkMailRead(MAIL_HARVEST_FESTIVAL_NOTICE) && gSeason == AUTUMN && gDayOfMonth == 10) {
        setMail(MAIL_HARVEST_FESTIVAL_NOTICE);
    }

    if (!checkMailRead(MAIL_RICK_CARPETS) && gSeason == AUTUMN && gDayOfMonth == 1) {
        setMail(MAIL_RICK_CARPETS);
    }

    if (!checkMailRead(MAIL_ORE_MINE_OPENS) && gSeason == WINTER && gDayOfMonth == 7) {
        setMail(MAIL_ORE_MINE_OPENS);
    }

    if (!checkMailRead(MAIL_DOG_RACE_NOTICE) && gSeason == WINTER && gDayOfMonth == 6) {
        setMail(MAIL_DOG_RACE_NOTICE);
    }

    if (!checkMailRead(MAIL_HOT_SPRING_COMPLETED) && gSeason == WINTER && gDayOfMonth == 17) {
        setMail(MAIL_HOT_SPRING_COMPLETED);
    }

    if (!checkMailRead(MAIL_BLUE_MIST_SEEDS) && gYear >= 2 && gSeason == SPRING && gDayOfMonth == 24) {
        setMail(MAIL_BLUE_MIST_SEEDS);
    }

    if (!checkMailRead(MAIL_DAD_SPRING_CHECK_IN) && gYear >= 2 && gSeason == SPRING) {
        setMail(MAIL_DAD_SPRING_CHECK_IN);
    }

    if (!checkMailRead(MAIL_DAD_WEDDING_CONGRATS) && checkLifeEventBit(MARRIED)) {
        setMail(MAIL_DAD_WEDDING_CONGRATS);
    }

    if (!checkMailRead(MAIL_DAD_SPRING_VISIT) && gYear >= 3 && gSeason == SPRING && gDayOfMonth == 3) {
        setMail(MAIL_DAD_SPRING_VISIT);
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/game", getTextIndexFromLetterIndex);

u16 getTextIndexFromLetterIndex(u8 index) {

    u16 arr[80];
    
    memcpy(arr, mailTextIndices, 0xA0);

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

// INCLUDE_RODATA("asm/nonmatchings/game/game", mailTextIndices);

// text indices for letters
static const u16 mailTextIndices[79] = { 
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