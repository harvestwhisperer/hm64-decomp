#include "common.h"

#include "game/animals.h"

#include "system/controller.h"
#include "system/entity.h"
#include "system/graphic.h"
#include "system/mapController.h"
#include "system/math.h"
#include "system/message.h"

#include "game/groundObjects.h"
#include "game/game.h"
#include "game/gameAudio.h"
#include "game/gameStatus.h"
#include "game/items.h"
#include "game/level.h"
#include "game/player.h"
#include "game/time.h"
#include "game/weather.h"

#include "mainproc.h"

#include "assetIndices/dialogueMenus.h"
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

u16 smallMilkInfo[4] = { SMALL_MILK_HELD_ITEM, 100, 6500, 10 };
u16 mediumMilkInfo[4] = { MEDIUM_MILK_HELD_ITEM, 150, 7000, 20 };
u16 largeMilkInfo[4] = { LARGE_MILK_HELD_ITEM, 300,  7500, 30 };
u16 goldenMilkInfo[4] = { GOLDEN_MILK_HELD_ITEM, 500, 8500, 50 };

SheepItemInfo sheepWoolInfo = {
    { WOOL_HELD_ITEM, 900, 4200 },
    { WOOL_HELD_ITEM, 900, 4600 },
    { HIGH_QUALITY_WOOL_HELD_ITEM, 1800, 5000}
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
void updateRanchSheep(u8);
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

void deactivateAnimalEntities(void) {
    Dog *dog = &dogInfo;
    Horse *horse = &horseInfo;


    u8 i;

    if (dog->flags & DOG_ENTITY_LOADED) {
        deactivateEntity(dog->entityIndex);
    }

    dog->actionState = 0;
    dog->stateTimer = 0;
    dog->flags &= ~DOG_ENTITY_LOADED;

    for (i = 0; i < MAX_CHICKENS; i++) {
        Chicken *chicken = &gChickens[i];


        if (chicken->flags & CHICKEN_ENTITY_LOADED) {
            deactivateEntity(chicken->entityIndex);
        }

        chicken->actionState = 0;
        chicken->stateTimer = 0;
        chicken->flags &= ~CHICKEN_ENTITY_LOADED;
        
    }

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {
        FarmAnimal *farmAnimal = &gFarmAnimals[i];


        if (farmAnimal->flags & FARM_ANIMAL_ENTITY_LOADED) {
            deactivateEntity(farmAnimal->entityIndex);
        }

        farmAnimal->actionState = 0;
        farmAnimal->stateTimer = 0;
        farmAnimal->flags &= ~FARM_ANIMAL_ENTITY_LOADED;
        
    }

    if (horse->flags & HORSE_ENTITY_LOADED) {
        deactivateEntity(horse->entityIndex);
    }

    horse->actionState = 0;
    horse->stateTimer = 0;
    horse->flags &= ~HORSE_ENTITY_LOADED;

    for (i = 0; i < MAX_MISC_ANIMALS; i++) {
        MiscAnimal *miscAnimal = &gMiscAnimals[i];

        
        if (miscAnimal->flags & MISC_ANIMAL_ENTITY_LOADED) {
            deactivateEntity(miscAnimal->entityIndex);
        }
        
        miscAnimal->flags = 0;
        miscAnimal->actionState = 0;
        miscAnimal->timer = 0;
        
    }
    
}

void setAnimalState(u8 animalType, u8 index, u8 type, u8 condition, u8 actionState) {
    Chicken *chicken = &gChickens[index];
    FarmAnimal *farmAnimal = &gFarmAnimals[index];


    switch (animalType) {       
        
        case 0:
            break;
        
        case 1:
            
            if (type != 0xFF) {
                chicken->type = type;
                chicken->typeCounter = 0;
            }
            
            if (condition != 0xFF) {
                chicken->condition = condition;
                chicken->conditionCounter = 0;
            }
            
            if (actionState != 0xFF) {
                chicken->actionState = actionState;
            }

            chicken->speed = 0;
            chicken->stateTimer = 0;
            chicken->unk_1B = 0;
        
            break;
        
        case 2:
            
            if (type != 0xFF) {
                farmAnimal->type = type;
                farmAnimal->typeCounter = 0;
            }
            
            if (condition != 0xFF) {
                farmAnimal->condition = condition;
                farmAnimal->conditionCounter = 0;
            }
            
            if (actionState != 0xFF) {
                farmAnimal->actionState = actionState;
            }

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;
        
            break;
        
        default:
            break;
        
    }
    
}

void adjustAllAnimalAffection(s8 amount) {

    u8 i;

    adjustDogAffection(amount);
    adjustHorseAffection(amount);

    for (i = 0; i < 8; i++) {
        adjustFarmAnimalAffection(i, amount);
    }
    
}

inline void adjustDogAffection(s8 amount) {
    if (dogInfo.flags & DOG_ACTIVE) {
        dogInfo.affection += adjustValue(dogInfo.affection, amount, 0xFF);
    }
}

inline void adjustHorseAffection(s8 amount) {
    if (horseInfo.flags & HORSE_ACTIVE) {
        horseInfo.affection += adjustValue(horseInfo.affection, amount, 0xFF);
    }
}

void adjustFarmAnimalAffection(u8 animalIndex, s8 amount) {
    FarmAnimal *farmAnimal = &gFarmAnimals[animalIndex];

    
    s8 adjusted;
    
    if (farmAnimal->flags & FARM_ANIMAL_ACTIVE) {
    
        switch (farmAnimal->type) {

            case BABY_COW:
            case CALF:
            case PREGNANT_COW:
            case BABY_SHEEP:
                adjusted = amount;
                break;

            case ADULT_COW:

                switch (farmAnimal->condition) {
                    
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
                
                switch (farmAnimal->condition) {

                    case SHEEP_NORMAL:
                        adjusted = amount;
                        break;
                
                    case SHEEP_SICK:
                        adjusted = (amount >= 0) ? (amount / 2) : -(amount * 2);
                        break;
                
                }
                
                break;

            case ADULT_SHEEP:
                
                switch (farmAnimal->condition) {
                    
                    case SHEEP_NORMAL:
                        adjusted = amount;
                        break;
                        
                    case SHEEP_SICK:
                        adjusted = (amount >= 0) ? (amount / 2) : -(amount * 2);
                        break;
            
                }

                break;

        }

        farmAnimal->affection += adjustValue(farmAnimal->affection, adjusted, MAX_AFFECTION);
    
    }
    
}

inline u16 getMilkHeldItemIndex(u8 animalIndex, u8 arg1) {
    FarmAnimal *farmAnimal = &gFarmAnimals[animalIndex];


    u16 res;
    
    if (farmAnimal->milkType == 0) {

        res = goldenMilkInfo[arg1];
        
    } else {

        if (farmAnimal->affection <= 150) {
            res = smallMilkInfo[arg1];
        }

        if (150 < farmAnimal->affection && farmAnimal->affection < 221) {
            res = mediumMilkInfo[arg1];
        }

        if (farmAnimal->affection >= 221) {
            res = largeMilkInfo[arg1];
        }
        
    }

    return res;

}

inline u16 getWoolHeldItemIndex(u8 animalIndex, u8 arg1) {
    FarmAnimal *farmAnimal = &gFarmAnimals[animalIndex];


    u16 res;

    if (farmAnimal->affection < 100) {
        res = sheepWoolInfo.arr[arg1];
    }

    if (99 < farmAnimal->affection && farmAnimal->affection < 200) {
        res = sheepWoolInfo.arr2[arg1];
    }

    if (farmAnimal->affection >= 200) {
        res = sheepWoolInfo.arr3[arg1];
    }
    
    return res;
    
}

bool handlePlayerAnimalInteraction(void) {
    Dog *dog = &dogInfo;
    Horse *horse = &horseInfo;


    Vec3f vec;
    
    bool set = FALSE;
    u8 i;
    
    u16 textIndex;
    u16 tempFlags;
    u8 groundObjectIndex;

    if (!(gPlayer.flags & PLAYER_RIDING_HORSE)) {

        if (gPlayer.heldItem == 0) {

            if ((dog->flags & DOG_ENTITY_LOADED) && entities[dog->entityIndex].entityCollidedWithIndex == ENTITY_PLAYER && entities[dog->entityIndex].buttonPressed == BUTTON_A) {

                // ???
                if (!i) {
                    gPlayer.heldItem = DOG_HELD_ITEM;
                } else {
                    gPlayer.heldItem = PUPPY_1_HELD_ITEM;
                }
                
                deactivateEntity(dog->entityIndex);
                setPlayerAction(PICKING_UP_ITEM, ANIM_PICKING_UP_ITEM);

                dog->flags &= ~(DOG_ENTITY_LOADED | DOG_FOLLOWING);
                dog->flags |= DOG_HELD;

                if (!(dog->flags & DOG_HELD_DAILY)) {

                    adjustDogAffection(1);

                    dog->flags |= DOG_HELD_DAILY;
                    
                }
                
                set = TRUE;
                
            }

            for (i = 0; i < MAX_CHICKENS && !set; i++) {
        Chicken *chicken = &gChickens[i];


                if ((chicken->flags & CHICKEN_ENTITY_LOADED) && entities[chicken->entityIndex].entityCollidedWithIndex == ENTITY_PLAYER && entities[chicken->entityIndex].buttonPressed == BUTTON_A) {

                    if (checkDailyEventBit(ANIMAL_SALE_IN_PROGRESS) && getLevelFlags(chicken->location) & LEVEL_FARM) {

                        if (chicken->type == ADULT_CHICKEN) {

                            setGameVariableString(13, chicken->name, 6);
                            showDialogueTextBox(DIALOGUE_MENU_RANCH_STORE_ANIMAL_SELL_CONFIRM);
                            
                            selectedAnimalType = CHICKEN_TYPE;
                            gSelectedAnimalIndex = i;
                            
                            setgAnimalSalePrice();

} else {
                            showTextBox(0, LEVEL_INTERACTIONS_TEXT_INDEX, 92, 0, 2);
                        }
                        
                        set = TRUE;
                        
                    } else {

                        switch (chicken->type) {

                            case ADULT_CHICKEN:
                                gPlayer.heldItem = CHICKEN_HELD_ITEM;
                                chicken->flags |= CHICKEN_HELD;
                                set = TRUE;
                                break;
                            
                            case CHICK:
                                gPlayer.heldItem = CHICK_HELD_ITEM;
                                chicken->flags |= CHICKEN_HELD;
                                set = TRUE;
                                break;
                            
                            case CHICKEN_EGG:

                                if (!(chicken->flags & CHICKEN_EGG_INCUBATING)) {
                                    set = TRUE;
                                    gPlayer.heldItem = EGG_HELD_ITEM;
                                    chicken->flags = 0;
                                }
                                
                                break;
                            
                        }

                        if (set) {
                            
                            deactivateEntity(chicken->entityIndex);
                            
                            setPlayerAction(PICKING_UP_ITEM, ANIM_PICKING_UP_ITEM);
                            gPlayer.heldAnimalIndex = i;
                            
                            chicken->flags &= ~CHICKEN_ENTITY_LOADED;
                            
                        }
                        
                    }
                    
                }

            }

            for (i = 0; i < MAX_FARM_ANIMALS && !set; i++) {
        FarmAnimal *farmAnimal = &gFarmAnimals[i];


                if ((farmAnimal->flags & FARM_ANIMAL_ENTITY_LOADED) && entities[farmAnimal->entityIndex].entityCollidedWithIndex == ENTITY_PLAYER) {

                    if (entities[farmAnimal->entityIndex].buttonPressed == BUTTON_A) {
                        
                        if (checkDailyEventBit(ANIMAL_SALE_IN_PROGRESS) && getLevelFlags(farmAnimal->location) & LEVEL_FARM) {

                            if ((farmAnimal->type == ADULT_COW || farmAnimal->type == ADULT_SHEEP) && (!(COW_HAPPY < farmAnimal->condition && farmAnimal->condition < COW_DEAD))) {
                                
                                setGameVariableString(13, farmAnimal->name, 6);
                                showDialogueTextBox(DIALOGUE_MENU_RANCH_STORE_ANIMAL_SELL_CONFIRM);
                                
                                if (farmAnimal->type == ADULT_COW) {
                                    selectedAnimalType = COW_TYPE;
                                } else {
                                    selectedAnimalType = SHEEP_TYPE;
                                }
                                
                                gSelectedAnimalIndex = i;
                                
                                setgAnimalSalePrice();
                                
                            }  else {
                                showTextBox(0, LEVEL_INTERACTIONS_TEXT_INDEX, 92, 0, 2);
                            }
                        
                        } else if (checkDailyEventBit(COW_FESTIVAL_ENTRY_DAILY)) {
    
                            if (farmAnimal->type == ADULT_COW && !(COW_HAPPY < farmAnimal->condition && farmAnimal->condition < COW_DEAD)) {
                                
                                showDialogueTextBox(DIALOGUE_MENU_COW_FESTIVAL_ENTRY_CONFIRM);
                                
                                setGameVariableString(13, farmAnimal->name, 6);
                                gSelectedAnimalIndex = i;
                                
                            }  else {
                                showTextBox(0, FESTIVALS_TEXT_INDEX, 53, 0, 0);
                            }
                            
                        } else {
    
                            setMessageBoxInterpolationWithFlags(0, -4, 2);
                            setMessageBoxViewSpacePosition(0, 0.0f, -64.0f, 352.0f);
                            setMessageBoxSpriteIndices(0, 1, 0, 0);
                            
                            setGameVariableString(0xD, farmAnimal->name, 6);
    
                            switch (farmAnimal->condition) {
    
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
    
                            if (farmAnimal->type == PREGNANT_COW) {
    
                                convertNumberToGameVariableString(21, 21 - farmAnimal->typeCounter, 1);
    
                                switch (farmAnimal->typeCounter) {
                                    
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
    
                            farmAnimal->flags |= (FARM_ANIMAL_APPROACHING  | FARM_ANIMAL_FOLLOWING);
                            
                            if (!(farmAnimal->flags & FARM_ANIMAL_TALKED_TO)) {
                                
                                adjustFarmAnimalAffection(i, 1);
                                setAnimalState(2, i, 0xFF, 0xFF, 17);
    
                                farmAnimal->flags |= FARM_ANIMAL_COLLISION_WITH_PLAYER;
                                
                                showAnimalExpressionBubble(COW_TYPE, i, 3);
    
                                farmAnimal->flags |= FARM_ANIMAL_TALKED_TO;
                                
                            }
                            
                        }
    
                        farmAnimal->flags &= ~FARM_ANIMAL_LINGERING;
    
                    }

                    set = TRUE;
                    
                }
                
            }
            
            if (horse->flags & HORSE_ENTITY_LOADED && entities[horse->entityIndex].entityCollidedWithIndex == ENTITY_PLAYER && entities[horse->entityIndex].buttonPressed == BUTTON_A) {

                if (gPlayer.heldItem == 0) {

                    if (horse->grown == TRUE) {

                        if (!checkTerrainCollisionInDirection(ENTITY_PLAYER, 24, convertScreenDirectionToWorldDirection(entities[ENTITY_PLAYER].direction, gMainMapIndex)) 
                            && !checkTerrainCollisionInDirection(ENTITY_PLAYER, 16, convertScreenDirectionToWorldDirection(gPlayer.direction, gMainMapIndex))) {
                            
                            vec = projectEntityPosition(ENTITY_PLAYER, 32, convertScreenDirectionToWorldDirection(gPlayer.direction, MAIN_MAP_INDEX));
                             
                            groundObjectIndex = getGroundObjectIndexFromCoordinates(vec.x, vec.z);

                            if (groundObjectIndex == 0xFF || (getGroundObjectPlayerInteractionsFlags(groundObjectIndex) & 8)) {
                                
                                setPlayerAction(MOUNTING_HORSE, ANIM_MOUNT_HORSE);

                                horse->flags &= ~(HORSE_ENTITY_LOADED | HORSE_FOLLOWING);

                                if (!(horse->flags & HORSE_RODE_DAILY)) {
                                    adjustHorseAffection(1);
                                    horse->flags |= HORSE_RODE_DAILY;
                                }
                                
                                set = TRUE;
                                
                            }
                            
                        }
                        
                    } else {

                        setMessageBoxInterpolationWithFlags(0, -4, 2);
                        setMessageBoxViewSpacePosition(0, 0.0f, -64.0f, 352.0f);
                        setMessageBoxSpriteIndices(0, 1, 0, 0);
                        setGameVariableString(3, horse->name, 6);
                        
                        if (horse->age == 0) {
                            showTextBox(1, ANIMAL_INTERACTIONS_TEXT_INDEX, 7, 0, 2);
                        } else {
                            showTextBox(1, ANIMAL_INTERACTIONS_TEXT_INDEX, 8, 0, 2);
                        }

                        horse->flags |= HORSE_FOLLOWING;
                            
                        if (!(horse->flags & HORSE_TALKED_TO_DAILY)) {

                            horse->actionState = 17;

                            if (checkHaveKeyItem(STAMINA_CARROT)) {
                                adjustHorseAffection(2);
                            } else {
                                adjustHorseAffection(1);
                            }

                            horse->flags |= (HORSE_COLLISION_WITH_PLAYER | HORSE_TALKED_TO_DAILY);
                            
                            showAnimalExpressionBubble(HORSE_TYPE, 0, 3);
                            
                        }
                    
                        set = TRUE;
    
                    }
                    
                }
                
            }

            for (i = 0; i < MAX_MISC_ANIMALS && !set; i++) {
        MiscAnimal *miscAnimal = &gMiscAnimals[i];


                if (miscAnimal->flags & MISC_ANIMAL_ENTITY_LOADED) {

                    if (entities[miscAnimal->entityIndex].entityCollidedWithIndex == ENTITY_PLAYER && entities[miscAnimal->entityIndex].buttonPressed == BUTTON_A) {

                        switch (miscAnimal->animalType) {

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

                                if (!(15 < miscAnimal->actionState && miscAnimal->actionState < 18)) {
                                    gPlayer.heldItem = SQUIRREL_HELD_ITEM;
                                    set = TRUE;
                                }
                                
                                break;
                            
                            case 10:
                                
                                if (!(15 < miscAnimal->actionState && miscAnimal->actionState < 18)) {
                                    gPlayer.heldItem = MONKEY_HELD_ITEM;
                                    set = TRUE;
                                }
                                
                                break;

                            case 16:

                                if (miscAnimal->actionState != 1) {
                                    gPlayer.heldItem = LADYBUG_HELD_ITEM;
                                    set = TRUE;
                                }
                                
                                break;
                            
                            case 17:
                                                                
                                if (miscAnimal->actionState != 1) {
                                    gPlayer.heldItem = CICADA_HELD_ITEM;
                                    set = TRUE;
                                }
                                
                                break;
                            
                            case 18:
                                                                
                                if (miscAnimal->actionState != 1) {
                                    gPlayer.heldItem = HORNED_BEETLE_HELD_ITEM;
                                    set = TRUE;
                                }
                                
                                break;
                            
                            case 19:
                                                                
                                if (miscAnimal->actionState != 1) {
                                    gPlayer.heldItem = STAG_BEETLE_HELD_ITEM;
                                    set = TRUE;
                                }
                                
                                break;
                            
                            case 21:
                                                                
                                if (miscAnimal->actionState != 1) {
                                    gPlayer.heldItem = CRICKET_HELD_ITEM;
                                    set = TRUE;
                                }
                                
                                break;
                            
                            case 15:
                                                                
                                if (miscAnimal->actionState != 1) {
                                    gPlayer.heldItem = BUTTERFLY_HELD_ITEM;
                                    set = TRUE;
                                }
                                
                                break;
                            
                            case 20:

                                if (miscAnimal->actionState != 1) {
                                    gPlayer.heldItem = RED_DRAGONFLY_HELD_ITEM;
                                    set = TRUE;
                                }
                                
                                break;
                            
                        }

                        if (set) {

                            deactivateEntity(miscAnimal->entityIndex);
                            
                            setPlayerAction(PICKING_UP_ITEM, ANIM_PICKING_UP_ITEM);
                            gPlayer.heldAnimalIndex = i;

                            miscAnimal->flags = 0;
                            
                        }

                        if (!checkDailyEventBit(HELD_ANIMAL)) {

                            gHappiness += adjustValue(gHappiness, 1, MAX_HAPPINESS);
                            setDailyEventBit(HELD_ANIMAL);
                            
                        }
                            
                    } else {

                        if (entities[miscAnimal->entityIndex].unk_5E == 0) {
                            
                            switch (miscAnimal->animalType) {

                                case 9:

                                    if (15 < miscAnimal->actionState && miscAnimal->actionState < 18) {
                                        miscAnimal->actionState = 0x11;    
                                    } else {
                                        miscAnimal->actionState = 0x20;
                                        miscAnimal->flags |= MISC_ANIMAL_RUNNING_AWAY;
                                    }
                                    
                                    break;

                                case 10:

                                    if (15 < miscAnimal->actionState && miscAnimal->actionState < 18) {
                                        miscAnimal->actionState = 0x11;
                                    } else {
                                        miscAnimal->actionState = 0x20;
                                        miscAnimal->flags |= MISC_ANIMAL_RUNNING_AWAY;
                                    }
                                    
                                    break;

                                case 7:
                                    miscAnimal->actionState = 0x20;
                                    miscAnimal->flags |= MISC_ANIMAL_RUNNING_AWAY;
                                    break;

                                case 8:
                                    miscAnimal->actionState = 0x20;
                                    miscAnimal->flags |= MISC_ANIMAL_RUNNING_AWAY;
                                    break;

                                case 11:
                                    miscAnimal->actionState = 0x20;
                                    miscAnimal->flags |= MISC_ANIMAL_RUNNING_AWAY;
                                    break;

                                case 12:
                                    miscAnimal->actionState = 0x20;
                                    miscAnimal->flags |= MISC_ANIMAL_RUNNING_AWAY;
                                    break;

                                case 13:
                                    miscAnimal->actionState = 0x20;
                                    miscAnimal->flags |= MISC_ANIMAL_RUNNING_AWAY;
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

void initializeAnimalEntities(void) {

    u8 i;

    if (!checkLifeEventBit(ENTERED_DOG_RACE) || !checkDailyEventBit(DOG_RACE)) {
        initializeDogEntity();
    }

    for (i = 0; i < MAX_CHICKENS; i++) {
        initializeChickenEntity(i);
    }

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {
        initializeFarmAnimalEntity(i);
    }

    if (!checkLifeEventBit(ENTERED_HORSE_RACE) || !checkDailyEventBit(HORSE_RACE)) {
        initializeHorseEntity();
    }

    // other animals
    for (i = 0; i < 7; i++) {
        initializeMiscAnimalEntity(i, 0);
    }
    
}

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

void updateAnimalCoordinates(void) {
    Dog *dog = &dogInfo;
    Horse *horse = &horseInfo;


    u8 i = 0;

    if ((dog->flags & DOG_ENTITY_LOADED) && (dog->location == gBaseMapIndex)) {
        dog->coordinates.x = entities[dog->entityIndex].coordinates.x; 
        dog->coordinates.y = entities[dog->entityIndex].coordinates.y;
        dog->coordinates.z = entities[dog->entityIndex].coordinates.z;
    }

    for (i = 0; i < MAX_CHICKENS; i++) {
        Chicken *chicken = &gChickens[i];


        if ((chicken->flags & CHICKEN_ENTITY_LOADED) && (chicken->location == gBaseMapIndex)) {

            chicken->coordinates.x = entities[chicken->entityIndex].coordinates.x; 
            chicken->coordinates.y = entities[chicken->entityIndex].coordinates.y;
            chicken->coordinates.z = entities[chicken->entityIndex].coordinates.z;
            
        }
        
    }

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {
        FarmAnimal *farmAnimal = &gFarmAnimals[i];


        if ((farmAnimal->flags & FARM_ANIMAL_ENTITY_LOADED) && (farmAnimal->location == gBaseMapIndex)) {

            farmAnimal->coordinates.x = entities[farmAnimal->entityIndex].coordinates.x; 
            farmAnimal->coordinates.y = entities[farmAnimal->entityIndex].coordinates.y;
            farmAnimal->coordinates.z = entities[farmAnimal->entityIndex].coordinates.z;
            
        }
        
    }

    if ((horse->flags & HORSE_ENTITY_LOADED) && (horse->location == gBaseMapIndex)) {
        horse->coordinates.x = entities[horse->entityIndex].coordinates.x; 
        horse->coordinates.y = entities[horse->entityIndex].coordinates.y;
        horse->coordinates.z = entities[horse->entityIndex].coordinates.z;
    }
    
}

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

void feedAllAnimals(void) {

    u8 i;

    for (i = 0; i < MAX_CHICKENS; i++) {
        gChickens[i].flags |= CHICKEN_FED;
    }

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {
        gFarmAnimals[i].flags |= (FARM_ANIMAL_FED | FARM_ANIMAL_ATE_GRASS);
    }
    
}

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

u8 initializeNewChicken(u8 animalType, u8 arg1) {
    Chicken *chicken = &gChickens[arg1];


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

            gChickens[found].location = chicken->location;

            gChickens[found].coordinates.x = chicken->coordinates.x;
            gChickens[found].coordinates.y = chicken->coordinates.y;
            gChickens[found].coordinates.z = chicken->coordinates.z;
            
        }

        setAnimalState(1, found, animalType, 0, 0);

        gChickens[found].direction = 2;
        gChickens[found].flags = CHICKEN_ACTIVE;
        
    }

    return found;
    
}

void initializeChicken(u8 chickenIndex) {
    Chicken *chicken = &gChickens[chickenIndex];


    chicken->location = 0;
    
    chicken->actionState = 0;
    chicken->direction = 0;
    chicken->speed = 0;
    chicken->stateTimer = 0;
    chicken->unk_1B = 0;

    chicken->type = 0;
    chicken->condition = 0;
    chicken->typeCounter = 0;
    chicken->conditionCounter = 0;

    chicken->coordinates.x = 0;
    chicken->coordinates.y = 0;
    chicken->coordinates.z = 0;
    
    chicken->flags = 0;
    
    chicken->name[0] = 0;
    chicken->name[1] = 0;
    chicken->name[2] = 0;
    chicken->name[3] = 0;
    chicken->name[4] = 0;
    chicken->name[5] = 0;
    
}

u8 initializeNewFarmAnimal(u8 animalType, u8 arg1) {
    FarmAnimal *farmAnimal = &gFarmAnimals[arg1];


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

            gFarmAnimals[index].motherName[0] = farmAnimal->name[0];
            gFarmAnimals[index].motherName[1] = farmAnimal->name[1];
            gFarmAnimals[index].motherName[2] = farmAnimal->name[2];
            gFarmAnimals[index].motherName[3] = farmAnimal->name[3];
            gFarmAnimals[index].motherName[4] = farmAnimal->name[4];
            gFarmAnimals[index].motherName[5] = farmAnimal->name[5];
            
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

// initialize watched cows from Doug
void initializeWatchedCows(void) {
    initializeFarmAnimal(mrsManaCow1Index);
    initializeFarmAnimal(mrsManaCow2Index);
    initializeFarmAnimal(mrsManaCow3Index);
}

void setFarmAnimalLocation(u8 animalIndex) {
    FarmAnimal *farmAnimal = &gFarmAnimals[animalIndex];


    farmAnimal->location = BARN;

    if (farmAnimal->type == PREGNANT_COW) {
        
        farmAnimal->coordinates.x = pregnantCowStartingCoordinates.x;
        farmAnimal->coordinates.y = pregnantCowStartingCoordinates.y;
        farmAnimal->coordinates.z = pregnantCowStartingCoordinates.z;
        farmAnimal->direction = DIRECTION_N;

    } else {

        farmAnimal->coordinates.x = farmAnimalStartingCoordinates[animalIndex].x;
        farmAnimal->coordinates.y = farmAnimalStartingCoordinates[animalIndex].y;
        farmAnimal->coordinates.z = farmAnimalStartingCoordinates[animalIndex].z;
        
        if (animalIndex >= 4) {
            farmAnimal->direction = DIRECTION_E;
        } else {
            farmAnimal->direction = DIRECTION_W;
        }

    }
}

void initializeFarmAnimal(u8 animalIndex) {
    FarmAnimal *farmAnimal = &gFarmAnimals[animalIndex];


    farmAnimal->location = 0;
    
    farmAnimal->actionState = 0;
    farmAnimal->direction = 0;
    farmAnimal->speed = 0;
    farmAnimal->stateTimer = 0;
    farmAnimal->unk_1E = 0;
    
    farmAnimal->type = 0;
    farmAnimal->condition = 0;
    farmAnimal->typeCounter = 0;
    farmAnimal->conditionCounter = 0;

    farmAnimal->coordinates.x = 0;
    farmAnimal->coordinates.y = 0;
    farmAnimal->coordinates.z = 0;

    farmAnimal->flags = 0;
    
    farmAnimal->name[0] = 0;
    farmAnimal->name[1] = 0;
    farmAnimal->name[2] = 0;
    farmAnimal->name[3] = 0;
    farmAnimal->name[4] = 0;
    farmAnimal->name[5] = 0;
    
}

void initializeDog(void) {
    Dog *dog = &dogInfo;


    dog->location = FARM;
    dog->coordinates.y = 0.0f;
    dog->actionState = 0;
    dog->direction = DIRECTION_S;
    dog->affection = 0; 
    dog->speed = 0;
    dog->stateTimer = 0;
    dog->unk_1B = 0;
    dog->flags = DOG_ACTIVE;
    dog->coordinates.x = -432.0f;
    dog->coordinates.z = 96.0f;
    
}

void initializeHorse(void) {
    Horse *horse = &horseInfo;

    
    horse->location = FARM;
    horse->coordinates.y = 0.0f;
    horse->actionState = 0;
    horse->direction = DIRECTION_S;
    horse->speed = 0;
    horse->stateTimer = 0;
    horse->unk_1B = 0;
    horse->grown = 0;
    horse->age = 0;
    horse->affection = 0;
    horse->flags = HORSE_ACTIVE;
    horse->coordinates.x = -240.0f;
    horse->coordinates.z = -384.0f;

}

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

void setDogLocation(u8 mapIndex) {
    Dog *dog = &dogInfo;


    if (dog->flags & DOG_ACTIVE && (mapIndex == 0xFF || dog->location == mapIndex)) {
        dog->location = FARM;
        dog->coordinates.y = 0.0f;
        dog->direction = DIRECTION_S;
        dog->flags &= ~DOG_HELD;
        dog->coordinates.x = -432.0f;
        dog->coordinates.z = 96.0f;
    }

}

void setHorseLocation(u8 mapIndex) {
    Horse *horse = &horseInfo;


    if (horse->flags & HORSE_ACTIVE && (mapIndex == 0xFF || horse->location == mapIndex)) {
        horse->coordinates.y = 0;
        horse->location = FARM;
        horse->direction = DIRECTION_S;
        horse->coordinates.x = -240.0f;
        horse->coordinates.z = -384.0f;
    }

}

void resetFarmAnimalLocation(u8 mapIndex, u8 animalIndex) {

    if ((gFarmAnimals[animalIndex].flags & FARM_ANIMAL_ACTIVE) && (mapIndex == 0xFF ||  gFarmAnimals[animalIndex].location == mapIndex)) {
        setFarmAnimalLocation(animalIndex);
    }
    
}

void resetChickenLocation(u8 mapIndex, u8 chickenIndex) {
    Chicken *chicken = &gChickens[chickenIndex];


    if ((chicken->flags & FARM_ANIMAL_ACTIVE) && (mapIndex == 0xFF || chicken->location == mapIndex)) {

        chicken->location = COOP;

        if (chicken->flags & CHICKEN_EGG_INCUBATING) {

            chicken->coordinates.x = 96.0f;
            chicken->coordinates.y = 0;
            chicken->coordinates.z = -144.0f;
            
        } else {

            chicken->coordinates.x = chickenStartingCoordinates[chickenIndex].x;
            chicken->coordinates.y = chickenStartingCoordinates[chickenIndex].y;
            chicken->coordinates.z = chickenStartingCoordinates[chickenIndex].z;
            
        }

        chicken->flags &= ~CHICKEN_HELD;
        
    }
    
}

void spawnMiscAnimals(void) {

    switch (gBaseMapIndex) {
        
        case FARM:

            if (gSeason != WINTER && gWeather == SUNNY && 5 < gHour && gHour < 9) {

                if (gMiscAnimals[4].spawnVariant == 1) {
                    spawnMiscAnimal(MISC_ANIMAL_SPARROW, DIRECTION_S, -336.0f, 0.0f, 0.0f);
                    spawnMiscAnimal(MISC_ANIMAL_SPARROW, DIRECTION_S, -368.0f, 0.0f, 32.0f);
                }

                if (gMiscAnimals[4].spawnVariant == 2) {
                    spawnMiscAnimal(MISC_ANIMAL_SPARROW, DIRECTION_S, 320.0f, 0.0f, -352.0f);
                    spawnMiscAnimal(MISC_ANIMAL_SPARROW, DIRECTION_S, 288.0f, 0.0f, -352.0f);
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
                gMiscAnimals[spawnMiscAnimal(MISC_ANIMAL_SQUIRREL, DIRECTION_S, 144.0f, 0.0f, -256.0f)].actionState = 0x10;
            }

            if (gSeason == WINTER && 10 < gHour && gHour < 17 && gMiscAnimals[3].spawnVariant >= 2 && (u8)(gPlayer.heldItem + 0x5D) >= 8) {
                spawnMiscAnimal(MISC_ANIMAL_MONKEY, DIRECTION_E, -192.0f, 0.0f, -224.0f);
            }

            if (gSeason == SUMMER && gWeather != RAIN && 7 < gHour && gHour < 16 && gMiscAnimals[4].spawnVariant < 2) {
                spawnMiscAnimal(MISC_ANIMAL_SNAKE, DIRECTION_S, 96.0f, 0.0f, -64.0f);
            }

            break;

        case MOUNTAIN_1:

            if (gSeason == SUMMER && 10 < gHour && gHour < 17 && gMiscAnimals[1].spawnVariant < 2 && (u8)(gPlayer.heldItem + 0x65) >= 8) {
                spawnMiscAnimal(MISC_ANIMAL_BUNNY, DIRECTION_W, 128.0f, 0.0f, 64.0f);
            }

            if (gSeason == SPRING && 6 < gHour && gHour < 16 && !(gMiscAnimals[2].spawnVariant < 2) && (u8)(gPlayer.heldItem + 0x6D) >= 8) {
                spawnMiscAnimal(MISC_ANIMAL_SQUIRREL, DIRECTION_S, 256.0f, 0.0f, -416.0f);
            }
            
            if (gSeason == SUMMER && 10 < gHour && gHour < 17 && gMiscAnimals[2].spawnVariant < 1 && (u8)(gPlayer.heldItem + 0x6D) >= 8) {
                spawnMiscAnimal(MISC_ANIMAL_SQUIRREL, DIRECTION_S, 160.0f, 0.0f, 128.0f);
            }

            if (gSeason == AUTUMN && 10 < gHour && gHour < 17 && gMiscAnimals[3].spawnVariant == 1 && (u8)(gPlayer.heldItem + 0x5D) >= 8) {
                spawnMiscAnimal(MISC_ANIMAL_MONKEY, DIRECTION_S, -96.0f, 0.0f, 128.0f);
            }

            if (gSeason == SUMMER && gWeather != RAIN && 7 < gHour && gHour < 16 && !(gMiscAnimals[4].spawnVariant < 2)) {
                spawnMiscAnimal(MISC_ANIMAL_SNAKE, DIRECTION_S, 160.0f, 0.0f, 128.0f);
            }

            if (gSeason == AUTUMN && gWeather != RAIN && 8 < gHour && gHour < 16 && !(gMiscAnimals[5].spawnVariant < 2) && gPlayer.heldItem != RED_DRAGONFLY_HELD_ITEM) {
                spawnMiscAnimal(MISC_ANIMAL_DRAGONFLY, DIRECTION_S, 0.0f, 0.0f, -128.0f);
            }

            if (gSeason == AUTUMN && gWeather != RAIN && 8 < gHour && gHour < 16 && gMiscAnimals[4].spawnVariant < 2 && gPlayer.heldItem != CRICKET_HELD_ITEM) {
                spawnMiscAnimal(MISC_ANIMAL_CRICKET, DIRECTION_S, 128.0f, 0.0f, 64.0f);
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
                spawnMiscAnimal(MISC_ANIMAL_BUNNY, DIRECTION_N, -192.0f, 0.0f, -192.0f);
            }

            if (gSeason == SPRING && 6 < gHour && gHour < 16 && gMiscAnimals[2].spawnVariant < 2 && (u8)(gPlayer.heldItem + 0x6D) >= 8) {
                spawnMiscAnimal(MISC_ANIMAL_SQUIRREL, DIRECTION_S, -224.0f, 0.0f, -160.0f);
            }

            if (gSeason == SUMMER && 10 < gHour && gHour < 17 && gMiscAnimals[3].spawnVariant < 2 && (u8)(gPlayer.heldItem + 0x5D) >= 8) {
                spawnMiscAnimal(MISC_ANIMAL_MONKEY, DIRECTION_S, -320.0f, 0.0f, -320.0f);
            }

            if (gSeason == SUMMER && 10 < gHour && gHour < 17 && !(gMiscAnimals[3].spawnVariant < 2) && (u8)(gPlayer.heldItem + 0x5D) >= 8) {
                spawnMiscAnimal(MISC_ANIMAL_MONKEY, DIRECTION_S, 0.0f, 0.0f, 288.0f);
            }

            if (gSeason == AUTUMN && 10 < gHour && gHour < 17 && gMiscAnimals[3].spawnVariant == 0 && (u8)(gPlayer.heldItem + 0x5D) >= 8) {
                gMiscAnimals[spawnMiscAnimal(MISC_ANIMAL_MONKEY, DIRECTION_S, -112.0f, 0.0f, -160.0f)].actionState = 0x10;
            }

            if (gSeason != WINTER && gWeather != RAIN && 7 < gHour && gHour < 16 && gMiscAnimals[4].spawnVariant < 2) {
                spawnMiscAnimal(MISC_ANIMAL_BIRD, DIRECTION_S, -128.0f, 0.0f, 32.0f);
                spawnMiscAnimal(MISC_ANIMAL_BIRD, DIRECTION_E, -96.0f, 0.0f, 64.0f);
            }

            if (gSeason < AUTUMN && gWeather != RAIN && 8 < gHour && gHour < 16 && gPlayer.heldItem < 0xB4) {
                spawnMiscAnimal(MISC_ANIMAL_LADYBUG, DIRECTION_S, 96.0f, 0.0f, -96.0f);
            }

            if (gSeason == SUMMER && gWeather != RAIN && 8 < gHour && gHour < 16 && gMiscAnimals[3].spawnVariant < 2 && gPlayer.heldItem != CICADA_HELD_ITEM) {
                spawnMiscAnimal(MISC_ANIMAL_CICADA, DIRECTION_S, -112.0f, 0.0f, -160.0f);
            }
            
            break;
        
        case TOP_OF_MOUNTAIN_1:

            if (gSeason == SUMMER && gWeather != RAIN && 8 < gHour && gHour < 16 && gMiscAnimals[0].spawnVariant < 2 && gPlayer.heldItem != HORNED_BEETLE_HELD_ITEM) {
                spawnMiscAnimal(MISC_ANIMAL_HORNED_BEETLE, DIRECTION_S, 192.0f, 0.0f, -192.0f);
            }

            if (gSeason == SUMMER && gWeather != RAIN && 8 < gHour && gHour < 16 && gMiscAnimals[1].spawnVariant < 2 && gPlayer.heldItem != STAG_BEETLE_HELD_ITEM) {
                spawnMiscAnimal(MISC_ANIMAL_STAG_BEETLE, DIRECTION_S, 224.0f, 0.0f, 0.0f);
            }

            break;
        
        case MOON_MOUNTAIN:

            if (gSeason != WINTER && gWeather != RAIN && 7 < gHour && gHour < 16 && !(gMiscAnimals[4].spawnVariant < 2)) {
                spawnMiscAnimal(MISC_ANIMAL_BIRD, DIRECTION_S, 192.0f, 0.0f, -64.0f);
                spawnMiscAnimal(MISC_ANIMAL_BIRD, DIRECTION_E, 224.0f, 0.0f, -96.0f);
            }

            if (gSeason == AUTUMN && gWeather != RAIN && 8 < gHour && gHour < 16 && gMiscAnimals[5].spawnVariant == 0 && gPlayer.heldItem != RED_DRAGONFLY_HELD_ITEM) {
                spawnMiscAnimal(MISC_ANIMAL_DRAGONFLY, DIRECTION_S, 128.0f, 0.0f, -256.0f);
            }

            break;
        
        case SQUARE:

            if (gSeason != WINTER && gWeather == SUNNY && 5 < gHour && gHour < 9 && gMiscAnimals[4].spawnVariant == 3) {
                spawnMiscAnimal(MISC_ANIMAL_SPARROW, DIRECTION_S, -128.0f, 0.0f, 0.0f);
                spawnMiscAnimal(MISC_ANIMAL_SPARROW, DIRECTION_S, -96.0f, 0.0f, -32.0f);
            }

            break;
        
        case BEACH:

            if (gSeason == SUMMER && gWeather != RAIN && 7 < gHour && gHour < 16) {
                spawnMiscAnimal(MISC_ANIMAL_CRAB, DIRECTION_S, -256.0f, 0.0f, -64.0f);
            }

            break;

        case VILLAGE_1:
                
            if (gSeason < AUTUMN && gWeather != RAIN && 8 < gHour && gHour < 16 && gMiscAnimals[0].spawnVariant == 0 && gPlayer.heldItem != BUTTERFLY_HELD_ITEM) {
                spawnMiscAnimal(MISC_ANIMAL_WHITE_BUTTERFLY, DIRECTION_S, 64.0f, 0.0f, -352.0f);
            }

            if (gSeason < AUTUMN && gWeather != RAIN && 8 < gHour && gHour < 16 && gMiscAnimals[0].spawnVariant == 1 && gPlayer.heldItem != BUTTERFLY_HELD_ITEM) {
                spawnMiscAnimal(MISC_ANIMAL_WHITE_BUTTERFLY, DIRECTION_S, -448.0f, 0.0f, 0.0f);
            }

            if (gSeason < AUTUMN && gWeather != RAIN && 8 < gHour && gHour < 16 && gMiscAnimals[5].spawnVariant == 1 && gPlayer.heldItem < LADYBUG_HELD_ITEM) {
                spawnMiscAnimal(MISC_ANIMAL_LADYBUG, DIRECTION_S, -448.0f, 0.0f, 64.0f);
            }
        
            break;

        case VILLAGE_2:

            if (8 < gHour && gHour < 18) {

                if (gWeather != RAIN && gSeason != WINTER) {
                    
                    if ((u8)(gPlayer.heldItem + 0x55) >= 8) {
                        spawnMiscAnimal(0, DIRECTION_S, 400.0f, 0.0f, -160.0f);
                    }
    
                    if (checkLifeEventBit(PUPPIES) && (u8)(gPlayer.heldItem - 0x7B) >= 8) {
                        spawnMiscAnimal(1, DIRECTION_S, 432.0f, 0.0f, -160.0f);
                    }
                    
                }

                if (8 < gHour && gHour < 18 && !(SUNNY < gWeather && gWeather < 4)) {
                    
                    if ((u8)(gPlayer.heldItem + 0x7D) >= 8) {
                        spawnMiscAnimal(2, DIRECTION_S, 32.0f, 0.0f, -96.0f);
                    }
                
                    spawnMiscAnimal(2, DIRECTION_S, 0.0f, 0.0f, -64.0f);
                
                }
                
            }

            break;

        case POTION_SHOP:

            if (gWeather == RAIN || gSeason == WINTER)  {

                if ((u8)(gPlayer.heldItem + 0x55) >= 8) {
                    spawnMiscAnimal(0, DIRECTION_S, -64.0f, 0.0f, -64.0f);
                }

                if (checkLifeEventBit(PUPPIES) && (u8)(gPlayer.heldItem - 0x7B) >= 8) {
                    spawnMiscAnimal(1, DIRECTION_S, -96.0f, 0.0f, -64.0f);
                }
                
            }
            
            break;
        
        case RANCH:

            if (gMiscAnimals[0].spawnVariant < 2 && gWeather != RAIN && gSeason != WINTER) {
                spawnMiscAnimal(4, DIRECTION_N, -384.0f, 0.0f, 32.0f);
            }

            if (gMiscAnimals[1].spawnVariant < 2 && gWeather != RAIN && gSeason != WINTER) {
                spawnMiscAnimal(3, DIRECTION_E, -320.0f, 0.0f, 0.0f);
            }
   
            if (gMiscAnimals[2].spawnVariant < 2 && gWeather != RAIN && gSeason != WINTER) {
                spawnMiscAnimal(3, DIRECTION_S, -288.0f, 0.0f, 96.0f);
            }         
   
            if (gMiscAnimals[3].spawnVariant < 2 && gWeather != RAIN && gSeason != WINTER) {
                spawnMiscAnimal(6, DIRECTION_N, -288.0f, 0.0f, 128.0f);
            }         
   
            if (gMiscAnimals[4].spawnVariant < 2 && gWeather != RAIN && gSeason != WINTER) {
                spawnMiscAnimal(5, DIRECTION_E, -352.0f, 0.0f, 80.0f);
            }         

            break;
        
        case RANCH_BARN:

            if (gMiscAnimals[0].spawnVariant >= 2 || gWeather == RAIN || gSeason == WINTER) {
                spawnMiscAnimal(4, DIRECTION_W, -96.0f, 0.0f, -192.0f);
            }

            if (gMiscAnimals[1].spawnVariant >= 2 || gWeather == RAIN || gSeason == WINTER) {
                spawnMiscAnimal(3, DIRECTION_W, -96.0f, 0.0f, -128.0f);
            }

            if (gMiscAnimals[2].spawnVariant >= 2 || gWeather == RAIN || gSeason == WINTER) {
                spawnMiscAnimal(3, DIRECTION_W, -96.0f, 0.0f, 64.0f);
            }

            if (gMiscAnimals[3].spawnVariant >= 2 || gWeather == RAIN || gSeason == WINTER) {
                spawnMiscAnimal(6, DIRECTION_E, 0.0f, 0.0f, 64.0f);
            }

            if (gMiscAnimals[4].spawnVariant >= 2 || gWeather == RAIN || gSeason == WINTER) {
                spawnMiscAnimal(5, DIRECTION_E, 0.0f, 0.0f, 128.0f);
            }
            
            break;
        
    }
    
}

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

void updateDogAffectionIfFed(void) {
    
    if (checkDailyEventBit(FED_DOG) && (dogInfo.flags & DOG_ACTIVE)) {
        dogInfo.affection += adjustValue(dogInfo.affection, 2, MAX_AFFECTION);
    }
    
}

void updateChickenStartOfDay(u8 index) {
    Chicken *chicken = &gChickens[index];

    
    if ((chicken->flags & CHICKEN_ACTIVE) && !(chicken->flags & CHICKEN_NEWBORN)) {
        
        if (chicken->flags & 0x80) {
            
            chicken->flags &= ~CHICKEN_FED;

        } else {

            if (chicken->location != COOP) {
                chicken->flags &= ~CHICKEN_FED;
            }

            if (chicken->location == FARM) {
                
                if (gSeason != WINTER) {
                    chicken->flags |= CHICKEN_FED;
                } 

            } 

        }

        switch (chicken->type) {
        
            case CHICKEN_EGG:
                
                if (chicken->flags & CHICKEN_EGG_INCUBATING) {
                    
                    chicken->typeCounter++;
                    
                    if (chicken->typeCounter == CHICKEN_EGG_INCUBATION_DURATION) {
                        
                        bornChickenIndex = initializeNewChicken(1, 0xFF);
                        
                        if (bornChickenIndex != 0xFF) {

                            gChickens[bornChickenIndex].flags |= CHICKEN_NEWBORN;
                            chicken->flags &= ~CHICKEN_ACTIVE;

                            setLifeEventBit(CHICKEN_BORN);
                            
                        } else {                        
                            chicken->typeCounter--;
                        }
                        
                    }
                    
                }
                
                break;
            
            case CHICK:
                
                chicken->typeCounter++;
                
                if (chicken->typeCounter == CHICK_DURATION) {
                    setAnimalState(1, index, ADULT_CHICKEN, 0, 0);
                }
                
                break;
            
            case ADULT_CHICKEN:
                
                switch (chicken->condition) {
                    
                    case CHICKEN_NORMAL:
                        
                        if ((chicken->flags & CHICKEN_FED)) {
                            initializeNewChicken(0, index);
                        } else {
                            setAnimalState(1, index, 0xFF, CHICKEN_STARVED, 0);
                        }
                        
                        break;
                    
                    case CHICKEN_STARVED:
                        
                        if (chicken->flags & CHICKEN_FED) {
                            setAnimalState(1, index, 0xFF, CHICKEN_NORMAL, 0);
                        } else {
                            
                            chicken->conditionCounter++;
                            
                            if (chicken->conditionCounter == 3) {
                                
                                setAnimalState(1, index, 0xFF, CHICKEN_DEAD, 0xFF);
                                
                                chicken->flags = 0;

                                setLifeEventBit(ANIMAL_DIED);
                                
                                gDeadAnimalName[0] = chicken->name[0];
                                gDeadAnimalName[1] = chicken->name[1];
                                gDeadAnimalName[2] = chicken->name[2];
                                gDeadAnimalName[3] = chicken->name[3];
                                gDeadAnimalName[4] = chicken->name[4];
                                gDeadAnimalName[5] = chicken->name[5];

                            }
                            
                        }
                        
                        break;
                    
                }

            break;
            
        }
        
    }
    
}

void updateFarmAnimalStartOfDay(u8 index) {
    FarmAnimal *farmAnimal = &gFarmAnimals[index];

    
    if ((farmAnimal->flags & FARM_ANIMAL_ACTIVE) && !(farmAnimal->flags & FARM_ANIMAL_NEWBORN)) {
        
        if (farmAnimal->flags & 0x4000) {
            
            farmAnimal->flags &= ~(FARM_ANIMAL_FED | FARM_ANIMAL_ATE_GRASS);

        } else if (farmAnimal->location == FARM) {
            
            farmAnimal->flags &= ~(FARM_ANIMAL_FED | FARM_ANIMAL_ATE_GRASS);
            
            if (checkAnimalCanEatGrass()) {
                farmAnimal->flags |= (FARM_ANIMAL_FED | FARM_ANIMAL_ATE_GRASS);
            }
            
        } else {
            adjustFarmAnimalAffection(index, -1);
        }

        if (gCowFestivalEnteredCowIndex!= 0xFF) {
            
            if (gCowFestivalEnteredCowIndex== index) {
                gFarmAnimals[gCowFestivalEnteredCowIndex].flags |= (FARM_ANIMAL_FED | FARM_ANIMAL_ATE_GRASS);
            }
            
        }

        switch (farmAnimal->type) {
            
            case BABY_COW:
                farmAnimal->typeCounter++;
                
                if (farmAnimal->typeCounter == COW_INFANCY_DURATION) {
                    setAnimalState(2, index, CALF, 0xFF, 0);
                }
                
                break;
            
            case CALF:
                farmAnimal->typeCounter++;
                
                if (farmAnimal->typeCounter == COW_YOUTH_DURATION) {
                    setAnimalState(2, index, ADULT_COW, 0xFF, 0);
                }
                
                break;
            
            case ADULT_COW:
                
                switch (farmAnimal->condition) {
                    
                    case COW_NORMAL:
                        
                        if (!(farmAnimal->flags & FARM_ANIMAL_FED)) {
                            
                            adjustFarmAnimalAffection(index, -8);
                            
                            if (!(getRandomNumberInRange(0, 1))) {
                                setAnimalState(2, index, 0xFF, COW_SICK, 0);
                                adjustFarmAnimalAffection(index, -30);
                                gHappiness += adjustValue(gHappiness, -10, 0xFF);
                            }
        
                        } else if (farmAnimal->location == FARM) {
                            
                            if (!(getRandomNumberInRange(0, 3))) {
                                setAnimalState(2, index, 0xFF, COW_HAPPY, 0);
                                adjustFarmAnimalAffection(index, 30);
                                gHappiness += adjustValue(gHappiness, 5, 0xFF);
                            }
                            
                        }
                        
                        if (farmAnimal->flags & FARM_ANIMAL_PREGNANT) {
                            setAnimalState(2, index, PREGNANT_COW, 0xFF, 0);
                        }
                        
                        break;

                    case COW_HAPPY:

                        farmAnimal->conditionCounter++;
                            
                        if (farmAnimal->conditionCounter == 3) {
                            setAnimalState(2, index, 0xFF, COW_NORMAL, 0);
                        }
                        
                        break;
                    
                    case COW_MAD:
                        
                        if (farmAnimal->flags & FARM_ANIMAL_FED) {
                            
                            adjustFarmAnimalAffection(index, -8);
                            
                            farmAnimal->conditionCounter++;
                            
                            if (farmAnimal->conditionCounter == 3) {
                                setAnimalState(2, index, 0xFF, COW_NORMAL, 0);
                            }
                            
                        }
                        
                        break;

                    case COW_SICK:
                        
                        farmAnimal->conditionCounter++;
                        
                        if (farmAnimal->conditionCounter == ANIMAL_DEATH_COUNT) {
                            
                            setAnimalState(2, index, 0xFF, COW_DEAD, 0xFF);

                            farmAnimal->flags = 0;
                            
                            setLifeEventBit(ANIMAL_DIED);
                            
                            gDeadAnimalName[0] = farmAnimal->name[0];
                            gDeadAnimalName[1] = farmAnimal->name[1];
                            gDeadAnimalName[2] = farmAnimal->name[2];
                            gDeadAnimalName[3] = farmAnimal->name[3];
                            gDeadAnimalName[4] = farmAnimal->name[4];
                            gDeadAnimalName[5] = farmAnimal->name[5];
                            
                        }
                        
                        break;
                    
                }
            
                break;
            
            case PREGNANT_COW:
                
                farmAnimal->typeCounter++;
                
                if (farmAnimal->typeCounter == COW_PREGNANCY_DURATION) {
                    
                    bornAnimalIndex = initializeNewFarmAnimal(0, index);
                    
                    if (bornAnimalIndex != 0xFF) {
    
                        gFarmAnimals[bornAnimalIndex].flags |= FARM_ANIMAL_NEWBORN;
                        gFarmAnimals[bornAnimalIndex].affection = farmAnimal->affection / 2;
                        
                        setAnimalState(2, index, ADULT_COW, 0xFF, 0);
                        setLifeEventBit(FARM_ANIMAL_BORN);
                    
                    } else {
                        farmAnimal->typeCounter--;
                    }
                    
                }
                
                break;
            
            case BABY_SHEEP:
                
                farmAnimal->typeCounter++;
                
                if (farmAnimal->typeCounter == SHEEP_YOUTH_DURATION) {
                    setAnimalState(2, index, ADULT_SHEEP, 0xFF, 0);
                }
    
                break;
            
            case SHEARED_SHEEP:
                
                farmAnimal->typeCounter++;
                
                if (farmAnimal->typeCounter == WOOL_REGROW_DURATION) {
                    setAnimalState(2, index, ADULT_SHEEP, 0xFF, 0);
                }
                
                switch (farmAnimal->condition) {
                    
                    case SHEEP_NORMAL:
                        
                        if (!(farmAnimal->flags & FARM_ANIMAL_FED)) {
                            
                            adjustFarmAnimalAffection(index, -8);
                            
                            if (!(getRandomNumberInRange(0, 1))) {
                                
                                setAnimalState(2, index, 0xFF, SHEEP_SICK, 0);
                                adjustFarmAnimalAffection(index, -30);
                                
                                gHappiness += adjustValue(gHappiness, -10, 0xFF);
                                
                            }
                            
                        }
                        
                        break;
                    
                    case SHEEP_SICK:
                        
                        farmAnimal->conditionCounter++;
                        
                        if (farmAnimal->conditionCounter == ANIMAL_DEATH_COUNT) {
                            
                            setAnimalState(2, index, 0xFF, SHEEP_DEAD, 0xFF);
                            farmAnimal->flags = 0;
                            
                            setLifeEventBit(ANIMAL_DIED);
                            
                            gDeadAnimalName[0] = farmAnimal->name[0];
                            gDeadAnimalName[1] = farmAnimal->name[1];
                            gDeadAnimalName[2] = farmAnimal->name[2];
                            gDeadAnimalName[3] = farmAnimal->name[3];
                            gDeadAnimalName[4] = farmAnimal->name[4];
                            gDeadAnimalName[5] = farmAnimal->name[5];
                            
                        }
                        
                        break;
                    
                }
                
                break;
            
            case ADULT_SHEEP:
                
                switch (farmAnimal->condition) {
                    
                    case SHEEP_NORMAL:

                        if (!(farmAnimal->flags & FARM_ANIMAL_FED)) {
                            
                            adjustFarmAnimalAffection(index, -8);
                            
                            if (!(getRandomNumberInRange(0, 1))) {
                                
                                setAnimalState(2, index, 0xFF, SHEEP_SICK, 0);
                                adjustFarmAnimalAffection(index, -30);
                                gHappiness += adjustValue(gHappiness, -10, 0xFF);
                                
                            }
                        }
                        
                        break;
    
                    case SHEEP_SICK:
                        
                        farmAnimal->conditionCounter++;
                        
                        if (farmAnimal->conditionCounter == ANIMAL_DEATH_COUNT) {
                            
                            setAnimalState(2, index, 0xFF, SHEEP_DEAD, 0xFF);
                            farmAnimal->flags = 0;
                            
                            setLifeEventBit(ANIMAL_DIED);
                            
                            gDeadAnimalName[0] = farmAnimal->name[0];
                            gDeadAnimalName[1] = farmAnimal->name[1];
                            gDeadAnimalName[2] = farmAnimal->name[2];
                            gDeadAnimalName[3] = farmAnimal->name[3];
                            gDeadAnimalName[4] = farmAnimal->name[4];
                            gDeadAnimalName[5] = farmAnimal->name[5];
                            
                        }
                        
                        break;
                    
                }
    
                break;
            
        }
        
    }
    
}

void updateHorseAge(void) {
    Horse *horse = &horseInfo;


    if ((horse->flags & HORSE_ACTIVE) && horse->grown == FALSE) {
        
        horse->age++;
    
        if (horse->age == 20) {
            horse->grown = TRUE;
            setLifeEventBit(HORSE_GROWN);
        }
    
    }
    
}

void initializeDogEntity(void) {
    Dog *dog = &dogInfo;

    
    if ((dog->flags & DOG_ACTIVE) && (dog->location == gBaseMapIndex) && !(dog->flags & DOG_HELD)) {
        
        dog->entityIndex = 1;
        
        loadEntity(1, ENTITY_ASSET_PLAYER_DOG, TRUE);
        setEntityCollidable(dog->entityIndex, TRUE);
        setEntityYMovement(dog->entityIndex, TRUE);
        setCameraTrackingEntity(dog->entityIndex, FALSE);
        
        setEntityDirection(dog->entityIndex, convertWorldDirectionToScreenDirection(dog->direction, MAIN_MAP_INDEX));
        setEntityCoordinates(dog->entityIndex, dog->coordinates.x, dog->coordinates.y, dog->coordinates.z);
        
        dog->actionState = 0;
        dog->stateTimer = 0;
        dog->flags |= DOG_ENTITY_LOADED;
        
    }
    
    updateDog();
    
}

void initializeChickenEntity(u8 chickenIndex) {
    Chicken *chicken = &gChickens[chickenIndex];


    if ((chicken->flags & CHICKEN_ACTIVE) && (chicken->location == gBaseMapIndex) && !(chicken->flags & CHICKEN_HELD)) {
    
        chicken->entityIndex = chickenIndex + 2;
        
        switch (chicken->type) {

            case ADULT_CHICKEN:
                initializeAnimalEntity(chickenIndex + 2, (u16*)ENTITY_SLOTS_2_7_PALETTE, (AnimationFrameMetadata*)ENTITY_SLOTS_2_7_ANIM_METADATA, (u32*)ENTITY_SLOTS_2_7_SPRITESHEET_INDEX, (u32*)ENTITY_SLOTS_2_7_TEXTURE_TO_PALETTE_LOOKUP);
                loadEntity(chicken->entityIndex, 0x44, TRUE);
                break;         
            
            case CHICK:
                initializeAnimalEntity(chickenIndex + 2, (u16*)ENTITY_CHICK_PALETTE, (AnimationFrameMetadata*)ENTITY_CHICK_ANIM_METADATA, (u32*)ENTITY_CHICK_SPRITESHEET_INDEX, (u32*)ENTITY_CHICK_TEXTURE_TO_PALETTE_LOOKUP);
                loadEntity(chicken->entityIndex, 0x43, TRUE);
                break;
            
            case CHICKEN_EGG:
                initializeAnimalEntity(chickenIndex + 2, (u16*)ENTITY_SLOTS_8_13_PALETTE, (AnimationFrameMetadata*)ENTITY_SLOTS_8_13_ANIM_METADATA, (u32*)ENTITY_SLOTS_8_13_SPRITESHEET_INDEX, (u32*)ENTITY_SLOTS_8_13_TEXTURE_TO_PALETTE_LOOKUP);
                loadEntity(chicken->entityIndex, 0x5D, TRUE);
                break;
            
            default:
                break;
            
        }

        setEntityCollidable(chicken->entityIndex, TRUE);
        setEntityYMovement(chicken->entityIndex, TRUE);
        setCameraTrackingEntity(chicken->entityIndex, FALSE);

        setEntityDirection(chicken->entityIndex, convertWorldDirectionToScreenDirection(chicken->direction, MAIN_MAP_INDEX));        
        setEntityCoordinates(chicken->entityIndex, chicken->coordinates.x, chicken->coordinates.y, chicken->coordinates.z);
        
        chicken->actionState = 0;
        chicken->flags |= CHICKEN_ENTITY_LOADED;
        
    }
    
    updateChicken(chickenIndex);
    
}

void initializeFarmAnimalEntity(u8 index) {
    FarmAnimal *farmAnimal = &gFarmAnimals[index];


    if ((farmAnimal->flags & FARM_ANIMAL_ACTIVE) && (gCowFestivalEnteredCowIndex!= index) && (farmAnimal->location == gBaseMapIndex)) {
        
        farmAnimal->entityIndex = index + 21;
        
        switch (farmAnimal->type) {

            case BABY_COW:
                initializeAnimalEntity(farmAnimal->entityIndex, (u16*)ENTITY_BABY_COW_PALETTE, (AnimationFrameMetadata*)ENTITY_BABY_COW_ANIM_METADATA, (u32*)ENTITY_BABY_COW_SPRITESHEET_INDEX, (u32*)ENTITY_BABY_COW_TEXTURE_TO_PALETTE_LOOKUP);
                loadEntity(farmAnimal->entityIndex, 0x45, TRUE);
                break;
            case CALF:
                initializeAnimalEntity(farmAnimal->entityIndex, (u16*)ENTITY_CALF_PALETTE, (AnimationFrameMetadata*)ENTITY_CALF_ANIM_METADATA, (u32*)ENTITY_CALF_SPRITESHEET_INDEX, (u32*)ENTITY_CALF_TEXTURE_TO_PALETTE_LOOKUP);
                loadEntity(farmAnimal->entityIndex, 0x46, TRUE);
                break;
            case ADULT_COW:
                initializeAnimalEntity(farmAnimal->entityIndex, (u16*)ENTITY_SLOTS_21_28_PALETTE, (AnimationFrameMetadata*)ENTITY_SLOTS_21_28_ANIM_METADATA, (u32*)ENTITY_SLOTS_21_28_SPRITESHEET_INDEX, (u32*)ENTITY_SLOTS_21_28_TEXTURE_TO_PALETTE_LOOKUP);
                loadEntity(farmAnimal->entityIndex, 0x47, TRUE);
                break;
            case PREGNANT_COW:
                initializeAnimalEntity(farmAnimal->entityIndex, (u16*)ENTITY_PREGNANT_COW_PALETTE, (AnimationFrameMetadata*)ENTITY_PREGNANT_COW_ANIM_METADATA, (u32*)ENTITY_PREGNANT_COW_SPRITESHEET_INDEX, (u32*)ENTITY_PREGNANT_COW_TEXTURE_TO_PALETTE_LOOKUP);
                loadEntity(farmAnimal->entityIndex, 0x57, TRUE);
                break;
            case BABY_SHEEP:
                initializeAnimalEntity(farmAnimal->entityIndex, (u16*)ENTITY_BABY_SHEEP_PALETTE, (AnimationFrameMetadata*)ENTITY_BABY_SHEEP_ANIM_METADATA, (u32*)ENTITY_BABY_SHEEP_SPRITESHEET_INDEX, (u32*)ENTITY_BABY_SHEEP_TEXTURE_TO_PALETTE_LOOKUP);
                loadEntity(farmAnimal->entityIndex, 0x48, TRUE);
                break;
            case ADULT_SHEEP:
            case SHEARED_SHEEP:
                initializeAnimalEntity(farmAnimal->entityIndex, (u16*)ENTITY_ADULT_SHEEP_PALETTE, (AnimationFrameMetadata*)ENTITY_ADULT_SHEEP_ANIM_METADATA, (u32*)ENTITY_ADULT_SHEEP_SPRITESHEET_INDEX, (u32*)ENTITY_ADULT_SHEEP_TEXTURE_TO_PALETTE_LOOKUP);
                loadEntity(farmAnimal->entityIndex, 0x49, TRUE);
                break;

        }

        if (farmAnimal->location == BARN) {
            setFarmAnimalLocation(index);
        }
        
        setEntityCollidable(farmAnimal->entityIndex, TRUE);
        setEntityYMovement(farmAnimal->entityIndex, TRUE);
        setCameraTrackingEntity(farmAnimal->entityIndex, FALSE);

        setEntityDirection(farmAnimal->entityIndex, convertWorldDirectionToScreenDirection(farmAnimal->direction, MAIN_MAP_INDEX));
        setEntityCoordinates(farmAnimal->entityIndex, farmAnimal->coordinates.x, farmAnimal->coordinates.y, farmAnimal->coordinates.z);
    
        setEntityHandlesMultipleCollisions(farmAnimal->entityIndex, FALSE);
    
        farmAnimal->flags |= FARM_ANIMAL_ENTITY_LOADED;
        farmAnimal->flags &= ~(FARM_ANIMAL_APPROACHING | FARM_ANIMAL_LINGERING);
        
        if ((gWeather == SUNNY) || !(getLevelFlags(farmAnimal->location) & LEVEL_OUTDOORS)) {
            farmAnimal->actionState = 0;
        } else {
            farmAnimal->actionState = 19;
        }
    
    }
    
    updateFarmAnimal(index);

}

void initializeHorseEntity(void) {
    Horse *horse = &horseInfo;


    if ((horse->flags & HORSE_ACTIVE) && (horse->location == gBaseMapIndex) && !(horse->flags & 8)) {
        
        horse->entityIndex = 38;
        
        switch (horse->grown) {                      
            case FALSE:
                loadEntity(38, ENTITY_ASSET_HORSE_PONY, TRUE);
                break;
            case TRUE:
                loadEntity(38, ENTITY_ASSET_HORSE_GROWN, TRUE);
                break;
        }
        
        setEntityCollidable(horse->entityIndex, TRUE);
        setEntityYMovement(horse->entityIndex, TRUE);
        setCameraTrackingEntity(horse->entityIndex, FALSE);
        
        setEntityDirection(horse->entityIndex, convertWorldDirectionToScreenDirection(horse->direction, MAIN_MAP_INDEX));
        setEntityCoordinates(horse->entityIndex, horse->coordinates.x, horse->coordinates.y, horse->coordinates.z);
        
        horse->actionState = 0;
        horse->flags |= HORSE_ENTITY_LOADED;

    }
    
    updateHorse();

}

void initializeMiscAnimalEntity(u8 index, u8 arg1) {
    MiscAnimal *miscAnimal = &gMiscAnimals[index];


    if ((miscAnimal->flags & MISC_ANIMAL_ACTIVE) && (index < 7)) {
        
        // initialize entity objects if needed
        switch (miscAnimal->animalType) {

            case MISC_ANIMAL_PLAYER_DOG:
                initializeAnimalEntity(miscAnimal->entityIndex, (u16*)ENTITY_SLOTS_1_20_PALETTE, (AnimationFrameMetadata*)ENTITY_SLOTS_1_20_ANIM_METADATA, (u32*)ENTITY_SLOTS_1_20_SPRITESHEET_INDEX, (u32*)ENTITY_SLOTS_1_20_TEXTURE_TO_PALETTE_LOOKUP);
                setEntityPaletteIndex(miscAnimal->entityIndex, 2);
                break;

            case MISC_ANIMAL_COW:
                initializeAnimalEntity(miscAnimal->entityIndex, (u16*)ENTITY_SLOTS_21_28_PALETTE, (AnimationFrameMetadata*)ENTITY_SLOTS_21_28_ANIM_METADATA, (u32*)ENTITY_SLOTS_21_28_SPRITESHEET_INDEX, (u32*)ENTITY_SLOTS_21_28_TEXTURE_TO_PALETTE_LOOKUP);
                break;

            default:

                switch (index) {

                    case 0:
                        initializeAnimalEntity(miscAnimal->entityIndex, (u16*)ENTITY_SLOT_14_PALETTE, (AnimationFrameMetadata*)ENTITY_SLOT_14_ANIM_METADATA, (u32*)ENTITY_SLOT_14_SPRITESHEET_INDEX, (u32*)ENTITY_SLOT_14_TEXTURE_TO_PALETTE_LOOKUP);
                        break;
                    case 1:
                        initializeAnimalEntity(miscAnimal->entityIndex, (u16*)ENTITY_SLOT_15_PALETTE, (AnimationFrameMetadata*)ENTITY_SLOT_15_ANIM_METADATA, (u32*)ENTITY_SLOT_15_SPRITESHEET_INDEX, (u32*)ENTITY_SLOT_15_TEXTURE_TO_PALETTE_LOOKUP);
                        break;
                    case 2:
                        initializeAnimalEntity(miscAnimal->entityIndex, (u16*)ENTITY_SLOT_16_PALETTE, (AnimationFrameMetadata*)ENTITY_SLOT_16_ANIM_METADATA, (u32*)ENTITY_SLOT_16_SPRITESHEET_INDEX, (u32*)ENTITY_SLOT_16_TEXTURE_TO_PALETTE_LOOKUP);
                        break;
                    case 3:
                        initializeAnimalEntity(miscAnimal->entityIndex, (u16*)ENTITY_SLOT_17_PALETTE, (AnimationFrameMetadata*)ENTITY_SLOT_17_ANIM_METADATA, (u32*)ENTITY_SLOT_17_SPRITESHEET_INDEX, (u32*)ENTITY_SLOT_17_TEXTURE_TO_PALETTE_LOOKUP);
                        break;
                    case 4:
                        initializeAnimalEntity(miscAnimal->entityIndex, (u16*)ENTITY_SLOT_18_PALETTE, (AnimationFrameMetadata*)ENTITY_SLOT_18_ANIM_METADATA, (u32*)ENTITY_SLOT_18_SPRITESHEET_INDEX, (u32*)ENTITY_SLOT_18_TEXTURE_TO_PALETTE_LOOKUP);
                        break;

                }

                // set different palette for horse 2
                if (miscAnimal->animalType == MISC_ANIMAL_HORSE_2) {
                    setEntityPaletteIndex(miscAnimal->entityIndex, 0);
                }

                break;

        }
        
        switch (miscAnimal->animalType) {

            case MISC_ANIMAL_PLAYER_DOG:
                loadEntity(miscAnimal->entityIndex, ENTITY_ASSET_PLAYER_DOG, TRUE);
                break;
            case MISC_ANIMAL_NPC_DOG:
                loadEntity(miscAnimal->entityIndex, ENTITY_ASSET_DOG_VILLAGE, TRUE);
                break;
            case MISC_ANIMAL_CAT:
                loadEntity(miscAnimal->entityIndex, ENTITY_ASSET_CAT, TRUE);
                break;
            case MISC_ANIMAL_HORSE:
            case MISC_ANIMAL_HORSE_2:
                loadEntity(miscAnimal->entityIndex, ENTITY_ASSET_HORSE_UNBRIDLED, TRUE);
                break;
            case MISC_ANIMAL_SHEEP:
                loadEntity(miscAnimal->entityIndex, ENTITY_ASSET_SHEEP, TRUE);
                break;
            case MISC_ANIMAL_COW:
                loadEntity(miscAnimal->entityIndex, ENTITY_ASSET_COW, TRUE);
                break;
            case MISC_ANIMAL_FOX:
                loadEntity(miscAnimal->entityIndex, ENTITY_ASSET_FOX, TRUE);
                break;
            case MISC_ANIMAL_BUNNY:
                loadEntity(miscAnimal->entityIndex, ENTITY_ASSET_BUNNY, TRUE);
                break;
            case MISC_ANIMAL_SQUIRREL:
                loadEntity(miscAnimal->entityIndex, ENTITY_ASSET_SQUIRREL, TRUE);
                break;
            case MISC_ANIMAL_MONKEY:
                loadEntity(miscAnimal->entityIndex, ENTITY_ASSET_MONKEY, TRUE);
                break;
            case MISC_ANIMAL_SPARROW:
                loadEntity(miscAnimal->entityIndex, ENTITY_ASSET_BIRD, TRUE);
                setEntityPaletteIndex(miscAnimal->entityIndex, 0);
                break;
            case MISC_ANIMAL_BIRD:
                loadEntity(miscAnimal->entityIndex, ENTITY_ASSET_BIRD, TRUE);
                break;
            case MISC_ANIMAL_CRAB:
                loadEntity(miscAnimal->entityIndex, ENTITY_ASSET_CRAB, TRUE);
                break;
            case MISC_ANIMAL_SNAKE:
                loadEntity(miscAnimal->entityIndex, ENTITY_ASSET_SNAKE, TRUE);
                break;
            case MISC_ANIMAL_WHITE_BUTTERFLY:
                loadEntity(miscAnimal->entityIndex, ENTITY_ASSET_WHITE_BUTTERFLY, TRUE);
                break;
            case MISC_ANIMAL_LADYBUG:
                loadEntity(miscAnimal->entityIndex, ENTITY_ASSET_LADYBUG, TRUE);
                break;
            case MISC_ANIMAL_CICADA:
                loadEntity(miscAnimal->entityIndex, ENTITY_ASSET_CICADA, TRUE);
                break;
            case MISC_ANIMAL_HORNED_BEETLE:
                loadEntity(miscAnimal->entityIndex, ENTITY_ASSET_HORNED_BEETLE, TRUE);
                break;
            case MISC_ANIMAL_STAG_BEETLE:
                loadEntity(miscAnimal->entityIndex, ENTITY_ASSET_STAG_BEETLE, TRUE);
                break;
            case MISC_ANIMAL_DRAGONFLY:
                loadEntity(miscAnimal->entityIndex, ENTITY_ASSET_DRAGONFLY, TRUE);
                break;
            case MISC_ANIMAL_CRICKET:
                loadEntity(miscAnimal->entityIndex, ENTITY_ASSET_BELL_RING_CRICKET, TRUE);
                break;
        }

        setEntityCollidable(miscAnimal->entityIndex, TRUE);
        setEntityYMovement(miscAnimal->entityIndex, TRUE);
        setCameraTrackingEntity(miscAnimal->entityIndex, FALSE);
        
        setEntityDirection(miscAnimal->entityIndex, convertWorldDirectionToScreenDirection(miscAnimal->direction, MAIN_MAP_INDEX));
        setEntityCoordinates(miscAnimal->entityIndex, miscAnimal->coordinates.x, miscAnimal->coordinates.y, miscAnimal->coordinates.z);
        setEntityHandlesMultipleCollisions(miscAnimal->entityIndex, FALSE);
        
        miscAnimal->actionState = arg1;
        miscAnimal->flags |= MISC_ANIMAL_ENTITY_LOADED;
    
    }
    
    updateMiscAnimal(index);
    
}

void updateDog(void) {
    Dog *dog = &dogInfo;


    Vec3f vec;

    if ((dog->flags & DOG_ACTIVE) && (dog->flags & DOG_ENTITY_LOADED)) {
        
        if (checkEntityAnimationStateChanged(dog->entityIndex) || dog->flags & DOG_COLLISION_WITH_PLAYER) {

            if (dog->stateTimer == 0) {
                updateDogActionState();
            } else {
                dog->stateTimer--;
            }
            
            dog->flags &= ~DOG_COLLISION_WITH_PLAYER;
            
        }
        
        setEntityDirection(dog->entityIndex, convertWorldDirectionToScreenDirection(dog->direction, MAIN_MAP_INDEX));
        vec = getMovementVectorFromDirection(dog->speed, dog->direction, 0.0f);
        setEntityMovementVector(dog->entityIndex, vec.x, vec.y, vec.z, dog->speed);

    } 
    
}

void updateDogActionState(void) {
    Dog *dog = &dogInfo;


    u16 temp;
    
    switch (dog->actionState) {

        case 0:
            
            dog->speed = 0;
            dog->stateTimer = 10;
            dog->unk_1B = 0;
            
            setEntityDirectionalAnimation(dog->entityIndex, 0);
            
            if (dog->flags & DOG_FOLLOWING) {
                
                dog->direction = calculateAnimalDirectionToPlayer(entities[dog->entityIndex].coordinates.x, entities[dog->entityIndex].coordinates.z, entities[ENTITY_PLAYER].coordinates.x, entities[ENTITY_PLAYER].coordinates.z);
                dog->actionState = 4;
                
            } else {

                if (getRandomNumberInRange(0, 60) < 40) {
                    dog->actionState = 0;
                } else {
    
                    temp = getRandomNumberInRange(0, 20);
                    
                    if (temp < 7) {
                        dog->direction = temp;
                    }
                    
                    temp = getRandomNumberInRange(0, 10);
    
                    if (temp == 0) {
                        dog->actionState = 1;
                    }
                    if (temp == 1) {
                        dog->actionState = 2;
                    }
                    if (temp == 2) {
                        dog->actionState = 3;
                    }
                    if (temp == 3) {
                        dog->actionState = 4;
                    }
                    if (temp == 4) {
                        dog->actionState = 5;
                    }
                    if (temp == 5) {
                        dog->actionState = 6;
                    }
                    if (temp == 6) {
                        dog->actionState = 7;
                    }
                    if (temp == 7) {
                        dog->actionState = 8;
                    }
                    if (temp == 8) {
                        dog->actionState = 10;
                    }
                    if (temp == 9) {
                        dog->actionState = 24;
                    }
                    if (temp == 10) {
                        dog->actionState = 25;
                    }
                    
                }
                    
            } 

            dog->flags |= DOG_STATE_CHANGED;
            
            break;

        case 1:
    
            dog->speed = 1;
            dog->stateTimer = 10;
            dog->unk_1B = 0;
            
            setEntityDirectionalAnimation(dog->entityIndex, 8);
            
            temp = getRandomNumberInRange(0, 19);
            
            if (temp < 10) {
                dog->actionState = 1;
            } else if (temp >= 15) {
                dog->actionState = 4;
            } else {
                dog->actionState = 0;
            }

            dog->flags |= DOG_STATE_CHANGED;
            
            break;

        case 2:

            dog->speed = 0;
            dog->stateTimer = 0;
            dog->unk_1B = 0;
            
            setEntityDirectionalAnimation(dog->entityIndex, 48);
            dog->actionState = 16;
            
            dog->flags |= DOG_STATE_CHANGED;
            
            break;

        case 3:

            dog->speed = 0;
            dog->stateTimer = 10;
            dog->unk_1B = 0;

            setEntityDirectionalAnimation(dog->entityIndex, 32);
    
            temp = getRandomNumberInRange(0, 20);
            
            if (temp < 10) {
                
                dog->actionState = 3;
                
            } else {

                if (9 < temp && temp < 12) {
                    dog->actionState = 0;
                }

                if (11 < temp && temp < 14) {
                    dog->actionState = 11;
                }

                if (13 < temp && temp < 16) {
                    dog->actionState = 12;
                }
                
                if (15 < temp && temp < 18) {
                    dog->actionState = 13;
                }
                
                if (17 < temp && temp < 20) {
                    dog->actionState = 14;
                }
               
            }
            
            dog->flags |= DOG_STATE_CHANGED;

            break;
        
        case 4:
            
            dog->speed = 2;
            dog->stateTimer = 0;
            dog->unk_1B = 0;
            
            setEntityDirectionalAnimation(dog->entityIndex, 0x10);
            
            if (dog->flags & DOG_FOLLOWING) {
                
                dog->direction = calculateAnimalDirectionToPlayer(entities[dog->entityIndex].coordinates.x, entities[dog->entityIndex].coordinates.z, entities[ENTITY_PLAYER].coordinates.x, entities[ENTITY_PLAYER].coordinates.z);
                
                if (!(getRandomNumberInRange(0, 20))) {
                    dog->flags &= ~DOG_FOLLOWING;
                }
                
            } else {

                temp = getRandomNumberInRange(0, 19);

                if (temp < 10) {
                    
                    dog->actionState = 4;
                    
                } else {
    
                    if (9 < temp && temp < 13) {
                        dog->actionState = 0;
                    }
    
                    if (12 < temp && temp < 16) {
                        dog->actionState = 1;
                    }
    
                    if (15 < temp && temp < 19) {
                        dog->actionState = 21;
                    }
                   
                }
            
            }
            
            dog->flags |= DOG_STATE_CHANGED;

            break;

        case 5:
            
            dog->speed = 0;
            dog->stateTimer = 0;
            dog->unk_1B = 0;
            
            setEntityDirectionalAnimation(dog->entityIndex, 0x90);
            
            if (!getRandomNumberInRange(0, 2)) {
                dog->actionState = 5;
            } else {
                dog->actionState = 3;
            }
    
            dog->flags |= DOG_STATE_CHANGED;

            break;

case 6:

            dog->speed = 0;
            dog->stateTimer = 0;
            dog->unk_1B = 0;
            
            setEntityDirectionalAnimation(dog->entityIndex, 0xA0);
            
            if (!getRandomNumberInRange(0, 2)) {
                dog->actionState = 6;
            } else {
                dog->actionState = 0;
            }
    
            dog->flags |= DOG_STATE_CHANGED;
            
            break;

        case 7:

            dog->speed = 0;
            dog->stateTimer = 0;
            dog->unk_1B = 0;
            
            setEntityDirectionalAnimation(dog->entityIndex, 0xA8);
            
            if (!getRandomNumberInRange(0, 2)) {
                dog->actionState = 7;
            } else {
                dog->actionState = 0;
            }
    
            dog->flags |= DOG_STATE_CHANGED;
            
            break;
        
        case 8:

            dog->speed = 0;
            dog->stateTimer = 0;
            dog->unk_1B = 0;
            
            setEntityDirectionalAnimation(dog->entityIndex, 0xB0);
            
            if (getRandomNumberInRange(0, 2)) {
                dog->actionState = 0;
            } else {
                dog->actionState = 8;
            }
    
            dog->flags |= DOG_STATE_CHANGED;
            
            break;
        
        case 9:
            
            dog->speed = 0;
            dog->stateTimer = 0;
            dog->unk_1B = 0;
            
            setEntityDirectionalAnimation(dog->entityIndex, 0xB8);
            
            dog->actionState = 26;
            
            break;
        
        case 10:
            
            dog->speed = 0;
            dog->stateTimer = 10;
            dog->unk_1B = 0;
            
            setEntityDirectionalAnimation(dog->entityIndex, 0xC8);

            if (!getRandomNumberInRange(0, 2)) {
                dog->actionState = 10;
            } else {
                dog->actionState = 0;
            }

            dog->flags |= DOG_STATE_CHANGED;
            
            break;
                
        case 11:
            
            dog->speed = 0;
            dog->stateTimer = 10;
            dog->unk_1B = 0;
            
            setEntityDirectionalAnimation(dog->entityIndex, 0x28);

            if (!getRandomNumberInRange(0, 2)) {
                dog->actionState = 11;
            } else {
                dog->actionState = 3;
            }

            dog->flags |= DOG_STATE_CHANGED;
            
            break;
                        
        case 12:
            
            dog->speed = 0;
            dog->stateTimer = 10;
            dog->unk_1B = 0;
            
            setEntityDirectionalAnimation(dog->entityIndex, 0x50);

            if (!getRandomNumberInRange(0, 2)) {
                dog->actionState = 12;
            } else {
                dog->actionState = 3;
            }

            dog->flags |= DOG_STATE_CHANGED;
            
            break;
                        
        case 13:
            
            dog->speed = 0;
            dog->stateTimer = 0;
            dog->unk_1B = 0;
            
            setEntityDirectionalAnimation(dog->entityIndex, 0x58);

            if (!getRandomNumberInRange(0, 2)) {
                dog->actionState = 13;
            } else {
                dog->actionState = 3;
            }

            dog->flags |= DOG_STATE_CHANGED;
            
            break;
                        
        case 14:
            
            dog->speed = 0;
            dog->stateTimer = 0;
            dog->unk_1B = 0;
            
            setEntityDirectionalAnimation(dog->entityIndex, 0x60);

            if (!getRandomNumberInRange(0, 2)) {
                dog->actionState = 14;
            } else {
                dog->actionState = 3;
            }

            dog->flags |= DOG_STATE_CHANGED;
            
            break;

        case 16:
            
            dog->speed = 0;
            dog->stateTimer = 10;
            dog->unk_1B = 0;
            
            setEntityDirectionalAnimation(dog->entityIndex, 0x28);

            temp = getRandomNumberInRange(0, 14);

            if (temp < 10) {
                dog->actionState = 16;
            }
            if (temp == 10) {
                dog->actionState = 27;
            }
            if (temp == 11) {
                dog->actionState = 17;
            }
            if (temp == 12) {
                dog->actionState = 18;
            }
            if (temp == 13) {
                dog->actionState = 19;
            }
            if (temp == 14) {
                dog->actionState = 20;
            }

            dog->flags |= DOG_STATE_CHANGED;
            
            break;
                        
        case 17:
            
            dog->speed = 0;
            dog->stateTimer = 0;
            dog->unk_1B = 0;
            
            setEntityDirectionalAnimation(dog->entityIndex, 0x68);

            if (!getRandomNumberInRange(0, 2)) {
                dog->actionState = 17;
            } else {
                dog->actionState = 16;
            }

            dog->flags |= DOG_STATE_CHANGED;
            
            break;
                        
        case 18:
            
            dog->speed = 0;
            dog->stateTimer = 0;
            dog->unk_1B = 0;
            
            setEntityDirectionalAnimation(dog->entityIndex, 0x70);

            if (!getRandomNumberInRange(0, 2)) {
                dog->actionState = 18;
            } else {
                dog->actionState = 16;
            }

            dog->flags |= DOG_STATE_CHANGED;
            
            break;

        case 19:
            
            dog->speed = 0;
            dog->stateTimer = 0;
            dog->unk_1B = 0;
            
            setEntityDirectionalAnimation(dog->entityIndex, 0x78);

            if (!getRandomNumberInRange(0, 2)) {
                dog->actionState = 19;
            } else {
                dog->actionState = 16;
            }

            dog->flags |= DOG_STATE_CHANGED;
            
            break;

case 20:
            
            dog->speed = 0;
            dog->stateTimer = 10;
            dog->unk_1B = 0;
            
            setEntityDirectionalAnimation(dog->entityIndex, 0x80);

            if (!getRandomNumberInRange(0, 2)) {
                dog->actionState = 20;
            } else {
                dog->actionState = 16;
            }

            dog->flags |= DOG_STATE_CHANGED;
            
            break;

        case 21:
            
            dog->speed = 1;
            dog->stateTimer = 0;
            dog->unk_1B = 0;
            
            setEntityDirectionalAnimation(dog->entityIndex, 0x38);
            
            dog->actionState = 22;
            dog->flags |= DOG_STATE_CHANGED;
            
            break;        
        
        case 22:
            
            dog->speed = 2;
            dog->stateTimer = 0;
            dog->unk_1B = 0;
            
            setEntityDirectionalAnimation(dog->entityIndex, 0x40);
            
            dog->actionState = 23;
            dog->flags |= DOG_STATE_CHANGED;
            
            break;     
        
        case 23:
            
            dog->speed = 1;
            dog->stateTimer = 0;
            dog->unk_1B = 0;
            
            setEntityDirectionalAnimation(dog->entityIndex, 0x48);
            
            dog->actionState = 0;
            dog->flags |= DOG_STATE_CHANGED;
            
            break;     
                                
        case 24:
            
            dog->speed = 0;
            dog->stateTimer = 0;
            dog->unk_1B = 0;
            
            setEntityDirectionalAnimation(dog->entityIndex, 0x88);

            if (!getRandomNumberInRange(0, 2)) {
                dog->actionState = 24;
            } else {
                dog->actionState = 0;
            }

            dog->flags |= DOG_STATE_CHANGED;
            
            break;
                                
        case 25:
            
            dog->speed = 0;
            dog->stateTimer = 0;
            dog->unk_1B = 0;
            
            setEntityDirectionalAnimation(dog->entityIndex, 0x98);

            if (!getRandomNumberInRange(0, 2)) {
                dog->actionState = 25;
            } else {
                dog->actionState = 0;
            }

            dog->flags |= DOG_STATE_CHANGED;
            
            break;

        case 26:
            
            dog->speed = 0;
            dog->stateTimer = 0;
            dog->unk_1B = 0;
            
            setEntityDirectionalAnimation(dog->entityIndex, 0xC0);
            
            dog->actionState = 0;
            dog->flags |= DOG_STATE_CHANGED;
            
            break;     
        
        case 27:
            
            dog->speed = 0;
            dog->stateTimer = 0;
            dog->unk_1B = 0;
            
            setEntityDirectionalAnimation(dog->entityIndex, 48);
            
            dog->actionState = 0;
            dog->flags |= DOG_STATE_CHANGED;
            
            break;     
        
        case 32:
            
            dog->speed = 0;
            dog->stateTimer = 0;
            dog->unk_1B = 0;
            
            setEntityDirectionalAnimation(dog->entityIndex, 0);
            
            dog->flags |= DOG_STATE_CHANGED;
            
            break;     

        case 34:
            dog->speed = 0;
            dog->stateTimer = 0;
            dog->unk_1B = 0;
            
            setEntityDirectionalAnimation(dog->entityIndex, 0x98);
            
            dog->actionState = 0;
            dog->flags |= DOG_STATE_CHANGED;
            
            playSfx(0x3C);
            
            break;
        
    }
    
}

void updateChicken(u8 index) {
    Chicken *chicken = &gChickens[index];

    
    Vec3f vec;

    if (chicken->flags & CHICKEN_ACTIVE && chicken->flags & CHICKEN_ENTITY_LOADED) {
    
        if ((checkEntityAnimationStateChanged(chicken->entityIndex) || chicken->flags & CHICKEN_COLLISION_WITH_PLAYER)) {
    
            switch(chicken->type) {
    
                case ADULT_CHICKEN:
    
                    switch(chicken->condition) {
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

            chicken->flags &= ~CHICKEN_COLLISION_WITH_PLAYER;
    
        }

        setEntityDirection(chicken->entityIndex, convertWorldDirectionToScreenDirection(chicken->direction, MAIN_MAP_INDEX));
        
        vec = getMovementVectorFromDirection(chicken->speed, chicken->direction, 0);
        setEntityMovementVector(chicken->entityIndex, vec.x, vec.y, vec.z, chicken->speed);
    
    }

}

void updateAdultChickenNormal(u8 index) {
    Chicken *chicken = &gChickens[index];


    u16 temp;

    switch (chicken->actionState) {
        
        case 0:
            
            chicken->speed = 0;
            chicken->stateTimer = 0;
            chicken->unk_1B = 0;

            setEntityDirectionalAnimation(chicken->entityIndex, 0);

            if (getRandomNumberInRange(0, 100) < 40) {
                chicken->actionState = 0;
            } else {

                temp = getRandomNumberInRange(0, 10);
                
                if (temp < 7) {
                    chicken->direction = temp;
                }
                
                temp = getRandomNumberInRange(0, 60);
                
                if (temp < 20) {
                    chicken->actionState = 1;
                }
                if (19 < temp && temp < 40) {
                    chicken->actionState = 2;
                }
                if (39 < temp && temp < 60) {
                    chicken->actionState = 3;
                }
            
            }

            chicken->flags |= CHICKEN_STATE_CHANGED;
            
            break;
            
        case 1:
            chicken->speed  = 1;
            chicken->stateTimer = 0;
            chicken->unk_1B = 0;
            
            setEntityDirectionalAnimation(chicken->entityIndex, 8);
            
            if (getRandomNumberInRange(0, 20) < 10) {
                chicken->actionState = 1;
            } else {
                chicken->actionState = 0;
            }
            
            chicken->flags |= CHICKEN_STATE_CHANGED;
            playSfx(0x40);
            
            break;

        case 2:
            chicken->speed = 0;
            chicken->stateTimer = 0;
            chicken->unk_1B = 0;
            
            setEntityDirectionalAnimation(chicken->entityIndex, 0x10);

            chicken->actionState = 0;
            chicken->flags |= CHICKEN_STATE_CHANGED;
            
            break;

        case 3:

            chicken->speed = 0;
            chicken->stateTimer = 0;
            chicken->unk_1B = 0;

            setEntityDirectionalAnimation(chicken->entityIndex, 0x20);
        
            if (getRandomNumberInRange(0,19) < 10) {
                chicken->actionState = 0;      
            } else {
                chicken->actionState = 4;
            }

            chicken->flags |= CHICKEN_STATE_CHANGED;
            
            break;

        case 4:
            chicken->speed = 0;
            chicken->stateTimer = 0;
            chicken->unk_1B = 0;

            setEntityDirectionalAnimation(chicken->entityIndex, 0x18);
            
            if (getRandomNumberInRange(0, 19) < 10) {
                chicken->actionState = 4;
            } else {
                chicken->actionState = 3;
            }
        
            chicken->flags |= CHICKEN_STATE_CHANGED;
            
            break;
        
        case 16:
            chicken->speed = 0;
            chicken->stateTimer = 0;
            chicken->unk_1B = 0;

            setEntityDirectionalAnimation(chicken->entityIndex, 0);
            
            chicken->flags |= CHICKEN_STATE_CHANGED;

            break;

        case 17:
            chicken->speed = 0;
            chicken->stateTimer = 0;
            chicken->unk_1B = 0;

            setEntityDirectionalAnimation(chicken->entityIndex, 48);
            
            chicken->actionState = 0;
            chicken->flags |= CHICKEN_STATE_CHANGED;
            
            playSfx(0x41);
            
            break;
        
    }

}

void updateAdultChickenStarved(u8 chickenIndex) {
    Chicken *chicken = &gChickens[chickenIndex];


    u16 temp;
    
    chicken->speed = 0;
    chicken->stateTimer = 0;
    chicken->unk_1B = 0;

    setEntityDirectionalAnimation(chicken->entityIndex, 0x28);

    temp = getRandomNumberInRange(0, 1000);
    
    if (temp < 7) {
        chicken->direction = temp;
    }
    
    chicken->actionState = 0;
    chicken->flags |= CHICKEN_STATE_CHANGED;
    
}

void updateChick(u8 index) {
    Chicken *chicken = &gChickens[index];


    u16 temp ;

    switch (chicken->actionState) {

        case 0:
            chicken->speed = 0;
            chicken->stateTimer = 0;
            chicken->unk_1B = 0;

            setEntityDirectionalAnimation(chicken->entityIndex, 0);

            if (getRandomNumberInRange(0, 100) < 40) {
                chicken->actionState = 0;
            } else {
                
                temp = getRandomNumberInRange(0, 10);
                
                if (temp  < 7) {
                    chicken->direction = temp;
                }
                
                temp = getRandomNumberInRange(0, 40);

                if (temp  < 20) {
                    chicken->actionState = 1;
                }
                if (19 < temp  && temp  < 40) {
                    chicken->actionState = 2;
                }
            }
            
            chicken->flags |= CHICKEN_STATE_CHANGED;

            break;

        case 1:
            chicken->speed = 1;
            chicken->stateTimer = 0;
            chicken->unk_1B = 0;
            
            setEntityDirectionalAnimation(chicken->entityIndex, 8);
            
            if (getRandomNumberInRange(0, 20) < 10) {
                chicken->actionState = 1;
            } else {
                chicken->actionState = 0;
            }
            
            chicken->flags |= CHICKEN_STATE_CHANGED;
            
            playSfx(0x42);
            
            break;

        case 2:
            chicken->speed = 0;
            chicken->stateTimer = 0;
            chicken->unk_1B = 0;
        
            setEntityDirectionalAnimation(chicken->entityIndex, 16);
        
            chicken->actionState = 0;
            chicken->flags |= CHICKEN_STATE_CHANGED;
        
            break;

        case 16:
            chicken->speed = 0;
            chicken->stateTimer = 0;
            chicken->unk_1B = 0;
            
            setEntityDirectionalAnimation(chicken->entityIndex, 0);

            chicken->flags |= CHICKEN_STATE_CHANGED;
        
            break;
        
    }

}

void updateChickenEgg(u8 chickenIndex) {
    Chicken *chicken = &gChickens[chickenIndex];


    switch (chicken->actionState) {

        case 0:
        case 16:
            chicken->speed = 0;
            chicken->stateTimer = 0;
            chicken->unk_1B = 0;
            setEntityAnimation(chicken->entityIndex, 0x13);
            chicken->flags |= CHICKEN_STATE_CHANGED;
            
    }
    
}

void updateFarmAnimal(u8 index) {
    FarmAnimal *farmAnimal = &gFarmAnimals[index];


    Vec3f vec;
    u8 levelInteractionIndex;

    if ((farmAnimal->flags & FARM_ANIMAL_ACTIVE) && (farmAnimal->flags & FARM_ANIMAL_ENTITY_LOADED)) {

        if ((checkEntityAnimationStateChanged(farmAnimal->entityIndex)) || (farmAnimal->flags & FARM_ANIMAL_COLLISION_WITH_PLAYER)) {
            
            if (entities[farmAnimal->entityIndex].unk_5E == 0) {

                if (!(farmAnimal->flags & FARM_ANIMAL_FOLLOWING)) {

                    farmAnimal->direction = gPlayer.direction;
                    farmAnimal->actionState = 1;

                    farmAnimal->flags &= ~(FARM_ANIMAL_APPROACHING);
                    farmAnimal->flags |= (FARM_ANIMAL_LINGERING | FARM_ANIMAL_FOLLOWING);

                }

            }
            
            switch (farmAnimal->type) {

                case BABY_COW:
                    updateBabyCow(index);
                    break;

                case CALF:
                    updateCalf(index);
                    break;

                case ADULT_COW:

                    switch(farmAnimal->condition) {

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

                    switch(farmAnimal->condition) {

                        case SHEEP_NORMAL:
                            updateAdultSheepNormal(index);
                            break;

                        case SHEEP_SICK:
                            updateAdultSheepSick(index);
                            break;

                    }
    
                    break;

                case SHEARED_SHEEP:

                    switch(farmAnimal->condition) {

                        case SHEEP_NORMAL:
                            updateShearedSheepNormal(index);
                            break;

                        case SHEEP_SICK:
                            updateShearedSheepSick(index);
                            break;

                    }
    
                    break;

                }

            farmAnimal->flags &= ~(FARM_ANIMAL_FOLLOWING | FARM_ANIMAL_COLLISION_WITH_PLAYER);

        }
       
        setEntityDirection(farmAnimal->entityIndex, convertWorldDirectionToScreenDirection(farmAnimal->direction, MAIN_MAP_INDEX));
        
        vec = getMovementVectorFromDirection(farmAnimal->speed, farmAnimal->direction, 0.0f);
        setEntityMovementVector(farmAnimal->entityIndex, vec.x, vec.y, vec.z, farmAnimal->speed);
       
        levelInteractionIndex = getLevelInteractionIndexFromEntityPosition(farmAnimal->entityIndex, 0, 32);
        
        switch(farmAnimal->location) {

            case FARM_SPRING:

                if (levelInteractionIndex == 6) {

                    farmAnimal->location = BARN;
                    deactivateEntity(farmAnimal->entityIndex);

                    farmAnimal->flags &= ~FARM_ANIMAL_ENTITY_LOADED;
                    activateMapAddition(MAIN_MAP_INDEX, 0x1B, 0);  

                }
                
                break;

            case BARN:

                if (levelInteractionIndex == 1) {

                    farmAnimal->location = FARM_SPRING;
                    deactivateEntity(farmAnimal->entityIndex);
                    farmAnimal->flags &= ~FARM_ANIMAL_ENTITY_LOADED;

                    farmAnimal->coordinates.x = -128.0f;
                    farmAnimal->coordinates.y = 0.0f;
                    farmAnimal->coordinates.z = -288.0f;

                }
                
                break;

        }

    }

}

void updateAdultCowNormal(u8 index) {
    FarmAnimal *farmAnimal = &gFarmAnimals[index];

    
    u16 temp;

    switch (farmAnimal->actionState) {
        
        case 0:
        
            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;
            
            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0);
            
            if ((farmAnimal->flags & FARM_ANIMAL_APPROACHING) || (farmAnimal->flags & FARM_ANIMAL_LINGERING)) {
                
                farmAnimal->direction = calculateAnimalDirectionToPlayer(entities[farmAnimal->entityIndex].coordinates.x, entities[farmAnimal->entityIndex].coordinates.z, entities[ENTITY_PLAYER].coordinates.x, entities[ENTITY_PLAYER].coordinates.z);
                farmAnimal->actionState = 1;
                
            } else {
                
                if (getRandomNumberInRange(0, 60) < 40) {
                    farmAnimal->actionState = 0;
                } else {
                    
                    temp = getRandomNumberInRange(0, 20);
                    
                    if (temp < 7) {
                        farmAnimal->direction = temp;
                    }
        
                    temp = getRandomNumberInRange(0, 5);
                    
                    if (temp == 0) {
                        farmAnimal->actionState = 1;
                    }
                    
                    if (temp == 1) {
                        farmAnimal->actionState = 2;
                    }
                    
                    if (temp == 2) {
                        farmAnimal->actionState = 3;
                    }
                    
                    if (temp == 3) {
                        farmAnimal->actionState = 4;
                    }
                    
                    if (temp == 4) {
                        farmAnimal->actionState = 5;
                    }
                    
                    if (temp == 5) {
                        farmAnimal->actionState = 6;
                    }
                    
                }
                    
            }
                
            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
                
            break;
            
        case 1:
            
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;
            farmAnimal->speed = 1.0f;
            
            setEntityDirectionalAnimation(farmAnimal->entityIndex, 8);
            
            if (farmAnimal->flags & FARM_ANIMAL_APPROACHING) {
            
                farmAnimal->flags &= ~FARM_ANIMAL_LINGERING;
                farmAnimal->direction = calculateAnimalDirectionToPlayer(entities[farmAnimal->entityIndex].coordinates.x, entities[farmAnimal->entityIndex].coordinates.z, entities[ENTITY_PLAYER].coordinates.x, entities[ENTITY_PLAYER].coordinates.z);
                
                if (!(getRandomNumberInRange(0, 20))) {
                    farmAnimal->flags &= ~(FARM_ANIMAL_APPROACHING);
                }
                
            } else if (farmAnimal->flags & FARM_ANIMAL_LINGERING) {
                
                if (!(getRandomNumberInRange(0, 40))) {
                    farmAnimal->flags &= ~(FARM_ANIMAL_LINGERING);
                }
                
            } else {
                
                if (getRandomNumberInRange(0, 20) < 10) {
                    farmAnimal->actionState = 1;
                } else {
                    farmAnimal->actionState = 0;
                }
            
            }
            
            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
            
            break;
            
        case 2:
            
            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;
            
            setEntityDirectionalAnimation(farmAnimal->entityIndex, 32);
            
            temp = getRandomNumberInRange(0, 2);
            
            if (temp == 0) {
                farmAnimal->actionState = 2;
            }
            
            if (temp == 1) {
                farmAnimal->actionState = 0;
            }
            
            if (temp == 2) {
                farmAnimal->actionState = 9;
            }
            
            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
            
            break;
            
        case 3:
            
            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;
            
            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x10);
            
            farmAnimal->actionState = 7;
            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
            
            break;
            
        case 4:
            
            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;
            
            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x38);
            
            farmAnimal->actionState = 0;
            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
            
            break;
            
        case 5:
            
            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;
            
            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x40);
            
            farmAnimal->actionState = 0;
            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
            
            break;
            
        case 6:
            
            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;
            
            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x48);
            
            farmAnimal->actionState = 0;
            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
            
            break;
            
        case 7:
            
            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;
            
            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x18);
            
            temp = getRandomNumberInRange(0, 11);
            
            if (temp < 10) {
                farmAnimal->actionState = 7;
            }
            if (temp == 10) {
                farmAnimal->actionState = 8;
            }
            if (temp == 11) {
                farmAnimal->actionState = 10;
            }
            
            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
            
            break;
            
        case 8:
            
            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;
            
            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x28);
            
            if (getRandomNumberInRange(0, 4) < 4) {
                farmAnimal->actionState = 8;
            } else {
                farmAnimal->actionState = 7;
            }
            
            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
            
            break;
            
        case 9:
            
            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;
            
            setEntityDirectionalAnimation(farmAnimal->entityIndex, 48);
            
            farmAnimal->actionState = 2;
            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
            
            break;
            
        case 10:
            
            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;
            
            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x10);
            
            farmAnimal->actionState = 0;
            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
            
            break;
            
        case 16:
            
            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;
            
            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0);
            
            farmAnimal->flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            farmAnimal->flags &= ~FARM_ANIMAL_LINGERING;
            
            break;
        
        case 17:
            
            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;
            
            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x50);
            
            farmAnimal->actionState = 0;
            farmAnimal->flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            farmAnimal->flags &= ~FARM_ANIMAL_LINGERING;
            
            playSfx(0x36);
            
            break;
            
        case 18:
            
            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;
            
            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x58);
            
            farmAnimal->actionState = 0;
            
            farmAnimal->flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            farmAnimal->flags &= ~FARM_ANIMAL_LINGERING;
            
            playSfx(0x37);
            
            break;
            
        case 19:
            
            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;
            
            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x60);
            
            farmAnimal->actionState = 19;
            farmAnimal->flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            farmAnimal->flags &= ~FARM_ANIMAL_LINGERING;
            
            break;
        
    }
    
}

void updateAdultCowHappy(u8 index) {
    FarmAnimal *farmAnimal = &gFarmAnimals[index];


    u16 temp;
    u16 tempDirection;
    
    switch (farmAnimal->actionState) {

        case 0:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0xB0);

            if ((farmAnimal->flags & FARM_ANIMAL_APPROACHING) || (farmAnimal->flags & FARM_ANIMAL_LINGERING)) {

                farmAnimal->direction = calculateAnimalDirectionToPlayer(
                    entities[farmAnimal->entityIndex].coordinates.x,
                    entities[farmAnimal->entityIndex].coordinates.z,
                    entities[ENTITY_PLAYER].coordinates.x, 
                    entities[ENTITY_PLAYER].coordinates.z);

                farmAnimal->actionState = 1;
                
            } else {
        
                if (getRandomNumberInRange(0, 60) < 40) {
                
                    farmAnimal->actionState = 0;

                } else {

                    tempDirection = getRandomNumberInRange(0, 20);

                    if (tempDirection < 7) {
                        farmAnimal->direction = tempDirection;
                    }

                    temp = getRandomNumberInRange(0, 5);

                    if (temp == 0) {
                        farmAnimal->actionState = 1;
                    }
                    
                    if (temp == 1) {
                        farmAnimal->actionState = 2;
                    }

                    if (temp == 2) {
                        farmAnimal->actionState = 3;
                    }

                    if (temp == 3) {
                        farmAnimal->actionState = 4;
                    }

                    if (temp == 4) {
                        farmAnimal->actionState = 5;
                    }

                    if (temp == 5) {
                        farmAnimal->actionState = 6;
                    }

                }
                
            }
        
            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
            break;
        
        case 1:

            farmAnimal->speed = 1.0f;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0xB8);

            if (farmAnimal->flags & FARM_ANIMAL_APPROACHING) {

                farmAnimal->flags &= ~FARM_ANIMAL_LINGERING;

                farmAnimal->direction = calculateAnimalDirectionToPlayer(
                    entities[farmAnimal->entityIndex].coordinates.x,
                    entities[farmAnimal->entityIndex].coordinates.z,
                    entities[ENTITY_PLAYER].coordinates.x, 
                    entities[ENTITY_PLAYER].coordinates.z);

                if (getRandomNumberInRange(0, 20) == 0) {
                    farmAnimal->flags &= ~FARM_ANIMAL_APPROACHING;
                } 
                
            } else if (farmAnimal->flags & FARM_ANIMAL_LINGERING) {

                 if (getRandomNumberInRange(0, 40) == 0) {
                    farmAnimal->flags &= ~FARM_ANIMAL_LINGERING;
                } 
                
            } else {

                if (getRandomNumberInRange(0, 20) < 10) {
                    farmAnimal->actionState = 1;
                } else {
                    farmAnimal->actionState = 0;
                }
                
            }

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;            
            break;

        case 2:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0xD0);

            temp = getRandomNumberInRange(0, 2); 
            
            if (temp == 0) {
                farmAnimal->actionState = 2;
            } 
            if (temp == 1) {
                farmAnimal->actionState = 0;
            }
            if (temp == 2) {
                farmAnimal->actionState = 9;
            }

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;  
            break;

        case 3:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0xC0);
            
            farmAnimal->actionState = 7;
            
            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;  
            break;

        case 4:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0xE8);

            farmAnimal->actionState = 0;

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;  
            break;

        case 5:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0xF0);

            farmAnimal->actionState = 0;

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
            break;

        case 6:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0xF8);

            farmAnimal->actionState = 0;

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
            break;

        case 7:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0xC8);

            temp = getRandomNumberInRange(0, 11);

            if (temp < 10) {
                farmAnimal->actionState = 7;
            }

            if (temp == 10) {
                farmAnimal->actionState = 8;
            }

            if (temp == 11) {
                farmAnimal->actionState = 10;
            }

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
            break;

        case 8:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0xD8);

            if (getRandomNumberInRange(0, 4) >= 4) {
                farmAnimal->actionState = 7;
            } else {
                farmAnimal->actionState = 8;
            }

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
            
            break;

        case 9:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0xE0);

            farmAnimal->actionState = 2;

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
            break;

        case 10:
                    
            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0xC0);

            farmAnimal->actionState = 0;

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
            break;
        
        case 16:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0xB0);

            farmAnimal->flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            farmAnimal->flags &= ~(FARM_ANIMAL_LINGERING);
            
            break;
        
        case 17:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x50);

            farmAnimal->actionState = 0;

            farmAnimal->flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            farmAnimal->flags &= ~(FARM_ANIMAL_LINGERING);

            playSfx(0x36);
            
            break;
        
        case 18:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x100);

            farmAnimal->actionState = 0;

            farmAnimal->flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            farmAnimal->flags &= ~(FARM_ANIMAL_LINGERING);

            playSfx(0x37);

            break;
        
        case 19:
            
            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x60);

            farmAnimal->actionState = 19;

            farmAnimal->flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            farmAnimal->flags &= ~(FARM_ANIMAL_LINGERING);
            
            break;
        
    }
    
}

