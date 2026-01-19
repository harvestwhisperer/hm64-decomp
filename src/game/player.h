#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "common.h"

#define MAX_FATIGUE_POINTS 100
#define MAX_HAPPINESS 255
#define MAX_STAMINA 250
#define MAX_RUCKSACK_SLOTS 9
#define MAX_TOOL_SLOTS_RUCKSACK 9
#define MAX_TOOLBOX_SLOTS 8 * 4

/* actions */
#define CONTROLLER_INPUT 0
#define TOOL_USE 1
#define DISCARDING_ITEM 2
#define SHIPPING_ITEM 3
#define PICKING_UP_ITEM 4
#define PLACING_ITEM 5
#define EATING 6
#define STAMINA_EXHAUSTION 7
#define GETTING_INTO_BED 8
#define IDLE_NAP 9
#define WATCHING_TV 10
#define SHOPPING 11
#define JUMPING 12
#define MOUNTING_HORSE 13
#define DISMOUNTING_HORSE 14
#define WALKING_DOWN_STAIRS 15
#define USING_TOILET 16
#define BATHING 17
#define WHISTLING_FOR_DOG 18
#define WHISTLING_FOR_HORSE 19
#define DRINKING 20
#define STORING_ITEM_IN_RUCKSACK 21
#define FISHING 22
#define CHANGE_TOOL 23
// 24 unused
#define FATIGUE_THRESHOLD 25
#define CLIMBING_TREE 26
#define ACQUIRING_TOOL 27
#define JUMPING_DOWN 28
#define USING_HOT_SPRINGS 29
#define DIALOGUE_SELECTING 30
#define PUTTING_FOOD_IN_DOG_BOWL 31
#define DROPPING_ITEM_IN_WATER 32
// 33 unused
#define ACQUIRING_MUSIC_BOX 34

/* animation handlers */
#define ANIM_DEFAULT 0
#define ANIM_WALKING 1
#define ANIM_RUNNING 2
#define ANIM_TOOL_USE 3
#define ANIM_THROWING 4
#define ANIM_SHIPPING 5
#define ANIM_PICKING_UP_ITEM 6
#define ANIM_PLACING_ITEM 7
#define ANIM_EATING 8
#define ANIM_STAMINA_EXHAUSTION 9
#define ANIM_GET_INTO_BED 10
#define ANIM_IDLE_NAP 11
#define ANIM_WATCHING_TV 12
#define ANIM_SHOPPING 13
#define ANIM_JUMPING 14
#define ANIM_MOUNT_HORSE 15
#define ANIM_DISMOUNT_HORSE 16
// 17, 18 unused
#define ANIM_WHISTLE_FOR_DOG 19
#define ANIM_WHISTLE_FOR_HORSE 20
#define ANIM_DRINKING 21
#define ANIM_HANDLE_ITEM_RUCKSACK 22
#define ANIM_FISHING 23
// 24 unused
// 25 unused
#define ANIM_FATIGUE_THRESHOLD 26
#define ANIM_CLIMBING_TREE 27
#define ANIM_TOOL_ACQUISITION 28
// 29, 30 unused
#define ANIM_DIALOGUE_SELECTING 31
#define ANIM_PUT_FOOD_IN_DOG_BOWL 32
#define ANIM_DROP_ITEM_IN_WATER 33
#define ANIM_DRINK_ALCOHOL 34
#define ANIM_ACQUIRE_MUSIC_BOX 35

#define PLAYER_RIDING_HORSE 0x1
#define PLAYER_BATH_POSE_1 0x2
#define PLAYER_BATH_POSE_2 0x4
#define PLAYER_BATH_POSE_3 0x8
#define PLAYER_HOT_SPRING_POSE_1 0x10
#define PLAYER_HOT_SPRING_POSE_2 0x20
#define PLAYER_HOT_SPRING_POSE_3 0x40

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
	u8 savedDirection; // 0x6E
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

extern void initializePlayerHeldItem();
extern u8 storeTool(u8 tool);
extern void syncPlayerCoordinatesFromEntity();   
extern Vec3f getOffsetTileCoordinates(f32 arg1, u8 arg2);
extern void updatePlayerAction();  
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