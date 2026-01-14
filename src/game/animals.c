#include "common.h"

#include "game/animals.h"

#include "system/controller.h"
#include "system/entity.h"
#include "system/graphic.h"
#include "system/mapController.h"
#include "system/math.h"
#include "system/message.h"

#include "game/fieldObjects.h"
#include "game/game.h"
#include "game/gameAudio.h"
#include "game/gameStatus.h"
#include "game/items.h"
#include "game/level.h"
#include "game/player.h"
#include "game/time.h"
#include "game/weather.h"

#include "mainproc.h"

#include "assetIndices/entities.h"
#include "assetIndices/maps.h"
#include "assetIndices/sfxs.h"
#include "assetIndices/sprites.h"

#include "buffers/buffers.h"

// bss
Chicken gChickens[MAX_CHICKENS];
MiscAnimal gMiscAnimals[MAX_MISC_ANIMALS];
FarmAnimal gFarmAnimals[MAX_FARM_ANIMALS];
Dog dogInfo;
Horse horseInfo;

// watched cow stalls
u8 mrsManaCow1Index;
u8 mrsManaCow2Index;
u8 mrsManaCow3Index;

// shared bss
u8 bornChickenIndex;
u8 bornAnimalIndex;
u8 gCowFestivalEnteredCowIndex;
// price
u32 gAnimalSalePrice;

// game variable strings
u8 gMilkTypeString[2];
u8 D_801886D4[6];
u8 gDeadAnimalName[6];

u8 bornChickenIndex;
// newest animal index (generic)?
u8 gSelectedAnimalIndex;
// dead animal count
// TODO: label
u16 deadAnimalCount;
// purchased animal type
u8 selectedAnimalType;


// data

Vec3f chickenStartingCoordinates[] = {
    { -96.0f, 0.0f, 48.0f },
    { -96.0f, 0.0f, 16.0f },
    { -96.0f, 0.0f, -16.0f },
    { -96.0f, 0.0f, -48.0f },
    { -96.0f, 0.0f, -80.0f },
    { -96.0f, 0.0f, -112.0f },
    { -96.0f, 0.0f, 48.0f },
    { -96.0f, 0.0f, 16.0f },
    { -96.0f, 0.0f, -16.0f },
    { -96.0f, 0.0f, -48.0f },
    { -96.0f, 0.0f, -80.0f },
    { -96.0f, 0.0f, -112.0f },
};

Vec3f farmAnimalStartingCoordinates[] = {
    { -96.0f, 0.0f, 80.0f },
    { -96.0f, 0.0f, 16.0f },
    { -96.0f, 0.0f, -48.0f },
    { -96.0f, 0.0f, -112.0f },
    { 64.0f, 0.0f, 80.0f },
    { 64.0f, 0.0f, 16.0f },
    { 64.0f, 0.0f, -48.0f },
    { 64.0f, 0.0f, -112.0f },
};

Vec3f pregnantCowStartingCoordinates = { 208.0f, 0.0f, -208.0f };

u16 smallMilkInfo[4] = { SMALL_MILK, 100, 6500, 10 };
u16 mediumMilkInfo[4] = { MEDIUM_MILK, 150, 7000, 20 };
u16 largeMilkInfo[4] = { LARGE_MILK, 300,  7500, 30 };
u16 goldenMilkInfo[4] = { GOLDEN_MILK, 500, 8500, 50 };

SheepItemInfo sheepWoolInfo = {
    { WOOL, 900, 4200 },
    { WOOL, 900, 4600 },
    { HIGH_QUALITY_WOOL, 1800, 5000}
};

// forward declarations
void setAnimalState(u8, u8, u8, u8, u8);               
void adjustFarmAnimalAffection(u8, s8); 
void setFarmAnimalLocation(u8); 
void resetFarmAnimalLocation(u8 mapIndex, u8 animalIndex);
void updateDog();                                  
void updateHorse();                                  
void updateHorseGrown();                                 
void updateHorseNotGrown();       
void updateHorseAge(void);
void updateFarmAnimalStartOfDay(u8 index);
void updateChickenStartOfDay(u8 index);
void updateDogAffectionIfFed(void);
u8 spawnMiscAnimal(u8 index, u8 direction, f32 x, f32 y, f32 z);          
void initializeDogEntity();                                  
void initializeChickenEntity(u8);                               
void initializeFarmAnimalEntity(u8);                               
void initializeHorseEntity();                                  
void initializeMiscAnimalEntity(u8, u8); 
void updateChicken(u8);                               
void updateFarmAnimal(u8);                               
void updateMiscAnimal(u8);     
void updateAdultCowNormal(u8 index);      
void updateAdultCowHappy(u8 index);
void updateAdultCowMad(u8 index);
void updateAdultCowSick(u8 index);          
void updateBabyCow(u8 index); 
void updatePregnantCow(u8 index);
void updateAdultChickenNormal(u8 index);
void updateAdultChickenStarved(u8 index);
void updateChick(u8 index);
void updateChickenEgg(u8 index);
void updateAdultChickenStarved(u8 index);
void updateCalf(u8 index);
void updateMiscDog(u8 index);    
void updateBabySheep(u8 index);
void updateAdultSheepNormal(u8 index);
void updateAdultSheepSick(u8 index);
void updateShearedSheepNormal(u8 index);
void updateShearedSheepSick(u8 index);
void updatePlayerDog(u8);
void updateVillageDog(u8);
void updateCat(u8);
void updateRanchHorse(u8);
void updateWildSheep(u8);
void updateRanchCow(u8);
void updateFox(u8);
void updateBunny(u8);
void updateSquirrel(u8);
void updateMonkey(u8);
void updateSparrow(u8);
void updateBird(u8);
void updateCrab(u8);
void updateSnake(u8);
void updateWhiteButterfly(u8);
void updateLadybug(u8);
void updateCicada(u8);
void updateHornedBeetle(u8);
void updateStagBeetle(u8);
void setgAnimalSalePrice(void);
void updateDragonfly(u8);
void updateCricket(u8);                
void updateDogActionState(void);         
void showAnimalExpressionBubble(u8, u8, u8);


//INCLUDE_ASM("asm/nonmatchings/game/animals", deactivateAnimalEntities);

