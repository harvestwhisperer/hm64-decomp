#include "common.h"

#include "game/gameStatus.h"
#include "system/math.h"

#include "game/animals.h"
#include "game/game.h"
#include "game/level.h"
#include "game/fieldObjects.h"
#include "game/npc.h"
#include "game/overlayScreens.h"
#include "game/player.h"
#include "game/time.h"

#include "assetIndices/maps.h"

#include "data/fieldTileMaps/fieldTiles.h"

// bss
u32 dailyEventBits[32];
u32 lifeEventBits[32];
u32 specialDialogueBits[16];
u32 readMailBits[3];
u32 mailboxBits[3];
u32 recipesBits[1];
u32 albumBits;
u32 acquiredPowerNutBits;

// forward declaration
void toggleSpecialDialogueBit(u16);
void handleWifeHelpsWeeding(void);
void handleWifeCratesEggs(void);
void handleWifeHelpsWatering(void);
void handleWifeFeedsChickens(void);

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
                    handleWifeHelpsWeeding();
                } else {
                    handleWifeCratesEggs();
                }
            } 
            break;

        case POPURI:

            if (!getRandomNumberInRange(0,  (5 - npcAffection[POPURI] / 50))) {
                if (temp) {
                    handleWifeHelpsWatering();
                } else {
                    handleWifeCratesEggs();
                }
            } 
            break;

        case ELLI:

            if (!getRandomNumberInRange(0,  (5 - npcAffection[ELLI] / 50))) {
                if (temp) {
                    handleWifeFeedsChickens();
                } else {
                    handleWifeCratesEggs();
                }
            } 
            break;       

        case ANN:

            if (!getRandomNumberInRange(0,  (5 - npcAffection[ANN] / 50))) {
                if (temp) {
                    handleWifeFeedsChickens();
                    break;
                } else {
                    handleWifeCratesEggs();
                }
            } 
            break;

        case KAREN:

            if (!getRandomNumberInRange(0,  (5 - npcAffection[KAREN] / 50))) {
                if (temp) {
                    handleWifeCratesEggs();
                }
            } 
            break;

        default:
            break;
    }
}


//INCLUDE_ASM("asm/nonmatchings/game/gameStatus", handleWifeHelpsWeeding);

