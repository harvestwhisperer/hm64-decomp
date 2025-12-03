#include "common.h"

#include "game/setCutscenes.h"

#include "system/cutscene.h"
#include "system/entity.h"
#include "system/mapController.h"
#include "system/math.h"
#include "system/message.h"

#include "game/animals.h"
#include "game/evaluation.h"
#include "game/game.h"
#include "game/gameStatus.h"
#include "game/itemHandlers.h"
#include "game/level.h"
#include "game/fieldObjects.h"
#include "game/npc.h"
#include "game/overlayScreens.h"
#include "game/player.h"
#include "game/spriteInfo.h"
#include "game/weather.h"
                                  
// forward declarations
u16 setBeachCutscenes(void);                         
u16 setCaveCutscenes(void);                          
u16 setChurchCutscenes(void);                        
u16 setFarmCutscenes(void);
u16 setFlowerShopCutscenes(void);                    
u16 setHarvestSpriteCaveCutscenes(void);             
u16 setMoonMountainCutscenes(void);                     
u16 setMountain1Cutscenes(void);
u16 setRanchStoreCutscenes(void);                    
u16 setRickStoreCutscenes(void);                     
u16 setRoadCutscenes(void);                          
u16 setSpringCutscenes(void);                        
u16 setVineyardCellarCutscenes(void);                
u16 setVineyardHouseCutscenes(void); 
// TODO: label
u16 func_800A1C04(void);
u16 func_800A2250(void);
u16 func_800A29B0(void);
u16 func_800A2FA8(void);
u16 func_800A3F04(void);
u16 func_800A4878(void);
u16 func_800A4E50(void);
u16 func_800A5314(void);
u16 func_800A5F48(void);
u16 func_800A6440(void);
u16 func_800A6634(void);
u16 func_800A68C0(void);
u16 func_800A6A14(void);
u16 func_800A6EE4(void);

// bss
extern s32 gCutsceneCompletionFlags;
extern u16 D_8018981C;

// likely bss
extern u16 gCutsceneIndex;

// shared bss
// bakery points
extern u8 bakeryCardPoints;
extern u8 D_801C4216;
// chicken feed count
extern u16 chickenFeedQuantity;

// evaluation fish caught score
extern u16 D_801D62C4;
// evaluation: animals
extern u8 D_801D62C6;

// field tile data
extern u8 D_80113580[FIELD_HEIGHT][FIELD_WIDTH];
extern u8 D_80114E50[FIELD_HEIGHT][FIELD_WIDTH];
 
// data 
Addresses cutsceneBytecodeAddresses[] = {
      { 0xFF5C0, 0x100180 },   
      { 0xFF5C0, 0x100180 },   
      { 0xFF5C0, 0x100180 },   
      { 0xFF5C0, 0x100180 },   
      { 0xFF5C0, 0x100180 },   
      { 0xFF5C0, 0x100180 },   
      { 0xFF5C0, 0x100180 },   
      { 0x100180, 0x109920 },  
      { 0x100180, 0x109920 },  
      { 0x109920, 0x10EA80 },  
      { 0x10EA80, 0x10F980 },  
      { 0x10F980, 0x115310 },  
      { 0x115310, 0x1175F0 },  
      { 0x1175F0, 0x118D70 },  
      { 0x118D70, 0x120910 },  
      { 0x120910, 0x123CE0 },  
      { 0x123CE0, 0x125200 },  
      { 0x125200, 0x129760 },  
      { 0x129760, 0x12FE90 },  
      { 0x12FE90, 0x134510 },  
      { 0x134510, 0x135F40 },  
      { 0x135F40, 0x136930 },  
      { 0x136930, 0x1385E0 },  
      { 0x1385E0, 0x139840 },  
      { 0x139840, 0x13AFC0 },  
      { 0x13AFC0, 0x13D470 },  
      { 0x13D470, 0x13FAC0 },  
      { 0x13FAC0, 0x144F60 },  
      { 0x144F60, 0x1485B0 },  
      { 0x1485B0, 0x149CC0 },  
      { 0x149CC0, 0x14B8F0 },  
      { 0x14B8F0, 0x150120 },  
      { 0x150120, 0x1510B0 },  
      { 0xFF5C0, 0x100180 },   
      { 0xFF5C0, 0x100180 },   
      { 0x1510B0, 0x159330 },   
      { 0, 0 },     
      { 0, 0 },    
      { 0, 0 },    
      { 0, 0 },    
      { 0, 0 },    
      { 0, 0 },    
      { 0, 0 },    
      { 0, 0 },    
      { 0, 0 },    
      { 0, 0 },    
      { 0, 0 },    
      { 0, 0 }
};

// vaddr of cutscene bytecode + offset
void *cutsceneEntryPointAddresses[] = {
    0x802F4004,
    0x802F4010,
    0x802F401C,
    0x802F4028,
    0x802F4088,
    0x802F4094,
    0x802F40B0,
    0x802F8E6C,
    0x802F90F8,
    0x802F5004,
    0x802F5004,
    0x802F5004,
    0x802F5004,
    0x802F5004,
    0x802F500C,
    0x802F5004,
    0x802F5004,
    0x802F5008,
    0x802F5018,
    0x802F5008,
    0x802F5008,
    0x802F5000,
    0x802F5008,
    0x802F5010,
    0x802F5008,
    0x802F5008,
    0x802F5008,
    0x802F5014,
    0x802F5004,
    0x802F5004,
    0x802F5000,
    0x802F5004,
    0x802F5004,
    0x802F40CC,
    0x802F40D8,
    0x802F5004,
    0,0,0,0,0,0,0,0,0,0,0,0
};

// vaddrs of entire cutscene bytecode banks
void *cutsceneBankLoadAddresses[] = {
    0x802F4000,
    0x802F4000,
    0x802F4000,
    0x802F4000,
    0x802F4000,
    0x802F4000,
    0x802F4000,
    0x802F5000,
    0x802F5000,
    0x802F5000,
    0x802F5000,
    0x802F5000,
    0x802F5000,
    0x802F5000,
    0x802F5000,
    0x802F5000,
    0x802F5000,
    0x802F5000,
    0x802F5000,
    0x802F5000,
    0x802F5000,
    0x802F5000,
    0x802F5000,
    0x802F5000,
    0x802F5000,
    0x802F5000,
    0x802F5000,
    0x802F5000,
    0x802F5000,
    0x802F5000,
    0x802F5000,
    0x802F5000,
    0x802F5000,
    0x802F4000,
    0x802F4000,
    0x802F5000,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};

u16 cutsceneExecutorIndices[] = {
    32,
    33,
    33,
    33,
    34,
    34,
    34,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    33,
    33,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0
};

//INCLUDE_ASM("asm/nonmatchings/game/setCutscenes", func_8009BF90);

