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
#include "game/groundObjects.h"
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

#include "assetIndices/dialogueMenus.h"
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
u16 setPondCutscenes(void);                        
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
// set in 2 cutscenes
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
      { &_houseCutscenesSegmentRomStart, &_houseCutscenesSegmentRomEnd },
      { &_roadCutscenesSegmentRomStart, &_roadCutscenesSegmentRomEnd },
      { &_mountainCutscenesSegmentRomStart, &_mountainCutscenesSegmentRomEnd },
      { &_ranchCutscenesSegmentRomStart, &_ranchCutscenesSegmentRomEnd },
      { &_vineyardCutscenesSegmentRomStart, &_vineyardCutscenesSegmentRomEnd },
      { &_village1CutscenesSegmentRomStart, &_village1CutscenesSegmentRomEnd },
      { &_village2CutscenesSegmentRomStart, &_village2CutscenesSegmentRomEnd },
      { &_beachCutscenesSegmentRomStart, &_beachCutscenesSegmentRomEnd },
      { &_sowingFestivalSegmentRomStart, &_sowingFestivalSegmentRomEnd },
      { &_horseRaceSegmentRomStart, &_horseRaceSegmentRomEnd },
      { &_flowerFestivalSegmentRomStart, &_flowerFestivalSegmentRomEnd },
      { &_vegetableFestivalSegmentRomStart, &_vegetableFestivalSegmentRomEnd },
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

