#include "common.h"

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
#include "npc.h"
#include "player.h"
#include "shop.h"
#include "weather.h"

// bss
// struct
u8 D_80126560;
u8 D_80126561;
u8 D_80189826; 
u8 D_801C3E2C;
                                                                 
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
u8 func_800ACE50(u16 mapIndex) {

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
            result = 1;
        }
    }
    
    if (mapIndex == POND) {
        if (func_800309B4(0, 0.0f, 32.0f) == 0x10) {
            result = 1;
        }
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "levelInteractions", func_800ACFE8);

bool func_800ACFE8(u16 mapIndex) {

    bool result = 0;
    u8 temp;
    
    if (mapIndex == FARM) {
        temp = func_800309B4(0, 0, 32.0f);
        result = ((0x1A < temp && temp < 0x1D) || temp == 0x1D);
    }

    if (mapIndex == GREENHOUSE) {
        if (func_800309B4(0, 0, 32.0f) == 0x12) { 
            result = 1;
        }
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "levelInteractions", checkWineBarrelInteraction);

u8 checkWineBarrelInteraction(u16 mapIndex) {

    u8 result = 0;
    
    if (mapIndex == VINEYARD_CELLAR_BASEMENT) {
        result = func_800309B4(0, 0, 32.0f) == 0x11;
    }

    return result;
}

//INCLUDE_ASM(const s32, "levelInteractions", func_800AD0C4);

u8 func_800AD0C4(u16 mapIndex) {

    u8 result = 0;
    u8 object;
    
    if (mapIndex == MOUNTAIN_1) {

        object = func_800309B4(0, 0, 64.0f);

        if (object == 0x10 || object == 0x14) {
            gPlayer.fatigue[3] = 0;
            result = 1;
        }
        
        if (object == 0x12) {
            gPlayer.fatigue[3] = 1;
            result = 1;
        }
    }
    
    if (mapIndex == MOUNTAIN_2) {
        if (func_800309B4(0, 0, 64.0f) == 0x10) {
            gPlayer.fatigue[3] = 2;
            result = 1;
        }
    }
    
    if (mapIndex == BEACH) {
        if (func_800309B4(0, 0, 64.0f) == 0x10) {
            gPlayer.fatigue[3] = 3;
            result = 1;
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
    
    D_80126560 = 0xFF;
    D_80126561 = 0xFF;

    temp = func_800309B4(0, 0, 32.0f);

    D_80189826 = temp;
    
    if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
        D_801C3E2C = D_80189826;
    } else {
        D_801C3E2C = 0xFF;
    }

    set = 0;
    
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

    if (D_80126561 != 0xFF) {
        setAudio(D_80126561);
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
                if (D_80126560 != 0xFF) {
                    func_80038990(0, D_80126560, 0);
                }
                func_8005C940(8, 2);
                return set;
                
            } else {
                switch (func_80074C38(gEntranceIndex)) {                           
                    case 0x46:                                  
                        func_8005AE8C(0, 6, temp2, 0, 0);
                        setPlayerAction(0xF, 0);
                        gPlayer.unk_6F = 0x20;
                        gPlayer.unk_6E = 6;
                        break;
                    case 0x31:                               
                    case 0x3D:                                
                    case 0x4:                                  
                    case 0x27:                    
                        func_8005AE8C(0, 6, temp2, 0, 0);
                        break;
                    default:            
                        func_8005AE8C(1, 6, temp2, 0, 2);
                        break;
                    }
                    
                gEntranceIndex = tempExit;
            }

        } else {
            set = 0;
        }
        
        gEntranceIndex = tempExit;
        
    } 
    
    return set;
}

// jtbl_80121578
//INCLUDE_ASM(const s32, "levelInteractions", func_800AD8D0);

