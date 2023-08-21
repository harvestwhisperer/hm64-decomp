#include "common.h"

#include "system/controller.h"
#include "system/map.h"

#include "animals.h"
#include "npc.h"

extern u8 gHour;
extern u8 gDayOfWeek;
extern u8 gDayOfMonth;
extern u8 gSeason;
extern u8 gForecast;
extern u8 gYear;

extern u8 gHarvestKing;
extern u8 gFlowerFestivalGoddess;
extern u8 gVoteForFlowerFestivalGoddess;

extern u8 gWife;
extern u8 gWifePregnancyCounter;
extern u8 gWifeConceptionCounter;

extern u8 houseExtensionConstructionCounter;

extern Player gPlayer;
extern u8 gMaximumStamina;
extern u8 gHappiness;
extern u8 gItemBeingHeld;

extern u32 gCutsceneFlags;
extern u16 gCurrentSongIndex;
extern u8 gNamingScreenIndex;

extern u8 gBaseMapIndex;
extern Vec4f globalLightingRgba;
extern u16 gTileContextFlags;

extern Dog dogInfo;
extern Horse horseInfo;
extern FarmAnimal gFarmAnimals[];
extern Chicken gChickens[0xC];

extern u8 npcAffection[TOTAL_NPCS];
extern u32 dailyEventBits[];

extern Controller controllers[4];

extern u8 houseConstructionDays[6];
extern u16 lifeEventHouseConstructionBits[6];
extern u8 animalLocationsHouseConstruction[6];

extern u8 gFarmName[6];
extern u8 gWifeName[6];
extern u8 gBabyName[6];
extern u8 harvestKingName[6];

extern UnknownMapStruct1 D_80158248;

void adjustDogAffection(s8 value);                            
void adjustHorseAffection(s8 value);     
void setAnimalLocations(u8);  
void setDogLocation(u8);        
void setHorseLocation(u16);                            
           
void setPlayerAction(u16, u16);    
u8 checkFatigueLevel(); 
u32 acquireKeyItem(u8);
void removeKeyItem(u32);  
u8 checkHaveKeyItem(u8);

void setMainLoopCallbackFunctionIndex(u16 callbackIndex); 

u32 checkDailyEventBit(u16 bitIndex);
void setDailyEventBit(u16 bitIndex);
void toggleDailyEventBit(u16 bitIndex);                  
void setLifeEventBit(u16);                    
void toggleLifeEventBit(u16);                            
u32 checkLifeEventBit(u16 bitIndex);   
u32 checkSpecialDialogueBit(u16 bitIndex);
void setSpecialDialogueBit(u16 bitIndex);
void toggleSpecialDialogueBit(u16 bitIndex);    
u32 checkMailRead(u16);                            
void setMail(u16);      
void setRecipes(void);  

void setExit(u16 locationIndex);
void setClockNewDay(void);
void startNewDay(void);   

u8 checkAllSfxInactive(void);

u16 getRandomNumberInRange(u16 min, u16 max);
               
void func_8002F6F0();                               
void func_8002F8F0(u8, u8, u8, u16, s32);                   
void func_8002F730();   
void func_8002F7C8(u8, u8, u8, u8);   
void func_8002FB3C();
void func_8002FCB4(u16, u8);    
u8 func_80036A84(u16);       
void func_8003BE98(u16, u8, u8, u8, u8);
void func_8003BF7C(u16, u8, u8, u8, u8, u8);                                               
void func_8003C504(u32);     
void func_8003DDF8(u32, u32, u32, u32);
s32 func_8003F0DC();  
void func_8003F360(u16, s16, u8);     
void func_8003F54C(u16, f32, f32, f32);
void func_8003F690(u16, u8, u8, u8);  
void func_80043430(u16, u16, u16, u16);
void func_80046860();
void func_80046BB8();   
void func_80046C98();
void func_80046CF4(); 
void func_80056030(u8);        
void func_8005C07C(u8, u8);    
u8 func_80060DC0();                                 
void func_80060E58();                                   
void func_800610DC();
void func_80061A88(void); 
void func_8006EA44(u8, u8, u8);            
void func_80088C1C(u8, u8);            
u8 func_8009B3DC();                             
u8 func_8009B4EC();                                
u8 func_8009B564();          
u16 func_800A87C4();       
void func_800A8F74();
void func_800ACB5C(u16);                          
u8 func_800ACBB8(u16 songIndex);
void func_800ACC50(u16);   
void func_800D5548(u8);  
void func_800ED8A0(u8*, u8);

u8 func_8003F910(u8, u16, void*, void*, void*, void*, void*, void*, void*, u32, u32, u16, u16, f32, f32, f32); 
u8 func_80043A88();                                
void func_8005CDCC();                              
void func_800D55E4(u8, u8);                               
u16 func_800D5A6C(u16 index);      
                  
extern u8 D_8011F244;
extern u8 D_8011F245;
extern u16 D_8011F250;
extern u8 D_8011F258;
extern u8 D_8011F259;

extern u8 D_801C3F36;
extern u8 D_801C3F96;
extern u8 D_801C3F97;
extern u8 D_801C3F98;
extern u8 D_801C3F99;
extern u8 D_801C3F9A;

// Vec4f
extern f32 D_80180718;
extern f32 D_8018071C;
extern f32 D_80180720;
extern f32 D_80180724;

extern u8 D_801886D4[6];

extern u32 D_8013D45C;  

extern u8 D_8016F6E3;
extern u8 D_8016FAD8;
extern u16 D_8016FDF0;
extern u8 D_8017026F;
extern u8 D_8017027E;
extern u8 D_80170444;
extern u8 D_801886A0;

extern u16 D_801890D6;
extern u8 gBabyAge;
extern u8 D_801891C4;
extern s32 D_801891D4;
extern u8 D_8018A059;
extern u16 D_801FB686;   
extern u8 D_801FB9A4;
extern u8 D_801C3F37;
extern u8 D_801FC150;
extern u8 D_801FC155;
extern u8 D_801FC15C;
extern u8 D_801FD618;
extern u8 D_801FD621;
extern u8 D_80205201;

extern u8 D_8016FFE8;
extern u8 D_80170464;
extern u8 D_80182FB8;
extern u8 D_801FC155;
extern u8 D_80205624;

// related to callback index
extern MainLoopCallbackInfo D_80205230;

extern u8 D_802055D1;
extern u8 D_8020562C;
extern u8 D_80237412;
extern u8 D_80205236;                   

extern void *D_D47F00;
extern void *D_D49B80;
extern void *D_D49B80_2;
extern void *tvSprites_romTextureStart;

// has jumptable
//INCLUDE_ASM(const s32, "game", func_80059D90);