void updateAdultCowMad(u8 index) {
    FarmAnimal *farmAnimal = &gFarmAnimals[index];


    u16 temp;
    u16 tempDirection;
    
    switch (farmAnimal->actionState) {

        case 0:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x68);

            temp = getRandomNumberInRange(0, 60);

            if ((farmAnimal->flags & FARM_ANIMAL_APPROACHING) || (farmAnimal->flags & FARM_ANIMAL_LINGERING)) {

                farmAnimal->direction = calculateAnimalDirectionToPlayer(
                    entities[farmAnimal->entityIndex].coordinates.x,
                    entities[farmAnimal->entityIndex].coordinates.z,
                    entities[ENTITY_PLAYER].coordinates.x, 
                    entities[ENTITY_PLAYER].coordinates.z);

                farmAnimal->actionState = 1;
                
            } else {
        
                if (temp < 40) {
                
                    farmAnimal->actionState = 0;

                } else {

                    tempDirection = getRandomNumberInRange(0, 20);

                    if (tempDirection < 7) {
                        farmAnimal->direction = tempDirection;
                    }

                    temp = getRandomNumberInRange(0, 5);

                    if (temp == 0) {
                        farmAnimal->actionState = 1;
                    }
                    
                    if (temp == 1) {
                        farmAnimal->actionState = 2;
                    }

                    if (temp == 2) {
                        farmAnimal->actionState = 3;
                    }

                    if (temp == 3) {
                        farmAnimal->actionState = 4;
                    }

                }
                
            }
        
            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
            break;

case 1:

            farmAnimal->speed = 0.5f;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x70);

            if (farmAnimal->flags & FARM_ANIMAL_APPROACHING) {

                farmAnimal->flags &= ~FARM_ANIMAL_LINGERING;

                farmAnimal->direction = calculateAnimalDirectionToPlayer(
                    entities[farmAnimal->entityIndex].coordinates.x,
                    entities[farmAnimal->entityIndex].coordinates.z,
                    entities[ENTITY_PLAYER].coordinates.x, 
                    entities[ENTITY_PLAYER].coordinates.z);

                if (getRandomNumberInRange(0, 20) == 0) {
                    farmAnimal->flags &= ~FARM_ANIMAL_APPROACHING;
                } 
                
            } else if (farmAnimal->flags & FARM_ANIMAL_LINGERING) {

                 if (getRandomNumberInRange(0, 40) == 0) {
                    farmAnimal->flags &= ~FARM_ANIMAL_LINGERING;
                } 
                
            } else {

                if (getRandomNumberInRange(0, 20) < 10) {
                    farmAnimal->actionState = 1;
                } else {
                    farmAnimal->actionState = 0;
                }
                
            }

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;            
            break;

        case 2:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x88);

            temp = getRandomNumberInRange(0, 2); 
            
            if (temp == 0) {
                farmAnimal->actionState = 2;
            } 
            if (temp == 1) {
                farmAnimal->actionState = 0;
            }
            if (temp == 2) {
                farmAnimal->actionState = 7;
            }

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;  
            break;

        case 3:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x78);
            
            farmAnimal->actionState = 5;
            
            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;  
            break;

        case 4:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0xA8);

            farmAnimal->actionState = 0;

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;  
            break;

        case 5:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x80);

            temp = getRandomNumberInRange(0, 11);

            if (temp < 10) {
                farmAnimal->actionState = 5;
            }

            if (temp == 10) {
                farmAnimal->actionState = 6;
            }

            if (temp == 11) {
                farmAnimal->actionState = 8;
            }

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
            break;

        case 6:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x90);

            if (getRandomNumberInRange(0, 4) >= 4) {
                farmAnimal->actionState = 5;
            } else {
                farmAnimal->actionState = 6;
            }

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
            break;

        case 7:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x98);

            farmAnimal->actionState = 2;

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
            break;

        case 8:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x78);

            farmAnimal->actionState = 0;

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
            
            break;

        case 16:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x68);

            farmAnimal->flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            farmAnimal->flags &= ~(FARM_ANIMAL_LINGERING);
            
            break;
        
        case 17:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x50);

            farmAnimal->actionState = 0;

            farmAnimal->flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            farmAnimal->flags &= ~(FARM_ANIMAL_LINGERING);

            playSfx(0x36);
            
            break;
        
        case 18:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0xA0);

            farmAnimal->actionState = 0;

            farmAnimal->flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            farmAnimal->flags &= ~(FARM_ANIMAL_LINGERING);

            playSfx(0x37);

            break;
        
        case 19:
            
            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x60);

            farmAnimal->actionState = 19;

            farmAnimal->flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            farmAnimal->flags &= ~(FARM_ANIMAL_LINGERING);
            
            break;
        
    }
    
}

