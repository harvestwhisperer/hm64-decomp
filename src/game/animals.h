#ifndef _ANIMALS_H_
#define _ANIMALS_H_

#include "common.h"

#define MAX_CHICKENS 12
#define MAX_MISC_ANIMALS 7
#define MAX_FARM_ANIMALS 8

#define CHICK 1
#define ADULT_CHICKEN 2

// TODO: finish adding animal types
#define CALF 1
#define CHICK 1
#define ADULT_COW 2
#define ADULT_CHICKEN 2
#define PREGNANT_COW 3
#define BABY_SHEEP 4
#define ADULT_SHEEP 5
#define SHEARED_SHEEP 6

#define DOG_TYPE 0
#define HORSE_TYPE 1
#define COW_TYPE 2
#define SHEEP_TYPE 3
#define CHICKEN_TYPE 4

/* flags */
#define CHICKEN_FED 0x10

// 0x801C3BF0
typedef struct {
	u8 name[6];
	Vec3f coordinates;
	u16 entityIndex;
	u8 location;
	u8 unk_17;
	u8 direction;
	u8 unk_19;
	u8 unk_1A;
	u8 unk_1B;
	u8 type;
	u8 condition;
	u8 unk_1E;
	u8 starvedCounter;
	u16 flags;
} Chicken;

// 0x80170280
typedef struct {
	u8 affection;
	u8 name[6];
	Vec3f coordinates;
	f32 unk_14;
	u16 entityIndex;
	u8 location;
	u8 unk_1B;
	u8 direction;
	u8 unk_1D;
	u8 unk_1E;
	u8 type; // 0x1F
	u8 condition; // 0x20
	u8 age; // 0x21
	u8 conditionCounter; // 0x22
	u8 unk_23[6];
	u8 birthdaySeason; // 0x29
	u8 birthdayDayOfMonth; // 0x2A
	u8 goldenMilk; // 0x2B
	u16 flags; // 0x2C
} FarmAnimal;

// 0x801886B0
typedef struct {
	u8 affection; // 00
	u8 name[6]; // 01-07
	Vec3f coordinates; // 8
	u16 entityIndex; // 14
	u8 location; // 16
	u8 unk_17; // 17
	u8 direction; // 18
	u8 speed; // 19
	u8 unk_1A; // 1A
	u8 unk_1B; // 1B
	u8 unk_1C; // 1C
	u16 flags; // 1E
} Dog;

// 0x8016FDD0
typedef struct {
	u8 affection; // 00
	u8 name[6]; // 01
	Vec3f coordinates; // 08
	u16 entityIndex; // 14
	u8 location; // 16
	u8 unk_17; // 17
	u8 direction; // 18
	u8 speed; // 19
	u8 unk_1A; // 1A
	u8 unk_1B; // 1B
	u8 grown; // 1C
	u8 age; // 1D
	u8 unk_1E; // 1E
	u8 unk_1F; //1F
	u16 flags; // 20
} Horse;

// 0x8016FB08
typedef struct {
	Vec3f coordinates; // 08
	u16 entityIndex; // 14
	u8 mapIndex; // 16
	u8 unk_F; // 17
	u8 direction; // 18
	u8 zDisplacement; // 19
	u8 yDisplacement; // 1A
	u8 unk_13; // 1B
	u8 unk_14; // 1C
	u8 unk_15; // 1D
	u8 unk_16; // 1E
	u8 unk_17; // 1F
	u16 flags; // 20
} MiscAnimal;

extern bool func_80086764();
extern void func_800876D0();
extern void updateAnimalCoordinates();
extern void updateAnimals();
extern void func_80087D5C();
extern void resetAnimalStatuses();
extern u8 func_80087F28(u8, u8);
extern void setMrsManaCowsNames();
extern void func_800886D0();
extern void func_80088C1C(u8, u8);
extern u8 func_8008A4A8(u8, u8, f32, f32, f32);
extern void func_8008B1B8();
extern void func_8008B2E8(u8);
extern void func_8008B9AC();
extern void func_8008BAF0(u8, u8);
extern void func_80088D54();
extern void func_80099DE8();
extern void func_80099EEC();
extern void func_80099FF0();
extern bool func_8009A398(void);
extern bool func_8009A074();
extern bool func_8009A400();
extern u8 func_8009A810();
extern bool func_8009A100();
extern bool func_8009A17C();
extern bool func_8009A2D0();
extern bool func_8009A53C();
extern bool func_8009A97C();
extern void func_8009AAC8();
extern void func_8009B11C();
extern void func_8009B1BC(void);
extern void func_8009B25C();
extern u8 func_8009B2BC(u8);
extern u8 func_8009B320();
extern u8 func_8009B374();
extern u8 func_8009B3DC();
extern u8 func_8009B464();
extern u8 func_8009B4EC();
extern u8 func_8009B564();
extern u8 func_8009B5E0();
extern u8 func_8009B658();
extern u8 func_8009B7BC();
extern u8 func_8009B828(u8);
extern void func_8009BA74(u8);
extern u8 func_8009BBAC(void);
extern u8 func_8009BC44(void);
extern u8 func_8009BC54(void);
extern void func_8009BC64();

extern void initializeDog(void);
extern void initializeHorse(void);

extern void adjustDogAffection(s8 value);
extern void adjustHorseAffection(s8 value);

extern void setAnimalLocations(u8);
extern void setDogLocation(u8);
extern void setHorseLocation(u8);

extern MiscAnimal gMiscAnimals[MAX_MISC_ANIMALS];
extern Chicken gChickens[MAX_CHICKENS];
extern FarmAnimal gFarmAnimals[MAX_FARM_ANIMALS];
extern u8 D_8016FBCC[1];
extern u8 D_801886D4[6];
// TODO: label
// dead animal name for funeral
extern u8 deadAnimalName[6];
extern Dog dogInfo;
extern Horse horseInfo;

// watched cow indexes
extern u8 mrsManaCow3Index;
extern u8 mrsManaCow1Index;
extern u8 mrsManaCow2Index;

extern u16 D_8016FDF0;
extern u8 D_8016FFE8;
// newest farm animal index?
extern u8 D_80170464;
// cow festival stall
extern u8 D_80189054;
extern u8 D_801FC155;

// dead animal count
// TODO: label
extern u16 D_8013DC2E;

// purchased animal type
extern u8 D_801C4216;

#endif