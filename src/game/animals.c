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
#include "game/itemHandlers.h"
#include "game/level.h"
#include "game/player.h"
#include "game/weather.h"

#include "mainproc.h"

#include "game/spriteIndices.h"

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

extern u8 bornChickenIndex;
extern u8 bornAnimalIndex;
extern u8 D_80189054;
// price
extern u32 D_801890E0;

// game variable strings
extern u8 D_8016FBCC[2];
extern u8 D_801886D4[6];
extern u8 deadAnimalName[6];

extern u8 bornChickenIndex;
// newest farm animal index?
extern u8 bornAnimalIndex;
// cow festival stall
extern u8 D_80189054;
// newest animal index (generic)?
extern u8 D_801FC155;
// dead animal count
// TODO: label
extern u16 D_8013DC2E;
// purchased animal type
extern u8 D_801C4216;


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

Vec3f D_80114AB0 = { 208.0f, 0.0f, -208.0f };

u16 D_80114ABC[4] = { SMALL_MILK, 100, 6500, 10 };
u16 D_80114AC4[4] = { MEDIUM_MILK, 150, 7000, 20 };
u16 D_80114ACC[4] = { LARGE_MILK, 300,  7500, 30 };
u16 D_80114AD4[4] = { GOLDEN_MILK, 500, 8500, 50 };

SheepItemInfo D_80114ADC = {
    { WOOL, 900, 4200 },
    { WOOL, 900, 4600 },
    { HIGH_QUALITY_WOOL, 1800, 5000}
};

// forward declarations
void func_800861A4(u8, u8, u8, u8, u8);               
void func_80086458(u8, s8); 
u8 initializeNewFarmAnimal(u8 arg0, u8 arg1);
void setFarmAnimalLocation(u8); 
void updateDog();                                  
void updateHorse();                                  
void updateHorseGrown();                                 
void updateHorseNotGrown();       
u8 func_8008A4A8(u8 index, u8 direction, f32 x, f32 y, f32 z);          
void func_8008A5F0();
void func_8008A650(u8);                               
void func_8008A9E8(u8);                               
void func_8008B150();       
void func_8008B1B8();                                  
void func_8008B2E8(u8);                               
void func_8008B55C(u8);                               
void func_8008B9AC();                                  
void func_8008BAF0(u8, u8); 
void func_8008CF94(u8);                               
void func_8008DAA0(u8);                               
void func_8009476C(u8);     
void func_80094A5C(u8);                               
void func_800958EC(u8);                               
void func_80096264(u8);                               
void func_800967E0(u8);                               
void func_80096978(u8);                               
void func_80096DFC(u8);                               
void func_800973F8(u8);                               
void func_80097804(u8);                               
void func_80097CB0(u8);                               
void func_80098188(u8);                               
void func_80098740(u8);                               
void func_80098B3C(u8);                               
void func_80098F24(u8);                               
void func_800991C0(u8);                     
void func_80099424(u8);                               
void func_80099548(u8);                               
void func_800995F8(u8);                               
void func_80099804(u8);                               
void func_800999B0(u8);                               
void func_8009B914(void);              
void func_80099B5C(u8);                 
void func_80099C94(u8);                         
void func_8009BCC4(u8, u8, u8);


//INCLUDE_ASM("asm/nonmatchings/game/animals", deactivateAnimalEntities);

