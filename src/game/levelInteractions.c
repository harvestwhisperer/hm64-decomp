#include "common.h"

#include "game/levelInteractions.h"

#include "system/controller.h"
#include "system/dialogue.h"
#include "system/entity.h"
#include "system/graphic.h"
#include "system/map.h"
#include "system/mathUtils.h"
#include "system/message.h"
#include "system/globalSprites.h"

#include "game/animals.h"
#include "game/game.h"
#include "game/gameAudio.h"
#include "game/gameStatus.h"
#include "game/transition.h"
#include "game/itemHandlers.h"
#include "game/level.h"
#include "mainLoop.h"
#include "game/overlayScreens.h"
#include "game/npc.h"
#include "game/player.h"
#include "game/shop.h"
#include "game/spriteInfo.h"
#include "game/updateGame.h"
#include "game/weather.h"

// bss
static LevelInteractionsInfo levelInteractionsInfo;

// non-contiguous bss
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

                                                                  
//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", func_800ACD70);

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

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", func_800ACDF4);

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

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", func_800ACE50);

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

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", func_800ACEAC);

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

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", func_800ACEF8);

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

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", func_800ACFE8);

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

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", checkWineBarrelInteraction);

bool checkWineBarrelInteraction(u16 mapIndex) {

    bool result = FALSE;
    
    if (mapIndex == VINEYARD_CELLAR_BASEMENT) {
        result = func_800309B4(0, 0, 32.0f) == 0x11;
    }

    return result;
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", func_800AD0C4);

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
//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", func_800AD1D0);

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
    
    if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
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
                
                func_8005C940(8, MAP_LOAD);
                return set;
                
            } else {
                switch (func_80074C38(gEntranceIndex)) {                           
                    case KAREN_ROOM:                                  
                        showTextBox(0, 6, temp2, 0, 0);
                        setPlayerAction(0xF, 0);
                        gPlayer.actionTimer = 0x20;
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
//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", func_800AD8D0);

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
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                if (gPlayer.direction == SOUTH) {
                    result = TRUE;
                    levelInteractionsInfo.mapAdditionsIndex = 9;
                    levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN;
                    setEntrance(0xA);
                }
            }
            break;
        
        case 5:
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                if (gPlayer.direction == SOUTH) {
                    result = TRUE;
                    levelInteractionsInfo.mapAdditionsIndex = 0xD;
                    levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN;
                    setEntrance(0xE);
                }
            }
            break;
        
        case 6:
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                result = TRUE;
                levelInteractionsInfo.mapAdditionsIndex = 0xA;
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN;
                setEntrance(0x11);
            }
            break;
        
        case 7:
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                result = TRUE;
                levelInteractionsInfo.mapAdditionsIndex = 0xB;
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN;
                setEntrance(0x12);
            }
            break;

        case 8:
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                result = TRUE;
                levelInteractionsInfo.mapAdditionsIndex = 0x14;
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN;
                setEntrance(0x13);
            }
            break;

        case 9:
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                if (gPlayer.direction == WEST) {
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
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
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
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                
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
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                
                if (gPlayer.direction == EAST) {
                    func_8003FBD8(0x18, gLumber, 0);
                    showTextBox(1, 6, 0xA3, 0, 2);
                    result = TRUE;
                }
            }
            break;

        case 20:
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                func_8003FBD8(0x17, fodderQuantity, 0);
                showTextBox(1, 6, 0xA2, 0, 2);
                result = TRUE;
            }
            break;
        
        case 21:
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                showTextBox(1, 6, 0xA1, 0, 2);
                result = TRUE;
            }
            break;
        
        case 25:
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
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
//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", func_800ADCDC);

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
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A) && gWeather != TYPHOON) {
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
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A) && gPlayer.heldItem == 0 && checkLifeEventBit(MAYOR_TOUR) && !checkDailyEventBit(0x15)) {
                func_8005B09C(0);
                result = TRUE; 
            }
            break;
        case 0x12:
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A) && gPlayer.direction == SOUTH) {
                func_800B3694();
                // turn on TV
                func_800D8540();
                setPlayerAction(0xA, 0xC);
                levelInteractionsInfo.interactionSfxIndex = 0x2F;
                result = 2;
            }
            break;
        case 0x13:
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                func_80038990(0, 3, 1);
                func_8005CA2C(0xA, 0x15);
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN;
                result = TRUE;
            }
            break;
        case 0x14:
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                setMainLoopCallbackFunctionIndex(CALENDAR_LOAD);
                levelInteractionsInfo.interactionSfxIndex  = MENU_OPEN;
                result = TRUE;
            }
            break;
        case 0x15:
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A) != 0) {
                func_80038990(0, 0xA, 1);
                func_8005CA2C(0xA, 0x17);
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN;
                result = TRUE;
            }
            break;
        case 0x16:
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A) && gPlayer.heldItem == 0) {
                if (checkDailyEventBit(0x16)) {
                    if (gBabyAge < 30) {
                        gPlayer.heldItem = 0xBA;             
                        deactivateEntity(npcInfoArray[BABY].entityIndex);
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
//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", func_800AE00C);

static u8 func_800AE00C(u16 mapIndex, u8 arg1) {

    u8 result = 0;
    s32 temp;
    u16 temp2;

    switch (arg1) {
        
        case 1:
            
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = 1;
                setEntrance(2);
            }
            
            break;

        case 17:
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                if (gPlayer.heldItem == 0 && fodderQuantity != 0) {
                        gPlayer.heldItem = FODDER;
                        setPlayerAction(4, 6);
                        fodderQuantity += adjustValue((s32) fodderQuantity, -1, 999);
                        result = 2;
                }
            }
            
            break;

        case 18:
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                // unused code    
            }
            break;

        case 19:
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                
                if (gPlayer.heldItem != FODDER && gFarmAnimals[0].flags & 1) {

                    temp = gFarmAnimals[0].type;
                    
                    if (temp >= 0) {

                        if (temp < 4) {
                            
                            setGlobalSeasonName(gFarmAnimals[0].birthdaySeason);
                        
                            gCurrentSeasonName[0] = gGlobalSeasonName[0];
                            gCurrentSeasonName[1] = gGlobalSeasonName[1];
                            gCurrentSeasonName[2] = gGlobalSeasonName[2];
                            gCurrentSeasonName[3] = gGlobalSeasonName[3];
                            gCurrentSeasonName[4] = gGlobalSeasonName[4];
                            gCurrentSeasonName[5] = gGlobalSeasonName[5];

                            func_8003FBD8(0x15, gFarmAnimals[0].birthdayDayOfMonth, 1);
                            func_8009BA74(0);
                            setGameVariableString(0x26, gFarmAnimals[0].unk_23, 6);
                            func_80061690(gFarmAnimals[0].goldenMilk);
                            showTextBox(1, 6, 0xE5, 0, 2);
                            
                        } else if (temp < 7) {
                            showTextBox(1, 6, 0xED, 0, 2);
                        }
                                    
                    } 
                    
                    result = 1;

                }
            }
            
            break;

        case 20:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                
                if (gPlayer.heldItem != FODDER && gFarmAnimals[1].flags & 1) {

                    temp = gFarmAnimals[1].type;
                
                    if (temp >= 0) {

                        if (temp < 4) {
                            
                            setGlobalSeasonName(gFarmAnimals[1].birthdaySeason);
                        
                            gCurrentSeasonName[0] = gGlobalSeasonName[0];
                            gCurrentSeasonName[1] = gGlobalSeasonName[1];
                            gCurrentSeasonName[2] = gGlobalSeasonName[2];
                            gCurrentSeasonName[3] = gGlobalSeasonName[3];
                            gCurrentSeasonName[4] = gGlobalSeasonName[4];
                            gCurrentSeasonName[5] = gGlobalSeasonName[5];
                        
                            func_8003FBD8(0x15, gFarmAnimals[1].birthdayDayOfMonth, 1);
                            func_8009BA74(1);
                            setGameVariableString(0x26, gFarmAnimals[1].unk_23, 6);
                            func_80061690(gFarmAnimals[1].goldenMilk);
                            showTextBox(1, 6, 0xE6, 0, 2);
                    
                        } else if (temp < 7) {
                            showTextBox(1, 6, 0xEE, 0, 2);
                        }
                        
                    } 

                    result = 1;
                
                }
            }
            
            break;

        case 21:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                
                if (gPlayer.heldItem != FODDER && gFarmAnimals[2].flags & 1) {

                    temp = gFarmAnimals[2].type;
                
                    if (temp >= 0) {

                        if (temp < 4) {
                            
                            setGlobalSeasonName(gFarmAnimals[2].birthdaySeason);
                        
                            gCurrentSeasonName[0] = gGlobalSeasonName[0];
                            gCurrentSeasonName[1] = gGlobalSeasonName[1];
                            gCurrentSeasonName[2] = gGlobalSeasonName[2];
                            gCurrentSeasonName[3] = gGlobalSeasonName[3];
                            gCurrentSeasonName[4] = gGlobalSeasonName[4];
                            gCurrentSeasonName[5] = gGlobalSeasonName[5];
                        
                            func_8003FBD8(0x15, gFarmAnimals[2].birthdayDayOfMonth, 1);
                            func_8009BA74(2);
                            setGameVariableString(0x26, gFarmAnimals[2].unk_23, 6);
                            func_80061690(gFarmAnimals[2].goldenMilk);
                            showTextBox(1, 6, 0xE7, 0, 2);
                            
                        } else if (temp < 7) {
                            showTextBox(1, 6, 0xEF, 0, 2);
                        }

                    } 

                    result = 1;
                    
                }
            }
            
            break;

        case 22:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                
                if (gPlayer.heldItem != FODDER && gFarmAnimals[3].flags & 1) {

                    temp = gFarmAnimals[3].type;
                    
                        if (temp >= 0) {

                            if (temp < 4) {
                                
                                setGlobalSeasonName(gFarmAnimals[3].birthdaySeason);
                            
                                gCurrentSeasonName[0] = gGlobalSeasonName[0];
                                gCurrentSeasonName[1] = gGlobalSeasonName[1];
                                gCurrentSeasonName[2] = gGlobalSeasonName[2];
                                gCurrentSeasonName[3] = gGlobalSeasonName[3];
                                gCurrentSeasonName[4] = gGlobalSeasonName[4];
                                gCurrentSeasonName[5] = gGlobalSeasonName[5];
                            
                                func_8003FBD8(0x15, gFarmAnimals[3].birthdayDayOfMonth, 1);
                                func_8009BA74(3);
                                setGameVariableString(0x26, gFarmAnimals[3].unk_23, 6);
                                func_80061690(gFarmAnimals[3].goldenMilk);
                                showTextBox(1, 6, 0xE8, 0, 2);
                                
                            } else if (temp < 7) {
                                showTextBox(1, 6, 0xF0, 0, 2);
                            }

                        } 

                        result = 1;
        
                }
            }
            
            break;

        case 23:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                
                if (gPlayer.heldItem != FODDER && gFarmAnimals[4].flags & 1) {

                    temp = gFarmAnimals[4].type;
                    
                    if (temp >= 0) {

                        if (temp < 4) {
                            
                            setGlobalSeasonName(gFarmAnimals[4].birthdaySeason);
                        
                            gCurrentSeasonName[0] = gGlobalSeasonName[0];
                            gCurrentSeasonName[1] = gGlobalSeasonName[1];
                            gCurrentSeasonName[2] = gGlobalSeasonName[2];
                            gCurrentSeasonName[3] = gGlobalSeasonName[3];
                            gCurrentSeasonName[4] = gGlobalSeasonName[4];
                            gCurrentSeasonName[5] = gGlobalSeasonName[5];
                        
                            func_8003FBD8(0x15, gFarmAnimals[4].birthdayDayOfMonth, 1);
                            func_8009BA74(4);
                            setGameVariableString(0x26, gFarmAnimals[4].unk_23, 6);
                            func_80061690(gFarmAnimals[4].goldenMilk);
                            showTextBox(1, 6, 0xE9, 0, 2);
                            
                        } else if (temp < 7) {
                            showTextBox(1, 6, 0xF1, 0, 2);
                        }
                        
                        
                    } 

                    result = 1;
                    
                }
            }
            
            break;

        case 24:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                
                if (gPlayer.heldItem != FODDER && gFarmAnimals[5].flags & 1) {

                    temp = gFarmAnimals[5].type;
                    
                    if (temp >= 0) {

                        if (temp < 4) {
                            
                            setGlobalSeasonName(gFarmAnimals[5].birthdaySeason);
                        
                            gCurrentSeasonName[0] = gGlobalSeasonName[0];
                            gCurrentSeasonName[1] = gGlobalSeasonName[1];
                            gCurrentSeasonName[2] = gGlobalSeasonName[2];
                            gCurrentSeasonName[3] = gGlobalSeasonName[3];
                            gCurrentSeasonName[4] = gGlobalSeasonName[4];
                            gCurrentSeasonName[5] = gGlobalSeasonName[5];
                        
                            func_8003FBD8(0x15, gFarmAnimals[5].birthdayDayOfMonth, 1);
                            func_8009BA74(5);
                            setGameVariableString(0x26, gFarmAnimals[5].unk_23, 6);
                            func_80061690(gFarmAnimals[5].goldenMilk);
                            showTextBox(1, 6, 0xEA, 0, 2);
                            
                        } else if (temp < 7) {
                            showTextBox(1, 6, 0xF2, 0, 2);
                        }
                        
                        
                    } 

                    result = 1;
                    
                }
            }
            
            break;

        case 25:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                
                if (gPlayer.heldItem != FODDER && gFarmAnimals[6].flags & 1) {

                    temp = gFarmAnimals[6].type;
                    
                    if (temp >= 0) {

                        if (temp < 4) {
                            
                            setGlobalSeasonName(gFarmAnimals[6].birthdaySeason);
                        
                            gCurrentSeasonName[0] = gGlobalSeasonName[0];
                            gCurrentSeasonName[1] = gGlobalSeasonName[1];
                            gCurrentSeasonName[2] = gGlobalSeasonName[2];
                            gCurrentSeasonName[3] = gGlobalSeasonName[3];
                            gCurrentSeasonName[4] = gGlobalSeasonName[4];
                            gCurrentSeasonName[5] = gGlobalSeasonName[5];
                        
                            func_8003FBD8(0x15, gFarmAnimals[6].birthdayDayOfMonth, 1);
                            func_8009BA74(6);
                            setGameVariableString(0x26, gFarmAnimals[6].unk_23, 6);
                            func_80061690(gFarmAnimals[6].goldenMilk);
                            showTextBox(1, 6, 0xEB, 0, 2);
                            
                        } else if (temp < 7) {
                            showTextBox(1, 6, 0xF3, 0, 2);
                        }

                    } 

                    result = 1; 

                }
            }
            
            break;

        case 26:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                
                if (gPlayer.heldItem != FODDER && gFarmAnimals[7].flags & 1) {

                    temp = gFarmAnimals[7].type;
                    
                    if (temp >= 0) {

                        if (temp < 4) {
                            
                            setGlobalSeasonName(gFarmAnimals[7].birthdaySeason);
                        
                            gCurrentSeasonName[0] = gGlobalSeasonName[0];
                            gCurrentSeasonName[1] = gGlobalSeasonName[1];
                            gCurrentSeasonName[2] = gGlobalSeasonName[2];
                            gCurrentSeasonName[3] = gGlobalSeasonName[3];
                            gCurrentSeasonName[4] = gGlobalSeasonName[4];
                            gCurrentSeasonName[5] = gGlobalSeasonName[5];
                        
                            func_8003FBD8(0x15, gFarmAnimals[7].birthdayDayOfMonth, 1);
                            func_8009BA74(7);
                            setGameVariableString(0x26, gFarmAnimals[7].unk_23, 6);
                            func_80061690(gFarmAnimals[7].goldenMilk);
                            showTextBox(1, 6, 0xEC, 0, 2);
                            
                        } else if (temp < 7) {
                            showTextBox(1, 6, 0xF4, 0, 2);
                        }
                            
                    } 

                    result = 1;
                    
                }
            }
            
            break;

        case 27:
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                func_8003FBD8(0x17, fodderQuantity, 0);
                showTextBox(1, 6, 0xA8, 0, 2);
                result = 1;
            }
            
            break;
        
        case 28:
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                showTextBox(1, 6, 0xA9, 0, 2);
                result = 1;
            }
            
            break;

        default:
            break;
        
    }

    return result;
    
}