void func_80059D90(void) {

    u16 temp;

    if (!checkLifeEventBit(MARRIED)) goto not_married;

    gWifeConceptionCounter += adjustValue(gWifeConceptionCounter, 1, 120);
    
    if ((checkLifeEventBit(HAVE_BABY)) && (gBabyAge < 0xFF)) {
        gBabyAge += 1;
    }

    if (!checkLifeEventBit(HAVE_BABY) && checkLifeEventBit(WIFE_PREGNANT) && (gWifePregnancyCounter >= 60)) {
         
        toggleLifeEventBit(WIFE_PREGNANT);
        setLifeEventBit(HAVE_BABY);
        setSpecialDialogueBit(4);
        
         switch (gWife) {                     
            case MARIA:                              
                temp = 0x151;
                break;
            case POPURI:                             
                temp = 0x152;
                break;
            case ELLI:                                
                temp = 0x153;
                break;
            case ANN:                               
                temp = 0x154;
                break;
            case KAREN:                                
                temp = 0x155;
                break;
            default:
                goto check_kid;
        }

        setSpecialDialogueBit(temp);
     }
        
check_kid:

    if (!checkLifeEventBit(HAVE_BABY) && checkLifeEventBit(WIFE_PREGNANT)) {
        gWifePregnancyCounter += 1;
    }

    if (!checkLifeEventBit(HAVE_BABY) && checkLifeEventBit(HAVE_BABY_BED) && !checkLifeEventBit(WIFE_PREGNANT) && (npcAffection[gWife] >= 250) && (gWifeConceptionCounter >= 30)) {
        
        setLifeEventBit(WIFE_PREGNANT);
        
        gWifePregnancyCounter = 0;
        
         switch (gWife) {                        
            case MARIA:                               
                temp = 0x14C;
                break;
            case POPURI:                               
                temp = 0x14D;
                break;
            case ELLI:                                
                temp = 0x14E;
                break;
            case ANN:                             
                temp = 0x14F;
                break;
            case KAREN:                               
                temp = 0x150;
                break;
            default:
                goto not_married;
        }

        setSpecialDialogueBit(temp);
    }

not_married:

    if (checkLifeEventBit(MARIA_HARRIS_MARRIED)) {

        D_801FB9A4 += adjustValue(D_801FB9A4, 1, 120);

        if (checkLifeEventBit(MARIA_HARRIS_BABY) && (D_80205201 < 120)) {
            D_80205201 += 1;
        }

        if (!checkLifeEventBit(MARIA_HARRIS_BABY) && checkLifeEventBit(0x21) && (D_8016FAD8 >= 60)) {
            toggleLifeEventBit(0x21);
            setLifeEventBit(MARIA_HARRIS_BABY);
            setSpecialDialogueBit(0x32);
            toggleSpecialDialogueBit(0x37);
        }

        if (!checkLifeEventBit(MARIA_HARRIS_BABY) && checkLifeEventBit(0x21)) {
            D_8016FAD8 += 1;
        }

        if (!checkLifeEventBit(MARIA_HARRIS_BABY) && !checkLifeEventBit(0x21) && (D_801C3F96 >= 250) && (D_801FB9A4 >= 30)) {
            setLifeEventBit(0x21);
            D_8016FAD8 = 0;
            setSpecialDialogueBit(0x37);
        }
    }

     if (checkLifeEventBit(POPURI_GRAY_MARRIED)) {

        D_801886A0 += adjustValue(D_801886A0, 1, 120);

        if (checkLifeEventBit(POPURI_GRAY_BABY) && (D_801C3F36 < 120)) {
            D_801C3F36 += 1;
        }

        if (!checkLifeEventBit(POPURI_GRAY_BABY) && checkLifeEventBit(0x22) && (D_801891C4 >= 60)) {
            toggleLifeEventBit(0x22);
            setLifeEventBit(POPURI_GRAY_BABY);
            setSpecialDialogueBit(0x33);
            toggleSpecialDialogueBit(0x38);
        }

        if (!checkLifeEventBit(POPURI_GRAY_BABY) && checkLifeEventBit(0x22)) {
            D_801891C4 += 1;
        }

        if (!checkLifeEventBit(POPURI_GRAY_BABY) && !checkLifeEventBit(0x22) && (D_801C3F97 >= 250) && (D_801886A0 >= 30)) {
            setLifeEventBit(0x22);
            D_801891C4 = 0;
            setSpecialDialogueBit(0x38);
        }
    }

    if (checkLifeEventBit(ELLI_JEFF_MARRIED)) {

        D_8017027E += adjustValue(D_8017027E, 1, 120);

        if (checkLifeEventBit(ELLI_JEFF_BABY) && (D_801FC15C < 120)) {
            D_801FC15C += 1;

        }

        if (!checkLifeEventBit(ELLI_JEFF_BABY) && checkLifeEventBit(0x23) && (D_8020562C >= 60)) {
            toggleLifeEventBit(0x23);
            setLifeEventBit(ELLI_JEFF_BABY);
            setSpecialDialogueBit(0x34);
            toggleSpecialDialogueBit(0x39);
        }

        if (!checkLifeEventBit(ELLI_JEFF_BABY) && checkLifeEventBit(0x23)) {
            D_8020562C += 1;
        }

        if (!checkLifeEventBit(ELLI_JEFF_BABY) && !checkLifeEventBit(0x23) && (D_801C3F98 >= 250) && (D_8017027E >= 30)) {
            setLifeEventBit(0x23);
            D_8020562C = 0;
            setSpecialDialogueBit(0x39);
        }
    }

    if (checkLifeEventBit(ANN_CLIFF_MARRIED)) {

        D_80170444 += adjustValue(D_80170444, 1, 120);

        if (checkLifeEventBit(ANN_CLIFF_BABY) && (D_8016F6E3 < 120)) {
            D_8016F6E3 += 1;
        }

        if (!checkLifeEventBit(ANN_CLIFF_BABY) && checkLifeEventBit(0x24) && (D_8018A059 >= 60)) {
            toggleLifeEventBit(0x24);
            setLifeEventBit(ANN_CLIFF_BABY);
            setSpecialDialogueBit(0x35);
            toggleSpecialDialogueBit(0x3A);
        }

        if (!checkLifeEventBit(ANN_CLIFF_BABY) && checkLifeEventBit(0x24)) {
            D_8018A059 += 1;
        }

        if (!checkLifeEventBit(ANN_CLIFF_BABY) && !checkLifeEventBit(0x24) && (D_801C3F99 >= 250) && (D_80170444 >= 30)) {
            setLifeEventBit(0x24);
            D_8018A059 = 0;
            setSpecialDialogueBit(0x3A);
        }
    }

    if (checkLifeEventBit(KAREN_KAI_MARRIED)) {
        
        D_802055D1 += adjustValue(D_802055D1, 1, 120);

        if ((checkLifeEventBit(KAREN_KAI_BABY)) && (D_801FD618 < 120)) {
            D_801FD618 += 1;
        }

        if (!checkLifeEventBit(KAREN_KAI_BABY) && checkLifeEventBit(0x25) && (D_8017026F >= 60)) {
            toggleLifeEventBit(0x25);
            setLifeEventBit(KAREN_KAI_BABY);
            setSpecialDialogueBit(0x36);
            toggleSpecialDialogueBit(0x3B);
        }

        if (!checkLifeEventBit(KAREN_KAI_BABY) && checkLifeEventBit(0x25)) {
            D_8017026F += 1;
        }

        if (!checkLifeEventBit(KAREN_KAI_BABY) && !checkLifeEventBit(0x25) && (D_801C3F9A >= 250) && (D_802055D1 >= 30)) {
            setLifeEventBit(0x25);
            D_8017026F = 0;
            setSpecialDialogueBit(0x3B);
        }
    }
}

// jtbl_8011EF80
//INCLUDE_ASM(const s32, "game", func_8005A60C);