void deactivateAnimalEntities(void) {

    u8 i;

    if (dogInfo.flags & 4) {
        deactivateEntity(dogInfo.entityIndex);
    }

    dogInfo.unk_17 = 0;
    dogInfo.unk_1A = 0;
    dogInfo.flags &= ~4;

    for (i = 0; i < MAX_CHICKENS; i++) {

        if (gChickens[i].flags & 4) {
            deactivateEntity(gChickens[i].entityIndex);
        }

        gChickens[i].unk_17 = 0;
        gChickens[i].unk_1A = 0;
        gChickens[i].flags &= ~4;
        
    }

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {

        if (gFarmAnimals[i].flags & 4) {
            deactivateEntity(gFarmAnimals[i].entityIndex);
        }

        gFarmAnimals[i].unk_1B = 0;
        gFarmAnimals[i].unk_1D = 0;
        gFarmAnimals[i].flags &= ~4;
        
    }

    if (horseInfo.flags & 4) {
        deactivateEntity(horseInfo.entityIndex);
    }

    horseInfo.unk_17 = 0;
    horseInfo.unk_1A = 0;
    horseInfo.flags &= ~4;

    for (i = 0; i < MAX_MISC_ANIMALS; i++) {
        
        if (gMiscAnimals[i].flags & 4) {
            deactivateEntity(gMiscAnimals[i].entityIndex);
        }
        
        gMiscAnimals[i].flags = 0;
        gMiscAnimals[i].unk_F = 0;
        gMiscAnimals[i].unk_13 = 0;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_800861A4);

void func_800861A4(u8 animalType, u8 index, u8 type, u8 condition, u8 arg4) {

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
            
            if (arg4 != 0xFF) {
                gChickens[index].unk_17 = arg4;
            }

            gChickens[index].unk_19 = 0;
            gChickens[index].unk_1A = 0;
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
            
            if (arg4 != 0xFF) {
                gFarmAnimals[index].unk_1B = arg4;
            }

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;
        
            break;
        
        default:
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008634C);

void func_8008634C(s8 amount) {

    u8 i;

    adjustDogAffection(amount);
    adjustHorseAffection(amount);

    for (i = 0; i < 8; i++) {
        func_80086458(i, amount);
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", adjustDogAffection);

inline void adjustDogAffection(s8 amount) {
    if (dogInfo.flags & 1) {
        dogInfo.affection += adjustValue(dogInfo.affection, amount, 0xFF);
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", adjustHorseAffection);

inline void adjustHorseAffection(s8 amount) {
    if (horseInfo.flags & 1) {
        horseInfo.affection += adjustValue(horseInfo.affection, amount, 0xFF);
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80086458);

void func_80086458(u8 animalIndex, s8 amount) {
    
    s8 adjusted;
    
    if (gFarmAnimals[animalIndex].flags & 1) {
    
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

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008662C);

inline u16 func_8008662C(u8 animalIndex, u8 arg1) {

    u16 res;
    
    if (!gFarmAnimals[animalIndex].normalMilk) {

        res = D_80114AD4[arg1];
        
    } else {

        if (gFarmAnimals[animalIndex].affection <= 150) {
            res = D_80114ABC[arg1];
        }

        if (150 < gFarmAnimals[animalIndex].affection && gFarmAnimals[animalIndex].affection < 221) {
            res = D_80114AC4[arg1];
        }

        if (gFarmAnimals[animalIndex].affection >= 221) {
            res = D_80114ACC[arg1];
        }
        
    }

    return res;

}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_800866E0);

inline u16 func_800866E0(u8 animalIndex, u8 arg1) {

    u16 res;

    if (gFarmAnimals[animalIndex].affection < 100) {
        res = D_80114ADC.arr[arg1];
    }

    if (99 < gFarmAnimals[animalIndex].affection && gFarmAnimals[animalIndex].affection < 200) {
        res = D_80114ADC.arr2[arg1];
    }

    if (gFarmAnimals[animalIndex].affection >= 200) {
        res = D_80114ADC.arr3[arg1];
    }
    
    return res;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80086764);

// player - animal interaction handler
bool func_80086764(void) {

    Vec3f vec;
    
    bool set = FALSE;
    u8 i;
    
    u16 textIndex;
    u16 tempFlags;
    u8 groundObjectIndex;

    if (!(gPlayer.flags & 1)) {

        if (gPlayer.heldItem == 0) {

            if ((dogInfo.flags & 4) && entities[dogInfo.entityIndex].entityCollidedWithIndex == ENTITY_PLAYER && entities[dogInfo.entityIndex].buttonPressed == BUTTON_A) {

                // ???
                if (!i) {
                    gPlayer.heldItem = DOG_HELD_ITEM;
                } else {
                    gPlayer.heldItem = PUPPY_1_HELD_ITEM;
                }
                
                deactivateEntity(dogInfo.entityIndex);
                setPlayerAction(4, 6);

                dogInfo.flags &= ~(4 | 0x10);
                dogInfo.flags |= 8;

                if (!(dogInfo.flags & 0x40)) {

                    adjustDogAffection(1);

                    dogInfo.flags |= 0x40;
                    
                }
                
                set = TRUE;
                
            }

            for (i = 0; i < MAX_CHICKENS && !set; i++) {

                if ((gChickens[i].flags & 4) && entities[gChickens[i].entityIndex].entityCollidedWithIndex == ENTITY_PLAYER && entities[gChickens[i].entityIndex].buttonPressed == BUTTON_A) {

                    if (checkDailyEventBit(2) && getLevelFlags(gChickens[i].location) & 0x20) {

                        if (gChickens[i].type == ADULT_CHICKEN) {

                            setGameVariableString(0xD, gChickens[i].name, 6);
                            // pink overlay message
                            func_8005B09C(7);
                            
                            D_801C4216 = 4;
                            D_801FC155 = i;
                            
                            func_8009B914();
                            
                            
                        } else {
                            showTextBox(0, 6, 0x5C, 0, 2);
                        }
                        
                        set = TRUE;
                        
                    } else {

                        switch (gChickens[i].type) {

                            case ADULT_CHICKEN:
                                gPlayer.heldItem = CHICKEN_HELD_ITEM;
                                gChickens[i].flags |= 8;
                                set = TRUE;
                                break;
                            
                            case CHICK:
                                gPlayer.heldItem = CHICK_HELD_ITEM;
                                gChickens[i].flags |= 8;
                                set = TRUE;
                                break;
                            
                            case CHICKEN_EGG:

                                if (!(gChickens[i].flags & 0x20)) {
                                    set = TRUE;
                                    gPlayer.heldItem = EGG_HELD_ITEM;
                                    gChickens[i].flags = 0;
                                }
                                
                                break;
                            
                        }

                        if (set) {
                            
                            deactivateEntity(gChickens[i].entityIndex);
                            
                            setPlayerAction(4, 6);
                            gPlayer.heldAnimalIndex = i;
                            
                            gChickens[i].flags &= ~4;
                            
                        }
                        
                    }
                    
                }

            }

            for (i = 0; i < MAX_FARM_ANIMALS && !set; i++) {

                if ((gFarmAnimals[i].flags & 4) && entities[gFarmAnimals[i].entityIndex].entityCollidedWithIndex == ENTITY_PLAYER) {

                    if (entities[gFarmAnimals[i].entityIndex].buttonPressed == BUTTON_A) {
                        
                        if (checkDailyEventBit(2) && getLevelFlags(gFarmAnimals[i].location) & 0x20) {

                            if ((gFarmAnimals[i].type == ADULT_COW || gFarmAnimals[i].type == ADULT_SHEEP) && (!(COW_HAPPY < gFarmAnimals[i].condition && gFarmAnimals[i].condition < COW_DEAD))) {
                                
                                setGameVariableString(0xD, gFarmAnimals[i].name, 6);
                                // pink overlay message
                                func_8005B09C(7);
                                
                                if (gFarmAnimals[i].type == ADULT_COW) {
                                    D_801C4216 = 2;
                                } else {
                                    D_801C4216 = 3;
                                }
                                
                                D_801FC155 = i;
                                
                                func_8009B914();
                                
                            }  else {
                                showTextBox(0, 6, 0x5C, 0, 2);
                            }
                        
                        } else if (checkDailyEventBit(0x1F)) {
    
                            if (gFarmAnimals[i].type == ADULT_COW && !(COW_HAPPY < gFarmAnimals[i].condition && gFarmAnimals[i].condition < COW_DEAD)) {
                                
                                // pink overlay message
                                func_8005B09C(8);
                                
                                setGameVariableString(0xD, gFarmAnimals[i].name, 6);
                                D_801FC155 = i;
                                
                            }  else {
                                showTextBox(0, 4, 0x35, 0, 0);
                            }
                            
                        } else {
    
                            func_8003F360(0, -4, 2);
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
    
                            showTextBox(1, 7, textIndex, 0, 2);
    
                            gFarmAnimals[i].flags |= (0x200 | 0x2000);
                            
                            if (!(gFarmAnimals[i].flags & 0x1000)) {
                                
                                func_80086458(i, 1);
                                func_800861A4(2, i, 0xFF, 0xFF, 0x11);
    
                                gFarmAnimals[i].flags |= 0x8000;
                                
                                func_8009BCC4(2, i, 3);
    
                                gFarmAnimals[i].flags |= 0x1000;
                                
                            }
                            
                        }
    
                        gFarmAnimals[i].flags &= ~0x400;
    
                    }

                    set = TRUE;
                    
                }
                
            }
            
            if (horseInfo.flags & 4 && entities[horseInfo.entityIndex].entityCollidedWithIndex == ENTITY_PLAYER && entities[horseInfo.entityIndex].buttonPressed == BUTTON_A) {

                if (gPlayer.heldItem == 0) {

                    if (horseInfo.grown == TRUE) {

                        if (!checkTerrainCollisionInDirection(ENTITY_PLAYER, 0x18, convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, gMainMapIndex)) 
                            && !checkTerrainCollisionInDirection(ENTITY_PLAYER, 0x10, convertWorldToSpriteDirection(gPlayer.direction, gMainMapIndex))) {
                            
                            vec = projectEntityPosition(ENTITY_PLAYER, 0x20, convertWorldToSpriteDirection(gPlayer.direction, MAIN_MAP_INDEX));
                             
                            groundObjectIndex = getGroundObjectIndexFromCoordinates(vec.x, vec.z);

                            if (groundObjectIndex == 0xFF || (getGroundObjectPlayerInteractionsFlags(groundObjectIndex) & 8)) {
                                
                                setPlayerAction(0xD, 0xF);

                                horseInfo.flags &= ~(4 | 0x10);

                                if (!(horseInfo.flags & 0x400)) {

                                    adjustHorseAffection(1);

                                    horseInfo.flags |= 0x400;

                                }
                                
                                set = TRUE;
                                
                            }
                            
                        }
                        
                    } else {

                        func_8003F360(0, -4, 2);
                        setMessageBoxViewSpacePosition(0, 0.0f, -64.0f, 352.0f);
                        setMessageBoxSpriteIndices(0, 1, 0, 0);
                        setGameVariableString(3, horseInfo.name, 6);
                        
                        if (horseInfo.age == 0) {
                            showTextBox(1, 7, 7, 0, 2);
                        } else {
                            showTextBox(1, 7, 8, 0, 2);
                        }

                        horseInfo.flags |= 0x10;
                            
                        if (!(horseInfo.flags & 0x800)) {

                            horseInfo.unk_17 = 17;

                            if (checkHaveKeyItem(STAMINA_CARROT)) {
                                adjustHorseAffection(2);
                            } else {
                                adjustHorseAffection(1);
                            }

                            horseInfo.flags |= (0x20 | 0x800);
                            
                            func_8009BCC4(1, 0, 3);
                            
                        }
                    
                        set = TRUE;
    
                    }
                    
                }
                
            }

            for (i = 0; i < MAX_MISC_ANIMALS && !set; i++) {

                if (gMiscAnimals[i].flags & 4) {

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

                                if (!(15 < gMiscAnimals[i].unk_F && gMiscAnimals[i].unk_F < 18)) {
                                    gPlayer.heldItem = SQUIRREL_HELD_ITEM;
                                    set = TRUE;
                                }
                                
                                break;
                            
                            case 10:
                                
                                if (!(15 < gMiscAnimals[i].unk_F && gMiscAnimals[i].unk_F < 18)) {
                                    gPlayer.heldItem = MONKEY_HELD_ITEM;
                                    set = TRUE;
                                }
                                
                                break;

                            case 16:

                                if (gMiscAnimals[i].unk_F != 1) {
                                    gPlayer.heldItem = LADYBUG_HELD_ITEM;
                                    set = TRUE;
                                }
                                
                                break;
                            
                            case 17:
                                                                
                                if (gMiscAnimals[i].unk_F != 1) {
                                    gPlayer.heldItem = CICADA_HELD_ITEM;
                                    set = TRUE;
                                }
                                
                                break;
                            
                            case 18:
                                                                
                                if (gMiscAnimals[i].unk_F != 1) {
                                    gPlayer.heldItem = HORNED_BEETLE_HELD_ITEM;
                                    set = TRUE;
                                }
                                
                                break;
                            
                            case 19:
                                                                
                                if (gMiscAnimals[i].unk_F != 1) {
                                    gPlayer.heldItem = STAG_BEETLE_HELD_ITEM;
                                    set = TRUE;
                                }
                                
                                break;
                            
                            case 21:
                                                                
                                if (gMiscAnimals[i].unk_F != 1) {
                                    gPlayer.heldItem = CRICKET_HELD_ITEM;
                                    set = TRUE;
                                }
                                
                                break;
                            
                            case 15:
                                                                
                                if (gMiscAnimals[i].unk_F != 1) {
                                    gPlayer.heldItem = BUTTERFLY_HELD_ITEM;
                                    set = TRUE;
                                }
                                
                                break;
                            
                            case 20:

                                if (gMiscAnimals[i].unk_F != 1) {
                                    gPlayer.heldItem = RED_DRAGONFLY_HELD_ITEM;
                                    set = TRUE;
                                }
                                
                                break;
                            
                        }

                        if (set) {

                            deactivateEntity(gMiscAnimals[i].entityIndex);
                            
                            setPlayerAction(4, 6);
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

                                    if (15 < gMiscAnimals[i].unk_F && gMiscAnimals[i].unk_F < 18) {
                                        gMiscAnimals[i].unk_F = 0x11;    
                                    } else {
                                        gMiscAnimals[i].unk_F = 0x20;
                                        gMiscAnimals[i].flags |= 0x10;
                                    }
                                    
                                    break;

                                case 10:

                                    if (15 < gMiscAnimals[i].unk_F && gMiscAnimals[i].unk_F < 18) {
                                        gMiscAnimals[i].unk_F = 0x11;
                                    } else {
                                        gMiscAnimals[i].unk_F = 0x20;
                                        gMiscAnimals[i].flags |= 0x10;
                                    }
                                    
                                    break;

                                case 7:
                                    gMiscAnimals[i].unk_F = 0x20;
                                    gMiscAnimals[i].flags |= 0x10;
                                    break;

                                case 8:
                                    gMiscAnimals[i].unk_F = 0x20;
                                    gMiscAnimals[i].flags |= 0x10;
                                    break;

                                case 11:
                                    gMiscAnimals[i].unk_F = 0x20;
                                    gMiscAnimals[i].flags |= 0x10;
                                    break;

                                case 12:
                                    gMiscAnimals[i].unk_F = 0x20;
                                    gMiscAnimals[i].flags |= 0x10;
                                    break;

                                case 13:
                                    gMiscAnimals[i].unk_F = 0x20;
                                    gMiscAnimals[i].flags |= 0x10;
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

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_800876D0);

// initialize animal locations on farm
void func_800876D0(void) {

    u8 i;

    // check dog/horse race
    if (!checkLifeEventBit(0x52) || !checkDailyEventBit(0x42)) {
        func_8008B1B8();
    }

    for (i = 0; i < MAX_CHICKENS; i++) {
        func_8008B2E8(i);
    }

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {
        func_8008B55C(i);
    }

    // check dog/horse race
    if (!checkLifeEventBit(0x51) || !checkDailyEventBit(0x41)) {
        func_8008B9AC();
    }

    for (i = 0; i < 7; i++) {
        func_8008BAF0(i, 0);
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008779C);

void func_8008779C(void) {

    u8 i;

    if (dogInfo.flags & 4) {
        if (entities[dogInfo.entityIndex].flags & 8) {
            deactivateEntity(dogInfo.entityIndex);
        }
    }

    for (i = 0; i < MAX_CHICKENS; i++) {

        if (gChickens[i].flags & 4) {

            if (entities[gChickens[i].entityIndex].flags & 8) {
                deactivateEntity(gChickens[i].entityIndex);
            }

        }
    }

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {
        if (gFarmAnimals[i].flags & 4) {
            if (entities[gFarmAnimals[i].entityIndex].flags & 8) {
                deactivateEntity(gFarmAnimals[i].entityIndex);
            }
        }
    }

    if (horseInfo.flags & 4) {
        if (entities[horseInfo.entityIndex].flags & 8) {
            deactivateEntity(horseInfo.entityIndex);
        }
    }

    for (i = 0; i < MAX_MISC_ANIMALS; i++) {
        if (gMiscAnimals[i].flags & 4) {
            if (entities[gMiscAnimals[i].entityIndex].flags & 8) {
                deactivateEntity(gMiscAnimals[i].entityIndex);
            }
        }
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", updateAnimalCoordinates);

void updateAnimalCoordinates(void) {

    u8 i = 0;

    if ((dogInfo.flags) & 4 && (dogInfo.location == gBaseMapIndex)) {
        dogInfo.coordinates.x = entities[dogInfo.entityIndex].coordinates.x; 
        dogInfo.coordinates.y = entities[dogInfo.entityIndex].coordinates.y;
        dogInfo.coordinates.z = entities[dogInfo.entityIndex].coordinates.z;
    }

    for (i = 0; i < MAX_CHICKENS; i++) {

        if ((gChickens[i].flags & 4) && (gChickens[i].location == gBaseMapIndex)) {

            gChickens[i].coordinates.x = entities[gChickens[i].entityIndex].coordinates.x; 
            gChickens[i].coordinates.y = entities[gChickens[i].entityIndex].coordinates.y;
            gChickens[i].coordinates.z = entities[gChickens[i].entityIndex].coordinates.z;
            
        }
        
    }

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {

        if ((gFarmAnimals[i].flags & 4) && (gFarmAnimals[i].location == gBaseMapIndex)) {

            gFarmAnimals[i].coordinates.x = entities[gFarmAnimals[i].entityIndex].coordinates.x; 
            gFarmAnimals[i].coordinates.y = entities[gFarmAnimals[i].entityIndex].coordinates.y;
            gFarmAnimals[i].coordinates.z = entities[gFarmAnimals[i].entityIndex].coordinates.z;
            
        }
        
    }

    if ((horseInfo.flags) & 4 && (horseInfo.location == gBaseMapIndex)) {
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
        func_8008CF94(i);
    }

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {
        func_8008DAA0(i);
    }

    updateHorse();
    
    for (i = 0; i < 7; i++) {
        func_8009476C(i);
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80087D5C);

void func_80087D5C(void) {

    u8 i;

    for (i = 0; i < MAX_CHICKENS; i++) {
        gChickens[i].flags |= CHICKEN_FED;
    }

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {
        gFarmAnimals[i].flags |= (ANIMAL_FED | 0x10);
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", resetAnimalStatuses);

void resetAnimalStatuses(void) {

    u8 i;
    
    func_8008A5F0();

    dogInfo.flags &= ~(0x40 | 0x80);

    for (i = 6; i < MAX_CHICKENS; i++) {
        if (!(gChickens[i].flags & 0x20)) {
            gChickens[i].flags = 0;
        }
    }

    for (i = 0; i < MAX_CHICKENS; i++) {
        func_8008A650(i);
        gChickens[i].flags &= ~(CHICKEN_FED| 0x80);
    }

    for (i = 0; i < MAX_FARM_ANIMALS; i++) { 
        func_8008A9E8(i);
        gFarmAnimals[i].flags &= ~(8 | 0x10 | 0x20 | 0x40 | 0x80 | 0x100 | 0x1000 | 0x4000);
    }

    func_8008B150();

    horseInfo.flags &= ~(0x100 | 0x200 | 0x400 | 0x800);

}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80087F28);

// initialize new chicken when born
u8 func_80087F28(u8 animalType, u8 arg1) {

    u32 padding[4];
    
    u8 found;
    u8 i;
    u8 temp;

    found = 0xFF;
    temp = animalType != 0 ? 0 : 6;
    
    for (i = 0; i < 6 && found == 0xFF; i++) {

        if (!(gChickens[i + temp].flags & 1)) {
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

        func_800861A4(1, found, animalType, 0, 0);

        gChickens[found].direction = 2;
        gChickens[found].flags = 1;
        
    }

    return found;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", initializeChicken);

void initializeChicken(u8 chickenIndex) {

    gChickens[chickenIndex].location = 0;
    
    gChickens[chickenIndex].unk_17 = 0;
    gChickens[chickenIndex].direction = 0;
    gChickens[chickenIndex].unk_19 = 0;
    gChickens[chickenIndex].unk_1A = 0;
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

    // FIXME: shouldn't be necessary
    u32 padding[4];

    u8 index = 0xFF;
    u8 i = 0;

    // find free farm animal slot
    do {

        if (!(gFarmAnimals[i].flags & 1)) {
            index = i;
        }

        i++;
        
    } while (i < MAX_FARM_ANIMALS && index == 0xFF);

    if (index != 0xFF) {
        
        func_800861A4(2, index, animalType, 0, 0);
        setFarmAnimalLocation(index);

        gFarmAnimals[index].birthdaySeason = gSeason;
        gFarmAnimals[index].flags = 1;
        gFarmAnimals[index].birthdayDayOfMonth = gDayOfMonth;
        gFarmAnimals[index].normalMilk = 0xFF;
        gFarmAnimals[index].affection = 0;

        if (animalType == 0) {

            gFarmAnimals[index].unk_23[0] = gFarmAnimals[arg1].name[0];
            gFarmAnimals[index].unk_23[1] = gFarmAnimals[arg1].name[1];
            gFarmAnimals[index].unk_23[2] = gFarmAnimals[arg1].name[2];
            gFarmAnimals[index].unk_23[3] = gFarmAnimals[arg1].name[3];
            gFarmAnimals[index].unk_23[4] = gFarmAnimals[arg1].name[4];
            gFarmAnimals[index].unk_23[5] = gFarmAnimals[arg1].name[5];
            
        } else {

            gFarmAnimals[index].unk_23[0] = 0xF6;
            gFarmAnimals[index].unk_23[1] = 0xF6;
            gFarmAnimals[index].unk_23[2] = 0xF6;
            gFarmAnimals[index].unk_23[3] = 0xF6;
            gFarmAnimals[index].unk_23[4] = 0xF6;
            gFarmAnimals[index].unk_23[5] = 0xF6;
            
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

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_800886D0);

// initialize watched cows from Doug
void func_800886D0(void) {
    initializeFarmAnimal(mrsManaCow1Index);
    initializeFarmAnimal(mrsManaCow2Index);
    initializeFarmAnimal(mrsManaCow3Index);
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", setFarmAnimalLocation);

// set farm animal locations
void setFarmAnimalLocation(u8 animalIndex) {

    gFarmAnimals[animalIndex].location = BARN;

    if (gFarmAnimals[animalIndex].type == PREGNANT_COW) {
        
        gFarmAnimals[animalIndex].coordinates.x = D_80114AB0.x;
        gFarmAnimals[animalIndex].coordinates.y = D_80114AB0.y;
        gFarmAnimals[animalIndex].coordinates.z = D_80114AB0.z;
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
    
    gFarmAnimals[animalIndex].unk_1B = 0;
    gFarmAnimals[animalIndex].direction = 0;
    gFarmAnimals[animalIndex].unk_14 = 0;
    gFarmAnimals[animalIndex].unk_1D = 0;
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
    dogInfo.unk_17 = 0;
    dogInfo.direction = SOUTHWEST;
    dogInfo.affection = 0; 
    dogInfo.speed = 0;
    dogInfo.unk_1A = 0;
    dogInfo.unk_1B = 0;
    dogInfo.flags = 1;
    dogInfo.coordinates.x = -432.0f;
    dogInfo.coordinates.z = 96.0f;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", initializeHorse);

void initializeHorse(void) {
    
    horseInfo.location = FARM;
    horseInfo.coordinates.y = 0.0f;
    horseInfo.unk_17 = 0;
    horseInfo.direction = SOUTHWEST;
    horseInfo.speed = 0;
    horseInfo.unk_1A = 0;
    horseInfo.unk_1B = 0;
    horseInfo.grown = 0;
    horseInfo.age = 0;
    horseInfo.affection = 0;
    horseInfo.flags = 1;
    horseInfo.coordinates.x = -240.0f;
    horseInfo.coordinates.z = -384.0f;

}

//INCLUDE_ASM("asm/nonmatchings/game/animals", setAnimalLocations);

void setAnimalLocations(u8 mapIndex) {

    u8 i;
    
    setDogLocation(mapIndex);
    setHorseLocation(mapIndex);

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {
        func_80088BB0(mapIndex, i);
    }

    for (i = 0; i < MAX_CHICKENS; i++) {
        func_80088C1C(mapIndex, i);
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/animals", setDogLocation);

void setDogLocation(u8 mapIndex) {

    if (dogInfo.flags & 1 && (mapIndex == 0xFF || dogInfo.location == mapIndex)) {
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

    if (horseInfo.flags & 1 && (mapIndex == 0xFF || horseInfo.location == mapIndex)) {
        horseInfo.coordinates.y = 0;
        horseInfo.location = FARM;
        horseInfo.direction = SOUTHWEST;
        horseInfo.coordinates.x = -240.0f;
        horseInfo.coordinates.z = -384.0f;
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80088BB0);

void func_80088BB0(u8 mapIndex, u8 animalIndex) {

    if ((gFarmAnimals[animalIndex].flags & 1) && (mapIndex == 0xFF ||  gFarmAnimals[animalIndex].location == mapIndex)) {
        setFarmAnimalLocation(animalIndex);
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80088C1C);

void func_80088C1C(u8 mapIndex, u8 chickenIndex) {

    if ((gChickens[chickenIndex].flags & 1) && (mapIndex == 0xFF || gChickens[chickenIndex].location == mapIndex)) {

        gChickens[chickenIndex].location = COOP;

        if (gChickens[chickenIndex].flags & 0x20) {

            gChickens[chickenIndex].coordinates.x = 96.0f;
            gChickens[chickenIndex].coordinates.y = 0;
            gChickens[chickenIndex].coordinates.z = -144.0f;
            
        } else {

            gChickens[chickenIndex].coordinates.x = chickenStartingCoordinates[chickenIndex].x;
            gChickens[chickenIndex].coordinates.y = chickenStartingCoordinates[chickenIndex].y;
            gChickens[chickenIndex].coordinates.z = chickenStartingCoordinates[chickenIndex].z;
            
        }

        gChickens[chickenIndex].flags &= ~8;
        
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80088D54);

void func_80088D54(void) {

    switch (gBaseMapIndex) {
        
        case FARM:

            if (gSeason != WINTER && gWeather == SUNNY && 5 < gHour && gHour < 9) {

                if (gMiscAnimals[4].unk_16 == 1) {
                    func_8008A4A8(0xB, SOUTHWEST, -336.0f, 0.0f, 0.0f);
                    func_8008A4A8(0xB, SOUTHWEST, -368.0f, 0.0f, 32.0f);
                }

                if (gMiscAnimals[4].unk_16 == 2) {
                    func_8008A4A8(0xB, SOUTHWEST, 320.0f, 0.0f, -352.0f);
                    func_8008A4A8(0xB, SOUTHWEST, 288.0f, 0.0f, -352.0f);
                }
                
            }
            
            break;
        
        case POND:

            if (gSeason == AUTUMN && 12 < gHour && gHour < 22 && gMiscAnimals[0].unk_16 >= 2 && (u8)(gPlayer.heldItem + 0x75) >= 8) {
                func_8008A4A8(7, 6, -192.0f, 0.0f, -64.0f);
            }

            if (gSeason == WINTER && 12 < gHour && gHour < 22 && gMiscAnimals[0].unk_16 == 1 && (u8)(gPlayer.heldItem + 0x75) >= 8) {
                func_8008A4A8(7, 6, -192.0f, 0.0f, -64.0f);
            }
            
            if (gSeason == SPRING && 8 < gHour && gHour < 15 && gMiscAnimals[1].unk_16 == 1 && (u8)(gPlayer.heldItem + 0x65) >= 8) {
                func_8008A4A8(8, 4, -192.0f, 0.0f, -160.0f);
            }

            if (gSeason == SUMMER && 10 < gHour && gHour < 17 && !(gMiscAnimals[1].unk_16 < 2) && (u8)(gPlayer.heldItem + 0x65) >= 8) {
                func_8008A4A8(8, 4, -192.0f, 0.0f, -160.0f);
            }

            if (gSeason == SUMMER && 10 < gHour && gHour < 17 && gMiscAnimals[2].unk_16 == 1 && (u8)(gPlayer.heldItem + 0x6D) >= 8) {
                gMiscAnimals[func_8008A4A8(9, SOUTHWEST, 144.0f, 0.0f, -256.0f)].unk_F = 0x10;
            }

            if (gSeason == WINTER && 10 < gHour && gHour < 17 && gMiscAnimals[3].unk_16 >= 2 && (u8)(gPlayer.heldItem + 0x5D) >= 8) {
                func_8008A4A8(0xA, SOUTHEAST, -192.0f, 0.0f, -224.0f);
            }

            if (gSeason == SUMMER && gWeather != RAIN && 7 < gHour && gHour < 16 && gMiscAnimals[4].unk_16 < 2) {
                func_8008A4A8(0xE, SOUTHWEST, 96.0f, 0.0f, -64.0f);
            }

            break;

        case MOUNTAIN_1:

            if (gSeason == SUMMER && 10 < gHour && gHour < 17 && gMiscAnimals[1].unk_16 < 2 && (u8)(gPlayer.heldItem + 0x65) >= 8) {
                func_8008A4A8(8, NORTHWEST, 128.0f, 0.0f, 64.0f);
            }

            if (gSeason == SPRING && 6 < gHour && gHour < 16 && !(gMiscAnimals[2].unk_16 < 2) && (u8)(gPlayer.heldItem + 0x6D) >= 8) {
                func_8008A4A8(9, SOUTHWEST, 256.0f, 0.0f, -416.0f);
            }
            
            if (gSeason == SUMMER && 10 < gHour && gHour < 17 && gMiscAnimals[2].unk_16 < 1 && (u8)(gPlayer.heldItem + 0x6D) >= 8) {
                func_8008A4A8(9, SOUTHWEST, 160.0f, 0.0f, 128.0f);
            }

            if (gSeason == AUTUMN && 10 < gHour && gHour < 17 && gMiscAnimals[3].unk_16 == 1 && (u8)(gPlayer.heldItem + 0x5D) >= 8) {
                func_8008A4A8(0xA, SOUTHWEST, -96.0f, 0.0f, 128.0f);
            }

            if (gSeason == SUMMER && gWeather != RAIN && 7 < gHour && gHour < 16 && !(gMiscAnimals[4].unk_16 < 2)) {
                func_8008A4A8(0xE, SOUTHWEST, 160.0f, 0.0f, 128.0f);
            }

            if (gSeason == AUTUMN && gWeather != RAIN && 8 < gHour && gHour < 16 && !(gMiscAnimals[5].unk_16 < 2) && gPlayer.heldItem != RED_DRAGONFLY_HELD_ITEM) {
                func_8008A4A8(0x14, SOUTHWEST, 0.0f, 0.0f, -128.0f);
            }

            if (gSeason == AUTUMN && gWeather != RAIN && 8 < gHour && gHour < 16 && gMiscAnimals[4].unk_16 < 2 && gPlayer.heldItem != CRICKET_HELD_ITEM) {
                func_8008A4A8(0x15, SOUTHWEST, 128.0f, 0.0f, 64.0f);
            }
            
            break;

        case MOUNTAIN_2:

            if (gSeason == AUTUMN && 12 < gHour && gHour < 22 && gMiscAnimals[0].unk_16 < 2 && (u8)(gPlayer.heldItem + 0x75) >= 8) {
                func_8008A4A8(7, 6, -128.0f, 0.0f, 224.0f);
            }

            if (gSeason == WINTER && 12 < gHour && gHour < 22 && gMiscAnimals[0].unk_16 == 0 && (u8)(gPlayer.heldItem + 0x75) >= 8) {
                func_8008A4A8(7, 6, -128.0f, 0.0f, 224.0f);
            }
            
            if (gSeason == SPRING && 8 < gHour && gHour < 15 && gMiscAnimals[1].unk_16 < 1 && (u8)(gPlayer.heldItem + 0x65) >= 8) {
                func_8008A4A8(8, NORTHEAST, -192.0f, 0.0f, -192.0f);
            }

            if (gSeason == SPRING && 6 < gHour && gHour < 16 && gMiscAnimals[2].unk_16 < 2 && (u8)(gPlayer.heldItem + 0x6D) >= 8) {
                func_8008A4A8(9, SOUTHWEST, -224.0f, 0.0f, -160.0f);
            }

            if (gSeason == SUMMER && 10 < gHour && gHour < 17 && gMiscAnimals[3].unk_16 < 2 && (u8)(gPlayer.heldItem + 0x5D) >= 8) {
                func_8008A4A8(0xA, SOUTHWEST, -320.0f, 0.0f, -320.0f);
            }

            if (gSeason == SUMMER && 10 < gHour && gHour < 17 && !(gMiscAnimals[3].unk_16 < 2) && (u8)(gPlayer.heldItem + 0x5D) >= 8) {
                func_8008A4A8(0xA, SOUTHWEST, 0.0f, 0.0f, 288.0f);
            }

            if (gSeason == AUTUMN && 10 < gHour && gHour < 17 && gMiscAnimals[3].unk_16 == 0 && (u8)(gPlayer.heldItem + 0x5D) >= 8) {
                gMiscAnimals[func_8008A4A8(0xA, SOUTHWEST, -112.0f, 0.0f, -160.0f)].unk_F = 0x10;
            }

            if (gSeason != WINTER && gWeather != RAIN && 7 < gHour && gHour < 16 && gMiscAnimals[4].unk_16 < 2) {
                func_8008A4A8(0xC, SOUTHWEST, -128.0f, 0.0f, 32.0f);
                func_8008A4A8(0xC, SOUTHEAST, -96.0f, 0.0f, 64.0f);
            }

            if (gSeason < AUTUMN && gWeather != RAIN && 8 < gHour && gHour < 16 && gPlayer.heldItem < 0xB4) {
                func_8008A4A8(0x10, SOUTHWEST, 96.0f, 0.0f, -96.0f);
            }

            if (gSeason == SUMMER && gWeather != RAIN && 8 < gHour && gHour < 16 && gMiscAnimals[3].unk_16 < 2 && gPlayer.heldItem != CICADA_HELD_ITEM) {
                func_8008A4A8(0x11, SOUTHWEST, -112.0f, 0.0f, -160.0f);
            }
            
            break;
        
        case TOP_OF_MOUNTAIN_1:

            if (gSeason == SUMMER && gWeather != RAIN && 8 < gHour && gHour < 16 && gMiscAnimals[0].unk_16 < 2 && gPlayer.heldItem != HORNED_BEETLE_HELD_ITEM) {
                func_8008A4A8(0x12, SOUTHWEST, 192.0f, 0.0f, -192.0f);
            }

            if (gSeason == SUMMER && gWeather != RAIN && 8 < gHour && gHour < 16 && gMiscAnimals[1].unk_16 < 2 && gPlayer.heldItem != STAG_BEETLE_HELD_ITEM) {
                func_8008A4A8(0x13, SOUTHWEST, 224.0f, 0.0f, 0.0f);
            }

            break;
        
        case MOON_MOUNTAIN:

            if (gSeason != WINTER && gWeather != RAIN && 7 < gHour && gHour < 16 && !(gMiscAnimals[4].unk_16 < 2)) {
                func_8008A4A8(0xC, SOUTHWEST, 192.0f, 0.0f, -64.0f);
                func_8008A4A8(0xC, SOUTHEAST, 224.0f, 0.0f, -96.0f);
            }

            if (gSeason == AUTUMN && gWeather != RAIN && 8 < gHour && gHour < 16 && gMiscAnimals[5].unk_16 == 0 && gPlayer.heldItem != RED_DRAGONFLY_HELD_ITEM) {
                func_8008A4A8(0x14, SOUTHWEST, 128.0f, 0.0f, -256.0f);
            }

            break;
        
        case SQUARE:

            if (gSeason != WINTER && gWeather == SUNNY && 5 < gHour && gHour < 9 && gMiscAnimals[4].unk_16 == 3) {
                func_8008A4A8(0xB, SOUTHWEST, -128.0f, 0.0f, 0.0f);
                func_8008A4A8(0xB, SOUTHWEST, -96.0f, 0.0f, -32.0f);
            }

            break;
        
        case BEACH:

            if (gSeason == SUMMER && gWeather != RAIN && 7 < gHour && gHour < 16) {
                func_8008A4A8(0xD, SOUTHWEST, -256.0f, 0.0f, -64.0f);
            }

            break;

        case VILLAGE_1:
                
            if (gSeason < AUTUMN && gWeather != RAIN && 8 < gHour && gHour < 16 && gMiscAnimals[0].unk_16 == 0 && gPlayer.heldItem != BUTTERFLY_HELD_ITEM) {
                func_8008A4A8(0xF, SOUTHWEST, 64.0f, 0.0f, -352.0f);
            }

            if (gSeason < AUTUMN && gWeather != RAIN && 8 < gHour && gHour < 16 && gMiscAnimals[0].unk_16 == 1 && gPlayer.heldItem != BUTTERFLY_HELD_ITEM) {
                func_8008A4A8(0xF, SOUTHWEST, -448.0f, 0.0f, 0.0f);
            }

            if (gSeason < AUTUMN && gWeather != RAIN && 8 < gHour && gHour < 16 && gMiscAnimals[5].unk_16 == 1 && gPlayer.heldItem < LADYBUG_HELD_ITEM) {
                func_8008A4A8(0x10, SOUTHWEST, -448.0f, 0.0f, 64.0f);
            }
        
            break;

        case VILLAGE_2:

            if (8 < gHour && gHour < 18) {

                if (gWeather != RAIN && gSeason != WINTER) {
                    
                    if ((u8)(gPlayer.heldItem + 0x55) >= 8) {
                        func_8008A4A8(0, SOUTHWEST, 400.0f, 0.0f, -160.0f);
                    }
    
                    if (checkLifeEventBit(PUPPIES) && (u8)(gPlayer.heldItem - 0x7B) >= 8) {
                        func_8008A4A8(1, SOUTHWEST, 432.0f, 0.0f, -160.0f);
                    }
                    
                }

                if (8 < gHour && gHour < 18 && !(SUNNY < gWeather && gWeather < 4)) {
                    
                    if ((u8)(gPlayer.heldItem + 0x7D) >= 8) {
                        func_8008A4A8(2, SOUTHWEST, 32.0f, 0.0f, -96.0f);
                    }
                
                    func_8008A4A8(2, SOUTHWEST, 0.0f, 0.0f, -64.0f);
                
                }
                
            }

            break;

        case POTION_SHOP:

            if (gWeather == RAIN || gSeason == WINTER)  {

                if ((u8)(gPlayer.heldItem + 0x55) >= 8) {
                    func_8008A4A8(0, SOUTHWEST, -64.0f, 0.0f, -64.0f);
                }

                if (checkLifeEventBit(PUPPIES) && (u8)(gPlayer.heldItem - 0x7B) >= 8) {
                    func_8008A4A8(1, SOUTHWEST, -96.0f, 0.0f, -64.0f);
                }
                
            }
            
            break;
        
        case RANCH:

            if (gMiscAnimals[0].unk_16 < 2 && gWeather != RAIN && gSeason != WINTER) {
                func_8008A4A8(4, NORTHEAST, -384.0f, 0.0f, 32.0f);
            }

            if (gMiscAnimals[1].unk_16 < 2 && gWeather != RAIN && gSeason != WINTER) {
                func_8008A4A8(3, SOUTHEAST, -320.0f, 0.0f, 0.0f);
            }
   
            if (gMiscAnimals[2].unk_16 < 2 && gWeather != RAIN && gSeason != WINTER) {
                func_8008A4A8(3, SOUTHWEST, -288.0f, 0.0f, 96.0f);
            }         
   
            if (gMiscAnimals[3].unk_16 < 2 && gWeather != RAIN && gSeason != WINTER) {
                func_8008A4A8(6, NORTHEAST, -288.0f, 0.0f, 128.0f);
            }         
   
            if (gMiscAnimals[4].unk_16 < 2 && gWeather != RAIN && gSeason != WINTER) {
                func_8008A4A8(5, SOUTHEAST, -352.0f, 0.0f, 80.0f);
            }         

            break;
        
        case RANCH_BARN:

            if (gMiscAnimals[0].unk_16 >= 2 || gWeather == RAIN || gSeason == WINTER) {
                func_8008A4A8(4, NORTHWEST, -96.0f, 0.0f, -192.0f);
            }

            if (gMiscAnimals[1].unk_16 >= 2 || gWeather == RAIN || gSeason == WINTER) {
                func_8008A4A8(3, NORTHWEST, -96.0f, 0.0f, -128.0f);
            }

            if (gMiscAnimals[2].unk_16 >= 2 || gWeather == RAIN || gSeason == WINTER) {
                func_8008A4A8(3, NORTHWEST, -96.0f, 0.0f, 64.0f);
            }

            if (gMiscAnimals[3].unk_16 >= 2 || gWeather == RAIN || gSeason == WINTER) {
                func_8008A4A8(6, SOUTHEAST, 0.0f, 0.0f, 64.0f);
            }

            if (gMiscAnimals[4].unk_16 >= 2 || gWeather == RAIN || gSeason == WINTER) {
                func_8008A4A8(5, SOUTHEAST, 0.0f, 0.0f, 128.0f);
            }
            
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008A4A8);

u8 func_8008A4A8(u8 index, u8 direction, f32 x, f32 y, f32 z) {

    u32 padding[4];
    
    u8 i = 0;
    u8 found = (index != 0) ? 0xFF : 6;
    u8 temp;
    
    if (index == 5) {
        found = 5;    
    }

    while (i < 5 && found == 0xFF) {

        if (!(gMiscAnimals[i].flags & 1)) {
            found = i;
        }
        
        i++;
        
    }
        
    if (found != 0xFF) {

        gMiscAnimals[found].animalType = index;
        gMiscAnimals[found].direction = direction;
        gMiscAnimals[found].unk_F = 0;
        gMiscAnimals[found].yDisplacement = 0;
        gMiscAnimals[found].flags = 1;
        gMiscAnimals[found].coordinates.x = x;
        gMiscAnimals[found].coordinates.y = y;
        gMiscAnimals[found].coordinates.z = z;
        gMiscAnimals[found].entityIndex = ENTITY_MISC_ANIMAL_BASE_INDEX + found;
        gMiscAnimals[found].mapIndex = gBaseMapIndex;
        
    }

    return found;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008A5F0);

void func_8008A5F0(void) {
    
    if (checkDailyEventBit(0x43) && (dogInfo.flags & 1)) {
        dogInfo.affection += adjustValue(dogInfo.affection, 2, MAX_AFFECTION);
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008A650);

void func_8008A650(u8 index) {
    
    if ((gChickens[index].flags & 1) && !(gChickens[index].flags & 0x100)) {
        
        if (gChickens[index].flags & 0x80) {
            
            gChickens[index].flags &= ~0x10;

        } else {

            if (gChickens[index].location != COOP) {
                gChickens[index].flags &= ~0x10;
            }

            if (gChickens[index].location == FARM) {
                
                if (gSeason != WINTER) {
                    gChickens[index].flags |= 0x10;
                } 

            } 

        }

        switch (gChickens[index].type) {
        
            case CHICKEN_EGG:
                
                if (gChickens[index].flags & 0x20) {
                    
                    gChickens[index].typeCounter++;
                    
                    if (gChickens[index].typeCounter == CHICKEN_EGG_INCUBATION_DURATION) {
                        
                        bornChickenIndex = func_80087F28(1, 0xFF);
                        
                        if (bornChickenIndex != 0xFF) {

                            gChickens[bornChickenIndex].flags |= 0x100;
                            gChickens[index].flags &= ~1;

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
                    func_800861A4(1, index, ADULT_CHICKEN, 0, 0);
                }
                
                break;
            
            case ADULT_CHICKEN:
                
                switch (gChickens[index].condition) {
                    
                    case CHICKEN_NORMAL:
                        
                        if ((gChickens[index].flags & CHICKEN_FED)) {
                            func_80087F28(0, index);
                        } else {
                            func_800861A4(1, index, 0xFF, CHICKEN_STARVED, 0);
                        }
                        
                        break;
                    
                    case CHICKEN_STARVED:
                        
                        if (gChickens[index].flags & CHICKEN_FED) {
                            func_800861A4(1, index, 0xFF, CHICKEN_NORMAL, 0);
                        } else {
                            
                            gChickens[index].conditionCounter++;
                            
                            if (gChickens[index].conditionCounter == 3) {
                                
                                func_800861A4(1, index, 0xFF, CHICKEN_DEAD, 0xFF);
                                
                                gChickens[index].flags = 0;

                                setLifeEventBit(ANIMAL_DIED);
                                
                                deadAnimalName[0] = gChickens[index].name[0];
                                deadAnimalName[1] = gChickens[index].name[1];
                                deadAnimalName[2] = gChickens[index].name[2];
                                deadAnimalName[3] = gChickens[index].name[3];
                                deadAnimalName[4] = gChickens[index].name[4];
                                deadAnimalName[5] = gChickens[index].name[5];

                            }
                            
                        }
                        
                        break;
                    
                }

            break;
            
        }
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008A9E8);

void func_8008A9E8(u8 index) {
    
    if ((gFarmAnimals[index].flags & 1) && !(gFarmAnimals[index].flags & 0x800)) {
        
        if (gFarmAnimals[index].flags & 0x4000) {
            
            gFarmAnimals[index].flags &= ~(ANIMAL_FED | 0x10);
            
        } else if (gFarmAnimals[index].location == FARM) {
            
            gFarmAnimals[index].flags &= ~(ANIMAL_FED | 0x10);
            
            // get grass tiles
            if (func_800DB99C()) {
                gFarmAnimals[index].flags |= (ANIMAL_FED | 0x10);
            }
            
        } else {
            func_80086458(index, -1);
        }

        if (D_80189054 != 0xFF) {
            
            if (D_80189054 == index) {
                gFarmAnimals[D_80189054].flags |= (ANIMAL_FED | 0x10);
            }
            
        }

        switch (gFarmAnimals[index].type) {
            
            case BABY_COW:
                gFarmAnimals[index].typeCounter++;
                
                if (gFarmAnimals[index].typeCounter == COW_INFANCY_DURATION) {
                    func_800861A4(2, index, CALF, 0xFF, 0);
                }
                
                break;
            
            case CALF:
                gFarmAnimals[index].typeCounter++;
                
                if (gFarmAnimals[index].typeCounter == COW_YOUTH_DURATION) {
                    func_800861A4(2, index, ADULT_COW, 0xFF, 0);
                }
                
                break;
            
            case ADULT_COW:
                
                switch (gFarmAnimals[index].condition) {
                    
                    case COW_NORMAL:
                        
                        if (!(gFarmAnimals[index].flags & ANIMAL_FED)) {
                            
                            func_80086458(index, -8);
                            
                            if (!(getRandomNumberInRange(0, 1))) {
                                func_800861A4(2, index, 0xFF, COW_SICK, 0);
                                func_80086458(index, -30);
                                gHappiness += adjustValue(gHappiness, -10, 0xFF);
                            }
        
                        } else if (gFarmAnimals[index].location == FARM) {
                            
                            if (!(getRandomNumberInRange(0, 3))) {
                                func_800861A4(2, index, 0xFF, COW_HAPPY, 0);
                                func_80086458(index, 30);
                                gHappiness += adjustValue(gHappiness, 5, 0xFF);
                            }
                            
                        }
                        
                        if (gFarmAnimals[index].flags & COW_PREGNANT) {
                            func_800861A4(2, index, PREGNANT_COW, 0xFF, 0);
                        }
                        
                        break;

                    case COW_HAPPY:

                        gFarmAnimals[index].conditionCounter++;
                            
                        if (gFarmAnimals[index].conditionCounter == 3) {
                            func_800861A4(2, index, 0xFF, COW_NORMAL, 0);
                        }
                        
                        break;
                    
                    case COW_MAD:
                        
                        if (gFarmAnimals[index].flags & ANIMAL_FED) {
                            
                            func_80086458(index, -8);
                            
                            gFarmAnimals[index].conditionCounter++;
                            
                            if (gFarmAnimals[index].conditionCounter == 3) {
                                func_800861A4(2, index, 0xFF, COW_NORMAL, 0);
                            }
                            
                        }
                        
                        break;

                    case COW_SICK:
                        
                        gFarmAnimals[index].conditionCounter++;
                        
                        if (gFarmAnimals[index].conditionCounter == ANIMAL_DEATH_COUNT) {
                            
                            func_800861A4(2, index, 0xFF, COW_DEAD, 0xFF);

                            gFarmAnimals[index].flags = 0;
                            
                            setLifeEventBit(ANIMAL_DIED);
                            
                            deadAnimalName[0] = gFarmAnimals[index].name[0];
                            deadAnimalName[1] = gFarmAnimals[index].name[1];
                            deadAnimalName[2] = gFarmAnimals[index].name[2];
                            deadAnimalName[3] = gFarmAnimals[index].name[3];
                            deadAnimalName[4] = gFarmAnimals[index].name[4];
                            deadAnimalName[5] = gFarmAnimals[index].name[5];
                            
                        }
                        
                        break;
                    
                }
            
                break;
            
            case PREGNANT_COW:
                
                gFarmAnimals[index].typeCounter++;
                
                if (gFarmAnimals[index].typeCounter == COW_PREGNANCY_DURATION) {
                    
                    bornAnimalIndex = initializeNewFarmAnimal(0, index);
                    
                    if (bornAnimalIndex != 0xFF) {
    
                        gFarmAnimals[bornAnimalIndex].flags |= 0x800;
                        gFarmAnimals[bornAnimalIndex].affection = gFarmAnimals[index].affection / 2;
                        
                        func_800861A4(2, index, ADULT_COW, 0xFF, 0);
                        setLifeEventBit(FARM_ANIMAL_BORN);
                    
                    } else {
                        gFarmAnimals[index].typeCounter--;
                    }
                    
                }
                
                break;
            
            case BABY_SHEEP:
                
                gFarmAnimals[index].typeCounter++;
                
                if (gFarmAnimals[index].typeCounter == SHEEP_YOUTH_DURATION) {
                    func_800861A4(2, index, ADULT_SHEEP, 0xFF, 0);
                }
    
                break;
            
            case SHEARED_SHEEP:
                
                gFarmAnimals[index].typeCounter++;
                
                if (gFarmAnimals[index].typeCounter == WOOL_REGROW_DURATION) {
                    func_800861A4(2, index, ADULT_SHEEP, 0xFF, 0);
                }
                
                switch (gFarmAnimals[index].condition) {
                    
                    case SHEEP_NORMAL:
                        
                        if (!(gFarmAnimals[index].flags & ANIMAL_FED)) {
                            
                            func_80086458(index, -8);
                            
                            if (!(getRandomNumberInRange(0, 1))) {
                                
                                func_800861A4(2, index, 0xFF, SHEEP_SICK, 0);
                                func_80086458(index, -30);
                                
                                gHappiness += adjustValue(gHappiness, -10, 0xFF);
                                
                            }
                            
                        }
                        
                        break;
                    
                    case SHEEP_SICK:
                        
                        gFarmAnimals[index].conditionCounter++;
                        
                        if (gFarmAnimals[index].conditionCounter == ANIMAL_DEATH_COUNT) {
                            
                            func_800861A4(2, index, 0xFF, SHEEP_DEAD, 0xFF);
                            gFarmAnimals[index].flags = 0;
                            
                            setLifeEventBit(ANIMAL_DIED);
                            
                            deadAnimalName[0] = gFarmAnimals[index].name[0];
                            deadAnimalName[1] = gFarmAnimals[index].name[1];
                            deadAnimalName[2] = gFarmAnimals[index].name[2];
                            deadAnimalName[3] = gFarmAnimals[index].name[3];
                            deadAnimalName[4] = gFarmAnimals[index].name[4];
                            deadAnimalName[5] = gFarmAnimals[index].name[5];
                            
                        }
                        
                        break;
                    
                }
                
                break;
            
            case ADULT_SHEEP:
                
                switch (gFarmAnimals[index].condition) {
                    
                    case SHEEP_NORMAL:

                        if (!(gFarmAnimals[index].flags & ANIMAL_FED)) {
                            
                            func_80086458(index, -8);
                            
                            if (!(getRandomNumberInRange(0, 1))) {
                                
                                func_800861A4(2, index, 0xFF, SHEEP_SICK, 0);
                                func_80086458(index, -30);
                                gHappiness += adjustValue(gHappiness, -10, 0xFF);
                                
                            }
                        }
                        
                        break;
    
                    case SHEEP_SICK:
                        
                        gFarmAnimals[index].conditionCounter++;
                        
                        if (gFarmAnimals[index].conditionCounter == ANIMAL_DEATH_COUNT) {
                            
                            func_800861A4(2, index, 0xFF, SHEEP_DEAD, 0xFF);
                            gFarmAnimals[index].flags = 0;
                            
                            setLifeEventBit(ANIMAL_DIED);
                            
                            deadAnimalName[0] = gFarmAnimals[index].name[0];
                            deadAnimalName[1] = gFarmAnimals[index].name[1];
                            deadAnimalName[2] = gFarmAnimals[index].name[2];
                            deadAnimalName[3] = gFarmAnimals[index].name[3];
                            deadAnimalName[4] = gFarmAnimals[index].name[4];
                            deadAnimalName[5] = gFarmAnimals[index].name[5];
                            
                        }
                        
                        break;
                    
                }
    
                break;
            
        }
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008B150);

void func_8008B150(void) {

    if ((horseInfo.flags & 1) && horseInfo.grown == FALSE) {
        
        horseInfo.age++;
    
        if (horseInfo.age == 20) {
            horseInfo.grown = TRUE;
            setLifeEventBit(0x3C);
        }
    
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008B1B8);

void func_8008B1B8(void) {
    
    if ((dogInfo.flags & 1) && (dogInfo.location == gBaseMapIndex) && !(dogInfo.flags & 8)) {
        
        dogInfo.entityIndex = 1;
        
        loadEntity(1, 0x40, TRUE);
        setEntityCollidable(dogInfo.entityIndex, TRUE);
        setEntityYMovement(dogInfo.entityIndex, TRUE);
        func_8002FF38(dogInfo.entityIndex, FALSE);
        
        setEntityDirection(dogInfo.entityIndex, convertSpriteToWorldDirection(dogInfo.direction, MAIN_MAP_INDEX));
        setEntityCoordinates(dogInfo.entityIndex, dogInfo.coordinates.x, dogInfo.coordinates.y, dogInfo.coordinates.z);
        
        dogInfo.unk_17 = 0;
        dogInfo.unk_1A = 0;
        dogInfo.flags |= 4;
        
    }
    
    updateDog();
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008B2E8);

void func_8008B2E8(u8 chickenIndex) {

    if ((gChickens[chickenIndex].flags & 1) && (gChickens[chickenIndex].location == gBaseMapIndex) && !(gChickens[chickenIndex].flags & 8)) {
    
        gChickens[chickenIndex].entityIndex = chickenIndex + 2;
        
        switch (gChickens[chickenIndex].type) {

            case ADULT_CHICKEN:
                initializeAnimalEntity(chickenIndex + 2, (void*)0x80275E00, (void*)0x80275F00, (void*)0x80276700, (void*)0x80276800);
                loadEntity(gChickens[chickenIndex].entityIndex, 0x44, TRUE);
                break;         
            
            case CHICK:
                initializeAnimalEntity(chickenIndex + 2, (void*)0x80276B00, (void*)0x80276C00, (void*)0x80277000, (void*)0x80277100);
                loadEntity(gChickens[chickenIndex].entityIndex, 0x43, TRUE);
                break;
            
            case CHICKEN_EGG:
                initializeAnimalEntity(chickenIndex + 2, (void*)0x8028DD50, (void*)0x80290550, (void*)0x80293A50, (void*)0x80293C50);
                loadEntity(gChickens[chickenIndex].entityIndex, 0x5D, TRUE);
                break;
            
            default:
                break;
            
        }

        setEntityCollidable(gChickens[chickenIndex].entityIndex, TRUE);
        setEntityYMovement(gChickens[chickenIndex].entityIndex, TRUE);
        func_8002FF38(gChickens[chickenIndex].entityIndex, FALSE);

        setEntityDirection(gChickens[chickenIndex].entityIndex, convertSpriteToWorldDirection(gChickens[chickenIndex].direction, MAIN_MAP_INDEX));        
        setEntityCoordinates(gChickens[chickenIndex].entityIndex, gChickens[chickenIndex].coordinates.x, gChickens[chickenIndex].coordinates.y, gChickens[chickenIndex].coordinates.z);
        
        gChickens[chickenIndex].unk_17 = 0;
        gChickens[chickenIndex].flags |= 4;
        
    }
    
    func_8008CF94(chickenIndex);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008B55C);

void func_8008B55C(u8 index) {

    if ((gFarmAnimals[index].flags & 1) && (D_80189054 != index) && (gFarmAnimals[index].location == gBaseMapIndex)) {
        
        gFarmAnimals[index].entityIndex = index + 21;
        
        switch (gFarmAnimals[index].type) {

            case BABY_COW:
                initializeAnimalEntity(gFarmAnimals[index].entityIndex, (void*)0x802B1E40, (void*)0x802B1F40, (void*)0x802B2E40, (void*)0x802B2F40);
                loadEntity(gFarmAnimals[index].entityIndex, 0x45, TRUE);
                break;
            case CALF:
                initializeAnimalEntity(gFarmAnimals[index].entityIndex, (void*)0x802B0940, (void*)0x802B0A40, (void*)0x802B1940, (void*)0x802B1A40);
                loadEntity(gFarmAnimals[index].entityIndex, 0x46, TRUE);
                break;
            case ADULT_COW:
                initializeAnimalEntity(gFarmAnimals[index].entityIndex, (void*)0x802779C0, (void*)0x80277AC0, (void*)0x8027A6C0, (void*)0x8027A8C0);
                loadEntity(gFarmAnimals[index].entityIndex, 0x47, TRUE);
                break;
            case PREGNANT_COW:
                initializeAnimalEntity(gFarmAnimals[index].entityIndex, (void*)0x802B3140, (void*)0x802B3240, (void*)0x802B4240, (void*)0x802B4340);
                loadEntity(gFarmAnimals[index].entityIndex, 0x57, TRUE);
                break;
            case BABY_SHEEP:
                initializeAnimalEntity(gFarmAnimals[index].entityIndex, (void*)0x802B5A40, (void*)0x802B5B40, (void*)0x802B6240, (void*)0x802B6340);
                loadEntity(gFarmAnimals[index].entityIndex, 0x48, TRUE);
                break;
            case ADULT_SHEEP:
            case SHEARED_SHEEP:
                initializeAnimalEntity(gFarmAnimals[index].entityIndex, (void*)0x802B4540, (void*)0x802B4640, (void*)0x802B5640, (void*)0x802B5740);
                loadEntity(gFarmAnimals[index].entityIndex, 0x49, TRUE);
                break;

        }

        if (gFarmAnimals[index].location == BARN) {
            setFarmAnimalLocation(index);
        }
        
        setEntityCollidable(gFarmAnimals[index].entityIndex, TRUE);
        setEntityYMovement(gFarmAnimals[index].entityIndex, TRUE);
        func_8002FF38(gFarmAnimals[index].entityIndex, FALSE);

        setEntityDirection(gFarmAnimals[index].entityIndex, convertSpriteToWorldDirection(gFarmAnimals[index].direction, MAIN_MAP_INDEX));
        setEntityCoordinates(gFarmAnimals[index].entityIndex, gFarmAnimals[index].coordinates.x, gFarmAnimals[index].coordinates.y, gFarmAnimals[index].coordinates.z);
    
        func_800300F8(gFarmAnimals[index].entityIndex, FALSE);
    
        gFarmAnimals[index].flags |= 0x4;
        gFarmAnimals[index].flags &= ~(0x200 | 0x400);
        
        if ((gWeather == SUNNY) || !(getLevelFlags(gFarmAnimals[index].location) & 2)) {
            gFarmAnimals[index].unk_1B = 0;
        } else {
            gFarmAnimals[index].unk_1B = 19;
        }
    
    }
    
    func_8008DAA0(index);

}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008B9AC);

void func_8008B9AC(void) {

    if ((horseInfo.flags & 1) && (horseInfo.location == gBaseMapIndex) && !(horseInfo.flags & 8)) {
        
        horseInfo.entityIndex = 0x26;
        
        switch (horseInfo.grown) {                      
            case FALSE:
                loadEntity(0x26, 0x41, TRUE);
                break;
            case TRUE:
                loadEntity(0x26, 0x42, TRUE);
                break;
        }
        
        setEntityCollidable(horseInfo.entityIndex, TRUE);
        setEntityYMovement(horseInfo.entityIndex, TRUE);
        func_8002FF38(horseInfo.entityIndex, FALSE);
        
        setEntityDirection(horseInfo.entityIndex, convertSpriteToWorldDirection(horseInfo.direction, MAIN_MAP_INDEX));
        setEntityCoordinates(horseInfo.entityIndex, horseInfo.coordinates.x, horseInfo.coordinates.y, horseInfo.coordinates.z);
        
        horseInfo.unk_17 = 0;
        horseInfo.flags |= 4;

    }
    
    updateHorse();

}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008BAF0);

void func_8008BAF0(u8 index, u8 arg1) {

    if ((gMiscAnimals[index].flags & 1) && (index < 7)) {
        
        switch (gMiscAnimals[index].animalType) {
    
            case 0:
                initializeAnimalEntity(gMiscAnimals[index].entityIndex, (void*)0x80270210, (void*)0x80270410, (void*)0x80272000, (void*)0x80272200);
                setEntityPaletteIndex(gMiscAnimals[index].entityIndex, 2);
                break;
            
            case 5:
                initializeAnimalEntity(gMiscAnimals[index].entityIndex, (void*)0x802779C0, (void*)0x80277AC0, (void*)0x8027A6C0, (void*)0x8027A8C0);
                break;
            
            default:
                
                switch (index) {
                    
                    case 0:
                        initializeAnimalEntity(gMiscAnimals[index].entityIndex, (void*)0x80284130, (void*)0x80284230, (void*)0x802852B0, (void*)0x802854B0);
                        break;
                    case 1:
                        initializeAnimalEntity(gMiscAnimals[index].entityIndex, (void*)0x80285930, (void*)0x80285A30, (void*)0x80286AB0, (void*)0x80286CB0);
                        break;
                    case 2:
                        initializeAnimalEntity(gMiscAnimals[index].entityIndex, (void*)0x80287130, (void*)0x80287230, (void*)0x802882B0, (void*)0x802884B0);
                        break;
                    case 3:
                        initializeAnimalEntity(gMiscAnimals[index].entityIndex, (void*)0x80288930, (void*)0x80288A30, (void*)0x80289AB0, (void*)0x80289CB0);
                        break;
                    case 4:
                        initializeAnimalEntity(gMiscAnimals[index].entityIndex, (void*)0x8028A130, (void*)0x8028A230, (void*)0x8028B2B0, (void*)0x8028B4B0);
                        break;
                    }

                if (gMiscAnimals[index].animalType == 4) {
                    setEntityPaletteIndex(gMiscAnimals[index].entityIndex, 0);
                }
                
                break;
            
        }
        
        switch (gMiscAnimals[index].animalType) {
        
            case 0:
                loadEntity(gMiscAnimals[index].entityIndex, 0x40, TRUE);
                break;
            case 1:
                loadEntity(gMiscAnimals[index].entityIndex, 0x3F, TRUE);
                break;
            case 2:
                loadEntity(gMiscAnimals[index].entityIndex, 0x3E, TRUE);
                break;
            case 3:
            case 4:
                loadEntity(gMiscAnimals[index].entityIndex, 0x58, TRUE);
                break;
            case 6:
                loadEntity(gMiscAnimals[index].entityIndex, 0x49, TRUE);
                break;
            case 5:
                loadEntity(gMiscAnimals[index].entityIndex, 0x47, TRUE);
                break;
            case 7:
                loadEntity(gMiscAnimals[index].entityIndex, 0x38, TRUE);
                break;
            case 8:
                loadEntity(gMiscAnimals[index].entityIndex, 0x36, TRUE);
                break;
            case 9:
                loadEntity(gMiscAnimals[index].entityIndex, 0x3C, TRUE);
                break;
            case 10:
                loadEntity(gMiscAnimals[index].entityIndex, 0x39, TRUE);
                break;
            case 11:
                loadEntity(gMiscAnimals[index].entityIndex, 0x3D, TRUE);
                setEntityPaletteIndex(gMiscAnimals[index].entityIndex, 0);
                break;
            case 12:
                loadEntity(gMiscAnimals[index].entityIndex, 0x3D, TRUE);
                break;
            case 13:
                loadEntity(gMiscAnimals[index].entityIndex, 0x4B, TRUE);
                break;
            case 14:
                loadEntity(gMiscAnimals[index].entityIndex, 0x54, TRUE);
                break;
            case 15:
                loadEntity(gMiscAnimals[index].entityIndex, 0x50, TRUE);
                break;
            case 16:
                loadEntity(gMiscAnimals[index].entityIndex, 0x51, TRUE);
                break;
            case 17:
                loadEntity(gMiscAnimals[index].entityIndex, 0x4F, TRUE);
                break;
            case 18:
                loadEntity(gMiscAnimals[index].entityIndex, 0x4C, TRUE);
                break;
            case 19:
                loadEntity(gMiscAnimals[index].entityIndex, 0x4D, TRUE);
                break;
            case 20:
                loadEntity(gMiscAnimals[index].entityIndex, 0x52, TRUE);
                break;
            case 21:
                loadEntity(gMiscAnimals[index].entityIndex, 0x4E, TRUE);
                break;
        }

        setEntityCollidable(gMiscAnimals[index].entityIndex, TRUE);
        setEntityYMovement(gMiscAnimals[index].entityIndex, TRUE);
        func_8002FF38(gMiscAnimals[index].entityIndex, FALSE);
        
        setEntityDirection(gMiscAnimals[index].entityIndex, convertSpriteToWorldDirection(gMiscAnimals[index].direction, MAIN_MAP_INDEX));
        setEntityCoordinates(gMiscAnimals[index].entityIndex, gMiscAnimals[index].coordinates.x, gMiscAnimals[index].coordinates.y, gMiscAnimals[index].coordinates.z);
        func_800300F8(gMiscAnimals[index].entityIndex, FALSE);
        
        gMiscAnimals[index].unk_F = arg1;
        gMiscAnimals[index].flags |= 4;
    
    }
    
    func_8009476C(index);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", updateDog);

void updateDog(void) {

    Vec3f vec;

    if ((dogInfo.flags & 1) && (dogInfo.flags & 4)) {
        
        if (checkEntityAnimationStateChanged(dogInfo.entityIndex) || dogInfo.flags & 0x20) {

            if (dogInfo.unk_1A == 0) {
                func_8008C358();
            } else {
                dogInfo.unk_1A--;
            }
            
            dogInfo.flags &= ~0x20;
            
        }
        
        setEntityDirection(dogInfo.entityIndex, convertSpriteToWorldDirection(dogInfo.direction, MAIN_MAP_INDEX));
        vec = getMovementVectorFromDirection(dogInfo.speed, dogInfo.direction, 0.0f);
        setEntityMovementVector(dogInfo.entityIndex, vec.x, vec.y, vec.z, dogInfo.speed);

    } 
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008C358);

void func_8008C358(void) {

    u16 temp;
    
    switch (dogInfo.unk_17) {

        case 0:
            
            dogInfo.speed = 0;
            dogInfo.unk_1A = 10;
            dogInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 0);
            
            if (dogInfo.flags & 0x10) {
                
                dogInfo.direction = calculateAnimalDirectionToPlayer(entities[dogInfo.entityIndex].coordinates.x, entities[dogInfo.entityIndex].coordinates.z, entities[ENTITY_PLAYER].coordinates.x, entities[ENTITY_PLAYER].coordinates.z);
                dogInfo.unk_17 = 4;
                
            } else {

                if (getRandomNumberInRange(0, 60) < 40) {
                    dogInfo.unk_17 = 0;
                } else {
    
                    temp = getRandomNumberInRange(0, 20);
                    
                    if (temp < 7) {
                        dogInfo.direction = temp;
                    }
                    
                    temp = getRandomNumberInRange(0, 10);
    
                    if (temp == 0) {
                        dogInfo.unk_17 = 1;
                    }
                    if (temp == 1) {
                        dogInfo.unk_17 = 2;
                    }
                    if (temp == 2) {
                        dogInfo.unk_17 = 3;
                    }
                    if (temp == 3) {
                        dogInfo.unk_17 = 4;
                    }
                    if (temp == 4) {
                        dogInfo.unk_17 = 5;
                    }
                    if (temp == 5) {
                        dogInfo.unk_17 = 6;
                    }
                    if (temp == 6) {
                        dogInfo.unk_17 = 7;
                    }
                    if (temp == 7) {
                        dogInfo.unk_17 = 8;
                    }
                    if (temp == 8) {
                        dogInfo.unk_17 = 0xA;
                    }
                    if (temp == 9) {
                        dogInfo.unk_17 = 0x18;
                    }
                    if (temp == 10) {
                        dogInfo.unk_17 = 0x19;
                    }
                    
                }
                    
            } 

            dogInfo.flags |= 2;
            
            break;

        case 1:
    
            dogInfo.speed = 1;
            dogInfo.unk_1A = 10;
            dogInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 8);
            
            temp = getRandomNumberInRange(0, 19);
            
            if (temp < 10) {
                dogInfo.unk_17 = 1;
            } else if (temp >= 15) {
                dogInfo.unk_17 = 4;
            } else {
                dogInfo.unk_17 = 0;
            }

            dogInfo.flags |= 2;
            
            break;

        case 2:

            dogInfo.speed = 0;
            dogInfo.unk_1A = 0;
            dogInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 0x30);
            dogInfo.unk_17 = 0x10;
            
            dogInfo.flags |= 2;
            
            break;

        case 3:

            dogInfo.speed = 0;
            dogInfo.unk_1A = 10;
            dogInfo.unk_1B = 0;

            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 0x20);
    
            temp = getRandomNumberInRange(0, 20);
            
            if (temp < 10) {
                
                dogInfo.unk_17 = 3;
                
            } else {

                if (9 < temp && temp < 12) {
                    dogInfo.unk_17 = 0;
                }

                if (11 < temp && temp < 14) {
                    dogInfo.unk_17 = 11;
                }

                if (13 < temp && temp < 16) {
                    dogInfo.unk_17 = 12;
                }
                
                if (15 < temp && temp < 18) {
                    dogInfo.unk_17 = 13;
                }
                
                if (17 < temp && temp < 20) {
                    dogInfo.unk_17 = 14;
                }
               
            }
            
            dogInfo.flags |= 2;

            break;
        
        case 4:
            
            dogInfo.speed = 2;
            dogInfo.unk_1A = 0;
            dogInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 0x10);
            
            if (dogInfo.flags & 0x10) {
                
                dogInfo.direction = calculateAnimalDirectionToPlayer(entities[dogInfo.entityIndex].coordinates.x, entities[dogInfo.entityIndex].coordinates.z, entities[ENTITY_PLAYER].coordinates.x, entities[ENTITY_PLAYER].coordinates.z);
                
                if (!(getRandomNumberInRange(0, 20))) {
                    dogInfo.flags &= ~0x10;
                }
                
            } else {

                temp = getRandomNumberInRange(0, 19);

                if (temp < 10) {
                    
                    dogInfo.unk_17 = 4;
                    
                } else {
    
                    if (9 < temp && temp < 13) {
                        dogInfo.unk_17 = 0;
                    }
    
                    if (12 < temp && temp < 16) {
                        dogInfo.unk_17 = 1;
                    }
    
                    if (15 < temp && temp < 19) {
                        dogInfo.unk_17 = 21;
                    }
                   
                }
            
            }
            
            dogInfo.flags |= 2;

            break;

        case 5:
            
            dogInfo.speed = 0;
            dogInfo.unk_1A = 0;
            dogInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 0x90);
            
            if (!getRandomNumberInRange(0, 2)) {
                dogInfo.unk_17 = 5;
            } else {
                dogInfo.unk_17 = 3;
            }
    
            dogInfo.flags |= 2;

            break;


        case 6:

            dogInfo.speed = 0;
            dogInfo.unk_1A = 0;
            dogInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 0xA0);
            
            if (!getRandomNumberInRange(0, 2)) {
                dogInfo.unk_17 = 6;
            } else {
                dogInfo.unk_17 = 0;
            }
    
            dogInfo.flags |= 2;
            
            break;

        case 7:

            dogInfo.speed = 0;
            dogInfo.unk_1A = 0;
            dogInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 0xA8);
            
            if (!getRandomNumberInRange(0, 2)) {
                dogInfo.unk_17 = 7;
            } else {
                dogInfo.unk_17 = 0;
            }
    
            dogInfo.flags |= 2;
            
            break;
        
        case 8:

            dogInfo.speed = 0;
            dogInfo.unk_1A = 0;
            dogInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 0xB0);
            
            if (getRandomNumberInRange(0, 2)) {
                dogInfo.unk_17 = 0;
            } else {
                dogInfo.unk_17 = 8;
            }
    
            dogInfo.flags |= 2;
            
            break;
        
        case 9:
            
            dogInfo.speed = 0;
            dogInfo.unk_1A = 0;
            dogInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 0xB8);
            
            dogInfo.unk_17 = 26;
            
            break;
        
        case 10:
            
            dogInfo.speed = 0;
            dogInfo.unk_1A = 10;
            dogInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 0xC8);

            if (!getRandomNumberInRange(0, 2)) {
                dogInfo.unk_17 = 10;
            } else {
                dogInfo.unk_17 = 0;
            }

            dogInfo.flags |= 2;
            
            break;
                
        case 11:
            
            dogInfo.speed = 0;
            dogInfo.unk_1A = 10;
            dogInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 0x28);

            if (!getRandomNumberInRange(0, 2)) {
                dogInfo.unk_17 = 11;
            } else {
                dogInfo.unk_17 = 3;
            }

            dogInfo.flags |= 2;
            
            break;
                        
        case 12:
            
            dogInfo.speed = 0;
            dogInfo.unk_1A = 10;
            dogInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 0x50);

            if (!getRandomNumberInRange(0, 2)) {
                dogInfo.unk_17 = 12;
            } else {
                dogInfo.unk_17 = 3;
            }

            dogInfo.flags |= 2;
            
            break;
                        
        case 13:
            
            dogInfo.speed = 0;
            dogInfo.unk_1A = 0;
            dogInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 0x58);

            if (!getRandomNumberInRange(0, 2)) {
                dogInfo.unk_17 = 13;
            } else {
                dogInfo.unk_17 = 3;
            }

            dogInfo.flags |= 2;
            
            break;
                        
        case 14:
            
            dogInfo.speed = 0;
            dogInfo.unk_1A = 0;
            dogInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 0x60);

            if (!getRandomNumberInRange(0, 2)) {
                dogInfo.unk_17 = 14;
            } else {
                dogInfo.unk_17 = 3;
            }

            dogInfo.flags |= 2;
            
            break;

        case 16:
            
            dogInfo.speed = 0;
            dogInfo.unk_1A = 10;
            dogInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 0x28);

            temp = getRandomNumberInRange(0, 14);

            if (temp < 10) {
                dogInfo.unk_17 = 16;
            }
            if (temp == 10) {
                dogInfo.unk_17 = 27;
            }
            if (temp == 11) {
                dogInfo.unk_17 = 17;
            }
            if (temp == 12) {
                dogInfo.unk_17 = 18;
            }
            if (temp == 13) {
                dogInfo.unk_17 = 19;
            }
            if (temp == 14) {
                dogInfo.unk_17 = 20;
            }

            dogInfo.flags |= 2;
            
            break;
                        
        case 17:
            
            dogInfo.speed = 0;
            dogInfo.unk_1A = 0;
            dogInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 0x68);

            if (!getRandomNumberInRange(0, 2)) {
                dogInfo.unk_17 = 17;
            } else {
                dogInfo.unk_17 = 16;
            }

            dogInfo.flags |= 2;
            
            break;
                        
        case 18:
            
            dogInfo.speed = 0;
            dogInfo.unk_1A = 0;
            dogInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 0x70);

            if (!getRandomNumberInRange(0, 2)) {
                dogInfo.unk_17 = 18;
            } else {
                dogInfo.unk_17 = 16;
            }

            dogInfo.flags |= 2;
            
            break;

        case 19:
            
            dogInfo.speed = 0;
            dogInfo.unk_1A = 0;
            dogInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 0x78);

            if (!getRandomNumberInRange(0, 2)) {
                dogInfo.unk_17 = 19;
            } else {
                dogInfo.unk_17 = 16;
            }

            dogInfo.flags |= 2;
            
            break;

        
        case 20:
            
            dogInfo.speed = 0;
            dogInfo.unk_1A = 10;
            dogInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 0x80);

            if (!getRandomNumberInRange(0, 2)) {
                dogInfo.unk_17 = 20;
            } else {
                dogInfo.unk_17 = 16;
            }

            dogInfo.flags |= 2;
            
            break;

        case 21:
            
            dogInfo.speed = 1;
            dogInfo.unk_1A = 0;
            dogInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 0x38);
            
            dogInfo.unk_17 = 22;
            dogInfo.flags |= 2;
            
            break;        
        
        case 22:
            
            dogInfo.speed = 2;
            dogInfo.unk_1A = 0;
            dogInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 0x40);
            
            dogInfo.unk_17 = 23;
            dogInfo.flags |= 2;
            
            break;     
        
        case 23:
            
            dogInfo.speed = 1;
            dogInfo.unk_1A = 0;
            dogInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 0x48);
            
            dogInfo.unk_17 = 0;
            dogInfo.flags |= 2;
            
            break;     
                                
        case 24:
            
            dogInfo.speed = 0;
            dogInfo.unk_1A = 0;
            dogInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 0x88);

            if (!getRandomNumberInRange(0, 2)) {
                dogInfo.unk_17 = 24;
            } else {
                dogInfo.unk_17 = 0;
            }

            dogInfo.flags |= 2;
            
            break;
                                
        case 25:
            
            dogInfo.speed = 0;
            dogInfo.unk_1A = 0;
            dogInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 0x98);

            if (!getRandomNumberInRange(0, 2)) {
                dogInfo.unk_17 = 25;
            } else {
                dogInfo.unk_17 = 0;
            }

            dogInfo.flags |= 2;
            
            break;

        case 26:
            
            dogInfo.speed = 0;
            dogInfo.unk_1A = 0;
            dogInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 0xC0);
            
            dogInfo.unk_17 = 0;
            dogInfo.flags |= 2;
            
            break;     
        
        case 27:
            
            dogInfo.speed = 0;
            dogInfo.unk_1A = 0;
            dogInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 0x30);
            
            dogInfo.unk_17 = 0;
            dogInfo.flags |= 2;
            
            break;     
        
        case 32:
            
            dogInfo.speed = 0;
            dogInfo.unk_1A = 0;
            dogInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 0);
            
            dogInfo.flags |= 2;
            
            break;     

        case 34:
            dogInfo.speed = 0;
            dogInfo.unk_1A = 0;
            dogInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(dogInfo.entityIndex, 0x98);
            
            dogInfo.unk_17 = 0;
            dogInfo.flags |=  2;
            
            playSfx(0x3C);
            
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008CF94);

void func_8008CF94(u8 index) {
    
    Vec3f vec;

    if (gChickens[index].flags & 1 && gChickens[index].flags & 4) {
    
        if ((checkEntityAnimationStateChanged(gChickens[index].entityIndex) || gChickens[index].flags & 0x40)) {
    
            switch(gChickens[index].type) {
    
                case ADULT_CHICKEN:
    
                    switch(gChickens[index].condition) {
                        case CHICKEN_NORMAL:
                            func_8008D1C0(index);
                            break;
                        case CHICKEN_STARVED:
                            func_8008D650(index);
                            break;
                    }
        
                    break;
    
                case CHICK:
                    func_8008D70C(index);
                    break;
        
                case CHICKEN_EGG:
                    func_8008DA00(index);
                    break;
    
                }

            gChickens[index].flags &= 0xFFBF;
    
        }

        setEntityDirection(gChickens[index].entityIndex, convertSpriteToWorldDirection(gChickens[index].direction, MAIN_MAP_INDEX));
        
        vec = getMovementVectorFromDirection(gChickens[index].unk_19, gChickens[index].direction, 0);
        setEntityMovementVector(gChickens[index].entityIndex, vec.x, vec.y, vec.z, gChickens[index].unk_19);
    
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008D1C0);

void func_8008D1C0(u8 index) {

    u16 temp;

    switch (gChickens[index].unk_17) {
        
        case 0:
            
            gChickens[index].unk_19 = 0;
            gChickens[index].unk_1A = 0;
            gChickens[index].unk_1B = 0;

            setEntityAnimationWithDirectionChange(gChickens[index].entityIndex, 0);

            if (getRandomNumberInRange(0, 100) < 40) {
                gChickens[index].unk_17 = 0;
            } else {

                temp = getRandomNumberInRange(0, 10);
                
                if (temp < 7) {
                    gChickens[index].direction = temp;
                }
                
                temp = getRandomNumberInRange(0, 60);
                
                if (temp < 20) {
                    gChickens[index].unk_17 = 1;
                }
                if (19 < temp && temp < 40) {
                    gChickens[index].unk_17 = 2;
                }
                if (39 < temp && temp < 60) {
                    gChickens[index].unk_17 = 3;
                }
            
            }

            gChickens[index].flags |= 2;
            
            break;
            
        case 1:
            gChickens[index].unk_19  = 1;
            gChickens[index].unk_1A = 0;
            gChickens[index].unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(gChickens[index].entityIndex, 8);
            
            if (getRandomNumberInRange(0, 20) < 10) {
                gChickens[index].unk_17 = 1;
            } else {
                gChickens[index].unk_17 = 0;
            }
            
            gChickens[index].flags |= 2;
            playSfx(0x40);
            
            break;

        case 2:
            gChickens[index].unk_19 = 0;
            gChickens[index].unk_1A = 0;
            gChickens[index].unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(gChickens[index].entityIndex, 0x10);

            gChickens[index].unk_17 = 0;
            gChickens[index].flags |= 2;
            
            break;

        case 3:

            gChickens[index].unk_19 = 0;
            gChickens[index].unk_1A = 0;
            gChickens[index].unk_1B = 0;

            setEntityAnimationWithDirectionChange(gChickens[index].entityIndex, 0x20);
        
            if (getRandomNumberInRange(0,19) < 10) {
                gChickens[index].unk_17 = 0;      
            } else {
                gChickens[index].unk_17 = 4;
            }

            gChickens[index].flags |= 2;
            
            break;

        case 4:
            gChickens[index].unk_19 = 0;
            gChickens[index].unk_1A = 0;
            gChickens[index].unk_1B = 0;

            setEntityAnimationWithDirectionChange(gChickens[index].entityIndex, 0x18);
            
            if (getRandomNumberInRange(0, 19) < 10) {
                gChickens[index].unk_17 = 4;
            } else {
                gChickens[index].unk_17 = 3;
            }
        
            gChickens[index].flags |= 2;
            
            break;
        
        case 16:
            gChickens[index].unk_19 = 0;
            gChickens[index].unk_1A = 0;
            gChickens[index].unk_1B = 0;

            setEntityAnimationWithDirectionChange(gChickens[index].entityIndex, 0);
            
            gChickens[index].flags |= 2;

            break;

        case 17:
            gChickens[index].unk_19 = 0;
            gChickens[index].unk_1A = 0;
            gChickens[index].unk_1B = 0;

            setEntityAnimationWithDirectionChange(gChickens[index].entityIndex, 0x30);
            
            gChickens[index].unk_17 = 0;
            gChickens[index].flags |=  2;
            
            playSfx(0x41);
            
            break;
        
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008D650);

void func_8008D650(u8 chickenIndex) {

    u16 temp;
    
    gChickens[chickenIndex].unk_19 = 0;
    gChickens[chickenIndex].unk_1A = 0;
    gChickens[chickenIndex].unk_1B = 0;

    setEntityAnimationWithDirectionChange(gChickens[chickenIndex].entityIndex, 0x28);

    temp = getRandomNumberInRange(0, 1000);
    
    if (temp < 7) {
        gChickens[chickenIndex].direction = temp;
    }
    
    gChickens[chickenIndex].unk_17 = 0;
    gChickens[chickenIndex].flags |= 2;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008D70C);

void func_8008D70C(u8 index) {

    u16 temp ;

    switch (gChickens[index].unk_17) {

        case 0:
            gChickens[index].unk_19 = 0;
            gChickens[index].unk_1A = 0;
            gChickens[index].unk_1B = 0;

            setEntityAnimationWithDirectionChange(gChickens[index].entityIndex, 0);

            if (getRandomNumberInRange(0, 100) < 40) {
                gChickens[index].unk_17 = 0;
            } else {
                
                temp = getRandomNumberInRange(0, 10);
                
                if (temp  < 7) {
                    gChickens[index].direction = temp;
                }
                
                temp = getRandomNumberInRange(0, 40);

                if (temp  < 20) {
                    gChickens[index].unk_17 = 1;
                }
                if (19 < temp  && temp  < 40) {
                    gChickens[index].unk_17 = 2;
                }
            }
            
            gChickens[index].flags |= 2;

            break;

        case 1:
            gChickens[index].unk_19 = 1;
            gChickens[index].unk_1A = 0;
            gChickens[index].unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(gChickens[index].entityIndex, 8);
            
            if (getRandomNumberInRange(0, 20) < 10) {
                gChickens[index].unk_17 = 1;
            } else {
                gChickens[index].unk_17 = 0;
            }
            
            gChickens[index].flags |= 2;
            
            playSfx(0x42);
            
            break;

        case 2:
            gChickens[index].unk_19 = 0;
            gChickens[index].unk_1A = 0;
            gChickens[index].unk_1B = 0;
        
            setEntityAnimationWithDirectionChange(gChickens[index].entityIndex, 16);
        
            gChickens[index].unk_17 = 0;
            gChickens[index].flags |= 2;
        
            break;

        case 16:
            gChickens[index].unk_19 = 0;
            gChickens[index].unk_1A = 0;
            gChickens[index].unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(gChickens[index].entityIndex, 0);

            gChickens[index].flags |= 2;
        
            break;
        
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008DA00);

void func_8008DA00(u8 chickenIndex) {

    switch (gChickens[chickenIndex].unk_17) {

        case 0:
        case 16:
            gChickens[chickenIndex].unk_19 = 0;
            gChickens[chickenIndex].unk_1A = 0;
            gChickens[chickenIndex].unk_1B = 0;
            setEntityAnimation(gChickens[chickenIndex].entityIndex, 0x13);
            gChickens[chickenIndex].flags |= 2;
            
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008DAA0);

void func_8008DAA0(u8 index) {

    Vec3f vec;
    u8 levelInteractionIndex;

    if ((gFarmAnimals[index].flags & 1) && (gFarmAnimals[index].flags & 4)) {

        if ((checkEntityAnimationStateChanged(gFarmAnimals[index].entityIndex)) || (gFarmAnimals[index].flags & 0x8000)) {
            
            if (entities[gFarmAnimals[index].entityIndex].unk_5E == 0) {

                if (!(gFarmAnimals[index].flags & 0x2000)) {

                    gFarmAnimals[index].direction = gPlayer.direction;
                    gFarmAnimals[index].unk_1B = 1;

                    gFarmAnimals[index].flags &= ~(0x200);
                    gFarmAnimals[index].flags |= (0x400 | 0x2000);

                }

            }
            
            switch (gFarmAnimals[index].type) {

                case BABY_COW:
                    func_80090DF8(index);
                    break;

                case CALF:
                    func_800904E4(index);
                    break;

                case ADULT_COW:

                    switch(gFarmAnimals[index].condition) {

                        case COW_NORMAL:
                            func_8008DF9C(index);
                            break;

                        case COW_HAPPY:
                            func_8008E98C(index);
                            break;

                        case COW_MAD:
                            func_8008F37C(index);
                            break;

                        case COW_SICK:
                            func_8008FC68(index);
                            break;

                    }
                    
                    break;

                case PREGNANT_COW:
                    func_8009170C(index);
                    break;

                case BABY_SHEEP:
                    func_80093434(index);
                    break;

                case ADULT_SHEEP:

                    switch(gFarmAnimals[index].condition) {

                        case SHEEP_NORMAL:
                            func_80092094(index);
                            break;

                        case SHEEP_SICK:
                            func_80092808(index);
                            break;

                    }
    
                    break;

                case SHEARED_SHEEP:

                    switch(gFarmAnimals[index].condition) {

                        case SHEEP_NORMAL:
                            func_80092A64(index);
                            break;

                        case SHEEP_SICK:
                            func_800931D8(index);
                            break;

                    }
    
                    break;

                }

            gFarmAnimals[index].flags &= ~(0x2000 | 0x8000);

        }
       
        setEntityDirection(gFarmAnimals[index].entityIndex, convertSpriteToWorldDirection(gFarmAnimals[index].direction, MAIN_MAP_INDEX));
        
        vec = getMovementVectorFromDirection(gFarmAnimals[index].unk_14, gFarmAnimals[index].direction, 0.0f);
        setEntityMovementVector(gFarmAnimals[index].entityIndex, vec.x, vec.y, vec.z, gFarmAnimals[index].unk_14);
       
        levelInteractionIndex = getLevelInteractionIndexFromEntityPosition(gFarmAnimals[index].entityIndex, 0, 32);
        
        switch(gFarmAnimals[index].location) {

            case FARM_SPRING:

                if (levelInteractionIndex == 6) {

                    gFarmAnimals[index].location = BARN;
                    deactivateEntity(gFarmAnimals[index].entityIndex);

                    gFarmAnimals[index].flags &= ~(0x4);
                    func_80038990(MAIN_MAP_INDEX, 0x1B, 0);  

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

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008DF9C);

void func_8008DF9C(u8 index) {
    
    u16 temp;

    switch (gFarmAnimals[index].unk_1B) {
        
        case 0:
        
            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;
            
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0);
            
            if ((gFarmAnimals[index].flags & 0x200) || (gFarmAnimals[index].flags & 0x400)) {
                
                gFarmAnimals[index].direction = calculateAnimalDirectionToPlayer(entities[gFarmAnimals[index].entityIndex].coordinates.x, entities[gFarmAnimals[index].entityIndex].coordinates.z, entities[ENTITY_PLAYER].coordinates.x, entities[ENTITY_PLAYER].coordinates.z);
                gFarmAnimals[index].unk_1B = 1;
                
            } else {
                
                if (getRandomNumberInRange(0, 60) < 40) {
                    gFarmAnimals[index].unk_1B = 0;
                } else {
                    
                    temp = getRandomNumberInRange(0, 20);
                    
                    if (temp < 7) {
                        gFarmAnimals[index].direction = temp;
                    }
        
                    temp = getRandomNumberInRange(0, 5);
                    
                    if (temp == 0) {
                        gFarmAnimals[index].unk_1B = 1;
                    }
                    
                    if (temp == 1) {
                        gFarmAnimals[index].unk_1B = 2;
                    }
                    
                    if (temp == 2) {
                        gFarmAnimals[index].unk_1B = 3;
                    }
                    
                    if (temp == 3) {
                        gFarmAnimals[index].unk_1B = 4;
                    }
                    
                    if (temp == 4) {
                        gFarmAnimals[index].unk_1B = 5;
                    }
                    
                    if (temp == 5) {
                        gFarmAnimals[index].unk_1B = 6;
                    }
                    
                }
                    
            }
                
            gFarmAnimals[index].flags |= 2;
                
            break;
            
        case 1:
            
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;
            gFarmAnimals[index].unk_14 = 1.0f;
            
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 8);
            
            if (gFarmAnimals[index].flags & 0x200) {
            
                gFarmAnimals[index].flags &= ~0x400;
                gFarmAnimals[index].direction = calculateAnimalDirectionToPlayer(entities[gFarmAnimals[index].entityIndex].coordinates.x, entities[gFarmAnimals[index].entityIndex].coordinates.z, entities[ENTITY_PLAYER].coordinates.x, entities[ENTITY_PLAYER].coordinates.z);
                
                if (!(getRandomNumberInRange(0, 20))) {
                    gFarmAnimals[index].flags &= ~(0x200);
                }
                
            } else if (gFarmAnimals[index].flags & 0x400) {
                
                if (!(getRandomNumberInRange(0, 40))) {
                    gFarmAnimals[index].flags &= ~(0x400);
                }
                
            } else {
                
                if (getRandomNumberInRange(0, 20) < 10) {
                    gFarmAnimals[index].unk_1B = 1;
                } else {
                    gFarmAnimals[index].unk_1B = 0;
                }
            
            }
            
            gFarmAnimals[index].flags |= 2;
            
            break;
            
        case 2:
            
            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;
            
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 32);
            
            temp = getRandomNumberInRange(0, 2);
            
            if (temp == 0) {
                gFarmAnimals[index].unk_1B = 2;
            }
            
            if (temp == 1) {
                gFarmAnimals[index].unk_1B = 0;
            }
            
            if (temp == 2) {
                gFarmAnimals[index].unk_1B = 9;
            }
            
            gFarmAnimals[index].flags |= 2;
            
            break;
            
        case 3:
            
            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;
            
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x10);
            
            gFarmAnimals[index].unk_1B = 7;
            gFarmAnimals[index].flags |= 2;
            
            break;
            
        case 4:
            
            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;
            
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x38);
            
            gFarmAnimals[index].unk_1B = 0;
            gFarmAnimals[index].flags |= 2;
            
            break;
            
        case 5:
            
            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;
            
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x40);
            
            gFarmAnimals[index].unk_1B = 0;
            gFarmAnimals[index].flags |= 2;
            
            break;
            
        case 6:
            
            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;
            
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x48);
            
            gFarmAnimals[index].unk_1B = 0;
            gFarmAnimals[index].flags |= 2;
            
            break;
            
        case 7:
            
            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;
            
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x18);
            
            temp = getRandomNumberInRange(0, 11);
            
            if (temp < 10) {
                gFarmAnimals[index].unk_1B = 7;
            }
            if (temp == 10) {
                gFarmAnimals[index].unk_1B = 8;
            }
            if (temp == 11) {
                gFarmAnimals[index].unk_1B = 10;
            }
            
            gFarmAnimals[index].flags |= 2;
            
            break;
            
        case 8:
            
            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;
            
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x28);
            
            if (getRandomNumberInRange(0, 4) < 4) {
                gFarmAnimals[index].unk_1B = 8;
            } else {
                gFarmAnimals[index].unk_1B = 7;
            }
            
            gFarmAnimals[index].flags |= 2;
            
            break;
            
        case 9:
            
            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;
            
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x30);
            
            gFarmAnimals[index].unk_1B = 2;
            gFarmAnimals[index].flags |= 2;
            
            break;
            
        case 10:
            
            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;
            
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x10);
            
            gFarmAnimals[index].unk_1B = 0;
            gFarmAnimals[index].flags |= 2;
            
            break;
            
        case 16:
            
            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;
            
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0);
            
            gFarmAnimals[index].flags |= (2 | 0x2000);
            gFarmAnimals[index].flags &= ~0x400;
            
            break;
        
        case 17:
            
            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;
            
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x50);
            
            gFarmAnimals[index].unk_1B = 0;
            gFarmAnimals[index].flags |= (2 | 0x2000);
            gFarmAnimals[index].flags &= ~0x400;
            
            playSfx(0x36);
            
            break;
            
        case 18:
            
            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;
            
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x58);
            
            gFarmAnimals[index].unk_1B = 0;
            
            gFarmAnimals[index].flags |= (2 | 0x2000);
            gFarmAnimals[index].flags &= ~0x400;
            
            playSfx(0x37);
            
            break;
            
        case 19:
            
            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;
            
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x60);
            
            gFarmAnimals[index].unk_1B = 19;
            gFarmAnimals[index].flags |= (2 | 0x2000);
            gFarmAnimals[index].flags &= ~0x400;
            
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008E98C);