// jtbl_801216E8
//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", func_800AEB54);

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
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                showTextBox(1, 6, 0xAA, 0, 2);
                result = TRUE;
            }
            break;

        case 19:
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                if (gChickens[0].flags & 1) {
                    showTextBox(1, 6, 0xF5, 0, 2);
                    result = TRUE;
                }
            }
            break;
        
        case 20:
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                if (gChickens[1].flags & 1) {
                    showTextBox(1, 6, 0xF6, 0, 2);
                    result = TRUE;
                }
            }
            break;
        
        case 21:
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                if (gChickens[2].flags & 1) {
                    showTextBox(1, 6, 0xF7, 0, 2);
                    result = TRUE;
                }   
            }           
            break;
        
        case 22:
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                if (gChickens[3].flags & 1) {
                    showTextBox(1, 6, 0xF8, 0, 2);
                    result = TRUE;
                }         
            }       
            break;
        
        case 23:
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                if (gChickens[4].flags & 1) {
                    showTextBox(1, 6, 0xF9, 0, 2);
                    result = TRUE;
                }
            }       
            break;
        
        case 24:     
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
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
//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", func_800AED60);

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
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                result = TRUE;
                levelInteractionsInfo.mapAdditionsIndex = 2;
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN;
                setEntrance(0xD);
            }        
            break;
        case 16:
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                func_80038990(0, 0, 1);
                func_8005CA2C(0xA, 0x16);
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN;
                result = TRUE;
            }
            break;
        case 21:
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                setMainLoopCallbackFunctionIndex(RECIPE_BOOK_LOAD);
                levelInteractionsInfo.interactionSfxIndex = MENU_OPEN;
                result = TRUE;
            }
            break;
        case 22:
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                setMainLoopCallbackFunctionIndex(KITCHEN_PICTURE_LOAD);
                levelInteractionsInfo.interactionSfxIndex = MENU_OPEN;
                result = TRUE;
            }
            break;
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", func_800AEE8C);