void func_8005A60C(void) {

    u8* temp;

    if (!checkLifeEventBit(MARRIED)) goto handleAnimals;

    switch (gWife) {     
        case 0:
            temp = &npcAffection[MARIA];
            break;
        case 1:
            temp = &npcAffection[POPURI];
            break;
        case 2:
            temp = &npcAffection[ELLI];
            break;
        case 3:
            temp = &npcAffection[ANN];
            break;
        case 4:
            temp = &npcAffection[KAREN];
            break;
        default:
            goto handleBaby;
        }

    *temp += adjustValue(*temp, -1, MAX_AFFECTION);

handleBaby:
    if (checkLifeEventBit(HAVE_BABY)) {
        temp = &npcAffection[BABY];
        *temp += adjustValue(*temp, -1, MAX_AFFECTION);
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
        setSpecialDialogueBit(0x12);
    } else {
        toggleSpecialDialogueBit(0x12);
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
        setSpecialDialogueBit(0x27);
    }

    if (gForecast == RAIN) {
        setSpecialDialogueBit(0x13);
    } else {
        toggleSpecialDialogueBit(0x13);
    }

    if (checkLifeEventBit(0x10)) {
        setSpecialDialogueBit(0x18);
    }

    if (checkLifeEventBit(0x11)) {
        setSpecialDialogueBit(0x19);
    }

    if (checkLifeEventBit(0x12)) {
        setSpecialDialogueBit(0x1A);
    }

    if (checkLifeEventBit(0x13)) {
        setSpecialDialogueBit(0x1B);
    }

    // spirit festival
    if ((gSeason == WINTER) && (gDayOfMonth == 27)) {

        if (gHarvestKing == PLAYER) {
            setSpecialDialogueBit(0x142);
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
        toggleSpecialDialogueBit(0x142);
    }

    if ((gSeason == WINTER) && (gDayOfMonth == 10)) {
        setSpecialDialogueBit(THANKSGIVING);
    } else {
        toggleSpecialDialogueBit(THANKSGIVING);
    }

    if ((gSeason == WINTER) && (gDayOfMonth == 30)) {
        setSpecialDialogueBit(NEW_YEARS_EVE);
    } else {
        toggleSpecialDialogueBit(NEW_YEARS_EVE);
    }

    if ((gSeason == WINTER) && (gDayOfMonth == 24)) {
        setSpecialDialogueBit(STARRY_NIGHT_FESTIVAL_DIALOGUE_BIT);
    } else {
        toggleSpecialDialogueBit(STARRY_NIGHT_FESTIVAL_DIALOGUE_BIT);
    }

    if ((gSeason == WINTER) && (gDayOfMonth == 11)) {
        setSpecialDialogueBit(MARIA_BIRTHDAY);
    } else {
        toggleSpecialDialogueBit(MARIA_BIRTHDAY);
    }

    if ((gSeason == SPRING) && (gDayOfMonth == 22)) {
        setSpecialDialogueBit(POPURI_BIRTHDAY);
    } else {
        toggleSpecialDialogueBit(POPURI_BIRTHDAY);
    }

    if ((gSeason == FALL) && (gDayOfMonth == 1)) {
        setSpecialDialogueBit(ELLI_BIRTHDAY);
    } else {
        toggleSpecialDialogueBit(ELLI_BIRTHDAY);
    }

    if ((gSeason == SUMMER) && (gDayOfMonth == 14)) {
        setSpecialDialogueBit(ANN_BIRTHDAY);
    } else {
        toggleSpecialDialogueBit(ANN_BIRTHDAY);
    }

    if ((gSeason == WINTER) && (gDayOfMonth == 29)) {
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

// func_8005AE64
u32 adjustValue(s32 current, s32 amount, s32 max) {

    s32 sum;
    s32 temp;

    temp = amount;
    sum = current + temp;
    
    if (max < sum) {
        temp -= sum - max;
        sum = max;
    }
    
    if (sum < 0) {
        temp -= sum;
    }
    
    return temp;
}

//INCLUDE_ASM(const s32, "game", func_8005AE8C);

void func_8005AE8C(u16 arg0, u16 arg1, u16 arg2, int arg3, u16 arg4) {
  
    u32 temp;
  
    func_8002F6F0();
    func_80046C98();
    func_8002FCB4(0, 0);
    
    switch (arg0) {
        case 0:
          func_8003F54C(0, 24.0f, -64.0f, 352.0f);
          temp = 0;
          func_8003F690(0, temp, 0, 0);
          break;
        
        case 1:
          func_8003F54C(0, 0, -64.0f, 352.0f);
          temp = 1;
          func_8003F690(0, temp, 0, 0);
          break;
        
        default:
          break;
    }

    func_8003F360(0, -4, arg4);
  
    func_8003DDF8(0, arg1, arg2, arg3);
  
    setMainLoopCallbackFunctionIndex(0xA);
    
    D_8013D45C = 0;

    setPlayerAction(0, 0);
}

//INCLUDE_ASM(const s32, "game", func_8005AF94);

void func_8005AF94(u16 arg0, u16 arg1, u16 arg2, u32 arg3, u16 arg4) {
    
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


    func_8003F360(0, -4, arg4);
    func_80043430(0, arg1, arg2, arg3);

    setMainLoopCallbackFunctionIndex(9);

    controllers[0].unk_1C = 0;
    
    setPlayerAction(0, 0);

}

// jumptable: jbtl_8011EF98, 0xFA398
// 99.9%....
#ifdef PERMUTER
void func_8005B09C(u8 arg0) {

    D_80205230.unk_6 = arg0;

    switch (arg0) {
        default:
            goto func_end;
        case 0:
            func_8002F6F0();
            func_80046C98();
            func_8002FCB4(0, 0);
            func_8003F54C(0, 0, -64.0f, 352.0f);
            func_8003F690(0, 1, 0, 0);
            func_8003F360(0, -4, 2);
            func_80043430(0, 1, 0, 0);
            break;
            
        case 1:
            func_8002F6F0();
            func_80046C98();
            func_8002FCB4(0, 0);
            func_8003F54C(0, 0, -64.0f, 352.0f);
            func_8003F690(0, 1, 0, 0);
            func_8003F360(0, -4, 2);
            func_80043430(0, 1, 1, 0);
            break;
            
        case 2:
            func_8002F6F0();
            func_80046C98();
            func_8002FCB4(0, 0);
            func_8003F54C(0, 24.0f, -64.0f, 352.0f);
            func_8003F690(0, 0, 0, 0);
            func_8003F360(0, -4, 0);
            func_80043430(0, 0x3D, 0xC, 0x40);
            break;
            
        case 3:
            func_8002F6F0();
            func_80046C98();
            func_8002FCB4(0, 0);
            func_8003F54C(0, 24.0f, -64.0f, 352.0f);
            func_8003F690(0, 0, 0, 0);
            func_8003F360(0, -4, 0);
            func_80043430(0, 0x3D, 0xB, 0x40);
            break;
            
        case 4:
            func_8002F6F0();
            func_80046C98();
            func_8002FCB4(0, 0);
            func_8003F54C(0, 24.0f, -64.0f, 352.0f);
            func_8003F690(0, 0, 0, 0);
            func_8003F360(0, -4, 0);
            func_80043430(0, 0x3D, 0x12, 0x40);;
            break;
            
        case 5:
            func_8002F6F0();
            func_80046C98();
            func_8002FCB4(0, 0);
            func_8003F54C(0, 24.0f, -64.0f, 352.0f);
            func_8003F690(0, 0, 0, 0);
            func_8003F360(0, -4, 0);
            func_80043430(0, 0x3D, 0xD, 0x40);
            break;
            
        case 6:
            func_8002F6F0();
            func_80046C98();
            func_8002FCB4(0, 0);
            func_8003F54C(0, 24.0f, -64.0f, 352.0f);
            func_8003F690(0, 0, 0, 0);
            func_8003F360(0, -4, 0);
            func_80043430(0, 0x3D, 0xE, 0x40);
            break;
            
        case 7:
            func_8002F6F0();
            func_80046C98();
            func_8002FCB4(0, 0);
            func_8003F54C(0, 24.0f, -64.0f, 352.0f);
            func_8003F690(0, 0, 0, 0);
            func_8003F360(0, -4, 0);
            func_80043430(0, 0x3D, 0xF, 0x40);
            break;
            
        case 8:
            func_8002F6F0();
            func_80046C98();
            func_8002FCB4(0, 0);
            func_8003F54C(0, 24.0f, -64.0f, 352.0f);
            func_8003F690(0, 0, 0, 0);
            func_8003F360(0, -4, 0);
            func_80043430(0, 0x3E, 7, 0x40);
            break;
            
        case 9:
            func_8002F6F0();
            func_80046C98();
            func_8002FCB4(0, 0);
            func_8003F54C(0, 24.0f, -64.0f, 352.0f);
            func_8003F690(0, 0, 0, 0);
            func_8003F360(0, -4, 0);
            func_80043430(0, 0x3E, 6, 0x40);
            break;
            
        case 10:
            func_8002F6F0();
            func_80046C98();
            func_8002FCB4(0, 0);
            func_8003F54C(0, 24.0f, -64.0f, 352.0f);
            func_8003F690(0, 0, 0, 0);
            func_8003F360(0, -4, 0);
            func_80043430(0, 0x3E, 2, 0x40);
            break;
            
        case 11:
            func_8002F6F0();
            func_80046C98();
            func_8002FCB4(0, 0);
            func_8003F54C(0, 24.0f, -64.0f, 352.0f);
            func_8003F690(0, 0, 0, 0);
            func_8003F360(0, -4, 0);
            func_80043430(0, 0x3E, 0, 0x40);
            break;   
            
        case 12:
            func_8002F6F0();
            func_80046C98();
            func_8002FCB4(0, 0);
            func_8003F54C(0, 24.0f, -64.0f, 352.0f);
            func_8003F690(0, 0, 0, 0);
            func_8003F360(0, -4, 0);
            func_80043430(0, 0x3E, 1, 0x40);
            break;      
            
        case 13:
            if (((gSeason == SPRING) | (gSeason == WINTER))) {
                func_8002F6F0();
                func_80046C98();
                func_8002FCB4(0, 0);
                func_8003F54C(0, 0, -64.0f, 352.0f);
                func_8003F690(0, 1, 0, 0);
                func_8003F360(0, -4, 2);
                func_80043430(0, 0x44, 0, 0);
            } else {
                func_8002F6F0();
                func_80046C98();
                func_8002FCB4(0, 0);
                func_8003F54C(0, 0, -64.0f, 352.0f);
                func_8003F690(0, 1, 0, 0);
                func_8003F360(0, -4, 2);
                func_80043430(0, 0x44, 0xA, 0);
            }            
            break;
            
        case 14:
            if (((gSeason == SPRING) | (gSeason == WINTER))) {
                func_8002F6F0();
                func_80046C98();
                func_8002FCB4(0, 0);
                func_8003F54C(0, 0, -64.0f, 352.0f);
                func_8003F690(0, 1, 0, 0);
                func_8003F360(0, -4, 2);
                func_80043430(0, 0x44, 1, 0x40);
            } else {
                func_8002F6F0();
                func_80046C98();
                func_8002FCB4(0, 0);
                func_8003F54C(0, 0, -64.0f, 352.0f);
                func_8003F690(0, 1, 0, 0);
                func_8003F360(0, -4, 2);
                func_80043430(0, 0x44, 0xB, 0);
            }
            break;
            
        case 15:
            if (((gSeason == SPRING) | (gSeason == WINTER))) {
                func_8002F6F0();
                func_80046C98();
                func_8002FCB4(0, 0);
                func_8003F54C(0, 0, -64.0f, 352.0f);
                func_8003F690(0, 1, 0, 0);
                func_8003F360(0, -4, 2);
                func_80043430(0, 0x44, 2, 0x40);
            } else {
                func_8002F6F0();
                func_80046C98();
                func_8002FCB4(0, 0);
                func_8003F54C(0, 0, -64.0f, 352.0f);
                func_8003F690(0, 1, 0, 0);
                func_8003F360(0, -4, 2);
                func_80043430(0, 0x44, 0xC, 0);
            }
            break;
            
        case 16:
            if (((gSeason == SPRING) | (gSeason == WINTER))) {
                func_8002F6F0();
                func_80046C98();
                func_8002FCB4(0, 0);
                func_8003F54C(0, 0, -64.0f, 352.0f);
                func_8003F690(0, 1, 0, 0);
                func_8003F360(0, -4, 2);
                func_80043430(0, 0x44, 3, 0x40);
            } else {
                func_8002F6F0();
                func_80046C98();
                func_8002FCB4(0, 0);
                func_8003F54C(0, 0, -64.0f, 352.0f);
                func_8003F690(0, 1, 0, 0);
                func_8003F360(0, -4, 2);
                func_80043430(0, 0x44, 0xD, 0);
            }
            break;
            
        case 17:
            if (((gSeason == SPRING) | (gSeason == WINTER))) {
                func_8002F6F0();
                func_80046C98();
                func_8002FCB4(0, 0);
                func_8003F54C(0, 0, -64.0f, 352.0f);
                func_8003F690(0, 1, 0, 0);
                func_8003F360(0, -4, 2);
                func_80043430(0, 0x44, 4, 0x40);
            } else {
                func_8002F6F0();
                func_80046C98();
                func_8002FCB4(0, 0);
                func_8003F54C(0, 0, -64.0f, 352.0f);
                func_8003F690(0, 1, 0, 0);
                func_8003F360(0, -4, 2);
                func_80043430(0, 0x44, 0xE, 0);
            }
            break;
            
        case 18:
            if (((gSeason == SPRING) | (gSeason == WINTER))) {
                func_8002F6F0();
                func_80046C98();
                func_8002FCB4(0, 0);
                func_8003F54C(0, 0, -64.0f, 352.0f);
                func_8003F690(0, 1, 0, 0);
                func_8003F360(0, -4, 2);
                func_80043430(0, 0x44, 5, 0x40);
            } else {
                func_8002F6F0();
                func_80046C98();
                func_8002FCB4(0, 0);
                func_8003F54C(0, 0, -64.0f, 352.0f);
                func_8003F690(0, 1, 0, 0);
                func_8003F360(0, -4, 2);
                func_80043430(0, 0x44, 0xF, 0);
            }
            break;
            
        case 19:
            if (((gSeason == SPRING) | (gSeason == WINTER))) {
                func_8002F6F0();
                func_80046C98();
                func_8002FCB4(0, 0);
                func_8003F54C(0, 0, -64.0f, 352.0f);
                func_8003F690(0, 1, 0, 0);
                func_8003F360(0, -4, 2);
                func_80043430(0, 0x44, 6, 0x40);
            } else {
                func_8002F6F0();
                func_80046C98();
                func_8002FCB4(0, 0);
                func_8003F54C(0, 0, -64.0f, 352.0f);
                func_8003F690(0, 1, 0, 0);
                func_8003F360(0, -4, 2);
                func_80043430(0, 0x44, 0x10, 0);
            }
            break;
            
        case 20:
            if (((gSeason == SPRING) | (gSeason == WINTER))) {
                func_8002F6F0();
                func_80046C98();
                func_8002FCB4(0, 0);
                func_8003F54C(0, 0, -64.0f, 352.0f);
                func_8003F690(0, 1, 0, 0);
                func_8003F360(0, -4, 2);
                func_80043430(0, 0x44, 7, 0x40);
            } else {
                func_8002F6F0();
                func_80046C98();
                func_8002FCB4(0, 0);
                func_8003F54C(0, 0, -64.0f, 352.0f);
                func_8003F690(0, 1, 0, 0);
                func_8003F360(0, -4, 2);
                func_80043430(0, 0x44, 0x11, 0);
            }
            break;
            
        case 21:
            if (((gSeason == SPRING) | (gSeason == WINTER))) {
                func_8002F6F0();
                func_80046C98();
                func_8002FCB4(0, 0);
                func_8003F54C(0, 0, -64.0f, 352.0f);
                func_8003F690(0, 1, 0, 0);
                func_8003F360(0, -4, 2);
                func_80043430(0, 0x44, 8, 0x40);
            } else {
                func_8002F6F0();
                func_80046C98();
                func_8002FCB4(0, 0);
                func_8003F54C(0, 0, -64.0f, 352.0f);
                func_8003F690(0, 1, 0, 0);
                func_8003F360(0, -4, 2);
                func_80043430(0, 0x44, 0x12, 0);
            }
            break;
            
        case 22:
            if (((gSeason == SPRING) | (gSeason == WINTER))) {
                func_8002F6F0();
                func_80046C98();
                func_8002FCB4(0, 0);
                func_8003F54C(0, 0, -64.0f, 352.0f);
                func_8003F690(0, 1, 0, 0);
                func_8003F360(0, -4, 2);
                func_80043430(0, 0x44, 9, 0x40);
            } else {
                func_8002F6F0();
                func_80046C98();
                func_8002FCB4(0, 0);
                func_8003F54C(0, 0, -64.0f, 352.0f);
                func_8003F690(0, 1, 0, 0);
                func_8003F360(0, -4, 2);
                func_80043430(0, 0x44, 0x13, 0);
            }
            break;
        
        case 23:
            func_8002F6F0();
            func_80046C98();
            func_8002FCB4(0, 0);
            func_8003F54C(0, 0, -64.0f, 352.0f);
            func_8003F690(0, 1, 0, 0);
            func_8003F360(0, -4, 2);
            func_80043430(0, 1, 0x15, 0);
            break;

    }
           
    setMainLoopCallbackFunctionIndex(9);
    D_8013D440[0].unk_1C = 0;
    setPlayerToDefaultStanding(0, 0);
       
func_end:    
    setMainLoopCallbackFunctionIndex(0xC);
}
#else
INCLUDE_ASM(const s32, "game", func_8005B09C);
#endif

//INCLUDE_ASM(const s32, "game", func_8005C00C);

void func_8005C00C(void) {
    
    if (D_801891D4 < 0) {
        setMainLoopCallbackFunctionIndex(1);
        return;
    }
    
    if (!checkDailyEventBit(0x4B)) {
        // set audio settings for level
        func_8006EA44(gBaseMapIndex, gSeason, gHour);
    }
    
    // set lighting for level based on weather
    func_8005C07C(8, 1);
}

// jtbl_8011EFF8
INCLUDE_ASM(const s32, "game", func_8005C07C);

//INCLUDE_ASM(const s32, "game", func_8005C940);

void func_8005C940(u16 arg0, u16 arg1) {
    
    func_8003BF7C(0, 0, 0, 0, 0, 8);
    func_8002F8F0(0, 0, 0, 0, 8);
    
    // set song speed with default
    func_800ACB5C(gCurrentSongIndex);
    
    D_80205230.unk_0 = arg1;

    globalLightingRgba.r = 0;
    globalLightingRgba.g = 0;
    globalLightingRgba.b = 0;
    globalLightingRgba.a = 0;
    
    // vec4f
    D_80180718 = 0;
    D_8018071C = 0;
    D_80180720 = 0;
    D_80180724 = 0;
    
    if (arg1) {
        setMainLoopCallbackFunctionIndex(6);
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
    setMainLoopCallbackFunctionIndex(7);
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

    if ((func_80036A84(0)) && (func_800ACBB8(gCurrentSongIndex))) {

        func_8002F7C8(0, 0, 0, 0);
        func_8003BE98(0, 0, 0, 0, 0);
        func_80046BB8();
        func_80046860();

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

    // gTileContextFlags = flags on global tile struct
    // bits 4 and 5
    if (!(gTileContextFlags & 0x18)) {
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
        
        func_8003F910(0, 0x78, &D_D47F00, &D_D49B80, &D_D49B80_2, &tvSprites_romTextureStart, 0x8023B400, 0x8023CC00, 0x8023CE00, 0x8023D200, 0, 4, 0xFE, 106.0f, -15.0f, 0);
        func_8003F910(1, 0x78, &D_D47F00, &D_D49B80, &D_D49B80_2, &tvSprites_romTextureStart, 0x8023B400, 0x8023CC00, 0x8023CE00, 0x8023D200, 0, 0xD, 0xFE, 106.0f, -15.0f, 0);
        func_8005CDCC();
        
        setMainLoopCallbackFunctionIndex(1);
        
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
    
    if (func_8003F0DC() & 0xFF) {

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
        setMainLoopCallbackFunctionIndex(1);
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
        // player
        case 0:
            namePtr = gPlayer.name;
            break;
        // farm
        case 1:
            namePtr = &gFarmName;
            break;
        // dog
        case 2:
            namePtr = dogInfo.name;
            break;
        // horse
        case 3:
            namePtr = horseInfo.name; 
        break;
        // baby
        case 4:
            namePtr = &gBabyName;
            break;
        // cow or sheep
        case 5:
            namePtr = gFarmAnimals[D_801FC155].name;
            break;
        // cow or sheep
        case 6:
            namePtr = gFarmAnimals[D_801FC155].name;
            break;
        // chicken
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
    
    if ((gBaseMapIndex == COOP) && checkLifeEventBit(3)) {
        
        gNamingScreenIndex = 7;
        D_801FC155 = D_8016FFE8;
        gChickens[D_8016FFE8].upkeep &= 0xFEFF;
        
        setMainLoopCallbackFunctionIndex(0x33);
        toggleLifeEventBit(3);
        setLifeEventBit(0x38);
        
        set = 1;
        
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
    }
    
    if ((gBaseMapIndex == BARN) && checkLifeEventBit(4)) {
        
        gNamingScreenIndex = 5;
        D_801FC155 = D_80170464;
        gFarmAnimals[D_80170464].upkeep &= 0xF7FF;
            
        setMainLoopCallbackFunctionIndex(0x33);
        toggleLifeEventBit(4);
            
        set = 1;
        
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
    
    }
    
    if (gHour >= 18U) {
        setDailyEventBit(0xF);
        setDailyEventBit(0x10);
    }
    
    if (!set) {
        setMainLoopCallbackFunctionIndex(4);
    }
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
INCLUDE_ASM(const s32, "game", func_8005D2B0);

//INCLUDE_ASM(const s32, "game", func_80060454);

void func_80060454(void) {
    if (checkAllSfxInactive()) {
        setMainLoopCallbackFunctionIndex(D_80205230.unk_0);
    }
}

//INCLUDE_ASM(const s32, "game", func_80060490);

void func_80060490(void) {
    func_800A8F74();
}

INCLUDE_ASM(const s32, "game", func_800604B0);

INCLUDE_ASM(const s32, "game", func_800605F0);

//INCLUDE_ASM(const s32, "game", func_80060624);

void func_80060624(void) {
    
    short temp = 1;

    int temp1;
    int temp2;
    
    int tempStamina;
    u8 tempTime;

    if ((func_80036A84(0)) || !(D_80158248.flags & 1)) {
        
        func_800610DC();
        
        if (func_80060DC0()) {
            func_800ACC50(0x59);
            setMainLoopCallbackFunctionIndex(0xF);
            D_80205230.unk_0 = 0xD;
            return;
        }
        
        setExit(0xC);
        func_80056030(2);
        
        if (func_800A87C4()) {
            setMainLoopCallbackFunctionIndex(3);
            return;
        }
        
        if (!checkDailyEventBit(0)) {
            func_80060E58();
        }
        
        if (checkFatigueLevel() != 3) {
            
            tempTime = gHour;
            
            if (tempTime < 6) {
                tempTime += 0x18;
            }
            
            if (checkLifeEventBit(0x5A)) {

                tempStamina = gPlayer.fatigue[0];
                temp1 = -((30 - tempTime) * 3);
                temp2 = tempStamina + temp1;
                
                if ((101 - temp) < temp2) {
                    temp2 -= 100;
                    temp = temp2;
                    temp1 -= temp;
                    temp2 = 100;
                }
                
                if (temp2 < 0) {
                    temp1 -= temp2;
                }
                
                 gPlayer.fatigue[0] += temp1;
                
            } else {
                
                tempStamina = gPlayer.fatigue[0];
                temp2 = -((30 - tempTime) * 2);
                temp1 = tempStamina + temp2;
                
                if ((101 - temp) < temp1) {
                    temp1 -= 100;
                    temp = temp1;
                    temp2 -= temp;
                    temp1 = 100;
                }
                if (temp1 < 0) {
                    temp2 -= temp1;
                }
                
                gPlayer.fatigue[0] += temp2;
            }
        }
        
        if ((gHour - 18) < 5U) {
            setLifeEventBit(0x60);
        } else {
            toggleLifeEventBit(0x60);
        }
        
        setClockNewDay();
        
        gPlayer.currentStamina = gMaximumStamina;
        
        startNewDay();
        setExit(0xC);
        
        setMainLoopCallbackFunctionIndex(2);
        
        setDailyEventBit(0x53);
        setDailyEventBit(0x54);
    }
}

//INCLUDE_ASM(const s32, "game", func_80060838);

void func_80060838(void) {
    
    // check map flags
    if ((func_80036A84(0)) || !(D_80158248.flags & 1)) {

        func_800610DC();

        if (gPlayer.flags & 1) {

            gPlayer.flags &= 0xFFFE;
            
            toggleDailyEventBit(0x5C);
            
            setHorseLocation(0xFF);
            
            horseInfo.flags &= 0xFFF7;
        }

        if (func_80060DC0()) {
            
            // set sound effect
            func_800ACC50(0x59);
            
            setMainLoopCallbackFunctionIndex(0xF);

            // last loading screen callback index?
            D_80205230.unk_0 = 0xD;

            return;
        }

        setExit(HOUSE_START);
        
        // setup/handle loading
        func_80056030(2);

        if (func_800A87C4()) {
            setMainLoopCallbackFunctionIndex(3);
            return;
        }

        if (!checkDailyEventBit(0)) {
            // handle daily shipment
            func_80060E58();
        }

        startNewDay();

        setExit(HOUSE_START);
        
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
        setDailyEventBit(0x4D);
        setDailyEventBit(0x4E);
    }
    
    if (gSeason == SPRING && gDayOfMonth == 8) {
        setDailyEventBit(0x30);
        setDailyEventBit(0x28);
        setDailyEventBit(0x4D);
        setDailyEventBit(0x4E);
    }
    
    if (gSeason == SPRING && gDayOfMonth == 17) {
        setDailyEventBit(0x30);
        setDailyEventBit(0x28);
        setDailyEventBit(0x4D);
        setDailyEventBit(0x4E);
    }

    if (gSeason == FALL && gDayOfMonth == 28) {
        setDailyEventBit(0x30);
        setDailyEventBit(0x28);
        setDailyEventBit(0x4D);
        setDailyEventBit(0x4E);
    }
    
    if (gSeason == SPRING && gDayOfMonth == 23) {
        setDailyEventBit(0x30);
        setDailyEventBit(0x28);
        setDailyEventBit(0x4D);
        setDailyEventBit(0x4E);
    }

    if (gSeason == SUMMER && gDayOfMonth == 1) {
        setDailyEventBit(0x30);
        setDailyEventBit(0x4E);
    }
    
    if (gSeason == SUMMER && gDayOfMonth == 9) {
        setDailyEventBit(0x30);
        setDailyEventBit(0x28);
        setDailyEventBit(0x4D);
        setDailyEventBit(0x4E);
    }

    if (gSeason == SUMMER && gDayOfMonth == 17) {
        setDailyEventBit(0x30);
        setDailyEventBit(0x4E);
    }

    if (gSeason == SUMMER && gDayOfMonth == 24) {
        setDailyEventBit(0x30);
        setDailyEventBit(0x28);
        setDailyEventBit(0x4D);
        setDailyEventBit(0x4E);
    }

    if (gSeason == FALL && gDayOfMonth == 4) {
        setDailyEventBit(0x30);
        setDailyEventBit(0x28);
        setDailyEventBit(0x4D);
        setDailyEventBit(0x4E);
    }

    if (gSeason == FALL && gDayOfMonth == 12) {
        setDailyEventBit(0x30);
        setDailyEventBit(0x28);
        setDailyEventBit(0x4D);
        setDailyEventBit(0x4E);
    }
    
    if (gSeason == FALL && gDayOfMonth == 20) {
        setDailyEventBit(0x30);
        setDailyEventBit(0x28);
        setDailyEventBit(0x4D);
        setDailyEventBit(0x4E);
    }

    if (gSeason == WINTER && gDayOfMonth == 10) {
        setDailyEventBit(0x30);
        setDailyEventBit(0x4E);
    }
    
    if (gSeason == WINTER && gDayOfMonth == 19) {
        setDailyEventBit(0x30);
        setDailyEventBit(0x28);
        setDailyEventBit(0x4D);
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

INCLUDE_ASM(const s32, "game", func_80060DC0);

INCLUDE_ASM(const s32, "game", func_80060E58);

INCLUDE_ASM(const s32, "game", func_80060EC8);

//INCLUDE_ASM(const s32, "game", func_800610DC);

void func_800610DC(void) {

    int temp; 
    
    if (gPlayer.unk_2C) {
        
        func_800D5548(gPlayer.unk_2D);
        
        temp = gPlayer.unk_2C;
        
        if (temp >= 0x58) {
        
            if (temp >= 0x60) {
            
                if (temp < 0x70) {
                    func_80088C1C(0xff, gPlayer.unk_6C);
                }
                
            } else {
                setDogLocation(0xff);
            }
        }
        
        gPlayer.unk_2C = 0;
        gItemBeingHeld = 0xff;
    }
}

//INCLUDE_ASM(const s32, "game", func_80061178);

s32 func_80061178(void) {
    
    s32 result;

    result = (npcAffection[MARIA] < 220) ^ 1;
    
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
// dlabel jtbl_8011F1F8
//INCLUDE_ASM(const s32, "game", setDefaultBabyName);

void setDefaultBabyName(u8 wife) {
    
    switch (wife) {
        case 0:
            gBabyName[0] = char_L;
            gBabyName[1] = char_i;
            gBabyName[2] = char_y;
            gBabyName[3] = char_a;
            gBabyName[4] = 0xFF;
            gBabyName[5] = 0xFF;
            break;
        case 1:
            gBabyName[0] = char_P;
            gBabyName[1] = char_a;
            gBabyName[2] = char_r;
            gBabyName[3] = char_s;
            gBabyName[4] = char_l;
            gBabyName[5] = char_y;
            break;
        case 2:
            gBabyName[0] = char_C;
            gBabyName[1] = char_o;
            gBabyName[2] = char_c;
            gBabyName[3] = char_o;
            gBabyName[4] = char_t;
            gBabyName[5] = char_e;
            break;
        case 3:
            gBabyName[0] = char_M;
            gBabyName[1] = char_i;
            gBabyName[2] = char_n;
            gBabyName[3] = char_t;
            gBabyName[4] = 0xFF;
            gBabyName[5] = 0xFF; 
            break;
        case 4:
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

#ifdef PERMUTER
u8 func_800616CC(u8 arg0) {

    u8 result;
    
    s32 lumberCost;
    s32 goldCost;
 
    s32 lumberAfterCost;
    s32 adjustedLumber;

    s32 goldAfterCost;
    s32 adjustedGold;

    s16 *ptr1;
    s16 *ptr2;

    UnknownStruct costStruct = *(UnknownStruct*)D_8011F228;
    UnknownStruct lumberStruct = *(UnknownStruct*)D_8011F234;

    ptr1 = (s16*)&costStruct;
    ptr2 = (s16*)&lumberStruct;
    
    lumberCost = ptr1[arg0];
    goldCost = ptr2[arg0];
    
    lumberAfterCost = (u16)(D_80189E50) + (int)ptr1[arg0];
    adjustedLumber = (u16*)&costStruct;
    
    if (lumberAfterCost < MAX_LUMBER) {
        adjustedLumber -= (s32)(ptr1[arg0] - MAX_LUMBER);
        lumberAfterCost = MAX_LUMBER;
    }
    
    if (lumberAfterCost < 0) {
        adjustedLumber -= lumberAfterCost;
    }

    result = 2;

    if (ptr1[arg0] == ptr2[arg0]) {
        
        goldAfterCost = D_801FD60C + goldCost;
        
        if (goldAfterCost > MAX_GOLD) {
            adjustedGold = (ptr2[arg0] + (goldAfterCost - MAX_GOLD));
            goldAfterCost = MAX_GOLD;
        }
        if (goldAfterCost < 0) {
            adjustedGold -= goldAfterCost;
        }

        result = 1;

        if (adjustedGold == ptr2[arg0]) {

            //adjustedLumber = D_80189E50 - ptr2[arg0];

            if (ptr2[arg0] + D_80189E50 < MAX_LUMBER) {
                adjustedLumber -= ((s32)(ptr1[arg0] + MAX_LUMBER));
                lumberAfterCost = MAX_LUMBER;
            }

            if (lumberAfterCost < 0) {
                lumberAfterCost -= adjustedLumber;
            }

            D_80189E50 += lumberAfterCost;

            goldAfterCost = D_801FD60C + ptr2[arg0];
            
            if (D_801FD60C + goldCost > MAX_GOLD) {
                adjustedGold = (goldAfterCost - MAX_GOLD);
                goldAfterCost = MAX_GOLD;
            }
            
            if (goldAfterCost < 0) {
                adjustedGold -= goldAfterCost;
            }
            
            D_801FD60C += adjustedGold;
            
            D_801FC151 = 0;
            
            func_80065074(9);
            
            result = 0;
        }
        
    }
    
    return result;
}
#else
INCLUDE_ASM(const s32, "game", func_800616CC);
#endif

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
//INCLUDE_ASM(const s32, "game", getSumBacholerettesWithAffectionThreshold);

u8 getSumBacholerettesWithAffectionThreshold(u8 affectionLevel) {
    
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

INCLUDE_ASM(const s32, "game", func_80061A1C);

INCLUDE_ASM(const s32, "game", func_80061A88);

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

    if (!checkMailRead(0) && ((!checkLifeEventBit(MARRIED)) || gWife != MARIA) && (npcAffection[MARIA] >= 120) && (gSeason == SPRING) && !(getRandomNumberInRange(0, 10))) {
        setMail(0);
    }

    if (!checkMailRead(1) && ((!checkLifeEventBit(MARRIED)) || gWife != MARIA) && (npcAffection[MARIA] >= 140) && (gSeason == SUMMER) && !(getRandomNumberInRange(0, 10))) {
        setMail(1);
    }

    if (!checkMailRead(2) && ((!checkLifeEventBit(MARRIED)) || gWife != MARIA) && (npcAffection[MARIA] >= 160) && (gSeason == FALL) && !(getRandomNumberInRange(0, 10))) {
        setMail(2);
    }

    if (!checkMailRead(3) && ((!checkLifeEventBit(MARRIED)) || gWife != MARIA) && (npcAffection[MARIA] >= 180) && (gSeason == WINTER) && !(getRandomNumberInRange(0, 10))) {
        setMail(3);
    }

    if (!checkMailRead(4) && ((!checkLifeEventBit(MARRIED)) || (gWife != POPURI)) && (npcAffection[POPURI] >= 120) && (gSeason == SPRING) && !(getRandomNumberInRange(0, 10))) {
        setMail(4);
    }

    if (!checkMailRead(5) && ((!checkLifeEventBit(MARRIED)) || (gWife != POPURI)) && (npcAffection[POPURI] >= 140) && (gSeason == SUMMER) && !(getRandomNumberInRange(0, 10))) {
        setMail(5);
    }
    if (!checkMailRead(6) && ((!checkLifeEventBit(MARRIED)) || (gWife != POPURI)) && (npcAffection[POPURI] >= 160) && (gSeason == FALL) && !(getRandomNumberInRange(0, 10))) {
        setMail(6);
    }

    if (!checkMailRead(7) && ((!checkLifeEventBit(MARRIED)) || (gWife != POPURI)) && (npcAffection[POPURI] >= 180) && (gSeason == WINTER) && !(getRandomNumberInRange(0, 10))) {
        setMail(7);
    }

    if (!checkMailRead(8) && ((!checkLifeEventBit(MARRIED)) || (gWife != ELLI)) && (npcAffection[ELLI] >= 120) && (gSeason == SPRING) && !(getRandomNumberInRange(0, 10))) {
        setMail(8);
    }

    if (!checkMailRead(9) && ((!checkLifeEventBit(MARRIED)) || (gWife != ELLI)) && (npcAffection[ELLI] >= 140) && (gSeason == SUMMER) && !(getRandomNumberInRange(0, 10))) {
        setMail(9);
    }

    if (!checkMailRead(0xA) && ((!checkLifeEventBit(MARRIED)) || (gWife != ELLI)) && (npcAffection[ELLI] >= 160) && (gSeason == FALL) && !(getRandomNumberInRange(0, 10))) {
        setMail(0xA);
    }

    if (!checkMailRead(0xB) && ((!checkLifeEventBit(MARRIED)) || (gWife != ELLI)) && (npcAffection[ELLI] >= 180) && (gSeason == WINTER) && !(getRandomNumberInRange(0, 10))) {
        setMail(0xB);
    }

    if (!checkMailRead(0xC) && ((!checkLifeEventBit(MARRIED)) || (gWife != ANN)) && (npcAffection[ANN] >= 120) && (gSeason == SPRING) && !(getRandomNumberInRange(0, 10))) {
        setMail(0xC);
    }

    if (!checkMailRead(0xD) && ((!checkLifeEventBit(MARRIED)) || (gWife != ANN)) && (npcAffection[ANN] >= 140) && (gSeason == SUMMER) && !(getRandomNumberInRange(0, 10))) {
        setMail(0xD);
    }

    if (!checkMailRead(0xE) && ((!checkLifeEventBit(MARRIED)) || (gWife != ANN)) && (npcAffection[ANN] >= 160) && (gSeason == FALL) && !(getRandomNumberInRange(0, 10))) {
        setMail(0xE);
    }

    if (!checkMailRead(0xF) && ((!checkLifeEventBit(MARRIED)) || (gWife != ANN)) && (npcAffection[ANN] >= 180) && (gSeason == WINTER) && !(getRandomNumberInRange(0, 10))) {
        setMail(0xF);
    }

    if (!checkMailRead(0x10) && ((!checkLifeEventBit(MARRIED)) || (gWife != KAREN)) && (npcAffection[KAREN] >= 120) && (gSeason == SPRING) && !(getRandomNumberInRange(0, 10))) {
        setMail(0x10);
    }
    
    if (!checkMailRead(0x11) && ((!checkLifeEventBit(MARRIED)) || (gWife != KAREN)) && (npcAffection[KAREN] >= 140) && (gSeason == SUMMER) && !(getRandomNumberInRange(0, 10))) {
        setMail(0x11);
    }

    if (!checkMailRead(0x12) && ((!checkLifeEventBit(MARRIED)) || (gWife != KAREN)) && (npcAffection[KAREN] >= 160) && (gSeason == FALL) && !(getRandomNumberInRange(0, 10))) {
        setMail(0x12);
    }

    if (!checkMailRead(0x13) && ((!checkLifeEventBit(MARRIED)) || (gWife != KAREN)) && (npcAffection[KAREN] >= 180) && (gSeason == WINTER) && !(getRandomNumberInRange(0, 10))) {
        setMail(0x13);
    }

    if (!checkMailRead(0x14) && (npcAffection[MAYOR] >= 120) && (gSeason == SPRING) && !(getRandomNumberInRange(0, 10))) {
        setMail(0x14);
    }

    if (!checkMailRead(0x15) && (npcAffection[HARRIS] >= 120) && (gSeason == SPRING) && !(getRandomNumberInRange(0, 10))) {
        setMail(0x15);
    }

    if (!checkMailRead(0x16) && (checkLifeEventBit(MARIA_HARRIS_BABY))) {
        setMail(0x16);
    }

    if (!checkMailRead(0x17) && (npcAffection[GRAY] >= 120) && (gSeason == SPRING) && !(getRandomNumberInRange(0, 10))) {
        setMail(0x17);
    }

    if (!checkMailRead(0x18) && (checkLifeEventBit(POPURI_GRAY_BABY))) {
        setMail(0x18);
    }

    if (!checkMailRead(0x19) && (npcAffection[JEFF] >= 120) && (gSeason == SPRING) && !(getRandomNumberInRange(0, 10))) {
        setMail(0x19);
    }

    if (!checkMailRead(0x1A) && (checkLifeEventBit(ELLI_JEFF_BABY))) {
        setMail(0x1A);
    }

    if (!checkMailRead(0x1B) && (checkLifeEventBit(0x2D)) && !(getRandomNumberInRange(0, 10))) {
        setMail(0x1B);
    }

    if (!checkMailRead(0x1C) && (checkLifeEventBit(ANN_CLIFF_BABY))) {
        setMail(0x1C);
    }

    if (!checkMailRead(0x1D) && (npcAffection[KAI] >= 120) && (gSeason == SPRING) && !(getRandomNumberInRange(0, 10))) {
        setMail(0x1D);
    }

    if (!checkMailRead(0x1E) && (checkLifeEventBit(KAREN_KAI_BABY))) {
        setMail(0x1E);
    }

    if (!checkMailRead(0x1F) && (checkLifeEventBit(0x2E)) && !(getRandomNumberInRange(0, 10))) {
        setMail(0x1F);
    }

    if (!checkMailRead(0x20) && ((npcAffection[KENT] + npcAffection[STU]) >= 160) && (gSeason == SPRING) && !(getRandomNumberInRange(0, 10))) {
        setMail(0x20);
    }

    if (!checkMailRead(0x21) && (npcAffection[RICK] >= 120) && (gSeason == SPRING) && !(getRandomNumberInRange(0, 10))) {
        setMail(0x21);
    }
    
    if (!checkMailRead(0x22) && (npcAffection[BASIL] >= 0x64) && (gSeason == WINTER) && !(getRandomNumberInRange(0, 10))) {
        setMail(0x22);
    }

    if (!checkMailRead(0x23) && (checkLifeEventBit(SHADY_SALESMAN_MARIA_CUTSCENE))) {
        setMail(0x23);
    }

    if (!checkMailRead(0x24) && (checkLifeEventBit(MARRIED)) && (gWife == MARIA)) {
        setMail(0x24);
    }

    if (!checkMailRead(0x25) && (checkLifeEventBit(MARRIED)) && (gWife == POPURI)) {
        setMail(0x25);
    }

    if (!checkMailRead(0x26) && (checkLifeEventBit(MARRIED)) && (gWife == ELLI)) {
        setMail(0x26);
    }

    if (!checkMailRead(0x27) && (checkLifeEventBit(MARRIED)) && (gWife == ANN)) {
        setMail(0x27);
    }

    if (!checkMailRead(0x28) && (checkLifeEventBit(MARRIED)) && (gWife == KAREN)) {
        setMail(0x28);
    }

    if (!checkMailRead(0x29) && (checkLifeEventBit(HAVE_BABY)) && (gWife == MARIA)) {
        setMail(0x29);
    }

    if (!checkMailRead(0x2A) && (checkLifeEventBit(HAVE_BABY)) && (gWife == POPURI)) {
        setMail(0x2A);
    }

    if (!checkMailRead(0x2B) && (checkLifeEventBit(HAVE_BABY)) && (gWife == ELLI)) {
        setMail(0x2B);
    }

    if (!checkMailRead(0x2C) && (checkLifeEventBit(HAVE_BABY)) && (gWife == ANN)) {
        setMail(0x2C);
    }

    if (!checkMailRead(0x2D) && (checkLifeEventBit(HAVE_BABY)) && (gWife == KAREN)) {
        setMail(0x2D);
    }

    if (!checkMailRead(0x2E) && (checkDailyEventBit(0x34))) {
        setMail(0x2E);
    }

    if (!checkMailRead(0x2F) && (checkDailyEventBit(0x35))) {
        setMail(0x2F);
    }

    if (!checkMailRead(0x30) && (checkDailyEventBit(0x37))) {
        setMail(0x30);
    }

    if (!checkMailRead(0x31) && (checkDailyEventBit(0x39))) {
        setMail(0x31);
    }

    if (!checkMailRead(0x32) && (checkDailyEventBit(WON_CALENDAR_STICKERS_FROM_RAFFLE))) {
        setMail(0x32);
    }

    if (!checkMailRead(0x33) && (checkLifeEventBit(HAVE_KITCHEN)) && (checkLifeEventBit(HAVE_BATHROOM)) && (checkLifeEventBit(HAVE_STAIRS)) && (checkLifeEventBit(HAVE_GREENHOUSE)) && (checkLifeEventBit(HAVE_LOG_TERRACE)) && (checkLifeEventBit(HAVE_BABY_BED))) {
        setMail(0x33);
    }
    
    if (!checkMailRead(0x34) && (checkDailyEventBit(0x36))) {
        setMail(0x34);
    }

    if (!checkMailRead(0x35) && (checkDailyEventBit(0x3A))) {
        setMail(0x35);
    }

    if (!checkMailRead(0x36) && (checkDailyEventBit(0x3B))) {
        setMail(0x36);
    }

    if (!checkMailRead(0x37) && (checkDailyEventBit(0x3C))) {
        setMail(0x37);
    }

    if (!checkMailRead(0x38) && (checkDailyEventBit(0x3D))) {
        setMail(0x38);
    }

    if (!checkMailRead(0x39) && (checkDailyEventBit(0x3E))) {
        setMail(0x39);
    }

    if (!checkMailRead(0x3A) && (((gYear == 1 && (1 < gSeason)) || (1 < gYear)))) {
        setMail(0x3A);
    }

    if (!checkMailRead(0x3B) && (gSeason == SPRING) && (gDayOfMonth == 20)) {
        setMail(0x3B);
    }

    if (!checkMailRead(0x3D) && (gSeason == SPRING) && (gDayOfMonth == 15)) {
        setMail(0x3D);
    }

    if (!checkMailRead(0x3E) && (gSeason == SPRING) && (gDayOfMonth == 19)) {
        setMail(0x3E);
    }

    if (!checkMailRead(0x3F) && (gSeason == SUMMER) && (gDayOfMonth == 1)) {
        setMail(0x3F);
    }

    if (!checkMailRead(0x40) && !(getRandomNumberInRange(0, 20))) {
        setMail(0x40);
    }
    
    if (!checkMailRead(0x41) && (gSeason == SPRING) && (gDayOfMonth == 30)) {
        setMail(0x41);
    }

    if (!checkMailRead(0x42) && (gSeason == FALL) && (gDayOfMonth == 30)) {
        setMail(0x42);
    }

    if (!checkMailRead(0x43) && (gSeason == FALL) && (gDayOfMonth == 10)) {
        setMail(0x43);
    }

    if (!checkMailRead(0x44) && (gSeason == FALL) && (gDayOfMonth == 1)) {
        setMail(0x44);
    }

    if (!checkMailRead(0x45) && (gSeason == WINTER) && (gDayOfMonth == 7)) {
        setMail(0x45);
    }

    if (!checkMailRead(0x46) && (gSeason == WINTER) && (gDayOfMonth == 6)) {
        setMail(0x46);
    }

    if (!checkMailRead(0x47) && (gSeason == WINTER) && (gDayOfMonth == 17)) {
        setMail(0x47);
    }

    if (!checkMailRead(0x49) && (gYear >= 2) && (gSeason == SPRING) && (gDayOfMonth == 24)) {
        setMail(0x49);
    }
    
    if (!checkMailRead(0x4A) && (gYear >= 2) && (gSeason == SPRING)) {
        setMail(0x4A);
    }

    if (!checkMailRead(0x4B) && (checkLifeEventBit(MARRIED))) {
        setMail(0x4B);
    }

    if (!checkMailRead(0x4C) && (gYear >= 3) && (gSeason == SPRING) && (gDayOfMonth == 3)) {
        setMail(0x4C);
    }
}

// D_8011F25C
INCLUDE_ASM(const s32, "game", func_80063A2C);


/* data */

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

