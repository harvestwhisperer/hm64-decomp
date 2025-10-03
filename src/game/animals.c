#include "common.h"

#include "game/animals.h"

#include "system/entity.h"
#include "system/graphic.h"
#include "system/mapController.h"
#include "system/math.h"
#include "system/message.h"

#include "game/game.h"
#include "game/gameAudio.h"
#include "game/gameStatus.h"
#include "game/level.h"

#include "game/spriteInfo.h"

// bss
Chicken gChickens[MAX_CHICKENS];
ChickenEgg gChickenEggs[];
FarmAnimal gFarmAnimals[];
u8 D_801886D4[6];
Dog dogInfo;
Horse horseInfo;

// TODO: label
// watched cow stall #3
u8 mrsManaCow3Index;
// watched cow stall #1
u8 mrsManaCow1Index;
// watched cow stall #2
u8 mrsManaCow2Index;

extern u16 D_8016FDF0;
extern u8 D_8016FFE8;
// newest chicken index?
extern u8 D_80170464;

extern u8 D_80189054;
extern u8 D_8018908C;
extern u32 D_801890E0;
// newest animal index (generic)?
extern u8 D_801FC155;

// data
extern Vec3f chickenStartingCoordinates[];
extern Vec3f farmAnimalStartingCoordinates[];
extern Vec3f D_80114AB0;
extern u16 D_80114ABC[];
extern u16 D_80114AC0[2];
extern u16 D_80114ACC[];
extern u16 D_80114AC4[];
extern u16 D_80114AC8[2];
extern u16 D_80114ACC[];
extern u16 D_80114AD0[2];
extern u16 D_80114AD4[];
extern u16 D_80114AD8[2];
extern u16 D_80114ADC[];
extern u16 D_80114AE0[2];
extern u16 D_80114AE2[];
extern u16 D_80114AE4[];
extern u16 D_80114AE6[2];
extern u16 D_80114AE8[];
extern u16 D_80114AEA[];
extern u16 D_80114AEC[2];


// forward declarations
void func_800861A4(u8, u8, u8, u8, u8);               
void func_80086458(u8, s8); 
u8 initializeNewFarmAnimal(u8 arg0, u8 arg1);
void setFarmAnimalLocation(u8); 
void func_8008B1B8();                                  
void func_8008B2E8(u8);                               
void func_8008B55C(u8);                               
void func_8008B9AC();                                  
void func_8008BAF0(u8, u8); 
void updateDog();                                  
void func_8008CF94(u8);                               
void func_8008DAA0(u8);                               
void updateHorse();                                  
void updateHorseGrown();                                 
void updateHorseNotGrown();       
void func_8008A5F0();
void func_8008A650(u8);                               
void func_8008A9E8(u8);                               
void func_8008B150();       
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

    for (i = 0; i < MAX_CHICKEN_EGGS; i++) {
        
        if (gChickenEggs[i].flags & 4) {
            deactivateEntity(gChickenEggs[i].entityIndex);
        }
        
        gChickenEggs[i].flags = 0;
        gChickenEggs[i].unk_F = 0;
        gChickenEggs[i].unk_13 = 0;
        
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
                gChickens[index].unk_1E = 0;
            }
            
            if (condition != 0xFF) {
                gChickens[index].condition = condition;
                gChickens[index].starvedCounter = 0;
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
                gFarmAnimals[index].age = 0;
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

INCLUDE_ASM("asm/nonmatchings/game/animals", func_80086458);

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008662C);

u16 func_8008662C(u8 animalIndex, u8 arg1, u16 arg2) {

    u16 res = arg2;
    
    if (!gFarmAnimals[animalIndex].goldenMilk) {

        res = D_80114AD4[arg1];
        
    } else {

        if (gFarmAnimals[animalIndex].affection <= 150) {
            res = D_80114ABC[arg1];
        }

        // possible FIXME
        if ((u8)(u16)(gFarmAnimals[animalIndex].affection + 105) < 70) {
            res = D_80114AC4[arg1];
        }

        if (gFarmAnimals[animalIndex].affection >= 221) {
            res = D_80114ACC[arg1];
        }
        
    }

    return res;

}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_800866E0);

