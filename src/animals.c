#include "common.h"

#include "animals.h"

#include "system/graphic.h"
#include "system/mathUtils.h"
#include "system/sprite.h"
#include "system/mapContext.h"

#include "game.h"
#include "gameStatus.h"
#include "level.h"

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
extern Vec3f D_80114A50[];
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
void func_80088718(u8); 
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


INCLUDE_ASM(const s32, "animals", func_80085F70);

INCLUDE_ASM(const s32, "animals", func_800861A4);

//INCLUDE_ASM(const s32, "animals", func_8008634C);

void func_8008634C(s8 arg0) {

    u8 i;

    adjustDogAffection(arg0);
    adjustHorseAffection(arg0);

    for (i = 0; i < 8; i++) {
        func_80086458(i, arg0);
    }
    
}

//INCLUDE_ASM(const s32, "animals", adjustDogAffection);

void adjustDogAffection(s8 arg0) {
    if (dogInfo.flags & 1) {
        dogInfo.affection += adjustValue(dogInfo.affection, arg0, 0xFF);
    }
}

//INCLUDE_ASM(const s32, "animals", adjustHorseAffection);

void adjustHorseAffection(s8 arg0) {
    if (horseInfo.flags & 1) {
        horseInfo.affection += adjustValue(horseInfo.affection, arg0, 0xFF);
    }
}

// jtbl_80120320
INCLUDE_ASM(const s32, "animals", func_80086458);

//INCLUDE_ASM(const s32, "animals", func_8008662C);

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

//INCLUDE_ASM(const s32, "animals", func_800866E0);

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

INCLUDE_ASM(const s32, "animals", func_80086764);

//INCLUDE_ASM(const s32, "animals", func_800876D0);

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

//INCLUDE_ASM(const s32, "animals", func_8008779C);

void func_8008779C(void) {

    u8 i;

    if (dogInfo.flags & 4) {
        if (renderedSprites[dogInfo.spriteIndex].flags & 8) {
            func_8002FA2C(dogInfo.spriteIndex);
        }
    }

    for (i = 0; i < MAX_CHICKENS; i++) {

        if (gChickens[i].flags & 4) {

            if (renderedSprites[gChickens[i].spriteIndex].flags & 8) {
                func_8002FA2C(gChickens[i].spriteIndex);
            }

        }
    }

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {
        if (gFarmAnimals[i].flags & 4) {
            if (renderedSprites[gFarmAnimals[i].spriteIndex].flags & 8) {
                func_8002FA2C(gFarmAnimals[i].spriteIndex);
            }
        }
    }

    if (horseInfo.flags & 4) {
        if (renderedSprites[horseInfo.spriteIndex].flags & 8) {
            func_8002FA2C(horseInfo.spriteIndex);
        }
    }

    for (i = 0; i < 7; i++) {
        if (gChickenEggs[i].flags & 4) {
            if (renderedSprites[gChickenEggs[i].spriteIndex].flags & 8) {
                func_8002FA2C(gChickenEggs[i].spriteIndex);
            }
        }
    }
    
}

INCLUDE_ASM(const s32, "animals", func_800879C8);

//INCLUDE_ASM(const s32, "animals", func_80087CD4);

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

//INCLUDE_ASM(const s32, "animals", func_80087D5C);

void func_80087D5C(void) {

    u8 i;

    for (i = 0; i < MAX_CHICKENS; i++) {
        gChickens[i].flags |= 0x10;
    }

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {
        gFarmAnimals[i].flags |= (8 | 0x10);
    }
    
}

//INCLUDE_ASM(const s32, "animals", func_80087DEC);

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


INCLUDE_ASM(const s32, "animals", func_80087F28);

//INCLUDE_ASM(const s32, "animals", initializeChicken);

void initializeChicken(u8 chickenIndex) {

    gChickens[chickenIndex].location = 0;
    
    gChickens[chickenIndex].unk_17 = 0;
    gChickens[chickenIndex].unk_18 = 0;
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

//INCLUDE_ASM(const s32, "animals", func_8008820C);

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
        func_80088718(index);

        gFarmAnimals[index].birthdaySeason = gSeason;
        gFarmAnimals[index].flags = 1;
        gFarmAnimals[index].birthdayDayOfMonth = gDayOfMonth;
        gFarmAnimals[index].goldenMilk = 0xFF;
        gFarmAnimals[index].affection = 0;

        if (arg0 == 0) {

            gFarmAnimals[index].unk_23 = gFarmAnimals[arg1].name[0];
            gFarmAnimals[index].unk_24 = gFarmAnimals[arg1].name[1];
            gFarmAnimals[index].unk_25 = gFarmAnimals[arg1].name[2];
            gFarmAnimals[index].unk_26 = gFarmAnimals[arg1].name[3];
            gFarmAnimals[index].unk_27 = gFarmAnimals[arg1].name[4];
            gFarmAnimals[index].unk_28 = gFarmAnimals[arg1].name[5];
            
        } else {

            gFarmAnimals[index].unk_23 = 0xF6;
            gFarmAnimals[index].unk_24 = 0xF6;
            gFarmAnimals[index].unk_25 = 0xF6;
            gFarmAnimals[index].unk_26 = 0xF6;
            gFarmAnimals[index].unk_27 = 0xF6;
            gFarmAnimals[index].unk_28 = 0xF6;
            
        }
        
    }

    return index;
    
}