static inline void transitionCutscenes(u16 cutsceneIndex, u16 spawnPoint) {
    setSpawnPoint(spawnPoint);
    gCutsceneIndex = cutsceneIndex;
    loadCutscene(FALSE);
    loadMapAtSpawnPoint(spawnPoint);
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", initializeCutscene);

inline void initializeCutscene(u16 segmentIndex) {

    nuPiReadRom(cutsceneBytecodeAddresses[segmentIndex].romAddrStart, cutsceneBankLoadAddresses[segmentIndex], cutsceneBytecodeAddresses[segmentIndex].romAddrEnd - cutsceneBytecodeAddresses[segmentIndex].romAddrStart);

    gCutsceneCompletionFlags = 0;

    spawnCutsceneExecutor(cutsceneExecutorIndices[segmentIndex], cutsceneEntryPointAddresses[segmentIndex]);
    
    togglePauseEntities();
    
    gCutsceneBytecodeSegmentIndex = segmentIndex;

    gCutsceneFlags |= CUTSCENE_ACTIVE;

    clearDailyEventBit(EAT_BREAKFAST);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setLevelCutscene);

u16 setLevelCutscene(u16 mapIndex) {
    
    u16 bytecodeSegmentIndex = 0xFFFF;
    
    if (!(gCutsceneFlags & CUTSCENE_ACTIVE)) {
        
        gCutsceneFlags &= ~CUTSCENE_SUPPRESS_NPC_SETUP;

        switch (mapIndex) {
            case FARM:
                bytecodeSegmentIndex = setFarmCutscenes();
                break;
            case MOUNTAIN_1:
                bytecodeSegmentIndex = setMountain1Cutscenes();
                break;
            case MOUNTAIN_2:
                bytecodeSegmentIndex = setMountain2Cutscenes();
                break;
            case TOP_OF_MOUNTAIN_1:
                bytecodeSegmentIndex = setTopOfMountain1Cutscenes();
                break;
            case MOON_MOUNTAIN:
                bytecodeSegmentIndex = setMoonMountainCutscenes();
                break;
            case POND:
                bytecodeSegmentIndex = setPondCutscenes();
                break;
            case HARVEST_SPRITE_CAVE:
                bytecodeSegmentIndex = setHarvestSpriteCaveCutscenes();
                break;
            case CAVE:
                bytecodeSegmentIndex = setCaveCutscenes();
                break;
            case HOUSE:
                bytecodeSegmentIndex = setHouseCutscenes();
                break;
            case KITCHEN:
                bytecodeSegmentIndex = setKitchenCutscenes();
                break;
            case RANCH:
                bytecodeSegmentIndex = setRanchCutscenes();
                break;
            case RANCH_STORE:
                bytecodeSegmentIndex = setRanchStoreCutscenes();
                break;
            case VINEYARD:
                bytecodeSegmentIndex = setVineyardCutscenes();
                break;
            case VINEYARD_HOUSE:
                bytecodeSegmentIndex = setVineyardHouseCutscenes();
                break;
            case VINEYARD_CELLAR:
                bytecodeSegmentIndex = setVineyardCellarCutscenes();
                break;
            case VILLAGE_1:
                bytecodeSegmentIndex = setVillage1Cutscenes();
                break;
            case VILLAGE_2:
                bytecodeSegmentIndex = setVillage2Cutscenes();
                break;
            case RICK_STORE:
                bytecodeSegmentIndex = setRickStoreCutscenes();
                break;
            case FLOWER_SHOP:
                bytecodeSegmentIndex = setFlowerShopCutscenes();
                break;
            case BAKERY:
                bytecodeSegmentIndex = setBakeryCutscenes();
                break;
            case MAYOR_HOUSE:
                bytecodeSegmentIndex = setMayorHouseCutscenes();
                break;
            case LIBRARY:
                bytecodeSegmentIndex = setLibraryCutscenes();
                break;
            case CHURCH:
                bytecodeSegmentIndex = setChurchCutscenes();
                break;
            case TAVERN:
                bytecodeSegmentIndex = setTavernCutscenes();
                break;
            case SQUARE:
                bytecodeSegmentIndex = setSquareCutscenes();
                break;
            case RACE_TRACK:
                bytecodeSegmentIndex = setRaceTrackCutscenes();
                break;
            case BEACH:
                bytecodeSegmentIndex = setBeachCutscenes();
                break;
            case ROAD:
                bytecodeSegmentIndex = setRoadCutscenes();
                break;
            default:
                break;
            }
        
        if (bytecodeSegmentIndex != 0xFFFF) {
            initializeCutscene(bytecodeSegmentIndex);
        }
        
    }
    
    return bytecodeSegmentIndex;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setFarmVisitCutscenes);

// arg0 = cutscene type; 0 = show up while working; 1 = morning/show up on map entry
u16 setFarmVisitCutscenes(bool morningVisit) {

    bool set = FALSE;
    u16 result = 0xFFFF;
    u8 tempGirl;
    
    if (!checkLifeEventBit(MAYOR_TOUR)) {
        setLifeEventBit(MAYOR_TOUR);
        gCutsceneIndex = MAYOR_VILLAGE_TOUR;
        gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
        loadCutscene(morningVisit);
        set = TRUE;
    }

    if (gSpawnPointIndex != 0x6F) {
        
        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gYear == 1 && gSeason == AUTUMN && gDayOfMonth == 22 && (5 < gHour && gHour < 18)) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = 223;
                loadCutscene(morningVisit);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gYear == 1 && gSeason == WINTER && gDayOfMonth == 11 && (5 < gHour && gHour < 18)) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = 222;
                loadCutscene(morningVisit);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == SPRING && gDayOfMonth == 16 && (5 < gHour && gHour < 15) && (horseInfo.flags & HORSE_ACTIVE) && horseInfo.grown == TRUE) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = DOUG_FARM_HORSE_RACE;
                loadCutscene(morningVisit);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == AUTUMN && gDayOfMonth == 27 && (5 < gHour && gHour < 15) && (horseInfo.flags & HORSE_ACTIVE) && horseInfo.grown == TRUE) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = 126;
                loadCutscene(morningVisit);
                set = TRUE;
            }
        }

        if (!set) {
            
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == WINTER && gDayOfMonth == 18 && (5 < gHour && gHour < 15)) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = DOUG_FARM_DOG_RACE;
                loadCutscene(morningVisit);
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
                
                loadCutscene(morningVisit);
                set = TRUE;
        
            }
        
        }

        if (!set) {

            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == AUTUMN && gDayOfMonth == 2 && (5 < gHour && gHour < 9) && npcAffection[KENT] >= 30) {
                setDailyEventBit(FARM_MORNING_VISIT);
                
                if (getTotalFarmAnimalsByType(2)) {
                    gCutsceneIndex = 102;
                    setGameVariableString(14, gFarmAnimals[getCowWithHighestAffection()].name, 6);
                } else {
                    gCutsceneIndex = 101;
                }
                
                loadCutscene(morningVisit);
                set = TRUE;

            }

        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == WINTER && gDayOfMonth == 9 && (5 < gHour && gHour < 9) && npcAffection[KENT] >= 30) {
                setDailyEventBit(FARM_MORNING_VISIT);
                
                if (gYear == 1) {
                    gCutsceneIndex = KENT_FARM_THANKSGIVING_FESTIVAL;
                } else {
                    gCutsceneIndex = 104;
                }
                
                loadCutscene(morningVisit);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == SPRING && gDayOfMonth == 16 && (5 < gHour && gHour < 9)) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = MAYOR_FARM_HORSE_RACE;
                loadCutscene(morningVisit);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == SUMMER && gDayOfMonth == 17 && (5 < gHour && gHour < 9)) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = MAYOR_FARM_FIREFLY_FESTIVAL;
                loadCutscene(morningVisit);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == SUMMER && gDayOfMonth == 23 && (5 < gHour && gHour < 9)) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = MAYOR_FARM_SEA_FESTIVAL;
                loadCutscene(morningVisit);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == AUTUMN && gDayOfMonth == 11 && (5 < gHour && gHour < 9)) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = MAYOR_FARM_HARVEST_FESTIVAL;
                loadCutscene(morningVisit);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == AUTUMN && gDayOfMonth == 19 && (5 < gHour && gHour < 9)) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = MAYOR_FARM_EGG_FESTIVAL;
                loadCutscene(morningVisit);
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
                
                loadCutscene(morningVisit);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == SPRING && gDayOfMonth == 19 && (5 < gHour && gHour < 9)) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = BASIL_FARM_FLOWER_FESTIVAL;
                loadCutscene(morningVisit);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == SUMMER && gDayOfMonth == 8 && (5 < gHour && gHour < 9)) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = BASIL_FARM_VEGETABLE_FESTIVAL;
                loadCutscene(morningVisit);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(POPURI_CUTSCENE_DAILY) && npcAffection[POPURI] >= 50 && gSeason == SPRING && gDayOfMonth == 22 && (5 < gHour && gHour < 9) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
                setDailyEventBit(POPURI_CUTSCENE_DAILY);
                gCutsceneIndex = 156;
                loadCutscene(morningVisit);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(ELLI_CUTSCENE_DAILY) && npcAffection[ELLI] >= 50 && gSeason == AUTUMN && gDayOfMonth == 11 && (6 < gHour && gHour < 9) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {
                setDailyEventBit(ELLI_CUTSCENE_DAILY);
                gCutsceneIndex = 168;
                loadCutscene(morningVisit);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(ANN_CUTSCENE_DAILY) && npcAffection[ANN] >= 80 && gSeason == SPRING && gDayOfMonth == 16 && (6 < gHour && gHour < 9) && !checkLifeEventBit(MARRIED) && checkLifeEventBit(ENTERED_HORSE_RACE) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
                setDailyEventBit(ANN_CUTSCENE_DAILY);
                gCutsceneIndex = 179;
                loadCutscene(morningVisit);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(MARIA_CUTSCENE_DAILY) && npcAffection[MARIA] >= 50 && gSeason == SUMMER && gDayOfMonth == 1 && gHour == 8 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
                setDailyEventBit(MARIA_CUTSCENE_DAILY);
                gCutsceneIndex = MARIA_FIREWORKS_INVITATION;
                loadCutscene(morningVisit);
                set = TRUE;
            }            
        }
        
        if (!set) {
            if (!checkDailyEventBit(POPURI_CUTSCENE_DAILY) && npcAffection[POPURI] >= 50 && gSeason == SUMMER && gDayOfMonth == 1 && gHour == 10 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
                setDailyEventBit(POPURI_CUTSCENE_DAILY);
                gCutsceneIndex = POPURI_FIREWORKS_INVITATION;
                loadCutscene(morningVisit);
                set = TRUE;
            }            
        }

        if (!set) {
            if (!checkDailyEventBit(ELLI_CUTSCENE_DAILY) && npcAffection[ELLI] >= 50 && gSeason == SUMMER && gDayOfMonth == 1 && gHour == 12 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {
                setDailyEventBit(ELLI_CUTSCENE_DAILY);
                gCutsceneIndex = ELLI_FIREWORKS_INVITATION;
                loadCutscene(morningVisit);
                set = TRUE;
            }            
        }

        if (!set) {
            if (!checkDailyEventBit(ANN_CUTSCENE_DAILY) && npcAffection[ANN] >= 50 && gSeason == SUMMER && gDayOfMonth == 1 && gHour == 14 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
                setDailyEventBit(ANN_CUTSCENE_DAILY);
                gCutsceneIndex = ANN_FIREWORKS_INVITATION;
                loadCutscene(morningVisit);
                set = TRUE;
            }            
        }

        if (!set) {
            if (!checkDailyEventBit(KAREN_CUTSCENE_DAILY) && npcAffection[KAREN] >= 50 && gSeason == SUMMER && gDayOfMonth == 1 && gHour == 16 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
                setDailyEventBit(KAREN_CUTSCENE_DAILY);
                gCutsceneIndex = KAREN_FIREWORKS_INVITATION;
                loadCutscene(morningVisit);
                set = TRUE;
            }            
        }
        
        if (!set) {
            if (!checkDailyEventBit(MARIA_CUTSCENE_DAILY) && npcAffection[MARIA] >= 140 && gSeason == WINTER && gDayOfMonth == 10 && gHour == 8 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
                setDailyEventBit(MARIA_CUTSCENE_DAILY);
                gCutsceneIndex = MARIA_THANKSGIVING_GIFT;
                loadCutscene(morningVisit);
                set = TRUE;
            }            
        }        

        if (!set) {
            if (!checkDailyEventBit(POPURI_CUTSCENE_DAILY) && npcAffection[POPURI] >= 140 && gSeason == WINTER && gDayOfMonth == 10 && gHour == 10 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
                setDailyEventBit(POPURI_CUTSCENE_DAILY);
                gCutsceneIndex = POPURI_THANKSGIVING_GIFT;
                loadCutscene(morningVisit);
                set = TRUE;
            }            
        }        

        if (!set) {
            if (!checkDailyEventBit(ELLI_CUTSCENE_DAILY) && npcAffection[ELLI] >= 140 && gSeason == WINTER && gDayOfMonth == 10 && gHour == 12 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {
                setDailyEventBit(ELLI_CUTSCENE_DAILY);
                gCutsceneIndex = ELLI_THANKSGIVING_GIFT;
                loadCutscene(morningVisit);
                set = TRUE;
            }            
        }        

        if (!set) {
            if (!checkDailyEventBit(ANN_CUTSCENE_DAILY) && npcAffection[ANN] >= 140 && gSeason == WINTER && gDayOfMonth == 10 && gHour == 14 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
                setDailyEventBit(ANN_CUTSCENE_DAILY);
                gCutsceneIndex = ANN_THANKSGIVING_GIFT;
                loadCutscene(morningVisit);
                set = TRUE;
            }            
        }    

        if (!set) {
            if (!checkDailyEventBit(KAREN_CUTSCENE_DAILY) && npcAffection[KAREN] >= 140 && gSeason == WINTER && gDayOfMonth == 10 && gHour == 16 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
                setDailyEventBit(KAREN_CUTSCENE_DAILY);
                gCutsceneIndex = KAREN_THANKSGIVING_GIFT;
                loadCutscene(morningVisit);
                set = TRUE;
            }            
        }       

        if (!set) {
            if (!checkDailyEventBit(MARIA_CUTSCENE_DAILY) && npcAffection[MARIA] >= 100 && gSeason == WINTER && gDayOfMonth == 24 && gHour == 8 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
                setDailyEventBit(MARIA_CUTSCENE_DAILY);
                gCutsceneIndex = 149;
                loadCutscene(morningVisit);
                set = TRUE;
            }            
        }        

        if (!set) {
            if (!checkDailyEventBit(POPURI_CUTSCENE_DAILY) && npcAffection[POPURI] >= 100 && gSeason == WINTER && gDayOfMonth == 24 && gHour == 10 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
                setDailyEventBit(POPURI_CUTSCENE_DAILY);
                gCutsceneIndex = 160;
                loadCutscene(morningVisit);
                set = TRUE;
            }            
        }        

        if (!set) {
            if (!checkDailyEventBit(ELLI_CUTSCENE_DAILY) && npcAffection[ELLI] >= 100 && gSeason == WINTER && gDayOfMonth == 24 && gHour == 12 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {
                setDailyEventBit(ELLI_CUTSCENE_DAILY);
                gCutsceneIndex = 172;
                loadCutscene(morningVisit);
                set = TRUE;
            }            
        }       

        if (!set) {
            if (!checkDailyEventBit(ANN_CUTSCENE_DAILY) && npcAffection[ANN] >= 100 && gSeason == WINTER && gDayOfMonth == 24 && gHour == 14 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
                setDailyEventBit(ANN_CUTSCENE_DAILY);
                gCutsceneIndex = 183;
                loadCutscene(morningVisit);
                set = TRUE;
            }            
        }       

        if (!set) {
            if (!checkDailyEventBit(KAREN_CUTSCENE_DAILY) && npcAffection[KAREN] >= 100 && gSeason == WINTER && gDayOfMonth == 24 && gHour == 16 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
                setDailyEventBit(KAREN_CUTSCENE_DAILY);
                gCutsceneIndex = 196;
                loadCutscene(morningVisit);
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
                                    setLifeEventBit(PAINTING_FROM_MARIA);
                                    gCutsceneIndex = 153;
                                } else {
                                    gCutsceneIndex = 152;
                                }
                                
                                gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
                                loadCutscene(morningVisit);
                                set = TRUE;
                            }

                            break;
                        
                        case 2:
                            
                            if (!checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
                                
                               if (npcAffection[POPURI] >= 160) {
                                    acquireKeyItem(POTPURRI);
                                    gCutsceneIndex = 164;
                                } else {
                                    gCutsceneIndex = 163;
                                }
                                
                                gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
                                loadCutscene(morningVisit);
                                set = TRUE;                                
                            }

                            break;

                        case 3:

                            if (!checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {
                                
                               if (npcAffection[ELLI] >= 160) {
                                    acquireKeyItem(EMBROIDERED_HANDKERCHIEF);
                                    gCutsceneIndex = 176;
                                } else {
                                    gCutsceneIndex = 175;
                                }
                                
                                gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
                                loadCutscene(morningVisit);
                                set = TRUE;                                
                            }

                            break;

                        case 4:

                            if (!checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
                                
                               if (npcAffection[ANN] >= 160) {
                                    acquireKeyItem(HAND_KNIT_SOCKS);
                                    gCutsceneIndex = 187;
                                } else {
                                    gCutsceneIndex = 186;
                                }
                                
                                gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
                                loadCutscene(morningVisit);
                                set = TRUE;                                
                            }

                            break;   

                        case 5:

                            if (!checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
                                
                               if (npcAffection[KAREN] >= 160) {
                                    acquireKeyItem(LUCKY_BRACELET);
                                    gCutsceneIndex = 200;
                                } else {
                                    gCutsceneIndex = 199;
                                }
                                
                                gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
                                loadCutscene(morningVisit);
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
                clearLifeEventBit(BASIL_GONE);       
                
                if (gYear != 1) {
                    if (npcAffection[BASIL] >= 150) {
                        gCutsceneIndex = BASIL_POWER_NUT_GIFT;
                    } else {
                        gCutsceneIndex = BASIL_RETURN;
                    }
                } else {
                    gCutsceneIndex = BASIL_INTRO;
                }
                
                loadCutscene(morningVisit);
                set = TRUE;
            }            
        }

        if (!set) {
            if (checkLifeEventBit(BASIL_IN_TOWN) && ((gSeason == AUTUMN && gDayOfMonth >= 5) || gSeason == WINTER) && (5 < gHour && gHour < 17)) {
                clearLifeEventBit(BASIL_IN_TOWN);
                setLifeEventBit(BASIL_GONE);      
                gCutsceneIndex = BASIL_LEAVING;
                loadCutscene(morningVisit);
                set = TRUE;
            }
        }

        if (!set && morningVisit == FALSE) {
            if (!checkLifeEventBit(CLIFF_ARRIVED) && (gSeason == SPRING && !(gDayOfMonth < 18) || !(gSeason < SUMMER))) {
                setLifeEventBit(CLIFF_ARRIVED);      
                gCutsceneIndex = CLIFF_INTRO;
                loadCutscene(morningVisit);      
                set = TRUE;
            }
        } 

        // Cliff leaving
        if (!set) {
            if (checkLifeEventBit(CLIFF_ARRIVED) && !checkLifeEventBit(CLIFF_LEAVING) && gSeason == WINTER && (5 < gHour && gHour < 18) && gHarvestKing != 4 && gHarvestCoinFinder != 4) {
                
                if (checkLifeEventBit(MARRIED) && gWife == ANN && npcAffection[CLIFF] >= 150 && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
                    setLifeEventBit(CLIFF_LEAVING);
                    setLifeEventBit(CLIFF_GONE);
                    gCutsceneIndex = 213;
                    loadCutscene(morningVisit);
                    set = TRUE;                    
                }
                
                if (npcAffection[CLIFF] < 51 && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {              
                    setLifeEventBit(CLIFF_LEAVING);
                    setLifeEventBit(CLIFF_GONE);
                    gCutsceneIndex = CLIFF_LEAVING_DEFAULT;
                    loadCutscene(morningVisit);
                    set = TRUE;                                
                } 
            }
        }

        if (!set) {
            if (!checkLifeEventBit(KENT_CHICKEN_VISIT) && checkDailyEventBit(KENT_ACQUIRED_CHICKEN_VISIT_DAILY) && npcAffection[KENT] >= 5 && (5 < gHour && gHour < 9)) {
                setLifeEventBit(KENT_CHICKEN_VISIT);
                gCutsceneIndex = 105;
                loadCutscene(morningVisit);
                set = TRUE;                               
            }
        }
        
        if (!set) {
            if (!checkLifeEventBit(KENT_SHEEP_VISIT) && checkDailyEventBit(KENT_ACQUIRED_SHEEP_VISIT_DAILY) && npcAffection[KENT] >= 5 && (5 < gHour && gHour < 9)) {
                setLifeEventBit(KENT_SHEEP_VISIT);
                gCutsceneIndex = 106;
                loadCutscene(morningVisit);
                set = TRUE;                               
            }
        }
        
        if (!set) {

            if (!checkLifeEventBit(KENT_COW_VISIT) && checkDailyEventBit(KENT_ACQUIRED_COW_VISIT_DAILY) && npcAffection[KENT] >= 5 && gWeather == SUNNY && (5 < gHour && gHour < 9)) {
                
                setLifeEventBit(KENT_COW_VISIT);

                // get cow + grass count
                if (!func_8009B828(0)) {
                    gCutsceneIndex = 108;
                } else {
                    gCutsceneIndex = 107;
                }
                
                loadCutscene(morningVisit);
                set = TRUE;                               
            }
        }
        
        if (!set) {
            if (!checkLifeEventBit(KENT_CHICK_VISIT) && checkDailyEventBit(KENT_CHICK_HATCHED_VISIT_DAILY) && npcAffection[KENT] >= 5 && (5 < gHour && gHour < 9)) {
                setLifeEventBit(KENT_CHICK_VISIT);
                gCutsceneIndex = 111;
                loadCutscene(morningVisit);
                set = TRUE;                               
            }
        }            
        
        if (!set) {
            if (!checkLifeEventBit(KENT_HORSE_VISIT) && checkDailyEventBit(KENT_ACQUIRED_HORSE_VISIT_DAILY) && npcAffection[KENT] >= 5 && (5 < gHour && gHour < 9)) {
                setLifeEventBit(KENT_HORSE_VISIT);
                gCutsceneIndex = 109;
                loadCutscene(morningVisit);
                set = TRUE;                               
            }
        }     

        if (!set) {
            if (!checkLifeEventBit(KENT_HORSE_GROWN_VISIT) && checkDailyEventBit(KENT_HORSE_GROWN_VISIT_DAILY) && npcAffection[KENT] >= 5 && (5 < gHour && gHour < 9)) {
                setLifeEventBit(KENT_HORSE_GROWN_VISIT);
                gCutsceneIndex = 110;
                loadCutscene(morningVisit);
                set = TRUE;                               
            }
        }     

        if (!set) {
            
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gForecast == TYPHOON && ((npcAffection[HARVEST_SPRITE_1] + npcAffection[HARVEST_SPRITE_2] + npcAffection[HARVEST_SPRITE_3]) / 3) >= 120 && (5 < gHour && gHour < 9)) {

                if ((getTotalChickenCount() + getTotalFarmAnimalsCount()) >= 2) {           
                    setDailyEventBit(FARM_MORNING_VISIT);
                    gCutsceneIndex = 227;
                    loadCutscene(morningVisit);
                    set = TRUE;                               
                }
            }

        }    

        if (!set) {
            
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && ((npcAffection[CARPENTER_1] + npcAffection[CARPENTER_2] + npcAffection[MASTER_CARPENTER]) / 3) >= 120 && (5 < gHour && gHour < 9)) {

                if (checkDailyEventBit(EARTHQUAKE_CARPENTER_VISIT_DAILY)) {           
                    setDailyEventBit(FARM_MORNING_VISIT);
                    gCutsceneIndex = 221;
                    loadCutscene(morningVisit);
                    set = TRUE;                               
                }
            }

        }    

        if (!set) {
            if (!checkLifeEventBit(DOUG_FARM_FAVOR_OFFERED) && getTotalFarmAnimalsCount() < 6 && gYear == 1 && gSeason == AUTUMN && gDayOfMonth == 30 && (5 < gHour && gHour < 24)) {
                setLifeEventBit(DOUG_FARM_FAVOR_OFFERED);
                gCutsceneIndex = DOUG_FARM_FAVOR;
                loadCutscene(morningVisit);
                set = TRUE;    
            }
        }
        
        if (!set && morningVisit == FALSE) {
            if (!checkLifeEventBit(DOUG_DROPPING_OFF_COWS_FAVOR) && checkLifeEventBit(MRS_MANA_COWS_EVENT) && gSeason == WINTER && gDayOfMonth == 1 && (5 < gHour && gHour < 10) && gSpawnPointIndex == FARM_SPAWN_POINT_1) {
                setLifeEventBit(DOUG_DROPPING_OFF_COWS_FAVOR);
                gCutsceneIndex = DOUG_DROPPING_OFF_COWS;
                loadCutscene(morningVisit);
                set = TRUE;    
            }
        }
        
        if (!set && morningVisit == FALSE) {
            if (checkLifeEventBit(MRS_MANA_COWS_EVENT) && gSeason == WINTER && gDayOfMonth == 5 && (5 < gHour && gHour < 10) && gSpawnPointIndex == FARM_SPAWN_POINT_1) {
                clearLifeEventBit(MRS_MANA_COWS_EVENT);
                gCutsceneIndex = MISS_MANA_PICKUP_COWS;
                loadCutscene(morningVisit);
                set = TRUE;    
            }
        }      

        if (!set) {
            if (!checkLifeEventBit(KENT_FARM_GIFT_FLOWERS) && npcAffection[KENT] >= 50 && gSeason == SUMMER && (14 < gHour && gHour < 17)) {
                setLifeEventBit(KENT_FARM_GIFT_FLOWERS);
                gCutsceneIndex = 112;
                loadCutscene(morningVisit);
                set = TRUE;    
            }
        }
        
        if (!set) {
            if (!checkLifeEventBit(PUPPIES) && (npcAffection[KENT] >= 150 || npcAffection[KENT] + npcAffection[STU] >= 250) && (5 < gHour && gHour < 18)) {
                setLifeEventBit(PUPPIES);
                gCutsceneIndex = KENT_AND_STU_FARM_PUPPIES;
                loadCutscene(morningVisit);
                set = TRUE;    
            }
        }
        
        if (!set) {
            if (!checkLifeEventBit(STU_MOONDROP_FLOWER_VISIT) && getFarmMoondropFlowerCount() >= 8 && gWeather == SUNNY && (gSeason && gSeason < 3) && (5 < gHour && gHour < 12)) {
                setLifeEventBit(STU_MOONDROP_FLOWER_VISIT);
                gCutsceneIndex = 114;
                loadCutscene(morningVisit);
                set = TRUE;    
            }
        }        
        
        if (!set) {
            if (!checkLifeEventBit(STU_PINK_CAT_MINT_VISIT) && getFarmPinkCatMintFlowersCount() && gWeather == SUNNY && (gSeason && gSeason < AUTUMN) && (5 < gHour && gHour < 12)) {
                setLifeEventBit(STU_PINK_CAT_MINT_VISIT);
                gCutsceneIndex = 115;
                loadCutscene(morningVisit);
                set = TRUE;    
            }
        }        

        if (!set) {
            if (!checkLifeEventBit(STU_MARBLE_TRADE_VISIT) && getFarmPinkCatMintFlowersCount() >= 16 && npcAffection[STU] >= 80 && gWeather == SUNNY && (gSeason && gSeason < AUTUMN) && (5 < gHour && gHour < 12)) {
                setLifeEventBit(STU_MARBLE_TRADE_VISIT);
                gCutsceneIndex = 116;
                loadCutscene(morningVisit);
                set = TRUE;    
            }
        }      

        if (!set) {
            if (!checkLifeEventBit(MARIA_FARM_GIFT_1) && npcAffection[MARIA] >= 50 && (14 < gHour && gHour < 17) && (!checkLifeEventBit(MARRIED) || gWife != MARIA)) {
                setLifeEventBit(MARIA_FARM_GIFT_1);
                gCutsceneIndex = MARIA_FARM_GIFT_CHOCOLATE;
                loadCutscene(morningVisit);
                set = TRUE;    
            }
        }               
        
        if (!set) {
            if (!checkLifeEventBit(MARIA_FARM_GIFT_2) && npcAffection[MARIA] >= 80 && (14 < gHour && gHour < 17) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
                setLifeEventBit(MARIA_FARM_GIFT_2);
                gCutsceneIndex = MARIA_FARM_GIFT_RICE_BALLS;
                loadCutscene(morningVisit);
                set = TRUE;                               
            }
        }     

        if (!set) {
            if (!checkLifeEventBit(MARIA_FIREWORKS) && npcAffection[MARIA] >= 200 && gWeather == SUNNY && gSeason == SUMMER && NIGHTTIME && !checkDailyEventBit(FESTIVAL_DAY_CUTSCENE_GUARD) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
                setLifeEventBit(MARIA_FIREWORKS);
                gCutsceneIndex = 147;
                loadCutscene(morningVisit);
                set = TRUE;                               
            }
        }     
        
        if (!set) {
            if (!checkLifeEventBit(POPURI_FARM_GIFT_1) && npcAffection[POPURI] >= 50 && (14 < gHour && gHour < 17) && (!checkLifeEventBit(MARRIED) || gWife != POPURI)) {
                setLifeEventBit(POPURI_FARM_GIFT_1);
                gCutsceneIndex = POPURI_FARM_GIFT_TEA;
                loadCutscene(morningVisit);
                set = TRUE;                               
            }
        }          
        
        if (!set) {
            if (!checkLifeEventBit(POPURI_FARM_GIFT_2) && npcAffection[POPURI] >= 80 && (14 < gHour && gHour < 17) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
                setLifeEventBit(POPURI_FARM_GIFT_2);
                gCutsceneIndex = POPURI_FARM_GIFT_COOKIES;
                loadCutscene(morningVisit);
                set = TRUE;                               
            }
        }          
        if (!set) {
            
            if (!checkLifeEventBit(POPURI_BLUE_MIST) && blueMistFlowerPlot >= 194 && npcAffection[POPURI] >= 200 && gWeather == SUNNY && gSeason == SUMMER && gDayOfMonth == 30 && (5 < gHour && gHour < 18) 
                    && ((checkLifeEventBit(MARRIED) && gWife == POPURI) || (!checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)))) {
                
                blueMistFlowerPlot = 0;
                pondFieldTiles[5][3] = 0;
                setLifeEventBit(POPURI_BLUE_MIST);
                gCutsceneIndex = 158;
                loadCutscene(morningVisit);
                set = TRUE;           
                
            }
        } 

        if (!set) {
            if (!checkLifeEventBit(ELLI_FARM_GIFT_1) && npcAffection[ELLI] >= 50 && (14 < gHour && gHour < 17) && (!checkLifeEventBit(MARRIED) || gWife != ELLI)) {
                setLifeEventBit(ELLI_FARM_GIFT_1);
                gCutsceneIndex = 165;
                loadCutscene(morningVisit);
                set = TRUE;                      
            }
        }
        
        if (!set) {
            if (!checkLifeEventBit(ELLI_FARM_GIFT_2) && npcAffection[ELLI] >= 80 && (14 < gHour && gHour < 17) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {
                setLifeEventBit(ELLI_FARM_GIFT_2);
                gCutsceneIndex = 166;
                loadCutscene(morningVisit);
                set = TRUE;                      
            }
        }
        
        if (!set) {
            if (!checkLifeEventBit(ELLI_MOON_VIEWING) && npcAffection[ELLI] >= 200 && gWeather == SUNNY && gSeason == AUTUMN && gDayOfMonth == 9 && NIGHTTIME && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {
                setLifeEventBit(ELLI_MOON_VIEWING);
                gCutsceneIndex = 169;
                loadCutscene(morningVisit);
                set = TRUE;                      
            }
        }      

        if (!set) {
            if (!checkLifeEventBit(ELLI_THANKSGIVING_EGG_VISIT) && getChickenEggCount() >= 6 && gSeason == WINTER && gDayOfMonth == 9 && (5 < gHour && gHour < 24) && (!checkLifeEventBit(MARRIED) || gWife != ELLI)) {
                setLifeEventBit(ELLI_THANKSGIVING_EGG_VISIT);
                gCutsceneIndex = 170;
                loadCutscene(morningVisit);
                set = TRUE;                      
            }
        }  

        if (!set) {
            if (!checkLifeEventBit(ANN_FARM_GIFT_1) && npcAffection[ANN] >= 50 && (14 < gHour && gHour < 17) && (!checkLifeEventBit(MARRIED) || gWife != ANN)) {
                setLifeEventBit(ANN_FARM_GIFT_1);
                gCutsceneIndex = 177;
                loadCutscene(morningVisit);
                set = TRUE;                      
            }
        }  

        if (!set) {
            if (!checkLifeEventBit(ANN_FARM_GIFT_2) && npcAffection[ANN] >= 80 && (14 < gHour && gHour < 17) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
                setLifeEventBit(ANN_FARM_GIFT_2);
                gCutsceneIndex = 178;
                loadCutscene(morningVisit);
                set = TRUE;                      
            }
        }  
        
        if (!set) {
            if (!checkLifeEventBit(ANN_FIREWORKS) && npcAffection[ANN] >= 200 && gWeather == SUNNY && gSeason == WINTER && (5 < gHour && gHour < 18) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
                setLifeEventBit(ANN_FIREWORKS);
                gCutsceneIndex = 181;
                loadCutscene(morningVisit);
                set = TRUE;                      
            }
        }  

        if (!set) {
            if (!checkLifeEventBit(ANN_FARM_VISIT_HORSE_GROWN) && npcAffection[ANN] >= 50 && (5 < gHour && gHour < 12) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && checkLifeEventBit(HORSE_GROWN)) {
                setLifeEventBit(ANN_FARM_VISIT_HORSE_GROWN);
                gCutsceneIndex = 188;
                loadCutscene(morningVisit);
                set = TRUE;                      
            }
        }  

        if (!set) {
            if (!checkLifeEventBit(ANN_HORSE_RACE_ELIGIBILITY_VISIT) && npcAffection[ANN] >= 80 && (5 < gHour && gHour < 12) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && checkLifeEventBit(HORSE_GROWN)) {
                setLifeEventBit(ANN_HORSE_RACE_ELIGIBILITY_VISIT);
                gCutsceneIndex = 189;
                loadCutscene(morningVisit);
                set = TRUE;                      
            }
        }  

        if (!set) {
            if (!checkLifeEventBit(KAREN_FARM_GIFT_1) && npcAffection[KAREN] >= 60 && (1 < gSeason && gSeason < 4) && (14 < gHour && gHour < 17) && (!checkLifeEventBit(MARRIED) || gWife != KAREN)) {
                setLifeEventBit(KAREN_FARM_GIFT_1);
                gCutsceneIndex = 190;
                loadCutscene(morningVisit);
                set = TRUE;                      
            }
        }  
        
        if (!set) {
            if (!checkLifeEventBit(KAREN_FARM_GIFT_2) && npcAffection[KAREN] >= 100 && (14 < gHour && gHour < 17) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
                setLifeEventBit(KAREN_FARM_GIFT_2);
                gCutsceneIndex = 191;
                loadCutscene(morningVisit);
                set = TRUE;                      
            }
        }  
        
        if (!set) {
            if (!checkLifeEventBit(KAREN_KEIFU_FAIRIES_INVITE) && checkLifeEventBit(HARVEST_GODDESS_VINEYARD_BLESSING_DREAM) && npcAffection[KAREN] >= 200 && gWeather == SUNNY && gSeason == AUTUMN && gDayOfMonth == 7 && NIGHTTIME && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
                setLifeEventBit(KAREN_KEIFU_FAIRIES_INVITE);
                gCutsceneIndex = 193;
                loadCutscene(morningVisit);
                set = TRUE;                      
            }
        }  
        
        if (!set) {
            
            if (!checkLifeEventBit(FIRST_VINEYARD_WINE_GIFT) && checkLifeEventBit(HARVEST_GODDESS_VINEYARD_BLESSING_DREAM) && gSeason == WINTER && (6 < gDayOfMonth && gDayOfMonth < 31) && (5 < gHour && gHour < 18)) {
                
                setLifeEventBit(FIRST_VINEYARD_WINE_GIFT);
                setLifeEventBit(VINEYARD_FIRST_WINE);

                if (npcAffection[KAREN] >= npcAffection[KAI]) {
                    
                    if (!checkLifeEventBit(KAREN_GONE)) {
                        gCutsceneIndex = 194;
                    } else {
                        gCutsceneIndex = 215;
                    }

                } else {

                    if (checkLifeEventBit(KAI_GONE)) {
                        gCutsceneIndex = 194;
                    } else {
                        gCutsceneIndex = 215;
                    }      
                }

                loadCutscene(morningVisit);
                set = TRUE;      

            }

        }          

        if (!set) {
            if (!checkLifeEventBit(HARRIS_FARM_GIFT_MUSHROOMS) && npcAffection[HARRIS] >= 50 && gSeason == AUTUMN && (14 < gHour && gHour < 17)) {
                setLifeEventBit(HARRIS_FARM_GIFT_MUSHROOMS);
                gCutsceneIndex = 204;
                loadCutscene(morningVisit);
                set = TRUE;                      
            }
        }       
        
        if (!set) {
            if (!checkLifeEventBit(CLIFF_FARM_GIFT_FISH) && npcAffection[CLIFF] >= 50 && (14 < gHour && gHour < 17)) {
                setLifeEventBit(CLIFF_FARM_GIFT_FISH);
                gCutsceneIndex = 210;
                loadCutscene(morningVisit);
                set = TRUE;                      
            }
        }       
        
        if (!set) {
            if (!checkLifeEventBit(KAI_CELEBRATION_WINE_VISIT) && checkLifeEventBit(KAREN_KAI_HAVE_BABY) && (5 < gHour && gHour < 18) && (!checkLifeEventBit(MARRIED) || gWife != KAREN) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
                setLifeEventBit(KAI_CELEBRATION_WINE_VISIT);
                gCutsceneIndex = 218;
                loadCutscene(morningVisit);
                set = TRUE;                      
            }
        }    

        if (!set && morningVisit == FALSE) {
            if (!checkLifeEventBit(RICK_TRANSPORTER_VISIT) && checkLifeEventBit(GIVE_RICK_RARE_METAL) && npcAffection[RICK] >= 120 && gWeather == SUNNY && (gSeason && gSeason < 4) && (5 < gHour && gHour < 18) && gSpawnPointIndex == FARM_SPAWN_POINT_1) {
                setLifeEventBit(RICK_TRANSPORTER_VISIT);
                gCutsceneIndex = 220;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        }
        
        if (!set) {
            if (!checkLifeEventBit(SHADY_SALESMAN_VISIT_CHICKEN_FEED) && (5 < gHour && gHour < 12) && getTotalChickenCount() >= 5 && chickenFeedQuantity < 990) {
                setLifeEventBit(SHADY_SALESMAN_VISIT_CHICKEN_FEED);
                gCutsceneIndex = 224;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        }

        if (!set) {
            if (!checkLifeEventBit(SHADY_SALESMAN_NECTAR_SEEDS_VISIT) && (gSeason && gSeason < 4) && (5 < gHour && gHour < 12) && gTotalCropsShipped >= 900 && chickenFeedQuantity < 990) {
                setLifeEventBit(SHADY_SALESMAN_NECTAR_SEEDS_VISIT);
                gCutsceneIndex = 225;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        }
        
        if (!set && morningVisit == FALSE) {
            if (!checkLifeEventBit(TOURIST_COUPLE_FLOWER_VISIT) && (getFarmMoondropFlowerCount() + getFarmPinkCatMintFlowersCount()) >= 24 && gWeather == SUNNY && gSeason == SPRING && (5 < gHour && gHour < 18) && gSpawnPointIndex == FARM_SPAWN_POINT_1) {
                setLifeEventBit(TOURIST_COUPLE_FLOWER_VISIT);
                gCutsceneIndex = 228;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        }

        if (!set && morningVisit == FALSE) {
            if (!checkLifeEventBit(TOURIST_COUPLE_GRASS_FIELDS_VISIT) && getFarmGrassTilesSum() >= 90 && gWeather == SUNNY && gSeason == AUTUMN && (5 < gHour && gHour < 18) && gSpawnPointIndex == FARM_SPAWN_POINT_1) {
                setLifeEventBit(TOURIST_COUPLE_GRASS_FIELDS_VISIT);
                gCutsceneIndex = 230;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        }
        
        if (!set && morningVisit == FALSE) {
            if (!checkLifeEventBit(TOURIST_COUPLE_VINEYARD_VISIT) && checkLifeEventBit(VINEYARD_FIRST_WINE) && gWeather == SUNNY && gSeason == AUTUMN && (5 < gHour && gHour < 18)) {
                setLifeEventBit(TOURIST_COUPLE_VINEYARD_VISIT);
                gCutsceneIndex = 231;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        }
        
        if (!set && morningVisit == FALSE) {
            if (!checkLifeEventBit(OLD_COUPLE_HORSE_RACE_WINNER_VISIT) && checkLifeEventBit(WON_HORSE_RACE) && gWeather == SUNNY && gSeason == SPRING && (5 < gHour && gHour < 18) && gSpawnPointIndex == FARM_SPAWN_POINT_1) {
                setLifeEventBit(OLD_COUPLE_HORSE_RACE_WINNER_VISIT);
                gCutsceneIndex = 232;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        }
        
        if (!set && morningVisit == FALSE) {
            if (!checkLifeEventBit(OLD_COUPLE_WINTER_FARM_MORNING_VISIT) && checkLifeEventBit(COW_FESTIVAL_FIRST_PRIZE) && gWeather == SUNNY && getTotalCowsCount() >= 3 && gSeason == WINTER && (5 < gHour && gHour < 18) && gSpawnPointIndex == FARM_SPAWN_POINT_1) {
                setLifeEventBit(OLD_COUPLE_WINTER_FARM_MORNING_VISIT);
                gCutsceneIndex = 233;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        }
        
        if (!set && morningVisit == FALSE) {
            // get cow + grass count
            if (!checkLifeEventBit(FESTIVAL_GIRLS_COW_FESTIVAL_WIN_VISIT) && checkLifeEventBit(COW_FESTIVAL_FIRST_PRIZE) && func_8009B828(1) && gWeather == SUNNY && gSeason == AUTUMN && (5 < gHour && gHour < 18) && gSpawnPointIndex == FARM_SPAWN_POINT_1) {
                setLifeEventBit(FESTIVAL_GIRLS_COW_FESTIVAL_WIN_VISIT);
                gCutsceneIndex = 234;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        }

        if (!set && morningVisit == FALSE) {
            if (!checkLifeEventBit(FESTIVAL_GIRLS_FLOWER_FIELD_VISIT) && (getFarmMoondropFlowerCount() + getFarmPinkCatMintFlowersCount()) >= 24 && gWeather == SUNNY && gSeason == SUMMER && (5 < gHour && gHour < 18) && gSpawnPointIndex == FARM_SPAWN_POINT_1) {
                setLifeEventBit(FESTIVAL_GIRLS_FLOWER_FIELD_VISIT);
                gCutsceneIndex = 236;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        }        

        if (!set) {
            if (checkLifeEventBit(HORSE_RACE_WON_THIS_YEAR) && npcAffection[DOUG] >= 60 && (16 < gHour && gHour < 21) && ((gSeason == SPRING && gDayOfMonth == 18) || gSeason == AUTUMN && gDayOfMonth == 29)) {
                clearLifeEventBit(HORSE_RACE_WON_THIS_YEAR);
                gCutsceneIndex = 129;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        }       
        
        if (!set) {
            if (checkLifeEventBit(COW_FESTIVAL_WON_THIS_YEAR) && npcAffection[DOUG] >= 60 && gSeason == AUTUMN && gDayOfMonth == 5 && (16 < gHour && gHour < 21)) {
                clearLifeEventBit(COW_FESTIVAL_WON_THIS_YEAR);
                gCutsceneIndex = 130;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        }          

        if (!set) {
            if (checkLifeEventBit(WON_VEGETABLE_FESTIVAL) && npcAffection[BASIL] >= 60 && gSeason == SUMMER && gDayOfMonth == 10 && (16 < gHour && gHour < 21)) {
                clearLifeEventBit(WON_VEGETABLE_FESTIVAL);
                gCutsceneIndex = 139;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        }       
        
        if (!set) {
            if (!checkLifeEventBit(KAI_LEAVING) && checkLifeEventBit(KAREN_GONE) && npcAffection[KAI] <= 80 && gSeason == WINTER && (5 < gHour && gHour < 18) && gHarvestKing != 5 && gHarvestCoinFinder != 5 && (!checkLifeEventBit(MARRIED) || gWife != KAREN) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
                setLifeEventBit(KAI_LEAVING);
                setLifeEventBit(KAI_GONE);
                setSpecialDialogueBit(KAI_GONE_DIALOGUE);
                gCutsceneIndex = 219;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        }      
        
        if (!set) {
            if (!checkDailyEventBit(SPIRIT_FESTIVAL_INVITATION_VISIT) && npcAffection[POTION_SHOP_DEALER] >= 50 && gWeather == SUNNY && gYear == 1 && gSeason == WINTER && gDayOfMonth == 27 && (5 < gHour && gHour < 9)) {
                setDailyEventBit(SPIRIT_FESTIVAL_INVITATION_VISIT);
                gCutsceneIndex = 201;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        } 

      if (!set) {
            if (!checkDailyEventBit(SPIRIT_FESTIVAL_INVITATION_VISIT) && npcAffection[HARRIS] >= 100 && gHarvestKing == 1 && gWeather == SUNNY && gSeason == WINTER && gDayOfMonth == 27 && (5 < gHour && gHour < 9)) {
                setDailyEventBit(SPIRIT_FESTIVAL_INVITATION_VISIT);
                gCutsceneIndex = 202;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        }    
        
      if (!set) {
            if (!checkDailyEventBit(SPIRIT_FESTIVAL_INVITATION_VISIT) && npcAffection[GRAY] >= 100 && gHarvestKing == 2 && gWeather == SUNNY && gSeason == WINTER && gDayOfMonth == 27 && (5 < gHour && gHour < 9)) {
                setDailyEventBit(SPIRIT_FESTIVAL_INVITATION_VISIT);
                gCutsceneIndex = 205;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        }    

      if (!set) {
            if (!checkDailyEventBit(SPIRIT_FESTIVAL_INVITATION_VISIT) && npcAffection[JEFF] >= 100 && gHarvestKing == 3 && gWeather == SUNNY && gSeason == WINTER && gDayOfMonth == 27 && (5 < gHour && gHour < 9)) {
                setDailyEventBit(SPIRIT_FESTIVAL_INVITATION_VISIT);
                gCutsceneIndex = 207;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        }    
        
    if (!set) {
        if (!checkDailyEventBit(SPIRIT_FESTIVAL_INVITATION_VISIT) && npcAffection[CLIFF] >= 100 && gHarvestKing == 4 && gWeather == SUNNY && gSeason == WINTER && gDayOfMonth == 27 && (5 < gHour && gHour < 9)) {
            setDailyEventBit(SPIRIT_FESTIVAL_INVITATION_VISIT);
            gCutsceneIndex = 211;
            loadCutscene(morningVisit);
            set = TRUE;                             
        }
    }        
        
       if (!set) {
            if (!checkDailyEventBit(SPIRIT_FESTIVAL_INVITATION_VISIT) && npcAffection[KAI] >= 100 && gHarvestKing == 5 && gWeather == SUNNY && gSeason == WINTER && gDayOfMonth == 27 && (5 < gHour && gHour < 9)) {
                setDailyEventBit(SPIRIT_FESTIVAL_INVITATION_VISIT);
                gCutsceneIndex = 216;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        }      
            
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(MARIA_CUTSCENE_DAILY) && gSeason == SUMMER && gDayOfMonth == 1 && gWeather == SUNNY && (17 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == MARIA) {
                setDailyEventBit(MARIA_CUTSCENE_DAILY);
                gCutsceneIndex = 239;
                loadCutscene(morningVisit);
                gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
                set = TRUE;                             
            }
        }
            
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(POPURI_CUTSCENE_DAILY) && gSeason == SUMMER && gDayOfMonth == 1 && gWeather == SUNNY && (17 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == POPURI) {
                setDailyEventBit(POPURI_CUTSCENE_DAILY);
                gCutsceneIndex = 240;
                loadCutscene(morningVisit);
                gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
                set = TRUE;                             
            }
        }
            
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(ELLI_CUTSCENE_DAILY) && gSeason == SUMMER && gDayOfMonth == 1 && gWeather == SUNNY && (17 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == ELLI) {
                setDailyEventBit(ELLI_CUTSCENE_DAILY);
                gCutsceneIndex = 241;
                loadCutscene(morningVisit);
                gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
                set = TRUE;                             
            }
        }
            
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(ANN_CUTSCENE_DAILY) && gSeason == SUMMER && gDayOfMonth == 1 && gWeather == SUNNY && (17 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == ANN) {
                setDailyEventBit(ANN_CUTSCENE_DAILY);
                gCutsceneIndex = 242;
                loadCutscene(morningVisit);
                gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
                set = TRUE;                             
            }
        }
            
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(KAREN_CUTSCENE_DAILY) && gSeason == SUMMER && gDayOfMonth == 1 && gWeather == SUNNY && (17 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == KAREN) {
                setDailyEventBit(KAREN_CUTSCENE_DAILY);
                gCutsceneIndex = 243;
                loadCutscene(morningVisit);
                gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
                set = TRUE;                             
            }
        }
    
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(MARIA_CUTSCENE_DAILY) && gSeason == WINTER && gDayOfMonth == 24 && gWeather == SUNNY && (17 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == MARIA) {
                setDailyEventBit(MARIA_CUTSCENE_DAILY);
                gCutsceneIndex = 244;
                loadCutscene(morningVisit);
                gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
                set = TRUE;                             
            }
        }
    
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(POPURI_CUTSCENE_DAILY) && gSeason == WINTER && gDayOfMonth == 24 && gWeather == SUNNY && (17 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == POPURI) {
                setDailyEventBit(POPURI_CUTSCENE_DAILY);
                gCutsceneIndex = 245;
                loadCutscene(morningVisit);
                gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
                set = TRUE;                             
            }
        }
    
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(ELLI_CUTSCENE_DAILY) && gSeason == WINTER && gDayOfMonth == 24 && gWeather == SUNNY && (17 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == ELLI) {
                setDailyEventBit(ELLI_CUTSCENE_DAILY);
                gCutsceneIndex = 246;
                loadCutscene(morningVisit);
                gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
                set = TRUE;                             
            }
        }
    
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(ANN_CUTSCENE_DAILY) && gSeason == WINTER && gDayOfMonth == 24 && gWeather == SUNNY && (17 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == ANN) {
                setDailyEventBit(ANN_CUTSCENE_DAILY);
                gCutsceneIndex = 247;
                loadCutscene(morningVisit);
                gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
                set = TRUE;                             
            }
        }
    
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(KAREN_CUTSCENE_DAILY) && gSeason == WINTER && gDayOfMonth == 24 && gWeather == SUNNY && (17 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == KAREN) {
                setDailyEventBit(KAREN_CUTSCENE_DAILY);
                gCutsceneIndex = 248;
                loadCutscene(morningVisit);
                gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
                set = TRUE;                             
            }
        }
    
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(MARIA_CUTSCENE_DAILY) && gSeason == SUMMER && gDayOfMonth == 7 && gWeather == SUNNY && (16 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == MARIA && npcAffection[MARIA] >= 220) {
                setDailyEventBit(MARIA_CUTSCENE_DAILY);
                gCutsceneIndex = 150;
                loadCutscene(morningVisit);
                gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
                set = TRUE;                             
            }
        }
            
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(POPURI_CUTSCENE_DAILY) && gSeason == SUMMER && gDayOfMonth == 7 && gWeather == SUNNY && (16 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == POPURI && npcAffection[POPURI] >= 220) {
                setDailyEventBit(POPURI_CUTSCENE_DAILY);
                gCutsceneIndex = 161;
                loadCutscene(morningVisit);
                gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
                set = TRUE;                             
            }
        }
    
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(ELLI_CUTSCENE_DAILY) && gSeason == SUMMER && gDayOfMonth == 7 && gWeather == SUNNY && (16 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == ELLI && npcAffection[ELLI] >= 220) {
                setDailyEventBit(ELLI_CUTSCENE_DAILY);
                gCutsceneIndex = 173;
                loadCutscene(morningVisit);
                gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
                set = TRUE;                             
            }
        }
    
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(ANN_CUTSCENE_DAILY) && gSeason == SUMMER && gDayOfMonth == 7 && gWeather == SUNNY && (16 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == ANN && npcAffection[ANN] >= 220) {
                setDailyEventBit(ANN_CUTSCENE_DAILY);
                gCutsceneIndex = 184;
                loadCutscene(morningVisit);
                gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
                set = TRUE;                             
            }
        }
    
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(KAREN_CUTSCENE_DAILY) && gSeason == SUMMER && gDayOfMonth == 7 && gWeather == SUNNY && (16 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == KAREN && npcAffection[KAREN] >= 220) {
                setDailyEventBit(KAREN_CUTSCENE_DAILY);
                gCutsceneIndex = 197;
                loadCutscene(morningVisit);
                gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
                set = TRUE;                             
            }
        }
            
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(MARIA_CUTSCENE_DAILY) && gSeason == AUTUMN && gDayOfMonth == 9 && gWeather == SUNNY && (16 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == MARIA && npcAffection[MARIA] >= 220) {
                setDailyEventBit(MARIA_CUTSCENE_DAILY);
                gCutsceneIndex = 151;
                loadCutscene(morningVisit);
                gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
                set = TRUE;                             
            }
        }       
    
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(POPURI_CUTSCENE_DAILY) && gSeason == AUTUMN && gDayOfMonth == 9 && gWeather == SUNNY && (16 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == POPURI && npcAffection[POPURI] >= 220) {
                setDailyEventBit(POPURI_CUTSCENE_DAILY);
                gCutsceneIndex = 162;
                loadCutscene(morningVisit);
                gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
                set = TRUE;                             
            }
        }      
    
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(ELLI_CUTSCENE_DAILY) && gSeason == AUTUMN && gDayOfMonth == 9 && gWeather == SUNNY && (16 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == ELLI && npcAffection[ELLI] >= 220) {
                setDailyEventBit(ELLI_CUTSCENE_DAILY);
                gCutsceneIndex = 174;
                loadCutscene(morningVisit);
                gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
                set = TRUE;                             
            }
        }        
            
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(ANN_CUTSCENE_DAILY) && gSeason == AUTUMN && gDayOfMonth == 9 && gWeather == SUNNY && (16 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == ANN && npcAffection[ANN] >= 220) {
                setDailyEventBit(ANN_CUTSCENE_DAILY);
                gCutsceneIndex = 185;
                loadCutscene(morningVisit);
                gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
                set = TRUE;                             
            }
        }     
            
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(KAREN_CUTSCENE_DAILY) && gSeason == AUTUMN && gDayOfMonth == 9 && gWeather == SUNNY && (16 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == KAREN && npcAffection[KAREN] >= 220) {
                setDailyEventBit(KAREN_CUTSCENE_DAILY);
                gCutsceneIndex = 198;
                loadCutscene(morningVisit);
                gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
                set = TRUE;                             
            }
        }       

    }

    return result;
}


