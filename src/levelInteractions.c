#include "common.h"

#include "levelInteractions.h"

#include "system/controller.h"
#include "system/dialogue.h"
#include "system/map.h"
#include "system/mathUtils.h"
#include "system/message.h"
#include "system/sprite.h"

#include "animals.h"
#include "game.h"
#include "gameAudio.h"
#include "gameStatus.h"
#include "initialize2.h"
#include "itemHandlers.h"
#include "level.h"
#include "mainLoop.h"
#include "npc.h"
#include "player.h"
#include "shop.h"
#include "spriteIndices.h"
#include "weather.h"

// bss
// struct
extern LevelInteractionsInfo levelInteractionsInfo;

u8 D_80189826; 
u8 D_801C3E2C;

// forward declarations
static bool func_800AD8D0(u16, u8);
static bool func_800ADCDC(u16, u8);
static bool func_800AE00C(u16, u8);
static bool func_800AEB54(u16, u8);
static bool func_800AED60(u16, u8);
static bool func_800AEE8C(u16, u8);
static bool func_800AF060(u16, u8);
static bool func_800AF0B0(u16, u8);
static bool func_800AF494(u16, u8);
static bool func_800AFA2C(u16, u8);
static bool func_800AFA7C(u16, u8);
static bool func_800AFCD0(u16, u8);
static bool func_800AFD20(u16, u8);
static bool func_800AFF9C(u16, u8);
static bool func_800B00E0(u16, u8);
static bool func_800B01EC(u16, u8);
static bool func_800B0378(u16, u8);
static bool func_800B0714(u16, u8);
static bool func_800B0A64(u16, u8);
static bool func_800B0AFC(u16, u8);
static bool func_800B0C48(u16, u8);
static bool func_800B0C98(u16, u8);
static bool func_800B0FB8(u16, u8);
static bool func_800B106C(u16, u8);
static bool func_800B1438(u16, u8);
static bool func_800B1540(u16, u8);
static bool func_800B1808(u16, u8);
static bool func_800B1994(u16, u8);
static bool func_800B1AC4(u16, u8);
static bool handleCarpenterHutInteractions(u16, u8);
static bool handleDumplingHouseExit(u16, u8);
static bool func_800B1DBC(u16, u8);
static bool func_800B20C8(u16, u8);
static bool func_800B2118(u16, u8);
static bool func_800B2264(u16, u8);
static bool func_800B2340(u16, u8);
static bool func_800B23A4(u16, u8);
static bool func_800B24D4(u16, u8);
static bool func_800B256C(u16, u8);
static bool func_800B2604(u16, u8);
static bool func_800B27CC(u16, u8);
static bool handleRanchStoreExits(u16, u8);
static bool func_800B2B90(u16, u8);
static bool func_800B2C28(u16, u8);
static bool func_800B2C78(u16, u8);
static bool func_800B1EE4(u16, u8);
static bool func_800B2078(u16, u8);

                                                                  
//INCLUDE_ASM(const s32, "levelInteractions", func_800ACD70);

u8 func_800ACD70(u16 mapIndex) {

    u8 result = 0xFF;
    
    u8 temp;
    
    if ((mapIndex == FARM || mapIndex == COOP || mapIndex == BARN || mapIndex == GREENHOUSE)) {

        temp = func_800309B4(0, 0, 32.0f);

        if (temp == 0x10) {
            result = temp;
        }
    }

    return result;
}

//INCLUDE_ASM(const s32, "levelInteractions", func_800ACDF4);

u8 func_800ACDF4(u16 mapIndex) {

    u8 result = 0xFF;
    
    u8 temp;
    u8 temp2;
    
    if (mapIndex == BARN) {

        temp = func_800309B4(0, 0, 32.0f);
        
        if (17 < temp && temp < 27) {
            result = temp;
        }
    }

    return result;
}

//INCLUDE_ASM(const s32, "levelInteractions", func_800ACE50);

// coop/chicken feed
bool func_800ACE50(u16 mapIndex) {

    u8 temp;
    u8 result = 0xFF;
    
    if (mapIndex == COOP) {
        
        temp = func_800309B4(0, 0, 32.0f);

        if (18 < temp && temp < 25) {
            result = temp;
        }

    }
    
    return result;
}

//INCLUDE_ASM(const s32, "levelInteractions", func_800ACEAC);

u8 func_800ACEAC(u16 mapIndex) {

    u8 result = 0xFF;
    
    if (mapIndex == FARM) {

        // for reference
        // -(cVar1 != '\x17') | 1;
        //result = -(func_800309B4(0, 0, 32.0f) != 0x17) | 1;
        //result = -((func_800309B4(0, 0, 32.0f) != 0x17) ? 1 : 0) | 1;        

        result = func_800309B4(0, 0, 32.0f) == 0x17 ? 1 : 0xFF;

    }

    return result;
}

//INCLUDE_ASM(const s32, "levelInteractions", func_800ACEF8);

