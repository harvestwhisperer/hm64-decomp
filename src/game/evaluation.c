#include "common.h"

#include "game/game.h"
#include "game/gameStatus.h"
#include "game/fieldObjects.h"
#include "game/npc.h"

// bss

extern u16 D_8013D438;
// field score
extern u16 D_8013D438;
// grass score
extern u16 D_80205638;

// shipping score
extern u16 D_80189824;
// combined shipping totals
extern u32 D_801C3E1C;

// sick days total
extern u16 D_801886E0;

// npc affection score
extern u16 D_801C3B62;
// sum of girls' affection
extern u16 D_80215DF0;

// animal scores
extern u8 D_801886D0;
extern u8 D_801D62C6;
extern u8 D_80205209;
// dead animals
extern u16 D_801886A8;

// recipes
extern u8 D_80188F68;

// house extension score
extern u8 D_8017026E;

// fish score
extern u16 D_801D62C4;

//INCLUDE_ASM("asm/nonmatchings/game/evaluation", calculateNPCAffectionScore);

u16 calculateNPCAffectionScore(void) {
    
    u8 i;
    u16 score = 0;

    for (i = 0; i < MAX_NPCS; i++) {
        score += npcAffection[i];
    }
    
    return score;

}

//INCLUDE_ASM("asm/nonmatchings/game/evaluation", calculateHouseExtensionScore);

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

//INCLUDE_ASM("asm/nonmatchings/game/evaluation", calculateGrassScore);

u16 calculateGrassScore(void) {
    
    u8 j, i;
    u16 score = 0;

    for (i = 0; i < FIELD_HEIGHT; i++) {
        for (j = 0; j < FIELD_WIDTH; j++) {
            switch (farmFieldTiles[i][j]) {
                 default:
                    break;
                case GRASS_PLANTED_STAGE_1 ... GRASS_CUT:
                    score++;
                    break;
            }
        }
    }

  return score;

}

//INCLUDE_ASM("asm/nonmatchings/game/evaluation", calculateFieldScore);

u16 calculateFieldScore(void) {

    u8 i;
    u8 j;
    u16 score = 0;

    for (i = 0; i < FIELD_HEIGHT; i++) {
        for (j = 0; j < FIELD_WIDTH; j++) {
            if (farmFieldTiles[i][j] > WEED) {
                score++;
            } 
        }
    } 

    return score;

}

//INCLUDE_ASM("asm/nonmatchings/game/evaluation", calculateShippingScore);

u8 calculateShippingScore(void) {

    u8 score = 0;
    
    u8 totalCropsShipped;
    u8 eggsShipped;
    u8 goldenMilkShipped;
    
    totalCropsShipped = (gTotalCropsShipped * 100) / MAX_TOTAL_CROPS_SHIPPED;
    goldenMilkShipped = (gTotalMilkShipped * 100) / MAX_ANIMAL_ITEM_SHIPPED;
    eggsShipped = (gTotalEggsShipped * 100) / MAX_ANIMAL_ITEM_SHIPPED;

    if (eggsShipped >= goldenMilkShipped || totalCropsShipped >= goldenMilkShipped) {

        score = 1;

        if (totalCropsShipped >= eggsShipped) {
            score = 2;            
        }
        
    }

    return score;

}