// farm
u8 func_800AD8D0(u16 mapIndex, u8 arg1) {

    u8 result = 0;
    u8 temp;
    
    switch (arg1) {

        case 1:
        case 2:
            break;
            break;
         
        case 3:        
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = 1;
                setEntrance(0x14);
            }
            
            return result;
        
        case 4:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                if (gPlayer.direction == 4) {
                    result = 1;
                    D_80126560 = 9;
                    D_80126561 = 0x32;
                    setEntrance(0xA);
                }
            }
            break;
        
        case 5:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                if (gPlayer.direction == 4) {
                    result = 1;
                    D_80126560 = 0xD;
                    D_80126561 = 0x32;
                    setEntrance(0xE);
                }
            }
            break;
        
        case 6:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                result = 1;
                D_80126560 = 0xA;
                D_80126561 = 0x32;
                setEntrance(0x11);
            }
            break;
        
        case 7:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                result = 1;
                D_80126560 = 0xB;
                D_80126561 = 0x32;
                setEntrance(0x12);
            }
            break;

        case 8:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                result = 1;
                D_80126560 = 0x14;
                D_80126561 = 0x32;
                setEntrance(0x13);
            }
            break;

        case 9:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                if (gPlayer.direction == 6) {
                    result = 1;
                    D_80126560 = 0xC;
                    D_80126561 = 0x32;
                    setEntrance(0xB);
                }
            }
            break;
        
        case 26:
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = 1;
                setEntrance(0x70);
            }
            break;
        
        case 17:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                if (gPlayer.unk_2C == 0) {
                    if (!(gPlayer.flags & 1)) {
                        if (gLumber != 0) {
                            gPlayer.unk_2C = 3;
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
                    func_8005AE8C(1, 5, func_80063A2C(temp), 0, 2);
                    if (temp == 0x23) {
                        acquireKeyItem(GOLD_PENDANT);
                        setSpecialDialogueBit(HAVE_GOLD_PENDANT);
                    }
                } else {
                    func_8005AE8C(1, 5, 0x3A, 0, 2);
                }

                result = 1;
            }
            break;
        
        case 19:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                
                if (gPlayer.direction == 2) {
                    func_8003FBD8(0x18, gLumber, 0);
                    func_8005AE8C(1, 6, 0xA3, 0, 2);
                    result = 1;
                }
            }
            break;

        case 20:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                func_8003FBD8(0x17, D_801806D0.unk_44, 0);
                func_8005AE8C(1, 6, 0xA2, 0, 2);
                result = 1;
            }
            break;
        
        case 21:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                func_8005AE8C(1, 6, 0xA1, 0, 2);
                result = 1;
            }
            break;
        
        case 25:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                if (!checkHaveKeyItem(TREASURE_MAP)) {
                    func_8005AE8C(1, 6, 0xA7, 0, 2);
                    acquireKeyItem(TREASURE_MAP);
                    result = 1;
                    D_80189828.unk_F = 0;
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
bool func_800ADCDC(u16 mapIndex, u8 arg1) {

    bool result = 0;
    
    switch (arg1) {
        case 1:
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                if (gWeather != TYPHOON) {
                    result = 1;
                    setEntrance(0);
                }
            }
            break;     
        case 2:
            if (func_8004D380(CONTROLLER_1, BUTTON_A) && gWeather != TYPHOON) {
                D_80126560 = 2;
                D_80126561 = 0x32;
                result = 1;
                if (checkLifeEventBit(0x10)) {                   
                    setEntrance(0xF);
                } else {
                    setEntrance(8);
                }
            }            
            break;
        case 3:
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                if (gWeather != 5) {
                    result = 1;
                    setEntrance(0x6F);
                }
            }
            break;
        case 0x10:
            if (func_8004D380(CONTROLLER_1, BUTTON_A) && gPlayer.unk_2C == 0 && checkLifeEventBit(0x3E) && !checkDailyEventBit(0x15)) {
                func_8005B09C(0);
                result = 1; 
            }
            break;
        case 0x12:
            if (func_8004D380(CONTROLLER_1, BUTTON_A) && gPlayer.direction == 4) {
                func_800B3694();
                func_800D8540();
                setPlayerAction(0xA, 0xC);
                D_80126561 = 0x2F;
                result = 2;
            }
            break;
        case 0x13:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                func_80038990(0, 3, 1);
                func_8005CA2C(0xA, 0x15);
                D_80126561 = 0x32;
                result = 1;
            }
            break;
        case 0x14:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                setMainLoopCallbackFunctionIndex(0x1B);
                D_80126561  = 8;
                result = 1;
            }
            break;
        case 0x15:
            if (func_8004D380(CONTROLLER_1, BUTTON_A) != 0) {
                func_80038990(0, 0xA, 1);
                func_8005CA2C(0xA, 0x17);
                D_80126561 = 0x32;
                result = 1;
            }
            break;
        case 0x16:
            if (func_8004D380(CONTROLLER_1, BUTTON_A) && gPlayer.unk_2C == 0) {
                if (checkDailyEventBit(0x16)) {
                    if (gBabyAge < 30) {
                        gPlayer.unk_2C = 0xBA;             
                        func_8002FA2C(npcInfoArray[BABY].spriteIndex);
                        npcInfoArray[BABY].flags &= ~4;
                        setPlayerAction(4, 6);
                        if (!checkDailyEventBit(0x55)) {
                            npcAffection[5] += adjustValue(npcAffection[5], 2, 0xFF);
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
bool func_800AEB54(u16 mapIndex, u8 arg1) {

    bool result = 0;

    switch (arg1) {
        
        case 1:
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = 1;
                setEntrance(3);
            }
            break;
        
        case 18:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                func_8005AE8C(1, 6, 0xAA, 0, 2);
                result = 1;
            }
            break;

        case 19:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                if (gChickens[0].flags & 1) {
                    func_8005AE8C(1, 6, 0xF5, 0, 2);
                    result = 1;
                }
            }
            break;
        
        case 20:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                if (gChickens[1].flags & 1) {
                    func_8005AE8C(1, 6, 0xF6, 0, 2);
                    result = 1;
                }
            }
            break;
        
        case 21:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                if (gChickens[2].flags & 1) {
                    func_8005AE8C(1, 6, 0xF7, 0, 2);
                    result = 1;
                }   
            }           
            break;
        
        case 22:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                if (gChickens[3].flags & 1) {
                    func_8005AE8C(1, 6, 0xF8, 0, 2);
                    result = 1;
                }         
            }       
            break;
        
        case 23:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                if (gChickens[4].flags & 1) {
                    func_8005AE8C(1, 6, 0xF9, 0, 2);
                    result = 1;
                }
            }       
            break;
        
        case 24:     
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                if (gChickens[5].flags & 1) {
                    func_8005AE8C(1, 6, 0xFA, 0, 2);
                    result = 1;
                }          
            }       
            break;        
    }

    return result;
    
}

