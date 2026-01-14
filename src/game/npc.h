#ifndef _NPC_H
#define _NPC_H

#include "common.h"

#define MAX_NPCS 48

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
#define KAI 10
#define MAYOR 11
#define MAYOR_WIFE 12
#define LILLIA 13
#define BASIL 14
#define ELLEN 15
#define DOUG 16
#define GOTZ 17
#define SASHA 18
#define POTION_SHOP_DEALER 19
#define KENT 20
#define STU 21
#define MIDWIFE 22
#define MAY 23
#define RICK 24
#define PASTOR 25
#define SHIPPER 26
#define SAIBARA 27
#define DUKE 28
#define GREG 29
#define CARPENTER_1 30
#define CARPENTER_2 31
#define MASTER_CARPENTER 32
#define HARVEST_SPRITE_1 33
#define HARVEST_SPRITE_2 34
#define HARVEST_SPRITE_3 35
#define SYDNEY 36
#define BARLEY 37
#define MRS_MANA 38
#define JOHN 39
#define GOURMET_JUDGE 40
#define MARIA_HARRIS_BABY 41
#define POPURI_GRAY_BABY 42
#define ELLI_JEFF_BABY 43
#define ANN_CLIFF_BABY 44
#define KAREN_KAI_BABY 45
#define ENTOMOLOGIST 46

#define NPC_ACTIVE 1
#define NPC_NEEDS_UPDATE 2
#define NPC_ENTITY_LOADED 4
#define NPC_ATTACHED 8

/* NPC animation modes */
#define NPC_ANIMATION_IDLE              0   
#define NPC_ANIMATION_WANDER            1  
#define NPC_ANIMATION_LOCATION_SPECIAL  2  
#define NPC_ANIMATION_CUSTOM            3   
#define NPC_ANIMATION_SLEEPING          4
#define NPC_ANIMATION_BABY_HOUSE        4
#define NPC_ANIMATION_BABY_CRIB         5
#define NPC_ANIMATION_TALKING           16 
#define NPC_ANIMATION_FACE_PLAYER       32 

// 0x801FB9D0
typedef struct {
	Vec3f startingCoordinates;
	Vec3f currentCoordinates;
	u16 entityIndex;
	u8 wanderRadiusX;
	u8 wanderRadiusZ;
	u8 levelIndex;
	u8 animationMode;
	u8 defaultAnimationMode;
	u8 direction;
	u8 speed;
	u8 animationState;
	u8 animationTimer;
	u8 location;
	u8 idleAnimation;
	u8 movingAnimation;
	u16 flags;
} npcInfo;

extern void deactivateNPCEntities();
extern u8 setupNPCEntity(u8 npcIndex, u8 entityOffset);
extern void randomizeNPCLocations();
extern void setupActiveNPCs();
extern void updateNPCCoordinates();
extern u8 checkNPCInteraction();
extern bool getBlueFeatherResponse(u8 index, u16 arg1);
extern void setNPCAnimations();
extern void setNPCSpawningLocations();        
extern bool findNPCToTalkTo(void);                      

extern u8 npcAffection[MAX_NPCS];
extern npcInfo npcs[MAX_NPCS];

extern u8 npcTalkingTo;

#endif