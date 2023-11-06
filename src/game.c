#include "common.h"

#include "system/controller.h"
#include "system/cutscene.h"
#include "system/dialogue.h"
#include "system/map.h"
#include "system/mathUtils.h"
#include "system/memory.h"
#include "system/message.h"
#include "system/sprite.h"
#include "system/mapContext.h"

#include "animals.h"
#include "evaluation.h"
#include "game.h"
#include "gameAudio.h"
#include "gameStatus.h"
#include "initialize2.h"
#include "itemHandlers.h"
#include "level.h"
#include "mainLoop.h"
#include "mapObjects.h"
#include "namingScreen.h"
#include "npc.h"
#include "player.h"
#include "setCutscenes.h"
#include "shop.h"
#include "updateGame.h"
#include "weather.h"

// forward declaration
u8 func_80060DC0();

// bss
extern u8 gSeconds;
extern u8 gHour;
extern u8 gMinutes;
extern u8 gDayOfWeek;
extern u8 gDayOfMonth;
extern u8 gSeason;
extern u8 gNextSeason;
extern u8 gYear;

extern Vec4f D_80180718;
extern Vec4f globalLightingRgba;

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

extern u16 gCurrentSongIndex;
extern u8 gNamingScreenIndex;

extern u8 gFarmName[6];
extern u8 gWifeName[6];
extern u8 gBabyName[6];
extern u8 harvestKingName[6];

extern u16 gTotalGrassTiles;
extern u16 gTotalPinkCatMintFlowersGrowing;

extern u32 gTotalCropsShipped;
extern u32 gTotalEggsShipped;
extern u32 gTotalGoldenMilkShipped;

extern u8 D_801C3F36;
extern u8 D_801C3F96;
extern u8 D_801C3F97;
extern u8 D_801C3F98;
extern u8 D_801C3F99;

extern u8 D_801C3F37;
extern u8 D_801FC150;
extern u8 D_801FC15C;
extern u8 D_801FD621;
extern u8 D_80205201;
extern u8 D_8020562C;
extern u8 D_80237412;  

extern u8 karenKaiBabyAge;
extern u8 karenKaiNewlywedCounter;
extern u8 karenPregnancyCounter;
extern u8 annCliffBabyAge;
extern u8 annAndCliffNewlywedCounter;
extern u8 annPregnancyCounter;

extern u8 D_801C4216;

extern MainLoopCallbackInfo D_80205230;

// unused
extern u16 D_801FB686;   

// shared: likely bss
extern u8 D_8016FAD8;
extern u8 D_8017027E;
extern u8 D_801886A0;
extern u8 D_801891C4;
extern u8 D_801FB9A4;

//extern u16 D_801C3F32;
extern u8 D_801C4216;
extern u16 D_801886D2;
extern u8 D_80189054;
extern u32 D_801890E0;

// data
extern u8 D_80113B20[];
extern u8 D_80113B38[];
extern u8 D_80113B50[];
extern u8 D_80113B68[];
extern u8 D_80113B80[];
extern u8 D_80113B98[];
extern u8 D_80113BB0[];
extern u8 D_80113BC8[];
extern u8 D_80113BE0[];
extern u8 D_80113BF8[];
extern u8 D_80113C10[];
extern u8 D_80113C28[];

// rodata
extern u8 houseConstructionDays[6];
extern u16 lifeEventHouseConstructionBits[6];
extern u8 animalLocationsHouseConstruction[6];
extern MemoryRead_32 D_8011F25C[];

static const s16 houseExtensionPrices[6];     
static const s16 houseExtensionLumberCosts[6];

// forward declarations
extern inline void func_8005AE8C(u16 arg0, u16 arg1, u16 arg2, int arg3, u16 arg4);
extern void func_8005B09C(u8);
extern void func_8005B09C(u8);        

//INCLUDE_ASM(const s32, "game", func_80059D90);

void func_80059D90(void) {

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

        D_801FB9A4 += adjustValue(D_801FB9A4, 1, 120);

        if (checkLifeEventBit(MARIA_HARRIS_BABY) && D_80205201 < 120) {
            D_80205201++;
        }

        if (!checkLifeEventBit(MARIA_HARRIS_BABY) && checkLifeEventBit(MARIA_PREGNANT) && D_8016FAD8 >= 60) {
            toggleLifeEventBit(MARIA_PREGNANT);
            setLifeEventBit(MARIA_HARRIS_BABY);
            setSpecialDialogueBit(MARIA_HARRIS_BABY_DIALOGUE);
            toggleSpecialDialogueBit(MARIA_PREGNANT_DIALOGUE);
        }

        if (!checkLifeEventBit(MARIA_HARRIS_BABY) && checkLifeEventBit(MARIA_PREGNANT)) {
            D_8016FAD8++;
        }

        if (!checkLifeEventBit(MARIA_HARRIS_BABY) && !checkLifeEventBit(MARIA_PREGNANT) && D_801C3F96 >= 250 && D_801FB9A4 >= 30) {
            setLifeEventBit(MARIA_PREGNANT);
            D_8016FAD8 = 0;
            setSpecialDialogueBit(MARIA_PREGNANT_DIALOGUE);
        }
    }

     if (checkLifeEventBit(POPURI_GRAY_MARRIED)) {

        D_801886A0 += adjustValue(D_801886A0, 1, 120);

        if (checkLifeEventBit(POPURI_GRAY_BABY) && D_801C3F36 < 120) {
            D_801C3F36++;
        }

        if (!checkLifeEventBit(POPURI_GRAY_BABY) && checkLifeEventBit(POPURI_PREGNANT) && D_801891C4 >= 60) {
            toggleLifeEventBit(POPURI_PREGNANT);
            setLifeEventBit(POPURI_GRAY_BABY);
            setSpecialDialogueBit(POPURI_GRAY_BABY_DIALOGUE);
            toggleSpecialDialogueBit(0x38);
        }

        if (!checkLifeEventBit(POPURI_GRAY_BABY) && checkLifeEventBit(POPURI_PREGNANT)) {
            D_801891C4++;
        }

        if (!checkLifeEventBit(POPURI_GRAY_BABY) && !checkLifeEventBit(POPURI_PREGNANT) && D_801C3F97 >= 250 && D_801886A0 >= 30) {
            setLifeEventBit(POPURI_PREGNANT);
            D_801891C4 = 0;
            setSpecialDialogueBit(0x38);
        }
    }

    if (checkLifeEventBit(ELLI_JEFF_MARRIED)) {

        D_8017027E += adjustValue(D_8017027E, 1, 120);

        if (checkLifeEventBit(ELLI_JEFF_BABY) && D_801FC15C < 120) {
            D_801FC15C++;
        }

        if (!checkLifeEventBit(ELLI_JEFF_BABY) && checkLifeEventBit(ELLI_PREGNANT) && D_8020562C >= 60) {
            toggleLifeEventBit(ELLI_PREGNANT);
            setLifeEventBit(ELLI_JEFF_BABY);
            setSpecialDialogueBit(ELLI_JEFF_BABY_DIALOGUE);
            toggleSpecialDialogueBit(ELLI_PREGNANT_DIALOGUE);
        }

        if (!checkLifeEventBit(ELLI_JEFF_BABY) && checkLifeEventBit(ELLI_PREGNANT)) {
            D_8020562C++;
        }

        if (!checkLifeEventBit(ELLI_JEFF_BABY) && !checkLifeEventBit(ELLI_PREGNANT) && D_801C3F98 >= 250 && D_8017027E >= 30) {
            setLifeEventBit(ELLI_PREGNANT);
            D_8020562C = 0;
            setSpecialDialogueBit(ELLI_PREGNANT_DIALOGUE);
        }
    }

    if (checkLifeEventBit(ANN_CLIFF_MARRIED)) {

        annAndCliffNewlywedCounter += adjustValue(annAndCliffNewlywedCounter, 1, 120);

        if (checkLifeEventBit(ANN_CLIFF_BABY) && annCliffBabyAge < 120) {
            annCliffBabyAge++;
        }

        if (!checkLifeEventBit(ANN_CLIFF_BABY) && checkLifeEventBit(ANN_PREGNANT) && annPregnancyCounter >= 60) {
            toggleLifeEventBit(ANN_PREGNANT);
            setLifeEventBit(ANN_CLIFF_BABY);
            setSpecialDialogueBit(ANN_CLIFF_BABY_DIALOGUE);
            toggleSpecialDialogueBit(ANN_PREGNANT_DIALOGUE);
        }

        if (!checkLifeEventBit(ANN_CLIFF_BABY) && checkLifeEventBit(ANN_PREGNANT)) {
            annPregnancyCounter++;
        }

        if (!checkLifeEventBit(ANN_CLIFF_BABY) && !checkLifeEventBit(ANN_PREGNANT) && npcAffection[CLIFF] >= 250 && annAndCliffNewlywedCounter >= 30) {
            setLifeEventBit(ANN_PREGNANT);
            annPregnancyCounter = 0;
            setSpecialDialogueBit(ANN_PREGNANT_DIALOGUE);
        }
    }

    if (checkLifeEventBit(KAREN_KAI_MARRIED)) {
        
        karenKaiNewlywedCounter += adjustValue(karenKaiNewlywedCounter, 1, 120);

        if (checkLifeEventBit(KAREN_KAI_BABY) && karenKaiBabyAge < 120) {
            karenKaiBabyAge++;
        }

        if (!checkLifeEventBit(KAREN_KAI_BABY) && checkLifeEventBit(KAREN_PREGNANT) && karenPregnancyCounter >= 60) {
            toggleLifeEventBit(KAREN_PREGNANT);
            setLifeEventBit(KAREN_KAI_BABY);
            setSpecialDialogueBit(KAREN_KAI_BABY_DIALOGUE);
            toggleSpecialDialogueBit(KAREN_PREGNANT_DIALOGUE);
        }

        if (!checkLifeEventBit(KAREN_KAI_BABY) && checkLifeEventBit(KAREN_PREGNANT)) {
            karenPregnancyCounter++;
        }

        if (!checkLifeEventBit(KAREN_KAI_BABY) && !checkLifeEventBit(KAREN_PREGNANT) && npcAffection[KAI] >= 250 && karenKaiNewlywedCounter >= 30) {
            setLifeEventBit(0x25);
            karenPregnancyCounter = 0;
            setSpecialDialogueBit(KAREN_PREGNANT_DIALOGUE);
        }
    }
}

