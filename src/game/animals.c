#include "common.h"

#include "game/animals.h"

#include "system/entity.h"
#include "system/graphic.h"
#include "system/mapController.h"
#include "system/math.h"
#include "system/message.h"

#include "game/fieldObjects.h"
#include "game/game.h"
#include "game/gameAudio.h"
#include "game/gameStatus.h"
#include "game/level.h"
#include "game/player.h"

#include "game/spriteInfo.h"

// bss
Chicken gChickens[MAX_CHICKENS];
Bird gBirds[];
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
        
        if (gBirds[i].flags & 4) {
            deactivateEntity(gBirds[i].entityIndex);
        }
        
        gBirds[i].flags = 0;
        gBirds[i].unk_F = 0;
        gBirds[i].unk_13 = 0;
        
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

inline u16 func_8008662C(u8 animalIndex, u8 arg1) {

    u16 res;
    
    if (!gFarmAnimals[animalIndex].goldenMilk) {

        res = D_80114AD4[arg1];
        
    } else {

        if (gFarmAnimals[animalIndex].affection <= 150) {
            res = D_80114ABC[arg1];
        }

        // FIXME: should be a range
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

inline u16 func_800866E0(u8 animalIndex, u8 arg1) {

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
        if (gBirds[i].flags & 4) {
            if (entities[gBirds[i].entityIndex].flags & 8) {
                deactivateEntity(gBirds[i].entityIndex);
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

INCLUDE_ASM("asm/nonmatchings/game/animals", func_80088D54);

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008A4A8);

u8 func_8008A4A8(u8 birdIndex, u8 direction, f32 x, f32 y, f32 z) {

    u32 padding[4];
    
    u8 i = 0;
    u8 found = (birdIndex != 0) ? 0xFF : 6;
    u8 temp;
    
    if (birdIndex == MAX_BIRDS) {
        found = MAX_BIRDS;    
    }

    while (i < MAX_BIRDS && found == 0xFF) {

        if (!(gBirds[i].flags & 1)) {
            found = i;
        }
        
        i++;
        
    }
        
    if (found != 0xFF) {
        gBirds[found].unk_15 = birdIndex;
        gBirds[found].direction = direction;
        gBirds[found].unk_F = 0;
        gBirds[found].yDisplacement = 0;
        gBirds[found].flags = 1;
        gBirds[found].coordinates.x = x;
        gBirds[found].coordinates.y = y;
        gBirds[found].coordinates.z = z;
        gBirds[found].entityIndex = ENTITY_BIRD_BASE_INDEX + found;
        gBirds[found].mapIndex = gBaseMapIndex;
        
    }

    return found;
    
}

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

            case ADULT_CHICKEN:
                initializeAnimalEntity(chickenIndex + 2, (void*)0x80275E00, (void*)0x80275F00, (void*)0x80276700, (void*)0x80276800);
                loadEntity(gChickens[chickenIndex].entityIndex, 0x44, TRUE);
                break;         
            
            case CHICK:
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

// INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008B9AC);

void func_8008B9AC(void) {
    if ((horseInfo.flags & 0x1) && (horseInfo.location == gBaseMapIndex) && !(horseInfo.flags & 0x8)) {
        horseInfo.entityIndex = 0x26;
        
        switch (horseInfo.grown) {
        case FALSE:
            loadEntity(0x26, 0x41, 1);
            break;
        case TRUE:
            loadEntity(0x26, 0x42, 1);
            break;
        }

        setEntityCollidable(horseInfo.entityIndex, 1);
        setEntityYMovement(horseInfo.entityIndex, 1);
        func_8002FF38(horseInfo.entityIndex, 0);
        setEntityDirection(horseInfo.entityIndex, convertSpriteToWorldDirection(horseInfo.direction, MAIN_MAP_INDEX));
        setEntityCoordinates(horseInfo.entityIndex, horseInfo.coordinates.x, horseInfo.coordinates.y, horseInfo.coordinates.z);
        horseInfo.unk_17 = 0;
        horseInfo.flags |= 4;
    }
    
    updateHorse();
}


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

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008D1C0);

void func_8008D1C0(u8 index) {
    u16 randomNumber;

    switch (gChickens[index].unk_17) {
    case 0:
        gChickens[index].unk_19 = 0;
        gChickens[index].unk_1A = 0;
        gChickens[index].unk_1B = 0;
        setEntityAnimationWithDirectionChange(gChickens[index].entityIndex, 0);
        if (getRandomNumberInRange(0, 100) < 40) {
            gChickens[index].unk_17 = 0;
        } else {
            randomNumber = getRandomNumberInRange(0, 10);
            if (randomNumber < 7) {
                gChickens[index].direction = randomNumber;
            }
            randomNumber = getRandomNumberInRange(0, 60);
            if (randomNumber < 20) {
                gChickens[index].unk_17 = 1;
            }
            if (19 < randomNumber && randomNumber < 40) {
                gChickens[index].unk_17 = 2;
            }
            
            if (39 < randomNumber && randomNumber < 60) {
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
        setAudio(0x40);
        
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
        setAudio(0x41);
        
        break;
    }
}


//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008D650);

void func_8008D650(u8 index) {
    u16 randomDirection;

    gChickens[index].unk_19 = 0;
    gChickens[index].unk_1A = 0;
    gChickens[index].unk_1B = 0;
    setEntityAnimationWithDirectionChange(gChickens[index].entityIndex, 0x28);
    randomDirection = getRandomNumberInRange(0, 1000);
    if (randomDirection < 7) {
        gChickens[index].direction = randomDirection;
    }
    
    gChickens[index].unk_17 = 0;
    gChickens[index].flags |= 2;
}


//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008D70C);

void func_8008D70C(u8 index) {
    u16 randomNumber;

    switch (gChickens[index].unk_17) {
    case 0:
        gChickens[index].unk_19 = 0;
        gChickens[index].unk_1A = 0;
        gChickens[index].unk_1B = 0;
        setEntityAnimationWithDirectionChange(gChickens[index].entityIndex, 0);
        if (getRandomNumberInRange(0, 100) < 40) {
            gChickens[index].unk_17 = 0;
        } else {
            randomNumber = getRandomNumberInRange(0, 10);
            if (randomNumber < 7) {
                gChickens[index].direction = randomNumber;
            }
            randomNumber = getRandomNumberInRange(0, 40);
            if (randomNumber < 20) {
                gChickens[index].unk_17 = 1;
            }
            if ((u16) (randomNumber - 20) < 20) {
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
        if (getRandomNumberInRange(0, 0x14) < 10) {
            gChickens[index].unk_17 = 1;
        } else {
            gChickens[index].unk_17 = 0;
        }
        gChickens[index].flags |= 2;
        setAudio(0x42);
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

void func_8008DA00(u8 index) {
    switch(gChickens[index].unk_17)
    {
        case 0:
        case 0x10:
            gChickens[index].unk_19 = 0;
            gChickens[index].unk_1A = 0;
            gChickens[index].unk_1B = 0;
            setEntityAnimation(gChickens[index].entityIndex, 0x13);
            gChickens[index].flags |= 0x2;
    }
}

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
            setAudio(0x3AU);
            
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

    if ((gBirds[index].flags & 1) && (gBirds[index].flags & 4)) {

        if (checkEntityAnimationStateChanged(gBirds[index].entityIndex) || gBirds[index].unk_F == 0x20) {

            switch (gBirds[index].unk_15) {

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
         
        setEntityDirection(gBirds[index].entityIndex, convertSpriteToWorldDirection(gBirds[index].direction, MAIN_MAP_INDEX));
        
        vec = getMovementVectorFromDirection(gBirds[index].zDisplacement, gBirds[index].direction, 0.0f);
        vec.y = gBirds[index].yDisplacement;
        
        setEntityMovementVector(gBirds[index].entityIndex, vec.x, vec.y, vec.z, gBirds[index].zDisplacement);
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80094A5C);

void func_80094A5C(u8 birdIndex) {
    
    u16 temp;
    
    switch (gBirds[birdIndex].unk_F) {

         case 0:

            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 10;
            gBirds[birdIndex].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0);

             if (getRandomNumberInRange(0, 60) < 40) {
                gBirds[birdIndex].unk_F = 0;
            } else {
                 
                temp = getRandomNumberInRange(0, 20);

                if (temp < 7) {
                    gBirds[birdIndex].direction = temp;
                }

                temp = getRandomNumberInRange(0, 11);

                if (temp == 0) {
                    gBirds[birdIndex].unk_F = 1;
                }
                
                if (temp == 1) {
                    gBirds[birdIndex].unk_F = 2;
                }
                
                if (temp == 2) {
                    gBirds[birdIndex].unk_F = 3;
                }

                if (temp == 3) {
                    gBirds[birdIndex].unk_F = 4;
                }
                 
                if (temp == 4) {
                    gBirds[birdIndex].unk_F = 5;
                }
                
                if (temp == 5) {
                    gBirds[birdIndex].unk_F = 6;
                }
                
                if (temp == 6) {
                    gBirds[birdIndex].unk_F = 7;
                }

                if (temp == 9) {
                    gBirds[birdIndex].unk_F = 10;
                }
                 
                if (temp == 10) {
                    gBirds[birdIndex].unk_F = 24;
                }
                 
                if (temp == 11) {
                    gBirds[birdIndex].unk_F = 25;
                }             
                 
            }

            gBirds[birdIndex].flags |= 2;
        
            break;

        case 1:
            gBirds[birdIndex].zDisplacement = 1;
            gBirds[birdIndex].unk_13 = 10;
            gBirds[birdIndex].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 8);

            temp = getRandomNumberInRange(0, 19);
            
            if (temp < 10) {
                gBirds[birdIndex].unk_F = 1;
            } else if (temp < 15) {
                gBirds[birdIndex].unk_F = 0;
            } else {
                gBirds[birdIndex].unk_F = 4;
            }
            
            gBirds[birdIndex].flags |= 2;

            break;
        
        case 2:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0x30);
            
            gBirds[birdIndex].unk_F = 16;
            
            gBirds[birdIndex].flags |= 2;
        
            break;

        case 3:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 10;
            gBirds[birdIndex].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0x20);

            temp = getRandomNumberInRange(0, 20);
            
            if (temp >= 10) {

                if (9 < temp && temp < 12) {
                    gBirds[birdIndex].unk_F = 0;
                } 
                if (11 < temp && temp < 14) {
                    gBirds[birdIndex].unk_F = 11;
                }
                if (13 < temp && temp < 16) {
                    gBirds[birdIndex].unk_F = 12;   
                }
                if (15 < temp && temp < 18) {
                    gBirds[birdIndex].unk_F = 13;   
                }
                if (17 < temp && temp < 20) {
                    gBirds[birdIndex].unk_F = 14;   
                }
                
            } else {
                gBirds[birdIndex].unk_F = 3;
            }
            
            gBirds[birdIndex].flags |= 2;
            break;

        case 4:
            gBirds[birdIndex].zDisplacement = 2;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0x10);

            temp = getRandomNumberInRange(0, 19);
            
            if (temp >= 10) {

                if (9 < temp && temp < 13) {
                    gBirds[birdIndex].unk_F = 0;
                } 
                if (12 < temp && temp < 16) {
                    gBirds[birdIndex].unk_F = 1;
                }
                if (15 < temp && temp < 19) {
                    gBirds[birdIndex].unk_F = 21;   
                }

            } else {
                gBirds[birdIndex].unk_F = 4;
            }
            
            gBirds[birdIndex].flags |= 2;

            break;

        case 5:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0x90);

            if (!getRandomNumberInRange(0, 2)) {
                gBirds[birdIndex].unk_F = 5;
            } else {
                gBirds[birdIndex].unk_F = 3;
            }
            
            gBirds[birdIndex].flags |= 2;
            break;

        case 6:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0xA0);

            if (getRandomNumberInRange(0, 2)) {
                gBirds[birdIndex].unk_F = 0;
            } else {
                gBirds[birdIndex].unk_F = 6;
            }
            
            gBirds[birdIndex].flags |= 2;
            break;
        
        case 7:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0xA8);

            if (getRandomNumberInRange(0, 2)) {
                gBirds[birdIndex].unk_F = 0;
            } else {
                gBirds[birdIndex].unk_F = 7;
            }
            
            gBirds[birdIndex].flags |= 2;
            break;

        case 8:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0xB0);

            if (getRandomNumberInRange(0, 2)) {
                gBirds[birdIndex].unk_F = 0;
            } else {
                gBirds[birdIndex].unk_F = 8;
            }
            
            gBirds[birdIndex].flags |= 2;
            break;

        case 9:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0xB8);

            gBirds[birdIndex].unk_F = 26;
            
            break;

        case 10:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 10;
            gBirds[birdIndex].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0xC8);

            if (!getRandomNumberInRange(0, 2)) {
                gBirds[birdIndex].unk_F = 10;
            } else {
                gBirds[birdIndex].unk_F = 0;
            }
            
            gBirds[birdIndex].flags |= 2;
            break;

        case 11:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 10;
            gBirds[birdIndex].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0x28);

            if (!getRandomNumberInRange(0, 2)) {
                gBirds[birdIndex].unk_F = 11;
            } else {
                gBirds[birdIndex].unk_F = 3;
            }
            
            gBirds[birdIndex].flags |= 2;
            break;

        case 12:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 10;
            gBirds[birdIndex].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0x50);

            if (!getRandomNumberInRange(0, 2)) {
                gBirds[birdIndex].unk_F = 12;
            } else {
                gBirds[birdIndex].unk_F = 3;
            }
            
            gBirds[birdIndex].flags |= 2;

            break;
     
        case 13:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0x58);

            if (!getRandomNumberInRange(0, 2)) {
                gBirds[birdIndex].unk_F = 13;
            } else {
                gBirds[birdIndex].unk_F = 3;
            }
            
            gBirds[birdIndex].flags |= 2;
            break;
        
        case 14:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0x60);

            if (!getRandomNumberInRange(0, 2)) {
                gBirds[birdIndex].unk_F = 14;
            } else {
                gBirds[birdIndex].unk_F = 3;
            }
            
            gBirds[birdIndex].flags |= 2;
            break;

        case 16:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 10;
            gBirds[birdIndex].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0x28);

            temp = getRandomNumberInRange(0, 14);
            
            if (temp < 10) {
                gBirds[birdIndex].unk_F = 16;
            } 
            if (temp == 10) {
                gBirds[birdIndex].unk_F = 27;
            } 
            if (temp == 11) {
                gBirds[birdIndex].unk_F = 17;
            }  
            if (temp == 12) {
                gBirds[birdIndex].unk_F = 18;
            }
            if (temp == 13) {
                gBirds[birdIndex].unk_F = 19;
            }
            if (temp == 14) {
                gBirds[birdIndex].unk_F = 20;
            }
            
            gBirds[birdIndex].flags |= 2;
            break;
        
        case 17:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0x68);

            if (!getRandomNumberInRange(0, 2)) {
                gBirds[birdIndex].unk_F = 17;
            } else {
                gBirds[birdIndex].unk_F = 16;
            }
            
            gBirds[birdIndex].flags |= 2;
            break;
                
        case 18:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0x70);

            if (!getRandomNumberInRange(0, 2)) {
                gBirds[birdIndex].unk_F = 18;
            } else {
                gBirds[birdIndex].unk_F = 16;
            }
            
            gBirds[birdIndex].flags |= 2;
            break;
                
        case 19:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0x78);

            if (!getRandomNumberInRange(0, 2)) {
                gBirds[birdIndex].unk_F = 19;
            } else {
                gBirds[birdIndex].unk_F = 16;
            }
            
            gBirds[birdIndex].flags |= 2;
            break;
                
        case 20:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 10;
            gBirds[birdIndex].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0x80);

            if (!getRandomNumberInRange(0, 2)) {
                gBirds[birdIndex].unk_F = 20;
            } else {
                gBirds[birdIndex].unk_F = 16;
            }
            
            gBirds[birdIndex].flags |= 2;
            break;
                 
        case 21:
            gBirds[birdIndex].zDisplacement = 1;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0x38);

            gBirds[birdIndex].unk_F = 22;
            
            gBirds[birdIndex].flags |= 2;
            break;       
                 
        case 22:
            gBirds[birdIndex].zDisplacement = 2;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0x40);

            gBirds[birdIndex].unk_F = 23;
            
            gBirds[birdIndex].flags |= 2;
            break;       

        case 23:
            gBirds[birdIndex].zDisplacement = 1;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0x48);

            gBirds[birdIndex].unk_F = 0;
            
            gBirds[birdIndex].flags |= 2;
            break;      

        case 24:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0x88);

            if (!getRandomNumberInRange(0, 2)) {
                gBirds[birdIndex].unk_F = 24;
            } else {
                gBirds[birdIndex].unk_F = 0;
            }
            
            gBirds[birdIndex].flags |= 2;
            break;      

        case 25:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0x98);

            if (!getRandomNumberInRange(0, 2)) {
                gBirds[birdIndex].unk_F = 25;
            } else {
                gBirds[birdIndex].unk_F = 0;
            }
            
            gBirds[birdIndex].flags |= 2;
            break;      

        case 26:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0xC0);

            gBirds[birdIndex].unk_F = 0;
            
            gBirds[birdIndex].flags |= 2;
            break;      

        case 27:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0x30);

            gBirds[birdIndex].unk_F = 0;
            
            gBirds[birdIndex].flags |= 2;
            break;      
        
     }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_800958EC);