// bathroom
static bool func_800AEE8C(u16 mapIndex, u8 arg1) {

    bool result = FALSE;

    switch (arg1) {

        case 1:
            
            result = TRUE;
            
            if (checkLifeEventBit(HAVE_KITCHEN)) {
                setEntrance(0x10);
            } else {            
                setEntrance(6);
            }

            break;

        case 2:
            break;

        case 16:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                if (gPlayer.heldItem == 0 && entities[PLAYER].coordinates.z >= -44.0f && entities[PLAYER].coordinates.z < -36.0f) {
                    setDailyEventBit(6);
                    setPlayerAction(0x11, 0);
                    levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN;
                    result = 2;
                }
            }         

            break;

        case 17:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                if (gPlayer.heldItem == 0 && entities[PLAYER].coordinates.x >= 42.0f && entities[PLAYER].coordinates.x < 50.0f) {
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

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", func_800AF060);

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
//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", func_800AF0B0);

static bool func_800AF0B0(u16 mapIndex, u8 arg1) {

    bool result = FALSE;

    switch (arg1) {
        
        case 1:
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = TRUE;
                setEntrance(0x17);
            }
            break;
        
        case 2:
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = TRUE;
                setEntrance(0x3F);
            }
            break;

        case 3:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (checkDailyEventBit(0x28) || checkDailyEventBit(FESTIVAL)) { 
                    showTextBox(1, 6, 0x168, 0, 2);
                    result = TRUE;
                } else {
                    levelInteractionsInfo.mapAdditionsIndex = 0;
                    result = TRUE;
                    setEntrance(0x56);
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN;
                
            }
        
            break;

        case 4:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (gPlayer.direction == SOUTH) {
                    
                    if (checkDailyEventBit(0x28) || checkDailyEventBit(FESTIVAL)) { 
                        showTextBox(1, 6, 0x168, 0, 2);
                        result = TRUE;
                    } else {
                        levelInteractionsInfo.mapAdditionsIndex = 1;
                        result = TRUE;
                        setEntrance(0x59);
                    }
                    
                    levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN;
                        
                }
                
            }
        
            break;

        case 5:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (checkDailyEventBit(0x28) || checkDailyEventBit(FESTIVAL)) { 
                    showTextBox(1, 6, 0x168, 0, 2);
                    result = TRUE;
                } else {
                    levelInteractionsInfo.mapAdditionsIndex = 4;
                    result = TRUE;
                    setEntrance(0x5D);
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN;
                
            }
        
            break;

        case 6:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (checkDailyEventBit(0x28) || checkDailyEventBit(FESTIVAL)) { 
                    showTextBox(1, 6, 0x168, 0, 2);
                    result = TRUE;
                } else {
                    levelInteractionsInfo.mapAdditionsIndex = 3;
                    result = TRUE;
                    setEntrance(0x5E);
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN;
                
            }
        
            break;

        case 7:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (checkDailyEventBit(0x28) || checkDailyEventBit(FESTIVAL)) { 
                    showTextBox(1, 6, 0x168, 0, 2);
                    result = TRUE;
                } else {
                    result = TRUE;
                    setEntrance(0x5F);
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN;
                
            }
        
            break;

        case 8:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (checkDailyEventBit(0x28) || checkDailyEventBit(FESTIVAL)) { 
                    showTextBox(1, 6, 0x168, 0, 2);
                    result = TRUE;
                } else {
                    result = TRUE;
                    setEntrance(0x60);
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN;
                
            }
        
            break;

        case 9:

            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                if (gPlayer.direction == EAST) {
                    result = TRUE;
                    setEntrance(0x61);
                }
            }
            
            break;

        case 10:
            
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                if (gPlayer.direction == EAST) {
                    result = TRUE;
                    setEntrance(0x63);
                }
            }
            
            break;

        case 37:

            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                if (gPlayer.direction == NORTHEAST) {
                    result = TRUE;
                    setEntrance(0x63);
                }
            }
            
            break;

        case 38:

            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                if (gPlayer.direction == SOUTHEAST) {
                    result = TRUE;
                    setEntrance(0x63);
                }
            }
            
            break;

        case 39:

            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                if (gPlayer.direction == NORTHEAST) {
                    result = TRUE;
                    setEntrance(0x61);
                }
            }
            
            break;

        case 40:

            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                if (gPlayer.direction == SOUTHEAST) {
                    result = TRUE;
                    setEntrance(0x61);
                }
            }
            
            break;

        case 11:
            
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = TRUE;
                setEntrance(0x44);
            }
            
            break;

        case 12:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (checkDailyEventBit(0x28) || checkDailyEventBit(FESTIVAL)) {
                    showTextBox(1, 6, 0x168, 0, 2);
                    result = TRUE;

                } else {
                    levelInteractionsInfo.mapAdditionsIndex = 2;
                    result = TRUE;
                    setEntrance(0x5B);
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN;
                
            }
            
            break;
        
    }
    
    return result;
   
}

