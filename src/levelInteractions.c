#include "common.h"

#include "controller.h"

extern Player gPlayer;

extern u8 D_80126560;
extern u8 D_80126561;

u32 checkDailyEventBit(u16 bitIndex);                   
void setExit(u16 extiIndex);  

u8 func_800309B4(u16, u8, f32); 
u32 func_8004D380(u8, u32);                        
void func_80059334();                          
void func_8005AE8C(u16 arg0, u16 arg1, u16 arg2, int arg3, u16 arg4);                    
void func_8005AF94(u16, u16, u16, u16, u8);                   
void func_8005B09C(u8);                                 
void func_8005CA2C(u16, u16);                                             
void func_800DC9FC(u8);                                 
u8 func_800DDDFC(u16);                               


//INCLUDE_ASM(const s32, "levelInteractions", func_800ACD70);

u8 func_800ACD70(u16 mapIndex) {

    u8 temp;
    u8 result;

    result = 0xFF;
    
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

    u8 temp;
    u8 temp2;
    u8 result;

    result = 0xFF;
    
    if (mapIndex == BARN) {
        temp = func_800309B4(0, 0, 32.0f);
        temp2 = temp - 0x12;
        if (temp2 < 9) {
            result = temp;
        }
    }

    return result;
}

//INCLUDE_ASM(const s32, "levelInteractions", func_800ACE50);

// coop/chicken feed
u8 func_800ACE50(u16 mapIndex) {

    u8 temp_v0;
    u8 var_s0;

    var_s0 = 0xFF;
    
    if (mapIndex == COOP) {
        
        temp_v0 = func_800309B4(0, 0, 32.0f);

        if (((temp_v0 - 0x13) & 0xFF) < 6U) {
            var_s0 = temp_v0;
        }

    }
    
    return var_s0;
}

//INCLUDE_ASM(const s32, "levelInteractions", func_800ACEAC);

u8 func_800ACEAC(u16 mapIndex) {
    u8 result;

    result = 0xFF;
    
    if (mapIndex == FARM) {
        result = -(func_800309B4(0, 0, 32.0f) != 0x17) | 1;
    }

    return result;
}

//INCLUDE_ASM(const s32, "levelInteractions", func_800ACEF8);