void func_800958EC(u8 birdIndex) {

    u16 temp;
    
    switch (gBirds[birdIndex].unk_F) {

         case 0:

            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0);

             if (getRandomNumberInRange(0, 60) < 40) {
                gBirds[birdIndex].unk_F = 0;
            } else {
                 
                temp = getRandomNumberInRange(0, 20);

                if (temp < 7) {
                    gBirds[birdIndex].direction = temp;
                }

                temp = getRandomNumberInRange(0, 8);

                if (temp == 0) {
                    gBirds[birdIndex].unk_F = 1;
                }
                
                if (temp == 1) {
                    gBirds[birdIndex].unk_F = 2;
                }
                
                if (temp == 2) {
                    gBirds[birdIndex].unk_F = 3;
                }

                if (temp == 3) {
                    gBirds[birdIndex].unk_F = 4;
                }
                 
                if (temp == 4) {
                    gBirds[birdIndex].unk_F = 5;
                }
                
                if (temp == 5) {
                    gBirds[birdIndex].unk_F = 6;
                }
                
                if (temp == 6) {
                    gBirds[birdIndex].unk_F = 7;
                }

                if (temp == 7) {
                    gBirds[birdIndex].unk_F = 8;
                }
                 
                if (temp == 8) {
                    gBirds[birdIndex].unk_F = 9;
                }
                 
            }

            gBirds[birdIndex].flags |= 2;
        
            break;

        case 1:
            gBirds[birdIndex].zDisplacement = 1;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 8);

            temp = getRandomNumberInRange(0, 19);
            
            if (temp < 10) {
                gBirds[birdIndex].unk_F = 1;
            } else if (temp < 15) {
                gBirds[birdIndex].unk_F = 0;
            } else {
                gBirds[birdIndex].unk_F = 4;
            }
            
            gBirds[birdIndex].flags |= 2;

            break;
        
        case 2:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0x30);
            
            gBirds[birdIndex].unk_F = 12;
            
            gBirds[birdIndex].flags |= 2;
        
            break;

        case 3:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0x20);

            temp = getRandomNumberInRange(0, 20);
            
            if (temp >= 10) {

                if (9 < temp && temp < 13) {
                    gBirds[birdIndex].unk_F = 0;
                } 
                if (13 < temp && temp < 17) {
                    gBirds[birdIndex].unk_F = 10;
                }
                if (16 < temp && temp < 20) {
                    gBirds[birdIndex].unk_F = 11;   
                }
                
            } else {
                gBirds[birdIndex].unk_F = 3;
            }
            
            gBirds[birdIndex].flags |= 2;
            break;

        case 4:
            gBirds[birdIndex].zDisplacement = 2;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0x10);

            temp = getRandomNumberInRange(0, 19);
            
            if (temp < 10) {
                gBirds[birdIndex].unk_F = 4;
            } else if (temp < 15) {
                gBirds[birdIndex].unk_F = 0;
            } else {
                gBirds[birdIndex].unk_F = 1;
            }
            
            gBirds[birdIndex].flags |= 2;

            break;

        case 5:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0x50);

            if (!getRandomNumberInRange(0, 2)) {
                gBirds[birdIndex].unk_F = 5;
            } else {
                gBirds[birdIndex].unk_F = 3;
            }
            
            gBirds[birdIndex].flags |= 2;
            break;

        case 6:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0x58);

            if (!getRandomNumberInRange(0, 2)) {
                gBirds[birdIndex].unk_F = 6;
            } else {
                gBirds[birdIndex].unk_F = 0;
            }
            
            gBirds[birdIndex].flags |= 2;
            break;
        
        case 7:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0x60);

            if (!getRandomNumberInRange(0, 2)) {
                gBirds[birdIndex].unk_F = 7;
            } else {
                gBirds[birdIndex].unk_F = 0;
            }
            
            gBirds[birdIndex].flags |= 2;
            break;

        case 8:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0x70);

            if (!getRandomNumberInRange(0, 2)) {
                gBirds[birdIndex].unk_F = 8;
            } else {
                gBirds[birdIndex].unk_F = 0;
            }
            
            gBirds[birdIndex].flags |= 2;
            break;

        case 9:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0x68);

            if (!getRandomNumberInRange(0, 2)) {
                gBirds[birdIndex].unk_F = 9;
            } else {
                gBirds[birdIndex].unk_F = 0;
            }
            
            gBirds[birdIndex].flags |= 2;
            break;

        case 10:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0x18);

            if (!getRandomNumberInRange(0, 2)) {
                gBirds[birdIndex].unk_F = 10;
            } else {
                gBirds[birdIndex].unk_F = 3;
            }
            
            gBirds[birdIndex].flags |= 2;
            break;

        case 11:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0x38);

            if (!getRandomNumberInRange(0, 2)) {
                gBirds[birdIndex].unk_F = 11;
            } else {
                gBirds[birdIndex].unk_F = 3;
            }
            
            gBirds[birdIndex].flags |= 2;
            break;

        case 12:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0x28);

            temp = getRandomNumberInRange(0, 12);
            
            if (temp < 10) {
                gBirds[birdIndex].unk_F = 12;
            } 
            if (temp == 10) {
                gBirds[birdIndex].unk_F = 15;
            } 
            if (temp == 11) {
                gBirds[birdIndex].unk_F = 13;
            }  
            if (temp == 12) {
                gBirds[birdIndex].unk_F = 14;
            }
            
            gBirds[birdIndex].flags |= 2;

            break;
     
        case 13:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0x40);

            if (!getRandomNumberInRange(0, 2)) {
                gBirds[birdIndex].unk_F = 13;
            } else {
                gBirds[birdIndex].unk_F = 14;
            }
            
            gBirds[birdIndex].flags |= 2;
            break;
        
        case 14:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0x48);

            if (!getRandomNumberInRange(0, 2)) {
                gBirds[birdIndex].unk_F = 14;
            } else {
                gBirds[birdIndex].unk_F = 14;
            }
            
            gBirds[birdIndex].flags |= 2;
            break;

        case 15:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0x30);

            gBirds[birdIndex].unk_F = 0;
            
            gBirds[birdIndex].flags |= 2;
            break;
        
     }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80096264);