void func_8008E98C(u8 index) {

    u16 temp;
    u16 tempDirection;
    
    switch (gFarmAnimals[index].unk_1B) {

        case 0:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0xB0);

            if ((gFarmAnimals[index].flags & 0x200) || (gFarmAnimals[index].flags & 0x400)) {

                gFarmAnimals[index].direction = calculateAnimalDirectionToPlayer(
                    entities[gFarmAnimals[index].entityIndex].coordinates.x,
                    entities[gFarmAnimals[index].entityIndex].coordinates.z,
                    entities[ENTITY_PLAYER].coordinates.x, 
                    entities[ENTITY_PLAYER].coordinates.z);

                gFarmAnimals[index].unk_1B = 1;
                
            } else {
        
                if (getRandomNumberInRange(0, 60) < 40) {
                
                    gFarmAnimals[index].unk_1B = 0;

                } else {

                    tempDirection = getRandomNumberInRange(0, 20);

                    if (tempDirection < 7) {
                        gFarmAnimals[index].direction = tempDirection;
                    }

                    temp = getRandomNumberInRange(0, 5);

                    if (temp == 0) {
                        gFarmAnimals[index].unk_1B = 1;
                    }
                    
                    if (temp == 1) {
                        gFarmAnimals[index].unk_1B = 2;
                    }

                    if (temp == 2) {
                        gFarmAnimals[index].unk_1B = 3;
                    }

                    if (temp == 3) {
                        gFarmAnimals[index].unk_1B = 4;
                    }

                    if (temp == 4) {
                        gFarmAnimals[index].unk_1B = 5;
                    }

                    if (temp == 5) {
                        gFarmAnimals[index].unk_1B = 6;
                    }

                }
                
            }
        
            gFarmAnimals[index].flags |= 2;
            break;
        
        case 1:

            gFarmAnimals[index].unk_14 = 1.0f;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0xB8);

            if (gFarmAnimals[index].flags & 0x200) {

                gFarmAnimals[index].flags &= ~0x400;

                gFarmAnimals[index].direction = calculateAnimalDirectionToPlayer(
                    entities[gFarmAnimals[index].entityIndex].coordinates.x,
                    entities[gFarmAnimals[index].entityIndex].coordinates.z,
                    entities[ENTITY_PLAYER].coordinates.x, 
                    entities[ENTITY_PLAYER].coordinates.z);

                if (getRandomNumberInRange(0, 20) == 0) {
                    gFarmAnimals[index].flags &= ~0x200;
                } 
                
            } else if (gFarmAnimals[index].flags & 0x400) {

                 if (getRandomNumberInRange(0, 40) == 0) {
                    gFarmAnimals[index].flags &= ~0x400;
                } 
                
            } else {

                if (getRandomNumberInRange(0, 20) < 10) {
                    gFarmAnimals[index].unk_1B = 1;
                } else {
                    gFarmAnimals[index].unk_1B = 0;
                }
                
            }

            gFarmAnimals[index].flags |= 2;            
            break;

        case 2:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0xD0);

            temp = getRandomNumberInRange(0, 2); 
            
            if (temp == 0) {
                gFarmAnimals[index].unk_1B = 2;
            } 
            if (temp == 1) {
                gFarmAnimals[index].unk_1B = 0;
            }
            if (temp == 2) {
                gFarmAnimals[index].unk_1B = 9;
            }

            gFarmAnimals[index].flags |= 2;  
            break;

        case 3:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0xC0);
            
            gFarmAnimals[index].unk_1B = 7;
            
            gFarmAnimals[index].flags |= 2;  
            break;

        case 4:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0xE8);

            gFarmAnimals[index].unk_1B = 0;

            gFarmAnimals[index].flags |= 2;  
            break;

        case 5:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0xF0);

            gFarmAnimals[index].unk_1B = 0;

            gFarmAnimals[index].flags |= 2;
            break;

        case 6:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0xF8);

            gFarmAnimals[index].unk_1B = 0;

            gFarmAnimals[index].flags |= 2;
            break;

        case 7:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0xC8);

            temp = getRandomNumberInRange(0, 11);

            if (temp < 10) {
                gFarmAnimals[index].unk_1B = 7;
            }

            if (temp == 10) {
                gFarmAnimals[index].unk_1B = 8;
            }

            if (temp == 11) {
                gFarmAnimals[index].unk_1B = 10;
            }

            gFarmAnimals[index].flags |= 2;
            break;

        case 8:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0xD8);

            if (getRandomNumberInRange(0, 4) >= 4) {
                gFarmAnimals[index].unk_1B = 7;
            } else {
                gFarmAnimals[index].unk_1B = 8;
            }

            gFarmAnimals[index].flags |= 2;
            
            break;

        case 9:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0xE0);

            gFarmAnimals[index].unk_1B = 2;

            gFarmAnimals[index].flags |= 2;
            break;

        case 10:
                    
            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0xC0);

            gFarmAnimals[index].unk_1B = 0;

            gFarmAnimals[index].flags |= 2;
            break;
        
        case 16:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0xB0);

            gFarmAnimals[index].flags |= (2 | 0x2000);
            gFarmAnimals[index].flags &= ~(0x400);
            
            break;
        
        case 17:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x50);

            gFarmAnimals[index].unk_1B = 0;

            gFarmAnimals[index].flags |= (2 | 0x2000);
            gFarmAnimals[index].flags &= ~(0x400);

            playSfx(0x36);
            
            break;
        
        case 18:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x100);

            gFarmAnimals[index].unk_1B = 0;

            gFarmAnimals[index].flags |= (2 | 0x2000);
            gFarmAnimals[index].flags &= ~(0x400);

            playSfx(0x37);

            break;
        
        case 19:
            
            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x60);

            gFarmAnimals[index].unk_1B = 19;

            gFarmAnimals[index].flags |= (2 | 0x2000);
            gFarmAnimals[index].flags &= ~(0x400);
            
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008F37C);

