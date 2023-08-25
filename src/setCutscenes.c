#include "common.h"

#include "setCutscenes.h"
#include "npc.h"

u32 checkLifeEventBit(u16);        
void setLifeEventBit(u16);     
u32 checkDailyEventBit(u16);                        
void setDailyEventBit(u16);                              
void toggleDailyEventBit(u16);         

u32 adjustValue(s32 current, s32 amount, s32 max);

u8 checkHaveKeyItem(u8);  

void loadCutscene(u32);         

extern u8 gEntranceIndex;
extern u8 previousMapIndex;
extern u8 gBaseMapIndex;

extern u32 gCutsceneFlags;
extern u16 gCutsceneIndex;

extern u8 gHour;
extern u8 gDayOfWeek;
extern u8 gDayOfMonth;
extern u8 gSeason;
extern u8 gYear;
extern u8 gWeather;

extern u8 gFlowerFestivalGoddess;
extern u8 gHappiness;
extern u8 gHouseExtensionSelection;
extern u8 gWife;

extern u8 npcAffection[TOTAL_NPCS];

extern u8 blueMistFlowerPlot;

// flower shop points
extern u8 D_80181B10;
extern u8 D_801C4216;

void func_8002F730();   
u32 func_800469A8(u16, void*);                       
void func_80060E58();                                
u8 func_800616CC(u8);   
u8 checkHaveTool(u8); 
u8 func_8009B2BC(u8);                               
u16 func_8009C324(u8);                                

//void nuPiReadRom(u32 rom_addr, void* buf_ptr, u32 size);

extern Addresses D_80114AF0[];
extern void *D_80114C70[];
extern void *D_80114D30[];
extern u16 D_80114DF0[];

extern u32 D_801891D4;
extern u16 D_8018981C;
extern u32 dailyShippingBinValue;


INCLUDE_ASM(const s32, "setCutscenes", func_8009BF90);

// jtbl_80120EC0
INCLUDE_ASM(const s32, "setCutscenes", func_8009C054);

// has rodata
/*
u16 func_8009C054(u16 mapIndex) {
    
    u16 index = 0xFFFF;
    
    if (!(gCutsceneFlags & 1)) {
        gCutsceneFlags &= ~2;
        switch (mapIndex) {
            case FARM:
                index = setFarmCutscenes();
                break;
            case MOUNTAIN_1:
                index = setMountain1Cutscenes();
                break;
            case MOUNTAIN_2:
                index = func_800A1C04();
                break;
            case TOP_OF_MOUNTAIN_1:
                index = func_800A2250();
                break;
            case MOON_MOUNTAIN:
                index = setMountain2Cutscenes();
                break;
            case SPRING:
                index = setSpringCutscenes();
                break;
            case HARVEST_SPRITE_CAVE:
                index = setHarvestSpriteCaveCutscenes();
                break;
            case CAVE:
                index = setCaveCutscenes();
                break;
            case HOUSE:
                index = func_800A29B0();
                break;
            case KITCHEN:
                index = func_800A2FA8();
                break;
            case RANCH:
                index = func_800A3F04();
                break;
            case RANCH_STORE:
                index = setRanchStoreCutscenes();
                break;
            case VINEYARD:
                index = func_800A4878();
                break;
            case VINEYARD_HOUSE:
                index = setVineyardHouseCutscenes();
                break;
            case VINEYARD_CELLAR:
                index = setVineyardCellarCutscenes();
                break;
            case VILLAGE_1:
                index = func_800A4E50();
                break;
            case VILLAGE_2:
                index = func_800A5314();
                break;
            case RICK_STORE:
                index = setRickStoreCutscenes();
                break;
            case FLOWER_SHOP:
                index = setFlowerShopCutscenes();
                break;
            case BAKERY:
                index = func_800A5F48();
                break;
            case MAYOR_HOUSE:
                index = func_800A6440();
                break;
            case LIBRARY:
                index = func_800A6634();
                break;
            case CHURCH:
                index = setChurchCutscenes();
                break;
            case TAVERN:
                index = func_800A68C0();
                break;
            case SQUARE:
                index = func_800A6A14();
                break;
            case RACE_TRACK:
                index = func_800A6EE4();
                break;
            case BEACH:
                index = setBeachCutscenes();
                break;
            case ROAD:
                index = setRoadCutscenes();
                break;
            default:
                break;
            }
        
        if (index != 0xFFFF) {
            nuPiReadRom(D_80114AF0[index].romAddr, D_80114D30[index], D_80114AF0[index].vaddr - D_80114AF0[index].romAddr);
            D_801891D4 = 0;
            func_800469A8(D_80114DF0[index], D_80114C70[index]);
            func_8002F730();
            D_8018981C = index;
            gCutsceneFlags |= 1;
            toggleDailyEventBit(0x53);
        }
    }
    
    return index;
}

*/

