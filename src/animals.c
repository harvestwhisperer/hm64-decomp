#include "common.h"

#include "animals.h"

// bss
extern Chicken gChickens[MAX_CHICKENS];
extern FarmAnimal gFarmAnimals[];
extern u8 D_801886D4[6];
extern Dog dogInfo;
extern Horse horseInfo;

extern u8 D_8016F898;
extern u8 D_8016FAF8;
extern u16 D_8016FDF0;
extern u8 D_8016FFE8;
extern u8 D_80170464;
extern u8 D_80189054;
extern u8 D_8018908C;
extern u8 D_801C3F46;
extern u8 D_801FC155;

INCLUDE_ASM(const s32, "animals", func_80085F70);

INCLUDE_ASM(const s32, "animals", func_800861A4);

INCLUDE_ASM(const s32, "animals", func_8008634C);

INCLUDE_ASM(const s32, "animals", adjustDogAffection);

INCLUDE_ASM(const s32, "animals", adjustHorseAffection);

// jtbl_80120320
INCLUDE_ASM(const s32, "animals", func_80086458);

INCLUDE_ASM(const s32, "animals", func_8008662C);

INCLUDE_ASM(const s32, "animals", func_800866E0);

INCLUDE_ASM(const s32, "animals", func_80086764);

INCLUDE_ASM(const s32, "animals", func_800876D0);

INCLUDE_ASM(const s32, "animals", func_8008779C);

INCLUDE_ASM(const s32, "animals", func_800879C8);

INCLUDE_ASM(const s32, "animals", func_80087CD4);

INCLUDE_ASM(const s32, "animals", func_80087D5C);

INCLUDE_ASM(const s32, "animals", func_80087DEC);

INCLUDE_ASM(const s32, "animals", func_80087F28);

INCLUDE_ASM(const s32, "animals", func_80088104);

INCLUDE_ASM(const s32, "animals", func_8008820C);

INCLUDE_ASM(const s32, "animals", func_8008841C);

INCLUDE_ASM(const s32, "animals", func_800886D0);

INCLUDE_ASM(const s32, "animals", func_80088718);

INCLUDE_ASM(const s32, "animals", func_80088810);

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
    horseInfo.location = 0x52;
    horseInfo.coordinates.y = 0;
    horseInfo.unk_17 = 0;
    horseInfo.unk_18 = 0;
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

    for (i = 0; i < 8; i++) {
        func_80088BB0(mapIndex, i);
    }

    for (i = 0; i < 0xC; i++) {
        func_80088C1C(mapIndex, i);
    }

}

//INCLUDE_ASM(const s32, "animals", setDogLocation);

void setDogLocation(u8 mapIndex) {

    if ((dogInfo.flags & 1) && ((mapIndex == 0xFF) || (dogInfo.location == mapIndex))) {
        dogInfo.location = 0x52;
        dogInfo.coordinates.y = 0.0f;
        dogInfo.unk_18 = 0;
        dogInfo.flags &= 0xFFF7;
        dogInfo.coordinates.x = -432.0f;
        dogInfo.coordinates.z = 96.0f;
    }
}

//INCLUDE_ASM(const s32, "animals", setHorseLocation);

void setHorseLocation(u8 mapIndex) {

    if ((horseInfo.flags & 1) && ((mapIndex == 0xFF) || (horseInfo.location == mapIndex))) {
        horseInfo.coordinates.y = 0;
        horseInfo.location = 0x52;
        horseInfo.unk_18 = 0;
        horseInfo.coordinates.x = -240.0f;
        horseInfo.coordinates.z = -384.0f;
    }
}

INCLUDE_ASM(const s32, "animals", func_80088BB0);

INCLUDE_ASM(const s32, "animals", func_80088C1C);

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

INCLUDE_ASM(const s32, "animals", func_8008C208);

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

INCLUDE_ASM(const s32, "animals", func_80093B80);

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

INCLUDE_ASM(const s32, "animals", func_8009B25C);

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

INCLUDE_ASM(const s32, "animals", func_8009BC54);

INCLUDE_ASM(const s32, "animals", func_8009BC64);

// jtbl_80120EA8
INCLUDE_ASM(const s32, "animals", func_8009BCC4);