void func_8008F37C(u8 index) {

    u16 temp;
    u16 tempDirection;
    
    switch (gFarmAnimals[index].unk_1B) {

        case 0:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x68);

            temp = getRandomNumberInRange(0, 60);

            if ((gFarmAnimals[index].flags & 0x200) || (gFarmAnimals[index].flags & 0x400)) {

                gFarmAnimals[index].direction = calculateAnimalDirectionToPlayer(
                    entities[gFarmAnimals[index].entityIndex].coordinates.x,
                    entities[gFarmAnimals[index].entityIndex].coordinates.z,
                    entities[ENTITY_PLAYER].coordinates.x, 
                    entities[ENTITY_PLAYER].coordinates.z);

                gFarmAnimals[index].unk_1B = 1;
                
            } else {
        
                if (temp < 40) {
                
                    gFarmAnimals[index].unk_1B = 0;

                } else {

                    tempDirection = getRandomNumberInRange(0, 20);

                    if (tempDirection < 7) {
                        gFarmAnimals[index].direction = tempDirection;
                    }

                    temp = getRandomNumberInRange(0, 5);

                    if (temp == 0) {
                        gFarmAnimals[index].unk_1B = 1;
                    }
                    
                    if (temp == 1) {
                        gFarmAnimals[index].unk_1B = 2;
                    }

                    if (temp == 2) {
                        gFarmAnimals[index].unk_1B = 3;
                    }

                    if (temp == 3) {
                        gFarmAnimals[index].unk_1B = 4;
                    }

                }
                
            }
        
            gFarmAnimals[index].flags |= 2;
            break;

        
        case 1:

            gFarmAnimals[index].unk_14 = 0.5f;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x70);

            if (gFarmAnimals[index].flags & 0x200) {

                gFarmAnimals[index].flags &= ~0x400;

                gFarmAnimals[index].direction = calculateAnimalDirectionToPlayer(
                    entities[gFarmAnimals[index].entityIndex].coordinates.x,
                    entities[gFarmAnimals[index].entityIndex].coordinates.z,
                    entities[ENTITY_PLAYER].coordinates.x, 
                    entities[ENTITY_PLAYER].coordinates.z);

                if (getRandomNumberInRange(0, 20) == 0) {
                    gFarmAnimals[index].flags &= ~0x200;
                } 
                
            } else if (gFarmAnimals[index].flags & 0x400) {

                 if (getRandomNumberInRange(0, 40) == 0) {
                    gFarmAnimals[index].flags &= ~0x400;
                } 
                
            } else {

                if (getRandomNumberInRange(0, 20) < 10) {
                    gFarmAnimals[index].unk_1B = 1;
                } else {
                    gFarmAnimals[index].unk_1B = 0;
                }
                
            }

            gFarmAnimals[index].flags |= 2;            
            break;

        case 2:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x88);

            temp = getRandomNumberInRange(0, 2); 
            
            if (temp == 0) {
                gFarmAnimals[index].unk_1B = 2;
            } 
            if (temp == 1) {
                gFarmAnimals[index].unk_1B = 0;
            }
            if (temp == 2) {
                gFarmAnimals[index].unk_1B = 7;
            }

            gFarmAnimals[index].flags |= 2;  
            break;

        case 3:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x78);
            
            gFarmAnimals[index].unk_1B = 5;
            
            gFarmAnimals[index].flags |= 2;  
            break;

        case 4:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0xA8);

            gFarmAnimals[index].unk_1B = 0;

            gFarmAnimals[index].flags |= 2;  
            break;

        case 5:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x80);

            temp = getRandomNumberInRange(0, 11);

            if (temp < 10) {
                gFarmAnimals[index].unk_1B = 5;
            }

            if (temp == 10) {
                gFarmAnimals[index].unk_1B = 6;
            }

            if (temp == 11) {
                gFarmAnimals[index].unk_1B = 8;
            }

            gFarmAnimals[index].flags |= 2;
            break;

        case 6:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x90);

            if (getRandomNumberInRange(0, 4) >= 4) {
                gFarmAnimals[index].unk_1B = 5;
            } else {
                gFarmAnimals[index].unk_1B = 6;
            }

            gFarmAnimals[index].flags |= 2;
            break;

        case 7:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x98);

            gFarmAnimals[index].unk_1B = 2;

            gFarmAnimals[index].flags |= 2;
            break;

        case 8:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x78);

            gFarmAnimals[index].unk_1B = 0;

            gFarmAnimals[index].flags |= 2;
            
            break;

        case 16:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x68);

            gFarmAnimals[index].flags |= (2 | 0x2000);
            gFarmAnimals[index].flags &= ~(0x400);
            
            break;
        
        case 17:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x50);

            gFarmAnimals[index].unk_1B = 0;

            gFarmAnimals[index].flags |= (2 | 0x2000);
            gFarmAnimals[index].flags &= ~(0x400);

            playSfx(0x36);
            
            break;
        
        case 18:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0xA0);

            gFarmAnimals[index].unk_1B = 0;

            gFarmAnimals[index].flags |= (2 | 0x2000);
            gFarmAnimals[index].flags &= ~(0x400);

            playSfx(0x37);

            break;
        
        case 19:
            
            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x60);

            gFarmAnimals[index].unk_1B = 19;

            gFarmAnimals[index].flags |= (2 | 0x2000);
            gFarmAnimals[index].flags &= ~(0x400);
            
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008FC68);

void func_8008FC68(u8 index) {

    u16 temp;
    u16 tempDirection;
    
    switch (gFarmAnimals[index].unk_1B) {

        case 0:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x108);

            if ((gFarmAnimals[index].flags & 0x200) || (gFarmAnimals[index].flags & 0x400)) {

                gFarmAnimals[index].direction = calculateAnimalDirectionToPlayer(
                    entities[gFarmAnimals[index].entityIndex].coordinates.x,
                    entities[gFarmAnimals[index].entityIndex].coordinates.z,
                    entities[ENTITY_PLAYER].coordinates.x, 
                    entities[ENTITY_PLAYER].coordinates.z);

                gFarmAnimals[index].unk_1B = 1;
                
            } else {
        
                if (getRandomNumberInRange(0, 60) < 40) {
                
                    gFarmAnimals[index].unk_1B = 0;

                } else {

                    tempDirection = getRandomNumberInRange(0, 20);

                    if (tempDirection < 7) {
                        gFarmAnimals[index].direction = tempDirection;
                    }

                    temp = getRandomNumberInRange(0, 5);

                    if (temp == 0) {
                        gFarmAnimals[index].unk_1B = 1;
                    }
                    
                    if (temp == 1) {
                        gFarmAnimals[index].unk_1B = 2;
                    }

                    if (temp == 2) {
                        gFarmAnimals[index].unk_1B = 3;
                    }

                    if (temp == 3) {
                        gFarmAnimals[index].unk_1B = 4;
                    }

                }
                
            }
        
            gFarmAnimals[index].flags |= 2;
            break;

        
        case 1:

            gFarmAnimals[index].unk_14 = 0.5f;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x110);

            if (gFarmAnimals[index].flags & 0x200) {

                gFarmAnimals[index].flags &= ~0x400;

                gFarmAnimals[index].direction = calculateAnimalDirectionToPlayer(
                    entities[gFarmAnimals[index].entityIndex].coordinates.x,
                    entities[gFarmAnimals[index].entityIndex].coordinates.z,
                    entities[ENTITY_PLAYER].coordinates.x, 
                    entities[ENTITY_PLAYER].coordinates.z);

                if (getRandomNumberInRange(0, 20) == 0) {
                    gFarmAnimals[index].flags &= ~0x200;
                } 
                
            } else if (gFarmAnimals[index].flags & 0x400) {

                 if (getRandomNumberInRange(0, 40) == 0) {
                    gFarmAnimals[index].flags &= ~0x400;
                } 
                
            } else {

                if (getRandomNumberInRange(0, 20) < 10) {
                    gFarmAnimals[index].unk_1B = 1;
                } else {
                    gFarmAnimals[index].unk_1B = 0;
                }
                
            }

            gFarmAnimals[index].flags |= 2;            
            break;

        case 2:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x128);

            temp = getRandomNumberInRange(0, 2); 
            
            if (temp == 0) {
                gFarmAnimals[index].unk_1B = 2;
            } 
            if (temp == 1) {
                gFarmAnimals[index].unk_1B = 0;
            }
            if (temp == 2) {
                gFarmAnimals[index].unk_1B = 6;
            }

            gFarmAnimals[index].flags |= 2;  
            break;

        case 3:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x118);
            
            gFarmAnimals[index].unk_1B = 5;
            
            gFarmAnimals[index].flags |= 2;  
            
            break;

        case 4:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x138);

            gFarmAnimals[index].unk_1B = 0;

            gFarmAnimals[index].flags |= 2;  
            break;

        case 5:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x120);

            temp = getRandomNumberInRange(0, 11);

            if (temp < 10) {
                gFarmAnimals[index].unk_1B = 5;
            }

            if (temp == 10) {
                gFarmAnimals[index].unk_1B = 6;
            }

            if (temp == 11) {
                gFarmAnimals[index].unk_1B = 7;
            }

            gFarmAnimals[index].flags |= 2;
            break;

        case 6:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x130);

            gFarmAnimals[index].unk_1B = 2;

            gFarmAnimals[index].flags |= 2;
            break;

        case 7:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x118);

            gFarmAnimals[index].unk_1B = 0;

            gFarmAnimals[index].flags |= 2;
            break;

        case 16:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x108);

            gFarmAnimals[index].flags |= (2 | 0x2000);
            gFarmAnimals[index].flags &= ~(0x400);
            
            break;
        
        case 17:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x50);

            gFarmAnimals[index].unk_1B = 0;

            gFarmAnimals[index].flags |= (2 | 0x2000);
            gFarmAnimals[index].flags &= ~(0x400);

            playSfx(0x36);
            
            break;
        
        case 18:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x108);

            gFarmAnimals[index].unk_1B = 0;

            gFarmAnimals[index].flags |= (2 | 0x2000);
            gFarmAnimals[index].flags &= ~(0x400);

            break;
        
        case 19:
            
            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x60);

            gFarmAnimals[index].unk_1B = 19;

            gFarmAnimals[index].flags |= (2 | 0x2000);
            gFarmAnimals[index].flags &= ~(0x400);
            
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_800904E4);

