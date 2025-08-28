#ifndef _NPC_H
#define _NPC_H

#include "common.h"

#define TOTAL_NPCS 48

#define MARIA 0
#define POPURI 1
#define ELLI 2
#define ANN 3
#define KAREN 4
#define BABY 5
#define HARRIS 6
#define GRAY 7
#define JEFF 8
#define CLIFF 9
#define KAI 0xA
#define MAYOR 0xB
#define MAYOR_WIFE 0xC
#define LILLIA 0xD
#define BASIL 0xE
#define ELLEN 0xF
#define DOUG 0x10
#define GOTZ 0x11
#define SASHA 0x12
#define POTION_SHOP_DEALER 0x13
#define KENT 0x14
#define STU 0x15
#define MIDWIFE 0x16
#define MAY 0x17
#define RICK 0x18
#define PASTOR 0x19
#define SHIPPER 0x1A
#define SAIBARA 0x1B
#define DUKE 0x1C
#define GREG 0x1D
#define CARPENTER_1 0x1E
#define CARPENTER_2 0x1F
#define MASTER_CARPENTER 0x20
#define HARVEST_SPRITE_1 0x21
#define HARVEST_SPRITE_2 0x22
#define HARVEST_SPRITE_3 0x23
#define SYDNEY 0x24
#define BARLEY 0x25
#define GOURMET_JUDGE 0x26
#define PHOTOGRAPHER 0x27

// 0x801FB9D0
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
extern u8 func_80075374(u8 npcIndex, int arg1);
extern void func_80075A18(); 
extern void func_800758B8();                                  
extern void updateNPCCoordinates();  
extern u8 func_800858D4();    
extern bool func_80085D48(int index, u16 arg1); 
extern void setNpcAnimations(); 
extern void setNPCSpawningLocations();                     

extern u8 npcAffection[TOTAL_NPCS];
extern npcInfo npcInfoArray[];

extern u8 npcTalkingTo;

#endif