// jtbl_80121840
//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", func_800AF494);

static u8 func_800AF494(u16 mapIndex, u8 arg1) {

    u8 result = 0;
    
    switch (arg1) {

        case 1:

            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = 1;
                setEntrance(0x34);
            }
            break;
        
        case 2:
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (checkDailyEventBit(0x28) || checkDailyEventBit(FESTIVAL)) { 
                    showTextBox(1, 6, 0x168, 0, 2);
                    result = 1;
                } else {
                    levelInteractionsInfo.mapAdditionsIndex = 0;
                    result = 1;
                    setEntrance(0x58);
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN;
                
            }
        
            break;

        case 16:

            if (gPlayer.direction == SOUTH) {
                
                if (gSeason == WINTER && (24 < gDayOfMonth && gDayOfMonth < 30)) {

                    if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                        if (gPlayer.heldItem == 0) {
                            if (adjustValue(flowerShopPoints, -10, 100) == -10) {
                                setMainLoopCallbackFunctionIndex(0x22);
                                result = 1; 
                            } else {
                                showTextBox(0, 4, 0x3E, 0, 0);
                                result = 1;
                            }
                        }
                        result = 1;
                    }
                    
                } else {
                    if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                        showDialogueBox(0, 0x3D, 0x27, 0x80, 0);
                        result = 1;
                    }
                }
                
            }

            break;

        case 17:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {

                switch (gSeason) {
                    case SPRING:
                        showTextBox(1, 6, 0xE, 0, 2);
                        break;
                    case SUMMER:
                        showTextBox(1, 6, 0xF, 0, 2);
                        break;
                    case AUTUMN:
                        showTextBox(1, 6, 0x10, 0, 2);
                        break;
                    case WINTER:
                        showTextBox(1, 6, 0xC9, 0, 2);
                        break;
                    
                }

                result = 1;
        
            }

            break;

        case 18:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {

                switch (gSeason) {

                    case SPRING:
                        showTextBox(1, 6, 0x11, 0, 2);
                        break;
                    case SUMMER:
                        showTextBox(1, 6, 0x12, 0, 2);
                        break;
                    case AUTUMN:
                        showTextBox(1, 6, 0x13, 0, 2);
                        break;
                    case WINTER:
                        showTextBox(1, 6, 0x14, 0, 2);
                        break;
                    
                }

                result = 1;
                
            }

            break;

        case 19:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(0)) {
                        func_800DC9FC(0);
                    }
                    result = 2;
                }
            
            }

            break;

        case 20:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(1)) {
                        func_800DC9FC(1);
                    }
                    result = 2;
                }
            
            }

            break;

        case 21:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(2)) {
                        func_800DC9FC(2);
                    }
                    result = 2;
                }
            
            }

            break;

        case 22:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(4)) {
                        func_800DC9FC(4);
                    }
                    result = 2;
                }
            
            }

            break;

        case 23:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(5)) {
                        func_800DC9FC(5);
                    }
                    result = 2;
                }
            
            }

            break;

        case 24:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(6)) {
                        func_800DC9FC(6);
                    }
                    result = 2;
                }
            
            }

            break;

        case 25:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(3)) {
                        func_800DC9FC(3);
                    }
                    result = 2;
                }
            
            }

            break;

        case 26:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (gPlayer.heldItem == 0) {
                    
                    if (!checkShopItemShouldBeDisplayed(7)) {
                        
                        if (checkShopItemShouldBeDisplayed(8)) {
                            func_800DC9FC(8);
                        }

                        result = 2;
                        
                    } else {
                        func_800DC9FC(7);
                        result = 2;
                    }
                    
                }
            
            }

            break;

        case 27:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(10)) {
                        func_800DC9FC(10);
                    }
                    result = 2;
                }
            
            }

            break;

        case 30:
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                showDialogueBox(0, 0x3D, 0x28, 0x80, 0);
                result = 1;
            }
        
        
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", func_800AFA2C);

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
//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", func_800AFA7C);

static bool func_800AFA7C(u16 mapIndex, u8 arg1) {

    bool result = FALSE;

    switch (arg1) {

        case 1:
            
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = TRUE;
                setEntrance(0x35);
            }
            
            break;

        case 2:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (checkDailyEventBit(0x28) || checkDailyEventBit(FESTIVAL)) { 
                    showTextBox(1, 6, 0x168, 0, 2);
                    result = TRUE;
                } else {
                    levelInteractionsInfo.mapAdditionsIndex = 0;
                    result = TRUE;
                    setEntrance(0x5C);
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN;
                
            }
        
            break;

        case 3:

            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = TRUE;
                setEntrance(0x3D);
            }
            
            break;

        case 16:

            if (gPlayer.direction == SOUTH) {
                
                if (gSeason == WINTER && (24 < gDayOfMonth && gDayOfMonth < 30)) {

                    if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                        if (gPlayer.heldItem == 0) {
                            if (adjustValue(bakeryCardPoints, -10, 100) == -10) {
                                setMainLoopCallbackFunctionIndex(0x22);
                                result = TRUE; 
                            } else {
                                showTextBox(0, 4, 0x46, 0, 0);
                                result = TRUE;
                            }
                        }
                        result = TRUE;
                    }
                    
                } else {
                    if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                        showTextBox(0, 6, 0x43, 0, 0);
                        result = TRUE;
                    }
                }
                
            }

            break;

        case 17:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (gPlayer.direction == SOUTH && gPlayer.heldItem == 0) {
                    func_8005B09C(2);
                    result = TRUE;
                }
            
            }

            break;

        case 18:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (gPlayer.heldItem == 0) {
                    func_8005B09C(3);
                    result = TRUE;
                }
            
            }

            break;

        
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", func_800AFCD0);

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
//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", func_800AFD20);