//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setFarmCutscenes);

u16 setFarmCutscenes(void) {

    u16 bytecodeSegmentIndex = 0xFFFF;
    bool set = FALSE;
    u8 tempHappiness;

    // shipper on farm during birth
    if (checkDailyEventBit(BIRTH_EVENT_DAILY)) { 
        gCutsceneIndex = 257;
        gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
        loadCutscene(FALSE);
        set = TRUE;
    }
    
    if (!set) {
        if (checkDailyEventBit(PURCHASED_ANIMAL)) {
            bytecodeSegmentIndex = 1;
            set = TRUE;
            gCutsceneIndex = 0;
            gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
        }
    }
    
    if (!set) {

        if (checkDailyEventBit(ANIMAL_SALE_IN_PROGRESS)) {
            
            if (checkDailyEventBit(ANIMAL_SALE_CUTSCENE_DAILY)) {
                gCutsceneIndex = 1;
            } else {
                gCutsceneIndex = 0;
                setDailyEventBit(ANIMAL_SALE_CUTSCENE_DAILY);
            }
            
            gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
            bytecodeSegmentIndex = 2;
            set = TRUE;
        }

    }

    if (!set) {

        if (checkDailyEventBit(ANIMAL_SOLD)) {

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

            bytecodeSegmentIndex = 3;
            set = TRUE;
            gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
        
        }

    }

    // enter cow festival
    if (!set) {
        if (checkDailyEventBit(COW_FESTIVAL_ELIGIBLE) && !checkDailyEventBit(COW_FESTIVAL_ENTRY_RESOLVED)) {
            bytecodeSegmentIndex = 33;
            set = TRUE;
            gCutsceneIndex = 1;
            gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
        }
    }

    // cow festival take cow
    if (!set) {
        if (checkDailyEventBit(COW_FESTIVAL_COW_TAKEN)) {
            bytecodeSegmentIndex = 34;
            set = TRUE;
            gCutsceneIndex = 1;
            gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
        }
    }

    // house construction estimate
    if (!set) {
        if (checkDailyEventBit(HOUSE_EXTENSION_SELECTED)) {
            gCutsceneIndex = handlePurchaseHouseExtension(gHouseExtensionSelection);
            gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
            bytecodeSegmentIndex = 4;
            set = TRUE;
        }
    }

    // house construction
    if (!set) {
        if (checkLifeEventBit(HOUSE_EXTENSION_CONSTRUCTION) && checkDailyEventBit(HOUSE_EXTENSION_WORK) && !checkDailyEventBit(END_OF_DAILY_HOUSE_EXTENSION_WORK)) {
            gCutsceneIndex = 1;
            bytecodeSegmentIndex = 5;
            set = TRUE;
        }
    }

    // house construction completed
    if (!set) {
        if (checkDailyEventBit(CARPENTER_FINISHED)) {
            gCutsceneIndex = 0;
            bytecodeSegmentIndex = 6;
            set = TRUE;
        }
    }

    if (!set) {
        setFarmVisitCutscenes(0);
    }
    
    return bytecodeSegmentIndex;

}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setMountain1Cutscenes);

u16 setMountain1Cutscenes(void) {

    bool set = FALSE;
    u16 bytecodeSegmentIndex = 0xFFFF;

    if (!checkLifeEventBit(BRIDGE_COMPLETED) && gSeason == AUTUMN && (22 < gDayOfMonth && gDayOfMonth < 28) && (5 < gHour && gHour < 15)) {
        setDailyEventBit(MOUNTAIN_1_CUTSCENE_DAILY);
        if (checkDailyEventBit(MOUNTAIN_CARPENTER_HELP)) {
            gCutsceneIndex = 325;
        } else {
            gCutsceneIndex = 324;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        }
        loadCutscene(FALSE);
        set = TRUE;
    }

    if (!set) {
        if (!checkLifeEventBit(HOT_SPRINGS_COMPLETED) && gSeason == WINTER && (11 < gDayOfMonth && gDayOfMonth < 17) && (5 < gHour && gHour < 15)) {
            setDailyEventBit(MOUNTAIN_1_CUTSCENE_DAILY);
            if (checkDailyEventBit(MOUNTAIN_CARPENTER_HELP)) {
                gCutsceneIndex = 325;
            } else {
                gCutsceneIndex = 328;
                gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            }
            loadCutscene(FALSE);
            set = TRUE;  
        }
    }

    if (!set) {
        if (!checkHaveTool(FISHING_POLE)) {
            setDailyEventBit(MOUNTAIN_1_CUTSCENE_DAILY);
            gCutsceneIndex = 318;
            loadCutscene(FALSE);
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkLifeEventBit(ELLI_JEFF_FISHING_CUTSCENE) && npcAffection[JEFF] >= 180 && gWeather == SUNNY && gSeason == SUMMER && (5 < gHour && gHour < 18) && (!checkLifeEventBit(MARRIED) || gWife != ELLI) && !checkLifeEventBit(ELLI_JEFF_MARRIED) && gSpawnPointIndex == TOP_OF_MOUNTAIN_1_SUMMER) {
            setDailyEventBit(MOUNTAIN_1_CUTSCENE_DAILY);
            setLifeEventBit(ELLI_JEFF_FISHING_CUTSCENE);
            gCutsceneIndex = 308;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkLifeEventBit(MARIA_ELLI_FISHING_CUTSCENE) && npcAffection[MARIA] >= 150 && npcAffection[ELLI] >= 150 && gWeather == SUNNY && gSeason == SUMMER && (5 < gHour && gHour < 18) && gSpawnPointIndex == TOP_OF_MOUNTAIN_1_SUMMER) {
            setDailyEventBit(MOUNTAIN_1_CUTSCENE_DAILY);
            setLifeEventBit(MARIA_ELLI_FISHING_CUTSCENE);
            gCutsceneIndex = 310;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkLifeEventBit(POPURI_KAREN_MOUNTAIN_CUTSCENE) && npcAffection[POPURI] >= 150 && npcAffection[KAREN] >= 150 && gWeather == SUNNY && gSeason == SPRING && (5 < gHour && gHour < 18) && gSpawnPointIndex == 0x1A) {
            setDailyEventBit(MOUNTAIN_1_CUTSCENE_DAILY);
            setLifeEventBit(POPURI_KAREN_MOUNTAIN_CUTSCENE);
            gCutsceneIndex = 314;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
            set = TRUE;
        }
    }

    if (!set) {
      if (!checkLifeEventBit(ELLI_SPRAINED_ANKLE_CUTSCENE) && !checkDailyEventBit(FESTIVAL_DAY_CUTSCENE_GUARD) && npcAffection[ELLI] >= 180 && gWeather == SUNNY && (5 < gHour && gHour < 18) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {
            setDailyEventBit(MOUNTAIN_1_CUTSCENE_DAILY);
            setLifeEventBit(ELLI_SPRAINED_ANKLE_CUTSCENE);
            setDailyEventBit(ELLI_LOCATION_OVERRIDE);
            gCutsceneIndex = ELLI_SPRAINED_ANKLE;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
        }
    } 
    
    return bytecodeSegmentIndex;

}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setMountain2Cutscenes);

u16 setMountain2Cutscenes(void) {

    bool set = FALSE;
    u16 bytecodeSegmentIndex = 0xFFFF;

    if (!checkLifeEventBit(POPURI_MOUNTAIN_HELP) && checkLifeEventBit(POPURI_DREAM_CUTSCENE) && checkLifeEventBit(BRIDGE_COMPLETED) && gWeather == SUNNY && 5 < gHour && gHour < 18 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
        setLifeEventBit(POPURI_MOUNTAIN_HELP);
        gCutsceneIndex = 301;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
        set = TRUE;
    }

    if (!set && !checkLifeEventBit(POPURI_SPRAINED_ANKLE) && !checkDailyEventBit(FESTIVAL_DAY_CUTSCENE_GUARD) && npcAffection[POPURI] >= 180 && gWeather == SUNNY && 5 < gHour && gHour < 18 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
        setLifeEventBit(POPURI_SPRAINED_ANKLE);
        setDailyEventBit(POPURI_LOCATION_OVERRIDE);
        gCutsceneIndex = 302;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
        set = TRUE;
    }
    
    if (!set && !checkLifeEventBit(ANN_BABY_CHICK_MOUNTAIN) && checkLifeEventBit(ANN_DREAM_CUTSCENE) && gWeather == SUNNY && gSeason != WINTER && 5 < gHour && gHour < 18 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && gSpawnPointIndex == 0x1D) { 
        setLifeEventBit(ANN_BABY_CHICK_MOUNTAIN);
        gCutsceneIndex = 306;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
        set = TRUE;
    }

    if (!set && !checkLifeEventBit(ANN_GRAY_MOUNTAIN_CUTSCENE) && npcAffection[ANN] >= 80 && gWeather == SUNNY && gSeason == AUTUMN && 5 < gHour && gHour < 18) {
        setLifeEventBit(ANN_GRAY_MOUNTAIN_CUTSCENE);
        gCutsceneIndex = 307;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
        set = TRUE;
    }

    if (!set && !checkLifeEventBit(CLIFF_ANN_KANE_MOUNTAIN_CUTSCENE) && npcAffection[CLIFF] >= 150 && gWeather == SUNNY && gSeason != WINTER && 5 < gHour && gHour < 18 && (!checkLifeEventBit(MARRIED) || gWife != ANN) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && gSpawnPointIndex == 0x1D) {
        setLifeEventBit(CLIFF_ANN_KANE_MOUNTAIN_CUTSCENE);
        gCutsceneIndex = 309;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
        set = TRUE;
    }

    if (!set && !checkLifeEventBit(MARIA_KAREN_GRASSHOPPER_CUTSCENE) && npcAffection[MARIA] >= 150 && npcAffection[KAREN] >= 150 && gWeather == SUNNY && gSeason == SPRING && 5 < gHour && gHour < 18 && gSpawnPointIndex == 0x1D) {
        setLifeEventBit(MARIA_KAREN_GRASSHOPPER_CUTSCENE);
        gCutsceneIndex = 311;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
        set = TRUE;
    }
    
    if (!set && !checkLifeEventBit(STU_MAY_BUTTERFLY_CUTSCENE) && npcAffection[STU] >= 50 && npcAffection[MAY] >= 50 && gWeather == SUNNY && gSeason < AUTUMN && gDayOfWeek == SATURDAY && 5 < gHour && gHour < 18) {
        setLifeEventBit(STU_MAY_BUTTERFLY_CUTSCENE);
        gCutsceneIndex = 338;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
        set = TRUE;
    }


    if (!set && !checkLifeEventBit(POPURI_CONFESSION) && !checkLifeEventBit(MARRIED) && npcAffection[POPURI] >= 250 && gWeather == SUNNY && 11 < gHour && gHour < 18 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
        setLifeEventBit(POPURI_CONFESSION);
        gCutsceneIndex = 342;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
        set = TRUE;
    }

    return bytecodeSegmentIndex;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setTopOfMountain1Cutscenes);

u16 setTopOfMountain1Cutscenes(void) {

    bool set = FALSE;
    u16 bytecodeSegmentIndex = 0xFFFF;

    if (!checkLifeEventBit(POPURI_ELLI_HOT_SPRINGS_CHAT) && checkLifeEventBit(HOT_SPRINGS_COMPLETED) && npcAffection[POPURI] >= 150 && npcAffection[JEFF] >= 120 && gWeather == SUNNY && gSeason >= AUTUMN && (5 < gHour && gHour < 18) && (!checkLifeEventBit(MARRIED) || gWife != POPURI) && (!checkLifeEventBit(MARRIED) || gWife != ELLI) && !checkLifeEventBit(POPURI_GRAY_MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {
        setLifeEventBit(POPURI_ELLI_HOT_SPRINGS_CHAT);
        gCutsceneIndex = 312;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
        set = TRUE;
    }

    if (!set) {
        if (!checkLifeEventBit(TOP_OF_MOUNTAIN_AUTUMN_HOT_SPRINGS_CUTSCENE) && checkLifeEventBit(HOT_SPRINGS_COMPLETED) && gWeather == SUNNY && gSeason == AUTUMN && (5 < gHour && gHour < 18)) {
            setLifeEventBit(TOP_OF_MOUNTAIN_AUTUMN_HOT_SPRINGS_CUTSCENE);
            gCutsceneIndex = 339;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
            set = TRUE;
        }
    }

    if (!set) {
        // anonymous characters in hot springs, but logic implies it's the carpenters relaxing after getting rich 
        if (!checkLifeEventBit(TOP_OF_MOUNTAIN_WINTER_HOT_SPRINGS_CUTSCENE) && gWeather == SUNNY && gSeason == WINTER && (5 < gHour && gHour < 18) && checkLifeEventBit(HAVE_KITCHEN) && checkLifeEventBit(HAVE_BATHROOM) && checkLifeEventBit(HAVE_STAIRS) && checkLifeEventBit(HAVE_GREENHOUSE) && checkLifeEventBit(HAVE_LOG_TERRACE) && checkLifeEventBit(HAVE_BABY_BED) && checkLifeEventBit(HOT_SPRINGS_COMPLETED)) {
            setLifeEventBit(TOP_OF_MOUNTAIN_WINTER_HOT_SPRINGS_CUTSCENE);
            gCutsceneIndex = 340;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
        }
    }
    
    return bytecodeSegmentIndex;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setMoonMountainCutscenes);

u16 setMoonMountainCutscenes(void) {

    bool set = FALSE;
    u16 bytecodeSegmentIndex = 0xFFFF;

    if (gSeason == SUMMER && gDayOfMonth == 1 && NIGHTTIME && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
        gCutsceneIndex = MOUNTAIN_FIREWORKS;
        loadCutscene(FALSE);
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        set = TRUE;
    }
    
    if (!set) {
        if (gSeason == WINTER && gDayOfMonth == 24 && NIGHTTIME && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
            gCutsceneIndex = STARRY_NIGHT_MOON_MOUNTAIN;
            loadCutscene(FALSE);
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        }
    }
    
    return bytecodeSegmentIndex;
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setPondCutscenes);

u16 setPondCutscenes(void) {

    bool set = FALSE;
    u16 bytecodeSegmentIndex = 0xFFFF;

    if (!checkLifeEventBit(BLUE_MIST_COMPLETE) && blueMistFlowerPlot >= 0xC2 && gSeason < AUTUMN && (5 < gHour && gHour < 18)) {
        setLifeEventBit(BLUE_MIST_COMPLETE);
        setDailyEventBit(HARVEST_GODDESS_INTERACTION);
        gCutsceneIndex = 315;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
        set = TRUE;
    }

    if (!set) {
        if (!checkLifeEventBit(ENTOMOLOGIST_BLUE_MIST_CUTSCENE) && checkLifeEventBit(BLUE_MIST_COMPLETE) && blueMistFlowerPlot >= 0xC2 && gSeason < AUTUMN && (5 < gHour && gHour < 18)) {
            setLifeEventBit(ENTOMOLOGIST_BLUE_MIST_CUTSCENE);
            setDailyEventBit(HARVEST_GODDESS_INTERACTION);
            gCutsceneIndex = 316;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
            set = TRUE;
        }
    }

    if (!set) {
         if (!checkLifeEventBit(ENTOMOLOGIST_EMERALD_BUTTERFLY_CUTSCENE) && checkLifeEventBit(ENTOMOLOGIST_BLUE_MIST_CUTSCENE) && blueMistFlowerPlot >= 0xC2 && gSeason < AUTUMN && (5 < gHour && gHour < 18)) {
            setLifeEventBit(ENTOMOLOGIST_EMERALD_BUTTERFLY_CUTSCENE);
            setDailyEventBit(HARVEST_GODDESS_INTERACTION);
            gCutsceneIndex = 317;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
         }
    }
    
    return bytecodeSegmentIndex;
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setHarvestSpriteCaveCutscenes);

u16 setHarvestSpriteCaveCutscenes(void) {

    u16 bytecodeSegmentIndex = 0xFFFF;
    
    if (checkHaveKeyItem(MARBLES) && npcAffection[HARVEST_SPRITE_1] >= 80 && (5 < gHour && gHour < 18)) {
        gCutsceneIndex = 322;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
    }

    return bytecodeSegmentIndex;

}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setCaveCutscenes);

u16 setCaveCutscenes(void) {

    u16 bytecodeSegmentIndex = 0xFFFF;
    if (gSeason == WINTER && (7 < gDayOfMonth && gDayOfMonth < 31) && (5 < gHour && gHour < 16)) {
        gCutsceneIndex = 346;
        gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
        loadCutscene(FALSE);
    }

    return bytecodeSegmentIndex;

}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setHouseCutscenes);