void updateAdultCowSick(u8 index) {
    FarmAnimal *farmAnimal = &gFarmAnimals[index];


    u16 temp;
    u16 tempDirection;
    
    switch (farmAnimal->actionState) {

        case 0:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x108);

            if ((farmAnimal->flags & FARM_ANIMAL_APPROACHING) || (farmAnimal->flags & FARM_ANIMAL_LINGERING)) {

                farmAnimal->direction = calculateAnimalDirectionToPlayer(
                    entities[farmAnimal->entityIndex].coordinates.x,
                    entities[farmAnimal->entityIndex].coordinates.z,
                    entities[ENTITY_PLAYER].coordinates.x, 
                    entities[ENTITY_PLAYER].coordinates.z);

                farmAnimal->actionState = 1;
                
            } else {
        
                if (getRandomNumberInRange(0, 60) < 40) {
                
                    farmAnimal->actionState = 0;

                } else {

                    tempDirection = getRandomNumberInRange(0, 20);

                    if (tempDirection < 7) {
                        farmAnimal->direction = tempDirection;
                    }

                    temp = getRandomNumberInRange(0, 5);

                    if (temp == 0) {
                        farmAnimal->actionState = 1;
                    }
                    
                    if (temp == 1) {
                        farmAnimal->actionState = 2;
                    }

                    if (temp == 2) {
                        farmAnimal->actionState = 3;
                    }

                    if (temp == 3) {
                        farmAnimal->actionState = 4;
                    }

                }
                
            }
        
            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
            break;

case 1:

            farmAnimal->speed = 0.5f;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x110);

            if (farmAnimal->flags & FARM_ANIMAL_APPROACHING) {

                farmAnimal->flags &= ~FARM_ANIMAL_LINGERING;

                farmAnimal->direction = calculateAnimalDirectionToPlayer(
                    entities[farmAnimal->entityIndex].coordinates.x,
                    entities[farmAnimal->entityIndex].coordinates.z,
                    entities[ENTITY_PLAYER].coordinates.x, 
                    entities[ENTITY_PLAYER].coordinates.z);

                if (getRandomNumberInRange(0, 20) == 0) {
                    farmAnimal->flags &= ~FARM_ANIMAL_APPROACHING;
                } 
                
            } else if (farmAnimal->flags & FARM_ANIMAL_LINGERING) {

                 if (getRandomNumberInRange(0, 40) == 0) {
                    farmAnimal->flags &= ~FARM_ANIMAL_LINGERING;
                } 
                
            } else {

                if (getRandomNumberInRange(0, 20) < 10) {
                    farmAnimal->actionState = 1;
                } else {
                    farmAnimal->actionState = 0;
                }
                
            }

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;            
            break;

        case 2:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x128);

            temp = getRandomNumberInRange(0, 2); 
            
            if (temp == 0) {
                farmAnimal->actionState = 2;
            } 
            if (temp == 1) {
                farmAnimal->actionState = 0;
            }
            if (temp == 2) {
                farmAnimal->actionState = 6;
            }

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;  
            break;

        case 3:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x118);
            
            farmAnimal->actionState = 5;
            
            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;  
            
            break;

        case 4:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x138);

            farmAnimal->actionState = 0;

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;  
            break;

        case 5:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x120);

            temp = getRandomNumberInRange(0, 11);

            if (temp < 10) {
                farmAnimal->actionState = 5;
            }

            if (temp == 10) {
                farmAnimal->actionState = 6;
            }

            if (temp == 11) {
                farmAnimal->actionState = 7;
            }

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
            break;

        case 6:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x130);

            farmAnimal->actionState = 2;

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
            break;

        case 7:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x118);

            farmAnimal->actionState = 0;

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
            break;

        case 16:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x108);

            farmAnimal->flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            farmAnimal->flags &= ~(FARM_ANIMAL_LINGERING);
            
            break;
        
        case 17:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x50);

            farmAnimal->actionState = 0;

            farmAnimal->flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            farmAnimal->flags &= ~(FARM_ANIMAL_LINGERING);

            playSfx(0x36);
            
            break;
        
        case 18:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x108);

            farmAnimal->actionState = 0;

            farmAnimal->flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            farmAnimal->flags &= ~(FARM_ANIMAL_LINGERING);

            break;
        
        case 19:
            
            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x60);

            farmAnimal->actionState = 19;

            farmAnimal->flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            farmAnimal->flags &= ~(FARM_ANIMAL_LINGERING);
            
            break;
        
    }
    
}