u8 func_800ACEF8(u16 mapIndex) {
    
    u8 result;
    u8 temp;
     
    result = 0xFF;
    
    if (mapIndex == FARM) {
        temp = func_800309B4(0, 0.0f, 32.0f);
        result = ((0x1A < temp && temp < 0x1D) || temp == 0x1D) == FALSE ? 0xFF : 1;
    }
    
    if (mapIndex == MOUNTAIN_1) {
        temp = func_800309B4(0, 0.0f, 32.0f);
        if (temp == 0x10 || temp == 0x14) {
            result = TRUE;
        }
    }
    
    if (mapIndex == POND) {
        if (func_800309B4(0, 0.0f, 32.0f) == 0x10) {
            result = TRUE;
        }
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "levelInteractions", func_800ACFE8);

bool func_800ACFE8(u16 mapIndex) {

    bool result = FALSE;
    u8 temp;
    
    if (mapIndex == FARM) {
        temp = func_800309B4(0, 0, 32.0f);
        result = ((0x1A < temp && temp < 0x1D) || temp == 0x1D);
    }

    if (mapIndex == GREENHOUSE) {
        if (func_800309B4(0, 0, 32.0f) == 0x12) { 
            result = TRUE;
        }
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "levelInteractions", checkWineBarrelInteraction);

bool checkWineBarrelInteraction(u16 mapIndex) {

    bool result = FALSE;
    
    if (mapIndex == VINEYARD_CELLAR_BASEMENT) {
        result = func_800309B4(0, 0, 32.0f) == 0x11;
    }

    return result;
}

//INCLUDE_ASM(const s32, "levelInteractions", func_800AD0C4);

// fishing
bool func_800AD0C4(u16 mapIndex) {

    bool result = FALSE;
    u8 object;
    
    if (mapIndex == MOUNTAIN_1) {

        object = func_800309B4(0, 0, 64.0f);

        if (object == 0x10 || object == 0x14) {
            gPlayer.fatigue.unk_3 = 0;
            result = TRUE;
        }
        
        if (object == 0x12) {
            gPlayer.fatigue.unk_3 = 1;
            result = TRUE;
        }
    }
    
    if (mapIndex == MOUNTAIN_2) {
        if (func_800309B4(0, 0, 64.0f) == 0x10) {
            gPlayer.fatigue.unk_3 = 2;
            result = TRUE;
        }
    }
    
    if (mapIndex == BEACH) {
        if (func_800309B4(0, 0, 64.0f) == 0x10) {
            gPlayer.fatigue.unk_3 = 3;
            result = TRUE;
        }
    }
    
    return result;
} 

// jtbl_80121408
//INCLUDE_ASM(const s32, "levelInteractions", func_800AD1D0);

bool func_800AD1D0(u16 mapIndex) {

    bool set;
    s32 tempExit;
    u8 temp;
    u16 temp2;

    tempExit = gEntranceIndex;
    
    levelInteractionsInfo.mapAdditionsIndex = 0xFF;
    levelInteractionsInfo.interactionSfxIndex = 0xFF;

    temp = func_800309B4(0, 0, 32.0f);

    D_80189826 = temp;
    
    if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
        D_801C3E2C = D_80189826;
    } else {
        D_801C3E2C = 0xFF;
    }

    set = FALSE;
    
    if (temp) {
        switch (mapIndex) {
            case FARM:
                set = func_800AD8D0(mapIndex, temp);
                break;
            case HOUSE:
                set = func_800ADCDC(mapIndex, temp);
                break;
            case BARN:
                set = func_800AE00C(mapIndex, temp);
                break;
            case COOP:
                set = func_800AEB54(mapIndex, temp);
                break;
            case KITCHEN:
                set = func_800AED60(mapIndex, temp);
                break;
            case BATHROOM:
                set = func_800AEE8C(mapIndex, temp);
                break;
            case GREENHOUSE:
                set = func_800AF060(mapIndex, temp);
                break;
            case VILLAGE_1:
                set = func_800AF0B0(mapIndex, temp);
                break;
            case FLOWER_SHOP:
                set = func_800AF494(mapIndex, temp);
                break;
            case POPURI_ROOM:
                set = func_800AFA2C(mapIndex, temp);
                break;
            case BAKERY:
                set = func_800AFA7C(mapIndex, temp);
                break;
            case ELLI_ROOM:
                set = func_800AFCD0(mapIndex, temp);
                break;
            case RICK_STORE:
                set = func_800AFD20(mapIndex, temp);
                break;
            case SOUVENIR_SHOP:
                set = func_800AFF9C(mapIndex, temp);
                break;
            case CHURCH:
                set = func_800B00E0(mapIndex, temp);
                break;
            case TAVERN:
                set = func_800B01EC(mapIndex, temp);
                break;
            case VILLAGE_2:
                set = func_800B0378(mapIndex, temp);
                break;
            case LIBRARY:
                set = func_800B0714(mapIndex, temp);
                break;
            case MIDWIFE_HOUSE:
                set = func_800B0A64(mapIndex, temp);
                break;
            case MAYOR_HOUSE:
                set = func_800B0AFC(mapIndex, temp);
                break;
            case MARIA_ROOM:
                set = func_800B0C48(mapIndex, temp);
                break;
            case POTION_SHOP:
                set = func_800B0C98(mapIndex, temp);
                break;
            case POTION_SHOP_BEDROOM:
                set = func_800B0FB8(mapIndex, temp);
                break;
            case SQUARE:
                set = func_800B106C(mapIndex, temp);
                break;
            case MOUNTAIN_1:
                set = func_800B1438(mapIndex, temp);
                break;
            case MOUNTAIN_2:
                set = func_800B1540(mapIndex, temp);
                break;
            case TOP_OF_MOUNTAIN_1:
                set = func_800B1808(mapIndex, temp);
                break;
            case MOON_MOUNTAIN:
                set = func_800B1994(mapIndex, temp);
                break;
            case ROAD:
                set = func_800B1AC4(mapIndex, temp);
                break;
            case CARPENTER_HUT:
                set = handleCarpenterHutInteractions(mapIndex, temp);
                break;
            case DUMPLING_HOUSE:
                set = handleDumplingHouseExit(mapIndex, temp);
                break;
            case CAVE:
                set = func_800B1DBC(mapIndex, temp);
                break;
            case POND:
                set = func_800B20C8(mapIndex, temp);
                break;
            case VINEYARD:
                set = func_800B2118(mapIndex, temp);
                break;
            case VINEYARD_HOUSE:
                set = func_800B2264(mapIndex, temp);
                break;
            case KAREN_ROOM:
                set = func_800B2340(mapIndex, temp);
                break;
            case VINEYARD_CELLAR:
                set = func_800B23A4(mapIndex, temp);
                break;
            case VINEYARD_CELLAR_BASEMENT:
                set = func_800B24D4(mapIndex, temp);
                break;
            case RACE_TRACK:
                set = func_800B256C(mapIndex, temp);
                break;
            case RANCH:
                set = func_800B2604(mapIndex, temp);
                break;
            case RANCH_HOUSE:
                set = func_800B27CC(mapIndex, temp);
                break;
            case RANCH_STORE:
                set = handleRanchStoreExits(mapIndex, temp);
                break;
            case ANN_ROOM:
                set = func_800B2B90(mapIndex, temp);
                break;
            case RANCH_BARN:
                set = func_800B2C28(mapIndex, temp);
                break;
            case BEACH:
                set = func_800B2C78(mapIndex, temp);
                break;
            case HARVEST_SPRITE_CAVE:
                set = func_800B1EE4(mapIndex, temp);
                break;
            case MINE:
                set = func_800B2078(mapIndex, temp);
                break;
            default:
                break;
        }
    }

    if (set == 1) {
        setPlayerAction(0, 0);
    }

    if (levelInteractionsInfo.interactionSfxIndex != 0xFF) {
        setAudio(levelInteractionsInfo.interactionSfxIndex);
    }

    if (tempExit != gEntranceIndex) {
        
        if (!(gPlayer.flags & 1)) {
            
            temp2 = func_80074C50(gEntranceIndex);
         
            if (gEntranceIndex == 0x5F) {
                if (!checkLifeEventBit(MARRIED)) {
                    if (gSeason == WINTER) {
                        if (gDayOfMonth == 24 && (18 < gHour && gHour < 21)) {
                            temp2 = 0xFFFF;
                        }
                        if (gSeason == WINTER && gDayOfMonth == 30 && (17 < gHour && gHour < 24)) {
                            temp2 = 0xFFFF;
                        }
                    }
                } else {
                    if (gSeason == WINTER && gDayOfMonth == 30 && (17 < gHour && gHour < 24)) {
                        temp2 = 0xFFFF;
                    }
                }
            }
            if ((gEntranceIndex == 0x59 || gEntranceIndex == 0x5B) && !checkLifeEventBit(MARRIED) && gSeason == SUMMER && gDayOfMonth == 1 && (18 < gHour && gHour < 21)) {
                temp2 = 0xFFFF;
            }
    
            if (temp2 == 0xFFFF) {

                if (levelInteractionsInfo.mapAdditionsIndex != 0xFF) {
                    func_80038990(0, levelInteractionsInfo.mapAdditionsIndex, 0);
                }
                
                func_8005C940(8, 2);
                return set;
                
            } else {
                switch (func_80074C38(gEntranceIndex)) {                           
                    case KAREN_ROOM:                                  
                        showTextBox(0, 6, temp2, 0, 0);
                        setPlayerAction(0xF, 0);
                        gPlayer.unk_6F = 0x20;
                        gPlayer.unk_6E = 6;
                        break;
                    case POPURI_ROOM:                               
                    case MARIA_ROOM:                                
                    case ANN_ROOM:                                  
                    case ELLI_ROOM:                    
                        showTextBox(0, 6, temp2, 0, 0);
                        break;
                    default:            
                        showTextBox(1, 6, temp2, 0, 2);
                        break;
                    }
                    
                gEntranceIndex = tempExit;
            }

        } else {
            set = FALSE;
        }
        
        gEntranceIndex = tempExit;
        
    } 
    
    return set;

}

// jtbl_80121578
//INCLUDE_ASM(const s32, "levelInteractions", func_800AD8D0);

// farm
static bool func_800AD8D0(u16 mapIndex, u8 arg1) {

    bool result = FALSE;
    u8 temp;
    
    switch (arg1) {

        case 1:
        case 2:
            break;
            break;
         
        case 3:        
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = TRUE;
                setEntrance(0x14);
            }
            
            return result;
        
        case 4:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                if (gPlayer.direction == 4) {
                    result = TRUE;
                    levelInteractionsInfo.mapAdditionsIndex = 9;
                    levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN;
                    setEntrance(0xA);
                }
            }
            break;
        
        case 5:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                if (gPlayer.direction == 4) {
                    result = TRUE;
                    levelInteractionsInfo.mapAdditionsIndex = 0xD;
                    levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN;
                    setEntrance(0xE);
                }
            }
            break;
        
        case 6:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                result = TRUE;
                levelInteractionsInfo.mapAdditionsIndex = 0xA;
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN;
                setEntrance(0x11);
            }
            break;
        
        case 7:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                result = TRUE;
                levelInteractionsInfo.mapAdditionsIndex = 0xB;
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN;
                setEntrance(0x12);
            }
            break;

        case 8:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                result = TRUE;
                levelInteractionsInfo.mapAdditionsIndex = 0x14;
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN;
                setEntrance(0x13);
            }
            break;

        case 9:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                if (gPlayer.direction == 6) {
                    result = TRUE;
                    levelInteractionsInfo.mapAdditionsIndex = 0xC;
                    levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN;
                    setEntrance(0xB);
                }
            }
            break;
        
        case 26:
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = TRUE;
                setEntrance(0x70);
            }
            break;
        
        case 17:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                if (gPlayer.heldItem == 0) {
                    if (!(gPlayer.flags & 1)) {
                        if (gLumber != 0) {
                            gPlayer.heldItem = 3;
                            setPlayerAction(4, 6);
                            gLumber += adjustValue(gLumber, -1, 0x3E7);
                            result = 2;
                        }
                    }
                }
            }
            break;

        case 18:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                
                temp = func_8006536C(); 
 
                if (temp != 0xFF) {
                    setAlbumPicture(temp);
                    showTextBox(1, 5, func_80063A2C(temp), 0, 2);
                    if (temp == 0x23) {
                        acquireKeyItem(GOLD_PENDANT);
                        setSpecialDialogueBit(HAVE_GOLD_PENDANT);
                    }
                } else {
                    showTextBox(1, 5, 0x3A, 0, 2);
                }

                result = TRUE;
            }
            break;
        
        case 19:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                
                if (gPlayer.direction == 2) {
                    func_8003FBD8(0x18, gLumber, 0);
                    showTextBox(1, 6, 0xA3, 0, 2);
                    result = TRUE;
                }
            }
            break;

        case 20:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                func_8003FBD8(0x17, fodderQuantity, 0);
                showTextBox(1, 6, 0xA2, 0, 2);
                result = TRUE;
            }
            break;
        
        case 21:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                showTextBox(1, 6, 0xA1, 0, 2);
                result = TRUE;
            }
            break;
        
        case 25:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                if (!checkHaveKeyItem(TREASURE_MAP)) {
                    showTextBox(1, 6, 0xA7, 0, 2);
                    acquireKeyItem(TREASURE_MAP);
                    result = TRUE;
                    toolUse.unk_F = 0;
                }
            }

        case 30:
            break;
    }

    return result;
   
}