// jtbl_80121748
//INCLUDE_ASM(const s32, "levelInteractions", func_800AED60);

// kitchen
bool func_800AED60(u16 mapIndex, u8 arg1) {

    bool result = 0;

    switch (arg1) {
        case 1:
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = 1;
                setEntrance(0xB);
            }
            break;
        case 2:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                result = 1;
                D_80126560 = 2;
                D_80126561 = 0x32;
                setEntrance(0xD);
            }        
            break;
        case 16:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                func_80038990(0, 0, 1);
                func_8005CA2C(0xA, 0x16);
                D_80126561 = 0x32;
                result = 1;
            }
            break;
        case 21:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                setMainLoopCallbackFunctionIndex(0x1C);
                D_80126561 = 8;
                result = 1;
            }
            break;
        case 22:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                setMainLoopCallbackFunctionIndex(0x1A);
                D_80126561 = 8;
                result = 1;
            }
            break;
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "levelInteractions", func_800AEE8C);

// bathroom
bool func_800AEE8C(u16 mapIndex, u8 arg1) {

    bool result = 0;

    switch (arg1) {

        case 1:
            
            result = 1;
            
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
                if (gPlayer.unk_2C == 0 && renderedSprites[PLAYER].startingCoordinates.z >= -44.0f && renderedSprites[PLAYER].startingCoordinates.z < -36.0f) {
                    setDailyEventBit(6);
                    setPlayerAction(0x11, 0);
                    D_80126561 = 0x32;
                    result = 2;
                }
            }         

            break;

        case 0x11:

            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                if (gPlayer.unk_2C == 0 && renderedSprites[PLAYER].startingCoordinates.x >= 42.0f && renderedSprites[PLAYER].startingCoordinates.x < 50.0f) {
                    setDailyEventBit(6);
                    setPlayerAction(0x10, 0);
                    gHappiness += adjustValue(gHappiness, 2, 0xFF);
                    result = 2;
                    D_80126561 = 0x32;
                }
            }            

            break;
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "levelInteractions", func_800AF060);