void func_80096264(u8 birdIndex) {
    
    u32 temp;
    u16 temp2;
    u16 temp3;
    u8 tempDirection;
    
    switch (gBirds[birdIndex].unk_F) {

        case 0:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0);

            if (getRandomNumberInRange(0, 60) < 40) {
                gBirds[birdIndex].unk_F = 0;
            } else {

                temp = getRandomNumberInRange(0, 10);

                if (temp < 7) {
                    temp &= 0xFFFE;
                    temp2 = temp;
                    gBirds[birdIndex].direction = temp2;
                }

                temp = getRandomNumberInRange(0, 4);

                if (temp == 0) {
                    gBirds[birdIndex].unk_F = 1;
                }
                
                if (temp == 1) {
                    gBirds[birdIndex].unk_F = 2;
                }
                
                if (temp == 2) {
                    gBirds[birdIndex].unk_F = 3;
                }

                if (temp == 3) {
                    gBirds[birdIndex].unk_F = 4;
                }

            }

            gBirds[birdIndex].flags |= 2;
        
            break;

        case 1:
            gBirds[birdIndex].zDisplacement = 1;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 8);

            temp2 = getRandomNumberInRange(0, 19);
            
            if (temp2 < 10) {
                gBirds[birdIndex].unk_F = 1;
            } else if (temp2 < 15) {
                gBirds[birdIndex].unk_F = 0;
            } else {
                gBirds[birdIndex].unk_F = 5;
            }
            
            gBirds[birdIndex].flags |= 2;

            setAudio(0x43);
            break;

        case 2:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0x20);

            temp3 = getRandomNumberInRange(0, 20);
            
            if (temp3 >= 10) {

                if (9 < temp3 && temp3 < 13) {
                    gBirds[birdIndex].unk_F = 0;
                } 
                if (13 < temp3 && temp3 < 17) {
                    gBirds[birdIndex].unk_F = 6;
                }
                if (16 < temp3 && temp3 < 20) {
                    gBirds[birdIndex].unk_F = 7;   
                }
                
            } else {
                gBirds[birdIndex].unk_F = 2;
            }
            
            gBirds[birdIndex].flags |= 2;
            break;

        case 3:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0x18);

            temp = getRandomNumberInRange(0, 19);

            if (temp >= 10) {

                if (temp >= 15) {
                    gBirds[birdIndex].unk_F = 2;
                } else {
                    gBirds[birdIndex].unk_F = 0;
                }
                
            } else {
                gBirds[birdIndex].unk_F = 3;
            }
            
            gBirds[birdIndex].flags |= 2;
            break;
        
        case 4:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;

            setEntityAnimation(gBirds[birdIndex].entityIndex, 0x38);

            if (!getRandomNumberInRange(0, 2)) {
                gBirds[birdIndex].unk_F = 4;
            } else {
                gBirds[birdIndex].unk_F = 0;
            }
            
            gBirds[birdIndex].flags |= 2;
            break;
        
        case 5:
            gBirds[birdIndex].zDisplacement = 1;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0x10);

            if (!getRandomNumberInRange(0, 2)) {
                gBirds[birdIndex].unk_F = 5;
            } else {
                gBirds[birdIndex].unk_F = 1;
            }
            
            gBirds[birdIndex].flags |= 2;
            break;
        
       case 6:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0x30);

            if (!getRandomNumberInRange(0, 2)) {
                gBirds[birdIndex].unk_F = 6;
            } else {
                gBirds[birdIndex].unk_F = 2;
            }
            
            gBirds[birdIndex].flags |= 2;
            break;     

        case 7:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;

            setEntityAnimation(gBirds[birdIndex].entityIndex, 0x39);

            if (!getRandomNumberInRange(0, 2)) {
                gBirds[birdIndex].unk_F = 7;
            } else {
                gBirds[birdIndex].unk_F = 2;
            }

            gBirds[birdIndex].flags |= 2;
            break;

    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_800967E0);

