#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "common.h"

#define PLAYER 0

/* tools */
#define SICKLE 1
#define HOE 2
#define AX 3
#define HAMMER 4
#define WATERING_CAN 5
#define MILKER 6
#define BELL 7
#define BRUSH 8
#define CLIPPERS 9
#define TURNIP_SEEDS 10
#define POTATO_SEEDS 11
#define CABBAGE_SEEDS 12
#define TOMATO_SEEDS 13
#define CORN_SEEDS 14
#define EGGPLANT_SEEDS 15
#define STRAWBERRY_SEEDS 16
#define MOON_DROP_SEEDS 17
#define PINK_CAT_MINT_SEEDS 18
#define BLUE_MIST_SEEDS 19
#define BIRD_FEED 20
#define BALL 21
#define FEEDING_BOTTLE 22
#define FISHING_POLE 24
#define MIRACLE_POTION 25
#define COW_MEDICINE 26
#define GRASS_SEEDS 27
#define BLUE_FEATHER 28
#define EMPTY_BOTTLE 29

/* special items */
#define OCARINA 1
#define FLOWER_SHOP_CARD 2
#define BAKERY_CARD 3
#define BROKEN_MUSIC_BOX 4
#define FIXED_MUSIC_BOX 5
#define DOOR_TO_HEAVEN 6
#define WEATHER_VANE 7
#define GOLD_PENDANT 8
#define LIBRARY_BOOK 9
#define TREASURE_MAP 10
#define MARBLES 11
#define GOODLUCK_CHARM 12
#define MEDAL_BAG 13
#define HORSE_RACE_TICKET 14
#define DOG_RACE_TICKET 15
#define POTPURRI 16
#define EMBROIDERED_HANDKERCHIEF 17
#define HAND_KNIT_SOCKS 18
#define LUCKY_BRACELET 19
#define FLOWER_BATH_CRYSTALS 20
#define STAMINA_CARROT 21

/* item indices */
#define TOMATO 0x10

/* actions */
#define TOOL_USE 1
#define THROWING 2
#define PICKING_UP 4
#define PUTTING_DOWN 5
#define EATING 6
#define ABOUT_TO_DRINK 12
#define WHISTLING 19
#define DRINKING 20
#define CASTING_FISHING_ROD 22

#define TOTAL_ITEMS 67
#define MAX_RUCKSACK_SLOTS 9
#define MAX_TOOL_SLOTS_RUCKSACK 9
#define MAX_TOOLCHEST_SLOTS 0x20
#define MAX_KEY_ITEMS 24 

typedef struct {
	u8 currentStamina; // 0
	u8 name[6]; // 1-7
	u16 toolHeldCounter; // 0x8
	u16 toolUseCounters[5]; // 0xA
    u8 staminaLevelForCurrentToolUse; // 0x14
    u8 toolSlots[8]; // 0x15
    u8 currentTool; // 0x1D
    u8 bottleContents; // 0x1E
    u8 toolLevels[5];
    u8 belongingsSlots[8];
    u8 unk_2C;
    u8 unk_2D;
    u8 keyItemSlots[24];
    u8 unk_46;
    u8 unk_47;
	Vec3f startingCoordinates; // 0x48
	Vec3f currentCoordinates; // 0x54
	// might be velocity
	f32 unk_60; // 0x60
	u16 action1; // 0x64
	u16 action2; // 0x66
	u16 action3; // 0x68
	u8 action4; // 0x6A
	u8 unk_6B; // 0x6B
	u8 unk_6C; // 0x6C
	u8 direction; // 0x6D
	u8 unk_6E; // 0x6E
	u8 unk_6F; // 0x6F
	u8 unk_70;
	u8 fatigue[4]; // 71
    u16 flags; // 0x78
} Player;

extern void setupPlayerSprite(u16 arg0, u8 resetPlayer);

extern void func_80065AA0();
extern u8 func_80065BCC(u8 tool);
extern void func_80065F5C();   
extern Vec3f* func_80065F94(Vec3f *arg0, f32 arg1, u8 arg2);
extern void func_8006623C();  
extern void func_8006A2E8();   

extern void setPlayerAction(u16, u16);

extern u8 acquireKeyItem(u8);      
extern u8 checkHaveKeyItem(u8);  
extern u8 removeKeyItem(u8);    
extern u8 checkHaveTool(u8); 

extern u8 checkFatigueLevel(); 

extern Player gPlayer;

extern u8 gAlcoholTolerance;
extern u8 gHappiness;
extern u8 gMaximumStamina;
extern u8 gPlayerBirthdaySeason;

extern u8 gToolchestSlots[];

// seed counters
// turnip
extern u8 D_801FC154;
// potato
extern u8 D_80204DF4;
// eggplant
extern u8 D_80237458;
// cabbage
extern u8 D_8018A725;
// strawberry
extern u8 D_802373E8;
// tomato
extern u8 D_8013DC2C;
// corn
extern u8 D_801FAD91;
// grass seeds
extern u8 D_801C3E28;
// moondrop flower
extern u8 D_801C3F70;
// pink cat mint
extern u8 D_80205636;
// blue mist
extern u8 D_802373A8;

// watering can uses
extern u8 D_8016FBCD;
extern u16 D_80237410;

// counter until napping animation
extern u16 D_802226E0;
// likely player sick days
extern u16 D_8016F8F4;

#endif