// greenhouse
bool func_800AF060(u16 arg0, u8 arg1) {

    bool result = 0;

    if (arg1 == 1) {
        if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
            result = 1;
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

u8 func_800AFA2C(u16 mapIndex, u8 flag) {
    
    u8 result = 0;
    
    if (flag == 1) {
        if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
            result = 1;
            setEntrance(FLOWER_SHOP_ENTRANCE);
        }
    } 
    
    return result;
}

// jtbl_801218B8
INCLUDE_ASM(const s32, "levelInteractions", func_800AFA7C);

//INCLUDE_ASM(const s32, "levelInteractions", func_800AFCD0);

bool func_800AFCD0(u16 mapIndex, u8 arg1) {

    bool result = 0;

    if (arg1 == 1) {
        if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
            result = 1;
            setEntrance(0x5A);
        }
    }
    
    return result;
    
}

// jtbl_80121900
INCLUDE_ASM(const s32, "levelInteractions", func_800AFD20);

// jtbl_80121960
INCLUDE_ASM(const s32, "levelInteractions", func_800AFF9C);

//INCLUDE_ASM(const s32, "levelInteractions", func_800B00E0);

// church
bool func_800B00E0(u16 mapIndex, u8 arg1) {

    bool result = 0;

    switch (arg1) {
        case 1:
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = 1;
                setEntrance(0x38);
            }
            break;
        case 0x10:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {            
                func_8005AE8C(1, 6, 0xC0, 0, 2);
                result = 1;
            }            
            break;
        case 17:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                func_8005AE8C(1, 6, 0xBF, 0, 2);
                result = 1;
            }
            break;
        case 19:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                func_8005AE8C(1, 6, 0xD0, 0, 2);
                result = 1;
            }
            break;
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "levelInteractions", func_800B01EC);

// tavern
bool func_800B01EC(u16 arg0, u8 arg1) {

    bool result = 0;

    switch (arg1) {
        case 1:
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = 1;
                setEntrance(0x39);
            }
            break;
        case 17:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                func_8005AE8C(0, 6, 0x78, 0, 0);
                result = 1;
            }
            break;
        case 18:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {            
                if (gPlayer.unk_2C == 0) {
                    func_8005B09C(4);
                    result = 1;
                }  
            }
            break;
        case 19:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {   
                switch (gSeason) {
                    case SPRING:
                        func_8005AE8C(1, 6, 0xCB, 0, 2);
                        result = 1;
                        break;
                    case SUMMER:
                        func_8005AE8C(1, 6, 0xCC, 0, 2);
                        result = 1;
                        break;
                    case AUTUMN:
                        func_8005AE8C(1, 6, 0xCD, 0, 2);
                        result = 1;
                        break;
                    case WINTER:
                        func_8005AE8C(1, 6, 0xCE, 0, 2);
                        result = 1;
                        break;      
                    default:
                        result = 1;
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
bool func_800B0A64(u16 mapIndex, u8 arg1) {

    bool result = 0;

    switch (arg1) {
        case 1:
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) { 
                result = 1;
                setEntrance(0x43);
            }
            break;
        case 16:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                func_8005AE8C(1, 6, 0xD1, 0, 2);
                result = 1;
            }
        default:
            break;
    }
    
    return result;
    
}

INCLUDE_ASM(const s32, "levelInteractions", func_800B0AFC);

//INCLUDE_ASM(const s32, "levelInteractions", func_800B0C48);

bool func_800B0C48(u16 mapIndex, u8 arg1) {

   bool result = 0;

    if (arg1 == 1) {
        if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
            result = 1;
            setEntrance(0x66);
        }
    } 

    return result;
    
}

// jtbl_80121A78
INCLUDE_ASM(const s32, "levelInteractions", func_800B0C98);

//INCLUDE_ASM(const s32, "levelInteractions", func_800B0FB8);

bool func_800B0FB8(u16 mapIndex, u8 arg1) {

    bool result = 0;
    int temp;

    switch (arg1) {

        case 1:
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = 1;
                setEntrance(0x69);
            }
            break;

        case 16:

            temp = 5;

            if (func_8004D380(CONTROLLER_1, BUTTON_A)) { 

                // if (((s32) gSeason < 5) && (gSeason != 0)) 
                if (gSeason < temp && gSeason) {
                    func_8005AE8C(1, 6, 0xD0, 0, 2);
                }
                result = 1;
            }
            break;
    }
    
    return result;
    
}

