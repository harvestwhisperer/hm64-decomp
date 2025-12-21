#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "common.h"

#define MAX_FATIGUE_POINTS 100
#define MAX_HAPPINESS 255
#define MAX_STAMINA 250
#define MAX_RUCKSACK_SLOTS 9
#define MAX_TOOL_SLOTS_RUCKSACK 9
#define MAX_TOOLBOX_SLOTS 8 * 4

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
#define WHISTLE_FOR_DOG 18
#define WHISTLE_FOR_HORSE 19
#define DRINKING 20
#define PUTTING_ITEM_IN_RUCKSACK 21
#define CASTING_FISHING_ROD 22

// 0x80189060
typedef struct {
	u8 currentStamina; // 0
	u8 name[6]; // 1-7
	u16 toolHeldCounter; // 0x8
	u16 toolUseCounters[5]; // 0xA
    u8 currentToolLevel; // 0x14
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
	f32 velocity; // 0x60
	u16 actionHandler; // 0x64
	u16 animationHandler; // 0x66
	u16 actionPhaseFrameCounter; // 0x68
	u8 actionPhase; // 0x6A 
	u8 shopItemIndex; // 0x6B 
	u8 heldAnimalIndex; // 0x6C
	u8 direction; // 0x6D
	u8 unk_6E; // 0x6E
	u8 actionTimer; // 0x6F
	u8 groundObjectIndex;
    u8 fatigueCounter;
    u8 staminaExhaustionLevel;
    u8 fatigueThreshold;
	u8 fishingSpotType;
	u8 unused;
    u16 flags; // 0x76
} Player;

extern void setupPlayerEntity(u16 arg0, u8 resetPlayer);

extern void func_80065AA0();
extern u8 func_80065BCC(u8 tool);
extern void func_80065F5C();   
extern Vec3f getOffsetTileCoordinates(f32 arg1, u8 arg2);
extern void func_8006623C();  
extern void handlePlayerAnimation();   

extern void setPlayerAction(u16, u16);

extern u8 acquireKeyItem(u8);      
extern u8 checkHaveKeyItem(u8);  
extern u8 removeKeyItem(u8);    
extern u8 checkHaveTool(u8); 
extern u8 removeTool(u8 tool);

extern u8 checkFatigueLevel(); 

extern Player gPlayer;

extern u8 gAlcoholTolerance;
extern u8 gHappiness;
extern u8 gMaximumStamina;
extern u8 gPlayerBirthdaySeason;

extern u8 gToolboxSlots[];

// counter until napping animation
extern u16 playerIdleCounter;
extern u16 gSickDays;

#endif