void func_800904E4(u8 index) {
    
    u16 temp;
    u16 tempDirection;
    
    switch (gFarmAnimals[index].unk_1B) {
        
        case 0:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0);

            if ((gFarmAnimals[index].flags & 0x200) || (gFarmAnimals[index].flags & 0x400)) {

                gFarmAnimals[index].direction = calculateAnimalDirectionToPlayer(
                    entities[gFarmAnimals[index].entityIndex].coordinates.x,
                    entities[gFarmAnimals[index].entityIndex].coordinates.z,
                    entities[ENTITY_PLAYER].coordinates.x, 
                    entities[ENTITY_PLAYER].coordinates.z);

                gFarmAnimals[index].unk_1B = 1;
                
            } else {
        
                if (getRandomNumberInRange(0, 60) < 40) {
                    gFarmAnimals[index].unk_1B = 0;
                } else {

                    tempDirection = getRandomNumberInRange(0, 20);

                    if (tempDirection < 7) {
                        gFarmAnimals[index].direction = tempDirection;
                    }

                    temp = getRandomNumberInRange(0, 5);

                    if (temp == 0) {
                        gFarmAnimals[index].unk_1B = 1;
                    }
                    
                    if (temp == 1) {
                        gFarmAnimals[index].unk_1B = 2;
                    }

                    if (temp == 2) {
                        gFarmAnimals[index].unk_1B = 3;
                    }

                    if (temp == 3) {
                        gFarmAnimals[index].unk_1B = 4;
                    }
                    
                    if (temp == 4) {
                        gFarmAnimals[index].unk_1B = 5;
                    }

                    if (temp == 5) {
                        gFarmAnimals[index].unk_1B = 6;
                    }

                }
                
            }
        
            gFarmAnimals[index].flags |= 2;
            break;

        
        case 1:

            gFarmAnimals[index].unk_14 = 1.0f;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 8);

            if (gFarmAnimals[index].flags & 0x200) {

                gFarmAnimals[index].flags &= ~0x400;

                gFarmAnimals[index].direction = calculateAnimalDirectionToPlayer(
                    entities[gFarmAnimals[index].entityIndex].coordinates.x,
                    entities[gFarmAnimals[index].entityIndex].coordinates.z,
                    entities[ENTITY_PLAYER].coordinates.x, 
                    entities[ENTITY_PLAYER].coordinates.z);

                if (getRandomNumberInRange(0, 20) == 0) {
                    gFarmAnimals[index].flags &= ~0x200;
                } 
                
            } else if (gFarmAnimals[index].flags & 0x400) {

                 if (getRandomNumberInRange(0, 40) == 0) {
                    gFarmAnimals[index].flags &= ~0x400;
                } 
                
            } else {

                if (getRandomNumberInRange(0, 20) < 10) {
                    gFarmAnimals[index].unk_1B = 1;
                } else {
                    gFarmAnimals[index].unk_1B = 0;
                }
                
            }

            gFarmAnimals[index].flags |= 2;            
            break;

        case 2:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x10);

            temp = getRandomNumberInRange(0, 2); 
            
            if (temp == 0) {
                gFarmAnimals[index].unk_1B = 2;
            } 
            if (temp == 1) {
                gFarmAnimals[index].unk_1B = 0;
            }
            if (temp == 2) {
                gFarmAnimals[index].unk_1B = 8;
            }

            gFarmAnimals[index].flags |= 2;  
            break;

        case 3:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x18);

            temp = getRandomNumberInRange(0, 2); 
            
            if (temp == 0) {
                gFarmAnimals[index].unk_1B = 3;
            } 
            if (temp == 1) {
                gFarmAnimals[index].unk_1B = 0;
            }
            if (temp == 2) {
                gFarmAnimals[index].unk_1B = 7;
            }
            
            gFarmAnimals[index].flags |= 2;  
            
            break;

        case 4:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x30);

            gFarmAnimals[index].unk_1B = 0;

            gFarmAnimals[index].flags |= 2;  
            break;

        case 5:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x38);

            gFarmAnimals[index].unk_1B = 0;

            gFarmAnimals[index].flags |= 2;
            break;

        case 6:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x40);

            gFarmAnimals[index].unk_1B = 0;

            gFarmAnimals[index].flags |= 2;
            break;

        case 7:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x20);

            if (getRandomNumberInRange(0, 4) >= 4) {
                gFarmAnimals[index].unk_1B = 3;
            } else {
                gFarmAnimals[index].unk_1B = 7;
            }

            gFarmAnimals[index].flags |= 2;
            break;

        case 8:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x28);

            gFarmAnimals[index].unk_1B = 2;

            gFarmAnimals[index].flags |= 2;
            break;
    
        case 16:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0);

            gFarmAnimals[index].flags |= (2 | 0x2000);
            gFarmAnimals[index].flags &= ~(0x400);
            
            break;
        
        case 17:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x48);

            gFarmAnimals[index].unk_1B = 0;

            gFarmAnimals[index].flags |= (2 | 0x2000);
            gFarmAnimals[index].flags &= ~(0x400);

            playSfx(0x36);
            
            break;
        
        case 18:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x50);

            gFarmAnimals[index].unk_1B = 0;

            gFarmAnimals[index].flags |= (2 | 0x2000);
            gFarmAnimals[index].flags &= ~(0x400);

            playSfx(0x37);
            
            break;
        
        case 19:
            
            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x58);

            gFarmAnimals[index].unk_1B = 19;

            gFarmAnimals[index].flags |= (2 | 0x2000);
            gFarmAnimals[index].flags &= ~(0x400);
            
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80090DF8);

void func_80090DF8(u8 index) {

    u16 temp;
    u16 tempDirection;
    
    switch (gFarmAnimals[index].unk_1B) {
        
        case 0:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0);

            if ((gFarmAnimals[index].flags & 0x200) || (gFarmAnimals[index].flags & 0x400)) {

                gFarmAnimals[index].direction = calculateAnimalDirectionToPlayer(
                    entities[gFarmAnimals[index].entityIndex].coordinates.x,
                    entities[gFarmAnimals[index].entityIndex].coordinates.z,
                    entities[ENTITY_PLAYER].coordinates.x, 
                    entities[ENTITY_PLAYER].coordinates.z);

                gFarmAnimals[index].unk_1B = 1;
                
            } else {
        
                if (getRandomNumberInRange(0, 60) < 40) {
                    gFarmAnimals[index].unk_1B = 0;
                } else {

                    tempDirection = getRandomNumberInRange(0, 20);

                    if (tempDirection < 7) {
                        gFarmAnimals[index].direction = tempDirection;
                    }

                    temp = getRandomNumberInRange(0, 5);

                    if (temp == 0) {
                        gFarmAnimals[index].unk_1B = 1;
                    }
                    
                    if (temp == 1) {
                        gFarmAnimals[index].unk_1B = 2;
                    }

                    if (temp == 2) {
                        gFarmAnimals[index].unk_1B = 3;
                    }

                    if (temp == 3) {
                        gFarmAnimals[index].unk_1B = 4;
                    }
                    
                    if (temp == 4) {
                        gFarmAnimals[index].unk_1B = 5;
                    }

                    if (temp == 5) {
                        gFarmAnimals[index].unk_1B = 6;
                    }

                }
                
            }
        
            gFarmAnimals[index].flags |= 2;
            break;

        
        case 1:

            gFarmAnimals[index].unk_14 = 1.0f;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 8);

            if (gFarmAnimals[index].flags & 0x200) {

                gFarmAnimals[index].flags &= ~0x400;

                gFarmAnimals[index].direction = calculateAnimalDirectionToPlayer(
                    entities[gFarmAnimals[index].entityIndex].coordinates.x,
                    entities[gFarmAnimals[index].entityIndex].coordinates.z,
                    entities[ENTITY_PLAYER].coordinates.x, 
                    entities[ENTITY_PLAYER].coordinates.z);

                if (getRandomNumberInRange(0, 20) == 0) {
                    gFarmAnimals[index].flags &= ~0x200;
                } 
                
            } else if (gFarmAnimals[index].flags & 0x400) {

                 if (getRandomNumberInRange(0, 40) == 0) {
                    gFarmAnimals[index].flags &= ~0x400;
                } 
                
            } else {

                if (getRandomNumberInRange(0, 20) < 10) {
                    gFarmAnimals[index].unk_1B = 1;
                } else {
                    gFarmAnimals[index].unk_1B = 0;
                }
                
            }

            gFarmAnimals[index].flags |= 2;            
            break;

        case 2:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x10);

            temp = getRandomNumberInRange(0, 2); 
            
            if (temp == 0) {
                gFarmAnimals[index].unk_1B = 2;
            } 
            if (temp == 1) {
                gFarmAnimals[index].unk_1B = 0;
            }
            if (temp == 2) {
                gFarmAnimals[index].unk_1B = 8;
            }

            gFarmAnimals[index].flags |= 2;  
            break;

        case 3:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x18);

            temp = getRandomNumberInRange(0, 2); 
            
            if (temp == 0) {
                gFarmAnimals[index].unk_1B = 3;
            } 
            if (temp == 1) {
                gFarmAnimals[index].unk_1B = 0;
            }
            if (temp == 2) {
                gFarmAnimals[index].unk_1B = 7;
            }
            
            gFarmAnimals[index].flags |= 2;  
            
            break;

        case 4:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x30);

            gFarmAnimals[index].unk_1B = 0;

            gFarmAnimals[index].flags |= 2;  
            break;

        case 5:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x38);

            gFarmAnimals[index].unk_1B = 0;

            gFarmAnimals[index].flags |= 2;
            break;

        case 6:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x40);

            gFarmAnimals[index].unk_1B = 0;

            gFarmAnimals[index].flags |= 2;
            break;

        case 7:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x20);

            if (getRandomNumberInRange(0, 4) >= 4) {
                gFarmAnimals[index].unk_1B = 3;
            } else {
                gFarmAnimals[index].unk_1B = 7;
            }

            gFarmAnimals[index].flags |= 2;
            break;

        case 8:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x28);

            gFarmAnimals[index].unk_1B = 2;

            gFarmAnimals[index].flags |= 2;
            break;
    
        case 16:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0);

            gFarmAnimals[index].flags |= (2 | 0x2000);
            gFarmAnimals[index].flags &= ~(0x400);
            
            break;
        
        case 17:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x48);

            gFarmAnimals[index].unk_1B = 0;

            gFarmAnimals[index].flags |= (2 | 0x2000);
            gFarmAnimals[index].flags &= ~(0x400);

            playSfx(0x36);
            
            break;
        
        case 18:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x50);

            gFarmAnimals[index].unk_1B = 0;

            gFarmAnimals[index].flags |= (2 | 0x2000);
            gFarmAnimals[index].flags &= ~(0x400);

            playSfx(0x37);
            
            break;
        
        case 19:
            
            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x58);

            gFarmAnimals[index].unk_1B = 19;

            gFarmAnimals[index].flags |= (2 | 0x2000);
            gFarmAnimals[index].flags &= ~(0x400);
            
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009170C);

void func_8009170C(u8 index) {

    u16 temp;
    u16 tempDirection;
    
    switch (gFarmAnimals[index].unk_1B) {
        
        case 0:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0);

            if ((gFarmAnimals[index].flags & 0x200) || (gFarmAnimals[index].flags & 0x400)) {

                gFarmAnimals[index].direction = calculateAnimalDirectionToPlayer(
                    entities[gFarmAnimals[index].entityIndex].coordinates.x,
                    entities[gFarmAnimals[index].entityIndex].coordinates.z,
                    entities[ENTITY_PLAYER].coordinates.x, 
                    entities[ENTITY_PLAYER].coordinates.z);

                gFarmAnimals[index].unk_1B = 1;
                
            } else {
        
                if (getRandomNumberInRange(0, 60) < 40) {
                    gFarmAnimals[index].unk_1B = 0;
                } else {

                    tempDirection = getRandomNumberInRange(0, 20);

                    if (tempDirection < 7) {
                        gFarmAnimals[index].direction = tempDirection;
                    }

                    temp = getRandomNumberInRange(0, 5);

                    if (temp == 0) {
                        gFarmAnimals[index].unk_1B = 1;
                    }
                    
                    if (temp == 1) {
                        gFarmAnimals[index].unk_1B = 2;
                    }

                    if (temp == 2) {
                        gFarmAnimals[index].unk_1B = 3;
                    }

                    if (temp == 3) {
                        gFarmAnimals[index].unk_1B = 4;
                    }
                    
                    if (temp == 4) {
                        gFarmAnimals[index].unk_1B = 5;
                    }

                    if (temp == 5) {
                        gFarmAnimals[index].unk_1B = 6;
                    }

                }
                
            }
        
            gFarmAnimals[index].flags |= 2;
            break;
        
        case 1:

            gFarmAnimals[index].unk_14 = 0.5f;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 8);

            if (gFarmAnimals[index].flags & 0x200) {

                gFarmAnimals[index].flags &= ~0x400;

                gFarmAnimals[index].direction = calculateAnimalDirectionToPlayer(
                    entities[gFarmAnimals[index].entityIndex].coordinates.x,
                    entities[gFarmAnimals[index].entityIndex].coordinates.z,
                    entities[ENTITY_PLAYER].coordinates.x, 
                    entities[ENTITY_PLAYER].coordinates.z);

                if (getRandomNumberInRange(0, 20) == 0) {
                    gFarmAnimals[index].flags &= ~0x200;
                } 
                
            } else if (gFarmAnimals[index].flags & 0x400) {

                 if (getRandomNumberInRange(0, 40) == 0) {
                    gFarmAnimals[index].flags &= ~0x400;
                } 
                
            } else {

                if (getRandomNumberInRange(0, 20) < 10) {
                    gFarmAnimals[index].unk_1B = 1;
                } else {
                    gFarmAnimals[index].unk_1B = 0;
                }
                
            }

            gFarmAnimals[index].flags |= 2;            
            break;

        case 2:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x20);

            temp = getRandomNumberInRange(0, 2); 
            
            if (temp == 0) {
                gFarmAnimals[index].unk_1B = 2;
            } 
            if (temp == 1) {
                gFarmAnimals[index].unk_1B = 0;
            }
            if (temp == 2) {
                gFarmAnimals[index].unk_1B = 8;
            }

            gFarmAnimals[index].flags |= 2;  
            break;

        case 3:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x10);

            gFarmAnimals[index].unk_1B = 9;
            gFarmAnimals[index].flags |= 2;  
            
            break;

        case 4:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x38);

            gFarmAnimals[index].unk_1B = 0;

            gFarmAnimals[index].flags |= 2;  
            break;

        case 5:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x40);

            gFarmAnimals[index].unk_1B = 0;

            gFarmAnimals[index].flags |= 2;
            break;

        case 6:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x48);

            gFarmAnimals[index].unk_1B = 0;

            gFarmAnimals[index].flags |= 2;
            break;

        case 7:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x28);

            if (getRandomNumberInRange(0, 4) >= 4) {
                gFarmAnimals[index].unk_1B = 3;
            } else {
                gFarmAnimals[index].unk_1B = 7;
            }

            gFarmAnimals[index].flags |= 2;
            break;

        case 8:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x30);

            gFarmAnimals[index].unk_1B = 2;

            gFarmAnimals[index].flags |= 2;
            break;
        
        case 9:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x18);

            temp = getRandomNumberInRange(0, 2); 
            
            if (temp == 0) {
                gFarmAnimals[index].unk_1B = 9;
            } 
            if (temp == 1) {
                gFarmAnimals[index].unk_1B = 3;
            }
            if (temp == 2) {
                gFarmAnimals[index].unk_1B = 7;
            }
            
            gFarmAnimals[index].flags |= 2;
            break;
    
        case 16:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0);

            gFarmAnimals[index].flags |= (2 | 0x2000);
            gFarmAnimals[index].flags &= ~(0x400);
            
            break;
        
        case 17:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x50);

            gFarmAnimals[index].unk_1B = 0;

            gFarmAnimals[index].flags |= (2 | 0x2000);
            gFarmAnimals[index].flags &= ~(0x400);

            playSfx(0x36);
            
            break;
        
        case 18:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x58);

            gFarmAnimals[index].unk_1B = 0;

            gFarmAnimals[index].flags |= (2 | 0x2000);
            gFarmAnimals[index].flags &= ~(0x400);

            playSfx(0x37);
            
            break;
        
        case 19:
            
            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x60);

            gFarmAnimals[index].unk_1B = 19;

            gFarmAnimals[index].flags |= (2 | 0x2000);
            gFarmAnimals[index].flags &= ~(0x400);
            
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80092094);

void func_80092094(u8 index) {

    u16 temp;
    u16 tempDirection;
    
    switch (gFarmAnimals[index].unk_1B) {
        
        case 0:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0);

            if ((gFarmAnimals[index].flags & 0x200) || (gFarmAnimals[index].flags & 0x400)) {

                gFarmAnimals[index].direction = calculateAnimalDirectionToPlayer(
                    entities[gFarmAnimals[index].entityIndex].coordinates.x,
                    entities[gFarmAnimals[index].entityIndex].coordinates.z,
                    entities[ENTITY_PLAYER].coordinates.x, 
                    entities[ENTITY_PLAYER].coordinates.z);

                gFarmAnimals[index].unk_1B = 1;
                
            } else {
        
                if (getRandomNumberInRange(0, 60) < 40) {
                    gFarmAnimals[index].unk_1B = 0;
                } else {

                    tempDirection = getRandomNumberInRange(0, 20);

                    if (tempDirection < 7) {
                        gFarmAnimals[index].direction = tempDirection;
                    }

                    temp = getRandomNumberInRange(0, 2);

                    if (temp == 0) {
                        gFarmAnimals[index].unk_1B = 1;
                    }
                    
                    if (temp == 1) {
                        gFarmAnimals[index].unk_1B = 2;
                    }

                    if (temp == 2) {
                        gFarmAnimals[index].unk_1B = 3;
                    }

                }
                
            }
        
            gFarmAnimals[index].flags |= 2;
            break;

        case 1:

            gFarmAnimals[index].unk_14 = 1.0f;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 8);

            if (gFarmAnimals[index].flags & 0x200) {

                gFarmAnimals[index].flags &= ~0x400;

                gFarmAnimals[index].direction = calculateAnimalDirectionToPlayer(
                    entities[gFarmAnimals[index].entityIndex].coordinates.x,
                    entities[gFarmAnimals[index].entityIndex].coordinates.z,
                    entities[ENTITY_PLAYER].coordinates.x, 
                    entities[ENTITY_PLAYER].coordinates.z);

                if (getRandomNumberInRange(0, 20) == 0) {
                    gFarmAnimals[index].flags &= ~0x200;
                } 
                
            } else if (gFarmAnimals[index].flags & 0x400) {

                 if (getRandomNumberInRange(0, 40) == 0) {
                    gFarmAnimals[index].flags &= ~0x400;
                } 
                
            } else {

                if (getRandomNumberInRange(0, 20) < 10) {
                    gFarmAnimals[index].unk_1B = 1;
                } else {
                    gFarmAnimals[index].unk_1B = 0;
                }
                
            }

            gFarmAnimals[index].flags |= 2;            
            break;

        case 2:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x10);

            if (getRandomNumberInRange(0, 2) == 0) {
                gFarmAnimals[index].unk_1B = 2;
            } else {
                gFarmAnimals[index].unk_1B = 0;
            }

            gFarmAnimals[index].flags |= 2;  

            break;

        case 3:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x18);

            gFarmAnimals[index].unk_1B = 4;

            gFarmAnimals[index].flags |= 2;  
            
            break;


        case 4:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x20);

            temp = getRandomNumberInRange(0, 11);

            if (temp < 10) {
                gFarmAnimals[index].unk_1B = 4;
            }

            if (temp == 10) {
                gFarmAnimals[index].unk_1B = 4;
            }

            if (temp == 11) {
                gFarmAnimals[index].unk_1B = 6;
            }

            gFarmAnimals[index].flags |= 2;  
            break;

        case 5:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x28);

            if (getRandomNumberInRange(0, 4) >= 4) {
                gFarmAnimals[index].unk_1B = 4;
            } else {
                gFarmAnimals[index].unk_1B = 5;
            }

            gFarmAnimals[index].flags |= 2;
            break;

        case 6:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x18);

            gFarmAnimals[index].unk_1B = 0;
            
            gFarmAnimals[index].flags |= 2;
            break;
    
        case 16:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0);

            gFarmAnimals[index].flags |= (2 | 0x2000);
            gFarmAnimals[index].flags &= ~(0x400);
            
            break;
        
        case 17:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0);

            gFarmAnimals[index].unk_1B = 0;

            gFarmAnimals[index].flags |= (2 | 0x2000);
            gFarmAnimals[index].flags &= ~(0x400);

            playSfx(0x3A);
            
            break;
        
        case 18:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x30);

            gFarmAnimals[index].unk_1B = 0;

            gFarmAnimals[index].flags |= (2 | 0x2000);
            gFarmAnimals[index].flags &= ~(0x400);

            playSfx(0x3A);
            
            break;
        
        case 19:
            
            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x30);

            gFarmAnimals[index].unk_1B = 19;

            gFarmAnimals[index].flags |= (2 | 0x2000);
            gFarmAnimals[index].flags &= ~(0x400);
            
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80092808);

void func_80092808(u8 index) {

    switch (gFarmAnimals[index].unk_1B) {
        
        case 0:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x28);

            gFarmAnimals[index].flags |= 2;
            
            break;
    
        case 16:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0);

            gFarmAnimals[index].flags |= (2 | 0x2000);
            gFarmAnimals[index].flags &= ~(0x400);
            
            break;
        
        case 17:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0);

            gFarmAnimals[index].unk_1B = 0;

            gFarmAnimals[index].flags |= (2 | 0x2000);
            gFarmAnimals[index].flags &= ~(0x400);
            
            break;
        
        case 18:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x30);

            gFarmAnimals[index].unk_1B = 0;

            gFarmAnimals[index].flags |= (2 | 0x2000);
            gFarmAnimals[index].flags &= ~(0x400);
            
            break;
        
        case 19:
            
            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x30);

            gFarmAnimals[index].unk_1B = 19;

            gFarmAnimals[index].flags |= (2 | 0x2000);
            gFarmAnimals[index].flags &= ~(0x400);
            
            break;
    
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80092A64);

void func_80092A64(u8 index) {

    u16 temp;

    switch(gFarmAnimals[index].unk_1B) {
        
        case 0:
            
            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x38);

            if (gFarmAnimals[index].flags & 0x200 || gFarmAnimals[index].flags & 0x400) {
                
                gFarmAnimals[index].direction = calculateAnimalDirectionToPlayer(entities[gFarmAnimals[index].entityIndex].coordinates.x, entities[gFarmAnimals[index].entityIndex].coordinates.z, entities[ENTITY_PLAYER].coordinates.x, entities[ENTITY_PLAYER].coordinates.z);
                gFarmAnimals[index].unk_1B = 1;
            
            } else if (getRandomNumberInRange(0, 60) < 40) {
            
                gFarmAnimals[index].unk_1B = 0;
                    
            } else {
                
                temp = getRandomNumberInRange(0, 20);
                
                if (temp < 7) {
                    gFarmAnimals[index].direction = temp;
                }

                temp = getRandomNumberInRange(0, 2);

                if (temp == 0) {
                    gFarmAnimals[index].unk_1B = 1;
                } 
                
                if (temp == 1) {
                    gFarmAnimals[index].unk_1B = 2;
                } 
                
                if (temp == 2) {
                    gFarmAnimals[index].unk_1B = 3;
                }
                
            }

            gFarmAnimals[index].flags |= 2;
            
            break;

        case 1:
            
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;
            gFarmAnimals[index].unk_14 = 1.0f;
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x40);

            if (gFarmAnimals[index].flags & 0x200) {

                gFarmAnimals[index].flags &= ~0x400;
                gFarmAnimals[index].direction = calculateAnimalDirectionToPlayer(entities[gFarmAnimals[index].entityIndex].coordinates.x, entities[gFarmAnimals[index].entityIndex].coordinates.z, entities[ENTITY_PLAYER].coordinates.x, entities[ENTITY_PLAYER].coordinates.z);

                if (!getRandomNumberInRange(0, 20)) {
                    gFarmAnimals[index].flags &= ~0x200;
                }
                
            } else if (gFarmAnimals[index].flags & 0x400) {

                if (!getRandomNumberInRange(0, 40)) {
                    gFarmAnimals[index].flags &= ~0x400;
                }
                
            } else {
                if (getRandomNumberInRange(0, 20) < 10) {
                    gFarmAnimals[index].unk_1B = 1;
                } else {
                    gFarmAnimals[index].unk_1B = 0;
                }
            }

            gFarmAnimals[index].flags |= 2;
            
            break;

        case 2:
            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x48);
            
            if (!getRandomNumberInRange(0, 2)) {
                gFarmAnimals[index].unk_1B = 2;
            } else {
                gFarmAnimals[index].unk_1B = 0;
            }
            
            gFarmAnimals[index].flags |= 2;
        
            break;

        case 3:
            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x50);
            gFarmAnimals[index].unk_1B = 4;
            gFarmAnimals[index].flags |= 2;
            break;

        case 4:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x58);

            temp = getRandomNumberInRange(0, 11);

            if (temp < 10) {
                gFarmAnimals[index].unk_1B = 4;
            } 
            
            if (temp == 10) {
                gFarmAnimals[index].unk_1B = 4;
            } 
            
            if (temp == 11) {
                gFarmAnimals[index].unk_1B = 6;
            }

            gFarmAnimals[index].flags |= 2;
            
            break;

        case 5:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x60);

            if (getRandomNumberInRange(0, 4) >= 4) {
                gFarmAnimals[index].unk_1B = 4;
            } else {
                gFarmAnimals[index].unk_1B = 5;
            }

            gFarmAnimals[index].flags |= 2;

            break;

        case 6:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x50);
            gFarmAnimals[index].unk_1B = 0;
            gFarmAnimals[index].flags |= 2;
            
            break;

        case 16:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x38);
            gFarmAnimals[index].flags |= (0x2 | 0x2000);
            gFarmAnimals[index].flags &= ~0x400;
            break;

        case 17:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x38);
            gFarmAnimals[index].unk_1B = 0;
            gFarmAnimals[index].flags |= (0x2 | 0x2000);
            gFarmAnimals[index].flags &= ~0x400;
            playSfx(0x3A);
        
            break;

        case 18:
            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x68);
            gFarmAnimals[index].unk_1B = 0;
            gFarmAnimals[index].flags |= (0x2 | 0x2000);
            gFarmAnimals[index].flags &= ~0x400;
            playSfx(0x3A);
            break;

        case 19:
            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x68);
            gFarmAnimals[index].unk_1B = 19;
            gFarmAnimals[index].flags |= (0x2 | 0x2000);
            gFarmAnimals[index].flags &= ~0x400;
            break;

    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_800931D8);

void func_800931D8(u8 farmAnimalIndex) {

    u16 temp;
    
    switch (gFarmAnimals[farmAnimalIndex].unk_1B) {
        
        case 0:
            gFarmAnimals[farmAnimalIndex].unk_14 = 0;
            gFarmAnimals[farmAnimalIndex].unk_1D = 0;
            gFarmAnimals[farmAnimalIndex].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[farmAnimalIndex].entityIndex, 0x60);

            gFarmAnimals[farmAnimalIndex].flags |= 2;

            break;

        case 16:    
            gFarmAnimals[farmAnimalIndex].unk_14 = 0;
            gFarmAnimals[farmAnimalIndex].unk_1D = 0;
            gFarmAnimals[farmAnimalIndex].unk_1E = 0;
    
            setEntityAnimationWithDirectionChange(gFarmAnimals[farmAnimalIndex].entityIndex, 0x60);

            gFarmAnimals[farmAnimalIndex].flags |= (2 | 0x2000);
            gFarmAnimals[farmAnimalIndex].flags &= ~0x400;
            
            break;

        case 17:
            gFarmAnimals[farmAnimalIndex].unk_14 = 0;
            gFarmAnimals[farmAnimalIndex].unk_1D = 0;
            gFarmAnimals[farmAnimalIndex].unk_1E = 0;
    
            setEntityAnimationWithDirectionChange(gFarmAnimals[farmAnimalIndex].entityIndex, 0x60);

            gFarmAnimals[farmAnimalIndex].unk_1B = 0;

            gFarmAnimals[farmAnimalIndex].flags |= (2 | 0x2000);
            gFarmAnimals[farmAnimalIndex].flags &= ~0x400;
            
            break;
        
        case 18:
            gFarmAnimals[farmAnimalIndex].unk_14 = 0;
            gFarmAnimals[farmAnimalIndex].unk_1D = 0;
            gFarmAnimals[farmAnimalIndex].unk_1E = 0;
    
            setEntityAnimationWithDirectionChange(gFarmAnimals[farmAnimalIndex].entityIndex, 0x68);

            gFarmAnimals[farmAnimalIndex].unk_1B = 0;

            gFarmAnimals[farmAnimalIndex].flags |= (2 | 0x2000);
            gFarmAnimals[farmAnimalIndex].flags &= ~0x400;
            
            break;

        case 19:    
            gFarmAnimals[farmAnimalIndex].unk_14 = 0;
            gFarmAnimals[farmAnimalIndex].unk_1D = 0;
            gFarmAnimals[farmAnimalIndex].unk_1E = 0;
    
            setEntityAnimationWithDirectionChange(gFarmAnimals[farmAnimalIndex].entityIndex, 0x68);

            gFarmAnimals[farmAnimalIndex].unk_1B = 19;

            gFarmAnimals[farmAnimalIndex].flags |= (2 | 0x2000);
            gFarmAnimals[farmAnimalIndex].flags &= ~0x400;
            
            break;

    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80093434);

