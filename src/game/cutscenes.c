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
u32 raceStateFlags;
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
    CUTSCENE_BYTECODE_BUFFER_1 + CUTSCENE_BANK_SHIPPING_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_1 + CUTSCENE_BANK_ANIMAL_DELIVERY_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_1 + CUTSCENE_BANK_ANIMAL_SALE_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_1 + CUTSCENE_BANK_ANIMAL_SOLD_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_1 + CUTSCENE_BANK_HOUSE_EXTENSION_ESTIMATE_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_1 + CUTSCENE_BANK_HOUSE_EXTENSION_WORK_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_1 + CUTSCENE_BANK_HOUSE_EXTENSION_COMPLETED_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_1 + CUTSCENE_BANK_FARM_VISIT_MORNING_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_2 + CUTSCENE_BANK_FARM_VISIT_WORKING_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_2 + CUTSCENE_BANK_HOUSE_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_2 + CUTSCENE_BANK_ROAD_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_2 + CUTSCENE_BANK_MOUNTAIN_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_2 + CUTSCENE_BANK_RANCH_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_2 + CUTSCENE_BANK_VINEYARD_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_2 + CUTSCENE_BANK_VILLAGE_1_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_2 + CUTSCENE_BANK_VILLAGE_2_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_2 + CUTSCENE_BANK_BEACH_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_2 + CUTSCENE_BANK_SOWING_FESTIVAL_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_2 + CUTSCENE_BANK_HORSE_RACE_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_2 + CUTSCENE_BANK_FLOWER_FESTIVAL_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_2 + CUTSCENE_BANK_VEGETABLE_FESTIVAL_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_2 + CUTSCENE_BANK_FIREWORKS_FESTIVAL_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_2 + CUTSCENE_BANK_FIREFLY_FESTIVAL_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_2 + CUTSCENE_BANK_SEA_FESTIVAL_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_2 + CUTSCENE_BANK_COW_FESTIVAL_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_2 + CUTSCENE_BANK_HARVEST_FESTIVAL_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_2 + CUTSCENE_BANK_EGG_FESTIVAL_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_2 + CUTSCENE_BANK_DOG_RACE_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_2 + CUTSCENE_BANK_SPIRIT_FESTIVAL_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_2 + CUTSCENE_BANK_NEW_YEAR_FESTIVAL_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_2 + CUTSCENE_BANK_INTRO_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_2 + CUTSCENE_BANK_ENDING_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_2 + CUTSCENE_BANK_DEMOS_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_1 + CUTSCENE_BANK_COW_FESTIVAL_ENTRY_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_1 + CUTSCENE_BANK_COW_FESTIVAL_TAKE_COW_ENTRY_OFFSET,
    CUTSCENE_BYTECODE_BUFFER_2 + CUTSCENE_BANK_HOW_TO_PLAY_ENTRY_OFFSET,
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
    CUTSCENE_BANK_SHIPPING_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_ANIMAL_DELIVERY_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_ANIMAL_SALE_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_ANIMAL_SOLD_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_HOUSE_EXTENSION_ESTIMATE_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_HOUSE_EXTENSION_WORK_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_HOUSE_EXTENSION_COMPLETED_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_FARM_VISIT_MORNING_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_FARM_VISIT_WORKING_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_HOUSE_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_ROAD_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_MOUNTAIN_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_RANCH_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_VINEYARD_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_VILLAGE_1_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_VILLAGE_2_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_BEACH_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_SOWING_FESTIVAL_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_HORSE_RACE_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_FLOWER_FESTIVAL_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_VEGETABLE_FESTIVAL_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_FIREWORKS_FESTIVAL_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_FIREFLY_FESTIVAL_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_SEA_FESTIVAL_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_COW_FESTIVAL_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_HARVEST_FESTIVAL_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_EGG_FESTIVAL_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_DOG_RACE_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_SPIRIT_FESTIVAL_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_NEW_YEAR_FESTIVAL_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_INTRO_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_ENDING_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_DEMOS_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_COW_FESTIVAL_ENTRY_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_COW_FESTIVAL_TAKE_COW_STARTING_EXECUTOR_INDEX,
    CUTSCENE_BANK_HOW_TO_PLAY_STARTING_EXECUTOR_INDEX,
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

inline void initializeCutscene(u16 segmentIndex) {

    nuPiReadRom(cutsceneBytecodeAddresses[segmentIndex].romAddrStart, cutsceneBankLoadAddresses[segmentIndex], cutsceneBytecodeAddresses[segmentIndex].romAddrEnd - cutsceneBytecodeAddresses[segmentIndex].romAddrStart);

    gCutsceneCompletionFlags = 0;

    spawnCutsceneExecutor(cutsceneExecutorIndices[segmentIndex], cutsceneEntryPointAddresses[segmentIndex]);
    
    unpauseEntities();
    
    gCutsceneBytecodeSegmentIndex = segmentIndex;

    gCutsceneFlags |= CUTSCENE_ACTIVE;

    clearDailyEventBit(EAT_BREAKFAST);
    
}

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