u16 setHouseCutscenes(void) {

    u16 bytecodeSegmentIndex = 0xFFFF;
    bool set = FALSE;

    if (checkLifeEventBit(MARRIED) && !checkLifeEventBit(HAVE_BABY) && checkLifeEventBit(WIFE_PREGNANT) && gWifePregnancyCounter >= 60) {

        setDailyEventBit(BIRTH_EVENT_DAILY);
        gWeather = SUNNY;

        babyBirthdaySeason = gSeason;
        babyBirthdayDate = gDayOfMonth;

        switch (gWife) {
            
            case MARIA:
                gCutsceneIndex = MARIA_BIRTH;
                clearSpecialDialogueBit(WIFE_PREGNANT_MARIA_DIALOGUE);
                break;
            case POPURI:
                gCutsceneIndex = POPURI_BIRTH;
                clearSpecialDialogueBit(WIFE_PREGNANT_POPURI_DIALOGUE);
                break;
            case ELLI:
                gCutsceneIndex = ELLI_BIRTH;
                clearSpecialDialogueBit(WIFE_PREGNANT_ELLI_DIALOGUE);
                break;
            case ANN:
                gCutsceneIndex = ANN_BIRTH;
                clearSpecialDialogueBit(WIFE_PREGNANT_ANN_DIALOGUE);
                break;
            case KAREN:
                gCutsceneIndex = KAREN_BIRTH;
                clearSpecialDialogueBit(WIFE_PREGNANT_KAREN_DIALOGUE);
                break;
            
        }
        
        loadCutscene(FALSE);
        set = TRUE;
        gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
        
    }

    if (!set) {
        if (checkLifeEventBit(WENT_TO_BED_EARLY) && !checkLifeEventBit(KENT_HOUSE_VISIT_CUTSCENE) && !checkLifeEventBit(MARRIED) && npcAffection[KENT] >= 160 && !checkLifeEventBit(MARRIED)) {
            gCutsceneIndex = 1;
            loadCutscene(FALSE);
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkLifeEventBit(CLIFF_HOUSE_VISIT_CUTSCENE) && npcAffection[CLIFF] >= 30 && gWeather == SUNNY && (5 < gHour && gHour < 18) && (!checkLifeEventBit(MARRIED) || gWife != ANN) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
            gCutsceneIndex = 38;
            loadCutscene(FALSE);
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkDailyEventBit(FARM_MORNING_VISIT) && !checkLifeEventBit(WIFE_LEFT) && (5 < gHour && gHour < 18) && checkLifeEventBit(MARRIED) && gWife == MARIA && npcAffection[MARIA] < 100) {
            setDailyEventBit(FARM_MORNING_VISIT);
            setLifeEventBit(WIFE_LEFT);
            gCutsceneIndex = MARIA_LEAVES_MARRIAGE;
            loadCutscene(FALSE);
            gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkDailyEventBit(FARM_MORNING_VISIT) && !checkLifeEventBit(WIFE_LEFT) && (5 < gHour && gHour < 18) && checkLifeEventBit(MARRIED) && gWife == POPURI && npcAffection[POPURI] < 100) {
            setDailyEventBit(FARM_MORNING_VISIT);
            setLifeEventBit(WIFE_LEFT);
            gCutsceneIndex = POPURI_LEAVES_MARRIAGE;
            loadCutscene(FALSE);
            gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkDailyEventBit(FARM_MORNING_VISIT) && !checkLifeEventBit(WIFE_LEFT) && (5 < gHour && gHour < 18) && checkLifeEventBit(MARRIED) && gWife == ELLI && npcAffection[ELLI] < 100) {
            setDailyEventBit(FARM_MORNING_VISIT);
            setLifeEventBit(WIFE_LEFT);
            gCutsceneIndex = ELLI_LEAVES_MARRIAGE;
            loadCutscene(FALSE);
            gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkDailyEventBit(FARM_MORNING_VISIT) && !checkLifeEventBit(WIFE_LEFT) && (5 < gHour && gHour < 18) && checkLifeEventBit(MARRIED) && gWife == ANN && npcAffection[ANN] < 100) {
            setDailyEventBit(FARM_MORNING_VISIT);
            setLifeEventBit(WIFE_LEFT);
            gCutsceneIndex = ANN_LEAVES_MARRIAGE;
            loadCutscene(FALSE);
            gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkDailyEventBit(FARM_MORNING_VISIT) && !checkLifeEventBit(WIFE_LEFT) && (5 < gHour && gHour < 18) && checkLifeEventBit(MARRIED) && gWife == KAREN && npcAffection[KAREN] < 100) {
            setDailyEventBit(FARM_MORNING_VISIT);
            setLifeEventBit(WIFE_LEFT);
            gCutsceneIndex = KAREN_LEAVES_MARRIAGE;
            loadCutscene(FALSE);
            gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
            set = TRUE;
        }
    }
    
    return bytecodeSegmentIndex;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setKitchenCutscenes);

u16 setKitchenCutscenes(void) {

    u16 bytecodeSegmentIndex = 0xFFFF;
    bool set = FALSE;

    if (!checkDailyEventBit(MARIA_CUTSCENE_DAILY) && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && checkLifeEventBit(HAVE_BABY) && !checkDailyEventBit(BIRTH_EVENT_DAILY) && gWife == MARIA && babyBirthdaySeason == gSeason && babyBirthdayDate == gDayOfMonth) {
        setDailyEventBit(MARIA_CUTSCENE_DAILY);
        gCutsceneIndex = MARIA_BABY_BIRTHDAY;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
        
    }

    if (!set && !checkDailyEventBit(POPURI_CUTSCENE_DAILY) && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && checkLifeEventBit(HAVE_BABY) && !checkDailyEventBit(BIRTH_EVENT_DAILY) && gWife == POPURI && babyBirthdaySeason == gSeason && babyBirthdayDate == gDayOfMonth) {
        setDailyEventBit(POPURI_CUTSCENE_DAILY);
        gCutsceneIndex = POPURI_BABY_BIRTHDAY;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
    }

    if (!set && !checkDailyEventBit(ELLI_CUTSCENE_DAILY) && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && checkLifeEventBit(HAVE_BABY) && !checkDailyEventBit(BIRTH_EVENT_DAILY) && gWife == ELLI && babyBirthdaySeason == gSeason && babyBirthdayDate == gDayOfMonth) {
        setDailyEventBit(ELLI_CUTSCENE_DAILY);
        gCutsceneIndex = ELLI_BABY_BIRTHDAY;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
    }

    if (!set && !checkDailyEventBit(ANN_CUTSCENE_DAILY) && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && checkLifeEventBit(HAVE_BABY) && !checkDailyEventBit(BIRTH_EVENT_DAILY) && gWife == ANN && babyBirthdaySeason == gSeason && babyBirthdayDate == gDayOfMonth) {
        setDailyEventBit(ANN_CUTSCENE_DAILY);
        gCutsceneIndex = ANN_BABY_BIRTHDAY;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
    }

    if (!set && !checkDailyEventBit(KAREN_CUTSCENE_DAILY) && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && checkLifeEventBit(HAVE_BABY) && !checkDailyEventBit(BIRTH_EVENT_DAILY) && gWife == KAREN && babyBirthdaySeason == gSeason && babyBirthdayDate == gDayOfMonth) {
        setDailyEventBit(KAREN_CUTSCENE_DAILY);
        gCutsceneIndex = KAREN_BABY_BIRTHDAY;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
    }

    if (!set && !checkDailyEventBit(MARIA_CUTSCENE_DAILY) && gPlayerBirthdaySeason == gSeason && gDayOfMonth == 25 && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && gWife == MARIA && npcAffection[MARIA] >= 100) {
        setDailyEventBit(MARIA_CUTSCENE_DAILY);
        gCutsceneIndex = MARIA_MARRIED_PLAYER_BIRTHDAY;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
    }

    if (!set && !checkDailyEventBit(POPURI_CUTSCENE_DAILY) && gPlayerBirthdaySeason == gSeason && gDayOfMonth == 25 && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && gWife == POPURI && npcAffection[POPURI] >= 100) {
        setDailyEventBit(POPURI_CUTSCENE_DAILY);
        gCutsceneIndex = POPURI_MARRIED_PLAYER_BIRTHDAY;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
    }

    if (!set && !checkDailyEventBit(ELLI_CUTSCENE_DAILY) && gPlayerBirthdaySeason == gSeason && gDayOfMonth == 25 && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && gWife == ELLI && npcAffection[ELLI] >= 100) {
        setDailyEventBit(ELLI_CUTSCENE_DAILY);
        gCutsceneIndex = ELLI_MARRIED_PLAYER_BIRTHDAY;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
    }

    if (!set && !checkDailyEventBit(ANN_CUTSCENE_DAILY) && gPlayerBirthdaySeason == gSeason && gDayOfMonth == 25 && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && gWife == ANN && npcAffection[ANN] >= 100) {
        setDailyEventBit(ANN_CUTSCENE_DAILY);
        gCutsceneIndex = ANN_MARRIED_PLAYER_BIRTHDAY;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
    }

    if (!set && !checkDailyEventBit(KAREN_CUTSCENE_DAILY) && gPlayerBirthdaySeason == gSeason && gDayOfMonth == 25 && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && gWife == KAREN && npcAffection[KAREN] >= 100) {
        setDailyEventBit(KAREN_CUTSCENE_DAILY);
        gCutsceneIndex = KAREN_MARRIED_PLAYER_BIRTHDAY;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
    }

    if (!set && !checkDailyEventBit(MARIA_CUTSCENE_DAILY) && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && gWife == MARIA && anniversarySeason == gSeason && anniversaryDate == gDayOfMonth) {
        setDailyEventBit(MARIA_CUTSCENE_DAILY);
        gCutsceneIndex = MARIA_ANNIVERSARY;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
    }

    if (!set && !checkDailyEventBit(POPURI_CUTSCENE_DAILY) && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && gWife == POPURI && anniversarySeason == gSeason && anniversaryDate == gDayOfMonth) {
        setDailyEventBit(POPURI_CUTSCENE_DAILY);
        gCutsceneIndex = POPURI_ANNIVERSARY;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
    }

    if (!set && !checkDailyEventBit(ELLI_CUTSCENE_DAILY) && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && gWife == ELLI && anniversarySeason == gSeason && anniversaryDate == gDayOfMonth) {
        setDailyEventBit(ELLI_CUTSCENE_DAILY);
        gCutsceneIndex = ELLI_ANNIVERSARY;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
    }

    if (!set && !checkDailyEventBit(ANN_CUTSCENE_DAILY) && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && gWife == ANN && anniversarySeason == gSeason && anniversaryDate == gDayOfMonth) {
        setDailyEventBit(ANN_CUTSCENE_DAILY);
        gCutsceneIndex = ANN_ANNIVERSARY;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
    }

    if (!set && !checkDailyEventBit(KAREN_CUTSCENE_DAILY) && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && gWife == KAREN && anniversarySeason == gSeason && anniversaryDate == gDayOfMonth) {
        setDailyEventBit(KAREN_CUTSCENE_DAILY);
        gCutsceneIndex = KAREN_ANNIVERSARY;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
    }
    
    if (!set && !checkDailyEventBit(MARIA_CUTSCENE_DAILY) && gSeason == WINTER && gDayOfMonth == 10 && (16 < gHour && gHour < 20) && checkLifeEventBit(MARRIED) && gWife == MARIA) {
        setDailyEventBit(MARIA_CUTSCENE_DAILY);
        gCutsceneIndex = MARIA_MARRIED_THANKSGIVING;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
    }
        
    if (!set && !checkDailyEventBit(POPURI_CUTSCENE_DAILY) && gSeason == WINTER && gDayOfMonth == 10 && (16 < gHour && gHour < 20) && checkLifeEventBit(MARRIED) && gWife == POPURI) {
        setDailyEventBit(POPURI_CUTSCENE_DAILY);
        gCutsceneIndex = POPURI_MARRIED_THANKSGIVING;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
    }

    if (!set && !checkDailyEventBit(ELLI_CUTSCENE_DAILY) && gSeason == WINTER && gDayOfMonth == 10 && (16 < gHour && gHour < 20) && checkLifeEventBit(MARRIED) && gWife == ELLI) {
        setDailyEventBit(ELLI_CUTSCENE_DAILY);
        gCutsceneIndex = ELLI_MARRIED_THANKSGIVING;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
    }

    if (!set && !checkDailyEventBit(ANN_CUTSCENE_DAILY) && gSeason == WINTER && gDayOfMonth == 10 && (16 < gHour && gHour < 20) && checkLifeEventBit(MARRIED) && gWife == ANN) {
        setDailyEventBit(ANN_CUTSCENE_DAILY);
        gCutsceneIndex = ANN_MARRIED_THANKSGIVING;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
    }

    if (!set && !checkDailyEventBit(KAREN_CUTSCENE_DAILY) && gSeason == WINTER && gDayOfMonth == 10 && (16 < gHour && gHour < 20) && checkLifeEventBit(MARRIED) && gWife == KAREN) {
        setDailyEventBit(KAREN_CUTSCENE_DAILY);
        gCutsceneIndex = KAREN_MARRIED_THANKSGIVING;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
    }
    
    return bytecodeSegmentIndex;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setRanchCutscenes);

u16 setRanchCutscenes(void) {

    u16 bytecodeSegmentIndex = 0xFFFF;
    bool set = FALSE;

    if (gYear == 1 && gSeason == SPRING && gDayOfMonth >= 3 && gWeather == SUNNY && 5 < gHour && gHour < 17 && !checkLifeEventBit(HAVE_HORSE)) {

        if (!checkLifeEventBit(RANCH_GET_HORSE_CUTSCENE)) {
            gCutsceneIndex = 650;
        } else {
            gCutsceneIndex = 665;
        }

        loadCutscene(FALSE);
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        set = TRUE;
        
    }

    if (!set && gSeason == SUMMER && gDayOfMonth == 1 && 17 < gHour && gHour < 24 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {

        gCutsceneIndex = 659;
        loadCutscene(FALSE);
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        set = TRUE;
    
    }

    if (!set && gSeason == AUTUMN && gDayOfMonth == 4 && 5 < gHour && gHour < 18) {

        if (gCowFestivalEnteredCowIndex != 0xFF) {
            setDailyEventBit(COW_FESTIVAL_PLAYER_ENTERED);
            // set cutscene wild card/score variable
            D_801886D2 = gFarmAnimals[gCowFestivalEnteredCowIndex].affection;
        } else {
            clearDailyEventBit(COW_FESTIVAL_PLAYER_ENTERED);
        }
        
        gCutsceneIndex = 1150;
        loadCutscene(FALSE);
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        set = TRUE;
    
    }

    if (!set && !checkDailyEventBit(ANN_CUTSCENE_DAILY) && gSeason == WINTER && gDayOfMonth == 10 && 5 < gHour && gHour < 18 && !checkLifeEventBit(MARRIED)) {
        setDailyEventBit(ANN_CUTSCENE_DAILY);
        gCutsceneIndex = 662;
        loadCutscene(FALSE);
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        set = TRUE;

    }
    
    if (!set && !checkLifeEventBit(POPURI_ANN_RANCH_FLOWERS) && npcAffection[GRAY] >= 150 && gWeather == SUNNY && gSeason < AUTUMN && 5 < gHour && gHour < 17 && (!checkLifeEventBit(MARRIED) || gWife != POPURI) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {

        if (gSpawnPointIndex == 0x29) {
            gCutsceneIndex = 652;
            setLifeEventBit(POPURI_ANN_RANCH_FLOWERS);
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
            set = TRUE;
        }
        
        if (gSpawnPointIndex == 0x2A) {
            gCutsceneIndex = 653;
            setLifeEventBit(POPURI_ANN_RANCH_FLOWERS);
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
            set = TRUE;
        }
        
    }

    if (!set && !checkLifeEventBit(POPURI_GRAY_RANCH_EVENING) && npcAffection[GRAY] >= 180 && gWeather == SUNNY && gSeason < WINTER && 17 < gHour && gHour < 24 && (!checkLifeEventBit(MARRIED) || gWife != POPURI) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {

        if (gSpawnPointIndex == 0x29) {
            gCutsceneIndex = 654;
            setLifeEventBit(POPURI_GRAY_RANCH_EVENING);
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
            set = TRUE;
        }
        
        if (gSpawnPointIndex == 0x2A) {
            gCutsceneIndex = 655;
            setLifeEventBit(POPURI_GRAY_RANCH_EVENING);
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
            set = TRUE;
        }
        
    }

    if (!set && !checkLifeEventBit(ANN_POPURI_TALK_ABOUT_GRAY) && npcAffection[POPURI] >= 150 && npcAffection[ANN] >= 150 && gWeather == SUNNY && gSeason == AUTUMN && 5 < gHour && gHour < 18 && (!checkLifeEventBit(MARRIED) || gWife != POPURI) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {

        if (gSpawnPointIndex == 0x29) {
            gCutsceneIndex = 657;
            setLifeEventBit(ANN_POPURI_TALK_ABOUT_GRAY);
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
            set = TRUE;
        }
        
        if (gSpawnPointIndex == 0x2A) {
            gCutsceneIndex = 658;
            setLifeEventBit(ANN_POPURI_TALK_ABOUT_GRAY);
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
            set = TRUE;
        }
        
    }
    
    if (!set && !checkLifeEventBit(ANN_CLIFF_RANCH_CUTSCENE) && npcAffection[CLIFF] >= 100 && gWeather == SUNNY && gSeason < WINTER && 5 < gHour && gHour < 18 && (!checkLifeEventBit(MARRIED) || gWife != ANN) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {

        setLifeEventBit(ANN_CLIFF_RANCH_CUTSCENE);
        gCutsceneIndex = 660;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
        set = TRUE;

    }

    if (!set && !checkLifeEventBit(ANN_CONFESSION) && !checkLifeEventBit(MARRIED) && npcAffection[ANN] >= 250 && gWeather == SUNNY && 14 < gHour && gHour < 24 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && gSpawnPointIndex == 0x29) {

        setLifeEventBit(ANN_CONFESSION);
        gCutsceneIndex = 661;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
        set = TRUE;

    }

    return bytecodeSegmentIndex;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setRanchStoreCutscenes);

u16 setRanchStoreCutscenes(void) {

    u16 bytecodeSegmentIndex = 0xFFFF;
     
    if (!(checkLifeEventBit(ANN_SICK_VISIT)) && !checkDailyEventBit(FESTIVAL_DAY_CUTSCENE_GUARD) && npcAffection[ANN] >= 120 && (2 <= gWeather && gWeather < 4) && gDayOfWeek == SUNDAY && (5 < gHour && gHour < 18) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
        setLifeEventBit(ANN_SICK_VISIT);
        setDailyEventBit(ANN_LOCATION_OVERRIDE);
        gCutsceneIndex = ANN_SICK_VISIT_CUTSCENE;
        loadCutscene(FALSE);
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
    }
     
    if (checkLifeEventBit(MARRIED) && gWife == ANN && checkLifeEventBit(WIFE_LEFT)) {
        gCutsceneIndex = ANN_REPAIR_MARRIAGE;
        loadCutscene(FALSE);
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
    }

    return bytecodeSegmentIndex;

}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setVineyardCutscenes);

u16 setVineyardCutscenes(void) {

    u16 bytecodeSegmentIndex = 0xFFFF;
    bool set = FALSE;

    if (!checkLifeEventBit(KAREN_VINEYARD_INTRO) && (5 < gHour && gHour < 18) && (!checkLifeEventBit(MARRIED) || gWife != 4) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
        setLifeEventBit(KAREN_VINEYARD_INTRO);
        gCutsceneIndex = 700;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
        set = TRUE;
    }

    if (!set) {
        if (!checkLifeEventBit(KAREN_SPRAINED_ANKLE) && !checkDailyEventBit(FESTIVAL_DAY_CUTSCENE_GUARD) && npcAffection[KAREN] >= 180 && gWeather == SUNNY && (5 < gHour && gHour < 18) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            setLifeEventBit(KAREN_SPRAINED_ANKLE);
            setDailyEventBit(KAREN_LOCATION_OVERRIDE);
            gCutsceneIndex = 701;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkLifeEventBit(KAREN_ANN_TALK_ABOUT_CLIFF) && npcAffection[ANN] >= 120 && npcAffection[KAREN] >= 150 && gWeather == SUNNY && gSeason == WINTER && (5 < gHour && gHour < 18) && (!checkLifeEventBit(MARRIED) || gWife != KAREN) && !checkLifeEventBit(KAREN_KAI_MARRIED) && previousMapIndex == ROAD) {
            setLifeEventBit(KAREN_ANN_TALK_ABOUT_CLIFF);
            gCutsceneIndex = 702;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkLifeEventBit(KAI_KAREN_CUTSCENE) && npcAffection[KAI] >= 50 && npcAffection[KAREN] >= 50 && (5 < gHour && gHour < 18) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED) && previousMapIndex == ROAD) {
            setLifeEventBit(KAI_KAREN_CUTSCENE);
            gCutsceneIndex = 703;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
        }
    }

    return bytecodeSegmentIndex;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setVineyardHouseCutscenes);

u16 setVineyardHouseCutscenes(void) {
    
    u16 bytecodeSegmentIndex = 0xFFFF;
    
    if (!checkLifeEventBit(KAREN_SICK_VISIT) && !checkDailyEventBit(FESTIVAL_DAY_CUTSCENE_GUARD) && npcAffection[KAREN] >= 120U && (2 <= gWeather && gWeather < 4) && (5 < gHour && gHour < 18) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
        setLifeEventBit(KAREN_SICK_VISIT);
        setDailyEventBit(KAREN_LOCATION_OVERRIDE);
        gCutsceneIndex = KAREN_SICK;
        loadCutscene(FALSE);
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
    }
    
    if (checkLifeEventBit(MARRIED) && gWife == KAREN && checkLifeEventBit(WIFE_LEFT)) {
        gCutsceneIndex = 712;
        loadCutscene(FALSE);
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
    }
    
    return bytecodeSegmentIndex;
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setVineyardCellarCutscenes);

u16 setVineyardCellarCutscenes(void) {
    
    bool set;
    u16 bytecodeSegmentIndex = 0xFFFF;

    set = FALSE;
    
    if (!checkLifeEventBit(KAREN_YELLOW_HEART_EVENT_CUTSCENE) && checkLifeEventBit(KAREN_DREAM_CUTSCENE) && (5 < gHour && gHour < 18) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED) && previousMapIndex == VINEYARD) {
               
        setLifeEventBit(KAREN_YELLOW_HEART_EVENT_CUTSCENE);
        setDailyEventBit(KAREN_YELLOW_HEART_EVENT_DAILY);

        gCutsceneIndex = KAREN_YELLOW_HEART_EVENT_1;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);

        loadCutscene(FALSE);

        set = TRUE;

    }
    
    if (!set && checkDailyEventBit(KAREN_YELLOW_HEART_EVENT_DAILY) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED) && previousMapIndex == VINEYARD_CELLAR_BASEMENT) {
        gCutsceneIndex = KAREN_YELLOW_HEART_EVENT_4;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
    }
    
    return bytecodeSegmentIndex;
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setVillage1Cutscenes);