static bool func_800AFD20(u16 mapIndex, u8 arg1) {

    bool result = FALSE;

    switch (arg1) {

        case 1:
        
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = 1;
                setEntrance(0x36);
            }
            
            break;

        case 16:
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (gPlayer.direction == SOUTH) {
                        showTextBox(0, 6, 0x44, 0, 0);
                        result = 1;
                }
            
            }

            break;

        case 17:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(0x15)) {
                        func_800DC9FC(0x15);
                    }
                    result = 2;
                }
            
            }

            break;

        case 18:
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(0x14)) {
                        func_800DC9FC(0x14);
                    }
                    result = 2;
                }
            
            }

            break;

        case 19:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(0x13)) {
                        func_800DC9FC(0x13);
                    }
                    result = 2;
                }
            
            }

            break;

        case 20:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(0x16)) {
                        func_800DC9FC(0x16);
                    }
                    result = 2;
                }
            
            }

            break;

        case 21:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(0x18)) {
                        func_800DC9FC(0x18);
                    }
                    if (checkShopItemShouldBeDisplayed(0x19)) {
                        func_800DC9FC(0x19);
                    }
                    result = 2;
                }
            
            }

            break;

        case 22:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(0x17)) {
                        func_800DC9FC(0x17);
                    }
                    result = 2;
                }
            
            }

            break;

        case 24:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                showTextBox(0, 6, 0x36, 0, 0);
                result = 1;
            
            }

            break;
        
    }
    
    return result;
    
}

// jtbl_80121960
//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", func_800AFF9C);

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

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(0x25)) {
                        func_800DC9FC(0x25);
                    }
                    result = 2;
                }
            }
            break;

        case 18:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
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

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                showTextBox(0, 6, 0xCA, 0, 0);
                result = 1;
            }
            break;

    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", func_800B00E0);

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

        case 16:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {            
                showTextBox(1, 6, 0xC0, 0, 2);
                result = TRUE;
            }           

            break;

        case 17:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                showTextBox(1, 6, 0xBF, 0, 2);
                result = TRUE;
            }

            break;

        case 19:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                showTextBox(1, 6, 0xD0, 0, 2);
                result = TRUE;
            }

            break;

    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", func_800B01EC);

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

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                showTextBox(0, 6, 0x78, 0, 0);
                result = TRUE;
            }

            break;

        case 18:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {            
                if (gPlayer.heldItem == 0) {
                    func_8005B09C(4);
                    result = TRUE;
                }  
            }

            break;

        case 19:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {   

                switch (gSeason) {

                    case SPRING:
                        showTextBox(1, 6, 0xCB, 0, 2);
                        break;
                    case SUMMER:
                        showTextBox(1, 6, 0xCC, 0, 2);
                        break;
                    case AUTUMN:
                        showTextBox(1, 6, 0xCD, 0, 2);
                        break;
                    case WINTER:
                        showTextBox(1, 6, 0xCE, 0, 2);
                        break;      

                }
                
                result = TRUE;

            }

            break;

    }
    
    return result;
    
}

// jtbl_801219B0
//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", func_800B0378);

static bool func_800B0378(u16 mapIndex, u8 arg1) {

    bool result = FALSE;

    switch (arg1) {

        case 1:
        
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = TRUE;
                setEntrance(0x18);
            }
            
            break;

        case 2:
            
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = TRUE;
                setEntrance(0x33);
            }
            
            break;

        case 3:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (checkDailyEventBit(0x28) || checkDailyEventBit(FESTIVAL) || checkDailyEventBit(0x56)) { 
                    showTextBox(1, 6, 0x168, 0, 2);
                    result = TRUE;
                } else {
                    levelInteractionsInfo.mapAdditionsIndex = 1;
                    result = TRUE;
                    setEntrance(0x64);
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN;
                
            }
        
            break;

        case 4:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (checkDailyEventBit(0x28) || checkDailyEventBit(FESTIVAL)) { 
                    showTextBox(1, 6, 0x168, 0, 2);
                    result = TRUE;
                } else {
                    levelInteractionsInfo.mapAdditionsIndex = 0;
                    result = TRUE;
                    setEntrance(0x65);
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN;
                
            }
        
            break;

        case 5:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (checkDailyEventBit(0x28) || checkDailyEventBit(FESTIVAL)) { 
                    showTextBox(1, 6, 0x168, 0, 2);
                    result = TRUE;
                } else {
                    levelInteractionsInfo.mapAdditionsIndex = 3;
                    result = TRUE;
                    setEntrance(0x68);
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN;
                
            }
        
            break;

        case 6:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (checkDailyEventBit(0x28) || checkDailyEventBit(FESTIVAL)) { 
                    showTextBox(1, 6, 0x168, 0, 2);
                    result = TRUE;
                } else {
                    levelInteractionsInfo.mapAdditionsIndex = 2;
                    result = TRUE;
                    setEntrance(0x6B);
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN;
                
            }
        
            break;
        
        case 7:
            
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = TRUE;
                setEntrance(0x3C);
            }
            
            break;

        case 16:
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                showTextBox(1, 6, 0xB8, 0, 2);
                result = TRUE;
            
            }

            break;

        case 17:
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                showTextBox(1, 6, 0xB5, 0, 2);
                result = TRUE;
            
            }

            break;
        
        case 18:
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                showTextBox(1, 6, 0x160, 0, 2);
                result = TRUE;
            
            }

            break;

        case 19:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (checkLifeEventBit(0x50)) {
                    showTextBox(1, 6, 0x15F, 0, 2);
                    result = TRUE;
                } else {
                    showTextBox(1, 6, 0x167, 0, 2);
                    result = TRUE;
                }
            
            }

            break;

        case 20:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                showTextBox(1, 6, 0x161, 0, 2);
                result = TRUE;
            
            }

            break;

        case 21:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                showTextBox(1, 6, 0x161, 0, 2);
                result = TRUE;
            
            }

            break;

        case 22:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                showTextBox(1, 6, 0x162, 0, 2);
                result = TRUE;
            
            }

            break;

        case 23:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                showTextBox(1, 6, 0x163, 0, 2);
                result = TRUE;
            
            }

            break;
        
        
    }
    
    return result;
    
}

// jtbl_80121A10
//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", func_800B0714);