u16 func_800866E0(u8 animalIndex, u8 arg1) {

    u16 res;

    if (gFarmAnimals[animalIndex].affection < 100) {
        res = D_80114ADC[arg1];
    }

    if ((u8)(u16)(gFarmAnimals[animalIndex].affection - 100) < 100) {
        res = D_80114AE2[arg1];
    }

    if (gFarmAnimals[animalIndex].affection >= 200) {
        res = D_80114AE8[arg1];
    }
    
    return res;
    
}

INCLUDE_ASM("asm/nonmatchings/game/animals", func_80086764);

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

    for (i = 0; i < MAX_CHICKEN_EGGS; i++) {
        if (gChickenEggs[i].flags & 4) {
            if (entities[gChickenEggs[i].entityIndex].flags & 8) {
                deactivateEntity(gChickenEggs[i].entityIndex);
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
        gFarmAnimals[i].flags |= (8 | 0x10);
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

u8 func_80087F28(u8 arg0, u8 arg1) {

    u32 padding[4];
    
    u8 found;
    u8 i;
    u8 temp;

    found = 0xFF;
    temp = arg0 != 0 ? 0 : 6;
    
    for (i = 0; i < 6 && found == 0xFF; i++) {

        if (!(gChickens[i + temp].flags & 1)) {
            found = i + temp;
        }
        
    }

    if (found != 0xFF) {

        if (arg1 == 0xFF) {

            gChickens[found].location = 0x59;

            gChickens[found].coordinates.x = chickenStartingCoordinates[found].x;
            gChickens[found].coordinates.y = chickenStartingCoordinates[found].y;
            gChickens[found].coordinates.z = chickenStartingCoordinates[found].z;
            
        } else {

            gChickens[found].location = gChickens[arg1].location;

            gChickens[found].coordinates.x = gChickens[arg1].coordinates.x;
            gChickens[found].coordinates.y = gChickens[arg1].coordinates.y;
            gChickens[found].coordinates.z = gChickens[arg1].coordinates.z;
            
        }

        func_800861A4(1, found, arg0, 0, 0);

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
    gChickens[chickenIndex].unk_1E = 0;
    gChickens[chickenIndex].starvedCounter = 0;

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

u8 initializeNewFarmAnimal(u8 arg0, u8 arg1) {

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
        
        func_800861A4(2, index, arg0, 0, 0);
        setFarmAnimalLocation(index);

        gFarmAnimals[index].birthdaySeason = gSeason;
        gFarmAnimals[index].flags = 1;
        gFarmAnimals[index].birthdayDayOfMonth = gDayOfMonth;
        gFarmAnimals[index].goldenMilk = 0xFF;
        gFarmAnimals[index].affection = 0;

        if (arg0 == 0) {

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

    if (gFarmAnimals[animalIndex].type == UNUSED_ANIMAL_TYPE) {
        
        gFarmAnimals[animalIndex].coordinates.x = D_80114AB0.x;
        gFarmAnimals[animalIndex].coordinates.y = D_80114AB0.y;
        gFarmAnimals[animalIndex].coordinates.z = D_80114AB0.z;
        gFarmAnimals[animalIndex].direction = SOUTH;

    } else {

        gFarmAnimals[animalIndex].coordinates.x = farmAnimalStartingCoordinates[animalIndex].x;
        gFarmAnimals[animalIndex].coordinates.y = farmAnimalStartingCoordinates[animalIndex].y;
        gFarmAnimals[animalIndex].coordinates.z = farmAnimalStartingCoordinates[animalIndex].z;
        
        if (animalIndex >= 4) {
            gFarmAnimals[animalIndex].direction = WEST;
        } else {
            gFarmAnimals[animalIndex].direction = EAST;
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
    gFarmAnimals[animalIndex].age = 0;
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
    dogInfo.direction = NORTH;
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
    horseInfo.direction = NORTH;
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
        dogInfo.direction = NORTH;
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
        horseInfo.direction = NORTH;
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

INCLUDE_ASM("asm/nonmatchings/game/animals", func_80088D54);

#ifdef PERMUTER
u8 func_8008A4A8(u8 arg0, u8 arg1, f32 arg2, f32 arg3, f32 arg4) {

    u32 padding[4];
    
    u8 i = 0;
    u8 found = (arg0 != 0) ? 0xFF : 6;
    u8 temp;
    
    if (arg0 == 5) {
        found = 5;    
    }

    if (found == 0xFF) {
        
        for (i = 0; i < 5 && found == 0xFF; i++) {
    
            if (!(D_8016FB08[i].flags & 1)) {
                found = i;
            }
            
        }
    }

    temp = found;
        
    if (temp != 0xFF) {
        
        D_8016FB08[i].unk_15 = arg0;
        D_8016FB08[i].unk_10 = arg1;
        D_8016FB08[i].unk_F = 0;
        D_8016FB08[i].unk_12 = 0;
        D_8016FB08[i].flags = 1;
        D_8016FB08[i].unk_0.x = arg2;
        D_8016FB08[i].unk_0.y = arg3;
        D_8016FB08[i].unk_0.z = arg4;
        D_8016FB08[i].spriteIndex = found + 0xE;
        D_8016FB08[i].mapIndex = gBaseMapIndex;
        
    }

    return found;
    
}
#else
INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008A4A8);
#endif

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008A5F0);

void func_8008A5F0(void) {
    
    if (checkDailyEventBit(0x43) && (dogInfo.flags & 1)) {
        dogInfo.affection += adjustValue(dogInfo.affection, 2, MAX_AFFECTION);
    }
    
}

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008A650);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008A9E8);

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

            case 2:
                initializeAnimalEntity(chickenIndex + 2, (void*)0x80275E00, (void*)0x80275F00, (void*)0x80276700, (void*)0x80276800);
                loadEntity(gChickens[chickenIndex].entityIndex, 0x44, TRUE);
                break;         
            
            case 1:
                initializeAnimalEntity(chickenIndex + 2, (void*)0x80276B00, (void*)0x80276C00, (void*)0x80277000, (void*)0x80277100);
                loadEntity(gChickens[chickenIndex].entityIndex, 0x43, TRUE);
                break;
            
            case 0:
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

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008B55C);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008B9AC);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008BAF0);

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

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008C358);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008CF94);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008D1C0);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008D650);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008D70C);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008DA00);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008DAA0);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008DF9C);

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
                    entities[PLAYER].coordinates.x, 
                    entities[PLAYER].coordinates.z);

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
                    entities[PLAYER].coordinates.x, 
                    entities[PLAYER].coordinates.z);

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

            setAudio(0x36);
            
            break;
        
        case 18:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x100);

            gFarmAnimals[index].unk_1B = 0;

            gFarmAnimals[index].flags |= (2 | 0x2000);
            gFarmAnimals[index].flags &= ~(0x400);

            setAudio(0x37);

            break;
        
        case 19:
            
            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x60);

            gFarmAnimals[index].unk_1B = 0x13;

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
                    entities[PLAYER].coordinates.x, 
                    entities[PLAYER].coordinates.z);

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
                    entities[PLAYER].coordinates.x, 
                    entities[PLAYER].coordinates.z);

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

            setAudio(0x36);
            
            break;
        
        case 18:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0xA0);

            gFarmAnimals[index].unk_1B = 0;

            gFarmAnimals[index].flags |= (2 | 0x2000);
            gFarmAnimals[index].flags &= ~(0x400);

            setAudio(0x37);

            break;
        
        case 19:
            
            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x60);

            gFarmAnimals[index].unk_1B = 0x13;

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
                    entities[PLAYER].coordinates.x, 
                    entities[PLAYER].coordinates.z);

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
                    entities[PLAYER].coordinates.x, 
                    entities[PLAYER].coordinates.z);

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

            setAudio(0x36);
            
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

            gFarmAnimals[index].unk_1B = 0x13;

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
                    entities[PLAYER].coordinates.x, 
                    entities[PLAYER].coordinates.z);

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
                    entities[PLAYER].coordinates.x, 
                    entities[PLAYER].coordinates.z);

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

            setAudio(0x36);
            
            break;
        
        case 18:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x50);

            gFarmAnimals[index].unk_1B = 0;

            gFarmAnimals[index].flags |= (2 | 0x2000);
            gFarmAnimals[index].flags &= ~(0x400);

            setAudio(0x37);
            
            break;
        
        case 19:
            
            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x58);

            gFarmAnimals[index].unk_1B = 0x13;

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
                    entities[PLAYER].coordinates.x, 
                    entities[PLAYER].coordinates.z);

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
                    entities[PLAYER].coordinates.x, 
                    entities[PLAYER].coordinates.z);

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

            setAudio(0x36);
            
            break;
        
        case 18:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x50);

            gFarmAnimals[index].unk_1B = 0;

            gFarmAnimals[index].flags |= (2 | 0x2000);
            gFarmAnimals[index].flags &= ~(0x400);

            setAudio(0x37);
            
            break;
        
        case 19:
            
            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x58);

            gFarmAnimals[index].unk_1B = 0x13;

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
                    entities[PLAYER].coordinates.x, 
                    entities[PLAYER].coordinates.z);

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
                    entities[PLAYER].coordinates.x, 
                    entities[PLAYER].coordinates.z);

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

            setAudio(0x36);
            
            break;
        
        case 18:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x58);

            gFarmAnimals[index].unk_1B = 0;

            gFarmAnimals[index].flags |= (2 | 0x2000);
            gFarmAnimals[index].flags &= ~(0x400);

            setAudio(0x37);
            
            break;
        
        case 19:
            
            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x60);

            gFarmAnimals[index].unk_1B = 0x13;

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
                    entities[PLAYER].coordinates.x, 
                    entities[PLAYER].coordinates.z);

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
                    entities[PLAYER].coordinates.x, 
                    entities[PLAYER].coordinates.z);

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

            setAudio(0x3A);
            
            break;
        
        case 18:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x30);

            gFarmAnimals[index].unk_1B = 0;

            gFarmAnimals[index].flags |= (2 | 0x2000);
            gFarmAnimals[index].flags &= ~(0x400);

            setAudio(0x3A);
            
            break;
        
        case 19:
            
            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;

            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x30);

            gFarmAnimals[index].unk_1B = 0x13;

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

            gFarmAnimals[index].unk_1B = 0x13;

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
                
                gFarmAnimals[index].direction = calculateAnimalDirectionToPlayer(entities[gFarmAnimals[index].entityIndex].coordinates.x, entities[gFarmAnimals[index].entityIndex].coordinates.z, entities[PLAYER].coordinates.x, entities[PLAYER].coordinates.z);
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
                gFarmAnimals[index].direction = calculateAnimalDirectionToPlayer(entities[gFarmAnimals[index].entityIndex].coordinates.x, entities[gFarmAnimals[index].entityIndex].coordinates.z, entities[PLAYER].coordinates.x, entities[PLAYER].coordinates.z);

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
            setAudio(0x3A);
        
            break;

        case 18:
            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x68);
            gFarmAnimals[index].unk_1B = 0;
            gFarmAnimals[index].flags |= (0x2 | 0x2000);
            gFarmAnimals[index].flags &= ~0x400;
            setAudio(0x3A);
            break;

        case 19:
            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].entityIndex, 0x68);
            gFarmAnimals[index].unk_1B = 0x13;
            gFarmAnimals[index].flags |= (0x2 | 0x2000);
            gFarmAnimals[index].flags &= ~0x400;
            break;

    }
    
}