// arg0 = cutscene type; 0 = show up while working; 1 = morning/show up on map entry
u16 setFarmVisitCutscenes(bool morningVisit) {

    bool set = FALSE;
    u16 result = 0xFFFF;
    u8 tempGirl;
    
#if TESTING
// skip mayor tour
#else
    if (!checkLifeEventBit(MAYOR_TOUR)) {
        setLifeEventBit(MAYOR_TOUR);
        gCutsceneIndex = CUTSCENE_MAYOR_VILLAGE_TOUR;
        gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
        loadCutscene(morningVisit);
        set = TRUE;
    }
#endif

    if (gSpawnPointIndex != FARM_SPAWN_POINT_11) {
        
        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gYear == 1 && gSeason == AUTUMN && gDayOfMonth == 22 && (5 < gHour && gHour < 18)) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_MOUNTAIN_BRIDGE_CONSTRUCTION_REQUEST;
                loadCutscene(morningVisit);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gYear == 1 && gSeason == WINTER && gDayOfMonth == 11 && (5 < gHour && gHour < 18)) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_HOT_SPRINGS_CONSTRUCTION_REQUEST;
                loadCutscene(morningVisit);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == SPRING && gDayOfMonth == 16 && (5 < gHour && gHour < 15) && (horseInfo.flags & HORSE_ACTIVE) && horseInfo.grown == TRUE) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_DOUG_HORSE_RACE;
                loadCutscene(morningVisit);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == AUTUMN && gDayOfMonth == 27 && (5 < gHour && gHour < 15) && (horseInfo.flags & HORSE_ACTIVE) && horseInfo.grown == TRUE) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_DOUG_HORSE_RACE;
                loadCutscene(morningVisit);
                set = TRUE;
            }
        }

        if (!set) {
            
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == WINTER && gDayOfMonth == 18 && (5 < gHour && gHour < 15)) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_DOUG_DOG_RACE;
                loadCutscene(morningVisit);
                set = TRUE;
            }
        
        }

        if (!set) {

            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == SPRING && gDayOfMonth == 7 && (5 < gHour && gHour < 9)) {
                
                setDailyEventBit(FARM_MORNING_VISIT);
                
                if (npcAffection[KENT] < 30) {
                    if (gYear == 1) {
                        gCutsceneIndex = CUTSCENE_FARM_VISIT_MAYOR_SOWING_FESTIVAL_YEAR_1;
                    } else {
                        gCutsceneIndex = CUTSCENE_FARM_VISIT_MAYOR_SOWING_FESTIVAL;
                    }
                } else {
                    gCutsceneIndex = CUTSCENE_FARM_VISIT_KENT_SOWING_FESTIVAL;
                }
                
                loadCutscene(morningVisit);
                set = TRUE;
        
            }
        
        }

        if (!set) {

            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == AUTUMN && gDayOfMonth == 2 && (5 < gHour && gHour < 9) && npcAffection[KENT] >= 30) {
                setDailyEventBit(FARM_MORNING_VISIT);
                
                if (getTotalFarmAnimalsByType(2)) {
                    gCutsceneIndex = CUTSCENE_FARM_VISIT_KENT_COW_FESTIVAL_WITH_COWS;
                    setGameVariableString(14, gFarmAnimals[getCowWithHighestAffection()].name, 6);
                } else {
                    gCutsceneIndex = CUTSCENE_FARM_VISIT_KENT_COW_FESTIVAL_NO_COWS;
                }
                
                loadCutscene(morningVisit);
                set = TRUE;

            }

        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == WINTER && gDayOfMonth == 9 && (5 < gHour && gHour < 9) && npcAffection[KENT] >= 30) {
                setDailyEventBit(FARM_MORNING_VISIT);
                
                if (gYear == 1) {
                    gCutsceneIndex = CUTSCENE_FARM_VISIT_KENT_THANKSGIVING_FESTIVAL;
                } else {
                    gCutsceneIndex = CUTSCENE_FARM_VISIT_KENT_THANKSGIVING_FESTIVAL_RECURRING;
                }
                
                loadCutscene(morningVisit);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == SPRING && gDayOfMonth == 16 && (5 < gHour && gHour < 9)) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_MAYOR_HORSE_RACE;
                loadCutscene(morningVisit);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == SUMMER && gDayOfMonth == 17 && (5 < gHour && gHour < 9)) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_MAYOR_FIREFLY_FESTIVAL;
                loadCutscene(morningVisit);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == SUMMER && gDayOfMonth == 23 && (5 < gHour && gHour < 9)) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_MAYOR_SEA_FESTIVAL;
                loadCutscene(morningVisit);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == AUTUMN && gDayOfMonth == 11 && (5 < gHour && gHour < 9)) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_MAYOR_HARVEST_FESTIVAL;
                loadCutscene(morningVisit);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == AUTUMN && gDayOfMonth == 19 && (5 < gHour && gHour < 9)) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_MAYOR_EGG_FESTIVAL;
                loadCutscene(morningVisit);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == WINTER && gDayOfMonth == 27 && (5 < gHour && gHour < 9)) {
                
                setDailyEventBit(FARM_MORNING_VISIT);
                
                if (gHarvestKing == PLAYER) {
                    gCutsceneIndex = CUTSCENE_FARM_VISIT_MAYOR_SPIRIT_FESTIVAL_PLAYER_HARVEST_KING;
                } else {
                    gCutsceneIndex = CUTSCENE_FARM_VISIT_MAYOR_SPIRIT_FESTIVAL;
                }
                
                loadCutscene(morningVisit);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == SPRING && gDayOfMonth == 19 && (5 < gHour && gHour < 9)) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_BASIL_FLOWER_FESTIVAL;
                loadCutscene(morningVisit);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == SUMMER && gDayOfMonth == 8 && (5 < gHour && gHour < 9)) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_BASIL_VEGETABLE_FESTIVAL;
                loadCutscene(morningVisit);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(POPURI_CUTSCENE_DAILY) && npcAffection[POPURI] >= 50 && gSeason == SPRING && gDayOfMonth == 22 && (5 < gHour && gHour < 9) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
                setDailyEventBit(POPURI_CUTSCENE_DAILY);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_POPURI_BIRTHDAY_VISIT;
                loadCutscene(morningVisit);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(ELLI_CUTSCENE_DAILY) && npcAffection[ELLI] >= 50 && gSeason == AUTUMN && gDayOfMonth == 11 && (6 < gHour && gHour < 9) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {
                setDailyEventBit(ELLI_CUTSCENE_DAILY);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_ELLI_HARVEST_FESTIVAL_INVITATION;
                loadCutscene(morningVisit);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(ANN_CUTSCENE_DAILY) && npcAffection[ANN] >= 80 && gSeason == SPRING && gDayOfMonth == 16 && (6 < gHour && gHour < 9) && !checkLifeEventBit(MARRIED) && checkLifeEventBit(ENTERED_HORSE_RACE) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
                setDailyEventBit(ANN_CUTSCENE_DAILY);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_ANN_HORSE_RACE_DAY_VISIT;
                loadCutscene(morningVisit);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(MARIA_CUTSCENE_DAILY) && npcAffection[MARIA] >= 50 && gSeason == SUMMER && gDayOfMonth == 1 && gHour == 8 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
                setDailyEventBit(MARIA_CUTSCENE_DAILY);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_MARIA_FIREWORKS_INVITATION;
                loadCutscene(morningVisit);
                set = TRUE;
            }            
        }
        
        if (!set) {
            if (!checkDailyEventBit(POPURI_CUTSCENE_DAILY) && npcAffection[POPURI] >= 50 && gSeason == SUMMER && gDayOfMonth == 1 && gHour == 10 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
                setDailyEventBit(POPURI_CUTSCENE_DAILY);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_POPURI_FIREWORKS_INVITATION;
                loadCutscene(morningVisit);
                set = TRUE;
            }            
        }

        if (!set) {
            if (!checkDailyEventBit(ELLI_CUTSCENE_DAILY) && npcAffection[ELLI] >= 50 && gSeason == SUMMER && gDayOfMonth == 1 && gHour == 12 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {
                setDailyEventBit(ELLI_CUTSCENE_DAILY);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_ELLI_FIREWORKS_INVITATION;
                loadCutscene(morningVisit);
                set = TRUE;
            }            
        }

        if (!set) {
            if (!checkDailyEventBit(ANN_CUTSCENE_DAILY) && npcAffection[ANN] >= 50 && gSeason == SUMMER && gDayOfMonth == 1 && gHour == 14 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
                setDailyEventBit(ANN_CUTSCENE_DAILY);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_ANN_FIREWORKS_INVITATION;
                loadCutscene(morningVisit);
                set = TRUE;
            }            
        }

        if (!set) {
            if (!checkDailyEventBit(KAREN_CUTSCENE_DAILY) && npcAffection[KAREN] >= 50 && gSeason == SUMMER && gDayOfMonth == 1 && gHour == 16 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
                setDailyEventBit(KAREN_CUTSCENE_DAILY);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_KAREN_FIREWORKS_INVITATION;
                loadCutscene(morningVisit);
                set = TRUE;
            }            
        }
        
        if (!set) {
            if (!checkDailyEventBit(MARIA_CUTSCENE_DAILY) && npcAffection[MARIA] >= 140 && gSeason == WINTER && gDayOfMonth == 10 && gHour == 8 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
                setDailyEventBit(MARIA_CUTSCENE_DAILY);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_MARIA_THANKSGIVING_GIFT;
                loadCutscene(morningVisit);
                set = TRUE;
            }            
        }        

        if (!set) {
            if (!checkDailyEventBit(POPURI_CUTSCENE_DAILY) && npcAffection[POPURI] >= 140 && gSeason == WINTER && gDayOfMonth == 10 && gHour == 10 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
                setDailyEventBit(POPURI_CUTSCENE_DAILY);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_POPURI_THANKSGIVING_GIFT;
                loadCutscene(morningVisit);
                set = TRUE;
            }            
        }        

        if (!set) {
            if (!checkDailyEventBit(ELLI_CUTSCENE_DAILY) && npcAffection[ELLI] >= 140 && gSeason == WINTER && gDayOfMonth == 10 && gHour == 12 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {
                setDailyEventBit(ELLI_CUTSCENE_DAILY);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_ELLI_THANKSGIVING_GIFT;
                loadCutscene(morningVisit);
                set = TRUE;
            }            
        }        

        if (!set) {
            if (!checkDailyEventBit(ANN_CUTSCENE_DAILY) && npcAffection[ANN] >= 140 && gSeason == WINTER && gDayOfMonth == 10 && gHour == 14 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
                setDailyEventBit(ANN_CUTSCENE_DAILY);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_ANN_THANKSGIVING_GIFT;
                loadCutscene(morningVisit);
                set = TRUE;
            }            
        }    

        if (!set) {
            if (!checkDailyEventBit(KAREN_CUTSCENE_DAILY) && npcAffection[KAREN] >= 140 && gSeason == WINTER && gDayOfMonth == 10 && gHour == 16 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
                setDailyEventBit(KAREN_CUTSCENE_DAILY);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_KAREN_THANKSGIVING_GIFT;
                loadCutscene(morningVisit);
                set = TRUE;
            }            
        }       

        if (!set) {
            if (!checkDailyEventBit(MARIA_CUTSCENE_DAILY) && npcAffection[MARIA] >= 100 && gSeason == WINTER && gDayOfMonth == 24 && gHour == 8 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
                setDailyEventBit(MARIA_CUTSCENE_DAILY);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_MARIA_STARRY_NIGHT_VISIT;
                loadCutscene(morningVisit);
                set = TRUE;
            }            
        }        

        if (!set) {
            if (!checkDailyEventBit(POPURI_CUTSCENE_DAILY) && npcAffection[POPURI] >= 100 && gSeason == WINTER && gDayOfMonth == 24 && gHour == 10 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
                setDailyEventBit(POPURI_CUTSCENE_DAILY);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_POPURI_STARRY_NIGHT_VISIT;
                loadCutscene(morningVisit);
                set = TRUE;
            }            
        }        

        if (!set) {
            if (!checkDailyEventBit(ELLI_CUTSCENE_DAILY) && npcAffection[ELLI] >= 100 && gSeason == WINTER && gDayOfMonth == 24 && gHour == 12 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {
                setDailyEventBit(ELLI_CUTSCENE_DAILY);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_ELLI_STARRY_NIGHT_VISIT;
                loadCutscene(morningVisit);
                set = TRUE;
            }            
        }       

        if (!set) {
            if (!checkDailyEventBit(ANN_CUTSCENE_DAILY) && npcAffection[ANN] >= 100 && gSeason == WINTER && gDayOfMonth == 24 && gHour == 14 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
                setDailyEventBit(ANN_CUTSCENE_DAILY);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_ANN_STARRY_NIGHT_VISIT;
                loadCutscene(morningVisit);
                set = TRUE;
            }            
        }       

        if (!set) {
            if (!checkDailyEventBit(KAREN_CUTSCENE_DAILY) && npcAffection[KAREN] >= 100 && gSeason == WINTER && gDayOfMonth == 24 && gHour == 16 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
                setDailyEventBit(KAREN_CUTSCENE_DAILY);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_KAREN_STARRY_NIGHT_VISIT;
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
                                    gCutsceneIndex = CUTSCENE_FARM_VISIT_MARIA_PLAYER_BIRTHDAY_GIFT;
                                } else {
                                    gCutsceneIndex = CUTSCENE_FARM_VISIT_MARIA_PLAYER_BIRTHDAY;
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
                                    gCutsceneIndex = CUTSCENE_FARM_VISIT_POPURI_PLAYER_BIRTHDAY_GIFT;
                                } else {
                                    gCutsceneIndex = CUTSCENE_FARM_VISIT_POPURI_PLAYER_BIRTHDAY;
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
                                    gCutsceneIndex = CUTSCENE_FARM_VISIT_ELLI_PLAYER_BIRTHDAY_GIFT;
                                } else {
                                    gCutsceneIndex = CUTSCENE_FARM_VISIT_ELLI_PLAYER_BIRTHDAY;
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
                                    gCutsceneIndex = CUTSCENE_FARM_VISIT_ANN_PLAYER_BIRTHDAY_GIFT;
                                } else {
                                    gCutsceneIndex = CUTSCENE_FARM_VISIT_ANN_PLAYER_BIRTHDAY;
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
                                    gCutsceneIndex = CUTSCENE_FARM_VISIT_KAREN_PLAYER_BIRTHDAY_GIFT;
                                } else {
                                    gCutsceneIndex = CUTSCENE_FARM_VISIT_KAREN_PLAYER_BIRTHDAY;
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
                        gCutsceneIndex = CUTSCENE_FARM_VISIT_BASIL_POWER_NUT_GIFT;
                    } else {
                        gCutsceneIndex = CUTSCENE_FARM_VISIT_BASIL_RETURN;
                    }
                } else {
                    gCutsceneIndex = CUTSCENE_FARM_VISIT_BASIL_INTRO;
                }
                
                loadCutscene(morningVisit);
                set = TRUE;
            }            
        }

        if (!set) {
            if (checkLifeEventBit(BASIL_IN_TOWN) && ((gSeason == AUTUMN && gDayOfMonth >= 5) || gSeason == WINTER) && (5 < gHour && gHour < 17)) {
                clearLifeEventBit(BASIL_IN_TOWN);
                setLifeEventBit(BASIL_GONE);      
                gCutsceneIndex = CUTSCENE_FARM_VISIT_BASIL_LEAVING;
                loadCutscene(morningVisit);
                set = TRUE;
            }
        }

        if (!set && morningVisit == FALSE) {
            if (!checkLifeEventBit(CLIFF_ARRIVED) && (gSeason == SPRING && !(gDayOfMonth < 18) || !(gSeason < SUMMER))) {
                setLifeEventBit(CLIFF_ARRIVED);      
                gCutsceneIndex = CUTSCENE_FARM_VISIT_CLIFF_INTRO;
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
                    gCutsceneIndex = CUTSCENE_FARM_VISIT_CLIFF_LEAVING_ANN_MARRIED;
                    loadCutscene(morningVisit);
                    set = TRUE;                    
                }
                
                if (npcAffection[CLIFF] < 51 && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {              
                    setLifeEventBit(CLIFF_LEAVING);
                    setLifeEventBit(CLIFF_GONE);
                    gCutsceneIndex = CUTSCENE_FARM_VISIT_CLIFF_LEAVING_DEFAULT;
                    loadCutscene(morningVisit);
                    set = TRUE;                                
                } 
            }
        }

        if (!set) {
            if (!checkLifeEventBit(KENT_CHICKEN_VISIT) && checkDailyEventBit(KENT_ACQUIRED_CHICKEN_VISIT_DAILY) && npcAffection[KENT] >= 5 && (5 < gHour && gHour < 9)) {
                setLifeEventBit(KENT_CHICKEN_VISIT);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_KENT_CHICKEN_VISIT;
                loadCutscene(morningVisit);
                set = TRUE;                               
            }
        }
        
        if (!set) {
            if (!checkLifeEventBit(KENT_SHEEP_VISIT) && checkDailyEventBit(KENT_ACQUIRED_SHEEP_VISIT_DAILY) && npcAffection[KENT] >= 5 && (5 < gHour && gHour < 9)) {
                setLifeEventBit(KENT_SHEEP_VISIT);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_KENT_SHEEP_VISIT;
                loadCutscene(morningVisit);
                set = TRUE;                               
            }
        }
        
        if (!set) {

            if (!checkLifeEventBit(KENT_COW_VISIT) && checkDailyEventBit(KENT_ACQUIRED_COW_VISIT_DAILY) && npcAffection[KENT] >= 5 && gWeather == SUNNY && (5 < gHour && gHour < 9)) {
                
                setLifeEventBit(KENT_COW_VISIT);

                // get cow + grass count
                if (!func_8009B828(0)) {
                    gCutsceneIndex = CUTSCENE_FARM_VISIT_KENT_COW_VISIT_NO_GRASS;
                } else {
                    gCutsceneIndex = CUTSCENE_FARM_VISIT_KENT_COW_VISIT;
                }
                
                loadCutscene(morningVisit);
                set = TRUE;                               
            }
        }
        
        if (!set) {
            if (!checkLifeEventBit(KENT_CHICK_VISIT) && checkDailyEventBit(KENT_CHICK_HATCHED_VISIT_DAILY) && npcAffection[KENT] >= 5 && (5 < gHour && gHour < 9)) {
                setLifeEventBit(KENT_CHICK_VISIT);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_KENT_CHICK_VISIT;
                loadCutscene(morningVisit);
                set = TRUE;                               
            }
        }            
        
        if (!set) {
            if (!checkLifeEventBit(KENT_HORSE_VISIT) && checkDailyEventBit(KENT_ACQUIRED_HORSE_VISIT_DAILY) && npcAffection[KENT] >= 5 && (5 < gHour && gHour < 9)) {
                setLifeEventBit(KENT_HORSE_VISIT);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_KENT_HORSE_VISIT;
                loadCutscene(morningVisit);
                set = TRUE;                               
            }
        }     

        if (!set) {
            if (!checkLifeEventBit(KENT_HORSE_GROWN_VISIT) && checkDailyEventBit(KENT_HORSE_GROWN_VISIT_DAILY) && npcAffection[KENT] >= 5 && (5 < gHour && gHour < 9)) {
                setLifeEventBit(KENT_HORSE_GROWN_VISIT);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_KENT_HORSE_GROWN_VISIT;
                loadCutscene(morningVisit);
                set = TRUE;                               
            }
        }     

        if (!set) {
            
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gForecast == TYPHOON && ((npcAffection[HARVEST_SPRITE_1] + npcAffection[HARVEST_SPRITE_2] + npcAffection[HARVEST_SPRITE_3]) / 3) >= 120 && (5 < gHour && gHour < 9)) {

                if ((getTotalChickenCount() + getTotalFarmAnimalsCount()) >= 2) {           
                    setDailyEventBit(FARM_MORNING_VISIT);
                    gCutsceneIndex = CUTSCENE_FARM_VISIT_HARVEST_SPRITES_TYPHOON_VISIT;
                    loadCutscene(morningVisit);
                    set = TRUE;                               
                }
            }

        }    

        if (!set) {
            
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && ((npcAffection[CARPENTER_1] + npcAffection[CARPENTER_2] + npcAffection[MASTER_CARPENTER]) / 3) >= 120 && (5 < gHour && gHour < 9)) {

                if (checkDailyEventBit(EARTHQUAKE_CARPENTER_VISIT_DAILY)) {           
                    setDailyEventBit(FARM_MORNING_VISIT);
                    gCutsceneIndex = CUTSCENE_FARM_VISIT_CARPENTER_EARTHQUAKE_VISIT;
                    loadCutscene(morningVisit);
                    set = TRUE;                               
                }
            }

        }    

        if (!set) {
            if (!checkLifeEventBit(DOUG_FARM_FAVOR_OFFERED) && getTotalFarmAnimalsCount() < 6 && gYear == 1 && gSeason == AUTUMN && gDayOfMonth == 30 && (5 < gHour && gHour < 24)) {
                setLifeEventBit(DOUG_FARM_FAVOR_OFFERED);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_DOUG_FAVOR;
                loadCutscene(morningVisit);
                set = TRUE;    
            }
        }
        
        if (!set && morningVisit == FALSE) {
            if (!checkLifeEventBit(DOUG_DROPPING_OFF_COWS_FAVOR) && checkLifeEventBit(MRS_MANA_COWS_EVENT) && gSeason == WINTER && gDayOfMonth == 1 && (5 < gHour && gHour < 10) && gSpawnPointIndex == FARM_SPAWN_POINT_1) {
                setLifeEventBit(DOUG_DROPPING_OFF_COWS_FAVOR);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_DOUG_DROPPING_OFF_COWS;
                loadCutscene(morningVisit);
                set = TRUE;    
            }
        }
        
        if (!set && morningVisit == FALSE) {
            if (checkLifeEventBit(MRS_MANA_COWS_EVENT) && gSeason == WINTER && gDayOfMonth == 5 && (5 < gHour && gHour < 10) && gSpawnPointIndex == FARM_SPAWN_POINT_1) {
                clearLifeEventBit(MRS_MANA_COWS_EVENT);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_MISS_MANA_PICKUP_COWS;
                loadCutscene(morningVisit);
                set = TRUE;    
            }
        }      

        if (!set) {
            if (!checkLifeEventBit(KENT_FARM_GIFT_FLOWERS) && npcAffection[KENT] >= 50 && gSeason == SUMMER && (14 < gHour && gHour < 17)) {
                setLifeEventBit(KENT_FARM_GIFT_FLOWERS);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_KENT_FARM_GIFT_FLOWERS;
                loadCutscene(morningVisit);
                set = TRUE;    
            }
        }
        
        if (!set) {
            if (!checkLifeEventBit(PUPPIES) && (npcAffection[KENT] >= 150 || npcAffection[KENT] + npcAffection[STU] >= 250) && (5 < gHour && gHour < 18)) {
                setLifeEventBit(PUPPIES);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_KENT_AND_STU_PUPPIES;
                loadCutscene(morningVisit);
                set = TRUE;    
            }
        }
        
        if (!set) {
            if (!checkLifeEventBit(STU_MOONDROP_FLOWER_VISIT) && getFarmMoondropFlowerCount() >= 8 && gWeather == SUNNY && (gSeason && gSeason < 3) && (5 < gHour && gHour < 12)) {
                setLifeEventBit(STU_MOONDROP_FLOWER_VISIT);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_STU_MOONDROP_FLOWER_VISIT;
                loadCutscene(morningVisit);
                set = TRUE;    
            }
        }        
        
        if (!set) {
            if (!checkLifeEventBit(STU_PINK_CAT_MINT_VISIT) && getFarmPinkCatMintFlowersCount() && gWeather == SUNNY && (gSeason && gSeason < AUTUMN) && (5 < gHour && gHour < 12)) {
                setLifeEventBit(STU_PINK_CAT_MINT_VISIT);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_STU_PINK_CAT_MINT_VISIT;
                loadCutscene(morningVisit);
                set = TRUE;    
            }
        }        

        if (!set) {
            if (!checkLifeEventBit(STU_MARBLE_TRADE_VISIT) && getFarmPinkCatMintFlowersCount() >= 16 && npcAffection[STU] >= 80 && gWeather == SUNNY && (gSeason && gSeason < AUTUMN) && (5 < gHour && gHour < 12)) {
                setLifeEventBit(STU_MARBLE_TRADE_VISIT);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_STU_MARBLE_TRADE_VISIT;
                loadCutscene(morningVisit);
                set = TRUE;    
            }
        }      

        if (!set) {
            if (!checkLifeEventBit(MARIA_FARM_GIFT_1) && npcAffection[MARIA] >= 50 && (14 < gHour && gHour < 17) && (!checkLifeEventBit(MARRIED) || gWife != MARIA)) {
                setLifeEventBit(MARIA_FARM_GIFT_1);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_MARIA_GIFT_CHOCOLATE;
                loadCutscene(morningVisit);
                set = TRUE;    
            }
        }               
        
        if (!set) {
            if (!checkLifeEventBit(MARIA_FARM_GIFT_2) && npcAffection[MARIA] >= 80 && (14 < gHour && gHour < 17) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
                setLifeEventBit(MARIA_FARM_GIFT_2);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_MARIA_GIFT_RICE_BALLS;
                loadCutscene(morningVisit);
                set = TRUE;                               
            }
        }     

        if (!set) {
            if (!checkLifeEventBit(MARIA_FIREWORKS) && npcAffection[MARIA] >= 200 && gWeather == SUNNY && gSeason == SUMMER && NIGHTTIME && !checkDailyEventBit(FESTIVAL_DAY_CUTSCENE_GUARD) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
                setLifeEventBit(MARIA_FIREWORKS);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_MARIA_FIREWORKS;
                loadCutscene(morningVisit);
                set = TRUE;                               
            }
        }     
        
        if (!set) {
            if (!checkLifeEventBit(POPURI_FARM_GIFT_1) && npcAffection[POPURI] >= 50 && (14 < gHour && gHour < 17) && (!checkLifeEventBit(MARRIED) || gWife != POPURI)) {
                setLifeEventBit(POPURI_FARM_GIFT_1);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_POPURI_GIFT_TEA;
                loadCutscene(morningVisit);
                set = TRUE;                               
            }
        }          
        
        if (!set) {
            if (!checkLifeEventBit(POPURI_FARM_GIFT_2) && npcAffection[POPURI] >= 80 && (14 < gHour && gHour < 17) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
                setLifeEventBit(POPURI_FARM_GIFT_2);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_POPURI_GIFT_COOKIES;
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
                gCutsceneIndex = CUTSCENE_FARM_VISIT_POPURI_BLUE_MIST;
                loadCutscene(morningVisit);
                set = TRUE;           
                
            }
        } 

        if (!set) {
            if (!checkLifeEventBit(ELLI_FARM_GIFT_1) && npcAffection[ELLI] >= 50 && (14 < gHour && gHour < 17) && (!checkLifeEventBit(MARRIED) || gWife != ELLI)) {
                setLifeEventBit(ELLI_FARM_GIFT_1);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_ELLI_FARM_GIFT_1;
                loadCutscene(morningVisit);
                set = TRUE;                      
            }
        }
        
        if (!set) {
            if (!checkLifeEventBit(ELLI_FARM_GIFT_2) && npcAffection[ELLI] >= 80 && (14 < gHour && gHour < 17) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {
                setLifeEventBit(ELLI_FARM_GIFT_2);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_ELLI_FARM_GIFT_2;
                loadCutscene(morningVisit);
                set = TRUE;                      
            }
        }
        
        if (!set) {
            if (!checkLifeEventBit(ELLI_MOON_VIEWING) && npcAffection[ELLI] >= 200 && gWeather == SUNNY && gSeason == AUTUMN && gDayOfMonth == 9 && NIGHTTIME && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {
                setLifeEventBit(ELLI_MOON_VIEWING);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_ELLI_MOON_VIEWING;
                loadCutscene(morningVisit);
                set = TRUE;                      
            }
        }      

        if (!set) {
            if (!checkLifeEventBit(ELLI_THANKSGIVING_EGG_VISIT) && getChickenEggCount() >= 6 && gSeason == WINTER && gDayOfMonth == 9 && (5 < gHour && gHour < 24) && (!checkLifeEventBit(MARRIED) || gWife != ELLI)) {
                setLifeEventBit(ELLI_THANKSGIVING_EGG_VISIT);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_ELLI_THANKSGIVING_EGG_VISIT;
                loadCutscene(morningVisit);
                set = TRUE;                      
            }
        }  

        if (!set) {
            if (!checkLifeEventBit(ANN_FARM_GIFT_1) && npcAffection[ANN] >= 50 && (14 < gHour && gHour < 17) && (!checkLifeEventBit(MARRIED) || gWife != ANN)) {
                setLifeEventBit(ANN_FARM_GIFT_1);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_ANN_FARM_GIFT_1;
                loadCutscene(morningVisit);
                set = TRUE;                      
            }
        }  

        if (!set) {
            if (!checkLifeEventBit(ANN_FARM_GIFT_2) && npcAffection[ANN] >= 80 && (14 < gHour && gHour < 17) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
                setLifeEventBit(ANN_FARM_GIFT_2);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_ANN_FARM_GIFT_2;
                loadCutscene(morningVisit);
                set = TRUE;                      
            }
        }  
        
        if (!set) {
            if (!checkLifeEventBit(ANN_FIREWORKS) && npcAffection[ANN] >= 200 && gWeather == SUNNY && gSeason == WINTER && (5 < gHour && gHour < 18) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
                setLifeEventBit(ANN_FIREWORKS);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_ANN_FIREWORKS;
                loadCutscene(morningVisit);
                set = TRUE;                      
            }
        }  

        if (!set) {
            if (!checkLifeEventBit(ANN_FARM_VISIT_HORSE_GROWN) && npcAffection[ANN] >= 50 && (5 < gHour && gHour < 12) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && checkLifeEventBit(HORSE_GROWN)) {
                setLifeEventBit(ANN_FARM_VISIT_HORSE_GROWN);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_ANN_HORSE_GROWN_VISIT;
                loadCutscene(morningVisit);
                set = TRUE;                      
            }
        }  

        if (!set) {
            if (!checkLifeEventBit(ANN_HORSE_RACE_ELIGIBILITY_VISIT) && npcAffection[ANN] >= 80 && (5 < gHour && gHour < 12) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && checkLifeEventBit(HORSE_GROWN)) {
                setLifeEventBit(ANN_HORSE_RACE_ELIGIBILITY_VISIT);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_ANN_HORSE_RACE_ELIGIBILITY_VISIT;
                loadCutscene(morningVisit);
                set = TRUE;                      
            }
        }  

        if (!set) {
            if (!checkLifeEventBit(KAREN_FARM_GIFT_1) && npcAffection[KAREN] >= 60 && (1 < gSeason && gSeason < 4) && (14 < gHour && gHour < 17) && (!checkLifeEventBit(MARRIED) || gWife != KAREN)) {
                setLifeEventBit(KAREN_FARM_GIFT_1);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_KAREN_FARM_GIFT_1;
                loadCutscene(morningVisit);
                set = TRUE;                      
            }
        }  
        
        if (!set) {
            if (!checkLifeEventBit(KAREN_FARM_GIFT_2) && npcAffection[KAREN] >= 100 && (14 < gHour && gHour < 17) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
                setLifeEventBit(KAREN_FARM_GIFT_2);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_KAREN_FARM_GIFT_2;
                loadCutscene(morningVisit);
                set = TRUE;                      
            }
        }  
        
        if (!set) {
            if (!checkLifeEventBit(KAREN_KEIFU_FAIRIES_INVITE) && checkLifeEventBit(HARVEST_GODDESS_VINEYARD_BLESSING_DREAM) && npcAffection[KAREN] >= 200 && gWeather == SUNNY && gSeason == AUTUMN && gDayOfMonth == 7 && NIGHTTIME && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
                setLifeEventBit(KAREN_KEIFU_FAIRIES_INVITE);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_KAREN_KEIFU_FAIRIES_INVITE;
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
                        gCutsceneIndex = CUTSCENE_FARM_VISIT_KAREN_FIRST_VINEYARD_WINE_GIFT;
                    } else {
                        gCutsceneIndex = CUTSCENE_FARM_VISIT_KAI_FIRST_VINEYARD_WINE_GIFT;
                    }

                } else {

                    if (checkLifeEventBit(KAI_GONE)) {
                        gCutsceneIndex = CUTSCENE_FARM_VISIT_KAREN_FIRST_VINEYARD_WINE_GIFT;
                    } else {
                        gCutsceneIndex = CUTSCENE_FARM_VISIT_KAI_FIRST_VINEYARD_WINE_GIFT;
                    }      
                }

                loadCutscene(morningVisit);
                set = TRUE;      

            }

        }          

        if (!set) {
            if (!checkLifeEventBit(HARRIS_FARM_GIFT_MUSHROOMS) && npcAffection[HARRIS] >= 50 && gSeason == AUTUMN && (14 < gHour && gHour < 17)) {
                setLifeEventBit(HARRIS_FARM_GIFT_MUSHROOMS);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_HARRIS_FARM_GIFT_MUSHROOMS;
                loadCutscene(morningVisit);
                set = TRUE;                      
            }
        }       
        
        if (!set) {
            if (!checkLifeEventBit(CLIFF_FARM_GIFT_FISH) && npcAffection[CLIFF] >= 50 && (14 < gHour && gHour < 17)) {
                setLifeEventBit(CLIFF_FARM_GIFT_FISH);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_CLIFF_FARM_GIFT_FISH;
                loadCutscene(morningVisit);
                set = TRUE;                      
            }
        }       
        
        if (!set) {
            if (!checkLifeEventBit(KAI_CELEBRATION_WINE_VISIT) && checkLifeEventBit(KAREN_KAI_HAVE_BABY) && (5 < gHour && gHour < 18) && (!checkLifeEventBit(MARRIED) || gWife != KAREN) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
                setLifeEventBit(KAI_CELEBRATION_WINE_VISIT);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_KAI_CELEBRATION_WINE_VISIT;
                loadCutscene(morningVisit);
                set = TRUE;                      
            }
        }    

        if (!set && morningVisit == FALSE) {
            if (!checkLifeEventBit(RICK_TRANSPORTER_VISIT) && checkLifeEventBit(GIVE_RICK_RARE_METAL) && npcAffection[RICK] >= 120 && gWeather == SUNNY && (gSeason && gSeason < 4) && (5 < gHour && gHour < 18) && gSpawnPointIndex == FARM_SPAWN_POINT_1) {
                setLifeEventBit(RICK_TRANSPORTER_VISIT);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_RICK_TRANSPORTER_VISIT;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        }
        
        if (!set) {
            if (!checkLifeEventBit(SHADY_SALESMAN_VISIT_CHICKEN_FEED) && (5 < gHour && gHour < 12) && getTotalChickenCount() >= 5 && chickenFeedQuantity < 990) {
                setLifeEventBit(SHADY_SALESMAN_VISIT_CHICKEN_FEED);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_SHADY_SALESMAN_VISIT_CHICKEN_FEED;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        }

        if (!set) {
            if (!checkLifeEventBit(SHADY_SALESMAN_NECTAR_SEEDS_VISIT) && (gSeason && gSeason < 4) && (5 < gHour && gHour < 12) && gTotalCropsShipped >= 900 && chickenFeedQuantity < 990) {
                setLifeEventBit(SHADY_SALESMAN_NECTAR_SEEDS_VISIT);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_SHADY_SALESMAN_NECTAR_SEEDS_VISIT;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        }
        
        if (!set && morningVisit == FALSE) {
            if (!checkLifeEventBit(TOURIST_COUPLE_FLOWER_VISIT) && (getFarmMoondropFlowerCount() + getFarmPinkCatMintFlowersCount()) >= 24 && gWeather == SUNNY && gSeason == SPRING && (5 < gHour && gHour < 18) && gSpawnPointIndex == FARM_SPAWN_POINT_1) {
                setLifeEventBit(TOURIST_COUPLE_FLOWER_VISIT);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_TOURIST_COUPLE_FLOWER_VISIT;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        }

        if (!set && morningVisit == FALSE) {
            if (!checkLifeEventBit(TOURIST_COUPLE_GRASS_FIELDS_VISIT) && getFarmGrassTilesSum() >= 90 && gWeather == SUNNY && gSeason == AUTUMN && (5 < gHour && gHour < 18) && gSpawnPointIndex == FARM_SPAWN_POINT_1) {
                setLifeEventBit(TOURIST_COUPLE_GRASS_FIELDS_VISIT);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_TOURIST_COUPLE_GRASS_FIELDS_VISIT;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        }
        
        if (!set && morningVisit == FALSE) {
            if (!checkLifeEventBit(TOURIST_COUPLE_VINEYARD_VISIT) && checkLifeEventBit(VINEYARD_FIRST_WINE) && gWeather == SUNNY && gSeason == AUTUMN && (5 < gHour && gHour < 18)) {
                setLifeEventBit(TOURIST_COUPLE_VINEYARD_VISIT);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_TOURIST_COUPLE_VINEYARD_VISIT;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        }
        
        if (!set && morningVisit == FALSE) {
            if (!checkLifeEventBit(OLD_COUPLE_HORSE_RACE_WINNER_VISIT) && checkLifeEventBit(WON_HORSE_RACE) && gWeather == SUNNY && gSeason == SPRING && (5 < gHour && gHour < 18) && gSpawnPointIndex == FARM_SPAWN_POINT_1) {
                setLifeEventBit(OLD_COUPLE_HORSE_RACE_WINNER_VISIT);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_OLD_COUPLE_HORSE_RACE_WINNER_VISIT;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        }
        
        if (!set && morningVisit == FALSE) {
            if (!checkLifeEventBit(OLD_COUPLE_WINTER_FARM_MORNING_VISIT) && checkLifeEventBit(COW_FESTIVAL_FIRST_PRIZE) && gWeather == SUNNY && getTotalCowsCount() >= 3 && gSeason == WINTER && (5 < gHour && gHour < 18) && gSpawnPointIndex == FARM_SPAWN_POINT_1) {
                setLifeEventBit(OLD_COUPLE_WINTER_FARM_MORNING_VISIT);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_OLD_COUPLE_WINTER_FARM_MORNING_VISIT;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        }
        
        if (!set && morningVisit == FALSE) {
            // get cow + grass count
            if (!checkLifeEventBit(FESTIVAL_GIRLS_COW_FESTIVAL_WIN_VISIT) && checkLifeEventBit(COW_FESTIVAL_FIRST_PRIZE) && func_8009B828(1) && gWeather == SUNNY && gSeason == AUTUMN && (5 < gHour && gHour < 18) && gSpawnPointIndex == FARM_SPAWN_POINT_1) {
                setLifeEventBit(FESTIVAL_GIRLS_COW_FESTIVAL_WIN_VISIT);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_FESTIVAL_GIRLS_COW_FESTIVAL_WIN_VISIT;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        }

        if (!set && morningVisit == FALSE) {
            if (!checkLifeEventBit(FESTIVAL_GIRLS_FLOWER_FIELD_VISIT) && (getFarmMoondropFlowerCount() + getFarmPinkCatMintFlowersCount()) >= 24 && gWeather == SUNNY && gSeason == SUMMER && (5 < gHour && gHour < 18) && gSpawnPointIndex == FARM_SPAWN_POINT_1) {
                setLifeEventBit(FESTIVAL_GIRLS_FLOWER_FIELD_VISIT);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_FESTIVAL_GIRLS_FLOWER_FIELD_VISIT;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        }        

        if (!set) {
            if (checkLifeEventBit(HORSE_RACE_WON_THIS_YEAR) && npcAffection[DOUG] >= 60 && (16 < gHour && gHour < 21) && ((gSeason == SPRING && gDayOfMonth == 18) || gSeason == AUTUMN && gDayOfMonth == 29)) {
                clearLifeEventBit(HORSE_RACE_WON_THIS_YEAR);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_DOUG_HORSE_RACE_WIN_VISIT;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        }       
        
        if (!set) {
            if (checkLifeEventBit(COW_FESTIVAL_WON_THIS_YEAR) && npcAffection[DOUG] >= 60 && gSeason == AUTUMN && gDayOfMonth == 5 && (16 < gHour && gHour < 21)) {
                clearLifeEventBit(COW_FESTIVAL_WON_THIS_YEAR);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_DOUG_COW_FESTIVAL_WIN_VISIT;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        }          

        if (!set) {
            if (checkLifeEventBit(WON_VEGETABLE_FESTIVAL) && npcAffection[BASIL] >= 60 && gSeason == SUMMER && gDayOfMonth == 10 && (16 < gHour && gHour < 21)) {
                clearLifeEventBit(WON_VEGETABLE_FESTIVAL);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_BASIL_VEGETABLE_FESTIVAL_WIN_VISIT;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        }       
        
        if (!set) {
            if (!checkLifeEventBit(KAI_LEAVING) && checkLifeEventBit(KAREN_GONE) && npcAffection[KAI] <= 80 && gSeason == WINTER && (5 < gHour && gHour < 18) && gHarvestKing != 5 && gHarvestCoinFinder != 5 && (!checkLifeEventBit(MARRIED) || gWife != KAREN) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
                setLifeEventBit(KAI_LEAVING);
                setLifeEventBit(KAI_GONE);
                setSpecialDialogueBit(KAI_GONE_DIALOGUE);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_KAI_LEAVING;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        }      
        
        if (!set) {
            if (!checkDailyEventBit(SPIRIT_FESTIVAL_INVITATION_VISIT) && npcAffection[POTION_SHOP_DEALER] >= 50 && gWeather == SUNNY && gYear == 1 && gSeason == WINTER && gDayOfMonth == 27 && (5 < gHour && gHour < 9)) {
                setDailyEventBit(SPIRIT_FESTIVAL_INVITATION_VISIT);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_POTION_SHOP_DEALER_SPIRIT_FESTIVAL_INVITATION;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        } 

      if (!set) {
            if (!checkDailyEventBit(SPIRIT_FESTIVAL_INVITATION_VISIT) && npcAffection[HARRIS] >= 100 && gHarvestKing == 1 && gWeather == SUNNY && gSeason == WINTER && gDayOfMonth == 27 && (5 < gHour && gHour < 9)) {
                setDailyEventBit(SPIRIT_FESTIVAL_INVITATION_VISIT);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_HARRIS_SPIRIT_FESTIVAL_INVITATION;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        }    
        
      if (!set) {
            if (!checkDailyEventBit(SPIRIT_FESTIVAL_INVITATION_VISIT) && npcAffection[GRAY] >= 100 && gHarvestKing == 2 && gWeather == SUNNY && gSeason == WINTER && gDayOfMonth == 27 && (5 < gHour && gHour < 9)) {
                setDailyEventBit(SPIRIT_FESTIVAL_INVITATION_VISIT);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_GRAY_SPIRIT_FESTIVAL_INVITATION;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        }    

      if (!set) {
            if (!checkDailyEventBit(SPIRIT_FESTIVAL_INVITATION_VISIT) && npcAffection[JEFF] >= 100 && gHarvestKing == 3 && gWeather == SUNNY && gSeason == WINTER && gDayOfMonth == 27 && (5 < gHour && gHour < 9)) {
                setDailyEventBit(SPIRIT_FESTIVAL_INVITATION_VISIT);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_JEFF_SPIRIT_FESTIVAL_INVITATION;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        }    
        
    if (!set) {
        if (!checkDailyEventBit(SPIRIT_FESTIVAL_INVITATION_VISIT) && npcAffection[CLIFF] >= 100 && gHarvestKing == 4 && gWeather == SUNNY && gSeason == WINTER && gDayOfMonth == 27 && (5 < gHour && gHour < 9)) {
            setDailyEventBit(SPIRIT_FESTIVAL_INVITATION_VISIT);
            gCutsceneIndex = CUTSCENE_FARM_VISIT_CLIFF_SPIRIT_FESTIVAL_INVITATION;
            loadCutscene(morningVisit);
            set = TRUE;                             
        }
    }        
        
       if (!set) {
            if (!checkDailyEventBit(SPIRIT_FESTIVAL_INVITATION_VISIT) && npcAffection[KAI] >= 100 && gHarvestKing == 5 && gWeather == SUNNY && gSeason == WINTER && gDayOfMonth == 27 && (5 < gHour && gHour < 9)) {
                setDailyEventBit(SPIRIT_FESTIVAL_INVITATION_VISIT);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_KAI_SPIRIT_FESTIVAL_INVITATION;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        }      
            
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(MARIA_CUTSCENE_DAILY) && gSeason == SUMMER && gDayOfMonth == 1 && gWeather == SUNNY && (17 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == MARIA) {
                setDailyEventBit(MARIA_CUTSCENE_DAILY);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_MARIA_WIFE_SEA_FESTIVAL;
                loadCutscene(morningVisit);
                gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
                set = TRUE;                             
            }
        }
            
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(POPURI_CUTSCENE_DAILY) && gSeason == SUMMER && gDayOfMonth == 1 && gWeather == SUNNY && (17 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == POPURI) {
                setDailyEventBit(POPURI_CUTSCENE_DAILY);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_POPURI_WIFE_SEA_FESTIVAL;
                loadCutscene(morningVisit);
                gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
                set = TRUE;                             
            }
        }
            
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(ELLI_CUTSCENE_DAILY) && gSeason == SUMMER && gDayOfMonth == 1 && gWeather == SUNNY && (17 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == ELLI) {
                setDailyEventBit(ELLI_CUTSCENE_DAILY);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_ELLI_WIFE_SEA_FESTIVAL;
                loadCutscene(morningVisit);
                gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
                set = TRUE;                             
            }
        }
            
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(ANN_CUTSCENE_DAILY) && gSeason == SUMMER && gDayOfMonth == 1 && gWeather == SUNNY && (17 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == ANN) {
                setDailyEventBit(ANN_CUTSCENE_DAILY);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_ANN_WIFE_SEA_FESTIVAL;
                loadCutscene(morningVisit);
                gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
                set = TRUE;                             
            }
        }
            
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(KAREN_CUTSCENE_DAILY) && gSeason == SUMMER && gDayOfMonth == 1 && gWeather == SUNNY && (17 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == KAREN) {
                setDailyEventBit(KAREN_CUTSCENE_DAILY);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_KAREN_WIFE_SEA_FESTIVAL;
                loadCutscene(morningVisit);
                gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
                set = TRUE;                             
            }
        }
    
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(MARIA_CUTSCENE_DAILY) && gSeason == WINTER && gDayOfMonth == 24 && gWeather == SUNNY && (17 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == MARIA) {
                setDailyEventBit(MARIA_CUTSCENE_DAILY);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_MARIA_WIFE_STARRY_NIGHT;
                loadCutscene(morningVisit);
                gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
                set = TRUE;                             
            }
        }
    
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(POPURI_CUTSCENE_DAILY) && gSeason == WINTER && gDayOfMonth == 24 && gWeather == SUNNY && (17 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == POPURI) {
                setDailyEventBit(POPURI_CUTSCENE_DAILY);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_POPURI_WIFE_STARRY_NIGHT;
                loadCutscene(morningVisit);
                gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
                set = TRUE;                             
            }
        }
    
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(ELLI_CUTSCENE_DAILY) && gSeason == WINTER && gDayOfMonth == 24 && gWeather == SUNNY && (17 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == ELLI) {
                setDailyEventBit(ELLI_CUTSCENE_DAILY);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_ELLI_WIFE_STARRY_NIGHT;
                loadCutscene(morningVisit);
                gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
                set = TRUE;                             
            }
        }
    
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(ANN_CUTSCENE_DAILY) && gSeason == WINTER && gDayOfMonth == 24 && gWeather == SUNNY && (17 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == ANN) {
                setDailyEventBit(ANN_CUTSCENE_DAILY);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_ANN_WIFE_STARRY_NIGHT;
                loadCutscene(morningVisit);
                gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
                set = TRUE;                             
            }
        }
    
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(KAREN_CUTSCENE_DAILY) && gSeason == WINTER && gDayOfMonth == 24 && gWeather == SUNNY && (17 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == KAREN) {
                setDailyEventBit(KAREN_CUTSCENE_DAILY);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_KAREN_WIFE_STARRY_NIGHT;
                loadCutscene(morningVisit);
                gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
                set = TRUE;                             
            }
        }
    
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(MARIA_CUTSCENE_DAILY) && gSeason == SUMMER && gDayOfMonth == 7 && gWeather == SUNNY && (16 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == MARIA && npcAffection[MARIA] >= 220) {
                setDailyEventBit(MARIA_CUTSCENE_DAILY);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_MARIA_WIFE_FIREWORKS_FESTIVAL;
                loadCutscene(morningVisit);
                gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
                set = TRUE;                             
            }
        }
            
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(POPURI_CUTSCENE_DAILY) && gSeason == SUMMER && gDayOfMonth == 7 && gWeather == SUNNY && (16 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == POPURI && npcAffection[POPURI] >= 220) {
                setDailyEventBit(POPURI_CUTSCENE_DAILY);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_POPURI_WIFE_FIREWORKS_FESTIVAL;
                loadCutscene(morningVisit);
                gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
                set = TRUE;                             
            }
        }
    
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(ELLI_CUTSCENE_DAILY) && gSeason == SUMMER && gDayOfMonth == 7 && gWeather == SUNNY && (16 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == ELLI && npcAffection[ELLI] >= 220) {
                setDailyEventBit(ELLI_CUTSCENE_DAILY);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_ELLI_WIFE_FIREWORKS_FESTIVAL;
                loadCutscene(morningVisit);
                gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
                set = TRUE;                             
            }
        }
    
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(ANN_CUTSCENE_DAILY) && gSeason == SUMMER && gDayOfMonth == 7 && gWeather == SUNNY && (16 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == ANN && npcAffection[ANN] >= 220) {
                setDailyEventBit(ANN_CUTSCENE_DAILY);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_ANN_WIFE_FIREWORKS_FESTIVAL;
                loadCutscene(morningVisit);
                gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
                set = TRUE;                             
            }
        }
    
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(KAREN_CUTSCENE_DAILY) && gSeason == SUMMER && gDayOfMonth == 7 && gWeather == SUNNY && (16 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == KAREN && npcAffection[KAREN] >= 220) {
                setDailyEventBit(KAREN_CUTSCENE_DAILY);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_KAREN_WIFE_FIREWORKS_FESTIVAL;
                loadCutscene(morningVisit);
                gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
                set = TRUE;                             
            }
        }
            
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(MARIA_CUTSCENE_DAILY) && gSeason == AUTUMN && gDayOfMonth == 9 && gWeather == SUNNY && (16 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == MARIA && npcAffection[MARIA] >= 220) {
                setDailyEventBit(MARIA_CUTSCENE_DAILY);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_MARIA_WIFE_MOON_VIEWING;
                loadCutscene(morningVisit);
                gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
                set = TRUE;                             
            }
        }       
    
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(POPURI_CUTSCENE_DAILY) && gSeason == AUTUMN && gDayOfMonth == 9 && gWeather == SUNNY && (16 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == POPURI && npcAffection[POPURI] >= 220) {
                setDailyEventBit(POPURI_CUTSCENE_DAILY);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_POPURI_WIFE_MOON_VIEWING;
                loadCutscene(morningVisit);
                gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
                set = TRUE;                             
            }
        }      
    
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(ELLI_CUTSCENE_DAILY) && gSeason == AUTUMN && gDayOfMonth == 9 && gWeather == SUNNY && (16 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == ELLI && npcAffection[ELLI] >= 220) {
                setDailyEventBit(ELLI_CUTSCENE_DAILY);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_ELLI_WIFE_MOON_VIEWING;
                loadCutscene(morningVisit);
                gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
                set = TRUE;                             
            }
        }        
            
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(ANN_CUTSCENE_DAILY) && gSeason == AUTUMN && gDayOfMonth == 9 && gWeather == SUNNY && (16 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == ANN && npcAffection[ANN] >= 220) {
                setDailyEventBit(ANN_CUTSCENE_DAILY);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_ANN_WIFE_MOON_VIEWING;
                loadCutscene(morningVisit);
                gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
                set = TRUE;                             
            }
        }     
            
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(KAREN_CUTSCENE_DAILY) && gSeason == AUTUMN && gDayOfMonth == 9 && gWeather == SUNNY && (16 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == KAREN && npcAffection[KAREN] >= 220) {
                setDailyEventBit(KAREN_CUTSCENE_DAILY);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_KAREN_WIFE_MOON_VIEWING;
                loadCutscene(morningVisit);
                gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
                set = TRUE;                             
            }
        }       

    }

    return result;
    
}

