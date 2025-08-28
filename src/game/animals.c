#include "common.h"

#include "game/animals.h"

#include "system/entity.h"
#include "system/graphic.h"
#include "system/mathUtils.h"
#include "system/mapContext.h"

#include "game/game.h"
#include "game/gameAudio.h"
#include "game/gameStatus.h"
#include "game/level.h"
#include "game/spriteIndices.h"

// bss
Chicken gChickens[MAX_CHICKENS];
ChickenEgg gChickenEggs[];
FarmAnimal gFarmAnimals[];
u8 D_801886D4[6];
Dog dogInfo;
Horse horseInfo;

// TODO: label
// watched cow stall #3
u8 D_8016F898;
// watched cow stall #1
u8 D_8016FAF8;
// watched cow stall #2
u8 D_801C3F46;

extern u16 D_8016FDF0;
extern u8 D_8016FFE8;
// newest chicken index?
extern u8 D_80170464;

extern u8 D_80189054;
extern u8 D_8018908C;
// newest animal index (generic)?
extern u8 D_801FC155;
 
// data
// chicken starting coordianates
extern Vec3f D_801149C0[];
// farm animal starting coordinates
extern Vec3f farmAnimalStartingCoordinates[];
extern Vec3f D_80114AB0;
extern u16 D_80114ABC[];
extern u16 D_80114AC4[];
extern u16 D_80114ACC[];
extern u16 D_80114AD4[];
extern u16 D_80114ADC[];
extern u16 D_80114AE2[];
extern u16 D_80114AE8[];

// forward declarations
void func_800861A4(u8, u8, u8, u8, u8);               
void func_80086458(u8, s8); 
void setFarmAnimalLocation(u8); 
void func_8008B1B8();                                  
void func_8008B2E8(u8);                               
void func_8008B55C(u8);                               
void func_8008B9AC();                                  
void func_8008BAF0(u8, u8); 
void func_8008C208();                                  
void func_8008CF94(u8);                               
void func_8008DAA0(u8);                               
void func_80093B80();                                  
void func_80093CE0();                                 
void func_8009476C(u8);     
void func_8009427C();       
void func_8008A5F0();
void func_8008A650(u8);                               
void func_8008A9E8(u8);                               
void func_8008B150();       
void func_8009BCC4(u8, u8, u8);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_80085F70);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_800861A4);

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008634C);

void func_8008634C(s8 arg0) {

    u8 i;

    adjustDogAffection(arg0);
    adjustHorseAffection(arg0);

    for (i = 0; i < 8; i++) {
        func_80086458(i, arg0);
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", adjustDogAffection);

void adjustDogAffection(s8 arg0) {
    if (dogInfo.flags & 1) {
        dogInfo.affection += adjustValue(dogInfo.affection, arg0, 0xFF);
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", adjustHorseAffection);

void adjustHorseAffection(s8 arg0) {
    if (horseInfo.flags & 1) {
        horseInfo.affection += adjustValue(horseInfo.affection, arg0, 0xFF);
    }
}

// jtbl_80120320
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

        // FIXME: something wrong here
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
        if (entities[dogInfo.spriteIndex].flags & 8) {
            deactivateEntity(dogInfo.spriteIndex);
        }
    }

    for (i = 0; i < MAX_CHICKENS; i++) {

        if (gChickens[i].flags & 4) {

            if (entities[gChickens[i].spriteIndex].flags & 8) {
                deactivateEntity(gChickens[i].spriteIndex);
            }

        }
    }

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {
        if (gFarmAnimals[i].flags & 4) {
            if (entities[gFarmAnimals[i].spriteIndex].flags & 8) {
                deactivateEntity(gFarmAnimals[i].spriteIndex);
            }
        }
    }

    if (horseInfo.flags & 4) {
        if (entities[horseInfo.spriteIndex].flags & 8) {
            deactivateEntity(horseInfo.spriteIndex);
        }
    }

    for (i = 0; i < 7; i++) {
        if (gChickenEggs[i].flags & 4) {
            if (entities[gChickenEggs[i].spriteIndex].flags & 8) {
                deactivateEntity(gChickenEggs[i].spriteIndex);
            }
        }
    }
    
}

INCLUDE_ASM("asm/nonmatchings/game/animals", func_800879C8);

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80087CD4);

void func_80087CD4(void) {

    u8 i;
    
    func_8008C208();

    for (i = 0; i < MAX_CHICKENS; i++) {
        func_8008CF94(i);
    }

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {
        func_8008DAA0(i);
    }

    func_80093B80();
    
    for (i = 0; i < 7; i++) {
        func_8009476C(i);
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80087D5C);

void func_80087D5C(void) {

    u8 i;

    for (i = 0; i < MAX_CHICKENS; i++) {
        gChickens[i].flags |= 0x10;
    }

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {
        gFarmAnimals[i].flags |= (8 | 0x10);
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80087DEC);

void func_80087DEC(void) {

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
        gChickens[i].flags &= ~(0x10 | 0x80);
    }

    for (i = 0; i < MAX_FARM_ANIMALS; i++) { 
        func_8008A9E8(i);
        gFarmAnimals[i].flags &= ~(8 | 0x10 | 0x20 | 0x40 | 0x80 | 0x100 | 0x1000 | 0x4000);
    }

    func_8008B150();

    horseInfo.flags &= ~(0x100 | 0x200 | 0x400 | 0x800);

}

INCLUDE_ASM("asm/nonmatchings/game/animals", func_80087F28);

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

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008841C);

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_800886D0);

