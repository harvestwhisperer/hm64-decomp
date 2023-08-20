#ifndef _ANIMALS_H_
#define _ANIMALS_H_

#include "common.h"
#include "system/sprite.h"

#define MAX_CHICKENS 12

typedef struct {
	char name[6];
	Vec3f coordinates;
	u16 unk_14;
	u8 location;
	u8 unk_17;
	u8 unk_18;
	u8 unk_19;
	u8 unk_1A;
	u8 unk_1B;
	u8 type;
	u8 condition;
	u8 unk_1E;
	u8 starvedCounter;
	u16 upkeep;
} Chicken;

typedef struct {
	u8 affection;
	char name[6];
	u32 unk_8;
	u32 unk_C;
	u32 unk_10;
	u32 unk_14;
	u16 unk_18;
	u8 location;
	u8 unk_1B;
	u8 unk_1C;
	u8 unk_1D;
	u8 unk_1E;
	u8 type;
	u8 condition;
	u8 age;
	u8 conditionCounter;
	u8 existence;
	u8 unk_24;
	u8 unk_25;
	u8 unk_26;
	u8 unk_27;
	u8 unk_28;
	u8 birthdaySeason;
	u8 birthdayDayOfMonth;
	u8 goldenMilk;
	u16 upkeep;
} FarmAnimal;

typedef struct {
	u8 affection; // 00
	char name[6]; // 01-07
	Vec3f coordinates; // 8
	u16 spriteInfo; // 14
	u8 location; // 16
	u8 unk_17; // 17
	u8 unk_18; // 18
	u8 unk_19; // 19
	u8 unk_1A; // 1A
	u8 unk_1B; // 1B
    u16 unk_1C; // 1C 
	u16 flags; // 1E
} Dog;

typedef struct {
	u8 affection; // 00
	char name[6]; // 01
	Vec3f coordinates; // 08 
	u16 unk_14; // 14
	u8 location; // 16
	u8 unk_17; // 17
	u8 unk_18; // 18
	u8 unk_19; // 19
	u8 unk_1A; // 1A
	u8 unk_1B; // 1B
	u8 grown; // 1C
	u8 age; // 1D
	u8 unk_1E; // 1E
	u8 unk_1F; //1F
	u16 flags; // 20
} Horse;

typedef struct {
    u8 unk_0;
    u8 unk_1;
    u8 unk_2;
    u8 unk_3;
    u8 unk_4;
    u8 unk_5; 
} AnimalLocations;

#endif