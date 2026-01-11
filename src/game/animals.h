#ifndef _ANIMALS_H_
#define _ANIMALS_H_

#include "common.h"

#define MAX_CHICKENS 12
#define MAX_MISC_ANIMALS 7
#define MAX_FARM_ANIMALS 8

#define CHICKEN_EGG 0
#define CHICK 1
#define ADULT_CHICKEN 2

#define BABY_COW 0
#define CALF 1
#define ADULT_COW 2
#define PREGNANT_COW 3
#define BABY_SHEEP 4
#define ADULT_SHEEP 5
#define SHEARED_SHEEP 6

#define DOG_TYPE 0
#define HORSE_TYPE 1
#define COW_TYPE 2
#define SHEEP_TYPE 3
#define CHICKEN_TYPE 4

#define COW_INFANCY_DURATION 14
#define COW_YOUTH_DURATION 21
#define COW_PREGNANCY_DURATION 21
#define SHEEP_YOUTH_DURATION 15
#define WOOL_REGROW_DURATION 7
#define CHICKEN_EGG_INCUBATION_DURATION 3
#define CHICK_DURATION 7
#define ANIMAL_DEATH_COUNT 7

#define CHICKEN_NORMAL 0
#define CHICKEN_STARVED 1
#define CHICKEN_DEAD 2

#define COW_NORMAL 0
#define COW_HAPPY 1
#define COW_MAD 2
#define COW_SICK 3
#define COW_DEAD 4

#define SHEEP_NORMAL 0
#define SHEEP_SICK 3
#define SHEEP_DEAD 4

#define MISC_ANIMAL_PLAYER_DOG 0
#define MISC_ANIMAL_NPC_DOG 1
#define MISC_ANIMAL_CAT 2
#define MISC_ANIMAL_HORSE 3
#define MISC_ANIMAL_HORSE_2 4
#define MISC_ANIMAL_COW 5
#define MISC_ANIMAL_SHEEP 6
#define MISC_ANIMAL_FOX 7
#define MISC_ANIMAL_BUNNY 8
#define MISC_ANIMAL_SQUIRREL 9
#define MISC_ANIMAL_MONKEY 10
#define MISC_ANIMAL_SPARROW 11
#define MISC_ANIMAL_BIRD 12
#define MISC_ANIMAL_CRAB 13
#define MISC_ANIMAL_SNAKE 14
#define MISC_ANIMAL_WHITE_BUTTERFLY 15
#define MISC_ANIMAL_LADYBUG 16
#define MISC_ANIMAL_CICADA 17
#define MISC_ANIMAL_HORNED_BEETLE 18
#define MISC_ANIMAL_STAG_BEETLE 19
#define MISC_ANIMAL_DRAGONFLY 20
#define MISC_ANIMAL_CRICKET 21

#define DOG_ACTIVE 				1
#define DOG_STATE_CHANGED 		2
#define DOG_ENTITY_LOADED 		4
#define DOG_HELD                8
#define DOG_FOLLOWING           0x10
#define DOG_COLLISION_WITH_PLAYER 0x20
#define DOG_HELD_DAILY          0x40
#define DOG_WHISTLED_FOR_DAILY  0x80

#define HORSE_ACTIVE 			1
#define HORSE_STATE_CHANGED		2
#define HORSE_ENTITY_LOADED 	4
#define HORSE_BEING_RODE 		8	
#define HORSE_FOLLOWING         0x10
#define HORSE_COLLISION_WITH_PLAYER 0x20
#define HORSE_BRUSHED_DAILY     0x100
#define HORSE_WHISTLED_DAILY    0x200
#define HORSE_RODE_DAILY        0x400
#define HORSE_TALKED_TO_DAILY   0x800

#define CHICKEN_ACTIVE 	     	1
#define CHICKEN_STATE_CHANGED   2
#define CHICKEN_ENTITY_LOADED   4
#define CHICKEN_HELD 			8
#define CHICKEN_FED             0x10
#define CHICKEN_EGG_INCUBATING  0x20
#define CHICKEN_COLLISION_WITH_PLAYER    0x40
// 0x80 rained on
#define CHICKEN_NEWBORN         0x100

#define FARM_ANIMAL_ACTIVE 		1
#define FARM_ANIMAL_STATE_CHANGED 2
#define FARM_ANIMAL_ENTITY_LOADED 4
#define FARM_ANIMAL_FED         0x8
#define FARM_ANIMAL_ATE_GRASS   0x10
#define FARM_ANIMAL_BRUSHED     0x20
#define FARM_ANIMAL_MILKED      0x40
#define FARM_ANIMAL_PREGNANT    0x80
#define FARM_ANIMAL_SHEARED     0x100
#define FARM_ANIMAL_APPROACHING 0x200
#define FARM_ANIMAL_LINGERING   0x400
#define FARM_ANIMAL_NEWBORN     0x800
#define FARM_ANIMAL_TALKED_TO 	0x1000
#define FARM_ANIMAL_FOLLOWING   0x2000
// 0x4000 rained on
#define FARM_ANIMAL_COLLISION_WITH_PLAYER 0x8000

#define MISC_ANIMAL_ACTIVE 1
#define MISC_ANIMAL_STATE_CHANGED 2
#define MISC_ANIMAL_ENTITY_LOADED 4
#define MISC_ANIMAL_RUNNING_AWAY 0x10


// 0x801C3BF0
typedef struct {
	u8 name[6];
	Vec3f coordinates;
	u16 entityIndex;
	u8 location;
	u8 actionState;
	u8 direction;
	u8 speed;
	u8 stateTimer;
	u8 unk_1B;
	u8 type;
	u8 condition;
	u8 typeCounter; // age or incubation counter
	u8 conditionCounter;
	u16 flags;
} Chicken;