void func_800967E0(u8 birdIndex) {

    u16 temp;
    
    switch (gBirds[birdIndex].unk_F) {

         case 0:

            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0);

             if (getRandomNumberInRange(0, 60) < 56) {
                gBirds[birdIndex].unk_F = 0;
            } else {
                 
                temp = getRandomNumberInRange(0, 8);

                if (temp < 7) {
                    gBirds[birdIndex].direction = temp;
                }

                 gBirds[birdIndex].unk_F = 1;
                 
            }

            gBirds[birdIndex].flags |= 2;
        
            break;

        case 1:
            gBirds[birdIndex].zDisplacement = 1;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 8);
            
            if (getRandomNumberInRange(0, 19) < 4) {
                gBirds[birdIndex].unk_F = 1;
            } else {
                gBirds[birdIndex].unk_F = 0;
            }
            
            gBirds[birdIndex].flags |= 2;
        
            break;

     }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80096978);

void func_80096978(u8 birdIndex) {

    u16 temp;
    u8 tempDirection;
    
    switch (gBirds[birdIndex].unk_F) {

        case 0:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0);

            if (getRandomNumberInRange(0, 60) < 40) {
                gBirds[birdIndex].unk_F = 0;
            } else {

                temp = getRandomNumberInRange(0, 20);

                if (temp < 7) {
                    gBirds[birdIndex].direction = temp;
                }

                temp = getRandomNumberInRange(0, 2);

                if (temp == 0) {
                    gBirds[birdIndex].unk_F = 1;
                }
                
                if (temp == 1) {
                    gBirds[birdIndex].unk_F = 2;
                }
                
                if (temp == 2) {
                    gBirds[birdIndex].unk_F = 3;
                }

            }

            gBirds[birdIndex].flags |= 2;
        
            break;

        case 1:
            gBirds[birdIndex].zDisplacement = 1;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 8);

            if (getRandomNumberInRange(0, 20) < 10) {
                gBirds[birdIndex].unk_F = 1;
            } else {
                gBirds[birdIndex].unk_F = 0;
            }
            
            gBirds[birdIndex].flags |= 2;
        
            break;

        case 2:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0x10);

            if (!getRandomNumberInRange(0, 2)) {
                gBirds[birdIndex].unk_F = 2;
            } else {
                gBirds[birdIndex].unk_F = 0;
            }
            
            gBirds[birdIndex].flags |= 2;
            break;

        case 3:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0x18);

            gBirds[birdIndex].unk_F = 4;
            
            gBirds[birdIndex].flags |= 2;
            break;
        
        case 4:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0x20);

            temp = getRandomNumberInRange(0, 11);
            
            if (temp < 10) {
                gBirds[birdIndex].unk_F = 4;
            }
            
            if (temp == 10) {
                gBirds[birdIndex].unk_F = 4;
            }
            
            if (temp == 11) {
                gBirds[birdIndex].unk_F = 6;
            }

            
            gBirds[birdIndex].flags |= 2;
            break;
        
        case 5:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0x28);

            if (getRandomNumberInRange(0, 4) < 4) {
                gBirds[birdIndex].unk_F = 5;
            } else {
                gBirds[birdIndex].unk_F = 4;
            }
            
            gBirds[birdIndex].flags |= 2;
            break;
        
       case 6:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0x18);

            gBirds[birdIndex].unk_F = 0;
            
            gBirds[birdIndex].flags |= 2;
            break;     

    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80096DFC);

void func_80096DFC(u8 birdIndex) {

    u16 temp;
    u8 tempDirection;
    
    switch (gBirds[birdIndex].unk_F) {

        case 0:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0);

            if (getRandomNumberInRange(0, 60) < 40) {
                gBirds[birdIndex].unk_F = 0;
            } else {

                temp = getRandomNumberInRange(0, 20);

                if (temp < 7) {
                    gBirds[birdIndex].direction = temp;
                }

                temp = getRandomNumberInRange(0, 5);

                if (temp == 0) {
                    gBirds[birdIndex].unk_F = 1;
                }
                
                if (temp == 1) {
                    gBirds[birdIndex].unk_F = 2;
                }
                
                if (temp == 2) {
                    gBirds[birdIndex].unk_F = 3;
                }
                
                if (temp == 3) {
                    gBirds[birdIndex].unk_F = 4;
                }

                if (temp == 4) {
                    gBirds[birdIndex].unk_F = 5;
                }
                
                if (temp == 5) {
                    gBirds[birdIndex].unk_F = 6;
                }
            }

            gBirds[birdIndex].flags |= 2;
        
            break;

        case 1:
            gBirds[birdIndex].zDisplacement = 1;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 8);

            if (getRandomNumberInRange(0, 20) < 10) {
                gBirds[birdIndex].unk_F = 1;
            } else {
                gBirds[birdIndex].unk_F = 0;
            }
            
            gBirds[birdIndex].flags |= 2;
        
            break;

        case 2:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0x20);

            temp = getRandomNumberInRange(0, 2);
            
            if (temp == 0) {
                gBirds[birdIndex].unk_F = 2;
            }
            
            if (temp == 1) {
                gBirds[birdIndex].unk_F = 0;
            }
            
            if (temp == 2) {
                gBirds[birdIndex].unk_F = 9;
            }
            
            gBirds[birdIndex].flags |= 2;
            break;

        case 3:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0x10);

            gBirds[birdIndex].unk_F = 7;
            
            gBirds[birdIndex].flags |= 2;
            break;
        
        case 4:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0x38);

            gBirds[birdIndex].unk_F = 0;
            
            gBirds[birdIndex].flags |= 2;
            break;
        
        case 5:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0x40);

            gBirds[birdIndex].unk_F = 0;
            
            gBirds[birdIndex].flags |= 2;
            break;
        
       case 6:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0x48);

            gBirds[birdIndex].unk_F = 0;
            
            gBirds[birdIndex].flags |= 2;
            break;     

        case 7:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0x18);

            temp = getRandomNumberInRange(0, 11);
            
            if (temp < 10) {
                gBirds[birdIndex].unk_F = 7;
            }
            
            if (temp == 10) {
                gBirds[birdIndex].unk_F = 8;
            }
            
            if (temp == 11) {
                gBirds[birdIndex].unk_F = 10;
            }

            gBirds[birdIndex].flags |= 2;
            
            break;

        case 8:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0x28);

            if (getRandomNumberInRange(0, 4) < 4) {
                 gBirds[birdIndex].unk_F = 8;
            } else {
                 gBirds[birdIndex].unk_F = 7;
            }
            
            gBirds[birdIndex].flags |= 2;
            break;
       
        case 9:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0x30);

            gBirds[birdIndex].unk_F = 2;
            
            gBirds[birdIndex].flags |= 2;
            break;

        case 10:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0x10);

            gBirds[birdIndex].unk_F = 0;
            
            gBirds[birdIndex].flags |= 2;
            break;


    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_800973F8);