void deactivateAnimalEntities(void) {

    u8 i;

    if (dogInfo.flags & DOG_ENTITY_LOADED) {
        deactivateEntity(dogInfo.entityIndex);
    }

    dogInfo.actionState = 0;
    dogInfo.stateTimer = 0;
    dogInfo.flags &= ~DOG_ENTITY_LOADED;

    for (i = 0; i < MAX_CHICKENS; i++) {

        if (gChickens[i].flags & CHICKEN_ENTITY_LOADED) {
            deactivateEntity(gChickens[i].entityIndex);
        }

        gChickens[i].actionState = 0;
        gChickens[i].stateTimer = 0;
        gChickens[i].flags &= ~CHICKEN_ENTITY_LOADED;
        
    }

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {

        if (gFarmAnimals[i].flags & FARM_ANIMAL_ENTITY_LOADED) {
            deactivateEntity(gFarmAnimals[i].entityIndex);
        }

        gFarmAnimals[i].actionState = 0;
        gFarmAnimals[i].stateTimer = 0;
        gFarmAnimals[i].flags &= ~FARM_ANIMAL_ENTITY_LOADED;
        
    }

    if (horseInfo.flags & HORSE_ENTITY_LOADED) {
        deactivateEntity(horseInfo.entityIndex);
    }

    horseInfo.actionState = 0;
    horseInfo.stateTimer = 0;
    horseInfo.flags &= ~HORSE_ENTITY_LOADED;

    for (i = 0; i < MAX_MISC_ANIMALS; i++) {
        
        if (gMiscAnimals[i].flags & MISC_ANIMAL_ENTITY_LOADED) {
            deactivateEntity(gMiscAnimals[i].entityIndex);
        }
        
        gMiscAnimals[i].flags = 0;
        gMiscAnimals[i].actionState = 0;
        gMiscAnimals[i].timer = 0;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", setAnimalState);

void setAnimalState(u8 animalType, u8 index, u8 type, u8 condition, u8 actionState) {

    switch (animalType) {       
        
        case 0:
            break;
        
        case 1:
            
            if (type != 0xFF) {
                gChickens[index].type = type;
                gChickens[index].typeCounter = 0;
            }
            
            if (condition != 0xFF) {
                gChickens[index].condition = condition;
                gChickens[index].conditionCounter = 0;
            }
            
            if (actionState != 0xFF) {
                gChickens[index].actionState = actionState;
            }

            gChickens[index].speed = 0;
            gChickens[index].stateTimer = 0;
            gChickens[index].unk_1B = 0;
        
            break;
        
        case 2:
            
            if (type != 0xFF) {
                gFarmAnimals[index].type = type;
                gFarmAnimals[index].typeCounter = 0;
            }
            
            if (condition != 0xFF) {
                gFarmAnimals[index].condition = condition;
                gFarmAnimals[index].conditionCounter = 0;
            }
            
            if (actionState != 0xFF) {
                gFarmAnimals[index].actionState = actionState;
            }

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;
        
            break;
        
        default:
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", adjustAllAnimalAffection);

void adjustAllAnimalAffection(s8 amount) {

    u8 i;

    adjustDogAffection(amount);
    adjustHorseAffection(amount);

    for (i = 0; i < 8; i++) {
        adjustFarmAnimalAffection(i, amount);
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", adjustDogAffection);

inline void adjustDogAffection(s8 amount) {
    if (dogInfo.flags & DOG_ACTIVE) {
        dogInfo.affection += adjustValue(dogInfo.affection, amount, 0xFF);
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", adjustHorseAffection);

inline void adjustHorseAffection(s8 amount) {
    if (horseInfo.flags & HORSE_ACTIVE) {
        horseInfo.affection += adjustValue(horseInfo.affection, amount, 0xFF);
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", adjustFarmAnimalAffection);

void adjustFarmAnimalAffection(u8 animalIndex, s8 amount) {
    
    s8 adjusted;
    
    if (gFarmAnimals[animalIndex].flags & FARM_ANIMAL_ACTIVE) {
    
        switch (gFarmAnimals[animalIndex].type) {

            case BABY_COW:
            case CALF:
            case PREGNANT_COW:
            case BABY_SHEEP:
                adjusted = amount;
                break;

            case ADULT_COW:

                switch (gFarmAnimals[animalIndex].condition) {
                    
                    case COW_NORMAL:
                        adjusted = amount;
                        break;

                    case COW_MAD:
                        adjusted = (amount >= 0) ? (amount / 2) : -(amount * 2);
                        break;
                    
                    case COW_HAPPY:
                        adjusted = (amount >= 0) ? (amount * 2) : -(amount / 2);
                        break;

                    case COW_SICK:
                        adjusted = (amount >= 0) ? (amount / 2) : -(amount * 2);
                        break;
                    
                }

                break;

            case SHEARED_SHEEP:
                
                switch (gFarmAnimals[animalIndex].condition) {

                    case SHEEP_NORMAL:
                        adjusted = amount;
                        break;
                
                    case SHEEP_SICK:
                        adjusted = (amount >= 0) ? (amount / 2) : -(amount * 2);
                        break;
                
                }
                
                break;

            case ADULT_SHEEP:
                
                switch (gFarmAnimals[animalIndex].condition) {
                    
                    case SHEEP_NORMAL:
                        adjusted = amount;
                        break;
                        
                    case SHEEP_SICK:
                        adjusted = (amount >= 0) ? (amount / 2) : -(amount * 2);
                        break;
            
                }

                break;

        }

        gFarmAnimals[animalIndex].affection += adjustValue(gFarmAnimals[animalIndex].affection, adjusted, MAX_AFFECTION);
    
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", getMilkHeldItemIndex);

inline u16 getMilkHeldItemIndex(u8 animalIndex, u8 arg1) {

    u16 res;
    
    if (gFarmAnimals[animalIndex].milkType == 0) {

        res = goldenMilkInfo[arg1];
        
    } else {

        if (gFarmAnimals[animalIndex].affection <= 150) {
            res = smallMilkInfo[arg1];
        }

        if (150 < gFarmAnimals[animalIndex].affection && gFarmAnimals[animalIndex].affection < 221) {
            res = mediumMilkInfo[arg1];
        }

        if (gFarmAnimals[animalIndex].affection >= 221) {
            res = largeMilkInfo[arg1];
        }
        
    }

    return res;

}

//INCLUDE_ASM("asm/nonmatchings/game/animals", getWoolHeldItemIndex);

inline u16 getWoolHeldItemIndex(u8 animalIndex, u8 arg1) {

    u16 res;

    if (gFarmAnimals[animalIndex].affection < 100) {
        res = sheepWoolInfo.arr[arg1];
    }

    if (99 < gFarmAnimals[animalIndex].affection && gFarmAnimals[animalIndex].affection < 200) {
        res = sheepWoolInfo.arr2[arg1];
    }

    if (gFarmAnimals[animalIndex].affection >= 200) {
        res = sheepWoolInfo.arr3[arg1];
    }
    
    return res;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", handlePlayerAnimalInteraction);

bool handlePlayerAnimalInteraction(void) {

    Vec3f vec;
    
    bool set = FALSE;
    u8 i;
    
    u16 textIndex;
    u16 tempFlags;
    u8 groundObjectIndex;

    if (!(gPlayer.flags & PLAYER_RIDING_HORSE)) {

        if (gPlayer.heldItem == 0) {

            if ((dogInfo.flags & DOG_ENTITY_LOADED) && entities[dogInfo.entityIndex].entityCollidedWithIndex == ENTITY_PLAYER && entities[dogInfo.entityIndex].buttonPressed == BUTTON_A) {

                // ???
                if (!i) {
                    gPlayer.heldItem = DOG_HELD_ITEM;
                } else {
                    gPlayer.heldItem = PUPPY_1_HELD_ITEM;
                }
                
                deactivateEntity(dogInfo.entityIndex);
                setPlayerAction(PICKING_UP_ITEM, ANIM_PICKING_UP_ITEM);

                dogInfo.flags &= ~(DOG_ENTITY_LOADED | DOG_FOLLOWING);
                dogInfo.flags |= DOG_HELD;

                if (!(dogInfo.flags & DOG_HELD_DAILY)) {

                    adjustDogAffection(1);

                    dogInfo.flags |= DOG_HELD_DAILY;
                    
                }
                
                set = TRUE;
                
            }

            for (i = 0; i < MAX_CHICKENS && !set; i++) {

                if ((gChickens[i].flags & CHICKEN_ENTITY_LOADED) && entities[gChickens[i].entityIndex].entityCollidedWithIndex == ENTITY_PLAYER && entities[gChickens[i].entityIndex].buttonPressed == BUTTON_A) {

                    if (checkDailyEventBit(2) && getLevelFlags(gChickens[i].location) & LEVEL_FARM) {

                        if (gChickens[i].type == ADULT_CHICKEN) {

                            setGameVariableString(0xD, gChickens[i].name, 6);
                            showPinkOverlayText(7);
                            
                            selectedAnimalType = CHICKEN_TYPE;
                            gSelectedAnimalIndex = i;
                            
                            setgAnimalSalePrice();
                            
                            
                        } else {
                            showTextBox(0, SHOP_TEXT_INDEX, 0x5C, 0, 2);
                        }
                        
                        set = TRUE;
                        
                    } else {

                        switch (gChickens[i].type) {

                            case ADULT_CHICKEN:
                                gPlayer.heldItem = CHICKEN_HELD_ITEM;
                                gChickens[i].flags |= CHICKEN_HELD;
                                set = TRUE;
                                break;
                            
                            case CHICK:
                                gPlayer.heldItem = CHICK_HELD_ITEM;
                                gChickens[i].flags |= CHICKEN_HELD;
                                set = TRUE;
                                break;
                            
                            case CHICKEN_EGG:

                                if (!(gChickens[i].flags & CHICKEN_EGG_INCUBATING)) {
                                    set = TRUE;
                                    gPlayer.heldItem = EGG_HELD_ITEM;
                                    gChickens[i].flags = 0;
                                }
                                
                                break;
                            
                        }

                        if (set) {
                            
                            deactivateEntity(gChickens[i].entityIndex);
                            
                            setPlayerAction(PICKING_UP_ITEM, ANIM_PICKING_UP_ITEM);
                            gPlayer.heldAnimalIndex = i;
                            
                            gChickens[i].flags &= ~CHICKEN_ENTITY_LOADED;
                            
                        }
                        
                    }
                    
                }

            }

            for (i = 0; i < MAX_FARM_ANIMALS && !set; i++) {

                if ((gFarmAnimals[i].flags & FARM_ANIMAL_ENTITY_LOADED) && entities[gFarmAnimals[i].entityIndex].entityCollidedWithIndex == ENTITY_PLAYER) {

                    if (entities[gFarmAnimals[i].entityIndex].buttonPressed == BUTTON_A) {
                        
                        if (checkDailyEventBit(2) && getLevelFlags(gFarmAnimals[i].location) & LEVEL_FARM) {

                            if ((gFarmAnimals[i].type == ADULT_COW || gFarmAnimals[i].type == ADULT_SHEEP) && (!(COW_HAPPY < gFarmAnimals[i].condition && gFarmAnimals[i].condition < COW_DEAD))) {
                                
                                setGameVariableString(13, gFarmAnimals[i].name, 6);
                                // pink overlay message
                                showPinkOverlayText(7);
                                
                                if (gFarmAnimals[i].type == ADULT_COW) {
                                    selectedAnimalType = COW_TYPE;
                                } else {
                                    selectedAnimalType = SHEEP_TYPE;
                                }
                                
                                gSelectedAnimalIndex = i;
                                
                                setgAnimalSalePrice();
                                
                            }  else {
                                showTextBox(0, SHOP_TEXT_INDEX, 92, 0, 2);
                            }
                        
                        } else if (checkDailyEventBit(0x1F)) {
    
                            if (gFarmAnimals[i].type == ADULT_COW && !(COW_HAPPY < gFarmAnimals[i].condition && gFarmAnimals[i].condition < COW_DEAD)) {
                                
                                // pink overlay message
                                showPinkOverlayText(8);
                                
                                setGameVariableString(0xD, gFarmAnimals[i].name, 6);
                                gSelectedAnimalIndex = i;
                                
                            }  else {
                                showTextBox(0, FESTIVALS_TEXT_INDEX, 53, 0, 0);
                            }
                            
                        } else {
    
                            setMessageBoxInterpolationWithFlags(0, -4, 2);
                            setMessageBoxViewSpacePosition(0, 0.0f, -64.0f, 352.0f);
                            setMessageBoxSpriteIndices(0, 1, 0, 0);
                            
                            setGameVariableString(0xD, gFarmAnimals[i].name, 6);
    
                            switch (gFarmAnimals[i].condition) {
    
                                case 0:
                                    textIndex = 0;
                                    break;
                                
                                case 1:
                                    textIndex = 2;
                                    break;
    
                                case 2:
                                    textIndex = 1;
                                    break;
                                
                                case 3:
                                    textIndex = 3;
                                    break;
                                
                            }
    
                            if (gFarmAnimals[i].type == PREGNANT_COW) {
    
                                convertNumberToGameVariableString(21, 21 - gFarmAnimals[i].typeCounter, 1);
    
                                switch (gFarmAnimals[i].typeCounter) {
                                    
                                    case 0:
                                        textIndex = 4;
                                        break;
                                    case COW_PREGNANCY_DURATION:
                                        textIndex = 6;
                                        break;
                                    default:
                                        textIndex = 5;
                                        break;
                                    
                                }
    
                            }
    
                            showTextBox(1, ANIMAL_INTERACTIONS_TEXT_INDEX, textIndex, 0, 2);
    
                            gFarmAnimals[i].flags |= (FARM_ANIMAL_APPROACHING  | FARM_ANIMAL_FOLLOWING);
                            
                            if (!(gFarmAnimals[i].flags & FARM_ANIMAL_TALKED_TO)) {
                                
                                adjustFarmAnimalAffection(i, 1);
                                setAnimalState(2, i, 0xFF, 0xFF, 17);
    
                                gFarmAnimals[i].flags |= FARM_ANIMAL_COLLISION_WITH_PLAYER;
                                
                                showAnimalExpressionBubble(COW_TYPE, i, 3);
    
                                gFarmAnimals[i].flags |= FARM_ANIMAL_TALKED_TO;
                                
                            }
                            
                        }
    
                        gFarmAnimals[i].flags &= ~FARM_ANIMAL_LINGERING;
    
                    }

                    set = TRUE;
                    
                }
                
            }
            
            if (horseInfo.flags & HORSE_ENTITY_LOADED && entities[horseInfo.entityIndex].entityCollidedWithIndex == ENTITY_PLAYER && entities[horseInfo.entityIndex].buttonPressed == BUTTON_A) {

                if (gPlayer.heldItem == 0) {

                    if (horseInfo.grown == TRUE) {

                        if (!checkTerrainCollisionInDirection(ENTITY_PLAYER, 0x18, convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, gMainMapIndex)) 
                            && !checkTerrainCollisionInDirection(ENTITY_PLAYER, 0x10, convertWorldToSpriteDirection(gPlayer.direction, gMainMapIndex))) {
                            
                            vec = projectEntityPosition(ENTITY_PLAYER, 0x20, convertWorldToSpriteDirection(gPlayer.direction, MAIN_MAP_INDEX));
                             
                            groundObjectIndex = getGroundObjectIndexFromCoordinates(vec.x, vec.z);

                            if (groundObjectIndex == 0xFF || (getGroundObjectPlayerInteractionsFlags(groundObjectIndex) & 8)) {
                                
                                setPlayerAction(MOUNTING_HORSE, ANIM_MOUNT_HORSE);

                                horseInfo.flags &= ~(HORSE_ENTITY_LOADED | HORSE_FOLLOWING);

                                if (!(horseInfo.flags & HORSE_RODE_DAILY)) {
                                    adjustHorseAffection(1);
                                    horseInfo.flags |= HORSE_RODE_DAILY;
                                }
                                
                                set = TRUE;
                                
                            }
                            
                        }
                        
                    } else {

                        setMessageBoxInterpolationWithFlags(0, -4, 2);
                        setMessageBoxViewSpacePosition(0, 0.0f, -64.0f, 352.0f);
                        setMessageBoxSpriteIndices(0, 1, 0, 0);
                        setGameVariableString(3, horseInfo.name, 6);
                        
                        if (horseInfo.age == 0) {
                            showTextBox(1, ANIMAL_INTERACTIONS_TEXT_INDEX, 7, 0, 2);
                        } else {
                            showTextBox(1, ANIMAL_INTERACTIONS_TEXT_INDEX, 8, 0, 2);
                        }

                        horseInfo.flags |= HORSE_FOLLOWING;
                            
                        if (!(horseInfo.flags & HORSE_TALKED_TO_DAILY)) {

                            horseInfo.actionState = 17;

                            if (checkHaveKeyItem(STAMINA_CARROT)) {
                                adjustHorseAffection(2);
                            } else {
                                adjustHorseAffection(1);
                            }

                            horseInfo.flags |= (HORSE_COLLISION_WITH_PLAYER | HORSE_TALKED_TO_DAILY);
                            
                            showAnimalExpressionBubble(HORSE_TYPE, 0, 3);
                            
                        }
                    
                        set = TRUE;
    
                    }
                    
                }
                
            }

            for (i = 0; i < MAX_MISC_ANIMALS && !set; i++) {

                if (gMiscAnimals[i].flags & MISC_ANIMAL_ENTITY_LOADED) {

                    if (entities[gMiscAnimals[i].entityIndex].entityCollidedWithIndex == ENTITY_PLAYER && entities[gMiscAnimals[i].entityIndex].buttonPressed == BUTTON_A) {

                        switch (gMiscAnimals[i].animalType) {

                            case 0:
                                gPlayer.heldItem = DOG_2_HELD_ITEM;
                                set = TRUE;
                                break;
                            
                            case 1:
                                gPlayer.heldItem = PUPPY_1_HELD_ITEM;
                                set = TRUE;
                                break;
                            
                            case 2:
                                gPlayer.heldItem = CAT_HELD_ITEM;
                                set = TRUE;
                                break;
                            
                            case 7:
                                gPlayer.heldItem = FOX_HELD_ITEM;
                                set = TRUE;
                                break;
                            
                            case 8:
                                gPlayer.heldItem = RABBIT_HELD_ITEM;
                                set = TRUE;
                                break;
                            
                            case 9:

                                if (!(15 < gMiscAnimals[i].actionState && gMiscAnimals[i].actionState < 18)) {
                                    gPlayer.heldItem = SQUIRREL_HELD_ITEM;
                                    set = TRUE;
                                }
                                
                                break;
                            
                            case 10:
                                
                                if (!(15 < gMiscAnimals[i].actionState && gMiscAnimals[i].actionState < 18)) {
                                    gPlayer.heldItem = MONKEY_HELD_ITEM;
                                    set = TRUE;
                                }
                                
                                break;

                            case 16:

                                if (gMiscAnimals[i].actionState != 1) {
                                    gPlayer.heldItem = LADYBUG_HELD_ITEM;
                                    set = TRUE;
                                }
                                
                                break;
                            
                            case 17:
                                                                
                                if (gMiscAnimals[i].actionState != 1) {
                                    gPlayer.heldItem = CICADA_HELD_ITEM;
                                    set = TRUE;
                                }
                                
                                break;
                            
                            case 18:
                                                                
                                if (gMiscAnimals[i].actionState != 1) {
                                    gPlayer.heldItem = HORNED_BEETLE_HELD_ITEM;
                                    set = TRUE;
                                }
                                
                                break;
                            
                            case 19:
                                                                
                                if (gMiscAnimals[i].actionState != 1) {
                                    gPlayer.heldItem = STAG_BEETLE_HELD_ITEM;
                                    set = TRUE;
                                }
                                
                                break;
                            
                            case 21:
                                                                
                                if (gMiscAnimals[i].actionState != 1) {
                                    gPlayer.heldItem = CRICKET_HELD_ITEM;
                                    set = TRUE;
                                }
                                
                                break;
                            
                            case 15:
                                                                
                                if (gMiscAnimals[i].actionState != 1) {
                                    gPlayer.heldItem = BUTTERFLY_HELD_ITEM;
                                    set = TRUE;
                                }
                                
                                break;
                            
                            case 20:

                                if (gMiscAnimals[i].actionState != 1) {
                                    gPlayer.heldItem = RED_DRAGONFLY_HELD_ITEM;
                                    set = TRUE;
                                }
                                
                                break;
                            
                        }

                        if (set) {

                            deactivateEntity(gMiscAnimals[i].entityIndex);
                            
                            setPlayerAction(PICKING_UP_ITEM, ANIM_PICKING_UP_ITEM);
                            gPlayer.heldAnimalIndex = i;

                            gMiscAnimals[i].flags = 0;
                            
                        }

                        if (!checkDailyEventBit(HELD_ANIMAL)) {

                            gHappiness += adjustValue(gHappiness, 1, MAX_HAPPINESS);
                            setDailyEventBit(HELD_ANIMAL);
                            
                        }
                            
                    } else {

                        if (entities[gMiscAnimals[i].entityIndex].unk_5E == 0) {
                            
                            switch (gMiscAnimals[i].animalType) {

                                case 9:

                                    if (15 < gMiscAnimals[i].actionState && gMiscAnimals[i].actionState < 18) {
                                        gMiscAnimals[i].actionState = 0x11;    
                                    } else {
                                        gMiscAnimals[i].actionState = 0x20;
                                        gMiscAnimals[i].flags |= MISC_ANIMAL_RUNNING_AWAY;
                                    }
                                    
                                    break;

                                case 10:

                                    if (15 < gMiscAnimals[i].actionState && gMiscAnimals[i].actionState < 18) {
                                        gMiscAnimals[i].actionState = 0x11;
                                    } else {
                                        gMiscAnimals[i].actionState = 0x20;
                                        gMiscAnimals[i].flags |= MISC_ANIMAL_RUNNING_AWAY;
                                    }
                                    
                                    break;

                                case 7:
                                    gMiscAnimals[i].actionState = 0x20;
                                    gMiscAnimals[i].flags |= MISC_ANIMAL_RUNNING_AWAY;
                                    break;

                                case 8:
                                    gMiscAnimals[i].actionState = 0x20;
                                    gMiscAnimals[i].flags |= MISC_ANIMAL_RUNNING_AWAY;
                                    break;

                                case 11:
                                    gMiscAnimals[i].actionState = 0x20;
                                    gMiscAnimals[i].flags |= MISC_ANIMAL_RUNNING_AWAY;
                                    break;

                                case 12:
                                    gMiscAnimals[i].actionState = 0x20;
                                    gMiscAnimals[i].flags |= MISC_ANIMAL_RUNNING_AWAY;
                                    break;

                                case 13:
                                    gMiscAnimals[i].actionState = 0x20;
                                    gMiscAnimals[i].flags |= MISC_ANIMAL_RUNNING_AWAY;
                                    break;

                                case 14:
                                    break;
                                
                            }
                        
                            if (!checkDailyEventBit(HELD_ANIMAL)) {

                                gHappiness += adjustValue(gHappiness, 1, MAX_HAPPINESS);
                                setDailyEventBit(HELD_ANIMAL);
                                
                            }
                            
                        }
                        
                    }
                    
                }
                
            }
            
        }
        
    }
    
    return set;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", initializeAnimalEntities);

void initializeAnimalEntities(void) {

    u8 i;

    // check dog race
    if (!checkLifeEventBit(0x52) || !checkDailyEventBit(0x42)) {
        initializeDogEntity();
    }

    for (i = 0; i < MAX_CHICKENS; i++) {
        initializeChickenEntity(i);
    }

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {
        initializeFarmAnimalEntity(i);
    }

    // check horse race
    if (!checkLifeEventBit(0x51) || !checkDailyEventBit(0x41)) {
        initializeHorseEntity();
    }

    // other animals
    for (i = 0; i < 7; i++) {
        initializeMiscAnimalEntity(i, 0);
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", deactivateAnimalsAfterCutscene);

void deactivateAnimalsAfterCutscene(void) {

    u8 i;

    if (dogInfo.flags & DOG_ENTITY_LOADED) {
        if (entities[dogInfo.entityIndex].flags & ENTITY_IS_CURRENTLY_ANIMATED) {
            deactivateEntity(dogInfo.entityIndex);
        }
    }

    for (i = 0; i < MAX_CHICKENS; i++) {

        if (gChickens[i].flags & CHICKEN_ENTITY_LOADED) {

            if (entities[gChickens[i].entityIndex].flags & ENTITY_IS_CURRENTLY_ANIMATED) {
                deactivateEntity(gChickens[i].entityIndex);
            }

        }
    }

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {
        if (gFarmAnimals[i].flags & FARM_ANIMAL_ENTITY_LOADED) {
            if (entities[gFarmAnimals[i].entityIndex].flags & ENTITY_IS_CURRENTLY_ANIMATED) {
                deactivateEntity(gFarmAnimals[i].entityIndex);
            }
        }
    }

    if (horseInfo.flags & HORSE_ENTITY_LOADED) {
        if (entities[horseInfo.entityIndex].flags & ENTITY_IS_CURRENTLY_ANIMATED) {
            deactivateEntity(horseInfo.entityIndex);
        }
    }

    for (i = 0; i < MAX_MISC_ANIMALS; i++) {
        if (gMiscAnimals[i].flags & MISC_ANIMAL_ENTITY_LOADED) {
            if (entities[gMiscAnimals[i].entityIndex].flags & ENTITY_IS_CURRENTLY_ANIMATED) {
                deactivateEntity(gMiscAnimals[i].entityIndex);
            }
        }
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", updateAnimalCoordinates);

void updateAnimalCoordinates(void) {

    u8 i = 0;

    if ((dogInfo.flags & DOG_ENTITY_LOADED) && (dogInfo.location == gBaseMapIndex)) {
        dogInfo.coordinates.x = entities[dogInfo.entityIndex].coordinates.x; 
        dogInfo.coordinates.y = entities[dogInfo.entityIndex].coordinates.y;
        dogInfo.coordinates.z = entities[dogInfo.entityIndex].coordinates.z;
    }

    for (i = 0; i < MAX_CHICKENS; i++) {

        if ((gChickens[i].flags & CHICKEN_ENTITY_LOADED) && (gChickens[i].location == gBaseMapIndex)) {

            gChickens[i].coordinates.x = entities[gChickens[i].entityIndex].coordinates.x; 
            gChickens[i].coordinates.y = entities[gChickens[i].entityIndex].coordinates.y;
            gChickens[i].coordinates.z = entities[gChickens[i].entityIndex].coordinates.z;
            
        }
        
    }

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {

        if ((gFarmAnimals[i].flags & FARM_ANIMAL_ENTITY_LOADED) && (gFarmAnimals[i].location == gBaseMapIndex)) {

            gFarmAnimals[i].coordinates.x = entities[gFarmAnimals[i].entityIndex].coordinates.x; 
            gFarmAnimals[i].coordinates.y = entities[gFarmAnimals[i].entityIndex].coordinates.y;
            gFarmAnimals[i].coordinates.z = entities[gFarmAnimals[i].entityIndex].coordinates.z;
            
        }
        
    }

    if ((horseInfo.flags & HORSE_ENTITY_LOADED) && (horseInfo.location == gBaseMapIndex)) {
        horseInfo.coordinates.x = entities[horseInfo.entityIndex].coordinates.x; 
        horseInfo.coordinates.y = entities[horseInfo.entityIndex].coordinates.y;
        horseInfo.coordinates.z = entities[horseInfo.entityIndex].coordinates.z;
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", updateAnimals);

void updateAnimals(void) {

    u8 i;
    
    updateDog();

    for (i = 0; i < MAX_CHICKENS; i++) {
        updateChicken(i);
    }

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {
        updateFarmAnimal(i);
    }

    updateHorse();
    
    for (i = 0; i < 7; i++) {
        updateMiscAnimal(i);
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", feedAllAnimals);

void feedAllAnimals(void) {

    u8 i;

    for (i = 0; i < MAX_CHICKENS; i++) {
        gChickens[i].flags |= CHICKEN_FED;
    }

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {
        gFarmAnimals[i].flags |= (FARM_ANIMAL_FED | FARM_ANIMAL_ATE_GRASS);
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", resetAnimalStatuses);

void resetAnimalStatuses(void) {

    u8 i;
    
    updateDogAffectionIfFed();

    dogInfo.flags &= ~(DOG_HELD_DAILY | DOG_WHISTLED_FOR_DAILY);

    for (i = 6; i < MAX_CHICKENS; i++) {
        if (!(gChickens[i].flags & CHICKEN_EGG_INCUBATING)) {
            gChickens[i].flags = 0;
        }
    }

    for (i = 0; i < MAX_CHICKENS; i++) {
        updateChickenStartOfDay(i);
        gChickens[i].flags &= ~(CHICKEN_FED| 0x80);
    }

    for (i = 0; i < MAX_FARM_ANIMALS; i++) { 
        updateFarmAnimalStartOfDay(i);
        gFarmAnimals[i].flags &= ~(FARM_ANIMAL_FED | FARM_ANIMAL_ATE_GRASS | FARM_ANIMAL_BRUSHED | FARM_ANIMAL_MILKED | FARM_ANIMAL_PREGNANT | FARM_ANIMAL_SHEARED | FARM_ANIMAL_TALKED_TO | 0x4000);
    }

    updateHorseAge();

    horseInfo.flags &= ~(HORSE_BRUSHED_DAILY | HORSE_WHISTLED_DAILY | HORSE_RODE_DAILY | HORSE_TALKED_TO_DAILY);
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", initializeNewChicken);

u8 initializeNewChicken(u8 animalType, u8 arg1) {

    u8 found;
    u8 i;
    u8 temp;

    found = 0xFF;
    temp = animalType != 0 ? 0 : 6;
    
    for (i = 0; i < 6 && found == 0xFF; i++) {

        if (!(gChickens[i + temp].flags & CHICKEN_ACTIVE)) {
            found = i + temp;
        }
        
    }

    if (found != 0xFF) {

        if (arg1 == 0xFF) {

            gChickens[found].location = COOP;

            gChickens[found].coordinates.x = chickenStartingCoordinates[found].x;
            gChickens[found].coordinates.y = chickenStartingCoordinates[found].y;
            gChickens[found].coordinates.z = chickenStartingCoordinates[found].z;
            
        } else {

            gChickens[found].location = gChickens[arg1].location;

            gChickens[found].coordinates.x = gChickens[arg1].coordinates.x;
            gChickens[found].coordinates.y = gChickens[arg1].coordinates.y;
            gChickens[found].coordinates.z = gChickens[arg1].coordinates.z;
            
        }

        setAnimalState(1, found, animalType, 0, 0);

        gChickens[found].direction = 2;
        gChickens[found].flags = CHICKEN_ACTIVE;
        
    }

    return found;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", initializeChicken);

void initializeChicken(u8 chickenIndex) {

    gChickens[chickenIndex].location = 0;
    
    gChickens[chickenIndex].actionState = 0;
    gChickens[chickenIndex].direction = 0;
    gChickens[chickenIndex].speed = 0;
    gChickens[chickenIndex].stateTimer = 0;
    gChickens[chickenIndex].unk_1B = 0;

    gChickens[chickenIndex].type = 0;
    gChickens[chickenIndex].condition = 0;
    gChickens[chickenIndex].typeCounter = 0;
    gChickens[chickenIndex].conditionCounter = 0;

    gChickens[chickenIndex].coordinates.x = 0;
    gChickens[chickenIndex].coordinates.y = 0;
    gChickens[chickenIndex].coordinates.z = 0;
    
    gChickens[chickenIndex].flags = 0;
    
    gChickens[chickenIndex].name[0] = 0;
    gChickens[chickenIndex].name[1] = 0;
    gChickens[chickenIndex].name[2] = 0;
    gChickens[chickenIndex].name[3] = 0;
    gChickens[chickenIndex].name[4] = 0;
    gChickens[chickenIndex].name[5] = 0;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008820C);

u8 initializeNewFarmAnimal(u8 animalType, u8 arg1) {

    u8 index = 0xFF;
    u8 i = 0;

    // find free farm animal slot
    do {

        if (!(gFarmAnimals[i].flags & FARM_ANIMAL_ACTIVE)) {
            index = i;
        }

        i++;
        
    } while (i < MAX_FARM_ANIMALS && index == 0xFF);

    if (index != 0xFF) {
        
        setAnimalState(2, index, animalType, 0, 0);
        setFarmAnimalLocation(index);

        gFarmAnimals[index].birthdaySeason = gSeason;
        gFarmAnimals[index].flags = FARM_ANIMAL_ACTIVE;
        gFarmAnimals[index].birthdayDayOfMonth = gDayOfMonth;
        gFarmAnimals[index].milkType = 0xFF;
        gFarmAnimals[index].affection = 0;

        if (animalType == 0) {

            gFarmAnimals[index].motherName[0] = gFarmAnimals[arg1].name[0];
            gFarmAnimals[index].motherName[1] = gFarmAnimals[arg1].name[1];
            gFarmAnimals[index].motherName[2] = gFarmAnimals[arg1].name[2];
            gFarmAnimals[index].motherName[3] = gFarmAnimals[arg1].name[3];
            gFarmAnimals[index].motherName[4] = gFarmAnimals[arg1].name[4];
            gFarmAnimals[index].motherName[5] = gFarmAnimals[arg1].name[5];
            
        } else {

            gFarmAnimals[index].motherName[0] = 0xF6;
            gFarmAnimals[index].motherName[1] = 0xF6;
            gFarmAnimals[index].motherName[2] = 0xF6;
            gFarmAnimals[index].motherName[3] = 0xF6;
            gFarmAnimals[index].motherName[4] = 0xF6;
            gFarmAnimals[index].motherName[5] = 0xF6;
            
        }
        
    }

    return index;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", setMrsManaCowsNames);

void setMrsManaCowsNames(void) {

    mrsManaCow1Index = initializeNewFarmAnimal(1, 0);
    mrsManaCow2Index = initializeNewFarmAnimal(1, 0);
    mrsManaCow3Index = initializeNewFarmAnimal(1, 0);

    gFarmAnimals[mrsManaCow1Index].name[0] = char_M;
    gFarmAnimals[mrsManaCow1Index].name[1] = char_o;
    gFarmAnimals[mrsManaCow1Index].name[2] = char_m;
    gFarmAnimals[mrsManaCow1Index].name[3] = char_o;
    gFarmAnimals[mrsManaCow1Index].name[4] = char_k;
    gFarmAnimals[mrsManaCow1Index].name[5] = char_o;

    gFarmAnimals[mrsManaCow2Index].name[0] = char_M;
    gFarmAnimals[mrsManaCow2Index].name[1] = char_o;
    gFarmAnimals[mrsManaCow2Index].name[2] = char_i;
    gFarmAnimals[mrsManaCow2Index].name[3] = char_t;
    gFarmAnimals[mrsManaCow2Index].name[4] = char_i;
    gFarmAnimals[mrsManaCow2Index].name[5] = 0xFF;

    gFarmAnimals[mrsManaCow3Index].name[0] = char_M;
    gFarmAnimals[mrsManaCow3Index].name[1] = char_o;
    gFarmAnimals[mrsManaCow3Index].name[2] = char_t;
    gFarmAnimals[mrsManaCow3Index].name[3] = char_a;
    gFarmAnimals[mrsManaCow3Index].name[4] = 0xFF;
    gFarmAnimals[mrsManaCow3Index].name[5] = 0xFF;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", initializeWatchedCows);

// initialize watched cows from Doug
void initializeWatchedCows(void) {
    initializeFarmAnimal(mrsManaCow1Index);
    initializeFarmAnimal(mrsManaCow2Index);
    initializeFarmAnimal(mrsManaCow3Index);
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", setFarmAnimalLocation);

void setFarmAnimalLocation(u8 animalIndex) {

    gFarmAnimals[animalIndex].location = BARN;

    if (gFarmAnimals[animalIndex].type == PREGNANT_COW) {
        
        gFarmAnimals[animalIndex].coordinates.x = pregnantCowStartingCoordinates.x;
        gFarmAnimals[animalIndex].coordinates.y = pregnantCowStartingCoordinates.y;
        gFarmAnimals[animalIndex].coordinates.z = pregnantCowStartingCoordinates.z;
        gFarmAnimals[animalIndex].direction = NORTHEAST;

    } else {

        gFarmAnimals[animalIndex].coordinates.x = farmAnimalStartingCoordinates[animalIndex].x;
        gFarmAnimals[animalIndex].coordinates.y = farmAnimalStartingCoordinates[animalIndex].y;
        gFarmAnimals[animalIndex].coordinates.z = farmAnimalStartingCoordinates[animalIndex].z;
        
        if (animalIndex >= 4) {
            gFarmAnimals[animalIndex].direction = SOUTHEAST;
        } else {
            gFarmAnimals[animalIndex].direction = NORTHWEST;
        }

    }
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", initializeFarmAnimal);

void initializeFarmAnimal(u8 animalIndex) {

    gFarmAnimals[animalIndex].location = 0;
    
    gFarmAnimals[animalIndex].actionState = 0;
    gFarmAnimals[animalIndex].direction = 0;
    gFarmAnimals[animalIndex].speed = 0;
    gFarmAnimals[animalIndex].stateTimer = 0;
    gFarmAnimals[animalIndex].unk_1E = 0;
    
    gFarmAnimals[animalIndex].type = 0;
    gFarmAnimals[animalIndex].condition = 0;
    gFarmAnimals[animalIndex].typeCounter = 0;
    gFarmAnimals[animalIndex].conditionCounter = 0;

    gFarmAnimals[animalIndex].coordinates.x = 0;
    gFarmAnimals[animalIndex].coordinates.y = 0;
    gFarmAnimals[animalIndex].coordinates.z = 0;

    gFarmAnimals[animalIndex].flags = 0;
    
    gFarmAnimals[animalIndex].name[0] = 0;
    gFarmAnimals[animalIndex].name[1] = 0;
    gFarmAnimals[animalIndex].name[2] = 0;
    gFarmAnimals[animalIndex].name[3] = 0;
    gFarmAnimals[animalIndex].name[4] = 0;
    gFarmAnimals[animalIndex].name[5] = 0;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", initializeDog);

void initializeDog(void) {

    dogInfo.location = FARM;
    dogInfo.coordinates.y = 0.0f;
    dogInfo.actionState = 0;
    dogInfo.direction = SOUTHWEST;
    dogInfo.affection = 0; 
    dogInfo.speed = 0;
    dogInfo.stateTimer = 0;
    dogInfo.unk_1B = 0;
    dogInfo.flags = DOG_ACTIVE;
    dogInfo.coordinates.x = -432.0f;
    dogInfo.coordinates.z = 96.0f;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", initializeHorse);

void initializeHorse(void) {
    
    horseInfo.location = FARM;
    horseInfo.coordinates.y = 0.0f;
    horseInfo.actionState = 0;
    horseInfo.direction = SOUTHWEST;
    horseInfo.speed = 0;
    horseInfo.stateTimer = 0;
    horseInfo.unk_1B = 0;
    horseInfo.grown = 0;
    horseInfo.age = 0;
    horseInfo.affection = 0;
    horseInfo.flags = HORSE_ACTIVE;
    horseInfo.coordinates.x = -240.0f;
    horseInfo.coordinates.z = -384.0f;

}

//INCLUDE_ASM("asm/nonmatchings/game/animals", setAnimalLocations);

void setAnimalLocations(u8 mapIndex) {

    u8 i;
    
    setDogLocation(mapIndex);
    setHorseLocation(mapIndex);

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {
        resetFarmAnimalLocation(mapIndex, i);
    }

    for (i = 0; i < MAX_CHICKENS; i++) {
        resetChickenLocation(mapIndex, i);
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/animals", setDogLocation);

void setDogLocation(u8 mapIndex) {

    if (dogInfo.flags & DOG_ACTIVE && (mapIndex == 0xFF || dogInfo.location == mapIndex)) {
        dogInfo.location = FARM;
        dogInfo.coordinates.y = 0.0f;
        dogInfo.direction = SOUTHWEST;
        dogInfo.flags &= ~0x8;
        dogInfo.coordinates.x = -432.0f;
        dogInfo.coordinates.z = 96.0f;
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/animals", setHorseLocation);

void setHorseLocation(u8 mapIndex) {

    if (horseInfo.flags & HORSE_ACTIVE && (mapIndex == 0xFF || horseInfo.location == mapIndex)) {
        horseInfo.coordinates.y = 0;
        horseInfo.location = FARM;
        horseInfo.direction = SOUTHWEST;
        horseInfo.coordinates.x = -240.0f;
        horseInfo.coordinates.z = -384.0f;
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/animals", resetFarmAnimalLocation);

void resetFarmAnimalLocation(u8 mapIndex, u8 animalIndex) {

    if ((gFarmAnimals[animalIndex].flags & FARM_ANIMAL_ACTIVE) && (mapIndex == 0xFF ||  gFarmAnimals[animalIndex].location == mapIndex)) {
        setFarmAnimalLocation(animalIndex);
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", resetChickenLocation);

void resetChickenLocation(u8 mapIndex, u8 chickenIndex) {

    if ((gChickens[chickenIndex].flags & FARM_ANIMAL_ACTIVE) && (mapIndex == 0xFF || gChickens[chickenIndex].location == mapIndex)) {

        gChickens[chickenIndex].location = COOP;

        if (gChickens[chickenIndex].flags & CHICKEN_EGG_INCUBATING) {

            gChickens[chickenIndex].coordinates.x = 96.0f;
            gChickens[chickenIndex].coordinates.y = 0;
            gChickens[chickenIndex].coordinates.z = -144.0f;
            
        } else {

            gChickens[chickenIndex].coordinates.x = chickenStartingCoordinates[chickenIndex].x;
            gChickens[chickenIndex].coordinates.y = chickenStartingCoordinates[chickenIndex].y;
            gChickens[chickenIndex].coordinates.z = chickenStartingCoordinates[chickenIndex].z;
            
        }

        gChickens[chickenIndex].flags &= ~CHICKEN_HELD;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", spawnWildAnimals);

void spawnWildAnimals(void) {

    switch (gBaseMapIndex) {
        
        case FARM:

            if (gSeason != WINTER && gWeather == SUNNY && 5 < gHour && gHour < 9) {

                if (gMiscAnimals[4].spawnVariant == 1) {
                    spawnMiscAnimal(MISC_ANIMAL_SPARROW, SOUTHWEST, -336.0f, 0.0f, 0.0f);
                    spawnMiscAnimal(MISC_ANIMAL_SPARROW, SOUTHWEST, -368.0f, 0.0f, 32.0f);
                }

                if (gMiscAnimals[4].spawnVariant == 2) {
                    spawnMiscAnimal(MISC_ANIMAL_SPARROW, SOUTHWEST, 320.0f, 0.0f, -352.0f);
                    spawnMiscAnimal(MISC_ANIMAL_SPARROW, SOUTHWEST, 288.0f, 0.0f, -352.0f);
                }
                
            }
            
            break;
        
        case POND:

            if (gSeason == AUTUMN && 12 < gHour && gHour < 22 && gMiscAnimals[0].spawnVariant >= 2 && (u8)(gPlayer.heldItem + 0x75) >= 8) {
                spawnMiscAnimal(MISC_ANIMAL_FOX, 6, -192.0f, 0.0f, -64.0f);
            }

            if (gSeason == WINTER && 12 < gHour && gHour < 22 && gMiscAnimals[0].spawnVariant == 1 && (u8)(gPlayer.heldItem + 0x75) >= 8) {
                spawnMiscAnimal(MISC_ANIMAL_FOX, 6, -192.0f, 0.0f, -64.0f);
            }
            
            if (gSeason == SPRING && 8 < gHour && gHour < 15 && gMiscAnimals[1].spawnVariant == 1 && (u8)(gPlayer.heldItem + 0x65) >= 8) {
                spawnMiscAnimal(MISC_ANIMAL_BUNNY, 4, -192.0f, 0.0f, -160.0f);
            }

            if (gSeason == SUMMER && 10 < gHour && gHour < 17 && !(gMiscAnimals[1].spawnVariant < 2) && (u8)(gPlayer.heldItem + 0x65) >= 8) {
                spawnMiscAnimal(MISC_ANIMAL_BUNNY, 4, -192.0f, 0.0f, -160.0f);
            }

            if (gSeason == SUMMER && 10 < gHour && gHour < 17 && gMiscAnimals[2].spawnVariant == 1 && (u8)(gPlayer.heldItem + 0x6D) >= 8) {
                gMiscAnimals[spawnMiscAnimal(MISC_ANIMAL_SQUIRREL, SOUTHWEST, 144.0f, 0.0f, -256.0f)].actionState = 0x10;
            }

            if (gSeason == WINTER && 10 < gHour && gHour < 17 && gMiscAnimals[3].spawnVariant >= 2 && (u8)(gPlayer.heldItem + 0x5D) >= 8) {
                spawnMiscAnimal(MISC_ANIMAL_MONKEY, SOUTHEAST, -192.0f, 0.0f, -224.0f);
            }

            if (gSeason == SUMMER && gWeather != RAIN && 7 < gHour && gHour < 16 && gMiscAnimals[4].spawnVariant < 2) {
                spawnMiscAnimal(MISC_ANIMAL_SNAKE, SOUTHWEST, 96.0f, 0.0f, -64.0f);
            }

            break;

        case MOUNTAIN_1:

            if (gSeason == SUMMER && 10 < gHour && gHour < 17 && gMiscAnimals[1].spawnVariant < 2 && (u8)(gPlayer.heldItem + 0x65) >= 8) {
                spawnMiscAnimal(MISC_ANIMAL_BUNNY, NORTHWEST, 128.0f, 0.0f, 64.0f);
            }

            if (gSeason == SPRING && 6 < gHour && gHour < 16 && !(gMiscAnimals[2].spawnVariant < 2) && (u8)(gPlayer.heldItem + 0x6D) >= 8) {
                spawnMiscAnimal(MISC_ANIMAL_SQUIRREL, SOUTHWEST, 256.0f, 0.0f, -416.0f);
            }
            
            if (gSeason == SUMMER && 10 < gHour && gHour < 17 && gMiscAnimals[2].spawnVariant < 1 && (u8)(gPlayer.heldItem + 0x6D) >= 8) {
                spawnMiscAnimal(MISC_ANIMAL_SQUIRREL, SOUTHWEST, 160.0f, 0.0f, 128.0f);
            }

            if (gSeason == AUTUMN && 10 < gHour && gHour < 17 && gMiscAnimals[3].spawnVariant == 1 && (u8)(gPlayer.heldItem + 0x5D) >= 8) {
                spawnMiscAnimal(MISC_ANIMAL_MONKEY, SOUTHWEST, -96.0f, 0.0f, 128.0f);
            }

            if (gSeason == SUMMER && gWeather != RAIN && 7 < gHour && gHour < 16 && !(gMiscAnimals[4].spawnVariant < 2)) {
                spawnMiscAnimal(MISC_ANIMAL_SNAKE, SOUTHWEST, 160.0f, 0.0f, 128.0f);
            }

            if (gSeason == AUTUMN && gWeather != RAIN && 8 < gHour && gHour < 16 && !(gMiscAnimals[5].spawnVariant < 2) && gPlayer.heldItem != RED_DRAGONFLY_HELD_ITEM) {
                spawnMiscAnimal(MISC_ANIMAL_DRAGONFLY, SOUTHWEST, 0.0f, 0.0f, -128.0f);
            }

            if (gSeason == AUTUMN && gWeather != RAIN && 8 < gHour && gHour < 16 && gMiscAnimals[4].spawnVariant < 2 && gPlayer.heldItem != CRICKET_HELD_ITEM) {
                spawnMiscAnimal(MISC_ANIMAL_CRICKET, SOUTHWEST, 128.0f, 0.0f, 64.0f);
            }
            
            break;

        case MOUNTAIN_2:

            if (gSeason == AUTUMN && 12 < gHour && gHour < 22 && gMiscAnimals[0].spawnVariant < 2 && (u8)(gPlayer.heldItem + 0x75) >= 8) {
                spawnMiscAnimal(MISC_ANIMAL_FOX, 6, -128.0f, 0.0f, 224.0f);
            }

            if (gSeason == WINTER && 12 < gHour && gHour < 22 && gMiscAnimals[0].spawnVariant == 0 && (u8)(gPlayer.heldItem + 0x75) >= 8) {
                spawnMiscAnimal(MISC_ANIMAL_FOX, 6, -128.0f, 0.0f, 224.0f);
            }
            
            if (gSeason == SPRING && 8 < gHour && gHour < 15 && gMiscAnimals[1].spawnVariant < 1 && (u8)(gPlayer.heldItem + 0x65) >= 8) {
                spawnMiscAnimal(MISC_ANIMAL_BUNNY, NORTHEAST, -192.0f, 0.0f, -192.0f);
            }

            if (gSeason == SPRING && 6 < gHour && gHour < 16 && gMiscAnimals[2].spawnVariant < 2 && (u8)(gPlayer.heldItem + 0x6D) >= 8) {
                spawnMiscAnimal(MISC_ANIMAL_SQUIRREL, SOUTHWEST, -224.0f, 0.0f, -160.0f);
            }

            if (gSeason == SUMMER && 10 < gHour && gHour < 17 && gMiscAnimals[3].spawnVariant < 2 && (u8)(gPlayer.heldItem + 0x5D) >= 8) {
                spawnMiscAnimal(MISC_ANIMAL_MONKEY, SOUTHWEST, -320.0f, 0.0f, -320.0f);
            }

            if (gSeason == SUMMER && 10 < gHour && gHour < 17 && !(gMiscAnimals[3].spawnVariant < 2) && (u8)(gPlayer.heldItem + 0x5D) >= 8) {
                spawnMiscAnimal(MISC_ANIMAL_MONKEY, SOUTHWEST, 0.0f, 0.0f, 288.0f);
            }

            if (gSeason == AUTUMN && 10 < gHour && gHour < 17 && gMiscAnimals[3].spawnVariant == 0 && (u8)(gPlayer.heldItem + 0x5D) >= 8) {
                gMiscAnimals[spawnMiscAnimal(MISC_ANIMAL_MONKEY, SOUTHWEST, -112.0f, 0.0f, -160.0f)].actionState = 0x10;
            }

            if (gSeason != WINTER && gWeather != RAIN && 7 < gHour && gHour < 16 && gMiscAnimals[4].spawnVariant < 2) {
                spawnMiscAnimal(MISC_ANIMAL_BIRD, SOUTHWEST, -128.0f, 0.0f, 32.0f);
                spawnMiscAnimal(MISC_ANIMAL_BIRD, SOUTHEAST, -96.0f, 0.0f, 64.0f);
            }

            if (gSeason < AUTUMN && gWeather != RAIN && 8 < gHour && gHour < 16 && gPlayer.heldItem < 0xB4) {
                spawnMiscAnimal(MISC_ANIMAL_LADYBUG, SOUTHWEST, 96.0f, 0.0f, -96.0f);
            }

            if (gSeason == SUMMER && gWeather != RAIN && 8 < gHour && gHour < 16 && gMiscAnimals[3].spawnVariant < 2 && gPlayer.heldItem != CICADA_HELD_ITEM) {
                spawnMiscAnimal(MISC_ANIMAL_CICADA, SOUTHWEST, -112.0f, 0.0f, -160.0f);
            }
            
            break;
        
        case TOP_OF_MOUNTAIN_1:

            if (gSeason == SUMMER && gWeather != RAIN && 8 < gHour && gHour < 16 && gMiscAnimals[0].spawnVariant < 2 && gPlayer.heldItem != HORNED_BEETLE_HELD_ITEM) {
                spawnMiscAnimal(MISC_ANIMAL_HORNED_BEETLE, SOUTHWEST, 192.0f, 0.0f, -192.0f);
            }

            if (gSeason == SUMMER && gWeather != RAIN && 8 < gHour && gHour < 16 && gMiscAnimals[1].spawnVariant < 2 && gPlayer.heldItem != STAG_BEETLE_HELD_ITEM) {
                spawnMiscAnimal(0x13, SOUTHWEST, 224.0f, 0.0f, 0.0f);
            }

            break;
        
        case MOON_MOUNTAIN:

            if (gSeason != WINTER && gWeather != RAIN && 7 < gHour && gHour < 16 && !(gMiscAnimals[4].spawnVariant < 2)) {
                spawnMiscAnimal(MISC_ANIMAL_BIRD, SOUTHWEST, 192.0f, 0.0f, -64.0f);
                spawnMiscAnimal(MISC_ANIMAL_BIRD, SOUTHEAST, 224.0f, 0.0f, -96.0f);
            }

            if (gSeason == AUTUMN && gWeather != RAIN && 8 < gHour && gHour < 16 && gMiscAnimals[5].spawnVariant == 0 && gPlayer.heldItem != RED_DRAGONFLY_HELD_ITEM) {
                spawnMiscAnimal(MISC_ANIMAL_DRAGONFLY, SOUTHWEST, 128.0f, 0.0f, -256.0f);
            }

            break;
        
        case SQUARE:

            if (gSeason != WINTER && gWeather == SUNNY && 5 < gHour && gHour < 9 && gMiscAnimals[4].spawnVariant == 3) {
                spawnMiscAnimal(MISC_ANIMAL_SPARROW, SOUTHWEST, -128.0f, 0.0f, 0.0f);
                spawnMiscAnimal(MISC_ANIMAL_SPARROW, SOUTHWEST, -96.0f, 0.0f, -32.0f);
            }

            break;
        
        case BEACH:

            if (gSeason == SUMMER && gWeather != RAIN && 7 < gHour && gHour < 16) {
                spawnMiscAnimal(0xD, SOUTHWEST, -256.0f, 0.0f, -64.0f);
            }

            break;

        case VILLAGE_1:
                
            if (gSeason < AUTUMN && gWeather != RAIN && 8 < gHour && gHour < 16 && gMiscAnimals[0].spawnVariant == 0 && gPlayer.heldItem != BUTTERFLY_HELD_ITEM) {
                spawnMiscAnimal(0xF, SOUTHWEST, 64.0f, 0.0f, -352.0f);
            }

            if (gSeason < AUTUMN && gWeather != RAIN && 8 < gHour && gHour < 16 && gMiscAnimals[0].spawnVariant == 1 && gPlayer.heldItem != BUTTERFLY_HELD_ITEM) {
                spawnMiscAnimal(0xF, SOUTHWEST, -448.0f, 0.0f, 0.0f);
            }

            if (gSeason < AUTUMN && gWeather != RAIN && 8 < gHour && gHour < 16 && gMiscAnimals[5].spawnVariant == 1 && gPlayer.heldItem < LADYBUG_HELD_ITEM) {
                spawnMiscAnimal(MISC_ANIMAL_LADYBUG, SOUTHWEST, -448.0f, 0.0f, 64.0f);
            }
        
            break;

        case VILLAGE_2:

            if (8 < gHour && gHour < 18) {

                if (gWeather != RAIN && gSeason != WINTER) {
                    
                    if ((u8)(gPlayer.heldItem + 0x55) >= 8) {
                        spawnMiscAnimal(0, SOUTHWEST, 400.0f, 0.0f, -160.0f);
                    }
    
                    if (checkLifeEventBit(PUPPIES) && (u8)(gPlayer.heldItem - 0x7B) >= 8) {
                        spawnMiscAnimal(1, SOUTHWEST, 432.0f, 0.0f, -160.0f);
                    }
                    
                }

                if (8 < gHour && gHour < 18 && !(SUNNY < gWeather && gWeather < 4)) {
                    
                    if ((u8)(gPlayer.heldItem + 0x7D) >= 8) {
                        spawnMiscAnimal(2, SOUTHWEST, 32.0f, 0.0f, -96.0f);
                    }
                
                    spawnMiscAnimal(2, SOUTHWEST, 0.0f, 0.0f, -64.0f);
                
                }
                
            }

            break;

        case POTION_SHOP:

            if (gWeather == RAIN || gSeason == WINTER)  {

                if ((u8)(gPlayer.heldItem + 0x55) >= 8) {
                    spawnMiscAnimal(0, SOUTHWEST, -64.0f, 0.0f, -64.0f);
                }

                if (checkLifeEventBit(PUPPIES) && (u8)(gPlayer.heldItem - 0x7B) >= 8) {
                    spawnMiscAnimal(1, SOUTHWEST, -96.0f, 0.0f, -64.0f);
                }
                
            }
            
            break;
        
        case RANCH:

            if (gMiscAnimals[0].spawnVariant < 2 && gWeather != RAIN && gSeason != WINTER) {
                spawnMiscAnimal(4, NORTHEAST, -384.0f, 0.0f, 32.0f);
            }

            if (gMiscAnimals[1].spawnVariant < 2 && gWeather != RAIN && gSeason != WINTER) {
                spawnMiscAnimal(3, SOUTHEAST, -320.0f, 0.0f, 0.0f);
            }
   
            if (gMiscAnimals[2].spawnVariant < 2 && gWeather != RAIN && gSeason != WINTER) {
                spawnMiscAnimal(3, SOUTHWEST, -288.0f, 0.0f, 96.0f);
            }         
   
            if (gMiscAnimals[3].spawnVariant < 2 && gWeather != RAIN && gSeason != WINTER) {
                spawnMiscAnimal(6, NORTHEAST, -288.0f, 0.0f, 128.0f);
            }         
   
            if (gMiscAnimals[4].spawnVariant < 2 && gWeather != RAIN && gSeason != WINTER) {
                spawnMiscAnimal(5, SOUTHEAST, -352.0f, 0.0f, 80.0f);
            }         

            break;
        
        case RANCH_BARN:

            if (gMiscAnimals[0].spawnVariant >= 2 || gWeather == RAIN || gSeason == WINTER) {
                spawnMiscAnimal(4, NORTHWEST, -96.0f, 0.0f, -192.0f);
            }

            if (gMiscAnimals[1].spawnVariant >= 2 || gWeather == RAIN || gSeason == WINTER) {
                spawnMiscAnimal(3, NORTHWEST, -96.0f, 0.0f, -128.0f);
            }

            if (gMiscAnimals[2].spawnVariant >= 2 || gWeather == RAIN || gSeason == WINTER) {
                spawnMiscAnimal(3, NORTHWEST, -96.0f, 0.0f, 64.0f);
            }

            if (gMiscAnimals[3].spawnVariant >= 2 || gWeather == RAIN || gSeason == WINTER) {
                spawnMiscAnimal(6, SOUTHEAST, 0.0f, 0.0f, 64.0f);
            }

            if (gMiscAnimals[4].spawnVariant >= 2 || gWeather == RAIN || gSeason == WINTER) {
                spawnMiscAnimal(5, SOUTHEAST, 0.0f, 0.0f, 128.0f);
            }
            
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", spawnMiscAnimal);

u8 spawnMiscAnimal(u8 animalType, u8 direction, f32 x, f32 y, f32 z) {

    u8 i = 0;
    u8 found = (animalType != MISC_ANIMAL_PLAYER_DOG) ? 0xFF : 6;
    u8 temp;

    if (animalType == MISC_ANIMAL_COW) {
        found = 5;
    }

    while (i < 5 && found == 0xFF) {

        if (!(gMiscAnimals[i].flags & MISC_ANIMAL_ACTIVE)) {
            found = i;
        }
        
        i++;
        
    }
        
    if (found != 0xFF) {

        gMiscAnimals[found].animalType = animalType;
        gMiscAnimals[found].direction = direction;
        gMiscAnimals[found].actionState = 0;
        gMiscAnimals[found].yDisplacement = 0;
        gMiscAnimals[found].flags = MISC_ANIMAL_ACTIVE;
        gMiscAnimals[found].coordinates.x = x;
        gMiscAnimals[found].coordinates.y = y;
        gMiscAnimals[found].coordinates.z = z;
        gMiscAnimals[found].entityIndex = ENTITY_MISC_ANIMAL_BASE_INDEX + found;
        gMiscAnimals[found].mapIndex = gBaseMapIndex;
        
    }

    return found;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", updateDogAffectionIfFed);

void updateDogAffectionIfFed(void) {
    
    if (checkDailyEventBit(FED_DOG) && (dogInfo.flags & DOG_ACTIVE)) {
        dogInfo.affection += adjustValue(dogInfo.affection, 2, MAX_AFFECTION);
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", updateChickenStartOfDay);

void updateChickenStartOfDay(u8 index) {
    
    if ((gChickens[index].flags & CHICKEN_ACTIVE) && !(gChickens[index].flags & CHICKEN_NEWBORN)) {
        
        if (gChickens[index].flags & 0x80) {
            
            gChickens[index].flags &= ~CHICKEN_FED;

        } else {

            if (gChickens[index].location != COOP) {
                gChickens[index].flags &= ~CHICKEN_FED;
            }

            if (gChickens[index].location == FARM) {
                
                if (gSeason != WINTER) {
                    gChickens[index].flags |= CHICKEN_FED;
                } 

            } 

        }

        switch (gChickens[index].type) {
        
            case CHICKEN_EGG:
                
                if (gChickens[index].flags & CHICKEN_EGG_INCUBATING) {
                    
                    gChickens[index].typeCounter++;
                    
                    if (gChickens[index].typeCounter == CHICKEN_EGG_INCUBATION_DURATION) {
                        
                        bornChickenIndex = initializeNewChicken(1, 0xFF);
                        
                        if (bornChickenIndex != 0xFF) {

                            gChickens[bornChickenIndex].flags |= CHICKEN_NEWBORN;
                            gChickens[index].flags &= ~CHICKEN_ACTIVE;

                            setLifeEventBit(CHICKEN_BORN);
                            
                        } else {                        
                            gChickens[index].typeCounter--;
                        }
                        
                    }
                    
                }
                
                break;
            
            case CHICK:
                
                gChickens[index].typeCounter++;
                
                if (gChickens[index].typeCounter == CHICK_DURATION) {
                    setAnimalState(1, index, ADULT_CHICKEN, 0, 0);
                }
                
                break;
            
            case ADULT_CHICKEN:
                
                switch (gChickens[index].condition) {
                    
                    case CHICKEN_NORMAL:
                        
                        if ((gChickens[index].flags & CHICKEN_FED)) {
                            initializeNewChicken(0, index);
                        } else {
                            setAnimalState(1, index, 0xFF, CHICKEN_STARVED, 0);
                        }
                        
                        break;
                    
                    case CHICKEN_STARVED:
                        
                        if (gChickens[index].flags & CHICKEN_FED) {
                            setAnimalState(1, index, 0xFF, CHICKEN_NORMAL, 0);
                        } else {
                            
                            gChickens[index].conditionCounter++;
                            
                            if (gChickens[index].conditionCounter == 3) {
                                
                                setAnimalState(1, index, 0xFF, CHICKEN_DEAD, 0xFF);
                                
                                gChickens[index].flags = 0;

                                setLifeEventBit(ANIMAL_DIED);
                                
                                gDeadAnimalName[0] = gChickens[index].name[0];
                                gDeadAnimalName[1] = gChickens[index].name[1];
                                gDeadAnimalName[2] = gChickens[index].name[2];
                                gDeadAnimalName[3] = gChickens[index].name[3];
                                gDeadAnimalName[4] = gChickens[index].name[4];
                                gDeadAnimalName[5] = gChickens[index].name[5];

                            }
                            
                        }
                        
                        break;
                    
                }

            break;
            
        }
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", updateFarmAnimalStartOfDay);

void updateFarmAnimalStartOfDay(u8 index) {
    
    if ((gFarmAnimals[index].flags & FARM_ANIMAL_ACTIVE) && !(gFarmAnimals[index].flags & FARM_ANIMAL_NEWBORN)) {
        
        if (gFarmAnimals[index].flags & 0x4000) {
            
            gFarmAnimals[index].flags &= ~(FARM_ANIMAL_FED | FARM_ANIMAL_ATE_GRASS);

        } else if (gFarmAnimals[index].location == FARM) {
            
            gFarmAnimals[index].flags &= ~(FARM_ANIMAL_FED | FARM_ANIMAL_ATE_GRASS);
            
            if (checkAnimalCanEatGrass()) {
                gFarmAnimals[index].flags |= (FARM_ANIMAL_FED | FARM_ANIMAL_ATE_GRASS);
            }
            
        } else {
            adjustFarmAnimalAffection(index, -1);
        }

        if (gCowFestivalEnteredCowIndex!= 0xFF) {
            
            if (gCowFestivalEnteredCowIndex== index) {
                gFarmAnimals[gCowFestivalEnteredCowIndex].flags |= (FARM_ANIMAL_FED | FARM_ANIMAL_ATE_GRASS);
            }
            
        }

        switch (gFarmAnimals[index].type) {
            
            case BABY_COW:
                gFarmAnimals[index].typeCounter++;
                
                if (gFarmAnimals[index].typeCounter == COW_INFANCY_DURATION) {
                    setAnimalState(2, index, CALF, 0xFF, 0);
                }
                
                break;
            
            case CALF:
                gFarmAnimals[index].typeCounter++;
                
                if (gFarmAnimals[index].typeCounter == COW_YOUTH_DURATION) {
                    setAnimalState(2, index, ADULT_COW, 0xFF, 0);
                }
                
                break;
            
            case ADULT_COW:
                
                switch (gFarmAnimals[index].condition) {
                    
                    case COW_NORMAL:
                        
                        if (!(gFarmAnimals[index].flags & FARM_ANIMAL_FED)) {
                            
                            adjustFarmAnimalAffection(index, -8);
                            
                            if (!(getRandomNumberInRange(0, 1))) {
                                setAnimalState(2, index, 0xFF, COW_SICK, 0);
                                adjustFarmAnimalAffection(index, -30);
                                gHappiness += adjustValue(gHappiness, -10, MAX_HAPPINESS);
                            }
        
                        } else if (gFarmAnimals[index].location == FARM) {
                            
                            if (!(getRandomNumberInRange(0, 3))) {
                                setAnimalState(2, index, 0xFF, COW_HAPPY, 0);
                                adjustFarmAnimalAffection(index, 30);
                                gHappiness += adjustValue(gHappiness, 5, MAX_HAPPINESS);
                            }
                            
                        }
                        
                        if (gFarmAnimals[index].flags & FARM_ANIMAL_PREGNANT) {
                            setAnimalState(2, index, PREGNANT_COW, 0xFF, 0);
                        }
                        
                        break;

                    case COW_HAPPY:

                        gFarmAnimals[index].conditionCounter++;
                            
                        if (gFarmAnimals[index].conditionCounter == 3) {
                            setAnimalState(2, index, 0xFF, COW_NORMAL, 0);
                        }
                        
                        break;
                    
                    case COW_MAD:
                        
                        if (gFarmAnimals[index].flags & FARM_ANIMAL_FED) {
                            
                            adjustFarmAnimalAffection(index, -8);
                            
                            gFarmAnimals[index].conditionCounter++;
                            
                            if (gFarmAnimals[index].conditionCounter == 3) {
                                setAnimalState(2, index, 0xFF, COW_NORMAL, 0);
                            }
                            
                        }
                        
                        break;

                    case COW_SICK:
                        
                        gFarmAnimals[index].conditionCounter++;
                        
                        if (gFarmAnimals[index].conditionCounter == ANIMAL_DEATH_COUNT) {
                            
                            setAnimalState(2, index, 0xFF, COW_DEAD, 0xFF);

                            gFarmAnimals[index].flags = 0;
                            
                            setLifeEventBit(ANIMAL_DIED);
                            
                            gDeadAnimalName[0] = gFarmAnimals[index].name[0];
                            gDeadAnimalName[1] = gFarmAnimals[index].name[1];
                            gDeadAnimalName[2] = gFarmAnimals[index].name[2];
                            gDeadAnimalName[3] = gFarmAnimals[index].name[3];
                            gDeadAnimalName[4] = gFarmAnimals[index].name[4];
                            gDeadAnimalName[5] = gFarmAnimals[index].name[5];
                            
                        }
                        
                        break;
                    
                }
            
                break;
            
            case PREGNANT_COW:
                
                gFarmAnimals[index].typeCounter++;
                
                if (gFarmAnimals[index].typeCounter == COW_PREGNANCY_DURATION) {
                    
                    bornAnimalIndex = initializeNewFarmAnimal(0, index);
                    
                    if (bornAnimalIndex != 0xFF) {
    
                        gFarmAnimals[bornAnimalIndex].flags |= FARM_ANIMAL_NEWBORN;
                        gFarmAnimals[bornAnimalIndex].affection = gFarmAnimals[index].affection / 2;
                        
                        setAnimalState(2, index, ADULT_COW, 0xFF, 0);
                        setLifeEventBit(FARM_ANIMAL_BORN);
                    
                    } else {
                        gFarmAnimals[index].typeCounter--;
                    }
                    
                }
                
                break;
            
            case BABY_SHEEP:
                
                gFarmAnimals[index].typeCounter++;
                
                if (gFarmAnimals[index].typeCounter == SHEEP_YOUTH_DURATION) {
                    setAnimalState(2, index, ADULT_SHEEP, 0xFF, 0);
                }
    
                break;
            
            case SHEARED_SHEEP:
                
                gFarmAnimals[index].typeCounter++;
                
                if (gFarmAnimals[index].typeCounter == WOOL_REGROW_DURATION) {
                    setAnimalState(2, index, ADULT_SHEEP, 0xFF, 0);
                }
                
                switch (gFarmAnimals[index].condition) {
                    
                    case SHEEP_NORMAL:
                        
                        if (!(gFarmAnimals[index].flags & FARM_ANIMAL_FED)) {
                            
                            adjustFarmAnimalAffection(index, -8);
                            
                            if (!(getRandomNumberInRange(0, 1))) {
                                
                                setAnimalState(2, index, 0xFF, SHEEP_SICK, 0);
                                adjustFarmAnimalAffection(index, -30);
                                
                                gHappiness += adjustValue(gHappiness, -10, MAX_HAPPINESS);
                                
                            }
                            
                        }
                        
                        break;
                    
                    case SHEEP_SICK:
                        
                        gFarmAnimals[index].conditionCounter++;
                        
                        if (gFarmAnimals[index].conditionCounter == ANIMAL_DEATH_COUNT) {
                            
                            setAnimalState(2, index, 0xFF, SHEEP_DEAD, 0xFF);
                            gFarmAnimals[index].flags = 0;
                            
                            setLifeEventBit(ANIMAL_DIED);
                            
                            gDeadAnimalName[0] = gFarmAnimals[index].name[0];
                            gDeadAnimalName[1] = gFarmAnimals[index].name[1];
                            gDeadAnimalName[2] = gFarmAnimals[index].name[2];
                            gDeadAnimalName[3] = gFarmAnimals[index].name[3];
                            gDeadAnimalName[4] = gFarmAnimals[index].name[4];
                            gDeadAnimalName[5] = gFarmAnimals[index].name[5];
                            
                        }
                        
                        break;
                    
                }
                
                break;
            
            case ADULT_SHEEP:
                
                switch (gFarmAnimals[index].condition) {
                    
                    case SHEEP_NORMAL:

                        if (!(gFarmAnimals[index].flags & FARM_ANIMAL_FED)) {
                            
                            adjustFarmAnimalAffection(index, -8);
                            
                            if (!(getRandomNumberInRange(0, 1))) {
                                
                                setAnimalState(2, index, 0xFF, SHEEP_SICK, 0);
                                adjustFarmAnimalAffection(index, -30);
                                gHappiness += adjustValue(gHappiness, -10, MAX_HAPPINESS);
                                
                            }
                        }
                        
                        break;
    
                    case SHEEP_SICK:
                        
                        gFarmAnimals[index].conditionCounter++;
                        
                        if (gFarmAnimals[index].conditionCounter == ANIMAL_DEATH_COUNT) {
                            
                            setAnimalState(2, index, 0xFF, SHEEP_DEAD, 0xFF);
                            gFarmAnimals[index].flags = 0;
                            
                            setLifeEventBit(ANIMAL_DIED);
                            
                            gDeadAnimalName[0] = gFarmAnimals[index].name[0];
                            gDeadAnimalName[1] = gFarmAnimals[index].name[1];
                            gDeadAnimalName[2] = gFarmAnimals[index].name[2];
                            gDeadAnimalName[3] = gFarmAnimals[index].name[3];
                            gDeadAnimalName[4] = gFarmAnimals[index].name[4];
                            gDeadAnimalName[5] = gFarmAnimals[index].name[5];
                            
                        }
                        
                        break;
                    
                }
    
                break;
            
        }
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", updateHorseAge);

void updateHorseAge(void) {

    if ((horseInfo.flags & HORSE_ACTIVE) && horseInfo.grown == FALSE) {
        
        horseInfo.age++;
    
        if (horseInfo.age == 20) {
            horseInfo.grown = TRUE;
            setLifeEventBit(0x3C);
        }
    
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", initializeDogEntity);

void initializeDogEntity(void) {
    
    if ((dogInfo.flags & DOG_ACTIVE) && (dogInfo.location == gBaseMapIndex) && !(dogInfo.flags & DOG_HELD)) {
        
        dogInfo.entityIndex = 1;
        
        loadEntity(1, ENTITY_ASSET_PLAYER_DOG, TRUE);
        setEntityCollidable(dogInfo.entityIndex, TRUE);
        setEntityYMovement(dogInfo.entityIndex, TRUE);
        setCameraTrackingEntity(dogInfo.entityIndex, FALSE);
        
        setEntityDirection(dogInfo.entityIndex, convertSpriteToWorldDirection(dogInfo.direction, MAIN_MAP_INDEX));
        setEntityCoordinates(dogInfo.entityIndex, dogInfo.coordinates.x, dogInfo.coordinates.y, dogInfo.coordinates.z);
        
        dogInfo.actionState = 0;
        dogInfo.stateTimer = 0;
        dogInfo.flags |= DOG_ENTITY_LOADED;
        
    }
    
    updateDog();
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", initializeChickenEntity);

void initializeChickenEntity(u8 chickenIndex) {

    if ((gChickens[chickenIndex].flags & CHICKEN_ACTIVE) && (gChickens[chickenIndex].location == gBaseMapIndex) && !(gChickens[chickenIndex].flags & CHICKEN_HELD)) {
    
        gChickens[chickenIndex].entityIndex = chickenIndex + 2;
        
        switch (gChickens[chickenIndex].type) {

            case ADULT_CHICKEN:
                initializeAnimalEntity(chickenIndex + 2, (u16*)ENTITY_SLOTS_2_7_PALETTE, (AnimationFrameMetadata*)ENTITY_SLOTS_2_7_ANIM_METADATA, (u32*)ENTITY_SLOTS_2_7_SPRITESHEET_INDEX, (u32*)ENTITY_SLOTS_2_7_TEXTURE_TO_PALETTE_LOOKUP);
                loadEntity(gChickens[chickenIndex].entityIndex, 0x44, TRUE);
                break;         
            
            case CHICK:
                initializeAnimalEntity(chickenIndex + 2, (u16*)ENTITY_CHICK_PALETTE, (AnimationFrameMetadata*)ENTITY_CHICK_ANIM_METADATA, (u32*)ENTITY_CHICK_SPRITESHEET_INDEX, (u32*)ENTITY_CHICK_TEXTURE_TO_PALETTE_LOOKUP);
                loadEntity(gChickens[chickenIndex].entityIndex, 0x43, TRUE);
                break;
            
            case CHICKEN_EGG:
                initializeAnimalEntity(chickenIndex + 2, (u16*)ENTITY_SLOTS_8_13_PALETTE, (AnimationFrameMetadata*)ENTITY_SLOTS_8_13_ANIM_METADATA, (u32*)ENTITY_SLOTS_8_13_SPRITESHEET_INDEX, (u32*)ENTITY_SLOTS_8_13_TEXTURE_TO_PALETTE_LOOKUP);
                loadEntity(gChickens[chickenIndex].entityIndex, 0x5D, TRUE);
                break;
            
            default:
                break;
            
        }

        setEntityCollidable(gChickens[chickenIndex].entityIndex, TRUE);
        setEntityYMovement(gChickens[chickenIndex].entityIndex, TRUE);
        setCameraTrackingEntity(gChickens[chickenIndex].entityIndex, FALSE);

        setEntityDirection(gChickens[chickenIndex].entityIndex, convertSpriteToWorldDirection(gChickens[chickenIndex].direction, MAIN_MAP_INDEX));        
        setEntityCoordinates(gChickens[chickenIndex].entityIndex, gChickens[chickenIndex].coordinates.x, gChickens[chickenIndex].coordinates.y, gChickens[chickenIndex].coordinates.z);
        
        gChickens[chickenIndex].actionState = 0;
        gChickens[chickenIndex].flags |= CHICKEN_ENTITY_LOADED;
        
    }
    
    updateChicken(chickenIndex);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", initializeFarmAnimalEntity);

void initializeFarmAnimalEntity(u8 index) {

    if ((gFarmAnimals[index].flags & FARM_ANIMAL_ACTIVE) && (gCowFestivalEnteredCowIndex!= index) && (gFarmAnimals[index].location == gBaseMapIndex)) {
        
        gFarmAnimals[index].entityIndex = index + 21;
        
        switch (gFarmAnimals[index].type) {

            case BABY_COW:
                initializeAnimalEntity(gFarmAnimals[index].entityIndex, (u16*)ENTITY_BABY_COW_PALETTE, (AnimationFrameMetadata*)ENTITY_BABY_COW_ANIM_METADATA, (u32*)ENTITY_BABY_COW_SPRITESHEET_INDEX, (u32*)ENTITY_BABY_COW_TEXTURE_TO_PALETTE_LOOKUP);
                loadEntity(gFarmAnimals[index].entityIndex, 0x45, TRUE);
                break;
            case CALF:
                initializeAnimalEntity(gFarmAnimals[index].entityIndex, (u16*)ENTITY_CALF_PALETTE, (AnimationFrameMetadata*)ENTITY_CALF_ANIM_METADATA, (u32*)ENTITY_CALF_SPRITESHEET_INDEX, (u32*)ENTITY_CALF_TEXTURE_TO_PALETTE_LOOKUP);
                loadEntity(gFarmAnimals[index].entityIndex, 0x46, TRUE);
                break;
            case ADULT_COW:
                initializeAnimalEntity(gFarmAnimals[index].entityIndex, (u16*)ENTITY_SLOTS_21_28_PALETTE, (AnimationFrameMetadata*)ENTITY_SLOTS_21_28_ANIM_METADATA, (u32*)ENTITY_SLOTS_21_28_SPRITESHEET_INDEX, (u32*)ENTITY_SLOTS_21_28_TEXTURE_TO_PALETTE_LOOKUP);
                loadEntity(gFarmAnimals[index].entityIndex, 0x47, TRUE);
                break;
            case PREGNANT_COW:
                initializeAnimalEntity(gFarmAnimals[index].entityIndex, (u16*)ENTITY_PREGNANT_COW_PALETTE, (AnimationFrameMetadata*)ENTITY_PREGNANT_COW_ANIM_METADATA, (u32*)ENTITY_PREGNANT_COW_SPRITESHEET_INDEX, (u32*)ENTITY_PREGNANT_COW_TEXTURE_TO_PALETTE_LOOKUP);
                loadEntity(gFarmAnimals[index].entityIndex, 0x57, TRUE);
                break;
            case BABY_SHEEP:
                initializeAnimalEntity(gFarmAnimals[index].entityIndex, (u16*)ENTITY_BABY_SHEEP_PALETTE, (AnimationFrameMetadata*)ENTITY_BABY_SHEEP_ANIM_METADATA, (u32*)ENTITY_BABY_SHEEP_SPRITESHEET_INDEX, (u32*)ENTITY_BABY_SHEEP_TEXTURE_TO_PALETTE_LOOKUP);
                loadEntity(gFarmAnimals[index].entityIndex, 0x48, TRUE);
                break;
            case ADULT_SHEEP:
            case SHEARED_SHEEP:
                initializeAnimalEntity(gFarmAnimals[index].entityIndex, (u16*)ENTITY_ADULT_SHEEP_PALETTE, (AnimationFrameMetadata*)ENTITY_ADULT_SHEEP_ANIM_METADATA, (u32*)ENTITY_ADULT_SHEEP_SPRITESHEET_INDEX, (u32*)ENTITY_ADULT_SHEEP_TEXTURE_TO_PALETTE_LOOKUP);
                loadEntity(gFarmAnimals[index].entityIndex, 0x49, TRUE);
                break;

        }

        if (gFarmAnimals[index].location == BARN) {
            setFarmAnimalLocation(index);
        }
        
        setEntityCollidable(gFarmAnimals[index].entityIndex, TRUE);
        setEntityYMovement(gFarmAnimals[index].entityIndex, TRUE);
        setCameraTrackingEntity(gFarmAnimals[index].entityIndex, FALSE);

        setEntityDirection(gFarmAnimals[index].entityIndex, convertSpriteToWorldDirection(gFarmAnimals[index].direction, MAIN_MAP_INDEX));
        setEntityCoordinates(gFarmAnimals[index].entityIndex, gFarmAnimals[index].coordinates.x, gFarmAnimals[index].coordinates.y, gFarmAnimals[index].coordinates.z);
    
        setEntityHandlesMultipleCollisions(gFarmAnimals[index].entityIndex, FALSE);
    
        gFarmAnimals[index].flags |= FARM_ANIMAL_ENTITY_LOADED;
        gFarmAnimals[index].flags &= ~(FARM_ANIMAL_APPROACHING | FARM_ANIMAL_LINGERING);
        
        if ((gWeather == SUNNY) || !(getLevelFlags(gFarmAnimals[index].location) & LEVEL_OUTDOORS)) {
            gFarmAnimals[index].actionState = 0;
        } else {
            gFarmAnimals[index].actionState = 19;
        }
    
    }
    
    updateFarmAnimal(index);

}

//INCLUDE_ASM("asm/nonmatchings/game/animals", initializeHorseEntity);

void initializeHorseEntity(void) {

    if ((horseInfo.flags & HORSE_ACTIVE) && (horseInfo.location == gBaseMapIndex) && !(horseInfo.flags & 8)) {
        
        horseInfo.entityIndex = 38;
        
        switch (horseInfo.grown) {                      
            case FALSE:
                loadEntity(0x26, ENTITY_ASSET_HORSE_PONY, TRUE);
                break;
            case TRUE:
                loadEntity(0x26, ENTITY_ASSET_HORSE_GROWN, TRUE);
                break;
        }
        
        setEntityCollidable(horseInfo.entityIndex, TRUE);
        setEntityYMovement(horseInfo.entityIndex, TRUE);
        setCameraTrackingEntity(horseInfo.entityIndex, FALSE);
        
        setEntityDirection(horseInfo.entityIndex, convertSpriteToWorldDirection(horseInfo.direction, MAIN_MAP_INDEX));
        setEntityCoordinates(horseInfo.entityIndex, horseInfo.coordinates.x, horseInfo.coordinates.y, horseInfo.coordinates.z);
        
        horseInfo.actionState = 0;
        horseInfo.flags |= HORSE_ENTITY_LOADED;

    }
    
    updateHorse();

}

//INCLUDE_ASM("asm/nonmatchings/game/animals", initializeMiscAnimalEntity);

void initializeMiscAnimalEntity(u8 index, u8 arg1) {

    if ((gMiscAnimals[index].flags & MISC_ANIMAL_ACTIVE) && (index < 7)) {
        
        // initialize entity objects if needed
        switch (gMiscAnimals[index].animalType) {

            case MISC_ANIMAL_PLAYER_DOG:
                initializeAnimalEntity(gMiscAnimals[index].entityIndex, (u16*)ENTITY_SLOTS_1_20_PALETTE, (AnimationFrameMetadata*)ENTITY_SLOTS_1_20_ANIM_METADATA, (u32*)ENTITY_SLOTS_1_20_SPRITESHEET_INDEX, (u32*)ENTITY_SLOTS_1_20_TEXTURE_TO_PALETTE_LOOKUP);
                setEntityPaletteIndex(gMiscAnimals[index].entityIndex, 2);
                break;

            case MISC_ANIMAL_COW:
                initializeAnimalEntity(gMiscAnimals[index].entityIndex, (u16*)ENTITY_SLOTS_21_28_PALETTE, (AnimationFrameMetadata*)ENTITY_SLOTS_21_28_ANIM_METADATA, (u32*)ENTITY_SLOTS_21_28_SPRITESHEET_INDEX, (u32*)ENTITY_SLOTS_21_28_TEXTURE_TO_PALETTE_LOOKUP);
                break;

            default:

                switch (index) {

                    case 0:
                        initializeAnimalEntity(gMiscAnimals[index].entityIndex, (u16*)ENTITY_SLOT_14_PALETTE, (AnimationFrameMetadata*)ENTITY_SLOT_14_ANIM_METADATA, (u32*)ENTITY_SLOT_14_SPRITESHEET_INDEX, (u32*)ENTITY_SLOT_14_TEXTURE_TO_PALETTE_LOOKUP);
                        break;
                    case 1:
                        initializeAnimalEntity(gMiscAnimals[index].entityIndex, (u16*)ENTITY_SLOT_15_PALETTE, (AnimationFrameMetadata*)ENTITY_SLOT_15_ANIM_METADATA, (u32*)ENTITY_SLOT_15_SPRITESHEET_INDEX, (u32*)ENTITY_SLOT_15_TEXTURE_TO_PALETTE_LOOKUP);
                        break;
                    case 2:
                        initializeAnimalEntity(gMiscAnimals[index].entityIndex, (u16*)ENTITY_SLOT_16_PALETTE, (AnimationFrameMetadata*)ENTITY_SLOT_16_ANIM_METADATA, (u32*)ENTITY_SLOT_16_SPRITESHEET_INDEX, (u32*)ENTITY_SLOT_16_TEXTURE_TO_PALETTE_LOOKUP);
                        break;
                    case 3:
                        initializeAnimalEntity(gMiscAnimals[index].entityIndex, (u16*)ENTITY_SLOT_17_PALETTE, (AnimationFrameMetadata*)ENTITY_SLOT_17_ANIM_METADATA, (u32*)ENTITY_SLOT_17_SPRITESHEET_INDEX, (u32*)ENTITY_SLOT_17_TEXTURE_TO_PALETTE_LOOKUP);
                        break;
                    case 4:
                        initializeAnimalEntity(gMiscAnimals[index].entityIndex, (u16*)ENTITY_SLOT_18_PALETTE, (AnimationFrameMetadata*)ENTITY_SLOT_18_ANIM_METADATA, (u32*)ENTITY_SLOT_18_SPRITESHEET_INDEX, (u32*)ENTITY_SLOT_18_TEXTURE_TO_PALETTE_LOOKUP);
                        break;

                }

                // set different palette for horse 2
                if (gMiscAnimals[index].animalType == MISC_ANIMAL_HORSE_2) {
                    setEntityPaletteIndex(gMiscAnimals[index].entityIndex, 0);
                }

                break;

        }
        
        switch (gMiscAnimals[index].animalType) {

            case MISC_ANIMAL_PLAYER_DOG:
                loadEntity(gMiscAnimals[index].entityIndex, ENTITY_ASSET_PLAYER_DOG, TRUE);
                break;
            case MISC_ANIMAL_NPC_DOG:
                loadEntity(gMiscAnimals[index].entityIndex, ENTITY_ASSET_DOG_VILLAGE, TRUE);
                break;
            case MISC_ANIMAL_CAT:
                loadEntity(gMiscAnimals[index].entityIndex, ENTITY_ASSET_CAT, TRUE);
                break;
            case MISC_ANIMAL_HORSE:
            case MISC_ANIMAL_HORSE_2:
                loadEntity(gMiscAnimals[index].entityIndex, ENTITY_ASSET_HORSE_UNBRIDLED, TRUE);
                break;
            case MISC_ANIMAL_SHEEP:
                loadEntity(gMiscAnimals[index].entityIndex, ENTITY_ASSET_SHEEP, TRUE);
                break;
            case MISC_ANIMAL_COW:
                loadEntity(gMiscAnimals[index].entityIndex, ENTITY_ASSET_COW, TRUE);
                break;
            case MISC_ANIMAL_FOX:
                loadEntity(gMiscAnimals[index].entityIndex, ENTITY_ASSET_FOX, TRUE);
                break;
            case MISC_ANIMAL_BUNNY:
                loadEntity(gMiscAnimals[index].entityIndex, ENTITY_ASSET_BUNNY, TRUE);
                break;
            case MISC_ANIMAL_SQUIRREL:
                loadEntity(gMiscAnimals[index].entityIndex, ENTITY_ASSET_SQUIRREL, TRUE);
                break;
            case MISC_ANIMAL_MONKEY:
                loadEntity(gMiscAnimals[index].entityIndex, ENTITY_ASSET_MONKEY, TRUE);
                break;
            case MISC_ANIMAL_SPARROW:
                loadEntity(gMiscAnimals[index].entityIndex, ENTITY_ASSET_BIRD, TRUE);
                setEntityPaletteIndex(gMiscAnimals[index].entityIndex, 0);
                break;
            case MISC_ANIMAL_BIRD:
                loadEntity(gMiscAnimals[index].entityIndex, ENTITY_ASSET_BIRD, TRUE);
                break;
            case MISC_ANIMAL_CRAB:
                loadEntity(gMiscAnimals[index].entityIndex, ENTITY_ASSET_CRAB, TRUE);
                break;
            case MISC_ANIMAL_SNAKE:
                loadEntity(gMiscAnimals[index].entityIndex, ENTITY_ASSET_SNAKE, TRUE);
                break;
            case MISC_ANIMAL_WHITE_BUTTERFLY:
                loadEntity(gMiscAnimals[index].entityIndex, ENTITY_ASSET_WHITE_BUTTERFLY, TRUE);
                break;
            case MISC_ANIMAL_LADYBUG:
                loadEntity(gMiscAnimals[index].entityIndex, ENTITY_ASSET_LADYBUG, TRUE);
                break;
            case MISC_ANIMAL_CICADA:
                loadEntity(gMiscAnimals[index].entityIndex, ENTITY_ASSET_CICADA, TRUE);
                break;
            case MISC_ANIMAL_HORNED_BEETLE:
                loadEntity(gMiscAnimals[index].entityIndex, ENTITY_ASSET_HORNED_BEETLE, TRUE);
                break;
            case MISC_ANIMAL_STAG_BEETLE:
                loadEntity(gMiscAnimals[index].entityIndex, ENTITY_ASSET_STAG_BEETLE, TRUE);
                break;
            case MISC_ANIMAL_DRAGONFLY:
                loadEntity(gMiscAnimals[index].entityIndex, ENTITY_ASSET_DRAGONFLY, TRUE);
                break;
            case MISC_ANIMAL_CRICKET:
                loadEntity(gMiscAnimals[index].entityIndex, ENTITY_ASSET_BELL_RING_CRICKET, TRUE);
                break;
        }

        setEntityCollidable(gMiscAnimals[index].entityIndex, TRUE);
        setEntityYMovement(gMiscAnimals[index].entityIndex, TRUE);
        setCameraTrackingEntity(gMiscAnimals[index].entityIndex, FALSE);
        
        setEntityDirection(gMiscAnimals[index].entityIndex, convertSpriteToWorldDirection(gMiscAnimals[index].direction, MAIN_MAP_INDEX));
        setEntityCoordinates(gMiscAnimals[index].entityIndex, gMiscAnimals[index].coordinates.x, gMiscAnimals[index].coordinates.y, gMiscAnimals[index].coordinates.z);
        setEntityHandlesMultipleCollisions(gMiscAnimals[index].entityIndex, FALSE);
        
        gMiscAnimals[index].actionState = arg1;
        gMiscAnimals[index].flags |= MISC_ANIMAL_ENTITY_LOADED;
    
    }
    
    updateMiscAnimal(index);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", updateDog);

void updateDog(void) {

    Vec3f vec;

    if ((dogInfo.flags & DOG_ACTIVE) && (dogInfo.flags & DOG_ENTITY_LOADED)) {
        
        if (checkEntityAnimationStateChanged(dogInfo.entityIndex) || dogInfo.flags & DOG_COLLISION_WITH_PLAYER) {

            if (dogInfo.stateTimer == 0) {
                updateDogActionState();
            } else {
                dogInfo.stateTimer--;
            }
            
            dogInfo.flags &= ~DOG_COLLISION_WITH_PLAYER;
            
        }
        
        setEntityDirection(dogInfo.entityIndex, convertSpriteToWorldDirection(dogInfo.direction, MAIN_MAP_INDEX));
        vec = getMovementVectorFromDirection(dogInfo.speed, dogInfo.direction, 0.0f);
        setEntityMovementVector(dogInfo.entityIndex, vec.x, vec.y, vec.z, dogInfo.speed);

    } 
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", updateDogActionState);

void updateDogActionState(void) {

    u16 temp;
    
    switch (dogInfo.actionState) {

        case 0:
            
            dogInfo.speed = 0;
            dogInfo.stateTimer = 10;
            dogInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 0);
            
            if (dogInfo.flags & DOG_FOLLOWING) {
                
                dogInfo.direction = calculateAnimalDirectionToPlayer(entities[dogInfo.entityIndex].coordinates.x, entities[dogInfo.entityIndex].coordinates.z, entities[ENTITY_PLAYER].coordinates.x, entities[ENTITY_PLAYER].coordinates.z);
                dogInfo.actionState = 4;
                
            } else {

                if (getRandomNumberInRange(0, 60) < 40) {
                    dogInfo.actionState = 0;
                } else {
    
                    temp = getRandomNumberInRange(0, 20);
                    
                    if (temp < 7) {
                        dogInfo.direction = temp;
                    }
                    
                    temp = getRandomNumberInRange(0, 10);
    
                    if (temp == 0) {
                        dogInfo.actionState = 1;
                    }
                    if (temp == 1) {
                        dogInfo.actionState = 2;
                    }
                    if (temp == 2) {
                        dogInfo.actionState = 3;
                    }
                    if (temp == 3) {
                        dogInfo.actionState = 4;
                    }
                    if (temp == 4) {
                        dogInfo.actionState = 5;
                    }
                    if (temp == 5) {
                        dogInfo.actionState = 6;
                    }
                    if (temp == 6) {
                        dogInfo.actionState = 7;
                    }
                    if (temp == 7) {
                        dogInfo.actionState = 8;
                    }
                    if (temp == 8) {
                        dogInfo.actionState = 10;
                    }
                    if (temp == 9) {
                        dogInfo.actionState = 24;
                    }
                    if (temp == 10) {
                        dogInfo.actionState = 25;
                    }
                    
                }
                    
            } 

            dogInfo.flags |= DOG_STATE_CHANGED;
            
            break;

        case 1:
    
            dogInfo.speed = 1;
            dogInfo.stateTimer = 10;
            dogInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 8);
            
            temp = getRandomNumberInRange(0, 19);
            
            if (temp < 10) {
                dogInfo.actionState = 1;
            } else if (temp >= 15) {
                dogInfo.actionState = 4;
            } else {
                dogInfo.actionState = 0;
            }

            dogInfo.flags |= DOG_STATE_CHANGED;
            
            break;

        case 2:

            dogInfo.speed = 0;
            dogInfo.stateTimer = 0;
            dogInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 0x30);
            dogInfo.actionState = 16;
            
            dogInfo.flags |= DOG_STATE_CHANGED;
            
            break;

        case 3:

            dogInfo.speed = 0;
            dogInfo.stateTimer = 10;
            dogInfo.unk_1B = 0;

            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 0x20);
    
            temp = getRandomNumberInRange(0, 20);
            
            if (temp < 10) {
                
                dogInfo.actionState = 3;
                
            } else {

                if (9 < temp && temp < 12) {
                    dogInfo.actionState = 0;
                }

                if (11 < temp && temp < 14) {
                    dogInfo.actionState = 11;
                }

                if (13 < temp && temp < 16) {
                    dogInfo.actionState = 12;
                }
                
                if (15 < temp && temp < 18) {
                    dogInfo.actionState = 13;
                }
                
                if (17 < temp && temp < 20) {
                    dogInfo.actionState = 14;
                }
               
            }
            
            dogInfo.flags |= DOG_STATE_CHANGED;

            break;
        
        case 4:
            
            dogInfo.speed = 2;
            dogInfo.stateTimer = 0;
            dogInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 0x10);
            
            if (dogInfo.flags & DOG_FOLLOWING) {
                
                dogInfo.direction = calculateAnimalDirectionToPlayer(entities[dogInfo.entityIndex].coordinates.x, entities[dogInfo.entityIndex].coordinates.z, entities[ENTITY_PLAYER].coordinates.x, entities[ENTITY_PLAYER].coordinates.z);
                
                if (!(getRandomNumberInRange(0, 20))) {
                    dogInfo.flags &= ~DOG_FOLLOWING;
                }
                
            } else {

                temp = getRandomNumberInRange(0, 19);

                if (temp < 10) {
                    
                    dogInfo.actionState = 4;
                    
                } else {
    
                    if (9 < temp && temp < 13) {
                        dogInfo.actionState = 0;
                    }
    
                    if (12 < temp && temp < 16) {
                        dogInfo.actionState = 1;
                    }
    
                    if (15 < temp && temp < 19) {
                        dogInfo.actionState = 21;
                    }
                   
                }
            
            }
            
            dogInfo.flags |= DOG_STATE_CHANGED;

            break;

        case 5:
            
            dogInfo.speed = 0;
            dogInfo.stateTimer = 0;
            dogInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 0x90);
            
            if (!getRandomNumberInRange(0, 2)) {
                dogInfo.actionState = 5;
            } else {
                dogInfo.actionState = 3;
            }
    
            dogInfo.flags |= DOG_STATE_CHANGED;

            break;


        case 6:

            dogInfo.speed = 0;
            dogInfo.stateTimer = 0;
            dogInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 0xA0);
            
            if (!getRandomNumberInRange(0, 2)) {
                dogInfo.actionState = 6;
            } else {
                dogInfo.actionState = 0;
            }
    
            dogInfo.flags |= DOG_STATE_CHANGED;
            
            break;

        case 7:

            dogInfo.speed = 0;
            dogInfo.stateTimer = 0;
            dogInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 0xA8);
            
            if (!getRandomNumberInRange(0, 2)) {
                dogInfo.actionState = 7;
            } else {
                dogInfo.actionState = 0;
            }
    
            dogInfo.flags |= DOG_STATE_CHANGED;
            
            break;
        
        case 8:

            dogInfo.speed = 0;
            dogInfo.stateTimer = 0;
            dogInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 0xB0);
            
            if (getRandomNumberInRange(0, 2)) {
                dogInfo.actionState = 0;
            } else {
                dogInfo.actionState = 8;
            }
    
            dogInfo.flags |= DOG_STATE_CHANGED;
            
            break;
        
        case 9:
            
            dogInfo.speed = 0;
            dogInfo.stateTimer = 0;
            dogInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 0xB8);
            
            dogInfo.actionState = 26;
            
            break;
        
        case 10:
            
            dogInfo.speed = 0;
            dogInfo.stateTimer = 10;
            dogInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 0xC8);

            if (!getRandomNumberInRange(0, 2)) {
                dogInfo.actionState = 10;
            } else {
                dogInfo.actionState = 0;
            }

            dogInfo.flags |= DOG_STATE_CHANGED;
            
            break;
                
        case 11:
            
            dogInfo.speed = 0;
            dogInfo.stateTimer = 10;
            dogInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 0x28);

            if (!getRandomNumberInRange(0, 2)) {
                dogInfo.actionState = 11;
            } else {
                dogInfo.actionState = 3;
            }

            dogInfo.flags |= DOG_STATE_CHANGED;
            
            break;
                        
        case 12:
            
            dogInfo.speed = 0;
            dogInfo.stateTimer = 10;
            dogInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 0x50);

            if (!getRandomNumberInRange(0, 2)) {
                dogInfo.actionState = 12;
            } else {
                dogInfo.actionState = 3;
            }

            dogInfo.flags |= DOG_STATE_CHANGED;
            
            break;
                        
        case 13:
            
            dogInfo.speed = 0;
            dogInfo.stateTimer = 0;
            dogInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 0x58);

            if (!getRandomNumberInRange(0, 2)) {
                dogInfo.actionState = 13;
            } else {
                dogInfo.actionState = 3;
            }

            dogInfo.flags |= DOG_STATE_CHANGED;
            
            break;
                        
        case 14:
            
            dogInfo.speed = 0;
            dogInfo.stateTimer = 0;
            dogInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 0x60);

            if (!getRandomNumberInRange(0, 2)) {
                dogInfo.actionState = 14;
            } else {
                dogInfo.actionState = 3;
            }

            dogInfo.flags |= DOG_STATE_CHANGED;
            
            break;

        case 16:
            
            dogInfo.speed = 0;
            dogInfo.stateTimer = 10;
            dogInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 0x28);

            temp = getRandomNumberInRange(0, 14);

            if (temp < 10) {
                dogInfo.actionState = 16;
            }
            if (temp == 10) {
                dogInfo.actionState = 27;
            }
            if (temp == 11) {
                dogInfo.actionState = 17;
            }
            if (temp == 12) {
                dogInfo.actionState = 18;
            }
            if (temp == 13) {
                dogInfo.actionState = 19;
            }
            if (temp == 14) {
                dogInfo.actionState = 20;
            }

            dogInfo.flags |= DOG_STATE_CHANGED;
            
            break;
                        
        case 17:
            
            dogInfo.speed = 0;
            dogInfo.stateTimer = 0;
            dogInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 0x68);

            if (!getRandomNumberInRange(0, 2)) {
                dogInfo.actionState = 17;
            } else {
                dogInfo.actionState = 16;
            }

            dogInfo.flags |= DOG_STATE_CHANGED;
            
            break;
                        
        case 18:
            
            dogInfo.speed = 0;
            dogInfo.stateTimer = 0;
            dogInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 0x70);

            if (!getRandomNumberInRange(0, 2)) {
                dogInfo.actionState = 18;
            } else {
                dogInfo.actionState = 16;
            }

            dogInfo.flags |= DOG_STATE_CHANGED;
            
            break;

        case 19:
            
            dogInfo.speed = 0;
            dogInfo.stateTimer = 0;
            dogInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 0x78);

            if (!getRandomNumberInRange(0, 2)) {
                dogInfo.actionState = 19;
            } else {
                dogInfo.actionState = 16;
            }

            dogInfo.flags |= DOG_STATE_CHANGED;
            
            break;

        
        case 20:
            
            dogInfo.speed = 0;
            dogInfo.stateTimer = 10;
            dogInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 0x80);

            if (!getRandomNumberInRange(0, 2)) {
                dogInfo.actionState = 20;
            } else {
                dogInfo.actionState = 16;
            }

            dogInfo.flags |= DOG_STATE_CHANGED;
            
            break;

        case 21:
            
            dogInfo.speed = 1;
            dogInfo.stateTimer = 0;
            dogInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 0x38);
            
            dogInfo.actionState = 22;
            dogInfo.flags |= DOG_STATE_CHANGED;
            
            break;        
        
        case 22:
            
            dogInfo.speed = 2;
            dogInfo.stateTimer = 0;
            dogInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 0x40);
            
            dogInfo.actionState = 23;
            dogInfo.flags |= DOG_STATE_CHANGED;
            
            break;     
        
        case 23:
            
            dogInfo.speed = 1;
            dogInfo.stateTimer = 0;
            dogInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 0x48);
            
            dogInfo.actionState = 0;
            dogInfo.flags |= DOG_STATE_CHANGED;
            
            break;     
                                
        case 24:
            
            dogInfo.speed = 0;
            dogInfo.stateTimer = 0;
            dogInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 0x88);

            if (!getRandomNumberInRange(0, 2)) {
                dogInfo.actionState = 24;
            } else {
                dogInfo.actionState = 0;
            }

            dogInfo.flags |= DOG_STATE_CHANGED;
            
            break;
                                
        case 25:
            
            dogInfo.speed = 0;
            dogInfo.stateTimer = 0;
            dogInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 0x98);

            if (!getRandomNumberInRange(0, 2)) {
                dogInfo.actionState = 25;
            } else {
                dogInfo.actionState = 0;
            }

            dogInfo.flags |= DOG_STATE_CHANGED;
            
            break;

        case 26:
            
            dogInfo.speed = 0;
            dogInfo.stateTimer = 0;
            dogInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 0xC0);
            
            dogInfo.actionState = 0;
            dogInfo.flags |= DOG_STATE_CHANGED;
            
            break;     
        
        case 27:
            
            dogInfo.speed = 0;
            dogInfo.stateTimer = 0;
            dogInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 0x30);
            
            dogInfo.actionState = 0;
            dogInfo.flags |= DOG_STATE_CHANGED;
            
            break;     
        
        case 32:
            
            dogInfo.speed = 0;
            dogInfo.stateTimer = 0;
            dogInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 0);
            
            dogInfo.flags |= DOG_STATE_CHANGED;
            
            break;     

        case 34:
            dogInfo.speed = 0;
            dogInfo.stateTimer = 0;
            dogInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 0x98);
            
            dogInfo.actionState = 0;
            dogInfo.flags |= DOG_STATE_CHANGED;
            
            playSfx(0x3C);
            
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", updateChicken);

void updateChicken(u8 index) {
    
    Vec3f vec;

    if (gChickens[index].flags & CHICKEN_ACTIVE && gChickens[index].flags & CHICKEN_ENTITY_LOADED) {
    
        if ((checkEntityAnimationStateChanged(gChickens[index].entityIndex) || gChickens[index].flags & CHICKEN_COLLISION_WITH_PLAYER)) {
    
            switch(gChickens[index].type) {
    
                case ADULT_CHICKEN:
    
                    switch(gChickens[index].condition) {
                        case CHICKEN_NORMAL:
                            updateAdultChickenNormal(index);
                            break;
                        case CHICKEN_STARVED:
                            updateAdultChickenStarved(index);
                            break;
                    }
        
                    break;
    
                case CHICK:
                    updateChick(index);
                    break;
        
                case CHICKEN_EGG:
                    updateChickenEgg(index);
                    break;
    
                }

            gChickens[index].flags &= ~CHICKEN_COLLISION_WITH_PLAYER;
    
        }

        setEntityDirection(gChickens[index].entityIndex, convertSpriteToWorldDirection(gChickens[index].direction, MAIN_MAP_INDEX));
        
        vec = getMovementVectorFromDirection(gChickens[index].speed, gChickens[index].direction, 0);
        setEntityMovementVector(gChickens[index].entityIndex, vec.x, vec.y, vec.z, gChickens[index].speed);
    
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/animals", updateAdultChickenNormal);

void updateAdultChickenNormal(u8 index) {

    u16 temp;

    switch (gChickens[index].actionState) {
        
        case 0:
            
            gChickens[index].speed = 0;
            gChickens[index].stateTimer = 0;
            gChickens[index].unk_1B = 0;

            setEntityAnimationWithDirectionChange(gChickens[index].entityIndex, 0);

            if (getRandomNumberInRange(0, 100) < 40) {
                gChickens[index].actionState = 0;
            } else {

                temp = getRandomNumberInRange(0, 10);
                
                if (temp < 7) {
                    gChickens[index].direction = temp;
                }
                
                temp = getRandomNumberInRange(0, 60);
                
                if (temp < 20) {
                    gChickens[index].actionState = 1;
                }
                if (19 < temp && temp < 40) {
                    gChickens[index].actionState = 2;
                }
                if (39 < temp && temp < 60) {
                    gChickens[index].actionState = 3;
                }
            
            }

            gChickens[index].flags |= CHICKEN_STATE_CHANGED;
            
            break;
            
        case 1:
            gChickens[index].speed  = 1;
            gChickens[index].stateTimer = 0;
            gChickens[index].unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(gChickens[index].entityIndex, 8);
            
            if (getRandomNumberInRange(0, 20) < 10) {
                gChickens[index].actionState = 1;
            } else {
                gChickens[index].actionState = 0;
            }
            
            gChickens[index].flags |= CHICKEN_STATE_CHANGED;
            playSfx(0x40);
            
            break;

        case 2:
            gChickens[index].speed = 0;
            gChickens[index].stateTimer = 0;
            gChickens[index].unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(gChickens[index].entityIndex, 0x10);

            gChickens[index].actionState = 0;
            gChickens[index].flags |= CHICKEN_STATE_CHANGED;
            
            break;

        case 3:

            gChickens[index].speed = 0;
            gChickens[index].stateTimer = 0;
            gChickens[index].unk_1B = 0;

            setEntityAnimationWithDirectionChange(gChickens[index].entityIndex, 0x20);
        
            if (getRandomNumberInRange(0,19) < 10) {
                gChickens[index].actionState = 0;      
            } else {
                gChickens[index].actionState = 4;
            }

            gChickens[index].flags |= CHICKEN_STATE_CHANGED;
            
            break;

        case 4:
            gChickens[index].speed = 0;
            gChickens[index].stateTimer = 0;
            gChickens[index].unk_1B = 0;

            setEntityAnimationWithDirectionChange(gChickens[index].entityIndex, 0x18);
            
            if (getRandomNumberInRange(0, 19) < 10) {
                gChickens[index].actionState = 4;
            } else {
                gChickens[index].actionState = 3;
            }
        
            gChickens[index].flags |= CHICKEN_STATE_CHANGED;
            
            break;
        
        case 16:
            gChickens[index].speed = 0;
            gChickens[index].stateTimer = 0;
            gChickens[index].unk_1B = 0;

            setEntityAnimationWithDirectionChange(gChickens[index].entityIndex, 0);
            
            gChickens[index].flags |= CHICKEN_STATE_CHANGED;

            break;

        case 17:
            gChickens[index].speed = 0;
            gChickens[index].stateTimer = 0;
            gChickens[index].unk_1B = 0;

            setEntityAnimationWithDirectionChange(gChickens[index].entityIndex, 0x30);
            
            gChickens[index].actionState = 0;
            gChickens[index].flags |= CHICKEN_STATE_CHANGED;
            
            playSfx(0x41);
            
            break;
        
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/animals", updateAdultChickenStarved);

void updateAdultChickenStarved(u8 chickenIndex) {

    u16 temp;
    
    gChickens[chickenIndex].speed = 0;
    gChickens[chickenIndex].stateTimer = 0;
    gChickens[chickenIndex].unk_1B = 0;

    setEntityAnimationWithDirectionChange(gChickens[chickenIndex].entityIndex, 0x28);

    temp = getRandomNumberInRange(0, 1000);
    
    if (temp < 7) {
        gChickens[chickenIndex].direction = temp;
    }
    
    gChickens[chickenIndex].actionState = 0;
    gChickens[chickenIndex].flags |= CHICKEN_STATE_CHANGED;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", updateChick);

void updateChick(u8 index) {

    u16 temp ;

    switch (gChickens[index].actionState) {

        case 0:
            gChickens[index].speed = 0;
            gChickens[index].stateTimer = 0;
            gChickens[index].unk_1B = 0;

            setEntityAnimationWithDirectionChange(gChickens[index].entityIndex, 0);

            if (getRandomNumberInRange(0, 100) < 40) {
                gChickens[index].actionState = 0;
            } else {
                
                temp = getRandomNumberInRange(0, 10);
                
                if (temp  < 7) {
                    gChickens[index].direction = temp;
                }
                
                temp = getRandomNumberInRange(0, 40);

                if (temp  < 20) {
                    gChickens[index].actionState = 1;
                }
                if (19 < temp  && temp  < 40) {
                    gChickens[index].actionState = 2;
                }
            }
            
            gChickens[index].flags |= CHICKEN_STATE_CHANGED;

            break;

        case 1:
            gChickens[index].speed = 1;
            gChickens[index].stateTimer = 0;
            gChickens[index].unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(gChickens[index].entityIndex, 8);
            
            if (getRandomNumberInRange(0, 20) < 10) {
                gChickens[index].actionState = 1;
            } else {
                gChickens[index].actionState = 0;
            }
            
            gChickens[index].flags |= CHICKEN_STATE_CHANGED;
            
            playSfx(0x42);
            
            break;

        case 2:
            gChickens[index].speed = 0;
            gChickens[index].stateTimer = 0;
            gChickens[index].unk_1B = 0;
        
            setEntityAnimationWithDirectionChange(gChickens[index].entityIndex, 16);
        
            gChickens[index].actionState = 0;
            gChickens[index].flags |= CHICKEN_STATE_CHANGED;
        
            break;

        case 16:
            gChickens[index].speed = 0;
            gChickens[index].stateTimer = 0;
            gChickens[index].unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(gChickens[index].entityIndex, 0);

            gChickens[index].flags |= CHICKEN_STATE_CHANGED;
        
            break;
        
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/animals", updateChickenEgg);

void updateChickenEgg(u8 chickenIndex) {

    switch (gChickens[chickenIndex].actionState) {

        case 0:
        case 16:
            gChickens[chickenIndex].speed = 0;
            gChickens[chickenIndex].stateTimer = 0;
            gChickens[chickenIndex].unk_1B = 0;
            setEntityAnimation(gChickens[chickenIndex].entityIndex, 0x13);
            gChickens[chickenIndex].flags |= CHICKEN_STATE_CHANGED;
            
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", updateFarmAnimal);

void updateFarmAnimal(u8 index) {

    Vec3f vec;
    u8 levelInteractionIndex;

    if ((gFarmAnimals[index].flags & FARM_ANIMAL_ACTIVE) && (gFarmAnimals[index].flags & FARM_ANIMAL_ENTITY_LOADED)) {

        if ((checkEntityAnimationStateChanged(gFarmAnimals[index].entityIndex)) || (gFarmAnimals[index].flags & FARM_ANIMAL_COLLISION_WITH_PLAYER)) {
            
            if (entities[gFarmAnimals[index].entityIndex].unk_5E == 0) {

                if (!(gFarmAnimals[index].flags & FARM_ANIMAL_FOLLOWING)) {

                    gFarmAnimals[index].direction = gPlayer.direction;
                    gFarmAnimals[index].actionState = 1;

                    gFarmAnimals[index].flags &= ~(FARM_ANIMAL_APPROACHING);
                    gFarmAnimals[index].flags |= (FARM_ANIMAL_LINGERING | FARM_ANIMAL_FOLLOWING);

                }

            }
            
            switch (gFarmAnimals[index].type) {

                case BABY_COW:
                    updateBabyCow(index);
                    break;

                case CALF:
                    updateCalf(index);
                    break;

                case ADULT_COW:

                    switch(gFarmAnimals[index].condition) {

                        case COW_NORMAL:
                            updateAdultCowNormal(index);
                            break;

                        case COW_HAPPY:
                            updateAdultCowHappy(index);
                            break;

                        case COW_MAD:
                            updateAdultCowMad(index);
                            break;

                        case COW_SICK:
                            updateAdultCowSick(index);
                            break;

                    }
                    
                    break;

                case PREGNANT_COW:
                    updatePregnantCow(index);
                    break;

                case BABY_SHEEP:
                    updateBabySheep(index);
                    break;

                case ADULT_SHEEP:

                    switch(gFarmAnimals[index].condition) {

                        case SHEEP_NORMAL:
                            updateAdultSheepNormal(index);
                            break;

                        case SHEEP_SICK:
                            updateAdultSheepSick(index);
                            break;

                    }
    
                    break;

                case SHEARED_SHEEP:

                    switch(gFarmAnimals[index].condition) {

                        case SHEEP_NORMAL:
                            updateShearedSheepNormal(index);
                            break;

                        case SHEEP_SICK:
                            updateShearedSheepSick(index);
                            break;

                    }
    
                    break;

                }

            gFarmAnimals[index].flags &= ~(FARM_ANIMAL_FOLLOWING | FARM_ANIMAL_COLLISION_WITH_PLAYER);

        }
       
        setEntityDirection(gFarmAnimals[index].entityIndex, convertSpriteToWorldDirection(gFarmAnimals[index].direction, MAIN_MAP_INDEX));
        
        vec = getMovementVectorFromDirection(gFarmAnimals[index].speed, gFarmAnimals[index].direction, 0.0f);
        setEntityMovementVector(gFarmAnimals[index].entityIndex, vec.x, vec.y, vec.z, gFarmAnimals[index].speed);
       
        levelInteractionIndex = getLevelInteractionIndexFromEntityPosition(gFarmAnimals[index].entityIndex, 0, 32);
        
        switch(gFarmAnimals[index].location) {

            case FARM_SPRING:

                if (levelInteractionIndex == 6) {

                    gFarmAnimals[index].location = BARN;
                    deactivateEntity(gFarmAnimals[index].entityIndex);

                    gFarmAnimals[index].flags &= ~(0x4);
                    activateMapAddition(MAIN_MAP_INDEX, 0x1B, 0);  

                }
                
                break;

            case BARN:

                if (levelInteractionIndex == 1) {

                    gFarmAnimals[index].location = FARM_SPRING;
                    deactivateEntity(gFarmAnimals[index].entityIndex);
                    gFarmAnimals[index].flags &= ~(0x4);

                    gFarmAnimals[index].coordinates.x = -128.0f;
                    gFarmAnimals[index].coordinates.y = 0.0f;
                    gFarmAnimals[index].coordinates.z = -288.0f;

                }
                
                break;

        }

    }

}

//INCLUDE_ASM("asm/nonmatchings/game/animals", updateAdultCowNormal);

void updateAdultCowNormal(u8 index) {
    
    u16 temp;

    switch (gFarmAnimals[index].actionState) {
        
        case 0:
        
            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;
            
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0);
            
            if ((gFarmAnimals[index].flags & FARM_ANIMAL_APPROACHING) || (gFarmAnimals[index].flags & FARM_ANIMAL_LINGERING)) {
                
                gFarmAnimals[index].direction = calculateAnimalDirectionToPlayer(entities[gFarmAnimals[index].entityIndex].coordinates.x, entities[gFarmAnimals[index].entityIndex].coordinates.z, entities[ENTITY_PLAYER].coordinates.x, entities[ENTITY_PLAYER].coordinates.z);
                gFarmAnimals[index].actionState = 1;
                
            } else {
                
                if (getRandomNumberInRange(0, 60) < 40) {
                    gFarmAnimals[index].actionState = 0;
                } else {
                    
                    temp = getRandomNumberInRange(0, 20);
                    
                    if (temp < 7) {
                        gFarmAnimals[index].direction = temp;
                    }
        
                    temp = getRandomNumberInRange(0, 5);
                    
                    if (temp == 0) {
                        gFarmAnimals[index].actionState = 1;
                    }
                    
                    if (temp == 1) {
                        gFarmAnimals[index].actionState = 2;
                    }
                    
                    if (temp == 2) {
                        gFarmAnimals[index].actionState = 3;
                    }
                    
                    if (temp == 3) {
                        gFarmAnimals[index].actionState = 4;
                    }
                    
                    if (temp == 4) {
                        gFarmAnimals[index].actionState = 5;
                    }
                    
                    if (temp == 5) {
                        gFarmAnimals[index].actionState = 6;
                    }
                    
                }
                    
            }
                
            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;
                
            break;
            
        case 1:
            
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;
            gFarmAnimals[index].speed = 1.0f;
            
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 8);
            
            if (gFarmAnimals[index].flags & FARM_ANIMAL_APPROACHING) {
            
                gFarmAnimals[index].flags &= ~FARM_ANIMAL_LINGERING;
                gFarmAnimals[index].direction = calculateAnimalDirectionToPlayer(entities[gFarmAnimals[index].entityIndex].coordinates.x, entities[gFarmAnimals[index].entityIndex].coordinates.z, entities[ENTITY_PLAYER].coordinates.x, entities[ENTITY_PLAYER].coordinates.z);
                
                if (!(getRandomNumberInRange(0, 20))) {
                    gFarmAnimals[index].flags &= ~(FARM_ANIMAL_APPROACHING);
                }
                
            } else if (gFarmAnimals[index].flags & FARM_ANIMAL_LINGERING) {
                
                if (!(getRandomNumberInRange(0, 40))) {
                    gFarmAnimals[index].flags &= ~(FARM_ANIMAL_LINGERING);
                }
                
            } else {
                
                if (getRandomNumberInRange(0, 20) < 10) {
                    gFarmAnimals[index].actionState = 1;
                } else {
                    gFarmAnimals[index].actionState = 0;
                }
            
            }
            
            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;
            
            break;
            
        case 2:
            
            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;
            
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 32);
            
            temp = getRandomNumberInRange(0, 2);
            
            if (temp == 0) {
                gFarmAnimals[index].actionState = 2;
            }
            
            if (temp == 1) {
                gFarmAnimals[index].actionState = 0;
            }
            
            if (temp == 2) {
                gFarmAnimals[index].actionState = 9;
            }
            
            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;
            
            break;
            
        case 3:
            
            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;
            
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x10);
            
            gFarmAnimals[index].actionState = 7;
            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;
            
            break;
            
        case 4:
            
            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;
            
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x38);
            
            gFarmAnimals[index].actionState = 0;
            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;
            
            break;
            
        case 5:
            
            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;
            
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x40);
            
            gFarmAnimals[index].actionState = 0;
            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;
            
            break;
            
        case 6:
            
            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;
            
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x48);
            
            gFarmAnimals[index].actionState = 0;
            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;
            
            break;
            
        case 7:
            
            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;
            
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x18);
            
            temp = getRandomNumberInRange(0, 11);
            
            if (temp < 10) {
                gFarmAnimals[index].actionState = 7;
            }
            if (temp == 10) {
                gFarmAnimals[index].actionState = 8;
            }
            if (temp == 11) {
                gFarmAnimals[index].actionState = 10;
            }
            
            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;
            
            break;
            
        case 8:
            
            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;
            
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x28);
            
            if (getRandomNumberInRange(0, 4) < 4) {
                gFarmAnimals[index].actionState = 8;
            } else {
                gFarmAnimals[index].actionState = 7;
            }
            
            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;
            
            break;
            
        case 9:
            
            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;
            
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x30);
            
            gFarmAnimals[index].actionState = 2;
            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;
            
            break;
            
        case 10:
            
            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;
            
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x10);
            
            gFarmAnimals[index].actionState = 0;
            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;
            
            break;
            
        case 16:
            
            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;
            
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0);
            
            gFarmAnimals[index].flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            gFarmAnimals[index].flags &= ~FARM_ANIMAL_LINGERING;
            
            break;
        
        case 17:
            
            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;
            
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x50);
            
            gFarmAnimals[index].actionState = 0;
            gFarmAnimals[index].flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            gFarmAnimals[index].flags &= ~FARM_ANIMAL_LINGERING;
            
            playSfx(0x36);
            
            break;
            
        case 18:
            
            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;
            
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x58);
            
            gFarmAnimals[index].actionState = 0;
            
            gFarmAnimals[index].flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            gFarmAnimals[index].flags &= ~FARM_ANIMAL_LINGERING;
            
            playSfx(0x37);
            
            break;
            
        case 19:
            
            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;
            
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x60);
            
            gFarmAnimals[index].actionState = 19;
            gFarmAnimals[index].flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            gFarmAnimals[index].flags &= ~FARM_ANIMAL_LINGERING;
            
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", updateAdultCowHappy);

void updateAdultCowHappy(u8 index) {

    u16 temp;
    u16 tempDirection;
    
    switch (gFarmAnimals[index].actionState) {

        case 0:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0xB0);

            if ((gFarmAnimals[index].flags & FARM_ANIMAL_APPROACHING) || (gFarmAnimals[index].flags & FARM_ANIMAL_LINGERING)) {

                gFarmAnimals[index].direction = calculateAnimalDirectionToPlayer(
                    entities[gFarmAnimals[index].entityIndex].coordinates.x,
                    entities[gFarmAnimals[index].entityIndex].coordinates.z,
                    entities[ENTITY_PLAYER].coordinates.x, 
                    entities[ENTITY_PLAYER].coordinates.z);

                gFarmAnimals[index].actionState = 1;
                
            } else {
        
                if (getRandomNumberInRange(0, 60) < 40) {
                
                    gFarmAnimals[index].actionState = 0;

                } else {

                    tempDirection = getRandomNumberInRange(0, 20);

                    if (tempDirection < 7) {
                        gFarmAnimals[index].direction = tempDirection;
                    }

                    temp = getRandomNumberInRange(0, 5);

                    if (temp == 0) {
                        gFarmAnimals[index].actionState = 1;
                    }
                    
                    if (temp == 1) {
                        gFarmAnimals[index].actionState = 2;
                    }

                    if (temp == 2) {
                        gFarmAnimals[index].actionState = 3;
                    }

                    if (temp == 3) {
                        gFarmAnimals[index].actionState = 4;
                    }

                    if (temp == 4) {
                        gFarmAnimals[index].actionState = 5;
                    }

                    if (temp == 5) {
                        gFarmAnimals[index].actionState = 6;
                    }

                }
                
            }
        
            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;
            break;
        
        case 1:

            gFarmAnimals[index].speed = 1.0f;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0xB8);

            if (gFarmAnimals[index].flags & FARM_ANIMAL_APPROACHING) {

                gFarmAnimals[index].flags &= ~FARM_ANIMAL_LINGERING;

                gFarmAnimals[index].direction = calculateAnimalDirectionToPlayer(
                    entities[gFarmAnimals[index].entityIndex].coordinates.x,
                    entities[gFarmAnimals[index].entityIndex].coordinates.z,
                    entities[ENTITY_PLAYER].coordinates.x, 
                    entities[ENTITY_PLAYER].coordinates.z);

                if (getRandomNumberInRange(0, 20) == 0) {
                    gFarmAnimals[index].flags &= ~FARM_ANIMAL_APPROACHING;
                } 
                
            } else if (gFarmAnimals[index].flags & FARM_ANIMAL_LINGERING) {

                 if (getRandomNumberInRange(0, 40) == 0) {
                    gFarmAnimals[index].flags &= ~FARM_ANIMAL_LINGERING;
                } 
                
            } else {

                if (getRandomNumberInRange(0, 20) < 10) {
                    gFarmAnimals[index].actionState = 1;
                } else {
                    gFarmAnimals[index].actionState = 0;
                }
                
            }

            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;            
            break;

        case 2:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0xD0);

            temp = getRandomNumberInRange(0, 2); 
            
            if (temp == 0) {
                gFarmAnimals[index].actionState = 2;
            } 
            if (temp == 1) {
                gFarmAnimals[index].actionState = 0;
            }
            if (temp == 2) {
                gFarmAnimals[index].actionState = 9;
            }

            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;  
            break;

        case 3:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0xC0);
            
            gFarmAnimals[index].actionState = 7;
            
            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;  
            break;

        case 4:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0xE8);

            gFarmAnimals[index].actionState = 0;

            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;  
            break;

        case 5:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0xF0);

            gFarmAnimals[index].actionState = 0;

            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;
            break;

        case 6:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0xF8);

            gFarmAnimals[index].actionState = 0;

            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;
            break;

        case 7:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0xC8);

            temp = getRandomNumberInRange(0, 11);

            if (temp < 10) {
                gFarmAnimals[index].actionState = 7;
            }

            if (temp == 10) {
                gFarmAnimals[index].actionState = 8;
            }

            if (temp == 11) {
                gFarmAnimals[index].actionState = 10;
            }

            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;
            break;

        case 8:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0xD8);

            if (getRandomNumberInRange(0, 4) >= 4) {
                gFarmAnimals[index].actionState = 7;
            } else {
                gFarmAnimals[index].actionState = 8;
            }

            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;
            
            break;

        case 9:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0xE0);

            gFarmAnimals[index].actionState = 2;

            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;
            break;

        case 10:
                    
            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0xC0);

            gFarmAnimals[index].actionState = 0;

            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;
            break;
        
        case 16:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0xB0);

            gFarmAnimals[index].flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            gFarmAnimals[index].flags &= ~(FARM_ANIMAL_LINGERING);
            
            break;
        
        case 17:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x50);

            gFarmAnimals[index].actionState = 0;

            gFarmAnimals[index].flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            gFarmAnimals[index].flags &= ~(FARM_ANIMAL_LINGERING);

            playSfx(0x36);
            
            break;
        
        case 18:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x100);

            gFarmAnimals[index].actionState = 0;

            gFarmAnimals[index].flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            gFarmAnimals[index].flags &= ~(FARM_ANIMAL_LINGERING);

            playSfx(0x37);

            break;
        
        case 19:
            
            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x60);

            gFarmAnimals[index].actionState = 19;

            gFarmAnimals[index].flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            gFarmAnimals[index].flags &= ~(FARM_ANIMAL_LINGERING);
            
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", updateAdultCowMad);

void updateAdultCowMad(u8 index) {

    u16 temp;
    u16 tempDirection;
    
    switch (gFarmAnimals[index].actionState) {

        case 0:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x68);

            temp = getRandomNumberInRange(0, 60);

            if ((gFarmAnimals[index].flags & FARM_ANIMAL_APPROACHING) || (gFarmAnimals[index].flags & FARM_ANIMAL_LINGERING)) {

                gFarmAnimals[index].direction = calculateAnimalDirectionToPlayer(
                    entities[gFarmAnimals[index].entityIndex].coordinates.x,
                    entities[gFarmAnimals[index].entityIndex].coordinates.z,
                    entities[ENTITY_PLAYER].coordinates.x, 
                    entities[ENTITY_PLAYER].coordinates.z);

                gFarmAnimals[index].actionState = 1;
                
            } else {
        
                if (temp < 40) {
                
                    gFarmAnimals[index].actionState = 0;

                } else {

                    tempDirection = getRandomNumberInRange(0, 20);

                    if (tempDirection < 7) {
                        gFarmAnimals[index].direction = tempDirection;
                    }

                    temp = getRandomNumberInRange(0, 5);

                    if (temp == 0) {
                        gFarmAnimals[index].actionState = 1;
                    }
                    
                    if (temp == 1) {
                        gFarmAnimals[index].actionState = 2;
                    }

                    if (temp == 2) {
                        gFarmAnimals[index].actionState = 3;
                    }

                    if (temp == 3) {
                        gFarmAnimals[index].actionState = 4;
                    }

                }
                
            }
        
            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;
            break;

        
        case 1:

            gFarmAnimals[index].speed = 0.5f;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x70);

            if (gFarmAnimals[index].flags & FARM_ANIMAL_APPROACHING) {

                gFarmAnimals[index].flags &= ~FARM_ANIMAL_LINGERING;

                gFarmAnimals[index].direction = calculateAnimalDirectionToPlayer(
                    entities[gFarmAnimals[index].entityIndex].coordinates.x,
                    entities[gFarmAnimals[index].entityIndex].coordinates.z,
                    entities[ENTITY_PLAYER].coordinates.x, 
                    entities[ENTITY_PLAYER].coordinates.z);

                if (getRandomNumberInRange(0, 20) == 0) {
                    gFarmAnimals[index].flags &= ~FARM_ANIMAL_APPROACHING;
                } 
                
            } else if (gFarmAnimals[index].flags & FARM_ANIMAL_LINGERING) {

                 if (getRandomNumberInRange(0, 40) == 0) {
                    gFarmAnimals[index].flags &= ~FARM_ANIMAL_LINGERING;
                } 
                
            } else {

                if (getRandomNumberInRange(0, 20) < 10) {
                    gFarmAnimals[index].actionState = 1;
                } else {
                    gFarmAnimals[index].actionState = 0;
                }
                
            }

            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;            
            break;

        case 2:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x88);

            temp = getRandomNumberInRange(0, 2); 
            
            if (temp == 0) {
                gFarmAnimals[index].actionState = 2;
            } 
            if (temp == 1) {
                gFarmAnimals[index].actionState = 0;
            }
            if (temp == 2) {
                gFarmAnimals[index].actionState = 7;
            }

            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;  
            break;

        case 3:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x78);
            
            gFarmAnimals[index].actionState = 5;
            
            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;  
            break;

        case 4:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0xA8);

            gFarmAnimals[index].actionState = 0;

            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;  
            break;

        case 5:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x80);

            temp = getRandomNumberInRange(0, 11);

            if (temp < 10) {
                gFarmAnimals[index].actionState = 5;
            }

            if (temp == 10) {
                gFarmAnimals[index].actionState = 6;
            }

            if (temp == 11) {
                gFarmAnimals[index].actionState = 8;
            }

            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;
            break;

        case 6:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x90);

            if (getRandomNumberInRange(0, 4) >= 4) {
                gFarmAnimals[index].actionState = 5;
            } else {
                gFarmAnimals[index].actionState = 6;
            }

            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;
            break;

        case 7:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x98);

            gFarmAnimals[index].actionState = 2;

            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;
            break;

        case 8:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x78);

            gFarmAnimals[index].actionState = 0;

            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;
            
            break;

        case 16:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x68);

            gFarmAnimals[index].flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            gFarmAnimals[index].flags &= ~(FARM_ANIMAL_LINGERING);
            
            break;
        
        case 17:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x50);

            gFarmAnimals[index].actionState = 0;

            gFarmAnimals[index].flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            gFarmAnimals[index].flags &= ~(FARM_ANIMAL_LINGERING);

            playSfx(0x36);
            
            break;
        
        case 18:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0xA0);

            gFarmAnimals[index].actionState = 0;

            gFarmAnimals[index].flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            gFarmAnimals[index].flags &= ~(FARM_ANIMAL_LINGERING);

            playSfx(0x37);

            break;
        
        case 19:
            
            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x60);

            gFarmAnimals[index].actionState = 19;

            gFarmAnimals[index].flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            gFarmAnimals[index].flags &= ~(FARM_ANIMAL_LINGERING);
            
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", updateAdultCowSick);

void updateAdultCowSick(u8 index) {

    u16 temp;
    u16 tempDirection;
    
    switch (gFarmAnimals[index].actionState) {

        case 0:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x108);

            if ((gFarmAnimals[index].flags & FARM_ANIMAL_APPROACHING) || (gFarmAnimals[index].flags & FARM_ANIMAL_LINGERING)) {

                gFarmAnimals[index].direction = calculateAnimalDirectionToPlayer(
                    entities[gFarmAnimals[index].entityIndex].coordinates.x,
                    entities[gFarmAnimals[index].entityIndex].coordinates.z,
                    entities[ENTITY_PLAYER].coordinates.x, 
                    entities[ENTITY_PLAYER].coordinates.z);

                gFarmAnimals[index].actionState = 1;
                
            } else {
        
                if (getRandomNumberInRange(0, 60) < 40) {
                
                    gFarmAnimals[index].actionState = 0;

                } else {

                    tempDirection = getRandomNumberInRange(0, 20);

                    if (tempDirection < 7) {
                        gFarmAnimals[index].direction = tempDirection;
                    }

                    temp = getRandomNumberInRange(0, 5);

                    if (temp == 0) {
                        gFarmAnimals[index].actionState = 1;
                    }
                    
                    if (temp == 1) {
                        gFarmAnimals[index].actionState = 2;
                    }

                    if (temp == 2) {
                        gFarmAnimals[index].actionState = 3;
                    }

                    if (temp == 3) {
                        gFarmAnimals[index].actionState = 4;
                    }

                }
                
            }
        
            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;
            break;

        
        case 1:

            gFarmAnimals[index].speed = 0.5f;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x110);

            if (gFarmAnimals[index].flags & FARM_ANIMAL_APPROACHING) {

                gFarmAnimals[index].flags &= ~FARM_ANIMAL_LINGERING;

                gFarmAnimals[index].direction = calculateAnimalDirectionToPlayer(
                    entities[gFarmAnimals[index].entityIndex].coordinates.x,
                    entities[gFarmAnimals[index].entityIndex].coordinates.z,
                    entities[ENTITY_PLAYER].coordinates.x, 
                    entities[ENTITY_PLAYER].coordinates.z);

                if (getRandomNumberInRange(0, 20) == 0) {
                    gFarmAnimals[index].flags &= ~FARM_ANIMAL_APPROACHING;
                } 
                
            } else if (gFarmAnimals[index].flags & FARM_ANIMAL_LINGERING) {

                 if (getRandomNumberInRange(0, 40) == 0) {
                    gFarmAnimals[index].flags &= ~FARM_ANIMAL_LINGERING;
                } 
                
            } else {

                if (getRandomNumberInRange(0, 20) < 10) {
                    gFarmAnimals[index].actionState = 1;
                } else {
                    gFarmAnimals[index].actionState = 0;
                }
                
            }

            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;            
            break;

        case 2:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x128);

            temp = getRandomNumberInRange(0, 2); 
            
            if (temp == 0) {
                gFarmAnimals[index].actionState = 2;
            } 
            if (temp == 1) {
                gFarmAnimals[index].actionState = 0;
            }
            if (temp == 2) {
                gFarmAnimals[index].actionState = 6;
            }

            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;  
            break;

        case 3:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x118);
            
            gFarmAnimals[index].actionState = 5;
            
            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;  
            
            break;

        case 4:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x138);

            gFarmAnimals[index].actionState = 0;

            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;  
            break;

        case 5:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x120);

            temp = getRandomNumberInRange(0, 11);

            if (temp < 10) {
                gFarmAnimals[index].actionState = 5;
            }

            if (temp == 10) {
                gFarmAnimals[index].actionState = 6;
            }

            if (temp == 11) {
                gFarmAnimals[index].actionState = 7;
            }

            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;
            break;

        case 6:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x130);

            gFarmAnimals[index].actionState = 2;

            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;
            break;

        case 7:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x118);

            gFarmAnimals[index].actionState = 0;

            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;
            break;

        case 16:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x108);

            gFarmAnimals[index].flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            gFarmAnimals[index].flags &= ~(FARM_ANIMAL_LINGERING);
            
            break;
        
        case 17:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x50);

            gFarmAnimals[index].actionState = 0;

            gFarmAnimals[index].flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            gFarmAnimals[index].flags &= ~(FARM_ANIMAL_LINGERING);

            playSfx(0x36);
            
            break;
        
        case 18:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x108);

            gFarmAnimals[index].actionState = 0;

            gFarmAnimals[index].flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            gFarmAnimals[index].flags &= ~(FARM_ANIMAL_LINGERING);

            break;
        
        case 19:
            
            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x60);

            gFarmAnimals[index].actionState = 19;

            gFarmAnimals[index].flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            gFarmAnimals[index].flags &= ~(FARM_ANIMAL_LINGERING);
            
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", updateCalf);