u8 func_800ACEF8(u16 mapIndex) {
    
    u8 result;
    u8 temp;
    u8 temp2;
    
    result = 0xFF;
    
    if (mapIndex == FARM) {
        temp = func_800309B4(0, 0.0f, 32.0f);
        temp2 = temp - 0x1B;
        temp2 = (temp2 < 2 | temp == 0x1D);
        temp2 = -(temp2 < 1) | 1;
        result = temp2;
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

INCLUDE_ASM(const s32, "levelInteractions", func_800ACFE8);

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
INCLUDE_ASM(const s32, "levelInteractions", func_800AD1D0);

// jtbl_80121578
INCLUDE_ASM(const s32, "levelInteractions", func_800AD8D0);

// jtbl_801215F0
INCLUDE_ASM(const s32, "levelInteractions", func_800ADCDC);

// jtbl_80121678
INCLUDE_ASM(const s32, "levelInteractions", func_800AE00C);

// jtbl_801216E8
INCLUDE_ASM(const s32, "levelInteractions", func_800AEB54);

// jtbl_80121748
INCLUDE_ASM(const s32, "levelInteractions", func_800AED60);

INCLUDE_ASM(const s32, "levelInteractions", func_800AEE8C);

INCLUDE_ASM(const s32, "levelInteractions", func_800AF060);

// jtbl_801217A0
INCLUDE_ASM(const s32, "levelInteractions", func_800AF0B0);

// jtbl_80121840
INCLUDE_ASM(const s32, "levelInteractions", func_800AF494);

//INCLUDE_ASM(const s32, "levelInteractions", func_800AFA2C);

u8 func_800AFA2C(u8 arg0, u8 flag) {
    
    u8 result = 0;
    
    if (flag == 1) {
        if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
            result = 1;
            setExit(FLOWER_SHOP_ENTRANCE);
        }
    } 
    
    return result;
}

// jtbl_801218B8
INCLUDE_ASM(const s32, "levelInteractions", func_800AFA7C);

INCLUDE_ASM(const s32, "levelInteractions", func_800AFCD0);

// jtbl_80121900
INCLUDE_ASM(const s32, "levelInteractions", func_800AFD20);

// jtbl_80121960
INCLUDE_ASM(const s32, "levelInteractions", func_800AFF9C);

INCLUDE_ASM(const s32, "levelInteractions", func_800B00E0);

INCLUDE_ASM(const s32, "levelInteractions", func_800B01EC);

// jtbl_801219B0
INCLUDE_ASM(const s32, "levelInteractions", func_800B0378);

// jtbl_80121A10
INCLUDE_ASM(const s32, "levelInteractions", func_800B0714);

INCLUDE_ASM(const s32, "levelInteractions", func_800B0A64);

INCLUDE_ASM(const s32, "levelInteractions", func_800B0AFC);

INCLUDE_ASM(const s32, "levelInteractions", func_800B0C48);

// jtbl_80121A78
INCLUDE_ASM(const s32, "levelInteractions", func_800B0C98);

INCLUDE_ASM(const s32, "levelInteractions", func_800B0FB8);

// jtbl_80121AC8
INCLUDE_ASM(const s32, "levelInteractions", func_800B106C);

INCLUDE_ASM(const s32, "levelInteractions", func_800B1438);

// jtbl_80121B30
INCLUDE_ASM(const s32, "levelInteractions", func_800B1540);

// jtbl_80121B80
INCLUDE_ASM(const s32, "levelInteractions", func_800B1808);

INCLUDE_ASM(const s32, "levelInteractions", func_800B1994);

// jtbl_80121BD0
INCLUDE_ASM(const s32, "levelInteractions", func_800B1AC4);

#ifdef PERMUTER
u32 func_800B1C6C(u32 arg0, u8 arg1) {

    u32 temp = 0;
    int temp2 = arg1;
    
    if (temp2 != 0x10) {
        if (temp2 < 0x11) {
            if (temp2 == 1) {
                temp = 0;
                goto func_end;
            }
            if (checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                temp = 0;
                goto func_end;
            }        
            temp = 1;
            setExit(0x20);
            goto func_end;
        }


    } else {
       if (func_8004D380(CONTROLLER_1, BUTTON_A)) {
           
            if (!checkDailyEventBit(8)) {
                
                func_80059334();
                func_8005CA2C(1, 0x18);
                temp = 1;
                goto func_end;            
           } else {
                //func_8005AE8C(0,6,0x6E,0,0);
           }
                
           if (!func_8004D380(CONTROLLER_1, BUTTON_A)) {
               temp = 0;
               goto func_end;
           } 
           
           func_8005AE8C(0,6,0x91,0,0);
           temp = 1;
        }      
   }

func_end:
    return temp;
}
#else
INCLUDE_ASM(const s32, "levelInteractions", func_800B1C6C);
#endif

//INCLUDE_ASM(const s32, "levelInteractions", handleDumplingHouseExit);

u32 handleDumplingHouseExit(u32 arg0, u8 arg1) {
    u32 result = 0;

    if (arg1 == 1) {
        if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
            result = 1;
            setExit(DUMPLING_HOUSE_EXIT);
        }
    } 

    return result;
}

INCLUDE_ASM(const s32, "levelInteractions", func_800B1DBC);

INCLUDE_ASM(const s32, "levelInteractions", func_800B1EE4);

//INCLUDE_ASM(const s32, "levelInteractions", func_800B2078);

u32 func_800B2078(u32 arg0, u8 flag) {
    u32 result = 0;

    if (flag == 1) {
        if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
            result = 1;
            setExit(MINE_EXIT);
        }
    } 
    
    return result;
}

INCLUDE_ASM(const s32, "levelInteractions", func_800B20C8);

INCLUDE_ASM(const s32, "levelInteractions", func_800B2118);

INCLUDE_ASM(const s32, "levelInteractions", func_800B2264);

INCLUDE_ASM(const s32, "levelInteractions", func_800B2340);

INCLUDE_ASM(const s32, "levelInteractions", func_800B23A4);

INCLUDE_ASM(const s32, "levelInteractions", func_800B24D4);

INCLUDE_ASM(const s32, "levelInteractions", func_800B256C);

// jtbl_80121C18
INCLUDE_ASM(const s32, "levelInteractions", func_800B2604);

//INCLUDE_ASM(const s32, "levelInteractions", func_800B27CC);

u32 func_800B27CC(u8 mapIndex, u8 collisionIndex) {

    u32 result = 0;

    switch (collisionIndex) {
        case 1: 
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = 1;
                setExit(RANCH_FROM_RANCH_HOUSE);
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

u32 handleRanchStoreExits(u8 mapIndex, u8 collisionIndex) {

    u32 result = 0;
    
    switch (collisionIndex) {
        // exit
        case 1:
            if (!checkDailyEventBit(STORES_CLOSED_FOR_FESTIVAL)) {
                result = 1;
                setExit(RANCH_FROM_RANCH_STORE);
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
                    setExit(ANN_ROOM_ENTER);
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

INCLUDE_ASM(const s32, "levelInteractions", func_800B2B90);

INCLUDE_ASM(const s32, "levelInteractions", func_800B2C28);

INCLUDE_ASM(const s32, "levelInteractions", func_800B2C78);