void func_800973F8(u8 birdIndex) {
    
    u16 temp;
    u8 tempDirection;
    
    switch (gBirds[birdIndex].unk_F) {

        case 0:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0);

            if (getRandomNumberInRange(0, 60) < 40) {
                
                gBirds[birdIndex].unk_F = 0;
                
            } else {
                temp = getRandomNumberInRange(0, 10);

                if (temp < 7) {
                    gBirds[birdIndex].direction = temp;
                }

                temp = getRandomNumberInRange(0, 2);

                if (temp == 0) {
                    gBirds[birdIndex].unk_F = 1;
                }
                
                if (temp == 1) {
                    gBirds[birdIndex].unk_F = 2;
                }
                
                if (temp == 2) {
                    gBirds[birdIndex].unk_F = 3;
                }
                
            }

            gBirds[birdIndex].flags |= 2;
        
            break;

        case 1:
            gBirds[birdIndex].zDisplacement = 1;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 8);

            if (getRandomNumberInRange(0, 19) < 10) {
                gBirds[birdIndex].unk_F = 1;
            } else {
                gBirds[birdIndex].unk_F = 0;
            }
            
            gBirds[birdIndex].flags |= 2;
        
            break;

        case 2:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0x10);

            if (getRandomNumberInRange(0, 20) < 10) {
                gBirds[birdIndex].unk_F = 2;
            } else {
                gBirds[birdIndex].unk_F = 0;
            }
            
            gBirds[birdIndex].flags |= 2;
            break;

        case 3:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0x18);
            
            if (getRandomNumberInRange(0, 19) < 10) {
                gBirds[birdIndex].unk_F = 3;
            } else {
                gBirds[birdIndex].unk_F = 0;
            }

            gBirds[birdIndex].flags |= 2;
            break;

        case 32:
            gBirds[birdIndex].zDisplacement = 4;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 8);

            tempDirection = calculateAnimalDirectionToPlayer(entities[gBirds[birdIndex].entityIndex].coordinates.x, entities[gBirds[birdIndex].entityIndex].coordinates.z, entities[ENTITY_PLAYER].coordinates.x, entities[ENTITY_PLAYER].coordinates.z);

            gBirds[birdIndex].direction = (tempDirection + 4) % 8;
            
            if (!getRandomNumberInRange(0, 30)) {
                gBirds[birdIndex].flags &= ~(0x10);
            }
            
            gBirds[birdIndex].flags |= 2;
            
            break;

    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80097804);

void func_80097804(u8 birdIndex) {

    u16 temp;
    u8 tempDirection;
    
    switch (gBirds[birdIndex].unk_F) {

        case 0:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0);

            if (getRandomNumberInRange(0, 60) < 40) {
                gBirds[birdIndex].unk_F = 0;
            } else {

                temp = getRandomNumberInRange(0, 10);

                if (temp < 7) {
                    gBirds[birdIndex].direction = temp;
                }

                temp = getRandomNumberInRange(0, 2);

                if (temp == 0) {
                    gBirds[birdIndex].unk_F = 1;
                }
                
                if (temp == 1) {
                    gBirds[birdIndex].unk_F = 2;
                }
                
                if (temp == 2) {
                    gBirds[birdIndex].unk_F = 3;
                }
                
            }

            gBirds[birdIndex].flags |= 2;
        
            break;

        case 1:
            gBirds[birdIndex].zDisplacement = 1;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 8);

            if (getRandomNumberInRange(0, 19) < 10) {
                gBirds[birdIndex].unk_F = 1;
            } else {
                gBirds[birdIndex].unk_F = 0;
            }
            
            gBirds[birdIndex].flags |= 2;
        
            break;

        case 2:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0x10);

            if (getRandomNumberInRange(0, 20) < 10) {
                gBirds[birdIndex].unk_F = 2;
            } else {
                gBirds[birdIndex].unk_F = 0;
            }
            
            gBirds[birdIndex].flags |= 2;
            break;

        case 3:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;

            if (getRandomNumberInRange(0, 1)) {
                setEntityAnimation(gBirds[birdIndex].entityIndex, 0x18);
            } else {
                setEntityAnimation(gBirds[birdIndex].entityIndex, 0x19);
            }

            gBirds[birdIndex].unk_F = 0;
            
            gBirds[birdIndex].flags |= 2;
            break;

        case 32:
            gBirds[birdIndex].zDisplacement = 4;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;

            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 8);

            tempDirection = calculateAnimalDirectionToPlayer(entities[gBirds[birdIndex].entityIndex].coordinates.x, entities[gBirds[birdIndex].entityIndex].coordinates.z, entities[ENTITY_PLAYER].coordinates.x, entities[ENTITY_PLAYER].coordinates.z);

            gBirds[birdIndex].direction = (tempDirection + 4) % 8;
            
            if (!getRandomNumberInRange(0, 30)) {
                gBirds[birdIndex].flags &= ~(0x10);
            }
            
            gBirds[birdIndex].flags |= 2;
            
            break;

    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80097CB0);

void func_80097CB0(u8 birdIndex) {

    u16 temp;
    u8 tempDirection;
    
    switch (gBirds[birdIndex].unk_F) {

        case 0:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0);

            if (getRandomNumberInRange(0, 60) < 40) {
                
                gBirds[birdIndex].unk_F = 0;
                
            } else {
                
                temp = getRandomNumberInRange(0, 10);

                if (temp < 7) {
                    gBirds[birdIndex].direction = temp;
                }

                temp = getRandomNumberInRange(0, 1);

                if (temp == 0) {
                    gBirds[birdIndex].unk_F = 1;
                }
                
                if (temp == 1) {
                    gBirds[birdIndex].unk_F = 2;
                }
                
            } 
            
            gBirds[birdIndex].flags |= 2;
        
            break;


        case 1:
            gBirds[birdIndex].zDisplacement = 1;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 8);

            if (getRandomNumberInRange(0, 19) < 10) {
                gBirds[birdIndex].unk_F = 1;
            } else {
                gBirds[birdIndex].unk_F = 0;
            }
            
            gBirds[birdIndex].flags |= 2;
            
            break;
        
        case 2:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0x10);

            if (getRandomNumberInRange(0, 20) < 10) {
                gBirds[birdIndex].unk_F = 2;
            } else {
                gBirds[birdIndex].unk_F = 0;
            }
            
            gBirds[birdIndex].flags |= 2;
            break;


    
        case 16:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0);

            gBirds[birdIndex].unk_F = 0x10;
            gBirds[birdIndex].flags |= 2;
            
            break;

        case 17:
            setEntityCollidable(gBirds[birdIndex].entityIndex, 0);
            setEntityYMovement(gBirds[birdIndex].entityIndex, 0);
            setEntityShadow(gBirds[birdIndex].entityIndex, 0xFF);

            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].yDisplacement = 2;

            setEntityAnimation(gBirds[birdIndex].entityIndex, 0x19);

            gBirds[birdIndex].unk_13++;
            gBirds[birdIndex].flags |= 2;

            if (gBirds[birdIndex].unk_13 == 6) {
                deactivateEntity(gBirds[birdIndex].entityIndex);
                gBirds[birdIndex].flags = 0;
            }
            
            break;
        
       case 32:
            gBirds[birdIndex].zDisplacement = 4;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 8);
            
            tempDirection = calculateAnimalDirectionToPlayer(entities[gBirds[birdIndex].entityIndex].coordinates.x, entities[gBirds[birdIndex].entityIndex].coordinates.z, entities[ENTITY_PLAYER].coordinates.x, entities[ENTITY_PLAYER].coordinates.z);

            gBirds[birdIndex].direction = (tempDirection + 4) % 8;
            
            if (!getRandomNumberInRange(0, 30)) {
                gBirds[birdIndex].flags &= ~(0x10);
            }
            
            gBirds[birdIndex].flags |= 2;
            break;

    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80098188);

