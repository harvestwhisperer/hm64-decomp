#include "common.h"

#include "game/gameStatus.h"
#include "system/math.h"

#include "game/animals.h"
#include "game/game.h"
#include "game/level.h"
#include "game/fieldObjects.h"
#include "game/npc.h"
#include "game/player.h"

// bss
u32 dailyEventBits[];
u32 lifeEventBits[];
u32 specialDialogueBits[];
u32 readMailBits[];
u32 mailboxBits[];

// album bits
extern u32 albumBits;

// forward declaration
void toggleSpecialDialogueBit(u16);


// same as adjustValue from game.c
static inline int adjustValue2(int initial, int value, int max) {

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

// FIXME: should be adjustValue() call
static inline void handleAddShipment(s32 amount) {

    int temp;
    int checkShippingOverflow;
    int shippingValue;    
    int maxShipping;

    checkShippingOverflow = dailyShippingBinValue + amount;
    shippingValue = amount;
    maxShipping = MAX_TOTAL_SHIPPING;
    
    if (checkShippingOverflow > maxShipping) {
        temp = checkShippingOverflow - MAX_TOTAL_SHIPPING;
        shippingValue -= temp;
        checkShippingOverflow = maxShipping;
    }

    if (checkShippingOverflow < 0) {
        shippingValue -= checkShippingOverflow;
    }
    
    dailyShippingBinValue += shippingValue;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/gameStatus", handleWifeMorningHelp);

void handleWifeMorningHelp(void) {

    u8 temp;

    toggleSpecialDialogueBit(0x85);
    toggleSpecialDialogueBit(0x86);
    toggleSpecialDialogueBit(0x88);
    toggleSpecialDialogueBit(0x89);

    temp = getRandomNumberInRange(0, 1);

    switch (gWife) {

        case MARIA:

            if (!getRandomNumberInRange(0,  (5 - npcAffection[MARIA] / 50))) {
                if (temp) {
                    // weeding
                    func_80063D38();
                } else {
                    // eggs
                    func_80063E18();
                }
            } 
            break;

        case POPURI:

            if (!getRandomNumberInRange(0,  (5 - npcAffection[POPURI] / 50))) {
                if (temp) {
                    func_80063F3C();
                } else {
                    func_80063E18();
                }
            } 
            break;

        case ELLI:

            if (!getRandomNumberInRange(0,  (5 - npcAffection[ELLI] / 50))) {
                if (temp) {
                    func_80064048();
                } else {
                    func_80063E18();
                }
            } 
            break;       

        case ANN:

            if (!getRandomNumberInRange(0,  (5 - npcAffection[ANN] / 50))) {
                if (temp) {
                    func_80064048();
                    break;
                } else {
                    func_80063E18();
                }
            } 
            break;

        case KAREN:

            if (!getRandomNumberInRange(0,  (5 - npcAffection[KAREN] / 50))) {
                if (temp) {
                    func_80063E18();
                }
            } 
            break;

        default:
            break;
    }
}


//INCLUDE_ASM("asm/nonmatchings/game/gameStatus", func_80063D38);

// wife helps with weeding
void func_80063D38(void) {
    
    u8 i;
    u8 j;

    if (gSeason != WINTER) {

        for (i = 0; i < FIELD_HEIGHT; i++) {
            for (j = 0; j < FIELD_WIDTH; j++) {
                if (farmFieldTiles[i][j] == WEED) {
                    setSpecialDialogueBit(0x88);
                    setFieldTile(FARM, 1, j, i);
                } 
            }
        } 
        
    }
}
 
//INCLUDE_ASM("asm/nonmatchings/game/gameStatus", func_80063E18);

// wife crates eggs
void func_80063E18(void) {

    u8 i = 0;
 
    for (i = 0; i < MAX_CHICKENS; i++) {
        
        if (gChickens[i].flags & 1 && gChickens[i].type == 0 && !(gChickens[i].flags & 0x20)) {
            
            setSpecialDialogueBit(0x85);
            gChickens[i].flags = 0;

            handleAddShipment(EGG_VALUE);

            gTotalEggsShipped += adjustValue2(gTotalEggsShipped, 1, MAX_ANIMAL_ITEM_SHIPPED);
            
        }
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/gameStatus", func_80063F3C);

void func_80063F3C(void) {

    u8 i, j;
    u8 temp;
    
    if (gSeason != WINTER) {

        for (i = 0; i < FIELD_HEIGHT; i++) {
            for (j = 0; j < FIELD_WIDTH; j++) {
                temp = farmFieldTiles[i][j];
                if (farmFieldTiles[i][j] && getGroundObjectToolInteractionFlags(temp) & 0x10 && !(getRandomNumberInRange(0, 3))) {
                    setSpecialDialogueBit(0x89);
                    temp++;
                    setFieldTile(FARM, temp, j, i);
                }
            }
        }   
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/gameStatus", func_80064048);

void func_80064048(void) {

    u8 i;

    for (i = 0; i < MAX_CHICKENS; i++) {
        if ((gChickens[i].flags & 1) && gChickens[i].type == 2 && gChickens[i].location == COOP) {
            setSpecialDialogueBit(0x86);
            gChickens[i].flags |= 0x10;
        }
    }
    
}

// FIXME: something wrong here
static inline int adjustValue3(int initial, int value, int max) {

    int temp;
    int temp2;
    int adjusted;

    adjusted = value;
    temp = (initial + adjusted);
    temp -= 5;
    
    if (max < temp) {
        adjusted -= temp - max;
        temp = max;
    }
    
    if (temp < 0) {
        adjusted -= temp;
    } 
      
    return adjusted;
    
}

static inline int adjustValue4(int initial, int value, int max) {

    int temp;
    int temp2;
    int adjusted;

    adjusted = value;
    temp = (initial + adjusted);
    temp -= 10;
    
    if (max < temp) {
        adjusted -= temp - max;
        temp = max;
    }
    
    if (temp < 0) {
        adjusted -= temp;
    } 
      
    return adjusted;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/gameStatus", func_80064114);

// rain
void func_80064114(void) {

    u8 i;

    randomlyBreakLogPieces(16);

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {

        if ((gFarmAnimals[i].flags & 1) && getLevelFlags(gFarmAnimals[i].location) & 2) {

            gFarmAnimals[i].flags |= 0x4000;
            
            switch (gFarmAnimals[i].type) {

                case 0:
                    func_80086458(i, -20);
                    break;
                
                case 1:
                    func_80086458(i, -20);
                    break;
                
                case 2:

                    switch (gFarmAnimals[i].condition) {

                        case 0:
                        case 1:
                        case 2:

                            if (!getRandomNumberInRange(0, 3)) {
                                
                                func_800861A4(2, i, 0xFF, 2, 0);
                                func_80086458(i, -20);

                                // FIXME: should just be += adjustValue(gHappiness, -5, MAX_HAPPINESS)
                                gHappiness += adjustValue3(gHappiness + 5, -5, MAX_HAPPINESS);
                                
                            }
                            
                            if (!getRandomNumberInRange(0, 7)) {
                                
                                func_800861A4(2, i, 0xFF, 3, 0);
                                func_80086458(i, -30);
                        
                                gHappiness += adjustValue4(gHappiness + 10, -10, MAX_HAPPINESS);
                                
                            } 
                            
                            break;
                                                
                        case 3:
                            func_80086458(i, -20);
                            break;

                        default:
                            break;
                        
                    }
                    
                    break;
                
                case 3:
                    func_80086458(i, -20);
                    break;
                
                case 4:
                    func_80086458(i, -20);
                    break;
                
                case 5:

                    switch (gFarmAnimals[i].condition) {

                        case 0:
                            
                            if (!getRandomNumberInRange(0, 7)) {
                            
                                func_800861A4(2, i, 0xFF, 3, 0);
                                func_80086458(i, -30);
                                
                                gHappiness += adjustValue4(gHappiness + 10, -10, MAX_HAPPINESS);
                                
                            }
                            
                            break;

                        case 3:
                            func_80086458(i, -20);
                            break;
                            
                    }

                    break;
                
                case 6:

                    switch (gFarmAnimals[i].condition) {

                        case 0:
                            
                            if (!getRandomNumberInRange(0, 7)) {
                                    
                                func_800861A4(2, i, 0xFF, 3, 0);
                                func_80086458(i, -30);
                                
                                gHappiness += adjustValue4(gHappiness + 10, -10, MAX_HAPPINESS);
                                
                            } 
                            
                            break;

                        case 3:
                            func_80086458(i, -20);
                            break;
                        
                    }


                    break;
                
            }
            
        }
        
    }

    for (i = 0; i < MAX_CHICKENS; i++) {

        if ((gChickens[i].flags & 1) && getLevelFlags(gChickens[i].location) & 2) {

            gChickens[i].flags |= 0x80;

            switch (gChickens[i].type) {

                case 1:
                    break;
                case 2:
                    if (gChickens[i].condition == 0) {
                        func_800861A4(1, i, 0xFF, 1, 0);
                    }
                case 0:
                    break;
                
            } 

        }
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/gameStatus", func_800644B0);

// snow
void func_800644B0(void) {
    
    u8 i;

    randomlyBreakLogPieces(8);

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {

        if ((gFarmAnimals[i].flags & 1) && getLevelFlags(gFarmAnimals[i].location) & 2) {

            switch (gFarmAnimals[i].type) {

                case 0:
                    func_80086458(i, -20);
                    break;
                
                case 1:
                    func_80086458(i, -20);
                    break;
                
                case 2:

                    switch (gFarmAnimals[i].condition) {

                        case 0:
                        case 1:
                        case 2:

                            if (!getRandomNumberInRange(0, 1)) {
                                
                                func_800861A4(2, i, 0xFF, 2, 0);
                                func_80086458(i, -20);

                                gHappiness += adjustValue3(gHappiness + 5, -5, MAX_HAPPINESS);
                                
                            }
                            
                            if (!getRandomNumberInRange(0, 3)) {
                                
                                func_800861A4(2, i, 0xFF, 3, 0);
                                func_80086458(i, -30);
                        
                                gHappiness += adjustValue4(gHappiness + 10, -10, MAX_HAPPINESS);
                                
                            } 
                            
                            break;
                                                
                        case 3:
                            func_80086458(i, -20);
                            break;

                        default:
                            break;
                        
                    }
                    
                    break;
                
                case 3:
                    func_80086458(i, -20);
                    break;
                
                case 4:
                    func_80086458(i, -20);
                    break;
                
                case 5:

                    switch (gFarmAnimals[i].condition) {

                        case 0:
                            
                            if (!getRandomNumberInRange(0, 3)) {
                            
                                func_800861A4(2, i, 0xFF, 3, 0);
                                func_80086458(i, -30);
                                
                                gHappiness += adjustValue4(gHappiness + 10, -10, MAX_HAPPINESS);
                                
                            }
                            
                            break;

                        case 3:
                            func_80086458(i, -20);
                            break;
                            
                    }

                    break;
                
                case 6:

                    switch (gFarmAnimals[i].condition) {

                        case 0:
                            
                            if (!getRandomNumberInRange(0, 3)) {
                                    
                                func_800861A4(2, i, 0xFF, 3, 0);
                                func_80086458(i, -30);
                                
                                gHappiness += adjustValue4(gHappiness + 10, -10, MAX_HAPPINESS);
                                
                            } 
                            
                            break;

                        case 3:
                            func_80086458(i, -20);
                            break;
                        
                    }


                    break;
                
            }
            
        }
        
    }

    for (i = 0; i < MAX_CHICKENS; i++) {

        if ((gChickens[i].flags & 1) && getLevelFlags(gChickens[i].location) & 2) {

            switch (gChickens[i].type) {

                case 1:
                    break;
                case 2:
                    if (gChickens[i].condition == 0) {
                        func_800861A4(1, i, 0xFF, 1, 0);
                    }
                case 0:
                    break;
                
            } 

        }
        
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/gameStatus", func_80064814);

// typhoon
void func_80064814(void) {

    u8 i;

    randomlyBreakLogPieces(8);
    func_800DBC9C(4);
    func_800DBD88(8);

    if (checkLifeEventBit(HAVE_GREENHOUSE)) {

        if (!getRandomNumberInRange(0, 3)) {

            memcpy((u32)&greenhouseFieldTiles, (u32)D_80113760, 0x1E0);

            setLifeEventBit(0xD7);
            toggleLifeEventBit(HAVE_GREENHOUSE);
            setAnimalLocations(GREENHOUSE);
            
        }
        
    }

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {

        if ((gFarmAnimals[i].flags & 1) && getLevelFlags(gFarmAnimals[i].location) & 2) {

            switch (gFarmAnimals[i].type) {

                case 0:
                    func_80086458(i, -30);
                    break;
                
                case 1:
                    func_80086458(i, -30);
                    break;
                
                case 2:

                    switch (gFarmAnimals[i].condition) {

                        case 0:
                        case 1:
                        case 2:

                            if (!getRandomNumberInRange(0, 1)) {
                                
                                func_800861A4(2, i, 0xFF, 2, 0);
                                func_80086458(i, -20);

                                gHappiness += adjustValue3(gHappiness + 5, -5, MAX_HAPPINESS);
                                
                            }
                            
                            if (!getRandomNumberInRange(0, 1)) {
                                
                                func_800861A4(2, i, 0xFF, 3, 0);
                                func_80086458(i, -30);
                        
                                gHappiness += adjustValue4(gHappiness + 10, -10, MAX_HAPPINESS);
                                
                            } 
                            
                            break;
                                                
                        case 3:
                            func_80086458(i, -30);
                            break;

                        default:
                            break;
                        
                    }
                    
                    break;
                
                case 3:
                    func_80086458(i, -30);
                    break;
                
                case 4:
                    func_80086458(i, -30);
                    break;
                
                case 5:

                    switch (gFarmAnimals[i].condition) {

                        case 0:
                            
                            if (!getRandomNumberInRange(0, 1)) {
                            
                                func_800861A4(2, i, 0xFF, 3, 0);
                                func_80086458(i, -30);
                                
                                gHappiness += adjustValue4(gHappiness + 10, -10, MAX_HAPPINESS);
                                
                            }
                            
                            break;

                        case 3:
                            func_80086458(i, -30);
                            break;
                            
                    }

                    break;
                
                case 6:

                    switch (gFarmAnimals[i].condition) {

                        case 0:
                            
                            if (!getRandomNumberInRange(0, 1)) {
                                    
                                func_800861A4(2, i, 0xFF, 3, 0);
                                func_80086458(i, -30);
                                
                                gHappiness += adjustValue4(gHappiness + 10, -10, MAX_HAPPINESS);
                                
                            } 
                            
                            break;

                        case 3:
                            func_80086458(i, -30);
                            break;
                        
                    }


                    break;
                
            }
            
        }
        
    }

    for (i = 0; i < MAX_CHICKENS; i++) {

        if ((gChickens[i].flags & 1) && getLevelFlags(gChickens[i].location) & 2) {

            switch (gChickens[i].type) {

                case 1:

                    func_800861A4(1, i, 0xFF, 2, 0xFF);

                    gChickens[i].flags = 0;

                    setLifeEventBit(0x2B);
                    
                    D_8018985C[0] = gChickens[i].name[0];
                    D_8018985C[1] = gChickens[i].name[1];
                    D_8018985C[2] = gChickens[i].name[2];
                    D_8018985C[3] = gChickens[i].name[3];
                    D_8018985C[4] = gChickens[i].name[4];
                    D_8018985C[5] = gChickens[i].name[5];

                    break;

                case 2:

                    switch (gChickens[i].condition) {

                        case 0:
                        case 1:
                        
                            func_800861A4(1, i, 0xFF, 2, 0xFF);
        
                            gChickens[i].flags = 0;
        
                            setLifeEventBit(0x2B);
                            
                            D_8018985C[0] = gChickens[i].name[0];
                            D_8018985C[1] = gChickens[i].name[1];
                            D_8018985C[2] = gChickens[i].name[2];
                            D_8018985C[3] = gChickens[i].name[3];
                            D_8018985C[4] = gChickens[i].name[4];
                            D_8018985C[5] = gChickens[i].name[5];
        
                            break;

                    }

                case 0:
                    break;
                
            }

        }
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/gameStatus", func_80064CF0);

void func_80064CF0(void) {
    
    u8 i;

    randomlyBreakLogPieces(16);
    func_800DBE8C(8);

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {

        if ((gFarmAnimals[i].flags & 1) && getLevelFlags(gFarmAnimals[i].location) & 2) {

            switch (gFarmAnimals[i].type) {

                case 0:
                    func_80086458(i, -10);
                    break;
                
                case 1:
                    func_80086458(i, -10);
                    break;
                
                case 2:

                    switch (gFarmAnimals[i].condition) {

                        case 0:
                        case 1:
                        case 2:

                            if (!getRandomNumberInRange(0, 3)) {
                                
                                func_800861A4(2, i, 0xFF, 2, 0);
                                func_80086458(i, -20);

                                gHappiness += adjustValue3(gHappiness + 5, -5, MAX_HAPPINESS);
                                
                            }
                            
                            if (!getRandomNumberInRange(0, 7)) {
                                
                                func_800861A4(2, i, 0xFF, 3, 0);
                                func_80086458(i, -30);
                        
                                gHappiness += adjustValue4(gHappiness + 10, -10, MAX_HAPPINESS);
                                
                            } 
                            
                            break;
                                                
                        case 3:
                            func_80086458(i, -10);
                            break;

                        default:
                            break;
                        
                    }
                    
                    break;
                
                case 3:
                    func_80086458(i, -10);
                    break;
                
                case 4:
                    func_80086458(i, -10);
                    break;
                
                case 5:

                    switch (gFarmAnimals[i].condition) {

                        case 0:
                            
                            if (!getRandomNumberInRange(0, 7)) {
                            
                                func_800861A4(2, i, 0xFF, 3, 0);
                                func_80086458(i, -30);
                                
                                gHappiness += adjustValue4(gHappiness + 10, -10, MAX_HAPPINESS);
                                
                            }
                            
                            break;

                        case 3:
                            func_80086458(i, -10);
                            break;
                            
                    }

                    break;
                
                case 6:

                    switch (gFarmAnimals[i].condition) {

                        case 0:
                            
                            if (!getRandomNumberInRange(0, 7)) {
                                    
                                func_800861A4(2, i, 0xFF, 3, 0);
                                func_80086458(i, -30);
                                
                                gHappiness += adjustValue4(gHappiness + 10, -10, MAX_HAPPINESS);
                                
                            } 
                            
                            break;

                        case 3:
                            func_80086458(i, -10);
                            break;
                        
                    }


                    break;
                
            }
            
        }
        
    }

    for (i = 0; i < MAX_CHICKENS; i++) {

        if ((gChickens[i].flags & 1) && getLevelFlags(gChickens[i].location) & 2) {

            switch (gChickens[i].type) {

                case 1:
                    break;
                case 2:
                    if (gChickens[i].condition == 0 && !getRandomNumberInRange(0, 3)) {
                        func_800861A4(1, i, 0xFF, 1, 0);
                    }
                case 0:
                    break;
                
            } 

        }
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/gameStatus", setDailyEventBit);

void setDailyEventBit(u16 bitIndex) {
    u32 temp = bitIndex;
    dailyEventBits[temp / 32] |= 1 << (temp & 0x1F);
}

//INCLUDE_ASM("asm/nonmatchings/game/gameStatus", toggleDailyEventBit);

void toggleDailyEventBit(u16 bitIndex) {
    u32 temp = bitIndex;
    dailyEventBits[temp / 32] &= ~(1 << (temp & 0x1F));
}

//INCLUDE_ASM("asm/nonmatchings/game/gameStatus", checkDailyEventBit);

u32 checkDailyEventBit(u16 bitIndex) {
    u32 temp = bitIndex;
    return dailyEventBits[temp / 32] & (1 << (temp & 0x1F));
}

//INCLUDE_ASM("asm/nonmatchings/game/gameStatus", setLifeEventBit);

void setLifeEventBit(u16 bitIndex) {
    u32 temp = bitIndex;
    lifeEventBits[temp / 32] |= 1 << (temp & 0x1F);
}

//INCLUDE_ASM("asm/nonmatchings/game/gameStatus", toggleLifeEventBit);

void toggleLifeEventBit(u16 bitIndex) {
    u32 temp = bitIndex;
    lifeEventBits[temp / 32] &= ~(1 << (temp & 0x1F));
}

//INCLUDE_ASM("asm/nonmatchings/game/gameStatus", checkLifeEventBit);

u32 checkLifeEventBit(u16 bitIndex) {
    u32 temp = bitIndex;
    return lifeEventBits[temp / 32] & (1 << (temp & 0x1F));
}

//INCLUDE_ASM("asm/nonmatchings/game/gameStatus", setSpecialDialogueBit);

void setSpecialDialogueBit(u16 bitIndex) {
    u32 temp = bitIndex;
    specialDialogueBits[temp / 32] |= 1 << (temp & 0x1F);
}

//INCLUDE_ASM("asm/nonmatchings/game/gameStatus", toggleSpecialDialogueBit);

void toggleSpecialDialogueBit(u16 bitIndex) {
    u32 temp = bitIndex;
    specialDialogueBits[temp / 32] &= ~(1 << (temp & 0x1F));
}

//INCLUDE_ASM("asm/nonmatchings/game/gameStatus", checkSpecialDialogueBit);

u32 checkSpecialDialogueBit(u16 bitIndex) {
    u32 temp = bitIndex;
    return specialDialogueBits[temp / 32] & (1 << (temp & 0x1F));
}

//INCLUDE_ASM("asm/nonmatchings/game/gameStatus", func_8006523C);

void func_8006523C(u16 bitIndex) {
    u32 temp = bitIndex;
    readMailBits[temp / 32] |= (1 << (temp & 0x1F));
}

//INCLUDE_ASM("asm/nonmatchings/game/gameStatus", toggleReadLetterBit);

void toggleReadLetterBit(u16 bitIndex) {
    u32 temp = bitIndex;
    return readMailBits[temp / 32] &= ~(1 << (temp & 0x1F));
}

//INCLUDE_ASM("asm/nonmatchings/game/gameStatus", checkMailRead);

u32 checkMailRead(u16 bitIndex) {
    u32 temp = bitIndex;
    return readMailBits[temp / 32] & (1 << (temp & 0x1F));
}

//INCLUDE_ASM(const s32, "gameStatus", setMail);

void setMail(u16 bitIndex) {
    u32 temp = bitIndex;
    return mailboxBits[temp / 32] |= (1 << (temp & 0x1F));
}

//INCLUDE_ASM("asm/nonmatchings/game/gameStatus", func_80065308);

void func_80065308(u16 bitIndex) {
    u32 temp = bitIndex;
    mailboxBits[temp / 32] &= ~(1 << (temp & 0x1F));
}

//INCLUDE_ASM("asm/nonmatchings/game/gameStatus", func_80065340);

inline u32 func_80065340(u16 bitIndex) {
    u32 temp = bitIndex;
    return mailboxBits[temp / 32] & (1 << (temp & 0x1F));
}

static inline void toggleLetterBit(u32 i, u32 mailBox) {
    mailboxBits[i / 32] &= ~mailBox;
}

static inline void setReadMail(u32 i, u32 mailBox) {
    readMailBits[i / 32] |= mailBox;
}

static inline u32 getLetterBit(u32 mailBox) {
    return (1 << (mailBox & 0x1F));
}

//INCLUDE_ASM("asm/nonmatchings/game/gameStatus", func_8006536C);

// get first unread leter index
u8 func_8006536C(void) {
    
    u8 result = 0xFF;
    u8 i;
    
    u32 mailBox;
    u32 letterBit;
    
    for (i = 0; i < MAX_LETTERS && result == 0xFF; i++) {

        mailBox = mailboxBits[i / 32];
        letterBit = getLetterBit(i);
        
        if (mailBox & letterBit) {

            toggleLetterBit(i, letterBit);
            setReadMail(i, letterBit);
            
            result = i;
            
        }
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/gameStatus", setAlbumPicture);

void setAlbumPicture(u8 pictureBitIndex) {
    
    switch (pictureBitIndex) {
        case 0x24:
        case 0x25:
        case 0x26:
        case 0x27:
        case 0x28:
            albumBits |= 0x80;
            break;
        case 0x29:
        case 0x2A:
        case 0x2B:
        case 0x2C:
        case 0x2D:
            albumBits |= 0x100;
            break;
        case 0x2E:
            albumBits |= 2;
            break;
        case 0x2F:
            albumBits |= 4;
            break;
        case 0x30:
            albumBits |= 0x10;
            break;
        case 0x31:
            albumBits |= 0x40;
            break;
        case 0x32:
            albumBits |= 0x20;
            break;
        case 0x33:
            albumBits |= 0x4000;
            break;
        case 0x34:
            albumBits |= 8;
            break;
        case 0x35:
            albumBits |= 0x200;
            break;
        case 0x36:
            albumBits |= 0x400;
            break;
        case 0x37:
            albumBits |= 0x800;
            break;
        case 0x38:
            albumBits |= 0x1000;
            break;
        case 0x39:
            albumBits |= 0x2000;
            break;
        }
}

//INCLUDE_ASM("asm/nonmatchings/game/gameStatus", func_80065518);

// get sum of recipes
u8 func_80065518(void) {
    
    u8 result = 0;

    result = checkRecipe(0) != 0;
    
    if (checkRecipe(1)) {
        result++;
    }
    
    if (checkRecipe(2)) {
        result++;
    }

    if (checkRecipe(3)) {
        result++;
    }

    if (checkRecipe(4)) {
        result++;
    }

    if (checkRecipe(5)) {
        result++;
    }

    if (checkRecipe(6)) {
        result++;
    }
    
    if (checkRecipe(7)) {
        result++;
    }

    if (checkRecipe(8)) {
        result++;
    }

    if (checkRecipe(9)) {
        result++;
    }

    if (checkRecipe(0xA)) {
        result++;
    }

    if (checkRecipe(0xB)) {
        result++;
    }

    if (checkRecipe(0xC)) {
        result++;
    }

    if (checkRecipe(0xD)) {
        result++;
    }

    if (checkRecipe(0xE)) {
        result++;
    }

    if (checkRecipe(0xF)) {
        result++;
    }

    if (checkRecipe(0x10)) {
        result++;
    }

    if (checkRecipe(0x11)) {
        result++;
    }

    if (checkRecipe(0x12)) {
        result++;
    }

    if (checkRecipe(0x13)) {
        result++;
    }

    if (checkRecipe(0x14)) {
        result++;
    }

    if (checkRecipe(0x15)) {
        result++;
    }
    if (checkRecipe(0x16)) {
        result++;
    }

    if (checkRecipe(0x17)) {
        result++;
    }

    if (checkRecipe(0x18)) {
        result++;
    }

    if (checkRecipe(0x19)) {
        result++;
    }

    if (checkRecipe(0x1A)) {
        result++;
    }

    if (checkRecipe(0x1B)) {
        result++;
    }
    if (checkRecipe(0x1C)) {
        result++;
    }

    if (checkRecipe(0x1D)) {
        result++;
    }

    if (checkRecipe(0x1E)) {
        result++;
    }

    if (checkRecipe(0x1F)) {
        result++;
    }

    if (checkRecipe(0x20)) {
        result++;
    }

    if (checkRecipe(0x21)) {
        result++;
    }

    if (checkRecipe(0x22)) {
        result++;
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/gameStatus", getSumNpcAffection);

s32 getSumNpcAffection(void) {

    u16 result = 0;
    u8 i;

    for (i = 0; i < MAX_NPCS; i++) {
        result += npcAffection[i];
    }

    return result;
    
}

// empty function
void func_800657AC(void) {}

// empty function
void func_800657B4(void) {}

// empty function
void func_800657BC(void) {}

// empty function
void func_800657C4(void) {}