// jtbl_801215F0
//INCLUDE_ASM(const s32, "levelInteractions", func_800ADCDC);

// house
static bool func_800ADCDC(u16 mapIndex, u8 arg1) {

    bool result = FALSE;
    
    switch (arg1) {
        case 1:
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                if (gWeather != TYPHOON) {
                    result = TRUE;
                    setEntrance(0);
                }
            }
            break;     
        case 2:
            if (func_8004D380(CONTROLLER_1, BUTTON_A) && gWeather != TYPHOON) {
                levelInteractionsInfo.mapAdditionsIndex = 2;
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN;
                result = TRUE;
                if (checkLifeEventBit(HAVE_KITCHEN)) {                   
                    setEntrance(0xF);
                } else {
                    setEntrance(8);
                }
            }            
            break;
        case 3:
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                if (gWeather != TYPHOON) {
                    result = TRUE;
                    setEntrance(0x6F);
                }
            }
            break;
        case 0x10:
            if (func_8004D380(CONTROLLER_1, BUTTON_A) && gPlayer.heldItem == 0 && checkLifeEventBit(MAYOR_TOUR) && !checkDailyEventBit(0x15)) {
                func_8005B09C(0);
                result = TRUE; 
            }
            break;
        case 0x12:
            if (func_8004D380(CONTROLLER_1, BUTTON_A) && gPlayer.direction == 4) {
                func_800B3694();
                func_800D8540();
                setPlayerAction(0xA, 0xC);
                levelInteractionsInfo.interactionSfxIndex = 0x2F;
                result = 2;
            }
            break;
        case 0x13:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                func_80038990(0, 3, 1);
                func_8005CA2C(0xA, 0x15);
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN;
                result = TRUE;
            }
            break;
        case 0x14:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                setMainLoopCallbackFunctionIndex(CALENDAR_LOAD);
                levelInteractionsInfo.interactionSfxIndex  = MENU_OPEN;
                result = TRUE;
            }
            break;
        case 0x15:
            if (func_8004D380(CONTROLLER_1, BUTTON_A) != 0) {
                func_80038990(0, 0xA, 1);
                func_8005CA2C(0xA, 0x17);
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN;
                result = TRUE;
            }
            break;
        case 0x16:
            if (func_8004D380(CONTROLLER_1, BUTTON_A) && gPlayer.heldItem == 0) {
                if (checkDailyEventBit(0x16)) {
                    if (gBabyAge < 30) {
                        gPlayer.heldItem = 0xBA;             
                        func_8002FA2C(npcInfoArray[BABY].spriteIndex);
                        npcInfoArray[BABY].flags &= ~4;
                        setPlayerAction(4, 6);
                        if (!checkDailyEventBit(0x55)) {
                            npcAffection[BABY] += adjustValue(npcAffection[BABY], 2, MAX_AFFECTION);
                            setDailyEventBit(0x55);
                        }
                        toggleDailyEventBit(0x16);
                        result = 2;
                    }
                }
            }    
            break;
        case 0x21:
        default:
            break;
    }

    return result;
    
}