INCLUDE_ASM("asm/nonmatchings/game/animals", func_800931D8);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_80093434);

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

                horseInfo.direction = calculateAnimalDirectionToPlayer(entities[horseInfo.entityIndex].coordinates.x, entities[horseInfo.entityIndex].coordinates.z, entities[PLAYER].coordinates.x, entities[PLAYER].coordinates.z);
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
                
                horseInfo.direction = calculateAnimalDirectionToPlayer(entities[horseInfo.entityIndex].coordinates.x, entities[horseInfo.entityIndex].coordinates.z, entities[PLAYER].coordinates.x, entities[PLAYER].coordinates.z);
                
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
            
            setAudio(0x39);
            
            break;

        case 18:

            horseInfo.speed = 0;
            horseInfo.unk_1A = 0;
            horseInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(horseInfo.entityIndex, 0);
            
            horseInfo.unk_17 = 0;
            horseInfo.flags |= (0x2 | 0x80); 
            horseInfo.flags &= ~(0x40);
            
            setAudio(0x38);
            
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

                horseInfo.direction = calculateAnimalDirectionToPlayer(entities[horseInfo.entityIndex].coordinates.x, entities[horseInfo.entityIndex].coordinates.z, entities[PLAYER].coordinates.x, entities[PLAYER].coordinates.z);
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
                
                horseInfo.direction = calculateAnimalDirectionToPlayer(entities[horseInfo.entityIndex].coordinates.x, entities[horseInfo.entityIndex].coordinates.z, entities[PLAYER].coordinates.x, entities[PLAYER].coordinates.z);
                
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
            
            setAudio(0x39);
            
            break;

        case 18:

            horseInfo.speed = 0;
            horseInfo.unk_1A = 0;
            horseInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(horseInfo.entityIndex, 0);
            
            horseInfo.unk_17 = 0;
            horseInfo.flags |= (0x2 | 0x80); 
            horseInfo.flags &= ~(0x40);
            
            setAudio(0x38);
            
            break;

        default:
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009476C);

