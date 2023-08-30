#ifndef _NPC_H
#define _NPC_H

#include "common.h"

#define TOTAL_NPCS 48

typedef struct {
	/* 0x00*/  Vec3f startingCoordinates;
	/* 0x0C */ Vec3f currentCoordinates;
	/* 0x18 */ u16 spriteIndex;
	/* 0x1A */ u8 unk_1A;
	/* 0x1B */ u8 unk_1B;
	/* 0x1C */ u8 levelIndex;
	/* 0x1D */ u8 movingFlag;
	/* 0x1E */ u8 unk_1E;
	/* 0x1F */ u8 direction;
	/* 0x20 */ u8 unk_20;
	/* 0x21 */ u8 unk_21; // possibly animation flags
	/* 0x22 */ u8 unk_22;
	/* 0x23 */ u8 location;
	/* 0x24 */ u8 unk_24;
	/* 0x25 */ u8 unk_25;
	/* 0x26 */ u16 flags;
} npcInfo;


extern void func_800752C0();  
extern void func_80075A18(); 
extern void func_800758B8();                                  
extern void func_80075910();  
extern bool func_80085D48(int index, u16 arg1); 
extern void setNpcAnimations(); 
extern void setNPCSpawningLocations();                     

extern u8 npcAffection[TOTAL_NPCS];
extern npcInfo npcInfoArray[];

extern u8 D_801C3E18;


#endif