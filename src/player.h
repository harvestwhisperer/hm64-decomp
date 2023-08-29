#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "common.h"

typedef struct {
	u8 currentStamina; // 0
	char name[6]; // 1-7
    u8 padding;
	u8 fatigueCounter; // 8
    u32 unk_C;
    u32 unk_10;
    u8 unk_14;
    u8 toolSlots[8]; // 0x15
    u8 currentTool; // 0x1D
    u8 bottleContents; // 0x1E
    u8 unk_1F[5];
    u8 belongingsSlots[8];
    u8 unk_2C;
    u8 unk_2D;
    u8 keyItemSlots[24];
    u8 unk_46;
    u8 unk_47;
	Vec3f startingCoordinates; // 0x48
	Vec3f currentCoordinates; // 0x54
	u32 unk_60; // 0x60
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
	u8 fatigue[5]; // 71
    u16 flags; // 0x78
} Player;


extern void setupPlayerSprite(u16 arg0, u8 resetPlayer);

extern void func_80065AA0();
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

#endif