// sets initial cutscene executor object
inline void func_8009BF90(u16 arg0) {

    nuPiReadRom(cutsceneBytecodeAddresses[arg0].romAddrStart, cutsceneBankLoadAddresses[arg0], cutsceneBytecodeAddresses[arg0].romAddrEnd - cutsceneBytecodeAddresses[arg0].romAddrStart);

    gCutsceneCompletionFlags = 0;

    // param 1 = 0, 20, 21, or 22
    spawnCutsceneExecutor(cutsceneExecutorIndices[arg0], cutsceneEntryPointAddresses[arg0]);
    
    togglePauseEntities();
    
    D_8018981C = arg0;

    gCutsceneFlags |= 1;

    toggleDailyEventBit(0x53);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/setCutscenes", func_8009C054);

u16 func_8009C054(u16 mapIndex) {
    
    u16 offset = 0xFFFF;
    
    // can't use array/struct here as required by func_80056030
    if (!(gCutsceneFlags & 1)) {
        
        gCutsceneFlags &= ~2;

        switch (mapIndex) {
            case FARM:
                offset = setFarmCutscenes();
                break;
            case MOUNTAIN_1:
                offset = setMountain1Cutscenes();
                break;
            case MOUNTAIN_2:
                offset = func_800A1C04();
                break;
            case TOP_OF_MOUNTAIN_1:
                offset = func_800A2250();
                break;
            case MOON_MOUNTAIN:
                offset = setMoonMountainCutscenes();
                break;
            case POND:
                offset = setSpringCutscenes();
                break;
            case HARVEST_SPRITE_CAVE:
                offset = setHarvestSpriteCaveCutscenes();
                break;
            case CAVE:
                offset = setCaveCutscenes();
                break;
            case HOUSE:
                offset = func_800A29B0();
                break;
            case KITCHEN:
                offset = func_800A2FA8();
                break;
            case RANCH:
                offset = func_800A3F04();
                break;
            case RANCH_STORE:
                offset = setRanchStoreCutscenes();
                break;
            case VINEYARD:
                offset = func_800A4878();
                break;
            case VINEYARD_HOUSE:
                offset = setVineyardHouseCutscenes();
                break;
            case VINEYARD_CELLAR:
                offset = setVineyardCellarCutscenes();
                break;
            case VILLAGE_1:
                offset = func_800A4E50();
                break;
            case VILLAGE_2:
                offset = func_800A5314();
                break;
            case RICK_STORE:
                offset = setRickStoreCutscenes();
                break;
            case FLOWER_SHOP:
                offset = setFlowerShopCutscenes();
                break;
            case BAKERY:
                offset = func_800A5F48();
                break;
            case MAYOR_HOUSE:
                offset = func_800A6440();
                break;
            case LIBRARY:
                offset = func_800A6634();
                break;
            case CHURCH:
                offset = setChurchCutscenes();
                break;
            case TAVERN:
                offset = func_800A68C0();
                break;
            case SQUARE:
                offset = func_800A6A14();
                break;
            case RACE_TRACK:
                offset = func_800A6EE4();
                break;
            case BEACH:
                offset = setBeachCutscenes();
                break;
            case ROAD:
                offset = setRoadCutscenes();
                break;
            default:
                break;
            }
        
        if (offset != 0xFFFF) {
            func_8009BF90(offset);
        }
        
    }
    
    return offset;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/setCutscenes", func_8009C324);

// arg0 = cutscene type; 0 = show up while working; 1 = morning/show up on map entry
u16 func_8009C324(u8 arg0) {

    bool set = FALSE;
    u16 result = 0xFFFF;
    u8 tempGirl;
    
    if (!checkLifeEventBit(MAYOR_TOUR)) {
        setLifeEventBit(MAYOR_TOUR);
        gCutsceneIndex = MAYOR_VILLAGE_TOUR;
        gCutsceneFlags |= 2;
        loadCutscene(arg0);
        set = TRUE;
    }

    if (gEntranceIndex != 0x6F) {
        
        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gYear == 1 && gSeason == AUTUMN && gDayOfMonth == 22 && DAYTIME) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = 0xDF;
                loadCutscene(arg0);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gYear == 1 && gSeason == WINTER && gDayOfMonth == 11 && DAYTIME) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = 0xDE;
                loadCutscene(arg0);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == SPRING && gDayOfMonth == 16 && (5 < gHour && gHour < 15) && (horseInfo.flags & 1) && horseInfo.grown == TRUE) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = DOUG_FARM_HORSE_RACE;
                loadCutscene(arg0);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == AUTUMN && gDayOfMonth == 27 && (5 < gHour && gHour < 15) && (horseInfo.flags & 1) && horseInfo.grown == TRUE) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = 0x7E;
                loadCutscene(arg0);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == WINTER && gDayOfMonth == 18 && (5 < gHour && gHour < 15)) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = DOUG_FARM_DOG_RACE;
                loadCutscene(arg0);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == SPRING && gDayOfMonth == 7 && (5 < gHour && gHour < 9)) {
                
                setDailyEventBit(FARM_MORNING_VISIT);
                
                if (npcAffection[KENT] < 30) {
                    if (gYear == 1) {
                        gCutsceneIndex = MAYOR_FARM_SOWING_FESTIVAL_YEAR_1;
                    } else {
                        gCutsceneIndex = MAYOR_FARM_SOWING_FESTIVAL;
                    }
                } else {
                    gCutsceneIndex = KENT_FARM_SOWING_FESITVAL;
                }
                
                loadCutscene(arg0);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == AUTUMN && gDayOfMonth == 2 && (5 < gHour && gHour < 9) && npcAffection[KENT] >= 30) {
                setDailyEventBit(FARM_MORNING_VISIT);
                
                if (func_8009B2BC(2)) {
                    gCutsceneIndex = 0x66;
                    setGameVariableString(0xE, gFarmAnimals[func_8009B464()].name, 6);
                } else {
                    gCutsceneIndex = 0x65;
                }
                
                loadCutscene(arg0);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == WINTER && gDayOfMonth == 9 && (5 < gHour && gHour < 9) && npcAffection[KENT] >= 30) {
                setDailyEventBit(FARM_MORNING_VISIT);
                
                if (gYear == 1) {
                    gCutsceneIndex = KENT_FARM_THANKSGIVING_FESTIVAL;
                } else {
                    gCutsceneIndex = 0x68;
                }
                
                loadCutscene(arg0);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == SPRING && gDayOfMonth == 16 && (5 < gHour && gHour < 9)) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = MAYOR_FARM_HORSE_RACE;
                loadCutscene(arg0);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == SUMMER && gDayOfMonth == 17 && (5 < gHour && gHour < 9)) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = MAYOR_FARM_FIREFLY_FESTIVAL;
                loadCutscene(arg0);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == SUMMER && gDayOfMonth == 23 && (5 < gHour && gHour < 9)) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = MAYOR_FARM_SEA_FESTIVAL;
                loadCutscene(arg0);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == AUTUMN && gDayOfMonth == 11 && (5 < gHour && gHour < 9)) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = MAYOR_FARM_HARVEST_FESTIVAL;
                loadCutscene(arg0);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == AUTUMN && gDayOfMonth == 19 && (5 < gHour && gHour < 9)) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = MAYOR_FARM_EGG_FESTIVAL;
                loadCutscene(arg0);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == WINTER && gDayOfMonth == 27 && (5 < gHour && gHour < 9)) {
                
                setDailyEventBit(FARM_MORNING_VISIT);
                
                if (gHarvestKing == PLAYER) {
                    gCutsceneIndex = MAYOR_FARM_SPIRIT_FESTIVAL_PLAYER_HARVEST_KING;
                } else {
                    gCutsceneIndex = MAYOR_FARM_SPIRIT_FESTIVAL;
                }
                
                loadCutscene(arg0);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == SPRING && gDayOfMonth == 19 && (5 < gHour && gHour < 9)) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = BASIL_FARM_FLOWER_FESTIVAL;
                loadCutscene(arg0);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == SUMMER && gDayOfMonth == 8 && (5 < gHour && gHour < 9)) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = BASIL_FARM_VEGETABLE_FESTIVAL;
                loadCutscene(arg0);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(POPURI_MARRIAGE_CUTSCENE) && npcAffection[POPURI] >= 50 && gSeason == SPRING && gDayOfMonth == 22 && (5 < gHour && gHour < 9) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
                setDailyEventBit(POPURI_MARRIAGE_CUTSCENE);
                gCutsceneIndex = 0x9C;
                loadCutscene(arg0);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(ELLI_MARRIAGE_CUTSCENE) && npcAffection[ELLI] >= 50 && gSeason == AUTUMN && gDayOfMonth == 11 && (6 < gHour && gHour < 9) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {
                setDailyEventBit(ELLI_MARRIAGE_CUTSCENE);
                gCutsceneIndex = 0xA8;
                loadCutscene(arg0);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(ANN_MARRIAGE_CUTSCENE) && npcAffection[ANN] >= 80 && gSeason == SPRING && gDayOfMonth == 16 && (6 < gHour && gHour < 9) && !checkLifeEventBit(MARRIED) && checkLifeEventBit(0x51) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
                setDailyEventBit(ANN_MARRIAGE_CUTSCENE);
                gCutsceneIndex = 0xB3;
                loadCutscene(arg0);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(MARIA_MARRIAGE_CUTSCENE) && npcAffection[MARIA] >= 50 && gSeason == SUMMER && gDayOfMonth == 1 && gHour == 8 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
                setDailyEventBit(MARIA_MARRIAGE_CUTSCENE);
                gCutsceneIndex = MARIA_FIREWORKS_INVITATION;
                loadCutscene(arg0);
                set = TRUE;
            }            
        }
        
        if (!set) {
            if (!checkDailyEventBit(POPURI_MARRIAGE_CUTSCENE) && npcAffection[POPURI] >= 50 && gSeason == SUMMER && gDayOfMonth == 1 && gHour == 10 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
                setDailyEventBit(POPURI_MARRIAGE_CUTSCENE);
                gCutsceneIndex = POPURI_FIREWORKS_INVITATION;
                loadCutscene(arg0);
                set = TRUE;
            }            
        }

        if (!set) {
            if (!checkDailyEventBit(ELLI_MARRIAGE_CUTSCENE) && npcAffection[ELLI] >= 50 && gSeason == SUMMER && gDayOfMonth == 1 && gHour == 12 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {
                setDailyEventBit(ELLI_MARRIAGE_CUTSCENE);
                gCutsceneIndex = ELLI_FIREWORKS_INVITATION;
                loadCutscene(arg0);
                set = TRUE;
            }            
        }

        if (!set) {
            if (!checkDailyEventBit(ANN_MARRIAGE_CUTSCENE) && npcAffection[ANN] >= 50 && gSeason == SUMMER && gDayOfMonth == 1 && gHour == 14 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
                setDailyEventBit(ANN_MARRIAGE_CUTSCENE);
                gCutsceneIndex = ANN_FIREWORKS_INVITATION;
                loadCutscene(arg0);
                set = TRUE;
            }            
        }

        if (!set) {
            if (!checkDailyEventBit(KAREN_MARRIAGE_CUTSCENE) && npcAffection[KAREN] >= 50 && gSeason == SUMMER && gDayOfMonth == 1 && gHour == 16 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
                setDailyEventBit(KAREN_MARRIAGE_CUTSCENE);
                gCutsceneIndex = KAREN_FIREWORKS_INVITATION;
                loadCutscene(arg0);
                set = TRUE;
            }            
        }
        
        if (!set) {
            if (!checkDailyEventBit(MARIA_MARRIAGE_CUTSCENE) && npcAffection[MARIA] >= 140 && gSeason == WINTER && gDayOfMonth == 10 && gHour == 8 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
                setDailyEventBit(MARIA_MARRIAGE_CUTSCENE);
                gCutsceneIndex = MARIA_THANKSGIVING_GIFT;
                loadCutscene(arg0);
                set = TRUE;
            }            
        }        

        if (!set) {
            if (!checkDailyEventBit(POPURI_MARRIAGE_CUTSCENE) && npcAffection[POPURI] >= 140 && gSeason == WINTER && gDayOfMonth == 10 && gHour == 10 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
                setDailyEventBit(POPURI_MARRIAGE_CUTSCENE);
                gCutsceneIndex = POPURI_THANKSGIVING_GIFT;
                loadCutscene(arg0);
                set = TRUE;
            }            
        }        

        if (!set) {
            if (!checkDailyEventBit(ELLI_MARRIAGE_CUTSCENE) && npcAffection[ELLI] >= 140 && gSeason == WINTER && gDayOfMonth == 10 && gHour == 12 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {
                setDailyEventBit(ELLI_MARRIAGE_CUTSCENE);
                gCutsceneIndex = ELLI_THANKSGIVING_GIFT;
                loadCutscene(arg0);
                set = TRUE;
            }            
        }        

        if (!set) {
            if (!checkDailyEventBit(ANN_MARRIAGE_CUTSCENE) && npcAffection[ANN] >= 140 && gSeason == WINTER && gDayOfMonth == 10 && gHour == 14 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
                setDailyEventBit(ANN_MARRIAGE_CUTSCENE);
                gCutsceneIndex = ANN_THANKSGIVING_GIFT;
                loadCutscene(arg0);
                set = TRUE;
            }            
        }    

        if (!set) {
            if (!checkDailyEventBit(KAREN_MARRIAGE_CUTSCENE) && npcAffection[KAREN] >= 140 && gSeason == WINTER && gDayOfMonth == 10 && gHour == 16 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
                setDailyEventBit(KAREN_MARRIAGE_CUTSCENE);
                gCutsceneIndex = KAREN_THANKSGIVING_GIFT;
                loadCutscene(arg0);
                set = TRUE;
            }            
        }       

        if (!set) {
            if (!checkDailyEventBit(MARIA_MARRIAGE_CUTSCENE) && npcAffection[MARIA] >= 100 && gSeason == WINTER && gDayOfMonth == 24 && gHour == 8 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
                setDailyEventBit(MARIA_MARRIAGE_CUTSCENE);
                gCutsceneIndex = 0x95;
                loadCutscene(arg0);
                set = TRUE;
            }            
        }        

        if (!set) {
            if (!checkDailyEventBit(POPURI_MARRIAGE_CUTSCENE) && npcAffection[POPURI] >= 100 && gSeason == WINTER && gDayOfMonth == 24 && gHour == 10 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
                setDailyEventBit(POPURI_MARRIAGE_CUTSCENE);
                gCutsceneIndex = 0xA0;
                loadCutscene(arg0);
                set = TRUE;
            }            
        }        

        if (!set) {
            if (!checkDailyEventBit(ELLI_MARRIAGE_CUTSCENE) && npcAffection[ELLI] >= 100 && gSeason == WINTER && gDayOfMonth == 24 && gHour == 12 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {
                setDailyEventBit(ELLI_MARRIAGE_CUTSCENE);
                gCutsceneIndex = 0xAC;
                loadCutscene(arg0);
                set = TRUE;
            }            
        }       

        if (!set) {
            if (!checkDailyEventBit(ANN_MARRIAGE_CUTSCENE) && npcAffection[ANN] >= 100 && gSeason == WINTER && gDayOfMonth == 24 && gHour == 14 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
                setDailyEventBit(ANN_MARRIAGE_CUTSCENE);
                gCutsceneIndex = 0xB7;
                loadCutscene(arg0);
                set = TRUE;
            }            
        }       

        if (!set) {
            if (!checkDailyEventBit(KAREN_MARRIAGE_CUTSCENE) && npcAffection[KAREN] >= 100 && gSeason == WINTER && gDayOfMonth == 24 && gHour == 16 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
                setDailyEventBit(KAREN_MARRIAGE_CUTSCENE);
                gCutsceneIndex = 0xC4;
                loadCutscene(arg0);
                set = TRUE;
            }            
        }       

       if (!set) {
           
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == gPlayerBirthdaySeason && gDayOfMonth == 25 && DAYTIME) {
                
                tempGirl = getBacholeretteWithHighestAffection(120);
                
                if (tempGirl) {       

                    setDailyEventBit(FARM_MORNING_VISIT);
                    
                    switch (tempGirl) {
                        
                        case 1:
                            
                            if (!checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
                                
                                if (npcAffection[MARIA] >= 160) {
                                    setLifeEventBit(0x3F);
                                    gCutsceneIndex = 0x99;
                                } else {
                                    gCutsceneIndex = 0x98;
                                }
                                
                                gCutsceneFlags |= 2;
                                loadCutscene(arg0);
                                set = TRUE;
                            }

                            break;
                        
                        case 2:
                            
                            if (!checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
                                
                               if (npcAffection[POPURI] >= 160) {
                                    acquireKeyItem(POTPURRI);
                                    gCutsceneIndex = 0xA4;
                                } else {
                                    gCutsceneIndex = 0xA3;
                                }
                                
                                gCutsceneFlags |= 2;
                                loadCutscene(arg0);
                                set = TRUE;                                
                            }

                            break;

                        case 3:

                            if (!checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {
                                
                               if (npcAffection[ELLI] >= 160) {
                                    acquireKeyItem(EMBROIDERED_HANDKERCHIEF);
                                    gCutsceneIndex = 0xB0;
                                } else {
                                    gCutsceneIndex = 0xAF;
                                }
                                
                                gCutsceneFlags |= 2;
                                loadCutscene(arg0);
                                set = TRUE;                                
                            }

                            break;

                        case 4:

                            if (!checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
                                
                               if (npcAffection[ANN] >= 160) {
                                    acquireKeyItem(HAND_KNIT_SOCKS);
                                    gCutsceneIndex = 0xBB;
                                } else {
                                    gCutsceneIndex = 0xBA;
                                }
                                
                                gCutsceneFlags |= 2;
                                loadCutscene(arg0);
                                set = TRUE;                                
                            }

                            break;   

                        case 5:

                            if (!checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
                                
                               if (npcAffection[KAREN] >= 160) {
                                    acquireKeyItem(LUCKY_BRACELET);
                                    gCutsceneIndex = 0xC8;
                                } else {
                                    gCutsceneIndex = 0xC7;
                                }
                                
                                gCutsceneFlags |= 2;
                                loadCutscene(arg0);
                                set = TRUE;                                
                            }

                            break;

                        default:
                            break; 
                    }
                }
            }            
        }      

        if (!set) {
            if (!checkLifeEventBit(BASIL_IN_TOWN) && ((gSeason == SPRING && gDayOfMonth >= 15) || gSeason == SUMMER) && (5 < gHour && gHour < 17)) {
                
                setLifeEventBit(BASIL_IN_TOWN);
                toggleLifeEventBit(BASIL_GONE);       
                
                if (gYear != 1) {
                    if (npcAffection[BASIL] >= 150) {
                        gCutsceneIndex = BASIL_POWER_NUT_GIFT;
                    } else {
                        gCutsceneIndex = BASIL_RETURN;
                    }
                } else {
                    gCutsceneIndex = BASIL_INTRO;
                }
                
                loadCutscene(arg0);
                set = TRUE;
            }            
        }

        if (!set) {
            if (checkLifeEventBit(BASIL_IN_TOWN) && ((gSeason == AUTUMN && gDayOfMonth >= 5) || gSeason == WINTER) && (5 < gHour && gHour < 17)) {
                toggleLifeEventBit(BASIL_IN_TOWN);
                setLifeEventBit(BASIL_GONE);      
                gCutsceneIndex = BASIL_LEAVING;
                loadCutscene(arg0);
                set = TRUE;
            }
        }

        if (!set && arg0 == 0) {
            if (!checkLifeEventBit(CLIFF_ARRIVED) && (gSeason == SPRING && !(gDayOfMonth < 18) || !(gSeason < SUMMER))) {
                setLifeEventBit(CLIFF_ARRIVED);      
                gCutsceneIndex = CLIFF_INTRO;
                loadCutscene(arg0);      
                set = TRUE;
            }
        } 

        // Cliff leaving
        if (!set) {
            if (checkLifeEventBit(0x41) && !checkLifeEventBit(0x42) && gSeason == WINTER && DAYTIME && gHarvestKing != 4 && gHarvestCoinFinder != 4) {
                
                if (MARRIED_TO_ANN && npcAffection[CLIFF] >= 150 && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
                    setLifeEventBit(0x42);
                    setLifeEventBit(0x2D);
                    gCutsceneIndex = 0xD5;
                    loadCutscene(arg0);
                    set = TRUE;                    
                }
                
                if (npcAffection[CLIFF] < 51 && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {              
                    setLifeEventBit(0x42);
                    setLifeEventBit(0x2D);
                    gCutsceneIndex = CLIFF_LEAVING_DEFAULT;
                    loadCutscene(arg0);
                    set = TRUE;                                
                } 
            }
        }

        if (!set) {
            if (!checkLifeEventBit(0x33) && checkDailyEventBit(0x22) && npcAffection[KENT] >= 5 && (5 < gHour && gHour < 9)) {
                setLifeEventBit(0x33);
                gCutsceneIndex = 0x69;
                loadCutscene(arg0);
                set = TRUE;                               
            }
        }
        
        if (!set) {
            if (!checkLifeEventBit(0x35) && checkDailyEventBit(0x23) && npcAffection[KENT] >= 5 && (5 < gHour && gHour < 9)) {
                setLifeEventBit(0x35);
                gCutsceneIndex = 0x6A;
                loadCutscene(arg0);
                set = TRUE;                               
            }
        }
        
        if (!set) {
            if (!checkLifeEventBit(0x37) && checkDailyEventBit(0x24) && npcAffection[KENT] >= 5 && gWeather == SUNNY && (5 < gHour && gHour < 9)) {
                
                setLifeEventBit(0x37);

                if (!func_8009B828(0)) {
                    gCutsceneIndex = 0x6C;
                } else {
                    gCutsceneIndex = 0x6B;
                }
                
                loadCutscene(arg0);
                set = TRUE;                               
            }
        }
        
        if (!set) {
            if (!checkLifeEventBit(0x39) && checkDailyEventBit(0x25) && npcAffection[KENT] >= 5 && (5 < gHour && gHour < 9)) {
                setLifeEventBit(0x39);
                gCutsceneIndex = 0x6F;
                loadCutscene(arg0);
                set = TRUE;                               
            }
        }            
        
        if (!set) {
            if (!checkLifeEventBit(0x3B) && checkDailyEventBit(0x26) && npcAffection[KENT] >= 5 && (5 < gHour && gHour < 9)) {
                setLifeEventBit(0x3B);
                gCutsceneIndex = 0x6D;
                loadCutscene(arg0);
                set = TRUE;                               
            }
        }     

        if (!set) {
            if (!checkLifeEventBit(0x3D) && checkDailyEventBit(0x27) && npcAffection[KENT] >= 5 && (5 < gHour && gHour < 9)) {
                setLifeEventBit(0x3D);
                gCutsceneIndex = 0x6E;
                loadCutscene(arg0);
                set = TRUE;                               
            }
        }     

        if (!set) {
            
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gForecast == TYPHOON && ((npcAffection[HARVEST_SPRITE_1] + npcAffection[HARVEST_SPRITE_2] + npcAffection[HARVEST_SPRITE_3]) / 3) >= 120 && (5 < gHour && gHour < 9)) {

                if ((func_8009B564() + func_8009B374()) >= 2) {           
                    setDailyEventBit(FARM_MORNING_VISIT);
                    gCutsceneIndex = 0xE3;
                    loadCutscene(arg0);
                    set = TRUE;                               
                }
            }
        }    

        if (!set) {
            
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && ((npcAffection[CARPENTER_1] + npcAffection[CARPENTER_2] + npcAffection[MASTER_CARPENTER]) / 3) >= 120 && (5 < gHour && gHour < 9)) {

                if (checkDailyEventBit(0x3F)) {           
                    setDailyEventBit(FARM_MORNING_VISIT);
                    gCutsceneIndex = 0xDD;
                    loadCutscene(arg0);
                    set = TRUE;                               
                }
            }
        }    

        if (!set) {
            if (!checkLifeEventBit(0x8E) && func_8009B374() < 6 && gYear == 1 && gSeason == AUTUMN && gDayOfMonth == 30 && (5 < gHour && gHour < 24)) {
                setLifeEventBit(0x8E);
                gCutsceneIndex = DOUG_FARM_FAVOR;
                loadCutscene(arg0);
                set = TRUE;    
            }
        }
        
        if (!set && arg0 == 0) {
            if (!checkLifeEventBit(0x8F) && checkLifeEventBit(0x94) && gSeason == WINTER && gDayOfMonth == 1 && (5 < gHour && gHour < 10) && gEntranceIndex == HOUSE_EXIT) {
                setLifeEventBit(0x8F);
                gCutsceneIndex = DOUG_DROPPING_OFF_COWS;
                loadCutscene(arg0);
                set = TRUE;    
            }
        }
        
        if (!set && arg0 == 0) {
            if (checkLifeEventBit(0x94) && gSeason == WINTER && gDayOfMonth == 5 && (5 < gHour && gHour < 10) && gEntranceIndex == HOUSE_EXIT) {
                toggleLifeEventBit(0x94);
                gCutsceneIndex = MISS_MANA_PICKUP_COWS;
                loadCutscene(arg0);
                set = TRUE;    
            }
        }      

        if (!set) {
            if (!checkLifeEventBit(0x68) && npcAffection[KENT] >= 50 && gSeason == SUMMER && (14 < gHour && gHour < 17)) {
                setLifeEventBit(0x68);
                gCutsceneIndex = 0x70;
                loadCutscene(arg0);
                set = TRUE;    
            }
        }
        
        if (!set) {
            if (!checkLifeEventBit(PUPPIES) && (npcAffection[KENT] >= 150 || npcAffection[KENT] + npcAffection[STU] >= 250) && DAYTIME) {
                setLifeEventBit(PUPPIES);
                gCutsceneIndex = KENT_AND_STU_FARM_PUPPIES;
                loadCutscene(arg0);
                set = TRUE;    
            }
        }
        
        if (!set) {
            if (!checkLifeEventBit(0x6A) && func_800DC008() >= 8 && gWeather == SUNNY && (gSeason && gSeason < 3) && (5 < gHour && gHour < 12)) {
                setLifeEventBit(0x6A);
                gCutsceneIndex = 0x72;
                loadCutscene(arg0);
                set = TRUE;    
            }
        }        
        
        if (!set) {
            if (!checkLifeEventBit(0x6B) && getFarmPinkCatMintFlowersTilesSum() && gWeather == SUNNY && (gSeason && gSeason < 3) && (5 < gHour && gHour < 12)) {
                setLifeEventBit(0x6B);
                gCutsceneIndex = 0x73;
                loadCutscene(arg0);
                set = TRUE;    
            }
        }        

        if (!set) {
            if (!checkLifeEventBit(0x6C) && getFarmPinkCatMintFlowersTilesSum() >= 16 && npcAffection[STU] >= 80 && gWeather == SUNNY && (gSeason && gSeason < 3) && (5 < gHour && gHour < 12)) {
                setLifeEventBit(0x6C);
                gCutsceneIndex = 0x74;
                loadCutscene(arg0);
                set = TRUE;    
            }
        }      

        if (!set) {
            if (!checkLifeEventBit(0x6D) && npcAffection[MARIA] >= 50 && (14 < gHour && gHour < 17) && (!checkLifeEventBit(MARRIED) || gWife != MARIA)) {
                setLifeEventBit(0x6D);
                gCutsceneIndex = MARIA_FARM_GIFT_CHOCOLATE;
                loadCutscene(arg0);
                set = TRUE;    
            }
        }               
        
        if (!set) {
            if (!checkLifeEventBit(0x6E) && npcAffection[MARIA] >= 80 && (14 < gHour && gHour < 17) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
                setLifeEventBit(0x6E);
                gCutsceneIndex = MARIA_FARM_GIFT_RICE_BALLS;
                loadCutscene(arg0);
                set = TRUE;                               
            }
        }     

        if (!set) {
            if (!checkLifeEventBit(0x6F) && npcAffection[MARIA] >= 200 && gWeather == SUNNY && gSeason == SUMMER && (17 < gHour && gHour < 24) && !checkDailyEventBit(FESTIVAL_DAY_TYPE_2) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
                setLifeEventBit(0x6F);
                gCutsceneIndex = 0x93;
                loadCutscene(arg0);
                set = TRUE;                               
            }
        }     
        
        if (!set) {
            if (!checkLifeEventBit(0x70) && npcAffection[POPURI] >= 50 && (14 < gHour && gHour < 17) && (!checkLifeEventBit(MARRIED) || gWife != POPURI)) {
                setLifeEventBit(0x70);
                gCutsceneIndex = POPURI_FARM_GIFT_TEA;
                loadCutscene(arg0);
                set = TRUE;                               
            }
        }          
        
        if (!set) {
            if (!checkLifeEventBit(0x71) && npcAffection[POPURI] >= 80 && (14 < gHour && gHour < 17) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
                setLifeEventBit(0x71);
                gCutsceneIndex = POPURI_FARM_GIFT_COOKIES;
                loadCutscene(arg0);
                set = TRUE;                               
            }
        }          
        if (!set) {
            
            if (!checkLifeEventBit(0x72) && blueMistFlowerPlot >= 0xC2 && npcAffection[POPURI] >= 200 && gWeather == SUNNY && gSeason == SUMMER && gDayOfMonth == 30 && DAYTIME 
                    && ((MARRIED_TO_POPURI) || (!checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)))) {
                
                blueMistFlowerPlot = 0;
                pondFieldTiles[5][3] = 0;
                setLifeEventBit(0x72);
                gCutsceneIndex = 0x9E;
                loadCutscene(arg0);
                set = TRUE;           
                
            }
        } 

        if (!set) {
            if (!checkLifeEventBit(0x73) && npcAffection[ELLI] >= 50 && (14 < gHour && gHour < 17) && (!checkLifeEventBit(MARRIED) || gWife != ELLI)) {
                setLifeEventBit(0x73);
                gCutsceneIndex = 0xA5;
                loadCutscene(arg0);
                set = TRUE;                      
            }
        }
        
        if (!set) {
            if (!checkLifeEventBit(0x74) && npcAffection[ELLI] >= 80 && (14 < gHour && gHour < 17) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {
                setLifeEventBit(0x74);
                gCutsceneIndex = 0xA6;
                loadCutscene(arg0);
                set = TRUE;                      
            }
        }
        
        if (!set) {
            if (!checkLifeEventBit(0x75) && npcAffection[ELLI] >= 200 && gWeather == SUNNY && gSeason == AUTUMN && gDayOfMonth == 9 && (17 < gHour && gHour < 24) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {
                setLifeEventBit(0x75);
                gCutsceneIndex = 0xA9;
                loadCutscene(arg0);
                set = TRUE;                      
            }
        }      

        if (!set) {
            if (!checkLifeEventBit(0x76) && func_8009B658() >= 6 && gSeason == WINTER && gDayOfMonth == 9 && (5 < gHour && gHour < 24) && (!checkLifeEventBit(MARRIED) || gWife != ELLI)) {
                setLifeEventBit(0x76);
                gCutsceneIndex = 0xAA;
                loadCutscene(arg0);
                set = TRUE;                      
            }
        }  

        if (!set) {
            if (!checkLifeEventBit(0x77) && npcAffection[ANN] >= 50 && (14 < gHour && gHour < 17) && (!checkLifeEventBit(MARRIED) || gWife != ANN)) {
                setLifeEventBit(0x77);
                gCutsceneIndex = 0xB1;
                loadCutscene(arg0);
                set = TRUE;                      
            }
        }  

        if (!set) {
            if (!checkLifeEventBit(0x78) && npcAffection[ANN] >= 80 && (14 < gHour && gHour < 17) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
                setLifeEventBit(0x78);
                gCutsceneIndex = 0xB2;
                loadCutscene(arg0);
                set = TRUE;                      
            }
        }  
        
        if (!set) {
            if (!checkLifeEventBit(0x79) && npcAffection[ANN] >= 200 && gWeather == SUNNY && gSeason == WINTER && DAYTIME && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
                setLifeEventBit(0x79);
                gCutsceneIndex = 0xB5;
                loadCutscene(arg0);
                set = TRUE;                      
            }
        }  

        if (!set) {
            if (!checkLifeEventBit(0x7A) && npcAffection[ANN] >= 50 && (5 < gHour && gHour < 12) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && checkLifeEventBit(0x3C)) {
                setLifeEventBit(0x7A);
                gCutsceneIndex = 0xBC;
                loadCutscene(arg0);
                set = TRUE;                      
            }
        }  

        if (!set) {
            if (!checkLifeEventBit(0x7B) && npcAffection[ANN] >= 80 && (5 < gHour && gHour < 12) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && checkLifeEventBit(0x3C)) {
                setLifeEventBit(0x7B);
                gCutsceneIndex = 0xBD;
                loadCutscene(arg0);
                set = TRUE;                      
            }
        }  

        if (!set) {
            if (!checkLifeEventBit(0x7C) && npcAffection[KAREN] >= 60 && (1 < gSeason && gSeason < 4) && (14 < gHour && gHour < 17) && (!checkLifeEventBit(MARRIED) || gWife != KAREN)) {
                setLifeEventBit(0x7C);
                gCutsceneIndex = 0xBE;
                loadCutscene(arg0);
                set = TRUE;                      
            }
        }  
        
        if (!set) {
            if (!checkLifeEventBit(0x7D) && npcAffection[KAREN] >= 100 && (14 < gHour && gHour < 17) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
                setLifeEventBit(0x7D);
                gCutsceneIndex = 0xBF;
                loadCutscene(arg0);
                set = TRUE;                      
            }
        }  
        
        if (!set) {
            if (!checkLifeEventBit(0x7E) && checkLifeEventBit(0x96) && npcAffection[KAREN] >= 200 && gWeather == SUNNY && gSeason == AUTUMN && gDayOfMonth == 7 && (17 < gHour && gHour < 24) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
                setLifeEventBit(0x7E);
                gCutsceneIndex = 0xC1;
                loadCutscene(arg0);
                set = TRUE;                      
            }
        }  
        
        if (!set) {
            
            if (!checkLifeEventBit(0x7F) && checkLifeEventBit(0x96) && gSeason == WINTER && (6 < gDayOfMonth && gDayOfMonth < 31) && DAYTIME) {
                
                setLifeEventBit(0x7F);
                setLifeEventBit(0x20);

                if (npcAffection[KAREN] >= npcAffection[KAI]) {
                    
                    if (!checkLifeEventBit(0x2F)) {
                        gCutsceneIndex = 0xC2;
                    } else {   
                        gCutsceneIndex = 0xD7;
                    }

                } else {

                    if (checkLifeEventBit(KAI_GONE)) {
                        gCutsceneIndex = 0xC2;
                    } else {
                        gCutsceneIndex = 0xD7;
                    }      
                }

                loadCutscene(arg0);
                set = TRUE;      

            }

        }          

        if (!set) {
            if (!checkLifeEventBit(0x80) && npcAffection[HARRIS] >= 50 && gSeason == AUTUMN && (14 < gHour && gHour < 17)) {
                setLifeEventBit(0x80);
                gCutsceneIndex = 0xCC;
                loadCutscene(arg0);
                set = TRUE;                      
            }
        }       
        
        if (!set) {
            if (!checkLifeEventBit(0x81) && npcAffection[CLIFF] >= 50 && (14 < gHour && gHour < 17)) {
                setLifeEventBit(0x81);
                gCutsceneIndex = 0xD2;
                loadCutscene(arg0);
                set = TRUE;                      
            }
        }       
        
        if (!set) {
            if (!checkLifeEventBit(0x83) && checkLifeEventBit(0x2A) && DAYTIME && (!checkLifeEventBit(MARRIED) || gWife != KAREN) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
                setLifeEventBit(0x83);
                gCutsceneIndex = 0xDA;
                loadCutscene(arg0);
                set = TRUE;                      
            }
        }    

        if (!set && arg0 == 0) {
            if (!checkLifeEventBit(0x84) && checkLifeEventBit(6) && npcAffection[RICK] >= 120 && gWeather == SUNNY && (gSeason && gSeason < 4) && DAYTIME && gEntranceIndex == HOUSE_EXIT) {
                setLifeEventBit(0x84);
                gCutsceneIndex = 0xDC;
                loadCutscene(arg0);
                set = TRUE;                             
            }
        }
        
        if (!set) {
            if (!checkLifeEventBit(0x85) && (5 < gHour && gHour < 12) && func_8009B564() >= 5 && chickenFeedQuantity < 990) {
                setLifeEventBit(0x85);
                gCutsceneIndex = 0xE0;
                loadCutscene(arg0);
                set = TRUE;                             
            }
        }

        if (!set) {
            if (!checkLifeEventBit(0x86) && (gSeason && gSeason < 4) && (5 < gHour && gHour < 12) && gTotalCropsShipped >= 900 && chickenFeedQuantity < 990) {
                setLifeEventBit(0x86);
                gCutsceneIndex = 0xE1;
                loadCutscene(arg0);
                set = TRUE;                             
            }
        }
        
        if (!set && arg0 == 0) {
            if (!checkLifeEventBit(0x87) && (func_800DC008() + getFarmPinkCatMintFlowersTilesSum()) >= 24 && gWeather == SUNNY && gSeason == SPRING && DAYTIME && gEntranceIndex == HOUSE_EXIT) {
                setLifeEventBit(0x87);
                gCutsceneIndex = 0xE4;
                loadCutscene(arg0);
                set = TRUE;                             
            }
        }

        if (!set && arg0 == 0) {
            if (!checkLifeEventBit(0x88) && getFarmGrassTilesSum() >= 90 && gWeather == SUNNY && gSeason == AUTUMN && DAYTIME && gEntranceIndex == HOUSE_EXIT) {
                setLifeEventBit(0x88);
                gCutsceneIndex = 0xE6;
                loadCutscene(arg0);
                set = TRUE;                             
            }
        }
        
        if (!set && arg0 == 0) {
            if (!checkLifeEventBit(0x89) && checkLifeEventBit(0x20) && gWeather == SUNNY && gSeason == AUTUMN && DAYTIME) {
                setLifeEventBit(0x89);
                gCutsceneIndex = 0xE7;
                loadCutscene(arg0);
                set = TRUE;                             
            }
        }
        
        if (!set && arg0 == 0) {
            if (!checkLifeEventBit(0x8A) && checkLifeEventBit(0x53) && gWeather == SUNNY && gSeason == SPRING && DAYTIME && gEntranceIndex == HOUSE_EXIT) {
                setLifeEventBit(0x8A);
                gCutsceneIndex = 0xE8;
                loadCutscene(arg0);
                set = TRUE;                             
            }
        }
        
        if (!set && arg0 == 0) {
            if (!checkLifeEventBit(0x8B) && checkLifeEventBit(0x30) && gWeather == SUNNY && func_8009B3DC() >= 3 && gSeason == WINTER && DAYTIME && gEntranceIndex == HOUSE_EXIT) {
                setLifeEventBit(0x8B);
                gCutsceneIndex = 0xE9;
                loadCutscene(arg0);
                set = TRUE;                             
            }
        }
        
        if (!set && arg0 == 0) {
            if (!checkLifeEventBit(0x8C) && checkLifeEventBit(0x30) && func_8009B828(1) && gWeather == SUNNY && gSeason == AUTUMN && DAYTIME && gEntranceIndex == HOUSE_EXIT) {
                setLifeEventBit(0x8C);
                gCutsceneIndex = 0xEA;
                loadCutscene(arg0);
                set = TRUE;                             
            }
        }

        if (!set && arg0 == 0) {
            if (!checkLifeEventBit(0x8D) && (func_800DC008() + getFarmPinkCatMintFlowersTilesSum()) >= 24 && gWeather == SUNNY && gSeason == SUMMER && DAYTIME && gEntranceIndex == HOUSE_EXIT) {
                setLifeEventBit(0x8D);
                gCutsceneIndex = 0xEC;
                loadCutscene(arg0);
                set = TRUE;                             
            }
        }        

        if (!set) {
            if (checkLifeEventBit(0x90) && npcAffection[DOUG] >= 60 && (16 < gHour && gHour < 21) && ((gSeason == SPRING && gDayOfMonth == 18) || gSeason == AUTUMN && gDayOfMonth == 29)) {
                toggleLifeEventBit(0x90);
                gCutsceneIndex = 0x81;
                loadCutscene(arg0);
                set = TRUE;                             
            }
        }       
        
        if (!set) {
            if (checkLifeEventBit(0x91) && npcAffection[DOUG] >= 60 && gSeason == AUTUMN && gDayOfMonth == 5 && (16 < gHour && gHour < 21)) {
                toggleLifeEventBit(0x91);
                gCutsceneIndex = 0x82;
                loadCutscene(arg0);
                set = TRUE;                             
            }
        }          

        if (!set) {
            if (checkLifeEventBit(0x92) && npcAffection[BASIL] >= 60 && gSeason == SUMMER && gDayOfMonth == 10 && (16 < gHour && gHour < 21)) {
                toggleLifeEventBit(0x92);
                gCutsceneIndex = 0x8B;
                loadCutscene(arg0);
                set = TRUE;                             
            }
        }       
        
        if (!set) {
            if (!checkLifeEventBit(0x95) && checkLifeEventBit(0x2F) && npcAffection[KAI] <= 80 && gSeason == WINTER && DAYTIME && gHarvestKing != 5 && gHarvestCoinFinder != 5 && (!checkLifeEventBit(MARRIED) || gWife != KAREN) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
                setLifeEventBit(0x95);
                setLifeEventBit(0x2E);
                setSpecialDialogueBit(0x24);
                gCutsceneIndex = 0xDB;
                loadCutscene(arg0);
                set = TRUE;                             
            }
        }      
        
        if (!set) {
            if (!checkDailyEventBit(0x51) && npcAffection[POTION_SHOP_DEALER] >= 50 && gWeather == SUNNY && gYear == 1 && gSeason == WINTER && gDayOfMonth == 27 && (5 < gHour && gHour < 9)) {
                setDailyEventBit(0x51);
                gCutsceneIndex = 0xC9;
                loadCutscene(arg0);
                set = TRUE;                             
            }
        } 

      if (!set) {
            if (!checkDailyEventBit(0x51) && npcAffection[HARRIS] >= 100 && gHarvestKing == 1 && gWeather == SUNNY && gSeason == WINTER && gDayOfMonth == 27 && (5 < gHour && gHour < 9)) {
                setDailyEventBit(0x51);
                gCutsceneIndex = 0xCA;
                loadCutscene(arg0);
                set = TRUE;                             
            }
        }    
        
      if (!set) {
            if (!checkDailyEventBit(0x51) && npcAffection[GRAY] >= 100 && gHarvestKing == 2 && gWeather == SUNNY && gSeason == WINTER && gDayOfMonth == 27 && (5 < gHour && gHour < 9)) {
                setDailyEventBit(0x51);
                gCutsceneIndex = 0xCD;
                loadCutscene(arg0);
                set = TRUE;                             
            }
        }    

      if (!set) {
            if (!checkDailyEventBit(0x51) && npcAffection[JEFF] >= 100 && gHarvestKing == 3 && gWeather == SUNNY && gSeason == WINTER && gDayOfMonth == 27 && (5 < gHour && gHour < 9)) {
                setDailyEventBit(0x51);
                gCutsceneIndex = 0xCF;
                loadCutscene(arg0);
                set = TRUE;                             
            }
        }    
        
    if (!set) {
        if (!checkDailyEventBit(0x51) && npcAffection[CLIFF] >= 100 && gHarvestKing == 4 && gWeather == SUNNY && gSeason == WINTER && gDayOfMonth == 27 && (5 < gHour && gHour < 9)) {
            setDailyEventBit(0x51);
            gCutsceneIndex = 0xD3;
            loadCutscene(arg0);
            set = TRUE;                             
        }
    }        
        
       if (!set) {
            if (!checkDailyEventBit(0x51) && npcAffection[KAI] >= 100 && gHarvestKing == 5 && gWeather == SUNNY && gSeason == WINTER && gDayOfMonth == 27 && (5 < gHour && gHour < 9)) {
                setDailyEventBit(0x51);
                gCutsceneIndex = 0xD8;
                loadCutscene(arg0);
                set = TRUE;                             
            }
        }      
            
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(MARIA_MARRIAGE_CUTSCENE) && gSeason == SUMMER && gDayOfMonth == 1 && gWeather == SUNNY && (17 < gHour && gHour < 21) && MARRIED_TO_MARIA) {
                setDailyEventBit(MARIA_MARRIAGE_CUTSCENE);
                gCutsceneIndex = 0xEF;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
            
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(POPURI_MARRIAGE_CUTSCENE) && gSeason == SUMMER && gDayOfMonth == 1 && gWeather == SUNNY && (17 < gHour && gHour < 21) && MARRIED_TO_POPURI) {
                setDailyEventBit(POPURI_MARRIAGE_CUTSCENE);
                gCutsceneIndex = 0xF0;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
            
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(ELLI_MARRIAGE_CUTSCENE) && gSeason == SUMMER && gDayOfMonth == 1 && gWeather == SUNNY && (17 < gHour && gHour < 21) && MARRIED_TO_ELLI) {
                setDailyEventBit(ELLI_MARRIAGE_CUTSCENE);
                gCutsceneIndex = 0xF1;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
            
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(ANN_MARRIAGE_CUTSCENE) && gSeason == SUMMER && gDayOfMonth == 1 && gWeather == SUNNY && (17 < gHour && gHour < 21) && MARRIED_TO_ANN) {
                setDailyEventBit(ANN_MARRIAGE_CUTSCENE);
                gCutsceneIndex = 0xF2;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
            
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(KAREN_MARRIAGE_CUTSCENE) && gSeason == SUMMER && gDayOfMonth == 1 && gWeather == SUNNY && (17 < gHour && gHour < 21) && MARRIED_TO_KAREN) {
                setDailyEventBit(KAREN_MARRIAGE_CUTSCENE);
                gCutsceneIndex = 0xF3;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
    
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(MARIA_MARRIAGE_CUTSCENE) && gSeason == WINTER && gDayOfMonth == 24 && gWeather == SUNNY && (17 < gHour && gHour < 21) && MARRIED_TO_MARIA) {
                setDailyEventBit(MARIA_MARRIAGE_CUTSCENE);
                gCutsceneIndex = 0xF4;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
    
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(POPURI_MARRIAGE_CUTSCENE) && gSeason == WINTER && gDayOfMonth == 24 && gWeather == SUNNY && (17 < gHour && gHour < 21) && MARRIED_TO_POPURI) {
                setDailyEventBit(POPURI_MARRIAGE_CUTSCENE);
                gCutsceneIndex = 0xF5;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
    
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(ELLI_MARRIAGE_CUTSCENE) && gSeason == WINTER && gDayOfMonth == 24 && gWeather == SUNNY && (17 < gHour && gHour < 21) && MARRIED_TO_ELLI) {
                setDailyEventBit(ELLI_MARRIAGE_CUTSCENE);
                gCutsceneIndex = 0xF6;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
    
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(ANN_MARRIAGE_CUTSCENE) && gSeason == WINTER && gDayOfMonth == 24 && gWeather == SUNNY && (17 < gHour && gHour < 21) && MARRIED_TO_ANN) {
                setDailyEventBit(ANN_MARRIAGE_CUTSCENE);
                gCutsceneIndex = 0xF7;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
    
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(KAREN_MARRIAGE_CUTSCENE) && gSeason == WINTER && gDayOfMonth == 24 && gWeather == SUNNY && (17 < gHour && gHour < 21) && MARRIED_TO_KAREN) {
                setDailyEventBit(KAREN_MARRIAGE_CUTSCENE);
                gCutsceneIndex = 0xF8;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
    
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(MARIA_MARRIAGE_CUTSCENE) && gSeason == SUMMER && gDayOfMonth == 7 && gWeather == SUNNY && (16 < gHour && gHour < 21) && MARRIED_TO_MARIA && npcAffection[MARIA] >= 220) {
                setDailyEventBit(MARIA_MARRIAGE_CUTSCENE);
                gCutsceneIndex = 0x96;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
            
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(POPURI_MARRIAGE_CUTSCENE) && gSeason == SUMMER && gDayOfMonth == 7 && gWeather == SUNNY && (16 < gHour && gHour < 21) && MARRIED_TO_POPURI && npcAffection[POPURI] >= 220) {
                setDailyEventBit(POPURI_MARRIAGE_CUTSCENE);
                gCutsceneIndex = 0xA1;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
    
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(ELLI_MARRIAGE_CUTSCENE) && gSeason == SUMMER && gDayOfMonth == 7 && gWeather == SUNNY && (16 < gHour && gHour < 21) && MARRIED_TO_ELLI && npcAffection[ELLI] >= 220) {
                setDailyEventBit(ELLI_MARRIAGE_CUTSCENE);
                gCutsceneIndex = 0xAD;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
    
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(ANN_MARRIAGE_CUTSCENE) && gSeason == SUMMER && gDayOfMonth == 7 && gWeather == SUNNY && (16 < gHour && gHour < 21) && MARRIED_TO_ANN && npcAffection[ANN] >= 220) {
                setDailyEventBit(ANN_MARRIAGE_CUTSCENE);
                gCutsceneIndex = 0xB8;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
    
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(KAREN_MARRIAGE_CUTSCENE) && gSeason == SUMMER && gDayOfMonth == 7 && gWeather == SUNNY && (16 < gHour && gHour < 21) && MARRIED_TO_KAREN && npcAffection[KAREN] >= 220) {
                setDailyEventBit(KAREN_MARRIAGE_CUTSCENE);
                gCutsceneIndex = 0xC5;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
            
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(MARIA_MARRIAGE_CUTSCENE) && gSeason == AUTUMN && gDayOfMonth == 9 && gWeather == SUNNY && (16 < gHour && gHour < 21) && MARRIED_TO_MARIA && npcAffection[MARIA] >= 220) {
                setDailyEventBit(MARIA_MARRIAGE_CUTSCENE);
                gCutsceneIndex = 0x97;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }       
    
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(POPURI_MARRIAGE_CUTSCENE) && gSeason == AUTUMN && gDayOfMonth == 9 && gWeather == SUNNY && (16 < gHour && gHour < 21) && MARRIED_TO_POPURI && npcAffection[POPURI] >= 220) {
                setDailyEventBit(POPURI_MARRIAGE_CUTSCENE);
                gCutsceneIndex = 0xA2;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }      
    
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(ELLI_MARRIAGE_CUTSCENE) && gSeason == AUTUMN && gDayOfMonth == 9 && gWeather == SUNNY && (16 < gHour && gHour < 21) && MARRIED_TO_ELLI && npcAffection[ELLI] >= 220) {
                setDailyEventBit(ELLI_MARRIAGE_CUTSCENE);
                gCutsceneIndex = 0xAE;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }        
            
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(ANN_MARRIAGE_CUTSCENE) && gSeason == AUTUMN && gDayOfMonth == 9 && gWeather == SUNNY && (16 < gHour && gHour < 21) && MARRIED_TO_ANN && npcAffection[ANN] >= 220) {
                setDailyEventBit(ANN_MARRIAGE_CUTSCENE);
                gCutsceneIndex = 0xB9;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }     
            
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(KAREN_MARRIAGE_CUTSCENE) && gSeason == AUTUMN && gDayOfMonth == 9 && gWeather == SUNNY && (16 < gHour && gHour < 21) && MARRIED_TO_KAREN && npcAffection[KAREN] >= 220) {
                setDailyEventBit(KAREN_MARRIAGE_CUTSCENE);
                gCutsceneIndex = 0xC6;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }       

    }

    return result;
}


