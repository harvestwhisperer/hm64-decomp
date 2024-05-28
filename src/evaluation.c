#include "common.h"

#include "game.h"
#include "gameStatus.h"
#include "mapObjects.h"
#include "npc.h"

//INCLUDE_ASM(const s32, "evaluation", calculateNPCAffectionScore);

u16 calculateNPCAffectionScore(void) {
    
    u8 i;
    u16 score = 0;

    for (i = 0; i < TOTAL_NPCS; i++) {
        score += npcAffection[i];
    }
    
    return score;

}

//INCLUDE_ASM(const s32, "evaluation", calculateHouseExtensionScore);

u32 calculateHouseExtensionScore(void) {

    u8 score;

    score = checkLifeEventBit(HAVE_KITCHEN) != 0;
    
    if (checkLifeEventBit(HAVE_BATHROOM)) {
        score++;
    }
    if (checkLifeEventBit(HAVE_STAIRS)) {
        score++;
    }
    if (checkLifeEventBit(HAVE_GREENHOUSE)) {
        score++;
    }
    if (checkLifeEventBit(HAVE_LOG_TERRACE)) {
        score++;
    }
    if (checkLifeEventBit(HAVE_BABY_BED)) {
        score++;
    }
    
    return score;
    
}

u16 calculateGrassScore(void) {
    
    u8 j, i;
    u16 score = 0;

    for (i = 0; i < FIELD_WIDTH; i++) {
        for (j = 0; j < FIELD_HEIGHT; j++) {
            switch (farmFieldTiles[i][j]) {
                 default:
                    break;
                // bug: includes withered corn plants in score
                case CORN_PLANT_WITHERED ... GRASS_CUT:
                    score++;
                    break;
            }
        }
    }

  return score;

}

//INCLUDE_ASM(const s32, "evaluation", calculateFieldScore);

u16 calculateFieldScore(void) {

    u8 i;
    u8 j;
    u16 score = 0;

    for (i = 0; i < FIELD_WIDTH; i++) {
        for (j = 0; j < FIELD_HEIGHT; j++) {
            if (farmFieldTiles[i][j] > WEED) {
                score++;
            } 
        }
    } 

    return score;

}

//INCLUDE_ASM(const s32, "evaluation", calculateShippingScore);

u8 calculateShippingScore(void) {

    u8 score = 0;
    
    u8 totalCropsShipped;
    u8 eggsShipped;
    u8 goldenMilkShipped;
    
    totalCropsShipped = (gTotalCropsShipped * 100) / MAX_TOTAL_CROPS_SHIPPED;
    goldenMilkShipped = (gTotalGoldenMilkShipped * 100) / MAX_ANIMAL_ITEM_SHIPPED;
    eggsShipped = (gTotalEggsShipped * 100) / MAX_ANIMAL_ITEM_SHIPPED;

    if (eggsShipped >= goldenMilkShipped || totalCropsShipped >= goldenMilkShipped) {

        score = 1;

        if (totalCropsShipped >= eggsShipped) {
            score = 2;            
        }
        
    }

    return score;
}