void updateCalf(u8 index) {
    FarmAnimal *farmAnimal = &gFarmAnimals[index];

    
    u16 temp;
    u16 tempDirection;
    
    switch (farmAnimal->actionState) {
        
        case 0:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0);

            if ((farmAnimal->flags & FARM_ANIMAL_APPROACHING) || (farmAnimal->flags & FARM_ANIMAL_LINGERING)) {

                farmAnimal->direction = calculateAnimalDirectionToPlayer(
                    entities[farmAnimal->entityIndex].coordinates.x,
                    entities[farmAnimal->entityIndex].coordinates.z,
                    entities[ENTITY_PLAYER].coordinates.x, 
                    entities[ENTITY_PLAYER].coordinates.z);

                farmAnimal->actionState = 1;
                
            } else {
        
                if (getRandomNumberInRange(0, 60) < 40) {
                    farmAnimal->actionState = 0;
                } else {

                    tempDirection = getRandomNumberInRange(0, 20);

                    if (tempDirection < 7) {
                        farmAnimal->direction = tempDirection;
                    }

                    temp = getRandomNumberInRange(0, 5);

                    if (temp == 0) {
                        farmAnimal->actionState = 1;
                    }
                    
                    if (temp == 1) {
                        farmAnimal->actionState = 2;
                    }

                    if (temp == 2) {
                        farmAnimal->actionState = 3;
                    }

                    if (temp == 3) {
                        farmAnimal->actionState = 4;
                    }
                    
                    if (temp == 4) {
                        farmAnimal->actionState = 5;
                    }

                    if (temp == 5) {
                        farmAnimal->actionState = 6;
                    }

                }
                
            }
        
            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
            break;

case 1:

            farmAnimal->speed = 1.0f;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 8);

            if (farmAnimal->flags & FARM_ANIMAL_APPROACHING) {

                farmAnimal->flags &= ~FARM_ANIMAL_LINGERING;

                farmAnimal->direction = calculateAnimalDirectionToPlayer(
                    entities[farmAnimal->entityIndex].coordinates.x,
                    entities[farmAnimal->entityIndex].coordinates.z,
                    entities[ENTITY_PLAYER].coordinates.x, 
                    entities[ENTITY_PLAYER].coordinates.z);

                if (getRandomNumberInRange(0, 20) == 0) {
                    farmAnimal->flags &= ~FARM_ANIMAL_APPROACHING;
                } 
                
            } else if (farmAnimal->flags & FARM_ANIMAL_LINGERING) {

                 if (getRandomNumberInRange(0, 40) == 0) {
                    farmAnimal->flags &= ~FARM_ANIMAL_LINGERING;
                } 
                
            } else {

                if (getRandomNumberInRange(0, 20) < 10) {
                    farmAnimal->actionState = 1;
                } else {
                    farmAnimal->actionState = 0;
                }
                
            }

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;            
            break;

        case 2:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x10);

            temp = getRandomNumberInRange(0, 2); 
            
            if (temp == 0) {
                farmAnimal->actionState = 2;
            } 
            if (temp == 1) {
                farmAnimal->actionState = 0;
            }
            if (temp == 2) {
                farmAnimal->actionState = 8;
            }

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;  
            break;

        case 3:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x18);

            temp = getRandomNumberInRange(0, 2); 
            
            if (temp == 0) {
                farmAnimal->actionState = 3;
            } 
            if (temp == 1) {
                farmAnimal->actionState = 0;
            }
            if (temp == 2) {
                farmAnimal->actionState = 7;
            }
            
            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;  
            
            break;

        case 4:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 48);

            farmAnimal->actionState = 0;

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;  
            break;

        case 5:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x38);

            farmAnimal->actionState = 0;

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
            break;

        case 6:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x40);

            farmAnimal->actionState = 0;

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
            break;

        case 7:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x20);

            if (getRandomNumberInRange(0, 4) >= 4) {
                farmAnimal->actionState = 3;
            } else {
                farmAnimal->actionState = 7;
            }

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
            break;

        case 8:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x28);

            farmAnimal->actionState = 2;

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
            break;
    
        case 16:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0);

            farmAnimal->flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            farmAnimal->flags &= ~(FARM_ANIMAL_LINGERING);
            
            break;
        
        case 17:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x48);

            farmAnimal->actionState = 0;

            farmAnimal->flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            farmAnimal->flags &= ~(FARM_ANIMAL_LINGERING);

            playSfx(0x36);
            
            break;
        
        case 18:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x50);

            farmAnimal->actionState = 0;

            farmAnimal->flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            farmAnimal->flags &= ~(FARM_ANIMAL_LINGERING);

            playSfx(0x37);
            
            break;
        
        case 19:
            
            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x58);

            farmAnimal->actionState = 19;

            farmAnimal->flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            farmAnimal->flags &= ~(FARM_ANIMAL_LINGERING);
            
            break;
        
    }
    
}