// jtbl_80121030
INCLUDE_ASM(const s32, "setCutscenes", func_8009C324);

//INCLUDE_ASM(const s32, "setCutscenes", setFarmCutscenes);

u16 setFarmCutscenes(void) {

    u16 index = 0xFFFF;
    u8 set = 0;
    u8 tempHappiness;

    if (checkDailyEventBit(0x52)) { 
        gCutsceneIndex = 0x101;
        gCutsceneFlags |= 2;
        loadCutscene(0);
        set = 1;
    }
    
    if (!set) {
        if (checkDailyEventBit(1)) {
            index = 1;
            set = 1;
            gCutsceneIndex = 0;
            gCutsceneFlags |= 2;
        }

    }
    
    if (!set) {
        if (checkDailyEventBit(2)) {
            
            if (checkDailyEventBit(3)) {
                gCutsceneIndex = 1;
            } else {
                gCutsceneIndex = 0;
                setDailyEventBit(3);
            }
            
            gCutsceneFlags |= 2;
            index = 2;
            set = 1;
        }
    }

    if (!set) {
        if (checkDailyEventBit(4)) {
            switch (D_801C4216) {         
                case 4:
                    tempHappiness = adjustValue(gHappiness, -5, 0xFF);
                    gHappiness += tempHappiness;
                    gCutsceneIndex = 0;
                    break;
                case 2:
                    tempHappiness = adjustValue(gHappiness, -10, 0xFF);
                    gHappiness += tempHappiness;
                    gCutsceneIndex = 1;
                    break;
                case 3:
                    tempHappiness = adjustValue(gHappiness, -10, 0xFF);
                    gHappiness += tempHappiness;
                    gCutsceneIndex = 2;
                    break;
            }
            index = 3;
            set = 1;
            gCutsceneFlags |= 2;
        }
    }

    if (!set) {
        if (checkDailyEventBit(0x1D) && !checkDailyEventBit(0x1E)) {
            index = 0x21;
            set = 1;
            gCutsceneIndex = 1;
            gCutsceneFlags |= 2;
        }
    }

    if (!set) {
        if (checkDailyEventBit(0x20)) {
            index = 0x22;
            set = 1;
            gCutsceneIndex = 1;
            gCutsceneFlags |= 2;
        }
    }

    if (!set) {
        if (checkDailyEventBit(8)) {
            gCutsceneIndex = func_800616CC(gHouseExtensionSelection);
            gCutsceneFlags |= 2;
            index = 4;
            set = 1;
        }
    }

    if (!set) {
        if (checkLifeEventBit(0xD) && checkDailyEventBit(0xA) && !checkDailyEventBit(0xC)) {
            gCutsceneIndex = 1;
            index = 5;
            set = 1;
        }
    }

    if (!set) {
        if (checkDailyEventBit(0xB)) {
            gCutsceneIndex = 0;
            index = 6;
            set = 1;
        }
    }

    if (!set) {
        func_8009C324(0);
    }
    
    return index;
}

//INCLUDE_ASM(const s32, "setCutscenes", setMountain1Cutscenes);