void func_80098188(u8 birdIndex) {

    u16 temp;
    u8 tempDirection;
    
    switch (gBirds[birdIndex].unk_F) {

        case 0:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0);

            if (getRandomNumberInRange(0, 60) >= 40) {

                temp = getRandomNumberInRange(0, 10);

                if (temp < 7) {
                    gBirds[birdIndex].direction = temp;
                }

                temp = getRandomNumberInRange(0, 2);

                if (temp == 0) {
                    gBirds[birdIndex].unk_F = 1;
                }
                
                if (temp == 1) {
                    gBirds[birdIndex].unk_F = 2;
                }
                
                if (temp == 2) {
                    gBirds[birdIndex].unk_F = 3;
                }
                
            } else {
                gBirds[birdIndex].unk_F = 0;
            }

            gBirds[birdIndex].flags |= 2;
        
            break;


        case 1:
            gBirds[birdIndex].zDisplacement = 1;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 8);

            if (getRandomNumberInRange(0, 19) < 10) {
                gBirds[birdIndex].unk_F = 1;
            } else {
                gBirds[birdIndex].unk_F = 0;
            }
            
            gBirds[birdIndex].flags |= 2;
            break;



        case 2:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0x10);

            if (getRandomNumberInRange(0, 20) < 10) {
                gBirds[birdIndex].unk_F = 2;
            } else {
                gBirds[birdIndex].unk_F = 0;
            }
            
            gBirds[birdIndex].flags |= 2;
            setAudio(0x4D);
            
            break;


        case 3:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;
            setEntityAnimation(gBirds[birdIndex].entityIndex, 0x22);

            gBirds[birdIndex].unk_F = 0;
            gBirds[birdIndex].flags |= 2;
            break;
    
        case 16:
            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0);

            gBirds[birdIndex].unk_F = 0x10;
            gBirds[birdIndex].flags |= 2;
            
            break;

        case 17:

            setEntityCollidable(gBirds[birdIndex].entityIndex, 0);
            setEntityYMovement(gBirds[birdIndex].entityIndex, 0);
            setEntityShadow(gBirds[birdIndex].entityIndex, 0xFF);

            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].yDisplacement = 2;

            setEntityAnimation(gBirds[birdIndex].entityIndex, 0x1D);

            gBirds[birdIndex].unk_13++;
            gBirds[birdIndex].flags |= 2;

            if (gBirds[birdIndex].unk_13 == 6) {
                deactivateEntity(gBirds[birdIndex].entityIndex);
                gBirds[birdIndex].flags = 0;
            }
            
            break;
        
       case 32:
            gBirds[birdIndex].zDisplacement = 4;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;
            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 8);
            
            tempDirection = calculateAnimalDirectionToPlayer(entities[gBirds[birdIndex].entityIndex].coordinates.x, entities[gBirds[birdIndex].entityIndex].coordinates.z, entities[ENTITY_PLAYER].coordinates.x, entities[ENTITY_PLAYER].coordinates.z);

            gBirds[birdIndex].direction = (tempDirection + 4) % 8;
            
            if (!getRandomNumberInRange(0, 30)) {
                gBirds[birdIndex].flags &= ~(0x10);
            }
            
            gBirds[birdIndex].flags |= 2;
            break;


    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80098740);

void func_80098740(u8 birdIndex) {

    u16 temp;
    
    switch (gBirds[birdIndex].unk_F) {

        case 0:

            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;
            
            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0);
            
            if (getRandomNumberInRange(0, 60) < 40) {

                gBirds[birdIndex].unk_F = 0;
            
            } else {
                
                temp = getRandomNumberInRange(0, 10);
                
                if (temp < 7) {
                    gBirds[birdIndex].direction = temp;
                }

                if (getRandomNumberInRange(0, 30) >= 28) {
                    gBirds[birdIndex].unk_F = 32;
                } else {
                    gBirds[birdIndex].unk_F = 1;
                }
                
            }
            
            gBirds[birdIndex].flags |= 2;

            break;

        case 1:

            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;

            temp = getRandomNumberInRange(0, 3); 

            switch (temp) {

                case 0:
                    setEntityAnimation(gBirds[birdIndex].entityIndex, 0x10);
                    break;
                case 1:
                    setEntityAnimation(gBirds[birdIndex].entityIndex, 0x11);
                    break;
                case 2:
                    setEntityAnimation(gBirds[birdIndex].entityIndex, 0x12);
                    break;
                
                case 3:
                    setEntityAnimation(gBirds[birdIndex].entityIndex, 0x13);
                    break;
            } 
                
            if (getRandomNumberInRange(0, 60) < 40) {
                gBirds[birdIndex].unk_F = 1;
            } else {
                gBirds[birdIndex].unk_F = 0;
            }
            
            gBirds[birdIndex].flags |= 2;

            setAudio(0x46);
            
            break;

        case 32:

            setEntityCollidable(gBirds[birdIndex].entityIndex, FALSE);
            setEntityTracksCollisions(gBirds[birdIndex].entityIndex, FALSE);
            enableEntityMovement(gBirds[birdIndex].entityIndex, FALSE);
            setEntityYMovement(gBirds[birdIndex].entityIndex, FALSE);
            setEntityShadow(gBirds[birdIndex].entityIndex, 0xFF);
            
            entities[gBirds[birdIndex].entityIndex].yOffset = 0;
            
            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 8);
            
            gBirds[birdIndex].flags |= 2;

            if (gBirds[birdIndex].unk_13 < 129) {
                gBirds[birdIndex].zDisplacement = 4;
                gBirds[birdIndex].yDisplacement = 2;
                gBirds[birdIndex].unk_13++;
            } else {
                deactivateEntity(gBirds[birdIndex].entityIndex);
                gBirds[birdIndex].flags = 0;
            }
            
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80098B3C);

void func_80098B3C(u8 birdIndex) {
    
    u16 temp;
    
    switch (gBirds[birdIndex].unk_F) {

        case 0:

            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;
            
            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0);
            
            if (getRandomNumberInRange(0, 60) < 40) {

                gBirds[birdIndex].unk_F = 0;
            
            } else {
                
                temp = getRandomNumberInRange(0, 10);
                
                if (temp < 7) {
                    gBirds[birdIndex].direction = temp;
                }

                if (getRandomNumberInRange(0, 30) >= 28) {
                    gBirds[birdIndex].unk_F = 32;
                } else {
                    gBirds[birdIndex].unk_F = 1;
                }
                
            }
            
            gBirds[birdIndex].flags |= 2;

            break;

        case 1:

            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;

            temp = getRandomNumberInRange(0, 3); 

            switch (temp) {

                case 0:
                    setEntityAnimation(gBirds[birdIndex].entityIndex, 0x10);
                    break;
                case 1:
                    setEntityAnimation(gBirds[birdIndex].entityIndex, 0x11);
                    break;
                case 2:
                    setEntityAnimation(gBirds[birdIndex].entityIndex, 0x12);
                    break;
                
                case 3:
                    setEntityAnimation(gBirds[birdIndex].entityIndex, 0x13);
                    break;
            } 
                
            if (getRandomNumberInRange(0, 60) < 40) {
                gBirds[birdIndex].unk_F = 1;
            } else {
                gBirds[birdIndex].unk_F = 0;
            }
            
            gBirds[birdIndex].flags |= 2;

            if (!getRandomNumberInRange(0, 1)) {
                setAudio(0x48);
            } else {
                setAudio(0x49);
            }
            
            break;

        case 32:

            setEntityCollidable(gBirds[birdIndex].entityIndex, FALSE);
            setEntityTracksCollisions(gBirds[birdIndex].entityIndex, FALSE);
            enableEntityMovement(gBirds[birdIndex].entityIndex, FALSE);
            setEntityYMovement(gBirds[birdIndex].entityIndex, FALSE);
            setEntityShadow(gBirds[birdIndex].entityIndex, 0xFF);
            entities[gBirds[birdIndex].entityIndex].yOffset = 0;
            
            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 8);

            gBirds[birdIndex].zDisplacement = 4;
            gBirds[birdIndex].yDisplacement = 2;
            gBirds[birdIndex].flags |= 2;
            gBirds[birdIndex].unk_13++;
                    
            break;

    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80098F24);