//INCLUDE_ASM("asm/nonmatchings/game/setCutscenes", setFarmCutscenes);

u16 setFarmCutscenes(void) {

    u16 index = 0xFFFF;
    bool set = FALSE;
    u8 tempHappiness;

    if (checkDailyEventBit(0x52)) { 
        gCutsceneIndex = 0x101;
        gCutsceneFlags |= 2;
        loadCutscene(0);
        set = TRUE;
    }
    
    if (!set) {
        if (checkDailyEventBit(1)) {
            index = 1;
            set = TRUE;
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
            set = TRUE;
        }
    }

    if (!set) {
        if (checkDailyEventBit(4)) {
            switch (D_801C4216) {         
                case 4:
                    tempHappiness = adjustValue(gHappiness, -5, MAX_HAPPINESS);
                    gHappiness += tempHappiness;
                    gCutsceneIndex = 0;
                    break;
                case 2:
                    tempHappiness = adjustValue(gHappiness, -10, MAX_HAPPINESS);
                    gHappiness += tempHappiness;
                    gCutsceneIndex = 1;
                    break;
                case 3:
                    tempHappiness = adjustValue(gHappiness, -10, MAX_HAPPINESS);
                    gHappiness += tempHappiness;
                    gCutsceneIndex = 2;
                    break;
            }
            index = 3;
            set = TRUE;
            gCutsceneFlags |= 2;
        }
    }

    if (!set) {
        if (checkDailyEventBit(0x1D) && !checkDailyEventBit(0x1E)) {
            index = 0x21;
            set = TRUE;
            gCutsceneIndex = 1;
            gCutsceneFlags |= 2;
        }
    }

    if (!set) {
        if (checkDailyEventBit(0x20)) {
            index = 0x22;
            set = TRUE;
            gCutsceneIndex = 1;
            gCutsceneFlags |= 2;
        }
    }

    if (!set) {
        if (checkDailyEventBit(8)) {
            gCutsceneIndex = func_800616CC(gHouseExtensionSelection);
            gCutsceneFlags |= 2;
            index = 4;
            set = TRUE;
        }
    }

    if (!set) {
        if (checkLifeEventBit(0xD) && checkDailyEventBit(0xA) && !checkDailyEventBit(0xC)) {
            gCutsceneIndex = 1;
            index = 5;
            set = TRUE;
        }
    }

    if (!set) {
        if (checkDailyEventBit(0xB)) {
            gCutsceneIndex = 0;
            index = 6;
            set = TRUE;
        }
    }

    if (!set) {
        func_8009C324(0);
    }
    
    return index;

}

//INCLUDE_ASM("asm/nonmatchings/game/setCutscenes", setMountain1Cutscenes);