void func_8009476C(u8 index) {

    Vec3f vec;

    if ((gChickenEggs[index].flags & 1) && (gChickenEggs[index].flags & 4)) {

        if (checkEntityAnimationStateChanged(gChickenEggs[index].entityIndex) || gChickenEggs[index].unk_F == 0x20) {

            switch (gChickenEggs[index].unk_15) {

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
         
        setEntityDirection(gChickenEggs[index].entityIndex, convertSpriteToWorldDirection(gChickenEggs[index].direction, MAIN_MAP_INDEX));
        
        vec = getMovementVectorFromDirection(gChickenEggs[index].zDisplacement, gChickenEggs[index].direction, 0.0f);
        vec.y = gChickenEggs[index].yDisplacement;
        
        setEntityMovementVector(gChickenEggs[index].entityIndex, vec.x, vec.y, vec.z, gChickenEggs[index].zDisplacement);
        
    }
    
}

INCLUDE_ASM("asm/nonmatchings/game/animals", func_80094A5C);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_800958EC);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_80096264);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_800967E0);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_80096978);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_80096DFC);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_800973F8);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_80097804);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_80097CB0);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_80098188);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_80098740);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_80098B3C);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_80098F24);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_800991C0);

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80099424);

void func_80099424(u8 index) {

    switch (gChickenEggs[index].unk_F) {

        case 0:
        case 1:

            setEntityCollidable(gChickenEggs[index].entityIndex, FALSE);
            setEntityTracksCollisions(gChickenEggs[index].entityIndex, FALSE);
    
            entities[gChickenEggs[index].entityIndex].yOffset = 0x20;
    
            gChickenEggs[index].zDisplacement = 1;
            gChickenEggs[index].unk_13 = 0;
            gChickenEggs[index].unk_14 = 0;
    
            setEntityAnimation(gChickenEggs[index].entityIndex, 2);
    
            gChickenEggs[index].direction = getRandomNumberInRange(0, 7);
            gChickenEggs[index].unk_F = 0;
            gChickenEggs[index].flags |= 2;
            
            break;
        
        default:
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80099548);

void func_80099548(u8 index) {

    switch (gChickenEggs[index].unk_F) {

        case 0:
        case 1:

            gChickenEggs[index].zDisplacement = 0;
            gChickenEggs[index].unk_13 = 0;
            gChickenEggs[index].unk_14 = 0;
    
            setEntityAnimation(gChickenEggs[index].entityIndex, 0);

            gChickenEggs[index].unk_F = 0;
            gChickenEggs[index].flags |= 2;

            break;
        
        default:
            break;
    
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_800995F8);

void func_800995F8(u8 index) {

    switch (gChickenEggs[index].unk_F) {

        case 0:

            gChickenEggs[index].zDisplacement = 0;
            gChickenEggs[index].unk_13 = 0;
            gChickenEggs[index].unk_14 = 0;

            setEntityAnimation(gChickenEggs[index].entityIndex, 0);

            entities[gChickenEggs[index].entityIndex].yOffset = 0x40;

            gChickenEggs[index].flags |= 2;

            if (!(5 < gHour && gHour < 16)) {
                setAudio(0x51);
            } else {
                setAudio(0x4E);
            }
            
            break;
        
        case 1:

            setEntityCollidable(gChickenEggs[index].entityIndex, FALSE);
            setEntityTracksCollisions(gChickenEggs[index].entityIndex, FALSE);
            enableEntityMovement(gChickenEggs[index].entityIndex, FALSE);
            setEntityYMovement(gChickenEggs[index].entityIndex, FALSE);
            setEntityShadow(gChickenEggs[index].entityIndex, 0xFF);

            entities[gChickenEggs[index].entityIndex].yOffset = 0;

            setEntityAnimation(gChickenEggs[index].entityIndex, 2);

            gChickenEggs[index].zDisplacement = 4;
            gChickenEggs[index].yDisplacement = 2;

            gChickenEggs[index].flags |= 2;

            gChickenEggs[index].unk_13++;
            
            break;    
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80099804);

void func_80099804(u8 index) {

     switch (gChickenEggs[index].unk_F) {

        case 0:

            gChickenEggs[index].zDisplacement = 0;
            gChickenEggs[index].unk_13 = 0;
            gChickenEggs[index].unk_14 = 0;

            setEntityAnimation(gChickenEggs[index].entityIndex, 0);

            gChickenEggs[index].flags |= 2;

            break;
        
        case 1:

            setEntityCollidable(gChickenEggs[index].entityIndex, FALSE);
            setEntityTracksCollisions(gChickenEggs[index].entityIndex, FALSE);
            enableEntityMovement(gChickenEggs[index].entityIndex, FALSE);
            setEntityYMovement(gChickenEggs[index].entityIndex, FALSE);
            setEntityShadow(gChickenEggs[index].entityIndex, 0xFF);

            entities[gChickenEggs[index].entityIndex].yOffset = 0;

            setEntityAnimation(gChickenEggs[index].entityIndex, 4);

            gChickenEggs[index].zDisplacement = 4;
            gChickenEggs[index].yDisplacement = 2;

            gChickenEggs[index].flags |= 2;

            gChickenEggs[index].unk_13++;
            
            break;    
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_800999B0);

void func_800999B0(u8 index) {

    switch (gChickenEggs[index].unk_F) {

        case 0:

            gChickenEggs[index].zDisplacement = 0;
            gChickenEggs[index].unk_13 = 0;
            gChickenEggs[index].unk_14 = 0;

            setEntityAnimation(gChickenEggs[index].entityIndex, 0);

            gChickenEggs[index].flags |= 2;

            break;
        
        case 1:

            setEntityCollidable(gChickenEggs[index].entityIndex, FALSE);
            setEntityTracksCollisions(gChickenEggs[index].entityIndex, FALSE);
            enableEntityMovement(gChickenEggs[index].entityIndex, FALSE);
            setEntityYMovement(gChickenEggs[index].entityIndex, FALSE);
            setEntityShadow(gChickenEggs[index].entityIndex, 0xFF);

            entities[gChickenEggs[index].entityIndex].yOffset = 0;

            setEntityAnimation(gChickenEggs[index].entityIndex, 4);

            gChickenEggs[index].zDisplacement = 4;
            gChickenEggs[index].yDisplacement = 2;

            gChickenEggs[index].flags |= 2;

            gChickenEggs[index].unk_13++;
            
            break;    
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80099B5C);

void func_80099B5C(u8 index) {

    u16 temp;
    
    switch (gChickenEggs[index].unk_F) {

        case 0:
        case 1:

            setEntityCollidable(gChickenEggs[index].entityIndex, FALSE);
            setEntityTracksCollisions(gChickenEggs[index].entityIndex, FALSE);
                        
            entities[gChickenEggs[index].entityIndex].yOffset = 0x20;

            gChickenEggs[index].zDisplacement = 1;
            gChickenEggs[index].unk_13 = 0;
            gChickenEggs[index].unk_14 = 0;

            setEntityAnimation(gChickenEggs[index].entityIndex, 2);

            temp = getRandomNumberInRange(0, 20);

            if (temp < 7) {
                gChickenEggs[index].direction = temp;
            }
            
            gChickenEggs[index].unk_F = 0;
            gChickenEggs[index].flags |= 2;
            
            break;    
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80099C94);

void func_80099C94(u8 index) {
    
    switch (gChickenEggs[index].unk_F) {

        case 0:

            gChickenEggs[index].zDisplacement = 0;
            gChickenEggs[index].unk_13 = 0;
            gChickenEggs[index].unk_14 = 0;

            setEntityAnimation(gChickenEggs[index].entityIndex, 0);

            gChickenEggs[index].flags |= 2;

            break;
        
        case 1:

            setEntityAnimation(gChickenEggs[index].entityIndex, 2);
            gChickenEggs[index].flags |= 2;

            if (gChickenEggs[index].unk_13 < 0x21) {
                gChickenEggs[index].zDisplacement = 2;
                gChickenEggs[index].yDisplacement = 0;
                gChickenEggs[index].unk_13++;
            } else {
                deactivateEntity(gChickenEggs[index].entityIndex);
                gChickenEggs[index].flags = 0;
            }


            
            break;    
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80099DE8);

void func_80099DE8(void) {
    
    u8 i = 0;
    bool set = FALSE;

    func_800305CC(PLAYER, 0.0f, 8.0f, 0);
    
    do {
        
        if ((gFarmAnimals[i].flags & 4) && (entities[gFarmAnimals[i].entityIndex].entityCollidedWithIndex == 0)) {
            func_800861A4(2, i, 0xFF, 0xFF, 0x10);
            set = TRUE;
            gFarmAnimals[i].flags |= 0x8000; 
        }
        
        i++;
        
    } while (i < MAX_FARM_ANIMALS && set == FALSE);
        
}

INCLUDE_ASM("asm/nonmatchings/game/animals", func_80099EEC);

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80099FF0);

void func_80099FF0(void) {

    func_800305CC(PLAYER, 0.0f, 8.0f, 0);
    
    if ((dogInfo.flags & 4) && (entities[dogInfo.entityIndex].entityCollidedWithIndex == 0)) {
        dogInfo.unk_17 = 0x20;
        dogInfo.flags |= 0x20;
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009A074);

bool func_8009A074(void) {
    
    bool result;

    func_800305CC(PLAYER, 0.0f, 8.0f, 0);
    
    result = 0;

    if ((horseInfo.flags & 4) && (entities[horseInfo.entityIndex].entityCollidedWithIndex == 0)) {
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

#ifdef PERMUTER
void func_8009A17C(void) {

    u8 i = 0;
    bool set = FALSE;

    while (!set && i < MAX_FARM_ANIMALS) {

        if ((gFarmAnimals[i].flags & 4) && gFarmAnimals[i].unk_1B == 0x10) {

            func_800861A4(2, i, 0xFF, 0xFF, 0);

            if (!(gFarmAnimals[i].flags & 0x20)) {

                switch (gFarmAnimals[i].type) {

                    case 0 ... 3:
                        func_80086458(i, 2);
                        func_800861A4(2, i, 0xFF, 0xFF, 0x11);
                        gFarmAnimals[i].flags |= 0x20 | 0x8000;
                        func_8009BCC4(2, i, 3);
                        set = TRUE;
                        break;

                    case 4 ... 6:
                        func_80086458(i, 2);
                        func_800861A4(2, i, 0xFF, 0xFF, 0x11);
                        gFarmAnimals[i].flags |= 0x20 | 0x8000;
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
    
}
#else
INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009A17C);
#endif

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

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009A400);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009A53C);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009A810);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009A97C);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009AAC8);

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009AC54);

bool func_8009AC54(void) {

    u8 i = 0;
    bool result = FALSE;

    do {

        if ((gFarmAnimals[i].flags & 4) && gFarmAnimals[i].unk_1B == 0x10) {

            func_800861A4(2, i, 0xFF, 0xFF, 0);

            switch (gFarmAnimals[i].type) {

                case 0 ... 3:
                    
                    func_80086458(i, -10);
                    func_800861A4(2, i, 0xFF, 0xFF, 0x12);
                    
                    gFarmAnimals[i].flags |= 0x8000;
                    
                    func_8009BCC4(2, i, 0);

                    if (gFarmAnimals[i].type == 2) {
                        
                        if (!(getRandomNumberInRange(0, 7))) {
                            func_800861A4(2, i, 0xFF, 2, 0xFFU);
                            func_80086458(i, -20);
                            gHappiness += adjustValue(gHappiness, -5, 0xFF);
                        }
                        
                    }
                    
                    result = TRUE;
                    
                    break;

                case 4 ... 6:
                    
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

                 case 2:
                    func_800861A4(1, i, 0xFF, 0xFF, 0x11);
                    gChickens[i].flags |= 0x40;
                    func_8009BCC4(4, i, 0);
                    set = TRUE;
                    break;
                 
                 case 1:
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
        setAudio(0x3B);
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
        setAudio(0x38);
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

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009B2BC);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009B320);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009B374);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009B3DC);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009B464);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009B4EC);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009B564);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009B5E0);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009B658);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009B6B8);

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009B7BC);