void updateCalf(u8 index) {
    
    u16 temp;
    u16 tempDirection;
    
    switch (gFarmAnimals[index].actionState) {
        
        case 0:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0);

            if ((gFarmAnimals[index].flags & FARM_ANIMAL_APPROACHING) || (gFarmAnimals[index].flags & FARM_ANIMAL_LINGERING)) {

                gFarmAnimals[index].direction = calculateAnimalDirectionToPlayer(
                    entities[gFarmAnimals[index].entityIndex].coordinates.x,
                    entities[gFarmAnimals[index].entityIndex].coordinates.z,
                    entities[ENTITY_PLAYER].coordinates.x, 
                    entities[ENTITY_PLAYER].coordinates.z);

                gFarmAnimals[index].actionState = 1;
                
            } else {
        
                if (getRandomNumberInRange(0, 60) < 40) {
                    gFarmAnimals[index].actionState = 0;
                } else {

                    tempDirection = getRandomNumberInRange(0, 20);

                    if (tempDirection < 7) {
                        gFarmAnimals[index].direction = tempDirection;
                    }

                    temp = getRandomNumberInRange(0, 5);

                    if (temp == 0) {
                        gFarmAnimals[index].actionState = 1;
                    }
                    
                    if (temp == 1) {
                        gFarmAnimals[index].actionState = 2;
                    }

                    if (temp == 2) {
                        gFarmAnimals[index].actionState = 3;
                    }

                    if (temp == 3) {
                        gFarmAnimals[index].actionState = 4;
                    }
                    
                    if (temp == 4) {
                        gFarmAnimals[index].actionState = 5;
                    }

                    if (temp == 5) {
                        gFarmAnimals[index].actionState = 6;
                    }

                }
                
            }
        
            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;
            break;

        
        case 1:

            gFarmAnimals[index].speed = 1.0f;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 8);

            if (gFarmAnimals[index].flags & FARM_ANIMAL_APPROACHING) {

                gFarmAnimals[index].flags &= ~FARM_ANIMAL_LINGERING;

                gFarmAnimals[index].direction = calculateAnimalDirectionToPlayer(
                    entities[gFarmAnimals[index].entityIndex].coordinates.x,
                    entities[gFarmAnimals[index].entityIndex].coordinates.z,
                    entities[ENTITY_PLAYER].coordinates.x, 
                    entities[ENTITY_PLAYER].coordinates.z);

                if (getRandomNumberInRange(0, 20) == 0) {
                    gFarmAnimals[index].flags &= ~FARM_ANIMAL_APPROACHING;
                } 
                
            } else if (gFarmAnimals[index].flags & FARM_ANIMAL_LINGERING) {

                 if (getRandomNumberInRange(0, 40) == 0) {
                    gFarmAnimals[index].flags &= ~FARM_ANIMAL_LINGERING;
                } 
                
            } else {

                if (getRandomNumberInRange(0, 20) < 10) {
                    gFarmAnimals[index].actionState = 1;
                } else {
                    gFarmAnimals[index].actionState = 0;
                }
                
            }

            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;            
            break;

        case 2:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x10);

            temp = getRandomNumberInRange(0, 2); 
            
            if (temp == 0) {
                gFarmAnimals[index].actionState = 2;
            } 
            if (temp == 1) {
                gFarmAnimals[index].actionState = 0;
            }
            if (temp == 2) {
                gFarmAnimals[index].actionState = 8;
            }

            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;  
            break;

        case 3:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x18);

            temp = getRandomNumberInRange(0, 2); 
            
            if (temp == 0) {
                gFarmAnimals[index].actionState = 3;
            } 
            if (temp == 1) {
                gFarmAnimals[index].actionState = 0;
            }
            if (temp == 2) {
                gFarmAnimals[index].actionState = 7;
            }
            
            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;  
            
            break;

        case 4:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x30);

            gFarmAnimals[index].actionState = 0;

            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;  
            break;

        case 5:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x38);

            gFarmAnimals[index].actionState = 0;

            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;
            break;

        case 6:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x40);

            gFarmAnimals[index].actionState = 0;

            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;
            break;

        case 7:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x20);

            if (getRandomNumberInRange(0, 4) >= 4) {
                gFarmAnimals[index].actionState = 3;
            } else {
                gFarmAnimals[index].actionState = 7;
            }

            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;
            break;

        case 8:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x28);

            gFarmAnimals[index].actionState = 2;

            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;
            break;
    
        case 16:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0);

            gFarmAnimals[index].flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            gFarmAnimals[index].flags &= ~(FARM_ANIMAL_LINGERING);
            
            break;
        
        case 17:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x48);

            gFarmAnimals[index].actionState = 0;

            gFarmAnimals[index].flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            gFarmAnimals[index].flags &= ~(FARM_ANIMAL_LINGERING);

            playSfx(0x36);
            
            break;
        
        case 18:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x50);

            gFarmAnimals[index].actionState = 0;

            gFarmAnimals[index].flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            gFarmAnimals[index].flags &= ~(FARM_ANIMAL_LINGERING);

            playSfx(0x37);
            
            break;
        
        case 19:
            
            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x58);

            gFarmAnimals[index].actionState = 19;

            gFarmAnimals[index].flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            gFarmAnimals[index].flags &= ~(FARM_ANIMAL_LINGERING);
            
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", updateBabyCow);