u16 setMountain1Cutscenes(void) {

    bool set = FALSE;
    u16 index = 0xFFFF;

    if (!checkLifeEventBit(0x48) && gSeason == AUTUMN && (22 < gDayOfMonth && gDayOfMonth < 28) && (5 < gHour && gHour < 15)) {
        setDailyEventBit(0x21);
        if (checkDailyEventBit(0x2C)) {
            gCutsceneIndex = 0x145;
        } else {
            gCutsceneIndex = 0x144;
            gCutsceneFlags |= (2 | 4);
        }
        loadCutscene(0);
        set = TRUE;
    }

    if (!set) {
        if (!checkLifeEventBit(0x4C) && gSeason == WINTER && (11 < gDayOfMonth && gDayOfMonth < 17) && (5 < gHour && gHour < 15)) {
            setDailyEventBit(0x21);
            if (checkDailyEventBit(0x2C)) {
                gCutsceneIndex = 0x145;
            } else {
                gCutsceneIndex = 0x148;
                gCutsceneFlags |= (2 | 4);
            }
            loadCutscene(0);
            set = TRUE;  
        }
    }

    if (!set) {
        if (!checkHaveTool(FISHING_POLE)) {
            setDailyEventBit(0x21);
            gCutsceneIndex = 0x13E;
            loadCutscene(0);
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkLifeEventBit(0xA4) && npcAffection[JEFF] >= 180 && gWeather == SUNNY && gSeason == SUMMER && DAYTIME && (!checkLifeEventBit(MARRIED) || gWife != ELLI) && !checkLifeEventBit(ELLI_JEFF_MARRIED) && gEntranceIndex == TOP_OF_MOUNTAIN_1_SUMMER) {
            setDailyEventBit(0x21);
            setLifeEventBit(0xA4);
            gCutsceneIndex = 0x134;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkLifeEventBit(0xA6) && npcAffection[MARIA] >= 150 && npcAffection[ELLI] >= 150 && gWeather == SUNNY && gSeason == SUMMER && DAYTIME && gEntranceIndex == TOP_OF_MOUNTAIN_1_SUMMER) {
            setDailyEventBit(0x21);
            setLifeEventBit(0xA6);
            gCutsceneIndex = 0x136;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkLifeEventBit(0xA9) && npcAffection[POPURI] >= 150 && npcAffection[KAREN] >= 150 && gWeather == SUNNY && gSeason == SPRING && DAYTIME && gEntranceIndex == 0x1A) {
            setDailyEventBit(0x21);
            setLifeEventBit(0xA9);
            gCutsceneIndex = 0x13A;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
            set = TRUE;
        }
    }

    if (!set) {
      if (!checkLifeEventBit(0xAD) && !checkDailyEventBit(FESTIVAL_DAY_TYPE_2) && npcAffection[ELLI] >= 180 && gWeather == SUNNY && DAYTIME && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {
            setDailyEventBit(0x21);
            setLifeEventBit(0xAD);
            setDailyEventBit(0x58);
            gCutsceneIndex = ELLI_SPRAINED_ANKLE;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
        }
    } 
    
    return index;

}

// mountain 2
//INCLUDE_ASM("asm/nonmatchings/game/setCutscenes", func_800A1C04);

u16 func_800A1C04(void) {

    bool set = FALSE;
    u16 index = 0xFFFF;

    if (!checkLifeEventBit(0xA0) && checkLifeEventBit(0x64) && checkLifeEventBit(0x48) && gWeather == SUNNY && 5 < gHour && gHour < 18 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
        setLifeEventBit(0xA0);
        gCutsceneIndex = 0x12D;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
        set = TRUE;
    }

    if (!set && !checkLifeEventBit(0xA1) && !checkDailyEventBit(0x30) && npcAffection[POPURI] >= 180 && gWeather == SUNNY && 5 < gHour && gHour < 18 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
        setLifeEventBit(0xA1);
        setDailyEventBit(0x57);
        gCutsceneIndex = 0x12E;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
        set = TRUE;
    }
    
    if (!set && !checkLifeEventBit(0xA2) && checkLifeEventBit(0x66) && gWeather == SUNNY && gSeason != WINTER && 5 < gHour && gHour < 18 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && gEntranceIndex == 0x1D) { 
        setLifeEventBit(0xA2);
        gCutsceneIndex = 0x132;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
        set = TRUE;
    }

    if (!set && !checkLifeEventBit(0xA3) && npcAffection[ANN] >= 80 && gWeather == SUNNY && gSeason == AUTUMN && 5 < gHour && gHour < 18) {
        setLifeEventBit(0xA3);
        gCutsceneIndex = 0x133;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
        set = TRUE;
    }

    if (!set && !checkLifeEventBit(0xA5) && npcAffection[CLIFF] >= 150 && gWeather == SUNNY && gSeason != WINTER && 5 < gHour && gHour < 18 && (!checkLifeEventBit(MARRIED) || gWife != ANN) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && gEntranceIndex == 0x1D) {
        setLifeEventBit(0xA5);
        gCutsceneIndex = 0x135;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
        set = TRUE;
    }

    if (!set && !checkLifeEventBit(0xA7) && npcAffection[MARIA] >= 150 && npcAffection[KAREN] >= 150 && gWeather == SUNNY && gSeason == SPRING && 5 < gHour && gHour < 18 && gEntranceIndex == 0x1D) {
        setLifeEventBit(0xA7);
        gCutsceneIndex = 0x137;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
        set = TRUE;
    }
    
    if (!set && !checkLifeEventBit(0xAA) && npcAffection[STU] >= 50 && npcAffection[MAY] >= 50 && gWeather == SUNNY && gSeason < AUTUMN && gDayOfWeek == SATURDAY && 5 < gHour && gHour < 18) {
        setLifeEventBit(0xAA);
        gCutsceneIndex = 0x152;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
        set = TRUE;
    }


    if (!set && !checkLifeEventBit(0x98) && !checkLifeEventBit(MARRIED) && npcAffection[POPURI] >= 250 && gWeather == SUNNY && 11 < gHour && gHour < 18 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
        setLifeEventBit(0x98);
        gCutsceneIndex = 0x156;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
        set = TRUE;
    }

    return index;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/setCutscenes", func_800A2250);

// TODO: label; top of mountain 1
u16 func_800A2250(void) {

    bool set = FALSE;
    u16 index = 0xFFFF;

    if (!checkLifeEventBit(0xA8) && checkLifeEventBit(0x4C) && npcAffection[POPURI] >= 150 && npcAffection[JEFF] >= 120 && gWeather == SUNNY && gSeason >= AUTUMN && DAYTIME && (!checkLifeEventBit(MARRIED) || gWife != POPURI) && (!checkLifeEventBit(MARRIED) || gWife != ELLI) && !checkLifeEventBit(POPURI_GRAY_MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {
        setLifeEventBit(0xA8);
        gCutsceneIndex = 0x138;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
        set = TRUE;
    }

    if (!set) {
        if (!checkLifeEventBit(0xAB) && checkLifeEventBit(0x4C) && gWeather == SUNNY && gSeason == AUTUMN && DAYTIME) {
            setLifeEventBit(0xAB);
            gCutsceneIndex = 0x153;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkLifeEventBit(0xAC) && gWeather == SUNNY && gSeason == WINTER && DAYTIME && checkLifeEventBit(HAVE_KITCHEN) && checkLifeEventBit(HAVE_BATHROOM) && checkLifeEventBit(HAVE_STAIRS) && checkLifeEventBit(HAVE_GREENHOUSE) && checkLifeEventBit(HAVE_LOG_TERRACE) && checkLifeEventBit(HAVE_BABY_BED) && checkLifeEventBit(HOT_SPRINGS_COMPLETED)) {
            setLifeEventBit(0xAC);
            gCutsceneIndex = 0x154;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
        }
    }
    
    return index;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/setCutscenes", setMoonMountainCutscenes);

// mountain 2
u16 setMoonMountainCutscenes(void) {

    bool set = FALSE;
    u16 index = 0xFFFF;

    if (gSeason == SUMMER && gDayOfMonth == 1 && NIGHTTIME && NOT_MARRIED && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
        gCutsceneIndex = MOUNTAIN_FIREWORKS;
        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
        set = TRUE;
    }
    
    if (!set) {
        if (gSeason == WINTER && gDayOfMonth == 24 && NIGHTTIME && NOT_MARRIED && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
            gCutsceneIndex = STARRY_NIGHT_MOON_MOUNTAIN;
            loadCutscene(0);
            gCutsceneFlags |= (2 | 4);
        }
    }
    
    return index;
}

//INCLUDE_ASM("asm/nonmatchings/game/setCutscenes", setSpringCutscenes);

u16 setSpringCutscenes(void) {

    bool set = FALSE;
    u16 index = 0xFFFF;

    if (!checkLifeEventBit(0x9F) && blueMistFlowerPlot >= 0xC2 && gSeason < AUTUMN && DAYTIME) {
        setLifeEventBit(0x9F);
        setDailyEventBit(0x46);
        gCutsceneIndex = 0x13B;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
        set = TRUE;
    }

    if (!set) {
        if (!checkLifeEventBit(0xD5) && checkLifeEventBit(0x9F) && blueMistFlowerPlot >= 0xC2 && gSeason < AUTUMN && DAYTIME) {
            setLifeEventBit(0xD5);
            setDailyEventBit(0x46);
            gCutsceneIndex = 0x13C;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
            set = TRUE;
        }
    }

    if (!set) {
         if (!checkLifeEventBit(0xD6) && checkLifeEventBit(0xD5) && blueMistFlowerPlot >= 0xC2 && gSeason < AUTUMN && DAYTIME) {
            setLifeEventBit(0xD6);
            setDailyEventBit(0x46);
            gCutsceneIndex = 0x13D;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
         }
    }
    
    return index;
}

//INCLUDE_ASM("asm/nonmatchings/game/setCutscenes", setHarvestSpriteCaveCutscenes);

u16 setHarvestSpriteCaveCutscenes(void) {

    u16 index = 0xFFFF;
    
    if (checkHaveKeyItem(MARBLES) && npcAffection[HARVEST_SPRITE_1] >= 80 && DAYTIME) {
        gCutsceneIndex = 0x142;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
    }

    return index;

}

//INCLUDE_ASM("asm/nonmatchings/game/setCutscenes", setCaveCutscenes);

u16 setCaveCutscenes(void) {

    u16 index = 0xFFFF;
    if (gSeason == WINTER && (7 < gDayOfMonth && gDayOfMonth < 31) && (5 < gHour && gHour < 16)) {
        gCutsceneIndex = 0x15A;
        gCutsceneFlags |= 2;
        loadCutscene(0);
    }

    return index;

}

//INCLUDE_ASM("asm/nonmatchings/game/setCutscenes", func_800A29B0);

// house
u16 func_800A29B0(void) {

    u16 index = 0xFFFF;
    bool set = FALSE;

    if (checkLifeEventBit(MARRIED) && !checkLifeEventBit(HAVE_BABY) && checkLifeEventBit(WIFE_PREGNANT) && gWifePregnancyCounter >= 60) {

        setDailyEventBit(0x52);
        gWeather = SUNNY;

        babyBirthdaySeason = gSeason;
        babyBirthdayDate = gDayOfMonth;

        switch (gWife) {
            
            case MARIA:
                gCutsceneIndex = MARIA_BIRTH;
                toggleSpecialDialogueBit(WIFE_PREGNANT_MARIA);
                break;
            case POPURI:
                gCutsceneIndex = POPURI_BIRTH;
                toggleSpecialDialogueBit(WIFE_PREGNANT_POPURI);
                break;
            case ELLI:
                gCutsceneIndex = ELLI_BIRTH;
                toggleSpecialDialogueBit(WIFE_PREGNANT_ELLI);
                break;
            case ANN:
                gCutsceneIndex = ANN_BIRTH;
                toggleSpecialDialogueBit(WIFE_PREGNANT_ANN);
                break;
            case KAREN:
                gCutsceneIndex = KAREN_BIRTH;
                toggleSpecialDialogueBit(WIFE_PREGNANT_KAREN);
                break;
            
        }
        
        loadCutscene(0);
        set = TRUE;
        gCutsceneFlags |= 2;
        
    }

    if (!set) {
        if (checkLifeEventBit(0x60) && !checkLifeEventBit(0x61) && !checkLifeEventBit(MARRIED) && npcAffection[KENT] >= 160 && !checkLifeEventBit(MARRIED)) {
            gCutsceneIndex = 1;
            loadCutscene(0);
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkLifeEventBit(0x62) && npcAffection[CLIFF] >= 30 && gWeather == SUNNY && DAYTIME && (!checkLifeEventBit(MARRIED) || gWife != ANN) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
            gCutsceneIndex = 0x26;
            loadCutscene(0);
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkDailyEventBit(FARM_MORNING_VISIT) && !checkLifeEventBit(WIFE_LEFT) && DAYTIME && MARRIED_TO_MARIA && npcAffection[MARIA] < 100) {
            setDailyEventBit(FARM_MORNING_VISIT);
            setLifeEventBit(WIFE_LEFT);
            gCutsceneIndex = MARIA_LEAVES_MARRIAGE;
            loadCutscene(0);
            gCutsceneFlags |= 2;
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkDailyEventBit(FARM_MORNING_VISIT) && !checkLifeEventBit(WIFE_LEFT) && DAYTIME && MARRIED_TO_POPURI && npcAffection[POPURI] < 100) {
            setDailyEventBit(FARM_MORNING_VISIT);
            setLifeEventBit(WIFE_LEFT);
            gCutsceneIndex = POPURI_LEAVES_MARRIAGE;
            loadCutscene(0);
            gCutsceneFlags |= 2;
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkDailyEventBit(FARM_MORNING_VISIT) && !checkLifeEventBit(WIFE_LEFT) && DAYTIME && MARRIED_TO_ELLI && npcAffection[ELLI] < 100) {
            setDailyEventBit(FARM_MORNING_VISIT);
            setLifeEventBit(WIFE_LEFT);
            gCutsceneIndex = ELLI_LEAVES_MARRIAGE;
            loadCutscene(0);
            gCutsceneFlags |= 2;
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkDailyEventBit(FARM_MORNING_VISIT) && !checkLifeEventBit(WIFE_LEFT) && DAYTIME && MARRIED_TO_ANN && npcAffection[ANN] < 100) {
            setDailyEventBit(FARM_MORNING_VISIT);
            setLifeEventBit(WIFE_LEFT);
            gCutsceneIndex = ANN_LEAVES_MARRIAGE;
            loadCutscene(0);
            gCutsceneFlags |= 2;
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkDailyEventBit(FARM_MORNING_VISIT) && !checkLifeEventBit(WIFE_LEFT) && DAYTIME && MARRIED_TO_KAREN && npcAffection[KAREN] < 100) {
            setDailyEventBit(FARM_MORNING_VISIT);
            setLifeEventBit(WIFE_LEFT);
            gCutsceneIndex = KAREN_LEAVES_MARRIAGE;
            loadCutscene(0);
            gCutsceneFlags |= 2;
            set = TRUE;
        }
    }
    
    return index;
    
}

// kitchen
//INCLUDE_ASM("asm/nonmatchings/game/setCutscenes", func_800A2FA8);

u16 func_800A2FA8(void) {

    u16 index = 0xFFFF;
    bool set = FALSE;

    if (!checkDailyEventBit(MARIA_MARRIAGE_CUTSCENE) && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && checkLifeEventBit(HAVE_BABY) && !checkDailyEventBit(0x52) && gWife == MARIA && babyBirthdaySeason == gSeason && babyBirthdayDate == gDayOfMonth) {
        setDailyEventBit(MARIA_MARRIAGE_CUTSCENE);
        gCutsceneIndex = MARIA_BABY_BIRTHDAY;
        set = TRUE;
        loadCutscene(0);
        gCutsceneFlags |= 2;
        
    }

    if (!set && !checkDailyEventBit(POPURI_MARRIAGE_CUTSCENE) && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && checkLifeEventBit(HAVE_BABY) && !checkDailyEventBit(0x52) && gWife == POPURI && babyBirthdaySeason == gSeason && babyBirthdayDate == gDayOfMonth) {
        setDailyEventBit(POPURI_MARRIAGE_CUTSCENE);
        gCutsceneIndex = POPURI_BABY_BIRTHDAY;
        set = TRUE;
        loadCutscene(0);
        gCutsceneFlags |= 2;
    }

    if (!set && !checkDailyEventBit(ELLI_MARRIAGE_CUTSCENE) && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && checkLifeEventBit(HAVE_BABY) && !checkDailyEventBit(0x52) && gWife == ELLI && babyBirthdaySeason == gSeason && babyBirthdayDate == gDayOfMonth) {
        setDailyEventBit(ELLI_MARRIAGE_CUTSCENE);
        gCutsceneIndex = ELLI_BABY_BIRTHDAY;
        set = TRUE;
        loadCutscene(0);
        gCutsceneFlags |= 2;
    }

    if (!set && !checkDailyEventBit(ANN_MARRIAGE_CUTSCENE) && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && checkLifeEventBit(HAVE_BABY) && !checkDailyEventBit(0x52) && gWife == ANN && babyBirthdaySeason == gSeason && babyBirthdayDate == gDayOfMonth) {
        setDailyEventBit(ANN_MARRIAGE_CUTSCENE);
        gCutsceneIndex = ANN_BABY_BIRTHDAY;
        set = TRUE;
        loadCutscene(0);
        gCutsceneFlags |= 2;
    }

    if (!set && !checkDailyEventBit(KAREN_MARRIAGE_CUTSCENE) && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && checkLifeEventBit(HAVE_BABY) && !checkDailyEventBit(0x52) && gWife == KAREN && babyBirthdaySeason == gSeason && babyBirthdayDate == gDayOfMonth) {
        setDailyEventBit(KAREN_MARRIAGE_CUTSCENE);
        gCutsceneIndex = KAREN_BABY_BIRTHDAY;
        set = TRUE;
        loadCutscene(0);
        gCutsceneFlags |= 2;
    }

    if (!set && !checkDailyEventBit(MARIA_MARRIAGE_CUTSCENE) && gPlayerBirthdaySeason == gSeason && gDayOfMonth == 25 && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && gWife == MARIA && npcAffection[MARIA] >= 100) {
        setDailyEventBit(MARIA_MARRIAGE_CUTSCENE);
        gCutsceneIndex = MARIA_MARRIED_PLAYER_BIRTHDAY;
        set = TRUE;
        loadCutscene(0);
        gCutsceneFlags |= 2;
    }

    if (!set && !checkDailyEventBit(POPURI_MARRIAGE_CUTSCENE) && gPlayerBirthdaySeason == gSeason && gDayOfMonth == 25 && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && gWife == POPURI && npcAffection[POPURI] >= 100) {
        setDailyEventBit(POPURI_MARRIAGE_CUTSCENE);
        gCutsceneIndex = POPURI_MARRIED_PLAYER_BIRTHDAY;
        set = TRUE;
        loadCutscene(0);
        gCutsceneFlags |= 2;
    }

    if (!set && !checkDailyEventBit(ELLI_MARRIAGE_CUTSCENE) && gPlayerBirthdaySeason == gSeason && gDayOfMonth == 25 && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && gWife == ELLI && npcAffection[ELLI] >= 100) {
        setDailyEventBit(ELLI_MARRIAGE_CUTSCENE);
        gCutsceneIndex = ELLI_MARRIED_PLAYER_BIRTHDAY;
        set = TRUE;
        loadCutscene(0);
        gCutsceneFlags |= 2;
    }

    if (!set && !checkDailyEventBit(ANN_MARRIAGE_CUTSCENE) && gPlayerBirthdaySeason == gSeason && gDayOfMonth == 25 && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && gWife == ANN && npcAffection[ANN] >= 100) {
        setDailyEventBit(ANN_MARRIAGE_CUTSCENE);
        gCutsceneIndex = ANN_MARRIED_PLAYER_BIRTHDAY;
        set = TRUE;
        loadCutscene(0);
        gCutsceneFlags |= 2;
    }

    if (!set && !checkDailyEventBit(KAREN_MARRIAGE_CUTSCENE) && gPlayerBirthdaySeason == gSeason && gDayOfMonth == 25 && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && gWife == KAREN && npcAffection[KAREN] >= 100) {
        setDailyEventBit(KAREN_MARRIAGE_CUTSCENE);
        gCutsceneIndex = KAREN_MARRIED_PLAYER_BIRTHDAY;
        set = TRUE;
        loadCutscene(0);
        gCutsceneFlags |= 2;
    }

    if (!set && !checkDailyEventBit(MARIA_MARRIAGE_CUTSCENE) && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && gWife == MARIA && anniversarySeason == gSeason && anniversaryDate == gDayOfMonth) {
        setDailyEventBit(MARIA_MARRIAGE_CUTSCENE);
        gCutsceneIndex = MARIA_ANNIVERSARY;
        set = TRUE;
        loadCutscene(0);
        gCutsceneFlags |= 2;
    }

    if (!set && !checkDailyEventBit(POPURI_MARRIAGE_CUTSCENE) && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && gWife == POPURI && anniversarySeason == gSeason && anniversaryDate == gDayOfMonth) {
        setDailyEventBit(POPURI_MARRIAGE_CUTSCENE);
        gCutsceneIndex = POPURI_ANNIVERSARY;
        set = TRUE;
        loadCutscene(0);
        gCutsceneFlags |= 2;
    }

    if (!set && !checkDailyEventBit(ELLI_MARRIAGE_CUTSCENE) && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && gWife == ELLI && anniversarySeason == gSeason && anniversaryDate == gDayOfMonth) {
        setDailyEventBit(ELLI_MARRIAGE_CUTSCENE);
        gCutsceneIndex = ELLI_ANNIVERSARY;
        set = TRUE;
        loadCutscene(0);
        gCutsceneFlags |= 2;
    }

    if (!set && !checkDailyEventBit(ANN_MARRIAGE_CUTSCENE) && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && gWife == ANN && anniversarySeason == gSeason && anniversaryDate == gDayOfMonth) {
        setDailyEventBit(ANN_MARRIAGE_CUTSCENE);
        gCutsceneIndex = ANN_ANNIVERSARY;
        set = TRUE;
        loadCutscene(0);
        gCutsceneFlags |= 2;
    }

    if (!set && !checkDailyEventBit(KAREN_MARRIAGE_CUTSCENE) && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && gWife == KAREN && anniversarySeason == gSeason && anniversaryDate == gDayOfMonth) {
        setDailyEventBit(KAREN_MARRIAGE_CUTSCENE);
        gCutsceneIndex = KAREN_ANNIVERSARY;
        set = TRUE;
        loadCutscene(0);
        gCutsceneFlags |= 2;
    }
    
    if (!set && !checkDailyEventBit(MARIA_MARRIAGE_CUTSCENE) && gSeason == WINTER && gDayOfMonth == 10 && (16 < gHour && gHour < 20) && checkLifeEventBit(MARRIED) && gWife == MARIA) {
        setDailyEventBit(MARIA_MARRIAGE_CUTSCENE);
        gCutsceneIndex = MARIA_MARRIED_THANKSGIVING;
        set = TRUE;
        loadCutscene(0);
        gCutsceneFlags |= 2;
    }
        
    if (!set && !checkDailyEventBit(POPURI_MARRIAGE_CUTSCENE) && gSeason == WINTER && gDayOfMonth == 10 && (16 < gHour && gHour < 20) && checkLifeEventBit(MARRIED) && gWife == POPURI) {
        setDailyEventBit(POPURI_MARRIAGE_CUTSCENE);
        gCutsceneIndex = POPURI_MARRIED_THANKSGIVING;
        set = TRUE;
        loadCutscene(0);
        gCutsceneFlags |= 2;
    }

    if (!set && !checkDailyEventBit(ELLI_MARRIAGE_CUTSCENE) && gSeason == WINTER && gDayOfMonth == 10 && (16 < gHour && gHour < 20) && checkLifeEventBit(MARRIED) && gWife == ELLI) {
        setDailyEventBit(ELLI_MARRIAGE_CUTSCENE);
        gCutsceneIndex = ELLI_MARRIED_THANKSGIVING;
        set = TRUE;
        loadCutscene(0);
        gCutsceneFlags |= 2;
    }

    if (!set && !checkDailyEventBit(ANN_MARRIAGE_CUTSCENE) && gSeason == WINTER && gDayOfMonth == 10 && (16 < gHour && gHour < 20) && checkLifeEventBit(MARRIED) && gWife == ANN) {
        setDailyEventBit(ANN_MARRIAGE_CUTSCENE);
        gCutsceneIndex = ANN_MARRIED_THANKSGIVING;
        set = TRUE;
        loadCutscene(0);
        gCutsceneFlags |= 2;
    }

    if (!set && !checkDailyEventBit(KAREN_MARRIAGE_CUTSCENE) && gSeason == WINTER && gDayOfMonth == 10 && (16 < gHour && gHour < 20) && checkLifeEventBit(MARRIED) && gWife == KAREN) {
        setDailyEventBit(KAREN_MARRIAGE_CUTSCENE);
        gCutsceneIndex = KAREN_MARRIED_THANKSGIVING;
        set = TRUE;
        loadCutscene(0);
        gCutsceneFlags |= 2;
    }
    
    return index;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/setCutscenes", func_800A3F04);

// ranch
u16 func_800A3F04(void) {

    u16 index = 0xFFFF;
    bool set = FALSE;

    if (gYear == 1 && gSeason == SPRING && gDayOfMonth >= 3 && gWeather == SUNNY && 5 < gHour && gHour < 17 && !checkLifeEventBit(5)) {

        if (!checkLifeEventBit(0xE)) {
            gCutsceneIndex = 0x28A;
        } else {
            gCutsceneIndex = 0x299;
        }

        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
        set = TRUE;
        
    }

    if (!set && gSeason == SUMMER && gDayOfMonth == 1 && 17 < gHour && gHour < 24 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(0x1D)) {

        gCutsceneIndex = 0x293;
        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
        set = TRUE;
    
    }

    if (!set && gSeason == AUTUMN && gDayOfMonth == 4 && 5 < gHour && gHour < 18) {

        if (D_80189054 != 0xFF) {
            setDailyEventBit(0x49);
            D_801886D2 = gFarmAnimals[D_80189054].affection;
        } else {
            toggleDailyEventBit(0x49);
        }
        
        gCutsceneIndex = 0x47E;
        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
        set = TRUE;
    
    }

    if (!set && !checkDailyEventBit(0x1B) && gSeason == WINTER && gDayOfMonth == 10 && 5 < gHour && gHour < 18 && !checkLifeEventBit(MARRIED)) {

        setDailyEventBit(0x1B);
        gCutsceneIndex = 0x296;
        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
        set = TRUE;
    
    }
    
    if (!set && !checkLifeEventBit(0xBE) && npcAffection[GRAY] >= 150 && gWeather == SUNNY && gSeason < AUTUMN && 5 < gHour && gHour < 17 && (!checkLifeEventBit(MARRIED) || gWife != POPURI) && !checkLifeEventBit(0x1B)) {

        if (gEntranceIndex == 0x29) {
            gCutsceneIndex = 0x28C;
            setLifeEventBit(0xBE);
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
            set = TRUE;
        }
        
        if (gEntranceIndex == 0x2A) {
            gCutsceneIndex = 0x28D;
            setLifeEventBit(0xBE);
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
            set = TRUE;
        }
        
    }

    if (!set && !checkLifeEventBit(0xBF) && npcAffection[GRAY] >= 180 && gWeather == SUNNY && gSeason < WINTER && 17 < gHour && gHour < 24 && (!checkLifeEventBit(MARRIED) || gWife != POPURI) && !checkLifeEventBit(0x1B)) {

        if (gEntranceIndex == 0x29) {
            gCutsceneIndex = 0x28E;
            setLifeEventBit(0xBF);
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
            set = TRUE;
        }
        
        if (gEntranceIndex == 0x2A) {
            gCutsceneIndex = 0x28F;
            setLifeEventBit(0xBF);
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
            set = TRUE;
        }
        
    }

    if (!set && !checkLifeEventBit(0xC0) && npcAffection[POPURI] >= 150 && npcAffection[ANN] >= 150 && gWeather == SUNNY && gSeason == AUTUMN && 5 < gHour && gHour < 18 && (!checkLifeEventBit(MARRIED) || gWife != POPURI) && !checkLifeEventBit(0x1B)) {

        if (gEntranceIndex == 0x29) {
            gCutsceneIndex = 0x291;
            setLifeEventBit(0xC0);
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
            set = TRUE;
        }
        
        if (gEntranceIndex == 0x2A) {
            gCutsceneIndex = 0x292;
            setLifeEventBit(0xC0);
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
            set = TRUE;
        }
        
    }
    
    if (!set && !checkLifeEventBit(0xC1) && npcAffection[CLIFF] >= 100 && gWeather == SUNNY && gSeason < WINTER && 5 < gHour && gHour < 18 && (!checkLifeEventBit(MARRIED) || gWife != ANN) && !checkLifeEventBit(0x1D)) {

        setLifeEventBit(0xC1);
        gCutsceneIndex = 0x294;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
        set = TRUE;

    }

    if (!set && !checkLifeEventBit(0x9B) && !checkLifeEventBit(MARRIED) && npcAffection[ANN] >= 250 && gWeather == SUNNY && 14 < gHour && gHour < 24 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(0x1D) && gEntranceIndex == 0x29) {

        setLifeEventBit(0x9B);
        gCutsceneIndex = 0x295;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
        set = TRUE;

    }

    return index;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/setCutscenes", setRanchStoreCutscenes);

u16 setRanchStoreCutscenes(void) {

    u16 index = 0xFFFF;
     
    if (!(checkLifeEventBit(ANN_SICK_VISIT)) && !checkDailyEventBit(FESTIVAL_DAY_TYPE_2) && npcAffection[ANN] >= 120 && RAINING && gDayOfWeek == SUNDAY && DAYTIME && NOT_MARRIED && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
        setLifeEventBit(ANN_SICK_VISIT);
        setDailyEventBit(0x59);
        gCutsceneIndex = ANN_SICK_VISIT_CUTSCENE;
        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
    }
     
    if (MARRIED_TO_ANN && checkLifeEventBit(WIFE_LEFT)) {
        gCutsceneIndex = ANN_REPAIR_MARRIAGE;
        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
    }

    return index;

}

//INCLUDE_ASM("asm/nonmatchings/game/setCutscenes", func_800A4878);

// vineyard
u16 func_800A4878(void) {

    u16 index = 0xFFFF;
    bool set = FALSE;

    if (!checkLifeEventBit(0xC3) && DAYTIME && (!checkLifeEventBit(MARRIED) || gWife != 4) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
        setLifeEventBit(0xC3);
        gCutsceneIndex = 0x2BC;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
        set = TRUE;
    }

    if (!set) {
        if (!checkLifeEventBit(0xC4) && !checkDailyEventBit(FESTIVAL_DAY_TYPE_2) && npcAffection[KAREN] >= 180 && gWeather == SUNNY && DAYTIME && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            setLifeEventBit(0xC4);
            setDailyEventBit(0x5A);
            gCutsceneIndex = 0x2BD;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkLifeEventBit(0xC5) && npcAffection[ANN] >= 120 && npcAffection[KAREN] >= 150 && gWeather == SUNNY && gSeason == WINTER && DAYTIME && (!checkLifeEventBit(MARRIED) || gWife != KAREN) && !checkLifeEventBit(KAREN_KAI_MARRIED) && previousMapIndex == 0x4E) {
            setLifeEventBit(0xC5);
            gCutsceneIndex = 0x2BE;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkLifeEventBit(0xC6) && npcAffection[KAI] >= 50 && npcAffection[KAREN] >= 50 && DAYTIME && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED) && previousMapIndex == 0x4E) {
            setLifeEventBit(0xC6);
            gCutsceneIndex = 0x2BF;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
        }
    }

    return index;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/setCutscenes", setVineyardHouseCutscenes);

u16 setVineyardHouseCutscenes(void) {
    
    u16 index = 0xFFFF;
    
    if (!checkLifeEventBit(KAREN_SICK_VISIT) && !checkDailyEventBit(FESTIVAL_DAY_TYPE_2) && npcAffection[KAREN] >= 120U && RAINING && DAYTIME && NOT_MARRIED && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
        setLifeEventBit(KAREN_SICK_VISIT);
        setDailyEventBit(0x5A);
        gCutsceneIndex = KAREN_SICK;
        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
    }
    
    if (MARRIED_TO_KAREN && checkLifeEventBit(WIFE_LEFT)) {
        gCutsceneIndex = 0x2C8;
        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
    }
    
    return index;
}

//INCLUDE_ASM("asm/nonmatchings/game/setCutscenes", setVineyardCellarCutscenes);

u16 setVineyardCellarCutscenes(void) {
    
    bool set;
    u16 index = 0xFFFF;

    set = FALSE;
    
    if (!checkLifeEventBit(KAREN_YELLOW_HEART_EVENT_CUTSCENE) && checkLifeEventBit(0x67) && DAYTIME && NOT_MARRIED && !checkLifeEventBit(KAREN_KAI_MARRIED) && previousMapIndex == VINEYARD) {
               
        setLifeEventBit(KAREN_YELLOW_HEART_EVENT_CUTSCENE);
        setDailyEventBit(0x4F);

        gCutsceneIndex = KAREN_YELLOW_HEART_EVENT_1;
        gCutsceneFlags |= (2 | 4);

        loadCutscene(0);

        set = TRUE;

    }
    
    if (!set && checkDailyEventBit(0x4F) && NOT_MARRIED && !checkLifeEventBit(KAREN_KAI_MARRIED) && previousMapIndex == VINEYARD_CELLAR_BASEMENT) {
        gCutsceneIndex = KAREN_YELLOW_HEART_EVENT_4;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
    }
    
    return index;
}

//INCLUDE_ASM("asm/nonmatchings/game/setCutscenes", func_800A4E50);

// village 1
u16 func_800A4E50(void) {

    u16 index = 0xFFFF;
    bool set = FALSE;

    if (!checkLifeEventBit(0x9E) && checkLifeEventBit(2) && checkHaveTool(0x13)) {
        setLifeEventBit(0x9E);
        gCutsceneIndex = 0x19A;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
        set = TRUE;
    } 

    if (!set && gSeason == SUMMER && gDayOfMonth == 9 && 5 < gHour && gHour < 18) {
        gCutsceneIndex = 0x3E8;
        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
        set = TRUE;
    } 
    
    if (!set && gSeason == AUTUMN && gDayOfMonth == 20 && 5 < gHour && gHour < 18) {
        gCutsceneIndex = 0x4E2;
        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
        set = TRUE;
    } 

    if (!set && !checkDailyEventBit(0x19) && gSeason == WINTER && gDayOfMonth == 10 && 5 < gHour && gHour < 18 && !checkLifeEventBit(MARRIED)) {
        setDailyEventBit(0x19);
        gCutsceneIndex = 0x1A8;
        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
        set = TRUE;
    } 


    if (!set && !checkLifeEventBit(0xAE) && npcAffection[MARIA] >= 150 && npcAffection[POPURI] >= 150 && gWeather == SUNNY && gSeason == SPRING && 5 < gHour && gHour < 18) {

        if (previousMapIndex == 0x4E) {
            
            gCutsceneIndex = 0x19C;
            setLifeEventBit(0xAE);
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
            set = TRUE;
            
        }
        
        if (previousMapIndex == 0x2D) {

            if (gEntranceIndex == 0x33) {
                gCutsceneIndex = 0x19D;
            } else {
                gCutsceneIndex = 0x19E;
            }

            setLifeEventBit(0xAE);
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
            set = TRUE;
            
        }
        
    } 
    
    if (!set && !checkLifeEventBit(0xAF) && npcAffection[GRAY] >= 120 && gWeather == SUNNY && gSeason < AUTUMN && 5 < gHour && gHour < 18 && (!checkLifeEventBit(MARRIED) || gWife != POPURI) && !checkLifeEventBit(0x1B)) {
        setLifeEventBit(0xAF);
        gCutsceneIndex = 0x19F;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
        set = TRUE;
    } 

    if (!set && !checkLifeEventBit(0x31) && npcAffection[ELLEN] >= 30 && gWeather == SUNNY && gYear == 2 && gSeason == SPRING && 5 < gHour && gHour < 18) {
        gCutsceneIndex = 0x1A0;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
        set = TRUE;
    } 
    
    return index;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/setCutscenes", func_800A5314);

// village 2
u16 func_800A5314(void) {

    u16 index = 0xFFFF;
    bool set = FALSE;

    if (!checkLifeEventBit(0xB6) && (npcAffection[MAYOR] + npcAffection[MAYOR_WIFE] >= 200) && npcAffection[MARIA] >= 150 && 5 < gHour && gHour < 18) {

        if (previousMapIndex == 0x4E) {

            setLifeEventBit(0xB6);
            gCutsceneIndex = 0x1F9;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
            set = TRUE;
            
        }

        if (previousMapIndex == 0x29) {

            if (gEntranceIndex == 0x3F) {
                gCutsceneIndex = 0x1FA;
            } else {
                gCutsceneIndex = 0x1FB;
            }

            setLifeEventBit(0xB6);
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
            set = TRUE;
            
        }
        
    }

    if (!set && !checkLifeEventBit(0xB7) && npcAffection[HARRIS] >= 160 && gWeather == SUNNY && 5 < gHour && gHour < 18 && (!checkLifeEventBit(MARRIED) || gWife != MARIA) && !checkLifeEventBit(0x1A)) {

        if (previousMapIndex == 0x4E) {

            setLifeEventBit(0xB7);
            gCutsceneIndex = 0x1FC;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
            set = TRUE;
            
        }

        if (previousMapIndex == 0x29) {

            if (gEntranceIndex == 0x3F) {
                gCutsceneIndex = 0x1FD;
            } else {
                gCutsceneIndex = 0x1FE;
            }

            setLifeEventBit(0xB7);
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
            set = TRUE;
            
        }
    
    }

    if (!set && !checkLifeEventBit(0xB8) && npcAffection[HARRIS] >= 180 && gSeason < AUTUMN && gWeather == SUNNY && 5 < gHour && gHour < 18 && (!checkLifeEventBit(MARRIED) || gWife != MARIA) && !checkLifeEventBit(0x1A)) {

        if (previousMapIndex == 0x4E) {

            setLifeEventBit(0xB8);
            gCutsceneIndex = 0x1FF;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
            set = TRUE;
            
        }

        if (previousMapIndex == 0x29) {

            if (gEntranceIndex == 0x3F) {
                gCutsceneIndex = 0x200;
            } else {
                gCutsceneIndex = 0x201;
            }

            setLifeEventBit(0xB8);
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
            set = TRUE;
            
        }
    
    }

    if (!set && !checkLifeEventBit(0xB9) && npcAffection[MARIA] >= 150 && npcAffection[ANN] >= 150 && gSeason == AUTUMN && 5 < gHour && gHour < 18) {

        if (previousMapIndex == 0x4E) {

            setLifeEventBit(0xB9);
            gCutsceneIndex = 0x202;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
            set = TRUE;
            
        }

        if (previousMapIndex == 0x29) {

            if (gEntranceIndex == 0x3F) {
                gCutsceneIndex = 0x203;
            } else {
                gCutsceneIndex = 0x204;
            }

            setLifeEventBit(0xB9);
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
            set = TRUE;
            
        }
    
    }

    if (!set && !checkLifeEventBit(0xBA) && checkLifeEventBit(0x65) && gWeather == SUNNY && 5 < gHour && gHour < 17 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(0x1C)) {

        if (previousMapIndex == 0x4E) {

            setLifeEventBit(0xBA);
            gCutsceneIndex = 0x206;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
            set = TRUE;
            
        }

        if (previousMapIndex == 0x29) {

            if (gEntranceIndex == 0x3F) {
                gCutsceneIndex = 0x207;
            } else {
                gCutsceneIndex = 0x208;
            }

            setLifeEventBit(0xBA);
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
            set = TRUE;
            
        }
    
    }

    if (!set && !checkLifeEventBit(0xD0) && checkLifeEventBit(0x1F) && gWife == MARIA && npcAffection[MIDWIFE] >= 80 && gWifePregnancyCounter == 9 && gWeather == SUNNY && 5 < gHour && gHour < 18) {

        setLifeEventBit(0xD0);
        gCutsceneIndex = 0x1F4;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
        set = TRUE;
        
    }

    if (!set && !checkLifeEventBit(0xD1) && checkLifeEventBit(0x1F) && gWife == POPURI && npcAffection[MIDWIFE] >= 80 && gWifePregnancyCounter == 9 && gWeather == SUNNY && 5 < gHour && gHour < 18) {

        setLifeEventBit(0xD1);
        gCutsceneIndex = 0x1F5;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
        set = TRUE;
        
    }

    if (!set && !checkLifeEventBit(0xD2) && checkLifeEventBit(0x1F) && gWife == ELLI && npcAffection[MIDWIFE] >= 80 && gWifePregnancyCounter == 9 && gWeather == SUNNY && 5 < gHour && gHour < 18) {

        setLifeEventBit(0xD2);
        gCutsceneIndex = 0x1F6;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
        set = TRUE;
        
    }

    if (!set && !checkLifeEventBit(0xD3) && checkLifeEventBit(0x1F) && gWife == ANN && npcAffection[MIDWIFE] >= 80 && gWifePregnancyCounter == 9 && gWeather == SUNNY && 5 < gHour && gHour < 18) {

        setLifeEventBit(0xD3);
        gCutsceneIndex = 0x1F7;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
        set = TRUE;
        
    }

    if (!set && !checkLifeEventBit(0xD4) && checkLifeEventBit(0x1F) && gWife == KAREN && npcAffection[MIDWIFE] >= 80 && gWifePregnancyCounter == 9 && gWeather == SUNNY && 5 < gHour && gHour < 18) {

        setLifeEventBit(0xD4);
        gCutsceneIndex = 0x1F8;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
        set = TRUE;
        
    }

    if (!set && !checkLifeEventBit(0x9A) && !checkLifeEventBit(MARRIED) && npcAffection[MARIA] >= 250 && gWeather == SUNNY && 11 < gHour && gHour < 18 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(0x1A) && previousMapIndex == 0x3C) {
        setLifeEventBit(0x9A);
        gCutsceneIndex = 0x205;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
    } 
    
    return index;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/setCutscenes", setRickStoreCutscenes);

u16 setRickStoreCutscenes(void) {
    
    u16 index = 0xFFFF;
    
    if (!checkLifeEventBit(ANN_RICK_CUTSCENE) && npcAffection[RICK] >= 40 && DAYTIME) {
        setLifeEventBit(ANN_RICK_CUTSCENE);
        gCutsceneIndex = ANN_AND_RICK;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
    }
    
    return index;

}

//INCLUDE_ASM("asm/nonmatchings/game/setCutscenes", setFlowerShopCutscenes);

u16 setFlowerShopCutscenes(void) {
    
    u16 index = 0xFFFF;
    
    if (!checkLifeEventBit(POPURI_SICK_CUTSCENE) && !checkDailyEventBit(FESTIVAL_DAY_TYPE_2) && npcAffection[POPURI] >= 120 && RAINING && DAYTIME && NOT_MARRIED && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
        setLifeEventBit(POPURI_SICK_CUTSCENE);
        setDailyEventBit(0x57);
        gCutsceneIndex = POPURI_SICK;
        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
    }
    
    if (MARRIED_TO_POPURI && checkLifeEventBit(WIFE_LEFT)) {
        gCutsceneIndex = 0x1BC;
        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
    }
    
    return index;

}

//INCLUDE_ASM("asm/nonmatchings/game/setCutscenes", func_800A5F48);

// bakery
u16 func_800A5F48(void) {

    bool set = FALSE;
    u16 index = 0xFFFF;

    if (gSeason == SUMMER && gDayOfMonth == 1 && 17 < gHour && gHour < 24 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(0x1C)) {

        if (!checkLifeEventBit(0x31)) {
            gCutsceneIndex = 0x1A2;
        } else {
            gCutsceneIndex = 0x1A4;
        }

        loadCutscene(0);
        set = TRUE;
        gCutsceneFlags |= (2 | 4);
        
    }

    if (!set && !checkDailyEventBit(0x1A) && gSeason == WINTER && gDayOfMonth == 10 && 5 < gHour && gHour < 18 && !checkLifeEventBit(MARRIED)) {

        setDailyEventBit(0x1A);
        gCutsceneIndex = 0x1AB;
        loadCutscene(0);
        set = TRUE;
        gCutsceneFlags |= (2 | 4);
        
    }

    if (!set && !checkLifeEventBit(0xB2) && npcAffection[ELLI] >= 150 && npcAffection[ANN] >= 150 && 5 < gHour && gHour < 18 && (gSeason == AUTUMN && gDayOfMonth >= 15 || gSeason == WINTER && gDayOfMonth < 10)) {

        setLifeEventBit(0xB2);
        gCutsceneIndex = 0x1B0;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
        set = TRUE;
        
    }

    if (!set && !checkLifeEventBit(0xB3) && !checkDailyEventBit(0x30) && npcAffection[JEFF] >= 150 && 5 < gHour && gHour < 18  && (!checkLifeEventBit(MARRIED) || gWife != ELLI) && !checkLifeEventBit(0x1C) && previousMapIndex == 0x29) {

        setLifeEventBit(0xB3);
        
        if (gEntranceIndex == 0x59) {
            gCutsceneIndex = 0x1B1;
        } else {
            gCutsceneIndex = 0x1B2;
        }
        
        loadCutscene(0);
        set = TRUE;
        gCutsceneFlags |= (2 | 4);
        
    }
    
    if (!set && !checkLifeEventBit(0x97) && !checkDailyEventBit(0x30) && npcAffection[ELLI] >= 120 && (SUNNY < gWeather && gWeather < 4) && 5 < gHour && gHour < 18  && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(0x1C)) {

        setLifeEventBit(0x97);
        setDailyEventBit(0x58);
        gCutsceneIndex = 0x1BA;
        loadCutscene(0);
        set = TRUE;
        gCutsceneFlags |= (2 | 4);
        
    }

    if (!set && !checkLifeEventBit(0x99) && !checkLifeEventBit(MARRIED) && npcAffection[ELLI] >= 250 && gWeather == SUNNY && 11 < gHour && gHour < 17 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(0x1C)) {

        setLifeEventBit(0x99);
        gCutsceneIndex = 0x1aa;
        set = TRUE;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
        
    }

    if (checkLifeEventBit(MARRIED) && gWife == ELLI && checkLifeEventBit(0xD9)) {
        gCutsceneIndex = 0x1Bd;
        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
    }

    return index;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/setCutscenes", func_800A6440);

// mayor house
u16 func_800A6440(void) {

    u16 index = 0xFFFF;
    bool set = FALSE;

    if (!checkDailyEventBit(MARIA_MARRIAGE_CUTSCENE) && gSeason == WINTER && gDayOfMonth == 10 && (5 < gHour && gHour < 18) && !checkLifeEventBit(MARRIED)) {
        setDailyEventBit(MARIA_MARRIAGE_CUTSCENE);
        gCutsceneIndex = THANKSGIVING_MAYOR_HOUSE;
        set = TRUE;
        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
    }

    if (!set && !checkLifeEventBit(MARIA_SICK_CUTSCENE) && !checkDailyEventBit(FESTIVAL_DAY_TYPE_2) && npcAffection[MARIA] >= 120 && (SUNNY < gWeather && gWeather < 4) && gDayOfWeek == MONDAY && (8 < gHour && gHour < 17) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
        setLifeEventBit(MARIA_SICK_CUTSCENE);
        setDailyEventBit(MARIA_ABSENT);
        gCutsceneIndex = MARIA_SICK;
        set = TRUE;
        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
    }

    if (checkLifeEventBit(MARRIED) && gWife == MARIA && checkLifeEventBit(WIFE_LEFT)) {
        gCutsceneIndex = MARIA_REPAIR_MARRIAGE;
        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
    }

    return index;

}    

//INCLUDE_ASM("asm/nonmatchings/game/setCutscenes", func_800A6634);

// library
u16 func_800A6634(void) {

    u16 index = 0xFFFF;
    bool set = FALSE;

    if (!checkLifeEventBit(MARIA_SPRAINED_ANKLE_CUTSCENE) && !checkDailyEventBit(FESTIVAL_DAY_TYPE_2) && npcAffection[MARIA] >= 180 && (8 < gHour && gHour < 17) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
        setLifeEventBit(MARIA_SPRAINED_ANKLE_CUTSCENE);
        setDailyEventBit(MARIA_ABSENT);
        gCutsceneIndex = MARIA_SPRAINED_ANKLE;
        set = TRUE;
        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
    }

    if (!set) {
        if (!checkLifeEventBit(0xBD) && checkLifeEventBit(0x63) && (8 < gHour && gHour < 17) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
            setLifeEventBit(0xBD);
            gCutsceneIndex = 0x20F;
            loadCutscene(0);
            gCutsceneFlags |= (2 | 4);
        }
    }
    
    return index;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/setCutscenes", setChurchCutscenes);

u16 setChurchCutscenes(void) {

    bool set = FALSE;
    u16 index = 0xFFFF;

    if (gSeason == WINTER && gDayOfMonth == 24 && NIGHTTIME && NOT_MARRIED) {     
        gCutsceneIndex = STARRY_NIGHT_FESTIVAL;
        loadCutscene(0);
        set = TRUE;
        gCutsceneFlags |= (2 | 4);
    } 
    
    if (!set) {
        if (gSeason == WINTER && gDayOfMonth == 30 && NIGHTTIME) {
            gCutsceneIndex = CHURCH_NEW_YEARS;
            loadCutscene(0);
            gCutsceneFlags |= (2 | 4);
        }    
    } 
    
    return index;
}

//INCLUDE_ASM("asm/nonmatchings/game/setCutscenes", func_800A68C0);

// tavern
u16 func_800A68C0(void) {

    u16 index = 0xFFFF;
    bool set = FALSE;

    if (gSeason == WINTER && gDayOfMonth == 30 && (17 < gHour && gHour < 24)) {
        gCutsceneIndex = TAVERN_NEW_YEARS;
        loadCutscene(0);
        set = TRUE;
        gCutsceneFlags |= (2 | 4);
    }

    if (!set && !checkLifeEventBit(KAREN_KAI_TAVERN_CUTSCENE) && npcAffection[KAREN] >= 80 && npcAffection[KAI] >= 80 && (17 < gHour && gHour < 24) && (!checkLifeEventBit(MARRIED) || gWife != KAREN) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
        setLifeEventBit(KAREN_KAI_TAVERN_CUTSCENE);
        setSpecialDialogueBit(KAREN_KAI_TAVERN_CUTSCENE_DIALOGUE);
        gCutsceneIndex = KAREN_KAI_TAVERN;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
    }

    return index;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/setCutscenes", func_800A6A14);

// square
u16 func_800A6A14(void) {

    u16 index = 0xFFFF;
    bool set = FALSE;

    if (gSeason == SPRING && gDayOfMonth == 8 && 5 < gHour && gHour < 18) {

        if (gYear == 1) {
            gCutsceneIndex = 0x352;
        } else {
            if (gHarvestKing == PLAYER) {
                gCutsceneIndex = 0x353;
            } else {
                gCutsceneIndex = 0x354;
            }
        }
        
        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
        set = TRUE;
        
    }

    if (!set && gSeason == SPRING && gDayOfMonth == 23 && 5 < gHour && gHour < 18) {

        gCutsceneIndex = 0x3B6;
        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
        set = TRUE;
        
    }

    if (!set && gSeason == SUMMER && gDayOfMonth == 1 && 17 < gHour && gHour < 24 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(0x1A)) {

        gCutsceneIndex = 0x258;
        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
        set = TRUE;
        
    }

    if (!set && gSeason == AUTUMN && gDayOfMonth == 12 && 5 < gHour && gHour < 18) {

        setDailyEventBit(0x5D);
        setHarvestKingName(gHarvestCoinFinder);
        
        gCutsceneIndex = 0x4B0;
        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
        set = TRUE;
        
    }

    if (!set && gSeason == WINTER && gDayOfMonth == 27 && 17 < gHour && gHour < 24) {

        if (gHarvestKing == PLAYER) {
            gCutsceneIndex = 0x546;
        } else {
            if (!checkDailyEventBit(0x50)) {
                gCutsceneIndex = 0x547;
            } else {
                gCutsceneIndex = 0x548;
            }
        }
    
        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
        set = TRUE;
        
    }


    if (!set && gSeason == SPRING && gDayOfMonth == 1 && 5 < gHour && gHour < 18) {
        gCutsceneIndex = 0x578;
        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
        set = TRUE;
    }

    if (!set && gSeason == SPRING && gDayOfMonth == 17 && 7 < gHour && gHour < 18) {
        func_800CB0C4();
        gCutsceneIndex = 0x384;
        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
        set = TRUE;
    }
    
    if (!set && gSeason == AUTUMN && gDayOfMonth == 28 && 7 < gHour && gHour < 18) {
        func_800CB0C4();
        gCutsceneIndex = 0x384;
        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
        set = TRUE;
    }

    if (!set && gSeason == WINTER && gDayOfMonth == 19 && 7 < gHour && gHour < 18) {
        func_800CB8F0();
        gCutsceneIndex = 0x514;
        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
    }
    
    return index;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/setCutscenes", func_800A6EE4);

// race track
u16 func_800A6EE4(void) {

    u16 index = 0xFFFF;
    bool set = FALSE;

    if (checkDailyEventBit(0x40) && checkDailyEventBit(0x41)) {
        func_800CB0C4();
        toggleDailyEventBit(0x40);
        gCutsceneIndex = HORSE_RACE_STARTING_ANNOUNCEMENT;
        loadCutscene(0);
        set = TRUE;
        gCutsceneFlags |= (2 | 4);
    }

    if (!set) {
        if (checkDailyEventBit(0x40) && checkDailyEventBit(0x42)) {
            func_800CB8F0();
            toggleDailyEventBit(0x40);
            gCutsceneIndex = DOG_RACE_AFTER_RACE;
            loadCutscene(0);
            set = TRUE;
            gCutsceneFlags |= (2 | 4);
        }
    }

    if (!set) { 
        if (gSeason == SPRING && gDayOfMonth == 17 && (7 < gHour && gHour < 18)) {
            func_800CB0C4();
            gCutsceneIndex = HORSE_RACE_STARTING_ANNOUNCEMENT;
            loadCutscene(0);
            set = TRUE;
            gCutsceneFlags |= (2 | 4);
        }
    }
    
    if (!set) { 
        if (gSeason == AUTUMN && gDayOfMonth == 28 && (7 < gHour && gHour < 18)) {
            func_800CB0C4();
            gCutsceneIndex = HORSE_RACE_STARTING_ANNOUNCEMENT;
            loadCutscene(0);
            set = TRUE;
            gCutsceneFlags |= (2 | 4);
        }
    }

    if (!set) { 
        if (gSeason == WINTER && gDayOfMonth == 19 && (7 < gHour && gHour < 18)) {
            func_800CB8F0();
            gCutsceneIndex = DOG_RACE_AFTER_RACE;
            loadCutscene(0);
            set = TRUE;
            gCutsceneFlags |= (2 | 4);
        }
    }
    
    return index;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/setCutscenes", setBeachCutscenes);

u16 setBeachCutscenes(void) {
    
    bool set = FALSE;
    u16 index = 0xFFFF;

    if (gSeason == SUMMER && gDayOfMonth == 17 && NIGHTTIME) {
        gCutsceneIndex = FIREFLY_FESTIVAL;
        loadCutscene(0);
        set = TRUE;
        gCutsceneFlags |= (2 | 4);
    }

    if (!set) {
        if (gSeason == SUMMER && gDayOfMonth == 24 && DAYTIME) {
            gCutsceneIndex = SEA_FESTIVAL;
            loadCutscene(0);
            set = TRUE;
            gCutsceneFlags |= (2 | 4);
        }
    }
    
    if (!set) {
        if (gSeason == SUMMER && gDayOfMonth == 1 && NIGHTTIME && NOT_MARRIED && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            gCutsceneIndex = BEACH_FIREWORKS;
            loadCutscene(0);
            set = TRUE;
            gCutsceneFlags |= (2 | 4);
        }
    }
    
    if (!set) {
        if (!checkDailyEventBit(KAREN_MARRIAGE_CUTSCENE) && gSeason == WINTER && gDayOfMonth == 10 && DAYTIME && NOT_MARRIED) {
            setDailyEventBit(KAREN_MARRIAGE_CUTSCENE);
            gCutsceneIndex = 0x2F1;
            loadCutscene(0);
            gCutsceneFlags |= (2 | 4);
            set = TRUE;
        }
    }

    if (!set) {
        if (gSeason == WINTER && gDayOfMonth == 24 && NIGHTTIME && NOT_MARRIED && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            gCutsceneIndex = 0x2F4;
            loadCutscene(0);
            gCutsceneFlags |= (2 | 4);
            set = TRUE;
        }
    }
    
    if (!set) {
        if (!checkLifeEventBit(0xCA) && npcAffection[KAREN] < 101 && DAYTIME && (gYear >= 3 || (gYear == 2 && gSeason == WINTER)) && (NOT_MARRIED || gWife != KAREN) && !checkLifeEventBit(KAREN_KAI_MARRIED) && gFlowerFestivalGoddess != 4) {
            setLifeEventBit(0xCA);
            setLifeEventBit(0x2F);
            setSpecialDialogueBit(0x23);
            gCutsceneIndex = 0x2F0;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkLifeEventBit(KAREN_AND_ELLI_BEACH_CUTSCENE) && npcAffection[ELLI] >= 150 && npcAffection[KAREN] >= 150 && gWeather == SUNNY && gSeason == SUMMER && DAYTIME && (NOT_MARRIED || gWife != KAREN) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            setLifeEventBit(KAREN_AND_ELLI_BEACH_CUTSCENE);
            gCutsceneIndex = KAREN_AND_ELLI;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkLifeEventBit(0x9C) && NOT_MARRIED && npcAffection[KAREN] >= 250 && gWeather == SUNNY && (14 < gHour && gHour < 24) && NOT_MARRIED && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            setLifeEventBit(0x9C);
            gCutsceneIndex = KAREN_PINK_HEART_EVENT;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);     
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkLifeEventBit(0x9D) && checkLifeEventBit(0x20) && npcAffection[KAI] >= 180 && gWeather == SUNNY && NIGHTTIME && (NOT_MARRIED || gWife != KAREN) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            setLifeEventBit(0x9D);
            gCutsceneIndex = 0x2EF;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
        }
    }
    
    return index;
}

//INCLUDE_ASM("asm/nonmatchings/game/setCutscenes", setRoadCutscenes);

u16 setRoadCutscenes(void) {
    
    bool set = FALSE;
    u16 index = 0xFFFF;

    if (!checkLifeEventBit(0xCC) && npcAffection[CLIFF] >= 200 && gSeason == WINTER && DAYTIME && (!checkLifeEventBit(MARRIED) || gWife != ANN) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && previousMapIndex == 0x52) {
        setLifeEventBit(0xCC);
        gCutsceneIndex = 0x320;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
        set = TRUE;
    }

    if (!set) {
        if (!checkLifeEventBit(0xCD) && bakeryCardPoints >= 6 && npcAffection[ANN] >= 150 && DAYTIME && NOT_MARRIED && !checkLifeEventBit(ANN_CLIFF_MARRIED) && previousMapIndex == RANCH) {
            setLifeEventBit(0xCD);
            gCutsceneIndex = 0x321;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
            set = TRUE;
        }    
    }

    if (!set) {
        if (!checkLifeEventBit(SHADY_SALESMAN_MARIA_CUTSCENE) && checkLifeEventBit(0xB6) && DAYTIME) {

            if (previousMapIndex == VILLAGE_1) {
                setLifeEventBit(SHADY_SALESMAN_MARIA_CUTSCENE);
                gCutsceneIndex = SHADY_SALESMAN_AND_MARIA_1;
                gCutsceneFlags |= (2 | 4);
                loadCutscene(0);
                set = TRUE;
            }

            if (previousMapIndex == VILLAGE_2) {
                setLifeEventBit(SHADY_SALESMAN_MARIA_CUTSCENE);
                gCutsceneIndex = SHADY_SALESMAN_AND_MARIA_2;
                gCutsceneFlags |= (2 | 4);
                loadCutscene(0);
                set = TRUE;
            }
        }
    }

    if (!set) {
        if (!checkLifeEventBit(ANN_SPRAINED_ANKLE_CUTSCENE) && !checkDailyEventBit(FESTIVAL_DAY_TYPE_2) && npcAffection[ANN] >= 180 && DAYTIME && NOT_MARRIED && !checkLifeEventBit(ANN_CLIFF_MARRIED) && previousMapIndex == FARM) {
            setLifeEventBit(ANN_SPRAINED_ANKLE_CUTSCENE);
            setDailyEventBit(0x59);
            gCutsceneIndex = ANN_SPRAINED_ANKLE;
            loadCutscene(0);
            gCutsceneFlags |= (2 | 4); 
        }
    }
    
    return index;

}

//INCLUDE_ASM("asm/nonmatchings/game/setCutscenes", func_800A7AE8);
 
// time-based daily cutscenes/update game
u16 func_800A7AE8(u8 arg0) {

    u16 index = 0xFFFF;
    bool set = FALSE;
    
    if (!(gCutsceneFlags & 1)) {
        
        gCutsceneFlags &= ~2;
    
        if (gHour >= 17 && !checkDailyEventBit(0)) {

            if (dailyShippingBinValue) {
                gCutsceneIndex = 0;
            } else {
                gCutsceneIndex = 1;
            }

            handleDailyShipment();

            if (gBaseMapIndex == FARM) {
                index = 0;
                set = TRUE;
            }       

        }
        
        if (!set) {

            if (checkLifeEventBit(HOUSE_EXTENSION_CONSTRUCTION)) {

                if (gHour >= 8 && !checkDailyEventBit(HOUSE_EXTENSION_WORK) && !checkDailyEventBit(END_OF_DAILY_HOUSE_EXTENSION_WORK)) {
                    
                    setDailyEventBit(HOUSE_EXTENSION_WORK);
                    
                    if (gBaseMapIndex == FARM) {
                        gCutsceneIndex = 0;
                        index = 5;
                        set = TRUE;
                    }
                
                }

                if (gHour >= 15) {
                    if (!checkDailyEventBit(END_OF_DAILY_HOUSE_EXTENSION_WORK)) {
                        setDailyEventBit(END_OF_DAILY_HOUSE_EXTENSION_WORK);
                    }
                }
            }   
        }
        
        if (!set) {
            
            if (!checkDailyEventBit(0x1D) && !checkDailyEventBit(0x1E) && gSeason == AUTUMN && gDayOfMonth == 3 && (5 < gHour && gHour < 15) && func_8009B2BC(2)) {
               
                setDailyEventBit(0x1D);
                
                if (gBaseMapIndex == FARM) {
                    index = 0x21;
                    set = TRUE;
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

            // DMA cutscene data
            func_8009BF90(index);

        }
    }

    return index;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/setCutscenes", func_800A7DFC);

// cutscenes on wake up: evaluation, wedding, rivals' weddings, animal funeral, sick days
u16 func_800A7DFC(void) {

    bool set = FALSE;
    u16 result = 0xFFFF;
    u8 temp;

    if (!(gCutsceneFlags & 1)) {
        
        gCutsceneFlags &= ~2;

        // evaluation
        if (gYear == 3 && gSeason == SUMMER && gDayOfMonth == 1) {
            
            set = TRUE;
            
            D_801C3B62 = calculateNPCAffectionScore();
            
            D_80215DF0 = npcAffection[MARIA] + npcAffection[POPURI] + npcAffection[ELLI] + npcAffection[ANN] + npcAffection[KAREN];
            
            // sick days
            D_801886E0 = gSickDays;
            // dead animals
            D_801886A8 = D_8013DC2E;
            
            D_801C3E1C = gTotalCropsShipped + gTotalMilkShipped + gTotalEggsShipped;
            
            // fish caught
            D_801D62C4 = totalFishCaught; 
            
            D_8017026E = calculateHouseExtensionScore();
            D_80205638 = calculateGrassScore();
            D_8013D438 = calculateFieldScore(); 
            D_80189824 = calculateShippingScore();
            
            // animals
            D_80205209 = func_8009B3DC();
            D_801D62C6 = func_8009B4EC();
            D_801886D0 = func_8009B564();

            // recipes sum
            D_80188F68 = func_80065518();

            setEntrance(0x3E);
            gCutsceneIndex = EVALUATION;
            loadCutscene(0);
            gCutsceneFlags |= (2 | 4);
            
        }

        // wedding
        if (!set && gDayOfWeek == SUNDAY && checkLifeEventBit(ENGAGED)) {
            
            toggleLifeEventBit(ENGAGED);
            setLifeEventBit(MARRIED);
            setSpecialDialogueBit(3);
            
            anniversarySeason = gSeason;
            anniversaryDate = gDayOfMonth;
            
            switch (gWife) {                        
                case MARIA:                                 
                    gCutsceneIndex = MARIA_WEDDING;
                    setSpecialDialogueBit(MARRIED_TO_MARIA_DIALOGUE);
                    setSpecialDialogueBit(GOT_MARRIED_DIALOGUE);
                    break;
                case 1:                                 
                    gCutsceneIndex = POPURI_WEDDING;
                    setSpecialDialogueBit(MARRIED_TO_POPURI_DIALOGUE);
                    setSpecialDialogueBit(GOT_MARRIED_DIALOGUE);
                    break;
                case 2:                                 
                    gCutsceneIndex = ELLI_WEDDING;
                    setSpecialDialogueBit(MARRIED_TO_ELLI_DIALOGUE);
                    setSpecialDialogueBit(GOT_MARRIED_DIALOGUE);
                    break;
                case 3:                                 
                    gCutsceneIndex = ANN_WEDDING;
                    setSpecialDialogueBit(MARRIED_TO_ANN_DIALOGUE);
                    setSpecialDialogueBit(GOT_MARRIED_DIALOGUE);
                    break;
                case 4:                                 
                    gCutsceneIndex = KAREN_WEDDING;
                    setSpecialDialogueBit(MARRIED_TO_KAREN_DIALOGUE);
                    setSpecialDialogueBit(GOT_MARRIED_DIALOGUE);
                    break;
                default:
                    break;
            }
            
            setEntrance(0x32);
            loadCutscene(0);
            set = TRUE;
            gCutsceneFlags |= (2 | 4);

        }

        // rivals' weddings
        if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_2)) { 
            
            if (!set && !checkLifeEventBit(MARIA_HARRIS_MARRIED) && (!checkLifeEventBit(MARRIED) || gWife != MARIA) && npcAffection[HARRIS] >= 200 && (checkLifeEventBit(MARRIED) || npcAffection[MARIA] < 120))  {
                
                setLifeEventBit(MARIA_HARRIS_MARRIED);
                setSpecialDialogueBit(0xB);
                gCutsceneIndex = 0xCB;
                setEntrance(0);
                loadCutscene(0);
                set = TRUE;
                gCutsceneFlags |= (2 | 4);
                
            }

            if (!set && !checkLifeEventBit(POPURI_GRAY_MARRIED) && (!checkLifeEventBit(MARRIED) || gWife != POPURI) && npcAffection[GRAY] >= 200 && (checkLifeEventBit(MARRIED) || npcAffection[POPURI] < 120)) {
               
                setLifeEventBit(POPURI_GRAY_MARRIED);
                setSpecialDialogueBit(0xC);
                gCutsceneIndex = 0xCE;
                setEntrance(0);
                loadCutscene(0);
                set = TRUE;
                gCutsceneFlags |= (2 | 4);

            }

            if (!set && !checkLifeEventBit(ELLI_JEFF_MARRIED) && (!checkLifeEventBit(MARRIED) || gWife != ELLI) && npcAffection[JEFF] >= 200 && (checkLifeEventBit(MARRIED) || npcAffection[ELLI] < 120)) {
                
                setLifeEventBit(ELLI_JEFF_MARRIED);
                setSpecialDialogueBit(0xD);
                gCutsceneIndex = 0xD0;
                setEntrance(0);
                loadCutscene(0);
                set = TRUE;
                gCutsceneFlags |= (2 | 4);

            }

            if (!set && !checkLifeEventBit(ANN_CLIFF_MARRIED) && (!checkLifeEventBit(MARRIED) || gWife != ANN) && npcAffection[CLIFF] >= 200 && (checkLifeEventBit(MARRIED) || npcAffection[ANN] < 120)) {
                
                setLifeEventBit(ANN_CLIFF_MARRIED);
                setSpecialDialogueBit(0xE);
                gCutsceneIndex = 0xD6;
                setEntrance(0);
                loadCutscene(0);
                set = TRUE;
                gCutsceneFlags |= (2 | 4);

            }
            
            if (!set && !checkLifeEventBit(KAREN_KAI_MARRIED) && (!checkLifeEventBit(MARRIED) || gWife != KAREN) && npcAffection[KAI] >= 200 && (checkLifeEventBit(MARRIED) || npcAffection[KAREN] < 120) && checkLifeEventBit(0x96) && !checkLifeEventBit(KAI_GONE) && !checkLifeEventBit(KAREN_GONE)) {
                
                setLifeEventBit(KAREN_KAI_MARRIED);
                setSpecialDialogueBit(0xF);
                gCutsceneIndex = 0xD9;
                setEntrance(0);
                loadCutscene(0);
                set = TRUE;
                gCutsceneFlags |= (2 | 4);

            }
            
        }

        // animal funeral
        if (!set && checkLifeEventBit(ANIMAL_DIED) && !checkDailyEventBit(FESTIVAL_DAY_TYPE_2)) {
            
            toggleLifeEventBit(ANIMAL_DIED);
            setLifeEventBit(0x50);
            setEntrance(0x3E);
            gCutsceneIndex = 0x20B;
            loadCutscene(0);
            gCutsceneFlags |= (2 | 4);
            set = TRUE;
            D_8013DC2E += adjustValue(D_8013DC2E, 1, 999);

        }

        // sick day
        if (!set && checkFatigueLevel() == 3) {
            
            gPlayer.fatigueCounter = 0;
            
            setEntrance(0xA);

            temp = getBacholeretteWithHighestAffection(120);

            if (temp) {

                gCutsceneIndex = 0;

                switch (temp) {

                    case 1:

                        if (!checkLifeEventBit(MARRIED) || gWife != MARIA) {
                            if (!checkLifeEventBit(MARRIED)) {                            
                                if (!checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
                                    gCutsceneIndex = MARIA_VISIT_PLAYER_SICK;
                                }
                            }
                            
                            break;
                        } 
                        
                        gCutsceneIndex = MARIA_VISIT_PLAYER_SICK;
                        break;
                
                        
                    case 2:

                        if (!checkLifeEventBit(MARRIED) || gWife != POPURI) {
                            if (!checkLifeEventBit(MARRIED)) {                            
                                if (!checkLifeEventBit(POPURI_GRAY_MARRIED)) {
                                    gCutsceneIndex = 0xA;
                                }
                            }
                            break;
                            
                        } 
                        
                        gCutsceneIndex = 0xA;
                        break;
                    
                    case 3:

                        if (!checkLifeEventBit(MARRIED) || gWife != ELLI) {
                            if (!checkLifeEventBit(MARRIED)) {                            
                                if (!checkLifeEventBit(ELLI_JEFF_MARRIED)) {
                                    gCutsceneIndex = 0x11;
                                }
                            }

                            break;
                        } 

                        gCutsceneIndex = 0x11;
                        break;
                    
                    case 4: 

                        if (!checkLifeEventBit(MARRIED) || gWife != ANN) {
                            if (!checkLifeEventBit(MARRIED)) {                            
                                if (!checkLifeEventBit(ANN_CLIFF_MARRIED)) {
                                    gCutsceneIndex = 0x18;
                                }
                            }

                            break;
                        } 

                        gCutsceneIndex = 0x18;
                        break;

                    case 5:

                        if (!checkLifeEventBit(MARRIED) || gWife != KAREN) {
                            if (!checkLifeEventBit(MARRIED)) {                            
                                if (!checkLifeEventBit(KAREN_KAI_MARRIED)) {
                                    gCutsceneIndex = 0x1F;
                                }
                            }
                            break;
                        }

                        gCutsceneIndex = 0x1F;
                        break;

                    default:
                        break;

                }

            } else {

                if (npcAffection[KENT] >= 50) {
                    gCutsceneIndex = 0;
                } else {
                    gCutsceneIndex = 0x33;
                }
            }
            
            loadCutscene(0);
            gCutsceneFlags |= 2;
            set = 2;

            gSickDays += adjustValue(gSickDays, 1, 999);

        }
    }

    if (set) {
        gWeather = SUNNY;
    }

    if (set == 1) {
        // take care of all animals
        func_80087D5C();
    }

    return result;
}

//INCLUDE_ASM("asm/nonmatchings/game/setCutscenes", func_800A87C4);

// dreams
u16 func_800A87C4(void) {

    bool set = FALSE;

    if (!checkLifeEventBit(0x96) && checkSpecialDialogueBit(0x14B) && gSeason == AUTUMN && gDayOfMonth == 6) {
        gCutsceneIndex = 0x28;
        loadCutscene(0);
        set = TRUE;
        gCutsceneFlags |= 2;
    }

    if (!set) {
        if (!checkLifeEventBit(0x63) && npcAffection[MARIA] >= 160 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            gCutsceneIndex = 0x29;
            loadCutscene(0);
            set = TRUE;
        }
    }

    if (!set) { 
        if (!checkLifeEventBit(0x64) && npcAffection[POPURI] >= 160 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            gCutsceneIndex = 0x2A;
            loadCutscene(0);
            set = TRUE;
        }
    }
    
    if (!set) { 
        if (!checkLifeEventBit(0x65) && npcAffection[ELLI] >= 160 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            gCutsceneIndex = 0x2B;
            loadCutscene(0);
            set = TRUE;
        }
    }

    if (!set) { 
        if (!checkLifeEventBit(0x66) && npcAffection[ANN] >= 160 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            gCutsceneIndex = 0x2C;
            loadCutscene(0);
            set = TRUE;
        }
    }

    if (!set) { 
        if (!checkLifeEventBit(0x67) && npcAffection[KAREN] >= 160 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            gCutsceneIndex = 0x2D;
            loadCutscene(0);
            set = TRUE;
        }
    }

    if (!set) { 
        if (!checkLifeEventBit(0xD8) && checkLifeEventBit(MARRIED) && checkLifeEventBit(1) && !checkDailyEventBit(0x52) && gWife == MARIA && (29 < gBabyAge && gBabyAge < 120) && !getRandomNumberInRange(0, 10)) {
            setLifeEventBit(0xD8);
            gCutsceneIndex = 9;
            loadCutscene(0);
            set = TRUE;
            gCutsceneFlags |= (2 | 4);
        }
    }

    if (!set) { 
        if (!checkLifeEventBit(0xD8) && checkLifeEventBit(MARRIED) && checkLifeEventBit(1) && !checkDailyEventBit(0x52) && gWife == POPURI && (29 < gBabyAge && gBabyAge < 120) && !getRandomNumberInRange(0, 10)) {
            setLifeEventBit(0xD8);
            gCutsceneIndex = 0x10;
            loadCutscene(0);
            set = TRUE;
            gCutsceneFlags |= (2 | 4);
        }
    }

    if (!set) { 
        if (!checkLifeEventBit(0xD8) && checkLifeEventBit(MARRIED) && checkLifeEventBit(1) && !checkDailyEventBit(0x52) && gWife == ELLI && (29 < gBabyAge && gBabyAge < 120) && !getRandomNumberInRange(0, 10)) {
            setLifeEventBit(0xD8);
            gCutsceneIndex = 0x17;
            loadCutscene(0);
            set = TRUE;
            gCutsceneFlags |= (2 | 4);
        }
    }

    if (!set) { 
        if (!checkLifeEventBit(0xD8) && checkLifeEventBit(MARRIED) && checkLifeEventBit(1) && !checkDailyEventBit(0x52) && gWife == ANN && (29 < gBabyAge && gBabyAge < 120) && !getRandomNumberInRange(0, 10)) {
            setLifeEventBit(0xD8);
            gCutsceneIndex = 0x1E;
            loadCutscene(0);
            set = TRUE;
            gCutsceneFlags |= (2 | 4);
        }
    }

    if (!set) { 
        if (!checkLifeEventBit(0xD8) && checkLifeEventBit(MARRIED) && checkLifeEventBit(1) && !checkDailyEventBit(0x52) && gWife == KAREN && (29 < gBabyAge && gBabyAge < 120) && !getRandomNumberInRange(0, 10)) {
            setLifeEventBit(0xD8);
            gCutsceneIndex = 0x25;
            loadCutscene(0);
            set = TRUE;
            gCutsceneFlags |= (2 | 4);
        }
    }
    
    return set;
    
}

#ifdef PERMUTER
void func_800A8F74(void) {
    // ? var_a0_2;
    // ? var_a1;
    // ?* var_a2;
    // ?* var_a2_10;
    // ?* var_a2_11;
    // ?* var_a2_12;
    // ?* var_a2_13;
    // ?* var_a2_2;
    // ?* var_a2_3;
    // ?* var_a2_4;
    // ?* var_a2_5;
    // ?* var_a2_6;
    // ?* var_a2_7;
    // ?* var_a2_8;
    // ?* var_a2_9;
    // ?* var_a3;
    // ?* var_a3_10;
    // ?* var_a3_11;
    // ?* var_a3_12;
    // ?* var_a3_13;
    // ?* var_a3_2;
    // ?* var_a3_3;
    // ?* var_a3_4;
    // ?* var_a3_5;
    // ?* var_a3_6;
    // ?* var_a3_7;
    // ?* var_a3_8;
    // ?* var_a3_9;
    s16 var_a2_14;
    s16 var_a3_14;
    s32 temp_a0;
    s32 temp_f2;
    s32 temp_f2_2;
    s32 temp_v1_4;
    s32 var_a0;
    s32 var_a1_2;
    s32 var_s0;
    s32 var_s0_2;
    s32 var_s2;
    s32 var_v0_18;
    s32 var_v0_19;
    s32 var_v0_22;
    s32 var_v0_23;
    s32 var_v0_24;
    s32 var_v0_25;
    s32 var_v0_26;
    s32 var_v0_27;
    s32 var_v0_28;
    s8 var_v0;
    s8 var_v0_2;
    u16 var_v0_10;
    u16 var_v0_11;
    u16 var_v0_12;
    u16 var_v0_13;
    u16 var_v0_14;
    u16 var_v0_15;
    u16 var_v0_16;
    u16 var_v0_17;
    u16 var_v0_20;
    u16 var_v0_21;
    u16 var_v0_3;
    u16 var_v0_4;
    u16 var_v0_5;
    u16 var_v0_6;
    u16 var_v0_7;
    u16 var_v0_8;
    u16 var_v0_9;
    u32 temp_v1_5;
    u8 temp_s0_2;
    u8 temp_s1;
    u8 temp_v1_3;
    u8* temp_s3;
    u8* temp_v1;
    u8* temp_v1_2;
    u8* var_v1;
    void* temp_s0;
    void* temp_t0;
    void* temp_t0_10;
    void* temp_t0_11;
    void* temp_t0_12;
    void* temp_t0_13;
    void* temp_t0_2;
    void* temp_t0_3;
    void* temp_t0_4;
    void* temp_t0_5;
    void* temp_t0_6;
    void* temp_t0_7;
    void* temp_t0_8;
    void* temp_t0_9;

    temp_a0 = gCutsceneFlags;
    if (temp_a0 & 1) {
        if (gCutsceneCompletionFlags & 1) {
            gCutsceneFlags = temp_a0 & ~1;
            switch (D_8018981C) {                   /* switch 1 */
            case 1:                                 /* switch 1 */
                toggleDailyEventBit(1);
                switch (D_801C4216) {               /* switch 2 */
                case 0:                             /* switch 2 */
                    var_v0 = 2;
block_40:
                    gNamingScreenIndex = var_v0;
                default:                            /* switch 2 */
                    break;
                case 1:                             /* switch 2 */
                    setLifeEventBit(0x3A);
                    var_v0 = 3;
                    goto block_40;
                case 2:                             /* switch 2 */
                    setLifeEventBit(0x36);
                    gNamingScreenIndex = 5;
                    var_v0_2 = initializeNewFarmAnimal(1, 0);
block_11:
                    D_801FC155 = var_v0_2;
                    break;
                case 3:                             /* switch 2 */
                    setLifeEventBit(0x34);
                    gNamingScreenIndex = 6;
                    var_v0_2 = initializeNewFarmAnimal(4, 0);
                    goto block_11;
                case 4:                             /* switch 2 */
                    setLifeEventBit(0x32);
                    gNamingScreenIndex = 7;
                    // initialize chicken
                    var_v0_2 = func_80087F28(2, 0xFF);
                    goto block_11;
                }
                func_8005C940(1, NAMING_SCREEN);
                break;
            case 7:                                 /* switch 1 */
            case 8:                                 /* switch 1 */
                if (gCutsceneIndex != 0xAA) {
                    if ((s32) gCutsceneIndex < 0xAB) {
                        if ((s32) gCutsceneIndex < 0x83) {
                            if ((s32) gCutsceneIndex < 0x81) {
                                var_a0 = 0;
                                if (gCutsceneIndex != 0x71) {

                                } else {
block_105:
                                    setEntrance(var_a0);
                                    setMainLoopCallbackFunctionIndex(MAP_LOAD);
                                }
                            } else {
                                goto block_27;
                            }
                        } else if (gCutsceneIndex != 0x8B) {

                        } else {
                            goto block_27;
                        }
                    } else if ((s32) gCutsceneIndex < 0xF4) {
                        if ((s32) gCutsceneIndex < 0xEF) {

                        } else {
block_27:
                            if (gCutsceneCompletionFlags & 0x40000000) {
                                goto block_142;
                            }
                        }
                    } else if ((s32) gCutsceneIndex < 0x101) {
                        if ((s32) gCutsceneIndex < 0xFA) {

                        } else {
                        case 17:                    /* switch 1 */
                        case 19:                    /* switch 1 */
                        case 22:                    /* switch 1 */
                        case 23:                    /* switch 1 */
                        case 25:                    /* switch 1 */
                        case 26:                    /* switch 1 */
                        case 28:                    /* switch 1 */
                        case 29:                    /* switch 1 */
                        case 0x130:                 /* switch 4 */
                        case 0x131:                 /* switch 4 */
                        case 0x293:                 /* switch 5 */
                        case 0x195:                 /* switch 6 */
                        case 0x196:                 /* switch 6 */
                        case 0x197:                 /* switch 6 */
                        case 0x198:                 /* switch 6 */
                        case 0x199:                 /* switch 6 */
                        case 0x1A2:                 /* switch 6 */
                        case 0x1A4:                 /* switch 6 */
                        case 0x1AD:                 /* switch 6 */
                        case 0x1AE:                 /* switch 6 */
                        case 0x1AF:                 /* switch 6 */
                        case 0x1B3:                 /* switch 6 */
                        case 0x1B4:                 /* switch 6 */
block_142:
                            setMainLoopCallbackFunctionIndex(END_OF_FESTIVAL_DAY_1);
                        }
                    }
                } else {
                    func_8009B6B8(temp_a0);
                    func_8009B6B8();
                    func_8009B6B8();
                    func_8009B6B8();
                    func_8009B6B8();
                    func_8009B6B8();
                }
                break;
            case 9:                                 /* switch 1 */
                switch (gCutsceneIndex) {           /* switch 3 */
                case 0x1:                           /* switch 3 */
                case 0x2:                           /* switch 3 */
                    setLifeEventBit(0x61);
                    break;
                case 0x26:                          /* switch 3 */
                case 0x27:                          /* switch 3 */
                    setLifeEventBit(0x62);
                    break;
                case 0x28:                          /* switch 3 */
                    setLifeEventBit(0x96);
                case 0x0:                           /* switch 3 */
                case 0x3:                           /* switch 3 */
                case 0x5:                           /* switch 3 */
                case 0x9:                           /* switch 3 */
                case 0xA:                           /* switch 3 */
                case 0xC:                           /* switch 3 */
                case 0x10:                          /* switch 3 */
                case 0x11:                          /* switch 3 */
                case 0x13:                          /* switch 3 */
                case 0x17:                          /* switch 3 */
                case 0x18:                          /* switch 3 */
                case 0x1A:                          /* switch 3 */
                case 0x1E:                          /* switch 3 */
                case 0x1F:                          /* switch 3 */
                case 0x21:                          /* switch 3 */
                case 0x25:                          /* switch 3 */
                case 0x33:                          /* switch 3 */
                case 0x190:                         /* switch 6 */
                case 0x191:                         /* switch 6 */
                case 0x192:                         /* switch 6 */
                case 0x193:                         /* switch 6 */
                case 0x194:                         /* switch 6 */
                case 0x20B:                         /* switch 12 */
block_108:
                    setMainLoopCallbackFunctionIndex(END_OF_DAY_1);
                    break;
                case 0x29:                          /* switch 3 */
                    setLifeEventBit(0x63);
                    goto block_108;
                case 0x2A:                          /* switch 3 */
                    setLifeEventBit(0x64);
                    goto block_108;
                case 0x2B:                          /* switch 3 */
                    setLifeEventBit(0x65);
                    goto block_108;
                case 0x2C:                          /* switch 3 */
                    setLifeEventBit(0x66);
                    goto block_108;
                case 0x2D:                          /* switch 3 */
                    setLifeEventBit(0x67);
                    goto block_108;
                case 0x4:                           /* switch 3 */
                case 0xB:                           /* switch 3 */
                case 0x12:                          /* switch 3 */
                case 0x19:                          /* switch 3 */
                case 0x20:                          /* switch 3 */
                    var_v0 = 4;
                    if ((u8) gHour >= 6) {
                        goto block_40;
                    }
                    goto block_108;
                }
                break;
            case 10:                                /* switch 1 */
                if ((gCutsceneIndex == 0x324) && (gCutsceneCompletionFlags & 0x40000000)) {
                    gHour += 2;
                    setEntrance(RANCH_FROM_RANCH_STORE);
                    setMainLoopCallbackFunctionIndex(MAP_LOAD);
                }
                /* fallthrough */
            case 11:                                /* switch 1 */
                switch (gCutsceneIndex) {           /* switch 4 */
                case 0x12E:                         /* switch 4 */
                    if (gCutsceneCompletionFlags & 0x40000000) {
                        var_a0 = 0x34;
block_104:
                        gHour += 2;
                        goto block_105;
                    }
                    break;
                case 0x157:                         /* switch 4 */
                    if (gCutsceneCompletionFlags & 0x40000000) {
                        var_a0 = 0x35;
                        goto block_104;
                    }
                    break;
                case 0x13E:                         /* switch 4 */
                    // store tool
                    func_80065BCC(FISHING_POLE);
                    break;
                case 0x144:                         /* switch 4 */
                case 0x148:                         /* switch 4 */
                    setDailyEventBit(0x2C);
                    break;
                case 0x147:                         /* switch 4 */
                    if ((gSeason == AUTUMN) && (gDayOfMonth == 27)) {
                        setLifeEventBit(BRIDGE_COMPLETED);
                    }
                    break;
                case 0x14A:                         /* switch 4 */
                    if ((gSeason == WINTER) && (gDayOfMonth == 16)) {
                        setLifeEventBit(HOT_SPRINGS_COMPLETED);
                        setDailyEventBit(0x5B);
                    }
                    clearForagableObjects(TOP_OF_MOUNTAIN_1);
                    break;
                case 0x150:                         /* switch 4 */
                    toggleDailyEventBit(0x47);
                    setLifeEventBit(0x5E);
                    break;
                case 0x151:                         /* switch 4 */
                    toggleDailyEventBit(0x48);
                    setLifeEventBit(0x5F);
                    break;
                case 0x155:                         /* switch 4 */
                case 0x158:                         /* switch 4 */
                    toggleDailyEventBit(0x45);
                    setDailyEventBit(0x46);
                    break;
                case 0x159:                         /* switch 4 */
                    setSpecialDialogueBit(0x14B);
                    goto block_142;
                }
                break;
            case 12:                                /* switch 1 */
                switch (gCutsceneIndex) {           /* switch 5 */

                    // get horse at ranch
                case 0x28A:                         /* switch 5 */
                case 0x299:                         /* switch 5 */
                    gNamingScreenIndex = 3;
                    func_8005C940(1, NAMING_SCREEN);
                    setLifeEventBit(HAVE_HORSE);
                    break;
                case 0x28B:                         /* switch 5 */
                    func_80056030(0);
                    setMainLoopCallbackFunctionIndex(4);
                    initializeHorse();
                    break;
                case 0x297:                         /* switch 5 */
                    var_a0 = 0x4C;
                    goto block_105;
                case 0x298:                         /* switch 5 */
                    // Ann
                    (&npcAffection)[3] = 120;
block_107:
                    toggleLifeEventBit(WIFE_LEFT);
                    goto block_108;
                }
                break;
            case 13:                                /* switch 1 */
                switch (gCutsceneIndex) {           /* switch 11; irregular */
                case 0x2BD:                         /* switch 11 */
                    if (gCutsceneCompletionFlags & 0x40000000) {
                        var_a0 = 0x2F;
                        goto block_104;
                    }
                    break;
                case 0x2C0:                         /* switch 11 */
                    var_a0 = 0x50;
                    goto block_105;
                case 0x2C1:                         /* switch 11 */
                    if (gCutsceneCompletionFlags & 0x40000000) {
                        toggleDailyEventBit(0x4F);
                        var_a0 = 0x30;
                        goto block_105;
                    }
                    break;
                case 0x2C6:                         /* switch 11 */
                    // Karen
                    (&npcAffection)[4] = 0x78;
                    goto block_107;
                }
                break;
            case 14:                                /* switch 1 */
                switch (gCutsceneIndex) {           /* switch 6 */
                case 0x19B:                         /* switch 6 */
                    setEntrance(0x1F);
                    setMainLoopCallbackFunctionIndex(MAP_LOAD);
                    D_802373A8 += adjustValue(D_802373A8, -1, 1);
                    removeTool(BLUE_MIST_SEEDS);
                    blueMistFlowerPlot = 0xB3;
                    D_8016FE6F = 0xB3;
                    break;
                case 0x1A0:                         /* switch 6 */
                    setLifeEventBit(0x31);
                    setSpecialDialogueBit(0x43);
                    goto block_142;
                case 0x1AC:                         /* switch 6 */
                    if (gCutsceneCompletionFlags & 2) {

                    } else {
                        goto block_142;
                    }
                    break;
                case 0x1B5:                         /* switch 6 */
                    // flower shop entrance
                    var_a0 = 0x57;
                    goto block_105;
                case 0x1B6:                         /* switch 6 */
                    var_a0 = 0x5A;
                    goto block_105;
                case 0x1B7:                         /* switch 6 */
                    // Popuri
                    (&npcAffection)[1] = 120;
                    goto block_107;
                case 0x1B8:                         /* switch 6 */
                    // Elli
                    (&npcAffection)[2] = 120;
                    goto block_107;
                }
                break;
            case 15:                                /* switch 1 */
                switch (gCutsceneIndex) {           /* switch 12; irregular */
                case 0x20C:                         /* switch 12 */
                    var_a0 = 0x66;
                    goto block_105;
                case 0x20E:                         /* switch 12 */
                    if (gCutsceneCompletionFlags & 0x40000000) {
                        var_a0 = 0x41;
                        goto block_104;
                    }
                    break;
                case 0x211:                         /* switch 12 */
                    npcAffection[MARIA] = 120;
                    goto block_107;
                }
                break;
            case 16:                                /* switch 1 */
                if ((gCutsceneIndex != 0x2EE) && (gCutsceneIndex != 0x2F4)) {

                } else {
                    goto block_142;
                }
                break;
            case 18:                                /* switch 1 */
                if (((s32) gCutsceneIndex < 0x388) && ((s32) gCutsceneIndex >= 0x386)) {
                    if (*(&overlayScreenStrings + 0x6D) == *(&overlayScreenStrings + 0x6C)) {
                        if (((D_8018A72E - 1) == 2) && ((u8) (&horseInfo)[0x1E] >= 3)) {
                            (&horseInfo)[0x1E] = 2;
                        }
                        if (((D_8018A72D - 1) == 2) && ((u8) (&horseInfo)[0x1E] >= 2)) {
                            (&horseInfo)[0x1E] = 1;
                        }
                        if ((D_8018A72C - 1) == 2) {
                            var_v1 = &horseInfo + 0x1E;
block_139:
                            if (*var_v1 ) {
                                *var_v1 = 0;
                            }
                        }
                    }
block_141:
                    setMainLoopCallbackFunctionIndex(HORSE_RACE_RESULTS_LOAD);
                    gCutsceneCompletionFlags = 0;
                }
                break;
            case 20:                                /* switch 1 */
                if (gCutsceneIndex != 0x3E9) {

                } else {
                    goto block_142;
                }
                break;
            case 21:                                /* switch 1 */
                if (gCutsceneIndex != 0x258) {

                } else {
                    goto block_142;
                }
                break;
            case 24:                                /* switch 1 */
                if (checkLifeEventBit(0x91)) {
                    *(&gFarmAnimals + 0x2B + (D_80189054 * 0x30)) = (s8) D_80180710;
                }
                goto block_142;
            case 27:                                /* switch 1 */
                if (gCutsceneIndex == 0x516) {
                    if (*(&overlayScreenStrings + 0x6D) == *(&overlayScreenStrings + 0x6C)) {
                        if (D_8018A72E == 1) {
                            temp_v1 = &dogInfo + 0x1C;
                            if ((u8) *temp_v1 >= 3) {
                                *temp_v1 = 2;
                            }
                        }
                        if (D_8018A72D == 1) {
                            temp_v1_2 = &dogInfo + 0x1C;
                            if ((u8) *temp_v1_2 >= 2) {
                                *temp_v1_2 = 1;
                            }
                        }
                        if (D_8018A72C == 1) {
                            var_v1 = &dogInfo + 0x1C;
                            goto block_139;
                        }
                    }
                    goto block_141;
                }
                break;
            case 31:                                /* switch 1 */
                if (gCutsceneIndex == 0x611) {
                    // fontPalette1
                    nuPiReadRom(&D_E13770, 0x8030A000, &D_E137A0 - &D_E13770);
                    func_8003F464(0, 0xE, 0xE, 0x802FF000, 0x8030A000);
                    func_8003F360(0, -4, 0);
                    setMessageBoxSfx(0, 0x57, 8, 1);
                    var_a0_2 = 0x5AA;
                    if (D_80180710 == 0) {
                        // show pink overlay message
                        func_8005B09C(23);
                    } else {
block_153:
                        var_a1 = 0x61;
block_154:
                        func_80055F08(var_a0_2, var_a1, 1);
                    }
                }
                break;
            case 32:                                /* switch 1 */
                var_a0_2 = 0x5DE;
                if (gCutsceneIndex == 0x5DD) {
                    var_a1 = 0x1A;
                } else {

                    // memcpy
                    var_a3 = &farmFieldTiles;
                    var_a2 = &D_80113580;
                    temp_t0 = &D_80113580 + 0x1E0;
                    if (((s32) &D_80113580 | (s32) &farmFieldTiles) & 3) {
                        do {
                            var_a3->unk0 = (unaligned s32) var_a2->unk0;
                            var_a3->unk4 = (unaligned s32) var_a2->unk4;
                            var_a3->unk8 = (unaligned s32) var_a2->unk8;
                            var_a3->unkC = (unaligned s32) var_a2->unkC;
                            var_a2 += 0x10;
                            var_a3 += 0x10;
                        } while (var_a2 != temp_t0);
                        var_a0_2 = 0x5AA;
                    } else {
                        do {
                            var_a3->unk0 = (s32) var_a2->unk0;
                            var_a3->unk4 = (s32) var_a2->unk4;
                            var_a3->unk8 = (s32) var_a2->unk8;
                            var_a3->unkC = (s32) var_a2->unkC;
                            var_a2 += 0x10;
                            var_a3 += 0x10;
                        } while (var_a2 != temp_t0);
                        var_a0_2 = 0x5AA;
                    }


                    goto block_153;
                }
                goto block_154;
            case 35:                                /* switch 1 */
                if ((gCutsceneIndex == 0x640) && !(gCutsceneCompletionFlags & 2)) {

                    // memcpy
                    var_a3_2 = &farmFieldTiles;
                    var_a2_2 = &D_80113580;
                    temp_t0_2 = &D_80113580 + 0x1E0;
                    if (((s32) &D_80113580 | (s32) &farmFieldTiles) & 3) {
                        do {
                            var_a1_2 = (unaligned s32) var_a2_2->unkC;
                            var_a3_2->unk0 = (unaligned s32) var_a2_2->unk0;
                            var_a3_2->unk4 = (unaligned s32) var_a2_2->unk4;
                            var_a3_2->unk8 = (unaligned s32) var_a2_2->unk8;
                            var_a3_2->unkC = var_a1_2;
                            var_a2_2 += 0x10;
                            var_a3_2 += 0x10;
                        } while (var_a2_2 != temp_t0_2);
                    } else {
                        do {
                            var_a1_2 = var_a2_2->unkC;
                            var_a3_2->unk0 = (s32) var_a2_2->unk0;
                            var_a3_2->unk4 = (s32) var_a2_2->unk4;
                            var_a3_2->unk8 = (s32) var_a2_2->unk8;
                            var_a3_2->unkC = var_a1_2;
                            var_a2_2 += 0x10;
                            var_a3_2 += 0x10;
                        } while (var_a2_2 != temp_t0_2);
                    }

                
                    initializeTitleScreen(1, var_a1_2, var_a2_2, var_a3_2);
                
                }
                break;
            }
            if ((gCutsceneCompletionFlags & 0x800) && (D_8018981C == 0x1E)) {
                func_80046BB8();
                deactivateSprites();
                func_8003C504(0);
                initializeTitleScreen(0);
            }
            if (gCutsceneCompletionFlags & 2) {
                updateAnimalCoordinates();
                deactivateAnimalEntities();
                if (gCutsceneIndex != 0x1AC) {
                    if ((s32) gCutsceneIndex < 0x1AD) {
                        if (gCutsceneIndex != 0xCE) {
                            if ((s32) gCutsceneIndex < 0xCF) {
                                switch (gCutsceneIndex) { /* switch 18; irregular */
                                case 0x1:           /* switch 18 */
                                    setEntrance(0x3E);
                                    var_v0_3 = 2;
                                    goto block_372;
                                case 0x26:          /* switch 18 */
                                    setEntrance(HOUSE_EXIT);
                                    var_v0_4 = 0x27;
                                    goto block_374;
                                case 0x93:          /* switch 18 */
                                    deactivateSprites();
                                    deactivateGlobalSprites();
                                    initializeCutsceneExecutors();
                                    deactivateNPCEntities();
                                    deactivateAnimalEntities();
                                    func_800D51B0();
                                    initializeEntityInstances(1);
                                    setEntrance(SPRING_ENTER);
                                    var_v0_5 = 0xFA;
                                    goto block_322;
                                case 0xC5:          /* switch 18 */
                                case 0xB8:          /* switch 18 */
                                case 0xAD:          /* switch 18 */
                                case 0x96:          /* switch 18 */
                                case 0xA1:          /* switch 18 */
                                    deactivateSprites();
                                    deactivateGlobalSprites();
                                    initializeCutsceneExecutors();
                                    deactivateNPCEntities();
                                    deactivateAnimalEntities();
                                    func_800D51B0();
                                    initializeEntityInstances(1);
                                    setEntrance(0x1A);
                                    var_v0_6 = 0xFB;
                                    goto block_376;
                                case 0xB9:          /* switch 18 */
                                case 0xC6:          /* switch 18 */
                                case 0xAE:          /* switch 18 */
                                case 0xA2:          /* switch 18 */
                                case 0x97:          /* switch 18 */
                                    deactivateSprites();
                                    deactivateGlobalSprites();
                                    initializeCutsceneExecutors();
                                    deactivateNPCEntities();
                                    deactivateAnimalEntities();
                                    func_800D51B0();
                                    initializeEntityInstances(1);
                                    setEntrance(0x25);
                                    var_v0_7 = 0xFC;
                                    goto block_305;
                                case 0x9E:          /* switch 18 */
                                    deactivateSprites();
                                    deactivateGlobalSprites();
                                    initializeCutsceneExecutors();
                                    deactivateNPCEntities();
                                    deactivateAnimalEntities();
                                    func_800D51B0();
                                    initializeEntityInstances(1);
                                    setEntrance(SPRING_ENTER);
                                    var_v0_5 = 0xFD;
                                    goto block_322;
                                case 0xA9:          /* switch 18 */
                                    deactivateSprites();
                                    deactivateGlobalSprites();
                                    initializeCutsceneExecutors();
                                    deactivateNPCEntities();
                                    deactivateAnimalEntities();
                                    func_800D51B0();
                                    initializeEntityInstances(1);
                                    setEntrance(SPRING_ENTER);
                                    var_v0_5 = 0xFE;
                                    goto block_322;
                                case 0xB5:          /* switch 18 */
                                    deactivateSprites();
                                    deactivateGlobalSprites();
                                    initializeCutsceneExecutors();
                                    deactivateNPCEntities();
                                    deactivateAnimalEntities();
                                    func_800D51B0();
                                    initializeEntityInstances(1);
                                    setEntrance(SPRING_ENTER);
                                    var_v0_5 = 0xFF;
                                    goto block_322;
                                case 0xC1:          /* switch 18 */
                                    deactivateSprites();
                                    deactivateGlobalSprites();
                                    initializeCutsceneExecutors();
                                    deactivateNPCEntities();
                                    deactivateAnimalEntities();
                                    func_800D51B0();
                                    initializeEntityInstances(1);
                                    setEntrance(0x2E);
                                    var_v0_8 = 0x100;
                                    goto block_347;
                                case 0xCB:          /* switch 18 */
                                    deactivateSprites();
                                    deactivateGlobalSprites();
                                    initializeCutsceneExecutors();
                                    deactivateNPCEntities();
                                    deactivateAnimalEntities();
                                    func_800D51B0();
                                    initializeEntityInstances(1);
                                    setEntrance(0x32);
                                    var_v0_9 = 0x195;
                                    goto block_324;
                                }
                            } else if (gCutsceneIndex != 0x144) {
                                if ((s32) gCutsceneIndex < 0x145) {
                                    switch (gCutsceneIndex) { /* switch 17; irregular */
                                    case 0xD0:      /* switch 17 */
                                        deactivateSprites();
                                        deactivateGlobalSprites();
                                        initializeCutsceneExecutors();
                                        deactivateNPCEntities();
                                        deactivateAnimalEntities();
                                        func_800D51B0();
                                        initializeEntityInstances(1);
                                        setEntrance(0x32);
                                        var_v0_9 = 0x197;
                                        goto block_324;
                                    case 0xD6:      /* switch 17 */
                                        deactivateSprites();
                                        deactivateGlobalSprites();
                                        initializeCutsceneExecutors();
                                        deactivateNPCEntities();
                                        deactivateAnimalEntities();
                                        func_800D51B0();
                                        initializeEntityInstances(1);
                                        setEntrance(0x32);
                                        var_v0_9 = 0x198;
                                        goto block_324;
                                    case 0xD9:      /* switch 17 */
                                        deactivateSprites();
                                        deactivateGlobalSprites();
                                        initializeCutsceneExecutors();
                                        deactivateNPCEntities();
                                        deactivateAnimalEntities();
                                        func_800D51B0();
                                        initializeEntityInstances(1);
                                        setEntrance(0x32);
                                        var_v0_9 = 0x199;
                                        goto block_324;
                                    default:        /* switch 17 */
                                        deactivateSprites();
                                        deactivateGlobalSprites();
                                        initializeCutsceneExecutors();
                                        deactivateNPCEntities();
                                        deactivateAnimalEntities();
                                        func_800D51B0();
                                        initializeEntityInstances(1);
                                        setEntrance(0x5F);
                                        gCutsceneIndex = 0x1AC;
                                        loadCutscene(0);
                                        func_8006E840(0x5F);
                                        break;
                                    case 0xF5:      /* switch 17 */
                                        setEntrance(0x25);
                                        var_v0_7 = 0x131;
block_305:
                                        gCutsceneIndex = var_v0_7;
                                        loadCutscene(0);
                                        func_8006E840(0x25);
                                        break;
                                    case 0xF8:      /* switch 17 */
                                        setEntrance(0x27);
                                        gCutsceneIndex = 0x2F4;
                                        loadCutscene(0);
                                        func_8006E840(0x27);
                                        break;
                                    }
                                } else {
                                    switch (gCutsceneIndex) { /* switch 16; irregular */
                                    case 0x146:     /* switch 16 */
                                        setEntrance(0x1A);
                                        var_v0_6 = 0x147;
                                        goto block_376;
                                    case 0x148:     /* switch 16 */
                                        setEntrance(0x23);
                                        var_v0_10 = 0x149;
                                        goto block_310;
                                    case 0x149:     /* switch 16 */
                                        setEntrance(0x1A);
                                        var_v0_6 = 0x14A;
                                        goto block_376;
                                    case 0x158:     /* switch 16 */
                                        setEntrance(0x2E);
                                        var_v0_8 = 0x159;
                                        goto block_347;
                                    case 0x15A:     /* switch 16 */
                                        D_801886D2 = getRandomNumberInRange(0, 9);
                                        D_80180710 = getRandomNumberInRange(0, 9);
                                        clearForagableObjects(MINE_2);
                                        setEntrance(0x71);
                                        gCutsceneIndex = 0x15B;
                                        loadCutscene(0);
                                        func_8006E840(0x71);
                                        break;
                                    default:        /* switch 16 */
                                        if (((u8) gHour >= 0x11) || ((u8) gPlayer < 2)) {
                                            gCutsceneIndex = 0x15D;
                                            // MINE_EXIT
                                            var_s0 = 0x47;
                                            goto block_444;
                                        }
                                        if ((u16) D_80189824 < 0xA) {
                                            D_801886D2 = getRandomNumberInRange(0, 9);
                                            D_80180710 = getRandomNumberInRange(0, 9);
                                        } else {
                                            D_80180710 = 0xFF;
                                            D_801886D2 = 0xFF;
                                        }
                                        gCutsceneIndex = 0x15C;
                                        clearForagableObjects(0x45);
                                        var_s0_2 = 0x71;
                                        goto block_445;
                                    case 0x19A:     /* switch 16 */
                                        setEntrance(SPRING_ENTER);
                                        var_v0_5 = 0x19B;
block_322:
                                        gCutsceneIndex = var_v0_5;
                                        loadCutscene(0);
                                        func_8006E840(SPRING_ENTER);
                                        break;
                                    case 0x1AA:     /* switch 16 */
                                        setEntrance(0x32);
                                        var_v0_9 = 0x1BB;
                                        goto block_324;
                                    }
                                }
                            } else {
                                setEntrance(0x23);
                                var_v0_10 = 0x146;
block_310:
                                gCutsceneIndex = var_v0_10;
                                loadCutscene(0);
                                func_8006E840(0x23);
                            }
                        } else {
                            deactivateSprites();
                            deactivateGlobalSprites();
                            initializeCutsceneExecutors();
                            deactivateNPCEntities();
                            deactivateAnimalEntities();
                            func_800D51B0();
                            initializeEntityInstances(1);
                            setEntrance(0x32);
                            var_v0_9 = 0x196;
block_324:
                            gCutsceneIndex = var_v0_9;
                            loadCutscene(0);
                            func_8006E840(0x32);
                        }
                    } else if ((s32) gCutsceneIndex < 0x386) {
                        if ((s32) gCutsceneIndex < 0x384) {
                            if ((s32) gCutsceneIndex < 0x214) {
                                if ((s32) gCutsceneIndex < 0x212) {
                                    switch (gCutsceneIndex) { /* switch 15; irregular */
                                    case 0x1BC:     /* switch 15 */
                                    case 0x1B9:     /* switch 15 */
                                        setEntrance(0x58);
                                        var_v0_11 = 0x1B7;
                                        if (gCutsceneIndex == 0x1B9) {
                                            var_v0_11 = 0x1B5;
                                        }
                                        gCutsceneIndex = var_v0_11;
                                        loadCutscene(0);
                                        func_8006E840(0x58);
                                        break;
                                    case 0x1BD:     /* switch 15 */
                                    case 0x1BA:     /* switch 15 */
                                        setEntrance(0x5C);
                                        var_v0_12 = 0x1B8;
                                        if (gCutsceneIndex == 0x1BA) {
                                            var_v0_12 = 0x1B6;
                                        }
                                        gCutsceneIndex = var_v0_12;
                                        loadCutscene(0);
                                        func_8006E840(0x5C);
                                        break;
                                    case 0x20D:     /* switch 15 */
                                        setEntrance(0x3E);
                                        var_v0_3 = 0x20E;
                                        goto block_372;
                                    case 0x20E:     /* switch 15 */
                                        setEntrance(0x41);
                                        func_8006E840(0x41);
                                        break;
                                    case 0x20F:     /* switch 15 */
                                        setEntrance(0x3E);
                                        var_v0_3 = 0x210;
                                        goto block_372;
                                    }
                                } else {
                                    setEntrance(0x67);
                                    var_v0_13 = 0x211;
                                    if (gCutsceneIndex == 0x212) {
                                        var_v0_13 = 0x20C;
                                    }
                                    gCutsceneIndex = var_v0_13;
                                    loadCutscene(0);
                                    func_8006E840(0x67);
                                }
                            } else if ((s32) gCutsceneIndex < 0x2C5) {
                                if ((s32) gCutsceneIndex < 0x2C3) {
                                    if (gCutsceneIndex != 0x2C1) {
                                        if ((s32) gCutsceneIndex < 0x2C2) {
                                            if ((s32) gCutsceneIndex < 0x29C) {
                                                if ((s32) gCutsceneIndex >= 0x29A) {
                                                    setEntrance(0x4D);
                                                    var_v0_14 = 0x298;
                                                    if (gCutsceneIndex == 0x29A) {
                                                        var_v0_14 = 0x297;
                                                    }
                                                    gCutsceneIndex = var_v0_14;
                                                    loadCutscene(0);
                                                    func_8006E840(0x4D);
                                                } else {
                                                    goto block_449;
                                                }
                                            } else {
                                                goto block_449;
                                            }
                                        } else {
                                            setEntrance(0x53);
                                            gCutsceneIndex = 0x2C3;
                                            loadCutscene(0);
                                            func_8006E840(0x53);
                                        }
                                    } else {
                                        gCutsceneIndex = 0x2C2;
                                        loadCutscene(0);
                                        func_8006E840(0x55);
                                    }
                                } else {
                                    toggleDailyEventBit(0x4F);
                                    setEntrance(0x2E);
                                    var_v0_8 = 0x2C5;
block_347:
                                    gCutsceneIndex = var_v0_8;
                                    loadCutscene(0);
                                    func_8006E840(0x2E);
                                }
                            } else if ((s32) gCutsceneIndex >= 0x2C7) {
                                if ((s32) gCutsceneIndex >= 0x2C9) {
                                    if (gCutsceneIndex != 0x324) {
                                        goto block_449;
                                    }
                                    setEntrance(0x2B);
                                    func_8006E840(0x2B);
                                } else {
                                    setEntrance(0x52);
                                    var_v0_15 = 0x2C6;
                                    if (gCutsceneIndex == 0x2C7) {
                                        var_v0_15 = 0x2C0;
                                    }
                                    gCutsceneIndex = var_v0_15;
                                    loadCutscene(0);
                                    func_8006E840(0x52);
                                }
                            } else {
block_449:
                                gCutsceneIndex = 0xFFFF;
                            }
                        } else {
                            if ((u8) gHour >= 0x11) {
                                setEntrance(0x61);
                                var_v0_16 = 0x388;
                                goto block_359;
                            }
                            var_v0_17 = 0x386;
                            if (*(&overlayScreenStrings + 0x6D) == *(&overlayScreenStrings + 0x6C)) {
                                gCutsceneIndex = 0x387;
                                D_80189824 = (u16) horseInfo.affection;
                                D_801886D2 = MULTU_HI(horseInfo.affection, 0xAAAAAAAB) & 0x1FE;
                                D_80180710 = (horseInfo.affection / 3) & 0xFF;
                            } else {
                                goto block_446;
                            }
                            goto block_447;
                        }
                    } else if (gCutsceneIndex != 0x5AD) {
                        if ((s32) gCutsceneIndex < 0x5AE) {
                            switch ((s32) gCutsceneIndex) { /* switch 14; irregular */
                            case 0x3E8:             /* switch 14 */
                                deactivateSprites();
                                deactivateGlobalSprites();
                                initializeCutsceneExecutors();
                                deactivateNPCEntities();
                                deactivateAnimalEntities();
                                func_800D51B0();
                                initializeEntityInstances(2);
                                setEntrance(0x61);
                                var_v0_16 = 0x3E9;
                                goto block_359;
                            default:                /* switch 14 */
                                if ((u8) gHour >= 0x11) {
                                    setEntrance(0x61);
                                    var_v0_16 = 0x517;
                                    goto block_359;
                                }
                                if (*(&overlayScreenStrings + 0x6D) == *(&overlayScreenStrings + 0x6C)) {
                                    var_v0_18 = D_801C3F78 | 0x20;
                                } else {
                                    var_v0_18 = D_801C3F78 & ~0x20;
                                }
                                D_801C3F78 = var_v0_18;
                                if (*(&overlayScreenStrings + 0x6E) == *(&overlayScreenStrings + 0x6C)) {
                                    var_v0_19 = D_801C3F78 | 0x40;
                                } else {
                                    var_v0_19 = D_801C3F78 & ~0x40;
                                }
                                D_801C3F78 = var_v0_19;
                                var_v0_17 = 0x516;
                                goto block_446;
                            case 0x516:             /* switch 14 */
                                setupPlayerEntity(0x6C, 1);
                                func_8006A2E8();
                                setEntitiesColor(0, 0, 0, 0);
                                setEntrance(0x6C);
                                gCutsceneIndex = 0x515;
                                loadCutscene(0);
                                func_8006E840(0x6C);
                                break;
                            case 0x5AA:             /* switch 14 */
                                setEntrance(0);
                                var_v0_20 = 0x5AB;
                                goto block_379;
                            case 0x5AB:             /* switch 14 */
                                setEntrance(0x14);
                                gCutsceneIndex = 0x5AC;
                                loadCutscene(0);
                                func_8006E840(0x14);
                                break;
                            }
                        } else {
                            switch (gCutsceneIndex) { /* switch 13; irregular */
                            default:                /* switch 13 */
                                var_v0_17 = 0x640;
block_446:
                                gCutsceneIndex = var_v0_17;
block_447:
                                loadCutscene(0);
                                break;
                            case 0x5AE:             /* switch 13 */
                                setEntrance(0);
                                var_v0_4 = 0x5AF;
block_374:
                                gCutsceneIndex = var_v0_4;
                                loadCutscene(0);
                                func_8006E840(0);
                                gCutsceneFlags = (gCutsceneFlags & ~0x10) | 8;
                                break;
                            case 0x60E:             /* switch 13 */
                                setEntrance(0x1A);
                                var_v0_6 = 0x60F;
block_376:
                                gCutsceneIndex = var_v0_6;
                                loadCutscene(0);
                                func_8006E840(0x1A);
                                break;
                            case 0x60F:             /* switch 13 */
                                setEntrance(0xA);
                                gCutsceneIndex = 0x610;
                                loadCutscene(0);
                                func_8006E840(0xA);
                                break;
                            case 0x610:             /* switch 13 */
                                setEntrance(0);
                                var_v0_20 = 0x611;
block_379:
                                gCutsceneIndex = var_v0_20;
                                loadCutscene(0);
                                func_8006E840(0);
                                break;
                            case 0x640:             /* switch 13 */
                                deactivateSprites();
                                deactivateGlobalSprites();
                                initializeCutsceneExecutors();
                                deactivateNPCEntities();
                                deactivateAnimalEntities();
                                func_800D51B0();
                                initializeEntityInstances(0);
                                if (D_80189824 == 0) {
                                    gSeason = SUMMER;

                                    // memcpy
                                    var_a3_3 = &farmFieldTiles;
                                    var_a2_3 = &D_80115030;
                                    temp_t0_3 = &D_80115030 + 0x1E0;
                                    if (((s32) &D_80115030 | (s32) &farmFieldTiles) & 3) {
                                        do {
                                            var_a3_3->unk0 = (unaligned s32) var_a2_3->unk0;
                                            var_a3_3->unk4 = (unaligned s32) var_a2_3->unk4;
                                            var_a3_3->unk8 = (unaligned s32) var_a2_3->unk8;
                                            var_a3_3->unkC = (unaligned s32) var_a2_3->unkC;
                                            var_a2_3 += 0x10;
                                            var_a3_3 += 0x10;
                                        } while (var_a2_3 != temp_t0_3);
                                        var_s0 = 0;
                                    } else {
                                        do {
                                            var_a3_3->unk0 = (s32) var_a2_3->unk0;
                                            var_a3_3->unk4 = (s32) var_a2_3->unk4;
                                            var_a3_3->unk8 = (s32) var_a2_3->unk8;
                                            var_a3_3->unkC = (s32) var_a2_3->unkC;
                                            var_a2_3 += 0x10;
                                            var_a3_3 += 0x10;
                                        } while (var_a2_3 != temp_t0_3);
                                        // HOUSE_EXIT
                                        var_s0 = 0;
                                    }
                                    var_v0_21 = 0x641;
                                    goto block_443;
                                }

                                if (D_80189824 == 1) {
                                    gSeason = SPRING;
                                    // HOUSE_EXIT
                                    var_s0 = 0;
                                    switch (D_801886D2) { /* switch 7 */
                                    case 0:         /* switch 7 */

                                        // memcpy
                                        var_a3_4 = &farmFieldTiles;
                                        var_a2_4 = &D_80115210;
                                        temp_t0_4 = &D_80115210 + 0x1E0;
                                        if (((s32) &D_80115210 | (s32) &farmFieldTiles) & 3) {
                                            do {
                                                var_a3_4->unk0 = (unaligned s32) var_a2_4->unk0;
                                                var_a3_4->unk4 = (unaligned s32) var_a2_4->unk4;
                                                var_a3_4->unk8 = (unaligned s32) var_a2_4->unk8;
                                                var_a3_4->unkC = (unaligned s32) var_a2_4->unkC;
                                                var_a2_4 += 0x10;
                                                var_a3_4 += 0x10;
                                            } while (var_a2_4 != temp_t0_4);
                                            var_v0_21 = 0x642;
                                        } else {
                                            do {
                                                var_a3_4->unk0 = (s32) var_a2_4->unk0;
                                                var_a3_4->unk4 = (s32) var_a2_4->unk4;
                                                var_a3_4->unk8 = (s32) var_a2_4->unk8;
                                                var_a3_4->unkC = (s32) var_a2_4->unkC;
                                                var_a2_4 += 0x10;
                                                var_a3_4 += 0x10;
                                            } while (var_a2_4 != temp_t0_4);
                                            var_v0_21 = 0x642;
                                        }

                                        goto block_443;
                                    case 1:         /* switch 7 */

                                        // memcpy
                                        var_a3_5 = &farmFieldTiles;
                                        var_a2_5 = &D_801153F0;
                                        temp_t0_5 = &D_801153F0 + 0x1E0;
                                        if (((s32) &D_801153F0 | (s32) &farmFieldTiles) & 3) {
                                            do {
                                                var_a3_5->unk0 = (unaligned s32) var_a2_5->unk0;
                                                var_a3_5->unk4 = (unaligned s32) var_a2_5->unk4;
                                                var_a3_5->unk8 = (unaligned s32) var_a2_5->unk8;
                                                var_a3_5->unkC = (unaligned s32) var_a2_5->unkC;
                                                var_a2_5 += 0x10;
                                                var_a3_5 += 0x10;
                                            } while (var_a2_5 != temp_t0_5);
                                            var_v0_21 = 0x643;
                                        } else {
                                            do {
                                                var_a3_5->unk0 = (s32) var_a2_5->unk0;
                                                var_a3_5->unk4 = (s32) var_a2_5->unk4;
                                                var_a3_5->unk8 = (s32) var_a2_5->unk8;
                                                var_a3_5->unkC = (s32) var_a2_5->unkC;
                                                var_a2_5 += 0x10;
                                                var_a3_5 += 0x10;
                                            } while (var_a2_5 != temp_t0_5);
                                            var_v0_21 = 0x643;
                                        }
                                        goto block_443;


                                    case 2:         /* switch 7 */

                                        // memcpy
                                        var_a3_6 = &farmFieldTiles;
                                        var_a2_6 = &D_801155D0;
                                        temp_t0_6 = &D_801155D0 + 0x1E0;
                                        if (((s32) &D_801155D0 | (s32) &farmFieldTiles) & 3) {
                                            do {
                                                var_a3_6->unk0 = (unaligned s32) var_a2_6->unk0;
                                                var_a3_6->unk4 = (unaligned s32) var_a2_6->unk4;
                                                var_a3_6->unk8 = (unaligned s32) var_a2_6->unk8;
                                                var_a3_6->unkC = (unaligned s32) var_a2_6->unkC;
                                                var_a2_6 += 0x10;
                                                var_a3_6 += 0x10;
                                            } while (var_a2_6 != temp_t0_6);
                                            var_v0_21 = 0x644;
                                        } else {
                                            do {
                                                var_a3_6->unk0 = (s32) var_a2_6->unk0;
                                                var_a3_6->unk4 = (s32) var_a2_6->unk4;
                                                var_a3_6->unk8 = (s32) var_a2_6->unk8;
                                                var_a3_6->unkC = (s32) var_a2_6->unkC;
                                                var_a2_6 += 0x10;
                                                var_a3_6 += 0x10;
                                            } while (var_a2_6 != temp_t0_6);
                                            var_v0_21 = 0x644;
                                        }
                                        goto block_443;
                                    
                                    case 3:         /* switch 7 */
                                    
                                        // memcpy
                                        var_a3_7 = &farmFieldTiles;
                                        var_a2_7 = &D_801157B0;
                                        temp_t0_7 = &D_801157B0 + 0x1E0;
                                        if (((s32) &D_801157B0 | (s32) &farmFieldTiles) & 3) {
                                            do {
                                                var_a3_7->unk0 = (unaligned s32) var_a2_7->unk0;
                                                var_a3_7->unk4 = (unaligned s32) var_a2_7->unk4;
                                                var_a3_7->unk8 = (unaligned s32) var_a2_7->unk8;
                                                var_a3_7->unkC = (unaligned s32) var_a2_7->unkC;
                                                var_a2_7 += 0x10;
                                                var_a3_7 += 0x10;
                                            } while (var_a2_7 != temp_t0_7);
                                            var_v0_21 = 0x645;
                                        } else {
                                            do {
                                                var_a3_7->unk0 = (s32) var_a2_7->unk0;
                                                var_a3_7->unk4 = (s32) var_a2_7->unk4;
                                                var_a3_7->unk8 = (s32) var_a2_7->unk8;
                                                var_a3_7->unkC = (s32) var_a2_7->unkC;
                                                var_a2_7 += 0x10;
                                                var_a3_7 += 0x10;
                                            } while (var_a2_7 != temp_t0_7);
                                            var_v0_21 = 0x645;
                                        }
                                        goto block_443;

                                    case 4:         /* switch 7 */

                                        // memcpy
                                        var_a3_8 = &farmFieldTiles;
                                        var_a2_8 = &D_80115990;
                                        temp_t0_8 = &D_80115990 + 0x1E0;
                                        if (((s32) &D_80115990 | (s32) &farmFieldTiles) & 3) {
                                            do {
                                                var_a3_8->unk0 = (unaligned s32) var_a2_8->unk0;
                                                var_a3_8->unk4 = (unaligned s32) var_a2_8->unk4;
                                                var_a3_8->unk8 = (unaligned s32) var_a2_8->unk8;
                                                var_a3_8->unkC = (unaligned s32) var_a2_8->unkC;
                                                var_a2_8 += 0x10;
                                                var_a3_8 += 0x10;
                                            } while (var_a2_8 != temp_t0_8);
                                            var_v0_21 = 0x646;
                                        } else {
                                            do {
                                                var_a3_8->unk0 = (s32) var_a2_8->unk0;
                                                var_a3_8->unk4 = (s32) var_a2_8->unk4;
                                                var_a3_8->unk8 = (s32) var_a2_8->unk8;
                                                var_a3_8->unkC = (s32) var_a2_8->unkC;
                                                var_a2_8 += 0x10;
                                                var_a3_8 += 0x10;
                                            } while (var_a2_8 != temp_t0_8);
                                            var_v0_21 = 0x646;
                                        }
                                        goto block_443;
                                    }


                                } else {
                                    gSeason = SUMMER;
                                    // HOUSE_EXIT
                                    var_s0 = 0;
                                    switch (D_801886D2) { /* switch 8 */
                                    case 0:         /* switch 8 */

                                        // memcpy
                                        var_a3_9 = &farmFieldTiles;
                                        var_a2_9 = &D_80115B70;
                                        temp_t0_9 = &D_80115B70 + 0x1E0;
                                        if (((s32) &D_80115B70 | (s32) &farmFieldTiles) & 3) {
                                            do {
                                                var_a3_9->unk0 = (unaligned s32) var_a2_9->unk0;
                                                var_a3_9->unk4 = (unaligned s32) var_a2_9->unk4;
                                                var_a3_9->unk8 = (unaligned s32) var_a2_9->unk8;
                                                var_a3_9->unkC = (unaligned s32) var_a2_9->unkC;
                                                var_a2_9 += 0x10;
                                                var_a3_9 += 0x10;
                                            } while (var_a2_9 != temp_t0_9);
                                            var_v0_21 = 0x647;
                                        } else {
                                            do {
                                                var_a3_9->unk0 = (s32) var_a2_9->unk0;
                                                var_a3_9->unk4 = (s32) var_a2_9->unk4;
                                                var_a3_9->unk8 = (s32) var_a2_9->unk8;
                                                var_a3_9->unkC = (s32) var_a2_9->unkC;
                                                var_a2_9 += 0x10;
                                                var_a3_9 += 0x10;
                                            } while (var_a2_9 != temp_t0_9);
                                            var_v0_21 = 0x647;
                                        }
block_443:
                                        gCutsceneIndex = var_v0_21;
                                        break;
                                    case 1:         /* switch 8 */

                                        // memcpy
                                        var_a3_10 = &farmFieldTiles;
                                        var_a2_10 = &D_80115D50;
                                        temp_t0_10 = &D_80115D50 + 0x1E0;
                                        if (((s32) &D_80115D50 | (s32) &farmFieldTiles) & 3) {
                                            do {
                                                var_a3_10->unk0 = (unaligned s32) var_a2_10->unk0;
                                                var_a3_10->unk4 = (unaligned s32) var_a2_10->unk4;
                                                var_a3_10->unk8 = (unaligned s32) var_a2_10->unk8;
                                                var_a3_10->unkC = (unaligned s32) var_a2_10->unkC;
                                                var_a2_10 += 0x10;
                                                var_a3_10 += 0x10;
                                            } while (var_a2_10 != temp_t0_10);
                                            var_v0_21 = 0x648;
                                        } else {
                                            do {
                                                var_a3_10->unk0 = (s32) var_a2_10->unk0;
                                                var_a3_10->unk4 = (s32) var_a2_10->unk4;
                                                var_a3_10->unk8 = (s32) var_a2_10->unk8;
                                                var_a3_10->unkC = (s32) var_a2_10->unkC;
                                                var_a2_10 += 0x10;
                                                var_a3_10 += 0x10;
                                            } while (var_a2_10 != temp_t0_10);
                                            var_v0_21 = 0x648;
                                        }

                                        goto block_443;
                                    case 2:         /* switch 8 */

                                        // memcpy
                                        var_a3_11 = &farmFieldTiles;
                                        var_a2_11 = &D_80115F30;
                                        temp_t0_11 = &D_80115F30 + 0x1E0;
                                        if (((s32) &D_80115F30 | (s32) &farmFieldTiles) & 3) {
                                            do {
                                                var_a3_11->unk0 = (unaligned s32) var_a2_11->unk0;
                                                var_a3_11->unk4 = (unaligned s32) var_a2_11->unk4;
                                                var_a3_11->unk8 = (unaligned s32) var_a2_11->unk8;
                                                var_a3_11->unkC = (unaligned s32) var_a2_11->unkC;
                                                var_a2_11 += 0x10;
                                                var_a3_11 += 0x10;
                                            } while (var_a2_11 != temp_t0_11);
                                            var_s0 = 0x11;
                                        } else {
                                            do {
                                                var_a3_11->unk0 = (s32) var_a2_11->unk0;
                                                var_a3_11->unk4 = (s32) var_a2_11->unk4;
                                                var_a3_11->unk8 = (s32) var_a2_11->unk8;
                                                var_a3_11->unkC = (s32) var_a2_11->unkC;
                                                var_a2_11 += 0x10;
                                                var_a3_11 += 0x10;
                                            } while (var_a2_11 != temp_t0_11);
                                            var_s0 = 0x11;
                                        }
                                        var_v0_21 = 0x649;
                                        goto block_443;


                                    case 3:         /* switch 8 */

                                        // memcpy
                                        var_a3_12 = &farmFieldTiles;
                                        var_a2_12 = &D_80116110;
                                        temp_t0_12 = &D_80116110 + 0x1E0;
                                        if (((s32) &D_80116110 | (s32) &farmFieldTiles) & 3) {
                                            do {
                                                var_a3_12->unk0 = (unaligned s32) var_a2_12->unk0;
                                                var_a3_12->unk4 = (unaligned s32) var_a2_12->unk4;
                                                var_a3_12->unk8 = (unaligned s32) var_a2_12->unk8;
                                                var_a3_12->unkC = (unaligned s32) var_a2_12->unkC;
                                                var_a2_12 += 0x10;
                                                var_a3_12 += 0x10;
                                            } while (var_a2_12 != temp_t0_12);
                                            var_v0_21 = 0x64A;
                                        } else {
                                            do {
                                                var_a3_12->unk0 = (s32) var_a2_12->unk0;
                                                var_a3_12->unk4 = (s32) var_a2_12->unk4;
                                                var_a3_12->unk8 = (s32) var_a2_12->unk8;
                                                var_a3_12->unkC = (s32) var_a2_12->unkC;
                                                var_a2_12 += 0x10;
                                                var_a3_12 += 0x10;
                                            } while (var_a2_12 != temp_t0_12);
                                            var_v0_21 = 0x64A;
                                        }
                                        goto block_443;


                                    case 4:         /* switch 8 */
                                        
                                        // memcpy
                                        var_a3_13 = &farmFieldTiles;
                                        var_a2_13 = &D_80116110;
                                        temp_t0_13 = &D_80116110 + 0x1E0;
                                        if (((s32) &D_80116110 | (s32) &farmFieldTiles) & 3) {
                                            do {
                                                var_a3_13->unk0 = (unaligned s32) var_a2_13->unk0;
                                                var_a3_13->unk4 = (unaligned s32) var_a2_13->unk4;
                                                var_a3_13->unk8 = (unaligned s32) var_a2_13->unk8;
                                                var_a3_13->unkC = (unaligned s32) var_a2_13->unkC;
                                                var_a2_13 += 0x10;
                                                var_a3_13 += 0x10;
                                            } while (var_a2_13 != temp_t0_13);
                                            var_v0_21 = 0x64B;
                                        } else {
                                            do {
                                                var_a3_13->unk0 = (s32) var_a2_13->unk0;
                                                var_a3_13->unk4 = (s32) var_a2_13->unk4;
                                                var_a3_13->unk8 = (s32) var_a2_13->unk8;
                                                var_a3_13->unkC = (s32) var_a2_13->unkC;
                                                var_a2_13 += 0x10;
                                                var_a3_13 += 0x10;
                                            } while (var_a2_13 != temp_t0_13);
                                            var_v0_21 = 0x64B;
                                        }
                                        goto block_443;
                                    }
                                }
block_444:
                                var_s0_2 = var_s0;
block_445:
                                setEntrance(var_s0_2);
                                loadCutscene(0);
                                func_8006E840(var_s0_2);
                                break;
                            }
                        }
                    } else {
                        setEntrance(0x3E);
                        var_v0_3 = 0x5AE;
block_372:
                        gCutsceneIndex = var_v0_3;
                        loadCutscene(0);
                        func_8006E840(0x3E);
                    }
                } else {
                    setEntrance(0x61);
                    var_v0_16 = 0x1AD;
                    if (!(gCutsceneCompletionFlags & 8) && (var_v0_16 = 0x1AE, ((gCutsceneCompletionFlags & 0x10) == 0))) {
                        var_v0_16 = 0x1AF;
                        if (gCutsceneCompletionFlags & 0x20) {
                            goto block_359;
                        }
                    } else {
block_359:
                        gCutsceneIndex = var_v0_16;
                    }
                    loadCutscene(0);
                    func_8006E840(0x61);
                }
                if (gCutsceneIndex != 0xFFFF) {
                    *(&unknownRGBA + 0xC) = 0.0f;
                    *(&unknownRGBA + 8) = 0.0f;
                    *(&unknownRGBA + 4) = 0.0f;
                    unknownRGBA = 0.0f;
                    setMainLoopCallbackFunctionIndex(4);
                }
            }
            if (((gCutsceneIndex == 0x4B0) || (gCutsceneIndex == 0x578))) {
                setEntitySpriteDimensions(ENTITY_ASSET_SHIPPER, 0xC, 0xC);
                setEntitySpriteDimensions(ENTITY_ASSET_ASSISTANT_CARPENTER, 0xC, 0xC);
            }
            if (checkDailyEventBit(0x5F)) {
                func_800657BC();
            }
            gCutsceneCompletionFlags = 0;
        }
        if (gCutsceneCompletionFlags & 4) {
            temp_s0 = &overlayScreenStrings + 6;
            setGameVariableString(0x28, (D_8018A72C * 6) + temp_s0, 6);
            setGameVariableString(0x29, (D_8018A72D * 6) + temp_s0, 6);
            gCutsceneCompletionFlags &= ~4;
        }
        if (gCutsceneCompletionFlags & 0x40) {
            setSpritePaletteIndex(0x62, 0);
            gCutsceneCompletionFlags &= ~0x40;
        }
        if (gCutsceneCompletionFlags & 0x80) {
            setSpritePaletteIndex(0x62, 1);
            gCutsceneCompletionFlags &= ~0x80;
        }
        if (gCutsceneCompletionFlags & 0x100) {
            // fontPalette3
            nuPiReadRom(&D_E137D0, 0x8030A000, &D_E13800 - &D_E137D0);
            func_8003F464(0, 0xE, 0xE, 0x802FF000, 0x8030A000);
            func_8003F360(0, 1, 1);
            setMessageBoxSfx(0, 0xFF, 0xFF, 0xFF);
            gCutsceneCompletionFlags &= ~0x100;
        }
        if (gCutsceneCompletionFlags & 0x20000) {
            setEntrance(HOUSE_EXIT);
            func_8006E840(0);
            func_8003BE98(0, 0xFF, 0xFF, 0xFF, 0xFF);
            gCutsceneCompletionFlags &= 0xFFFDFFFF;
        }
        if (gCutsceneCompletionFlags & 0x40000) {
            func_80073244(0);
            gCutsceneCompletionFlags &= 0xFFFBFFFF;
        }
        if (gCutsceneCompletionFlags & 0x80000) {
            func_80073244(1);
            gCutsceneCompletionFlags &= 0xFFF7FFFF;
        }
        if (gCutsceneCompletionFlags & 0x100000) {
            func_80073244(2);
            gCutsceneCompletionFlags &= 0xFFEFFFFF;
        }
        if (gCutsceneCompletionFlags & 0x200000) {
            setEntrance(0x12);
            func_8006E840(0x12);
            func_8003BE98(0, 0, 0, 0, 0);
            gCutsceneCompletionFlags &= 0xFFDFFFFF;
        }
        if (gCutsceneCompletionFlags & 0x400000) {
            setEntrance(0x11);
            func_8006E840(0x11);
            func_8003BE98(0, 0, 0, 0, 0);
            gCutsceneCompletionFlags &= 0xFFBFFFFF;
        }
        if (gCutsceneCompletionFlags & 0x800000) {
            func_8007341C(0);
            gCutsceneCompletionFlags &= 0xFF7FFFFF;
        }
        if (gCutsceneCompletionFlags & 0x01000000) {
            func_8007341C(2);
            gCutsceneCompletionFlags &= 0xFEFFFFFF;
        }
        if (gCutsceneCompletionFlags & 0x02000000) {
            func_80034DC8(0, 0, 0x13);
            gCutsceneCompletionFlags &= 0xFDFFFFFF;
        }
        if ((gCutsceneCompletionFlags & 0x04000000) && ((u32) (*(&entities + 0x54) - 0x98) < 0x18)) {
            temp_f2 = (s32) *(&entities + 0x28);
            var_v0_22 = (s16) temp_f2 + 0xF0;
            if (var_v0_22 < 0) {
                var_v0_22 = (s16) temp_f2 + 0x10F;
            }
            var_a2_14 = (var_v0_22 >> 5) - 2;
            temp_f2_2 = (s32) *(&entities + 0x30);
            var_v0_23 = (s16) temp_f2_2 + 0xF0;
            if (var_v0_23 < 0) {
                var_v0_23 = (s16) temp_f2_2 + 0x10F;
            }
            // gPlayer.direction
            temp_v1_3 = (&gPlayer)[0x6D];
            var_a3_14 = (var_v0_23 >> 5) - 2;
            switch (temp_v1_3) {                    /* switch 9 */
            case 1:                                 /* switch 9 */
                var_a2_14 -= 1;
            case 0:                                 /* switch 9 */
block_498:
                var_a3_14 += 1;
                break;
            case 2:                                 /* switch 9 */
                var_a2_14 -= 1;
                break;
            case 3:                                 /* switch 9 */
                var_a2_14 -= 1;
                /* fallthrough */
            case 4:                                 /* switch 9 */
block_494:
                var_a3_14 -= 1;
                break;
            case 5:                                 /* switch 9 */
                var_a2_14 += 1;
                goto block_494;
            case 6:                                 /* switch 9 */
                var_a2_14 += 1;
                break;
            case 7:                                 /* switch 9 */
                var_a2_14 += 1;
                goto block_498;
            }
            if (((var_a2_14 < 0xA) & (var_a3_14 < 0xA)) && (((u32) ~var_a2_14 >> 0x1F) & ((u32) ~var_a3_14 >> 0x1F))) {
                temp_s3 = (var_a3_14 * 0x14) + &D_80170468 + var_a2_14;
                if (*temp_s3 == 0) {
                    var_s2 = 0;
                    if (var_a2_14 == D_801886D2) {
                        var_v0_24 = var_a3_14 << 0x10;
                        if (var_a3_14 == D_80180710) {
                            func_80038B58(0, 0, (var_a2_14 + 2) & 0xFF, (var_a3_14 + 2) & 0xFF);
                            func_80036C08(0);
                            func_800CF8F8(1, 0, (f32) ((var_a2_14 << 5) - 0xA0), 0, (f32) ((var_a3_14 << 5) - 0xA0));
                            var_s2 = 1;
                            *temp_s3 = 0xD9;
                        } else {
                            goto block_506;
                        }
                    } else {
                        var_v0_24 = var_a3_14 << 0x10;
block_506:
                        *(((var_v0_24 >> 0x10) * 0x14) + &D_80170468 + var_a2_14) = 2;
                    }
                    if (!(var_s2 & 0xFF)) {
                        var_v0_25 = var_s2 & 0xFF;
                        if ((u16) D_80189824 >= 3) {
                            if (!(powerNutBits & 0x20) && (gItemBeingHeld == 0xFF) && !(getRandomNumberInRange(0, 100))) {
                                gItemBeingHeld = 0xFE;
                                powerNutBits |= 0x20;
                            }
                            var_v0_25 = var_s2 & 0xFF;
                            if (checkSpecialDialogueBit(0x15) == 0) {
                                var_v0_25 = var_s2 & 0xFF;
                                if (gItemBeingHeld == 0xFF) {
                                    var_v0_25 = var_s2 & 0xFF;
                                    if (!(getRandomNumberInRange(0, 100))) {
                                        gItemBeingHeld = 0xFD;
                                        setSpecialDialogueBit(0x15);
                                        acquireKeyItem(7);
                                        var_v0_25 = var_s2 & 0xFF;
                                    }
                                }
                            }
                        }
                        if (var_v0_25 == 0) {
                            var_v0_26 = var_s2 & 0xFF;
                            if ((u16) D_80189824 >= 2) {
                                temp_s1 = gItemBeingHeld;
                                var_v0_26 = var_s2 & 0xFF;
                                if (temp_s1 == 0xFF) {
                                    if ((u32) (getRandomNumberInRange(0, 100)) < 4) {
                                        (&gPlayer)[0x2C] = 0x2F;
                                        gItemBeingHeld = 0x2F;
                                    }
                                    temp_s0_2 = gItemBeingHeld;
                                    var_v0_26 = var_s2 & 0xFF;
                                    if (temp_s0_2 == temp_s1) {
                                        if ((u32) (getRandomNumberInRange(0, 100)) < 2) {
                                            (&gPlayer)[0x2C] = 0x30;
                                            gItemBeingHeld = 0x30;
                                        }
                                        var_v0_26 = var_s2 & 0xFF;
                                        if (gItemBeingHeld == temp_s0_2) {
                                            if ((u32) (getRandomNumberInRange(0, 100)) < 3) {
                                                (&gPlayer)[0x2C] = 0x32;
                                                gItemBeingHeld = 0x32;
                                            }
                                            var_v0_26 = var_s2 & 0xFF;
                                        }
                                    }
                                }
                            }
                            if (var_v0_26 == 0) {
                                var_v0_27 = var_s2 & 0xFF;
                                if (gItemBeingHeld == 0xFF) {
                                    if ((u32) (getRandomNumberInRange(0, 100)) < 0xA) {
                                        gItemBeingHeld = 0xFC;
                                    }
                                    var_v0_27 = var_s2 & 0xFF;
                                }
                                if (var_v0_27 == 0) {
                                    var_v0_28 = var_s2 & 0xFF;
                                    if (gItemBeingHeld == 0xFF) {
                                        if ((u32) (getRandomNumberInRange(0, 100)) < 8) {
                                            (&gPlayer)[0x2C] = 0x2E;
                                            gItemBeingHeld = 0x2E;
                                        }
                                        var_v0_28 = var_s2 & 0xFF;
                                    }
                                    if ((var_v0_28 == 0) && (gItemBeingHeld == 0xFF) && ((u32) (getRandomNumberInRange(0, 100)) < 5)) {
                                        (&gPlayer)[0x2C] = 0x31;
                                        gItemBeingHeld = 0x31;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            gCutsceneCompletionFlags &= 0xFBFFFFFF;
        }
        if ((gCutsceneCompletionFlags & 0x08000000) && ((u8) gItemBeingHeld < 0xFC)) {
            handleEatingAndDrinking();
        }
        temp_v1_4 = gCutsceneCompletionFlags & 0x1000;
        gCutsceneCompletionFlags &= 0xF7FFFFFF;
        if (temp_v1_4) {
            setDailyEventBit(6);
        } else {
            toggleDailyEventBit(6);
        }
        if (gCutsceneCompletionFlags & 0x2000) {
            setDailyEventBit(0x13);
        } else {
            toggleDailyEventBit(0x13);
        }
        if (gCutsceneCompletionFlags & 0x4000) {
            setDailyEventBit(0x12);
        } else {
            toggleDailyEventBit(0x12);
        }
        if (gCutsceneCompletionFlags & 0x400) {
            setDailyEventBit(0x15);
        } else {
            toggleDailyEventBit(0x15);
        }
        if (gCutsceneCompletionFlags & 0x200) {
            setDailyEventBit(0x28);
        }
        if (checkDailyEventBit(0x2B)) {
            toggleDailyEventBit(0x2B);
            gPlayer.currentStamina += adjustValue(gPlayer.currentStamina, 50, gMaximumStamina);
        }
        if (checkDailyEventBit(0x2A)) {
            toggleDailyEventBit(0x2A);
            func_8008634C(5);
        }
        if (gCutsceneCompletionFlags & 0x8000) {
            func_800610DC();
            gCutsceneCompletionFlags &= 0xFFFF7FFF;
        }
        if (gCutsceneFlags & 8) {
            func_800876D0();
            gCutsceneFlags &= ~8;
        }
        if (gCutsceneFlags & 0x10) {
            func_8008779C();
            gCutsceneFlags &= ~0x10;
        }
        if (gCutsceneCompletionFlags & 0x10000000) {
            func_800876D0();
            gCutsceneCompletionFlags &= 0xEFFFFFFF;
        }
        if (gCutsceneCompletionFlags & 0x20000000) {
            func_8008779C();
            gCutsceneCompletionFlags &= 0xDFFFFFFF;
        }
        if (checkLifeEventBit(0x4B)) {
            toggleLifeEventBit(0x4B);
            acquireKeyItem(0xB);
            func_800DC1E8();
        }
        if (checkLifeEventBit(0x82)) {
            toggleLifeEventBit(0x82);
            removeKeyItem(0xB);
            setSpecialDialogueBit(0x1F);
        }
        if (checkLifeEventBit(0x49)) {
            toggleLifeEventBit(0x49);
            func_80065BCC(0x14);
            chickenFeedQuantity += adjustValue((u8) chickenFeedQuantity, 0xA, 0x3E7);
        }
        if (checkLifeEventBit(0x4A)) {
            toggleLifeEventBit(0x4A);
            temp_v1_5 = getRandomNumberInRange(0, 5);
            switch (temp_v1_5) {                    /* switch 10 */
            case 0:                                 /* switch 10 */
                func_80065BCC(0xA);
                D_801FC154 += adjustValue(D_801FC154, 1, 0x14);
                return;
            case 1:                                 /* switch 10 */
                func_80065BCC(0xB);
                D_80204DF4 += adjustValue(D_80204DF4, 1, 0x14);
                return;
            case 2:                                 /* switch 10 */
                func_80065BCC(0xC);
                D_8018A725 += adjustValue(D_8018A725, 1, 0x14);
                return;
            case 3:                                 /* switch 10 */
                func_80065BCC(0xD);
                D_8013DC2C += adjustValue(D_8013DC2C, 1, 0x14);
                return;
            case 4:                                 /* switch 10 */
                func_80065BCC(0xE);
                D_801FAD91 += adjustValue(D_801FAD91, 1, 0x14);
                return;
            case 5:                                 /* switch 10 */
                func_80065BCC(0xF);
                D_80237458 += adjustValue(D_80237458, 1, 0x14);
                break;
            }
        }
    } else {
    default:                                        /* switch 10 */

    }
}

#else
INCLUDE_ASM("asm/nonmatchings/game/setCutscenes", func_800A8F74);
#endif

//INCLUDE_ASM("asm/nonmatchings/game/setCutscenes", loadCutscene);

void loadCutscene(u8 arg0) {
    
    u16 index;

    if (gCutsceneIndex >= 0x640) {
        
        D_8021E6D0 = 0;
        func_8003C084(0, 7);
        index = 35;
        
    } else if (gCutsceneIndex >= 0x60E) {
    
        D_8021E6D0 = 0;
        func_8003C084(0, 7);
        index = 31;        
    
    } else if (gCutsceneIndex >= 0x5DC) {
             
        D_8021E6D0 = 0;
        func_8003C084(0, 7);

        memcpy((u32)&farmFieldTiles, (u32)D_80114E50, FIELD_HEIGHT * FIELD_WIDTH);

        index = 32;
        
    } else if (gCutsceneIndex >= 0x5AA) {
        
        D_8021E6D0 = 0;
        
        func_8003C084(0, 7);
        
        gSeason = 1;

        memcpy((u32)farmFieldTiles,(u32)D_80113580, FIELD_HEIGHT * FIELD_WIDTH);

        index = 30;
        
    } else if (gCutsceneIndex >= 0x578) {
    
        index = 29;
        setEntitySpriteDimensions(ENTITY_ASSET_SHIPPER, 12, 24);
        setEntitySpriteDimensions(ENTITY_ASSET_ASSISTANT_CARPENTER, 12, 24);
    
    } else if (gCutsceneIndex >= 0x546) {
    
        index = 28;
    }
    else if (gCutsceneIndex >= 0x514) {
    
        index = 27;  
    }
    else if (gCutsceneIndex >= 0x4E2) {
    
        index = 26;
    
    } else if (gCutsceneIndex >= 0x4B0) {
        
        index = 25;
        setEntitySpriteDimensions(ENTITY_ASSET_SHIPPER, 12, 24);
        setEntitySpriteDimensions(ENTITY_ASSET_ASSISTANT_CARPENTER, 12, 24);
        
    } else  {

        index = 24;
        
        // COW_FESTIVAL
        if (gCutsceneIndex < 0x47E) {
            
            // FIXME: fake match
            __asm__ __volatile__("" : : : "memory");

            // constants should probably be macros designating bytecode bank boundaries
            // SEA_FESTIVAL
            if (gCutsceneIndex >= 0x44C) {
                index = 23;
            } else if (gCutsceneIndex >= 0x41A) {
                index = 22;
            } else if (gCutsceneIndex >= 0x3E8) {
                index = 20;    
            } else if (gCutsceneIndex >= 0x3B6) {
                index = 19;
            } else if (gCutsceneIndex >= 0x384) {
                index = 18;  
            } else if (gCutsceneIndex >= 0x352) {
                index = 17;  
            } else if (gCutsceneIndex >= 0x320) {
                index = 10;  
            } else if (gCutsceneIndex >= 0x2EE) {
                index = 16;  
            } else if (gCutsceneIndex >= 0x2BC) {
                index = 13;  
            } else if (gCutsceneIndex > 0x289) {
                index = 12;
            } else if (gCutsceneIndex >= 0x258) {
                index = 21;
            } else if (gCutsceneIndex >= 0x1F4) {
                index = 15;    
            } else if (gCutsceneIndex >= 0x190) {
                index = 14;
            } else if (gCutsceneIndex >= 0x12C) {
                index = 11;
            } else if (gCutsceneIndex >= 0x64) {
                
                if (arg0) {
                    index = 7;
                } else  {

                    index = 8;

                    switch (gCutsceneIndex) {

                        case 0xDC:
                            D_8021E6D0 = 0;
                            func_8003C084(0, 7);
                            break;
                        case 0xE4 ... 0xEE:
                            D_8021E6D0 = 0;
                            func_8003C084(0, 7);
                            break;
                        default:
                            break;
                    
                    }
                    
                } 
                
            } else if (gCutsceneIndex != 0x27) {
                
                index = 9;
                
            } else {
                
                index = 9;
                D_8021E6D0 = 0;
                func_8003C084(0, 7);
                
            }
                
        }
    } 
    
    nuPiReadRom(cutsceneBytecodeAddresses[index].romAddrStart, cutsceneBankLoadAddresses[index], cutsceneBytecodeAddresses[index].romAddrEnd - cutsceneBytecodeAddresses[index].romAddrStart);
    
    gCutsceneCompletionFlags = 0;
    
    spawnCutsceneExecutor(cutsceneExecutorIndices[index], cutsceneEntryPointAddresses[index]);
    togglePauseEntities();
    
    D_8018981C = index;
    
    gCutsceneFlags |= 1;
    
    toggleDailyEventBit(0x53);
    
    switch (gCutsceneIndex) {
        case STARRY_NIGHT_MOON_MOUNTAIN:
        case MARIA_WEDDING ... KAREN_WEDDING:
        case STARRY_NIGHT_FESTIVAL:
        case 0x20B:
        case 0x2F4:
        case SOWING_FESTIVAL_POTION_SHOP_DEALER ... SOWING_FESTIVAL_HARRIS:
        case HORSE_RACE_SQUARE: 
        case FLOWER_FESTIVAL:
        case 0x41A:
        case SEA_FESTIVAL:
        case COW_FESTIVAL:
        case 0x4B0:
        case EGG_FESTIVAL:
        case DOG_RACE_SQUARE:
        case 0x546 ... 0x548:
        case NEW_YEAR_FESTIVAL:
        case 0x5AA:
        case INTRO:
        case DEMO_CUTSCENE_1: 
        case DEMO_CUTSCENE_2:
        case DEMO_CUTSCENE_3:
        case EVALUATION:
        case HOW_TO_PLAY_CUTSCENE:
            setDailyEventBit(0x4B);
            break;
    }
    
}