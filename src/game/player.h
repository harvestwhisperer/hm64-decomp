#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "common.h"

#define MAX_RUCKSACK_SLOTS 9
#define MAX_TOOL_SLOTS_RUCKSACK 9
#define MAX_TOOLCHEST_SLOTS 8 * 4
#define MAX_HAPPINESS 255
#define MAX_STAMINA 250

// TODO: finish adding actions
/* actions */
#define STANDING 0
#define TOOL_USE 1
#define THROWING 2
#define PICKING_UP 4
#define PUTTING_DOWN 5
#define EATING 6
#define CROUCHING 10
#define ABOUT_TO_DRINK 12
#define WHISTLING 19
#define DRINKING 20
#define CASTING_FISHING_ROD 22

typedef struct {
    u8 counter;
    u8 level;
    u8 unk_2;
    u8 unk_3;
    u8 unk_4;
} Fatigue;

// 0x80189060
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
    u8 heldItem;
    u8 itemInfoIndex;
    u8 keyItemSlots[24];
    u8 unk_46;
    u8 unk_47;
	Vec3f coordinates; // 0x48
	Vec3f movementVector; // 0x54
	f32 unk_60; // 0x60, likely speed/velocity
	u16 currentAction; // 0x64
	u16 nextAction; // 0x66
	u16 actionProgress; // 0x68
	u8 animationState; // 0x6A 
	u8 shopItemIndex; // 0x6B 
	u8 heldChickenIndex; // 0x6C
	u8 direction; // 0x6D
	u8 unk_6E; // 0x6E
	u8 actionTimer; // 0x6F
	u8 unk_70; // collision object id? 
	Fatigue fatigue; // 71
    u16 flags; // 0x76
} Player;

typedef struct {
    u8 unk_0;
    u8 unk_1;
    u8 unk_2;
    u8 unk_3;
    u8 unk_4;
    u8 unk_5;
    u8 unk_6;
    u8 unk_7;
    u8 unk_8;
    u8 unk_9;
    u8 unk_10;
    u8 unk_11;
    u8 unk_12;
    u8 unk_13;
    u8 unk_14;
} UnknownPlayerData;

extern void setupPlayerEntity(u16 arg0, u8 resetPlayer);

extern void handleEatingAndDrinking();
extern u8 func_80065BCC(u8 tool);
extern void func_80065F5C();   
extern Vec3f func_80065F94(f32 arg1, u8 arg2);
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

// TODO: labels
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
// chicken feed
extern u16 chickenFeedQuantity;

// counter until napping animation
extern u16 playerIdleCounter;
extern u16 gSickDays;

#endif