void func_80093434(u8 farmAnimalIndex) {

    u16 temp;
    
    switch (gFarmAnimals[farmAnimalIndex].unk_1B) {
        
        case 0:
            gFarmAnimals[farmAnimalIndex].unk_14 = 0;
            gFarmAnimals[farmAnimalIndex].unk_1D = 0;
            gFarmAnimals[farmAnimalIndex].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[farmAnimalIndex].entityIndex, 0);

            if (gFarmAnimals[farmAnimalIndex].flags & 0x200 || gFarmAnimals[farmAnimalIndex].flags & 0x400) {

                gFarmAnimals[farmAnimalIndex].direction = calculateAnimalDirectionToPlayer(entities[gFarmAnimals[farmAnimalIndex].entityIndex].coordinates.x, 
                                   entities[gFarmAnimals[farmAnimalIndex].entityIndex].coordinates.z, entities[ENTITY_PLAYER].coordinates.x, entities[ENTITY_PLAYER].coordinates.z);

                gFarmAnimals[farmAnimalIndex].unk_1B = 1;
                
            } else if (getRandomNumberInRange(0, 60) < 40) {

                gFarmAnimals[farmAnimalIndex].unk_1B = 0;
                
            } else {

                temp = getRandomNumberInRange(0, 20);

                if (temp < 7) {
                    gFarmAnimals[farmAnimalIndex].direction = temp;
                }

                temp = getRandomNumberInRange(0, 2);

                if (temp == 0) {
                    gFarmAnimals[farmAnimalIndex].unk_1B = 1;
                }

                if (temp == 1) {
                    gFarmAnimals[farmAnimalIndex].unk_1B = 2;
                }

                if (temp == 2) {
                    gFarmAnimals[farmAnimalIndex].unk_1B = 3;
                }
                
            }

            gFarmAnimals[farmAnimalIndex].flags |= 2;

            break;

        case 1:
            gFarmAnimals[farmAnimalIndex].unk_14 = 1.0f;
            gFarmAnimals[farmAnimalIndex].unk_1D = 0;
            gFarmAnimals[farmAnimalIndex].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[farmAnimalIndex].entityIndex, 8);

            if (gFarmAnimals[farmAnimalIndex].flags & 0x200) {

                gFarmAnimals[farmAnimalIndex].flags &= ~0x400;

                gFarmAnimals[farmAnimalIndex].direction = calculateAnimalDirectionToPlayer(entities[gFarmAnimals[farmAnimalIndex].entityIndex].coordinates.x, 
                                   entities[gFarmAnimals[farmAnimalIndex].entityIndex].coordinates.z, entities[ENTITY_PLAYER].coordinates.x, entities[ENTITY_PLAYER].coordinates.z);

                if (!getRandomNumberInRange(0, 20)) {
                    gFarmAnimals[farmAnimalIndex].flags &= ~0x200;
                }
                
            } else if (gFarmAnimals[farmAnimalIndex].flags & 0x400) {

                if (!getRandomNumberInRange(0, 40)) {
                    gFarmAnimals[farmAnimalIndex].flags &= ~0x400;
                }
                
            } else {

                if (getRandomNumberInRange(0, 20) < 10) {
                    gFarmAnimals[farmAnimalIndex].unk_1B = 1;
                } else {
                    gFarmAnimals[farmAnimalIndex].unk_1B = 0;
                }
                
            }

            gFarmAnimals[farmAnimalIndex].flags |= 2;

            break;

        case 2:    
            gFarmAnimals[farmAnimalIndex].unk_14 = 0;
            gFarmAnimals[farmAnimalIndex].unk_1D = 0;
            gFarmAnimals[farmAnimalIndex].unk_1E = 0;
    
            setEntityAnimationWithDirectionChange(gFarmAnimals[farmAnimalIndex].entityIndex, 0x10);

            if (!getRandomNumberInRange(0, 2)) {
                gFarmAnimals[farmAnimalIndex].unk_1B = 2;
            } else {
                gFarmAnimals[farmAnimalIndex].unk_1B = 0;
            }

            gFarmAnimals[farmAnimalIndex].flags |= 2;
            
            break;
        
        case 3:    
            gFarmAnimals[farmAnimalIndex].unk_14 = 0;
            gFarmAnimals[farmAnimalIndex].unk_1D = 0;
            gFarmAnimals[farmAnimalIndex].unk_1E = 0;
    
            setEntityAnimationWithDirectionChange(gFarmAnimals[farmAnimalIndex].entityIndex, 0x20);

            gFarmAnimals[farmAnimalIndex].unk_1B = 4;

            gFarmAnimals[farmAnimalIndex].flags |= 2;
            
            break;
        
        case 4:    
            gFarmAnimals[farmAnimalIndex].unk_14 = 0;
            gFarmAnimals[farmAnimalIndex].unk_1D = 0;
            gFarmAnimals[farmAnimalIndex].unk_1E = 0;
    
            setEntityAnimationWithDirectionChange(gFarmAnimals[farmAnimalIndex].entityIndex, 0x28);

            temp = getRandomNumberInRange(0, 11); 

            if (temp < 10) {
                gFarmAnimals[farmAnimalIndex].unk_1B = 4;
            } 
            
            if (temp == 10) {
                gFarmAnimals[farmAnimalIndex].unk_1B = 4;
            } 
            
            if (temp == 11) {
                gFarmAnimals[farmAnimalIndex].unk_1B = 6;
            } 

            gFarmAnimals[farmAnimalIndex].flags |= 2;
            
            break;
    
        case 5:    
            gFarmAnimals[farmAnimalIndex].unk_14 = 0;
            gFarmAnimals[farmAnimalIndex].unk_1D = 0;
            gFarmAnimals[farmAnimalIndex].unk_1E = 0;
    
            setEntityAnimationWithDirectionChange(gFarmAnimals[farmAnimalIndex].entityIndex, 0x18);

            if (getRandomNumberInRange(0, 4) < 4) {
                gFarmAnimals[farmAnimalIndex].unk_1B = 5;
            } else {
                gFarmAnimals[farmAnimalIndex].unk_1B = 4;
            }

            gFarmAnimals[farmAnimalIndex].flags |= 2;
            
            break;

        case 6:    
            gFarmAnimals[farmAnimalIndex].unk_14 = 0;
            gFarmAnimals[farmAnimalIndex].unk_1D = 0;
            gFarmAnimals[farmAnimalIndex].unk_1E = 0;
    
            setEntityAnimationWithDirectionChange(gFarmAnimals[farmAnimalIndex].entityIndex, 0x20);

            gFarmAnimals[farmAnimalIndex].unk_1B = 0;

            gFarmAnimals[farmAnimalIndex].flags |= 2;
            
            break;

        case 16:    
            gFarmAnimals[farmAnimalIndex].unk_14 = 0;
            gFarmAnimals[farmAnimalIndex].unk_1D = 0;
            gFarmAnimals[farmAnimalIndex].unk_1E = 0;
    
            setEntityAnimationWithDirectionChange(gFarmAnimals[farmAnimalIndex].entityIndex, 0);

            gFarmAnimals[farmAnimalIndex].flags |= (2 | 0x2000);
            gFarmAnimals[farmAnimalIndex].flags &= ~0x400;
            
            break;

        case 17:
        case 18:
            gFarmAnimals[farmAnimalIndex].unk_14 = 0;
            gFarmAnimals[farmAnimalIndex].unk_1D = 0;
            gFarmAnimals[farmAnimalIndex].unk_1E = 0;
    
            setEntityAnimationWithDirectionChange(gFarmAnimals[farmAnimalIndex].entityIndex, 0);

            gFarmAnimals[farmAnimalIndex].unk_1B = 0;

            gFarmAnimals[farmAnimalIndex].flags |= (2 | 0x2000);
            gFarmAnimals[farmAnimalIndex].flags &= ~0x400;
            playSfx(0x3A);
            
            break;

        case 19:    
            gFarmAnimals[farmAnimalIndex].unk_14 = 0;
            gFarmAnimals[farmAnimalIndex].unk_1D = 0;
            gFarmAnimals[farmAnimalIndex].unk_1E = 0;
    
            setEntityAnimationWithDirectionChange(gFarmAnimals[farmAnimalIndex].entityIndex, 0);

            gFarmAnimals[farmAnimalIndex].unk_1B = 19;

            gFarmAnimals[farmAnimalIndex].flags |= (2 | 0x2000);
            gFarmAnimals[farmAnimalIndex].flags &= ~0x400;
            
            break;

    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", updateHorse);

void updateHorse(void) {

    Vec3f vec;

    if ((horseInfo.flags & 1) && (horseInfo.flags & 4)) {

        if (checkEntityAnimationStateChanged(horseInfo.entityIndex) || horseInfo.flags & 0x20) {
            
            switch (horseInfo.grown) {
                case FALSE:
                    updateHorseNotGrown();
                    break;                    
                case TRUE:
                    updateHorseGrown();
                    break;
            }
            
            horseInfo.flags &= ~(0x20 | 0x80);
            
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
    
    switch (horseInfo.unk_17) {

        case 0:
            
            horseInfo.speed = 0;
            horseInfo.unk_1A = 0;
            horseInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(horseInfo.entityIndex, 0);
            
            if (horseInfo.flags & 0x10) {

                horseInfo.direction = calculateAnimalDirectionToPlayer(entities[horseInfo.entityIndex].coordinates.x, entities[horseInfo.entityIndex].coordinates.z, entities[ENTITY_PLAYER].coordinates.x, entities[ENTITY_PLAYER].coordinates.z);
                horseInfo.unk_17 = 4;
                
            } else {

                if (getRandomNumberInRange(0, 60) < 40)  {
                    
                    horseInfo.unk_17 = 0;
                    
                } else {
                        
                    tempDirection = getRandomNumberInRange(0, 20);

                    if (tempDirection < 7) {
                        horseInfo.direction = tempDirection;
                    }

                    temp = getRandomNumberInRange(0, 5);

                    if (temp == 0) {
                        horseInfo.unk_17 = 1;
                    }
                    
                    if (temp == 1) {
                        horseInfo.unk_17 = 2;
                    }

                    if (temp == 2) {
                        horseInfo.unk_17 = 3;
                    }

                    if (temp == 3) {
                        horseInfo.unk_17 = 4;
                    }

                    if (temp == 4) {
                        horseInfo.unk_17 = 5;
                    }

                    if (temp == 5) {
                        horseInfo.unk_17 = 6;
                    }
                    
                }

            } 
            
            horseInfo.flags |= 2;
            break;

        case 1:

            horseInfo.speed = 1;
            horseInfo.unk_1A = 0;
            horseInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(horseInfo.entityIndex, 8);
            
            if (getRandomNumberInRange(0, 20) < 10) {
                horseInfo.unk_17 = 1;
            } else {
                horseInfo.unk_17 = 0;
            }

            horseInfo.flags |= 2;
            break;

        case 2:

            horseInfo.speed = 0;
            horseInfo.unk_1A = 0;
            horseInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(horseInfo.entityIndex, 0x20U);

            if (getRandomNumberInRange(0, 2) == 0) {
                horseInfo.unk_17 = 2;
            } else {   
                horseInfo.unk_17 = 0;    
            }       
            
            horseInfo.flags |= 2;
            break;

        case 3:

            horseInfo.speed = 0;
            horseInfo.unk_1A = 0;
            horseInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(horseInfo.entityIndex, 0x18);
            
            horseInfo.unk_17 = 7;
            horseInfo.flags |= 2;
            
            break;

        case 4:

            horseInfo.speed = 2;
            horseInfo.unk_1A = 0;
            horseInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(horseInfo.entityIndex, 0x10);

            if (horseInfo.flags & 0x10) {
                
                horseInfo.direction = calculateAnimalDirectionToPlayer(entities[horseInfo.entityIndex].coordinates.x, entities[horseInfo.entityIndex].coordinates.z, entities[ENTITY_PLAYER].coordinates.x, entities[ENTITY_PLAYER].coordinates.z);
                
                if (getRandomNumberInRange(0, 20) == 0) {
                    horseInfo.flags &= ~(0x10);
                }
                
            } else {
                
                if (getRandomNumberInRange(0, 19) < 10) {
                    horseInfo.unk_17 = 4;
                } else {                
                    horseInfo.unk_17 = 0;    
                }
                
            }
        
            horseInfo.flags |= 2;
            break;

        case 5:

            horseInfo.speed = 0;
            horseInfo.unk_1A = 0;
            horseInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(horseInfo.entityIndex, 0x48);

            if (getRandomNumberInRange(0, 2) == 0) {
                horseInfo.unk_17 = 5;
            } else {   
                horseInfo.unk_17 = 0;    
            }
           
            horseInfo.flags |= 2;
            break;

        case 6:

            horseInfo.speed = 0;
            horseInfo.unk_1A = 0;
            horseInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(horseInfo.entityIndex, 0x50);

            if (getRandomNumberInRange(0, 2) == 0) {
                horseInfo.unk_17 = 6;
            } else {
                horseInfo.unk_17 = 0;    
            }
           
            horseInfo.flags |= 2;
            break;

        case 7:

            horseInfo.speed = 0;
            horseInfo.unk_1A = 0;
            horseInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(horseInfo.entityIndex, 0x28);

            if (getRandomNumberInRange(0, 2) == 0) {
                horseInfo.unk_17 = 7;
            } else {
                horseInfo.unk_17 = 0;    
            }
           
            horseInfo.flags |= 2;
            break;

        case 8:
            
            horseInfo.speed = 0;
            horseInfo.unk_1A = 0;
            horseInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(horseInfo.entityIndex, 0x30);

            if (getRandomNumberInRange(0, 2) == 0) {
                horseInfo.unk_17 = 8;
            } else {
                horseInfo.unk_17 = 0;    
            }
           
            horseInfo.flags |= 2;
            break;
        
        
        case 16:
            
            horseInfo.speed = 0;
            horseInfo.unk_1A = 0;
            horseInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(horseInfo.entityIndex, 0);
            
            horseInfo.flags |= (0x2 | 0x80); 
            horseInfo.flags &= ~(0x40);
            
            break;

        case 17:

            horseInfo.speed = 0;
            horseInfo.unk_1A = 0;
            horseInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(horseInfo.entityIndex, 0x48);
            
            horseInfo.unk_17 = 0;
            horseInfo.flags |= (0x2 | 0x80); 
            horseInfo.flags &= ~(0x40);
            
            playSfx(0x39);
            
            break;

        case 18:

            horseInfo.speed = 0;
            horseInfo.unk_1A = 0;
            horseInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(horseInfo.entityIndex, 0);
            
            horseInfo.unk_17 = 0;
            horseInfo.flags |= (0x2 | 0x80); 
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
    
    switch (horseInfo.unk_17) {

        case 0:
            
            horseInfo.speed = 0;
            horseInfo.unk_1A = 0;
            horseInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(horseInfo.entityIndex, 0);
            
            if (horseInfo.flags & 0x10) {

                horseInfo.direction = calculateAnimalDirectionToPlayer(entities[horseInfo.entityIndex].coordinates.x, entities[horseInfo.entityIndex].coordinates.z, entities[ENTITY_PLAYER].coordinates.x, entities[ENTITY_PLAYER].coordinates.z);
                horseInfo.unk_17 = 4;
                
            } else {

                if (getRandomNumberInRange(0, 60) < 40)  {
                    
                    horseInfo.unk_17 = 0;
                    
                } else {
                        
                    tempDirection = getRandomNumberInRange(0, 20);

                    if (tempDirection < 7) {
                        horseInfo.direction = tempDirection;
                    }

                    temp = getRandomNumberInRange(0, 5);

                    if (temp == 0) {
                        horseInfo.unk_17 = 1;
                    }
                    
                    if (temp == 1) {
                        horseInfo.unk_17 = 2;
                    }

                    if (temp == 2) {
                        horseInfo.unk_17 = 3;
                    }

                    if (temp == 3) {
                        horseInfo.unk_17 = 4;
                    }

                    if (temp == 4) {
                        horseInfo.unk_17 = 5;
                    }
                    
                }

            } 
            
            horseInfo.flags |= 2;
            break;

        case 1:

            horseInfo.speed = 1;
            horseInfo.unk_1A = 0;
            horseInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(horseInfo.entityIndex, 8);
            
            if (getRandomNumberInRange(0, 20) < 10) {
                horseInfo.unk_17 = 1;
            } else {
                horseInfo.unk_17 = 0;
            }

            horseInfo.flags |= 2;
            break;

        case 2:

            horseInfo.speed = 0;
            horseInfo.unk_1A = 0;
            horseInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(horseInfo.entityIndex, 0x20U);

            if (getRandomNumberInRange(0, 2) == 0) {
                horseInfo.unk_17 = 2;
            } else {   
                horseInfo.unk_17 = 0;    
            }       
            
            horseInfo.flags |= 2;
            break;

        case 3:

            horseInfo.speed = 0;
            horseInfo.unk_1A = 0;
            horseInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(horseInfo.entityIndex, 0x18);
            
            horseInfo.unk_17 = 6;
            horseInfo.flags |= 2;
            
            break;

        case 4:

            horseInfo.speed = 2;
            horseInfo.unk_1A = 0;
            horseInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(horseInfo.entityIndex, 0x10);

            if (horseInfo.flags & 0x10) {
                
                horseInfo.direction = calculateAnimalDirectionToPlayer(entities[horseInfo.entityIndex].coordinates.x, entities[horseInfo.entityIndex].coordinates.z, entities[ENTITY_PLAYER].coordinates.x, entities[ENTITY_PLAYER].coordinates.z);
                
                if (getRandomNumberInRange(0, 20) == 0) {
                    horseInfo.flags &= ~(0x10);
                }
                
            } else {
                
                if (getRandomNumberInRange(0, 2) == 0) {
                    horseInfo.unk_17 = 4;
                } else {                
                    horseInfo.unk_17 = 0;    
                }
                
            }
        
            horseInfo.flags |= 2;
            break;

        case 5:

            horseInfo.speed = 0;
            horseInfo.unk_1A = 0;
            horseInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(horseInfo.entityIndex, 0x30);

            if (getRandomNumberInRange(0, 2) == 0) {
                horseInfo.unk_17 = 5;
            } else {   
                horseInfo.unk_17 = 0;    
            }
           
            horseInfo.flags |= 2;
            break;

        case 6:

            horseInfo.speed = 0;
            horseInfo.unk_1A = 0;
            horseInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(horseInfo.entityIndex, 0x28);

            if (getRandomNumberInRange(0, 2) == 0) {
                horseInfo.unk_17 = 6;
            } else {
                horseInfo.unk_17 = 0;    
            }
           
            horseInfo.flags |= 2;
            break;
        
        case 16:
            
            horseInfo.speed = 0;
            horseInfo.unk_1A = 0;
            horseInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(horseInfo.entityIndex, 0);
            
            horseInfo.flags |= (0x2 | 0x80); 
            horseInfo.flags &= ~(0x40);
            
            break;

        case 17:

            horseInfo.speed = 0;
            horseInfo.unk_1A = 0;
            horseInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(horseInfo.entityIndex, 0x30);
            
            horseInfo.unk_17 = 0;
            horseInfo.flags |= (0x2 | 0x80); 
            horseInfo.flags &= ~(0x40);
            
            playSfx(0x39);
            
            break;

        case 18:

            horseInfo.speed = 0;
            horseInfo.unk_1A = 0;
            horseInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(horseInfo.entityIndex, 0);
            
            horseInfo.unk_17 = 0;
            horseInfo.flags |= (0x2 | 0x80); 
            horseInfo.flags &= ~(0x40);
            
            playSfx(0x38);
            
            break;

        default:
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009476C);

void func_8009476C(u8 index) {

    Vec3f vec;

    if ((gMiscAnimals[index].flags & 1) && (gMiscAnimals[index].flags & 4)) {

        if (checkEntityAnimationStateChanged(gMiscAnimals[index].entityIndex) || gMiscAnimals[index].unk_F == 0x20) {

            switch (gMiscAnimals[index].animalType) {

                case 0:
                    func_80094A5C(index);
                    break;
                
                case 1:
                    func_800958EC(index);
                    break;
                
                case 2:
                    func_80096264(index);
                    break;
                
                case 3:
                case 4:
                    func_800967E0(index);
                    break;

                case 6:
                    func_80096978(index);
                    break;
                
                case 5:
                    func_80096DFC(index);
                    break;
                case 7:
                    func_800973F8(index);
                    break;
                case 8:
                    func_80097804(index);
                    break;
                case 9:
                    func_80097CB0(index);
                    break;
                case 10:
                    func_80098188(index);
                    break;
                case 11:
                    func_80098740(index);
                    break;
                case 12:
                    func_80098B3C(index);
                    break;
                case 13:
                    func_80098F24(index);
                    break;
                case 14:
                    func_800991C0(index);
                    break;
                case 15:
                    func_80099424(index);
                    break;
                case 16:
                    func_80099548(index);
                    break;
                case 17:
                    func_800995F8(index);
                    break;
                case 18:
                    func_80099804(index);
                    break;
                case 19:
                    func_800999B0(index);
                    break;
                case 20:
                    func_80099B5C(index);
                    break;
                case 21:
                    func_80099C94(index);
                    break;
                
            }

        }
         
        setEntityDirection(gMiscAnimals[index].entityIndex, convertSpriteToWorldDirection(gMiscAnimals[index].direction, MAIN_MAP_INDEX));
        
        vec = getMovementVectorFromDirection(gMiscAnimals[index].zDisplacement, gMiscAnimals[index].direction, 0.0f);
        vec.y = gMiscAnimals[index].yDisplacement;
        
        setEntityMovementVector(gMiscAnimals[index].entityIndex, vec.x, vec.y, vec.z, gMiscAnimals[index].zDisplacement);
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80094A5C);

void func_80094A5C(u8 index) {
    
    u16 temp;
    
    switch (gMiscAnimals[index].unk_F) {

         case 0:

            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 10;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0);

             if (getRandomNumberInRange(0, 60) < 40) {
                gMiscAnimals[index].unk_F = 0;
            } else {
                 
                temp = getRandomNumberInRange(0, 20);

                if (temp < 7) {
                    gMiscAnimals[index].direction = temp;
                }

                temp = getRandomNumberInRange(0, 11);

                if (temp == 0) {
                    gMiscAnimals[index].unk_F = 1;
                }
                
                if (temp == 1) {
                    gMiscAnimals[index].unk_F = 2;
                }
                
                if (temp == 2) {
                    gMiscAnimals[index].unk_F = 3;
                }

                if (temp == 3) {
                    gMiscAnimals[index].unk_F = 4;
                }
                 
                if (temp == 4) {
                    gMiscAnimals[index].unk_F = 5;
                }
                
                if (temp == 5) {
                    gMiscAnimals[index].unk_F = 6;
                }
                
                if (temp == 6) {
                    gMiscAnimals[index].unk_F = 7;
                }

                if (temp == 9) {
                    gMiscAnimals[index].unk_F = 10;
                }
                 
                if (temp == 10) {
                    gMiscAnimals[index].unk_F = 24;
                }
                 
                if (temp == 11) {
                    gMiscAnimals[index].unk_F = 25;
                }             
                 
            }

            gMiscAnimals[index].flags |= 2;
        
            break;

        case 1:
            gMiscAnimals[index].zDisplacement = 1;
            gMiscAnimals[index].unk_13 = 10;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 8);

            temp = getRandomNumberInRange(0, 19);
            
            if (temp < 10) {
                gMiscAnimals[index].unk_F = 1;
            } else if (temp < 15) {
                gMiscAnimals[index].unk_F = 0;
            } else {
                gMiscAnimals[index].unk_F = 4;
            }
            
            gMiscAnimals[index].flags |= 2;

            break;
        
        case 2:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x30);
            
            gMiscAnimals[index].unk_F = 16;
            
            gMiscAnimals[index].flags |= 2;
        
            break;

        case 3:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 10;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x20);

            temp = getRandomNumberInRange(0, 20);
            
            if (temp < 10) {

                gMiscAnimals[index].unk_F = 3;

            } else {

                if (9 < temp && temp < 12) {
                    gMiscAnimals[index].unk_F = 0;
                } 
                if (11 < temp && temp < 14) {
                    gMiscAnimals[index].unk_F = 11;
                }
                if (13 < temp && temp < 16) {
                    gMiscAnimals[index].unk_F = 12;   
                }
                if (15 < temp && temp < 18) {
                    gMiscAnimals[index].unk_F = 13;   
                }
                if (17 < temp && temp < 20) {
                    gMiscAnimals[index].unk_F = 14;   
                }

            }
            
            gMiscAnimals[index].flags |= 2;
            break;

        case 4:
            gMiscAnimals[index].zDisplacement = 2;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x10);

            temp = getRandomNumberInRange(0, 19);
            
            if (temp >= 10) {

                if (9 < temp && temp < 13) {
                    gMiscAnimals[index].unk_F = 0;
                } 
                if (12 < temp && temp < 16) {
                    gMiscAnimals[index].unk_F = 1;
                }
                if (15 < temp && temp < 19) {
                    gMiscAnimals[index].unk_F = 21;   
                }

            } else {
                gMiscAnimals[index].unk_F = 4;
            }
            
            gMiscAnimals[index].flags |= 2;

            break;

        case 5:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x90);

            if (!getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].unk_F = 5;
            } else {
                gMiscAnimals[index].unk_F = 3;
            }
            
            gMiscAnimals[index].flags |= 2;
            break;

        case 6:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0xA0);

            if (getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].unk_F = 0;
            } else {
                gMiscAnimals[index].unk_F = 6;
            }
            
            gMiscAnimals[index].flags |= 2;
            break;
        
        case 7:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0xA8);

            if (getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].unk_F = 0;
            } else {
                gMiscAnimals[index].unk_F = 7;
            }
            
            gMiscAnimals[index].flags |= 2;
            break;

        case 8:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0xB0);

            if (getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].unk_F = 0;
            } else {
                gMiscAnimals[index].unk_F = 8;
            }
            
            gMiscAnimals[index].flags |= 2;
            break;

        case 9:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0xB8);

            gMiscAnimals[index].unk_F = 26;
            
            break;

        case 10:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 10;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0xC8);

            if (!getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].unk_F = 10;
            } else {
                gMiscAnimals[index].unk_F = 0;
            }
            
            gMiscAnimals[index].flags |= 2;
            break;

        case 11:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 10;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x28);

            if (!getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].unk_F = 11;
            } else {
                gMiscAnimals[index].unk_F = 3;
            }
            
            gMiscAnimals[index].flags |= 2;
            break;

        case 12:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 10;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x50);

            if (!getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].unk_F = 12;
            } else {
                gMiscAnimals[index].unk_F = 3;
            }
            
            gMiscAnimals[index].flags |= 2;

            break;
     
        case 13:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x58);

            if (!getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].unk_F = 13;
            } else {
                gMiscAnimals[index].unk_F = 3;
            }
            
            gMiscAnimals[index].flags |= 2;
            break;
        
        case 14:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x60);

            if (!getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].unk_F = 14;
            } else {
                gMiscAnimals[index].unk_F = 3;
            }
            
            gMiscAnimals[index].flags |= 2;
            break;

        case 16:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 10;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x28);

            temp = getRandomNumberInRange(0, 14);
            
            if (temp < 10) {
                gMiscAnimals[index].unk_F = 16;
            } 
            if (temp == 10) {
                gMiscAnimals[index].unk_F = 27;
            } 
            if (temp == 11) {
                gMiscAnimals[index].unk_F = 17;
            }  
            if (temp == 12) {
                gMiscAnimals[index].unk_F = 18;
            }
            if (temp == 13) {
                gMiscAnimals[index].unk_F = 19;
            }
            if (temp == 14) {
                gMiscAnimals[index].unk_F = 20;
            }
            
            gMiscAnimals[index].flags |= 2;
            break;
        
        case 17:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x68);

            if (!getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].unk_F = 17;
            } else {
                gMiscAnimals[index].unk_F = 16;
            }
            
            gMiscAnimals[index].flags |= 2;
            break;
                
        case 18:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x70);

            if (!getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].unk_F = 18;
            } else {
                gMiscAnimals[index].unk_F = 16;
            }
            
            gMiscAnimals[index].flags |= 2;
            break;
                
        case 19:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x78);

            if (!getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].unk_F = 19;
            } else {
                gMiscAnimals[index].unk_F = 16;
            }
            
            gMiscAnimals[index].flags |= 2;
            break;
                
        case 20:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 10;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x80);

            if (!getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].unk_F = 20;
            } else {
                gMiscAnimals[index].unk_F = 16;
            }
            
            gMiscAnimals[index].flags |= 2;
            break;
                 
        case 21:
            gMiscAnimals[index].zDisplacement = 1;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x38);

            gMiscAnimals[index].unk_F = 22;
            
            gMiscAnimals[index].flags |= 2;
            break;       
                 
        case 22:
            gMiscAnimals[index].zDisplacement = 2;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x40);

            gMiscAnimals[index].unk_F = 23;
            
            gMiscAnimals[index].flags |= 2;
            break;       

        case 23:
            gMiscAnimals[index].zDisplacement = 1;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x48);

            gMiscAnimals[index].unk_F = 0;
            
            gMiscAnimals[index].flags |= 2;
            break;      

        case 24:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x88);

            if (!getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].unk_F = 24;
            } else {
                gMiscAnimals[index].unk_F = 0;
            }
            
            gMiscAnimals[index].flags |= 2;
            break;      

        case 25:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x98);

            if (!getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].unk_F = 25;
            } else {
                gMiscAnimals[index].unk_F = 0;
            }
            
            gMiscAnimals[index].flags |= 2;
            break;      

        case 26:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0xC0);

            gMiscAnimals[index].unk_F = 0;
            
            gMiscAnimals[index].flags |= 2;
            break;      

        case 27:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x30);

            gMiscAnimals[index].unk_F = 0;
            
            gMiscAnimals[index].flags |= 2;
            break;      
        
     }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_800958EC);