// initialize watched cows from Doug
void func_800886D0(void) {
    initializeFarmAnimal(D_8016FAF8);
    initializeFarmAnimal(D_801C3F46);
    initializeFarmAnimal(D_8016F898);
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", setFarmAnimalLocation);

// set farm animal locations
void setFarmAnimalLocation(u8 animalIndex) {

    gFarmAnimals[animalIndex].location = BARN;

    if (gFarmAnimals[animalIndex].type == UNUSED_ANIMAL_TYPE) {
        
        gFarmAnimals[animalIndex].coordinates.x = D_80114AB0.x;
        gFarmAnimals[animalIndex].coordinates.y = D_80114AB0.y;
        gFarmAnimals[animalIndex].coordinates.z = D_80114AB0.z;
        gFarmAnimals[animalIndex].unk_1C = 4;

    } else {

        gFarmAnimals[animalIndex].coordinates.x = farmAnimalStartingCoordinates[animalIndex].x;
        gFarmAnimals[animalIndex].coordinates.y = farmAnimalStartingCoordinates[animalIndex].y;
        gFarmAnimals[animalIndex].coordinates.z = farmAnimalStartingCoordinates[animalIndex].z;
        
        if (animalIndex >= 4) {
            gFarmAnimals[animalIndex].unk_1C = 6;
        } else {
            gFarmAnimals[animalIndex].unk_1C = 2;
        }
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", initializeFarmAnimal);

void initializeFarmAnimal(u8 animalIndex) {

    gFarmAnimals[animalIndex].location = 0;
    
    gFarmAnimals[animalIndex].unk_1B = 0;
    gFarmAnimals[animalIndex].unk_1C = 0;
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
    dogInfo.unk_19 = 0;
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
    horseInfo.unk_19 = 0;
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

            gChickens[chickenIndex].coordinates.x = D_801149C0[chickenIndex].x;
            gChickens[chickenIndex].coordinates.y = D_801149C0[chickenIndex].y;
            gChickens[chickenIndex].coordinates.z = D_801149C0[chickenIndex].z;
            
        }

        gChickens[chickenIndex].flags &= ~8;
        
    }
}

INCLUDE_ASM("asm/nonmatchings/game/animals", func_80088D54);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008A4A8);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008A5F0);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008A650);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008A9E8);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008B150);

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008B1B8);

