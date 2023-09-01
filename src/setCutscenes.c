#include "common.h"

#include "setCutscenes.h"

#include "system/cutscene.h"
#include "system/message.h"
#include "system/sprite.h"

#include "animals.h"
#include "game.h"
#include "gameStatus.h"
#include "mapObjects.h"
#include "level.h"
#include "npc.h"
#include "player.h"
#include "weather.h"
                                 
// forward declarations
u16 setBeachCutscenes(void);                         
u16 setCaveCutscenes(void);                          
u16 setChurchCutscenes(void);                        
u16 setFlowerShopCutscenes(void);                    
u16 setHarvestSpriteCaveCutscenes(void);             
u16 setMoonMountainCutscenes(void);                     
u16 setRanchStoreCutscenes(void);                    
u16 setRickStoreCutscenes(void);                     
u16 setRoadCutscenes(void);                          
u16 setSpringCutscenes(void);                        
u16 setVineyardCellarCutscenes(void);                
u16 setVineyardHouseCutscenes(void);            

// bss
s32 D_801891D4;
extern u16 D_8018981C;

// likely bss
extern u16 gCutsceneIndex;

// shared globals   
extern u8 gHouseExtensionSelection;
extern u8 D_8016FE6F;
// flower shop points
extern u8 D_80181B10;
extern u8 D_801C4216;
extern u16 D_80237410;

// data
extern Addresses cutsceneMapAddresses[];
extern void *D_80114C70[];
extern void *D_80114D30[];
extern u16 D_80114DF0[];


INCLUDE_ASM(const s32, "setCutscenes", func_8009BF90);

// jtbl_80120EC0
//INCLUDE_ASM(const s32, "setCutscenes", func_8009C054);

u16 func_8009C054(u16 mapIndex) {
    
    u16 result = 0xFFFF;
    
    if (!(gCutsceneFlags & 1)) {
        
        gCutsceneFlags &= ~2;

        switch (mapIndex) {
            case FARM:
                result = setFarmCutscenes();
                break;
            case MOUNTAIN_1:
                result = setMountain1Cutscenes();
                break;
            case MOUNTAIN_2:
                result = func_800A1C04();
                break;
            case TOP_OF_MOUNTAIN_1:
                result = func_800A2250();
                break;
            case MOON_MOUNTAIN:
                result = setMoonMountainCutscenes();
                break;
            case POND:
                result = setSpringCutscenes();
                break;
            case HARVEST_SPRITE_CAVE:
                result = setHarvestSpriteCaveCutscenes();
                break;
            case CAVE:
                result = setCaveCutscenes();
                break;
            case HOUSE:
                result = func_800A29B0();
                break;
            case KITCHEN:
                result = func_800A2FA8();
                break;
            case RANCH:
                result = func_800A3F04();
                break;
            case RANCH_STORE:
                result = setRanchStoreCutscenes();
                break;
            case VINEYARD:
                result = func_800A4878();
                break;
            case VINEYARD_HOUSE:
                result = setVineyardHouseCutscenes();
                break;
            case VINEYARD_CELLAR:
                result = setVineyardCellarCutscenes();
                break;
            case VILLAGE_1:
                result = func_800A4E50();
                break;
            case VILLAGE_2:
                result = func_800A5314();
                break;
            case RICK_STORE:
                result = setRickStoreCutscenes();
                break;
            case FLOWER_SHOP:
                result = setFlowerShopCutscenes();
                break;
            case BAKERY:
                result = func_800A5F48();
                break;
            case MAYOR_HOUSE:
                result = func_800A6440();
                break;
            case LIBRARY:
                result = func_800A6634();
                break;
            case CHURCH:
                result = setChurchCutscenes();
                break;
            case TAVERN:
                result = func_800A68C0();
                break;
            case SQUARE:
                result = func_800A6A14();
                break;
            case RACE_TRACK:
                result = func_800A6EE4();
                break;
            case BEACH:
                result = setBeachCutscenes();
                break;
            case ROAD:
                result = setRoadCutscenes();
                break;
            default:
                break;
            }
        
        if (result != 0xFFFF) {

            nuPiReadRom(cutsceneMapAddresses[result].romAddr, D_80114D30[result], cutsceneMapAddresses[result].vaddr - cutsceneMapAddresses[result].romAddr);
            
            D_801891D4 = 0;
            
            func_800469A8(D_80114DF0[result], D_80114C70[result]);
            func_8002F730();
            
            D_8018981C = result;
            
            gCutsceneFlags |= 1;
            
            toggleDailyEventBit(0x53);
        }
    }
    
    return result;
}

// jtbl_80121030
//INCLUDE_ASM(const s32, "setCutscenes", func_8009C324);

