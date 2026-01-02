#ifndef _LEVEL_INTERACTIONS_H_
#define _LEVEL_INTERACTIONS_H_

#include "common.h"

typedef struct {
    u8 mapAdditionsIndex;
    u8 interactionSfxIndex;
} LevelInteractionsInfo;

extern u8 checkShippingBinInteraction(u16 mapIndex);
extern u8 checkBarnAnimalFeedInteraction(u16 mapIndex);
extern u8 checkFarmDogBowlInteraction(u16 mapIndex);
extern u8 checkWaterSourceInteraction(u16 mapIndex);
extern bool checkWineBarrelInteraction(u16);
extern bool checkChickenFeedInteraction(u16 mapIndex);
extern bool checkWaterableSoilInteraction(u16 mapIndex);   
extern bool checkFishingSpotInteraction(u16 mapIndex);
extern bool handleLevelInteraction(u16 mapIndex);

extern LevelInteractionsInfo levelInteractionsInfo;

#endif