//INCLUDE_ASM(const s32, "game", func_8005A60C);

void func_8005A60C(void) {

    if (!checkLifeEventBit(MARRIED)) goto handleAnimals;

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

handleAnimals:
    adjustDogAffection(-1);
    adjustHorseAffection(-1);
}

// func_8005A708
//INCLUDE_ASM(const s32, "game", setSpecialDialogues);

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
        setSpecialDialogueBit(RAIN_FORECAST);
    } else {
        toggleSpecialDialogueBit(RAIN_FORECAST);
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
            setSpecialDialogueBit(PLAYER_HARVEST_KING);
        }

        toggleSpecialDialogueBit(0x138);
        toggleSpecialDialogueBit(0x139);
        toggleSpecialDialogueBit(0x13A);
        toggleSpecialDialogueBit(0x13B);
        toggleSpecialDialogueBit(0x13C);
        toggleSpecialDialogueBit(0x13D);
        toggleSpecialDialogueBit(0x13E);
        toggleSpecialDialogueBit(0x13F);
        toggleSpecialDialogueBit(0x140);
        toggleSpecialDialogueBit(0x141);

        // spirit festival assistants
        D_801C3F37 = 0;
        D_801FD621 = 0;
        D_801FC150 = 0;
        D_80237412 = 0;

    } else {
        toggleSpecialDialogueBit(PLAYER_HARVEST_KING);
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

//INCLUDE_ASM(const s32, "game", func_8005AAE4);

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

// func_8005ABBC
//INCLUDE_ASM(const s32, "game", resetDailyBits);

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

//INCLUDE_ASM(const s32, "game", func_8005AE8C);

// show text box
inline void func_8005AE8C(u16 arg0, u16 arg1, u16 arg2, int arg3, u16 arg4) {
  
    func_8002F6F0();
    func_80046C98();
    func_8002FCB4(0, 0);
    
    switch (arg0) {
        
        case 0:
          func_8003F54C(0, 24.0f, -64.0f, 352.0f);
          func_8003F690(0, 0, 0, 0);
          break;
        
        case 1:
          func_8003F54C(0, 0, -64.0f, 352.0f);
          func_8003F690(0, 1, 0, 0);
          break;
        
        default:
          break;
    }

    func_8003F360(0, ~(1 | 2), arg4);
  
    func_8003DDF8(0, arg1, arg2, arg3);
  
    setMainLoopCallbackFunctionIndex(TEXT);
    
    controllers[0].unk_1C = 0;

    setPlayerAction(0, 0);
}

//INCLUDE_ASM(const s32, "game", func_8005AF94);

// show dialogue box
inline void func_8005AF94(u16 arg0, u16 arg1, u16 arg2, u32 arg3, u16 arg4) {
    
    func_8002F6F0();
    func_80046C98();
    func_8002FCB4(0, 0);

    switch (arg0) {
        case 0:
            func_8003F54C(0, 24.0f, -64.0f, 352.0f);
            func_8003F690(0, 0, 0, 0);
            break;
        case 1:
            func_8003F54C(0, 0, -64.0f, 352.0f);
            func_8003F690(0, 1, 0, 0);
            break;
        default:
            break;
    }

    func_8003F360(0, ~(1 | 2), arg4);
    func_80043430(0, arg1, arg2, arg3);

    setMainLoopCallbackFunctionIndex(DIALOGUE);

    controllers[0].unk_1C = 0;
    
    setPlayerAction(0, 0);

}

//INCLUDE_ASM(const s32, "game", func_8005B09C);

// jumptable: jbtl_8011EF98, 0xFA398
void func_8005B09C(u8 arg0) {

    D_80205230.unk_6 = arg0;

    switch (arg0) {
        default:
            break;
        case 0:
            func_8005AF94(1, 1, 0, 0, 2);
            break;
        case 1:
            func_8005AF94(1, 1, 1, 0, 2);
            break;    
        case 2:
            func_8005AF94(0, 0x3D, 0xC, 0x40, 0);
            break;         
        case 3:
            func_8005AF94(0, 0x3D, 0xB, 0x40, 0);
            break;          
        case 4:
            func_8005AF94(0, 0x3D, 0x12, 0x40, 0);
            break;       
        case 5:
            func_8005AF94(0, 0x3D, 0xD, 0x40, 0);
            break;        
        case 6:
            func_8005AF94(0, 0x3D, 0xE, 0x40, 0);
            break;      
        case 7:
            func_8005AF94(0, 0x3D, 0xF, 0x40, 0);
            break;      
        case 8:
            func_8005AF94(0, 0x3E, 7, 0x40, 0);
            break;      
        case 9:
            func_8005AF94(0, 0x3E, 6, 0x40, 0);
            break;    
        case 10:
            func_8005AF94(0, 0x3E, 2, 0x40, 0);
            break;  
        case 11:
            func_8005AF94(0, 0x3E, 0, 0x40, 0);
            break;        
        case 12:
            func_8005AF94(0, 0x3E, 1, 0x40, 0);
            break;          
        case 13:
            if (gSeason == SPRING || gSeason == WINTER) {
                func_8005AF94(1, 0x44, 0, 0, 2);
            } else {
                func_8005AF94(1, 0x44, 10, 0, 2);
            }            
            break;
        case 14:
            if (gSeason == SPRING || gSeason == WINTER) {
                func_8005AF94(1, 0x44, 1, 0x40, 2);
            } else {
                func_8005AF94(1, 0x44, 0xB, 0, 2);
            }
            break;
        case 15:
            if (gSeason == SPRING || gSeason == WINTER) {
                func_8005AF94(1, 0x44, 2, 0x40, 2);
            } else {
                func_8005AF94(1, 0x44, 0xC, 0, 2);
            }
            break; 
        case 16:
            if (gSeason == SPRING || gSeason == WINTER) {
                func_8005AF94(1, 0x44, 3, 0x40, 2);
            } else {
                func_8005AF94(1, 0x44, 0xD, 0, 2);
            }
            break;   
        case 17:
            if (gSeason == SPRING || gSeason == WINTER) {
                func_8005AF94(1, 0x44, 4, 0x40, 2);
            } else {
                func_8005AF94(1, 0x44, 0xE, 0, 2);
            }
            break;     
        case 18:
            if (gSeason == SPRING || gSeason == WINTER) {
                func_8005AF94(1, 0x44, 5, 0x40, 2);
            } else {
                func_8005AF94(1, 0x44, 0xF, 0, 2);
            }
            break;        
        case 19:
            if (gSeason == SPRING || gSeason == WINTER) {
                func_8005AF94(1, 0x44, 6, 0x40, 2);
            } else {
                func_8005AF94(1, 0x44, 0x10, 0, 2);
            }
            break;        
        case 20:
            if (gSeason == SPRING || gSeason == WINTER) {
                func_8005AF94(1, 0x44, 7, 0x40, 2);
            } else {
                func_8005AF94(1, 0x44, 0x11, 0, 2);
            }
            break;       
        case 21:
            if (gSeason == SPRING || gSeason == WINTER) {
                func_8005AF94(1, 0x44, 8, 0x40, 2);
            } else {
                func_8005AF94(1, 0x44, 0x12, 0, 2);
            }
            break;       
        case 22:
            if (gSeason == SPRING || gSeason == WINTER) {
                func_8005AF94(1, 0x44, 9, 0x40, 2);
            } else {
                func_8005AF94(1, 0x44, 0x13, 0, 2);
            }
            break;     
        case 23:
            func_8005AF94(1, 1, 0x15, 0, 2);
            break;    
    }

    setMainLoopCallbackFunctionIndex(PINK_OVERLAY_TEXT);
}

//INCLUDE_ASM(const s32, "game", func_8005C00C);

void func_8005C00C(void) {
    
    if (D_801891D4 < 0) {
        setMainLoopCallbackFunctionIndex(MAIN_GAME);
        return;
    }
    
    if (!checkDailyEventBit(0x4B)) {
        // set audio settings for level
        setLevelAudio(gBaseMapIndex, gSeason, gHour);
    }
     
    // set lighting for level based on weather
    func_8005C07C(8, 1);
}

// jtbl_8011EFF8
//INCLUDE_ASM(const s32, "game", func_8005C07C);

void func_8005C07C(s16 arg0, u16 arg1) {

    Vec4f vec;
    
    // 4 = bit 3 = outdoors flag
    if (getLevelFlags(gBaseMapIndex) & 4) {
        
        vec = setWeatherLighting(gWeather);

        switch (gWeather) {
            case SUNNY:
            case 4:
            case TYPHOON:
                globalLightingRgba.r = D_80113B20[gHour];
                globalLightingRgba.g = D_80113B38[gHour];
                globalLightingRgba.b = D_80113B50[gHour];
                globalLightingRgba.a = D_80113B68[gHour];
                break;
            case RAIN:
                globalLightingRgba.r = D_80113B80[gHour];
                globalLightingRgba.g = D_80113B98[gHour];
                globalLightingRgba.b = D_80113BB0[gHour];
                globalLightingRgba.a = D_80113BC8[gHour];  
                break;
            case SNOW:
                globalLightingRgba.r = D_80113BE0[gHour];
                globalLightingRgba.g = D_80113BF8[gHour];
                globalLightingRgba.b = D_80113C10[gHour];
                globalLightingRgba.a = D_80113C28[gHour];  
                break;
            default:
                break;
        }
        
    } else {
        func_8006EB94(&globalLightingRgba, gBaseMapIndex);
    }
    
    if (arg0 == 0) {
        
        func_8002F7C8(globalLightingRgba.r, globalLightingRgba.g, globalLightingRgba.b, globalLightingRgba.a);
        func_8003BE98(0, globalLightingRgba.r, globalLightingRgba.g, globalLightingRgba.b, globalLightingRgba.a);
        
    } else if (globalLightingRgba.r != D_80180718.r || globalLightingRgba.g != D_80180718.g || globalLightingRgba.b != D_80180718.b || globalLightingRgba.a != D_80180718.a) {
        
        func_8002F8F0(globalLightingRgba.r, globalLightingRgba.g, globalLightingRgba.b, globalLightingRgba.a, arg0);
        func_8003BF7C(0, globalLightingRgba.r, globalLightingRgba.g, globalLightingRgba.b, globalLightingRgba.a, arg0);
        
        if (!checkDailyEventBit(0x4B)) {
            setSongWithVolume(gCurrentSongIndex, gSongVolume);
        }
    }

    D_80180718.r = globalLightingRgba.r;
    D_80180718.g = globalLightingRgba.g;
    D_80180718.b = globalLightingRgba.b;
    D_80180718.a = globalLightingRgba.a;

    D_80205230.unk_0 = arg1;
    
    if (arg1) {
        setMainLoopCallbackFunctionIndex(5);
        func_8002F730();
        func_80046CF4();
    }
}

//INCLUDE_ASM(const s32, "game", func_8005C940);

inline void func_8005C940(u16 arg0, u16 arg1) {
    
    func_8003BF7C(0, 0, 0, 0, 0, 8);
    func_8002F8F0(0, 0, 0, 0, 8);
       
    setSongWithDefaultSpeed(gCurrentSongIndex);
    
    D_80205230.unk_0 = arg1;

    resetGlobalLighting();
    
    if (D_80205230.unk_0) {
        setMainLoopCallbackFunctionIndex(LEVEL_LOAD_2);
        func_8002F6F0();
        func_80046C98();
        func_8002FCB4(0, 0);
    }
}

//INCLUDE_ASM(const s32, "game", func_8005CA2C);

void func_8005CA2C(u16 arg0, u16 arg1) {
    D_80205230.unk_4 = arg0;
    D_80205230.unk_2 = 0;
    D_80205230.unk_0 = arg1;
    setMainLoopCallbackFunctionIndex(OVERLAY_SCREEN_LOAD);
}

//INCLUDE_ASM(const s32, "game", func_8005CA64);

void func_8005CA64(void) {

    func_800A8F74();

    if (func_80036A84(0)) {
        setMainLoopCallbackFunctionIndex(D_80205230.unk_0);
    }
}

//INCLUDE_ASM(const s32, "game", func_8005CAA8);

void func_8005CAA8(void) {

    func_800A8F74();

    if (func_80036A84(0) && checkDefaultSongChannelOpen(gCurrentSongIndex)) {

        func_8002F7C8(0, 0, 0, 0);
        func_8003BE98(0, 0, 0, 0, 0);
        // reset cutscene structs
        func_80046BB8();
        initializeCutsceneMaps();

        D_801891D4 = 0;

        gCutsceneFlags = 0;

        setMainLoopCallbackFunctionIndex(D_80205230.unk_0);
    }
}

//INCLUDE_ASM(const s32, "game", func_8005CB50);

void func_8005CB50(void) {

    if (D_80205230.unk_2 >= D_80205230.unk_4) {
        setMainLoopCallbackFunctionIndex(D_80205230.unk_0);
        return;
    }

    D_80205230.unk_2++;
}

//INCLUDE_ASM(const s32, "game", func_8005CBA4);

void func_8005CBA4(void) {

    if (!(gMapModelContext[0].flags & (0x8 | 0x10))) {
        func_8002F730();
        func_80046CF4();
        func_8002FCB4(0, 1);
        setMainLoopCallbackFunctionIndex(1);
    }

}

//INCLUDE_ASM(const s32, "game", func_8005CBF0);

void func_8005CBF0(void) {
    
    if (func_80043A88()) {
        
        if (!(func_800D5A6C(gPlayer.unk_2C) & 0xD80)) {
            func_800D55E4(gPlayer.unk_2D, 1);
            gPlayer.unk_2C = 0;
            gItemBeingHeld = 0xFF;
        }
         
        func_8003F910(0, 0x78, &dialogueIconsTexture_ROM_START, &dialogueIconsTexture_ROM_END, &dialogueIconsPalette_ROM_START, &dialogueIconsPalette_ROM_END, (void*)DIALOGUE_ICONS_TEXTURES_VADDR, (void*)0x8023CC00, (void*)0x8023CE00, (void*)0x8023D200, 0, 4, 0xFE, 106.0f, -15.0f, 0);
        func_8003F910(1, 0x78, &dialogueIconsTexture_ROM_START, &dialogueIconsTexture_ROM_END, &dialogueIconsPalette_ROM_START, &dialogueIconsPalette_ROM_END, (void*)DIALOGUE_ICONS_TEXTURES_VADDR, (void*)0x8023CC00, (void*)0x8023CE00, (void*)0x8023D200, 0, 0xD, 0xFE, 106.0f, -15.0f, 0);
       
        func_8005CDCC();
        
        setMainLoopCallbackFunctionIndex(MAIN_GAME);
        
        func_8002F730();
        func_80046CF4();
        func_8002FCB4(0, 1);
    }
}

//INCLUDE_ASM(const s32, "game", func_8005CDCC);

void func_8005CDCC(void) {
    
    npcAffection[HARVEST_SPRITE_2] = npcAffection[HARVEST_SPRITE_1];
    npcAffection[HARVEST_SPRITE_3] = npcAffection[HARVEST_SPRITE_1];
    
    toggleSpecialDialogueBit(0x135);

    func_80061A88();

    setRecipes();
    
    if (checkSpecialDialogueBit(0x22) && !checkHaveKeyItem(6)) {
        acquireKeyItem(6);
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
        setLifeEventBit(7);
    }
    if (checkSpecialDialogueBit(0x84)) {
        setLifeEventBit(6);
    }
}

//INCLUDE_ASM(const s32, "game", func_8005CEFC);

void func_8005CEFC(void) {
    
    if (func_8003F0DC()) {

        setMainLoopCallbackFunctionIndex(MAIN_GAME);

        // reset bits on flags in structs
        func_8002F730();
        func_80046CF4();
        func_8002FCB4(0, 1);
    }
}

//INCLUDE_ASM(const s32, "game", func_8005CF4C);

void func_8005CF4C(void) {

    if (D_801FB686 == 0) {
        setMainLoopCallbackFunctionIndex(MAIN_GAME);
        func_8002F730();
        func_80046CF4();
        func_8002FCB4(0, 1);
    }
}

// jtbl_8011F010, 0xFA410
//INCLUDE_ASM(const s32, "game", func_8005CF94);

void func_8005CF94(void) {
    
    u8* namePtr;

    func_8002FB3C();
    func_80046C98();
    func_8002FCB4(0, 0);
    func_8003C504(0);
    
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
    
    func_800ED8A0(namePtr, gNamingScreenIndex);
}

//INCLUDE_ASM(const s32, "game", func_8005D0BC);

void func_8005D0BC(void) {
    
    u8 set;
    int checkOverflow;
    s16 maxHappiness;
    u8 happinessIncrease;
    
    toggleDailyEventBit(6);
    toggleDailyEventBit(0x12);
    toggleDailyEventBit(0x13);

    setPlayerAction(0, 0);

    func_80056030(1);
    
    set = 0;
    
    if (gBaseMapIndex == COOP && checkLifeEventBit(3)) {
        
        gNamingScreenIndex = 7;
        D_801FC155 = D_8016FFE8;
        gChickens[D_8016FFE8].flags &= ~0x100;
        
        setMainLoopCallbackFunctionIndex(NAMING_SCREEN);
        toggleLifeEventBit(3);
        setLifeEventBit(0x38);
        
        set = 1;
        
        // should be inline adjustValue call

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
    
    if (gBaseMapIndex == BARN && checkLifeEventBit(4)) {
        
        gNamingScreenIndex = 5;
        D_801FC155 = D_80170464;
        gFarmAnimals[D_80170464].flags &= ~0x800;
            
        setMainLoopCallbackFunctionIndex(NAMING_SCREEN);
        toggleLifeEventBit(4);
            
        set = 1;
        

        // should be inline adjustValue call

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
    
    if (gHour >= 18U) {
        setDailyEventBit(0xF);
        setDailyEventBit(0x10);
    }
    
    if (!set) {
        setMainLoopCallbackFunctionIndex(4);
    }
}

static inline void inline1(u8 arg0, u8 arg1, u8 arg2, u8 arg3, u8 arg4) {
    switch (arg0) {                   
        case 0:                                 
            func_800DC9FC(arg1);
            setMainLoopCallbackFunctionIndex(1);
            break;
        case 1:                                 
            func_800DC9FC(arg2);
            setMainLoopCallbackFunctionIndex(1);
            break;
        case 2:                                 
            func_800DC9FC(arg3);
            setMainLoopCallbackFunctionIndex(1);
            break;
        case 3:                                 
            func_800DC9FC(arg4);
            setMainLoopCallbackFunctionIndex(1);
            break;
    }
}

static inline void inline2(u8 arg0, u8 arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5) {
    switch (arg0) {                    
        case 0:                                 
            func_800DC9FC(arg1);
            setMainLoopCallbackFunctionIndex(1);
            break;
        case 1:                                 
            func_800DC9FC(arg2);
            setMainLoopCallbackFunctionIndex(1);
            break;
        case 2:                                 
            func_800DC9FC(arg3);
            setMainLoopCallbackFunctionIndex(1);
            break;
        case 3:                                 
            func_800DC9FC(arg4);
            setMainLoopCallbackFunctionIndex(1);
            break;
        case 4:                                 
            func_800DC9FC(arg5);
            setMainLoopCallbackFunctionIndex(1);
            break;
    }
}

static inline void inline3() {                 
    D_801C3E40[0].struct5.flags &= ~0x40;
    func_80043AD8(0);
    setPlayerAction(0, 0);
    setMainLoopCallbackFunctionIndex(1);
}

// from initialize2.c
static inline void func_80055F08_2(u16 cutsceneIndex, u16 entranceIndex, u8 arg2) {
    
    func_8002E1B8();
    func_8002B710();
    func_8003D970();
    initializeCutsceneMaps();
    func_80053088();
    
    resetGlobalLighting();

    func_8002F7C8(0, 0, 0, 0);
    func_8003BE98(0, 0, 0, 0, 0);

    gHour = 12;

    // set ptrs to rom addresses for sprites
    func_800563D0(arg2);

    setEntrance(entranceIndex);

    gCutsceneIndex = cutsceneIndex;

    // trigger cutscene and load cutscene assets
    loadCutscene(0);
    
    func_8006E840(gEntranceIndex);
    setupPlayerSprite(gEntranceIndex, 0);

    func_8006A2E8();

    setMainLoopCallbackFunctionIndex(4);

}

// possible split

// jtbl_8011F030
// jtbl_8011F090
// jtbl_8011F0A8
// jtbl_8011F0C0
// jtbl_8011F0D8
// jtbl_8011F0F0
// jtbl_8011F108
// jtbl_8011F120
// jtbl_8011F138
// jtbl_8011F150
// jtbl_8011F168
// jtbl_8011F180
// jtbl_8011F198
// jtbl_8011F1B0
// jtbl_8011F1C8
// 0xFA430-0xFA600
//INCLUDE_ASM(const s32, "game", func_8005D2B0);

// pink overlay main loop callback
void func_8005D2B0() {

    u8 temp;

    if (D_801C3E40[0].struct5.flags & 4) {
        
        func_8002F730();
        func_80046CF4();
        func_8002FCB4(0, 1);

        temp = func_80043C6C(0);

        switch (D_80205230.unk_6) {

            case 0:                                     
                switch (temp) {                      
                    case 0:                                 
                        func_8005B09C(1);
                        break;
                    case 1:                                 
                        func_8005AF94(1, 1, 2, 0x80, 2);
                        break;
                    case 2:                                 
                        setMainLoopCallbackFunctionIndex(0x19);
                        break;
                    case 3:                                 
                        setMainLoopCallbackFunctionIndex(0x1D);
                        break;
                    case 4:                                 
                        setMainLoopCallbackFunctionIndex(1);
                        break;
                    }
                
                break;
            
            // diary
            case 1:
                switch (temp) {
                        // write in diary/save game
                    case 0:                                 
                        setMainLoopCallbackFunctionIndex(1);
                        setPlayerAction(8, 10);
                        setDailyEventBit(7);
                        break;
                        // don't write
                    case 1:                                 
                        setMainLoopCallbackFunctionIndex(1);
                        setPlayerAction(8, 10);
                        break;
                    case 2:
                        setMainLoopCallbackFunctionIndex(1);
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
                        func_8005B09C(6);
                        break;
                    case 1:                                
                        func_8005AE8C(0, 6, 0x55, 0, 0);
                        setDailyEventBit(2);
                        setDailyEventBit(5);
                        break;
                    case 2:                                
                        func_800DC9FC(0x21);
                        setMainLoopCallbackFunctionIndex(1);
                        break;
                    }
                
                break;

            case 6:                                
                inline1(temp, 30, 31, 32, 33);
                break;

            case 7:                                     
                switch (temp) {                    
                    case 0:                         

                        gGold += adjustValue(gGold, D_801890E0, 999999);
                        
                        D_801890E0 = 0;                        
                        D_801C3E40[0].struct5.flags &= ~0x40;
                        
                        func_80043AD8(0);
                        setEntrance(9);

                        func_8005C940(0, 2);

                        toggleDailyEventBit(2);
                        setDailyEventBit(4);

                        switch (D_801C4216) {
                            case 4:
                                func_80088104(D_801FC155);
                                break;
                            case 2 ... 3:
                                func_80088810(D_801FC155);
                                break;
                        }
                        
                        break;
                    
                    case 1:                                 
                        D_801C3E40[0].struct5.flags &= ~0x40;
                        func_80043AD8(0);
                        setMainLoopCallbackFunctionIndex(1);
                        break;
                    }
                
                break;

            case 8:                         
                switch (temp) {                    
                    case 0:                                 
                        
                        D_801C3E40[0].struct5.flags &= ~0x40;
                        func_80043AD8(0);
                        setEntrance(9);

                        func_8005C940(0, 2);

                        toggleDailyEventBit(0x1D);
                        setDailyEventBit(0x1E);
                        setDailyEventBit(0x20);
                        D_80189054 = D_801FC155;
                        break;
                        
                    case 1:                                 
                        D_801C3E40[0].struct5.flags &= ~0x40;
                        func_80043AD8(0);
                        setMainLoopCallbackFunctionIndex(1);
                        break;
                    }
                
                break;

            case 9:                                     
                if (temp >= 5) {
                    gVoteForFlowerFestivalGoddess = 0xFF;
                } else {
                    gVoteForFlowerFestivalGoddess = temp;
                }
                
                D_801C3E40[0].struct5.flags &= ~0x40;
                func_80043AD8(0);
                setMainLoopCallbackFunctionIndex(1);
                break;
            
            case 10:                                    
                switch (temp) {                   
                    case 0:                                 
                        func_800DC9FC(50);
                        setMainLoopCallbackFunctionIndex(1);
                        break;
                    case 1:                                 
                        func_800DC9FC(51);
                        setMainLoopCallbackFunctionIndex(1);
                        break;    
                    case 2:                                 
                        func_800DC9FC(52);
                        setMainLoopCallbackFunctionIndex(1);
                        break;    
                    case 3:                                 
                        func_8005AE8C(0, 4, 31, 0, 0);
                        break; 
                    }
                
                    break;

            case 11:                     
                switch (temp) {                     
                    case 0:                                 
                        func_800DC9FC(45);
                        setMainLoopCallbackFunctionIndex(1);
                        break;    
                    case 1:                                 
                        func_800DC9FC(46);
                        setMainLoopCallbackFunctionIndex(1);
                        break;    
                    case 2:                                 
                        func_800DC9FC(49);
                        setMainLoopCallbackFunctionIndex(1);
                        break;    
                    case 3:
                        func_8005AE8C(0, 4, 20, 0, 0);
                        break;
                    }
                
                break;

            case 12:                                    
                switch (temp) {                     
                    case 0:                                 
                        func_800DC9FC(47);
                        setMainLoopCallbackFunctionIndex(1);
                        break;    
                    case 1:                                 
                        func_800DC9FC(48);
                        setMainLoopCallbackFunctionIndex(1);
                        break;    
                    case 2:                                 
                        func_800DC9FC(49);
                        setMainLoopCallbackFunctionIndex(1);
                        break;    
                    case 3:
                        func_8005AE8C(0, 4, 20, 0, 0);
                        break;
                    }
                
                break;

            case 13:                                    
                if (gSeason == SPRING || gSeason == WINTER) {
                    if (gPlayer.action1 == 0x1E || temp == 3) {
                        switch (temp) {             
                            case 0:                         
                                func_8005AE8C(1, 1, 0, 0, 0);
                                break;
                            case 1:                         
                                func_8005AE8C(1, 1, 1, 0, 0);
                                break;
                            case 2:                         
                                func_8005AE8C(1, 1, 2, 0, 0);
                                break;
                            case 3:
                                D_801C3E40[0].struct5.flags &= ~0x40;
                                func_80043AD8(0);
                                setPlayerAction(0, 0);
                                setMainLoopCallbackFunctionIndex(1);
                                break;
                            }
                        
                        func_800D55E4(gPlayer.unk_2D, 1);
                        gPlayer.unk_2C = 0;
                        break;
                    }
                    
                    setPlayerAction(0x1E, 0x1F);
                    setMainLoopCallbackFunctionIndex(1);
                    break;
                }
                
                if (gPlayer.action1 == 0x1E || temp == 3) {
                    switch (temp) {                 
                        case 0:                             
                            func_8005AE8C(1, 1, 3, 0, 0);
                            break;
                        case 1:                             
                            func_8005AE8C(1, 1, 4, 0, 0);
                            break;     
                        case 2:                             
                            func_8005AE8C(1, 1, 5, 0, 0);
                            break;
                        case 3:
                            D_801C3E40[0].struct5.flags &= ~0x40;
                            func_80043AD8(0);
                            setPlayerAction(0, 0);
                            setMainLoopCallbackFunctionIndex(1);
                            break;
                        }
                    
                    func_800D55E4(gPlayer.unk_2D, 1);
                    gPlayer.unk_2C = 0;
                    break;
                    
                }
                
                setPlayerAction(0x1E, 0x1F);
                setMainLoopCallbackFunctionIndex(1);
                break;

            case 14:                                    
                if (gSeason == SPRING || gSeason == WINTER) {
                    if (gPlayer.action1 == 0x1E || temp == 3) {
                        switch (temp) {            
                            case 0:                        
                                func_8005AE8C(1, 1, 9, 0, 0);
                                break;
                            case 1:                        
                                func_8005AE8C(1, 1, 10, 0, 0);
                                break;
                            case 2:                        
                                func_8005AE8C(1, 1, 11, 0, 0);
                                break;
                            case 3:
                                D_801C3E40[0].struct5.flags &= ~0x40;
                                func_80043AD8(0);
                                setPlayerAction(0, 0);
                                setMainLoopCallbackFunctionIndex(1);
                                break;
                            }
                        
                        func_800D55E4(gPlayer.unk_2D, 1);
                        gPlayer.unk_2C = 0;
                        break;
                        
                    }
                    
                    setPlayerAction(0x1E, 0x1F);
                    setMainLoopCallbackFunctionIndex(1);
                    break;
                }
                
                if (gPlayer.action1 == 0x1E || temp == 3) {
                    switch (temp) {                
                        case 0:                             
                            func_8005AE8C(1, 1, 6, 0, 0);
                            break;
                        case 1:                             
                            func_8005AE8C(1, 1, 7, 0, 0);
                            break;
                        case 2:                             
                            func_8005AE8C(1, 1, 8, 0, 0);
                            break;
                        case 3:
                            D_801C3E40[0].struct5.flags &= ~0x40;
                            func_80043AD8(0);
                            setPlayerAction(0, 0);
                            setMainLoopCallbackFunctionIndex(1);
                            break;
                    }
                
                    func_800D55E4(gPlayer.unk_2D, 1);
                    gPlayer.unk_2C = 0;
                    break;
                    
                }
                
                setPlayerAction(0x1E, 0x1F);
                setMainLoopCallbackFunctionIndex(1);
                break;


        case 15:                                    
            if (gSeason == SPRING || gSeason == WINTER) {
                if (gPlayer.action1 == 0x1E || temp == 3) {
                    switch (temp) {             
                        case 0:                         
                            func_8005AE8C(1, 1, 12, 0, 0);
                            break;
                        case 1:                         
                            func_8005AE8C(1, 1, 13, 0, 0);
                            break;
                        case 2:                         
                            func_8005AE8C(1, 1, 14, 0, 0);
                            break;
                        case 3:
                            D_801C3E40[0].struct5.flags &= ~0x40;
                            func_80043AD8(0);
                            setPlayerAction(0, 0);
                            setMainLoopCallbackFunctionIndex(1);
                            break;
                        }
                    
                    func_800D55E4(gPlayer.unk_2D, 1);
                    gPlayer.unk_2C = 0;
                    break;
                }
                
                setPlayerAction(0x1E, 0x1F);
                setMainLoopCallbackFunctionIndex(1);
                break;
            }
            
            if (gPlayer.action1 == 0x1E || temp == 3) {
                switch (temp) {                         
                    case 0:                             
                        func_8005AE8C(1, 1, 15, 0, 0);
                        break;
                    case 1:                             
                        func_8005AE8C(1, 1, 16, 0, 0);
                        break;
                    case 2:                             
                        func_8005AE8C(1, 1, 17, 0, 0);
                        break;
                    case 3:
                        D_801C3E40[0].struct5.flags &= ~0x40;
                        func_80043AD8(0);
                        setPlayerAction(0, 0);
                        setMainLoopCallbackFunctionIndex(1);
                        break;
                    }
                
                func_800D55E4(gPlayer.unk_2D, 1);
                gPlayer.unk_2C = 0;
                break;
                
            }
            
                setPlayerAction(0x1E, 0x1F);
                setMainLoopCallbackFunctionIndex(1);
                break;

        case 16:                                    
            if (gSeason == SPRING || gSeason == WINTER) {
                if (gPlayer.action1 == 0x1E || temp == 3) {
                    switch (temp) {             
                        case 0:                         
                            func_8005AE8C(1, 1, 18, 0, 0);
                            break;
                        case 1:                         
                            func_8005AE8C(1, 1, 19, 0, 0);
                            break;
                        case 2:                         
                            func_8005AE8C(1, 1, 20, 0, 0);
                            break;                        
                        case 3:
                            D_801C3E40[0].struct5.flags &= ~0x40;
                            func_80043AD8(0);
                            setPlayerAction(0, 0);
                            setMainLoopCallbackFunctionIndex(1);
                            break;
                        }
                    
                        func_800D55E4(gPlayer.unk_2D, 1);
                        gPlayer.unk_2C = 0;
                        break;
                    }
                
                    setPlayerAction(0x1E, 0x1F);
                    setMainLoopCallbackFunctionIndex(1);
                    break;
                }
            
            if (gPlayer.action1 == 0x1E || temp == 3) {
                switch (temp) {                 
                    case 0:                             
                        func_8005AE8C(1, 1, 21, 0, 0);
                        break;
                    case 1:                             
                        func_8005AE8C(1, 1, 22, 0, 0);
                        break;
                    case 2:                             
                        func_8005AE8C(1, 1, 23, 0, 0);
                        break;
                    case 3:
                        D_801C3E40[0].struct5.flags &= ~0x40;
                        func_80043AD8(0);
                        setPlayerAction(0, 0);
                        setMainLoopCallbackFunctionIndex(1);
                        break;
                    }
                
                    func_800D55E4(gPlayer.unk_2D, 1);
                    gPlayer.unk_2C = 0;
                    break;
                }
            
            setPlayerAction(0x1E, 0x1F);
            setMainLoopCallbackFunctionIndex(1);
            break;

        case 17:                                    
            if (gSeason == SPRING || gSeason == WINTER) {
                if (gPlayer.action1 == 0x1E || temp == 4) {
                    switch (temp) {            
                        case 0:                         
                            func_8005AE8C(1, 1, 25, 0, 0);
                            break;
                        case 1:                         
                            func_8005AE8C(1, 1, 26, 0, 0);
                            break;
                        case 2:                         
                            func_8005AE8C(1, 1, 27, 0, 0);
                            break;
                        case 3:                         
                            func_8005AE8C(1, 1, 24, 0, 0);
                            break;
                        case 4:
                            D_801C3E40[0].struct5.flags &= ~0x40;
                            func_80043AD8(0);
                            setPlayerAction(0, 0);
                            setMainLoopCallbackFunctionIndex(1);
                            break;
                        }
                    
                    func_800D55E4(gPlayer.unk_2D, 1);
                    gPlayer.unk_2C = 0;
                    break;
                }
                
                setPlayerAction(0x1E, 0x1F);
                setMainLoopCallbackFunctionIndex(1);
                break;
            }
            
            if (gPlayer.action1 == 0x1E || temp == 4) {
                switch (temp) {                
                    case 0:                             
                        func_8005AE8C(1, 1, 28, 0, 0);
                        break;
                    case 1:                             
                        func_8005AE8C(1, 1, 29, 0, 0);
                        break;
                    case 2:                             
                        func_8005AE8C(1, 1, 30, 0, 0);
                        break;
                    case 3:                             
                        func_8005AE8C(1, 1, 31, 0, 0);
                        break;                       
                    case 4:        
                        D_801C3E40[0].struct5.flags &= ~0x40;
                        func_80043AD8(0);
                        setPlayerAction(0, 0);
                        setMainLoopCallbackFunctionIndex(1);
                        break;
                    }
            
                    func_800D55E4(gPlayer.unk_2D, 1);
                    gPlayer.unk_2C = 0;
                    break;
                }
        
            setPlayerAction(0x1E, 0x1F);
            setMainLoopCallbackFunctionIndex(1);
            break;

        case 18:                                    
            if (gSeason == SPRING || gSeason == WINTER) {
                if (gPlayer.action1 == 0x1E || temp == 4) {
                    switch (temp) {             
                        case 0:                         
                            func_8005AE8C(1, 1, 32, 0, 0);
                            break;
                        case 1:                         
                            func_8005AE8C(1, 1, 33, 0, 0);
                            break;
                        case 2:                         
                            func_8005AE8C(1, 1, 34, 0, 0);
                            break;
                        case 3:
                            func_8005AE8C(1, 1, 35, 0, 0);
                            break;
                        case 4:
                            D_801C3E40[0].struct5.flags &= ~0x40;
                            func_80043AD8(0);
                            setPlayerAction(0, 0);
                            setMainLoopCallbackFunctionIndex(1);
                            break;
                            
                        }
                    
                        func_800D55E4(gPlayer.unk_2D, 1);
                        gPlayer.unk_2C = 0;
                        break;
                    }
                
                    setPlayerAction(0x1E, 0x1F);
                    setMainLoopCallbackFunctionIndex(1);
                    break;
                }
            
            if (gPlayer.action1 == 0x1E || temp == 4) {
                switch (temp) {                 
                    case 0:                             
                        func_8005AE8C(1, 1, 36, 0, 0);
                        break;
                    case 1:                             
                        func_8005AE8C(1, 1, 37, 0, 0);
                        break;
                    case 2:                             
                        func_8005AE8C(1, 1, 38, 0, 0);
                        break;
                    case 3:
                        func_8005AE8C(1, 1, 39, 0, 0);
                        break;
                    case 4:
                        D_801C3E40[0].struct5.flags &= ~0x40;
                        func_80043AD8(0);
                        setPlayerAction(0, 0);
                        setMainLoopCallbackFunctionIndex(1);
                        break;
                    }
                
                    func_800D55E4(gPlayer.unk_2D, 1);
                    gPlayer.unk_2C = 0;
                    break;
                }
            
            setPlayerAction(0x1E, 0x1F);
            setMainLoopCallbackFunctionIndex(1);
            break;
        
        case 19:                                    
            if (gSeason == SPRING || gSeason == WINTER) {
                if (gPlayer.action1 == 0x1E || temp == 4) {
                    switch (temp) {             
                        case 0:                         
                            func_8005AE8C(1, 1, 40, 0, 0);
                            break;
                        case 1:                         
                            func_8005AE8C(1, 1, 41, 0, 0);
                            break;
                        case 2:                         
                            func_8005AE8C(1, 1, 42, 0, 0);
                            break;
                        case 3:
                            func_8005AE8C(1, 1, 43, 0, 0);
                            break;
                        case 4:
                            D_801C3E40[0].struct5.flags &= ~0x40;
                            func_80043AD8(0);
                            setPlayerAction(0, 0);
                            setMainLoopCallbackFunctionIndex(1);
                            break;
                        }
                    
                        func_800D55E4(gPlayer.unk_2D, 1);
                        gPlayer.unk_2C = 0;
                        break;
                    }
                
                    setPlayerAction(0x1E, 0x1F);
                    setMainLoopCallbackFunctionIndex(1);
                    break;
                }
            
            if (gPlayer.action1 == 0x1E || temp == 4) {
                switch (temp) {                 
                    case 0:                             
                        func_8005AE8C(1, 1, 44, 0, 0);
                        break;
                    case 1:                             
                        func_8005AE8C(1, 1, 45, 0, 0);
                        break;
                    case 2:                             
                        func_8005AE8C(1, 1, 46, 0, 0);
                        break;
                    case 3:
                        func_8005AE8C(1, 1, 47, 0, 0);
                        break;
                    case 4:
                        D_801C3E40[0].struct5.flags &= ~0x40;
                        func_80043AD8(0);
                        setPlayerAction(0, 0);
                        setMainLoopCallbackFunctionIndex(1);
                        break;
                    }
                
                    func_800D55E4(gPlayer.unk_2D, 1);
                    gPlayer.unk_2C = 0;
                    break;
                }
            
            setPlayerAction(0x1E, 0x1F);
            setMainLoopCallbackFunctionIndex(1);
            break;
        
        case 20:                                    
            if (gSeason == SPRING || gSeason == WINTER) {
                if (gPlayer.action1 == 0x1E || temp == 4) {
                    switch (temp) {             
                        case 0:                         
                            func_8005AE8C(1, 1, 48, 0, 0);
                            break;
                        case 1:                         
                            func_8005AE8C(1, 1, 49, 0, 0);
                            break;
                        case 2:                         
                            func_8005AE8C(1, 1, 50, 0, 0);
                            break;
                        case 3:
                            func_8005AE8C(1, 1, 51, 0, 0);
                            break;
                        case 4:
                            D_801C3E40[0].struct5.flags &= ~0x40;
                            func_80043AD8(0);
                            setPlayerAction(0, 0);
                            setMainLoopCallbackFunctionIndex(1);
                            break;
                        }
                    
                        func_800D55E4(gPlayer.unk_2D, 1);
                        gPlayer.unk_2C = 0;
                        break;
                    }
                
                setPlayerAction(0x1E, 0x1F);
                setMainLoopCallbackFunctionIndex(1);
                break;
            }
            
            if (gPlayer.action1 == 0x1E || temp == 4) {
                switch (temp) {                 
                    case 0:                             
                        func_8005AE8C(1, 1, 52, 0, 0);
                        break;
                    case 1:                             
                        func_8005AE8C(1, 1, 53, 0, 0);
                        break;
                    case 2:                             
                        func_8005AE8C(1, 1, 54, 0, 0);
                        break;
                    case 3:   
                        func_8005AE8C(1, 1, 55, 0, 0);
                        break;
                    case 4:
                        D_801C3E40[0].struct5.flags &= ~0x40;
                        func_80043AD8(0);
                        setPlayerAction(0, 0);
                        setMainLoopCallbackFunctionIndex(1);
                        break;
                    }
                
                func_800D55E4(gPlayer.unk_2D, 1);
                gPlayer.unk_2C = 0;
                break;
            }
            
            setPlayerAction(0x1E, 0x1F);
            setMainLoopCallbackFunctionIndex(1);
            break;

        case 21:                                    
            if (gSeason == SPRING || gSeason == WINTER) {
                if (gPlayer.action1 == 0x1E || temp == 4) {
                    switch (temp) {             
                        case 0:                         
                            func_8005AE8C(1, 1, 56, 0, 0);
                            break;
                        case 1:                         
                            func_8005AE8C(1, 1, 57, 0, 0);
                            break;
                        case 2:                         
                            func_8005AE8C(1, 1, 58, 0, 0);
                            break;
                        case 3:
                            func_8005AE8C(1, 1, 59, 0, 0);
                            break;
                        case 4:
                            D_801C3E40[0].struct5.flags &= ~0x40;
                            func_80043AD8(0);
                            setPlayerAction(0, 0);
                            setMainLoopCallbackFunctionIndex(1);
                            break;
                        }
                
                    func_800D55E4(gPlayer.unk_2D, 1);
                    gPlayer.unk_2C = 0;
                    break;
                
                }
                
                setPlayerAction(0x1E, 0x1F);
                setMainLoopCallbackFunctionIndex(1);
                break;
            }
            
            if (gPlayer.action1 == 0x1E || temp == 4) {
                switch (temp) {                 
                    case 0:                             
                        func_8005AE8C(1, 1, 60, 0, 0);
                        break;
                    case 1:                             
                        func_8005AE8C(1, 1, 61, 0, 0);
                        break;
                    case 2:                             
                        func_8005AE8C(1, 1, 62, 0, 0);
                        break;
                    case 3:
                        func_8005AE8C(1, 1, 63, 0, 0);
                        break;
                    case 4:
                        D_801C3E40[0].struct5.flags &= ~0x40;
                        func_80043AD8(0);
                        setPlayerAction(0, 0);
                        setMainLoopCallbackFunctionIndex(1);
                        break;
                    }
                
                    func_800D55E4(gPlayer.unk_2D, 1);
                    gPlayer.unk_2C = 0;
                    break;
                }
            
            setPlayerAction(0x1E, 0x1F);
            setMainLoopCallbackFunctionIndex(1);
            break;

        case 22:                                    
            if (gSeason == SPRING || gSeason == WINTER) {
                if (gPlayer.action1 == 0x1E || temp == 4) {
                    switch (temp) {             
                        case 0:                         
                            func_8005AE8C(1, 1, 64, 0, 0);
                            break;
                        case 1:                         
                            func_8005AE8C(1, 1, 65, 0, 0);
                            break;
                        case 2:                         
                            func_8005AE8C(1, 1, 66, 0, 0);
                            break;
                        case 3:
                            func_8005AE8C(1, 1, 67, 0, 0);
                            break;
                        case 4:
                            D_801C3E40[0].struct5.flags &= ~0x40;
                            func_80043AD8(0);
                            setPlayerAction(0, 0);
                            setMainLoopCallbackFunctionIndex(1);
                            break;
                        }
                    
                        func_800D55E4(gPlayer.unk_2D, 1);
                        gPlayer.unk_2C = 0;
                        break;
                    }
                
                    setPlayerAction(0x1E, 0x1F);
                    setMainLoopCallbackFunctionIndex(1);
                    break;
                }
            
            if (gPlayer.action1 == 0x1E || temp == 4) {
                switch (temp) {                 
                    case 0:                             
                        func_8005AE8C(1, 1, 68, 0, 0);
                        break;
                    case 1:                             
                        func_8005AE8C(1, 1, 69, 0, 0);
                        break;
                    case 2:                             
                        func_8005AE8C(1, 1, 70, 0, 0);
                        break;
                    case 3:
                        func_8005AE8C(1, 1, 71, 0, 0);
                        break;
                    case 4:
                        D_801C3E40[0].struct5.flags &= ~0x40;
                        func_80043AD8(0);
                        setPlayerAction(0, 0);
                        setMainLoopCallbackFunctionIndex(1);
                        break;
                    }
                
                func_800D55E4(gPlayer.unk_2D, 1);
                gPlayer.unk_2C = 0;
                break;
                
            }
            
            setPlayerAction(0x1E, 0x1F);
            setMainLoopCallbackFunctionIndex(1);
            break;

        case 23:                                    
            switch (temp) {                   
                case 0:                 
                    if (calculateHouseExtensionScore() == 6 && checkLifeEventBit(MARRIED) && npcAffection[gWife] >= 250 && checkLifeEventBit(1) && dogInfo.affection >= 200 && getSumNpcAffection() >= 2500 && func_800DBF90() >= 0x180 && gMaximumStamina >= 0xBE && gHappiness >= 250 && func_8009B5E0() && D_801886D2 >= 10) {          
                        albumBits |= 0x8000;  
                    }
                    
                    setMainLoopCallbackFunctionIndex(13);
                    break;
                
                case 1:                                 
                    func_80055F08_2(0x5AA, 0x61, 1);
                    break;
                }
            
            break;

        }
    }
}

//INCLUDE_ASM(const s32, "game", func_80060454);

void func_80060454(void) {
    if (checkAllSfxInactive()) {
        setMainLoopCallbackFunctionIndex(D_80205230.unk_0);
    }
}

//INCLUDE_ASM(const s32, "game", func_80060490);

// main loop callback 3
void func_80060490(void) {
    func_800A8F74();
} 

//INCLUDE_ASM(const s32, "game", func_800604B0);

void func_800604B0(void) {

    if (func_80036A84(0) || !(mainMap[0].mapStruct9.flags & 1)) {
        
        func_8003F54C(0, 0, -64.0f, 352.0f);
        func_8003F690(0, 1, 0, 0);
        func_8003F360(0, ~(1 | 2), 0);
        
        switch (gCutsceneIndex) {
            case 0x81 ... 0x82:
            case 0x8B:              
            case 0x195 ... 0x199:
                func_8003DDF8(0, 4, 0x4D, 0);
                break;
            case 0x44C:
            case 0x4E2:
            case 0x578:
                func_8003DDF8(0, 4, 0x4A, 0);
                break;
            case 0x1A0:
                func_8003DDF8(0, 4, 0x4C, 0);
                break;
            default:
                func_8003DDF8(0, 4, 0x4B, 0);
                break;
        }
        
        setMainLoopCallbackFunctionIndex(END_OF_FESTIVAL_DAY_2);
    }
}

//INCLUDE_ASM(const s32, "game", func_800605F0);

void func_800605F0(void) {
    if (func_8003F0DC()) {
        setMainLoopCallbackFunctionIndex(END_OF_DAY_1);
    }
}

//INCLUDE_ASM(const s32, "game", func_80060624);

void func_80060624(void) {
    
    short temp = 1;

    int temp1;
    int temp2;
    
    int tempStamina;
    u8 tempTime;

    if (func_80036A84(0) || !(mainMap[0].mapStruct9.flags & 1)) {
        
        func_800610DC();
        
        if (func_80060DC0()) {
            setAudio(0x59);
            setMainLoopCallbackFunctionIndex(0xF);
            D_80205230.unk_0 = 0xD;
            return;
        }
        
        setEntrance(MIDDLE_OF_HOUSE);
        func_80056030(2);
        
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
                gPlayer.fatigue[0] += adjustValue(gPlayer.fatigue[0], -((30 - tempTime) * 3), 100);
            } else {
                gPlayer.fatigue[0] += adjustValue(gPlayer.fatigue[0], -((30 - tempTime) * 2), 100);
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

//INCLUDE_ASM(const s32, "game", func_80060838);
 
void func_80060838(void) {
    
    // check map flags
    if (func_80036A84(0) || !(mainMap[0].mapStruct9.flags & 1)) {

        func_800610DC();

        if (gPlayer.flags & 1) {

            gPlayer.flags &= ~1;
            
            toggleDailyEventBit(0x5C);
             
            setHorseLocation(0xFF);
            
            horseInfo.flags &= ~0x8;
        }

        if (func_80060DC0()) {
            
            // set sound effect
            setAudio(0x59);
            
            setMainLoopCallbackFunctionIndex(0xF);

            // last loading screen callback index?
            D_80205230.unk_0 = 0xD;

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

//INCLUDE_ASM(const s32, "game", setFestivalDailyBits);

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

//INCLUDE_ASM(const s32, "game", func_80060DC0);

bool func_80060DC0(void) {

    bool result = 0;

    if (!checkLifeEventBit(0x4E) && gSeason > AUTUMN && (5 < gDayOfMonth && gDayOfMonth < 10)) {

        if (!getRandomNumberInRange(0, 2) || gDayOfMonth == 9) {
            setLifeEventBit(0x4E);
            result = 1;
            func_80064CF0();
        }
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "game", handleDailyShipment);

void handleDailyShipment(void) {

    gGold += adjustValue(gGold, dailyShippingBinValue, MAX_GOLD);
    dailyShippingBinValue = 0;

    setDailyEventBit(DAILY_SHIPMENT);

}
 
//INCLUDE_ASM(const s32, "game", func_80060EC8);

// has to be compiler generated code
u8 func_80060EC8(f32 arg0, f32 arg1, f32 arg2, f32 arg3) {
   
    s32 temp_s0;
    s32 temp_v0;
    u8 result;
    u32 temp_a2;

    temp_s0 = getAbsoluteValue((s32)(s16)(arg2 - arg0));
    temp_v0 = getAbsoluteValue((s32)(s16)(arg3 - arg1));
    
    if (arg0 <= arg2) {
        
        temp_a2 = temp_s0 << 0x10;
        
        if (arg1 <= arg3) {
            
            if ((s16)temp_s0 <= (s16)temp_v0) {
                result = -(((s32) ((s16) temp_v0 + ((u32) (temp_v0 << 0x10) >> 0x1F)) >> 1) < (s16) temp_s0) & 7;
            } else {
                result = (-(((s32) ((s16) temp_s0 + ((temp_a2) >> 0x1F)) >> 1) < (s16) temp_v0) & 7) | 6;
            }
            
        } else if ((s16) temp_s0 <= (s16) temp_v0) {
            result = (-(((s32) ((s16) temp_v0 + ((u32) (temp_v0 << 0x10) >> 0x1F)) >> 1) < (s16) temp_s0) & 5) | 4;
        } else {
            do {
                result = 5;
                if (((s32) ((s16) temp_s0 + ((temp_a2) >> 0x1F)) >> 1) >= (s16) temp_v0) {
                    result = 6;
                }
            } while (0);
        }
        
    } else if (arg1 <= arg3) {
        
        if ((s16)temp_s0 <= (s16)temp_v0) {
            result = ((s32) ((s16) temp_v0 + ((u32) (temp_v0 << 0x10) >> 0x1F)) >> 1) < (s16) temp_s0;
        } else {
            result = 1;
            if (((s32) ((s16) temp_s0 + ((u32) (temp_s0 << 0x10) >> 0x1F)) >> 1) >= (s16) temp_v0) {
                result = 2;
            }
        }
        
    } else if ((s16) temp_s0 <= (s16) temp_v0) {
        result = 3;
        if (((s32) ((s16) temp_v0 + ((u32) (temp_v0 << 0x10) >> 0x1F)) >> 1) >= (s16) temp_s0) {
            result = 4;
        }
        
    } else {
        result = (-(((s32) ((s16) temp_s0 + ((u32) (temp_s0 << 0x10) >> 0x1F)) >> 1) < (s16) temp_v0) & 3) | 2;
    }
    
    return result;

}

//INCLUDE_ASM(const s32, "game", func_800610DC);

void func_800610DC(void) {
    
    if (gPlayer.unk_2C) {
        
        func_800D5548(gPlayer.unk_2D);

        switch (gPlayer.unk_2C) {
            case 0x60 ... 0x6F:
                func_80088C1C(0xFF, gPlayer.unk_6C);
                break;
            case 0x58 ... 0x5F:
                setDogLocation(0xFF);
                break;
            default:
                break;
        }
        
        gPlayer.unk_2C = 0;
        gItemBeingHeld = 0xFF;
    }
}

//INCLUDE_ASM(const s32, "game", func_80061178);

bool func_80061178(void) {
    
    bool result;

    result = npcAffection[MARIA] >= 220;
    
    if (npcAffection[POPURI] >= 220) {
        result = 1;
    }
    
    if (npcAffection[ELLI] >= 220) {
        result = 1;
    }

    if (npcAffection[ANN] >= 220) {
        result = 1;
    }

    if (npcAffection[KAREN] >= 220) {
        result = 1;
    }
    
    return result;
}

// func_800611E0

//INCLUDE_ASM(const s32, "game", setWifeNameString);

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
// jtbl_8011F1F8
//INCLUDE_ASM(const s32, "game", setDefaultBabyName);

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

//INCLUDE_ASM(const s32, "game", setHarvestKingName);

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

//INCLUDE_ASM(const s32, "game", func_80061690);

void func_80061690(void) {
    D_801886D4[0] = 0xF6;
    D_801886D4[1] = 0xF6;
    D_801886D4[2] = 0xF6;
    D_801886D4[3] = 0xF6;
    D_801886D4[4] = 0xF6;
    D_801886D4[5] = 0xF6;
}

//INCLUDE_ASM(const s32, "game", func_800616CC);

u8 func_800616CC(u8 houseExtensionIndex) {
    
    int goldCost;
    int lumberCost;
    int temp1;
    int temp2;
    
    s16 *arr;
    
    UnknownStruct costStruct = *(UnknownStruct*)houseExtensionPrices;
    UnknownStruct lumberStruct = *(UnknownStruct*)houseExtensionLumberCosts;
    
    arr = (s16*)(&costStruct);
    lumberCost = arr[houseExtensionIndex + 8];
    
    temp1 = adjustValue(gLumber, lumberCost, 999);

    if (temp1 == lumberCost) {    
          
        goldCost = arr[houseExtensionIndex];
        temp2 = adjustValue(gGold, goldCost, 999999);

        temp1 = 1;
        
        if (temp2 == goldCost) {

            gLumber += adjustValue(gLumber, lumberCost, 999);
            gGold += adjustValue(gGold, arr[houseExtensionIndex], 999999);

            houseExtensionConstructionCounter = 0;
            setDailyEventBit(9);
                
            return 0; 
            
        }
        
        return temp1;
        
    }
    
    return 2;
}

// func_80061860
//INCLUDE_ASM(const s32, "game", setFlowerFestivalGoddess);

void setFlowerFestivalGoddess(void) {
    
    u8 temp;

    if (gVoteForFlowerFestivalGoddess < 5 && !getRandomNumberInRange(0, 2)) {
        temp = gVoteForFlowerFestivalGoddess;
    } else {
        temp = getRandomNumberInRange(0, 4);
    }
    
    gFlowerFestivalGoddess = temp;
    
    if (checkLifeEventBit(0x2F) && gFlowerFestivalGoddess == KAREN) {
        gFlowerFestivalGoddess = 0;
    }
}

// func_800618EC
//INCLUDE_ASM(const s32, "game", getBacholeretteWithHighestAffection);

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

//INCLUDE_ASM(const s32, "game", func_80061A1C);

inline void func_80061A1C(u8 arg0, u8 arg1) {
    switch (arg0) {                   
        case 1:
            D_801FD621 = arg1;
            break;
        case 2:
            D_801FC150 = arg1;
            break;
        case 3:
            D_80237412 = arg1;
            break;
    }
}

static inline void setUnknown(u8 value) {
    
    D_801C3F37++;

    func_80061A1C(D_801C3F37, value);

}

//INCLUDE_ASM(const s32, "game", func_80061A88);


void func_80061A88(void) {

    u8 temp;
    u8 temp2;
    
    if (checkSpecialDialogueBit(0x138)) {
        
        toggleSpecialDialogueBit(0x138);
        
        setUnknown(1);
        
    }
    
    if (checkSpecialDialogueBit(0x139)) {
        
        toggleSpecialDialogueBit(0x139);
        
        setUnknown(3);
    }

    if (checkSpecialDialogueBit(0x13A)) {
        
        toggleSpecialDialogueBit(0x13A);
        
        setUnknown(2);
    }
    
    if (checkSpecialDialogueBit(0x13B)) {
        
        toggleSpecialDialogueBit(0x13B);
        
        setUnknown(4);
    }
    
   if (checkSpecialDialogueBit(0x13C)) {
        
        toggleSpecialDialogueBit(0x13C);
       
        setUnknown(5);
    } 

   if (checkSpecialDialogueBit(0x13D)) {
        
        toggleSpecialDialogueBit(0x13D);
       
        setUnknown(6);
    }

   if (checkSpecialDialogueBit(0x13E)) {
        
        toggleSpecialDialogueBit(0x13E);
       
        setUnknown(7);
    } 

   if (checkSpecialDialogueBit(0x13F)) {
        
        toggleSpecialDialogueBit(0x13F);
       
        setUnknown(8);
    } 

   if (checkSpecialDialogueBit(0x140)) {
        
        toggleSpecialDialogueBit(0x140);
       
        setUnknown(9);
    } 

   if (checkSpecialDialogueBit(0x141)) {
        
        toggleSpecialDialogueBit(0x141);
       
        setUnknown(10);
    } 
    
    if (D_801C3F37 >= 3) {
        toggleSpecialDialogueBit(0x142);
    }
}

//INCLUDE_ASM(const s32, "game", setRecipes);

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
//INCLUDE_ASM(const s32, "game", handleHouseConstruction);

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
            result = 1;
            
        }
    }
    
    return result;
}

// func_8006252C
//INCLUDE_ASM(const s32, "game", setLetters);

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

    if (!checkMailRead(0x16) && checkLifeEventBit(MARIA_HARRIS_BABY)) {
        setMail(0x16);
    }

    if (!checkMailRead(0x17) && npcAffection[GRAY] >= 120 && gSeason == SPRING && !getRandomNumberInRange(0, 10)) {
        setMail(0x17);
    }

    if (!checkMailRead(0x18) && checkLifeEventBit(POPURI_GRAY_BABY)) {
        setMail(0x18);
    }

    if (!checkMailRead(0x19) && npcAffection[JEFF] >= 120 && gSeason == SPRING && !getRandomNumberInRange(0, 10)) {
        setMail(0x19);
    }

    if (!checkMailRead(0x1A) && checkLifeEventBit(ELLI_JEFF_BABY)) {
        setMail(0x1A);
    }

    if (!checkMailRead(0x1B) && checkLifeEventBit(CLIFF_GONE) && !getRandomNumberInRange(0, 10)) {
        setMail(0x1B);
    }
 
    if (!checkMailRead(0x1C) && checkLifeEventBit(ANN_CLIFF_BABY)) {
        setMail(0x1C);
    }

    if (!checkMailRead(0x1D) && npcAffection[KAI] >= 120 && gSeason == SPRING && !getRandomNumberInRange(0, 10)) {
        setMail(0x1D);
    }

    if (!checkMailRead(0x1E) && checkLifeEventBit(KAREN_KAI_BABY)) {
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
//INCLUDE_ASM(const s32, "game", func_80063A2C);

u16 func_80063A2C(u8 arg0) {

    u16 arr[80];
    
    MemoryRead_32 *ptr;
    MemoryRead_32 *ptr2 = D_8011F25C;
    
    ptr = arr;

    if (((u32)arr | (u32)D_8011F25C) % 4) {
        do {
            *(Unaligned32*)ptr++ = *(Unaligned32*)ptr2++;
        } while (ptr2 != (D_8011F25C + 0xA));        
    } else {
        do {
            *(Aligned32*)ptr++ = *(Aligned32*)ptr2++;
        } while (ptr2 != (D_8011F25C + 0xA));       
    }

    return arr[arg0];
}

/* rodata */

// D_8011F228-D_8011FA700

//INCLUDE_RODATA(const s32, "game", houseExtensionPrices);

static const s16 houseExtensionPrices[6] = { -5000, -3000, -2000, -30000, -7000, -1000 };

//INCLUDE_RODATA(const s32, "game", houseExtensionLumberCosts);

static const s16 houseExtensionLumberCosts[6] = { -450, -300, -250, -580, -350, -150 };

//INCLUDE_RODATA(const s32, "game", houseConstructionDays);

static const u8 houseConstructionDays[6] = { 3, 3, 3, 3, 3, 1 };

//INCLUDE_RODATA(const s32, "game", lifeEventHouseConstructionBits);

static const u16 lifeEventHouseConstructionBits[6] = { 0x10, 0x11, 0x12, 0x13, 0x14, 0x15 };

//INCLUDE_RODATA(const s32, "game", animalLocationsHouseConstruction);

static const u8 animalLocationsHouseConstruction[6] = { 0x52, 0x52, 0x57, 0x52, 0x52, 0x57 };

INCLUDE_RODATA(const s32, "game", D_8011F25C);

/*
static const u16 D_8011F25C[75] = { 
     0, 1, 2, 3, 4, 5, 
     8, 9, 10, 11, 12, 
     13, 14, 15, 16, 17, 
     18, 19, 20, 21, 22, 
     23, 24, 25, 26, 27, 
     28, 29, 30, 31, 32,
     33, 34, 35, 36, 36, 
     36, 36, 36, 36, 36,
     36, 36, 36, 36, 36, 
     36, 36, 36, 36, 38, 
     39, 39, 39, 39, 39, 
     40, 41, 42, 43, 44,
     45, 46, 47, 47, 48, 
     49, 50, 51, 52, 53,
     54, 55, 58, 59, 0, 
     0x0000 0000, 0x0000 0000 
};
*/