void func_8008B1B8(void) {
    
    if (dogInfo.flags & 1 && (dogInfo.location == gBaseMapIndex) && !(dogInfo.flags & 8)) {
        
        dogInfo.spriteIndex = 1;
        
        loadEntity(1, 0x40, 1);
        func_8003019C(dogInfo.spriteIndex, 1);
        func_80030054(dogInfo.spriteIndex, 1);
        func_8002FF38(dogInfo.spriteIndex, 0);
        
        setEntityDirection(dogInfo.spriteIndex, convertSpriteToMapDirection(dogInfo.direction, MAIN_MAP_INDEX));
        setEntityCoordinates(dogInfo.spriteIndex, dogInfo.coordinates.x, dogInfo.coordinates.y, dogInfo.coordinates.z);
        
        dogInfo.unk_17 = 0;
        dogInfo.unk_1A = 0;
        dogInfo.flags |= 4;
        
    }
    
    func_8008C208();
    
}

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008B2E8);

void func_8008B2E8(u8 chickenIndex) {

    if ((gChickens[chickenIndex].flags & 1) && (gChickens[chickenIndex].location == gBaseMapIndex) && !(gChickens[chickenIndex].flags & 8)) {
    
        gChickens[chickenIndex].spriteIndex = chickenIndex + 2;
        
        switch (gChickens[chickenIndex].type) {

            case 2:
                initializeAnimalEntity(chickenIndex + 2, (void*)0x80275E00, (void*)0x80275F00, (void*)0x80276700, (void*)0x80276800);
                loadEntity(gChickens[chickenIndex].spriteIndex, 0x44, 1);
                break;         
            
            case 1:
                initializeAnimalEntity(chickenIndex + 2, (void*)0x80276B00, (void*)0x80276C00, (void*)0x80277000, (void*)0x80277100);
                loadEntity(gChickens[chickenIndex].spriteIndex, 0x43, 1);
                break;
            
            case 0:
                initializeAnimalEntity(chickenIndex + 2, (void*)0x8028DD50, (void*)0x80290550, (void*)0x80293A50, (void*)0x80293C50);
                loadEntity(gChickens[chickenIndex].spriteIndex, 0x5D, 1);
                break;
            
            default:
                break;
            
        }

        func_8003019C(gChickens[chickenIndex].spriteIndex, 1);
        func_80030054(gChickens[chickenIndex].spriteIndex, 1);
        func_8002FF38(gChickens[chickenIndex].spriteIndex, 0);

        setEntityDirection(gChickens[chickenIndex].spriteIndex, convertSpriteToMapDirection(gChickens[chickenIndex].direction, MAIN_MAP_INDEX));        
        setEntityCoordinates(gChickens[chickenIndex].spriteIndex, gChickens[chickenIndex].coordinates.x, gChickens[chickenIndex].coordinates.y, gChickens[chickenIndex].coordinates.z);
        
        gChickens[chickenIndex].unk_17 = 0;
        gChickens[chickenIndex].flags |= 4;
        
    }
    
    func_8008CF94(chickenIndex);
    
}

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008B55C);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008B9AC);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008BAF0);

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008C208);

void func_8008C208(void) {

    Vec3f vec;

    if ((dogInfo.flags & 1) && (dogInfo.flags & 4)) {
        
        if (checkEntityAnimationStateChanged(dogInfo.spriteIndex) || dogInfo.flags & 0x20) {

            if (dogInfo.unk_1A == 0) {
                func_8008C358();
            } else {
                dogInfo.unk_1A--;
            }
            
            dogInfo.flags &= ~0x20;
            
        }
        
        setEntityDirection(dogInfo.spriteIndex, convertSpriteToMapDirection(dogInfo.direction, MAIN_MAP_INDEX));
        getMovementVectorFromDirection(&vec, dogInfo.unk_19, dogInfo.direction, 0.0f);
        func_8002FE10(dogInfo.spriteIndex, vec.x, vec.y, vec.z, dogInfo.unk_19);

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

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008E98C);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008F37C);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8008FC68);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_800904E4);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_80090DF8);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009170C);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_80092094);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_80092808);

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80092A64);