void updateBabyCow(u8 index) {

    u16 temp;
    u16 tempDirection;
    
    switch (gFarmAnimals[index].actionState) {
        
        case 0:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0);

            if ((gFarmAnimals[index].flags & FARM_ANIMAL_APPROACHING) || (gFarmAnimals[index].flags & FARM_ANIMAL_LINGERING)) {

                gFarmAnimals[index].direction = calculateAnimalDirectionToPlayer(
                    entities[gFarmAnimals[index].entityIndex].coordinates.x,
                    entities[gFarmAnimals[index].entityIndex].coordinates.z,
                    entities[ENTITY_PLAYER].coordinates.x, 
                    entities[ENTITY_PLAYER].coordinates.z);

                gFarmAnimals[index].actionState = 1;
                
            } else {
        
                if (getRandomNumberInRange(0, 60) < 40) {
                    gFarmAnimals[index].actionState = 0;
                } else {

                    tempDirection = getRandomNumberInRange(0, 20);

                    if (tempDirection < 7) {
                        gFarmAnimals[index].direction = tempDirection;
                    }

                    temp = getRandomNumberInRange(0, 5);

                    if (temp == 0) {
                        gFarmAnimals[index].actionState = 1;
                    }
                    
                    if (temp == 1) {
                        gFarmAnimals[index].actionState = 2;
                    }

                    if (temp == 2) {
                        gFarmAnimals[index].actionState = 3;
                    }

                    if (temp == 3) {
                        gFarmAnimals[index].actionState = 4;
                    }
                    
                    if (temp == 4) {
                        gFarmAnimals[index].actionState = 5;
                    }

                    if (temp == 5) {
                        gFarmAnimals[index].actionState = 6;
                    }

                }
                
            }
        
            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;
            break;

        
        case 1:

            gFarmAnimals[index].speed = 1.0f;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 8);

            if (gFarmAnimals[index].flags & FARM_ANIMAL_APPROACHING) {

                gFarmAnimals[index].flags &= ~FARM_ANIMAL_LINGERING;

                gFarmAnimals[index].direction = calculateAnimalDirectionToPlayer(
                    entities[gFarmAnimals[index].entityIndex].coordinates.x,
                    entities[gFarmAnimals[index].entityIndex].coordinates.z,
                    entities[ENTITY_PLAYER].coordinates.x, 
                    entities[ENTITY_PLAYER].coordinates.z);

                if (getRandomNumberInRange(0, 20) == 0) {
                    gFarmAnimals[index].flags &= ~FARM_ANIMAL_APPROACHING;
                } 
                
            } else if (gFarmAnimals[index].flags & FARM_ANIMAL_LINGERING) {

                 if (getRandomNumberInRange(0, 40) == 0) {
                    gFarmAnimals[index].flags &= ~FARM_ANIMAL_LINGERING;
                } 
                
            } else {

                if (getRandomNumberInRange(0, 20) < 10) {
                    gFarmAnimals[index].actionState = 1;
                } else {
                    gFarmAnimals[index].actionState = 0;
                }
                
            }

            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;            
            break;

        case 2:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x10);

            temp = getRandomNumberInRange(0, 2); 
            
            if (temp == 0) {
                gFarmAnimals[index].actionState = 2;
            } 
            if (temp == 1) {
                gFarmAnimals[index].actionState = 0;
            }
            if (temp == 2) {
                gFarmAnimals[index].actionState = 8;
            }

            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;  
            break;

        case 3:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x18);

            temp = getRandomNumberInRange(0, 2); 
            
            if (temp == 0) {
                gFarmAnimals[index].actionState = 3;
            } 
            if (temp == 1) {
                gFarmAnimals[index].actionState = 0;
            }
            if (temp == 2) {
                gFarmAnimals[index].actionState = 7;
            }
            
            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;  
            
            break;

        case 4:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x30);

            gFarmAnimals[index].actionState = 0;

            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;  
            break;

        case 5:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x38);

            gFarmAnimals[index].actionState = 0;

            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;
            break;

        case 6:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x40);

            gFarmAnimals[index].actionState = 0;

            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;
            break;

        case 7:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x20);

            if (getRandomNumberInRange(0, 4) >= 4) {
                gFarmAnimals[index].actionState = 3;
            } else {
                gFarmAnimals[index].actionState = 7;
            }

            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;
            break;

        case 8:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x28);

            gFarmAnimals[index].actionState = 2;

            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;
            break;
    
        case 16:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0);

            gFarmAnimals[index].flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            gFarmAnimals[index].flags &= ~(FARM_ANIMAL_LINGERING);
            
            break;
        
        case 17:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x48);

            gFarmAnimals[index].actionState = 0;

            gFarmAnimals[index].flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            gFarmAnimals[index].flags &= ~(FARM_ANIMAL_LINGERING);

            playSfx(0x36);
            
            break;
        
        case 18:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x50);

            gFarmAnimals[index].actionState = 0;

            gFarmAnimals[index].flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            gFarmAnimals[index].flags &= ~(FARM_ANIMAL_LINGERING);

            playSfx(0x37);
            
            break;
        
        case 19:
            
            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x58);

            gFarmAnimals[index].actionState = 19;

            gFarmAnimals[index].flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            gFarmAnimals[index].flags &= ~(FARM_ANIMAL_LINGERING);
            
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", updatePregnantCow);