// barn
// jtbl_80121678
INCLUDE_ASM(const s32, "levelInteractions", func_800AE00C);

// jtbl_801216E8
//INCLUDE_ASM(const s32, "levelInteractions", func_800AEB54);

// coop
static bool func_800AEB54(u16 mapIndex, u8 arg1) {

    bool result = FALSE;

    switch (arg1) {
        
        case 1:
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = TRUE;
                setEntrance(3);
            }
            break;
        
        case 18:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                showTextBox(1, 6, 0xAA, 0, 2);
                result = TRUE;
            }
            break;

        case 19:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                if (gChickens[0].flags & 1) {
                    showTextBox(1, 6, 0xF5, 0, 2);
                    result = TRUE;
                }
            }
            break;
        
        case 20:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                if (gChickens[1].flags & 1) {
                    showTextBox(1, 6, 0xF6, 0, 2);
                    result = TRUE;
                }
            }
            break;
        
        case 21:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                if (gChickens[2].flags & 1) {
                    showTextBox(1, 6, 0xF7, 0, 2);
                    result = TRUE;
                }   
            }           
            break;
        
        case 22:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                if (gChickens[3].flags & 1) {
                    showTextBox(1, 6, 0xF8, 0, 2);
                    result = TRUE;
                }         
            }       
            break;
        
        case 23:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                if (gChickens[4].flags & 1) {
                    showTextBox(1, 6, 0xF9, 0, 2);
                    result = TRUE;
                }
            }       
            break;
        
        case 24:     
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                if (gChickens[5].flags & 1) {
                    showTextBox(1, 6, 0xFA, 0, 2);
                    result = TRUE;
                }          
            }       
            break;        
    }

    return result;
    
}