void updateBabyCow(u8 index) {
    FarmAnimal *farmAnimal = &gFarmAnimals[index];


    u16 temp;
    u16 tempDirection;
    
    switch (farmAnimal->actionState) {
        
        case 0:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0);

            if ((farmAnimal->flags & FARM_ANIMAL_APPROACHING) || (farmAnimal->flags & FARM_ANIMAL_LINGERING)) {

                farmAnimal->direction = calculateAnimalDirectionToPlayer(
                    entities[farmAnimal->entityIndex].coordinates.x,
                    entities[farmAnimal->entityIndex].coordinates.z,
                    entities[ENTITY_PLAYER].coordinates.x, 
                    entities[ENTITY_PLAYER].coordinates.z);

                farmAnimal->actionState = 1;
                
            } else {
        
                if (getRandomNumberInRange(0, 60) < 40) {
                    farmAnimal->actionState = 0;
                } else {

                    tempDirection = getRandomNumberInRange(0, 20);

                    if (tempDirection < 7) {
                        farmAnimal->direction = tempDirection;
                    }

                    temp = getRandomNumberInRange(0, 5);

                    if (temp == 0) {
                        farmAnimal->actionState = 1;
                    }
                    
                    if (temp == 1) {
                        farmAnimal->actionState = 2;
                    }

                    if (temp == 2) {
                        farmAnimal->actionState = 3;
                    }

                    if (temp == 3) {
                        farmAnimal->actionState = 4;
                    }
                    
                    if (temp == 4) {
                        farmAnimal->actionState = 5;
                    }

                    if (temp == 5) {
                        farmAnimal->actionState = 6;
                    }

                }
                
            }
        
            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
            break;

case 1:

            farmAnimal->speed = 1.0f;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 8);

            if (farmAnimal->flags & FARM_ANIMAL_APPROACHING) {

                farmAnimal->flags &= ~FARM_ANIMAL_LINGERING;

                farmAnimal->direction = calculateAnimalDirectionToPlayer(
                    entities[farmAnimal->entityIndex].coordinates.x,
                    entities[farmAnimal->entityIndex].coordinates.z,
                    entities[ENTITY_PLAYER].coordinates.x, 
                    entities[ENTITY_PLAYER].coordinates.z);

                if (getRandomNumberInRange(0, 20) == 0) {
                    farmAnimal->flags &= ~FARM_ANIMAL_APPROACHING;
                } 
                
            } else if (farmAnimal->flags & FARM_ANIMAL_LINGERING) {

                 if (getRandomNumberInRange(0, 40) == 0) {
                    farmAnimal->flags &= ~FARM_ANIMAL_LINGERING;
                } 
                
            } else {

                if (getRandomNumberInRange(0, 20) < 10) {
                    farmAnimal->actionState = 1;
                } else {
                    farmAnimal->actionState = 0;
                }
                
            }

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;            
            break;

        case 2:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x10);

            temp = getRandomNumberInRange(0, 2); 
            
            if (temp == 0) {
                farmAnimal->actionState = 2;
            } 
            if (temp == 1) {
                farmAnimal->actionState = 0;
            }
            if (temp == 2) {
                farmAnimal->actionState = 8;
            }

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;  
            break;

        case 3:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x18);

            temp = getRandomNumberInRange(0, 2); 
            
            if (temp == 0) {
                farmAnimal->actionState = 3;
            } 
            if (temp == 1) {
                farmAnimal->actionState = 0;
            }
            if (temp == 2) {
                farmAnimal->actionState = 7;
            }
            
            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;  
            
            break;

        case 4:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 48);

            farmAnimal->actionState = 0;

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;  
            break;

        case 5:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x38);

            farmAnimal->actionState = 0;

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
            break;

        case 6:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x40);

            farmAnimal->actionState = 0;

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
            break;

        case 7:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x20);

            if (getRandomNumberInRange(0, 4) >= 4) {
                farmAnimal->actionState = 3;
            } else {
                farmAnimal->actionState = 7;
            }

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
            break;

        case 8:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x28);

            farmAnimal->actionState = 2;

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
            break;
    
        case 16:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0);

            farmAnimal->flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            farmAnimal->flags &= ~(FARM_ANIMAL_LINGERING);
            
            break;
        
        case 17:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x48);

            farmAnimal->actionState = 0;

            farmAnimal->flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            farmAnimal->flags &= ~(FARM_ANIMAL_LINGERING);

            playSfx(0x36);
            
            break;
        
        case 18:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x50);

            farmAnimal->actionState = 0;

            farmAnimal->flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            farmAnimal->flags &= ~(FARM_ANIMAL_LINGERING);

            playSfx(0x37);
            
            break;
        
        case 19:
            
            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x58);

            farmAnimal->actionState = 19;

            farmAnimal->flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            farmAnimal->flags &= ~(FARM_ANIMAL_LINGERING);
            
            break;
        
    }
    
}

void updatePregnantCow(u8 index) {
    FarmAnimal *farmAnimal = &gFarmAnimals[index];


    u16 temp;
    u16 tempDirection;
    
    switch (farmAnimal->actionState) {
        
        case 0:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0);

            if ((farmAnimal->flags & FARM_ANIMAL_APPROACHING) || (farmAnimal->flags & FARM_ANIMAL_LINGERING)) {

                farmAnimal->direction = calculateAnimalDirectionToPlayer(
                    entities[farmAnimal->entityIndex].coordinates.x,
                    entities[farmAnimal->entityIndex].coordinates.z,
                    entities[ENTITY_PLAYER].coordinates.x, 
                    entities[ENTITY_PLAYER].coordinates.z);

                farmAnimal->actionState = 1;
                
            } else {
        
                if (getRandomNumberInRange(0, 60) < 40) {
                    farmAnimal->actionState = 0;
                } else {

                    tempDirection = getRandomNumberInRange(0, 20);

                    if (tempDirection < 7) {
                        farmAnimal->direction = tempDirection;
                    }

                    temp = getRandomNumberInRange(0, 5);

                    if (temp == 0) {
                        farmAnimal->actionState = 1;
                    }
                    
                    if (temp == 1) {
                        farmAnimal->actionState = 2;
                    }

                    if (temp == 2) {
                        farmAnimal->actionState = 3;
                    }

                    if (temp == 3) {
                        farmAnimal->actionState = 4;
                    }
                    
                    if (temp == 4) {
                        farmAnimal->actionState = 5;
                    }

                    if (temp == 5) {
                        farmAnimal->actionState = 6;
                    }

                }
                
            }
        
            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
            break;
        
        case 1:

            farmAnimal->speed = 0.5f;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 8);

            if (farmAnimal->flags & FARM_ANIMAL_APPROACHING) {

                farmAnimal->flags &= ~FARM_ANIMAL_LINGERING;

                farmAnimal->direction = calculateAnimalDirectionToPlayer(
                    entities[farmAnimal->entityIndex].coordinates.x,
                    entities[farmAnimal->entityIndex].coordinates.z,
                    entities[ENTITY_PLAYER].coordinates.x, 
                    entities[ENTITY_PLAYER].coordinates.z);

                if (getRandomNumberInRange(0, 20) == 0) {
                    farmAnimal->flags &= ~FARM_ANIMAL_APPROACHING;
                } 
                
            } else if (farmAnimal->flags & FARM_ANIMAL_LINGERING) {

                 if (getRandomNumberInRange(0, 40) == 0) {
                    farmAnimal->flags &= ~FARM_ANIMAL_LINGERING;
                } 
                
            } else {

                if (getRandomNumberInRange(0, 20) < 10) {
                    farmAnimal->actionState = 1;
                } else {
                    farmAnimal->actionState = 0;
                }
                
            }

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;            
            break;

        case 2:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x20);

            temp = getRandomNumberInRange(0, 2); 
            
            if (temp == 0) {
                farmAnimal->actionState = 2;
            } 
            if (temp == 1) {
                farmAnimal->actionState = 0;
            }
            if (temp == 2) {
                farmAnimal->actionState = 8;
            }

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;  
            break;

        case 3:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x10);

            farmAnimal->actionState = 9;
            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;  
            
            break;

        case 4:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x38);

            farmAnimal->actionState = 0;

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;  
            break;

        case 5:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x40);

            farmAnimal->actionState = 0;

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
            break;

        case 6:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x48);

            farmAnimal->actionState = 0;

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
            break;

        case 7:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x28);

            if (getRandomNumberInRange(0, 4) >= 4) {
                farmAnimal->actionState = 3;
            } else {
                farmAnimal->actionState = 7;
            }

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
            break;

        case 8:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 48);

            farmAnimal->actionState = 2;

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
            break;
        
        case 9:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x18);

            temp = getRandomNumberInRange(0, 2); 
            
            if (temp == 0) {
                farmAnimal->actionState = 9;
            } 
            if (temp == 1) {
                farmAnimal->actionState = 3;
            }
            if (temp == 2) {
                farmAnimal->actionState = 7;
            }
            
            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
            break;
    
        case 16:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0);

            farmAnimal->flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            farmAnimal->flags &= ~(FARM_ANIMAL_LINGERING);
            
            break;
        
        case 17:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x50);

            farmAnimal->actionState = 0;

            farmAnimal->flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            farmAnimal->flags &= ~(FARM_ANIMAL_LINGERING);

            playSfx(0x36);
            
            break;
        
        case 18:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x58);

            farmAnimal->actionState = 0;

            farmAnimal->flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            farmAnimal->flags &= ~(FARM_ANIMAL_LINGERING);

            playSfx(0x37);
            
            break;
        
        case 19:
            
            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x60);

            farmAnimal->actionState = 19;

            farmAnimal->flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            farmAnimal->flags &= ~(FARM_ANIMAL_LINGERING);
            
            break;
        
    }
    
}

void updateAdultSheepNormal(u8 index) {
    FarmAnimal *farmAnimal = &gFarmAnimals[index];


    u16 temp;
    u16 tempDirection;
    
    switch (farmAnimal->actionState) {
        
        case 0:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0);

            if ((farmAnimal->flags & FARM_ANIMAL_APPROACHING) || (farmAnimal->flags & FARM_ANIMAL_LINGERING)) {

                farmAnimal->direction = calculateAnimalDirectionToPlayer(
                    entities[farmAnimal->entityIndex].coordinates.x,
                    entities[farmAnimal->entityIndex].coordinates.z,
                    entities[ENTITY_PLAYER].coordinates.x, 
                    entities[ENTITY_PLAYER].coordinates.z);

                farmAnimal->actionState = 1;
                
            } else {
        
                if (getRandomNumberInRange(0, 60) < 40) {
                    farmAnimal->actionState = 0;
                } else {

                    tempDirection = getRandomNumberInRange(0, 20);

                    if (tempDirection < 7) {
                        farmAnimal->direction = tempDirection;
                    }

                    temp = getRandomNumberInRange(0, 2);

                    if (temp == 0) {
                        farmAnimal->actionState = 1;
                    }
                    
                    if (temp == 1) {
                        farmAnimal->actionState = 2;
                    }

                    if (temp == 2) {
                        farmAnimal->actionState = 3;
                    }

                }
                
            }
        
            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
            break;

        case 1:

            farmAnimal->speed = 1.0f;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 8);

            if (farmAnimal->flags & FARM_ANIMAL_APPROACHING) {

                farmAnimal->flags &= ~FARM_ANIMAL_LINGERING;

                farmAnimal->direction = calculateAnimalDirectionToPlayer(
                    entities[farmAnimal->entityIndex].coordinates.x,
                    entities[farmAnimal->entityIndex].coordinates.z,
                    entities[ENTITY_PLAYER].coordinates.x, 
                    entities[ENTITY_PLAYER].coordinates.z);

                if (getRandomNumberInRange(0, 20) == 0) {
                    farmAnimal->flags &= ~FARM_ANIMAL_APPROACHING;
                } 
                
            } else if (farmAnimal->flags & FARM_ANIMAL_LINGERING) {

                 if (getRandomNumberInRange(0, 40) == 0) {
                    farmAnimal->flags &= ~FARM_ANIMAL_LINGERING;
                } 
                
            } else {

                if (getRandomNumberInRange(0, 20) < 10) {
                    farmAnimal->actionState = 1;
                } else {
                    farmAnimal->actionState = 0;
                }
                
            }

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;            
            break;

        case 2:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x10);

            if (getRandomNumberInRange(0, 2) == 0) {
                farmAnimal->actionState = 2;
            } else {
                farmAnimal->actionState = 0;
            }

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;  

            break;

        case 3:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x18);

            farmAnimal->actionState = 4;

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;  
            
            break;

case 4:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x20);

            temp = getRandomNumberInRange(0, 11);

            if (temp < 10) {
                farmAnimal->actionState = 4;
            }

            if (temp == 10) {
                farmAnimal->actionState = 4;
            }

            if (temp == 11) {
                farmAnimal->actionState = 6;
            }

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;  
            break;

        case 5:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x28);

            if (getRandomNumberInRange(0, 4) >= 4) {
                farmAnimal->actionState = 4;
            } else {
                farmAnimal->actionState = 5;
            }

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
            break;

        case 6:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x18);

            farmAnimal->actionState = 0;
            
            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
            break;
    
        case 16:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0);

            farmAnimal->flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            farmAnimal->flags &= ~(FARM_ANIMAL_LINGERING);
            
            break;
        
        case 17:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0);

            farmAnimal->actionState = 0;

            farmAnimal->flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            farmAnimal->flags &= ~(FARM_ANIMAL_LINGERING);

            playSfx(0x3A);
            
            break;
        
        case 18:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 48);

            farmAnimal->actionState = 0;

            farmAnimal->flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            farmAnimal->flags &= ~(FARM_ANIMAL_LINGERING);

            playSfx(0x3A);
            
            break;
        
        case 19:
            
            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 48);

            farmAnimal->actionState = 19;

            farmAnimal->flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            farmAnimal->flags &= ~(FARM_ANIMAL_LINGERING);
            
            break;
        
    }
    
}