// jtbl_80121AC8
INCLUDE_ASM(const s32, "levelInteractions", func_800B106C);

//INCLUDE_ASM(const s32, "levelInteractions", func_800B1438);

// mountain 1
bool func_800B1438(u16 mapIndex, u8 arg1) {

    bool result = 0;

    switch (arg1) {
        case 1:
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) { 
                result = 1;
                setEntrance(0x19);                
            }
            break;
        case 2:
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) { 
                result = 1;
                setEntrance(0x1D);                
            }
            break;
        case 3:
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) { 
                result = 1;
                setEntrance(0x23);                
            }
            break;
        case 19:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) { 
                func_8005AE8C(1, 6, 0xAD, 0, 2);
                result = 1;
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
bool func_800B1994(u16 mapIndex, u8 arg1) {

    bool result = 0;

    switch (arg1) {

        case 1:
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) { 
                result = 1;
                setEntrance(0x21);                
            }
            break;

        case 2:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) { 
                if (checkDailyEventBit(0x28) || checkDailyEventBit(0x4D)) {
                    func_8005AE8C(1, 6, 0x168, 0, 2);
                    result = 1;
                } else {
                    result = 1;
                    D_80126560 = 0;
                    setEntrance(0x4A);                
                }
                
                D_80126561 = 0x32;
            }
            break;        

        case 16:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) { 
                func_8005AE8C(1, 6, 0xB1, 0, 2);
                result = 1;            
            }
            break;
    }
    
    return result;
    
}

// jtbl_80121BD0
INCLUDE_ASM(const s32, "levelInteractions", func_800B1AC4);

//INCLUDE_ASM(const s32, "levelInteractions", handleCarpenterHutInteractions);

bool handleCarpenterHutInteractions(u16 mapIndex, u8 arg1) {

    bool result = 0;
    
    switch (arg1) {
        case 1:
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = 1;
                setEntrance(0x20);
            } 
            break;
        case 0x10:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                if (!checkDailyEventBit(8)) {
                    func_80059334();
                    func_8005CA2C(1, 0x18);
                    result = 1;
                } else {
                    func_8005AE8C(0, 6, 0x6E, 0, 0);
                    result = 1;
                }     
            }  
            break;
        case 0x11:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                func_8005AE8C(0, 6, 0x91, 0, 0);
                result = 1;
            }
            break;
        default:
            break;
    }

    return result;
}

//INCLUDE_ASM(const s32, "levelInteractions", handleDumplingHouseExit);

bool handleDumplingHouseExit(u16 mapIndex, u8 arg1) {

    bool result = 0;

    if (arg1 == 1) {
        if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
            result = 1;
            setEntrance(DUMPLING_HOUSE_EXIT);
        }
    } 

    return result;
}

//INCLUDE_ASM(const s32, "levelInteractions", func_800B1DBC);

bool func_800B1DBC(u16 mapIndex, u8 arg1) {

    bool result = 0;

    switch (arg1) {

        case 1:
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) { 
                result = 1;
                setEntrance(0x1E);                
            }
            break;

        case 2:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) { 
                if (checkDailyEventBit(0x28) || checkDailyEventBit(0x4D)) {
                    func_8005AE8C(1, 6, 0x168, 0, 2);
                    result = 1;
                } else {
                    result = 1;
                    setEntrance(0x6D);
                }
            }
            break;

        case 3:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) { 
                func_8005AE8C(1, 6, 0xB2, 0, 2);
                result = 1;
                D_80126561 = 0x32;        
            }
            break;
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "levelInteractions", func_800B1EE4);