u16 setFarmCutscenes(void) {

    u16 bytecodeSegmentIndex = 0xFFFF;
    bool set = FALSE;
    u8 tempHappiness;

    // shipper on farm during birth
    if (checkDailyEventBit(BIRTH_EVENT_DAILY)) { 
        gCutsceneIndex = CUTSCENE_FARM_VISIT_SHIPPER_BIRTH_EVENT_VISIT;
        gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
        loadCutscene(FALSE);
        set = TRUE;
    }
    
    if (!set) {
        if (checkDailyEventBit(PURCHASED_ANIMAL)) {
            bytecodeSegmentIndex = CUTSCENE_BANK_ANIMAL_DELIVERY;
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
            bytecodeSegmentIndex = CUTSCENE_BANK_ANIMAL_SALE;
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

            bytecodeSegmentIndex = CUTSCENE_BANK_ANIMAL_SOLD;
            set = TRUE;
            gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;

        }

    }

    if (!set) {
        if (checkDailyEventBit(COW_FESTIVAL_ELIGIBLE) && !checkDailyEventBit(COW_FESTIVAL_ENTRY_RESOLVED)) {
            bytecodeSegmentIndex = CUTSCENE_BANK_COW_FESTIVAL_ENTRY;
            set = TRUE;
            gCutsceneIndex = 1;
            gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
        }
    }

    if (!set) {
        if (checkDailyEventBit(COW_FESTIVAL_COW_TAKEN)) {
            bytecodeSegmentIndex = CUTSCENE_BANK_COW_FESTIVAL_TAKE_COW;
            set = TRUE;
            gCutsceneIndex = 1;
            gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
        }
    }

    if (!set) {
        if (checkDailyEventBit(HOUSE_EXTENSION_SELECTED)) {
            gCutsceneIndex = handlePurchaseHouseExtension(gHouseExtensionSelection);
            gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
            bytecodeSegmentIndex = CUTSCENE_BANK_HOUSE_EXTENSION_ESTIMATE;
            set = TRUE;
        }
    }

    if (!set) {
        if (checkLifeEventBit(HOUSE_EXTENSION_CONSTRUCTION) && checkDailyEventBit(HOUSE_EXTENSION_WORK) && !checkDailyEventBit(END_OF_DAILY_HOUSE_EXTENSION_WORK)) {
            gCutsceneIndex = 1;
            bytecodeSegmentIndex = CUTSCENE_BANK_HOUSE_EXTENSION_WORK;
            set = TRUE;
        }
    }

    if (!set) {
        if (checkDailyEventBit(CARPENTER_FINISHED)) {
            gCutsceneIndex = 0;
            bytecodeSegmentIndex = CUTSCENE_BANK_HOUSE_EXTENSION_COMPLETED;
            set = TRUE;
        }
    }

    if (!set) {
        setFarmVisitCutscenes(0);
    }
    
    return bytecodeSegmentIndex;

}

u16 setMountain1Cutscenes(void) {

    bool set = FALSE;
    u16 bytecodeSegmentIndex = 0xFFFF;

    if (!checkLifeEventBit(BRIDGE_COMPLETED) && gSeason == AUTUMN && (22 < gDayOfMonth && gDayOfMonth < 28) && (5 < gHour && gHour < 15)) {
        setDailyEventBit(MOUNTAIN_1_CUTSCENE_DAILY);
        if (checkDailyEventBit(MOUNTAIN_CARPENTER_HELP)) {
            gCutsceneIndex = CUTSCENE_MOUNTAIN_ASSISTANT_CARPENTER_GUARDS_PATH;
        } else {
            gCutsceneIndex = CUTSCENE_MOUNTAIN_MASTER_CARPENTER_BRIDGE_RECRUIT;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        }
        loadCutscene(FALSE);
        set = TRUE;
    }

    if (!set) {
        if (!checkLifeEventBit(HOT_SPRINGS_COMPLETED) && gSeason == WINTER && (11 < gDayOfMonth && gDayOfMonth < 17) && (5 < gHour && gHour < 15)) {
            setDailyEventBit(MOUNTAIN_1_CUTSCENE_DAILY);
            if (checkDailyEventBit(MOUNTAIN_CARPENTER_HELP)) {
                gCutsceneIndex = CUTSCENE_MOUNTAIN_ASSISTANT_CARPENTER_GUARDS_PATH;
            } else {
                gCutsceneIndex = CUTSCENE_MOUNTAIN_MASTER_CARPENTER_HOT_SPRINGS_RECRUIT;
                gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            }
            loadCutscene(FALSE);
            set = TRUE;  
        }
    }

    if (!set) {
        if (!checkHaveTool(FISHING_POLE)) {
            setDailyEventBit(MOUNTAIN_1_CUTSCENE_DAILY);
            gCutsceneIndex = CUTSCENE_MOUNTAIN_GREG_FISHING_POLE;
            loadCutscene(FALSE);
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkLifeEventBit(ELLI_JEFF_FISHING_CUTSCENE) && npcAffection[JEFF] >= 180 && gWeather == SUNNY && gSeason == SUMMER && (5 < gHour && gHour < 18) && (!checkLifeEventBit(MARRIED) || gWife != ELLI) && !checkLifeEventBit(ELLI_JEFF_MARRIED) && gSpawnPointIndex == TOP_OF_MOUNTAIN_1_SUMMER) {
            setDailyEventBit(MOUNTAIN_1_CUTSCENE_DAILY);
            setLifeEventBit(ELLI_JEFF_FISHING_CUTSCENE);
            gCutsceneIndex = CUTSCENE_MOUNTAIN_ELLI_JEFF_FISHING;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkLifeEventBit(MARIA_ELLI_FISHING_CUTSCENE) && npcAffection[MARIA] >= 150 && npcAffection[ELLI] >= 150 && gWeather == SUNNY && gSeason == SUMMER && (5 < gHour && gHour < 18) && gSpawnPointIndex == TOP_OF_MOUNTAIN_1_SUMMER) {
            setDailyEventBit(MOUNTAIN_1_CUTSCENE_DAILY);
            setLifeEventBit(MARIA_ELLI_FISHING_CUTSCENE);
            gCutsceneIndex = CUTSCENE_MOUNTAIN_MARIA_ELLI_FISHING;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkLifeEventBit(POPURI_KAREN_MOUNTAIN_CUTSCENE) && npcAffection[POPURI] >= 150 && npcAffection[KAREN] >= 150 && gWeather == SUNNY && gSeason == SPRING && (5 < gHour && gHour < 18) && gSpawnPointIndex == MOUNTAIN_1_SPAWN_POINT_1) {
            setDailyEventBit(MOUNTAIN_1_CUTSCENE_DAILY);
            setLifeEventBit(POPURI_KAREN_MOUNTAIN_CUTSCENE);
            gCutsceneIndex = CUTSCENE_MOUNTAIN_POPURI_KAREN_MOONDROP_WINE;
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
            gCutsceneIndex = CUTSCENE_MOUNTAIN_ELLI_SPRAINED_ANKLE;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
        }
    } 
    
    return bytecodeSegmentIndex;

}

u16 setMountain2Cutscenes(void) {

    bool set = FALSE;
    u16 bytecodeSegmentIndex = 0xFFFF;

    if (!checkLifeEventBit(POPURI_MOUNTAIN_HELP) && checkLifeEventBit(POPURI_DREAM_CUTSCENE) && checkLifeEventBit(BRIDGE_COMPLETED) && gWeather == SUNNY && 5 < gHour && gHour < 18 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
        setLifeEventBit(POPURI_MOUNTAIN_HELP);
        gCutsceneIndex = CUTSCENE_MOUNTAIN_POPURI_MOUNTAIN_HELP;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
        set = TRUE;
    }

    if (!set && !checkLifeEventBit(POPURI_SPRAINED_ANKLE) && !checkDailyEventBit(FESTIVAL_DAY_CUTSCENE_GUARD) && npcAffection[POPURI] >= 180 && gWeather == SUNNY && 5 < gHour && gHour < 18 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
        setLifeEventBit(POPURI_SPRAINED_ANKLE);
        setDailyEventBit(POPURI_LOCATION_OVERRIDE);
        gCutsceneIndex = CUTSCENE_MOUNTAIN_POPURI_SPRAINED_ANKLE;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
        set = TRUE;
    }
    
    if (!set && !checkLifeEventBit(ANN_BABY_CHICK_MOUNTAIN) && checkLifeEventBit(ANN_DREAM_CUTSCENE) && gWeather == SUNNY && gSeason != WINTER && 5 < gHour && gHour < 18 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && gSpawnPointIndex == MOUNTAIN_2_SPAWN_POINT_1) { 
        setLifeEventBit(ANN_BABY_CHICK_MOUNTAIN);
        gCutsceneIndex = CUTSCENE_MOUNTAIN_ANN_BABY_CHICK;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
        set = TRUE;
    }

    if (!set && !checkLifeEventBit(ANN_GRAY_MOUNTAIN_CUTSCENE) && npcAffection[ANN] >= 80 && gWeather == SUNNY && gSeason == AUTUMN && 5 < gHour && gHour < 18) {
        setLifeEventBit(ANN_GRAY_MOUNTAIN_CUTSCENE);
        gCutsceneIndex = CUTSCENE_MOUNTAIN_ANN_GRAY;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
        set = TRUE;
    }

    if (!set && !checkLifeEventBit(CLIFF_ANN_KANE_MOUNTAIN_CUTSCENE) && npcAffection[CLIFF] >= 150 && gWeather == SUNNY && gSeason != WINTER && 5 < gHour && gHour < 18 && (!checkLifeEventBit(MARRIED) || gWife != ANN) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && gSpawnPointIndex == MOUNTAIN_2_SPAWN_POINT_1) {
        setLifeEventBit(CLIFF_ANN_KANE_MOUNTAIN_CUTSCENE);
        gCutsceneIndex = CUTSCENE_MOUNTAIN_CLIFF_ANN_KANE;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
        set = TRUE;
    }

    if (!set && !checkLifeEventBit(MARIA_KAREN_GRASSHOPPER_CUTSCENE) && npcAffection[MARIA] >= 150 && npcAffection[KAREN] >= 150 && gWeather == SUNNY && gSeason == SPRING && 5 < gHour && gHour < 18 && gSpawnPointIndex == MOUNTAIN_2_SPAWN_POINT_1) {
        setLifeEventBit(MARIA_KAREN_GRASSHOPPER_CUTSCENE);
        gCutsceneIndex = CUTSCENE_MOUNTAIN_MARIA_KAREN_GRASSHOPPER;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
        set = TRUE;
    }
    
    if (!set && !checkLifeEventBit(STU_MAY_BUTTERFLY_CUTSCENE) && npcAffection[STU] >= 50 && npcAffection[MAY] >= 50 && gWeather == SUNNY && gSeason < AUTUMN && gDayOfWeek == SATURDAY && 5 < gHour && gHour < 18) {
        setLifeEventBit(STU_MAY_BUTTERFLY_CUTSCENE);
        gCutsceneIndex = CUTSCENE_MOUNTAIN_STU_MAY_BUTTERFLY;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
        set = TRUE;
    }

    if (!set && !checkLifeEventBit(POPURI_CONFESSION) && !checkLifeEventBit(MARRIED) && npcAffection[POPURI] >= 250 && gWeather == SUNNY && 11 < gHour && gHour < 18 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
        setLifeEventBit(POPURI_CONFESSION);
        gCutsceneIndex = CUTSCENE_MOUNTAIN_POPURI_CONFESSION;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
        set = TRUE;
    }

    return bytecodeSegmentIndex;
    
}

u16 setTopOfMountain1Cutscenes(void) {

    bool set = FALSE;
    u16 bytecodeSegmentIndex = 0xFFFF;

    if (!checkLifeEventBit(POPURI_ELLI_HOT_SPRINGS_CHAT) && checkLifeEventBit(HOT_SPRINGS_COMPLETED) && npcAffection[POPURI] >= 150 && npcAffection[JEFF] >= 120 && gWeather == SUNNY && gSeason >= AUTUMN && (5 < gHour && gHour < 18) && (!checkLifeEventBit(MARRIED) || gWife != POPURI) && (!checkLifeEventBit(MARRIED) || gWife != ELLI) && !checkLifeEventBit(POPURI_GRAY_MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {
        setLifeEventBit(POPURI_ELLI_HOT_SPRINGS_CHAT);
        gCutsceneIndex = CUTSCENE_MOUNTAIN_POPURI_ELLI_HOT_SPRINGS_CHAT;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
        set = TRUE;
    }

    if (!set) {
        if (!checkLifeEventBit(TOP_OF_MOUNTAIN_AUTUMN_HOT_SPRINGS_CUTSCENE) && checkLifeEventBit(HOT_SPRINGS_COMPLETED) && gWeather == SUNNY && gSeason == AUTUMN && (5 < gHour && gHour < 18)) {
            setLifeEventBit(TOP_OF_MOUNTAIN_AUTUMN_HOT_SPRINGS_CUTSCENE);
            gCutsceneIndex = CUTSCENE_MOUNTAIN_AUTUMN_HOT_SPRINGS;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
            set = TRUE;
        }
    }

    if (!set) {
        // anonymous characters in hot springs, but logic implies it's the carpenters relaxing after getting rich 
        if (!checkLifeEventBit(TOP_OF_MOUNTAIN_WINTER_HOT_SPRINGS_CUTSCENE) && gWeather == SUNNY && gSeason == WINTER && (5 < gHour && gHour < 18) && checkLifeEventBit(HAVE_KITCHEN) && checkLifeEventBit(HAVE_BATHROOM) && checkLifeEventBit(HAVE_STAIRS) && checkLifeEventBit(HAVE_GREENHOUSE) && checkLifeEventBit(HAVE_LOG_TERRACE) && checkLifeEventBit(HAVE_BABY_BED) && checkLifeEventBit(HOT_SPRINGS_COMPLETED)) {
            setLifeEventBit(TOP_OF_MOUNTAIN_WINTER_HOT_SPRINGS_CUTSCENE);
            gCutsceneIndex = CUTSCENE_MOUNTAIN_WINTER_HOT_SPRINGS;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
        }
    }
    
    return bytecodeSegmentIndex;
    
}

u16 setMoonMountainCutscenes(void) {

    bool set = FALSE;
    u16 bytecodeSegmentIndex = 0xFFFF;

    if (gSeason == SUMMER && gDayOfMonth == 1 && NIGHTTIME && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
        gCutsceneIndex = CUTSCENE_MOUNTAIN_FIREWORKS;
        loadCutscene(FALSE);
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        set = TRUE;
    }
    
    if (!set) {
        if (gSeason == WINTER && gDayOfMonth == 24 && NIGHTTIME && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
            gCutsceneIndex = CUTSCENE_MOUNTAIN_STARRY_NIGHT_MOON;
            loadCutscene(FALSE);
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        }
    }
    
    return bytecodeSegmentIndex;

}

u16 setPondCutscenes(void) {

    bool set = FALSE;
    u16 bytecodeSegmentIndex = 0xFFFF;

    if (!checkLifeEventBit(BLUE_MIST_COMPLETE) && blueMistFlowerPlot >= 0xC2 && gSeason < AUTUMN && (5 < gHour && gHour < 18)) {
        setLifeEventBit(BLUE_MIST_COMPLETE);
        setDailyEventBit(HARVEST_GODDESS_INTERACTION);
        gCutsceneIndex = CUTSCENE_MOUNTAIN_POPURI_BLUE_MIST;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
        set = TRUE;
    }

    if (!set) {
        if (!checkLifeEventBit(ENTOMOLOGIST_BLUE_MIST_CUTSCENE) && checkLifeEventBit(BLUE_MIST_COMPLETE) && blueMistFlowerPlot >= 0xC2 && gSeason < AUTUMN && (5 < gHour && gHour < 18)) {
            setLifeEventBit(ENTOMOLOGIST_BLUE_MIST_CUTSCENE);
            setDailyEventBit(HARVEST_GODDESS_INTERACTION);
            gCutsceneIndex = CUTSCENE_MOUNTAIN_ENTOMOLOGIST_BLUE_MIST;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
            set = TRUE;
        }
    }

    if (!set) {
         if (!checkLifeEventBit(ENTOMOLOGIST_EMERALD_BUTTERFLY_CUTSCENE) && checkLifeEventBit(ENTOMOLOGIST_BLUE_MIST_CUTSCENE) && blueMistFlowerPlot >= 0xC2 && gSeason < AUTUMN && (5 < gHour && gHour < 18)) {
            setLifeEventBit(ENTOMOLOGIST_EMERALD_BUTTERFLY_CUTSCENE);
            setDailyEventBit(HARVEST_GODDESS_INTERACTION);
            gCutsceneIndex = CUTSCENE_MOUNTAIN_ENTOMOLOGIST_EMERALD_BUTTERFLY;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
         }
    }
    
    return bytecodeSegmentIndex;
}

u16 setHarvestSpriteCaveCutscenes(void) {

    u16 bytecodeSegmentIndex = 0xFFFF;
    
    if (checkHaveKeyItem(MARBLES) && npcAffection[HARVEST_SPRITE_1] >= 80 && (5 < gHour && gHour < 18)) {
        gCutsceneIndex = CUTSCENE_MOUNTAIN_HARVEST_SPRITES_POWER_NUT;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
    }

    return bytecodeSegmentIndex;

}

u16 setCaveCutscenes(void) {

    u16 bytecodeSegmentIndex = 0xFFFF;
    if (gSeason == WINTER && (7 < gDayOfMonth && gDayOfMonth < 31) && (5 < gHour && gHour < 16)) {
        gCutsceneIndex = CUTSCENE_MOUNTAIN_MINE_2_DIG_PROMPT;
        gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
        loadCutscene(FALSE);
    }

    return bytecodeSegmentIndex;

}

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
                gCutsceneIndex = CUTSCENE_HOUSE_MARIA_BIRTH;
                clearSpecialDialogueBit(WIFE_PREGNANT_MARIA_DIALOGUE);
                break;
            case POPURI:
                gCutsceneIndex = CUTSCENE_HOUSE_POPURI_BIRTH;
                clearSpecialDialogueBit(WIFE_PREGNANT_POPURI_DIALOGUE);
                break;
            case ELLI:
                gCutsceneIndex = CUTSCENE_HOUSE_ELLI_BIRTH;
                clearSpecialDialogueBit(WIFE_PREGNANT_ELLI_DIALOGUE);
                break;
            case ANN:
                gCutsceneIndex = CUTSCENE_HOUSE_ANN_BIRTH;
                clearSpecialDialogueBit(WIFE_PREGNANT_ANN_DIALOGUE);
                break;
            case KAREN:
                gCutsceneIndex = CUTSCENE_HOUSE_KAREN_BIRTH;
                clearSpecialDialogueBit(WIFE_PREGNANT_KAREN_DIALOGUE);
                break;
            
        }
        
        loadCutscene(FALSE);
        set = TRUE;
        gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
        
    }

    if (!set) {
        if (checkLifeEventBit(WENT_TO_BED_EARLY) && !checkLifeEventBit(KENT_HOUSE_VISIT_CUTSCENE) && !checkLifeEventBit(MARRIED) && npcAffection[KENT] >= 160 && !checkLifeEventBit(MARRIED)) {
            gCutsceneIndex = CUTSCENE_HOUSE_KENT_HOUSE_VISIT_CUTSCENE;
            loadCutscene(FALSE);
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkLifeEventBit(CLIFF_HOUSE_VISIT_CUTSCENE) && npcAffection[CLIFF] >= 30 && gWeather == SUNNY && (5 < gHour && gHour < 18) && (!checkLifeEventBit(MARRIED) || gWife != ANN) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
            gCutsceneIndex = CUTSCENE_HOUSE_CLIFF_HOUSE_VISIT_CUTSCENE;
            loadCutscene(FALSE);
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkDailyEventBit(FARM_MORNING_VISIT) && !checkLifeEventBit(WIFE_LEFT) && (5 < gHour && gHour < 18) && checkLifeEventBit(MARRIED) && gWife == MARIA && npcAffection[MARIA] < 100) {
            setDailyEventBit(FARM_MORNING_VISIT);
            setLifeEventBit(WIFE_LEFT);
            gCutsceneIndex = CUTSCENE_HOUSE_MARIA_LEAVES_MARRIAGE;
            loadCutscene(FALSE);
            gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkDailyEventBit(FARM_MORNING_VISIT) && !checkLifeEventBit(WIFE_LEFT) && (5 < gHour && gHour < 18) && checkLifeEventBit(MARRIED) && gWife == POPURI && npcAffection[POPURI] < 100) {
            setDailyEventBit(FARM_MORNING_VISIT);
            setLifeEventBit(WIFE_LEFT);
            gCutsceneIndex = CUTSCENE_HOUSE_POPURI_LEAVES_MARRIAGE;
            loadCutscene(FALSE);
            gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkDailyEventBit(FARM_MORNING_VISIT) && !checkLifeEventBit(WIFE_LEFT) && (5 < gHour && gHour < 18) && checkLifeEventBit(MARRIED) && gWife == ELLI && npcAffection[ELLI] < 100) {
            setDailyEventBit(FARM_MORNING_VISIT);
            setLifeEventBit(WIFE_LEFT);
            gCutsceneIndex = CUTSCENE_HOUSE_ELLI_LEAVES_MARRIAGE;
            loadCutscene(FALSE);
            gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkDailyEventBit(FARM_MORNING_VISIT) && !checkLifeEventBit(WIFE_LEFT) && (5 < gHour && gHour < 18) && checkLifeEventBit(MARRIED) && gWife == ANN && npcAffection[ANN] < 100) {
            setDailyEventBit(FARM_MORNING_VISIT);
            setLifeEventBit(WIFE_LEFT);
            gCutsceneIndex = CUTSCENE_HOUSE_ANN_LEAVES_MARRIAGE;
            loadCutscene(FALSE);
            gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkDailyEventBit(FARM_MORNING_VISIT) && !checkLifeEventBit(WIFE_LEFT) && (5 < gHour && gHour < 18) && checkLifeEventBit(MARRIED) && gWife == KAREN && npcAffection[KAREN] < 100) {
            setDailyEventBit(FARM_MORNING_VISIT);
            setLifeEventBit(WIFE_LEFT);
            gCutsceneIndex = CUTSCENE_HOUSE_KAREN_LEAVES_MARRIAGE;
            loadCutscene(FALSE);
            gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
            set = TRUE;
        }
    }
    
    return bytecodeSegmentIndex;
    
}