void func_80092A64(u8 index) {

    u16 temp;

    switch(gFarmAnimals[index].unk_1B) {
        
        case 0:
            
            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].spriteIndex, 0x38);

            if (gFarmAnimals[index].flags & 0x200 || gFarmAnimals[index].flags & 0x400) {
                
                gFarmAnimals[index].unk_1C = func_80060EC8(entities[gFarmAnimals[index].spriteIndex].coordinates.x, entities[gFarmAnimals[index].spriteIndex].coordinates.z, entities[PLAYER].coordinates.x, entities[PLAYER].coordinates.z);
                gFarmAnimals[index].unk_1B = 1;
            
            } else if (getRandomNumberInRange(0, 60) < 40) {
            
                gFarmAnimals[index].unk_1B = 0;
                    
            } else {
                
                temp = getRandomNumberInRange(0, 20);
                
                if (temp < 7) {
                    gFarmAnimals[index].unk_1C = temp;
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
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].spriteIndex, 0x40);

            if (gFarmAnimals[index].flags & 0x200) {

                gFarmAnimals[index].flags &= ~0x400;
                gFarmAnimals[index].unk_1C = func_80060EC8(entities[gFarmAnimals[index].spriteIndex].coordinates.x, entities[gFarmAnimals[index].spriteIndex].coordinates.z, entities[PLAYER].coordinates.x, entities[PLAYER].coordinates.z);

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
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].spriteIndex, 0x48);
            
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
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].spriteIndex, 0x50);
            gFarmAnimals[index].unk_1B = 4;
            gFarmAnimals[index].flags |= 2;
            break;

        case 4:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].spriteIndex, 0x58);

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
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].spriteIndex, 0x60);

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
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].spriteIndex, 0x50);
            gFarmAnimals[index].unk_1B = 0;
            gFarmAnimals[index].flags |= 2;
            
            break;

        case 16:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].spriteIndex, 0x38);
            gFarmAnimals[index].flags |= (0x2 | 0x2000);
            gFarmAnimals[index].flags &= ~0x400;
            break;

        case 17:

            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].spriteIndex, 0x38);
            gFarmAnimals[index].unk_1B = 0;
            gFarmAnimals[index].flags |= (0x2 | 0x2000);
            gFarmAnimals[index].flags &= ~0x400;
            setAudio(0x3A);
        
            break;

        case 18:
            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].spriteIndex, 0x68);
            gFarmAnimals[index].unk_1B = 0;
            gFarmAnimals[index].flags |= (0x2 | 0x2000);
            gFarmAnimals[index].flags &= ~0x400;
            setAudio(0x3A);
            break;

        case 19:
            gFarmAnimals[index].unk_14 = 0;
            gFarmAnimals[index].unk_1D = 0;
            gFarmAnimals[index].unk_1E = 0;
            setEntityAnimationWithDirectionChange(gFarmAnimals[index].spriteIndex, 0x68);
            gFarmAnimals[index].unk_1B = 0x13;
            gFarmAnimals[index].flags |= (0x2 | 0x2000);
            gFarmAnimals[index].flags &= ~0x400;
            break;

    }
    
}

INCLUDE_ASM("asm/nonmatchings/game/animals", func_800931D8);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_80093434);

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_80093B80);

void func_80093B80(void) {

    Vec3f vec;

    if ((horseInfo.flags & 1) && (horseInfo.flags & 4)) {

        if (checkEntityAnimationStateChanged(horseInfo.spriteIndex) || horseInfo.flags & 0x20) {
            
            switch (horseInfo.grown) {
                case FALSE:
                    func_8009427C();
                    break;                    
                case TRUE:
                    func_80093CE0();
                    break;
            }
            
            horseInfo.flags &= ~(0x20 | 0x80);
            
        }
        
        setEntityDirection(horseInfo.spriteIndex, convertSpriteToMapDirection(horseInfo.direction, MAIN_MAP_INDEX));
        getMovementVectorFromDirection(&vec, horseInfo.unk_19, horseInfo.direction, 0.0f);
        func_8002FE10(horseInfo.spriteIndex, vec.x, vec.y, vec.z, horseInfo.unk_19);

    } 
}