u16 setMountain1Cutscenes(void) {

    u8 set = 0;
    u16 index = 0xFFFF;

    if (!checkLifeEventBit(0x48) && gSeason == FALL && (gDayOfMonth - 23) < 5U && (gHour - 6) < 9U) {
        setDailyEventBit(0x21);
        if (checkDailyEventBit(0x2C)) {
            gCutsceneIndex = 0x145;
        } else {
            gCutsceneIndex = 0x144;
            gCutsceneFlags |= 6;
        }
        loadCutscene(0);
        set = 1;
    }

    if (!set) {
        if (!checkLifeEventBit(0x4C) && gSeason == WINTER && (gDayOfMonth - 12) < 5U && (gHour - 6) < 9U) {
            setDailyEventBit(0x21);
            if (checkDailyEventBit(0x2C)) {
                gCutsceneIndex = 0x145;
            } else {
                gCutsceneIndex = 0x148;
                gCutsceneFlags |= 6;
            }
            loadCutscene(0);
            set = 1;  
        }
    }

    if (!set) {
        if (!checkHaveTool(FISHING_POLE)) {
            setDailyEventBit(0x21);
            gCutsceneIndex = 0x13E;
            loadCutscene(0);
            set = 1;
        }
    }

    if (!set) {
        if (!checkLifeEventBit(0xA4) && npcAffection[JEFF] >= 0xB4 && gWeather == SUNNY && gSeason == SUMMER && DAYTIME &&  (!checkLifeEventBit(MARRIED) || gWife != ELLI) && !checkLifeEventBit(ELLI_JEFF_MARRIED) && gEntranceIndex == TOP_OF_MOUNTAIN_1_SUMMER) {
            setDailyEventBit(0x21);
            setLifeEventBit(0xA4);
            gCutsceneIndex = 0x134;
            gCutsceneFlags |= 6;
            loadCutscene(0);
            set = 1;
        }
    }

    if (!set) {
        if (!checkLifeEventBit(0xA6) && npcAffection[MARIA] >= 150 && npcAffection[ELLI] >= 150 && gWeather == SUNNY && gSeason == SUMMER && DAYTIME && gEntranceIndex == TOP_OF_MOUNTAIN_1_SUMMER) {
            setDailyEventBit(0x21);
            setLifeEventBit(0xA6);
            gCutsceneIndex = 0x136;
            gCutsceneFlags |= 6;
            loadCutscene(0);
            set = 1;
        }
    }

    if (!set) {
        if (!checkLifeEventBit(0xA9) && npcAffection[POPURI] >= 150 && npcAffection[KAREN] >= 150 && gWeather == SUNNY && gSeason == SPRING && DAYTIME && gEntranceIndex == 0x1A) {
            setDailyEventBit(0x21);
            setLifeEventBit(0xA9);
            gCutsceneIndex = 0x13A;
            gCutsceneFlags |= 6;
            loadCutscene(0);
            set = 1;
        }
    }

    if (!set) {
      if (!checkLifeEventBit(0xAD) && !checkDailyEventBit(0x30) && npcAffection[ELLI] >= 180 && gWeather == SUNNY && DAYTIME && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {
            setDailyEventBit(0x21);
            setLifeEventBit(0xAD);
            setDailyEventBit(0x58);
            gCutsceneIndex = ELLI_SPRAINED_ANKLE;
            gCutsceneFlags |= 6;
            loadCutscene(0);
        }
    } 
    
    return index;
}

INCLUDE_ASM(const s32, "setCutscenes", func_800A1C04);

INCLUDE_ASM(const s32, "setCutscenes", func_800A2250);

//INCLUDE_ASM(const s32, "setCutscenes", setMountain2Cutscenes);

// mountain 2
u16 setMountain2Cutscenes(void) {

    u8 set = 0;
    u16 index = 0xFFFF;

    if ((gSeason == SUMMER) && (gDayOfMonth == 1) && (NIGHTTIME) && NOT_MARRIED && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
        gCutsceneIndex = MOUNTAIN_FIREWORKS;
        loadCutscene(0);
        gCutsceneFlags |= 6;
        set = 1;
    }
    
    if (!set) {
        if ((gSeason == WINTER) && (gDayOfMonth == 24)  && (NIGHTTIME) && NOT_MARRIED && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
            gCutsceneIndex = 0x131;
            loadCutscene(0);
            gCutsceneFlags |= 6;
        }
    }
    
    return index;
}

//INCLUDE_ASM(const s32, "setCutscenes", setSpringCutscenes);

// spring
u16 setSpringCutscenes(void) {

    u8 set = 0;
    u16 index = 0xFFFF;

    if (!checkLifeEventBit(0x9F) && (blueMistFlowerPlot >= 0xC2) && (gSeason < 3) && DAYTIME) {
        setLifeEventBit(0x9F);
        setDailyEventBit(0x46);
        gCutsceneIndex = 0x13B;
        gCutsceneFlags |= 6;
        loadCutscene(0);
        set = 1;
    }

    if (!set) {
        if (!checkLifeEventBit(0xD5) && checkLifeEventBit(0x9F) && (blueMistFlowerPlot >= 0xC2) && (gSeason < FALL) && DAYTIME) {
            setLifeEventBit(0xD5);
            setDailyEventBit(0x46);
            gCutsceneIndex = 0x13C;
            gCutsceneFlags |= 6;
            loadCutscene(0);
            set = 1;
        }
    }

    if (!set) {
         if (!checkLifeEventBit(0xD6) && checkLifeEventBit(0xD5) && (blueMistFlowerPlot >= 0xC2) && (gSeason < FALL) && DAYTIME) {
            setLifeEventBit(0xD6);
            setDailyEventBit(0x46);
            gCutsceneIndex = 0x13D;
            gCutsceneFlags |= 6;
            loadCutscene(0);
         }
    }
    
    return index;
}

//INCLUDE_ASM(const s32, "setCutscenes", setHarvestSpriteCaveCutscenes);

u16 setHarvestSpriteCaveCutscenes(void) {

    u16 index = 0xFFFF;
    
    if (checkHaveKeyItem(MARBLES) && npcAffection[HARVEST_SPRITE_1] >= 80 && DAYTIME) {
        gCutsceneIndex = 0x142;
        gCutsceneFlags |= 6;
        loadCutscene(0);
    }

    return index;

}

//INCLUDE_ASM(const s32, "setCutscenes", setCaveCutscenes);

u16 setCaveCutscenes(void) {

    u16 index = 0xFFFF;
    
    if ((gSeason == WINTER) && ((gDayOfMonth - 8) < 23U) && (gHour - 6) < 10U) {
        gCutsceneIndex = 0x15A;
        gCutsceneFlags |= 2;
        loadCutscene(0);
    }

    return index;

}

// jtbl_80121048
INCLUDE_ASM(const s32, "setCutscenes", func_800A29B0);

INCLUDE_ASM(const s32, "setCutscenes", func_800A2FA8);

INCLUDE_ASM(const s32, "setCutscenes", func_800A3F04);

//INCLUDE_ASM(const s32, "setCutscenes", setRanchStoreCutscenes);

u16 setRanchStoreCutscenes(void) {

    u16 index = 0xFFFF;
    
    if (!(checkLifeEventBit(ANN_SICK_VISIT)) && (!checkDailyEventBit(0x30)) && (npcAffection[ANN] >= 120U) && RAINING && gDayOfWeek == SUNDAY && DAYTIME && (NOT_MARRIED) && (!checkLifeEventBit(ANN_CLIFF_MARRIED))) {
        setLifeEventBit(ANN_SICK_VISIT);
        setDailyEventBit(0x59);
        gCutsceneIndex = 0x29A;
        loadCutscene(0);
        gCutsceneFlags |= 6;
    }
    
    if (MARRIED_TO_ANN && checkLifeEventBit(WIFE_LEFT)) {
        gCutsceneIndex = 0x29B;
        loadCutscene(0);
        gCutsceneFlags |= 6;
    }

    return index;
}

INCLUDE_ASM(const s32, "setCutscenes", func_800A4878);

//INCLUDE_ASM(const s32, "setCutscenes", setVineyardHouseCutscenes);

u16 setVineyardHouseCutscenes(void) {
    
    u16 index = 0xFFFF;
    
    if (!checkLifeEventBit(KAREN_SICK_VISIT) && !checkDailyEventBit(0x30) && (npcAffection[KAREN] >= 120U) && RAINING && DAYTIME && NOT_MARRIED && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
        setLifeEventBit(KAREN_SICK_VISIT);
        setDailyEventBit(0x5A);
        gCutsceneIndex = KAREN_SICK;
        loadCutscene(0);
        gCutsceneFlags |= 6;
    }
    
    if (MARRIED_TO_KAREN && checkLifeEventBit(WIFE_LEFT)) {
        gCutsceneIndex = 0x2C8;
        loadCutscene(0);
        gCutsceneFlags |= 6;
    }
    
    return index;
}

//INCLUDE_ASM(const s32, "setCutscenes", setVineyardCellarCutscenes);

u16 setVineyardCellarCutscenes(void) {
    
    u8 set;
    u16 index = 0xFFFF;

    set = 0;
    
    if (!checkLifeEventBit(KAREN_YELLOW_HEART_EVENT_CUTSCENE) && checkLifeEventBit(0x67) && DAYTIME && NOT_MARRIED && !checkLifeEventBit(KAREN_KAI_MARRIED) && previousMapIndex == VINEYARD) {
               
        setLifeEventBit(KAREN_YELLOW_HEART_EVENT_CUTSCENE);
        setDailyEventBit(0x4F);

        gCutsceneIndex = KAREN_YELLOW_HEART_EVENT_1;
        gCutsceneFlags |= 6;

        loadCutscene(0);

        set = 1;

    }
    
    if (!set && checkDailyEventBit(0x4F) && NOT_MARRIED && !checkLifeEventBit(KAREN_KAI_MARRIED) && previousMapIndex == VINEYARD_CELLAR_BASEMENT) {
        gCutsceneIndex = KAREN_YELLOW_HEART_EVENT_4;
        gCutsceneFlags |= 6;
        loadCutscene(0);
    }
    
    return index;
}

INCLUDE_ASM(const s32, "setCutscenes", func_800A4E50);

INCLUDE_ASM(const s32, "setCutscenes", func_800A5314);

//INCLUDE_ASM(const s32, "setCutscenes", setRickStoreCutscenes);

u16 setRickStoreCutscenes(void) {
    
    u16 index = 0xFFFF;
    
    if (!checkLifeEventBit(ANN_RICK_CUTSCENE) && (npcAffection[RICK] >= 40) && DAYTIME) {
        setLifeEventBit(ANN_RICK_CUTSCENE);
        gCutsceneIndex = ANN_AND_RICK;
        gCutsceneFlags |= 6;
        loadCutscene(0);
    }
    
    return index;

}

//INCLUDE_ASM(const s32, "setCutscenes", setFlowerShopCutscenes);

u16 setFlowerShopCutscenes(void) {
    
    u16 index = 0xFFFF;
    
    if (!checkLifeEventBit(POPURI_SICK_CUTSCENE) && !checkDailyEventBit(0x30) && npcAffection[POPURI] >= 120 && RAINING && DAYTIME && NOT_MARRIED && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
        setLifeEventBit(POPURI_SICK_CUTSCENE);
        setDailyEventBit(0x57);
        gCutsceneIndex = POPURI_SICK;
        loadCutscene(0);
        gCutsceneFlags |= 6;
    }
    
    if (MARRIED_TO_POPURI && checkLifeEventBit(WIFE_LEFT)) {
        gCutsceneIndex = 0x1BC;
        loadCutscene(0);
        gCutsceneFlags |= 6;
    }
    
    return index;

}

INCLUDE_ASM(const s32, "setCutscenes", func_800A5F48);

INCLUDE_ASM(const s32, "setCutscenes", func_800A6440);

INCLUDE_ASM(const s32, "setCutscenes", func_800A6634);

//INCLUDE_ASM(const s32, "setCutscenes", setChurchCutscenes);

u16 setChurchCutscenes(void) {

    u8 set = 0;
    u16 index = 0xFFFF;

    if (gSeason == WINTER && gDayOfMonth == 24 && NIGHTTIME && NOT_MARRIED) {     
        gCutsceneIndex = STARRY_NIGHT_FESTIVAL;
        loadCutscene(0);
        set = 1;
        gCutsceneFlags |= 6;
    } 
    
    if (!set) {
        if (gSeason == WINTER && gDayOfMonth == 30 && NIGHTTIME) {
            gCutsceneIndex = CHURCH_NEW_YEARS;
            loadCutscene(0);
            gCutsceneFlags |= 6;
        }    
    } 
    
    return index;
}

INCLUDE_ASM(const s32, "setCutscenes", func_800A68C0);

INCLUDE_ASM(const s32, "setCutscenes", func_800A6A14);

INCLUDE_ASM(const s32, "setCutscenes", func_800A6EE4);

//INCLUDE_ASM(const s32, "setCutscenes", setBeachCutscenes);

u16 setBeachCutscenes(void) {
    
    u8 set = 0;
    u16 index = 0xFFFF;

    if (gSeason == SUMMER && gDayOfMonth == 17 && NIGHTTIME) {
        gCutsceneIndex = FIREFLY_FESTIVAL;
        loadCutscene(0);
        set = 1;
        gCutsceneFlags |= 6;
    }

    if (!set) {
        if (gSeason == SUMMER && gDayOfMonth == 24 && DAYTIME) {
            gCutsceneIndex = SEA_FESTIVAL;
            loadCutscene(0);
            set = 1;
            gCutsceneFlags |= 6;
        }
    }
    
    if (!set) {
        if (gSeason == SUMMER && gDayOfMonth == 1 && NIGHTTIME && NOT_MARRIED && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            gCutsceneIndex = BEACH_FIREWORKS;
            loadCutscene(0);
            set = 1;
            gCutsceneFlags |= 6;
        }
    }
    
    if (!set) {
        if (!checkDailyEventBit(0x1C) && gSeason == WINTER && gDayOfMonth == 10 && DAYTIME && NOT_MARRIED) {
            setDailyEventBit(0x1C);
            gCutsceneIndex = 0x2F1;
            loadCutscene(0);
            gCutsceneFlags |= 6;
            set = 1;
        }
    }

    if (!set) {
        if (gSeason == WINTER && gDayOfMonth == 24 && NIGHTTIME && NOT_MARRIED && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            gCutsceneIndex = 0x2F4;
            loadCutscene(0);
            gCutsceneFlags |= 6;
            set = 1;
        }
    }
    
    if (!set) {
        if (!checkLifeEventBit(0xCA) && npcAffection[KAREN] < 101 && DAYTIME && (gYear >= 3 || (gYear == 2 && gSeason == WINTER)) && (NOT_MARRIED || gWife != KAREN) && !checkLifeEventBit(KAREN_KAI_MARRIED) && gFlowerFestivalGoddess != 4) {
            setLifeEventBit(0xCA);
            setLifeEventBit(0x2F);
            setSpecialDialogueBit(0x23);
            gCutsceneIndex = 0x2F0;
            gCutsceneFlags |= 6;
            loadCutscene(0);
            set = 1;
        }
    }

    if (!set) {
        if (!checkLifeEventBit(0xCB) && npcAffection[ELLI] >= 150 && npcAffection[KAREN] >= 150 && gWeather == SUNNY && gSeason == SUMMER && DAYTIME && (NOT_MARRIED || gWife != KAREN) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            setLifeEventBit(0xCB);
            gCutsceneIndex = KAREN_AND_ELLI;
            gCutsceneFlags |= 6;
            loadCutscene(0);
            set = 1;
        }
    }

    if (!set) {
        if (!checkLifeEventBit(0x9C) && NOT_MARRIED && npcAffection[KAREN] >= 250 && gWeather == SUNNY && (gHour - 15) < 9U && NOT_MARRIED && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            setLifeEventBit(0x9C);
            gCutsceneIndex = KAREN_PINK_HEART_EVENT;
            gCutsceneFlags |= 6;
            loadCutscene(0);     
            set = 1;
        }
    }

    if (!set) {
        if (!checkLifeEventBit(0x9D) && checkLifeEventBit(0x20) && npcAffection[KAI] >= 180 && gWeather == SUNNY && NIGHTTIME && (NOT_MARRIED || gWife != KAREN) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            setLifeEventBit(0x9D);
            gCutsceneIndex = 0x2EF;
            gCutsceneFlags |= 6;
            loadCutscene(0);
        }
    }
    
    return index;
}

//INCLUDE_ASM(const s32, "setCutscenes", setRoadCutscenes);

u16 setRoadCutscenes(void) {
    
    u8 set = 0;
    u16 index = 0xFFFF;

    if (!checkLifeEventBit(0xCC) && npcAffection[CLIFF] >= 200 && gSeason == WINTER && DAYTIME && (!checkLifeEventBit(0) || gWife != 3) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && previousMapIndex == 0x52) {
        setLifeEventBit(0xCC);
        gCutsceneIndex = 0x320;
        gCutsceneFlags |= 6;
        loadCutscene(0);
        set = 1;
    }

    if (!set) {
        if (!checkLifeEventBit(0xCD) && D_80181B10 >= 6 && npcAffection[ANN] >= 150 && DAYTIME && NOT_MARRIED && !checkLifeEventBit(ANN_CLIFF_MARRIED) && previousMapIndex == RANCH) {
            setLifeEventBit(0xCD);
            gCutsceneIndex = 0x321;
            gCutsceneFlags |= 6;
            loadCutscene(0);
            set = 1;
        }    
    }

    if (!set) {
        if (!checkLifeEventBit(SHADY_SALESMAN_MARIA_CUTSCENE) && checkLifeEventBit(0xB6) && DAYTIME) {

            if (previousMapIndex == VILLAGE_1) {
                setLifeEventBit(SHADY_SALESMAN_MARIA_CUTSCENE);
                gCutsceneIndex = SHADY_SALESMAN_AND_MARIA_1;
                gCutsceneFlags |= 6;
                loadCutscene(0);
                set = 1;
            }

            if (previousMapIndex == VILLAGE_2) {
                setLifeEventBit(SHADY_SALESMAN_MARIA_CUTSCENE);
                gCutsceneIndex = SHADY_SALESMAN_AND_MARIA_2;
                gCutsceneFlags |= 6;
                loadCutscene(0);
                set = 1;
            }
        }
    }

    if (!set) {
        if (!checkLifeEventBit(ANN_SPRAINED_ANKLE_CUTSCENE) && !checkDailyEventBit(0x30) && npcAffection[ANN] >= 180 && DAYTIME && NOT_MARRIED && !checkLifeEventBit(ANN_CLIFF_MARRIED) && previousMapIndex == FARM) {
            setLifeEventBit(ANN_SPRAINED_ANKLE_CUTSCENE);
            setDailyEventBit(0x59);
            gCutsceneIndex = ANN_SPRAINED_ANKLE;
            loadCutscene(0);
            gCutsceneFlags |= 6; 
        }
    }
    
    return index;

}

//INCLUDE_ASM(const s32, "setCutscenes", func_800A7AE8);

u16 func_800A7AE8(void) {

    u16 index = 0xFFFF;
    u8 set = 0;
    
    if (!(gCutsceneFlags & 1)) {
        
        gCutsceneFlags &= ~2;
    
        if (gHour >= 17 && !checkDailyEventBit(0)) {

            if (dailyShippingBinValue) {
                gCutsceneIndex = 0;
            } else {
                gCutsceneIndex = 1;
            }

            func_80060E58();

            if (gBaseMapIndex == FARM) {
                index = 0;
                set = 1;
            }       

        }
        
        if (!set) {

            if (checkLifeEventBit(0xD)) {

                if (gHour >= 8 && !checkDailyEventBit(0xA) && !checkDailyEventBit(0xC)) {
                    
                    setDailyEventBit(0xA);
                    
                    if (gBaseMapIndex == FARM) {
                        gCutsceneIndex = 0;
                        index = 5;
                        set = 1;
                    }
                
                }

                if (gHour >= 0xF) {
                    if (!checkDailyEventBit(0xC)) {
                        setDailyEventBit(0xC);
                    }
                }
            }   
        }
        
        if (!set) {
            
            if (!checkDailyEventBit(0x1D) && !checkDailyEventBit(0x1E) && gSeason == FALL && gDayOfMonth == 3 && (gHour - 6) < 9U && func_8009B2BC(2)) {
               
                setDailyEventBit(0x1D);
                
                if (gBaseMapIndex == FARM) {
                    index = 0x21;
                    set = 1;
                    gCutsceneFlags |= 2;
                }
                
                if (gHour >= 16) {
                    if (!checkDailyEventBit(0x1E)) {
                        setDailyEventBit(0x1E);
                    }
                }
            }
        }
        
        if (!set) {
            if (gBaseMapIndex == FARM) {
                func_8009C324(1);
            }
        }
        
        if (index != 0xFFFF) {
            nuPiReadRom(D_80114AF0[index].romAddr, D_80114D30[index], D_80114AF0[index].vaddr - D_80114AF0[index].romAddr);
            D_801891D4 = 0;
            func_800469A8(D_80114DF0[index], D_80114C70[index]);
            func_8002F730();
            D_8018981C = index;
            gCutsceneFlags |= 1;
            toggleDailyEventBit(0x53);
        }
    }

    return index;
    
}

// jtbl_80121060
// jtbl_80121078
INCLUDE_ASM(const s32, "setCutscenes", func_800A7DFC);

INCLUDE_ASM(const s32, "setCutscenes", func_800A87C4);

// jtbl_80121090
// jtbl_80121120
// jtbl_80121138
// jtbl_80121208
// jtbl_801212B8
// jtbl_801212F8
// jtbl_801213A0
// jtbl_801213B8
// jtbl_801213D0
// jtbl_801213F0
INCLUDE_ASM(const s32, "setCutscenes", func_800A8F74);

INCLUDE_ASM(const s32, "setCutscenes", loadCutscene);
