#include "common.h"

#include "game/cutscenes.h"

#include "system/cutscene.h"
#include "system/entity.h"
#include "system/graphic.h"
#include "system/mapController.h"
#include "system/math.h"
#include "system/message.h"

#include "game/animals.h"
#include "game/evaluation.h"
#include "game/game.h"
#include "game/gameStatus.h"
#include "game/items.h"
#include "game/level.h"
#include "game/fieldObjects.h"
#include "game/namingScreen.h"
#include "game/npc.h"
#include "game/overlayScreens.h"
#include "game/player.h"
#include "game/shop.h"
#include "game/time.h"
#include "game/title.h"
#include "game/transition.h"
#include "game/weather.h"

#include "mainLoop.h"

#include "buffers/buffers.h"
#include "ld_symbols.h"

#include "data/fieldTileMaps/cutsceneFieldTiles.h"
#include "data/fieldTileMaps/fieldTiles.h"

#include "assetIndices/dialogues.h"
#include "assetIndices/cutscenes.h"
#include "assetIndices/entities.h"
#include "assetIndices/maps.h"
#include "assetIndices/sprites.h"

// forward declarations
u16 setBeachCutscenes(void);                          
u16 setCaveCutscenes(void);                          
u16 setChurchCutscenes(void);                        
u16 setFarmCutscenes(void);
u16 setFlowerShopCutscenes(void);                    
u16 setHarvestSpriteCaveCutscenes(void);             
u16 setMoonMountainCutscenes(void);                     
u16 setMountain1Cutscenes(void);
u16 setRanchStoreCutscenes(void);                    
u16 setRickStoreCutscenes(void);                     
u16 setRoadCutscenes(void);                          
u16 setSpringCutscenes(void);                        
u16 setVineyardCellarCutscenes(void);                
u16 setVineyardHouseCutscenes(void); 
// TODO: finish labeling
u16 setMountain2Cutscenes(void);
u16 setTopOfMountain1Cutscenes(void);
u16 setHouseCutscenes(void);
u16 setKitchenCutscenes(void);
u16 setRanchCutscenes(void);
u16 setVineyardCutscenes(void);
u16 setVillage1Cutscenes(void);
u16 setVillage2Cutscenes(void);
u16 setBakeryCutscenes(void);
u16 setMayorHouseCutscenes(void);
u16 setLibraryCutscenes(void);
u16 setTavernCutscenes(void);
u16 setSquareCutscenes(void);
u16 setRaceTrackCutscenes(void);

// bss
s32 gCutsceneCompletionFlags;
u16 gCutsceneBytecodeSegmentIndex;
u16 gCutsceneIndex;
u32 gCutsceneFlags;

// shipping score
// horse affection (horse race)
u16 D_80189824;

u16 D_80180710;

// evaluation
// field score
u16 D_8013D438;
// animal scores
u8 D_801886D0;
// house extension score
u8 D_8017026E;
// dead animals
u16 D_801886A8;
u16 D_801886D2;
// sick days total
u16 D_801886E0;
// recipes
u8 D_80188F68;
u8 D_801C3B60;
// npc affection score
u16 D_801C3B62;
// combined shipping totals
u32 D_801C3E1C;
u32 D_801C3F4C;
// fish score
u16 D_801D62C4;
// animal score
u8 D_801D62C6;
u8 D_80205209;
// grass score
u16 D_80205638;
// sum of girls' affection
u16 D_80215DF0;

u32 D_80237A00;

u8 cutsceneLevelInteractionIndex;
u8 interactedWithLevelInteractionIndex;

RacingContext gRacingContext;


// shared (with overlayScreens.c)
u32 D_801C3F78;
u8 gRaceFinishOrder[6];

// data 
Addresses cutsceneBytecodeAddresses[] = {
      { &_farmBusinessSegmentRomStart, &_farmBusinessSegmentRomEnd },
      { &_farmBusinessSegmentRomStart, &_farmBusinessSegmentRomEnd },
      { &_farmBusinessSegmentRomStart, &_farmBusinessSegmentRomEnd },
      { &_farmBusinessSegmentRomStart, &_farmBusinessSegmentRomEnd },
      { &_farmBusinessSegmentRomStart, &_farmBusinessSegmentRomEnd },
      { &_farmBusinessSegmentRomStart, &_farmBusinessSegmentRomEnd },
      { &_farmBusinessSegmentRomStart, &_farmBusinessSegmentRomEnd },
      { &_farmVisitsSegmentRomStart, &_farmVisitsSegmentRomEnd },
      { &_farmVisitsSegmentRomStart, &_farmVisitsSegmentRomEnd },
      { &_familyCutscenesSegmentRomStart, &_familyCutscenesSegmentRomEnd },
      { &_cutsceneBank4SegmentRomStart, &_cutsceneBank4SegmentRomEnd },
      { &_cutsceneBank5SegmentRomStart, &_cutsceneBank5SegmentRomEnd },
      { &_annCutscenesSegmentRomStart, &_annCutscenesSegmentRomEnd },
      { &_karenCutscenes1SegmentRomStart, &_karenCutscenes1SegmentRomEnd },
      { &_cutsceneBank8SegmentRomStart, &_cutsceneBank8SegmentRomEnd },
      { &_mariaCutscenesSegmentRomStart, &_mariaCutscenesSegmentRomEnd },
      { &_karenCutscenes2SegmentRomStart, &_karenCutscenes2SegmentRomEnd },
      { &_sowingFestivalSegmentRomStart, &_sowingFestivalSegmentRomEnd },
      { &_horseRaceSegmentRomStart, &_horseRaceSegmentRomEnd },
      { &_flowerFestivalSegmentRomStart, &_flowerFestivalSegmentRomEnd },
      { &_squareFireworksSegmentRomStart, &_squareFireworksSegmentRomEnd },
      { &_fireworksFestivalSegmentRomStart, &_fireworksFestivalSegmentRomEnd },
      { &_fireflyFestivalSegmentRomStart, &_fireflyFestivalSegmentRomEnd },
      { &_seaFestivalSegmentRomStart, &_seaFestivalSegmentRomEnd },
      { &_cowFestivalSegmentRomStart, &_cowFestivalSegmentRomEnd },
      { &_harvestFestivalSegmentRomStart, &_harvestFestivalSegmentRomEnd },
      { &_eggFestivalSegmentRomStart, &_eggFestivalSegmentRomEnd },
      { &_dogRaceSegmentRomStart, &_dogRaceSegmentRomEnd },
      { &_spiritFestivalSegmentRomStart, &_spiritFestivalSegmentRomEnd },
      { &_newYearFestivalSegmentRomStart, &_newYearFestivalSegmentRomEnd },
      { &_funeralIntroSegmentRomStart, &_funeralIntroSegmentRomEnd },
      { &_evaluationEndingSegmentRomStart, &_evaluationEndingSegmentRomEnd },
      { &_demosSegmentRomStart, &_demosSegmentRomEnd },
      { &_farmBusinessSegmentRomStart, &_farmBusinessSegmentRomEnd },
      { &_farmBusinessSegmentRomStart, &_farmBusinessSegmentRomEnd },
      { &_howToPlaySegmentRomStart, &_howToPlaySegmentRomEnd },
      { 0, 0 },
      { 0, 0 },
      { 0, 0 },
      { 0, 0 },
      { 0, 0 },
      { 0, 0 },
      { 0, 0 },
      { 0, 0 },
      { 0, 0 },
      { 0, 0 },
      { 0, 0 },
      { 0, 0 }
};

void *cutsceneEntryPointAddresses[] = {
    CUTSCENE_BYTECODE_BUFFER_1 + CUTSCENE_BANK_0_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_1 + CUTSCENE_BANK_1_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_1 + CUTSCENE_BANK_2_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_1 + CUTSCENE_BANK_3_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_1 + CUTSCENE_BANK_4_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_1 + CUTSCENE_BANK_5_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_1 + CUTSCENE_BANK_6_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_1 + CUTSCENE_BANK_7_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_2 + CUTSCENE_BANK_8_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_2 + CUTSCENE_BANK_9_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_2 + CUTSCENE_BANK_10_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_2 + CUTSCENE_BANK_11_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_2 + CUTSCENE_BANK_12_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_2 + CUTSCENE_BANK_13_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_2 + CUTSCENE_BANK_14_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_2 + CUTSCENE_BANK_15_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_2 + CUTSCENE_BANK_16_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_2 + CUTSCENE_BANK_17_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_2 + CUTSCENE_BANK_18_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_2 + CUTSCENE_BANK_19_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_2 + CUTSCENE_BANK_20_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_2 + CUTSCENE_BANK_21_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_2 + CUTSCENE_BANK_22_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_2 + CUTSCENE_BANK_23_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_2 + CUTSCENE_BANK_24_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_2 + CUTSCENE_BANK_25_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_2 + CUTSCENE_BANK_26_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_2 + CUTSCENE_BANK_27_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_2 + CUTSCENE_BANK_28_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_2 + CUTSCENE_BANK_29_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_2 + CUTSCENE_BANK_30_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_2 + CUTSCENE_BANK_31_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_2 + CUTSCENE_BANK_32_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_1 + CUTSCENE_BANK_33_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_1 + CUTSCENE_BANK_34_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_2 + CUTSCENE_BANK_35_ENTRY_OFFSET,
    0,0,0,0,0,0,0,0,0,0,0,0
};

// vaddrs of entire cutscene bytecode banks
void *cutsceneBankLoadAddresses[] = {
    CUTSCENE_BYTECODE_BUFFER_1,
    CUTSCENE_BYTECODE_BUFFER_1,
    CUTSCENE_BYTECODE_BUFFER_1,
    CUTSCENE_BYTECODE_BUFFER_1,
    CUTSCENE_BYTECODE_BUFFER_1,
    CUTSCENE_BYTECODE_BUFFER_1,
    CUTSCENE_BYTECODE_BUFFER_1,
    CUTSCENE_BYTECODE_BUFFER_2,
    CUTSCENE_BYTECODE_BUFFER_2,
    CUTSCENE_BYTECODE_BUFFER_2,
    CUTSCENE_BYTECODE_BUFFER_2,
    CUTSCENE_BYTECODE_BUFFER_2,
    CUTSCENE_BYTECODE_BUFFER_2,
    CUTSCENE_BYTECODE_BUFFER_2,
    CUTSCENE_BYTECODE_BUFFER_2,
    CUTSCENE_BYTECODE_BUFFER_2,
    CUTSCENE_BYTECODE_BUFFER_2,
    CUTSCENE_BYTECODE_BUFFER_2,
    CUTSCENE_BYTECODE_BUFFER_2,
    CUTSCENE_BYTECODE_BUFFER_2,
    CUTSCENE_BYTECODE_BUFFER_2,
    CUTSCENE_BYTECODE_BUFFER_2,
    CUTSCENE_BYTECODE_BUFFER_2,
    CUTSCENE_BYTECODE_BUFFER_2,
    CUTSCENE_BYTECODE_BUFFER_2,
    CUTSCENE_BYTECODE_BUFFER_2,
    CUTSCENE_BYTECODE_BUFFER_2,
    CUTSCENE_BYTECODE_BUFFER_2,
    CUTSCENE_BYTECODE_BUFFER_2,
    CUTSCENE_BYTECODE_BUFFER_2,
    CUTSCENE_BYTECODE_BUFFER_2,
    CUTSCENE_BYTECODE_BUFFER_2,
    CUTSCENE_BYTECODE_BUFFER_2,
    CUTSCENE_BYTECODE_BUFFER_1,
    CUTSCENE_BYTECODE_BUFFER_1,
    CUTSCENE_BYTECODE_BUFFER_2,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};

u16 cutsceneExecutorIndices[] = {
    CUTSCENE_BANK_0_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_1_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_2_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_3_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_4_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_5_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_6_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_7_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_8_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_9_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_10_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_11_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_12_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_13_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_14_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_15_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_16_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_17_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_18_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_19_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_20_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_21_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_22_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_23_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_24_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_25_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_26_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_27_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_28_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_29_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_30_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_31_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_32_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_33_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_34_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_35_STARTING_EXECUTOR_INDEX,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0
};