void updatePregnantCow(u8 index) {

    u16 temp;
    u16 tempDirection;
    
    switch (gFarmAnimals[index].actionState) {
        
        case 0:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0);

            if ((gFarmAnimals[index].flags & FARM_ANIMAL_APPROACHING) || (gFarmAnimals[index].flags & FARM_ANIMAL_LINGERING)) {

                gFarmAnimals[index].direction = calculateAnimalDirectionToPlayer(
                    entities[gFarmAnimals[index].entityIndex].coordinates.x,
                    entities[gFarmAnimals[index].entityIndex].coordinates.z,
                    entities[ENTITY_PLAYER].coordinates.x, 
                    entities[ENTITY_PLAYER].coordinates.z);

                gFarmAnimals[index].actionState = 1;
                
            } else {
        
                if (getRandomNumberInRange(0, 60) < 40) {
                    gFarmAnimals[index].actionState = 0;
                } else {

                    tempDirection = getRandomNumberInRange(0, 20);

                    if (tempDirection < 7) {
                        gFarmAnimals[index].direction = tempDirection;
                    }

                    temp = getRandomNumberInRange(0, 5);

                    if (temp == 0) {
                        gFarmAnimals[index].actionState = 1;
                    }
                    
                    if (temp == 1) {
                        gFarmAnimals[index].actionState = 2;
                    }

                    if (temp == 2) {
                        gFarmAnimals[index].actionState = 3;
                    }

                    if (temp == 3) {
                        gFarmAnimals[index].actionState = 4;
                    }
                    
                    if (temp == 4) {
                        gFarmAnimals[index].actionState = 5;
                    }

                    if (temp == 5) {
                        gFarmAnimals[index].actionState = 6;
                    }

                }
                
            }
        
            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;
            break;
        
        case 1:

            gFarmAnimals[index].speed = 0.5f;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 8);

            if (gFarmAnimals[index].flags & FARM_ANIMAL_APPROACHING) {

                gFarmAnimals[index].flags &= ~FARM_ANIMAL_LINGERING;

                gFarmAnimals[index].direction = calculateAnimalDirectionToPlayer(
                    entities[gFarmAnimals[index].entityIndex].coordinates.x,
                    entities[gFarmAnimals[index].entityIndex].coordinates.z,
                    entities[ENTITY_PLAYER].coordinates.x, 
                    entities[ENTITY_PLAYER].coordinates.z);

                if (getRandomNumberInRange(0, 20) == 0) {
                    gFarmAnimals[index].flags &= ~FARM_ANIMAL_APPROACHING;
                } 
                
            } else if (gFarmAnimals[index].flags & FARM_ANIMAL_LINGERING) {

                 if (getRandomNumberInRange(0, 40) == 0) {
                    gFarmAnimals[index].flags &= ~FARM_ANIMAL_LINGERING;
                } 
                
            } else {

                if (getRandomNumberInRange(0, 20) < 10) {
                    gFarmAnimals[index].actionState = 1;
                } else {
                    gFarmAnimals[index].actionState = 0;
                }
                
            }

            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;            
            break;

        case 2:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x20);

            temp = getRandomNumberInRange(0, 2); 
            
            if (temp == 0) {
                gFarmAnimals[index].actionState = 2;
            } 
            if (temp == 1) {
                gFarmAnimals[index].actionState = 0;
            }
            if (temp == 2) {
                gFarmAnimals[index].actionState = 8;
            }

            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;  
            break;

        case 3:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x10);

            gFarmAnimals[index].actionState = 9;
            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;  
            
            break;

        case 4:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x38);

            gFarmAnimals[index].actionState = 0;

            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;  
            break;

        case 5:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x40);

            gFarmAnimals[index].actionState = 0;

            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;
            break;

        case 6:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x48);

            gFarmAnimals[index].actionState = 0;

            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;
            break;

        case 7:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x28);

            if (getRandomNumberInRange(0, 4) >= 4) {
                gFarmAnimals[index].actionState = 3;
            } else {
                gFarmAnimals[index].actionState = 7;
            }

            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;
            break;

        case 8:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x30);

            gFarmAnimals[index].actionState = 2;

            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;
            break;
        
        case 9:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x18);

            temp = getRandomNumberInRange(0, 2); 
            
            if (temp == 0) {
                gFarmAnimals[index].actionState = 9;
            } 
            if (temp == 1) {
                gFarmAnimals[index].actionState = 3;
            }
            if (temp == 2) {
                gFarmAnimals[index].actionState = 7;
            }
            
            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;
            break;
    
        case 16:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0);

            gFarmAnimals[index].flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            gFarmAnimals[index].flags &= ~(FARM_ANIMAL_LINGERING);
            
            break;
        
        case 17:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x50);

            gFarmAnimals[index].actionState = 0;

            gFarmAnimals[index].flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            gFarmAnimals[index].flags &= ~(FARM_ANIMAL_LINGERING);

            playSfx(0x36);
            
            break;
        
        case 18:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x58);

            gFarmAnimals[index].actionState = 0;

            gFarmAnimals[index].flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            gFarmAnimals[index].flags &= ~(FARM_ANIMAL_LINGERING);

            playSfx(0x37);
            
            break;
        
        case 19:
            
            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x60);

            gFarmAnimals[index].actionState = 19;

            gFarmAnimals[index].flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            gFarmAnimals[index].flags &= ~(FARM_ANIMAL_LINGERING);
            
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", updateAdultSheepNormal);

void updateAdultSheepNormal(u8 index) {

    u16 temp;
    u16 tempDirection;
    
    switch (gFarmAnimals[index].actionState) {
        
        case 0:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0);

            if ((gFarmAnimals[index].flags & FARM_ANIMAL_APPROACHING) || (gFarmAnimals[index].flags & FARM_ANIMAL_LINGERING)) {

                gFarmAnimals[index].direction = calculateAnimalDirectionToPlayer(
                    entities[gFarmAnimals[index].entityIndex].coordinates.x,
                    entities[gFarmAnimals[index].entityIndex].coordinates.z,
                    entities[ENTITY_PLAYER].coordinates.x, 
                    entities[ENTITY_PLAYER].coordinates.z);

                gFarmAnimals[index].actionState = 1;
                
            } else {
        
                if (getRandomNumberInRange(0, 60) < 40) {
                    gFarmAnimals[index].actionState = 0;
                } else {

                    tempDirection = getRandomNumberInRange(0, 20);

                    if (tempDirection < 7) {
                        gFarmAnimals[index].direction = tempDirection;
                    }

                    temp = getRandomNumberInRange(0, 2);

                    if (temp == 0) {
                        gFarmAnimals[index].actionState = 1;
                    }
                    
                    if (temp == 1) {
                        gFarmAnimals[index].actionState = 2;
                    }

                    if (temp == 2) {
                        gFarmAnimals[index].actionState = 3;
                    }

                }
                
            }
        
            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;
            break;

        case 1:

            gFarmAnimals[index].speed = 1.0f;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 8);

            if (gFarmAnimals[index].flags & FARM_ANIMAL_APPROACHING) {

                gFarmAnimals[index].flags &= ~FARM_ANIMAL_LINGERING;

                gFarmAnimals[index].direction = calculateAnimalDirectionToPlayer(
                    entities[gFarmAnimals[index].entityIndex].coordinates.x,
                    entities[gFarmAnimals[index].entityIndex].coordinates.z,
                    entities[ENTITY_PLAYER].coordinates.x, 
                    entities[ENTITY_PLAYER].coordinates.z);

                if (getRandomNumberInRange(0, 20) == 0) {
                    gFarmAnimals[index].flags &= ~FARM_ANIMAL_APPROACHING;
                } 
                
            } else if (gFarmAnimals[index].flags & FARM_ANIMAL_LINGERING) {

                 if (getRandomNumberInRange(0, 40) == 0) {
                    gFarmAnimals[index].flags &= ~FARM_ANIMAL_LINGERING;
                } 
                
            } else {

                if (getRandomNumberInRange(0, 20) < 10) {
                    gFarmAnimals[index].actionState = 1;
                } else {
                    gFarmAnimals[index].actionState = 0;
                }
                
            }

            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;            
            break;

        case 2:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x10);

            if (getRandomNumberInRange(0, 2) == 0) {
                gFarmAnimals[index].actionState = 2;
            } else {
                gFarmAnimals[index].actionState = 0;
            }

            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;  

            break;

        case 3:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x18);

            gFarmAnimals[index].actionState = 4;

            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;  
            
            break;


        case 4:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x20);

            temp = getRandomNumberInRange(0, 11);

            if (temp < 10) {
                gFarmAnimals[index].actionState = 4;
            }

            if (temp == 10) {
                gFarmAnimals[index].actionState = 4;
            }

            if (temp == 11) {
                gFarmAnimals[index].actionState = 6;
            }

            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;  
            break;

        case 5:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x28);

            if (getRandomNumberInRange(0, 4) >= 4) {
                gFarmAnimals[index].actionState = 4;
            } else {
                gFarmAnimals[index].actionState = 5;
            }

            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;
            break;

        case 6:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x18);

            gFarmAnimals[index].actionState = 0;
            
            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;
            break;
    
        case 16:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0);

            gFarmAnimals[index].flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            gFarmAnimals[index].flags &= ~(FARM_ANIMAL_LINGERING);
            
            break;
        
        case 17:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0);

            gFarmAnimals[index].actionState = 0;

            gFarmAnimals[index].flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            gFarmAnimals[index].flags &= ~(FARM_ANIMAL_LINGERING);

            playSfx(0x3A);
            
            break;
        
        case 18:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x30);

            gFarmAnimals[index].actionState = 0;

            gFarmAnimals[index].flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            gFarmAnimals[index].flags &= ~(FARM_ANIMAL_LINGERING);

            playSfx(0x3A);
            
            break;
        
        case 19:
            
            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x30);

            gFarmAnimals[index].actionState = 19;

            gFarmAnimals[index].flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            gFarmAnimals[index].flags &= ~(FARM_ANIMAL_LINGERING);
            
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", updateAdultSheepSick);

void updateAdultSheepSick(u8 index) {

    switch (gFarmAnimals[index].actionState) {
        
        case 0:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x28);

            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;
            
            break;
    
        case 16:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0);

            gFarmAnimals[index].flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            gFarmAnimals[index].flags &= ~(FARM_ANIMAL_LINGERING);
            
            break;
        
        case 17:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0);

            gFarmAnimals[index].actionState = 0;

            gFarmAnimals[index].flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            gFarmAnimals[index].flags &= ~(FARM_ANIMAL_LINGERING);
            
            break;
        
        case 18:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x30);

            gFarmAnimals[index].actionState = 0;

            gFarmAnimals[index].flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            gFarmAnimals[index].flags &= ~(FARM_ANIMAL_LINGERING);
            
            break;
        
        case 19:
            
            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x30);

            gFarmAnimals[index].actionState = 19;

            gFarmAnimals[index].flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            gFarmAnimals[index].flags &= ~(FARM_ANIMAL_LINGERING);
            
            break;
    
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", updateShearedSheepNormal);