void updateAdultSheepSick(u8 index) {
    FarmAnimal *farmAnimal = &gFarmAnimals[index];


    switch (farmAnimal->actionState) {
        
        case 0:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x28);

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
            
            break;
    
        case 16:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0);

            farmAnimal->flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            farmAnimal->flags &= ~(FARM_ANIMAL_LINGERING);
            
            break;
        
        case 17:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0);

            farmAnimal->actionState = 0;

            farmAnimal->flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            farmAnimal->flags &= ~(FARM_ANIMAL_LINGERING);
            
            break;
        
        case 18:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 48);

            farmAnimal->actionState = 0;

            farmAnimal->flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            farmAnimal->flags &= ~(FARM_ANIMAL_LINGERING);
            
            break;
        
        case 19:
            
            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 48);

            farmAnimal->actionState = 19;

            farmAnimal->flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            farmAnimal->flags &= ~(FARM_ANIMAL_LINGERING);
            
            break;
    
    }
    
}

void updateShearedSheepNormal(u8 index) {
    FarmAnimal *farmAnimal = &gFarmAnimals[index];


    u16 temp;

    switch(farmAnimal->actionState) {
        
        case 0:
            
            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;
            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x38);

            if (farmAnimal->flags & FARM_ANIMAL_APPROACHING || farmAnimal->flags & FARM_ANIMAL_LINGERING) {
                
                farmAnimal->direction = calculateAnimalDirectionToPlayer(entities[farmAnimal->entityIndex].coordinates.x, entities[farmAnimal->entityIndex].coordinates.z, entities[ENTITY_PLAYER].coordinates.x, entities[ENTITY_PLAYER].coordinates.z);
                farmAnimal->actionState = 1;
            
            } else if (getRandomNumberInRange(0, 60) < 40) {
            
                farmAnimal->actionState = 0;
                    
            } else {
                
                temp = getRandomNumberInRange(0, 20);
                
                if (temp < 7) {
                    farmAnimal->direction = temp;
                }

                temp = getRandomNumberInRange(0, 2);

                if (temp == 0) {
                    farmAnimal->actionState = 1;
                } 
                
                if (temp == 1) {
                    farmAnimal->actionState = 2;
                } 
                
                if (temp == 2) {
                    farmAnimal->actionState = 3;
                }
                
            }

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
            
            break;

        case 1:
            
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;
            farmAnimal->speed = 1.0f;
            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x40);

            if (farmAnimal->flags & FARM_ANIMAL_APPROACHING) {

                farmAnimal->flags &= ~FARM_ANIMAL_LINGERING;
                farmAnimal->direction = calculateAnimalDirectionToPlayer(entities[farmAnimal->entityIndex].coordinates.x, entities[farmAnimal->entityIndex].coordinates.z, entities[ENTITY_PLAYER].coordinates.x, entities[ENTITY_PLAYER].coordinates.z);

                if (!getRandomNumberInRange(0, 20)) {
                    farmAnimal->flags &= ~FARM_ANIMAL_APPROACHING;
                }
                
            } else if (farmAnimal->flags & FARM_ANIMAL_LINGERING) {

                if (!getRandomNumberInRange(0, 40)) {
                    farmAnimal->flags &= ~FARM_ANIMAL_LINGERING;
                }
                
            } else {
                if (getRandomNumberInRange(0, 20) < 10) {
                    farmAnimal->actionState = 1;
                } else {
                    farmAnimal->actionState = 0;
                }
            }

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
            
            break;

        case 2:
            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;
            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x48);
            
            if (!getRandomNumberInRange(0, 2)) {
                farmAnimal->actionState = 2;
            } else {
                farmAnimal->actionState = 0;
            }
            
            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
        
            break;

        case 3:
            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;
            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x50);
            farmAnimal->actionState = 4;
            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
            break;

        case 4:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;
            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x58);

            temp = getRandomNumberInRange(0, 11);

            if (temp < 10) {
                farmAnimal->actionState = 4;
            } 
            
            if (temp == 10) {
                farmAnimal->actionState = 4;
            } 
            
            if (temp == 11) {
                farmAnimal->actionState = 6;
            }

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
            
            break;

        case 5:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;
            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x60);

            if (getRandomNumberInRange(0, 4) >= 4) {
                farmAnimal->actionState = 4;
            } else {
                farmAnimal->actionState = 5;
            }

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;

            break;

        case 6:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;
            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x50);
            farmAnimal->actionState = 0;
            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
            
            break;

        case 16:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;
            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x38);
            farmAnimal->flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            farmAnimal->flags &= ~FARM_ANIMAL_LINGERING;
            break;

        case 17:

            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;
            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x38);
            farmAnimal->actionState = 0;
            farmAnimal->flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            farmAnimal->flags &= ~FARM_ANIMAL_LINGERING;
            playSfx(0x3A);
        
            break;

        case 18:
            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;
            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x68);
            farmAnimal->actionState = 0;
            farmAnimal->flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            farmAnimal->flags &= ~FARM_ANIMAL_LINGERING;
            playSfx(0x3A);
            break;

        case 19:
            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;
            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x68);
            farmAnimal->actionState = 19;
            farmAnimal->flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            farmAnimal->flags &= ~FARM_ANIMAL_LINGERING;
            break;

    }
    
}

void updateShearedSheepSick(u8 farmAnimalIndex) {
    FarmAnimal *farmAnimal = &gFarmAnimals[farmAnimalIndex];


    u16 temp;
    
    switch (farmAnimal->actionState) {
        
        case 0:
            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x60);

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;

            break;

        case 16:    
            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;
    
            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x60);

            farmAnimal->flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            farmAnimal->flags &= ~FARM_ANIMAL_LINGERING;
            
            break;

        case 17:
            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;
    
            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x60);

            farmAnimal->actionState = 0;

            farmAnimal->flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            farmAnimal->flags &= ~FARM_ANIMAL_LINGERING;
            
            break;
        
        case 18:
            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;
    
            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x68);

            farmAnimal->actionState = 0;

            farmAnimal->flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            farmAnimal->flags &= ~FARM_ANIMAL_LINGERING;
            
            break;

        case 19:    
            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;
    
            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x68);

            farmAnimal->actionState = 19;

            farmAnimal->flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            farmAnimal->flags &= ~FARM_ANIMAL_LINGERING;
            
            break;

    }
    
}

void updateBabySheep(u8 farmAnimalIndex) {
    FarmAnimal *farmAnimal = &gFarmAnimals[farmAnimalIndex];


    u16 temp;
    
    switch (farmAnimal->actionState) {
        
        case 0:
            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0);

            if (farmAnimal->flags & FARM_ANIMAL_APPROACHING || farmAnimal->flags & FARM_ANIMAL_LINGERING) {

                farmAnimal->direction = calculateAnimalDirectionToPlayer(entities[farmAnimal->entityIndex].coordinates.x, 
                                   entities[farmAnimal->entityIndex].coordinates.z, entities[ENTITY_PLAYER].coordinates.x, entities[ENTITY_PLAYER].coordinates.z);

                farmAnimal->actionState = 1;
                
            } else if (getRandomNumberInRange(0, 60) < 40) {

                farmAnimal->actionState = 0;
                
            } else {

                temp = getRandomNumberInRange(0, 20);

                if (temp < 7) {
                    farmAnimal->direction = temp;
                }

                temp = getRandomNumberInRange(0, 2);

                if (temp == 0) {
                    farmAnimal->actionState = 1;
                }

                if (temp == 1) {
                    farmAnimal->actionState = 2;
                }

                if (temp == 2) {
                    farmAnimal->actionState = 3;
                }
                
            }

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;

            break;

        case 1:
            farmAnimal->speed = 1.0f;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;

            setEntityDirectionalAnimation(farmAnimal->entityIndex, 8);

            if (farmAnimal->flags & FARM_ANIMAL_APPROACHING) {

                farmAnimal->flags &= ~FARM_ANIMAL_LINGERING;

                farmAnimal->direction = calculateAnimalDirectionToPlayer(entities[farmAnimal->entityIndex].coordinates.x, 
                                   entities[farmAnimal->entityIndex].coordinates.z, entities[ENTITY_PLAYER].coordinates.x, entities[ENTITY_PLAYER].coordinates.z);

                if (!getRandomNumberInRange(0, 20)) {
                    farmAnimal->flags &= ~FARM_ANIMAL_APPROACHING;
                }
                
            } else if (farmAnimal->flags & FARM_ANIMAL_LINGERING) {

                if (!getRandomNumberInRange(0, 40)) {
                    farmAnimal->flags &= ~FARM_ANIMAL_LINGERING;
                }
                
            } else {

                if (getRandomNumberInRange(0, 20) < 10) {
                    farmAnimal->actionState = 1;
                } else {
                    farmAnimal->actionState = 0;
                }
                
            }

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;

            break;

        case 2:    
            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;
    
            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x10);

            if (!getRandomNumberInRange(0, 2)) {
                farmAnimal->actionState = 2;
            } else {
                farmAnimal->actionState = 0;
            }

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
            
            break;
        
        case 3:    
            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;
    
            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x20);

            farmAnimal->actionState = 4;

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
            
            break;
        
        case 4:    
            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;
    
            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x28);

            temp = getRandomNumberInRange(0, 11); 

            if (temp < 10) {
                farmAnimal->actionState = 4;
            } 
            
            if (temp == 10) {
                farmAnimal->actionState = 4;
            } 
            
            if (temp == 11) {
                farmAnimal->actionState = 6;
            } 

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
            
            break;
    
        case 5:    
            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;
    
            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x18);

            if (getRandomNumberInRange(0, 4) < 4) {
                farmAnimal->actionState = 5;
            } else {
                farmAnimal->actionState = 4;
            }

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
            
            break;

        case 6:    
            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;
    
            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0x20);

            farmAnimal->actionState = 0;

            farmAnimal->flags |= FARM_ANIMAL_STATE_CHANGED;
            
            break;

        case 16:    
            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;
    
            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0);

            farmAnimal->flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            farmAnimal->flags &= ~FARM_ANIMAL_LINGERING;
            
            break;

        case 17:
        case 18:
            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;
    
            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0);

            farmAnimal->actionState = 0;

            farmAnimal->flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            farmAnimal->flags &= ~FARM_ANIMAL_LINGERING;
            playSfx(0x3A);
            
            break;

        case 19:    
            farmAnimal->speed = 0;
            farmAnimal->stateTimer = 0;
            farmAnimal->unk_1E = 0;
    
            setEntityDirectionalAnimation(farmAnimal->entityIndex, 0);

            farmAnimal->actionState = 19;

            farmAnimal->flags |= (FARM_ANIMAL_STATE_CHANGED | FARM_ANIMAL_FOLLOWING);
            farmAnimal->flags &= ~FARM_ANIMAL_LINGERING;
            
            break;

    }
    
}

void updateHorse(void) {
    Horse *horse = &horseInfo;


    Vec3f vec;

    if ((horse->flags & HORSE_ACTIVE) && (horse->flags & HORSE_ENTITY_LOADED)) {

        if (checkEntityAnimationStateChanged(horse->entityIndex) || horse->flags & HORSE_COLLISION_WITH_PLAYER) {
            
            switch (horse->grown) {
                case FALSE:
                    updateHorseNotGrown();
                    break;                    
                case TRUE:
                    updateHorseGrown();
                    break;
            }
            
            horse->flags &= ~(HORSE_COLLISION_WITH_PLAYER  | 0x80);
            
        }
        
        setEntityDirection(horse->entityIndex, convertWorldDirectionToScreenDirection(horse->direction, MAIN_MAP_INDEX));
        vec = getMovementVectorFromDirection(horse->speed, horse->direction, 0.0f);
        setEntityMovementVector(horse->entityIndex, vec.x, vec.y, vec.z, horse->speed);

    } 
}

void updateHorseGrown(void) {
    Horse *horse = &horseInfo;


    u16 tempDirection;
    u16 temp;
    
    switch (horse->actionState) {

        case 0:
            
            horse->speed = 0;
            horse->stateTimer = 0;
            horse->unk_1B = 0;
            
            setEntityDirectionalAnimation(horse->entityIndex, 0);
            
            if (horse->flags & HORSE_FOLLOWING) {

                horse->direction = calculateAnimalDirectionToPlayer(entities[horse->entityIndex].coordinates.x, entities[horse->entityIndex].coordinates.z, entities[ENTITY_PLAYER].coordinates.x, entities[ENTITY_PLAYER].coordinates.z);
                horse->actionState = 4;
                
            } else {

                if (getRandomNumberInRange(0, 60) < 40)  {
                    
                    horse->actionState = 0;
                    
                } else {
                        
                    tempDirection = getRandomNumberInRange(0, 20);

                    if (tempDirection < 7) {
                        horse->direction = tempDirection;
                    }

                    temp = getRandomNumberInRange(0, 5);

                    if (temp == 0) {
                        horse->actionState = 1;
                    }
                    
                    if (temp == 1) {
                        horse->actionState = 2;
                    }

                    if (temp == 2) {
                        horse->actionState = 3;
                    }

                    if (temp == 3) {
                        horse->actionState = 4;
                    }

                    if (temp == 4) {
                        horse->actionState = 5;
                    }

                    if (temp == 5) {
                        horse->actionState = 6;
                    }
                    
                }

            } 
            
            horse->flags |= HORSE_STATE_CHANGED;
            break;

        case 1:

            horse->speed = 1;
            horse->stateTimer = 0;
            horse->unk_1B = 0;
            
            setEntityDirectionalAnimation(horse->entityIndex, 8);
            
            if (getRandomNumberInRange(0, 20) < 10) {
                horse->actionState = 1;
            } else {
                horse->actionState = 0;
            }

            horse->flags |= HORSE_STATE_CHANGED;
            break;

        case 2:

            horse->speed = 0;
            horse->stateTimer = 0;
            horse->unk_1B = 0;
            
            setEntityDirectionalAnimation(horse->entityIndex, 0x20);

            if (getRandomNumberInRange(0, 2) == 0) {
                horse->actionState = 2;
            } else {   
                horse->actionState = 0;    
            }       
            
            horse->flags |= HORSE_STATE_CHANGED;
            break;

        case 3:

            horse->speed = 0;
            horse->stateTimer = 0;
            horse->unk_1B = 0;
            
            setEntityDirectionalAnimation(horse->entityIndex, 0x18);
            
            horse->actionState = 7;
            horse->flags |= HORSE_STATE_CHANGED;
            
            break;

        case 4:

            horse->speed = 2;
            horse->stateTimer = 0;
            horse->unk_1B = 0;
            
            setEntityDirectionalAnimation(horse->entityIndex, 0x10);

            if (horse->flags & HORSE_FOLLOWING) {
                
                horse->direction = calculateAnimalDirectionToPlayer(entities[horse->entityIndex].coordinates.x, entities[horse->entityIndex].coordinates.z, entities[ENTITY_PLAYER].coordinates.x, entities[ENTITY_PLAYER].coordinates.z);
                
                if (getRandomNumberInRange(0, 20) == 0) {
                    horse->flags &= ~(HORSE_FOLLOWING);
                }
                
            } else {
                
                if (getRandomNumberInRange(0, 19) < 10) {
                    horse->actionState = 4;
                } else {                
                    horse->actionState = 0;    
                }
                
            }
        
            horse->flags |= HORSE_STATE_CHANGED;
            break;

        case 5:

            horse->speed = 0;
            horse->stateTimer = 0;
            horse->unk_1B = 0;
            
            setEntityDirectionalAnimation(horse->entityIndex, 0x48);

            if (getRandomNumberInRange(0, 2) == 0) {
                horse->actionState = 5;
            } else {   
                horse->actionState = 0;    
            }
           
            horse->flags |= HORSE_STATE_CHANGED;
            break;

        case 6:

            horse->speed = 0;
            horse->stateTimer = 0;
            horse->unk_1B = 0;
            
            setEntityDirectionalAnimation(horse->entityIndex, 0x50);

            if (getRandomNumberInRange(0, 2) == 0) {
                horse->actionState = 6;
            } else {
                horse->actionState = 0;    
            }
           
            horse->flags |= HORSE_STATE_CHANGED;
            break;

        case 7:

            horse->speed = 0;
            horse->stateTimer = 0;
            horse->unk_1B = 0;
            
            setEntityDirectionalAnimation(horse->entityIndex, 0x28);

            if (getRandomNumberInRange(0, 2) == 0) {
                horse->actionState = 7;
            } else {
                horse->actionState = 0;    
            }
           
            horse->flags |= HORSE_STATE_CHANGED;
            break;

        case 8:
            
            horse->speed = 0;
            horse->stateTimer = 0;
            horse->unk_1B = 0;
            
            setEntityDirectionalAnimation(horse->entityIndex, 48);

            if (getRandomNumberInRange(0, 2) == 0) {
                horse->actionState = 8;
            } else {
                horse->actionState = 0;    
            }
           
            horse->flags |= HORSE_STATE_CHANGED;
            break;

case 16:
            
            horse->speed = 0;
            horse->stateTimer = 0;
            horse->unk_1B = 0;
            
            setEntityDirectionalAnimation(horse->entityIndex, 0);
            
            horse->flags |= (HORSE_STATE_CHANGED | 0x80); 
            horse->flags &= ~(0x40);
            
            break;

        case 17:

            horse->speed = 0;
            horse->stateTimer = 0;
            horse->unk_1B = 0;
            
            setEntityDirectionalAnimation(horse->entityIndex, 0x48);
            
            horse->actionState = 0;
            horse->flags |= (HORSE_STATE_CHANGED | 0x80); 
            horse->flags &= ~(0x40);
            
            playSfx(0x39);
            
            break;

        case 18:

            horse->speed = 0;
            horse->stateTimer = 0;
            horse->unk_1B = 0;
            
            setEntityDirectionalAnimation(horse->entityIndex, 0);
            
            horse->actionState = 0;
            horse->flags |= (HORSE_STATE_CHANGED | 0x80); 
            horse->flags &= ~(0x40);
            
            playSfx(0x38);
            
            break;

        default:
            break;
        
    }
    
}

void updateHorseNotGrown(void) {
    Horse *horse = &horseInfo;


    u16 tempDirection;
    u16 temp;
    
    switch (horse->actionState) {

        case 0:
            
            horse->speed = 0;
            horse->stateTimer = 0;
            horse->unk_1B = 0;
            
            setEntityDirectionalAnimation(horse->entityIndex, 0);
            
            if (horse->flags & HORSE_FOLLOWING) {

                horse->direction = calculateAnimalDirectionToPlayer(entities[horse->entityIndex].coordinates.x, entities[horse->entityIndex].coordinates.z, entities[ENTITY_PLAYER].coordinates.x, entities[ENTITY_PLAYER].coordinates.z);
                horse->actionState = 4;
                
            } else {

                if (getRandomNumberInRange(0, 60) < 40)  {
                    
                    horse->actionState = 0;
                    
                } else {
                        
                    tempDirection = getRandomNumberInRange(0, 20);

                    if (tempDirection < 7) {
                        horse->direction = tempDirection;
                    }

                    temp = getRandomNumberInRange(0, 5);

                    if (temp == 0) {
                        horse->actionState = 1;
                    }
                    
                    if (temp == 1) {
                        horse->actionState = 2;
                    }

                    if (temp == 2) {
                        horse->actionState = 3;
                    }

                    if (temp == 3) {
                        horse->actionState = 4;
                    }

                    if (temp == 4) {
                        horse->actionState = 5;
                    }
                    
                }

            } 
            
            horse->flags |= HORSE_STATE_CHANGED;
            break;

        case 1:

            horse->speed = 1;
            horse->stateTimer = 0;
            horse->unk_1B = 0;
            
            setEntityDirectionalAnimation(horse->entityIndex, 8);
            
            if (getRandomNumberInRange(0, 20) < 10) {
                horse->actionState = 1;
            } else {
                horse->actionState = 0;
            }

            horse->flags |= HORSE_STATE_CHANGED;
            break;

        case 2:

            horse->speed = 0;
            horse->stateTimer = 0;
            horse->unk_1B = 0;
            
            setEntityDirectionalAnimation(horse->entityIndex, 0x20U);

            if (getRandomNumberInRange(0, 2) == 0) {
                horse->actionState = 2;
            } else {   
                horse->actionState = 0;    
            }       
            
            horse->flags |= HORSE_STATE_CHANGED;
            break;

        case 3:

            horse->speed = 0;
            horse->stateTimer = 0;
            horse->unk_1B = 0;
            
            setEntityDirectionalAnimation(horse->entityIndex, 0x18);
            
            horse->actionState = 6;
            horse->flags |= HORSE_STATE_CHANGED;
            
            break;

        case 4:

            horse->speed = 2;
            horse->stateTimer = 0;
            horse->unk_1B = 0;
            
            setEntityDirectionalAnimation(horse->entityIndex, 0x10);

            if (horse->flags & HORSE_FOLLOWING) {
                
                horse->direction = calculateAnimalDirectionToPlayer(entities[horse->entityIndex].coordinates.x, entities[horse->entityIndex].coordinates.z, entities[ENTITY_PLAYER].coordinates.x, entities[ENTITY_PLAYER].coordinates.z);
                
                if (getRandomNumberInRange(0, 20) == 0) {
                    horse->flags &= ~HORSE_FOLLOWING;
                }
                
            } else {
                
                if (getRandomNumberInRange(0, 2) == 0) {
                    horse->actionState = 4;
                } else {                
                    horse->actionState = 0;    
                }
                
            }
        
            horse->flags |= HORSE_STATE_CHANGED;
            break;

        case 5:

            horse->speed = 0;
            horse->stateTimer = 0;
            horse->unk_1B = 0;
            
            setEntityDirectionalAnimation(horse->entityIndex, 48);

            if (getRandomNumberInRange(0, 2) == 0) {
                horse->actionState = 5;
            } else {   
                horse->actionState = 0;    
            }
           
            horse->flags |= HORSE_STATE_CHANGED;
            break;

        case 6:

            horse->speed = 0;
            horse->stateTimer = 0;
            horse->unk_1B = 0;
            
            setEntityDirectionalAnimation(horse->entityIndex, 0x28);

            if (getRandomNumberInRange(0, 2) == 0) {
                horse->actionState = 6;
            } else {
                horse->actionState = 0;    
            }
           
            horse->flags |= HORSE_STATE_CHANGED;
            break;
        
        case 16:
            
            horse->speed = 0;
            horse->stateTimer = 0;
            horse->unk_1B = 0;
            
            setEntityDirectionalAnimation(horse->entityIndex, 0);
            
            horse->flags |= (HORSE_STATE_CHANGED | 0x80); 
            horse->flags &= ~(0x40);
            
            break;

        case 17:

            horse->speed = 0;
            horse->stateTimer = 0;
            horse->unk_1B = 0;
            
            setEntityDirectionalAnimation(horse->entityIndex, 48);
            
            horse->actionState = 0;
            horse->flags |= (HORSE_STATE_CHANGED | 0x80); 
            horse->flags &= ~(0x40);
            
            playSfx(0x39);
            
            break;

        case 18:

            horse->speed = 0;
            horse->stateTimer = 0;
            horse->unk_1B = 0;
            
            setEntityDirectionalAnimation(horse->entityIndex, 0);
            
            horse->actionState = 0;
            horse->flags |= (HORSE_STATE_CHANGED | 0x80); 
            horse->flags &= ~(0x40);
            
            playSfx(0x38);
            
            break;

        default:
            break;
        
    }
    
}

void updateMiscAnimal(u8 index) {
    MiscAnimal *miscAnimal = &gMiscAnimals[index];


    Vec3f vec;

    if ((miscAnimal->flags & MISC_ANIMAL_ACTIVE) && (miscAnimal->flags & MISC_ANIMAL_ENTITY_LOADED)) {

        if (checkEntityAnimationStateChanged(miscAnimal->entityIndex) || miscAnimal->actionState == 0x20) {

            switch (miscAnimal->animalType) {

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
                    updateRanchSheep(index);
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
         
        setEntityDirection(miscAnimal->entityIndex, convertWorldDirectionToScreenDirection(miscAnimal->direction, MAIN_MAP_INDEX));
        
        vec = getMovementVectorFromDirection(miscAnimal->zDisplacement, miscAnimal->direction, 0.0f);
        vec.y = miscAnimal->yDisplacement;
        
        setEntityMovementVector(miscAnimal->entityIndex, vec.x, vec.y, vec.z, miscAnimal->zDisplacement);
        
    }
    
}

void updateMiscDog(u8 index) {
    MiscAnimal *miscAnimal = &gMiscAnimals[index];

    
    u16 temp;
    
    switch (miscAnimal->actionState) {

         case 0:

            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 10;
            miscAnimal->unk_14 = 0;

            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0);

             if (getRandomNumberInRange(0, 60) < 40) {
                miscAnimal->actionState = 0;
            } else {
                 
                temp = getRandomNumberInRange(0, 20);

                if (temp < 7) {
                    miscAnimal->direction = temp;
                }

                temp = getRandomNumberInRange(0, 11);

                if (temp == 0) {
                    miscAnimal->actionState = 1;
                }
                
                if (temp == 1) {
                    miscAnimal->actionState = 2;
                }
                
                if (temp == 2) {
                    miscAnimal->actionState = 3;
                }

                if (temp == 3) {
                    miscAnimal->actionState = 4;
                }
                 
                if (temp == 4) {
                    miscAnimal->actionState = 5;
                }
                
                if (temp == 5) {
                    miscAnimal->actionState = 6;
                }
                
                if (temp == 6) {
                    miscAnimal->actionState = 7;
                }

                if (temp == 9) {
                    miscAnimal->actionState = 10;
                }
                 
                if (temp == 10) {
                    miscAnimal->actionState = 24;
                }
                 
                if (temp == 11) {
                    miscAnimal->actionState = 25;
                }             
                 
            }

            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
        
            break;

        case 1:
            miscAnimal->zDisplacement = 1;
            miscAnimal->timer = 10;
            miscAnimal->unk_14 = 0;
            setEntityDirectionalAnimation(miscAnimal->entityIndex, 8);

            temp = getRandomNumberInRange(0, 19);
            
            if (temp < 10) {
                miscAnimal->actionState = 1;
            } else if (temp < 15) {
                miscAnimal->actionState = 0;
            } else {
                miscAnimal->actionState = 4;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;

            break;
        
        case 2:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;
            setEntityDirectionalAnimation(miscAnimal->entityIndex, 48);
            
            miscAnimal->actionState = 16;
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
        
            break;

        case 3:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 10;
            miscAnimal->unk_14 = 0;
            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0x20);

            temp = getRandomNumberInRange(0, 20);
            
            if (temp < 10) {

                miscAnimal->actionState = 3;

            } else {

                if (9 < temp && temp < 12) {
                    miscAnimal->actionState = 0;
                } 
                if (11 < temp && temp < 14) {
                    miscAnimal->actionState = 11;
                }
                if (13 < temp && temp < 16) {
                    miscAnimal->actionState = 12;   
                }
                if (15 < temp && temp < 18) {
                    miscAnimal->actionState = 13;   
                }
                if (17 < temp && temp < 20) {
                    miscAnimal->actionState = 14;   
                }

            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;

        case 4:
            miscAnimal->zDisplacement = 2;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;
            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0x10);

            temp = getRandomNumberInRange(0, 19);
            
            if (temp >= 10) {

                if (9 < temp && temp < 13) {
                    miscAnimal->actionState = 0;
                } 
                if (12 < temp && temp < 16) {
                    miscAnimal->actionState = 1;
                }
                if (15 < temp && temp < 19) {
                    miscAnimal->actionState = 21;   
                }

            } else {
                miscAnimal->actionState = 4;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;

            break;

        case 5:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;

            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0x90);

            if (!getRandomNumberInRange(0, 2)) {
                miscAnimal->actionState = 5;
            } else {
                miscAnimal->actionState = 3;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;

        case 6:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;

            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0xA0);

            if (getRandomNumberInRange(0, 2)) {
                miscAnimal->actionState = 0;
            } else {
                miscAnimal->actionState = 6;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;
        
        case 7:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;

            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0xA8);

            if (getRandomNumberInRange(0, 2)) {
                miscAnimal->actionState = 0;
            } else {
                miscAnimal->actionState = 7;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;

        case 8:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;

            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0xB0);

            if (getRandomNumberInRange(0, 2)) {
                miscAnimal->actionState = 0;
            } else {
                miscAnimal->actionState = 8;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;

        case 9:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;

            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0xB8);

            miscAnimal->actionState = 26;
            
            break;

        case 10:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 10;
            miscAnimal->unk_14 = 0;

            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0xC8);

            if (!getRandomNumberInRange(0, 2)) {
                miscAnimal->actionState = 10;
            } else {
                miscAnimal->actionState = 0;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;

        case 11:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 10;
            miscAnimal->unk_14 = 0;

            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0x28);

            if (!getRandomNumberInRange(0, 2)) {
                miscAnimal->actionState = 11;
            } else {
                miscAnimal->actionState = 3;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;

        case 12:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 10;
            miscAnimal->unk_14 = 0;
            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0x50);

            if (!getRandomNumberInRange(0, 2)) {
                miscAnimal->actionState = 12;
            } else {
                miscAnimal->actionState = 3;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;

            break;
     
        case 13:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;

            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0x58);

            if (!getRandomNumberInRange(0, 2)) {
                miscAnimal->actionState = 13;
            } else {
                miscAnimal->actionState = 3;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;
        
        case 14:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;

            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0x60);

            if (!getRandomNumberInRange(0, 2)) {
                miscAnimal->actionState = 14;
            } else {
                miscAnimal->actionState = 3;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;

        case 16:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 10;
            miscAnimal->unk_14 = 0;

            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0x28);

            temp = getRandomNumberInRange(0, 14);
            
            if (temp < 10) {
                miscAnimal->actionState = 16;
            } 
            if (temp == 10) {
                miscAnimal->actionState = 27;
            } 
            if (temp == 11) {
                miscAnimal->actionState = 17;
            }  
            if (temp == 12) {
                miscAnimal->actionState = 18;
            }
            if (temp == 13) {
                miscAnimal->actionState = 19;
            }
            if (temp == 14) {
                miscAnimal->actionState = 20;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;
        
        case 17:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;

            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0x68);

            if (!getRandomNumberInRange(0, 2)) {
                miscAnimal->actionState = 17;
            } else {
                miscAnimal->actionState = 16;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;
                
        case 18:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;

            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0x70);

            if (!getRandomNumberInRange(0, 2)) {
                miscAnimal->actionState = 18;
            } else {
                miscAnimal->actionState = 16;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;
                
        case 19:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;

            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0x78);

            if (!getRandomNumberInRange(0, 2)) {
                miscAnimal->actionState = 19;
            } else {
                miscAnimal->actionState = 16;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;
                
        case 20:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 10;
            miscAnimal->unk_14 = 0;

            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0x80);

            if (!getRandomNumberInRange(0, 2)) {
                miscAnimal->actionState = 20;
            } else {
                miscAnimal->actionState = 16;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;
                 
        case 21:
            miscAnimal->zDisplacement = 1;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;

            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0x38);

            miscAnimal->actionState = 22;
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;       
                 
        case 22:
            miscAnimal->zDisplacement = 2;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;

            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0x40);

            miscAnimal->actionState = 23;
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;       

        case 23:
            miscAnimal->zDisplacement = 1;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;

            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0x48);

            miscAnimal->actionState = 0;
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;      

        case 24:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;

            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0x88);

            if (!getRandomNumberInRange(0, 2)) {
                miscAnimal->actionState = 24;
            } else {
                miscAnimal->actionState = 0;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;      

        case 25:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;

            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0x98);

            if (!getRandomNumberInRange(0, 2)) {
                miscAnimal->actionState = 25;
            } else {
                miscAnimal->actionState = 0;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;      

        case 26:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;

            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0xC0);

            miscAnimal->actionState = 0;
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;      

        case 27:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;

            setEntityDirectionalAnimation(miscAnimal->entityIndex, 48);

            miscAnimal->actionState = 0;
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;      
        
     }
    
}