// jtbl_80121748
//INCLUDE_ASM(const s32, "levelInteractions", func_800AED60);

// kitchen
static bool func_800AED60(u16 mapIndex, u8 arg1) {

    bool result = FALSE;

    switch (arg1) {
        case 1:
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = TRUE;
                setEntrance(0xB);
            }
            break;
        case 2:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                result = TRUE;
                levelInteractionsInfo.mapAdditionsIndex = 2;
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN;
                setEntrance(0xD);
            }        
            break;
        case 16:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                func_80038990(0, 0, 1);
                func_8005CA2C(0xA, 0x16);
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN;
                result = TRUE;
            }
            break;
        case 21:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                setMainLoopCallbackFunctionIndex(RECIPE_BOOK_LOAD);
                levelInteractionsInfo.interactionSfxIndex = MENU_OPEN;
                result = TRUE;
            }
            break;
        case 22:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                setMainLoopCallbackFunctionIndex(KITCHEN_PICTURE_LOAD);
                levelInteractionsInfo.interactionSfxIndex = MENU_OPEN;
                result = TRUE;
            }
            break;
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "levelInteractions", func_800AEE8C);

// bathroom
static bool func_800AEE8C(u16 mapIndex, u8 arg1) {

    bool result = FALSE;

    switch (arg1) {

        case 1:
            
            result = TRUE;
            
            if (checkLifeEventBit(0x10)) {
                setEntrance(0x10);
            } else {            
                setEntrance(6);
            }

            break;

        case 2:
            break;

        case 0x10:

            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                if (gPlayer.heldItem == 0 && animatedSprites[PLAYER].startingCoordinates.z >= -44.0f && animatedSprites[PLAYER].startingCoordinates.z < -36.0f) {
                    setDailyEventBit(6);
                    setPlayerAction(0x11, 0);
                    levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN;
                    result = 2;
                }
            }         

            break;

        case 0x11:

            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                if (gPlayer.heldItem == 0 && animatedSprites[PLAYER].startingCoordinates.x >= 42.0f && animatedSprites[PLAYER].startingCoordinates.x < 50.0f) {
                    setDailyEventBit(6);
                    setPlayerAction(0x10, 0);
                    gHappiness += adjustValue(gHappiness, 2, 0xFF);
                    result = 2;
                    levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN;
                }
            }            

            break;
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "levelInteractions", func_800AF060);

// greenhouse
static bool func_800AF060(u16 arg0, u8 arg1) {

    bool result = FALSE;

    if (arg1 == 1) {
        if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
            result = TRUE;
            setEntrance(7);
        }
    } 
    
    return result;
    
}

// jtbl_801217A0
INCLUDE_ASM(const s32, "levelInteractions", func_800AF0B0);

// jtbl_80121840
INCLUDE_ASM(const s32, "levelInteractions", func_800AF494);

//INCLUDE_ASM(const s32, "levelInteractions", func_800AFA2C);

static bool func_800AFA2C(u16 mapIndex, u8 arg1) {
    
    bool result = FALSE;
    
    if (arg1 == 1) {
        if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
            result = TRUE;
            setEntrance(FLOWER_SHOP_ENTRANCE);
        }
    } 
    
    return result;

}

// jtbl_801218B8
INCLUDE_ASM(const s32, "levelInteractions", func_800AFA7C);

//INCLUDE_ASM(const s32, "levelInteractions", func_800AFCD0);

static bool func_800AFCD0(u16 mapIndex, u8 arg1) {

    bool result = FALSE;

    if (arg1 == 1) {
        if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
            result = TRUE;
            setEntrance(0x5A);
        }
    }
    
    return result;
    
}

// jtbl_80121900
INCLUDE_ASM(const s32, "levelInteractions", func_800AFD20);

// jtbl_80121960
//INCLUDE_ASM(const s32, "levelInteractions", func_800AFF9C);

static u8 func_800AFF9C(u16 arg0, u8 arg1) {

    u8 result = 0;
    
    switch (arg1) {
        case 1:
            if (!checkDailyEventBit(0x2D)) {
                result = 1;
                setEntrance(0x37);
            }
            break;
        case 17:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(0x25)) {
                        func_800DC9FC(0x25);
                    }
                    result = 2;
                }
            }
            break;
        case 18:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(0x26)) {
                        func_800DC9FC(0x26);
                    }
                    if (checkShopItemShouldBeDisplayed(0x27)) {
                        func_800DC9FC(0x27);
                    }
                    result = 2;
                }
            }
            break;
        case 16:
        case 19:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                showTextBox(0, 6, 0xCA, 0, 0);
                result = 1;
            }
            break;
        default:
            break;
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "levelInteractions", func_800B00E0);

// church
static bool func_800B00E0(u16 mapIndex, u8 arg1) {

    bool result = FALSE;

    switch (arg1) {
        case 1:
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = TRUE;
                setEntrance(0x38);
            }
            break;
        case 0x10:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {            
                showTextBox(1, 6, 0xC0, 0, 2);
                result = TRUE;
            }            
            break;
        case 17:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                showTextBox(1, 6, 0xBF, 0, 2);
                result = TRUE;
            }
            break;
        case 19:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                showTextBox(1, 6, 0xD0, 0, 2);
                result = TRUE;
            }
            break;
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "levelInteractions", func_800B01EC);