INCLUDE_ASM("asm/nonmatchings/game/animals", func_80093CE0);

#ifdef PERMUTER
void func_8009427C(void) {

    u16 tempDirection;
    u16 temp;
    
    switch (horseInfo.unk_17) {

        case 0:
            
            horseInfo.unk_19 = 0;
            horseInfo.unk_1A = 0;
            horseInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(horseInfo.spriteIndex, 0);
            
            if (horseInfo.flags & 0x10) {

                horseInfo.direction = func_80060EC8(entities[horseInfo.spriteIndex].coordinates.x, entities[horseInfo.spriteIndex].coordinates.z, entities[PLAYER].coordinates.x, entities[PLAYER].coordinates.z);
                horseInfo.unk_17 = 4;
                
            } else if (getRandomNumberInRange(0, 60) >= 40) {

                tempDirection = getRandomNumberInRange(0, 20);

                if (tempDirection < MAX_DIRECTIONS) {
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
                
            } else {
                horseInfo.unk_17 = 0;
            }
            
            horseInfo.flags |= 2;
            break;

        case 1:

            horseInfo.unk_19 = 1;
            horseInfo.unk_1A = 0;
            horseInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(horseInfo.spriteIndex, 8);
            
            if (getRandomNumberInRange(0, 0x14) < 10) {
                horseInfo.unk_17 = 1;
            } else {
                horseInfo.unk_17 = 0;
            }

            horseInfo.flags |= 2;
            break;

        case 2:

            horseInfo.unk_19 = 0;
            horseInfo.unk_1A = 0;
            horseInfo.unk_1B = 0;
            setEntityAnimationWithDirectionChange(horseInfo.spriteIndex, 0x20);

            if (!getRandomNumberInRange(0, 2)) {
                horseInfo.unk_17 = 2;
            } else {   
                horseInfo.unk_17 = 0;    
            }       
            
            horseInfo.flags |= 2;
            break;

        case 3:

            horseInfo.unk_19 = 0;
            horseInfo.unk_1A = 0;
            horseInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(horseInfo.spriteIndex, 0x18);
            
            horseInfo.unk_17 = 6;
            horseInfo.flags |= 2;
            
            break;

        case 4:

            horseInfo.unk_19 = 2;
            horseInfo.unk_1A = 0;
            horseInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(horseInfo.spriteIndex, 0x10);

            if (horseInfo.flags & 0x10) {
                
                horseInfo.direction = func_80060EC8(entities[horseInfo.spriteIndex].coordinates.x, entities[horseInfo.spriteIndex].coordinates.z, entities[PLAYER].coordinates.x, entities[PLAYER].coordinates.z);
                
                if (!getRandomNumberInRange(0, 20)) {
                    horseInfo.flags &= ~(0x10);
                }

            } else {
                
                if (!getRandomNumberInRange(0, 2)) {
                    horseInfo.unk_17 = 4;
                } else {                
                    horseInfo.unk_17 = 0;    
                }
            
            }
        
            horseInfo.flags |= 2;
            break;

        case 5:

            horseInfo.unk_19 = 0;
            horseInfo.unk_1A = 0;
            horseInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(horseInfo.spriteIndex, 0x30);

            if (!getRandomNumberInRange(0, 2)) {
                horseInfo.unk_17 = 5;
            } else {   
                horseInfo.unk_17 = 0;    
            }
           
            horseInfo.flags |= 2;
            break;

        case 6:

            horseInfo.unk_19 = 0;
            horseInfo.unk_1A = 0;
            horseInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(horseInfo.spriteIndex, 0x28);

            if (getRandomNumberInRange(0, 2)) {
                horseInfo.unk_17 = 0;    
            } else {
                horseInfo.unk_17 = 6;
            }
           
            horseInfo.flags |= 2;
            break;
        
        case 16:
            
            horseInfo.unk_19 = 0;
            horseInfo.unk_1A = 0;
            horseInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(horseInfo.spriteIndex, 0);
            
            horseInfo.flags |= (0x2 | 0x80); 
            horseInfo.flags &= ~(0x40);
            
            break;

        case 17:

            horseInfo.unk_19 = 0;
            horseInfo.unk_1A = 0;
            horseInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(horseInfo.spriteIndex, 0x30);
            
            horseInfo.unk_17 = 0;
            horseInfo.flags |= (0x2 | 0x80); 
            horseInfo.flags &= ~(0x40);
            
            setAudio(0x39);
            
            break;

        case 18:

            horseInfo.unk_19 = 0;
            horseInfo.unk_1A = 0;
            horseInfo.unk_1B = 0;
            
            setEntityAnimationWithDirectionChange(horseInfo.spriteIndex, 0);
            
            horseInfo.unk_17 = 0;
            horseInfo.flags |= (0x2 | 0x80); 
            horseInfo.flags &= ~(0x40);
            
            setAudio(0x38);
            
            break;
        
    }
    
}
#else
INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009427C);
#endif

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009476C);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_80094A5C);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_800958EC);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_80096264);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_800967E0);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_80096978);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_80096DFC);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_800973F8);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_80097804);