void updateVillageDog(u8 index) {
    MiscAnimal *miscAnimal = &gMiscAnimals[index];


    u16 temp;
    
    switch (miscAnimal->actionState) {

         case 0:

            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;
            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0);

             if (getRandomNumberInRange(0, 60) < 40) {
                miscAnimal->actionState = 0;
            } else {
                 
                temp = getRandomNumberInRange(0, 20);

                if (temp < 7) {
                    miscAnimal->direction = temp;
                }

                temp = getRandomNumberInRange(0, 8);

                if (temp == 0) {
                    miscAnimal->actionState = 1;
                }
                
                if (temp == 1) {
                    miscAnimal->actionState = 2;
                }
                
                if (temp == 2) {
                    miscAnimal->actionState = 3;
                }

                if (temp == 3) {
                    miscAnimal->actionState = 4;
                }
                 
                if (temp == 4) {
                    miscAnimal->actionState = 5;
                }
                
                if (temp == 5) {
                    miscAnimal->actionState = 6;
                }
                
                if (temp == 6) {
                    miscAnimal->actionState = 7;
                }

                if (temp == 7) {
                    miscAnimal->actionState = 8;
                }
                 
                if (temp == 8) {
                    miscAnimal->actionState = 9;
                }
                 
            }

            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
        
            break;

        case 1:
            miscAnimal->zDisplacement = 1;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;
            setEntityDirectionalAnimation(miscAnimal->entityIndex, 8);

            temp = getRandomNumberInRange(0, 19);
            
            if (temp < 10) {
                miscAnimal->actionState = 1;
            } else if (temp < 15) {
                miscAnimal->actionState = 0;
            } else {
                miscAnimal->actionState = 4;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;

            break;
        
        case 2:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;
            setEntityDirectionalAnimation(miscAnimal->entityIndex, 48);
            
            miscAnimal->actionState = 12;
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
        
            break;

        case 3:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;
            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0x20);

            temp = getRandomNumberInRange(0, 20);
            
            if (temp >= 10) {

                if (9 < temp && temp < 13) {
                    miscAnimal->actionState = 0;
                } 
                if (13 < temp && temp < 17) {
                    miscAnimal->actionState = 10;
                }
                if (16 < temp && temp < 20) {
                    miscAnimal->actionState = 11;   
                }
                
            } else {
                miscAnimal->actionState = 3;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;

        case 4:
            miscAnimal->zDisplacement = 2;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;
            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0x10);

            temp = getRandomNumberInRange(0, 19);
            
            if (temp < 10) {
                miscAnimal->actionState = 4;
            } else if (temp < 15) {
                miscAnimal->actionState = 0;
            } else {
                miscAnimal->actionState = 1;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;

            break;

        case 5:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;

            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0x50);

            if (!getRandomNumberInRange(0, 2)) {
                miscAnimal->actionState = 5;
            } else {
                miscAnimal->actionState = 3;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;

        case 6:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;

            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0x58);

            if (!getRandomNumberInRange(0, 2)) {
                miscAnimal->actionState = 6;
            } else {
                miscAnimal->actionState = 0;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;
        
        case 7:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;

            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0x60);

            if (!getRandomNumberInRange(0, 2)) {
                miscAnimal->actionState = 7;
            } else {
                miscAnimal->actionState = 0;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;

        case 8:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;

            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0x70);

            if (!getRandomNumberInRange(0, 2)) {
                miscAnimal->actionState = 8;
            } else {
                miscAnimal->actionState = 0;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;

        case 9:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;

            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0x68);

            if (!getRandomNumberInRange(0, 2)) {
                miscAnimal->actionState = 9;
            } else {
                miscAnimal->actionState = 0;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;

        case 10:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;

            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0x18);

            if (!getRandomNumberInRange(0, 2)) {
                miscAnimal->actionState = 10;
            } else {
                miscAnimal->actionState = 3;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;

        case 11:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;

            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0x38);

            if (!getRandomNumberInRange(0, 2)) {
                miscAnimal->actionState = 11;
            } else {
                miscAnimal->actionState = 3;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;

        case 12:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;
            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0x28);

            temp = getRandomNumberInRange(0, 12);
            
            if (temp < 10) {
                miscAnimal->actionState = 12;
            } 
            if (temp == 10) {
                miscAnimal->actionState = 15;
            } 
            if (temp == 11) {
                miscAnimal->actionState = 13;
            }  
            if (temp == 12) {
                miscAnimal->actionState = 14;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;

            break;
     
        case 13:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;

            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0x40);

            if (!getRandomNumberInRange(0, 2)) {
                miscAnimal->actionState = 13;
            } else {
                miscAnimal->actionState = 14;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;
        
        case 14:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;

            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0x48);

            if (!getRandomNumberInRange(0, 2)) {
                miscAnimal->actionState = 14;
            } else {
                miscAnimal->actionState = 14;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;

        case 15:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;

            setEntityDirectionalAnimation(miscAnimal->entityIndex, 48);

            miscAnimal->actionState = 0;
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;
        
     }
    
}

void updateCat(u8 index) {
    MiscAnimal *miscAnimal = &gMiscAnimals[index];

    
    u32 temp;
    u16 temp2;
    u16 temp3;
    u8 tempDirection;
    
    switch (miscAnimal->actionState) {

        case 0:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;
            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0);

            if (getRandomNumberInRange(0, 60) < 40) {
                miscAnimal->actionState = 0;
            } else {

                temp = getRandomNumberInRange(0, 10);

                if (temp < 7) {
                    temp &= 0xFFFE;
                    temp2 = temp;
                    miscAnimal->direction = temp2;
                }

                temp = getRandomNumberInRange(0, 4);

                if (temp == 0) {
                    miscAnimal->actionState = 1;
                }
                
                if (temp == 1) {
                    miscAnimal->actionState = 2;
                }
                
                if (temp == 2) {
                    miscAnimal->actionState = 3;
                }

                if (temp == 3) {
                    miscAnimal->actionState = 4;
                }

            }

            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
        
            break;

        case 1:
            miscAnimal->zDisplacement = 1;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;
            setEntityDirectionalAnimation(miscAnimal->entityIndex, 8);

            temp2 = getRandomNumberInRange(0, 19);
            
            if (temp2 < 10) {
                miscAnimal->actionState = 1;
            } else if (temp2 < 15) {
                miscAnimal->actionState = 0;
            } else {
                miscAnimal->actionState = 5;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;

            playSfx(0x43);
            break;

        case 2:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;
            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0x20);

            temp3 = getRandomNumberInRange(0, 20);
            
            if (temp3 >= 10) {

                if (9 < temp3 && temp3 < 13) {
                    miscAnimal->actionState = 0;
                } 
                if (13 < temp3 && temp3 < 17) {
                    miscAnimal->actionState = 6;
                }
                if (16 < temp3 && temp3 < 20) {
                    miscAnimal->actionState = 7;   
                }
                
            } else {
                miscAnimal->actionState = 2;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;

        case 3:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;

            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0x18);

            temp = getRandomNumberInRange(0, 19);

            if (temp >= 10) {

                if (temp >= 15) {
                    miscAnimal->actionState = 2;
                } else {
                    miscAnimal->actionState = 0;
                }
                
            } else {
                miscAnimal->actionState = 3;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;
        
        case 4:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;

            setEntityAnimation(miscAnimal->entityIndex, 0x38);

            if (!getRandomNumberInRange(0, 2)) {
                miscAnimal->actionState = 4;
            } else {
                miscAnimal->actionState = 0;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;
        
        case 5:
            miscAnimal->zDisplacement = 1;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;

            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0x10);

            if (!getRandomNumberInRange(0, 2)) {
                miscAnimal->actionState = 5;
            } else {
                miscAnimal->actionState = 1;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;
        
       case 6:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;

            setEntityDirectionalAnimation(miscAnimal->entityIndex, 48);

            if (!getRandomNumberInRange(0, 2)) {
                miscAnimal->actionState = 6;
            } else {
                miscAnimal->actionState = 2;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;     

        case 7:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;

            setEntityAnimation(miscAnimal->entityIndex, 0x39);

            if (!getRandomNumberInRange(0, 2)) {
                miscAnimal->actionState = 7;
            } else {
                miscAnimal->actionState = 2;
            }

            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;

    }
    
}

void updateRanchHorse(u8 index) {
    MiscAnimal *miscAnimal = &gMiscAnimals[index];


    u16 temp;
    
    switch (miscAnimal->actionState) {

         case 0:

            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;
            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0);

             if (getRandomNumberInRange(0, 60) < 56) {
                miscAnimal->actionState = 0;
            } else {
                 
                temp = getRandomNumberInRange(0, 8);

                if (temp < 7) {
                    miscAnimal->direction = temp;
                }

                 miscAnimal->actionState = 1;
                 
            }

            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
        
            break;

        case 1:
            miscAnimal->zDisplacement = 1;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;
            setEntityDirectionalAnimation(miscAnimal->entityIndex, 8);
            
            if (getRandomNumberInRange(0, 19) < 4) {
                miscAnimal->actionState = 1;
            } else {
                miscAnimal->actionState = 0;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
        
            break;

     }
    
}

void updateRanchSheep(u8 index) {
    MiscAnimal *miscAnimal = &gMiscAnimals[index];


    u16 temp;
    u8 tempDirection;
    
    switch (miscAnimal->actionState) {

        case 0:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;
            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0);

            if (getRandomNumberInRange(0, 60) < 40) {
                miscAnimal->actionState = 0;
            } else {

                temp = getRandomNumberInRange(0, 20);

                if (temp < 7) {
                    miscAnimal->direction = temp;
                }

                temp = getRandomNumberInRange(0, 2);

                if (temp == 0) {
                    miscAnimal->actionState = 1;
                }
                
                if (temp == 1) {
                    miscAnimal->actionState = 2;
                }
                
                if (temp == 2) {
                    miscAnimal->actionState = 3;
                }

            }

            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
        
            break;

        case 1:
            miscAnimal->zDisplacement = 1;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;
            setEntityDirectionalAnimation(miscAnimal->entityIndex, 8);

            if (getRandomNumberInRange(0, 20) < 10) {
                miscAnimal->actionState = 1;
            } else {
                miscAnimal->actionState = 0;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
        
            break;

        case 2:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;
            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0x10);

            if (!getRandomNumberInRange(0, 2)) {
                miscAnimal->actionState = 2;
            } else {
                miscAnimal->actionState = 0;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;

        case 3:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;

            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0x18);

            miscAnimal->actionState = 4;
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;
        
        case 4:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;

            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0x20);

            temp = getRandomNumberInRange(0, 11);
            
            if (temp < 10) {
                miscAnimal->actionState = 4;
            }
            
            if (temp == 10) {
                miscAnimal->actionState = 4;
            }
            
            if (temp == 11) {
                miscAnimal->actionState = 6;
            }

            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;
        
        case 5:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;

            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0x28);

            if (getRandomNumberInRange(0, 4) < 4) {
                miscAnimal->actionState = 5;
            } else {
                miscAnimal->actionState = 4;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;
        
       case 6:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;

            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0x18);

            miscAnimal->actionState = 0;
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;     

    }
    
}

void updateRanchCow(u8 index) {
    MiscAnimal *miscAnimal = &gMiscAnimals[index];


    u16 temp;
    u8 tempDirection;
    
    switch (miscAnimal->actionState) {

        case 0:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;
            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0);

            if (getRandomNumberInRange(0, 60) < 40) {
                miscAnimal->actionState = 0;
            } else {

                temp = getRandomNumberInRange(0, 20);

                if (temp < 7) {
                    miscAnimal->direction = temp;
                }

                temp = getRandomNumberInRange(0, 5);

                if (temp == 0) {
                    miscAnimal->actionState = 1;
                }
                
                if (temp == 1) {
                    miscAnimal->actionState = 2;
                }
                
                if (temp == 2) {
                    miscAnimal->actionState = 3;
                }
                
                if (temp == 3) {
                    miscAnimal->actionState = 4;
                }

                if (temp == 4) {
                    miscAnimal->actionState = 5;
                }
                
                if (temp == 5) {
                    miscAnimal->actionState = 6;
                }
            }

            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
        
            break;

        case 1:
            miscAnimal->zDisplacement = 1;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;
            setEntityDirectionalAnimation(miscAnimal->entityIndex, 8);

            if (getRandomNumberInRange(0, 20) < 10) {
                miscAnimal->actionState = 1;
            } else {
                miscAnimal->actionState = 0;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
        
            break;

        case 2:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;
            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0x20);

            temp = getRandomNumberInRange(0, 2);
            
            if (temp == 0) {
                miscAnimal->actionState = 2;
            }
            
            if (temp == 1) {
                miscAnimal->actionState = 0;
            }
            
            if (temp == 2) {
                miscAnimal->actionState = 9;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;

        case 3:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;

            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0x10);

            miscAnimal->actionState = 7;
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;
        
        case 4:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;

            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0x38);

            miscAnimal->actionState = 0;
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;
        
        case 5:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;

            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0x40);

            miscAnimal->actionState = 0;
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;
        
       case 6:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;

            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0x48);

            miscAnimal->actionState = 0;
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;     

        case 7:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;

            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0x18);

            temp = getRandomNumberInRange(0, 11);
            
            if (temp < 10) {
                miscAnimal->actionState = 7;
            }
            
            if (temp == 10) {
                miscAnimal->actionState = 8;
            }
            
            if (temp == 11) {
                miscAnimal->actionState = 10;
            }

            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            
            break;

        case 8:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;

            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0x28);

            if (getRandomNumberInRange(0, 4) < 4) {
                 miscAnimal->actionState = 8;
            } else {
                 miscAnimal->actionState = 7;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;
       
        case 9:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;

            setEntityDirectionalAnimation(miscAnimal->entityIndex, 48);

            miscAnimal->actionState = 2;
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;

        case 10:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;

            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0x10);

            miscAnimal->actionState = 0;
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;

}
    
}

void updateFox(u8 index) {
    MiscAnimal *miscAnimal = &gMiscAnimals[index];

    
    u16 temp;
    u8 tempDirection;
    
    switch (miscAnimal->actionState) {

        case 0:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;
            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0);

            if (getRandomNumberInRange(0, 60) < 40) {
                
                miscAnimal->actionState = 0;
                
            } else {
                temp = getRandomNumberInRange(0, 10);

                if (temp < 7) {
                    miscAnimal->direction = temp;
                }

                temp = getRandomNumberInRange(0, 2);

                if (temp == 0) {
                    miscAnimal->actionState = 1;
                }
                
                if (temp == 1) {
                    miscAnimal->actionState = 2;
                }
                
                if (temp == 2) {
                    miscAnimal->actionState = 3;
                }
                
            }

            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
        
            break;

        case 1:
            miscAnimal->zDisplacement = 1;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;
            setEntityDirectionalAnimation(miscAnimal->entityIndex, 8);

            if (getRandomNumberInRange(0, 19) < 10) {
                miscAnimal->actionState = 1;
            } else {
                miscAnimal->actionState = 0;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
        
            break;

        case 2:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;
            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0x10);

            if (getRandomNumberInRange(0, 20) < 10) {
                miscAnimal->actionState = 2;
            } else {
                miscAnimal->actionState = 0;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;

        case 3:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;

            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0x18);
            
            if (getRandomNumberInRange(0, 19) < 10) {
                miscAnimal->actionState = 3;
            } else {
                miscAnimal->actionState = 0;
            }

            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;

        case 32:
            miscAnimal->zDisplacement = 4;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;

            setEntityDirectionalAnimation(miscAnimal->entityIndex, 8);

            tempDirection = calculateAnimalDirectionToPlayer(entities[miscAnimal->entityIndex].coordinates.x, entities[miscAnimal->entityIndex].coordinates.z, entities[ENTITY_PLAYER].coordinates.x, entities[ENTITY_PLAYER].coordinates.z);

            miscAnimal->direction = (tempDirection + 4) % 8;
            
            if (!getRandomNumberInRange(0, 30)) {
                miscAnimal->flags &= ~MISC_ANIMAL_RUNNING_AWAY;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            
            break;

    }
    
}