u8 func_8009B7BC(void) {

    u8 i;
    u8 sum = 0;

    for (i = 0; i < MAX_CHICKENS; i++) {

        if ((gChickens[i].flags & 1) && (gChickens[i].type == 0) && (gChickens[i].flags & 0x20)) {
            sum++;
        }
        
    }
    
    return sum;
    
}

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009B828);

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009B914);

void func_8009B914(u16 arg0) {

    u8 temp;
    u16 temp2;
    
    switch (D_801C4216) {

        case 2:
            
            if (gFarmAnimals[D_801FC155].goldenMilk == 0) {
                
                temp2 = D_80114AD8[0];
                
            } else {

                temp = gFarmAnimals[D_801FC155].affection; 

                if (temp < 151) {
                    temp2 = D_80114AC0[0];
                }
                
                if (150 < temp && temp < 221) {
                    temp2 = D_80114AC8[0];
                }
    
                if (temp >= 221) {
                    temp2 = D_80114AD0[0];
                }
                    
            }

            D_801890E0 = temp2;
            
            break;

        case 3:

            temp = gFarmAnimals[D_801FC155].affection; 
            
            if (temp < 100) {
                arg0 = D_80114AE0[0];
            }
            
            if (99 < temp && temp < 200) {
                arg0 = D_80114AE6[0];
            }

            if (temp >= 200) {
                arg0 = D_80114AEC[0];
            }

            D_801890E0 = arg0;
            
            break;
        
        case 4:
            D_801890E0 = 0x1F4;
            break;

        default:
            break;
            
    }

    convertNumberToString(0x12, D_801890E0, 0);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009BA74);