u16 setKitchenCutscenes(void) {

    u16 bytecodeSegmentIndex = 0xFFFF;
    bool set = FALSE;

    if (!checkDailyEventBit(MARIA_CUTSCENE_DAILY) && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && checkLifeEventBit(HAVE_BABY) && !checkDailyEventBit(BIRTH_EVENT_DAILY) && gWife == MARIA && babyBirthdaySeason == gSeason && babyBirthdayDate == gDayOfMonth) {
        setDailyEventBit(MARIA_CUTSCENE_DAILY);
        gCutsceneIndex = CUTSCENE_HOUSE_MARIA_BABY_BIRTHDAY;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
        
    }

    if (!set && !checkDailyEventBit(POPURI_CUTSCENE_DAILY) && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && checkLifeEventBit(HAVE_BABY) && !checkDailyEventBit(BIRTH_EVENT_DAILY) && gWife == POPURI && babyBirthdaySeason == gSeason && babyBirthdayDate == gDayOfMonth) {
        setDailyEventBit(POPURI_CUTSCENE_DAILY);
        gCutsceneIndex = CUTSCENE_HOUSE_POPURI_BABY_BIRTHDAY;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
    }

    if (!set && !checkDailyEventBit(ELLI_CUTSCENE_DAILY) && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && checkLifeEventBit(HAVE_BABY) && !checkDailyEventBit(BIRTH_EVENT_DAILY) && gWife == ELLI && babyBirthdaySeason == gSeason && babyBirthdayDate == gDayOfMonth) {
        setDailyEventBit(ELLI_CUTSCENE_DAILY);
        gCutsceneIndex = CUTSCENE_HOUSE_ELLI_BABY_BIRTHDAY;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
    }

    if (!set && !checkDailyEventBit(ANN_CUTSCENE_DAILY) && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && checkLifeEventBit(HAVE_BABY) && !checkDailyEventBit(BIRTH_EVENT_DAILY) && gWife == ANN && babyBirthdaySeason == gSeason && babyBirthdayDate == gDayOfMonth) {
        setDailyEventBit(ANN_CUTSCENE_DAILY);
        gCutsceneIndex = CUTSCENE_HOUSE_ANN_BABY_BIRTHDAY;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
    }

    if (!set && !checkDailyEventBit(KAREN_CUTSCENE_DAILY) && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && checkLifeEventBit(HAVE_BABY) && !checkDailyEventBit(BIRTH_EVENT_DAILY) && gWife == KAREN && babyBirthdaySeason == gSeason && babyBirthdayDate == gDayOfMonth) {
        setDailyEventBit(KAREN_CUTSCENE_DAILY);
        gCutsceneIndex = CUTSCENE_HOUSE_KAREN_BABY_BIRTHDAY;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
    }

    if (!set && !checkDailyEventBit(MARIA_CUTSCENE_DAILY) && gPlayerBirthdaySeason == gSeason && gDayOfMonth == 25 && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && gWife == MARIA && npcAffection[MARIA] >= 100) {
        setDailyEventBit(MARIA_CUTSCENE_DAILY);
        gCutsceneIndex = CUTSCENE_HOUSE_MARIA_MARRIED_PLAYER_BIRTHDAY;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
    }

    if (!set && !checkDailyEventBit(POPURI_CUTSCENE_DAILY) && gPlayerBirthdaySeason == gSeason && gDayOfMonth == 25 && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && gWife == POPURI && npcAffection[POPURI] >= 100) {
        setDailyEventBit(POPURI_CUTSCENE_DAILY);
        gCutsceneIndex = CUTSCENE_HOUSE_POPURI_MARRIED_PLAYER_BIRTHDAY;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
    }

    if (!set && !checkDailyEventBit(ELLI_CUTSCENE_DAILY) && gPlayerBirthdaySeason == gSeason && gDayOfMonth == 25 && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && gWife == ELLI && npcAffection[ELLI] >= 100) {
        setDailyEventBit(ELLI_CUTSCENE_DAILY);
        gCutsceneIndex = CUTSCENE_HOUSE_ELLI_MARRIED_PLAYER_BIRTHDAY;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
    }

    if (!set && !checkDailyEventBit(ANN_CUTSCENE_DAILY) && gPlayerBirthdaySeason == gSeason && gDayOfMonth == 25 && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && gWife == ANN && npcAffection[ANN] >= 100) {
        setDailyEventBit(ANN_CUTSCENE_DAILY);
        gCutsceneIndex = CUTSCENE_HOUSE_ANN_MARRIED_PLAYER_BIRTHDAY;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
    }

    if (!set && !checkDailyEventBit(KAREN_CUTSCENE_DAILY) && gPlayerBirthdaySeason == gSeason && gDayOfMonth == 25 && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && gWife == KAREN && npcAffection[KAREN] >= 100) {
        setDailyEventBit(KAREN_CUTSCENE_DAILY);
        gCutsceneIndex = CUTSCENE_HOUSE_KAREN_MARRIED_PLAYER_BIRTHDAY;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
    }

    if (!set && !checkDailyEventBit(MARIA_CUTSCENE_DAILY) && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && gWife == MARIA && anniversarySeason == gSeason && anniversaryDate == gDayOfMonth) {
        setDailyEventBit(MARIA_CUTSCENE_DAILY);
        gCutsceneIndex = CUTSCENE_HOUSE_MARIA_ANNIVERSARY;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
    }

    if (!set && !checkDailyEventBit(POPURI_CUTSCENE_DAILY) && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && gWife == POPURI && anniversarySeason == gSeason && anniversaryDate == gDayOfMonth) {
        setDailyEventBit(POPURI_CUTSCENE_DAILY);
        gCutsceneIndex = CUTSCENE_HOUSE_POPURI_ANNIVERSARY;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
    }

    if (!set && !checkDailyEventBit(ELLI_CUTSCENE_DAILY) && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && gWife == ELLI && anniversarySeason == gSeason && anniversaryDate == gDayOfMonth) {
        setDailyEventBit(ELLI_CUTSCENE_DAILY);
        gCutsceneIndex = CUTSCENE_HOUSE_ELLI_ANNIVERSARY;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
    }

    if (!set && !checkDailyEventBit(ANN_CUTSCENE_DAILY) && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && gWife == ANN && anniversarySeason == gSeason && anniversaryDate == gDayOfMonth) {
        setDailyEventBit(ANN_CUTSCENE_DAILY);
        gCutsceneIndex = CUTSCENE_HOUSE_ANN_ANNIVERSARY;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
    }

    if (!set && !checkDailyEventBit(KAREN_CUTSCENE_DAILY) && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && gWife == KAREN && anniversarySeason == gSeason && anniversaryDate == gDayOfMonth) {
        setDailyEventBit(KAREN_CUTSCENE_DAILY);
        gCutsceneIndex = CUTSCENE_HOUSE_KAREN_ANNIVERSARY;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
    }
    
    if (!set && !checkDailyEventBit(MARIA_CUTSCENE_DAILY) && gSeason == WINTER && gDayOfMonth == 10 && (16 < gHour && gHour < 20) && checkLifeEventBit(MARRIED) && gWife == MARIA) {
        setDailyEventBit(MARIA_CUTSCENE_DAILY);
        gCutsceneIndex = CUTSCENE_HOUSE_MARIA_MARRIED_THANKSGIVING;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
    }
        
    if (!set && !checkDailyEventBit(POPURI_CUTSCENE_DAILY) && gSeason == WINTER && gDayOfMonth == 10 && (16 < gHour && gHour < 20) && checkLifeEventBit(MARRIED) && gWife == POPURI) {
        setDailyEventBit(POPURI_CUTSCENE_DAILY);
        gCutsceneIndex = CUTSCENE_HOUSE_POPURI_MARRIED_THANKSGIVING;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
    }

    if (!set && !checkDailyEventBit(ELLI_CUTSCENE_DAILY) && gSeason == WINTER && gDayOfMonth == 10 && (16 < gHour && gHour < 20) && checkLifeEventBit(MARRIED) && gWife == ELLI) {
        setDailyEventBit(ELLI_CUTSCENE_DAILY);
        gCutsceneIndex = CUTSCENE_HOUSE_ELLI_MARRIED_THANKSGIVING;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
    }

    if (!set && !checkDailyEventBit(ANN_CUTSCENE_DAILY) && gSeason == WINTER && gDayOfMonth == 10 && (16 < gHour && gHour < 20) && checkLifeEventBit(MARRIED) && gWife == ANN) {
        setDailyEventBit(ANN_CUTSCENE_DAILY);
        gCutsceneIndex = CUTSCENE_HOUSE_ANN_MARRIED_THANKSGIVING;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
    }

    if (!set && !checkDailyEventBit(KAREN_CUTSCENE_DAILY) && gSeason == WINTER && gDayOfMonth == 10 && (16 < gHour && gHour < 20) && checkLifeEventBit(MARRIED) && gWife == KAREN) {
        setDailyEventBit(KAREN_CUTSCENE_DAILY);
        gCutsceneIndex = CUTSCENE_HOUSE_KAREN_MARRIED_THANKSGIVING;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
    }
    
    return bytecodeSegmentIndex;
    
}