void func_800958EC(u8 index) {

    u16 temp;
    
    switch (gMiscAnimals[index].unk_F) {

         case 0:

            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0);

             if (getRandomNumberInRange(0, 60) < 40) {
                gMiscAnimals[index].unk_F = 0;
            } else {
                 
                temp = getRandomNumberInRange(0, 20);

                if (temp < 7) {
                    gMiscAnimals[index].direction = temp;
                }

                temp = getRandomNumberInRange(0, 8);

                if (temp == 0) {
                    gMiscAnimals[index].unk_F = 1;
                }
                
                if (temp == 1) {
                    gMiscAnimals[index].unk_F = 2;
                }
                
                if (temp == 2) {
                    gMiscAnimals[index].unk_F = 3;
                }

                if (temp == 3) {
                    gMiscAnimals[index].unk_F = 4;
                }
                 
                if (temp == 4) {
                    gMiscAnimals[index].unk_F = 5;
                }
                
                if (temp == 5) {
                    gMiscAnimals[index].unk_F = 6;
                }
                
                if (temp == 6) {
                    gMiscAnimals[index].unk_F = 7;
                }

                if (temp == 7) {
                    gMiscAnimals[index].unk_F = 8;
                }
                 
                if (temp == 8) {
                    gMiscAnimals[index].unk_F = 9;
                }
                 
            }

            gMiscAnimals[index].flags |= 2;
        
            break;

        case 1:
            gMiscAnimals[index].zDisplacement = 1;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 8);

            temp = getRandomNumberInRange(0, 19);
            
            if (temp < 10) {
                gMiscAnimals[index].unk_F = 1;
            } else if (temp < 15) {
                gMiscAnimals[index].unk_F = 0;
            } else {
                gMiscAnimals[index].unk_F = 4;
            }
            
            gMiscAnimals[index].flags |= 2;

            break;
        
        case 2:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x30);
            
            gMiscAnimals[index].unk_F = 12;
            
            gMiscAnimals[index].flags |= 2;
        
            break;

        case 3:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x20);

            temp = getRandomNumberInRange(0, 20);
            
            if (temp >= 10) {

                if (9 < temp && temp < 13) {
                    gMiscAnimals[index].unk_F = 0;
                } 
                if (13 < temp && temp < 17) {
                    gMiscAnimals[index].unk_F = 10;
                }
                if (16 < temp && temp < 20) {
                    gMiscAnimals[index].unk_F = 11;   
                }
                
            } else {
                gMiscAnimals[index].unk_F = 3;
            }
            
            gMiscAnimals[index].flags |= 2;
            break;

        case 4:
            gMiscAnimals[index].zDisplacement = 2;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x10);

            temp = getRandomNumberInRange(0, 19);
            
            if (temp < 10) {
                gMiscAnimals[index].unk_F = 4;
            } else if (temp < 15) {
                gMiscAnimals[index].unk_F = 0;
            } else {
                gMiscAnimals[index].unk_F = 1;
            }
            
            gMiscAnimals[index].flags |= 2;

            break;

        case 5:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x50);

            if (!getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].unk_F = 5;
            } else {
                gMiscAnimals[index].unk_F = 3;
            }
            
            gMiscAnimals[index].flags |= 2;
            break;

        case 6:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x58);

            if (!getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].unk_F = 6;
            } else {
                gMiscAnimals[index].unk_F = 0;
            }
            
            gMiscAnimals[index].flags |= 2;
            break;
        
        case 7:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x60);

            if (!getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].unk_F = 7;
            } else {
                gMiscAnimals[index].unk_F = 0;
            }
            
            gMiscAnimals[index].flags |= 2;
            break;

        case 8:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x70);

            if (!getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].unk_F = 8;
            } else {
                gMiscAnimals[index].unk_F = 0;
            }
            
            gMiscAnimals[index].flags |= 2;
            break;

        case 9:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x68);

            if (!getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].unk_F = 9;
            } else {
                gMiscAnimals[index].unk_F = 0;
            }
            
            gMiscAnimals[index].flags |= 2;
            break;

        case 10:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x18);

            if (!getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].unk_F = 10;
            } else {
                gMiscAnimals[index].unk_F = 3;
            }
            
            gMiscAnimals[index].flags |= 2;
            break;

        case 11:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x38);

            if (!getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].unk_F = 11;
            } else {
                gMiscAnimals[index].unk_F = 3;
            }
            
            gMiscAnimals[index].flags |= 2;
            break;

        case 12:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x28);

            temp = getRandomNumberInRange(0, 12);
            
            if (temp < 10) {
                gMiscAnimals[index].unk_F = 12;
            } 
            if (temp == 10) {
                gMiscAnimals[index].unk_F = 15;
            } 
            if (temp == 11) {
                gMiscAnimals[index].unk_F = 13;
            }  
            if (temp == 12) {
                gMiscAnimals[index].unk_F = 14;
            }
            
            gMiscAnimals[index].flags |= 2;

            break;
     
        case 13:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x40);

            if (!getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].unk_F = 13;
            } else {
                gMiscAnimals[index].unk_F = 14;
            }
            
            gMiscAnimals[index].flags |= 2;
            break;
        
        case 14:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x48);

            if (!getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].unk_F = 14;
            } else {
                gMiscAnimals[index].unk_F = 14;
            }
            
            gMiscAnimals[index].flags |= 2;
            break;

        case 15:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x30);

            gMiscAnimals[index].unk_F = 0;
            
            gMiscAnimals[index].flags |= 2;
            break;
        
     }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80096264);

void func_80096264(u8 index) {
    
    u32 temp;
    u16 temp2;
    u16 temp3;
    u8 tempDirection;
    
    switch (gMiscAnimals[index].unk_F) {

        case 0:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0);

            if (getRandomNumberInRange(0, 60) < 40) {
                gMiscAnimals[index].unk_F = 0;
            } else {

                temp = getRandomNumberInRange(0, 10);

                if (temp < 7) {
                    temp &= 0xFFFE;
                    temp2 = temp;
                    gMiscAnimals[index].direction = temp2;
                }

                temp = getRandomNumberInRange(0, 4);

                if (temp == 0) {
                    gMiscAnimals[index].unk_F = 1;
                }
                
                if (temp == 1) {
                    gMiscAnimals[index].unk_F = 2;
                }
                
                if (temp == 2) {
                    gMiscAnimals[index].unk_F = 3;
                }

                if (temp == 3) {
                    gMiscAnimals[index].unk_F = 4;
                }

            }

            gMiscAnimals[index].flags |= 2;
        
            break;

        case 1:
            gMiscAnimals[index].zDisplacement = 1;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 8);

            temp2 = getRandomNumberInRange(0, 19);
            
            if (temp2 < 10) {
                gMiscAnimals[index].unk_F = 1;
            } else if (temp2 < 15) {
                gMiscAnimals[index].unk_F = 0;
            } else {
                gMiscAnimals[index].unk_F = 5;
            }
            
            gMiscAnimals[index].flags |= 2;

            playSfx(0x43);
            break;

        case 2:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x20);

            temp3 = getRandomNumberInRange(0, 20);
            
            if (temp3 >= 10) {

                if (9 < temp3 && temp3 < 13) {
                    gMiscAnimals[index].unk_F = 0;
                } 
                if (13 < temp3 && temp3 < 17) {
                    gMiscAnimals[index].unk_F = 6;
                }
                if (16 < temp3 && temp3 < 20) {
                    gMiscAnimals[index].unk_F = 7;   
                }
                
            } else {
                gMiscAnimals[index].unk_F = 2;
            }
            
            gMiscAnimals[index].flags |= 2;
            break;

        case 3:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x18);

            temp = getRandomNumberInRange(0, 19);

            if (temp >= 10) {

                if (temp >= 15) {
                    gMiscAnimals[index].unk_F = 2;
                } else {
                    gMiscAnimals[index].unk_F = 0;
                }
                
            } else {
                gMiscAnimals[index].unk_F = 3;
            }
            
            gMiscAnimals[index].flags |= 2;
            break;
        
        case 4:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimation(gMiscAnimals[index].entityIndex, 0x38);

            if (!getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].unk_F = 4;
            } else {
                gMiscAnimals[index].unk_F = 0;
            }
            
            gMiscAnimals[index].flags |= 2;
            break;
        
        case 5:
            gMiscAnimals[index].zDisplacement = 1;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x10);

            if (!getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].unk_F = 5;
            } else {
                gMiscAnimals[index].unk_F = 1;
            }
            
            gMiscAnimals[index].flags |= 2;
            break;
        
       case 6:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x30);

            if (!getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].unk_F = 6;
            } else {
                gMiscAnimals[index].unk_F = 2;
            }
            
            gMiscAnimals[index].flags |= 2;
            break;     

        case 7:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimation(gMiscAnimals[index].entityIndex, 0x39);

            if (!getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].unk_F = 7;
            } else {
                gMiscAnimals[index].unk_F = 2;
            }

            gMiscAnimals[index].flags |= 2;
            break;

    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_800967E0);

void func_800967E0(u8 index) {

    u16 temp;
    
    switch (gMiscAnimals[index].unk_F) {

         case 0:

            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0);

             if (getRandomNumberInRange(0, 60) < 56) {
                gMiscAnimals[index].unk_F = 0;
            } else {
                 
                temp = getRandomNumberInRange(0, 8);

                if (temp < 7) {
                    gMiscAnimals[index].direction = temp;
                }

                 gMiscAnimals[index].unk_F = 1;
                 
            }

            gMiscAnimals[index].flags |= 2;
        
            break;

        case 1:
            gMiscAnimals[index].zDisplacement = 1;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 8);
            
            if (getRandomNumberInRange(0, 19) < 4) {
                gMiscAnimals[index].unk_F = 1;
            } else {
                gMiscAnimals[index].unk_F = 0;
            }
            
            gMiscAnimals[index].flags |= 2;
        
            break;

     }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80096978);

void func_80096978(u8 index) {

    u16 temp;
    u8 tempDirection;
    
    switch (gMiscAnimals[index].unk_F) {

        case 0:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0);

            if (getRandomNumberInRange(0, 60) < 40) {
                gMiscAnimals[index].unk_F = 0;
            } else {

                temp = getRandomNumberInRange(0, 20);

                if (temp < 7) {
                    gMiscAnimals[index].direction = temp;
                }

                temp = getRandomNumberInRange(0, 2);

                if (temp == 0) {
                    gMiscAnimals[index].unk_F = 1;
                }
                
                if (temp == 1) {
                    gMiscAnimals[index].unk_F = 2;
                }
                
                if (temp == 2) {
                    gMiscAnimals[index].unk_F = 3;
                }

            }

            gMiscAnimals[index].flags |= 2;
        
            break;

        case 1:
            gMiscAnimals[index].zDisplacement = 1;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 8);

            if (getRandomNumberInRange(0, 20) < 10) {
                gMiscAnimals[index].unk_F = 1;
            } else {
                gMiscAnimals[index].unk_F = 0;
            }
            
            gMiscAnimals[index].flags |= 2;
        
            break;

        case 2:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x10);

            if (!getRandomNumberInRange(0, 2)) {
                gMiscAnimals[index].unk_F = 2;
            } else {
                gMiscAnimals[index].unk_F = 0;
            }
            
            gMiscAnimals[index].flags |= 2;
            break;

        case 3:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x18);

            gMiscAnimals[index].unk_F = 4;
            
            gMiscAnimals[index].flags |= 2;
            break;
        
        case 4:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x20);

            temp = getRandomNumberInRange(0, 11);
            
            if (temp < 10) {
                gMiscAnimals[index].unk_F = 4;
            }
            
            if (temp == 10) {
                gMiscAnimals[index].unk_F = 4;
            }
            
            if (temp == 11) {
                gMiscAnimals[index].unk_F = 6;
            }

            
            gMiscAnimals[index].flags |= 2;
            break;
        
        case 5:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x28);

            if (getRandomNumberInRange(0, 4) < 4) {
                gMiscAnimals[index].unk_F = 5;
            } else {
                gMiscAnimals[index].unk_F = 4;
            }
            
            gMiscAnimals[index].flags |= 2;
            break;
        
       case 6:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x18);

            gMiscAnimals[index].unk_F = 0;
            
            gMiscAnimals[index].flags |= 2;
            break;     

    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80096DFC);

void func_80096DFC(u8 index) {

    u16 temp;
    u8 tempDirection;
    
    switch (gMiscAnimals[index].unk_F) {

        case 0:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0);

            if (getRandomNumberInRange(0, 60) < 40) {
                gMiscAnimals[index].unk_F = 0;
            } else {

                temp = getRandomNumberInRange(0, 20);

                if (temp < 7) {
                    gMiscAnimals[index].direction = temp;
                }

                temp = getRandomNumberInRange(0, 5);

                if (temp == 0) {
                    gMiscAnimals[index].unk_F = 1;
                }
                
                if (temp == 1) {
                    gMiscAnimals[index].unk_F = 2;
                }
                
                if (temp == 2) {
                    gMiscAnimals[index].unk_F = 3;
                }
                
                if (temp == 3) {
                    gMiscAnimals[index].unk_F = 4;
                }

                if (temp == 4) {
                    gMiscAnimals[index].unk_F = 5;
                }
                
                if (temp == 5) {
                    gMiscAnimals[index].unk_F = 6;
                }
            }

            gMiscAnimals[index].flags |= 2;
        
            break;

        case 1:
            gMiscAnimals[index].zDisplacement = 1;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 8);

            if (getRandomNumberInRange(0, 20) < 10) {
                gMiscAnimals[index].unk_F = 1;
            } else {
                gMiscAnimals[index].unk_F = 0;
            }
            
            gMiscAnimals[index].flags |= 2;
        
            break;

        case 2:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x20);

            temp = getRandomNumberInRange(0, 2);
            
            if (temp == 0) {
                gMiscAnimals[index].unk_F = 2;
            }
            
            if (temp == 1) {
                gMiscAnimals[index].unk_F = 0;
            }
            
            if (temp == 2) {
                gMiscAnimals[index].unk_F = 9;
            }
            
            gMiscAnimals[index].flags |= 2;
            break;

        case 3:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x10);

            gMiscAnimals[index].unk_F = 7;
            
            gMiscAnimals[index].flags |= 2;
            break;
        
        case 4:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x38);

            gMiscAnimals[index].unk_F = 0;
            
            gMiscAnimals[index].flags |= 2;
            break;
        
        case 5:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x40);

            gMiscAnimals[index].unk_F = 0;
            
            gMiscAnimals[index].flags |= 2;
            break;
        
       case 6:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x48);

            gMiscAnimals[index].unk_F = 0;
            
            gMiscAnimals[index].flags |= 2;
            break;     

        case 7:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x18);

            temp = getRandomNumberInRange(0, 11);
            
            if (temp < 10) {
                gMiscAnimals[index].unk_F = 7;
            }
            
            if (temp == 10) {
                gMiscAnimals[index].unk_F = 8;
            }
            
            if (temp == 11) {
                gMiscAnimals[index].unk_F = 10;
            }

            gMiscAnimals[index].flags |= 2;
            
            break;

        case 8:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x28);

            if (getRandomNumberInRange(0, 4) < 4) {
                 gMiscAnimals[index].unk_F = 8;
            } else {
                 gMiscAnimals[index].unk_F = 7;
            }
            
            gMiscAnimals[index].flags |= 2;
            break;
       
        case 9:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x30);

            gMiscAnimals[index].unk_F = 2;
            
            gMiscAnimals[index].flags |= 2;
            break;

        case 10:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x10);

            gMiscAnimals[index].unk_F = 0;
            
            gMiscAnimals[index].flags |= 2;
            break;


    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_800973F8);

void func_800973F8(u8 index) {
    
    u16 temp;
    u8 tempDirection;
    
    switch (gMiscAnimals[index].unk_F) {

        case 0:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0);

            if (getRandomNumberInRange(0, 60) < 40) {
                
                gMiscAnimals[index].unk_F = 0;
                
            } else {
                temp = getRandomNumberInRange(0, 10);

                if (temp < 7) {
                    gMiscAnimals[index].direction = temp;
                }

                temp = getRandomNumberInRange(0, 2);

                if (temp == 0) {
                    gMiscAnimals[index].unk_F = 1;
                }
                
                if (temp == 1) {
                    gMiscAnimals[index].unk_F = 2;
                }
                
                if (temp == 2) {
                    gMiscAnimals[index].unk_F = 3;
                }
                
            }

            gMiscAnimals[index].flags |= 2;
        
            break;

        case 1:
            gMiscAnimals[index].zDisplacement = 1;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 8);

            if (getRandomNumberInRange(0, 19) < 10) {
                gMiscAnimals[index].unk_F = 1;
            } else {
                gMiscAnimals[index].unk_F = 0;
            }
            
            gMiscAnimals[index].flags |= 2;
        
            break;

        case 2:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x10);

            if (getRandomNumberInRange(0, 20) < 10) {
                gMiscAnimals[index].unk_F = 2;
            } else {
                gMiscAnimals[index].unk_F = 0;
            }
            
            gMiscAnimals[index].flags |= 2;
            break;

        case 3:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x18);
            
            if (getRandomNumberInRange(0, 19) < 10) {
                gMiscAnimals[index].unk_F = 3;
            } else {
                gMiscAnimals[index].unk_F = 0;
            }

            gMiscAnimals[index].flags |= 2;
            break;

        case 32:
            gMiscAnimals[index].zDisplacement = 4;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 8);

            tempDirection = calculateAnimalDirectionToPlayer(entities[gMiscAnimals[index].entityIndex].coordinates.x, entities[gMiscAnimals[index].entityIndex].coordinates.z, entities[ENTITY_PLAYER].coordinates.x, entities[ENTITY_PLAYER].coordinates.z);

            gMiscAnimals[index].direction = (tempDirection + 4) % 8;
            
            if (!getRandomNumberInRange(0, 30)) {
                gMiscAnimals[index].flags &= ~(0x10);
            }
            
            gMiscAnimals[index].flags |= 2;
            
            break;

    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80097804);

void func_80097804(u8 index) {

    u16 temp;
    u8 tempDirection;
    
    switch (gMiscAnimals[index].unk_F) {

        case 0:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0);

            if (getRandomNumberInRange(0, 60) < 40) {
                gMiscAnimals[index].unk_F = 0;
            } else {

                temp = getRandomNumberInRange(0, 10);

                if (temp < 7) {
                    gMiscAnimals[index].direction = temp;
                }

                temp = getRandomNumberInRange(0, 2);

                if (temp == 0) {
                    gMiscAnimals[index].unk_F = 1;
                }
                
                if (temp == 1) {
                    gMiscAnimals[index].unk_F = 2;
                }
                
                if (temp == 2) {
                    gMiscAnimals[index].unk_F = 3;
                }
                
            }

            gMiscAnimals[index].flags |= 2;
        
            break;

        case 1:
            gMiscAnimals[index].zDisplacement = 1;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 8);

            if (getRandomNumberInRange(0, 19) < 10) {
                gMiscAnimals[index].unk_F = 1;
            } else {
                gMiscAnimals[index].unk_F = 0;
            }
            
            gMiscAnimals[index].flags |= 2;
        
            break;

        case 2:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x10);

            if (getRandomNumberInRange(0, 20) < 10) {
                gMiscAnimals[index].unk_F = 2;
            } else {
                gMiscAnimals[index].unk_F = 0;
            }
            
            gMiscAnimals[index].flags |= 2;
            break;

        case 3:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;

            if (getRandomNumberInRange(0, 1)) {
                setEntityAnimation(gMiscAnimals[index].entityIndex, 0x18);
            } else {
                setEntityAnimation(gMiscAnimals[index].entityIndex, 0x19);
            }

            gMiscAnimals[index].unk_F = 0;
            
            gMiscAnimals[index].flags |= 2;
            break;

        case 32:
            gMiscAnimals[index].zDisplacement = 4;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 8);

            tempDirection = calculateAnimalDirectionToPlayer(entities[gMiscAnimals[index].entityIndex].coordinates.x, entities[gMiscAnimals[index].entityIndex].coordinates.z, entities[ENTITY_PLAYER].coordinates.x, entities[ENTITY_PLAYER].coordinates.z);

            gMiscAnimals[index].direction = (tempDirection + 4) % 8;
            
            if (!getRandomNumberInRange(0, 30)) {
                gMiscAnimals[index].flags &= ~(0x10);
            }
            
            gMiscAnimals[index].flags |= 2;
            
            break;

    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80097CB0);

void func_80097CB0(u8 index) {

    u16 temp;
    u8 tempDirection;
    
    switch (gMiscAnimals[index].unk_F) {

        case 0:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0);

            if (getRandomNumberInRange(0, 60) < 40) {
                
                gMiscAnimals[index].unk_F = 0;
                
            } else {
                
                temp = getRandomNumberInRange(0, 10);

                if (temp < 7) {
                    gMiscAnimals[index].direction = temp;
                }

                temp = getRandomNumberInRange(0, 1);

                if (temp == 0) {
                    gMiscAnimals[index].unk_F = 1;
                }
                
                if (temp == 1) {
                    gMiscAnimals[index].unk_F = 2;
                }
                
            } 
            
            gMiscAnimals[index].flags |= 2;
        
            break;


        case 1:
            gMiscAnimals[index].zDisplacement = 1;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 8);

            if (getRandomNumberInRange(0, 19) < 10) {
                gMiscAnimals[index].unk_F = 1;
            } else {
                gMiscAnimals[index].unk_F = 0;
            }
            
            gMiscAnimals[index].flags |= 2;
            
            break;
        
        case 2:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x10);

            if (getRandomNumberInRange(0, 20) < 10) {
                gMiscAnimals[index].unk_F = 2;
            } else {
                gMiscAnimals[index].unk_F = 0;
            }
            
            gMiscAnimals[index].flags |= 2;
            break;


    
        case 16:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0);

            gMiscAnimals[index].unk_F = 0x10;
            gMiscAnimals[index].flags |= 2;
            
            break;

        case 17:
            setEntityCollidable(gMiscAnimals[index].entityIndex, 0);
            setEntityYMovement(gMiscAnimals[index].entityIndex, 0);
            setEntityShadow(gMiscAnimals[index].entityIndex, 0xFF);

            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].yDisplacement = 2;

            setEntityAnimation(gMiscAnimals[index].entityIndex, 0x19);

            gMiscAnimals[index].unk_13++;
            gMiscAnimals[index].flags |= 2;

            if (gMiscAnimals[index].unk_13 == 6) {
                deactivateEntity(gMiscAnimals[index].entityIndex);
                gMiscAnimals[index].flags = 0;
            }
            
            break;
        
       case 32:
            gMiscAnimals[index].zDisplacement = 4;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 8);
            
            tempDirection = calculateAnimalDirectionToPlayer(entities[gMiscAnimals[index].entityIndex].coordinates.x, entities[gMiscAnimals[index].entityIndex].coordinates.z, entities[ENTITY_PLAYER].coordinates.x, entities[ENTITY_PLAYER].coordinates.z);

            gMiscAnimals[index].direction = (tempDirection + 4) % 8;
            
            if (!getRandomNumberInRange(0, 30)) {
                gMiscAnimals[index].flags &= ~(0x10);
            }
            
            gMiscAnimals[index].flags |= 2;
            break;

    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80098188);

void func_80098188(u8 index) {

    u16 temp;
    u8 tempDirection;
    
    switch (gMiscAnimals[index].unk_F) {

        case 0:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0);

            if (getRandomNumberInRange(0, 60) >= 40) {

                temp = getRandomNumberInRange(0, 10);

                if (temp < 7) {
                    gMiscAnimals[index].direction = temp;
                }

                temp = getRandomNumberInRange(0, 2);

                if (temp == 0) {
                    gMiscAnimals[index].unk_F = 1;
                }
                
                if (temp == 1) {
                    gMiscAnimals[index].unk_F = 2;
                }
                
                if (temp == 2) {
                    gMiscAnimals[index].unk_F = 3;
                }
                
            } else {
                gMiscAnimals[index].unk_F = 0;
            }

            gMiscAnimals[index].flags |= 2;
        
            break;


        case 1:
            gMiscAnimals[index].zDisplacement = 1;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 8);

            if (getRandomNumberInRange(0, 19) < 10) {
                gMiscAnimals[index].unk_F = 1;
            } else {
                gMiscAnimals[index].unk_F = 0;
            }
            
            gMiscAnimals[index].flags |= 2;
            break;



        case 2:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0x10);

            if (getRandomNumberInRange(0, 20) < 10) {
                gMiscAnimals[index].unk_F = 2;
            } else {
                gMiscAnimals[index].unk_F = 0;
            }
            
            gMiscAnimals[index].flags |= 2;
            playSfx(0x4D);
            
            break;


        case 3:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimation(gMiscAnimals[index].entityIndex, 0x22);

            gMiscAnimals[index].unk_F = 0;
            gMiscAnimals[index].flags |= 2;
            break;
    
        case 16:
            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0);

            gMiscAnimals[index].unk_F = 0x10;
            gMiscAnimals[index].flags |= 2;
            
            break;

        case 17:

            setEntityCollidable(gMiscAnimals[index].entityIndex, 0);
            setEntityYMovement(gMiscAnimals[index].entityIndex, 0);
            setEntityShadow(gMiscAnimals[index].entityIndex, 0xFF);

            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].yDisplacement = 2;

            setEntityAnimation(gMiscAnimals[index].entityIndex, 0x1D);

            gMiscAnimals[index].unk_13++;
            gMiscAnimals[index].flags |= 2;

            if (gMiscAnimals[index].unk_13 == 6) {
                deactivateEntity(gMiscAnimals[index].entityIndex);
                gMiscAnimals[index].flags = 0;
            }
            
            break;
        
       case 32:
            gMiscAnimals[index].zDisplacement = 4;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 8);
            
            tempDirection = calculateAnimalDirectionToPlayer(entities[gMiscAnimals[index].entityIndex].coordinates.x, entities[gMiscAnimals[index].entityIndex].coordinates.z, entities[ENTITY_PLAYER].coordinates.x, entities[ENTITY_PLAYER].coordinates.z);

            gMiscAnimals[index].direction = (tempDirection + 4) % 8;
            
            if (!getRandomNumberInRange(0, 30)) {
                gMiscAnimals[index].flags &= ~(0x10);
            }
            
            gMiscAnimals[index].flags |= 2;
            break;


    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80098740);

void func_80098740(u8 index) {

    u16 temp;
    
    switch (gMiscAnimals[index].unk_F) {

        case 0:

            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;
            
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0);
            
            if (getRandomNumberInRange(0, 60) < 40) {

                gMiscAnimals[index].unk_F = 0;
            
            } else {
                
                temp = getRandomNumberInRange(0, 10);
                
                if (temp < 7) {
                    gMiscAnimals[index].direction = temp;
                }

                if (getRandomNumberInRange(0, 30) >= 28) {
                    gMiscAnimals[index].unk_F = 32;
                } else {
                    gMiscAnimals[index].unk_F = 1;
                }
                
            }
            
            gMiscAnimals[index].flags |= 2;

            break;

        case 1:

            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
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
                gMiscAnimals[index].unk_F = 1;
            } else {
                gMiscAnimals[index].unk_F = 0;
            }
            
            gMiscAnimals[index].flags |= 2;

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
            
            gMiscAnimals[index].flags |= 2;

            if (gMiscAnimals[index].unk_13 < 129) {
                gMiscAnimals[index].zDisplacement = 4;
                gMiscAnimals[index].yDisplacement = 2;
                gMiscAnimals[index].unk_13++;
            } else {
                deactivateEntity(gMiscAnimals[index].entityIndex);
                gMiscAnimals[index].flags = 0;
            }
            
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80098B3C);

