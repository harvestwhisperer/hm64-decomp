#include "common.h"
#include "npc.h"

extern u32 gTotalGoldenMilkShipped;
extern u32 gTotalEggsShipped;
extern u32 gTotalCropsShipped;

extern u8 npcAffection[TOTAL_NPCS];

extern u8 farmFieldTiles[FARM_FIELD_WIDTH][FARM_FIELD_HEIGHT];

u32 checkLifeEventBit(u16 bitIndex);


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

u8 calculateHouseExtensionScore(void) {

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

    for (i = 0; i < FARM_FIELD_WIDTH; i++) {
        for (j = 0; j < FARM_FIELD_HEIGHT; j++) {
            switch (farmFieldTiles[i][j]) {
                 default:
                    break;
                // bug?
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

    for (i = 0; i < FARM_FIELD_WIDTH; i++) {
        for (j = 0; j < FARM_FIELD_HEIGHT; j++) {
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
    
    totalCropsShipped = ((gTotalCropsShipped * 100) / 99999);
    goldenMilkShipped = ((gTotalGoldenMilkShipped * 100) / 9999);
    eggsShipped = ((gTotalEggsShipped * 100) / 9999);

    if ((goldenMilkShipped <= eggsShipped) || (goldenMilkShipped <= totalCropsShipped)) {
        score = 1;
        if (eggsShipped <= totalCropsShipped) {
            score = 2;            
        }
    }

    return score;
}