void updateShearedSheepNormal(u8 index) {

    u16 temp;

    switch(gFarmAnimals[index].actionState) {
        
        case 0:
            
            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x38);

            if (gFarmAnimals[index].flags & FARM_ANIMAL_APPROACHING || gFarmAnimals[index].flags & FARM_ANIMAL_LINGERING) {
                
                gFarmAnimals[index].direction = calculateAnimalDirectionToPlayer(entities[gFarmAnimals[index].entityIndex].coordinates.x, entities[gFarmAnimals[index].entityIndex].coordinates.z, entities[ENTITY_PLAYER].coordinates.x, entities[ENTITY_PLAYER].coordinates.z);
                gFarmAnimals[index].actionState = 1;
            
            } else if (getRandomNumberInRange(0, 60) < 40) {
            
                gFarmAnimals[index].actionState = 0;
                    
            } else {
                
                temp = getRandomNumberInRange(0, 20);
                
                if (temp < 7) {
                    gFarmAnimals[index].direction = temp;
                }

                temp = getRandomNumberInRange(0, 2);

                if (temp == 0) {
                    gFarmAnimals[index].actionState = 1;
                } 
                
                if (temp == 1) {
                    gFarmAnimals[index].actionState = 2;
                } 
                
                if (temp == 2) {
                    gFarmAnimals[index].actionState = 3;
                }
                
            }

            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;
            
            break;

        case 1:
            
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;
            gFarmAnimals[index].speed = 1.0f;
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x40);

            if (gFarmAnimals[index].flags & FARM_ANIMAL_APPROACHING) {

                gFarmAnimals[index].flags &= ~FARM_ANIMAL_LINGERING;
                gFarmAnimals[index].direction = calculateAnimalDirectionToPlayer(entities[gFarmAnimals[index].entityIndex].coordinates.x, entities[gFarmAnimals[index].entityIndex].coordinates.z, entities[ENTITY_PLAYER].coordinates.x, entities[ENTITY_PLAYER].coordinates.z);

                if (!getRandomNumberInRange(0, 20)) {
                    gFarmAnimals[index].flags &= ~FARM_ANIMAL_APPROACHING;
                }
                
            } else if (gFarmAnimals[index].flags & FARM_ANIMAL_LINGERING) {

                if (!getRandomNumberInRange(0, 40)) {
                    gFarmAnimals[index].flags &= ~FARM_ANIMAL_LINGERING;
                }
                
            } else {
                if (getRandomNumberInRange(0, 20) < 10) {
                    gFarmAnimals[index].actionState = 1;
                } else {
                    gFarmAnimals[index].actionState = 0;
                }
            }

            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;
            
            break;

        case 2:
            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x48);
            
            if (!getRandomNumberInRange(0, 2)) {
                gFarmAnimals[index].actionState = 2;
            } else {
                gFarmAnimals[index].actionState = 0;
            }
            
            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;
        
            break;

        case 3:
            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x50);
            gFarmAnimals[index].actionState = 4;
            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;
            break;

        case 4:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x58);

            temp = getRandomNumberInRange(0, 11);

            if (temp < 10) {
                gFarmAnimals[index].actionState = 4;
            } 
            
            if (temp == 10) {
                gFarmAnimals[index].actionState = 4;
            } 
            
            if (temp == 11) {
                gFarmAnimals[index].actionState = 6;
            }

            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;
            
            break;

        case 5:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x60);

            if (getRandomNumberInRange(0, 4) >= 4) {
                gFarmAnimals[index].actionState = 4;
            } else {
                gFarmAnimals[index].actionState = 5;
            }

            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;

            break;

        case 6:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x50);
            gFarmAnimals[index].actionState = 0;
            gFarmAnimals[index].flags |= FARM_ANIMAL_STATE_CHANGED;
            
            break;

        case 16:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x38);
            gFarmAnimals[index].flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            gFarmAnimals[index].flags &= ~FARM_ANIMAL_LINGERING;
            break;

        case 17:

            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x38);
            gFarmAnimals[index].actionState = 0;
            gFarmAnimals[index].flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            gFarmAnimals[index].flags &= ~FARM_ANIMAL_LINGERING;
            playSfx(0x3A);
        
            break;

        case 18:
            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x68);
            gFarmAnimals[index].actionState = 0;
            gFarmAnimals[index].flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            gFarmAnimals[index].flags &= ~FARM_ANIMAL_LINGERING;
            playSfx(0x3A);
            break;

        case 19:
            gFarmAnimals[index].speed = 0;
            gFarmAnimals[index].stateTimer = 0;
            gFarmAnimals[index].unk_1E = 0;
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x68);
            gFarmAnimals[index].actionState = 19;
            gFarmAnimals[index].flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            gFarmAnimals[index].flags &= ~FARM_ANIMAL_LINGERING;
            break;

    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", updateShearedSheepSick);

void updateShearedSheepSick(u8 farmAnimalIndex) {

    u16 temp;
    
    switch (gFarmAnimals[farmAnimalIndex].actionState) {
        
        case 0:
            gFarmAnimals[farmAnimalIndex].speed = 0;
            gFarmAnimals[farmAnimalIndex].stateTimer = 0;
            gFarmAnimals[farmAnimalIndex].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[farmAnimalIndex].entityIndex, 0x60);

            gFarmAnimals[farmAnimalIndex].flags |= FARM_ANIMAL_STATE_CHANGED;

            break;

        case 16:    
            gFarmAnimals[farmAnimalIndex].speed = 0;
            gFarmAnimals[farmAnimalIndex].stateTimer = 0;
            gFarmAnimals[farmAnimalIndex].unk_1E = 0;
    
            setEntityAnimationWithDirectionChange(gFarmAnimals[farmAnimalIndex].entityIndex, 0x60);

            gFarmAnimals[farmAnimalIndex].flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            gFarmAnimals[farmAnimalIndex].flags &= ~FARM_ANIMAL_LINGERING;
            
            break;

        case 17:
            gFarmAnimals[farmAnimalIndex].speed = 0;
            gFarmAnimals[farmAnimalIndex].stateTimer = 0;
            gFarmAnimals[farmAnimalIndex].unk_1E = 0;
    
            setEntityAnimationWithDirectionChange(gFarmAnimals[farmAnimalIndex].entityIndex, 0x60);

            gFarmAnimals[farmAnimalIndex].actionState = 0;

            gFarmAnimals[farmAnimalIndex].flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            gFarmAnimals[farmAnimalIndex].flags &= ~FARM_ANIMAL_LINGERING;
            
            break;
        
        case 18:
            gFarmAnimals[farmAnimalIndex].speed = 0;
            gFarmAnimals[farmAnimalIndex].stateTimer = 0;
            gFarmAnimals[farmAnimalIndex].unk_1E = 0;
    
            setEntityAnimationWithDirectionChange(gFarmAnimals[farmAnimalIndex].entityIndex, 0x68);

            gFarmAnimals[farmAnimalIndex].actionState = 0;

            gFarmAnimals[farmAnimalIndex].flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            gFarmAnimals[farmAnimalIndex].flags &= ~FARM_ANIMAL_LINGERING;
            
            break;

        case 19:    
            gFarmAnimals[farmAnimalIndex].speed = 0;
            gFarmAnimals[farmAnimalIndex].stateTimer = 0;
            gFarmAnimals[farmAnimalIndex].unk_1E = 0;
    
            setEntityAnimationWithDirectionChange(gFarmAnimals[farmAnimalIndex].entityIndex, 0x68);

            gFarmAnimals[farmAnimalIndex].actionState = 19;

            gFarmAnimals[farmAnimalIndex].flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            gFarmAnimals[farmAnimalIndex].flags &= ~FARM_ANIMAL_LINGERING;
            
            break;

    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", updateBabySheep);

void updateBabySheep(u8 farmAnimalIndex) {

    u16 temp;
    
    switch (gFarmAnimals[farmAnimalIndex].actionState) {
        
        case 0:
            gFarmAnimals[farmAnimalIndex].speed = 0;
            gFarmAnimals[farmAnimalIndex].stateTimer = 0;
            gFarmAnimals[farmAnimalIndex].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[farmAnimalIndex].entityIndex, 0);

            if (gFarmAnimals[farmAnimalIndex].flags & FARM_ANIMAL_APPROACHING || gFarmAnimals[farmAnimalIndex].flags & FARM_ANIMAL_LINGERING) {

                gFarmAnimals[farmAnimalIndex].direction = calculateAnimalDirectionToPlayer(entities[gFarmAnimals[farmAnimalIndex].entityIndex].coordinates.x, 
                                   entities[gFarmAnimals[farmAnimalIndex].entityIndex].coordinates.z, entities[ENTITY_PLAYER].coordinates.x, entities[ENTITY_PLAYER].coordinates.z);

                gFarmAnimals[farmAnimalIndex].actionState = 1;
                
            } else if (getRandomNumberInRange(0, 60) < 40) {

                gFarmAnimals[farmAnimalIndex].actionState = 0;
                
            } else {

                temp = getRandomNumberInRange(0, 20);

                if (temp < 7) {
                    gFarmAnimals[farmAnimalIndex].direction = temp;
                }

                temp = getRandomNumberInRange(0, 2);

                if (temp == 0) {
                    gFarmAnimals[farmAnimalIndex].actionState = 1;
                }

                if (temp == 1) {
                    gFarmAnimals[farmAnimalIndex].actionState = 2;
                }

                if (temp == 2) {
                    gFarmAnimals[farmAnimalIndex].actionState = 3;
                }
                
            }

            gFarmAnimals[farmAnimalIndex].flags |= FARM_ANIMAL_STATE_CHANGED;

            break;

        case 1:
            gFarmAnimals[farmAnimalIndex].speed = 1.0f;
            gFarmAnimals[farmAnimalIndex].stateTimer = 0;
            gFarmAnimals[farmAnimalIndex].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[farmAnimalIndex].entityIndex, 8);

            if (gFarmAnimals[farmAnimalIndex].flags & FARM_ANIMAL_APPROACHING) {

                gFarmAnimals[farmAnimalIndex].flags &= ~FARM_ANIMAL_LINGERING;

                gFarmAnimals[farmAnimalIndex].direction = calculateAnimalDirectionToPlayer(entities[gFarmAnimals[farmAnimalIndex].entityIndex].coordinates.x, 
                                   entities[gFarmAnimals[farmAnimalIndex].entityIndex].coordinates.z, entities[ENTITY_PLAYER].coordinates.x, entities[ENTITY_PLAYER].coordinates.z);

                if (!getRandomNumberInRange(0, 20)) {
                    gFarmAnimals[farmAnimalIndex].flags &= ~FARM_ANIMAL_APPROACHING;
                }
                
            } else if (gFarmAnimals[farmAnimalIndex].flags & FARM_ANIMAL_LINGERING) {

                if (!getRandomNumberInRange(0, 40)) {
                    gFarmAnimals[farmAnimalIndex].flags &= ~FARM_ANIMAL_LINGERING;
                }
                
            } else {

                if (getRandomNumberInRange(0, 20) < 10) {
                    gFarmAnimals[farmAnimalIndex].actionState = 1;
                } else {
                    gFarmAnimals[farmAnimalIndex].actionState = 0;
                }
                
            }

            gFarmAnimals[farmAnimalIndex].flags |= FARM_ANIMAL_STATE_CHANGED;

            break;

        case 2:    
            gFarmAnimals[farmAnimalIndex].speed = 0;
            gFarmAnimals[farmAnimalIndex].stateTimer = 0;
            gFarmAnimals[farmAnimalIndex].unk_1E = 0;
    
            setEntityAnimationWithDirectionChange(gFarmAnimals[farmAnimalIndex].entityIndex, 0x10);

            if (!getRandomNumberInRange(0, 2)) {
                gFarmAnimals[farmAnimalIndex].actionState = 2;
            } else {
                gFarmAnimals[farmAnimalIndex].actionState = 0;
            }

            gFarmAnimals[farmAnimalIndex].flags |= FARM_ANIMAL_STATE_CHANGED;
            
            break;
        
        case 3:    
            gFarmAnimals[farmAnimalIndex].speed = 0;
            gFarmAnimals[farmAnimalIndex].stateTimer = 0;
            gFarmAnimals[farmAnimalIndex].unk_1E = 0;
    
            setEntityAnimationWithDirectionChange(gFarmAnimals[farmAnimalIndex].entityIndex, 0x20);

            gFarmAnimals[farmAnimalIndex].actionState = 4;

            gFarmAnimals[farmAnimalIndex].flags |= FARM_ANIMAL_STATE_CHANGED;
            
            break;
        
        case 4:    
            gFarmAnimals[farmAnimalIndex].speed = 0;
            gFarmAnimals[farmAnimalIndex].stateTimer = 0;
            gFarmAnimals[farmAnimalIndex].unk_1E = 0;
    
            setEntityAnimationWithDirectionChange(gFarmAnimals[farmAnimalIndex].entityIndex, 0x28);

            temp = getRandomNumberInRange(0, 11); 

            if (temp < 10) {
                gFarmAnimals[farmAnimalIndex].actionState = 4;
            } 
            
            if (temp == 10) {
                gFarmAnimals[farmAnimalIndex].actionState = 4;
            } 
            
            if (temp == 11) {
                gFarmAnimals[farmAnimalIndex].actionState = 6;
            } 

            gFarmAnimals[farmAnimalIndex].flags |= FARM_ANIMAL_STATE_CHANGED;
            
            break;
    
        case 5:    
            gFarmAnimals[farmAnimalIndex].speed = 0;
            gFarmAnimals[farmAnimalIndex].stateTimer = 0;
            gFarmAnimals[farmAnimalIndex].unk_1E = 0;
    
            setEntityAnimationWithDirectionChange(gFarmAnimals[farmAnimalIndex].entityIndex, 0x18);

            if (getRandomNumberInRange(0, 4) < 4) {
                gFarmAnimals[farmAnimalIndex].actionState = 5;
            } else {
                gFarmAnimals[farmAnimalIndex].actionState = 4;
            }

            gFarmAnimals[farmAnimalIndex].flags |= FARM_ANIMAL_STATE_CHANGED;
            
            break;

        case 6:    
            gFarmAnimals[farmAnimalIndex].speed = 0;
            gFarmAnimals[farmAnimalIndex].stateTimer = 0;
            gFarmAnimals[farmAnimalIndex].unk_1E = 0;
    
            setEntityAnimationWithDirectionChange(gFarmAnimals[farmAnimalIndex].entityIndex, 0x20);

            gFarmAnimals[farmAnimalIndex].actionState = 0;

            gFarmAnimals[farmAnimalIndex].flags |= FARM_ANIMAL_STATE_CHANGED;
            
            break;

        case 16:    
            gFarmAnimals[farmAnimalIndex].speed = 0;
            gFarmAnimals[farmAnimalIndex].stateTimer = 0;
            gFarmAnimals[farmAnimalIndex].unk_1E = 0;
    
            setEntityAnimationWithDirectionChange(gFarmAnimals[farmAnimalIndex].entityIndex, 0);

            gFarmAnimals[farmAnimalIndex].flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            gFarmAnimals[farmAnimalIndex].flags &= ~FARM_ANIMAL_LINGERING;
            
            break;

        case 17:
        case 18:
            gFarmAnimals[farmAnimalIndex].speed = 0;
            gFarmAnimals[farmAnimalIndex].stateTimer = 0;
            gFarmAnimals[farmAnimalIndex].unk_1E = 0;
    
            setEntityAnimationWithDirectionChange(gFarmAnimals[farmAnimalIndex].entityIndex, 0);

            gFarmAnimals[farmAnimalIndex].actionState = 0;

            gFarmAnimals[farmAnimalIndex].flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            gFarmAnimals[farmAnimalIndex].flags &= ~FARM_ANIMAL_LINGERING;
            playSfx(0x3A);
            
            break;

        case 19:    
            gFarmAnimals[farmAnimalIndex].speed = 0;
            gFarmAnimals[farmAnimalIndex].stateTimer = 0;
            gFarmAnimals[farmAnimalIndex].unk_1E = 0;
    
            setEntityAnimationWithDirectionChange(gFarmAnimals[farmAnimalIndex].entityIndex, 0);

            gFarmAnimals[farmAnimalIndex].actionState = 19;

            gFarmAnimals[farmAnimalIndex].flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            gFarmAnimals[farmAnimalIndex].flags &= ~FARM_ANIMAL_LINGERING;
            
            break;

    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", updateHorse);

void updateHorse(void) {

    Vec3f vec;

    if ((horseInfo.flags & HORSE_ACTIVE) && (horseInfo.flags & HORSE_ENTITY_LOADED)) {

        if (checkEntityAnimationStateChanged(horseInfo.entityIndex) || horseInfo.flags & HORSE_COLLISION_WITH_PLAYER) {
            
            switch (horseInfo.grown) {
                case FALSE:
                    updateHorseNotGrown();
                    break;                    
                case TRUE:
                    updateHorseGrown();
                    break;
            }
            
            horseInfo.flags &= ~(HORSE_COLLISION_WITH_PLAYER  | 0x80);
            
        }
        
        setEntityDirection(horseInfo.entityIndex, convertSpriteToWorldDirection(horseInfo.direction, MAIN_MAP_INDEX));
        vec = getMovementVectorFromDirection(horseInfo.speed, horseInfo.direction, 0.0f);
        setEntityMovementVector(horseInfo.entityIndex, vec.x, vec.y, vec.z, horseInfo.speed);

    } 
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", updateHorseGrown);

void updateHorseGrown(void) {

    u16 tempDirection;
    u16 temp;
    
    switch (horseInfo.actionState) {

        case 0:
            
            horseInfo.speed = 0;
            horseInfo.stateTimer = 0;
            horseInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(horseInfo.entityIndex, 0);
            
            if (horseInfo.flags & HORSE_FOLLOWING) {

                horseInfo.direction = calculateAnimalDirectionToPlayer(entities[horseInfo.entityIndex].coordinates.x, entities[horseInfo.entityIndex].coordinates.z, entities[ENTITY_PLAYER].coordinates.x, entities[ENTITY_PLAYER].coordinates.z);
                horseInfo.actionState = 4;
                
            } else {

                if (getRandomNumberInRange(0, 60) < 40)  {
                    
                    horseInfo.actionState = 0;
                    
                } else {
                        
                    tempDirection = getRandomNumberInRange(0, 20);

                    if (tempDirection < 7) {
                        horseInfo.direction = tempDirection;
                    }

                    temp = getRandomNumberInRange(0, 5);

                    if (temp == 0) {
                        horseInfo.actionState = 1;
                    }
                    
                    if (temp == 1) {
                        horseInfo.actionState = 2;
                    }

                    if (temp == 2) {
                        horseInfo.actionState = 3;
                    }

                    if (temp == 3) {
                        horseInfo.actionState = 4;
                    }

                    if (temp == 4) {
                        horseInfo.actionState = 5;
                    }

                    if (temp == 5) {
                        horseInfo.actionState = 6;
                    }
                    
                }

            } 
            
            horseInfo.flags |= HORSE_STATE_CHANGED;
            break;

        case 1:

            horseInfo.speed = 1;
            horseInfo.stateTimer = 0;
            horseInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(horseInfo.entityIndex, 8);
            
            if (getRandomNumberInRange(0, 20) < 10) {
                horseInfo.actionState = 1;
            } else {
                horseInfo.actionState = 0;
            }

            horseInfo.flags |= HORSE_STATE_CHANGED;
            break;

        case 2:

            horseInfo.speed = 0;
            horseInfo.stateTimer = 0;
            horseInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(horseInfo.entityIndex, 0x20);

            if (getRandomNumberInRange(0, 2) == 0) {
                horseInfo.actionState = 2;
            } else {   
                horseInfo.actionState = 0;    
            }       
            
            horseInfo.flags |= HORSE_STATE_CHANGED;
            break;

        case 3:

            horseInfo.speed = 0;
            horseInfo.stateTimer = 0;
            horseInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(horseInfo.entityIndex, 0x18);
            
            horseInfo.actionState = 7;
            horseInfo.flags |= HORSE_STATE_CHANGED;
            
            break;

        case 4:

            horseInfo.speed = 2;
            horseInfo.stateTimer = 0;
            horseInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(horseInfo.entityIndex, 0x10);

            if (horseInfo.flags & HORSE_FOLLOWING) {
                
                horseInfo.direction = calculateAnimalDirectionToPlayer(entities[horseInfo.entityIndex].coordinates.x, entities[horseInfo.entityIndex].coordinates.z, entities[ENTITY_PLAYER].coordinates.x, entities[ENTITY_PLAYER].coordinates.z);
                
                if (getRandomNumberInRange(0, 20) == 0) {
                    horseInfo.flags &= ~(HORSE_FOLLOWING);
                }
                
            } else {
                
                if (getRandomNumberInRange(0, 19) < 10) {
                    horseInfo.actionState = 4;
                } else {                
                    horseInfo.actionState = 0;    
                }
                
            }
        
            horseInfo.flags |= HORSE_STATE_CHANGED;
            break;

        case 5:

            horseInfo.speed = 0;
            horseInfo.stateTimer = 0;
            horseInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(horseInfo.entityIndex, 0x48);

            if (getRandomNumberInRange(0, 2) == 0) {
                horseInfo.actionState = 5;
            } else {   
                horseInfo.actionState = 0;    
            }
           
            horseInfo.flags |= HORSE_STATE_CHANGED;
            break;

        case 6:

            horseInfo.speed = 0;
            horseInfo.stateTimer = 0;
            horseInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(horseInfo.entityIndex, 0x50);

            if (getRandomNumberInRange(0, 2) == 0) {
                horseInfo.actionState = 6;
            } else {
                horseInfo.actionState = 0;    
            }
           
            horseInfo.flags |= HORSE_STATE_CHANGED;
            break;

        case 7:

            horseInfo.speed = 0;
            horseInfo.stateTimer = 0;
            horseInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(horseInfo.entityIndex, 0x28);

            if (getRandomNumberInRange(0, 2) == 0) {
                horseInfo.actionState = 7;
            } else {
                horseInfo.actionState = 0;    
            }
           
            horseInfo.flags |= HORSE_STATE_CHANGED;
            break;

        case 8:
            
            horseInfo.speed = 0;
            horseInfo.stateTimer = 0;
            horseInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(horseInfo.entityIndex, 0x30);

            if (getRandomNumberInRange(0, 2) == 0) {
                horseInfo.actionState = 8;
            } else {
                horseInfo.actionState = 0;    
            }
           
            horseInfo.flags |= HORSE_STATE_CHANGED;
            break;
        
        
        case 16:
            
            horseInfo.speed = 0;
            horseInfo.stateTimer = 0;
            horseInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(horseInfo.entityIndex, 0);
            
            horseInfo.flags |= (HORSE_STATE_CHANGED | 0x80); 
            horseInfo.flags &= ~(0x40);
            
            break;

        case 17:

            horseInfo.speed = 0;
            horseInfo.stateTimer = 0;
            horseInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(horseInfo.entityIndex, 0x48);
            
            horseInfo.actionState = 0;
            horseInfo.flags |= (HORSE_STATE_CHANGED | 0x80); 
            horseInfo.flags &= ~(0x40);
            
            playSfx(0x39);
            
            break;

        case 18:

            horseInfo.speed = 0;
            horseInfo.stateTimer = 0;
            horseInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(horseInfo.entityIndex, 0);
            
            horseInfo.actionState = 0;
            horseInfo.flags |= (HORSE_STATE_CHANGED | 0x80); 
            horseInfo.flags &= ~(0x40);
            
            playSfx(0x38);
            
            break;

        default:
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", updateHorseNotGrown);

void updateHorseNotGrown(void) {

    u16 tempDirection;
    u16 temp;
    
    switch (horseInfo.actionState) {

        case 0:
            
            horseInfo.speed = 0;
            horseInfo.stateTimer = 0;
            horseInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(horseInfo.entityIndex, 0);
            
            if (horseInfo.flags & HORSE_FOLLOWING) {

                horseInfo.direction = calculateAnimalDirectionToPlayer(entities[horseInfo.entityIndex].coordinates.x, entities[horseInfo.entityIndex].coordinates.z, entities[ENTITY_PLAYER].coordinates.x, entities[ENTITY_PLAYER].coordinates.z);
                horseInfo.actionState = 4;
                
            } else {

                if (getRandomNumberInRange(0, 60) < 40)  {
                    
                    horseInfo.actionState = 0;
                    
                } else {
                        
                    tempDirection = getRandomNumberInRange(0, 20);

                    if (tempDirection < 7) {
                        horseInfo.direction = tempDirection;
                    }

                    temp = getRandomNumberInRange(0, 5);

                    if (temp == 0) {
                        horseInfo.actionState = 1;
                    }
                    
                    if (temp == 1) {
                        horseInfo.actionState = 2;
                    }

                    if (temp == 2) {
                        horseInfo.actionState = 3;
                    }

                    if (temp == 3) {
                        horseInfo.actionState = 4;
                    }

                    if (temp == 4) {
                        horseInfo.actionState = 5;
                    }
                    
                }

            } 
            
            horseInfo.flags |= HORSE_STATE_CHANGED;
            break;

        case 1:

            horseInfo.speed = 1;
            horseInfo.stateTimer = 0;
            horseInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(horseInfo.entityIndex, 8);
            
            if (getRandomNumberInRange(0, 20) < 10) {
                horseInfo.actionState = 1;
            } else {
                horseInfo.actionState = 0;
            }

            horseInfo.flags |= HORSE_STATE_CHANGED;
            break;

        case 2:

            horseInfo.speed = 0;
            horseInfo.stateTimer = 0;
            horseInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(horseInfo.entityIndex, 0x20U);

            if (getRandomNumberInRange(0, 2) == 0) {
                horseInfo.actionState = 2;
            } else {   
                horseInfo.actionState = 0;    
            }       
            
            horseInfo.flags |= HORSE_STATE_CHANGED;
            break;

        case 3:

            horseInfo.speed = 0;
            horseInfo.stateTimer = 0;
            horseInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(horseInfo.entityIndex, 0x18);
            
            horseInfo.actionState = 6;
            horseInfo.flags |= HORSE_STATE_CHANGED;
            
            break;

        case 4:

            horseInfo.speed = 2;
            horseInfo.stateTimer = 0;
            horseInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(horseInfo.entityIndex, 0x10);

            if (horseInfo.flags & HORSE_FOLLOWING) {
                
                horseInfo.direction = calculateAnimalDirectionToPlayer(entities[horseInfo.entityIndex].coordinates.x, entities[horseInfo.entityIndex].coordinates.z, entities[ENTITY_PLAYER].coordinates.x, entities[ENTITY_PLAYER].coordinates.z);
                
                if (getRandomNumberInRange(0, 20) == 0) {
                    horseInfo.flags &= ~HORSE_FOLLOWING;
                }
                
            } else {
                
                if (getRandomNumberInRange(0, 2) == 0) {
                    horseInfo.actionState = 4;
                } else {                
                    horseInfo.actionState = 0;    
                }
                
            }
        
            horseInfo.flags |= HORSE_STATE_CHANGED;
            break;

        case 5:

            horseInfo.speed = 0;
            horseInfo.stateTimer = 0;
            horseInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(horseInfo.entityIndex, 0x30);

            if (getRandomNumberInRange(0, 2) == 0) {
                horseInfo.actionState = 5;
            } else {   
                horseInfo.actionState = 0;    
            }
           
            horseInfo.flags |= HORSE_STATE_CHANGED;
            break;

        case 6:

            horseInfo.speed = 0;
            horseInfo.stateTimer = 0;
            horseInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(horseInfo.entityIndex, 0x28);

            if (getRandomNumberInRange(0, 2) == 0) {
                horseInfo.actionState = 6;
            } else {
                horseInfo.actionState = 0;    
            }
           
            horseInfo.flags |= HORSE_STATE_CHANGED;
            break;
        
        case 16:
            
            horseInfo.speed = 0;
            horseInfo.stateTimer = 0;
            horseInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(horseInfo.entityIndex, 0);
            
            horseInfo.flags |= (HORSE_STATE_CHANGED | 0x80); 
            horseInfo.flags &= ~(0x40);
            
            break;

        case 17:

            horseInfo.speed = 0;
            horseInfo.stateTimer = 0;
            horseInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(horseInfo.entityIndex, 0x30);
            
            horseInfo.actionState = 0;
            horseInfo.flags |= (HORSE_STATE_CHANGED | 0x80); 
            horseInfo.flags &= ~(0x40);
            
            playSfx(0x39);
            
            break;

        case 18:

            horseInfo.speed = 0;
            horseInfo.stateTimer = 0;
            horseInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(horseInfo.entityIndex, 0);
            
            horseInfo.actionState = 0;
            horseInfo.flags |= (HORSE_STATE_CHANGED | 0x80); 
            horseInfo.flags &= ~(0x40);
            
            playSfx(0x38);
            
            break;

        default:
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", updateMiscAnimal);

void updateMiscAnimal(u8 index) {

    Vec3f vec;

    if ((gMiscAnimals[index].flags & MISC_ANIMAL_ACTIVE) && (gMiscAnimals[index].flags & MISC_ANIMAL_ENTITY_LOADED)) {

        if (checkEntityAnimationStateChanged(gMiscAnimals[index].entityIndex) || gMiscAnimals[index].actionState == 0x20) {

            switch (gMiscAnimals[index].animalType) {

                case MISC_ANIMAL_PLAYER_DOG:
                    updateMiscDog(index);
                    break;

                case MISC_ANIMAL_NPC_DOG:
                    updateVillageDog(index);
                    break;

                case MISC_ANIMAL_CAT:
                    updateCat(index);
                    break;

                case MISC_ANIMAL_HORSE:
                case MISC_ANIMAL_HORSE_2:
                    updateRanchHorse(index);
                    break;

                case MISC_ANIMAL_SHEEP:
                    updateWildSheep(index);
                    break;

                case MISC_ANIMAL_COW:
                    updateRanchCow(index);
                    break;
                case MISC_ANIMAL_FOX:
                    updateFox(index);
                    break;
                case MISC_ANIMAL_BUNNY:
                    updateBunny(index);
                    break;
                case MISC_ANIMAL_SQUIRREL:
                    updateSquirrel(index);
                    break;
                case MISC_ANIMAL_MONKEY:
                    updateMonkey(index);
                    break;
                case MISC_ANIMAL_SPARROW:
                    updateSparrow(index);
                    break;
                case MISC_ANIMAL_BIRD:
                    updateBird(index);
                    break;
                case MISC_ANIMAL_CRAB:
                    updateCrab(index);
                    break;
                case MISC_ANIMAL_SNAKE:
                    updateSnake(index);
                    break;
                case MISC_ANIMAL_WHITE_BUTTERFLY:
                    updateWhiteButterfly(index);
                    break;
                case MISC_ANIMAL_LADYBUG:
                    updateLadybug(index);
                    break;
                case MISC_ANIMAL_CICADA:
                    updateCicada(index);
                    break;
                case MISC_ANIMAL_HORNED_BEETLE:
                    updateHornedBeetle(index);
                    break;
                case MISC_ANIMAL_STAG_BEETLE:
                    updateStagBeetle(index);
                    break;
                case MISC_ANIMAL_DRAGONFLY:
                    updateDragonfly(index);
                    break;
                case MISC_ANIMAL_CRICKET:
                    updateCricket(index);
                    break;

            }

        }
         
        setEntityDirection(gMiscAnimals[index].entityIndex, convertSpriteToWorldDirection(gMiscAnimals[index].direction, MAIN_MAP_INDEX));
        
        vec = getMovementVectorFromDirection(gMiscAnimals[index].zDisplacement, gMiscAnimals[index].direction, 0.0f);
        vec.y = gMiscAnimals[index].yDisplacement;
        
        setEntityMovementVector(gMiscAnimals[index].entityIndex, vec.x, vec.y, vec.z, gMiscAnimals[index].zDisplacement);
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", updateMiscDog);

void updateMiscDog(u8 index) {
    
    u16 temp;
    
    switch (gMiscAnimals[index].actionState) {

         case 0:

            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 10;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0);

             if (getRandomNumberInRange(0, 60) < 40) {
                gMiscAnimals[index].actionState = 0;
            } else {
                 
                temp = getRandomNumberInRange(0, 20);

                if (temp < 7) {
                    gMiscAnimals[index].direction = temp;
                }

                temp = getRandomNumberInRange(0, 11);

                if (temp == 0) {
                    gMiscAnimals[index].actionState = 1;
                }
                
                if (temp == 1) {
                    gMiscAnimals[index].actionState = 2;
                }
                
                if (temp == 2) {
                    gMiscAnimals[index].actionState = 3;
                }

                if (temp == 3) {
                    gMiscAnimals[index].actionState = 4;
                }
                 
                if (temp == 4) {
                    gMiscAnimals[index].actionState = 5;
                }
                
                if (temp == 5) {
                    gMiscAnimals[index].actionState = 6;
                }
                
                if (temp == 6) {
                    gMiscAnimals[index].actionState = 7;
                }

                if (temp == 9) {
                    gMiscAnimals[index].actionState = 10;
                }
                 
                if (temp == 10) {
                    gMiscAnimals[index].actionState = 24;
                }
                 
                if (temp == 11) {
                    gMiscAnimals[index].actionState = 25;
                }             
                 
            }

            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
        
            break;

        case 1:
            gMiscAnimals[index].zDisplacement = 1;
            gMiscAnimals[index].timer = 10;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 8);

            temp = getRandomNumberInRange(0, 19);
            
            if (temp < 10) {
                gMiscAnimals[index].actionState = 1;
            } else if (temp < 15) {
                gMiscAnimals[index].actionState = 0;
            } else {
                gMiscAnimals[index].actionState = 4;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;

            break;
        
        case 2:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x30);
            
            gMiscAnimals[index].actionState = 16;
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
        
            break;

        case 3:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 10;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x20);

            temp = getRandomNumberInRange(0, 20);
            
            if (temp < 10) {

                gMiscAnimals[index].actionState = 3;

            } else {

                if (9 < temp && temp < 12) {
                    gMiscAnimals[index].actionState = 0;
                } 
                if (11 < temp && temp < 14) {
                    gMiscAnimals[index].actionState = 11;
                }
                if (13 < temp && temp < 16) {
                    gMiscAnimals[index].actionState = 12;   
                }
                if (15 < temp && temp < 18) {
                    gMiscAnimals[index].actionState = 13;   
                }
                if (17 < temp && temp < 20) {
                    gMiscAnimals[index].actionState = 14;   
                }

            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;

        case 4:
            gMiscAnimals[index].zDisplacement = 2;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x10);

            temp = getRandomNumberInRange(0, 19);
            
            if (temp >= 10) {

                if (9 < temp && temp < 13) {
                    gMiscAnimals[index].actionState = 0;
                } 
                if (12 < temp && temp < 16) {
                    gMiscAnimals[index].actionState = 1;
                }
                if (15 < temp && temp < 19) {
                    gMiscAnimals[index].actionState = 21;   
                }

            } else {
                gMiscAnimals[index].actionState = 4;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;

            break;

        case 5:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x90);

            if (!getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].actionState = 5;
            } else {
                gMiscAnimals[index].actionState = 3;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;

        case 6:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0xA0);

            if (getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].actionState = 0;
            } else {
                gMiscAnimals[index].actionState = 6;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;
        
        case 7:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0xA8);

            if (getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].actionState = 0;
            } else {
                gMiscAnimals[index].actionState = 7;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;

        case 8:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0xB0);

            if (getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].actionState = 0;
            } else {
                gMiscAnimals[index].actionState = 8;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;

        case 9:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0xB8);

            gMiscAnimals[index].actionState = 26;
            
            break;

        case 10:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 10;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0xC8);

            if (!getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].actionState = 10;
            } else {
                gMiscAnimals[index].actionState = 0;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;

        case 11:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 10;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x28);

            if (!getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].actionState = 11;
            } else {
                gMiscAnimals[index].actionState = 3;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;

        case 12:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 10;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x50);

            if (!getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].actionState = 12;
            } else {
                gMiscAnimals[index].actionState = 3;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;

            break;
     
        case 13:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x58);

            if (!getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].actionState = 13;
            } else {
                gMiscAnimals[index].actionState = 3;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;
        
        case 14:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x60);

            if (!getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].actionState = 14;
            } else {
                gMiscAnimals[index].actionState = 3;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;

        case 16:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 10;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x28);

            temp = getRandomNumberInRange(0, 14);
            
            if (temp < 10) {
                gMiscAnimals[index].actionState = 16;
            } 
            if (temp == 10) {
                gMiscAnimals[index].actionState = 27;
            } 
            if (temp == 11) {
                gMiscAnimals[index].actionState = 17;
            }  
            if (temp == 12) {
                gMiscAnimals[index].actionState = 18;
            }
            if (temp == 13) {
                gMiscAnimals[index].actionState = 19;
            }
            if (temp == 14) {
                gMiscAnimals[index].actionState = 20;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;
        
        case 17:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x68);

            if (!getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].actionState = 17;
            } else {
                gMiscAnimals[index].actionState = 16;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;
                
        case 18:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x70);

            if (!getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].actionState = 18;
            } else {
                gMiscAnimals[index].actionState = 16;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;
                
        case 19:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x78);

            if (!getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].actionState = 19;
            } else {
                gMiscAnimals[index].actionState = 16;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;
                
        case 20:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 10;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x80);

            if (!getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].actionState = 20;
            } else {
                gMiscAnimals[index].actionState = 16;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;
                 
        case 21:
            gMiscAnimals[index].zDisplacement = 1;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x38);

            gMiscAnimals[index].actionState = 22;
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;       
                 
        case 22:
            gMiscAnimals[index].zDisplacement = 2;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x40);

            gMiscAnimals[index].actionState = 23;
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;       

        case 23:
            gMiscAnimals[index].zDisplacement = 1;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x48);

            gMiscAnimals[index].actionState = 0;
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;      

        case 24:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x88);

            if (!getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].actionState = 24;
            } else {
                gMiscAnimals[index].actionState = 0;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;      

        case 25:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x98);

            if (!getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].actionState = 25;
            } else {
                gMiscAnimals[index].actionState = 0;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;      

        case 26:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0xC0);

            gMiscAnimals[index].actionState = 0;
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;      

        case 27:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x30);

            gMiscAnimals[index].actionState = 0;
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;      
        
     }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", updateVillageDog);