void func_80098F24(u8 birdIndex) {
    
    u32 temp;
    u16 temp2;
    
    switch (gBirds[birdIndex].unk_F) {

        case 0:

            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;
            
            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0);
            
            if (getRandomNumberInRange(0, 60) >= 40) {

                temp = getRandomNumberInRange(0, 10);

                if (temp < 7) {
                    // ???
                    temp &= ~3;
                    temp2 = temp;
                    gBirds[birdIndex].direction = temp2;
                } 
                
                gBirds[birdIndex].unk_F = 1;
            
            } else {
                gBirds[birdIndex].unk_F = 0;
            }
            
            gBirds[birdIndex].flags |= 2;

            break;

        case 1:

            gBirds[birdIndex].zDisplacement = 1;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;
            
            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 8);

            if (getRandomNumberInRange(0, 19) < 10) {
                gBirds[birdIndex].unk_F = 1;
            } else {
                gBirds[birdIndex].unk_F = 0;
            }
            
            gBirds[birdIndex].flags |= 2;

            break;

        case 32:

            setEntityCollidable(gBirds[birdIndex].entityIndex, 0);
            setEntityYMovement(gBirds[birdIndex].entityIndex, 0);
            gBirds[birdIndex].direction = SOUTHEAST;
            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 8);
            
            gBirds[birdIndex].flags |= 2;

            if (gBirds[birdIndex].unk_13 < 17) {
                gBirds[birdIndex].zDisplacement = 4;
                gBirds[birdIndex].yDisplacement = 0;
                gBirds[birdIndex].unk_13++;
            } else {
                deactivateEntity(gBirds[birdIndex].entityIndex);
                gBirds[birdIndex].flags = 0;
            }
                    
            break;
            
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_800991C0);

void func_800991C0(u8 birdIndex) {

    u16 temp;
    
    switch (gBirds[birdIndex].unk_F) {

        case 0:

            gBirds[birdIndex].zDisplacement = 0;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;
            
            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 0);
            
            if (getRandomNumberInRange(0, 60) >= 40) {
                
                temp = getRandomNumberInRange(0, 10);
                
                if (temp < 7) {
                    gBirds[birdIndex].direction = temp;
                }
                
                gBirds[birdIndex].unk_F = 1;
            
            } else {
                gBirds[birdIndex].unk_F = 0;
            }
            
            gBirds[birdIndex].flags |= 2;

            break;

        case 1:

            gBirds[birdIndex].zDisplacement = 1;
            gBirds[birdIndex].unk_13 = 0;
            gBirds[birdIndex].unk_14 = 0;
            
            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 8);

            if (getRandomNumberInRange(0, 19) < 10) {
                gBirds[birdIndex].unk_F = 1;
            } else {
                gBirds[birdIndex].unk_F = 0;
            }
            
            gBirds[birdIndex].flags |= 2;

            break;

        case 32:

            setEntityAnimationWithDirectionChange(gBirds[birdIndex].entityIndex, 8);
            
            gBirds[birdIndex].flags |= 2;

            if (gBirds[birdIndex].unk_13 < 33) {
                gBirds[birdIndex].zDisplacement = 4;
                gBirds[birdIndex].yDisplacement = 0;
                gBirds[birdIndex].unk_13++;
            } else {
                deactivateEntity(gBirds[birdIndex].entityIndex);
                gBirds[birdIndex].flags = 0;
            }
                    
            break;
            
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80099424);