static bool func_800B0714(u16 mapIndex, u8 arg1) {

    bool result = FALSE;

    switch (arg1) {

        case 1:

            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = TRUE;
                setEntrance(0x40);
            }
            
            break;

        case 16:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {  

                if (gPlayer.direction == SOUTH) {

                    if (checkLifeEventBit(MARRIED) && gWife == MARIA) {
                        showTextBox(0, 6, 0xD7, 0, 0);
                        result = TRUE;
                    } else {
                        showTextBox(0, 6, 0xD6, 0, 0);
                        result = TRUE;
                    }
                    
                    
                }
                
            }

            break;

        case 17:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {  

                if (gPlayer.heldItem == 0) {

                    showTextBox(1, 1, 0, 0, 2);
                    result = TRUE;
                    
                } 
                        
            }

            break;    
        
        case 18:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {  

                if (gPlayer.heldItem == 0) {

                    showTextBox(1, 1, 1, 0, 2);
                    result = TRUE;
                    
                } 
                        
            }

            break;          

        case 19:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {  

                if (gPlayer.heldItem == 0) {

                    showTextBox(1, 1, 2, 0, 2);
                    result = TRUE;
                    
                } 
                        
            }

            break;          
        
        case 20:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {  

                if (gPlayer.heldItem == 0) {

                    showTextBox(1, 1, 3, 0, 2);
                    result = TRUE;
                    
                } 
                        
            }

            break;          
        
        case 21:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {  

                if (gPlayer.heldItem == 0) {

                    showTextBox(1, 1, 4, 0, 2);
                    result = TRUE;
                    
                } 
                        
            }

            break;          
         
        case 22:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {  

                if (gPlayer.heldItem == 0) {

                    showTextBox(1, 1, 5, 0, 2);
                    result = TRUE;
                    
                } 
                        
            }

            break;
        
        case 23:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {  

                if (gPlayer.heldItem == 0) {

                    showTextBox(1, 1, 6, 0, 2);
                    result = TRUE;
                    
                } 
                        
            }

            break;          
        
        case 24:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {  

                if (gPlayer.heldItem == 0) {

                    showTextBox(1, 1, 7, 0, 2);
                    result = TRUE;
                    
                } 
                        
            }

            break;          
        
        case 25:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {

                if (gPlayer.direction == EAST && gPlayer.heldItem == 0) {

                    showTextBox(1, 1, 8, 0, 2);
                    result = TRUE;
                    
                } 
                        
            }

            break;   
        
        case 26:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {  

                if (gPlayer.direction == EAST && gPlayer.heldItem == 0) {

                    showTextBox(1, 1, 9, 0, 2);
                    result = TRUE;
                    
                } 
                        
            }

            break;          
        
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", func_800B0A64);

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

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                showTextBox(1, 6, 0xD1, 0, 2);
                result = TRUE;
            }

            break;

    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", func_800B0AFC);

static bool func_800B0AFC(u16 mapIndex, u8 arg1) {

    bool result = FALSE;

    switch (arg1) {

        case 1:

            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = TRUE;
                setEntrance(0x41);
            }
            
            break;

        case 2:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (checkDailyEventBit(0x28) || checkDailyEventBit(FESTIVAL)) { 
                    showTextBox(1, 6, 0x168, 0, 2);
                    result = TRUE;
                } else {
                    levelInteractionsInfo.mapAdditionsIndex = 0;
                    result = TRUE;
                    setEntrance(0x67);
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN;
                
            }
        
            break;

        case 16:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                switch (gSeason) {

                    case SPRING:
                    case SUMMER:
                    case AUTUMN:
                    case WINTER:
                        showTextBox(1, 6, 0xCF, 0, 2);
                        break;

                }
                
                result = TRUE; 
            
            }

            break;

    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", func_800B0C48);

static bool func_800B0C48(u16 mapIndex, u8 arg1) {

   bool result = FALSE;

    switch (arg1) {
        case 1:

            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = TRUE;
                setEntrance(0x66);
            }
            
            break;

    } 

    return result;
    
}

// jtbl_80121A78
//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", func_800B0C98);

static u8 func_800B0C98(u16 mapIndex, u8 arg1) {

    bool result = 0;

    switch (arg1) {

        case 1:

            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = 1;
                setEntrance(0x42);
            }
            
            break;
        
        case 2:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (checkDailyEventBit(0x28) || checkDailyEventBit(FESTIVAL)) { 
                    showTextBox(1, 6, 0x168, 0, 2);
                    result = 1;
                } else {
                    levelInteractionsInfo.mapAdditionsIndex = 0;
                    result = 1;
                    setEntrance(0x6A);
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN;
                
            }
        
            break;
        
        case 16:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {  

                if (gPlayer.heldItem == 0) {

                    switch (checkShopItemShouldBeDisplayed(0x22)) {

                        case 0:
                            showTextBox(0, 6, 0x6C, 0, 0);
                            result = 1;
                            break;
                        case 1:
                            func_800DC9FC(0x22);
                            result = 2;
                            break;
                        case 2:
                            showTextBox(0, 6, 0x6F, 0, 0);
                            result = 1;
                            break;
                        case 3:
                            showTextBox(0, 6, 0x6B, 0, 0);
                            result = 1;
                            break;
                        
                    }
                    
                } 
                        
            }

            break;    
        
        
        case 17:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {  

                if (gPlayer.heldItem == 0) {

                    switch (checkShopItemShouldBeDisplayed(0x23)) {

                        case 0:
                            showTextBox(0, 6, 0x6C, 0, 0);
                            result = 1;
                            break;
                        case 1:
                            func_800DC9FC(0x23);
                            result = 2;
                            break;
                        case 2:
                            showTextBox(0, 6, 0x6F, 0, 0);
                            result = 1;
                            break;
                        case 3:
                            showTextBox(0, 6, 0x6B, 0, 0);
                            result = 1;
                            break;
                        
                    }
                    
                } 
                        
            }

            break;    
        
        case 18:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {  

                if (gPlayer.heldItem == 0) {

                    switch (checkShopItemShouldBeDisplayed(0x24)) {

                        case 0:
                            showTextBox(0, 6, 0x6C, 0, 0);
                            result = 1;
                            break;
                        case 1:
                            func_800DC9FC(0x24);
                            result = 2;
                            break;
                        case 2:
                            showTextBox(0, 6, 0x6F, 0, 0);
                            result = 1;
                            break;
                        case 3:
                            showTextBox(0, 6, 0x6B, 0, 0);
                            result = 1;
                            break;
                        
                    }
                    
                } 
                        
            }

            break;   
        
        case 19:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                showTextBox(0, 6, 0x70, 0, 0);
                result = 1;
            
            }

            break;
        
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", func_800B0FB8);

// potion shop bedroom
static bool func_800B0FB8(u16 mapIndex, u8 arg1) {

    bool result = FALSE;

    switch (arg1) {

        case 1:
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = TRUE;
                setEntrance(0x69);
            }
            break;

        case 16:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                switch (gSeason) {

                    case SPRING:
                    case SUMMER:
                    case AUTUMN:
                    case WINTER:
                        showTextBox(1, 6, 0xD0, 0, 2);
                        break;

                }

                result = TRUE;
            }

            break;

    }
    
    return result;
    
}