void updateVillageDog(u8 index) {

    u16 temp;
    
    switch (gMiscAnimals[index].actionState) {

         case 0:

            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0);

             if (getRandomNumberInRange(0, 60) < 40) {
                gMiscAnimals[index].actionState = 0;
            } else {
                 
                temp = getRandomNumberInRange(0, 20);

                if (temp < 7) {
                    gMiscAnimals[index].direction = temp;
                }

                temp = getRandomNumberInRange(0, 8);

                if (temp == 0) {
                    gMiscAnimals[index].actionState = 1;
                }
                
                if (temp == 1) {
                    gMiscAnimals[index].actionState = 2;
                }
                
                if (temp == 2) {
                    gMiscAnimals[index].actionState = 3;
                }

                if (temp == 3) {
                    gMiscAnimals[index].actionState = 4;
                }
                 
                if (temp == 4) {
                    gMiscAnimals[index].actionState = 5;
                }
                
                if (temp == 5) {
                    gMiscAnimals[index].actionState = 6;
                }
                
                if (temp == 6) {
                    gMiscAnimals[index].actionState = 7;
                }

                if (temp == 7) {
                    gMiscAnimals[index].actionState = 8;
                }
                 
                if (temp == 8) {
                    gMiscAnimals[index].actionState = 9;
                }
                 
            }

            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
        
            break;

        case 1:
            gMiscAnimals[index].zDisplacement = 1;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 8);

            temp = getRandomNumberInRange(0, 19);
            
            if (temp < 10) {
                gMiscAnimals[index].actionState = 1;
            } else if (temp < 15) {
                gMiscAnimals[index].actionState = 0;
            } else {
                gMiscAnimals[index].actionState = 4;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;

            break;
        
        case 2:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x30);
            
            gMiscAnimals[index].actionState = 12;
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
        
            break;

        case 3:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x20);

            temp = getRandomNumberInRange(0, 20);
            
            if (temp >= 10) {

                if (9 < temp && temp < 13) {
                    gMiscAnimals[index].actionState = 0;
                } 
                if (13 < temp && temp < 17) {
                    gMiscAnimals[index].actionState = 10;
                }
                if (16 < temp && temp < 20) {
                    gMiscAnimals[index].actionState = 11;   
                }
                
            } else {
                gMiscAnimals[index].actionState = 3;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;

        case 4:
            gMiscAnimals[index].zDisplacement = 2;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x10);

            temp = getRandomNumberInRange(0, 19);
            
            if (temp < 10) {
                gMiscAnimals[index].actionState = 4;
            } else if (temp < 15) {
                gMiscAnimals[index].actionState = 0;
            } else {
                gMiscAnimals[index].actionState = 1;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;

            break;

        case 5:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x50);

            if (!getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].actionState = 5;
            } else {
                gMiscAnimals[index].actionState = 3;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;

        case 6:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x58);

            if (!getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].actionState = 6;
            } else {
                gMiscAnimals[index].actionState = 0;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;
        
        case 7:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x60);

            if (!getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].actionState = 7;
            } else {
                gMiscAnimals[index].actionState = 0;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;

        case 8:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x70);

            if (!getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].actionState = 8;
            } else {
                gMiscAnimals[index].actionState = 0;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;

        case 9:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x68);

            if (!getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].actionState = 9;
            } else {
                gMiscAnimals[index].actionState = 0;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;

        case 10:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x18);

            if (!getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].actionState = 10;
            } else {
                gMiscAnimals[index].actionState = 3;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;

        case 11:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x38);

            if (!getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].actionState = 11;
            } else {
                gMiscAnimals[index].actionState = 3;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;

        case 12:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x28);

            temp = getRandomNumberInRange(0, 12);
            
            if (temp < 10) {
                gMiscAnimals[index].actionState = 12;
            } 
            if (temp == 10) {
                gMiscAnimals[index].actionState = 15;
            } 
            if (temp == 11) {
                gMiscAnimals[index].actionState = 13;
            }  
            if (temp == 12) {
                gMiscAnimals[index].actionState = 14;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;

            break;
     
        case 13:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x40);

            if (!getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].actionState = 13;
            } else {
                gMiscAnimals[index].actionState = 14;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;
        
        case 14:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x48);

            if (!getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].actionState = 14;
            } else {
                gMiscAnimals[index].actionState = 14;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;

        case 15:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x30);

            gMiscAnimals[index].actionState = 0;
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;
        
     }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", updateCat);

void updateCat(u8 index) {
    
    u32 temp;
    u16 temp2;
    u16 temp3;
    u8 tempDirection;
    
    switch (gMiscAnimals[index].actionState) {

        case 0:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0);

            if (getRandomNumberInRange(0, 60) < 40) {
                gMiscAnimals[index].actionState = 0;
            } else {

                temp = getRandomNumberInRange(0, 10);

                if (temp < 7) {
                    temp &= 0xFFFE;
                    temp2 = temp;
                    gMiscAnimals[index].direction = temp2;
                }

                temp = getRandomNumberInRange(0, 4);

                if (temp == 0) {
                    gMiscAnimals[index].actionState = 1;
                }
                
                if (temp == 1) {
                    gMiscAnimals[index].actionState = 2;
                }
                
                if (temp == 2) {
                    gMiscAnimals[index].actionState = 3;
                }

                if (temp == 3) {
                    gMiscAnimals[index].actionState = 4;
                }

            }

            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
        
            break;

        case 1:
            gMiscAnimals[index].zDisplacement = 1;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 8);

            temp2 = getRandomNumberInRange(0, 19);
            
            if (temp2 < 10) {
                gMiscAnimals[index].actionState = 1;
            } else if (temp2 < 15) {
                gMiscAnimals[index].actionState = 0;
            } else {
                gMiscAnimals[index].actionState = 5;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;

            playSfx(0x43);
            break;

        case 2:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x20);

            temp3 = getRandomNumberInRange(0, 20);
            
            if (temp3 >= 10) {

                if (9 < temp3 && temp3 < 13) {
                    gMiscAnimals[index].actionState = 0;
                } 
                if (13 < temp3 && temp3 < 17) {
                    gMiscAnimals[index].actionState = 6;
                }
                if (16 < temp3 && temp3 < 20) {
                    gMiscAnimals[index].actionState = 7;   
                }
                
            } else {
                gMiscAnimals[index].actionState = 2;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;

        case 3:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x18);

            temp = getRandomNumberInRange(0, 19);

            if (temp >= 10) {

                if (temp >= 15) {
                    gMiscAnimals[index].actionState = 2;
                } else {
                    gMiscAnimals[index].actionState = 0;
                }
                
            } else {
                gMiscAnimals[index].actionState = 3;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;
        
        case 4:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimation(gMiscAnimals[index].entityIndex, 0x38);

            if (!getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].actionState = 4;
            } else {
                gMiscAnimals[index].actionState = 0;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;
        
        case 5:
            gMiscAnimals[index].zDisplacement = 1;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x10);

            if (!getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].actionState = 5;
            } else {
                gMiscAnimals[index].actionState = 1;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;
        
       case 6:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x30);

            if (!getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].actionState = 6;
            } else {
                gMiscAnimals[index].actionState = 2;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;     

        case 7:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimation(gMiscAnimals[index].entityIndex, 0x39);

            if (!getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].actionState = 7;
            } else {
                gMiscAnimals[index].actionState = 2;
            }

            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;

    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", updateRanchHorse);

void updateRanchHorse(u8 index) {

    u16 temp;
    
    switch (gMiscAnimals[index].actionState) {

         case 0:

            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0);

             if (getRandomNumberInRange(0, 60) < 56) {
                gMiscAnimals[index].actionState = 0;
            } else {
                 
                temp = getRandomNumberInRange(0, 8);

                if (temp < 7) {
                    gMiscAnimals[index].direction = temp;
                }

                 gMiscAnimals[index].actionState = 1;
                 
            }

            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
        
            break;

        case 1:
            gMiscAnimals[index].zDisplacement = 1;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 8);
            
            if (getRandomNumberInRange(0, 19) < 4) {
                gMiscAnimals[index].actionState = 1;
            } else {
                gMiscAnimals[index].actionState = 0;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
        
            break;

     }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", updateWildSheep);

void updateWildSheep(u8 index) {

    u16 temp;
    u8 tempDirection;
    
    switch (gMiscAnimals[index].actionState) {

        case 0:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0);

            if (getRandomNumberInRange(0, 60) < 40) {
                gMiscAnimals[index].actionState = 0;
            } else {

                temp = getRandomNumberInRange(0, 20);

                if (temp < 7) {
                    gMiscAnimals[index].direction = temp;
                }

                temp = getRandomNumberInRange(0, 2);

                if (temp == 0) {
                    gMiscAnimals[index].actionState = 1;
                }
                
                if (temp == 1) {
                    gMiscAnimals[index].actionState = 2;
                }
                
                if (temp == 2) {
                    gMiscAnimals[index].actionState = 3;
                }

            }

            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
        
            break;

        case 1:
            gMiscAnimals[index].zDisplacement = 1;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 8);

            if (getRandomNumberInRange(0, 20) < 10) {
                gMiscAnimals[index].actionState = 1;
            } else {
                gMiscAnimals[index].actionState = 0;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
        
            break;

        case 2:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x10);

            if (!getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].actionState = 2;
            } else {
                gMiscAnimals[index].actionState = 0;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;

        case 3:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x18);

            gMiscAnimals[index].actionState = 4;
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;
        
        case 4:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x20);

            temp = getRandomNumberInRange(0, 11);
            
            if (temp < 10) {
                gMiscAnimals[index].actionState = 4;
            }
            
            if (temp == 10) {
                gMiscAnimals[index].actionState = 4;
            }
            
            if (temp == 11) {
                gMiscAnimals[index].actionState = 6;
            }

            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;
        
        case 5:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x28);

            if (getRandomNumberInRange(0, 4) < 4) {
                gMiscAnimals[index].actionState = 5;
            } else {
                gMiscAnimals[index].actionState = 4;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;
        
       case 6:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x18);

            gMiscAnimals[index].actionState = 0;
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;     

    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", updateRanchCow);

void updateRanchCow(u8 index) {

    u16 temp;
    u8 tempDirection;
    
    switch (gMiscAnimals[index].actionState) {

        case 0:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0);

            if (getRandomNumberInRange(0, 60) < 40) {
                gMiscAnimals[index].actionState = 0;
            } else {

                temp = getRandomNumberInRange(0, 20);

                if (temp < 7) {
                    gMiscAnimals[index].direction = temp;
                }

                temp = getRandomNumberInRange(0, 5);

                if (temp == 0) {
                    gMiscAnimals[index].actionState = 1;
                }
                
                if (temp == 1) {
                    gMiscAnimals[index].actionState = 2;
                }
                
                if (temp == 2) {
                    gMiscAnimals[index].actionState = 3;
                }
                
                if (temp == 3) {
                    gMiscAnimals[index].actionState = 4;
                }

                if (temp == 4) {
                    gMiscAnimals[index].actionState = 5;
                }
                
                if (temp == 5) {
                    gMiscAnimals[index].actionState = 6;
                }
            }

            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
        
            break;

        case 1:
            gMiscAnimals[index].zDisplacement = 1;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 8);

            if (getRandomNumberInRange(0, 20) < 10) {
                gMiscAnimals[index].actionState = 1;
            } else {
                gMiscAnimals[index].actionState = 0;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
        
            break;

        case 2:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x20);

            temp = getRandomNumberInRange(0, 2);
            
            if (temp == 0) {
                gMiscAnimals[index].actionState = 2;
            }
            
            if (temp == 1) {
                gMiscAnimals[index].actionState = 0;
            }
            
            if (temp == 2) {
                gMiscAnimals[index].actionState = 9;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;

        case 3:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x10);

            gMiscAnimals[index].actionState = 7;
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;
        
        case 4:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x38);

            gMiscAnimals[index].actionState = 0;
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;
        
        case 5:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x40);

            gMiscAnimals[index].actionState = 0;
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;
        
       case 6:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x48);

            gMiscAnimals[index].actionState = 0;
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;     

        case 7:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x18);

            temp = getRandomNumberInRange(0, 11);
            
            if (temp < 10) {
                gMiscAnimals[index].actionState = 7;
            }
            
            if (temp == 10) {
                gMiscAnimals[index].actionState = 8;
            }
            
            if (temp == 11) {
                gMiscAnimals[index].actionState = 10;
            }

            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            
            break;

        case 8:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x28);

            if (getRandomNumberInRange(0, 4) < 4) {
                 gMiscAnimals[index].actionState = 8;
            } else {
                 gMiscAnimals[index].actionState = 7;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;
       
        case 9:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x30);

            gMiscAnimals[index].actionState = 2;
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;

        case 10:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x10);

            gMiscAnimals[index].actionState = 0;
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;


    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", updateFox);