INCLUDE_ASM(const s32, "animals", func_8008841C);

//INCLUDE_ASM(const s32, "animals", func_800886D0);

// initialize watched cows from Doug
void func_800886D0(void) {
    initializeFarmAnimal(D_8016FAF8);
    initializeFarmAnimal(D_801C3F46);
    initializeFarmAnimal(D_8016F898);
}

//INCLUDE_ASM(const s32, "animals", func_80088718);

// set farm animal locations
void func_80088718(u8 animalIndex) {

    gFarmAnimals[animalIndex].location = BARN;

    if (gFarmAnimals[animalIndex].type == 3) {
        
        gFarmAnimals[animalIndex].coordinates.x = D_80114AB0.x;
        gFarmAnimals[animalIndex].coordinates.y = D_80114AB0.y;
        gFarmAnimals[animalIndex].coordinates.z = D_80114AB0.z;
        gFarmAnimals[animalIndex].unk_1C = 4;

    } else {

        gFarmAnimals[animalIndex].coordinates.x = D_80114A50[animalIndex].x;
        gFarmAnimals[animalIndex].coordinates.y = D_80114A50[animalIndex].y;
        gFarmAnimals[animalIndex].coordinates.z = D_80114A50[animalIndex].z;
        
        if (animalIndex >= 4) {
            gFarmAnimals[animalIndex].unk_1C = 6;
        } else {
            gFarmAnimals[animalIndex].unk_1C = 2;
        }
    }
}

//INCLUDE_ASM(const s32, "animals", initializeFarmAnimal);

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

//INCLUDE_ASM(const s32, "animals", initializeDog);

void initializeDog(void) {

    dogInfo.location = FARM;
    dogInfo.coordinates.y = 0;
    dogInfo.unk_17 = 0;
    dogInfo.unk_18 = 0;
    dogInfo.affection = 0; 
    dogInfo.unk_19 = 0;
    dogInfo.unk_1A = 0;
    dogInfo.unk_1B = 0;
    dogInfo.flags = 1;
    dogInfo.coordinates.x = -432.0f;
    dogInfo.coordinates.z = 96.0f;
    
}

//INCLUDE_ASM(const s32, "animals", initializeHorse);