// square
// jtbl_80121AC8
//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", func_800B106C);

// FIXME: lots of unnecessary gotos
u8 func_800B106C(u16 mapIndex, u8 arg1) {

    u8 result = 0;
    u8 temp;

    switch (arg1) {

        case 1:

            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = 1;
                setEntrance(0x3A);
            }
            
            break;


        case 2:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {  

                levelInteractionsInfo.mapAdditionsIndex = 0;
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN;
                result = 1;
                setEntrance(0x6C);
                
            }
            
            break;

        case 3:

            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = 1;
                setEntrance(0x3B);
            }
            
            break;

        case 16:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {

                if (gPlayer.heldItem == 0) {
                    temp = 10;
label3:
                    func_8005B09C(temp);
                    result = 1;
label4:
                }
                
            }

            break;
        
        case 21:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (checkDailyEventBit(0x41)) {

                    if (func_800CE714(0) && func_800CE714(1)) {
                        
                        if (func_800CE714(2)) {
                            goto label2;
                        } else {
                            goto label;
                        }
                        
                    } else {
                        func_80046C98();
                        setMainLoopCallbackFunctionIndex(0x21);
                    }
                    
                } else if (func_800CE828(0) && func_800CE828(1) && func_800CE828(2)) {
label2:
                    showTextBox(0, 4, 0x47, 0, 0);
                    
                } else {
label:
                    func_80046C98();
                    setMainLoopCallbackFunctionIndex(0x21);
                    
                }
                
                result = 1;
                
            }
            
            break;

        case 22:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {  

                if (overlayScreenStrings.unk_6B == overlayScreenStrings.unk_6A) {
                    showTextBox(0, 4, 10, 0, 0);
                } else if (overlayScreenStrings.unk_6A == 3) {
                    showTextBox(0, 4, 8, 0, 0);
                } else if (!overlayScreenStrings.unk_6D[overlayScreenStrings.unk_6A]) {
                    func_80046C98();
                    setMainLoopCallbackFunctionIndex(0x1F);
                } else {
                    showTextBox(0, 4, 7, 0, 0);
                }
                
                result = 1;
                
            }
            
            break;

        case 24:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (gPlayer.heldItem == 0) {
                
                    if (checkShopItemShouldBeDisplayed(0x35)) {
                        func_80046C98();
                        func_800DC9FC(0x35);
                    }
                    
                    result = 2;
                
                }
                
            }
            
            break;

        case 26:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {

                if (gSeason == SPRING && (18 < gDayOfMonth && gDayOfMonth < 23)) {

                    if (gVoteForFlowerFestivalGoddess == 0xFF) {
                        temp = 9;
                        goto label3;
                    } else {
                        showTextBox(0, 4, 0x23, 0, 2);
                        result = 1;
                        goto label4;
                    }
                    
                }

                if (!checkHaveTool(EMPTY_BOTTLE)) {

                    if (gPlayer.heldItem == 0) {
                    
                        if (checkShopItemShouldBeDisplayed(0x36)) {
                            func_80046C98();
                            func_800DC9FC(0x36);
                            goto label5;
                        }
                        
                        if (checkShopItemShouldBeDisplayed(0x37)) {
                            func_80046C98();   
                            func_800DC9FC(0x37);
label5:
                        }
                        
                        result = 2;
                        
                    }
                    
                } else { 
                    showTextBox(0, 4, 0x26, 0, 0);
                }
            }
            
            break;
        
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", func_800B1438);

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

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                showTextBox(1, 6, 0xAD, 0, 2);
                result = TRUE;
            }

            break;

    }
    
    return result;    
    
}

// jtbl_80121B30
//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", func_800B1540);

static u8 func_800B1540(u16 mapIndex, u8 arg1) {

    u8 result = 0;
    
    switch (arg1) {
        
        case 1:

            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = 1;
                setEntrance(0x1B);
            }
            
            break;

    
        case 2:
            
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = 1;
                setEntrance(0x45);
            }
            
            break;

        case 3:
            
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = 1;
                setEntrance(0x48);
            }
            
            break;

        case 4:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (gPlayer.direction == SOUTH) {
                    
                    if (checkDailyEventBit(0x28) || checkDailyEventBit(FESTIVAL)) { 
                        
                        showTextBox(1, 6, 0x168, 0, 2);
                        result = 1;
                        
                    } else {
                        
                        if (gYear == 1) {

                            if (!(gSeason == AUTUMN && (19 < gDayOfMonth && gDayOfMonth < 28))) {
                                
                                if (gSeason == WINTER) {
                                    if (8 < gDayOfMonth && gDayOfMonth < 17) {
                                        showTextBox(1, 6, 0x168, 0, 2);
                                        result = 1;
                                    } else {
                                        levelInteractionsInfo.mapAdditionsIndex = 3;
                                        result = 1;
                                        setEntrance(0x49);
                                    }
                                } else {
                                    levelInteractionsInfo.mapAdditionsIndex = 3;
                                    result = 1;
                                    setEntrance(0x49);
                                }

                            } else {
                                showTextBox(1, 6, 0x168, 0, 2);
                                result = 1;
                            }
                            
                        } else {
                            levelInteractionsInfo.mapAdditionsIndex = 3;
                            result = 1;
                            setEntrance(0x49);
                        }
                    }
                    
                    levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN;
                    
                }
            
            }

            break;
    
        case 5:

            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = 1;
                setEntrance(0x25);
            }
            
            break;

    
        case 6:

            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = 1;
                setEntrance(0x24);
            }
            
            break;

    
        case 17:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (gPlayer.heldItem == 0) {

                    if (func_800309B4(PLAYER, 0.0f, 16.0f) == 0x11) {
                        setDailyEventBit(6);
                        setPlayerAction(0x1A, 0x1B);
                        result = 2;
                    }
                    
                }
            }
            
            break;
    
        case 18:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (gPlayer.direction == NORTH) {
                    setDailyEventBit(6);
                    setPlayerAction(0x1C, 0xE);
                    result = 2;
                }
            
            }

            break;
    
        case 19:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                showTextBox(1, 6, 0xAF, 0, 2);
                result = 1;
            
            }

            break;

    }

    return result;
    
}

// jtbl_80121B80
//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", func_800B1808);

static u8 func_800B1808(u16 mapIndex, u8 arg1) {

    bool result = 0;

    switch (arg1) {

        case 1:

            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = 1;
                setEntrance(0x1C);
            }
            
            break;

        case 2:

            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = 1;
                setEntrance(0x22);
            }
            
            break;

        case 17:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                showTextBox(1, 6, 0xAB, 0, 2);
                result = 1;
            
            }

            break;

        // hot springs
        case 18:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {  

                if (!checkDailyEventBit(0x28)) {

                    if (gPlayer.heldItem == 0) {
                        if (-48.0f <= entities[PLAYER].coordinates.x && entities[PLAYER].coordinates.x < -40.0f) {
                            setDailyEventBit(6);
                            setPlayerAction(0x1D, 0);
                            levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN;
                            result = 2;
                        }
                    }
                    
                }
            
            }

            break;
        
        case 20:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                showTextBox(1, 6, 0xAE, 0, 2);
                result = 1;
            }

            break;
        
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", func_800B1994);

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

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
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
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                showTextBox(1, 6, 0xB1, 0, 2);
                result = TRUE;            
            }

            break;

    }
    
    return result;
    
}