void updateFox(u8 index) {
    
    u16 temp;
    u8 tempDirection;
    
    switch (gMiscAnimals[index].actionState) {

        case 0:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0);

            if (getRandomNumberInRange(0, 60) < 40) {
                
                gMiscAnimals[index].actionState = 0;
                
            } else {
                temp = getRandomNumberInRange(0, 10);

                if (temp < 7) {
                    gMiscAnimals[index].direction = temp;
                }

                temp = getRandomNumberInRange(0, 2);

                if (temp == 0) {
                    gMiscAnimals[index].actionState = 1;
                }
                
                if (temp == 1) {
                    gMiscAnimals[index].actionState = 2;
                }
                
                if (temp == 2) {
                    gMiscAnimals[index].actionState = 3;
                }
                
            }

            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
        
            break;

        case 1:
            gMiscAnimals[index].zDisplacement = 1;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 8);

            if (getRandomNumberInRange(0, 19) < 10) {
                gMiscAnimals[index].actionState = 1;
            } else {
                gMiscAnimals[index].actionState = 0;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
        
            break;

        case 2:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x10);

            if (getRandomNumberInRange(0, 20) < 10) {
                gMiscAnimals[index].actionState = 2;
            } else {
                gMiscAnimals[index].actionState = 0;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;

        case 3:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x18);
            
            if (getRandomNumberInRange(0, 19) < 10) {
                gMiscAnimals[index].actionState = 3;
            } else {
                gMiscAnimals[index].actionState = 0;
            }

            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;

        case 32:
            gMiscAnimals[index].zDisplacement = 4;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 8);

            tempDirection = calculateAnimalDirectionToPlayer(entities[gMiscAnimals[index].entityIndex].coordinates.x, entities[gMiscAnimals[index].entityIndex].coordinates.z, entities[ENTITY_PLAYER].coordinates.x, entities[ENTITY_PLAYER].coordinates.z);

            gMiscAnimals[index].direction = (tempDirection + 4) % 8;
            
            if (!getRandomNumberInRange(0, 30)) {
                gMiscAnimals[index].flags &= ~(0x10);
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            
            break;

    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", updateBunny);

void updateBunny(u8 index) {

    u16 temp;
    u8 tempDirection;
    
    switch (gMiscAnimals[index].actionState) {

        case 0:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0);

            if (getRandomNumberInRange(0, 60) < 40) {
                gMiscAnimals[index].actionState = 0;
            } else {

                temp = getRandomNumberInRange(0, 10);

                if (temp < 7) {
                    gMiscAnimals[index].direction = temp;
                }

                temp = getRandomNumberInRange(0, 2);

                if (temp == 0) {
                    gMiscAnimals[index].actionState = 1;
                }
                
                if (temp == 1) {
                    gMiscAnimals[index].actionState = 2;
                }
                
                if (temp == 2) {
                    gMiscAnimals[index].actionState = 3;
                }
                
            }

            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
        
            break;

        case 1:
            gMiscAnimals[index].zDisplacement = 1;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 8);

            if (getRandomNumberInRange(0, 19) < 10) {
                gMiscAnimals[index].actionState = 1;
            } else {
                gMiscAnimals[index].actionState = 0;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
        
            break;

        case 2:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x10);

            if (getRandomNumberInRange(0, 20) < 10) {
                gMiscAnimals[index].actionState = 2;
            } else {
                gMiscAnimals[index].actionState = 0;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;

        case 3:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;

            if (getRandomNumberInRange(0, 1)) {
                setEntityAnimation(gMiscAnimals[index].entityIndex, 0x18);
            } else {
                setEntityAnimation(gMiscAnimals[index].entityIndex, 0x19);
            }

            gMiscAnimals[index].actionState = 0;
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;

        case 32:
            gMiscAnimals[index].zDisplacement = 4;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 8);

            tempDirection = calculateAnimalDirectionToPlayer(entities[gMiscAnimals[index].entityIndex].coordinates.x, entities[gMiscAnimals[index].entityIndex].coordinates.z, entities[ENTITY_PLAYER].coordinates.x, entities[ENTITY_PLAYER].coordinates.z);

            gMiscAnimals[index].direction = (tempDirection + 4) % 8;
            
            if (!getRandomNumberInRange(0, 30)) {
                gMiscAnimals[index].flags &= ~(0x10);
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            
            break;

    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", updateSquirrel);

void updateSquirrel(u8 index) {

    u16 temp;
    u8 tempDirection;
    
    switch (gMiscAnimals[index].actionState) {

        case 0:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0);

            if (getRandomNumberInRange(0, 60) < 40) {
                
                gMiscAnimals[index].actionState = 0;
                
            } else {
                
                temp = getRandomNumberInRange(0, 10);

                if (temp < 7) {
                    gMiscAnimals[index].direction = temp;
                }

                temp = getRandomNumberInRange(0, 1);

                if (temp == 0) {
                    gMiscAnimals[index].actionState = 1;
                }
                
                if (temp == 1) {
                    gMiscAnimals[index].actionState = 2;
                }
                
            } 
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
        
            break;


        case 1:
            gMiscAnimals[index].zDisplacement = 1;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 8);

            if (getRandomNumberInRange(0, 19) < 10) {
                gMiscAnimals[index].actionState = 1;
            } else {
                gMiscAnimals[index].actionState = 0;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            
            break;
        
        case 2:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x10);

            if (getRandomNumberInRange(0, 20) < 10) {
                gMiscAnimals[index].actionState = 2;
            } else {
                gMiscAnimals[index].actionState = 0;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;


    
        case 16:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0);

            gMiscAnimals[index].actionState = 0x10;
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            
            break;

        case 17:
            setEntityCollidable(gMiscAnimals[index].entityIndex, 0);
            setEntityYMovement(gMiscAnimals[index].entityIndex, 0);
            setEntityShadow(gMiscAnimals[index].entityIndex, 0xFF);

            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].yDisplacement = 2;

            setEntityAnimation(gMiscAnimals[index].entityIndex, 0x19);

            gMiscAnimals[index].timer++;
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;

            if (gMiscAnimals[index].timer == 6) {
                deactivateEntity(gMiscAnimals[index].entityIndex);
                gMiscAnimals[index].flags = 0;
            }
            
            break;
        
       case 32:
            gMiscAnimals[index].zDisplacement = 4;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 8);
            
            tempDirection = calculateAnimalDirectionToPlayer(entities[gMiscAnimals[index].entityIndex].coordinates.x, entities[gMiscAnimals[index].entityIndex].coordinates.z, entities[ENTITY_PLAYER].coordinates.x, entities[ENTITY_PLAYER].coordinates.z);

            gMiscAnimals[index].direction = (tempDirection + 4) % 8;
            
            if (!getRandomNumberInRange(0, 30)) {
                gMiscAnimals[index].flags &= ~(0x10);
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;

    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", updateMonkey);

void updateMonkey(u8 index) {

    u16 temp;
    u8 tempDirection;
    
    switch (gMiscAnimals[index].actionState) {

        case 0:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0);

            if (getRandomNumberInRange(0, 60) >= 40) {

                temp = getRandomNumberInRange(0, 10);

                if (temp < 7) {
                    gMiscAnimals[index].direction = temp;
                }

                temp = getRandomNumberInRange(0, 2);

                if (temp == 0) {
                    gMiscAnimals[index].actionState = 1;
                }
                
                if (temp == 1) {
                    gMiscAnimals[index].actionState = 2;
                }
                
                if (temp == 2) {
                    gMiscAnimals[index].actionState = 3;
                }
                
            } else {
                gMiscAnimals[index].actionState = 0;
            }

            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
        
            break;


        case 1:
            gMiscAnimals[index].zDisplacement = 1;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 8);

            if (getRandomNumberInRange(0, 19) < 10) {
                gMiscAnimals[index].actionState = 1;
            } else {
                gMiscAnimals[index].actionState = 0;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;



        case 2:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x10);

            if (getRandomNumberInRange(0, 20) < 10) {
                gMiscAnimals[index].actionState = 2;
            } else {
                gMiscAnimals[index].actionState = 0;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            playSfx(0x4D);
            
            break;


        case 3:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimation(gMiscAnimals[index].entityIndex, 0x22);

            gMiscAnimals[index].actionState = 0;
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;
    
        case 16:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0);

            gMiscAnimals[index].actionState = 0x10;
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            
            break;

        case 17:

            setEntityCollidable(gMiscAnimals[index].entityIndex, 0);
            setEntityYMovement(gMiscAnimals[index].entityIndex, 0);
            setEntityShadow(gMiscAnimals[index].entityIndex, 0xFF);

            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].yDisplacement = 2;

            setEntityAnimation(gMiscAnimals[index].entityIndex, 0x1D);

            gMiscAnimals[index].timer++;
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;

            if (gMiscAnimals[index].timer == 6) {
                deactivateEntity(gMiscAnimals[index].entityIndex);
                gMiscAnimals[index].flags = 0;
            }
            
            break;
        
       case 32:
            gMiscAnimals[index].zDisplacement = 4;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 8);
            
            tempDirection = calculateAnimalDirectionToPlayer(entities[gMiscAnimals[index].entityIndex].coordinates.x, entities[gMiscAnimals[index].entityIndex].coordinates.z, entities[ENTITY_PLAYER].coordinates.x, entities[ENTITY_PLAYER].coordinates.z);

            gMiscAnimals[index].direction = (tempDirection + 4) % 8;
            
            if (!getRandomNumberInRange(0, 30)) {
                gMiscAnimals[index].flags &= ~(0x10);
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            break;


    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", updateSparrow);

void updateSparrow(u8 index) {

    u16 temp;
    
    switch (gMiscAnimals[index].actionState) {

        case 0:

            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;
            
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0);
            
            if (getRandomNumberInRange(0, 60) < 40) {

                gMiscAnimals[index].actionState = 0;
            
            } else {
                
                temp = getRandomNumberInRange(0, 10);
                
                if (temp < 7) {
                    gMiscAnimals[index].direction = temp;
                }

                if (getRandomNumberInRange(0, 30) >= 28) {
                    gMiscAnimals[index].actionState = 32;
                } else {
                    gMiscAnimals[index].actionState = 1;
                }
                
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;

            break;

        case 1:

            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;

            temp = getRandomNumberInRange(0, 3); 

            switch (temp) {

                case 0:
                    setEntityAnimation(gMiscAnimals[index].entityIndex, 0x10);
                    break;
                case 1:
                    setEntityAnimation(gMiscAnimals[index].entityIndex, 0x11);
                    break;
                case 2:
                    setEntityAnimation(gMiscAnimals[index].entityIndex, 0x12);
                    break;
                
                case 3:
                    setEntityAnimation(gMiscAnimals[index].entityIndex, 0x13);
                    break;
            } 
                
            if (getRandomNumberInRange(0, 60) < 40) {
                gMiscAnimals[index].actionState = 1;
            } else {
                gMiscAnimals[index].actionState = 0;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;

            playSfx(0x46);
            
            break;

        case 32:

            setEntityCollidable(gMiscAnimals[index].entityIndex, FALSE);
            setEntityTracksCollisions(gMiscAnimals[index].entityIndex, FALSE);
            enableEntityMovement(gMiscAnimals[index].entityIndex, FALSE);
            setEntityYMovement(gMiscAnimals[index].entityIndex, FALSE);
            setEntityShadow(gMiscAnimals[index].entityIndex, 0xFF);
            
            entities[gMiscAnimals[index].entityIndex].yOffset = 0;
            
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 8);
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;

            if (gMiscAnimals[index].timer < 129) {
                gMiscAnimals[index].zDisplacement = 4;
                gMiscAnimals[index].yDisplacement = 2;
                gMiscAnimals[index].timer++;
            } else {
                deactivateEntity(gMiscAnimals[index].entityIndex);
                gMiscAnimals[index].flags = 0;
            }
            
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", updateBird);

void updateBird(u8 index) {
    
    u16 temp;
    
    switch (gMiscAnimals[index].actionState) {

        case 0:

            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;
            
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0);
            
            if (getRandomNumberInRange(0, 60) < 40) {

                gMiscAnimals[index].actionState = 0;
            
            } else {
                
                temp = getRandomNumberInRange(0, 10);
                
                if (temp < 7) {
                    gMiscAnimals[index].direction = temp;
                }

                if (getRandomNumberInRange(0, 30) >= 28) {
                    gMiscAnimals[index].actionState = 32;
                } else {
                    gMiscAnimals[index].actionState = 1;
                }
                
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;

            break;

        case 1:

            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;

            temp = getRandomNumberInRange(0, 3); 

            switch (temp) {

                case 0:
                    setEntityAnimation(gMiscAnimals[index].entityIndex, 0x10);
                    break;
                case 1:
                    setEntityAnimation(gMiscAnimals[index].entityIndex, 0x11);
                    break;
                case 2:
                    setEntityAnimation(gMiscAnimals[index].entityIndex, 0x12);
                    break;
                
                case 3:
                    setEntityAnimation(gMiscAnimals[index].entityIndex, 0x13);
                    break;
            } 
                
            if (getRandomNumberInRange(0, 60) < 40) {
                gMiscAnimals[index].actionState = 1;
            } else {
                gMiscAnimals[index].actionState = 0;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;

            if (!getRandomNumberInRange(0, 1)) {
                playSfx(0x48);
            } else {
                playSfx(0x49);
            }
            
            break;

        case 32:

            setEntityCollidable(gMiscAnimals[index].entityIndex, FALSE);
            setEntityTracksCollisions(gMiscAnimals[index].entityIndex, FALSE);
            enableEntityMovement(gMiscAnimals[index].entityIndex, FALSE);
            setEntityYMovement(gMiscAnimals[index].entityIndex, FALSE);
            setEntityShadow(gMiscAnimals[index].entityIndex, 0xFF);
            entities[gMiscAnimals[index].entityIndex].yOffset = 0;
            
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 8);

            gMiscAnimals[index].zDisplacement = 4;
            gMiscAnimals[index].yDisplacement = 2;
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            gMiscAnimals[index].timer++;
                    
            break;

    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", updateCrab);

void updateCrab(u8 index) {
    
    u32 temp;
    u16 temp2;
    
    switch (gMiscAnimals[index].actionState) {

        case 0:

            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;
            
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0);
            
            if (getRandomNumberInRange(0, 60) >= 40) {

                temp = getRandomNumberInRange(0, 10);

                if (temp < 7) {
                    // ???
                    temp &= ~3;
                    temp2 = temp;
                    gMiscAnimals[index].direction = temp2;
                } 
                
                gMiscAnimals[index].actionState = 1;
            
            } else {
                gMiscAnimals[index].actionState = 0;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;

            break;

        case 1:

            gMiscAnimals[index].zDisplacement = 1;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;
            
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 8);

            if (getRandomNumberInRange(0, 19) < 10) {
                gMiscAnimals[index].actionState = 1;
            } else {
                gMiscAnimals[index].actionState = 0;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;

            break;

        case 32:

            setEntityCollidable(gMiscAnimals[index].entityIndex, 0);
            setEntityYMovement(gMiscAnimals[index].entityIndex, 0);
            gMiscAnimals[index].direction = SOUTHEAST;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 8);
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;

            if (gMiscAnimals[index].timer < 17) {
                gMiscAnimals[index].zDisplacement = 4;
                gMiscAnimals[index].yDisplacement = 0;
                gMiscAnimals[index].timer++;
            } else {
                deactivateEntity(gMiscAnimals[index].entityIndex);
                gMiscAnimals[index].flags = 0;
            }
                    
            break;
            
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", updateSnake);

void updateSnake(u8 index) {

    u16 temp;
    
    switch (gMiscAnimals[index].actionState) {

        case 0:

            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;
            
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0);
            
            if (getRandomNumberInRange(0, 60) >= 40) {
                
                temp = getRandomNumberInRange(0, 10);
                
                if (temp < 7) {
                    gMiscAnimals[index].direction = temp;
                }
                
                gMiscAnimals[index].actionState = 1;
            
            } else {
                gMiscAnimals[index].actionState = 0;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;

            break;

        case 1:

            gMiscAnimals[index].zDisplacement = 1;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;
            
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 8);

            if (getRandomNumberInRange(0, 19) < 10) {
                gMiscAnimals[index].actionState = 1;
            } else {
                gMiscAnimals[index].actionState = 0;
            }
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;

            break;

        case 32:

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 8);
            
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;

            if (gMiscAnimals[index].timer < 33) {
                gMiscAnimals[index].zDisplacement = 4;
                gMiscAnimals[index].yDisplacement = 0;
                gMiscAnimals[index].timer++;
            } else {
                deactivateEntity(gMiscAnimals[index].entityIndex);
                gMiscAnimals[index].flags = 0;
            }
                    
            break;
            
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", updateWhiteButterfly);

void updateWhiteButterfly(u8 index) {

    switch (gMiscAnimals[index].actionState) {

        case 0:
        case 1:

            setEntityCollidable(gMiscAnimals[index].entityIndex, FALSE);
            setEntityTracksCollisions(gMiscAnimals[index].entityIndex, FALSE);
    
            entities[gMiscAnimals[index].entityIndex].yOffset = 0x20;
    
            gMiscAnimals[index].zDisplacement = 1;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;
    
            setEntityAnimation(gMiscAnimals[index].entityIndex, 2);
    
            gMiscAnimals[index].direction = getRandomNumberInRange(0, 7);
            gMiscAnimals[index].actionState = 0;
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            
            break;
        
        default:
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", updateLadybug);

void updateLadybug(u8 index) {

    switch (gMiscAnimals[index].actionState) {

        case 0:
        case 1:

            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;
    
            setEntityAnimation(gMiscAnimals[index].entityIndex, 0);

            gMiscAnimals[index].actionState = 0;
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;

            break;
        
        default:
            break;
    
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", updateCicada);

void updateCicada(u8 index) {

    switch (gMiscAnimals[index].actionState) {

        case 0:

            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimation(gMiscAnimals[index].entityIndex, 0);

            entities[gMiscAnimals[index].entityIndex].yOffset = 64;

            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;

            if (!(5 < gHour && gHour < 16)) {
                playSfx(0x51);
            } else {
                playSfx(0x4E);
            }
            
            break;
        
        case 1:

            setEntityCollidable(gMiscAnimals[index].entityIndex, FALSE);
            setEntityTracksCollisions(gMiscAnimals[index].entityIndex, FALSE);
            enableEntityMovement(gMiscAnimals[index].entityIndex, FALSE);
            setEntityYMovement(gMiscAnimals[index].entityIndex, FALSE);
            setEntityShadow(gMiscAnimals[index].entityIndex, 0xFF);

            entities[gMiscAnimals[index].entityIndex].yOffset = 0;

            setEntityAnimation(gMiscAnimals[index].entityIndex, 2);

            gMiscAnimals[index].zDisplacement = 4;
            gMiscAnimals[index].yDisplacement = 2;

            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;

            gMiscAnimals[index].timer++;
            
            break;    
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", updateHornedBeetle);

void updateHornedBeetle(u8 index) {

     switch (gMiscAnimals[index].actionState) {

        case 0:

            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimation(gMiscAnimals[index].entityIndex, 0);

            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;

            break;
        
        case 1:

            setEntityCollidable(gMiscAnimals[index].entityIndex, FALSE);
            setEntityTracksCollisions(gMiscAnimals[index].entityIndex, FALSE);
            enableEntityMovement(gMiscAnimals[index].entityIndex, FALSE);
            setEntityYMovement(gMiscAnimals[index].entityIndex, FALSE);
            setEntityShadow(gMiscAnimals[index].entityIndex, 0xFF);

            entities[gMiscAnimals[index].entityIndex].yOffset = 0;

            setEntityAnimation(gMiscAnimals[index].entityIndex, 4);

            gMiscAnimals[index].zDisplacement = 4;
            gMiscAnimals[index].yDisplacement = 2;

            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;

            gMiscAnimals[index].timer++;
            
            break;    
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", updateStagBeetle);

void updateStagBeetle(u8 index) {

    switch (gMiscAnimals[index].actionState) {

        case 0:

            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimation(gMiscAnimals[index].entityIndex, 0);

            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;

            break;
        
        case 1:

            setEntityCollidable(gMiscAnimals[index].entityIndex, FALSE);
            setEntityTracksCollisions(gMiscAnimals[index].entityIndex, FALSE);
            enableEntityMovement(gMiscAnimals[index].entityIndex, FALSE);
            setEntityYMovement(gMiscAnimals[index].entityIndex, FALSE);
            setEntityShadow(gMiscAnimals[index].entityIndex, 0xFF);

            entities[gMiscAnimals[index].entityIndex].yOffset = 0;

            setEntityAnimation(gMiscAnimals[index].entityIndex, 4);

            gMiscAnimals[index].zDisplacement = 4;
            gMiscAnimals[index].yDisplacement = 2;

            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;

            gMiscAnimals[index].timer++;
            
            break;    
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", updateDragonfly);

void updateDragonfly(u8 index) {

    u16 temp;
    
    switch (gMiscAnimals[index].actionState) {

        case 0:
        case 1:

            setEntityCollidable(gMiscAnimals[index].entityIndex, FALSE);
            setEntityTracksCollisions(gMiscAnimals[index].entityIndex, FALSE);
                        
            entities[gMiscAnimals[index].entityIndex].yOffset = 0x20;

            gMiscAnimals[index].zDisplacement = 1;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimation(gMiscAnimals[index].entityIndex, 2);

            temp = getRandomNumberInRange(0, 20);

            if (temp < 7) {
                gMiscAnimals[index].direction = temp;
            }
            
            gMiscAnimals[index].actionState = 0;
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;
            
            break;    
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", updateCricket);

void updateCricket(u8 index) {
    
    switch (gMiscAnimals[index].actionState) {

        case 0:

            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].timer = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimation(gMiscAnimals[index].entityIndex, 0);

            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;

            break;
        
        case 1:

            setEntityAnimation(gMiscAnimals[index].entityIndex, 2);
            gMiscAnimals[index].flags |= MISC_ANIMAL_STATE_CHANGED;

            if (gMiscAnimals[index].timer < 33) {
                gMiscAnimals[index].zDisplacement = 2;
                gMiscAnimals[index].yDisplacement = 0;
                gMiscAnimals[index].timer++;
            } else {
                deactivateEntity(gMiscAnimals[index].entityIndex);
                gMiscAnimals[index].flags = 0;
            }
            
            break;    
        
    }
    
}
//INCLUDE_ASM("asm/nonmatchings/game/animals", handleFarmAnimalPlayerCollision);

void handleFarmAnimalPlayerCollision(void) {
    
    u8 i = 0;
    bool set = FALSE;

    checkEntityProximity(ENTITY_PLAYER, 0.0f, 8.0f, 0);
    
    while (i < MAX_FARM_ANIMALS && !set) {
        
        if ((gFarmAnimals[i].flags & FARM_ANIMAL_ENTITY_LOADED) && (entities[gFarmAnimals[i].entityIndex].entityCollidedWithIndex == ENTITY_PLAYER)) {
            setAnimalState(2, i, 0xFF, 0xFF, 16);
            set = TRUE;
            gFarmAnimals[i].flags |= FARM_ANIMAL_COLLISION_WITH_PLAYER; 
        }
        
        i++;
        
    } 
        
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", handleChickenPlayerCollision);

void handleChickenPlayerCollision(void) {

    u8 i = 0;
    bool set = FALSE;

    checkEntityProximity(ENTITY_PLAYER, 0.0f, 8.0f, 0);

    while (i < MAX_CHICKENS && !set) {

        if ((gChickens[i].flags & CHICKEN_ENTITY_LOADED) && entities[gChickens[i].entityIndex].entityCollidedWithIndex == ENTITY_PLAYER) {

            setAnimalState(1, i, 0xFF, 0xFF, 16);
            set = TRUE;
            gChickens[i].flags |= CHICKEN_COLLISION_WITH_PLAYER;

        }

        i++;
        
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/animals", handleDogPlayerCollision);

void handleDogPlayerCollision(void) {

    checkEntityProximity(ENTITY_PLAYER, 0.0f, 8.0f, 0);
    
    if ((dogInfo.flags & DOG_ENTITY_LOADED) && (entities[dogInfo.entityIndex].entityCollidedWithIndex == ENTITY_PLAYER)) {
        dogInfo.actionState = 0x20;
        dogInfo.flags |= DOG_COLLISION_WITH_PLAYER;
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", handleHorsePlayerInteraction);

bool handleHorsePlayerInteraction(void) {
    
    bool result;

    checkEntityProximity(ENTITY_PLAYER, 0.0f, 8.0f, 0);
    
    result = FALSE;

    if ((horseInfo.flags & HORSE_ENTITY_LOADED) && (entities[horseInfo.entityIndex].entityCollidedWithIndex == ENTITY_PLAYER)) {
        result = TRUE;
        horseInfo.actionState = 16;
        horseInfo.flags |= HORSE_COLLISION_WITH_PLAYER;
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", handleHorseGrownPlayerInteraction);

bool handleHorseGrownPlayerInteraction(void) {
    
    bool result = FALSE;

    if ((horseInfo.flags & HORSE_ENTITY_LOADED) && (horseInfo.grown == TRUE) && (entities[horseInfo.entityIndex].entityCollidedWithIndex == ENTITY_PLAYER)) {
        horseInfo.actionState = 16;
        horseInfo.flags |=  HORSE_COLLISION_WITH_PLAYER;
        result = TRUE;
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/game/animals", handleBrushFarmAnimal);

bool handleBrushFarmAnimal(void) {

    u8 i = 0;
    bool set = FALSE;

    while (i < MAX_FARM_ANIMALS && !set) {

        if ((gFarmAnimals[i].flags & FARM_ANIMAL_ENTITY_LOADED) && gFarmAnimals[i].actionState == 16) {

            setAnimalState(2, i, 0xFF, 0xFF, 0);

            if (!(gFarmAnimals[i].flags & FARM_ANIMAL_BRUSHED)) {

                switch (gFarmAnimals[i].type) {

                    case BABY_COW ... PREGNANT_COW:
                        adjustFarmAnimalAffection(i, 2);
                        setAnimalState(2, i, 0xFF, 0xFF, 0x11);
                        gFarmAnimals[i].flags |= (FARM_ANIMAL_BRUSHED | FARM_ANIMAL_COLLISION_WITH_PLAYER);
                        showAnimalExpressionBubble(COW_TYPE, i, 3);
                        set = TRUE;
                        break;

                    case BABY_SHEEP ... SHEARED_SHEEP:
                        adjustFarmAnimalAffection(i, 2);
                        setAnimalState(2, i, 0xFF, 0xFF, 0x11);
                        gFarmAnimals[i].flags |= (FARM_ANIMAL_BRUSHED | FARM_ANIMAL_COLLISION_WITH_PLAYER);
                        showAnimalExpressionBubble(COW_TYPE, i, 3);
                        set = TRUE;
                        break;
                    
                    default:
                        break;
                    
                }
                
            }
            
        }
        
        i++;
        
    } 

    return set;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", handleBrushHorse);

bool handleBrushHorse(void) {

    bool result = FALSE;
    
    if (horseInfo.flags & HORSE_ENTITY_LOADED) {

        if (horseInfo.actionState == 16) {
            
            horseInfo.actionState = 0;
            
            if (!(horseInfo.flags & HORSE_BRUSHED_DAILY)) {
                
                horseInfo.actionState = 17;
                
                adjustHorseAffection(2);
            
                horseInfo.flags |= (HORSE_COLLISION_WITH_PLAYER | HORSE_BRUSHED_DAILY);
                showAnimalExpressionBubble(HORSE_TYPE, 0, 3);
                result = TRUE;
            
            }
                    
        }
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", handleHorseShippingItem);

bool handleHorseShippingItem(void) {

    bool result = FALSE;
    
    if ((horseInfo.flags & HORSE_ENTITY_LOADED) && (horseInfo.actionState == 16)) {
        
        horseInfo.actionState = 0;
        horseInfo.flags |= HORSE_COLLISION_WITH_PLAYER;
        showAnimalExpressionBubble(HORSE_TYPE, 0, 1);
        
        result = TRUE;
    
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", handleAnimalMedicineUse);

bool handleAnimalMedicineUse(void) {

    u8 i = 0;
    bool set = FALSE;

    while (i < MAX_FARM_ANIMALS && !set) {

        if ((gFarmAnimals[i].flags & FARM_ANIMAL_ENTITY_LOADED) && gFarmAnimals[i].actionState == 16) {

            setAnimalState(2, i, 0xFF, 0xFF, 0);

            switch (gFarmAnimals[i].type) {

                case ADULT_COW:
                case ADULT_SHEEP ... SHEARED_SHEEP:
                    if (gFarmAnimals[i].condition == 3) {
                        setAnimalState(2, i, 0xFF, 0, 0);
                        set = TRUE;
                    } 
                    break;
                
            }
            
        }

        i++;
        
    } 

    return set;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", handleMilkCow);

bool handleMilkCow(void) {

    u8 i = 0;
    bool set = FALSE;

    while (i < MAX_FARM_ANIMALS && !set) {

        if ((gFarmAnimals[i].flags & FARM_ANIMAL_ENTITY_LOADED) && gFarmAnimals[i].actionState == 16) {

            setAnimalState(2, i, 0xFF, 0xFF, 0);

            if (!(gFarmAnimals[i].flags & FARM_ANIMAL_MILKED)) {

                switch (gFarmAnimals[i].type) {

                    case ADULT_COW:
                        
                        if (gFarmAnimals[i].condition < COW_MAD) {
                            adjustFarmAnimalAffection(i, 1);
                            setAnimalState(2, i, 0xFF, 0xFF, 0x11);
                            gPlayer.heldItem = getMilkHeldItemIndex(i, 0);
                            gFarmAnimals[i].flags |= (FARM_ANIMAL_MILKED | FARM_ANIMAL_COLLISION_WITH_PLAYER);
                            showAnimalExpressionBubble(COW_TYPE, i, 3);
                            set = TRUE;
                        }
                        
                        break;
                    
                    case PREGNANT_COW:

                        adjustFarmAnimalAffection(i, 1);
                        setAnimalState(2, i, 0xFF, 0xFF, 0x11);

                        gPlayer.heldItem = getMilkHeldItemIndex(i, 0);

                        if (gPlayer.heldItem != SMALL_MILK) {
                            gPlayer.heldItem--;
                        }

                        gFarmAnimals[i].flags |= (FARM_ANIMAL_MILKED | FARM_ANIMAL_COLLISION_WITH_PLAYER);
                        showAnimalExpressionBubble(COW_TYPE, i, 3);
                        set = TRUE;
                        
                        break;
                    
                    
                }
                
            } 
            
        }
        
        i++;
        
    }

    return set;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", handleUseMiraclePotion);

u8 handleUseMiraclePotion(void) {

    u8 i = 0;
    bool set = FALSE;

    if (getTotalFarmAnimalsCount() < 8) {
        
        while (i < MAX_FARM_ANIMALS && !set) {
    
            if ((gFarmAnimals[i].flags & FARM_ANIMAL_ENTITY_LOADED) && gFarmAnimals[i].actionState == 16) {
    
                setAnimalState(2, i, 0xFF, 0xFF, 0);
                
                if (!(gFarmAnimals[i].flags & FARM_ANIMAL_PREGNANT) && gFarmAnimals[i].type == ADULT_COW) {
                
                    adjustFarmAnimalAffection(i, 10);
                    setAnimalState(2, i, 0xFF, COW_NORMAL, 0x11);
                    
                    gFarmAnimals[i].flags |= (FARM_ANIMAL_PREGNANT | FARM_ANIMAL_COLLISION_WITH_PLAYER);
                    
                    showAnimalExpressionBubble(COW_TYPE, i, 3);
                    set = TRUE;
                    
                }

            }
    
            i++;
            
        }

    }

    return set;

}

//INCLUDE_ASM("asm/nonmatchings/game/animals", handleGetMilkWithBottle);

bool handleGetMilkWithBottle(void) {

    u8 i = 0;
    bool set = FALSE;

    while (i < MAX_FARM_ANIMALS && !set) {

        if ((gFarmAnimals[i].flags & FARM_ANIMAL_ENTITY_LOADED) && gFarmAnimals[i].actionState == 16) {

            setAnimalState(2, i, 0xFF, 0xFF, 0);

            switch (gFarmAnimals[i].type) {

                case ADULT_COW:
                    
                    if (gFarmAnimals[i].condition < COW_MAD) {
                        setAnimalState(2, i, 0xFF, 0xFF, 0x11);
                        set = TRUE;
                        gFarmAnimals[i].flags |= FARM_ANIMAL_COLLISION_WITH_PLAYER;
                        gPlayer.bottleContents = 6;
                    }

                    break;

                case PREGNANT_COW:

                    setAnimalState(2, i, 0xFF, 0xFF, 0x11);
                    set = TRUE;
                    gFarmAnimals[i].flags |= FARM_ANIMAL_COLLISION_WITH_PLAYER;
                    gPlayer.bottleContents = 6;

                    break;

            }

        }

        i++;
        
    }

    return set;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", handleShearSheep);

void handleShearSheep(void) {

    u8 i = 0;
    bool set = FALSE;

    while (i < MAX_FARM_ANIMALS && !set) {

        if ((gFarmAnimals[i].flags & FARM_ANIMAL_ENTITY_LOADED) && gFarmAnimals[i].actionState == 16) {

            setAnimalState(2, i, 0xFF, 0xFF, 0);

            if (!(gFarmAnimals[i].flags & FARM_ANIMAL_SHEARED) && gFarmAnimals[i].type == ADULT_SHEEP) {
                
                adjustFarmAnimalAffection(i, 2);
                setAnimalState(2, i, SHEARED_SHEEP, 0xFF, 0x11);
                
                gPlayer.heldItem = getWoolHeldItemIndex(i, 0);
                gFarmAnimals[i].flags |= FARM_ANIMAL_SHEARED;
                
                showAnimalExpressionBubble(COW_TYPE, i, 3);
                set = TRUE;
                
            }

        }

        i++;
        
    } 

}

//INCLUDE_ASM("asm/nonmatchings/game/animals", handleHitFarmAnimalWithTool);

bool handleHitFarmAnimalWithTool(void) {

    u8 i = 0;
    bool result = FALSE;

    do {

        if ((gFarmAnimals[i].flags & FARM_ANIMAL_ENTITY_LOADED) && gFarmAnimals[i].actionState == 16) {

            setAnimalState(2, i, 0xFF, 0xFF, 0);

            switch (gFarmAnimals[i].type) {

                case BABY_COW ... PREGNANT_COW:
                    
                    adjustFarmAnimalAffection(i, -10);
                    setAnimalState(2, i, 0xFF, 0xFF, 0x12);
                    
                    gFarmAnimals[i].flags |= FARM_ANIMAL_COLLISION_WITH_PLAYER;
                    
                    showAnimalExpressionBubble(COW_TYPE, i, 0);

                    if (gFarmAnimals[i].type == ADULT_COW) {
                        
                        if (!(getRandomNumberInRange(0, 7))) {
                            setAnimalState(2, i, 0xFF, 2, 0xFF);
                            adjustFarmAnimalAffection(i, -20);
                            gHappiness += adjustValue(gHappiness, -5, MAX_HAPPINESS);
                        }
                        
                    }
                    
                    result = TRUE;
                    
                    break;

                case BABY_SHEEP ... SHEARED_SHEEP:
                    
                    adjustFarmAnimalAffection(i, -10);
                    setAnimalState(2, i, 0xFF, 0xFF, 0x12);
                    
                    gFarmAnimals[i].flags |= FARM_ANIMAL_COLLISION_WITH_PLAYER;
                    
                    showAnimalExpressionBubble(COW_TYPE, i, 0);
                    
                    result = TRUE;
                    
                    break;                    
                
            }
            
        }

        i++;
        
    } while (i < MAX_FARM_ANIMALS && result == FALSE);

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", handleHitChickenWithTool);

bool handleHitChickenWithTool(void) {

    u8 i = 0;
    bool set = FALSE;

    while (i < MAX_CHICKENS && !set) {

        if ((gChickens[i].flags & CHICKEN_ENTITY_LOADED) && gChickens[i].actionState == 16) {

            setAnimalState(1, i, 0xFF, 0xFF, 0);

             switch (gChickens[i].type) { 

                 case ADULT_CHICKEN:
                    setAnimalState(1, i, 0xFF, 0xFF, 0x11);
                    gChickens[i].flags |= CHICKEN_COLLISION_WITH_PLAYER;
                    showAnimalExpressionBubble(CHICKEN_TYPE, i, 0);
                    set = TRUE;
                    break;
                 
                 case CHICK:
                    gChickens[i].flags |= CHICKEN_COLLISION_WITH_PLAYER;
                    showAnimalExpressionBubble(CHICKEN_TYPE, i, 0);
                    set = TRUE;
                    break;

                default:
                    break;
                 
             }
            
        }

        i++;
        
    }

    return set;

}

//INCLUDE_ASM("asm/nonmatchings/game/animals", handleHitDogWithTool);
 
bool handleHitDogWithTool(void) {

    bool result = FALSE;
    
    if (dogInfo.flags & DOG_ENTITY_LOADED) {

        if (dogInfo.actionState == 0x20) {

            dogInfo.actionState = 34;

            adjustDogAffection(-8);

            dogInfo.flags |= DOG_COLLISION_WITH_PLAYER;
            showAnimalExpressionBubble(DOG_TYPE, 0, 0);
            result = TRUE;
            
        }
        
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", handleHitHorseWithTool);

bool handleHitHorseWithTool(void) {
    
    bool result = FALSE;
    
    if (horseInfo.flags & HORSE_ENTITY_LOADED) {

        if (horseInfo.actionState == 16) {

            horseInfo.actionState = 18;

            adjustHorseAffection(-8);

            horseInfo.flags |= HORSE_COLLISION_WITH_PLAYER;
            showAnimalExpressionBubble(HORSE_TYPE, 0, 0);
            result = TRUE;
            
        }
        
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", handleWhistleForDog);

void handleWhistleForDog(void) {

    if (dogInfo.flags & DOG_ENTITY_LOADED) {
        dogInfo.flags |= DOG_FOLLOWING;
        dogInfo.actionState = 0;
        playSfx(DOG_BARK);
    }

    if (!(dogInfo.flags & DOG_WHISTLED_FOR_DAILY)) {
        adjustDogAffection(1);
        dogInfo.flags |= DOG_WHISTLED_FOR_DAILY;
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", handleWhistleForHorse);

void handleWhistleForHorse(void) {
    
    if (horseInfo.flags & HORSE_ENTITY_LOADED) {
        horseInfo.flags |= HORSE_FOLLOWING;
        horseInfo.actionState = 0;
        playSfx(HORSE_NEIGH);
    }
    
    if (!(horseInfo.flags & HORSE_WHISTLED_DAILY)) {
        adjustHorseAffection(1);
        horseInfo.flags |= HORSE_WHISTLED_DAILY;
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", handleCallFarmAnimalsWithCowBell);

void handleCallFarmAnimalsWithCowBell(void) {

    u8 i;

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {

        if (gFarmAnimals[i].flags & FARM_ANIMAL_ENTITY_LOADED) {
            gFarmAnimals[i].flags |= (FARM_ANIMAL_APPROACHING | FARM_ANIMAL_FOLLOWING);
            gFarmAnimals[i].actionState = 0;
        }
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", getTotalFarmAnimalsByType);

u8 getTotalFarmAnimalsByType(u8 type) {

    u8 i;
    u8 count = 0;

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {
        
        if (gFarmAnimals[i].flags & FARM_ANIMAL_ACTIVE && gFarmAnimals[i].type == type) {
            count++;
        }
        
    }
    
    return count;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", getTotalPregnantFamAnimals);

u8 getTotalPregnantFamAnimals(void) {

    u8 i;
    u8 count = 0;

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {
        
        if ((gFarmAnimals[i].flags & FARM_ANIMAL_ACTIVE) && (gFarmAnimals[i].flags & FARM_ANIMAL_PREGNANT)) {
            count++;
        }
        
    }
    
    return count;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", getTotalFarmAnimalsCount);

// check all farm animals + animals going to be born
u8 getTotalFarmAnimalsCount(void) {

    u8 i;
    u8 count = 0;

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {
        
        if (gFarmAnimals[i].flags & FARM_ANIMAL_ACTIVE) {
            
            if (gFarmAnimals[i].type == PREGNANT_COW) {
                count++;
            }

            count++;
        
        }
        
    }
    
    return count;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", getTotalCowsCount);

u8 getTotalCowsCount(void) {

    u8 i;
    u8 count = 0;

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {
        
        if (gFarmAnimals[i].flags & FARM_ANIMAL_ACTIVE) {
            
            if ((0 < gFarmAnimals[i].type && gFarmAnimals[i].type < 3) || (gFarmAnimals[i].type == BABY_COW || gFarmAnimals[i].type == PREGNANT_COW)) {
                count++;
            }
    
        }
    }
    
    return count;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", getCowWithHighestAffection);

u8 getCowWithHighestAffection(void) {

    u8 i;
    u8 found = 0xFF;
    u8 temp = 0;

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {
        
        if ((gFarmAnimals[i].flags & FARM_ANIMAL_ACTIVE) && gFarmAnimals[i].type == ADULT_COW) {

            if (gFarmAnimals[i].affection >= temp) {
                found = i;
                temp = gFarmAnimals[i].affection;
            }
    
        }
    }
    
    return found;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", getTotalSheepCount);

u8 getTotalSheepCount(void) {

    u8 i;
    u8 count = 0;

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {
        
        if (gFarmAnimals[i].flags & FARM_ANIMAL_ACTIVE) {
            
            if (PREGNANT_COW < gFarmAnimals[i].type && gFarmAnimals[i].type < SHEARED_SHEEP || gFarmAnimals[i].type == SHEARED_SHEEP) {
                count++;
            }
    
        }
    }
    
    return count;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", getTotalChickenCount);

u8 getTotalChickenCount(void) {

    u8 i;
    u8 count = 0;

    for (i = 0; i < MAX_CHICKENS; i++) {

        if (gChickens[i].flags & CHICKEN_ACTIVE) {
            
            if (0 < gChickens[i].type && gChickens[i].type < 3) {
                count++;
            }

            if (gChickens[i].type == CHICKEN_EGG && (gChickens[i].flags & CHICKEN_EGG_INCUBATING)) {
                count++;
            }
            
        }  

    }
    
    return count;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", getHealthyChickenCount);

u8 getHealthyChickenCount(void) {

    u8 i;
    u8 count = 0;

    for (i = 0; i < MAX_CHICKENS; i++) {

        if ((gChickens[i].flags & CHICKEN_ACTIVE) && gChickens[i].type == ADULT_CHICKEN && gChickens[i].condition == CHICKEN_NORMAL) {
            count++;
        }

    }
    
    return count;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", getChickenEggCount);

u8 getChickenEggCount(void) {

    u8 i;
    u8 count = 0;

    for (i = 0; i < MAX_CHICKENS; i++) {

        if ((gChickens[i].flags & CHICKEN_ACTIVE) && gChickens[i].type == CHICKEN_EGG) {
            count++;
        }

    }
    
    return count;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", handleHatchChicken);

void handleHatchChicken() {

    u8 i;
    bool found = FALSE;

    i = 0;
    
    while (i < MAX_CHICKENS && !found) {

        if (!(gChickens[i].flags & CHICKEN_ACTIVE)) {

            i++;

        } else if (gChickens[i].type == CHICKEN_EGG) {
          
            if (!(gChickens[i].flags & CHICKEN_EGG_INCUBATING)) {
                found++;
            }
            
        } else {
            i++;
        }
        
    }

    if (found) {
        
        if (gChickens[i].flags & CHICKEN_ENTITY_LOADED) {
            if (entities[gChickens[i].entityIndex].flags & ENTITY_IS_CURRENTLY_ANIMATED) {
                deactivateEntity(gChickens[i].entityIndex);
            }
        }

        initializeChicken(i);
    
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", getIncubatingEggCount);

u8 getIncubatingEggCount(void) {

    u8 i;
    u8 sum = 0;

    for (i = 0; i < MAX_CHICKENS; i++) {

        if ((gChickens[i].flags & CHICKEN_ACTIVE) && (gChickens[i].type == CHICKEN_EGG) && (gChickens[i].flags & CHICKEN_EGG_INCUBATING)) {
            sum++;
        }
        
    }
    
    return sum;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009B828);

u8 func_8009B828(u8 arg0) {

    u8 i;
    u8 count = 0;
    
    for (i = 0; i < MAX_FARM_ANIMALS; i++) {

        // FIXME: should be range
        if ((gFarmAnimals[i].flags & FARM_ANIMAL_ACTIVE) 
            && (((u8)(gFarmAnimals[i].type - 1) < 2U 
                || (gFarmAnimals[i].type == BABY_COW || gFarmAnimals[i].type == PREGNANT_COW))
            && gFarmAnimals[i].location == FARM && (arg0 == 0 || (gFarmAnimals[i].milkType == 0)))) {

            count++;
        
        }
        
    }

    if (count) {
        count = getFarmGrassTilesSum() != 0;
    }

    return count;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", setgAnimalSalePrice);

void setgAnimalSalePrice() {

    u8 temp;
    u16 temp2;
    u16 temp3;
    
    switch (selectedAnimalType) {

        case 2:
            
            if (gFarmAnimals[gSelectedAnimalIndex].milkType == 0) {
                
                temp2 = goldenMilkInfo[2];
                
            } else {

                temp = gFarmAnimals[gSelectedAnimalIndex].affection; 

                if (temp < 151) {
                    temp2 = smallMilkInfo[2];
                }
                
                if (150 < temp && temp < 221) {
                    temp2 = mediumMilkInfo[2];
                }
    
                if (temp >= 221) {
                    temp2 = largeMilkInfo[2];
                }
                    
            }

            gAnimalSalePrice = temp2;
            
            break;

        case 3:

            temp = gFarmAnimals[gSelectedAnimalIndex].affection; 
            
            if (temp < 100) {
                temp3 = sheepWoolInfo.arr[2];
            }
            
            if (99 < temp && temp < 200) {
                temp3 = sheepWoolInfo.arr2[2];
            }

            if (temp >= 200) {
                temp3 = sheepWoolInfo.arr3[2];
            }

            gAnimalSalePrice = temp3;
            
            break;
        
        case 4:
            gAnimalSalePrice = 500;
            break;

        default:
            break;

    }

    convertNumberToGameVariableString(0x12, gAnimalSalePrice, 0);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", generateMilkTypeString);

void generateMilkTypeString(u8 index) {

    u16 temp;

    if (CALF < gFarmAnimals[index].type && gFarmAnimals[index].type < BABY_SHEEP) {

        if (gFarmAnimals[index].milkType == 0) {
            
            temp = goldenMilkInfo[0];
            
        } else {

            if (gFarmAnimals[index].affection < 151) {
                temp = smallMilkInfo[0];
            }
            
            if (150 < gFarmAnimals[index].affection && gFarmAnimals[index].affection < 221) {
                temp = mediumMilkInfo[0];
            }

            if (gFarmAnimals[index].affection >= 221) {
                temp = largeMilkInfo[0];
            }
            
        }

        switch (temp) {

            case 0x15:
                gMilkTypeString[0] = char_S;
                break;
            case 0x16:
                gMilkTypeString[0] = char_M;
                break;
            case 0x17:
                gMilkTypeString[0] = char_L;
                break;
            case 0x18:
                gMilkTypeString[0] = char_G;
                break;
            default:
                break;
            
        }
        
    } else {
        gMilkTypeString[0] = 0xF6;
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009BB70);

// same as func_80061690
// unused
// void func_8009BB70(void) {
//     D_801886D4[0] = 0xF6;
//     D_801886D4[1] = 0xF6;
//     D_801886D4[2] = 0xF6;
//     D_801886D4[3] = 0xF6;
//     D_801886D4[4] = 0xF6;
//     D_801886D4[5] = 0xF6;
// }

//INCLUDE_ASM("asm/nonmatchings/game/animals", getBestCowMilkType);

u8 getBestCowMilkType(void) {

    u8 temp;
    u8 found = 0xFF;
    u8 i = 0;

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {

        if (gFarmAnimals[i].flags & FARM_ANIMAL_ACTIVE) {

            // FIXME: should be range
            if ((u8)(gFarmAnimals[i].type - 1) < 2U || (gFarmAnimals[i].type == BABY_COW || gFarmAnimals[i].type == PREGNANT_COW)) {
                
                temp = gFarmAnimals[i].milkType;
    
                if (temp < found) {
                    found = temp;
                }
                
            }
            
        }
        
    }

    return found;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", getDogBestRacePlacement);

u8 getDogBestRacePlacement(void) {
    return dogInfo.bestRacePlacement;
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", getHorseBestRacePlacement);

u8 getHorseBestRacePlacement(void) {
    return horseInfo.bestRacePlacement;
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", randomizeMiscAnimalSpawnVariants);

void randomizeMiscAnimalSpawnVariants(void) {

    u8 i;

    for (i = 0; i < MAX_MISC_ANIMALS; i++) {
        gMiscAnimals[i].spawnVariant = getRandomNumberInRange(0, 3);
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", showAnimalExpressionBubble);

void showAnimalExpressionBubble(u8 animalType, u8 animalIndex, u8 arg2) {

    deactivateEntity(0x30);
    loadEntity(0x30, ENTITY_ASSET_HOLDABLE_ITEMS_1, TRUE);
    setCameraTrackingEntity(0x30, FALSE);

    switch (animalType) {

        case DOG_TYPE:
            setEntityTrackingTarget(0x30, dogInfo.entityIndex, 0xFF);
            break;

        case HORSE_TYPE:
            setEntityTrackingTarget(0x30, horseInfo.entityIndex, 0xFF);
            break;

        case COW_TYPE:
            setEntityTrackingTarget(0x30, gFarmAnimals[animalIndex].entityIndex, 0xFF);
            break;
        
        case SHEEP_TYPE:
            setEntityTrackingTarget(0x30, gFarmAnimals[animalIndex].entityIndex, 0xFF);
            break;

        case CHICKEN_TYPE:
            setEntityTrackingTarget(0x30, gChickens[animalIndex].entityIndex, 0xFF);
            break;

        default:
            break;
        
    }

    setEntityCollidable(0x30, FALSE);
    setEntityYMovement(0x30, FALSE);
    setEntityTracksCollisions(0x30, FALSE);
    enableEntityMovement(0x30, FALSE);

    setEntityColor(0x30, mainMap[MAIN_MAP_INDEX].mapGlobals.currentRGBA.r, mainMap[MAIN_MAP_INDEX].mapGlobals.currentRGBA.g, mainMap[MAIN_MAP_INDEX].mapGlobals.currentRGBA.b, mainMap[MAIN_MAP_INDEX].mapGlobals.currentRGBA.a);
    setEntityAttachmentOffset(0x30, 0, 0x30, 0);
    setEntityAnimation(0x30, arg2);
    
}