void initializeHorse(void) {
    horseInfo.location = FARM;
    horseInfo.coordinates.y = 0;
    horseInfo.unk_17 = 0;
    horseInfo.direction = 0;
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

//INCLUDE_ASM(const s32, "animals", setAnimalLocations);

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

//INCLUDE_ASM(const s32, "animals", setDogLocation);

void setDogLocation(u8 mapIndex) {

    if (dogInfo.flags & 1 && (mapIndex == 0xFF || dogInfo.location == mapIndex)) {
        dogInfo.location = FARM;
        dogInfo.coordinates.y = 0.0f;
        dogInfo.unk_18 = 0;
        dogInfo.flags &= ~0x8;
        dogInfo.coordinates.x = -432.0f;
        dogInfo.coordinates.z = 96.0f;
    }
}

//INCLUDE_ASM(const s32, "animals", setHorseLocation);

void setHorseLocation(u8 mapIndex) {

    if (horseInfo.flags & 1 && (mapIndex == 0xFF || horseInfo.location == mapIndex)) {
        horseInfo.coordinates.y = 0;
        horseInfo.location = FARM;
        horseInfo.direction = 0;
        horseInfo.coordinates.x = -240.0f;
        horseInfo.coordinates.z = -384.0f;
    }
}

//INCLUDE_ASM(const s32, "animals", func_80088BB0);

void func_80088BB0(u8 mapIndex, u8 animalIndex) {

    if ((gFarmAnimals[animalIndex].flags & 1) && (mapIndex == 0xFF ||  gFarmAnimals[animalIndex].location == mapIndex)) {
        func_80088718(animalIndex);
    }
    
}

//INCLUDE_ASM(const s32, "animals", func_80088C1C);

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

INCLUDE_ASM(const s32, "animals", func_80088D54);

INCLUDE_ASM(const s32, "animals", func_8008A4A8);

INCLUDE_ASM(const s32, "animals", func_8008A5F0);

INCLUDE_ASM(const s32, "animals", func_8008A650);

INCLUDE_ASM(const s32, "animals", func_8008A9E8);

INCLUDE_ASM(const s32, "animals", func_8008B150);

INCLUDE_ASM(const s32, "animals", func_8008B1B8);

INCLUDE_ASM(const s32, "animals", func_8008B2E8);

INCLUDE_ASM(const s32, "animals", func_8008B55C);

INCLUDE_ASM(const s32, "animals", func_8008B9AC);

INCLUDE_ASM(const s32, "animals", func_8008BAF0);

//INCLUDE_ASM(const s32, "animals", func_8008C208);

void func_8008C208(void) {

    Vec3f vec;

    if ((dogInfo.flags & 1) && (dogInfo.flags & 4)) {
        
        if (func_8002FECC(dogInfo.spriteIndex) || dogInfo.flags & 0x20) {

            if (dogInfo.unk_1A == 0) {
                func_8008C358();
            } else {
                dogInfo.unk_1A--;
            }
            
            dogInfo.flags &= ~0x20;
            
        }
        
        setSpriteDirection(dogInfo.spriteIndex, (dogInfo.unk_18 + 8 - func_8003C1A4(MAIN_MAP_INDEX)) % 8);
        func_80028520(&vec, dogInfo.unk_19, dogInfo.unk_18, 0);
        func_8002FE10(dogInfo.spriteIndex, vec.x, vec.y, vec.z, dogInfo.unk_19);
    } 
    
}

INCLUDE_ASM(const s32, "animals", func_8008C358);

INCLUDE_ASM(const s32, "animals", func_8008CF94);

INCLUDE_ASM(const s32, "animals", func_8008D1C0);

INCLUDE_ASM(const s32, "animals", func_8008D650);

INCLUDE_ASM(const s32, "animals", func_8008D70C);

INCLUDE_ASM(const s32, "animals", func_8008DA00);

INCLUDE_ASM(const s32, "animals", func_8008DAA0);

INCLUDE_ASM(const s32, "animals", func_8008DF9C);

INCLUDE_ASM(const s32, "animals", func_8008E98C);

INCLUDE_ASM(const s32, "animals", func_8008F37C);

INCLUDE_ASM(const s32, "animals", func_8008FC68);

INCLUDE_ASM(const s32, "animals", func_800904E4);

INCLUDE_ASM(const s32, "animals", func_80090DF8);

INCLUDE_ASM(const s32, "animals", func_8009170C);

INCLUDE_ASM(const s32, "animals", func_80092094);

INCLUDE_ASM(const s32, "animals", func_80092808);

INCLUDE_ASM(const s32, "animals", func_80092A64);

INCLUDE_ASM(const s32, "animals", func_800931D8);

INCLUDE_ASM(const s32, "animals", func_80093434);

//INCLUDE_ASM(const s32, "animals", func_80093B80);

void func_80093B80(void) {

    Vec3f vec;

    if ((horseInfo.flags & 1) && (horseInfo.flags & 4)) {

        if (func_8002FECC(horseInfo.spriteIndex) || horseInfo.flags & 0x20) {
            
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
        
        setSpriteDirection(horseInfo.spriteIndex, (horseInfo.direction + 8 - func_8003C1A4(MAIN_MAP_INDEX)) % 8);
        func_80028520(&vec, horseInfo.unk_19, horseInfo.direction, 0);
        func_8002FE10(horseInfo.spriteIndex, vec.x, vec.y, vec.z, horseInfo.unk_19);

    } 
}

INCLUDE_ASM(const s32, "animals", func_80093CE0);

INCLUDE_ASM(const s32, "animals", func_8009427C);

INCLUDE_ASM(const s32, "animals", func_8009476C);

INCLUDE_ASM(const s32, "animals", func_80094A5C);

INCLUDE_ASM(const s32, "animals", func_800958EC);

INCLUDE_ASM(const s32, "animals", func_80096264);

INCLUDE_ASM(const s32, "animals", func_800967E0);

INCLUDE_ASM(const s32, "animals", func_80096978);

INCLUDE_ASM(const s32, "animals", func_80096DFC);

INCLUDE_ASM(const s32, "animals", func_800973F8);

INCLUDE_ASM(const s32, "animals", func_80097804);

// jtbl_80120E20
INCLUDE_ASM(const s32, "animals", func_80097CB0);

INCLUDE_ASM(const s32, "animals", func_80098188);

INCLUDE_ASM(const s32, "animals", func_80098740);

INCLUDE_ASM(const s32, "animals", func_80098B3C);

INCLUDE_ASM(const s32, "animals", func_80098F24);

INCLUDE_ASM(const s32, "animals", func_800991C0);

INCLUDE_ASM(const s32, "animals", func_80099424);

INCLUDE_ASM(const s32, "animals", func_80099548);

INCLUDE_ASM(const s32, "animals", func_800995F8);

INCLUDE_ASM(const s32, "animals", func_80099804);

INCLUDE_ASM(const s32, "animals", func_800999B0);

INCLUDE_ASM(const s32, "animals", func_80099B5C);

INCLUDE_ASM(const s32, "animals", func_80099C94);

INCLUDE_ASM(const s32, "animals", func_80099DE8);

INCLUDE_ASM(const s32, "animals", func_80099EEC);

INCLUDE_ASM(const s32, "animals", func_80099FF0);

INCLUDE_ASM(const s32, "animals", func_8009A074);

INCLUDE_ASM(const s32, "animals", func_8009A100);

INCLUDE_ASM(const s32, "animals", func_8009A17C);

INCLUDE_ASM(const s32, "animals", func_8009A2D0);

INCLUDE_ASM(const s32, "animals", func_8009A398);

INCLUDE_ASM(const s32, "animals", func_8009A400);

INCLUDE_ASM(const s32, "animals", func_8009A53C);

INCLUDE_ASM(const s32, "animals", func_8009A810);

INCLUDE_ASM(const s32, "animals", func_8009A97C);

INCLUDE_ASM(const s32, "animals", func_8009AAC8);

INCLUDE_ASM(const s32, "animals", func_8009AC54);

INCLUDE_ASM(const s32, "animals", func_8009AE7C);

INCLUDE_ASM(const s32, "animals", func_8009AFB4);

INCLUDE_ASM(const s32, "animals", func_8009B068);

INCLUDE_ASM(const s32, "animals", func_8009B11C);

INCLUDE_ASM(const s32, "animals", func_8009B1BC);

//INCLUDE_ASM(const s32, "animals", func_8009B25C);

void func_8009B25C(void) {

    u8 i;

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {

        if (gFarmAnimals[i].flags & 4) {
            gFarmAnimals[i].flags |= (0x200 | 0x2000);
            gFarmAnimals[i].unk_1B = 0;
        }
        
    }
    
}


INCLUDE_ASM(const s32, "animals", func_8009B2BC);

INCLUDE_ASM(const s32, "animals", func_8009B320);

INCLUDE_ASM(const s32, "animals", func_8009B374);

INCLUDE_ASM(const s32, "animals", func_8009B3DC);

INCLUDE_ASM(const s32, "animals", func_8009B464);

INCLUDE_ASM(const s32, "animals", func_8009B4EC);

INCLUDE_ASM(const s32, "animals", func_8009B564);

INCLUDE_ASM(const s32, "animals", func_8009B5E0);

INCLUDE_ASM(const s32, "animals", func_8009B658);

INCLUDE_ASM(const s32, "animals", func_8009B6B8);

INCLUDE_ASM(const s32, "animals", func_8009B7BC);

INCLUDE_ASM(const s32, "animals", func_8009B828);

INCLUDE_ASM(const s32, "animals", func_8009B914);

INCLUDE_ASM(const s32, "animals", func_8009BA74);

//INCLUDE_ASM(const s32, "animals", func_8009BB70);

// same as func_80061690
void func_8009BB70(void) {
    D_801886D4[0] = 0xF6;
    D_801886D4[1] = 0xF6;
    D_801886D4[2] = 0xF6;
    D_801886D4[3] = 0xF6;
    D_801886D4[4] = 0xF6;
    D_801886D4[5] = 0xF6;
}

INCLUDE_ASM(const s32, "animals", func_8009BBAC);

INCLUDE_ASM(const s32, "animals", func_8009BC44);

// FIXME: use dialogue box struct
// u8 func_8009BC44(void) {
//     return D_801886CC;
// }

//INCLUDE_ASM(const s32, "animals", func_8009BC54);

u8 func_8009BC54(void) {
    return horseInfo.unk_1E;
}

//INCLUDE_ASM(const s32, "animals", func_8009BC64);

void func_8009BC64(void) {

    u8 i;

    for (i = 0; i < 7; i++) {
        gChickenEggs[i].unk_16 = getRandomNumberInRange(0, 3);
    }
    
}

// jtbl_80120EA8
INCLUDE_ASM(const s32, "animals", func_8009BCC4);