u16 func_8009C324(u8 arg0) {

    u8 set = 0;
    u16 result = 0xFFFF;
    u8 tempGirl;
    
    if (!checkLifeEventBit(MAYOR_TOUR)) {
        setLifeEventBit(MAYOR_TOUR);
        gCutsceneIndex = MAYOR_VILLAGE_TOUR;
        gCutsceneFlags |= 2;
        loadCutscene(arg0);
        set = 1;
    }

    if (gEntranceIndex != 0x6F) {
        
        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gYear == 1 && gSeason == AUTUMN && gDayOfMonth == 22 && (gHour - 6) < 12U) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = 0xDF;
                loadCutscene(arg0);
                set = 1;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gYear == 1 && gSeason == WINTER && gDayOfMonth == 11 && (gHour - 6) < 12U) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = 0xDE;
                loadCutscene(arg0);
                set = 1;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == SPRING && gDayOfMonth == 16 && (gHour - 6) < 0x9U && (horseInfo.flags & 1) && horseInfo.grown == TRUE) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = DOUG_FARM_HORSE_RACE;
                loadCutscene(arg0);
                set = 1;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == AUTUMN && gDayOfMonth == 27 && (gHour - 6) < 9U && (horseInfo.flags & 1) && horseInfo.grown == TRUE) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = 0x7E;
                loadCutscene(arg0);
                set = 1;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == WINTER && gDayOfMonth == 18 && (gHour - 6) < 9U) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = DOUG_FARM_DOG_RACE;
                loadCutscene(arg0);
                set = 1;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == SPRING && gDayOfMonth == 7 && (gHour - 6) < 3U) {
                
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
                set = 1;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == AUTUMN && gDayOfMonth == 2 && (gHour - 6) < 3U && npcAffection[KENT] >= 0x1E) {
                setDailyEventBit(FARM_MORNING_VISIT);
                
                if (func_8009B2BC(2)) {
                    gCutsceneIndex = 0x66;
                    func_8003FBA0(0xE, gFarmAnimals[func_8009B464()].name, 6);
                } else {
                    gCutsceneIndex = 0x65;
                }
                
                loadCutscene(arg0);
                set = 1;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == WINTER && gDayOfMonth == 9 && (gHour - 6) < 3U && npcAffection[KENT] >= 0x1E) {
                setDailyEventBit(FARM_MORNING_VISIT);
                
                if (gYear == 1) {
                    gCutsceneIndex = KENT_FARM_THANKSGIVING_FESTIVAL;
                } else {
                    gCutsceneIndex = 0x68;
                }
                
                loadCutscene(arg0);
                set = 1;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == SPRING && gDayOfMonth == 16 && (gHour - 6) < 3U) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = MAYOR_FARM_HORSE_RACE;
                loadCutscene(arg0);
                set = 1;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == SUMMER && gDayOfMonth == 17 && (gHour - 6) < 3U) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = MAYOR_FARM_FIREFLY_FESTIVAL;
                loadCutscene(arg0);
                set = 1;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == SUMMER && gDayOfMonth == 23 && (gHour - 6) < 3U) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = MAYOR_FARM_SEA_FESTIVAL;
                loadCutscene(arg0);
                set = 1;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == AUTUMN && gDayOfMonth == 11 && (gHour - 6) < 3U) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = MAYOR_FARM_HARVEST_FESTIVAL;
                loadCutscene(arg0);
                set = 1;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == AUTUMN && gDayOfMonth == 19 && (gHour - 6) < 3U) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = MAYOR_FARM_EGG_FESTIVAL;
                loadCutscene(arg0);
                set = 1;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == WINTER && gDayOfMonth == 27 && (gHour - 6) < 3U) {
                
                setDailyEventBit(FARM_MORNING_VISIT);
                
                if (gHarvestKing == PLAYER) {
                    gCutsceneIndex = MAYOR_FARM_SPIRIT_FESTIVAL_PLAYER_HARVEST_KING;
                } else {
                    gCutsceneIndex = MAYOR_FARM_SPIRIT_FESTIVAL;
                }
                
                loadCutscene(arg0);
                set = 1;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == SPRING && gDayOfMonth == 19 && (gHour - 6) < 3U) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = BASIL_FARM_FLOWER_FESTIVAL;
                loadCutscene(arg0);
                set = 1;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == SUMMER && gDayOfMonth == 8 && (gHour - 6) < 3U) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = BASIL_FARM_VEGETABLE_FESTIVAL;
                loadCutscene(arg0);
                set = 1;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(0x19) && npcAffection[POPURI] >= 50 && gSeason == SPRING && gDayOfMonth == 22 && (gHour - 6) < 3U && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
                setDailyEventBit(0x19);
                gCutsceneIndex = 0x9C;
                loadCutscene(arg0);
                set = 1;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(0x1A) && npcAffection[ELLI] >= 50 && gSeason == AUTUMN && gDayOfMonth == 11 && (gHour - 7) < 2U && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {
                setDailyEventBit(0x1A);
                gCutsceneIndex = 0xA8;
                loadCutscene(arg0);
                set = 1;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(0x1B) && npcAffection[ANN] >= 80 && gSeason == SPRING && gDayOfMonth == 16 && (gHour - 7) < 2U && !checkLifeEventBit(MARRIED) && checkLifeEventBit(0x51) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
                setDailyEventBit(0x1B);
                gCutsceneIndex = 0xB3;
                loadCutscene(arg0);
                set = 1;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(0x18) && npcAffection[MARIA] >= 50 && gSeason == SUMMER && gDayOfMonth == 1 && gHour == 8 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
                setDailyEventBit(0x18);
                gCutsceneIndex = MARIA_FIREWORKS_INVITATION;
                loadCutscene(arg0);
                set = 1;
            }            
        }
        
        if (!set) {
            if (!checkDailyEventBit(0x19) && npcAffection[POPURI] >= 50 && gSeason == SUMMER && gDayOfMonth == 1 && gHour == 10 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
                setDailyEventBit(0x19);
                gCutsceneIndex = POPURI_FIREWORKS_INVITATION;
                loadCutscene(arg0);
                set = 1;
            }            
        }

        if (!set) {
            if (!checkDailyEventBit(0x1A) && npcAffection[ELLI] >= 50 && gSeason == SUMMER && gDayOfMonth == 1 && gHour == 12 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {
                setDailyEventBit(0x1A);
                gCutsceneIndex = ELLI_FIREWORKS_INVITATION;
                loadCutscene(arg0);
                set = 1;
            }            
        }

        if (!set) {
            if (!checkDailyEventBit(0x1B) && npcAffection[ANN] >= 50 && gSeason == SUMMER && gDayOfMonth == 1 && gHour == 14 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
                setDailyEventBit(0x1B);
                gCutsceneIndex = ANN_FIREWORKS_INVITATION;
                loadCutscene(arg0);
                set = 1;
            }            
        }

        if (!set) {
            if (!checkDailyEventBit(0x1C) && npcAffection[KAREN] >= 50 && gSeason == SUMMER && gDayOfMonth == 1 && gHour == 16 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
                setDailyEventBit(0x1C);
                gCutsceneIndex = KAREN_FIREWORKS_INVITATION;
                loadCutscene(arg0);
                set = 1;
            }            
        }
        
        if (!set) {
            if (!checkDailyEventBit(0x18) && npcAffection[MARIA] >= 140 && gSeason == WINTER && gDayOfMonth == 10 && gHour == 8 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
                setDailyEventBit(0x18);
                gCutsceneIndex = MARIA_THANKSGIVING_GIFT;
                loadCutscene(arg0);
                set = 1;
            }            
        }        

        if (!set) {
            if (!checkDailyEventBit(0x19) && npcAffection[POPURI] >= 140 && gSeason == WINTER && gDayOfMonth == 10 && gHour == 10 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
                setDailyEventBit(0x19);
                gCutsceneIndex = POPURI_THANKSGIVING_GIFT;
                loadCutscene(arg0);
                set = 1;
            }            
        }        

        if (!set) {
            if (!checkDailyEventBit(0x1A) && npcAffection[ELLI] >= 140 && gSeason == WINTER && gDayOfMonth == 10 && gHour == 12 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {
                setDailyEventBit(0x1A);
                gCutsceneIndex = ELLI_THANKSGIVING_GIFT;
                loadCutscene(arg0);
                set = 1;
            }            
        }        

        if (!set) {
            if (!checkDailyEventBit(0x1B) && npcAffection[ANN] >= 140 && gSeason == WINTER && gDayOfMonth == 10 && gHour == 14 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
                setDailyEventBit(0x1B);
                gCutsceneIndex = ANN_THANKSGIVING_GIFT;
                loadCutscene(arg0);
                set = 1;
            }            
        }    

        if (!set) {
            if (!checkDailyEventBit(0x1C) && npcAffection[KAREN] >= 140 && gSeason == WINTER && gDayOfMonth == 10 && gHour == 16 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
                setDailyEventBit(0x1C);
                gCutsceneIndex = KAREN_THANKSGIVING_GIFT;
                loadCutscene(arg0);
                set = 1;
            }            
        }       

        if (!set) {
            if (!checkDailyEventBit(0x18) && npcAffection[MARIA] >= 100 && gSeason == WINTER && gDayOfMonth == 24 && gHour == 8 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
                setDailyEventBit(0x18);
                gCutsceneIndex = 0x95;
                loadCutscene(arg0);
                set = 1;
            }            
        }        

        if (!set) {
            if (!checkDailyEventBit(0x19) && npcAffection[POPURI] >= 100 && gSeason == WINTER && gDayOfMonth == 24 && gHour == 10 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
                setDailyEventBit(0x19);
                gCutsceneIndex = 0xA0;
                loadCutscene(arg0);
                set = 1;
            }            
        }        

        if (!set) {
            if (!checkDailyEventBit(0x1A) && npcAffection[ELLI] >= 100 && gSeason == WINTER && gDayOfMonth == 24 && gHour == 12 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {
                setDailyEventBit(0x1A);
                gCutsceneIndex = 0xAC;
                loadCutscene(arg0);
                set = 1;
            }            
        }       

        if (!set) {
            if (!checkDailyEventBit(0x1B) && npcAffection[ANN] >= 100 && gSeason == WINTER && gDayOfMonth == 24 && gHour == 14 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
                setDailyEventBit(0x1B);
                gCutsceneIndex = 0xB7;
                loadCutscene(arg0);
                set = 1;
            }            
        }       

        if (!set) {
            if (!checkDailyEventBit(0x1C) && npcAffection[KAREN] >= 100 && gSeason == WINTER && gDayOfMonth == 24 && gHour == 16 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
                setDailyEventBit(0x1C);
                gCutsceneIndex = 0xC4;
                loadCutscene(arg0);
                set = 1;
            }            
        }       

       if (!set) {
           
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == gPlayerBirthdaySeason && gDayOfMonth == 25 && (gHour - 6) < 12U) {
                
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
                                set = 1;
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
                                set = 1;                                
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
                                set = 1;                                
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
                                set = 1;                                
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
                                set = 1;                                
                            }

                            break;

                        default:
                            break; 
                    }
                }
            }            
        }      

        if (!set) {
            if (!checkLifeEventBit(BASIL_IN_TOWN) && ((gSeason == SPRING && gDayOfMonth >= 15) || gSeason == SUMMER) && (gHour - 6) < 11U) {
                
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
                set = 1;
            }            
        }

        if (!set) {
            if (checkLifeEventBit(BASIL_IN_TOWN) && ((gSeason == AUTUMN && gDayOfMonth >= 5) || gSeason == WINTER) && (gHour - 6) < 11U) {
                toggleLifeEventBit(BASIL_IN_TOWN);
                setLifeEventBit(BASIL_GONE);      
                gCutsceneIndex = BASIL_LEAVING;
                loadCutscene(arg0);
                set = 1;
            }
        }

        if (!set && !arg0) {
            if (!checkLifeEventBit(CLIFF_ARRIVED) && (gSeason == SPRING && !(gDayOfMonth < 18) || !(gSeason < SUMMER))) {
                setLifeEventBit(CLIFF_ARRIVED);      
                gCutsceneIndex = CLIFF_INTRO;
                loadCutscene(arg0);      
                set = 1;
            }
        } 

        // Cliff leaving
        if (!set) {
            if (checkLifeEventBit(0x41) && !checkLifeEventBit(0x42) && gSeason == WINTER && (gHour - 6) < 12U && gHarvestKing != 4 && gHarvestCoinFinder != 4) {
                
                if (checkLifeEventBit(MARRIED) && gWife == ANN && npcAffection[CLIFF] >= 150 && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
                    setLifeEventBit(0x42);
                    setLifeEventBit(0x2D);
                    gCutsceneIndex = 0xD5;
                    loadCutscene(arg0);
                    set = 1;                    
                }
                
                if (npcAffection[CLIFF] < 51 && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {              
                    setLifeEventBit(0x42);
                    setLifeEventBit(0x2D);
                    gCutsceneIndex = CLIFF_LEAVING_DEFAULT;
                    loadCutscene(arg0);
                    set = 1;                                
                } 
            }
        }

        if (!set) {
            if (!checkLifeEventBit(0x33) && checkDailyEventBit(0x22) && npcAffection[KENT] >= 5 && (gHour - 6) < 3U) {
                setLifeEventBit(0x33);
                gCutsceneIndex = 0x69;
                loadCutscene(arg0);
                set = 1;                               
            }
        }
        
        if (!set) {
            if (!checkLifeEventBit(0x35) && checkDailyEventBit(0x23) && npcAffection[KENT] >= 5 && (gHour - 6) < 3U) {
                setLifeEventBit(0x35);
                gCutsceneIndex = 0x6A;
                loadCutscene(arg0);
                set = 1;                               
            }
        }
        
        if (!set) {
            if (!checkLifeEventBit(0x37) && checkDailyEventBit(0x24) && npcAffection[KENT] >= 5 && gWeather == SUNNY && (gHour - 6) < 3U) {
                
                setLifeEventBit(0x37);

                if (!func_8009B828(0)) {
                    gCutsceneIndex = 0x6C;
                } else {
                    gCutsceneIndex = 0x6B;
                }
                
                loadCutscene(arg0);
                set = 1;                               
            }
        }
        
        if (!set) {
            if (!checkLifeEventBit(0x39) && checkDailyEventBit(0x25) && npcAffection[KENT] >= 5 && (gHour - 6) < 3U) {
                setLifeEventBit(0x39);
                gCutsceneIndex = 0x6F;
                loadCutscene(arg0);
                set = 1;                               
            }
        }            
        
        if (!set) {
            if (!checkLifeEventBit(0x3B) && checkDailyEventBit(0x26) && npcAffection[KENT] >= 5 && (gHour - 6) < 3U) {
                setLifeEventBit(0x3B);
                gCutsceneIndex = 0x6D;
                loadCutscene(arg0);
                set = 1;                               
            }
        }     

        if (!set) {
            if (!checkLifeEventBit(0x3D) && checkDailyEventBit(0x27) && npcAffection[KENT] >= 5 && (gHour - 6) < 3U) {
                setLifeEventBit(0x3D);
                gCutsceneIndex = 0x6E;
                loadCutscene(arg0);
                set = 1;                               
            }
        }     

        if (!set) {
            
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gForecast == TYPHOON && ((npcAffection[HARVEST_SPRITE_1] + npcAffection[HARVEST_SPRITE_2] + npcAffection[HARVEST_SPRITE_3]) / 3) >= 120 && (gHour - 6) < 3U) {

                if ((func_8009B564() + func_8009B374()) >= 2) {           
                    setDailyEventBit(FARM_MORNING_VISIT);
                    gCutsceneIndex = 0xE3;
                    loadCutscene(arg0);
                    set = 1;                               
                }
            }
        }    

        if (!set) {
            
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && ((npcAffection[CARPENTER_1] + npcAffection[CARPENTER_2] + npcAffection[MASTER_CARPENTER]) / 3) >= 120 && (gHour - 6) < 3U) {

                if (checkDailyEventBit(0x3F)) {           
                    setDailyEventBit(FARM_MORNING_VISIT);
                    gCutsceneIndex = 0xDD;
                    loadCutscene(arg0);
                    set = 1;                               
                }
            }
        }    

        if (!set) {
            if (!checkLifeEventBit(0x8E) && func_8009B374() < 6 && gYear == 1 && gSeason == AUTUMN && gDayOfMonth == 30 && (gHour - 6) < 18U) {
                setLifeEventBit(0x8E);
                gCutsceneIndex = DOUG_FARM_FAVOR;
                loadCutscene(arg0);
                set = 1;    
            }
        }
        
        if (!set && !arg0) {
            if (!checkLifeEventBit(0x8F) && checkLifeEventBit(0x94) && gSeason == WINTER && gDayOfMonth == 1 && (gHour - 6) < 4U && gEntranceIndex == HOUSE_EXIT) {
                setLifeEventBit(0x8F);
                gCutsceneIndex = DOUG_DROPPING_OFF_COWS;
                loadCutscene(arg0);
                set = 1;    
            }
        }
        
        if (!set && !arg0) {
            if (checkLifeEventBit(0x94) && gSeason == WINTER && gDayOfMonth == 5 && (gHour - 6) < 4U && gEntranceIndex == HOUSE_EXIT) {
                toggleLifeEventBit(0x94);
                gCutsceneIndex = MISS_MANA_PICKUP_COWS;
                loadCutscene(arg0);
                set = 1;    
            }
        }      

        if (!set) {
            if (!checkLifeEventBit(0x68) && npcAffection[KENT] >= 50 && gSeason == SUMMER && (gHour - 15) < 2U) {
                setLifeEventBit(0x68);
                gCutsceneIndex = 0x70;
                loadCutscene(arg0);
                set = 1;    
            }
        }
        
        if (!set) {
            if (!checkLifeEventBit(PUPPIES) && (npcAffection[KENT] >= 150 || npcAffection[KENT] + npcAffection[STU] >= 250) && (gHour - 6) < 12U) {
                setLifeEventBit(PUPPIES);
                gCutsceneIndex = KENT_AND_STU_FARM_PUPPIES;
                loadCutscene(arg0);
                set = 1;    
            }
        }
        
        if (!set) {
            if (!checkLifeEventBit(0x6A) && func_800DC008() >= 8 && gWeather == SUNNY && (gSeason && gSeason < 3) && (gHour - 6) < 6U) {
                setLifeEventBit(0x6A);
                gCutsceneIndex = 0x72;
                loadCutscene(arg0);
                set = 1;    
            }
        }        
        
        if (!set) {
            if (!checkLifeEventBit(0x6B) && countPinkCatMintFlowersFarm() && gWeather == SUNNY && (gSeason && gSeason < 3) && (gHour - 6) < 6U) {
                setLifeEventBit(0x6B);
                gCutsceneIndex = 0x73;
                loadCutscene(arg0);
                set = 1;    
            }
        }        

        if (!set) {
            if (!checkLifeEventBit(0x6C) && countPinkCatMintFlowersFarm() >= 16 && npcAffection[STU] >= 80 && gWeather == SUNNY && (gSeason && gSeason < 3) && (gHour - 6) < 6U) {
                setLifeEventBit(0x6C);
                gCutsceneIndex = 0x74;
                loadCutscene(arg0);
                set = 1;    
            }
        }      

        if (!set) {
            if (!checkLifeEventBit(0x6D) && npcAffection[MARIA] >= 50 && (gHour - 15) < 2U && (!checkLifeEventBit(MARRIED) || gWife != MARIA)) {
                setLifeEventBit(0x6D);
                gCutsceneIndex = MARIA_FARM_GIFT_CHOCOLATE;
                loadCutscene(arg0);
                set = 1;    
            }
        }               
        
        if (!set) {
            if (!checkLifeEventBit(0x6E) && npcAffection[MARIA] >= 80 && (gHour - 15) < 2U && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
                setLifeEventBit(0x6E);
                gCutsceneIndex = MARIA_FARM_GIFT_RICE_BALLS;
                loadCutscene(arg0);
                set = 1;                               
            }
        }     

        if (!set) {
            if (!checkLifeEventBit(0x6F) && npcAffection[MARIA] >= 200 && gWeather == SUNNY && gSeason == SUMMER && (gHour - 18) < 6U && !checkDailyEventBit(0x30) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
                setLifeEventBit(0x6F);
                gCutsceneIndex = 0x93;
                loadCutscene(arg0);
                set = 1;                               
            }
        }     
        
        if (!set) {
            if (!checkLifeEventBit(0x70) && npcAffection[POPURI] >= 50 && (gHour - 15) < 2U && (!checkLifeEventBit(MARRIED) || gWife != POPURI)) {
                setLifeEventBit(0x70);
                gCutsceneIndex = POPURI_FARM_GIFT_TEA;
                loadCutscene(arg0);
                set = 1;                               
            }
        }          
        
        if (!set) {
            if (!checkLifeEventBit(0x71) && npcAffection[POPURI] >= 80 && (gHour - 15) < 2U && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
                setLifeEventBit(0x71);
                gCutsceneIndex = POPURI_FARM_GIFT_COOKIES;
                loadCutscene(arg0);
                set = 1;                               
            }
        }          
        if (!set) {
            if (!checkLifeEventBit(0x72) && blueMistFlowerPlot >= 0xC2 && npcAffection[POPURI] >= 200 && gWeather == SUNNY && gSeason == SUMMER && gDayOfMonth == 30 && (gHour - 6) < 12U 
                    && ((checkLifeEventBit(MARRIED) && gWife == POPURI) || (!checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)))) {
                blueMistFlowerPlot = 0;
                D_8016FE6F = 0;
                setLifeEventBit(0x72);
                gCutsceneIndex = 0x9E;
                loadCutscene(arg0);
                set = 1;           
                
            }
        } 

        if (!set) {
            if (!checkLifeEventBit(0x73) && npcAffection[ELLI] >= 50 && (gHour - 15) < 2U && (!checkLifeEventBit(MARRIED) || gWife != ELLI)) {
                setLifeEventBit(0x73);
                gCutsceneIndex = 0xA5;
                loadCutscene(arg0);
                set = 1;                      
            }
        }
        
        if (!set) {
            if (!checkLifeEventBit(0x74) && npcAffection[ELLI] >= 80 && (gHour - 15) < 2U && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {
                setLifeEventBit(0x74);
                gCutsceneIndex = 0xA6;
                loadCutscene(arg0);
                set = 1;                      
            }
        }
        
        if (!set) {
            if (!checkLifeEventBit(0x75) && npcAffection[ELLI] >= 200 && gWeather == SUNNY && gSeason == AUTUMN && gDayOfMonth == 9 && (gHour - 18) < 6U && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {
                setLifeEventBit(0x75);
                gCutsceneIndex = 0xA9;
                loadCutscene(arg0);
                set = 1;                      
            }
        }      

        if (!set) {
            if (!checkLifeEventBit(0x76) && func_8009B658() >= 6 && gSeason == WINTER && gDayOfMonth == 9 && (gHour - 6) < 18U && (!checkLifeEventBit(MARRIED) || gWife != ELLI)) {
                setLifeEventBit(0x76);
                gCutsceneIndex = 0xAA;
                loadCutscene(arg0);
                set = 1;                      
            }
        }  

        if (!set) {
            if (!checkLifeEventBit(0x77) && npcAffection[ANN] >= 50 && (gHour - 15) < 2U && (!checkLifeEventBit(MARRIED) || gWife != ANN)) {
                setLifeEventBit(0x77);
                gCutsceneIndex = 0xB1;
                loadCutscene(arg0);
                set = 1;                      
            }
        }  

        if (!set) {
            if (!checkLifeEventBit(0x78) && npcAffection[ANN] >= 80 && (gHour - 15) < 2U && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
                setLifeEventBit(0x78);
                gCutsceneIndex = 0xB2;
                loadCutscene(arg0);
                set = 1;                      
            }
        }  
        
        if (!set) {
            if (!checkLifeEventBit(0x79) && npcAffection[ANN] >= 200 && gWeather == SUNNY && gSeason == WINTER && (gHour - 6) < 12U && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
                setLifeEventBit(0x79);
                gCutsceneIndex = 0xB5;
                loadCutscene(arg0);
                set = 1;                      
            }
        }  

        if (!set) {
            if (!checkLifeEventBit(0x7A) && npcAffection[ANN] >= 50 && (gHour - 6) < 6U && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && checkLifeEventBit(0x3C)) {
                setLifeEventBit(0x7A);
                gCutsceneIndex = 0xBC;
                loadCutscene(arg0);
                set = 1;                      
            }
        }  

        if (!set) {
            if (!checkLifeEventBit(0x7B) && npcAffection[ANN] >= 80 && (gHour - 6) < 6U && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && checkLifeEventBit(0x3C)) {
                setLifeEventBit(0x7B);
                gCutsceneIndex = 0xBD;
                loadCutscene(arg0);
                set = 1;                      
            }
        }  

        if (!set) {
            if (!checkLifeEventBit(0x7C) && npcAffection[KAREN] >= 60 && (gSeason - 2) < 2U && (gHour - 15) < 2U && (!checkLifeEventBit(MARRIED) || gWife != KAREN)) {
                setLifeEventBit(0x7C);
                gCutsceneIndex = 0xBE;
                loadCutscene(arg0);
                set = 1;                      
            }
        }  
        
        if (!set) {
            if (!checkLifeEventBit(0x7D) && npcAffection[KAREN] >= 100 && (gHour - 15) < 2U && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
                setLifeEventBit(0x7D);
                gCutsceneIndex = 0xBF;
                loadCutscene(arg0);
                set = 1;                      
            }
        }  
        
        if (!set) {
            if (!checkLifeEventBit(0x7E) && checkLifeEventBit(0x96) && npcAffection[KAREN] >= 200 && gWeather == SUNNY && gSeason == AUTUMN && gDayOfMonth == 7 && (gHour - 18) < 6U && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
                setLifeEventBit(0x7E);
                gCutsceneIndex = 0xC1;
                loadCutscene(arg0);
                set = 1;                      
            }
        }  
        
        if (!set) {
            if (!checkLifeEventBit(0x7F) && checkLifeEventBit(0x96) && gSeason == WINTER && (gDayOfMonth - 7) < 24U && (gHour - 6) < 12U) {
                
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
                set = 1;                      
            }
        }          

        if (!set) {
            if (!checkLifeEventBit(0x80) && npcAffection[HARRIS] >= 50 && gSeason == AUTUMN && (gHour - 15) < 2U) {
                setLifeEventBit(0x80);
                gCutsceneIndex = 0xCC;
                loadCutscene(arg0);
                set = 1;                      
            }
        }       
        
        if (!set) {
            if (!checkLifeEventBit(0x81) && npcAffection[CLIFF] >= 50 && (gHour - 15) < 2U) {
                setLifeEventBit(0x81);
                gCutsceneIndex = 0xD2;
                loadCutscene(arg0);
                set = 1;                      
            }
        }       
        
        if (!set) {
            if (!checkLifeEventBit(0x83) && checkLifeEventBit(0x2A) && (gHour - 6) < 12U && (!checkLifeEventBit(MARRIED) || gWife != 4) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
                setLifeEventBit(0x83);
                gCutsceneIndex = 0xDA;
                loadCutscene(arg0);
                set = 1;                      
            }
        }    

        if (!set && !arg0) {
            if (!checkLifeEventBit(0x84) && checkLifeEventBit(6) && npcAffection[RICK] >= 120 && gWeather == SUNNY && (gSeason && gSeason < 4) && (gHour - 6) < 12U && gEntranceIndex == HOUSE_EXIT) {
                setLifeEventBit(0x84);
                gCutsceneIndex = 0xDC;
                loadCutscene(arg0);
                set = 1;                             
            }
        }
        
        if (!set) {
            if (!checkLifeEventBit(0x85) && (gHour - 6) < 6U && func_8009B564() >= 5 && D_80237410 < 990) {
                setLifeEventBit(0x85);
                gCutsceneIndex = 0xE0;
                loadCutscene(arg0);
                set = 1;                             
            }
        }

        if (!set) {
            if (!checkLifeEventBit(0x86) && (gSeason && gSeason < 4) && (gHour - 6) < 6U && gTotalCropsShipped >= 900 && D_80237410 < 990) {
                setLifeEventBit(0x86);
                gCutsceneIndex = 0xE1;
                loadCutscene(arg0);
                set = 1;                             
            }
        }
        
        if (!set && !arg0) {
            if (!checkLifeEventBit(0x87) && (func_800DC008() + countPinkCatMintFlowersFarm()) >= 24 && gWeather == SUNNY && gSeason == SPRING && (gHour - 6) < 12U && gEntranceIndex == HOUSE_EXIT) {
                setLifeEventBit(0x87);
                gCutsceneIndex = 0xE4;
                loadCutscene(arg0);
                set = 1;                             
            }
        }

        if (!set && !arg0) {
            if (!checkLifeEventBit(0x88) && func_800DBF90() >= 90 && gWeather == SUNNY && gSeason == AUTUMN && (gHour - 6) < 12U && gEntranceIndex == HOUSE_EXIT) {
                setLifeEventBit(0x88);
                gCutsceneIndex = 0xE6;
                loadCutscene(arg0);
                set = 1;                             
            }
        }
        
        if (!set && !arg0) {
            if (!checkLifeEventBit(0x89) && checkLifeEventBit(0x20) && gWeather == SUNNY && gSeason == AUTUMN && (gHour - 6) < 12U) {
                setLifeEventBit(0x89);
                gCutsceneIndex = 0xE7;
                loadCutscene(arg0);
                set = 1;                             
            }
        }
        
        if (!set && !arg0) {
            if (!checkLifeEventBit(0x8A) && checkLifeEventBit(0x53) && gWeather == SUNNY && gSeason == SPRING && (gHour - 6) < 12U && gEntranceIndex == HOUSE_EXIT) {
                setLifeEventBit(0x8A);
                gCutsceneIndex = 0xE8;
                loadCutscene(arg0);
                set = 1;                             
            }
        }
        
        if (!set && !arg0) {
            if (!checkLifeEventBit(0x8B) && checkLifeEventBit(0x30) && gWeather == SUNNY && func_8009B3DC() >= 3 && gSeason == WINTER && (gHour - 6) < 12U && gEntranceIndex == HOUSE_EXIT) {
                setLifeEventBit(0x8B);
                gCutsceneIndex = 0xE9;
                loadCutscene(arg0);
                set = 1;                             
            }
        }
        
        if (!set && !arg0) {
            if (!checkLifeEventBit(0x8C) && checkLifeEventBit(0x30) && func_8009B828(1) && gWeather == SUNNY && gSeason == AUTUMN && (gHour - 6) < 12U && gEntranceIndex == HOUSE_EXIT) {
                setLifeEventBit(0x8C);
                gCutsceneIndex = 0xEA;
                loadCutscene(arg0);
                set = 1;                             
            }
        }

        if (!set && !arg0) {
            if (!checkLifeEventBit(0x8D) && (func_800DC008() + countPinkCatMintFlowersFarm()) >= 24 && gWeather == SUNNY && gSeason == SUMMER && (gHour - 6) < 12U && gEntranceIndex == HOUSE_EXIT) {
                setLifeEventBit(0x8D);
                gCutsceneIndex = 0xEC;
                loadCutscene(arg0);
                set = 1;                             
            }
        }        

        if (!set) {
            if (checkLifeEventBit(0x90) && npcAffection[DOUG] >= 60 && (gHour - 17) < 4U && ((gSeason == SPRING && gDayOfMonth == 18) || gSeason == AUTUMN && gDayOfMonth == 29)) {
                toggleLifeEventBit(0x90);
                gCutsceneIndex = 0x81;
                loadCutscene(arg0);
                set = 1;                             
            }
        }       
        
        if (!set) {
            if (checkLifeEventBit(0x91) && npcAffection[DOUG] >= 60 && gSeason == AUTUMN && gDayOfMonth == 5 && (gHour - 17) < 4U) {
                toggleLifeEventBit(0x91);
                gCutsceneIndex = 0x82;
                loadCutscene(arg0);
                set = 1;                             
            }
        }          

        if (!set) {
            if (checkLifeEventBit(0x92) && npcAffection[BASIL] >= 60 && gSeason == SUMMER && gDayOfMonth == 10 && (gHour - 17) < 4U) {
                toggleLifeEventBit(0x92);
                gCutsceneIndex = 0x8B;
                loadCutscene(arg0);
                set = 1;                             
            }
        }       
        
        if (!set) {
            if (!checkLifeEventBit(0x95) && checkLifeEventBit(0x2F) && npcAffection[KAI] <= 80 && gSeason == WINTER && (gHour - 6) < 12U && gHarvestKing != 5 && gHarvestCoinFinder != 5 && (!checkLifeEventBit(MARRIED) || gWife != 4) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
                setLifeEventBit(0x95);
                setLifeEventBit(0x2E);
                setSpecialDialogueBit(0x24);
                gCutsceneIndex = 0xDB;
                loadCutscene(arg0);
                set = 1;                             
            }
        }      
        
        if (!set) {
            if (!checkDailyEventBit(0x51) && npcAffection[POTION_SHOP_DEALER] >= 50 && gWeather == SUNNY && gYear == 1 && gSeason == WINTER && gDayOfMonth == 27 && (gHour - 6) < 3U) {
                setDailyEventBit(0x51);
                gCutsceneIndex = 0xC9;
                loadCutscene(arg0);
                set = 1;                             
            }
        } 

      if (!set) {
            if (!checkDailyEventBit(0x51) && npcAffection[HARRIS] >= 100 && gHarvestKing == 1 && gWeather == SUNNY && gSeason == WINTER && gDayOfMonth == 27 && (gHour - 6) < 3U) {
                setDailyEventBit(0x51);
                gCutsceneIndex = 0xCA;
                loadCutscene(arg0);
                set = 1;                             
            }
        }    
        
      if (!set) {
            if (!checkDailyEventBit(0x51) && npcAffection[GRAY] >= 100 && gHarvestKing == 2 && gWeather == SUNNY && gSeason == WINTER && gDayOfMonth == 27 && (gHour - 6) < 3U) {
                setDailyEventBit(0x51);
                gCutsceneIndex = 0xCD;
                loadCutscene(arg0);
                set = 1;                             
            }
        }    

      if (!set) {
            if (!checkDailyEventBit(0x51) && npcAffection[JEFF] >= 100 && gHarvestKing == 3 && gWeather == SUNNY && gSeason == WINTER && gDayOfMonth == 27 && (gHour - 6) < 3U) {
                setDailyEventBit(0x51);
                gCutsceneIndex = 0xCF;
                loadCutscene(arg0);
                set = 1;                             
            }
        }    
        
    if (!set) {
        if (!checkDailyEventBit(0x51) && npcAffection[CLIFF] >= 100 && gHarvestKing == 4 && gWeather == SUNNY && gSeason == WINTER && gDayOfMonth == 27 && (gHour - 6) < 3U) {
            setDailyEventBit(0x51);
            gCutsceneIndex = 0xD3;
            loadCutscene(arg0);
            set = 1;                             
        }
    }        
        
       if (!set) {
            if (!checkDailyEventBit(0x51) && npcAffection[KAI] >= 100 && gHarvestKing == 5 && gWeather == SUNNY && gSeason == WINTER && gDayOfMonth == 27 && (gHour - 6) < 3U) {
                setDailyEventBit(0x51);
                gCutsceneIndex = 0xD8;
                loadCutscene(arg0);
                set = 1;                             
            }
        }      
            
        if (!set && !arg0) {
            if (!checkDailyEventBit(0x18) && gSeason == SUMMER && gDayOfMonth == 1 && gWeather == SUNNY && (gHour - 18) < 3U && checkLifeEventBit(MARRIED) && gWife == MARIA) {
                setDailyEventBit(0x18);
                gCutsceneIndex = 0xEF;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = 1;                             
            }
        }
            
        if (!set && !arg0) {
            if (!checkDailyEventBit(0x19) && gSeason == SUMMER && gDayOfMonth == 1 && gWeather == SUNNY && (gHour - 18) < 3U && checkLifeEventBit(MARRIED) && gWife == POPURI) {
                setDailyEventBit(0x19);
                gCutsceneIndex = 0xF0;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = 1;                             
            }
        }
            
        if (!set && !arg0) {
            if (!checkDailyEventBit(0x1A) && gSeason == SUMMER && gDayOfMonth == 1 && gWeather == SUNNY && (gHour - 18) < 3U && checkLifeEventBit(MARRIED) && gWife == ELLI) {
                setDailyEventBit(0x1A);
                gCutsceneIndex = 0xF1;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = 1;                             
            }
        }
            
        if (!set && !arg0) {
            if (!checkDailyEventBit(0x1B) && gSeason == SUMMER && gDayOfMonth == 1 && gWeather == SUNNY && (gHour - 18) < 3U && checkLifeEventBit(MARRIED) && gWife == ANN) {
                setDailyEventBit(0x1B);
                gCutsceneIndex = 0xF2;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = 1;                             
            }
        }
            
        if (!set && !arg0) {
            if (!checkDailyEventBit(0x1C) && gSeason == SUMMER && gDayOfMonth == 1 && gWeather == SUNNY && (gHour - 18) < 3U && checkLifeEventBit(MARRIED) && gWife == KAREN) {
                setDailyEventBit(0x1C);
                gCutsceneIndex = 0xF3;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = 1;                             
            }
        }
    
        if (!set && !arg0) {
            if (!checkDailyEventBit(0x18) && gSeason == WINTER && gDayOfMonth == 24 && gWeather == SUNNY && (gHour - 18) < 3U && checkLifeEventBit(MARRIED) && gWife == MARIA) {
                setDailyEventBit(0x18);
                gCutsceneIndex = 0xF4;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = 1;                             
            }
        }
    
        if (!set && !arg0) {
            if (!checkDailyEventBit(0x19) && gSeason == WINTER && gDayOfMonth == 24 && gWeather == SUNNY && (gHour - 18) < 3U && checkLifeEventBit(MARRIED) && gWife == POPURI) {
                setDailyEventBit(0x19);
                gCutsceneIndex = 0xF5;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = 1;                             
            }
        }
    
        if (!set && !arg0) {
            if (!checkDailyEventBit(0x1A) && gSeason == WINTER && gDayOfMonth == 24 && gWeather == SUNNY && (gHour - 18) < 3U && checkLifeEventBit(MARRIED) && gWife == ELLI) {
                setDailyEventBit(0x1A);
                gCutsceneIndex = 0xF6;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = 1;                             
            }
        }
    
        if (!set && !arg0) {
            if (!checkDailyEventBit(0x1B) && gSeason == WINTER && gDayOfMonth == 24 && gWeather == SUNNY && (gHour - 18) < 3U && checkLifeEventBit(MARRIED) && gWife == ANN) {
                setDailyEventBit(0x1B);
                gCutsceneIndex = 0xF7;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = 1;                             
            }
        }
    
        if (!set && !arg0) {
            if (!checkDailyEventBit(0x1C) && gSeason == WINTER && gDayOfMonth == 24 && gWeather == SUNNY && (gHour - 18) < 3U && checkLifeEventBit(MARRIED) && gWife == KAREN) {
                setDailyEventBit(0x1C);
                gCutsceneIndex = 0xF8;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = 1;                             
            }
        }
    
        if (!set && !arg0) {
            if (!checkDailyEventBit(0x18) && gSeason == SUMMER && gDayOfMonth == 7 && gWeather == SUNNY && (gHour - 17) < 4U && checkLifeEventBit(MARRIED) && gWife == MARIA && npcAffection[MARIA] >= 220) {
                setDailyEventBit(0x18);
                gCutsceneIndex = 0x96;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = 1;                             
            }
        }
            
        if (!set && !arg0) {
            if (!checkDailyEventBit(0x19) && gSeason == SUMMER && gDayOfMonth == 7 && gWeather == SUNNY && (gHour - 17) < 4U && checkLifeEventBit(MARRIED) && gWife == POPURI && npcAffection[POPURI] >= 220) {
                setDailyEventBit(0x19);
                gCutsceneIndex = 0xA1;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = 1;                             
            }
        }
    
        if (!set && !arg0) {
            if (!checkDailyEventBit(0x1A) && gSeason == SUMMER && gDayOfMonth == 7 && gWeather == SUNNY && (gHour - 17) < 4U && checkLifeEventBit(MARRIED) && gWife == ELLI && npcAffection[ELLI] >= 220) {
                setDailyEventBit(0x1A);
                gCutsceneIndex = 0xAD;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = 1;                             
            }
        }
    
        if (!set && !arg0) {
            if (!checkDailyEventBit(0x1B) && gSeason == SUMMER && gDayOfMonth == 7 && gWeather == SUNNY && (gHour - 17) < 4U && checkLifeEventBit(MARRIED) && gWife == ANN && npcAffection[ANN] >= 220) {
                setDailyEventBit(0x1B);
                gCutsceneIndex = 0xB8;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = 1;                             
            }
        }
    
        if (!set && !arg0) {
            if (!checkDailyEventBit(0x1C) && gSeason == SUMMER && gDayOfMonth == 7 && gWeather == SUNNY && (gHour - 17) < 4U && checkLifeEventBit(MARRIED) && gWife == KAREN && npcAffection[KAREN] >= 220) {
                setDailyEventBit(0x1C);
                gCutsceneIndex = 0xC5;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = 1;                             
            }
        }
            
        if (!set && !arg0) {
            if (!checkDailyEventBit(0x18) && gSeason == AUTUMN && gDayOfMonth == 9 && gWeather == SUNNY && (gHour - 17) < 4U && checkLifeEventBit(MARRIED) && gWife == MARIA && npcAffection[MARIA] >= 220) {
                setDailyEventBit(0x18);
                gCutsceneIndex = 0x97;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = 1;                             
            }
        }       
    
        if (!set && !arg0) {
            if (!checkDailyEventBit(0x19) && gSeason == AUTUMN && gDayOfMonth == 9 && gWeather == SUNNY && (gHour - 17) < 4U && checkLifeEventBit(MARRIED) && gWife == POPURI && npcAffection[POPURI] >= 220) {
                setDailyEventBit(0x19);
                gCutsceneIndex = 0xA2;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = 1;                             
            }
        }      
    
        if (!set && !arg0) {
            if (!checkDailyEventBit(0x1A) && gSeason == AUTUMN && gDayOfMonth == 9 && gWeather == SUNNY && (gHour - 17) < 4U && checkLifeEventBit(MARRIED) && gWife == ELLI && npcAffection[ELLI] >= 220) {
                setDailyEventBit(0x1A);
                gCutsceneIndex = 0xAE;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = 1;                             
            }
        }        
            
        if (!set && !arg0) {
            if (!checkDailyEventBit(0x1B) && gSeason == AUTUMN && gDayOfMonth == 9 && gWeather == SUNNY && (gHour - 17) < 4U && checkLifeEventBit(MARRIED) && gWife == ANN && npcAffection[ANN] >= 220) {
                setDailyEventBit(0x1B);
                gCutsceneIndex = 0xB9;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = 1;                             
            }
        }     
            
        if (!set && !arg0) {
            if (!checkDailyEventBit(0x1C) && gSeason == AUTUMN && gDayOfMonth == 9 && gWeather == SUNNY && (gHour - 17) < 4U && checkLifeEventBit(MARRIED) && gWife == KAREN && npcAffection[KAREN] >= 220) {
                setDailyEventBit(0x1C);
                gCutsceneIndex = 0xC6;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = 1;                             
            }
        }       

    }

    return result;
}


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

    if (!checkLifeEventBit(0x48) && gSeason == AUTUMN && (gDayOfMonth - 23) < 5U && (gHour - 6) < 9U) {
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
        if (!checkLifeEventBit(0xA4) && npcAffection[JEFF] >= 0xB4 && gWeather == SUNNY && gSeason == SUMMER && DAYTIME && (!checkLifeEventBit(MARRIED) || gWife != ELLI) && !checkLifeEventBit(ELLI_JEFF_MARRIED) && gEntranceIndex == TOP_OF_MOUNTAIN_1_SUMMER) {
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

//INCLUDE_ASM(const s32, "setCutscenes", setMoonMountainCutscenes);

// mountain 2
u16 setMoonMountainCutscenes(void) {

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
        if (!checkLifeEventBit(0xD5) && checkLifeEventBit(0x9F) && (blueMistFlowerPlot >= 0xC2) && (gSeason < AUTUMN) && DAYTIME) {
            setLifeEventBit(0xD5);
            setDailyEventBit(0x46);
            gCutsceneIndex = 0x13C;
            gCutsceneFlags |= 6;
            loadCutscene(0);
            set = 1;
        }
    }

    if (!set) {
         if (!checkLifeEventBit(0xD6) && checkLifeEventBit(0xD5) && (blueMistFlowerPlot >= 0xC2) && (gSeason < AUTUMN) && DAYTIME) {
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

    if (!checkLifeEventBit(0xCC) && npcAffection[CLIFF] >= 200 && gSeason == WINTER && DAYTIME && (!checkLifeEventBit(MARRIED) || gWife != 3) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && previousMapIndex == 0x52) {
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
 
u16 func_800A7AE8(u8 index) {

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
            
            if (!checkDailyEventBit(0x1D) && !checkDailyEventBit(0x1E) && gSeason == AUTUMN && gDayOfMonth == 3 && (gHour - 6) < 9U && func_8009B2BC(2)) {
               
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
            nuPiReadRom(cutsceneMapAddresses[index].romAddr, D_80114D30[index], cutsceneMapAddresses[index].vaddr - cutsceneMapAddresses[index].romAddr);
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