u16 setVillage1Cutscenes(void) {

    u16 bytecodeSegmentIndex = 0xFFFF;
    bool set = FALSE;

    if (!checkLifeEventBit(BLUE_MIST_CYCLE_ACTIVE) && checkLifeEventBit(BOUGHT_BLUE_MIST_SEEDS) && checkHaveTool(BLUE_MIST_SEEDS)) {
        setLifeEventBit(BLUE_MIST_CYCLE_ACTIVE);
        gCutsceneIndex = 410;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
        set = TRUE;
    } 

    if (!set && gSeason == SUMMER && gDayOfMonth == 9 && 5 < gHour && gHour < 18) {
        gCutsceneIndex = 1000;
        loadCutscene(FALSE);
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        set = TRUE;
    } 
    
    if (!set && gSeason == AUTUMN && gDayOfMonth == 20 && 5 < gHour && gHour < 18) {
        gCutsceneIndex = 1250;
        loadCutscene(FALSE);
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        set = TRUE;
    } 

    if (!set && !checkDailyEventBit(POPURI_CUTSCENE_DAILY) && gSeason == WINTER && gDayOfMonth == 10 && 5 < gHour && gHour < 18 && !checkLifeEventBit(MARRIED)) {
        setDailyEventBit(POPURI_CUTSCENE_DAILY);
        gCutsceneIndex = 424;
        loadCutscene(FALSE);
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        set = TRUE;
    } 


    if (!set && !checkLifeEventBit(MARIA_POPURI_VILLAGE_CUTSCENE) && npcAffection[MARIA] >= 150 && npcAffection[POPURI] >= 150 && gWeather == SUNNY && gSeason == SPRING && 5 < gHour && gHour < 18) {

        if (previousMapIndex == ROAD) {
            
            gCutsceneIndex = 412;
            setLifeEventBit(MARIA_POPURI_VILLAGE_CUTSCENE);
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
            set = TRUE;
            
        }
        
        if (previousMapIndex == VILLAGE_2) {

            if (gSpawnPointIndex == 0x33) {
                gCutsceneIndex = 413;
            } else {
                gCutsceneIndex = 414;
            }

            setLifeEventBit(MARIA_POPURI_VILLAGE_CUTSCENE);
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
            set = TRUE;
            
        }
        
    } 
    
    if (!set && !checkLifeEventBit(GRAY_VILLAGE1_CUTSCENE) && npcAffection[GRAY] >= 120 && gWeather == SUNNY && gSeason < AUTUMN && 5 < gHour && gHour < 18 && (!checkLifeEventBit(MARRIED) || gWife != POPURI) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
        setLifeEventBit(GRAY_VILLAGE1_CUTSCENE);
        gCutsceneIndex = 415;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
        set = TRUE;
    } 

    if (!set && !checkLifeEventBit(ELLEN_DIED) && npcAffection[ELLEN] >= 30 && gWeather == SUNNY && gYear == 2 && gSeason == SPRING && 5 < gHour && gHour < 18) {
        gCutsceneIndex = 416;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
        set = TRUE;
    } 
    
    return bytecodeSegmentIndex;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setVillage2Cutscenes);

// village 2
u16 setVillage2Cutscenes(void) {

    u16 bytecodeSegmentIndex = 0xFFFF;
    bool set = FALSE;

    if (!checkLifeEventBit(MAYOR_SHADY_SALESMAN_CUTSCENE) && (npcAffection[MAYOR] + npcAffection[MAYOR_WIFE] >= 200) && npcAffection[MARIA] >= 150 && (5 < gHour && gHour < 18)) {

        if (previousMapIndex == ROAD) {

            setLifeEventBit(MAYOR_SHADY_SALESMAN_CUTSCENE);
            gCutsceneIndex = 505;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
            set = TRUE;
            
        }

        if (previousMapIndex == VILLAGE_1) {

            if (gSpawnPointIndex == 0x3F) {
                gCutsceneIndex = 506;
            } else {
                gCutsceneIndex = 507;
            }

            setLifeEventBit(MAYOR_SHADY_SALESMAN_CUTSCENE);
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
            set = TRUE;
            
        }
        
    }

    if (!set && !checkLifeEventBit(MARIA_HARRIS_CUTSCENE) && npcAffection[HARRIS] >= 160 && gWeather == SUNNY && 5 < gHour && gHour < 18 && (!checkLifeEventBit(MARRIED) || gWife != MARIA) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {

        if (previousMapIndex == ROAD) {

            setLifeEventBit(MARIA_HARRIS_CUTSCENE);
            gCutsceneIndex = 508;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
            set = TRUE;
            
        }

        if (previousMapIndex == VILLAGE_1) {

            if (gSpawnPointIndex == 0x3F) {
                gCutsceneIndex = 509;
            } else {
                gCutsceneIndex = 510;
            }

            setLifeEventBit(MARIA_HARRIS_CUTSCENE);
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
            set = TRUE;
            
        }
    
    }

    if (!set && !checkLifeEventBit(SHADY_SALESMAN_MARIA_VILLAGE_2_CUTSCENE) && npcAffection[HARRIS] >= 180 && gSeason < AUTUMN && gWeather == SUNNY && 5 < gHour && gHour < 18 && (!checkLifeEventBit(MARRIED) || gWife != MARIA) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {

        if (previousMapIndex == ROAD) {

            setLifeEventBit(SHADY_SALESMAN_MARIA_VILLAGE_2_CUTSCENE);
            gCutsceneIndex = 511;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
            set = TRUE;
            
        }

        if (previousMapIndex == VILLAGE_1) {

            if (gSpawnPointIndex == 0x3F) {
                gCutsceneIndex = 512;
            } else {
                gCutsceneIndex = 513;
            }

            setLifeEventBit(SHADY_SALESMAN_MARIA_VILLAGE_2_CUTSCENE);
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
            set = TRUE;
            
        }
    
    }

    if (!set && !checkLifeEventBit(MARIA_ANN_VILLAGE_CUTSCENE) && npcAffection[MARIA] >= 150 && npcAffection[ANN] >= 150 && gSeason == AUTUMN && 5 < gHour && gHour < 18) {

        if (previousMapIndex == ROAD) {

            setLifeEventBit(MARIA_ANN_VILLAGE_CUTSCENE);
            gCutsceneIndex = 514;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
            set = TRUE;
            
        }

        if (previousMapIndex == VILLAGE_1) {

            if (gSpawnPointIndex == 0x3F) {
                gCutsceneIndex = 515;
            } else {
                gCutsceneIndex = 516;
            }

            setLifeEventBit(MARIA_ANN_VILLAGE_CUTSCENE);
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
            set = TRUE;
            
        }
    
    }

    if (!set && !checkLifeEventBit(ELLI_PLAYING_HOUSE_CUTSCENE) && checkLifeEventBit(ELLI_DREAM_CUTSCENE) && gWeather == SUNNY && 5 < gHour && gHour < 17 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {

        if (previousMapIndex == ROAD) {

            setLifeEventBit(ELLI_PLAYING_HOUSE_CUTSCENE);
            gCutsceneIndex = 518;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
            set = TRUE;
            
        }

        if (previousMapIndex == VILLAGE_1) {

            if (gSpawnPointIndex == 0x3F) {
                gCutsceneIndex = 519;
            } else {
                gCutsceneIndex = 520;
            }

            setLifeEventBit(ELLI_PLAYING_HOUSE_CUTSCENE);
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
            set = TRUE;
            
        }
    
    }

    if (!set && !checkLifeEventBit(MARIA_PREGNANT_MIDWIFE_CUTSCENE) && checkLifeEventBit(WIFE_PREGNANT) && gWife == MARIA && npcAffection[MIDWIFE] >= 80 && gWifePregnancyCounter == 9 && gWeather == SUNNY && 5 < gHour && gHour < 18) {

        setLifeEventBit(MARIA_PREGNANT_MIDWIFE_CUTSCENE);
        gCutsceneIndex = 500;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
        set = TRUE;
        
    }

    if (!set && !checkLifeEventBit(POPURI_PREGNANCY_MIDWIFE_CUTSCENE) && checkLifeEventBit(WIFE_PREGNANT) && gWife == POPURI && npcAffection[MIDWIFE] >= 80 && gWifePregnancyCounter == 9 && gWeather == SUNNY && 5 < gHour && gHour < 18) {

        setLifeEventBit(POPURI_PREGNANCY_MIDWIFE_CUTSCENE);
        gCutsceneIndex = 501;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
        set = TRUE;
        
    }

    if (!set && !checkLifeEventBit(ELLI_PREGNANCY_MIDWIFE_CUTSCENE) && checkLifeEventBit(WIFE_PREGNANT) && gWife == ELLI && npcAffection[MIDWIFE] >= 80 && gWifePregnancyCounter == 9 && gWeather == SUNNY && 5 < gHour && gHour < 18) {

        setLifeEventBit(ELLI_PREGNANCY_MIDWIFE_CUTSCENE);
        gCutsceneIndex = 502;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
        set = TRUE;
        
    }

    if (!set && !checkLifeEventBit(ANN_PREGNANCY_MIDWIFE_CUTSCENE) && checkLifeEventBit(WIFE_PREGNANT) && gWife == ANN && npcAffection[MIDWIFE] >= 80 && gWifePregnancyCounter == 9 && gWeather == SUNNY && 5 < gHour && gHour < 18) {

        setLifeEventBit(ANN_PREGNANCY_MIDWIFE_CUTSCENE);
        gCutsceneIndex = 503;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
        set = TRUE;
        
    }

    if (!set && !checkLifeEventBit(KAREN_PREGNANCY_MIDWIFE_CUTSCENE) && checkLifeEventBit(WIFE_PREGNANT) && gWife == KAREN && npcAffection[MIDWIFE] >= 80 && gWifePregnancyCounter == 9 && gWeather == SUNNY && 5 < gHour && gHour < 18) {

        setLifeEventBit(KAREN_PREGNANCY_MIDWIFE_CUTSCENE);
        gCutsceneIndex = 504;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
        set = TRUE;
        
    }

    if (!set && !checkLifeEventBit(MARIA_CONFESSION) && !checkLifeEventBit(MARRIED) && npcAffection[MARIA] >= 250 && gWeather == SUNNY && 11 < gHour && gHour < 18 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED) && previousMapIndex == LIBRARY) {
        setLifeEventBit(MARIA_CONFESSION);
        gCutsceneIndex = 517;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
    } 
    
    return bytecodeSegmentIndex;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setRickStoreCutscenes);

u16 setRickStoreCutscenes(void) {
    
    u16 bytecodeSegmentIndex = 0xFFFF;
    
    if (!checkLifeEventBit(ANN_RICK_CUTSCENE) && npcAffection[RICK] >= 40 && (5 < gHour && gHour < 18)) {
        setLifeEventBit(ANN_RICK_CUTSCENE);
        gCutsceneIndex = ANN_AND_RICK;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
    }
    
    return bytecodeSegmentIndex;

}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setFlowerShopCutscenes);

u16 setFlowerShopCutscenes(void) {
    
    u16 bytecodeSegmentIndex = 0xFFFF;
    
    if (!checkLifeEventBit(POPURI_SICK_CUTSCENE) && !checkDailyEventBit(FESTIVAL_DAY_CUTSCENE_GUARD) && npcAffection[POPURI] >= 120 && (2 <= gWeather && gWeather < 4) && (5 < gHour && gHour < 18) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
        setLifeEventBit(POPURI_SICK_CUTSCENE);
        setDailyEventBit(POPURI_LOCATION_OVERRIDE);
        gCutsceneIndex = POPURI_SICK;
        loadCutscene(FALSE);
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
    }
    
    if (checkLifeEventBit(MARRIED) && gWife == POPURI && checkLifeEventBit(WIFE_LEFT)) {
        gCutsceneIndex = 444;
        loadCutscene(FALSE);
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
    }
    
    return bytecodeSegmentIndex;

}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setBakeryCutscenes);