// jtbl_80121BD0
//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", func_800B1AC4);

static bool func_800B1AC4(u16 mapIndex, u8 arg1) {

    bool result = FALSE;

    switch (arg1) {

        case 1:

            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = TRUE;
                setEntrance(0x1);
            }
            
            break;


        case 2:

            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = TRUE;
                setEntrance(0x29);
            }
            
            break;


        case 3:

            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = TRUE;
                setEntrance(0x2E);
            }
            
            break;

        case 4:

            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = TRUE;
                setEntrance(0x32);
            }
            
            break;


        case 5:

            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = TRUE;
                setEntrance(0x3E);
            }
            
            break;


        case 6:

            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = TRUE;
                setEntrance(0x1A);
            }
            
            break;

        case 16:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                showTextBox(1, 6, 0xB4, 0, 2);
                result = TRUE;
            
            }

            break;

        case 17:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                showTextBox(1, 6, 0xB6, 0, 2);
                result = TRUE;
            
            }

            break;

        case 18:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                showTextBox(1, 6, 0xB7, 0, 2);
                result = TRUE;
            
            }

            break;
        
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", handleCarpenterHutInteractions);

static bool handleCarpenterHutInteractions(u16 mapIndex, u8 arg1) {

    bool result = FALSE;
    
    switch (arg1) {
        
        case 1:

            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = TRUE;
                setEntrance(0x20);
            } 
        
            break;

        case 16:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {

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

        case 17:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                showTextBox(0, 6, 0x91, 0, 0);
                result = TRUE;
            }

            break;
    }

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", handleDumplingHouseExit);

static bool handleDumplingHouseExit(u16 mapIndex, u8 arg1) {

    bool result = FALSE;

    switch (arg1) {

        case 1:

            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = TRUE;
                setEntrance(DUMPLING_HOUSE_EXIT);
            }

            break;

    } 

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", func_800B1DBC);

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

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
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

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                showTextBox(1, 6, 0xB2, 0, 2);
                result = TRUE;
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN;        
            }

            break;
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", func_800B1EE4);

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

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
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

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {  
                showTextBox(1, 6, 0x9D, 0, 2);
                result = TRUE;
            }

            break;

        case 18:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

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
            
            break;

    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", func_800B2078);

// mine
static bool func_800B2078(u16 mapIndex, u8 arg1) {

    bool result = FALSE;

    switch (arg1) {
        
        case 1:

            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = TRUE;
                setEntrance(MINE_EXIT);
            }

            break;

    } 
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", func_800B20C8);

// pond
static bool func_800B20C8(u16 mapIndex, u8 arg1) {
    
    bool result = FALSE;

    switch (arg1) {
        
        case 1:

            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) { 
                result = TRUE;
                setEntrance(0x1F);                
            }
            
            break;

    }
    
    return result;

}

// vineyard
//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", func_800B2118);

bool func_800B2118(u16 mapIndex, u8 arg1) {

    bool result = FALSE;

    switch (arg1) {

        case 1:

            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = TRUE;
                setEntrance(0x16);
            }
            
            break;

        case 2:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (checkDailyEventBit(0x28) || checkDailyEventBit(FESTIVAL)) { 
                    showTextBox(1, 6, 0x168, 0, 2);
                    result = TRUE;
                } else {
                    levelInteractionsInfo.mapAdditionsIndex = 0;
                    result = TRUE;
                    setEntrance(0x51);
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN;
                
            }
        
            break;

        case 3:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (checkDailyEventBit(0x28) || checkDailyEventBit(FESTIVAL)) { 
                    showTextBox(1, 6, 0x168, 0, 2);
                    result = TRUE;
                } else {
                    levelInteractionsInfo.mapAdditionsIndex = 1;
                    result = TRUE;
                    setEntrance(0x53);
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN;
                
            }
        
            break;
        
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", func_800B2264);

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

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                showTextBox(1, 6, 0xCF, 0, 2);
                result = TRUE;
            }

            break;
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", func_800B2340);

static bool func_800B2340(u16 mapIndex, u8 arg1) {

    bool result = FALSE;    
    
    switch (arg1) {

        case 1:
        
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {

                if (gPlayer.direction == WEST) { 
                    result = TRUE;
                    setEntrance(0x50);
                }

            }

            break;

    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", func_800B23A4);

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
                 if (SOUTHWEST < gPlayer.direction && gPlayer.direction < (MAX_DIRECTIONS + 1) || gPlayer.direction == SOUTHWEST) {
                    result = TRUE;
                    setEntrance(0x55);
                 }
             }

            break;     

        case 16:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {            
                showTextBox(1, 6, 0xD2, 0, 2);
                result = TRUE;
            }          

            break;

        case 17:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                showTextBox(1, 6, 0xD3, 0, 2);
                result = TRUE;
            }
            break;

    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", func_800B24D4);

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

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                showTextBox(1, 6, 0xD4, 0, 2);            
                result = TRUE;
            }

            break;

    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", func_800B256C);

// race track
static bool func_800B256C(u16 mapIndex, u8 arg1) {

    bool result = FALSE; 
    
    switch (arg1) {
        case 1:
            result = TRUE;
            setEntrance(0x62);
            break;
        case 16:
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
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

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", func_800B2604);

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
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                
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
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
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
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

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

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", func_800B27CC);

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

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                showTextBox(1, 6, 0x95, 0, 2);
                result = TRUE;
            }

            break;

        case 17:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                showTextBox(1, 6, 0x93, 0, 2);
                result = TRUE;
            }

            break;

        case 18:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                showTextBox(1, 6, 0xD0, 0, 2);
                result = TRUE;
            }

            break;
    }
    
    return result;
}

// jtbl_80121C30
//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", handleRanchStoreExits);

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

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {

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

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A) && gPlayer.direction == EAST) {

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

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(0x1A)) {
                        func_800DC9FC(0x1A);
                    }
                    result = 2;
                }
            }

            break;

        case 18:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(0x1D)) {
                        func_800DC9FC(0x1D);
                    }
                    result = 2;
                }
            }

            break;

        case 19:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                if (gPlayer.heldItem == 0) {
                     if (checkShopItemShouldBeDisplayed(0x1B)) {
                        func_800DC9FC(0x1B);
                    }
                    result = 2;
                }
            }

            break;

        case 20:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                if (gPlayer.heldItem == 0) {
                     if (checkShopItemShouldBeDisplayed(0x1C)) {
                        func_800DC9FC(0x1C);
                    }
                    result = 2;
                }
            }

            break;

        case 21:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                showDialogueBox(1, 0x3D, 0x29, 0x80, 2);
                result = 1;
            }

            break;

   }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", func_800B2B90);

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

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                showTextBox(1, 6, 0xCF, 0, 2);
                result = TRUE;
            }

            break;

    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", func_800B2C28);

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

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", func_800B2C78);

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