// 0x80170280
typedef struct {
	u8 affection;
	u8 name[6];
	Vec3f coordinates;
	f32 speed;
	u16 entityIndex;
	u8 location;
	u8 actionState;
	u8 direction;
	u8 stateTimer;
	u8 unk_1E;
	u8 type;
	u8 condition;
	u8 typeCounter; // 0x21  overloaded field: age, pregnancy, sheared status
	u8 conditionCounter;
	u8 motherName[6];
	u8 birthdaySeason;
	u8 birthdayDayOfMonth;
	u8 milkType;
	u16 flags;
} FarmAnimal;

// 0x801886B0
typedef struct {
	u8 affection;
	u8 name[6];
	Vec3f coordinates;
	u16 entityIndex;
	u8 location;
	u8 actionState;
	u8 direction;
	u8 speed;
	u8 stateTimer;
	u8 unk_1B;
	u8 bestRacePlacement;
	u16 flags;
} Dog;

// 0x8016FDD0
typedef struct {
	u8 affection;
	u8 name[6];
	Vec3f coordinates;
	u16 entityIndex;
	u8 location;
	u8 actionState;
	u8 direction;
	u8 speed;
	u8 stateTimer;
	u8 unk_1B;
	u8 grown;
	u8 age;
	u8 bestRacePlacement;
	u8 unk_1F;
	u16 flags;
} Horse;

// birds, insects, dogs/cats
// 0x8016FB08
typedef struct {
	Vec3f coordinates;
	u16 entityIndex;
	u8 mapIndex;
	u8 actionState;
	u8 direction;
	u8 zDisplacement;
	u8 yDisplacement;
	u8 timer;
	u8 unk_14;
	u8 animalType;
	u8 spawnVariant;
	u8 unk_17;
	u16 flags;
} MiscAnimal;

typedef struct {
    u16 arr[3];
    u16 arr2[3];
    u16 arr3[3];
} SheepItemInfo;

extern u8 initializeNewFarmAnimal(u8 arg0, u8 arg1);
extern void adjustAllAnimalAffection(s8 amount);
extern void adjustFarmAnimalAffection(u8, s8); 
extern void setAnimalState(u8 animalType, u8 index, u8 type, u8 condition, u8 arg4);
extern bool handlePlayerAnimalInteraction();
extern void initializeAnimalEntities();
extern void initializeChicken(u8 chickenIndex);
extern void func_8008779C(void);
extern void updateAnimalCoordinates();
extern void updateAnimals();
extern void feedAllAnimals();
extern void resetAnimalStatuses();
extern u8 initializeNewChicken(u8, u8);
extern void initializeFarmAnimal(u8 animalIndex);
extern void setMrsManaCowsNames();
extern void initializeWatchedCows();
extern void resetChickenLocation(u8, u8);
extern u8 spawnMiscAnimal(u8, u8, f32, f32, f32);
extern void initializeDogEntity();
extern void initializeHorseEntity();
extern void initializeChickenEntity(u8);
extern void initializeFarmAnimalEntity(u8 index);
extern void initializeMiscAnimalEntity(u8, u8);
extern void spawnWildAnimals();
extern void handleFarmAnimalPlayerCollision();
extern void handleChickenPlayerCollision();
extern void handleDogPlayerCollision();
extern bool handleHorseShippingItem(void);
extern bool handleHorsePlayerInteraction();
extern bool handleAnimalMedicineUse();
extern u8 handleUseMiraclePotion();
extern bool handleHorseGrownPlayerInteraction();
extern bool handleBrushFarmAnimal();
extern bool handleBrushHorse();
extern bool handleMilkCow();
extern bool handleGetMilkWithBottle();
extern void handleShearSheep();
extern void handleWhistleForDog();
extern void handleWhistleForHorse(void);
extern void handleCallFarmAnimalsWithCowBell();
extern u8 getTotalFarmAnimalsByType(u8);
extern u8 getTotalPregnantFamAnimals();
extern u8 getTotalFarmAnimalsCount();
extern u8 getTotalCowsCount();
extern u8 getCowWithHighestAffection();
extern u8 getTotalSheepCount();
extern u8 getTotalChickenCount();
extern u8 getHealthyChickenCount();
extern u8 getChickenEggCount();
extern void handleHatchChicken();
extern u8 getIncubatingEggCount();
extern void deactivateAnimalEntities(void);
extern u8 func_8009B828(u8);
extern void generateMilkTypeString(u8);
extern u8 getBestCowMilkType(void);
extern u8 getDogBestRacePlacement(void);
extern u8 getHorseBestRacePlacement(void);
extern void randomizeMiscAnimalSpawnVariants();
extern bool handleHitDogWithTool(void);
extern bool handleHitHorseWithTool(void);
extern bool handleHitChickenWithTool(void);
extern bool handleHitFarmAnimalWithTool(void);

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
extern u8 gMilkTypeString[2];
extern u8 D_801886D4[6];
extern u8 gDeadAnimalName[6];
extern Dog dogInfo;
extern Horse horseInfo;

// watched cow indexes
extern u8 mrsManaCow3Index;
extern u8 mrsManaCow1Index;
extern u8 mrsManaCow2Index;

extern u16 D_8016FDF0;
extern u8 bornChickenIndex;
// newest farm animal index?
extern u8 bornAnimalIndex;
// cow festival stall
extern u8 gCowFestivalEnteredCowIndex;
extern u8 gSelectedAnimalIndex;

// dead animal count
// TODO: label
extern u16 deadAnimalCount;

// purchased animal type
extern u8 selectedAnimalType;

// animal price?
extern u32 gAnimalSalePrice;

#endif