void func_80099424(u8 index) {

    switch (gBirds[index].unk_F) {

        case 0:
        case 1:

            setEntityCollidable(gBirds[index].entityIndex, FALSE);
            setEntityTracksCollisions(gBirds[index].entityIndex, FALSE);
    
            entities[gBirds[index].entityIndex].yOffset = 0x20;
    
            gBirds[index].zDisplacement = 1;
            gBirds[index].unk_13 = 0;
            gBirds[index].unk_14 = 0;
    
            setEntityAnimation(gBirds[index].entityIndex, 2);
    
            gBirds[index].direction = getRandomNumberInRange(0, 7);
            gBirds[index].unk_F = 0;
            gBirds[index].flags |= 2;
            
            break;
        
        default:
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80099548);

void func_80099548(u8 index) {

    switch (gBirds[index].unk_F) {

        case 0:
        case 1:

            gBirds[index].zDisplacement = 0;
            gBirds[index].unk_13 = 0;
            gBirds[index].unk_14 = 0;
    
            setEntityAnimation(gBirds[index].entityIndex, 0);

            gBirds[index].unk_F = 0;
            gBirds[index].flags |= 2;

            break;
        
        default:
            break;
    
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_800995F8);

void func_800995F8(u8 index) {

    switch (gBirds[index].unk_F) {

        case 0:

            gBirds[index].zDisplacement = 0;
            gBirds[index].unk_13 = 0;
            gBirds[index].unk_14 = 0;

            setEntityAnimation(gBirds[index].entityIndex, 0);

            entities[gBirds[index].entityIndex].yOffset = 0x40;

            gBirds[index].flags |= 2;

            if (!(5 < gHour && gHour < 16)) {
                setAudio(0x51);
            } else {
                setAudio(0x4E);
            }
            
            break;
        
        case 1:

            setEntityCollidable(gBirds[index].entityIndex, FALSE);
            setEntityTracksCollisions(gBirds[index].entityIndex, FALSE);
            enableEntityMovement(gBirds[index].entityIndex, FALSE);
            setEntityYMovement(gBirds[index].entityIndex, FALSE);
            setEntityShadow(gBirds[index].entityIndex, 0xFF);

            entities[gBirds[index].entityIndex].yOffset = 0;

            setEntityAnimation(gBirds[index].entityIndex, 2);

            gBirds[index].zDisplacement = 4;
            gBirds[index].yDisplacement = 2;

            gBirds[index].flags |= 2;

            gBirds[index].unk_13++;
            
            break;    
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80099804);

void func_80099804(u8 index) {

     switch (gBirds[index].unk_F) {

        case 0:

            gBirds[index].zDisplacement = 0;
            gBirds[index].unk_13 = 0;
            gBirds[index].unk_14 = 0;

            setEntityAnimation(gBirds[index].entityIndex, 0);

            gBirds[index].flags |= 2;

            break;
        
        case 1:

            setEntityCollidable(gBirds[index].entityIndex, FALSE);
            setEntityTracksCollisions(gBirds[index].entityIndex, FALSE);
            enableEntityMovement(gBirds[index].entityIndex, FALSE);
            setEntityYMovement(gBirds[index].entityIndex, FALSE);
            setEntityShadow(gBirds[index].entityIndex, 0xFF);

            entities[gBirds[index].entityIndex].yOffset = 0;

            setEntityAnimation(gBirds[index].entityIndex, 4);

            gBirds[index].zDisplacement = 4;
            gBirds[index].yDisplacement = 2;

            gBirds[index].flags |= 2;

            gBirds[index].unk_13++;
            
            break;    
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_800999B0);

void func_800999B0(u8 index) {

    switch (gBirds[index].unk_F) {

        case 0:

            gBirds[index].zDisplacement = 0;
            gBirds[index].unk_13 = 0;
            gBirds[index].unk_14 = 0;

            setEntityAnimation(gBirds[index].entityIndex, 0);

            gBirds[index].flags |= 2;

            break;
        
        case 1:

            setEntityCollidable(gBirds[index].entityIndex, FALSE);
            setEntityTracksCollisions(gBirds[index].entityIndex, FALSE);
            enableEntityMovement(gBirds[index].entityIndex, FALSE);
            setEntityYMovement(gBirds[index].entityIndex, FALSE);
            setEntityShadow(gBirds[index].entityIndex, 0xFF);

            entities[gBirds[index].entityIndex].yOffset = 0;

            setEntityAnimation(gBirds[index].entityIndex, 4);

            gBirds[index].zDisplacement = 4;
            gBirds[index].yDisplacement = 2;

            gBirds[index].flags |= 2;

            gBirds[index].unk_13++;
            
            break;    
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80099B5C);

void func_80099B5C(u8 index) {

    u16 temp;
    
    switch (gBirds[index].unk_F) {

        case 0:
        case 1:

            setEntityCollidable(gBirds[index].entityIndex, FALSE);
            setEntityTracksCollisions(gBirds[index].entityIndex, FALSE);
                        
            entities[gBirds[index].entityIndex].yOffset = 0x20;

            gBirds[index].zDisplacement = 1;
            gBirds[index].unk_13 = 0;
            gBirds[index].unk_14 = 0;

            setEntityAnimation(gBirds[index].entityIndex, 2);

            temp = getRandomNumberInRange(0, 20);

            if (temp < 7) {
                gBirds[index].direction = temp;
            }
            
            gBirds[index].unk_F = 0;
            gBirds[index].flags |= 2;
            
            break;    
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80099C94);

void func_80099C94(u8 index) {
    
    switch (gBirds[index].unk_F) {

        case 0:

            gBirds[index].zDisplacement = 0;
            gBirds[index].unk_13 = 0;
            gBirds[index].unk_14 = 0;

            setEntityAnimation(gBirds[index].entityIndex, 0);

            gBirds[index].flags |= 2;

            break;
        
        case 1:

            setEntityAnimation(gBirds[index].entityIndex, 2);
            gBirds[index].flags |= 2;

            if (gBirds[index].unk_13 < 0x21) {
                gBirds[index].zDisplacement = 2;
                gBirds[index].yDisplacement = 0;
                gBirds[index].unk_13++;
            } else {
                deactivateEntity(gBirds[index].entityIndex);
                gBirds[index].flags = 0;
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

        if ((gFarmAnimals[i].flags & 4) && gFarmAnimals[i].unk_1B == 0x10) {

            func_800861A4(2, i, 0xFF, 0xFF, 0);

            if (!(gFarmAnimals[i].flags & 0x20)) {

                switch (gFarmAnimals[i].type) {

                    case 0 ... 3:
                        func_80086458(i, 2);
                        func_800861A4(2, i, 0xFF, 0xFF, 0x11);
                        gFarmAnimals[i].flags |= (0x20 | 0x8000);
                        func_8009BCC4(2, i, 3);
                        set = TRUE;
                        break;

                    case 4 ... 6:
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

        if (gFarmAnimals[i].flags & 4 && gFarmAnimals[i].unk_1B == 0x10) {

            func_800861A4(2, i, 0xFF, 0xFF, 0);

            switch (gFarmAnimals[i].type) {

                case 2:
                case 5 ... 6:
                    if (gFarmAnimals[i].condition == 3) {
                        func_800861A4(2, i, 0xFF, 0, 0);
                        set = TRUE;
                    } 
                    
                
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

        if (gFarmAnimals[i].flags & 4 && gFarmAnimals[i].unk_1B == 0x10) {

            func_800861A4(2, i, 0xFF, 0xFF, 0);

            if (!(gFarmAnimals[i].flags & 0x40)) {

                switch (gFarmAnimals[i].type) {

                    case 2:
                        
                        if (gFarmAnimals[i].condition < 2) {
                            func_80086458(i, 1);
                            func_800861A4(2, i, 0xFF, 0xFF, 0x11);
                            gPlayer.heldItem = func_8008662C(i, 0);
                            gFarmAnimals[i].flags |= (0x40 | 0x8000);
                            func_8009BCC4(2, i, 3);
                            set = TRUE;
                        }
                        
                        break;
                    
                    case 3:
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
    
            if (gFarmAnimals[i].flags & 4 && gFarmAnimals[i].unk_1B == 0x10) {
    
                func_800861A4(2, i, 0xFF, 0xFF, 0);
                
                if (!(gFarmAnimals[i].flags & 0x80) && gFarmAnimals[i].type == 2) {
                
                    func_80086458(i, 10);
                    func_800861A4(2, i, 0xFF, 0, 0x11);
                    
                    gFarmAnimals[i].flags |= (0x80 | 0x8000);
                    
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

        if (gFarmAnimals[i].flags & 4 && gFarmAnimals[i].unk_1B == 0x10) {

            func_800861A4(2, i, 0xFF, 0xFF, 0);

            switch (gFarmAnimals[i].type) {

                case 2:
                    if (gFarmAnimals[i].condition < 2) {
                        func_800861A4(2, i, 0xFF, 0xFF, 0x11);
                        set = TRUE;
                        gFarmAnimals[i].flags |= 0x8000;
                        gPlayer.bottleContents = 6;
                    }
                    break;

                case 3:
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

        if (gFarmAnimals[i].flags & 4 && gFarmAnimals[i].unk_1B == 0x10) {

            func_800861A4(2, i, 0xFF, 0xFF, 0);

            if (!(gFarmAnimals[i].flags & 0x100) && gFarmAnimals[i].type == 5) {
                
                func_80086458(i, 2);
                func_800861A4(2, i, 6, 0xFF, 0x11);
                
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
        
        if (gFarmAnimals[i].flags & 1 && gFarmAnimals[i].flags & 0x80) {
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
            
            if (gFarmAnimals[i].type == 3) {
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
            
            if ((u8)(gFarmAnimals[i].type - 1) < 2U || (gFarmAnimals[i].type == 0 || gFarmAnimals[i].type == 3)) {
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
        
        if (gFarmAnimals[i].flags & 1 &&  gFarmAnimals[i].type == 2) {

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
            
            if (3 < gFarmAnimals[i].type && gFarmAnimals[i].type < 6 || gFarmAnimals[i].type == 6) {
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
            
            if ((gChickens[i].type - 1) < 2U) {
                count++;
            }

            if (gChickens[i].type == 0 && gChickens[i].flags & 0x20) {
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
        if (gChickens[i].flags & 1 && gChickens[i].type == 2 && gChickens[i].condition == 0) {
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
        if (gChickens[i].flags & 1 && gChickens[i].type == 0) {
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
        } else if (gChickens[i].type == 0) {
          
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

        if ((gChickens[i].flags & 1) && (gChickens[i].type == 0) && (gChickens[i].flags & 0x20)) {
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

        if ((gFarmAnimals[i].flags & 1) 
            && (((u8)(gFarmAnimals[i].type - 1) < 2U 
                || (gFarmAnimals[i].type == 0 || gFarmAnimals[i].type == 3))
            && gFarmAnimals[i].location == FARM && (arg0 == 0 || !gFarmAnimals[i].goldenMilk))) {

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
                temp3 = D_80114AE0[0];
            }
            
            if (99 < temp && temp < 200) {
                temp3 = D_80114AE6[0];
            }

            if (temp >= 200) {
                temp3 = D_80114AEC[0];
            }

            D_801890E0 = temp3;
            
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

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009BBAC);

u8 func_8009BBAC(void) {

    u8 temp;
    u8 found = 0xFF;
    u8 i = 0;

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {

        if (gFarmAnimals[i].flags & 1) {

            // FIXME
            if ((u8)(gFarmAnimals[i].type - 1) < 2U || (gFarmAnimals[i].type == 0 || gFarmAnimals[i].type == 3)) {
                
                temp = gFarmAnimals[i].goldenMilk;
    
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

    for (i = 0; i < MAX_CHICKEN_EGGS; i++) {
        gBirds[i].unk_16 = getRandomNumberInRange(0, 3);
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