// jtbl_80120E20
INCLUDE_ASM("asm/nonmatchings/game/animals", func_80097CB0);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_80098188);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_80098740);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_80098B3C);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_80098F24);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_800991C0);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_80099424);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_80099548);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_800995F8);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_80099804);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_800999B0);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_80099B5C);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_80099C94);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_80099DE8);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_80099EEC);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_80099FF0);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009A074);

//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009A100);

bool func_8009A100(void) {
    
    bool result = FALSE;

    if ((horseInfo.flags & 4) && (horseInfo.grown == TRUE) && (entities[horseInfo.spriteIndex].unk_58 == 0)) {
        
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

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009A2D0);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009A398);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009A400);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009A53C);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009A810);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009A97C);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009AAC8);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009AC54);

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

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009AFB4);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009B068);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009B11C);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009B1BC);

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

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009B914);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009BA74);

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

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009BBAC);

INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009BC44);

// FIXME: use dialogue box struct
// u8 func_8009BC44(void) {
//     return D_801886CC;
// }

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

// jtbl_80120EA8
//INCLUDE_ASM("asm/nonmatchings/game/animals", func_8009BCC4);

// show expression bubble when interacting with animals
void func_8009BCC4(u8 animalType, u8 animalIndex, u8 arg2) {

    // Entity index 0x30 = globalSprite index 0x1E
    // tools, holdable items, expression bubbles, field items

    deactivateEntity(0x30);
    loadEntity(0x30, 0x5B, 1);
    func_8002FF38(0x30, 0);

    switch (animalType) {

        case DOG_TYPE:
            setEntityTrackingTarget(0x30, dogInfo.spriteIndex, 0xFF);
            break;

        case HORSE_TYPE:
            setEntityTrackingTarget(0x30, horseInfo.spriteIndex, 0xFF);
            break;

        case COW_TYPE:
            setEntityTrackingTarget(0x30, gFarmAnimals[animalIndex].spriteIndex, 0xFF);
            break;
        
        case SHEEP_TYPE:
            setEntityTrackingTarget(0x30, gFarmAnimals[animalIndex].spriteIndex, 0xFF);
            break;

        case CHICKEN_TYPE:
            setEntityTrackingTarget(0x30, gChickens[animalIndex].spriteIndex, 0xFF);
            break;

        default:
            break;
        
    }

    func_8003019C(0x30, 0);
    func_80030054(0x30, 0);
    func_80030240(0x30, 0);
    func_800302E4(0x30, 0);

    setEntityColor(0x30, mainMap[MAIN_MAP_INDEX].mapFloats.groundRgba.r, mainMap[MAIN_MAP_INDEX].mapFloats.groundRgba.g, mainMap[MAIN_MAP_INDEX].mapFloats.groundRgba.b, mainMap[MAIN_MAP_INDEX].mapFloats.groundRgba.a);
    func_8002EDF0(0x30, 0, 0x30, 0);
    setEntityAnimation(0x30, arg2);
    
}