u16 setBakeryCutscenes(void) {

    bool set = FALSE;
    u16 bytecodeSegmentIndex = 0xFFFF;

    if (gSeason == SUMMER && gDayOfMonth == 1 && 17 < gHour && gHour < 24 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {

        if (!checkLifeEventBit(ELLEN_DIED)) {
            gCutsceneIndex = 418;
        } else {
            gCutsceneIndex = 420;
        }

        loadCutscene(FALSE);
        set = TRUE;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        
    }

    if (!set && !checkDailyEventBit(ELLI_CUTSCENE_DAILY) && gSeason == WINTER && gDayOfMonth == 10 && 5 < gHour && gHour < 18 && !checkLifeEventBit(MARRIED)) {
        setDailyEventBit(ELLI_CUTSCENE_DAILY);
        gCutsceneIndex = 427;
        loadCutscene(FALSE);
        set = TRUE;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
    }

    if (!set && !checkLifeEventBit(ELLI_ANN_BAKING) && npcAffection[ELLI] >= 150 && npcAffection[ANN] >= 150 && 5 < gHour && gHour < 18 && (gSeason == AUTUMN && gDayOfMonth >= 15 || gSeason == WINTER && gDayOfMonth < 10)) {

        setLifeEventBit(ELLI_ANN_BAKING);
        gCutsceneIndex = 432;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
        set = TRUE;
        
    }

    if (!set && !checkLifeEventBit(ELLI_POPURI_BAKERY) && !checkDailyEventBit(FESTIVAL_DAY_CUTSCENE_GUARD) && npcAffection[JEFF] >= 150 && 5 < gHour && gHour < 18  && (!checkLifeEventBit(MARRIED) || gWife != ELLI) && !checkLifeEventBit(ELLI_JEFF_MARRIED) && previousMapIndex == VILLAGE_1) {

        setLifeEventBit(ELLI_POPURI_BAKERY);
        
        if (gSpawnPointIndex == 0x59) {
            gCutsceneIndex = 433;
        } else {
            gCutsceneIndex = 434;
        }
        
        loadCutscene(FALSE);
        set = TRUE;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        
    }
    
    if (!set && !checkLifeEventBit(ELLI_SICK_VISIT) && !checkDailyEventBit(FESTIVAL_DAY_CUTSCENE_GUARD) && npcAffection[ELLI] >= 120 && (SUNNY < gWeather && gWeather < 4) && (5 < gHour && gHour < 18)  && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {

        setLifeEventBit(ELLI_SICK_VISIT);
        setDailyEventBit(ELLI_LOCATION_OVERRIDE);
        gCutsceneIndex = 442;
        loadCutscene(FALSE);
        set = TRUE;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        
    }

    if (!set && !checkLifeEventBit(ELLI_CONFESSION) && !checkLifeEventBit(MARRIED) && npcAffection[ELLI] >= 250 && gWeather == SUNNY && 11 < gHour && gHour < 17 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {

        setLifeEventBit(ELLI_CONFESSION);
        gCutsceneIndex = 426;
        set = TRUE;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
        
    }

    if (checkLifeEventBit(MARRIED) && gWife == ELLI && checkLifeEventBit(WIFE_LEFT)) {
        gCutsceneIndex = 445;
        loadCutscene(FALSE);
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
    }

    return bytecodeSegmentIndex;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setMayorHouseCutscenes);

u16 setMayorHouseCutscenes(void) {

    u16 bytecodeSegmentIndex = 0xFFFF;
    bool set = FALSE;

    if (!checkDailyEventBit(MARIA_CUTSCENE_DAILY) && gSeason == WINTER && gDayOfMonth == 10 && (5 < gHour && gHour < 18) && !checkLifeEventBit(MARRIED)) {
        setDailyEventBit(MARIA_CUTSCENE_DAILY);
        gCutsceneIndex = THANKSGIVING_MAYOR_HOUSE;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
    }

    if (!set && !checkLifeEventBit(MARIA_SICK_CUTSCENE) && !checkDailyEventBit(FESTIVAL_DAY_CUTSCENE_GUARD) && npcAffection[MARIA] >= 120 && (SUNNY < gWeather && gWeather < 4) && gDayOfWeek == MONDAY && (8 < gHour && gHour < 17) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
        setLifeEventBit(MARIA_SICK_CUTSCENE);
        setDailyEventBit(MARIA_LOCATION_OVERRIDE);
        gCutsceneIndex = MARIA_SICK;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
    }

    if (checkLifeEventBit(MARRIED) && gWife == MARIA && checkLifeEventBit(WIFE_LEFT)) {
        gCutsceneIndex = MARIA_REPAIR_MARRIAGE;
        loadCutscene(FALSE);
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
    }

    return bytecodeSegmentIndex;

}    

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setLibraryCutscenes);

u16 setLibraryCutscenes(void) {

    u16 bytecodeSegmentIndex = 0xFFFF;
    bool set = FALSE;

    if (!checkLifeEventBit(MARIA_SPRAINED_ANKLE_CUTSCENE) && !checkDailyEventBit(FESTIVAL_DAY_CUTSCENE_GUARD) && npcAffection[MARIA] >= 180 && (8 < gHour && gHour < 17) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
        setLifeEventBit(MARIA_SPRAINED_ANKLE_CUTSCENE);
        setDailyEventBit(MARIA_LOCATION_OVERRIDE);
        gCutsceneIndex = MARIA_SPRAINED_ANKLE;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
    }

    if (!set) {
        if (!checkLifeEventBit(MARIA_CHILDRENS_BOOK) && checkLifeEventBit(MARIA_DREAM_CUTSCENE) && (8 < gHour && gHour < 17) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
            setLifeEventBit(MARIA_CHILDRENS_BOOK);
            gCutsceneIndex = 527;
            loadCutscene(FALSE);
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        }
    }
    
    return bytecodeSegmentIndex;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setChurchCutscenes);

u16 setChurchCutscenes(void) {

    bool set = FALSE;
    u16 bytecodeSegmentIndex = 0xFFFF;

    if (gSeason == WINTER && gDayOfMonth == 24 && NIGHTTIME && !checkLifeEventBit(MARRIED)) {     
        gCutsceneIndex = STARRY_NIGHT_FESTIVAL;
        loadCutscene(FALSE);
        set = TRUE;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
    } 
    
    if (!set) {
        if (gSeason == WINTER && gDayOfMonth == 30 && NIGHTTIME) {
            gCutsceneIndex = CHURCH_NEW_YEARS;
            loadCutscene(FALSE);
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        }    
    } 
    
    return bytecodeSegmentIndex;
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setTavernCutscenes);

u16 setTavernCutscenes(void) {

    u16 bytecodeSegmentIndex = 0xFFFF;
    bool set = FALSE;

    if (gSeason == WINTER && gDayOfMonth == 30 && NIGHTTIME) {
        gCutsceneIndex = TAVERN_NEW_YEARS;
        loadCutscene(FALSE);
        set = TRUE;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
    }

    if (!set && !checkLifeEventBit(KAREN_KAI_TAVERN_CUTSCENE) && npcAffection[KAREN] >= 80 && npcAffection[KAI] >= 80 && NIGHTTIME && (!checkLifeEventBit(MARRIED) || gWife != KAREN) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
        setLifeEventBit(KAREN_KAI_TAVERN_CUTSCENE);
        setSpecialDialogueBit(KAREN_KAI_TAVERN_CUTSCENE_DIALOGUE);
        gCutsceneIndex = KAREN_KAI_TAVERN;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
    }

    return bytecodeSegmentIndex;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setSquareCutscenes);

u16 setSquareCutscenes(void) {

    u16 bytecodeSegmentIndex = 0xFFFF;
    bool set = FALSE;

    if (gSeason == SPRING && gDayOfMonth == 8 && 5 < gHour && gHour < 18) {

        if (gYear == 1) {
            gCutsceneIndex = 850;
        } else {
            if (gHarvestKing == PLAYER) {
                gCutsceneIndex = 851;
            } else {
                gCutsceneIndex = 852;
            }
        }
        
        loadCutscene(FALSE);
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        set = TRUE;
        
    }

    if (!set && gSeason == SPRING && gDayOfMonth == 23 && 5 < gHour && gHour < 18) {

        gCutsceneIndex = 950;
        loadCutscene(FALSE);
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        set = TRUE;
        
    }

    if (!set && gSeason == SUMMER && gDayOfMonth == 1 && 17 < gHour && gHour < 24 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {

        gCutsceneIndex = 600;
        loadCutscene(FALSE);
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        set = TRUE;
        
    }

    if (!set && gSeason == AUTUMN && gDayOfMonth == 12 && 5 < gHour && gHour < 18) {

        setDailyEventBit(HARVEST_FESTIVAL_DAILY);
        setHarvestKingName(gHarvestCoinFinder);
        
        gCutsceneIndex = 1200;
        loadCutscene(FALSE);
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        set = TRUE;
        
    }

    if (!set && gSeason == WINTER && gDayOfMonth == 27 && 17 < gHour && gHour < 24) {

        if (gHarvestKing == PLAYER) {
            gCutsceneIndex = 1350;
        } else {
            if (!checkDailyEventBit(SPIRIT_FESTIVAL_INVITATION_ACCEPTED)) {
                gCutsceneIndex = 1351;
            } else {
                gCutsceneIndex = 1352;
            }
        }
    
        loadCutscene(FALSE);
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        set = TRUE;
        
    }


    if (!set && gSeason == SPRING && gDayOfMonth == 1 && 5 < gHour && gHour < 18) {
        gCutsceneIndex = 1400;
        loadCutscene(FALSE);
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        set = TRUE;
    }

    if (!set && gSeason == SPRING && gDayOfMonth == 17 && 7 < gHour && gHour < 18) {
        updateHorseRaceContext();
        gCutsceneIndex = 900;
        loadCutscene(FALSE);
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        set = TRUE;
    }
    
    if (!set && gSeason == AUTUMN && gDayOfMonth == 28 && 7 < gHour && gHour < 18) {
        updateHorseRaceContext();
        gCutsceneIndex = 900;
        loadCutscene(FALSE);
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        set = TRUE;
    }

    if (!set && gSeason == WINTER && gDayOfMonth == 19 && 7 < gHour && gHour < 18) {
        updateDogRaceContext();
        gCutsceneIndex = 1300;
        loadCutscene(FALSE);
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
    }
    
    return bytecodeSegmentIndex;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setRaceTrackCutscenes);

u16 setRaceTrackCutscenes(void) {

    u16 bytecodeSegmentIndex = 0xFFFF;
    bool set = FALSE;

    if (checkDailyEventBit(RACE_COMPLETED) && checkDailyEventBit(HORSE_RACE)) {
        updateHorseRaceContext();
        clearDailyEventBit(RACE_COMPLETED);
        gCutsceneIndex = HORSE_RACE_STARTING_ANNOUNCEMENT;
        loadCutscene(FALSE);
        set = TRUE;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
    }

    if (!set) {
        if (checkDailyEventBit(RACE_COMPLETED) && checkDailyEventBit(DOG_RACE)) {
            updateDogRaceContext();
            clearDailyEventBit(RACE_COMPLETED);
            gCutsceneIndex = DOG_RACE_AFTER_RACE;
            loadCutscene(FALSE);
            set = TRUE;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        }
    }

    if (!set) { 
        if (gSeason == SPRING && gDayOfMonth == 17 && (7 < gHour && gHour < 18)) {
            updateHorseRaceContext();
            gCutsceneIndex = HORSE_RACE_STARTING_ANNOUNCEMENT;
            loadCutscene(FALSE);
            set = TRUE;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        }
    }
    
    if (!set) { 
        if (gSeason == AUTUMN && gDayOfMonth == 28 && (7 < gHour && gHour < 18)) {
            updateHorseRaceContext();
            gCutsceneIndex = HORSE_RACE_STARTING_ANNOUNCEMENT;
            loadCutscene(FALSE);
            set = TRUE;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        }
    }

    if (!set) { 
        if (gSeason == WINTER && gDayOfMonth == 19 && (7 < gHour && gHour < 18)) {
            updateDogRaceContext();
            gCutsceneIndex = DOG_RACE_AFTER_RACE;
            loadCutscene(FALSE);
            set = TRUE;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        }
    }
    
    return bytecodeSegmentIndex;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setBeachCutscenes);

u16 setBeachCutscenes(void) {
    
    bool set = FALSE;
    u16 bytecodeSegmentIndex = 0xFFFF;

    if (gSeason == SUMMER && gDayOfMonth == 17 && NIGHTTIME) {
        gCutsceneIndex = FIREFLY_FESTIVAL;
        loadCutscene(FALSE);
        set = TRUE;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
    }

    if (!set) {
        if (gSeason == SUMMER && gDayOfMonth == 24 && (5 < gHour && gHour < 18)) {
            gCutsceneIndex = SEA_FESTIVAL;
            loadCutscene(FALSE);
            set = TRUE;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        }
    }
    
    if (!set) {
        if (gSeason == SUMMER && gDayOfMonth == 1 && NIGHTTIME && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            gCutsceneIndex = BEACH_FIREWORKS;
            loadCutscene(FALSE);
            set = TRUE;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        }
    }
    
    if (!set) {
        if (!checkDailyEventBit(KAREN_CUTSCENE_DAILY) && gSeason == WINTER && gDayOfMonth == 10 && (5 < gHour && gHour < 18) && !checkLifeEventBit(MARRIED)) {
            setDailyEventBit(KAREN_CUTSCENE_DAILY);
            gCutsceneIndex = 753;
            loadCutscene(FALSE);
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            set = TRUE;
        }
    }

    if (!set) {
        if (gSeason == WINTER && gDayOfMonth == 24 && NIGHTTIME && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            gCutsceneIndex = 756;
            loadCutscene(FALSE);
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            set = TRUE;
        }
    }
    
    if (!set) {
        if (!checkLifeEventBit(KAREN_LEAVING) && npcAffection[KAREN] < 101 && (5 < gHour && gHour < 18) && (gYear >= 3 || (gYear == 2 && gSeason == WINTER)) && (!checkLifeEventBit(MARRIED) || gWife != KAREN) && !checkLifeEventBit(KAREN_KAI_MARRIED) && gFlowerFestivalGoddess != 4) {
            setLifeEventBit(KAREN_LEAVING);
            setLifeEventBit(KAREN_GONE);
            setSpecialDialogueBit(KAREN_GONE_DIALOGUE);
            gCutsceneIndex = 752;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkLifeEventBit(KAREN_AND_ELLI_BEACH_CUTSCENE) && npcAffection[ELLI] >= 150 && npcAffection[KAREN] >= 150 && gWeather == SUNNY && gSeason == SUMMER && (5 < gHour && gHour < 18) && (!checkLifeEventBit(MARRIED) || gWife != KAREN) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            setLifeEventBit(KAREN_AND_ELLI_BEACH_CUTSCENE);
            gCutsceneIndex = KAREN_AND_ELLI;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkLifeEventBit(KAREN_CONFESSION) && !checkLifeEventBit(MARRIED) && npcAffection[KAREN] >= 250 && gWeather == SUNNY && (14 < gHour && gHour < 24) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            setLifeEventBit(KAREN_CONFESSION);
            gCutsceneIndex = KAREN_CONFESSION_CUTSCENE;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);     
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkLifeEventBit(KAREN_KAI_BEACH_CUTSCENE) && checkLifeEventBit(VINEYARD_FIRST_WINE) && npcAffection[KAI] >= 180 && gWeather == SUNNY && NIGHTTIME && (!checkLifeEventBit(MARRIED) || gWife != KAREN) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            setLifeEventBit(KAREN_KAI_BEACH_CUTSCENE);
            gCutsceneIndex = 751;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
        }
    }
    
    return bytecodeSegmentIndex;
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setRoadCutscenes);

u16 setRoadCutscenes(void) {
    
    bool set = FALSE;
    u16 bytecodeSegmentIndex = 0xFFFF;

    if (!checkLifeEventBit(ANN_CLIFF_ROAD_CUTSCENE) && npcAffection[CLIFF] >= 200 && gSeason == WINTER && (5 < gHour && gHour < 18) && (!checkLifeEventBit(MARRIED) || gWife != ANN) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && previousMapIndex == FARM) {
        setLifeEventBit(ANN_CLIFF_ROAD_CUTSCENE);
        gCutsceneIndex = 800;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
        set = TRUE;
    }

    if (!set) {
        if (!checkLifeEventBit(ANN_RETURNS_BAKERY_CARD) && bakeryCardPoints >= 6 && npcAffection[ANN] >= 150 && (5 < gHour && gHour < 18) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && previousMapIndex == RANCH) {
            setLifeEventBit(ANN_RETURNS_BAKERY_CARD);
            gCutsceneIndex = 801;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
            set = TRUE;
        }    
    }

    if (!set) {
        if (!checkLifeEventBit(SHADY_SALESMAN_MARIA_CUTSCENE) && checkLifeEventBit(MAYOR_SHADY_SALESMAN_CUTSCENE) && (5 < gHour && gHour < 18)) {

            if (previousMapIndex == VILLAGE_1) {
                setLifeEventBit(SHADY_SALESMAN_MARIA_CUTSCENE);
                gCutsceneIndex = SHADY_SALESMAN_AND_MARIA_1;
                gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
                loadCutscene(FALSE);
                set = TRUE;
            }

            if (previousMapIndex == VILLAGE_2) {
                setLifeEventBit(SHADY_SALESMAN_MARIA_CUTSCENE);
                gCutsceneIndex = SHADY_SALESMAN_AND_MARIA_2;
                gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
                loadCutscene(FALSE);
                set = TRUE;
            }
        }
    }

    if (!set) {
        if (!checkLifeEventBit(ANN_SPRAINED_ANKLE_CUTSCENE) && !checkDailyEventBit(FESTIVAL_DAY_CUTSCENE_GUARD) && npcAffection[ANN] >= 180 && (5 < gHour && gHour < 18) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && previousMapIndex == FARM) {
            setLifeEventBit(ANN_SPRAINED_ANKLE_CUTSCENE);
            setDailyEventBit(ANN_LOCATION_OVERRIDE);
            gCutsceneIndex = ANN_SPRAINED_ANKLE;
            loadCutscene(FALSE);
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS); 
        }
    }
    
    return bytecodeSegmentIndex;

}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", handleTimedDailyCutscenes);
 
// time-based daily cutscenes/update game
// mapIndex unused
u16 handleTimedDailyCutscenes(u8 mapIndex) {

    u16 bytecodeSegmentIndex = 0xFFFF;
    bool set = FALSE;
    
    if (!(gCutsceneFlags & CUTSCENE_ACTIVE)) {
        
        gCutsceneFlags &= ~CUTSCENE_SUPPRESS_NPC_SETUP;
    
        if (gHour >= 17 && !checkDailyEventBit(DAILY_SHIPMENT)) {

            if (dailyShippingBinValue) {
                gCutsceneIndex = 0;
            } else {
                gCutsceneIndex = 1;
            }

            handleDailyShipment();

            if (gBaseMapIndex == FARM) {
                bytecodeSegmentIndex = 0;
                set = TRUE;
            }       

        }
        
        if (!set) {

            if (checkLifeEventBit(HOUSE_EXTENSION_CONSTRUCTION)) {

                if (gHour >= 8 && !checkDailyEventBit(HOUSE_EXTENSION_WORK) && !checkDailyEventBit(END_OF_DAILY_HOUSE_EXTENSION_WORK)) {
                    
                    setDailyEventBit(HOUSE_EXTENSION_WORK);
                    
                    if (gBaseMapIndex == FARM) {
                        gCutsceneIndex = 0;
                        bytecodeSegmentIndex = 5;
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
            
            if (!checkDailyEventBit(COW_FESTIVAL_ELIGIBLE) && !checkDailyEventBit(COW_FESTIVAL_ENTRY_RESOLVED) && gSeason == AUTUMN && gDayOfMonth == 3 && (5 < gHour && gHour < 15) && getTotalFarmAnimalsByType(2)) {
               
                setDailyEventBit(COW_FESTIVAL_ELIGIBLE);
                
                if (gBaseMapIndex == FARM) {
                    bytecodeSegmentIndex = 33;
                    set = TRUE;
                    gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
                }
                
                if (gHour >= 16) {
                    if (!checkDailyEventBit(COW_FESTIVAL_ENTRY_RESOLVED)) {
                        setDailyEventBit(COW_FESTIVAL_ENTRY_RESOLVED);
                    }
                }

            }

        }
        
        if (!set) {
            if (gBaseMapIndex == FARM) {
                setFarmVisitCutscenes(1);
            }
        }
        
        if (bytecodeSegmentIndex != 0xFFFF) {
            initializeCutscene(bytecodeSegmentIndex);
        }
    }

    return bytecodeSegmentIndex;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", handleStartOfDayCutscenes);

// cutscenes on wake up: evaluation, wedding, rivals' weddings, animal funeral, sick days
u16 handleStartOfDayCutscenes(void) {

    bool set = FALSE;
    u16 result = 0xFFFF;
    u8 temp;

    if (!(gCutsceneFlags & CUTSCENE_ACTIVE)) {
        
        gCutsceneFlags &= ~CUTSCENE_SUPPRESS_NPC_SETUP;

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

            setSpawnPoint(VILLAGE_2_SPAWN_POINT_1);
            gCutsceneIndex = EVALUATION;
            loadCutscene(FALSE);
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            
        }

        // wedding
        if (!set && gDayOfWeek == SUNDAY && checkLifeEventBit(ENGAGED)) {
            
            clearLifeEventBit(ENGAGED);
            setLifeEventBit(MARRIED);
            setSpecialDialogueBit(MARRIED_DIALOGUE);
            
            anniversarySeason = gSeason;
            anniversaryDate = gDayOfMonth;
            
            switch (gWife) {                        
                case MARIA:                                 
                    gCutsceneIndex = MARIA_WEDDING;
                    setSpecialDialogueBit(MARRIED_TO_MARIA_DIALOGUE);
                    setSpecialDialogueBit(MARRIED_WIFE_DIALOGUE);
                    break;
                case 1:                                 
                    gCutsceneIndex = POPURI_WEDDING;
                    setSpecialDialogueBit(MARRIED_TO_POPURI_DIALOGUE);
                    setSpecialDialogueBit(MARRIED_WIFE_DIALOGUE);
                    break;
                case 2:                                 
                    gCutsceneIndex = ELLI_WEDDING;
                    setSpecialDialogueBit(MARRIED_TO_ELLI_DIALOGUE);
                    setSpecialDialogueBit(MARRIED_WIFE_DIALOGUE);
                    break;
                case 3:                                 
                    gCutsceneIndex = ANN_WEDDING;
                    setSpecialDialogueBit(MARRIED_TO_ANN_DIALOGUE);
                    setSpecialDialogueBit(MARRIED_WIFE_DIALOGUE);
                    break;
                case 4:                                 
                    gCutsceneIndex = KAREN_WEDDING;
                    setSpecialDialogueBit(MARRIED_TO_KAREN_DIALOGUE);
                    setSpecialDialogueBit(MARRIED_WIFE_DIALOGUE);
                    break;
                default:
                    break;
            }
            
            setSpawnPoint(VILLAGE_1_SPAWN_POINT_1);
            loadCutscene(FALSE);
            set = TRUE;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);

        }

        // rivals' weddings
        if (!checkDailyEventBit(FESTIVAL_DAY_CUTSCENE_GUARD)) { 
            
            if (!set && !checkLifeEventBit(MARIA_HARRIS_MARRIED) && (!checkLifeEventBit(MARRIED) || gWife != MARIA) && npcAffection[HARRIS] >= 200 && (checkLifeEventBit(MARRIED) || npcAffection[MARIA] < 120))  {
                
                setLifeEventBit(MARIA_HARRIS_MARRIED);
                setSpecialDialogueBit(MARIA_HARRIS_MARRIED_DIALOGUE);
                gCutsceneIndex = 203;
                setSpawnPoint(FARM_SPAWN_POINT_1);
                loadCutscene(FALSE);
                set = TRUE;
                gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
                
            }

            if (!set && !checkLifeEventBit(POPURI_GRAY_MARRIED) && (!checkLifeEventBit(MARRIED) || gWife != POPURI) && npcAffection[GRAY] >= 200 && (checkLifeEventBit(MARRIED) || npcAffection[POPURI] < 120)) {
               
                setLifeEventBit(POPURI_GRAY_MARRIED);
                setSpecialDialogueBit(POPURI_GRAY_MARRIED_DIALOGUE);
                gCutsceneIndex = 206;
                setSpawnPoint(FARM_SPAWN_POINT_1);
                loadCutscene(FALSE);
                set = TRUE;
                gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);

            }

            if (!set && !checkLifeEventBit(ELLI_JEFF_MARRIED) && (!checkLifeEventBit(MARRIED) || gWife != ELLI) && npcAffection[JEFF] >= 200 && (checkLifeEventBit(MARRIED) || npcAffection[ELLI] < 120)) {
                
                setLifeEventBit(ELLI_JEFF_MARRIED);
                setSpecialDialogueBit(ELLI_JEFF_MARRIED_DIALOGUE);
                gCutsceneIndex = 208;
                setSpawnPoint(FARM_SPAWN_POINT_1);
                loadCutscene(FALSE);
                set = TRUE;
                gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);

            }

            if (!set && !checkLifeEventBit(ANN_CLIFF_MARRIED) && (!checkLifeEventBit(MARRIED) || gWife != ANN) && npcAffection[CLIFF] >= 200 && (checkLifeEventBit(MARRIED) || npcAffection[ANN] < 120)) {
                
                setLifeEventBit(ANN_CLIFF_MARRIED);
                setSpecialDialogueBit(ANN_CLIFF_MARRIED_DIALOGUE);
                gCutsceneIndex = 214;
                setSpawnPoint(FARM_SPAWN_POINT_1);
                loadCutscene(FALSE);
                set = TRUE;
                gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);

            }
            
            if (!set && !checkLifeEventBit(KAREN_KAI_MARRIED) && (!checkLifeEventBit(MARRIED) || gWife != KAREN) && npcAffection[KAI] >= 200 && (checkLifeEventBit(MARRIED) || npcAffection[KAREN] < 120) && checkLifeEventBit(HARVEST_GODDESS_VINEYARD_BLESSING_DREAM) && !checkLifeEventBit(KAI_GONE) && !checkLifeEventBit(KAREN_GONE)) {
                
                setLifeEventBit(KAREN_KAI_MARRIED);
                setSpecialDialogueBit(KAREN_KAI_MARRIED_DIALOGUE);
                gCutsceneIndex = 217;
                setSpawnPoint(FARM_SPAWN_POINT_1);
                loadCutscene(FALSE);
                set = TRUE;
                gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);

            }
            
        }

        // animal funeral
        if (!set && checkLifeEventBit(ANIMAL_DIED) && !checkDailyEventBit(FESTIVAL_DAY_CUTSCENE_GUARD)) {
            
            clearLifeEventBit(ANIMAL_DIED);
            setLifeEventBit(ANIMAL_FUNERAL);
            setSpawnPoint(VILLAGE_2_SPAWN_POINT_1);
            gCutsceneIndex = 523;
            loadCutscene(FALSE);
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            set = TRUE;
            deadAnimalCount += adjustValue(deadAnimalCount, 1, 999);

        }

        // sick day
        if (!set && checkFatigueLevel() == 3) {

            gPlayer.fatigueCounter = 0;

            setSpawnPoint(HOUSE_SPAWN_POINT_1);

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
                        } else {
                            gCutsceneIndex = MARIA_VISIT_PLAYER_SICK;
                        }
                        
                        break;
                        
                    case 2:

                        if (!checkLifeEventBit(MARRIED) || gWife != POPURI) {
                            if (!checkLifeEventBit(MARRIED)) {                            
                                if (!checkLifeEventBit(POPURI_GRAY_MARRIED)) {
                                    gCutsceneIndex = 10;
                                }
                            }
                        } else {
                            gCutsceneIndex = 10;
                        }
                        
                        break;
                    
                    case 3:

                        if (!checkLifeEventBit(MARRIED) || gWife != ELLI) {
                            if (!checkLifeEventBit(MARRIED)) {                            
                                if (!checkLifeEventBit(ELLI_JEFF_MARRIED)) {
                                    gCutsceneIndex = 17;
                                }
                            }
                        } else {
                            gCutsceneIndex = 17;
                        }

                        break;
                    
                    case 4: 

                        if (!checkLifeEventBit(MARRIED) || gWife != ANN) {
                            if (!checkLifeEventBit(MARRIED)) {                            
                                if (!checkLifeEventBit(ANN_CLIFF_MARRIED)) {
                                    gCutsceneIndex = 24;
                                }
                            }
                        }  else{
                            gCutsceneIndex = 24;
                        }

                        break;

                    case 5:

                        if (!checkLifeEventBit(MARRIED) || gWife != KAREN) {
                            if (!checkLifeEventBit(MARRIED)) {                            
                                if (!checkLifeEventBit(KAREN_KAI_MARRIED)) {
                                    gCutsceneIndex = 31;
                                }
                            }
                        } else {
                            gCutsceneIndex = 31;
                        }

                        break;

                    default:
                        break;

                }

            } else {

                if (npcAffection[KENT] >= 50) {
                    gCutsceneIndex = 0;
                } else {
                    gCutsceneIndex = 51;
                }
            }
            
            loadCutscene(FALSE);
            gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
            set = 2;

            gSickDays += adjustValue(gSickDays, 1, MAX_SICK_DAYS);

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

    if (!checkLifeEventBit(HARVEST_GODDESS_VINEYARD_BLESSING_DREAM) && checkSpecialDialogueBit(GRAPEVINE_SPIRIT_AWAKENED_DIALOGUE) && gSeason == AUTUMN && gDayOfMonth == 6) {
        gCutsceneIndex = 40;
        loadCutscene(FALSE);
        set = TRUE;
        gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
    }

    if (!set) {
        if (!checkLifeEventBit(MARIA_DREAM_CUTSCENE) && npcAffection[MARIA] >= 160 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            gCutsceneIndex = 41;
            loadCutscene(FALSE);
            set = TRUE;
        }
    }

    if (!set) { 
        if (!checkLifeEventBit(POPURI_DREAM_CUTSCENE) && npcAffection[POPURI] >= 160 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            gCutsceneIndex = POPURI_DREAM;
            loadCutscene(FALSE);
            set = TRUE;
        }
    }
    
    if (!set) { 
        if (!checkLifeEventBit(ELLI_DREAM_CUTSCENE) && npcAffection[ELLI] >= 160 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            gCutsceneIndex = 43;
            loadCutscene(FALSE);
            set = TRUE;
        }
    }

    if (!set) { 
        if (!checkLifeEventBit(ANN_DREAM_CUTSCENE) && npcAffection[ANN] >= 160 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            gCutsceneIndex = 44;
            loadCutscene(FALSE);
            set = TRUE;
        }
    }

    if (!set) { 
        if (!checkLifeEventBit(KAREN_DREAM_CUTSCENE) && npcAffection[KAREN] >= 160 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            gCutsceneIndex = 45;
            loadCutscene(FALSE);
            set = TRUE;
        }
    }

    if (!set) { 
        if (!checkLifeEventBit(BABY_FEVER_CUTSCENE) && checkLifeEventBit(MARRIED) && checkLifeEventBit(HAVE_BABY) && !checkDailyEventBit(BIRTH_EVENT_DAILY) && gWife == MARIA && (29 < gBabyAge && gBabyAge < 120) && !getRandomNumberInRange(0, 10)) {
            setLifeEventBit(BABY_FEVER_CUTSCENE);
            gCutsceneIndex = 9;
            loadCutscene(FALSE);
            set = TRUE;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        }
    }

    if (!set) { 
        if (!checkLifeEventBit(BABY_FEVER_CUTSCENE) && checkLifeEventBit(MARRIED) && checkLifeEventBit(HAVE_BABY) && !checkDailyEventBit(BIRTH_EVENT_DAILY) && gWife == POPURI && (29 < gBabyAge && gBabyAge < 120) && !getRandomNumberInRange(0, 10)) {
            setLifeEventBit(BABY_FEVER_CUTSCENE);
            gCutsceneIndex = 16;
            loadCutscene(FALSE);
            set = TRUE;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        }
    }

    if (!set) { 
        if (!checkLifeEventBit(BABY_FEVER_CUTSCENE) && checkLifeEventBit(MARRIED) && checkLifeEventBit(HAVE_BABY) && !checkDailyEventBit(BIRTH_EVENT_DAILY) && gWife == ELLI && (29 < gBabyAge && gBabyAge < 120) && !getRandomNumberInRange(0, 10)) {
            setLifeEventBit(BABY_FEVER_CUTSCENE);
            gCutsceneIndex = 23;
            loadCutscene(FALSE);
            set = TRUE;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        }
    }

    if (!set) { 
        if (!checkLifeEventBit(BABY_FEVER_CUTSCENE) && checkLifeEventBit(MARRIED) && checkLifeEventBit(HAVE_BABY) && !checkDailyEventBit(BIRTH_EVENT_DAILY) && gWife == ANN && (29 < gBabyAge && gBabyAge < 120) && !getRandomNumberInRange(0, 10)) {
            setLifeEventBit(BABY_FEVER_CUTSCENE);
            gCutsceneIndex = 30;
            loadCutscene(FALSE);
            set = TRUE;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        }
    }

    if (!set) { 
        if (!checkLifeEventBit(BABY_FEVER_CUTSCENE) && checkLifeEventBit(MARRIED) && checkLifeEventBit(HAVE_BABY) && !checkDailyEventBit(BIRTH_EVENT_DAILY) && gWife == KAREN && (29 < gBabyAge && gBabyAge < 120) && !getRandomNumberInRange(0, 10)) {
            setLifeEventBit(BABY_FEVER_CUTSCENE);
            gCutsceneIndex = 37;
            loadCutscene(FALSE);
            set = TRUE;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        }
    }
    
    return set;
    
}

// INCLUDE_ASM("asm/nonmatchings/game/cutscenes", handleCutsceneCompletion);