// tavern
static bool func_800B01EC(u16 arg0, u8 arg1) {

    bool result = FALSE;

    switch (arg1) {
        case 1:
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = TRUE;
                setEntrance(0x39);
            }
            break;
        case 17:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                showTextBox(0, 6, 0x78, 0, 0);
                result = TRUE;
            }
            break;
        case 18:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {            
                if (gPlayer.heldItem == 0) {
                    func_8005B09C(4);
                    result = TRUE;
                }  
            }
            break;
        case 19:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {   
                switch (gSeason) {
                    case SPRING:
                        showTextBox(1, 6, 0xCB, 0, 2);
                        result = TRUE;
                        break;
                    case SUMMER:
                        showTextBox(1, 6, 0xCC, 0, 2);
                        result = TRUE;
                        break;
                    case AUTUMN:
                        showTextBox(1, 6, 0xCD, 0, 2);
                        result = TRUE;
                        break;
                    case WINTER:
                        showTextBox(1, 6, 0xCE, 0, 2);
                        result = TRUE;
                        break;      
                    default:
                        result = TRUE;
                        break;
                }
            }
            break;
    }
    
    return result;
    
}

// jtbl_801219B0
INCLUDE_ASM(const s32, "levelInteractions", func_800B0378);

// jtbl_80121A10
INCLUDE_ASM(const s32, "levelInteractions", func_800B0714);

//INCLUDE_ASM(const s32, "levelInteractions", func_800B0A64);

// midwife
static bool func_800B0A64(u16 mapIndex, u8 arg1) {

    bool result = FALSE;

    switch (arg1) {
        case 1:
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) { 
                result = TRUE;
                setEntrance(0x43);
            }
            break;
        case 16:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                showTextBox(1, 6, 0xD1, 0, 2);
                result = TRUE;
            }
        default:
            break;
    }
    
    return result;
    
}

INCLUDE_ASM(const s32, "levelInteractions", func_800B0AFC);

//INCLUDE_ASM(const s32, "levelInteractions", func_800B0C48);

static bool func_800B0C48(u16 mapIndex, u8 arg1) {

   bool result = FALSE;

    if (arg1 == 1) {
        if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
            result = TRUE;
            setEntrance(0x66);
        }
    } 

    return result;
    
}

// jtbl_80121A78
INCLUDE_ASM(const s32, "levelInteractions", func_800B0C98);

//INCLUDE_ASM(const s32, "levelInteractions", func_800B0FB8);

// potion shop bedroom
static bool func_800B0FB8(u16 mapIndex, u8 arg1) {

    bool result = FALSE;
    
    // FIXME: shouldn't be necessary
    int temp;

    switch (arg1) {

        case 1:
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = TRUE;
                setEntrance(0x69);
            }
            break;

        case 16:

            temp = 5;

            if (func_8004D380(CONTROLLER_1, BUTTON_A)) { 

                // FIXME: should be 0 < gSeason && gSeason < 5?
                // if (((s32) gSeason < 5) && (gSeason != 0)) 
                if (gSeason < temp && gSeason) {
                    showTextBox(1, 6, 0xD0, 0, 2);
                }

                result = TRUE;
            }
            break;
    }
    
    return result;
    
}

// jtbl_80121AC8
INCLUDE_ASM(const s32, "levelInteractions", func_800B106C);

//INCLUDE_ASM(const s32, "levelInteractions", func_800B1438);

// mountain 1
static bool func_800B1438(u16 mapIndex, u8 arg1) {

    bool result = FALSE;

    switch (arg1) {
        case 1:
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) { 
                result = TRUE;
                setEntrance(0x19);                
            }
            break;
        case 2:
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) { 
                result = TRUE;
                setEntrance(0x1D);                
            }
            break;
        case 3:
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) { 
                result = TRUE;
                setEntrance(0x23);                
            }
            break;
        case 19:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) { 
                showTextBox(1, 6, 0xAD, 0, 2);
                result = TRUE;
            }
            break;
    }
    
    return result;    
    
}

// jtbl_80121B30
INCLUDE_ASM(const s32, "levelInteractions", func_800B1540);

// jtbl_80121B80
INCLUDE_ASM(const s32, "levelInteractions", func_800B1808);

//INCLUDE_ASM(const s32, "levelInteractions", func_800B1994);

// moon mountain
static bool func_800B1994(u16 mapIndex, u8 arg1) {

    bool result = FALSE;

    switch (arg1) {

        case 1:
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) { 
                result = TRUE;
                setEntrance(0x21);                
            }
            break;

        case 2:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) { 
                if (checkDailyEventBit(0x28) || checkDailyEventBit(FESTIVAL)) {
                    showTextBox(1, 6, 0x168, 0, 2);
                    result = TRUE;
                } else {
                    result = TRUE;
                    levelInteractionsInfo.mapAdditionsIndex = 0;
                    setEntrance(0x4A);                
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN;
            }
            break;        

        case 16:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) { 
                showTextBox(1, 6, 0xB1, 0, 2);
                result = TRUE;            
            }
            break;
    }
    
    return result;
    
}

// jtbl_80121BD0
INCLUDE_ASM(const s32, "levelInteractions", func_800B1AC4);

//INCLUDE_ASM(const s32, "levelInteractions", handleCarpenterHutInteractions);

static bool handleCarpenterHutInteractions(u16 mapIndex, u8 arg1) {

    bool result = FALSE;
    
    switch (arg1) {
        case 1:
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = TRUE;
                setEntrance(0x20);
            } 
            break;
        case 0x10:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                if (!checkDailyEventBit(8)) {
                    func_80059334();
                    func_8005CA2C(1, 0x18);
                    result = TRUE;
                } else {
                    showTextBox(0, 6, 0x6E, 0, 0);
                    result = TRUE;
                }     
            }  
            break;
        case 0x11:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                showTextBox(0, 6, 0x91, 0, 0);
                result = TRUE;
            }
            break;
        default:
            break;
    }

    return result;
}

//INCLUDE_ASM(const s32, "levelInteractions", handleDumplingHouseExit);

