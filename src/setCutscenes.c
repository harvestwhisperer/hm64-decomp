#include "common.h"

#include "setCutscenes.h"

#include "system/cutscene.h"
#include "system/message.h"
#include "system/sprite.h"

#include "animals.h"
#include "evaluation.h"
#include "game.h"
#include "gameStatus.h"
#include "itemHandlers.h"
#include "mapObjects.h"
#include "level.h"
#include "npc.h"
#include "overlayScreens.h"
#include "player.h"
#include "spriteIndices.h"
#include "weather.h"
                                  
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
s32 D_801891D4;
extern u16 D_8018981C;

// likely bss
extern u16 gCutsceneIndex;

// shared globals   
extern u8 gHouseExtensionSelection;
extern u8 D_8016FE6F;
// bakery points
extern u8 bakeryCardPoints;
extern u8 D_801C4216;
// chicken feed count
extern u16 chickenFeedQuantity;

// evaluation fish caught score
extern u16 D_801D62C4;
// evaluation: animals
extern u8 D_801D62C6;
 
// data 
extern Addresses cutsceneMapAddresses[];
// cutscene master object vaddr
extern void *D_80114C70[];
// entire cutscene map vaddr
extern void *D_80114D30[];
// cutscene map asset index
extern u16 D_80114DF0[];
extern u8 D_80113580[0x18][0x14];


//INCLUDE_ASM(const s32, "setCutscenes", func_8009BF90);

// sets master cutscene map object
inline void func_8009BF90(u16 arg0) {

    nuPiReadRom(cutsceneMapAddresses[arg0].romAddrStart, D_80114D30[arg0], cutsceneMapAddresses[arg0].romAddrEnd - cutsceneMapAddresses[arg0].romAddrStart);

    D_801891D4 = 0;

    // param 1 = 0, 20, 21, or 22
    func_800469A8(D_80114DF0[arg0], D_80114C70[arg0]);
    
    func_8002F730();
    
    D_8018981C = arg0;

    gCutsceneFlags |= 1;

    toggleDailyEventBit(0x53);
    
}

// jtbl_80120EC0
//INCLUDE_ASM(const s32, "setCutscenes", func_8009C054);

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