void func_80098B3C(u8 index) {
    
    u16 temp;
    
    switch (gMiscAnimals[index].unk_F) {

        case 0:

            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;
            
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0);
            
            if (getRandomNumberInRange(0, 60) < 40) {

                gMiscAnimals[index].unk_F = 0;
            
            } else {
                
                temp = getRandomNumberInRange(0, 10);
                
                if (temp < 7) {
                    gMiscAnimals[index].direction = temp;
                }

                if (getRandomNumberInRange(0, 30) >= 28) {
                    gMiscAnimals[index].unk_F = 32;
                } else {
                    gMiscAnimals[index].unk_F = 1;
                }
                
            }
            
            gMiscAnimals[index].flags |= 2;

            break;

        case 1:

            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
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
                gMiscAnimals[index].unk_F = 1;
            } else {
                gMiscAnimals[index].unk_F = 0;
            }
            
            gMiscAnimals[index].flags |= 2;

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
            gMiscAnimals[index].flags |= 2;
            gMiscAnimals[index].unk_13++;
                    
            break;

    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80098F24);

void func_80098F24(u8 index) {
    
    u32 temp;
    u16 temp2;
    
    switch (gMiscAnimals[index].unk_F) {

        case 0:

            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
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
                
                gMiscAnimals[index].unk_F = 1;
            
            } else {
                gMiscAnimals[index].unk_F = 0;
            }
            
            gMiscAnimals[index].flags |= 2;

            break;

        case 1:

            gMiscAnimals[index].zDisplacement = 1;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;
            
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 8);

            if (getRandomNumberInRange(0, 19) < 10) {
                gMiscAnimals[index].unk_F = 1;
            } else {
                gMiscAnimals[index].unk_F = 0;
            }
            
            gMiscAnimals[index].flags |= 2;

            break;

        case 32:

            setEntityCollidable(gMiscAnimals[index].entityIndex, 0);
            setEntityYMovement(gMiscAnimals[index].entityIndex, 0);
            gMiscAnimals[index].direction = SOUTHEAST;
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 8);
            
            gMiscAnimals[index].flags |= 2;

            if (gMiscAnimals[index].unk_13 < 17) {
                gMiscAnimals[index].zDisplacement = 4;
                gMiscAnimals[index].yDisplacement = 0;
                gMiscAnimals[index].unk_13++;
            } else {
                deactivateEntity(gMiscAnimals[index].entityIndex);
                gMiscAnimals[index].flags = 0;
            }
                    
            break;
            
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_800991C0);

void func_800991C0(u8 index) {

    u16 temp;
    
    switch (gMiscAnimals[index].unk_F) {

        case 0:

            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;
            
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 0);
            
            if (getRandomNumberInRange(0, 60) >= 40) {
                
                temp = getRandomNumberInRange(0, 10);
                
                if (temp < 7) {
                    gMiscAnimals[index].direction = temp;
                }
                
                gMiscAnimals[index].unk_F = 1;
            
            } else {
                gMiscAnimals[index].unk_F = 0;
            }
            
            gMiscAnimals[index].flags |= 2;

            break;

        case 1:

            gMiscAnimals[index].zDisplacement = 1;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;
            
            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 8);

            if (getRandomNumberInRange(0, 19) < 10) {
                gMiscAnimals[index].unk_F = 1;
            } else {
                gMiscAnimals[index].unk_F = 0;
            }
            
            gMiscAnimals[index].flags |= 2;

            break;

        case 32:

            setEntityAnimationWithDirectionChange(gMiscAnimals[index].entityIndex, 8);
            
            gMiscAnimals[index].flags |= 2;

            if (gMiscAnimals[index].unk_13 < 33) {
                gMiscAnimals[index].zDisplacement = 4;
                gMiscAnimals[index].yDisplacement = 0;
                gMiscAnimals[index].unk_13++;
            } else {
                deactivateEntity(gMiscAnimals[index].entityIndex);
                gMiscAnimals[index].flags = 0;
            }
                    
            break;
            
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80099424);

void func_80099424(u8 index) {

    switch (gMiscAnimals[index].unk_F) {

        case 0:
        case 1:

            setEntityCollidable(gMiscAnimals[index].entityIndex, FALSE);
            setEntityTracksCollisions(gMiscAnimals[index].entityIndex, FALSE);
    
            entities[gMiscAnimals[index].entityIndex].yOffset = 0x20;
    
            gMiscAnimals[index].zDisplacement = 1;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;
    
            setEntityAnimation(gMiscAnimals[index].entityIndex, 2);
    
            gMiscAnimals[index].direction = getRandomNumberInRange(0, 7);
            gMiscAnimals[index].unk_F = 0;
            gMiscAnimals[index].flags |= 2;
            
            break;
        
        default:
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80099548);

void func_80099548(u8 index) {

    switch (gMiscAnimals[index].unk_F) {

        case 0:
        case 1:

            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;
    
            setEntityAnimation(gMiscAnimals[index].entityIndex, 0);

            gMiscAnimals[index].unk_F = 0;
            gMiscAnimals[index].flags |= 2;

            break;
        
        default:
            break;
    
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_800995F8);

void func_800995F8(u8 index) {

    switch (gMiscAnimals[index].unk_F) {

        case 0:

            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimation(gMiscAnimals[index].entityIndex, 0);

            entities[gMiscAnimals[index].entityIndex].yOffset = 0x40;

            gMiscAnimals[index].flags |= 2;

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

            gMiscAnimals[index].flags |= 2;

            gMiscAnimals[index].unk_13++;
            
            break;    
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80099804);

void func_80099804(u8 index) {

     switch (gMiscAnimals[index].unk_F) {

        case 0:

            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimation(gMiscAnimals[index].entityIndex, 0);

            gMiscAnimals[index].flags |= 2;

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

            gMiscAnimals[index].flags |= 2;

            gMiscAnimals[index].unk_13++;
            
            break;    
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_800999B0);

void func_800999B0(u8 index) {

    switch (gMiscAnimals[index].unk_F) {

        case 0:

            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimation(gMiscAnimals[index].entityIndex, 0);

            gMiscAnimals[index].flags |= 2;

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

            gMiscAnimals[index].flags |= 2;

            gMiscAnimals[index].unk_13++;
            
            break;    
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80099B5C);

void func_80099B5C(u8 index) {

    u16 temp;
    
    switch (gMiscAnimals[index].unk_F) {

        case 0:
        case 1:

            setEntityCollidable(gMiscAnimals[index].entityIndex, FALSE);
            setEntityTracksCollisions(gMiscAnimals[index].entityIndex, FALSE);
                        
            entities[gMiscAnimals[index].entityIndex].yOffset = 0x20;

            gMiscAnimals[index].zDisplacement = 1;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimation(gMiscAnimals[index].entityIndex, 2);

            temp = getRandomNumberInRange(0, 20);

            if (temp < 7) {
                gMiscAnimals[index].direction = temp;
            }
            
            gMiscAnimals[index].unk_F = 0;
            gMiscAnimals[index].flags |= 2;
            
            break;    
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80099C94);

void func_80099C94(u8 index) {
    
    switch (gMiscAnimals[index].unk_F) {

        case 0:

            gMiscAnimals[index].zDisplacement = 0;
            gMiscAnimals[index].unk_13 = 0;
            gMiscAnimals[index].unk_14 = 0;

            setEntityAnimation(gMiscAnimals[index].entityIndex, 0);

            gMiscAnimals[index].flags |= 2;

            break;
        
        case 1:

            setEntityAnimation(gMiscAnimals[index].entityIndex, 2);
            gMiscAnimals[index].flags |= 2;

            if (gMiscAnimals[index].unk_13 < 0x21) {
                gMiscAnimals[index].zDisplacement = 2;
                gMiscAnimals[index].yDisplacement = 0;
                gMiscAnimals[index].unk_13++;
            } else {
                deactivateEntity(gMiscAnimals[index].entityIndex);
                gMiscAnimals[index].flags = 0;
            }
            
            break;    
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80099DE8);

void func_80099DE8(void) {
    
    u8 i = 0;
    bool set = FALSE;

    func_800305CC(ENTITY_PLAYER, 0.0f, 8.0f, 0);
    
    while (i < MAX_FARM_ANIMALS && !set) {
        
        if ((gFarmAnimals[i].flags & 4) && (entities[gFarmAnimals[i].entityIndex].entityCollidedWithIndex == ENTITY_PLAYER)) {
            func_800861A4(2, i, 0xFF, 0xFF, 0x10);
            set = TRUE;
            gFarmAnimals[i].flags |= 0x8000; 
        }
        
        i++;
        
    } 
        
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80099EEC);

void func_80099EEC(void) {

    u8 i = 0;
    bool set = FALSE;

    func_800305CC(ENTITY_PLAYER, 0.0f, 8.0f, 0);

    while (i < MAX_CHICKENS && !set) {

        if ((gChickens[i].flags & 4) && entities[gChickens[i].entityIndex].entityCollidedWithIndex == ENTITY_PLAYER) {

            func_800861A4(1, i, 0xFF, 0xFF, 0x10);
            set = TRUE;
            gChickens[i].flags |= 0x40;

        }

        i++;
        
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80099FF0);

void func_80099FF0(void) {

    func_800305CC(ENTITY_PLAYER, 0.0f, 8.0f, 0);
    
    if ((dogInfo.flags & 4) && (entities[dogInfo.entityIndex].entityCollidedWithIndex == ENTITY_PLAYER)) {
        dogInfo.unk_17 = 0x20;
        dogInfo.flags |= 0x20;
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009A074);

bool func_8009A074(void) {
    
    bool result;

    func_800305CC(ENTITY_PLAYER, 0.0f, 8.0f, 0);
    
    result = 0;

    if ((horseInfo.flags & 4) && (entities[horseInfo.entityIndex].entityCollidedWithIndex == ENTITY_PLAYER)) {
        result = 1;
        horseInfo.unk_17 = 0x10;
        horseInfo.flags |= 0x20;
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009A100);

bool func_8009A100(void) {
    
    bool result = FALSE;

    if ((horseInfo.flags & 4) && (horseInfo.grown == TRUE) && (entities[horseInfo.entityIndex].entityCollidedWithIndex == ENTITY_PLAYER)) {
        
        horseInfo.unk_17 = 0x10;
        horseInfo.flags |=  0x20;
        result = TRUE;
        
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009A17C);

bool func_8009A17C(void) {

    u8 i = 0;
    bool set = FALSE;

    while (i < MAX_FARM_ANIMALS && !set) {

        if ((gFarmAnimals[i].flags & 4) && gFarmAnimals[i].unk_1B == 16) {

            func_800861A4(2, i, 0xFF, 0xFF, 0);

            if (!(gFarmAnimals[i].flags & 0x20)) {

                switch (gFarmAnimals[i].type) {

                    case BABY_COW ... PREGNANT_COW:
                        func_80086458(i, 2);
                        func_800861A4(2, i, 0xFF, 0xFF, 0x11);
                        gFarmAnimals[i].flags |= (0x20 | 0x8000);
                        func_8009BCC4(2, i, 3);
                        set = TRUE;
                        break;

                    case BABY_SHEEP ... SHEARED_SHEEP:
                        func_80086458(i, 2);
                        func_800861A4(2, i, 0xFF, 0xFF, 0x11);
                        gFarmAnimals[i].flags |= (0x20 | 0x8000);
                        func_8009BCC4(2, i, 3);
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

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009A2D0);

// brush horse
bool func_8009A2D0(void) {

    bool result = FALSE;
    
    if (horseInfo.flags & 4) {

        if (horseInfo.unk_17 == 0x10) {
            
            horseInfo.unk_17 = 0;
            
            if (!(horseInfo.flags & 0x100)) {
                
                horseInfo.unk_17 = 0x11;
                
                adjustHorseAffection(2);
            
                horseInfo.flags |= (0x20 | 0x100);
                func_8009BCC4(1, 0, 3);
                result = TRUE;
            
            }
                    
        }
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009A398);

bool func_8009A398(void) {

    bool result = FALSE;
    
    if ((horseInfo.flags & 4) && (horseInfo.unk_17 == 0x10)) {
        
        horseInfo.unk_17 = 0;
        horseInfo.flags |= 0x20;
        func_8009BCC4(1, 0, 1);
        
        result = TRUE;
    
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009A400);

bool func_8009A400(void) {

    u8 i = 0;
    bool set = FALSE;

    while (i < MAX_FARM_ANIMALS && !set) {

        if ((gFarmAnimals[i].flags & 4) && gFarmAnimals[i].unk_1B == 16) {

            func_800861A4(2, i, 0xFF, 0xFF, 0);

            switch (gFarmAnimals[i].type) {

                case ADULT_COW:
                case ADULT_SHEEP ... SHEARED_SHEEP:
                    if (gFarmAnimals[i].condition == 3) {
                        func_800861A4(2, i, 0xFF, 0, 0);
                        set = TRUE;
                    } 
                    break;
                
            }
            
        }

        i++;
        
    } 

    return set;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009A53C);

bool func_8009A53C(void) {

    u8 i = 0;
    bool set = FALSE;

    while (i < MAX_FARM_ANIMALS && !set) {

        if ((gFarmAnimals[i].flags & 4) && gFarmAnimals[i].unk_1B == 16) {

            func_800861A4(2, i, 0xFF, 0xFF, 0);

            if (!(gFarmAnimals[i].flags & 0x40)) {

                switch (gFarmAnimals[i].type) {

                    case ADULT_COW:
                        
                        if (gFarmAnimals[i].condition < COW_MAD) {
                            func_80086458(i, 1);
                            func_800861A4(2, i, 0xFF, 0xFF, 0x11);
                            gPlayer.heldItem = func_8008662C(i, 0);
                            gFarmAnimals[i].flags |= (0x40 | 0x8000);
                            func_8009BCC4(2, i, 3);
                            set = TRUE;
                        }
                        
                        break;
                    
                    case PREGNANT_COW:

                        func_80086458(i, 1);
                        func_800861A4(2, i, 0xFF, 0xFF, 0x11);

                        gPlayer.heldItem = func_8008662C(i, 0);

                        if (gPlayer.heldItem != 0x15) {
                            gPlayer.heldItem--;
                        }

                        gFarmAnimals[i].flags |= (0x40 | 0x8000);
                        func_8009BCC4(2, i, 3);
                        set = TRUE;
                        
                        break;
                    
                    
                }
                
            } 
            
        }
        
        i++;
        
    }

    return set;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009A810);

u8 func_8009A810(void) {

    u8 i = 0;
    bool set = FALSE;

    if (func_8009B374() < 8) {
        
        while (i < MAX_FARM_ANIMALS && !set) {
    
            if ((gFarmAnimals[i].flags & 4) && gFarmAnimals[i].unk_1B == 16) {
    
                func_800861A4(2, i, 0xFF, 0xFF, 0);
                
                if (!(gFarmAnimals[i].flags & COW_PREGNANT) && gFarmAnimals[i].type == ADULT_COW) {
                
                    func_80086458(i, 10);
                    func_800861A4(2, i, 0xFF, COW_NORMAL, 0x11);
                    
                    gFarmAnimals[i].flags |= (COW_PREGNANT | 0x8000);
                    
                    func_8009BCC4(2, i, 3);
                    set = TRUE;
                    
                }

            }
    
            i++;
            
        }
    }

    return set;

}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009A97C);

bool func_8009A97C(void) {

    u8 i = 0;
    bool set = FALSE;

    while (i < MAX_FARM_ANIMALS && !set) {

        if ((gFarmAnimals[i].flags & 4) && gFarmAnimals[i].unk_1B == 16) {

            func_800861A4(2, i, 0xFF, 0xFF, 0);

            switch (gFarmAnimals[i].type) {

                case ADULT_COW:
                    
                if (gFarmAnimals[i].condition < COW_MAD) {
                        func_800861A4(2, i, 0xFF, 0xFF, 0x11);
                        set = TRUE;
                        gFarmAnimals[i].flags |= 0x8000;
                        gPlayer.bottleContents = 6;
                    }

                    break;

                case PREGNANT_COW:

                    func_800861A4(2, i, 0xFF, 0xFF, 0x11);
                    set = TRUE;
                    gFarmAnimals[i].flags |= 0x8000;
                    gPlayer.bottleContents = 6;

                    break;

            }

        }

        i++;
        
    }

    return set;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009AAC8);

void func_8009AAC8(void) {

    u8 i = 0;
    bool set = FALSE;

    while (i < MAX_FARM_ANIMALS && !set) {

        if ((gFarmAnimals[i].flags & 4) && gFarmAnimals[i].unk_1B == 16) {

            func_800861A4(2, i, 0xFF, 0xFF, 0);

            if (!(gFarmAnimals[i].flags & 0x100) && gFarmAnimals[i].type == ADULT_SHEEP) {
                
                func_80086458(i, 2);
                func_800861A4(2, i, SHEARED_SHEEP, 0xFF, 0x11);
                
                gPlayer.heldItem = func_800866E0(i, 0);
                gFarmAnimals[i].flags |= 0x100;
                
                func_8009BCC4(2, i, 3);
                set = TRUE;
                
            }

        }

        i++;
        
    } 

}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009AC54);

bool func_8009AC54(void) {

    u8 i = 0;
    bool result = FALSE;

    do {

        if ((gFarmAnimals[i].flags & 4) && gFarmAnimals[i].unk_1B == 16) {

            func_800861A4(2, i, 0xFF, 0xFF, 0);

            switch (gFarmAnimals[i].type) {

                case BABY_COW ... PREGNANT_COW:
                    
                    func_80086458(i, -10);
                    func_800861A4(2, i, 0xFF, 0xFF, 0x12);
                    
                    gFarmAnimals[i].flags |= 0x8000;
                    
                    func_8009BCC4(2, i, 0);

                    if (gFarmAnimals[i].type == ADULT_COW) {
                        
                        if (!(getRandomNumberInRange(0, 7))) {
                            func_800861A4(2, i, 0xFF, 2, 0xFFU);
                            func_80086458(i, -20);
                            gHappiness += adjustValue(gHappiness, -5, 0xFF);
                        }
                        
                    }
                    
                    result = TRUE;
                    
                    break;

                case BABY_SHEEP ... SHEARED_SHEEP:
                    
                    func_80086458(i, -10);
                    func_800861A4(2, i, 0xFF, 0xFF, 0x12);
                    
                    gFarmAnimals[i].flags |= 0x8000;
                    
                    func_8009BCC4(2, i, 0);
                    
                    result = TRUE;
                    
                    break;                    
                
            }
            
        }

        i++;
        
    } while (i < MAX_FARM_ANIMALS && result == FALSE);

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009AE7C);

bool func_8009AE7C(void) {

    u8 i = 0;
    bool set = FALSE;

    while (i < MAX_CHICKENS && !set) {

        if ((gChickens[i].flags & 4) && gChickens[i].unk_17 == 0x10) {

            func_800861A4(1, i, 0xFF, 0xFF, 0);

             switch (gChickens[i].type) { 

                 case ADULT_CHICKEN:
                    func_800861A4(1, i, 0xFF, 0xFF, 0x11);
                    gChickens[i].flags |= 0x40;
                    func_8009BCC4(4, i, 0);
                    set = TRUE;
                    break;
                 
                 case CHICK:
                    gChickens[i].flags |= 0x40;
                    func_8009BCC4(4, i, 0);
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

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009AFB4);
 
bool func_8009AFB4(void) {

    bool result = FALSE;
    
    if (dogInfo.flags & 4) {

        if (dogInfo.unk_17 == 0x20) {

            dogInfo.unk_17 = 0x22;

            adjustDogAffection(-8);

            dogInfo.flags |= 0x20;
            func_8009BCC4(0, 0, 0);
            result = TRUE;
            
        }
        
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009B068);

bool func_8009B068(void) {
    
    bool result = FALSE;
    
    if (horseInfo.flags & 4) {

        if (horseInfo.unk_17 == 0x10) {

            horseInfo.unk_17 = 0x12;

            adjustHorseAffection(-8);

            horseInfo.flags |= 0x20;
            func_8009BCC4(1, 0, 0);
            result = TRUE;
            
        }
        
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009B11C);

void func_8009B11C(void) {

    if (dogInfo.flags & 4) {
        dogInfo.flags |= 0x10;
        dogInfo.unk_17 = 0;
        playSfx(0x3B);
    }

    if (!(dogInfo.flags & 0x80)) {
        adjustDogAffection(1);
        dogInfo.flags |= 0x80;
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009B1BC);

void func_8009B1BC(void) {
    
    if (horseInfo.flags & 4) {
        horseInfo.flags |= 0x10;
        horseInfo.unk_17 = 0;
        playSfx(0x38);
    }
    
    if (!(horseInfo.flags & 0x200)) {
        adjustHorseAffection(1);
        horseInfo.flags |= 0x200;
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009B25C);

void func_8009B25C(void) {

    u8 i;

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {

        if (gFarmAnimals[i].flags & 4) {
            gFarmAnimals[i].flags |= (0x200 | 0x2000);
            gFarmAnimals[i].unk_1B = 0;
        }
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009B2BC);

u8 func_8009B2BC(u8 type) {

    u8 i;
    u8 count = 0;

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {
        
        if (gFarmAnimals[i].flags & 1 && gFarmAnimals[i].type == type) {
                count++;
        }
        
    }
    
    return count;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009B320);

u8 func_8009B320(void) {

    u8 i;
    u8 count = 0;

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {
        
        if ((gFarmAnimals[i].flags & 1) && (gFarmAnimals[i].flags & 0x80)) {
            count++;
        }
        
    }
    
    return count;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009B374);

u8 func_8009B374(void) {

    u8 i;
    u8 count = 0;

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {
        
        if (gFarmAnimals[i].flags & 1) {
            
            if (gFarmAnimals[i].type == PREGNANT_COW) {
                count++;
            }

            count++;
        
        }
        
    }
    
    return count;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009B3DC);

u8 func_8009B3DC(void) {

    u8 i;
    u8 count = 0;

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {
        
        if (gFarmAnimals[i].flags & 1) {
            
            // FIXME: should be range
            if ((u8)(gFarmAnimals[i].type - 1) < 2U || (gFarmAnimals[i].type == BABY_COW || gFarmAnimals[i].type == PREGNANT_COW)) {
                count++;
            }
    
        }
    }
    
    return count;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009B464);

u8 func_8009B464(void) {

    u8 i;
    u8 found = 0xFF;
    u8 temp = 0;

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {
        
        if ((gFarmAnimals[i].flags & 1) && gFarmAnimals[i].type == ADULT_COW) {

            if (gFarmAnimals[i].affection >= temp) {
                found = i;
                temp = gFarmAnimals[i].affection;
            }
    
        }
    }
    
    return found;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009B4EC);

u8 func_8009B4EC(void) {

    u8 i;
    u8 count = 0;

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {
        
        if (gFarmAnimals[i].flags & 1) {
            
            if (PREGNANT_COW < gFarmAnimals[i].type && gFarmAnimals[i].type < SHEARED_SHEEP || gFarmAnimals[i].type == SHEARED_SHEEP) {
                count++;
            }
    
        }
    }
    
    return count;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009B564);

u8 func_8009B564(void) {

    u8 i;
    u8 count = 0;

    for (i = 0; i < MAX_CHICKENS; i++) {

        if (gChickens[i].flags & 1) {
            
            // FIXME: should be range
            if ((gChickens[i].type - 1) < 2U) {
                count++;
            }

            if (gChickens[i].type == CHICKEN_EGG && (gChickens[i].flags & 0x20)) {
                count++;
            }
            
        }  

    }
    
    return count;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009B5E0);

u8 func_8009B5E0(void) {

    u8 i;
    u8 count = 0;

    for (i = 0; i < MAX_CHICKENS; i++) {

        if ((gChickens[i].flags & 1) && gChickens[i].type == ADULT_CHICKEN && gChickens[i].condition == CHICKEN_NORMAL) {
            count++;
        }

    }
    
    return count;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009B658);

u8 func_8009B658(void) {

    u8 i;
    u8 count = 0;

    for (i = 0; i < MAX_CHICKENS; i++) {

        if ((gChickens[i].flags & 1) && gChickens[i].type == CHICKEN_EGG) {
            count++;
        }

    }
    
    return count;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009B6B8);

void func_8009B6B8() {

    u8 i;
    bool found = FALSE;

    i = 0;
    
    while (i < MAX_CHICKENS && !found) {

        if (!(gChickens[i].flags & 1)) {

            i++;

        } else if (gChickens[i].type == CHICKEN_EGG) {
          
            if (!(gChickens[i].flags & 0x20)) {
                found++;
            }
            
        } else {
            i++;
        }
        
    }

    if (found) {
        
        if (gChickens[i].flags & 4) {
            if (entities[gChickens[i].entityIndex].flags & 8) {
                deactivateEntity(gChickens[i].entityIndex);
            }
        }

        initializeChicken(i);
    
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009B7BC);

u8 func_8009B7BC(void) {

    u8 i;
    u8 sum = 0;

    for (i = 0; i < MAX_CHICKENS; i++) {

        if ((gChickens[i].flags & 1) && (gChickens[i].type == CHICKEN_EGG) && (gChickens[i].flags & 0x20)) {
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
        if ((gFarmAnimals[i].flags & 1) 
            && (((u8)(gFarmAnimals[i].type - 1) < 2U 
                || (gFarmAnimals[i].type == BABY_COW || gFarmAnimals[i].type == PREGNANT_COW))
            && gFarmAnimals[i].location == FARM && (arg0 == 0 || !gFarmAnimals[i].normalMilk))) {

            // FIXME: fake match to do a regswap
            if (count) {
                count++;
            } else {
                count++;
            }
        
        }
        
    }

    if (count) {
        count = getFarmGrassTilesSum() != 0;
    }

    return count;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009B914);

void func_8009B914() {

    u8 temp;
    u16 temp2;
    u16 temp3;
    
    switch (D_801C4216) {

        case 2:
            
            if (!gFarmAnimals[D_801FC155].normalMilk) {
                
                temp2 = D_80114AD4[2];
                
            } else {

                temp = gFarmAnimals[D_801FC155].affection; 

                if (temp < 151) {
                    temp2 = D_80114ABC[2];
                }
                
                if (150 < temp && temp < 221) {
                    temp2 = D_80114AC4[2];
                }
    
                if (temp >= 221) {
                    temp2 = D_80114ACC[2];
                }
                    
            }

            D_801890E0 = temp2;
            
            break;

        case 3:

            temp = gFarmAnimals[D_801FC155].affection; 
            
            if (temp < 100) {
                temp3 = D_80114ADC.arr[2];
            }
            
            if (99 < temp && temp < 200) {
                temp3 = D_80114ADC.arr2[2];
            }

            if (temp >= 200) {
                temp3 = D_80114ADC.arr3[2];
            }

            D_801890E0 = temp3;
            
            break;
        
        case 4:
            D_801890E0 = 500;
            break;

        default:
            break;
    }

    convertNumberToGameVariableString(0x12, D_801890E0, 0);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009BA74);

void func_8009BA74(u8 index) {

    u16 temp;

    if (CALF < gFarmAnimals[index].type && gFarmAnimals[index].type < BABY_SHEEP) {

        if (!gFarmAnimals[index].normalMilk) {
            
            temp = D_80114AD4[0];
            
        } else {

            if (gFarmAnimals[index].affection < 151) {
                temp = D_80114ABC[0];
            }
            
            if (150 < gFarmAnimals[index].affection && gFarmAnimals[index].affection < 221) {
                temp = D_80114AC4[0];
            }

            if (gFarmAnimals[index].affection >= 221) {
                temp = D_80114ACC[0];
            }
            
        }

        switch (temp) {

            case 0x15:
                D_8016FBCC[0] = char_S;
                break;
            case 0x16:
                D_8016FBCC[0] = char_M;
                break;
            case 0x17:
                D_8016FBCC[0] = char_L;
                break;
            case 0x18:
                D_8016FBCC[0] = char_G;
                break;
            default:
                break;
            
        }
        
    } else {
        D_8016FBCC[0] = 0xF6;
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009BB70);

// same as func_80061690
void func_8009BB70(void) {
    D_801886D4[0] = 0xF6;
    D_801886D4[1] = 0xF6;
    D_801886D4[2] = 0xF6;
    D_801886D4[3] = 0xF6;
    D_801886D4[4] = 0xF6;
    D_801886D4[5] = 0xF6;
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009BBAC);

u8 func_8009BBAC(void) {

    u8 temp;
    u8 found = 0xFF;
    u8 i = 0;

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {

        if (gFarmAnimals[i].flags & 1) {

            // FIXME: should be range
            if ((u8)(gFarmAnimals[i].type - 1) < 2U || (gFarmAnimals[i].type == BABY_COW || gFarmAnimals[i].type == PREGNANT_COW)) {
                
                temp = gFarmAnimals[i].normalMilk;
    
                if (temp < found) {
                    found = temp;
                }
                
            }
            
        }
        
    }

    return found;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009BC44);

u8 func_8009BC44(void) {
    return dogInfo.unk_1C;
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009BC54);

u8 func_8009BC54(void) {
    return horseInfo.unk_1E;
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009BC64);

void func_8009BC64(void) {

    u8 i;

    for (i = 0; i < MAX_MISC_ANIMALS; i++) {
        gMiscAnimals[i].unk_16 = getRandomNumberInRange(0, 3);
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009BCC4);

// show expression bubble when interacting with animals
void func_8009BCC4(u8 animalType, u8 animalIndex, u8 arg2) {

    // Entity index 0x30 = globalSprite index 0x1E
    // tools, holdable items, expression bubbles, field items

    deactivateEntity(0x30);
    loadEntity(0x30, 0x5B, TRUE);
    func_8002FF38(0x30, FALSE);

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