static bool handleDumplingHouseExit(u16 mapIndex, u8 arg1) {

    bool result = FALSE;

    if (arg1 == 1) {
        if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
            result = TRUE;
            setEntrance(DUMPLING_HOUSE_EXIT);
        }
    } 

    return result;

}

//INCLUDE_ASM(const s32, "levelInteractions", func_800B1DBC);

static bool func_800B1DBC(u16 mapIndex, u8 arg1) {

    bool result = FALSE;

    switch (arg1) {

        case 1:
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) { 
                result = TRUE;
                setEntrance(0x1E);                
            }
            break;

        case 2:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) { 
                if (checkDailyEventBit(0x28) || checkDailyEventBit(FESTIVAL)) {
                    showTextBox(1, 6, 0x168, 0, 2);
                    result = TRUE;
                } else {
                    result = TRUE;
                    setEntrance(0x6D);
                }
            }
            break;

        case 3:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) { 
                showTextBox(1, 6, 0xB2, 0, 2);
                result = TRUE;
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN;        
            }
            break;
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "levelInteractions", func_800B1EE4);

// harvest sprite cave
static bool func_800B1EE4(u16 mapIndex, u8 arg1) {

    bool result = FALSE;

    switch (arg1) {

        case 1:
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) { 
                result = TRUE;
                setEntrance(0x46);                
            }
            break;

        case 16:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) { 
                if (getRandomNumberInRange(0, 1)) {
                    showTextBox(1, 6, 0x94, 0, 2);
                    result = TRUE;
                } else {
                    showTextBox(1, 6, 0x95, 0, 2);
                    result = TRUE;         
                }
            }
            break;

        case 17:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {  
                showTextBox(1, 6, 0x9D, 0, 2);
                result = TRUE;
            }
            break;

        case 18:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) { 
                if (checkDailyEventBit(0x28) || checkDailyEventBit(FESTIVAL)) {
                    showTextBox(1, 6, 0x168, 0, 2);
                    result = TRUE;
                } else if (getRandomNumberInRange(0, 1)) {
                    showTextBox(1, 6, 0x98, 0, 2);
                    result = TRUE;
                } else {
                    showTextBox(1, 6, 0x99, 0, 2);
                    result = TRUE;
                }
                
            }
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "levelInteractions", func_800B2078);

// mine
static bool func_800B2078(u16 mapIndex, u8 arg1) {

    bool result = FALSE;

    if (arg1 == 1) {
        if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
            result = TRUE;
            setEntrance(MINE_EXIT);
        }
    } 
    
    return result;

}

//INCLUDE_ASM(const s32, "levelInteractions", func_800B20C8);

// pond
static bool func_800B20C8(u16 mapIndex, u8 arg1) {
    
    bool result = FALSE;

    if (arg1 == 1) {
        if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) { 
            result = TRUE;
            setEntrance(0x1F);                
        }
    }
    
    return result;

}

// vineyard
INCLUDE_ASM(const s32, "levelInteractions", func_800B2118);

//INCLUDE_ASM(const s32, "levelInteractions", func_800B2264);

// vineyard house
static bool func_800B2264(u16 mapIndex, u8 arg1) {

    bool result = FALSE;

    switch (arg1) {
        case 1:
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) { 
                result = TRUE;
                setEntrance(0x52);                
            }
            break;
        case 2:
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) { 
                result = TRUE;
                setEntrance(0x2F);                
            }
            break;
        case 17:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) { 
                showTextBox(1, 6, 0xCF, 0, 2);
                result = TRUE;
            }
            break;
    }
    
    return result;

}

//INCLUDE_ASM(const s32, "levelInteractions", func_800B2340);

static bool func_800B2340(u16 mapIndex, u8 arg1) {

    bool result = FALSE;    
    
    if (arg1 == 1) {
        if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
            if (gPlayer.direction == 6) { 
                result = TRUE;
                setEntrance(0x50);
            }
        }
    }
    
    return result;

}

//INCLUDE_ASM(const s32, "levelInteractions", func_800B23A4);

// vineyard cellar
static bool func_800B23A4(u16 mapIndex, u8 arg1) {

    bool result = FALSE;
    
    switch (arg1) {
        case 1:
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = TRUE;
                setEntrance(0x30);
            }
            break;
         case 2:
             if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) { 
                 if (5 < gPlayer.direction && gPlayer.direction < 8 || gPlayer.direction == 5) {
                    result = TRUE;
                    setEntrance(0x55);
                 }
             }
            break;     
        case 16:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {            
                showTextBox(1, 6, 0xD2, 0, 2);
                result = TRUE;
            }            
            break;
        case 17:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                showTextBox(1, 6, 0xD3, 0, 2);
                result = TRUE;
            }
            break;

    }

    return result;
    
}

//INCLUDE_ASM(const s32, "levelInteractions", func_800B24D4);

// vineyard cellar basement
static bool func_800B24D4(u16 mapIndex, u8 arg1) {

    bool result = FALSE; 

    switch (arg1) {
        case 1:
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) { 
                result = TRUE;
                setEntrance(0x54);
            }
            break;
        case 16:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) { 
                showTextBox(1, 6, 0xD4, 0, 2);            
                result = TRUE;
            }
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "levelInteractions", func_800B256C);

// race track
static bool func_800B256C(u16 mapIndex, u8 arg1) {

    bool result = FALSE; 
    
    switch (arg1) {
        case 1:
            result = TRUE;
            setEntrance(0x62);
            break;
        case 16:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) { 
                if (gPlayer.heldItem == 0) {
                    if (checkDailyEventBit(0x41)) {
                        func_8005B09C(0xB);
                    } else {
                        func_8005B09C(0xC);
                    }
                    result = TRUE;
                }            
            }
            break;
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "levelInteractions", func_800B2604);