// harvest sprite cave
bool func_800B1EE4(u16 mapIndex, u8 arg1) {

    bool result = 0;

    switch (arg1) {

        case 1:
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) { 
                result = 1;
                setEntrance(0x46);                
            }
            break;

        case 16:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) { 
                if (getRandomNumberInRange(0, 1)) {
                    func_8005AE8C(1, 6, 0x94, 0, 2);
                    result = 1;
                } else {
                    func_8005AE8C(1, 6, 0x95, 0, 2);
                    result = 1;         
                }
            }
            break;

        case 17:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {  
                func_8005AE8C(1, 6, 0x9D, 0, 2);
                result = 1;
            }
            break;

        case 18:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) { 
                if (checkDailyEventBit(0x28) || checkDailyEventBit(0x4D)) {
                    func_8005AE8C(1, 6, 0x168, 0, 2);
                    result = 1;
                } else if (getRandomNumberInRange(0, 1)) {
                    func_8005AE8C(1, 6, 0x98, 0, 2);
                    result = 1;
                } else {
                    func_8005AE8C(1, 6, 0x99, 0, 2);
                    result = 1;
                }
                
            }
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "levelInteractions", func_800B2078);

// mine
bool func_800B2078(u16 mapIndex, u8 arg1) {

    bool result = 0;

    if (arg1 == 1) {
        if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
            result = 1;
            setEntrance(MINE_EXIT);
        }
    } 
    
    return result;
}

//INCLUDE_ASM(const s32, "levelInteractions", func_800B20C8);

// pond
bool func_800B20C8(u16 mapIndex, u8 arg1) {
    
    bool result = 0;

    if (arg1 == 1) {
        if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) { 
            result = 1;
            setEntrance(0x1F);                
        }
    }
    
    return result;
}

// vineyard
INCLUDE_ASM(const s32, "levelInteractions", func_800B2118);

//INCLUDE_ASM(const s32, "levelInteractions", func_800B2264);

// vineyard house
bool func_800B2264(u16 mapIndex, u8 arg1) {

    bool result = 0;

    switch (arg1) {
        case 1:
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) { 
                result = 1;
                setEntrance(0x52);                
            }
            break;
        case 2:
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) { 
                result = 1;
                setEntrance(0x2F);                
            }
            break;
        case 17:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) { 
                func_8005AE8C(1, 6, 0xCF, 0, 2);
                result = 1;
            }
            break;
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "levelInteractions", func_800B2340);

bool func_800B2340(u16 mapIndex, u8 arg1) {

    bool result = 0;    
    
    if (arg1 == 1) {
        if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
            if (gPlayer.direction == 6) { 
                result = 1;
                setEntrance(0x50);
            }
        }
    }
    
    return result;

}

//INCLUDE_ASM(const s32, "levelInteractions", func_800B23A4);

// vineyard cellar
bool func_800B23A4(u16 mapIndex, u8 arg1) {

    bool result = 0;
    
    switch (arg1) {
        case 1:
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = 1;
                setEntrance(0x30);
            }
            break;
         case 2:
             if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) { 
                 if (5 < gPlayer.direction && gPlayer.direction < 8 || gPlayer.direction == 5) {
                    result = 1;
                    setEntrance(0x55U);
                 }
             }
            break;     
        case 16:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {            
                func_8005AE8C(1, 6, 0xD2, 0, 2);
                result = 1;
            }            
            break;
        case 17:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
                func_8005AE8C(1, 6, 0xD3, 0, 2);
                result = 1;
            }
            break;

    }

    return result;
    
}

//INCLUDE_ASM(const s32, "levelInteractions", func_800B24D4);

// vineyard cellar basement
bool func_800B24D4(u16 mapIndex, u8 arg1) {

    bool result = 0; 

    switch (arg1) {
        case 1:
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) { 
                result = 1;
                setEntrance(0x54);
            }
            break;
        case 16:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) { 
                func_8005AE8C(1, 6, 0xD4, 0, 2);            
                result = 1;
            }
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "levelInteractions", func_800B256C);

// race track
bool func_800B256C(u16 mapIndex, u8 arg1) {

    bool result = 0; 
    
    switch (arg1) {
        case 1:
            result = 1;
            setEntrance(0x62);
            break;
        case 16:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) { 
                if (gPlayer.unk_2C == 0) {
                    if (checkDailyEventBit(0x41)) {
                        func_8005B09C(0xB);
                    } else {
                        func_8005B09C(0xC);
                    }
                    result = 1;
                }            
            }
            break;
    }
    
    return result;
}

// jtbl_80121C18
INCLUDE_ASM(const s32, "levelInteractions", func_800B2604);