u16 setRanchCutscenes(void) {

    u16 bytecodeSegmentIndex = 0xFFFF;
    bool set = FALSE;

    if (gYear == 1 && gSeason == SPRING && gDayOfMonth >= 3 && gWeather == SUNNY && 5 < gHour && gHour < 17 && !checkLifeEventBit(HAVE_HORSE)) {

        if (!checkLifeEventBit(RANCH_GET_HORSE_CUTSCENE)) {
            gCutsceneIndex = CUTSCENE_RANCH_GET_HORSE;
        } else {
            gCutsceneIndex = CUTSCENE_RANCH_DOUG_HORSE_OFFER_ALT;
        }

        loadCutscene(FALSE);
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        set = TRUE;
        
    }

    if (!set && gSeason == SUMMER && gDayOfMonth == 1 && 17 < gHour && gHour < 24 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {

        gCutsceneIndex = CUTSCENE_RANCH_ANN_FIREWORKS;
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
        
        gCutsceneIndex = CUTSCENE_COW_FESTIVAL;
        loadCutscene(FALSE);
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        set = TRUE;
    
    }

    if (!set && !checkDailyEventBit(ANN_CUTSCENE_DAILY) && gSeason == WINTER && gDayOfMonth == 10 && 5 < gHour && gHour < 18 && !checkLifeEventBit(MARRIED)) {
        setDailyEventBit(ANN_CUTSCENE_DAILY);
        gCutsceneIndex = CUTSCENE_RANCH_ANN_BIRTHDAY;
        loadCutscene(FALSE);
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        set = TRUE;

    }
    
    if (!set && !checkLifeEventBit(POPURI_ANN_RANCH_FLOWERS) && npcAffection[GRAY] >= 150 && gWeather == SUNNY && gSeason < AUTUMN && 5 < gHour && gHour < 17 && (!checkLifeEventBit(MARRIED) || gWife != POPURI) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {

        if (gSpawnPointIndex == RANCH_SPAWN_POINT_1) {
            gCutsceneIndex = CUTSCENE_RANCH_POPURI_ANN_FLOWERS_1;
            setLifeEventBit(POPURI_ANN_RANCH_FLOWERS);
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
            set = TRUE;
        }
        
        if (gSpawnPointIndex == RANCH_SPAWN_POINT_2) {
            gCutsceneIndex = CUTSCENE_RANCH_POPURI_ANN_FLOWERS_2;
            setLifeEventBit(POPURI_ANN_RANCH_FLOWERS);
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
            set = TRUE;
        }
        
    }

    if (!set && !checkLifeEventBit(POPURI_GRAY_RANCH_EVENING) && npcAffection[GRAY] >= 180 && gWeather == SUNNY && gSeason < WINTER && 17 < gHour && gHour < 24 && (!checkLifeEventBit(MARRIED) || gWife != POPURI) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {

        if (gSpawnPointIndex == RANCH_SPAWN_POINT_1) {
            gCutsceneIndex = CUTSCENE_RANCH_POPURI_GRAY_EVENING_1;
            setLifeEventBit(POPURI_GRAY_RANCH_EVENING);
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
            set = TRUE;
        }
        
        if (gSpawnPointIndex == RANCH_SPAWN_POINT_2) {
            gCutsceneIndex = CUTSCENE_RANCH_POPURI_GRAY_EVENING_2;
            setLifeEventBit(POPURI_GRAY_RANCH_EVENING);
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
            set = TRUE;
        }
        
    }

    if (!set && !checkLifeEventBit(ANN_POPURI_TALK_ABOUT_GRAY) && npcAffection[POPURI] >= 150 && npcAffection[ANN] >= 150 && gWeather == SUNNY && gSeason == AUTUMN && 5 < gHour && gHour < 18 && (!checkLifeEventBit(MARRIED) || gWife != POPURI) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {

        if (gSpawnPointIndex == RANCH_SPAWN_POINT_1) {
            gCutsceneIndex = CUTSCENE_RANCH_ANN_POPURI_TALK_GRAY_1;
            setLifeEventBit(ANN_POPURI_TALK_ABOUT_GRAY);
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
            set = TRUE;
        }
        
        if (gSpawnPointIndex == RANCH_SPAWN_POINT_2) {
            gCutsceneIndex = CUTSCENE_RANCH_ANN_POPURI_TALK_GRAY_2;
            setLifeEventBit(ANN_POPURI_TALK_ABOUT_GRAY);
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
            set = TRUE;
        }
        
    }
    
    if (!set && !checkLifeEventBit(ANN_CLIFF_RANCH_CUTSCENE) && npcAffection[CLIFF] >= 100 && gWeather == SUNNY && gSeason < WINTER && 5 < gHour && gHour < 18 && (!checkLifeEventBit(MARRIED) || gWife != ANN) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {

        setLifeEventBit(ANN_CLIFF_RANCH_CUTSCENE);
        gCutsceneIndex = CUTSCENE_RANCH_ANN_CLIFF_MEETING;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
        set = TRUE;

    }

    if (!set && !checkLifeEventBit(ANN_CONFESSION) && !checkLifeEventBit(MARRIED) && npcAffection[ANN] >= 250 && gWeather == SUNNY && 14 < gHour && gHour < 24 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && gSpawnPointIndex == RANCH_SPAWN_POINT_1) {

        setLifeEventBit(ANN_CONFESSION);
        gCutsceneIndex = CUTSCENE_RANCH_ANN_CONFESSION;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
        set = TRUE;

    }

    return bytecodeSegmentIndex;
    
}

u16 setRanchStoreCutscenes(void) {

    u16 bytecodeSegmentIndex = 0xFFFF;
     
    if (!(checkLifeEventBit(ANN_SICK_VISIT)) && !checkDailyEventBit(FESTIVAL_DAY_CUTSCENE_GUARD) && npcAffection[ANN] >= 120 && (2 <= gWeather && gWeather < 4) && gDayOfWeek == SUNDAY && (5 < gHour && gHour < 18) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
        setLifeEventBit(ANN_SICK_VISIT);
        setDailyEventBit(ANN_LOCATION_OVERRIDE);
        gCutsceneIndex = CUTSCENE_RANCH_ANN_SICK_VISIT_TRANSITION;
        loadCutscene(FALSE);
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
    }
     
    if (checkLifeEventBit(MARRIED) && gWife == ANN && checkLifeEventBit(WIFE_LEFT)) {
        gCutsceneIndex = CUTSCENE_RANCH_ANN_REPAIR_MARRIAGE_TRANSITION;
        loadCutscene(FALSE);
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
    }

    return bytecodeSegmentIndex;

}

u16 setVineyardCutscenes(void) {

    u16 bytecodeSegmentIndex = 0xFFFF;
    bool set = FALSE;

    if (!checkLifeEventBit(KAREN_VINEYARD_INTRO) && (5 < gHour && gHour < 18) && (!checkLifeEventBit(MARRIED) || gWife != 4) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
        setLifeEventBit(KAREN_VINEYARD_INTRO);
        gCutsceneIndex = CUTSCENE_VINEYARD_KAREN_GOTZ_QUARREL;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
        set = TRUE;
    }

    if (!set) {
        if (!checkLifeEventBit(KAREN_SPRAINED_ANKLE) && !checkDailyEventBit(FESTIVAL_DAY_CUTSCENE_GUARD) && npcAffection[KAREN] >= 180 && gWeather == SUNNY && (5 < gHour && gHour < 18) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            setLifeEventBit(KAREN_SPRAINED_ANKLE);
            setDailyEventBit(KAREN_LOCATION_OVERRIDE);
            gCutsceneIndex = CUTSCENE_VINEYARD_KAREN_SPRAINED_ANKLE;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkLifeEventBit(KAREN_ANN_TALK_ABOUT_CLIFF) && npcAffection[ANN] >= 120 && npcAffection[KAREN] >= 150 && gWeather == SUNNY && gSeason == WINTER && (5 < gHour && gHour < 18) && (!checkLifeEventBit(MARRIED) || gWife != KAREN) && !checkLifeEventBit(KAREN_KAI_MARRIED) && previousMapIndex == ROAD) {
            setLifeEventBit(KAREN_ANN_TALK_ABOUT_CLIFF);
            gCutsceneIndex = CUTSCENE_VINEYARD_KAREN_ANN_CLIFF_TALK;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkLifeEventBit(KAI_KAREN_CUTSCENE) && npcAffection[KAI] >= 50 && npcAffection[KAREN] >= 50 && (5 < gHour && gHour < 18) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED) && previousMapIndex == ROAD) {
            setLifeEventBit(KAI_KAREN_CUTSCENE);
            gCutsceneIndex = CUTSCENE_VINEYARD_KAI_KAREN_RECONCILE;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
        }
    }

    return bytecodeSegmentIndex;
    
}

u16 setVineyardHouseCutscenes(void) {
    
    u16 bytecodeSegmentIndex = 0xFFFF;
    
    if (!checkLifeEventBit(KAREN_SICK_VISIT) && !checkDailyEventBit(FESTIVAL_DAY_CUTSCENE_GUARD) && npcAffection[KAREN] >= 120U && (2 <= gWeather && gWeather < 4) && (5 < gHour && gHour < 18) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
        setLifeEventBit(KAREN_SICK_VISIT);
        setDailyEventBit(KAREN_LOCATION_OVERRIDE);
        gCutsceneIndex = CUTSCENE_VINEYARD_KAREN_SICK_TRANSITION;
        loadCutscene(FALSE);
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
    }
    
    if (checkLifeEventBit(MARRIED) && gWife == KAREN && checkLifeEventBit(WIFE_LEFT)) {
        gCutsceneIndex = CUTSCENE_VINEYARD_KAREN_REPAIR_MARRIAGE_TRANSITION;
        loadCutscene(FALSE);
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
    }
    
    return bytecodeSegmentIndex;

}

u16 setVineyardCellarCutscenes(void) {
    
    bool set;
    u16 bytecodeSegmentIndex = 0xFFFF;

    set = FALSE;
    
    if (!checkLifeEventBit(KAREN_YELLOW_HEART_EVENT_CUTSCENE) && checkLifeEventBit(KAREN_DREAM_CUTSCENE) && (5 < gHour && gHour < 18) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED) && previousMapIndex == VINEYARD) {
               
        setLifeEventBit(KAREN_YELLOW_HEART_EVENT_CUTSCENE);
        setDailyEventBit(KAREN_YELLOW_HEART_EVENT_DAILY);

        gCutsceneIndex = CUTSCENE_VINEYARD_KAREN_CELLAR_OFFER;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);

        loadCutscene(FALSE);

        set = TRUE;

    }
    
    if (!set && checkDailyEventBit(KAREN_YELLOW_HEART_EVENT_DAILY) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED) && previousMapIndex == VINEYARD_CELLAR_BASEMENT) {
        gCutsceneIndex = CUTSCENE_VINEYARD_CELLAR_HIDDEN_REENTRY;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
    }
    
    return bytecodeSegmentIndex;
}

u16 setVillage1Cutscenes(void) {

    u16 bytecodeSegmentIndex = 0xFFFF;
    bool set = FALSE;

    if (!checkLifeEventBit(BLUE_MIST_CYCLE_ACTIVE) && checkLifeEventBit(BOUGHT_BLUE_MIST_SEEDS) && checkHaveTool(BLUE_MIST_SEEDS)) {
        setLifeEventBit(BLUE_MIST_CYCLE_ACTIVE);
        gCutsceneIndex = CUTSCENE_VILLAGE1_POPURI_BLUE_MIST_SEEDS;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
        set = TRUE;
    } 

    if (!set && gSeason == SUMMER && gDayOfMonth == 9 && 5 < gHour && gHour < 18) {
        gCutsceneIndex = CUTSCENE_VEGETABLE_FESTIVAL_VILLAGE;
        loadCutscene(FALSE);
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        set = TRUE;
    } 
    
    if (!set && gSeason == AUTUMN && gDayOfMonth == 20 && 5 < gHour && gHour < 18) {
        gCutsceneIndex = CUTSCENE_EGG_FESTIVAL;
        loadCutscene(FALSE);
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        set = TRUE;
    } 

    if (!set && !checkDailyEventBit(POPURI_CUTSCENE_DAILY) && gSeason == WINTER && gDayOfMonth == 10 && 5 < gHour && gHour < 18 && !checkLifeEventBit(MARRIED)) {
        setDailyEventBit(POPURI_CUTSCENE_DAILY);
        gCutsceneIndex = CUTSCENE_VILLAGE1_POPURI_CAKE_FOR_DAD;
        loadCutscene(FALSE);
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        set = TRUE;
    } 

    if (!set && !checkLifeEventBit(MARIA_POPURI_VILLAGE_CUTSCENE) && npcAffection[MARIA] >= 150 && npcAffection[POPURI] >= 150 && gWeather == SUNNY && gSeason == SPRING && 5 < gHour && gHour < 18) {

        if (previousMapIndex == ROAD) {
            
            gCutsceneIndex = CUTSCENE_VILLAGE1_ELLI_COOKIES;
            setLifeEventBit(MARIA_POPURI_VILLAGE_CUTSCENE);
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
            set = TRUE;
            
        }
        
        if (previousMapIndex == VILLAGE_2) {

            if (gSpawnPointIndex == VILLAGE_1_SPAWN_POINT_2) {
                gCutsceneIndex = CUTSCENE_VILLAGE1_ELLI_COOKIES_VARIANT;
            } else {
                gCutsceneIndex = CUTSCENE_VILLAGE1_ELLI_COOKIES_VARIANT2;
            }

            setLifeEventBit(MARIA_POPURI_VILLAGE_CUTSCENE);
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
            set = TRUE;
            
        }
        
    } 
    
    if (!set && !checkLifeEventBit(GRAY_VILLAGE1_CUTSCENE) && npcAffection[GRAY] >= 120 && gWeather == SUNNY && gSeason < AUTUMN && 5 < gHour && gHour < 18 && (!checkLifeEventBit(MARRIED) || gWife != POPURI) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
        setLifeEventBit(GRAY_VILLAGE1_CUTSCENE);
        gCutsceneIndex = CUTSCENE_VILLAGE1_GRAY_VISIT;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
        set = TRUE;
    } 

    if (!set && !checkLifeEventBit(ELLEN_DIED) && npcAffection[ELLEN] >= 30 && gWeather == SUNNY && gYear == 2 && gSeason == SPRING && 5 < gHour && gHour < 18) {
        gCutsceneIndex = CUTSCENE_VILLAGE1_ELLEN_DIES;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
        set = TRUE;
    } 
    
    return bytecodeSegmentIndex;
    
}

// village 2
u16 setVillage2Cutscenes(void) {

    u16 bytecodeSegmentIndex = 0xFFFF;
    bool set = FALSE;

    if (!checkLifeEventBit(MAYOR_SHADY_SALESMAN_CUTSCENE) && (npcAffection[MAYOR] + npcAffection[MAYOR_WIFE] >= 200) && npcAffection[MARIA] >= 150 && (5 < gHour && gHour < 18)) {

        if (previousMapIndex == ROAD) {

            setLifeEventBit(MAYOR_SHADY_SALESMAN_CUTSCENE);
            gCutsceneIndex = CUTSCENE_VILLAGE2_MAYOR_SHADY_SALESMAN_1;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
            set = TRUE;
            
        }

        if (previousMapIndex == VILLAGE_1) {

            if (gSpawnPointIndex == VILLAGE_2_SPAWN_POINT_2) {
                gCutsceneIndex = CUTSCENE_VILLAGE2_MAYOR_SHADY_SALESMAN_2;
            } else {
                gCutsceneIndex = CUTSCENE_VILLAGE2_MAYOR_SHADY_SALESMAN_3;
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
            gCutsceneIndex = CUTSCENE_VILLAGE2_MARIA_HARRIS_1;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
            set = TRUE;
            
        }

        if (previousMapIndex == VILLAGE_1) {

            if (gSpawnPointIndex == VILLAGE_2_SPAWN_POINT_2) {
                gCutsceneIndex = CUTSCENE_VILLAGE2_MARIA_HARRIS_2;
            } else {
                gCutsceneIndex = CUTSCENE_VILLAGE2_MARIA_HARRIS_3;
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
            gCutsceneIndex = CUTSCENE_VILLAGE2_SHADY_SALESMAN_MARIA_1;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
            set = TRUE;
            
        }

        if (previousMapIndex == VILLAGE_1) {

            if (gSpawnPointIndex == VILLAGE_2_SPAWN_POINT_2) {
                gCutsceneIndex = CUTSCENE_VILLAGE2_SHADY_SALESMAN_MARIA_2;
            } else {
                gCutsceneIndex = CUTSCENE_VILLAGE2_SHADY_SALESMAN_MARIA_3;
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
            gCutsceneIndex = CUTSCENE_VILLAGE2_MARIA_ANN_1;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
            set = TRUE;
            
        }

        if (previousMapIndex == VILLAGE_1) {

            if (gSpawnPointIndex == VILLAGE_2_SPAWN_POINT_2) {
                gCutsceneIndex = CUTSCENE_VILLAGE2_MARIA_ANN_2;
            } else {
                gCutsceneIndex = CUTSCENE_VILLAGE2_MARIA_ANN_3;
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
            gCutsceneIndex = CUTSCENE_VILLAGE2_ELLI_PLAYING_HOUSE_1;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
            set = TRUE;
            
        }

        if (previousMapIndex == VILLAGE_1) {

            if (gSpawnPointIndex == VILLAGE_2_SPAWN_POINT_2) {
                gCutsceneIndex = CUTSCENE_VILLAGE2_ELLI_PLAYING_HOUSE_2;
            } else {
                gCutsceneIndex = CUTSCENE_VILLAGE2_ELLI_PLAYING_HOUSE_3;
            }

            setLifeEventBit(ELLI_PLAYING_HOUSE_CUTSCENE);
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
            set = TRUE;
            
        }
    
    }

    if (!set && !checkLifeEventBit(MARIA_PREGNANT_MIDWIFE_CUTSCENE) && checkLifeEventBit(WIFE_PREGNANT) && gWife == MARIA && npcAffection[MIDWIFE] >= 80 && gWifePregnancyCounter == 9 && gWeather == SUNNY && 5 < gHour && gHour < 18) {

        setLifeEventBit(MARIA_PREGNANT_MIDWIFE_CUTSCENE);
        gCutsceneIndex = CUTSCENE_VILLAGE2_MARIA_PREGNANT_MIDWIFE;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
        set = TRUE;
        
    }

    if (!set && !checkLifeEventBit(POPURI_PREGNANCY_MIDWIFE_CUTSCENE) && checkLifeEventBit(WIFE_PREGNANT) && gWife == POPURI && npcAffection[MIDWIFE] >= 80 && gWifePregnancyCounter == 9 && gWeather == SUNNY && 5 < gHour && gHour < 18) {

        setLifeEventBit(POPURI_PREGNANCY_MIDWIFE_CUTSCENE);
        gCutsceneIndex = CUTSCENE_VILLAGE2_POPURI_PREGNANT_MIDWIFE;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
        set = TRUE;
        
    }

    if (!set && !checkLifeEventBit(ELLI_PREGNANCY_MIDWIFE_CUTSCENE) && checkLifeEventBit(WIFE_PREGNANT) && gWife == ELLI && npcAffection[MIDWIFE] >= 80 && gWifePregnancyCounter == 9 && gWeather == SUNNY && 5 < gHour && gHour < 18) {

        setLifeEventBit(ELLI_PREGNANCY_MIDWIFE_CUTSCENE);
        gCutsceneIndex = CUTSCENE_VILLAGE2_ELLI_PREGNANT_MIDWIFE;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
        set = TRUE;
        
    }

    if (!set && !checkLifeEventBit(ANN_PREGNANCY_MIDWIFE_CUTSCENE) && checkLifeEventBit(WIFE_PREGNANT) && gWife == ANN && npcAffection[MIDWIFE] >= 80 && gWifePregnancyCounter == 9 && gWeather == SUNNY && 5 < gHour && gHour < 18) {

        setLifeEventBit(ANN_PREGNANCY_MIDWIFE_CUTSCENE);
        gCutsceneIndex = CUTSCENE_VILLAGE2_ANN_PREGNANT_MIDWIFE;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
        set = TRUE;
        
    }

    if (!set && !checkLifeEventBit(KAREN_PREGNANCY_MIDWIFE_CUTSCENE) && checkLifeEventBit(WIFE_PREGNANT) && gWife == KAREN && npcAffection[MIDWIFE] >= 80 && gWifePregnancyCounter == 9 && gWeather == SUNNY && 5 < gHour && gHour < 18) {

        setLifeEventBit(KAREN_PREGNANCY_MIDWIFE_CUTSCENE);
        gCutsceneIndex = CUTSCENE_VILLAGE2_KAREN_PREGNANT_MIDWIFE;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
        set = TRUE;
        
    }

    if (!set && !checkLifeEventBit(MARIA_CONFESSION) && !checkLifeEventBit(MARRIED) && npcAffection[MARIA] >= 250 && gWeather == SUNNY && 11 < gHour && gHour < 18 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED) && previousMapIndex == LIBRARY) {
        setLifeEventBit(MARIA_CONFESSION);
        gCutsceneIndex = CUTSCENE_VILLAGE2_MARIA_CONFESSION;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
    } 
    
    return bytecodeSegmentIndex;
    
}

u16 setRickStoreCutscenes(void) {
    
    u16 bytecodeSegmentIndex = 0xFFFF;
    
    if (!checkLifeEventBit(ANN_RICK_CUTSCENE) && npcAffection[RICK] >= 40 && (5 < gHour && gHour < 18)) {
        setLifeEventBit(ANN_RICK_CUTSCENE);
        gCutsceneIndex = CUTSCENE_VILLAGE1_ANN_AND_RICK;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
    }
    
    return bytecodeSegmentIndex;

}

u16 setFlowerShopCutscenes(void) {
    
    u16 bytecodeSegmentIndex = 0xFFFF;
    
    if (!checkLifeEventBit(POPURI_SICK_CUTSCENE) && !checkDailyEventBit(FESTIVAL_DAY_CUTSCENE_GUARD) && npcAffection[POPURI] >= 120 && (2 <= gWeather && gWeather < 4) && (5 < gHour && gHour < 18) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
        setLifeEventBit(POPURI_SICK_CUTSCENE);
        setDailyEventBit(POPURI_LOCATION_OVERRIDE);
        gCutsceneIndex = CUTSCENE_VILLAGE1_POPURI_SICK_TRANSITION;
        loadCutscene(FALSE);
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
    }
    
    if (checkLifeEventBit(MARRIED) && gWife == POPURI && checkLifeEventBit(WIFE_LEFT)) {
        gCutsceneIndex = CUTSCENE_VILLAGE1_POPURI_REPAIR_MARRIAGE_LILLIA_ENTRY;
        loadCutscene(FALSE);
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
    }
    
    return bytecodeSegmentIndex;

}

u16 setBakeryCutscenes(void) {

    bool set = FALSE;
    u16 bytecodeSegmentIndex = 0xFFFF;

    if (gSeason == SUMMER && gDayOfMonth == 1 && 17 < gHour && gHour < 24 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {

        if (!checkLifeEventBit(ELLEN_DIED)) {
            gCutsceneIndex = CUTSCENE_VILLAGE1_ELLI_SPARKLERS;
        } else {
            gCutsceneIndex = CUTSCENE_VILLAGE1_JEFF_BAKERY;
        }

        loadCutscene(FALSE);
        set = TRUE;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        
    }

    if (!set && !checkDailyEventBit(ELLI_CUTSCENE_DAILY) && gSeason == WINTER && gDayOfMonth == 10 && 5 < gHour && gHour < 18 && !checkLifeEventBit(MARRIED)) {
        setDailyEventBit(ELLI_CUTSCENE_DAILY);
        gCutsceneIndex = CUTSCENE_VILLAGE1_ELLI_WALK;
        loadCutscene(FALSE);
        set = TRUE;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
    }

    if (!set && !checkLifeEventBit(ELLI_ANN_BAKING) && npcAffection[ELLI] >= 150 && npcAffection[ANN] >= 150 && 5 < gHour && gHour < 18 && (gSeason == AUTUMN && gDayOfMonth >= 15 || gSeason == WINTER && gDayOfMonth < 10)) {

        setLifeEventBit(ELLI_ANN_BAKING);
        gCutsceneIndex = CUTSCENE_VILLAGE1_JEFF_BAKERY_VISIT;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
        set = TRUE;
        
    }

    if (!set && !checkLifeEventBit(ELLI_POPURI_BAKERY) && !checkDailyEventBit(FESTIVAL_DAY_CUTSCENE_GUARD) && npcAffection[JEFF] >= 150 && 5 < gHour && gHour < 18  && (!checkLifeEventBit(MARRIED) || gWife != ELLI) && !checkLifeEventBit(ELLI_JEFF_MARRIED) && previousMapIndex == VILLAGE_1) {

        setLifeEventBit(ELLI_POPURI_BAKERY);
        
        if (gSpawnPointIndex == BAKERY_SPAWN_POINT_1) {
            gCutsceneIndex = CUTSCENE_VILLAGE1_POPURI_PIE_CAKE_TEA;
        } else {
            gCutsceneIndex = CUTSCENE_VILLAGE1_POPURI_PIE_CAKE_TEA_VARIANT;
        }
        
        loadCutscene(FALSE);
        set = TRUE;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        
    }
    
    if (!set && !checkLifeEventBit(ELLI_SICK_VISIT) && !checkDailyEventBit(FESTIVAL_DAY_CUTSCENE_GUARD) && npcAffection[ELLI] >= 120 && (SUNNY < gWeather && gWeather < 4) && (5 < gHour && gHour < 18)  && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {

        setLifeEventBit(ELLI_SICK_VISIT);
        setDailyEventBit(ELLI_LOCATION_OVERRIDE);
        gCutsceneIndex = CUTSCENE_VILLAGE1_ELLI_SICK_JEFF_ENTRY;
        loadCutscene(FALSE);
        set = TRUE;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        
    }

    if (!set && !checkLifeEventBit(ELLI_CONFESSION) && !checkLifeEventBit(MARRIED) && npcAffection[ELLI] >= 250 && gWeather == SUNNY && 11 < gHour && gHour < 17 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {

        setLifeEventBit(ELLI_CONFESSION);
        gCutsceneIndex = CUTSCENE_VILLAGE1_ELLI_CONFESSION_1;
        set = TRUE;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
        
    }

    if (checkLifeEventBit(MARRIED) && gWife == ELLI && checkLifeEventBit(WIFE_LEFT)) {
        gCutsceneIndex = CUTSCENE_VILLAGE1_ELLI_REPAIR_MARRIAGE_JEFF_ENTRY;
        loadCutscene(FALSE);
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
    }

    return bytecodeSegmentIndex;
    
}

u16 setMayorHouseCutscenes(void) {

    u16 bytecodeSegmentIndex = 0xFFFF;
    bool set = FALSE;

    if (!checkDailyEventBit(MARIA_CUTSCENE_DAILY) && gSeason == WINTER && gDayOfMonth == 10 && (5 < gHour && gHour < 18) && !checkLifeEventBit(MARRIED)) {
        setDailyEventBit(MARIA_CUTSCENE_DAILY);
        gCutsceneIndex = CUTSCENE_VILLAGE2_THANKSGIVING_MAYOR_HOUSE;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
    }

    if (!set && !checkLifeEventBit(MARIA_SICK_CUTSCENE) && !checkDailyEventBit(FESTIVAL_DAY_CUTSCENE_GUARD) && npcAffection[MARIA] >= 120 && (SUNNY < gWeather && gWeather < 4) && gDayOfWeek == MONDAY && (8 < gHour && gHour < 17) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
        setLifeEventBit(MARIA_SICK_CUTSCENE);
        setDailyEventBit(MARIA_LOCATION_OVERRIDE);
        gCutsceneIndex = CUTSCENE_VILLAGE2_MARIA_SICK_TRANSITION;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
    }

    if (checkLifeEventBit(MARRIED) && gWife == MARIA && checkLifeEventBit(WIFE_LEFT)) {
        gCutsceneIndex = CUTSCENE_VILLAGE2_MARIA_REPAIR_MARRIAGE_TRANSITION;
        loadCutscene(FALSE);
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
    }

    return bytecodeSegmentIndex;

}    

u16 setLibraryCutscenes(void) {

    u16 bytecodeSegmentIndex = 0xFFFF;
    bool set = FALSE;

    if (!checkLifeEventBit(MARIA_SPRAINED_ANKLE_CUTSCENE) && !checkDailyEventBit(FESTIVAL_DAY_CUTSCENE_GUARD) && npcAffection[MARIA] >= 180 && (8 < gHour && gHour < 17) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
        setLifeEventBit(MARIA_SPRAINED_ANKLE_CUTSCENE);
        setDailyEventBit(MARIA_LOCATION_OVERRIDE);
        gCutsceneIndex = CUTSCENE_VILLAGE2_MARIA_SPRAINED_ANKLE;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
    }

    if (!set) {
        if (!checkLifeEventBit(MARIA_CHILDRENS_BOOK) && checkLifeEventBit(MARIA_DREAM_CUTSCENE) && (8 < gHour && gHour < 17) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
            setLifeEventBit(MARIA_CHILDRENS_BOOK);
            gCutsceneIndex = CUTSCENE_VILLAGE2_MARIA_CHILDRENS_BOOK;
            loadCutscene(FALSE);
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        }
    }
    
    return bytecodeSegmentIndex;
    
}

u16 setChurchCutscenes(void) {

    bool set = FALSE;
    u16 bytecodeSegmentIndex = 0xFFFF;

    if (gSeason == WINTER && gDayOfMonth == 24 && NIGHTTIME && !checkLifeEventBit(MARRIED)) {     
        gCutsceneIndex = CUTSCENE_VILLAGE1_STARRY_NIGHT_FESTIVAL;
        loadCutscene(FALSE);
        set = TRUE;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
    } 
    
    if (!set) {
        if (gSeason == WINTER && gDayOfMonth == 30 && NIGHTTIME) {
            gCutsceneIndex = CUTSCENE_VILLAGE1_CHURCH_NEW_YEARS;
            loadCutscene(FALSE);
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        }    
    } 
    
    return bytecodeSegmentIndex;
}

u16 setTavernCutscenes(void) {

    u16 bytecodeSegmentIndex = 0xFFFF;
    bool set = FALSE;

    if (gSeason == WINTER && gDayOfMonth == 30 && NIGHTTIME) {
        gCutsceneIndex = CUTSCENE_VILLAGE1_TAVERN_NEW_YEARS;
        loadCutscene(FALSE);
        set = TRUE;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
    }

    if (!set && !checkLifeEventBit(KAREN_KAI_TAVERN_CUTSCENE) && npcAffection[KAREN] >= 80 && npcAffection[KAI] >= 80 && NIGHTTIME && (!checkLifeEventBit(MARRIED) || gWife != KAREN) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
        setLifeEventBit(KAREN_KAI_TAVERN_CUTSCENE);
        setSpecialDialogueBit(KAREN_KAI_TAVERN_CUTSCENE_DIALOGUE);
        gCutsceneIndex = CUTSCENE_VILLAGE1_KAREN_KAI_TAVERN;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
    }

    return bytecodeSegmentIndex;
    
}

u16 setSquareCutscenes(void) {

    u16 bytecodeSegmentIndex = 0xFFFF;
    bool set = FALSE;

    if (gSeason == SPRING && gDayOfMonth == 8 && 5 < gHour && gHour < 18) {

        if (gYear == 1) {
            gCutsceneIndex = CUTSCENE_SOWING_FESTIVAL_POTION_SHOP_DEALER;
        } else {
            if (gHarvestKing == PLAYER) {
                gCutsceneIndex = CUTSCENE_SOWING_FESTIVAL_PLAYER;
            } else {
                gCutsceneIndex = CUTSCENE_SOWING_FESTIVAL_BACHELOR;
            }
        }
        
        loadCutscene(FALSE);
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        set = TRUE;
        
    }

    if (!set && gSeason == SPRING && gDayOfMonth == 23 && 5 < gHour && gHour < 18) {

        gCutsceneIndex = CUTSCENE_FLOWER_FESTIVAL;
        loadCutscene(FALSE);
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        set = TRUE;
        
    }

    if (!set && gSeason == SUMMER && gDayOfMonth == 1 && 17 < gHour && gHour < 24 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {

        gCutsceneIndex = CUTSCENE_FIREWORKS_FESTIVAL;
        loadCutscene(FALSE);
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        set = TRUE;
        
    }

    if (!set && gSeason == AUTUMN && gDayOfMonth == 12 && 5 < gHour && gHour < 18) {

        setDailyEventBit(HARVEST_FESTIVAL_DAILY);
        setHarvestKingName(gHarvestCoinFinder);
        
        gCutsceneIndex = CUTSCENE_HARVEST_FESTIVAL;
        loadCutscene(FALSE);
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        set = TRUE;
        
    }

    if (!set && gSeason == WINTER && gDayOfMonth == 27 && 17 < gHour && gHour < 24) {

        if (gHarvestKing == PLAYER) {
            gCutsceneIndex = CUTSCENE_SPIRIT_FESTIVAL_HARVEST_KING;
        } else {
            if (!checkDailyEventBit(SPIRIT_FESTIVAL_INVITATION_ACCEPTED)) {
                gCutsceneIndex = CUTSCENE_SPIRIT_FESTIVAL;
            } else {
                gCutsceneIndex = CUTSCENE_SPIRIT_FESTIVAL_REPEAT;
            }
        }
    
        loadCutscene(FALSE);
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        set = TRUE;
        
    }

    if (!set && gSeason == SPRING && gDayOfMonth == 1 && 5 < gHour && gHour < 18) {
        gCutsceneIndex = CUTSCENE_NEW_YEAR_FESTIVAL;
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
        gCutsceneIndex = CUTSCENE_DOG_RACE_SQUARE;
        loadCutscene(FALSE);
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
    }
    
    return bytecodeSegmentIndex;
    
}

u16 setRaceTrackCutscenes(void) {

    u16 bytecodeSegmentIndex = 0xFFFF;
    bool set = FALSE;

    if (checkDailyEventBit(RACE_COMPLETED) && checkDailyEventBit(HORSE_RACE)) {
        updateHorseRaceContext();
        clearDailyEventBit(RACE_COMPLETED);
        gCutsceneIndex = CUTSCENE_HORSE_RACE_STARTING_ANNOUNCEMENT;
        loadCutscene(FALSE);
        set = TRUE;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
    }

    if (!set) {
        if (checkDailyEventBit(RACE_COMPLETED) && checkDailyEventBit(DOG_RACE)) {
            updateDogRaceContext();
            clearDailyEventBit(RACE_COMPLETED);
            gCutsceneIndex = CUTSCENE_DOG_RACE_AFTER_RACE;
            loadCutscene(FALSE);
            set = TRUE;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        }
    }

    if (!set) { 
        if (gSeason == SPRING && gDayOfMonth == 17 && (7 < gHour && gHour < 18)) {
            updateHorseRaceContext();
            gCutsceneIndex = CUTSCENE_HORSE_RACE_STARTING_ANNOUNCEMENT;
            loadCutscene(FALSE);
            set = TRUE;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        }
    }
    
    if (!set) { 
        if (gSeason == AUTUMN && gDayOfMonth == 28 && (7 < gHour && gHour < 18)) {
            updateHorseRaceContext();
            gCutsceneIndex = CUTSCENE_HORSE_RACE_STARTING_ANNOUNCEMENT;
            loadCutscene(FALSE);
            set = TRUE;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        }
    }

    if (!set) { 
        if (gSeason == WINTER && gDayOfMonth == 19 && (7 < gHour && gHour < 18)) {
            updateDogRaceContext();
            gCutsceneIndex = CUTSCENE_DOG_RACE_AFTER_RACE;
            loadCutscene(FALSE);
            set = TRUE;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        }
    }
    
    return bytecodeSegmentIndex;
    
}

u16 setBeachCutscenes(void) {
    
    bool set = FALSE;
    u16 bytecodeSegmentIndex = 0xFFFF;

    if (gSeason == SUMMER && gDayOfMonth == 17 && NIGHTTIME) {
        gCutsceneIndex = CUTSCENE_FIREFLY_FESTIVAL;
        loadCutscene(FALSE);
        set = TRUE;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
    }

    if (!set) {
        if (gSeason == SUMMER && gDayOfMonth == 24 && (5 < gHour && gHour < 18)) {
            gCutsceneIndex = CUTSCENE_SEA_FESTIVAL;
            loadCutscene(FALSE);
            set = TRUE;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        }
    }
    
    if (!set) {
        if (gSeason == SUMMER && gDayOfMonth == 1 && NIGHTTIME && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            gCutsceneIndex = CUTSCENE_BEACH_FIREWORKS;
            loadCutscene(FALSE);
            set = TRUE;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        }
    }
    
    if (!set) {
        if (!checkDailyEventBit(KAREN_CUTSCENE_DAILY) && gSeason == WINTER && gDayOfMonth == 10 && (5 < gHour && gHour < 18) && !checkLifeEventBit(MARRIED)) {
            setDailyEventBit(KAREN_CUTSCENE_DAILY);
            gCutsceneIndex = CUTSCENE_BEACH_KAREN_WINTER_SHARE;
            loadCutscene(FALSE);
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            set = TRUE;
        }
    }

    if (!set) {
        if (gSeason == WINTER && gDayOfMonth == 24 && NIGHTTIME && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            gCutsceneIndex = CUTSCENE_BEACH_KAREN_STARRY_NIGHT;
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
            gCutsceneIndex = CUTSCENE_BEACH_KAREN_LEAVING;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkLifeEventBit(KAREN_AND_ELLI_BEACH_CUTSCENE) && npcAffection[ELLI] >= 150 && npcAffection[KAREN] >= 150 && gWeather == SUNNY && gSeason == SUMMER && (5 < gHour && gHour < 18) && (!checkLifeEventBit(MARRIED) || gWife != KAREN) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            setLifeEventBit(KAREN_AND_ELLI_BEACH_CUTSCENE);
            gCutsceneIndex = CUTSCENE_BEACH_KAREN_AND_ELLI;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkLifeEventBit(KAREN_CONFESSION) && !checkLifeEventBit(MARRIED) && npcAffection[KAREN] >= 250 && gWeather == SUNNY && (14 < gHour && gHour < 24) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            setLifeEventBit(KAREN_CONFESSION);
            gCutsceneIndex = CUTSCENE_BEACH_KAREN_CONFESSION;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);     
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkLifeEventBit(KAREN_KAI_BEACH_CUTSCENE) && checkLifeEventBit(VINEYARD_FIRST_WINE) && npcAffection[KAI] >= 180 && gWeather == SUNNY && NIGHTTIME && (!checkLifeEventBit(MARRIED) || gWife != KAREN) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            setLifeEventBit(KAREN_KAI_BEACH_CUTSCENE);
            gCutsceneIndex = CUTSCENE_BEACH_KAREN_KAI;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
        }
    }
    
    return bytecodeSegmentIndex;

}

u16 setRoadCutscenes(void) {
    
    bool set = FALSE;
    u16 bytecodeSegmentIndex = 0xFFFF;

    if (!checkLifeEventBit(ANN_CLIFF_ROAD_CUTSCENE) && npcAffection[CLIFF] >= 200 && gSeason == WINTER && (5 < gHour && gHour < 18) && (!checkLifeEventBit(MARRIED) || gWife != ANN) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && previousMapIndex == FARM) {
        setLifeEventBit(ANN_CLIFF_ROAD_CUTSCENE);
        gCutsceneIndex = CUTSCENE_ROAD_CLIFF_LEAVING_2;
        gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        loadCutscene(FALSE);
        set = TRUE;
    }

    if (!set) {
        if (!checkLifeEventBit(ANN_RETURNS_BAKERY_CARD) && bakeryCardPoints >= 6 && npcAffection[ANN] >= 150 && (5 < gHour && gHour < 18) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && previousMapIndex == RANCH) {
            setLifeEventBit(ANN_RETURNS_BAKERY_CARD);
            gCutsceneIndex = CUTSCENE_ROAD_ANN_RETURNS_BAKERY_CARD;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
            loadCutscene(FALSE);
            set = TRUE;
        }    
    }

    if (!set) {
        if (!checkLifeEventBit(SHADY_SALESMAN_MARIA_CUTSCENE) && checkLifeEventBit(MAYOR_SHADY_SALESMAN_CUTSCENE) && (5 < gHour && gHour < 18)) {

            if (previousMapIndex == VILLAGE_1) {
                setLifeEventBit(SHADY_SALESMAN_MARIA_CUTSCENE);
                gCutsceneIndex = CUTSCENE_ROAD_SHADY_SALESMAN_AND_MARIA_1;
                gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
                loadCutscene(FALSE);
                set = TRUE;
            }

            if (previousMapIndex == VILLAGE_2) {
                setLifeEventBit(SHADY_SALESMAN_MARIA_CUTSCENE);
                gCutsceneIndex = CUTSCENE_ROAD_SHADY_SALESMAN_AND_MARIA_2;
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
            gCutsceneIndex = CUTSCENE_ROAD_ANN_SPRAINED_ANKLE;
            loadCutscene(FALSE);
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS); 
        }
    }
    
    return bytecodeSegmentIndex;

}

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
                bytecodeSegmentIndex = CUTSCENE_BANK_SHIPPING;
                set = TRUE;
            }

        }
        
        if (!set) {

            if (checkLifeEventBit(HOUSE_EXTENSION_CONSTRUCTION)) {

                if (gHour >= 8 && !checkDailyEventBit(HOUSE_EXTENSION_WORK) && !checkDailyEventBit(END_OF_DAILY_HOUSE_EXTENSION_WORK)) {
                    
                    setDailyEventBit(HOUSE_EXTENSION_WORK);
                    
                    if (gBaseMapIndex == FARM) {
                        gCutsceneIndex = 0;
                        bytecodeSegmentIndex = CUTSCENE_BANK_HOUSE_EXTENSION_WORK;
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
                    bytecodeSegmentIndex = CUTSCENE_BANK_COW_FESTIVAL_ENTRY;
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
            gCutsceneIndex = CUTSCENE_EVALUATION;
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
                    gCutsceneIndex = CUTSCENE_VILLAGE1_MARIA_WEDDING;
                    setSpecialDialogueBit(MARRIED_TO_MARIA_DIALOGUE);
                    setSpecialDialogueBit(MARRIED_WIFE_DIALOGUE);
                    break;
                case 1:                                 
                    gCutsceneIndex = CUTSCENE_VILLAGE1_POPURI_WEDDING;
                    setSpecialDialogueBit(MARRIED_TO_POPURI_DIALOGUE);
                    setSpecialDialogueBit(MARRIED_WIFE_DIALOGUE);
                    break;
                case 2:                                 
                    gCutsceneIndex = CUTSCENE_VILLAGE1_ELLI_WEDDING;
                    setSpecialDialogueBit(MARRIED_TO_ELLI_DIALOGUE);
                    setSpecialDialogueBit(MARRIED_WIFE_DIALOGUE);
                    break;
                case 3:                                 
                    gCutsceneIndex = CUTSCENE_VILLAGE1_ANN_WEDDING;
                    setSpecialDialogueBit(MARRIED_TO_ANN_DIALOGUE);
                    setSpecialDialogueBit(MARRIED_WIFE_DIALOGUE);
                    break;
                case 4:                                 
                    gCutsceneIndex = CUTSCENE_VILLAGE1_KAREN_WEDDING;
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
                gCutsceneIndex = CUTSCENE_FARM_VISIT_MARIA_HARRIS_MARRIED;
                setSpawnPoint(FARM_SPAWN_POINT_1);
                loadCutscene(FALSE);
                set = TRUE;
                gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
                
            }

            if (!set && !checkLifeEventBit(POPURI_GRAY_MARRIED) && (!checkLifeEventBit(MARRIED) || gWife != POPURI) && npcAffection[GRAY] >= 200 && (checkLifeEventBit(MARRIED) || npcAffection[POPURI] < 120)) {
               
                setLifeEventBit(POPURI_GRAY_MARRIED);
                setSpecialDialogueBit(POPURI_GRAY_MARRIED_DIALOGUE);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_POPURI_GRAY_MARRIED;
                setSpawnPoint(FARM_SPAWN_POINT_1);
                loadCutscene(FALSE);
                set = TRUE;
                gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);

            }

            if (!set && !checkLifeEventBit(ELLI_JEFF_MARRIED) && (!checkLifeEventBit(MARRIED) || gWife != ELLI) && npcAffection[JEFF] >= 200 && (checkLifeEventBit(MARRIED) || npcAffection[ELLI] < 120)) {
                
                setLifeEventBit(ELLI_JEFF_MARRIED);
                setSpecialDialogueBit(ELLI_JEFF_MARRIED_DIALOGUE);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_ELLI_JEFF_MARRIED;
                setSpawnPoint(FARM_SPAWN_POINT_1);
                loadCutscene(FALSE);
                set = TRUE;
                gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);

            }

            if (!set && !checkLifeEventBit(ANN_CLIFF_MARRIED) && (!checkLifeEventBit(MARRIED) || gWife != ANN) && npcAffection[CLIFF] >= 200 && (checkLifeEventBit(MARRIED) || npcAffection[ANN] < 120)) {
                
                setLifeEventBit(ANN_CLIFF_MARRIED);
                setSpecialDialogueBit(ANN_CLIFF_MARRIED_DIALOGUE);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_ANN_CLIFF_MARRIED;
                setSpawnPoint(FARM_SPAWN_POINT_1);
                loadCutscene(FALSE);
                set = TRUE;
                gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);

            }
            
            if (!set && !checkLifeEventBit(KAREN_KAI_MARRIED) && (!checkLifeEventBit(MARRIED) || gWife != KAREN) && npcAffection[KAI] >= 200 && (checkLifeEventBit(MARRIED) || npcAffection[KAREN] < 120) && checkLifeEventBit(HARVEST_GODDESS_VINEYARD_BLESSING_DREAM) && !checkLifeEventBit(KAI_GONE) && !checkLifeEventBit(KAREN_GONE)) {
                
                setLifeEventBit(KAREN_KAI_MARRIED);
                setSpecialDialogueBit(KAREN_KAI_MARRIED_DIALOGUE);
                gCutsceneIndex = CUTSCENE_FARM_VISIT_KAREN_KAI_MARRIED;
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
            gCutsceneIndex = CUTSCENE_VILLAGE2_ANIMAL_FUNERAL;
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

                gCutsceneIndex = CUTSCENE_HOUSE_KENT_VISIT_PLAYER_SICK;

                switch (temp) {

                    case 1:

                        if (!checkLifeEventBit(MARRIED) || gWife != MARIA) {
                            if (!checkLifeEventBit(MARRIED)) {                            
                                if (!checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
                                    gCutsceneIndex = CUTSCENE_HOUSE_MARIA_VISIT_PLAYER_SICK;
                                }
                            }
                        } else {
                            gCutsceneIndex = CUTSCENE_HOUSE_MARIA_VISIT_PLAYER_SICK;
                        }
                        
                        break;
                        
                    case 2:

                        if (!checkLifeEventBit(MARRIED) || gWife != POPURI) {
                            if (!checkLifeEventBit(MARRIED)) {                            
                                if (!checkLifeEventBit(POPURI_GRAY_MARRIED)) {
                                    gCutsceneIndex = CUTSCENE_HOUSE_POPURI_VISIT_PLAYER_SICK;
                                }
                            }
                        } else {
                            gCutsceneIndex = CUTSCENE_HOUSE_POPURI_VISIT_PLAYER_SICK;
                        }
                        
                        break;
                    
                    case 3:

                        if (!checkLifeEventBit(MARRIED) || gWife != ELLI) {
                            if (!checkLifeEventBit(MARRIED)) {                            
                                if (!checkLifeEventBit(ELLI_JEFF_MARRIED)) {
                                    gCutsceneIndex = CUTSCENE_HOUSE_ELLI_VISIT_PLAYER_SICK;
                                }
                            }
                        } else {
                            gCutsceneIndex = CUTSCENE_HOUSE_ELLI_VISIT_PLAYER_SICK;
                        }

                        break;
                    
                    case 4: 

                        if (!checkLifeEventBit(MARRIED) || gWife != ANN) {
                            if (!checkLifeEventBit(MARRIED)) {                            
                                if (!checkLifeEventBit(ANN_CLIFF_MARRIED)) {
                                    gCutsceneIndex = CUTSCENE_HOUSE_ANN_VISIT_PLAYER_SICK;
                                }
                            }
                        }  else{
                            gCutsceneIndex = CUTSCENE_HOUSE_ANN_VISIT_PLAYER_SICK;
                        }

                        break;

                    case 5:

                        if (!checkLifeEventBit(MARRIED) || gWife != KAREN) {
                            if (!checkLifeEventBit(MARRIED)) {                            
                                if (!checkLifeEventBit(KAREN_KAI_MARRIED)) {
                                    gCutsceneIndex = CUTSCENE_HOUSE_KAREN_VISIT_PLAYER_SICK;
                                }
                            }
                        } else {
                            gCutsceneIndex = CUTSCENE_HOUSE_KAREN_VISIT_PLAYER_SICK;
                        }

                        break;

                    default:
                        break;

                }

            } else {

                if (npcAffection[KENT] >= 50) {
                    gCutsceneIndex = CUTSCENE_HOUSE_KENT_VISIT_PLAYER_SICK;
                } else {
                    gCutsceneIndex = CUTSCENE_HOUSE_PLAYER_SICK_NO_VISITOR;
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

// dreams
u16 setDreamCutscenes(void) {

    bool set = FALSE;

    if (!checkLifeEventBit(HARVEST_GODDESS_VINEYARD_BLESSING_DREAM) && checkSpecialDialogueBit(GRAPEVINE_SPIRIT_AWAKENED_DIALOGUE) && gSeason == AUTUMN && gDayOfMonth == 6) {
        gCutsceneIndex = CUTSCENE_HOUSE_HARVEST_GODDESS_VINEYARD_BLESSING_DREAM;
        loadCutscene(FALSE);
        set = TRUE;
        gCutsceneFlags |= CUTSCENE_SUPPRESS_NPC_SETUP;
    }

    if (!set) {
        if (!checkLifeEventBit(MARIA_DREAM_CUTSCENE) && npcAffection[MARIA] >= 160 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            gCutsceneIndex = CUTSCENE_HOUSE_MARIA_DREAM;
            loadCutscene(FALSE);
            set = TRUE;
        }
    }

    if (!set) { 
        if (!checkLifeEventBit(POPURI_DREAM_CUTSCENE) && npcAffection[POPURI] >= 160 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            gCutsceneIndex = CUTSCENE_HOUSE_POPURI_DREAM;
            loadCutscene(FALSE);
            set = TRUE;
        }
    }
    
    if (!set) { 
        if (!checkLifeEventBit(ELLI_DREAM_CUTSCENE) && npcAffection[ELLI] >= 160 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            gCutsceneIndex = CUTSCENE_HOUSE_ELLI_DREAM;
            loadCutscene(FALSE);
            set = TRUE;
        }
    }

    if (!set) { 
        if (!checkLifeEventBit(ANN_DREAM_CUTSCENE) && npcAffection[ANN] >= 160 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            gCutsceneIndex = CUTSCENE_HOUSE_ANN_DREAM;
            loadCutscene(FALSE);
            set = TRUE;
        }
    }

    if (!set) { 
        if (!checkLifeEventBit(KAREN_DREAM_CUTSCENE) && npcAffection[KAREN] >= 160 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            gCutsceneIndex = CUTSCENE_HOUSE_KAREN_DREAM;
            loadCutscene(FALSE);
            set = TRUE;
        }
    }

    if (!set) { 
        if (!checkLifeEventBit(BABY_FEVER_CUTSCENE) && checkLifeEventBit(MARRIED) && checkLifeEventBit(HAVE_BABY) && !checkDailyEventBit(BIRTH_EVENT_DAILY) && gWife == MARIA && (29 < gBabyAge && gBabyAge < 120) && !getRandomNumberInRange(0, 10)) {
            setLifeEventBit(BABY_FEVER_CUTSCENE);
            gCutsceneIndex = CUTSCENE_HOUSE_MARIA_BABY_FEVER;
            loadCutscene(FALSE);
            set = TRUE;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        }
    }

    if (!set) { 
        if (!checkLifeEventBit(BABY_FEVER_CUTSCENE) && checkLifeEventBit(MARRIED) && checkLifeEventBit(HAVE_BABY) && !checkDailyEventBit(BIRTH_EVENT_DAILY) && gWife == POPURI && (29 < gBabyAge && gBabyAge < 120) && !getRandomNumberInRange(0, 10)) {
            setLifeEventBit(BABY_FEVER_CUTSCENE);
            gCutsceneIndex = CUTSCENE_HOUSE_POPURI_BABY_FEVER;
            loadCutscene(FALSE);
            set = TRUE;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        }
    }

    if (!set) { 
        if (!checkLifeEventBit(BABY_FEVER_CUTSCENE) && checkLifeEventBit(MARRIED) && checkLifeEventBit(HAVE_BABY) && !checkDailyEventBit(BIRTH_EVENT_DAILY) && gWife == ELLI && (29 < gBabyAge && gBabyAge < 120) && !getRandomNumberInRange(0, 10)) {
            setLifeEventBit(BABY_FEVER_CUTSCENE);
            gCutsceneIndex = CUTSCENE_HOUSE_ELLI_BABY_FEVER;
            loadCutscene(FALSE);
            set = TRUE;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        }
    }

    if (!set) { 
        if (!checkLifeEventBit(BABY_FEVER_CUTSCENE) && checkLifeEventBit(MARRIED) && checkLifeEventBit(HAVE_BABY) && !checkDailyEventBit(BIRTH_EVENT_DAILY) && gWife == ANN && (29 < gBabyAge && gBabyAge < 120) && !getRandomNumberInRange(0, 10)) {
            setLifeEventBit(BABY_FEVER_CUTSCENE);
            gCutsceneIndex = CUTSCENE_HOUSE_ANN_BABY_FEVER;
            loadCutscene(FALSE);
            set = TRUE;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        }
    }

    if (!set) { 
        if (!checkLifeEventBit(BABY_FEVER_CUTSCENE) && checkLifeEventBit(MARRIED) && checkLifeEventBit(HAVE_BABY) && !checkDailyEventBit(BIRTH_EVENT_DAILY) && gWife == KAREN && (29 < gBabyAge && gBabyAge < 120) && !getRandomNumberInRange(0, 10)) {
            setLifeEventBit(BABY_FEVER_CUTSCENE);
            gCutsceneIndex = CUTSCENE_HOUSE_KAREN_BABY_FEVER;
            loadCutscene(FALSE);
            set = TRUE;
            gCutsceneFlags |= (CUTSCENE_SUPPRESS_NPC_SETUP | CUTSCENE_SUPPRESS_FARM_ANIMALS);
        }
    }
    
    return set;
    
}

void handleCutsceneCompletion(void) {

    bool found;
    u16 tempSpawnPoint;
    
    if (gCutsceneFlags & CUTSCENE_ACTIVE) {

        if (gCutsceneCompletionFlags & CUTSCENE_COMPLETION_EXIT) {

            gCutsceneFlags &= ~CUTSCENE_ACTIVE;

            switch (gCutsceneBytecodeSegmentIndex) {

                case CUTSCENE_BANK_ANIMAL_DELIVERY:                                 

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

                case CUTSCENE_BANK_FARM_VISIT_MORNING:
                case CUTSCENE_BANK_FARM_VISIT_WORKING:

                    switch (gCutsceneIndex) {

                        case CUTSCENE_FARM_VISIT_ELLI_THANKSGIVING_EGG_VISIT:
                            handleHatchChicken();
                            handleHatchChicken();
                            handleHatchChicken();
                            handleHatchChicken();
                            handleHatchChicken();
                            handleHatchChicken();
                            break;    
                        
                        case CUTSCENE_FARM_VISIT_KENT_AND_STU_PUPPIES:
                            setSpawnPoint(FARM_SPAWN_POINT_1);
                            setMainLoopCallbackFunctionIndex(MAP_LOAD);
                            break;
                        
                        case CUTSCENE_FARM_VISIT_DOUG_HORSE_RACE_WIN_VISIT:
                        case CUTSCENE_FARM_VISIT_DOUG_COW_FESTIVAL_WIN_VISIT:
                        case CUTSCENE_FARM_VISIT_BASIL_VEGETABLE_FESTIVAL_WIN_VISIT:
                        case CUTSCENE_FARM_VISIT_MARIA_WIFE_SEA_FESTIVAL ... CUTSCENE_FARM_VISIT_KAREN_WIFE_SEA_FESTIVAL:
                            if (gCutsceneCompletionFlags & CUTSCENE_COMPLETION_CONDITIONAL_OUTCOME) {
                                setMainLoopCallbackFunctionIndex(END_OF_FESTIVAL_DAY_1);
                            }
                            break;
                        
                        case CUTSCENE_FARM_VISIT_ELLI_THANKSGIVING_GIFT:
                            break;
                        
                        case CUTSCENE_FARM_VISIT_MARIA_FIREWORKS_TRANSITION ... CUTSCENE_FARM_VISIT_KAREN_KEIFU_FAIRIES_INVITE_TRANSITION:
                            setMainLoopCallbackFunctionIndex(END_OF_FESTIVAL_DAY_1);
                            break;
                        
                    }
                    
                    break;

                case CUTSCENE_BANK_HOUSE:
                    
                    switch (gCutsceneIndex) {           

                        case CUTSCENE_HOUSE_KENT_HOUSE_VISIT_CUTSCENE:                           
                        case CUTSCENE_HOUSE_KENT_HOUSE_VISIT_CHECKIN:                           
                            setLifeEventBit(KENT_HOUSE_VISIT_CUTSCENE);
                            break;
                        case CUTSCENE_HOUSE_CLIFF_HOUSE_VISIT_CUTSCENE:                          
                        case CUTSCENE_HOUSE_CLIFF_AND_ANN_INTRODUCTION:                          
                            setLifeEventBit(CLIFF_HOUSE_VISIT_CUTSCENE);
                            break;
                        case CUTSCENE_HOUSE_HARVEST_GODDESS_VINEYARD_BLESSING_DREAM:                          
                            setLifeEventBit(HARVEST_GODDESS_VINEYARD_BLESSING_DREAM);
                        case CUTSCENE_HOUSE_KENT_VISIT_PLAYER_SICK:                           
                        case CUTSCENE_HOUSE_MARIA_VISIT_PLAYER_SICK:                           
                        case CUTSCENE_HOUSE_MARIA_AFTER_BABY_NAMING:                           
                        case CUTSCENE_HOUSE_MARIA_BABY_FEVER:                           
                        case CUTSCENE_HOUSE_POPURI_VISIT_PLAYER_SICK:                           
                        case CUTSCENE_HOUSE_POPURI_AFTER_BABY_NAMING:                           
                        case CUTSCENE_HOUSE_POPURI_BABY_FEVER:                          
                        case CUTSCENE_HOUSE_ELLI_VISIT_PLAYER_SICK:                          
                        case CUTSCENE_HOUSE_ELLI_AFTER_BABY_NAMING:                          
                        case CUTSCENE_HOUSE_ELLI_BABY_FEVER:                          
                        case CUTSCENE_HOUSE_ANN_VISIT_PLAYER_SICK:                          
                        case CUTSCENE_HOUSE_ANN_AFTER_BABY_NAMING:                          
                        case CUTSCENE_HOUSE_ANN_BABY_FEVER:                          
                        case CUTSCENE_HOUSE_KAREN_VISIT_PLAYER_SICK:                          
                        case CUTSCENE_HOUSE_KAREN_AFTER_BABY_NAMING:                          
                        case CUTSCENE_HOUSE_KAREN_BABY_FEVER:                          
                        case CUTSCENE_HOUSE_PLAYER_SICK_NO_VISITOR:                          
                            setMainLoopCallbackFunctionIndex(END_OF_DAY_1);
                            break;
                        case CUTSCENE_HOUSE_MARIA_DREAM:                          
                            setLifeEventBit(MARIA_DREAM_CUTSCENE);
                            setMainLoopCallbackFunctionIndex(END_OF_DAY_1);
                            break;
                        case CUTSCENE_HOUSE_POPURI_DREAM:                          
                            setLifeEventBit(POPURI_DREAM_CUTSCENE);
                            setMainLoopCallbackFunctionIndex(END_OF_DAY_1);                             
                            break;
                        case CUTSCENE_HOUSE_ELLI_DREAM:                          
                            setLifeEventBit(ELLI_DREAM_CUTSCENE);
                            setMainLoopCallbackFunctionIndex(END_OF_DAY_1);                             
                            break;
                        case CUTSCENE_HOUSE_ANN_DREAM:                          
                            setLifeEventBit(ANN_DREAM_CUTSCENE);
                            setMainLoopCallbackFunctionIndex(END_OF_DAY_1);                             
                            break;
                        case CUTSCENE_HOUSE_KAREN_DREAM:                          
                            setLifeEventBit(KAREN_DREAM_CUTSCENE);
                            setMainLoopCallbackFunctionIndex(END_OF_DAY_1);                             
                            break;
                        case CUTSCENE_HOUSE_MARIA_BIRTH:                           
                        case CUTSCENE_HOUSE_POPURI_BIRTH:                           
                        case CUTSCENE_HOUSE_ELLI_BIRTH:                          
                        case CUTSCENE_HOUSE_ANN_BIRTH:                          
                        case CUTSCENE_HOUSE_KAREN_BIRTH:                          
                            if (gHour >= 6) {
                                gNamingScreenIndex = NAMING_SCREEN_TYPE_BABY;
                                handleExitLevel(1, NAMING_SCREEN_LOAD);
                            } else {
                                setMainLoopCallbackFunctionIndex(END_OF_DAY_1);                             
                            }
                            break;
                        
                        }

                    break;

                case CUTSCENE_BANK_ROAD:
                    if (gCutsceneIndex == CUTSCENE_ROAD_ANN_SPRAINED_ANKLE && (gCutsceneCompletionFlags & CUTSCENE_COMPLETION_CONDITIONAL_OUTCOME)) {
                        gHour += 2;
                        setSpawnPoint(RANCH_SPAWN_POINT_3);
                        setMainLoopCallbackFunctionIndex(MAP_LOAD);
                    }
                
                case CUTSCENE_BANK_MOUNTAIN:

                    switch (gCutsceneIndex) {

                        case CUTSCENE_MOUNTAIN_POPURI_SPRAINED_ANKLE:
                            if (gCutsceneCompletionFlags & CUTSCENE_COMPLETION_CONDITIONAL_OUTCOME) {
                                gHour += 2;
                                setSpawnPoint(VILLAGE_1_SPAWN_POINT_3);
                                setMainLoopCallbackFunctionIndex(MAP_LOAD);
                            }
                            break;

                        case CUTSCENE_MOUNTAIN_ELLI_SPRAINED_ANKLE:
                            if (gCutsceneCompletionFlags & CUTSCENE_COMPLETION_CONDITIONAL_OUTCOME) {
                                gHour += 2;
                                setSpawnPoint(VILLAGE_1_SPAWN_POINT_4);
                                setMainLoopCallbackFunctionIndex(MAP_LOAD);
                            }
                            break;

                        case CUTSCENE_MOUNTAIN_FIREWORKS:
                        case CUTSCENE_MOUNTAIN_STARRY_NIGHT_MOON:
                            setMainLoopCallbackFunctionIndex(END_OF_FESTIVAL_DAY_1);
                            break;

                        case CUTSCENE_MOUNTAIN_GREG_FISHING_POLE:                         
                            storeTool(FISHING_POLE);
                            break;
                        
                        case CUTSCENE_MOUNTAIN_MASTER_CARPENTER_BRIDGE_RECRUIT:
                        case CUTSCENE_MOUNTAIN_MASTER_CARPENTER_HOT_SPRINGS_RECRUIT:
                            setDailyEventBit(MOUNTAIN_CARPENTER_HELP);
                            break;

                        case CUTSCENE_MOUNTAIN_MASTER_CARPENTER_BRIDGE_PAY:
                            if (gSeason == AUTUMN && gDayOfMonth == 27) {
                                setLifeEventBit(BRIDGE_COMPLETED);
                            }
                            break;

                        case CUTSCENE_MOUNTAIN_MASTER_CARPENTER_HOT_SPRINGS_COMPLETE:                         
                            if (gSeason == WINTER && gDayOfMonth == 16) {
                                setLifeEventBit(HOT_SPRINGS_COMPLETED);
                                setDailyEventBit(HOT_SPRINGS_COMPLETED_PHOTO_MAIL);
                            }
                            clearForagableObjects(TOP_OF_MOUNTAIN_1);
                            break;
                        
                        case CUTSCENE_MOUNTAIN_KAPPA_GIFT:                         
                            clearDailyEventBit(KAPPA_FISH_OFFERING_DAILY);
                            setLifeEventBit(MET_KAPPA);
                            break;
                        
                        case CUTSCENE_MOUNTAIN_KAPPA_POWER_NUT:                         
                            clearDailyEventBit(KAPPA_LARGE_FISH_OFFERING_DAILY);
                            setLifeEventBit(KAPPA_POWER_NUT_GIFT);
                            break;
                        
                        case CUTSCENE_MOUNTAIN_HARVEST_GODDESS_OFFERING:                         
                        case CUTSCENE_MOUNTAIN_HARVEST_GODDESS_GRAPEVINE_OFFERING:                         
                            clearDailyEventBit(HARVEST_GODDESS_OFFERING);
                            setDailyEventBit(HARVEST_GODDESS_INTERACTION);
                            break;
                        
                        case CUTSCENE_MOUNTAIN_HARVEST_GODDESS_GRAPEVINE_AWAKEN:                         
                            setSpecialDialogueBit(GRAPEVINE_SPIRIT_AWAKENED_DIALOGUE);
                            setMainLoopCallbackFunctionIndex(END_OF_FESTIVAL_DAY_1);
                            break;
                        
                    }
                    
                    break;
                
                case CUTSCENE_BANK_RANCH:            
                                    
                    switch (gCutsceneIndex) {           

                        case CUTSCENE_RANCH_ANN_FIREWORKS:
                            setMainLoopCallbackFunctionIndex(END_OF_FESTIVAL_DAY_1);
                            break;

                        case CUTSCENE_RANCH_GET_HORSE:                         
                        case CUTSCENE_RANCH_DOUG_HORSE_OFFER_ALT:                         
                            gNamingScreenIndex = NAMING_SCREEN_TYPE_HORSE;
                            handleExitLevel(1, NAMING_SCREEN_LOAD);
                            setLifeEventBit(HAVE_HORSE);
                            break;
                        case CUTSCENE_RANCH_HORSE_NAMING_FOLLOWUP:                         
                            loadLevel(0);
                            setMainLoopCallbackFunctionIndex(SET_AUDIO_AND_LIGHTING);
                            initializeHorse();
                            break;
                        case CUTSCENE_RANCH_ANN_SICK_VISIT_ROOM:
                            setSpawnPoint(RANCH_STORE_SPAWN_POINT_2);
                            setMainLoopCallbackFunctionIndex(MAP_LOAD);
                            break;
                        case CUTSCENE_RANCH_ANN_REPAIR_MARRIAGE_ROOM:                         
                            npcAffection[ANN] = 120;
                            clearLifeEventBit(WIFE_LEFT);
                            setMainLoopCallbackFunctionIndex(END_OF_DAY_1);
                            break;
                        }

                    break;

                case CUTSCENE_BANK_VINEYARD:
                    
                    switch (gCutsceneIndex) {          

                        case CUTSCENE_VINEYARD_KAREN_SPRAINED_ANKLE:
                            if (gCutsceneCompletionFlags & CUTSCENE_COMPLETION_CONDITIONAL_OUTCOME) {
                                gHour += 2;
                                setSpawnPoint(VINEYARD_SPAWN_POINT_2);
                                setMainLoopCallbackFunctionIndex(MAP_LOAD);
                            }
                            break;
                        
                        case CUTSCENE_VINEYARD_KAREN_SICK_VISIT_ROOM:
                            setSpawnPoint(VINEYARD_HOUSE_SPAWN_POINT_1);
                            setMainLoopCallbackFunctionIndex(MAP_LOAD);
                            break;
                        
                        case CUTSCENE_VINEYARD_KAREN_CELLAR_OFFER:
                            if (gCutsceneCompletionFlags & CUTSCENE_COMPLETION_CONDITIONAL_OUTCOME) {
                                clearDailyEventBit(KAREN_YELLOW_HEART_EVENT_DAILY);
                                setSpawnPoint(VINEYARD_SPAWN_POINT_3);
                                setMainLoopCallbackFunctionIndex(MAP_LOAD);
                            }
                            break;
                        
                        case CUTSCENE_VINEYARD_KAREN_REPAIR_MARRIAGE_ROOM:                         
                            npcAffection[KAREN] = 120;
                            clearLifeEventBit(WIFE_LEFT);
                            setMainLoopCallbackFunctionIndex(END_OF_DAY_1);
                            break;
                    }
                    
                    break;
                
                case CUTSCENE_BANK_VILLAGE_1:

                    switch (gCutsceneIndex) {          
                        
                        case CUTSCENE_VILLAGE1_MARIA_WEDDING:
                        case CUTSCENE_VILLAGE1_POPURI_WEDDING:
                        case CUTSCENE_VILLAGE1_ELLI_WEDDING:
                        case CUTSCENE_VILLAGE1_ANN_WEDDING:
                        case CUTSCENE_VILLAGE1_KAREN_WEDDING:
                            setMainLoopCallbackFunctionIndex(END_OF_DAY_1);
                            break;
                        case CUTSCENE_VILLAGE1_RIVAL_WEDDING_MARIA:                 
                        case CUTSCENE_VILLAGE1_RIVAL_WEDDING_POPURI:                 
                        case CUTSCENE_VILLAGE1_RIVAL_WEDDING_ELLI:                 
                        case CUTSCENE_VILLAGE1_RIVAL_WEDDING_ANN:                 
                        case CUTSCENE_VILLAGE1_RIVAL_WEDDING_KAREN:        
                            setMainLoopCallbackFunctionIndex(END_OF_FESTIVAL_DAY_1);
                            break; 
                        
                        case CUTSCENE_VILLAGE1_POPURI_MOUNTAIN_FOUNTAIN:
                            setSpawnPoint(MOUNTAIN_2_SPAWN_POINT_3);
                            setMainLoopCallbackFunctionIndex(MAP_LOAD);
                            blueMistSeedsQuantity += adjustValue(blueMistSeedsQuantity, -1, 1);
                            removeTool(BLUE_MIST_SEEDS);
                            blueMistFlowerPlot = 0xB3;
                            pondFieldTiles[5][3] = 0xB3;
                            break;
                            
                        case CUTSCENE_VILLAGE1_ELLEN_DIES:                        
                            setLifeEventBit(ELLEN_DIED);
                            setSpecialDialogueBit(ELLEN_DIED_DIALOGUE);
                            setMainLoopCallbackFunctionIndex(END_OF_FESTIVAL_DAY_1);
                            break;
                        
                        case CUTSCENE_VILLAGE1_ELLI_SPARKLERS:                 
                        case CUTSCENE_VILLAGE1_JEFF_BAKERY:                 
                        case CUTSCENE_VILLAGE1_NEW_YEAR_SUNRISE:                 
                        case CUTSCENE_VILLAGE1_NEW_YEAR_SUNRISE_ELLI:                 
                        case CUTSCENE_VILLAGE1_NEW_YEAR_SUNRISE_ANN:                 
                        case CUTSCENE_VILLAGE1_CHURCH_NEW_YEARS:                 
                        case CUTSCENE_VILLAGE1_TAVERN_NEW_YEARS:                 
                            setMainLoopCallbackFunctionIndex(END_OF_FESTIVAL_DAY_1);
                            break;

                        case CUTSCENE_VILLAGE1_STARRY_NIGHT_FESTIVAL:                        
                            if (!(gCutsceneCompletionFlags & CUTSCENE_COMPLETION_CHAIN_NEXT)) {
                                setMainLoopCallbackFunctionIndex(END_OF_FESTIVAL_DAY_1);
                            }
                            break;

                        case CUTSCENE_VILLAGE1_POPURI_COLD_VISIT:
                            setSpawnPoint(FLOWER_SHOP_SPAWN_POINT_2);
                            setMainLoopCallbackFunctionIndex(MAP_LOAD);
                            break;
                
                        case CUTSCENE_VILLAGE1_ELLI_SICK_VISIT:
                            setSpawnPoint(BAKERY_SPAWN_POINT_2);
                            setMainLoopCallbackFunctionIndex(MAP_LOAD);
                            break;
                
                        case CUTSCENE_VILLAGE1_POPURI_REPAIR_MARRIAGE:                        
                            npcAffection[POPURI] = 120;
                            clearLifeEventBit(WIFE_LEFT);
                            setMainLoopCallbackFunctionIndex(END_OF_DAY_1);
                            break;
                
                        case CUTSCENE_VILLAGE1_ELLI_REPAIR_MARRIAGE:                        
                            npcAffection[ELLI] = 120;
                            clearLifeEventBit(WIFE_LEFT);
                            setMainLoopCallbackFunctionIndex(END_OF_DAY_1);
                            break;

                        }
                    
                    break;

                case CUTSCENE_BANK_VILLAGE_2:

                    switch (gCutsceneIndex) {           

                        case CUTSCENE_VILLAGE2_ANIMAL_FUNERAL:                        
                            setMainLoopCallbackFunctionIndex(END_OF_DAY_1);
                            break;

                        case CUTSCENE_VILLAGE2_MARIA_SICK_VISIT_ROOM:
                            setSpawnPoint(MAYOR_HOUSE_SPAWN_POINT_2);
                            setMainLoopCallbackFunctionIndex(MAP_LOAD);
                            break;
                            
                        case CUTSCENE_VILLAGE2_MARIA_SPRAINED_ANKLE_ROOM:
                            if (gCutsceneCompletionFlags & CUTSCENE_COMPLETION_CONDITIONAL_OUTCOME) {
                                gHour += 2;
                                setSpawnPoint(VILLAGE_2_SPAWN_POINT_4);
                                setMainLoopCallbackFunctionIndex(MAP_LOAD);
                            }
                            break;

                        case CUTSCENE_VILLAGE2_MARIA_REPAIR_MARRIAGE_ROOM:                         
                            npcAffection[MARIA] = 120;
                            clearLifeEventBit(WIFE_LEFT);
                            setMainLoopCallbackFunctionIndex(END_OF_DAY_1);
                        
                        }
                    
                    break;
                    
                case CUTSCENE_BANK_BEACH:
                    if (gCutsceneIndex != CUTSCENE_BEACH_FIREWORKS) {
                        if (gCutsceneIndex != CUTSCENE_BEACH_KAREN_STARRY_NIGHT) {
                            break;
                        }
                    } 
                    setMainLoopCallbackFunctionIndex(END_OF_FESTIVAL_DAY_1);
                    break;

                case CUTSCENE_BANK_SOWING_FESTIVAL:
                    setMainLoopCallbackFunctionIndex(END_OF_FESTIVAL_DAY_1);
                    break;
                
                case CUTSCENE_BANK_HORSE_RACE:

                    switch (gCutsceneIndex) {

                        case CUTSCENE_HORSE_RACE_RACE:
                        case CUTSCENE_HORSE_RACE_PLAYER:

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

                case CUTSCENE_BANK_FLOWER_FESTIVAL:
                    setMainLoopCallbackFunctionIndex(END_OF_FESTIVAL_DAY_1);
                    break;
                
                case CUTSCENE_BANK_VEGETABLE_FESTIVAL:
                    if (gCutsceneIndex == CUTSCENE_VEGETABLE_FESTIVAL_SQUARE) {
                        setMainLoopCallbackFunctionIndex(END_OF_FESTIVAL_DAY_1);
                    }  
                    break;
                
                case CUTSCENE_BANK_FIREWORKS_FESTIVAL:
                    if (gCutsceneIndex == CUTSCENE_FIREWORKS_FESTIVAL) {
                        setMainLoopCallbackFunctionIndex(END_OF_FESTIVAL_DAY_1);
                    } 
                    break;

                case CUTSCENE_BANK_FIREFLY_FESTIVAL:
                case CUTSCENE_BANK_SEA_FESTIVAL:
                    setMainLoopCallbackFunctionIndex(END_OF_FESTIVAL_DAY_1);
                    break;
                
                case CUTSCENE_BANK_COW_FESTIVAL:   
                    if (checkLifeEventBit(COW_FESTIVAL_WON_THIS_YEAR)) {
                        gFarmAnimals[gCowFestivalEnteredCowIndex].milkType = D_80180710;
                    }
                    setMainLoopCallbackFunctionIndex(END_OF_FESTIVAL_DAY_1);
                    break;

                case CUTSCENE_BANK_HARVEST_FESTIVAL:
                    setMainLoopCallbackFunctionIndex(END_OF_FESTIVAL_DAY_1);
                    break;

                case CUTSCENE_BANK_EGG_FESTIVAL:
                    setMainLoopCallbackFunctionIndex(END_OF_FESTIVAL_DAY_1);
                    break;
                
                case CUTSCENE_BANK_DOG_RACE:
                    
                    if (gCutsceneIndex == CUTSCENE_DOG_RACE_RACE) {
                        
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

                case CUTSCENE_BANK_SPIRIT_FESTIVAL:
                case CUTSCENE_BANK_NEW_YEAR_FESTIVAL:
                    setMainLoopCallbackFunctionIndex(END_OF_FESTIVAL_DAY_1);
                    break;

                case CUTSCENE_BANK_ENDING:
                    
                    if (gCutsceneIndex == CUTSCENE_CREDITS) {

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
                            launchIntroCutscene(CUTSCENE_FUNERAL, SQUARE_SPAWN_POINT_1, 1);
                        }

                    }

                    break;
                
                case CUTSCENE_BANK_DEMOS:
                    if (gCutsceneIndex == CUTSCENE_DEMO_2) {
                        launchIntroCutscene(1502, MOUNTAIN_1_SPAWN_POINT_1, 1);
                    } else {
                        memcpy(farmFieldTiles, defaultFarmFieldTiles, FIELD_HEIGHT * FIELD_WIDTH);
                        launchIntroCutscene(CUTSCENE_FUNERAL, SQUARE_SPAWN_POINT_1, 1);
                    }
                    break;

                case CUTSCENE_BANK_HOW_TO_PLAY:
                    if ((gCutsceneIndex == 1600) && !(gCutsceneCompletionFlags & CUTSCENE_COMPLETION_CHAIN_NEXT)) {
                        memcpy(farmFieldTiles, defaultFarmFieldTiles, FIELD_HEIGHT * FIELD_WIDTH);
                        initializeTitleScreen(1);
                    }
                    break;
                
            }

            // opening cutscenes (funeral, mayor tour)
            if ((gCutsceneCompletionFlags & CUTSCENE_COMPLETION_RETURN_TO_TITLE) && gCutsceneBytecodeSegmentIndex == 30) {
                deactivateCutsceneExecutors();
                deactivateSprites();
                unloadMapAssets(MAIN_MAP_INDEX);
                initializeTitleScreen(0);
            }

            if (gCutsceneCompletionFlags & CUTSCENE_COMPLETION_CHAIN_NEXT) {

                updateAnimalCoordinates();
                deactivateAnimalEntities();

                switch (gCutsceneIndex) {

                    case CUTSCENE_HOUSE_KENT_HOUSE_VISIT_CUTSCENE:
                        transitionCutscenes(CUTSCENE_HOUSE_KENT_HOUSE_VISIT_CHECKIN, VILLAGE_2_SPAWN_POINT_1);
                        break;

                    case CUTSCENE_HOUSE_CLIFF_HOUSE_VISIT_CUTSCENE:         
                        transitionCutscenes(CUTSCENE_HOUSE_CLIFF_AND_ANN_INTRODUCTION, FARM_SPAWN_POINT_1);
                        gCutsceneFlags &= ~CUTSCENE_DEACTIVATE_ANIMALS_ON_COMPLETION;
                        gCutsceneFlags |= CUTSCENE_REINIT_ANIMALS_ON_COMPLETION;
                        break;
                    
                    case CUTSCENE_FARM_VISIT_MARIA_FIREWORKS:          
                        deactivateSprites();
                        deactivateGlobalSprites();
                        initializeCutsceneExecutors();
                        deactivateNPCEntities();
                        deactivateAnimalEntities();
                        clearAllItemContextSlots();
                        initializeEntityInstances(1);
                        transitionCutscenes(CUTSCENE_FARM_VISIT_MARIA_FIREWORKS_TRANSITION, POND_SPAWN_POINT_1);
                        break;

                    case CUTSCENE_FARM_VISIT_MARIA_WIFE_FIREWORKS_FESTIVAL:          
                    case CUTSCENE_FARM_VISIT_POPURI_WIFE_FIREWORKS_FESTIVAL:          
                    case CUTSCENE_FARM_VISIT_ELLI_WIFE_FIREWORKS_FESTIVAL:          
                    case CUTSCENE_FARM_VISIT_ANN_WIFE_FIREWORKS_FESTIVAL:          
                    case CUTSCENE_FARM_VISIT_KAREN_WIFE_FIREWORKS_FESTIVAL:          
                        deactivateSprites();
                        deactivateGlobalSprites();
                        initializeCutsceneExecutors();
                        deactivateNPCEntities();
                        deactivateAnimalEntities();
                        clearAllItemContextSlots();
                        initializeEntityInstances(1);
                        transitionCutscenes(CUTSCENE_FARM_VISIT_WIFE_FIREWORKS_FESTIVAL_TRANSITION, MOUNTAIN_1_SPAWN_POINT_1);
                        break; 
                    
                    case CUTSCENE_FARM_VISIT_MARIA_WIFE_MOON_VIEWING:          
                    case CUTSCENE_FARM_VISIT_POPURI_WIFE_MOON_VIEWING:          
                    case CUTSCENE_FARM_VISIT_ELLI_WIFE_MOON_VIEWING:          
                    case CUTSCENE_FARM_VISIT_ANN_WIFE_MOON_VIEWING:          
                    case CUTSCENE_FARM_VISIT_KAREN_WIFE_MOON_VIEWING:          
                        deactivateSprites();
                        deactivateGlobalSprites();
                        initializeCutsceneExecutors();
                        deactivateNPCEntities();
                        deactivateAnimalEntities();
                        clearAllItemContextSlots();
                        initializeEntityInstances(1);
                        transitionCutscenes(CUTSCENE_FARM_VISIT_WIFE_MOON_VIEWING_TRANSITION, MOON_MOUNTAIN_SPAWN_POINT_1);
                        break;
                
                    case CUTSCENE_FARM_VISIT_POPURI_BLUE_MIST:          
                        deactivateSprites();
                        deactivateGlobalSprites();
                        initializeCutsceneExecutors();
                        deactivateNPCEntities();
                        deactivateAnimalEntities();
                        clearAllItemContextSlots();
                        initializeEntityInstances(1);
                        transitionCutscenes(CUTSCENE_FARM_VISIT_POPURI_BLUE_MIST_TRANSITION, POND_SPAWN_POINT_1);
                        break;

                    case CUTSCENE_FARM_VISIT_ELLI_MOON_VIEWING:          
                        deactivateSprites();
                        deactivateGlobalSprites();
                        initializeCutsceneExecutors();
                        deactivateNPCEntities();
                        deactivateAnimalEntities();
                        clearAllItemContextSlots();
                        initializeEntityInstances(1);
                        transitionCutscenes(CUTSCENE_FARM_VISIT_ELLI_MOON_VIEWING_TRANSITION, POND_SPAWN_POINT_1);
                        break;
                    
                    case CUTSCENE_FARM_VISIT_ANN_FIREWORKS:          
                        deactivateSprites();
                        deactivateGlobalSprites();
                        initializeCutsceneExecutors();
                        deactivateNPCEntities();
                        deactivateAnimalEntities();
                        clearAllItemContextSlots();
                        initializeEntityInstances(1);
                        transitionCutscenes(CUTSCENE_FARM_VISIT_ANN_FIREWORKS_TRANSITION, POND_SPAWN_POINT_1);
                        break;
                    
                    case CUTSCENE_FARM_VISIT_KAREN_KEIFU_FAIRIES_INVITE:          
                        deactivateSprites();
                        deactivateGlobalSprites();
                        initializeCutsceneExecutors();
                        deactivateNPCEntities();
                        deactivateAnimalEntities();
                        clearAllItemContextSlots();
                        initializeEntityInstances(1);
                        transitionCutscenes(CUTSCENE_FARM_VISIT_KAREN_KEIFU_FAIRIES_INVITE_TRANSITION, VINEYARD_SPAWN_POINT_1);
                        break;
                    
                    case CUTSCENE_FARM_VISIT_MARIA_HARRIS_MARRIED:          
                        deactivateSprites();
                        deactivateGlobalSprites();
                        initializeCutsceneExecutors();
                        deactivateNPCEntities();
                        deactivateAnimalEntities();
                        clearAllItemContextSlots();
                        initializeEntityInstances(1);
                        transitionCutscenes(CUTSCENE_VILLAGE1_RIVAL_WEDDING_MARIA, VILLAGE_1_SPAWN_POINT_1);
                        break;

                    case CUTSCENE_FARM_VISIT_POPURI_GRAY_MARRIED:
                        deactivateSprites();
                        deactivateGlobalSprites();
                        initializeCutsceneExecutors();
                        deactivateNPCEntities();
                        deactivateAnimalEntities();
                        clearAllItemContextSlots();
                        initializeEntityInstances(1);
                        transitionCutscenes(CUTSCENE_VILLAGE1_RIVAL_WEDDING_POPURI, VILLAGE_1_SPAWN_POINT_1); 
                        break;
                    
                    case CUTSCENE_FARM_VISIT_ELLI_JEFF_MARRIED:      
                        deactivateSprites();
                        deactivateGlobalSprites();
                        initializeCutsceneExecutors();
                        deactivateNPCEntities();
                        deactivateAnimalEntities();
                        clearAllItemContextSlots();
                        initializeEntityInstances(1);
                        transitionCutscenes(CUTSCENE_VILLAGE1_RIVAL_WEDDING_ELLI, VILLAGE_1_SPAWN_POINT_1);
                        break;

                    case CUTSCENE_FARM_VISIT_ANN_CLIFF_MARRIED:      
                        deactivateSprites();
                        deactivateGlobalSprites();
                        initializeCutsceneExecutors();
                        deactivateNPCEntities();
                        deactivateAnimalEntities();
                        clearAllItemContextSlots();
                        initializeEntityInstances(1);
                        transitionCutscenes(CUTSCENE_VILLAGE1_RIVAL_WEDDING_ANN, VILLAGE_1_SPAWN_POINT_1);
                        break;
                    
                    case CUTSCENE_FARM_VISIT_KAREN_KAI_MARRIED:      
                        deactivateSprites();
                        deactivateGlobalSprites();
                        initializeCutsceneExecutors();
                        deactivateNPCEntities();
                        deactivateAnimalEntities();
                        clearAllItemContextSlots();
                        initializeEntityInstances(1);
                        transitionCutscenes(CUTSCENE_VILLAGE1_RIVAL_WEDDING_KAREN, VILLAGE_1_SPAWN_POINT_1);
                        break;

                    case CUTSCENE_FARM_VISIT_MARIA_WIFE_STARRY_NIGHT:
                    case CUTSCENE_FARM_VISIT_ELLI_WIFE_STARRY_NIGHT:
                    case CUTSCENE_FARM_VISIT_ANN_WIFE_STARRY_NIGHT:
                        deactivateSprites();
                        deactivateGlobalSprites();
                        initializeCutsceneExecutors();
                        deactivateNPCEntities();
                        deactivateAnimalEntities();
                        clearAllItemContextSlots();
                        initializeEntityInstances(1);
                        transitionCutscenes(CUTSCENE_VILLAGE1_STARRY_NIGHT_FESTIVAL, CHURCH_SPAWN_POINT_1);
                        break;
                    
                    case CUTSCENE_FARM_VISIT_POPURI_WIFE_STARRY_NIGHT:      
                        transitionCutscenes(CUTSCENE_MOUNTAIN_STARRY_NIGHT_MOON, MOON_MOUNTAIN_SPAWN_POINT_1);
                        break;
                    
                    case CUTSCENE_FARM_VISIT_KAREN_WIFE_STARRY_NIGHT:      
                        transitionCutscenes(CUTSCENE_BEACH_KAREN_STARRY_NIGHT, BEACH_SPAWN_POINT_1);
                        break;
                    
                    case CUTSCENE_MOUNTAIN_MASTER_CARPENTER_BRIDGE_RECRUIT:
                        transitionCutscenes(CUTSCENE_MOUNTAIN_MASTER_CARPENTER_BRIDGE_WORK, TOP_OF_MOUNTAIN_1_SPAWN_POINT_1);
                        break;

                    case CUTSCENE_MOUNTAIN_MASTER_CARPENTER_BRIDGE_WORK:     
                        transitionCutscenes(CUTSCENE_MOUNTAIN_MASTER_CARPENTER_BRIDGE_PAY, MOUNTAIN_1_SPAWN_POINT_1);
                        break;
                    
                    case CUTSCENE_MOUNTAIN_MASTER_CARPENTER_HOT_SPRINGS_RECRUIT:     
                        transitionCutscenes(CUTSCENE_MOUNTAIN_MASTER_CARPENTER_HOT_SPRINGS_WORK, TOP_OF_MOUNTAIN_1_SPAWN_POINT_1);
                        break;
                    
                    case CUTSCENE_MOUNTAIN_MASTER_CARPENTER_HOT_SPRINGS_WORK:     
                        transitionCutscenes(CUTSCENE_MOUNTAIN_MASTER_CARPENTER_HOT_SPRINGS_COMPLETE, MOUNTAIN_1_SPAWN_POINT_1);
                        break;
                    
                    case CUTSCENE_MOUNTAIN_HARVEST_GODDESS_GRAPEVINE_OFFERING:     
                        transitionCutscenes(CUTSCENE_MOUNTAIN_HARVEST_GODDESS_GRAPEVINE_AWAKEN, VINEYARD_SPAWN_POINT_1);
                        break;

                    case CUTSCENE_MOUNTAIN_MINE_2_DIG_PROMPT:     
                        // set cutscene wild card/score variables
                        D_801886D2 = getRandomNumberInRange(0, 9);
                        D_80180710 = getRandomNumberInRange(0, 9);
                        clearForagableObjects(MINE_2);
                        transitionCutscenes(CUTSCENE_MOUNTAIN_MINE_2_DIG_EVENT, MINE_2_SPAWN_POINT_1);
                        break;

                    case CUTSCENE_MOUNTAIN_MINE_2_DIG_EVENT:
                    case CUTSCENE_MOUNTAIN_MINE_2_DIG_REENTRY:
                        
                        if (gHour >= 17 || gPlayer.currentStamina < 2) {
                            gCutsceneIndex = CUTSCENE_MOUNTAIN_MINE_2_DIG_EXIT;
                            tempSpawnPoint = CAVE_SPAWN_POINT_3;
                        } else {
    
                            if (D_80189824 < 10) {
                                // set cutscene wild card/score variables
                                D_801886D2 = getRandomNumberInRange(0, 9);
                                D_80180710 = getRandomNumberInRange(0, 9);
                            } else {
                                D_80180710 = 0xFF;
                                D_801886D2 = 0xFF;
                            }

                            gCutsceneIndex = CUTSCENE_MOUNTAIN_MINE_2_DIG_REENTRY;
                            tempSpawnPoint = MINE_2_SPAWN_POINT_1;
                            
                            clearForagableObjects(MINE_2);
                 
                        }
                        
                        setSpawnPoint(tempSpawnPoint);
                        loadCutscene(FALSE);
                        loadMapAtSpawnPoint(tempSpawnPoint);

                        break;

                    case CUTSCENE_VILLAGE1_POPURI_BLUE_MIST_SEEDS:     
                        transitionCutscenes(CUTSCENE_VILLAGE1_POPURI_MOUNTAIN_FOUNTAIN, POND_SPAWN_POINT_1);
                        break;
                    
                    case CUTSCENE_VILLAGE1_ELLI_CONFESSION_1:     
                        transitionCutscenes(CUTSCENE_VILLAGE1_ELLI_CONFESSION_2, VILLAGE_1_SPAWN_POINT_1);
                        break;

                    case CUTSCENE_VILLAGE1_STARRY_NIGHT_FESTIVAL:

                        setSpawnPoint(SQUARE_SPAWN_POINT_1);
    
                        if (!(gCutsceneCompletionFlags & CUTSCENE_COMPLETION_STARRY_NIGHT_429)) {

                            if (!(gCutsceneCompletionFlags & CUTSCENE_COMPLETION_STARRY_NIGHT_430)) {
                                
                                if ((gCutsceneCompletionFlags & CUTSCENE_COMPLETION_STARRY_NIGHT_431)) {
                                    gCutsceneIndex = CUTSCENE_VILLAGE1_NEW_YEAR_SUNRISE_ANN;
                                } 
                                
                            } else {
                                gCutsceneIndex = CUTSCENE_VILLAGE1_NEW_YEAR_SUNRISE_ELLI;
                            }
                            
                        } else {
                            gCutsceneIndex = CUTSCENE_VILLAGE1_NEW_YEAR_SUNRISE;
    
                        }
                        
                        loadCutscene(FALSE);
                        loadMapAtSpawnPoint(SQUARE_SPAWN_POINT_1);
                        break;

                    case CUTSCENE_VILLAGE1_POPURI_SICK_TRANSITION:
                    case CUTSCENE_VILLAGE1_POPURI_REPAIR_MARRIAGE_LILLIA_ENTRY:
                        setSpawnPoint(POPURI_ROOM_SPAWN_POINT_1);
                        if (gCutsceneIndex == CUTSCENE_VILLAGE1_POPURI_SICK_TRANSITION) {
                            gCutsceneIndex = CUTSCENE_VILLAGE1_POPURI_COLD_VISIT;
                        } else {
                            gCutsceneIndex = CUTSCENE_VILLAGE1_POPURI_REPAIR_MARRIAGE;
                        }
                        loadCutscene(FALSE);
                        loadMapAtSpawnPoint(POPURI_ROOM_SPAWN_POINT_1);
                        break;
                    
                    case CUTSCENE_VILLAGE1_ELLI_SICK_JEFF_ENTRY:
                    case CUTSCENE_VILLAGE1_ELLI_REPAIR_MARRIAGE_JEFF_ENTRY:
                        setSpawnPoint(ELLI_ROOM_SPAWN_POINT_1);
                        if (gCutsceneIndex == CUTSCENE_VILLAGE1_ELLI_SICK_JEFF_ENTRY) {
                            gCutsceneIndex = CUTSCENE_VILLAGE1_ELLI_SICK_VISIT;
                        } else {
                            gCutsceneIndex = CUTSCENE_VILLAGE1_ELLI_REPAIR_MARRIAGE;
                        }
                        loadCutscene(FALSE);
                        loadMapAtSpawnPoint(ELLI_ROOM_SPAWN_POINT_1);
                        break;

                    case CUTSCENE_VILLAGE2_MARIA_SPRAINED_ANKLE:
                        setSpawnPoint(VILLAGE_2_SPAWN_POINT_1);
                        gCutsceneIndex = CUTSCENE_VILLAGE2_MARIA_SPRAINED_ANKLE_ROOM;
                        loadCutscene(FALSE);
                        loadMapAtSpawnPoint(VILLAGE_2_SPAWN_POINT_1);
                        break;
                    
                    case CUTSCENE_VILLAGE2_MARIA_SPRAINED_ANKLE_ROOM:
                        setSpawnPoint(VILLAGE_2_SPAWN_POINT_4);
                        loadMapAtSpawnPoint(VILLAGE_2_SPAWN_POINT_4);
                        break;
                    
                    case CUTSCENE_VILLAGE2_MARIA_CHILDRENS_BOOK:
                        transitionCutscenes(CUTSCENE_VILLAGE2_MARIA_CHILDRENS_BOOK_ROOM, VILLAGE_2_SPAWN_POINT_1);
                        break;   

                    case CUTSCENE_VILLAGE2_MARIA_SICK_TRANSITION:
                    case CUTSCENE_VILLAGE2_MARIA_REPAIR_MARRIAGE_TRANSITION:
                        setSpawnPoint(MARIA_ROOM_SPAWN_POINT_1);
                        if (gCutsceneIndex == CUTSCENE_VILLAGE2_MARIA_SICK_TRANSITION) {
                            gCutsceneIndex = CUTSCENE_VILLAGE2_MARIA_SICK_VISIT_ROOM;
                        } else {
                            gCutsceneIndex = CUTSCENE_VILLAGE2_MARIA_REPAIR_MARRIAGE_ROOM;
                        }
                        loadCutscene(FALSE);
                        loadMapAtSpawnPoint(MARIA_ROOM_SPAWN_POINT_1);
                        break;

                    case CUTSCENE_RANCH_ANN_SICK_VISIT_TRANSITION:
                    case CUTSCENE_RANCH_ANN_REPAIR_MARRIAGE_TRANSITION:
                        setSpawnPoint(ANN_ROOM_SPAWN_POINT_1);
                        if (gCutsceneIndex == CUTSCENE_RANCH_ANN_SICK_VISIT_TRANSITION) {
                            gCutsceneIndex = CUTSCENE_RANCH_ANN_SICK_VISIT_ROOM;
                        } else {
                            gCutsceneIndex = CUTSCENE_RANCH_ANN_REPAIR_MARRIAGE_ROOM;
                        }
                        loadCutscene(FALSE);
                        loadMapAtSpawnPoint(ANN_ROOM_SPAWN_POINT_1);
                        break;
                    
                    case CUTSCENE_VINEYARD_KAREN_CELLAR_OFFER:
                        gCutsceneIndex = CUTSCENE_VINEYARD_KAREN_CELLAR;
                        loadCutscene(FALSE);
                        loadMapAtSpawnPoint(VINEYARD_CELLAR_BASEMENT_SPAWN_POINT_1);
                        break;

                    case CUTSCENE_VINEYARD_KAREN_CELLAR:
                        transitionCutscenes(CUTSCENE_VINEYARD_CELLAR_HIDDEN, VINEYARD_CELLAR_SPAWN_POINT_1);
                        break;

                    case CUTSCENE_VINEYARD_CELLAR_HIDDEN:
                    case CUTSCENE_VINEYARD_CELLAR_HIDDEN_REENTRY:
                        clearDailyEventBit(KAREN_YELLOW_HEART_EVENT_DAILY);
                        transitionCutscenes(CUTSCENE_VINEYARD_KAREN_RETURN, VINEYARD_SPAWN_POINT_1);
                        break;
                    
                    case CUTSCENE_VINEYARD_KAREN_SICK_TRANSITION:
                    case CUTSCENE_VINEYARD_KAREN_REPAIR_MARRIAGE_TRANSITION:
                        setSpawnPoint(KAREN_ROOM_SPAWN_POINT_1);
                        if (gCutsceneIndex == CUTSCENE_VINEYARD_KAREN_SICK_TRANSITION) {
                            gCutsceneIndex = CUTSCENE_VINEYARD_KAREN_SICK_VISIT_ROOM;
                        } else {
                            gCutsceneIndex = CUTSCENE_VINEYARD_KAREN_REPAIR_MARRIAGE_ROOM;
                        }
                        loadCutscene(FALSE);
                        loadMapAtSpawnPoint(KAREN_ROOM_SPAWN_POINT_1);
                        break;
                    
                    case CUTSCENE_ROAD_ANN_SPRAINED_ANKLE:
                        setSpawnPoint(RANCH_SPAWN_POINT_3);
                        loadMapAtSpawnPoint(RANCH_SPAWN_POINT_3);
                        break;
                    
                    case 900:
                    case CUTSCENE_HORSE_RACE_STARTING_ANNOUNCEMENT:
                         
                        if (gHour >= 17) {
                            transitionCutscenes(CUTSCENE_HORSE_RACE_END_OF_DAY, SQUARE_SPAWN_POINT_1);
                        } else {

                            if (gRacingContext.playerRaceNumber == gRacingContext.currentRaceIndex) {
                                gCutsceneIndex = CUTSCENE_HORSE_RACE_PLAYER;
                                // set cutscene wild card/score variables
                                D_80189824 = horseInfo.affection;
                                D_801886D2 = (horseInfo.affection / 3) * 2;
                                D_80180710 = horseInfo.affection / 3;
                            } else {
                                gCutsceneIndex = CUTSCENE_HORSE_RACE_RACE;
                            }

                            loadCutscene(FALSE);
                            
                        }
                        
                        break;

                    case CUTSCENE_HORSE_RACE_RACE:
                    case CUTSCENE_HORSE_RACE_PLAYER:
                        break;

                    case CUTSCENE_VEGETABLE_FESTIVAL_VILLAGE:
                        deactivateSprites();
                        deactivateGlobalSprites();
                        initializeCutsceneExecutors();
                        deactivateNPCEntities();
                        deactivateAnimalEntities();
                        clearAllItemContextSlots();
                        initializeEntityInstances(2);
                        transitionCutscenes(CUTSCENE_VEGETABLE_FESTIVAL_SQUARE, SQUARE_SPAWN_POINT_1);
                        break;

                    case CUTSCENE_DOG_RACE_SQUARE:
                    case CUTSCENE_DOG_RACE_AFTER_RACE:
                        if (gHour >= 17) {
                            transitionCutscenes(CUTSCENE_DOG_RACE_POST_RACE, SQUARE_SPAWN_POINT_1);
                        } else {
                            if (gRacingContext.playerRaceNumber == gRacingContext.currentRaceIndex) {
                                raceStateFlags |= RACE_STATE_PLAYER_IN_RACE;
                            } else {
                                raceStateFlags &= ~RACE_STATE_PLAYER_IN_RACE;
                            }
                            if (gRacingContext.rivalEntryRaceIndex == gRacingContext.currentRaceIndex) {
                                raceStateFlags |= RACE_STATE_RIVAL_IN_RACE;
                            } else {
                                raceStateFlags &= ~RACE_STATE_RIVAL_IN_RACE;
                            }
                            
                            gCutsceneIndex = CUTSCENE_DOG_RACE_RACE;
                            loadCutscene(FALSE);
                            
                        }
                        
                        break;

                    case CUTSCENE_DOG_RACE_RACE:
                        setupPlayerEntity(RACE_TRACK_SPAWN_POINT_1, 1);
                        handlePlayerAnimation();
                        setEntitiesRGBA(0, 0, 0, 0);
                        setSpawnPoint(RACE_TRACK_SPAWN_POINT_1);
                        gCutsceneIndex = CUTSCENE_DOG_RACE_AFTER_RACE;
                        loadCutscene(FALSE);
                        loadMapAtSpawnPoint(RACE_TRACK_SPAWN_POINT_1);
                        break;

                    case CUTSCENE_FUNERAL:
                        transitionCutscenes(CUTSCENE_FUNERAL_ROAD, FARM_SPAWN_POINT_1);
                        break;

                    case CUTSCENE_FUNERAL_ROAD:             
                        transitionCutscenes(CUTSCENE_FUNERAL_EPILOGUE, ROAD_SPAWN_POINT_1);
                        break;

                    case CUTSCENE_MAYOR_VILLAGE_TOUR:
                        transitionCutscenes(CUTSCENE_MAYOR_VILLAGE_TOUR_2, VILLAGE_2_SPAWN_POINT_1);
                        break;
                    
                    case CUTSCENE_MAYOR_VILLAGE_TOUR_2:            
                        transitionCutscenes(CUTSCENE_MAYOR_VILLAGE_TOUR_3, FARM_SPAWN_POINT_1);
                        gCutsceneFlags &= ~CUTSCENE_DEACTIVATE_ANIMALS_ON_COMPLETION;
                        gCutsceneFlags |= CUTSCENE_REINIT_ANIMALS_ON_COMPLETION;
                        break;

                    case CUTSCENE_DEMO_2:
                        break;

                    case CUTSCENE_EVALUATION:             
                        transitionCutscenes(CUTSCENE_EVALUATION_MOUNTAIN, MOUNTAIN_1_SPAWN_POINT_1);
                        break;

                    case CUTSCENE_EVALUATION_MOUNTAIN:             
                        transitionCutscenes(CUTSCENE_EVALUATION_HOUSE, HOUSE_SPAWN_POINT_1);
                        break;

                    case CUTSCENE_EVALUATION_HOUSE:             
                        transitionCutscenes(CUTSCENE_CREDITS, FARM_SPAWN_POINT_1);
                        break;

                    case CUTSCENE_HOW_TO_PLAY_INTRO:     
                        
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
                            gCutsceneIndex = CUTSCENE_HOW_TO_PLAY_RUCKSACK;

                        } else if (D_80189824 == 1) {

                            gSeason = SPRING;

                            tempSpawnPoint = FARM_SPAWN_POINT_1;
                            
                            switch (D_801886D2) {

                                case 0:
                                    memcpy(farmFieldTiles, D_80115210, FIELD_HEIGHT * FIELD_WIDTH); 
                                    gCutsceneIndex = CUTSCENE_HOW_TO_PLAY_CLEARING;
                                    break;

                                case 1:
                                    memcpy(farmFieldTiles, D_801153F0, FIELD_HEIGHT * FIELD_WIDTH); 
                                    gCutsceneIndex = CUTSCENE_HOW_TO_PLAY_TILLING;
                                    break;

                                case 2:
                                    memcpy(farmFieldTiles, D_801155D0, FIELD_HEIGHT * FIELD_WIDTH); 
                                    gCutsceneIndex = CUTSCENE_HOW_TO_PLAY_SEEDS;
                                    break;

                                case 3:
                                    memcpy(farmFieldTiles, D_801157B0, FIELD_HEIGHT * FIELD_WIDTH); 
                                    gCutsceneIndex = CUTSCENE_HOW_TO_PLAY_WATERING;
                                    break;
                                
                                case 4:
                                    memcpy(farmFieldTiles, D_80115990, FIELD_HEIGHT * FIELD_WIDTH); 
                                    gCutsceneIndex = CUTSCENE_HOW_TO_PLAY_HARVEST;
                                    break;
                                
                            }

                        } else {

                            gSeason = SUMMER;
                            tempSpawnPoint = 0;
                            
                            switch (D_801886D2) {

                                case 0:
                                    memcpy(farmFieldTiles, D_80115B70, FIELD_HEIGHT * FIELD_WIDTH); 
                                    gCutsceneIndex = CUTSCENE_HOW_TO_PLAY_CHICKENS;
                                    break;

                                case 1:
                                    memcpy(farmFieldTiles, D_80115D50, FIELD_HEIGHT * FIELD_WIDTH); 
                                    gCutsceneIndex = CUTSCENE_HOW_TO_PLAY_COWS;
                                    break;

                                case 2:
                                    memcpy(farmFieldTiles, D_80115F30, FIELD_HEIGHT * FIELD_WIDTH); 
                                    tempSpawnPoint = BARN_SPAWN_POINT_1;
                                    gCutsceneIndex = CUTSCENE_HOW_TO_PLAY_SHEEP;
                                    break;

                                case 3:
                                    memcpy(farmFieldTiles, D_80116110, FIELD_HEIGHT * FIELD_WIDTH); 
                                    gCutsceneIndex = CUTSCENE_HOW_TO_PLAY_HORSE;
                                    break;
                                
                                case 4:
                                    memcpy(farmFieldTiles, D_80116110, FIELD_HEIGHT * FIELD_WIDTH); 
                                    gCutsceneIndex = CUTSCENE_HOW_TO_PLAY_DOG;
                                    break;
                                
                            }
                            
                        }

                        setSpawnPoint(tempSpawnPoint);
                        loadCutscene(FALSE);
                        loadMapAtSpawnPoint(tempSpawnPoint);

                        break;

                    case CUTSCENE_HOW_TO_PLAY_RUCKSACK ... CUTSCENE_HOW_TO_PLAY_DOG:
                        gCutsceneIndex = CUTSCENE_HOW_TO_PLAY_INTRO;
                        loadCutscene(FALSE); 
                        break;

                    default:
                        gCutsceneIndex = 0xFFFF;
                        break;
                    
                }

                if (gCutsceneIndex != 0xFFFF) {
                    previousLightingRGBA.a = 0.0f;
                    previousLightingRGBA.b = 0.0f;
                    previousLightingRGBA.g = 0.0f;
                    previousLightingRGBA.r = 0.0f;
                    setMainLoopCallbackFunctionIndex(SET_AUDIO_AND_LIGHTING);
                }
                
            }
            
            if (((gCutsceneIndex == CUTSCENE_HARVEST_FESTIVAL) || (gCutsceneIndex == CUTSCENE_NEW_YEAR_FESTIVAL))) {
                setEntitySpriteDimensions(ENTITY_ASSET_SHIPPER, 12, 12);
                setEntitySpriteDimensions(ENTITY_ASSET_ASSISTANT_CARPENTER, 12, 12);
            }

            gCutsceneCompletionFlags = 0;
            
        }

        // horse/dog race flags
        if (gCutsceneCompletionFlags & CUTSCENE_COMPLETION_PUBLISH_RACER_NAMES) {
            setGameVariableString(40, gRacingContext.oddsAndNames.racerNameLookup[gRaceFinishOrder[0]], 6);
            setGameVariableString(41, gRacingContext.oddsAndNames.racerNameLookup[gRaceFinishOrder[1]], 6);
            gCutsceneCompletionFlags &= ~CUTSCENE_COMPLETION_PUBLISH_RACER_NAMES;
        }

        // sprite color flags
        if (gCutsceneCompletionFlags & CUTSCENE_COMPLETION_SPRITE_98_PALETTE_0) {
            setSpritePaletteIndex(98, 0);
            gCutsceneCompletionFlags &= ~CUTSCENE_COMPLETION_SPRITE_98_PALETTE_0;
        }
        if (gCutsceneCompletionFlags & CUTSCENE_COMPLETION_SPRITE_98_PALETTE_1) {
            setSpritePaletteIndex(98, 1);
            gCutsceneCompletionFlags &= ~CUTSCENE_COMPLETION_SPRITE_98_PALETTE_1;
        }

        if (gCutsceneCompletionFlags & CUTSCENE_COMPLETION_LOAD_FONT_PALETTE_3) {

            u32 ptr1 = (u32)&_fontPalette3SegmentRomStart;
            u32 ptr2 =  (u32)&_fontPalette3SegmentRomEnd;
            
            // fontPalette3
            nuPiReadRom(ptr1, FONT_PALETTE_1_BUFFER, ptr2 - ptr1);
            setMessageBoxFont(0, 14, 14, FONT_TEXTURE_BUFFER, FONT_PALETTE_1_BUFFER);
            setMessageBoxInterpolationWithFlags(0, 1, 1);
            setMessageBoxSfx(0, 0xFF, 0xFF, 0xFF);
            gCutsceneCompletionFlags &= ~CUTSCENE_COMPLETION_LOAD_FONT_PALETTE_3;
        }

        // map flags

        if (gCutsceneCompletionFlags & CUTSCENE_COMPLETION_LOAD_FARM_MAP) {
            setSpawnPoint(FARM_SPAWN_POINT_1);
            // prep map
            loadMapAtSpawnPoint(0);
            setMapControllerRGBA(0, 255, 255, 255, 255);
            gCutsceneCompletionFlags &= ~CUTSCENE_COMPLETION_LOAD_FARM_MAP;
        }

        // feed chickens
        if (gCutsceneCompletionFlags & CUTSCENE_COMPLETION_CHICKEN_FEED_0) {
            setChickenFeedSprite(0);
            gCutsceneCompletionFlags &= ~CUTSCENE_COMPLETION_CHICKEN_FEED_0;
        }
        if (gCutsceneCompletionFlags & CUTSCENE_COMPLETION_CHICKEN_FEED_1) {
            setChickenFeedSprite(1);
            gCutsceneCompletionFlags &= ~CUTSCENE_COMPLETION_CHICKEN_FEED_1;
        }
        if (gCutsceneCompletionFlags & CUTSCENE_COMPLETION_CHICKEN_FEED_2) {
            setChickenFeedSprite(2);
            gCutsceneCompletionFlags &= ~CUTSCENE_COMPLETION_CHICKEN_FEED_2;
        }

        if (gCutsceneCompletionFlags & CUTSCENE_COMPLETION_LOAD_COOP_MAP) {
            setSpawnPoint(COOP_SPAWN_POINT_1);
            loadMapAtSpawnPoint(COOP_SPAWN_POINT_1);
            setMapControllerRGBA(0, 0, 0, 0, 0);
            gCutsceneCompletionFlags &= ~CUTSCENE_COMPLETION_LOAD_COOP_MAP;
        }

        if (gCutsceneCompletionFlags & CUTSCENE_COMPLETION_LOAD_BARN_MAP) {
            setSpawnPoint(BARN_SPAWN_POINT_1);
            // prep map
            loadMapAtSpawnPoint(BARN_SPAWN_POINT_1);
            setMapControllerRGBA(0, 0, 0, 0, 0);
            gCutsceneCompletionFlags &= ~CUTSCENE_COMPLETION_LOAD_BARN_MAP;
        }

        if (gCutsceneCompletionFlags & CUTSCENE_COMPLETION_BARN_FODDER_0) {
            // map object
            setBarnFodderSprite(0);
            gCutsceneCompletionFlags &= ~CUTSCENE_COMPLETION_BARN_FODDER_0;
        }

        if (gCutsceneCompletionFlags & CUTSCENE_COMPLETION_BARN_FODDER_2) {
            setBarnFodderSprite(2);
            gCutsceneCompletionFlags &= ~CUTSCENE_COMPLETION_BARN_FODDER_2;
        }

        if (gCutsceneCompletionFlags & CUTSCENE_COMPLETION_MAP_OBJECT_ANIM) {
            // map objects
            setMapObjectAnimation(0, 0, 0x13);
            gCutsceneCompletionFlags &= ~CUTSCENE_COMPLETION_MAP_OBJECT_ANIM;
        }

        // mining
        // might be its own inline func
        if ((gCutsceneCompletionFlags & CUTSCENE_COMPLETION_MINING_STRIKE) && (151 < entities[ENTITY_PLAYER].animationIndices.animationIndex && entities[ENTITY_PLAYER].animationIndices.animationIndex < 176)) {

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
            
            gCutsceneCompletionFlags &= ~CUTSCENE_COMPLETION_MINING_STRIKE;
        
        }

        if ((gCutsceneCompletionFlags & CUTSCENE_COMPLETION_REFRESH_HELD_ITEM) && gItemBeingHeld < 252) {
            initializePlayerHeldItem();
        }

        gCutsceneCompletionFlags &= ~CUTSCENE_COMPLETION_REFRESH_HELD_ITEM;

        if (gCutsceneCompletionFlags & CUTSCENE_COMPLETION_SUSPEND_TIME) {
            setDailyEventBit(SUSPEND_TIME_DURING_ANIMATION);
        } else {
            clearDailyEventBit(SUSPEND_TIME_DURING_ANIMATION);
        }

        if (gCutsceneCompletionFlags & CUTSCENE_COMPLETION_BLOCK_PAUSE) {
            setDailyEventBit(BLOCK_PAUSE_SCREEN);
        } else {
            clearDailyEventBit(BLOCK_PAUSE_SCREEN);
        }

        if (gCutsceneCompletionFlags & CUTSCENE_COMPLETION_BLOCK_BUTTONS) {
            setDailyEventBit(BLOCK_BUTTON_USAGE);
        } else {
            clearDailyEventBit(BLOCK_BUTTON_USAGE);
        }

        if (gCutsceneCompletionFlags & CUTSCENE_COMPLETION_BLOCK_DIARY) {
            setDailyEventBit(DIARY_BLOCKED);
        } else {
            clearDailyEventBit(DIARY_BLOCKED);
        }

        if (gCutsceneCompletionFlags & CUTSCENE_COMPLETION_DAY_CONSUMING) {
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

        if (gCutsceneCompletionFlags & CUTSCENE_COMPLETION_CLEAR_HELD_ITEM) {
            clearHeldItemsAtEndOfDay();
            gCutsceneCompletionFlags &= ~CUTSCENE_COMPLETION_CLEAR_HELD_ITEM;
        }

        if (gCutsceneFlags & 8) {
            initializeAnimalEntities();
            gCutsceneFlags &= ~CUTSCENE_REINIT_ANIMALS_ON_COMPLETION;
        }

        if (gCutsceneFlags & 0x10) {
            deactivateAnimalsAfterCutscene();
            gCutsceneFlags &= ~CUTSCENE_DEACTIVATE_ANIMALS_ON_COMPLETION;
        }

        if (gCutsceneCompletionFlags & CUTSCENE_COMPLETION_REINIT_ANIMALS) {
            initializeAnimalEntities();
            gCutsceneCompletionFlags &= ~CUTSCENE_COMPLETION_REINIT_ANIMALS;
        }

        if (gCutsceneCompletionFlags & CUTSCENE_COMPLETION_DEACTIVATE_ANIMALS) {
            deactivateAnimalsAfterCutscene();
            gCutsceneCompletionFlags &= ~CUTSCENE_COMPLETION_DEACTIVATE_ANIMALS;
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

void loadCutscene(bool morningVisit) {
    
    u16 bytecodeSegmentIndex;

    if (gCutsceneIndex >= 1600) {

        gCameraRotationOffset = 0;
        setInitialMapRotation(MAIN_MAP_INDEX, MAP_ROTATION_SW);
        bytecodeSegmentIndex = CUTSCENE_BANK_HOW_TO_PLAY;


    } else if (gCutsceneIndex >= 1550) {

        gCameraRotationOffset = 0;
        setInitialMapRotation(MAIN_MAP_INDEX, MAP_ROTATION_SW);
        bytecodeSegmentIndex = CUTSCENE_BANK_ENDING;


    } else if (gCutsceneIndex >= 1500) {

        gCameraRotationOffset = 0;
        setInitialMapRotation(MAIN_MAP_INDEX, MAP_ROTATION_SW);

        memcpy(farmFieldTiles, demoCutsceneFieldTiles, FIELD_HEIGHT * FIELD_WIDTH);

        bytecodeSegmentIndex = CUTSCENE_BANK_DEMOS;

    } else if (gCutsceneIndex >= 1450) {

        gCameraRotationOffset = 0;
        setInitialMapRotation(MAIN_MAP_INDEX, MAP_ROTATION_SW);


        gSeason = SPRING;

        memcpy(farmFieldTiles, defaultFarmFieldTiles, FIELD_HEIGHT * FIELD_WIDTH);

        bytecodeSegmentIndex = CUTSCENE_BANK_INTRO;

    } else if (gCutsceneIndex >= 1400) {

        bytecodeSegmentIndex = CUTSCENE_BANK_NEW_YEAR_FESTIVAL;
        setEntitySpriteDimensions(ENTITY_ASSET_SHIPPER, 12, 24);
        setEntitySpriteDimensions(ENTITY_ASSET_ASSISTANT_CARPENTER, 12, 24);

    } else if (gCutsceneIndex >= 1350) {
        bytecodeSegmentIndex = CUTSCENE_BANK_SPIRIT_FESTIVAL;
    } else if (gCutsceneIndex >= 1300) {
        bytecodeSegmentIndex = CUTSCENE_BANK_DOG_RACE;
    } else if (gCutsceneIndex >= 1250) {
        bytecodeSegmentIndex = CUTSCENE_BANK_EGG_FESTIVAL;
    } else if (gCutsceneIndex >= 1200) {

        bytecodeSegmentIndex = CUTSCENE_BANK_HARVEST_FESTIVAL;
        setEntitySpriteDimensions(ENTITY_ASSET_SHIPPER, 12, 24);
        setEntitySpriteDimensions(ENTITY_ASSET_ASSISTANT_CARPENTER, 12, 24);

    } else if (gCutsceneIndex >= 1150) {
        bytecodeSegmentIndex = CUTSCENE_BANK_COW_FESTIVAL;
    } else if (gCutsceneIndex >= 1100) {
        bytecodeSegmentIndex = CUTSCENE_BANK_SEA_FESTIVAL;
    } else if (gCutsceneIndex >= 1050) {
        bytecodeSegmentIndex = CUTSCENE_BANK_FIREFLY_FESTIVAL;
    } else if (gCutsceneIndex >= 1000) {
        bytecodeSegmentIndex = CUTSCENE_BANK_VEGETABLE_FESTIVAL;
    } else if (gCutsceneIndex >= 950) {
        bytecodeSegmentIndex = CUTSCENE_BANK_FLOWER_FESTIVAL;
    } else if (gCutsceneIndex >= 900) {
        bytecodeSegmentIndex = CUTSCENE_BANK_HORSE_RACE;
    } else if (gCutsceneIndex >= 850) {
        bytecodeSegmentIndex = CUTSCENE_BANK_SOWING_FESTIVAL;
    } else if (gCutsceneIndex >= 800) {
        bytecodeSegmentIndex = CUTSCENE_BANK_ROAD;
    } else if (gCutsceneIndex >= 750) {
        bytecodeSegmentIndex = CUTSCENE_BANK_BEACH;
    } else if (gCutsceneIndex >= 700) {
        bytecodeSegmentIndex = CUTSCENE_BANK_VINEYARD;
    } else if (gCutsceneIndex >= 650) {
        bytecodeSegmentIndex = CUTSCENE_BANK_RANCH;
    } else if (gCutsceneIndex >= 600) {
        bytecodeSegmentIndex = CUTSCENE_BANK_FIREWORKS_FESTIVAL;
    } else if (gCutsceneIndex >= 500) {
        bytecodeSegmentIndex = CUTSCENE_BANK_VILLAGE_2;
    } else if (gCutsceneIndex >= 400) {
        bytecodeSegmentIndex = CUTSCENE_BANK_VILLAGE_1;
    } else if (gCutsceneIndex >= 300) {
        bytecodeSegmentIndex = CUTSCENE_BANK_MOUNTAIN;
    } else if (gCutsceneIndex >= 100) {

        if (morningVisit) {
            bytecodeSegmentIndex = CUTSCENE_BANK_FARM_VISIT_MORNING;
        } else  {

            bytecodeSegmentIndex = CUTSCENE_BANK_FARM_VISIT_WORKING;

            switch (gCutsceneIndex) {

                case CUTSCENE_FARM_VISIT_RICK_TRANSPORTER_VISIT:
                    gCameraRotationOffset = 0;
                    setInitialMapRotation(MAIN_MAP_INDEX, MAP_ROTATION_SW);
                    break;
                case CUTSCENE_FARM_VISIT_TOURIST_COUPLE_FLOWER_VISIT ... CUTSCENE_FARM_VISIT_MISS_MANA_PICKUP_COWS:
                    gCameraRotationOffset = 0;
                    setInitialMapRotation(MAIN_MAP_INDEX, MAP_ROTATION_SW);
                    break;
                default:
                    break;

            }

        }

    } else {

        bytecodeSegmentIndex = CUTSCENE_BANK_HOUSE;

        if (gCutsceneIndex == CUTSCENE_HOUSE_CLIFF_AND_ANN_INTRODUCTION) {
            gCameraRotationOffset = 0;
            setInitialMapRotation(MAIN_MAP_INDEX, MAP_ROTATION_SW);
        }


    }

    initializeCutscene(bytecodeSegmentIndex);
    
    switch (gCutsceneIndex) {
        case CUTSCENE_MOUNTAIN_STARRY_NIGHT_MOON:
        case CUTSCENE_VILLAGE1_MARIA_WEDDING ... CUTSCENE_VILLAGE1_KAREN_WEDDING:
        case CUTSCENE_VILLAGE1_STARRY_NIGHT_FESTIVAL:
        case CUTSCENE_VILLAGE2_ANIMAL_FUNERAL:
        case CUTSCENE_BEACH_KAREN_STARRY_NIGHT:
        case CUTSCENE_SOWING_FESTIVAL_POTION_SHOP_DEALER ... CUTSCENE_SOWING_FESTIVAL_BACHELOR:
        case CUTSCENE_HORSE_RACE_SQUARE: 
        case CUTSCENE_FLOWER_FESTIVAL:
        case CUTSCENE_FIREFLY_FESTIVAL:
        case CUTSCENE_SEA_FESTIVAL:
        case CUTSCENE_COW_FESTIVAL:
        case CUTSCENE_HARVEST_FESTIVAL:
        case CUTSCENE_EGG_FESTIVAL:
        case CUTSCENE_DOG_RACE_SQUARE:
        case CUTSCENE_SPIRIT_FESTIVAL_HARVEST_KING ... CUTSCENE_SPIRIT_FESTIVAL_REPEAT:
        case CUTSCENE_NEW_YEAR_FESTIVAL:
        case CUTSCENE_FUNERAL:
        case CUTSCENE_OPENING_LOGOS:
        case CUTSCENE_DEMO_1: 
        case CUTSCENE_DEMO_2:
        case CUTSCENE_DEMO_3:
        case CUTSCENE_EVALUATION:
        case CUTSCENE_HOW_TO_PLAY_INTRO:
            setDailyEventBit(CUTSCENE_AUDIO_OVERRIDE);
            break;
    }
    
}