void handleWifeHelpsWeeding(void) {
    
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
 
//INCLUDE_ASM("asm/nonmatchings/game/gameStatus", handleWifeCratesEggs);

void handleWifeCratesEggs(void) {

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

//INCLUDE_ASM("asm/nonmatchings/game/gameStatus", handleWifeHelpsWatering);

void handleWifeHelpsWatering(void) {

    u8 i, j;
    u8 temp;
    
    if (gSeason != WINTER) {

        for (i = 0; i < FIELD_HEIGHT; i++) {
            for (j = 0; j < FIELD_WIDTH; j++) {
                temp = farmFieldTiles[i][j];
                if (farmFieldTiles[i][j] && (getGroundObjectToolInteractionFlags(temp) & GROUND_OBJECT_WATERABLE) && !(getRandomNumberInRange(0, 3))) {
                    setSpecialDialogueBit(0x89);
                    temp++;
                    setFieldTile(FARM, temp, j, i);
                }
            }
        }   

    }

}

//INCLUDE_ASM("asm/nonmatchings/game/gameStatus", handleWifeFeedsChickens);

void handleWifeFeedsChickens(void) {

    u8 i;

    for (i = 0; i < MAX_CHICKENS; i++) {
        if ((gChickens[i].flags & 1) && gChickens[i].type == 2 && gChickens[i].location == COOP) {
            setSpecialDialogueBit(0x86);
            gChickens[i].flags |= CHICKEN_FED;
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

//INCLUDE_ASM("asm/nonmatchings/game/gameStatus", updateFarmStatusRain);

void updateFarmStatusRain(void) {

    u8 i;

    randomlyBreakLogPieces(16);

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {

        if ((gFarmAnimals[i].flags & FARM_ANIMAL_ACTIVE) && (getLevelFlags(gFarmAnimals[i].location) & LEVEL_OUTDOORS)) {

            gFarmAnimals[i].flags |= 0x4000;
            
            switch (gFarmAnimals[i].type) {

                case BABY_COW:
                    adjustFarmAnimalAffection(i, -20);
                    break;
                
                case CALF:
                    adjustFarmAnimalAffection(i, -20);
                    break;
                
                case ADULT_COW:

                    switch (gFarmAnimals[i].condition) {

                        case COW_NORMAL:
                        case COW_HAPPY:
                        case COW_MAD:

                            if (!getRandomNumberInRange(0, 3)) {
                                
                                setAnimalState(2, i, 0xFF, COW_MAD, 0);
                                adjustFarmAnimalAffection(i, -20);

                                // FIXME: should just be += adjustValue(gHappiness, -5, MAX_HAPPINESS)
                                gHappiness += adjustValue3(gHappiness + 5, -5, MAX_HAPPINESS);
                                
                            }
                            
                            if (!getRandomNumberInRange(0, 7)) {
                                
                                setAnimalState(2, i, 0xFF, COW_SICK, 0);
                                adjustFarmAnimalAffection(i, -30);
                        
                                gHappiness += adjustValue4(gHappiness + 10, -10, MAX_HAPPINESS);
                                
                            } 
                            
                            break;
                                                
                        case COW_SICK:
                            adjustFarmAnimalAffection(i, -20);
                            break;

                        default:
                            break;
                        
                    }
                    
                    break;
                
                case PREGNANT_COW:
                    adjustFarmAnimalAffection(i, -20);
                    break;
                
                case BABY_SHEEP:
                    adjustFarmAnimalAffection(i, -20);
                    break;
                
                case ADULT_SHEEP:

                    switch (gFarmAnimals[i].condition) {

                        case SHEEP_NORMAL:
                            
                            if (!getRandomNumberInRange(0, 7)) {
                            
                                setAnimalState(2, i, 0xFF, SHEEP_SICK, 0);
                                adjustFarmAnimalAffection(i, -30);
                                
                                gHappiness += adjustValue4(gHappiness + 10, -10, MAX_HAPPINESS);
                                
                            }
                            
                            break;

                        case SHEEP_SICK:
                            adjustFarmAnimalAffection(i, -20);
                            break;
                            
                    }

                    break;
                
                case SHEARED_SHEEP:

                    switch (gFarmAnimals[i].condition) {

                        case SHEEP_NORMAL:
                            
                            if (!getRandomNumberInRange(0, 7)) {
                                    
                                setAnimalState(2, i, 0xFF, SHEEP_SICK, 0);
                                adjustFarmAnimalAffection(i, -30);
                                
                                gHappiness += adjustValue4(gHappiness + 10, -10, MAX_HAPPINESS);
                                
                            } 
                            
                            break;

                        case SHEEP_SICK:
                            adjustFarmAnimalAffection(i, -20);
                            break;
                        
                    }


                    break;
                
            }
            
        }
        
    }

    for (i = 0; i < MAX_CHICKENS; i++) {

        if ((gChickens[i].flags & 1) && getLevelFlags(gChickens[i].location) & LEVEL_OUTDOORS) {

            gChickens[i].flags |= 0x80;

            switch (gChickens[i].type) {

                case CHICK:
                    break;
                case ADULT_CHICKEN:
                    if (gChickens[i].condition == CHICKEN_NORMAL) {
                        setAnimalState(1, i, 0xFF, CHICKEN_STARVED, 0);
                    }
                case CHICKEN_EGG:
                    break;
                
            } 

        }
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/gameStatus", updateFarmStatusSnow);

// snow
void updateFarmStatusSnow(void) {
    
    u8 i;

    randomlyBreakLogPieces(8);

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {

        if ((gFarmAnimals[i].flags & 1) && getLevelFlags(gFarmAnimals[i].location) & LEVEL_OUTDOORS) {

            switch (gFarmAnimals[i].type) {

                case BABY_COW:
                    adjustFarmAnimalAffection(i, -20);
                    break;
                
                case CALF:
                    adjustFarmAnimalAffection(i, -20);
                    break;
                
                case ADULT_COW:

                    switch (gFarmAnimals[i].condition) {

                        case COW_NORMAL:
                        case COW_HAPPY:
                        case COW_MAD:

                            if (!getRandomNumberInRange(0, 1)) {
                                
                                setAnimalState(2, i, 0xFF, COW_MAD, 0);
                                adjustFarmAnimalAffection(i, -20);

                                gHappiness += adjustValue3(gHappiness + 5, -5, MAX_HAPPINESS);
                                
                            }
                            
                            if (!getRandomNumberInRange(0, 3)) {
                                
                                setAnimalState(2, i, 0xFF, COW_SICK, 0);
                                adjustFarmAnimalAffection(i, -30);
                        
                                gHappiness += adjustValue4(gHappiness + 10, -10, MAX_HAPPINESS);
                                
                            } 
                            
                            break;
                                                
                        case COW_SICK:
                            adjustFarmAnimalAffection(i, -20);
                            break;

                        default:
                            break;
                        
                    }
                    
                    break;
                
                case PREGNANT_COW:
                    adjustFarmAnimalAffection(i, -20);
                    break;
                
                case BABY_SHEEP:
                    adjustFarmAnimalAffection(i, -20);
                    break;
                
                case ADULT_SHEEP:

                    switch (gFarmAnimals[i].condition) {

                        case SHEEP_NORMAL:
                            
                            if (!getRandomNumberInRange(0, 3)) {
                            
                                setAnimalState(2, i, 0xFF, SHEEP_SICK, 0);
                                adjustFarmAnimalAffection(i, -30);
                                
                                gHappiness += adjustValue4(gHappiness + 10, -10, MAX_HAPPINESS);
                                
                            }
                            
                            break;

                        case SHEEP_SICK:
                            adjustFarmAnimalAffection(i, -20);
                            break;
                            
                    }

                    break;
                
                case SHEARED_SHEEP:

                    switch (gFarmAnimals[i].condition) {

                        case SHEEP_NORMAL:
                            
                            if (!getRandomNumberInRange(0, 3)) {
                                    
                                setAnimalState(2, i, 0xFF, SHEEP_SICK, 0);
                                adjustFarmAnimalAffection(i, -30);
                                
                                gHappiness += adjustValue4(gHappiness + 10, -10, MAX_HAPPINESS);
                                
                            } 
                            
                            break;

                        case SHEEP_SICK:
                            adjustFarmAnimalAffection(i, -20);
                            break;
                        
                    }


                    break;
                
            }
            
        }
        
    }

    for (i = 0; i < MAX_CHICKENS; i++) {

        if ((gChickens[i].flags & 1) && getLevelFlags(gChickens[i].location) & LEVEL_OUTDOORS) {

            switch (gChickens[i].type) {

                case CHICK:
                    break;
                case ADULT_CHICKEN:
                    if (gChickens[i].condition == CHICKEN_NORMAL) {
                        setAnimalState(1, i, 0xFF, CHICKEN_STARVED, 0);
                    }
                case CHICKEN_EGG:
                    break;
                
            } 

        }
        
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/gameStatus", updateFarmStatusTyphoon);

void updateFarmStatusTyphoon(void) {

    u8 i;

    randomlyBreakLogPieces(8);
    randomlyCutGrass(4);
    randomlyDestroyCrops(8);

    if (checkLifeEventBit(HAVE_GREENHOUSE)) {

        if (!getRandomNumberInRange(0, 3)) {

            memcpy(greenhouseFieldTiles, D_80113760, FIELD_HEIGHT * FIELD_WIDTH);

            setLifeEventBit(0xD7);
            toggleLifeEventBit(HAVE_GREENHOUSE);
            setAnimalLocations(GREENHOUSE);
            
        }
        
    }

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {

        if ((gFarmAnimals[i].flags & 1) && getLevelFlags(gFarmAnimals[i].location) & LEVEL_OUTDOORS) {

            switch (gFarmAnimals[i].type) {

                case BABY_COW:
                    adjustFarmAnimalAffection(i, -30);
                    break;
                
                case CALF:
                    adjustFarmAnimalAffection(i, -30);
                    break;
                
                case ADULT_COW:

                    switch (gFarmAnimals[i].condition) {

                        case COW_NORMAL:
                        case COW_HAPPY:
                        case COW_MAD:

                            if (!getRandomNumberInRange(0, 1)) {
                                
                                setAnimalState(2, i, 0xFF, COW_MAD, 0);
                                adjustFarmAnimalAffection(i, -20);

                                gHappiness += adjustValue3(gHappiness + 5, -5, MAX_HAPPINESS);
                                
                            }
                            
                            if (!getRandomNumberInRange(0, 1)) {
                                
                                setAnimalState(2, i, 0xFF, COW_SICK, 0);
                                adjustFarmAnimalAffection(i, -30);
                        
                                gHappiness += adjustValue4(gHappiness + 10, -10, MAX_HAPPINESS);
                                
                            } 
                            
                            break;
                                                
                        case COW_SICK:
                            adjustFarmAnimalAffection(i, -30);
                            break;

                        default:
                            break;
                        
                    }
                    
                    break;
                
                case PREGNANT_COW:
                    adjustFarmAnimalAffection(i, -30);
                    break;
                
                case BABY_SHEEP:
                    adjustFarmAnimalAffection(i, -30);
                    break;
                
                case ADULT_SHEEP:

                    switch (gFarmAnimals[i].condition) {

                        case SHEEP_NORMAL:
                            
                            if (!getRandomNumberInRange(0, 1)) {
                            
                                setAnimalState(2, i, 0xFF, SHEEP_SICK, 0);
                                adjustFarmAnimalAffection(i, -30);
                                
                                gHappiness += adjustValue4(gHappiness + 10, -10, MAX_HAPPINESS);
                                
                            }
                            
                            break;

                        case SHEEP_SICK:
                            adjustFarmAnimalAffection(i, -30);
                            break;
                            
                    }

                    break;
                
                case SHEARED_SHEEP:

                    switch (gFarmAnimals[i].condition) {

                        case SHEEP_NORMAL:
                            
                            if (!getRandomNumberInRange(0, 1)) {
                                    
                                setAnimalState(2, i, 0xFF, SHEEP_SICK, 0);
                                adjustFarmAnimalAffection(i, -30);
                                
                                gHappiness += adjustValue4(gHappiness + 10, -10, MAX_HAPPINESS);
                                
                            } 
                            
                            break;

                        case SHEEP_SICK:
                            adjustFarmAnimalAffection(i, -30);
                            break;
                        
                    }


                    break;
                
            }
            
        }
        
    }

    for (i = 0; i < MAX_CHICKENS; i++) {

        if ((gChickens[i].flags & 1) && getLevelFlags(gChickens[i].location) & LEVEL_OUTDOORS) {

            switch (gChickens[i].type) {

                case CHICK:

                    setAnimalState(1, i, 0xFF, CHICKEN_DEAD, 0xFF);

                    gChickens[i].flags = 0;

                    setLifeEventBit(ANIMAL_DIED);
                    
                    gDeadAnimalName[0] = gChickens[i].name[0];
                    gDeadAnimalName[1] = gChickens[i].name[1];
                    gDeadAnimalName[2] = gChickens[i].name[2];
                    gDeadAnimalName[3] = gChickens[i].name[3];
                    gDeadAnimalName[4] = gChickens[i].name[4];
                    gDeadAnimalName[5] = gChickens[i].name[5];

                    break;

                case ADULT_CHICKEN:

                    switch (gChickens[i].condition) {

                        case CHICKEN_NORMAL:
                        case CHICKEN_STARVED:
                        
                            setAnimalState(1, i, 0xFF, CHICKEN_DEAD, 0xFF);
        
                            gChickens[i].flags = 0;
        
                            setLifeEventBit(ANIMAL_DIED);
                            
                            gDeadAnimalName[0] = gChickens[i].name[0];
                            gDeadAnimalName[1] = gChickens[i].name[1];
                            gDeadAnimalName[2] = gChickens[i].name[2];
                            gDeadAnimalName[3] = gChickens[i].name[3];
                            gDeadAnimalName[4] = gChickens[i].name[4];
                            gDeadAnimalName[5] = gChickens[i].name[5];
        
                            break;

                    }

                case CHICKEN_EGG:
                    break;
                
            }

        }
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/gameStatus", func_80064CF0);

// earthquake
void func_80064CF0(void) {
    
    u8 i;

    randomlyBreakLogPieces(16);
    randomlyResetGreenhouseTiles(8);

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {

        if ((gFarmAnimals[i].flags & 1) && getLevelFlags(gFarmAnimals[i].location) & LEVEL_OUTDOORS) {

            switch (gFarmAnimals[i].type) {

                case BABY_COW:
                    adjustFarmAnimalAffection(i, -10);
                    break;
                
                case CALF:
                    adjustFarmAnimalAffection(i, -10);
                    break;
                
                case ADULT_COW:

                    switch (gFarmAnimals[i].condition) {

                        case COW_NORMAL:
                        case COW_HAPPY:
                        case COW_MAD:

                            if (!getRandomNumberInRange(0, 3)) {
                                
                                setAnimalState(2, i, 0xFF, COW_MAD, 0);
                                adjustFarmAnimalAffection(i, -20);

                                gHappiness += adjustValue3(gHappiness + 5, -5, MAX_HAPPINESS);
                                
                            }
                            
                            if (!getRandomNumberInRange(0, 7)) {
                                
                                setAnimalState(2, i, 0xFF, COW_SICK, 0);
                                adjustFarmAnimalAffection(i, -30);
                        
                                gHappiness += adjustValue4(gHappiness + 10, -10, MAX_HAPPINESS);
                                
                            } 
                            
                            break;
                                                
                        case COW_SICK:
                            adjustFarmAnimalAffection(i, -10);
                            break;

                        default:
                            break;
                        
                    }
                    
                    break;
                
                case PREGNANT_COW:
                    adjustFarmAnimalAffection(i, -10);
                    break;
                
                case BABY_SHEEP:
                    adjustFarmAnimalAffection(i, -10);
                    break;
                
                case ADULT_SHEEP:

                    switch (gFarmAnimals[i].condition) {

                        case SHEEP_NORMAL:
                            
                            if (!getRandomNumberInRange(0, 7)) {
                            
                                setAnimalState(2, i, 0xFF, SHEEP_SICK, 0);
                                adjustFarmAnimalAffection(i, -30);
                                
                                gHappiness += adjustValue4(gHappiness + 10, -10, MAX_HAPPINESS);
                                
                            }
                            
                            break;

                        case SHEEP_SICK:
                            adjustFarmAnimalAffection(i, -10);
                            break;
                            
                    }

                    break;
                
                case SHEARED_SHEEP:

                    switch (gFarmAnimals[i].condition) {

                        case SHEEP_NORMAL:
                            
                            if (!getRandomNumberInRange(0, 7)) {
                                    
                                setAnimalState(2, i, 0xFF, SHEEP_SICK, 0);
                                adjustFarmAnimalAffection(i, -30);
                                
                                gHappiness += adjustValue4(gHappiness + 10, -10, MAX_HAPPINESS);
                                
                            } 
                            
                            break;

                        case SHEEP_SICK:
                            adjustFarmAnimalAffection(i, -10);
                            break;
                        
                    }


                    break;
                
            }
            
        }
        
    }

    for (i = 0; i < MAX_CHICKENS; i++) {

        if ((gChickens[i].flags & 1) && getLevelFlags(gChickens[i].location) & LEVEL_OUTDOORS) {

            switch (gChickens[i].type) {

                case CHICK:
                    break;
                case ADULT_CHICKEN:
                    if (gChickens[i].condition == CHICKEN_NORMAL && !getRandomNumberInRange(0, 3)) {
                        setAnimalState(1, i, 0xFF, CHICKEN_STARVED, 0);
                    }
                    break;
                case CHICKEN_EGG:
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

//INCLUDE_ASM("asm/nonmatchings/game/gameStatus", addToReadMail);

// unused
void addToReadMail(u16 bitIndex) {
    u32 temp = bitIndex;
    readMailBits[temp / 32] |= (1 << (temp & 0x1F));
}

//INCLUDE_ASM("asm/nonmatchings/game/gameStatus", toggleReadLetterBit);

void toggleReadLetterBit(u16 bitIndex) {
    u32 temp = bitIndex;
    readMailBits[temp / 32] &= ~(1 << (temp & 0x1F));
}

//INCLUDE_ASM("asm/nonmatchings/game/gameStatus", checkMailRead);

u32 checkMailRead(u16 bitIndex) {
    u32 temp = bitIndex;
    return readMailBits[temp / 32] & (1 << (temp & 0x1F));
}

//INCLUDE_ASM(const s32, "gameStatus", setMail);

void setMail(u16 bitIndex) {
    u32 temp = bitIndex;
    mailboxBits[temp / 32] |= (1 << (temp & 0x1F));
}

//INCLUDE_ASM("asm/nonmatchings/game/gameStatus", readLetter);

// unused
void readLetter(u16 bitIndex) {
    u32 temp = bitIndex;
    mailboxBits[temp / 32] &= ~(1 << (temp & 0x1F));
}

//INCLUDE_ASM("asm/nonmatchings/game/gameStatus", checkLetterInMailBox);

// unused
inline u32 checkLetterInMailBox(u16 bitIndex) {
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

//INCLUDE_ASM("asm/nonmatchings/game/gameStatus", readMail);

// get first unread leter index
u8 readMail(void) {
    
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

//INCLUDE_ASM("asm/nonmatchings/game/gameStatus", setAlbumPictureFromLetter);

void setAlbumPictureFromLetter(u8 letterIndex) {
    
    switch (letterIndex) {
        case 36:
        case 37:
        case 38:
        case 39:
        case 40:
            albumBits |= PHOTO_WEDDING;
            break;
        case 41:
        case 42:
        case 43:
        case 44:
        case 45:
            albumBits |= PHOTO_BIRTH;
            break;
        case 46:
            albumBits |= PHOTO_SOWING_FESTIVAL;
            break;
        case 47:
            albumBits |= PHOTO_HORSE_RACE;
            break;
        case 48:
            albumBits |= PHOTO_SEA_FESTIVAL;
            break;
        case 49:
            albumBits |= PHOTO_DOG_RACE;
            break;
        case 50:
            albumBits |= PHOTO_HOT_SPRINGS;
            break;
        case 51:
            albumBits |= PHOTO_HOME_EXTENSIONS;
            break;
        case 52:
            albumBits |= PHOTO_COW_FESTIVAL;
            break;
        case 53:
            albumBits |= PHOTO_MARIA_FIREFLIES;
            break;
        case 54:
            albumBits |= PHOTO_POPURI_BLUE_MIST;
            break;
        case 55:
            albumBits |= PHOTO_ELLI_ESSENCE_OF_MOONDROP;
            break;
        case 56:
            albumBits |= PHOTO_ANN_RABBIT;
            break;
        case 57:
            albumBits |= PHOTO_KAREN_KEIFU_FAIRIES;
            break;
        }

}

//INCLUDE_ASM("asm/nonmatchings/game/gameStatus", getAcquiredRecipesTotal);

u8 getAcquiredRecipesTotal(void) {
    
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

// main loop callback function
// empty function
void func_800657B4(void) {}

// empty function
void func_800657BC(void) {}

// main loop callback function
// empty function
void func_800657C4(void) {}