void updateBunny(u8 index) {
    MiscAnimal *miscAnimal = &gMiscAnimals[index];


    u16 temp;
    u8 tempDirection;
    
    switch (miscAnimal->actionState) {

        case 0:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;
            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0);

            if (getRandomNumberInRange(0, 60) < 40) {
                miscAnimal->actionState = 0;
            } else {

                temp = getRandomNumberInRange(0, 10);

                if (temp < 7) {
                    miscAnimal->direction = temp;
                }

                temp = getRandomNumberInRange(0, 2);

                if (temp == 0) {
                    miscAnimal->actionState = 1;
                }
                
                if (temp == 1) {
                    miscAnimal->actionState = 2;
                }
                
                if (temp == 2) {
                    miscAnimal->actionState = 3;
                }
                
            }

            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
        
            break;

        case 1:
            miscAnimal->zDisplacement = 1;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;
            setEntityDirectionalAnimation(miscAnimal->entityIndex, 8);

            if (getRandomNumberInRange(0, 19) < 10) {
                miscAnimal->actionState = 1;
            } else {
                miscAnimal->actionState = 0;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
        
            break;

        case 2:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;
            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0x10);

            if (getRandomNumberInRange(0, 20) < 10) {
                miscAnimal->actionState = 2;
            } else {
                miscAnimal->actionState = 0;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;

        case 3:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;

            if (getRandomNumberInRange(0, 1)) {
                setEntityAnimation(miscAnimal->entityIndex, 0x18);
            } else {
                setEntityAnimation(miscAnimal->entityIndex, 0x19);
            }

            miscAnimal->actionState = 0;
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;

        case 32:
            miscAnimal->zDisplacement = 4;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;

            setEntityDirectionalAnimation(miscAnimal->entityIndex, 8);

            tempDirection = calculateAnimalDirectionToPlayer(entities[miscAnimal->entityIndex].coordinates.x, entities[miscAnimal->entityIndex].coordinates.z, entities[ENTITY_PLAYER].coordinates.x, entities[ENTITY_PLAYER].coordinates.z);

            miscAnimal->direction = (tempDirection + 4) % 8;
            
            if (!getRandomNumberInRange(0, 30)) {
                miscAnimal->flags &= ~MISC_ANIMAL_RUNNING_AWAY;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            
            break;

    }
    
}

void updateSquirrel(u8 index) {
    MiscAnimal *miscAnimal = &gMiscAnimals[index];


    u16 temp;
    u8 tempDirection;
    
    switch (miscAnimal->actionState) {

        case 0:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;
            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0);

            if (getRandomNumberInRange(0, 60) < 40) {
                
                miscAnimal->actionState = 0;
                
            } else {
                
                temp = getRandomNumberInRange(0, 10);

                if (temp < 7) {
                    miscAnimal->direction = temp;
                }

                temp = getRandomNumberInRange(0, 1);

                if (temp == 0) {
                    miscAnimal->actionState = 1;
                }
                
                if (temp == 1) {
                    miscAnimal->actionState = 2;
                }
                
            } 
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
        
            break;

case 1:
            miscAnimal->zDisplacement = 1;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;
            setEntityDirectionalAnimation(miscAnimal->entityIndex, 8);

            if (getRandomNumberInRange(0, 19) < 10) {
                miscAnimal->actionState = 1;
            } else {
                miscAnimal->actionState = 0;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            
            break;
        
        case 2:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;
            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0x10);

            if (getRandomNumberInRange(0, 20) < 10) {
                miscAnimal->actionState = 2;
            } else {
                miscAnimal->actionState = 0;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;

case 16:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;
            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0);

            miscAnimal->actionState = 0x10;
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            
            break;

        case 17:
            setEntityCollidable(miscAnimal->entityIndex, 0);
            setEntityYMovement(miscAnimal->entityIndex, 0);
            setEntityShadow(miscAnimal->entityIndex, 0xFF);

            miscAnimal->zDisplacement = 0;
            miscAnimal->yDisplacement = 2;

            setEntityAnimation(miscAnimal->entityIndex, 0x19);

            miscAnimal->timer++;
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;

            if (miscAnimal->timer == 6) {
                deactivateEntity(miscAnimal->entityIndex);
                miscAnimal->flags = 0;
            }
            
            break;
        
       case 32:
            miscAnimal->zDisplacement = 4;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;
            setEntityDirectionalAnimation(miscAnimal->entityIndex, 8);
            
            tempDirection = calculateAnimalDirectionToPlayer(entities[miscAnimal->entityIndex].coordinates.x, entities[miscAnimal->entityIndex].coordinates.z, entities[ENTITY_PLAYER].coordinates.x, entities[ENTITY_PLAYER].coordinates.z);

            miscAnimal->direction = (tempDirection + 4) % 8;
            
            if (!getRandomNumberInRange(0, 30)) {
                miscAnimal->flags &= ~MISC_ANIMAL_RUNNING_AWAY;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;

    }
    
}

void updateMonkey(u8 index) {
    MiscAnimal *miscAnimal = &gMiscAnimals[index];


    u16 temp;
    u8 tempDirection;
    
    switch (miscAnimal->actionState) {

        case 0:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;
            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0);

            if (getRandomNumberInRange(0, 60) >= 40) {

                temp = getRandomNumberInRange(0, 10);

                if (temp < 7) {
                    miscAnimal->direction = temp;
                }

                temp = getRandomNumberInRange(0, 2);

                if (temp == 0) {
                    miscAnimal->actionState = 1;
                }
                
                if (temp == 1) {
                    miscAnimal->actionState = 2;
                }
                
                if (temp == 2) {
                    miscAnimal->actionState = 3;
                }
                
            } else {
                miscAnimal->actionState = 0;
            }

            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
        
            break;

case 1:
            miscAnimal->zDisplacement = 1;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;
            setEntityDirectionalAnimation(miscAnimal->entityIndex, 8);

            if (getRandomNumberInRange(0, 19) < 10) {
                miscAnimal->actionState = 1;
            } else {
                miscAnimal->actionState = 0;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;

case 2:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;
            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0x10);

            if (getRandomNumberInRange(0, 20) < 10) {
                miscAnimal->actionState = 2;
            } else {
                miscAnimal->actionState = 0;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            playSfx(0x4D);
            
            break;

case 3:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;
            setEntityAnimation(miscAnimal->entityIndex, 0x22);

            miscAnimal->actionState = 0;
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;
    
        case 16:
            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;
            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0);

            miscAnimal->actionState = 0x10;
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            
            break;

        case 17:

            setEntityCollidable(miscAnimal->entityIndex, 0);
            setEntityYMovement(miscAnimal->entityIndex, 0);
            setEntityShadow(miscAnimal->entityIndex, 0xFF);

            miscAnimal->zDisplacement = 0;
            miscAnimal->yDisplacement = 2;

            setEntityAnimation(miscAnimal->entityIndex, 0x1D);

            miscAnimal->timer++;
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;

            if (miscAnimal->timer == 6) {
                deactivateEntity(miscAnimal->entityIndex);
                miscAnimal->flags = 0;
            }
            
            break;
        
       case 32:
            miscAnimal->zDisplacement = 4;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;
            setEntityDirectionalAnimation(miscAnimal->entityIndex, 8);
            
            tempDirection = calculateAnimalDirectionToPlayer(entities[miscAnimal->entityIndex].coordinates.x, entities[miscAnimal->entityIndex].coordinates.z, entities[ENTITY_PLAYER].coordinates.x, entities[ENTITY_PLAYER].coordinates.z);

            miscAnimal->direction = (tempDirection + 4) % 8;
            
            if (!getRandomNumberInRange(0, 30)) {
                miscAnimal->flags &= ~MISC_ANIMAL_RUNNING_AWAY;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            break;

}
    
}

void updateSparrow(u8 index) {
    MiscAnimal *miscAnimal = &gMiscAnimals[index];


    u16 temp;
    
    switch (miscAnimal->actionState) {

        case 0:

            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;
            
            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0);
            
            if (getRandomNumberInRange(0, 60) < 40) {

                miscAnimal->actionState = 0;
            
            } else {
                
                temp = getRandomNumberInRange(0, 10);
                
                if (temp < 7) {
                    miscAnimal->direction = temp;
                }

                if (getRandomNumberInRange(0, 30) >= 28) {
                    miscAnimal->actionState = 32;
                } else {
                    miscAnimal->actionState = 1;
                }
                
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;

            break;

        case 1:

            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;

            temp = getRandomNumberInRange(0, 3); 

            switch (temp) {

                case 0:
                    setEntityAnimation(miscAnimal->entityIndex, 0x10);
                    break;
                case 1:
                    setEntityAnimation(miscAnimal->entityIndex, 17);
                    break;
                case 2:
                    setEntityAnimation(miscAnimal->entityIndex, 0x12);
                    break;
                
                case 3:
                    setEntityAnimation(miscAnimal->entityIndex, 0x13);
                    break;
            } 
                
            if (getRandomNumberInRange(0, 60) < 40) {
                miscAnimal->actionState = 1;
            } else {
                miscAnimal->actionState = 0;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;

            playSfx(0x46);
            
            break;

        case 32:

            setEntityCollidable(miscAnimal->entityIndex, FALSE);
            setEntityTracksCollisions(miscAnimal->entityIndex, FALSE);
            enableEntityMovement(miscAnimal->entityIndex, FALSE);
            setEntityYMovement(miscAnimal->entityIndex, FALSE);
            setEntityShadow(miscAnimal->entityIndex, 0xFF);
            
            entities[miscAnimal->entityIndex].yOffset = 0;
            
            setEntityDirectionalAnimation(miscAnimal->entityIndex, 8);
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;

            if (miscAnimal->timer < 129) {
                miscAnimal->zDisplacement = 4;
                miscAnimal->yDisplacement = 2;
                miscAnimal->timer++;
            } else {
                deactivateEntity(miscAnimal->entityIndex);
                miscAnimal->flags = 0;
            }
            
    }
    
}

void updateBird(u8 index) {
    MiscAnimal *miscAnimal = &gMiscAnimals[index];

    
    u16 temp;
    
    switch (miscAnimal->actionState) {

        case 0:

            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;
            
            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0);
            
            if (getRandomNumberInRange(0, 60) < 40) {

                miscAnimal->actionState = 0;
            
            } else {
                
                temp = getRandomNumberInRange(0, 10);
                
                if (temp < 7) {
                    miscAnimal->direction = temp;
                }

                if (getRandomNumberInRange(0, 30) >= 28) {
                    miscAnimal->actionState = 32;
                } else {
                    miscAnimal->actionState = 1;
                }
                
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;

            break;

        case 1:

            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;

            temp = getRandomNumberInRange(0, 3); 

            switch (temp) {

                case 0:
                    setEntityAnimation(miscAnimal->entityIndex, 0x10);
                    break;
                case 1:
                    setEntityAnimation(miscAnimal->entityIndex, 17);
                    break;
                case 2:
                    setEntityAnimation(miscAnimal->entityIndex, 0x12);
                    break;
                
                case 3:
                    setEntityAnimation(miscAnimal->entityIndex, 0x13);
                    break;
            } 
                
            if (getRandomNumberInRange(0, 60) < 40) {
                miscAnimal->actionState = 1;
            } else {
                miscAnimal->actionState = 0;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;

            if (!getRandomNumberInRange(0, 1)) {
                playSfx(0x48);
            } else {
                playSfx(0x49);
            }
            
            break;

        case 32:

            setEntityCollidable(miscAnimal->entityIndex, FALSE);
            setEntityTracksCollisions(miscAnimal->entityIndex, FALSE);
            enableEntityMovement(miscAnimal->entityIndex, FALSE);
            setEntityYMovement(miscAnimal->entityIndex, FALSE);
            setEntityShadow(miscAnimal->entityIndex, 0xFF);
            entities[miscAnimal->entityIndex].yOffset = 0;
            
            setEntityDirectionalAnimation(miscAnimal->entityIndex, 8);

            miscAnimal->zDisplacement = 4;
            miscAnimal->yDisplacement = 2;
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            miscAnimal->timer++;
                    
            break;

    }
    
}

void updateCrab(u8 index) {
    MiscAnimal *miscAnimal = &gMiscAnimals[index];

    
    u32 temp;
    u16 temp2;
    
    switch (miscAnimal->actionState) {

        case 0:

            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;
            
            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0);
            
            if (getRandomNumberInRange(0, 60) >= 40) {

                temp = getRandomNumberInRange(0, 10);

                if (temp < 7) {
                    // ???
                    temp &= ~3;
                    temp2 = temp;
                    miscAnimal->direction = temp2;
                } 
                
                miscAnimal->actionState = 1;
            
            } else {
                miscAnimal->actionState = 0;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;

            break;

        case 1:

            miscAnimal->zDisplacement = 1;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;
            
            setEntityDirectionalAnimation(miscAnimal->entityIndex, 8);

            if (getRandomNumberInRange(0, 19) < 10) {
                miscAnimal->actionState = 1;
            } else {
                miscAnimal->actionState = 0;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;

            break;

        case 32:

            setEntityCollidable(miscAnimal->entityIndex, 0);
            setEntityYMovement(miscAnimal->entityIndex, 0);
            miscAnimal->direction = DIRECTION_E;
            setEntityDirectionalAnimation(miscAnimal->entityIndex, 8);
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;

            if (miscAnimal->timer < 17) {
                miscAnimal->zDisplacement = 4;
                miscAnimal->yDisplacement = 0;
                miscAnimal->timer++;
            } else {
                deactivateEntity(miscAnimal->entityIndex);
                miscAnimal->flags = 0;
            }
                    
            break;
            
    }
}

void updateSnake(u8 index) {
    MiscAnimal *miscAnimal = &gMiscAnimals[index];


    u16 temp;
    
    switch (miscAnimal->actionState) {

        case 0:

            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;
            
            setEntityDirectionalAnimation(miscAnimal->entityIndex, 0);
            
            if (getRandomNumberInRange(0, 60) >= 40) {
                
                temp = getRandomNumberInRange(0, 10);
                
                if (temp < 7) {
                    miscAnimal->direction = temp;
                }
                
                miscAnimal->actionState = 1;
            
            } else {
                miscAnimal->actionState = 0;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;

            break;

        case 1:

            miscAnimal->zDisplacement = 1;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;
            
            setEntityDirectionalAnimation(miscAnimal->entityIndex, 8);

            if (getRandomNumberInRange(0, 19) < 10) {
                miscAnimal->actionState = 1;
            } else {
                miscAnimal->actionState = 0;
            }
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;

            break;

        case 32:

            setEntityDirectionalAnimation(miscAnimal->entityIndex, 8);
            
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;

            if (miscAnimal->timer < 33) {
                miscAnimal->zDisplacement = 4;
                miscAnimal->yDisplacement = 0;
                miscAnimal->timer++;
            } else {
                deactivateEntity(miscAnimal->entityIndex);
                miscAnimal->flags = 0;
            }
                    
            break;
            
    }
    
}

void updateWhiteButterfly(u8 index) {
    MiscAnimal *miscAnimal = &gMiscAnimals[index];


    switch (miscAnimal->actionState) {

        case 0:
        case 1:

            setEntityCollidable(miscAnimal->entityIndex, FALSE);
            setEntityTracksCollisions(miscAnimal->entityIndex, FALSE);
    
            entities[miscAnimal->entityIndex].yOffset = 0x20;
    
            miscAnimal->zDisplacement = 1;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;
    
            setEntityAnimation(miscAnimal->entityIndex, 2);
    
            miscAnimal->direction = getRandomNumberInRange(0, 7);
            miscAnimal->actionState = 0;
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            
            break;
        
        default:
            break;
        
    }
    
}

void updateLadybug(u8 index) {
    MiscAnimal *miscAnimal = &gMiscAnimals[index];


    switch (miscAnimal->actionState) {

        case 0:
        case 1:

            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;
    
            setEntityAnimation(miscAnimal->entityIndex, 0);

            miscAnimal->actionState = 0;
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;

            break;
        
        default:
            break;
    
    }
    
}

void updateCicada(u8 index) {
    MiscAnimal *miscAnimal = &gMiscAnimals[index];


    switch (miscAnimal->actionState) {

        case 0:

            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;

            setEntityAnimation(miscAnimal->entityIndex, 0);

            entities[miscAnimal->entityIndex].yOffset = 64;

            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;

            if (!(5 < gHour && gHour < 16)) {
                playSfx(0x51);
            } else {
                playSfx(0x4E);
            }
            
            break;
        
        case 1:

            setEntityCollidable(miscAnimal->entityIndex, FALSE);
            setEntityTracksCollisions(miscAnimal->entityIndex, FALSE);
            enableEntityMovement(miscAnimal->entityIndex, FALSE);
            setEntityYMovement(miscAnimal->entityIndex, FALSE);
            setEntityShadow(miscAnimal->entityIndex, 0xFF);

            entities[miscAnimal->entityIndex].yOffset = 0;

            setEntityAnimation(miscAnimal->entityIndex, 2);

            miscAnimal->zDisplacement = 4;
            miscAnimal->yDisplacement = 2;

            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;

            miscAnimal->timer++;
            
            break;    
        
    }
    
}

void updateHornedBeetle(u8 index) {
    MiscAnimal *miscAnimal = &gMiscAnimals[index];


     switch (miscAnimal->actionState) {

        case 0:

            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;

            setEntityAnimation(miscAnimal->entityIndex, 0);

            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;

            break;
        
        case 1:

            setEntityCollidable(miscAnimal->entityIndex, FALSE);
            setEntityTracksCollisions(miscAnimal->entityIndex, FALSE);
            enableEntityMovement(miscAnimal->entityIndex, FALSE);
            setEntityYMovement(miscAnimal->entityIndex, FALSE);
            setEntityShadow(miscAnimal->entityIndex, 0xFF);

            entities[miscAnimal->entityIndex].yOffset = 0;

            setEntityAnimation(miscAnimal->entityIndex, 4);

            miscAnimal->zDisplacement = 4;
            miscAnimal->yDisplacement = 2;

            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;

            miscAnimal->timer++;
            
            break;    
        
    }
    
}

void updateStagBeetle(u8 index) {
    MiscAnimal *miscAnimal = &gMiscAnimals[index];


    switch (miscAnimal->actionState) {

        case 0:

            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;

            setEntityAnimation(miscAnimal->entityIndex, 0);

            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;

            break;
        
        case 1:

            setEntityCollidable(miscAnimal->entityIndex, FALSE);
            setEntityTracksCollisions(miscAnimal->entityIndex, FALSE);
            enableEntityMovement(miscAnimal->entityIndex, FALSE);
            setEntityYMovement(miscAnimal->entityIndex, FALSE);
            setEntityShadow(miscAnimal->entityIndex, 0xFF);

            entities[miscAnimal->entityIndex].yOffset = 0;

            setEntityAnimation(miscAnimal->entityIndex, 4);

            miscAnimal->zDisplacement = 4;
            miscAnimal->yDisplacement = 2;

            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;

            miscAnimal->timer++;
            
            break;    
        
    }
    
}

void updateDragonfly(u8 index) {
    MiscAnimal *miscAnimal = &gMiscAnimals[index];


    u16 temp;
    
    switch (miscAnimal->actionState) {

        case 0:
        case 1:

            setEntityCollidable(miscAnimal->entityIndex, FALSE);
            setEntityTracksCollisions(miscAnimal->entityIndex, FALSE);
                        
            entities[miscAnimal->entityIndex].yOffset = 0x20;

            miscAnimal->zDisplacement = 1;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;

            setEntityAnimation(miscAnimal->entityIndex, 2);

            temp = getRandomNumberInRange(0, 20);

            if (temp < 7) {
                miscAnimal->direction = temp;
            }
            
            miscAnimal->actionState = 0;
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;
            
            break;    
        
    }
    
}

void updateCricket(u8 index) {
    MiscAnimal *miscAnimal = &gMiscAnimals[index];

    
    switch (miscAnimal->actionState) {

        case 0:

            miscAnimal->zDisplacement = 0;
            miscAnimal->timer = 0;
            miscAnimal->unk_14 = 0;

            setEntityAnimation(miscAnimal->entityIndex, 0);

            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;

            break;
        
        case 1:

            setEntityAnimation(miscAnimal->entityIndex, 2);
            miscAnimal->flags |= MISC_ANIMAL_STATE_CHANGED;

            if (miscAnimal->timer < 33) {
                miscAnimal->zDisplacement = 2;
                miscAnimal->yDisplacement = 0;
                miscAnimal->timer++;
            } else {
                deactivateEntity(miscAnimal->entityIndex);
                miscAnimal->flags = 0;
            }
            
            break;    
        
    }
    
}

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

void handleDogPlayerCollision(void) {
    Dog *dog = &dogInfo;


    checkEntityProximity(ENTITY_PLAYER, 0.0f, 8.0f, 0);
    
    if ((dog->flags & DOG_ENTITY_LOADED) && (entities[dog->entityIndex].entityCollidedWithIndex == ENTITY_PLAYER)) {
        dog->actionState = 0x20;
        dog->flags |= DOG_COLLISION_WITH_PLAYER;
    }
    
}

bool handleHorsePlayerInteraction(void) {
    Horse *horse = &horseInfo;

    
    bool result;

    checkEntityProximity(ENTITY_PLAYER, 0.0f, 8.0f, 0);
    
    result = FALSE;

    if ((horse->flags & HORSE_ENTITY_LOADED) && (entities[horse->entityIndex].entityCollidedWithIndex == ENTITY_PLAYER)) {
        result = TRUE;
        horse->actionState = 16;
        horse->flags |= HORSE_COLLISION_WITH_PLAYER;
    }
    
    return result;
    
}

bool handleHorseGrownPlayerInteraction(void) {
    Horse *horse = &horseInfo;

    
    bool result = FALSE;

    if ((horse->flags & HORSE_ENTITY_LOADED) && (horse->grown == TRUE) && (entities[horse->entityIndex].entityCollidedWithIndex == ENTITY_PLAYER)) {
        horse->actionState = 16;
        horse->flags |=  HORSE_COLLISION_WITH_PLAYER;
        result = TRUE;
    }
    
    return result;

}

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
                        setAnimalState(2, i, 0xFF, 0xFF, 17);
                        gFarmAnimals[i].flags |= (FARM_ANIMAL_BRUSHED | FARM_ANIMAL_COLLISION_WITH_PLAYER);
                        showAnimalExpressionBubble(COW_TYPE, i, 3);
                        set = TRUE;
                        break;

                    case BABY_SHEEP ... SHEARED_SHEEP:
                        adjustFarmAnimalAffection(i, 2);
                        setAnimalState(2, i, 0xFF, 0xFF, 17);
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

bool handleBrushHorse(void) {
    Horse *horse = &horseInfo;


    bool result = FALSE;
    
    if (horse->flags & HORSE_ENTITY_LOADED) {

        if (horse->actionState == 16) {
            
            horse->actionState = 0;
            
            if (!(horse->flags & HORSE_BRUSHED_DAILY)) {
                
                horse->actionState = 17;
                
                adjustHorseAffection(2);
            
                horse->flags |= (HORSE_COLLISION_WITH_PLAYER | HORSE_BRUSHED_DAILY);
                showAnimalExpressionBubble(HORSE_TYPE, 0, 3);
                result = TRUE;
            
            }
                    
        }
    }
    
    return result;
    
}

bool handleHorseShippingItem(void) {
    Horse *horse = &horseInfo;


    bool result = FALSE;
    
    if ((horse->flags & HORSE_ENTITY_LOADED) && (horse->actionState == 16)) {
        
        horse->actionState = 0;
        horse->flags |= HORSE_COLLISION_WITH_PLAYER;
        showAnimalExpressionBubble(HORSE_TYPE, 0, 1);
        
        result = TRUE;
    
    }

    return result;
    
}

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
                            setAnimalState(2, i, 0xFF, 0xFF, 17);
                            gPlayer.heldItem = getMilkHeldItemIndex(i, 0);
                            gFarmAnimals[i].flags |= (FARM_ANIMAL_MILKED | FARM_ANIMAL_COLLISION_WITH_PLAYER);
                            showAnimalExpressionBubble(COW_TYPE, i, 3);
                            set = TRUE;
                        }
                        
                        break;
                    
                    case PREGNANT_COW:

                        adjustFarmAnimalAffection(i, 1);
                        setAnimalState(2, i, 0xFF, 0xFF, 17);

                        gPlayer.heldItem = getMilkHeldItemIndex(i, 0);

                        if (gPlayer.heldItem != SMALL_MILK_HELD_ITEM) {
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

u8 handleUseMiraclePotion(void) {

    u8 i = 0;
    bool set = FALSE;

    if (getTotalFarmAnimalsCount() < 8) {
        
        while (i < MAX_FARM_ANIMALS && !set) {
    
            if ((gFarmAnimals[i].flags & FARM_ANIMAL_ENTITY_LOADED) && gFarmAnimals[i].actionState == 16) {
    
                setAnimalState(2, i, 0xFF, 0xFF, 0);
                
                if (!(gFarmAnimals[i].flags & FARM_ANIMAL_PREGNANT) && gFarmAnimals[i].type == ADULT_COW) {
                
                    adjustFarmAnimalAffection(i, 10);
                    setAnimalState(2, i, 0xFF, COW_NORMAL, 17);
                    
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

bool handleGetMilkWithBottle(void) {

    u8 i = 0;
    bool set = FALSE;

    while (i < MAX_FARM_ANIMALS && !set) {

        if ((gFarmAnimals[i].flags & FARM_ANIMAL_ENTITY_LOADED) && gFarmAnimals[i].actionState == 16) {

            setAnimalState(2, i, 0xFF, 0xFF, 0);

            switch (gFarmAnimals[i].type) {

                case ADULT_COW:
                    
                    if (gFarmAnimals[i].condition < COW_MAD) {
                        setAnimalState(2, i, 0xFF, 0xFF, 17);
                        set = TRUE;
                        gFarmAnimals[i].flags |= FARM_ANIMAL_COLLISION_WITH_PLAYER;
                        gPlayer.bottleContents = BOTTLE_CONTENTS_MILK;
                    }

                    break;

                case PREGNANT_COW:

                    setAnimalState(2, i, 0xFF, 0xFF, 17);
                    set = TRUE;
                    gFarmAnimals[i].flags |= FARM_ANIMAL_COLLISION_WITH_PLAYER;
                    gPlayer.bottleContents = BOTTLE_CONTENTS_MILK;

                    break;

            }

        }

        i++;
        
    }

    return set;
    
}

void handleShearSheep(void) {

    u8 i = 0;
    bool set = FALSE;

    while (i < MAX_FARM_ANIMALS && !set) {

        if ((gFarmAnimals[i].flags & FARM_ANIMAL_ENTITY_LOADED) && gFarmAnimals[i].actionState == 16) {

            setAnimalState(2, i, 0xFF, 0xFF, 0);

            if (!(gFarmAnimals[i].flags & FARM_ANIMAL_SHEARED) && gFarmAnimals[i].type == ADULT_SHEEP) {
                
                adjustFarmAnimalAffection(i, 2);
                setAnimalState(2, i, SHEARED_SHEEP, 0xFF, 17);
                
                gPlayer.heldItem = getWoolHeldItemIndex(i, 0);
                gFarmAnimals[i].flags |= FARM_ANIMAL_SHEARED;
                
                showAnimalExpressionBubble(COW_TYPE, i, 3);
                set = TRUE;
                
            }

        }

        i++;
        
    } 

}

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
                            setAnimalState(2, i, 0xFF, 2, 0xFFU);
                            adjustFarmAnimalAffection(i, -20);
                            gHappiness += adjustValue(gHappiness, -5, 0xFF);
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

bool handleHitChickenWithTool(void) {

    u8 i = 0;
    bool set = FALSE;

    while (i < MAX_CHICKENS && !set) {

        if ((gChickens[i].flags & CHICKEN_ENTITY_LOADED) && gChickens[i].actionState == 16) {

            setAnimalState(1, i, 0xFF, 0xFF, 0);

             switch (gChickens[i].type) { 

                 case ADULT_CHICKEN:
                    setAnimalState(1, i, 0xFF, 0xFF, 17);
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

bool handleHitDogWithTool(void) {
    Dog *dog = &dogInfo;


    bool result = FALSE;
    
    if (dog->flags & DOG_ENTITY_LOADED) {

        if (dog->actionState == 0x20) {

            dog->actionState = 34;

            adjustDogAffection(-8);

            dog->flags |= DOG_COLLISION_WITH_PLAYER;
            showAnimalExpressionBubble(DOG_TYPE, 0, 0);
            result = TRUE;
            
        }
        
    }

    return result;
    
}

bool handleHitHorseWithTool(void) {
    Horse *horse = &horseInfo;

    
    bool result = FALSE;
    
    if (horse->flags & HORSE_ENTITY_LOADED) {

        if (horse->actionState == 16) {

            horse->actionState = 18;

            adjustHorseAffection(-8);

            horse->flags |= HORSE_COLLISION_WITH_PLAYER;
            showAnimalExpressionBubble(HORSE_TYPE, 0, 0);
            result = TRUE;
            
        }
        
    }

    return result;
    
}

void handleWhistleForDog(void) {
    Dog *dog = &dogInfo;


    if (dog->flags & DOG_ENTITY_LOADED) {
        dog->flags |= DOG_FOLLOWING;
        dog->actionState = 0;
        playSfx(DOG_BARK);
    }

    if (!(dog->flags & DOG_WHISTLED_FOR_DAILY)) {
        adjustDogAffection(1);
        dog->flags |= DOG_WHISTLED_FOR_DAILY;
    }
    
}

void handleWhistleForHorse(void) {
    Horse *horse = &horseInfo;

    
    if (horse->flags & HORSE_ENTITY_LOADED) {
        horse->flags |= HORSE_FOLLOWING;
        horse->actionState = 0;
        playSfx(HORSE_NEIGH);
    }
    
    if (!(horse->flags & HORSE_WHISTLED_DAILY)) {
        adjustHorseAffection(1);
        horse->flags |= HORSE_WHISTLED_DAILY;
    }
    
}

void handleCallFarmAnimalsWithCowBell(void) {

    u8 i;

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {

        if (gFarmAnimals[i].flags & FARM_ANIMAL_ENTITY_LOADED) {
            gFarmAnimals[i].flags |= (FARM_ANIMAL_APPROACHING | FARM_ANIMAL_FOLLOWING);
            gFarmAnimals[i].actionState = 0;
        }
        
    }
    
}

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

u8 getTotalCowsCount(void) {

    u8 i;
    u8 count = 0;

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {
        
        if (gFarmAnimals[i].flags & FARM_ANIMAL_ACTIVE) {

            switch (gFarmAnimals[i].type) {
                case BABY_COW:
                case CALF:
                case ADULT_COW:
                case PREGNANT_COW:
                    count++;
                    break;
            }

        }
    }

    return count;

}

u8 getCowWithHighestAffection(void) {

    u8 i;
    u8 found = 0xFF;
    u8 temp = 0;

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {
        FarmAnimal *farmAnimal = &gFarmAnimals[i];

        
        if ((farmAnimal->flags & FARM_ANIMAL_ACTIVE) && farmAnimal->type == ADULT_COW) {

            if (farmAnimal->affection >= temp) {
                found = i;
                temp = farmAnimal->affection;
            }
    
        }
    }
    
    return found;
    
}

u8 getTotalSheepCount(void) {

    u8 i;
    u8 count = 0;

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {
        FarmAnimal *farmAnimal = &gFarmAnimals[i];

        
        if (farmAnimal->flags & FARM_ANIMAL_ACTIVE) {
            
            if (PREGNANT_COW < farmAnimal->type && farmAnimal->type < SHEARED_SHEEP || farmAnimal->type == SHEARED_SHEEP) {
                count++;
            }
    
        }
    }
    
    return count;
    
}

u8 getTotalChickenCount(void) {

    u8 i;
    u8 count = 0;

    for (i = 0; i < MAX_CHICKENS; i++) {
        Chicken *chicken = &gChickens[i];


        if (chicken->flags & CHICKEN_ACTIVE) {
            
            if (0 < chicken->type && chicken->type < 3) {
                count++;
            }

            if (chicken->type == CHICKEN_EGG && (chicken->flags & CHICKEN_EGG_INCUBATING)) {
                count++;
            }
            
        }  

    }
    
    return count;
    
}

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

u8 func_8009B828(u8 arg0) {

    u8 i;
    u8 count = 0;
    
    for (i = 0; i < MAX_FARM_ANIMALS; i++) {
        FarmAnimal *farmAnimal = &gFarmAnimals[i];


        if (farmAnimal->flags & FARM_ANIMAL_ACTIVE) {

            switch (farmAnimal->type) {
                case BABY_COW:
                case CALF:
                case ADULT_COW:
                case PREGNANT_COW:
                    if (farmAnimal->location == FARM && (arg0 == 0 || farmAnimal->milkType == 0)) {
                        count++;
                    }
                    break;
            }

        }

    }

    if (count) {
        count = getFarmGrassTilesSum() != 0;
    }

    return count;
    
}

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

    convertNumberToGameVariableString(18, gAnimalSalePrice, 0);
    
}

void generateMilkTypeString(u8 index) {
    FarmAnimal *farmAnimal = &gFarmAnimals[index];


    u16 temp;

    if (CALF < farmAnimal->type && farmAnimal->type < BABY_SHEEP) {

        if (farmAnimal->milkType == 0) {
            
            temp = goldenMilkInfo[0];
            
        } else {

            if (farmAnimal->affection < 151) {
                temp = smallMilkInfo[0];
            }
            
            if (150 < farmAnimal->affection && farmAnimal->affection < 221) {
                temp = mediumMilkInfo[0];
            }

            if (farmAnimal->affection >= 221) {
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

u8 getBestCowMilkType(void) {

    u8 temp;
    u8 found = 0xFF;
    u8 i = 0;

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {

        if (gFarmAnimals[i].flags & FARM_ANIMAL_ACTIVE) {

            switch (gFarmAnimals[i].type) {
                case BABY_COW:
                case CALF:
                case ADULT_COW:
                case PREGNANT_COW:
                    temp = gFarmAnimals[i].milkType;

                    if (temp < found) {
                        found = temp;
                    }
                    break;
            }

        }
        
    }

    return found;
    
}

u8 getDogBestRacePlacement(void) {
    return dogInfo.bestRacePlacement;
}

u8 getHorseBestRacePlacement(void) {
    return horseInfo.bestRacePlacement;
}

void randomizeMiscAnimalSpawnVariants(void) {

    u8 i;

    for (i = 0; i < MAX_MISC_ANIMALS; i++) {
        gMiscAnimals[i].spawnVariant = getRandomNumberInRange(0, 3);
    }
    
}

void showAnimalExpressionBubble(u8 animalType, u8 animalIndex, u8 arg2) {

    deactivateEntity(48);
    loadEntity(48, ENTITY_ASSET_EXPRESSION_BUBBLES, TRUE);
    setCameraTrackingEntity(48, FALSE);

    switch (animalType) {

        case DOG_TYPE:
            setEntityTrackingTarget(48, dogInfo.entityIndex, 0xFF);
            break;

        case HORSE_TYPE:
            setEntityTrackingTarget(48, horseInfo.entityIndex, 0xFF);
            break;

        case COW_TYPE:
            setEntityTrackingTarget(48, gFarmAnimals[animalIndex].entityIndex, 0xFF);
            break;
        
        case SHEEP_TYPE:
            setEntityTrackingTarget(48, gFarmAnimals[animalIndex].entityIndex, 0xFF);
            break;

        case CHICKEN_TYPE:
            setEntityTrackingTarget(48, gChickens[animalIndex].entityIndex, 0xFF);
            break;

        default:
            break;
        
    }

    setEntityCollidable(48, FALSE);
    setEntityYMovement(48, FALSE);
    setEntityTracksCollisions(48, FALSE);
    enableEntityMovement(48, FALSE);

    setEntityColor(48, mainMap[MAIN_MAP_INDEX].mapGlobals.currentRGBA.r, mainMap[MAIN_MAP_INDEX].mapGlobals.currentRGBA.g, mainMap[MAIN_MAP_INDEX].mapGlobals.currentRGBA.b, mainMap[MAIN_MAP_INDEX].mapGlobals.currentRGBA.a);
    setEntityAttachmentOffset(48, 0, 48, 0);
    setEntityAnimation(48, arg2);
    
}