// FIXME: gotos
bool func_800B2604(u16 mapIndex, u8 arg1) {

    bool result = FALSE;
    
    switch (arg1) {

        case 1:

            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = TRUE;
                setEntrance(0x15);
            }
            break;
        
        case 2:

            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = TRUE;
                setEntrance(0x28);
            }
            break;
        
        case 3:
            
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                
                if (!checkDailyEventBit(0x28) && !checkDailyEventBit(FESTIVAL)) {

                     if (!(gSeason == AUTUMN && gDayOfMonth == 3)) { 
                        levelInteractionsInfo.mapAdditionsIndex = 2;
                        result = TRUE;
                        setEntrance(0x4B);
                     } else {
                        result = TRUE;
                        goto label;
                     }
                     
                } else {
                    goto label;
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN;
                
            }
            
            break;
        
        case 4:
            
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) { 
                
                if (checkDailyEventBit(0x28) || checkDailyEventBit(FESTIVAL)) { 
                    showTextBox(1, 6, 0x168, 0, 2);
                    result = TRUE;
                } else {
                    levelInteractionsInfo.mapAdditionsIndex = 1;
                    result = TRUE;
                    setEntrance(0x4E);
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN;
                
            }
            
            break;
        
        case 5:
            
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) { 

                if (checkDailyEventBit(0x28) || checkDailyEventBit(FESTIVAL)) {
label:
                    showTextBox(1, 6, 0x168, 0, 2);
                    result = TRUE;

                } else {
                    levelInteractionsInfo.mapAdditionsIndex = 0;
                    result = TRUE;
                    setEntrance(0x4F);
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN;
                
            }
            
            break;
        
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "levelInteractions", func_800B27CC);

// ranch house
static bool func_800B27CC(u16 mapIndex, u8 collisionIndex) {

    bool result = FALSE;

    switch (collisionIndex) {
        case 1: 
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = TRUE;
                setEntrance(RANCH_FROM_RANCH_HOUSE);
            }
            break;
        case 16:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) { 
                showTextBox(1, 6, 0x95, 0, 2);
                result = TRUE;
            }
            break;
        case 17:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) { 
                showTextBox(1, 6, 0x93, 0, 2);
                result = TRUE;
            }
            break;
        case 18:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) { 
                showTextBox(1, 6, 0xD0, 0, 2);
                result = TRUE;
            }
            break;
    }
    
    return result;
}

// jtbl_80121C30
//INCLUDE_ASM(const s32, "levelInteractions", handleRanchStoreExits);

static u8 handleRanchStoreExits(u16 mapIndex, u8 collisionIndex) {

    u8 result = 0;
    
    switch (collisionIndex) {

        // exit
        case 1:
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = 1;
                setEntrance(RANCH_FROM_RANCH_STORE);
            }
            break;

        // ann's room
        case 2:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                if (checkDailyEventBit(0x28) || checkDailyEventBit(FESTIVAL)) {
                    showTextBox(1, 6, 0x168, 0, 2);
                    result = 1;
                } else {
                    levelInteractionsInfo.mapAdditionsIndex = 0;
                    result = 1;
                    setEntrance(ANN_ROOM_ENTER);
                }
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN;
            } 
            break;

        // the following seem like unused items/interactions
        case 16:
            if (func_8004D380(CONTROLLER_1, BUTTON_A) && gPlayer.direction == 2) {
                    if (!checkDailyEventBit(1) && !checkDailyEventBit(2)) {
                         if (checkDailyEventBit(5)) {
                            showTextBox(0, 6, 0x165, 0, 0);
                        } else {
                            func_8005B09C(5);  
                        }
                    } else {
                        showTextBox(0, 6, 0x164, 0, 0);
                    }
                result = 1;   
            } 
            break;

        case 17:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) { 
                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(0x1A)) {
                        func_800DC9FC(0x1A);
                    }
                    result = 2;
                }
            }
            break;

        case 18:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) { 
                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(0x1D)) {
                        func_800DC9FC(0x1D);
                    }
                    result = 2;
                }
            }
            break;

        case 19:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) { 
                if (gPlayer.heldItem == 0) {
                     if (checkShopItemShouldBeDisplayed(0x1B)) {
                        func_800DC9FC(0x1B);
                    }
                    result = 2;
                }
            }
            break;

        case 20:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) { 
                if (gPlayer.heldItem == 0) {
                     if (checkShopItemShouldBeDisplayed(0x1C)) {
                        func_800DC9FC(0x1C);
                    }
                    result = 2;
                }
            }
            break;

        case 21:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) { 
                showDialogueBox(1, 0x3D, 0x29, 0x80, 2);
                result = 1;
            }
            break;
   }
    
    return result;

}

//INCLUDE_ASM(const s32, "levelInteractions", func_800B2B90);

// ann's room
static bool func_800B2B90(u16 mapIndex, u8 arg1) {

    bool result = FALSE; 

    switch (arg1) {
        case 1:
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = TRUE;
                setEntrance(0x4C);
            }
            break;
        case 19:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) { 
                showTextBox(1, 6, 0xCF, 0, 2);
                result = TRUE;
            }
            break;
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "levelInteractions", func_800B2C28);

// ranch barn
static bool func_800B2C28(u16 mapIndex, u8 arg1) {

    bool result = FALSE;    

    if (arg1 == 1) {
        if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
            result = TRUE;
            setEntrance(0x2D);
        }
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "levelInteractions", func_800B2C78);

// beach
static bool func_800B2C78(u16 mapIndex, u8 arg1) {

    bool result = FALSE;    
    
    switch (arg1) {
        case 1:
            break;
        case 2:
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = TRUE;
                setEntrance(0x2A);
            }
            break;
    }
    
    return result;
}

// FIXME:
// hack to get this rodata to align along 10 bytes
static const padding[2] = { 0, 0 };