// jtbl_80121030
//INCLUDE_ASM(const s32, "setCutscenes", func_8009C324);

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
                    func_8003FBA0(0xE, gFarmAnimals[func_8009B464()].name, 6);
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
            if (!checkDailyEventBit(0x19) && npcAffection[POPURI] >= 50 && gSeason == SPRING && gDayOfMonth == 22 && (5 < gHour && gHour < 9) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
                setDailyEventBit(0x19);
                gCutsceneIndex = 0x9C;
                loadCutscene(arg0);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(0x1A) && npcAffection[ELLI] >= 50 && gSeason == AUTUMN && gDayOfMonth == 11 && (6 < gHour && gHour < 9) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {
                setDailyEventBit(0x1A);
                gCutsceneIndex = 0xA8;
                loadCutscene(arg0);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(0x1B) && npcAffection[ANN] >= 80 && gSeason == SPRING && gDayOfMonth == 16 && (6 < gHour && gHour < 9) && !checkLifeEventBit(MARRIED) && checkLifeEventBit(0x51) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
                setDailyEventBit(0x1B);
                gCutsceneIndex = 0xB3;
                loadCutscene(arg0);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(0x18) && npcAffection[MARIA] >= 50 && gSeason == SUMMER && gDayOfMonth == 1 && gHour == 8 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
                setDailyEventBit(0x18);
                gCutsceneIndex = MARIA_FIREWORKS_INVITATION;
                loadCutscene(arg0);
                set = TRUE;
            }            
        }
        
        if (!set) {
            if (!checkDailyEventBit(0x19) && npcAffection[POPURI] >= 50 && gSeason == SUMMER && gDayOfMonth == 1 && gHour == 10 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
                setDailyEventBit(0x19);
                gCutsceneIndex = POPURI_FIREWORKS_INVITATION;
                loadCutscene(arg0);
                set = TRUE;
            }            
        }

        if (!set) {
            if (!checkDailyEventBit(0x1A) && npcAffection[ELLI] >= 50 && gSeason == SUMMER && gDayOfMonth == 1 && gHour == 12 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {
                setDailyEventBit(0x1A);
                gCutsceneIndex = ELLI_FIREWORKS_INVITATION;
                loadCutscene(arg0);
                set = TRUE;
            }            
        }

        if (!set) {
            if (!checkDailyEventBit(0x1B) && npcAffection[ANN] >= 50 && gSeason == SUMMER && gDayOfMonth == 1 && gHour == 14 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
                setDailyEventBit(0x1B);
                gCutsceneIndex = ANN_FIREWORKS_INVITATION;
                loadCutscene(arg0);
                set = TRUE;
            }            
        }

        if (!set) {
            if (!checkDailyEventBit(0x1C) && npcAffection[KAREN] >= 50 && gSeason == SUMMER && gDayOfMonth == 1 && gHour == 16 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
                setDailyEventBit(0x1C);
                gCutsceneIndex = KAREN_FIREWORKS_INVITATION;
                loadCutscene(arg0);
                set = TRUE;
            }            
        }
        
        if (!set) {
            if (!checkDailyEventBit(0x18) && npcAffection[MARIA] >= 140 && gSeason == WINTER && gDayOfMonth == 10 && gHour == 8 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
                setDailyEventBit(0x18);
                gCutsceneIndex = MARIA_THANKSGIVING_GIFT;
                loadCutscene(arg0);
                set = TRUE;
            }            
        }        

        if (!set) {
            if (!checkDailyEventBit(0x19) && npcAffection[POPURI] >= 140 && gSeason == WINTER && gDayOfMonth == 10 && gHour == 10 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
                setDailyEventBit(0x19);
                gCutsceneIndex = POPURI_THANKSGIVING_GIFT;
                loadCutscene(arg0);
                set = TRUE;
            }            
        }        

        if (!set) {
            if (!checkDailyEventBit(0x1A) && npcAffection[ELLI] >= 140 && gSeason == WINTER && gDayOfMonth == 10 && gHour == 12 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {
                setDailyEventBit(0x1A);
                gCutsceneIndex = ELLI_THANKSGIVING_GIFT;
                loadCutscene(arg0);
                set = TRUE;
            }            
        }        

        if (!set) {
            if (!checkDailyEventBit(0x1B) && npcAffection[ANN] >= 140 && gSeason == WINTER && gDayOfMonth == 10 && gHour == 14 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
                setDailyEventBit(0x1B);
                gCutsceneIndex = ANN_THANKSGIVING_GIFT;
                loadCutscene(arg0);
                set = TRUE;
            }            
        }    

        if (!set) {
            if (!checkDailyEventBit(0x1C) && npcAffection[KAREN] >= 140 && gSeason == WINTER && gDayOfMonth == 10 && gHour == 16 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
                setDailyEventBit(0x1C);
                gCutsceneIndex = KAREN_THANKSGIVING_GIFT;
                loadCutscene(arg0);
                set = TRUE;
            }            
        }       

        if (!set) {
            if (!checkDailyEventBit(0x18) && npcAffection[MARIA] >= 100 && gSeason == WINTER && gDayOfMonth == 24 && gHour == 8 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
                setDailyEventBit(0x18);
                gCutsceneIndex = 0x95;
                loadCutscene(arg0);
                set = TRUE;
            }            
        }        

        if (!set) {
            if (!checkDailyEventBit(0x19) && npcAffection[POPURI] >= 100 && gSeason == WINTER && gDayOfMonth == 24 && gHour == 10 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
                setDailyEventBit(0x19);
                gCutsceneIndex = 0xA0;
                loadCutscene(arg0);
                set = TRUE;
            }            
        }        

        if (!set) {
            if (!checkDailyEventBit(0x1A) && npcAffection[ELLI] >= 100 && gSeason == WINTER && gDayOfMonth == 24 && gHour == 12 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {
                setDailyEventBit(0x1A);
                gCutsceneIndex = 0xAC;
                loadCutscene(arg0);
                set = TRUE;
            }            
        }       

        if (!set) {
            if (!checkDailyEventBit(0x1B) && npcAffection[ANN] >= 100 && gSeason == WINTER && gDayOfMonth == 24 && gHour == 14 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
                setDailyEventBit(0x1B);
                gCutsceneIndex = 0xB7;
                loadCutscene(arg0);
                set = TRUE;
            }            
        }       

        if (!set) {
            if (!checkDailyEventBit(0x1C) && npcAffection[KAREN] >= 100 && gSeason == WINTER && gDayOfMonth == 24 && gHour == 16 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
                setDailyEventBit(0x1C);
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
            if (!checkLifeEventBit(0x6B) && countPinkCatMintFlowersFarm() && gWeather == SUNNY && (gSeason && gSeason < 3) && (5 < gHour && gHour < 12)) {
                setLifeEventBit(0x6B);
                gCutsceneIndex = 0x73;
                loadCutscene(arg0);
                set = TRUE;    
            }
        }        

        if (!set) {
            if (!checkLifeEventBit(0x6C) && countPinkCatMintFlowersFarm() >= 16 && npcAffection[STU] >= 80 && gWeather == SUNNY && (gSeason && gSeason < 3) && (5 < gHour && gHour < 12)) {
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
            if (!checkLifeEventBit(0x6F) && npcAffection[MARIA] >= 200 && gWeather == SUNNY && gSeason == SUMMER && (17 < gHour && gHour < 24) && !checkDailyEventBit(FESTIVAL_DAY) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
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
                D_8016FE6F = 0;
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
            if (!checkLifeEventBit(0x87) && (func_800DC008() + countPinkCatMintFlowersFarm()) >= 24 && gWeather == SUNNY && gSeason == SPRING && DAYTIME && gEntranceIndex == HOUSE_EXIT) {
                setLifeEventBit(0x87);
                gCutsceneIndex = 0xE4;
                loadCutscene(arg0);
                set = TRUE;                             
            }
        }

        if (!set && arg0 == 0) {
            if (!checkLifeEventBit(0x88) && func_800DBF90() >= 90 && gWeather == SUNNY && gSeason == AUTUMN && DAYTIME && gEntranceIndex == HOUSE_EXIT) {
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
            if (!checkLifeEventBit(0x8D) && (func_800DC008() + countPinkCatMintFlowersFarm()) >= 24 && gWeather == SUNNY && gSeason == SUMMER && DAYTIME && gEntranceIndex == HOUSE_EXIT) {
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
            if (!checkDailyEventBit(0x18) && gSeason == SUMMER && gDayOfMonth == 1 && gWeather == SUNNY && (17 < gHour && gHour < 21) && MARRIED_TO_MARIA) {
                setDailyEventBit(0x18);
                gCutsceneIndex = 0xEF;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
            
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(0x19) && gSeason == SUMMER && gDayOfMonth == 1 && gWeather == SUNNY && (17 < gHour && gHour < 21) && MARRIED_TO_POPURI) {
                setDailyEventBit(0x19);
                gCutsceneIndex = 0xF0;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
            
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(0x1A) && gSeason == SUMMER && gDayOfMonth == 1 && gWeather == SUNNY && (17 < gHour && gHour < 21) && MARRIED_TO_ELLI) {
                setDailyEventBit(0x1A);
                gCutsceneIndex = 0xF1;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
            
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(0x1B) && gSeason == SUMMER && gDayOfMonth == 1 && gWeather == SUNNY && (17 < gHour && gHour < 21) && MARRIED_TO_ANN) {
                setDailyEventBit(0x1B);
                gCutsceneIndex = 0xF2;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
            
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(0x1C) && gSeason == SUMMER && gDayOfMonth == 1 && gWeather == SUNNY && (17 < gHour && gHour < 21) && MARRIED_TO_KAREN) {
                setDailyEventBit(0x1C);
                gCutsceneIndex = 0xF3;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
    
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(0x18) && gSeason == WINTER && gDayOfMonth == 24 && gWeather == SUNNY && (17 < gHour && gHour < 21) && MARRIED_TO_MARIA) {
                setDailyEventBit(0x18);
                gCutsceneIndex = 0xF4;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
    
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(0x19) && gSeason == WINTER && gDayOfMonth == 24 && gWeather == SUNNY && (17 < gHour && gHour < 21) && MARRIED_TO_POPURI) {
                setDailyEventBit(0x19);
                gCutsceneIndex = 0xF5;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
    
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(0x1A) && gSeason == WINTER && gDayOfMonth == 24 && gWeather == SUNNY && (17 < gHour && gHour < 21) && MARRIED_TO_ELLI) {
                setDailyEventBit(0x1A);
                gCutsceneIndex = 0xF6;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
    
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(0x1B) && gSeason == WINTER && gDayOfMonth == 24 && gWeather == SUNNY && (17 < gHour && gHour < 21) && MARRIED_TO_ANN) {
                setDailyEventBit(0x1B);
                gCutsceneIndex = 0xF7;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
    
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(0x1C) && gSeason == WINTER && gDayOfMonth == 24 && gWeather == SUNNY && (17 < gHour && gHour < 21) && MARRIED_TO_KAREN) {
                setDailyEventBit(0x1C);
                gCutsceneIndex = 0xF8;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
    
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(0x18) && gSeason == SUMMER && gDayOfMonth == 7 && gWeather == SUNNY && (16 < gHour && gHour < 21) && MARRIED_TO_MARIA && npcAffection[MARIA] >= 220) {
                setDailyEventBit(0x18);
                gCutsceneIndex = 0x96;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
            
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(0x19) && gSeason == SUMMER && gDayOfMonth == 7 && gWeather == SUNNY && (16 < gHour && gHour < 21) && MARRIED_TO_POPURI && npcAffection[POPURI] >= 220) {
                setDailyEventBit(0x19);
                gCutsceneIndex = 0xA1;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
    
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(0x1A) && gSeason == SUMMER && gDayOfMonth == 7 && gWeather == SUNNY && (16 < gHour && gHour < 21) && MARRIED_TO_ELLI && npcAffection[ELLI] >= 220) {
                setDailyEventBit(0x1A);
                gCutsceneIndex = 0xAD;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
    
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(0x1B) && gSeason == SUMMER && gDayOfMonth == 7 && gWeather == SUNNY && (16 < gHour && gHour < 21) && MARRIED_TO_ANN && npcAffection[ANN] >= 220) {
                setDailyEventBit(0x1B);
                gCutsceneIndex = 0xB8;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
    
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(0x1C) && gSeason == SUMMER && gDayOfMonth == 7 && gWeather == SUNNY && (16 < gHour && gHour < 21) && MARRIED_TO_KAREN && npcAffection[KAREN] >= 220) {
                setDailyEventBit(0x1C);
                gCutsceneIndex = 0xC5;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
            
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(0x18) && gSeason == AUTUMN && gDayOfMonth == 9 && gWeather == SUNNY && (16 < gHour && gHour < 21) && MARRIED_TO_MARIA && npcAffection[MARIA] >= 220) {
                setDailyEventBit(0x18);
                gCutsceneIndex = 0x97;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }       
    
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(0x19) && gSeason == AUTUMN && gDayOfMonth == 9 && gWeather == SUNNY && (16 < gHour && gHour < 21) && MARRIED_TO_POPURI && npcAffection[POPURI] >= 220) {
                setDailyEventBit(0x19);
                gCutsceneIndex = 0xA2;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }      
    
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(0x1A) && gSeason == AUTUMN && gDayOfMonth == 9 && gWeather == SUNNY && (16 < gHour && gHour < 21) && MARRIED_TO_ELLI && npcAffection[ELLI] >= 220) {
                setDailyEventBit(0x1A);
                gCutsceneIndex = 0xAE;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }        
            
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(0x1B) && gSeason == AUTUMN && gDayOfMonth == 9 && gWeather == SUNNY && (16 < gHour && gHour < 21) && MARRIED_TO_ANN && npcAffection[ANN] >= 220) {
                setDailyEventBit(0x1B);
                gCutsceneIndex = 0xB9;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }     
            
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(0x1C) && gSeason == AUTUMN && gDayOfMonth == 9 && gWeather == SUNNY && (16 < gHour && gHour < 21) && MARRIED_TO_KAREN && npcAffection[KAREN] >= 220) {
                setDailyEventBit(0x1C);
                gCutsceneIndex = 0xC6;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }       

    }

    return result;
}


//INCLUDE_ASM(const s32, "setCutscenes", setFarmCutscenes);

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

//INCLUDE_ASM(const s32, "setCutscenes", setMountain1Cutscenes);

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
      if (!checkLifeEventBit(0xAD) && !checkDailyEventBit(FESTIVAL_DAY) && npcAffection[ELLI] >= 180 && gWeather == SUNNY && DAYTIME && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {
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
INCLUDE_ASM(const s32, "setCutscenes", func_800A1C04);

//INCLUDE_ASM(const s32, "setCutscenes", func_800A2250);

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

//INCLUDE_ASM(const s32, "setCutscenes", setMoonMountainCutscenes);

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
            gCutsceneIndex = 0x131;
            loadCutscene(0);
            gCutsceneFlags |= (2 | 4);
        }
    }
    
    return index;
}

//INCLUDE_ASM(const s32, "setCutscenes", setSpringCutscenes);

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

//INCLUDE_ASM(const s32, "setCutscenes", setHarvestSpriteCaveCutscenes);

u16 setHarvestSpriteCaveCutscenes(void) {

    u16 index = 0xFFFF;
    
    if (checkHaveKeyItem(MARBLES) && npcAffection[HARVEST_SPRITE_1] >= 80 && DAYTIME) {
        gCutsceneIndex = 0x142;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
    }

    return index;

}

//INCLUDE_ASM(const s32, "setCutscenes", setCaveCutscenes);

u16 setCaveCutscenes(void) {

    u16 index = 0xFFFF;
    if (gSeason == WINTER && (7 < gDayOfMonth && gDayOfMonth < 31) && (5 < gHour && gHour < 16)) {
        gCutsceneIndex = 0x15A;
        gCutsceneFlags |= 2;
        loadCutscene(0);
    }

    return index;

}

// jtbl_80121048
//INCLUDE_ASM(const s32, "setCutscenes", func_800A29B0);

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
INCLUDE_ASM(const s32, "setCutscenes", func_800A2FA8);

// ranch
INCLUDE_ASM(const s32, "setCutscenes", func_800A3F04);

//INCLUDE_ASM(const s32, "setCutscenes", setRanchStoreCutscenes);

u16 setRanchStoreCutscenes(void) {

    u16 index = 0xFFFF;
     
    if (!(checkLifeEventBit(ANN_SICK_VISIT)) && !checkDailyEventBit(FESTIVAL_DAY) && npcAffection[ANN] >= 120 && RAINING && gDayOfWeek == SUNDAY && DAYTIME && NOT_MARRIED && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
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

//INCLUDE_ASM(const s32, "setCutscenes", func_800A4878);

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
        if (!checkLifeEventBit(0xC4) && !checkDailyEventBit(FESTIVAL_DAY) && npcAffection[KAREN] >= 180 && gWeather == SUNNY && DAYTIME && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
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

//INCLUDE_ASM(const s32, "setCutscenes", setVineyardHouseCutscenes);

u16 setVineyardHouseCutscenes(void) {
    
    u16 index = 0xFFFF;
    
    if (!checkLifeEventBit(KAREN_SICK_VISIT) && !checkDailyEventBit(FESTIVAL_DAY) && npcAffection[KAREN] >= 120U && RAINING && DAYTIME && NOT_MARRIED && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
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

//INCLUDE_ASM(const s32, "setCutscenes", setVineyardCellarCutscenes);

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

// village 1
INCLUDE_ASM(const s32, "setCutscenes", func_800A4E50);

// village 2
INCLUDE_ASM(const s32, "setCutscenes", func_800A5314);

//INCLUDE_ASM(const s32, "setCutscenes", setRickStoreCutscenes);

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

//INCLUDE_ASM(const s32, "setCutscenes", setFlowerShopCutscenes);

u16 setFlowerShopCutscenes(void) {
    
    u16 index = 0xFFFF;
    
    if (!checkLifeEventBit(POPURI_SICK_CUTSCENE) && !checkDailyEventBit(FESTIVAL_DAY) && npcAffection[POPURI] >= 120 && RAINING && DAYTIME && NOT_MARRIED && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
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

// bakery
INCLUDE_ASM(const s32, "setCutscenes", func_800A5F48);

//INCLUDE_ASM(const s32, "setCutscenes", func_800A6440);

// mayor house
u16 func_800A6440(void) {

    u16 index = 0xFFFF;
    bool set = FALSE;

    if (!checkDailyEventBit(0x18) && gSeason == WINTER && gDayOfMonth == 10 && (5 < gHour && gHour < 18) && !checkLifeEventBit(MARRIED)) {
        setDailyEventBit(0x18);
        gCutsceneIndex = THANKSGIVING_MAYOR_HOUSE;
        set = TRUE;
        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
    }

    if (!set && !checkLifeEventBit(MARIA_SICK_CUTSCENE) && !checkDailyEventBit(FESTIVAL_DAY) && npcAffection[MARIA] >= 120 && (SUNNY < gWeather && gWeather < 4) && gDayOfWeek == MONDAY && (8 < gHour && gHour < 17) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
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

//INCLUDE_ASM(const s32, "setCutscenes", func_800A6634);

// library
u16 func_800A6634(void) {

    u16 index = 0xFFFF;
    bool set = FALSE;

    if (!checkLifeEventBit(MARIA_SPRAINED_ANKLE_CUTSCENE) && !checkDailyEventBit(FESTIVAL_DAY) && npcAffection[MARIA] >= 180 && (8 < gHour && gHour < 17) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
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

//INCLUDE_ASM(const s32, "setCutscenes", setChurchCutscenes);

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

INCLUDE_ASM(const s32, "setCutscenes", func_800A68C0);

INCLUDE_ASM(const s32, "setCutscenes", func_800A6A14);

//INCLUDE_ASM(const s32, "setCutscenes", func_800A6EE4);

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

//INCLUDE_ASM(const s32, "setCutscenes", setBeachCutscenes);

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
        if (!checkDailyEventBit(0x1C) && gSeason == WINTER && gDayOfMonth == 10 && DAYTIME && NOT_MARRIED) {
            setDailyEventBit(0x1C);
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

//INCLUDE_ASM(const s32, "setCutscenes", setRoadCutscenes);

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
        if (!checkLifeEventBit(ANN_SPRAINED_ANKLE_CUTSCENE) && !checkDailyEventBit(FESTIVAL_DAY) && npcAffection[ANN] >= 180 && DAYTIME && NOT_MARRIED && !checkLifeEventBit(ANN_CLIFF_MARRIED) && previousMapIndex == FARM) {
            setLifeEventBit(ANN_SPRAINED_ANKLE_CUTSCENE);
            setDailyEventBit(0x59);
            gCutsceneIndex = ANN_SPRAINED_ANKLE;
            loadCutscene(0);
            gCutsceneFlags |= (2 | 4); 
        }
    }
    
    return index;

}

//INCLUDE_ASM(const s32, "setCutscenes", func_800A7AE8);
 
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

// jtbl_80121060
// jtbl_80121078
//INCLUDE_ASM(const s32, "setCutscenes", func_800A7DFC);

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
            
            D_801C3E1C = gTotalCropsShipped + gTotalGoldenMilkShipped + gTotalEggsShipped;
            
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
            
            D_80183690 = gSeason;
            D_801C6220 = gDayOfMonth;
            
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
        if (!checkDailyEventBit(FESTIVAL_DAY)) { 
            
            if (!set && !checkLifeEventBit(MARIA_HARRIS_MARRIED) && (!checkLifeEventBit(MARRIED) || gWife != 0) && npcAffection[HARRIS] >= 200 && (checkLifeEventBit(MARRIED) || npcAffection[MARIA] < 120))  {
                
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
        if (!set && checkLifeEventBit(ANIMAL_DIED) && !checkDailyEventBit(FESTIVAL_DAY)) {
            
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
            
            gPlayer.fatigue.counter = 0;
            
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
        func_80087D5C();
    }

    return result;
}

// dreams
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