//INCLUDE_ASM(const s32, "levelInteractions", func_800B27CC);

// ranch house
bool func_800B27CC(u16 mapIndex, u8 collisionIndex) {

    bool result = 0;

    switch (collisionIndex) {
        case 1: 
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = 1;
                setEntrance(RANCH_FROM_RANCH_HOUSE);
            }
            break;
        case 16:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) { 
                func_8005AE8C(1, 6, 0x95, 0, 2);
                result = 1;
            }
            break;
        case 17:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) { 
                func_8005AE8C(1, 6, 0x93, 0, 2);
                result = 1;
            }
            break;
        case 18:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) { 
                func_8005AE8C(1, 6, 0xD0, 0, 2);
                result = 1;
            }
            break;
    }
    
    return result;
}

// jtbl_80121C30
//INCLUDE_ASM(const s32, "levelInteractions", handleRanchStoreExits);

bool handleRanchStoreExits(u16 mapIndex, u8 collisionIndex) {

    bool result = 0;
    
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
                if (checkDailyEventBit(0x28) || checkDailyEventBit(0x4D)) {
                    func_8005AE8C(1, 6, 0x168, 0, 2);
                    result = 1;
                } else {
                    D_80126560 = 0;
                    result = 1;
                    setEntrance(ANN_ROOM_ENTER);
                }
                D_80126561 = 0x32;
            } 
            break;

        // the following seem like unused items/interactions
        case 16:
            if (func_8004D380(CONTROLLER_1, BUTTON_A) && gPlayer.direction == 2) {
                    if (!checkDailyEventBit(1) && !checkDailyEventBit(2)) {
                         if (checkDailyEventBit(5)) {
                            func_8005AE8C(0, 6, 0x165, 0, 0);
                        } else {
                            func_8005B09C(5);  
                        }
                    } else {
                        func_8005AE8C(0, 6, 0x164, 0, 0);
                    }
                result = 1;   
            } 
            break;
        case 17:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) { 
                if (gPlayer.unk_2C == 0) {
                    if (func_800DDDFC(0x1A)) {
                        func_800DC9FC(0x1A);
                    }
                    result = 2;
                }
            }
            break;
        case 18:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) { 
                if (gPlayer.unk_2C == 0) {
                    if (func_800DDDFC(0x1D)) {
                        func_800DC9FC(0x1D);
                    }
                    result = 2;
                }
            }
            break;
        case 19:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) { 
                if (gPlayer.unk_2C == 0) {
                     if (func_800DDDFC(0x1B)) {
                        func_800DC9FC(0x1B);
                    }
                    result = 2;
                }
            }
            break;
        case 20:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) { 
                if (gPlayer.unk_2C == 0) {
                     if (func_800DDDFC(0x1C)) {
                        func_800DC9FC(0x1C);
                    }
                    result = 2;
                }
            }
            break;
        case 21:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) { 
                func_8005AF94(1, 0x3D, 0x29, 0x80, 2);
                result = 1;
            }
            break;
   }
    
    return result;
}

//INCLUDE_ASM(const s32, "levelInteractions", func_800B2B90);

// ann room
bool func_800B2B90(u16 mapIndex, u8 arg1) {

    bool result = 0; 

    switch (arg1) {
        case 1:
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = 1;
                setEntrance(0x4C);
            }
            break;
        case 19:
            if (func_8004D380(CONTROLLER_1, BUTTON_A)) { 
                func_8005AE8C(1, 6, 0xCF, 0, 2);
                result = 1;
            }
            break;
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "levelInteractions", func_800B2C28);

// ranch barn
bool func_800B2C28(u16 mapIndex, u8 arg1) {

    bool result = 0;    

    if (arg1 == 1) {
        if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
            result = 1;
            setEntrance(0x2D);
        }
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "levelInteractions", func_800B2C78);

// beach
bool func_800B2C78(u16 mapIndex, u8 arg1) {

    bool result = 0;    
    
    switch (arg1) {
        case 1:
            break;
        case 2:
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = 1;
                setEntrance(0x2A);
            }
            break;
    }
    
    return result;
}

// hack to get this rodata to align along 10 bytes
static const padding[2] = { 0, 0 };