static inline void transitionCutscenes(u16 cutsceneIndex, u16 entranceIndex) {
    setEntrance(entranceIndex);
    gCutsceneIndex = cutsceneIndex;
    loadCutscene(0);
    loadLevelFromEntrance(entranceIndex);
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", initializeCutscene);

inline void initializeCutscene(u16 segmentIndex) {

    nuPiReadRom(cutsceneBytecodeAddresses[segmentIndex].romAddrStart, cutsceneBankLoadAddresses[segmentIndex], cutsceneBytecodeAddresses[segmentIndex].romAddrEnd - cutsceneBytecodeAddresses[segmentIndex].romAddrStart);

    gCutsceneCompletionFlags = 0;

    spawnCutsceneExecutor(cutsceneExecutorIndices[segmentIndex], cutsceneEntryPointAddresses[segmentIndex]);
    
    togglePauseEntities();
    
    gCutsceneBytecodeSegmentIndex = segmentIndex;

    gCutsceneFlags |= CUTSCENE_ACTIVE;

    toggleDailyEventBit(0x53);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setLevelCutscene);

u16 setLevelCutscene(u16 mapIndex) {
    
    u16 offset = 0xFFFF;
    
    if (!(gCutsceneFlags & 1)) {
        
        gCutsceneFlags &= ~2;

        switch (mapIndex) {
            case FARM:
                offset = setFarmCutscenes();
                break;
            case MOUNTAIN_1:
                offset = setMountain1Cutscenes();
                break;
            case MOUNTAIN_2:
                offset = setMountain2Cutscenes();
                break;
            case TOP_OF_MOUNTAIN_1:
                offset = setTopOfMountain1Cutscenes();
                break;
            case MOON_MOUNTAIN:
                offset = setMoonMountainCutscenes();
                break;
            case POND:
                offset = setSpringCutscenes();
                break;
            case HARVEST_SPRITE_CAVE:
                offset = setHarvestSpriteCaveCutscenes();
                break;
            case CAVE:
                offset = setCaveCutscenes();
                break;
            case HOUSE:
                offset = setHouseCutscenes();
                break;
            case KITCHEN:
                offset = setKitchenCutscenes();
                break;
            case RANCH:
                offset = setRanchCutscenes();
                break;
            case RANCH_STORE:
                offset = setRanchStoreCutscenes();
                break;
            case VINEYARD:
                offset = setVineyardCutscenes();
                break;
            case VINEYARD_HOUSE:
                offset = setVineyardHouseCutscenes();
                break;
            case VINEYARD_CELLAR:
                offset = setVineyardCellarCutscenes();
                break;
            case VILLAGE_1:
                offset = setVillage1Cutscenes();
                break;
            case VILLAGE_2:
                offset = setVillage2Cutscenes();
                break;
            case RICK_STORE:
                offset = setRickStoreCutscenes();
                break;
            case FLOWER_SHOP:
                offset = setFlowerShopCutscenes();
                break;
            case BAKERY:
                offset = setBakeryCutscenes();
                break;
            case MAYOR_HOUSE:
                offset = setMayorHouseCutscenes();
                break;
            case LIBRARY:
                offset = setLibraryCutscenes();
                break;
            case CHURCH:
                offset = setChurchCutscenes();
                break;
            case TAVERN:
                offset = setTavernCutscenes();
                break;
            case SQUARE:
                offset = setSquareCutscenes();
                break;
            case RACE_TRACK:
                offset = setRaceTrackCutscenes();
                break;
            case BEACH:
                offset = setBeachCutscenes();
                break;
            case ROAD:
                offset = setRoadCutscenes();
                break;
            default:
                break;
            }
        
        if (offset != 0xFFFF) {
            initializeCutscene(offset);
        }
        
    }
    
    return offset;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setFarmVisitCutscenes);

// arg0 = cutscene type; 0 = show up while working; 1 = morning/show up on map entry
u16 setFarmVisitCutscenes(u8 arg0) {

    bool set = FALSE;
    u16 result = 0xFFFF;
    u8 tempGirl;
    
    if (!checkLifeEventBit(MAYOR_TOUR)) {
        setLifeEventBit(MAYOR_TOUR);
        gCutsceneIndex = MAYOR_VILLAGE_TOUR;
        gCutsceneFlags |= 2;
        loadCutscene(arg0);
        set = TRUE;
    }

    if (gEntranceIndex != 0x6F) {
        
        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gYear == 1 && gSeason == AUTUMN && gDayOfMonth == 22 && (5 < gHour && gHour < 18)) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = 0xDF;
                loadCutscene(arg0);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gYear == 1 && gSeason == WINTER && gDayOfMonth == 11 && (5 < gHour && gHour < 18)) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = 0xDE;
                loadCutscene(arg0);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == SPRING && gDayOfMonth == 16 && (5 < gHour && gHour < 15) && (horseInfo.flags & 1) && horseInfo.grown == TRUE) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = DOUG_FARM_HORSE_RACE;
                loadCutscene(arg0);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == AUTUMN && gDayOfMonth == 27 && (5 < gHour && gHour < 15) && (horseInfo.flags & 1) && horseInfo.grown == TRUE) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = 0x7E;
                loadCutscene(arg0);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == WINTER && gDayOfMonth == 18 && (5 < gHour && gHour < 15)) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = DOUG_FARM_DOG_RACE;
                loadCutscene(arg0);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == SPRING && gDayOfMonth == 7 && (5 < gHour && gHour < 9)) {
                
                setDailyEventBit(FARM_MORNING_VISIT);
                
                if (npcAffection[KENT] < 30) {
                    if (gYear == 1) {
                        gCutsceneIndex = MAYOR_FARM_SOWING_FESTIVAL_YEAR_1;
                    } else {
                        gCutsceneIndex = MAYOR_FARM_SOWING_FESTIVAL;
                    }
                } else {
                    gCutsceneIndex = KENT_FARM_SOWING_FESITVAL;
                }
                
                loadCutscene(arg0);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == AUTUMN && gDayOfMonth == 2 && (5 < gHour && gHour < 9) && npcAffection[KENT] >= 30) {
                setDailyEventBit(FARM_MORNING_VISIT);
                
                if (getTotalFarmAnimalsByType(2)) {
                    gCutsceneIndex = 0x66;
                    setGameVariableString(0xE, gFarmAnimals[getCowWithHighestAffection()].name, 6);
                } else {
                    gCutsceneIndex = 0x65;
                }
                
                loadCutscene(arg0);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == WINTER && gDayOfMonth == 9 && (5 < gHour && gHour < 9) && npcAffection[KENT] >= 30) {
                setDailyEventBit(FARM_MORNING_VISIT);
                
                if (gYear == 1) {
                    gCutsceneIndex = KENT_FARM_THANKSGIVING_FESTIVAL;
                } else {
                    gCutsceneIndex = 0x68;
                }
                
                loadCutscene(arg0);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == SPRING && gDayOfMonth == 16 && (5 < gHour && gHour < 9)) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = MAYOR_FARM_HORSE_RACE;
                loadCutscene(arg0);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == SUMMER && gDayOfMonth == 17 && (5 < gHour && gHour < 9)) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = MAYOR_FARM_FIREFLY_FESTIVAL;
                loadCutscene(arg0);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == SUMMER && gDayOfMonth == 23 && (5 < gHour && gHour < 9)) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = MAYOR_FARM_SEA_FESTIVAL;
                loadCutscene(arg0);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == AUTUMN && gDayOfMonth == 11 && (5 < gHour && gHour < 9)) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = MAYOR_FARM_HARVEST_FESTIVAL;
                loadCutscene(arg0);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == AUTUMN && gDayOfMonth == 19 && (5 < gHour && gHour < 9)) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = MAYOR_FARM_EGG_FESTIVAL;
                loadCutscene(arg0);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == WINTER && gDayOfMonth == 27 && (5 < gHour && gHour < 9)) {
                
                setDailyEventBit(FARM_MORNING_VISIT);
                
                if (gHarvestKing == PLAYER) {
                    gCutsceneIndex = MAYOR_FARM_SPIRIT_FESTIVAL_PLAYER_HARVEST_KING;
                } else {
                    gCutsceneIndex = MAYOR_FARM_SPIRIT_FESTIVAL;
                }
                
                loadCutscene(arg0);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == SPRING && gDayOfMonth == 19 && (5 < gHour && gHour < 9)) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = BASIL_FARM_FLOWER_FESTIVAL;
                loadCutscene(arg0);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == SUMMER && gDayOfMonth == 8 && (5 < gHour && gHour < 9)) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = BASIL_FARM_VEGETABLE_FESTIVAL;
                loadCutscene(arg0);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(POPURI_MARRIAGE_CUTSCENE) && npcAffection[POPURI] >= 50 && gSeason == SPRING && gDayOfMonth == 22 && (5 < gHour && gHour < 9) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
                setDailyEventBit(POPURI_MARRIAGE_CUTSCENE);
                gCutsceneIndex = 0x9C;
                loadCutscene(arg0);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(ELLI_MARRIAGE_CUTSCENE) && npcAffection[ELLI] >= 50 && gSeason == AUTUMN && gDayOfMonth == 11 && (6 < gHour && gHour < 9) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {
                setDailyEventBit(ELLI_MARRIAGE_CUTSCENE);
                gCutsceneIndex = 0xA8;
                loadCutscene(arg0);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(ANN_MARRIAGE_CUTSCENE) && npcAffection[ANN] >= 80 && gSeason == SPRING && gDayOfMonth == 16 && (6 < gHour && gHour < 9) && !checkLifeEventBit(MARRIED) && checkLifeEventBit(0x51) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
                setDailyEventBit(ANN_MARRIAGE_CUTSCENE);
                gCutsceneIndex = 0xB3;
                loadCutscene(arg0);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(MARIA_MARRIAGE_CUTSCENE) && npcAffection[MARIA] >= 50 && gSeason == SUMMER && gDayOfMonth == 1 && gHour == 8 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
                setDailyEventBit(MARIA_MARRIAGE_CUTSCENE);
                gCutsceneIndex = MARIA_FIREWORKS_INVITATION;
                loadCutscene(arg0);
                set = TRUE;
            }            
        }
        
        if (!set) {
            if (!checkDailyEventBit(POPURI_MARRIAGE_CUTSCENE) && npcAffection[POPURI] >= 50 && gSeason == SUMMER && gDayOfMonth == 1 && gHour == 10 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
                setDailyEventBit(POPURI_MARRIAGE_CUTSCENE);
                gCutsceneIndex = POPURI_FIREWORKS_INVITATION;
                loadCutscene(arg0);
                set = TRUE;
            }            
        }

        if (!set) {
            if (!checkDailyEventBit(ELLI_MARRIAGE_CUTSCENE) && npcAffection[ELLI] >= 50 && gSeason == SUMMER && gDayOfMonth == 1 && gHour == 12 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {
                setDailyEventBit(ELLI_MARRIAGE_CUTSCENE);
                gCutsceneIndex = ELLI_FIREWORKS_INVITATION;
                loadCutscene(arg0);
                set = TRUE;
            }            
        }

        if (!set) {
            if (!checkDailyEventBit(ANN_MARRIAGE_CUTSCENE) && npcAffection[ANN] >= 50 && gSeason == SUMMER && gDayOfMonth == 1 && gHour == 14 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
                setDailyEventBit(ANN_MARRIAGE_CUTSCENE);
                gCutsceneIndex = ANN_FIREWORKS_INVITATION;
                loadCutscene(arg0);
                set = TRUE;
            }            
        }

        if (!set) {
            if (!checkDailyEventBit(KAREN_MARRIAGE_CUTSCENE) && npcAffection[KAREN] >= 50 && gSeason == SUMMER && gDayOfMonth == 1 && gHour == 16 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
                setDailyEventBit(KAREN_MARRIAGE_CUTSCENE);
                gCutsceneIndex = KAREN_FIREWORKS_INVITATION;
                loadCutscene(arg0);
                set = TRUE;
            }            
        }
        
        if (!set) {
            if (!checkDailyEventBit(MARIA_MARRIAGE_CUTSCENE) && npcAffection[MARIA] >= 140 && gSeason == WINTER && gDayOfMonth == 10 && gHour == 8 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
                setDailyEventBit(MARIA_MARRIAGE_CUTSCENE);
                gCutsceneIndex = MARIA_THANKSGIVING_GIFT;
                loadCutscene(arg0);
                set = TRUE;
            }            
        }        

        if (!set) {
            if (!checkDailyEventBit(POPURI_MARRIAGE_CUTSCENE) && npcAffection[POPURI] >= 140 && gSeason == WINTER && gDayOfMonth == 10 && gHour == 10 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
                setDailyEventBit(POPURI_MARRIAGE_CUTSCENE);
                gCutsceneIndex = POPURI_THANKSGIVING_GIFT;
                loadCutscene(arg0);
                set = TRUE;
            }            
        }        

        if (!set) {
            if (!checkDailyEventBit(ELLI_MARRIAGE_CUTSCENE) && npcAffection[ELLI] >= 140 && gSeason == WINTER && gDayOfMonth == 10 && gHour == 12 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {
                setDailyEventBit(ELLI_MARRIAGE_CUTSCENE);
                gCutsceneIndex = ELLI_THANKSGIVING_GIFT;
                loadCutscene(arg0);
                set = TRUE;
            }            
        }        

        if (!set) {
            if (!checkDailyEventBit(ANN_MARRIAGE_CUTSCENE) && npcAffection[ANN] >= 140 && gSeason == WINTER && gDayOfMonth == 10 && gHour == 14 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
                setDailyEventBit(ANN_MARRIAGE_CUTSCENE);
                gCutsceneIndex = ANN_THANKSGIVING_GIFT;
                loadCutscene(arg0);
                set = TRUE;
            }            
        }    

        if (!set) {
            if (!checkDailyEventBit(KAREN_MARRIAGE_CUTSCENE) && npcAffection[KAREN] >= 140 && gSeason == WINTER && gDayOfMonth == 10 && gHour == 16 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
                setDailyEventBit(KAREN_MARRIAGE_CUTSCENE);
                gCutsceneIndex = KAREN_THANKSGIVING_GIFT;
                loadCutscene(arg0);
                set = TRUE;
            }            
        }       

        if (!set) {
            if (!checkDailyEventBit(MARIA_MARRIAGE_CUTSCENE) && npcAffection[MARIA] >= 100 && gSeason == WINTER && gDayOfMonth == 24 && gHour == 8 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
                setDailyEventBit(MARIA_MARRIAGE_CUTSCENE);
                gCutsceneIndex = 0x95;
                loadCutscene(arg0);
                set = TRUE;
            }            
        }        

        if (!set) {
            if (!checkDailyEventBit(POPURI_MARRIAGE_CUTSCENE) && npcAffection[POPURI] >= 100 && gSeason == WINTER && gDayOfMonth == 24 && gHour == 10 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
                setDailyEventBit(POPURI_MARRIAGE_CUTSCENE);
                gCutsceneIndex = 0xA0;
                loadCutscene(arg0);
                set = TRUE;
            }            
        }        

        if (!set) {
            if (!checkDailyEventBit(ELLI_MARRIAGE_CUTSCENE) && npcAffection[ELLI] >= 100 && gSeason == WINTER && gDayOfMonth == 24 && gHour == 12 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {
                setDailyEventBit(ELLI_MARRIAGE_CUTSCENE);
                gCutsceneIndex = 0xAC;
                loadCutscene(arg0);
                set = TRUE;
            }            
        }       

        if (!set) {
            if (!checkDailyEventBit(ANN_MARRIAGE_CUTSCENE) && npcAffection[ANN] >= 100 && gSeason == WINTER && gDayOfMonth == 24 && gHour == 14 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
                setDailyEventBit(ANN_MARRIAGE_CUTSCENE);
                gCutsceneIndex = 0xB7;
                loadCutscene(arg0);
                set = TRUE;
            }            
        }       

        if (!set) {
            if (!checkDailyEventBit(KAREN_MARRIAGE_CUTSCENE) && npcAffection[KAREN] >= 100 && gSeason == WINTER && gDayOfMonth == 24 && gHour == 16 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
                setDailyEventBit(KAREN_MARRIAGE_CUTSCENE);
                gCutsceneIndex = 0xC4;
                loadCutscene(arg0);
                set = TRUE;
            }            
        }       

       if (!set) {
           
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == gPlayerBirthdaySeason && gDayOfMonth == 25 && (5 < gHour && gHour < 18)) {
                
                tempGirl = getBacholeretteWithHighestAffection(120);
                
                if (tempGirl) {       

                    setDailyEventBit(FARM_MORNING_VISIT);
                    
                    switch (tempGirl) {
                        
                        case 1:
                            
                            if (!checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
                                
                                if (npcAffection[MARIA] >= 160) {
                                    setLifeEventBit(0x3F);
                                    gCutsceneIndex = 0x99;
                                } else {
                                    gCutsceneIndex = 0x98;
                                }
                                
                                gCutsceneFlags |= 2;
                                loadCutscene(arg0);
                                set = TRUE;
                            }

                            break;
                        
                        case 2:
                            
                            if (!checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
                                
                               if (npcAffection[POPURI] >= 160) {
                                    acquireKeyItem(POTPURRI);
                                    gCutsceneIndex = 0xA4;
                                } else {
                                    gCutsceneIndex = 0xA3;
                                }
                                
                                gCutsceneFlags |= 2;
                                loadCutscene(arg0);
                                set = TRUE;                                
                            }

                            break;

                        case 3:

                            if (!checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {
                                
                               if (npcAffection[ELLI] >= 160) {
                                    acquireKeyItem(EMBROIDERED_HANDKERCHIEF);
                                    gCutsceneIndex = 0xB0;
                                } else {
                                    gCutsceneIndex = 0xAF;
                                }
                                
                                gCutsceneFlags |= 2;
                                loadCutscene(arg0);
                                set = TRUE;                                
                            }

                            break;

                        case 4:

                            if (!checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
                                
                               if (npcAffection[ANN] >= 160) {
                                    acquireKeyItem(HAND_KNIT_SOCKS);
                                    gCutsceneIndex = 0xBB;
                                } else {
                                    gCutsceneIndex = 0xBA;
                                }
                                
                                gCutsceneFlags |= 2;
                                loadCutscene(arg0);
                                set = TRUE;                                
                            }

                            break;   

                        case 5:

                            if (!checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
                                
                               if (npcAffection[KAREN] >= 160) {
                                    acquireKeyItem(LUCKY_BRACELET);
                                    gCutsceneIndex = 0xC8;
                                } else {
                                    gCutsceneIndex = 0xC7;
                                }
                                
                                gCutsceneFlags |= 2;
                                loadCutscene(arg0);
                                set = TRUE;                                
                            }

                            break;

                        default:
                            break; 
                    }
                }
            }            
        }      

        if (!set) {
            if (!checkLifeEventBit(BASIL_IN_TOWN) && ((gSeason == SPRING && gDayOfMonth >= 15) || gSeason == SUMMER) && (5 < gHour && gHour < 17)) {
                
                setLifeEventBit(BASIL_IN_TOWN);
                toggleLifeEventBit(BASIL_GONE);       
                
                if (gYear != 1) {
                    if (npcAffection[BASIL] >= 150) {
                        gCutsceneIndex = BASIL_POWER_NUT_GIFT;
                    } else {
                        gCutsceneIndex = BASIL_RETURN;
                    }
                } else {
                    gCutsceneIndex = BASIL_INTRO;
                }
                
                loadCutscene(arg0);
                set = TRUE;
            }            
        }

        if (!set) {
            if (checkLifeEventBit(BASIL_IN_TOWN) && ((gSeason == AUTUMN && gDayOfMonth >= 5) || gSeason == WINTER) && (5 < gHour && gHour < 17)) {
                toggleLifeEventBit(BASIL_IN_TOWN);
                setLifeEventBit(BASIL_GONE);      
                gCutsceneIndex = BASIL_LEAVING;
                loadCutscene(arg0);
                set = TRUE;
            }
        }

        if (!set && arg0 == 0) {
            if (!checkLifeEventBit(CLIFF_ARRIVED) && (gSeason == SPRING && !(gDayOfMonth < 18) || !(gSeason < SUMMER))) {
                setLifeEventBit(CLIFF_ARRIVED);      
                gCutsceneIndex = CLIFF_INTRO;
                loadCutscene(arg0);      
                set = TRUE;
            }
        } 

        // Cliff leaving
        if (!set) {
            if (checkLifeEventBit(0x41) && !checkLifeEventBit(0x42) && gSeason == WINTER && (5 < gHour && gHour < 18) && gHarvestKing != 4 && gHarvestCoinFinder != 4) {
                
                if (checkLifeEventBit(MARRIED) && gWife == ANN && npcAffection[CLIFF] >= 150 && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
                    setLifeEventBit(0x42);
                    setLifeEventBit(0x2D);
                    gCutsceneIndex = 0xD5;
                    loadCutscene(arg0);
                    set = TRUE;                    
                }
                
                if (npcAffection[CLIFF] < 51 && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {              
                    setLifeEventBit(0x42);
                    setLifeEventBit(0x2D);
                    gCutsceneIndex = CLIFF_LEAVING_DEFAULT;
                    loadCutscene(arg0);
                    set = TRUE;                                
                } 
            }
        }

        if (!set) {
            if (!checkLifeEventBit(0x33) && checkDailyEventBit(0x22) && npcAffection[KENT] >= 5 && (5 < gHour && gHour < 9)) {
                setLifeEventBit(0x33);
                gCutsceneIndex = 0x69;
                loadCutscene(arg0);
                set = TRUE;                               
            }
        }
        
        if (!set) {
            if (!checkLifeEventBit(0x35) && checkDailyEventBit(0x23) && npcAffection[KENT] >= 5 && (5 < gHour && gHour < 9)) {
                setLifeEventBit(0x35);
                gCutsceneIndex = 0x6A;
                loadCutscene(arg0);
                set = TRUE;                               
            }
        }
        
        if (!set) {

            if (!checkLifeEventBit(0x37) && checkDailyEventBit(0x24) && npcAffection[KENT] >= 5 && gWeather == SUNNY && (5 < gHour && gHour < 9)) {
                
                setLifeEventBit(0x37);

                // get cow + grass count
                if (!func_8009B828(0)) {
                    gCutsceneIndex = 0x6C;
                } else {
                    gCutsceneIndex = 0x6B;
                }
                
                loadCutscene(arg0);
                set = TRUE;                               
            }
        }
        
        if (!set) {
            if (!checkLifeEventBit(0x39) && checkDailyEventBit(0x25) && npcAffection[KENT] >= 5 && (5 < gHour && gHour < 9)) {
                setLifeEventBit(0x39);
                gCutsceneIndex = 0x6F;
                loadCutscene(arg0);
                set = TRUE;                               
            }
        }            
        
        if (!set) {
            if (!checkLifeEventBit(0x3B) && checkDailyEventBit(0x26) && npcAffection[KENT] >= 5 && (5 < gHour && gHour < 9)) {
                setLifeEventBit(0x3B);
                gCutsceneIndex = 0x6D;
                loadCutscene(arg0);
                set = TRUE;                               
            }
        }     

        if (!set) {
            if (!checkLifeEventBit(0x3D) && checkDailyEventBit(0x27) && npcAffection[KENT] >= 5 && (5 < gHour && gHour < 9)) {
                setLifeEventBit(0x3D);
                gCutsceneIndex = 0x6E;
                loadCutscene(arg0);
                set = TRUE;                               
            }
        }     

        if (!set) {
            
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gForecast == TYPHOON && ((npcAffection[HARVEST_SPRITE_1] + npcAffection[HARVEST_SPRITE_2] + npcAffection[HARVEST_SPRITE_3]) / 3) >= 120 && (5 < gHour && gHour < 9)) {

                if ((getTotalChickenCount() + getTotalFarmAnimalsCount()) >= 2) {           
                    setDailyEventBit(FARM_MORNING_VISIT);
                    gCutsceneIndex = 0xE3;
                    loadCutscene(arg0);
                    set = TRUE;                               
                }
            }

        }    

        if (!set) {
            
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && ((npcAffection[CARPENTER_1] + npcAffection[CARPENTER_2] + npcAffection[MASTER_CARPENTER]) / 3) >= 120 && (5 < gHour && gHour < 9)) {

                if (checkDailyEventBit(0x3F)) {           
                    setDailyEventBit(FARM_MORNING_VISIT);
                    gCutsceneIndex = 0xDD;
                    loadCutscene(arg0);
                    set = TRUE;                               
                }
            }
        }    

        if (!set) {
            if (!checkLifeEventBit(0x8E) && getTotalFarmAnimalsCount() < 6 && gYear == 1 && gSeason == AUTUMN && gDayOfMonth == 30 && (5 < gHour && gHour < 24)) {
                setLifeEventBit(0x8E);
                gCutsceneIndex = DOUG_FARM_FAVOR;
                loadCutscene(arg0);
                set = TRUE;    
            }
        }
        
        if (!set && arg0 == 0) {
            if (!checkLifeEventBit(0x8F) && checkLifeEventBit(0x94) && gSeason == WINTER && gDayOfMonth == 1 && (5 < gHour && gHour < 10) && gEntranceIndex == HOUSE_EXIT) {
                setLifeEventBit(0x8F);
                gCutsceneIndex = DOUG_DROPPING_OFF_COWS;
                loadCutscene(arg0);
                set = TRUE;    
            }
        }
        
        if (!set && arg0 == 0) {
            if (checkLifeEventBit(0x94) && gSeason == WINTER && gDayOfMonth == 5 && (5 < gHour && gHour < 10) && gEntranceIndex == HOUSE_EXIT) {
                toggleLifeEventBit(0x94);
                gCutsceneIndex = MISS_MANA_PICKUP_COWS;
                loadCutscene(arg0);
                set = TRUE;    
            }
        }      

        if (!set) {
            if (!checkLifeEventBit(0x68) && npcAffection[KENT] >= 50 && gSeason == SUMMER && (14 < gHour && gHour < 17)) {
                setLifeEventBit(0x68);
                gCutsceneIndex = 0x70;
                loadCutscene(arg0);
                set = TRUE;    
            }
        }
        
        if (!set) {
            if (!checkLifeEventBit(PUPPIES) && (npcAffection[KENT] >= 150 || npcAffection[KENT] + npcAffection[STU] >= 250) && (5 < gHour && gHour < 18)) {
                setLifeEventBit(PUPPIES);
                gCutsceneIndex = KENT_AND_STU_FARM_PUPPIES;
                loadCutscene(arg0);
                set = TRUE;    
            }
        }
        
        if (!set) {
            if (!checkLifeEventBit(0x6A) && getFarmMoondropFlowerCount() >= 8 && gWeather == SUNNY && (gSeason && gSeason < 3) && (5 < gHour && gHour < 12)) {
                setLifeEventBit(0x6A);
                gCutsceneIndex = 0x72;
                loadCutscene(arg0);
                set = TRUE;    
            }
        }        
        
        if (!set) {
            if (!checkLifeEventBit(0x6B) && getFarmPinkCatMintFlowersCount() && gWeather == SUNNY && (gSeason && gSeason < 3) && (5 < gHour && gHour < 12)) {
                setLifeEventBit(0x6B);
                gCutsceneIndex = 0x73;
                loadCutscene(arg0);
                set = TRUE;    
            }
        }        

        if (!set) {
            if (!checkLifeEventBit(0x6C) && getFarmPinkCatMintFlowersCount() >= 16 && npcAffection[STU] >= 80 && gWeather == SUNNY && (gSeason && gSeason < 3) && (5 < gHour && gHour < 12)) {
                setLifeEventBit(0x6C);
                gCutsceneIndex = 0x74;
                loadCutscene(arg0);
                set = TRUE;    
            }
        }      

        if (!set) {
            if (!checkLifeEventBit(0x6D) && npcAffection[MARIA] >= 50 && (14 < gHour && gHour < 17) && (!checkLifeEventBit(MARRIED) || gWife != MARIA)) {
                setLifeEventBit(0x6D);
                gCutsceneIndex = MARIA_FARM_GIFT_CHOCOLATE;
                loadCutscene(arg0);
                set = TRUE;    
            }
        }               
        
        if (!set) {
            if (!checkLifeEventBit(0x6E) && npcAffection[MARIA] >= 80 && (14 < gHour && gHour < 17) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
                setLifeEventBit(0x6E);
                gCutsceneIndex = MARIA_FARM_GIFT_RICE_BALLS;
                loadCutscene(arg0);
                set = TRUE;                               
            }
        }     

        if (!set) {
            if (!checkLifeEventBit(0x6F) && npcAffection[MARIA] >= 200 && gWeather == SUNNY && gSeason == SUMMER && NIGHTTIME && !checkDailyEventBit(FESTIVAL_DAY_TYPE_2) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
                setLifeEventBit(0x6F);
                gCutsceneIndex = 0x93;
                loadCutscene(arg0);
                set = TRUE;                               
            }
        }     
        
        if (!set) {
            if (!checkLifeEventBit(0x70) && npcAffection[POPURI] >= 50 && (14 < gHour && gHour < 17) && (!checkLifeEventBit(MARRIED) || gWife != POPURI)) {
                setLifeEventBit(0x70);
                gCutsceneIndex = POPURI_FARM_GIFT_TEA;
                loadCutscene(arg0);
                set = TRUE;                               
            }
        }          
        
        if (!set) {
            if (!checkLifeEventBit(0x71) && npcAffection[POPURI] >= 80 && (14 < gHour && gHour < 17) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
                setLifeEventBit(0x71);
                gCutsceneIndex = POPURI_FARM_GIFT_COOKIES;
                loadCutscene(arg0);
                set = TRUE;                               
            }
        }          
        if (!set) {
            
            if (!checkLifeEventBit(0x72) && blueMistFlowerPlot >= 0xC2 && npcAffection[POPURI] >= 200 && gWeather == SUNNY && gSeason == SUMMER && gDayOfMonth == 30 && (5 < gHour && gHour < 18) 
                    && ((checkLifeEventBit(MARRIED) && gWife == POPURI) || (!checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)))) {
                
                blueMistFlowerPlot = 0;
                pondFieldTiles[5][3] = 0;
                setLifeEventBit(0x72);
                gCutsceneIndex = 0x9E;
                loadCutscene(arg0);
                set = TRUE;           
                
            }
        } 

        if (!set) {
            if (!checkLifeEventBit(0x73) && npcAffection[ELLI] >= 50 && (14 < gHour && gHour < 17) && (!checkLifeEventBit(MARRIED) || gWife != ELLI)) {
                setLifeEventBit(0x73);
                gCutsceneIndex = 0xA5;
                loadCutscene(arg0);
                set = TRUE;                      
            }
        }
        
        if (!set) {
            if (!checkLifeEventBit(0x74) && npcAffection[ELLI] >= 80 && (14 < gHour && gHour < 17) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {
                setLifeEventBit(0x74);
                gCutsceneIndex = 0xA6;
                loadCutscene(arg0);
                set = TRUE;                      
            }
        }
        
        if (!set) {
            if (!checkLifeEventBit(0x75) && npcAffection[ELLI] >= 200 && gWeather == SUNNY && gSeason == AUTUMN && gDayOfMonth == 9 && NIGHTTIME && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {
                setLifeEventBit(0x75);
                gCutsceneIndex = 0xA9;
                loadCutscene(arg0);
                set = TRUE;                      
            }
        }      

        if (!set) {
            if (!checkLifeEventBit(0x76) && getChickenEggCount() >= 6 && gSeason == WINTER && gDayOfMonth == 9 && (5 < gHour && gHour < 24) && (!checkLifeEventBit(MARRIED) || gWife != ELLI)) {
                setLifeEventBit(0x76);
                gCutsceneIndex = 0xAA;
                loadCutscene(arg0);
                set = TRUE;                      
            }
        }  

        if (!set) {
            if (!checkLifeEventBit(0x77) && npcAffection[ANN] >= 50 && (14 < gHour && gHour < 17) && (!checkLifeEventBit(MARRIED) || gWife != ANN)) {
                setLifeEventBit(0x77);
                gCutsceneIndex = 0xB1;
                loadCutscene(arg0);
                set = TRUE;                      
            }
        }  

        if (!set) {
            if (!checkLifeEventBit(0x78) && npcAffection[ANN] >= 80 && (14 < gHour && gHour < 17) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
                setLifeEventBit(0x78);
                gCutsceneIndex = 0xB2;
                loadCutscene(arg0);
                set = TRUE;                      
            }
        }  
        
        if (!set) {
            if (!checkLifeEventBit(0x79) && npcAffection[ANN] >= 200 && gWeather == SUNNY && gSeason == WINTER && (5 < gHour && gHour < 18) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
                setLifeEventBit(0x79);
                gCutsceneIndex = 0xB5;
                loadCutscene(arg0);
                set = TRUE;                      
            }
        }  

        if (!set) {
            if (!checkLifeEventBit(0x7A) && npcAffection[ANN] >= 50 && (5 < gHour && gHour < 12) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && checkLifeEventBit(0x3C)) {
                setLifeEventBit(0x7A);
                gCutsceneIndex = 0xBC;
                loadCutscene(arg0);
                set = TRUE;                      
            }
        }  

        if (!set) {
            if (!checkLifeEventBit(0x7B) && npcAffection[ANN] >= 80 && (5 < gHour && gHour < 12) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && checkLifeEventBit(0x3C)) {
                setLifeEventBit(0x7B);
                gCutsceneIndex = 0xBD;
                loadCutscene(arg0);
                set = TRUE;                      
            }
        }  

        if (!set) {
            if (!checkLifeEventBit(0x7C) && npcAffection[KAREN] >= 60 && (1 < gSeason && gSeason < 4) && (14 < gHour && gHour < 17) && (!checkLifeEventBit(MARRIED) || gWife != KAREN)) {
                setLifeEventBit(0x7C);
                gCutsceneIndex = 0xBE;
                loadCutscene(arg0);
                set = TRUE;                      
            }
        }  
        
        if (!set) {
            if (!checkLifeEventBit(0x7D) && npcAffection[KAREN] >= 100 && (14 < gHour && gHour < 17) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
                setLifeEventBit(0x7D);
                gCutsceneIndex = 0xBF;
                loadCutscene(arg0);
                set = TRUE;                      
            }
        }  
        
        if (!set) {
            if (!checkLifeEventBit(0x7E) && checkLifeEventBit(0x96) && npcAffection[KAREN] >= 200 && gWeather == SUNNY && gSeason == AUTUMN && gDayOfMonth == 7 && NIGHTTIME && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
                setLifeEventBit(0x7E);
                gCutsceneIndex = 0xC1;
                loadCutscene(arg0);
                set = TRUE;                      
            }
        }  
        
        if (!set) {
            
            if (!checkLifeEventBit(0x7F) && checkLifeEventBit(0x96) && gSeason == WINTER && (6 < gDayOfMonth && gDayOfMonth < 31) && (5 < gHour && gHour < 18)) {
                
                setLifeEventBit(0x7F);
                setLifeEventBit(0x20);

                if (npcAffection[KAREN] >= npcAffection[KAI]) {
                    
                    if (!checkLifeEventBit(0x2F)) {
                        gCutsceneIndex = 0xC2;
                    } else {   
                        gCutsceneIndex = 0xD7;
                    }

                } else {

                    if (checkLifeEventBit(KAI_GONE)) {
                        gCutsceneIndex = 0xC2;
                    } else {
                        gCutsceneIndex = 0xD7;
                    }      
                }

                loadCutscene(arg0);
                set = TRUE;      

            }

        }          

        if (!set) {
            if (!checkLifeEventBit(0x80) && npcAffection[HARRIS] >= 50 && gSeason == AUTUMN && (14 < gHour && gHour < 17)) {
                setLifeEventBit(0x80);
                gCutsceneIndex = 0xCC;
                loadCutscene(arg0);
                set = TRUE;                      
            }
        }       
        
        if (!set) {
            if (!checkLifeEventBit(0x81) && npcAffection[CLIFF] >= 50 && (14 < gHour && gHour < 17)) {
                setLifeEventBit(0x81);
                gCutsceneIndex = 0xD2;
                loadCutscene(arg0);
                set = TRUE;                      
            }
        }       
        
        if (!set) {
            if (!checkLifeEventBit(0x83) && checkLifeEventBit(0x2A) && (5 < gHour && gHour < 18) && (!checkLifeEventBit(MARRIED) || gWife != KAREN) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
                setLifeEventBit(0x83);
                gCutsceneIndex = 0xDA;
                loadCutscene(arg0);
                set = TRUE;                      
            }
        }    

        if (!set && arg0 == 0) {
            if (!checkLifeEventBit(0x84) && checkLifeEventBit(6) && npcAffection[RICK] >= 120 && gWeather == SUNNY && (gSeason && gSeason < 4) && (5 < gHour && gHour < 18) && gEntranceIndex == HOUSE_EXIT) {
                setLifeEventBit(0x84);
                gCutsceneIndex = 0xDC;
                loadCutscene(arg0);
                set = TRUE;                             
            }
        }
        
        if (!set) {
            if (!checkLifeEventBit(0x85) && (5 < gHour && gHour < 12) && getTotalChickenCount() >= 5 && chickenFeedQuantity < 990) {
                setLifeEventBit(0x85);
                gCutsceneIndex = 0xE0;
                loadCutscene(arg0);
                set = TRUE;                             
            }
        }

        if (!set) {
            if (!checkLifeEventBit(0x86) && (gSeason && gSeason < 4) && (5 < gHour && gHour < 12) && gTotalCropsShipped >= 900 && chickenFeedQuantity < 990) {
                setLifeEventBit(0x86);
                gCutsceneIndex = 0xE1;
                loadCutscene(arg0);
                set = TRUE;                             
            }
        }
        
        if (!set && arg0 == 0) {
            if (!checkLifeEventBit(0x87) && (getFarmMoondropFlowerCount() + getFarmPinkCatMintFlowersCount()) >= 24 && gWeather == SUNNY && gSeason == SPRING && (5 < gHour && gHour < 18) && gEntranceIndex == HOUSE_EXIT) {
                setLifeEventBit(0x87);
                gCutsceneIndex = 0xE4;
                loadCutscene(arg0);
                set = TRUE;                             
            }
        }

        if (!set && arg0 == 0) {
            if (!checkLifeEventBit(0x88) && getFarmGrassTilesSum() >= 90 && gWeather == SUNNY && gSeason == AUTUMN && (5 < gHour && gHour < 18) && gEntranceIndex == HOUSE_EXIT) {
                setLifeEventBit(0x88);
                gCutsceneIndex = 0xE6;
                loadCutscene(arg0);
                set = TRUE;                             
            }
        }
        
        if (!set && arg0 == 0) {
            if (!checkLifeEventBit(0x89) && checkLifeEventBit(0x20) && gWeather == SUNNY && gSeason == AUTUMN && (5 < gHour && gHour < 18)) {
                setLifeEventBit(0x89);
                gCutsceneIndex = 0xE7;
                loadCutscene(arg0);
                set = TRUE;                             
            }
        }
        
        if (!set && arg0 == 0) {
            if (!checkLifeEventBit(0x8A) && checkLifeEventBit(0x53) && gWeather == SUNNY && gSeason == SPRING && (5 < gHour && gHour < 18) && gEntranceIndex == HOUSE_EXIT) {
                setLifeEventBit(0x8A);
                gCutsceneIndex = 0xE8;
                loadCutscene(arg0);
                set = TRUE;                             
            }
        }
        
        if (!set && arg0 == 0) {
            if (!checkLifeEventBit(0x8B) && checkLifeEventBit(0x30) && gWeather == SUNNY && getTotalCowsCount() >= 3 && gSeason == WINTER && (5 < gHour && gHour < 18) && gEntranceIndex == HOUSE_EXIT) {
                setLifeEventBit(0x8B);
                gCutsceneIndex = 0xE9;
                loadCutscene(arg0);
                set = TRUE;                             
            }
        }
        
        if (!set && arg0 == 0) {
            // get cow + grass count
            if (!checkLifeEventBit(0x8C) && checkLifeEventBit(0x30) && func_8009B828(1) && gWeather == SUNNY && gSeason == AUTUMN && (5 < gHour && gHour < 18) && gEntranceIndex == HOUSE_EXIT) {
                setLifeEventBit(0x8C);
                gCutsceneIndex = 0xEA;
                loadCutscene(arg0);
                set = TRUE;                             
            }
        }

        if (!set && arg0 == 0) {
            if (!checkLifeEventBit(0x8D) && (getFarmMoondropFlowerCount() + getFarmPinkCatMintFlowersCount()) >= 24 && gWeather == SUNNY && gSeason == SUMMER && (5 < gHour && gHour < 18) && gEntranceIndex == HOUSE_EXIT) {
                setLifeEventBit(0x8D);
                gCutsceneIndex = 0xEC;
                loadCutscene(arg0);
                set = TRUE;                             
            }
        }        

        if (!set) {
            if (checkLifeEventBit(0x90) && npcAffection[DOUG] >= 60 && (16 < gHour && gHour < 21) && ((gSeason == SPRING && gDayOfMonth == 18) || gSeason == AUTUMN && gDayOfMonth == 29)) {
                toggleLifeEventBit(0x90);
                gCutsceneIndex = 0x81;
                loadCutscene(arg0);
                set = TRUE;                             
            }
        }       
        
        if (!set) {
            if (checkLifeEventBit(0x91) && npcAffection[DOUG] >= 60 && gSeason == AUTUMN && gDayOfMonth == 5 && (16 < gHour && gHour < 21)) {
                toggleLifeEventBit(0x91);
                gCutsceneIndex = 0x82;
                loadCutscene(arg0);
                set = TRUE;                             
            }
        }          

        if (!set) {
            if (checkLifeEventBit(0x92) && npcAffection[BASIL] >= 60 && gSeason == SUMMER && gDayOfMonth == 10 && (16 < gHour && gHour < 21)) {
                toggleLifeEventBit(0x92);
                gCutsceneIndex = 0x8B;
                loadCutscene(arg0);
                set = TRUE;                             
            }
        }       
        
        if (!set) {
            if (!checkLifeEventBit(0x95) && checkLifeEventBit(0x2F) && npcAffection[KAI] <= 80 && gSeason == WINTER && (5 < gHour && gHour < 18) && gHarvestKing != 5 && gHarvestCoinFinder != 5 && (!checkLifeEventBit(MARRIED) || gWife != KAREN) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
                setLifeEventBit(0x95);
                setLifeEventBit(0x2E);
                setSpecialDialogueBit(0x24);
                gCutsceneIndex = 0xDB;
                loadCutscene(arg0);
                set = TRUE;                             
            }
        }      
        
        if (!set) {
            if (!checkDailyEventBit(0x51) && npcAffection[POTION_SHOP_DEALER] >= 50 && gWeather == SUNNY && gYear == 1 && gSeason == WINTER && gDayOfMonth == 27 && (5 < gHour && gHour < 9)) {
                setDailyEventBit(0x51);
                gCutsceneIndex = 0xC9;
                loadCutscene(arg0);
                set = TRUE;                             
            }
        } 

      if (!set) {
            if (!checkDailyEventBit(0x51) && npcAffection[HARRIS] >= 100 && gHarvestKing == 1 && gWeather == SUNNY && gSeason == WINTER && gDayOfMonth == 27 && (5 < gHour && gHour < 9)) {
                setDailyEventBit(0x51);
                gCutsceneIndex = 0xCA;
                loadCutscene(arg0);
                set = TRUE;                             
            }
        }    
        
      if (!set) {
            if (!checkDailyEventBit(0x51) && npcAffection[GRAY] >= 100 && gHarvestKing == 2 && gWeather == SUNNY && gSeason == WINTER && gDayOfMonth == 27 && (5 < gHour && gHour < 9)) {
                setDailyEventBit(0x51);
                gCutsceneIndex = 0xCD;
                loadCutscene(arg0);
                set = TRUE;                             
            }
        }    

      if (!set) {
            if (!checkDailyEventBit(0x51) && npcAffection[JEFF] >= 100 && gHarvestKing == 3 && gWeather == SUNNY && gSeason == WINTER && gDayOfMonth == 27 && (5 < gHour && gHour < 9)) {
                setDailyEventBit(0x51);
                gCutsceneIndex = 0xCF;
                loadCutscene(arg0);
                set = TRUE;                             
            }
        }    
        
    if (!set) {
        if (!checkDailyEventBit(0x51) && npcAffection[CLIFF] >= 100 && gHarvestKing == 4 && gWeather == SUNNY && gSeason == WINTER && gDayOfMonth == 27 && (5 < gHour && gHour < 9)) {
            setDailyEventBit(0x51);
            gCutsceneIndex = 0xD3;
            loadCutscene(arg0);
            set = TRUE;                             
        }
    }        
        
       if (!set) {
            if (!checkDailyEventBit(0x51) && npcAffection[KAI] >= 100 && gHarvestKing == 5 && gWeather == SUNNY && gSeason == WINTER && gDayOfMonth == 27 && (5 < gHour && gHour < 9)) {
                setDailyEventBit(0x51);
                gCutsceneIndex = 0xD8;
                loadCutscene(arg0);
                set = TRUE;                             
            }
        }      
            
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(MARIA_MARRIAGE_CUTSCENE) && gSeason == SUMMER && gDayOfMonth == 1 && gWeather == SUNNY && (17 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == MARIA) {
                setDailyEventBit(MARIA_MARRIAGE_CUTSCENE);
                gCutsceneIndex = 0xEF;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
            
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(POPURI_MARRIAGE_CUTSCENE) && gSeason == SUMMER && gDayOfMonth == 1 && gWeather == SUNNY && (17 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == POPURI) {
                setDailyEventBit(POPURI_MARRIAGE_CUTSCENE);
                gCutsceneIndex = 0xF0;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
            
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(ELLI_MARRIAGE_CUTSCENE) && gSeason == SUMMER && gDayOfMonth == 1 && gWeather == SUNNY && (17 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == ELLI) {
                setDailyEventBit(ELLI_MARRIAGE_CUTSCENE);
                gCutsceneIndex = 0xF1;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
            
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(ANN_MARRIAGE_CUTSCENE) && gSeason == SUMMER && gDayOfMonth == 1 && gWeather == SUNNY && (17 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == ANN) {
                setDailyEventBit(ANN_MARRIAGE_CUTSCENE);
                gCutsceneIndex = 0xF2;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
            
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(KAREN_MARRIAGE_CUTSCENE) && gSeason == SUMMER && gDayOfMonth == 1 && gWeather == SUNNY && (17 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == KAREN) {
                setDailyEventBit(KAREN_MARRIAGE_CUTSCENE);
                gCutsceneIndex = 0xF3;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
    
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(MARIA_MARRIAGE_CUTSCENE) && gSeason == WINTER && gDayOfMonth == 24 && gWeather == SUNNY && (17 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == MARIA) {
                setDailyEventBit(MARIA_MARRIAGE_CUTSCENE);
                gCutsceneIndex = 0xF4;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
    
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(POPURI_MARRIAGE_CUTSCENE) && gSeason == WINTER && gDayOfMonth == 24 && gWeather == SUNNY && (17 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == POPURI) {
                setDailyEventBit(POPURI_MARRIAGE_CUTSCENE);
                gCutsceneIndex = 0xF5;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
    
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(ELLI_MARRIAGE_CUTSCENE) && gSeason == WINTER && gDayOfMonth == 24 && gWeather == SUNNY && (17 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == ELLI) {
                setDailyEventBit(ELLI_MARRIAGE_CUTSCENE);
                gCutsceneIndex = 0xF6;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
    
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(ANN_MARRIAGE_CUTSCENE) && gSeason == WINTER && gDayOfMonth == 24 && gWeather == SUNNY && (17 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == ANN) {
                setDailyEventBit(ANN_MARRIAGE_CUTSCENE);
                gCutsceneIndex = 0xF7;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
    
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(KAREN_MARRIAGE_CUTSCENE) && gSeason == WINTER && gDayOfMonth == 24 && gWeather == SUNNY && (17 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == KAREN) {
                setDailyEventBit(KAREN_MARRIAGE_CUTSCENE);
                gCutsceneIndex = 0xF8;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
    
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(MARIA_MARRIAGE_CUTSCENE) && gSeason == SUMMER && gDayOfMonth == 7 && gWeather == SUNNY && (16 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == MARIA && npcAffection[MARIA] >= 220) {
                setDailyEventBit(MARIA_MARRIAGE_CUTSCENE);
                gCutsceneIndex = 0x96;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
            
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(POPURI_MARRIAGE_CUTSCENE) && gSeason == SUMMER && gDayOfMonth == 7 && gWeather == SUNNY && (16 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == POPURI && npcAffection[POPURI] >= 220) {
                setDailyEventBit(POPURI_MARRIAGE_CUTSCENE);
                gCutsceneIndex = 0xA1;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
    
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(ELLI_MARRIAGE_CUTSCENE) && gSeason == SUMMER && gDayOfMonth == 7 && gWeather == SUNNY && (16 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == ELLI && npcAffection[ELLI] >= 220) {
                setDailyEventBit(ELLI_MARRIAGE_CUTSCENE);
                gCutsceneIndex = 0xAD;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
    
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(ANN_MARRIAGE_CUTSCENE) && gSeason == SUMMER && gDayOfMonth == 7 && gWeather == SUNNY && (16 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == ANN && npcAffection[ANN] >= 220) {
                setDailyEventBit(ANN_MARRIAGE_CUTSCENE);
                gCutsceneIndex = 0xB8;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
    
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(KAREN_MARRIAGE_CUTSCENE) && gSeason == SUMMER && gDayOfMonth == 7 && gWeather == SUNNY && (16 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == KAREN && npcAffection[KAREN] >= 220) {
                setDailyEventBit(KAREN_MARRIAGE_CUTSCENE);
                gCutsceneIndex = 0xC5;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
            
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(MARIA_MARRIAGE_CUTSCENE) && gSeason == AUTUMN && gDayOfMonth == 9 && gWeather == SUNNY && (16 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == MARIA && npcAffection[MARIA] >= 220) {
                setDailyEventBit(MARIA_MARRIAGE_CUTSCENE);
                gCutsceneIndex = 0x97;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }       
    
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(POPURI_MARRIAGE_CUTSCENE) && gSeason == AUTUMN && gDayOfMonth == 9 && gWeather == SUNNY && (16 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == POPURI && npcAffection[POPURI] >= 220) {
                setDailyEventBit(POPURI_MARRIAGE_CUTSCENE);
                gCutsceneIndex = 0xA2;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }      
    
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(ELLI_MARRIAGE_CUTSCENE) && gSeason == AUTUMN && gDayOfMonth == 9 && gWeather == SUNNY && (16 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == ELLI && npcAffection[ELLI] >= 220) {
                setDailyEventBit(ELLI_MARRIAGE_CUTSCENE);
                gCutsceneIndex = 0xAE;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }        
            
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(ANN_MARRIAGE_CUTSCENE) && gSeason == AUTUMN && gDayOfMonth == 9 && gWeather == SUNNY && (16 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == ANN && npcAffection[ANN] >= 220) {
                setDailyEventBit(ANN_MARRIAGE_CUTSCENE);
                gCutsceneIndex = 0xB9;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }     
            
        if (!set && arg0 == 0) {
            if (!checkDailyEventBit(KAREN_MARRIAGE_CUTSCENE) && gSeason == AUTUMN && gDayOfMonth == 9 && gWeather == SUNNY && (16 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == KAREN && npcAffection[KAREN] >= 220) {
                setDailyEventBit(KAREN_MARRIAGE_CUTSCENE);
                gCutsceneIndex = 0xC6;
                loadCutscene(arg0);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }       

    }

    return result;
}


//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setFarmCutscenes);

u16 setFarmCutscenes(void) {

    u16 index = 0xFFFF;
    bool set = FALSE;
    u8 tempHappiness;

    if (checkDailyEventBit(0x52)) { 
        gCutsceneIndex = 0x101;
        gCutsceneFlags |= 2;
        loadCutscene(0);
        set = TRUE;
    }
    
    if (!set) {
        if (checkDailyEventBit(1)) {
            index = 1;
            set = TRUE;
            gCutsceneIndex = 0;
            gCutsceneFlags |= 2;
        }
    }
    
    if (!set) {
        if (checkDailyEventBit(2)) {
            
            if (checkDailyEventBit(3)) {
                gCutsceneIndex = 1;
            } else {
                gCutsceneIndex = 0;
                setDailyEventBit(3);
            }
            
            gCutsceneFlags |= 2;
            index = 2;
            set = TRUE;
        }
    }

    if (!set) {
        if (checkDailyEventBit(4)) {
            switch (selectedAnimalType) {         
                case CHICKEN_TYPE:
                    tempHappiness = adjustValue(gHappiness, -5, MAX_HAPPINESS);
                    gHappiness += tempHappiness;
                    gCutsceneIndex = 0;
                    break;
                case COW_TYPE:
                    tempHappiness = adjustValue(gHappiness, -10, MAX_HAPPINESS);
                    gHappiness += tempHappiness;
                    gCutsceneIndex = 1;
                    break;
                case SHEEP_TYPE:
                    tempHappiness = adjustValue(gHappiness, -10, MAX_HAPPINESS);
                    gHappiness += tempHappiness;
                    gCutsceneIndex = 2;
                    break;
            }
            index = 3;
            set = TRUE;
            gCutsceneFlags |= 2;
        }
    }

    if (!set) {
        if (checkDailyEventBit(0x1D) && !checkDailyEventBit(0x1E)) {
            index = 0x21;
            set = TRUE;
            gCutsceneIndex = 1;
            gCutsceneFlags |= 2;
        }
    }

    if (!set) {
        if (checkDailyEventBit(0x20)) {
            index = 0x22;
            set = TRUE;
            gCutsceneIndex = 1;
            gCutsceneFlags |= 2;
        }
    }

    if (!set) {
        if (checkDailyEventBit(8)) {
            gCutsceneIndex = handlePurchaseHouseExtension(gHouseExtensionSelection);
            gCutsceneFlags |= 2;
            index = 4;
            set = TRUE;
        }
    }

    if (!set) {
        if (checkLifeEventBit(0xD) && checkDailyEventBit(0xA) && !checkDailyEventBit(0xC)) {
            gCutsceneIndex = 1;
            index = 5;
            set = TRUE;
        }
    }

    if (!set) {
        if (checkDailyEventBit(0xB)) {
            gCutsceneIndex = 0;
            index = 6;
            set = TRUE;
        }
    }

    if (!set) {
        setFarmVisitCutscenes(0);
    }
    
    return index;

}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setMountain1Cutscenes);

u16 setMountain1Cutscenes(void) {

    bool set = FALSE;
    u16 index = 0xFFFF;

    if (!checkLifeEventBit(0x48) && gSeason == AUTUMN && (22 < gDayOfMonth && gDayOfMonth < 28) && (5 < gHour && gHour < 15)) {
        setDailyEventBit(0x21);
        if (checkDailyEventBit(0x2C)) {
            gCutsceneIndex = 0x145;
        } else {
            gCutsceneIndex = 0x144;
            gCutsceneFlags |= (2 | 4);
        }
        loadCutscene(0);
        set = TRUE;
    }

    if (!set) {
        if (!checkLifeEventBit(0x4C) && gSeason == WINTER && (11 < gDayOfMonth && gDayOfMonth < 17) && (5 < gHour && gHour < 15)) {
            setDailyEventBit(0x21);
            if (checkDailyEventBit(0x2C)) {
                gCutsceneIndex = 0x145;
            } else {
                gCutsceneIndex = 0x148;
                gCutsceneFlags |= (2 | 4);
            }
            loadCutscene(0);
            set = TRUE;  
        }
    }

    if (!set) {
        if (!checkHaveTool(FISHING_POLE)) {
            setDailyEventBit(0x21);
            gCutsceneIndex = 0x13E;
            loadCutscene(0);
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkLifeEventBit(0xA4) && npcAffection[JEFF] >= 180 && gWeather == SUNNY && gSeason == SUMMER && (5 < gHour && gHour < 18) && (!checkLifeEventBit(MARRIED) || gWife != ELLI) && !checkLifeEventBit(ELLI_JEFF_MARRIED) && gEntranceIndex == TOP_OF_MOUNTAIN_1_SUMMER) {
            setDailyEventBit(0x21);
            setLifeEventBit(0xA4);
            gCutsceneIndex = 0x134;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkLifeEventBit(0xA6) && npcAffection[MARIA] >= 150 && npcAffection[ELLI] >= 150 && gWeather == SUNNY && gSeason == SUMMER && (5 < gHour && gHour < 18) && gEntranceIndex == TOP_OF_MOUNTAIN_1_SUMMER) {
            setDailyEventBit(0x21);
            setLifeEventBit(0xA6);
            gCutsceneIndex = 0x136;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkLifeEventBit(0xA9) && npcAffection[POPURI] >= 150 && npcAffection[KAREN] >= 150 && gWeather == SUNNY && gSeason == SPRING && (5 < gHour && gHour < 18) && gEntranceIndex == 0x1A) {
            setDailyEventBit(0x21);
            setLifeEventBit(0xA9);
            gCutsceneIndex = 0x13A;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
            set = TRUE;
        }
    }

    if (!set) {
      if (!checkLifeEventBit(0xAD) && !checkDailyEventBit(FESTIVAL_DAY_TYPE_2) && npcAffection[ELLI] >= 180 && gWeather == SUNNY && (5 < gHour && gHour < 18) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {
            setDailyEventBit(0x21);
            setLifeEventBit(0xAD);
            setDailyEventBit(0x58);
            gCutsceneIndex = ELLI_SPRAINED_ANKLE;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
        }
    } 
    
    return index;

}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setMountain2Cutscenes);

u16 setMountain2Cutscenes(void) {

    bool set = FALSE;
    u16 index = 0xFFFF;

    if (!checkLifeEventBit(0xA0) && checkLifeEventBit(0x64) && checkLifeEventBit(0x48) && gWeather == SUNNY && 5 < gHour && gHour < 18 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
        setLifeEventBit(0xA0);
        gCutsceneIndex = 0x12D;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
        set = TRUE;
    }

    if (!set && !checkLifeEventBit(0xA1) && !checkDailyEventBit(0x30) && npcAffection[POPURI] >= 180 && gWeather == SUNNY && 5 < gHour && gHour < 18 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
        setLifeEventBit(0xA1);
        setDailyEventBit(0x57);
        gCutsceneIndex = 0x12E;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
        set = TRUE;
    }
    
    if (!set && !checkLifeEventBit(0xA2) && checkLifeEventBit(0x66) && gWeather == SUNNY && gSeason != WINTER && 5 < gHour && gHour < 18 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && gEntranceIndex == 0x1D) { 
        setLifeEventBit(0xA2);
        gCutsceneIndex = 0x132;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
        set = TRUE;
    }

    if (!set && !checkLifeEventBit(0xA3) && npcAffection[ANN] >= 80 && gWeather == SUNNY && gSeason == AUTUMN && 5 < gHour && gHour < 18) {
        setLifeEventBit(0xA3);
        gCutsceneIndex = 0x133;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
        set = TRUE;
    }

    if (!set && !checkLifeEventBit(0xA5) && npcAffection[CLIFF] >= 150 && gWeather == SUNNY && gSeason != WINTER && 5 < gHour && gHour < 18 && (!checkLifeEventBit(MARRIED) || gWife != ANN) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && gEntranceIndex == 0x1D) {
        setLifeEventBit(0xA5);
        gCutsceneIndex = 0x135;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
        set = TRUE;
    }

    if (!set && !checkLifeEventBit(0xA7) && npcAffection[MARIA] >= 150 && npcAffection[KAREN] >= 150 && gWeather == SUNNY && gSeason == SPRING && 5 < gHour && gHour < 18 && gEntranceIndex == 0x1D) {
        setLifeEventBit(0xA7);
        gCutsceneIndex = 0x137;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
        set = TRUE;
    }
    
    if (!set && !checkLifeEventBit(0xAA) && npcAffection[STU] >= 50 && npcAffection[MAY] >= 50 && gWeather == SUNNY && gSeason < AUTUMN && gDayOfWeek == SATURDAY && 5 < gHour && gHour < 18) {
        setLifeEventBit(0xAA);
        gCutsceneIndex = 0x152;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
        set = TRUE;
    }


    if (!set && !checkLifeEventBit(0x98) && !checkLifeEventBit(MARRIED) && npcAffection[POPURI] >= 250 && gWeather == SUNNY && 11 < gHour && gHour < 18 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
        setLifeEventBit(0x98);
        gCutsceneIndex = 0x156;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
        set = TRUE;
    }

    return index;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setTopOfMountain1Cutscenes);

// TODO: label; top of mountain 1
u16 setTopOfMountain1Cutscenes(void) {

    bool set = FALSE;
    u16 index = 0xFFFF;

    if (!checkLifeEventBit(0xA8) && checkLifeEventBit(0x4C) && npcAffection[POPURI] >= 150 && npcAffection[JEFF] >= 120 && gWeather == SUNNY && gSeason >= AUTUMN && (5 < gHour && gHour < 18) && (!checkLifeEventBit(MARRIED) || gWife != POPURI) && (!checkLifeEventBit(MARRIED) || gWife != ELLI) && !checkLifeEventBit(POPURI_GRAY_MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {
        setLifeEventBit(0xA8);
        gCutsceneIndex = 0x138;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
        set = TRUE;
    }

    if (!set) {
        if (!checkLifeEventBit(0xAB) && checkLifeEventBit(0x4C) && gWeather == SUNNY && gSeason == AUTUMN && (5 < gHour && gHour < 18)) {
            setLifeEventBit(0xAB);
            gCutsceneIndex = 0x153;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkLifeEventBit(0xAC) && gWeather == SUNNY && gSeason == WINTER && (5 < gHour && gHour < 18) && checkLifeEventBit(HAVE_KITCHEN) && checkLifeEventBit(HAVE_BATHROOM) && checkLifeEventBit(HAVE_STAIRS) && checkLifeEventBit(HAVE_GREENHOUSE) && checkLifeEventBit(HAVE_LOG_TERRACE) && checkLifeEventBit(HAVE_BABY_BED) && checkLifeEventBit(HOT_SPRINGS_COMPLETED)) {
            setLifeEventBit(0xAC);
            gCutsceneIndex = 0x154;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
        }
    }
    
    return index;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setMoonMountainCutscenes);

// mountain 2
u16 setMoonMountainCutscenes(void) {

    bool set = FALSE;
    u16 index = 0xFFFF;

    if (gSeason == SUMMER && gDayOfMonth == 1 && NIGHTTIME && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
        gCutsceneIndex = MOUNTAIN_FIREWORKS;
        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
        set = TRUE;
    }
    
    if (!set) {
        if (gSeason == WINTER && gDayOfMonth == 24 && NIGHTTIME && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
            gCutsceneIndex = STARRY_NIGHT_MOON_MOUNTAIN;
            loadCutscene(0);
            gCutsceneFlags |= (2 | 4);
        }
    }
    
    return index;
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setSpringCutscenes);

u16 setSpringCutscenes(void) {

    bool set = FALSE;
    u16 index = 0xFFFF;

    if (!checkLifeEventBit(0x9F) && blueMistFlowerPlot >= 0xC2 && gSeason < AUTUMN && (5 < gHour && gHour < 18)) {
        setLifeEventBit(0x9F);
        setDailyEventBit(0x46);
        gCutsceneIndex = 0x13B;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
        set = TRUE;
    }

    if (!set) {
        if (!checkLifeEventBit(0xD5) && checkLifeEventBit(0x9F) && blueMistFlowerPlot >= 0xC2 && gSeason < AUTUMN && (5 < gHour && gHour < 18)) {
            setLifeEventBit(0xD5);
            setDailyEventBit(0x46);
            gCutsceneIndex = 0x13C;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
            set = TRUE;
        }
    }

    if (!set) {
         if (!checkLifeEventBit(0xD6) && checkLifeEventBit(0xD5) && blueMistFlowerPlot >= 0xC2 && gSeason < AUTUMN && (5 < gHour && gHour < 18)) {
            setLifeEventBit(0xD6);
            setDailyEventBit(0x46);
            gCutsceneIndex = 0x13D;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
         }
    }
    
    return index;
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setHarvestSpriteCaveCutscenes);

u16 setHarvestSpriteCaveCutscenes(void) {

    u16 index = 0xFFFF;
    
    if (checkHaveKeyItem(MARBLES) && npcAffection[HARVEST_SPRITE_1] >= 80 && (5 < gHour && gHour < 18)) {
        gCutsceneIndex = 0x142;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
    }

    return index;

}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setCaveCutscenes);

u16 setCaveCutscenes(void) {

    u16 index = 0xFFFF;
    if (gSeason == WINTER && (7 < gDayOfMonth && gDayOfMonth < 31) && (5 < gHour && gHour < 16)) {
        gCutsceneIndex = 0x15A;
        gCutsceneFlags |= 2;
        loadCutscene(0);
    }

    return index;

}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setHouseCutscenes);

u16 setHouseCutscenes(void) {

    u16 index = 0xFFFF;
    bool set = FALSE;

    if (checkLifeEventBit(MARRIED) && !checkLifeEventBit(HAVE_BABY) && checkLifeEventBit(WIFE_PREGNANT) && gWifePregnancyCounter >= 60) {

        setDailyEventBit(0x52);
        gWeather = SUNNY;

        babyBirthdaySeason = gSeason;
        babyBirthdayDate = gDayOfMonth;

        switch (gWife) {
            
            case MARIA:
                gCutsceneIndex = MARIA_BIRTH;
                toggleSpecialDialogueBit(WIFE_PREGNANT_MARIA);
                break;
            case POPURI:
                gCutsceneIndex = POPURI_BIRTH;
                toggleSpecialDialogueBit(WIFE_PREGNANT_POPURI);
                break;
            case ELLI:
                gCutsceneIndex = ELLI_BIRTH;
                toggleSpecialDialogueBit(WIFE_PREGNANT_ELLI);
                break;
            case ANN:
                gCutsceneIndex = ANN_BIRTH;
                toggleSpecialDialogueBit(WIFE_PREGNANT_ANN);
                break;
            case KAREN:
                gCutsceneIndex = KAREN_BIRTH;
                toggleSpecialDialogueBit(WIFE_PREGNANT_KAREN);
                break;
            
        }
        
        loadCutscene(0);
        set = TRUE;
        gCutsceneFlags |= 2;
        
    }

    if (!set) {
        if (checkLifeEventBit(0x60) && !checkLifeEventBit(0x61) && !checkLifeEventBit(MARRIED) && npcAffection[KENT] >= 160 && !checkLifeEventBit(MARRIED)) {
            gCutsceneIndex = 1;
            loadCutscene(0);
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkLifeEventBit(0x62) && npcAffection[CLIFF] >= 30 && gWeather == SUNNY && (5 < gHour && gHour < 18) && (!checkLifeEventBit(MARRIED) || gWife != ANN) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
            gCutsceneIndex = 0x26;
            loadCutscene(0);
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkDailyEventBit(FARM_MORNING_VISIT) && !checkLifeEventBit(WIFE_LEFT) && (5 < gHour && gHour < 18) && checkLifeEventBit(MARRIED) && gWife == MARIA && npcAffection[MARIA] < 100) {
            setDailyEventBit(FARM_MORNING_VISIT);
            setLifeEventBit(WIFE_LEFT);
            gCutsceneIndex = MARIA_LEAVES_MARRIAGE;
            loadCutscene(0);
            gCutsceneFlags |= 2;
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkDailyEventBit(FARM_MORNING_VISIT) && !checkLifeEventBit(WIFE_LEFT) && (5 < gHour && gHour < 18) && checkLifeEventBit(MARRIED) && gWife == POPURI && npcAffection[POPURI] < 100) {
            setDailyEventBit(FARM_MORNING_VISIT);
            setLifeEventBit(WIFE_LEFT);
            gCutsceneIndex = POPURI_LEAVES_MARRIAGE;
            loadCutscene(0);
            gCutsceneFlags |= 2;
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkDailyEventBit(FARM_MORNING_VISIT) && !checkLifeEventBit(WIFE_LEFT) && (5 < gHour && gHour < 18) && checkLifeEventBit(MARRIED) && gWife == ELLI && npcAffection[ELLI] < 100) {
            setDailyEventBit(FARM_MORNING_VISIT);
            setLifeEventBit(WIFE_LEFT);
            gCutsceneIndex = ELLI_LEAVES_MARRIAGE;
            loadCutscene(0);
            gCutsceneFlags |= 2;
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkDailyEventBit(FARM_MORNING_VISIT) && !checkLifeEventBit(WIFE_LEFT) && (5 < gHour && gHour < 18) && checkLifeEventBit(MARRIED) && gWife == ANN && npcAffection[ANN] < 100) {
            setDailyEventBit(FARM_MORNING_VISIT);
            setLifeEventBit(WIFE_LEFT);
            gCutsceneIndex = ANN_LEAVES_MARRIAGE;
            loadCutscene(0);
            gCutsceneFlags |= 2;
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkDailyEventBit(FARM_MORNING_VISIT) && !checkLifeEventBit(WIFE_LEFT) && (5 < gHour && gHour < 18) && checkLifeEventBit(MARRIED) && gWife == KAREN && npcAffection[KAREN] < 100) {
            setDailyEventBit(FARM_MORNING_VISIT);
            setLifeEventBit(WIFE_LEFT);
            gCutsceneIndex = KAREN_LEAVES_MARRIAGE;
            loadCutscene(0);
            gCutsceneFlags |= 2;
            set = TRUE;
        }
    }
    
    return index;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setKitchenCutscenes);

u16 setKitchenCutscenes(void) {

    u16 index = 0xFFFF;
    bool set = FALSE;

    if (!checkDailyEventBit(MARIA_MARRIAGE_CUTSCENE) && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && checkLifeEventBit(HAVE_BABY) && !checkDailyEventBit(0x52) && gWife == MARIA && babyBirthdaySeason == gSeason && babyBirthdayDate == gDayOfMonth) {
        setDailyEventBit(MARIA_MARRIAGE_CUTSCENE);
        gCutsceneIndex = MARIA_BABY_BIRTHDAY;
        set = TRUE;
        loadCutscene(0);
        gCutsceneFlags |= 2;
        
    }

    if (!set && !checkDailyEventBit(POPURI_MARRIAGE_CUTSCENE) && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && checkLifeEventBit(HAVE_BABY) && !checkDailyEventBit(0x52) && gWife == POPURI && babyBirthdaySeason == gSeason && babyBirthdayDate == gDayOfMonth) {
        setDailyEventBit(POPURI_MARRIAGE_CUTSCENE);
        gCutsceneIndex = POPURI_BABY_BIRTHDAY;
        set = TRUE;
        loadCutscene(0);
        gCutsceneFlags |= 2;
    }

    if (!set && !checkDailyEventBit(ELLI_MARRIAGE_CUTSCENE) && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && checkLifeEventBit(HAVE_BABY) && !checkDailyEventBit(0x52) && gWife == ELLI && babyBirthdaySeason == gSeason && babyBirthdayDate == gDayOfMonth) {
        setDailyEventBit(ELLI_MARRIAGE_CUTSCENE);
        gCutsceneIndex = ELLI_BABY_BIRTHDAY;
        set = TRUE;
        loadCutscene(0);
        gCutsceneFlags |= 2;
    }

    if (!set && !checkDailyEventBit(ANN_MARRIAGE_CUTSCENE) && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && checkLifeEventBit(HAVE_BABY) && !checkDailyEventBit(0x52) && gWife == ANN && babyBirthdaySeason == gSeason && babyBirthdayDate == gDayOfMonth) {
        setDailyEventBit(ANN_MARRIAGE_CUTSCENE);
        gCutsceneIndex = ANN_BABY_BIRTHDAY;
        set = TRUE;
        loadCutscene(0);
        gCutsceneFlags |= 2;
    }

    if (!set && !checkDailyEventBit(KAREN_MARRIAGE_CUTSCENE) && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && checkLifeEventBit(HAVE_BABY) && !checkDailyEventBit(0x52) && gWife == KAREN && babyBirthdaySeason == gSeason && babyBirthdayDate == gDayOfMonth) {
        setDailyEventBit(KAREN_MARRIAGE_CUTSCENE);
        gCutsceneIndex = KAREN_BABY_BIRTHDAY;
        set = TRUE;
        loadCutscene(0);
        gCutsceneFlags |= 2;
    }

    if (!set && !checkDailyEventBit(MARIA_MARRIAGE_CUTSCENE) && gPlayerBirthdaySeason == gSeason && gDayOfMonth == 25 && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && gWife == MARIA && npcAffection[MARIA] >= 100) {
        setDailyEventBit(MARIA_MARRIAGE_CUTSCENE);
        gCutsceneIndex = MARIA_MARRIED_PLAYER_BIRTHDAY;
        set = TRUE;
        loadCutscene(0);
        gCutsceneFlags |= 2;
    }

    if (!set && !checkDailyEventBit(POPURI_MARRIAGE_CUTSCENE) && gPlayerBirthdaySeason == gSeason && gDayOfMonth == 25 && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && gWife == POPURI && npcAffection[POPURI] >= 100) {
        setDailyEventBit(POPURI_MARRIAGE_CUTSCENE);
        gCutsceneIndex = POPURI_MARRIED_PLAYER_BIRTHDAY;
        set = TRUE;
        loadCutscene(0);
        gCutsceneFlags |= 2;
    }

    if (!set && !checkDailyEventBit(ELLI_MARRIAGE_CUTSCENE) && gPlayerBirthdaySeason == gSeason && gDayOfMonth == 25 && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && gWife == ELLI && npcAffection[ELLI] >= 100) {
        setDailyEventBit(ELLI_MARRIAGE_CUTSCENE);
        gCutsceneIndex = ELLI_MARRIED_PLAYER_BIRTHDAY;
        set = TRUE;
        loadCutscene(0);
        gCutsceneFlags |= 2;
    }

    if (!set && !checkDailyEventBit(ANN_MARRIAGE_CUTSCENE) && gPlayerBirthdaySeason == gSeason && gDayOfMonth == 25 && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && gWife == ANN && npcAffection[ANN] >= 100) {
        setDailyEventBit(ANN_MARRIAGE_CUTSCENE);
        gCutsceneIndex = ANN_MARRIED_PLAYER_BIRTHDAY;
        set = TRUE;
        loadCutscene(0);
        gCutsceneFlags |= 2;
    }

    if (!set && !checkDailyEventBit(KAREN_MARRIAGE_CUTSCENE) && gPlayerBirthdaySeason == gSeason && gDayOfMonth == 25 && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && gWife == KAREN && npcAffection[KAREN] >= 100) {
        setDailyEventBit(KAREN_MARRIAGE_CUTSCENE);
        gCutsceneIndex = KAREN_MARRIED_PLAYER_BIRTHDAY;
        set = TRUE;
        loadCutscene(0);
        gCutsceneFlags |= 2;
    }

    if (!set && !checkDailyEventBit(MARIA_MARRIAGE_CUTSCENE) && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && gWife == MARIA && anniversarySeason == gSeason && anniversaryDate == gDayOfMonth) {
        setDailyEventBit(MARIA_MARRIAGE_CUTSCENE);
        gCutsceneIndex = MARIA_ANNIVERSARY;
        set = TRUE;
        loadCutscene(0);
        gCutsceneFlags |= 2;
    }

    if (!set && !checkDailyEventBit(POPURI_MARRIAGE_CUTSCENE) && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && gWife == POPURI && anniversarySeason == gSeason && anniversaryDate == gDayOfMonth) {
        setDailyEventBit(POPURI_MARRIAGE_CUTSCENE);
        gCutsceneIndex = POPURI_ANNIVERSARY;
        set = TRUE;
        loadCutscene(0);
        gCutsceneFlags |= 2;
    }

    if (!set && !checkDailyEventBit(ELLI_MARRIAGE_CUTSCENE) && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && gWife == ELLI && anniversarySeason == gSeason && anniversaryDate == gDayOfMonth) {
        setDailyEventBit(ELLI_MARRIAGE_CUTSCENE);
        gCutsceneIndex = ELLI_ANNIVERSARY;
        set = TRUE;
        loadCutscene(0);
        gCutsceneFlags |= 2;
    }

    if (!set && !checkDailyEventBit(ANN_MARRIAGE_CUTSCENE) && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && gWife == ANN && anniversarySeason == gSeason && anniversaryDate == gDayOfMonth) {
        setDailyEventBit(ANN_MARRIAGE_CUTSCENE);
        gCutsceneIndex = ANN_ANNIVERSARY;
        set = TRUE;
        loadCutscene(0);
        gCutsceneFlags |= 2;
    }

    if (!set && !checkDailyEventBit(KAREN_MARRIAGE_CUTSCENE) && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && gWife == KAREN && anniversarySeason == gSeason && anniversaryDate == gDayOfMonth) {
        setDailyEventBit(KAREN_MARRIAGE_CUTSCENE);
        gCutsceneIndex = KAREN_ANNIVERSARY;
        set = TRUE;
        loadCutscene(0);
        gCutsceneFlags |= 2;
    }
    
    if (!set && !checkDailyEventBit(MARIA_MARRIAGE_CUTSCENE) && gSeason == WINTER && gDayOfMonth == 10 && (16 < gHour && gHour < 20) && checkLifeEventBit(MARRIED) && gWife == MARIA) {
        setDailyEventBit(MARIA_MARRIAGE_CUTSCENE);
        gCutsceneIndex = MARIA_MARRIED_THANKSGIVING;
        set = TRUE;
        loadCutscene(0);
        gCutsceneFlags |= 2;
    }
        
    if (!set && !checkDailyEventBit(POPURI_MARRIAGE_CUTSCENE) && gSeason == WINTER && gDayOfMonth == 10 && (16 < gHour && gHour < 20) && checkLifeEventBit(MARRIED) && gWife == POPURI) {
        setDailyEventBit(POPURI_MARRIAGE_CUTSCENE);
        gCutsceneIndex = POPURI_MARRIED_THANKSGIVING;
        set = TRUE;
        loadCutscene(0);
        gCutsceneFlags |= 2;
    }

    if (!set && !checkDailyEventBit(ELLI_MARRIAGE_CUTSCENE) && gSeason == WINTER && gDayOfMonth == 10 && (16 < gHour && gHour < 20) && checkLifeEventBit(MARRIED) && gWife == ELLI) {
        setDailyEventBit(ELLI_MARRIAGE_CUTSCENE);
        gCutsceneIndex = ELLI_MARRIED_THANKSGIVING;
        set = TRUE;
        loadCutscene(0);
        gCutsceneFlags |= 2;
    }

    if (!set && !checkDailyEventBit(ANN_MARRIAGE_CUTSCENE) && gSeason == WINTER && gDayOfMonth == 10 && (16 < gHour && gHour < 20) && checkLifeEventBit(MARRIED) && gWife == ANN) {
        setDailyEventBit(ANN_MARRIAGE_CUTSCENE);
        gCutsceneIndex = ANN_MARRIED_THANKSGIVING;
        set = TRUE;
        loadCutscene(0);
        gCutsceneFlags |= 2;
    }

    if (!set && !checkDailyEventBit(KAREN_MARRIAGE_CUTSCENE) && gSeason == WINTER && gDayOfMonth == 10 && (16 < gHour && gHour < 20) && checkLifeEventBit(MARRIED) && gWife == KAREN) {
        setDailyEventBit(KAREN_MARRIAGE_CUTSCENE);
        gCutsceneIndex = KAREN_MARRIED_THANKSGIVING;
        set = TRUE;
        loadCutscene(0);
        gCutsceneFlags |= 2;
    }
    
    return index;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setRanchCutscenes);

u16 setRanchCutscenes(void) {

    u16 index = 0xFFFF;
    bool set = FALSE;

    if (gYear == 1 && gSeason == SPRING && gDayOfMonth >= 3 && gWeather == SUNNY && 5 < gHour && gHour < 17 && !checkLifeEventBit(5)) {

        if (!checkLifeEventBit(0xE)) {
            gCutsceneIndex = 0x28A;
        } else {
            gCutsceneIndex = 0x299;
        }

        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
        set = TRUE;
        
    }

    if (!set && gSeason == SUMMER && gDayOfMonth == 1 && 17 < gHour && gHour < 24 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(0x1D)) {

        gCutsceneIndex = 0x293;
        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
        set = TRUE;
    
    }

    if (!set && gSeason == AUTUMN && gDayOfMonth == 4 && 5 < gHour && gHour < 18) {

        if (gCowFestivalEnteredCowIndex!= 0xFF) {
            setDailyEventBit(0x49);
            // set cutscene wild card/score variable
            D_801886D2 = gFarmAnimals[gCowFestivalEnteredCowIndex].affection;
        } else {
            toggleDailyEventBit(0x49);
        }
        
        gCutsceneIndex = 0x47E;
        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
        set = TRUE;
    
    }

    if (!set && !checkDailyEventBit(0x1B) && gSeason == WINTER && gDayOfMonth == 10 && 5 < gHour && gHour < 18 && !checkLifeEventBit(MARRIED)) {

        setDailyEventBit(0x1B);
        gCutsceneIndex = 0x296;
        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
        set = TRUE;
    
    }
    
    if (!set && !checkLifeEventBit(0xBE) && npcAffection[GRAY] >= 150 && gWeather == SUNNY && gSeason < AUTUMN && 5 < gHour && gHour < 17 && (!checkLifeEventBit(MARRIED) || gWife != POPURI) && !checkLifeEventBit(0x1B)) {

        if (gEntranceIndex == 0x29) {
            gCutsceneIndex = 0x28C;
            setLifeEventBit(0xBE);
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
            set = TRUE;
        }
        
        if (gEntranceIndex == 0x2A) {
            gCutsceneIndex = 0x28D;
            setLifeEventBit(0xBE);
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
            set = TRUE;
        }
        
    }

    if (!set && !checkLifeEventBit(0xBF) && npcAffection[GRAY] >= 180 && gWeather == SUNNY && gSeason < WINTER && 17 < gHour && gHour < 24 && (!checkLifeEventBit(MARRIED) || gWife != POPURI) && !checkLifeEventBit(0x1B)) {

        if (gEntranceIndex == 0x29) {
            gCutsceneIndex = 0x28E;
            setLifeEventBit(0xBF);
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
            set = TRUE;
        }
        
        if (gEntranceIndex == 0x2A) {
            gCutsceneIndex = 0x28F;
            setLifeEventBit(0xBF);
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
            set = TRUE;
        }
        
    }

    if (!set && !checkLifeEventBit(0xC0) && npcAffection[POPURI] >= 150 && npcAffection[ANN] >= 150 && gWeather == SUNNY && gSeason == AUTUMN && 5 < gHour && gHour < 18 && (!checkLifeEventBit(MARRIED) || gWife != POPURI) && !checkLifeEventBit(0x1B)) {

        if (gEntranceIndex == 0x29) {
            gCutsceneIndex = 0x291;
            setLifeEventBit(0xC0);
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
            set = TRUE;
        }
        
        if (gEntranceIndex == 0x2A) {
            gCutsceneIndex = 0x292;
            setLifeEventBit(0xC0);
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
            set = TRUE;
        }
        
    }
    
    if (!set && !checkLifeEventBit(0xC1) && npcAffection[CLIFF] >= 100 && gWeather == SUNNY && gSeason < WINTER && 5 < gHour && gHour < 18 && (!checkLifeEventBit(MARRIED) || gWife != ANN) && !checkLifeEventBit(0x1D)) {

        setLifeEventBit(0xC1);
        gCutsceneIndex = 0x294;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
        set = TRUE;

    }

    if (!set && !checkLifeEventBit(0x9B) && !checkLifeEventBit(MARRIED) && npcAffection[ANN] >= 250 && gWeather == SUNNY && 14 < gHour && gHour < 24 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(0x1D) && gEntranceIndex == 0x29) {

        setLifeEventBit(0x9B);
        gCutsceneIndex = 0x295;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
        set = TRUE;

    }

    return index;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setRanchStoreCutscenes);

u16 setRanchStoreCutscenes(void) {

    u16 index = 0xFFFF;
     
    if (!(checkLifeEventBit(ANN_SICK_VISIT)) && !checkDailyEventBit(FESTIVAL_DAY_TYPE_2) && npcAffection[ANN] >= 120 && (2 <= gWeather && gWeather < 4) && gDayOfWeek == SUNDAY && (5 < gHour && gHour < 18) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
        setLifeEventBit(ANN_SICK_VISIT);
        setDailyEventBit(0x59);
        gCutsceneIndex = ANN_SICK_VISIT_CUTSCENE;
        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
    }
     
    if (checkLifeEventBit(MARRIED) && gWife == ANN && checkLifeEventBit(WIFE_LEFT)) {
        gCutsceneIndex = ANN_REPAIR_MARRIAGE;
        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
    }

    return index;

}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setVineyardCutscenes);

u16 setVineyardCutscenes(void) {

    u16 index = 0xFFFF;
    bool set = FALSE;

    if (!checkLifeEventBit(0xC3) && (5 < gHour && gHour < 18) && (!checkLifeEventBit(MARRIED) || gWife != 4) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
        setLifeEventBit(0xC3);
        gCutsceneIndex = 0x2BC;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
        set = TRUE;
    }

    if (!set) {
        if (!checkLifeEventBit(0xC4) && !checkDailyEventBit(FESTIVAL_DAY_TYPE_2) && npcAffection[KAREN] >= 180 && gWeather == SUNNY && (5 < gHour && gHour < 18) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            setLifeEventBit(0xC4);
            setDailyEventBit(0x5A);
            gCutsceneIndex = 0x2BD;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkLifeEventBit(0xC5) && npcAffection[ANN] >= 120 && npcAffection[KAREN] >= 150 && gWeather == SUNNY && gSeason == WINTER && (5 < gHour && gHour < 18) && (!checkLifeEventBit(MARRIED) || gWife != KAREN) && !checkLifeEventBit(KAREN_KAI_MARRIED) && previousMapIndex == 0x4E) {
            setLifeEventBit(0xC5);
            gCutsceneIndex = 0x2BE;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkLifeEventBit(0xC6) && npcAffection[KAI] >= 50 && npcAffection[KAREN] >= 50 && (5 < gHour && gHour < 18) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED) && previousMapIndex == 0x4E) {
            setLifeEventBit(0xC6);
            gCutsceneIndex = 0x2BF;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
        }
    }

    return index;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setVineyardHouseCutscenes);

u16 setVineyardHouseCutscenes(void) {
    
    u16 index = 0xFFFF;
    
    if (!checkLifeEventBit(KAREN_SICK_VISIT) && !checkDailyEventBit(FESTIVAL_DAY_TYPE_2) && npcAffection[KAREN] >= 120U && (2 <= gWeather && gWeather < 4) && (5 < gHour && gHour < 18) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
        setLifeEventBit(KAREN_SICK_VISIT);
        setDailyEventBit(0x5A);
        gCutsceneIndex = KAREN_SICK;
        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
    }
    
    if (checkLifeEventBit(MARRIED) && gWife == KAREN && checkLifeEventBit(WIFE_LEFT)) {
        gCutsceneIndex = 0x2C8;
        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
    }
    
    return index;
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setVineyardCellarCutscenes);

u16 setVineyardCellarCutscenes(void) {
    
    bool set;
    u16 index = 0xFFFF;

    set = FALSE;
    
    if (!checkLifeEventBit(KAREN_YELLOW_HEART_EVENT_CUTSCENE) && checkLifeEventBit(0x67) && (5 < gHour && gHour < 18) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED) && previousMapIndex == VINEYARD) {
               
        setLifeEventBit(KAREN_YELLOW_HEART_EVENT_CUTSCENE);
        setDailyEventBit(0x4F);

        gCutsceneIndex = KAREN_YELLOW_HEART_EVENT_1;
        gCutsceneFlags |= (2 | 4);

        loadCutscene(0);

        set = TRUE;

    }
    
    if (!set && checkDailyEventBit(0x4F) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED) && previousMapIndex == VINEYARD_CELLAR_BASEMENT) {
        gCutsceneIndex = KAREN_YELLOW_HEART_EVENT_4;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
    }
    
    return index;
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setVillage1Cutscenes);

u16 setVillage1Cutscenes(void) {

    u16 index = 0xFFFF;
    bool set = FALSE;

    if (!checkLifeEventBit(0x9E) && checkLifeEventBit(2) && checkHaveTool(0x13)) {
        setLifeEventBit(0x9E);
        gCutsceneIndex = 0x19A;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
        set = TRUE;
    } 

    if (!set && gSeason == SUMMER && gDayOfMonth == 9 && 5 < gHour && gHour < 18) {
        gCutsceneIndex = 0x3E8;
        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
        set = TRUE;
    } 
    
    if (!set && gSeason == AUTUMN && gDayOfMonth == 20 && 5 < gHour && gHour < 18) {
        gCutsceneIndex = 0x4E2;
        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
        set = TRUE;
    } 

    if (!set && !checkDailyEventBit(0x19) && gSeason == WINTER && gDayOfMonth == 10 && 5 < gHour && gHour < 18 && !checkLifeEventBit(MARRIED)) {
        setDailyEventBit(0x19);
        gCutsceneIndex = 0x1A8;
        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
        set = TRUE;
    } 


    if (!set && !checkLifeEventBit(0xAE) && npcAffection[MARIA] >= 150 && npcAffection[POPURI] >= 150 && gWeather == SUNNY && gSeason == SPRING && 5 < gHour && gHour < 18) {

        if (previousMapIndex == 0x4E) {
            
            gCutsceneIndex = 0x19C;
            setLifeEventBit(0xAE);
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
            set = TRUE;
            
        }
        
        if (previousMapIndex == 0x2D) {

            if (gEntranceIndex == 0x33) {
                gCutsceneIndex = 0x19D;
            } else {
                gCutsceneIndex = 0x19E;
            }

            setLifeEventBit(0xAE);
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
            set = TRUE;
            
        }
        
    } 
    
    if (!set && !checkLifeEventBit(0xAF) && npcAffection[GRAY] >= 120 && gWeather == SUNNY && gSeason < AUTUMN && 5 < gHour && gHour < 18 && (!checkLifeEventBit(MARRIED) || gWife != POPURI) && !checkLifeEventBit(0x1B)) {
        setLifeEventBit(0xAF);
        gCutsceneIndex = 0x19F;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
        set = TRUE;
    } 

    if (!set && !checkLifeEventBit(0x31) && npcAffection[ELLEN] >= 30 && gWeather == SUNNY && gYear == 2 && gSeason == SPRING && 5 < gHour && gHour < 18) {
        gCutsceneIndex = 0x1A0;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
        set = TRUE;
    } 
    
    return index;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setVillage2Cutscenes);

// village 2
u16 setVillage2Cutscenes(void) {

    u16 index = 0xFFFF;
    bool set = FALSE;

    if (!checkLifeEventBit(0xB6) && (npcAffection[MAYOR] + npcAffection[MAYOR_WIFE] >= 200) && npcAffection[MARIA] >= 150 && 5 < gHour && gHour < 18) {

        if (previousMapIndex == 0x4E) {

            setLifeEventBit(0xB6);
            gCutsceneIndex = 0x1F9;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
            set = TRUE;
            
        }

        if (previousMapIndex == 0x29) {

            if (gEntranceIndex == 0x3F) {
                gCutsceneIndex = 0x1FA;
            } else {
                gCutsceneIndex = 0x1FB;
            }

            setLifeEventBit(0xB6);
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
            set = TRUE;
            
        }
        
    }

    if (!set && !checkLifeEventBit(0xB7) && npcAffection[HARRIS] >= 160 && gWeather == SUNNY && 5 < gHour && gHour < 18 && (!checkLifeEventBit(MARRIED) || gWife != MARIA) && !checkLifeEventBit(0x1A)) {

        if (previousMapIndex == 0x4E) {

            setLifeEventBit(0xB7);
            gCutsceneIndex = 0x1FC;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
            set = TRUE;
            
        }

        if (previousMapIndex == 0x29) {

            if (gEntranceIndex == 0x3F) {
                gCutsceneIndex = 0x1FD;
            } else {
                gCutsceneIndex = 0x1FE;
            }

            setLifeEventBit(0xB7);
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
            set = TRUE;
            
        }
    
    }

    if (!set && !checkLifeEventBit(0xB8) && npcAffection[HARRIS] >= 180 && gSeason < AUTUMN && gWeather == SUNNY && 5 < gHour && gHour < 18 && (!checkLifeEventBit(MARRIED) || gWife != MARIA) && !checkLifeEventBit(0x1A)) {

        if (previousMapIndex == 0x4E) {

            setLifeEventBit(0xB8);
            gCutsceneIndex = 0x1FF;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
            set = TRUE;
            
        }

        if (previousMapIndex == 0x29) {

            if (gEntranceIndex == 0x3F) {
                gCutsceneIndex = 0x200;
            } else {
                gCutsceneIndex = 0x201;
            }

            setLifeEventBit(0xB8);
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
            set = TRUE;
            
        }
    
    }

    if (!set && !checkLifeEventBit(0xB9) && npcAffection[MARIA] >= 150 && npcAffection[ANN] >= 150 && gSeason == AUTUMN && 5 < gHour && gHour < 18) {

        if (previousMapIndex == 0x4E) {

            setLifeEventBit(0xB9);
            gCutsceneIndex = 0x202;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
            set = TRUE;
            
        }

        if (previousMapIndex == 0x29) {

            if (gEntranceIndex == 0x3F) {
                gCutsceneIndex = 0x203;
            } else {
                gCutsceneIndex = 0x204;
            }

            setLifeEventBit(0xB9);
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
            set = TRUE;
            
        }
    
    }

    if (!set && !checkLifeEventBit(0xBA) && checkLifeEventBit(0x65) && gWeather == SUNNY && 5 < gHour && gHour < 17 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(0x1C)) {

        if (previousMapIndex == 0x4E) {

            setLifeEventBit(0xBA);
            gCutsceneIndex = 0x206;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
            set = TRUE;
            
        }

        if (previousMapIndex == 0x29) {

            if (gEntranceIndex == 0x3F) {
                gCutsceneIndex = 0x207;
            } else {
                gCutsceneIndex = 0x208;
            }

            setLifeEventBit(0xBA);
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
            set = TRUE;
            
        }
    
    }

    if (!set && !checkLifeEventBit(0xD0) && checkLifeEventBit(0x1F) && gWife == MARIA && npcAffection[MIDWIFE] >= 80 && gWifePregnancyCounter == 9 && gWeather == SUNNY && 5 < gHour && gHour < 18) {

        setLifeEventBit(0xD0);
        gCutsceneIndex = 0x1F4;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
        set = TRUE;
        
    }

    if (!set && !checkLifeEventBit(0xD1) && checkLifeEventBit(0x1F) && gWife == POPURI && npcAffection[MIDWIFE] >= 80 && gWifePregnancyCounter == 9 && gWeather == SUNNY && 5 < gHour && gHour < 18) {

        setLifeEventBit(0xD1);
        gCutsceneIndex = 0x1F5;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
        set = TRUE;
        
    }

    if (!set && !checkLifeEventBit(0xD2) && checkLifeEventBit(0x1F) && gWife == ELLI && npcAffection[MIDWIFE] >= 80 && gWifePregnancyCounter == 9 && gWeather == SUNNY && 5 < gHour && gHour < 18) {

        setLifeEventBit(0xD2);
        gCutsceneIndex = 0x1F6;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
        set = TRUE;
        
    }

    if (!set && !checkLifeEventBit(0xD3) && checkLifeEventBit(0x1F) && gWife == ANN && npcAffection[MIDWIFE] >= 80 && gWifePregnancyCounter == 9 && gWeather == SUNNY && 5 < gHour && gHour < 18) {

        setLifeEventBit(0xD3);
        gCutsceneIndex = 0x1F7;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
        set = TRUE;
        
    }

    if (!set && !checkLifeEventBit(0xD4) && checkLifeEventBit(0x1F) && gWife == KAREN && npcAffection[MIDWIFE] >= 80 && gWifePregnancyCounter == 9 && gWeather == SUNNY && 5 < gHour && gHour < 18) {

        setLifeEventBit(0xD4);
        gCutsceneIndex = 0x1F8;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
        set = TRUE;
        
    }

    if (!set && !checkLifeEventBit(0x9A) && !checkLifeEventBit(MARRIED) && npcAffection[MARIA] >= 250 && gWeather == SUNNY && 11 < gHour && gHour < 18 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(0x1A) && previousMapIndex == 0x3C) {
        setLifeEventBit(0x9A);
        gCutsceneIndex = 0x205;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
    } 
    
    return index;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setRickStoreCutscenes);

u16 setRickStoreCutscenes(void) {
    
    u16 index = 0xFFFF;
    
    if (!checkLifeEventBit(ANN_RICK_CUTSCENE) && npcAffection[RICK] >= 40 && (5 < gHour && gHour < 18)) {
        setLifeEventBit(ANN_RICK_CUTSCENE);
        gCutsceneIndex = ANN_AND_RICK;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
    }
    
    return index;

}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setFlowerShopCutscenes);

u16 setFlowerShopCutscenes(void) {
    
    u16 index = 0xFFFF;
    
    if (!checkLifeEventBit(POPURI_SICK_CUTSCENE) && !checkDailyEventBit(FESTIVAL_DAY_TYPE_2) && npcAffection[POPURI] >= 120 && (2 <= gWeather && gWeather < 4) && (5 < gHour && gHour < 18) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
        setLifeEventBit(POPURI_SICK_CUTSCENE);
        setDailyEventBit(0x57);
        gCutsceneIndex = POPURI_SICK;
        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
    }
    
    if (checkLifeEventBit(MARRIED) && gWife == POPURI && checkLifeEventBit(WIFE_LEFT)) {
        gCutsceneIndex = 0x1BC;
        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
    }
    
    return index;

}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setBakeryCutscenes);

u16 setBakeryCutscenes(void) {

    bool set = FALSE;
    u16 index = 0xFFFF;

    if (gSeason == SUMMER && gDayOfMonth == 1 && 17 < gHour && gHour < 24 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(0x1C)) {

        if (!checkLifeEventBit(0x31)) {
            gCutsceneIndex = 0x1A2;
        } else {
            gCutsceneIndex = 0x1A4;
        }

        loadCutscene(0);
        set = TRUE;
        gCutsceneFlags |= (2 | 4);
        
    }

    if (!set && !checkDailyEventBit(0x1A) && gSeason == WINTER && gDayOfMonth == 10 && 5 < gHour && gHour < 18 && !checkLifeEventBit(MARRIED)) {

        setDailyEventBit(0x1A);
        gCutsceneIndex = 0x1AB;
        loadCutscene(0);
        set = TRUE;
        gCutsceneFlags |= (2 | 4);
        
    }

    if (!set && !checkLifeEventBit(0xB2) && npcAffection[ELLI] >= 150 && npcAffection[ANN] >= 150 && 5 < gHour && gHour < 18 && (gSeason == AUTUMN && gDayOfMonth >= 15 || gSeason == WINTER && gDayOfMonth < 10)) {

        setLifeEventBit(0xB2);
        gCutsceneIndex = 0x1B0;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
        set = TRUE;
        
    }

    if (!set && !checkLifeEventBit(0xB3) && !checkDailyEventBit(0x30) && npcAffection[JEFF] >= 150 && 5 < gHour && gHour < 18  && (!checkLifeEventBit(MARRIED) || gWife != ELLI) && !checkLifeEventBit(0x1C) && previousMapIndex == 0x29) {

        setLifeEventBit(0xB3);
        
        if (gEntranceIndex == 0x59) {
            gCutsceneIndex = 0x1B1;
        } else {
            gCutsceneIndex = 0x1B2;
        }
        
        loadCutscene(0);
        set = TRUE;
        gCutsceneFlags |= (2 | 4);
        
    }
    
    if (!set && !checkLifeEventBit(0x97) && !checkDailyEventBit(0x30) && npcAffection[ELLI] >= 120 && (SUNNY < gWeather && gWeather < 4) && 5 < gHour && gHour < 18  && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(0x1C)) {

        setLifeEventBit(0x97);
        setDailyEventBit(0x58);
        gCutsceneIndex = 0x1BA;
        loadCutscene(0);
        set = TRUE;
        gCutsceneFlags |= (2 | 4);
        
    }

    if (!set && !checkLifeEventBit(0x99) && !checkLifeEventBit(MARRIED) && npcAffection[ELLI] >= 250 && gWeather == SUNNY && 11 < gHour && gHour < 17 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(0x1C)) {

        setLifeEventBit(0x99);
        gCutsceneIndex = 0x1aa;
        set = TRUE;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
        
    }

    if (checkLifeEventBit(MARRIED) && gWife == ELLI && checkLifeEventBit(0xD9)) {
        gCutsceneIndex = 0x1Bd;
        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
    }

    return index;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setMayorHouseCutscenes);

u16 setMayorHouseCutscenes(void) {

    u16 index = 0xFFFF;
    bool set = FALSE;

    if (!checkDailyEventBit(MARIA_MARRIAGE_CUTSCENE) && gSeason == WINTER && gDayOfMonth == 10 && (5 < gHour && gHour < 18) && !checkLifeEventBit(MARRIED)) {
        setDailyEventBit(MARIA_MARRIAGE_CUTSCENE);
        gCutsceneIndex = THANKSGIVING_MAYOR_HOUSE;
        set = TRUE;
        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
    }

    if (!set && !checkLifeEventBit(MARIA_SICK_CUTSCENE) && !checkDailyEventBit(FESTIVAL_DAY_TYPE_2) && npcAffection[MARIA] >= 120 && (SUNNY < gWeather && gWeather < 4) && gDayOfWeek == MONDAY && (8 < gHour && gHour < 17) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
        setLifeEventBit(MARIA_SICK_CUTSCENE);
        setDailyEventBit(MARIA_ABSENT);
        gCutsceneIndex = MARIA_SICK;
        set = TRUE;
        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
    }

    if (checkLifeEventBit(MARRIED) && gWife == MARIA && checkLifeEventBit(WIFE_LEFT)) {
        gCutsceneIndex = MARIA_REPAIR_MARRIAGE;
        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
    }

    return index;

}    

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setLibraryCutscenes);

u16 setLibraryCutscenes(void) {

    u16 index = 0xFFFF;
    bool set = FALSE;

    if (!checkLifeEventBit(MARIA_SPRAINED_ANKLE_CUTSCENE) && !checkDailyEventBit(FESTIVAL_DAY_TYPE_2) && npcAffection[MARIA] >= 180 && (8 < gHour && gHour < 17) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
        setLifeEventBit(MARIA_SPRAINED_ANKLE_CUTSCENE);
        setDailyEventBit(MARIA_ABSENT);
        gCutsceneIndex = MARIA_SPRAINED_ANKLE;
        set = TRUE;
        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
    }

    if (!set) {
        if (!checkLifeEventBit(0xBD) && checkLifeEventBit(0x63) && (8 < gHour && gHour < 17) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
            setLifeEventBit(0xBD);
            gCutsceneIndex = 0x20F;
            loadCutscene(0);
            gCutsceneFlags |= (2 | 4);
        }
    }
    
    return index;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setChurchCutscenes);

u16 setChurchCutscenes(void) {

    bool set = FALSE;
    u16 index = 0xFFFF;

    if (gSeason == WINTER && gDayOfMonth == 24 && NIGHTTIME && !checkLifeEventBit(MARRIED)) {     
        gCutsceneIndex = STARRY_NIGHT_FESTIVAL;
        loadCutscene(0);
        set = TRUE;
        gCutsceneFlags |= (2 | 4);
    } 
    
    if (!set) {
        if (gSeason == WINTER && gDayOfMonth == 30 && NIGHTTIME) {
            gCutsceneIndex = CHURCH_NEW_YEARS;
            loadCutscene(0);
            gCutsceneFlags |= (2 | 4);
        }    
    } 
    
    return index;
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setTavernCutscenes);

u16 setTavernCutscenes(void) {

    u16 index = 0xFFFF;
    bool set = FALSE;

    if (gSeason == WINTER && gDayOfMonth == 30 && NIGHTTIME) {
        gCutsceneIndex = TAVERN_NEW_YEARS;
        loadCutscene(0);
        set = TRUE;
        gCutsceneFlags |= (2 | 4);
    }

    if (!set && !checkLifeEventBit(KAREN_KAI_TAVERN_CUTSCENE) && npcAffection[KAREN] >= 80 && npcAffection[KAI] >= 80 && NIGHTTIME && (!checkLifeEventBit(MARRIED) || gWife != KAREN) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
        setLifeEventBit(KAREN_KAI_TAVERN_CUTSCENE);
        setSpecialDialogueBit(KAREN_KAI_TAVERN_CUTSCENE_DIALOGUE);
        gCutsceneIndex = KAREN_KAI_TAVERN;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
    }

    return index;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setSquareCutscenes);

u16 setSquareCutscenes(void) {

    u16 index = 0xFFFF;
    bool set = FALSE;

    if (gSeason == SPRING && gDayOfMonth == 8 && 5 < gHour && gHour < 18) {

        if (gYear == 1) {
            gCutsceneIndex = 0x352;
        } else {
            if (gHarvestKing == PLAYER) {
                gCutsceneIndex = 0x353;
            } else {
                gCutsceneIndex = 0x354;
            }
        }
        
        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
        set = TRUE;
        
    }

    if (!set && gSeason == SPRING && gDayOfMonth == 23 && 5 < gHour && gHour < 18) {

        gCutsceneIndex = 0x3B6;
        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
        set = TRUE;
        
    }

    if (!set && gSeason == SUMMER && gDayOfMonth == 1 && 17 < gHour && gHour < 24 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(0x1A)) {

        gCutsceneIndex = 0x258;
        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
        set = TRUE;
        
    }

    if (!set && gSeason == AUTUMN && gDayOfMonth == 12 && 5 < gHour && gHour < 18) {

        setDailyEventBit(0x5D);
        setHarvestKingName(gHarvestCoinFinder);
        
        gCutsceneIndex = 0x4B0;
        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
        set = TRUE;
        
    }

    if (!set && gSeason == WINTER && gDayOfMonth == 27 && 17 < gHour && gHour < 24) {

        if (gHarvestKing == PLAYER) {
            gCutsceneIndex = 0x546;
        } else {
            if (!checkDailyEventBit(0x50)) {
                gCutsceneIndex = 0x547;
            } else {
                gCutsceneIndex = 0x548;
            }
        }
    
        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
        set = TRUE;
        
    }


    if (!set && gSeason == SPRING && gDayOfMonth == 1 && 5 < gHour && gHour < 18) {
        gCutsceneIndex = 0x578;
        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
        set = TRUE;
    }

    if (!set && gSeason == SPRING && gDayOfMonth == 17 && 7 < gHour && gHour < 18) {
        updateHorseRaceContext();
        gCutsceneIndex = 0x384;
        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
        set = TRUE;
    }
    
    if (!set && gSeason == AUTUMN && gDayOfMonth == 28 && 7 < gHour && gHour < 18) {
        updateHorseRaceContext();
        gCutsceneIndex = 0x384;
        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
        set = TRUE;
    }

    if (!set && gSeason == WINTER && gDayOfMonth == 19 && 7 < gHour && gHour < 18) {
        updateDogRaceContext();
        gCutsceneIndex = 0x514;
        loadCutscene(0);
        gCutsceneFlags |= (2 | 4);
    }
    
    return index;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setRaceTrackCutscenes);

u16 setRaceTrackCutscenes(void) {

    u16 index = 0xFFFF;
    bool set = FALSE;

    if (checkDailyEventBit(0x40) && checkDailyEventBit(0x41)) {
        updateHorseRaceContext();
        toggleDailyEventBit(0x40);
        gCutsceneIndex = HORSE_RACE_STARTING_ANNOUNCEMENT;
        loadCutscene(0);
        set = TRUE;
        gCutsceneFlags |= (2 | 4);
    }

    if (!set) {
        if (checkDailyEventBit(0x40) && checkDailyEventBit(0x42)) {
            updateDogRaceContext();
            toggleDailyEventBit(0x40);
            gCutsceneIndex = DOG_RACE_AFTER_RACE;
            loadCutscene(0);
            set = TRUE;
            gCutsceneFlags |= (2 | 4);
        }
    }

    if (!set) { 
        if (gSeason == SPRING && gDayOfMonth == 17 && (7 < gHour && gHour < 18)) {
            updateHorseRaceContext();
            gCutsceneIndex = HORSE_RACE_STARTING_ANNOUNCEMENT;
            loadCutscene(0);
            set = TRUE;
            gCutsceneFlags |= (2 | 4);
        }
    }
    
    if (!set) { 
        if (gSeason == AUTUMN && gDayOfMonth == 28 && (7 < gHour && gHour < 18)) {
            updateHorseRaceContext();
            gCutsceneIndex = HORSE_RACE_STARTING_ANNOUNCEMENT;
            loadCutscene(0);
            set = TRUE;
            gCutsceneFlags |= (2 | 4);
        }
    }

    if (!set) { 
        if (gSeason == WINTER && gDayOfMonth == 19 && (7 < gHour && gHour < 18)) {
            updateDogRaceContext();
            gCutsceneIndex = DOG_RACE_AFTER_RACE;
            loadCutscene(0);
            set = TRUE;
            gCutsceneFlags |= (2 | 4);
        }
    }
    
    return index;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setBeachCutscenes);

u16 setBeachCutscenes(void) {
    
    bool set = FALSE;
    u16 index = 0xFFFF;

    if (gSeason == SUMMER && gDayOfMonth == 17 && NIGHTTIME) {
        gCutsceneIndex = FIREFLY_FESTIVAL;
        loadCutscene(0);
        set = TRUE;
        gCutsceneFlags |= (2 | 4);
    }

    if (!set) {
        if (gSeason == SUMMER && gDayOfMonth == 24 && (5 < gHour && gHour < 18)) {
            gCutsceneIndex = SEA_FESTIVAL;
            loadCutscene(0);
            set = TRUE;
            gCutsceneFlags |= (2 | 4);
        }
    }
    
    if (!set) {
        if (gSeason == SUMMER && gDayOfMonth == 1 && NIGHTTIME && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            gCutsceneIndex = BEACH_FIREWORKS;
            loadCutscene(0);
            set = TRUE;
            gCutsceneFlags |= (2 | 4);
        }
    }
    
    if (!set) {
        if (!checkDailyEventBit(KAREN_MARRIAGE_CUTSCENE) && gSeason == WINTER && gDayOfMonth == 10 && (5 < gHour && gHour < 18) && !checkLifeEventBit(MARRIED)) {
            setDailyEventBit(KAREN_MARRIAGE_CUTSCENE);
            gCutsceneIndex = 0x2F1;
            loadCutscene(0);
            gCutsceneFlags |= (2 | 4);
            set = TRUE;
        }
    }

    if (!set) {
        if (gSeason == WINTER && gDayOfMonth == 24 && NIGHTTIME && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            gCutsceneIndex = 0x2F4;
            loadCutscene(0);
            gCutsceneFlags |= (2 | 4);
            set = TRUE;
        }
    }
    
    if (!set) {
        if (!checkLifeEventBit(0xCA) && npcAffection[KAREN] < 101 && (5 < gHour && gHour < 18) && (gYear >= 3 || (gYear == 2 && gSeason == WINTER)) && (!checkLifeEventBit(MARRIED) || gWife != KAREN) && !checkLifeEventBit(KAREN_KAI_MARRIED) && gFlowerFestivalGoddess != 4) {
            setLifeEventBit(0xCA);
            setLifeEventBit(0x2F);
            setSpecialDialogueBit(0x23);
            gCutsceneIndex = 0x2F0;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkLifeEventBit(KAREN_AND_ELLI_BEACH_CUTSCENE) && npcAffection[ELLI] >= 150 && npcAffection[KAREN] >= 150 && gWeather == SUNNY && gSeason == SUMMER && (5 < gHour && gHour < 18) && (!checkLifeEventBit(MARRIED) || gWife != KAREN) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            setLifeEventBit(KAREN_AND_ELLI_BEACH_CUTSCENE);
            gCutsceneIndex = KAREN_AND_ELLI;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkLifeEventBit(0x9C) && !checkLifeEventBit(MARRIED) && npcAffection[KAREN] >= 250 && gWeather == SUNNY && (14 < gHour && gHour < 24) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            setLifeEventBit(0x9C);
            gCutsceneIndex = KAREN_PINK_HEART_EVENT;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);     
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkLifeEventBit(0x9D) && checkLifeEventBit(0x20) && npcAffection[KAI] >= 180 && gWeather == SUNNY && NIGHTTIME && (!checkLifeEventBit(MARRIED) || gWife != KAREN) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            setLifeEventBit(0x9D);
            gCutsceneIndex = 0x2EF;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
        }
    }
    
    return index;
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setRoadCutscenes);

u16 setRoadCutscenes(void) {
    
    bool set = FALSE;
    u16 index = 0xFFFF;

    if (!checkLifeEventBit(0xCC) && npcAffection[CLIFF] >= 200 && gSeason == WINTER && (5 < gHour && gHour < 18) && (!checkLifeEventBit(MARRIED) || gWife != ANN) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && previousMapIndex == 0x52) {
        setLifeEventBit(0xCC);
        gCutsceneIndex = 0x320;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(0);
        set = TRUE;
    }

    if (!set) {
        if (!checkLifeEventBit(0xCD) && bakeryCardPoints >= 6 && npcAffection[ANN] >= 150 && (5 < gHour && gHour < 18) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && previousMapIndex == RANCH) {
            setLifeEventBit(0xCD);
            gCutsceneIndex = 0x321;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(0);
            set = TRUE;
        }    
    }

    if (!set) {
        if (!checkLifeEventBit(SHADY_SALESMAN_MARIA_CUTSCENE) && checkLifeEventBit(0xB6) && (5 < gHour && gHour < 18)) {

            if (previousMapIndex == VILLAGE_1) {
                setLifeEventBit(SHADY_SALESMAN_MARIA_CUTSCENE);
                gCutsceneIndex = SHADY_SALESMAN_AND_MARIA_1;
                gCutsceneFlags |= (2 | 4);
                loadCutscene(0);
                set = TRUE;
            }

            if (previousMapIndex == VILLAGE_2) {
                setLifeEventBit(SHADY_SALESMAN_MARIA_CUTSCENE);
                gCutsceneIndex = SHADY_SALESMAN_AND_MARIA_2;
                gCutsceneFlags |= (2 | 4);
                loadCutscene(0);
                set = TRUE;
            }
        }
    }

    if (!set) {
        if (!checkLifeEventBit(ANN_SPRAINED_ANKLE_CUTSCENE) && !checkDailyEventBit(FESTIVAL_DAY_TYPE_2) && npcAffection[ANN] >= 180 && (5 < gHour && gHour < 18) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && previousMapIndex == FARM) {
            setLifeEventBit(ANN_SPRAINED_ANKLE_CUTSCENE);
            setDailyEventBit(0x59);
            gCutsceneIndex = ANN_SPRAINED_ANKLE;
            loadCutscene(0);
            gCutsceneFlags |= (2 | 4); 
        }
    }
    
    return index;

}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", handleTimedDailyCutscenes);
 
// time-based daily cutscenes/update game
u16 handleTimedDailyCutscenes(u8 arg0) {

    u16 index = 0xFFFF;
    bool set = FALSE;
    
    if (!(gCutsceneFlags & 1)) {
        
        gCutsceneFlags &= ~2;
    
        if (gHour >= 17 && !checkDailyEventBit(0)) {

            if (dailyShippingBinValue) {
                gCutsceneIndex = 0;
            } else {
                gCutsceneIndex = 1;
            }

            handleDailyShipment();

            if (gBaseMapIndex == FARM) {
                index = 0;
                set = TRUE;
            }       

        }
        
        if (!set) {

            if (checkLifeEventBit(HOUSE_EXTENSION_CONSTRUCTION)) {

                if (gHour >= 8 && !checkDailyEventBit(HOUSE_EXTENSION_WORK) && !checkDailyEventBit(END_OF_DAILY_HOUSE_EXTENSION_WORK)) {
                    
                    setDailyEventBit(HOUSE_EXTENSION_WORK);
                    
                    if (gBaseMapIndex == FARM) {
                        gCutsceneIndex = 0;
                        index = 5;
                        set = TRUE;
                    }
                
                }

                if (gHour >= 15) {
                    if (!checkDailyEventBit(END_OF_DAILY_HOUSE_EXTENSION_WORK)) {
                        setDailyEventBit(END_OF_DAILY_HOUSE_EXTENSION_WORK);
                    }
                }
            }   
        }
        
        if (!set) {
            
            if (!checkDailyEventBit(0x1D) && !checkDailyEventBit(0x1E) && gSeason == AUTUMN && gDayOfMonth == 3 && (5 < gHour && gHour < 15) && getTotalFarmAnimalsByType(2)) {
               
                setDailyEventBit(0x1D);
                
                if (gBaseMapIndex == FARM) {
                    index = 0x21;
                    set = TRUE;
                    gCutsceneFlags |= 2;
                }
                
                if (gHour >= 16) {
                    if (!checkDailyEventBit(0x1E)) {
                        setDailyEventBit(0x1E);
                    }
                }
            }
        }
        
        if (!set) {
            if (gBaseMapIndex == FARM) {
                setFarmVisitCutscenes(1);
            }
        }
        
        if (index != 0xFFFF) {
            initializeCutscene(index);
        }
    }

    return index;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", handleEndOfDayCutscenes);

// cutscenes on wake up: evaluation, wedding, rivals' weddings, animal funeral, sick days
u16 handleEndOfDayCutscenes(void) {

    bool set = FALSE;
    u16 result = 0xFFFF;
    u8 temp;

    if (!(gCutsceneFlags & 1)) {
        
        gCutsceneFlags &= ~2;

        // evaluation
        if (gYear == 3 && gSeason == SUMMER && gDayOfMonth == 1) {
            
            set = TRUE;
            
            D_801C3B62 = calculateNPCAffectionScore();
            
            D_80215DF0 = npcAffection[MARIA] + npcAffection[POPURI] + npcAffection[ELLI] + npcAffection[ANN] + npcAffection[KAREN];
            
            // sick days
            D_801886E0 = gSickDays;
            // dead animals
            D_801886A8 = deadAnimalCount;
            
            D_801C3E1C = gTotalCropsShipped + gTotalMilkShipped + gTotalEggsShipped;
            
            // fish caught
            D_801D62C4 = totalFishCaught; 
            
            D_8017026E = calculateHouseExtensionScore();
            D_80205638 = calculateGrassScore();
            D_8013D438 = calculateFieldScore(); 
            D_80189824 = calculateShippingScore();
            
            // animals
            D_80205209 = getTotalCowsCount();
            D_801D62C6 = getTotalSheepCount();
            D_801886D0 = getTotalChickenCount();

            // recipes sum
            D_80188F68 = getAcquiredRecipesTotal();

            setEntrance(0x3E);
            gCutsceneIndex = EVALUATION;
            loadCutscene(0);
            gCutsceneFlags |= (2 | 4);
            
        }

        // wedding
        if (!set && gDayOfWeek == SUNDAY && checkLifeEventBit(ENGAGED)) {
            
            toggleLifeEventBit(ENGAGED);
            setLifeEventBit(MARRIED);
            setSpecialDialogueBit(3);
            
            anniversarySeason = gSeason;
            anniversaryDate = gDayOfMonth;
            
            switch (gWife) {                        
                case MARIA:                                 
                    gCutsceneIndex = MARIA_WEDDING;
                    setSpecialDialogueBit(MARRIED_TO_MARIA_DIALOGUE);
                    setSpecialDialogueBit(GOT_MARRIED_DIALOGUE);
                    break;
                case 1:                                 
                    gCutsceneIndex = POPURI_WEDDING;
                    setSpecialDialogueBit(MARRIED_TO_POPURI_DIALOGUE);
                    setSpecialDialogueBit(GOT_MARRIED_DIALOGUE);
                    break;
                case 2:                                 
                    gCutsceneIndex = ELLI_WEDDING;
                    setSpecialDialogueBit(MARRIED_TO_ELLI_DIALOGUE);
                    setSpecialDialogueBit(GOT_MARRIED_DIALOGUE);
                    break;
                case 3:                                 
                    gCutsceneIndex = ANN_WEDDING;
                    setSpecialDialogueBit(MARRIED_TO_ANN_DIALOGUE);
                    setSpecialDialogueBit(GOT_MARRIED_DIALOGUE);
                    break;
                case 4:                                 
                    gCutsceneIndex = KAREN_WEDDING;
                    setSpecialDialogueBit(MARRIED_TO_KAREN_DIALOGUE);
                    setSpecialDialogueBit(GOT_MARRIED_DIALOGUE);
                    break;
                default:
                    break;
            }
            
            setEntrance(0x32);
            loadCutscene(0);
            set = TRUE;
            gCutsceneFlags |= (2 | 4);

        }

        // rivals' weddings
        if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_2)) { 
            
            if (!set && !checkLifeEventBit(MARIA_HARRIS_MARRIED) && (!checkLifeEventBit(MARRIED) || gWife != MARIA) && npcAffection[HARRIS] >= 200 && (checkLifeEventBit(MARRIED) || npcAffection[MARIA] < 120))  {
                
                setLifeEventBit(MARIA_HARRIS_MARRIED);
                setSpecialDialogueBit(0xB);
                gCutsceneIndex = 0xCB;
                setEntrance(0);
                loadCutscene(0);
                set = TRUE;
                gCutsceneFlags |= (2 | 4);
                
            }

            if (!set && !checkLifeEventBit(POPURI_GRAY_MARRIED) && (!checkLifeEventBit(MARRIED) || gWife != POPURI) && npcAffection[GRAY] >= 200 && (checkLifeEventBit(MARRIED) || npcAffection[POPURI] < 120)) {
               
                setLifeEventBit(POPURI_GRAY_MARRIED);
                setSpecialDialogueBit(0xC);
                gCutsceneIndex = 0xCE;
                setEntrance(0);
                loadCutscene(0);
                set = TRUE;
                gCutsceneFlags |= (2 | 4);

            }

            if (!set && !checkLifeEventBit(ELLI_JEFF_MARRIED) && (!checkLifeEventBit(MARRIED) || gWife != ELLI) && npcAffection[JEFF] >= 200 && (checkLifeEventBit(MARRIED) || npcAffection[ELLI] < 120)) {
                
                setLifeEventBit(ELLI_JEFF_MARRIED);
                setSpecialDialogueBit(0xD);
                gCutsceneIndex = 0xD0;
                setEntrance(0);
                loadCutscene(0);
                set = TRUE;
                gCutsceneFlags |= (2 | 4);

            }

            if (!set && !checkLifeEventBit(ANN_CLIFF_MARRIED) && (!checkLifeEventBit(MARRIED) || gWife != ANN) && npcAffection[CLIFF] >= 200 && (checkLifeEventBit(MARRIED) || npcAffection[ANN] < 120)) {
                
                setLifeEventBit(ANN_CLIFF_MARRIED);
                setSpecialDialogueBit(0xE);
                gCutsceneIndex = 0xD6;
                setEntrance(0);
                loadCutscene(0);
                set = TRUE;
                gCutsceneFlags |= (2 | 4);

            }
            
            if (!set && !checkLifeEventBit(KAREN_KAI_MARRIED) && (!checkLifeEventBit(MARRIED) || gWife != KAREN) && npcAffection[KAI] >= 200 && (checkLifeEventBit(MARRIED) || npcAffection[KAREN] < 120) && checkLifeEventBit(0x96) && !checkLifeEventBit(KAI_GONE) && !checkLifeEventBit(KAREN_GONE)) {
                
                setLifeEventBit(KAREN_KAI_MARRIED);
                setSpecialDialogueBit(0xF);
                gCutsceneIndex = 0xD9;
                setEntrance(0);
                loadCutscene(0);
                set = TRUE;
                gCutsceneFlags |= (2 | 4);

            }
            
        }

        // animal funeral
        if (!set && checkLifeEventBit(ANIMAL_DIED) && !checkDailyEventBit(FESTIVAL_DAY_TYPE_2)) {
            
            toggleLifeEventBit(ANIMAL_DIED);
            setLifeEventBit(0x50);
            setEntrance(0x3E);
            gCutsceneIndex = 0x20B;
            loadCutscene(0);
            gCutsceneFlags |= (2 | 4);
            set = TRUE;
            deadAnimalCount += adjustValue(deadAnimalCount, 1, 999);

        }

        // sick day
        if (!set && checkFatigueLevel() == 3) {
            
            gPlayer.fatigueCounter = 0;
            
            setEntrance(0xA);

            temp = getBacholeretteWithHighestAffection(120);

            if (temp) {

                gCutsceneIndex = 0;

                switch (temp) {

                    case 1:

                        if (!checkLifeEventBit(MARRIED) || gWife != MARIA) {
                            if (!checkLifeEventBit(MARRIED)) {                            
                                if (!checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
                                    gCutsceneIndex = MARIA_VISIT_PLAYER_SICK;
                                }
                            }
                            
                            break;
                        } 
                        
                        gCutsceneIndex = MARIA_VISIT_PLAYER_SICK;
                        break;
                
                        
                    case 2:

                        if (!checkLifeEventBit(MARRIED) || gWife != POPURI) {
                            if (!checkLifeEventBit(MARRIED)) {                            
                                if (!checkLifeEventBit(POPURI_GRAY_MARRIED)) {
                                    gCutsceneIndex = 0xA;
                                }
                            }
                            break;
                            
                        } 
                        
                        gCutsceneIndex = 0xA;
                        break;
                    
                    case 3:

                        if (!checkLifeEventBit(MARRIED) || gWife != ELLI) {
                            if (!checkLifeEventBit(MARRIED)) {                            
                                if (!checkLifeEventBit(ELLI_JEFF_MARRIED)) {
                                    gCutsceneIndex = 0x11;
                                }
                            }

                            break;
                        } 

                        gCutsceneIndex = 0x11;
                        break;
                    
                    case 4: 

                        if (!checkLifeEventBit(MARRIED) || gWife != ANN) {
                            if (!checkLifeEventBit(MARRIED)) {                            
                                if (!checkLifeEventBit(ANN_CLIFF_MARRIED)) {
                                    gCutsceneIndex = 0x18;
                                }
                            }

                            break;
                        } 

                        gCutsceneIndex = 0x18;
                        break;

                    case 5:

                        if (!checkLifeEventBit(MARRIED) || gWife != KAREN) {
                            if (!checkLifeEventBit(MARRIED)) {                            
                                if (!checkLifeEventBit(KAREN_KAI_MARRIED)) {
                                    gCutsceneIndex = 0x1F;
                                }
                            }
                            break;
                        }

                        gCutsceneIndex = 0x1F;
                        break;

                    default:
                        break;

                }

            } else {

                if (npcAffection[KENT] >= 50) {
                    gCutsceneIndex = 0;
                } else {
                    gCutsceneIndex = 0x33;
                }
            }
            
            loadCutscene(0);
            gCutsceneFlags |= 2;
            set = 2;

            gSickDays += adjustValue(gSickDays, 1, 999);

        }
    }

    if (set) {
        gWeather = SUNNY;
    }

    if (set == 1) {
        feedAllAnimals();
    }

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setDreamCutscenes);

// dreams
u16 setDreamCutscenes(void) {

    bool set = FALSE;

    if (!checkLifeEventBit(0x96) && checkSpecialDialogueBit(0x14B) && gSeason == AUTUMN && gDayOfMonth == 6) {
        gCutsceneIndex = 0x28;
        loadCutscene(0);
        set = TRUE;
        gCutsceneFlags |= 2;
    }

    if (!set) {
        if (!checkLifeEventBit(0x63) && npcAffection[MARIA] >= 160 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            gCutsceneIndex = 0x29;
            loadCutscene(0);
            set = TRUE;
        }
    }

    if (!set) { 
        if (!checkLifeEventBit(0x64) && npcAffection[POPURI] >= 160 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            gCutsceneIndex = 0x2A;
            loadCutscene(0);
            set = TRUE;
        }
    }
    
    if (!set) { 
        if (!checkLifeEventBit(0x65) && npcAffection[ELLI] >= 160 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            gCutsceneIndex = 0x2B;
            loadCutscene(0);
            set = TRUE;
        }
    }

    if (!set) { 
        if (!checkLifeEventBit(0x66) && npcAffection[ANN] >= 160 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            gCutsceneIndex = 0x2C;
            loadCutscene(0);
            set = TRUE;
        }
    }

    if (!set) { 
        if (!checkLifeEventBit(0x67) && npcAffection[KAREN] >= 160 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            gCutsceneIndex = 0x2D;
            loadCutscene(0);
            set = TRUE;
        }
    }

    if (!set) { 
        if (!checkLifeEventBit(0xD8) && checkLifeEventBit(MARRIED) && checkLifeEventBit(1) && !checkDailyEventBit(0x52) && gWife == MARIA && (29 < gBabyAge && gBabyAge < 120) && !getRandomNumberInRange(0, 10)) {
            setLifeEventBit(0xD8);
            gCutsceneIndex = 9;
            loadCutscene(0);
            set = TRUE;
            gCutsceneFlags |= (2 | 4);
        }
    }

    if (!set) { 
        if (!checkLifeEventBit(0xD8) && checkLifeEventBit(MARRIED) && checkLifeEventBit(1) && !checkDailyEventBit(0x52) && gWife == POPURI && (29 < gBabyAge && gBabyAge < 120) && !getRandomNumberInRange(0, 10)) {
            setLifeEventBit(0xD8);
            gCutsceneIndex = 0x10;
            loadCutscene(0);
            set = TRUE;
            gCutsceneFlags |= (2 | 4);
        }
    }

    if (!set) { 
        if (!checkLifeEventBit(0xD8) && checkLifeEventBit(MARRIED) && checkLifeEventBit(1) && !checkDailyEventBit(0x52) && gWife == ELLI && (29 < gBabyAge && gBabyAge < 120) && !getRandomNumberInRange(0, 10)) {
            setLifeEventBit(0xD8);
            gCutsceneIndex = 0x17;
            loadCutscene(0);
            set = TRUE;
            gCutsceneFlags |= (2 | 4);
        }
    }

    if (!set) { 
        if (!checkLifeEventBit(0xD8) && checkLifeEventBit(MARRIED) && checkLifeEventBit(1) && !checkDailyEventBit(0x52) && gWife == ANN && (29 < gBabyAge && gBabyAge < 120) && !getRandomNumberInRange(0, 10)) {
            setLifeEventBit(0xD8);
            gCutsceneIndex = 0x1E;
            loadCutscene(0);
            set = TRUE;
            gCutsceneFlags |= (2 | 4);
        }
    }

    if (!set) { 
        if (!checkLifeEventBit(0xD8) && checkLifeEventBit(MARRIED) && checkLifeEventBit(1) && !checkDailyEventBit(0x52) && gWife == KAREN && (29 < gBabyAge && gBabyAge < 120) && !getRandomNumberInRange(0, 10)) {
            setLifeEventBit(0xD8);
            gCutsceneIndex = 0x25;
            loadCutscene(0);
            set = TRUE;
            gCutsceneFlags |= (2 | 4);
        }
    }
    
    return set;
    
}

// INCLUDE_ASM("asm/nonmatchings/game/cutscenes", handleCutsceneCompletion);

void handleCutsceneCompletion(void) {

    bool found;
    u16 tempEntrance;
    
    if (gCutsceneFlags & CUTSCENE_ACTIVE) {

        if (gCutsceneCompletionFlags & 1) {

            gCutsceneFlags &= ~CUTSCENE_ACTIVE;

            switch (gCutsceneBytecodeSegmentIndex) {

                case 1:                                 

                    toggleDailyEventBit(1);
                
                    // switch on acquired animal
                    switch (selectedAnimalType) {               

                        case DOG_TYPE:                             
                            gNamingScreenIndex = 2;
                            break;
                        
                        case HORSE_TYPE:                             
                            setLifeEventBit(0x3A);
                            gNamingScreenIndex = 3;
                            break;

                        case COW_TYPE:                             
                            setLifeEventBit(0x36);
                            gNamingScreenIndex = 5;
                            gSelectedAnimalIndex = initializeNewFarmAnimal(1, 0);
                            break;
                            
                        case SHEEP_TYPE:                             
                            setLifeEventBit(0x34);
                            gNamingScreenIndex = 6;
                            gSelectedAnimalIndex = initializeNewFarmAnimal(4, 0);
                            break;

                        case CHICKEN_TYPE:                             
                            setLifeEventBit(0x32);
                            gNamingScreenIndex = 7;
                            gSelectedAnimalIndex = initializeNewChicken(2, 0xFF);
                            break;

                        default:
                            break;
                        
                    }
                    
                    handleExitLevel(1, NAMING_SCREEN_LOAD);
                    break;

                case 7:
                case 8:

                    switch (gCutsceneIndex) {

                        case 0xAA:
                            handleHatchChicken();
                            handleHatchChicken();
                            handleHatchChicken();
                            handleHatchChicken();
                            handleHatchChicken();
                            handleHatchChicken();
                            break;    
                        
                        case 0x71:
                            setEntrance(0);
                            setMainLoopCallbackFunctionIndex(MAP_LOAD);
                            break;
                        
                        case 0x81:
                        case 0x82:
                        case 0x8B:
                        case 0xEF ... 0xF3:
                            if (gCutsceneCompletionFlags & 0x40000000) {
                                setMainLoopCallbackFunctionIndex(END_OF_FESTIVAL_DAY_1);
                            }
                            break;
                        
                        case 0xAB:
                            break;
                        
                        case 0xFA ... 0x100:
                            setMainLoopCallbackFunctionIndex(END_OF_FESTIVAL_DAY_1);
                            break;
                        
                    }
                    
                    break;

                case 9:
                    
                    switch (gCutsceneIndex) {           

                        case 0x1:                           
                        case 0x2:                           
                            setLifeEventBit(0x61);
                            break;
                        case 0x26:                          
                        case 0x27:                          
                            setLifeEventBit(0x62);
                            break;
                        case 0x28:                          
                            setLifeEventBit(0x96);
                        case 0x0:                           
                        case MARIA_VISIT_PLAYER_SICK:                           
                        case 0x5:                           
                        case 0x9:                           
                        case 0xA:                           
                        case 0xC:                           
                        case 0x10:                          
                        case 0x11:                          
                        case 0x13:                          
                        case 0x17:                          
                        case 0x18:                          
                        case 0x1A:                          
                        case 0x1E:                          
                        case 0x1F:                          
                        case 0x21:                          
                        case 0x25:                          
                        case 0x33:                          
                            setMainLoopCallbackFunctionIndex(END_OF_DAY_1);
                            break;
                        case 0x29:                          
                            setLifeEventBit(0x63);
                            setMainLoopCallbackFunctionIndex(END_OF_DAY_1);
                            break;
                        case 0x2A:                          
                            setLifeEventBit(0x64);
                            setMainLoopCallbackFunctionIndex(END_OF_DAY_1);                             
                            break;
                        case 0x2B:                          
                            setLifeEventBit(0x65);
                            setMainLoopCallbackFunctionIndex(END_OF_DAY_1);                             
                            break;
                        case 0x2C:                          
                            setLifeEventBit(0x66);
                            setMainLoopCallbackFunctionIndex(END_OF_DAY_1);                             
                            break;
                        case 0x2D:                          
                            setLifeEventBit(0x67);
                            setMainLoopCallbackFunctionIndex(END_OF_DAY_1);                             
                            break;
                        case 0x4:                           
                        case 0xB:                           
                        case 0x12:                          
                        case 0x19:                          
                        case 0x20:                          
                            if (gHour >= 6) {
                                gNamingScreenIndex = 4;
                                handleExitLevel(1, NAMING_SCREEN_LOAD);
                            } else {
                                setMainLoopCallbackFunctionIndex(END_OF_DAY_1);                             
                            }
                            break;
                        
                        }

                    break;

                case 10:
                    if (gCutsceneIndex == ANN_SPRAINED_ANKLE && (gCutsceneCompletionFlags & 0x40000000)) {
                        gHour += 2;
                        setEntrance(RANCH_FROM_RANCH_STORE);
                        setMainLoopCallbackFunctionIndex(MAP_LOAD);
                    }
                
                case 11:

                    switch (gCutsceneIndex) {

                        case 0x12E:
                            if (gCutsceneCompletionFlags & 0x40000000) {
                                gHour += 2;
                                setEntrance(0x34);
                                setMainLoopCallbackFunctionIndex(MAP_LOAD);
                            }
                            break;

                        case ELLI_SPRAINED_ANKLE:                         
                            if (gCutsceneCompletionFlags & 0x40000000) {
                                gHour += 2;
                                setEntrance(0x35);
                                setMainLoopCallbackFunctionIndex(MAP_LOAD);
                            }
                            break;

                        case 0x130:
                        case 0x131:
                            setMainLoopCallbackFunctionIndex(END_OF_FESTIVAL_DAY_1);
                            break;

                        case 0x13E:                         
                            // store tool
                            storeTool(FISHING_POLE);
                            break;
                        
                        case 0x144:                         
                        case 0x148:                         
                            setDailyEventBit(0x2C);
                            break;
                        
                        case 0x147:                         
                            if (gSeason == AUTUMN && gDayOfMonth == 27) {
                                setLifeEventBit(BRIDGE_COMPLETED);
                            }
                            break;
                        
                        case 0x14A:                         
                            if (gSeason == WINTER && gDayOfMonth == 16) {
                                setLifeEventBit(HOT_SPRINGS_COMPLETED);
                                setDailyEventBit(0x5B);
                            }
                            clearForagableObjects(TOP_OF_MOUNTAIN_1);
                            break;
                        
                        case 0x150:                         
                            toggleDailyEventBit(0x47);
                            setLifeEventBit(0x5E);
                            break;
                        
                        case 0x151:                         
                            toggleDailyEventBit(0x48);
                            setLifeEventBit(0x5F);
                            break;
                        
                        case 0x155:                         
                        case 0x158:                         
                            toggleDailyEventBit(0x45);
                            setDailyEventBit(0x46);
                            break;
                        
                        case 0x159:                         
                            setSpecialDialogueBit(0x14B);
                            setMainLoopCallbackFunctionIndex(END_OF_FESTIVAL_DAY_1);
                            break;
                        
                    }
                    
                    break;
                
                case 12:            
                                    
                    switch (gCutsceneIndex) {           

                        case 0x293:
                            setMainLoopCallbackFunctionIndex(END_OF_FESTIVAL_DAY_1);
                            break;

                        // get horse at ranch
                        case RANCH_GET_HORSE:                         
                        case 0x299:                         
                            gNamingScreenIndex = 3;
                            handleExitLevel(1, NAMING_SCREEN_LOAD);
                            setLifeEventBit(HAVE_HORSE);
                            break;
                        case 0x28B:                         
                            loadLevel(0);
                            setMainLoopCallbackFunctionIndex(SET_AUDIO_AND_LIGHTING);
                            initializeHorse();
                            break;
                        case 0x297:                         
                            setEntrance(0x4C);
                            setMainLoopCallbackFunctionIndex(MAP_LOAD);
                            break;
                        case 0x298:                         
                            npcAffection[ANN] = 120;
                            toggleLifeEventBit(WIFE_LEFT);
                            setMainLoopCallbackFunctionIndex(END_OF_DAY_1);
                            break;
                        }

                    break;

                case 13:
                    
                    switch (gCutsceneIndex) {          

                        case 0x2BD:                         
                            if (gCutsceneCompletionFlags & 0x40000000) {
                                gHour += 2;
                                setEntrance(0x2F);
                                setMainLoopCallbackFunctionIndex(MAP_LOAD);
                            }
                            break;
                        
                        case 0x2C0:                         
                            setEntrance(0x50);
                            setMainLoopCallbackFunctionIndex(MAP_LOAD);
                            break;
                        
                        case KAREN_YELLOW_HEART_EVENT_1:                         
                            if (gCutsceneCompletionFlags & 0x40000000) {
                                toggleDailyEventBit(0x4F);
                                setEntrance(0x30);
                                setMainLoopCallbackFunctionIndex(MAP_LOAD);                               
                            }
                            break;
                        
                        case 0x2C6:                         
                            npcAffection[KAREN] = 120;
                            toggleLifeEventBit(WIFE_LEFT);
                            setMainLoopCallbackFunctionIndex(END_OF_DAY_1);
                            break;
                    }
                    
                    break;
                
                case 14:

                    switch (gCutsceneIndex) {          
                        
                        case 0x190:
                        case 0x191:
                        case 0x192:
                        case 0x193:
                        case 0x194:
                            setMainLoopCallbackFunctionIndex(END_OF_DAY_1);
                            break;
                        case 0x195:                 
                        case 0x196:                 
                        case 0x197:                 
                        case 0x198:                 
                        case 0x199:        
                            setMainLoopCallbackFunctionIndex(END_OF_FESTIVAL_DAY_1);
                            break; 
                        
                        case 0x19B:                        
                            setEntrance(0x1F);
                            setMainLoopCallbackFunctionIndex(MAP_LOAD);
                            D_802373A8 += adjustValue(D_802373A8, -1, 1);
                            removeTool(BLUE_MIST_SEEDS);
                            blueMistFlowerPlot = 0xB3;
                            pondFieldTiles[5][3] = 0xB3;
                            break;
                            
                        case 0x1A0:                        
                            setLifeEventBit(0x31);
                            setSpecialDialogueBit(0x43);
                            setMainLoopCallbackFunctionIndex(END_OF_FESTIVAL_DAY_1);
                            break;
                        
                        case 0x1A2:                 
                        case 0x1A4:                 
                        case 0x1AD:                 
                        case 0x1AE:                 
                        case 0x1AF:                 
                        case 0x1B3:                 
                        case 0x1B4:                 
                            setMainLoopCallbackFunctionIndex(END_OF_FESTIVAL_DAY_1);
                            break;

                        case STARRY_NIGHT_FESTIVAL:                        
                            if (!(gCutsceneCompletionFlags & 2)) {
                                setMainLoopCallbackFunctionIndex(END_OF_FESTIVAL_DAY_1);
                            }
                            break;

                        case 0x1B5:                        
                            // flower shop entrance
                            setEntrance(0x57);
                            setMainLoopCallbackFunctionIndex(MAP_LOAD);               
                            break;
                
                        case 0x1B6:                        
                            setEntrance(0x5A);
                            setMainLoopCallbackFunctionIndex(MAP_LOAD);               
                            break;
                
                        case 0x1B7:                        
                            // Popuri
                            npcAffection[POPURI] = 120;
                            toggleLifeEventBit(WIFE_LEFT);
                            setMainLoopCallbackFunctionIndex(END_OF_DAY_1);
                            break;
                
                        case 0x1B8:                        
                            npcAffection[ELLI] = 120;
                            toggleLifeEventBit(WIFE_LEFT);
                            setMainLoopCallbackFunctionIndex(END_OF_DAY_1);
                            break;

                        }
                    
                    break;

                case 15:

                    switch (gCutsceneIndex) {           

                        case 0x20B:                        
                            setMainLoopCallbackFunctionIndex(END_OF_DAY_1);
                            break;

                        case 0x20C:                         
                            setEntrance(0x66);
                            setMainLoopCallbackFunctionIndex(MAP_LOAD);
                            break;
                            
                        case 0x20E:                         
                            if (gCutsceneCompletionFlags & 0x40000000) {
                                gHour += 2;
                                setEntrance(0x41);
                                setMainLoopCallbackFunctionIndex(MAP_LOAD);
                            }
                            break;

                        case 0x211:                         
                            npcAffection[MARIA] = 120;
                            toggleLifeEventBit(WIFE_LEFT);
                            setMainLoopCallbackFunctionIndex(END_OF_DAY_1);
                        
                        }
                    
                    break;
                    
                case 16:
                    if (gCutsceneIndex != BEACH_FIREWORKS) {
                        if (gCutsceneIndex != 0x2F4) {
                            break;
                        }
                    } 
                    setMainLoopCallbackFunctionIndex(END_OF_FESTIVAL_DAY_1);
                    break;

                case 17:
                    setMainLoopCallbackFunctionIndex(END_OF_FESTIVAL_DAY_1);
                    break;
                
                case 18:

                    switch (gCutsceneIndex) {

                        case 0x386 ... 0x387:

                            if (gRacingContext.playerRaceNumber == gRacingContext.currentRaceIndex) {
                                
                                if (((gRaceFinishOrder[2] - 1) == 2) && horseInfo.bestRacePlacement >= 3) {
                                    horseInfo.bestRacePlacement = 2;
                                }
                                
                                if (((gRaceFinishOrder[1] - 1) == 2) && horseInfo.bestRacePlacement >= 2) {
                                    horseInfo.bestRacePlacement = 1;
                                }
                                
                                if ((gRaceFinishOrder[0] - 1) == 2 && horseInfo.bestRacePlacement) {
                                    horseInfo.bestRacePlacement = 0;
                                }
     
                            }
                            
                            setMainLoopCallbackFunctionIndex(RACE_RESULTS_LOAD);
                            gCutsceneCompletionFlags = 0;
                            break;
                        
                    }
                    
                    break;

                case 19:
                    setMainLoopCallbackFunctionIndex(END_OF_FESTIVAL_DAY_1);
                    break;
                
                case 20:
                    if (gCutsceneIndex == VEGETABLE_FESTIVAL_SQUARE) {
                        setMainLoopCallbackFunctionIndex(END_OF_FESTIVAL_DAY_1);
                    }  
                    break;
                
                case 21:
                    if (gCutsceneIndex == SQUARE_FIREWORKS) {
                        setMainLoopCallbackFunctionIndex(END_OF_FESTIVAL_DAY_1);
                    } 
                    break;

                case 22:
                case 23:
                    setMainLoopCallbackFunctionIndex(END_OF_FESTIVAL_DAY_1);
                    break;
                
                case 24:   
                    if (checkLifeEventBit(0x91)) {
                        gFarmAnimals[gCowFestivalEnteredCowIndex].milkType = D_80180710;
                    }
                    setMainLoopCallbackFunctionIndex(END_OF_FESTIVAL_DAY_1);
                    break;

                case 25:
                    setMainLoopCallbackFunctionIndex(END_OF_FESTIVAL_DAY_1);
                    break;

                case 26:
                    setMainLoopCallbackFunctionIndex(END_OF_FESTIVAL_DAY_1);
                    break;
                
                case 27:
                    
                    if (gCutsceneIndex == 0x516) {
                        
                        if (gRacingContext.playerRaceNumber == gRacingContext.currentRaceIndex) {
                            
                            if (gRaceFinishOrder[2] == 1 && dogInfo.bestRacePlacement >= 3) {
                                dogInfo.bestRacePlacement = 2;
                            }
                            
                            if (gRaceFinishOrder[1] == 1 && dogInfo.bestRacePlacement >= 2) {
                                dogInfo.bestRacePlacement = 1;
                            }
                            
                            if (gRaceFinishOrder[0] == 1 && dogInfo.bestRacePlacement) {
                                dogInfo.bestRacePlacement = 0;
                            }

                        }
                        
                        setMainLoopCallbackFunctionIndex(RACE_RESULTS_LOAD);
                        gCutsceneCompletionFlags = 0;
                    
                    }
                    
                    break;

                case 28:
                case 29:
                    setMainLoopCallbackFunctionIndex(END_OF_FESTIVAL_DAY_1);
                    break;

                case 31:
                    
                    if (gCutsceneIndex == 0x611) {

                        u32 ptr1 = (u32)&_fontPalette1SegmentRomStart;
                        u32 ptr2 =  (u32)&_fontPalette1SegmentRomEnd;
                    
                        // fontPalette1
                        nuPiReadRom((u32)&_fontPalette1SegmentRomStart, FONT_PALETTE_1_BUFFER, ptr2 - ptr1);
                        setMessageBoxFont(0, 0xE, 0xE, FONT_TEXTURE_BUFFER, FONT_PALETTE_1_BUFFER);
                        setMessageBoxInterpolationWithFlags(0, -4, 0);
                        setMessageBoxSfx(0, 0x57, 8, 1);
 
                        if (D_80180710 == 0) {
                            // show pink overlay message
                            showPinkOverlayText(23);
                        } else {
                            launchIntroCutscene(0x5AA, 0x61, 1);

                        }
                    }

                    break;
                
                case 32:
                    if (gCutsceneIndex == DEMO_CUTSCENE_2) {
                        launchIntroCutscene(0x5DE, 0x1A, 1);
                    } else {
                        memcpy(farmFieldTiles, D_80113580, FIELD_HEIGHT * FIELD_WIDTH);
                        launchIntroCutscene(0x5AA, 0x61, 1);
                    }
                    break;

                case 35:
                    if ((gCutsceneIndex == 0x640) && !(gCutsceneCompletionFlags & 2)) {
                        memcpy(farmFieldTiles, D_80113580, FIELD_HEIGHT * FIELD_WIDTH);
                        initializeTitleScreen(1);
                    }
                    break;
                
            }

            if ((gCutsceneCompletionFlags & 0x800) && gCutsceneBytecodeSegmentIndex == 30) {
                deactivateCutsceneExecutors();
                deactivateSprites();
                unloadMapAssets(MAIN_MAP_INDEX);
                initializeTitleScreen(0);
            }

            if (gCutsceneCompletionFlags & 2) {

                updateAnimalCoordinates();
                deactivateAnimalEntities();

                switch (gCutsceneIndex) {

                    case 1:
                        transitionCutscenes(2, 0x3E);
                        break;

                    case 0x26:         
                        transitionCutscenes(0x27, 0);
                        gCutsceneFlags &= ~0x10;
                        gCutsceneFlags |= 8;
                        break;
                    
                    case 0x93:          
                        deactivateSprites();
                        deactivateGlobalSprites();
                        initializeCutsceneExecutors();
                        deactivateNPCEntities();
                        deactivateAnimalEntities();
                        clearAllItemContextSlots();
                        initializeEntityInstances(1);
                        transitionCutscenes(0xFA, SPRING_ENTER);
                        break;

                    case 0x96:          
                    case 0xA1:          
                    case 0xAD:          
                    case 0xB8:          
                    case 0xC5:          
                        deactivateSprites();
                        deactivateGlobalSprites();
                        initializeCutsceneExecutors();
                        deactivateNPCEntities();
                        deactivateAnimalEntities();
                        clearAllItemContextSlots();
                        initializeEntityInstances(1);
                        transitionCutscenes(0xFB, 0x1A);
                        break; 
                    
                    case 0x97:          
                    case 0xA2:          
                    case 0xAE:          
                    case 0xB9:          
                    case 0xC6:          
                        deactivateSprites();
                        deactivateGlobalSprites();
                        initializeCutsceneExecutors();
                        deactivateNPCEntities();
                        deactivateAnimalEntities();
                        clearAllItemContextSlots();
                        initializeEntityInstances(1);
                        transitionCutscenes(0xFC, 0x25);
                        break;
                
                    case 0x9E:          
                        deactivateSprites();
                        deactivateGlobalSprites();
                        initializeCutsceneExecutors();
                        deactivateNPCEntities();
                        deactivateAnimalEntities();
                        clearAllItemContextSlots();
                        initializeEntityInstances(1);
                        transitionCutscenes(0xFD, SPRING_ENTER);
                        break;

                    case 0xA9:          
                        deactivateSprites();
                        deactivateGlobalSprites();
                        initializeCutsceneExecutors();
                        deactivateNPCEntities();
                        deactivateAnimalEntities();
                        clearAllItemContextSlots();
                        initializeEntityInstances(1);
                        transitionCutscenes(0xFE, SPRING_ENTER);
                        break;
                    
                    case 0xB5:          
                        deactivateSprites();
                        deactivateGlobalSprites();
                        initializeCutsceneExecutors();
                        deactivateNPCEntities();
                        deactivateAnimalEntities();
                        clearAllItemContextSlots();
                        initializeEntityInstances(1);
                        transitionCutscenes(0xFF, 0x48);
                        break;
                    
                    case 0xC1:          
                        deactivateSprites();
                        deactivateGlobalSprites();
                        initializeCutsceneExecutors();
                        deactivateNPCEntities();
                        deactivateAnimalEntities();
                        clearAllItemContextSlots();
                        initializeEntityInstances(1);
                        transitionCutscenes(0x100, 0x2E);
                        break;
                    
                    case 0xCB:          
                        deactivateSprites();
                        deactivateGlobalSprites();
                        initializeCutsceneExecutors();
                        deactivateNPCEntities();
                        deactivateAnimalEntities();
                        clearAllItemContextSlots();
                        initializeEntityInstances(1);
                        transitionCutscenes(0x195, 0x32);
                        break;

                    case 0xCE:
                        deactivateSprites();
                        deactivateGlobalSprites();
                        initializeCutsceneExecutors();
                        deactivateNPCEntities();
                        deactivateAnimalEntities();
                        clearAllItemContextSlots();
                        initializeEntityInstances(1);
                        transitionCutscenes(0x196, 0x32); 
                        break;
                    
                    case 0xD0:      
                        deactivateSprites();
                        deactivateGlobalSprites();
                        initializeCutsceneExecutors();
                        deactivateNPCEntities();
                        deactivateAnimalEntities();
                        clearAllItemContextSlots();
                        initializeEntityInstances(1);
                        transitionCutscenes(0x197, 0x32);
                        break;

                    case 0xD6:      
                        deactivateSprites();
                        deactivateGlobalSprites();
                        initializeCutsceneExecutors();
                        deactivateNPCEntities();
                        deactivateAnimalEntities();
                        clearAllItemContextSlots();
                        initializeEntityInstances(1);
                        transitionCutscenes(0x198, 0x32);
                        break;
                    
                    case 0xD9:      
                        deactivateSprites();
                        deactivateGlobalSprites();
                        initializeCutsceneExecutors();
                        deactivateNPCEntities();
                        deactivateAnimalEntities();
                        clearAllItemContextSlots();
                        initializeEntityInstances(1);
                        transitionCutscenes(0x199, 0x32);
                        break;

                    case 0xF4:
                    case 0xF6:
                    case 0xF7:
                        deactivateSprites();
                        deactivateGlobalSprites();
                        initializeCutsceneExecutors();
                        deactivateNPCEntities();
                        deactivateAnimalEntities();
                        clearAllItemContextSlots();
                        initializeEntityInstances(1);
                        transitionCutscenes(STARRY_NIGHT_FESTIVAL, 0x5F);
                        break;
                    
                    case 0xF5:      
                        transitionCutscenes(0x131, 0x25);
                        break;
                    
                    case 0xF8:      
                        transitionCutscenes(0x2F4, 0x27);
                        break;
                    
                    case 0x144:
                        transitionCutscenes(0x146, 0x23);
                        break;

                    case 0x146:     
                        transitionCutscenes(0x147, 0x1A);
                        break;
                    
                    case 0x148:     
                        transitionCutscenes(0x149, 0x23);
                        break;
                    
                    case 0x149:     
                        transitionCutscenes(0x14A, 0x1A);
                        break;
                    
                    case 0x158:     
                        transitionCutscenes(0x159, 0x2E);
                        break;

                    case 0x15A:     
                        // set cutscene wild card/score variables
                        D_801886D2 = getRandomNumberInRange(0, 9);
                        D_80180710 = getRandomNumberInRange(0, 9);
                        clearForagableObjects(MINE_2);
                        transitionCutscenes(0x15B, 0x71);
                        break;

                    case 0x15B:
                    case 0x15C:
                        
                        if (gHour >= 17 || gPlayer.currentStamina < 2) {
                            gCutsceneIndex = 0x15D;
                            tempEntrance = 0x47;
                        } else {
    
                            if (D_80189824 < 0xA) {
                                // set cutscene wild card/score variables
                                D_801886D2 = getRandomNumberInRange(0, 9);
                                D_80180710 = getRandomNumberInRange(0, 9);
                            } else {
                                D_80180710 = 0xFF;
                                D_801886D2 = 0xFF;
                            }

                            gCutsceneIndex = 0x15C;
                            tempEntrance = 0x71;
                            
                            clearForagableObjects(0x45);
                 
                        }
                        
                        setEntrance(tempEntrance);
                        loadCutscene(0);
                        loadLevelFromEntrance(tempEntrance);

                        break;

                    case 0x19A:     
                        transitionCutscenes(0x19B, SPRING_ENTER);
                        break;
                    
                    case 0x1AA:     
                        transitionCutscenes(0x1BB, 0x32);
                        break;

                    // 0x1AC
                    case STARRY_NIGHT_FESTIVAL:

                        setEntrance(0x61);
    
                        if (!(gCutsceneCompletionFlags & 8)) {

                            if (!(gCutsceneCompletionFlags & 0x10)) {
                                
                                if ((gCutsceneCompletionFlags & 0x20)) {
                                    gCutsceneIndex = 0x1AF;
                                } 
                                
                            } else {
                                gCutsceneIndex = 0x1AE;
                            }
                            
                        } else {
                            gCutsceneIndex = 0x1AD;
    
                        }
                        
                        loadCutscene(0);
                        loadLevelFromEntrance(0x61);
                        break;
                    
                    case 0x1B9:     
                    case 0x1BC:
                        setEntrance(0x58);
                        if (gCutsceneIndex == 0x1B9) {
                            gCutsceneIndex = 0x1B5;
                        } else {
                            gCutsceneIndex = 0x1B7;
                        }
                        loadCutscene(0);
                        loadLevelFromEntrance(0x58);
                        break;
                    
                    case 0x1BA:     
                    case 0x1BD:     
                        setEntrance(0x5C);
                        if (gCutsceneIndex == 0x1BA) {
                            gCutsceneIndex = 0x1B6;
                        } else {    
                            gCutsceneIndex = 0x1B8;
                        }
                        loadCutscene(0);
                        loadLevelFromEntrance(0x5C);
                        break;

                    case 0x20D:
                        setEntrance(0x3E);
                        gCutsceneIndex = 0x20E;
                        loadCutscene(0);
                        loadLevelFromEntrance(0x3E);
                        break;
                    
                    case 0x20E:     
                        setEntrance(0x41);
                        loadLevelFromEntrance(0x41);
                        break;
                    
                    case 0x20F:
                        transitionCutscenes(0x210, 0x3E);
                        break;   

                    case 0x212:
                    case 0x213:
                        setEntrance(0x67);
                        if (gCutsceneIndex == 0x212) {
                            gCutsceneIndex = 0x20C;
                        } else {
                            gCutsceneIndex = 0x211;
                        }
                        loadCutscene(0);
                        loadLevelFromEntrance(0x67);
                        break;

                    // 0x29A
                    case ANN_SICK_VISIT_CUTSCENE:
                    case 0x29B:
                        setEntrance(0x4D);
                        if (gCutsceneIndex == 0x29A) {
                            gCutsceneIndex = 0x297;
                        } else {
                            gCutsceneIndex = 0x298;
                        }
                        loadCutscene(0);
                        loadLevelFromEntrance(0x4D);
                        break;
                    
                    // 0x2C1
                    case KAREN_YELLOW_HEART_EVENT_1:
                        gCutsceneIndex = 0x2C2;
                        loadCutscene(0);
                        loadLevelFromEntrance(0x55);
                        break;

                    case 0x2C2:
                        transitionCutscenes(0x2C3, 0x53);
                        break;

                    case 0x2C3:
                    case 0x2C4:
                        toggleDailyEventBit(0x4F);
                        transitionCutscenes(0x2C5, 0x2E);
                        break;
                    
                    case 0x2C7:
                    case 0x2C8:
                        setEntrance(0x52);
                        if (gCutsceneIndex == 0x2C7) {
                            gCutsceneIndex = 0x2C0;
                        } else {
                            gCutsceneIndex = 0x2C6;
                        }
                        loadCutscene(0);
                        loadLevelFromEntrance(0x52);
                        break;
                    
                    // 0x324
                    case ANN_SPRAINED_ANKLE:
                        setEntrance(0x2B);
                        loadLevelFromEntrance(0x2B);
                        break;
                    
                    case 0x384:
                    case HORSE_RACE_STARTING_ANNOUNCEMENT:
                         
                        if (gHour >= 17) {
                            transitionCutscenes(0x388, 0x61);
                        } else {

                            if (gRacingContext.playerRaceNumber == gRacingContext.currentRaceIndex) {
                                gCutsceneIndex = 0x387;
                                // set cutscene wild card/score variables
                                D_80189824 = horseInfo.affection;
                                D_801886D2 = (horseInfo.affection / 3) * 2;
                                D_80180710 = horseInfo.affection / 3;
                            } else {
                                gCutsceneIndex = 0x386;
                            }

                            loadCutscene(0);
                            
                        }
                        
                        break;

                    case 0x386:
                    case 0x387:
                        break;

                    case VEGETABLE_FESTIVAL_VILLAGE:
                        deactivateSprites();
                        deactivateGlobalSprites();
                        initializeCutsceneExecutors();
                        deactivateNPCEntities();
                        deactivateAnimalEntities();
                        clearAllItemContextSlots();
                        initializeEntityInstances(2);
                        transitionCutscenes(VEGETABLE_FESTIVAL_SQUARE, 0x61);
                        break;

                    case 0x514:
                    case 0x515:
                        if (gHour >= 17) {
                            transitionCutscenes(0x517, 0x61);
                        } else {
                            if (gRacingContext.playerRaceNumber == gRacingContext.currentRaceIndex) {
                                D_801C3F78 |= 0x20;
                            } else {
                                D_801C3F78 &= ~0x20;
                            }
                            if (gRacingContext.unk_6E == gRacingContext.currentRaceIndex) {
                                D_801C3F78 |= 0x40;
                            } else {
                                D_801C3F78 &= ~0x40;
                            }
                            
                            gCutsceneIndex = 0x516;
                            loadCutscene(0);
                            
                        }
                        
                        break;

                    case 0x516:
                        setupPlayerEntity(0x6C, 1);
                        handlePlayerAnimation();
                        setEntitiesRGBA(0, 0, 0, 0);
                        setEntrance(0x6C);
                        gCutsceneIndex = 0x515;
                        loadCutscene(0);
                        loadLevelFromEntrance(0x6C);
                        break;

                    case 0x5AA:             
                        transitionCutscenes(0x5AB, 0);
                        break;

                    case 0x5AB:             
                        transitionCutscenes(0x5AC, 0x14);
                        break;

                    // 0x5AD
                    case MAYOR_VILLAGE_TOUR:
                        transitionCutscenes(0x5AE, 0x3E);
                        break;
                    
                    case 0x5AE:            
                        transitionCutscenes(0x5AF, 0);
                        gCutsceneFlags &= ~0x10;
                        gCutsceneFlags |= 8;
                        break;


                    case 0x5DD:
                        break;

                    // 0x60E
                    case EVALUATION:             
                        transitionCutscenes(0x60F, 0x1A);
                        break;

                    case 0x60F:             
                        transitionCutscenes(0x610, 0xA);
                        break;

                    case 0x610:             
                        transitionCutscenes(0x611, 0);
                        break;

                    // 0x640
                    case HOW_TO_PLAY_CUTSCENE:     
                        
                        deactivateSprites();
                        deactivateGlobalSprites();
                        initializeCutsceneExecutors();
                        deactivateNPCEntities();
                        deactivateAnimalEntities();
                        clearAllItemContextSlots();
                        initializeEntityInstances(0);

                        if (D_80189824 == 0) {

                            gSeason = SUMMER;
                            
                            memcpy(farmFieldTiles, D_80115030, FIELD_HEIGHT * FIELD_WIDTH); 
                            tempEntrance = 0;
                            gCutsceneIndex = 0x641;
                            
                            
                        } else if (D_80189824 == 1) {

                            gSeason = SPRING;

                            tempEntrance = 0;
                            
                            switch (D_801886D2) {

                                case 0:
                                    memcpy(farmFieldTiles, D_80115210, FIELD_HEIGHT * FIELD_WIDTH); 
                                    gCutsceneIndex = 0x642;
                                    break;

                                case 1:
                                    memcpy(farmFieldTiles, D_801153F0, FIELD_HEIGHT * FIELD_WIDTH); 
                                    gCutsceneIndex = 0x643;
                                    break;

                                case 2:
                                    memcpy(farmFieldTiles, D_801155D0, FIELD_HEIGHT * FIELD_WIDTH); 
                                    gCutsceneIndex = 0x644;
                                    break;

                                case 3:
                                    memcpy(farmFieldTiles, D_801157B0, FIELD_HEIGHT * FIELD_WIDTH); 
                                    gCutsceneIndex = 0x645;
                                    break;
                                
                                case 4:
                                    memcpy(farmFieldTiles, D_80115990, FIELD_HEIGHT * FIELD_WIDTH); 
                                    gCutsceneIndex = 0x646;
                                    break;
                                
                            }

                        } else {

                            gSeason = SUMMER;
                            tempEntrance = 0;
                            
                            switch (D_801886D2) {

                                case 0:
                                    memcpy(farmFieldTiles, D_80115B70, FIELD_HEIGHT * FIELD_WIDTH); 
                                    gCutsceneIndex = 0x647;
                                    break;

                                case 1:
                                    memcpy(farmFieldTiles, D_80115D50, FIELD_HEIGHT * FIELD_WIDTH); 
                                    gCutsceneIndex = 0x648;
                                    break;

                                case 2:
                                    memcpy(farmFieldTiles, D_80115F30, FIELD_HEIGHT * FIELD_WIDTH); 
                                    tempEntrance = 0x11;
                                    gCutsceneIndex = 0x649;
                                    break;

                                case 3:
                                    memcpy(farmFieldTiles, D_80116110, FIELD_HEIGHT * FIELD_WIDTH); 
                                    gCutsceneIndex = 0x64A;
                                    break;
                                
                                case 4:
                                    memcpy(farmFieldTiles, D_80116110, FIELD_HEIGHT * FIELD_WIDTH); 
                                    gCutsceneIndex = 0x64B;
                                    break;
                                
                            }
                            
                        }

                        setEntrance(tempEntrance);
                        loadCutscene(0);
                        loadLevelFromEntrance(tempEntrance);

                        break;

                    case 0x641 ... 0x64B:
                        gCutsceneIndex = 0x640;
                        loadCutscene(0); 
                        break;

                    default:
                        gCutsceneIndex = 0xFFFF;
                        break;
                    
                }

                if (gCutsceneIndex != 0xFFFF) {
                    f32 temp = 0.0f;
                    previousLightingRGBA.a = temp;
                    previousLightingRGBA.b = temp;
                    previousLightingRGBA.g = temp;
                    previousLightingRGBA.r = temp;
                    setMainLoopCallbackFunctionIndex(SET_AUDIO_AND_LIGHTING);
                }
                
            }
            
            if (((gCutsceneIndex == 0x4B0) || (gCutsceneIndex == NEW_YEAR_FESTIVAL))) {
                setEntitySpriteDimensions(ENTITY_ASSET_SHIPPER, 12, 12);
                setEntitySpriteDimensions(ENTITY_ASSET_ASSISTANT_CARPENTER, 12, 12);
            }

            if (checkDailyEventBit(0x5F)) {
                // empty function 
                func_800657BC();
            }

            gCutsceneCompletionFlags = 0;
            
        }

        // horse/dog race flags
        if (gCutsceneCompletionFlags & 4) {
            setGameVariableString(0x28, gRacingContext.oddsAndNames.racerNameLookup[gRaceFinishOrder[0]], 6);
            setGameVariableString(0x29, gRacingContext.oddsAndNames.racerNameLookup[gRaceFinishOrder[1]], 6);
            gCutsceneCompletionFlags &= ~4;
        }

        // sprite color flags
        if (gCutsceneCompletionFlags & 0x40) {
            setSpritePaletteIndex(0x62, 0);
            gCutsceneCompletionFlags &= ~0x40;
        }
        if (gCutsceneCompletionFlags & 0x80) {
            setSpritePaletteIndex(0x62, 1);
            gCutsceneCompletionFlags &= ~0x80;
        }

        if (gCutsceneCompletionFlags & 0x100) {

            u32 ptr1 = (u32)&_fontPalette3SegmentRomStart;
            u32 ptr2 =  (u32)&_fontPalette3SegmentRomEnd;
            
            // fontPalette3
            nuPiReadRom(ptr1, FONT_PALETTE_1_BUFFER, ptr2 - ptr1);
            setMessageBoxFont(0, 0xE, 0xE, FONT_TEXTURE_BUFFER, FONT_PALETTE_1_BUFFER);
            setMessageBoxInterpolationWithFlags(0, 1, 1);
            setMessageBoxSfx(0, 0xFF, 0xFF, 0xFF);
            gCutsceneCompletionFlags &= ~0x100;
        }

        // map flags

        if (gCutsceneCompletionFlags & 0x20000) {
            setEntrance(HOUSE_EXIT);
            // prep map
            loadLevelFromEntrance(0);
            setMapControllerRGBA(0, 0xFF, 0xFF, 0xFF, 0xFF);
            gCutsceneCompletionFlags &= ~0x20000;
        }

        // feed chickens
        if (gCutsceneCompletionFlags & 0x40000) {
            setChickenFeedSprite(0);
            gCutsceneCompletionFlags &= ~0x40000;
        }
        if (gCutsceneCompletionFlags & 0x80000) {
            setChickenFeedSprite(1);
            gCutsceneCompletionFlags &= ~0x80000;
        }
        if (gCutsceneCompletionFlags & 0x100000) {
            setChickenFeedSprite(2);
            gCutsceneCompletionFlags &= ~0x100000;
        }

        if (gCutsceneCompletionFlags & 0x200000) {
            setEntrance(0x12);
            loadLevelFromEntrance(0x12);
            setMapControllerRGBA(0, 0, 0, 0, 0);
            gCutsceneCompletionFlags &= ~0x200000;
        }

        if (gCutsceneCompletionFlags & 0x400000) {
            setEntrance(0x11);
            // prep map
            loadLevelFromEntrance(0x11);
            setMapControllerRGBA(0, 0, 0, 0, 0);
            gCutsceneCompletionFlags &= ~0x400000;
        }

        if (gCutsceneCompletionFlags & 0x800000) {
            // map object
            setBarnFodderSprite(0);
            gCutsceneCompletionFlags &= ~0x800000;
        }

        if (gCutsceneCompletionFlags & 0x1000000) {
            setBarnFodderSprite(2);
            gCutsceneCompletionFlags &= ~0x1000000;
        }

        if (gCutsceneCompletionFlags & 0x2000000) {
            // map objects
            setMapObjectAnimation(0, 0, 0x13);
            gCutsceneCompletionFlags &= ~0x2000000;
        }

        // mining
        // might be its own inline func
        if ((gCutsceneCompletionFlags & 0x4000000) && (151 < entities[ENTITY_PLAYER].animationIndices.animationIndex && entities[ENTITY_PLAYER].animationIndices.animationIndex < 176)) {

            s16 tempX, tempZ;

            tempX = ((s16)entities[ENTITY_PLAYER].coordinates.x + 240) / 32;
            tempX -= 2; 

            tempZ = ((s16)entities[ENTITY_PLAYER].coordinates.z + 240) / 32;
            tempZ -= 2;

            switch (gPlayer.direction) {

                case WEST:
                    tempX--;
                case SOUTHWEST:
                    tempZ++;
                    break;
                case NORTHWEST:
                    tempX--;
                    break;
                case NORTH:
                    tempX--;
                    tempZ--;
                    break;
                case NORTHEAST:
                    tempZ--;
                    break;
                case EAST:
                    tempX++; 
                    tempZ--;
                    break;
                case SOUTHEAST:
                    tempX++;
                    break;
                case SOUTH:
                    tempX++;
                    tempZ++;
                    break;
                
            }

            if (tempX < 10 && tempZ < 10 && tempX >= 0 && tempZ >= 0) {

                if (mineFieldTiles[tempZ][tempX] == 0) {

                    found = FALSE;

                    if (tempX == D_801886D2 && tempZ == D_80180710) {

                        setMapAdditionIndexFromCoordinates(0, 0, tempX + 2, tempZ + 2);
                        setGridToTileTextureMappings(MAIN_MAP_INDEX);
                        spawnToolEffectEntity(1, 0, (f32)((tempX * 32) - 160), 0, (f32)((tempZ * 32) - 160));

                        found = TRUE;
                        mineFieldTiles[tempZ][tempX] = 0xD9;
                            
                    } else {
                        mineFieldTiles[tempZ][tempX] = 2;
                    }
                    
                    if (!found) {

                        if (D_80189824 >= 3) {

                            if (!(powerNutBits & 0x20) && gItemBeingHeld == 0xFF && !(getRandomNumberInRange(0, 100))) {
                                gItemBeingHeld = 254;
                                powerNutBits |= 0x20;
                            }

                            if (!checkSpecialDialogueBit(0x15)) {
                                if (gItemBeingHeld == 0xFF) {
                                    if (!(getRandomNumberInRange(0, 100))) {
                                        gItemBeingHeld = 0xFD;
                                        setSpecialDialogueBit(0x15);
                                        acquireKeyItem(WEATHER_VANE);
                                    }
                                }
                            }
                                
                        }
                        
                    }

                    if (!found) {

                        if (D_80189824 >= 2) {

                            u8 temp = gItemBeingHeld;
                            u8 temp2;

                            if (gItemBeingHeld == 0xFF) {

                                if (getRandomNumberInRange(0, 100) < 4) {
                                    gPlayer.heldItem = BLUE_ROCK;
                                    gItemBeingHeld = BLUE_ROCK;
                                }

                                temp2 = gItemBeingHeld; 

                                if (gItemBeingHeld == temp) {
                                    
                                    if (getRandomNumberInRange(0, 100) < 2) {
                                        gPlayer.heldItem = RARE_METAL;
                                        gItemBeingHeld = RARE_METAL;
                                    }

                                    if (gItemBeingHeld == temp2) {

                                        if (getRandomNumberInRange(0, 100) < 3) {
                                            gPlayer.heldItem = PONTANA_ROOT;
                                            gItemBeingHeld = PONTANA_ROOT;
                                        }
                                        
                                    }
                                    
                                }
                                
                            }
                            
                        }
                        
                    }

                    if (!found) {

                        if (gItemBeingHeld == 0xFF) {

                            if (getRandomNumberInRange(0, 100) < 10) {
                                gItemBeingHeld = 0xFC;
                            }
                            
                        }
                        
                    }

                    if (!found) {

                        if (gItemBeingHeld == 0xFF) {
                            if (getRandomNumberInRange(0, 100) < 8) {
                                gPlayer.heldItem = 0x2E;
                                gItemBeingHeld = 0x2E;
                            }
                        }
                        
                    }

                    if (!found) {

                        if (gItemBeingHeld == 0xFF) {
                            if (getRandomNumberInRange(0, 100) < 5) {
                                gPlayer.heldItem = 0x31;
                                gItemBeingHeld = 0x31;
                            }
                        }
                        
                    }
                    
                } 
                
            }
            
            gCutsceneCompletionFlags &= ~0x4000000;
        
        }

        if ((gCutsceneCompletionFlags & 0x8000000) && gItemBeingHeld < 252) {
            initializePlayerHeldItem();
        }

        gCutsceneCompletionFlags &= ~0x8000000;

        if (gCutsceneCompletionFlags & 0x1000) {
            setDailyEventBit(6);
        } else {
            toggleDailyEventBit(6);
        }

        if (gCutsceneCompletionFlags & 0x2000) {
            setDailyEventBit(0x13);
        } else {
            toggleDailyEventBit(0x13);
        }

        if (gCutsceneCompletionFlags & 0x4000) {
            setDailyEventBit(0x12);
        } else {
            toggleDailyEventBit(0x12);
        }

        if (gCutsceneCompletionFlags & 0x400) {
            setDailyEventBit(0x15);
        } else {
            toggleDailyEventBit(0x15);
        }

        if (gCutsceneCompletionFlags & 0x200) {
            setDailyEventBit(0x28);
        }

        if (checkDailyEventBit(0x2B)) {
            toggleDailyEventBit(0x2B);
            gPlayer.currentStamina += adjustValue(gPlayer.currentStamina, 50, gMaximumStamina);
        }

        if (checkDailyEventBit(0x2A)) {
            toggleDailyEventBit(0x2A);
            adjustAllAnimalAffection(5);
        }

        if (gCutsceneCompletionFlags & 0x8000) {
            clearHeldItemsAtEndOfDay();
            gCutsceneCompletionFlags &= ~0x8000;
        }

        if (gCutsceneFlags & 8) {
            // initialize animal entities
            initializeAnimalEntities();
            gCutsceneFlags &= ~8;
        }

        if (gCutsceneFlags & 0x10) {
            // deactivate animal entities
            func_8008779C();
            gCutsceneFlags &= ~0x10;
        }

        if (gCutsceneCompletionFlags & 0x10000000) {
            initializeAnimalEntities();
            gCutsceneCompletionFlags &= ~0x10000000;
        }

        if (gCutsceneCompletionFlags & 0x20000000) {
            // deactivate animal entities
            func_8008779C();
            gCutsceneCompletionFlags &= ~0x20000000;
        }

        // handle life event bit updates

        if (checkLifeEventBit(0x4B)) {
            toggleLifeEventBit(0x4B);
            acquireKeyItem(MARBLES);
            removePinkCatMintFlowerFromFarm();
        }
        
        if (checkLifeEventBit(0x82)) {
            toggleLifeEventBit(0x82);
            removeKeyItem(MARBLES);
            setSpecialDialogueBit(0x1F);
        }
        
        if (checkLifeEventBit(0x49)) {
            toggleLifeEventBit(0x49);
            storeTool(CHICKEN_FEED);
            chickenFeedQuantity += adjustValue(chickenFeedQuantity, 10, 999);
        }

        // seeds
        if (checkLifeEventBit(0x4A)) {

            toggleLifeEventBit(0x4A);
            
            switch (getRandomNumberInRange(0, 5)) {                    
                
                case 0:                                 
                    storeTool(0xA);
                    D_801FC154 += adjustValue(D_801FC154, 1, 20);
                    return;
                case 1:                                 
                    storeTool(0xB);
                    D_80204DF4 += adjustValue(D_80204DF4, 1, 20);
                    return;
                case 2:                                 
                    storeTool(0xC);
                    D_8018A725 += adjustValue(D_8018A725, 1, 20);
                    return;
                case 3:                                 
                    storeTool(0xD);
                    D_8013DC2C += adjustValue(D_8013DC2C, 1, 20);
                    return;
                case 4:                                 
                    storeTool(0xE);
                    D_801FAD91 += adjustValue(D_801FAD91, 1, 20);
                    return;
                case 5:                                 
                    storeTool(0xF);
                    D_80237458 += adjustValue(D_80237458, 1, 20);
                    break;
                
            }
            
        }
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", loadCutscene);

void loadCutscene(u8 arg0) {
    
    u16 bytecodeSegmentIndex;

    if (gCutsceneIndex >= 0x640) {
        
        gCameraRotationOffset = 0;
        setInitialMapRotation(MAIN_MAP_INDEX, SOUTH);
        bytecodeSegmentIndex = 35;
        
    } else if (gCutsceneIndex >= 0x60E) {
    
        gCameraRotationOffset = 0;
        setInitialMapRotation(MAIN_MAP_INDEX, SOUTH);
        bytecodeSegmentIndex = 31;        
    
    } else if (gCutsceneIndex >= 0x5DC) {
             
        gCameraRotationOffset = 0;
        setInitialMapRotation(MAIN_MAP_INDEX, SOUTH);

        memcpy(farmFieldTiles, D_80114E50, FIELD_HEIGHT * FIELD_WIDTH);

        bytecodeSegmentIndex = 32;
        
    } else if (gCutsceneIndex >= 0x5AA) {
        
        gCameraRotationOffset = 0;
        setInitialMapRotation(MAIN_MAP_INDEX, SOUTH);
        
        gSeason = SPRING;

        memcpy(farmFieldTiles, D_80113580, FIELD_HEIGHT * FIELD_WIDTH);

        bytecodeSegmentIndex = 30;
        
    } else if (gCutsceneIndex >= 0x578) {
    
        bytecodeSegmentIndex = 29;
        setEntitySpriteDimensions(ENTITY_ASSET_SHIPPER, 12, 24);
        setEntitySpriteDimensions(ENTITY_ASSET_ASSISTANT_CARPENTER, 12, 24);
    
    } else if (gCutsceneIndex >= 0x546) {
        bytecodeSegmentIndex = 28;
    } else if (gCutsceneIndex >= 0x514) {
        bytecodeSegmentIndex = 27;  
    } else if (gCutsceneIndex >= 0x4E2) {
        bytecodeSegmentIndex = 26;
    } else if (gCutsceneIndex >= 0x4B0) {
        
        bytecodeSegmentIndex = 25;
        setEntitySpriteDimensions(ENTITY_ASSET_SHIPPER, 12, 24);
        setEntitySpriteDimensions(ENTITY_ASSET_ASSISTANT_CARPENTER, 12, 24);
        
    } else if (gCutsceneIndex >= 0x47E) {
        bytecodeSegmentIndex = 24;
    } else if (gCutsceneIndex >= 0x44C) {
        bytecodeSegmentIndex = 23;
    } else if (gCutsceneIndex >= 0x41A) {
        bytecodeSegmentIndex = 22;
    } else if (gCutsceneIndex >= 0x3E8) {
        bytecodeSegmentIndex = 20;    
    } else if (gCutsceneIndex >= 0x3B6) {
        bytecodeSegmentIndex = 19;
    } else if (gCutsceneIndex >= 0x384) {
        bytecodeSegmentIndex = 18;  
    } else if (gCutsceneIndex >= 0x352) {
        bytecodeSegmentIndex = 17;  
    } else if (gCutsceneIndex >= 0x320) {
        bytecodeSegmentIndex = 10;  
    } else if (gCutsceneIndex >= 0x2EE) {
        bytecodeSegmentIndex = 16;  
    } else if (gCutsceneIndex >= 0x2BC) {
        bytecodeSegmentIndex = 13;  
    } else if (gCutsceneIndex > 0x289) {
        bytecodeSegmentIndex = 12;
    } else if (gCutsceneIndex >= 0x258) {
        bytecodeSegmentIndex = 21;
    } else if (gCutsceneIndex >= 0x1F4) {
        bytecodeSegmentIndex = 15;    
    } else if (gCutsceneIndex >= 0x190) {
        bytecodeSegmentIndex = 14;
    } else if (gCutsceneIndex >= 0x12C) {
        bytecodeSegmentIndex = 11;
    } else if (gCutsceneIndex >= 0x64) {
        
        if (arg0) {
            bytecodeSegmentIndex = 7;
        } else  {

            bytecodeSegmentIndex = 8;

            switch (gCutsceneIndex) {

                case 0xDC:
                    gCameraRotationOffset = 0;
                    setInitialMapRotation(MAIN_MAP_INDEX, SOUTH);
                    break;
                case 0xE4 ... 0xEE:
                    gCameraRotationOffset = 0;
                    setInitialMapRotation(MAIN_MAP_INDEX, SOUTH);
                    break;
                default:
                    break;
            
            }
            
        } 
        
    } else {
        
        bytecodeSegmentIndex = 9;
        
        if (gCutsceneIndex == 0x27) {
            gCameraRotationOffset = 0;
            setInitialMapRotation(MAIN_MAP_INDEX, SOUTH);
        } 
        
    }

    nuPiReadRom(cutsceneBytecodeAddresses[bytecodeSegmentIndex].romAddrStart, cutsceneBankLoadAddresses[bytecodeSegmentIndex], cutsceneBytecodeAddresses[bytecodeSegmentIndex].romAddrEnd - cutsceneBytecodeAddresses[bytecodeSegmentIndex].romAddrStart);
    
    gCutsceneCompletionFlags = 0;
    
    spawnCutsceneExecutor(cutsceneExecutorIndices[bytecodeSegmentIndex], cutsceneEntryPointAddresses[bytecodeSegmentIndex]);
    togglePauseEntities();
    
    gCutsceneBytecodeSegmentIndex = bytecodeSegmentIndex;
    
    gCutsceneFlags |= CUTSCENE_ACTIVE;
    
    toggleDailyEventBit(0x53);
    
    switch (gCutsceneIndex) {
        case STARRY_NIGHT_MOON_MOUNTAIN:
        case MARIA_WEDDING ... KAREN_WEDDING:
        case STARRY_NIGHT_FESTIVAL:
        case 0x20B:
        case 0x2F4:
        case SOWING_FESTIVAL_POTION_SHOP_DEALER ... SOWING_FESTIVAL_HARRIS:
        case HORSE_RACE_SQUARE: 
        case FLOWER_FESTIVAL:
        case 0x41A:
        case SEA_FESTIVAL:
        case COW_FESTIVAL:
        case 0x4B0:
        case EGG_FESTIVAL:
        case DOG_RACE_SQUARE:
        case 0x546 ... 0x548:
        case NEW_YEAR_FESTIVAL:
        case 0x5AA:
        case INTRO:
        case DEMO_CUTSCENE_1: 
        case DEMO_CUTSCENE_2:
        case DEMO_CUTSCENE_3:
        case EVALUATION:
        case HOW_TO_PLAY_CUTSCENE:
            setDailyEventBit(0x4B);
            break;
    }
    
}