void handleCutsceneCompletion(void) {

    bool found;
    u16 tempSpawnPoint;
    
    if (gCutsceneFlags & CUTSCENE_ACTIVE) {

        if (gCutsceneCompletionFlags & 1) {

            gCutsceneFlags &= ~CUTSCENE_ACTIVE;

            switch (gCutsceneBytecodeSegmentIndex) {

                case 1:                                 

                    clearDailyEventBit(PURCHASED_ANIMAL);
                
                    // switch on acquired animal
                    switch (selectedAnimalType) {               

                        case DOG_TYPE:                             
                            gNamingScreenIndex = NAMING_SCREEN_TYPE_DOG;
                            break;
                        
                        case HORSE_TYPE:                             
                            setLifeEventBit(ACQUIRED_HORSE);
                            gNamingScreenIndex = NAMING_SCREEN_TYPE_HORSE;
                            break;

                        case COW_TYPE:                             
                            setLifeEventBit(ACQUIRED_COW);
                            gNamingScreenIndex = NAMING_SCREEN_TYPE_COW;
                            gSelectedAnimalIndex = initializeNewFarmAnimal(1, 0);
                            break;
                            
                        case SHEEP_TYPE:                             
                            setLifeEventBit(ACQUIRED_SHEEP);
                            gNamingScreenIndex = NAMING_SCREEN_TYPE_SHEEP;
                            gSelectedAnimalIndex = initializeNewFarmAnimal(4, 0);
                            break;

                        case CHICKEN_TYPE:                             
                            setLifeEventBit(ACQUIRED_CHICKEN);
                            gNamingScreenIndex = NAMING_SCREEN_TYPE_CHICKEN;
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

                        case 170:
                            handleHatchChicken();
                            handleHatchChicken();
                            handleHatchChicken();
                            handleHatchChicken();
                            handleHatchChicken();
                            handleHatchChicken();
                            break;    
                        
                        case 113:
                            setSpawnPoint(FARM_SPAWN_POINT_1);
                            setMainLoopCallbackFunctionIndex(MAP_LOAD);
                            break;
                        
                        case 129:
                        case 130:
                        case 139:
                        case 239 ... 243:
                            if (gCutsceneCompletionFlags & 0x40000000) {
                                setMainLoopCallbackFunctionIndex(END_OF_FESTIVAL_DAY_1);
                            }
                            break;
                        
                        case 171:
                            break;
                        
                        case 250 ... 256:
                            setMainLoopCallbackFunctionIndex(END_OF_FESTIVAL_DAY_1);
                            break;
                        
                    }
                    
                    break;

                // house cutscenes
                case 9:
                    
                    switch (gCutsceneIndex) {           

                        case 1:                           
                        case 2:                           
                            setLifeEventBit(KENT_HOUSE_VISIT_CUTSCENE);
                            break;
                        case 38:                          
                        case 39:                          
                            setLifeEventBit(CLIFF_HOUSE_VISIT_CUTSCENE);
                            break;
                        case 40:                          
                            setLifeEventBit(HARVEST_GODDESS_VINEYARD_BLESSING_DREAM);
                        case 0:                           
                        case MARIA_VISIT_PLAYER_SICK:                           
                        case 5:                           
                        case 9:                           
                        case 10:                           
                        case 12:                           
                        case 16:                          
                        case 17:                          
                        case 19:                          
                        case 23:                          
                        case 24:                          
                        case 26:                          
                        case 30:                          
                        case 31:                          
                        case 33:                          
                        case 37:                          
                        case 51:                          
                            setMainLoopCallbackFunctionIndex(END_OF_DAY_1);
                            break;
                        case MARIA_DREAM:                          
                            setLifeEventBit(MARIA_DREAM_CUTSCENE);
                            setMainLoopCallbackFunctionIndex(END_OF_DAY_1);
                            break;
                        case POPURI_DREAM:                          
                            setLifeEventBit(POPURI_DREAM_CUTSCENE);
                            setMainLoopCallbackFunctionIndex(END_OF_DAY_1);                             
                            break;
                        case ELLI_DREAM:                          
                            setLifeEventBit(ELLI_DREAM_CUTSCENE);
                            setMainLoopCallbackFunctionIndex(END_OF_DAY_1);                             
                            break;
                        case ANN_DREAM:                          
                            setLifeEventBit(ANN_DREAM_CUTSCENE);
                            setMainLoopCallbackFunctionIndex(END_OF_DAY_1);                             
                            break;
                        case KAREN_DREAM:                          
                            setLifeEventBit(KAREN_DREAM_CUTSCENE);
                            setMainLoopCallbackFunctionIndex(END_OF_DAY_1);                             
                            break;
                        case MARIA_BIRTH:                           
                        case POPURI_BIRTH:                           
                        case ELLI_BIRTH:                          
                        case ANN_BIRTH:                          
                        case KAREN_BIRTH:                          
                            if (gHour >= 6) {
                                gNamingScreenIndex = NAMING_SCREEN_TYPE_BABY;
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
                        setSpawnPoint(RANCH_SPAWN_POINT_3);
                        setMainLoopCallbackFunctionIndex(MAP_LOAD);
                    }
                
                case 11:

                    switch (gCutsceneIndex) {

                        case 302:
                            if (gCutsceneCompletionFlags & 0x40000000) {
                                gHour += 2;
                                setSpawnPoint(VILLAGE_1_SPAWN_POINT_3);
                                setMainLoopCallbackFunctionIndex(MAP_LOAD);
                            }
                            break;

                        case ELLI_SPRAINED_ANKLE:
                            if (gCutsceneCompletionFlags & 0x40000000) {
                                gHour += 2;
                                setSpawnPoint(VILLAGE_1_SPAWN_POINT_4);
                                setMainLoopCallbackFunctionIndex(MAP_LOAD);
                            }
                            break;

                        case MOUNTAIN_FIREWORKS:
                        case STARRY_NIGHT_MOON_MOUNTAIN:
                            setMainLoopCallbackFunctionIndex(END_OF_FESTIVAL_DAY_1);
                            break;

                        case 318:                         
                            storeTool(FISHING_POLE);
                            break;
                        
                        case 324:
                        case 328:
                            setDailyEventBit(MOUNTAIN_CARPENTER_HELP);
                            break;

                        case 327:
                            if (gSeason == AUTUMN && gDayOfMonth == 27) {
                                setLifeEventBit(BRIDGE_COMPLETED);
                            }
                            break;

                        case 330:                         
                            if (gSeason == WINTER && gDayOfMonth == 16) {
                                setLifeEventBit(HOT_SPRINGS_COMPLETED);
                                setDailyEventBit(HOT_SPRINGS_COMPLETED_PHOTO_MAIL);
                            }
                            clearForagableObjects(TOP_OF_MOUNTAIN_1);
                            break;
                        
                        case 336:                         
                            clearDailyEventBit(KAPPA_FISH_OFFERING_DAILY);
                            setLifeEventBit(MET_KAPPA);
                            break;
                        
                        case 337:                         
                            clearDailyEventBit(KAPPA_LARGE_FISH_OFFERING_DAILY);
                            setLifeEventBit(KAPPA_POWER_NUT_GIFT);
                            break;
                        
                        case 341:                         
                        case 344:                         
                            clearDailyEventBit(HARVEST_GODDESS_OFFERING);
                            setDailyEventBit(HARVEST_GODDESS_INTERACTION);
                            break;
                        
                        case 345:                         
                            setSpecialDialogueBit(GRAPEVINE_SPIRIT_AWAKENED_DIALOGUE);
                            setMainLoopCallbackFunctionIndex(END_OF_FESTIVAL_DAY_1);
                            break;
                        
                    }
                    
                    break;
                
                case 12:            
                                    
                    switch (gCutsceneIndex) {           

                        case 659:
                            setMainLoopCallbackFunctionIndex(END_OF_FESTIVAL_DAY_1);
                            break;

                        case RANCH_GET_HORSE:                         
                        case 665:                         
                            gNamingScreenIndex = NAMING_SCREEN_TYPE_HORSE;
                            handleExitLevel(1, NAMING_SCREEN_LOAD);
                            setLifeEventBit(HAVE_HORSE);
                            break;
                        case 651:                         
                            loadLevel(0);
                            setMainLoopCallbackFunctionIndex(SET_AUDIO_AND_LIGHTING);
                            initializeHorse();
                            break;
                        case 663:
                            setSpawnPoint(RANCH_STORE_SPAWN_POINT_2);
                            setMainLoopCallbackFunctionIndex(MAP_LOAD);
                            break;
                        case 664:                         
                            npcAffection[ANN] = 120;
                            clearLifeEventBit(WIFE_LEFT);
                            setMainLoopCallbackFunctionIndex(END_OF_DAY_1);
                            break;
                        }

                    break;

                case 13:
                    
                    switch (gCutsceneIndex) {          

                        case 701:
                            if (gCutsceneCompletionFlags & 0x40000000) {
                                gHour += 2;
                                setSpawnPoint(VINEYARD_SPAWN_POINT_2);
                                setMainLoopCallbackFunctionIndex(MAP_LOAD);
                            }
                            break;
                        
                        case 704:
                            setSpawnPoint(VINEYARD_HOUSE_SPAWN_POINT_1);
                            setMainLoopCallbackFunctionIndex(MAP_LOAD);
                            break;
                        
                        case KAREN_YELLOW_HEART_EVENT_1:
                            if (gCutsceneCompletionFlags & 0x40000000) {
                                clearDailyEventBit(KAREN_YELLOW_HEART_EVENT_DAILY);
                                setSpawnPoint(VINEYARD_SPAWN_POINT_3);
                                setMainLoopCallbackFunctionIndex(MAP_LOAD);
                            }
                            break;
                        
                        case 710:                         
                            npcAffection[KAREN] = 120;
                            clearLifeEventBit(WIFE_LEFT);
                            setMainLoopCallbackFunctionIndex(END_OF_DAY_1);
                            break;
                    }
                    
                    break;
                
                case 14:

                    switch (gCutsceneIndex) {          
                        
                        case 400:
                        case 401:
                        case 402:
                        case 403:
                        case 404:
                            setMainLoopCallbackFunctionIndex(END_OF_DAY_1);
                            break;
                        case 405:                 
                        case 406:                 
                        case 407:                 
                        case 408:                 
                        case 409:        
                            setMainLoopCallbackFunctionIndex(END_OF_FESTIVAL_DAY_1);
                            break; 
                        
                        case 411:
                            setSpawnPoint(MOUNTAIN_2_SPAWN_POINT_3);
                            setMainLoopCallbackFunctionIndex(MAP_LOAD);
                            blueMistSeedsQuantity += adjustValue(blueMistSeedsQuantity, -1, 1);
                            removeTool(BLUE_MIST_SEEDS);
                            blueMistFlowerPlot = 0xB3;
                            pondFieldTiles[5][3] = 0xB3;
                            break;
                            
                        case 416:                        
                            setLifeEventBit(ELLEN_DIED);
                            setSpecialDialogueBit(ELLEN_DIED_DIALOGUE);
                            setMainLoopCallbackFunctionIndex(END_OF_FESTIVAL_DAY_1);
                            break;
                        
                        case 418:                 
                        case 420:                 
                        case 429:                 
                        case 430:                 
                        case 431:                 
                        case 435:                 
                        case 436:                 
                            setMainLoopCallbackFunctionIndex(END_OF_FESTIVAL_DAY_1);
                            break;

                        case STARRY_NIGHT_FESTIVAL:                        
                            if (!(gCutsceneCompletionFlags & 2)) {
                                setMainLoopCallbackFunctionIndex(END_OF_FESTIVAL_DAY_1);
                            }
                            break;

                        case 437:
                            setSpawnPoint(FLOWER_SHOP_SPAWN_POINT_2);
                            setMainLoopCallbackFunctionIndex(MAP_LOAD);
                            break;
                
                        case 438:
                            setSpawnPoint(BAKERY_SPAWN_POINT_2);
                            setMainLoopCallbackFunctionIndex(MAP_LOAD);
                            break;
                
                        case 439:                        
                            npcAffection[POPURI] = 120;
                            clearLifeEventBit(WIFE_LEFT);
                            setMainLoopCallbackFunctionIndex(END_OF_DAY_1);
                            break;
                
                        case 440:                        
                            npcAffection[ELLI] = 120;
                            clearLifeEventBit(WIFE_LEFT);
                            setMainLoopCallbackFunctionIndex(END_OF_DAY_1);
                            break;

                        }
                    
                    break;

                case 15:

                    switch (gCutsceneIndex) {           

                        case 523:                        
                            setMainLoopCallbackFunctionIndex(END_OF_DAY_1);
                            break;

                        case 524:
                            setSpawnPoint(MAYOR_HOUSE_SPAWN_POINT_2);
                            setMainLoopCallbackFunctionIndex(MAP_LOAD);
                            break;
                            
                        case 526:
                            if (gCutsceneCompletionFlags & 0x40000000) {
                                gHour += 2;
                                setSpawnPoint(VILLAGE_2_SPAWN_POINT_4);
                                setMainLoopCallbackFunctionIndex(MAP_LOAD);
                            }
                            break;

                        case 529:                         
                            npcAffection[MARIA] = 120;
                            clearLifeEventBit(WIFE_LEFT);
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

                        case 902:
                        case 903:

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
                    if (gCutsceneIndex == FIREWORKS_FESTIVAL) {
                        setMainLoopCallbackFunctionIndex(END_OF_FESTIVAL_DAY_1);
                    } 
                    break;

                case 22:
                case 23:
                    setMainLoopCallbackFunctionIndex(END_OF_FESTIVAL_DAY_1);
                    break;
                
                case 24:   
                    if (checkLifeEventBit(COW_FESTIVAL_WON_THIS_YEAR)) {
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
                    
                    if (gCutsceneIndex == 1302) {
                        
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
                    
                    if (gCutsceneIndex == 1553) {

                        u32 ptr1 = (u32)&_fontPalette1SegmentRomStart;
                        u32 ptr2 =  (u32)&_fontPalette1SegmentRomEnd;
                    
                        // fontPalette1
                        nuPiReadRom((u32)&_fontPalette1SegmentRomStart, FONT_PALETTE_1_BUFFER, ptr2 - ptr1);
                        setMessageBoxFont(0, 14, 14, FONT_TEXTURE_BUFFER, FONT_PALETTE_1_BUFFER);
                        setMessageBoxInterpolationWithFlags(0, -4, 0);
                        setMessageBoxSfx(0, 0x57, 8, 1);
 
                        if (D_80180710 == 0) {
                            showDialogueTextBox(DIALOGUE_MENU_EVALUATION_CONTINUE_PROMPT);
                        } else {
                            launchIntroCutscene(FUNERAL, SQUARE_SPAWN_POINT_1, 1);
                        }

                    }

                    break;
                
                case 32:
                    if (gCutsceneIndex == DEMO_CUTSCENE_2) {
                        launchIntroCutscene(1502, MOUNTAIN_1_SPAWN_POINT_1, 1);
                    } else {
                        memcpy(farmFieldTiles, D_80113580, FIELD_HEIGHT * FIELD_WIDTH);
                        launchIntroCutscene(FUNERAL, SQUARE_SPAWN_POINT_1, 1);
                    }
                    break;

                case 35:
                    if ((gCutsceneIndex == 1600) && !(gCutsceneCompletionFlags & 2)) {
                        memcpy(farmFieldTiles, D_80113580, FIELD_HEIGHT * FIELD_WIDTH);
                        initializeTitleScreen(1);
                    }
                    break;
                
            }

            // opening cutscenes (funeral, mayor tour)
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
                        transitionCutscenes(2, VILLAGE_2_SPAWN_POINT_1);
                        break;

                    case 38:         
                        transitionCutscenes(39, 0);
                        gCutsceneFlags &= ~CUTSCENE_DEACTIVATE_ANIMALS_ON_COMPLETION;
                        gCutsceneFlags |= CUTSCENE_REINIT_ANIMALS_ON_COMPLETION;
                        break;
                    
                    case 147:          
                        deactivateSprites();
                        deactivateGlobalSprites();
                        initializeCutsceneExecutors();
                        deactivateNPCEntities();
                        deactivateAnimalEntities();
                        clearAllItemContextSlots();
                        initializeEntityInstances(1);
                        transitionCutscenes(250, 72);
                        break;

                    case 150:          
                    case 161:          
                    case 173:          
                    case 184:          
                    case 197:          
                        deactivateSprites();
                        deactivateGlobalSprites();
                        initializeCutsceneExecutors();
                        deactivateNPCEntities();
                        deactivateAnimalEntities();
                        clearAllItemContextSlots();
                        initializeEntityInstances(1);
                        transitionCutscenes(251, MOUNTAIN_1_SPAWN_POINT_1);
                        break; 
                    
                    case 151:          
                    case 162:          
                    case 174:          
                    case 185:          
                    case 198:          
                        deactivateSprites();
                        deactivateGlobalSprites();
                        initializeCutsceneExecutors();
                        deactivateNPCEntities();
                        deactivateAnimalEntities();
                        clearAllItemContextSlots();
                        initializeEntityInstances(1);
                        transitionCutscenes(252, MOON_MOUNTAIN_SPAWN_POINT_1);
                        break;
                
                    case 158:          
                        deactivateSprites();
                        deactivateGlobalSprites();
                        initializeCutsceneExecutors();
                        deactivateNPCEntities();
                        deactivateAnimalEntities();
                        clearAllItemContextSlots();
                        initializeEntityInstances(1);
                        transitionCutscenes(253, 72);
                        break;

                    case 169:          
                        deactivateSprites();
                        deactivateGlobalSprites();
                        initializeCutsceneExecutors();
                        deactivateNPCEntities();
                        deactivateAnimalEntities();
                        clearAllItemContextSlots();
                        initializeEntityInstances(1);
                        transitionCutscenes(254, 72);
                        break;
                    
                    case 181:          
                        deactivateSprites();
                        deactivateGlobalSprites();
                        initializeCutsceneExecutors();
                        deactivateNPCEntities();
                        deactivateAnimalEntities();
                        clearAllItemContextSlots();
                        initializeEntityInstances(1);
                        transitionCutscenes(255, POND_SPAWN_POINT_1);
                        break;
                    
                    case 193:          
                        deactivateSprites();
                        deactivateGlobalSprites();
                        initializeCutsceneExecutors();
                        deactivateNPCEntities();
                        deactivateAnimalEntities();
                        clearAllItemContextSlots();
                        initializeEntityInstances(1);
                        transitionCutscenes(256, VINEYARD_SPAWN_POINT_1);
                        break;
                    
                    case 203:          
                        deactivateSprites();
                        deactivateGlobalSprites();
                        initializeCutsceneExecutors();
                        deactivateNPCEntities();
                        deactivateAnimalEntities();
                        clearAllItemContextSlots();
                        initializeEntityInstances(1);
                        transitionCutscenes(405, VILLAGE_1_SPAWN_POINT_1);
                        break;

                    case 206:
                        deactivateSprites();
                        deactivateGlobalSprites();
                        initializeCutsceneExecutors();
                        deactivateNPCEntities();
                        deactivateAnimalEntities();
                        clearAllItemContextSlots();
                        initializeEntityInstances(1);
                        transitionCutscenes(406, VILLAGE_1_SPAWN_POINT_1); 
                        break;
                    
                    case 208:      
                        deactivateSprites();
                        deactivateGlobalSprites();
                        initializeCutsceneExecutors();
                        deactivateNPCEntities();
                        deactivateAnimalEntities();
                        clearAllItemContextSlots();
                        initializeEntityInstances(1);
                        transitionCutscenes(407, VILLAGE_1_SPAWN_POINT_1);
                        break;

                    case 214:      
                        deactivateSprites();
                        deactivateGlobalSprites();
                        initializeCutsceneExecutors();
                        deactivateNPCEntities();
                        deactivateAnimalEntities();
                        clearAllItemContextSlots();
                        initializeEntityInstances(1);
                        transitionCutscenes(408, VILLAGE_1_SPAWN_POINT_1);
                        break;
                    
                    case 217:      
                        deactivateSprites();
                        deactivateGlobalSprites();
                        initializeCutsceneExecutors();
                        deactivateNPCEntities();
                        deactivateAnimalEntities();
                        clearAllItemContextSlots();
                        initializeEntityInstances(1);
                        transitionCutscenes(409, VILLAGE_1_SPAWN_POINT_1);
                        break;

                    case 244:
                    case 246:
                    case 247:
                        deactivateSprites();
                        deactivateGlobalSprites();
                        initializeCutsceneExecutors();
                        deactivateNPCEntities();
                        deactivateAnimalEntities();
                        clearAllItemContextSlots();
                        initializeEntityInstances(1);
                        transitionCutscenes(STARRY_NIGHT_FESTIVAL, CHURCH_SPAWN_POINT_1);
                        break;
                    
                    case 245:      
                        transitionCutscenes(305, MOON_MOUNTAIN_SPAWN_POINT_1);
                        break;
                    
                    case 248:      
                        transitionCutscenes(756, BEACH_SPAWN_POINT_1);
                        break;
                    
                    case 324:
                        transitionCutscenes(326, TOP_OF_MOUNTAIN_1_SPAWN_POINT_1);
                        break;

                    case 326:     
                        transitionCutscenes(327, MOUNTAIN_1_SPAWN_POINT_1);
                        break;
                    
                    case 328:     
                        transitionCutscenes(329, TOP_OF_MOUNTAIN_1_SPAWN_POINT_1);
                        break;
                    
                    case 329:     
                        transitionCutscenes(330, MOUNTAIN_1_SPAWN_POINT_1);
                        break;
                    
                    case 344:     
                        transitionCutscenes(345, VINEYARD_SPAWN_POINT_1);
                        break;

                    case 346:     
                        // set cutscene wild card/score variables
                        D_801886D2 = getRandomNumberInRange(0, 9);
                        D_80180710 = getRandomNumberInRange(0, 9);
                        clearForagableObjects(MINE_2);
                        transitionCutscenes(347, MINE_2_SPAWN_POINT_1);
                        break;

                    case 347:
                    case 348:
                        
                        if (gHour >= 17 || gPlayer.currentStamina < 2) {
                            gCutsceneIndex = 349;
                            tempSpawnPoint = CAVE_SPAWN_POINT_3;
                        } else {
    
                            if (D_80189824 < 0xA) {
                                // set cutscene wild card/score variables
                                D_801886D2 = getRandomNumberInRange(0, 9);
                                D_80180710 = getRandomNumberInRange(0, 9);
                            } else {
                                D_80180710 = 0xFF;
                                D_801886D2 = 0xFF;
                            }

                            gCutsceneIndex = 348;
                            tempSpawnPoint = MINE_2_SPAWN_POINT_1;
                            
                            clearForagableObjects(MINE_2);
                 
                        }
                        
                        setSpawnPoint(tempSpawnPoint);
                        loadCutscene(FALSE);
                        loadMapAtSpawnPoint(tempSpawnPoint);

                        break;

                    case 410:     
                        transitionCutscenes(411, 72);
                        break;
                    
                    case 426:     
                        transitionCutscenes(443, VILLAGE_1_SPAWN_POINT_1);
                        break;

                    case STARRY_NIGHT_FESTIVAL:

                        setSpawnPoint(SQUARE_SPAWN_POINT_1);
    
                        if (!(gCutsceneCompletionFlags & 8)) {

                            if (!(gCutsceneCompletionFlags & 0x10)) {
                                
                                if ((gCutsceneCompletionFlags & 0x20)) {
                                    gCutsceneIndex = 431;
                                } 
                                
                            } else {
                                gCutsceneIndex = 430;
                            }
                            
                        } else {
                            gCutsceneIndex = 429;
    
                        }
                        
                        loadCutscene(FALSE);
                        loadMapAtSpawnPoint(SQUARE_SPAWN_POINT_1);
                        break;

                    case 441:
                    case 444:
                        setSpawnPoint(POPURI_ROOM_SPAWN_POINT_1);
                        if (gCutsceneIndex == 441) {
                            gCutsceneIndex = 437;
                        } else {
                            gCutsceneIndex = 439;
                        }
                        loadCutscene(FALSE);
                        loadMapAtSpawnPoint(POPURI_ROOM_SPAWN_POINT_1);
                        break;
                    
                    case 442:
                    case 445:
                        setSpawnPoint(ELLI_ROOM_SPAWN_POINT_1);
                        if (gCutsceneIndex == 442) {
                            gCutsceneIndex = 438;
                        } else {
                            gCutsceneIndex = 440;
                        }
                        loadCutscene(FALSE);
                        loadMapAtSpawnPoint(ELLI_ROOM_SPAWN_POINT_1);
                        break;

                    case 525:
                        setSpawnPoint(VILLAGE_2_SPAWN_POINT_1);
                        gCutsceneIndex = 526;
                        loadCutscene(FALSE);
                        loadMapAtSpawnPoint(VILLAGE_2_SPAWN_POINT_1);
                        break;
                    
                    case 526:
                        setSpawnPoint(VILLAGE_2_SPAWN_POINT_4);
                        loadMapAtSpawnPoint(VILLAGE_2_SPAWN_POINT_4);
                        break;
                    
                    case 527:
                        transitionCutscenes(528, VILLAGE_2_SPAWN_POINT_1);
                        break;   

                    case 530:
                    case 531:
                        setSpawnPoint(MARIA_ROOM_SPAWN_POINT_1);
                        if (gCutsceneIndex == 530) {
                            gCutsceneIndex = 524;
                        } else {
                            gCutsceneIndex = 529;
                        }
                        loadCutscene(FALSE);
                        loadMapAtSpawnPoint(MARIA_ROOM_SPAWN_POINT_1);
                        break;

                    case ANN_SICK_VISIT_CUTSCENE:
                    case 667:
                        setSpawnPoint(ANN_ROOM_SPAWN_POINT_1);
                        if (gCutsceneIndex == ANN_SICK_VISIT_CUTSCENE) {
                            gCutsceneIndex = 663;
                        } else {
                            gCutsceneIndex = 664;
                        }
                        loadCutscene(FALSE);
                        loadMapAtSpawnPoint(ANN_ROOM_SPAWN_POINT_1);
                        break;
                    
                    case KAREN_YELLOW_HEART_EVENT_1:
                        gCutsceneIndex = 706;
                        loadCutscene(FALSE);
                        loadMapAtSpawnPoint(VINEYARD_CELLAR_BASEMENT_SPAWN_POINT_1);
                        break;

                    case 706:
                        transitionCutscenes(707, VINEYARD_CELLAR_SPAWN_POINT_1);
                        break;

                    case 707:
                    case 708:
                        clearDailyEventBit(KAREN_YELLOW_HEART_EVENT_DAILY);
                        transitionCutscenes(709, VINEYARD_SPAWN_POINT_1);
                        break;
                    
                    case 711:
                    case 712:
                        setSpawnPoint(KAREN_ROOM_SPAWN_POINT_1);
                        if (gCutsceneIndex == 711) {
                            gCutsceneIndex = 704;
                        } else {
                            gCutsceneIndex = 710;
                        }
                        loadCutscene(FALSE);
                        loadMapAtSpawnPoint(KAREN_ROOM_SPAWN_POINT_1);
                        break;
                    
                    case ANN_SPRAINED_ANKLE:
                        setSpawnPoint(RANCH_SPAWN_POINT_3);
                        loadMapAtSpawnPoint(RANCH_SPAWN_POINT_3);
                        break;
                    
                    case 900:
                    case HORSE_RACE_STARTING_ANNOUNCEMENT:
                         
                        if (gHour >= 17) {
                            transitionCutscenes(904, SQUARE_SPAWN_POINT_1);
                        } else {

                            if (gRacingContext.playerRaceNumber == gRacingContext.currentRaceIndex) {
                                gCutsceneIndex = 903;
                                // set cutscene wild card/score variables
                                D_80189824 = horseInfo.affection;
                                D_801886D2 = (horseInfo.affection / 3) * 2;
                                D_80180710 = horseInfo.affection / 3;
                            } else {
                                gCutsceneIndex = 902;
                            }

                            loadCutscene(FALSE);
                            
                        }
                        
                        break;

                    case 902:
                    case 903:
                        break;

                    case VEGETABLE_FESTIVAL_VILLAGE:
                        deactivateSprites();
                        deactivateGlobalSprites();
                        initializeCutsceneExecutors();
                        deactivateNPCEntities();
                        deactivateAnimalEntities();
                        clearAllItemContextSlots();
                        initializeEntityInstances(2);
                        transitionCutscenes(VEGETABLE_FESTIVAL_SQUARE, SQUARE_SPAWN_POINT_1);
                        break;

                    case 1300:
                    case 1301:
                        if (gHour >= 17) {
                            transitionCutscenes(1303, SQUARE_SPAWN_POINT_1);
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
                            
                            gCutsceneIndex = 1302;
                            loadCutscene(FALSE);
                            
                        }
                        
                        break;

                    case 1302:
                        setupPlayerEntity(RACE_TRACK_SPAWN_POINT_1, 1);
                        handlePlayerAnimation();
                        setEntitiesRGBA(0, 0, 0, 0);
                        setSpawnPoint(RACE_TRACK_SPAWN_POINT_1);
                        gCutsceneIndex = 1301;
                        loadCutscene(FALSE);
                        loadMapAtSpawnPoint(RACE_TRACK_SPAWN_POINT_1);
                        break;

                    case FUNERAL:             
                        transitionCutscenes(1451, 0);
                        break;

                    case 1451:             
                        transitionCutscenes(1452, ROAD_SPAWN_POINT_1);
                        break;

                    case MAYOR_VILLAGE_TOUR:
                        transitionCutscenes(1454, VILLAGE_2_SPAWN_POINT_1);
                        break;
                    
                    case 1454:            
                        transitionCutscenes(1455, 0);
                        gCutsceneFlags &= ~CUTSCENE_DEACTIVATE_ANIMALS_ON_COMPLETION;
                        gCutsceneFlags |= CUTSCENE_REINIT_ANIMALS_ON_COMPLETION;
                        break;

                    case 1501:
                        break;

                    case EVALUATION:             
                        transitionCutscenes(1551, MOUNTAIN_1_SPAWN_POINT_1);
                        break;

                    case 1551:             
                        transitionCutscenes(1552, HOUSE_SPAWN_POINT_1);
                        break;

                    case 1552:             
                        transitionCutscenes(1553, 0);
                        break;

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
                            tempSpawnPoint = FARM_SPAWN_POINT_1;
                            gCutsceneIndex = 1601;
                            
                            
                        } else if (D_80189824 == 1) {

                            gSeason = SPRING;

                            tempSpawnPoint = FARM_SPAWN_POINT_1;
                            
                            switch (D_801886D2) {

                                case 0:
                                    memcpy(farmFieldTiles, D_80115210, FIELD_HEIGHT * FIELD_WIDTH); 
                                    gCutsceneIndex = 1602;
                                    break;

                                case 1:
                                    memcpy(farmFieldTiles, D_801153F0, FIELD_HEIGHT * FIELD_WIDTH); 
                                    gCutsceneIndex = 1603;
                                    break;

                                case 2:
                                    memcpy(farmFieldTiles, D_801155D0, FIELD_HEIGHT * FIELD_WIDTH); 
                                    gCutsceneIndex = 1604;
                                    break;

                                case 3:
                                    memcpy(farmFieldTiles, D_801157B0, FIELD_HEIGHT * FIELD_WIDTH); 
                                    gCutsceneIndex = 1605;
                                    break;
                                
                                case 4:
                                    memcpy(farmFieldTiles, D_80115990, FIELD_HEIGHT * FIELD_WIDTH); 
                                    gCutsceneIndex = 1606;
                                    break;
                                
                            }

                        } else {

                            gSeason = SUMMER;
                            tempSpawnPoint = 0;
                            
                            switch (D_801886D2) {

                                case 0:
                                    memcpy(farmFieldTiles, D_80115B70, FIELD_HEIGHT * FIELD_WIDTH); 
                                    gCutsceneIndex = 1607;
                                    break;

                                case 1:
                                    memcpy(farmFieldTiles, D_80115D50, FIELD_HEIGHT * FIELD_WIDTH); 
                                    gCutsceneIndex = 1608;
                                    break;

                                case 2:
                                    memcpy(farmFieldTiles, D_80115F30, FIELD_HEIGHT * FIELD_WIDTH); 
                                    tempSpawnPoint = BARN_SPAWN_POINT_1;
                                    gCutsceneIndex = 1609;
                                    break;

                                case 3:
                                    memcpy(farmFieldTiles, D_80116110, FIELD_HEIGHT * FIELD_WIDTH); 
                                    gCutsceneIndex = 1610;
                                    break;
                                
                                case 4:
                                    memcpy(farmFieldTiles, D_80116110, FIELD_HEIGHT * FIELD_WIDTH); 
                                    gCutsceneIndex = 1611;
                                    break;
                                
                            }
                            
                        }

                        setSpawnPoint(tempSpawnPoint);
                        loadCutscene(FALSE);
                        loadMapAtSpawnPoint(tempSpawnPoint);

                        break;

                    case 1601 ... 1611:
                        gCutsceneIndex = 1600;
                        loadCutscene(FALSE); 
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
            
            if (((gCutsceneIndex == HARVEST_FESTIVAL) || (gCutsceneIndex == NEW_YEAR_FESTIVAL))) {
                setEntitySpriteDimensions(ENTITY_ASSET_SHIPPER, 12, 12);
                setEntitySpriteDimensions(ENTITY_ASSET_ASSISTANT_CARPENTER, 12, 12);
            }

            if (checkDailyEventBit(95)) {
                // empty function 
                func_800657BC();
            }

            gCutsceneCompletionFlags = 0;
            
        }

        // horse/dog race flags
        if (gCutsceneCompletionFlags & 4) {
            setGameVariableString(40, gRacingContext.oddsAndNames.racerNameLookup[gRaceFinishOrder[0]], 6);
            setGameVariableString(41, gRacingContext.oddsAndNames.racerNameLookup[gRaceFinishOrder[1]], 6);
            gCutsceneCompletionFlags &= ~4;
        }

        // sprite color flags
        if (gCutsceneCompletionFlags & 0x40) {
            setSpritePaletteIndex(98, 0);
            gCutsceneCompletionFlags &= ~0x40;
        }
        if (gCutsceneCompletionFlags & 0x80) {
            setSpritePaletteIndex(98, 1);
            gCutsceneCompletionFlags &= ~0x80;
        }

        if (gCutsceneCompletionFlags & 0x100) {

            u32 ptr1 = (u32)&_fontPalette3SegmentRomStart;
            u32 ptr2 =  (u32)&_fontPalette3SegmentRomEnd;
            
            // fontPalette3
            nuPiReadRom(ptr1, FONT_PALETTE_1_BUFFER, ptr2 - ptr1);
            setMessageBoxFont(0, 14, 14, FONT_TEXTURE_BUFFER, FONT_PALETTE_1_BUFFER);
            setMessageBoxInterpolationWithFlags(0, 1, 1);
            setMessageBoxSfx(0, 0xFF, 0xFF, 0xFF);
            gCutsceneCompletionFlags &= ~0x100;
        }

        // map flags

        if (gCutsceneCompletionFlags & 0x20000) {
            setSpawnPoint(FARM_SPAWN_POINT_1);
            // prep map
            loadMapAtSpawnPoint(0);
            setMapControllerRGBA(0, 255, 255, 255, 255);
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
            setSpawnPoint(COOP_SPAWN_POINT_1);
            loadMapAtSpawnPoint(COOP_SPAWN_POINT_1);
            setMapControllerRGBA(0, 0, 0, 0, 0);
            gCutsceneCompletionFlags &= ~0x200000;
        }

        if (gCutsceneCompletionFlags & 0x400000) {
            setSpawnPoint(BARN_SPAWN_POINT_1);
            // prep map
            loadMapAtSpawnPoint(BARN_SPAWN_POINT_1);
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

                case DIRECTION_SW:
                    tempX--;
                case DIRECTION_S:
                    tempZ++;
                    break;
                case DIRECTION_W:
                    tempX--;
                    break;
                case DIRECTION_NW:
                    tempX--;
                    tempZ--;
                    break;
                case DIRECTION_N:
                    tempZ--;
                    break;
                case DIRECTION_NE:
                    tempX++; 
                    tempZ--;
                    break;
                case DIRECTION_E:
                    tempX++;
                    break;
                case DIRECTION_SE:
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

                            if (!(acquiredPowerNutBits & MINE_POWER_NUT) && gItemBeingHeld == 0xFF && !(getRandomNumberInRange(0, 100))) {
                                gItemBeingHeld = 254;
                                acquiredPowerNutBits |= MINE_POWER_NUT;
                            }

                            if (!checkSpecialDialogueBit(HAVE_WEATHER_VANE_DIALOGUE)) {
                                if (gItemBeingHeld == 0xFF) {
                                    if (!(getRandomNumberInRange(0, 100))) {
                                        gItemBeingHeld = 253;
                                        setSpecialDialogueBit(HAVE_WEATHER_VANE_DIALOGUE);
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

                                // these direct assignments to gItemBeingHeld are bugs
                                // typically, gItemBeingHeld = getHeldItemDialogueItemIndex(heldItemIndex); is called, which provides a different index that doesn't match with the gPlayer.heldItem index
                                if (getRandomNumberInRange(0, 100) < 4) {
                                    gPlayer.heldItem = BLUE_ROCK_HELD_ITEM;
                                    gItemBeingHeld = BLUE_ROCK_HELD_ITEM;
                                }

                                temp2 = gItemBeingHeld; 

                                if (gItemBeingHeld == temp) {
                                    
                                    if (getRandomNumberInRange(0, 100) < 2) {
                                        gPlayer.heldItem = RARE_METAL_HELD_ITEM;
                                        gItemBeingHeld = RARE_METAL_HELD_ITEM;
                                    }

                                    if (gItemBeingHeld == temp2) {

                                        if (getRandomNumberInRange(0, 100) < 3) {
                                            gPlayer.heldItem = PONTATA_ROOT_HELD_ITEM;
                                            gItemBeingHeld = PONTATA_ROOT_HELD_ITEM;
                                        }
                                        
                                    }
                                    
                                }
                                
                            }
                            
                        }
                        
                    }

                    if (!found) {

                        if (gItemBeingHeld == 0xFF) {

                            // junk
                            if (getRandomNumberInRange(0, 100) < 10) {
                                gItemBeingHeld = 252;
                            }
                            
                        }
                        
                    }

                    if (!found) {

                        if (gItemBeingHeld == 0xFF) {
                            if (getRandomNumberInRange(0, 100) < 8) {
                                gPlayer.heldItem = IRON_ORE_HELD_ITEM;
                                gItemBeingHeld = IRON_ORE_HELD_ITEM;
                            }
                        }
                        
                    }

                    if (!found) {

                        if (gItemBeingHeld == 0xFF) {
                            if (getRandomNumberInRange(0, 100) < 5) {
                                gPlayer.heldItem = MOONLIGHT_STONE_HELD_ITEM;
                                gItemBeingHeld = MOONLIGHT_STONE_HELD_ITEM;
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
            setDailyEventBit(SUSPEND_TIME_DURING_ANIMATION);
        } else {
            clearDailyEventBit(SUSPEND_TIME_DURING_ANIMATION);
        }

        if (gCutsceneCompletionFlags & 0x2000) {
            setDailyEventBit(BLOCK_PAUSE_SCREEN);
        } else {
            clearDailyEventBit(BLOCK_PAUSE_SCREEN);
        }

        if (gCutsceneCompletionFlags & 0x4000) {
            setDailyEventBit(BLOCK_BUTTON_USAGE);
        } else {
            clearDailyEventBit(BLOCK_BUTTON_USAGE);
        }

        if (gCutsceneCompletionFlags & 0x400) {
            setDailyEventBit(DIARY_BLOCKED);
        } else {
            clearDailyEventBit(DIARY_BLOCKED);
        }

        if (gCutsceneCompletionFlags & 0x200) {
            setDailyEventBit(DAY_CONSUMING_CUTSCENE);
        }

        if (checkDailyEventBit(HARVEST_GODDESS_STAMINA_WISH_DAILY)) {
            clearDailyEventBit(HARVEST_GODDESS_STAMINA_WISH_DAILY);
            gPlayer.currentStamina += adjustValue(gPlayer.currentStamina, 50, gMaximumStamina);
        }

        if (checkDailyEventBit(SHOOTING_STAR_ANIMAL_WISH_DAILY)) {
            clearDailyEventBit(SHOOTING_STAR_ANIMAL_WISH_DAILY);
            adjustAllAnimalAffection(5);
        }

        if (gCutsceneCompletionFlags & 0x8000) {
            clearHeldItemsAtEndOfDay();
            gCutsceneCompletionFlags &= ~0x8000;
        }

        if (gCutsceneFlags & 8) {
            initializeAnimalEntities();
            gCutsceneFlags &= ~CUTSCENE_REINIT_ANIMALS_ON_COMPLETION;
        }

        if (gCutsceneFlags & 0x10) {
            deactivateAnimalsAfterCutscene();
            gCutsceneFlags &= ~CUTSCENE_DEACTIVATE_ANIMALS_ON_COMPLETION;
        }

        if (gCutsceneCompletionFlags & 0x10000000) {
            initializeAnimalEntities();
            gCutsceneCompletionFlags &= ~0x10000000;
        }

        if (gCutsceneCompletionFlags & 0x20000000) {
            deactivateAnimalsAfterCutscene();
            gCutsceneCompletionFlags &= ~0x20000000;
        }

        if (checkLifeEventBit(STU_MARBLE_TRADE)) {
            clearLifeEventBit(STU_MARBLE_TRADE);
            acquireKeyItem(MARBLES);
            removePinkCatMintFlowerFromFarm();
        }
        
        if (checkLifeEventBit(HARVEST_SPRITE_POWER_NUT_TRADE)) {
            clearLifeEventBit(HARVEST_SPRITE_POWER_NUT_TRADE);
            removeKeyItem(MARBLES);
            setSpecialDialogueBit(HARVEST_SPRITES_POWER_NUT_TRADE_DIALOGUE);
        }
        
        if (checkLifeEventBit(BOUGHT_SUPER_HIGH_GRADE_CHICKEN_FEED)) {
            clearLifeEventBit(BOUGHT_SUPER_HIGH_GRADE_CHICKEN_FEED);
            storeTool(CHICKEN_FEED);
            chickenFeedQuantity += adjustValue(chickenFeedQuantity, 10, 999);
        }

        if (checkLifeEventBit(BOUGHT_NECTAR_FRUIT_SEEDS)) {

            clearLifeEventBit(BOUGHT_NECTAR_FRUIT_SEEDS);
            
            switch (getRandomNumberInRange(0, 5)) {                    
                
                case 0:                                 
                    storeTool(TURNIP_SEEDS);
                    turnipSeedsQuantity += adjustValue(turnipSeedsQuantity, 1, 20);
                    return;
                case 1:                                 
                    storeTool(POTATO_SEEDS);
                    potatoSeedsQuantity += adjustValue(potatoSeedsQuantity, 1, 20);
                    return;
                case 2:                                 
                    storeTool(CABBAGE_SEEDS);
                    cabbageSeedsQuantity += adjustValue(cabbageSeedsQuantity, 1, 20);
                    return;
                case 3:                                 
                    storeTool(TOMATO_SEEDS);
                    tomatoSeedsQuantity += adjustValue(tomatoSeedsQuantity, 1, 20);
                    return;
                case 4:                                 
                    storeTool(CORN_SEEDS);
                    cornSeedsQuantity += adjustValue(cornSeedsQuantity, 1, 20);
                    return;
                case 5:                                 
                    storeTool(EGGPLANT_SEEDS);
                    eggplantSeedsQuantity += adjustValue(eggplantSeedsQuantity, 1, 20);
                    break;
                
            }
            
        }
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", loadCutscene);

void loadCutscene(bool morningVisit) {
    
    u16 bytecodeSegmentIndex;

    if (gCutsceneIndex >= 1600) {

        // how to play
        
        gCameraRotationOffset = 0;
        setInitialMapRotation(MAIN_MAP_INDEX, MAP_ROTATION_SW);
        bytecodeSegmentIndex = 35;
        
    } else if (gCutsceneIndex >= 1550) {

        // evaluation/credits
    
        gCameraRotationOffset = 0;
        setInitialMapRotation(MAIN_MAP_INDEX, MAP_ROTATION_SW);
        bytecodeSegmentIndex = 31;        
    
    } else if (gCutsceneIndex >= 1500) {

        // demos
             
        gCameraRotationOffset = 0;
        setInitialMapRotation(MAIN_MAP_INDEX, MAP_ROTATION_SW);

        memcpy(farmFieldTiles, D_80114E50, FIELD_HEIGHT * FIELD_WIDTH);

        bytecodeSegmentIndex = 32;
        
    } else if (gCutsceneIndex >= 1450) {

        // opening
        // mayor tour
        
        gCameraRotationOffset = 0;
        setInitialMapRotation(MAIN_MAP_INDEX, MAP_ROTATION_SW);
        
        gSeason = SPRING;

        memcpy(farmFieldTiles, D_80113580, FIELD_HEIGHT * FIELD_WIDTH);

        bytecodeSegmentIndex = 30;
        
    } else if (gCutsceneIndex >= 1400) {

        // new year festival
    
        bytecodeSegmentIndex = 29;
        setEntitySpriteDimensions(ENTITY_ASSET_SHIPPER, 12, 24);
        setEntitySpriteDimensions(ENTITY_ASSET_ASSISTANT_CARPENTER, 12, 24);
    
    } else if (gCutsceneIndex >= 1350) {
        // spirit festival
        bytecodeSegmentIndex = 28;
    } else if (gCutsceneIndex >= 1300) {
        // dog race
        bytecodeSegmentIndex = 27;  
    } else if (gCutsceneIndex >= 1250) {
        // egg festival
        bytecodeSegmentIndex = 26;
    } else if (gCutsceneIndex >= 1200) {

        // harvest festival
        
        bytecodeSegmentIndex = 25;
        setEntitySpriteDimensions(ENTITY_ASSET_SHIPPER, 12, 24);
        setEntitySpriteDimensions(ENTITY_ASSET_ASSISTANT_CARPENTER, 12, 24);
        
    } else if (gCutsceneIndex >= 1150) {
        // cow festival
        bytecodeSegmentIndex = 24;
    } else if (gCutsceneIndex >= 1100) {
        // sea festival
        bytecodeSegmentIndex = 23;
    } else if (gCutsceneIndex >= 1050) {
        // firefly festival
        bytecodeSegmentIndex = 22;
    } else if (gCutsceneIndex >= 1000) {
        // vegetable festival
        bytecodeSegmentIndex = 20;    
    } else if (gCutsceneIndex >= 950) {
        // flower festival
        bytecodeSegmentIndex = 19;
    } else if (gCutsceneIndex >= 900) {
        // horse race
        bytecodeSegmentIndex = 18;  
    } else if (gCutsceneIndex >= 850) {
        // sowing festival
        bytecodeSegmentIndex = 17;  
    } else if (gCutsceneIndex >= 800) {
        // road
        bytecodeSegmentIndex = 10;  
    } else if (gCutsceneIndex >= 750) {
        // beach
        bytecodeSegmentIndex = 16;  
    } else if (gCutsceneIndex >= 700) {
        // vineyard
        bytecodeSegmentIndex = 13;  
    } else if (gCutsceneIndex >= 650) {
        // ranch
        bytecodeSegmentIndex = 12;
    } else if (gCutsceneIndex >= 600) {
        // fireworks festival
        bytecodeSegmentIndex = 21;
    } else if (gCutsceneIndex >= 500) {
        // vllage 2
        bytecodeSegmentIndex = 15;    
    } else if (gCutsceneIndex >= 400) {
        // village 1
        bytecodeSegmentIndex = 14;
    } else if (gCutsceneIndex >= 300) {
        // mountain
        bytecodeSegmentIndex = 11;
    } else if (gCutsceneIndex >= 100) {
        
        if (morningVisit) {
            bytecodeSegmentIndex = 7;
        } else  {

            bytecodeSegmentIndex = 8;

            // farm visits
            switch (gCutsceneIndex) {

                case 220:
                    gCameraRotationOffset = 0;
                    setInitialMapRotation(MAIN_MAP_INDEX, MAP_ROTATION_SW);
                    break;
                case 228 ... 238:
                    gCameraRotationOffset = 0;
                    setInitialMapRotation(MAIN_MAP_INDEX, MAP_ROTATION_SW);
                    break;
                default:
                    break;
            
            }
            
        } 
        
    } else {
        
        bytecodeSegmentIndex = 9;
        
        // family
        if (gCutsceneIndex == 39) {
            gCameraRotationOffset = 0;
            setInitialMapRotation(MAIN_MAP_INDEX, MAP_ROTATION_SW);
        } 
        
    }

    initializeCutscene(bytecodeSegmentIndex);
    
    switch (gCutsceneIndex) {
        case STARRY_NIGHT_MOON_MOUNTAIN:
        case MARIA_WEDDING ... KAREN_WEDDING:
        case STARRY_NIGHT_FESTIVAL:
        case 523:
        case 756:
        case SOWING_FESTIVAL_POTION_SHOP_DEALER ... SOWING_FESTIVAL_BACHELOR:
        case HORSE_RACE_SQUARE: 
        case FLOWER_FESTIVAL:
        case 1050:
        case SEA_FESTIVAL:
        case COW_FESTIVAL:
        case 1200:
        case EGG_FESTIVAL:
        case DOG_RACE_SQUARE:
        case 1350 ... 1352:
        case NEW_YEAR_FESTIVAL:
        case FUNERAL:
        case OPENING_LOGOS:
        case DEMO_CUTSCENE_1: 
        case DEMO_CUTSCENE_2:
        case DEMO_CUTSCENE_3:
        case EVALUATION:
        case HOW_TO_PLAY_CUTSCENE:
            setDailyEventBit(CUTSCENE_AUDIO_OVERRIDE);
            break;
    }
    
}