void func_8009BA74(u8 index) {

    u16 temp;

    if (1 < gFarmAnimals[index].type && gFarmAnimals[index].type < 4) {

        if (gFarmAnimals[index].goldenMilk == 0) {
            
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
                D_8016FBCC[0] = 0xB3;
                break;
            case 0x16:
                D_8016FBCC[0] = 0xAD;
                break;
            case 0x17:
                D_8016FBCC[0] = 0xAC;
                break;
            case 0x18:
                D_8016FBCC[0] = 0xA7;
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

#ifdef PERMUTER
u8 func_8009BBAC(void) {

    u8 temp;
    u8 found = 0xFF;
    u8 i = 0;

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {

        if (gFarmAnimals[i].flags & 1) {

            if ((1 < gFarmAnimals[i].type && gFarmAnimals[i].type < 4) || (gFarmAnimals[i].type == 0 || gFarmAnimals[i].type == 3)) {
                
                temp = gFarmAnimals[i].goldenMilk;
    
                if (i < temp) {
                    found = temp;
                }
                
            }
            
        }
        
    }

    return found;
    
}
#else
INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009BBAC);
#endif

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

    for (i = 0; i < MAX_CHICKEN_EGGS; i++) {
        gChickenEggs[i].unk_16 = getRandomNumberInRange(0, 3);
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

    setEntityCollidable(0x30, 0);
    setEntityYMovement(0x30, FALSE);
    setEntityTracksCollisions(0x30, FALSE);
    enableEntityMovement(0x30, FALSE);

    setEntityColor(0x30, mainMap[MAIN_MAP_INDEX].mapGlobals.currentRGBA.r, mainMap[MAIN_MAP_INDEX].mapGlobals.currentRGBA.g, mainMap[MAIN_MAP_INDEX].mapGlobals.currentRGBA.b, mainMap[MAIN_MAP_INDEX].mapGlobals.currentRGBA.a);
    setEntityAttachmentOffset(0x30, 0, 0x30, 0);
    setEntityAnimation(0x30, arg2);
    
}